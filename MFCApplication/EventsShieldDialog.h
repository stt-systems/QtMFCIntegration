#pragma once

#include "QtModal_MFC.h"
#include "targetver.h"
#include <afxdialogex.h>

/**
 This invisible dialog is in charge of, while active, forwarding all the messages to Qt in order to make the current
 QDialog a modal one respect MFC.
 */
class EventsShieldDialog : protected CDialog
{
public:
  /**
   Launches the shield.
   @param resource_id Resource ID associated with the shield dialog in the application.
   @param parent Parent window.
   @param on_activate_callback Callback to invoke each time the Qt dialog has to be brought to front.
   @param dialog_id ID associated to the Qt dialog.
   */
  static void DeployShield(UINT resource_id, CWnd *parent, const QtModalActivateCallback &on_activate_callback,
                           int dialog_id);

  /**
   Closes the shield.
   */
  static void FoldShield();

protected:
  static void CALLBACK TimerEvent(HWND, UINT, UINT_PTR nIDEvent, DWORD);

  DECLARE_MESSAGE_MAP()

protected:
  /**
   @param resource_id Resource ID associated with the shield dialog in the application.
   @param parent Parent window.
   */
  explicit EventsShieldDialog(UINT resource_id, CWnd *parent, const QtModalActivateCallback &on_activate_callback,
                              int dialog_id);
  virtual ~EventsShieldDialog();

protected: // messages that must be captured and forwarded to Qt
  BOOL PreTranslateMessage(MSG *pMsg);

  afx_msg void OnWindowPosChanging(WINDOWPOS FAR *lpwndpos);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnActivateApp(BOOL bActive, DWORD);
  afx_msg void OnActivate(UINT nState, CWnd *, BOOL);

private:
  struct Private;
  Private *p;
};
