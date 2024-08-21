#ifndef QATEMSTREAMING_H
#define QATEMSTREAMING_H

#include <QObject>

#include "qatemsubsystembase.h"

class LIBQATEMCONTROLSHARED_EXPORT QAtemStreaming : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(bool isSupported READ isSupported NOTIFY isSupportedChanged FINAL)
    Q_PROPERTY(quint32 streamingDatarate READ getStreamingDatarate NOTIFY streamingDatarateChanged)
    Q_PROPERTY(quint16 streamingCache READ getStreamingCache NOTIFY streamingCacheChanged)
    Q_PROPERTY(QTime streamingTime READ getStreamingTime NOTIFY streamingTimeChanged)

public:
    explicit QAtemStreaming(QObject *parent = nullptr);
    Q_INVOKABLE quint32 getStreamingDatarate() { return m_streaming_datarate; }
    Q_INVOKABLE quint16 getStreamingCache() { return m_streaming_cache; }
    Q_INVOKABLE QTime getStreamingTime() const { return m_stream_time; }

    Q_INVOKABLE QString getName() const { return m_name; }
    Q_INVOKABLE QString getUrl() const { return m_url; }
    Q_INVOKABLE QString getKey() const { return m_key; }

    Q_INVOKABLE QString getUsername() const { return m_username; }
    Q_INVOKABLE QString getPassword() const { return m_password; }

    bool isSupported() const
    {
        return m_is_supported;
    }

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
    void streamingServiceUpdated(QString name, QString url, QString key);
    void streamingAuthenticatonUpdated(QString username, QString password);

    void isSupportedChanged();

protected slots:
    void onSRSS(const QByteArray &payload);
    void onSRST(const QByteArray &payload);
    void onStRS(const QByteArray &payload);
    void onSLow(const QByteArray &payload);

    void onSAth(const QByteArray &payload);
    void onSTAB(const QByteArray &payload);
    void onSRSU(const QByteArray &payload);
private:
    bool m_is_supported;
    quint32 m_streaming_datarate=0;
    quint16 m_streaming_cache=0;

    bool m_stream_framedrop=false;
    QTime m_stream_time;

    bool m_low_latency=false;

    QString m_name;
    QString m_url;
    QString m_key;

    QString m_username;
    QString m_password;

    QVector<quint32> m_vbitrates;
    QVector<quint32> m_abitrates;    
};

#endif // QATEMSTREAMING_H
