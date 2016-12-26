#include "bookmark.h"
#include "ui_bookmark.h"

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
	BookmarkChanged(false), no_set_pointers(false)
{
	ui->setupUi(this);
		//Если сделаешь ui->tree->setExpandsOnDoubleClick ( false ), будешь получать сигнал.
	ui->free->setExpandsOnDoubleClick(false);

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

	if (!items.count())
{
	itm_ptr = addFreeRootDate(ptrFree);
	InsertItem(itm_ptr, url, site_name);
	}else
	InsertItem(items.first(), url, site_name);
	BookmarkChanged = true;
}

void Bookmark::InsertItem(QTreeWidgetItem *parent, QString url, QString site_name)
{
	if(parent->isExpanded() == false) parent->setExpanded(true);

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

void Bookmark::set_pointers2()
{
	std::queue<QObject *> qo_que;
	QWidget *qw;
	QObject *qo_ptr;
		//Обход дерева в ширину в MainWindow
	qo_que.push(this->parent());

	while(!qo_que.empty())
	{
		qo_ptr = qo_que.front();
		qo_que.pop();

			//Поиск по имени и инициализация указателя, если найдено
		if (qo_ptr->objectName().toLower() == "tabwidget")
		{
				//qDebug() << qo_ptr->objectName();
			qw = dynamic_cast<QTabWidget *>(qo_ptr)->currentWidget();
		 break;
		 }

		for (const auto &it : qo_ptr->children())
		{
			 qo_que.push(it);
		}
	}

		//Очистить очередь
	while (!qo_que.empty())
	{
		qo_que.pop();
	}

		//Обход дерева в ширину в текущей странице Tab
	qo_que.push(qw);

	while (!qo_que.empty())
	{
		qo_ptr = qo_que.front();
		qo_que.pop();

			//Поиск по имени и инициализация указателей, если найдено
		if (qo_ptr->objectName().toLower() == "omnibox")
			//qDebug() << qo_ptr->objectName();
		 qle = dynamic_cast<QLineEdit *>(qo_ptr);
		else if (qo_ptr->objectName().toLower() == "toolbuttonback")
			//qDebug() << qo_ptr->objectName();
		qtbb = dynamic_cast<QToolButton *>(qo_ptr);
		else if (qo_ptr->objectName().toLower() == "toolbuttonforward")
			//qDebug() << qo_ptr->objectName();
		qtbf = dynamic_cast<QToolButton *>(qo_ptr);

		for (const auto &it : qo_ptr->children())
		{
			qo_que.push(it);
		}
	}
}

void Bookmark::LoadSettings()
{
	 if (QFile::exists(qApp->applicationName() + ".ini"))
	 {
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

	if (!BookmarkFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		qWarning() << "Unable to open Bookmark file" << BookmarkFile.fileName();
		return;
	}

	QTextStream out(&BookmarkFile);

	QTreeWidgetItemIterator it(ui->free);

	while (*(++it))
	{
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

	if (!BookmarkFile.open(QFile::ReadOnly))
	{
		qWarning() << "Unable to open Bookmark file" << BookmarkFile.fileName();
		return;
	}

	QTextStream in(&BookmarkFile);

	while (!in.atEnd())
	{
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
	QTreeWidgetItemIterator it(ui->free);

	while (*(++it))
	{
		if ((*it)->checkState(CHECKED) == Qt::Checked)
		selected.append((*it));
	};
	foreach (QTreeWidgetItem *item, selected)
	{
		delete item;
		BookmarkChanged = true;
	}

	int cnt = ui->free->topLevelItemCount();
	QTreeWidgetItem *iptr;

	for (int i = 0; i < cnt; ++i)
	{
		iptr = ui->free->topLevelItem(i);

	if (iptr != nullptr && iptr->childCount() == 0)
	 ui->free->takeTopLevelItem(i);
	}
}

void Bookmark::CallSite(QTreeWidgetItem *item, int index)
{
	if (item != nullptr && item->parent() != nullptr)
	{
		QString url = item->text(URLS);
		qle->setText(url);
		emit qle->returnPressed();
		emit this->close();
	}
}

