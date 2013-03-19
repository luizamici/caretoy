#ifndef CT_WIZARDPAGE_H
#define CT_WIZARDPAGE_H

#include <QWidget>
#include <QWizardPage>

class CTWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CTWizardPage(QWidget *parent = 0);

    void registerFields(const QString &name, QWidget *widget,
                        const char *property = 0,
                        const char *changedSignal = 0);
    
};

#endif // CT_WIZARDPAGE_H
