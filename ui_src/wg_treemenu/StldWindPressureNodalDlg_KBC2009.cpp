// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_KBC2009.h"

#include "StldWindPressureNodalDlg.h"
#include "StldWindPressureAreaDlg_KBC2009_GustF.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

// #include "..\wg_base\wg_base_MsgDll.h"
// 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_KBC2009 dialog

CStldWindPressureNodalDlg_KBC2009::CStldWindPressureNodalDlg_KBC2009(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_KBC2009::IDD, pParent)
{
	m_pData = NULL;
	m_bInAutoCalc = FALSE;
}

void CStldWindPressureNodalDlg_KBC2009::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
}


BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_KBC2009, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_KBC2009 message handlers

BOOL CStldWindPressureNodalDlg_KBC2009::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	
	if(m_pData)
		Data2Dlg( (void*) m_pData);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureNodalDlg_KBC2009::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureNodalDlg_KBC2009::InitDefaultData()
{
}

BOOL CStldWindPressureNodalDlg_KBC2009::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;

	m_pData->CodeParam.KBC2009.dGfExternal = m_editGustFactor_Ex.GetEditValue();

	return TRUE;
}

void CStldWindPressureNodalDlg_KBC2009::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	
	m_editGustFactor_Ex.SetEditUnit(m_pData->CodeParam.KBC2009.dGfExternal, 4);
}

void CStldWindPressureNodalDlg_KBC2009::OnCmdGustFactor()
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

	T_WVEP_KBC2009& KBC2009 = WvepD.CodeParam.KBC2009;

	CStldWindPressureAreaDlg_KBC2009_GustF dlg;
	dlg.SetWindData(KBC2009, m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgInfo(m_pData->CodeParam.KBC2009.nRigidity, m_pData->CodeParam.KBC2009.dBreath, m_pData->CodeParam.KBC2009.dNaturalFreq,
			m_pData->CodeParam.KBC2009.dDampingRatio, m_pData->CodeParam.KBC2009.dGfExternal);

		Data2Dlg( (void*) m_pData);
	}
}
