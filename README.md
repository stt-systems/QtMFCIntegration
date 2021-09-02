# QtMFCIntegration

No problem if the Qt window is non-modal.

If it is modal, the problem is that MFC and Qt have independent event loops, so a dialog that is modal Qt is not modal for MFC.

To fix it, we create an *invisible shield* in MFC (a transparent modal dialog of the size of the main window). This *shield* captures all events not targeted to the dialog, and activates the Qt dialog instead. Also, it temporarily disables MFC keyboard shortcuts.

A complete example can be seen in the local projects `QSampleDLL` / `MFCApplication`.

## The DLL using Qt: QSampleDLL

Check the `QSampleDLL` sample project.

- Create a `QApplication` if it doesn't exist (can use `STT::QtMFCIntegration::testAndCreateQtApplication` to ease the process).
- The MFC application must have something like an `::activateWindow()` that brings the dialog to front (`QDialog::activateWindow()`).
- Optional: translations, icon, window title, stylesheet.

The Qt dialog also uses a delegate pattern to notify that the dialog will be shown or hidden:

```cpp
class Delegate {
public: // compatibility with non-Qt applications
  virtual void myDialogWillBeShown() {}
  virtual void myDialogHasBeenClosed() {}
};
```

- Before showing the dialog

```cpp
QObject::connect(&dlg, &QDialog::finished, [=]() {
  m_delegate->myDialogHasBeenClosed();
});
m_delegate->myDialogWillBeShown();
```

## MFC application

Check the `MFCApplication` sample project.

The GUI in MFC must create the *shield* each time the Qt dialog is launched, and deactivate it on close.

Check `QtModal_MFC.h`:

- `SetUpQtModalInMFC` prepares the environment to create the _shield_.
- `BeginQtModalInMFC` launches the _shield_ and invokes the callback.
- `EndQtModalInMFC` deactivates the _shield_.

Example:

```cpp
SetUpQtModalInMFC(IDD_QT_MODAL_DIALOG, this, [=](int id) {
  SetFocusToQtDialog(id);
});

// ...

void MFCApplication::SetFocusToQtDialog(int id)
{
  assert(id >= 0 && id < (int)QT_DIALOG_IDS);

  switch ((QT_DIALOG_ID)id) {
  case IDD_QT_MODAL_DIALOG:
    if (m_dialog) {
      m_dialog->activateWindow();
    }
    break;
  }
}
```

The callback will be in charge of activating (bring to front) the Qt dialog when it is necessary. The ID that receives will be used to differentiate between several Qt dialog that may coexist in the application.

To finish, inherit the delegate and invoke `BeginQtModalInMFC` and `EndQtModalInMFC`.

```cpp
class MFCApplication : public QSampleDLL::Delegate {
private:
  virtual void myDialogWillBeShown() override { QtBeginModal(IDD_QT_MODAL_DIALOG); }
  virtual void myDialogHasBeenClosed() override { QtEndModal(); }
};
```

## Embedding an MFC control on Qt

Sometimes re-using an existing MFC component is a lot quicker than developing a new one based on Qt, at least in the short term, and there are some scenarios where it is possible to achieve, basically those that have limited interaction (like a visualization-only control or a customized button). The guidance here is to create a wrapper on the control that is not MFC-dependant, and create the MFC class on an empty `QWidget` (you can get the window handle `HWND` using `QWidget::winId`). The wrapper should also take care of events like resize, click, etc.
