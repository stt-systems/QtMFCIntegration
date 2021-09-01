#ifndef HWNDWIDGET_H_
#define HWNDWIDGET_H_

#include <qwidget.h>
#include <memory>
#include <functional>

class SampleControl;

class HWNDWidget : public QWidget
{
public:
  explicit HWNDWidget(QWidget *parent = nullptr);
  virtual ~HWNDWidget();

  void create(std::function<void()> &&callback);

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  std::unique_ptr<SampleControl> m_control;
};

#endif
