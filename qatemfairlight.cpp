
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
        m_atemConnection->unregisterCommand("FAAI", this);
        m_atemConnection->unregisterCommand("FAIP", this);
        m_atemConnection->unregisterCommand("FASP", this);
    }

    m_atemConnection=qac;

    m_atemConnection->registerCommand("FDLv", this, "onFDLv");
    m_atemConnection->registerCommand("FMLv", this, "onFMLv");
    m_atemConnection->registerCommand("FAAI", this, "onFAAI");
    m_atemConnection->registerCommand("FAIP", this, "onFAIP");
    m_atemConnection->registerCommand("FASP", this, "onFASP");

    emit atemConnectionChanged();
}

void QAtemFairlight::setAudioLevelsEnabled(bool enabled)
{
    QByteArray cmd("SFLN");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(enabled);

    m_atemConnection->sendCommand(cmd, payload);
}

void QAtemFairlight::resetPeakLevels(bool all, bool master)
{
    QByteArray cmd("RFLP");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(all | master << 1) ;

    m_atemConnection->sendCommand(cmd, payload);
}

void QAtemFairlight::onFMLv(const QByteArray &payload)
{
    qDebug() << "FMLv: " << payload;
}

void QAtemFairlight::onFDLv(const QByteArray &payload)
{
    qDebug() << "FDLv: " << payload;
}

void QAtemFairlight::onFAAI(const QByteArray &payload)
{
    qDebug() << "FAAI: " << payload;
}

void QAtemFairlight::onFAIP(const QByteArray &payload)
{
    qDebug() << "FAIP: " << payload;
}

void QAtemFairlight::onFASP(const QByteArray &payload)
{
    qDebug() << "FASP: " << payload;

    QAtem::U16_U8 as;

    as.u8[1] = static_cast<quint8>(payload.at(6));
    as.u8[0] = static_cast<quint8>(payload.at(7));

    qDebug() << "AudioSource: " << as.u16;
}
