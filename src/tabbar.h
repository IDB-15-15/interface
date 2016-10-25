#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>
#include <QMoveEvent>

namespace Browser {

class TabBar final : public QTabBar {
public:
    TabBar();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};

}

#endif // TABBAR_H
