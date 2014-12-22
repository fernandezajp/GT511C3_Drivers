// This software is based on the source code of SDK Fingerprint Demo of Beijing Smackbio Technology Co., Ltd
// and is published under the GPL license because there is no prior indication of this.
// Copyright (C) Alvaro Fernandez

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#pragma once

#include "CCommSerial.h"

#define CMD_OPEN					0x01
#define CMD_CLOSE					0x02
#define CMD_USBINTERNALCHECK		0x03
#define CMD_CHANGEBAUDRATE			0x04
#define CMD_SETIAPMODE				0x05
#define CMD_CMOSLED					0x12
#define CMD_GETENROLLCOUNT			0x20
#define CMD_CHECKENROLLED			0x21
#define CMD_ENROLLSTART				0x22
#define CMD_ENROLL1					0x23
#define CMD_ENROLL2					0x24
#define CMD_ENROLL3					0x25
#define CMD_ISPRESSFINGER			0x26
#define CMD_DELETEID				0x40
#define CMD_DELETEALL				0x41
#define CMD_VERIFY					0x50
#define CMD_IDENTIFY				0x51
#define CMD_VERIFYTEMPLATE			0x52
#define CMD_IDENTIFYTEMPLATE		0x53
#define CMD_CAPTUREFINGER			0x60
#define CMD_MAKETEMPLATE			0x61
#define CMD_GETIMAGE				0x62
#define CMD_GETRAWIMAGE				0x63
#define CMD_GETTEMPLATE				0x70
#define CMD_SETTEMPLATE				0x71
#define CMD_GETDATABASESTART		0x72
#define CMD_GETDATABASEEND			0x73
#define CMD_UPGRADEFIRMWARE			0x80
#define CMD_UPGRADEISOCDIMAGE		0x81
#define CMD_ACK						0x30
#define CMD_NACK					0x31

#define NACK_IDENTIFY_FAILED		0x1008
#define NACK_DB_IS_EMPTY			0x100A

#define OK							0
#define DEVICE_ID					0x01
#define COMMAND_START_CODE1			0x55
#define COMMAND_START_CODE2			0xAA
#define DATA_START_CODE1			0x5A
#define DATA_START_CODE2			0xA5
#define HEADER_SIZE					2
#define DEV_ID_SIZE					2

#define RESP_PKT					CMD_PKT
#define PKT_SIZE					sizeof(CMD_PKT)
#define CHK_SUM_SIZE				2
#define COMM_DEF_TIMEOUT			3000
#define FP_TEMPLATE_SIZE			498

#define PKT_ERR_START				-500
#define PKT_COMM_ERR				PKT_ERR_START+1
#define PKT_HDR_ERR					PKT_ERR_START+2
#define PKT_DEV_ID_ERR				PKT_ERR_START+3
#define PKT_CHK_SUM_ERR				PKT_ERR_START+4
#define PKT_PARAM_ERR				PKT_ERR_START+5


#define IMG8BIT_SIZE	256*256

enum { NONE = -2000, COMM_ERR, };

typedef struct {		
	unsigned char 	Head1;		
	unsigned char 	Head2;		
	unsigned char	wDevId0;
	unsigned char	wDevId1;
	unsigned char	nParam0;
	unsigned char	nParam1;
	unsigned char	nParam2;
	unsigned char	nParam3;
	unsigned char	wCmd0;
	unsigned char	wCmd1;
	unsigned char 	wChkSum0;
	unsigned char 	wChkSum1;
} CMD_PKT;

typedef struct _devinfo
{
	DWORD FirmwareVersion;
	DWORD IsoAreaMaxSize;
	BYTE DeviceSerialNumber[16];
} devinfo;

#pragma pack(1)
struct FP_BITMAP
{
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFO bmInfo;
	RGBQUAD bmiColors[255];
	
	FP_BITMAP(int cx, int cy)
	{
		bmfHdr.bfType = ((WORD) ('M' << 8) | 'B');  // "BM"
		bmfHdr.bfSize = sizeof(FP_BITMAP) + cx*cy;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = sizeof(FP_BITMAP);
		
		bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth = cx;
		bmInfo.bmiHeader.biHeight	= -cy;
		bmInfo.bmiHeader.biPlanes	= 1;
		bmInfo.bmiHeader.biBitCount = 8;
		bmInfo.bmiHeader.biCompression = 0;
		bmInfo.bmiHeader.biSizeImage = cx*cy;
		bmInfo.bmiHeader.biXPelsPerMeter = 0;
		bmInfo.bmiHeader.biYPelsPerMeter = 0;
		bmInfo.bmiHeader.biClrUsed = 0;
		bmInfo.bmiHeader.biClrImportant = 0;
		
		RGBQUAD *pals = bmInfo.bmiColors;
		for (int i = 0; i < 256; i++) {
			pals[i].rgbBlue = i;
			pals[i].rgbGreen = i;
			pals[i].rgbRed = i;
			pals[i].rgbReserved = 0;
		}
	}
};
#pragma pack()

class CGT511C3
{
public:
	BYTE	gbyImg8bit[IMG8BIT_SIZE];

	CGT511C3();
	CGT511C3(int, DWORD);
	virtual ~CGT511C3();

	int open(void);
	int change_baudrate(int);
	int cmosLed(BOOL);
	int getEnrollCount(void);
	int checkEnrolled(int);
	int enroll_start(int);
	int enroll_num(int);
	int isPressFinger(void);
	int deleteId(int);
	int deleteAll(void);
	int verify(int);
	int identify(void);
	int verifyTemplate(int);
	int identifyTemplate(void);
	int captureFinger(BOOL);
	int makeTemplate(void);
	int capture(BOOL);
	int getImage(void);
	int getRawImage(void);
	int getTemplate(int);
	DWORD FirmwareVersion(void);
	DWORD IsoAreaMaxSize(void);
	void close(void);
	
	WORD gwLastAck;
	int  gwLastAckParam;
	BYTE byTemplate[FP_TEMPLATE_SIZE];
	bool status;
	
private:
	CCommSerial commport;
	WORD uwDevID;
	DWORD CommTimeOut;
	devinfo gDevInfo;

	int executeCmd(WORD, int);
	int SendCmd(WORD, WORD, int);
	int ReceiveCmd(WORD, WORD*, int*);
	int SendData(WORD, BYTE* , int);
	int ReceiveData(WORD, BYTE*, int, DWORD);
	WORD CalcChkSumOfCmd(CMD_PKT*);
	WORD CalcChkSumOfDataPkt( BYTE*, int );
};
