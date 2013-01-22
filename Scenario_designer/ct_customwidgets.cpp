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

//TOBE deprecated
//void CTBigLight::setParameters(QDomElement root)
//{
//    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

//    if (NULL != activation)
//    {
//        double val_activation =
//                root.namedItem("activation").toElement().text().toDouble();
//        activation->setValue(val_activation);
//    }
//    if(NULL != duration_min && NULL != duration_max)
//    {
//        QDomElement xml_duration = root.namedItem("duration").toElement();
//        double val_duration_min =
//                xml_duration.namedItem("from").toElement().text().toDouble();
//        double val_duration_max =
//                xml_duration.namedItem("to").toElement().text().toDouble();
//        duration_min->setMinimum(val_duration_min);
//        duration_min->setValue(val_duration_min);
//        duration_max->setValue(val_duration_max);
//    }
//    QDomElement xml_intensity = root.namedItem("intensity").toElement();
//    int val_intensity_min =
//            xml_intensity.namedItem("from").toElement().text().toInt();
//    int val_intensity_max =
//            xml_intensity.namedItem("to").toElement().text().toInt();
//    intensity_min->setMinimum(val_intensity_min);
//    intensity_min->setValue(val_intensity_min);
//    intensity_max->setValue(val_intensity_max);

//    QDomElement xml_rings = root.namedItem("area").toElement();
//    int val_rings_min =
//            xml_rings.namedItem("from").toElement().text().toInt();
//    int val_rings_max =
//            xml_rings.namedItem("to").toElement().text().toInt();
//    rings_min->setValue(val_rings_min);
//    rings_min->setValue(val_rings_min);
//    rings_max->setValue(val_rings_max);

//    QString val_color = root.namedItem("color").toElement().text();
//    color->setCurrentIndex(color->findText(val_color));
//}


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


//TOBE deprecated
//QDomElement CTBigLight::getParameters()
//{
//    QDomDocument doc;
//    QDomElement root;

//    if (NULL != activation) { root = doc.createElement("stimulus"); }
//    else { root = doc.createElement("action"); }
//    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
//    root.setAttribute("id", id);
//    root.setAttribute("name", name);

//    if (NULL != activation)
//    {
//        QDomElement xml_activation = doc.createElement("activation");
//        root.appendChild(xml_activation);
//        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
//    }
//    if(NULL != duration_min && NULL!= duration_max)
//    {
//        QDomElement xml_duration = doc.createElement("duration");
//        root.appendChild(xml_duration);
//        QDomElement xml_duration_min = doc.createElement("from");
//        xml_duration.appendChild(xml_duration_min);
//        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
//        QDomElement xml_duration_max = doc.createElement("to");
//        xml_duration.appendChild(xml_duration_max);
//        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
//    }

//    QDomElement xml_intensity = doc.createElement("intensity");
//    root.appendChild(xml_intensity);
//    QDomElement xml_intensity_min = doc.createElement("from");
//    xml_intensity.appendChild(xml_intensity_min);
//    xml_intensity_min.appendChild(
//                doc.createTextNode(intensity_min->cleanText()));
//    QDomElement xml_intensity_max = doc.createElement("to");
//    xml_intensity.appendChild(xml_intensity_max);
//    xml_intensity_max.appendChild(
//                doc.createTextNode(intensity_max->cleanText()));

//    QDomElement xml_rings = doc.createElement("area");
//    root.appendChild(xml_rings);
//    QDomElement xml_rings_min = doc.createElement("from");
//    xml_rings.appendChild(xml_rings_min);
//    xml_rings_min.appendChild(doc.createTextNode(rings_min->cleanText()));
//    QDomElement xml_rings_max = doc.createElement("to");
//    xml_rings.appendChild(xml_rings_max);
//    xml_rings_max.appendChild(doc.createTextNode(rings_max->cleanText()));

//    QDomElement xml_color = doc.createElement("color");
//    root.appendChild(xml_color);
//    xml_color.appendChild(doc.createTextNode(color->currentText()));

//    return root;
//}

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

//TOBE deprecated
//void CTButton::setParameters(QDomElement root)
//{
//    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

//    if (NULL != activation)
//    {
//        double val_activation =
//                root.namedItem("activation").toElement().text().toDouble();
//        activation->setValue(val_activation);
//    }
//    if(NULL != duration_min && NULL != duration_max)
//    {
//        QDomElement xml_duration = root.namedItem("duration").toElement();
//        double val_duration_min =
//                xml_duration.namedItem("from").toElement().text().toDouble();
//        double val_duration_max =
//                xml_duration.namedItem("to").toElement().text().toDouble();
//        duration_min->setMinimum(val_duration_min);
//        duration_min->setValue(val_duration_min);
//        duration_max->setValue(val_duration_max);
//    }
//    QDomElement xml_intensity = root.namedItem("intensity").toElement();
//    int val_intensity_min =
//            xml_intensity.namedItem("from").toElement().text().toInt();
//    int val_intensity_max =
//            xml_intensity.namedItem("to").toElement().text().toInt();
//    intensity_min->setMinimum(val_intensity_min);
//    intensity_min->setValue(val_intensity_min);
//    intensity_max->setValue(val_intensity_max);
//}


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


//TOBE deprecated
//QDomElement CTButton::getParameters()
//{
//    QDomDocument doc;
//    QDomElement root;

//    if (NULL != activation) { root = doc.createElement("stimulus"); }
//    else { root = doc.createElement("action"); }
//    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
//    root.setAttribute("id", id);
//    root.setAttribute("name", name);

//    if (NULL != activation)
//    {
//        QDomElement xml_activation = doc.createElement("activation");
//        root.appendChild(xml_activation);
//        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
//    }
//    if(NULL != duration_min && NULL!= duration_max)
//    {
//        QDomElement xml_duration = doc.createElement("duration");
//        root.appendChild(xml_duration);
//        QDomElement xml_duration_min = doc.createElement("from");
//        xml_duration.appendChild(xml_duration_min);
//        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
//        QDomElement xml_duration_max = doc.createElement("to");
//        xml_duration.appendChild(xml_duration_max);
//        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
//    }
//    QDomElement xml_intensity = doc.createElement("intensity");
//    root.appendChild(xml_intensity);
//    QDomElement xml_intensity_min = doc.createElement("from");
//    xml_intensity.appendChild(xml_intensity_min);
//    xml_intensity_min.appendChild(
//                doc.createTextNode(intensity_min->cleanText()));
//    QDomElement xml_intensity_max = doc.createElement("to");
//    xml_intensity.appendChild(xml_intensity_max);
//    xml_intensity_max.appendChild(
//                doc.createTextNode(intensity_max->cleanText()));

//    return root;
//}

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

//TOBE deprecated
void CTLight::setParameters(QDomElement root)
{
    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

    if (NULL != activation)
    {
        double val_activation =
                root.namedItem("activation").toElement().text().toDouble();
        activation->setValue(val_activation);
    }
    if( NULL != duration_min && NULL != duration_max)
    {
        QDomElement xml_duration = root.namedItem("duration").toElement();
        double val_duration_min =
                xml_duration.namedItem("from").toElement().text().toDouble();
        double val_duration_max =
                xml_duration.namedItem("to").toElement().text().toDouble();
        duration_min->setMinimum(val_duration_min);
        duration_min->setValue(val_duration_min);
        duration_max->setValue(val_duration_max);
    }

    QDomElement xml_intensity = root.namedItem("intensity").toElement();
    int val_intensity_min =
            xml_intensity.namedItem("from").toElement().text().toInt();
    int val_intensity_max =
            xml_intensity.namedItem("to").toElement().text().toInt();
    intensity_min->setMinimum(val_intensity_min);
    intensity_min->setValue(val_intensity_min);
    intensity_max->setValue(val_intensity_max);
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

//TOBE deprecated
QDomElement CTLight::getParameters()
{
    QDomDocument doc;
    QDomElement root;

    if (NULL != activation) { root = doc.createElement("stimulus"); }
    else { root = doc.createElement("action"); }
    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
    root.setAttribute("id", id);
    root.setAttribute("name", name);

    if (NULL != activation)
    {
        QDomElement xml_activation = doc.createElement("activation");
        root.appendChild(xml_activation);
        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
    }
    if(NULL != duration_min && NULL!= duration_max)
    {
        QDomElement xml_duration = doc.createElement("duration");
        root.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
    }

    QDomElement xml_intensity = doc.createElement("intensity");
    root.appendChild(xml_intensity);
    QDomElement xml_intensity_min = doc.createElement("from");
    xml_intensity.appendChild(xml_intensity_min);
    xml_intensity_min.appendChild(
                doc.createTextNode(intensity_min->cleanText()));
    QDomElement xml_intensity_max = doc.createElement("to");
    xml_intensity.appendChild(xml_intensity_max);
    xml_intensity_max.appendChild(
                doc.createTextNode(intensity_max->cleanText()));

    return root;
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

//TOBE deprecated
void CTConstLight::setParameters(QDomElement root)
{
    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

    if (NULL != activation)
    {
        double val_activation =
                root.namedItem("activation").toElement().text().toDouble();
        activation->setValue(val_activation);
    }
    if( NULL != duration_min && NULL != duration_max){
        QDomElement xml_duration = root.namedItem("duration").toElement();
        double val_duration_min =
                xml_duration.namedItem("from").toElement().text().toDouble();
        double val_duration_max =
                xml_duration.namedItem("to").toElement().text().toDouble();
        duration_min->setMinimum(val_duration_min);
        duration_min->setValue(val_duration_min);
        duration_max->setValue(val_duration_max);
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

//TOBE deprecated
QDomElement CTConstLight::getParameters()
{
    QDomDocument doc;
    QDomElement root;

    if (NULL != activation) { root = doc.createElement("stimulus"); }
    else { root = doc.createElement("action"); }
    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
    root.setAttribute("id", id);
    root.setAttribute("name", name);

    if (NULL != activation)
    {
        QDomElement xml_activation = doc.createElement("activation");
        root.appendChild(xml_activation);
        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
    }
    if(NULL != duration_min && NULL!= duration_max)
    {
        QDomElement xml_duration = doc.createElement("duration");
        root.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
    }

//    QDomElement xml_duration = doc.createElement("duration");
//    root.appendChild(xml_duration);
//    QDomElement xml_duration_min = doc.createElement("from");
//    xml_duration.appendChild(xml_duration_min);
//    xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
//    QDomElement xml_duration_max = doc.createElement("to");
//    xml_duration.appendChild(xml_duration_max);
//    xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));

    return root;
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
        activation->setup(" sec.", 1, 0.0, 300.0, 0.1, 0.0);
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

//TOBE deprecated
void CTSpeaker::setParameters(QDomElement root)
{
    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

    if (NULL != activation)
    {
        double val_activation =
                root.namedItem("activation").toElement().text().toDouble();
        activation->setValue(val_activation);
    }

    if( NULL != duration_min && NULL != duration_max)
    {
        QDomElement xml_duration = root.namedItem("duration").toElement();
        double val_duration_min =
                xml_duration.namedItem("from").toElement().text().toDouble();
        double val_duaration_max =
                xml_duration.namedItem("to").toElement().text().toDouble();

        duration_min->setMinimum(val_duration_min);
        duration_min->setValue(val_duration_min);
        duration_max->setValue(val_duaration_max);
    }

    QDomElement xml_volume = root.namedItem("volume").toElement();
    int val_volume_min =
            xml_volume.namedItem("from").toElement().text().toInt();
    int val_volume_max =
            xml_volume.namedItem("to").toElement().text().toInt();
    volume_min->setMinimum(val_volume_min);
    volume_min->setValue(val_volume_min);
    volume_max->setValue(val_volume_max);

//    QDomElement xml_duration = root.namedItem("duration").toElement();
//    double val_duration_min =
//            xml_duration.namedItem("from").toElement().text().toDouble();
//    double val_duaration_max =
//            xml_duration.namedItem("to").toElement().text().toDouble();

//    duration_min->setMinimum(val_duration_min);
//    duration_min->setValue(val_duration_min);
//    duration_max->setValue(val_duaration_max);

    if(root.attribute("left_rear") == "true"){ left_rear->setChecked(true);}
    if(root.attribute("left_front") == "true"){ left_front->setChecked(true);}
    if(root.attribute("right_front") == "true"){ right_front->setChecked(true);}
    if(root.attribute("right_rear") == "true") { right_rear->setChecked(true);}

    QString val_sound = root.namedItem("sound").toElement().text();
    sound->setCurrentIndex(sound->findText(val_sound));
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

//TOBE deprecated
QDomElement CTSpeaker::getParameters()
{
    QDomDocument doc;
    QDomElement root;

    if (NULL != activation) { root = doc.createElement("stimulus"); }
    else { root = doc.createElement("action"); }
    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
    root.setAttribute("id", id);
    root.setAttribute("name", name);
    root.setAttribute("left_front",left_front->isChecked() ? "true" : "false");
    root.setAttribute("left_rear",left_rear->isChecked() ? "true" : "false");
    root.setAttribute("right_front",right_front->isChecked() ? "true" : "false");
    root.setAttribute("right_rear",right_rear->isChecked() ? "true" : "false");

    if (NULL != activation)
    {
        QDomElement xml_activation = doc.createElement("activation");
        root.appendChild(xml_activation);
        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
    }
    if(NULL != duration_min && NULL!= duration_max)
    {
        QDomElement xml_duration = doc.createElement("duration");
        root.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
    }

    QDomElement xml_volume = doc.createElement("volume");
    root.appendChild(xml_volume);
    QDomElement xml_volume_min = doc.createElement("from");
    xml_volume.appendChild(xml_volume_min);
    xml_volume_min.appendChild(doc.createTextNode(volume_min->cleanText()));
    QDomElement xml_volume_max = doc.createElement("to");
    xml_volume.appendChild(xml_volume_max);
    xml_volume_max.appendChild(doc.createTextNode(volume_max->cleanText()));

    QDomElement xml_sound = doc.createElement("sound");
    root.appendChild(xml_sound);
    xml_sound.appendChild(doc.createTextNode(sound->currentText()));

    return root;
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


/*!
 * \brief CTScreen::setParameters
 *
 * Sets values of all configurable parameters based on the content of the
 * supplied XML tree.
 *
 * \param root XML tree containing all parameters.
 */

//TOBE deprecated
void CTScreen::setParameters(QDomElement root)
{
    if ("true" == root.attribute("enabled")) { state->setChecked(true); }

    if (NULL != activation)
    {
        double val_activation =
                root.namedItem("activation").toElement().text().toDouble();
        activation->setValue(val_activation);
    }
    if( NULL != duration_min && NULL != duration_max)
    {
        QDomElement xml_duration = root.namedItem("duration").toElement();
        double val_duration_min =
                xml_duration.namedItem("from").toElement().text().toDouble();
        double val_duaration_max =
                xml_duration.namedItem("to").toElement().text().toDouble();

        duration_min->setMinimum(val_duration_min);
        duration_min->setValue(val_duration_min);
        duration_max->setValue(val_duaration_max);
    }

    QString val_video = root.namedItem("video").toElement().text();
    video->setCurrentIndex(video->findText(val_video));
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


/*!
 * \brief CTScreen::getParameters
 *
 * Retrieves values of all configurable parameters and returns them as a tree.
 *
 * \return XML tree containing data of all configurable parameters.
 */
//TOBE deprecated
QDomElement CTScreen::getParameters()
{
    QDomDocument doc;
    QDomElement root;

    if (NULL != activation) { root = doc.createElement("stimulus"); }
    else { root = doc.createElement("action"); }
    root.setAttribute("enabled", state->isChecked() ? "true" : "false");
    root.setAttribute("id", id);
    root.setAttribute("name", name);

    if (NULL != activation)
    {
        QDomElement xml_activation = doc.createElement("activation");
        root.appendChild(xml_activation);
        xml_activation.appendChild(doc.createTextNode(activation->cleanText()));
    }
    if(NULL != duration_min && NULL!= duration_max)
    {
        QDomElement xml_duration = doc.createElement("duration");
        root.appendChild(xml_duration);
        QDomElement xml_duration_min = doc.createElement("from");
        xml_duration.appendChild(xml_duration_min);
        xml_duration_min.appendChild(doc.createTextNode(duration_min->cleanText()));
        QDomElement xml_duration_max = doc.createElement("to");
        xml_duration.appendChild(xml_duration_max);
        xml_duration_max.appendChild(doc.createTextNode(duration_max->cleanText()));
    }

    QDomElement xml_video = doc.createElement("video");
    root.appendChild(xml_video);
    xml_video.appendChild(doc.createTextNode(video->currentText()));

    return root;
}
