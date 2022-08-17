#include "scene_reader.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../shader_manager/shader_manager.hpp"
#include "../scene_manager/scene_objects/drawable.hpp"
#include "../scene_manager/drawable_properties/model_transformation.hpp"
#include "structs/drawable_data.hpp"
#include "../scene_manager/scene_objects/camera.hpp"
#include "../scene_manager/scene_manager.hpp"
#include "transformation_node.hpp"

#include "../vulkan_renderer/vre_renderer/vre_commandbuffer.hpp"

namespace vre
{
	SceneReader::SceneReader(std::shared_ptr<Scene> scene, std::shared_ptr<RenderTarget> renderTarget)
	{
		m_scene = scene;

		m_renderTarget = renderTarget;
		m_listOfRenderLists = std::make_shared<std::list<std::shared_ptr<Renderer::VreRenderer::VreRenderList>>>();
		
		if (renderTarget == nullptr)
		{
			// no config means we are adapting the swapchain configuration
			m_isMainScene = true;
			m_renderList = std::make_shared<Renderer::VreRenderer::VreRenderList>();
		}
		else
		{
			auto config = renderTarget->GetConfiguration();
			m_renderList = std::make_shared<Renderer::VreRenderer::VreRenderList>(config->mWidth, config->mHeight, renderTarget->GetImageViews(), config->mFormat, config->mClearColorValues);
		}

		m_renderList->SetSceneReader(this);
		scene->AddNodeAddedCallback(getNodeAddedCallback());
		scene->AddNodeRemovedCallback(getNodeRemovedCallback());
	}

	SceneReader::~SceneReader()
	{
		FreeAllocatedMemory();

		// if render target is != nullptr this means the scene reader belongs to a render target
		// since render targets can be used multiple times in a scene, but don't need multiple scene readers,
		// each scene reader is stored in a static map and has to be removed as well from this map upon destruction.
		if (m_renderTarget != nullptr)
		{
			auto renderTargetAddress = &(*m_renderTarget);
			auto it = s_mapRenderTargetToSceneReader.find(renderTargetAddress);
			s_mapRenderTargetToSceneReader.erase(it);
		}
	}

	void SceneReader::ReadComplete()
	{
		// traverse scenegraph and create a copy for the scene reader to store transformations
		createTransformationGraph();

		// sort all drawables into transparent and non transparent objects
		// check if a render target exists
		traverseTransformGraph(m_transformationRootNode, sortDrawables());

		// create vulkan buffers
		// compute transformations
		allocateSceneMemory();

		// sort drawables into render order
		createRenderList();

		// initialize render list if drawer already exists
		if (Renderer::VreRenderer::VreRenderer::GetInstance()->GetVreDrawer() != nullptr)
		{
			m_renderList->Initialize();
		}

		m_listOfRenderLists->push_back(m_renderList);
	}

	void SceneReader::UpdateTransparentDrawableOrder()
	{
		// called when the camera moved for example, so it could be the order has changed
		// TODO: instead of just clearing transparent drawables
		// it could be checked if the transparent drawable order has actually changed
		// and adjust the drawables.
		
		//TODO: Keep sorted list instead of clearing the complete list for performance increase.
		m_renderList->GetTransparentDrawables()->clear();
		addTransparentDrawablesToRenderList();

		// TODO: true means order changed => rerecording cmd buffer is necessary. true is currently always returned
		m_renderList->GetVreCommandBuffer()->RequestRerecording(false, true);
	}

	void SceneReader::FreeAllocatedMemory()
	{
		for (const auto& drawableData : m_allDrawables)
		{
			freeDrawableData(drawableData);
		}

		m_renderList->GetOpaqueDrawables()->clear();
		m_renderList->GetTransparentDrawables()->clear();

		m_allDrawables.clear();
		m_opaqueDrawables.clear();
		m_transparentDrawables.clear();
	}

	void SceneReader::traverseGraph(const std::shared_ptr<Node> node, std::function<void(const std::shared_ptr<Node> node)> func)
	{		
		for(auto& childNode : *node->GetChildren())
		{
			traverseGraph(childNode, func);
		}

		func(node);
	}

	void SceneReader::traverseTransformGraph(const std::shared_ptr<TransformationNode> node, std::function<void(const std::shared_ptr<TransformationNode> node)> func)
	{
		for (auto& childNode : *node->GetChildren())
		{
			traverseTransformGraph(childNode, func);
		}

		func(node);
	}

	void SceneReader::createTransformationGraph()
	{
		auto rootNode = m_scene.lock()->GetRootNode();
		m_transformationRootNode = std::make_shared<TransformationNode>(rootNode, this);

		addChildsToTransformationNode(rootNode, m_transformationRootNode);
	}

	void SceneReader::addChildsToTransformationNode(std::shared_ptr<Node> node, std::shared_ptr<TransformationNode> transformationNode)
	{
		// add transformation node to scene node
		node->AddTransformationNode(transformationNode);

		// create a transformation node for each child
		for (auto& childNode : *node->GetChildren())
		{
			auto childTransformNode = std::make_shared<TransformationNode>(childNode, this);
			transformationNode->AddChild(childTransformNode);
			addChildsToTransformationNode(childNode, childTransformNode);
		}
	}

	std::function<void(const std::shared_ptr<TransformationNode> node)> SceneReader::sortDrawables()
	{
		return [this](std::shared_ptr<TransformationNode> transformationNode) -> void
		{
			auto node = transformationNode->GetNode().lock();
			auto drawable = node->GetDrawable();
			if (drawable == nullptr)
				return;

			// skip if the drawable has the render target attached thats currently processed.
			// no infinite loops. yet.
			auto renderTargets = drawable->GetPropertyVector<RenderTarget>();
			if (renderTargets != nullptr)
			{
				for (const auto& renderTarget : *renderTargets)
				{
					if (renderTarget == m_renderTarget)
					{
						// do not add this drawable
						return;
					}
				}
			}

			// part 1 - sort drawables
            auto shaderProperty = drawable->GetProperty<ShadersBase>();
            auto meshProperty = drawable->GetProperty<Mesh>();
            checkPtr(shaderProperty, "SceneReader::sortDrawables: the drawable doesn't have shaders attached to it! Shaders are a mandatory property!");
            checkPtr(meshProperty, "SceneReader::sortDrawables: the drawable doesn't have a mesh attached to it! A mesh is mandatory!");

			std::shared_ptr<DrawableData> drawableData;
			if (drawable->IsTransparent())
			{
				drawableData = addDrawableToList(node, m_transparentDrawables);
			}
			else
			{
				drawableData = addDrawableToList(node, m_opaqueDrawables);
			}

			// part 2
			// attach transformation node
			drawableData->mTransformationNode = transformationNode;
			drawableData->mTransformation = transformationNode->GetFinalDrawableTransformation();

			// part 3 - check for attached render targets
			// each render targets results in a new scene & scene reader
			auto renderTargetCount = drawable->GetPropertyCount<RenderTarget>();
			for (size_t i = 0; i < renderTargetCount; i++)
			{
				auto renderTarget = drawable->GetProperty<RenderTarget>(i);

				// the real memory address serves as identifier for the rendertargets
				auto renderTargetAddress = &(*renderTarget);

				// render targets can be used multiple times in a graph
				// only create a scene for them once
				auto iterator = s_mapRenderTargetToSceneReader.find(renderTargetAddress);
				if (iterator == s_mapRenderTargetToSceneReader.end())
				{
					renderTarget->AllocateMemory(drawableData);
					auto config = renderTarget->GetConfiguration();
					auto rootNode = config->mRootNode.lock();
					checkPtr(rootNode, std::string("SceneReader::sortDrawables: RenderTargetConfiguration named ") + config->mName + std::string(" does not have a root node attached. Set mRootNode to fix this!"));
					checkPtr(config->mCamera, std::string("SceneReader::sortDrawables: RenderTargetConfiguration named ") + config->mName + std::string(" does not have a camera attached. Set mCamera to fix this!"));
					auto scene = std::make_shared<Scene>(rootNode, config->mCamera);
					auto sceneReader = std::make_shared<SceneReader>(scene, renderTarget);
					scene->SetSceneReader(sceneReader);
					m_renderTargetScenes.push_back(scene);

					// make sure not to read same render target twice by adding it to a static list
					s_mapRenderTargetToSceneReader.insert({ renderTargetAddress, sceneReader });

					// the member list will invalidate the strong reference when the scene reader object is deleted
					m_mapRenderTargetToSceneReader.insert({ renderTargetAddress, sceneReader });

					// read and create render target content
					sceneReader->ReadComplete();
					auto renderLists = sceneReader->GetListOfRenderLists();

					// append all renderlists of scene reader to the render lists of the current scene.
					m_listOfRenderLists->insert(m_listOfRenderLists->end(), renderLists->begin(), renderLists->end());
				}
			}

		};
	}

	std::shared_ptr<ShaderCombination> SceneReader::determineShaderCombination(std::shared_ptr<Drawable> drawable)
	{
		auto shaders = drawable->GetProperty<ShadersBase>();
		if (shaders == nullptr)
		{
			err("SceneReader::determineShaderCombination: no shader property set for drawable.");
		}
		return shaders->GetShaderCombination();
	}

	std::shared_ptr<DrawableData> SceneReader::addDrawableToList(std::shared_ptr<Node> node, DrawableVector& drawableVector)
	{
		// create new object to be drawn by the renderer
		auto drawableData = std::make_shared<DrawableData>();
		drawableData->mNode = node;

		drawableVector.push_back(drawableData);
		m_allDrawables.push_back(drawableData);
		return drawableData;
	}

	void SceneReader::allocateSceneMemory(size_t startOffset)
	{
		for(size_t i = startOffset; i < m_allDrawables.size(); i++)
		{
			auto& drawableData = m_allDrawables[i];
			// let each property allocate memory themselves
			auto drawableProperties = drawableData->mNode->GetDrawable()->GetAllProperties();
			for (const auto& [propertyId, propertyVector] : *drawableProperties)
			{
				for (uint32_t i = 0; i < propertyVector.size(); i++)
				{
					propertyVector[i]->AllocateMemory(drawableData);
				}
			}
			createDescriptorSets(drawableData);
		}
	}

	void SceneReader::createRenderList()
	{
		addOpaqueDrawablesToRenderList();
		addTransparentDrawablesToRenderList();
	}

	void SceneReader::addOpaqueDrawablesToRenderList()
	{
		// sort drawables by shader combination index
		ShaderCombinationDrawableMapping drawableMapping;
		for (const auto drawableData : m_opaqueDrawables)
		{
			auto drawable = drawableData->mNode->GetDrawable();
			uint64_t scIndex = drawable->GetProperty<ShadersBase>()->GetShaderCombination()->GetShaderCombinationIndex();

			auto iterator = drawableMapping.find(scIndex);
			if (iterator == drawableMapping.end())
			{
				auto vector = std::make_shared<DrawableVector>();
				vector->push_back(drawableData);
				drawableMapping.insert({ scIndex, vector });
			}
			else
			{
				iterator->second->push_back(drawableData);
			}
		}

		auto shaderCombinations = ShaderManager::GetInstance()->GetShaderCombinations();
		for (const auto& scIndexDrawableVectorPair : drawableMapping)
		{
			Renderer::VreRenderer::DrawableList drawableList;

			// find the shader combination
			auto scIndex = scIndexDrawableVectorPair.first;
			auto scIt = shaderCombinations->find(scIndex);
			if (scIt == shaderCombinations->end())
			{
				err("SceneReader::addOpaqueDrawablesToRenderList: Requested shader combination index has no shader combination!");
			}

			// set the shader combination for these drawables
			drawableList.mShaderCombination = scIt->second.lock();

			// add the drawables to the list
			for (const auto& drawableData : *scIndexDrawableVectorPair.second)
			{
				drawableList.mDrawableDataList.push_back(drawableData);
			}
			m_renderList->GetOpaqueDrawables()->push_back(drawableList);
		}
	}

	void SceneReader::addTransparentDrawablesToRenderList()
	{
		//TODO:
		// keep pipeline
		auto camera = m_scene.lock()->GetCamera();
		QVector3D* cameraPos = camera->GetCameraEye();
		using ShaderCombinationDrawableDataPair = std::pair<uint64_t, std::shared_ptr<DrawableData>>;
		std::map<float, ShaderCombinationDrawableDataPair> distanceSortedDrawables;
		std::unordered_set<float> distances;

		for (const auto drawableData : m_transparentDrawables)
		{
			// calculate distance to camera
			QVector4D posColumn = drawableData->mTransformation->column(3);
			QVector3D pos3d(posColumn.x(), posColumn.y(), posColumn.z());

			float distance = cameraPos->distanceToPoint(pos3d);

			auto it = distances.find(distance);
			while(it != distances.end())
			{ 
				// make sure distance is unique by adding a small offset
				distance += 0.001f;
				it = distances.find(distance);
			}
			distances.insert(distance);

			// get shader index for drawable
			uint64_t scIndex = drawableData->mNode->GetDrawable()->GetProperty<ShadersBase>()->GetShaderCombination()->GetShaderCombinationIndex();
			
			// sort by distance, mapped with scIndex
			distanceSortedDrawables.insert({ distance, {scIndex, drawableData} });
		}

		// now go through the by distance sorted drawable list
		// and group drawables with the same shader combination together
		auto shaderCombinations = ShaderManager::GetInstance()->GetShaderCombinations();
		uint64_t currentScIndex = -1;
		uint64_t scIndex = -1;
		Renderer::VreRenderer::DrawableList drawableList;
		for(auto it = distanceSortedDrawables.rbegin(); it != distanceSortedDrawables.rend(); it++)
		{
			scIndex = it->second.first;
			if (scIndex != currentScIndex)
			{
				// if the new scIndex differs, push the current drawable list
				// the first element won't push an empty list because no shader combination is set yet
				if (drawableList.mShaderCombination != nullptr)
				{
					// copy by value, so drawable list can be cleared and reused afterwards
					m_renderList->GetTransparentDrawables()->push_back(drawableList);
					drawableList.mDrawableDataList.clear();
				}

				// set the shader combination for the current drawable list
				auto scIt = shaderCombinations->find(scIndex);
				if (scIt == shaderCombinations->end())
				{
					err("SceneReader::addOpaqueDrawablesToRenderList: Requested shaderCombinationId has no shaderCombination!");
				}
				drawableList.mShaderCombination = scIt->second.lock();
				currentScIndex = scIndex;
			}
			drawableList.mDrawableDataList.push_back(it->second.second);
		}
		// add the final drawable list
		if(drawableList.mDrawableDataList.size() > 0)
			m_renderList->GetTransparentDrawables()->push_back(drawableList);
	}

	void SceneReader::createDescriptorSets(std::shared_ptr<DrawableData> drawableData)
	{
		auto shaders = drawableData->mNode->GetDrawable()->GetProperty<ShadersBase>();
		auto uniformBuffer = shaders->CreateUniformBuffer();
		uniformBuffer->Create(drawableData);
		uniformBuffer->SetScene(m_scene.lock());
		drawableData->mUniformBuffer = uniformBuffer;
	}

	std::function<void(std::shared_ptr<Node> addedNode, Node* parentNode)> SceneReader::getNodeAddedCallback()
	{
		return [this](std::shared_ptr<Node> addedNode, Node* parentNode) -> void
		{
			// what has to happen in case a new node is added?
			// step1: add the node to the transformation tree so its correctly transformed
			// step2: add to list of drawables (order is irrelevant)
			// step4: allocate its memory
			// step5: update the scene render list
			// step6: ?rerecord command buffers? (done automatically?)

			// step 1:
			// add the node to the transformation tree
			auto newTransformationNode = std::make_shared<TransformationNode>(addedNode, this);

			// find all transformation nodes added to the parent node
			auto transformationNodes = parentNode->GetTransformationNodes();
			for (auto& transformationNode : *transformationNodes)
			{
				// if transformation node scene reader is equal to current scene reader
				// we have found the correct parent transformation node
				// (other method would be to access the transformation root node and traverse it)
				if (transformationNode->GetSceneReader() == this)
				{
					transformationNode->AddChild(newTransformationNode);
					addChildsToTransformationNode(addedNode, newTransformationNode);
					break;
				}
			}

			// step2:
			// add to list of drawables
			// first remove current scene render list, because current scene render list must always be the last item.
			m_listOfRenderLists->pop_back();

			// remember sizes of current lists
			auto opaqueOldSize = m_opaqueDrawables.size();
			auto transparentOldSize = m_transparentDrawables.size();
			auto allOldSize = m_allDrawables.size();

			// add new items to list
			traverseTransformGraph(newTransformationNode, sortDrawables());
			
			// step3:
			// allocate its memory
			allocateSceneMemory(allOldSize);
			
			// step4:
			// update render list

			// easiest thing would be to rerecord renderlist completely, but probably not whats intended.
			// instead, its also possible to get the drawable shader properties and search through the list as where to add them
			m_renderList->GetTransparentDrawables()->clear();
			m_renderList->GetOpaqueDrawables()->clear();
			createRenderList();
			
			m_listOfRenderLists->push_back(m_renderList);

			// tell drawer to rerecord command buffers before next iteration
			m_renderList->GetVreCommandBuffer()->RequestRerecording(m_opaqueDrawables.size() != opaqueOldSize, m_transparentDrawables.size() != transparentOldSize);

		};
	}

	std::function<void(std::shared_ptr<Node> removedNode, Node* parentNode)> SceneReader::getNodeRemovedCallback()
	{
		return [this](std::shared_ptr<Node> removedNode, Node* parentNode) -> void
		{
			// remove all drawables that are attached to this node
			// collect all nodes
			std::unordered_set<std::shared_ptr<Node>> allChildren;
			removedNode->GetAllChildren(allChildren);
			allChildren.insert(removedNode);

			auto eraseElements = [this, allChildren](DrawableVector& drawableVector)
			{
				uint32_t removedElements = 0;
				bool elementErased = false;
				for (auto it = drawableVector.begin(); it != drawableVector.end();)
				{
					elementErased = false;
					auto drawableData = (*it);

					if (allChildren.find(drawableData->mNode) != allChildren.end())
					{
						// free drawable properties
						freeDrawableData(drawableData);

						// signal that iterator increase is down by erase function
						elementErased = true;
						it = drawableVector.erase(it);

						// if all child nodes have been removed, stop iteration
						removedElements++;
						if (allChildren.size() == removedElements)
						{
							return;
						}
					}

					if (!elementErased)
					{
						it++;
					}
				}
			};

			auto opaqueOldSize = m_opaqueDrawables.size();
			auto transparentOldSize = m_transparentDrawables.size();

			eraseElements(m_allDrawables);
			eraseElements(m_opaqueDrawables);
			eraseElements(m_transparentDrawables);

			// update render list
			m_renderList->GetTransparentDrawables()->clear();
			m_renderList->GetOpaqueDrawables()->clear();
			createRenderList();

			// tell drawer to rerecord command buffers before next iteration
			m_renderList->GetVreCommandBuffer()->RequestRerecording(m_opaqueDrawables.size() != opaqueOldSize, m_transparentDrawables.size() != transparentOldSize);

		};
	}
	void SceneReader::freeDrawableData(std::shared_ptr<DrawableData> drawableData)
	{
		// free drawable id related properties
		auto drawableProperties = drawableData->mNode->GetDrawable()->GetAllProperties();
		for (const auto& [propertyId, propertyVector] : *drawableProperties)
		{
			for (uint32_t i = 0; i < propertyVector.size(); i++)
			{
				propertyVector[i]->FreeMemory(drawableData);
			}
		}
	}
}
