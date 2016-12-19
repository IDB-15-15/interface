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

  private:
    Ui::HistoryDialog *ui;
    QTreeWidgetItem *addTreeRootDate(QTreeWidget *ptrTree);
    void DeleteUrl (QTreeWidgetItem *currentItem);
    void InsertItem (QTreeWidgetItem *parent, QString url);
private slots:
    void addUrl(QTreeWidget *ptrTree,
                         QString url);
    void RemoveItem();
    void RemoveItems();
    void addUrlTest(QTreeWidget *ptrTree,
                    QString url)
{
        qDebug()<<url;
}

};

#endif // HISTORYDIALOG_H
