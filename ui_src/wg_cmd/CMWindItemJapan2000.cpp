// CMWindItemJapan2000.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan2000.h"
#include "CMWindItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2000 dialog


CCMWindItemJapan2000::CCMWindItemJapan2000(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemJapan2000::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemJapan2000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data.Initialize();
}

void CCMWindItemJapan2000::SetData2Dlg(T_WIND_JPN2000& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemJapan2000::SetDlg2Data(T_WIND_JPN2000& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemJapan2000::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan2000)
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan2000, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemJapan2000)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2000 message handlers

BOOL CCMWindItemJapan2000::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemJapan2000::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editSpeed.SetUnitType(D_UNITSYS_NONE);

	InitCategoryCombo();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemJapan2000::DestroyWindow() 
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

void CCMWindItemJapan2000::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = {_T("I"), _T("II"), _T("III"), _T("IV")};
	int nNum = sizeof (aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(2);
}

void CCMWindItemJapan2000::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
}

BOOL CCMWindItemJapan2000::Dlg2Data()
{
	m_Data.Initialize();
	m_Data.nExposureCategory = m_cobxCategory.GetCurSel();
	m_Data.nExposureCategory += 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	return TRUE;
}

LRESULT CCMWindItemJapan2000::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
