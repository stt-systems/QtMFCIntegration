#ifndef QTMFCINTEGRATION_H_
#define QTMFCINTEGRATION_H_

#ifdef QTMFCINTEGRATION_LIB
#  define QTMFCINTEGRATION_EXPORT __declspec(dllexport)
#else
#  define QTMFCINTEGRATION_EXPORT __declspec(dllimport)
#endif

#include <cassert>
#include <type_traits>
#include <qmessagebox.h>

namespace STT::QtMFCIntegration
{
  /**
   Generic delegate to handle when a Qt dialog is shown and closed.
   */
  struct QtDialogsDelegate
  {
    virtual void qtDialogWillBeShown() {}
    virtual void qtDialogHasBeenClosed() {}
  };

  namespace Utils
  {
    // Gets the class of a member function (if so)
    // https://stackoverflow.com/q/42175294/1485885
    template<typename T>
    struct ClassOf
    {
      using type = void;
    };
    template<typename Return, typename Class>
    struct ClassOf<Return(Class::*)>
    {
      using type = Class;
    };
  }

  /**
   Wrapper to other delegates, compatible with method requiring a QtDialogsDelegate.
   @tparam Delegate Class of the delegate to wrap.
   @tparam FuncWillBeShown Type of the method to call when the Qt dialog will be shown.
   @tparam FuncHasBeenClosed Type of the method to call when the Qt dialog has been closed.
   */
  template<class Delegate, typename FuncWillBeShown, typename FuncHasBeenClosed>
  class QtDialogsDelegateWrapper final : public QtDialogsDelegate
  {
    Delegate *m_base_delegate;
    FuncWillBeShown m_will_be_shown;
    FuncHasBeenClosed m_has_been_closed;

  public:
    explicit QtDialogsDelegateWrapper(Delegate *base_delegate, FuncWillBeShown will_be_shown,
                                      FuncHasBeenClosed has_been_closed)
      : m_base_delegate{base_delegate}, m_will_be_shown{will_be_shown}, m_has_been_closed{has_been_closed}
    {
    }
    QtDialogsDelegateWrapper() = delete;

    void qtDialogWillBeShown() override
    {
      using Class = typename Utils::ClassOf<FuncWillBeShown>::type;
      if constexpr (std::is_void_v<Class>) { // it is a function, lambda or std::function
        m_will_be_shown();
      } else if constexpr (std::is_convertible_v<decltype(m_base_delegate), Class *>) {
        if (m_base_delegate) { (m_base_delegate->*m_will_be_shown)(); }
      } else {
        static_assert(false, "Delegate doesn't have a method compatible with FuncWillBeShown");
      }
    }
    void qtDialogHasBeenClosed() override
    {
      using Class = typename Utils::ClassOf<FuncHasBeenClosed>::type;
      if constexpr (std::is_void_v<Class>) { // it is a function, lambda or std::function
        m_has_been_closed();
      } else if constexpr (std::is_convertible_v<decltype(m_base_delegate), Class *>) {
        if (m_base_delegate) { (m_base_delegate->*m_has_been_closed)(); }
      } else {
        static_assert(false, "Delegate doesn't have a method compatible with FuncHasBeenClosed");
      }
    }
  };

  /**
   Helper function to create a QApplication in case no one exists yet.
   If a new QApplication is created, then it will inherit following fields from original executable:
   - Product name
   - Product version
   - Icon
   */
  QTMFCINTEGRATION_EXPORT void testAndCreateQtApplication();

  // Stack operations to get the front-most dialog.
  /**
   Returns the front-most dialog (nullptr if none).
   */
  QTMFCINTEGRATION_EXPORT QDialog *getActiveDialog();

  /**
   Returns the number of dialogs in the stack.
   */
  QTMFCINTEGRATION_EXPORT int getSizeOfDialogStack();

  /**
   Pushes a dialog into the stack.
   @param dialog The QDialog to push.
   */
  QTMFCINTEGRATION_EXPORT void pushActiveDialog(QDialog *dialog);

  /**
   Removes a dialog from the top of the stack. It does nothing if the stack is empty.
   */
  QTMFCINTEGRATION_EXPORT void popActiveDialog();

  /**
   Shows a message box, similarly as done manually with QMessageBox, but it adds the Qt-MFC integration sugar. It is not
   necessary to use this function if the message box is shown from another dialog already integrated, but it is
   required if the mesage box is invoked without any supporting parent.
   @param parent Parent widget (can be nullptr).
   @param title Title of the message box (the message box will use the application's name if empty).
   @param icon Icon of the message box.
   @param label Message text.
   @param buttons Buttons to show.
   @param default_button Button to be defaulted.
   @param delegate The Qt-MFC integration delegate.
   */
  QTMFCINTEGRATION_EXPORT int showMessageBox(QWidget *parent, const QString &title, QMessageBox::Icon icon,
                                             const QString &label,
                                             QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                             QMessageBox::StandardButton default_button = QMessageBox::Ok,
                                             QtDialogsDelegate *delegate = nullptr);

  /**
   Executes a dialog, similarly as doing `dialog->exec()`, but it adds the Qt-MFC integration sugar.
   @param dialog The dialog to show.
   @param delegate The Qt-MFC integration delegate.
   */
  QTMFCINTEGRATION_EXPORT int execDialog(QDialog *dialog, QtDialogsDelegate *delegate);
}

#endif
