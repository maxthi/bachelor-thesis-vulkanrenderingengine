#include "project.hpp"
#include "vre_core.hpp"
#include "../input_manager/vre_input_listener.hpp"

namespace vre
{
	void Project::CompleteSetup(QWindow* window)
	{
        // allow the project to allocate memory and init scenes
        auto sceneManager = SceneManager::GetInstance();
        Setup(sceneManager);

        // get renderer
        m_renderer = Renderer::VreRenderer::VreRenderer::GetInstance();
        
        auto sceneReader = sceneManager->GetActiveScene()->GetSceneReader();
        sceneReader->ReadComplete();
        m_renderer->SetRenderLists(sceneReader->GetListOfRenderLists());

        // init renderer based on RenderList
        m_renderer->Initialize();

        InputManager::GetInstance()->RegisterListener(std::make_shared<VreInputListener>(window, this));
	}

    void Project::CompleteCleanup()
    {
        if (m_renderer != nullptr)
        {
            m_renderer->Destroy();
        }

        Cleanup();
    }

    void Project::CompleteUpdate()
    {
        // call user specific update code
        Update();
        
        // draw current frame
        m_renderer->DrawFrame();

        // update the camera
        SceneManager::GetInstance()->GetActiveScene()->GetCamera()->Update();
    }
}