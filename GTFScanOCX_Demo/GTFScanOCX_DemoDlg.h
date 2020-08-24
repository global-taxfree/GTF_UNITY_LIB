// GTFScanOCX_DemoDlg.h : header file
//

#if !defined(AFX_GTFSCANOCX_DEMODLG_H__C0DDD3C6_0C8E_4549_AD11_E8473614DB41__INCLUDED_)
#define AFX_GTFSCANOCX_DEMODLG_H__C0DDD3C6_0C8E_4549_AD11_E8473614DB41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGTFScanOCX_DemoDlg dialog

class CGTFScanOCX_DemoDlg : public CDialog
{
// Construction
public:
	CGTFScanOCX_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGTFScanOCX_DemoDlg)
	enum { IDD = IDD_GTFSCANOCX_DEMO_DIALOG };
	CComboBox	m_cmbPortNo;
	CComboBox	m_cmbScan;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTFScanOCX_DemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL Init_LoadOCX();
	BOOL Init_Control();
	void Ini_Set(int nRW);
	void Disp_ErrorMessage(short nErrNo);

	CWnd* m_pMgr;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGTFScanOCX_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDataScan();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSelchangeScanner();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:

// Functions
	short Ocx_ClosePort();
	short Ocx_Scan();
	short Ocx_ReceiveData(short TimeOut);
	CString Ocx_GetPassportInfo();
	short Ocx_OpenPortByNum(short PortNum);
	short Ocx_Clear();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GTFSCANOCX_DEMODLG_H__C0DDD3C6_0C8E_4549_AD11_E8473614DB41__INCLUDED_)
