// DgnGenFrameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenFrameDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenFrameDlg dialog


CDgnGenFrameDlg::CDgnGenFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenFrameDlg)
	m_bAutoK = FALSE;
	m_XBraced = -1;
	m_YBraced = -1;
	m_DgnType = -1;
	m_b02Q0  = FALSE;
	//}}AFX_DATA_INIT
}


void CDgnGenFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenFrameDlg)
	DDX_Check(pDX, IDC_DGN_CTRLDATA_AUTOK, m_bAutoK);
	DDX_Radio(pDX, IDC_DGN_CTRLDATA_XUNBRACED, m_XBraced);
	DDX_Radio(pDX, IDC_DGN_CTRLDATA_YUNBRACED, m_YBraced);
	DDX_Radio(pDX, IDC_DGN_CTRLDATA_3D, m_DgnType);
	DDX_Control(pDX, IDC_DGN_MTYP_CMB, m_cmbMtyp);
	DDX_Control(pDX, IDC_DGN_STYP_CMB, m_cmbStyp);
	DDX_Check(pDX,   IDC_CHECK_02QOUPLIMIT, m_b02Q0);	//add by maxiao
	DDX_Text(pDX, IDC_EDIT_02QOUPLIMIT, m_d02Q0);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenFrameDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnGenFrameDlg)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_XBRACED, OnDgnCtrldataXbraced)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_XUNBRACED, OnDgnCtrldataXunbraced)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_YBRACED, OnDgnCtrldataYbraced)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_YUNBRACED, OnDgnCtrldataYunbraced)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_AUTOK, OnDgnCtrldataAutok)
	ON_CBN_SELCHANGE(IDC_DGN_STYP_CMB, OnSelChangeStypCmb)//add by maxiao
	ON_BN_CLICKED(IDC_CHECK_02QOUPLIMIT, OnDgnCtrlCheck02Q0)//add by maxiao
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenFrameDlg message handlers

BOOL CDgnGenFrameDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// Initialize Default
	if(!m_pDoc->m_pAttrCtrl->ExistDctl()) Initial_Data();
	else                                  Initial_ExistData();

	// Add by ZINU.('02.6.13).
	// Change, Jaeoh.('08.01.10). 비활성화 품. 김문성 GJ님 요청.
/*
#if defined(_CIVIL)
	GetDlgItem(IDC_DGN_CTRLDATA_AUTOK)->EnableWindow(FALSE);
	m_bAutoK = FALSE;
#else
	GetDlgItem(IDC_DGN_CTRLDATA_AUTOK)->EnableWindow(TRUE);
#endif
	*/

	return TRUE;
}

void CDgnGenFrameDlg::Initial_Data()
{
	SetMtypCombo();
	SetStypCombo_GB10(0);
	SetStructTypeControl();

	T_DCTL_D cData;
	cData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDctl())  
	{
		m_pDoc->m_pAttrCtrl->GetDctl(cData);

		m_XBraced = cData.bBraceFrame_X;
		m_YBraced = cData.bBraceFrame_Y;
		m_DgnType = cData.nDesignType;
		m_bAutoK  = cData.bAutoKFactor;
		m_cmbMtyp.SetCurSel(cData.nStructMatlType);
		m_cmbStyp.SetCurSel(cData.nStructType);
		m_b02Q0   = cData.b02Q0UpLimit;//add by maxiao
		m_d02Q0		= cData.d02Q0UpLimit;
	}
	else
	{
	  m_bAutoK = FALSE;
#if defined(_MGEN)
	  m_XBraced = 1;
	  m_YBraced = 1;
#else
		m_XBraced = 0;
		m_YBraced = 0;
#endif
		m_DgnType = 0;    
		m_cmbMtyp.SetCurSel(0);
		m_cmbStyp.SetCurSel(1);
		m_b02Q0 = FALSE;
		m_d02Q0 = 2;
	}
	GetDlgItem(IDC_EDIT_02QOUPLIMIT)->EnableWindow(m_b02Q0);
	UpdateData(FALSE);
}

void CDgnGenFrameDlg::Initial_ExistData()
{
	SetMtypCombo();
	SetStypCombo_GB10(0);
	SetStructTypeControl();

	T_DCTL_D cData; cData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDctl(cData);

	m_XBraced = cData.bBraceFrame_X;
	m_YBraced = cData.bBraceFrame_Y;
	m_bAutoK  = cData.bAutoKFactor;
	m_DgnType = cData.nDesignType;
	
	m_cmbMtyp.SetCurSel(cData.nStructMatlType);
	m_cmbStyp.SetCurSel(cData.nStructType);
	m_b02Q0 = cData.b02Q0UpLimit;
	m_d02Q0 = cData.d02Q0UpLimit;
	GetDlgItem(IDC_EDIT_02QOUPLIMIT)->EnableWindow(m_b02Q0);
	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnOK() 
{
	UpdateData(TRUE);

	T_DCTL_D cData;
	cData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDctl())  m_pDoc->m_pAttrCtrl->GetDctl(cData);

	cData.bAutoKFactor  = m_bAutoK;
	cData.bBraceFrame_X = m_XBraced;
	cData.bBraceFrame_Y = m_YBraced;
	cData.nDesignType   = m_DgnType;

	cData.nStructMatlType = m_cmbMtyp.GetCurSel();
	cData.nStructType     = m_cmbStyp.GetCurSel();
	cData.b02Q0UpLimit = m_b02Q0;
	cData.d02Q0UpLimit = m_d02Q0;
	if (m_b02Q0 && m_d02Q0 < 1. )
	{
		AfxMessageBox(_T("Error:0.2Q0 < 1.0!"));
		return;
	}

	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDctl(cData);
	//ASSERT(bCheck);
	if(bCheck)  CDialogMove::OnOK();
}

void CDgnGenFrameDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnGenFrameDlg::OnDgnCtrldataXbraced() 
{
//	m_XBraced = 1;
//	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnDgnCtrldataXunbraced() 
{
//	m_XBraced = 0;
//	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnDgnCtrldataYbraced() 
{
//	m_YBraced = 1;
//	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnDgnCtrldataYunbraced() 
{
//	m_YBraced = 0;
//	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnDgnCtrldataAutok() 
{
//	if(m_bAutoK)  m_bAutoK = FALSE;
 // else          m_bAutoK = TRUE;

 // UpdateData(FALSE);
}
void CDgnGenFrameDlg::OnSelChangeStypCmb()//add by maxiao
{
	m_b02Q0 = (m_cmbMtyp.GetCurSel() ==0 && m_cmbStyp.GetCurSel()==1);
	GetDlgItem(IDC_CHECK_02QOUPLIMIT)->EnableWindow(m_b02Q0);
	GetDlgItem(IDC_EDIT_02QOUPLIMIT)->EnableWindow(m_b02Q0);
	UpdateData(FALSE);
}

void CDgnGenFrameDlg::OnDgnCtrlCheck02Q0()//add by maxiao
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_02QOUPLIMIT)->EnableWindow(m_b02Q0);
	UpdateData(FALSE);
}
void CDgnGenFrameDlg::SetMtypCombo()
{
	m_cmbMtyp.ResetContent();

	int nItem = 1;
	CDlgUtil::CobxAddItem(m_cmbMtyp, _LS(IDS_DGN_GEN_STRUCTMATL_RC),           nItem++);
	CDlgUtil::CobxAddItem(m_cmbMtyp, _LS(IDS_DGN_GEN_STRUCTMATL_COMBINED),     nItem++);
	CDlgUtil::CobxAddItem(m_cmbMtyp, _LS(IDS_DGN_GEN_STRUCTMATL_STL_WALL),     nItem++);
	CDlgUtil::CobxAddItem(m_cmbMtyp, _LS(IDS_DGN_GEN_STRUCTMATL_STL_WALL_NOT), nItem++);

	m_cmbMtyp.SetCurSel(0);
	
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbMtyp);

	GetDlgItem(IDC_DGN_MTYP_CMB)->EnableWindow(FALSE);
}

void CDgnGenFrameDlg::SetStypCombo_GB10(int nStructMatlType)
{
	m_cmbStyp.ResetContent();

	int nItem = 1;
	if(nStructMatlType==0) // RC조.
	{
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_FRAME),           nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_FRAME_SHEARWALL), nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_SHEARWALL),       nItem++);
	}
	else if(nStructMatlType==1) // SRC조.
	{
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_RC_TUBE),     nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_RC_TUBE),     nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_OUT_RC_TUBE), nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_OUT_RC_IN),   nItem++);
	}
	else if(nStructMatlType==2 || nStructMatlType==3) // 비내력벽이 있는/없는 S조.
	{
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_FRAME),            nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_FRAME_CENT_BRACE), nItem++);
		CDlgUtil::CobxAddItem(m_cmbStyp, _LS(IDS_DGN_GEN_STRUCT_STL_FRAME_ECCN_BRACE), nItem++);
	}
	else ASSERT(0);

	m_cmbStyp.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStyp);
}

void CDgnGenFrameDlg::SetStructTypeControl()
{
	T_DCON_D DconD;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();
	
	BOOL bNewCodeGB = FALSE;
	if (DconD.DesignCode==_T("GB/T50010-10")) bNewCodeGB = TRUE;
	if (CProduct::GetMovingType()==D_PRODUCT_MOVING_CH && DconD.DesignCode==_T("")) bNewCodeGB = TRUE;
	
	GetDlgItem(IDC_DGN_STYP_GROUP_STATIC)->ShowWindow(bNewCodeGB);
	GetDlgItem(IDC_DGN_MTYP_STATIC)->ShowWindow(bNewCodeGB);
	GetDlgItem(IDC_DGN_STYP_STATIC)->ShowWindow(bNewCodeGB);
	GetDlgItem(IDC_DGN_MTYP_CMB)->ShowWindow(bNewCodeGB);
	GetDlgItem(IDC_DGN_STYP_CMB)->ShowWindow(bNewCodeGB);

	CRect RectDlg, RectOk, RectCn, RectKf, RectGp, Rect02Q0;
	GetWindowRect(&RectDlg);  
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);  
	GetDlgItem(IDC_DGN_CTRLDATA_AUTOK)->GetWindowRect(&RectKf);
	GetDlgItem(IDC_DGN_STYP_GROUP_STATIC)->GetWindowRect(&RectGp);
	GetDlgItem(IDC_EDIT_02QOUPLIMIT)->GetWindowRect(&Rect02Q0);//add by maxiao
	
	int nStypGroupHeight = bNewCodeGB? RectGp.Height() + 2*Rect02Q0.Height() : 0;
	
	CRect rDlg;
	rDlg.left	= RectDlg.left;
	rDlg.top	= RectDlg.top;
	rDlg.right	= RectDlg.right;
	rDlg.bottom	= RectKf.bottom + globalUtils.ScaleByDPI(20) + RectOk.Height() + nStypGroupHeight;
	
	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= rDlg.bottom - globalUtils.ScaleByDPI(8) - RectOk.Height();
	rOk.bottom   	= rOk.top + RectOk.Height();
	
	CRect rCn;
	rCn.left		= RectCn.left;
	rCn.right		= RectCn.right;
	rCn.top			= rOk.top;
	rCn.bottom		= rOk.bottom;
	
	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK    )->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);  
}