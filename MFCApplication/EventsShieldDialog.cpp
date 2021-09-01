// +
#include "EventsShieldDialog.h"
#include <cassert>
#include <atomic>
#include <mutex>

namespace
{
  const int sk_do_modal_timer{WM_USER + 127};

  std::mutex s_mutex;
  std::atomic_bool s_modal_open{false};
  static EventsShieldDialog *s_singleton_dialog{nullptr};
}

struct EventsShieldDialog::Private
{
  CWnd *parent{nullptr};
  QtModalActivateCallback on_activate_callback{nullptr};
  int dialog_id{-1};
};

BEGIN_MESSAGE_MAP(EventsShieldDialog, CDialog)
ON_WM_WINDOWPOSCHANGING()
ON_WM_SYSCOMMAND()
ON_WM_ACTIVATEAPP()
ON_WM_ACTIVATE()
END_MESSAGE_MAP()

void EventsShieldDialog::DeployShield(UINT resource_id, CWnd *parent,
                                      const QtModalActivateCallback &on_activate_callback, int dialog_id)
{
  assert(parent);

  // Prevent launching shield twice
  if (s_modal_open.exchange(true)) { return; }

  std::lock_guard<std::mutex> lock{s_mutex};

  // It should only be one shield active
  assert(!s_singleton_dialog);
  s_singleton_dialog = new EventsShieldDialog{resource_id, parent, on_activate_callback, dialog_id};

  // The shield dialog is delayed until the next event cycle, in this case executed by Qt
  s_modal_open = true;
  parent->SetTimer(sk_do_modal_timer, 100, &EventsShieldDialog::TimerEvent);
}

void EventsShieldDialog::FoldShield()
{
  std::lock_guard<std::mutex> lock{s_mutex};

  if (s_singleton_dialog) {
    // Avoid displaying the shield if it hasn't been launched yet
    s_singleton_dialog->p->parent->KillTimer(sk_do_modal_timer);

    // Close the dialog (shield)
    ::PostMessage(s_singleton_dialog->GetSafeHwnd(), WM_COMMAND, IDOK, 0);
  }
}

void EventsShieldDialog::TimerEvent(HWND, UINT, UINT_PTR nIDEvent, DWORD)
{
  if (nIDEvent != sk_do_modal_timer) { return; }

  std::unique_lock lock{s_mutex}; // in case the timer event is dispatched _after_ a FoldShield invokation

  if (s_singleton_dialog) {
    auto parent = s_singleton_dialog->p->parent;
    parent->KillTimer(sk_do_modal_timer); // this is a one-shot timer
    lock.unlock();                        // prevent a dead-lock

    // Actually show the shield
    s_singleton_dialog->DoModal();

    delete s_singleton_dialog;
    s_singleton_dialog = nullptr;

    // Bring parent MFC window to front
    parent->SetActiveWindow();

    s_modal_open = false;
  }
}

EventsShieldDialog::EventsShieldDialog(UINT resource_id, CWnd *parent,
                                       const QtModalActivateCallback &on_activate_callback, int dialog_id)
  : CDialog{resource_id, parent}, p{new Private{}}
{
  p->parent = parent;
  p->on_activate_callback = on_activate_callback;
  p->dialog_id = dialog_id;
}

EventsShieldDialog::~EventsShieldDialog()
{
  delete p;
}

BOOL EventsShieldDialog::PreTranslateMessage(MSG *pMsg)
{
  // Filter out key events (e.g.: the ESC key to close the dialog)
  if (pMsg->message == WM_KEYDOWN) { return TRUE; }

  return CDialog::PreTranslateMessage(pMsg);
}

void EventsShieldDialog::OnWindowPosChanging(WINDOWPOS FAR *lpwndpos)
{
  // Do not show the dialog on creation, wait until it is invoked
  lpwndpos->flags &= ~SWP_SHOWWINDOW;

  CDialog::OnWindowPosChanging(lpwndpos);
}

void EventsShieldDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0XFFF0) != SC_CLOSE) {
    // Discard close commands (e.g.: ALT + F4)
    CDialog::OnSysCommand(nID, lParam);
  }
}

void EventsShieldDialog::OnActivateApp(BOOL bActive, DWORD)
{
  // User has activated the application, forward petition to activate Qt's dialog instead
  if (bActive && p->on_activate_callback) { p->on_activate_callback(p->dialog_id); }
}

void EventsShieldDialog::OnActivate(UINT nState, CWnd *, BOOL)
{
  // User has activated the shield, forward petition to activate Qt's dialog instead
  if ((nState == WA_ACTIVE || nState == WA_CLICKACTIVE) && p->on_activate_callback) {
    p->on_activate_callback(p->dialog_id);
  }
}
