#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Usage: client <name> <port> <interval>" << endl;
        return 1;
    }

    string name = argv[1];
    int port = stoi(argv[2]);
    int interval = stoi(argv[3]);

    printf("name %s port %d interval %d\n", name.c_str(), port, interval);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    while (true) {
        time_t now = time(0);
        tm* tm = localtime(&now);
        char buffer[64];

	for (int i = 0; i < 64; i++) {
	  buffer[i] = ' ';
	}
	
        strftime(buffer, 64, "[%Y-%m-%d %H:%M:%S.%03d] ", tm);
	strcat(buffer, name.c_str());
	
        send(sockfd, buffer, strlen(buffer), 0);

        this_thread::sleep_for(chrono::seconds(interval));
    }

    close(sockfd);
    return 0;
}
