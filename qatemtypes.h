/*
Copyright 2014  Peter Simonsson <peter.simonsson@gmail.com>

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

#ifndef QATEM_TOPOLOGY_H
#define QATEM_TOPOLOGY_H

#include "libqatemcontrol_global.h"

#include <QObject>
#include <QtGlobal>
#include <QString>
#include <QPointF>
#include <QSizeF>
#include <QColor>

namespace QAtem
{
    Q_NAMESPACE

    typedef union
    {
        quint16 u16;        
        quint8 u8[2];
    } U16_U8;

    typedef union
    {
        qint16 s16;
        qint8 u8[2];
    } S16_S8;

    typedef union
    {
        quint32 u32;
        quint16 u16[2];
        quint8 u8[4];
    } U32_U8;

    typedef union
    {
        qint32 s32;
        qint8 s8[4];
    } S32_S8;

    typedef union
    {
        qint64 s64;
        qint8  s8[8];
    } S64_S8;

    inline quint16 uint16at(const QByteArray &ba, int p) {
        QAtem::U16_U8 val;
        val.u8[1] = static_cast<quint8>(ba.at(p));
        val.u8[0] = static_cast<quint8>(ba.at(p+1));

        return val.u16;
    };

    inline qint16 int16at(const QByteArray &ba, int p) {
        QAtem::S16_S8 val;
        val.u8[1] = static_cast<quint8>(ba.at(p));
        val.u8[0] = static_cast<quint8>(ba.at(p+1));

        return val.s16;
    };

    inline quint16 uint32at(const QByteArray &ba, int p) {
        QAtem::U32_U8 val;
        val.u8[3] = static_cast<quint8>(ba.at(p));
        val.u8[2] = static_cast<quint8>(ba.at(p+1));
        val.u8[1] = static_cast<quint8>(ba.at(p+2));
        val.u8[0] = static_cast<quint8>(ba.at(p+3));

        return val.u32;
    };

    inline qint64 int64at(const QByteArray &ba, int p) {
        QAtem::S64_S8 val;
        val.s8[7] = static_cast<qint8>(ba.at(p));
        val.s8[6] = static_cast<qint8>(ba.at(p+1));
        val.s8[5] = static_cast<qint8>(ba.at(p+2));
        val.s8[4] = static_cast<qint8>(ba.at(p+3));
        val.s8[3] = static_cast<qint8>(ba.at(p+4));
        val.s8[2] = static_cast<qint8>(ba.at(p+5));
        val.s8[1] = static_cast<qint8>(ba.at(p+6));
        val.s8[0] = static_cast<qint8>(ba.at(p+7));

        return val.s64;
    };

    inline bool boolat(const QByteArray &ba, int p) {
        return static_cast<bool>(ba.at(p));
    };

    struct LIBQATEMCONTROLSHARED_EXPORT Topology
    {
        quint8 MEs;
        quint8 sources;
        quint8 colorGenerators;
        quint8 auxBusses;
        quint8 downstreamKeyers;
        quint8 upstreamKeyers;
        quint8 stingers;
        quint8 DVEs;
        quint8 supersources;
        bool hasSD;
        Q_GADGET
        Q_PROPERTY(int MEs MEMBER MEs)
        Q_PROPERTY(int sources MEMBER sources)
        Q_PROPERTY(int colorGenerators MEMBER colorGenerators)
        Q_PROPERTY(int auxBusses MEMBER auxBusses)
        Q_PROPERTY(int downstreamKeyers MEMBER downstreamKeyers)
        Q_PROPERTY(int upstreamKeyers MEMBER upstreamKeyers)
        Q_PROPERTY(int stingers MEMBER stingers)
        Q_PROPERTY(int DVEs MEMBER DVEs)
        Q_PROPERTY(int supersources MEMBER supersources)
        Q_PROPERTY(bool hasSD MEMBER hasSD)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT VideoMode
    {
        VideoMode() : index(0), framesPerSecond(0.0) {}
        VideoMode(quint8 i, const QString &n, const QSize &s, float f) :
            index(i), name(n), size(s), framesPerSecond(f) {}

        quint8 index;
        QString name;
        QSize size;
        float framesPerSecond;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MultiView
    {
        MultiView(quint8 i) : index(i) {}

        quint8 index;
        /// Multi view layout, 0 = prg/prv on top, 1 = prg/prv on bottom, 2 = prg/prv on left, 3 = prg/prv on right
        quint8 layout;
        quint16 sources[10];
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT InputInfo
    {
        InputInfo()
        {
            index = 0;
            tally = 0;
            externalType = 0;
            internalType = 0;
            availableExternalTypes = 0;
            availability = 0;
            meAvailability = 0;
        }

        quint16 index;
        quint8 tally;
        quint8 externalType; // 0 = Internal, 1 = SDI, 2 = HDMI, 3 = Composite, 4 = Component, 5 = SVideo
        quint8 internalType; // 0 = External, 1 = Black, 2 = Color Bars, 3 = Color Generator, 4 = Media Player Fill, 5 = Media Player Key, 6 = SuperSource, 128 = ME Output, 129 = Auxiliary, 130 = Mask
        quint8 availableExternalTypes; // Bit 0: SDI, 1: HDMI, 2: Component, 3: Composite, 4: SVideo
        quint8 availability; // Bit 0: Auxiliary, 1: Multiviewer, 2: SuperSource Art, 3: SuperSource Box, 4: Key Sources
        quint8 meAvailability; // Bit 0: ME1 + Fill Sources, 1: ME2 + Fill Sources
        QString longText;
        QString shortText;
        Q_GADGET
        Q_PROPERTY(int index MEMBER index)
        Q_PROPERTY(int tally MEMBER tally)
        Q_PROPERTY(int externalType MEMBER externalType)
        Q_PROPERTY(int internalType MEMBER internalType)
        Q_PROPERTY(QString longText MEMBER longText)
        Q_PROPERTY(QString shortText MEMBER shortText)
    };

    enum MediaType
    {
        StillMedia = 1,
        ClipMedia = 2,
        SoundMedia = 3
    };
    Q_ENUM_NS(MediaType)

    struct LIBQATEMCONTROLSHARED_EXPORT MediaInfo
    {
        quint8 index;
        bool used;
        quint16 frameCount;
        QString name;
        MediaType type;
        QByteArray hash;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MediaPlayerState
    {
        quint8 index;
        bool loop;
        bool playing;
        bool atBegining;
        quint8 currentFrame;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT AudioInput
    {
        quint16 index;
        quint8 type; // 0 = Video input, 1 = Media player, 2 = External
        quint8 plugType; // 0 = Internal, 1 = SDI, 2 = HDMI, 3 = Component, 4 = Composite, 5 = SVideo, 32 = XLR, 64 = AES/EBU, 128 = RCA
        quint8 state; // 0 = Off, 1 = On, 2 = AFV
        float balance;
        float gain; // dB
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT AudioLevel
    {
        quint16 index;
        float left;
        float right;
        float peakLeft;
        float peakRight;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT DveKeyFrame
    {
        QPointF position;
        QSizeF size;
        float rotation;
        float lightSourceDirection;
        quint8 lightSourceAltitude;
        QColor borderColor;
        float borderOutsideWidth;
        float borderInsideWidth;
        quint8 borderOutsideSoften;
        quint8 borderInsideSoften;
        quint8 borderOpacity;
        float borderBevelPosition;
        quint8 borderBevelSoften;
        float maskTop;
        float maskBottom;
        float maskLeft;
        float maskRight;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT Camera
    {
        Camera(quint8 i) :
            input(i), focus(0), autoFocused(false), iris(0), zoomSpeed(0),
            gain(0), whiteBalance(0), shutter(0),
            liftR(0), liftG(0), liftB(0), liftY(0),
            gammaR(0), gammaG(0), gammaB(0), gammaY(0),
            gainR(0), gainG(0), gainB(0), gainY(0),
            contrast(0), lumMix(0), hue(0), saturation(0)
        {
        }

        quint8 input;
        quint16 focus;
        bool autoFocused;
        quint16 iris;
        qint16 zoomSpeed;
        quint16 gain;
        quint16 whiteBalance;
        quint16 shutter;
        float liftR;
        float liftG;
        float liftB;
        float liftY;
        float gammaR;
        float gammaG;
        float gammaB;
        float gammaY;
        float gainR;
        float gainG;
        float gainB;
        float gainY;
        quint8 contrast;
        quint8 lumMix;
        quint16 hue;
        quint8 saturation;
        Q_GADGET
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MacroInfo
    {
        quint8 index;
        bool used;
        QString name;
        QString description;
        Q_GADGET
    };

    enum MacroRunningState
    {
        MacroStoped,
        MacroRunning,
        MacroWaiting
    };
    Q_ENUM_NS(MacroRunningState)
}

#endif //QATEM_TOPOLOGY_H
