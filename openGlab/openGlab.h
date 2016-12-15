
// openGlab.h : main header file for the openGlab application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CopenGlabApp:
// See openGlab.cpp for the implementation of this class
//

class CopenGlabApp : public CWinAppEx
{
public:
	CopenGlabApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CopenGlabApp theApp;
