#ifndef QATEMFAIRLIGHT_H
#define QATEMFAIRLIGHT_H

#include <QObject>
#include "libqatemcontrol_global.h"
#include "qatemconnection.h"
#include "qatemsubsystembase.h"

class QAtemConnection;

class LIBQATEMCONTROLSHARED_EXPORT QAtemFairlight : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(QAtemConnection* atemConnection READ getAtemConnection WRITE setAtemConnection NOTIFY atemConnectionChanged FINAL REQUIRED)

public:
    explicit QAtemFairlight(QObject *parent=nullptr);
    ~QAtemFairlight();

    void setAtemConnection(QAtemConnection *qac);

    QAtemConnection *getAtemConnection() { return m_atemConnection; }

public slots:
    void setAudioLevelsEnabled(bool enabled);
    void resetPeakLevels(bool all, bool master);

protected slots:
    void onFMLv(const QByteArray &payload);
    void onFDLv(const QByteArray &payload);
    void onFAAI(const QByteArray &payload);
    void onFAIP(const QByteArray &payload);
    void onFASP(const QByteArray &payload);
    void onFAMP(const QByteArray &payload);
    void onFMTl(const QByteArray &payload);

signals:
    void audioLevelChanged(quint16 audioSource, quint16 levelLeft, quint16 levelRight, quint16 levelPeakLeft, quint16 levelPeakRight);
    void masterAudioLevelChanged(quint16 levelLeft, quint16 levelRight, quint16 levelPeakLeft, quint16 levelPeakRight);
    void tallyChanged(quint16 audioSource, qint8 state);

private:
    bool sendCommand(const QByteArray cmd, const QByteArray payload);
    QAtemConnection *m_atemConnection;
    QList<QByteArray> m_commands;
    QMap<quint16, QAtem::AudioInput> m_inputs;
    QMap<qint16, QAtem::AudioLevel> m_input;
    QMap<qint16, QAtem::AudioLevel> m_output;

signals:
    void atemConnectionChanged();
};

#endif // QATEMFAIRLIGHT_H
