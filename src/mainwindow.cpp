#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainform.h"
#include "tabbar.h"

namespace Browser {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow {parent},
    ui {new Ui::MainWindow} {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked,
            [this] {
        ui->tabWidget->addTab(new MainForm {}, QString {});
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            [this] (int tab) {
        ui->tabWidget->removeTab(tab);
    });

    emit ui->pushButton->clicked();
}

MainWindow::~MainWindow() = default;

} // namespace Browser
