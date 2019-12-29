
// FSMDlg.h : header file
//

#pragma once


// CFSMDlg dialog
class CFSMDlg : public CDialogEx
{
// Construction
public:
	CFSMDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FSM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickeddstbutton();
	afx_msg void OnBnClickedsourcebutton();
	afx_msg void OnBnClickedmainexec();
	afx_msg void OnBnClickedresetbutton();
	afx_msg void OnBnClickedexitbutton();
	CEdit _dst;
	CEdit _src;
	CEdit _dstDirName;
	CEdit _extensions;
	CButton _subDir;
	CButton _rebuild;
	CButton _move;
	CButton _copy;
};
