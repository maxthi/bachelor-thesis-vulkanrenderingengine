#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class VreMainWindow; }
QT_END_NAMESPACE

namespace vre
{
    /// @brief The VreMainWindow is a basic qt widget class that can be used together with the vre by calling
    /// RunVreGUI without template arguments. It uses the vre_mainwindow.ui as its gui file, which provides an
    /// empty window. This window can be extended by the user with all kinds of qt widgets.
    class VreMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        VreMainWindow(QWidget* parent = nullptr);
        ~VreMainWindow();

    private:
        Ui::VreMainWindow* ui;
    };

}