// DgnConCodeNewChild_14_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_14_Dlg.h"
#include "DgnConWallEnvelopMethod.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild4Dlg dialog


CDgnConCodeNewChild14Dlg::CDgnConCodeNewChild14Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild14Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild4Dlg)
// 	m_pData = pData;
	m_bStrongColmWeakBeam = FALSE;
	m_bBeamDeflect = FALSE;
	m_nSpecialWallMtd = 0;
	m_nBndrElemMethod = 0;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;

	m_dConShearStrengthRatio = 0.0;
	m_bConsiderReduction = FALSE;
	m_bSubBeam = TRUE;
	m_bCantilever = TRUE;
	m_bUnderBmColm = TRUE;
	m_bTorsionDesign = FALSE;

	m_dMrft = 0.0;
	m_dTrft = 0.0;
	m_dQ = 3.0;
	m_dR = 1.0;
	m_nPMCurveMethod = 0;
	m_nCheckPos = EN_SELECT_CHECK_POS_BOT;
	m_nFsMethod = EN_FSMETHOD_2_3_FY;

	m_aSelectFrameType.RemoveAll();
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI0);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI1);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI2);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI3);

	m_aStrongColmWeakBeam.RemoveAll();
	m_aStrongColmWeakBeam.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aShearWallType.RemoveAll();
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N0);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N2);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N3);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N4);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N5);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N6);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N7);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N8);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N12);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N13);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N14);

	m_aShearforDesign.RemoveAll();
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR0);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR12);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR13);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR21);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR14);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR11);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR1);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR2);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR3);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR4);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR5);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR6);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR7);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR8);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR9);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR10);

	m_aMembTypeSeisDgn.RemoveAll();
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N1);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N2);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N3);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N4);
	m_aMembTypeSeisDgn.Add(IDC_DGN_EUROCODE204_N24);
	m_aMembTypeSeisDgn.Add(IDC_DGN_EUROCODE204_EM_BTN);

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_TORSION_GROUP);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_STATIC);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_FACTOR);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_aBeamColmJoint.RemoveAll();
	m_aBeamColmJoint.Add(IDC_DGN_EUROCODE204_N17);

	m_arCtrlSDR.RemoveAll();
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR11);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR1);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR2);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR3);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR4);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR5);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR6);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR7);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR8);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR9);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR10);

	m_aPMCurveMethod.RemoveAll();
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_P_RADIO);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_MP_RADIO);

	m_aSelectPosGroup.RemoveAll();
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_N19_GRB);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_TOP_RDO);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_BOT_RDO);

	m_aSCWBDesign.RemoveAll();
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_GRP);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);

	m_aRdoSCWBMethod.RemoveAll();
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);
	m_aRdoSCWBMethod.FreeExtra();

	m_aConsiderReduction.RemoveAll();
	m_aConsiderReduction.Add(IDC_DGN_RCCTRL_INTERSHEAR2);

	m_aFsMethod.RemoveAll();
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_FRM);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_2_3_FY_RADIO);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_PROGRAM_RADIO2);
}

void CDgnConCodeNewChild14Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild4Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK, m_bStrongColmWeakBeam);
	DDX_Check(pDX, IDC_DGN_RCCTRL_BEAM_DEFLECT, m_bBeamDeflect);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N12, m_nSpecialWallMtd);
	DDX_Radio(pDX, IDC_DGN_EQ_ACI1, m_iFrameType);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N3, m_nBndrElemMethod);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);

	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7, m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);

	DDX_Check(pDX, IDC_DGN_KCI_USD07_N2, m_bSubBeam);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N3, m_bCantilever);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N4, m_bUnderBmColm);
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N17, m_bBCJointDgn);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);

	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);
	DDX_Text(pDX, IDC_DGN_WALLTYPE_N7, m_dR);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N5, m_CmbQ);
	//DDX_Control(pDX, IDC_DGN_WALLTYPE_N7,  m_CmbIe);
	DDX_Control(pDX, IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	DDX_Control(pDX, IDC_DGN_SEIS_PARAM_GRB, m_grpSeisParam);

	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_TOP_RDO, m_nCheckPos);
	DDX_Check(pDX, IDC_DGN_RCCTRL_INTERSHEAR2, m_bConsiderReduction);
	DDX_Radio(pDX, IDC_DGN_FS_METHOD_2_3_FY_RADIO, m_nFsMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild14Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild4Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI1, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI2, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI3, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N12, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N13, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N14, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N3, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N8, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N17, OnBCJoint)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_EM_BTN, OnBtnWallEnvelopMethod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild4Dlg message handlers

BOOL CDgnConCodeNewChild14Dlg::OnInitDialog()
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();

	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild14Dlg::SetComboCdAndIe()
{
	m_CmbQ.ResetContent();
	m_CmbQ.AddString(_T("2.00"));
	m_CmbQ.AddString(_T("3.00"));
	m_CmbQ.AddString(_T("4.00"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!m_bExistData) m_CmbQ.SetCurSel(1);
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dQ);
		m_CmbQ.SetWindowText(strText);
	}

	// 	m_CmbIe.ResetContent();
	// 	m_CmbIe.AddString(_T("1.00"));
	// 	m_CmbIe.AddString(_T("1.25"));
	// 	m_CmbIe.AddString(_T("1.50"));

		// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	// 	if(!m_bExistData) m_CmbIe.SetCurSel(1);
	// 	else
	// 	{
	// 		CString strText = _T("");
	// 		strText.Format(_T("%.2f"), m_dR);
	// 		m_CmbIe.SetWindowText(strText);
	// 	}
}


void CDgnConCodeNewChild14Dlg::SetComboBox()
{
	SetComboCdAndIe();
}
void CDgnConCodeNewChild14Dlg::InitCtrl()
{
	m_grpSeisParam.InitControl(this, CDgnConCodeNewChild14Dlg::IDD, IDC_DGN_SEIS_PARAM_GRB, TRUE, TRUE);
	m_grpSeisParam.SetFoldState(FALSE); // 무조건 펼친다

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if (!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용

		const CString& strCode = m_pData->DesignCode;
		if (strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KCI_USD12 ||
			strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 ||
			strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
			strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 ||
			strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
			strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
			strCode == CONCODE_NSR_10 || strCode == CONCODE_NSCP_2015 ||
			strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
		{
			m_iFrameType = 1;
		}
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1);  // 1: Norminal Strength
	}
	else
	{
		m_dQ = m_pData->dQ >= 1.0 ? m_pData->dQ : 1.0;
		m_dR = m_pData->dR >= 1.0 ? m_pData->dR : 1.0;
		m_nSpecialWallMtd = m_pData->nSpecialWall;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_nBndrElemMethod = (m_pData->iBndrElemMethod == 0 || m_pData->iBndrElemMethod == 1) ? m_pData->iBndrElemMethod : 0;

		m_bSubBeam = m_pData->bNonSeisSubBeam;
		m_bCantilever = m_pData->bNonSeisCantilever;
		m_bUnderBmColm = m_pData->bNonSeisUnderGround;

		m_dMrft = m_pData->dMrft;
		m_dTrft = m_pData->dTrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;

		m_bBCJointDgn = m_pData->bBCJointDgn;
		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
		m_bBeamDeflect = m_pData->bCheckBeamDeflect;
		m_nPMCurveMethod = m_pData->nPMCurveMethod;
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
		m_nCheckPos = m_pData->nCheckPos;
		m_bConsiderReduction = m_pData->bConsiderReduction;
		m_nFsMethod = m_pData->nFsMethod4Beam;
	}

	SetComboBox();

	GetInitValue(strCode);
	UpdateData(FALSE);

	OnChangeTorsion();
	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowBySpecialWall();
	EnableWindowByBeamColmJointDesign(FALSE);
	OnBCJoint();
}

void CDgnConCodeNewChild14Dlg::OnDgnRcctrlSpecial()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	GetInitValue(strCode);
	UpdateData(FALSE);

	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild14Dlg::GetInitValue(CString strCode)
{
	if (!m_bExistData)
	{
		m_iAlphaType1 = 0;
		m_dAlpha1 = 1.0;
		m_dAlpha2 = 1.0;
		m_iFrameType = 1;
		m_nSpecialWallMtd = 2;
	}
	BOOL bDefOK = (m_dAlpha1*m_dAlpha2 == 0.0 ? TRUE : FALSE);
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
}

void CDgnConCodeNewChild14Dlg::OnChangeFrameType()
{
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	m_iAlphaType1 = 0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	UpdateData(FALSE);

	SetEQCtrl(strCode);
	EnableWindowByBeamColmJointDesign();
}

void CDgnConCodeNewChild14Dlg::OnChkSpecialWall()
{
	UpdateData(TRUE);
	EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild14Dlg::OnChangeBndrElemMethodType()
{
	UpdateData(TRUE);
	EnableWindowByBndrElemMethod();
}

void CDgnConCodeNewChild14Dlg::OnChangeTorsion()
{
	if (m_btnTorsion.GetCheck())
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(FALSE);
	}
}


void CDgnConCodeNewChild14Dlg::EnableWindowByBndrElemMethod()
{
	if (m_nBndrElemMethod == 1)
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(TRUE);
	}
}

void CDgnConCodeNewChild14Dlg::EnableWindowByBeamColmJointDesign(BOOL bChangeValue/*=TRUE*/)
{
	BOOL bEnable = FALSE; 
	if(m_iFrameType==0) bEnable = TRUE;
	if(bChangeValue && m_bBCJointDgn != bEnable)
	{
		m_bBCJointDgn = bEnable;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_DGN_EUROCODE204_N17)->EnableWindow(bEnable);
	OnBCJoint();

	CDlgUtil::CtrlEnableDisable(this, m_aSCWBDesign, m_iFrameType == 0 || m_iFrameType == 1);
}

void CDgnConCodeNewChild14Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild14Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild14Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft = m_pData->dMrft;
	m_dTrft = m_pData->dTrft;
	m_iAlphaType1 = m_pData->iAlphaType;
	m_dAlpha1 = m_pData->dAlpha1;
	m_dAlpha2 = m_pData->dAlpha2;
	m_bTorsionDesign = m_pData->bTorsionDesign;
	m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;

	// Save KCI-USD07
	CString strNewValue = _T("");
	m_CmbQ.GetWindowText(strNewValue);
	m_dQ = _ttof(strNewValue);
	// 	m_CmbIe.GetWindowText(strNewValue);
	// 	m_dR = _ttof(strNewValue);

	m_nSpecialWallMtd = m_pData->nSpecialWall;

	m_nBndrElemMethod = m_pData->iBndrElemMethod;
	m_dQ = m_pData->dQ;
	m_dR = m_pData->dR;

	// Exposure Condition
	m_bSubBeam = m_pData->bNonSeisSubBeam;
	m_bCantilever = m_pData->bNonSeisCantilever;
	m_bUnderBmColm = m_pData->bNonSeisUnderGround;
	m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
	m_nPMCurveMethod = m_pData->nPMCurveMethod;
	m_nCheckPos = m_pData->nCheckPos;
	m_bConsiderReduction = m_pData->bConsiderReduction;
	m_nFsMethod = m_pData->nFsMethod4Beam;

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
}

void CDgnConCodeNewChild14Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	GetInitValue(strCode);
	UpdateData(FALSE);

	SetComboBox();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowByBeamColmJointDesign();
}
void CDgnConCodeNewChild14Dlg::ResizeChildDlg()
{
	BOOL bShowFrame = m_iFrameType != 2;
	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	BOOL bSpecialEQShowFrame = (!bSeisParamFolded && m_bSpecialEQ && bShowFrame);

	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_SEIS_PARAM_GRB, m_bSpecialEQ, TRUE);

	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_EQ_FACTOR0, bSpecialEQShowFrame, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aBeamColmJoint, bSpecialEQShowFrame);
	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_EUROCODE204_N19_GRB, bSpecialEQShowFrame, TRUE);

	if (!bSeisParamFolded/* && m_bSpecialEQ*/)
		ReAlingShearDesignCtrl();

	UINT nIDRef = 0;
	UINT nIDTgt = 0;
	CRect RectRef, RectTgt;

	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectTgt);
	int nSpace = (int)RectTgt.Height() / 2;

	if (!bSeisParamFolded)
	{
		nIDRef = IDC_DGN_KCI_USD07_N1;
		nIDTgt = IDC_DGN_SEIS_PARAM_GRB;
		GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
		GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
		ScreenToClient(RectRef);
		ScreenToClient(RectTgt);
		RectTgt.bottom = RectRef.bottom + nSpace;
		GetDlgItem(nIDTgt)->MoveWindow(RectTgt, TRUE);
	}

	///////////////////////////////////////////////////////////////
	GetDlgItem(IDC_DGN_EUROCODE204_N17)->GetWindowRect(RectRef);
	GetDlgItem(IDC_DGN_EUROCODE204_N19_GRB)->GetWindowRect(RectTgt);

	int nDistY = RectTgt.top - RectRef.bottom - globalUtils.ScaleByDPI(5);

	CDlgUtil::CtrlMoveDistY(this, m_aBeamColmJoint, nDistY);

	///////////////////////////////////////////////////////////////
	nIDRef = IDC_DGN_RCCTRL_SPECIAL;
	nIDTgt = IDC_DGN_RCCTRL_TORSION;
	if (m_bSpecialEQ)
	{
		nIDRef = IDC_DGN_SEIS_PARAM_GRB;
	}
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	nDistY = RectRef.bottom - RectTgt.top + nSpace;
	CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aConsiderReduction, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);


	CDlgUtil::CtrlShowHide(this, m_aTorsionDesign, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aMomentRedistr, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aPMCurveMethod, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aConsiderReduction, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aFsMethod, SW_SHOW);

	///////////////////////////////////////////////////////////////

// 	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectRef);
// 	GetDlgItem(IDC_DGN_PM_CURVE_METHOD)->GetWindowRect(&RectTgt);
// 
// 	nDistY = RectRef.bottom - RectTgt.top + nSpace;
// 	CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
// 
// 	GetDlgItem(IDC_DGN_PM_CURVE_METHOD)->GetWindowRect(&RectRef);
// 	GetDlgItem(IDC_DGN_RCCTRL_INTERSHEAR2)->GetWindowRect(&RectTgt);
// 
// 	nDistY = RectRef.bottom - RectTgt.top + nSpace;
// 	CDlgUtil::CtrlMoveDistY(this, m_aInteractionShear, nDistY);


// 	CDlgUtil::CtrlShowHide(this, m_aMomentRedistr, SW_SHOW);
// 	CDlgUtil::CtrlShowHide(this, m_aPMCurveMethod, SW_SHOW);
// 	CDlgUtil::CtrlShowHide(this, m_aInteractionShear, SW_SHOW);

	///////////////////////////////////////////////////////////////
	nIDRef = IDC_DGN_FS_METHOD_FRM;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	ScreenToClient(RectRef);
	GetClientRect(&RectTgt);
	RectTgt.bottom = RectRef.bottom + nSpace;
	MoveWindow(RectTgt, TRUE);

	Invalidate();
	CDconChildDialog::ResizeChildDlg();
}

void CDgnConCodeNewChild14Dlg::ReAlingShearDesignCtrl()
{
	const int nGap = globalUtils.ScaleByDPI(6);
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	int iStatus = GetDataStatus(strCode);
	int nDistY1 = 0, nDistY2 = 0;
	CWnd* pShear4Dgn = GetDlgItem(IDC_DGN_EQ_FACTOR0);
	CWnd* pScwbMethod = GetDlgItem(IDC_DGN_SCWB_METHOD_GRP);
	CWnd* pMemberType = GetDlgItem(IDC_DGN_EUROCODE204_N24);
	CWnd* pSelCheck = GetDlgItem(IDC_DGN_EUROCODE204_N19_GRB);
	CRect Shear4DgnRect, ScwbMethodRect, MemberTypeRect, SelCheckRect;
	pShear4Dgn->GetWindowRect(Shear4DgnRect);
	pScwbMethod->GetWindowRect(ScwbMethodRect);
	pSelCheck->GetWindowRect(SelCheckRect);
	pMemberType->GetWindowRect(MemberTypeRect);
	if ((iStatus == 20 && m_iFrameType == 2))
	{
		nDistY1 = Shear4DgnRect.top - ScwbMethodRect.top;
		if (nDistY1 != 0) CDlgUtil::CtrlMoveDistY(this, m_aSCWBDesign, nDistY1);

		pScwbMethod->GetWindowRect(ScwbMethodRect);
		nDistY2 = ScwbMethodRect.bottom - MemberTypeRect.top + nGap;
		if (nDistY2 != 0) CDlgUtil::CtrlMoveDistY(this, m_aMembTypeSeisDgn, nDistY2);
	}
	else
	{
		nDistY1 = Shear4DgnRect.bottom - ScwbMethodRect.top + nGap;
		if (nDistY1 != 0) CDlgUtil::CtrlMoveDistY(this, m_aSCWBDesign, nDistY1);

		nDistY2 = SelCheckRect.bottom - MemberTypeRect.top + nGap;
		if (nDistY2 != 0) CDlgUtil::CtrlMoveDistY(this, m_aMembTypeSeisDgn, nDistY2);
	}

	CDlgUtil::CtrlShowHide(this, m_aSCWBDesign, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aMembTypeSeisDgn, SW_SHOW);

}

void CDgnConCodeNewChild14Dlg::EnableWindowBySpecialWall()
{
	CArray<UINT, UINT> aShearWallRdo;
	aShearWallRdo.RemoveAll();
	aShearWallRdo.Add(IDC_DGN_WALLTYPE_N12);
	aShearWallRdo.Add(IDC_DGN_WALLTYPE_N13);
	aShearWallRdo.Add(IDC_DGN_WALLTYPE_N14);
	aShearWallRdo.FreeExtra();

	int nCheck = 2;
	CDlgUtil::CtrlRadioGetCheck(this, aShearWallRdo, nCheck);
	BOOL bSpecial = (nCheck != 2);

	if (bSpecial)
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N3)->EnableWindow(TRUE);
		if (m_nBndrElemMethod == 0)
		{
			GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_DGN_WALLTYPE_N8)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N8)->EnableWindow(FALSE);
	}
}

void CDgnConCodeNewChild14Dlg::SetEQCtrl(const CString& strCode)
{
	int iStatus = GetDataStatus(strCode);

	GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);

	if (m_iAlphaType1 == 0 || m_iAlphaType1 == 1)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
	}
	else if (m_iAlphaType1 == 2)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
	}


	CString strQ = _T("");
	strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
	if (m_iFrameType == 0) //Speical
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_SPECIAL_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
	}
	else //Intermediate, Ordinary
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
	}
	GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_NTC));


	if (strQ != _T(""))
	{
		CString strMax = _T(""), strMin = _T(""), strQ1 = _T(""), strQ2 = _T("");
		strQ1.Format(_T("%s1"), strQ);
		strQ2.Format(_T("%s2"), strQ);
		strMax.Format(_T("MAX(%s,%s)"), strQ1, strQ2);
		strMin.Format(_T("MIN(%s,%s)"), strQ1, strQ2);
		GetDlgItem(IDC_DGN_EQ_FACTOR1)->SetWindowText(strMax);
		GetDlgItem(IDC_DGN_EQ_FACTOR2)->SetWindowText(strMin);
		GetDlgItem(IDC_DGN_EQ_FACTOR3)->SetWindowText(strQ1);
		GetDlgItem(IDC_DGN_EQ_FACTOR4)->SetWindowText(strQ2);
	}

	// Add by ZINU.('08.03.20). NO:3292, Multiply R by Vc (Vu1 > 0.5*Vu2).
	// ACI318-Series only Special Moment Frame, TWN-USD92.
	if (iStatus == 20 && m_iFrameType != 2)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_HIDE);
	}

	if(m_iFrameType == 2) // Low
	{
		m_nSpecialWallMtd = 2;
		GetDlgItem(IDC_DGN_WALLTYPE_N12)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALLTYPE_N13)->EnableWindow(FALSE);
		UpdateData(FALSE);

		OnChkSpecialWall();
	}
	else if(m_iFrameType == 0 || m_iFrameType == 1) // High, Medium
	{
		GetDlgItem(IDC_DGN_WALLTYPE_N12)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALLTYPE_N13)->EnableWindow(TRUE);
	}
}

BOOL CDgnConCodeNewChild14Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft = m_dMrft;
	m_pData->dTrft = m_dTrft;

	m_pData->iAlphaType = m_iAlphaType1;
	m_pData->dAlpha1 = m_dAlpha1;
	m_pData->dAlpha2 = m_dAlpha2;
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->dConShearStrengthRatio = m_dConShearStrengthRatio;
	m_pData->bCheckBeamDeflect = m_bBeamDeflect;
	m_pData->bBCJointDgn = m_bBCJointDgn;

	// Save KCI-USD07
	CString strNewValue = _T("");
	m_CmbQ.GetWindowText(strNewValue);
	m_dQ = _ttof(strNewValue);
	// 	m_CmbIe.GetWindowText(strNewValue);
	// 	m_dR = _ttof(strNewValue);

	m_pData->nSpecialWall = m_nSpecialWallMtd;

	m_pData->iBndrElemMethod = m_nBndrElemMethod;
	m_pData->dQ = m_dQ;
	m_pData->dR = m_dR;

	// Exposure Condition
	m_pData->bNonSeisSubBeam = m_bSubBeam;
	m_pData->bNonSeisCantilever = m_bCantilever;
	m_pData->bNonSeisUnderGround = m_bUnderBmColm;
	m_pData->bStrongColmWeakBeam = m_bStrongColmWeakBeam;
	m_pData->nPMCurveMethod = m_nPMCurveMethod;
	m_pData->nCheckPos = m_nCheckPos;
	m_pData->bConsiderReduction = m_bConsiderReduction;
	m_pData->nFsMethod4Beam = m_nFsMethod;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);

	CDconChildDialog::Dlg2Data();
	return TRUE;
}

void CDgnConCodeNewChild14Dlg::OnBCJoint()//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
{
	UpdateData(TRUE);
	BOOL bJoint = m_bBCJointDgn;
	CDlgUtil::CtrlEnableDisable(this, m_aSelectPosGroup, bJoint);
}
void CDgnConCodeNewChild14Dlg::OnBtnWallEnvelopMethod()
{
	CDgnConWallEnvelopMethod Dlg;
	Dlg.SetMethod(m_pData->nWallDgnEnvelop);

	if (Dlg.DoModal() == IDOK)
	{
		m_pData->nWallDgnEnvelop = Dlg.GetMethod();
	}
}

LRESULT CDgnConCodeNewChild14Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	ResizeChildDlg();
	if (!bSeisParamFolded)
	{
		CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
		SetEQCtrl(strCode);
	}

	return 0L;
}