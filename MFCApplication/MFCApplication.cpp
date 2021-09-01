// +
#include "targetver.h"
#include "afxwinappex.h"
#include "resource.h"
#include "MFCApplication.h"
#include "MainFrm.h"

#ifdef _DEBUG
#  define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMFCApplicationApp, CWinApp)
ON_COMMAND(ID_MODAL_DIALOG, &CMFCApplicationApp::OnShowModalDialog)
ON_COMMAND(ID_NON_MODAL_DIALOG, &CMFCApplicationApp::OnShowNonModalDialog)
ON_COMMAND(ID_UNPROTECTED_MESSAGE_BOX, &CMFCApplicationApp::OnShowUnprotectedMessageBox)
ON_COMMAND(ID_PROTECTED_MESSAGE_BOX, &CMFCApplicationApp::OnShowProtectedMessageBox)
END_MESSAGE_MAP()

CMFCApplicationApp::CMFCApplicationApp() noexcept
{
  m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

  SetAppID(_T("STTSystems.QtMFCIntegration.2021.9"));
}

// The one and only CMFCApplicationApp object
CMFCApplicationApp theApp;

// CMFCApplicationApp initialization
BOOL CMFCApplicationApp::InitInstance()
{
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinApp::InitInstance();

  // To create the main window, this code creates a new frame window
  // object and then sets it as the application's main window object
  auto pFrame = new CMainFrame{};
  if (!pFrame) { return FALSE; }
  m_pMainWnd = pFrame;

  // Create main MDI frame window
  if (!pFrame->LoadFrame(IDR_MAINFRAME)) { return FALSE; }

  // Try to load shared MDI menus and accelerator table
  HINSTANCE hInst = AfxGetResourceHandle();
  m_hMDIMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));

  // The main window has been initialized, so show and update it
  pFrame->ShowWindow(SW_NORMAL);
  pFrame->UpdateWindow();

  return TRUE;
}

int CMFCApplicationApp::ExitInstance()
{
  if (m_hMDIMenu) { FreeResource(m_hMDIMenu); }

  return CWinApp::ExitInstance();
}

// CMFCApplicationApp message handlers
void CMFCApplicationApp::OnShowModalDialog()
{
  auto pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
  pFrame->ShowModalDialog();
}

void CMFCApplicationApp::OnShowNonModalDialog()
{
  auto pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
  pFrame->ShowNonModalDialog();
}

void CMFCApplicationApp::OnShowUnprotectedMessageBox()
{
  auto pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
  pFrame->ShowUnprotectedMessageBox();
}

void CMFCApplicationApp::OnShowProtectedMessageBox()
{
  auto pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
  pFrame->ShowProtectedMessageBox();
}
