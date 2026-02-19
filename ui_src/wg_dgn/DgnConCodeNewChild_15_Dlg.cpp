#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_15_Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnConCodeNewChild15Dlg::CDgnConCodeNewChild15Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild15Dlg::IDD, pParent, pData, bExistData)
{
	m_aSeismicID.Add(IDC_DGN_CON_CODE_SP_3_GROUP);
	m_aSeismicID.Add(IDC_DGN_EQ_GRADE1);
	m_aSeismicID.Add(IDC_DGN_EQ_GRADE2);
	m_aSeismicID.Add(IDC_DGN_EQ_GRADE3);

    m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeNewChild15Dlg::~CDgnConCodeNewChild15Dlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

void CDgnConCodeNewChild15Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_PHI_CHK, m_bConsiderPhi);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_STATICALLY_CHK, m_bStaticallyDeterminable);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_CREEP_CHK, m_bCreep);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_CODE14_CHK, m_bCosiderSP14_1330_2018);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_SEISMICS_CHK, m_bIncludeSeismics);
	DDX_Check(pDX, IDC_DGN_CON_CODE_SP_NONLINEAR_CALC_SLAB_CHK, m_bNonlinearCalculation);

	DDX_Radio(pDX, IDC_DGN_EQ_GRADE1, m_nIntensityDegree);
	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild15Dlg, CDconChildDialog)
	ON_BN_CLICKED(IDC_DGN_CON_CODE_SP_CODE14_CHK, OnDgnSpSeismicChk)
END_MESSAGE_MAP()

BOOL CDgnConCodeNewChild15Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();
	UpdateChildData();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild15Dlg::InitCtrl()
{
	ResizeChildDlg();

	//SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
	//EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild15Dlg::OnDgnSpSeismicChk()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aSeismicID, m_bCosiderSP14_1330_2018);
}

void CDgnConCodeNewChild15Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_bConsiderPhi = m_pData->bConsiderPhi;
	m_bStaticallyDeterminable = m_pData->bStaticallyDeterminable;
	m_bCreep = m_pData->bCreep;
	((CButton*)GetDlgItem(IDC_DGN_CON_CODE_SP_SECOND_LIMIT_CHK))->SetCheck(m_pData->bCalculationSeconGroup);
	m_bCosiderSP14_1330_2018 = m_pData->bCosiderSP14_1330_2018;
	m_bIncludeSeismics = m_pData->bIncludeSeismics;
	m_nIntensityDegree = m_pData->nIntensityDegree;
	m_nPMCurveMethod = m_pData->nPMCurveMethod;
	m_bNonlinearCalculation = m_pData->bNonlinearCalculation;
	//UpdateChildData(FALSE);
}

void CDgnConCodeNewChild15Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	UpdateData(FALSE);
	ResizeChildDlg();
}

void CDgnConCodeNewChild15Dlg::ResizeChildDlg()
{ 
	CDconChildDialog::ResizeChildDlg();
}

BOOL CDgnConCodeNewChild15Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->bConsiderPhi = m_bConsiderPhi;
	m_pData->bStaticallyDeterminable = m_bStaticallyDeterminable;
	m_pData->bCreep = m_bCreep;
	m_pData->bCalculationSeconGroup = ((CButton*)GetDlgItem(IDC_DGN_CON_CODE_SP_SECOND_LIMIT_CHK))->GetCheck();
	m_pData->bCosiderSP14_1330_2018 = m_bCosiderSP14_1330_2018;
	m_pData->bIncludeSeismics = m_bIncludeSeismics;
	m_pData->nIntensityDegree = m_nIntensityDegree;
	m_pData->nPMCurveMethod = m_nPMCurveMethod;
	m_pData->bNonlinearCalculation = m_bNonlinearCalculation;
	return CDconChildDialog::Dlg2Data();
}