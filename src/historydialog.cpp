#include "historydialog.h"
#include "ui_historydialog.h"

#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QUrl>
//#include <QVariant>
#include "messdialog.h"
#include "mainwindow.h"
#include "mainform.h"
#include "queue"

enum column_no {HIDDEN = 0, CHECKED = 1, DATES = 2, SITE_NAMES = 3, URLS = 4};

HistoryDialog::HistoryDialog(QWidget *parent = nullptr):
  QDialog(parent),
  ui(new Ui::HistoryDialog),
  settings(QCoreApplication::applicationName() + QLatin1String(".ini") , QSettings::IniFormat),
  HistoryFileName(QCoreApplication::applicationName() + QLatin1String(".his")),
  HistoryChanged(false), no_set_pointers(false)

{
  ui->setupUi(this);
  //Считать настройки окна
  LoadSettings();
  //  ui->tree->setSortingEnabled(true);
  ui->tree->setAutoFillBackground(true);
  ui->tree->setColumnCount(5);
  ui->tree->setColumnHidden(HIDDEN, true);
  //Если сделаешь tv->setExpandsOnDoubleClick ( false ), будешь получать сигнал.
  ui->tree->setExpandsOnDoubleClick(false);
  //связываем кнопку RemoveAll с соответствующим слотом
  connect(ui->btnRemoveAll, SIGNAL(clicked(bool)), this, SLOT(RemoveItems()), Qt::UniqueConnection);
  //связываем кнопку Remove с соответствующим слотом
  connect(ui->btnRemove, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()), Qt::UniqueConnection);
  //Соединение для выбранной записи
  //  connect(ui->tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SelectItem(QTreeWidgetItem *, int)));
  connect(ui->tree, &QTreeWidget::itemDoubleClicked, this, &HistoryDialog::CallSite);
  //  connect(ui->tree, &QTreeWidget::itemDoubleClicked, [this](QTreeWidgetItem *item, int index){});
  //  Browser::MainForm(parent).omniBox->text()="" ;
  //test
  //  addUrl("http://lenta.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 20), QTime::currentTime()));
  //  addUrl("http://vz.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 20), QTime::currentTime()));
  //  addUrl("http://ria.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 20), QTime::currentTime()));
  //  addUrl("http://lenta.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 21), QTime::currentTime()));
  //  addUrl("http://vz.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 21), QTime::currentTime()));
  //  addUrl("http://ria.ru", "Наименование сайта", QDateTime(QDate(2016, 12, 21), QTime::currentTime()));
  //  addUrl("http://lenta.ru", "Наименование сайта", QDateTime(QDate(2016, 10, 22), QTime::currentTime()));
  //  addUrl("http://vz.ru", "Наименование сайта", QDateTime(QDate(2016, 10, 22), QTime::currentTime()));
  //  addUrl("http://ria.ru", "Наименование сайта", QDateTime(QDate(2016, 10, 22), QTime::currentTime()));
  //test
  LoadHistory();
}

HistoryDialog::~HistoryDialog()
{
  delete ui;
}

void HistoryDialog::AddCurrentHistory(QString url, QString site_name)
{
  addUrl(url, site_name, QDateTime::currentDateTime());
}

QTreeWidgetItem *HistoryDialog::addTreeRootDate(QTreeWidget *ptrTree, QDateTime date_time)
{
  QTreeWidgetItem *ptrItem;
  ptrItem = new QTreeWidgetItem(ptrTree);
  ptrItem->setText(HIDDEN, date_time.toString("yyyy.MM.dd hh:mm:ss"));
  ptrItem->setText(CHECKED, date_time.toString("dd.MM.yyyy"));
  ptrItem->setExpanded(true);
  return ptrItem;
}

void HistoryDialog::addUrl(QString url, QString site_name, QDateTime date_time)
{
  QString date;
  QTreeWidget *ptrTree = ui->tree;
  QTreeWidgetItem *itm_ptr;
  date = date_time.toString("dd.MM.yyyy");
  QList<QTreeWidgetItem *> items = ptrTree->findItems(date, Qt::MatchExactly, 1);

  if (!items.count()) {
	itm_ptr = addTreeRootDate(ptrTree, date_time);
	InsertItem(itm_ptr, url, site_name, date_time);
  } else
	InsertItem(items.first(), url, site_name, date_time);

  ptrTree->sortItems(HIDDEN, Qt::DescendingOrder);
  HistoryChanged = true;
}

void HistoryDialog::InsertItem(QTreeWidgetItem *parent, QString url, QString site_name, QDateTime date_time)
{
  if (parent->isExpanded() == false) parent->setExpanded(true);

  QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);
  //Устанавливаем состояние check
  newItem->setText(HIDDEN, date_time.toString("yyyy.MM.dd hh:mm:ss"));
  newItem->setCheckState(CHECKED, Qt::Unchecked);
  newItem->setText(DATES, date_time.toString("hh:mm"));
  newItem->setText(SITE_NAMES, site_name);
  newItem->setText(URLS, url);
  //test
  //  int MyURLData = Qt::UserRole + 1;
  //  newItem->setData(URLS, MyURLData, time);
  //  newItem->setData(URLS, MyURLData, QUrl(url));
  //test
  newItem->setExpanded(true);
}

void HistoryDialog::SaveSettings()
{
  settings.beginGroup(this->objectName());
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.endGroup();
  settings.sync();
}

void HistoryDialog::set_pointers()
{
  std::queue<QObject *> qo_que;
  QWidget *qw;
  QObject *qo_ptr;
  //Обход дерева в ширину в MainWindow
  qo_que.push(this->parent());

  while (!qo_que.empty()) {
	qo_ptr = qo_que.front();
	qo_que.pop();

	//Поиск по имени и инициализация указателя, если найдено
	if (qo_ptr->objectName().toLower() == "tabwidget") {
	  qDebug() << qo_ptr->objectName();
	  qw = dynamic_cast<QTabWidget *>(qo_ptr)->currentWidget();
	  break;
	}

	for (const auto &it : qo_ptr->children()) {
	  qo_que.push(it);
	}
  }

  //Очистить очередь
  while (!qo_que.empty()) {
	qo_que.pop();
  }

  //Обход дерева в ширину в текущей странице Tab
  qo_que.push(qw);

  while (!qo_que.empty()) {
	qo_ptr = qo_que.front();
	qo_que.pop();

	//Поиск по имени и инициализация указателей, если найдено
	if (qo_ptr->objectName().toLower() == "omnibox")
	  //      qDebug() << qo_ptr->objectName();
	  qle = dynamic_cast<QLineEdit *>(qo_ptr);
	else if (qo_ptr->objectName().toLower() == "toolbuttonback")
	  //      qDebug() << qo_ptr->objectName();
	  qtbb = dynamic_cast<QToolButton *>(qo_ptr);
	else if (qo_ptr->objectName().toLower() == "toolbuttonforward")
	  //      qDebug() << qo_ptr->objectName();
	  qtbf = dynamic_cast<QToolButton *>(qo_ptr);

	for (const auto &it : qo_ptr->children()) {
	  qo_que.push(it);
	}
  }

//  if (qle != nullptr)
//    qDebug() <<  "omnibox";
//  if (qtbb != nullptr)
//    qDebug() <<  "toolbuttonback";
//  if (qtbf != nullptr)
//    qDebug() <<  "toolbuttonforward";
}


void HistoryDialog::LoadSettings()
{
  if (QFile::exists(qApp->applicationName() + ".ini")) {
	settings.beginGroup(this->objectName());
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	settings.endGroup();
  }
}


void HistoryDialog::SaveHistory()
{
  //    QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
  if (!HistoryChanged)
	return;

  QFile historyFile(HistoryFileName);

  if (!historyFile.open(QFile::WriteOnly | QFile::Truncate)) {
	qWarning() << "Unable to open history file" << historyFile.fileName();
	return;
  }

  QTextStream out(&historyFile);
  //Проход с помощью итератора
  QTreeWidgetItemIterator it(ui->tree);

  while (*(++it)) {
	if ((*it)->parent() != nullptr)
	  out << (*it)->text(HIDDEN) << "\n" << (*it)->text(SITE_NAMES) << "\n" << (*it)->text(URLS) << "\n";
  };

  historyFile.close();

  HistoryChanged = false;
}
void HistoryDialog::LoadHistory()
{
  QDateTime date_time;
  QString line, site_name, url;
  QFile historyFile(HistoryFileName);

  if (!historyFile.exists())
	return;

  if (!historyFile.open(QFile::ReadOnly)) {
	qWarning() << "Unable to open history file" << historyFile.fileName();
	return;
  }

  QTextStream in(&historyFile);

  while (!in.atEnd()) {
	line = in.readLine();
	date_time = QDateTime::fromString(line, "yyyy.MM.dd hh:mm:ss");

	if (!date_time.isValid()) continue;

	line = in.readLine();
	site_name = line;
	line = in.readLine();
	url = line;
	addUrl(url, site_name, date_time);
  }

  historyFile.close();
}
void HistoryDialog::RemoveItem()
{
  int index;
  QList<QTreeWidgetItem *> selected;
  //Проход с помощью итератора
  QTreeWidgetItemIterator it(ui->tree);

  while (*(++it)) {
	if ((*it)->checkState(CHECKED) == Qt::Checked)
	  selected.append((*it));
  };

  foreach (QTreeWidgetItem *item, selected) {
	delete item;
	HistoryChanged = true;
  }

  int cnt = ui->tree->topLevelItemCount();
  QTreeWidgetItem *iptr;

  for (int i = 0; i < cnt; ++i) {
	iptr = ui->tree->topLevelItem(i);

	if (iptr != nullptr && iptr->childCount() == 0)
	  ui->tree->takeTopLevelItem(i);
  }
}
void HistoryDialog::RemoveItems()
{
  if (YNMess("Удалить все записи ?")) {
	ui->tree->clear();
	HistoryChanged = true;
  }
}
void HistoryDialog::CallSite(QTreeWidgetItem *item, int index)
{
  if (item != nullptr && item->parent() != nullptr) {
	QString url = item->text(URLS);
	qle->setText(url);
	emit qle->returnPressed();
	//    if (qtw != nullptr)
	//      emit qtw->setCurrentIndex(0);
	//    else
	//      qDebug() << "Tabwidget is absent";
	emit this->close();
  }
}

