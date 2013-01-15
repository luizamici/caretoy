#include "ct_blocksource.h"

CTBlockSource::CTBlockSource(QWidget *parent) : QWidget(parent)
{
    // Initialize member variables
    this->dragStartPosition = QPoint();
    p_logger = Log4Qt::Logger::logger("CTBlockSource");
}

// Initialize the widget based on the defined/supplied block names.
void CTBlockSource::initialize()
{
    p_logger->info("CTBlockSource::initialize begin...");
    // Initialize/configure visual appearance
    this->setAcceptDrops(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(10);
    layout->setSpacing(10);
    this->setLayout(layout);

    CTSimpleBlock *block_stick = new CTSimpleBlock(CT_BLOCK_STICK);
    layout->addWidget(block_stick);
    CTSimpleBlock *block_flower = new CTSimpleBlock(CT_BLOCK_FLOWER);
    layout->addWidget(block_flower);
    CTSimpleBlock *block_ring = new CTSimpleBlock(CT_BLOCK_RING);
    layout->addWidget(block_ring);
    CTSimpleBlock *block_mickey = new CTSimpleBlock(CT_BLOCK_MICKEY);
    layout->addWidget(block_mickey);
    CTSimpleBlock *block_u = new CTSimpleBlock(CT_BLOCK_U);
    layout->addWidget(block_u);
    CTSimpleBlock *block_wall_left = new CTSimpleBlock(CT_BLOCK_WALL_LEFT);
    layout->addWidget(block_wall_left);
    CTSimpleBlock *block_wall_right = new CTSimpleBlock(CT_BLOCK_WALL_RIGHT);
    layout->addWidget(block_wall_right);
    CTSimpleBlock *block_wall_screen = new CTSimpleBlock(CT_BLOCK_WALL_SCREEN);
    layout->addWidget(block_wall_screen);
    CTSimpleBlock *block_arch = new CTSimpleBlock(CT_BLOCK_ARCH);
    layout->addWidget(block_arch);

    p_logger->info("CTBlockSource::initialize end");
}

// Record initial position for a drag operation.
void CTBlockSource::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton & event->buttons()) { dragStartPosition = event->pos(); }
}

// Check for the requirements of a drag operation. If fulfilled, pack
// data of the selected block and execute the drag operation.
void CTBlockSource::mouseMoveEvent(QMouseEvent *event)
{
    // Check requirements
    if (!(Qt::LeftButton & event->buttons())) { return; }

    if (QApplication::startDragDistance() > (event->pos() - dragStartPosition).manhattanLength())
    {
        return;
    }

    // Select block
    bool blockFound = false;
    QWidget *widget = childAt(event->pos());
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
    int positionIndex = -1;

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
    drag->setHotSpot(event->pos() - block->pos());
    drag->exec(Qt::CopyAction, Qt::CopyAction);
}

// If the MIME data is of the correct format, accept the drag operation
// upon entering the widget.
void CTBlockSource::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else { event->ignore(); }
    }
    else { event->ignore(); }
}

// If the MIME data is of the correct format, accept drag operation
// position events related to the widget.
void CTBlockSource::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else { event->ignore();}
    }
    else { event->ignore(); }
}

// Ignore drop events
void CTBlockSource::dropEvent(QDropEvent *event) { event->ignore(); }
