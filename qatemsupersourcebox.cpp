/*
Copyright 2024  Kaj-Michael Lang <milang@tal.org>

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

#include "qatemsupersourcebox.h"

QAtemSuperSourceBox::QAtemSuperSourceBox(quint8 ss, quint8 box, QAtemConnection *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SSBP" << "SSCs" << "SSBd" << "SSSB";
    m_atemConnection=parent;

    // xxx
    m_ssid=ss;
    m_id=box;

    m_box.enabled=false;
    m_box.source=0;
    m_box.position.setX(0);
    m_box.position.setY(0);
    m_box.size=500;
    m_box.crop_enabled=false;
    m_box.crop.setX(0);
    m_box.crop.setY(0);
    m_box.crop.setZ(0);
    m_box.crop.setW(0);
    m_box.border_enabled=false;
    m_box.border_color.setRgbF(0,0,0);
    m_box.width_inner=0;
    m_box.width_outer=0;
}

QAtemSuperSourceBox::~QAtemSuperSourceBox()
{

}

void QAtemSuperSourceBox::setBox(bool enabled, uint source, QPoint pos, uint size, bool crop_enabled, QVector4D crop)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;
    QAtem::S16_S8 v2;

    v1.u16 = 1+2+4+8+16+32+64+128+256+512;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);
    payload[4] = static_cast<char>(enabled);

    v1.u16 = source;
    payload[6] = static_cast<char>(v1.u8[1]);
    payload[7] = static_cast<char>(v1.u8[0]);

    v2.s16 = pos.x();
    payload[8] = static_cast<char>(v2.u8[1]);
    payload[9] = static_cast<char>(v2.u8[0]);

    v2.s16 = pos.y();
    payload[10] = static_cast<char>(v2.u8[1]);
    payload[11] = static_cast<char>(v2.u8[0]);

    v1.u16 = size;
    payload[12] = static_cast<char>(v1.u8[1]);
    payload[13] = static_cast<char>(v1.u8[0]);

    payload[14] = static_cast<char>(crop_enabled);

    qDebug() << "setBox -- Crop" << crop.x() << crop.y() << crop.z() << crop.w();

    // Top
    v1.u16 = crop.x();
    payload[16] = static_cast<char>(v1.u8[1]);
    payload[17] = static_cast<char>(v1.u8[0]);

    // Bottom
    v1.u16 = crop.y();
    payload[18] = static_cast<char>(v1.u8[1]);
    payload[19] = static_cast<char>(v1.u8[0]);

    // Left
    v1.u16 = crop.z();
    payload[20] = static_cast<char>(v1.u8[1]);
    payload[21] = static_cast<char>(v1.u8[0]);

    // Right
    v1.u16 = crop.w();
    payload[22] = static_cast<char>(v1.u8[1]);
    payload[23] = static_cast<char>(v1.u8[0]);

    // qDebug() << pos << size << crop << payload.toHex(':');

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setPosition(QPoint pos, uint size)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;
    QAtem::S16_S8 v2;

    v1.u16 = QAtem::SuperBoxPositionX | QAtem::SuperBoxPositionY;
    if (size<1000) {
        v1.u16 |= QAtem::SuperBoxSize;
    }
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);

    v2.s16 = pos.x();
    payload[8] = static_cast<char>(v2.u8[1]);
    payload[9] = static_cast<char>(v2.u8[0]);

    v2.s16 = pos.y();
    payload[10] = static_cast<char>(v2.u8[1]);
    payload[11] = static_cast<char>(v2.u8[0]);

    v1.u16 = size;
    payload[12] = static_cast<char>(v1.u8[1]);
    payload[13] = static_cast<char>(v1.u8[0]);

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setOnAir(bool enabled)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;

    v1.u16 = QAtem::SuperBoxEnabled;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);
    payload[4] = static_cast<char>(enabled);

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setSource(quint8 source)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;

    v1.u16 = QAtem::SuperBoxSource;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);

    v1.u16 = source;
    payload[6] = static_cast<char>(v1.u8[1]);
    payload[7] = static_cast<char>(v1.u8[0]);

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setCropEnabled(bool enabled)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;

    v1.u16 = QAtem::SuperBoxCropped;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);

    payload[14] = static_cast<char>(enabled);

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setCrop(QVector4D crop)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;

    v1.u16 = QAtem::SuperBoxCropBottom | QAtem::SuperBoxCropTop | QAtem::SuperBoxCropLeft | QAtem::SuperBoxCropRight;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);

    qDebug() << "setCrop" << crop.x() << crop.y() << crop.z() << crop.w();

    // Top
    v1.u16 = crop.x();
    payload[16] = static_cast<char>(v1.u8[1]);
    payload[17] = static_cast<char>(v1.u8[0]);

    // Bottom
    v1.u16 = crop.y();
    payload[18] = static_cast<char>(v1.u8[1]);
    payload[19] = static_cast<char>(v1.u8[0]);

    // Left
    v1.u16 = crop.z();
    payload[20] = static_cast<char>(v1.u8[1]);
    payload[21] = static_cast<char>(v1.u8[0]);

    // Right
    v1.u16 = crop.w();
    payload[22] = static_cast<char>(v1.u8[1]);
    payload[23] = static_cast<char>(v1.u8[0]);

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setBorder(bool enabled)
{
    QByteArray cmd("CSSB");
    QByteArray payload(24, 0x0);

    payload[1] = 0x01;
    payload[2] = static_cast<char>(m_ssid);
    payload[3] = static_cast<char>(m_id);
    payload[4] = static_cast<char>(enabled);

    qDebug() << "CSSB: " << payload.toHex(':');

    sendCommand(cmd, payload);
}

void QAtemSuperSourceBox::setBorderColor(QColor rgb)
{
    QByteArray cmd("CSSB");
    QByteArray pll(24, 0x0);
    QByteArray pls(24, 0x0);
    QByteArray plh(24, 0x0);
    QAtem::U16_U8 v1;
    float h,s,l;

    QColor hsl=rgb.toHsl();
    hsl.getHslF(&h, &s, &l);

    qDebug() << "CSSB: " << m_id << h*3600 << s*1000 << l*1000;

    //
    plh[0] = 0x00;
    plh[1] = 0x80;
    plh[2] = static_cast<char>(m_ssid);
    plh[3] = static_cast<char>(m_id);
    v1.u16 = h*3600;
    plh[18] = static_cast<char>(v1.u8[1]);
    plh[19] = static_cast<char>(v1.u8[0]);

    pls[0] = 0x01;
    pls[1] = 0x00;
    pls[2] = static_cast<char>(m_ssid);
    pls[3] = static_cast<char>(m_id);
    v1.u16 = s*1000;
    pls[20] = static_cast<char>(v1.u8[1]);
    pls[21] = static_cast<char>(v1.u8[0]);

    pll[0] = 0x02;
    pll[1] = 0x00;
    pll[2] = static_cast<char>(m_ssid);
    pll[3] = static_cast<char>(m_id);
    v1.u16 = l*1000;
    pll[22] = static_cast<char>(v1.u8[1]);
    pll[23] = static_cast<char>(v1.u8[0]);

    qDebug() << "CSSB: " << plh.toHex(':');
    qDebug() << "CSSB: " << pls.toHex(':');
    qDebug() << "CSSB: " << pll.toHex(':');

    sendCommand(cmd, plh);
    sendCommand(cmd, pls);
    sendCommand(cmd, pll);
}

void QAtemSuperSourceBox::onSSBP(const QByteArray &payload)
{
    QVector4D c;
    QPoint p;

    quint8 ssid=static_cast<qint8>(payload.at(6));
    quint8 ssboxid=static_cast<qint8>(payload.at(7));

    if (ssid!=m_ssid)
        return;

    if (ssboxid!=m_id)
        return;

    bool enabled=static_cast<qint8>(payload.at(8));

    quint16 source=QAtem::uint16at(payload, 10);

    // Position & Size x,y,s
    // 0-4800 -4800-0
    p.setX(QAtem::int16at(payload, 12));
    p.setY(QAtem::int16at(payload, 14));
    quint16 size=QAtem::uint16at(payload, 16); // 0-1000

    // Cropping
    bool crop=static_cast<qint8>(payload.at(18));
    // 0-18000
    c.setX(QAtem::uint16at(payload, 20));
    c.setY(QAtem::uint16at(payload, 22));
    c.setZ(QAtem::uint16at(payload, 24));
    c.setW(QAtem::uint16at(payload, 26));

    if (m_box.enabled!=enabled) {
        m_box.enabled=enabled;
        emit onAirChanged();
    }

    if (m_box.source!=source) {
        m_box.source=source;
        emit sourceChanged();
    }

    if (m_box.position!=p) {
        m_box.position=p;
        emit positionChanged();
    }

    if (m_box.size!=size) {
        m_box.size=size;
        emit sizeChanged();
    }

    if (m_box.crop!=c) {
        m_box.crop=c;
        emit cropRectChanged();
    }
    if (m_box.crop_enabled!=crop) {
        m_box.crop_enabled=crop;
        emit cropChanged();
    }

    emit boxPropertiesChanged();
}

void QAtemSuperSourceBox::onSSBd(const QByteArray &payload)
{
    qDebug() << "SSBd: " << payload.toHex(':');
}

void QAtemSuperSourceBox::onSSSB(const QByteArray &payload)
{
    QColor hsl;

    quint8 ssid=static_cast<qint8>(payload.at(6));
    quint8 ssboxid=static_cast<qint8>(payload.at(7));

    if (ssid!=m_ssid)
        return;

    if (ssboxid!=m_id)
        return;

    bool enabled=static_cast<qint8>(payload.at(8));

    quint16 ow1=QAtem::uint16at(payload, 10); // 0-65535
    quint16 ow2=QAtem::uint16at(payload, 12);

    quint16 iw1=QAtem::uint16at(payload, 14); // 0-65535
    quint16 iw2=QAtem::uint16at(payload, 16);
    quint16 iw3=QAtem::uint16at(payload, 18);
    quint16 iw4=QAtem::uint16at(payload, 20);

    quint16 ch=QAtem::uint16at(payload, 22); // 0-3600
    quint16 cs=QAtem::uint16at(payload, 24); // 0-1000
    quint16 cl=QAtem::uint16at(payload, 26); // 0-1000

    if (m_box.border_enabled!=enabled) {
        m_box.border_enabled=enabled;
        emit borderChanged();
    }

    hsl.setHslF(ch/3600.0, cs/1000.0, cl/1000.0);
    if (m_box.border_color!=hsl.toRgb()) {
        m_box.border_color=hsl.toRgb();
        emit borderColorChanged();
    }
    m_box.width_inner=iw1;
    m_box.width_outer=ow1;

    emit borderPropertiesChanged();
}

quint8 QAtemSuperSourceBox::superSource() const
{
    return m_ssid;
}

quint8 QAtemSuperSourceBox::box() const
{
    return m_id;
}

bool QAtemSuperSourceBox::boxOnAir() const
{
    return m_box.enabled;
}

bool QAtemSuperSourceBox::boxCrop() const
{
    return m_box.crop_enabled;
}

QVector4D QAtemSuperSourceBox::boxCropRect() const
{
    return m_box.crop;
}

uint QAtemSuperSourceBox::boxSource() const
{
    return m_box.source;
}

uint QAtemSuperSourceBox::boxSize() const
{
    return m_box.size;
}

QPoint QAtemSuperSourceBox::boxPosition() const
{
    return m_box.position;
}

bool QAtemSuperSourceBox::boxBorder() const
{
    return m_box.border_enabled;
}

QColor QAtemSuperSourceBox::boxBorderColor() const
{
    return m_box.border_color;
}
