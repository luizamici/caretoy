#ifndef CT_TRASHCAN_H
#define CT_TRASHCAN_H

#include <QtGui>

class CTTrashCan : public QFrame
{
    Q_OBJECT

public:
    explicit CTTrashCan(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // CT_TRASHCAN_H
