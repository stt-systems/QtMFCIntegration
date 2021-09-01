// +
#include "QtModal_MFC.h"
#include "targetver.h"
#include "EventsShieldDialog.h"
#include <vector>

namespace
{
  UINT s_resource_id{0};                          ///< Resource ID associated with the shield dialog in the application.
  CWnd *s_parent{nullptr};                        ///< Parent window.
  QtModalActivateCallback s_on_activate{nullptr}; ///< Callback invoked each time a Qt dialog has to be activated.

  /**
   Helper struct for preserving the previous state of the accelerator table.
   */
  struct AcceleratorsBackup
  {
    std::vector<ACCEL> accelerators;

    /**
     Backups current accelerator table and delete original, effectively disabling all keyboard shortcuts of main app.
     */
    void copyAndReset()
    {
      HACCEL hAccelerators = ((CFrameWnd *)(AfxGetApp()->m_pMainWnd))->m_hAccelTable;
      accelerators.resize(CopyAcceleratorTable(hAccelerators, nullptr, 0));
      VERIFY(CopyAcceleratorTable(hAccelerators, accelerators.data(), (int)accelerators.size()) ==
             (int)accelerators.size());
      DestroyAcceleratorTable(hAccelerators);
    }

    /**
     Restores accelerator table.
     */
    void restore()
    {
      ((CFrameWnd *)(AfxGetApp()->m_pMainWnd))->m_hAccelTable =
        CreateAcceleratorTable(accelerators.data(), (int)accelerators.size());
      accelerators.clear();
    }
  } s_accelerators_backup;
}

void SetUpQtModalInMFC(unsigned int resource_id, CWnd *parent, const QtModalActivateCallback &on_activate)
{
  // This function eases the launch of the shield by caching common values, avoiding the method showing the Qt dialog
  // the need to specify all of these parameters every time
  s_resource_id = resource_id;
  s_parent = parent;
  s_on_activate = on_activate;
}

void BeginQtModalInMFC(int dialog_id)
{
  s_accelerators_backup.copyAndReset();

  EventsShieldDialog::DeployShield(s_resource_id, s_parent, s_on_activate, dialog_id);
}

void EndQtModalInMFC()
{
  EventsShieldDialog::FoldShield();

  s_accelerators_backup.restore();
}
