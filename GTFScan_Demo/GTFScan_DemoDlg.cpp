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
	
	Clear();						//종료시 disconnect 해야함. (DAWIN)
	
}



BOOL CGTFScan_DemoDlg::Init_Control()
{
	// 콤보박스 초기화

	m_cmbScan.AddString(_T("GTF 스캐너"));
	m_cmbScan.AddString(_T("wisescan 420 스캐너"));
	m_cmbScan.AddString(_T("DAWIN 스캐너"));

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

	// 환경 초기값 취득 (스캐너 종류, Port 초기 위치)
	Ini_Set(0);

	return TRUE;
}


// 스캔 처리와 데이터 취득
void CGTFScan_DemoDlg::OnDataScan() 
{
	// 스캐닝할 스캐너 종류 저장 (Ini 파일)
	Ini_Set(1);

	// Scan 처리를 하고 데이터를 취득한다
	long nRet = 0L;

	// 스캐닝 데이터용
	char PassportInfo[SCANDATA_SIZE+1];
	memset(PassportInfo, 0x00, sizeof(PassportInfo));

	// Scanner Open
	int nPortNo = m_cmbPortNo.GetCurSel() + 1;
	nRet = OpenPortByNum(nPortNo);
//	nRet = OpenPort();
	if(nRet)
	{
		// Scan 하라는 command를 전송한다
		nRet = Scan();
		if(nRet)
		{
			// Scan 된 결과물을 취득한다. (스캔 데이터)
			nRet = ReceiveData(SCAN_TIMEOUT);			// Time-Out 시간 설정(초)
			if(nRet > 0)								// 정상처리
			{
				// GTF 구형기기 기준으로 데이터 재배치
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
// nRet 값 그대로 가지고 감
//				nRet = -1;						// 여권을 넣어주세요
			}
		}
		ClosePort();
	}else{
		nRet = 2;								// 케이블 연결에러
	}

	Disp_ErrorMessage(nRet);
}

// 통합 에러 메시지 표시
void CGTFScan_DemoDlg::Disp_ErrorMessage(long nErrNo)
{
	CString  strErrMsg;

	switch(nErrNo)
	{
	/////////////////////////////////////////////
	// 에러 메시지 내용
	/////////////////////////////////////////////
	case 1:
		strErrMsg = _T("Time Out");
		break;
	case 2:
		strErrMsg = _T("케이블 연결에러");
		break;
	/////////////////////////////////////////////
	case -1:
		strErrMsg = _T("여권을 넣어주세요");
		break;

	/////////////////////////////////////////////
	// wisecube 전용 에러
	/////////////////////////////////////////////
	case -10:
		strErrMsg = _T("E 01:일반적인 문자인식 오류");
		break;
	case -56:
		strErrMsg = _T("명령 데이터 이상");
		break;
	case -54:
		strErrMsg = _T("미 정의 명령");
		break;
	case -39:
		strErrMsg = _T("CIS 센서 이송 정위치 인식 불량");
		break;
	case -4:
		strErrMsg = _T("CIS 센서 이동중 장애 발생");
		break;

	}

	if(nErrNo)
	{
		::MessageBox(NULL, strErrMsg, _T("여권인식라이브러리"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
	}
}

void CGTFScan_DemoDlg::Ini_Set(int nRW)
{
	CString strFileName, strCode;
	strFileName = _T("\\GTF_SET.ini");

	char	szBuf[128];
	int nRet;


	CString strTempDir = _T("C:\\GTF_PASSPORT");		// 생성할 폴더 경로

	if(access(strTempDir, 0) != 0)				// 폴더가 있는지 확인
	{ 
		CreateDirectory(strTempDir, NULL);
	}
	
	TCHAR szCurPath[MAX_PATH];
	memset(szCurPath, 0x00, sizeof(szCurPath));

//	GetCurrentDirectory(MAX_PATH, szCurPath);

	strcpy(szCurPath, (LPCSTR)strTempDir);
	strcat(szCurPath, (LPCSTR)strFileName);		// C:\\GTF_PASSPORT\\GTF_SET.ini   (고정값)
	
	if(nRW == 0)						// 초기값 읽기 처리
	{
		memset(szBuf, 0, 128);
		nRet = GetPrivateProfileInt ("ENV", "SCANNER", 0, szCurPath);			// 스캐너 종류
		m_cmbScan.SetCurSel(nRet);
		nRet = GetPrivateProfileInt ("ENV", "PORT_NO", 4, szCurPath);			// COM Port No
		m_cmbPortNo.SetCurSel(nRet-1);
	}
	else								// 정보 저장 처리
	{
		strCode.Format("%d", m_cmbScan.GetCurSel());					// 스캐너 종류
		WritePrivateProfileString("ENV", "SCANNER", strCode, szCurPath);

		strCode.Format("%d", (m_cmbPortNo.GetCurSel()+1));					// COM Port No
		WritePrivateProfileString("ENV", "PORT_NO", strCode, szCurPath);
	}
}

