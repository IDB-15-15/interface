#include "messdialog.h"
#include <QTextCodec>

bool YNMess(QString mess)
{
    return (QMessageBox::question(nullptr, "Внимание !",
                            mess, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) ? true : false;
}

void WarnMess(QString mess)
{
    QMessageBox::warning(nullptr, "Предупреждение !",mess, QMessageBox::Close);
}

void InfoMess(QString mess)
{
    QMessageBox::information(nullptr,"Сообщение !",mess, QMessageBox::Close);
}

void ErrMess(QString mess)
{
    QMessageBox::critical(nullptr, "Ошибка !", mess, QMessageBox::Close);
}
