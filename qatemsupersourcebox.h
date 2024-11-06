#ifndef QATEMSUPERSOURCEBOX_H
#define QATEMSUPERSOURCEBOX_H

#include <QObject>
#include "qatemsubsystembase.h"

class LIBQATEMCONTROLSHARED_EXPORT QAtemSuperSourceBox : public QAtemSubsystemBase
{
    Q_OBJECT
    Q_PROPERTY(uint superSource READ superSource CONSTANT FINAL)
    Q_PROPERTY(uint box READ box CONSTANT FINAL)

    Q_PROPERTY(bool onAir READ boxOnAir NOTIFY onAirChanged FINAL)
    Q_PROPERTY(bool crop READ boxCrop NOTIFY cropChanged FINAL)
    Q_PROPERTY(QRect cropRect READ boxCropRect NOTIFY cropRectChanged FINAL)
    Q_PROPERTY(uint source READ boxSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(QPoint position READ boxPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(uint size READ boxSize NOTIFY sizeChanged FINAL)

    Q_PROPERTY(bool border READ boxBorder NOTIFY borderChanged FINAL)
    Q_PROPERTY(QColor borderColor READ boxBorderColor NOTIFY borderColorChanged FINAL)

public:
    explicit QAtemSuperSourceBox(quint8 ss, quint8 box, QAtemConnection *parent);
    ~QAtemSuperSourceBox();

    Q_INVOKABLE void setBox(bool enabled, uint source, QPoint pos, uint size, bool crop_enabled, QRect crop);
    Q_INVOKABLE void setOnAir(bool enabled);
    Q_INVOKABLE void setSource(quint8 source);
    Q_INVOKABLE void setCropEnabled(bool enabled);

    Q_INVOKABLE void setBorder(bool enabled);
    Q_INVOKABLE void setBorderColor(QColor rgb);

    quint8 superSource() const;
    quint8 box() const;

    bool boxOnAir() const;
    bool boxCrop() const;
    QRect boxCropRect() const;
    uint boxSource() const;
    uint boxSize() const;
    QPoint boxPosition() const;

    bool boxBorder() const;
    QColor boxBorderColor() const;

signals:
    void onAirChanged();
    void cropChanged();
    void cropRectChanged();
    void sourceChanged();
    void sizeChanged();
    void positionChanged();
    void boxPropertiesChanged();

    void borderChanged();
    void borderColorChanged();

    void borderPropertiesChanged();

protected slots:
    void onSSSB(const QByteArray &payload);
    void onSSBd(const QByteArray &payload);
    void onSSBP(const QByteArray &payload);
private:
    quint8 m_ssid;
    quint8 m_id;
    QAtem::SuperSourceBoxSettings m_box;
};

#endif // QATEMSUPERSOURCEBOX_H
