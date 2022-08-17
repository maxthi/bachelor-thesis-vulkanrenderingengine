#include "mainwindow.hpp"

#include <QApplication>
#include <QLoggingCategory>

#include <project_relevant_headers.hpp>

using namespace vre;

std::shared_ptr<Scene> MainScene();


class GuiExampleProj : public Project
{
public:
    void Setup(std::shared_ptr<SceneManager> sceneManager) override
    {
		auto mainScene = MainScene();
		sceneManager->AddScene(mainScene);
		sceneManager->SetActiveScene(mainScene);
    }
};


int main(int argc, char *argv[])
{
	VreConfiguration vreConfig{};
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mProject = std::make_shared<GuiExampleProj>();
	vreConfig.mPathToResources = Filesystem::getResourcePath();

	//return RunVreGUI<MainWindow>(argc, argv, vreConfig);
	return RunVreGUI(argc, argv, vreConfig);
}


std::shared_ptr<Scene> MainScene()
{
	// setup resources
	// model source from https://opengameart.org/content/fireicestone-golem
	auto mesh = std::make_shared<Mesh>("models/golem_0.obj", QVector4D(1.0f, 0.0f, 0.0f, 1.0f), true);
	auto colorShader = SceneHelper::GetShadersColoredObject("shaders/shader_color_vert.spv", "shaders/shader_color_frag.spv");

	// setup drawables
	auto drawable1 = std::make_shared<Drawable>();
	drawable1->AddProperty(mesh);
	drawable1->AddProperty(colorShader);

	auto child1 = std::make_shared<Node>();
	child1->GetTransformation()->translate(-6.0f, -3.0f, -9.0f);
	child1->UpdateTransformation();
	child1->SetDrawable(drawable1);
	child1->Name = "Child1";

	auto mesh2 = std::make_shared<Mesh>("models/golem_0.obj", QVector4D(0.0f, 1.0f, 0.0f, 1.0f), true);
	auto drawable2 = std::make_shared<Drawable>();
	drawable2->AddProperty(mesh2);
	drawable2->AddProperty(colorShader);

	auto child2 = std::make_shared<Node>();
	child2->GetTransformation()->translate(0.0f, -3.0f, -9.0f);
	child2->UpdateTransformation();
	child2->SetDrawable(drawable2);
	child2->Name = "Child2";

	auto mesh3 = std::make_shared<Mesh>("models/golem_0.obj", QVector4D(0.0f, 0.0f, 1.0f, 1.0f), true);
	auto drawable3 = std::make_shared<Drawable>();
	drawable3->AddProperty(mesh3);
	drawable3->AddProperty(colorShader);

	auto child3 = std::make_shared<Node>();
	child3->GetTransformation()->translate(6.0f, -3.0f, -9.0f);
	child3->UpdateTransformation();
	child3->SetDrawable(drawable3);
	child3->Name = "Child3";

	// setup scene
	auto root = std::make_shared<Node>();
	root->AddChild(child1);
	root->AddChild(child2);
	root->AddChild(child3);

	auto camera = std::make_shared<Camera>();
	camera->SetPosition(8.1f, 6.3f, 4.4f);
	camera->SetCameraDirection(-0.41f, -0.42f, -0.80f);
	return std::make_shared<Scene>(root, camera);
}
