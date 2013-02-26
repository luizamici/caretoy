#ifndef CT_SCENARIOCANVAS_H
#define CT_SCENARIOCANVAS_H

#include <QtGui>
#include <QtXml>
#include "ct_simpleblock.h"
#include "TableOfScenarios/ct_dialog.h"

//temporary
#include "TableOfScenarios/ct_dbconn.h"
#include "TableOfScenarios/ct_logger.h"

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
    void clear();

    QString id_scenario;
    QString description;
    QString creation_date;
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
    void loadScenarioFromFile();
    void loadScenario(QHash<QString,QString> scenario);
    void saveScenario(QString description, QString execution_day, QString execution_order);

signals:
    void save(QHash<QString,QString> scenario);
    void titleChanged(QString newTitle);
};

#endif // CT_SCENARIOCANVAS_H
