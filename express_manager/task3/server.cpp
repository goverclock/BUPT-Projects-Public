#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // prepare
    int serv_sock = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    assert(serv_sock != -1);

    struct sockaddr_in serv_adr, clnt_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    int ret = bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    assert(ret != -1);

    ret = listen(serv_sock, 5);
    assert(ret != -1);

    while (1) {
        // reap a child
        ret = waitpid(-1, nullptr, WNOHANG);
        if (ret != 0 && ret != -1) {
            printf("client left %d\n", ret);
            fflush(stdout);
        }

        // connect to a client
        socklen_t clnt_adr_sz;
        int clnt_sock =
            accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1) {
            // no incoming client
            // no need to clear errno
            continue;
        }

        // fork, exec
        int pid = fork();
        if (pid == 0) {          // child
            dup2(clnt_sock, 0);  // 0 - stdin
            dup2(clnt_sock, 1);  // 1 - stdout
            chdir("./base");
            execv("./base", argv);

            puts("execv error");
            exit(2);
        }

        // parent
        printf("new client %d\n", pid);
        fflush(stdout);
        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}
