#ifndef CT_LOGGER_H
#define CT_LOGGER_H

#include <QtDebug>
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

    void initialize(QString filePath);
};

#endif // CT_LOGGER_H
