#include "ct_logindialog.h"
#include "ui_ct_logindialog.h"

CTLoginDialog::CTLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTLoginDialog)
{

    this->setWindowIcon(QIcon(":images/logo.jpeg"));
    ui->setupUi(this);

    QLabel *logo = new QLabel();
    logo->setPixmap(QPixmap(":images/logo.png"));
    ui->horizontalLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));
    ui->horizontalLayout->addWidget(logo);
    ui->horizontalLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,QSizePolicy::Maximum));

    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    ui->horizontalLayout_3->addWidget(statusBar);


    ui->usernameEdit->setPlaceholderText("Username");
    ui->passwordEdit->setPlaceholderText("Password");
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void CTLoginDialog::okPressed(){
    emit credentials(ui->usernameEdit->text(), ui->passwordEdit->text());
}

CTLoginDialog::~CTLoginDialog()
{
    delete ui;
}
