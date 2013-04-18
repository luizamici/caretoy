#ifndef CT_MEDIAOBJECT_H
#define CT_MEDIAOBJECT_H
#include "phonon/MediaObject"

class CTMediaObject : public Phonon::MediaObject
{
    Q_OBJECT
public:
    CTMediaObject();

public slots:
    void update(int millisec);
};

#endif // CT_MEDIAOBJECT_H
