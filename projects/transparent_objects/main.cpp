#include <project_relevant_headers.hpp>
#include <QVector4D>

using namespace vre;

std::shared_ptr<Scene> MainScene();

class TransparentObjects : public Project
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
	vreConfig.mProject = std::make_shared<TransparentObjects>();
	vreConfig.mApplicationName = "Transparent Objects";
	vreConfig.mPathToResources = Filesystem::getResourcePath();

	return RunVre(argc, argv, vreConfig);
}

std::shared_ptr<Scene> MainScene()
{
	// build scene hierarchy
	auto root = std::make_shared<Node>();

	std::array<std::shared_ptr<Texture2D>, 3> textures = {
		std::make_shared<Texture2D>("textures/tex1.png"),
		std::make_shared<Texture2D>("textures/tex2.png"),
		std::make_shared<Texture2D>("textures/tex3.png")
	};

	auto mesh = SceneHelper::GetMeshSimplePlane(QVector4D(1.0f, 0.0f, 0.0f, 0.1f));
	auto shadersTexture = SceneHelper::GetShadersTexturedObject("shaders/shader_texture_vert.spv", "shaders/shader_texture_frag.spv");
	int numObjects = 50;
	for (int i = -numObjects; i < numObjects; i++)
	{
		auto drawable = std::make_shared<Drawable>();
		drawable->AddProperty(mesh);
		drawable->AddProperty(textures[abs(i % 3)]);
		drawable->AddProperty(shadersTexture);
		drawable->SetTransparency(true);

		auto node = std::make_shared<Node>();
		node->SetDrawable(drawable);
		node->GetTransformation()->translate(0, 0, i * 2);
		node->UpdateTransformation();
		root->AddChild(node);

		node = std::make_shared<Node>();
		node->SetDrawable(drawable);
		node->GetTransformation()->translate(3, 0, i * 2);
		node->UpdateTransformation();
		root->AddChild(node);

		node = std::make_shared<Node>();
		node->SetDrawable(drawable);
		node->GetTransformation()->translate(0, 3, i * 2);
		node->UpdateTransformation();
		root->AddChild(node);

		node = std::make_shared<Node>();
		node->SetDrawable(drawable);
		node->GetTransformation()->translate(3, 3, i * 2);
		node->UpdateTransformation();
		root->AddChild(node);
	}

	auto camera = std::make_shared<Camera>();
	camera->SetPosition(1.38f, 1.76f, -1.97f);
	return std::make_shared<Scene>(root, camera);
}
