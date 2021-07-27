/*
 * XML parser header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_TOOL_XMLPARSER_H__
#define __SP_TOOL_XMLPARSER_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_XMLPARSER


#include "Base/spInputOutputString.hpp"
#include "Base/spInputOutputFileSystem.hpp"

#include <list>
#include <vector>


namespace sp
{
namespace tool
{


struct SXMLAttribute
{
    io::stringc Name, Value;
};

struct SXMLBlock
{
    io::stringc Name, Text;
    std::list<SXMLAttribute> Attributes;
    std::list<SXMLBlock> Blocks;
};


class SP_EXPORT XMLParser
{
    
    public:
        
        XMLParser();
        virtual ~XMLParser();
        
        /* Functions */
        
        /**
        Loads an XML file (such as HTML websites).
        \param Filename: XML filename which is to be loaded.
        \return SXMLBlock structure which represents the main block wherein all the data is stored.
        */
        bool loadFile(const io::stringc &Filename);
        
        //! Returns the main block which will be generated after reading an XML file.
        inline SXMLBlock getMainBlock() const
        {
            return MainBlock_;
        }
        
    protected:
        
        /* Functions */
        
        bool readXML();
        bool readNextLine();
        
        bool readAttributes(s32 Start, s32 End);
        bool readText(s32 Start, s32 End);
        
        s32 getNextTokenPos(const io::stringc &Str, u32 StartPos = 0) const;
        bool addAttributeToBlock(const io::stringc &Attribute);
        
        void pushBlockStack();
        void popBlockStack();
        
        io::stringc getLinesIndicate() const;
        void printErrorLI(const io::stringc &Message) const;
        void printWarningLI(const io::stringc &Message) const;
        
        /* === Members === */
        
        io::FileSystem* FileSys_;
        io::File* File_;
        
        io::stringc Line_;
        u32 CurLineNr_;
        
        std::list<SXMLBlock*> BlockStack_;
        SXMLBlock* CurBlock_;
        SXMLBlock MainBlock_;
        
};


} // /namespace tool

} // /namespace sp


#endif

#endif



// ================================================================================
