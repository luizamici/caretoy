#ifndef CT_XMLHANDLER_H
#define CT_XMLHANDLER_H

#define CT_BLOCK_STICK 1
#define CT_BLOCK_RING 2
#define CT_BLOCK_FLOWER 3
#define CT_BLOCK_MICKEY 4
#define CT_BLOCK_U 5
#define CT_BLOCK_WALL_LEFT 6
#define CT_BLOCK_WALL_RIGHT 7
#define CT_BLOCK_WALL_SCREEN 8
#define CT_BLOCK_ARCH 9

#include <QXmlDefaultHandler>
#include "ct_conftoyflower.h"
#include "ct_conftoymickey.h"
#include "ct_conftoyring.h"
#include "TableOfScenarios/ct_logger.h"


class CTXmlHandler : public QXmlDefaultHandler
{
public:

    bool startDocument();
    bool startElement(const QString &, const QString &, const QString &qName,
                         const QXmlAttributes & att);
    bool endElement(const QString &, const QString &, const QString &);
    bool fatalError(const QXmlParseException &exception);
    bool characters(const QString& ch);

    void setWidget(int id_widget, QWidget *widget, int stimuli, int actions);
    void setStimuli(QList<CTConstLight *> &lightStimuli, QList<CTSpeaker *> &speakerStimuli,
                    QList<CTLight*> &archLightStimuli, QList<CTScreen*> &screenStimuli,
                    QList<CTBigLight*> &bigLightStimuli, QList<CTButton*> &buttonStimuli);
    void setActions(QList<CTConstLight *> &lightAction, QList<CTSpeaker *> &speakerAction,
                    QList<CTScreen*> &screenAction , QList<CTBigLight*> &bigLightAction,
                    QList<CTButton*> &buttonAction, QList<CTLight*> &archLightAction);
    double getBlockDuration();

    int idWidget;
    int num_stimuli;
    int num_actions;
    bool enabled;
    double block_duration;
    QString event_name;
    QHash<QString,QVariant> attr;
    QString block_name;

    QList<CTConstLight *> light_stimuli, light_actions;
    QList<CTSpeaker *> speaker_stimuli, speaker_actions;
    QList<CTLight *> arch_light_stimuli, arch_light_actions;
    QList<CTScreen *> screen_stimuli, screen_actions;
    QList<CTBigLight *> biglight_stimuli, biglight_actions;
    QList<CTButton *> button_stimuli, button_actions;

    CTConstLight *light;
    CTLight *archLight;
    CTSpeaker *speaker;
    CTScreen *screen;
    CTBigLight *bigLight;
    CTButton *button;
    CTDoubleSpinBox *qsb_force;
    CTDoubleSpinBox *qsb_duration_min;
    CTDoubleSpinBox *qsb_duration_max;
    CTDoubleSpinBox *qsb_pressure;
    CTSpinBox *qsb_block_repetitions;

    QPlainTextEdit *qte_comment;   
    QRadioButton *qrb_null_event;
    QRadioButton *qrb_pressure_event;
    QRadioButton *qrb_force_event;
    QRadioButton *qrb_position_event;
    QRadioButton *qrb_body_event;
    QRadioButton *qrb_head_event;
    QRadioButton *qrb_button_pressed_event;

    QComboBox *qcb_force;
    QComboBox *qcb_position;
    QComboBox *qcb_body;
    QComboBox *qcb_head;
    QComboBox *qcb_pressure;
    QComboBox *qcb_button;
private:
    QStringList elementName;
    QList<int> elementIndentation;

    bool setSubWidgets(int id_widget, QWidget *widget);
};

#endif // CT_XMLHANDLER_H
