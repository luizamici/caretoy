#include "ct_seekslider.h"
#include "phonon/MediaObject"

CTSeekSlider::CTSeekSlider(Phonon::MediaObject * media, QWidget * parent)
    : Phonon::SeekSlider(media,parent)
{
}
