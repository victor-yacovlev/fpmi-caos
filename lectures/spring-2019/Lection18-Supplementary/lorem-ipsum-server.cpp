#include <cstring>
#include <cstdint>

extern "C" {
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <errno.h>
}

#include <string>
#include <map>
#include <iostream>


static const uint16_t PortNumber = 3000;
static const size_t MaxEvents = 10000;
static const size_t QuantumSize = 10 /* bytes */;
static const std::string Message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus elementum sem sit amet ipsum mattis, ut vestibulum diam ornare. Nullam id tortor quis arcu vehicula ornare. Nunc sit amet ultricies felis. Donec auctor sagittis commodo. Aenean cursus, turpis et imperdiet vestibulum, nibh nisi laoreet quam, a posuere ligula arcu non nisi. Sed at sodales neque, eu sollicitudin neque. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nullam ultricies, eros sed finibus efficitur, odio nisl volutpat ex, id tincidunt nunc metus ac elit. Sed gravida dui eu velit pellentesque, sit amet pellentesque nunc tincidunt. Cras dignissim porttitor eros vel accumsan. Sed tellus nisl, viverra quis velit laoreet, scelerisque elementum neque. Fusce purus eros, pretium sed lorem non, maximus accumsan sem. Donec tincidunt lorem eu diam ultrices, sed varius leo lobortis. Nulla nec convallis odio. Maecenas bibendum tellus vel mauris congue maximus. Proin id lectus accumsan, vehicula risus sit amet, gravida velit.\n";

static sig_atomic_t StopRequest = 0;

static int
create_and_bind()
{
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (0 > sd) {
        perror("Socket");
        exit(1);
    }
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PortNumber);
    int bs = bind(sd, (sockaddr*) &addr, sizeof(addr));
    if (0 > bs) {
        perror("Bind");
        close(sd);
        exit(1);
    }
    return sd;
}

static void
make_non_blocking(int sd)
{
    int flags = fcntl(sd, F_GETFL);
    fcntl(sd, F_SETFL, flags | O_NONBLOCK);
}


static void
stop_handler(int s)
{
    StopRequest = 1;
}

int main(int argc, char* argv[])
{
    int status = 0;

    int sock_fd = create_and_bind();

    make_non_blocking(sock_fd);

    status = listen(sock_fd, SOMAXCONN);
    if (0 > status) {
        perror("Listen");
        close(sock_fd);
        exit(1);
    }


    /* Create epoll queue */
    int ed = epoll_create1(0);

    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = sock_fd;
    event.events = EPOLLIN|EPOLLOUT;

    status = epoll_ctl(ed, EPOLL_CTL_ADD, sock_fd, &event);
    if (0 > status) {
        perror("Epoll control");
        close(sock_fd);
        exit(1);
    }


    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = stop_handler;
    act.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);
    signal(SIGPIPE, SIG_IGN);

    epoll_event *pending_events = new epoll_event[MaxEvents];

    std::map<int,size_t> out_data_positions;

    while ( ! StopRequest ) {

        int n = epoll_wait(ed, pending_events, MaxEvents, -1);
        if (-1 == n) {
            break; // Bye!
        }
        else if (0 > n) {
            perror("Epoll wait");
            close(sock_fd);
            exit(1);
        }

        for (int i=0; i<n; ++i) {
            const uint32_t emask = pending_events[i].events;
            if ( (emask & EPOLLERR) || (emask & EPOLLHUP) )
            {
                if (emask & EPOLLERR)
                    std::cerr << "Something wrong!";
                int hang_fd = pending_events[i].data.fd;
                if (out_data_positions.count(hang_fd)) {
                    out_data_positions.erase(hang_fd);
                }
                close(hang_fd);
                continue;
            }
            else if (pending_events[i].data.fd == sock_fd) {
                // Incoming connection event

                while (true) {
                    // There is possible several connections at a time,
                    // so accept them all

                    struct sockaddr_in in_addr;
                    socklen_t in_addr_size = sizeof(in_addr);
                    int incoming_fd = accept(sock_fd, (sockaddr*)&in_addr, &in_addr_size);
                    if (-1 == incoming_fd) {
                        // This might be not an error!
                        if (EAGAIN == errno || EWOULDBLOCK == errno) {
                            break;
                        }
                        else {
                            perror("Accept failed");
                            close(sock_fd);
                            exit(1);
                        }
                    }
                    else {
                        // Register newly created file descriptor for
                        // event processing
                        make_non_blocking(incoming_fd);
                        event.data.fd = incoming_fd;
                        event.events = EPOLLIN | EPOLLOUT;
                        status = epoll_ctl(ed, EPOLL_CTL_ADD, incoming_fd, &event);
                        if (0 > status) {
                            perror("Epoll control for incoming connection");
                            close(sock_fd);
                            exit(1);
                        }
                        out_data_positions.insert(std::make_pair(incoming_fd, 0));
                    }
                    continue;
                } /* end while(true) */

            }
            else if ( emask & EPOLLOUT ) {
                // Previous data block was successfully sent,
                // and current connection is ready to eat some
                // more data
                int out_fd = pending_events[i].data.fd;
                size_t last_pos = out_data_positions[out_fd];

                const std::string message_quant = Message.substr(last_pos, QuantumSize);
                size_t new_pos = message_quant.length() < QuantumSize
                        ? 0
                        : last_pos + QuantumSize;
                write(out_fd, message_quant.c_str(), message_quant.length());
                out_data_positions[out_fd] = new_pos;

            }
            else if ( emask & EPOLLIN ) {
                // Connection wants to write us some data
                // TODO implement me the same way!
            }
            else {
                std::cerr << "This branch unreachable!" << std::endl;
                close(sock_fd);
                exit(1);
            }
        }
    }

    delete[] pending_events;
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
    std::cout << "Bye!" << std::endl;
    return 0;
}
