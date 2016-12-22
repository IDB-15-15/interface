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
      ui->tabWidget->addTab(new MainForm(this, hd), QString {});
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [this](int tab) {
      ui->tabWidget->removeTab(tab);
    });
    //соединение для вызова окна истории
//    connect(ui->btnHistory, &QPushButton::clicked, hd, &HistoryDialog::show, Qt::UniqueConnection);
     connect(ui->btnHistory, &QPushButton::clicked, [this]() {hd->show();});

    //
    emit ui->pushButton->clicked();
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
      SaveSettings();
      hd->SaveSettings();
      hd->SaveHistory();
      event->accept();
  }

  void MainWindow::SaveSettings()
  {
      settings.beginGroup(this->objectName());
      //    settings.setValue("geometry", saveGeometry());
      settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("state", saveState());
    settings.endGroup();
    settings.sync();
  }

  void MainWindow::LoadSettings()
  {
    //      qDebug() << "FILE" << fileName << " DOESN'T EXIST" << endl;
    if (QFile::exists(qApp->applicationName() + ".ini")) {
      settings.beginGroup(this->objectName());
//      restoreGeometry(settings.value("geometry").toByteArray());
      QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
      QSize size = settings.value("size", QSize(400, 400)).toSize();
      resize(size);
      move(pos);
      restoreState(settings.value("state", QByteArray()) .toByteArray());
      settings.endGroup();
    }
  }

  MainWindow::~MainWindow() = default;

} // namespace Browser
