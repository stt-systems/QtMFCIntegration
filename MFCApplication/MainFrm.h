#pragma once

#include "targetver.h"
#include <afxframewndex.h>
#include "../QSampleDLL/QSampleDLL.h"

class CMainFrame : public CMDIFrameWnd, public QSampleDLL::Delegate
{
  DECLARE_DYNAMIC(CMainFrame)
public:
  CMainFrame() noexcept;
  virtual ~CMainFrame();

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT &cs);

protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()

public: // methods to show different Qt-MFC integration examples
  void ShowModalDialog();
  void ShowNonModalDialog();

  void ShowUnprotectedMessageBox();
  void ShowProtectedMessageBox();

private:
  /**
   Just a helper function to ease the launch of the Qt dialog under different conditions.
   */
  void ShowDialog(QSampleDLL *dialog, QSampleDLL::Delegate *delegate);

public: // Qt/MFC support
  // Each Qt dialog launched from MFC must have an unique ID so it is possible to communicate with the correct dialog
  // every time is required
  enum QT_DIALOG_ID {
    QT_DIALOG_SAMPLE_DLL,
    QT_DIALOG_TOTAL_IDS,
  };

  void SetupQtSupport();

  // Helper methods to ease delegates definition
  void QtBeginModal(int id);
  void QtEndModal();

  // Switch back to Qt dialog every time the event's shield is activated
  void SetFocusToQtDialog(int id);

private: // QSampleDLL::Delegate
  void myDialogWillBeShown() override { QtBeginModal(QT_DIALOG_SAMPLE_DLL); }
  void myDialogHasBeenClosed() override { QtEndModal(); }

private:
  // In order to transfer the focus to Qt-based dialogs, they must be accessible within the whole the class
  std::unique_ptr<QSampleDLL> m_sample_dll_dialog;
};
