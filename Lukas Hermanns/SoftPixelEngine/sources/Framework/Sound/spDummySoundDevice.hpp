/*
 * Dummy sound header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_AUDIO_SOUNDDEVICE_DUMMY_H__
#define __SP_AUDIO_SOUNDDEVICE_DUMMY_H__


#include "Base/spStandard.hpp"
#include "Framework/Sound/spSoundDevice.hpp"
#include "Framework/Sound/spDummySound.hpp"


namespace sp
{
namespace audio
{


class SP_EXPORT DummySoundDevice : public SoundDevice
{
        
    public:
            
        DummySoundDevice();
        ~DummySoundDevice();
        
        /* Functions */
        
        io::stringc getInterface() const;
        
        DummySound* createSound(const ESoundModes Mode = SOUND_DYNAMIC);
        
};


} // /namespace audio

} // /namespace sp


#endif



// ================================================================================
