/*
 * Network system header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_NETWORK_SYSTEM_H__
#define __SP_NETWORK_SYSTEM_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


#include "Framework/Network/spNetworkCore.hpp"
#include "Framework/Network/spNetworkSocket.hpp"
#include "Framework/Network/spNetworkClient.hpp"
#include "Framework/Network/spNetworkAddress.hpp"
#include "Base/spThreadManager.hpp"


namespace sp
{
namespace network
{


static const u32 NETWORK_DEFPORT = 80;


//! Types of network packets.
enum ENetworkPacketTypes
{
    PACKET_CLIENTDATA = 0,  //!< Client sended data ("Buffer" points to client sended data).
    PACKET_CLIENTJOIN,      //!< Client has joined the server ("Buffer" points to the new NetworkClient object).
    PACKET_CLIENTLEFT,      //!< Client has left the server ("Buffer" points to the old memory address. Don't use this memory anymore!).
    PACKET_SERVEROFF,       //!< Server has disconnected.
    PACKET_WEBREQUEST,      //!< Packet is a web server request (e.g. to get website content).
};


//! Network packet containing data buffer and its sender.
struct SNetworkPacket
{
    SNetworkPacket(
        const ENetworkPacketTypes InitType = PACKET_CLIENTDATA, NetworkClient* InitSender = 0)
        : Type(InitType), Buffer(0), BufferSize(0), Sender(InitSender), ClientOrigin(0)
    {
    }
    ~SNetworkPacket()
    {
    }
    
    /* Functions */
    
    inline void deleteBuffer()
    {
        if (Buffer)
        {
            delete [] static_cast<c8*>(Buffer);
            Buffer = 0;
        }
    }
    
    /* Members */
    ENetworkPacketTypes Type;       //!< Packet type (normal client send packets are always of the tpye PACKET_CLIENTDATA).
    void* Buffer;                   //!< Packet buffer which is actual a byte array. You need to know by your self what type this buffer is (use pointer casting).
    u32 BufferSize;                 //!< Size (in bytes) of "Buffer".
    NetworkClient* Sender;          //!< Client who send the packet. Null if the packet comes from the server.
    NetworkClient* ClientOrigin;    //!< Used to pass packets from a client over the server to another client.
};


/**
NetworkSystem for local or inter network PC communication. This class uses multithreading. For the server there are continuous three
threads running and for clients two threads.
*/
class SP_EXPORT NetworkSystem
{
    
    public:
        
        NetworkSystem(const EInternetProtocols Protocol = PROTOCOL_TCP);
        ~NetworkSystem();
        
        /* === Functions === */
        
        //! Returns the network system version.
        io::stringc getVersion() const;
        
        /**
        Opens a new local server.
        \param Port: Specifies the network port. By default 80.
        */
        void openServer(u16 Port = NETWORK_DEFPORT);
        
        /**
        Joins an open local server.
        \param IPAddress: Specifies the server's IP address. If you want to pass the host name
        use "getHostIPAddress" to get the IP address out of the host name.
        \param Port: Specifies the network port. By default 80.
        \param isWebServerConnection: Specifies whether you join a web server or not.
        If you want to connect to a web server (such as "www.google.com") set this parameter to true
        and also use "getHostIPAddress".
        */
        void joinServer(const io::stringc &IPAddress, u16 Port = NETWORK_DEFPORT, bool isWebServerConnection = false);
        
        //! Disconnects the current running network session. If the server disconnects all clients will receive a disconnection message.
        void disconnect();
        
        /**
        Sends a network packet to the specified clients.
        \param Buffer: Specifies the buffer which is to be send. This can be any kind of buffer (e.g. '&YourStructuredObject' or 'int*').
        \param BufferSize: Specifies the size of the given buffer.
        \param Acceptor: Specifies the acceptor client. By default null which means that the packet is for all clients.
        \note There are NetworkClient objects for each network member also for the server.
        For the server client object the "isServer" function will return true.
        */
        void sendPacket(const void* Buffer, u32 BufferSize, NetworkClient* Acceptor = 0);
        
        /**
        Sends a string message to the specified clients.
        \param Message: Specifies the string message.
        \param Acceptor: Specifies the acceptor client.
        */
        void sendPacket(const io::stringc &Message, NetworkClient* Acceptor = 0);
        
        /**
        Sends a reqeust in form of a string message to the web server with which you are connected
        (if you are connected with a web server).
        \param Message: Specifies the request string message (e.g. "GET /index.html HTTP/1.1\r\nHost: www.google.com\r\n\r\n").
        You can also use "getServerRequest" for some standard requests.
        \see { openServer }
        */
        void sendWebServerRequest(const io::stringc &Message);
        
        /**
        Picks the last network packet which has been received.
        \param Packet: Specifies the packet reference in which the received data will be stored.
        \note After picking a packet you need to delete the buffer using "SNetworkPacket::deleteBuffer" (e.g. "MyPacket.deleteBuffer();").
        Otherwise all the memory will only deleted with disconnection.
        */
        bool pickPacket(SNetworkPacket &Packet);
        
        /**
        Returns the IP address of the specified host name.
        \param HostName: Specifies the host name (or rather local PC name).
        */
        io::stringc getHostIPAddress(const io::stringc &HostName) const;
        
        //! Returns the official host name (e.g. "www.google.com" to "www.l.google.com").
        io::stringc getOfficialHostName(const io::stringc &HostName) const;
        
        /**
        Gets all IP addresses of the specified host name.
        \param HostName: Specifies the host name (or rather local PC name):
        \return List of all IP addresses.
        */
        std::list<io::stringc> getHostIPAddressList(const io::stringc &HostName) const;
        
        /**
        Gets all network member host names (or rather local PC names).
        \return List of all host names.
        \note This function sometimes needs a couple of seconds and currently is only supported for MS/Windows.
        */
        std::list<io::stringc> getNetworkMembers() const;
        
        /**
        Gets a "GET" server request string to get file data from the specified server.
        \param HostName: Specifies the internet host name (e.g. "www.google.de").
        \param Filename: Specifies the filename you want to get (e.g. "/index.html").
        \return Server request (or rather command).
        */
        static io::stringc getServerRequest(const io::stringc &HostName, const io::stringc &Filename);
        
        /* === Inline functions === */
        
        //! Returns true if you are connected and you can send and receive packets.
        inline bool isConnected() const
        {
            return isConnected_;
        }
        
        //! Returns true if you are the server.
        inline bool isServer() const
        {
            return isServer_;
        }
        
        //! Returns true if you are connected to a web server.
        inline bool isWebServerConnection() const
        {
            return isWebServerConnection_;
        }
        
        /**
        Returns the NetworkClient object list.
        \note This is a "SecureList" for thread safe container operations. When you want to iterate over this list
        call "lock" before and "unlock" after iteration to be sure that the networt threads won't be able to
        have access to this list at the same time.
        */
        inline dim::SecureList<NetworkClient*> getClientList() const
        {
            return ClientList_;
        }
        
    private:
        
        /* === Friends === */
        
        friend THREAD_PROC(NetworkThreadOpenServer);
        friend THREAD_PROC(NetworkThreadJoinServer);
        friend THREAD_PROC(NetworkThreadAcceptClient);
        friend THREAD_PROC(NetworkThreadSendPacket);
        friend THREAD_PROC(NetworkThreadRecvPacket);
        
        /* === Functions === */
        
        #if defined(SP_PLATFORM_WINDOWS)
        void createWinSock();
        void deleteWinSock();
        #endif
        
        void yield(u32 Milliseconds = 1);
        
        void sendNetworkPacket(
            const ENetworkPacketTypes Type, const void* Buffer, u32 BufferSize,
            NetworkClient* Acceptor, NetworkClient* Sender = 0
        );
        
        NetworkClient* createServerAccount();
        NetworkClient* createClientAccount(SOCKET Socket, const sockaddr_in &Address);
        NetworkClient* createClientAccount(u32 IPAddress);
        
        NetworkClient* addClientAccount(NetworkClient* NewClient);
        
        void removeClient(NetworkClient* Client);
        
        static void sendSocketPacket(SOCKET Socket, const SNetworkPacket &Packet);
        
        /* === Members === */
        
        #if defined(SP_PLATFORM_WINDOWS)
        WSADATA WinSock_;
        #endif
        
        EInternetProtocols Protocol_;
        bool isServer_, isConnected_, isWebServerConnection_;
        u32 LocalIP_;
        u16 Port_;
        
        NetworkSocket* Socket_;
        NetworkAddress* Address_;
        
        ThreadManager* ThreadOpenServer_;
        ThreadManager* ThreadJoinServer_;
        ThreadManager* ThreadAcceptClient_;
        ThreadManager* ThreadSendPacket_;
        ThreadManager* ThreadRecvPacket_;
        
        dim::SecureList<NetworkClient*> ClientList_;
        std::map<u32, NetworkClient*> ClientIPMap_;
        std::map<SOCKET, NetworkClient*> ClientSocketMap_;
        
        dim::SecureList<SNetworkPacket> SendStack_, RecvStack_;
        
};


} // /namespace network

} // /namespace sp


#endif

#endif



// ================================================================================
