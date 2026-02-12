// CStldWindPressureAreaDlg_KDS2021.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KDS2021.h"

#include "StldWindPressureAreaDlg.h"
#include "StldWindPressureAreaDlg_KDS2021_GustF.h"
#include "StldWindPressureAreaDlg_KBC2016_FrcCoefAuto.h"
#include "StldWindPressureVibrationDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KDS2021 dialog

CStldWindPressureAreaDlg_KDS2021::CStldWindPressureAreaDlg_KDS2021(int nCodeType, CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureAreaDlg_KDS2021::IDD, pParent)
{
	m_pData = NULL;
	m_nCodeType = nCodeType;
	m_bInAutoCalc = FALSE;

	m_aCtrlMethod.RemoveAll();
	m_aCtrlMethod.Add(IDC_WG_TREEMENU_METHOD_RDO1);
	m_aCtrlMethod.Add(IDC_WG_TREEMENU_METHOD_RDO2);
	m_aCtrlMethod.FreeExtra();

	m_aCtrlBuildingType.RemoveAll();
	m_aCtrlBuildingType.Add(IDC_WG_TREEMENU_BLDTYPE_RDO1);
	m_aCtrlBuildingType.Add(IDC_WG_TREEMENU_BLDTYPE_RDO2);
	m_aCtrlBuildingType.FreeExtra();

	m_aCtrlBuilding.RemoveAll();
	m_aCtrlBuilding.Add(IDC_WG_TREEMENU_DIR_ACROSS_RDO);
	m_aCtrlBuilding.Add(IDC_WG_TREEMENU_DIR_TORSIONAL_RDO);
	m_aCtrlBuilding.FreeExtra();

	m_aCtrlTorsional_Sub.RemoveAll();
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_STC);
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_EDT);
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_UNIT);
	m_aCtrlTorsional_Sub.FreeExtra();
}

void CStldWindPressureAreaDlg_KDS2021::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDP_VORTEX_SHEDDING_CHK,  m_chkVortex);
	DDX_Control(pDX, IDC_TM_DIRECT_FACTOR_EDIT,		 m_editDirectFactor);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_IN,     m_editGustFactor_In);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_EX,     m_editCoefFactor_Ex);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_IN,     m_editCoefFactor_In);

	DDX_Control(pDX, IDC_TM_FORCE_COEF_AUTO_CMB,				m_cmbStructureType);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_ALONG_RDO,			m_rdoAlongWind);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_ACROSS_RDO,		m_rdoAcrossWind);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO,	m_rdoTorsionalWind);

	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_POINT_EDT,  m_wndCoordXYZ);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_POINT_UNIT,  m_wndCoordUnit);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KDS2021, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
	ON_BN_CLICKED(IDC_TM_FORCE_COEF_AUTO_BTN,   OnCmdCoefFactor)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_FORCECOEF, OnChkAutoCalcForceCoef)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_BTN,			OnCmdVibrationBtn)
	ON_BN_CLICKED(IDC_WG_TREEMENU_BLDTYPE_RDO1, OnCmdBuildingTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_BLDTYPE_RDO2, OnCmdBuildingTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_ALONG_RDO			, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_ACROSS_RDO		, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_TORSIONAL_RDO	, OnCmdWindTypeCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KDS2021 message handlers

BOOL CStldWindPressureAreaDlg_KDS2021::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editDirectFactor.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editGustFactor_In.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_In.SetUnitType(D_UNITSYS_NONE);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, 1);

	InitStructureTypeCombo();

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndCoordXYZ.SetNextLink(NULL);
	
	if(m_pData)
	{
		m_pData->nCodeType = m_nCodeType;
		int    nStore = 0;
		double dHeight = 0.0;
		double dThick = 0.0;
		double dWidth = 0.0;
		if(CWindLoadGeneratorCtrl::GetModelHeightWidthThickTotal(m_pData->nDirection, m_pData->dAngle, dHeight, nStore, dWidth, dThick))
		{
			if(m_pData->CodeParam.KDS2021.dBLy<=0.0) m_pData->CodeParam.KDS2021.dBLy = dWidth;
			if(m_pData->CodeParam.KDS2021.dBLx<=0.0) m_pData->CodeParam.KDS2021.dBLx = dThick;
		}

		Data2Dlg( (void*) m_pData);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureAreaDlg_KDS2021::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureAreaDlg_KDS2021::InitDefaultData()
{
}

void CStldWindPressureAreaDlg_KDS2021::InitStructureTypeCombo()
{
	m_cmbStructureType.ResetContent();
	int nIndex = 0;
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CIRCLE_PLAN);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_LATTICE_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_LATTICE_TOWERS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_FENCES_ON_GROUND)); m_cmbStructureType.SetItemData(nIndex, ENM_T_FENCES_ON_GRND);
	m_cmbStructureType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStructureType);
}

BOOL CStldWindPressureAreaDlg_KDS2021::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;

    m_pData->CodeParam.KDS2021.dKd = m_editDirectFactor.GetEditValue();
    m_pData->CodeParam.KDS2021.dGfExternal = m_editGustFactor_Ex.GetEditValue();
	m_pData->CodeParam.KDS2021.dGfInternal = m_editGustFactor_In.GetEditValue();
	m_pData->CodeParam.KDS2021.dCfExternal = m_editCoefFactor_Ex.GetEditValue();
	m_pData->CodeParam.KDS2021.dCfInternal = m_editCoefFactor_In.GetEditValue();
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	if(pBtn->GetCheck()) m_pData->CodeParam.KDS2021.bAutoForceCoef = TRUE;
	else                 m_pData->CodeParam.KDS2021.bAutoForceCoef = FALSE;


    if ( m_pData->CodeParam.KDS2021.bAutoForceCoef )
    {
        if ( m_pData->arKBC2009ForceCoef.GetSize() <= 1 && m_bInAutoCalc == FALSE )
        {
            if ( bWarning ) AfxMessageBox(_LS(IDS_TM_WINDP_KBC2009_NOAUTOCALCINFO));
            return FALSE;
        }
    }
	
	m_pData->CodeParam.KDS2021.nAutoForceCoefStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, m_pData->CodeParam.KDS2021.nBuildingType);
	m_pData->CodeParam.KDS2021.bAlongWind = m_rdoAlongWind.GetCheck();
	m_pData->CodeParam.KDS2021.bAcrossWind = m_rdoAcrossWind.GetCheck();
	m_pData->CodeParam.KDS2021.bTorsionalWind = m_rdoTorsionalWind.GetCheck();

	if(m_pData->CodeParam.KDS2021.bTorsionalWind)
	{
		CString strPos;
		m_wndCoordXYZ.GetWindowText(strPos);
		MyParser Parser;
		if(!Parser.ParsingPositionArg(strPos)) return FALSE;
		if(!Parser.GetPositionData(m_pData->CodeParam.KDS2021.dCenterPnt[0],m_pData->CodeParam.KDS2021.dCenterPnt[1],m_pData->CodeParam.KDS2021.dCenterPnt[2])) return FALSE;
	}
	else
	{
		m_pData->CodeParam.KDS2021.dCenterPnt[0] = 0.;
		m_pData->CodeParam.KDS2021.dCenterPnt[1] = 0.;
		m_pData->CodeParam.KDS2021.dCenterPnt[2] = 0.;
	}

	return TRUE;
}

void CStldWindPressureAreaDlg_KDS2021::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	if(m_pData== NULL) { ASSERT(0); return; }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_WVEP_K WvepK = 0;
	T_WVEP_D WvepD;
	m_pParent->GetWvepKey(WvepK);
	if (!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
		WvepD.CodeParam.KDS2021.Initialize();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, WvepD.CodeParam.KDS2021.nMethod);
	m_chkVortex.SetCheck(WvepD.CodeParam.KDS2021.bVortexShedding);
	
	if(WvepD.CodeParam.KDS2021.nMethod==0) m_pData->CodeParam.KDS2021.bAutoForceCoef = FALSE;
	
	m_editDirectFactor.SetEditUnit(m_pData->CodeParam.KDS2021.dKd, 4);
	m_editGustFactor_Ex.SetEditUnit(m_pData->CodeParam.KDS2021.dGfExternal, 4);
	m_editGustFactor_In.SetEditUnit(m_pData->CodeParam.KDS2021.dGfInternal, 4);
	m_editCoefFactor_Ex.SetEditUnit(m_pData->CodeParam.KDS2021.dCfExternal, 4);
	m_editCoefFactor_In.SetEditUnit(m_pData->CodeParam.KDS2021.dCfInternal, 4);
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	pBtn->SetCheck(m_pData->CodeParam.KDS2021.bAutoForceCoef);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlBuildingType, m_pData->CodeParam.KDS2021.nBuildingType);
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO, m_pData->CodeParam.KDS2021.nBuildingType==1);
	m_rdoAlongWind.SetCheck(m_pData->CodeParam.KDS2021.bAlongWind);
	m_rdoAcrossWind.SetCheck(m_pData->CodeParam.KDS2021.bAcrossWind);
	m_rdoTorsionalWind.SetCheck(m_pData->CodeParam.KDS2021.bTorsionalWind);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTorsional_Sub, m_pData->CodeParam.KDS2021.bTorsionalWind);

	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_pData->CodeParam.KDS2021.dCenterPnt[0], m_pData->CodeParam.KDS2021.dCenterPnt[1], m_pData->CodeParam.KDS2021.dCenterPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);

	int nStructType = m_cmbStructureType.GetCount();
	for(int i=0; i<nStructType; i++)
	{
		if(m_cmbStructureType.GetItemData(i)!=m_pData->CodeParam.KDS2021.nAutoForceCoefStructType) continue;
		m_cmbStructureType.SetCurSel(i);
		break;
	}

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_WINDP_VORTEX_SHEDDING_CHK);
	int nVortex = pBtn2->GetCheck();
	int nMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);

	BOOL bEnableCtrl = nMethod == 1 && nVortex != 1 ? TRUE : FALSE;

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_STATIC0, bEnableCtrl, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_STATIC3, nVortex != 1, TRUE);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_STATIC10,  bEnableCtrl);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_DIRECT_FACTOR_EDIT, bEnableCtrl);
     
	CDlgUtil::CtrlEnableDisable(this, IDC_WINDP_AUTOCALC_FORCECOEF, bEnableCtrl);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_BLDTYPE_GRB, bEnableCtrl, TRUE);
	if(nMethod==0)
	{
		GetDlgItem(IDC_TM_FORCE_COEF_AUTO_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_FORCE_COEF_AUTO_CMB)->EnableWindow(FALSE);
		
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_DIR_GRB, FALSE, TRUE);

		m_rdoAlongWind.SetCheck(TRUE);
		m_rdoAcrossWind.SetCheck(FALSE);
		m_rdoTorsionalWind.SetCheck(FALSE);
	}
	GetDlgItem(IDC_WG_TREEMENU_DIR_ALONG_RDO)->EnableWindow(TRUE);
	OnChkAutoCalcForceCoef();
}

void CStldWindPressureAreaDlg_KDS2021::OnCmdGustFactor()
{
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
	
	
	CStldWindPressureAreaDlg_KDS2021_GustF dlg;
	dlg.SetWindData(WvepD.CodeParam.KDS2021, m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgInfo(m_pData->CodeParam.KDS2021);
		
		Data2Dlg( (void*) m_pData);
	}
}

void CStldWindPressureAreaDlg_KDS2021::OnCmdCoefFactor()
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

void CStldWindPressureAreaDlg_KDS2021::OnChkAutoCalcForceCoef()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_WINDP_VORTEX_SHEDDING_CHK);
	int nCheck = pBtn->GetCheck();
	int nVortex = pBtn2->GetCheck();

	GetDlgItem(IDC_TM_FORCE_COEF_AUTO_BTN)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_TM_FORCE_COEF_AUTO_CMB)->EnableWindow(nCheck==1);
	GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_EX)->EnableWindow(nCheck==0 && nVortex==0);
	GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_IN)->EnableWindow(nCheck==0 && nVortex==0);

	if(nCheck==1)
	{
		m_rdoAlongWind.SetCheck(TRUE);
		m_rdoAcrossWind.SetCheck(FALSE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBuildingType, nCheck==0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBuilding, nCheck==0);

	OnCmdWindTypeCheck();

	if(nCheck==0)
	{
		int nBuildingType = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, nBuildingType);
		CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO, nBuildingType==1);
	}
}

void CStldWindPressureAreaDlg_KDS2021::OnCmdVibrationBtn() 
{
	CStldWindPressureVibrationDlg dlg;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, dlg.m_nBuildingType);
	dlg.m_nAcrossCheck = m_rdoAcrossWind.GetCheck();
	dlg.m_nTorsionalCheck = m_rdoTorsionalWind.GetCheck();
	ASSERT(0);
// 	dlg.SetWindData(m_pData->GetKDS2021(), m_pData->nDirection, m_pData->dAngle);
	if (dlg.DoModal() == IDOK)
	{
		m_pData->CodeParam.KDS2021.dBLy = dlg.m_dBreadthX;
		m_pData->CodeParam.KDS2021.dBLx = dlg.m_dBreadthY;
		m_pData->CodeParam.KDS2021.dNoa = dlg.m_dFreqA;
		m_pData->CodeParam.KDS2021.dNot = dlg.m_dFreqT;
		m_pData->CodeParam.KDS2021.dZf = dlg.m_dZf;
	}
}

void CStldWindPressureAreaDlg_KDS2021::OnCmdBuildingTypeRadio()
{
	int nBuildingType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, nBuildingType);
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO, nBuildingType==1);

	if(nBuildingType==0 && m_rdoTorsionalWind.GetCheck())
	{
		m_rdoAlongWind.SetCheck(TRUE);
		m_rdoAcrossWind.SetCheck(FALSE);
		m_rdoTorsionalWind.SetCheck(FALSE);
	}
	OnCmdWindTypeCheck();
}

void CStldWindPressureAreaDlg_KDS2021::OnCmdWindTypeCheck()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	int nAutoCoefCheck = pBtn->GetCheck();
	int nBuildingType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, nBuildingType);
	int nAcrossCheck = m_rdoAcrossWind.GetCheck();
	int nTorsionalCheck = m_rdoTorsionalWind.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_BTN, nAutoCoefCheck==0 && ((nBuildingType==1&&nAcrossCheck == 1) || nTorsionalCheck == 1));

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTorsional_Sub, nTorsionalCheck==1);
	if(nTorsionalCheck==1)
	{
		m_wndCoordXYZ.SetFocus();
	}
}

void CStldWindPressureAreaDlg_KDS2021::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CStldWindPressureAreaDlg_KDS2021::UpdateBuffer()
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