#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

char server_reply[100];
int recv_size;
char str[10];

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {

    // Initialize Winsock
    WSADATA wsa;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (iResult != 0) {
        printf("Inisialisasi gagal: %d\n", iResult);
        return 1;
    }
    
    // Create a socket
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Gagal membuat socket: %d", WSAGetLastError());
        return 1;
    }

    // connect to server
    char *server_ip = "127.0.0.1"; // Ganti dengan alamat IP server Anda
    int server_port = 12345; // Ganti dengan port yang Anda gunakan di server.py   
   
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Koneksi gagal");
        return 1;
    }
    
    do{
        // masukkan dan baca nama file yang ingin diupload
        char filename[30];
        printf("masukkan nama file yang ingin diupload: ");
        scanf("%s[^\n]", filename);        
        FILE *file;
        long file_size;
        char *file_data;
        file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Gagal membuka file\n");
            return 1;
        }
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        file_data = (char *)malloc(file_size);
        fread(file_data, file_size, 1, file);
        fclose(file);

        printf("file berhasil dibaca\n");

        //upload ke server
        printf("Ketik \"upload\" untuk mengirim file: ");
        scanf("%s", str);
        if (send(client_socket, str, strlen(str), 0) < 0) {
            printf("Gagal mengirim data");
            return 1;
        }
        if(strcmp(str, "upload") == 0){
            if (send(client_socket, file_data, file_size, 0) < 0) {
                printf("Gagal mengirim data");
                return 1;
            }
            closesocket(client_socket);
            WSACleanup();
            break;
        }
        if((recv_size = recv(client_socket, server_reply, 2000, 0)) > 0){
            server_reply[recv_size] = '\0';
            printf("Server reply: %s\n", server_reply);
        }
    } while (strcmp(str, "upload") != 0);
    
    printf("Data PDF berhasil dikirim\n");

    return 0;
}
