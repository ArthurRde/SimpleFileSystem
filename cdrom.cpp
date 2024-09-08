#include "cdrom.h"

bool CDROM::getIsBurned() const
{
    return isBurned;
}

void CDROM::setIsBurned(bool newIsBurned)
{
    isBurned = newIsBurned;
}

CDROM::CDROM(string portName_) {
    portName = portName_;
}

