// CMWindItemNBC1995.cpp : implementation file
//
// 2003. 09.  23   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemNBC1995.h"
#include "CMWindItemDlg.h"
#include "WindGustNbc95Dlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemNBC1995 dialog


CCMWindItemNBC1995::CCMWindItemNBC1995(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemNBC1995::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemNBC1995)
	//}}AFX_DATA_INIT
	m_aCtrlProcedure.Add(IDC_CMD_SIMPLE_RD1);
	m_aCtrlProcedure.Add(IDC_CMD_DETAIL_RD2);

	m_aCtrlSimple.Add(IDC_CMD_GUSTF_TITLE1);
	m_aCtrlSimple.Add(IDC_CMD_GUSTF_EDIT1);

	m_aCtrlDetail.Add(IDC_CMD_GUSTF_TITLE2);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_CGX_TITLE2);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_CGX_EDIT2);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_CGY_TITLE2);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_CGY_EDIT2);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_BUTTON2);
	m_aCtrlDetail.Add(IDC_CMD_CATEGORY_TITLE);
	m_aCtrlDetail.Add(IDC_CMD_CATEGORY_COMBO);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemNBC1995::SetData2Dlg(T_WIND_NBC1995& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}


BOOL CCMWindItemNBC1995::SetDlg2Data(T_WIND_NBC1995& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemNBC1995::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemNBC1995)
	DDX_Control(pDX, IDC_CMD_HILLSHAPE_COMBO, m_cobxHillShape);
	DDX_Control(pDX, IDC_CMD_TOPO_CHECK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_HLENGTH_UNIT, m_unitHLength);
	DDX_Control(pDX, IDC_CMD_HLENGTH_EDIT, m_editHLength);
	DDX_Control(pDX, IDC_CMD_HHEIGHT_UNIT, m_unitHHeight);
	DDX_Control(pDX, IDC_CMD_HHEIGHT_EDIT, m_editHHeight);
	DDX_Control(pDX, IDC_CMD_GUSTF_EDIT1, m_editGustFactor);
	DDX_Control(pDX, IDC_CMD_GUSTF_CGY_EDIT2, m_editGustCgy);
	DDX_Control(pDX, IDC_CMD_GUSTF_CGX_EDIT2, m_editGustCgx);
	DDX_Control(pDX, IDC_CMD_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_CBDIST_UNIT, m_unitCBDistance);
	DDX_Control(pDX, IDC_CMD_CBDIST_EDIT, m_editCBDistance);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_BLOCATION_COMBO, m_cobxBLocation);
	DDX_Control(pDX, IDC_CMD_BHEIGHT_UNIT, m_unitBHeight);
	DDX_Control(pDX, IDC_CMD_BHEIGHT_EDIT, m_editBHeight);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemNBC1995, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemNBC1995)
	ON_BN_CLICKED(IDC_CMD_DETAIL_RD2, OnCmdSimpleDetailRadio)
	ON_BN_CLICKED(IDC_CMD_SIMPLE_RD1, OnCmdSimpleDetailRadio)
	ON_BN_CLICKED(IDC_CMD_LOADEVAL_CHK, OnCmdLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_TOPO_CHECK, OnCmdTopoCheck)
	ON_CBN_SELCHANGE(IDC_CMD_HILLSHAPE_COMBO, OnSelchangeCmdHillshapeCombo)
	ON_BN_CLICKED(IDC_CMD_GUSTF_BUTTON2, OnCmdGustfButton2)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemNBC1995 message handlers

BOOL CCMWindItemNBC1995::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_unitHLength.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillLength);
	m_editHLength.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillLength);
	m_editHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillHeight);
	m_unitHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillHeight);
	m_unitCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillDistance);
	m_editCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dHillDistance);
	m_unitBHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dBuildingHeight);
	m_editBHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.NBC1995_dBuildingHeight);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor.SetUnitType(D_UNITSYS_NONE);
	m_editGustCgy.SetUnitType(D_UNITSYS_NONE);
	m_editGustCgx.SetUnitType(D_UNITSYS_NONE);
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	AlignControls();
	InitCategoryCombo();
	InitHillShapeCombo();
	InitHillLocCombo();

	m_grpWindParam.InitControl(this, CCMWindItemNBC1995::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemNBC1995::DestroyWindow() 
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

void CCMWindItemNBC1995::OnCmdSimpleDetailRadio() 
{
	// TODO: Add your control notification handler code here
	int nProcedure;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlProcedure, nProcedure);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDetail, nProcedure == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nProcedure == 0);
}

void CCMWindItemNBC1995::OnCmdLoadevalChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	m_editForceCoef.EnableWindow(nCheck == 1);
}

void CCMWindItemNBC1995::OnCmdTopoCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	m_cobxHillShape.EnableWindow(nCheck == 1);
	m_cobxBLocation.EnableWindow(nCheck == 1);
	m_editHHeight.EnableWindow(nCheck == 1);
	m_editHLength.EnableWindow(nCheck == 1);
	m_editCBDistance.EnableWindow(nCheck == 1);
	OnSelchangeCmdHillshapeCombo();
}

void CCMWindItemNBC1995::OnSelchangeCmdHillshapeCombo() 
{
	// TODO: Add your control notification handler code here
	int nTopo = m_chkTopographic.GetCheck();
	if (nTopo == 1)
	{
		int nHillShape = m_cobxHillShape.GetCurSel();
		m_cobxBLocation.EnableWindow(nHillShape == 1);
	}
}

void CCMWindItemNBC1995::OnCmdGustfButton2() 
{
	// TODO: Add your control notification handler code here	
	CWindGustNbc95Dlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.m_nExposure     = m_cobxCategory.GetCurSel(); 
	dlg.m_dWindSpeed    = m_editSpeed.GetEditValue();
	dlg.m_dBldgHeight   = m_editBHeight.GetEditValue();
	dlg.m_dBreadthX     = m_dNbc95BY;
	dlg.m_dBreadthY     = m_dNbc95BX; 
	dlg.m_dFrequencyX   = m_dNbc95FrequencyX;
	dlg.m_dFrequencyY   = m_dNbc95FrequencyY;
	dlg.m_dDamping      = m_dNbc95Damping;
	dlg.m_bTopography   = m_chkTopographic.GetCheck();
	dlg.m_dHillHeight   = m_editHHeight.GetEditValue();
	dlg.m_dHillLength   = m_editHLength.GetEditValue();
	dlg.m_dHillDistance = m_editCBDistance.GetEditValue();
	dlg.m_nHillShape    = m_cobxHillShape.GetCurSel();
	dlg.m_nBldgLocation = m_cobxBLocation.GetCurSel();
	
	if (dlg.DoModal() == IDOK)
	{
		m_editGustCgx.SetEditUnit(dlg.m_dGustFactorX);
		m_editGustCgy.SetEditUnit(dlg.m_dGustFactorY);		
		m_dNbc95BX         = dlg.m_dBreadthY;
		m_dNbc95BY         = dlg.m_dBreadthX;
		m_dNbc95FrequencyX = dlg.m_dFrequencyX;  
		m_dNbc95FrequencyY = dlg.m_dFrequencyY; 
		m_dNbc95Damping    = dlg.m_dDamping;	
	}	
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemNBC1995::InitDefaultData()
{
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
	
	// 폭 구하기 
	double dBx = 0.0;
	double dBy = 0.0;
	if (nStorNum > 0)
	{	
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if(StorD2.dStoryLevel > dBaseLevel) 
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	// 초기화
	m_dNbc95BX   = dBx;			
	m_dNbc95BY   = dBy;			
	m_dNbc95FrequencyX = 0.0;
	m_dNbc95FrequencyY = 0.0;
	m_dNbc95Damping = 0.0;

	if (!m_bModify)
		m_Data.dBuildingHeight = StorD.dStoryLevel - dBaseLevel;	
}

void CCMWindItemNBC1995::AlignControls()
{
	CArray<UINT, UINT> aControls;
	aControls.Add(IDC_CMD_GUSTF_TITLE2);
	aControls.Add(IDC_CMD_GUSTF_CGX_TITLE2);
	aControls.Add(IDC_CMD_GUSTF_CGX_EDIT2);
	aControls.Add(IDC_CMD_GUSTF_CGY_TITLE2);
	aControls.Add(IDC_CMD_GUSTF_CGY_EDIT2);
	aControls.Add(IDC_CMD_GUSTF_BUTTON2);

	CRect ToRect, FromRect;
	m_editGustFactor.GetWindowRect(ToRect);
	m_editGustCgx.GetWindowRect(FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCMWindItemNBC1995::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("A"), _T("B"), _T("C")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemNBC1995::InitHillShapeCombo()
{
	m_cobxHillShape.ResetContent();

	CString aItem[] = {_T("2-D Ridge or Valley"), _T("2-D Escarpment"), _T("3-D Axisym. Hill")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxHillShape.AddString(aItem[i]);
	m_cobxHillShape.SetCurSel(0);
}

void CCMWindItemNBC1995::InitHillLocCombo()
{
	m_cobxBLocation.ResetContent();

	CString aItem[] = {_T("Upwind"), _T("Downwind")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxBLocation.AddString(aItem[i]);
	m_cobxBLocation.SetCurSel(0);
}

void CCMWindItemNBC1995::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlProcedure, m_Data.nProcedure-1);
	OnCmdSimpleDetailRadio();
	m_editSpeed.SetEditUnit(m_Data.dWindSpeed);
	m_editBHeight.SetEditUnit(m_Data.dBuildingHeight);
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory);
	m_editGustFactor.SetEditUnit(m_Data.dGustFactor);
	m_editGustCgx.SetEditUnit(m_Data.dGustFactorX);
	m_editGustCgy.SetEditUnit(m_Data.dGustFactorY);
	m_chkTopographic.SetCheck(m_Data.bTopography);
	OnCmdTopoCheck();
	m_cobxHillShape.SetCurSel(m_Data.nHillShape);
	OnSelchangeCmdHillshapeCombo();
	m_cobxBLocation.SetCurSel(m_Data.nBldgLocation);
	m_editHHeight.SetEditUnit(m_Data.dHillHeight);
	m_editHLength.SetEditUnit(m_Data.dHillLength);
	m_editCBDistance.SetEditUnit(m_Data.dHillDistance);
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);
}

BOOL CCMWindItemNBC1995::Dlg2Data()
{
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlProcedure, m_Data.nProcedure);
	m_Data.nProcedure += 1;
	m_Data.dWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dBuildingHeight = m_editBHeight.GetEditValue();
	if (m_Data.nProcedure == 2)
	{
		m_Data.nExposureCategory = m_cobxCategory.GetCurSel();
		m_Data.dGustFactorX = m_editGustCgx.GetEditValue();
		m_Data.dGustFactorY = m_editGustCgy.GetEditValue();
	}
	else m_Data.dGustFactor = m_editGustFactor.GetEditValue();
	
	m_Data.bTopography = m_chkTopographic.GetCheck();
	if (m_Data.bTopography)
	{
		m_Data.nHillShape = m_cobxHillShape.GetCurSel();
		if (m_Data.nHillShape == 1) m_cobxBLocation.SetCurSel(m_Data.nBldgLocation);
		m_Data.dHillHeight = m_editHHeight.GetEditValue();
		m_Data.dHillLength = m_editHLength.GetEditValue();
	 m_Data.dHillDistance = m_editCBDistance.GetEditValue();
	}

	m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
	if (m_Data.bUseForceCoef)
		m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();

	return TRUE;
}

LRESULT CCMWindItemNBC1995::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdSimpleDetailRadio();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
