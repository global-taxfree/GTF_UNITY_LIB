
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "DawinCtrl.h"

// DAWIN ��ĳ�ʿ�
int   ( FAR PASCAL * pConnect )();
int   ( FAR PASCAL * pSetAutoMode )(HWND hWnd, UINT wm_user);
int   ( FAR PASCAL * pResetAutoMode )();
int   ( FAR PASCAL * pDoScan )();
int   ( FAR PASCAL * pDisconnect )();
char* ( FAR PASCAL *pGetNation)();
char* ( FAR PASCAL *pGetSurname)();
char* ( FAR PASCAL *pGetGivenname)();
char* ( FAR PASCAL *pGetPassnum)();
char* ( FAR PASCAL *pGetBirthdate)();
char* ( FAR PASCAL *pGetExpiredate)();
char* ( FAR PASCAL *pGetSex)();

BOOL m_bConnect = FALSE;
HINSTANCE m_hDawinDll;

CDawinCtrl gDawin;


CDawinCtrl::CDawinCtrl(void)
{
	m_bConnect = FALSE;

	DawinFuncLoad();

}


CDawinCtrl::~CDawinCtrl(void)
{

//	FreeLibrary(m_hDawinDll);
//	m_hDawinDll = 0;

	m_bConnect = FALSE;
}

BOOL CDawinCtrl::Initialize( void )
{

	return TRUE;
}


BOOL CDawinCtrl::Connect(void)
{
	// Connect�Ǿ� �ִ� ���¿����� �ٽ� Connect ���� ����
	if(m_bConnect == FALSE)
	{
		DawinFuncLoad();
		m_bConnect = pConnect();
	}

	return m_bConnect;
}

BOOL CDawinCtrl::DisConnect(void)
{

	// Clear() �Լ����� ȣ�� (DAWIN ����) --> ���� �ÿ��� ���
	BOOL bRet = pDisconnect();
	FreeLibrary(m_hDawinDll);
	m_bConnect = FALSE;
	m_hDawinDll = 0;

	return bRet;
}

BOOL CDawinCtrl::Test_DisConnect(void)
{

	BOOL bRet = pDisconnect();
	FreeLibrary(m_hDawinDll);

	return bRet;
}


BOOL CDawinCtrl::Scan(void)
{
	BOOL bRet;

	bRet = pDoScan();

	return bRet;
}

BOOL CDawinCtrl::GetData(char *refPassInfo)
{
	char PassportInfo[65+1];

	memset(PassportInfo,	0x00,	sizeof(PassportInfo));
	memset(PassportInfo,	0x20,	65);

	// ��ü�̸����� ��ȯ
	CString strTemp, strTemp1, strPassNo;

	strTemp  = pGetSurname();
	strTemp1 = pGetGivenname();
	strTemp = strTemp + _T(" ") + strTemp1;
	// ���ǹ�ȣ (NULL �������� ���)
	strPassNo = pGetPassnum();

	strncpy((char*)&PassportInfo[0],  (LPCSTR)strTemp,          strTemp.GetLength());	// ��ü �̸�
	strncpy((char*)&PassportInfo[40], (LPCSTR)strPassNo,	    strPassNo.GetLength());	// ���ǹ�ȣ
	strncpy((char*)&PassportInfo[49], (LPCSTR)pGetNation(),	    3);		// �����ڵ�
	strncpy((char*)&PassportInfo[52], (LPCSTR)pGetSex(),	    1);		// ����
	strncpy((char*)&PassportInfo[53], (LPCSTR)pGetBirthdate(),	6);		// �������
	strncpy((char*)&PassportInfo[59], (LPCSTR)pGetExpiredate(),	6);		// ���� �������

	memcpy(refPassInfo,	PassportInfo,	strlen(PassportInfo));

	return 1;
}

BOOL CDawinCtrl::DawinFuncLoad(void)
{
	m_hDawinDll = LoadLibrary(_T("Dwscan.dll"));
	if (m_hDawinDll)
	{
		(FARPROC &)pConnect		    = GetProcAddress(m_hDawinDll, "Connect");
		(FARPROC &)pSetAutoMode     = GetProcAddress(m_hDawinDll, "SetAutoMode");
		(FARPROC &)pResetAutoMode   = GetProcAddress(m_hDawinDll, "ResetAutoMode");
		(FARPROC &)pDoScan          = GetProcAddress(m_hDawinDll, "DoScan");
		(FARPROC &)pDisconnect	    = GetProcAddress(m_hDawinDll, "Disconnect");
		(FARPROC &)pGetNation       = GetProcAddress(m_hDawinDll, "GetNation");
		(FARPROC &)pGetSurname      = GetProcAddress(m_hDawinDll, "GetSurname");
		(FARPROC &)pGetGivenname    = GetProcAddress(m_hDawinDll, "GetGivenname");
		(FARPROC &)pGetPassnum      = GetProcAddress(m_hDawinDll, "GetPassnum");
		(FARPROC &)pGetBirthdate    = GetProcAddress(m_hDawinDll, "GetBirthdate");
		(FARPROC &)pGetExpiredate   = GetProcAddress(m_hDawinDll, "GetExpiredate");
		(FARPROC &)pGetSex          = GetProcAddress(m_hDawinDll, "GetSex");

		if( !(pConnect) || !(pDisconnect) || !(pSetAutoMode) || !(pResetAutoMode) || !(pDoScan) || !(pGetNation)
			|| !(pGetSurname) || !(pGetGivenname) || !(pGetPassnum) || !(pGetBirthdate) || !(pGetExpiredate) || !(pGetSex))
		{
			AfxMessageBox("Function load Fail!!");
		}
	}
	else
	{
		AfxMessageBox("Failed to load dll file!!");
		return FALSE;
	}

	return TRUE;
}