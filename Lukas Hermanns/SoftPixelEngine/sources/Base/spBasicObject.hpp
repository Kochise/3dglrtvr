/*
 * Basic object header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_BASICOBJECT_H__
#define __SP_BASICOBJECT_H__


#include "Base/spStandard.hpp"


namespace sp
{


class SP_EXPORT BasicObject
{
    
    public:
        
        virtual ~BasicObject()
        {
        }
        
        /* Inline functions */
        
        /**
        Sets the user data.
        \param Data: Pointer to an individual data field which can be defined by the user.
        By default 0 when the pointer is unused.
        */
        inline void setUserData(void* Data)
        {
            UserData_ = Data;
        }
        
        /**
        \return Pointer to an individual data field which can be defined by the user.
        Use ANSI-C casting e.g. like: "MyDataStructure* MyData = (MyDataStructure*)MyNodeObject->getUserData();".
        The pointer type must be the same which has been set before with "setUserData".
        */
        inline void* getUserData() const
        {
            return UserData_;
        }
        
    protected:
        
        BasicObject() : UserData_(0)
        {
        }
        
    private:
        
        /* Members */
        
        void* UserData_;
        
};


} // /namespace sp


#endif



// ================================================================================
