/*
 * Sound interface file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Sound/spSound.hpp"
#include "Base/spInputOutputLog.hpp"
#include "Base/spInputOutputFileSystem.hpp"


namespace sp
{
namespace audio
{


Sound::Sound(const ESoundModes Mode) : Mode_(Mode)
{
    /* Default settings */
    Radius_         = 10.0f;
    Volume_         = 1.0f;
    Balance_        = 0.0f;
    Speed_          = 1.0f;
    Seek_           = 0.0f;
    
    isPaused_       = false;
    isPlaying_      = false;
    isLooped_       = false;
    isVolumetric_   = false;
    isRecording_    = false;
    
    hasSeekChanged_ = false;
}
Sound::~Sound()
{
}

void Sound::play(bool Looped)
{
    isPlaying_  = true;
    isPaused_   = false;
    isLooped_   = Looped;
}
void Sound::pause(bool Paused)
{
    isPaused_   = Paused;
}
void Sound::stop()
{
    isPlaying_  = false;
    isLooped_   = false;
}

bool Sound::saveRecord(const io::stringc &Filename)
{
    return false;
}
void Sound::beginRecording() { }
void Sound::endRecording() { }

void Sound::setVolumetric(bool isVolumetric)
{
    isVolumetric_ = isVolumetric;
    setVolume(Volume_);
}

void Sound::setVolumetricRadius(f32 Radius)
{
    Radius_ = Radius;
}

void Sound::setPosition(const dim::vector3df &Position)
{
    Position_ = Position;
}


/*
 * ======= Private: =======
 */

bool Sound::checkFile(const io::stringc &Filename) const
{
    /* Information message */
    io::Log::message("Load sound: \"" + Filename + "\"");
    io::Log::upperTab();
    
    /* Check if the file does exist */
    if (!io::FileSystem().findFile(Filename))
    {
        io::Log::error("Could not found sound file");
        io::Log::lowerTab();
        return false;
    }
    
    io::Log::lowerTab();
    
    return true;
}


} // /namespace audio

} // /namespace sp



// ================================================================================
