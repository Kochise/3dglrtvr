/*
 * Network socket file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Network/spNetworkSocket.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


namespace sp
{
namespace network
{


NetworkSocket::NetworkSocket(const EInternetProtocols Protocol, SOCKET Socket) : Socket_(Socket)
{
    if (Socket_ == INVALID_SOCKET)
    {
        /* Create socket object */
        switch (Protocol)
        {
            case PROTOCOL_TCP:
                Socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                break;
            case PROTOCOL_UDP:
                Socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                break;
        }
        
        /* Check for errors */
        if (Socket_ == INVALID_SOCKET)
        {
            #if defined(SP_PLATFORM_WINDOWS)
            io::Log::error("Could not create socket (error code = " + io::stringc(WSAGetLastError()) + ")");
            #elif defined(SP_PLATFORM_LINUX)
            io::Log::error("Could not create socket");
            #endif
        }
    }
}
NetworkSocket::~NetworkSocket()
{
    /* Delete socket object */
    #if defined(SP_PLATFORM_WINDOWS)
    closesocket(Socket_);
    #elif defined(SP_PLATFORM_LINUX)
    close(Socket_);
    #endif
}


} // /namespace network

} // /namespace sp


#endif



// ================================================================================
