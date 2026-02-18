// DgnStlRatingParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnStlRatingParamDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "../wg_db/DataCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_base/DlgUtil.h"

#include "RatingPhiCPGLSD15Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnStlRatingParamDlg::CDgnStlRatingParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlRatingParamDlg::IDD, pParent)
{
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_aLSDCtrl.RemoveAll();
	m_aLSDCtrl.Add(IDC_DGN_STL_RATING_LSD_CALC_GROUP);
	m_aLSDCtrl.Add(IDC_DGN_STL_RATING_LSD_USER_CHECK);
	m_aLSDCtrl.Add(IDC_DGN_STL_RATING_LSD_MRF_BTN);

	m_aOutputCtrl.RemoveAll();
	m_aOutputCtrlASD.RemoveAll();
	//m_aOutputCtrlForce.RemoveAll();
	m_aOutputCtrlDetail.RemoveAll();
	m_aOutputCtrl.Add(IDC_DGN_STL_RATING_OUTPUT_GRP);
	//m_aOutputCtrlForce.Add(IDC_TOTAL_SECTION_FORCE_CHK);
	m_aOutputCtrl.Add(IDC_TOTAL_SECTION_FORCE_CHK);
	m_aOutputCtrlASD.Add(IDC_ALLOWABLE_STRESS_CALC_CHK);
	m_aOutputCtrlDetail.Add(IDC_DETAIL_REPORT_CHK);
	m_aOutputCtrlASD.Add(IDC_SECTION_INPUT_INFO_CHK);
	m_aOutputCtrl.Add(IDC_SEL_ALL_BTN);
	m_aOutputCtrl.Add(IDC_UNSEL_ALL_BTN);

}

CDgnStlRatingParamDlg::~CDgnStlRatingParamDlg()
{

}

void CDgnStlRatingParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingParamDlg)

	
	DDX_Control(pDX, IDC_LIVE_LOAD_CRITERION_LVAL_EDT, m_EdtLiveLoadCriLeft);
	DDX_Control(pDX, IDC_LIVE_LOAD_CRITERION_RVAL_EDT, m_EdtLiveLoadCriRight);

	DDX_Control(pDX, IDC_DGN_STL_RATING_LSD_USER_CHECK, m_chkUserInput);
	DDX_Control(pDX, IDC_TOTAL_SECTION_FORCE_CHK, m_chkTotalSectionForce);
	DDX_Control(pDX, IDC_ALLOWABLE_STRESS_CALC_CHK, m_chkAllowStressCalc);
	DDX_Control(pDX, IDC_SECTION_INPUT_INFO_CHK, m_chkSectionInputInfo);
	DDX_Control(pDX, IDC_DETAIL_REPORT_CHK, m_chkDetailReport);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlRatingParamDlg, CInternationalDlg)
#undef CDialog
	ON_BN_CLICKED(IDC_DGN_STL_RATING_LSD_USER_CHECK,  OnCheckUserInputData)
	ON_BN_CLICKED(IDC_DGN_STL_RATING_LSD_MRF_BTN, OnClickModDgnParam)
	ON_BN_CLICKED(IDC_SEL_ALL_BTN, OnBtnSelAll)
	ON_BN_CLICKED(IDC_UNSEL_ALL_BTN, OnBtnUnSelAll)
	ON_BN_CLICKED(IDOK, &CDgnStlRatingParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDgnStlRatingParamDlg::OnBnClickedCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingParamDlg message handlers


BOOL CDgnStlRatingParamDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetControl();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDS_D tRcds; tRcds.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(tRcds);
	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
	if(!pDoc->m_pAttrCtrl2->GetRpas(m_Data))
	{
		m_Data.Initialize();

		if(tRcds.nRatingCode == STL_KSCE_ASD10)           {m_strLiveLoadType = _T("DB"); m_Data.dDgnLiveLoad = 24.0;} // ???
		else if(tRcds.nRatingCode == STL_KSCE_RAIL_ASD11) {m_strLiveLoadType = _T("LS"); m_Data.dDgnLiveLoad = 22.0;} // ???
		else if(tRcds.nRatingCode == STL_KSCE_LSD15)      {m_strLiveLoadType = _T("KL"); m_Data.dDgnLiveLoad = 510.0;} // ???
		else{ASSERT(0);}
	}

	//	if(tRcds.nRatingCode == STL_KSCE_ASD10)
	//	{
	//		m_Data.strDgnLiveLoad = _T("DB");
	//		m_Data.dDgnLiveLoad = 24;
	//	}	
	//	else if(tRcds.nRatingCode == STL_KSCE_RAIL_ASD11)
	//	{
	//		m_Data.strDgnLiveLoad = _T("LS");
	//		m_Data.dDgnLiveLoad = 22;
	//	}
	//	else if (tRcds.nRatingCode == STL_KSCE_LSD15)
	//	{
	//		m_Data.strDgnLiveLoad = _T("KL");
	//		m_Data.dDgnLiveLoad = 510.0;
	//	}
	//	else
	//	{
	//		ASSERT(0);
	//		m_Data.strDgnLiveLoad = _T("DB");
	//		m_Data.dDgnLiveLoad = 24;
	//	}
	//}

	if (tRcds.nRatingCode == STL_KSCE_ASD10 || tRcds.nRatingCode == STL_KSCE_RAIL_ASD11)
	{		
		CDlgUtil::CtrlShowHide(this, m_aLSDCtrl, FALSE);

		GetDlgItem(IDC_DGN_STL_RATING_LSD_CALC_GROUP)->GetWindowRect(&RectOrg);

		CRect rTarget, rSource;
		GetDlgItem(IDC_DGN_STL_RATING_LSD_CALC_GROUP)->GetWindowRect(rSource);    
		//CDlgUtil::CtrlShowHide(this, m_aLSDCtrl , TRUE );
		GetDlgItem(IDC_DGN_STL_RATING_OUTPUT_GRP)->GetWindowRect(rSource);
		GetDlgItem(IDC_DGN_STL_RATING_LSD_CALC_GROUP)->GetWindowRect(rTarget);
		int nDistY = rTarget.top - rSource.top;
		CDlgUtil::CtrlMoveDistY(this, m_aOutputCtrl, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aOutputCtrlASD, nDistY);
		//CDlgUtil::CtrlMoveDistY(this, m_aOutputCtrlForce, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aOutputCtrlDetail, nDistY);
	}

	else if (tRcds.nRatingCode == STL_KSCE_LSD15)
	{
		CDlgUtil::CtrlShowHide(this, m_aLSDCtrl,	TRUE);
		CDlgUtil::CtrlShowHide(this, m_aOutputCtrlASD, FALSE);

		GetDlgItem(IDC_ALLOWABLE_STRESS_CALC_CHK)->GetWindowRect(&RectOrg);

		CRect rTarget, rSource;
		GetDlgItem(IDC_ALLOWABLE_STRESS_CALC_CHK)->GetWindowRect(rSource);    
		//CDlgUtil::CtrlShowHide(this, m_aLSDCtrl , TRUE );
		GetDlgItem(IDC_DETAIL_REPORT_CHK)->GetWindowRect(rSource);
		GetDlgItem(IDC_ALLOWABLE_STRESS_CALC_CHK)->GetWindowRect(rTarget);
		int nDistY = (rTarget.top - rSource.top);
		CDlgUtil::CtrlMoveDistY(this, m_aOutputCtrlDetail, nDistY);
	}
	else{ASSERT(0);}

	GetDlgItem(IDC_SEL_ALL_BTN)->GetWindowRect(&RectOrg);

	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right		= RectDlg.right;
	rDlg.bottom		= RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(32); //(int)(2.5*RectOk.Height());//2.5
	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8); //(int)(1.75*RectOk.Height());//1.75
	rOk.bottom		= rOk.top + RectOk.Height();
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

	if(tRcds.nRatingCode == STL_KSCE_ASD10)           {m_strLiveLoadType = _T("DB"); m_Data.dDgnLiveLoad = 24.0;} // ???
	else if(tRcds.nRatingCode == STL_KSCE_RAIL_ASD11) {m_strLiveLoadType = _T("LS"); m_Data.dDgnLiveLoad = 22.0;} // ???
	else if(tRcds.nRatingCode == STL_KSCE_LSD15)      {m_strLiveLoadType = _T("KL"); m_Data.dDgnLiveLoad = 510.0;} // ???
	else{ASSERT(0);}


	Data2Dlg();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingParamDlg::SetControl() 
{
	m_aSurveyMethodRdo.RemoveAll();
	m_aSurveyMethodRdo.Add(IDC_DEFLCTION_RDO);
	m_aSurveyMethodRdo.Add(IDC_STAIN_RDO);
	
	m_EdtLiveLoadCriRight.SetUnitType(D_UNITSYS_NONE);
}

BOOL CDgnStlRatingParamDlg::Dlg2Data()
{  
	UpdateData(TRUE);
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDS_D tRcds; tRcds.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(tRcds);

	CDlgUtil::CtrlRadioGetCheck(this, m_aSurveyMethodRdo, m_Data.nSurveyMeth);
	
	m_EdtLiveLoadCriLeft.GetWindowText(m_Data.strDgnLiveLoad);
	m_Data.dDgnLiveLoad = m_EdtLiveLoadCriRight.GetEditValue();
	
	m_Data.bPrintParm[0] = m_chkTotalSectionForce.GetCheck();
	m_Data.bPrintParm[1] = m_chkAllowStressCalc.GetCheck();
	m_Data.bPrintParm[2] = m_chkSectionInputInfo.GetCheck();
	m_Data.bPrintParm[3] = m_chkDetailReport.GetCheck();

	if (tRcds.nRatingCode == STL_KSCE_LSD15)
	{
		m_Data.bPrintParm[1] = FALSE;
		m_Data.bPrintParm[2] = FALSE;
	}

	if(m_chkUserInput.GetCheck())//사용자
		m_Data.bUserInputData = TRUE;
	else if(!m_chkUserInput.GetCheck())//사용자
	{
		m_Data.bUserInputData = FALSE;
		m_Data.Set_STL_Rating_AnnexData_LSD();
	}
	else
		ASSERT(0);

	return TRUE;
}

void CDgnStlRatingParamDlg::Data2Dlg()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDS_D tRcds; tRcds.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(tRcds);

	CDlgUtil::CtrlRadioSetCheck(this, m_aSurveyMethodRdo, m_Data.nSurveyMeth);
	
	m_EdtLiveLoadCriLeft.SetWindowText(m_strLiveLoadType);
	m_EdtLiveLoadCriRight.SetEditUnit(m_Data.dDgnLiveLoad);

	m_chkTotalSectionForce.SetCheck(m_Data.bPrintParm[0]);
	m_chkAllowStressCalc.SetCheck(m_Data.bPrintParm[1]);
	m_chkSectionInputInfo.SetCheck(m_Data.bPrintParm[2]);
	m_chkDetailReport.SetCheck(m_Data.bPrintParm[3]);

	if (tRcds.nRatingCode == STL_KSCE_LSD15)
	{
		m_chkAllowStressCalc.SetCheck(FALSE);
		m_chkSectionInputInfo.SetCheck(FALSE);
	}

	if(m_Data.bUserInputData)//사용자 입력
	{
		m_chkUserInput.SetCheck(TRUE);
	}
	else
	{
		m_chkUserInput.SetCheck(FALSE);
		m_Data.Set_STL_Rating_AnnexData_LSD();
	}

	GetDlgItem(IDC_DGN_STL_RATING_LSD_MRF_BTN)->EnableWindow(m_Data.bUserInputData);

	UpdateData(FALSE);
}

void CDgnStlRatingParamDlg::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_chkUserInput.GetCheck();

	GetDlgItem(IDC_DGN_STL_RATING_LSD_MRF_BTN)->EnableWindow(bChk);

	m_Data.bUserInputData = bChk;
}


void CDgnStlRatingParamDlg::OnClickModDgnParam()
{
	CRatingPhiCPGLSD15Dlg dlg;
	dlg.m_Data  = m_Data;
	if(dlg.DoModal()==IDOK)
	{
		m_Data = dlg.m_Data;
	}
}

void CDgnStlRatingParamDlg::OnBnClickedCancel() 
{	
	CDialogMove::OnCancel();
}

//void CDgnStlRatingParamDlg::Execute()
//{	
//	// DB에 Add	
//	m_pDoc->m_pDataCtrl->AddRpas(m_tData);
//}

void CDgnStlRatingParamDlg::OnBnClickedOk() 
{
	//if(Dlg2Data())
	//{
	//	Execute();
	//	CDialogMove::OnOK();
	//}	
	Dlg2Data();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pDataCtrl->AddRpas(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CDgnStlRatingParamDlg::OnBtnSelAll() 
{
	m_chkTotalSectionForce.SetCheck(TRUE);
	m_chkAllowStressCalc.SetCheck(TRUE);
	m_chkSectionInputInfo.SetCheck(TRUE);
	m_chkDetailReport.SetCheck(TRUE);
}

void CDgnStlRatingParamDlg::OnBtnUnSelAll() 
{
	m_chkTotalSectionForce.SetCheck(FALSE);
	m_chkAllowStressCalc.SetCheck(FALSE);
	m_chkSectionInputInfo.SetCheck(FALSE);
	m_chkDetailReport.SetCheck(FALSE);
}
