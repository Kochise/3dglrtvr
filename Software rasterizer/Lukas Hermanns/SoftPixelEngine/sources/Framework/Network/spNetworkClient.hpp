/*
 * Network client header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_NETWORK_CLIENT_H__
#define __SP_NETWORK_CLIENT_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


#include "Framework/Network/spNetworkSocket.hpp"
#include "Framework/Network/spNetworkAddress.hpp"
#include "Base/spThreadManager.hpp"


namespace sp
{
namespace network
{


//! Network client class for client handling. For the clients there will be a client object which is actually the server.
class SP_EXPORT NetworkClient
{
    
    public:
        
        ~NetworkClient();
        
        /* Inline functions */
        
        //! Returns true if this client is actually the server.
        inline bool isServer() const
        {
            return isServer_;
        }
        
        //! Returns the NetworkSocket object.
        inline NetworkSocket* getSocket() const
        {
            return Socket_;
        }
        
        //! Returns the NetworkAddress object.
        inline NetworkAddress* getAddress() const
        {
            return Address_;
        }
        
    private:
        
        friend class NetworkSystem;
        
        /* Functions */
        
        NetworkClient(
            NetworkAddress* Address, NetworkSocket* Socket = 0, bool isServer = false
        );
        
        /* Members */
        
        NetworkAddress* Address_;
        NetworkSocket* Socket_;
        
        bool isServer_;
        
        static u16 IDCounter_;
        
};


} // /namespace network

} // /namespace sp


#endif

#endif



// ================================================================================
