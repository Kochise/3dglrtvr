/*
 * Dummy sound file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Sound/spDummySound.hpp"


namespace sp
{
namespace audio
{


DummySound::DummySound(const ESoundModes Mode) : Sound(Mode)
{
}
DummySound::~DummySound()
{
}

bool DummySound::reload(const io::stringc &Filename, u8 BufferCount)
{
    if (checkFile(Filename))
    {
        Filename_ = Filename;
        return true;
    }
    
    return false;
}
void DummySound::close()
{
}

void DummySound::setSeek(f32 Seek)
{
}
f32 DummySound::getSeek() const
{
    return 0.0f;
}

void DummySound::setVolume(f32 Volume)
{
    Volume_ = Volume;
}
void DummySound::setSpeed(f32 Speed)
{
    Speed_ = Speed;
}
void DummySound::setBalance(f32 Balance)
{
    Balance_ = Balance;
}

f32 DummySound::getLength() const
{
    return 0.0f;
}
bool DummySound::valid() const
{
    return false;
}


} // /namespace audio

} // /namespace sp



// ================================================================================
