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
    qDeleteAll(m_superSourceBoxes);
    m_superSourceBoxes.resize(4);

    for(quint8 i = 0; i < m_superSourceBoxes.count(); i++) {
        m_superSourceBoxes[i] = new QSuperSourceBoxSettings();
    }
}

void QAtemSuperSource::setSuperSource(quint8 boxid, bool enabled, quint8 source, QPoint pos, double size, bool crop_enabled, QRect crop)
{
    QByteArray cmd("SSBP");
    QByteArray payload(20, 0x0);
    QAtem::U16_U8 v1;
    QAtem::S16_S8 v2;

    payload[0] = static_cast<char>(m_superSourceID);
    payload[1] = static_cast<char>(boxid);
    payload[2] = static_cast<char>(enabled);

    v1.u16 = source;
    payload[3] = static_cast<char>(v1.u8[1]);
    payload[4] = static_cast<char>(v1.u8[0]);

    v2.s16 = pos.x();
    payload[5] = static_cast<char>(v2.u8[1]);
    payload[6] = static_cast<char>(v2.u8[0]);

    v2.s16 = pos.y();
    payload[7] = static_cast<char>(v2.u8[1]);
    payload[8] = static_cast<char>(v2.u8[0]);

    v1.u16 = (int16_t)size*1000.0;
    payload[9] = static_cast<char>(v1.u8[1]);
    payload[10] = static_cast<char>(v1.u8[0]);

    payload[11] = static_cast<char>(crop_enabled);

    v1.u16 = crop.top();
    payload[12] = static_cast<char>(v1.u8[1]);
    payload[13] = static_cast<char>(v1.u8[0]);

    v1.u16 = crop.bottom();
    payload[14] = static_cast<char>(v1.u8[1]);
    payload[15] = static_cast<char>(v1.u8[0]);

    v1.u16 = crop.left();
    payload[16] = static_cast<char>(v1.u8[1]);
    payload[17] = static_cast<char>(v1.u8[0]);

    v1.u16 = crop.right();
    payload[18] = static_cast<char>(v1.u8[1]);
    payload[19] = static_cast<char>(v1.u8[0]);

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

    m_superSourceBoxes[ssboxid]->m_enabled=enabled;
    m_superSourceBoxes[ssboxid]->m_source=source;
    m_superSourceBoxes[ssboxid]->m_position.setX(posx);
    m_superSourceBoxes[ssboxid]->m_position.setY(posy);
    m_superSourceBoxes[ssboxid]->m_size=size;

    m_superSourceBoxes[ssboxid]->m_crop_enabled=crop;
    m_superSourceBoxes[ssboxid]->m_crop.setTop(cropTop);
    m_superSourceBoxes[ssboxid]->m_crop.setBottom(cropBottom);
    m_superSourceBoxes[ssboxid]->m_crop.setLeft(cropLeft);
    m_superSourceBoxes[ssboxid]->m_crop.setRight(cropRight);

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

    quint16 art_fill_source=QAtem::uint16at(payload, 7);
    quint16 art_cut_source=QAtem::uint16at(payload, 9);

    quint8 art_option=static_cast<qint8>(payload.at(11));
    bool art_premultiplied=static_cast<qint8>(payload.at(12));

    quint16 art_clip=QAtem::uint16at(payload, 13);
    quint16 art_gain=QAtem::uint16at(payload, 15);
    bool art_invert_key=static_cast<qint8>(payload.at(17));

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
