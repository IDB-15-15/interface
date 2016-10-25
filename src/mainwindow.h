#ifndef BROWSER_MAINWINDOW_H
#define BROWSER_MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>

#include <memory>

namespace Browser {

namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui;
};


} // namespace Browser
#endif // BROWSER_MAINWINDOW_H
