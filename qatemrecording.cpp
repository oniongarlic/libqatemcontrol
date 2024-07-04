#include "qatemrecording.h"

QAtemRecording::QAtemRecording(QObject *parent)
    : QAtemSubsystemBase{parent}
{
    m_commands << "RTMD" << "RTMS" << "RTMR";
    m_atemConnection=nullptr;
    m_is_supported=false;
}

/**
 * @brief QAtemConnection::onRTMD
 * @param payload
 *
 * Recording media information
 */
void QAtemRecording::onRTMD(const QByteArray& payload)
{


    QAtem::RecordingInfo r;
    r.disk=QAtem::uint32at(payload, 6);
    r.available=QAtem::uint32at(payload, 10);
    r.volumeName=payload.mid(16, 64);
    r.status=static_cast<QAtem::RecordingDiskStatus>(QAtem::uint16at(payload, 14));

    m_info.insert(r.disk, r);

    emit infoChanged(r);
}

/**
 * @brief QAtemConnection::onRTMS
 * @param payload
 */
void QAtemRecording::onRTMS(const QByteArray& payload)
{
    QAtem::U32_U8 val1, val2;
    val1.u8[3] = static_cast<quint8>(payload.at(6));
    val1.u8[2] = static_cast<quint8>(payload.at(7));
    val1.u8[1] = static_cast<quint8>(payload.at(8));
    val1.u8[0] = static_cast<quint8>(payload.at(9));

    val2.u8[3] = static_cast<quint8>(payload.at(10));
    val2.u8[2] = static_cast<quint8>(payload.at(11));
    val2.u8[1] = static_cast<quint8>(payload.at(12));
    val2.u8[0] = static_cast<quint8>(payload.at(13));

    qDebug() << "RTMS" << payload.size() << ":" << val1.u16 << val1.u16 << val2.u32;

    if (!m_is_supported) {
        m_is_supported=true;
        emit isSupportedChanged();
    }
}

/**
 * @brief QAtemConnection::onRTMR
 * @param payload
 */
void QAtemRecording::onRTMR(const QByteArray& payload)
{
    quint8 h,m,s,f;
    h = static_cast<quint8>(payload.at(6)); // h
    m = static_cast<quint8>(payload.at(7)); // m
    s = static_cast<quint8>(payload.at(8)); // s
    f = static_cast<quint8>(payload.at(9)); // frame

    m_record_framedrop = static_cast<bool>(payload.at(10));

    m_record_time.setHMS(h, m, s);
    m_record_frame=f;

    emit recordingTimeChanged(m_record_time, m_record_frame);

    qDebug() << "RTMR" << m_record_time << f << m_record_framedrop;
}

void QAtemRecording::record(bool record)
{
    QByteArray cmd("RcTM");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(record);

    sendCommand(cmd, payload);
}

void QAtemRecording::startRecording()
{
    record(true);
}

void QAtemRecording::stopRecording()
{
    record(false);
}

void QAtemRecording::requestRecordingStatus()
{
    QByteArray cmd("RMDR");
    QByteArray payload;

    sendCommand(cmd, payload);
}


