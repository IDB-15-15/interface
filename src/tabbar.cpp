#include "tabbar.h"

namespace Browser {

TabBar::TabBar() {
    setMovable(true);
    setTabsClosable(true);

    insertTab(count() - 1, QString {});

    setTabEnabled(count() - 1, false);
    setTabButton(count() - 1, QTabBar::RightSide, nullptr);
}

void TabBar::mouseMoveEvent(QMouseEvent* event) {

}

void TabBar::mousePressEvent(QMouseEvent* event) {
    const int tab = tabAt(event->pos());

}

void TabBar::mouseReleaseEvent(QMouseEvent* event) {

}

}
