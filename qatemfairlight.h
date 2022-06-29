#ifndef QATEMFAIRLIGHT_H
#define QATEMFAIRLIGHT_H

#include <QObject>
#include "libqatemcontrol_global.h"
#include "qatemconnection.h"

class QAtemConnection;

class LIBQATEMCONTROLSHARED_EXPORT QAtemFairlight : public QObject
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

private:
    QAtemConnection *m_atemConnection;

signals:
    void atemConnectionChanged();
};

#endif // QATEMFAIRLIGHT_H
