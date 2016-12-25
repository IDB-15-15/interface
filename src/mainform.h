#ifndef BROWSER_MAINFORM_H
#define BROWSER_MAINFORM_H

#include <QWidget>

#include <memory>
#include "historydialog.h"
#include "bookmark.h"
//#include "mainwindow.h"


namespace Browser {

namespace Ui {
class MainForm;
}

class MainForm : public QWidget {
    Q_OBJECT

public:
	explicit MainForm(QWidget *parent, HistoryDialog *hd_, Bookmark *bm_);

    virtual ~MainForm();

    void go();

	void Mark();

private:
    std::unique_ptr<Ui::MainForm> ui;
    HistoryDialog *hd;
	Bookmark *bm;
	QWidget* content = nullptr;
private slots:
    void SiteRequest(QString url);
};


} // namespace Browser

#endif // BROWSER_MAINFORM_H
