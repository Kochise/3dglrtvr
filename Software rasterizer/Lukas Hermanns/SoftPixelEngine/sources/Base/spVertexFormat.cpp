/*
 * Vertex format file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spVertexFormat.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"
#include "RenderSystem/Direct3D11/spDirect3D11RenderSystem.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace video
{


VertexFormat::VertexFormat() : Flags_(0), InputLayout_(0)
{
}
VertexFormat::~VertexFormat()
{
    /* Delete vertex input layout (only used for Direct3D11) */
    __spVideoDriver->updateVertexInputLayout(this, false);
}

io::stringc VertexFormat::getIdentifier() const
{
    if (Name_.size())
        return "vertex format \"" + Name_ + "\"";
    return "anonymous vertex format";
}

u32 VertexFormat::getFormatSize() const
{
    u32 Size;
    getFormatSize(Size);
    return Size;
}

s32 VertexFormat::getDataTypeSize(const ERendererDataTypes Type)
{
    static const s32 SizeList[] = { 4, 8, 1, 2, 4, 1, 2, 4 };
    return SizeList[Type];
}


/*
 * ======= Protected: =======
 */

void VertexFormat::getFormatSize(u32 &Size) const
{
    Size = 0;
    
    if (Flags_ & VERTEXFORMAT_COORD)
        addComponentFormatSize(Size, Coord_);
    if (Flags_ & VERTEXFORMAT_COLOR)
        addComponentFormatSize(Size, Color_);
	if (Flags_ & VERTEXFORMAT_NORMAL)
        addComponentFormatSize(Size, Normal_);
	if (Flags_ & VERTEXFORMAT_BINORMAL)
        addComponentFormatSize(Size, Binormal_);
	if (Flags_ & VERTEXFORMAT_TANGENT)
        addComponentFormatSize(Size, Tangent_);
	if (Flags_ & VERTEXFORMAT_FOGCOORD)
        addComponentFormatSize(Size, FogCoord_);
	if (Flags_ & VERTEXFORMAT_TEXCOORDS)
	{
	    for (std::vector<SVertexAttribute>::const_iterator it = TexCoords_.begin(); it != TexCoords_.end(); ++it)
            addComponentFormatSize(Size, *it);
	}
	if (Flags_ & VERTEXFORMAT_UNIVERSAL)
	{
	    for (std::vector<SVertexAttribute>::const_iterator it = Universals_.begin(); it != Universals_.end(); ++it)
            addComponentFormatSize(Size, *it);
	}
}

void VertexFormat::addComponentFormatSize(u32 &Size, const SVertexAttribute &Attrib) const
{
    if (!Attrib.isReference)
        Size += Attrib.Size * getDataTypeSize(Attrib.Type);
}

void VertexFormat::constructComponent(
    const EVertexFormatFlags Type, SVertexAttribute &Attrib, s32 &Offset, s32 MinSize, s32 MaxSize)
{
    if ((Flags_ & Type) && !Attrib.isReference)
    {
        math::Clamp(Attrib.Size, MinSize, MaxSize);
        Attrib.Offset = Offset;
        Offset += getDataTypeSize(Attrib.Type) * Attrib.Size;
    }
}

void VertexFormat::constructFormat()
{
    if (__spVideoDriver->getRendererType() == RENDERER_DIRECT3D9)
    {
        Color_.Size = 4;
        Color_.Type = DATATYPE_UNSIGNED_BYTE;
    }
    
    s32 Offset = 0;
    
    constructComponent(VERTEXFORMAT_COORD, Coord_, Offset, 2, 4);
    constructComponent(VERTEXFORMAT_NORMAL, Normal_, Offset, 3, 3);
    constructComponent(VERTEXFORMAT_COLOR, Color_, Offset, 3, 4);
    
    for (std::vector<SVertexAttribute>::iterator it = TexCoords_.begin(); it != TexCoords_.end(); ++it)
        constructComponent(VERTEXFORMAT_TEXCOORDS, *it, Offset, 1, 4);
    
    constructComponent(VERTEXFORMAT_FOGCOORD, FogCoord_, Offset, 1, 1);
    constructComponent(VERTEXFORMAT_TANGENT, Tangent_, Offset, 3, 3);
    constructComponent(VERTEXFORMAT_BINORMAL, Binormal_, Offset, 3, 3);
    
    for (std::vector<SVertexAttribute>::iterator it = Universals_.begin(); it != Universals_.end(); ++it)
        constructComponent(VERTEXFORMAT_UNIVERSAL, *it, Offset, 1, 4);
    
    /* Update vertex input layout (only used for Direct3D11) */
    __spVideoDriver->updateVertexInputLayout(this, true);
}


} // /namespace video

} // /namespace sp



// ================================================================================
