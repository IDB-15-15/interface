#include "historydialog.h"
#include "ui_historydialog.h"
#include <QDate>
#include <QTime>
#include "messdialog.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HistoryDialog)
{
  ui->setupUi(this);
  ui->tree->setSortingEnabled(true);
  ui->tree->setAutoFillBackground(true);
  //связываем кнопку RemoveAll с соответствующим слотом
  connect(ui->btnRemoveAll, SIGNAL(clicked(bool)), this, SLOT(RemoveItems()), Qt::UniqueConnection);
  //связываем кнопку Remove с соответствующим слотом
  connect(ui->btnRemove, SIGNAL(clicked(bool)), this, SLOT(RemoveItem()), Qt::UniqueConnection);
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
  return ptrItem;
}

void HistoryDialog::addUrl(QTreeWidget *ptrTree, QString url)
{
  QString date;
  date = QDate::currentDate().toString("dd.MM.yyyy");
  QList<QTreeWidgetItem *> items = ptrTree->findItems(date, Qt::MatchExactly, 0);

  if (!items.count()) {
    InsertItem(addTreeRootDate(ptrTree), url);
  } else
    InsertItem(items.first(), url);
}

void HistoryDialog::InsertItem(QTreeWidgetItem *parent, QString url)
{
  if (parent->isExpanded() == false) parent->setExpanded(true);

  QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);
  QString time;
  time = QTime::currentTime().toString("hh:mm");
  //Устанавливаем состояние check
  newItem->setCheckState(0, Qt::Unchecked);
  newItem->setText(1, time);
  newItem->setText(2, url);
  newItem->setExpanded(true);
}

void HistoryDialog::RemoveItem()
{
  QString url;
  url = "http:\\www.lenta.ru";
  addUrl(ui->tree, url);
}

void HistoryDialog::RemoveItems()
{
  if (YNMess("Удалить все записи ?"))
    ui->tree->clear();
}

