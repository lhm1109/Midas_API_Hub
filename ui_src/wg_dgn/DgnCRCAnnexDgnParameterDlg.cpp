// DgnCRCAnnexDgnParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnCRCAnnexDgnParameterDlg.h"

#include "DgnStruct.h"
//#include "DgnPscCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_base\MSExcel.h"
#include "..\wg_base\XLParser.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dgn::def;

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog
CDgnCRCAnnexDgnParameterDlg::CDgnCRCAnnexDgnParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCRCAnnexDgnParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	//}}AFX_DATA_INIT
}

void CDgnCRCAnnexDgnParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER3, m_dPartialFac_c_PT);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER5, m_dPartialFac_s_PT);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER7, m_dPartialFac_ps_PT);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER10, m_dPartialFac_c_A);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER12, m_dPartialFac_s_A);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER14, m_dPartialFac_ps_A);

	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER17, m_dPartialFac_c_S);
	DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER19, m_dPartialFac_s_S);
	//DDX_Control(pDX, IDC_EC2PSC_PARTIAL_PARAMETER21,    m_dPartialFac_ps_S);
	//
	DDX_Control(pDX, IDC_EC2PSC_ALPHA_PARAMETER2, m_dAlpha_cc);
	DDX_Control(pDX, IDC_EC2PSC_ALPHA_PARAMETER6, m_dAlpha_cc_4Shear);
	DDX_Control(pDX, IDC_EC2PSC_ALPHA_PARAMETER4, m_dAlpha_ct);
	//
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER3, m_dStressLim_c_k1);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER5, m_dStressLim_c_k3);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER7, m_dStressLim_c_k4);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER9, m_dStressLim_c_k6);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER12, m_dStressLim_s_k1);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER14, m_dStressLim_s_k2);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER16, m_dStressLim_s_k5);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER18, m_dStressLim_s_k7);
	DDX_Control(pDX, IDC_EC2PSC_STRESSLIM_PARAMETER20, m_dStressLim_s_k8);
	//
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER3, m_dk_c_ps_stage);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER5, m_dk_t_ps_stage);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER8, m_dk_c_ps);
	DDX_Control(pDX, IDC_EC2PSC_PRINCIPAL_PARAMETER10, m_dk_t_ps);
	//
	DDX_Control(pDX, IDC_EC2PSC_CRACK_PARAMETER2, m_dk3_cr);
	DDX_Control(pDX, IDC_EC2PSC_CRACK_PARAMETER4, m_dk4_cr);
	//
	DDX_Control(pDX, IDC_EC2PSC_FATIGUE_PARAMETER_K1_EDT, m_edtk1_Fatigue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnCRCAnnexDgnParameterDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_BN_CLICKED(IDC_EC2PSC_CODE_PARAMETER3, OnResetDgnParam_byAnnex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnCRCAnnexDgnParameterDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	// v835 에는 막고 나가자.
	GetDlgItem(IDC_EC2PSC_FATIGUE_PARAMETER_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EC2PSC_FATIGUE_PARAMETER_K1_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EC2PSC_FATIGUE_PARAMETER_K1_EDT)->ShowWindow(SW_HIDE);
	if (m_pParamData->iDgnCode == IRC_112_2011_PSC || m_pParamData->iDgnCode == IRS_PSC ||
		m_pParamData->iDgnCode == IRC_112_2020_PSC)
	{
		GetDlgItem(IDC_EC2PSC_STRESSLIM_PARAMETER7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EC2PSC_STRESSLIM_PARAMETER12)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EC2PSC_STRESSLIM_PARAMETER7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EC2PSC_STRESSLIM_PARAMETER12)->EnableWindow(TRUE);
	}

	SetAlphaCtrl();

	return TRUE;
}

void CDgnCRCAnnexDgnParameterDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnCRCAnnexDgnParameterDlg::OnOK()
{
	UpdateData(TRUE);

	Dlg2Data();

	CDialogMove::OnOK();
}

void CDgnCRCAnnexDgnParameterDlg::Dlg2Data()
{
	m_pParamData->dGamma_c_PT = m_dPartialFac_c_PT.GetEditValue();
	m_pParamData->dGamma_c_A = m_dPartialFac_c_A.GetEditValue();
	m_pParamData->dGamma_c_serv = m_dPartialFac_c_S.GetEditValue();
	m_pParamData->dGamma_s_PT = m_dPartialFac_s_PT.GetEditValue();
	m_pParamData->dGamma_s_A = m_dPartialFac_s_A.GetEditValue();
	m_pParamData->dGamma_s_serv = m_dPartialFac_s_S.GetEditValue();
	m_pParamData->dGamma_ps_PT = m_dPartialFac_ps_PT.GetEditValue();
	m_pParamData->dGamma_ps_A = m_dPartialFac_ps_A.GetEditValue();
	//m_pParamData->dGamma_ps_serv = m_dPartialFac_ps_S.GetEditValue();
	//
	m_pParamData->dAlpha_cc = m_dAlpha_cc.GetEditValue();
	m_pParamData->dAlphacc4Shear = m_dAlpha_cc_4Shear.GetEditValue();
	m_pParamData->dAlpha_ct = m_dAlpha_ct.GetEditValue();
	//
	m_pParamData->dk1_sl = m_dStressLim_c_k1.GetEditValue();
	m_pParamData->dk3_sl = m_dStressLim_c_k3.GetEditValue();
	m_pParamData->dk4_sl = m_dStressLim_c_k4.GetEditValue();
	m_pParamData->dk6_sl = m_dStressLim_c_k6.GetEditValue();
	m_pParamData->dk1_ps = m_dStressLim_s_k1.GetEditValue();
	m_pParamData->dk2_ps = m_dStressLim_s_k2.GetEditValue();
	m_pParamData->dk5_ps = m_dStressLim_s_k5.GetEditValue();
	m_pParamData->dk7_ps = m_dStressLim_s_k7.GetEditValue();
	m_pParamData->dk8_ps = m_dStressLim_s_k8.GetEditValue();
	//
	m_pParamData->dk_c_ps_stage = m_dk_c_ps_stage.GetEditValue();
	m_pParamData->dk_t_ps_stage = m_dk_t_ps_stage.GetEditValue();
	m_pParamData->dk_c_ps = m_dk_c_ps.GetEditValue();
	m_pParamData->dk_t_ps = m_dk_t_ps.GetEditValue();
	//
	m_pParamData->dk3_cr = m_dk3_cr.GetEditValue();
	m_pParamData->dk4_cr = m_dk4_cr.GetEditValue();
	//
	m_pParamData->dk1_Fatigue = m_edtk1_Fatigue.GetEditValue();
}

void CDgnCRCAnnexDgnParameterDlg::Initial_Data()
{
	if (m_pParamData->iDgnCode == IRC_112_2011_PSC)
	{
		CString strCodeIRC = _T("");  strCodeIRC = _T("IRC:112-2011");
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER0)->SetWindowText(strCodeIRC);  // DgnCode!!
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER1)->SetWindowText(_T(""));  // the _T("/")

		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER2)->SetWindowText(_T(""));  // National Annex!!
		GetDlgItem(IDC_EC2PSC_PARTIAL_PARAMETER1)->SetWindowText(_T("Basic && Seismic"));
	}
	else if (m_pParamData->iDgnCode == IRC_112_2020_PSC)
	{
		CString strCodeIRC = _T("");  strCodeIRC = _T("IRC:112-2020");
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER0)->SetWindowText(strCodeIRC);  // DgnCode!!
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER1)->SetWindowText(_T(""));  // the _T("/")

		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER2)->SetWindowText(_T(""));  // National Annex!!
		GetDlgItem(IDC_EC2PSC_PARTIAL_PARAMETER1)->SetWindowText(_T("Basic && Seismic"));
	}
	else if (m_pParamData->iDgnCode == IRS_PSC)
	{
		CString strCodeIRC = _T("");  strCodeIRC = _T("IRS Concrete Bridge Code");
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER0)->SetWindowText(strCodeIRC);  // DgnCode!!
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER1)->SetWindowText(_T(""));  // the _T("/")


	}
	else
	{
		// Eurocode2-2:05!!
		CString strCode = _T("");  strCode = _T("Eurocode2-2:05");
		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER0)->SetWindowText(strCode);  // DgnCode!!

		int iIndex = 0; iIndex = m_nAnnex;

		strCode = CNationalAnnexTool::ConvertNationalAnnex(iIndex);

		GetDlgItem(IDC_EC2PSC_CODE_PARAMETER2)->SetWindowText(strCode);  // National Annex!!
		GetDlgItem(IDC_EC2PSC_PARTIAL_PARAMETER1)->SetWindowText(_T("Persistent && Transient"));

	}
	// SET UNIT!!
	m_dPartialFac_c_PT.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_c_A.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_c_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_PT.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_A.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_s_S.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_PT.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_A.SetUnitType(D_UNITSYS_NONE);
	m_dPartialFac_ps_S.SetUnitType(D_UNITSYS_NONE);
	//
	m_dAlpha_cc.SetUnitType(D_UNITSYS_NONE);
	m_dAlpha_cc_4Shear.SetUnitType(D_UNITSYS_NONE);
	m_dAlpha_ct.SetUnitType(D_UNITSYS_NONE);
	//
	m_dStressLim_c_k1.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k3.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k4.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_c_k6.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k1.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k2.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k5.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k7.SetUnitType(D_UNITSYS_NONE);
	m_dStressLim_s_k8.SetUnitType(D_UNITSYS_NONE);
	//
	m_dk_c_ps_stage.SetUnitType(D_UNITSYS_NONE);
	m_dk_t_ps_stage.SetUnitType(D_UNITSYS_NONE);
	m_dk_c_ps.SetUnitType(D_UNITSYS_NONE);
	m_dk_t_ps.SetUnitType(D_UNITSYS_NONE);
	//
	m_dk3_cr.SetUnitType(D_UNITSYS_NONE);
	m_dk4_cr.SetUnitType(D_UNITSYS_NONE);
	//
	m_edtk1_Fatigue.SetUnitType(D_UNITSYS_NONE);


	// SET DATA!!

	// SET Default DATA by ANNEX!!
	T_PSCD_D ParamData; ParamData.Initialize();
	//GetDefaultDatabyAnnex(ParamData, m_nAnnex);
	if (m_pParamData->iDgnCode == IRC_112_2011_PSC || m_pParamData->iDgnCode == IRC_112_2020_PSC)
	{
		ParamData.Set_IRC112Data();
	}
	else if (m_pParamData->iDgnCode == IRS_PSC)
	{
		ParamData.Set_IRSData();
	}
	else
	{
		CDBDoc*	pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		pDoc->m_pDataCtrl->SetAnnexData(m_nAnnex, ParamData);
	}


	m_dPartialFac_c_PT.SetEditUnit(m_pParamData->dGamma_c_PT == 0.0 ? ParamData.dGamma_c_PT : m_pParamData->dGamma_c_PT);
	m_dPartialFac_c_A.SetEditUnit(m_pParamData->dGamma_c_A == 0.0 ? ParamData.dGamma_c_A : m_pParamData->dGamma_c_A);
	m_dPartialFac_c_S.SetEditUnit(m_pParamData->dGamma_c_serv == 0.0 ? ParamData.dGamma_c_serv : m_pParamData->dGamma_c_serv);
	m_dPartialFac_s_PT.SetEditUnit(m_pParamData->dGamma_s_PT == 0.0 ? ParamData.dGamma_s_PT : m_pParamData->dGamma_s_PT);
	m_dPartialFac_s_A.SetEditUnit(m_pParamData->dGamma_s_A == 0.0 ? ParamData.dGamma_s_A : m_pParamData->dGamma_s_A);
	m_dPartialFac_s_S.SetEditUnit(m_pParamData->dGamma_s_serv == 0.0 ? ParamData.dGamma_s_serv : m_pParamData->dGamma_s_serv);   // 확인할 것!!
	m_dPartialFac_ps_PT.SetEditUnit(m_pParamData->dGamma_ps_PT == 0.0 ? ParamData.dGamma_ps_PT : m_pParamData->dGamma_ps_PT);
	m_dPartialFac_ps_A.SetEditUnit(m_pParamData->dGamma_ps_A == 0.0 ? ParamData.dGamma_ps_A : m_pParamData->dGamma_ps_A);
	m_dPartialFac_ps_S.SetEditUnit(m_pParamData->dGamma_s_serv == 0.0 ? ParamData.dGamma_s_serv : m_pParamData->dGamma_s_serv);  // 확인할 것!!
	//
	m_dAlpha_cc.SetEditUnit(m_pParamData->dAlpha_cc == 0.0 ? ParamData.dAlpha_cc : m_pParamData->dAlpha_cc);
	m_dAlpha_cc_4Shear.SetEditUnit(EQ0(m_pParamData->dAlphacc4Shear) ? ParamData.dAlphacc4Shear : m_pParamData->dAlphacc4Shear);
	m_dAlpha_ct.SetEditUnit(m_pParamData->dAlpha_ct == 0.0 ? ParamData.dAlpha_ct : m_pParamData->dAlpha_ct);
	//
	m_dStressLim_c_k1.SetEditUnit(m_pParamData->dk1_sl == 0.0 ? ParamData.dk1_sl : m_pParamData->dk1_sl);
	m_dStressLim_c_k3.SetEditUnit(m_pParamData->dk3_sl == 0.0 ? ParamData.dk3_sl : m_pParamData->dk3_sl);
	m_dStressLim_c_k4.SetEditUnit(m_pParamData->dk4_sl == 0.0 ? ParamData.dk4_sl : m_pParamData->dk4_sl);
	m_dStressLim_c_k6.SetEditUnit(m_pParamData->dk6_sl == 0.0 ? ParamData.dk6_sl : m_pParamData->dk6_sl);
	m_dStressLim_s_k1.SetEditUnit(m_pParamData->dk1_ps == 0.0 ? ParamData.dk1_ps : m_pParamData->dk1_ps);
	m_dStressLim_s_k2.SetEditUnit(m_pParamData->dk2_ps == 0.0 ? ParamData.dk2_ps : m_pParamData->dk2_ps);
	m_dStressLim_s_k5.SetEditUnit(m_pParamData->dk5_ps == 0.0 ? ParamData.dk5_ps : m_pParamData->dk5_ps);
	m_dStressLim_s_k7.SetEditUnit(m_pParamData->dk7_ps == 0.0 ? ParamData.dk7_ps : m_pParamData->dk7_ps);
	m_dStressLim_s_k8.SetEditUnit(m_pParamData->dk8_ps == 0.0 ? ParamData.dk8_ps : m_pParamData->dk8_ps);
	//
	m_dk_c_ps_stage.SetEditUnit(m_pParamData->dk_c_ps_stage == 0.0 ? ParamData.dk_c_ps_stage : m_pParamData->dk_c_ps_stage);
	m_dk_t_ps_stage.SetEditUnit(m_pParamData->dk_t_ps_stage == 0.0 ? ParamData.dk_t_ps_stage : m_pParamData->dk_t_ps_stage);
	m_dk_c_ps.SetEditUnit(m_pParamData->dk_c_ps == 0.0 ? ParamData.dk_c_ps : m_pParamData->dk_c_ps);
	m_dk_t_ps.SetEditUnit(m_pParamData->dk_t_ps == 0.0 ? ParamData.dk_t_ps : m_pParamData->dk_t_ps);
	//  
	m_dk3_cr.SetEditUnit(m_pParamData->dk3_cr == 0.0 ? ParamData.dk3_cr : m_pParamData->dk3_cr);
	m_dk4_cr.SetEditUnit(m_pParamData->dk4_cr == 0.0 ? ParamData.dk4_cr : m_pParamData->dk4_cr);
	//
	m_edtk1_Fatigue.SetEditUnit(m_pParamData->dk1_Fatigue == 0.0 ? ParamData.dk1_Fatigue : m_pParamData->dk1_Fatigue);
}

void CDgnCRCAnnexDgnParameterDlg::OnResetDgnParam_byAnnex()
{
	// RESET!!
	if (m_pParamData->iDgnCode == IRC_112_2011_PSC)
	{
		m_pParamData->Set_IRC112Data();
	}
	else if (m_pParamData->iDgnCode == IRC_112_2020_PSC)
	{
		m_pParamData->Set_IRC112Data();
	}
	else if (m_pParamData->iDgnCode == IRS_PSC)
	{
		m_pParamData->Set_IRSData();
	}
	else
	{
		CDBDoc*	pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		pDoc->m_pDataCtrl->SetAnnexData(m_nAnnex, *m_pParamData);
	}
	Initial_Data();
}

void CDgnCRCAnnexDgnParameterDlg::SetAlphaCtrl()
{
	const auto bAlphacc4Shear = HasAlphacc4ShearCtrl();
	GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER5)->ShowWindow(bAlphacc4Shear);
	GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER6)->ShowWindow(bAlphacc4Shear);

	if (!bAlphacc4Shear)
	{
		CRect RectFix;
		GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER5)->GetWindowRect(&RectFix);

		CRect RectStc;
		CRect RectEdt;
		GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER3)->GetWindowRect(&RectStc);
		GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER4)->GetWindowRect(&RectEdt);

		const auto nMoveX = RectStc.left - RectFix.left;
		RectStc.left -= nMoveX;
		RectEdt.left -= nMoveX;

		ScreenToClient(RectStc);
		ScreenToClient(RectEdt);
		GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER3)->SetWindowPos(NULL, RectStc.left, RectStc.top, RectStc.Width(), RectStc.Height(), SWP_NOSIZE | SWP_NOZORDER);
		GetDlgItem(IDC_EC2PSC_ALPHA_PARAMETER4)->SetWindowPos(NULL, RectEdt.left, RectEdt.top, RectEdt.Width(), RectEdt.Height(), SWP_NOSIZE | SWP_NOZORDER);
	}

	return;
}

bool CDgnCRCAnnexDgnParameterDlg::HasAlphacc4ShearCtrl()
{
	return (m_nAnnex == Luxembourg || m_nAnnex == UnitedKingdom);
}