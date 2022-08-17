#include "vre.hpp"

#include "vulkan_window.hpp"
#include "vre_mainwindow.hpp"

#include <QApplication>
#include <QLoggingCategory>
#include <exception>


Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

namespace vre
{
    int RunVre(int argc, char* argv[], VreConfiguration& vreConfig)
    {
        QApplication app(argc, argv);
        QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

        VreCore::GetInstance()->SetConfig(&vreConfig);
        
        vre::VulkanWindow w;
        w.setTitle(vreConfig.mApplicationName.c_str());
        w.resize(vreConfig.mWindowWidth, vreConfig.mWindowHeight);
        w.show();
        return app.exec();
    }

    int RunVreGUI(int argc, char* argv[], VreConfiguration& vreConfig)
    {
        QApplication a(argc, argv);
        VreMainWindow w;
        w.show();

        vreConfig.mIsEmbeddedInGUI = true;
        vreConfig.mHostWindow = &w;
        VreCore::GetInstance()->SetConfig(&vreConfig);

        vre::VulkanWindow* window = new vre::VulkanWindow();
        w.setCentralWidget(w.createWindowContainer(window, &w));

        w.setWindowTitle(vreConfig.mApplicationName.c_str());
        w.resize(vreConfig.mWindowWidth, vreConfig.mWindowHeight);
        w.show();
        return a.exec();
    }
}