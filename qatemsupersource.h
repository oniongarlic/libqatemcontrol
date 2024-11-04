#ifndef QATEMSUPERSOURCE_H
#define QATEMSUPERSOURCE_H

#include <QObject>
#include <QRect>
#include <QPoint>

#include "qatemsubsystembase.h"
#include "qatemsupersourcebox.h"

class QAtemConnection;

class LIBQATEMCONTROLSHARED_EXPORT QAtemSuperSource : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(uint superSourceID READ superSourceID WRITE setSuperSourceID NOTIFY superSourceIDChanged REQUIRED FINAL)
public:
    explicit QAtemSuperSource(QObject *parent = nullptr);
    ~QAtemSuperSource();

    quint8 superSourceID() const;
    void setSuperSourceID(quint8 newSuperSourceID);

    Q_INVOKABLE QList<QAtemSuperSourceBox*> getSuperSourceBoxes() {
        return m_boxes;
    };

    Q_INVOKABLE QAtemSuperSourceBox *getSuperSourceBox(quint8 boxid);
    Q_INVOKABLE QAtem::SuperSourceArt getSuperSourceProperties();    
    Q_INVOKABLE void updateSuperSourceProperties();
    Q_INVOKABLE void setSuperSource(quint8 boxid, bool enabled, uint source, QPoint pos, uint size, bool crop_enabled, QRect crop);

    void setAtemConnection(QAtemConnection *qac);
signals:
    void superSourceIDChanged();
    void superSourceChanged(quint8 boxid);
    void superSourcePropertiesChanged();

protected:
    void createSuperSourceBoxes();
protected slots:
    void onSSrc(const QByteArray &payload);    

private:
    quint8 m_ssid=0;
    QAtem::SuperSourceArt m_ssart;

    QList<QAtemSuperSourceBox*> m_boxes;
};

#endif // QATEMSUPERSOURCE_H
