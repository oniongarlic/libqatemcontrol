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

#ifndef QATEMMIXEFFECT_H
#define QATEMMIXEFFECT_H

#include <qatemconnection.h>
#include <qupstreamkeysettings.h>

#include <QObject>

class QColor;

class LIBQATEMCONTROLSHARED_EXPORT QAtemMixEffect : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 meID READ meId NOTIFY meIdChanged)
    Q_PROPERTY(FTBStatus fadeToBlackStatus READ fadeToBlackStatus NOTIFY fadeToBlackStatusChanged FINAL)

    Q_PROPERTY(quint16 programInputSource READ programInput NOTIFY programInputChanged FINAL)
    Q_PROPERTY(quint16 previewInputSource READ previewInput NOTIFY previewInputChanged FINAL)

public:
    explicit QAtemMixEffect(QAtemConnection *parent = nullptr);
    explicit QAtemMixEffect(quint8 id, QAtemConnection *parent = nullptr);
    ~QAtemMixEffect();

    enum FTBStatus {
            FadeOff,
            FadeOn,
            FadeInProgress
    };
    Q_ENUM(FTBStatus)

    enum Sources {
        BlackInput=0,

        CameraInput1=1,
        CameraInput2=2,
        CameraInput3=3,
        CameraInput4=4,
        CameraInput5=5,
        CameraInput6=6,
        CameraInput7=7,
        CameraInput8=8,
        CameraInput9=9,

        CameraInput10=10,
        CameraInput11=11,
        CameraInput12=12,
        CameraInput13=13,
        CameraInput14=14,
        CameraInput15=15,
        CameraInput16=16,
        CameraInput17=17,
        CameraInput18=18,
        CameraInput19=19,

        CameraInput20=20,
        CameraInput21=21,
        CameraInput22=22,
        CameraInput23=23,
        CameraInput24=24,
        CameraInput25=25,
        CameraInput26=26,
        CameraInput27=27,
        CameraInput28=28,
        CameraInput29=29,

        CameraInput30=30,
        CameraInput31=31,
        CameraInput32=32,
        CameraInput33=33,
        CameraInput34=34,
        CameraInput35=35,
        CameraInput36=36,
        CameraInput37=37,
        CameraInput38=38,
        CameraInput39=39,
        CameraInput40=40,

        ColorBarsInput=1000,
        ColorGenerator1=2001,
        ColorGenerator2=2002,

        MediaPlayer1=3010,
        MediaPlayer1Key=3011,

        MediaPlayer2=3020,
        MediaPlayer2Key=3021,

        MediaPlayer3=3030,
        MediaPlayer3Key=3031,

        MediaPlayer4=3040,
        MediaPlayer4Key=3041,

        Key1Mask=4010,
        Key2Mask=4020,
        Key3Mask=4030,
        Key4Mask=4040,
        Key5Mask=4050,
        Key6Mask=4060,
        Key7Mask=4070,
        Key8Mask=4080,
        Key9Mask=4090,
        Key10Mask=4100,
        Key11Mask=4110,
        Key12Mask=4120,
        Key13Mask=4130,
        Key14Mask=4140,
        Key15Mask=4150,
        Key16Mask=4160,

        DSK1Mask=5010,
        DSK2Mask=5020,
        DSK3Mask=5030,
        DSK4Mask=5040,

        SuperSource1=6000,
        SuperSource2=6001,

        CleanFeed1=7001,
        CleanFeed2=7002,
        CleanFeed3=7003,
        CleanFeed4=7004,

        ME1Program=10010,
        ME1Preview=10011,

        ME2Program=10020,
        ME2Preview=10021,

        ME3Program=10030,
        ME3Preview=10031,

        ME4Program=10040,
        ME4Preview=10041,

    };
    Q_ENUM(Sources)

    Q_INVOKABLE void createUpstreamKeyers(quint8 count);

    /// @returns the index of the input that is on program
    Q_INVOKABLE quint16 programInput() const { return m_programInput; }
    /// @returns the index of the input that is on preview
    Q_INVOKABLE quint16 previewInput() const { return m_previewInput; }

    /// @returns true if transition preview is enabled
    Q_INVOKABLE bool transitionPreviewEnabled() const { return m_transitionPreviewEnabled; }
    /// @returns number of frames left of transition
    Q_INVOKABLE quint8 transitionFrameCount() const { return m_transitionFrameCount; }
    /// @returns percent left of transition
    Q_INVOKABLE quint16 transitionPosition() const { return m_transitionPosition; }
    /// @returns keyers used on next transition. Bit 0 = Background, 1-4 = keys, only bit 0 and 1 available on TVS
    Q_INVOKABLE quint8 keyersOnNextTransition() const { return m_keyersOnNextTransition; }
    /// @returns index of selected transition style for next transition. Bit 0 = Mix, 1 = Dip, 2 = Wipe, 3 = DVE and 4 = Stinger, only bit 0-2 available on TVS
    Q_INVOKABLE quint8 nextTransitionStyle() const { return m_nextTransitionStyle; }
    /// @returns keyers used on current transition. Bit 0 = Background, 1-4 = keys, only bit 0 and 1 available on TVS
    Q_INVOKABLE quint8 keyersOnCurrentTransition() const { return m_keyersOnCurrentTransition; }
    /// @returns index of selected transition style for current transition. Bit 0 = Mix, 1 = Dip, 2 = Wipe, 3 = DVE and 4 = Stinger, only bit 0-2 available on TVS
    Q_INVOKABLE quint8 currentTransitionStyle() const { return m_currentTransitionStyle; }

    /// @returns true if fade to black is on.
    Q_INVOKABLE bool fadeToBlackEnabled() const { return m_fadeToBlackEnabled; }
    /// @returns true if fade to black is fading to/from black
    Q_INVOKABLE bool fadeToBlackFading() const { return m_fadeToBlackFading; }
    /// @returns number of frames left of fade to black transition.
    Q_INVOKABLE quint8 fadeToBlackFrameCount() const { return m_fadeToBlackFrameCount; }
    /// @returns duration in number of frames for the fade to black transition.
    Q_INVOKABLE quint8 fadeToBlackFrames() const { return m_fadeToBlackFrames; }

    FTBStatus fadeToBlackStatus() const;

    /// @returns duration in number of frames for mix transition
    Q_INVOKABLE quint8 mixFrames() const { return m_mixFrames; }

    /// @returns duration in number of frames for dip transition
    Q_INVOKABLE quint8 dipFrames() const { return m_dipFrames; }
    /// @returns the source used for a dip transition
    Q_INVOKABLE quint16 dipSource() const { return m_dipSource; }

    /// @returns duration in number of frames for wipe transition
    Q_INVOKABLE quint8 wipeFrames() const { return m_wipeFrames; }
    /// @returns the border source index, used for wipe transition
    Q_INVOKABLE quint16 wipeBorderSource() const { return m_wipeBorderSource; }
    /// @returns border width for wipe transition
    Q_INVOKABLE quint16 wipeBorderWidth() const { return m_wipeBorderWidth; }
    /// @returns border softness for wipe transition
    Q_INVOKABLE quint16 wipeBorderSoftness() const { return m_wipeBorderSoftness; }
    /// @returns type of wipe transition
    Q_INVOKABLE quint8 wipeType() const { return m_wipeType; }
    /// @returns symmetry of wipe transition
    Q_INVOKABLE quint16 wipeSymmetry() const { return m_wipeSymmetry; }
    /// @returns x position of wipe transition
    Q_INVOKABLE quint16 wipeXPosition() const { return m_wipeXPosition; }
    /// @returns y position of wipe transition
    Q_INVOKABLE quint16 wipeYPosition() const { return m_wipeYPosition; }
    /// @returns true if wipe transition direction should be reversed
    Q_INVOKABLE bool wipeReverseDirection() const { return m_wipeReverseDirection; }
    /// @returns true if wipe transition direction should flip flop
    Q_INVOKABLE bool wipeFlipFlop() const { return m_wipeFlipFlop; }

    /// @returns duration in number of frames for DVE transition
    Q_INVOKABLE quint8 dveRate() const { return m_dveRate; }
    /**
     * @returns the selected effect for DVE transition
     *
     * Swosh:
     * 0 = Top left
     * 1 = Up
     * 2 = Top right
     * 3 = Left
     * 4 = Right
     * 5 = Bottom left
     * 6 = Down
     * 7 = Bottom right
     *
     * Spin:
     * 8 = Down left, clockwise
     * 9 = Up left, clockwise
     * 10 = Down right, clockwise
     * 11 = Up right, clockwise
     * 12 = Up right, anti clockwise
     * 13 = Down right, anti clockwise
     * 14 = Up left, anti clockwise
     * 15 = Down left, anti clockwise
     *
     * Squeeze:
     * 16 = Top left
     * 17 = Up
     * 18 = Top right
     * 19 = Left
     * 20 = Right
     * 21 = Bottom left
     * 22 = Down
     * 23 = Bottom right
     *
     * Push:
     * 24 = Top left
     * 25 = Up
     * 26 = Top right
     * 27 = Left
     * 28 = Right
     * 29 = Bottom left
     * 30 = Down
     * 31 = Bottom right
     *
     * Graphic:
     * 32 = Spin clockwise
     * 33 = Spin anti clockwise
     * 34 = Logo wipe
     */
    Q_INVOKABLE quint8 dveEffect() const { return m_dveEffect; }
    Q_INVOKABLE quint16 dveFillSource() const { return m_dveFillSource; }
    Q_INVOKABLE quint16 dveKeySource() const { return m_dveKeySource; }
    Q_INVOKABLE bool dveKeyEnabled() const { return m_dveKeyEnabled; }
    Q_INVOKABLE bool dvePreMultipliedKeyEnabled() const { return m_dvePreMultipliedKeyEnabled; }
    /// @returns the clip of the key in per cent for the DVE transition
    Q_INVOKABLE float dveKeyClip() const { return m_dveKeyClip; }
    /// @returns the gain of the key in per cent for the DVE transition
    Q_INVOKABLE float dveKeyGain() const { return m_dveKeyGain; }
    Q_INVOKABLE bool dveInvertKeyEnabled() const { return m_dveEnableInvertKey; }
    Q_INVOKABLE bool dveReverseDirection() const { return m_dveReverseDirection; }
    Q_INVOKABLE bool dveFlipFlopDirection() const { return m_dveFlipFlopDirection; }

    /// @returns source used for the Stinger transition. 1 = Media player 1, 2 = Media player 2
    Q_INVOKABLE quint8 stingerSource() const { return m_stingerSource; }
    /// @returns true if the Stinger transition has a pre multiplied key
    Q_INVOKABLE bool stingerPreMultipliedKeyEnabled() const { return m_stingerPreMultipliedKeyEnabled; }
    Q_INVOKABLE float stingerClip() const { return m_stingerClip; }
    Q_INVOKABLE float stingerGain() const { return m_stingerGain; }
    Q_INVOKABLE bool stingerInvertKeyEnabled() const { return m_stingerInvertKeyEnabled; }
    Q_INVOKABLE quint16 stingerPreRoll() const { return m_stingerPreRoll; }
    Q_INVOKABLE quint16 stingerClipDuration() const { return m_stingerClipDuration; }
    Q_INVOKABLE quint16 stingerTriggerPoint() const { return m_stingerTriggerPoint; }
    Q_INVOKABLE quint16 stingerMixRate() const { return m_stingerMixRate; }

    /// @returns number of upstream keys available on this M/E
    Q_INVOKABLE quint8 upstreamKeyCount() const { return static_cast<quint8>(m_upstreamKeys.count()); }
    /// @returns true if upstream key @p keyer is on air
    Q_INVOKABLE bool upstreamKeyOnAir(quint8 keyer) const;
    /// @returns the key type for upstream key @p keyer, 0 = luma, 1 = chroma, 2 = pattern, 3 = DVE
    Q_INVOKABLE quint8 upstreamKeyType(quint8 keyer) const { return m_upstreamKeys[keyer]->m_type; }
    /// @returns the source used as fill for upstream key @p keyer
    Q_INVOKABLE quint16 upstreamKeyFillSource(quint8 keyer) const { return m_upstreamKeys[keyer]->m_fillSource; }
    /// @returns the source used as key for upstream key @p keyer
    Q_INVOKABLE quint16 upstreamKeyKeySource(quint8 keyer) const { return m_upstreamKeys[keyer]->m_keySource; }
    /// @returns true if the mask is enabled for upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyEnableMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_enableMask; }
    /// @returns top mask for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyTopMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_topMask; }
    /// @returns bottom mask for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyBottomMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_bottomMask; }
    /// @returns left mask for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyLeftMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_leftMask; }
    /// @returns right mask for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyRightMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_rightMask; }
    /// @returns true if the key is pre multiplied for luma upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyLumaPreMultipliedKey(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaPreMultipliedKey; }
    /// @returns true if the key source should be inverted for luma upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyLumaInvertKey(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaInvertKey; }
    /// @returns clip for luma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyLumaClip(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaClip; }
    /// @returns gain for luma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyLumaGain(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaGain; }
    /// @returns hue for chroma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyChromaHue(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaHue; }
    /// @returns gain for chroma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyChromaGain(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaGain; }
    /// @returns y suppress for chroma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyChromaYSuppress(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaYSuppress; }
    /// @returns lift for chroma upstream key @p keyer
    Q_INVOKABLE float upstreamKeyChromaLift(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaLift; }
    /// @returns true if chroma upstream key @p keyer should have narrow chroma key range
    Q_INVOKABLE bool upstreamKeyChromaNarrowRange(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaNarrowRange; }
    /// @returns pattern of pattern upstream key @p keyer
    Q_INVOKABLE quint8 upstreamKeyPatternPattern(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternPattern; }
    /// @returns true if pattern upstream key @p keyer should invert the pattern
    Q_INVOKABLE bool upstreamKeyPatternInvertPattern(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternInvertPattern; }
    /// @returns size for pattern upstream key @p keyer
    Q_INVOKABLE float upstreamKeyPatternSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSize; }
    /// @returns symmetry for pattern upstream key @p keyer
    Q_INVOKABLE float upstreamKeyPatternSymmetry(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSymmetry; }
    /// @returns softness for pattern upstream key @p keyer
    Q_INVOKABLE float upstreamKeyPatternSoftness(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSoftness; }
    /// @returns x position for pattern upstream key @p keyer
    Q_INVOKABLE float upstreamKeyPatternXPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternXPosition; }
    /// @returns y position for pattern upstream key @p keyer
    Q_INVOKABLE float upstreamKeyPatternYPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternYPosition; }
    /// @returns x position of DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEXPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveXPosition; }
    /// @returns y position of DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEYPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveYPosition; }
    /// @returns x size of DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEXSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveXSize; }
    /// @returns y size of DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEYSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveYSize; }
    /// @returns rotation of DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVERotation(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveRotation; }
    /// @returns true if the drop shadow is enabled on the DVE for upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyDVEDropShadowEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveEnableDropShadow; }
    /// @returns direction of the light source for the drop shadow on the DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVELightSourceDirection(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveLightSourceDirection; }
    /// @returns altitude of the light source for the drop shadow on the DVE for upstream keu @p keyer
    Q_INVOKABLE quint8 upstreamKeyDVELightSourceAltitude(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveLightSourceAltitude; }
    /// @returns true if the border is enabled on the DVE for upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyDVEBorderEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveEnableBorder; }
    /// @returns the border style of the DVE for upstream key @p keyer. 0 = No Bevel, 1 = Bevel In Out, 2 = Bevel In, 3 = Bevel Out
    Q_INVOKABLE quint8 upstreamKeyDVEBorderStyle(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderStyle; }
    /// @returns the border color of the DVE for upstream key @p keyer
    Q_INVOKABLE QColor upstreamKeyDVEBorderColor(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderColor; }
    /// @returns the outside width of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEBorderOutsideWidth(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOutsideWidth; }
    /// @returns the inside width of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEBorderInsideWidth(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderInsideWidth; }
    /// @returns the outside soften (%) of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE quint8 upstreamKeyDVEBorderOutsideSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOutsideSoften; }
    /// @returns the inside soften (%) of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE quint8 upstreamKeyDVEBorderInsideSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderInsideSoften; }
    /// @returns the opacity of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE quint8 upstreamKeyDVEBorderOpacity(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOpacity; }
    /// @returns the bevel position of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEBorderBevelPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderBevelPosition; }
    /// @returns the bevel soften (%) of the border of the DVE for upstream key @p keyer
    Q_INVOKABLE float upstreamKeyDVEBorderBevelSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderBevelSoften; }
    /// @returns the rate in frames the DVE for upstream key @p keyer runs at
    Q_INVOKABLE quint8 upstreamKeyDVERate(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveRate; }
    /// @returns true if key frame A has been set for the DVE for upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyDVEKeyFrameASet(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveKeyFrameASet; }
    /// @returns true if key frame B has been set for the DVE for upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyDVEKeyFrameBSet(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveKeyFrameBSet; }
    Q_INVOKABLE bool upstreamKeyDVEMaskEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskEnabled; }
    Q_INVOKABLE float upstreamKeyDVEMaskTop(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskTop; }
    Q_INVOKABLE float upstreamKeyDVEMaskBottom(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskBottom; }
    Q_INVOKABLE float upstreamKeyDVEMaskLeft(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskLeft; }
    Q_INVOKABLE float upstreamKeyDVEMaskRight(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskRight; }
    /// @returns true if fly is enabled for non DVE type of the upstream key @p keyer
    Q_INVOKABLE bool upstreamKeyEnableFly(quint8 keyer) const { return m_upstreamKeys[keyer]->m_enableFly; }
    /// @returns the current key frame settings for keyer @p keyer and key frame @frame. 1 = KeyFrame A, 2 = KeyFrame B
    QAtem::DveKeyFrame upstreamKeyKeyFrame(quint8 keyer, quint8 frame) const { return m_upstreamKeys[keyer]->m_keyFrames[frame - 1]; }

    quint8 meId() const
    {
        return m_id;
    }

public slots:
    void cut();
    void autoTransition();

    void changeProgramInput(quint16 index);
    void changePreviewInput(quint16 index);

    /// Set the current position of the transition to @p position. Set @p position to 0 to signal transition done.
    void setTransitionPosition(quint16 position);
    void setTransitionPreview(bool state);
    void setTransitionType(quint8 type);
    void setUpstreamKeyOnNextTransition(quint8 keyer, bool state);
    void setBackgroundOnNextTransition(bool state);

    void toggleFadeToBlack();
    void setFadeToBlackFrameRate(quint8 frames);

    void setMixFrames(quint8 frames);

    void setDipFrames(quint8 frames);
    void setDipSource(quint16 source);

    void setWipeBorderSource(quint16 source);
    void setWipeFrames(quint8 frames);
    void setWipeBorderWidth(quint16 width);
    void setWipeBorderSoftness(quint16 softness);
    void setWipeType(quint8 type);
    void setWipeSymmetry(quint16 value);
    void setWipeXPosition(quint16 value);
    void setWipeYPosition(quint16 value);
    void setWipeReverseDirection(bool reverse);
    void setWipeFlipFlop(bool flipFlop);

    void setDVERate(quint8 frames);
    /**
     * Set the effect used for the DVE transition to @p effect
     *
     * Swosh:
     * 0 = Top left
     * 1 = Up
     * 2 = Top right
     * 3 = Left
     * 4 = Right
     * 5 = Bottom left
     * 6 = Down
     * 7 = Bottom right
     *
     * Spin:
     * 8 = Down left, clockwise
     * 9 = Up left, clockwise
     * 10 = Down right, clockwise
     * 11 = Up right, clockwise
     * 12 = Up right, anti clockwise
     * 13 = Down right, anti clockwise
     * 14 = Up left, anti clockwise
     * 15 = Down left, anti clockwise
     *
     * Squeeze:
     * 16 = Top left
     * 17 = Up
     * 18 = Top right
     * 19 = Left
     * 20 = Right
     * 21 = Bottom left
     * 22 = Down
     * 23 = Bottom right
     *
     * Push:
     * 24 = Top left
     * 25 = Up
     * 26 = Top right
     * 27 = Left
     * 28 = Right
     * 29 = Bottom left
     * 30 = Down
     * 31 = Bottom right
     *
     * Graphic:
     * 32 = Spin clockwise
     * 33 = Spin anti clockwise
     * 34 = Logo wipe
     */
    void setDVEEffect(quint8 effect);
    void setDVEFillSource(quint16 source);
    void setDVEKeySource(quint16 source);
    void setDVEKeyEnabled(bool enabled);
    void setDVEPreMultipliedKeyEnabled(bool enabled);
    /// Set clip of key for DVE transition to @p percent
    void setDVEKeyClip(float percent);
    /// Set gain of key for DVE transition to @p percent
    void setDVEKeyGain(float percent);
    void setDVEInvertKeyEnabled(bool enabled);
    /// Set to true to do the DVE transition in the reverse direction
    void setDVEReverseDirection(bool reverse);
    /// Set to true to flip flop the direction of the DVE transition between uses
    void setDVEFlipFlopDirection(bool flipFlop);

    /// Set the source used for Stinger transition to @p source. 1 = Media player 1, 2 = Media player 2
    void setStingerSource(quint8 source);
    /// Enable if the key is pre multiplied in the source for the Stinger transition
    void setStingerPreMultipliedKeyEnabled(bool enabled);
    void setStingerClip(float percent);
    void setStingerGain(float percent);
    void setStingerInvertKeyEnabled(bool enabled);
    void setStingerPreRoll(quint16 frames);
    void setStingerClipDuration(quint16 frames);
    void setStingerTriggerPoint(quint16 frames);
    void setStingerMixRate(quint16 frames);

    void setUpstreamKeyOnAir(quint8 keyer, bool state);
    void setUpstreamKeyType(quint8 keyer, quint8 type);
    void setUpstreamKeyFillSource(quint8 keyer, quint16 source);
    void setUpstreamKeyKeySource(quint8 keyer, quint16 source);
    void setUpstreamKeyEnableMask(quint8 keyer, bool enable);
    void setUpstreamKeyMask(quint8 keyer, float top, float bottom, float left, float right);
    void setUpstreamKeyLumaPreMultipliedKey(quint8 keyer, bool preMultiplied);
    void setUpstreamKeyLumaInvertKey(quint8 keyer, bool invert);
    void setUpstreamKeyLumaClip(quint8 keyer, float clip);
    void setUpstreamKeyLumaGain(quint8 keyer, float gain);
    void setUpstreamKeyChromaHue(quint8 keyer, float hue);
    void setUpstreamKeyChromaGain(quint8 keyer, float gain);
    void setUpstreamKeyChromaYSuppress(quint8 keyer, float ySuppress);
    void setUpstreamKeyChromaLift(quint8 keyer, float lift);
    void setUpstreamKeyChromaNarrowRange(quint8 keyer, bool narrowRange);
    void setUpstreamKeyPatternPattern(quint8 keyer, quint8 pattern);
    void setUpstreamKeyPatternInvertPattern(quint8 keyer, bool invert);
    void setUpstreamKeyPatternSize(quint8 keyer, float size);
    void setUpstreamKeyPatternSymmetry(quint8 keyer, float symmetry);
    void setUpstreamKeyPatternSoftness(quint8 keyer, float softness);
    void setUpstreamKeyPatternXPosition(quint8 keyer, float xPosition);
    void setUpstreamKeyPatternYPosition(quint8 keyer, float yPosition);
    void setUpstreamKeyDVEPosition(quint8 keyer, float xPosition, float yPosition);
    void setUpstreamKeyDVESize(quint8 keyer, float xSize, float ySize);
    void setUpstreamKeyDVEPositionAndSize(quint8 keyer, QPointF position, QSizeF size);
    void setUpstreamKeyDVERotation(quint8 keyer, float rotation);
    void setUpstreamKeyDVELightSource(quint8 keyer, float direction, quint8 altitude);
    void setUpstreamKeyDVEDropShadowEnabled(quint8 keyer, bool enabled);
    void setUpstreamKeyDVEBorderEnabled(quint8 keyer, bool enabled);
    /// Set the border style of the upstream key DVE. 0 = No Bevel, 1 = Bevel In Out, 2 = Bevel In, 3 = Bevel Out
    void setUpstreamKeyDVEBorderStyle(quint8 keyer, quint8 style);
    void setUpstreamKeyDVEBorderColorH(quint8 keyer, float h);
    void setUpstreamKeyDVEBorderColorS(quint8 keyer, float s);
    void setUpstreamKeyDVEBorderColorL(quint8 keyer, float l);
    void setUpstreamKeyDVEBorderColor(quint8 keyer, const QColor& color);
    void setUpstreamKeyDVEBorderWidth(quint8 keyer, float outside, float inside);
    void setUpstreamKeyDVEBorderSoften(quint8 keyer, quint8 outside, quint8 inside);
    void setUpstreamKeyDVEBorderOpacity(quint8 keyer, quint8 opacity);
    void setUpstreamKeyDVEBorderBevelPosition(quint8 keyer, float position);
    void setUpstreamKeyDVEBorderBevelSoften(quint8 keyer, float soften);
    void setUpstreamKeyDVERate(quint8 keyer, quint8 rate);
    /// Set the @p keyFrame of the DVE for upstream keyer @p keyer. 1 = Keyframe A, 2 = Keyframe B
    void setUpstreamKeyDVEKeyFrame(quint8 keyer, quint8 keyFrame);
    /**
     * Make the upstream key @p keyer run to @p position. 1 = Keyframe A, 2 = Keyframe B, 3 = Fullscreen, 4 = Infinite
     * If the @p position = infinite there is also a @p direction.
     * Available directions:
     * 0 = Center
     * 1 = Top left
     * 2 = Up
     * 3 = Top right
     * 4 = Left
     * 5 = Center
     * 6 = Right
     * 7 = Bottom left
     * 8 = Down
     * 9 = Bottom right
     */
    void runUpstreamKeyTo(quint8 keyer, quint8 position, quint8 direction);
    /// Enable fly on the non DVE key types of the upstream key @p keyer
    void setUpstreamKeyFlyEnabled(quint8 keyer, bool enable);
    void setUpstreamKeyDVEMaskEnabled(quint8 keyer, bool enable);
    void setUpstreamKeyDVEMask(quint8 keyer, float top, float bottom, float left, float right);

protected slots:
    void onPrgI(const QByteArray& payload);
    void onPrvI(const QByteArray& payload);

    void onTrPr(const QByteArray& payload);
    void onTrPs(const QByteArray& payload);
    void onTrSS(const QByteArray& payload);

    void onFtbS(const QByteArray& payload);
    void onFtbP(const QByteArray& payload);

    void onTMxP(const QByteArray& payload);

    void onTDpP(const QByteArray& payload);

    void onTWpP(const QByteArray& payload);

    void onTDvP(const QByteArray& payload);

    void onTStP(const QByteArray& payload);

    void onKeOn(const QByteArray& payload);
    void onKeBP(const QByteArray& payload);
    void onKeLm(const QByteArray& payload);
    void onKeCk(const QByteArray& payload);
    void onKePt(const QByteArray& payload);
    void onKeDV(const QByteArray& payload);
    void onKeFS(const QByteArray& payload);
    void onKKFP(const QByteArray& payload);

protected:
    void setKeyOnNextTransition (int index, bool state);

private:
    quint8 m_id;
    QAtemConnection *m_atemConnection;

    quint16 m_programInput;
    quint16 m_previewInput;

    bool m_transitionPreviewEnabled;
    quint8 m_transitionFrameCount;
    quint16 m_transitionPosition;
    quint8 m_keyersOnCurrentTransition;
    quint8 m_currentTransitionStyle;
    quint8 m_keyersOnNextTransition;
    quint8 m_nextTransitionStyle;

    bool m_fadeToBlackEnabled;
    bool m_fadeToBlackFading;
    quint8 m_fadeToBlackFrameCount;
    quint8 m_fadeToBlackFrames;

    quint8 m_mixFrames;

    quint8 m_dipFrames;
    quint16 m_dipSource;

    quint8 m_wipeFrames;
    quint16 m_wipeBorderSource;
    quint16 m_wipeBorderWidth;
    quint16 m_wipeBorderSoftness;
    quint8 m_wipeType;
    quint16 m_wipeSymmetry;
    quint16 m_wipeXPosition;
    quint16 m_wipeYPosition;
    bool m_wipeReverseDirection;
    bool m_wipeFlipFlop;

    quint8 m_dveRate;
    quint8 m_dveEffect;
    quint16 m_dveFillSource;
    quint16 m_dveKeySource;
    bool m_dveKeyEnabled;
    bool m_dvePreMultipliedKeyEnabled;
    float m_dveKeyClip;
    float m_dveKeyGain;
    bool m_dveEnableInvertKey;
    bool m_dveReverseDirection;
    bool m_dveFlipFlopDirection;

    quint8 m_stingerSource;
    bool m_stingerPreMultipliedKeyEnabled;
    float m_stingerClip;
    float m_stingerGain;
    bool m_stingerInvertKeyEnabled;
    quint16 m_stingerPreRoll;
    quint16 m_stingerClipDuration;
    quint16 m_stingerTriggerPoint;
    quint16 m_stingerMixRate;

    QVector<QUpstreamKeySettings*> m_upstreamKeys;

    quint8 m_meID;

signals:
    void programInputChanged(quint8 me, quint16 oldIndex, quint16 newIndex);
    void previewInputChanged(quint8 me, quint16 oldIndex, quint16 newIndex);

    void transitionPreviewChanged(quint8 me, bool state);
    void transitionFrameCountChanged(quint8 me, quint8 count);
    void transitionPositionChanged(quint8 me, quint16 count);
    void nextTransitionStyleChanged(quint8 me, quint8 style);
    void keyersOnNextTransitionChanged(quint8 me, quint8 keyers);
    void currentTransitionStyleChanged(quint8 me, quint8 style);
    void keyersOnCurrentTransitionChanged(quint8 me, quint8 keyers);

    ///@p fading is true while fading to/from black, @p enabled is true when program is faded to black
    void fadeToBlackChanged(quint8 me, bool fading, bool enabled);
    void fadeToBlackFrameCountChanged(quint8 me, quint8 count);
    void fadeToBlackFramesChanged(quint8 me, quint8 frames);

    void fadeToBlackStatusChanged(FTBStatus status);

    void mixFramesChanged(quint8 me, quint8 frames);

    void dipFramesChanged(quint8 me, quint8 frames);
    void dipSourceChanged(quint8 me, quint16 source);

    void wipeFramesChanged(quint8 me, quint8 frames);
    void wipeBorderWidthChanged(quint8 me, quint16 width);
    void wipeBorderSoftnessChanged(quint8 me, quint16 softness);
    void wipeTypeChanged(quint8 me, quint8 type);
    void wipeSymmetryChanged(quint8 me, quint16 value);
    void wipeXPositionChanged(quint8 me, quint16 value);
    void wipeYPositionChanged(quint8 me, quint16 value);
    void wipeReverseDirectionChanged(quint8 me, bool reverse);
    void wipeFlipFlopChanged(quint8 me, bool flipFlop);
    void wipeBorderSourceChanged(quint8 me, quint16 index);

    void dveRateChanged(quint8 me, quint16 frames);
    void dveEffectChanged(quint8 me, quint8 effect);
    void dveFillSourceChanged(quint8 me, quint16 source);
    void dveKeySourceChanged(quint8 me, quint16 source);
    void dveEnableKeyChanged(quint8 me, bool enabled);
    void dveEnablePreMultipliedKeyChanged(quint8 me, bool enabled);
    void dveKeyClipChanged(quint8 me, float clip);
    void dveKeyGainChanged(quint8 me, float gain);
    void dveEnableInvertKeyChanged(quint8 me, bool enabled);
    void dveReverseDirectionChanged(quint8 me, bool reverse);
    void dveFlipFlopDirectionChanged(quint8 me, bool flipFlop);

    void stingerSourceChanged(quint8 me, quint8 source);
    void stingerEnablePreMultipliedKeyChanged(quint8 me, bool enabled);
    void stingerClipChanged(quint8 me, float percent);
    void stingerGainChanged(quint8 me, float percent);
    void stingerEnableInvertKeyChanged(quint8 me, bool enabled);
    void stingerPreRollChanged(quint8 me, quint16 frames);
    void stingerClipDurationChanged(quint8 me, quint16 frames);
    void stingerTriggerPointChanged(quint8 me, quint16 frames);
    void stingerMixRateChanged(quint8 me, quint16 frames);

    void upstreamKeyOnAirChanged(quint8 me, quint8 keyer, bool state);
    void upstreamKeyTypeChanged(quint8 me, quint8 keyer, quint8 type);
    void upstreamKeyFillSourceChanged(quint8 me, quint8 keyer, quint16 source);
    void upstreamKeyKeySourceChanged(quint8 me, quint8 keyer, quint16 source);
    void upstreamKeyEnableMaskChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyTopMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyBottomMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyLeftMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyRightMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyLumaPreMultipliedKeyChanged(quint8 me, quint8 keyer, bool preMultiplied);
    void upstreamKeyLumaInvertKeyChanged(quint8 me, quint8 keyer, bool invert);
    void upstreamKeyLumaClipChanged(quint8 me, quint8 keyer, float clip);
    void upstreamKeyLumaGainChanged(quint8 me, quint8 keyer, float gain);
    void upstreamKeyChromaHueChanged(quint8 me, quint8 keyer, float hue);
    void upstreamKeyChromaGainChanged(quint8 me, quint8 keyer, float gain);
    void upstreamKeyChromaYSuppressChanged(quint8 me, quint8 keyer, float ySuppress);
    void upstreamKeyChromaLiftChanged(quint8 me, quint8 keyer, float lift);
    void upstreamKeyChromaNarrowRangeChanged(quint8 me, quint8 keyer, bool narrowRange);
    void upstreamKeyPatternPatternChanged(quint8 me, quint8 keyer, quint8 pattern);
    void upstreamKeyPatternInvertPatternChanged(quint8 me, quint8 keyer, bool invert);
    void upstreamKeyPatternSizeChanged(quint8 me, quint8 keyer, float size);
    void upstreamKeyPatternSymmetryChanged(quint8 me, quint8 keyer, float symmetry);
    void upstreamKeyPatternSoftnessChanged(quint8 me, quint8 keyer, float softness);
    void upstreamKeyPatternXPositionChanged(quint8 me, quint8 keyer, float xPosition);
    void upstreamKeyPatternYPositionChanged(quint8 me, quint8 keyer, float yPosition);
    void upstreamKeyDVEXPositionChanged(quint8 me, quint8 keyer, float xPosition);
    void upstreamKeyDVEYPositionChanged(quint8 me, quint8 keyer, float yPosition);
    void upstreamKeyDVEXSizeChanged(quint8 me, quint8 keyer, float xSize);
    void upstreamKeyDVEYSizeChanged(quint8 me, quint8 keyer, float ySize);
    void upstreamKeyDVERotationChanged(quint8 me, quint8 keyer, float rotation);
    void upstreamKeyDVEEnableDropShadowChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyDVELighSourceDirectionChanged(quint8 me, quint8 keyer, float direction);
    void upstreamKeyDVELightSourceAltitudeChanged(quint8 me, quint8 keyer, quint8 altitude);
    void upstreamKeyDVEEnableBorderChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyDVEBorderStyleChanged(quint8 me, quint8 keyer, quint8 style);
    void upstreamKeyDVEBorderColorChanged(quint8 me, quint8 keyer, QColor color);
    void upstreamKeyDVEBorderOutsideWidthChanged(quint8 me, quint8 keyer, float width);
    void upstreamKeyDVEBorderInsideWidthChanged(quint8 me, quint8 keyer, float width);
    void upstreamKeyDVEBorderOutsideSoftenChanged(quint8 me, quint8 keyer, quint8 soften);
    void upstreamKeyDVEBorderInsideSoftenChanged(quint8 me, quint8 keyer, quint8 soften);
    void upstreamKeyDVEBorderOpacityChanged(quint8 me, quint8 keyer, quint8 opacity);
    void upstreamKeyDVEBorderBevelPositionChanged(quint8 me, quint8 keyer, float position);
    void upstreamKeyDVEBorderBevelSoftenChanged(quint8 me, quint8 keyer, float soften);
    void upstreamKeyDVERateChanged(quint8 me, quint8 keyer, quint8 rate);
    void upstreamKeyDVEKeyFrameASetChanged(quint8 me, quint8 keyer, bool set);
    void upstreamKeyDVEKeyFrameBSetChanged(quint8 me, quint8 keyer, bool set);
    void upstreamKeyEnableFlyChanged(quint8 me, quint8 keyer, bool enabled);
    void upstreamKeyDVEKeyFrameChanged(quint8 me, quint8 keyer, quint8 frame);
    void upstreamKeyDVEMaskEnabledChanged(quint8 me, quint8 keyer, bool enabled);
    void upstreamKeyDVEMaskTopChanged(quint8 me, quint8 keyer, float top);
    void upstreamKeyDVEMaskBottomChanged(quint8 me, quint8 keyer, float bottom);
    void upstreamKeyDVEMaskLeftChanged(quint8 me, quint8 keyer, float left);
    void upstreamKeyDVEMaskRightChanged(quint8 me, quint8 keyer, float right);
    void meIdChanged(quint8 meID);
};

#endif // QATEMMIXEFFECT_H
