#ifndef CT_SCENARIOCANVAS_H
#define CT_SCENARIOCANVAS_H

#include <QtGui>
#include <QtXml>
#include "ct_simpleblock.h"

//temporary
#include "ct_logger.h"

class CTScenarioCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioCanvas(QWidget *parent = 0);
    void initialize();
    bool isNewScenario();

private:
    QPoint dragStartPosition;
    QGridLayout *contentLayout;
    QList<CTSimpleBlock *> blocks;
    int getPositionIndex(QPoint eventPos, bool dropIndex);
    void updateBlockSequence();

    QString outcome_measures;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    void resetScenario();
    void loadScenarioFromFile();
    void loadScenario();
    void saveScenario();
    void calculateRuntime();

signals:
    void save(QHash<QString,QString> scenario);
    void titleChanged(QString newTitle);
    void updateRuntime(double tot_runtime);
};

#endif // CT_SCENARIOCANVAS_H
