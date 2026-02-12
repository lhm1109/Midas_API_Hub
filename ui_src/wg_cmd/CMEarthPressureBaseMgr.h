// CMEarthPressureBaseMgr.h: interface for the CCMEarthPressureBaseMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CM_EARTH_BASE_MGR_H__)
#define __CM_EARTH_BASE_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

class CChildDialog;
class CCMEarthPressureBaseDlg;
class CCMEarthPressureDlgMover;
class CCMEarthPressureBaseChildDlg;

class __MY_EXT_CLASS__ CCMEarthPressureBaseMgr
{
public:
	CCMEarthPressureBaseMgr(CWnd* pParent);
	virtual ~CCMEarthPressureBaseMgr();

	void InitAfterEnd();
		
	// Operation
protected:	
	virtual void CreateCodeData(void* pData) { ASSERT(0); };  // 생성자에서 생성 
	virtual void DeleteCodeData();      // 소멸자에서 소멸 
	// Member Var.
protected:
	CCMEarthPressureBaseDlg*  m_pParent;
	CCMEarthPressureDlgMover* m_pDlgMover;

	int  m_nInitCodeIndex;
	int  m_nCurCodeIndex;
	BOOL m_bCobxMsgChecker;

	CStringArray      m_aCodeName;
	CArray<int, int> m_aCodeType;
	CArray<CCMEarthPressureBaseChildDlg*, CCMEarthPressureBaseChildDlg*> m_aCodeDlg;
};

#include "HeaderPost.h"      /////////
#endif
