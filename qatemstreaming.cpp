#include "qatemstreaming.h"

QAtemStreaming::QAtemStreaming(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "SRST" << "SRSS" << "StRS" << "SLow";
    m_atemConnection=nullptr;
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
