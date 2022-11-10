
// mdctrlDlg.h: 头文件
//

#pragma once


// CmdctrlDlg 对话框
class CmdctrlDlg : public CDialogEx
{
// 构造
public:
	CmdctrlDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MDCTRL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedWin32();
	afx_msg void OnBnClickedDriver();
	afx_msg void OnBnClickedBtnOpDir();
	afx_msg void OnBnClickedBtninstall();
	afx_msg void OnItemchangedListDrview(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListDrview(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_drview;
	CEdit DriverFilePath;
	static  CString gms_ServerName_from_btm;
	static CString gms_ServerName_from_list;
public:
	VOID ShowServiceStatus(DWORD type);
	BOOL LoadNTDriver(char* lpDriverName, char* lpDriverPathName);
	BOOL UnLoadSys(char* szSvrName);
	VOID Refresh();

};
