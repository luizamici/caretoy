#ifndef CT_BLOCKSOURCE_H
#define CT_BLOCKSOURCE_H

#include <QtGui>
#include <QtXml>
#include "ct_simpleblock.h"

class CTBlockSource : public QWidget
{
    Q_OBJECT

public:
    explicit CTBlockSource(QWidget *parent = 0);
    void initialize();

private:
    QPoint dragStartPosition;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // CT_BLOCKSOURCE_H
