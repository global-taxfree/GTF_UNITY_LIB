// GTFScanOCX_Demo.h : main header file for the GTFSCANOCX_DEMO application
//

#if !defined(AFX_GTFSCANOCX_DEMO_H__E5DE3CD5_8A51_4F3F_81CB_FFD5251CF104__INCLUDED_)
#define AFX_GTFSCANOCX_DEMO_H__E5DE3CD5_8A51_4F3F_81CB_FFD5251CF104__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGTFScanOCX_DemoApp:
// See GTFScanOCX_Demo.cpp for the implementation of this class
//

class CGTFScanOCX_DemoApp : public CWinApp
{
public:
	CGTFScanOCX_DemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTFScanOCX_DemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGTFScanOCX_DemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GTFSCANOCX_DEMO_H__E5DE3CD5_8A51_4F3F_81CB_FFD5251CF104__INCLUDED_)
