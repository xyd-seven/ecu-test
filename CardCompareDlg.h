// CardCompareDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "SerialPort.h"

#define BUFF_SIZE 5120
#define RECEIVE_SIZE 115
#define READ_TIME 1000

//#define START_TEST 1
//#define DISPLAY_VALUE 1

#define ST9_START 1600
#define ST9_END 1650
#define ST9_OK_DEV1 1655
#define ST9_OK_DEV2 1656
#define ST9_OK_DEV3 1657
#define ST9_OK_DEV4 1658
#define ST9_NG_DEV1 1660
#define ST9_NG_DEV2 1661
#define ST9_NG_DEV3 1662
#define ST9_NG_DEV4 1663
#define ST9_NG_IMEI 1665


struct threadParam{
	BYTE inbuff[BUFF_SIZE];
	int iRead;
	int iWrite;
	int iDiscard;
	int iFlagEnd;

	int FlagImei;
	int FlagShock;
	int FlagRsrp;
	int FlagReg;
	int FlagVol;
	int FlagTem;
	int FlagBan;
	int FlagPwr;
	int FlagSv;

	int FlagImeiLuan;
};

struct StatisticsParam{
	int iCountDev;
	int iOKCountDev;
	int iNGCountImei;
	int iNGCountShock;
	int iNGCountRsrp;
	int iNGCountReg;
	int iNGCountVol;
	int iNGCountTem;
	int iNGCountBan;
	int iNGCountPwr;
	int iNGCountSv;
};

// CCardCompareDlg 对话框
class CCardCompareDlg : public CDialog
{
// 构造
public:
	CCardCompareDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CARDCOMPARE_DIALOG };
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int RE_LOG_ShowLine(int num,const CString & s);
	afx_msg void OnBnClickedButton1(UINT nID);
	afx_msg void OnBnClickedButton2(UINT nID);
	void ImeiCompareFunc(int num,struct threadParam *param);
	void InfoCompareFunc(struct threadParam *param,CListCtrl *m_ListCtrl,CString name, CString value);
	void ClearFlag();
	void ReceiveSerialProc(int num,SerialPort* pSerialPortM,CListCtrl *m_ListCtrl,CString &m_Imei,struct threadParam *param);
	void InitDev(CComboBox* combo1,CComboBox* combo2,CListCtrl *m_ListCtrl);
	void WriteResult(int num,struct threadParam *param,unsigned int addrok,unsigned int addrng,CStdioFile* pListFile,struct StatisticsParam *countparam);
	afx_msg void OnBnClickedButton10();
	void UpdateCompareValue();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnNMCustomdrawList1(UINT nID,NMHDR *pNMHDR, LRESULT *pResult);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void GetImeiFromFile();
	void MySleep(int interval);
	void ClearDisplay();

	/*CStdioFile ListFile1;
	CStdioFile ListFile2;
	CStdioFile ListFile3;
	CStdioFile ListFile4;*/

	//CStdioFile ListFile5;

	CRichEditCtrl m_RE_Log;
	CRichEditCtrl m_RE_Log2;
	CRichEditCtrl m_RE_Log3;
	CRichEditCtrl m_RE_Log4;
	
	CListCtrl m_ListCtrl1;
	CListCtrl m_ListCtrl2;
	CListCtrl m_ListCtrl3;
	CListCtrl m_ListCtrl4;

	SerialPort SerialPortM1;
	SerialPort SerialPortM2;
	SerialPort SerialPortM3;
	SerialPort SerialPortM4;
	
	CString ImeiBuff1;
	CString ImeiBuff2;
	CString ImeiBuff3;
	CString ImeiBuff4;
	CString ImeiCom1;
	CString ImeiCom2;
	CString ImeiCom3;
	CString ImeiCom4;

	int iStartFlag;
	int iEndFlag;
	int iReadCount;

	CString str_shock;
	CString str_reg;
	CString str_rsrp;
	CString str_pwr;
	CString str_sv;
	CString str_vlower;
	CString str_vupper;
	CString str_t1lower;
	CString str_t1upper;
	CString str_t2lower;
	CString str_t2upper;
	CString str_reg_check;
	CString str_check_time;

	struct threadParam threadParamM1;
	struct threadParam threadParamM2;
	struct threadParam threadParamM3;
	struct threadParam threadParamM4;

	struct StatisticsParam devParamM1;
	struct StatisticsParam devParamM2;
	struct StatisticsParam devParamM3;
	struct StatisticsParam devParamM4;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton12();
};
