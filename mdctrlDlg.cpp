
// mdctrlDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "mdctrl.h"
#include "mdctrlDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString  CmdctrlDlg::gms_ServerName_from_btm = "";
CString  CmdctrlDlg::gms_ServerName_from_list = "";
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CmdctrlDlg 对话框



CmdctrlDlg::CmdctrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MDCTRL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmdctrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DRVIEW, m_list_drview);
	DDX_Control(pDX, ID_DRIVER_FILE_PATH, DriverFilePath);
}

BEGIN_MESSAGE_MAP(CmdctrlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WIN32, &CmdctrlDlg::OnBnClickedWin32)
	ON_BN_CLICKED(IDC_DRIVER, &CmdctrlDlg::OnBnClickedDriver)
	ON_BN_CLICKED(IDC_BTN_OP_DIR, &CmdctrlDlg::OnBnClickedBtnOpDir)
	ON_BN_CLICKED(IDC_BTN_install, &CmdctrlDlg::OnBnClickedBtninstall)
	#pragma warning( push )
	#pragma warning( disable : 26454 )
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DRVIEW, &CmdctrlDlg::OnItemchangedListDrview)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DRVIEW, &CmdctrlDlg::OnRclickListDrview)
	#pragma warning (pop)
END_MESSAGE_MAP()


// CmdctrlDlg 消息处理程序

BOOL CmdctrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CButton*)GetDlgItem(IDC_DRIVER))->SetCheck(TRUE);
	CRect rect;
	m_list_drview.GetWindowRect(&rect);
	m_list_drview.SetExtendedStyle(m_list_drview.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_drview.InsertColumn(0, _T("服务名"), LVCFMT_LEFT, rect.Width() / 4, 0);
	m_list_drview.InsertColumn(1, _T("描述"), LVCFMT_LEFT, 5 * rect.Width() / 8, 1);
	m_list_drview.InsertColumn(2, _T("状态"), LVCFMT_LEFT, rect.Width() / 8, 2);
	ShowServiceStatus(SERVICE_DRIVER); //or		SERVICE_WIN32

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmdctrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmdctrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmdctrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CmdctrlDlg::ShowServiceStatus(DWORD type)
{
	m_list_drview.DeleteAllItems();
	SC_HANDLE hSc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSc)
	{
		DWORD dwCount = 0;
		DWORD dwSize = 0;
		LPENUM_SERVICE_STATUS lpStatus;
		auto ret = EnumServicesStatus(hSc, type, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwCount, NULL);
		if (!ret && GetLastError() == ERROR_MORE_DATA)
		{
			//lpStatus = (LPENUM_SERVICE_STATUS)(new BYTE[dwSize]);
			lpStatus = new ENUM_SERVICE_STATUS[dwSize];
			ret = EnumServicesStatus(hSc, type, SERVICE_STATE_ALL, lpStatus, dwSize, &dwSize, &dwCount, NULL);
			if (ret)
			{
				for (DWORD i = 0; i < dwCount; ++i)
				{
					m_list_drview.InsertItem(i, lpStatus[i].lpServiceName);
					m_list_drview.SetItemText(i, 1, lpStatus[i].lpDisplayName);
					CString s;
					switch (lpStatus[i].ServiceStatus.dwCurrentState)
					{
					case SERVICE_RUNNING:
						s = _T("运行");
						break;
					case SERVICE_STOPPED:
						s = _T("停止");
						break;
					case SERVICE_PAUSED:
						s = _T("暂停");
						break;
					case SERVICE_START_PENDING:
						s = _T("正在启动");
						break;
					case SERVICE_STOP_PENDING:
						s = _T("正在停止");
						break;
					case SERVICE_CONTINUE_PENDING:
						s = _T("继续挂起");
						break;
					case SERVICE_PAUSE_PENDING:
						s = _T("暂停挂起");
						break;
					default:
						s = _T("其他");
						break;
					}
					m_list_drview.SetItemText(i, 2, s);
				}
			}
			delete [] lpStatus;
		}
		UpdateData(false);
		CloseServiceHandle(hSc);
	}
}

BOOL CmdctrlDlg::LoadNTDriver(char* lpDriverName, char* lpDriverPathName)
{
	BOOL bRet = FALSE;

	SC_HANDLE hServiceMgr = NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK = NULL;//NT驱动程序的服务句柄

	//打开服务控制管理器
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hServiceMgr == NULL)
	{
		//OpenSCManager失败
		TRACE("OpenSCManager() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BExit;
	}
	else
	{
		////OpenSCManager成功
		TRACE("OpenSCManager() ok ! \n");
	}

	//创建驱动所对应的服务
	hServiceDDK = CreateService(hServiceMgr,
		lpDriverName, //驱动程序的在注册表中的名字  
		lpDriverName, // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
		lpDriverPathName, // 注册表驱动程序的 ImagePath 值  
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	DWORD dwRtn = 0;
	//判断服务是否失败
	if (hServiceDDK == NULL)
	{
		dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
		{
			//由于其他原因创建服务失败
			TRACE("CrateService() 失败 %d ! \n", dwRtn);
			bRet = FALSE;
			goto BExit;
		}
		else
		{
			//服务创建失败，是由于服务已经创立过
			TRACE("CrateService() 服务创建失败，是由于服务已经创立过 ERROR is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
			MessageBox(NULL, "服务已经存在!", MB_ICONERROR);
		}

		// 驱动程序已经加载，只需要打开  
		hServiceDDK = OpenService(hServiceMgr, lpDriverName, SERVICE_ALL_ACCESS);
		if (hServiceDDK == NULL)
		{
			//如果打开服务也失败，则意味错误
			dwRtn = GetLastError();
			TRACE("OpenService() 失败 %d ! \n", dwRtn);
			bRet = FALSE;
			goto BExit;
		}
		else
		{
			TRACE("OpenService() 成功 ! \n");
		}
	}
	else
	{
		TRACE("CrateService() 成功 ! \n");
	}

	//开启此项服务
	bRet = StartService(hServiceDDK, NULL, NULL);
	if (!bRet)  //开启服务不成功
	{
		TRACE("StartService() 失败 服务可能已经开启%d ! \n", dwRtn);
	}
	bRet = TRUE;
	//离开前关闭句柄
BExit:
	if (hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if (hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;
}





BOOL CmdctrlDlg::UnLoadSys(char* szSvrName)
{
	//一定义所用到的变量
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = NULL;//SCM管理器的句柄,用来存放OpenSCManager的返回值
	SC_HANDLE hService = NULL;//NT驱动程序的服务句柄，用来存放OpenService的返回值
	SERVICE_STATUS SvrSta;
	//二打开SCM管理器
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		//带开SCM管理器失败
		TRACE("OpenSCManager() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		//打开SCM管理器成功
		TRACE("OpenSCManager() ok ! \n");
	}
	//三打开驱动所对应的服务
	hService = OpenService(hSCM, szSvrName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		//打开驱动所对应的服务失败 退出
		TRACE("OpenService() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		TRACE("OpenService() ok ! \n");  //打开驱动所对应的服务 成功
	}
	//四停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。  
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &SvrSta))
	{
		TRACE("用ControlService() 停止驱动程序失败 错误号:%d !\n", GetLastError());
	}
	else
	{
		//停止驱动程序成功
		TRACE("用ControlService() 停止驱动程序成功 !\n");
	}
	//五动态卸载驱动服务。  
	if (!DeleteService(hService))  //TRUE//FALSE
	{
		//卸载失败
		TRACE("卸载失败:DeleteSrevice()错误号:%d !\n", GetLastError());
	}
	else
	{
		//卸载成功
		TRACE("卸载成功 !\n");

	}
	bRet = TRUE;
	//六 离开前关闭打开的句柄
BeforeLeave:
	if (hService > 0)
	{
		CloseServiceHandle(hService);
	}
	if (hSCM > 0)
	{
		CloseServiceHandle(hSCM);
	}
	return bRet;
}

VOID CmdctrlDlg::Refresh()
{	
	UpdateData(false);
	if (IsDlgButtonChecked(IDC_DRIVER))
		ShowServiceStatus(SERVICE_DRIVER);
	if (IsDlgButtonChecked(IDC_WIN32))
		ShowServiceStatus(SERVICE_WIN32);
	m_list_drview.EnsureVisible(m_list_drview.GetItemCount() - 1, FALSE);
	return;
}



void CmdctrlDlg::OnBnClickedWin32()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowServiceStatus(SERVICE_WIN32);
}


void CmdctrlDlg::OnBnClickedDriver()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowServiceStatus(SERVICE_DRIVER);
}




void CmdctrlDlg::OnBnClickedBtnOpDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ReadFilePath;
	CFileDialog fileDlg(true, _T("sys"), _T("*.sys"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Driver Files (*.sys)|*.sys|All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		ReadFilePath = fileDlg.GetPathName();
		GetDlgItem(ID_DRIVER_FILE_PATH)->SetWindowText(ReadFilePath);
		CString filename = fileDlg.GetFileName();
		gms_ServerName_from_btm = fileDlg.GetFileName();
		UpdateData(false);
	}
}




void CmdctrlDlg::OnBnClickedBtninstall() //保留
{
	//tmpServerPath 用于接受从界面的编辑框中提取内容
	CString tmpServerPath;
	DriverFilePath.GetWindowText(tmpServerPath);
	//gms_ServerName 这个是全局的变量 复制的输入有两个 一个是 CListCtrl 界面上鼠标左键单机获取   一个是当打开驱动文件时获取
	if (tmpServerPath.IsEmpty())
	{
		MessageBox(NULL, "请选择驱动文件!", MB_ICONERROR);

	}
	else
	{
		LoadNTDriver(gms_ServerName_from_btm.GetBuffer(), tmpServerPath.GetBuffer());		
		Refresh();
	}

}




void CmdctrlDlg::OnItemchangedListDrview(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_list_drview.GetNextItem(-1, LVNI_SELECTED);
	CString s = m_list_drview.GetItemText(nIndex, 0);
	gms_ServerName_from_list = s;
	*pResult = 0;
}


void CmdctrlDlg::OnRclickListDrview(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 //TODO: 在此添加控件通知处理程序代码
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if (index == -1)
		return;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, 10001, _T("刷新"));
	AppendMenu(hMenu, MF_STRING, 10002, _T("卸载"));

	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);//弹出菜单

	switch (Cmd)//响应点击的菜单
	{
	case 10001://刷新
			{
				Refresh();
				break;
			}	
	case 10002://卸载
			{
				CString hint =  "您确定要删除  "+ gms_ServerName_from_list+"  该服务!";
				if(IDOK==MessageBox(hint, "提示!", MB_OKCANCEL | MB_ICONEXCLAMATION | MB_ICONWARNING))
				{ 
					UnLoadSys(gms_ServerName_from_list.GetBuffer());
					Refresh();
				}
				break;
			}

	}

	*pResult = 0;
}
















//void CmdctrlDlg::OnBnClickedStart()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	auto pos = m_list_drview.GetFirstSelectedItemPosition();
//	if (pos)
//	{
//		int item = m_list_drview.GetNextSelectedItem(pos);
//		if (item > -1)
//		{
//			SC_HANDLE hSc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//			if (hSc)
//			{
//				CString name = m_list_drview.GetItemText(item, 0);
//				if (!name.IsEmpty())
//				{
//					SC_HANDLE hService = OpenService(hSc, name, SERVICE_ALL_ACCESS);
//					if (hService)
//					{
//						if (StartServiceW(hService, 0, NULL))
//							m_list_drview.SetItemText(item, 2, _T("运行"));
//						else
//							AfxMessageBox(CString(_T("启动：")) + name + _T("失败!"));
//						CloseServiceHandle(hService);
//					}
//				}
//				CloseServiceHandle(hSc);
//			}
//		}
//	}
//}


//void CmdctrlDlg::OnBnClickedStop()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	auto pos = m_list_drview.GetFirstSelectedItemPosition();
//	if (pos)
//	{
//		int item = m_list_drview.GetNextSelectedItem(pos);
//		if (item > -1)
//		{
//			SC_HANDLE hSc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//			if (hSc)
//			{
//				CString name = m_list_drview.GetItemText(item, 0);
//				if (!name.IsEmpty())
//				{
//					SC_HANDLE hService = OpenService(hSc, name, SERVICE_ALL_ACCESS);
//					if (hService)
//					{
//						SERVICE_STATUS status;
//						if (ControlService(hService, SERVICE_CONTROL_STOP, &status))
//							m_list_drview.SetItemText(item, 2, _T("停止"));
//						else
//							AfxMessageBox(CString(_T("停止：")) + name + _T("失败!"));
//						CloseServiceHandle(hService);
//					}
//				}
//				CloseServiceHandle(hSc);
//			}
//		}
//	}
//}