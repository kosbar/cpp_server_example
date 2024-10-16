#include <iostream>
#include <string>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: server <port>" << endl;
        return 1;
    }

    int port = stoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(sockfd, 10) < 0) {
        perror("listen");
        return 1;
    }

    ofstream log_file("log.txt");
    if (!log_file) {
        log_file.open("log.txt", ios::out);
    }

    while (true) {
        int client_sockfd = accept(sockfd, NULL, NULL);
        if (client_sockfd < 0) {
            perror("accept");
            continue;
        }

        thread t([client_sockfd, &log_file]() {
            char buffer[64];
            while (true) {
	      if (recv(client_sockfd, buffer, 64, 0)) {
		cout << buffer << endl;
		log_file << buffer << endl;
	      }
            }
            close(client_sockfd);
        });

        t.detach();
    }

    return 0;
}
