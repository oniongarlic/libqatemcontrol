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
#include <QPoint>
#include <QPointF>
#include <QSizeF>
#include <QColor>
#include <QRect>
#include <QVector4D>

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

    inline quint32 uint32at(const QByteArray &ba, int p) {
        QAtem::U32_U8 val;
        val.u8[3] = static_cast<quint8>(ba.at(p));
        val.u8[2] = static_cast<quint8>(ba.at(p+1));
        val.u8[1] = static_cast<quint8>(ba.at(p+2));
        val.u8[0] = static_cast<quint8>(ba.at(p+3));

        return val.u32;
    };

    inline qint32 int32at(const QByteArray &ba, int p) {
        QAtem::S32_S8 val;
        val.s8[3] = static_cast<quint8>(ba.at(p));
        val.s8[2] = static_cast<quint8>(ba.at(p+1));
        val.s8[1] = static_cast<quint8>(ba.at(p+2));
        val.s8[0] = static_cast<quint8>(ba.at(p+3));

        return val.s32;
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

    inline QString stringat(const QByteArray &ba, int index, int len=-1) {
        QString a=ba.mid(index, len);
        a.truncate(a.indexOf(QChar::Null));
        return a;
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
        Q_PROPERTY(quint8 index MEMBER index)
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QSize size MEMBER size)
        Q_PROPERTY(float framesPerSecond MEMBER framesPerSecond)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MultiView
    {
        MultiView(quint8 i) : index(i) {}

        quint8 index;
        /// Multi view layout, 0 = prg/prv on top, 1 = prg/prv on bottom, 2 = prg/prv on left, 3 = prg/prv on right
        quint8 layout;
        quint16 sources[10];
        Q_GADGET
        Q_PROPERTY(quint8 index MEMBER index)
        Q_PROPERTY(quint8 layout MEMBER layout)
    };

    enum InputTypeExternal
    {
        InputInternal = 0,
        InputSDI = 1,
        InputHDMI = 2,
        InputComposite = 3,
        InputComponents = 4,
        InputSVideo = 5
    };
    Q_ENUM_NS(InputTypeExternal)

    enum InputTypeInternal
    {
        InputExternal = 0,
        InputBlack = 1,
        InputColorBars = 2,
        InputColorGenerator = 3,
        InputMediaPlayerFill = 4,
        InputMediaPlayerKey = 5,
        InputSuperSource = 6,
        InputDirect = 7,
        InputMEOutput = 128,
        InputAux = 129,
        InputMask = 130,
        InputMultiView = 131
    };
    Q_ENUM_NS(InputTypeInternal)

    enum InputMEAvailability {
        InputME1=1,
        InputME2=2,
        InputME3=4, // xxx?
        InputME4=8,
    };
    Q_ENUM_NS(InputMEAvailability)

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
            defaultName = true;
        }

        quint16 index;
        quint16 tally;
        quint16 externalType; // 0 = Internal, 1 = SDI, 2 = HDMI, 3 = Composite, 4 = Component, 5 = SVideo
        quint16 internalType; // 0 = External, 1 = Black, 2 = Color Bars, 3 = Color Generator, 4 = Media Player Fill, 5 = Media Player Key, 6 = SuperSource, 128 = ME Output, 129 = Auxiliary, 130 = Mask
        quint16 availableExternalTypes; // Bit 0: SDI, 1: HDMI, 2: Component, 3: Composite, 4: SVideo
        quint16 availability; // Bit 0: Auxiliary, 1: Multiviewer, 2: SuperSource Art, 3: SuperSource Box, 4: Key Sources
        quint16 meAvailability; // Bit 0: ME1 + Fill Sources, 1: ME2 + Fill Sources
        bool defaultName;
        QString longText;
        QString shortText;
        Q_GADGET
        Q_PROPERTY(quint16 index MEMBER index)
        Q_PROPERTY(quint16 tally MEMBER tally)
        Q_PROPERTY(quint16 externalType MEMBER externalType)
        Q_PROPERTY(quint16 internalType MEMBER internalType)
        Q_PROPERTY(quint16 availableExternalTypes MEMBER availableExternalTypes)
        Q_PROPERTY(quint16 availability MEMBER availability)
        Q_PROPERTY(quint16 meAvailability MEMBER meAvailability)
        Q_PROPERTY(bool defaultName MEMBER defaultName)
        Q_PROPERTY(QString longText MEMBER longText)
        Q_PROPERTY(QString shortText MEMBER shortText)
    };

    enum RecordingDiskStatus
    {
        DiskIdle = 0,
        DiskUnformatted = 1,
        DiskActive= 2,
        DiskRecording = 4
    };
    Q_ENUM_NS(RecordingDiskStatus)

    struct LIBQATEMCONTROLSHARED_EXPORT RecordingInfo
    {
        RecordingInfo()
        {
            disk=0;
            available=0;
        }

        quint32 disk;
        quint32 available;
        RecordingDiskStatus status;
        QString volumeName;

        Q_GADGET
        Q_PROPERTY(quint16 disk MEMBER disk)
        Q_PROPERTY(RecordingDiskStatus status MEMBER status)
        Q_PROPERTY(QString volumeName MEMBER volumeName)
    };

    enum TimeCodeMode
    {
        FreeRun = 0,
        TimeOfDay = 1
    };
    Q_ENUM_NS(TimeCodeMode)

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
        Q_PROPERTY(quint8 index MEMBER index)
        Q_PROPERTY(bool used MEMBER used)
        Q_PROPERTY(quint16 frameCount MEMBER frameCount)
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(MediaType type MEMBER type)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MediaPlayerState
    {
        quint8 index;
        bool loop;
        bool playing;
        bool atBegining;
        quint8 currentFrame;
        Q_GADGET
        Q_PROPERTY(quint8 index MEMBER index)
        Q_PROPERTY(bool loop MEMBER loop)
        Q_PROPERTY(bool playing MEMBER playing)
        Q_PROPERTY(bool atBegining MEMBER atBegining)
        Q_PROPERTY(quint8 currentFrame MEMBER currentFrame)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT AudioInput
    {
        quint16 index;
        quint8 type; // 0 = Video input, 1 = Media player, 2 = External
        quint8 plugType; // 0 = Internal, 1 = SDI, 2 = HDMI, 3 = Component, 4 = Composite, 5 = SVideo, 32 = XLR, 64 = AES/EBU, 128 = RCA
        quint8 state; // 0 = Off, 1 = On, 2 = AFV
        float balance;
        float gain; // dB
        bool fairlight;
        bool eq;
        quint8 eq_bands;
        float eq_gain;
        quint8 input_level;
        qint16 fbalance;
        qint32 fgain;
        Q_GADGET
        Q_PROPERTY(quint16 index MEMBER index)
        Q_PROPERTY(quint8 type MEMBER type)
        Q_PROPERTY(quint8 plugType MEMBER plugType)
        Q_PROPERTY(quint8 state MEMBER state)
        Q_PROPERTY(float balance MEMBER balance)
        Q_PROPERTY(float gain MEMBER gain)
        Q_PROPERTY(bool fairlight MEMBER fairlight)
        Q_PROPERTY(bool eq MEMBER eq)
        Q_PROPERTY(quint8 eq_bands MEMBER eq_bands)
        Q_PROPERTY(float eq_gain MEMBER eq_gain)
        Q_PROPERTY(quint8 input_level MEMBER input_level)
        Q_PROPERTY(qint16 fbalance MEMBER fbalance)
        Q_PROPERTY(qint32 fgain MEMBER fgain)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT AudioLevel
    {
        quint16 index;
        float left;
        float right;
        float peakLeft;
        float peakRight;
        Q_GADGET
        Q_PROPERTY(quint16 index MEMBER index)
        Q_PROPERTY(float left MEMBER left)
        Q_PROPERTY(float right MEMBER right)
        Q_PROPERTY(float peakLeft MEMBER peakLeft)
        Q_PROPERTY(float peakRight MEMBER peakRight)
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
        Q_PROPERTY(QPointF position MEMBER position)
        Q_PROPERTY(QSizeF size MEMBER size)
        Q_PROPERTY(float rotation MEMBER rotation)
        Q_PROPERTY(float lightSourceDirection MEMBER lightSourceDirection)
        Q_PROPERTY(quint8 lightSourceAltitude MEMBER lightSourceAltitude)
        Q_PROPERTY(float borderOutsideWidth MEMBER borderOutsideWidth)
        Q_PROPERTY(float borderInsideWidth MEMBER borderInsideWidth)
        Q_PROPERTY(quint8 borderOutsideSoften MEMBER borderOutsideSoften)
        Q_PROPERTY(quint8 borderInsideSoften MEMBER borderInsideSoften)
        Q_PROPERTY(quint8 borderOpacity MEMBER borderOpacity)
        Q_PROPERTY(float borderBevelPosition MEMBER borderBevelPosition)
        Q_PROPERTY(quint8 borderBevelSoften MEMBER borderBevelSoften)
        Q_PROPERTY(float maskTop MEMBER maskTop)
        Q_PROPERTY(float maskBottom MEMBER maskBottom)
        Q_PROPERTY(float maskLeft MEMBER maskLeft)
        Q_PROPERTY(float maskRight MEMBER maskRight)
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
        Q_PROPERTY(quint8 input MEMBER input)
        Q_PROPERTY(quint16 focus MEMBER focus)
        Q_PROPERTY(bool autoFocused MEMBER autoFocused)
        Q_PROPERTY(quint16 iris MEMBER iris)
        Q_PROPERTY(qint16 zoomSpeed MEMBER zoomSpeed)
        Q_PROPERTY(quint16 gain MEMBER gain)
        Q_PROPERTY(quint16 whiteBalance MEMBER whiteBalance)
        Q_PROPERTY(quint16 shutter MEMBER shutter)
        Q_PROPERTY(float liftR MEMBER liftR)
        Q_PROPERTY(float liftG MEMBER liftG)
        Q_PROPERTY(float liftB MEMBER liftB)
        Q_PROPERTY(float liftY MEMBER liftY)
        Q_PROPERTY(float gammaR MEMBER gammaR)
        Q_PROPERTY(float gammaG MEMBER gammaG)
        Q_PROPERTY(float gammaB MEMBER gammaB)
        Q_PROPERTY(float gammaY MEMBER gammaY)
        Q_PROPERTY(float gainR MEMBER gainR)
        Q_PROPERTY(float gainG MEMBER gainG)
        Q_PROPERTY(float gainB MEMBER gainB)
        Q_PROPERTY(float gainY MEMBER gainY)
        Q_PROPERTY(quint8 contrast MEMBER contrast)
        Q_PROPERTY(quint8 lumMix MEMBER lumMix)
        Q_PROPERTY(quint16 hue MEMBER hue)
        Q_PROPERTY(quint8 saturation MEMBER saturation)
    };

    struct LIBQATEMCONTROLSHARED_EXPORT MacroInfo
    {
        quint8 index;
        bool used;
        QString name;
        QString description;
        Q_GADGET
        Q_PROPERTY(quint8 macroIndex MEMBER index)
        Q_PROPERTY(bool used MEMBER used)
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QString description MEMBER description)
    };

    enum SuperSourceArtOption
    {
        SuperSourceBackground=0,
        SuperSourceForeground
    };
    Q_ENUM_NS(SuperSourceArtOption)

    struct LIBQATEMCONTROLSHARED_EXPORT SuperSourceArt
    {
        quint16 fillSource;
        quint16 cutSource;
        SuperSourceArtOption option;
        bool premultiplied;
        double clip;
        double gain;
        bool invertkey;
        Q_GADGET
        Q_PROPERTY(quint16 fillSource MEMBER fillSource)
        Q_PROPERTY(quint16 cutSource MEMBER cutSource)
        Q_PROPERTY(SuperSourceArtOption option MEMBER option)
        Q_PROPERTY(bool premultiplied MEMBER premultiplied)
        Q_PROPERTY(double clip MEMBER clip)
        Q_PROPERTY(double gain MEMBER gain)
        Q_PROPERTY(bool invertKey MEMBER invertkey)
    };

    enum SuperSourceBoxFlags
    {
        SuperBoxEnabled = 1 << 0,
        SuperBoxSource = 1 << 1,
        SuperBoxPositionX = 1 << 2,
        SuperBoxPositionY = 1 << 3,
        SuperBoxSize = 1 << 4,
        SuperBoxCropped = 1 << 5,
        SuperBoxCropTop = 1 << 6,
        SuperBoxCropBottom = 1 << 7,
        SuperBoxCropLeft = 1 << 8,
        SuperBoxCropRight = 1 << 9,
    };
    Q_ENUM_NS(SuperSourceBoxFlags)
    Q_DECLARE_FLAGS(SuperSourceBoxMask, SuperSourceBoxFlags)

    struct LIBQATEMCONTROLSHARED_EXPORT SuperSourceBoxSettings
    {
        bool enabled;
        quint8 source;
        QPoint position;
        uint size;
        bool crop_enabled;
        QVector4D crop;
        bool border_enabled;
        QColor border_color;
        uint width_inner;
        uint width_outer;

        Q_GADGET
        Q_PROPERTY(bool enabled MEMBER enabled)
        Q_PROPERTY(quint8 source MEMBER source)
        Q_PROPERTY(QPoint position MEMBER position)
        Q_PROPERTY(uint size MEMBER size)
        Q_PROPERTY(bool cropEnabled MEMBER crop_enabled)
        Q_PROPERTY(QVector4D crop MEMBER crop)
        Q_PROPERTY(bool borderEnabled MEMBER border_enabled)
        Q_PROPERTY(QColor borderColor MEMBER border_color)
        Q_PROPERTY(uint borderWidthInner MEMBER width_inner)
        Q_PROPERTY(uint borderWidthOuter MEMBER width_outer)
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
