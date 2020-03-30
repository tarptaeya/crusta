#include "webchannel.h"
#include "webchannel_p.h"

WebChannelObject::WebChannelObject(QObject *parent)
    : QObject(parent)
{
    m_storage = new Storage(this);
}

QObject *WebChannelObject::storage() const
{
    return m_storage;
}

Storage::Storage(QObject *parent)
    : QObject(parent)
{
}

QVariant Storage::get(const QString &string) const
{
    return m_settings.value(string);
}

void Storage::set(const QString &string, const QVariant &value)
{
    m_settings.setValue(string, value);
}
