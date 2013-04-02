#include "ct_blockconfig.h"
#include "ui_ct_blockconfig.h"

CTBlockConfig::CTBlockConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTBlockConfig)
{
    ui->setupUi(this);

    ui->qlb_block_image->setPixmap(QPixmap(":/images/ct_block.png"));

    connect(ui->qpb_reset, SIGNAL(clicked()), this, SLOT(resetConfig()));
    connect(ui->qpb_save, SIGNAL(clicked()), this, SLOT(saveConfiguration()));
    connect(ui->qpb_cancel, SIGNAL(clicked()), this, SLOT(close()));
}

CTBlockConfig::~CTBlockConfig()
{
    delete ui;
}


void CTBlockConfig::showParameters(int id, QString xml)
{
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Entering CTBlockConfig::showParameters ... ");
    this->id = id;

    switch (id) {
//    case CT_BLOCK_FLOWER:
//    {
//        CTConfToyFlower *config = new CTConfToyFlower();
//        ui->qsa_config->setWidget(config);
//        if (!xml.isEmpty()) {
//            if(!config->setParameters(xml))
//               Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                            "CTBlockConfig::showParameters -> flower block"
//                            "parameters not set ");
//        }
//        else
//        {
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                         "CTBlockConfig::showParameters -> empty parameters passed to flower block");
//        }
//        break;
//    }
    case CT_BLOCK_MICKEY:
    {
        CTConfToyMickey *config = new CTConfToyMickey();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> mickey block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to mickey block");
        }
        break;
    }
//    case CT_BLOCK_RING:
//    {
//        CTConfToyRing *config = new CTConfToyRing();
//        ui->qsa_config->setWidget(config);
//        if(!xml.isEmpty())
//        {
//            if(!config->setParameters(xml))
//                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                             "CTBlockConfig::showParameters -> ring block parameters not set ");
//        }
//        else
//        {
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                         "CTBlockConfig::showParameters -> empty parameters passed to ring block");
//        }
//        break;
//    }
//    case CT_BLOCK_STICK:
//    {
//        CTConfToyStick *config = new CTConfToyStick();
//        ui->qsa_config->setWidget(config);
//        if(!xml.isEmpty())
//        {
//            if(!config->setParameters(xml))
//                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                             "CTBlockConfig::showParameters -> stick block parameters not set ");
//        }
//        else
//        {
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                         "CTBlockConfig::showParameters -> empty parameters passed to stick block");
//        }
//        break;
//    }
    case CT_BLOCK_U:
    {
        CTConfToyU *config = new CTConfToyU();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> utoy block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to utoy block");
        }
        break;
    }
    case CT_BLOCK_ARCH:
    {
        CTConfArch *config = new CTConfArch();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> arch block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to arch block");
        }
        break;
    }
    case CT_BLOCK_WALL_SCREEN:
    {
        CTConfWallScreen *config = new CTConfWallScreen();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> screen block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to wall_screen block");
        }
        break;
    }
    case CT_BLOCK_WALL_LEFT:
    {
        CTConfWallLeft *config = new CTConfWallLeft();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> wall left block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to wall_left block");
        }
        break;
    }
    case CT_BLOCK_WALL_RIGHT:
    {
        CTConfWallRight *config = new CTConfWallRight();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> wall right block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to wall_right block");
        }
        break;
    }
    case CT_BLOCK_LRING:
    {
        CTConfToyLRing *config = new CTConfToyLRing();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> large ring block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to large_ring block");
        }
        break;
    }
    case CT_BLOCK_SRING:
    {
        CTConfToySRing *config = new CTConfToySRing();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
                Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                             "CTBlockConfig::showParameters -> small ring block parameters not set ");
        }
        else
        {
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                         "CTBlockConfig::showParameters -> empty parameters passed to small_ring block");
        }
        break;
    }
    default:
        break;
    }
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Exit CTBlockConfig::showParameters . ");
}


void CTBlockConfig::resetConfig()
{
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Entering CTBlockConfig::resetConfig ... ");
    switch (id) {
//    case CT_BLOCK_STICK:
//    {
//        delete ui->qsa_config->widget();
//        CTConfToyStick *config = new CTConfToyStick();
//        ui->qsa_config->setWidget(config);
//        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
//                    "CTBlockConfig::resetConfig resetting stick block. ");
//        break;
//    }
//    case CT_BLOCK_RING:
//    {
//        delete ui->qsa_config->widget();
//        CTConfToyRing *config = new CTConfToyRing();
//        ui->qsa_config->setWidget(config);
//        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
//                    "CTBlockConfig::resetConfig resetting ring block. ");
//        break;
//    }
//    case CT_BLOCK_FLOWER:
//    {
//        delete ui->qsa_config->widget();
//        CTConfToyFlower *config = new CTConfToyFlower();
//        ui->qsa_config->setWidget(config);
//        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
//                    "CTBlockConfig::resetConfig resetting flower block. ");
//        break;
//    }
    case CT_BLOCK_MICKEY:
    {
        delete ui->qsa_config->widget();
        CTConfToyMickey *config = new CTConfToyMickey();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting mickey block. ");
        break;
    }
    case CT_BLOCK_U:
    {
        delete ui->qsa_config->widget();
        CTConfToyU *config = new CTConfToyU();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting utoy block. ");
        break;
    }
    case CT_BLOCK_ARCH:
    {
        delete ui->qsa_config->widget();
        CTConfArch *config = new CTConfArch();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting arch block. ");
        break;
    }
    case CT_BLOCK_WALL_LEFT:
    {
        delete ui->qsa_config->widget();
        CTConfWallLeft *config = new CTConfWallLeft();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting wall_left block. ");
        break;
    }
    case CT_BLOCK_WALL_RIGHT:
    {
        delete ui->qsa_config->widget();
        CTConfWallRight *config = new CTConfWallRight();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting wall_right block. ");
        break;
    }
    case CT_BLOCK_WALL_SCREEN:
    {
        delete ui->qsa_config->widget();
        CTConfWallScreen *config = new CTConfWallScreen();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting screen block. ");
        break;
    }
    case CT_BLOCK_LRING:
    {
        delete ui->qsa_config->widget();
        CTConfToyLRing *config = new CTConfToyLRing();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting screen block. ");
        break;
    }
    case CT_BLOCK_SRING:
    {
        delete ui->qsa_config->widget();
        CTConfToySRing *config = new CTConfToySRing();
        ui->qsa_config->setWidget(config);
        Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                    "CTBlockConfig::resetConfig resetting screen block. ");
        break;
    }
    default:
        break;
    }
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Exit CTBlockConfig::resetConfig . ");
}

void CTBlockConfig::saveConfiguration()
{
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Entering CTBlockConfig::saveConfiguration ... ");
    switch (id) {
//    case CT_BLOCK_STICK:
//    {
//        CTConfToyStick *config = (CTConfToyStick *) ui->qsa_config->widget();
//        QString parameters = config->getParameters("QXmlStreamWriter");
//        if(parameters.isEmpty())
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                        "CTBlockConfig::saveConfiguration got parameters from Stick block empty! ");
//        emit finishedConfig(parameters);
//        break;
//    }
//    case CT_BLOCK_RING:
//    {
//        CTConfToyRing *config = (CTConfToyRing *) ui->qsa_config->widget();
//        QString parameters = config->getParameters("QXmlStreamWriter");
//        if(parameters.isEmpty())
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                        "CTBlockConfig::saveConfiguration got parameters from ring block empty! ");
//        emit finishedConfig(parameters);
//        break;
//    }
//    case CT_BLOCK_FLOWER:
//    {
//        CTConfToyFlower *config = (CTConfToyFlower *) ui->qsa_config->widget();
//        QString parameters = config->getParameters("QXmlStreamWriter");
//        if(parameters.isEmpty())
//            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
//                        "CTBlockConfig::saveConfiguration got parameters from flower block empty! ");
//        emit finishedConfig(parameters);
//        break;
//    }
    case CT_BLOCK_MICKEY:
    {
        CTConfToyMickey *config = (CTConfToyMickey *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from mickey block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_U:
    {
        CTConfToyU *config = (CTConfToyU *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from uToy block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_ARCH:
    {
        CTConfArch *config = (CTConfArch *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from arch block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_WALL_LEFT:
    {
        CTConfWallLeft *config = (CTConfWallLeft *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from wall_left block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_WALL_RIGHT:
    {
        CTConfWallRight *config = (CTConfWallRight *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from wall_right block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_WALL_SCREEN:
    {
        CTConfWallScreen *config = (CTConfWallScreen *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from screen block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_LRING:
    {
        CTConfToyLRing *config = (CTConfToyLRing *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from large_ring block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    case CT_BLOCK_SRING:
    {
        CTConfToySRing *config = (CTConfToySRing *) ui->qsa_config->widget();
        QString parameters = config->getParameters("QXmlStreamWriter");
        if(parameters.isEmpty())
            Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->warn(
                        "CTBlockConfig::saveConfiguration got parameters from small_ring block empty! ");
        emit finishedConfig(parameters);
        break;
    }
    default:
        break;
    }
    Log4Qt::Logger::logger(QLatin1String("CTBlockConfig"))->info(
                "Exit CTBlockConfig::saveConfiguration . ");
    close();
}
