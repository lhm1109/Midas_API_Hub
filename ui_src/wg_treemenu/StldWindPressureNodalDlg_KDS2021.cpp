// CMWindItemKDS2021.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_KDS2021.h"

#include "StldWindPressureNodalDlg.h"
#include "StldWindPressureAreaDlg_KDS2021_GustF.h"
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
// CStldWindPressureNodalDlg_KDS2021 dialog

CStldWindPressureNodalDlg_KDS2021::CStldWindPressureNodalDlg_KDS2021(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_KDS2021::IDD, pParent)
{
	m_pData = NULL;
	m_bInAutoCalc = FALSE;

	m_aCtrlMethod.RemoveAll();
	m_aCtrlMethod.Add(IDC_WG_TREEMENU_METHOD_RDO1);
	m_aCtrlMethod.Add(IDC_WG_TREEMENU_METHOD_RDO2);
	m_aCtrlMethod.FreeExtra();

	m_aCtrlBuildingType.RemoveAll();
	m_aCtrlBuildingType.Add(IDC_WG_TREEMENU_BLDTYPE_RDO1);
	m_aCtrlBuildingType.Add(IDC_WG_TREEMENU_BLDTYPE_RDO2);
	m_aCtrlBuildingType.FreeExtra();

	m_aCtrlTorsional_Sub.RemoveAll();
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_STC);
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_EDT);
	m_aCtrlTorsional_Sub.Add(IDC_WG_TREEMENU_DIR_POINT_UNIT);
	m_aCtrlTorsional_Sub.FreeExtra();
}

void CStldWindPressureNodalDlg_KDS2021::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDP_VORTEX_SHEDDING_CHK, m_chkVortex);
	DDX_Control(pDX, IDC_TM_DIRECT_FACTOR_EDIT, m_editDirectFactor);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_ALONG_RDO,			m_rdoAlongWind);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_ACROSS_RDO,		m_rdoAcrossWind);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO,	m_rdoTorsionalWind);

	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_POINT_EDT,  m_wndCoordXYZ);
	DDX_Control(pDX, IDC_WG_TREEMENU_DIR_POINT_UNIT,  m_wndCoordUnit);
}

BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_KDS2021, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_BTN,			OnCmdVibrationBtn)
	ON_BN_CLICKED(IDC_WG_TREEMENU_BLDTYPE_RDO1, OnCmdBuildingTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_BLDTYPE_RDO2, OnCmdBuildingTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_ALONG_RDO			, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_ACROSS_RDO		, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_WG_TREEMENU_DIR_TORSIONAL_RDO	, OnCmdWindTypeCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_KDS2021 message handlers

BOOL CStldWindPressureNodalDlg_KDS2021::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editDirectFactor.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, 1);

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndCoordXYZ.SetNextLink(NULL);
	
	if(m_pData)
	{
		int    nStore;
		double dHeight;
		double dThick;
		double dWidth;
		if(CWindLoadGeneratorCtrl::GetModelHeightWidthThickTotal(m_pData->nDirection, m_pData->dAngle, dHeight, nStore, dWidth, dThick))
		{
			
			if (m_pData->CodeParam.KDS2021.dBLy <= 0.0) m_pData->CodeParam.KDS2021.dBLy = dWidth;
			if (m_pData->CodeParam.KDS2021.dBLx <= 0.0) m_pData->CodeParam.KDS2021.dBLx = dThick;

		}

		Data2Dlg( (void*) m_pData);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureNodalDlg_KDS2021::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureNodalDlg_KDS2021::InitDefaultData()
{
}

BOOL CStldWindPressureNodalDlg_KDS2021::Dlg2Data( void* pData, BOOL bWarning )
{
	SetMemberData(pData);

	m_pData->CodeParam.KDS2021.dKd = m_editDirectFactor.GetEditValue();
	m_pData->CodeParam.KDS2021.dGfExternal = m_editGustFactor_Ex.GetEditValue();
	
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

void CStldWindPressureNodalDlg_KDS2021::Data2Dlg( void* pData, BOOL bWarning )
{
	SetMemberData(pData);
	if (m_pData == NULL) { ASSERT(0); return; }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	T_WVEP_K WvepK = 0;
	T_WVEP_D WvepD;

	m_pParent->GetWvepKey(WvepK);
	if (!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
		WvepD.CodeParam.KDS2021.Initialize();

	T_WVEP_KDS2021* pWvepData = &WvepD.CodeParam.KDS2021;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, pWvepData->nMethod);
	m_chkVortex.SetCheck(WvepD.CodeParam.KDS2021.bVortexShedding);

	m_editDirectFactor.SetEditUnit(m_pData->CodeParam.KDS2021.dKd, 4);
	m_editGustFactor_Ex.SetEditUnit(m_pData->CodeParam.KDS2021.dGfExternal, 4);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlBuildingType, m_pData->CodeParam.KDS2021.nBuildingType);
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_TORSIONAL_RDO, m_pData->CodeParam.KDS2021.nBuildingType==1);
	m_rdoAlongWind.SetCheck(m_pData->CodeParam.KDS2021.bAlongWind);
	m_rdoAcrossWind.SetCheck(m_pData->CodeParam.KDS2021.bAcrossWind);
	m_rdoTorsionalWind.SetCheck(m_pData->CodeParam.KDS2021.bTorsionalWind);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTorsional_Sub, m_pData->CodeParam.KDS2021.bTorsionalWind);

	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_pData->CodeParam.KDS2021.dCenterPnt[0], m_pData->CodeParam.KDS2021.dCenterPnt[1], m_pData->CodeParam.KDS2021.dCenterPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_WINDP_VORTEX_SHEDDING_CHK);
	int nVortex = pBtn2->GetCheck();

	int nMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);

	BOOL bEnableCtrl = nMethod == 1 && nVortex != 1 ? TRUE : FALSE;

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_STATIC0, bEnableCtrl, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_BLDTYPE_GRB, nMethod==1, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_TREEMENU_DIR_GRB, nMethod==1, TRUE);

	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_STATIC10, bEnableCtrl);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_DIRECT_FACTOR_EDIT, bEnableCtrl);

	if(nMethod==0)
	{
		m_rdoAlongWind.SetCheck(TRUE);
		m_rdoAcrossWind.SetCheck(FALSE);
		m_rdoTorsionalWind.SetCheck(FALSE);
	}
	OnCmdWindTypeCheck();
	GetDlgItem(IDC_WG_TREEMENU_DIR_ALONG_RDO)->EnableWindow(TRUE);
}

void CStldWindPressureNodalDlg_KDS2021::OnCmdGustFactor()
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

	T_WVEP_KDS2021* pWvepData = &WvepD.CodeParam.KDS2021;

	CStldWindPressureAreaDlg_KDS2021_GustF dlg;
	dlg.SetWindData(*pWvepData, m_pData);
	if(dlg.DoModal() == IDOK)
	{

		dlg.GetDlgInfo(m_pData->CodeParam.KDS2021);
		Data2Dlg( (void*) m_pData);
	}
}

void CStldWindPressureNodalDlg_KDS2021::OnCmdVibrationBtn() 
{
	ASSERT(0);
	CStldWindPressureVibrationDlg dlg;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, dlg.m_nBuildingType);
	dlg.m_nAcrossCheck = m_rdoAcrossWind.GetCheck();
	dlg.m_nTorsionalCheck = m_rdoTorsionalWind.GetCheck();
// 	dlg.SetWindData(m_pData->CodeParam.KDS2021, m_pData->nDirection, m_pData->dAngle);
	if (dlg.DoModal() == IDOK)
	{
		m_pData->CodeParam.KDS2021.dBLy = dlg.m_dBreadthX;
		m_pData->CodeParam.KDS2021.dBLx = dlg.m_dBreadthY;
		m_pData->CodeParam.KDS2021.dNoa = dlg.m_dFreqA;
		m_pData->CodeParam.KDS2021.dNot = dlg.m_dFreqT;
		m_pData->CodeParam.KDS2021.dZf = dlg.m_dZf;
	}
}

void CStldWindPressureNodalDlg_KDS2021::OnCmdBuildingTypeRadio()
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

void CStldWindPressureNodalDlg_KDS2021::OnCmdWindTypeCheck()
{
	int nBuildingType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBuildingType, nBuildingType);
	int nAcrossCheck = m_rdoAcrossWind.GetCheck();
	int nTorsionalCheck = m_rdoTorsionalWind.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_WG_TREEMENU_DIR_BTN, (nBuildingType==1&&nAcrossCheck == 1) || nTorsionalCheck == 1);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlTorsional_Sub, nTorsionalCheck==1);
	if(nTorsionalCheck==1)
	{
		m_wndCoordXYZ.SetFocus();
	}
}

void CStldWindPressureNodalDlg_KDS2021::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CStldWindPressureNodalDlg_KDS2021::UpdateBuffer()
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