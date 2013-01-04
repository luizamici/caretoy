#ifndef CT_XMLHANDLER_H
#define CT_XMLHANDLER_H

#include <QXmlDefaultHandler>
#include "ct_conftoyflower.h"


class CTXmlHandler : public QXmlDefaultHandler
{
public:

    bool startDocument();
    bool startElement(const QString &, const QString &, const QString &qName,
                         const QXmlAttributes & att);
    bool endElement(const QString &, const QString &, const QString &);
    bool fatalError(const QXmlParseException &exception);
    bool characters(const QString& ch);

    QStringList& names();
    QList<int>& indentations();
    void setWidget(QString id_widget, QWidget *widget, int stimuli, int actions);
    void setStimuli(QList<CTConstLight *> &lightStimuli, QList<CTSpeaker *> &speakerStimuli);
    void setActions(QList<CTConstLight *> &lightAction, QList<CTSpeaker *> &speakerAction);
    double getBlockDuration();

    QString idWidget;
    int num_stimuli;
    int num_actions;
    bool enabled;
    double block_duration;
    QHash<QString,QVariant> attr;

    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    CTConstLight *light;
    CTSpeaker *speaker;
    CTDoubleSpinBox *qsb_force;
    CTDoubleSpinBox *qsb_duration_min;
    CTDoubleSpinBox *qsb_duration_max;
    CTSpinBox *qsb_block_repetitions;

    QPlainTextEdit *qte_comment;   
    QRadioButton *qrb_null_event;
    QRadioButton *qrb_force_event;
    QRadioButton *qrb_position_event;
    QRadioButton *qrb_body_event;
    QRadioButton *qrb_head_event;
    QComboBox *qcb_force;
    QComboBox *qcb_position;
    QComboBox *qcb_body;
    QComboBox *qcb_head;


private:
    int indentationLevel;
    QStringList elementName;
    QList<int> elementIndentation;
};

#endif // CT_XMLHANDLER_H
