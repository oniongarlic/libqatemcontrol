#include "qatemsubsystembase.h"

QAtemSubsystemBase::QAtemSubsystemBase(QObject *parent)
    : QObject{parent}
{

}

QAtemSubsystemBase::~QAtemSubsystemBase()
{
    if (m_atemConnection) {
        for (const auto &i : qAsConst(m_commands)) {
            m_atemConnection->unregisterCommand(i, this);
        }
    }
}

void QAtemSubsystemBase::setAtemConnection(QAtemConnection *qac)
{
    if (m_atemConnection) {
        for (const auto &i : qAsConst(m_commands)) {
            m_atemConnection->unregisterCommand(i, this);
        }
    }

    m_atemConnection=qac;
    if (qac==nullptr)
        return;

    for (const auto &i : qAsConst(m_commands)) {
        m_atemConnection->registerCommand(i, this, "on"+i);
    }

    emit atemConnectionChanged();
}

bool QAtemSubsystemBase::sendCommand(const QByteArray cmd, const QByteArray payload)
{
    if (!m_atemConnection) {
        qWarning() << "No ATEM connection set" ;
        return false;
    }
    return m_atemConnection->sendCommand(cmd, payload);
}
