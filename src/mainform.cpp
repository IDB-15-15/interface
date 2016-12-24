#include "mainform.h"
#include "ui_mainform.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QRegExpValidator>

namespace Browser
{

  MainForm::MainForm(QWidget *parent=nullptr, HistoryDialog *hd_=nullptr) :
    QWidget {parent},
    ui {new Ui::MainForm}
  ,hd(hd_)
  {
    ui->setupUi(this);
    //Установить валидатор на строку ввода URL
    QString regexp = "((?:https?|ftp)://\\S+)";
    QRegExp rx(regexp);
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->omniBox->setValidator(validator);
    //Инициализируем указатели на строку URL и кнопки продвижения по истории посещения сайтов
    hd->set_pointers(ui->omniBox,ui->toolButtonBack,ui->toolButtonForward);
    connect(ui->omniBox, &QLineEdit::returnPressed, this, &MainForm::go);
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

} // namespace Browser
