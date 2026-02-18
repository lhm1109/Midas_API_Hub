// DgnStlRatingServiceParamTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingServiceParamTabJDlg.h"
#include "DgnStlRatingServiceParamDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingServiceParamTabJDlg dialog


CDgnStlRatingServiceParamTabJDlg::CDgnStlRatingServiceParamTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnStlRatingServiceParamTabJDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlRatingServiceParamTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CDgnStlRatingServiceParamDlg*)pParent;

	m_Data.Initialize();
	m_Data.dTopFlange = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_STRESS, m_Data.dTopFlange);
	m_Data.dBotFlange = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_STRESS, m_Data.dBotFlange);


	m_pDoc = CDBDoc::GetDocPoint();

	arWarpingCtrlID.RemoveAll();
	arWarpingCtrlID.Add(IDC_DGN_STL_RATING_SERVICE_TOP_EDT);
	arWarpingCtrlID.Add(IDC_DGN_STL_RATING_SERVICE_TOP_UNT);
	arWarpingCtrlID.Add(IDC_DGN_STL_RATING_SERVICE_BOT_EDT);
	arWarpingCtrlID.Add(IDC_DGN_STL_RATING_SERVICE_BOT_UNT);
}

void CDgnStlRatingServiceParamTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingServiceParamTabJDlg)
	DDX_Control(pDX, IDC_DGN_STL_RATING_SERVICE_TOP_EDT, m_edtTopFlange);
	DDX_Control(pDX, IDC_DGN_STL_RATING_SERVICE_TOP_UNT, m_untTopFlange);
	DDX_Control(pDX, IDC_DGN_STL_RATING_SERVICE_BOT_EDT, m_edtBotFlange);
	DDX_Control(pDX, IDC_DGN_STL_RATING_SERVICE_BOT_UNT, m_untBotFlange);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlRatingServiceParamTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnStlRatingServiceParamTabJDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingServiceParamTabJDlg message handlers

BOOL CDgnStlRatingServiceParamTabJDlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	InitUnit();

	SetText();

	Data2Dlg();

	ControlsEnableDisable(m_pParent->m_bBothIJ);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnStlRatingServiceParamTabJDlg::DestroyWindow()
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}

void CDgnStlRatingServiceParamTabJDlg::InitUnit()
{
	m_edtTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);

}

void CDgnStlRatingServiceParamTabJDlg::Data2Dlg()
{
	m_edtTopFlange.SetEditUnit(m_Data.dTopFlange);
	m_edtBotFlange.SetEditUnit(m_Data.dBotFlange);

	UpdateData(FALSE);
}

BOOL CDgnStlRatingServiceParamTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dTopFlange = m_edtTopFlange.GetEditValue();
	m_Data.dBotFlange = m_edtBotFlange.GetEditValue();

	return TRUE;
}

void CDgnStlRatingServiceParamTabJDlg::SetText()
{
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_WARPING_GRP)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_TAB_WARPING_GRP));
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_TOP_STC)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_TAB_TOP_F));
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_BOT_STC)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_TAB_BOT_F));
}

void CDgnStlRatingServiceParamTabJDlg::GetSrspData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CDgnStlRatingServiceParamTabJDlg::SetSrspData()
{
	Data2Dlg();
}

void CDgnStlRatingServiceParamTabJDlg::ControlsEnableDisable(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_TOP_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_TOP_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_TOP_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_BOT_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_BOT_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_RATING_SERVICE_BOT_UNT)->EnableWindow(bEnable);

	this->RedrawWindow();
}
