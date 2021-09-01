// +
#include "HWNDWidget.h"
#include "../SampleControl/SampleControl.h"
#include <qevent.h>

HWNDWidget::HWNDWidget(QWidget *parent) : QWidget{parent} {}

HWNDWidget::~HWNDWidget() {}

void HWNDWidget::create(std::function<void()> &&callback)
{
  if (!m_control) {
    m_control = std::make_unique<SampleControl>();
    m_control->Create((SampleControl::HWND)winId(), std::move(callback));
  }
}

void HWNDWidget::resizeEvent(QResizeEvent *event)
{
  if (m_control) {
    const auto size = event->size();
    m_control->Resize(size.width(), size.height());
  }
}
