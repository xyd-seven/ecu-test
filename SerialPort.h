#pragma once

#define SERIAL_BAUD_RATE	115200
#define SERIAL_DATA_BIT		8
#define SERIAL_PARITY		L"NONE"	//NONE, ODD, EVEN
#define SERIAL_STOP_BIT		L"1"	//ONESTOPBIT:L"1", TWOSTOPBITS:L"2"

#define        MAXBLOCK (1024*3) //缓冲区大小
#define        XON 0x11
#define        XOFF 0x13

//#define MAX_MB_MSG_LENGTH 1024
#define MAX_RESPONSE_MICRO_SECONDS 1000

typedef enum _UserErrorType{
	MY_EC_COMMON = -2000,	
	MY_EC_SERIAL_SEND_ERROR,
	MY_EC_SERIAL_SEND_TIMEOUT,
	MY_EC_SERIAL_RECV_ERROR,
	MY_EC_SERIAL_RECV_TIMEOUT,
	MY_EC_SERIAL_TYPE_ERROR,
	MY_EC_SCM_SETTIME_ERROR,
	MY_EC_PARAM_OUT_OF_RANGE,
	MY_EC_OK = 0
}UserErrorType;

const char SERIAL_FLAG_START[1] = {0x7E};
const char SERIAL_FLAG_END[1] = {0x7F};

class SerialPort
{
public:
	SerialPort(void);
	~SerialPort(void);

	HANDLE m_hMutex;
	HANDLE m_hCom;

	//功能：	初始化（打开串口） 0：成功； 非0值：失败
	int Serial_Init(LPCTSTR m_sPort,int BaudRate);
	int Serial_Release();
	int Serial_ReadData( BYTE inbuff[], DWORD &nBytesRead );
	int Serial_WriteData( BYTE Outbuff[], int size );
	int Serial_ClearData();
};
