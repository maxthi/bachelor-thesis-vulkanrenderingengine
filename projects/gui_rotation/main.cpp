#include <project_relevant_headers.hpp>

#include <qobject.h>
#include <QApplication>
#include <QLoggingCategory>

#include "slider_callback.hpp"

// the auto moc generated ui file
#include "./ui_rotation.h"

using namespace vre;

std::shared_ptr<Scene> MainScene();
std::shared_ptr<vre::Node> drawable1Node;
std::shared_ptr<vre::Node> drawable2Node;
std::shared_ptr<ModelTransformation> modelTransformation;
SliderCallback* gSliderCallback;

/// @brief GuiRoboter
class GuiRoboterProj : public Project
{
public:
    void Setup(std::shared_ptr<SceneManager> sceneManager) override
    {
		auto mainScene = MainScene();
		sceneManager->AddScene(mainScene);
		sceneManager->SetActiveScene(mainScene);
		
		auto lDock = new Ui_RotationDock();
        auto mainWindow = VreCore::GetInstance()->GetHostWindow();
		QDockWidget* lDockWidget = new QDockWidget(QString("Rotation"), mainWindow);
		lDock->setupUi(lDockWidget);
		lDockWidget->resize(150, 100);
		mainWindow->addDockWidget(Qt::RightDockWidgetArea, lDockWidget);
		lDockWidget->show();

		gSliderCallback = new SliderCallback();
		gSliderCallback->SetNode(drawable1Node);
		gSliderCallback->SetNode2(drawable2Node);
		gSliderCallback->SetModelTransformation(modelTransformation);

		//QObject::connect(OpenGLWidget::getInstance(), SIGNAL(sigFPS(int)), lDock->lcdNumber, SLOT(display(int)));
		// node rotation
		QObject::connect(lDock->verticalSlider, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateX(int)));
		QObject::connect(lDock->verticalSlider_2, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateY(int)));
		QObject::connect(lDock->verticalSlider_3, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateZ(int)));

		// model rotation
		QObject::connect(lDock->verticalSlider_4, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateModelX(int)));
		QObject::connect(lDock->verticalSlider_5, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateModelY(int)));
		QObject::connect(lDock->verticalSlider_6, SIGNAL(valueChanged(int)), gSliderCallback, SLOT(RotateModelZ(int)));
    }

	void Cleanup() override
	{
		drawable1Node.reset();
		drawable2Node.reset();
		modelTransformation.reset();
		delete gSliderCallback;
	}
};

int main(int argc, char *argv[])
{
	VreConfiguration vreConfig{};
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mProject = std::make_shared<GuiRoboterProj>();
	vreConfig.mApplicationName = "Rotation of objects using Qt GUI";
	vreConfig.mPathToResources = Filesystem::getResourcePath();
	return RunVreGUI(argc, argv, vreConfig);
}

std::shared_ptr<Scene> MainScene()
{
	// setup resources
	auto meshCube1 = std::make_shared<Mesh>("models/cube.obj", QVector4D(0.0f, 1.0f, 0.0f, 1.0f), true);
	auto meshCube2 = std::make_shared<Mesh>("models/cube.obj", QVector4D(0.0f, 0.0f, 1.0f, 1.0f), true);

	// model transform
	auto modeltransform = std::make_shared<QMatrix4x4>();
	modeltransform->rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	modelTransformation = std::make_shared<ModelTransformation>(modeltransform);

	// setup drawables
	auto shaders1 = SceneHelper::GetShadersColoredObject("shaders/shader_color_vert.spv", "shaders/shader_color_frag.spv");
	auto drawable1 = std::make_shared<Drawable>();
	drawable1->AddProperty(meshCube1);
	drawable1->AddProperty(shaders1);
	drawable1->AddProperty(modelTransformation);

	auto drawable2 = std::make_shared<Drawable>();
	drawable2->AddProperty(meshCube2);
	drawable2->AddProperty(shaders1);
	drawable2->AddProperty(modelTransformation);

	drawable1Node = std::make_shared<Node>();
	drawable1Node->SetDrawable(drawable1);
	drawable1Node->Name = "Drawable1";

	drawable2Node = std::make_shared<Node>();
	drawable2Node->SetDrawable(drawable2);
	drawable2Node->Name = "Drawable2";

	// drawable 2 is a bit left to the side
	auto transform2 = drawable2Node->GetTransformation();
	transform2->translate(3.0f, 0.0f, 0.0f);
	drawable2Node->UpdateTransformation();

	auto cubeMesh = std::make_shared<Mesh>("models/cube.obj", QVector4D(1.0f, 0.0f, 0.0f, 0.3f), true);
	// the colord object
	auto shaders3 = SceneHelper::GetShadersColoredObject("shaders/shader_color_vert.spv", "shaders/shader_color_frag.spv");
	auto cubeDrawable = std::make_shared<Drawable>();
	cubeDrawable->AddProperty(cubeMesh);
	cubeDrawable->AddProperty(shaders3);

	auto cubeNode = std::make_shared<Node>();
	cubeNode->SetDrawable(cubeDrawable);
	cubeNode->Name = "Cube";

	// shift the cube a bit on top of the first drawable
	auto cubeTransform = cubeNode->GetTransformation();
	cubeTransform->scale(0.5f);
	cubeTransform->translate(0.0f, 6.0f, 0.0f);
	cubeNode->UpdateTransformation();

	drawable1Node->AddChild(cubeNode);

	// build scene hierarchy
	auto root = std::make_shared<Node>();

	root->AddChild(drawable1Node);
	root->AddChild(drawable2Node);

	auto camera = std::make_shared<Camera>();
	camera->SetPosition(4.5f, 3.7f, -5.6);
	camera->SetCameraDirection(-0.48f, -0.36, 0.79);

	return std::make_shared<Scene>(root, camera);
}
