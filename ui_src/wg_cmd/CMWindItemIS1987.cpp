// CMWindItemIS1987.cpp : implementation file
//
// 2003. 09. 24  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemIS1987.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIS1987 dialog


CCMWindItemIS1987::CCMWindItemIS1987(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemIS1987::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemIS1987)
	//}}AFX_DATA_INIT
	m_aCtrlMethod.Add(IDC_CMD_STANDARD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_GUSTF_RD2);

	m_aCtrlAutoUser.Add(IDC_CMD_AUTO_RD);
	m_aCtrlAutoUser.Add(IDC_CMD_USER_RD);

	m_aCtrlStandard.Add(IDC_CMD_BCLASS_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_BCLASS_COMBO);
	m_aCtrlStandard.Add(IDC_CMD_FRICTION_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_FRICTION_COMBO);
	m_aCtrlStandard.Add(IDC_CMD_LOADEVAL_CHK);
	m_aCtrlStandard.Add(IDC_CMD_ANAL_FRAME2);
	m_aCtrlStandard.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlStandard.Add(IDC_CMD_FCOEF_EDIT);

	m_aCtrlGustFactor.Add(IDC_CMD_GUSTF_TITLE);
	m_aCtrlGustFactor.Add(IDC_CMD_GUSTF_EDIT);
	m_aCtrlGustFactor.Add(IDC_CMD_FORCECOEF_TITLE);
	m_aCtrlGustFactor.Add(IDC_CMD_FORCECOEF_EDIT);

	m_aCtrlFriction.Add(IDC_CMD_FRICTION_TITLE);
	m_aCtrlFriction.Add(IDC_CMD_FRICTION_COMBO);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemIS1987::SetData2Dlg(T_WIND_IS1987& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemIS1987::SetDlg2Data(T_WIND_IS1987& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemIS1987::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemIS1987)
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_VRANGE_UNIT, m_unitVRange);
	DDX_Control(pDX, IDC_CMD_VRANGE_EDIT, m_editVRange);
	DDX_Control(pDX, IDC_CMD_TOPOFACTOR_EDIT, m_editTopoFactor);
	DDX_Control(pDX, IDC_CMD_TOPO_CHK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SCLASS_COMBO, m_cobxStructClass);
	DDX_Control(pDX, IDC_CMD_RISKCOEF_EDIT, m_editRiskCoef);
	DDX_Control(pDX, IDC_CMD_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_GUSTF_EDIT, m_editGustFactor);
	DDX_Control(pDX, IDC_CMD_FRICTION_COMBO, m_cobxFriction);
	DDX_Control(pDX, IDC_CMD_FORCECOEF_EDIT, m_editForceCoef2);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_BCLASS_COMBO, m_cobxBuildingClass);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemIS1987, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemIS1987)
	ON_BN_CLICKED(IDC_CMD_STANDARD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_USER_RD, OnCmdAutoUserRadio)
	ON_CBN_SELCHANGE(IDC_CMD_SCLASS_COMBO, OnSelchangeCmdSclassCombo)
	ON_BN_CLICKED(IDC_CMD_LOADEVAL_CHK, OnCmdLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_TOPO_CHK, OnCmdTopoChk)
	ON_BN_CLICKED(IDC_CMD_GUSTF_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_AUTO_RD, OnCmdAutoUserRadio)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT, OnChangeCmdSpeedEdit)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIS1987 message handlers

BOOL CCMWindItemIS1987::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);
	m_unitVRange.SetUnitType(CUnitCtrl::m_WIND_UNIT.IS1987_dTopographyRange);
	m_editVRange.SetUnitType(CUnitCtrl::m_WIND_UNIT.IS1987_dTopographyRange);
	m_editTopoFactor.SetUnitType(D_UNITSYS_NONE);
	m_editRiskCoef.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor.SetUnitType(D_UNITSYS_NONE);
	m_editForceCoef2.SetUnitType(D_UNITSYS_NONE);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	AlignControls();
	InitCategoryCombo();
	InitBuildingClassCombo();
	InitFrictionCombo();
	InitStructureClassCombo();

	m_grpWindParam.InitControl(this, CCMWindItemIS1987::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemIS1987::DestroyWindow() 
{
	Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CCMWindItemIS1987::OnCmdMethodRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStandard, nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGustFactor, nMethod == 1);

	if (nMethod == 0)
	{
		int nCheck = m_chkLoadEval.GetCheck();
		CDlgUtil::CtrlShowHide(this, m_aCtrlFriction, nCheck != 1);
	}
}

void CCMWindItemIS1987::OnCmdAutoUserRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	m_cobxStructClass.EnableWindow(nMethod == 0);
	m_editRiskCoef.SetReadOnly(nMethod == 0);
	if (nMethod == 0) CalculateRiskCoef();
}

void CCMWindItemIS1987::OnCmdLoadevalChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	m_editForceCoef.EnableWindow(nCheck == 1);

	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	if (nMethod == 0)
		CDlgUtil::CtrlShowHide(this, m_aCtrlFriction, nCheck != 1);
}

void CCMWindItemIS1987::OnCmdTopoChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	m_editTopoFactor.EnableWindow(nCheck == 1);
	m_editVRange.EnableWindow(nCheck == 1);
}

void CCMWindItemIS1987::OnSelchangeCmdSclassCombo() 
{
	// TODO: Add your control notification handler code here
	CalculateRiskCoef();
}

void CCMWindItemIS1987::OnChangeCmdSpeedEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CalculateRiskCoef();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemIS1987::InitDefaultData()
{
	if (m_bModify) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);  
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();
	
	m_Data.dTopographyRange = StorD.dStoryLevel - dBaseLevel;				
}

void CCMWindItemIS1987::AlignControls()
{
	CArray<UINT, UINT> aControls;
	aControls.Add(IDC_CMD_GUSTF_TITLE);
	aControls.Add(IDC_CMD_GUSTF_EDIT);
	aControls.Add(IDC_CMD_FORCECOEF_TITLE);
	aControls.Add(IDC_CMD_FORCECOEF_EDIT);

	CRect ToRect, FromRect;
	m_cobxBuildingClass.GetWindowRect(ToRect);
	m_editGustFactor.GetWindowRect(FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCMWindItemIS1987::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("I"), _T("II"), _T("III"), _T("IV")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemIS1987::InitBuildingClassCombo()
{
	m_cobxBuildingClass.ResetContent();

	CString aItem[] = {_T("A"), _T("B"), _T("C")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxBuildingClass.AddString(aItem[i]);
	m_cobxBuildingClass.SetCurSel(0);
}

void CCMWindItemIS1987::InitFrictionCombo()
{
	m_cobxFriction.ResetContent();

	CString aItem[] = {_T("0.01"), _T("0.02"), _T("0.04")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxFriction.AddString(aItem[i]);
	m_cobxFriction.SetCurSel(0); 
}

void CCMWindItemIS1987::InitStructureClassCombo()
{
	m_cobxStructClass.ResetContent();

	CString aItem[] = {_LS(IDS_CMD_WIND_IS1987_StrClassA), _LS(IDS_CMD_WIND_IS1987_StrClassB), 
		_LS(IDS_CMD_WIND_IS1987_StrClassC), _LS(IDS_CMD_WIND_IS1987_StrClassD)};
	
	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxStructClass.AddString(aItem[i]);
		m_cobxStructClass.SetItemData(nIndex, i);
	}
	m_cobxStructClass.SetCurSel(0); 
}

void CCMWindItemIS1987::CalculateRiskCoef()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	if (nMethod != 0) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return; }

	int nIndex = m_cobxStructClass.GetCurSel();
	if (nIndex == CB_ERR) return;
	int nStructClass = m_cobxStructClass.GetItemData(nIndex);

	CString csVal;
	m_editSpeed.GetWindowText(csVal);

	double dRiskCoef;
	dRiskCoef = pDoc->m_pEditData->GetIS1987RiskCoef(nStructClass, _tstof(csVal));
	m_editRiskCoef.SetEditUnit(dRiskCoef);
}

void CCMWindItemIS1987::Data2Dlg()
{
	CString csVal;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure-1);
	OnCmdMethodRadio();

	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	OnChangeCmdSpeedEdit();
	m_cobxCategory.SetCurSel(m_Data.nTerrainCategory);
	m_cobxBuildingClass.SetCurSel(m_Data.nBuildingClass);
	if (m_Data.dFrictionCoef == 0.01 || m_Data.dFrictionCoef == 0.02 ||
			m_Data.dFrictionCoef == 0.04)
	{
		csVal.Format(_T("%g"), m_Data.dFrictionCoef);
		m_cobxFriction.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dFrictionCoef);
		m_cobxFriction.SetWindowText(csVal);
	}
	m_editGustFactor.SetEditUnit(m_Data.dGustFactor);
	m_editForceCoef2.SetEditUnit(m_Data.dForceCoef);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAutoUser, m_Data.nAutoCalcRiskCoef);
	OnCmdAutoUserRadio();
	m_cobxStructClass.SetCurSel(m_Data.nStructureClass);
	OnSelchangeCmdSclassCombo();
	m_editRiskCoef.SetEditUnit(m_Data.dRiskCoef);

	m_chkTopographic.SetCheck(m_Data.bTopography);
	OnCmdTopoChk();
	m_editTopoFactor.SetEditUnit(m_Data.dTopographicFactor);
	m_editVRange.SetEditUnit(m_Data.dTopographyRange);

	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoef);
}

BOOL CCMWindItemIS1987::Dlg2Data()
{
	CString csVal;
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nProcedure);
	m_Data.nProcedure += 1;

	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.nTerrainCategory = m_cobxCategory.GetCurSel();
	if (m_Data.nProcedure == 1)
	{
		m_Data.nBuildingClass = m_cobxBuildingClass.GetCurSel();
		m_cobxFriction.GetWindowText(csVal);
		m_Data.dFrictionCoef = _tstof(csVal);
	}
	else 
	{
		m_Data.dGustFactor = m_editGustFactor.GetEditValue();
		m_Data.dForceCoef  = m_editForceCoef2.GetEditValue();
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, m_Data.nAutoCalcRiskCoef);
	if (m_Data.nAutoCalcRiskCoef == 0)
		m_Data.nStructureClass = m_cobxStructClass.GetCurSel();
	m_Data.dRiskCoef = m_editRiskCoef.GetEditValue();

	m_Data.bTopography = m_chkTopographic.GetCheck();
	if (m_Data.bTopography)
	{
		m_Data.dTopographicFactor = m_editTopoFactor.GetEditValue();
		m_Data.dTopographyRange = m_editVRange.GetEditValue();
	}

	if (m_Data.nProcedure == 1)
	{
		m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
		if (m_Data.bUseForceCoef)
		m_Data.dForceCoef = m_editForceCoef.GetEditValue();
	}

	return TRUE;
}

LRESULT CCMWindItemIS1987::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdMethodRadio();
				OnCmdLoadevalChk();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
