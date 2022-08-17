#include <project_relevant_headers.hpp>

using namespace vre;

// global node pointers for easy access. shared resources have to be released in cleanup.
auto root = std::make_shared<Node>();
auto cube2Node = std::make_shared<Node>();
auto cube3Node = std::make_shared<Node>();
auto cube4Node = std::make_shared<Node>();


std::shared_ptr<Node> CreateTargetSceneRootNode()
{
	// setup drawables
	auto drawable = std::make_shared<Drawable>();

	auto root = std::make_shared<Node>();
	root->SetDrawable(drawable);
	root->Name = "RenderTargetExample target scene root";
	return root;
}

auto g_rtNode = std::make_shared<Node>();

void createRTNode()
{
	auto renderTargetConfig = std::make_shared<RenderTargetConfiguration>();
	renderTargetConfig->mHeight = 1080;
	renderTargetConfig->mWidth = 1920;
	renderTargetConfig->mCamera = std::make_shared<Camera>();
	renderTargetConfig->mCamera->SetPosition(0, 0, 14);
	renderTargetConfig->mRootNode = cube2Node;
	renderTargetConfig->mClearColorValues[0] = 0.1f;
	renderTargetConfig->mClearColorValues[1] = 0.1f;
	renderTargetConfig->mClearColorValues[2] = 0.1f;
	renderTargetConfig->mName = "g_Node";

	auto renderTarget = std::make_shared<RenderTarget>(renderTargetConfig);

	// setup the node that is a plane with a render target attached to it
	auto renderTargetDrawable = std::make_shared<Drawable>();

	auto shaders = SceneHelper::GetRenderTargetShaders("shaders/shader_texture_vert.spv", "shaders/shader_texture_frag.spv");
	auto mesh = SceneHelper::GetMeshSimplePlane(QVector4D(1, 0, 1, 1));
	renderTargetDrawable->AddProperty(shaders);
	renderTargetDrawable->AddProperty(renderTarget);
	renderTargetDrawable->AddProperty(mesh);

	g_rtNode->SetDrawable(renderTargetDrawable);
	auto transformation = g_rtNode->GetTransformation();
	transformation->scale(5.0f);
	transformation->translate(0, 4, -5);
	g_rtNode->UpdateTransformation();
}

std::shared_ptr<Scene> PhongShadingScene()
{
	createRTNode();
	// setup the base scene, which contains of three nodes with 3 colored cubes
	//
	//
	//
		// reusable data
		auto colorShaders = SceneHelper::GetShadersColoredObject("shaders/shader_color_vert.spv", "shaders/shader_color_frag.spv");

		// cube red
		auto cubeMesh1 = std::make_shared<Mesh>("models/cube.obj", QVector4D(1.0f, 0.0f, 0.0f, 0.5f), true);
		auto cube1Drawable = std::make_shared<Drawable>();
		cube1Drawable->AddProperty(cubeMesh1);
		cube1Drawable->AddProperty(colorShaders);

		auto cube1Node = std::make_shared<Node>();
		cube1Node->SetDrawable(cube1Drawable);
		cube1Node->Name = "Cube1Node";

		auto transformation = cube1Node->GetTransformation();
		transformation->translate(0, -3, 0);
		cube1Node->UpdateTransformation();

		// cube green
		auto cubeMesh2 = std::make_shared<Mesh>("models/cube.obj", QVector4D(0.0f, 1.0f, 0.0f, 0.5f), true);
		auto cube2Drawable = std::make_shared<Drawable>();
		cube2Drawable->AddProperty(cubeMesh2);
		cube2Drawable->AddProperty(colorShaders);

		cube2Node->SetDrawable(cube2Drawable);
		cube2Node->Name = "Cube2Node";

		transformation = cube2Node->GetTransformation();
		transformation->translate(3, 0, 0);
		cube2Node->UpdateTransformation();

		cube1Node->AddChild(cube2Node);

		// cube blue
		auto cubeMesh3 = std::make_shared<Mesh>("models/cube.obj", QVector4D(0.0f, 0.0f, 1.0f, 0.5f), true);
		auto cube3Drawable = std::make_shared<Drawable>();
		cube3Drawable->AddProperty(cubeMesh3);
		cube3Drawable->AddProperty(colorShaders);

		cube3Node->SetDrawable(cube3Drawable);
		cube3Node->Name = "Cube3Node";

		transformation = cube3Node->GetTransformation();
		transformation->translate(0, 3, 0);
		cube3Node->UpdateTransformation();

		cube2Node->AddChild(cube3Node);

		// cube yellow
		auto cubeMesh4 = std::make_shared<Mesh>("models/cube.obj", QVector4D(1.0f, 1.0f, 0.0f, 0.5f), true);
		auto cube4Drawable = std::make_shared<Drawable>();
		cube4Drawable->AddProperty(cubeMesh4);
		cube4Drawable->AddProperty(colorShaders);

		cube4Node->SetDrawable(cube4Drawable);
		cube4Node->Name = "Cube4Node";

		transformation = cube4Node->GetTransformation();
		transformation->translate(3, 3, 0);
		cube4Node->UpdateTransformation();
		//cube2Node->AddChild(cube4Node);

	// root node
	root->AddChild(cube1Node);
	root->Name = "RenderTargetExample main scene root";

	// setup the render target nodes and the render targets
	//
	//
	//
		std::array<std::shared_ptr<Node>, 5> nodes = {cube1Node, cube1Node, cube2Node, cube2Node, cube3Node};
		for (int i = 0; i < nodes.size(); i++)
		{
			auto renderTargetConfig = std::make_shared<RenderTargetConfiguration>();
			renderTargetConfig->mHeight = 1080;
			renderTargetConfig->mWidth = 1920;
			renderTargetConfig->mCamera = std::make_shared<Camera>();
			renderTargetConfig->mCamera->SetPosition(0, 0, 14);
			renderTargetConfig->mRootNode = nodes[i];
			renderTargetConfig->mClearColorValues[0] = 0.1f;
			renderTargetConfig->mClearColorValues[1] = 0.1f;
			renderTargetConfig->mClearColorValues[2] = 0.1f;

			auto renderTarget = std::make_shared<RenderTarget>(renderTargetConfig);

			// setup the node that is a plane with a render target attached to it
			auto renderTargetDrawable = std::make_shared<Drawable>();

			auto shaders = SceneHelper::GetRenderTargetShaders("shaders/shader_texture_vert.spv", "shaders/shader_texture_frag.spv");
			auto mesh = SceneHelper::GetMeshSimplePlane(QVector4D(1, 0, 1, 1));
			renderTargetDrawable->AddProperty(shaders);
			renderTargetDrawable->AddProperty(renderTarget);
			renderTargetDrawable->AddProperty(mesh);

			auto renderTargetNode = std::make_shared<Node>();
			renderTargetNode->SetDrawable(renderTargetDrawable);
			transformation = renderTargetNode->GetTransformation();
			//transformation->scale(5.0f);
			int offset = -6 + i * (3);
			transformation->translate(-6 + i * 3, 0, -5);
			renderTargetNode->UpdateTransformation();

			root->AddChild(renderTargetNode);
		}

	auto camera = std::make_shared<Camera>();
	camera->SetPosition(0, 0, 14);
	//camera->SetCameraDirection(0.55f, -0.57, -0.59);
	return std::make_shared<Scene>(root, camera);
}

class RenderTargetProject : public Project
{
public:
	void Setup(std::shared_ptr<SceneManager> sceneManager) override
	{
		auto scene = PhongShadingScene();
		sceneManager->AddScene(scene);
		sceneManager->SetActiveScene(scene);
	}

	EventDispatching KeyPressed(Qt::Key key) override
	{

		// remove node
		if (key == Qt::Key::Key_Z)
		{
			cube2Node->AddChild(cube3Node);
		}

		if (key == Qt::Key::Key_U)
		{
			cube2Node->RemoveChild(cube3Node);
		}

		if (key == Qt::Key::Key_B)
		{
			cube2Node->AddChild(cube4Node);
		}

		if (key == Qt::Key::Key_N)
		{
			cube2Node->RemoveChild(cube4Node);
		}

		if (key == Qt::Key::Key_G)
		{
			root->AddChild(g_rtNode);
		}

		if (key == Qt::Key::Key_H)
		{
			root->RemoveChild(g_rtNode);
		}


		if (key == Qt::Key::Key_D)
		{
			cube2Node->GetTransformation()->rotate(1, 0, 1);
			cube2Node->UpdateTransformation();
			return EventDispatching::CatchEvent;
		}

		if (key == Qt::Key::Key_A)
		{
			cube2Node->GetTransformation()->rotate(-1, 0, 1);
			cube2Node->UpdateTransformation();
			return EventDispatching::CatchEvent;
		}

		if (key == Qt::Key::Key_Q)
		{

			cube3Node->GetTransformation()->rotate(-1, 0, 1);
			cube3Node->UpdateTransformation();
			return EventDispatching::CatchEvent;
		}

		if (key == Qt::Key::Key_E)
		{
			cube3Node->GetTransformation()->rotate(1, 0, 1);
			cube3Node->UpdateTransformation();
			return EventDispatching::CatchEvent;
		}

		return EventDispatching::PassEvent;
	}

	void Cleanup() override
	{
		root.reset();
		cube2Node.reset();
		cube3Node.reset();
		cube4Node.reset();
		g_rtNode.reset();
	}
};

int main(int argc, char* argv[])
{
	VreConfiguration vreConfig{};
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mProject = std::make_shared<RenderTargetProject>();
	vreConfig.mApplicationName = "RenderTargetExample";
	vreConfig.mPathToResources = Filesystem::getResourcePath();
	return RunVre(argc, argv, vreConfig);
}
