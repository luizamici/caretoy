#include "ct_simpleblock.h"
#include "ui_ct_simpleblock.h"

CTSimpleBlock::CTSimpleBlock(int id, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CTSimpleBlock)
{
    ui->setupUi(this);

    this->id = id;
    configurable = false;

    switch (id) {
    case CT_BLOCK_STICK:
        ui->qlb_block_name->setText("Stick");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/stick.png"));
        break;
    case CT_BLOCK_RING:
        ui->qlb_block_name->setText("Ring");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/ring.png"));
        break;
    case CT_BLOCK_FLOWER:
        ui->qlb_block_name->setText("Flower");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/flower.png"));
        break;
    case CT_BLOCK_MICKEY:
        ui->qlb_block_name->setText("Mickey");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/mickey.png"));
        break;
    case CT_BLOCK_U:
        ui->qlb_block_name->setText("UToy");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/uToy.png"));
        break;
    case CT_BLOCK_ARCH:
        ui->qlb_block_name->setText("Arch");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/arch2.png"));
        break;
    case CT_BLOCK_WALL_LEFT:
        ui->qlb_block_name->setText("Left wall");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/wall.png"));
        break;
    case CT_BLOCK_WALL_RIGHT:
        ui->qlb_block_name->setText("Right wall");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/wall.png"));
        break;
    case CT_BLOCK_WALL_SCREEN:
        ui->qlb_block_name->setText("Screen wall");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/screen_wall.png"));
        break;
    default:
        break;
    }

    ui->qlb_block_info->setPixmap(QPixmap(":/images/ct_info.png"));
    ui->qlb_block_info->setToolTip("Block has not been configured yet.");
}

CTSimpleBlock::~CTSimpleBlock()
{
    delete ui;
}

QString CTSimpleBlock::getName() { return ui->qlb_block_name->text(); }

QPixmap CTSimpleBlock::getImage()
{
    QPixmap pixmap(*ui->qlb_block_image->pixmap());
    return pixmap;
}

void CTSimpleBlock::setConfiguration(QString xml)
{
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Entering CTSimpleBlock::setConfiguration ... ");
    if(xml.isEmpty())
        Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->warn(
                    "CTSimpleBlock::setConfiguration -> setting empty configuration!");
    xml_config = xml;
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Exit CTSimpleBlock::setConfiguration . ");
}

QString CTSimpleBlock::getConfiguration(QString str)
{
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Entering CTSimpleBlock::getConfiguration ... ");
    str = "Using Sax Parser";
    if(xml_config.isEmpty())
        Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->warn(
                    "CTSimpleBlock::getConfiguration -> configuration empty!");
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Exit CTSimpleBlock::getConfiguration . ");
    return xml_config;
}


void CTSimpleBlock::enableConfig(bool value) { configurable = value; }


void CTSimpleBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Entering CTSimpleBlock::mouseDoubleClickEvent ... ");
    if ((Qt::LeftButton & event->buttons()) && configurable)
    {
        event->accept();
        CTBlockConfig block_config;
        block_config.setWindowTitle("Block configuration: " +
                                    ui->qlb_block_name->text());
        block_config.showParameters(id, xml_config);
        connect(&block_config,SIGNAL(finishedConfig(QString)),
                this, SLOT(setConfiguration(QString)));
        block_config.exec();
    }
    else {
        Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->warn(
                    "CTSimpleBlock::mouseDoubleClickEvent -> RightButton clicked!");
        event->ignore();
    }
    Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->info(
                "Exit CTSimpleBlock::mouseDoubleClickEvent . ");
}
