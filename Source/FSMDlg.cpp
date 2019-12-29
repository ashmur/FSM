
// FSMDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FSM.h"
#include "FSMDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFSMDlg dialog



CFSMDlg::CFSMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FSM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFSMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, dst, _dst);
	DDX_Control(pDX, source, _src);
	DDX_Control(pDX, dstName, _dstDirName);
	DDX_Control(pDX, extensions, _extensions);
	DDX_Control(pDX, subDir, _subDir);
	DDX_Control(pDX, rebuild, _rebuild);
	DDX_Control(pDX, move, _move);
	DDX_Control(pDX, copy, _copy);
}

BEGIN_MESSAGE_MAP(CFSMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(dstButton, &CFSMDlg::OnBnClickeddstbutton)
	ON_BN_CLICKED(sourceButton, &CFSMDlg::OnBnClickedsourcebutton)
	ON_BN_CLICKED(mainExec, &CFSMDlg::OnBnClickedmainexec)
	ON_BN_CLICKED(resetButton, &CFSMDlg::OnBnClickedresetbutton)
	ON_BN_CLICKED(exitButton, &CFSMDlg::OnBnClickedexitbutton)
END_MESSAGE_MAP()


// CFSMDlg message handlers

BOOL CFSMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFSMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFSMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFSMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFSMDlg::OnBnClickedsourcebutton()
{
	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		_src.SetWindowText(dlg.GetPathName());

	}
}


void CFSMDlg::OnBnClickeddstbutton()
{
	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		_dst.SetWindowText(dlg.GetPathName());

	}
}


void CFSMDlg::OnBnClickedmainexec()
{
	//python integration
	char   psBuffer[128];
	FILE* pPipe;

	CString pythonLocation("\\Python38-32\\python.exe"); 
	CString scriptName("\\Scripts\\FSM.py");
	
	CString srcDir;
	CString dstDir;
	CString dstDirName;
	CString extensionList;

	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	pythonLocation = currentDir + pythonLocation;
	scriptName = currentDir + scriptName;
	
	pythonLocation.Replace((LPCWSTR)"\\", (LPCWSTR)"/");

	_src.GetWindowTextW(srcDir);
	_dst.GetWindowTextW(dstDir);
	_dstDirName.GetWindowTextW(dstDirName);
	_extensions.GetWindowTextW(extensionList);

	int subDirCheck = _subDir.GetCheck();
	int rebuildCheck = _rebuild.GetCheck();
	int moveCheck = _move.GetCheck();
	int copyCheck = _copy.GetCheck();

	int radioCheck = moveCheck | copyCheck;

	if (srcDir.IsEmpty())
	{
		AfxMessageBox(_T("Source directory is not selected!"), MB_OK | MB_ICONSTOP);
	}
	else if (dstDir.IsEmpty())
	{
		AfxMessageBox(_T("Destination directory is not selected!"), MB_OK | MB_ICONSTOP);
	}
	else if (dstDirName.IsEmpty())
	{
		AfxMessageBox(_T("Destination directory name is not specified!"), MB_OK | MB_ICONSTOP);
	}
	else if (!radioCheck)
	{
		AfxMessageBox(_T("Copy or move must be selected!"), MB_OK | MB_ICONSTOP);
	}
	else
	{
		int deleteNeeded = moveCheck == 1 ? 1 : 0;

		CString subDirString;
		subDirString.Format(_T("%d"), subDirCheck);
		CString rebuildString;
		rebuildString.Format(_T("%d"), rebuildCheck);
		CString deleteString;
		deleteString.Format(_T("%d"), deleteNeeded);

		CString stringToCall = pythonLocation + " \"" + scriptName + "\" \"" + srcDir + "\" \"" + dstDir + "\" " +
			subDirString + " " + rebuildString + " " + deleteString + " \"" + dstDirName + "\" " + extensionList;

		char sysCallString[1000];
		sprintf(sysCallString, "%ls", stringToCall.GetBuffer());

		if ((pPipe = _popen(sysCallString, "rt")) == NULL)
			exit(1);

		int returnValue = -1;
		while (fgets(psBuffer, 128, pPipe)) {
			returnValue = psBuffer[0] - 48;
		}

		if (feof(pPipe))
			int closePython = _pclose(pPipe);

		switch (returnValue)
		{
		case 0:
			AfxMessageBox(_T("Program executed succesfully!"), MB_OK | MB_ICONINFORMATION);
			break;
		case 1:
			AfxMessageBox(_T("No sufficient arguments provided!"), MB_OK | MB_ICONSTOP);
			break;
		case 2:
			AfxMessageBox(_T("Source is not a directory!"), MB_OK | MB_ICONSTOP);
			break;
		case 3:
			AfxMessageBox(_T("Destination is not a directory!"), MB_OK | MB_ICONSTOP);
			break;
		case 4:
			AfxMessageBox(_T("The destination directory with the same name already exists!"), MB_OK | MB_ICONSTOP);
			break;
		default:
			AfxMessageBox(_T("other error from the script occured"), MB_OK | MB_ICONSTOP);
			break;
		}
	}
}

void CFSMDlg::OnBnClickedresetbutton()
{
	_src.SetWindowText((LPCWSTR)"");
	_dst.SetWindowText((LPCWSTR)"");
	_dstDirName.SetWindowText((LPCWSTR)"");
	_extensions.SetWindowText((LPCWSTR)"");
	_subDir.SetCheck(0);
	_rebuild.SetCheck(0);
	_move.SetCheck(0);
	_copy.SetCheck(0);
}


void CFSMDlg::OnBnClickedexitbutton()
{
	PostQuitMessage(0);
}
