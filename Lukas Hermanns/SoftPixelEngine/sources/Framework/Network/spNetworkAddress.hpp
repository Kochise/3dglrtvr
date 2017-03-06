/*
 * Network address header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_NETWORK_ADDRESS_H__
#define __SP_NETWORK_ADDRESS_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


#include "Framework/Network/spNetworkCore.hpp"


namespace sp
{
namespace network
{


//! Network address class which stores the network port and IP address.
class SP_EXPORT NetworkAddress
{
    
    public:
        
        NetworkAddress(const sockaddr_in &SocketAddress);
        NetworkAddress(u16 Port, u32 IPAddress = 0);
        NetworkAddress(u16 Port, const io::stringc &IPAddress);
        ~NetworkAddress();
        
        /* Functions */
        
        //! Returns the IP address name (e.g. "192.168.2.1").
        io::stringc getIPAddressName() const;
        
        /* Inline functions */
        
        //! Returns the IP address value.
        inline u32 getIPAddress() const
        {
            return Addr_.sin_addr.s_addr;
        }
        
        //! Returns the network port.
        inline u16 getPort() const
        {
            return Addr_.sin_port;
        }
        
        inline const sockaddr_in& getSocketAddress() const
        {
            return Addr_;
        }
        inline sockaddr_in& getSocketAddress()
        {
            return Addr_;
        }
        
    private:
        
        /* Members */
        
        sockaddr_in Addr_;
        
};


} // /namespace network

} // /namespace sp


#endif

#endif



// ================================================================================
