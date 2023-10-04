#ifndef QATEMSUPERSOURCE_H
#define QATEMSUPERSOURCE_H

#include <QObject>
#include <QRect>
#include <QPoint>

#include "qatemsubsystembase.h"

class QAtemConnection;

class QAtemSuperSource : public QAtemSubsystemBase
{
    Q_OBJECT
public:
    explicit QAtemSuperSource(QObject *parent = nullptr);

public slots:
    void setSuperSource(quint8 boxid, bool enabled, quint8 source, QPoint pos, double size, bool crop_enabled, QRect crop);
protected slots:
    void onSSBP(const QByteArray &payload);
    void onSSCs(const QByteArray &payload);
    void onSSrc(const QByteArray &payload);
    void onSSBd(const QByteArray &payload);

private:
    quint8 m_superSourceID=1;
};

#endif // QATEMSUPERSOURCE_H
