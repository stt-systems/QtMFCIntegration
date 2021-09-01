#pragma once

#ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
#endif

#define WIN32_LEAN_AND_MEAN
#include <winsdkver.h>
#include <sdkddkver.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#  include <afxole.h>   // MFC OLE classes
#  include <afxodlgs.h> // MFC OLE dialog classes
#  include <afxdisp.h>  // MFC Automation classes
#endif                  // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#  include <afxcmn.h> // MFC support for Windows Common Controls
#endif                // _AFX_NO_AFXCMN_SUPPORT

#include <afxdialogex.h>
