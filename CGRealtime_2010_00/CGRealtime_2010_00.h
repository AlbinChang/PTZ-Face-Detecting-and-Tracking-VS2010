
// CGRealtime_2010_00.h : CGRealtime_2010_00 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCGRealtime_2010_00App:
// �йش����ʵ�֣������ CGRealtime_2010_00.cpp
//

class CCGRealtime_2010_00App : public CWinAppEx
{
public:
	CCGRealtime_2010_00App();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGRealtime_2010_00App theApp;
