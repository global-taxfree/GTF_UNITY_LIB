// GTFScan_Demo.h : main header file for the GTFSCAN_DEMO application
//

#if !defined(AFX_GTFSCAN_DEMO_H__2C8B247F_9295_4532_A737_D0E681C9B38A__INCLUDED_)
#define AFX_GTFSCAN_DEMO_H__2C8B247F_9295_4532_A737_D0E681C9B38A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoApp:
// See GTFScan_Demo.cpp for the implementation of this class
//

class CGTFScan_DemoApp : public CWinApp
{
public:
	CGTFScan_DemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTFScan_DemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGTFScan_DemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GTFSCAN_DEMO_H__2C8B247F_9295_4532_A737_D0E681C9B38A__INCLUDED_)
