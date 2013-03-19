#ifndef CT_WIZARDLAYOUT_H
#define CT_WIZARDLAYOUT_H

#include <QWidget>
#include <QCompleter>

namespace Ui {
class CTWizardLayout;
}

class CTWizardLayout : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTWizardLayout(QWidget *parent = 0);
    ~CTWizardLayout();

    void setPatient(QHash<QString,QString> patient);
    QHash<QString,QString> getPatient();

    void validReferenceEditing(bool b);
    void setCompleter(QStringList id_list);
    void clear();
    
private:
    Ui::CTWizardLayout *ui;
};

#endif // CT_WIZARDLAYOUT_H
