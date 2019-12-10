# Sockets with connection setup

## Socket

A socket is a file descriptor that is open for both reading and writing. It is used for interaction between:
 * different processes running on the same computer (*host*);
 * different processes running on different *hosts*.

To create a socket use the `socket` system call:

```
#include <sys/types.h>
#include <sys/socket.h>

int socket(
  int domain,    // domain type
  int type,      // the type of interaction via the socket
  int protocol   // protocol number or 0 for auto-selection
)
```

The socket mechanism appeared in the 80's of the XX century, when there was no single standard for network communication, and sockets were an abstraction over any network communication mechanism, supporting a huge number of different protocols.

In modern systems there are only a few mechanisms defining the socket namespace, that can be considered to be used; others are legacy, that we will not discuss further.

  * `AF_UNIX` (`man 7 unix`) - a namespace of local UNIX sockets that allow different processes to communicate within the same computer, using as the address a unique name (no longer than 107 bytes) of a special file.
 * `AF_INET` (`man 7 ip`) - tuple space consisting of 32-bit IPv4 addresses and 16-bit port numbers. The IP address identifies the host for communication on which the process is running.
 * `AF_INET6` (`man 7 ipv6`) - similar to `AF_INET`, but uses 128-bit IPv6 host addressing; this standard is not yet supported by all hosts and Internet service providers.
 * `AF_PACKET` (`man 7 packet`) - low level interaction.

Sockets usually communicate in one of two ways (specified as the second parameter `type`):
 * `SOCK_STREAM` - interacts with system calls `read` and `write` as with a regular file descriptor. In the case of network communication, this implies the use of the `TCP` protocol.
 * `SOCK_DGRAM` - communication without pre-setting interaction to send short messages. In case of communication over the network, this implies the usage of `UDP` protocol.

## Socket pair

Sometimes sockets are convenient to use as a mechanism of communication between different threads or related processes: unlike pipes, they are two-way, and in addition, support the processing of the event "close connection". A socket pair is created using the `socketpair` system call:

```
int socketpair(
  int domain,    // In Linux: AF_UNIX only is supported
  int type,      // SOCK_STREAM or SOCK_DGRAM
  int protocol,  // Only value 0 in Linux
  int sv[2]      // An array of two int's (similar to pipe)
)
```

Unlike unnamed pipes, which are created by the `pipe` system call, it does not matter for a pair of sockets which element of the `sv` array is used for reading and which element is used for writing - they are equal.

## Using sockets as a client

Sockets can participate in the communication in one of two roles. A process can be a *server*, that means it declares some address (file name, or tuple of IP address and port number) to receive incoming connections, or it can act as a *client*, that means it connects to some server.

Once the socket is created, it is not ready yet to interact with the `read` and `write` system calls. The interaction with the server is established using the `connect` system call. After successful execution of this system call, interaction becomes possible before the system call `shutdown` won't be executed.

```
int connect(
  int sockfd,                  // the socket file descriptor

  const struct sockaddr *addr, // a pointer to an *abstract* structure
                               // that describes
                               // the connection address

  socklen_t addrlen            // the size of the real structure
                               // that is passed as
                               // the second parameter
)
```

Since the C language is not object-oriented, it is necessary to pass as an address:
 1. A structure whose first field contains an integer with a value that matches the `domain` of the corresponding socket
 2. The size of this structure.

Particular structures that are "inherited" from the abstract `sockaddr` structure can be:

1. For UNIX address space - structure `sockaddr_un`

```
#include <sys/socket.h>
#include <sys/un.h>

struct sockaddr_un {
  sa_family_t   sun_family;    // you need to write AF_UNIX
  char          sun_path[108]; // the path to the socket file
};
```

2. For addressing in IPv4 - structure `sockaddr_in`:

```
#include <sys/socket.h>
#include <netinet/in.h>

struct sockaddr_in {
  sa_family_t    sin_family; // you need to write AF_INET
  in_port_t      sin_port;   // uint16_t port number
  struct in_addr sin_addr;   // a structure of a single field:
                             // - in_addr_t s_addr;
                             //   where in_addr_t - is uint32_t
};
```

3. For addressing in IPv6 - structure `sockaddr_in6`:

```
#include <sys/socket.h>
#include <netinet/in.h>

struct sockaddr_in6 {
  sa_family_t    sin6_family; // you need to write AF_INET6
  in_port_t      sin6_port;   // uint16_t port number
  uint32_t       sin6_flowinfo; // additional IPv6 field
  struct in6_addr sin6_addr;  // a structure of a single field
                  // declared as union {
                              //     uint8_t  [16];
                              //     uint16_t [8];
                              //     uint32_t [4];
                              // };
                              // i.e. the size of in6_addr is 128 bits
  uint32_t       sin6_scope_id; // additional IPv6 field
};
```


## IPv4 addresses

The host address in IPv4 network is a 32-bit unsigned integer in *network byte order*, that is, Big-Endian. The same is for the port numbers.

The byte order conversion from network to system and vice versa is performed using one of the functions declared in `<arpa/inet.h>`:
 * `uint32_t htonl(uint32_t hostlong)` - 32-bit from system to network byte order;
 * `uint32_t ntohl(uint32_t netlong)` - 32-bit from network to system byte order;
 * `uint16_t htons(uint16_t hostshort)` - 16-bit from system to network byte order;
 * `uint16_t ntohs(uint16_t netshort)` - 16-bit from network to system byte order.

IPv4 addresses are usually written in decimal notation, separating each byte with a dot, for example: `192.168.1.1`. Such kind of notation can be converted from text to a 32-bit address using the `inet_aton` or `inet_addr` function.


## Closing the network connection

The `close` system call is intended to close the *file descriptor*, and it must be called to release an entry in the file descriptor table. This is a necessary but not a sufficient requirement when working with TCP sockets.

Besides closing the file descriptor, it is good to notify the opposite sides that the network connection is closing.

This notification is done via the `shutdown` system call.


## Using sockets as a server

To use a socket as a server, you should do the following:

 1. Associate a socket with some address. To do this, use the `bind` system call, whose parameters are exactly the same as for the `connect` system call.  If the computer has more than one IP address, the address `0.0.0.0` means `all addresses`. Often when debugging and there is such a problem that the port with a certain number was already busy on the previous run of the program (and, for example, was not correctly closed). It is solved by force reuse of the address:

```
// In the release build you shouldn't do like this!
#ifdef DEBUG
int val = 1;
setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
#endif
```

 2. Create a queue that will contain incoming but not yet received connections. This is done using the `listen` system call, which takes as a parameter the maximum number of pending connections. For Linux, this value is 128, defined in the constant `SOMAXCONN`.

 3. Accept one connection at a time using the `accept` system call. The second and third parameters of this system call can be `NULL` if we are not interested in the address of the one who connected to us. The `accept` system call blocks execution until an incoming connection appears. After that it returns the file descriptor of the new socket, which is associated with a specific client that is connected to us.
 
 
## Linux tools for debugging networking

### Network I/O

The `nc` command (short for` netcat`) works similarly to the `cat` command, but in
the argument is not a file name for outputting a data stream, but a pair
`host port`. The `-u` option means sending a UDP packet.

If you intend to use only IPv4 addressing, and not IPv6, then
option `-4` is used.

```
# Example: transfer data from in.dat to UDP socket on localhost
# port 3000 and write the output to the out.dat file
> cat in.dat | nc -4 -u localhost 3000> out.dat
```

### God mode

The `wireshark` utility allows you to view absolutely all packets from the` Ethernet` level that pass through the system. This requires `root` privileges, or the` Linux Capabilities` setting for the `/ usr / bin / dumpcap` command, which is part of` wireshark`:
```
sudo setcap cap_net_raw,cap_net_admin+eip /usr/bin/dumpcap
```

Since many network packets pass through the system, you must configure a filter to search only packets of interest.

### Python

The Python standard library contains socket tools that exactly match their POSIX counterparts.

An example of sending a UDP message:
```
from socket import socket, AF_INET, SOCK_DGRAM

IP = "127.0.0.1"
PORT = 3000

sock = socket(AF_INET, SOCK_DGRAM)   # creating UDP-socket
# Connection is not required
sock.sendto("Hello!\n", (IP, PORT))  # sending message
```

Recieving UDP-messages:
```
from socket import socket, AF_INET, SOCK_DGRAM

IP = "127.0.0.1"
PORT = 3000
MAX_SIZE = 1024

sock = socket(AF_INET, SOCK_DGRAM)       # creating UDP-socket
sock.bind((IP, PORT))                    # declaring port 

while True:
    data, addr = sock.recvfrom(MAX_SIZE) # recieving message
    print("Got {} from {}", data, addr)
```
