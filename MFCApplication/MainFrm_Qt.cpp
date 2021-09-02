// +
#include "MainFrm.h"
#include "resource.h"
#include "QtModal_MFC.h"
#include <cassert>

void CMainFrame::SetupQtSupport()
{
  // Prepares events-shield
  // The shield dialog must be defined as a dialog in the resource file
  SetUpQtModalInMFC(IDD_QT_MODAL_DIALOG, this, [=](int id) { SetFocusToQtDialog(id); });
}

void CMainFrame::QtBeginModal(int id)
{
  assert(id >= 0 && id < (int)QT_DIALOG_TOTAL_IDS);

  BeginQtModalInMFC(id);
}

void CMainFrame::QtEndModal()
{
  EndQtModalInMFC();
}

void CMainFrame::SetFocusToQtDialog(int id)
{
  assert(id >= 0 && id < (int)QT_DIALOG_TOTAL_IDS);

  const auto qt_dialog_id = (QT_DIALOG_ID)id;
  switch (qt_dialog_id) {
  case QT_DIALOG_SAMPLE_DLL:
    if (m_sample_dll_dialog) { m_sample_dll_dialog->activateWindow(); }
    break;
  }
}

namespace
{
  void configureDialog(QSampleDLL *dialog)
  {
    dialog->setLocale("es");

    // Document must be saved in UTF-8
    dialog->setText(u8"This text should be encoded in UTF-8 👍🏼😎");
    dialog->setNumber(__LINE__);
  }

  CString utf8ToCString(const std::string &str)
  {
    const auto wlen = MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), (int)str.size(), NULL, 0);
    if (wlen <= 0) { return {}; }

    std::wstring wstr;
    wstr.resize(wlen);
    MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), (int)str.size(), wstr.data(), wlen);

    return CString{wstr.c_str()};
  }
}

void CMainFrame::ShowDialog(QSampleDLL *dialog, QSampleDLL::Delegate *delegate)
{
  configureDialog(dialog);

  if (dialog->showDialog(delegate)) {
    CString message;

    const auto text = utf8ToCString(dialog->getText());     // explicit conversion (from multi-byte string)
    const auto wtext = CString{dialog->getWText().c_str()}; // implicit conversion (from wide string)
    message.Format(_T("Dialog accepted.\n\ngetText(): %s\ngetWText(): %s\ngetNumber(): %d"), text.GetString(),
                   wtext.GetString(), dialog->getNumber());
    MessageBox(message);
  } else {
    MessageBox(_T("Dialog rejected."));
  }
}

void CMainFrame::ShowModalDialog()
{
  // Prevent launching the dialog twice
  if (m_sample_dll_dialog) { return; }

  m_sample_dll_dialog = std::make_unique<QSampleDLL>("Modal");

  ShowDialog(m_sample_dll_dialog.get(), this);

  m_sample_dll_dialog.reset();
}

void CMainFrame::ShowNonModalDialog()
{
  QSampleDLL dummy{"Non-modal"};

  ShowDialog(&dummy, nullptr);
}

void CMainFrame::ShowUnprotectedMessageBox()
{
  QSampleDLL dummy{"Unprotected"};

  configureDialog(&dummy);

  dummy.showMessageBox(nullptr);
}

void CMainFrame::ShowProtectedMessageBox()
{
  // Prevent launching the dialog twice
  if (m_sample_dll_dialog) { return; }

  m_sample_dll_dialog = std::make_unique<QSampleDLL>("Modal");

  configureDialog(m_sample_dll_dialog.get());

  m_sample_dll_dialog->showMessageBox(this);

  m_sample_dll_dialog.reset();
}
