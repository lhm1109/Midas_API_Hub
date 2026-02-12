// CMWindItemTaiwan1986.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemTaiwan1986.h"
#include "CMWindItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemTaiwan1986 dialog


CCMWindItemTaiwan1986::CCMWindItemTaiwan1986(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemTaiwan1986::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemTaiwan1986)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_bModify = FALSE;
	m_Data.Initialize();
}

void CCMWindItemTaiwan1986::SetData2Dlg(T_WIND_TAIWAN86& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemTaiwan1986::SetDlg2Data(T_WIND_TAIWAN86& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemTaiwan1986::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemTaiwan1986)
	DDX_Control(pDX, IDC_CMD_SITE_CATEGORY_CMB, m_wndSiteCategoryCmb);
	DDX_Control(pDX, IDC_CMD_SHAPE_FACTOR_EDIT, m_wndShapeFactor);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemTaiwan1986, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemTaiwan1986)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemTaiwan1986 message handlers

BOOL CCMWindItemTaiwan1986::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemTaiwan1986::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_wndShapeFactor.SetUnitType(D_UNITSYS_NONE);
	
	if (!m_bInit)
	{    
		m_bInit = TRUE;
	}
	
	InitSiteCategoryCombo();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemTaiwan1986::DestroyWindow() 
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

void CCMWindItemTaiwan1986::InitSiteCategoryCombo()
{
	m_wndSiteCategoryCmb.ResetContent();

	CString aItem[] = {_T("Grade 100"), _T("Grade 150"), _T("Grade 200"), _T("Grade 250")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_wndSiteCategoryCmb.AddString(aItem[i]);
	m_wndSiteCategoryCmb.SetCurSel(0);
}

void CCMWindItemTaiwan1986::Data2Dlg()
{
	CString csVal;
	
	m_wndShapeFactor.SetEditUnit(m_Data.dShapeFactor);  
	m_wndSiteCategoryCmb.SetCurSel(m_Data.nSiteCategory);  
}

BOOL CCMWindItemTaiwan1986::Dlg2Data()
{
	CString csVal;
	m_Data.Initialize();
	
	m_Data.dShapeFactor = m_wndShapeFactor.GetEditValue();
	m_Data.nSiteCategory = m_wndSiteCategoryCmb.GetCurSel();
	
	return TRUE;
}

LRESULT CCMWindItemTaiwan1986::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
