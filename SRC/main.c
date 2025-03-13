#include <wut.h>
#include <coreinit/screen.h>
#include <coreinit/memdefaultheap.h>
#include <nsysnet/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 5000
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRAMEBUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * 4) // RGBA
#define PACKET_SIZE 1024  // UDP packet size

int sockfd;
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);

void init_network() {
    nsysnet_init();

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd < 0) {
        OSFatal("Socket creation failed");
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_BROADCAST;  // Broadcast mode
    client_addr.sin_port = htons(SERVER_PORT);
}

void capture_and_send_screen() {
    void *framebuffer = MEMAllocFromDefaultHeapEx(FRAMEBUFFER_SIZE, 0x100);

    while (1) {
        OSScreenGetBufferEx(SCREEN_TV, framebuffer);

        for (int offset = 0; offset < FRAMEBUFFER_SIZE; offset += PACKET_SIZE) {
            sendto(sockfd, (char*)framebuffer + offset, PACKET_SIZE, 0,
                   (struct sockaddr *)&client_addr, client_len);
        }

        OSSleepTicks(OSSecondsToTicks(1.0 / 30));  // Send at 30 FPS
    }

    MEMFreeToDefaultHeap(framebuffer);
}

int main(void) {
    init_network();
    OSScreenInit();
    capture_and_send_screen();
    return 0;
}
