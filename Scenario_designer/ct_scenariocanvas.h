#ifndef CT_SCENARIOCANVAS_H
#define CT_SCENARIOCANVAS_H

#include <QtGui>
#include <QtXml>
#include "ct_simpleblock.h"

//temporary
#include "ct_dbconn.h"
#include "ct_logger.h"

class CTScenarioCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioCanvas(QWidget *parent = 0);
    void initialize();
    bool isNewScenario();
    QString getDescription();
    QString getExecutionDay();
    QString getExecutionOrder();


private:
    QPoint dragStartPosition;
    QGridLayout *contentLayout;
    QList<CTSimpleBlock *> blocks;
    int getPositionIndex(QPoint eventPos, bool dropIndex);
    void updateBlockSequence();


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    void resetScenario();
    void loadScenario();

private slots:
    void getInfoAndSave();

signals:
    void save(QHash<QString,QString> scenario);
};

#endif // CT_SCENARIOCANVAS_H
