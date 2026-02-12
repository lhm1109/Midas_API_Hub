// CMWindItemUBC1997.cpp : implementation file
//
// 2003. 09. 23   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemUBC1997.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemUBC1997 dialog


CCMWindItemUBC1997::CCMWindItemUBC1997(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemUBC1997::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemUBC1997)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlMethod.Add(IDC_CMD_SIMPLE_RD1);
	m_aCtrlMethod.Add(IDC_CMD_ANAL_RD2);

	m_aCtrlProjectArea.Add(IDC_CMD_PRES_COEF_TITLE);
	m_aCtrlProjectArea.Add(IDC_CMD_PRES_COEF_EDIT);

	m_aCtrlNormalForce.Add(IDC_CMD_ROOFH_TITLE);
	m_aCtrlNormalForce.Add(IDC_CMD_ROOFH_EDIT);
	m_aCtrlNormalForce.Add(IDC_CMD_ROOFH_UNIT);

	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemUBC1997::SetData2Dlg(T_WIND_UBC1997& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemUBC1997::SetDlg2Data(T_WIND_UBC1997& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}


void CCMWindItemUBC1997::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemUBC1997)
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOFH_UNIT, m_unittRoofH);
	DDX_Control(pDX, IDC_CMD_ROOFH_EDIT, m_editRoofH);
	DDX_Control(pDX, IDC_CMD_PRES_COEF_EDIT, m_editPresCoef);
	DDX_Control(pDX, IDC_CMD_IFACTOR_EDIT, m_editIFactor);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemUBC1997, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemUBC1997)
	ON_BN_CLICKED(IDC_CMD_SIMPLE_RD1, OnCmdSimpleAnalRadio)
	ON_BN_CLICKED(IDC_CMD_ANAL_RD2, OnCmdSimpleAnalRadio)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemUBC1997 message handlers

BOOL CCMWindItemUBC1997::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemUBC1997::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	CRect ToRect, FromRect;
	m_editRoofH.GetWindowRect(ToRect);
	m_editPresCoef.GetWindowRect(FromRect);
	int nDistY =  ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlProjectArea, nDistY);

	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.UBC1997_dBasicWindSpeed);
	m_unittRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.UBC1997_dHRoof);
	m_editRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.UBC1997_dHRoof);
	m_editPresCoef.SetUnitType(CUnitCtrl::m_WIND_UNIT.UBC1997_dPressureCoefficient);
	m_editIFactor.SetUnitType(CUnitCtrl::m_WIND_UNIT.UBC1997_dImportanceFactor);
 
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

BOOL CCMWindItemUBC1997::DestroyWindow() 
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

void CCMWindItemUBC1997::OnCmdSimpleAnalRadio() 
{
	// TODO: Add your control notification handler code here
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod); 
	CDlgUtil::CtrlShowHide(this, m_aCtrlProjectArea, nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlNormalForce, nMethod == 1);	
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function
	
void CCMWindItemUBC1997::InitDefaultData()
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
	
	if (!m_bModify) m_Data.dHRoof = StorD.dStoryLevel - dBaseLevel;
}

void CCMWindItemUBC1997::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemUBC1997::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure - 1);
	OnCmdSimpleAnalRadio();
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editIFactor.SetEditUnit(m_Data.dImportanceFactor);
	m_editPresCoef.SetEditUnit(m_Data.dPressureCoefficient);
	m_editRoofH.SetEditUnit(m_Data.dHRoof);
}

BOOL CCMWindItemUBC1997::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nProcedure);
	m_Data.nProcedure += 1;
	m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dImportanceFactor = m_editIFactor.GetEditValue();
	if (m_Data.nProcedure == 1)
		m_Data.dPressureCoefficient = m_editPresCoef.GetEditValue();
	else 
		m_Data.dHRoof = m_editRoofH.GetEditValue();

	return TRUE;
}

LRESULT CCMWindItemUBC1997::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				OnCmdSimpleAnalRadio();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
