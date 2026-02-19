// WriteLoadRatingResult.h: interface for the CCRCImportFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_WRITELOADRATINGRESULT_H__)
#define _WRITELOADRATINGRESULT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CWriteLoadRatingResult
{
public:
	CWriteLoadRatingResult();
	virtual ~CWriteLoadRatingResult();

	BOOL Execute_BridgeLoadRaitng();

	ArrElemPairKey m_arBlrdElemKList;
	CArray<UINT,UINT> m_arBlrdMembKList;
	CArray<UINT,UINT> m_arTndnElemKList;
	CArray<UINT,UINT> m_arStreRatingCaseKList;
	CArray<UINT,UINT> m_arServRatingCaseKList;

protected:
	CCRCDataCtrl m_DataCtrl;
	CCRCForceCtrl m_ForceCtrl;
	ArrElemPairKey m_ElemKList;
	CArray<UINT, UINT> m_MembKList;

	CDBDoc* m_pDoc;

	BOOL MakeRatingDesignData();
};

#endif // !defined(_WRITELOADRATINGRESULT_H__)
