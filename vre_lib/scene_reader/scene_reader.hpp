#pragma once
#include "../common.hpp"
#include "structs/drawable_data.hpp"
#include "definitions/shadercombination_drawable_mapping.hpp"

#include "../scene_manager/scene.hpp"
#include "../scene_manager/scene_objects/node.hpp"
#include "../scene_manager/scene_objects/drawable.hpp"
#include "../scene_manager/drawable_properties/render_target.hpp"
#include "../scene_manager/drawable_properties/shaders.hpp"

#include "../shader_manager/shader_combination.hpp"
#include "../vulkan_renderer/vre_renderer/vre_renderlist.hpp"

namespace vre
{

	/// @brief The SceneReader is the connection between a scene and the renderer.
	/// The SceneReader knows about the existing drawable properties and what to do with them.
	/// For example to add 2D Textures to a uniformbuffer.
	/// The main job furthermore is to parse the scene into a data structure, that the Renderer can use directly for rendering.
	/// This means the SceneReader uses the VulkanManager to create the buffers and other VulkanObjects.
	class SceneReader
	{
	public:
		/// @brief Creates a scene reader object for a scene.
		/// @param scene - The scene to create the scene reader for.
		/// @param renderTarget - Optional rendertarget to render this scene to.
		SceneReader(std::shared_ptr<Scene> scene, std::shared_ptr<RenderTarget> renderTarget = nullptr);


		~SceneReader();

		std::shared_ptr<std::list<std::shared_ptr<Renderer::VreRenderer::VreRenderList>>> GetListOfRenderLists() { return m_listOfRenderLists; }

		/// @brief Allocate memory for the scenegraph.
		void ReadComplete();

		/// @brief Make sure the order of the transparent drawables is still correct, otherwise update the order
		/// @param renderList 
		void UpdateTransparentDrawableOrder();

		/// @brief Frees all the memory allocated for the related scene.
		void FreeAllocatedMemory();

		/// @brief Get the scene that the scene reader manages.
		/// @return A shared ptr to the scene.
		std::shared_ptr<Scene> GetScene() { return m_scene.lock(); }

	private:
		/// @brief Keeping track of all RenderTargets that have been processed already.
		/// Each Rendertarget has its own SceneReader, because it needs its own subset of uniform buffers
		/// The identifiying key is the memory address of the render targets.
		static inline std::unordered_map<RenderTarget*, std::weak_ptr<SceneReader>> s_mapRenderTargetToSceneReader{};

		/// @brief Store a strong reference of each render target in use.
		std::unordered_map<RenderTarget*, std::shared_ptr<SceneReader>> m_mapRenderTargetToSceneReader{};

		/// @brief This is a sorted list of render lists. It has to be returned to the renderer, which will render each
		/// RenderList in order. A scene reader will always add the current scene at the end of the list.
		/// If the scene contains render targets, these can add additonal RenderList items before.
		std::shared_ptr<std::list<std::shared_ptr<Renderer::VreRenderer::VreRenderList>>> m_listOfRenderLists;

		/// @brief The RenderList of the scene.
		std::shared_ptr<Renderer::VreRenderer::VreRenderList> m_renderList;

		/// @brief Each scene reader that renders into a buffer that is not the swapchain buffer, needs a render target configuration.
		/// The information about the target buffer will be passed to the renderlist. The renderer will then create the resources, accordingly.
		std::shared_ptr<RenderTargetConfiguration> m_renderTargetConfiguration{};

		/// @brief Indicates if the scene reader object controls the main scene. (the scene that renders onto the swapchain images)
		bool m_isMainScene{ false };

		/// @brief SceneManager scene that this SceneReader scene consists of.
		std::weak_ptr<Scene> m_scene;

		/// @brief If the scene reader is based on a render target, this value contains the ptr.
		std::shared_ptr<RenderTarget> m_renderTarget{};

		/// @brief List of all drawables.
		DrawableVector m_allDrawables;

		/// @brief List of all opaque drawables.
		DrawableVector m_opaqueDrawables;

		/// @brief List of all transparent drawables.
		DrawableVector m_transparentDrawables;
		
		/// @brief The root of the transformation graph.
		std::shared_ptr<TransformationNode> m_transformationRootNode;

		/// @brief This vector holds the strong references to scenes that have been created because of render targets
		/// and therefore belong to this scene reader.
		std::vector<std::shared_ptr<Scene>> m_renderTargetScenes;

		/// @brief Traverses the scenegraph and calls 'func' to process each node.
		/// @param node - The node at which traversion starts, usually the root node.
		/// @param func - The function to process the data in the node.
		void traverseGraph(const std::shared_ptr<Node> node, std::function<void(const std::shared_ptr<Node> node)> func);

		void traverseTransformGraph(const std::shared_ptr<TransformationNode> node, std::function<void(const std::shared_ptr<TransformationNode> node)> func);

		/// @brief Creates a copy of the scenes graph in order to store transformations.
		void createTransformationGraph();

		/// @brief Called from createTransformationGraph. Traverses scene graph and adds its childs to the transformation graph.
		/// @param node - The node which childs are processed.
		/// @param transformationNode - The transformation parent node to store the new childs in.
		void addChildsToTransformationNode(std::shared_ptr<Node> node, std::shared_ptr<TransformationNode> transformationNode);

		/// @brief For each node, sort drawable by transparency/opacity and by the shader stage combination.
		/// @return The lambda to call with the current node in the scenegraph traversion.
        std::function<void(const std::shared_ptr<TransformationNode> node)> sortDrawables();

		/// @brief Examine all shaders a drawable uses and determine the resulting unique transmutation.
		/// @param drawable - The current drawable to examine shaders at.
		/// @return A unique combination of shaders.
        std::shared_ptr<ShaderCombination> determineShaderCombination(std::shared_ptr<Drawable> drawable);

		/// @brief To sort the drawables into a mapping, either opaque or transparent, with the shader stage combination as key.
		/// @param drawable - The drawable to sort.
		/// @param shaderStageCombination  - The key for the mapping.
		/// @param drawableMapping - Which mapping to sort the drawable into.
		std::shared_ptr<DrawableData> addDrawableToList(std::shared_ptr<Node> node, DrawableVector& drawableVector);

		/// @brief Stores the drawable properties on the GPU for rendering. Can only happen when Vulkan has been initialiased.
		/// @param startOffset - The starting index in the list of all drawables. Can be used if new drawables are added.
		void allocateSceneMemory(size_t startOffset = 0);

		/// @brief Create a RenderList to be used by the renderer.
		/// @param renderList - The RenderList.
		void createRenderList();

		/// @brief Sorts the opaque drawables by their shader combination and add them to the render list.
		void addOpaqueDrawablesToRenderList();

		/// @brief Sorts the transparent drawables first by distance to the camera and groups them together by shader combination.
		void addTransparentDrawablesToRenderList();

		void createDescriptorSets(std::shared_ptr<DrawableData> drawableData);

		/// @brief Returns the callback to be called when the associated scene has a node added.
		std::function<void(std::shared_ptr<Node> addedNode, Node* parentNode)> getNodeAddedCallback();

		/// @brief Returns the callback to be called when the associated scene has a node removed.
		std::function<void(std::shared_ptr<Node> removedNode, Node* parentNode)> getNodeRemovedCallback();

		void freeDrawableData(std::shared_ptr<DrawableData> drawableData);
	};
}
