
#include "qatemfairlight.h"
#include "qatemconnection.h"

QAtemFairlight::QAtemFairlight(QObject *parent) : QObject(parent)
{

}

QAtemFairlight::~QAtemFairlight()
{

}

void QAtemFairlight::setAtemConnection(QAtemConnection *qac)
{
    if (m_atemConnection) {
        m_atemConnection->unregisterCommand("FDLv", this);
        m_atemConnection->unregisterCommand("FMLv", this);

    }
    m_atemConnection=qac;

    m_atemConnection->registerCommand("FDLv", this, "onFDLv");
    m_atemConnection->registerCommand("FMLv", this, "onFMLv");

    emit atemConnectionChanged();
}

void QAtemFairlight::setFairlightAudioLevelsEnabled(bool enabled)
{
    QByteArray cmd("SFLN");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(enabled);

    m_atemConnection->sendCommand(cmd, payload);
}

void QAtemFairlight::onFMLv(const QByteArray &payload)
{

}

void QAtemFairlight::onFDLv(const QByteArray &payload)
{

}
