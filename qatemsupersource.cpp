#include "qatemsupersource.h"

QAtemSuperSource::QAtemSuperSource(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SSBP" << "SSrc" << "SSCs" << "SSBd";
    m_atemConnection=nullptr;

    createSuperSourceBoxes();
}

void QAtemSuperSource::createSuperSourceBoxes()
{
    m_superSourceBoxes.resize(4);
    for (int i=0;i<4;i++) {
        QAtem::SuperSourceBoxSettings box=m_superSourceBoxes[i];
        box.m_enabled=false;
        box.m_source=0;
        box.m_size=1;
        box.m_crop_enabled=false;
        box.m_crop.setX(0);
        box.m_crop.setY(0);
        box.m_crop.setWidth(0);
        box.m_crop.setHeight(0);
        qDebug() << box.m_enabled << box.m_size;
    }
}

void QAtemSuperSource::updateSuperSource(quint8 boxid) {
    setSuperSource(boxid,
                   m_superSourceBoxes[boxid].m_enabled,
                   m_superSourceBoxes[boxid].m_source,
                   m_superSourceBoxes[boxid].m_position,
                   m_superSourceBoxes[boxid].m_size,
                   m_superSourceBoxes[boxid].m_crop_enabled,
                   m_superSourceBoxes[boxid].m_crop);
}

void QAtemSuperSource::setSuperSource(quint8 boxid, bool enabled, quint8 source, QPoint pos, uint size, bool crop_enabled, QRect crop)
{
    QByteArray cmd("CSBP");
    QByteArray payload(24, 0x0);
    QAtem::U16_U8 v1;
    QAtem::S16_S8 v2;

    v1.u16 = 1+2+4+8+16+32+64+128+256+512;
    payload[0] = static_cast<char>(v1.u8[1]);
    payload[1] = static_cast<char>(v1.u8[0]);

    payload[2] = static_cast<char>(m_superSourceID);
    payload[3] = static_cast<char>(boxid);
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

    // Top
    v1.u16 = crop.y();
    payload[16] = static_cast<char>(v1.u8[1]);
    payload[17] = static_cast<char>(v1.u8[0]);

    // Bottom
    v1.u16 = crop.height();
    payload[18] = static_cast<char>(v1.u8[1]);
    payload[19] = static_cast<char>(v1.u8[0]);

    // Left
    v1.u16 = crop.x();
    payload[20] = static_cast<char>(v1.u8[1]);
    payload[21] = static_cast<char>(v1.u8[0]);

    // Right
    v1.u16 = crop.width();
    payload[22] = static_cast<char>(v1.u8[1]);
    payload[23] = static_cast<char>(v1.u8[0]);

    qDebug() << pos << size << crop << payload.toHex(':');

    sendCommand(cmd, payload);
}

void QAtemSuperSource::onSSBP(const QByteArray &payload)
{
    qDebug() << "SSBP: " << payload.toHex(':');

    quint8 ssid=static_cast<qint8>(payload.at(6));

    if (m_superSourceID!=ssid)
        return;

    quint8 ssboxid=static_cast<qint8>(payload.at(7));
    bool enabled=static_cast<qint8>(payload.at(8));

    quint16 source=QAtem::uint16at(payload, 10);

    qint16 posx=QAtem::int16at(payload, 12); // 0-4800 -4800-0
    qint16 posy=QAtem::int16at(payload, 14);
    quint16 size=QAtem::uint16at(payload, 16); // 0-1000

    bool crop=static_cast<qint8>(payload.at(18));
    quint16 cropTop=QAtem::uint16at(payload, 20); // 0-18000
    quint16 cropBottom=QAtem::uint16at(payload, 22);
    quint16 cropLeft=QAtem::uint16at(payload, 24);
    quint16 cropRight=QAtem::uint16at(payload, 26);

    qDebug() << "SuperSource: " << ssid << ssboxid << enabled << source;
    qDebug() << "-Pos (X Y S): " << posx << posy << size;
    qDebug() << "-Crop (T B L R)" << crop << cropTop << cropBottom << cropLeft << cropRight;

    m_superSourceBoxes[ssboxid].m_enabled=enabled;
    m_superSourceBoxes[ssboxid].m_source=source;
    m_superSourceBoxes[ssboxid].m_position.setX(posx);
    m_superSourceBoxes[ssboxid].m_position.setY(posy);
    m_superSourceBoxes[ssboxid].m_size=size;

    m_superSourceBoxes[ssboxid].m_crop_enabled=crop;
    m_superSourceBoxes[ssboxid].m_crop.setY(cropTop);
    m_superSourceBoxes[ssboxid].m_crop.setHeight(cropBottom);
    m_superSourceBoxes[ssboxid].m_crop.setX(cropLeft);
    m_superSourceBoxes[ssboxid].m_crop.setWidth(cropRight);

    emit superSourceChanged(ssboxid);
}

void QAtemSuperSource::onSSCs(const QByteArray &payload)
{
    qDebug() << "SSCs: " << payload.toHex(':');
}

void QAtemSuperSource::onSSrc(const QByteArray &payload)
{
    qDebug() << "SSrc: " << payload.toHex(':');

    quint8 ssid=static_cast<qint8>(payload.at(6));

    if (m_superSourceID!=ssid)
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

    emit superSourcePropertiesChanged();
}

void QAtemSuperSource::onSSBd(const QByteArray &payload)
{
    qDebug() << "SSBd: " << payload.toHex(':');
}

quint8 QAtemSuperSource::superSourceID() const
{
    return m_superSourceID;
}

void QAtemSuperSource::setSuperSourceID(quint8 newSuperSourceID)
{
    if (m_superSourceID == newSuperSourceID)
        return;
    m_superSourceID = newSuperSourceID;
    emit superSourceIDChanged();
}

QAtem::SuperSourceBoxSettings QAtemSuperSource::getSuperSourceBox(quint8 boxid)
{
    if (boxid>m_superSourceBoxes.size())
        boxid=3;
    return m_superSourceBoxes.at(boxid);
}
