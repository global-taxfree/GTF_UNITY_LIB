// GTFScanOCX_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GTFScanOCX_Demo.h"
#include "GTFScanOCX_DemoDlg.h"
#include <atlconv.h>
#include <Windows.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCANDATA_SIZE				100						// ��ĵ ������ Size
#define SCAN_TIMEOUT				10						// ��ĵ Ÿ�Ӿƿ��ð� (��)

/////////////////////////////////////////////////////////////////////////////
// CGTFScanOCX_DemoDlg dialog

CGTFScanOCX_DemoDlg::CGTFScanOCX_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGTFScanOCX_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGTFScanOCX_DemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGTFScanOCX_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGTFScanOCX_DemoDlg)
	DDX_Control(pDX, IDC_COMBO2, m_cmbPortNo);
	DDX_Control(pDX, IDC_COMBO1, m_cmbScan);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGTFScanOCX_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CGTFScanOCX_DemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnDataScan)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeScanner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGTFScanOCX_DemoDlg message handlers

BOOL CGTFScanOCX_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	if(Init_LoadOCX() == FALSE)
		return FALSE;

	Init_Control();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGTFScanOCX_DemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGTFScanOCX_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGTFScanOCX_DemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
}


BOOL CGTFScanOCX_DemoDlg::Init_LoadOCX()
{
/*
	HRESULT hr = S_OK; 
	HINSTANCE hInst = ::LoadLibrary("GTF_PASSPORT_COMM_ActiveX.ocx"); 

	if (hInst == NULL) 
	{
		AfxMessageBox("FAILED OCX");
		return GetLastError(); 
	}

	FARPROC fnProc = ::GetProcAddress(hInst, "DllRegisterServer"); 

	if (fnProc != NULL)
	{ 
		hr = (*fnProc)(); 
	} 
	else 
	{
		hr = GetLastError(); 
	}
	
	::FreeLibrary(hInst);
*/


	static CLSID const clsid = { 0xbf7e50a7, 0x7f9b, 0x4d1e, 0xbe, 0x6a, 0xff, 0xef, 0x30, 0x9a, 0x52, 0xd1};

	m_pMgr = new CWnd();

	if (!m_pMgr->CreateControl(clsid, NULL, WS_CHILD, CRect(0,0,0,0), this, (UINT) m_pMgr))
	{
		LONG DD = GetLastError();
		delete m_pMgr;
		m_pMgr = NULL;
		TRACE("���� �����Դϴ�.");
		return FALSE;
	}

	return TRUE;
}


BOOL CGTFScanOCX_DemoDlg::Init_Control()
{
	// �޺��ڽ� �ʱ�ȭ

	m_cmbScan.AddString(_T("GTF ��ĳ��"));
	m_cmbScan.AddString(_T("wisescan 420 ��ĳ��"));
	m_cmbScan.AddString(_T("DAWIN ��ĳ��"));

	m_cmbPortNo.AddString(_T("COM1"));
	m_cmbPortNo.AddString(_T("COM2"));
	m_cmbPortNo.AddString(_T("COM3"));
	m_cmbPortNo.AddString(_T("COM4"));
	m_cmbPortNo.AddString(_T("COM5"));
	m_cmbPortNo.AddString(_T("COM6"));
	m_cmbPortNo.AddString(_T("COM7"));
	m_cmbPortNo.AddString(_T("COM8"));
	m_cmbPortNo.AddString(_T("COM9"));
	m_cmbPortNo.AddString(_T("COM10"));
	m_cmbPortNo.AddString(_T("COM11"));
	m_cmbPortNo.AddString(_T("COM12"));
	m_cmbPortNo.AddString(_T("COM13"));
	m_cmbPortNo.AddString(_T("COM14"));
	m_cmbPortNo.AddString(_T("COM15"));
	m_cmbPortNo.AddString(_T("COM16"));
	m_cmbPortNo.AddString(_T("COM17"));
	m_cmbPortNo.AddString(_T("COM18"));
	m_cmbPortNo.AddString(_T("COM19"));
	m_cmbPortNo.AddString(_T("COM20"));

	// ȯ�� �ʱⰪ ��� (��ĳ�� ����, Port �ʱ� ��ġ)
	Ini_Set(0);

	return TRUE;
}

void CGTFScanOCX_DemoDlg::OnDataScan() 
{
	// ��ĳ���� ��ĳ�� ���� ���� (Ini ����)
	Ini_Set(1);

	// Scan ó���� �ϰ� �����͸� ����Ѵ�
	short nRet = 0;
	CString strData;

	// Scanner Open
	int nPortNo = m_cmbPortNo.GetCurSel() + 1;
	
	nRet = Ocx_OpenPortByNum(nPortNo);
	if(nRet)
	{
		// Scan �϶�� command�� �����Ѵ�
		nRet = Ocx_Scan();
		if(nRet)
		{
			// Scan �� ������� ����Ѵ�. (��ĵ ������)
			nRet = Ocx_ReceiveData(SCAN_TIMEOUT);			// Time-Out �ð� ����(��)
			if(nRet > 0)									// ����ó��
			{
				strData = Ocx_GetPassportInfo();			// ��ĵ ������ ���ϱ�

				((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
				((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(0, strData);

				nRet = 0;
			}
			else if(nRet == 0)
			{
				nRet = 1;						// TIEMEOUT
			}else if(nRet < 0)
			{
// nRet �� �״�� ������ ��
//				nRet = -1;						// ������ �־��ּ���
			}
		}
		Ocx_ClosePort();
	}else{
		nRet = 2;								// ���̺� ���ῡ��
	}

	Disp_ErrorMessage(nRet);	
}

// ���� ���� �޽��� ǥ��
void CGTFScanOCX_DemoDlg::Disp_ErrorMessage(short nErrNo)
{
	CString  strErrMsg;

	switch((int)nErrNo)
	{
	/////////////////////////////////////////////
	// ���� �޽��� ����
	/////////////////////////////////////////////
	case 1:
		strErrMsg = _T("Time Out");
		break;
	case 2:
		strErrMsg = _T("���̺� ���ῡ��");
		break;
	/////////////////////////////////////////////
	case -1:
		strErrMsg = _T("������ �־��ּ���");
		break;

	/////////////////////////////////////////////
	// wisecube ���� ����
	/////////////////////////////////////////////
	case -10:
		strErrMsg = _T("E 01:�Ϲ����� �����ν� ����");
		break;
	case -56:
		strErrMsg = _T("��� ������ �̻�");
		break;
	case -54:
		strErrMsg = _T("�� ���� ���");
		break;
	case -39:
		strErrMsg = _T("CIS ���� �̼� ����ġ �ν� �ҷ�");
		break;
	case -4:
		strErrMsg = _T("CIS ���� �̵��� ��� �߻�");
		break;

	}

	if(nErrNo)
	{
		::MessageBox(NULL, strErrMsg, _T("�����νĶ��̺귯��"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
	}
}


void CGTFScanOCX_DemoDlg::Ini_Set(int nRW)
{
	CString strFileName, strCode;
	strFileName = _T("\\GTF_SET.ini");

	char	szBuf[128];
	int nRet;


	CString strTempDir = _T("C:\\GTF_PASSPORT");		// ������ ���� ���

	if(access(strTempDir, 0) != 0)				// ������ �ִ��� Ȯ��
	{ 
		CreateDirectory(strTempDir, NULL);
	}

	TCHAR szCurPath[MAX_PATH];
	memset(szCurPath, 0x00, sizeof(szCurPath));

	strcpy(szCurPath, (LPCSTR)strTempDir);
	strcat(szCurPath, (LPCSTR)strFileName);		// C:\\GTF_PASSPORT\\GTF_SET.ini   (������)
	
	if(nRW == 0)						// �ʱⰪ �б� ó��
	{
		memset(szBuf, 0, 128);
		nRet = GetPrivateProfileInt ("ENV", "SCANNER", 0, szCurPath);			// ��ĳ�� ����
		m_cmbScan.SetCurSel(nRet);
		nRet = GetPrivateProfileInt ("ENV", "PORT_NO", 4, szCurPath);			// COM Port No
		m_cmbPortNo.SetCurSel(nRet-1);
	}
	else								// ���� ���� ó��
	{
		strCode.Format("%d", m_cmbScan.GetCurSel());					// ��ĳ�� ����
		WritePrivateProfileString("ENV", "SCANNER", strCode, szCurPath);

		strCode.Format("%d", (m_cmbPortNo.GetCurSel()+1));					// COM Port No
		WritePrivateProfileString("ENV", "PORT_NO", strCode, szCurPath);
	}
}

short CGTFScanOCX_DemoDlg::Ocx_OpenPortByNum(short PortNum)
{
	short result;
	static BYTE parms[] = VTS_I2 ;
	m_pMgr->InvokeHelper(11, DISPATCH_METHOD, VT_I2, (void*)&result, parms, PortNum);
	return result;
}

short CGTFScanOCX_DemoDlg::Ocx_ClosePort()
{
	short result;
	m_pMgr->InvokeHelper(2, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CGTFScanOCX_DemoDlg::Ocx_Scan()
{
	short result;
	m_pMgr->InvokeHelper(3, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CGTFScanOCX_DemoDlg::Ocx_ReceiveData(short TimeOut)
{
	short result;
	static BYTE parms[] = VTS_I2 ;
	m_pMgr->InvokeHelper(5, DISPATCH_METHOD, VT_I2, (void*)&result, parms, TimeOut);
	return result;
}

CString CGTFScanOCX_DemoDlg::Ocx_GetPassportInfo()
{
	CString strScan;

	m_pMgr->InvokeHelper(8, DISPATCH_METHOD, VT_BSTR, (void*)&strScan, NULL);

	return strScan;
}

short CGTFScanOCX_DemoDlg::Ocx_Clear()
{
	short result;
	m_pMgr->InvokeHelper(9, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}


void CGTFScanOCX_DemoDlg::OnClose() 
{
	Ocx_Clear();						//����� disconnect �ؾ���. (DAWIN)
	
	CDialog::OnClose();
}

void CGTFScanOCX_DemoDlg::OnSelchangeScanner() 
{
	// TODO: Add your control notification handler code here
	
}
