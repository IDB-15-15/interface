#ifndef BROWSER_MAINWINDOW_H
#define BROWSER_MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QtGui>

#include <memory>
#include "historydialog.h"

namespace Browser
{

  namespace Ui
  {
    class MainWindow;
  }

  class MainWindow final : public QMainWindow
  {
      Q_OBJECT

    public:
      explicit MainWindow(QWidget *parent = 0);
      virtual ~MainWindow();

    private:
      void closeEvent(QCloseEvent *event)
      {
        SaveSettings();
        event->accept();
      }
      void SaveSettings();
      void LoadSettings();
    private:
      std::unique_ptr<Ui::MainWindow> ui;
      QSettings settings;
      HistoryDialog *hd;
    private slots:
  };


} // namespace Browser
#endif // BROWSER_MAINWINDOW_H
