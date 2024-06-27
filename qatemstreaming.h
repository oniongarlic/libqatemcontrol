#ifndef QATEMSTREAMING_H
#define QATEMSTREAMING_H

#include <QObject>

#include "qatemsubsystembase.h"

class QAtemStreaming : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(quint32 streamingDatarate READ getStreamingDatarate NOTIFY streamingDatarateChanged)
    Q_PROPERTY(quint16 streamingCache READ getStreamingCache NOTIFY streamingCacheChanged)
    Q_PROPERTY(QTime streamingTime READ getStreamingTime NOTIFY streamingTimeChanged)

public:
    explicit QAtemStreaming(QObject *parent = nullptr);
    Q_INVOKABLE quint32 getStreamingDatarate() { return m_streaming_datarate; }
    Q_INVOKABLE quint16 getStreamingCache() { return m_streaming_cache; }
    Q_INVOKABLE QTime getStreamingTime() const { return m_stream_time; }

public slots:
    void stream(bool stream);
    void startStreaming();
    void stopStreaming();
    void requestStreamingStatus();
    void setLowLatencyStreaming(bool low);

protected:

signals:
    void streamingDatarateChanged(quint32 datarate);
    void streamingCacheChanged(quint16 cache);
    void streamingTimeChanged(QTime time);

protected slots:
    void onSRSS(const QByteArray &payload);
    void onSRST(const QByteArray &payload);
    void onStRS(const QByteArray &payload);
    void onSLow(const QByteArray &payload);

private:
    quint32 m_streaming_datarate=0;
    quint16 m_streaming_cache=0;

    bool m_stream_framedrop=false;
    QTime m_stream_time;

    bool m_low_latency=false;
};

#endif // QATEMSTREAMING_H
