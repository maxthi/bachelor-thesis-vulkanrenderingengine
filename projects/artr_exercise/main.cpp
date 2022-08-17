#include <project_relevant_headers.hpp>
#include "vertex_layout_artr.hpp"

using namespace vre;

std::shared_ptr<Mesh> createTriangleMesh()
{
	// TODO siehe A1&A2
	
	// Info:
	// Das Koordinatensystem ist wie in OpenGL rechtshändig, (siehe main() vkConfig->UseRightHandedSystem)
	// x
	// 0,0,0 ist der mittelpunkt

	// Tipps für A2) topologie: triangle list -> 3 vertices bilden ein triangle
	// zu jedem vertex in vertices muss es eine color in colors geben, sonst crash ;-;
	std::vector<QVector3D> vertices =
	{
		// x, y, z
		{-1.0f, 1.0f, 1.0f}, // left top corner
		{1.0f, 1.0f, 1.0f}, // right top corner
		{-1.0f, -1.0f, 1.0f}, // left bottom corner
		{1.0f, -1.0f, 1.0f}, //right bottom corner
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2, 3, 2, 1
	};

	std::vector<QVector4D> colors =
	{
		{1.0f, 0.0f, 0.0f, 1.0f}, // rgba
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
	};

	// texCoords x,y / u,v
	// not used anyways, but hey
	std::vector<QVector2D> texCoords =
	{
		{0.0f, 0.0f}, // left top corner
		{1.0f, 0.0f}, // right top corner
		{0.5f, 1.0f}, // middle bottom corner
	};

	return std::make_shared<Mesh>(vertices, indices, colors, texCoords);
}

std::shared_ptr<Scene> SetupScene()
{

	auto mesh = createTriangleMesh();
	auto shaders = SceneHelper::GetShadersColoredObject("shaders/shader_color_vert.spv", "shaders/shader_color_frag.spv");

	// TODO: Diese Shader für Aufgabe 3 wieder einkommentieren.
	
	// altes vertex layout mit clown shader
	shaders = SceneHelper::GetShadersColoredObject("shaders/shader_clown_vert.spv", "shaders/shader_color_frag.spv");
	
	// eigenes vertex layout mit clown shader
	//shaders = VertexLayoutARTR::GetShaders("shaders/shader_clown_vert.spv", "shaders/shader_color_frag.spv");
	

	// ACHTUNG:
	//		Anschließend nicht ausführen solange vertex_layout_artr.hpp nicht ausgefüllt wurde, sonst
	//		gib es einen draw command error
	
	auto drawable = std::make_shared<Drawable>();
	drawable->AddProperty(mesh);
	drawable->AddProperty(shaders);

	// root node
	auto root = std::make_shared<Node>();
	root->SetDrawable(drawable);
	root->GetTransformation()->translate(0,0,-5);
	root->UpdateTransformation();

	// optional camera
	auto camera = std::make_shared<Camera>();
	return std::make_shared<Scene>(root, camera);
}

class ARTRProject : public Project
{
public:
	void Setup(std::shared_ptr<SceneManager> sceneManager) override
	{
		// scene setup
		auto scene = SetupScene();
		// adding the scene to the manager
		sceneManager->AddScene(scene);
		// set as active scene to render
		sceneManager->SetActiveScene(scene);
	}
};

int main(int argc, char* argv[])
{
	// configure vre here
	VreConfiguration vreConfig{};
	// use the right handed system to be compliant with OpenGL shaders etc.
	vreConfig.mVulkanCfg.UseRightHandedSystem = true;
	vreConfig.mProject = std::make_shared<ARTRProject>();
	vreConfig.mApplicationName = "ARTR Exercise";
	vreConfig.mPathToResources = Filesystem::getResourcePath();

	return RunVre(argc, argv, vreConfig);
}