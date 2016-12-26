#include "mainform.h"
#include "ui_mainform.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QRegExpValidator>

#include "modulsystem.hpp"


namespace Browser
{

  MainForm::MainForm(QWidget *parent = nullptr, HistoryDialog *hd_ = nullptr, Bookmark *bm_ = nullptr) :
    QWidget {parent},
    ui {new Ui::MainForm}
  , hd(hd_)
  , bm(bm_)
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
    //Соединение кнопки Go со строкой ввода URL
    connect(ui->toolButtonGo, &QPushButton::clicked, ui->omniBox, &QLineEdit::returnPressed);
    //Соединение кнопки Reload со слотом запроса к текущему сайту
    connect(ui->toolButtonReload, &QPushButton::clicked, [this]() {
      SiteRequest(ui->omniBox->text());
    });
  }

  MainForm::~MainForm() = default;

  void MainForm::go()
  {
    QString url = ui->omniBox->text();

    if (SiteRequest(url)) {
      //Запись URL в историю посещений сайтов
      QString site_name = "Наименование сайта";
      hd->AddCurrentHistory(url, site_name);
    }
  }

  void MainForm::Mark()
  {
    //Р вЂ”Р В°Р С—Р С‘РЎРѓРЎРЉ URL Р Р† Р С‘РЎРѓРЎвЂљР С•РЎР‚Р С‘РЎР‹ Р С—Р С•РЎРѓР ВµРЎвЂ°Р ВµР Р…Р С‘Р в„– РЎРѓР В°Р в„–РЎвЂљР С•Р Р†
    QString url = ui->omniBox->text();
    QString site_name = "Наименование сайта";
    bm->AddCurrentBookmark(url, site_name);
  }

  bool MainForm::SiteRequest(QString url)
  {
    if (url.isEmpty())
      return false;

    //    QNetworkAccessManager manager;
    //    //    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {ui->omniBox->text()}});
    //    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {url}});
    //    QEventLoop event;
    //    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    //    event.exec();
    //    QString html = response->readAll();
    //    html.remove(QRegExp("<[^>]*>"));
    //    ui->plainTextEditContent->setPlainText(html.simplified());
    Network::NetworkRes res;
    res = Network::give_result(url);
    Parser::Tree tree = Parser::parse(&res);
    Parser::Tree::Tag root = boost::get<Parser::Tree::Tag>(tree.root);
    QWidget *get = Render::render(tree.root);

    if (get != nullptr) {
      ui->verticalLayout->addWidget(get);
      return true;
    } else
      return false;
  }

} // namespace Browser
