
// WinInet_Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWinInet_TestApp: 
// �йش����ʵ�֣������ WinInet_Test.cpp
//

class CWinInet_TestApp : public CWinApp
{
public:
	CWinInet_TestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWinInet_TestApp theApp;