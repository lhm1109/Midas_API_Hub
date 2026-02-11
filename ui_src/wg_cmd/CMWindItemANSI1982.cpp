// CMWindItemANSI1982.cpp : implementation file
//
// 2003. 09. 23   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemANSI1982.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemANSI1982 dialog


CCMWindItemANSI1982::CCMWindItemANSI1982(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemANSI1982::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMWindItemANSI1982)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMWindItemANSI1982::SetData2Dlg(T_WIND_ANSI1982& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemANSI1982::SetDlg2Data(T_WIND_ANSI1982& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemANSI1982::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemANSI1982)
	DDX_Control(pDX, IDC_CMD_WINWARD_COEF_EDIT, m_editWinwardCoef);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_LEEWARD_COEF_EDIT, m_editLeewardCoef);
	DDX_Control(pDX, IDC_CMD_IFACTOR_EDIT, m_editIFactor);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemANSI1982, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemANSI1982)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemANSI1982 message handlers

BOOL CCMWindItemANSI1982::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemANSI1982::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

 	m_editWinwardCoef.SetUnitType(CUnitCtrl::m_WIND_UNIT.ANSI1982_dWindwardCoefficient);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.ANSI1982_dBasicWindSpeed);
	m_editLeewardCoef.SetUnitType(CUnitCtrl::m_WIND_UNIT.ANSI1982_dLewardCoefficient);
	m_editIFactor.SetUnitType(CUnitCtrl::m_WIND_UNIT.ANSI1982_dImportanceFactor);

	InitCategoryCombo();

	Data2Dlg(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemANSI1982::DestroyWindow() 
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

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemANSI1982::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemANSI1982::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory - 1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editIFactor.SetEditUnit(m_Data.dImportanceFactor);
	m_editWinwardCoef.SetEditUnit(m_Data.dWindwardCoefficient);
	m_editLeewardCoef.SetEditUnit(m_Data.dLewardCoefficient);
}

BOOL CCMWindItemANSI1982::Dlg2Data()
{
	m_Data.nExposureCategory = m_cobxCategory.GetCurSel();
	m_Data.nExposureCategory += 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dImportanceFactor = m_editIFactor.GetEditValue();
	m_Data.dWindwardCoefficient = m_editWinwardCoef.GetEditValue();
	m_Data.dLewardCoefficient = m_editLeewardCoef.GetEditValue();

	return TRUE;
}

LRESULT CCMWindItemANSI1982::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}