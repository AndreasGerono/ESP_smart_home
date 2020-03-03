//
// esp01.c
// Created by Andreas Gerono on 10/10/2019.

#include "esp01.h"
#include "string.h"
#include "tasker.h"
#include <stdio.h>
#include <stdlib.h>

#define RX_BUFFER_SIZE 100
#define TX_BUFFER_SIZE 50
#define PAIRING 9999

enum Status{
	AP_Disconnected,
	AP_Connected,
	Server_Connected,
	Passthrough
} esp_status;

static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t tx_buffer[TX_BUFFER_SIZE];
static uint32_t device_id;




void clean_rx_buffer(){
	memset(rx_buffer, ' ', RX_BUFFER_SIZE);
}


void disable_passthrough(){
	HAL_UART_Transmit(&ESP_UART, (uint8_t*) "+++", 3, HAL_MAX_DELAY);
	HAL_UART_Abort(&ESP_UART);
	HAL_UART_Receive_IT(&huart1, rx_buffer, RX_BUFFER_SIZE);
	HAL_Delay(1100);
}

void enable_passthrough(){
	esp_send_data("AT+CIPMODE=1");
	esp_wait_for_response("OK", 500);
	esp_send_data("AT+CIPSEND");
	esp_wait_for_response(">", 500);
	esp_status = Passthrough;
}

void connect_to_server(){
	esp_send_data("AT+CIPSTART=\"TCP\",\"192.168.0.143\",1337");
	esp_wait_for_response("OK", 1000);
	esp_status = Server_Connected;
}

void save_device_id(uint32_t id){
	device_id = id;
}

void pair_device(){
	char buffer[20];
	itoa((device_id*10000 + PAIRING), buffer, 10);
	esp_send_data(buffer);
}

void esp_send_data(const char* data){
	strcpy((char*)tx_buffer, data);
	strcat((char*)tx_buffer, "\r\n");
	HAL_UART_Transmit(&ESP_UART, tx_buffer, strlen(data)+2, HAL_MAX_DELAY);
	HAL_UART_Abort(&ESP_UART);
	HAL_UART_Receive_IT(&huart1, rx_buffer, RX_BUFFER_SIZE);
	clean_rx_buffer();
}


_Bool search_rx_buffer(const char *str){
	return NULL != strstr((const char*)rx_buffer, str);
}

void esp_wait_for_response(const char* response, uint16_t timeout_ms){
	Task time_out_task = task_make(timeout_ms, (void*)NVIC_SystemReset);
	task_start(time_out_task);
	while(!search_rx_buffer(response) && !task_state(time_out_task, NULL));
}


void set_esp_status(){
	esp_send_data("AT+CIPSTATUS");
	esp_wait_for_response("OK", 500);
	if (search_rx_buffer("STATUS:2") || search_rx_buffer("STATUS:4"))
		esp_status = AP_Connected;
	else if (search_rx_buffer("STATUS:3"))
		esp_status = Server_Connected;
	else
		esp_status = AP_Disconnected;

}

void esp_initialize(uint32_t device_id){
	HAL_Delay(5000);
	save_device_id(device_id);
	disable_passthrough();
	set_esp_status();

	if (esp_status == AP_Connected)
		connect_to_server();
	if (esp_status == Server_Connected)
		enable_passthrough();
	if (esp_status == Passthrough)
		pair_device();
	if (esp_status == AP_Disconnected){

	}

}

void esp_wps(){
	disable_passthrough();
	esp_send_data("AT+CWMODE=1");
	esp_send_data("AT+WPS=1");
	esp_wait_for_response("connecting", 10000);
	esp_initialize(device_id);
}

void esp_check_connection(){
	if (search_rx_buffer("Connected"))
		pair_device();
	else if(search_rx_buffer("ERROR"))
		esp_initialize(device_id);
	else{
		LD2_GPIO_Port->ODR ^= LD2_Pin;
	}
}

void esp_uart_callback(){	//put this in UART rx_callback in main
	HAL_UART_Receive_IT(&huart1, rx_buffer, RX_BUFFER_SIZE);
}





