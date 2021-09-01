#ifndef MYWND_H_
#define MYWND_H_

#include <afxwin.h>
#include <functional>
#include <memory>

class MyWnd : public CWnd
{
  DECLARE_DYNAMIC(MyWnd)

private:
  enum Buttons {
    BUTTON1 = 1,
  };

public:
  explicit MyWnd(std::function<void()> &&callback);

public:
  BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd,
              UINT nID, CCreateContext *pContext = NULL) override;

protected:
  BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;

protected:
  DECLARE_MESSAGE_MAP()
  afx_msg void OnSize(UINT nType, int cx, int cy);

private:
  std::function<void()> m_callback;
  std::unique_ptr<CButton> m_button;
};

#endif
