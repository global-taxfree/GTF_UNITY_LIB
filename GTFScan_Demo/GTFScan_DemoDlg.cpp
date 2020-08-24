// GTFScan_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Windows.h>
#include <io.h>

#include "GTFScan_Demo.h"
#include "GTFScan_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		SCANDATA_SIZE		100
#define		SCAN_TIMEOUT		10

extern "C" __declspec(dllimport) int __stdcall OpenPort();
extern "C" __declspec(dllimport) int __stdcall ClosePort();
extern "C" __declspec(dllimport) int __stdcall Scan();
extern "C" __declspec(dllimport) int __stdcall ScanCancel();
extern "C" __declspec(dllimport) int __stdcall ReceiveData(int TimeOut);
extern "C" __declspec(dllimport) int __stdcall GetPassportInfo(char *refPassInfo);
extern "C" __declspec(dllimport) int __stdcall OpenPortByNum(int PorNum);
extern "C" __declspec(dllimport) int __stdcall Clear();


/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoDlg dialog

CGTFScan_DemoDlg::CGTFScan_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGTFScan_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGTFScan_DemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGTFScan_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGTFScan_DemoDlg)
	DDX_Control(pDX, IDC_LIST1, m_Box);
	DDX_Control(pDX, IDC_COMBO2, m_cmbPortNo);
	DDX_Control(pDX, IDC_COMBO1, m_cmbScan);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGTFScan_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CGTFScan_DemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnDataScan)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoDlg message handlers

BOOL CGTFScan_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	Init_Control();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGTFScan_DemoDlg::OnPaint() 
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
HCURSOR CGTFScan_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGTFScan_DemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	Clear();						//����� disconnect �ؾ���. (DAWIN)
	
}



BOOL CGTFScan_DemoDlg::Init_Control()
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


// ��ĵ ó���� ������ ���
void CGTFScan_DemoDlg::OnDataScan() 
{
	// ��ĳ���� ��ĳ�� ���� ���� (Ini ����)
	Ini_Set(1);

	// Scan ó���� �ϰ� �����͸� ����Ѵ�
	long nRet = 0L;

	// ��ĳ�� �����Ϳ�
	char PassportInfo[SCANDATA_SIZE+1];
	memset(PassportInfo, 0x00, sizeof(PassportInfo));

	// Scanner Open
	int nPortNo = m_cmbPortNo.GetCurSel() + 1;
	nRet = OpenPortByNum(nPortNo);
//	nRet = OpenPort();
	if(nRet)
	{
		// Scan �϶�� command�� �����Ѵ�
		nRet = Scan();
		if(nRet)
		{
			// Scan �� ������� ����Ѵ�. (��ĵ ������)
			nRet = ReceiveData(SCAN_TIMEOUT);			// Time-Out �ð� ����(��)
			if(nRet > 0)								// ����ó��
			{
				// GTF ������� �������� ������ ���ġ
				GetPassportInfo(PassportInfo);

				((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
				((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(0, (LPTSTR)PassportInfo);

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
		ClosePort();
	}else{
		nRet = 2;								// ���̺� ���ῡ��
	}

	Disp_ErrorMessage(nRet);
}

// ���� ���� �޽��� ǥ��
void CGTFScan_DemoDlg::Disp_ErrorMessage(long nErrNo)
{
	CString  strErrMsg;

	switch(nErrNo)
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

void CGTFScan_DemoDlg::Ini_Set(int nRW)
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

//	GetCurrentDirectory(MAX_PATH, szCurPath);

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

