// CMWindItemCodeMgr.cpp: implementation of the CCMWindPressureDlgAreaMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StldWindPressureNodalMgr.h"

#include "StldWindPressureNodalDlg_CH2019.h"
#include "StldWindPressureNodalDlg_CH2012.h"
#include "StldWindPressureNodalDlg_CH2001.h"
#include "StldWindPressureNodalDlg_KBC2009.h"
#include "StldWindPressureNodalDlg_KBC2016.h"
#include "StldWindPressureNodalDlg_KDS2021.h"
#include "StldWindPressureNodalDlg_ASCE7_16.h"
#include "StldWindPressureNodalDlg_IS875_2015.h"
// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CStldWindPressureNodalMgr::CStldWindPressureNodalMgr(CWnd* pParent, void* pData)
	:CCMWindPressureBaseMgr(pParent)
{  
	CreateCodeDataBefo();
	CreateCodeData(pData);
	if(!CreateCodeDataNext())
		ASSERT(0);
}

CStldWindPressureNodalMgr::~CStldWindPressureNodalMgr()
{
}

void CStldWindPressureNodalMgr::CreateCodeData( void* pData )
{
	if(m_aCodeDlg.GetSize() != EN_WIND_PRESSURE_CODE_TYPE_NUMBER)
		return;

	auto L_SetCodeData = [this](const EN_WIND_PRESSURE_CODE_TYPE& enCode, CCMWindPressureBaseChildDlg* pDlg)
	{
		int nCode = GetDBCodeIdx(enCode);
		m_aCodeName[enCode] = GetCodeStrByDBCode(nCode);
		m_aCodeDlg [enCode] = pDlg;
		m_aCodeType[enCode] = nCode;
	};

	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KDS2022, new CStldWindPressureNodalDlg_KDS2021((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KDS2019, new CStldWindPressureNodalDlg_KBC2016((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KBC2016, new CStldWindPressureNodalDlg_KBC2016((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KBC2009, new CStldWindPressureNodalDlg_KBC2009((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2012, new CStldWindPressureNodalDlg_CH2012((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2001, new CStldWindPressureNodalDlg_CH2001((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2019, new CStldWindPressureNodalDlg_CH2019((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016, new CStldWindPressureNodalDlg_ASCE7_16((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022, new CStldWindPressureNodalDlg_ASCE7_16((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024, new CStldWindPressureNodalDlg_ASCE7_16((CWnd*)m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_IS875_2015, new CStldWindPressureNodalDlg_IS875_2015((CWnd*)m_pParent));

	SetMemberData(pData);
	for(int i = 0 ; i < EN_WIND_PRESSURE_CODE_TYPE_NUMBER; i++)
	{
		m_aCodeDlg[i]->SetMemberData(pData);
	}
}