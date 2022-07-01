
#include "qatemfairlight.h"
#include "qatemconnection.h"

QAtemFairlight::QAtemFairlight(QObject *parent) : QObject(parent)
{
    m_commands << "FDLv" << "FMLv" << "FAAI" << "FAIP" << "FASP" << "FAMP" << "FMTl";
}

QAtemFairlight::~QAtemFairlight()
{

}

void QAtemFairlight::setAtemConnection(QAtemConnection *qac)
{
    if (m_atemConnection) {        
        for (const auto &i : qAsConst(m_commands)) {
            m_atemConnection->unregisterCommand(i, this);
        }
    }

    m_atemConnection=qac;

    for (const auto &i : qAsConst(m_commands)) {
        m_atemConnection->registerCommand(i, this, "on"+i);
    }

    emit atemConnectionChanged();
}

/**
 * @brief QAtemFairlight::setAudioLevelsEnabled
 * @param enabled
 *
 * SFLN: Request to send audio level information
 *
 */
void QAtemFairlight::setAudioLevelsEnabled(bool enabled)
{
    QByteArray cmd("SFLN");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(enabled);

    m_atemConnection->sendCommand(cmd, payload);
}

void QAtemFairlight::resetPeakLevels(bool all, bool master)
{
    QByteArray cmd("RFLP");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(all | master << 1) ;

    m_atemConnection->sendCommand(cmd, payload);
}

/**
 * @brief QAtemFairlight::onFMLv
 * @param payload
 *
 * FMLv: Audio level information, input/output/peaks
 *
 */
void QAtemFairlight::onFMLv(const QByteArray &payload)
{
    // qDebug() << "FMLv: " << payload;

    qint64 s=QAtem::int64at(payload, 6);
    quint16 as=QAtem::uint16at(payload, 14);

    quint16 ill=QAtem::uint16at(payload, 16);
    quint16 irl=QAtem::uint16at(payload, 18);
    quint16 ilp=QAtem::uint16at(payload, 20);
    quint16 irp=QAtem::uint16at(payload, 22);

    quint16 oll=QAtem::uint16at(payload, 30);
    quint16 orl=QAtem::uint16at(payload, 32);
    quint16 orp=QAtem::uint16at(payload, 34);
    quint16 olp=QAtem::uint16at(payload, 36);

    qDebug() << "L: " << s << as << ill << irl << oll << orl;

    emit audioLevelChanged(as, oll, orl, orp, olp);
}

void QAtemFairlight::onFDLv(const QByteArray &payload)
{
    //qDebug() << "FDLv: " << payload;

    qint16 mill=QAtem::int16at(payload, 6);
    qint16 mirl=QAtem::int16at(payload, 8);
    qint16 milp=QAtem::int16at(payload, 10);
    qint16 mirp=QAtem::int16at(payload, 12);

    qint16 cgr=QAtem::int16at(payload, 14);
    qint16 lgr=QAtem::int16at(payload, 16);

    qint16 moll=QAtem::int16at(payload, 18);
    qint16 morl=QAtem::int16at(payload, 20);
    qint16 molp=QAtem::int16at(payload, 22);
    qint16 morp=QAtem::int16at(payload, 24);

    qint16 mll=QAtem::int16at(payload, 26);
    qint16 mrl=QAtem::int16at(payload, 28);
    qint16 mlp=QAtem::int16at(payload, 30);
    qint16 mrp=QAtem::int16at(payload, 32);

    qDebug() << "Master:" << moll << morl << molp << morp;
}

void QAtemFairlight::onFAAI(const QByteArray &payload)
{
    qDebug() << "FAAI: " << payload;
}

void QAtemFairlight::onFAIP(const QByteArray &payload)
{
    qDebug() << "FAIP: " << payload;
}

/**
 * @brief QAtemFairlight::onFAMP
 * @param payload
 *
 * FAMP: Fairlight master audio details
 *
 */
void QAtemFairlight::onFAMP(const QByteArray &payload)
{
    // qDebug() << "FAMP: " << payload;

    qint8 eqs=static_cast<qint8>(payload.at(6));
    bool enabled=static_cast<bool>(payload.at(7));
    bool fftb=static_cast<bool>(payload.at(22));

    qDebug() << "EQ: " << eqs << enabled << fftb;
}

/**
 * @brief QAtemFairlight::onFMTl
 * @param payload
 *
 * FMTl: Fairlight Audio Tally
 * 0 = off
 * 1 = on
 * 2 = afv
 *
 */
void QAtemFairlight::onFMTl(const QByteArray &payload)
{
    quint16 cnt=QAtem::uint16at(payload, 6);
    for (quint16 i=0; i<cnt; i++) {
        quint16 as=QAtem ::uint16at(payload, 22+i*11);
        qint8 state=static_cast<qint8>(payload.at(24+i*11));

        qDebug() << "AT" << i << as << state;

        if (m_inputs[as].state!=state) {
            m_inputs[as].state=state;

            emit tallyChanged(as, state);
        }
    }
}

/**
 * @brief QAtemFairlight::onFASP
 * @param payload
 *
 * FASP: Fairlight Audio sources
 *
 */
void QAtemFairlight::onFASP(const QByteArray &payload)
{
    // qDebug() << "FASP: " << payload;

    quint16 as=QAtem::uint16at(payload, 6);
    qint64 s=QAtem::int64at(payload, 14);
    qint8 st=static_cast<qint8>(payload.at(22));
    qint8 mfd=static_cast<qint8>(payload.at(23));
    qint8 fd=static_cast<qint8>(payload.at(24));

    qint8 bands=static_cast<qint8>(payload.at(34));
    bool eq=static_cast<bool>(payload.at(35));

    qDebug() << "AudioSource: " << as << s << st << mfd << fd << eq << bands;

    m_inputs[as].index=as;
    m_inputs[as].fairlight=true;
    m_inputs[as].eq=eq;
    m_inputs[as].eq_bands=bands;
}
