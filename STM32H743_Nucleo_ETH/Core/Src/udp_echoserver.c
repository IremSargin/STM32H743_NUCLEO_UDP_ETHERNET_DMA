#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#define UDP_SERVER_PORT 7
#define UDP_CLIENT_PORT 7



void udp_echoserver_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);


static struct udp_pcb *upcb;

void udp_echoserver_init(void)
{
    err_t err;

    upcb = udp_new();

    if (upcb)
    {
        err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);

        if(err == ERR_OK)
        {
            udp_recv(upcb, udp_echoserver_receive_callback, NULL);
        }
        else
        {
            udp_remove(upcb);
            upcb = NULL;
        }
    }
}

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{

    ip_addr_copy(upcb->remote_ip, *addr);
        upcb->remote_port = port;


    if (p != NULL && p->len > 0) {

        char *response = (char *)p->payload;
        uint16_t length = p->len;

        HAL_UART_Transmit(&huart2, (uint8_t *)response, length, 1000);
    }


    pbuf_free(p);
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{


	if(huart->Instance == USART2)
	    {

	        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT,Size, PBUF_RAM);



	            memcpy(p->payload, &data,Size);


	            udp_sendto(upcb, p, &upcb->remote_ip, upcb->remote_port);


	            pbuf_free(p);


        HAL_UARTEx_ReceiveToIdle_IT(&huart2, data, size);
    }
}


