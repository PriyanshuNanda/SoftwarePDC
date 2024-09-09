// #include <bits/stdc++.h>
#include"cmdframe.h"
#include <chrono>
#include <iostream>
#include<winsock2.h>

using namespace std;
unsigned char DATASYNC[2],CFGSYNC[2],CMDSYNC[2],CMDDATASEND[2],CMDDATAOFF[2],CMDCFGSEND[2];
// 
void cmdFrameInit(){
    CMDSYNC[0] = 0xaa;
	CMDSYNC[1] = 0x41;
	// CMDSYNC[2] = '\0';

	CMDCFGSEND[0] = 0x00;
	CMDCFGSEND[1] = 0x05;
	// CMDCFGSEND[2] = '\0'; 

	CMDDATASEND[0] = 0x00;
	CMDDATASEND[1] = 0x02;
	// CMDDATASEND[2] = '\0'; 

	CMDDATAOFF[0] = 0x00;
	CMDDATAOFF[1] = 0x01;
	// CMDDATAOFF[2] = '\0'; 

	DATASYNC[0] = 0xaa;
	DATASYNC[1] = 0x01;
	// DATASYNC[2] = '\0';

	CFGSYNC[0] = 0xaa;
	CFGSYNC[1] = 0x31;
	// CFGSYNC[2] = '\0'
}

uint16_t compute_CRC(unsigned char *message,int length)
{
	uint16_t crc=0x0ffff,temp,quick;
	int i;

	for(i=0;i<length;i++)
	{
		temp=(crc>>8)^message[i];
		crc<<=8;
		quick=temp ^ ( temp >>4);
		crc ^=quick;
		quick<<=5;
		crc ^=quick;
		quick <<=7;
		crc ^= quick;
	}
	return crc;
}



void int_to_ascii_convertor(unsigned int n,unsigned char hex[]) {

	hex[0] = n >> 8;
	hex[1] = n ;
	// cout<<hex[0]<< " " <<hex[1]<<endl;
}

void long_int_to_ascii_convertor(unsigned long int n,unsigned char hex[]) {

	hex[0] = n >> 24;
	hex[1] = n >> 16;
	hex[2] = n >> 8;
	hex[3] = n ;
}

void byte_by_byte_copy(unsigned char dst[],unsigned char src[],int index,int n) {

	int i;
	for(i = 0;i<n; i++) 
		dst[index + i] = src[i];					
}

void create_command_frame(int type,int pmu_id,char cmdframe[]) {

	int f = 18;
	long int sec,frac = 0;
	cmdFrameInit();
	unsigned char fsize[2],pmuid[2],soc[4],fracsec[4];
	uint16_t chk;

	// memset(cmdframe,'\0',18);
	// memset(fsize,'\0',2);

	int_to_ascii_convertor(f,fsize);
	int_to_ascii_convertor(pmu_id,pmuid);

	sec= (long int)time (NULL);
    // std::cout<<sec<<endl; 
	long_int_to_ascii_convertor(sec,soc);
	long_int_to_ascii_convertor(frac,fracsec);

	int index = 0;

	switch(type) {

	case 1 : byte_by_byte_copy((unsigned char *)cmdframe,CMDSYNC,index,2); // SEND CFG
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,fsize,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,pmuid,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,soc,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,fracsec,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,CMDCFGSEND,index,2);
	index += 2;
	chk = compute_CRC((unsigned char *)cmdframe,index);
	cmdframe[index++] = (chk >> 8) & ~(~0<<8);  	/* CHKSUM high byte; */
	cmdframe[index] = (chk ) & ~(~0<<8);     	/* CHKSUM low byte;  */
	break;

	case 2 : byte_by_byte_copy((unsigned char *)cmdframe,CMDSYNC,index,2);  // SEND DATA ON
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,fsize,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,pmuid,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,soc,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,fracsec,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,CMDDATASEND,index,2);
	index += 2;
	chk = compute_CRC((unsigned char *)cmdframe,index);
	cmdframe[index++] = (chk >> 8) & ~(~0<<8);  	/* CHKSUM high byte; */
	cmdframe[index] = (chk ) & ~(~0<<8);     	/* CHKSUM low byte;  */
	break;

	case 3 : byte_by_byte_copy((unsigned char *)cmdframe,CMDSYNC,index,2);  // PUT OFF DATA TRANSMISSION
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,fsize,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,pmuid,index,2);
	index += 2;
	byte_by_byte_copy((unsigned char *)cmdframe,soc,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,fracsec,index,4);
	index += 4;
	byte_by_byte_copy((unsigned char *)cmdframe,CMDDATAOFF,index,2);
	index += 2;
	chk = compute_CRC((unsigned char *)cmdframe,index);
	cmdframe[index++] = (chk >> 8) & ~(~0<<8);  	/* CHKSUM high byte; */
	cmdframe[index] = (chk ) & ~(~0<<8);     	/* CHKSUM low byte;  */
	break;

	default: std::cout<<"Please enter a valid request?\n";
	break;
	}
};



void sendCFGCmdFrame(SOCKET clientSocket,int pmuId){
    unsigned char cmdframe[18];
    create_command_frame(1, pmuId, (char *)cmdframe); // 1 = send CFG, 2 = data on , 3 = data off
    if (send(clientSocket, (char *)cmdframe, sizeof(cmdframe), 0) == SOCKET_ERROR)
    {
        printf("send failed from cfg ask: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
}
void sendDFCmdFrame(SOCKET clientSocket,int pmuId){
    unsigned char cmdframe[18];
    create_command_frame(2, pmuId, (char *)cmdframe); // 1 = send CFG, 2 = data on , 3 = data off
    if (send(clientSocket, (char *)cmdframe, sizeof(cmdframe), 0) == SOCKET_ERROR)
    {
        printf("send failed from df ask: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
}
void sendCloseStreamCmdFrame(SOCKET clientSocket,int pmuId){
    unsigned char cmdframe[18];
    create_command_frame(3, pmuId, (char *)cmdframe); // 1 = send CFG, 2 = data on , 3 = data off
    if (send(clientSocket, (char *)cmdframe, sizeof(cmdframe), 0) == SOCKET_ERROR)
    {
        printf("send failed from closeDataStream: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
}