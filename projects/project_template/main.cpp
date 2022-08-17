#include <project_relevant_headers.hpp>

using namespace vre;

std::shared_ptr<Scene> GetScene()
{
	// root node
	auto root = std::make_shared<Node>();

	// optional camera
	auto camera = std::make_shared<Camera>();
	return std::make_shared<Scene>(root, camera);
}

class TemplateProject : public Project
{
public:
	void Setup(std::shared_ptr<SceneManager> sceneManager) override
	{
		// scene setup
		auto scene = GetScene();
		// adding the scene to the manager
		sceneManager->AddScene(scene);
		// set as scene to render
		sceneManager->SetActiveScene(scene);
	}

	void Cleanup() override
	{
		// free resources
	}

	EventDispatching KeyPressed(Qt::Key key) override
	{
		// key actions
		if (key == Qt::Key::Key_A)
		{
		}
		return EventDispatching::PassEvent;
	}

};

int main(int argc, char* argv[])
{
	// configure the vre here
	// use the right handed system to be compliant with OpenGL shaders etc.
	VreConfiguration vreConfig{};
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mProject = std::make_shared<TemplateProject>();
	vreConfig.mApplicationName = "Template Scene";
	vreConfig.mPathToResources = Filesystem::getResourcePath();

	return RunVre(argc, argv, vreConfig);
}