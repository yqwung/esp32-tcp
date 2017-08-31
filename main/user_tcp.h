#ifndef __USER_TCP_H__
#define __USER_TCP_H__

#ifdef __cplusplus
extern "C" {
#endif


#define USER_DEFAULT_TCP_PORT          4000
#define USER_DEFAULT_PKTSIZE           16

char user_udp_server(void);
void user_receive_data(void *pvParameters);


#ifdef __cplusplus
}
#endif

#endif /*#ifndef __USER_TCP_H__*/
