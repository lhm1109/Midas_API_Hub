// CMWindPressureBaseMgr.h: interface for the CCMWindPressureBaseMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CM_WIND_BASE_MGR_H__)
#define __CM_WIND_BASE_MGR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

enum EN_WIND_PRESSURE_CODE_TYPE
{
	EN_WIND_PRESSURE_CODE_TYPE_ALL = -1,
	EN_WIND_PRESSURE_CODE_TYPE_KDS2022 = 0,
	EN_WIND_PRESSURE_CODE_TYPE_KDS2019,
	EN_WIND_PRESSURE_CODE_TYPE_KBC2016,
	EN_WIND_PRESSURE_CODE_TYPE_KBC2009,
	EN_WIND_PRESSURE_CODE_TYPE_IS875_2015,
	EN_WIND_PRESSURE_CODE_TYPE_CHINA2012,
	EN_WIND_PRESSURE_CODE_TYPE_CHINA2001,
	EN_WIND_PRESSURE_CODE_TYPE_CHINA2019,
	EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016,
	EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022,
	EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024,
	EN_WIND_PRESSURE_CODE_TYPE_NUMBER,
};

#include "HeaderPre.h"

class CChildDialog;
class CCMWindPressureBaseDlg;
class CCMWindPressureDlgMover;
class CCMWindPressureBaseChildDlg;

class __MY_EXT_CLASS__ CCMWindPressureBaseMgr
{
public:
	CCMWindPressureBaseMgr(CWnd* pParent);
	virtual ~CCMWindPressureBaseMgr();

	void InitAfterEnd();

	// Interface
public:
	virtual int  GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable=NULL);
	virtual void ChangeCodeDlg(int nCodeIndex);

	static CString GetCodeStrByDBCode(int nCode);
	static EN_WIND_PRESSURE_CODE_TYPE GetCodeIdxbyDBCode(int nCode);
	static int GetDBCodeIdx( EN_WIND_PRESSURE_CODE_TYPE enType );

	virtual BOOL CodeDlg2Data( void* pData, EN_WIND_PRESSURE_CODE_TYPE enType, BOOL bWarning = FALSE );
	virtual void Data2CodeDlg( void* pData, EN_WIND_PRESSURE_CODE_TYPE enType, BOOL bWarning = FALSE );

	void SetMemberData(void* pData) { m_pData = pData; }
	void SetInit(BOOL bSet) { m_bInit = bSet; }
	// Operation
protected:
	void CreateCodeDataBefo();
	BOOL CreateCodeDataNext();
	virtual void CreateCodeData( void* pData ) { ASSERT(0); };  // 생성자에서 생성 
	virtual void DeleteCodeData();      // 소멸자에서 소멸 
	// Member Var.
protected:
	CCMWindPressureBaseDlg*  m_pParent;
	CCMWindPressureDlgMover* m_pDlgMover;
	void* m_pData;

	int  m_nInitCodeIndex;
	int  m_nCurCodeIndex;
	BOOL m_bCobxMsgChecker;
	BOOL m_bInit;

	CStringArray      m_aCodeName;
	CArray<int , int> m_aCodeType;
	CArray<CCMWindPressureBaseChildDlg*, CCMWindPressureBaseChildDlg*> m_aCodeDlg;
};

#include "HeaderPost.h"      /////////
#endif
