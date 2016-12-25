#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QDialog>
#include <QTreeWidget>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QTabWidget>
#include <memory>
#include "ui_mainform.h"

namespace Ui
{
  class Bookmark;
}

class Bookmark : public QDialog
{
	Q_OBJECT

  public:
	explicit Bookmark(QWidget *parent);
	~Bookmark();

	 void addUrl2(QString url, QString site_name);
	void AddCurrentBookmark(QString url, QString site_name);
	void SaveBookmark();
	void SaveSettings2();
	void set_pointers2();

  private:
	Ui::Bookmark *ui;
	QSettings settings2;
	QString BookmarkFileName;
	bool BookmarkChanged;
	bool no_set_pointers;
	QLineEdit *qle;
	QToolButton *qtbb;
	QToolButton *qtbf;
	QTreeWidgetItem* current_item;

	QTreeWidgetItem *addFreeRootDate(QTreeWidget *ptrTree);
	void InsertItem(QTreeWidgetItem *parent, QString url, QString site_name);
	void LoadSettings();
	void LoadBookmark();

  private slots:
	void RemoveItem();
	void CallSite(QTreeWidgetItem *item, int index);
};


#endif // BOOKMARK_H
