
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDefPage.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_base\DlgTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefPage dialog


CCMLoadCombDefPage::CCMLoadCombDefPage(CLoadCombDefineData* pData, CWnd* pParent/*=NULL*/)
	: CChildDialog(CCMLoadCombDefPage::IDD, pParent)
{
	m_pData = pData;
	m_pParent = (CCMLoadCombDefDlg*)pParent;
	m_bInit = TRUE;
}

void CCMLoadCombDefPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombDefPage)
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CODE_COMBO, m_cmbCode);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CASE_CHECK, m_bChkCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CASE_WIND_CHECK, m_bChkWind);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLoadCombDefPage, CChildDialog)
	//{{AFX_MSG_MAP(CCMLoadCombDefPage)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_DEF_CODE_COMBO, OnSelectCode)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_DEF_RESET_BUTTON, OnResetButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_ACTIVE_TAB, OpenNewTab)
	ON_REGISTERED_MESSAGE(CDlgTabCtrl::WM_CDLGTABCTRL_INACTIVE_TAB, CloseCurTab)
END_MESSAGE_MAP()

BOOL CCMLoadCombDefPage::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitSelectCode();
	Data2Dlg();

	return TRUE;
}

void CCMLoadCombDefPage::InitGrid(BOOL bInit)
{
	int nIndex = m_cmbCode.GetCurSel();

	m_wndGrid.Initialize(nIndex, m_pData, bInit);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
}

BOOL CCMLoadCombDefPage::Data2Dlg()
{
	if (m_pData->m_strCode == _T(""))
		m_pData->m_strCode = _T("GB50017-17");

	m_cmbCode.SetCurSel(GetSelectCodeIndex(m_pData->m_strCode));

	if (m_pData->m_strCode == _T("GB50017-17") || m_pData->m_strCode == _T("JGJ99-2015") || m_pData->m_strCode == _T("GB55006-2021"))
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_CASE_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_CASE_CHECK)->EnableWindow(FALSE);
	}

	m_bChkCase.SetCheck(m_pData->m_bImperfection);
	m_bChkWind.SetCheck(m_pData->m_bWindForward);

	InitGrid(m_bInit);
	return TRUE;
}

BOOL CCMLoadCombDefPage::Dlg2Data()
{
	UpdateData(TRUE);
	int nIndex = m_cmbCode.GetCurSel();
	m_cmbCode.GetLBText(nIndex, m_pData->m_strCode);
	m_pData->m_bImperfection = m_bChkCase.GetCheck() ? TRUE : FALSE;
	m_pData->m_bWindForward = m_bChkWind.GetCheck() ? TRUE : FALSE;
	int nRow = m_wndGrid.GetRowCount();
	int nCol = m_wndGrid.GetColCount();
	m_pData->m_item.initial();
	CString aTitle[15] = { _T("D"), _T("L"), _T("LR"), _T("W"), _T("E"), _T("EVT"), _T("S"), _T("T"), _T("PS"), _T("EH"), _T("EV"), _T("WP"), _T("FP"), _T("SF"), _T("B") };
	CArrayEx<double, double> arFactor;
	for (int i = 0; i < nRow; i++)
	{
		int nIdx = m_wndGrid.GetValueRowCol(i + 1, 1) == _T("") ? 0 : _ttoi(m_wndGrid.GetValueRowCol(i + 1, 1));
		if (nIdx > 0)
		{
			arFactor.RemoveAll();
			for (int k = 1; k < nCol; k++)
			{
				double dVal = m_wndGrid.GetValueRowCol(i + 1, k + 1) == _T("") ? 0.0 : _ttof(m_wndGrid.GetValueRowCol(i + 1, k + 1));
				arFactor.Add(dVal);
			}
			m_pData->m_item.m_aFactor.Add(arFactor);
		}
	}

	for (int k = 1; k < nCol; k++)
	{
		m_pData->m_item.m_aType.Add(aTitle[k - 1]);
	}
	UpdateData(FALSE);
	return TRUE;
}

void CCMLoadCombDefPage::OnSelectCode()
{
	UpdateData(TRUE);
	int nIndex = m_cmbCode.GetCurSel();
	if (nIndex == 0 || nIndex == 1 || nIndex == 9)
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_CASE_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOADCOMB_DEF_CASE_CHECK)->EnableWindow(FALSE);
	}
	InitGrid(TRUE);

	UpdateData(FALSE);
}

void CCMLoadCombDefPage::InitSelectCode()
{
	m_cmbCode.ResetContent();
	CStringArray CodeName;
	CodeName.Add(_T("GB50017-17"));	//0
	CodeName.Add(_T("JGJ99-2015"));	//1
	CodeName.Add(_T("GB50017-03"));	//2
	CodeName.Add(_T("GB/T50010-10"));	//3
	CodeName.Add(_T("JGJ3-2010"));	//4
	CodeName.Add(_T("GB50010-02"));	//5
	CodeName.Add(_T("GB50018-02"));	//6
	CodeName.Add(_T("GB50069-02"));	//7
	CodeName.Add(_T("GB50429-07"));	//8
	CodeName.Add(_T("GB55006-2021"));	//9
	for (int i = 0; i < CodeName.GetSize(); i++)
	{
		m_cmbCode.AddString(CodeName.GetAt(i));
	}
	m_cmbCode.SetCurSel(0);
}

int CCMLoadCombDefPage::GetSelectCodeIndex(CString strCode)
{
	if (strCode == _T("GB50017-17"))
		return 0;
	else if (strCode == _T("JGJ99-2015"))
		return 1;
	else if (strCode == _T("GB50017-03"))
		return 2;
	else if (strCode == _T("GB/T50010-10"))
		return 3;
	else if (strCode == _T("JGJ3-2010"))
		return 4;
	else if (strCode == _T("GB50010-02"))
		return 5;
	else if (strCode == _T("GB50018-02"))
		return 6;
	else if (strCode == _T("GB50069-02"))
		return 7;
	else if (strCode == _T("GB50429-07"))
		return 8;
	else if (strCode == _T("GB55006-2021"))
		return 9;
	return 0;
}

void CCMLoadCombDefPage::OnResetButton()
{
	InitGrid(TRUE);
}

LRESULT CCMLoadCombDefPage::OpenNewTab(WPARAM wParam, LPARAM lParam)
{
	//OnActive_Tab();  
	if (!Data2Dlg())
		return 0L;
	m_bInit = FALSE;
	return 0L;
}

LRESULT CCMLoadCombDefPage::CloseCurTab(WPARAM wParam, LPARAM lParam)
{
	if (!Dlg2Data())
		return 0L;

	return 0L;
}
