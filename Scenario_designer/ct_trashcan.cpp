#include "ct_trashcan.h"

CTTrashCan::CTTrashCan(QWidget *parent) : QFrame(parent)
{
    // Initialize/configure visual appearance
    this->setAcceptDrops(true);
    this->setFrameStyle(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Sunken);

    this->setLayout(new QVBoxLayout());
    this->layout()->setMargin(10);
    this->layout()->setSpacing(10);

    QLabel *qlbTrash = new QLabel();
    qlbTrash->setPixmap(QPixmap(":/images/ct_trash.png"));
    qlbTrash->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->layout()->addWidget(qlbTrash);
}

// If the MIME data is of the correct format, accept the drag operation
// upon entering the widget.
void CTTrashCan::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        event->acceptProposedAction();
    }
    else { event->ignore(); }
}

// If the MIME data is of the correct format, accept drag operation
// position events related to the widget.
void CTTrashCan::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        event->acceptProposedAction();
    }
    else { event->ignore(); }
}

// If the MIME data is of the correct format, accept the drop of the
// dragged block and do nothing.
void CTTrashCan::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-block"))
    {
        event->acceptProposedAction();
    }
    else { event->ignore(); }
}
