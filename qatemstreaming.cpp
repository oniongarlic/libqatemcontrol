#include "qatemstreaming.h"

QAtemStreaming::QAtemStreaming(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SRST" << "SRSS" << "StRS" << "SLow" << "SRSU" << "STAB" << "SAth";
    m_atemConnection=nullptr;
    m_vbitrates.resize(2);
    m_abitrates.resize(2);
}

void QAtemStreaming::stream(bool stream)
{
    QByteArray cmd("StrR");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(stream);

    sendCommand(cmd, payload);
}

void QAtemStreaming::startStreaming()
{
    stream(true);
}

void QAtemStreaming::stopStreaming()
{
    stream(false);
}

void QAtemStreaming::requestStreamingStatus()
{
    QByteArray cmd("SRDR");
    QByteArray payload;

    sendCommand(cmd, payload);
}

void QAtemStreaming::setLowLatencyStreaming(bool low)
{
    QByteArray cmd("SLow");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(low);

    sendCommand(cmd, payload);
}


void QAtemStreaming::onSRST(const QByteArray& payload)
{
    quint8 h,m,s,f;
    h = static_cast<quint8>(payload.at(6)); // h
    m = static_cast<quint8>(payload.at(7)); // m
    s = static_cast<quint8>(payload.at(8)); // s
    f = static_cast<quint8>(payload.at(9)); // frame

    m_stream_framedrop = static_cast<bool>(payload.at(10));

    m_stream_time.setHMS(h, m, s);
    emit streamingTimeChanged(m_stream_time);

    qDebug() << "SRST" << m_stream_time << f << m_stream_framedrop;
}

void QAtemStreaming::onSRSS(const QByteArray& payload)
{
    QAtem::U32_U8 val;
    val.u8[3] = static_cast<quint8>(payload.at(6));
    val.u8[2] = static_cast<quint8>(payload.at(7));
    val.u8[1] = static_cast<quint8>(payload.at(8));
    val.u8[0] = static_cast<quint8>(payload.at(9));

    m_streaming_datarate = val.u32;

    QAtem::U16_U8 val2;
    val2.u8[1] = static_cast<quint8>(payload.at(10));
    val2.u8[0] = static_cast<quint8>(payload.at(11));

    m_streaming_cache = val2.u16;

    qDebug() << "SRSS" << payload.size() << ":" << val.u32 << val2.u16;

    emit streamingDatarateChanged(m_streaming_datarate);
    emit streamingCacheChanged(m_streaming_cache);
}

void QAtemStreaming::onStRS(const QByteArray& payload)
{
    QAtem::U32_U8 val;
    val.u8[3] = static_cast<quint8>(payload.at(6));
    val.u8[2] = static_cast<quint8>(payload.at(7));
    val.u8[1] = static_cast<quint8>(payload.at(8));
    val.u8[0] = static_cast<quint8>(payload.at(9));

    qDebug() << "StRS" << payload.size() << ":" << val.u16[0] << val.u16[1];
}

void QAtemStreaming::onSLow(const QByteArray& payload)
{
    m_low_latency = QAtem::boolat(payload, 6);
}

/**
 * @brief QAtemSuperSource::onSRSU
 * @param payload
 *
 * Streaming service
 *
 */
void QAtemStreaming::onSRSU(const QByteArray &payload)
{
    m_name=QString(payload.mid(6, 64));
    m_url=QString(payload.mid(6+64, 512));
    m_key=QString(payload.mid(6+64+512, 512));

    m_vbitrates[0]=QAtem::uint32at(payload, 1024+64+6);
    m_vbitrates[1]=QAtem::uint32at(payload, 1024+64+6+4);

    qDebug() << "VideoBitrates:" << m_vbitrates;

    emit streamingServiceUpdated(m_name, m_url, m_key);
}

/**
 * @brief QAtemSuperSource::onSTAB
 * @param payload
 *
 * Streaming audio bitrate
 *
 */
void QAtemStreaming::onSTAB(const QByteArray &payload)
{
    qDebug() << "STAB: " << payload.toHex(':');

    m_abitrates[0]=QAtem::uint32at(payload, 6);
    m_abitrates[1]=QAtem::uint32at(payload, 6+4);

    qDebug() << "AudioBitrates:" << m_abitrates;
}

/**
 * @brief QAtemSuperSource::onSRSU
 * @param payload
 *
 * Streaming auth/key
 *
 */
void QAtemStreaming::onSAth(const QByteArray &payload)
{
    m_username=QString(payload.mid(6, 64));
    m_password=QString(payload.mid(6+64, 64));

    emit streamingAuthenticatonUpdated(m_username, m_password);
}
