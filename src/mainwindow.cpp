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
  settings(qApp->applicationName() + ".ini" , QSettings::IniFormat),hd(nullptr)
  {
    ui->setupUi(this);
    //������� ��������� ����
    LoadSettings();
    connect(ui->pushButton, &QPushButton::clicked,
    [this] {
      ui->tabWidget->addTab(new MainForm {}, QString {});
    });
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
    [this](int tab) {
      ui->tabWidget->removeTab(tab);
    });
    //���������� ��� ������ ���� �������
    connect(ui->btnHistory, SIGNAL(clicked(bool)), this, SLOT(ShowHistory()), Qt::UniqueConnection);
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

  void MainWindow::ShowHistory()
  {
    //������� ���� �������
    //    HistoryDialog *hd = new HistoryDialog(this);
    if (hd == nullptr)
      hd = new HistoryDialog(this);

    hd->show();
    //    delete hd;
  }

  MainWindow::~MainWindow() = default;

} // namespace Browser
