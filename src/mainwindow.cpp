#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainform.h"
#include "tabbar.h"
//#include "historydialog.h"

namespace Browser
{

  MainWindow::MainWindow(QWidget *parent) :
    QMainWindow {parent},
    ui {new Ui::MainWindow},
  settings(qApp->applicationName() + ".ini" , QSettings::IniFormat),
    hd(new HistoryDialog(this))
  {
    ui->setupUi(this);
    //Считать настройки окна
    LoadSettings();
    connect(ui->pushButton, &QPushButton::clicked,
    [this] {
      ui->tabWidget->addTab(new MainForm (this,hd), QString {});
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
    [this](int tab) {
      ui->tabWidget->removeTab(tab);
    });
    //соединение для вызова окна истории
    connect(ui->btnHistory, SIGNAL(clicked(bool)), hd, SLOT(show()), Qt::UniqueConnection);
    //
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
    if (QFile::exists(qApp->applicationName() + ".ini")) {
      settings.beginGroup(this->objectName());
      restoreGeometry(settings.value("geometry").toByteArray());
      restoreState(settings.value("state", QByteArray()) .toByteArray());
      settings.endGroup();
    }
  }

  MainWindow::~MainWindow() = default;

} // namespace Browser
