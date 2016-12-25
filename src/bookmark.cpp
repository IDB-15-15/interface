#include "bookmark.h"
#include "ui_bookmark.h"

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QUrl>
#include "messdialog.h"
#include "mainwindow.h"
#include "mainform.h"
#include "queue"

enum column_no { CHECKED = 0, SITE_NAMES = 1, URLS = 2};

Bookmark::Bookmark(QWidget *parent = nullptr):
  QDialog(parent),
  ui(new Ui::Bookmark),
  settings2(QCoreApplication::applicationName() + QLatin1String(".ini") , QSettings::IniFormat),
  BookmarkFileName(QCoreApplication::applicationName() + QLatin1String(".his")),
  BookmarkChanged(false), no_set_pointers(false)

{
  ui->setupUi(this);

  LoadSettings();

  ui->free->setAutoFillBackground(true);
  ui->free->setColumnCount(3);

  ui->free->setExpandsOnDoubleClick(false);

  connect(ui->abcRemove, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()), Qt::UniqueConnection);

  connect(ui->free, &QTreeWidget::itemDoubleClicked, this, &Bookmark::CallSite);

  LoadBookmark();
}

Bookmark::~Bookmark()
{
  delete ui;
}

void Bookmark::AddCurrentBookmark(QString url, QString site_name)
{
  addUrl2(url, site_name);
}

QTreeWidgetItem *Bookmark::addFreeRootDate(QTreeWidget *ptrFree)
{
  QTreeWidgetItem *ptrItem;
  ptrItem = new QTreeWidgetItem(ptrFree);
  ptrItem->setExpanded(true);
  return ptrItem;
}

void Bookmark::addUrl2(QString url, QString site_name)
{
  QString date;
  QTreeWidget *ptrFree = ui->free;
  QTreeWidgetItem *itm_ptr;
  QList<QTreeWidgetItem *> items = ptrFree->findItems(date, Qt::MatchExactly,1);

  if (!items.count()) {
	itm_ptr = addFreeRootDate(ptrFree);
	InsertItem(itm_ptr, url, site_name);
  } else
	InsertItem(items.first(), url, site_name);
  BookmarkChanged = true;
}

void Bookmark::InsertItem(QTreeWidgetItem *parent, QString url, QString site_name)
{
  if (parent->isExpanded() == false) parent->setExpanded(true);

  QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);

  newItem->setText(SITE_NAMES, site_name);
  newItem->setCheckState(CHECKED, Qt::Unchecked);
  newItem->setText(URLS, url);
  newItem->setExpanded(true);
}

void Bookmark::SaveSettings2()
{
  settings2.beginGroup(this->objectName());
  settings2.setValue("pos", pos());
  settings2.setValue("size", size());
  settings2.endGroup();
  settings2.sync();
}

void Bookmark::LoadSettings()
{
  if (QFile::exists(qApp->applicationName() + ".ini")) {
	settings2.beginGroup(this->objectName());
	QPoint pos = settings2.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings2.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	settings2.endGroup();
  }
}


void Bookmark::SaveBookmark()
{

  if (!BookmarkChanged)
	return;

  QFile BookmarkFile("BookmarkFileName.txt");

  if (!BookmarkFile.open(QFile::WriteOnly | QFile::Truncate)) {
	qWarning() << "Unable to open Bookmark file" << BookmarkFile.fileName();
	return;
  }

  QTextStream out(&BookmarkFile);
  //Проход с помощью итератора
  QTreeWidgetItemIterator it(ui->free);

  while (*(++it)) {
	if ((*it)->parent() != nullptr)
	  out << (*it)->text(SITE_NAMES) << "\n" << (*it)->text(URLS) << "\n";
  };

  BookmarkFile.close();

  BookmarkChanged = false;
}

void Bookmark::LoadBookmark()
{
  QString line, site_name, url;
  QFile BookmarkFile("BookmarkFileName.txt");

  if (!BookmarkFile.exists())
	return;

  if (!BookmarkFile.open(QFile::ReadOnly)) {
	qWarning() << "Unable to open Bookmark file" << BookmarkFile.fileName();
	return;
  }

  QTextStream in(&BookmarkFile);

  while (!in.atEnd()) {

	line = in.readLine();
	site_name = line;
	line = in.readLine();
	url = line;
	addUrl2(url, site_name);
  }

  BookmarkFile.close();
}
void Bookmark::RemoveItem()
{
  int index;
  QList<QTreeWidgetItem *> selected;
  //Проход с помощью итератора
  QTreeWidgetItemIterator it(ui->free);
  while (*(++it)) {
	if ((*it)->checkState(CHECKED) == Qt::Checked)
	  selected.append((*it));
  };
  foreach (QTreeWidgetItem *item, selected) {
	delete item;
	BookmarkChanged = true;
  }

  int cnt = ui->free->topLevelItemCount();
  QTreeWidgetItem *iptr;

  for (int i = 0; i < cnt; ++i) {
	iptr = ui->free->topLevelItem(i);

	if (iptr != nullptr && iptr->childCount() == 0)
	  ui->free->takeTopLevelItem(i);
  }
}
void Bookmark::CallSite(QTreeWidgetItem *item, int index)
{
  if (item != nullptr && item->parent() != nullptr) {
	QString url = item->text(URLS);
	qle->setText(url);
	emit qle->returnPressed();

	emit this->close();
  }
}

