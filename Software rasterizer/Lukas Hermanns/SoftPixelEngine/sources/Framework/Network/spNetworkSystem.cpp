/*
 * Network system file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Framework/Network/spNetworkSystem.hpp"

#ifdef SP_COMPILE_WITH_NETWORKSYSTEM


#if defined(SP_PLATFORM_WINDOWS)
#   include <lm.h>
#endif


namespace sp
{
namespace network
{


THREAD_PROC(NetworkThreadOpenServer);
THREAD_PROC(NetworkThreadJoinServer);
THREAD_PROC(NetworkThreadAcceptClient);
THREAD_PROC(NetworkThreadSendPacket);
THREAD_PROC(NetworkThreadRecvPacket);


NetworkSystem::NetworkSystem(const EInternetProtocols Protocol)
    : Protocol_(Protocol), LocalIP_(0), Port_(0), Socket_(0), Address_(0),
    ThreadOpenServer_(0), ThreadJoinServer_(0), ThreadAcceptClient_(0), ThreadSendPacket_(0), ThreadRecvPacket_(0)
{
    io::Log::message(getVersion());
    
    /* Generel settings */
    isServer_               = false;
    isConnected_            = false;
    isWebServerConnection_  = false;
    
    #ifdef SP_PLATFORM_WINDOWS
    createWinSock();
    #endif
    
    io::Log::message("");
}
NetworkSystem::~NetworkSystem()
{
    disconnect();
    
    #ifdef SP_PLATFORM_WINDOWS
    deleteWinSock();
    #endif
}

io::stringc NetworkSystem::getVersion() const
{
    return "SmartNetworkSystem - v.3.0";
}

void NetworkSystem::openServer(u16 Port)
{
    /* General server settings */
    isServer_               = true;
    isWebServerConnection_  = false;
    Port_                   = Port;
    
    /* Create main socket */
    Socket_ = new NetworkSocket(Protocol_);
    
    /* Configure network address structure */
    MemoryManager::deleteMemory(Address_);
    Address_ = new NetworkAddress(Port);
    
    /* Start threads */
    ThreadOpenServer_   = new ThreadManager(NetworkThreadOpenServer,    this);
    ThreadAcceptClient_ = new ThreadManager(NetworkThreadAcceptClient,  this);
    ThreadSendPacket_   = new ThreadManager(NetworkThreadSendPacket,    this);
    ThreadRecvPacket_   = new ThreadManager(NetworkThreadRecvPacket,    this);
}

void NetworkSystem::joinServer(const io::stringc &IPAddress, u16 Port, bool isWebServerConnection)
{
    /* General server settings */
    isServer_               = false;
    isWebServerConnection_  = isWebServerConnection;
    Port_                   = Port;
    
    /* Create main socket */
    Socket_ = new NetworkSocket(Protocol_);
    
    /* Configure network address structure */
    MemoryManager::deleteMemory(Address_);
    Address_ = new NetworkAddress(Port, IPAddress);
    
    /* Start threads */
    ThreadJoinServer_ = new ThreadManager(NetworkThreadJoinServer, this);
    ThreadSendPacket_ = new ThreadManager(NetworkThreadSendPacket, this);
    ThreadRecvPacket_ = new ThreadManager(NetworkThreadRecvPacket, this);
}

void NetworkSystem::disconnect()
{
    if (!isConnected_)
        return;
    
    io::Log::message("Disconnect from network connection");
    
    /* Send message to leave */
    if (isServer_)
        sendNetworkPacket(PACKET_SERVEROFF, 0, 0, 0);
    else
        sendNetworkPacket(PACKET_CLIENTLEFT, 0, 0, 0);
    
    /* wait until all clients got the message and then finally disconnect */
    while (ThreadSendPacket_->running())
        yield();
    
    /* Terminate all threads */
    MemoryManager::deleteMemory(ThreadOpenServer_);
    MemoryManager::deleteMemory(ThreadJoinServer_);
    MemoryManager::deleteMemory(ThreadAcceptClient_);
    MemoryManager::deleteMemory(ThreadSendPacket_);
    MemoryManager::deleteMemory(ThreadRecvPacket_);
    
    /*
    For server the main socket and address are not linked to a client object,
    thus these objects need to be deleted here.
    */
    if (isServer_)
    {
        MemoryManager::deleteMemory(Socket_);
        MemoryManager::deleteMemory(Address_);
    }
    
    /* Close network sockets and delete clients */
    MemoryManager::deleteList(ClientList_);
    ClientIPMap_.clear();
    
    /* Delete send and receive stack */
    for (std::list<SNetworkPacket>::iterator it = SendStack_.begin(); it != SendStack_.end(); ++it)
        it->deleteBuffer();
    for (std::list<SNetworkPacket>::iterator it = RecvStack_.begin(); it != RecvStack_.end(); ++it)
        it->deleteBuffer();
    
    SendStack_.clear();
    RecvStack_.clear();
    
    /* ReSettings */
    isServer_       = false;
    isConnected_    = false;
    LocalIP_        = 0;
    Port_           = 0;
}

void NetworkSystem::sendPacket(const void* Buffer, u32 BufferSize, NetworkClient* Acceptor)
{
    if (isConnected_)
        sendNetworkPacket(PACKET_CLIENTDATA, Buffer, BufferSize, Acceptor);
}

void NetworkSystem::sendPacket(const io::stringc &Message, NetworkClient* Acceptor)
{
    if (isConnected_)
        sendNetworkPacket(PACKET_CLIENTDATA, Message.c_str(), Message.size() + 1, Acceptor);
}

void NetworkSystem::sendWebServerRequest(const io::stringc &Message)
{
    if (isConnected_)
        sendNetworkPacket(PACKET_WEBREQUEST, Message.c_str(), Message.size() + 1, 0);
}

bool NetworkSystem::pickPacket(SNetworkPacket &Packet)
{
    if (isConnected_ && !RecvStack_.empty())
    {
        Packet = RecvStack_.front();
        RecvStack_.pop_front();
        return true;
    }
    return false;
}

io::stringc NetworkSystem::getHostIPAddress(const io::stringc &HostName) const
{
    /* Get host information */
    hostent* HostInfo = gethostbyname(HostName.c_str());
    
    if (HostInfo && HostInfo->h_addrtype == AF_INET)
    {
        /* Extract IP address */
        u8* IPAddress = (u8*)HostInfo->h_addr;
        
        return (
            io::stringc((s32)IPAddress[0]) + "." +
            io::stringc((s32)IPAddress[1]) + "." +
            io::stringc((s32)IPAddress[2]) + "." +
            io::stringc((s32)IPAddress[3])
        );
    }
    
    return "";
}

io::stringc NetworkSystem::getOfficialHostName(const io::stringc &HostName) const
{
    /* Get host information */
    hostent* HostInfo = gethostbyname(HostName.c_str());
    return (HostInfo && HostInfo->h_addrtype == AF_INET) ? io::stringc(HostInfo->h_name) : "";
}

std::list<io::stringc> NetworkSystem::getHostIPAddressList(const io::stringc &HostName) const
{
    std::list<io::stringc> AddressList;
    
    /* Get host information */
    hostent* HostInfo = gethostbyname(HostName.c_str());
    
    if (HostInfo && HostInfo->h_addrtype == AF_INET)
    {
        s32 i = 0;
        c8* pIPAddr;
        
        /* Extract each IP address */
        while ( ( pIPAddr = HostInfo->h_addr_list[i++] ) != 0 )
        {
            AddressList.push_back(
                io::stringc((s32)(u8)pIPAddr[0]) + "." +
                io::stringc((s32)(u8)pIPAddr[1]) + "." +
                io::stringc((s32)(u8)pIPAddr[2]) + "." +
                io::stringc((s32)(u8)pIPAddr[3])
            );
        }
    }
    
    return AddressList;
}

std::list<io::stringc> NetworkSystem::getNetworkMembers() const
{
    std::list<io::stringc> MemberList;
    
    #if defined(SP_PLATFORM_WINDOWS)
    
    SERVER_INFO_100* Buffer = 0;
    DWORD Count, i;
    
    /* Get server enumeration */
    if (NetServerEnum(0, 100, (LPBYTE*)&Buffer, MAX_PREFERRED_LENGTH, &i, &Count, SV_TYPE_ALL, 0, 0) != NERR_Success)
    {
        io::Log::error("Network member enumeration failed");
        return MemberList;
    }
    
    /* Extract network members */
    for (i = 0; i < Count; ++i)
    {
        /* Add name to the list */
        MemberList.push_back(
            io::stringw(std::wstring((c16*)Buffer[i].sv100_name)).toAscii().c_str()
        );
    }
    
    #endif
    
    return MemberList;
}

io::stringc NetworkSystem::getServerRequest(const io::stringc &HostName, const io::stringc &Filename)
{
    return "GET " + Filename + " HTTP/1.1\r\nHost: " + HostName + "\r\n\r\n";
}


/*
 * ======= Private: =======
 */

#ifdef SP_PLATFORM_WINDOWS

void NetworkSystem::createWinSock()
{
    /* Windows socket startup */
    const WORD VersionRequest = MAKEWORD(2, 2);
    
    const s32 ErrorCode = WSAStartup(VersionRequest, &WinSock_);
    
    /* Check for error */
    if (ErrorCode)
    {
        switch (ErrorCode)
        {
            case WSASYSNOTREADY:
                io::Log::error("WinSock startup error: WinSock not ready"); break;
            case WSAVERNOTSUPPORTED:
                io::Log::error("WinSock startup error: Requested WinSock version not supported"); break;
            case WSAEINPROGRESS:
                io::Log::error("WinSock startup error: Blocking WinSock 1.1 operation in progress"); break;
            case WSAEPROCLIM:
                io::Log::error("WinSock startup error: Maximum WinSock tasks reached"); break;
            case WSAEFAULT:
                io::Log::error("WinSock startup error: lpWSAData is not a valid pointer"); break;
            default:
                io::Log::error("WinSock startup error: Unknown error code: " + io::stringc(ErrorCode)); break;
        }
        
        return;
    }
    
    /* Print information about the socket */
    io::Log::message(io::stringc(WinSock_.szDescription) + " " + io::stringc(WinSock_.szSystemStatus));
}

void NetworkSystem::deleteWinSock()
{
    WSACleanup();
}

#endif

void NetworkSystem::yield(u32 Milliseconds)
{
    #if defined(SP_PLATFORM_WINDOWS)
    Sleep(Milliseconds);
    #elif defined(SP_PLATFORM_LINUX)
    usleep(Milliseconds * 1000);
    #endif
}

/*
Packet buffer format:
    Sender IP ................................................. 4 bytes (unsigned int)
    Acceptor IP (0 means all, > 0 means client IP address) .... 4 bytes (unsigned int)
    Packet type ............................................... 1 byte
    Buffer .................................................... X bytes
    
Buffer format for PACKET_CLIENTJOIN:
    Client IP address ......................................... 4 bytes (unsigned int)
    
Buffer format for PACKET_CLIENTLEFT:
    Client IP address ......................................... 4 bytes (unsigned int)
    
Buffer format for PACKET_SERVEROFF:
    empty
    
Paclet buffer format for PACKET_WEBREQUEST:
    NULL terminated string .................................... X bytes
*/
void NetworkSystem::sendNetworkPacket(
    const ENetworkPacketTypes Type, const void* Buffer, u32 BufferSize, NetworkClient* Acceptor, NetworkClient* Sender)
{
    c8* FinalBuffer = 0;
    
    if (Type == PACKET_WEBREQUEST)
    {
        /* Allocate memory for the final buffer and copy buffer */
        FinalBuffer = new c8[BufferSize];
        memcpy(FinalBuffer, Buffer, BufferSize);
    }
    else
    {
        /* Allocate memory for the final buffer */
        FinalBuffer = new c8[9 + BufferSize];
        
        /* Setup internal data */
        *(u32*)FinalBuffer          = ((Sender && Sender->getAddress()) ? Sender->getAddress()->getIPAddress() : LocalIP_);
        *(u32*)(FinalBuffer + 4)    = ((Acceptor && Acceptor->getAddress()) ? Acceptor->getAddress()->getIPAddress() : 0);
        FinalBuffer[8]              = (c8)Type;
        
        /* Copy buffer */
        memcpy(FinalBuffer + 9, Buffer, BufferSize);
        BufferSize += 9;
    }
    
    /*
    Add packet to the sending stack.
    Store the original sender (ClientOrigin) for the server to pass packets to other clients.
    */
    SNetworkPacket Packet;
    {
        Packet.Type         = Type;
        Packet.Buffer       = FinalBuffer;
        Packet.BufferSize   = BufferSize;
        Packet.Sender       = Acceptor;
        Packet.ClientOrigin = (isServer_ ? Sender : 0);
    }
    SendStack_.push_back(Packet);
}

NetworkClient* NetworkSystem::createServerAccount()
{
    return addClientAccount(
        new NetworkClient(Address_, Socket_, true)
    );
}

NetworkClient* NetworkSystem::createClientAccount(SOCKET Socket, const sockaddr_in &Address)
{
    return addClientAccount(
        new NetworkClient(new NetworkAddress(Address), new NetworkSocket(Protocol_, Socket))
    );
}

NetworkClient* NetworkSystem::createClientAccount(u32 IPAddress)
{
    return addClientAccount(
        new NetworkClient(new NetworkAddress(Port_, IPAddress))
    );
}

NetworkClient* NetworkSystem::addClientAccount(NetworkClient* NewClient)
{
    if (NewClient)
    {
        ClientList_.push_back(NewClient);
        
        if (NewClient->getAddress())
            ClientIPMap_[NewClient->getAddress()->getIPAddress()] = NewClient;
        if (NewClient->getSocket())
            ClientSocketMap_[NewClient->getSocket()->getSocket()] = NewClient;
        
        return NewClient;
    }
    return 0;
}

void NetworkSystem::removeClient(NetworkClient* Client)
{
    if (Client)
    {
        if (Client->getAddress())
            ClientIPMap_.erase(Client->getAddress()->getIPAddress());
        if (Client->getSocket())
            ClientSocketMap_.erase(Client->getSocket()->getSocket());
        MemoryManager::removeElement(ClientList_, Client, true);
    }
}

void NetworkSystem::sendSocketPacket(SOCKET Socket, const SNetworkPacket &Packet)
{
    if (send(Socket, (const c8*)Packet.Buffer, Packet.BufferSize, 0) == SOCKET_ERROR)
        io::Log::error("Sending network packet failed");
}


} // /namespace network

} // /namespace sp


#endif



// ================================================================================
