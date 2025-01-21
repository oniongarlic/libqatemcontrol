/*
Copyright 2023  Kaj-Michael Lang <milang@tal.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "qatemfairlight.h"
#include "qatemconnection.h"

QAtemFairlight::QAtemFairlight(QObject *parent) :
    QAtemSubsystemBase(parent)
{
    m_commands << "FDLv" << "FMLv" << "FAAI" << "FAIP" << "FASP" << "FAMP" << "FMTl";

    m_commands << "AIXP" << "AICP" << "AILP" << "AEBP";

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

qint16 QAtemFairlight::getInputCount()
{
    return m_inputs.count();
}

QList<quint16> QAtemFairlight::inputSources()
{
    return m_inputs.keys();
}

QAtem::AudioInput QAtemFairlight::inputInfo(qint16 source)
{
    return m_inputs[source];
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

/**
 * @brief QAtemFairlight::onFAAI
 * @param payload
 *
 * Analog audio input level
 *
 */
void QAtemFairlight::onFAAI(const QByteArray &payload)
{
    // qDebug() << "FAAI: " << payload;

    quint16 as=QAtem::uint16at(payload, 6);
    quint8 sil=static_cast<qint8>(payload.at(8));
    quint8 ail=static_cast<qint8>(payload.at(8));

    m_inputs[as].input_level=as;

    qDebug() << "FAAI: " << as << sil << ail;
}

/**
 * @brief QAtemFairlight::onFAIP
 * @param payload
 *
 * Audio input type & configuration
 *
 */
void QAtemFairlight::onFAIP(const QByteArray &payload)
{
    // qDebug() << "FAIP: " << payload;

    quint16 as=QAtem::uint16at(payload, 6);
    quint8 it=static_cast<qint8>(payload.at(8));
    quint16 ept=QAtem::uint16at(payload, 12);

    quint8 sc=static_cast<qint8>(payload.at(15));
    quint8 ac=static_cast<qint8>(payload.at(16));

    quint8 sil=static_cast<qint8>(payload.at(17));
    quint8 ail=static_cast<qint8>(payload.at(18));

    qDebug() << "FAIP: " << as << it << ept << sc << ac << sil << ail;
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
    qint32 eq_gain=QAtem::int32at(payload, 10);
    qint32 makeup_gain=QAtem::int32at(payload, 14);
    qint32 gain=QAtem::int32at(payload, 18);
    bool fftb=static_cast<bool>(payload.at(22));

    qDebug() << "EQ: " << eqs << enabled << eq_gain << makeup_gain << gain << fftb;
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

    qDebug() << "FMTl:" << cnt;

    for (quint16 i=0; i<cnt; i++) {
        quint16 as=QAtem ::uint16at(payload, 22+i*11);
        qint8 state=static_cast<qint8>(payload.at(24+i*11));

        // qDebug() << "FMTl:" << i << as << state;

        if (m_inputs[as].state!=state) {
            m_inputs[as].state=state;

            emit tallyChanged(as, state);
        }
    }
    emit tallyUpdated();
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
    quint16 as=QAtem::uint16at(payload, 6);
    qint64 sid=QAtem::int64at(payload, 14);
    qint8 st=static_cast<qint8>(payload.at(22));
    qint8 mfd=static_cast<qint8>(payload.at(23));
    qint8 fd=static_cast<qint8>(payload.at(24));

    qint32 gain=QAtem::int32at(payload, 26);

    qint8 bands=static_cast<qint8>(payload.at(34));
    bool eq=static_cast<bool>(payload.at(35));

    qint16 balance=QAtem::int16at(payload, 46); // -10000 - 10000
    qint32 fgain=QAtem::int32at(payload, 50); // -10000 - 0 - 1000

    qDebug() << "FASP: " << payload.toHex(':');
    qDebug() << "AudioSource: " << as << fgain << balance << gain << sid << st << mfd << fd << eq << bands;

    m_inputs[as].index=as;
    m_inputs[as].gain=gain;
    m_inputs[as].fairlight=true;
    m_inputs[as].eq=eq;
    m_inputs[as].eq_bands=bands;

    m_inputs[as].fbalance=balance;
    m_inputs[as].fgain=fgain;

    emit audioSourceUpdated(as);
}

/**
 * @brief QAtemFairlight::onAIXP
 * @param payload
 *
 * Audio input expander/gate
 *
 */
void QAtemFairlight::onAIXP(const QByteArray &payload)
{
    qDebug() << "AIXP: " << payload.toHex(':');
}

/**
 * @brief QAtemFairlight::onAICP
 * @param payload
 *
 * Audio input compression
 *
 */
void QAtemFairlight::onAICP(const QByteArray &payload)
{
    qDebug() << "AICP: " << payload.toHex(':');
}

/**
 * @brief QAtemFairlight::onAILP
 * @param payload
 *
 * Audio input limiter
 *
 */
void QAtemFairlight::onAILP(const QByteArray &payload)
{
    qDebug() << "AILP: " << payload.toHex(':');
}

/**
 * @brief QAtemFairlight::onAEBP
 * @param payload
 *
 * Audio input eq bands
 *
 */
void QAtemFairlight::onAEBP(const QByteArray &payload)
{
    qDebug() << "AILP: " << payload.toHex(':');
}
