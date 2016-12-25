#include "mainform.h"
#include "ui_mainform.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QRegExpValidator>

namespace Browser
{

  MainForm::MainForm(QWidget *parent=nullptr, HistoryDialog *hd_=nullptr, Bookmark *bm_=nullptr) :
    QWidget {parent},
    ui {new Ui::MainForm}
  ,hd(hd_)
  ,bm(bm_)
  {
    ui->setupUi(this);
    //Установить валидатор на строку ввода URL
    QString regexp = "((?:https?|ftp)://\\S+)";
    QRegExp rx(regexp);
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->omniBox->setValidator(validator);
    //
    connect(ui->omniBox, &QLineEdit::returnPressed, this, &MainForm::go);
	connect(ui->pushMark, &QPushButton::clicked, this, &MainForm::Mark);
    //Соединение кнопок продвижения по истории посещения сайтов  с формой просмотра истории
//    connect(ui->toolButtonBack,SIGNAL(clicked(bool),hd,SLOT());

  }

  MainForm::~MainForm() = default;

  void MainForm::go()
  {
    QNetworkAccessManager manager;
    //Запись URL в историю посещений сайтов
    QString url= ui->omniBox->text();
    QString site_name = "Наименование сайта";
    hd->AddCurrentHistory(url,site_name);

//    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {ui->omniBox->text()}});
    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {url}});
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();
    QString html = response->readAll();
    html.remove(QRegExp("<[^>]*>"));
    ui->plainTextEditContent->setPlainText(html.simplified());
  }

  void MainForm::Mark(){
	  QNetworkAccessManager manager;
	  //Запись URL в историю посещений сайтов
	  QString url= ui->omniBox->text();
	  QString site_name = "Наименование сайта";
	  bm->AddCurrentBookmark(url,site_name);
  }

} // namespace Browser
