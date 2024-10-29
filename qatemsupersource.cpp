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
        box.m_border_enabled=false;
        box.m_border_color.setRgbF(0,0,0);
        box.m_width_inner=0;
        box.m_width_outer=0;
    }
}

void QAtemSuperSource::updateSuperSource(quint8 boxid) {
    if (boxid>m_superSourceBoxes.size()) {
        qWarning("Invalid SuperSource Box id");
        return;
    }
    setSuperSource(boxid,
                   m_superSourceBoxes[boxid].m_enabled,
                   m_superSourceBoxes[boxid].m_source,
                   m_superSourceBoxes[boxid].m_position,
                   m_superSourceBoxes[boxid].m_size,
                   m_superSourceBoxes[boxid].m_crop_enabled,
                   m_superSourceBoxes[boxid].m_crop);
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
    payload[1]=m_superSourceID;

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

    // qDebug() << pos << size << crop << payload.toHex(':');

    sendCommand(cmd, payload);
}

void QAtemSuperSource::setBorder(quint8 boxid, bool enabled)
{
    QByteArray cmd("CSSB");
    QByteArray payload(24, 0x0);

    payload[1] = 0x01;
    payload[2] = static_cast<char>(m_superSourceID);
    payload[3] = static_cast<char>(boxid);
    payload[4] = static_cast<char>(enabled);

    qDebug() << "CSSB: " << payload.toHex(':');

    sendCommand(cmd, payload);
}

void QAtemSuperSource::setBorderColor(quint8 boxid, QColor rgb)
{
    QByteArray cmd("CSSB");
    QByteArray pll(24, 0x0);
    QByteArray pls(24, 0x0);
    QByteArray plh(24, 0x0);
    QAtem::U16_U8 v1;
    float h,s,l;

    QColor hsl=rgb.toHsl();
    hsl.getHslF(&h, &s, &l);

    qDebug() << "CSSB: " << boxid << h*3600 << s*1000 << l*1000;

    //
    plh[0] = 0x00;
    plh[1] = 0x80;
    plh[2] = static_cast<char>(m_superSourceID);
    plh[3] = static_cast<char>(boxid);
    v1.u16 = h*3600;
    plh[18] = static_cast<char>(v1.u8[1]);
    plh[19] = static_cast<char>(v1.u8[0]);

    pls[0] = 0x01;
    pls[1] = 0x00;
    pls[2] = static_cast<char>(m_superSourceID);
    pls[3] = static_cast<char>(boxid);
    v1.u16 = s*1000;
    pls[20] = static_cast<char>(v1.u8[1]);
    pls[21] = static_cast<char>(v1.u8[0]);

    pll[0] = 0x02;
    pll[1] = 0x00;
    pll[2] = static_cast<char>(m_superSourceID);
    pll[3] = static_cast<char>(boxid);
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

    m_ssart.fillSource=art_fill_source;
    m_ssart.cutSource=art_cut_source;
    m_ssart.option=art_option==0 ? QAtem::SuperSourceBackground : QAtem::SuperSourceForeground;
    m_ssart.clip=art_clip/1000.0;
    m_ssart.gain=art_gain/1000.0;
    m_ssart.premultiplied=art_premultiplied;
    m_ssart.invertkey=art_invert_key;

    emit superSourcePropertiesChanged();
}

void QAtemSuperSource::onSSBd(const QByteArray &payload)
{
    qDebug() << "SSBd: " << payload.toHex(':');
}

void QAtemSuperSource::onSSSB(const QByteArray &payload)
{
    qDebug() << "SSSB: " << payload.toHex(':');

    quint8 ssid=static_cast<qint8>(payload.at(6));
    quint8 ssboxid=static_cast<qint8>(payload.at(7));

    if (ssboxid>3)
        return;

    QAtem::SuperSourceBoxSettings box=m_superSourceBoxes[ssboxid];

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

    box.m_border_enabled=enabled;
    box.m_border_color.setHslF(ch/3600.0, cs/1000.0, cl/1000.0);
    box.m_width_inner=iw1;
    box.m_width_outer=ow1;

    qDebug() << ssid << ssboxid << enabled << ow1 << ow2 << iw1 << iw2 << iw3 << iw4 << ch << cs << cl << box.m_border_color.toRgb();
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
    if (boxid>m_superSourceBoxes.size()) {
        qWarning("Invalid SuperSource Box id");
        boxid=3;
    }
    return m_superSourceBoxes.at(boxid);
}
