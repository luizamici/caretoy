#ifndef CT_LOGGER_H
#define CT_LOGGER_H

#include <QSettings>
#include <QDebug>

#include "log4qt/consoleappender.h"
#include "log4qt/logger.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/logmanager.h"
#include "log4qt/helpers/initialisationhelper.h"
#include "log4qt/fileappender.h"

class CTLogger
{
public:
    CTLogger();

    void initialize();
};

#endif // CT_LOGGER_H
