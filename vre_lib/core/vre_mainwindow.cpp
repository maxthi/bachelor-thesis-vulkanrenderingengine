#include "vre_mainwindow.hpp"
#include "./ui_vre_mainwindow.h"

#include "vre_core.hpp"

namespace vre
{
    VreMainWindow::VreMainWindow(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::VreMainWindow)
    {
        ui->setupUi(this);
    }

    VreMainWindow::~VreMainWindow()
    {
        delete ui;
    }
}

