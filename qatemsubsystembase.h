#ifndef QATEMSUBSYSTEMBASE_H
#define QATEMSUBSYSTEMBASE_H

#include <QObject>
#include "libqatemcontrol_global.h"
#include "qatemconnection.h"

class LIBQATEMCONTROLSHARED_EXPORT QAtemSubsystemBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAtemConnection* atemConnection READ getAtemConnection WRITE setAtemConnection NOTIFY atemConnectionChanged FINAL REQUIRED)

public:
    explicit QAtemSubsystemBase(QObject *parent = nullptr);
    ~QAtemSubsystemBase();

    void setAtemConnection(QAtemConnection *qac);    
    QAtemConnection *getAtemConnection() { return m_atemConnection; }

signals:
    void atemConnectionChanged();

protected:
    bool sendCommand(const QByteArray cmd, const QByteArray payload);
    QAtemConnection *m_atemConnection=nullptr;
    QList<QByteArray> m_commands;

};

#endif // QATEMSUBSYSTEMBASE_H
