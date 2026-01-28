#include "stdafx.h"
//#include "afxwin.h"

//#include <stdio.h>
//#include <string.h>
//#include <atlstr.h>

#include "sn.h"

//功能：从指定的imsi查找到对应的sn
//ret: 成功返回对应的sn号，失败返回0
uint64_t getSn(uint64_t imsi)
{
    int16_t pos = 0;
    int16_t posMax = 0;
    int16_t posMin = 0;
    
    //IMSI数组已经排序，使用二分法来查找
    posMin = 0;
    pos = SN_NUM / 2;
    posMax = SN_NUM - 1;
    while (1)
    {
        if (imsi == IMSI_CONST[pos])
        {
            break; //查找成功
        }
        else if (imsi > IMSI_CONST[pos])
        {
            posMin = pos + 1;
            if (posMin > posMax)
            {
                return 0; //查找失败
            }
            
            pos = ((posMax - posMin) / 2) + posMin;  
        }
        else if (imsi < IMSI_CONST[pos])
        {
            posMax = pos - 1;
            if (posMax <  posMin)
            {
                return 0; //查找失败
            }
            
            pos = ((posMax - posMin) / 2) + posMin;  
        }
    }
    
    return SN_CONST[pos];
}

//把imsi的格式转换成整形
//imsi的值类似为520039400014987；固定长度15
uint64_t imsi2num(uint8_t* imsi)
{
    int8_t i = 0;    
    uint64_t value = 0;
    uint64_t multiplier = 1;
    
    for (i = 14; i >= 0; i--)
    {
        value += (imsi[i] - '0') * multiplier;
        multiplier = multiplier * 10;
    }
    
    return value;
}

//把sn的格式转换成字符串，存储在snStr中
//sn的值类似为1820021711005；固定长度13
void sn2string(uint8_t* snStr, uint64_t snNum)
{
    int8_t i = 0;
    uint64_t multiplier = 1;
    
    for (i = 12; i >= 0; i--)
    {
       snStr[i] =  ((snNum / multiplier) % 10) + '0';
       multiplier = multiplier * 10;
    }
}

//通过imsi的值取得对应的sn值，imsi通过value传入，获取的sn的值通过value传出
//ret: 0 成功，-1 查询失败
int8_t imsi2sn(uint8_t* value)
{
    uint64_t valNum = 0;
    
    valNum = imsi2num(value+1);//value第一个字节是':',之后是ismi号
    valNum = getSn(valNum);
    
    if (valNum == 0)
    {
        return -1;
    }
    else
    {
        sn2string(value, valNum);
    }
    
    return 0;
}

void printUchar(const uint8_t* value, uint8_t len)
{
	uint8_t i = 0;

    for (i = 0; i < len; i++)
	{
		//printf("%02X ", value[i]);
		//printf("%c ", value[i] - '0');
		printf("%c", value[i]);
	}

	printf("\n");
}

#define SN_TXT _T("D:\\GZQ_ZDH_CHECK\\ei_sn_si.txt")

//文件的格式为：一行一条数据，数据格式例如：IMEI:865820031161420 S/N:1830021744248 IMSI:460040510903684
void updateSnFile(CString data)
{
	/*CStdioFile ListFile;

	ListFile.Open(SN_TXT, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
	ListFile.SeekToEnd();
	ListFile.WriteString(data);
	ListFile.Close();*/
}

//通过imsi的值取得对应的sn值，获取后，直接在valCstring中进行替代
//valCstring长度不正确，不做任何处理
//查询不到对应的sn，sn对应的字段为空; 
//该函数x86程序专用
//原数据示例:IMEI:865820031161420IMSI:460040510903684
//转换后数据示例:IMEI:865820031161420S/N:1830021744248
void getSnX86(CString & valCstring)
{
	u8 dataLen = 0;
	int i = 0;
	uint8_t value[64];
	uint8_t fileData[64];
	CString fileString;

	if (valCstring.GetLength() < 40)
	{
		return; //长度不到40，valCstring.GetAt(i)会导致程序死掉
	}

	for (i = 0; i < 40; i++)
	{
		value[i] = valCstring.GetAt(i);
	}

	for (i = 0; i < 20; i++)
	{
		fileData[i] = valCstring.GetAt(i);
	}
    fileData[20] = ' ';
	fileData[38] = ' ';
	for (i = 20; i < 40; i++)
	{
		fileData[i+19] = valCstring.GetAt(i);
	}
	fileData[59] = '\n';

	//printUchar(value, 40);

	//获取sn
    value[20] = 'S';
    value[21] = '/';
    value[22] = 'N';
    value[23] = ':';
                
    //sn号固定13位数字
    if (imsi2sn(value+24) == -1)
	{          
		dataLen = 24; //没有对应的SN号，无数据
    }
    else
    {
		dataLen = 37;

		for (i = 20; i < 37; i++)
	    {
		    fileData[i+1] = value[i];
	    }

		fileString.Empty();
	    for (i = 0; i < 60; i++)
	    {
		    fileString.Insert(i, fileData[i]);
	    }

		updateSnFile(fileString); //只有当imsi，sn, imei都正确才会有输出
    }

	//printUchar(value, dataLen);

	valCstring.Empty();
	for (i = 0; i < dataLen; i++)
	{
		valCstring.Insert(i, value[i]);
	}

	//printCstring(ImeiCom1);
	//printf("%S\n", ImeiCom1);
}

void test_imsi2sn(void)
{
    //uint8_t value[15] = "520039400014987";
    uint8_t value[] = "460045733800921";
    
    imsi2sn(value);
    
    return;
}

void test_sn2tring(void)
{
    uint64_t snNum1 = 1820021711350;
    //uint64_t snNum2 = 0020021711005; //识别为8进制数
    uint64_t snNum2 = 20021711005; 
    uint8_t snStr[13];
    
    sn2string(snStr, snNum1);
    sn2string(snStr, snNum2);
    snNum1 = 0;
}

void test_imsi2num(void)
{
    uint8_t imsi1[] = "520039400014000";
    uint8_t imsi2[] = "000039400014111";
    uint8_t imsi3[] = "460045733800921";
    uint64_t num = 0;
    
    num = imsi2num(imsi1);
    num = imsi2num(imsi2);
    num = imsi2num(imsi3);
    num = 0;
}

void testSn(void)
{
    uint64_t imsi1 = 520039400014987;  
    uint64_t imsi2 = 520039400015706; 
    uint64_t imsi3 = 520039400015769; 
    uint64_t imsi4 = 520039400017386; 
    uint64_t sn = 0;
    
    sn = getSn(imsi1);
    sn = getSn(imsi2);
    sn = getSn(imsi3);
    sn = getSn(imsi4);
    sn = 0;
}


 