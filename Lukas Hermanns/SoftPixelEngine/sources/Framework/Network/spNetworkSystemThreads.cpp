/*
 * Network system thread file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "spNetworkSystem.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


namespace sp
{
namespace network
{


THREAD_PROC(NetworkThreadOpenServer)
{
    /* Get network system objects */
    NetworkSystem* NetSys   = (NetworkSystem*)Arguments;
    SOCKET Socket           = NetSys->Socket_->getSocket();
    
    /* Open network connection */
    if (bind(Socket, (sockaddr*)NetSys->Address_, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        io::Log::error("Server connection failed");
        return 0;
    }
    else
        io::Log::message("Server connection successful");
    
    /* Update own IP address */
    NetSys->LocalIP_ = NetSys->Address_->getIPAddress();
    
    /* Get maximum connections available at a time */
    #if defined(SP_PLATFORM_WINDOWS)
    const s32 Backlog = SOMAXCONN;
    #elif defined(SP_PLATFORM_LINUX)
    const s32 Backlog = 1024;
    #endif
    
    /* Listen for clients */
    if (listen(Socket, Backlog) == SOCKET_ERROR)
    {
        io::Log::error("Server listen status failed");
        return 0;
    }
    else
        io::Log::message("Server listen status successful");
    
    NetSys->isConnected_ = true;
    
    return 0;
}

THREAD_PROC(NetworkThreadJoinServer)
{
    /* Get network system objects */
    NetworkSystem* NetSys   = (NetworkSystem*)Arguments;
    SOCKET Socket           = NetSys->Socket_->getSocket();
    
    /* Join network connection */
    if (connect(Socket, (sockaddr*)&NetSys->Address_->getSocketAddress(), sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        io::Log::error("Joining server failed");
        return 0;
    }
    else
        io::Log::message("Joining server successful");
    
    /* Update own IP address */
    // ###### !TODO! ######
    //NetSys->LocalIP_ = NetSys->Address_->getIPAddress(); // <- this is the server IP address
    
    /* Create new client account for server */
    NetSys->createServerAccount();
    
    NetSys->isConnected_ = true;
    
    return 0;
}

THREAD_PROC(NetworkThreadAcceptClient)
{
    /* Get network system object */
    NetworkSystem* NetSys = (NetworkSystem*)Arguments;
    
    /* Wait until server connection thread was termianted */
    while (NetSys->ThreadOpenServer_->running())
        NetSys->yield();
    
    /* Get main socket */
    SOCKET Socket = NetSys->Socket_->getSocket();
    
    #if defined(SP_PLATFORM_WINDOWS)
    s32 AddressLen;
    #else
    socklen_t AddressLen;
    #endif
    
    /* Run for ever, thread scheduling is shifted to the outside */
    while (1)
    {
        /* Configure client structures */
        sockaddr_in Address;
        AddressLen = sizeof(sockaddr_in);
        
        /* Accept new clients */
        SOCKET ClientSocket = accept(Socket, (sockaddr*)&Address, &AddressLen);
        
        if (ClientSocket != INVALID_SOCKET)
        {
            /* Create new client account */
            NetworkClient* NewClient = NetSys->createClientAccount(ClientSocket, Address);
            
            if (NetSys->isServer_)
            {
                /* Tell your self about the new one */
                NetSys->RecvStack_.push_back(SNetworkPacket(PACKET_CLIENTJOIN, NewClient));
                
                /* Tell all other clients about the new one */
                NetSys->sendNetworkPacket(PACKET_CLIENTJOIN, 0, 0, 0, NewClient);
                
                /* Tell the new client about all the other clients */
                for (std::list<NetworkClient*>::iterator it = NetSys->ClientList_.begin(); it != NetSys->ClientList_.end(); ++it)
                {
                    if (*it != NewClient)
                        NetSys->sendNetworkPacket(PACKET_CLIENTJOIN, 0, 0, NewClient, *it);
                }
            }
            
            io::Log::message("Created new client account");
        }
        else
            io::Log::error("Client connection failed");
    }
    
    return 0;
}

THREAD_PROC(NetworkThreadSendPacket)
{
    /* Get network system object */
    NetworkSystem* NetSys = (NetworkSystem*)Arguments;
    
    /* Wait until server connection thread was termianted */
    while ( ( NetSys->ThreadOpenServer_ && NetSys->ThreadOpenServer_->running() ) ||
            ( NetSys->ThreadJoinServer_ && NetSys->ThreadJoinServer_->running() ) )
    {
        NetSys->yield();
    }
    
    /* Get main socket */
    SOCKET Socket = NetSys->Socket_->getSocket();
    
    /* Run for ever, thread scheduling is shifted to the outside */
    while (1)
    {
        /* Wait until there are new packets to send */
        while (NetSys->SendStack_.empty())
            NetSys->yield();
        
        /* Send first network packet */
        SNetworkPacket Packet = NetSys->SendStack_.front();
        
        if (NetSys->isServer_)
        {
            /* Send packet to one or each client */
            if (!Packet.Sender)
            {
                for (std::list<NetworkClient*>::iterator it = NetSys->ClientList_.begin(); it != NetSys->ClientList_.end(); ++it)
                {
                    /* Check if packet was originally send by the current client */
                    if (Packet.ClientOrigin != *it)
                        NetworkSystem::sendSocketPacket((*it)->getSocket()->getSocket(), Packet);
                }
            }
            else
                NetworkSystem::sendSocketPacket(Packet.Sender->getSocket()->getSocket(), Packet);
        }
        else
            NetworkSystem::sendSocketPacket(Socket, Packet);
        
        /* Delete sended message from the stack */
        Packet.deleteBuffer();
        NetSys->SendStack_.pop_front();
        
        /* Check if the disconnection was told other clients */
        if ( ( NetSys->isServer_ && Packet.Type == PACKET_SERVEROFF ) || ( !NetSys->isServer_ && Packet.Type == PACKET_CLIENTLEFT ) )
        {
            /* Wait a moment that the clients can receive their last packets for this session */
            NetSys->yield(100);
            break;
        }
    }
    
    return 0;
}

THREAD_PROC(NetworkThreadRecvPacket)
{
    /* Get network system object */
    NetworkSystem* NetSys = (NetworkSystem*)Arguments;
    
    /* Wait until server connection thread was termianted */
    while ( ( NetSys->ThreadOpenServer_ && NetSys->ThreadOpenServer_->running() ) ||
            ( NetSys->ThreadJoinServer_ && NetSys->ThreadJoinServer_->running() ) )
    {
        NetSys->yield();
    }
    
    /* Get main socket */
    SOCKET Socket = NetSys->Socket_->getSocket();
    SOCKET CurrentSocket = Socket;
    
    /* Allocate enough memory for receiving large packets */
    static u32 PACKETBUFFER_SIZE = 4098;
    c8* PacketBuffer = new c8[PACKETBUFFER_SIZE];
    
    /* Variables for handling current or temporary data */
    SNetworkPacket Packet;
    NetworkClient* CurrentClient = 0;
    s32 SelectionCount = 0;
    fd_set ReadFDS;
    
    /* Run for ever, thread scheduling is shifted to the outside */
    while (1)
    {
        if (NetSys->isServer_)
        {
            if (!SelectionCount)
            {
                /* Wait until at least one client has joined the server */
                while (NetSys->ClientList_.empty())
                    NetSys->yield();
                
                /* Initialize all client sockets for selection */
                FD_ZERO(&ReadFDS);
                
                SOCKET LastSocket = INVALID_SOCKET;
                
                NetSys->ClientList_.lock();
                
                for (std::list<NetworkClient*>::iterator it = NetSys->ClientList_.begin(); it != NetSys->ClientList_.end(); ++it)
                {
                    LastSocket = (*it)->getSocket()->getSocket();
                    FD_SET(LastSocket, &ReadFDS);
                }
                
                NetSys->ClientList_.unlock();
                
                /* Select all sockets which needs to be read */
                timeval TimeOut = { 5, 0 };
                
                s32 SelectCount = select(LastSocket + 1, &ReadFDS, 0, 0, &TimeOut);
                
                /* Check for client selection problems */
                if (SelectCount == SOCKET_ERROR)
                {
                    io::Log::error("Network client selection failed");
                    continue;
                }
                if (SelectCount <= 0)
                {
                    /* Time out over limit -> try again */
                    continue;
                }
                
                /* Process client socket reading */
                //SelectionCount = ReadFDS.fd_count; // <- SelectionCount is already the number of sockets to handle (for Windows and Linux)
                
                #if 1
                io::Log::message("DEB: SelectionCount = " + io::stringc(SelectionCount));
                #endif
            }
            
            /* Get next client socket to read */
            --SelectionCount;
            
            #if defined(SP_PLATFORM_WINDOWS)
            CurrentSocket = ReadFDS.fd_array[SelectionCount];
            #elif defined(SP_PLATFORM_LINUX)
            NetSys->ClientList_.lock();
            
            s32 i = 0;
            for (std::list<NetworkClient*>::iterator it = NetSys->ClientList_.begin(); it != NetSys->ClientList_.end(); ++it, ++i)
            {
                if (FD_ISSET(i, &ReadFDS))
                {
                    CurrentSocket = (*it)->getSocket()->getSocket();
                    break;
                }
            }
            
            NetSys->ClientList_.unlock();
            #endif
            
            CurrentClient = NetSys->ClientSocketMap_[CurrentSocket];
        }
        
        /* Receive a new network packet */
        s32 Bytes = recv(CurrentSocket, PacketBuffer, PACKETBUFFER_SIZE, 0);
        
        /* Check packet validity */
        if (Bytes == SOCKET_ERROR)
        {
            io::Log::error("Receiving network packet failed");
            
            if (NetSys->isServer_)
            {
                /* No client connection -> remove client object and tell it all the other clients */
                NetSys->sendNetworkPacket(PACKET_CLIENTLEFT, 0, 0, 0, CurrentClient);
                NetSys->removeClient(CurrentClient);
            }
            else
            {
                /* No server connection -> disconnect */
                NetSys->disconnect();
                return 0;
            }
            
            continue;
        }
        if (Bytes <= 0)
            continue;
        
        if (NetSys->isWebServerConnection_)
        {
            /* Get packet information */
            Packet.Type         = PACKET_WEBREQUEST;
            Packet.BufferSize   = Bytes + 1;
            Packet.Sender       = 0;
        }
        else
        {
            if (Bytes < 9)
            {
                io::Log::error("Received network packet is invalid");
                continue;
            }
            
            /* Get packet information (not the buffer yet, we don't know if it's for the local PC) */
            Packet.Type         = (ENetworkPacketTypes)PacketBuffer[8];
            u32 SenderIP        = *(u32*)PacketBuffer;
            u32 AcceptorIP      = *(u32*)(PacketBuffer + 4);
            Packet.BufferSize   = Bytes - 9;
            
            /* Get sender and acceptor object */
            NetworkClient* Acceptor = NetSys->ClientIPMap_[AcceptorIP];
            Packet.Sender           = (NetSys->isServer_ ? CurrentClient : NetSys->ClientIPMap_[SenderIP]);
            
            /*
            Check for internal packet processing:
             -> server needs to pass client packets
             -> clients need to be informed about new clients joining the server
            */
            if (NetSys->isServer_)
            {
                /* Check if the packet needs to be passed by the server to another client */
                if (AcceptorIP != NetSys->LocalIP_ || !Acceptor)
                {
                    NetSys->sendNetworkPacket(Packet.Type, PacketBuffer + 9, Packet.BufferSize, Acceptor, Packet.Sender);
                    if (AcceptorIP != NetSys->LocalIP_)
                        continue;
                }
            }
            else
            {
                /* Check if the server told you about a new other client */
                if (Packet.Type == PACKET_CLIENTJOIN)
                    NetSys->createClientAccount(SenderIP);
            }
            
            /* Check if a client left the server */
            if (Packet.Type == PACKET_CLIENTLEFT)
                NetSys->removeClient(Packet.Sender);
        }
        
        /* Copy buffer to have an own reference */
        if (Packet.BufferSize > 0)
        {
            Packet.Buffer = new c8[Packet.BufferSize];
            
            if (NetSys->isWebServerConnection_)
            {
                memcpy(Packet.Buffer, PacketBuffer, Packet.BufferSize);
                ((c8*)Packet.Buffer)[Bytes] = 0;
            }
            else
                memcpy(Packet.Buffer, PacketBuffer + 9, Packet.BufferSize);
        }
        else
            Packet.Buffer = 0;
        
        /* Add network packet to the receive stack */
        NetSys->RecvStack_.push_back(Packet);
    }
    
    return 0;
}


} // /namespace network

} // /namespace sp


#endif



// ================================================================================
