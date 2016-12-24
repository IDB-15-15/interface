#ifndef BROWSER_MAINFORM_H
#define BROWSER_MAINFORM_H

#include <QWidget>

#include <memory>
#include "historydialog.h"
//#include "mainwindow.h"


namespace Browser {

namespace Ui {
class MainForm;
}

class MainForm : public QWidget {
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent, HistoryDialog *hd_);

    virtual ~MainForm();

    void go();

private:
    std::unique_ptr<Ui::MainForm> ui;
    HistoryDialog *hd;
	QWidget* content = nullptr;
private slots:
};


} // namespace Browser

#endif // BROWSER_MAINFORM_H
