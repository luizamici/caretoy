#ifndef CT_BLOCKCONFIG_H
#define CT_BLOCKCONFIG_H

#define CT_BLOCK_STICK 1
#define CT_BLOCK_RING 2
#define CT_BLOCK_FLOWER 3
#define CT_BLOCK_MICKEY 4
#define CT_BLOCK_U 5
#define CT_BLOCK_WALL_LEFT 6
#define CT_BLOCK_WALL_RIGHT 7
#define CT_BLOCK_WALL_SCREEN 8
#define CT_BLOCK_ARCH 9

#include <QtGui>
#include <QtXml>
#include "ct_confarch.h"
#include "ct_conftoyflower.h"
#include "ct_conftoymickey.h"
#include "ct_conftoyring.h"
#include "ct_conftoystick.h"
#include "ct_conftoyu.h"
#include "ct_confwallleft.h"
#include "ct_confwallright.h"
#include "ct_confwallscreen.h"

//Temporary
#include "ct_logger.h"

namespace Ui { class CTBlockConfig; }

class CTBlockConfig : public QDialog
{
    Q_OBJECT

public:
    explicit CTBlockConfig(QWidget *parent = 0);
    ~CTBlockConfig();
    void showParameters(int id, QString xml);

private:
    Ui::CTBlockConfig *ui;
    int id;

signals:
    void finishedConfig(QString xml);

private slots:
    void resetConfig();
    void saveConfiguration();
};

#endif // CT_BLOCKCONFIG_H
