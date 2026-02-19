// DgnStlRatingRivetsTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingRivetsTabDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingServiceParamTabJDlg dialog
CDgnStlRatingRivetsTabDlg::CDgnStlRatingRivetsTabDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnStlRatingRivetsTabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlRatingRivetsTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

void CDgnStlRatingRivetsTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingRivetsTabDlg)
	DDX_Control(pDX, IDC_DGN_LONGI_SPACING_EDT,     m_edtLongiSpacing);
	DDX_Control(pDX, IDC_DGN_LONGI_SPACING_UNT,     m_untLongiSpacing);
	DDX_Control(pDX, IDC_DGN_ULT_TENS_STRENGTH_EDT, m_edtUltTensStre);
	DDX_Control(pDX, IDC_DGN_ULT_TENS_STRENGTH_UNT, m_untUltTensStre);
	DDX_Control(pDX, IDC_DGN_YIELD_STRENGTH_EDT,    m_edtYieldStre);
	DDX_Control(pDX, IDC_DGN_YIELD_STRENGTH_UNT,    m_untYieldStre);
	//}}AFX_DATA_MAP 
}

void CDgnStlRatingRivetsTabDlg::GetRivtData(T_RIVT_SUB_D& rData)
{
	Dlg2Data();

	rData = m_Data;
}

void CDgnStlRatingRivetsTabDlg::SetRivtData(T_RIVT_SUB_D& Data)
{
	m_Data = Data;

	Data2Dlg();
}

void CDgnStlRatingRivetsTabDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;

	CArray<UINT, UINT> aControls;

	aControls.Add(IDC_DGN_YIELD_STRENGTH_STC);
	aControls.Add(IDC_DGN_YIELD_STRENGTH_EDT);
	aControls.Add(IDC_DGN_YIELD_STRENGTH_UNT);
	
	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_YIELD_STRENGTH_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
}

void CDgnStlRatingRivetsTabDlg::ShowHideControls()
{
	T_RPSS_D Rpss; Rpss.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRpss(Rpss);

	BOOL bUltTensStre = FALSE;
	BOOL bYieldStre = FALSE;

	if (Rpss.nFormula == 0) bUltTensStre = TRUE;
	else if (Rpss.nFormula == 1) bYieldStre = TRUE;

	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_STC)->ShowWindow(bUltTensStre);
	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_EDT)->ShowWindow(bUltTensStre);
	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_UNT)->ShowWindow(bUltTensStre);

	GetDlgItem(IDC_DGN_YIELD_STRENGTH_STC)->ShowWindow(bYieldStre);
	GetDlgItem(IDC_DGN_YIELD_STRENGTH_EDT)->ShowWindow(bYieldStre);
	GetDlgItem(IDC_DGN_YIELD_STRENGTH_UNT)->ShowWindow(bYieldStre);	
}

void CDgnStlRatingRivetsTabDlg::EnableDisableControls(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_LONGI_SPACING_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_LONGI_SPACING_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_LONGI_SPACING_UNT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_ULT_TENS_STRENGTH_UNT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_YIELD_STRENGTH_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_YIELD_STRENGTH_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_YIELD_STRENGTH_UNT)->EnableWindow(bEnable);
}

void CDgnStlRatingRivetsTabDlg::InitUnit()
{
	m_edtLongiSpacing.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dLongiSpacing);
	m_untLongiSpacing.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dLongiSpacing);
	m_edtUltTensStre.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dUltTensStre);
	m_untUltTensStre.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dUltTensStre);
	m_edtYieldStre.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dYieldStre);
	m_untYieldStre.SetUnitType(CUnitCtrl::m_RIVT_UNIT.dYieldStre);
}

void CDgnStlRatingRivetsTabDlg::Data2Dlg()
{
	m_edtLongiSpacing.SetEditUnit(m_Data.dLongiSpacing);
	m_edtUltTensStre.SetEditUnit(m_Data.dUltTensStre);
	m_edtYieldStre.SetEditUnit(m_Data.dYieldStre);

	UpdateData(FALSE);
}

BOOL CDgnStlRatingRivetsTabDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dLongiSpacing = m_edtLongiSpacing.GetEditValue();
	m_Data.dUltTensStre = m_edtUltTensStre.GetEditValue();
	m_Data.dYieldStre = m_edtYieldStre.GetEditValue();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnStlRatingRivetsTabDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnStlRatingRivetsTabDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingRivetsTabDlg message handlers

BOOL CDgnStlRatingRivetsTabDlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	InitUnit();

	AlignControls();
	ShowHideControls();

	Data2Dlg();

	EnableDisableControls(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnStlRatingRivetsTabDlg::DestroyWindow()
{
	return CChildDialog::DestroyWindow();
}