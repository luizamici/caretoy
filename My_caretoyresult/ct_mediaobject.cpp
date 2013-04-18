#include "ct_mediaobject.h"

CTMediaObject::CTMediaObject()
{
}

void CTMediaObject::update(int millisec){
    seek(millisec);
    tick(millisec);
}
