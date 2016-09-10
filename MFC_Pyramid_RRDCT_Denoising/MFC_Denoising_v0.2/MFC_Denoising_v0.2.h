
// MFC_Denoising_v0.2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFC_Denoising_v02App:
// See MFC_Denoising_v0.2.cpp for the implementation of this class
//

class CMFC_Denoising_v02App : public CWinApp
{
public:
	CMFC_Denoising_v02App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFC_Denoising_v02App theApp;