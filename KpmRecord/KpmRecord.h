
// KpmRecord.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKpmRecordApp: 
// �йش����ʵ�֣������ KpmRecord.cpp
//

class CKpmRecordApp : public CWinApp
{
public:
	CKpmRecordApp();

// ��д
public:
	virtual BOOL InitInstance();

protected:
//	ULONG_PTR m_gdiplusToken;    // ULONG PTR Ϊint64 ����

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CKpmRecordApp theApp;