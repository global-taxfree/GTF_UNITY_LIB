// GTF_PASSPORT_COMM_ActiceXCtl.cpp : Implementation of the CGTF_PASSPORT_COMM_ActiceXCtrl ActiveX Control class.

#include <windows.h>

#include "stdafx.h"
#include "GTF_PASSPORT_COMM_ActiceX.h"
#include "GTF_PASSPORT_COMM_ActiceXCtl.h"
#include "GTF_PASSPORT_COMM_ActiceXPpg.h"

#include "ComDef.h"
#include "Comm.h"
#include "CommCtrl.h"
#include "Serial.h"
#include "WS420Ctrl.h"
#include "DawinCtrl.h"

void HexDump(unsigned char *pDcs, int len);

#define DATA_LEN	106

extern CCommCtrl	gCom;								// GTF용 구형
extern CWS420Ctrl   gWiseCom;							// Wisecude420용 통신
extern CDawinCtrl   gDawin;								// DAWIN용 내부함수

extern INT AutoDetect(HWND hWnd, INT iBaudRate );

static UINT	g_nPortNum = 0;
static UINT	g_nBaudRate = 115200;
static BOOL g_bCommOpened = FALSE;

static BYTE	g_CommBuf[512];
// }


int Set_ScannerKind(void);
HWND g_hWnd;
CString g_MRZ1;
CString g_MRZ2;
int  g_ScanKind = 0;					// 스캐너 종류 0:GTF 구형  1:wisecube420  2:DAWIN

CSerial rs232;
CRITICAL_SECTION g_cx;

enum
{
	CHK_STX,
	CHK_LENGTH,
	CHK_COMMAND,
	CHK_DATA,
	CHK_ETX,
	CHK_LRC
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGTF_PASSPORT_COMM_ActiceXCtrl, COleControl)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl, COleControl)
	//{{AFX_MSG_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "OpenPort", OpenPort, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "ClosePort", ClosePort, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "Scan", Scan, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "ScanCancel", ScanCancel, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "ReceiveData", ReceiveData, VT_I2, VTS_I2)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "GetMRZ1", GetMRZ1, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "GetMRZ2", GetMRZ2, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "GetPassportInfo", GetPassportInfo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "Clear", Clear, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "CheckReceiveData", CheckReceiveData, VT_I2, VTS_NONE)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "OpenPortByNum", OpenPortByNumber, VT_I2, VTS_I2)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "OpenPortByNumber", OpenPortByNumber, VT_I2, VTS_I2)
	DISP_FUNCTION(CGTF_PASSPORT_COMM_ActiceXCtrl, "testAlert", testAlert, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl, COleControl)
	//{{AFX_EVENT_MAP(CGTF_PASSPORT_COMM_ActiceXCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CGTF_PASSPORT_COMM_ActiceXCtrl, 1)
	PROPPAGEID(CGTF_PASSPORT_COMM_ActiceXPropPage::guid)
END_PROPPAGEIDS(CGTF_PASSPORT_COMM_ActiceXCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CGTF_PASSPORT_COMM_ActiceXCtrl, "GTFPASSPORTCOMMACTICEX.GTFPASSPORTCOMMActiceXCtrl.1",
	0xbf7e50a7, 0x7f9b, 0x4d1e, 0xbe, 0x6a, 0xff, 0xef, 0x30, 0x9a, 0x52, 0xd1)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CGTF_PASSPORT_COMM_ActiceXCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DGTF_PASSPORT_COMM_ActiceX =
		{ 0xabd0ad29, 0x5e2a, 0x4def, { 0xb0, 0x18, 0xdc, 0x48, 0xdc, 0x34, 0x20, 0x99 } };
const IID BASED_CODE IID_DGTF_PASSPORT_COMM_ActiceXEvents =
		{ 0x1357b0da, 0xe04, 0x42cf, { 0x91, 0xa1, 0xf6, 0x38, 0x1e, 0xc9, 0xc5, 0x3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwGTF_PASSPORT_COMM_ActiceXOleMisc =
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CGTF_PASSPORT_COMM_ActiceXCtrl, IDS_GTF_PASSPORT_COMM_ACTICEX, _dwGTF_PASSPORT_COMM_ActiceXOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::CGTF_PASSPORT_COMM_ActiceXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CGTF_PASSPORT_COMM_ActiceXCtrl

BOOL CGTF_PASSPORT_COMM_ActiceXCtrl::CGTF_PASSPORT_COMM_ActiceXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_GTF_PASSPORT_COMM_ACTICEX,
			IDB_GTF_PASSPORT_COMM_ACTICEX,
			afxRegApartmentThreading,
			_dwGTF_PASSPORT_COMM_ActiceXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::CGTF_PASSPORT_COMM_ActiceXCtrl - Constructor

CGTF_PASSPORT_COMM_ActiceXCtrl::CGTF_PASSPORT_COMM_ActiceXCtrl()
{
	InitializeIIDs(&IID_DGTF_PASSPORT_COMM_ActiceX, &IID_DGTF_PASSPORT_COMM_ActiceXEvents);

	g_ScanKind = Set_ScannerKind();							// 스캐너 종류 파라미터
	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::~CGTF_PASSPORT_COMM_ActiceXCtrl - Destructor

CGTF_PASSPORT_COMM_ActiceXCtrl::~CGTF_PASSPORT_COMM_ActiceXCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::OnDraw - Drawing function

void CGTF_PASSPORT_COMM_ActiceXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::DoPropExchange - Persistence support

void CGTF_PASSPORT_COMM_ActiceXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl::OnResetState - Reset control to default state

void CGTF_PASSPORT_COMM_ActiceXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CGTF_PASSPORT_COMM_ActiceXCtrl message handlers


short CGTF_PASSPORT_COMM_ActiceXCtrl::OpenPort() 
{
	g_ScanKind = Set_ScannerKind();							// 스캐너 종류 파라미터

	switch(g_ScanKind)
	{
	case 0:
		// 자동으로 port 번호 검색
		g_nPortNum = AutoDetect(g_hWnd, g_nBaudRate);

		// Open된 상태에 따라서 Port Open 처리
		if( !g_bCommOpened ){
			g_bCommOpened = gCom.OpenPort(g_hWnd, g_nPortNum, g_nBaudRate );
		}else{
			gCom.SetHwnd(g_hWnd);
		}
		break;

	case 1:
		g_nPortNum = gWiseCom.SearchPort(&rs232);
		g_bCommOpened = gWiseCom.Open(&rs232, g_nPortNum, g_nBaudRate );
		break;

	case 2:
		g_bCommOpened = gDawin.Connect();					// 1:성공  0:실패
		break;
	}

	if( g_bCommOpened )
	{
		::InitializeCriticalSection(&g_cx);
		return 1;
	}
	return 0;
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::OpenPortByNum(short PorNum) 
{
	// OpenPortByNumer() 와 소스내용은 동일(수정 시 두군데 수정해야 함)
	g_ScanKind = Set_ScannerKind();							// 스캐너 종류 파라미터

	CString strTemp;
	strTemp.Format("ScnnerKind: %d , PorNum: %d", g_ScanKind, PorNum);
	
	
	switch(g_ScanKind)
	{
	case 0:
		if( !g_bCommOpened ){
			g_bCommOpened = gCom.OpenPort(g_hWnd, PorNum, g_nBaudRate );
		}else{
			gCom.SetHwnd(g_hWnd);
		}
		break;

	case 1:
		g_bCommOpened = gWiseCom.Open(&rs232, PorNum, g_nBaudRate );
		break;

	case 2:
		g_bCommOpened = gDawin.Connect();					// 1:성공  0:실패
		break;
	}

	if( g_bCommOpened )
	{
		::InitializeCriticalSection(&g_cx);

		return 1;
	}

	return 0;
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::OpenPortByNumber(short PorNum) 
{
	// OpenPortByNum() 와 소스내용은 동일(수정 시 두군데 수정해야 함)
	
	g_ScanKind = Set_ScannerKind();							// 스캐너 종류 파라미터

	CString strTemp;
	strTemp.Format("ScnnerKind: %d , PorNum: %d", g_ScanKind, PorNum);
	
	switch(g_ScanKind)
	{
	case 0:
		if( !g_bCommOpened ){
			g_bCommOpened = gCom.OpenPort(g_hWnd, PorNum, g_nBaudRate );
		}else{
			gCom.SetHwnd(g_hWnd);
		}
		break;

	case 1:
		g_bCommOpened = gWiseCom.Open(&rs232, PorNum, g_nBaudRate );
		break;

	case 2:
		g_bCommOpened = gDawin.Connect();					// 1:성공  0:실패
		break;
	}

	if( g_bCommOpened )
	{
		::InitializeCriticalSection(&g_cx);

		return 1;
	}

	return 0;
}

int ExceptionReceiveData(){

	switch(g_ScanKind)
	{
	case 0:
	    gCom.m_Queue.GetData( 200, g_CommBuf );
		break;

	case 1:
		break;

	case 2:
		break;
	}

	return 1;
}


int ReceiveCommData(){

    int     cnt;

	char    MRZ1[45];
	char    MRZ2[45];
	int     rcv_len=0;
	int     state=CHK_STX;
	BYTE    rcv_packet[512];

    int     len=0;
	int		run_f=1;
	int		nErrNo = 1;

	switch(g_ScanKind)
	{
	case 0:
		{
		cnt = gCom.m_Queue.GetData( DATA_LEN, g_CommBuf );
		printf("%d ", cnt);
		if(!cnt)
		{
			return 0;
		}
		else
		{
			memset(rcv_packet,	0x00,	sizeof(rcv_packet));
			memcpy((char*)&rcv_packet[rcv_len],(const char*)g_CommBuf, cnt);
		}

//		HexDump((unsigned char*)rcv_packet,  cnt);	

		rcv_len+=cnt;

		while(run_f)
		{
			switch(state)
			{
				case CHK_STX:
					if(rcv_packet[0]==0x02)
					{
						state=CHK_LENGTH;
					}
					break;
				case CHK_LENGTH:
					if(rcv_len>2)
					{
						//len =  *( (unsigned short *)&rcv_packet[1]);
						len =  rcv_packet[1]<<8 | rcv_packet[2];
						if(rcv_len> (len+3))
						{
							state=CHK_COMMAND;
						}
						else
						{
							return 0;
						}
					}
					else
						return 0;

					break;
				case CHK_COMMAND:
					if(rcv_packet[3]==0xB1)
					{
						state=CHK_DATA;
					}
					else
					{
						rcv_len=0;
						state=CHK_STX;
						return 0;
					}
					break;
				case CHK_DATA:
				case CHK_ETX:
					if(len == 102)  //success
					{
						memset(MRZ1,0x00,45);
						memset(MRZ2,0x00,45);
						strncpy(MRZ1,(const char *)&rcv_packet[4],44);
						strncpy(MRZ2,(const char *)&rcv_packet[48],44);
						g_MRZ1 = (CString)MRZ1;
						g_MRZ2 = (CString)MRZ2;

						if(rcv_packet[104] == 0x03) //ETX
							state=CHK_LRC;
						else
						{
							rcv_len=0;
							state=CHK_STX;
							return 0;
						}

					}
					else
					{
						if(rcv_packet[5] == 0x03) //ETX
							state=CHK_LRC;
						else
						{
							rcv_len=0;
							state=CHK_STX;
							return 0;
						}
						return -1;
					}
					break;
				case CHK_LRC:
					rcv_len = (len == 102)?  (rcv_len-106) : (rcv_len-7);
					if(rcv_len)
					{
						int i = (len == 102)?  106:7;

						for(int j=0;j<rcv_len;j++,i++)
						{
							rcv_packet[j]=rcv_packet[i];
						}
					}
					run_f=0;
					state=CHK_STX;
					break;
			}
		}
		}

		break;

	case 1:
		{
		char response[1024];		// command response buff
		char cmd[3] = {0};
		char dat[1024];
		int  timer_req  = 0;
		int	 timercount = -1;		// timeout count value of one shot state
		int  ret = 0;
		int  cnt = 0;

		gWiseCom.Flush(&rs232);

		memset(response, 0x00, sizeof(response));
		memset(dat, 0x00, sizeof(dat));

		while(1)
		{
			if( gWiseCom.ReadDataWaiting(&rs232) )
			{
				ret = gWiseCom.ReadData(&rs232, response, 1);
				if( ret == 1 ) 
				{
					// **********************************************
					// command response 
					// **********************************************
					if( response[0] == '#' )
					{
						ret = gWiseCom.ReadUpto( &rs232, &response[1], sizeof(response) - 2, 100, 0X00 );		// 명령 2바이트를 다시 보냄
						if( ret > 0 ) 
						{
							ret = gWiseCom.check_response( response, ret + 1, cmd, dat, sizeof(dat) );
							switch(ret)
							{
								case 0:
									printf( "[ER]:no response\n" );
									break;
								case 1:					// 정상 응답 (P)
									printf( "[OK]:%s \nCMD:%s DAT:%s\n",response, cmd, dat );

									if( cmd[0] == 'G' && cmd[1] == 'S' ) {                             // #GS 기기상태 확인
										printf( "sensor: ");
										if(      dat[0] == '0') printf( "Document not detected\n");
										else if( dat[0] == '1') printf( "Carriage is in left side\n");
										else if( dat[0] == '2') printf( "Carriage is in right side\n");

										printf( "State : ");
										if(      dat[1] == 'I') printf( "Idle state\n");
										else if( dat[1] == 'R') printf( "Run State\n");
										else if( dat[1] == 'O') printf( "One shot state\n");
										else if( dat[1] == 'P') printf( "Run State\n");
										else if( dat[1] == 'Q') printf( "One shot state\n");
									}

									if( cmd[0] == 'S' && cmd[1] == 'T' ) {                              // Set Status 명령 #ST 기기의 동작상태 설정
										if( dat[0] == 'O' && timer_req == 1 )							// STO : 일회동작상태
										{
											timercount = 100;
											timer_req = 0;
										}
										else
											timercount = -1;	// disable timer
									}

									break;
								case 2:					// 비정상응답 (N)
									printf( "[OK]:%s \nCMD:%s ERR:%s\n",response, cmd, dat );
									char strDat[5];
									strDat[4] = 0x00;
									memcpy(strDat, dat, 4);
									nErrNo = atoi(strDat) * -1; 
									return nErrNo;

									break;
								case -1:				// 에러
									printf( "[ER]:%s\n",response);
									break;
							}
						}

					}
					else if( response[0] == 0x02 ) 
					{
						// **********************************************
						// passport data receive
						// **********************************************
						memset(&response[1], 0x00, sizeof(response) - 1);
						ret = gWiseCom.ReadUpto( &rs232, &response[1], sizeof(response) - 2, 100, 0X03 );
						response[ret] = 0x00;
						printf( "%s\n", &response[1] );

						if(response[1] == 'E')			// 문자 인식 오류
						{
							nErrNo = -10;
							break;
						}

						memset(MRZ1,0x00,45);
						memset(MRZ2,0x00,45);
						strncpy(MRZ1, (const char *)&response[2],44);
						strncpy(MRZ2, (const char *)&response[46],44);
						g_MRZ1 = (CString)MRZ1;
						g_MRZ2 = (CString)MRZ2;

						timercount = -1;

						break;
					}
				}
			} 
			else 
			{
				Sleep(100);
				cnt ++;
			}
			// **********************************************
			// oneshot mode timeout check
			// **********************************************
			if( timercount > 0 ) 
			{
				timercount --;
				if( timercount == 0 ) {
					ret = gWiseCom.SendCommand( &rs232, _T("STI") );
					timercount = -1;
				}
			}

			if(cnt == 100)
			{
				nErrNo = 0;
				break;
			}
		}
		}
		break;

	case 2:
		break;
	}

	return nErrNo;
}



short CGTF_PASSPORT_COMM_ActiceXCtrl::Scan() 
{
	switch(g_ScanKind)
	{
	case 0:
		unsigned char packet[6];
		if( g_bCommOpened )
		{
			// Scanner Code send
			memset(packet,0x00,6);
			packet[0]=0x02;  //STX
			packet[1]=0x00;  //size
			packet[2]=0x02;  //size
			packet[3]=0xA1;  //Command ID
			packet[4]=0x03;  //ETX
			for(int i=1;i<5;i++)
				packet[5]^=packet[i];

			gCom.SendData( packet,6);
			Sleep(2000);

			return 1;
		}
		break;

	case 1:
		if( g_bCommOpened )
		{
			gWiseCom.SendCommand( &rs232, _T("STQ") );			// 일회동작상태 (여권이 놓이면 데이터를 읽고 데이터 송부, 오류 응답 있음)
			return 1;
		}

		break;

	case 2:
		if( g_bCommOpened )
		{
			gDawin.Scan();
			return 1;
		}
		break;
	}

	return 0;
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::ScanCancel() 
{
	int nRet = 0;
	unsigned char packet[6];

	if(g_ScanKind)			// GTF 구형에서만 구현 가능 (캔슬 모드)
	{
		return 0;
	}


	if( g_bCommOpened )
	{
		// Scanner Code send
		memset(packet,0x00,6);
		packet[0]=0x02;  //STX
		packet[1]=0x00;  //size
		packet[2]=0x02;  //size
		packet[3]=0xAC;  //Command ID
		packet[4]=0x03;  //ETX
		for(int i=1;i<5;i++)
			packet[5]^=packet[i];

		gCom.SendData( packet,6);

		ExceptionReceiveData();

		return 1;
	}

	return 0;
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::ReceiveData(short TimeOut) 
{
	int nRetry = 0;
	int nRet = 0;
	int nReturn = 0;

	switch(g_ScanKind)
	{
	case 0:
		while(1)
		{
			g_MRZ1.Empty();
			g_MRZ2.Empty();

			nRet = ReceiveCommData();
			if(nRet == 1)
			{
				/////////////////////////////////////
				// 정상 데이터 수신
				/////////////////////////////////////
				nReturn = 1;
				break;
			}else if(nRet == -1)
			{
				/////////////////////////////////////
				// 오류 데이터 수신
				/////////////////////////////////////
				Sleep(100);

				ExceptionReceiveData();

				nReturn = -1;
				return nReturn;
			}

			if(nRetry == (TimeOut*10))
			{
				ScanCancel();
				Sleep(100);
				ExceptionReceiveData();
				nReturn = 0;
				break;
			}

			Sleep(100);
			nRetry++;
		}
		break;

	case 1:
		while(1)
		{
			g_MRZ1.Empty();
			g_MRZ2.Empty();

			nRet = ReceiveCommData();
			if(nRet == 1)
			{
				/////////////////////////////////////
				// 정상 데이터 수신
				/////////////////////////////////////
				nReturn = 1;
				break;
			}else if(nRet < 0)
			{
				/////////////////////////////////////
				// 오류 데이터 수신
				/////////////////////////////////////
				Sleep(100);

				ExceptionReceiveData();

				nReturn = nRet;						// 원래의 에러 코드를 가지고 감. (전용 에러)
				return nReturn;
			}

			if(nRetry == (TimeOut*10))
			{
				ScanCancel();
				Sleep(100);
				ExceptionReceiveData();
				nReturn = 0;
				break;
			}

			Sleep(100);
			nRetry++;
		}

		break;

	case 2:
		{
			return 1;
		}
		break;
	}



	return nReturn;
}

BSTR CGTF_PASSPORT_COMM_ActiceXCtrl::GetMRZ1() 
{
	CString strResult;
	
	char MRZ1[44+1];

	// DAWIN의 경우, 데이터 취득이 불가(공백)
	/////////////////////////////////////////////

	memset(MRZ1,	0x00,	sizeof(MRZ1));
	memcpy(MRZ1, (LPSTR)(LPCTSTR)g_MRZ1,		g_MRZ1.GetLength());

	//memcpy(refMRZ1,	MRZ1,	g_MRZ1.GetLength());

	strResult = MRZ1;

	return strResult.AllocSysString();
}

BSTR CGTF_PASSPORT_COMM_ActiceXCtrl::GetMRZ2() 
{
	CString strResult;
	char MRZ2[44+1];

	// DAWIN의 경우, 데이터 취득이 불가(공백)
	/////////////////////////////////////////////

	memset(MRZ2,	0x00,	sizeof(MRZ2));
	memcpy(MRZ2, (LPSTR)(LPCTSTR)g_MRZ2,		g_MRZ2.GetLength());

//	memcpy(refMRZ2,	MRZ2,	g_MRZ2.GetLength());

	strResult = MRZ2;
	return strResult.AllocSysString();
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::Clear() 
{

	g_MRZ1.Empty();
	g_MRZ2.Empty();

	if(g_ScanKind == 2)
	{
		gDawin.DisConnect();
		g_bCommOpened = FALSE;
	}
	
	return 0;
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::ClosePort() 
{
	if(g_bCommOpened == FALSE)
	{
		return 1;
	}

	switch(g_ScanKind)
	{
	case 0:
		gCom.ClosePort();
		gCom.Initialize();
		g_bCommOpened = FALSE;

		break;

	case 1:
		gWiseCom.Close(&rs232);
		g_bCommOpened = FALSE;
		break;

	case 2:
		g_bCommOpened = FALSE;
		break;
	}

	::DeleteCriticalSection(&g_cx);
	
	return 1;
}


BSTR CGTF_PASSPORT_COMM_ActiceXCtrl::GetPassportInfo() 
{
	CString strResult;
	char PassportInfo[65+1];

	memset(PassportInfo,	0x00,	sizeof(PassportInfo));
	memset(PassportInfo,	0x20,	65);

	switch(g_ScanKind)
	{
	case 0:
	case 1:
		if(g_MRZ1.GetLength() !=0 && g_MRZ2.GetLength() != 0)
		{
			CString tmpName = g_MRZ1.Mid(5, g_MRZ1.GetLength());
			int tmpPos = tmpName.Find("<<", 0);
			CString firstName = tmpName.Mid(0, tmpPos) ;
			CString lastName = tmpName.Mid(tmpPos, tmpName.GetLength());
			lastName.Replace("<", "");
	
			CString fullName;
			fullName.Format(_T("%s %s"), (LPCTSTR)firstName, (LPCTSTR)lastName);
	
			CString passportNo = g_MRZ2.Mid(0, 9);
			passportNo.Replace("<", "");
			CString country = g_MRZ2.Mid(10, 3);
			country.Replace("<", "");
			CString dateOfbirth = g_MRZ2.Mid(13, 6);
			dateOfbirth.Replace("<", "");
			CString gender = g_MRZ2.Mid(20, 1);
			gender.Replace("<", "");
			CString expiredate = g_MRZ2.Mid(21, 6);
			expiredate.Replace("<", "");
	
			memcpy(PassportInfo,	(LPCTSTR)fullName.GetBuffer(0), fullName.GetLength());
			memcpy(PassportInfo+40,	(LPCTSTR)passportNo.GetBuffer(0), passportNo.GetLength());
			memcpy(PassportInfo+49,	(LPCTSTR)country.GetBuffer(0), country.GetLength());
			memcpy(PassportInfo+52,	(LPCTSTR)gender.GetBuffer(0), gender.GetLength());
			memcpy(PassportInfo+53,	(LPCTSTR)dateOfbirth.GetBuffer(0), dateOfbirth.GetLength());
			memcpy(PassportInfo+59,	(LPCTSTR)expiredate.GetBuffer(0), expiredate.GetLength());
	
			//memcpy(refPassInfo,	PassportInfo,	strlen(PassportInfo));
		}
		strResult = PassportInfo;
		memset(PassportInfo,	0x00,	sizeof(PassportInfo));

		return strResult.AllocSysString();

	case 2:
		{
			gDawin.GetData(PassportInfo);
			strResult = PassportInfo;

			return strResult.AllocSysString();
		}
		break;
	}

	return strResult.AllocSysString();
}

short CGTF_PASSPORT_COMM_ActiceXCtrl::CheckReceiveData() 
{
	int nRet = 0;
	int nReturn = 0;

	nRet = ReceiveCommData();
	if(nRet == 1){
		/////////////////////////////////////
		// 정상 데이터 수신
		/////////////////////////////////////
		nReturn = 1;
	}else if(nRet == -1){
		/////////////////////////////////////
		// 오류 데이터 수신
		/////////////////////////////////////
		nReturn = -1;
	}else if(nRet == 0){
		/////////////////////////////////////
		// 데이터 미수신
		/////////////////////////////////////
		nReturn = 0;
	}

	return nReturn;
	
}


int Set_ScannerKind(void)
{
	CString strFileName;
//	strFileName = _T("\\GTF_SET.ini");
	strFileName = _T("C:\\GTF_PASSPORT\\GTF_SET.ini");

	int nRet = 0;
/*
	TCHAR szCurPath[MAX_PATH];
	memset(szCurPath, 0x00, sizeof(szCurPath));

	GetCurrentDirectory(MAX_PATH, szCurPath);
	strncat(szCurPath,  (LPCSTR)strFileName , MAX_PATH);

	CString strTemp;
	strTemp.Format("current Path: %s", (LPCTSTR)szCurPath);
	AfxMessageBox(strTemp);
*/
	//nRet = GetPrivateProfileInt ("ENV", "SCANNER", 0, szCurPath);			// 스캐너 종류
	nRet = GetPrivateProfileInt ("ENV", "SCANNER", 0, strFileName);			// 스캐너 종류
	strFileName ="";

	return nRet;
}


BSTR CGTF_PASSPORT_COMM_ActiceXCtrl::testAlert() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult ="Alert Test ";
	return strResult.AllocSysString();
}
