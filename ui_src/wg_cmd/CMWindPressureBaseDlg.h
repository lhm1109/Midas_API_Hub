#if !defined(__CM_WIND_BASE_DLG_H__)
#define __CM_WIND_BASE_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"
#include "CMChildBarBase.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureBaseDlg dialog
class CCMWindPressureBaseMgr;

class __MY_EXT_CLASS__ CCMWindPressureBaseDlg : public CCMChildBarBase
{
public:
	CCMWindPressureBaseDlg(UINT nID);
	virtual ~CCMWindPressureBaseDlg();

	// Pure Virtual
	virtual BOOL Dlg2Data(BOOL bWarning = TRUE) = 0;
	virtual BOOL Data2Dlg(BOOL bWarning = TRUE) = 0;
	virtual void GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID) = 0;
	virtual BOOL GetHolderRect( CRect* pRect ) = 0;

	// Virtual
	virtual void  End();
	virtual CWnd* GetBaseDlg() { return NULL; }
	virtual BOOL  GetWvepKey(T_WVEP_K& WvepK) { ASSERT(0); WvepK = 0; return FALSE; }

protected:
	CCMWindPressureBaseMgr* m_pMgr;
	CArray<BOOL, BOOL> m_aWlCodeEnable;
	int m_nOldSel;
};

#include "HeaderPost.h"      /////////
#endif
