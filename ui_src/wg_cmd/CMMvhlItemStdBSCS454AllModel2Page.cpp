// CMMvhlItemStdBSCS454AllModel2Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSCS454AllModel2Page.h"
#include "CMMvhlItemStdBSDlg.h"
#include "CMMvhlSItemUsrLaneFactor.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS454AllModel2Page dialog

CCMMvhlItemStdBSCS454AllModel2Page::CCMMvhlItemStdBSCS454AllModel2Page(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSCS454AllModel2Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSCS454AllModel2Page)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nLaneFact = 0;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSCS454AllModel2Page::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSCS454AllModel2Page)
	DDX_Radio  (pDX, IDC_CMD_MVHL_LANE_FACTOR_CS_RDO1, m_nLaneFact);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC_CS_WND, m_wndPicture);
 	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO, m_cmbCategory);
 	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO, m_cmbLevel);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSCS454AllModel2Page::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSCS454AllModel2Page::SetData2Dlg()
{
	m_nLaneFact = (m_pData->nLaneFactor > 1) ? 1 : m_pData->nLaneFactor;
	m_cmbCategory.SetCurSel(m_pData->nAddDataCategory);
	m_cmbLevel.SetCurSel(m_pData->nAddDataLoadLevel);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSCS454AllModel2Page::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->nLaneFactor = m_nLaneFact;
	m_pData->nAddDataCategory = m_cmbCategory.GetCurSel();
	m_pData->nAddDataLoadLevel = m_cmbLevel.GetCurSel();

	return TRUE;
}

void CCMMvhlItemStdBSCS454AllModel2Page::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFact == 1);
	GetDlgItem(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFact == 1);
}

void CCMMvhlItemStdBSCS454AllModel2Page::InitCtrl()
{
	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + _T("cmd_mvhl_011.svg"));
}

void CCMMvhlItemStdBSCS454AllModel2Page::InitCombo()
{
	m_cmbCategory.ResetContent();
	m_cmbCategory.AddString(_T("Hp"));
	m_cmbCategory.AddString(_T("Mp"));
	m_cmbCategory.AddString(_T("Lp"));
	m_cmbCategory.AddString(_T("Hg"));
	m_cmbCategory.AddString(_T("Mg"));
	m_cmbCategory.AddString(_T("Lg"));
	m_cmbCategory.SetCurSel(0);

	m_cmbLevel.ResetContent();
	m_cmbLevel.AddString(_T("40t"));
	m_cmbLevel.AddString(_T("26t"));
	m_cmbLevel.AddString(_T("18t"));
	m_cmbLevel.AddString(_T("7.5t"));
	m_cmbLevel.AddString(_T("G1 FE"));
	m_cmbLevel.AddString(_T("G2 FE"));
	m_cmbLevel.AddString(_T("3t"));
	m_cmbLevel.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSCS454AllModel2Page, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSCS454AllModel2Page)
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO1, OnLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_CS_RDO2, OnLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_CS_LANE_FACT_USER_BTN, OnLaneFactorBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS454AllModel2Page message handlers
BOOL CCMMvhlItemStdBSCS454AllModel2Page::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitCtrl();
	InitCombo();

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSCS454AllModel2Page::OnLaneFactorRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSCS454AllModel2Page::OnLaneFactorBtn()
{
	CCMMvhlSItemUsrLaneFactor dlg;
	dlg.SetData(*m_pData);
	if (dlg.DoModal() == IDOK)
	{
		m_pData->dLaneFactor[0] = dlg.GetData().dLaneFactor[0];
		m_pData->dLaneFactor[1] = dlg.GetData().dLaneFactor[1];
		m_pData->dLaneFactor[2] = dlg.GetData().dLaneFactor[2];
		m_pData->dLaneFactor[3] = dlg.GetData().dLaneFactor[3];
	}
}