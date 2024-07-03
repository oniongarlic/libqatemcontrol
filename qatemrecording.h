#ifndef QATEMRECORDING_H
#define QATEMRECORDING_H

#include <QObject>
#include <QTime>

#include "qatemsubsystembase.h"

class QAtemRecording : public QAtemSubsystemBase
{
    Q_OBJECT

    Q_PROPERTY(QTime recordingTime READ getRecordingTime NOTIFY recordingTimeChanged)

public:
    explicit QAtemRecording(QObject *parent = nullptr);

    Q_INVOKABLE QTime getRecordingTime() const { return m_record_time; }

public slots:
    void record(bool record);
    void startRecording();
    void stopRecording();
    void requestRecordingStatus();

signals:
    void recordingTimeChanged(QTime time);

protected slots:
    void onRTMR(const QByteArray &payload);
    void onRTMS(const QByteArray &payload);
    void onRTMD(const QByteArray &payload);
private:
    quint32 m_recording_datarate;
    bool m_record_framedrop;
    QTime m_record_time;
};

#endif // QATEMRECORDING_H
