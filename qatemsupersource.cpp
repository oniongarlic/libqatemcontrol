#include "qatemsupersource.h"

QAtemSuperSource::QAtemSuperSource(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SSBP" << "SSrc" << "SSCs" << "SSBd";
    m_atemConnection=nullptr;
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
    qDebug() << "SSBP: " << payload.toHex();

    quint8 ssid=static_cast<qint8>(payload.at(6));
    quint8 ssboxid=static_cast<qint8>(payload.at(7));
    bool enabled=static_cast<qint8>(payload.at(8));

    quint16 source=QAtem::uint16at(payload, 9);

    quint16 posx=QAtem::uint16at(payload, 11);
    quint16 posy=QAtem::uint16at(payload, 13);
    quint16 size=QAtem::uint16at(payload, 15);

    bool crop=static_cast<qint8>(payload.at(17));
    quint16 cropTop=QAtem::uint16at(payload, 18);
    quint16 cropBottom=QAtem::uint16at(payload, 20);
    quint16 cropLeft=QAtem::uint16at(payload, 22);
    quint16 cropRight=QAtem::uint16at(payload, 24);

    qDebug() << "SuperSource: " << ssid << ssboxid << enabled << source;
    qDebug() << "-Pos: " << posx << posy << size;
    qDebug() << "-Crop" << crop << cropTop << cropBottom << cropLeft << cropRight;
}

void QAtemSuperSource::onSSCs(const QByteArray &payload)
{
    qDebug() << "SSCs: " << payload.toHex();
}

void QAtemSuperSource::onSSrc(const QByteArray &payload)
{
    qDebug() << "SSrc: " << payload.toHex();
}

void QAtemSuperSource::onSSBd(const QByteArray &payload)
{
    qDebug() << "SSBd: " << payload.toHex();
}
