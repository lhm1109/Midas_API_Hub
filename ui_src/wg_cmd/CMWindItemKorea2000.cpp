// CMWindItemKorea2000.cpp : implementation file
//
// 2003. 09. 22   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemKorea2000.h"
#include "CMWindItemDlg.h"
#include "WindGustFactorDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea2000 dialog


CCMWindItemKorea2000::CCMWindItemKorea2000(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemKorea2000::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemKorea2000)
	//}}AFX_DATA_INIT
	m_aCtrlType.Add(IDC_CMD_RIGID_RADIO1);
	m_aCtrlType.Add(IDC_CMD_FLEX_RADIO2);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemKorea2000::SetData2Dlg(T_WIND_KS2000& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemKorea2000::SetDlg2Data(T_WIND_KS2000& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemKorea2000::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemKorea2000)
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT, m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_LOAD_EVAL_CHECK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_VKZT_UNIT, m_unitVKZT);
	DDX_Control(pDX, IDC_CMD_VKZT_EDIT, m_editVKZT);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT, m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_PRESSURE_COEF_EDIT, m_editPresCoef);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT, m_editKZT);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT, m_editGustFactor);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT_Y, m_editGustFactorY);   // new 6.5.0
	DDX_Control(pDX, IDC_CMD_FORCE_COEFF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemKorea2000, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemKorea2000)
	ON_BN_CLICKED(IDC_CMD_FLEX_RADIO2, OnCmdGustRadio)
	ON_BN_CLICKED(IDC_CMD_GUST_FACTOR_BTN, OnCmdGustFactorBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_EVAL_CHECK, OnCmdLoadEvalCheck)
	ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
	ON_BN_CLICKED(IDC_CMD_RIGID_RADIO1, OnCmdGustRadio)
	ON_CBN_SELCHANGE(IDC_CMD_CATEGORY_COMBO, OnSelchangeCmdCategoryCombo)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea2000 message handlers

BOOL CCMWindItemKorea2000::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemKorea2000::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_unitVKZT.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dHzt);
	m_editVKZT.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dHzt);
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dHRoof);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dHRoof);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dBasicWindSpeed);
	m_editPresCoef.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dImportanceFactor);
	m_editKZT.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dKzt);
	m_editGustFactor.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dGustFactor);
	m_editGustFactorY.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS2000_dGustFactor);  // new 6.5.0
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);

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


BOOL CCMWindItemKorea2000::DestroyWindow() 
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

void CCMWindItemKorea2000::OnCmdGustRadio() 
{
	int nGustType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, nGustType);
	if(nGustType == 0)
	{
		((CButton*)GetDlgItem(IDC_CMD_GUST_FACTOR_BTN))->EnableWindow(FALSE);	
		CString aGust[] = {_T("2.5"), _T("2.2"), _T("1.9"), _T("1.8")};
		int nIndex = m_cobxCategory.GetCurSel();
		if (nIndex != CB_ERR) 
		{
			m_editGustFactor.SetWindowText(aGust[nIndex]);	
			m_editGustFactorY.SetWindowText(aGust[nIndex]);   // new 6.5.0
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CMD_GUST_FACTOR_BTN))->EnableWindow(TRUE);
		OnCmdGustFactorBtn(); 
	}		
}

void CCMWindItemKorea2000::OnCmdGustFactorBtn() 
{
	// TODO: Add your control notification handler code here	
	CWindGustFactorDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	dlg.m_dHeight			= m_editRoofHeight.GetEditValue();
	dlg.m_dImportance = m_editPresCoef.GetEditValue();
	dlg.m_nExposure		= m_cobxCategory.GetCurSel();
	dlg.m_dSpeed			= m_editSpeed.GetEditValue();
	dlg.m_dKzt				= m_editKZT.GetEditValue();
	dlg.m_dHzt        = m_editVKZT.GetEditValue();
	dlg.m_bTopography = m_chkTopoEffect.GetCheck();
	dlg.m_dWidth			= m_dBYKOR2000;
	dlg.m_dBreadth		= m_dBXKOR2000;
	dlg.m_dFrequencyX = m_dFreqXKOR2000;
	dlg.m_dFrequencyY = m_dFreqYKOR2000;
	dlg.m_dDamping	  = m_dDampingKOR2000;  

	if (dlg.DoModal() == IDOK)
	{
		/*
		if (!dlg.m_bDirection) 
		 m_editGustFactor.SetEditUnit(dlg.m_dGustFactorX);
		else									 
			m_editGustFactor.SetEditUnit(dlg.m_dGustFactorY);	
		*/

		// new 6.5.0
		m_editGustFactor.SetEditUnit(dlg.m_dGustFactorX, 4);
		m_editGustFactorY.SetEditUnit(dlg.m_dGustFactorY, 4);
		
		m_dBYKOR2000			= dlg.m_dWidth;
		m_dBXKOR2000			= dlg.m_dBreadth;
		m_dFreqXKOR2000		= dlg.m_dFrequencyX;
		m_dFreqYKOR2000		= dlg.m_dFrequencyY;
		m_dDampingKOR2000 = dlg.m_dDamping;		
	}	
}

void CCMWindItemKorea2000::OnCmdLoadEvalCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	m_editForceCoef.EnableWindow(nCheck == 1);
}

void CCMWindItemKorea2000::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
	m_editVKZT.EnableWindow(nCheck == 1);
}

void CCMWindItemKorea2000::OnSelchangeCmdCategoryCombo() 
{
	// TODO: Add your control notification handler code here
	int nGustType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, nGustType);
	if (nGustType == 0)
	{
		CString aGust[] = {_T("2.5"), _T("2.2"), _T("1.9"), _T("1.8")};
		int nIndex = m_cobxCategory.GetCurSel();
		if (nIndex != CB_ERR) 
		{
			m_editGustFactor.SetWindowText(aGust[nIndex]);
			m_editGustFactorY.SetWindowText(aGust[nIndex]);	  // new 6.5.0
		}
	} 
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemKorea2000::InitDefaultData()
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
	m_dBXKOR2000 = dBx;	
	m_dBYKOR2000 = dBy;	
	m_dFreqXKOR2000 = 0.0;	
	m_dFreqYKOR2000 = 0.0;	
	m_dDampingKOR2000 = 0.0;

	if (!m_bModify) 
	{
		m_Data.dHRoof	= StorD.dStoryLevel - dBaseLevel;	
		m_editGustFactor.SetWindowText(_T("2.2")); 
		m_editGustFactorY.SetWindowText(_T("2.2"));   // new 6.5.0
	}
}

void CCMWindItemKorea2000::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemKorea2000::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	OnSelchangeCmdCategoryCombo();
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editPresCoef.SetEditUnit(m_Data.dImportanceFactor);
	m_editRoofHeight.SetEditUnit(m_Data.dHRoof);
	m_chkTopoEffect.SetCheck(m_Data.bTopography);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
	m_editVKZT.SetEditUnit(m_Data.dHzt);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlType, m_Data.nRigidity);
	((CButton*)GetDlgItem(IDC_CMD_GUST_FACTOR_BTN))->EnableWindow(m_Data.nRigidity == 1);	
	m_editGustFactor.SetEditUnit(m_Data.dGustFactor, 4);
	m_editGustFactorY.SetEditUnit(m_Data.dGustFactorY, 4);   // new 6.5.0
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdLoadEvalCheck();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);
}

BOOL CCMWindItemKorea2000::Dlg2Data()
{
	m_Data.Initialize();

	m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dImportanceFactor = m_editPresCoef.GetEditValue();
	m_Data.dHRoof = m_editRoofHeight.GetEditValue();
	m_Data.bTopography = m_chkTopoEffect.GetCheck();
	if (m_Data.bTopography)
	{
		m_Data.dKzt = m_editKZT.GetEditValue();
		m_Data.dHzt = m_editVKZT.GetEditValue();
	}
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, m_Data.nRigidity);
	m_Data.dGustFactor = m_editGustFactor.GetEditValue();
	m_Data.dGustFactorY = m_editGustFactorY.GetEditValue();   // new 6.5.0
	m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
	if (m_Data.bUseForceCoef)
		m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();

	return TRUE;
}

LRESULT CCMWindItemKorea2000::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
