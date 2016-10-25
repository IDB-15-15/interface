#include "mainform.h"
#include "ui_mainform.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace Browser {

MainForm::MainForm(QWidget *parent) :
    QWidget {parent},
    ui {new Ui::MainForm} {
    ui->setupUi(this);

    connect(ui->omniBox, &QLineEdit::returnPressed, this, &MainForm::go);
}

MainForm::~MainForm() = default;

void MainForm::go() {
    QNetworkAccessManager manager;
    QNetworkReply* response = manager.get(QNetworkRequest {QUrl {ui->omniBox->text()}});
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();

    QString html = response->readAll();
    html.remove(QRegExp("<[^>]*>"));

    ui->plainTextEditContent->setPlainText(html.simplified());
}

} // namespace Browser
