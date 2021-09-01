#include "MainFrm.h"
#include "resource.h"
#include "QtModal_MFC.h"

#ifdef _DEBUG
#  define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
ON_WM_CREATE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame() noexcept {}

CMainFrame::~CMainFrame() {}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) { return -1; }

  // Configure Qt-MFC integration
  SetupQtSupport();

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT &cs)
{
  if (!CMDIFrameWnd::PreCreateWindow(cs)) { return FALSE; }

  cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

  return TRUE;
}
