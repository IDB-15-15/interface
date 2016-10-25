#ifndef BROWSER_MAINFORM_H
#define BROWSER_MAINFORM_H

#include <QWidget>

#include <memory>

namespace Browser {

namespace Ui {
class MainForm;
}

class MainForm : public QWidget {
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    virtual ~MainForm();

    void go();

private:
    std::unique_ptr<Ui::MainForm> ui;
};


} // namespace Browser

#endif // BROWSER_MAINFORM_H
