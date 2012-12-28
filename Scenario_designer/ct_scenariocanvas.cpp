#include "ct_scenariocanvas.h"

#define BLOCK_EDGE 130

CTScenarioCanvas::CTScenarioCanvas(QWidget *parent) : QWidget(parent)
{
    // Initialize member variables
    this->dragStartPosition = QPoint();
    this->contentLayout = 0;
    this->blocks.clear();
}

// Initialize the widget.
void CTScenarioCanvas::initialize()
{
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
}

// Determine linear position index on the grid layout.
int CTScenarioCanvas::getPositionIndex(QPoint eventPos, bool dropIndex)
{
    int cellEdge = BLOCK_EDGE + 10;
    int numberOfColumns = this->width() / cellEdge;

    int columnIndex;
    if (dropIndex)
    {
        columnIndex = floor(((double) eventPos.x() / (double) cellEdge) + 0.5);
    }
    else { columnIndex = eventPos.x() / cellEdge; }

    int rowIndex = eventPos.y() / cellEdge;
    return (numberOfColumns * rowIndex) + columnIndex;
}

// Update the scenario canvas with the current sequence of blocks.
void CTScenarioCanvas::updateBlockSequence()
{
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
    while (!this->blocks.isEmpty())
    {
        CTSimpleBlock *tmp = this->blocks.takeFirst();
        delete tmp;
    }
    id_scenario.clear();
    description.clear();
    execution_day.clear();
    execution_order.clear();
}

/*Loads scenario from file*/
void CTScenarioCanvas::loadScenario()
{
    QString name = QFileDialog::getOpenFileName(this, "Load scenario", QDir::currentPath());
    if (!name.isNull())
    {
        QFile file(name);
        if (file.open(QIODevice::ReadOnly))
        {
            if (!this->blocks.isEmpty()) { resetScenario(); }

            QDomDocument doc;
            doc.setContent(&file);
            QDomElement root = doc.documentElement();
            QDomElement xml_blocks = root.namedItem("blocks").toElement();
            QDomNodeList scenario_blocks = xml_blocks.childNodes();

            for (int i = 0; i < scenario_blocks.size(); i++)
            {
                QDomElement xml_conf = scenario_blocks.at(i).toElement();
                QString block_name = xml_conf.attribute("name");
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

            file.close();
            updateBlockSequence();
        }
    }
}


void CTScenarioCanvas::loadScenario(QHash<QString, QString> scenario)
{
    /*clearing the canvas*/
    resetScenario();

    /*getting the id_scenario for editing*/
    id_scenario = scenario["id"];
    description = scenario["description"];
    execution_day = scenario["execution_day"];
    execution_order = scenario["execution_order"];

    QDomDocument doc;
    doc.setContent(scenario["xml_description"]);
    QDomElement root = doc.documentElement();
    QDomElement xml_blocks = root.namedItem("blocks").toElement();
    QDomNodeList scenario_blocks = xml_blocks.childNodes();

    for (int i = 0; i < scenario_blocks.size(); i++)
    {
        QDomElement xml_conf = scenario_blocks.at(i).toElement();
        QString block_name = xml_conf.attribute("name");
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
    updateBlockSequence();
}

// Save the scenario data into file
void CTScenarioCanvas::saveScenario()
{
    if (!this->blocks.isEmpty())
    {
        QDomDocument doc("");
        doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));
        QDomElement root = doc.createElement("scenario_data");
        doc.appendChild(root);
        QDomElement subroot = doc.createElement("blocks");
        root.appendChild(subroot);
        subroot.setAttribute("number", this->blocks.count());
        for (int i = 0; i < this->blocks.count(); i++)
        {
            subroot.appendChild(this->blocks.at(i)->getConfiguration());
        }

        QString name = QFileDialog::getSaveFileName(this, "Save scenario", QDir::currentPath());
        if (!name.isNull())
        {
            QFile file(name);
            if (file.open(QFile::WriteOnly))
            {
                QTextStream fileStream(&file);
                fileStream << doc.toString(4);
                file.close();
            }
        }
    }
}


void CTScenarioCanvas::getInfoAndSave(QString description,
                                      QString execution_day,
                                      QString execution_order)
{
    QHash<QString,QString> scenario;
    if(!this->blocks.isEmpty())
    {
        QDomDocument doc("");
        doc.appendChild(doc.createProcessingInstruction(
                            "xml","version=\"1.0\" encoding=\"UTF-8\""));
        QDomElement root = doc.createElement("scenario_data");
        doc.appendChild(root);
        QDomElement subroot = doc.createElement("blocks");
        root.appendChild(subroot);
        subroot.setAttribute("number", this->blocks.count());
        for (int i = 0; i < this->blocks.count(); i++)
        {
            subroot.appendChild(this->blocks.at(i)->getConfiguration());
        }
        if(id_scenario.isEmpty()){
            scenario["creation_date"] = QDateTime::currentDateTime().
                    toString("yyyy-MM-dd HH:mm");
        }
        if(!id_scenario.isEmpty()){
            scenario["id"] = id_scenario;
        }
        scenario["xml_description"] = doc.toString();
        scenario["last_edited"] = QDateTime::currentDateTime().
                toString("yyyy-MM-dd HH:mm");
        scenario["execution_day"] = execution_day;
        scenario["execution_order"] = execution_order;
        scenario["description"] = description;

        emit save(scenario);
    }
}

void CTScenarioCanvas::getInfoAndSave()
{
    QHash<QString,QString> scenario;
    if(!this->blocks.isEmpty())
    {
        QDomDocument doc("");
        doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));
        QDomElement root = doc.createElement("scenario_data");
        doc.appendChild(root);
        QDomElement subroot = doc.createElement("blocks");
        root.appendChild(subroot);
        subroot.setAttribute("number", this->blocks.count());
        for (int i = 0; i < this->blocks.count(); i++)
        {
            subroot.appendChild(this->blocks.at(i)->getConfiguration());
        }
        scenario["id"] = id_scenario;
        scenario["xml_description"] = doc.toString();
        scenario["last_edited"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");

        qDebug() << doc.toString();
        emit save(scenario);
    }
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
