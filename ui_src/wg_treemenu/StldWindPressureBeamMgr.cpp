// CMWindItemCodeMgr.cpp: implementation of the CCMWindPressureDlgAreaMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StldWindPressureBeamMgr.h"

#include "StldWindPressureBeamDlg.h"
#include "StldWindPressureAreaDlg_CH2019.h"
#include "StldWindPressureAreaDlg_CH2012.h"
#include "StldWindPressureAreaDlg_CH2001.h"
#include "StldWindPressureAreaDlg_KBC2009.h"
#include "StldWindPressureAreaDlg_KBC2016.h"
#include "StldWindPressureAreaDlg_KDS2021.h"
#include "StldWindPressureAreaDlg_ASCE7_16.h"
#include "StldWindPressureAreaDlg_IS875_2015.h"

// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CStldWindPressureBeamMgr::CStldWindPressureBeamMgr(CWnd* pParent, void* pData)
	:CCMWindPressureBaseMgr(pParent)
{  
	CreateCodeDataBefo();
	CreateCodeData(pData);
	if(!CreateCodeDataNext())
		ASSERT(0);
}

CStldWindPressureBeamMgr::~CStldWindPressureBeamMgr()
{
}

void CStldWindPressureBeamMgr::CreateCodeData( void* pData )
{
	if(m_aCodeDlg.GetSize() != EN_WIND_PRESSURE_CODE_TYPE_NUMBER)
		return;

	auto L_SetCodeData = [this](const EN_WIND_PRESSURE_CODE_TYPE& enCode, CCMWindPressureBaseChildDlg* pDlg)
	{
		int nCode = GetDBCodeIdx(enCode);
		m_aCodeName[enCode] = GetCodeStrByDBCode(nCode);
		m_aCodeDlg[enCode] = pDlg;
		m_aCodeType[enCode] = nCode;
	};

	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KDS2022, new CStldWindPressureAreaDlg_KDS2021(KDS_W_2022, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KDS2019, new CStldWindPressureAreaDlg_KBC2016(KDS_W_2019, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KBC2016, new CStldWindPressureAreaDlg_KBC2016(KBC_W_2016, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_KBC2009, new CStldWindPressureAreaDlg_KBC2009(m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2012, new CStldWindPressureAreaDlg_CH2012(m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2001, new CStldWindPressureAreaDlg_CH2001(m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_CHINA2019, new CStldWindPressureAreaDlg_CH2019(m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016, new CStldWindPressureAreaDlg_ASCE7_16(ASCE7_W_2016, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022, new CStldWindPressureAreaDlg_ASCE7_16(ASCE7_W_2022, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024, new CStldWindPressureAreaDlg_ASCE7_16(NSCP_W_2024, m_pParent));
	L_SetCodeData(EN_WIND_PRESSURE_CODE_TYPE_IS875_2015, new CStldWindPressureAreaDlg_IS875_2015(IS_W_875_2015, m_pParent));

	SetMemberData(pData);
	for(int i = 0 ; i < EN_WIND_PRESSURE_CODE_TYPE_NUMBER; i++)
	{
		m_aCodeDlg[i]->SetMemberData(pData);
	}
}