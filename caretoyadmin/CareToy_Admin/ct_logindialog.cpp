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
    ui->hl_header->addSpacerItem(new QSpacerItem(
                                            20,20,QSizePolicy::MinimumExpanding,
                                            QSizePolicy::Maximum));
    ui->hl_header->addWidget(logo);
    ui->hl_header->addSpacerItem(new QSpacerItem(
                                            20,20,QSizePolicy::MinimumExpanding,
                                            QSizePolicy::Maximum));
    statusBar = new QStatusBar();
    statusBar->setMaximumHeight(20);
    statusBar->setFont(QFont("Helvetica [Cronyx]", 10));
    showMessage("Connecting to server ...");

    ui->hl_footer->addWidget(statusBar);

    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setInputMethodHints(Qt::ImhHiddenText|
                                          Qt::ImhNoPredictiveText|
                                          Qt::ImhNoAutoUppercase);
}


void CTLoginDialog::on_okButton_clicked()
{
    emit credentials(ui->usernameEdit->text(), ui->passwordEdit->text());
}

void CTLoginDialog::on_cancelButton_clicked()
{
    this->close();
}

void CTLoginDialog::showMessage(QString mss)
{
    statusBar->showMessage(mss);
}

CTLoginDialog::~CTLoginDialog()
{
    delete ui;
}
