/*
 * XML parser file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Tools/spToolXMLParser.hpp"

#ifdef SP_COMPILE_WITH_XMLPARSER


namespace sp
{
namespace tool
{


//#define __XML_DEBUG__
//#define __XML_OUTPUT_DEBUG__

void __debPrintXMLBlock__(const SXMLBlock &block)
{
    io::Log::message(block.Name);
    for (std::list<SXMLBlock>::const_iterator it = block.Blocks.begin(); it != block.Blocks.end(); ++it)
        __debPrintXMLBlock__(*it);
}

XMLParser::XMLParser() : FileSys_(0), File_(0)
{
    FileSys_ = new io::FileSystem();
}
XMLParser::~XMLParser()
{
    MemoryManager::deleteMemory(FileSys_);
}

bool XMLParser::loadFile(const io::stringc &Filename)
{
    CurLineNr_      = 0;
    MainBlock_.Name = "MainBlock";
    MainBlock_.Text = "";
    
    MainBlock_.Blocks.clear();
    MainBlock_.Attributes.clear();
    
    CurBlock_ = &MainBlock_;
    
    if ( !( File_ = FileSys_->openFile(Filename, io::FILE_READ) ) )
        return false;
    
    io::Log::message("Load XML file: \"" + Filename + "\"");
    io::Log::upperTab();
    
    bool Result = readXML();
    
    if (!Result)
        io::Log::error("Loading XML failed");
    
    FileSys_->closeFile(File_);
    io::Log::lowerTab();
    
    #ifdef __XML_OUTPUT_DEBUG__
    __debPrintXMLBlock__(MainBlock_);
    #endif
    
    return Result;
}


/*
 * ========== Protected: ==========
 */

bool XMLParser::readXML()
{
    bool isComment = false;
    s32 pos1, pos2;
    io::stringc NoneCommentStr;
    Line_ = "";
    
    while (1)
    {
        if (isComment)
        {
            // Read next line in file
            if (!readNextLine())
                break;
            
            pos1 = Line_.find("-->", 0);
            
            if (pos1 != -1)
            {
                isComment = false;
                Line_ = NoneCommentStr + Line_.right(Line_.size() - pos1 - 3);
            }
        }
        else
        {
            // Find XML code (<...>) or text.
            pos1 = Line_.find("<", 0);
            pos2 = Line_.find(">", 0);
            
            if (pos1 != -1 && pos2 != -1)
            {
                if (pos1 < pos2) // < ... > (block code)
                {
                    if (!readAttributes(pos1 + 1, pos2))
                        return false;
                }
                else if (pos1 > pos2) // > ... <
                {
                    if (!readText(pos2 + 1, pos1))
                        return false;
                }
                
                Line_ = Line_.right(Line_.size() - math::Max(pos1, pos2));
            }
            else
            {
                // Read next line in file
                if (!readNextLine())
                    break;
                
                // Search for comments
                pos1 = Line_.find("<!--", 0);
                
                if (pos1 != -1)
                {
                    isComment = true;
                    NoneCommentStr = Line_.left(pos1);
                }
            }
        }
    }
    
    return true;
}

bool XMLParser::readNextLine()
{
    ++CurLineNr_;
    Line_ += File_->readString();
    return !File_->isEOF();
}

bool XMLParser::readAttributes(s32 Start, s32 End)
{
    s32 pos1, pos2;
    
    io::stringc Trimed  = Line_.section(Start - 1, End + 1).trim();
    io::stringc Str     = Line_.section(Start, End);
    io::stringc Attribute, AttributeTrim;
    
    bool PushBlock  = (Trimed.find("</", 0) == -1);
    bool PopBlock   = (!PushBlock || Trimed.find("/>", 0) != -1);
    
    if (!PushBlock)
    {
        if (PopBlock)
            popBlockStack();
        return true;
    }
    
    pushBlockStack();
    
    // Read block's name
    pos2 = getNextTokenPos(Str);
    CurBlock_->Name = Str.left(pos2);
    
    #ifdef __XML_DEBUG__
    if (PushBlock)
        io::printMessage("Name: " + io::stringc().create('\t', BlockStack_.size()) + CurBlock_->Name);
    #endif
    
    // Read block's attributes
    while ( ( pos1 = getNextTokenPos(Str, pos2 + 1) ) > pos2 )
    {
        Attribute       = Str.section(pos2, pos1);
        AttributeTrim   = Attribute.trim();
        
        if (AttributeTrim == "/" || !AttributeTrim.size())
            break;
        
        if (!addAttributeToBlock(Attribute))
            return false;
        
        pos2 = pos1;
    }
    
    if (PopBlock)
        popBlockStack();
    
    return true;
}

bool XMLParser::readText(s32 Start, s32 End)
{
    // Read normal text
    CurBlock_->Text = Line_.section(Start, End);
    
    // Prepare text for special characters
    CurBlock_->Text = CurBlock_->Text.replace("&copy;", c8(169));
    
    return true;
}

s32 XMLParser::getNextTokenPos(const io::stringc &Str, u32 StartPos) const
{
    bool isInsideString = false;
    
    for (u32 i = StartPos; i < Str.size(); ++i)
    {
        if (Str[i] == '\"')
            isInsideString = !isInsideString;
        if ( !isInsideString && ( Str[i] == ' ' || Str[i] == '\t' || Str[i] == '\n' ) )
            return i;
    }
    
    return Str.size();
}

bool XMLParser::addAttributeToBlock(const io::stringc &Attribute)
{
    SXMLAttribute NewAttrib;
    
    s32 pos1, pos2;
    
    pos1 = Attribute.find("=", 0);
    
    if (pos1 == -1)
    {
        printErrorLI("Invalid attribute field");
        return false;
    }
    
    // Set attribue name
    NewAttrib.Name  = Attribute.left(pos1).trim();
    
    // Set attribue value
    NewAttrib.Value = Attribute.right(Attribute.size() - pos1 - 1);
    
    pos1 = NewAttrib.Value.find("\"", 0);
    
    if (pos1 != -1)
    {
        pos2 = NewAttrib.Value.find("\"", pos1 + 1);
        
        if (pos2 == -1)
        {
            printErrorLI("Incomplete string field");
            return false;
        }
        else
            NewAttrib.Value = NewAttrib.Value.section(pos1 + 1, pos2);
    }
    
    CurBlock_->Attributes.push_back(NewAttrib);
    
    #ifdef __XML_DEBUG__
    io::printMessage("Attrib: " + io::stringc().create('\t', BlockStack_.size()) + "[" + NewAttrib.Name + "] = [" + NewAttrib.Value + "]");
    #endif
    
    return true;
}

void XMLParser::pushBlockStack()
{
    CurBlock_->Blocks.resize(CurBlock_->Blocks.size() + 1);
    CurBlock_ = &CurBlock_->Blocks.back();
    
    BlockStack_.push_back(CurBlock_);
}

void XMLParser::popBlockStack()
{
    if (!BlockStack_.empty())
    {
        BlockStack_.pop_back();
        CurBlock_ = BlockStack_.back();
    }
    else
        CurBlock_ = &MainBlock_;
}

io::stringc XMLParser::getLinesIndicate() const
{
    return " (Line #" + io::stringc(CurLineNr_) + ")";
}

void XMLParser::printErrorLI(const io::stringc &Message) const
{
    io::Log::error(Message + getLinesIndicate());
}
void XMLParser::printWarningLI(const io::stringc &Message) const
{
    io::Log::warning(Message + getLinesIndicate());
}


} // /namespace tool

} // /namespace sp


#endif



// ================================================================================
