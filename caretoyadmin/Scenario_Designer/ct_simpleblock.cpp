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
    case CT_BLOCK_LRING:
        ui->qlb_block_name->setText("Large ring");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/ring.png"));
        break;
    case CT_BLOCK_SRING:
    {
        ui->qlb_block_name->setText("Small ring");
        ui->qlb_block_image->setPixmap(QPixmap(":/images/ring.png"));
        break;
    }
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

void CTSimpleBlock::setToolTip(const QString &toolTip)
{
    ui->qlb_block_info->setToolTip(toolTip);
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

void CTSimpleBlock::setConfigurations(QString xml,QString comment)
{
    if(xml.isEmpty())
        Log4Qt::Logger::logger(QLatin1String("CTSimpleBlock"))->warn(
                    "CTSimpleBlock::setConfiguration -> setting empty configuration!");
    xml_config = xml;
    emit updateRuntime(xml_config);
    if(comment.trimmed() == "")
        setToolTip("No description for this block");
    else
        setToolTip(comment);
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

double CTSimpleBlock::getRuntime()
{
    double tot_runtime = 0.0;
    double _duration = 0.0;
    int _rep = 0;
    bool readDuration = false;
    bool readRep = false;

    QXmlStreamReader reader(xml_config);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement() && reader.name() == "duration")
            readDuration = true;
        if(reader.isEndElement() && reader.name() == "duration")
            readDuration = false;
        if(reader.isStartElement() && reader.name() == "repetitions")
            readRep = true;
        if(reader.isEndElement() && reader.name() == "repetitions")
            readRep = false;
        if(reader.isCharacters() && !reader.text().toString().trimmed().isEmpty()
                && readDuration)
            _duration = reader.text().toString().toDouble();
        if(reader.isCharacters() && !reader.text().toString().trimmed().isEmpty()
                && readRep)
            _rep = reader.text().toString().toInt();
        if(reader.isEndElement() && reader.name() == "runtime")
            tot_runtime += (_duration * _rep);
    }
    return tot_runtime;
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
        block_config.setWindowTitle(tr("[*]Block configuration: ") +
                                    ui->qlb_block_name->text());
        block_config.showParameters(id, xml_config);
//        connect(&block_config,SIGNAL(finishedConfig(QString)),
//                this, SLOT(setConfiguration(QString)));

        connect(&block_config, SIGNAL(finishedConfigs(QString,QString)),
                this, SLOT(setConfigurations(QString,QString)));
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
