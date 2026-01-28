// CardCompareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CardCompare.h"
#include "CardCompareDlg.h"
#include "interface.h"
#include <locale>
//#include <iostream>
#include <fstream>


#define SHOCK	_T("shock")
#define RSRP	_T("rsrp")
#define CELLID	_T("cellid")
#define SNR		_T("snr")
#define REG		_T("reg")
#define LAT		_T("lat")
#define LON		_T("lon")
#define V		_T("v")
#define T		_T("t")
#define T1		_T("t1")
#define T2		_T("t2")
#define PWR		_T("pwr")
#define SV		_T("sv")

#define SHOCK_ROW		0
#define SHOCK_COLUMN	0
#define PWR_ROW			1
#define PWR_COLUMN		2
#define REG_ROW			1
#define REG_COLUMN		0
#define SV_ROW			0
#define SV_COLUMN		2
#define V_ROW			2
#define V_COLUMN		2
#define RSRP_ROW		2
#define RSRP_COLUMN		0
#define CELLID_ROW		4
#define CELLID_COLUMN	0
#define SNR_ROW			4
#define SNR_COLUMN		2
#define LAT_ROW			5
#define LAT_COLUMN		0
#define LON_ROW			5
#define LON_COLUMN		2
#define T1_ROW			3
#define T1_COLUMN		0
#define T2_ROW			3
#define T2_COLUMN		2




#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

#define INI_DIR _T("D:\\GZQ_ZDH_CHECK")
#define INI_PATH _T("D:\\GZQ_ZDH_CHECK\\GZQ_ZDH_Info_Compare.ini")
#define LOG_PATH_1 _T("D:\\GZQ_ZDH_CHECK\\dev1.log")
#define LOG_PATH_2 _T("D:\\GZQ_ZDH_CHECK\\dev2.log")
#define LOG_PATH_3 _T("D:\\GZQ_ZDH_CHECK\\dev3.log")
#define LOG_PATH_4 _T("D:\\GZQ_ZDH_CHECK\\dev4.log")
#define LOG_PATH_5 _T("D:\\GZQ_ZDH_CHECK\\devcount.log")

UINT ThreadFunc1(LPVOID lParam);
UINT ThreadFunc2(LPVOID lParam);
UINT ThreadFunc3(LPVOID lParam);
UINT ThreadFunc4(LPVOID lParam);

using namespace std;

//______________________________________________________________________________________________________

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//______________________________________________________________________________________________________


// 用于应用程序“配置”菜单项的对话框
class CSystemSetDlg : public CDialog
{
public:
	CSystemSetDlg(CWnd* pParent = NULL);

// 对话框数据
	enum { IDD = IDD_DIALOG_SYSTEMSET };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CCardCompareDlg* CompareDlg;
};

CSystemSetDlg::CSystemSetDlg(CWnd* pParent /*=NULL*/) : CDialog(CSystemSetDlg::IDD)
{
	CompareDlg = (CCardCompareDlg*)pParent;
}
BOOL CSystemSetDlg::OnInitDialog()
{
	CString str_edit;
	GetPrivateProfileString(_T("systemset"),_T("shock"),_T("1"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_SHOCK, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("reg"),_T("1"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_REG, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("rsrp"),_T("-200"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_RSRP, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("pwr"),_T("1"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_PWR, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("sv"),_T("20"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_SV, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("vlower"),_T("3.5"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_VLOWER, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("vupper"),_T("4.1"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_VUPPER, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("t1lower"),_T("1.0"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_T1LOWER, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("t1upper"),_T("2.0"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_T1UPPER, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("t2lower"),_T("20"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_T2LOWER, str_edit);
	GetPrivateProfileString(_T("systemset"),_T("t2upper"),_T("40"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_T2UPPER, str_edit);
	
	GetPrivateProfileString(_T("switchset"),_T("reg"),_T("1"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	int iCheck = _ttoi(str_edit);
	if(iCheck == 1){
		((CButton*)GetDlgItem(IDC_RADIO_REGYES))->SetCheck(1);
	}
	else{
		((CButton*)GetDlgItem(IDC_RADIO_REGNO))->SetCheck(1);
	}

	GetPrivateProfileString(_T("otherset"),_T("checktime"),_T("15"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	SetDlgItemText(IDC_EDIT_CHECK_TIME, str_edit);
	return TRUE;
}

void CSystemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CSystemSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str_edit(_T(""));
	GetDlgItemText(IDC_EDIT_SHOCK, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("shock"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_REG, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("reg"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_RSRP, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("rsrp"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_PWR, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("pwr"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_SV, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("sv"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_VLOWER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("vlower"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_VUPPER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("vupper"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_T1LOWER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("t1lower"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_T1UPPER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("t1upper"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_T2LOWER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("t2lower"),str_edit,INI_PATH);
	GetDlgItemText(IDC_EDIT_T2UPPER, str_edit);
	WritePrivateProfileString(_T("systemset"),_T("t2upper"),str_edit,INI_PATH);
	
	if(((CButton*)GetDlgItem(IDC_RADIO_REGYES))->GetCheck()){
		WritePrivateProfileString(_T("switchset"),_T("reg"),_T("1"),INI_PATH);
	}
	else{
		WritePrivateProfileString(_T("switchset"),_T("reg"),_T("0"),INI_PATH);
	}

	GetDlgItemText(IDC_EDIT_CHECK_TIME, str_edit);
	WritePrivateProfileString(_T("otherset"),_T("checktime"),str_edit,INI_PATH);

	CompareDlg->UpdateCompareValue();

	OnOK();
}

BEGIN_MESSAGE_MAP(CSystemSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSystemSetDlg::OnBnClickedOk)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

//______________________________________________________________________________________________________


// 用于应用程序“输入密码”菜单项的对话框

class CPasswordDlg : public CDialog
{
public:
	CPasswordDlg(CWnd* pParent = NULL);

// 对话框数据
	enum { IDD = IDD_DIALOG_PASSWORD };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CCardCompareDlg* CompareDlg;
};

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/) : CDialog(CPasswordDlg::IDD)
{
	CompareDlg = (CCardCompareDlg*)pParent;
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CPasswordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str_edit(_T(""));
	GetDlgItemText(IDC_EDIT_PASSWORD, str_edit);
	CString str_read;
	GetPrivateProfileString(_T("systemset"),_T("password"),_T("67679999"),str_read.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	if(str_edit.Compare(str_read)==0){
		OnOK();
		CSystemSetDlg dlg(CompareDlg);
		dlg.DoModal();
	}
	else{
		SetDlgItemText(IDC_STATIC_PASSWORD, _T("密码错误，请重新输入。"));
		SetDlgItemText(IDC_EDIT_PASSWORD, _T(""));
	}	
}
BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//______________________________________________________________________________________________________


// CCardCompareDlg 对话框

CCardCompareDlg::CCardCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardCompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	ClearFlag();
	CFileFind m_FileFind;
	CString m_sFilePath = INI_DIR;
	if(!m_FileFind.FindFile(m_sFilePath))  //路径不存在则创建该路径
	{
		CreateDirectory(m_sFilePath,NULL);
	}
}

void CCardCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl1);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl3);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl4);
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_RE_Log);
	DDX_Control(pDX, IDC_RICHEDIT_LOG2, m_RE_Log2);
	DDX_Control(pDX, IDC_RICHEDIT_LOG3, m_RE_Log3);
	DDX_Control(pDX, IDC_RICHEDIT_LOG4, m_RE_Log4);
}

BEGIN_MESSAGE_MAP(CCardCompareDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BUTTON1, &CCardCompareDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CCardCompareDlg::OnBnClickedButton2)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON1, IDC_BUTTON4, &CCardCompareDlg::OnBnClickedButton1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON5, IDC_BUTTON8, &CCardCompareDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, &CCardCompareDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CCardCompareDlg::OnBnClickedButton11)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CCardCompareDlg::OnNMCustomdrawList1)
	ON_NOTIFY_RANGE(NM_CUSTOMDRAW, IDC_LIST1, IDC_LIST4,&CCardCompareDlg::OnNMCustomdrawList1)
END_MESSAGE_MAP()


// CCardCompareDlg 消息处理程序

BOOL CCardCompareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	InitDev(( CComboBox*)GetDlgItem(IDC_COMBO1),( CComboBox*)GetDlgItem(IDC_COMBO2),&m_ListCtrl1);
	InitDev(( CComboBox*)GetDlgItem(IDC_COMBO3),( CComboBox*)GetDlgItem(IDC_COMBO4),&m_ListCtrl2);
	InitDev(( CComboBox*)GetDlgItem(IDC_COMBO5),( CComboBox*)GetDlgItem(IDC_COMBO6),&m_ListCtrl3);
	InitDev(( CComboBox*)GetDlgItem(IDC_COMBO7),( CComboBox*)GetDlgItem(IDC_COMBO8),&m_ListCtrl4);
	
	//打开日志文件
#if 0
	TCHAR exepath[MAX_PATH];
	CString  strdir,tmpdir; 
	memset(exepath,0,MAX_PATH); 
	GetModuleFileName(NULL,exepath,MAX_PATH); 
	tmpdir=exepath; 
	strdir=tmpdir.Left(tmpdir.ReverseFind('\\'));
	strdir.Format(_T("%s\\log.txt"),strdir);
#endif

	CString str_edit;
	GetPrivateProfileString(_T("dev1"),_T("com"),_T("6"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	CComboBox* combo= ( CComboBox*)GetDlgItem(IDC_COMBO1);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev1"),_T("rate"),_T("4"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO2);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev2"),_T("com"),_T("7"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO3);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev2"),_T("rate"),_T("4"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO4);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev3"),_T("com"),_T("8"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO5);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev3"),_T("rate"),_T("4"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO6);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev4"),_T("com"),_T("9"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO7);
	combo->SetCurSel(_ttoi(str_edit));
	GetPrivateProfileString(_T("dev4"),_T("rate"),_T("4"),str_edit.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	combo= ( CComboBox*)GetDlgItem(IDC_COMBO8);
	combo->SetCurSel(_ttoi(str_edit));

	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);

	OnBnClickedButton9();

	UpdateCompareValue();
	
	AfxBeginThread(ThreadFunc1, (LPVOID)this);
	AfxBeginThread(ThreadFunc2, (LPVOID)this);
	AfxBeginThread(ThreadFunc3, (LPVOID)this);
	AfxBeginThread(ThreadFunc4, (LPVOID)this);

	SetTimer(TIMER1,READ_TIME,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCardCompareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE){
		KillTimer(TIMER1);
		KillTimer(TIMER2);
		KillTimer(TIMER3);
		/*ListFile1.Close();
		ListFile2.Close();
		ListFile3.Close();
		ListFile4.Close();*/
		SerialPortM1.Serial_Release();
		SerialPortM2.Serial_Release();
		SerialPortM3.Serial_Release();
		SerialPortM4.Serial_Release();
		OnBnClickedButton12();
		CDialog::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCardCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CCardCompareDlg::ClearFlag()
{
	ImeiBuff1=_T("");
	ImeiBuff2=_T("");
	ImeiBuff3=_T("");
	ImeiBuff4=_T("");
	ImeiCom1=_T("");
	ImeiCom2=_T("");
	ImeiCom3=_T("");
	ImeiCom4=_T("");
	memset(&threadParamM1,0,sizeof(struct threadParam));
	memset(&threadParamM2,0,sizeof(struct threadParam));
	memset(&threadParamM3,0,sizeof(struct threadParam));
	memset(&threadParamM4,0,sizeof(struct threadParam));
	iReadCount = 0;
	iEndFlag = 0;
}

extern void getSnX86(CString & valCstring); //此处修改，代码可以优化

void CCardCompareDlg::ImeiCompareFunc(int num,struct threadParam *param)
{
	if(num==1){
		if(param->FlagImei == 0)
		{
			getSnX86(ImeiCom1);
			if(ImeiBuff1.Compare(ImeiCom1)==0){
				param->FlagImei = 1;
				param->FlagImeiLuan = 0;
				SetDlgItemText(IDC_STATIC_RESULT, _T("OK"));
			}
			else{
				param->FlagImei = 0;
				SetDlgItemText(IDC_STATIC_RESULT, _T("NG"));
				if(ImeiBuff1.Compare(_T("NOREAD"))!=0){
					param->FlagImeiLuan = 1;
				}
			}
		}
	}
	else if(num==2){
		if(param->FlagImei == 0)
		{
			getSnX86(ImeiCom2);
			if(ImeiBuff2.Compare(ImeiCom2)==0){
				param->FlagImei = 1;
				param->FlagImeiLuan = 0;
				SetDlgItemText(IDC_STATIC_RESULT2, _T("OK"));
			}
			else{
				param->FlagImei = 0;
				SetDlgItemText(IDC_STATIC_RESULT2, _T("NG"));
				if(ImeiBuff2.Compare(_T("NOREAD"))!=0){
					param->FlagImeiLuan = 1;
				}
			}
		}
	}
	else if(num==3){
		//RE_LOG_ShowLine(num,ImeiCom3);
		//RE_LOG_ShowLine(num,_T("\n"));
		if(param->FlagImei == 0)
		{
			getSnX86(ImeiCom3);
			if(ImeiBuff3.Compare(ImeiCom3)==0){
				param->FlagImei = 1;
				param->FlagImeiLuan = 0;
				SetDlgItemText(IDC_STATIC_RESULT3, _T("OK"));
			}
			else{
				param->FlagImei = 0;
				SetDlgItemText(IDC_STATIC_RESULT3, _T("NG"));
				if(ImeiBuff3.Compare(_T("NOREAD"))!=0){
					param->FlagImeiLuan = 1;
				}
			}
		}
	}
	else if(num==4){
		if(param->FlagImei == 0)
		{
			getSnX86(ImeiCom4);
			if(ImeiBuff4.Compare(ImeiCom4)==0){
				param->FlagImei = 1;
				param->FlagImeiLuan = 0;
				SetDlgItemText(IDC_STATIC_RESULT4, _T("OK"));
			}
			else{
				param->FlagImei = 0;
				SetDlgItemText(IDC_STATIC_RESULT4, _T("NG"));
				if(ImeiBuff4.Compare(_T("NOREAD"))!=0){
					param->FlagImeiLuan = 1;
				}
			}
		}
	}
}

void CCardCompareDlg::InfoCompareFunc(struct threadParam *param,CListCtrl *m_ListCtrl,CString name, CString value)
{
	if(name.Compare(SHOCK) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,value);
#else
		 if(param->FlagShock == 0){
			if(value.Compare(str_shock)==0){
				m_ListCtrl->SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T("OK"));
				param->FlagShock = 1;
			}
			else{
				m_ListCtrl->SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T("NG"));
			}
		 }
#endif
	 }		
	else if(name.Compare(RSRP) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(RSRP_ROW,RSRP_COLUMN+1,value);
#else
		if(param->FlagRsrp == 0){
			int int_com = _ttoi(value);
			int int_edit = _ttoi(str_rsrp);
			if(int_com >= int_edit){
				m_ListCtrl->SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T("OK"));
				param->FlagRsrp = 1;
			}
			else{
				m_ListCtrl->SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
	else if(name.Compare(CELLID) == 0){
		m_ListCtrl->SetItemText(CELLID_ROW,CELLID_COLUMN+1,value);
	}
	else if(name.Compare(SNR) == 0){
		m_ListCtrl->SetItemText(SNR_ROW,SNR_COLUMN+1,value);
	}
	else if(name.Compare(REG) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(REG_ROW,REG_COLUMN+1,value);
#else
	int iCheck = _ttoi(str_reg_check);
	if(iCheck == 1){
		if(param->FlagReg == 0){
			if(value.Compare(str_reg)==0){
				m_ListCtrl->SetItemText(REG_ROW,REG_COLUMN+1,_T("OK"));
				param->FlagReg = 1;
			}
			else{
				m_ListCtrl->SetItemText(REG_ROW,REG_COLUMN+1,_T("NG"));
			}
		}
	}
	else{
		param->FlagReg = 1;
		m_ListCtrl->SetItemText(REG_ROW,REG_COLUMN+1,value);
	}
#endif
	}
	else if(name.Compare(LAT) == 0){
		m_ListCtrl->SetItemText(LAT_ROW,LAT_COLUMN+1,value);
	}
	else if(name.Compare(LON) == 0){
		m_ListCtrl->SetItemText(LON_ROW,LON_COLUMN+1,value);
	}
	else if(name.Compare(V) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(V_ROW,V_COLUMN+1,value);
#else
		if(param->FlagVol == 0){
			double int_com = _tstof(value);
			double int_lower = _tstof(str_vlower);
			double int_upper = _tstof(str_vupper);
			if(int_com >= int_lower && int_com <= int_upper){
				m_ListCtrl->SetItemText(V_ROW,V_COLUMN+1,_T("OK"));
				param->FlagVol = 1;
			}
			else{
				m_ListCtrl->SetItemText(V_ROW,V_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
	else if(name.Compare(T1) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(T1_ROW,T1_COLUMN+1,value);
#else
		if(param->FlagTem == 0){
			double int_com = _tstof(value);
			double int_lower = _tstof(str_t1lower);
			double int_upper = _tstof(str_t1upper);
			if(int_com >= int_lower && int_com <= int_upper){
				m_ListCtrl->SetItemText(T1_ROW,T1_COLUMN+1,_T("OK"));
				param->FlagTem = 1;
			}
			else{
				m_ListCtrl->SetItemText(T1_ROW,T1_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
	else if(name.Compare(T2) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(T2_ROW,T2_COLUMN+1,value);
#else
		if(param->FlagBan == 0){
			double int_com = _tstof(value);
			double int_lower = _tstof(str_t2lower);
			double int_upper = _tstof(str_t2upper);
			if(int_com >= int_lower && int_com <= int_upper){
				m_ListCtrl->SetItemText(T2_ROW,T2_COLUMN+1,_T("OK"));
				param->FlagBan = 1;
			}
			else{
				m_ListCtrl->SetItemText(T2_ROW,T2_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
	else if(name.Compare(PWR) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(PWR_ROW,PWR_COLUMN+1,value);
#else
		if(param->FlagPwr == 0){
			if(value.Compare(str_pwr)==0){
				m_ListCtrl->SetItemText(PWR_ROW,PWR_COLUMN+1,_T("OK"));
				param->FlagPwr = 1;
			}
			else{
				m_ListCtrl->SetItemText(PWR_ROW,PWR_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
	else if(name.Compare(SV) == 0){
#ifdef DISPLAY_VALUE
		m_ListCtrl->SetItemText(SV_ROW,SV_COLUMN+1,value);
#else
		if(param->FlagSv == 0){
			int int_com = _ttoi(value);
			int int_edit = _ttoi(str_sv);
			if(int_com >= int_edit){
				m_ListCtrl->SetItemText(SV_ROW,SV_COLUMN+1,_T("OK"));
				param->FlagSv = 1;
			}
			else{
				m_ListCtrl->SetItemText(SV_ROW,SV_COLUMN+1,_T("NG"));
			}
		}
#endif
	}
}
void CCardCompareDlg::GetImeiFromFile()
{
	CString theString(_T(""));
	CString str_temp(_T(""));
	int i = 0;
	int len = 0;
	char str[1024]={0};
	int idex = 1;
	FILE *fp=fopen("D:\\SN.txt","r");
	if(fp){
		len = fread(str, sizeof(char), 1024, fp);
		for(i=0;i<len;i++){
			if(str[i] == ','){
				if(idex == 1){
					ImeiBuff1 = ImeiBuff1+theString;
				}
				else if(idex == 2){
					ImeiBuff2 = ImeiBuff2+theString;
				}
				else if(idex == 3){
					ImeiBuff3 = ImeiBuff3+theString;
				}
				else if(idex == 4){
					ImeiBuff4 = ImeiBuff4+theString;
				}
				idex++;
				theString=_T("");
			}
			else if((str[i] != '\n') && (str[i] != '\r') && (str[i] != ' ')){
				str_temp.Format(_T("%C"), str[i]);
				theString = theString + str_temp;
			}
		}
		if(idex == 4){
			ImeiBuff4 = ImeiBuff4+theString;
		}
		fclose(fp);
	}
	else{
		RE_LOG_ShowLine(1,_T("open SN.txt fail\n"));
	}

	SetDlgItemText(IDC_EDIT_SCAN,  ImeiBuff1);
	SetDlgItemText(IDC_EDIT_SCAN2, ImeiBuff2);
	SetDlgItemText(IDC_EDIT_SCAN3, ImeiBuff3);
	SetDlgItemText(IDC_EDIT_SCAN4, ImeiBuff4);
	/*ListFile1.WriteString(_T("the IMEI read from txt is ["));
	ListFile2.WriteString(_T("the IMEI read from txt is ["));
	ListFile3.WriteString(_T("the IMEI read from txt is ["));
	ListFile4.WriteString(_T("the IMEI read from txt is ["));
	ListFile1.WriteString(ImeiBuff1);
	ListFile2.WriteString(ImeiBuff2);
	ListFile3.WriteString(ImeiBuff3);
	ListFile4.WriteString(ImeiBuff4);
	ListFile1.WriteString(_T("]\n"));
	ListFile2.WriteString(_T("]\n"));
	ListFile3.WriteString(_T("]\n"));
	ListFile4.WriteString(_T("]\n"));*/
}
void CCardCompareDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(TIMER1 == nIDEvent){
		iStartFlag = mc_read(1600, 10);
		//RE_LOG_ShowLine(_T("等待开始信号......"));
#ifdef START_TEST
		iStartFlag=1;
#endif

		if(iStartFlag == 1){
			ClearFlag();
			ClearDisplay();
			RE_LOG_ShowLine(1,_T("检测开始\n"));
			RE_LOG_ShowLine(2,_T("检测开始\n"));
			RE_LOG_ShowLine(3,_T("检测开始\n"));
			RE_LOG_ShowLine(4,_T("检测开始\n"));
			SerialPortM1.Serial_ClearData();
			SerialPortM2.Serial_ClearData();
			SerialPortM3.Serial_ClearData();
			SerialPortM4.Serial_ClearData();
			iStartFlag = 2;
			KillTimer(TIMER1);
			WriteResult(0,NULL,ST9_START,ST9_START,NULL,NULL);

#if 0
			BOOL bOpen = ListFile1.Open(LOG_PATH_1, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
			if (!bOpen){
				RE_LOG_ShowLine(1,_T("日志打开失败\n"));
			}	
			bOpen = ListFile2.Open(LOG_PATH_2, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
			if (!bOpen){
				RE_LOG_ShowLine(2,_T("日志打开失败\n"));
			}	
			bOpen = ListFile3.Open(LOG_PATH_3, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
			if (!bOpen){
				RE_LOG_ShowLine(3,_T("日志打开失败\n"));
			}		
			bOpen = ListFile4.Open(LOG_PATH_4, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
			if (!bOpen){
				RE_LOG_ShowLine(4,_T("日志打开失败\n"));
			}
			ListFile1.SeekToEnd();
			ListFile2.SeekToEnd();
			ListFile3.SeekToEnd();
			ListFile4.SeekToEnd();
			CString szCurrentDateTime;     
			time_t nowtime;     
			struct tm* ptm;     
			time(&nowtime);     
			ptm = localtime(&nowtime);     
			szCurrentDateTime.Format(_T("_________________________________________________%04d-%02d-%02d %02d:%02d:%02d_________________________________________________\n"),ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
			ListFile1.WriteString(szCurrentDateTime);
			ListFile2.WriteString(szCurrentDateTime);
			ListFile3.WriteString(szCurrentDateTime);
			ListFile4.WriteString(szCurrentDateTime);
#endif	
			GetImeiFromFile();
			SetTimer(TIMER2,1000,NULL);						
		}
	}
	else if(TIMER2 == nIDEvent){
		iReadCount++;
		if(iReadCount>=_ttoi(str_check_time)){
			iEndFlag = 1;	
			if((threadParamM1.iFlagEnd == 1) && (threadParamM2.iFlagEnd == 1) && (threadParamM3.iFlagEnd == 1) && (threadParamM4.iFlagEnd == 1)){
				WriteResult(5,NULL,ST9_NG_IMEI,ST9_NG_IMEI,NULL,NULL);
				MySleep(50);
				WriteResult(1,&threadParamM1,ST9_OK_DEV1,ST9_NG_DEV1,NULL,&devParamM1);
				MySleep(50);
				WriteResult(2,&threadParamM2,ST9_OK_DEV2,ST9_NG_DEV2,NULL,&devParamM2);
				MySleep(50);
				WriteResult(3,&threadParamM3,ST9_OK_DEV3,ST9_NG_DEV3,NULL,&devParamM3);
				MySleep(50);
				WriteResult(4,&threadParamM4,ST9_OK_DEV4,ST9_NG_DEV4,NULL,&devParamM4);
				MySleep(80);
				WriteResult(6,NULL,ST9_END,ST9_END,NULL,NULL);
				/*if(ListFile1){
					ListFile1.Close();
				}
				if(ListFile2){
					ListFile2.Close();
				}
				if(ListFile3){
					ListFile3.Close();
				}
				if(ListFile4){
					ListFile4.Close();
				}*/
				RE_LOG_ShowLine(1,_T("检测结束\n"));
				RE_LOG_ShowLine(2,_T("检测结束\n"));
				RE_LOG_ShowLine(3,_T("检测结束\n"));
				RE_LOG_ShowLine(4,_T("检测结束\n"));
				KillTimer(TIMER2);
				iStartFlag = 0;
#ifdef START_TEST
				SetTimer(TIMER1,READ_TIME*2,NULL);
#else
				SetTimer(TIMER1,READ_TIME,NULL);
#endif
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

int CCardCompareDlg::RE_LOG_ShowLine(int num,const CString & s)
{
	CRichEditCtrl *m_Log;
	if(num==1){
		m_Log = &m_RE_Log;
	}
	else if(num==2){
		m_Log = &m_RE_Log2;
	}
	else if(num==3){
		m_Log = &m_RE_Log3;
	}
	else if(num==4){
		m_Log = &m_RE_Log4;
	}
	else{
		m_Log = &m_RE_Log;
	}
	m_Log->SetSel(-1, -1);
	//CString str = s + _T("\n");
	CString str = s;
	m_Log->ReplaceSel(str);
	m_Log->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 0;
}

void CCardCompareDlg::OnBnClickedButton1(UINT nID)
{
	// TODO: 在此添加控件通知处理程序代码
	int iComboID1 = 0;
	int iComboID2 = 0;
	CString str_dev(_T(""));
	if(nID == IDC_BUTTON1){
		iComboID1 = IDC_COMBO1;
		iComboID2 = IDC_COMBO2;
		str_dev=_T("dev1");
		RE_LOG_ShowLine(1,_T("设备1打开串口\n"));
	}
	else if(nID == IDC_BUTTON2){
		iComboID1 = IDC_COMBO3;
		iComboID2 = IDC_COMBO4;
		str_dev=_T("dev2");
		RE_LOG_ShowLine(2,_T("设备2打开串口\n"));
	}
	else if(nID == IDC_BUTTON3){
		iComboID1 = IDC_COMBO5;
		iComboID2 = IDC_COMBO6;
		str_dev=_T("dev3");
		RE_LOG_ShowLine(3,_T("设备3打开串口\n"));
	}
	else if(nID == IDC_BUTTON4){
		iComboID1 = IDC_COMBO7;
		iComboID2 = IDC_COMBO8;
		str_dev=_T("dev4");
		RE_LOG_ShowLine(4,_T("设备4打开串口\n"));
	}
	else{
		return;
	}
	CComboBox* combo= ( CComboBox*)GetDlgItem(iComboID1);
	int nIndex = combo->GetCurSel();
	CString strItem;
	combo->GetLBText( nIndex, strItem);
	CString str_com(_T(""));
	str_com.Format(_T("\\\\.\\%s"),strItem);
	CComboBox* combo2= ( CComboBox*)GetDlgItem(iComboID2);
	int nIndex2 = combo2->GetCurSel();
	CString strItem2;
	combo2->GetLBText( nIndex2, strItem2);
	int rate = _ttoi(strItem2);

	CString str_edit(_T(""));
	str_edit.Format(_T("%d"),nIndex);
	WritePrivateProfileString(str_dev,_T("com"),str_edit,INI_PATH);
	str_edit.Format(_T("%d"),nIndex2);
	WritePrivateProfileString(str_dev,_T("rate"),str_edit,INI_PATH);

	int ret=0;
	CString str_ret(_T(""));
	if(nID == IDC_BUTTON1){
		ret = SerialPortM1.Serial_Init(str_com,rate);	
		str_ret.Format(_T("Serial_Init ret = %x\n"),ret);
		RE_LOG_ShowLine(1,str_ret);
		if(ret==0){
			GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		}
	}
	else if(nID == IDC_BUTTON2){
		ret = SerialPortM2.Serial_Init(str_com,rate);	
		str_ret.Format(_T("Serial_Init ret = %x\n"),ret);
		RE_LOG_ShowLine(2,str_ret);
		if(ret==0){
			GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
		}
	}
	else if(nID == IDC_BUTTON3){
		ret = SerialPortM3.Serial_Init(str_com,rate);	
		str_ret.Format(_T("Serial_Init ret = %x\n"),ret);
		RE_LOG_ShowLine(3,str_ret);
		if(ret==0){
			GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
		}
	}
	else if(nID == IDC_BUTTON4){
		ret = SerialPortM4.Serial_Init(str_com,rate);	
		str_ret.Format(_T("Serial_Init ret = %x\n"),ret);
		RE_LOG_ShowLine(4,str_ret);
		if(ret==0){
			GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
		}
	}
	if(ret==0){
		GetDlgItem(nID)->EnableWindow(FALSE);
	}
}

void CCardCompareDlg::OnBnClickedButton2(UINT nID)
{
	// TODO: 在此添加控件通知处理程序代码
	if(nID == IDC_BUTTON5){
		SerialPortM1.Serial_Release();
		RE_LOG_ShowLine(1,_T("设备1关闭串口\n"));
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	}
	else if(nID == IDC_BUTTON6){
		SerialPortM2.Serial_Release();
		RE_LOG_ShowLine(2,_T("设备2关闭串口\n"));
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
	else if(nID == IDC_BUTTON7){
		SerialPortM3.Serial_Release();
		RE_LOG_ShowLine(3,_T("设备3关闭串口\n"));
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	}
	else if(nID == IDC_BUTTON8){
		SerialPortM4.Serial_Release();
		RE_LOG_ShowLine(4,_T("设备4关闭串口\n"));
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	else{
		return;
	}
}

UINT ThreadFunc1(LPVOID lParam)
{
	CCardCompareDlg *pWnd = (CCardCompareDlg *)lParam;
#ifndef START_TEST
	if (mc_open("192.168.1.91", 3050) != 0){
		pWnd->RE_LOG_ShowLine(1,_T("mc_open fail\n"));
	}else{
		pWnd->RE_LOG_ShowLine(1,_T("mc_open success\n"));
	}
#endif
	while(1){
		pWnd->ReceiveSerialProc(1,&pWnd->SerialPortM1,&pWnd->m_ListCtrl1,pWnd->ImeiCom1,&pWnd->threadParamM1);
		pWnd->MySleep(50);
	}
	return 0;
}
UINT ThreadFunc2(LPVOID lParam)
{
	CCardCompareDlg *pWnd = (CCardCompareDlg *)lParam;
	while(1){
		pWnd->ReceiveSerialProc(2,&pWnd->SerialPortM2,&pWnd->m_ListCtrl2,pWnd->ImeiCom2,&pWnd->threadParamM2);
		pWnd->MySleep(50);
	}
	return 0;
}
UINT ThreadFunc3(LPVOID lParam)
{
	CCardCompareDlg *pWnd = (CCardCompareDlg *)lParam;
	while(1){
		pWnd->ReceiveSerialProc(3,&pWnd->SerialPortM3,&pWnd->m_ListCtrl3,pWnd->ImeiCom3,&pWnd->threadParamM3);
		pWnd->MySleep(50);
	}
	return 0;
}
UINT ThreadFunc4(LPVOID lParam)
{
	CCardCompareDlg *pWnd = (CCardCompareDlg *)lParam;
	while(1){
		pWnd->ReceiveSerialProc(4,&pWnd->SerialPortM4,&pWnd->m_ListCtrl4,pWnd->ImeiCom4,&pWnd->threadParamM4);
		pWnd->MySleep(50);
	}
	return 0;
}
void CCardCompareDlg::ReceiveSerialProc(int num,SerialPort* pSerialPortM,CListCtrl *m_ListCtrl,CString &m_Imei,struct threadParam *param)
{
	if(iStartFlag != 2){
		return;
	}
	if(param->iFlagEnd == 1){
		return;
	}
	if(iEndFlag == 1){
		param->iFlagEnd = 1;
		return;
	}
	if(param->iWrite>=(BUFF_SIZE-1)){
		RE_LOG_ShowLine(num,_T("buff full!"));
		return;
	}
	DWORD nBytesRead = RECEIVE_SIZE;
	CString str_log(_T(""));
	CString str_vcom(_T(""));
	CString str_temp(_T(""));
	DWORD i=0;
	int ret = 0;
	int start = 0;
	int end = 0;
	int index1 = 0;
	int index2 = 0;
	int index3 = 0;
	CString temp1(_T(""));
	CString temp2(_T(""));
	CString temp3(_T(""));
	
	ret = pSerialPortM->Serial_ReadData(param->inbuff+param->iWrite, nBytesRead );
	if((ret==0) && (nBytesRead>0)){
		param->iWrite = param->iWrite + nBytesRead;
		while(1){
			if(param->iRead>=param->iWrite){
				param->iRead = 0;
				param->iWrite = 0;
				return;
			}
			str_vcom=_T("");
			for(i=param->iRead;i<param->iWrite;i++){
				if(param->inbuff[i]==0x0D && param->inbuff[i+1]==0x0A){
					param->iRead = i+2;
					break;
				}
				else{
					str_temp.Format(_T("%C"), param->inbuff[i]);
					str_vcom = str_vcom + str_temp;
				}
			}//for
			if(i>=param->iWrite){
				return;
			}
			RE_LOG_ShowLine(num,str_vcom);
			RE_LOG_ShowLine(num,_T("\n"));
			/*if(num == 1){
				ListFile1.WriteString(str_vcom);
				ListFile1.WriteString(_T("\n"));
			}
			else if(num == 2){
				ListFile2.WriteString(str_vcom);
				ListFile2.WriteString(_T("\n"));
			}
			else if(num == 3){
				ListFile3.WriteString(str_vcom);
				ListFile3.WriteString(_T("\n"));
			}
			else if(num == 4){
				ListFile4.WriteString(str_vcom);
				ListFile4.WriteString(_T("\n"));
			}*/
			
			//return;

			if(str_vcom.Find(_T("IMEI:"),0) != -1){
				m_Imei=_T("");
				m_Imei = m_Imei + str_vcom;
				param->iDiscard = 1;
			}//if
			else if(str_vcom.Find(_T("IMSI:"),0) != -1){
				m_Imei = m_Imei + str_vcom;
				ImeiCompareFunc(num,param);
				//param->iDiscard = 1;
			}//else
			else if(str_vcom.Find(_T("$info,"),0) != -1){
				if(param->iDiscard==0){
					RE_LOG_ShowLine(num,_T("discard info\n"));
					return;
				}
				index1=0;
				index2 = 0;
				index3 = 0;
				//str_vcom.Format(_T("%s"),_T("$info,shock:0,rsrp::-00102,cellid:140899138,snr:05,reg:1,lat:34.82447,lon:113.55722,v:0.25,t:2.49,029,pwr:1,sv:08"));
				while(1){
					 index1 = str_vcom.Find(_T(","),index1);
					 if(index1==-1){
						break;
					 }
					 index2 = str_vcom.Find(_T(":"),index1);
					 if(index2==-1){
						break;
					 }
					 temp1 = str_vcom.Mid( index1+1, index2-index1-1);
					 temp3 = str_vcom.Mid( index2+1, 1);
					 if(temp3.Compare(_T(":")) == 0){
						 index2 = index2+1;
					 }
					 
					 index3 = str_vcom.Find(_T(","),index2); 
					 if(index3 != -1){ 
						temp2 = str_vcom.Mid( index2+1, index3-index2-1);
					 }
					 else{
						 temp2 = str_vcom.Mid( index2+1, str_vcom.GetLength()-index2-1);
					 }
					 if(temp1.Compare(T) == 0){
						 //,t:2.49,029,pwr:1
						 InfoCompareFunc(param,m_ListCtrl,T1,temp2);
						 index2 = index3+1;
						 if(index2==-1){
							break;
						 }
						 index3 = str_vcom.Find(_T(","),index2);
						 if(index3==-1){
							break;
						 }
						 temp2 = str_vcom.Mid( index2, index3-index2);
						 InfoCompareFunc(param,m_ListCtrl,T2,temp2);
					 }
					 else{
						 InfoCompareFunc(param,m_ListCtrl,temp1,temp2);
					 }
					 if(index3==-1){
						break;
					 }
					 index1 = index3;
				}//while
			}//else
		}//while(1){
	}//if((ret==0) && (nBytesRead>0)){
	return;
}

void CCardCompareDlg::InitDev(CComboBox* combo1,CComboBox* combo2,CListCtrl *m_ListCtrl)
{
	int  lReg; 
	HKEY  hKey; 
	int i = 0; 
	//动态获取系统连接的串口
	lReg=RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),NULL, KEY_READ, &hKey); 
	if(lReg == ERROR_SUCCESS) 
	{ 
		TCHAR portName[0x100],commName[0x100];
		DWORD dwLong, dwSize;
		do{
			dwSize = sizeof(portName)/sizeof(TCHAR);
			dwLong = dwSize;
			lReg = RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(LPBYTE)commName,&dwSize);
			if(lReg == ERROR_NO_MORE_ITEMS){
				break;
			}
			combo1->AddString(commName);
			i++;
		}while(1);
		RegCloseKey(hKey);
	} 
	combo1->SetCurSel(0);

	combo2->AddString(_T("9600"));
	combo2->AddString(_T("19200"));
	combo2->AddString(_T("38400"));
	combo2->AddString(_T("57600"));
	combo2->AddString(_T("115200"));
	combo2->AddString(_T("230400"));
	combo2->SetCurSel(0);

	m_ListCtrl->InsertColumn(0,_T("项目"));
	m_ListCtrl->InsertColumn(1,_T("结果"));
	m_ListCtrl->InsertColumn(2,_T("项目"));
	m_ListCtrl->InsertColumn(3,_T("结果"));
	
	//$info,shock:0,rsrp::00000,cellid:000000000,snr:00,reg:0,lat:00.00000,lon:000.00000,v:0.00,t:2.49,029,pwr:1,sv:00

	m_ListCtrl->InsertItem(SHOCK_ROW,SHOCK);
	m_ListCtrl->SetItemText(SV_ROW,SV_COLUMN,SV);
	m_ListCtrl->InsertItem(REG_ROW,REG);
	m_ListCtrl->SetItemText(PWR_ROW,PWR_COLUMN,PWR);
	m_ListCtrl->InsertItem(RSRP_ROW,RSRP);
	m_ListCtrl->SetItemText(V_ROW,V_COLUMN,V);
	m_ListCtrl->InsertItem(T1_ROW,T1);
	m_ListCtrl->SetItemText(T2_ROW,T2_COLUMN,T2);
	m_ListCtrl->InsertItem(CELLID_ROW,CELLID);
	m_ListCtrl->SetItemText(SNR_ROW,SNR_COLUMN,SNR);
	m_ListCtrl->InsertItem(LAT_ROW,LAT);
	m_ListCtrl->SetItemText(LON_ROW,LON_COLUMN,LON);
	
	
	

	//重新分配列宽
	m_ListCtrl->SetColumnWidth(0,50);
	m_ListCtrl->SetColumnWidth(1,50);
	m_ListCtrl->SetColumnWidth(2,50);
	m_ListCtrl->SetColumnWidth(3,50);
	//扩展风格
	m_ListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);//|LVS_SHOWSELALWAYS);

}

void CCardCompareDlg::WriteResult(int num,struct threadParam *param,unsigned int addrok,unsigned int addrng,CStdioFile* pListFile,struct StatisticsParam *countparam)
{
	unsigned int addr = addrok;
	unsigned char value = 1;
	int ret = 0;
	CString str_log;
	
	if(num == 0){
		value = 0;
	}
	else if((num>=1) && (num<=4)){
		countparam->iCountDev++;
		if(param->FlagImei && param->FlagShock && param->FlagRsrp && param->FlagReg && param->FlagVol && param->FlagTem && param->FlagBan && param->FlagPwr && param->FlagSv){
			//pListFile->WriteString(_T("OK\n"));
			countparam->iOKCountDev++;
		}
		else{
			addr = addrng;
			//pListFile->WriteString(_T("NG\n"));
			if(param->FlagImei==0){
				countparam->iNGCountImei++;
			}
			if(param->FlagShock==0){
				countparam->iNGCountShock++;
			}
			if(param->FlagRsrp==0){
				countparam->iNGCountRsrp++;
			}
			if(param->FlagReg==0){
				countparam->iNGCountReg++;
			}
			if(param->FlagVol==0){
				countparam->iNGCountVol++;
			}
			if(param->FlagTem==0){
				countparam->iNGCountTem++;
			}
			if(param->FlagBan==0){
				countparam->iNGCountBan++;
			}
			if(param->FlagPwr==0){
				countparam->iNGCountPwr++;
			}
			if(param->FlagSv==0){
				countparam->iNGCountSv++;
			}
			
		}
	}
	else if(num == 5){
		if((threadParamM1.FlagImeiLuan==0) && (threadParamM2.FlagImeiLuan==0) && (threadParamM3.FlagImeiLuan==0) && (threadParamM4.FlagImeiLuan==0)){
			return;
		}
	}

	ret = mc_write(addr, value, 10);
	str_log.Format(_T("mc_write addr=%d value=%d ret=%d\n"),addr,value,ret);
	if(num>=1 && num<=4){
		RE_LOG_ShowLine(num,str_log);
	}
	else if(num == 5){
		RE_LOG_ShowLine(1,str_log);
	}
	else{
		if(ret!=0)
		RE_LOG_ShowLine(1,str_log);
	}
}


void CCardCompareDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CPasswordDlg dlg(this);
	dlg.DoModal();
}

void CCardCompareDlg::UpdateCompareValue()
{
	GetPrivateProfileString(_T("systemset"),_T("shock"),_T("1"),str_shock.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("reg"),_T("1"),str_reg.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("rsrp"),_T("-200"),str_rsrp.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("pwr"),_T("1"),str_pwr.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("sv"),_T("20"),str_sv.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("vlower"),_T("3.5"),str_vlower.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("vupper"),_T("4.1"),str_vupper.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("t1lower"),_T("1.0"),str_t1lower.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("t1upper"),_T("2.0"),str_t1upper.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("t2lower"),_T("20"),str_t2lower.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("systemset"),_T("t2upper"),_T("40"),str_t2upper.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);

	GetPrivateProfileString(_T("switchset"),_T("reg"),_T("1"),str_reg_check.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	GetPrivateProfileString(_T("otherset"),_T("checktime"),_T("15"),str_check_time.GetBuffer(MAX_PATH),MAX_PATH,INI_PATH);
	//FlagReg1=0;FlagReg2=0;FlagReg3=0;FlagReg4=0;
	
}



void CCardCompareDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	m_RE_Log.SetWindowText(_T(""));
	m_RE_Log2.SetWindowText(_T(""));
	m_RE_Log3.SetWindowText(_T(""));
	m_RE_Log4.SetWindowText(_T(""));
}

void CCardCompareDlg::OnNMCustomdrawList1(UINT nID,NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
    NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
    switch(lplvdr->nmcd.dwDrawStage)//判断状态
    {
        case CDDS_PREPAINT:
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            break;
        }
        case CDDS_ITEMPREPAINT://如果为画ITEM之前就要进行颜色的改变
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		}
		case CDDS_SUBITEM|CDDS_ITEMPREPAINT:
		{
            CString strText=_T("");
			if(nID == IDC_LIST1){
				strText = m_ListCtrl1.GetItemText(nmcd.dwItemSpec,lplvdr->iSubItem);
			}
			else if(nID == IDC_LIST2){
				strText = m_ListCtrl2.GetItemText(nmcd.dwItemSpec,lplvdr->iSubItem);
			}
			else if(nID == IDC_LIST3){
				strText = m_ListCtrl3.GetItemText(nmcd.dwItemSpec,lplvdr->iSubItem);
			}
			else if(nID == IDC_LIST4){
				strText = m_ListCtrl4.GetItemText(nmcd.dwItemSpec,lplvdr->iSubItem);
			}
            if(strText.Find(_T("NG")) != -1){
                lplvdr->clrText = RGB(0xff,0,0);  
            }
			else{
				lplvdr->clrText = RGB(0,0,0);
			}
			 *pResult = CDRF_DODEFAULT;
        }
        break;
    }
}

HBRUSH CCardCompareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    if(( IDC_STATIC_RESULT  ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	|| ( IDC_STATIC_RESULT2  ==  pWnd->GetDlgCtrlID())
	|| ( IDC_STATIC_RESULT3  ==  pWnd->GetDlgCtrlID())
	|| ( IDC_STATIC_RESULT4  ==  pWnd->GetDlgCtrlID()))
    {
		CString str_com(_T(""));
		GetDlgItemText(pWnd->GetDlgCtrlID(), str_com);
		if(str_com.Compare(_T("NG"))==0){
			pDC->SetTextColor(RGB(255,0,0));
		}
		else{
			pDC->SetTextColor(RGB(0,0,0));
		}
    }
    return hbr;//返回用于重绘控件背景的画刷
}

void CCardCompareDlg::MySleep(int interval)
{
	HANDLE evt = CreateEvent(NULL, TRUE, FALSE, NULL);
	WaitForSingleObject(evt, interval);
	CloseHandle(evt);
}

void CCardCompareDlg::ClearDisplay()
{
	SetDlgItemText(IDC_EDIT_SCAN,  _T(""));
	SetDlgItemText(IDC_EDIT_SCAN2, _T(""));
	SetDlgItemText(IDC_EDIT_SCAN3, _T(""));
	SetDlgItemText(IDC_EDIT_SCAN4, _T(""));

	SetDlgItemText(IDC_STATIC_RESULT,  _T("NG"));
	SetDlgItemText(IDC_STATIC_RESULT2,  _T("NG"));
	SetDlgItemText(IDC_STATIC_RESULT3,  _T("NG"));
	SetDlgItemText(IDC_STATIC_RESULT4,  _T("NG"));

	m_ListCtrl1.SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(PWR_ROW,PWR_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(REG_ROW,REG_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(SV_ROW,SV_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(V_ROW,V_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(CELLID_ROW,CELLID_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(SNR_ROW,SNR_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(LAT_ROW,LAT_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(LON_ROW,LON_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(T1_ROW,T1_COLUMN+1,_T(""));
	m_ListCtrl1.SetItemText(T2_ROW,T2_COLUMN+1,_T(""));

	m_ListCtrl2.SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(PWR_ROW,PWR_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(REG_ROW,REG_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(SV_ROW,SV_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(V_ROW,V_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(CELLID_ROW,CELLID_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(SNR_ROW,SNR_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(LAT_ROW,LAT_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(LON_ROW,LON_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(T1_ROW,T1_COLUMN+1,_T(""));
	m_ListCtrl2.SetItemText(T2_ROW,T2_COLUMN+1,_T(""));

	m_ListCtrl3.SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(PWR_ROW,PWR_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(REG_ROW,REG_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(SV_ROW,SV_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(V_ROW,V_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(CELLID_ROW,CELLID_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(SNR_ROW,SNR_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(LAT_ROW,LAT_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(LON_ROW,LON_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(T1_ROW,T1_COLUMN+1,_T(""));
	m_ListCtrl3.SetItemText(T2_ROW,T2_COLUMN+1,_T(""));

	m_ListCtrl4.SetItemText(SHOCK_ROW,SHOCK_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(PWR_ROW,PWR_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(REG_ROW,REG_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(SV_ROW,SV_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(V_ROW,V_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(RSRP_ROW,RSRP_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(CELLID_ROW,CELLID_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(SNR_ROW,SNR_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(LAT_ROW,LAT_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(LON_ROW,LON_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(T1_ROW,T1_COLUMN+1,_T(""));
	m_ListCtrl4.SetItemText(T2_ROW,T2_COLUMN+1,_T(""));

	OnBnClickedButton11();
}

void CCardCompareDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	//ListFile5.Open(LOG_PATH_5, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
	//ListFile5.SeekToEnd();
	CString szCurrentDateTime;     
	time_t nowtime;     
	struct tm* ptm;     
	time(&nowtime);     
	ptm = localtime(&nowtime);     
	//szCurrentDateTime.Format(_T("_____________________________________start time %04d-%02d-%02d %02d:%02d:%02d_________________________________________________\n"),ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
	szCurrentDateTime.Format(_T("start time %04d-%02d-%02d %02d:%02d:%02d\n"),ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
	//ListFile5.WriteString(szCurrentDateTime);
	//ListFile5.Close();
	memset(&devParamM1,0,sizeof(struct StatisticsParam));
	memset(&devParamM2,0,sizeof(struct StatisticsParam));
	memset(&devParamM3,0,sizeof(struct StatisticsParam));
	memset(&devParamM4,0,sizeof(struct StatisticsParam));
}

void CCardCompareDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	//ListFile5.Open(LOG_PATH_5, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
	//ListFile5.SeekToEnd();
	CString szCurrentDateTime;

	int total = devParamM1.iCountDev+devParamM2.iCountDev+devParamM3.iCountDev+devParamM4.iCountDev;
	if(total>0){
		//ListFile5.WriteString(_T("______________________________________________________________________________\n"));
		//szCurrentDateTime.Format(_T("|%10s|%10s|%10s|%10s|%10s|%10s|%10s|\n"),_T(" "),_T("position1"),_T("position2"),_T("position3"),_T("position4"),_T("total"),_T("percent"));
		//ListFile5.WriteString(szCurrentDateTime);
		//ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		/*szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%10s|\n"),_T("dev total"),devParamM1.iCountDev,devParamM2.iCountDev,devParamM3.iCountDev,devParamM4.iCountDev,total,_T(" "));
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total1 = devParamM1.iOKCountDev+devParamM2.iOKCountDev+devParamM3.iOKCountDev+devParamM4.iOKCountDev;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("dev OK"),devParamM1.iOKCountDev,devParamM2.iOKCountDev,devParamM3.iOKCountDev,devParamM4.iOKCountDev,total1,total1*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total2 = devParamM1.iCountDev-devParamM1.iOKCountDev+devParamM2.iCountDev-devParamM2.iOKCountDev+devParamM3.iCountDev-devParamM3.iOKCountDev+devParamM4.iCountDev-devParamM4.iOKCountDev;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("dev NG"),devParamM1.iCountDev-devParamM1.iOKCountDev,devParamM2.iCountDev-devParamM2.iOKCountDev,devParamM3.iCountDev-devParamM3.iOKCountDev,devParamM4.iCountDev-devParamM4.iOKCountDev,total2,total2*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total3 = devParamM1.iNGCountImei+devParamM2.iNGCountImei+devParamM3.iNGCountImei+devParamM4.iNGCountImei;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Imei NG"),devParamM1.iNGCountImei,devParamM2.iNGCountImei,devParamM3.iNGCountImei,devParamM4.iNGCountImei,total3,total3*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total4 = devParamM1.iNGCountShock+devParamM2.iNGCountShock+devParamM3.iNGCountShock+devParamM4.iNGCountShock;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Shock NG"),devParamM1.iNGCountShock,devParamM2.iNGCountShock,devParamM3.iNGCountShock,devParamM4.iNGCountShock,total4,total4*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total5 = devParamM1.iNGCountRsrp+devParamM2.iNGCountRsrp+devParamM3.iNGCountRsrp+devParamM4.iNGCountRsrp;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Rsrp NG"),devParamM1.iNGCountRsrp,devParamM2.iNGCountRsrp,devParamM3.iNGCountRsrp,devParamM4.iNGCountRsrp,total5,total5*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total6 = devParamM1.iNGCountReg+devParamM2.iNGCountReg+devParamM3.iNGCountReg+devParamM4.iNGCountReg;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Reg NG"),devParamM1.iNGCountReg,devParamM2.iNGCountReg,devParamM3.iNGCountReg,devParamM4.iNGCountReg,total6,total6*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total7 = devParamM1.iNGCountVol+devParamM2.iNGCountVol+devParamM3.iNGCountVol+devParamM4.iNGCountVol;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Vol NG"),devParamM1.iNGCountVol,devParamM2.iNGCountVol,devParamM3.iNGCountVol,devParamM4.iNGCountVol,total7,total7*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total8 = devParamM1.iNGCountTem+devParamM2.iNGCountTem+devParamM3.iNGCountTem+devParamM4.iNGCountTem;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("T1 NG"),devParamM1.iNGCountTem,devParamM2.iNGCountTem,devParamM3.iNGCountTem,devParamM4.iNGCountTem,total8,total8*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total9 = devParamM1.iNGCountBan+devParamM2.iNGCountBan+devParamM3.iNGCountBan+devParamM4.iNGCountBan;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("T2 NG"),devParamM1.iNGCountBan,devParamM2.iNGCountBan,devParamM3.iNGCountBan,devParamM4.iNGCountBan,total9,total9*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total10 = devParamM1.iNGCountPwr+devParamM2.iNGCountPwr+devParamM3.iNGCountPwr+devParamM4.iNGCountPwr;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Pwr NG"),devParamM1.iNGCountPwr,devParamM2.iNGCountPwr,devParamM3.iNGCountPwr,devParamM4.iNGCountPwr,total10,total10*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));

		int total11 = devParamM1.iNGCountSv+devParamM2.iNGCountSv+devParamM3.iNGCountSv+devParamM4.iNGCountSv;
		szCurrentDateTime.Format(_T("|%10s|%10d|%10d|%10d|%10d|%10d|%9d%%|\n"),_T("Sv NG"),devParamM1.iNGCountSv,devParamM2.iNGCountSv,devParamM3.iNGCountSv,devParamM4.iNGCountSv,total11,total11*100/total);
		ListFile5.WriteString(szCurrentDateTime);
		ListFile5.WriteString(_T("|__________|__________|__________|__________|__________|__________|__________|\n"));*/
	}
	
	time_t nowtime;     
	struct tm* ptm;     
	time(&nowtime);     
	ptm = localtime(&nowtime);     
	//szCurrentDateTime.Format(_T("_______________________________________end time %04d-%02d-%02d %02d:%02d:%02d_________________________________________________\n"),ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
	//szCurrentDateTime.Format(_T("end time %04d-%02d-%02d %02d:%02d:%02d\n"),ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
	//ListFile5.WriteString(szCurrentDateTime);
	//ListFile5.Close();
}
