#include "eobject.h"
#include "ewindows.h"

EObject::EObject(QObject *parent)
    : QObject (parent)
{
    m_ewindows = new EWindows(this);
}

QObject *EObject::windows()
{
    return m_ewindows;
}
