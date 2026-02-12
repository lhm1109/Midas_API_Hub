// CMWindItemEuro1992.cpp : implementation file
// 
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemEuro1992.h"
#include "CMWindItemDlg.h"

#include "WindEuroGustDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro1992 dialog


CCMWindItemEuro1992::CCMWindItemEuro1992(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemEuro1992::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemEuro1992)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlMethod.Add(IDC_CMD_SIMPLE_RD1);
	m_aCtrlMethod.Add(IDC_CMD_DETAIL_RD2);

	m_aCtrlDetail.Add(IDC_CMD_GUSTF_TITLE);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_EDIT);
	m_aCtrlDetail.Add(IDC_CMD_GUSTF_BUTTON);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemEuro1992::SetData2Dlg(T_WIND_EURO1992& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}


BOOL CCMWindItemEuro1992::SetDlg2Data(T_WIND_EURO1992& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}


void CCMWindItemEuro1992::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemEuro1992)
	DDX_Control(pDX, IDC_CMD_TOPO_CHECK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_WINDWARD_EDIT, m_editWindward);
	DDX_Control(pDX, IDC_CMD_VCT_UNIT, m_unitVCT);
	DDX_Control(pDX, IDC_CMD_VCT_EDIT, m_editVCT);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_LEEWARD_EDIT, m_editLeeward);
	DDX_Control(pDX, IDC_CMD_GUSTF_EDIT, m_editGustFactor);
	DDX_Control(pDX, IDC_CMD_FRICTION_EDIT, m_editFriction);
	DDX_Control(pDX, IDC_CMD_FCOEF_EDIT, m_editForceCeof);
	DDX_Control(pDX, IDC_CMD_CT_EDIT, m_editCT);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemEuro1992, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemEuro1992)
	ON_BN_CLICKED(IDC_CMD_DETAIL_RD2, OnCmdSimpleDetailRadio)
	ON_BN_CLICKED(IDC_CMD_LOADEVAL_CHK, OnCmdLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_TOPO_CHECK, OnCmdTopoCheck)
	ON_BN_CLICKED(IDC_CMD_SIMPLE_RD1, OnCmdSimpleDetailRadio)
	ON_BN_CLICKED(IDC_CMD_GUSTF_BUTTON, OnCmdGustfButton)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro1992 message handlers

BOOL CCMWindItemEuro1992::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemEuro1992::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_unitVCT.SetUnitType(CUnitCtrl::m_WIND_UNIT.EURO1992_dTopographyRange);
	m_editVCT.SetUnitType(CUnitCtrl::m_WIND_UNIT.EURO1992_dTopographyRange);
	m_editWindward.SetUnitType(D_UNITSYS_NONE);
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);
	m_editLeeward.SetUnitType(D_UNITSYS_NONE);
	m_editGustFactor.SetUnitType(D_UNITSYS_NONE);
	m_editFriction.SetUnitType(D_UNITSYS_NONE);
	m_editForceCeof.SetUnitType(D_UNITSYS_NONE);
	m_editCT.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitCategoryCombo();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemEuro1992::DestroyWindow() 
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


void CCMWindItemEuro1992::OnCmdSimpleDetailRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDetail, nMethod == 1);
}

void CCMWindItemEuro1992::OnCmdLoadevalChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	m_editForceCeof.EnableWindow(nCheck == 1);
	m_editWindward.EnableWindow(nCheck == 0);
	m_editLeeward.EnableWindow(nCheck == 0);
}

void CCMWindItemEuro1992::OnCmdTopoCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic.GetCheck();
	m_editCT.EnableWindow(nCheck == 1);
	m_editVCT.EnableWindow(nCheck == 1);
}

void CCMWindItemEuro1992::OnCmdGustfButton() 
{
	// TODO: Add your control notification handler code here	
	CWindEuroGustDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_nRoughCategory = m_cobxCategory.GetCurSel();
	dlg.m_dVref					 = m_editSpeed.GetEditValue();
	dlg.m_dCt						 = m_editCT.GetEditValue();		
	dlg.m_dCtRange       = m_editVCT.GetEditValue();
	dlg.m_bTopography    = m_chkTopographic.GetCheck();

	dlg.m_dH			 = m_dHEURO1992			 ;
	dlg.m_dBB			 = m_dBBEURO1992		 ;
	dlg.m_dBD			 = m_dBDEURO1992		 ;
	dlg.m_dFreq		 = m_dFreqEURO1992	 ;
	dlg.m_dDamping = m_dDampingEURO1992;

	if(dlg.DoModal() == IDOK)
	{		
		m_dHEURO1992			 = dlg.m_dH			 ;    
		m_dBBEURO1992		   = dlg.m_dBB		 ;		
		m_dBDEURO1992			 = dlg.m_dBD		 ; 		
		m_dFreqEURO1992		 = dlg.m_dFreq	 ;		
		m_dDampingEURO1992 = dlg.m_dDamping;		
		m_editGustFactor.SetEditUnit(dlg.m_dGustFactor);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemEuro1992::InitDefaultData()
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
	
	// 초기화
	m_dHEURO1992 = StorD.dStoryLevel - dBaseLevel;	
	m_dBBEURO1992		= 0.0;			
	m_dBDEURO1992		= 0.0;			
	m_dFreqEURO1992 = 0.0;		
	m_dDampingEURO1992 = 0.0;
}

void CCMWindItemEuro1992::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	CString aItem[] = {_T("I"), _T("II"), _T("III"), _T("IV")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemEuro1992::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure - 1);
	OnCmdSimpleDetailRadio();
	m_cobxCategory.SetCurSel(m_Data.nRoughCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dRefWindSpeed);
	m_editWindward.SetEditUnit(m_Data.dWindwardCoef);
	m_editLeeward.SetEditUnit(m_Data.dLeewardCoef);
	m_editFriction.SetEditUnit(m_Data.dFrictionCoef);

	m_chkTopographic.SetCheck(m_Data.bTopography);
	OnCmdTopoCheck();
	m_editCT.SetEditUnit(m_Data.dTopographyCoef);
	m_editVCT.SetEditUnit(m_Data.dTopographyRange);
	m_editGustFactor.SetEditUnit(m_Data.dGustFactor);

	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdLoadevalChk();
	m_editForceCeof.SetEditUnit(m_Data.dForceCoef);
}

BOOL CCMWindItemEuro1992::Dlg2Data()
{
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nProcedure);
	m_Data.nProcedure += 1;

	m_Data.nRoughCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dRefWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dFrictionCoef = m_editFriction.GetEditValue();

	m_Data.bTopography = m_chkTopographic.GetCheck();
	if (m_Data.bTopography)
	{
		m_Data.dTopographyCoef = m_editCT.GetEditValue();
		m_Data.dTopographyRange = m_editVCT.GetEditValue();
	}

	if (m_Data.nProcedure == 2)
		m_Data.dGustFactor = m_editGustFactor.GetEditValue();

	m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
	if (m_Data.bUseForceCoef)
		m_Data.dForceCoef = m_editForceCeof.GetEditValue();
	else 
	{
		m_Data.dWindwardCoef = m_editWindward.GetEditValue();
		m_Data.dLeewardCoef = m_editLeeward.GetEditValue();
	}

	return TRUE;
}

LRESULT CCMWindItemEuro1992::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
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
