
// KpmRecordDlg.h : 头文件
//

#pragma once
#include <list>

struct KpmRecord
{
	int nDay;
	int nKpm;
	int nKeyCount;
};

// CKpmRecordDlg 对话框
class CKpmRecordDlg : public CDialogEx
{
// 构造
public:
	CKpmRecordDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KPMRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
#define KEYTIMER 1
protected:
	HICON m_hIcon;
	int m_nMinute;
	int m_nToday;
	int m_nOldKeyCount;//上一分钟这一秒的键盘次数
	int m_nKeyCount;//当前分钟内的键盘次数
	int m_nKeyCountOfDay;//今天的按键总次数
	int m_nTodayKpm;//今日最高KPM
	int m_nTopKpm;//历史最高KPM
	int m_nTopKeyCount;//最高一天的按键次数
	int m_nAvageKeyCount;//平均每天按键次数
	std::list<KpmRecord> m_lRecords;
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	void InitRecord();
	void InstallHook();
	void EndHook(void);
	void UpdateKeyTime();
	void DrawKpmInfo();
	void SaveInfo();
	void LoadInfo();
	//LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


};
