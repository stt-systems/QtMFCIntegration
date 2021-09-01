#ifndef SAMPLECONTROL_H_
#define SAMPLECONTROL_H_

#include <functional>

#ifdef SAMPLECONTROL_LIB
#  define SAMPLECONTROL_EXPORT __declspec(dllexport)
#else
#  define SAMPLECONTROL_EXPORT __declspec(dllimport)
#endif

/**
 Wrapper on MFC control (or any other Windows' one). This wrapper is necessary to prevent exposing the MFC and Qt APIs
 between libraries.
 */
class SAMPLECONTROL_EXPORT SampleControl
{
public:
  SampleControl();
  virtual ~SampleControl();

public:
  using HWND = void *; ///< Prevent including Windows.h here.

  /**
   Creates the control.
   @param hwnd Handle to window where to create the control.
   @param callback Callback to execute when the control is clicked.
   */
  void Create(HWND hwnd, std::function<void()> &&callback);

  /**
   Resizes the control.
   @param width Width in pixels.
   @param height Height in pixels.
   */
  void Resize(int width, int height);

private:
  struct Private;
  Private *p = nullptr;
};

#endif
