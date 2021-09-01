// +
#include "pch.h"
#include "SampleControl.h"
#include "MyWnd.h"

struct SampleControl::Private
{
  MyWnd *wnd = nullptr;
  std::function<void()> callback;
};

SampleControl::SampleControl() : p{new Private{}} {}

SampleControl::~SampleControl()
{
  delete p;
}

void SampleControl::Create(HWND hwnd, std::function<void()> &&callback)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  p->callback = callback;
  if (!p->wnd) {
    auto parent = CWnd::FromHandle((::HWND)hwnd);

    CRect rect;
    ::GetWindowRect((::HWND)hwnd, &rect);
    const auto top_left = POINT{0, 0}; // the parent rect is in it's parent's coordinates
    const auto size = SIZE{rect.Width(), rect.Height()};

    p->wnd = new MyWnd{std::move(callback)};
    p->wnd->Create(NULL, _T("MyWnd"), WS_CHILD | WS_VISIBLE, CRect{top_left, size}, parent, 0);
  }
}

void SampleControl::Resize(int width, int height)
{
  if (!p->wnd || !::IsWindow(p->wnd->GetSafeHwnd())) { return; }

  p->wnd->SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE);
}
