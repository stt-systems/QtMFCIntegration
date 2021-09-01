#pragma once

#include <afxwin.h>

#ifndef __AFXWIN_H__
#  error "include 'stdafx.h' before including this file for PCH"
#endif

class CMFCApplicationApp : public CWinApp
{
public:
  CMFCApplicationApp() noexcept;

public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

protected:
  HMENU m_hMDIMenu;

public:
  afx_msg void OnShowModalDialog();
  afx_msg void OnShowNonModalDialog();
  afx_msg void OnShowUnprotectedMessageBox();
  afx_msg void OnShowProtectedMessageBox();
  DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationApp theApp;
