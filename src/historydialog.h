#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QString>
#include <QDebug>

namespace Ui
{
  class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit HistoryDialog(QWidget *parent = 0);
    ~HistoryDialog();

    void addUrl(QString url, QString site_name);
  private:
    Ui::HistoryDialog *ui;
    QTreeWidgetItem *addTreeRootDate(QTreeWidget *ptrTree);
    void DeleteUrl(QTreeWidgetItem *currentItem);
    void InsertItem(QTreeWidgetItem *parent, QString url, QString site_name);
  private slots:
    void RemoveItem();
    void RemoveItems();
    void SelectItem(QTreeWidgetItem *item, int index);
    void CallSite(QTreeWidgetItem *item, int index);
};

#endif // HISTORYDIALOG_H
