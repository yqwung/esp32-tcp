#include "user_tcp.h"
#include <sys/socket.h>


static int server_socket = 0;
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;
static unsigned int socklen = sizeof(client_addr);
static int connect_socket = 0;
bool g_rxtx_need_restart = false;

int g_total_data = 0;


//use this esp32 as a tcp server. return ESP_OK:success ESP_FAIL:error
char user_udp_server(void)
{
	printf("[TCP SERVER] Create TCP server socket!\r\n");
	printf("[TCP SERVER] Create TCP server port: !\r\n");
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
	{
        printf("create TCP socket error\r\n");
		
        return 0;
    }
	
	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(USER_DEFAULT_TCP_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	{
        //show_socket_error_reason("bind_server", server_socket);
		printf("[TCP SERVER] TCP bind error!\r\n");
        close(server_socket);
		
        return 0;
    }
    if (listen(server_socket, 5) < 0) 
	{
        //show_socket_error_reason("listen_server", server_socket);
		printf("[TCP SERVER] TCP listen error!\r\n");
        close(server_socket);
		
        return 0;
    }
    connect_socket = accept(server_socket, (struct sockaddr *)&client_addr, &socklen);  // 接收客户端上来的连接
    if (connect_socket < 0) 
	{
        //show_socket_error_reason("accept_server", connect_socket);
		printf("[TCP SERVER] TCP accept error!\r\n");
        close(server_socket);
		
        return 0;
    }

    //ESP_LOGI(TAG, "tcp connection established!");
	printf("[TCP SERVER] TCP connection established!\r\n");
	
    return 1;	
}


void user_receive_data(void *pvParameters)
{
    int len = 0;
    char *databuff = (char *)malloc(USER_DEFAULT_PKTSIZE * sizeof(char));
	
    while (1) 
	{
        int to_recv = USER_DEFAULT_PKTSIZE;
        while (to_recv > 0) 
		{
            len = recv(connect_socket, databuff, to_recv, 0);
            if (len > 0) 
			{
                g_total_data += len;
                to_recv -= len;
				printf("[TCP SERVER] TCP Server reveive data , len = %d!\r\n", len);
				
				send(connect_socket, databuff, len, 0);
            } 
			else 
			{
                //show_socket_error_reason("recv_data", connect_socket);
				printf("[TCP SERVER] TCP Server reveive data error!\r\n");
                break;
            }
        }
        if (g_total_data > 0) 
		{
            continue;
        } 
		else 
		{
            break;
        }
    }

    //g_rxtx_need_restart = true;
    free(databuff);
    //vTaskDelete(NULL);
}


