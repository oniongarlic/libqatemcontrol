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

#include "qatemsupersource.h"

QAtemSuperSource::QAtemSuperSource(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SSBP" << "SSrc" << "SSCs" << "SSBd" << "SSSB";
    m_atemConnection=nullptr;

    createSuperSourceBoxes();

    m_ssart.fillSource=0;
    m_ssart.cutSource=0;
    m_ssart.option=QAtem::SuperSourceBackground;
    m_ssart.clip=0;
    m_ssart.gain=0;
    m_ssart.premultiplied=true;
    m_ssart.invertkey=false;
}

QAtemSuperSource::~QAtemSuperSource()
{
    qDeleteAll(m_boxes);
}

void QAtemSuperSource::setAtemConnection(QAtemConnection *qac)
{
    QAtemSubsystemBase::setAtemConnection(qac);
    for (int i=0;i<4;i++) {
        m_boxes[i]->setAtemConnection(qac);
    }
}

void QAtemSuperSource::createSuperSourceBoxes()
{
    m_boxes.resize(4);
    for (int i=0;i<4;i++) {
        m_boxes[i]=new QAtemSuperSourceBox(m_ssid, i, m_atemConnection);
    }
}

QAtem::SuperSourceArt QAtemSuperSource::getSuperSourceProperties()
{
    return m_ssart;
}

void QAtemSuperSource::updateSuperSourceProperties()
{
    QByteArray cmd("CSSc");
    QByteArray payload(16, 0x0);
    QAtem::U16_U8 v1;

    payload[0]=1+2+4+8+16+32+64;
    payload[1]=m_ssid;

    v1.u16 = m_ssart.fillSource;
    payload[2]=static_cast<char>(v1.u8[1]);
    payload[3]=static_cast<char>(v1.u8[0]);

    v1.u16 = m_ssart.cutSource;
    payload[4]=static_cast<char>(v1.u8[1]);
    payload[5]=static_cast<char>(v1.u8[0]);

    payload[6]=m_ssart.option;
    payload[7]=m_ssart.premultiplied;

    v1.u16 = qBound(0, (int)(m_ssart.clip * 1000.0), 1000);
    payload[8]=static_cast<char>(v1.u8[1]);
    payload[9]=static_cast<char>(v1.u8[0]);

    v1.u16 = qBound(0, (int)(m_ssart.gain * 1000.0), 1000);
    payload[10]=static_cast<char>(v1.u8[1]);
    payload[11]=static_cast<char>(v1.u8[0]);

    payload[12]=m_ssart.invertkey;

    sendCommand(cmd, payload);
}

void QAtemSuperSource::setSuperSource(quint8 boxid, bool enabled, uint source, QPoint pos, uint size, bool crop_enabled, QRect crop)
{
    auto b=m_boxes.at(boxid);

    b->setBox(enabled, source, pos, size, crop_enabled, crop);
}

void QAtemSuperSource::onSSrc(const QByteArray &payload)
{
    qDebug() << "SSrc: " << payload.toHex(':');

    quint8 ssid=static_cast<qint8>(payload.at(6));

    if (m_ssid!=ssid)
        return;

    quint16 art_fill_source=QAtem::uint16at(payload, 8);
    quint16 art_cut_source=QAtem::uint16at(payload, 10);

    quint8 art_option=static_cast<qint8>(payload.at(12));
    bool art_premultiplied=static_cast<qint8>(payload.at(13));

    quint16 art_clip=QAtem::uint16at(payload, 14);
    quint16 art_gain=QAtem::uint16at(payload, 16);
    bool art_invert_key=static_cast<qint8>(payload.at(18));

    qDebug() << "SuperSourceProperties: " << art_fill_source << art_cut_source;
    qDebug() << "-" << art_option << art_premultiplied << art_clip << art_gain << art_invert_key;

    m_ssart.fillSource=art_fill_source;
    m_ssart.cutSource=art_cut_source;
    m_ssart.option=art_option==0 ? QAtem::SuperSourceBackground : QAtem::SuperSourceForeground;
    m_ssart.clip=art_clip/1000.0;
    m_ssart.gain=art_gain/1000.0;
    m_ssart.premultiplied=art_premultiplied;
    m_ssart.invertkey=art_invert_key;

    emit superSourcePropertiesChanged();
}

quint8 QAtemSuperSource::superSourceID() const
{
    return m_ssid;
}

void QAtemSuperSource::setSuperSourceID(quint8 newSuperSourceID)
{
    if (m_ssid == newSuperSourceID)
        return;
    m_ssid = newSuperSourceID;
    emit superSourceIDChanged();
}

QAtemSuperSourceBox *QAtemSuperSource::getSuperSourceBox(quint8 boxid)
{
    if (boxid>m_boxes.size()) {
        qWarning("Invalid SuperSource Box id");
        return nullptr;
    }
    return m_boxes.at(boxid);
}
