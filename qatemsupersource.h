#ifndef QATEMSUPERSOURCE_H
#define QATEMSUPERSOURCE_H

#include <QObject>
#include <QRect>
#include <QPoint>

#include "qatemsubsystembase.h"

class QAtemConnection;

struct LIBQATEMCONTROLSHARED_EXPORT QSuperSourceBoxSettings
{
    QSuperSourceBoxSettings()
    {
        m_enabled = false;
        m_source = 0;
    }

    Q_GADGET

public:
    bool m_enabled;
    quint8 m_source;
    QPoint m_position;
    double m_size;
    bool m_crop_enabled;
    QRect m_crop;
};

class QAtemSuperSource : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(quint8 superSourceID READ superSourceID WRITE setSuperSourceID NOTIFY superSourceIDChanged FINAL)
    friend struct QSuperSourceBoxSettings;
public:
    explicit QAtemSuperSource(QObject *parent = nullptr);

    quint8 superSourceID() const;
    void setSuperSourceID(quint8 newSuperSourceID);

public slots:
    void setSuperSource(quint8 boxid, bool enabled, quint8 source, QPoint pos, uint size, bool crop_enabled, QRect crop);

signals:
    void superSourceChanged(quint8 boxid);
    void superSourceIDChanged();
    void superSourcePropertiesChanged();

protected:
    void createSuperSourceBoxes();
protected slots:
    void onSSBP(const QByteArray &payload);
    void onSSCs(const QByteArray &payload);
    void onSSrc(const QByteArray &payload);
    void onSSBd(const QByteArray &payload);

private:
    quint8 m_superSourceID=0;
    QVector<QSuperSourceBoxSettings *> m_superSourceBoxes;
};

#endif // QATEMSUPERSOURCE_H
