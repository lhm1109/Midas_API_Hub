// CMWindItemCodeMgr.cpp: implementation of the CCMWindPressureVelocityCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureVelocityCodeMgr.h"

#include "CMWindPressureVelocityAddMod.h"
#include "CMWindPressureDlgMover.h"
#include "CMWindPressureVelo_KDS2021.h"
#include "CMWindPressureVelo_KBC2016.h"
#include "CMWindPressureVelo_KBC2009.h"
#include "CMWindPressureVelo_CH2012.h"
#include "CMWindPressureVelo_CH2001.h"
#include "CMWindPressureVelo_CH2019.h"
#include "CMWindPressureVelo_ASCE7_16.h"
// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "CMWindPressureVelo_IS875_2015.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//BOOL CLoadWindChecker::CheckAndErrorMsg(int nCode, CString& rcsErrMsg) 참고하자....

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMWindPressureVelocityCodeMgr::CCMWindPressureVelocityCodeMgr(CWnd* pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;

	m_nCurCodeIndex  = -1;
	m_nInitCodeIndex = 0;

	CreateCodeData();
	
	m_pDlgMover = NULL;
	m_pDlgMover = new CCMWindPressureDlgMover((CWnd*)m_pParent);
	
	CArray<UINT, UINT> caCtrls;
	UINT uiHolderID;
	m_pParent->GetMoveCtrlHolderID(caCtrls, uiHolderID);
	m_pDlgMover->SetSubCtrls(caCtrls, uiHolderID);
}

CCMWindPressureVelocityCodeMgr::~CCMWindPressureVelocityCodeMgr()
{
	DeleteCodeData();

	if(m_pDlgMover)
		delete m_pDlgMover;
	m_pDlgMover = NULL;
}

//////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////

// [WindCode] 추가시 수정
int  CCMWindPressureVelocityCodeMgr::GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable/*=NULL*/)
{
	aCodeName.RemoveAll();
	aCodeIndex.RemoveAll();

	CArray<int, int> aCodeId;
	int i;
	int nSize = CWindLoadGeneratorCtrl::GetEnableWindCode(aCodeId, aCodeEnable);
	for(i = 0 ; i < nSize; i++)
	{
		aCodeIndex.Add(GetCodeIdxbyDBCode(aCodeId[i]));
	}

	CString csErrMsg(_T(""));
	for (i = 0; i < aCodeIndex.GetSize(); i++)
	{
		//if (!CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadWind, aWindSeq[i] + 1, csErrMsg)) continue;
		aCodeName.Add(m_aCodeName[aCodeIndex[i]]);
	}

	return aCodeName.GetSize();
}

void CCMWindPressureVelocityCodeMgr::ChangeCodeDlg(int nCodeIndex)
{
	HWND hWnd = NULL;
	hWnd = m_pParent->GetSafeHwnd();
	if (hWnd == NULL || !(::IsWindow(hWnd))) return;

	if(m_nCurCodeIndex == nCodeIndex) return;
		
	if(nCodeIndex >= m_aCodeDlg.GetSize()) 
	{
		ASSERT(0);
		return;
	}

	//원래 대화상자 종료.
	if(m_nCurCodeIndex >= 0 && m_aCodeDlg.GetSize() > m_nCurCodeIndex)
	{
// 		CodeDlg2Data(m_pParent->m_Data, (EN_WIND_PRESSURE_CODE_TYPE)m_nCurCodeIndex);// 공용체로 바꾸어서 대화창 데이터를 저장할 수 없음.
		m_aCodeDlg[m_nCurCodeIndex]->End();
	}

	CRect WRect;
	if(!m_pParent->GetHolderRect(&WRect))
		ASSERT(0);
	m_pParent->ScreenToClient(WRect);
	if(m_aCodeDlg[nCodeIndex]->CreateInit(m_pParent, WRect.left,WRect.top))
	{
		Data2CodeDlg(m_pParent->m_Data, (EN_WIND_PRESSURE_CODE_TYPE)nCodeIndex);
		if(!m_pDlgMover->Execute(m_aCodeDlg[nCodeIndex]))
			ASSERT(0);
	}
	else
	{
		ASSERT(0);
	}

	m_nCurCodeIndex = nCodeIndex;
}

void CCMWindPressureVelocityCodeMgr::Data2CodeDlg(T_WVEP_D& data, EN_WIND_PRESSURE_CODE_TYPE enType)
{
	BOOL bAll = (enType == EN_WIND_PRESSURE_CODE_TYPE_ALL);

	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KDS2022)
	{
		CCMWindPressureVelo_KDS2021* pWnd = (CCMWindPressureVelo_KDS2021*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KDS2022];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.KDS2021);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KDS2019)
	{
		CCMWindPressureVelo_KBC2016* pWnd = (CCMWindPressureVelo_KBC2016*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KDS2019];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.KDS2019);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KBC2016)
	{
		CCMWindPressureVelo_KBC2016* pWnd = (CCMWindPressureVelo_KBC2016*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KBC2016];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.KBC2016);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KBC2009)
	{
		CCMWindPressureVelo_KBC2009* pWnd = (CCMWindPressureVelo_KBC2009*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KBC2009];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.KBC2009);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_IS875_2015)
	{
		CCMWindPressureVelo_IS875_2015* pWnd = (CCMWindPressureVelo_IS875_2015*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_IS875_2015];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.IS875_2015);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2012)
	{
		CCMWindPressureVelo_CH2012* pWnd = (CCMWindPressureVelo_CH2012*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2012];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.CH2012);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2001)
	{
		CCMWindPressureVelo_CH2001* pWnd = (CCMWindPressureVelo_CH2001*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2001];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.CH2001);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2019)
	{
		CCMWindPressureVelo_CH2019* pWnd = (CCMWindPressureVelo_CH2019*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2019];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.CH2019);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.ASCE7_16);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.ASCE7_22);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetData2Dlg(data.CodeParam.NSCP2024);
	}
}

BOOL CCMWindPressureVelocityCodeMgr::CodeDlg2Data(T_WVEP_D& data, EN_WIND_PRESSURE_CODE_TYPE enType)
{
	BOOL bAll = (enType == EN_WIND_PRESSURE_CODE_TYPE_ALL);

	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KDS2022)
	{
		CCMWindPressureVelo_KDS2021* pWnd = (CCMWindPressureVelo_KDS2021*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KDS2022];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.KDS2021);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KDS2019)
	{
		CCMWindPressureVelo_KBC2016* pWnd = (CCMWindPressureVelo_KBC2016*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KDS2019];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.KDS2019);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KBC2016)
	{
		CCMWindPressureVelo_KBC2016* pWnd = (CCMWindPressureVelo_KBC2016*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KBC2016];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.KBC2016);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_KBC2009)
	{
		CCMWindPressureVelo_KBC2009* pWnd = (CCMWindPressureVelo_KBC2009*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KBC2009];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.KBC2009);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_IS875_2015)
	{
		CCMWindPressureVelo_IS875_2015* pWnd = (CCMWindPressureVelo_IS875_2015*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_IS875_2015];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.IS875_2015);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2012)
	{
		CCMWindPressureVelo_CH2012* pWnd = (CCMWindPressureVelo_CH2012*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2012];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.CH2012);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2019)
	{
		CCMWindPressureVelo_CH2019* pWnd = (CCMWindPressureVelo_CH2019*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2019];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.CH2019);
	}
	if(bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_CHINA2001)
	{
		CCMWindPressureVelo_CH2001* pWnd = (CCMWindPressureVelo_CH2001*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_CHINA2001];
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.CH2001);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.ASCE7_16);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.ASCE7_22);
	}
	if (bAll || enType == EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024)
	{
		CCMWindPressureVelo_ASCE7_16* pWnd = (CCMWindPressureVelo_ASCE7_16*)m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024];
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->SetDlg2Data(data.CodeParam.ASCE7_22);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////

// [WindCode] 추가시 수정
void CCMWindPressureVelocityCodeMgr::CreateCodeData()
{
	m_aCodeName .SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);
	m_aCodeDlg  .SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);
	m_aCodeType	.SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);

	//T_WVEP_D 참조
	m_aCodeName[EN_WIND_PRESSURE_CODE_TYPE_KDS2022] = _T("KDS(41-12:2022)");
	m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_KDS2022] = new CCMWindPressureVelo_KDS2021(m_pParent);
	m_aCodeType[EN_WIND_PRESSURE_CODE_TYPE_KDS2022] = KDS_W_2022;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_KDS2019] = _T("KDS(41-10-15:2019)");
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_KDS2019] = new CCMWindPressureVelo_KBC2016(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_KDS2019] = KDS_W_2019;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_KBC2016] = _T("KBC(2016)");
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_KBC2016] = new CCMWindPressureVelo_KBC2016(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_KBC2016] = KBC_W_2016;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_KBC2009] = _T("KBC(2009)");
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_KBC2009] = new CCMWindPressureVelo_KBC2009(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_KBC2009] = KBC_W_2009;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_CHINA2012] = _T("China(GB50009-2012)");
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_CHINA2012] = new CCMWindPressureVelo_CH2012(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_CHINA2012] = CH_W_2012;
	
	m_aCodeName[EN_WIND_PRESSURE_CODE_TYPE_IS875_2015] = _T("IS875(2015)");
	m_aCodeDlg[EN_WIND_PRESSURE_CODE_TYPE_IS875_2015] = new CCMWindPressureVelo_IS875_2015(m_pParent);
	m_aCodeType[EN_WIND_PRESSURE_CODE_TYPE_IS875_2015] = IS_W_875_2015;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_CHINA2001] = _T("China(GB50009-2001)");
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_CHINA2001] = new CCMWindPressureVelo_CH2001(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_CHINA2001] = CH_W_2002;

	m_aCodeName [EN_WIND_PRESSURE_CODE_TYPE_CHINA2019] = _T("China(GB55001-2021)"); //GB50009-19Change into GB55001-2021
	m_aCodeDlg  [EN_WIND_PRESSURE_CODE_TYPE_CHINA2019] = new CCMWindPressureVelo_CH2019(m_pParent);
	m_aCodeType	[EN_WIND_PRESSURE_CODE_TYPE_CHINA2019] = CH_W_2021;

	m_aCodeName[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016] = FULL_ASCE7_W_2016; //GB50009-19Change into GB55001-2021
	m_aCodeDlg [EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016] = new CCMWindPressureVelo_ASCE7_16(m_pParent);
	m_aCodeType[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016] = ASCE7_W_2016;

	m_aCodeName[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022] = FULL_ASCE7_W_2022; //GB50009-19Change into GB55001-2021
	m_aCodeDlg [EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022] = new CCMWindPressureVelo_ASCE7_16(m_pParent);
	m_aCodeType[EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022] = ASCE7_W_2022;
		
	m_aCodeName[EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024] = FULL_NSCP_W_2024;
	m_aCodeDlg [EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024] = new CCMWindPressureVelo_ASCE7_16(m_pParent);
	m_aCodeType[EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024] = NSCP_W_2024;
}

void CCMWindPressureVelocityCodeMgr::DeleteCodeData()
{
	int nCount = m_aCodeDlg.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_aCodeDlg[i];
	}

	m_aCodeName .RemoveAll();
	m_aCodeDlg  .RemoveAll();
	m_aCodeType	.RemoveAll();
}

EN_WIND_PRESSURE_CODE_TYPE CCMWindPressureVelocityCodeMgr::GetCodeIdxbyDBCode( int nCode )
{
	switch (nCode)
	{
	case KDS_W_2022:	return EN_WIND_PRESSURE_CODE_TYPE_KDS2022;	
	case KDS_W_2019:	return EN_WIND_PRESSURE_CODE_TYPE_KDS2019;	
	case KBC_W_2016:	return EN_WIND_PRESSURE_CODE_TYPE_KBC2016;	
	case KBC_W_2009:	return EN_WIND_PRESSURE_CODE_TYPE_KBC2009;	
	case IS_W_875_2015: return EN_WIND_PRESSURE_CODE_TYPE_IS875_2015;
	case CH_W_2012:		return EN_WIND_PRESSURE_CODE_TYPE_CHINA2012;
	case CH_W_2002:		return EN_WIND_PRESSURE_CODE_TYPE_CHINA2001;
	case CH_W_2021:		return EN_WIND_PRESSURE_CODE_TYPE_CHINA2019;
	case ASCE7_W_2016:	return EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016;
	case ASCE7_W_2022:	return EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022;
	case NSCP_W_2024:   return EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024;
	default: ASSERT(0); break;
	}
	
	return EN_WIND_PRESSURE_CODE_TYPE_ALL;  
}
int CCMWindPressureVelocityCodeMgr::GetDBCodebyCodeIdx(EN_WIND_PRESSURE_CODE_TYPE nIdx)
{
	if (nIdx >= EN_WIND_PRESSURE_CODE_TYPE_NUMBER || nIdx <= EN_WIND_PRESSURE_CODE_TYPE_ALL)
	{
		ASSERT(FALSE);
		return -1;
	}
	return m_aCodeType[nIdx];
}