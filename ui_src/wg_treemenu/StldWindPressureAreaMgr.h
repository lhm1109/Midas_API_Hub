// CMWindItemCodeMgr.h: interface for the CCMWindPressureDlgAreaMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__TM_STLD_WIND_AREA_MGR_H__)
#define __TM_STLD_WIND_AREA_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_cmd\CMWindPressureBaseMgr.h"

class CStldWindPressureAreaMgr  : public CCMWindPressureBaseMgr
{
public:
	CStldWindPressureAreaMgr(CWnd* pParent, void* pData);
	virtual ~CStldWindPressureAreaMgr();
 
protected:
	virtual void CreateCodeData( void* pData );    // 积己磊俊辑 积己
};

#endif
