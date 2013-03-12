#include "ct_customwidgets.h"

CTSpinBox::CTSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
}

void CTSpinBox::setup(QString suffix,
                      int min,
                      int max,
                      int step,
                      int value)
{
    setSuffix(suffix);
    setMinimum(min);
    setMaximum(max);
    setSingleStep(step);
    setValue(value);
}

void CTSpinBox::setMinimumValue(int value) { setMinimum(value); }
void CTSpinBox::setMaximumValue(int value) { setMaximum(value); }

/* ########################################################################## */

CTDoubleSpinBox::CTDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
}

void CTDoubleSpinBox::setup(QString suffix,
                            int decimals,
                            double min,
                            double max,
                            double step,
                            double value)
{
    setSuffix(suffix);
    setDecimals(decimals);
    setMinimum(min);
    setMaximum(max);
    setSingleStep(step);
    setValue(value);
}

void CTDoubleSpinBox::setMinimumValue(double value) { setMinimum(value); }
void CTDoubleSpinBox::setMaximumValue(double value) { setMaximum(value); }

/*Added auxiliary methods for standardizing the usage of the point in doubles*/
QString CTDoubleSpinBox::textFromValue(double value) const
{
    QString text = QDoubleSpinBox::textFromValue(value);
    return text.replace(QLocale().decimalPoint(), QLatin1Char('.'));
}

/*
 *Even though a point might be used in the text version of a double,
 *it will be displayed according to the local settings
 */
double CTDoubleSpinBox::valueFromText(const QString& text) const
{
    return QString(text).replace(QLatin1Char('.'),QLocale().decimalPoint()).
            toDouble();
}

/* ########################################################################## */

CTBigLight::CTBigLight(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "light_cluster"; // TODO: change this to biglight
    isAction = action;

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Big light #" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the duration interval ********************/
    if(!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;


        connect(state, SIGNAL(toggled(bool)),
                duration_min, SLOT(setEnabled(bool)));
        connect(state, SIGNAL(toggled(bool)),
                duration_max, SLOT(setEnabled(bool)));

        connect(duration_min, SIGNAL(valueChanged(double)),
                duration_max, SLOT(setMinimumValue(double)));
        connect(duration_max, SIGNAL(valueChanged(double)),
                duration_min, SLOT(setMaximumValue(double)));
     }
    else{
        duration_min = NULL;
        duration_max = NULL;
    }
    /**************************************************************************/

    /* Add elements for control over the intensity interval *******************/
    layout->addWidget(new QLabel("Intensity interval"), row_index, 1);
    intensity_min = new CTSpinBox();
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_min->setEnabled(false);
    layout->addWidget(intensity_min, row_index, 2);
    intensity_max = new CTSpinBox();
    intensity_max->setup(" %", 0, 100, 1, 100);
    intensity_max->setEnabled(false);
    layout->addWidget(intensity_max, row_index, 3);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            intensity_min, SLOT(setEnabled(bool)));
    connect(state, SIGNAL(toggled(bool)),
            intensity_max, SLOT(setEnabled(bool)));

    connect(intensity_min, SIGNAL(valueChanged(int)),
            intensity_max, SLOT(setMinimumValue(int)));
    connect(intensity_max, SIGNAL(valueChanged(int)),
            intensity_min, SLOT(setMaximumValue(int)));
    /**************************************************************************/

    /* Add elements for control over the active rings *************************/
    layout->addWidget(new QLabel("Active rings"), row_index, 1);
    rings_min = new CTSpinBox();
    rings_min->setup(QString(), 1, 1, 1, 1);
    rings_min->setEnabled(false);
    layout->addWidget(rings_min, row_index, 2);
    rings_max = new CTSpinBox();
    rings_max->setup(QString(), 1, 3, 1, 1);
    rings_max->setEnabled(false);
    layout->addWidget(rings_max, row_index, 3);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            rings_min, SLOT(setEnabled(bool)));
    connect(state, SIGNAL(toggled(bool)),
            rings_max, SLOT(setEnabled(bool)));

    connect(rings_min, SIGNAL(valueChanged(int)),
            rings_max, SLOT(setMinimumValue(int)));
    connect(rings_max, SIGNAL(valueChanged(int)),
            rings_min, SLOT(setMaximumValue(int)));
    /**************************************************************************/

    /* Add elements for control over the light color **************************/
    layout->addWidget(new QLabel("Light color"), row_index, 1);
    color = new QComboBox();
    color->addItems(QStringList() << "Red" << "Green" << "Blue");
    color->setEnabled(false);
    layout->addWidget(color, row_index, 2, 1, 2);

    connect(state, SIGNAL(toggled(bool)),
            color, SLOT(setEnabled(bool)));
    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}

void CTBigLight::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}

void CTBigLight::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_max->setup(" %", 0, 100, 1, 100);
    rings_min->setup(QString(), 1, 1, 1, 1);
    rings_max->setup(QString(), 1, 3, 1, 1);
    color->setCurrentIndex(color->findText("Red"));
}

void CTBigLight::setParameters(bool b, QHash<QString,QVariant> attr)
{
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        intensity_max->setMinimum(attr["val_intensity_min"].toInt());
        intensity_min->setValue(attr["val_intensity_min"].toInt());
        intensity_max->setValue(attr["val_intensity_max"].toInt());

        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
        rings_max->setMinimum(attr["val_rings_min"].toInt());
        rings_min->setValue(attr["val_rings_min"].toInt());
        rings_max->setValue(attr["val_rings_max"].toInt());

        color->setCurrentIndex(color->findText(attr["val_color"].toString()));
    }
}

void CTBigLight::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    stream.writeStartElement("intensity");
    stream.writeTextElement("from",intensity_min->cleanText());
    stream.writeTextElement("to",intensity_max->cleanText());
    stream.writeEndElement(); // end intensity

    stream.writeStartElement("area");
    stream.writeTextElement("from" , rings_min->cleanText());
    stream.writeTextElement("to", rings_max->cleanText());
    stream.writeEndElement(); //end area

    stream.writeStartElement("color");
    stream.writeCharacters(color->currentText());
    stream.writeEndElement();//end color

    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }

    if( NULL != activation)
        stream.writeEndElement(); //end stimulus
}


/* ########################################################################## */

CTButton::CTButton(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "light_button";
    isAction = action;
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Button #" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the duration interval ********************/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                duration_min, SLOT(setEnabled(bool)));
        connect(state, SIGNAL(toggled(bool)),
                duration_max, SLOT(setEnabled(bool)));

        connect(duration_min, SIGNAL(valueChanged(double)),
                duration_max, SLOT(setMinimumValue(double)));
        connect(duration_max, SIGNAL(valueChanged(double)),
                duration_min, SLOT(setMaximumValue(double)));
    }
    else{
        duration_min = NULL;
        duration_max = NULL;
    }
    /**************************************************************************/

    /* Add elements for control over the intensity interval *******************/
    layout->addWidget(new QLabel("Intensity interval"), row_index, 1);
    intensity_min = new CTSpinBox();
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_min->setEnabled(false);
    layout->addWidget(intensity_min, row_index, 2);
    intensity_max = new CTSpinBox();
    intensity_max->setup(" %", 0, 100, 1, 100);
    intensity_max->setEnabled(false);
    layout->addWidget(intensity_max, row_index, 3);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            intensity_min, SLOT(setEnabled(bool)));
    connect(state, SIGNAL(toggled(bool)),
            intensity_max, SLOT(setEnabled(bool)));

    connect(intensity_min, SIGNAL(valueChanged(int)),
            intensity_max, SLOT(setMinimumValue(int)));
    connect(intensity_max, SIGNAL(valueChanged(int)),
            intensity_min, SLOT(setMaximumValue(int)));

    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}


void CTButton::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}

void CTButton::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_max->setup(" %", 0, 100, 1, 100);
}


void CTButton::setParameters(bool b, QHash<QString,QVariant> attr)
{
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        intensity_max->setMinimum(attr["val_intensity_min"].toInt());
        intensity_min->setValue(attr["val_intensity_min"].toInt());
        intensity_max->setValue(attr["val_intensity_max"].toInt());

        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
    }
}

void CTButton::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    stream.writeStartElement("intensity");
    stream.writeTextElement("from",intensity_min->cleanText());
    stream.writeTextElement("to",intensity_max->cleanText());
    stream.writeEndElement(); // end intensity
    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }

    if( NULL != activation)
        stream.writeEndElement(); //end stimulus
}



/* ########################################################################## */

CTLight::CTLight(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "light";
    isAction = action;

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Light #" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the duration interval ********************/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                duration_min, SLOT(setEnabled(bool)));
        connect(state, SIGNAL(toggled(bool)),
                duration_max, SLOT(setEnabled(bool)));

        connect(duration_min, SIGNAL(valueChanged(double)),
                duration_max, SLOT(setMinimumValue(double)));
        connect(duration_max, SIGNAL(valueChanged(double)),
                duration_min, SLOT(setMaximumValue(double)));
    }else{
        duration_min = NULL;
        duration_max = NULL;
    }
    /**************************************************************************/

    /* Add elements for control over the intensity interval *******************/
    layout->addWidget(new QLabel("Intensity interval"), row_index, 1);
    intensity_min = new CTSpinBox();
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_min->setEnabled(false);
    layout->addWidget(intensity_min, row_index, 2);
    intensity_max = new CTSpinBox();
    intensity_max->setup(" %", 0, 100, 1,100);
    intensity_max->setEnabled(false);
    layout->addWidget(intensity_max, row_index, 3);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            intensity_min, SLOT(setEnabled(bool)));
    connect(state, SIGNAL(toggled(bool)),
            intensity_max, SLOT(setEnabled(bool)));

    connect(intensity_min, SIGNAL(valueChanged(int)),
            intensity_max, SLOT(setMinimumValue(int)));
    connect(intensity_max, SIGNAL(valueChanged(int)),
            intensity_min, SLOT(setMaximumValue(int)));
    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}


void CTLight::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}

void CTLight::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
    intensity_min->setup(" %", 0, 100, 1, 0);
    intensity_max->setup(" %", 0, 100, 1, 100);
}

void CTLight::setParameters(bool b, QHash<QString,QVariant> attr)
{
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
        intensity_max->setMinimum(attr["val_intensity_min"].toInt());
        intensity_min->setValue(attr["val_intensity_min"].toInt());
        intensity_max->setValue(attr["val_intensity_max"].toInt());
    }
}

void CTLight::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    stream.writeStartElement("intensity");
    stream.writeTextElement("from",intensity_min->cleanText());
    stream.writeTextElement("to",intensity_max->cleanText());
    stream.writeEndElement(); // end intensity
    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }


    if( NULL != activation)
        stream.writeEndElement(); //end stimulus
}

/* ########################################################################## */

CTConstLight::CTConstLight(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "light";
    isAction = action;

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Light #" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the duration interval ********************/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                duration_min, SLOT(setEnabled(bool)));
        connect(state, SIGNAL(toggled(bool)),
                duration_max, SLOT(setEnabled(bool)));

        connect(duration_min, SIGNAL(valueChanged(double)),
                duration_max, SLOT(setMinimumValue(double)));
        connect(duration_max, SIGNAL(valueChanged(double)),
                duration_min, SLOT(setMaximumValue(double)));
    }
    else{
        duration_min = NULL;
        duration_max = NULL;
    }
    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}

void CTConstLight::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}

void CTConstLight::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
}


void CTConstLight::setParameters(bool b, QHash<QString,QVariant> attr)
{
    /*proccess if enabled*/
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
    }
}


void CTConstLight::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }
    if( NULL != activation)
        stream.writeEndElement(); //end stimulus
}

/* ########################################################################## */

CTSpeaker::CTSpeaker(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "speaker";
    isAction = action;
    soundList = QStringList() << "Audio2_10sec.wma" << "Audio2_17sec.wma"
                              << "Audio2_2sec.mp3" << "Audio2_4sec.wma"
                              << "Audio2_7sec.wma" << "Audio3_4sec.mp3"
                              << "Audio3_7sec.wma" << "Audio4_4sec.mp3"
                              << "Audio4_7sec.wma" << "Audio5_7sec.wma"
                              << "Audio_10sec.wma" << "Audio_12sec.wma"
                              << "Audio_14sec.mp3" << "Audio_15sec.wma"
                              << "Audio_17sec.wma" << "Audio_20sec.wma"
                              << "Audio_21sec.wma" << "Audio_2sec.wma"
                              << "Audio_3sec.mp3" << "Audio_45sec.wma"
                              << "Audio_4sec.wma" << "Audio_60sec.wma"
                              << "Audio_7sec.wma" << "Audio_8sec.wma"
                              << "Audio_9sec.wma";

    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Sound#" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        if(id == 1)
            activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        else
            activation->setup(" sec.", 1, 1.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the volume interval **********************/
    layout->addWidget(new QLabel("Volume interval"), row_index, 1);
    volume_min = new CTSpinBox();
    volume_min->setup(" %", 0,100, 1, 0);
    volume_min->setEnabled(false);
    layout->addWidget(volume_min, row_index, 2);
    volume_max = new CTSpinBox();
    volume_max->setup(" %", 0, 100, 1,100);
    volume_max->setEnabled(false);
    layout->addWidget(volume_max, row_index, 3);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            volume_min, SLOT(setEnabled(bool)));
    connect(state, SIGNAL(toggled(bool)),
            volume_max, SLOT(setEnabled(bool)));

    connect(volume_min, SIGNAL(valueChanged(int)),
            volume_max, SLOT(setMinimumValue(int)));
    connect(volume_max, SIGNAL(valueChanged(int)),
            volume_min, SLOT(setMaximumValue(int)));
    /**************************************************************************/

    /* Add elements for control over the duration interval **********************/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;

        connect(state,SIGNAL(toggled(bool)),
                duration_min,SLOT(setEnabled(bool)));
        connect(state,SIGNAL(toggled(bool)),
                duration_max,SLOT(setEnabled(bool)));

        connect(duration_min,SIGNAL(valueChanged(double)),
                duration_max,SLOT(setMinimumValue(double)));
        connect(duration_max,SIGNAL(valueChanged(double)),
                duration_min,SLOT(setMaximumValue(double)));
    }
    else{
        duration_min = NULL;
        duration_max = NULL;
    }

    if(!isAction)
    {
        connect(duration_max, SIGNAL(valueChanged(double)), this, SLOT(durationValueChanged()));
        connect(activation, SIGNAL(valueChanged(double)), this, SLOT(durationValueChanged()));
    }
    /**************************************************************************/

    /* Add elements for control over the sound file ***************************/
    layout->addWidget(new QLabel("Sound file"), row_index, 1);
    sound = new QComboBox();
    sound->addItems(soundList);
    sound->setEnabled(false);
    layout->addWidget(sound, row_index, 2, 1, 2);
    row_index++;

    connect(state, SIGNAL(toggled(bool)),
            sound, SLOT(setEnabled(bool)));
    /**************************************************************************/

    /* Add elements for control over the sound source ***************************/
    layout->addWidget(new QLabel("Use speakers:"),row_index,0);
    layout->addWidget(new QLabel("Left wall"),row_index,1);
    left_front = new QCheckBox("Front");
    left_front->setEnabled(false);
    layout->addWidget(left_front,row_index,2);
    left_rear = new QCheckBox("Rear");
    left_rear->setEnabled(false);
    layout->addWidget(left_rear,row_index,3);
    row_index++;

    layout->addWidget(new QLabel("Right wall"),row_index,1);
    right_front = new QCheckBox("Front");
    right_front->setEnabled(false);
    layout->addWidget(right_front,row_index,2);
    right_rear = new QCheckBox("Rear");
    right_rear->setEnabled(false);
    layout->addWidget(right_rear,row_index,3);

    connect(state,SIGNAL(toggled(bool)),
            left_front,SLOT(setEnabled(bool)));
    connect(state,SIGNAL(toggled(bool)),
            left_rear,SLOT(setEnabled(bool)));
    connect(state,SIGNAL(toggled(bool)),
            right_front,SLOT(setEnabled(bool)));
    connect(state,SIGNAL(toggled(bool)),
            right_rear,SLOT(setEnabled(bool)));

    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}

void CTSpeaker::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}


void CTSpeaker::durationValueChanged()
{
    emit(durationChanged(duration_max->value() + activation->value()));
}

void CTSpeaker::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
    volume_min->setup(" %", 0,100, 1, 0);
    volume_max->setup(" %", 0, 100, 1,100);
    sound->setCurrentIndex(sound->findText("Hello.wav"));
    right_front->setChecked(false);
    right_rear->setChecked(false);
    left_front->setChecked(false);
    left_rear->setChecked(false);
}

void CTSpeaker::setParameters(bool b, QHash<QString,QVariant> attr)
{
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
        volume_max->setMinimum(attr["val_volume_min"].toDouble());
        volume_min->setValue(attr["val_volume_min"].toDouble());
        volume_max->setValue(attr["val_volume_max"].toDouble());

        if(attr["left_rear"].toString() == "true"){ left_rear->setChecked(true);}
        if(attr["left_front"].toString() == "true"){ left_front->setChecked(true);}
        if(attr["right_front"].toString() == "true"){ right_front->setChecked(true);}
        if(attr["right_rear"].toString() == "true") { right_rear->setChecked(true);}

        sound->setCurrentIndex(sound->findText(attr["sound"].toString()));
    }
}


void CTSpeaker::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    stream.writeAttribute("left_front",left_front->isChecked() ? "true" : "false");
    stream.writeAttribute("left_rear",left_rear->isChecked() ? "true" : "false");
    stream.writeAttribute("right_front",right_front->isChecked() ? "true" : "false");
    stream.writeAttribute("right_rear",right_rear->isChecked() ? "true" : "false");
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    stream.writeStartElement("volume");
    stream.writeTextElement("from", volume_min->cleanText());
    stream.writeTextElement("to", volume_max->cleanText());
    stream.writeEndElement(); //end volume

    stream.writeStartElement("sound");
    stream.writeCharacters(sound->currentText());
    stream.writeEndElement(); // end sound

    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }

    if (NULL != activation)
        stream.writeEndElement(); //end stimulus
}

/* ########################################################################## */

CTScreen::CTScreen(int id, bool action, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    name = "screen";
    isAction = action;
    moviesList = QStringList() << "shipfrombottomthanleft.wmv"
                               << "animalsinthecentre.wmv"
                               << "animalsinthecentre_2.wmv"
                               << "ballfromleft_rotating.wmv"
                               << "catfromright_middle.wmv"
                               << "ducksfromthebottom.wmv"
                               << "frogformleft_high.wmv"
                               << "jumpingtiger.wmv"
                               << "mickey_mouse_cucu.wmv"
                               << "monkeyfromleft_low.wmv";
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    layout->setMargin(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(5);
    int row_index = 0;

    state = new QCheckBox("Screen #" + QString::number(id));
    layout->addWidget(state, row_index, 0);

    /* Add elements for control over the activation time of a stimulus ********/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Activation time"), row_index, 1);
        activation = new CTDoubleSpinBox();
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        activation->setEnabled(false);
        layout->addWidget(activation, row_index, 2);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                activation, SLOT(setEnabled(bool)));
    }
    else { activation = NULL; }
    /**************************************************************************/

    /* Add elements for control over the duration interval ********************/
    if (!isAction)
    {
        layout->addWidget(new QLabel("Duration interval"), row_index, 1);
        duration_min = new CTDoubleSpinBox();
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_min->setEnabled(false);
        layout->addWidget(duration_min, row_index, 2);
        duration_max = new CTDoubleSpinBox();
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
        duration_max->setEnabled(false);
        layout->addWidget(duration_max, row_index, 3);
        row_index++;

        connect(state, SIGNAL(toggled(bool)),
                duration_min, SLOT(setEnabled(bool)));
        connect(state, SIGNAL(toggled(bool)),
                duration_max, SLOT(setEnabled(bool)));

        connect(duration_min, SIGNAL(valueChanged(double)),
                duration_max, SLOT(setMinimumValue(double)));
        connect(duration_max, SIGNAL(valueChanged(double)),
                duration_min, SLOT(setMaximumValue(double)));
    }
    else{
        duration_min = NULL;
        duration_max = NULL;
    }
    /**************************************************************************/

    /* Add elements for control over the video file ***************************/
    layout->addWidget(new QLabel("Video file"), row_index, 1);
    video = new QComboBox();
    video->addItems(moviesList);
    video->setEnabled(false);
    layout->addWidget(video, row_index, 2, 1, 2);

    connect(state, SIGNAL(toggled(bool)),
            video, SLOT(setEnabled(bool)));
    /**************************************************************************/

    connect(state, SIGNAL(toggled(bool)),
            this, SLOT(stateChecked(bool)));
}


void CTScreen::stateChecked(bool b)
{
    if(!b)
    {
        /*Set default values*/
        setDefault();
    }
}

void CTScreen::setDefault()
{
    if(!isAction)
    {
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
        duration_min->setup(" sec.", 1, 0.1, 1.0, 0.1, 0.1);
        duration_max->setup(" sec.", 1, 0.1, 300.0, 0.1, 1.0);
    }
    video->setCurrentIndex(video->findText("Tiger.avi"));
}

void CTScreen::setParameters(bool b, QHash<QString,QVariant> attr)
{
    if( b )
    {
        state->setChecked(true);
        if (NULL != activation)
        {
            activation->setValue(attr["val_activation"].toDouble());
        }
        if( NULL != duration_min && NULL != duration_max)
        {
            duration_max->setMinimum(attr["val_duration_min"].toDouble());
            duration_min->setValue(attr["val_duration_min"].toDouble());
            duration_max->setValue(attr["val_duration_max"].toDouble());
        }
        video->setCurrentIndex(video->findText(attr["val_video"].toString()));
    }
}


void CTScreen::getParameters(QXmlStreamWriter &stream){

    if (NULL != activation)
        stream.writeStartElement("stimulus");
    else
        stream.writeStartElement("action");
    stream.writeAttribute("enabled",state->isChecked() ? "true" : "false");
    stream.writeAttribute("id",QString::number(id));
    stream.writeAttribute("name",name);
    if (NULL != activation)
    {
        stream.writeStartElement("activation");
        stream.writeCharacters(activation->cleanText());
        stream.writeEndElement(); //end activation
    }
    stream.writeStartElement("video");
    stream.writeCharacters(video->currentText());
    stream.writeEndElement();// end video

    if(NULL != duration_min && NULL!= duration_max)
    {
        stream.writeStartElement("duration");
        stream.writeTextElement("from", duration_min->cleanText());
        stream.writeTextElement("to", duration_max->cleanText());
        stream.writeEndElement(); //end duration
    }
    if( NULL != activation)
        stream.writeEndElement(); //end stimulus
}

