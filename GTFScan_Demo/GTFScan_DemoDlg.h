// GTFScan_DemoDlg.h : header file
//

#if !defined(AFX_GTFSCAN_DEMODLG_H__193D87C3_792E_43C1_8804_2F66CCD6FAD6__INCLUDED_)
#define AFX_GTFSCAN_DEMODLG_H__193D87C3_792E_43C1_8804_2F66CCD6FAD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoDlg dialog

class CGTFScan_DemoDlg : public CDialog
{
// Construction
public:
	CGTFScan_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGTFScan_DemoDlg)
	enum { IDD = IDD_GTFSCAN_DEMO_DIALOG };
	CListBox	m_Box;
	CComboBox	m_cmbPortNo;
	CComboBox	m_cmbScan;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGTFScan_DemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL Init_Control();
	void Ini_Set(int nRW);
	void Disp_ErrorMessage(long nErrNo);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGTFScan_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDataScan();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GTFSCAN_DEMODLG_H__193D87C3_792E_43C1_8804_2F66CCD6FAD6__INCLUDED_)
