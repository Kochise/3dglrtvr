/*
 * Network client file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Network/spNetworkClient.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


namespace sp
{
namespace network
{


NetworkClient::NetworkClient(NetworkAddress* Address, NetworkSocket* Socket, bool isServer)
    : Address_(Address), Socket_(Socket), isServer_(isServer)
{
    if (!Address_)
        io::Log::error("Network client without IP address");
}
NetworkClient::~NetworkClient()
{
    MemoryManager::deleteMemory(Socket_);
    MemoryManager::deleteMemory(Address_);
}


} // /namespace network

} // /namespace sp


#endif



// ================================================================================
