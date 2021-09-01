// +
#include "pch.h"
#include "MyWnd.h"

BEGIN_MESSAGE_MAP(MyWnd, CWnd)
ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(MyWnd, CWnd)

MyWnd::MyWnd(std::function<void()> &&callback) : m_callback{callback} {}

BOOL MyWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd,
                   UINT nID, CCreateContext *pContext)
{
  if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext)) { return FALSE; }

  m_button = std::make_unique<CButton>();
  m_button->Create(_T("Sample button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, BUTTON1);

  return TRUE;
}

BOOL MyWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
  switch (HIWORD(wParam)) {
  case BN_CLICKED:
    switch (LOWORD(wParam)) { // button ID
    case BUTTON1:
      if (m_callback) {
        m_callback();
      } else {
        // Note: the caption cannot be null because there is no app
        CWnd::MessageBox(_T("You've pressed the button."), _T("Title"));
      }
      break;
    }
    break;
  }

  return CWnd::OnCommand(wParam, lParam);
}

void MyWnd::OnSize(UINT nType, int cx, int cy)
{
  CWnd::OnSize(nType, cx, cy);

  if (m_button) { m_button->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE); }
}
