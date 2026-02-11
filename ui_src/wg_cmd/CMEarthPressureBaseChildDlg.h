#if !defined(__CM_EARTH_BASE_CHILD_DLG_H__)
#define __CM_EARTH_BASE_CHILD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMChildBarBase.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMEarthPressureBaseDlg dialog
class CCMEarthPressureBaseDlg;

class __MY_EXT_CLASS__ CCMEarthPressureBaseChildDlg : public CChildDialog
{
public:
	CCMEarthPressureBaseChildDlg(UINT nID, CWnd* pParent);
	virtual ~CCMEarthPressureBaseChildDlg();

	virtual BOOL Dlg2Data(void* pData, BOOL bWarning = FALSE) = 0;
	virtual void Data2Dlg(void* pData, BOOL bWarning = FALSE) = 0;
	virtual void SetMemberData(void* pData) = 0;

protected:
	CCMEarthPressureBaseDlg* m_pParent;
};

#include "HeaderPost.h"      /////////
#endif
