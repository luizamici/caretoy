#include "ct_logger.h"

CTLogger::CTLogger()
{
}

void CTLogger::initialize()
{
    // Create a layout
//    Log4Qt::LogManager::rootLogger();
    Log4Qt::TTCCLayout *p_layout = new Log4Qt::TTCCLayout();

    p_layout->setDateFormat("ISO8601");
    p_layout->setName(QLatin1String("My Layout"));
    p_layout->activateOptions();

    //TODO add a function for checking if the file exists,
    //in case it doesnt' it creates it
    //TODO add a function to check the file size
    //in case it's too big it creates a new one without canceling the old file
    Log4Qt::FileAppender *f_appender = new Log4Qt::FileAppender(
                p_layout,"/informatik/home/mici/git_caretoy/caretoy/log4qt/caretoy.log");
    f_appender->setName(QLatin1String("File appender"));
    f_appender->setAppendFile(true);
    f_appender->activateOptions();

    // Set appender on root logger
    Log4Qt::Logger::rootLogger()->addAppender(f_appender);

    Log4Qt::Logger::logger(QLatin1String("CTLogger"))->info(
                "File appender ready for being written.");
}
