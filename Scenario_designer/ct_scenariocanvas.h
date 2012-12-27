#ifndef CT_SCENARIOCANVAS_H
#define CT_SCENARIOCANVAS_H

#include <QtGui>
#include <QtXml>
#include "ct_simpleblock.h"
#include "TableOfScenarios/ct_dialog.h"

//temporary
#include "ct_dbconn.h"

class CTScenarioCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit CTScenarioCanvas(QWidget *parent = 0);
    void initialize();


private:
    QPoint dragStartPosition;
    QGridLayout *contentLayout;
    QList<CTSimpleBlock *> blocks;
    int getPositionIndex(QPoint eventPos, bool dropIndex);
    void updateBlockSequence();

    QString id_scenario;
    QString description;
    QString execution_day;
    QString execution_order;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    void resetScenario();
    void loadScenario();
    void saveScenario();
    void saveScenarioToDB();
    void loadScenario(QHash<QString,QString> scenario);

private slots:
    void getInfoAndSave(QString description, QString execution_day, QString execution_order);
    void getInfoAndSave();

signals:
    void save(QHash<QString,QString> scenario);
};

#endif // CT_SCENARIOCANVAS_H
