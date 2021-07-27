/*
 * Network socket header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_NETWORK_SOCKET_H__
#define __SP_NETWORK_SOCKET_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


#include "Framework/Network/spNetworkCore.hpp"


//! Internet protocols.
enum EInternetProtocols
{
    PROTOCOL_TCP,   //!< Transmission Control Protocol (TCP/IP). This is a safe protocol but slower.
    PROTOCOL_UDP,   //!< User Datagram Protocol (UDP/IP). This is an unsafe protocol but faster.
};


namespace sp
{
namespace network
{


//! Network socket class which will be used to communicate between the clients and the server.
class SP_EXPORT NetworkSocket
{
    
    public:
        
        ~NetworkSocket();
        
        /* Inline functions */
        
        SOCKET getSocket() const
        {
            return Socket_;
        }
        
    private:
        
        friend class NetworkSystem;
        
        /* Functions */
        
        NetworkSocket(const EInternetProtocols Protocol, SOCKET Socket = INVALID_SOCKET);
        
        void createSocket(const EInternetProtocols Protocol);
        
        /* Members */
        
        SOCKET Socket_;
        
};


} // /namespace network

} // /namespace sp


#endif

#endif



// ================================================================================
