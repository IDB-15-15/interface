#include "mainform.h"
#include "ui_mainform.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QRegExpValidator>

#include "parser.h"
#include "parsernode.hpp"

namespace Browser
{

  MainForm::MainForm(QWidget *parent, HistoryDialog *hd_) :
    QWidget {parent},
    ui {new Ui::MainForm}
  ,hd(hd_)
  {
    ui->setupUi(this);
    //UPD. URL на латинице. Если нужно распознавать и кириллические домены,
    //необходимо изменить все «a-z0-9» на «а-яёa-z0-9» и добавить в список доменных зон «рф» (от kvf77):
    //~^(?:(?:https?|ftp|telnet)://(?:[a-z0-9_-]{1,32}(?::[a-z0-9_-]{1,32})?@)?)?(?:(?:[a-z0-9-]{1,128}\.)+
    //(?:ru|su|com|net|org|mil|edu|arpa|gov|biz|info|aero|inc|name|[a-z]{2})|(?!0)(?:(?!0[^.]|255)[0-9]{1,3}\.)
    //{3}(?!0|255)[0-9]{1,3})(?:/[a-z0-9.,_@%&?+=\~/-]*)?(?:#[^ '\"&]*)?$~i
    //Установить валидатор на строку ввода URL
    //    QString regexp= "~^(?:(?:https?|ftp|telnet)://(?:[a-z0-9_-]{1,32}(?::[a-z0-9_-]{1,32})?@)?)?(?:(?:[a-z0-9-]{1,128}\.)+(?:ru|su|com|net|org|mil|edu|arpa|gov|biz|info|aero|inc|name|[a-z]{2})|(?!0)(?:(?!0[^.]|255)[0-9]{1,3}\.){3}(?!0|255)[0-9]{1,3})(?:/[a-z0-9.,_@%&?+=\~/-]*)?(?:#[^ '\"&]*)?$~i";
    QString regexp = "((?:https?|ftp|file)://\\S+)";
    //    QRegExp rx("((?:https?|ftp)://\\S+)");
    QRegExp rx(regexp);
    QValidator *validator = new QRegExpValidator(rx, this);
    //    QLineEdit *edit = new QLineEdit(this);
    //    edit->setValidator(validator);
    ui->omniBox->setValidator(validator);
    //
    connect(ui->omniBox, &QLineEdit::returnPressed, this, &MainForm::go, Qt::UniqueConnection);
  }

  MainForm::~MainForm() = default;

  void MainForm::go()
  {
    QNetworkAccessManager manager;
    //Запись URL в историю посещений сайтов
    QString url= ui->omniBox->text();
    hd->addUrl(url,"Наименование сайта",QDateTime::currentDateTime());
//    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {ui->omniBox->text()}});
    QNetworkReply *response = manager.get(QNetworkRequest {QUrl {url}});
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();
    QString html = response->readAll();
//    html.remove(QRegExp("<[^>]*>"));
//    ui->plainTextEditContent->setPlainText(html.simplified());

	// shit there
	if(content) {
		delete content;
		content = nullptr;
	}
	if(ui->plainTextEditContent) {
		delete ui->plainTextEditContent;
		ui->plainTextEditContent = nullptr;
	}

	std::shared_ptr<std::string> input = std::make_shared<std::string>(html.toUtf8().constData());
	Parser::Tree tree = Parser::parse(input);
	Parser::Tree::Tag root = boost::get<Parser::Tree::Tag>(tree.root);

    content = Render::render(tree.root);

	ui->verticalLayout->addWidget(content);
 }

} // namespace Browser
