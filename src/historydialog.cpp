#include "historydialog.h"
#include "ui_historydialog.h"
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QUrl>
//#include <QVariant>
#include "messdialog.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HistoryDialog),
  settings(qApp->applicationName() + ".ini" , QSettings::IniFormat),
  HistoryFileName(QCoreApplication::applicationName() + QLatin1String(".his")),
  HistoryChanged(false)
{
  ui->setupUi(this);
  //Считать настройки окна
  LoadSettings();
  //  ui->tree->setSortingEnabled(true);
  ui->tree->setAutoFillBackground(true);
  ui->tree->setColumnCount(5);
  ui->tree->setColumnHidden(0, true);
  //Если сделаешь tv->setExpandsOnDoubleClick ( false ), будешь получать сигнал.
  ui->tree->setExpandsOnDoubleClick(false);
  //связываем кнопку RemoveAll с соответствующим слотом
  connect(ui->btnRemoveAll, SIGNAL(clicked(bool)), this, SLOT(RemoveItems()), Qt::UniqueConnection);
  //связываем кнопку Remove с соответствующим слотом
  connect(ui->btnRemove, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()), Qt::UniqueConnection);
  //связываем закрытие окна со слотами SaveSettings и SaveHistory
  connect(this, SIGNAL(rejected()), this, SLOT(SaveSettings()));
  connect(this, SIGNAL(rejected()), this, SLOT(SaveHistory()));
  //Соединение для выбранной записи
  //    connect(ui->tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(SelectItem(QTreeWidgetItem *, int)));
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

QTreeWidgetItem *HistoryDialog::addTreeRootDate(QTreeWidget *ptrTree, QDateTime date_time)
{
  QTreeWidgetItem *ptrItem;
  ptrItem = new QTreeWidgetItem(ptrTree);
  ptrItem->setText(0, date_time.toString("yyyy.MM.dd hh:mm:ss"));
  ptrItem->setText(1, date_time.toString("dd.MM.yyyy"));
  //  ptrItem->setText(4, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
  ptrItem->setExpanded(true);
  //  ptrItem->setDisabled(true);
  return ptrItem;
}

void HistoryDialog::addUrl(QString url, QString site_name, QDateTime date_time)
{
  QString date;
  QTreeWidget *ptrTree = ui->tree;
  QTreeWidgetItem *itm_ptr;
  //  date = QDate::currentDate().toString("dd.MM.yyyy");
  date = date_time.toString("dd.MM.yyyy");
  QList<QTreeWidgetItem *> items = ptrTree->findItems(date, Qt::MatchExactly, 1);

  if (!items.count()) {
    //    itm_ptr = addTreeRootDate(ptrTree);
    itm_ptr = addTreeRootDate(ptrTree, date_time);
    InsertItem(itm_ptr, url, site_name, date_time);
  } else
    InsertItem(items.first(), url, site_name, date_time);

  ptrTree->sortItems(0, Qt::DescendingOrder);
  HistoryChanged = true;
  //  ptrTree->sortItems(4, Qt::DescendingOrder);
  //  ptrTree->sortByColumn(4, Qt::DescendingOrder);
}

void HistoryDialog::InsertItem(QTreeWidgetItem *parent, QString url, QString site_name, QDateTime date_time)
{
  if (parent->isExpanded() == false) parent->setExpanded(true);

  QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);
  //Устанавливаем состояние check
  newItem->setText(0, date_time.toString("yyyy.MM.dd hh:mm:ss"));
  newItem->setCheckState(1, Qt::Unchecked);
  newItem->setText(2, date_time.toString("hh:mm"));
  newItem->setText(3, site_name);
  newItem->setText(4, url);
  //test
  //  int MyURLData = Qt::UserRole + 1;
  //  newItem->setData(2, MyURLData, time);
  //
  //  newItem->setData(2, MyURLData, QUrl(url));
  newItem->setExpanded(true);
}

void HistoryDialog::SaveSettings()
{
  settings.beginGroup(this->objectName());
  //  settings.setValue("geometry", saveGeometry());
  //  settings.setValue("state", saveState());
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.endGroup();
  settings.sync();
}

void HistoryDialog::LoadSettings()
{
  if (QFile::exists(qApp->applicationName() + ".ini")) {
    settings.beginGroup(this->objectName());
    //    restoreGeometry(settings.value("geometry").toByteArray());
    //    restoreState(settings.value("state", QByteArray()) .toByteArray());
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
  //    if (directory.isEmpty())
  //        directory = QDir::homePath() + QLatin1String("/.") + QCoreApplication::applicationName();
  //    if (!QFile::exists(directory)) {
  //        QDir dir;
  //        dir.mkpath(directory);
  //    }
  //    QFile historyFile(directory + QLatin1String("/history"));
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
      out << (*it)->text(0) << "\n" << (*it)->text(3) << "\n" << (*it)->text(4) << "\n";
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
  //  QTreeWidgetItemIterator it(ui->tree->invisibleRootItem());

  while (*(++it)) {
    if ((*it)->checkState(1) == Qt::Checked)
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
  //    m_ui->textBrowser->setSource(item->data(0, MyURLData ).toUrl());
}
void HistoryDialog::SelectItem(QTreeWidgetItem *item, int index)
{
  //  if (item->checkState(0) == Qt::Checked)
  //    item->setSelected(true);
  //  else
  //    item->setSelected(false);
}

