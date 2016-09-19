
// KpmRecordDlg.h : ͷ�ļ�
//

#pragma once
#include <list>

struct KpmRecord
{
	int nDay;
	int nKpm;
	int nKeyCount;
};

// CKpmRecordDlg �Ի���
class CKpmRecordDlg : public CDialogEx
{
// ����
public:
	CKpmRecordDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KPMRECORD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
#define KEYTIMER 1
protected:
	HICON m_hIcon;
	int m_nMinute;
	int m_nToday;
	int m_nOldKeyCount;//��һ������һ��ļ��̴���
	int m_nKeyCount;//��ǰ�����ڵļ��̴���
	int m_nKeyCountOfDay;//����İ����ܴ���
	int m_nTodayKpm;//�������KPM
	int m_nTopKpm;//��ʷ���KPM
	int m_nTopKeyCount;//���һ��İ�������
	int m_nAvageKeyCount;//ƽ��ÿ�찴������
	std::list<KpmRecord> m_lRecords;
protected:
	// ���ɵ���Ϣӳ�亯��
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
