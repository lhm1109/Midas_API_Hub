// DgnRcRatingParamDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRcRatingParamDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_main\wg_main.h"

#include "RatingPhiRCLSD15Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDgnRcRatingParamDlg 대화 상자입니다.

//IMPLEMENT_DYNAMIC(CDgnRcRatingParamDlg, CDialogEx)

CDgnRCRatingParamDlg::CDgnRCRatingParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRCRatingParamDlg::IDD, pParent)
{
	m_nMeasureType = 0;
	m_dLiveLoadStaType = 0.0;
	m_dLiveLoadStaNum = 0.0;

	m_bSectForceRpt = TRUE;
	m_bStrengthCheckRpt = TRUE;
	m_strLiveLoadType = _T("");
	m_Data.Initialize();

	m_aLSDCtrl.RemoveAll();
	m_aLSDCtrl.Add(IDC_DGN_CALC_GROUP);
	m_aLSDCtrl.Add(IDC_DGN_LSDRC_RATING_DESIGN_USER_CHECK);
	m_aLSDCtrl.Add(IDC_DGN_LSDRC_RATING_DESIGN_MRF_BTN);

	m_aDefaultOutputCtrl.RemoveAll();
	m_aDefaultOutputCtrl.Add(IDC_RC_RATING_OUTPUT_GRP);
	m_aDefaultOutputCtrl.Add(IDC_DGN_RC_RATING_PARAM_SECT_FORCE_CHK);
	m_aDefaultOutputCtrl.Add(IDC_DGN_RC_RATING_PARAM_STRENGTH_CHK);

}

CDgnRCRatingParamDlg::~CDgnRCRatingParamDlg()
{
}

void CDgnRCRatingParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Radio  (pDX, IDC_DGN_RC_RATING_DISP_RDO,             m_nMeasureType);
	DDX_Control(pDX, IDC_DGN_RC_RATIGN_PARAM_LLS_STR_EDT,    m_edtLiveLoadStaType);
	DDX_Control(pDX, IDC_DGN_RC_RATIGN_PARAM_LLS_NUM_EDT,    m_edtLiveLoadStaNum);
	DDX_Control(pDX, IDC_DGN_LSDRC_RATING_DESIGN_USER_CHECK, m_chkUserInput);
	DDX_Check  (pDX, IDC_DGN_RC_RATING_PARAM_SECT_FORCE_CHK, m_bSectForceRpt);
	DDX_Check  (pDX, IDC_DGN_RC_RATING_PARAM_STRENGTH_CHK,   m_bStrengthCheckRpt);
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnRCRatingParamDlg, CInternationalDlg)
	#undef CDialog
	ON_BN_CLICKED(IDC_DGN_LSDRC_RATING_DESIGN_USER_CHECK,  OnCheckUserInputData)
	ON_BN_CLICKED(IDC_DGN_LSDRC_RATING_DESIGN_MRF_BTN, OnClickModDgnParam)
	ON_BN_CLICKED(IDOK, &CDgnRCRatingParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDgnRCRatingParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CDgnRCRatingParamDlg::InitUnit()
{
	m_edtLiveLoadStaType.SetUnitType(D_UNITSYS_NONE);
	m_edtLiveLoadStaNum.SetUnitType(D_UNITSYS_NONE);
}

void CDgnRCRatingParamDlg::ControlsEnableDisable()
{

}

void CDgnRCRatingParamDlg::Data2Dlg()
{
	m_nMeasureType      = m_Data.nSurveyMeth;
	m_edtLiveLoadStaType.SetWindowText(m_strLiveLoadType);
	m_edtLiveLoadStaNum.SetEditUnit(m_Data.dDgnLiveLoad);
	m_bSectForceRpt     = m_Data.bPrintParm[0];
	m_bStrengthCheckRpt = m_Data.bPrintParm[1];

	if(m_Data.bUserInputData)//사용자 입력
	{
		m_chkUserInput.SetCheck(TRUE);
	}
	else
	{
		m_chkUserInput.SetCheck(FALSE);
		m_Data.Set_RC_Rating_AnnexData_LSD();
	}
	GetDlgItem(IDC_DGN_LSDRC_RATING_DESIGN_MRF_BTN)->EnableWindow(m_Data.bUserInputData);

	UpdateData(FALSE);
}

BOOL CDgnRCRatingParamDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nSurveyMeth  = m_nMeasureType;
	m_Data.dDgnLiveLoad = m_edtLiveLoadStaNum.GetEditValue();
	m_edtLiveLoadStaType.GetWindowText(m_Data.strDgnLiveLoad);  
	m_Data.bPrintParm[0] = m_bSectForceRpt;
	m_Data.bPrintParm[1] = m_bStrengthCheckRpt;

	if(m_chkUserInput.GetCheck())//사용자
		m_Data.bUserInputData = TRUE;
	else if(!m_chkUserInput.GetCheck())//사용자
	{
		m_Data.bUserInputData = FALSE;
		m_Data.Set_RC_Rating_AnnexData_LSD();
	}
	else
		ASSERT(0);

	return TRUE;
}

void CDgnRCRatingParamDlg::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_chkUserInput.GetCheck();

	GetDlgItem(IDC_DGN_LSDRC_RATING_DESIGN_MRF_BTN)->EnableWindow(bChk);

	m_Data.bUserInputData = bChk;
}

void CDgnRCRatingParamDlg::OnClickModDgnParam()
{
	CRatingPhiRCLSD15Dlg dlg;
	dlg.m_Data  = m_Data;
	if(dlg.DoModal()==IDOK)
	{
		m_Data = dlg.m_Data;
	}
}


BOOL CDgnRCRatingParamDlg::GetLiveLoadTypeName()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDC_D DgnCodeD; DgnCodeD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcdc(DgnCodeD);
	if     (DgnCodeD.nRatingCode == KSCE_USD10_RC_RATING)     {m_strLiveLoadType = _T("DB");}  // ???
	else if(DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RC_RATING){m_strLiveLoadType = _T("LS");}  // ???
	else if(DgnCodeD.nRatingCode == KSCE_LSD15_RC_RATING)     {m_strLiveLoadType = _T("KL");}  // ???
	else{ASSERT(0);}

	return TRUE;
}

BOOL CDgnRCRatingParamDlg::InitCtrl()
{

	return TRUE;
}

BOOL CDgnRCRatingParamDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

 //CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCDC_D DgnCodeD; DgnCodeD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcdc(DgnCodeD);
	if(!pDoc->m_pAttrCtrl2->GetRpac(m_Data))
	{
		m_Data.Initialize();
	 // CDBDoc* pDoc = CDBDoc::GetDocPoint();
	 // T_RCDC_D DgnCodeD; DgnCodeD.Initialize();
	 //pDoc->m_pAttrCtrl2->GetDgnRcdc(DgnCodeD);
				 if(DgnCodeD.nRatingCode == KSCE_USD10_RC_RATING)           {m_strLiveLoadType = _T("DB"); m_Data.dDgnLiveLoad = 24.0;} // ???
		else if(DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RC_RATING)      {m_strLiveLoadType = _T("LS"); m_Data.dDgnLiveLoad = 22.0;} // ???
		else if(DgnCodeD.nRatingCode == KSCE_LSD15_RC_RATING)           {m_strLiveLoadType = _T("KL"); m_Data.dDgnLiveLoad = 510.0;} // ???
		else{ASSERT(0);}
	}

	if (DgnCodeD.nRatingCode == KSCE_LSD15_RC_RATING)
	{		
		GetDlgItem(IDC_DGN_CALC_GROUP)										->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_LSDRC_RATING_DESIGN_USER_CHECK)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_LSDRC_RATING_DESIGN_MRF_BTN)		->ShowWindow(TRUE);
	}
	else if (DgnCodeD.nRatingCode == KSCE_USD10_RC_RATING || DgnCodeD.nRatingCode == KSCE_RAIL_USD11_RC_RATING)
	{
		CRect RectDlg, RectOk, RectCn, RectOrg;
		GetWindowRect(&RectDlg);
		GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
		GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
		CDlgUtil::CtrlShowHide(this, m_aLSDCtrl, FALSE);

		GetDlgItem(IDC_DGN_CALC_GROUP)->GetWindowRect(&RectOrg);

		CRect rTarget, rSource;
		GetDlgItem(IDC_DGN_CALC_GROUP)->GetWindowRect(rSource);    
		//CDlgUtil::CtrlShowHide(this, m_aLSDCtrl , TRUE );
		GetDlgItem(IDC_RC_RATING_OUTPUT_GRP)->GetWindowRect(rSource);
		GetDlgItem(IDC_DGN_CALC_GROUP)->GetWindowRect(rTarget);
		int nDistY = rTarget.top - rSource.top;
		CDlgUtil::CtrlMoveDistY(this, m_aDefaultOutputCtrl, nDistY);

		GetDlgItem(IDC_RC_RATING_OUTPUT_GRP)->GetWindowRect(&RectOrg);
		CRect rDlg;
		rDlg.left		= RectDlg.left;
		rDlg.top		= RectDlg.top;
		rDlg.right		= RectDlg.right;
		rDlg.bottom		= RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24); //(int)(2.5*RectOk.Height());//2.5
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
	
	}
	else{ASSERT(0);}

	GetLiveLoadTypeName();
	InitUnit();	
	Data2Dlg();

	UpdateData(FALSE);

	ControlsEnableDisable();

	return TRUE;
}
// CDgnRcRatingParamDlg 메시지 처리기입니다.

void CDgnRCRatingParamDlg::OnBnClickedOk()
{
		Dlg2Data();

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if(pDoc->m_pDataCtrl->AddRpac(m_Data))
		{
			CDialogMove::OnOK();
		}
}


void CDgnRCRatingParamDlg::OnBnClickedCancel()
{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogMove::OnCancel();
}
