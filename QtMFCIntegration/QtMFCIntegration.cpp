// +
#include "QtMFCIntegration.h"

#include <stack>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsdkver.h>
#include <sdkddkver.h>
#include <Windows.h>
#include <shellapi.h>

#include <qapplication.h>
#include <qicon.h>
#include <QtWinExtras/qwinfunctions.h>

namespace
{
  /**
   Automates the invokation of delegate methods when necessary.
   */
  class AutomaticOnOff : public QObject
  {
    QDialog *m_dialog;
    QMetaObject::Connection m_connection;

  public:
    explicit AutomaticOnOff(QDialog *dialog, STT::QtMFCIntegration::QtDialogsDelegate *delegate) : m_dialog{dialog}
    {
      assert(dialog);

      // The delegate must be called only when the dialog is the first one

      m_dialog->installEventFilter(this);

      m_connection = QObject::connect(dialog, &QDialog::finished, [=]() {
        if (STT::QtMFCIntegration::getSizeOfDialogStack() == 1) { delegate->qtDialogHasBeenClosed(); }
      });

      STT::QtMFCIntegration::pushActiveDialog(dialog);

      if (STT::QtMFCIntegration::getSizeOfDialogStack() == 1) { delegate->qtDialogWillBeShown(); }
    }

    ~AutomaticOnOff()
    {
      STT::QtMFCIntegration::popActiveDialog();
      QObject::disconnect(m_connection);

      m_dialog->removeEventFilter(this);
    }

    bool eventFilter(QObject *obj, QEvent *event) override
    {
      if (event->type() == QEvent::Show) { m_dialog->activateWindow(); }
      return QObject::eventFilter(obj, event);
    }
  };

  std::stack<QDialog *> s_active_dialogs_tack;
}

QDialog *STT::QtMFCIntegration::getActiveDialog()
{
  if (s_active_dialogs_tack.empty()) { return nullptr; }
  return s_active_dialogs_tack.top();
}

QTMFCINTEGRATION_EXPORT int STT::QtMFCIntegration::getSizeOfDialogStack()
{
  return (int)s_active_dialogs_tack.size();
}

void STT::QtMFCIntegration::pushActiveDialog(QDialog *dialog)
{
  s_active_dialogs_tack.push(dialog);
}

void STT::QtMFCIntegration::popActiveDialog()
{
  if (!s_active_dialogs_tack.empty()) { s_active_dialogs_tack.pop(); }
}

int STT::QtMFCIntegration::showMessageBox(QWidget *parent, const QString &title, QMessageBox::Icon icon,
                                          const QString &label, QMessageBox::StandardButtons buttons,
                                          QMessageBox::StandardButton default_button, QtDialogsDelegate *delegate)
{
  assert(buttons & default_button); // default button must be included among buttons to be displayed

  QMessageBox dlg{icon, title, label, buttons, parent};

  dlg.setDefaultButton(default_button);

  return execDialog(&dlg, delegate);
}

int STT::QtMFCIntegration::execDialog(QDialog *dialog, QtDialogsDelegate *delegate)
{
  assert(dialog);

  auto automatic_on_off = delegate ? std::make_unique<AutomaticOnOff>(dialog, delegate) : nullptr;

  return dialog->exec();
}

namespace
{
  QString getApplicationInfo(const std::wstring &szInfo, const std::wstring &lang = L"040904e4")
  {
    // Get file full pathname
    TCHAR path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) { return {}; }

    // Then check if version info can be obtained
    DWORD dw_handle = 0;

    DWORD size = GetFileVersionInfoSize(path, &dw_handle);
    if (size < 1) { return {}; }

    // Get complete version info
    std::vector<TCHAR> data(size);
    if (GetFileVersionInfo(path, dw_handle, size, data.data()) == FALSE) { return {}; }

    // Get requested info
    TCHAR *buffer;
    UINT len;
    if (VerQueryValue(data.data(), (L"\\StringFileInfo\\" + lang + L"\\" + szInfo).c_str(), (LPVOID *)&buffer, &len) ==
        FALSE) {
      return {};
    }

    return QString::fromStdWString(buffer);
  }

  QIcon getApplicationIcon()
  {
    // Get file full pathname
    TCHAR path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) { return {}; }

    // Extract icon and convert to QIcon
    HINSTANCE instance = GetModuleHandle(NULL);
    HICON icon = ExtractIcon(instance, path, 0);

    return QIcon{QtWin::fromHICON(icon)};
  }
}

void STT::QtMFCIntegration::testAndCreateQtApplication()
{
  // Ommit if a QApplication already exists
  if (qApp) { return; }

  // These settings must be set before creating the first instance
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

  static auto original_filename = getApplicationInfo(L"OriginalFilename").toStdString();
  static int argc = 1;
  static char *argv[] = {const_cast<char *>(original_filename.c_str())};
  static auto app = new QApplication{argc, argv};

  // Copy data from executable
  qApp->setApplicationName(getApplicationInfo(L"ProductName"));
  qApp->setApplicationVersion(getApplicationInfo(L"ProductVersion"));
  qApp->setWindowIcon(getApplicationIcon());

  // Some common
  qApp->setStyle("fusion"); // force Fusion style
  qApp->setAttribute(Qt::AA_DisableWindowContextHelpButton, true);
}
