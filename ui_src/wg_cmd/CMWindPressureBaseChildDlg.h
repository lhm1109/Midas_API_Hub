#if !defined(__CM_WIND_BASE_CHILD_DLG_H__)
#define __CM_WIND_BASE_CHILD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CMChildBarBase.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureBaseDlg dialog
class CCMWindPressureBaseDlg;

class __MY_EXT_CLASS__ CCMWindPressureBaseChildDlg : public CChildDialog
{
public:
	CCMWindPressureBaseChildDlg(UINT nID, CWnd* pParent);
	virtual ~CCMWindPressureBaseChildDlg();
	
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE ) = 0;
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE ) = 0;
	virtual void SetMemberData( void* pData) = 0;

protected:
	CCMWindPressureBaseDlg* m_pParent;
};

#include "HeaderPost.h"      /////////
#endif
