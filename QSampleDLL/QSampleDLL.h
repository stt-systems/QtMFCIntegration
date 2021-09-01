#ifndef QSAMPLEDLL_H_
#define QSAMPLEDLL_H_

#include <string>

#ifdef QSAMPLEDLL_LIB
#  define QSAMPLEDLL_EXPORT __declspec(dllexport)
#else
#  define QSAMPLEDLL_EXPORT __declspec(dllimport)
#endif

class QSAMPLEDLL_EXPORT QSampleDLL
{
public:
  struct Delegate
  {
    virtual void myDialogWillBeShown() {}
    virtual void myDialogHasBeenClosed() {}
  };

public:
  explicit QSampleDLL(const std::string &title);
  virtual ~QSampleDLL();

public: // data specifics
  void setText(const std::string &text) noexcept;
  std::string getText() const noexcept;
  std::wstring getWText() const noexcept;

  void setNumber(int number) noexcept;
  int getNumber() const noexcept;

public:
  /**
   Shows the dialog.
   @param delegate The delegate for Qt-MFC integration.
   @return Whether the dialog was accepted or rejected.
   */
  bool showDialog(Delegate *delegate);

  /**
   Shows a sample message box with the current values.
   @param delegate The delegate for Qt-MFC integration.
   */
  void showMessageBox(Delegate *delegate);

  /**
   Brings dialog to front.
   */
  void activateWindow();

  /**
   Sets the locale and install the corresponding translators.
   @param locale Locale code as expected by QLocale (https://doc.qt.io/qt-5/qlocale.html#QLocale-1)
   */
  void setLocale(const std::string &locale);

private:
  struct Private;
  Private *p{nullptr};
};

#endif
