// StldWindPressureNodalDlg_ASCE7_16.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_ASCE7_16.h"

#include "StldWindPressureNodalDlg.h"
#include "StldWindPressureAreaDlg_ASCE7_16_GustF.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_ASCE7_16 dialog

CStldWindPressureNodalDlg_ASCE7_16::CStldWindPressureNodalDlg_ASCE7_16(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_ASCE7_16::IDD, pParent)
{
	m_pData = NULL;
	m_bInAutoCalc = FALSE;
}

void CStldWindPressureNodalDlg_ASCE7_16::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_GROUND_FACTOR_EDIT, m_editGroundFactor);
	DDX_Control(pDX, IDC_TM_DIRECT_FACTOR_EDIT, m_editDirectFactor);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
}

BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_ASCE7_16, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_ASCE7_16 message handlers

BOOL CStldWindPressureNodalDlg_ASCE7_16::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editGroundFactor.SetUnitType(D_UNITSYS_NONE);
	m_editDirectFactor.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);

	
	if(m_pData)
	{

		Data2Dlg( (void*) m_pData);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureNodalDlg_ASCE7_16::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureNodalDlg_ASCE7_16::InitDefaultData()
{
}

BOOL CStldWindPressureNodalDlg_ASCE7_16::Dlg2Data( void* pData, BOOL bWarning )
{
	SetMemberData(pData);

	T_WDPR_ASCE7_16* pWdprCode = m_pData->GetASCE7Series();
	if (pWdprCode == NULL) { ASSERT(0); return FALSE; }
	pWdprCode->dKe = m_editGroundFactor.GetEditValue();
	pWdprCode->dKd = m_editDirectFactor.GetEditValue();
	pWdprCode->dGfExternal = m_editGustFactor_Ex.GetEditValue();
	
	return TRUE;
}

void CStldWindPressureNodalDlg_ASCE7_16::Data2Dlg(void* pData, BOOL bWarning)
{
	SetMemberData(pData);
	if (m_pData == NULL) { ASSERT(0); return; }

	T_WDPR_ASCE7_16* pWdprCode = m_pData->GetASCE7Series();
	m_editGroundFactor.SetEditUnit(pWdprCode->dKe, 4);
	m_editDirectFactor.SetEditUnit(pWdprCode->dKd, 4);

	m_editGustFactor_Ex.SetEditUnit(pWdprCode->dGfExternal, 4);
}


void CStldWindPressureNodalDlg_ASCE7_16::OnCmdGustFactor()
{
	m_bInAutoCalc = TRUE;
	m_pParent->Dlg2Data(FALSE);

	T_WVEP_K WvepK;
	if(!m_pParent->GetWvepKey(WvepK))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return;
	}

	T_WVEP_D WvepD;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
	{
		ASSERT(0);
		WvepD.Initialize();
	}

	T_WVEP_ASCE7_16* pWvepCode = WvepD.GetASCE7Series();
	CStldWindPressureAreaDlg_ASCE7_GustF dlg;
 	dlg.SetWindData(pWvepCode, m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgInfo(*m_pData->GetASCE7Series());
		Data2Dlg((void*)m_pData);

// 		m_pData->GetASCE7()->dGfExternal = dlg.GetCalcGustFactor();
// 		m_editGustFactor_Ex.SetEditUnit(m_pData->GetASCE7()->dGfExternal, 4);
	}
}


void CStldWindPressureNodalDlg_ASCE7_16::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CStldWindPressureNodalDlg_ASCE7_16::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_WVEP_MFD):
		case(UR_WVEP_LT_ADD):
		case(UR_WVEP_LT_DEL):
			{
				if(m_pData) Data2Dlg( (void*) m_pData);
			}
			break;
		default:
			break;
		}
	} // end of while
}