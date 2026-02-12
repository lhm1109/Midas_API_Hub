// CStldWindPressureAreaDlg_ASCE7_16.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_ASCE7_16.h"

#include "StldWindPressureAreaDlg.h"
#include "StldWindPressureAreaDlg_ASCE7_16_GustF.h"
#include "StldWindPressureAreaDlg_KBC2016_FrcCoefAuto.h"
#include "StldWindPressureVibrationDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_ASCE7_16 dialog

CStldWindPressureAreaDlg_ASCE7_16::CStldWindPressureAreaDlg_ASCE7_16(int nCodeType, CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureAreaDlg_ASCE7_16::IDD, pParent)
{
	m_pData = NULL;
	m_nCodeType = nCodeType;
	m_bInAutoCalc = FALSE;

	
}

void CStldWindPressureAreaDlg_ASCE7_16::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_GROUND_FACTOR_EDIT, m_editGroundFactor);
	DDX_Control(pDX, IDC_TM_DIRECT_FACTOR_EDIT, m_editDirectFactor);

	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_IN,     m_editGustFactor_In);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_EX,     m_editCoefFactor_Ex);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_IN,     m_editCoefFactor_In);

	DDX_Control(pDX, IDC_TM_FORCE_COEF_AUTO_CMB,				m_cmbStructureType);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_ASCE7_16, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
	ON_BN_CLICKED(IDC_TM_FORCE_COEF_AUTO_BTN,   OnCmdCoefFactor)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_FORCECOEF, OnChkAutoCalcForceCoef)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_ASCE7_16 message handlers

BOOL CStldWindPressureAreaDlg_ASCE7_16::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editGroundFactor.SetUnitType(D_UNITSYS_NONE);
	m_editDirectFactor.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editGustFactor_In.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_In.SetUnitType(D_UNITSYS_NONE);

	InitStructureTypeCombo();


	if(m_pData)
	{
		m_pData->nCodeType = m_nCodeType;
		if(m_pData->GetASCE7Series() == NULL) { ASSERT(0); return FALSE; }

		Data2Dlg( (void*) m_pData);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureAreaDlg_ASCE7_16::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureAreaDlg_ASCE7_16::InitDefaultData()
{
}

void CStldWindPressureAreaDlg_ASCE7_16::InitStructureTypeCombo()
{
	m_cmbStructureType.ResetContent();
	int nIndex = 0;
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CIRCLE_PLAN);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_LATTICE_TOWERS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_SINGLEPLANE)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_SOLID_FREESTANDING)); m_cmbStructureType.SetItemData(nIndex, ENM_T_SOLID_FREE);
	m_cmbStructureType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStructureType);
}

BOOL CStldWindPressureAreaDlg_ASCE7_16::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;

	T_WDPR_ASCE7_16* pWdprCode = m_pData->GetASCE7Series();
	if(pWdprCode == NULL) { ASSERT(0); return FALSE; }
	pWdprCode->dKe = m_editGroundFactor.GetEditValue();
	pWdprCode->dKd = m_editDirectFactor.GetEditValue();

	pWdprCode->dGfExternal = m_editGustFactor_Ex.GetEditValue();
	pWdprCode->dGfInternal = m_editGustFactor_In.GetEditValue();
	pWdprCode->dCfExternal = m_editCoefFactor_Ex.GetEditValue();
	pWdprCode->dCfInternal = m_editCoefFactor_In.GetEditValue();
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	if(pBtn->GetCheck()) pWdprCode->bAutoForceCoef = TRUE;
	else                 pWdprCode->bAutoForceCoef = FALSE;

	if(pWdprCode->bAutoForceCoef)
	{
		if (m_pData->arKBC2009ForceCoef.GetSize() <= 1 && m_bInAutoCalc == FALSE)
		{
			if (bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_KBC2009_NOAUTOCALCINFO));
			return FALSE;
		}
	}

	pWdprCode->nAutoForceCoefStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	return TRUE;
}

void CStldWindPressureAreaDlg_ASCE7_16::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	T_WDPR_ASCE7_16* pWdprCode = m_pData->GetASCE7Series();
	if(pWdprCode == NULL) { ASSERT(0); return; }

	T_WVEP_K WvepK;
	if(m_pParent->GetWvepKey(WvepK))
	{
		T_WVEP_D WvepD;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if(!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
		{
			ASSERT(0);
			WvepD.Initialize();
		}

		if(WvepD.GetASCE7Series() == NULL) { ASSERT(0); return; }
	}

	m_editGroundFactor.SetEditUnit(pWdprCode->dKe, 4);
	m_editDirectFactor.SetEditUnit(pWdprCode->dKd, 4);
	
	m_editGustFactor_Ex.SetEditUnit(pWdprCode->dGfExternal, 4);
	m_editGustFactor_In.SetEditUnit(pWdprCode->dGfInternal, 4);
	m_editCoefFactor_Ex.SetEditUnit(pWdprCode->dCfExternal, 4);
	m_editCoefFactor_In.SetEditUnit(pWdprCode->dCfInternal, 4);
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	pBtn->SetCheck(pWdprCode->bAutoForceCoef);


	OnChkAutoCalcForceCoef();

	int nStructType = m_cmbStructureType.GetCount();
	for(int i=0; i<nStructType; i++)
	{
		if(m_cmbStructureType.GetItemData(i) != pWdprCode->nAutoForceCoefStructType) continue;
		m_cmbStructureType.SetCurSel(i);
		break;
	}

}

void CStldWindPressureAreaDlg_ASCE7_16::OnCmdGustFactor()
{
	T_WDPR_ASCE7_16* pWdprCode = m_pData->GetASCE7Series();
	if(pWdprCode == NULL) { ASSERT(0); return; }
	m_bInAutoCalc = TRUE;
	if(!m_pParent->Dlg2Data())
	{
		m_bInAutoCalc = FALSE;
		return;
	}
	m_bInAutoCalc = FALSE;
	
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
	if(pWvepCode == NULL) { ASSERT(0); return; }
	
	CStldWindPressureAreaDlg_ASCE7_GustF dlg;
 	dlg.SetWindData(pWvepCode, m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgInfo(*pWdprCode);
		Data2Dlg((void*)m_pData);
		//m_pData->GetASCE7()->dGfExternal =  dlg.GetCalcGustFactor();
		//m_editGustFactor_Ex.SetEditUnit(m_pData->GetASCE7()->dGfExternal, 4);
	}
}

void CStldWindPressureAreaDlg_ASCE7_16::OnCmdCoefFactor()
{
	m_bInAutoCalc = TRUE;
	if(!m_pParent->Dlg2Data())
	{
		m_bInAutoCalc = FALSE;
		return;
	}
	m_bInAutoCalc = FALSE;

	T_WVEP_K WvepK=NULL;
	if(!m_pParent->GetWvepKey(WvepK))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return;
	}

	int nStructCmbType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());
	CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto dlg(NULL,nStructCmbType,WvepK);
	dlg.SetData(m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_pData);
	}
}

void CStldWindPressureAreaDlg_ASCE7_16::OnChkAutoCalcForceCoef()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	int nCheck = pBtn->GetCheck();

	GetDlgItem(IDC_TM_FORCE_COEF_AUTO_BTN)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_TM_FORCE_COEF_AUTO_CMB)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_EX)->EnableWindow(nCheck==0);
	GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_IN)->EnableWindow(nCheck==0);




}


void CStldWindPressureAreaDlg_ASCE7_16::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CStldWindPressureAreaDlg_ASCE7_16::UpdateBuffer()
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