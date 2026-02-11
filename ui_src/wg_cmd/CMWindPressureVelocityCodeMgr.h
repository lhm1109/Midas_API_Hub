// CMWindItemCodeMgr.h: interface for the CCMWindPressureVelocityCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CM_WIND_VELO_CODE_MGR_H__)
#define __CM_WIND_VELO_CODE_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "CMWindPressureBaseMgr.h"

class CChildDialog;
class CCMWindPressureVelocityAddMod;
class CCMWindPressureDlgMover;

class CCMWindPressureVelocityCodeMgr  
{
public:
	CCMWindPressureVelocityCodeMgr(CWnd* pParent);
	virtual ~CCMWindPressureVelocityCodeMgr();

	// Interface
public:
	int  GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable=NULL);
	void ChangeCodeDlg(int nCodeIndex);

	void Data2CodeDlg(T_WVEP_D& data, EN_WIND_PRESSURE_CODE_TYPE enType);
	BOOL CodeDlg2Data(T_WVEP_D& data, EN_WIND_PRESSURE_CODE_TYPE enType);
	int  GetCurCodeIndex() { return m_nCurCodeIndex; }
	EN_WIND_PRESSURE_CODE_TYPE GetCodeIdxbyDBCode( int nCode );
	int GetDBCodebyCodeIdx(EN_WIND_PRESSURE_CODE_TYPE  nIdx);

	// Operation
protected:
	void CreateCodeData();    // 생성자에서 생성
	void DeleteCodeData();    // 소멸자에서 소멸

	// Member Var.
protected:
	CCMWindPressureVelocityAddMod* m_pParent;
	CCMWindPressureDlgMover*       m_pDlgMover;

	int  m_nInitCodeIndex;
	int  m_nCurCodeIndex;

	CStringArray                         m_aCodeName;
	CArray<CChildDialog*, CChildDialog*> m_aCodeDlg;
	CArray<int , int>                    m_aCodeType;
};

#endif
