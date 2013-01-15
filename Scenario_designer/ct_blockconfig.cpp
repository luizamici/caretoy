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

    p_logger = Log4Qt::Logger::logger("CTBlockConfig");
}

CTBlockConfig::~CTBlockConfig()
{
    delete ui;
}


void CTBlockConfig::showParameters(int id, QString xml)
{
    p_logger->info("CTBlockConfig::showParameters begin...");
    this->id = id;

    switch (id) {
    case CT_BLOCK_FLOWER:
    {
        CTConfToyFlower *config = new CTConfToyFlower();
        ui->qsa_config->setWidget(config);
        if (!xml.isEmpty()) {
            if(!config->setParameters(xml))
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Flower' not set!");
                qDebug() << "Parameters not set!";
            }
        }
        break;
    }
    case CT_BLOCK_MICKEY:
    {
        CTConfToyMickey *config = new CTConfToyMickey();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Mickey' not set!");
                qDebug() << "Parameters not set!";
            }
        }
        break;
    }
    case CT_BLOCK_RING:
    {
        CTConfToyRing *config = new CTConfToyRing();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Ring' not set!");
                qDebug() << "Parameters not set!";
            }
        }
        break;
    }
    case CT_BLOCK_STICK:
    {
        CTConfToyStick *config = new CTConfToyStick();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Stick' not set!");
                qDebug() << "Parameters not set!";
            }
        }
        break;
    }
    case CT_BLOCK_U:
    {
        CTConfToyU *config = new CTConfToyU();
        ui->qsa_config->setWidget(config);
        if(!xml.isEmpty())
        {
            if(!config->setParameters(xml))
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'UToy' not set!");
                qDebug() << "Parameters not set!";
            }
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
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Arch' not set!");
                qDebug() << "Parameters not set!";
            }
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
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Screen' not set!");
                qDebug() << "Parameters not set!";
            }
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
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Wall left' not set!");
                qDebug() << "Parameters not set!";
            }
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
            {
                p_logger->warn("CTBlockconfig::showParameter ;"
                               "parameters for block 'Wall right' not set!");
                qDebug() << "Parameters not set!";
            }
        }
        break;
    }
    default:
        break;
    }
    p_logger->info("CTBlockConfig::showParameters end");
}

void CTBlockConfig::resetConfig()
{
    p_logger->info("CTBlockConfig::resetConfig begin...");
    switch (id) {
    case CT_BLOCK_STICK:
    {
        delete ui->qsa_config->widget();
        CTConfToyStick *config = new CTConfToyStick();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_RING:
    {
        delete ui->qsa_config->widget();
        CTConfToyRing *config = new CTConfToyRing();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_FLOWER:
    {
        delete ui->qsa_config->widget();
        CTConfToyFlower *config = new CTConfToyFlower();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_MICKEY:
    {
        delete ui->qsa_config->widget();
        CTConfToyMickey *config = new CTConfToyMickey();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_U:
    {
        delete ui->qsa_config->widget();
        CTConfToyU *config = new CTConfToyU();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_ARCH:
    {
        delete ui->qsa_config->widget();
        CTConfArch *config = new CTConfArch();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_WALL_LEFT:
    {
        delete ui->qsa_config->widget();
        CTConfWallLeft *config = new CTConfWallLeft();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_WALL_RIGHT:
    {
        delete ui->qsa_config->widget();
        CTConfWallRight *config = new CTConfWallRight();
        ui->qsa_config->setWidget(config);
        break;
    }
    case CT_BLOCK_WALL_SCREEN:
    {
        delete ui->qsa_config->widget();
        CTConfWallScreen *config = new CTConfWallScreen();
        ui->qsa_config->setWidget(config);
        break;
    }
    default:
        break;
    }
    p_logger->info("CTBlockConfig::resetConfig end");
}

void CTBlockConfig::saveConfiguration()
{
    p_logger->info("CTBlockConfig::saveConfiguration begin...");
    switch (id) {
    case CT_BLOCK_STICK:
    {
        CTConfToyStick *config = (CTConfToyStick *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_RING:
    {
        CTConfToyRing *config = (CTConfToyRing *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_FLOWER:
    {
        CTConfToyFlower *config = (CTConfToyFlower *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_MICKEY:
    {
        CTConfToyMickey *config = (CTConfToyMickey *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_U:
    {
        CTConfToyU *config = (CTConfToyU *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_ARCH:
    {
        CTConfArch *config = (CTConfArch *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_WALL_LEFT:
    {
        CTConfWallLeft *config = (CTConfWallLeft *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_WALL_RIGHT:
    {
        CTConfWallRight *config = (CTConfWallRight *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    case CT_BLOCK_WALL_SCREEN:
    {
        CTConfWallScreen *config = (CTConfWallScreen *) ui->qsa_config->widget();
        emit finishedConfig(config->getParameters("QXmlStreamWriter"));
        break;
    }
    default:
        break;
    }
    p_logger->info("CTBlockConfig::saveConfiguration end");
    close();
}
