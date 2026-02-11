// CMWindItemCodeMgr.cpp: implementation of the CCMWindPressureBaseMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureBaseMgr.h"
#include "CMWindPressureBaseDlg.h"
#include "CMWindPressureDlgMover.h"
#include "CMWindPressureBaseChildDlg.h"

// *^^* 코드 추가시 여기수정 <- LoadDgnCodeCheck 파일에서 Lock Check Option 수정/추가 할 것!!
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMWindPressureBaseMgr::CCMWindPressureBaseMgr(CWnd* pParent)
{
	m_bCobxMsgChecker = FALSE;
	m_pParent = NULL;
	m_pData = NULL;
	m_bInit = TRUE;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureBaseDlg*)pParent;

	m_nCurCodeIndex  = -1;
	m_nInitCodeIndex = 0;

	m_pDlgMover = NULL;
	m_pDlgMover = new CCMWindPressureDlgMover((CWnd*)m_pParent, m_pParent->GetBaseDlg());

	CArray<UINT, UINT> caCtrls;
	UINT uiHolderID;
	m_pParent->GetMoveCtrlHolderID(caCtrls, uiHolderID);
	m_pDlgMover->SetSubCtrls(caCtrls, uiHolderID);
}

CCMWindPressureBaseMgr::~CCMWindPressureBaseMgr()
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
int  CCMWindPressureBaseMgr::GetCodeNameList(CStringArray& aCodeName, CArray<int, int>& aCodeIndex, CArray<BOOL, BOOL>* aCodeEnable/*=NULL*/)
{
	CArray<int, int> aWindSeq;
	aWindSeq.RemoveAll();

	int i, nSize;

	nSize = CWindLoadGeneratorCtrl::GetEnableWindCode(aWindSeq, aCodeEnable);

	CString csErrMsg(_T(""));
	for (i = 0; i < nSize; i++)
	{
		//if (!CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadWind, aWindSeq[i] + 1, csErrMsg)) continue;
		aCodeName.Add(GetCodeStrByDBCode(aWindSeq[i]));
		aCodeIndex.Add(aWindSeq[i]);
	}

	return nSize;
}

void CCMWindPressureBaseMgr::ChangeCodeDlg(int nCodeIndex)
{
	HWND hWnd = NULL;
	hWnd = m_pParent->GetSafeHwnd();
	if (hWnd == NULL || !(::IsWindow(hWnd))) return;

	nCodeIndex = (int)GetCodeIdxbyDBCode(nCodeIndex);

	if(m_bInit == FALSE && m_nCurCodeIndex == nCodeIndex) return;
		
	if(nCodeIndex >= m_aCodeDlg.GetSize()) 
	{  
		ASSERT(0);
		return;
	}

	//원래 대화상자 종료.
	if(m_nCurCodeIndex >= 0 && m_aCodeDlg.GetSize() > m_nCurCodeIndex)
	{
	  if(m_aCodeDlg[m_nCurCodeIndex] && ::IsWindow(m_aCodeDlg[m_nCurCodeIndex]->GetSafeHwnd()))
		  m_aCodeDlg[m_nCurCodeIndex]->End();
	}

	CRect WRect;
	if(!m_pParent->GetHolderRect(&WRect))
		ASSERT(0);
	m_pParent->ScreenToClient(WRect);

	if(m_aCodeDlg[nCodeIndex]->CreateInit(m_pParent, WRect.left,WRect.top))
	{
		if(!m_pDlgMover->Execute(m_aCodeDlg[nCodeIndex]))
			ASSERT(0);
	}
	else
	{
		ASSERT(0);
	}

	m_nCurCodeIndex = nCodeIndex;
}

void CCMWindPressureBaseMgr::DeleteCodeData()
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

void CCMWindPressureBaseMgr::CreateCodeDataBefo()
{
	int i;
	int nSize = EN_WIND_PRESSURE_CODE_TYPE_NUMBER;
	
	m_aCodeName .SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);
	m_aCodeDlg  .SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);
	m_aCodeType	.SetSize(EN_WIND_PRESSURE_CODE_TYPE_NUMBER);
	
	for(i = 0 ; i < nSize; i++)
	{
		m_aCodeDlg[i] = NULL;
		m_aCodeType[i] = 0;
	}
}

BOOL CCMWindPressureBaseMgr::CreateCodeDataNext()
{ 
	int i;
	int nSize = EN_WIND_PRESSURE_CODE_TYPE_NUMBER;
	if(nSize != m_aCodeName.GetSize())  return FALSE;
	if(nSize != m_aCodeDlg.GetSize())   return FALSE;
	if(nSize != m_aCodeType.GetSize()) return FALSE;
	
	for(i = 0 ; i < nSize; i++)
	{
		if(!m_aCodeDlg[i])                            return FALSE;
	}  
	return TRUE;
}

int CCMWindPressureBaseMgr::GetDBCodeIdx( EN_WIND_PRESSURE_CODE_TYPE enType )
{
	switch (enType)
	{
	case EN_WIND_PRESSURE_CODE_TYPE_KDS2022: return KDS_W_2022;
	case EN_WIND_PRESSURE_CODE_TYPE_KDS2019:   return KDS_W_2019;
	case EN_WIND_PRESSURE_CODE_TYPE_KBC2016:   return KBC_W_2016;
	case EN_WIND_PRESSURE_CODE_TYPE_KBC2009:   return KBC_W_2009;
	case EN_WIND_PRESSURE_CODE_TYPE_IS875_2015: return IS_W_875_2015;
	case EN_WIND_PRESSURE_CODE_TYPE_CHINA2012: return CH_W_2012;
	case EN_WIND_PRESSURE_CODE_TYPE_CHINA2001: return CH_W_2002;
	case EN_WIND_PRESSURE_CODE_TYPE_CHINA2019: return CH_W_2021;
	case EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016: return ASCE7_W_2016;
	case EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022: return ASCE7_W_2022;
	case EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024:  return NSCP_W_2024;
	default: ASSERT(0); return 0;
	}
}

CString CCMWindPressureBaseMgr::GetCodeStrByDBCode( int nCode )
{
	CString strTemp;
	CWindLoadGeneratorCtrl::GetWindCodeNameByCode(nCode, strTemp);
	return strTemp;
}

EN_WIND_PRESSURE_CODE_TYPE CCMWindPressureBaseMgr::GetCodeIdxbyDBCode( int nCode )
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
	default: ASSERT(0); return EN_WIND_PRESSURE_CODE_TYPE_ALL;
	}
}

BOOL CCMWindPressureBaseMgr::CodeDlg2Data( void* pData, EN_WIND_PRESSURE_CODE_TYPE enType, BOOL bWarning /*= FALSE */ )
{
	BOOL bAll = (enType == EN_WIND_PRESSURE_CODE_TYPE_ALL);
	BOOL bRetVal = TRUE;

	CCMWindPressureBaseChildDlg* pChildWnd = nullptr;
	auto l_Dlg2DataByCode = [&](EN_WIND_PRESSURE_CODE_TYPE enCurType)
	{
		if (bAll || enType == enCurType)
		{
			pChildWnd = (CCMWindPressureBaseChildDlg*)m_aCodeDlg[enCurType];
			if (pChildWnd && IsWindow(pChildWnd->GetSafeHwnd()))
				bRetVal &= pChildWnd->Dlg2Data(pData, bWarning);
		}
	};	

	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_KDS2022);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_KDS2019);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_KBC2016);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_KBC2009);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_IS875_2015);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2019);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2012);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2001);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022);
	l_Dlg2DataByCode(EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024);
	
	return bRetVal;  
}

void CCMWindPressureBaseMgr::Data2CodeDlg( void* pData, EN_WIND_PRESSURE_CODE_TYPE enType, BOOL bWarning /*= FALSE */ )
{
	BOOL bAll = (enType == EN_WIND_PRESSURE_CODE_TYPE_ALL);

	CCMWindPressureBaseChildDlg* pChildWnd = nullptr;
	auto l_Data2DlgByCode = [&](EN_WIND_PRESSURE_CODE_TYPE enCurType)
	{
		if (bAll || enType == enCurType)
		{
			pChildWnd = (CCMWindPressureBaseChildDlg*)m_aCodeDlg[enCurType];
			if (pChildWnd && IsWindow(pChildWnd->GetSafeHwnd()))
				pChildWnd->Data2Dlg(pData, bWarning);
		}
	};

	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_KDS2022);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_KDS2019);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_KBC2016);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_KBC2009);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_IS875_2015);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2019);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2012);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_CHINA2001);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2016);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_ASCE7_2022);
	l_Data2DlgByCode(EN_WIND_PRESSURE_CODE_TYPE_NSCP_2024);
}

void CCMWindPressureBaseMgr::InitAfterEnd()
{
	m_nCurCodeIndex  = -1;
	m_nInitCodeIndex = 0;

	int i;
	int nSize = m_aCodeDlg.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		if(m_aCodeDlg[i])
		{
			m_aCodeDlg[i]->End();
		}
	}
}
