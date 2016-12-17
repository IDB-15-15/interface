#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainform.h"
#include "tabbar.h"

const QString fileName = "MainWindow.ini";

namespace Browser
{

  MainWindow::MainWindow(QWidget *parent) :
    QMainWindow {parent},
    ui {new Ui::MainWindow},
  settings(fileName , QSettings::IniFormat)
  {
    ui->setupUi(this);
    //—читать настройки окна
    LoadSettings();
    connect(ui->pushButton, &QPushButton::clicked,
    [this] {
      ui->tabWidget->addTab(new MainForm {}, QString {});
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
    [this](int tab) {
      ui->tabWidget->removeTab(tab);
    });
    emit ui->pushButton->clicked();
  }

  void MainWindow::SaveSettings()
  {
    settings.beginGroup(this->objectName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();
    settings.sync();
  }

  void MainWindow::LoadSettings()
  {
    //      qDebug() << "FILE" << fileName << " DOESN'T EXIST" << endl;
    if (QFile::exists(fileName)) {
      settings.beginGroup(this->objectName());
      restoreGeometry(settings.value("geometry").toByteArray());
      restoreState(settings.value("state", QByteArray()) .toByteArray());
      settings.endGroup();
    }
  }

  MainWindow::~MainWindow() = default;

} // namespace Browser
