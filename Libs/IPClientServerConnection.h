#ifndef IPCLIENTSERVERCONNECTION_H_INCLUDED
#define IPCLIENTSERVERCONNECTION_H_INCLUDED

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

class IPAddress{
public:
    sockaddr_in ipAddress;

    static int DEFAULT_PORT;

    static IPAddress anyAddress( int port = DEFAULT_PORT ){
        return IPAddress( port, htonl(INADDR_ANY) );
    }

    IPAddress( String address, int port = DEFAULT_PORT ){
        ZeroMemory( &ipAddress, sizeof(ipAddress) );

        ipAddress.sin_family = AF_INET;
        ipAddress.sin_addr.s_addr = inet_addr(address.toCharString());
        ipAddress.sin_port = htons(port);
    }

    IPAddress( int port = DEFAULT_PORT, int addres = inet_addr("127.0.0.1") ){
        ZeroMemory( &ipAddress, sizeof(ipAddress) );

        ipAddress.sin_family = AF_INET;
        ipAddress.sin_addr.s_addr = addres;
        ipAddress.sin_port = htons(port);
    }

    friend ostream& operator << ( ostream &stream, const IPAddress& arg ){
        stream << " IP:" << inet_ntoa( arg.ipAddress.sin_addr ) << " PORT:" << ntohs(arg.ipAddress.sin_port);

        return stream;
    }
};
int IPAddress::DEFAULT_PORT = 27456;

class Socket{
public:
    int socketHandler;

    static int MAX_BUFFER_SIZE;

    Socket( String protocol = "UDP", int IPFamily = AF_INET ){

        if( protocol == "TCP" ){
            socketHandler = socket(IPFamily, SOCK_STREAM, IPPROTO_TCP);
        }
        else if(protocol == "UDP"){
            socketHandler = socket(IPFamily, SOCK_DGRAM, IPPROTO_UDP);
        }
        else{
            cout << "Protocol invalid or not implemented" << endl;
        }

        if( INVALID_SOCKET == socketHandler ){
            cout << "No socket initialized with error code: " << WSAGetLastError() << endl;
        }
    }

    void bindSocket( IPAddress hostAddress ){
        bind( socketHandler, (struct sockaddr *)&hostAddress.ipAddress, sizeof(hostAddress.ipAddress) );
    }

    void recieve( char **data, size_t &dataSize, IPAddress &sender ){

        *data = (char*)(new char[MAX_BUFFER_SIZE]);
        int addressLenght = (int)sizeof(sender.ipAddress);              ///no need to initialize addrLen

        dataSize = recvfrom( socketHandler, (char*)*data, MAX_BUFFER_SIZE, 0
                            , (struct sockaddr *)&sender.ipAddress , &addressLenght );
    }

    void send( char* data, size_t dataSize, IPAddress receiver  ){
        sendto( socketHandler, (const char*)data, dataSize, 0
               , (struct sockaddr *)&receiver.ipAddress, sizeof( receiver.ipAddress ) );
    }

    ~Socket(){
        closesocket(socketHandler);
    }
};

int Socket::MAX_BUFFER_SIZE = 1024 * 512;  // 512 KB

class UDP_Connection{
public:
    Socket *recvSock;
    Socket *sendSock;

    IPAddress hostAddress;
    IPAddress connectAddress;

    static void WSAInit(){
        WSAData wsaData;
        if( int result = WSAStartup( MAKEWORD(2,2), &wsaData ) ){
            cout << "Windows Sockets API could not be initialized, error code:" << result << endl;
        }
    }

    void WaitngTime( int timeoutMili ){
        setsockopt(recvSock->socketHandler, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutMili, sizeof(int));
    }

    UDP_Connection( IPAddress address, IPAddress hostAddress = IPAddress::anyAddress(), int IPFamily = AF_INET ){     /// AF_INET6 not implemented yet
        connectAddress = address;
        this->hostAddress = hostAddress;

        cout << this->connectAddress << endl;
        cout << this->hostAddress << endl;

        recvSock = new Socket("UDP", IPFamily);
        sendSock = new Socket("UDP", IPFamily);

        recvSock->bindSocket( hostAddress );
    }

    void send( char *data, size_t size ){
        sendSock->send( data, size, connectAddress );
    }

    int getConnectPort(){
        return ntohs( connectAddress.ipAddress.sin_port );
    }

    void setConnectPort( int port ){
        connectAddress.ipAddress.sin_port = htons( port );
    }

    IPAddress recieve( char **data, size_t &size ){
        IPAddress recvFrom;

        recvSock->recieve( data, size, recvFrom );

        return recvFrom;
    }

    ~UDP_Connection(){
        WSACleanup();
    }
};

class TCP_Conection{
public:

};

/**

UDP

    Client - Serv

        SendTo      -> Data, IP
        RecvFrom    <- IP, Data

        bind
        socket

TCP
        --- Serv
    socket
    bind

    listen()
    newSock = accept()

    recv(newSock)
    send(newSock)

        --- Client

    socket
    connect

    recv()
    send()
**/

//#define DEFAULT_PORT 27015
//#define DEFAULT_PORT_C "27015"
//#define DEFAULT_BUFLEN 512

//int main(int argc, char **argv)
//{
//    WSADATA wsaData;
//    int iResult;
//
//    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed with error: %d\n", iResult);
//        return 1;
//    }
//
//    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//
//    /// set server address
//    struct sockaddr_in servaddr;
//
//    int port;
//
//    char *serverAddr = new char[255];
//    cout << "enter server ip and port: " << endl;
//    cin >> serverAddr;
//    //cin >> port;
//
//    ZeroMemory( &servaddr, sizeof(servaddr) );
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_addr.s_addr = inet_addr(serverAddr);
//    servaddr.sin_port = htons(DEFAULT_PORT);
//
//    /// set server address
//
//    /// opt timeout
//    int nTimeout = 3000; // 3 seconds
//    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int));
//
//    /// opt timeout
//
//    char *str = new char[10000000000];
//
//    while (1)
//    {
//
//        int err = sendto( sockfd, str, 1024 * 16, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
//        cout << err << endl;
//    }
//
//    cout << "endSending" << endl;
//
//    return 0;
//}
//
//
//int main(int argc, char **argv)
//{
//    WSADATA wsaData;
//    int k;
//
//    cout << errno << endl;
//
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed with error: %d\n", iResult);
//        return 1;
//    }
//    cout << errno << endl;
//
//    int sockfd;
//    struct sockaddr_in servaddr, cliaddr;
//
//    cout << "Socket: " << (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) << endl;
//    cout << errno << endl;
//
//    ZeroMemory(&servaddr, sizeof(servaddr));
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    servaddr.sin_port = htons(DEFAULT_PORT);
//
//    cout << "Binding: " << bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) << endl;
//    cout << errno << endl;
//
//    cout << WSAGetLastError() << " wsa " << endl;
//
//    char *mesg = new char[255];
//    int bytes;
//    int len = 10;
//
//    while (1)
//    {
//        len = sizeof(cliaddr);
//        bytes = recvfrom(sockfd, mesg, 255, 0, (struct sockaddr *)&cliaddr, &len);
//        if( bytes != -1 )
//            cout << bytes << " recived from " << cliaddr.sin_addr.s_addr << " with mesage: " << mesg << endl;
//        else{
//            cout << errno << endl;
//            cout << WSAGetLastError() << " wsa " << endl;
//        }
//
//        ZeroMemory( mesg, 255 );
//    }
//
//    cout << "endSending" << endl;
//
//    return 0;
//}

/**

include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);

    return 0;
}
**/


#endif // IPCLIENTSERVERCONNECTION_H_INCLUDED
