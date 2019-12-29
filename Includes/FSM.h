
// FSM.h : main header file for the FSM application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFSMApp:
// See FSM.cpp for the implementation of this class
//

class CFSMApp : public CWinApp
{
public:
	CFSMApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFSMApp theApp;
