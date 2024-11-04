/*
Copyright 2023  Kaj-Michael Lang <milang@tal.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

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
