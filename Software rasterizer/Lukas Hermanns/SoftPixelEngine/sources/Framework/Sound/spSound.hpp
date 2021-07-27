/*
 * Sound interface header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_AUDIO_SOUND_H__
#define __SP_AUDIO_SOUND_H__


#include "Base/spStandard.hpp"
#include "Base/spDimensionVector3D.hpp"
#include "Base/spInputOutputString.hpp"


namespace sp
{
namespace audio
{


/*
 * Macros
 */

static const s32 DEF_SOUND_BUFFERCOUNT = 3;


/*
 * Enumerations
 */

enum ESoundModes
{
    SOUND_STATIC,
    SOUND_DYNAMIC,
};

enum ESoundQuality
{
    SOUND_QUALITY_LOW,      //!< Low sound quality. 8000 Hz is used.
    SOUND_QUALITY_MEDIUM,   //!< Medium sound quality. 11025 Hz is used.
    SOUND_QUALITY_HIGH,     //!< High sound quality. 22050 Hz is used.
    SOUND_QUALITY_EXTREME,  //!< Extreme sound quality. 44100 Hz is used.
};


/*
 * Sound root class
 */

class SP_EXPORT Sound
{
    
    public:
        
        virtual ~Sound();
        
        /* === Functions === */
        
        virtual bool reload(const io::stringc &Filename, u8 BufferCount = DEF_SOUND_BUFFERCOUNT) = 0;
        virtual void close() = 0;
        
        /**
        Starts playing the sound. The sound will be played from the beginning.
        \param Looped: If true the sound will be played in an endless loop.
        */
        virtual void play(bool Looped = false);
        
        //! Pauses (if "Paused" is true) or resumes (if "Paused" if false) the sound.
        virtual void pause(bool Paused = true);
        
        //! Stops playing the sound.
        virtual void stop();
        
        /**
        Sets the seek or status of the sound. To use this function "play" needs not to be called.
        \param Seek: Specifies the status. Must lie between 0.0 and 1.0.
        */
        virtual void setSeek(f32 Seek) = 0;
        virtual f32 getSeek() const = 0;
        
        /**
        Sets the volume of the sound.
        \param Volume: Specifies the volume for the sound. Must lie between 0.0 (quiet) and 1.0 (loud).
        By default 1.0.
        */
        virtual void setVolume(f32 Volume) = 0;
        
        /**
        Sets the speed or frequency of the sound.
        \param Speed: Specifies the speed. Must lie between 0.0 (slow, where with 0.0 the sound stops to play
        and 2.0 (fast). By default 1.0 (normal). Backwards playing is not possible!
        */
        virtual void setSpeed(f32 Speed) = 0;
        
        /**
        Sets the balance of the sound.
        \param Balance: Specifies the balance between two sound-output devices (the sound boxes).
        Must lie between -1.0 (left) and 1.0 (right). By default 0.0 (in the middle, both boxes have the
        same volume).
        */
        virtual void setBalance(f32 Balance) = 0;
        
        //! Returns the length of the sound.
        virtual f32 getLength() const = 0;
        
        //! Returns true if the object is a valid sound. Otherwise no sound file has been loaded or could not load.
        virtual bool valid() const = 0;
        
        virtual void setPosition(const dim::vector3df &Position);
        
        virtual void setVolumetric(bool isVolumetric);
        virtual void setVolumetricRadius(f32 Radius);
        
        /**
        Saves the record sound as a wave (WAV) file to the disk. A sound needs to be recorded using
        the "beginRecording" and "endRecording" before this function can be used.
        \param Filename: Filename or path where the sound file is to be saved.
        \return True if the sound could be saved.
        */
        virtual bool saveRecord(const io::stringc &Filename);
        
        //! Begins to record a sound from an sound-input device (microphone).
        virtual void beginRecording();
        
        //! Finish the recording sound. After recording a sound it can be saved as a wave (WAV) file.
        virtual void endRecording();
        
        /* === Inline functions === */
        
        inline u8 getBufferCount() const
        {
            return BufferCount_;
        }
        inline io::stringc getFilename() const
        {
            return Filename_;
        }
        
        inline bool playing() const
        {
            return isPlaying_;
        }
        inline bool finish() const
        {
            return getSeek() >= 1.0f;
        }
        
        inline bool paused() const
        {
            return isPaused_;
        }
        inline bool looped() const
        {
            return isLooped_;
        }
        inline bool recording() const
        {
            return isRecording_;
        }
        
        inline bool getVolumetric() const
        {
            return isVolumetric_;
        }
        inline f32 getVolumetricRadius() const
        {
            return Radius_;
        }
        
        inline dim::vector3df getPosition() const
        {
            return Position_;
        }
        
        inline f32 getSpeed() const
        {
            return Speed_;
        }
        inline f32 getVolume() const
        {
            return Volume_;
        }
        inline f32 getBalance() const
        {
            return Balance_;
        }
        
    protected:
        
        friend class SoundDevice;
        
        /* Header */
        
        Sound(const ESoundModes Mode);
        
        /* Functions */
        
        bool checkFile(const io::stringc &Filename) const;
        
        /* Members */
        
        ESoundModes Mode_;
        io::stringc Filename_;
        u8 BufferCount_;
        
        f32 Radius_, Volume_, Balance_, Speed_, Seek_;
        bool isPaused_, isLooped_, isPlaying_, isVolumetric_, isRecording_;
        
        dim::vector3df Position_;
        bool hasSeekChanged_;
        
};


} // /namespace audio

} // /namespace sp


#endif



// ================================================================================
