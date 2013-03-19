#include "ct_wizardpage.h"

CTWizardPage::CTWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
}

/*!
  !\brief: CTWizardPage::registerFields
  Call protected function of QWizardPage for registering fields
  */
void CTWizardPage::registerFields(const QString &name, QWidget *widget,
                                  const char *property,
                                  const char *changedSignal)
{
    registerField(name, widget, property, changedSignal);
}
