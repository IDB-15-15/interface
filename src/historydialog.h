#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QtGui>

namespace Ui
{
  class HistoryDialog;
}
/*
class myQTreeWidgetItem: public QTreeWidgetItem //: QTreeWidgetItem()
{
  public:
    myQTreeWidgetItem(QTreeWidgetItem *parent=nullptr): QTreeWidgetItem(parent) {}
    bool operator<(const myQTreeWidgetItem &other) const
    {
      int column = treeWidget()->sortColumn();

      // можно проверить, что находится в data(column, Qt::UserRole) и вызвать
      // свой оператор сравнения для этих данных, например вот так:
      if (!data(column, Qt::UserRole).isNull())
        if (!other.data(column, Qt::UserRole).isNull())
          return data(column, Qt::UserRole).toDateTime() < other.data(column, Qt::UserRole).toDateTime();

      const QVariant v1 = data(column, Qt::DisplayRole);
      const QVariant v2 = other.data(column, Qt::DisplayRole);
      return v1.toString() < v2.toString();
    }
};
*/
class HistoryDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit HistoryDialog(QWidget *parent = 0);
    ~HistoryDialog();

    void addUrl(QString url, QString site_name, QDateTime date_time);
  private:
    Ui::HistoryDialog *ui;
    QSettings settings;
    QString HistoryFileName;
    bool HistoryChanged;

    QTreeWidgetItem *addTreeRootDate(QTreeWidget *ptrTree, QDateTime date_time);
    void DeleteUrl(QTreeWidgetItem *currentItem);
    void InsertItem(QTreeWidgetItem *parent, QString url, QString site_name,QDateTime date_time);
    void LoadSettings();

    //    загрузка истории из файла
    void LoadHistory();

  private slots:
    void SaveSettings();
    void RemoveItem();
    void RemoveItems();
    // сохранение истории в файле
    void SaveHistory();
    void SelectItem(QTreeWidgetItem *item, int index);
    void CallSite(QTreeWidgetItem *item, int index);
};

#endif // HISTORYDIALOG_H
