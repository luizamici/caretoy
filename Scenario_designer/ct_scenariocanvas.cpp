#include "ct_scenariocanvas.h"

#define BLOCK_EDGE 130

CTScenarioCanvas::CTScenarioCanvas(QWidget *parent) : QWidget(parent)
{
    p_logger = Log4Qt::Logger::logger("CTScenarioCanvas");
    // Initialize member variables
    this->dragStartPosition = QPoint();
    this->contentLayout = 0;
    this->blocks.clear();
}

// Initialize the widget.
void CTScenarioCanvas::initialize()
{
    p_logger->info( "CTScenarioCanvas::initialize() begin...");
    // Initialize/configure visual appearance
    this->setAcceptDrops(true);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(10);
    layout->setSpacing(0);
    this->setLayout(layout);

    QVBoxLayout *helpLayout = new QVBoxLayout();
    helpLayout->setMargin(0);
    helpLayout->setSpacing(0);
    layout->addLayout(helpLayout);
    layout->addStretch();

    this->contentLayout = new QGridLayout();
    this->contentLayout->setMargin(0);
    this->contentLayout->setSpacing(10);
    helpLayout->addLayout(this->contentLayout);
    helpLayout->addStretch();
    p_logger->info("CTScenarioCanvas::initialize() end");
}

// Determine linear position index on the grid layout.
int CTScenarioCanvas::getPositionIndex(QPoint eventPos, bool dropIndex)
{

    p_logger->info( "CTScenarioCanvas::getPositionIndex() begin...");
    int cellEdge = BLOCK_EDGE + 10;
    int numberOfColumns = this->width() / cellEdge;

    int columnIndex;
    if (dropIndex)
    {
        columnIndex = floor(((double) eventPos.x() / (double) cellEdge) + 0.5);
    }
    else { columnIndex = eventPos.x() / cellEdge; }

    int rowIndex = eventPos.y() / cellEdge;
    p_logger->info( "CTScenarioCanvas::getPositionIndex() end");
    return (numberOfColumns * rowIndex) + columnIndex;
}

// Update the scenario canvas with the current sequence of blocks.
void CTScenarioCanvas::updateBlockSequence()
{
    p_logger->info( "CTScenarioCanvas::updateBlockSequence() begin...");
    int cellEdge = BLOCK_EDGE + 10;
    int numberOfColumns = this->width() / cellEdge;
    for (int i = 0; i < this->blocks.count(); i++)
    {
        this->contentLayout->addWidget(this->blocks.at(i), i/numberOfColumns, i%numberOfColumns);
    }
    qDebug() << "UPDATE: Number of blocks: " << this->blocks.count();
    for (int i = 0; i < this->blocks.count(); i++)
    {
        qDebug() << i << ": " << this->blocks.at(i)->getName();
    }
    p_logger->info( "CTScenarioCanvas::updateBlockSequence() end");
}

// Record initial position for a drag operation.
void CTScenarioCanvas::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton & event->buttons()) { this->dragStartPosition = event->pos(); }
}

// Check for the requirements of a drag operation. If fulfilled, pack
// data of the selected block and execute the drag operation.
void CTScenarioCanvas::mouseMoveEvent(QMouseEvent *event)
{
    // Check requirements
    if (!(Qt::LeftButton & event->buttons())) { return; }

    if (QApplication::startDragDistance() > (event->pos() - this->dragStartPosition).manhattanLength())
    {
        return;
    }

    // Select block
    bool blockFound = false;
    QWidget *widget = childAt(this->dragStartPosition);
    while (!blockFound)
    {
        if (0 != widget)
        {
            if (QString("CTSimpleBlock") != QString(widget->metaObject()->className()))
            {
                widget = widget->parentWidget();
            }
            else { blockFound = true; }
        }
        else { return; }
    }
    CTSimpleBlock *block = static_cast<CTSimpleBlock *> (widget);
    QString name = block->getName();
    QPixmap image = block->getImage();
    QString config;
    QTextStream stream(&config);
    block->getConfiguration().save(stream, 4);
    int positionIndex = this->getPositionIndex(this->dragStartPosition, false);

    // Pack block related data
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << name << image << config << positionIndex;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/x-block", itemData);

    // Setup and execute the drag operation
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap::grabWidget(block));
    // Offset the pixmap set in the previous step
    drag->setHotSpot(this->dragStartPosition - block->pos());

    if (Qt::CopyAction ==
            drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction))
    {
        // Determine whether the copy action resulted from a
        // local copy attempt
        bool local = false;
        bool done = false;
        QWidget *widget = drag->target();
        while (!local && !done)
        {
            if (0 != widget)
            {
                if (QString("CTScenarioCanvas") != QString(widget->metaObject()->className()))
                {
                    widget = widget->parentWidget();
                }
                else { local = true; }
            }
            else { done = true; }
        }
        if (!local)
        {
            CTSimpleBlock *tmp = this->blocks.takeAt(positionIndex);
            delete tmp;
        }
    }
}

// If the MIME data is of the correct format, accept the drag operation
// upon entering the widget.
void CTScenarioCanvas::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else { event->acceptProposedAction(); }
    }
    else { event->ignore(); }
}

// If the MIME data is of the correct format, accept drag operation
// position events related to the widget.
void CTScenarioCanvas::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else { event->acceptProposedAction(); }
    }
    else { event->ignore(); }
}

// If the MIME data is of the correct format, accept the drop of the
// dragged block. Unpack the block data, insert the block into the
// sequence and update the visual representation.
void CTScenarioCanvas::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        // Unpack the block data
        QByteArray itemData = event->mimeData()->data("application/x-block");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString name;
        QPixmap image;
        QString config;
        int oldPositionIndex;
        dataStream >> name >> image >> config >> oldPositionIndex;

        CTSimpleBlock *block;
        if ("Stick" == name) { block = new CTSimpleBlock(CT_BLOCK_STICK, this); }
        else if ("Flower" == name) { block = new CTSimpleBlock(CT_BLOCK_FLOWER, this); }
        else if ("Ring" == name) { block = new CTSimpleBlock(CT_BLOCK_RING, this); }
        else if ("Mickey" == name) { block = new CTSimpleBlock(CT_BLOCK_MICKEY, this); }
        else if ("UToy" == name) { block = new CTSimpleBlock(CT_BLOCK_U, this); }
        else if ("Left wall" == name) { block = new CTSimpleBlock(CT_BLOCK_WALL_LEFT, this); }
        else if ("Right wall" == name) { block = new CTSimpleBlock(CT_BLOCK_WALL_RIGHT, this); }
        else if ("Screen wall" == name) { block = new CTSimpleBlock(CT_BLOCK_WALL_SCREEN, this); }
        else if ("Arch" == name) { block = new CTSimpleBlock(CT_BLOCK_ARCH, this); }

        QDomDocument doc("");
        doc.setContent(config);
        //block->initialize(doc.firstChild().toElement());
        block->setConfiguration(doc.firstChild().toElement());
        block->enableConfig(true);

        // Insert the block into the sequence
        int newPositionIndex = this->getPositionIndex(event->pos(), true);
        if (newPositionIndex > this->blocks.count())
        {
            newPositionIndex = this->blocks.count();
        }

        if (-1 != oldPositionIndex) // -1: new block
        {
            // Copy if key was pressed
            if (Qt::ControlModifier == (Qt::ControlModifier & event->keyboardModifiers()))
            {
                this->blocks.insert(newPositionIndex, block);
            }
            else
            {
                if (newPositionIndex != oldPositionIndex) // not same index
                {
                    if (newPositionIndex > oldPositionIndex) // higher index
                    {
                        //qDebug() << "Higher index";
                        this->blocks.insert(newPositionIndex, block);
                        // Clean up
                        CTSimpleBlock *tmp = this->blocks.takeAt(oldPositionIndex);
                        delete tmp;
                    }
                    else if (newPositionIndex < oldPositionIndex) // lower index
                    {
                        //qDebug() << "Lower index";
                        this->blocks.insert(newPositionIndex, block);
                        // Clean up
                        CTSimpleBlock *tmp = this->blocks.takeAt(oldPositionIndex + 1);
                        delete tmp;
                    }
                }
            }
        }
        else { this->blocks.insert(newPositionIndex, block); }
        this->updateBlockSequence();

        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else { event->acceptProposedAction(); }
    }
    else { event->ignore(); }
}

// Clear the scenario sequence.
void CTScenarioCanvas::resetScenario()
{
    p_logger->info( "CTScenarioCanvas::resetScenario() begin...");
    while (!this->blocks.isEmpty())
    {
        CTSimpleBlock *tmp = this->blocks.takeFirst();
        delete tmp;
    }
    id_scenario.clear();
    description.clear();
    creation_date.clear();
    execution_day.clear();
    execution_order.clear();
    p_logger->info( "CTScenarioCanvas::resetScenario() end");
}


/*Load selected scenario*/
void CTScenarioCanvas::loadScenario(QHash<QString, QString> scenario)
{
    p_logger->info( "CTScenarioCanvas::loadScenario() begin...");
    /*clearing the canvas*/
    resetScenario();

    /*getting the id_scenario for editing*/
    id_scenario = scenario["id"];
    description = scenario["description"];
    creation_date = scenario["creation_date"];
    execution_day = scenario["execution_day"];
    execution_order = scenario["execution_order"];


    QXmlStreamReader reader(scenario["xml_description"]);
    QString xml_conf;
    QXmlStreamWriter stream(&xml_conf);
    stream.setAutoFormatting(true);

    QString block_name;
    while(!reader.atEnd())
    {
        reader.readNext();
        /*Skip start document element*/
        if(reader.tokenType() == QXmlStreamReader::StartDocument)
            continue;
        /*Skip scenario_data and blocks element*/
        if(reader.name() == "scenario_data" || reader.name() == "blocks")
            continue;
        /*When a new block is found*/
        if(reader.isStartElement() && reader.name() == "block")
        {
            xml_conf.clear();
            block_name = reader.attributes().value("name").toString();
        }
        stream.writeCurrentToken(reader);

        /*When a block is ending*/
        if(reader.isEndElement() && reader.name() == "block")
        {
            if ("stick" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_STICK);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("flower" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_FLOWER);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("ring" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_RING);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("mickey" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_MICKEY);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("utoy" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_U);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("wall_left" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_WALL_LEFT);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("wall_right" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_WALL_RIGHT);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("wall_screen" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_WALL_SCREEN);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
            else if ("arch" == block_name)
            {
                CTSimpleBlock *new_block = new CTSimpleBlock(CT_BLOCK_ARCH);
                new_block->enableConfig(true);
                new_block->setConfiguration(xml_conf);
                blocks.append(new_block);
            }
        }
    }
    updateBlockSequence();
    p_logger->info( "CTScenarioCanvas::loadScenario() end");
}



void CTScenarioCanvas::saveScenario(QString description,
                                      QString execution_day,
                                      QString execution_order)
{
    p_logger->info( "CTScenarioCanvas::saveScenario() begin...");
    QHash<QString,QString> scenario;
    if(!this->blocks.isEmpty())
    {
        QString xml_scenario;
        QXmlStreamWriter stream(&xml_scenario);
        stream.setAutoFormatting(true);
        stream.setAutoFormattingIndent(2);
        stream.writeStartDocument();
        stream.writeStartElement("scenario_data");
        stream.writeStartElement("blocks");
        stream.writeAttribute("number", QString::number(this->blocks.count()));

        for (int i = 0; i < this->blocks.count(); i++)
        {
            QString xml = this->blocks.at(i)->getConfiguration("Using sax parser");
            if(xml.isEmpty())
                p_logger->warn("CTScenarioCanvas::saveScenario() "
                              " Found configuration empty for block :"
                              + this->blocks.at(i)->getName());
            QXmlStreamReader reader(xml);
            while(!reader.atEnd())
            {
                reader.readNext();
                if(reader.tokenType() != QXmlStreamReader::StartDocument)
                    stream.writeCurrentToken(reader);
                if(reader.isEndElement() && reader.name() == "block")
                {
                    break;
                }
            }
        }
        stream.writeEndElement();
        stream.writeEndElement();
        stream.writeEndDocument();

        if(isNewScenario()){
            scenario["creation_date"] = QDateTime::currentDateTime().
                    toString("yyyy-MM-dd HH:mm");
        }
        else
            scenario["creation_date"] = creation_date;

        if(!isNewScenario()){
            scenario["id"] = id_scenario;
        }
        scenario["xml_description"] = xml_scenario;
        scenario["last_edited"] = QDateTime::currentDateTime().
                toString("yyyy-MM-dd HH:mm");
        scenario["execution_day"] = execution_day;
        scenario["execution_order"] = execution_order;
        scenario["description"] = description;
        emit save(scenario);
    }
    p_logger->info( "CTScenarioCanvas::saveScenario() end");
}

bool CTScenarioCanvas::isNewScenario()
{
    if(id_scenario.isEmpty())
        return true;
    else
        return false;
}

QString CTScenarioCanvas::getDescription()
{
    return this->description;
}

QString CTScenarioCanvas::getExecutionDay()
{
    return this->execution_day;
}

QString CTScenarioCanvas::getExecutionOrder()
{
    return this->execution_order;
}
