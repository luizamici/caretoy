#include "ct_staffmodule.h"
#include "Patients_Admin/ct_patientmodule.h"


CTStaffModule::CTStaffModule(QWidget *parent) :
    QWidget(parent)
{
//    this->setStyleSheet("background-image: url(:/images/logo.png);"
//                        "background-repeat:no-repeat;"
//                        "background-position:right bottom;"
//                        "margin-right:200px;");

    this->setWindowTitle("Staff Module");
    this->setWindowIcon(QIcon(":images/logo.jpeg"));
    this->setMinimumSize(1200,800);
    // Center the widget on the screen
    QRect desktop = QApplication::desktop()->availableGeometry();
    this->move(desktop.width()/2 - this->width()/2, desktop.height()/2 - this->height()/2);

    QFont serifFont("Serif", 12, QFont::Normal);
    searchPatient = new CTSearchPatient();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    this->setLayout(layout);

    //Big main Layout
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setMargin(0);
    verticalLayout->setSpacing(15);

    /*
     *First section containing Name, Surname and last login of the user
     */
    QHBoxLayout *subLayout1 = new QHBoxLayout();
    subLayout1->setMargin(10);
    subLayout1->setSpacing(30);
    label_1 = new QLabel();
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    label_1->setFont(font);

    subLayout1->addWidget(label_1, 0 , Qt::AlignRight);
    label_2 = new QLabel();

    subLayout1->addWidget(label_2);
    subLayout1->addStretch();

    verticalLayout->addLayout(subLayout1);

    /*
     *Line separating the user's data header from the
     *patients' list management
     */
    QFrame *horizontalLine = new QFrame();
    horizontalLine->setFrameShape(QFrame::HLine);
    horizontalLine->setFrameShadow(QFrame::Raised);
    verticalLayout->addWidget(horizontalLine);

    /*
     *Second section containing the list of patients,
     *and three pushButtons for adding, editing or deleting
     *an entry from the list
     */
    subLayout2 = new QGridLayout();
    subLayout2->setMargin(10);
    subLayout2->setSpacing(5);

    QHBoxLayout *horizLayout = new QHBoxLayout();

    QLabel *label_3 = new QLabel(tr("Patients"));
    label_3->setFont(serifFont);

    horizLayout->addWidget(label_3,1);
    horizLayout->addWidget(searchPatient);
    subLayout2->addLayout(horizLayout,0,0);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add");
    editButton = new QPushButton("Edit");
    editButton->setEnabled(false);
    deleteButton = new QPushButton("Delete");
    deleteButton->setEnabled(false);
    buttonLayout->addWidget(addButton, 1, Qt::AlignRight);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addSpacing(1);
    subLayout2->addLayout(buttonLayout,2,0);

    verticalLayout->addLayout(subLayout2);

    /*
     *Horizontal line which separates the second section(patient's management)
     *from the last one(workLog management)
     */
    QFrame *horizontalLine2 = new QFrame();
    horizontalLine2->setFrameShape(QFrame::HLine);
    horizontalLine2->setFrameShadow(QFrame::Raised);
    verticalLayout->addWidget(horizontalLine2);

    //Third section containing the worklog part of the staff module,
    //where the logs of the application's user(staff)
    //can be viewd and edited if necessary.
    subLayout3 = new QVBoxLayout();
    subLayout3->setSpacing(5);
    verticalLayout->addLayout(subLayout3);

    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    verticalLayout->addWidget(statusBar);

    layout->addLayout(verticalLayout);

    confirmationMessage = new QMessageBox(
                QMessageBox::Warning,"Staff Module",
                "Are u sure you wish to delete the selected patient?");
    confirmationMessage->setStandardButtons(
                QMessageBox::Ok | QMessageBox::Cancel);
    /*
     *When the Delete button is clicked the staffmoduleAdmin is notified by
     *the deleteSelectedPatient() signal
     */
    connect(deleteButton,SIGNAL(clicked()),this, SLOT(confirmDeletionOfpatient()));
}

/*
 *This needs to be examined a bit better since the removerow() doesn't what needed
 */
void CTStaffModule::confirmDeletionOfpatient(){
    int ret = confirmationMessage->exec();
    switch(ret){
    case QMessageBox::Ok :
        emit deleteSelectedPatient();
        break;
    case QMessageBox::Cancel :
        confirmationMessage->close();
        break;
    default:
        break;
    }
}

void CTStaffModule::showOkMessage(QString message){
    QPalette palette;
    palette.setColor( QPalette::WindowText, "green" );
    statusBar->setPalette( palette );
    statusBar->showMessage(message,5000);
}

void CTStaffModule::showFailureMessage(QString message)
{
    QPalette palette;
    palette.setColor( QPalette::WindowText, "red" );
    statusBar->setPalette( palette );
    statusBar->showMessage(message,5000);
}

void CTStaffModule::showMessage(QString message)
{
    QPalette palette;
    palette.setColor( QPalette::WindowText, "black" );
    statusBar->setPalette( palette );
    statusBar->showMessage(message,6000);
}


