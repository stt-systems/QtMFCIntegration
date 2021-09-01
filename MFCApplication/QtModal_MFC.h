#pragma once

#include <functional>

class CWnd;

/**
 Type for the callback to invoke each time a Qt dialog has to be brought to front. It receives the ID associated to the
 current Qt dialog.
 */
using QtModalActivateCallback = std::function<void(int)>;

/**
 Configures the Qt-MFC integration environment.
 @param resource_id Resource ID associated with the shield dialog in the application.
 @param parent Parent window.
 @param on_activate_callback Callback to be invoked each time a Qt dialog has to be brought to front.
 */
void SetUpQtModalInMFC(unsigned int resource_id, CWnd *parent, const QtModalActivateCallback &on_activate);

/**
 Prepares MFC to deal with a modal dialog in Qt.
 @param dialog_id The ID associated to the Qt dialog. It must be unique respect other Qt dialogs. It will be used to
 indicate the application which Qt dialog has to be activated by the shield.
 */
void BeginQtModalInMFC(int dialog_id);

/**
 Finishes the Qt modal mode.
 */
void EndQtModalInMFC();
