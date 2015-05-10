// Copyright 2015 Tony Wasserka
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <3ds.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

static void *SOC_buffer = NULL;
static FILE* write_stream = NULL;
static int netloader_listenfd = -1;
static int netloader_datafd = -1;

#define CITRACE_PORT 11113

void NetworkInit() {
    SOC_buffer = memalign(0x1000, 0x100000);
    if (SOC_buffer == NULL)
        return;

    SOC_Initialize((uint32_t*)SOC_buffer, 0x100000);

    netloader_listenfd = socket(AF_INET, SOCK_STREAM, 0);

    int flag = 1;
    setsockopt(netloader_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(CITRACE_PORT);

    bind(netloader_listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(netloader_listenfd, 10);

    netloader_datafd = accept(netloader_listenfd, (struct sockaddr*)NULL, NULL);

    write_stream = fdopen(netloader_datafd, "w");
}

void NetworkExit() {
    fclose(write_stream);

    close(netloader_listenfd);

    SOC_Shutdown();
}

static void NetworkVPrint(const char* str, va_list args)
{
    char buffer[4096];
    // TODO: Use vsnprintf instead
    int len = vsprintf(buffer, str, args);
    send(netloader_datafd, buffer, len+1, 0);
}

void NetworkPrint(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    NetworkVPrint(str, args);
    va_end(args);
}
