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
    Q_PROPERTY(int inputCount READ getInputCount NOTIFY inputCountChanged FINAL)

public:
    explicit QAtemFairlight(QObject *parent=nullptr);

public slots:
    void setAudioLevelsEnabled(bool enabled);
    void resetPeakLevels(bool all, bool master);
    qint16 getInputCount();
    QList<quint16> inputSources();
    QAtem::AudioInput inputInfo(qint16 source);

protected slots:
    void onFMLv(const QByteArray &payload);
    void onFDLv(const QByteArray &payload);
    void onFAAI(const QByteArray &payload);
    void onFAIP(const QByteArray &payload);
    void onFASP(const QByteArray &payload);
    void onFAMP(const QByteArray &payload);
    void onFMTl(const QByteArray &payload);

    void onAICP(const QByteArray &payload);
    void onAIXP(const QByteArray &payload);
    void onAILP(const QByteArray &payload);
    void onAEBP(const QByteArray &payload);

signals:
    void audioLevelChanged(quint16 audioSource, qint16 levelLeft, qint16 levelRight, qint16 levelPeakLeft, qint16 levelPeakRight);
    void masterAudioLevelChanged(qint16 levelLeft, qint16 levelRight, qint16 levelPeakLeft, qint16 levelPeakRight);
    void tallyChanged(quint16 audioSource, qint8 state);
    void tallyUpdated();
    void audioSourceUpdated(quint16 audioSource);

    void inputCountChanged();

private:
    QMap<quint16, QAtem::AudioInput> m_inputs;
    QMap<qint16, QAtem::AudioLevel> m_input;
    QMap<qint16, QAtem::AudioLevel> m_output;
};

#endif // QATEMFAIRLIGHT_H
