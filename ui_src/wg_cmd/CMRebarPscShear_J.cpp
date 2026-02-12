// CMRebarPscShear_J.cpp : implementation file
//
// 2004. 03. 08    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarPscShear_J.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear_J dialog


CCMRebarPscShear_J::CCMRebarPscShear_J(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarPscShear_J::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscShear_J)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_PSCD_D PscdD;
	if (!pDoc->m_pAttrCtrl->GetDgnPscd(PscdD)) PscdD.Initialize();
	m_bShowStirrup = (PscdD.iDgnCode == JTG_D62_04 || PscdD.iDgnCode ==TB_10002_3_05);   // JTG D62-04.

	m_aCtrlStirrup.Add(IDC_CMD_STRR_CHECK);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_PCH_TEXT);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_PCH_EDIT);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_PCH_UNIT);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_AW_TEXT);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_AW_EDIT);
	m_aCtrlStirrup.Add(IDC_CMD_STRR_AW_UNIT);

	m_bSymmetry = FALSE;
	m_Data.Initialize();
}


void CCMRebarPscShear_J::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarPscShear_J)
	DDX_Control(pDX, IDC_CMD_TENS_RF_EDIT, m_editTensRf);
	DDX_Control(pDX, IDC_CMD_STRR_AW_UNIT, m_unitStrrAw);
	DDX_Control(pDX, IDC_CMD_STRR_AW_EDIT, m_editStrrAw);
	DDX_Control(pDX, IDC_CMD_STRR_PCH_UNIT, m_unitStrrPch);
	DDX_Control(pDX, IDC_CMD_STRR_PCH_EDIT, m_editStrrPch);
	DDX_Control(pDX, IDC_CMD_STRR_CHECK, m_chkStirrup);
	DDX_Control(pDX, IDC_CMD_LBAR_ALT_EDIT, m_editLbarAlt);
	DDX_Control(pDX, IDC_CMD_LBAR_ALT_UNIT, m_unitLbarAlt);
	DDX_Control(pDX, IDC_CMD_LBAR_AWT_EDIT, m_editLbarAwt);
	DDX_Control(pDX, IDC_CMD_LBAR_AWT_UNIT, m_unitLbarAwt);
	DDX_Control(pDX, IDC_CMD_LBAR_PCH_EDIT, m_editLbarPch);
	DDX_Control(pDX, IDC_CMD_LBAR_PCH_UNIT, m_unitLbarPch);
	DDX_Control(pDX, IDC_CMD_SBAR_ANG_EDIT, m_editSbarAng);
	DDX_Control(pDX, IDC_CMD_SBAR_ANG_UNIT, m_unitSbarAng);
	DDX_Control(pDX, IDC_CMD_SBAR_AW_EDIT, m_editSbarAw);
	DDX_Control(pDX, IDC_CMD_SBAR_AW_UNIT, m_unitSbarAw);
	DDX_Control(pDX, IDC_CMD_SBAR_PCH_EDIT, m_editSbarPch);
	DDX_Control(pDX, IDC_CMD_SBAR_PCH_UNIT, m_unitSbarPch);
	DDX_Control(pDX, IDC_CMD_TENS_ANG_EDIT, m_editTensAng);
	DDX_Control(pDX, IDC_CMD_TENS_ANG_UNIT, m_unitTensAng);
	DDX_Control(pDX, IDC_CMD_TENS_AP_EDIT, m_editTenAp);
	DDX_Control(pDX, IDC_CMD_TENS_AP_UNIT, m_unitTenAp);
	DDX_Control(pDX, IDC_CMD_TENS_PCH_EDIT, m_editTensPch);
	DDX_Control(pDX, IDC_CMD_TENS_PCH_UNIT, m_unitTensPch);
	DDX_Control(pDX, IDC_CMD_TENS_PE_EDIT, m_editTensPe);
	DDX_Control(pDX, IDC_CMD_TENS_PE_UNIT, m_unitTensPe);
	DDX_Control(pDX, IDC_CMD_LBAR_CHECK, m_chkLbar);
	DDX_Control(pDX, IDC_CMD_SBAR_CHECK, m_chkSbar);
	DDX_Control(pDX, IDC_CMD_TENS_CHECK, m_chkTens);
	DDX_Control(pDX, IDC_CMD_ACP_CHECK, m_chkAcp);
	DDX_Control(pDX, IDC_CMD_CT_EDIT, m_editCoverThk);
	DDX_Control(pDX, IDC_CMD_CT_UNIT, m_unitCoverThk);
	DDX_Control(pDX, IDC_CMD_INC_FC_CHECK, m_chkIncFC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarPscShear_J, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarPscShear_J)
	ON_BN_CLICKED(IDC_CMD_LBAR_CHECK, OnCmdLbarCheck)
	ON_BN_CLICKED(IDC_CMD_SBAR_CHECK, OnCmdSbarCheck)
	ON_BN_CLICKED(IDC_CMD_TENS_CHECK, OnCmdTensCheck)
	ON_BN_CLICKED(IDC_CMD_STRR_CHECK, OnCmdStrrCheck)
	ON_BN_CLICKED(IDC_CMD_ACP_CHECK, OnCmdAcpCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// interface

void CCMRebarPscShear_J::SetCurData(T_RPSC_SBAR* pData, BOOL bSymmetry)
{
	ASSERT(pData);  
	m_Data = *pData;
	m_bSymmetry = bSymmetry;

	HWND hWnd = GetSafeHwnd();
	if (hWnd && ::IsWindow(hWnd)) 
	{
		Data2Dlg();
	}
}

BOOL CCMRebarPscShear_J::GetCurData(T_RPSC_SBAR* pData)
{
	ASSERT(pData);

	HWND hWnd = GetSafeHwnd();
	if (hWnd && ::IsWindow(hWnd)) Dlg2Data(pData);
	else *pData = m_Data;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear_J message handlers

BOOL CCMRebarPscShear_J::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editLbarAlt.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarAlt);
	m_unitLbarAlt.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarAlt);
	m_editLbarAwt.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarAwt);
	m_unitLbarAwt.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarAwt);
	m_editLbarPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarPitch);
	m_unitLbarPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarPitch);

	m_editSbarAng.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarTheta);
	m_unitSbarAng.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarTheta);
	m_editSbarAw.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarAw);
	m_unitSbarAw.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarAw);
	m_editSbarPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarPitch);
	m_unitSbarPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarPitch);
	
	m_editTensAng.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnTheta);
	m_unitTensAng.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnTheta);
	m_editTenAp.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnAp);
	m_unitTenAp.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnAp);
	m_editTensPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnDist);
	m_unitTensPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnDist);
	m_editTensPe.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnPs);
	m_unitTensPe.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dTndnPs);
	m_editTensRf.SetUnitType(D_UNITSYS_NONE);

	m_editStrrAw.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarAw);
	m_unitStrrAw.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarAw);
	m_editStrrPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarPitch);
	m_unitStrrPch.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dSbarPitch);

	m_editCoverThk.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarThick);
	m_unitCoverThk.SetUnitType(CUnitCtrl::m_RPSC_UNIT.dLbarThick);

	CDlgUtil::CtrlShowHide(this, m_aCtrlStirrup, m_bShowStirrup);

	CArray<UINT, UINT> aCtrlAcp;
	aCtrlAcp.Add(IDC_CMD_ACP_CHECK);
	aCtrlAcp.Add(IDC_CMD_CT_TEXT);
	aCtrlAcp.Add(IDC_CMD_CT_EDIT);
	aCtrlAcp.Add(IDC_CMD_CT_UNIT);
	aCtrlAcp.Add(IDC_CMD_INC_FC_CHECK);
 
	
	if (!m_bShowStirrup)
	{
		CRect rToMove, rToRef;
		m_chkStirrup.GetWindowRect(rToRef);
		m_chkAcp.GetWindowRect(rToMove);
		CDlgUtil::CtrlMoveDistY(this, aCtrlAcp, rToRef.top-rToMove.top);
	}
	
	// ±¹¹®ÆÇ¿¡¼­ Àá½Ã ¼û±è MNET1810-JBSEON-051031
	//if (CProduct::GetMovingType() == D_PRODUCT_MOVING_ORG)
	//  CDlgUtil::CtrlShowHide(this, aCtrlAcp, FALSE);

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMRebarPscShear_J::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Dlg2Data(&m_Data);
	
	return CChildDialog::DestroyWindow();
}


void CCMRebarPscShear_J::OnCmdLbarCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLbar.GetCheck();
	m_editLbarAlt.EnableWindow(nCheck);
	m_editLbarAwt.EnableWindow(nCheck);
	m_editLbarPch.EnableWindow(nCheck);	
}

void CCMRebarPscShear_J::OnCmdSbarCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkSbar.GetCheck();
	m_editSbarAng.EnableWindow(nCheck);
	m_editSbarAw.EnableWindow(nCheck);	
	m_editSbarPch.EnableWindow(nCheck);
}

void CCMRebarPscShear_J::OnCmdTensCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTens.GetCheck();
	m_editTensAng.EnableWindow(nCheck);
	m_editTenAp.EnableWindow(nCheck);
	m_editTensPch.EnableWindow(nCheck);
	m_editTensPe.EnableWindow(nCheck);	
	m_editTensRf.EnableWindow(nCheck);
}

void CCMRebarPscShear_J::OnCmdStrrCheck() 
{
	// TODO: Add your control notification handler code here
	if (!m_bShowStirrup) return;
	int nCheck = m_chkStirrup.GetCheck();
	m_editStrrPch.EnableWindow(nCheck);
	m_editStrrAw.EnableWindow(nCheck);
}

void CCMRebarPscShear_J::OnCmdAcpCheck()
{
	int nCheck = m_chkAcp.GetCheck();
	m_editCoverThk.EnableWindow(nCheck);
	m_chkIncFC.EnableWindow(nCheck);
 
}
/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMRebarPscShear_J::Data2Dlg()
{
	// °æ»çÀÎÀåÃ¶±Ù
	m_chkSbar.SetCheck(m_Data.bSbarExist);
	m_editSbarPch.SetEditUnit(m_Data.dSbarPitch);
	m_editSbarAng.SetEditUnit(m_Data.dSbarTheta);
	m_editSbarAw.SetEditUnit(m_Data.dSbarAw);
	OnCmdSbarCheck();

	// Àü´Ü°­ºÀ
	m_chkTens.SetCheck(m_Data.bTndnExist);
	m_editTensPch.SetEditUnit(m_Data.dTndnDist);
	m_editTensAng.SetEditUnit(m_Data.dTndnTheta);
	m_editTenAp.SetEditUnit(m_Data.dTndnAp);
	m_editTensPe.SetEditUnit(m_Data.dTndnPs);
	m_editTensRf.SetEditUnit(m_Data.dTndnFactor);
	OnCmdTensCheck();

	// È¾¹æÇâÃ¶±Ù
	m_chkLbar.SetCheck(m_Data.bLbarExist);
	m_editLbarPch.SetEditUnit(m_Data.dLbarPitch);
	m_editLbarAwt.SetEditUnit(m_Data.dLbarAwt);
	m_editLbarAlt.SetEditUnit(m_Data.dLbarAlt);
	OnCmdLbarCheck();

	// ¼öÁ÷¹æÇâ Ã¶±Ù
	if (m_bShowStirrup)
	{
		m_chkStirrup.SetCheck(m_Data.bStirExist); // MQC:1918 20080611 mylee
		m_editStrrPch.SetEditUnit(m_Data.dStirPitch);
		m_editStrrAw.SetEditUnit(m_Data.dStirAw);
		OnCmdStrrCheck();
	}

	// Acp °è»ê¿ë
	
	m_chkAcp.SetCheck(m_Data.bLbarFlg);
	m_editCoverThk.SetEditUnit(m_Data.dLbarThick);
	m_chkIncFC.SetCheck(m_Data.bIncFC);
	OnCmdAcpCheck();

	
	// enable/disable
	m_chkSbar.EnableWindow(!m_bSymmetry);
	m_chkTens.EnableWindow(!m_bSymmetry);
	m_chkLbar.EnableWindow(!m_bSymmetry);
	m_chkStirrup.EnableWindow(!m_bSymmetry);
	
	m_chkAcp.EnableWindow(!m_bSymmetry);
	if (m_bSymmetry)
	{
		m_editSbarPch.EnableWindow(FALSE);
		m_editSbarAng.EnableWindow(FALSE);
		m_editSbarAw.EnableWindow(FALSE);
		m_editTensPch.EnableWindow(FALSE);
		m_editTensAng.EnableWindow(FALSE);
		m_editTenAp.EnableWindow(FALSE);
		m_editTensPe.EnableWindow(FALSE);
		m_editTensRf.EnableWindow(FALSE);
		m_editLbarPch.EnableWindow(FALSE);
		m_editLbarAwt.EnableWindow(FALSE);
		m_editLbarAlt.EnableWindow(FALSE);
		m_editStrrPch.EnableWindow(FALSE);
		m_editStrrAw.EnableWindow(FALSE);
		
		m_editCoverThk.EnableWindow(FALSE);
		
		m_chkIncFC.EnableWindow(FALSE);
	}
}

void CCMRebarPscShear_J::Dlg2Data(T_RPSC_SBAR* pData)
{
	ASSERT(pData);
	pData->Initialize();

	// °æ»çÀÎÀåÃ¶±Ù
	pData->bSbarExist = m_chkSbar.GetCheck();
	if (pData->bSbarExist)
	{
		pData->dSbarPitch = m_editSbarPch.GetEditValue();
		pData->dSbarTheta = m_editSbarAng.GetEditValue();
		pData->dSbarAw = m_editSbarAw.GetEditValue();
	}

	// Àü´Ü°­ºÀ
	pData->bTndnExist = m_chkTens.GetCheck();
	if (pData->bTndnExist)
	{
		pData->dTndnDist = m_editTensPch.GetEditValue();
		pData->dTndnTheta = m_editTensAng.GetEditValue();
		pData->dTndnAp = m_editTenAp.GetEditValue();
		pData->dTndnPs = m_editTensPe.GetEditValue();
		pData->dTndnFactor = m_editTensRf.GetEditValue();
	}

	// È¾¹æÇâÃ¶±Ù
	pData->bLbarExist = m_chkLbar.GetCheck();
	if (pData->bLbarExist)
	{
		pData->dLbarPitch = m_editLbarPch.GetEditValue();
		pData->dLbarAwt = m_editLbarAwt.GetEditValue();
		pData->dLbarAlt = m_editLbarAlt.GetEditValue();
	}

	// ¼öÁ÷¹æÇâ Ã¶±Ù
	if (m_bShowStirrup)
	{
		pData->bStirExist = m_chkStirrup.GetCheck();
		if (pData->bStirExist)
		{
			pData->dStirPitch = m_editStrrPch.GetEditValue();
			pData->dStirAw = m_editStrrAw.GetEditValue();
		}
	}
	

	// Acp °è»ê
	pData->bLbarFlg = m_chkAcp.GetCheck();
	pData->dLbarThick = m_editCoverThk.GetEditValue();
	pData->bIncFC = m_chkIncFC.GetCheck();
 
}

