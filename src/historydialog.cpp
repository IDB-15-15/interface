#include "historydialog.h"
#include "ui_historydialog.h"
#include <QDate>
#include <QTime>
#include <QUrl>
#include "messdialog.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HistoryDialog)
{
  ui->setupUi(this);
  ui->tree->setSortingEnabled(true);
  ui->tree->setAutoFillBackground(true);
  //  ui->tree->setColumnCount(4);
  //Если сделаешь tv->setExpandsOnDoubleClick ( false ), будешь получать сигнал.
  ui->tree->setExpandsOnDoubleClick(false);
  //связываем кнопку RemoveAll с соответствующим слотом
  connect(ui->btnRemoveAll, SIGNAL(clicked(bool)), this, SLOT(RemoveItems()), Qt::UniqueConnection);
  //связываем кнопку Remove с соответствующим слотом
  connect(ui->btnRemove, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()), Qt::UniqueConnection);
  //Соединение для выбранной записи
//    connect(ui->tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(SelectItem(QTreeWidgetItem *, int)));
  //test
//  addUrl("http://lenta.ru", "Наименование сайта");
//  addUrl("http://vz.ru", "Наименование сайта");
//  addUrl("http://ria.ru", "Наименование сайта");
  //
}

HistoryDialog::~HistoryDialog()
{
  delete ui;
}

QTreeWidgetItem *HistoryDialog::addTreeRootDate(QTreeWidget *ptrTree)
{
  QTreeWidgetItem *ptrItem;
  QString date;
  date = QDate::currentDate().toString("dd.MM.yyyy");
  ptrItem = new QTreeWidgetItem(ptrTree);
  ptrItem->setText(0, date);
  ptrItem->setExpanded(true);
  //  ptrItem->setDisabled(true);
  return ptrItem;
}

void HistoryDialog::addUrl(QString url, QString site_name)
{
  QString date;
  QTreeWidget *ptrTree = ui->tree;
  QTreeWidgetItem *itm_ptr;
  date = QDate::currentDate().toString("dd.MM.yyyy");
  QList<QTreeWidgetItem *> items = ptrTree->findItems(date, Qt::MatchExactly, 0);

  if (!items.count()) {
    itm_ptr = addTreeRootDate(ptrTree);
    InsertItem(itm_ptr, url, site_name);
  } else
    InsertItem(items.first(), url, site_name);

  ptrTree->sortByColumn(1, Qt::DescendingOrder);
}

void HistoryDialog::InsertItem(QTreeWidgetItem *parent, QString url, QString site_name)
{
  if (parent->isExpanded() == false) parent->setExpanded(true);

  QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);
  QString time;
  int MyURLData = Qt::UserRole + 1;
  time = QTime::currentTime().toString("hh:mm");
  //Устанавливаем состояние check
  newItem->setCheckState(0, Qt::Unchecked);
  newItem->setText(1, time);
  newItem->setText(2, site_name);
  newItem->setText(3, url);
  //  newItem->setData(4, MyURLData, QUrl(url));
  newItem->setExpanded(true);
}

void HistoryDialog::RemoveItem()
{
  int index;
  QList<QTreeWidgetItem *> selected;
  //Проход с помощью итератора
  QTreeWidgetItemIterator it(ui->tree);
//  QTreeWidgetItemIterator it(ui->tree->invisibleRootItem());

  while (*(++it)) {
    if ((*it)->checkState(0) == Qt::Checked)
      selected.append((*it));
  };

  foreach (QTreeWidgetItem *item, selected) {
    delete item;
  }

  int cnt = ui->tree->topLevelItemCount();

  for (int i = 0; i < cnt; ++i) {
    QTreeWidgetItem *iptr = ui->tree->topLevelItem(i);

    if (iptr->childCount() == 0)
      ui->tree->takeTopLevelItem(i);
  }
}

void HistoryDialog::RemoveItems()
{
  if (YNMess("Удалить все записи ?"))
    ui->tree->clear();
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

