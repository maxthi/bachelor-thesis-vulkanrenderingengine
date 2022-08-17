#include <project_relevant_headers.hpp>
#include "vl_phong.hpp"
#include "ub_phong.hpp"

using namespace vre;

std::shared_ptr<Scene> PhongShadingScene()
{
	auto texture = std::make_shared<Texture2D>("textures/sphere_texture.png");
	auto mesh = std::make_shared<Mesh>("models/sphere.obj", QVector4D(1.0f, 0.0f, 1.0, 1.0f));
	
	// create shader pair with custom shader classes
	auto fragmentShader = FragmentShader::GetFragmentShader("shaders/shader_phong_frag.spv");
	auto vertexShader = VertexShader::GetVertexShader("shaders/shader_phong_vert.spv", std::make_shared<VertexLayoutPhong>());
	auto shaderCombination = ShaderManager::GetInstance()->GetShaderCombination(vertexShader, fragmentShader, UniformBufferPhong::GetDescriptorSetLayout());
	auto shaders = std::make_shared<Shaders<UniformBufferPhong>>(shaderCombination);

	// setup drawables
	auto drawable = std::make_shared<Drawable>();
	drawable->AddProperty(texture);
	drawable->AddProperty(mesh);
	drawable->AddProperty(shaders);

	auto root = std::make_shared<Node>();
	root->SetDrawable(drawable);

	auto camera = std::make_shared<Camera>();
	camera->SetPosition(-3.78f, 3.69f, 10.78f);
	camera->SetCameraDirection(0.31f, -0.31f, -0.89f);

	return std::make_shared<Scene>(root, camera);
}

class PhongShadingProject : public Project
{
public:
	void Setup(std::shared_ptr<SceneManager> sceneManager) override
	{
		auto scene = PhongShadingScene();
		sceneManager->AddScene(scene);
		sceneManager->SetActiveScene(scene);
	}
};

int main(int argc, char* argv[])
{
	VreConfiguration vreConfig{};
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mVulkanCfg.mPipelineConfiguration.mPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	vreConfig.mProject = std::make_shared<PhongShadingProject>();
	vreConfig.mApplicationName = "PhongShading";
	vreConfig.mPathToResources = Filesystem::getResourcePath();
	return RunVre(argc, argv, vreConfig);
}
