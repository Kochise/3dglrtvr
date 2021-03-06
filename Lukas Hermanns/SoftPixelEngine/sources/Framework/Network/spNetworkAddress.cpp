/*
 * Network address file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Network/spNetworkAddress.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


namespace sp
{
namespace network
{


NetworkAddress::NetworkAddress(const sockaddr_in &SocketAddress)
{
    memcpy(&Addr_, &SocketAddress, sizeof(sockaddr_in));
}
NetworkAddress::NetworkAddress(u16 Port, u32 IPAddress)
{
    /* Fill address structure */
    memset(&Addr_, 0, sizeof(sockaddr_in));
    
    Addr_.sin_family        = AF_INET;
    Addr_.sin_port          = htons(Port);
    Addr_.sin_addr.s_addr   = IPAddress;
}
NetworkAddress::NetworkAddress(u16 Port, const io::stringc &IPAddress)
{
    /* Fill address structure */
    memset(&Addr_, 0, sizeof(sockaddr_in));
    
    Addr_.sin_family        = AF_INET;
    Addr_.sin_port          = htons(Port);
    Addr_.sin_addr.s_addr   = inet_addr(IPAddress.c_str());
}
NetworkAddress::~NetworkAddress()
{
}

io::stringc NetworkAddress::getIPAddressName() const
{
    io::stringc Str;
    
    const u8* Addr = (const u8*)&Addr_.sin_addr.s_addr;
    
    Str += io::stringc((s32)Addr[0]) + ".";
    Str += io::stringc((s32)Addr[1]) + ".";
    Str += io::stringc((s32)Addr[2]) + ".";
    Str += io::stringc((s32)Addr[3]);
    
    return Str;
}


} // /namespace network

} // /namespace sp


#endif



// ================================================================================
