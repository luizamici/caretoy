#include "ct_worklogs.h"
#include <QtGui>
#include <QTextEdit>
#include <QToolButton>

CTWorklogs::CTWorklogs(QWidget *parent) :
    QWidget(parent)
{
    treeOfLogs = new CTTreeOfWorkLogs();

    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(20);

    /***********************************************************/
    /*  This is where the toolbar part of this widget starts   */
    /***********************************************************/
    toolBar = new QToolBar();
    toolBar->setMinimumHeight(35);
    toolBar->setMinimumWidth(200);

    /*
     *Inserting the toolbar actions : new, save, edit
     */
    toolBar->addAction(QIcon(":images/add_button.png"),"new",this,SLOT(newLog()));
    toolBar->addAction(QIcon(":images/save_button2.png"),"save",this, SIGNAL(saveLog()));
    toolBar->addAction(QIcon(":images/edit_button.png"),"edit",this,SLOT(enableWorklogEditor()));

    comboBox = new QComboBox();
    comboBox->setMinimumWidth(280);
    comboBox->setMinimumHeight(30);
    resetComboBox();

//    toolBar->addSeparator();
//    toolBar->addWidget(comboBox);

    /******************************************************/

    layout->addWidget(toolBar,0,1,Qt::AlignRight);

    /**************************************************/
    /*This widget is for showing a list of worklogs   */
    /*written in time by the logged in user           */
    /**************************************************/
    worklogList = new QTextEdit();
    worklogList->addScrollBarWidget(this->parentWidget(),Qt::AlignRight);
    disableWorkLogEditor(worklogList);

    /***************************************************/
    /*This widget is for editing a particular worklog  */
    /*relative to the selected time&day on the comboBox*/
    /***************************************************/
    workLogEdit = new QTextEdit();
    workLogEdit->setFrameStyle(QFrame::Box);
    workLogEdit->setFrameShadow(QFrame::Sunken);
    workLogEdit->addScrollBarWidget(this->parentWidget(),Qt::AlignRight);
    disableWorkLogEditor(workLogEdit);

//    layout->addWidget(worklogList,0,0,0,1);
    layout->addWidget(treeOfLogs,0,0,0,1);
    layout->addWidget(workLogEdit,1,1);
    this->setLayout(layout);

    connect(treeOfLogs, SIGNAL(clicked(QModelIndex)), this, SLOT(showLog(QModelIndex)));
}

void CTWorklogs::showLog(QModelIndex index)
{
    workLogEdit->setText(treeOfLogs->getWorklog(index));
    disableWorkLogEditor(workLogEdit);
}

/*
 *Before the StaffModule is showed, the comboBox is initialized from the staffModuleAdmin
 *with the current logs' timestamps on the DB relative to the logged in user
 */
void CTWorklogs::initializeComboBox(QStringList timestampsList){
    resetComboBox();
    comboBox->addItems(timestampsList);
}

/*
 *Before the StaffModule is showed, the worklogList is initialized from the staffModuleAdmin
 *with the current logs' timestamps on the DB and their corrisponding log as a list
 */
void CTWorklogs::initializeWorklogList(QStringList timestamps, QStringList worklogsList){
//    worklogList->clear();
//    for(int i = 0; i < timestamps.size(); i ++){
//        worklogList->append('*' + timestamps.at(i));
//        worklogList->append(worklogsList.at(i));
//        worklogList->append("    "); //this values is inserted for displaying purposes
//    }
}

/*
 *This function is a custom 'clear' for the comboBox widget
 *It clears all the dataItems except from the first two ones
 *which contain info and space for displaying purposes
 */
void CTWorklogs::resetComboBox(){

    /*
     *The first two items are only for displaying/tip purpose
     */
    comboBox->clear();
    comboBox->insertItem(0,QIcon(":images/search_button.png"),"Search by date");
    comboBox->addItem(" ");
    /*
     *The following code makes the first two items not selectable
     */
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(comboBox->model());
    QModelIndex firstIndex = model->index(0, comboBox->modelColumn(),comboBox->rootModelIndex());
    QModelIndex secondIndex = model->index(1,comboBox->modelColumn(),comboBox->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    QStandardItem* secondItem = model->itemFromIndex(secondIndex);
    firstItem->setSelectable(false);
    secondItem->setSelectable(false);
}

/*
 *Whenever the user wants to add/edit a log,
 *this slot enables the textEditor-which is disabled by default-
 *and puts the focus on it
 */
void CTWorklogs::enableWorklogEditor(){
    /*
     *setting the editor writable
     */
    workLogEdit->setReadOnly(false);
    /*
     *putting focus on the editor
     */
    workLogEdit->setFocus();
    /*
     *changing the background color
     */
    QPalette p = workLogEdit->palette();
    p.setColor(QPalette::Base, QColor(255, 255, 255));
    workLogEdit->setPalette(p);
}


void CTWorklogs::disableWorkLogEditor(QTextEdit *editor){
    /*
     *setting the editor read-only
     */
    editor->setReadOnly(true);
    /*
     *changing the background color
     */
    QPalette p2 = editor->palette();
    p2.setColor(QPalette::Base, QColor(240, 240, 255,30));
    editor->setPalette(p2);
}

void CTWorklogs::newLog(){

    /*
     *making the editor editable and putting focus in it,
     *clear in case an old message was being read
     */
    enableWorklogEditor();
    workLogEdit->clear();
    comboBox->setCurrentIndex(0);
}

QString CTWorklogs::getLog(){
    return (workLogEdit->toHtml());
}



