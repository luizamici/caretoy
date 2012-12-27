#ifndef CT_CUSTOMWIDGETS_H
#define CT_CUSTOMWIDGETS_H

#include <QtGui>
#include <QtXml>

/* ########################################################################## */

class CTSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit CTSpinBox(QWidget *parent = 0);
    void setup(QString suffix,
               int min,
               int max,
               int step,
               int value);

public slots:
    void setMinimumValue(int value);
    void setMaximumValue(int value);
};

/* ########################################################################## */

class CTDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    explicit CTDoubleSpinBox(QWidget *parent = 0);
    void setup(QString suffix,
               int decimals,
               double min,
               double max,
               double step,
               double value);

public slots:
    void setMinimumValue(double value);
    void setMaximumValue(double value);
};

/* ########################################################################## */

class CTBigLight : public QWidget
{
    Q_OBJECT

public:
    explicit CTBigLight(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    CTDoubleSpinBox *activation;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    CTSpinBox *intensity_min;
    CTSpinBox *intensity_max;
    CTSpinBox *rings_min;
    CTSpinBox *rings_max;
    QComboBox *color;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

/* ########################################################################## */

class CTButton : public QWidget
{
    Q_OBJECT

public:
    explicit CTButton(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    CTDoubleSpinBox *activation;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    CTSpinBox *intensity_min;
    CTSpinBox *intensity_max;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

/* ########################################################################## */

class CTLight : public QWidget
{
    Q_OBJECT

public:
    explicit CTLight(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    CTDoubleSpinBox *activation;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    CTSpinBox *intensity_min;
    CTSpinBox *intensity_max;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

/* ########################################################################## */

class CTConstLight : public QWidget
{
    Q_OBJECT

public:
    explicit CTConstLight(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    CTDoubleSpinBox *activation;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

/* ########################################################################## */

class CTSpeaker : public QWidget
{
    Q_OBJECT

public:
    explicit CTSpeaker(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    QCheckBox *left_front;
    QCheckBox *left_rear;
    QCheckBox *right_front;
    QCheckBox *right_rear;
    CTDoubleSpinBox *activation;
    CTSpinBox *volume_min;
    CTSpinBox *volume_max;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    QComboBox *sound;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

/* ########################################################################## */

class CTScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CTScreen(int id, bool action, QWidget *parent = 0);
    int id;
    QString name;
    QCheckBox *state;
    CTDoubleSpinBox *activation;
    CTDoubleSpinBox *duration_min;
    CTDoubleSpinBox *duration_max;
    QComboBox *video;
    void setParameters(QDomElement root);
    QDomElement getParameters();
};

#endif // CT_CUSTOMWIDGETS_H
