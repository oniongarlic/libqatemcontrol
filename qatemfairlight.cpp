
#include "qatemfairlight.h"
#include "qatemconnection.h"

QAtemFairlight::QAtemFairlight(QObject *parent) :
    QAtemSubsystemBase(parent)
{
    m_commands << "FDLv" << "FMLv" << "FAAI" << "FAIP" << "FASP" << "FAMP" << "FMTl";
    m_atemConnection=nullptr;
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

    sendCommand(cmd, payload);
}

void QAtemFairlight::resetPeakLevels(bool all, bool master)
{
    QByteArray cmd("RFLP");
    QByteArray payload(4, 0x0);

    payload[0] = static_cast<char>(all | master << 1) ;

    sendCommand(cmd, payload);
}

qint16 QAtemFairlight::getFairlightInputCount()
{
    return m_inputs.count();
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

    qint16 ill=QAtem::int16at(payload, 16);
    qint16 irl=QAtem::int16at(payload, 18);
    qint16 ilp=QAtem::int16at(payload, 20);
    qint16 irp=QAtem::int16at(payload, 22);

    qint16 egr=QAtem::int16at(payload, 24);
    qint16 cgr=QAtem::int16at(payload, 26);
    qint16 lgr=QAtem::int16at(payload, 28);

    qint16 oll=QAtem::int16at(payload, 30);
    qint16 orl=QAtem::int16at(payload, 32);
    qint16 orp=QAtem::int16at(payload, 34);
    qint16 olp=QAtem::int16at(payload, 36);

    qint16 ll=QAtem::int16at(payload, 38);
    qint16 rl=QAtem::int16at(payload, 40);
    qint16 rp=QAtem::int16at(payload, 42);
    qint16 lp=QAtem::int16at(payload, 44);

    // qDebug() << "L: " << s << as << ill << irl << oll << orl;

    emit audioLevelChanged(as, ll, rl, rp, lp);
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

    //qDebug() << "Master:" << moll << morl << molp << morp;

    emit masterAudioLevelChanged(mll, mrl, mrp, mlp);
}

void QAtemFairlight::onFAAI(const QByteArray &payload)
{
    qDebug() << "FAAI: " << payload;

    quint16 s=QAtem::uint16at(payload, 6);
    quint8 sil=static_cast<qint8>(payload.at(8));
    quint8 ail=static_cast<qint8>(payload.at(8));

    qDebug() << "FAIP: " << s << sil << ail;
}

void QAtemFairlight::onFAIP(const QByteArray &payload)
{
    qDebug() << "FAIP: " << payload;

    quint16 s=QAtem::uint16at(payload, 6);
    quint8 it=static_cast<qint8>(payload.at(8));

    qDebug() << "FAIP: " << s << it;
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

        //qDebug() << "AT" << i << as << state;

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
