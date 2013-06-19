#ifndef CT_SEEKSLIDER_H
#define CT_SEEKSLIDER_H

#include "phonon/SeekSlider"
#include "phonon/MediaObject"

class CTSeekSlider : public Phonon::SeekSlider
{
    Q_OBJECT
public:
    CTSeekSlider(Phonon::MediaObject *media, QWidget *parent);
};

#endif // CT_SEEKSLIDER_H
