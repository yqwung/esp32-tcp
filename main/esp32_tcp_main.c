#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "soc/rtc_cntl_reg.h"

#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

#include "user_wifi_sta.h"
#include "user_tcp.h"
#include "user_config.h"


void app_main()
{
	printf("========================================================\r\n");
	printf("[TCP SERVER] Example : wifi set mode of STA! \r\n");
	printf("[TCP SERVER] Free memory: %d bytes \r\n", esp_get_free_heap_size());
	//printf("[UDP SERVER] Build time: %s \r\n", BUID_TIME);

    nvs_flash_init();
	
    user_wifi_config_sta();                // 连接wifi
	
	vTaskDelay(10000 / portTICK_RATE_MS);  // 等待ESP32连接上wifi，才能创建UDP socket
	
	char socket_ret;
	
	//socket_ret = user_udp_server();
	socket_ret = user_udp_server();
	if(socket_ret == 0) 
	{
		//ESP_LOGI(TAG, "create udp socket error,stop.");
		printf("Create TCP socket error, stop.");
    }
	
	vTaskDelay(10000 / portTICK_RATE_MS);
	
	/*create a task to tx/rx data*/
    TaskHandle_t tx_rx_task;
    xTaskCreate(&user_receive_data, "tcp_recv_data", 4096, NULL, 4, &tx_rx_task);  
	//xTaskCreate(&user_client_send_recv_data, "send_recv_data", 4096, NULL, 4, &tx_rx_task);	
}
