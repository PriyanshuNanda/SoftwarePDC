#ifndef CMDFRAME_H   
#define CMDFRAME_H
#include <cstdint>
#include <winsock2.h>
uint16_t compute_CRC(unsigned char *message,int length);
void int_to_ascii_convertor(unsigned int n,unsigned char hex[]) ;
void long_int_to_ascii_convertor(unsigned long int n,unsigned char hex[]) ;
void byte_by_byte_copy(unsigned char dst[],unsigned char src[],int index,int n);
void create_command_frame(int type,int pmu_id,char cmdframe[]);
void sendCFGCmdFrame(SOCKET clientSocket,int pmuId);
void sendDFCmdFrame(SOCKET clientSocket,int pmuId);
void sendCloseStreamCmdFrame(SOCKET clientSocket,int pmuId);
#endif 