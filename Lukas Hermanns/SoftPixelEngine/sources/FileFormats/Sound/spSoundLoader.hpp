/*
 * Sound loader header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_AUDIO_SOUNDLOADER_H__
#define __SP_AUDIO_SOUNDLOADER_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_SOUNDSYSTEM)

#include "Base/spBaseFileFormatHandler.hpp"


namespace sp
{
namespace audio
{


//! Wave sound format structure.
struct SWaveFormat
{
    s16 Channels;
    s32 SamplesPerSec;
    s32 BytePerSec;
    s16 BlockAlign;
    s16 BitsPerSample;
};

struct SSoundData
{
    SSoundData() : WaveData(0), BufferSize(0)
    {
    }
    ~SSoundData()
    {
    }
    
    /* Members */
    c8* WaveData;
    u32 BufferSize;
    SWaveFromat Format;
};


class SP_EXPORT SoundLoader : public io::BaseFileFormatHandler
{
    
    public:
        
        virtual ~SoundLoader()
        {
        }
        
        /* Functions */
        
        virtual SSoundData* loadSoundData(const io::stringc &Filename) = 0;
        
    protected:
        
        SoundLoader() :
            io::BaseFileFormatHandler(), SoundData_(0)
        {
        }
        
        /* Members */
        
        SSoundData* SoundData_;
        
};


} // /namespace audio

} // /namespace sp


#endif

#endif



// ================================================================================
