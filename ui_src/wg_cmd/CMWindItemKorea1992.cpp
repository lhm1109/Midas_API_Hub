// CMWindItemKorea1992.cpp : implementation file
//
// 2003. 09. 22/    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemKorea1992.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea1992 dialog


CCMWindItemKorea1992::CCMWindItemKorea1992(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemKorea1992::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemKorea1992)
	//}}AFX_DATA_INIT
	m_Data.Initialize();
}

void CCMWindItemKorea1992::SetData2Dlg(T_WIND_KS1992& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemKorea1992::SetDlg2Data(T_WIND_KS1992& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}


void CCMWindItemKorea1992::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemKorea1992)
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_PRESSURE_COEF_EDIT, m_editCoeff);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemKorea1992, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemKorea1992)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKorea1992 message handlers

BOOL CCMWindItemKorea1992::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_grpWindParam.InitControl(this, CCMWindItemKorea1992::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS1992_dBasicWindSpeed);
	m_editCoeff.SetUnitType(CUnitCtrl::m_WIND_UNIT.KS1992_dPressureCoefficient);

	InitCategotyCombo();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemKorea1992::DestroyWindow() 
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


void CCMWindItemKorea1992::InitCategotyCombo()
{
	m_cobxCategory.ResetContent();

	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemKorea1992::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editCoeff.SetEditUnit(m_Data.dPressureCoefficient);
}

BOOL CCMWindItemKorea1992::Dlg2Data()
{
	m_Data.Initialize();

	int nIndex = m_cobxCategory.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_Data.nExposureCategory = m_cobxCategory.GetItemData(nIndex);
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.dPressureCoefficient = m_editCoeff.GetEditValue();

	return TRUE;
}

LRESULT CCMWindItemKorea1992::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
