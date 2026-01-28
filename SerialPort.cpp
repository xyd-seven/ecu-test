#include "StdAfx.h"
#include "SerialPort.h"

#define InValidHeader(DATA)  memcmp(DATA, SERIAL_FLAG_START, sizeof(SERIAL_FLAG_START))

int OpenCom(HANDLE &hCom, LPCTSTR m_sPort, int BaudRate, int Databit, LPCTSTR  parity, LPCTSTR  stopbit);
int CloseCom(HANDLE &hCom);
int ReadCom(HANDLE hCom, BYTE inbuff[], DWORD &nBytesRead, int ReadTime);
int WriteCom(HANDLE hCom, BYTE Outbuff[], int size );
int ClearCom(HANDLE hCom);
void ProccessErrorComm(HANDLE hCom,BYTE* m_Inbuff, int nBytesRead);

SerialPort::SerialPort(void)
{
	m_hCom = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort(void)
{
}

//初始化串口
int SerialPort::Serial_Init(LPCTSTR m_sPort,int BaudRate)
{
	m_hMutex = CreateMutex(NULL,FALSE,NULL);
	if(m_hMutex == NULL)
	{
		//创建互斥失败
		//LOG(L"Create Mutex Error!!!!");
	}

	int ret = ERROR_FILE_NOT_FOUND;

	//尝试打开COM7，成功则立即返回，否则尝试3次。
	for(int i=0; i<3; i++)
	{
		ret = OpenCom(m_hCom, m_sPort, BaudRate, SERIAL_DATA_BIT, SERIAL_PARITY, SERIAL_STOP_BIT);
		if( ret == ERROR_SUCCESS ) 
			break;
		Sleep(3);
	}
/*
	//3次尝试打开COM1失败后，尝试打开COM2-COM9
	if(ret != ERROR_SUCCESS){
		TCHAR strCom[10] = {0};	
		for(int i=2; i<10; i++)
		{
			_snwprintf_s(strCom, _TRUNCATE, L"\\\\.\\COM%d", i);			
			ret = OpenCom(m_hCom, strCom, SERIAL_BAUD_RATE, SERIAL_DATA_BIT, SERIAL_PARITY, SERIAL_STOP_BIT);
					
			if( ret == ERROR_SUCCESS )
				break;
		}
	}*/

	return ret;	
}


//反初始化（打开串口）
int SerialPort::Serial_Release()
{
	return CloseCom(m_hCom);
}

#define InValidEnd(DATA, length )  memcmp(DATA + length - sizeof(SERIAL_FLAG_END), SERIAL_FLAG_END, sizeof(SERIAL_FLAG_END) )


int SerialPort::Serial_ReadData( BYTE inbuff[], DWORD &nBytesRead )
{
	int ret = MY_EC_OK;
	WaitForSingleObject(m_hMutex,INFINITE);
	
	ret = ReadCom( m_hCom, inbuff, nBytesRead, MAX_RESPONSE_MICRO_SECONDS);
	/*
	if( ret || InValidHeader(inbuff) )
	{
	//	LOG(L"ReadCom Error!");
		ProccessErrorComm(inbuff, nBytesRead);
		ret =  MY_EC_SERIAL_RECV_ERROR;	
	}*/
	ReleaseMutex(m_hMutex);
	return ret;
}

int SerialPort::Serial_WriteData( BYTE Outbuff[], int size )
{
	int ret = MY_EC_OK;
	WaitForSingleObject(m_hMutex,INFINITE);

	ret = WriteCom( m_hCom, Outbuff, size );
	
	ProccessErrorComm(m_hCom,Outbuff, size);
	
	ReleaseMutex(m_hMutex);
	return ret;
}
int SerialPort::Serial_ClearData(){
	return ClearCom(m_hCom);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//休眠毫秒
void MySleep(int interval)
{
	HANDLE evt = CreateEvent(NULL, TRUE, FALSE, NULL);
	WaitForSingleObject(evt, interval);
	CloseHandle(evt);
}
//打开串口
//0：ERROR_SUCCESS 成功； 其他：GetLastError()值，失败及失败代码
int OpenCom(HANDLE &hCom, LPCTSTR m_sPort, int BaudRate, int Databit, LPCTSTR  parity, LPCTSTR  stopbit)
{
	int ret = ERROR_SUCCESS;
	if( hCom != INVALID_HANDLE_VALUE)
	{
		return ret;
	}

	COMMTIMEOUTS TimeOuts;                                ///串口输出时间 超时设置
	DCB dcb;                                              ///与端口匹配的设备    
	hCom=CreateFile(m_sPort,	GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL); // 以重叠方式打开串口
	if(hCom==INVALID_HANDLE_VALUE)
	{   
		int error = GetLastError();
	/*	TCHAR err_msg[1024]= {0};
		if(error == ERROR_ACCESS_DENIED)
		{
			wsprintf(err_msg, L"串口被占用！");
		}else if(error == ERROR_FILE_NOT_FOUND)
		{
			wsprintf(err_msg, L"串口不存在！");
		}else{
			wsprintf(err_msg, L"打开串口出错，错误代码:%d", error);	
		}		
		MessageBox(NULL, err_msg, L"打开串口出错", MB_ICONERROR);   /////重叠方式 异步通信（INVALID_HANDLE_VALUE）函数失败。
		*/
		ret = error;
		return ret;
	}   

	BOOL bRet = SetupComm(hCom,MAXBLOCK,MAXBLOCK);              //设置缓冲区
	if(!bRet)
	{
		//LOG1(L"SetCommon Buffer Error:%d", GetLastError());
	}
	memset(&TimeOuts,0,sizeof(TimeOuts));    
	TimeOuts.ReadIntervalTimeout=MAXDWORD;           // 把间隔超时设为最大，把总超时设为0将导致ReadFile立即返回并完成操作
	TimeOuts.ReadTotalTimeoutMultiplier=0;           //读时间系数
	TimeOuts.ReadTotalTimeoutConstant=0;            //读时间常量  
	TimeOuts.WriteTotalTimeoutMultiplier=50;       //总超时=时间系数*要求读/写的字符数+时间常量
	TimeOuts.WriteTotalTimeoutConstant=2000;       //设置写超时以指定WriteComm成员函数中的                                              
	SetCommTimeouts(hCom, &TimeOuts);           //GetOverlappedResult函数的等待时间*/
	if(!GetCommState(hCom, &dcb))               ////串口打开方式、端口、波特率 与端口匹配的设备
	{
		//MessageBox(NULL, L"GetCommState Failed", L"", MB_ICONERROR);
		return ERROR_INVALID_FUNCTION;
	}

	dcb.fParity=TRUE;                          //允许奇偶校验        
	dcb.fBinary=TRUE;
	if(wcscmp(parity,L"NONE")==0)
	{
		dcb.Parity=NOPARITY;
	}else if(wcscmp(parity,L"ODD")==0)
	{
		dcb.Parity=ODDPARITY;
	}else if(wcscmp(parity,L"EVEN")==0)
	{
		dcb.Parity=EVENPARITY;
	}

	if(wcscmp(stopbit,L"1")==0)//设置波特率
	{
		dcb.StopBits=ONESTOPBIT;
	}else if(wcscmp(stopbit,L"2")==0)//设置波特率
	{
		dcb.StopBits=TWOSTOPBITS;          
	}
	BOOL m_bEcho=FALSE;                        ///
	int m_nFlowCtrl=0;
	BOOL m_bNewLine=FALSE;                     ///
	dcb.BaudRate=BaudRate;                     // 波特率
	dcb.ByteSize=Databit;                     // 每字节位数    
	// 硬件流控制设置
	dcb.fOutxCtsFlow=m_nFlowCtrl==1;
	dcb.fRtsControl=m_nFlowCtrl==1    ?RTS_CONTROL_HANDSHAKE:RTS_CONTROL_ENABLE;    
	// XON/XOFF流控制设置（软件流控制！）
	dcb.fInX=dcb.fOutX=m_nFlowCtrl==2;
	dcb.XonChar=XON;
	dcb.XoffChar=XOFF;
	dcb.XonLim=50;
	dcb.XoffLim=50;    
	if( SetCommState(hCom, &dcb) )     
	{
		ret = ERROR_SUCCESS;         ////com的通讯口设置   
	}
	else
	{
		/*MessageBox(NULL, L"串口已打开，设置失败", L"", MB_ICONERROR);*/
		ret = GetLastError();
	}

	PurgeComm(hCom,PURGE_RXCLEAR | PURGE_TXCLEAR);

	return ret;
} 

//关闭串口
int CloseCom(HANDLE &hCom)
{
	BOOL bRet = CloseHandle(hCom);
	if(!bRet)
	{
		return GetLastError();
	}
	hCom = INVALID_HANDLE_VALUE;
	return ERROR_SUCCESS;
}

/*
如果在ReadTime时间内，还没有预期量的数据，则读出现成的部分数据，并返回MY_EC_SERIAL_RECV_ERROR表示失败。
应用层需要进行同步（丢弃残余数据，直到遇到预期标识位）。
如果有>=预期量的数据，则读取预期量的数据，并返回MY_EC_OK表示成功。
*/
int ReadCom(HANDLE hCom, BYTE inbuff[], DWORD &nBytesRead, int ReadTime)
{
	DWORD lrc;                                 ///纵向冗余校验
	DWORD endtime;                            /////////jiesuo
	static OVERLAPPED ol;
	int ReadNumber=0;    
	int numCount = 0 ;                             //控制读取的数目
	DWORD dwErrorMask;  
	DWORD nToRead = nBytesRead;
	nBytesRead = 0;
	COMSTAT comstat;    
	ol.Offset = 0;                            ///相对文件开始的字节偏移量
	ol.OffsetHigh =0 ;                        ///开始传送数据的字节偏移量的高位字，管道和通信时调用进程可忽略。
	ol.hEvent = NULL;                         ///标识事件，数据传送完成时设为信号状态
	ol.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);    
	endtime = GetTickCount() + ReadTime;//GetTickCount()取回系统开始至此所用的时间(毫秒) 
	memset(inbuff, 0, nToRead);
	BOOL bRet = ClearCommError(hCom,&dwErrorMask,&comstat);
	if( !bRet )
	{
		int ecode = GetLastError();
		return ecode;
	}
	while( comstat.cbInQue < nToRead && GetTickCount() < endtime )
	{
		bRet = ClearCommError(hCom,&dwErrorMask,&comstat);
		if(!bRet)
		{
			int ecode = GetLastError();
			return ecode;
		}
		MySleep(100);
	}
	/*
	如果在ReadTime时间内，还没有预期量的数据，则读出现成的部分数据，并返回MY_EC_SERIAL_RECV_ERROR表示失败。应用层需要进行同步（丢弃残余数据，直到遇到预期标识位）。
	如果有>=预期量的数据，则读取预期量的数据，并返回MY_EC_OK表示成功。
	*/
	int nTempRead = min(nToRead, comstat.cbInQue);
	if ( nTempRead == 0 ) //xyy 2014.04.28
	{
		return MY_EC_SERIAL_RECV_ERROR;
	}

	if(!ReadFile( hCom, inbuff, nTempRead, &nBytesRead, &ol ) )
	{    
		if(( lrc = GetLastError()) == ERROR_IO_PENDING)
		{
			///////////////////
			endtime = GetTickCount() + ReadTime;//GetTickCount()取回系统开始至此所用的时间(毫秒)
			while(!GetOverlappedResult(hCom,&ol,&nBytesRead,FALSE))//该函数取回重叠操作的结果
			{
				if(GetTickCount()>endtime)
					break;
			}    
		}        
	}
	/*
	if( nBytesRead < nToRead ) 
		return MY_EC_SERIAL_RECV_ERROR;*/

	return MY_EC_OK;
}

//返回值：0成功；负值：代表不同的失败原因
int WriteCom(HANDLE hCom, BYTE Outbuff[], int size )
{
	int ret = 0;

	DWORD nBytesWrite,endtime,lrc;
	static OVERLAPPED ol;
	DWORD dwErrorMask,dwError;
	COMSTAT comstat;
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	ol.Offset=0;              
	ol.OffsetHigh=0;
	ol.hEvent=NULL;               ///标识事件，数据传送完成时，将它设为信号状态
	ClearCommError(hCom,&dwErrorMask,&comstat);
	if(!WriteFile(hCom,Outbuff,size,&nBytesWrite,&ol)) 
	{
		if((lrc=GetLastError())==ERROR_IO_PENDING)
		{
			endtime=GetTickCount()+10000;
			while(!GetOverlappedResult(hCom,&ol,&nBytesWrite,FALSE))
			{   
				dwError=GetLastError();
				if(GetTickCount()>endtime)
				{ 
					/*
					MessageBox(NULL, L"写串口时间过长!", L"", MB_ICONERROR);*/
					ret = MY_EC_SERIAL_SEND_TIMEOUT;
					break;
				}
				if(dwError == ERROR_IO_INCOMPLETE) 
					continue;          //未完全读完时的正常返回结果 
				else
				{
					//    发生错误，尝试恢复！
					ClearCommError(hCom,&dwError,&comstat);
					ret = MY_EC_SERIAL_SEND_ERROR;
					break;
				}
			}
		}
	}    
	FlushFileBuffers(hCom);
	return ret;
}

//清空接收和发送缓冲区中的残余数据,返回值：0成功
int ClearCom(HANDLE hCom)
{
	PurgeComm(hCom,PURGE_RXCLEAR | PURGE_TXCLEAR);
	//PurgeComm(hCom,PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_TXABORT);
	return 0;
}

void ProccessErrorComm(HANDLE hCom,BYTE* m_Inbuff, int nBytesRead)
{
	/*TCHAR msg[10240] = {0};
	for(int i=0; i<nBytesRead; i++)
	{
		swprintf(msg+i*3, _TRUNCATE, L"%02X ", m_Inbuff[i]);
	}
	LOG2(L"Read %d Bytes:%s", nBytesRead, msg);*/
	ClearCom(hCom);
}