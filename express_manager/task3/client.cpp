#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    pollfd fds[2];
    fds[0].fd = 0;  // stdin
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sock;  // server sock
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    int ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    assert(ret != -1);

    int pipefd[2];
    pipe(pipefd);

    char buf[1024];
    while (1) {
        ret = poll(fds, 2, 1);
        assert(ret >= 0);
        if (fds[1].revents & POLLIN) {
            memset(buf, 0, sizeof(buf));
            recv(sock, buf, sizeof(buf) - 1, 0);
            printf("%s", buf);
            fflush(stdout);
        }
        if (fds[0].revents & POLLIN) {
            fgets(buf, sizeof(buf), stdin);
            write(sock, buf, strlen(buf));
        }
    }

    close(sock);

    return 0;
}
