/*
 * Font header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_VIDEODRIVER_FONT_H__
#define __SP_VIDEODRIVER_FONT_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutputString.hpp"
#include "Base/spDimension.hpp"
#include "Base/spMaterialColor.hpp"
#include "RenderSystem/spTextureBase.hpp"


namespace sp
{
namespace video
{


//! Font creation flags
enum EFontFlags
{
    FONT_BOLD       = 0x01, //!< Bold type (text looks fetter).
    FONT_ITALIC     = 0x02, //!< Italic type (text looks cursive).
    FONT_UNDERLINED = 0x04, //!< Text is underlined.
    FONT_STRIKEOUT  = 0x08, //!< Text is striked out.
    FONT_SYMBOLS    = 0x10, //!< Text may contain special symbols.
};


class SP_EXPORT Font
{
    
    public:
        
        Font();
        Font(
            void* ID, const io::stringc &FontName,
            const dim::size2di &Size, const std::vector<s32> &CharWidthList,
            video::Texture* TextureMap = 0
        );
        ~Font();
        
        //! Returns the width of the specified text.
        s32 getStringWidth(const io::stringc &Text) const;
        
        //! Returns the height of the specified text with regard to the line breaks.
        s32 getStringHeight(const io::stringc &Text) const;
        
        /* Inline functions */
        
        //! Returns the font's ID. Used inside the respective renderer.
        inline void* getID() const
        {
            return ID_;
        }
        
        //! Returns the font's size.
        inline dim::size2di getSize() const
        {
            return Size_;
        }
        
        //! Returns the font's name (e.g. "Times New Roman").
        inline io::stringc getName() const
        {
            return FontName_;
        };
        
        //! Returns the size of the specified text.
        inline dim::size2di getStringSize(const io::stringc &Text) const
        {
            return dim::size2di(getStringWidth(Text), Size_.Height);
        }
        
        //! Returns the texture if this font is a texture font.
        inline video::Texture* getTexture() const
        {
            return Texture_;
        }
        
    private:
        
        #ifdef SP_COMPILE_WITH_DIRECT3D11
        friend class Direct3D11Driver;
        #endif
        #if defined(SP_COMPILE_WITH_OPENGL) || defined(SP_COMPILE_WITH_OPENGLES1)
        friend class GLFixedFunctionPipeline;
        #endif
        
        /* Members */
        
        void* ID_;
        io::stringc FontName_;
        dim::size2di Size_;
        
        std::vector<s32> CharWidthList_;
        
        video::Texture* Texture_;
        
};


} // /namespace video

} // /namespace sp


#endif



// ================================================================================
