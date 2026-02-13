// DgnConCodeNewChild_12_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewCHILD_12_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_dgn\DgnCodeCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild12Dlg dialog


CDgnConCodeNewChild12Dlg::CDgnConCodeNewChild12Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild12Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild12Dlg)
// 	m_pData = pData;
	m_bStrongColmWeakBeam = FALSE;
	m_bSpecialWall = FALSE;
	m_bBeamDeflect = FALSE;
	m_nBndrElemMethod = 0;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dAlpha2_Colm = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_bSubBeam = TRUE;
	m_bCantilever = TRUE;
	m_bUnderBmColm = TRUE;
	m_bTorsionDesign = FALSE;
	m_dMrft = 0.0;
	m_dTrft = 0.0;
	m_dCd = 4.5;
	m_dIe = 1.2;
	m_nExposure = 0;
	m_nPMCurveMethod = 1;
	m_nCheckPos = EN_SELECT_CHECK_POS_BOT;
#ifdef _ORG
	m_nBeamRebarConsiderOpt = EN_MCMB_EQUI;
#else
	m_nBeamRebarConsiderOpt = EN_MCMB_EACH;
#endif // _ORG
	m_bConsiderBeamElemForce = FALSE;

	m_bConsiderInteractionShear = FALSE;
	m_nFsMethod = EN_FSMETHOD_2_3_FY;

	m_bShowExposure = TRUE;
	m_bShowAlpha2Colm = TRUE;
	m_bShowBeamDeflect = TRUE;
	m_nBeamDeflectSpan = 0;
	m_bShowPMCurveMethod = TRUE;

	m_aSelectFrameType.RemoveAll();
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI0);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI1);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI2);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI3);

	m_aStrongColmWeakBeam.RemoveAll();
	m_aStrongColmWeakBeam.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aShearWallType.RemoveAll();
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N0);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N1);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N2);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N3);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N4);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N5);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N6);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N7);
	m_aShearWallType.Add(IDC_DGN_WALLTYPE_N8);

	m_aShearforDesign.RemoveAll();
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR20);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR8);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR9);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR10);
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
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR25);

	m_aMembTypeSeisDgn.RemoveAll();
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N1);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N2);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N3);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N4);
	//m_aMembTypeSeisDgn.Add(IDC_DGN_EUROCODE204_N17);

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_TORSION_GROUP);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_STATIC);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_FACTOR);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_aExposureCond.RemoveAll();
	m_aExposureCond.Add(IDC_DGN_EXPOSURE_STC);
	m_aExposureCond.Add(IDC_DGN_EXPOSURE_DRY_RDO);
	m_aExposureCond.Add(IDC_DGN_EXPOSURE_ETC_RDO);

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
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR22);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR23);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR24);

	m_arCtrlAlphaColm.RemoveAll();
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR22);
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR23);
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR24);

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

	m_aInteractionShear.RemoveAll();
	m_aInteractionShear.Add(IDC_DGN_RCCTRL_INTERSHEAR2);

	m_aBeamCalcMethod.RemoveAll();
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD_EQUI_RADIO);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD_EACH_RADIO);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_DESIGN_EACH_ELEM_CHK);

	m_aFsMethod.RemoveAll();
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_FRM);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_2_3_FY_RADIO);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_PROGRAM_RADIO2);

	m_aBeamColmJoint.RemoveAll();
	m_aBeamColmJoint.Add(IDC_DGN_EUROCODE204_N17);
	m_aBeamColmJoint.Append(m_aSelectPosGroup);
}

void CDgnConCodeNewChild12Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild12Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK, m_bStrongColmWeakBeam);
	DDX_Check(pDX, IDC_DGN_WALLTYPE_N1, m_bSpecialWall);
	DDX_Check(pDX, IDC_DGN_RCCTRL_BEAM_DEFLECT, m_bBeamDeflect);

	DDX_Radio(pDX, IDC_DGN_EQ_ACI1, m_iFrameType);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N3, m_nBndrElemMethod);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Radio(pDX, IDC_DGN_EXPOSURE_DRY_RDO, m_nExposure);

	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7, m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR24, m_dAlpha2_Colm);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);

	DDX_Check(pDX, IDC_DGN_KCI_USD07_N2, m_bSubBeam);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N3, m_bCantilever);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N4, m_bUnderBmColm);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N17, m_bBCJointDgn);

	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	DDX_Control(pDX, IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N5, m_CmbCd);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N7, m_CmbIe);

	DDX_Control(pDX, IDC_DGN_SEIS_PARAM_GRB, m_grpSeisParam);

	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_TOP_RDO, m_nCheckPos);

	DDX_Radio(pDX, IDC_DGN_BEAM_CALC_METHOD_EQUI_RADIO, m_nBeamRebarConsiderOpt);
	DDX_Check(pDX, IDC_DGN_BEAM_DESIGN_EACH_ELEM_CHK, m_bConsiderBeamElemForce);

	DDX_Check(pDX, IDC_DGN_RCCTRL_INTERSHEAR2, m_bConsiderInteractionShear);
	DDX_Radio(pDX, IDC_DGN_FS_METHOD_2_3_FY_RADIO, m_nFsMethod);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild12Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild12Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N1, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N3, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N8, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI1, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI2, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI3, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N17, OnBCJoint)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild12Dlg message handlers

BOOL CDgnConCodeNewChild12Dlg::OnInitDialog()
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();

	CRect RefRect, TgtRect;

	GetDlgItem(IDC_DGN_RCCTRL_BEAM_DEFLECT)->GetWindowRect(&RefRect);
	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&TgtRect);

	m_nBeamDeflectSpan = RefRect.bottom - TgtRect.bottom;

	AlignControl();
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild12Dlg::SetComboCdAndIe()
{
	m_CmbCd.ResetContent();
	m_CmbCd.AddString(_T("1.25"));
	m_CmbCd.AddString(_T("1.50"));
	m_CmbCd.AddString(_T("2.00"));
	m_CmbCd.AddString(_T("2.50"));
	m_CmbCd.AddString(_T("3.00"));
	m_CmbCd.AddString(_T("3.25"));
	m_CmbCd.AddString(_T("4.00"));
	m_CmbCd.AddString(_T("4.50"));
	m_CmbCd.AddString(_T("5.00"));
	m_CmbCd.AddString(_T("5.50"));
	m_CmbCd.AddString(_T("6.00"));
	m_CmbCd.AddString(_T("6.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!m_bExistData)
	{
		m_CmbCd.SetCurSel(7);
	}
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dCd);
		m_CmbCd.SetWindowText(strText);
	}

	m_CmbIe.ResetContent();
	m_CmbIe.AddString(_T("1.00"));
	m_CmbIe.AddString(_T("1.20"));
	m_CmbIe.AddString(_T("1.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!m_bExistData)
	{
		m_CmbIe.SetCurSel(1);
	}
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dIe);
		m_CmbIe.SetWindowText(strText);
	}
}

void CDgnConCodeNewChild12Dlg::SetComboCdAngle_ACI318()
{
	m_CmbCd.ResetContent();
	m_CmbCd.AddString(_T("1.00"));
	m_CmbCd.AddString(_T("1.25"));
	m_CmbCd.AddString(_T("1.50"));
	m_CmbCd.AddString(_T("1.75"));
	m_CmbCd.AddString(_T("2.00"));
	m_CmbCd.AddString(_T("2.25"));
	m_CmbCd.AddString(_T("2.50"));
	m_CmbCd.AddString(_T("3.00"));
	m_CmbCd.AddString(_T("3.25"));
	m_CmbCd.AddString(_T("3.50"));
	m_CmbCd.AddString(_T("4.00"));
	m_CmbCd.AddString(_T("4.50"));
	m_CmbCd.AddString(_T("5.00"));
	m_CmbCd.AddString(_T("5.50"));
	m_CmbCd.AddString(_T("6.00"));
	m_CmbCd.AddString(_T("6.50"));

	m_CmbIe.ResetContent();
	m_CmbIe.AddString(_T("1.00"));
	m_CmbIe.AddString(_T("1.25"));
	m_CmbIe.AddString(_T("1.50"));

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!m_bExistData)
	{
		m_CmbCd.SetCurSel(11);
	}
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dCd);
		m_CmbCd.SetWindowText(strText);
	}

	// Modify by GAY. MQC:5022. (`12.07.30). Cd, Ie 변경 후, Dlg Open시에 초기값 설정되는 문제 수정.
	if (!m_bExistData)
	{
		m_CmbIe.SetCurSel(1);
	}
	else
	{
		CString strText = _T("");
		strText.Format(_T("%.2f"), m_dIe);
		m_CmbIe.SetWindowText(strText);
	}
}

void CDgnConCodeNewChild12Dlg::SetComboBox()
{
	const CString& strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

#if defined(_MGEN)
	if (strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
		strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 ||
		strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
		strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NSR_10 || strCode == CONCODE_NSCP_2015 ||
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		SetComboCdAngle_ACI318();
	}
	else
	{
		SetComboCdAndIe();
	}
#endif
}

BOOL CDgnConCodeNewChild12Dlg::UseAlpha2Colm(const CString& strCode)
{
	return CDgnCodeCtrl::IsConCodeUseAlpha2ColmSSWB(strCode) ? TRUE : FALSE;
}

BOOL CDgnConCodeNewChild12Dlg::UseBeamColmJointDesign(const CString& strCode)
{
	if (strCode == CONCODE_ACI318_14) { return TRUE; }
	if (strCode == CONCODE_ACI318M_14) { return TRUE; }
	if (strCode == CONCODE_ACI318_19) { return TRUE; }
	if (strCode == CONCODE_ACI318M_19) { return TRUE; }
	if (strCode == CONCODE_ACI318_25) { return TRUE; }
	if (strCode == CONCODE_ACI318M_25) { return TRUE; }
	if (strCode == CONCODE_NSCP_2015) { return TRUE; }
	if (strCode == CONCODE_NTC_DCEC_2017) { return TRUE; }
	if (strCode == CONCODE_NTC_DCEC_2023) { return TRUE; }
	return FALSE;
}

void CDgnConCodeNewChild12Dlg::AlignControl()
{
	//m_grpWallType.InitControl(this, CDgnConCodeNewChild12Dlg::IDD, IDC_DGN_WALLTYPE_N0, TRUE, TRUE);
}

void CDgnConCodeNewChild12Dlg::SetShowPMCurveMethod(const CString& strCode)
{
	const BOOL bShowMNCurve = CDBLib::IsConCodeForPMCalcMethod(strCode) ? TRUE : FALSE;

	if (m_bShowPMCurveMethod == bShowMNCurve) return;

	CRect RefRect, TgtRect;

	GetDlgItem(IDC_DGN_PM_CURVE_METHOD)->GetWindowRect(&TgtRect);

	int nDistY = TgtRect.top - TgtRect.bottom;
	if (!m_bShowPMCurveMethod) nDistY *= -1;

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	m_bShowPMCurveMethod = bShowMNCurve;
}

void CDgnConCodeNewChild12Dlg::InitCtrl()
{
	m_grpSeisParam.InitControl(this, CDgnConCodeNewChild12Dlg::IDD, IDC_DGN_SEIS_PARAM_GRB, TRUE, TRUE);
	m_grpSeisParam.SetFoldState(FALSE); // 무조건 펼친다

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if (!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1);  // 1: Norminal Strength
	}
	else
	{
		m_dCd = m_pData->dCd >= 1.0 ? m_pData->dCd : 1.0;
		m_dIe = m_pData->dIe >= 1.0 ? m_pData->dIe : 1.0;
		m_bSpecialWall = m_pData->nSpecialWall > 0 ? TRUE : FALSE;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_nBndrElemMethod = (m_pData->iBndrElemMethod == 0 || m_pData->iBndrElemMethod == 1) ? m_pData->iBndrElemMethod : 0;

		m_bSubBeam = m_pData->bNonSeisSubBeam;
		m_bCantilever = m_pData->bNonSeisCantilever;
		m_bUnderBmColm = m_pData->bNonSeisUnderGround;
		m_nExposure = m_pData->nExposure;

		m_dMrft = m_pData->dMrft;
		m_dTrft = m_pData->dTrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;
		m_dAlpha2_Colm = m_pData->dAlpha2_Colm;
		m_bBCJointDgn = m_pData->bBCJointDgn;

		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
		m_bBeamDeflect = m_pData->bCheckBeamDeflect;
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
		m_nPMCurveMethod = m_pData->nPMCurveMethod;
		m_nCheckPos = m_pData->nCheckPos;
		m_bConsiderInteractionShear = m_pData->bConsiderInteractShear;
		m_nBeamRebarConsiderOpt = m_pData->nBeamRebarConsiderOpt;
		m_bConsiderBeamElemForce = m_pData->bConsiderBeamElemForce;
		m_nFsMethod = m_pData->nFsMethod4Beam;

	}

	SetComboBox();
	SetShowBeamDeflection(strCode);
	SetShowExposure(strCode);
	SetAlpha2Colm(strCode);
	SetShowPMCurveMethod(strCode);

	GetInitValue(strCode);

	UpdateData(FALSE);

	OnChangeTorsion();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowBySpecialWall();
	EnableWindowByBeamColmJointDesign(strCode, FALSE);
	OnBCJoint();
}

void CDgnConCodeNewChild12Dlg::OnDgnRcctrlSpecial()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	GetInitValue(strCode);
	UpdateData(FALSE);

	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild12Dlg::GetInitValue(CString strCode)
{
	if (!m_bExistData)
	{
		m_iAlphaType1 = 1;
		m_dAlpha1 = 1.0;
		m_dAlpha2 = 2.0;
		m_dAlpha2_Colm = 2.0;
		m_iFrameType = 1;
	}
	BOOL bDefOK = (m_dAlpha1*m_dAlpha2 == 0.0 ? TRUE : FALSE);
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	if (bDefOK) m_dAlpha2_Colm = m_dAlpha2;

}

void CDgnConCodeNewChild12Dlg::OnChangeFrameType()
{
	UpdateData(TRUE);


	ResizeChildDlg();

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	SetEQCtrl(strCode);
	EnableWindowByBeamColmJointDesign(strCode);
}

void CDgnConCodeNewChild12Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild12Dlg::OnChkSpecialWall()
{
	UpdateData(TRUE);
	EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild12Dlg::OnChangeBndrElemMethodType()
{
	UpdateData(TRUE);
	EnableWindowByBndrElemMethod();
}

void CDgnConCodeNewChild12Dlg::EnableWindowByBeamColmJointDesign(const CString& strCode, BOOL bChangeValue/*=TRUE*/)
{
	const auto bEnable = [this, &strCode]()
	{
		if(strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
			strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25) return TRUE;
		else if(UseBeamColmJointDesign(strCode) && m_iFrameType == 0)     return TRUE;
		return FALSE;
	}();
	
	if (bChangeValue && m_bBCJointDgn != bEnable)
	{
		m_bBCJointDgn = bEnable;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_DGN_EUROCODE204_N17)->EnableWindow(bEnable);
	OnBCJoint();

	CDlgUtil::CtrlEnableDisable(this, m_aSCWBDesign, bEnable);
}

void CDgnConCodeNewChild12Dlg::EnableWindowByBndrElemMethod()
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

void CDgnConCodeNewChild12Dlg::OnChangeTorsion()
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

void CDgnConCodeNewChild12Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	m_dAlpha2_Colm = m_dAlpha2;
	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild12Dlg::SetShowExposure(const CString& strCode)
{
	BOOL bShowExposure_new = (strCode == CONCODE_KCI_USD12 || strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022) ? TRUE : FALSE;

	if (m_bShowExposure == bShowExposure_new) return;

	CRect RefRect, TgtRect;

	GetDlgItem(IDC_DGN_BEAM_DESIGN_EACH_ELEM_CHK)->GetWindowRect(&RefRect);
	GetDlgItem(IDC_DGN_EXPOSURE_STC)->GetWindowRect(&TgtRect);

	int nDistY = RefRect.bottom - TgtRect.bottom;
	if (!m_bShowExposure) nDistY *= -1;

	//CDlgUtil::CtrlMoveDistY(this, m_aBeamCalcMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aInteractionShear, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	m_bShowExposure = bShowExposure_new;
}

void CDgnConCodeNewChild12Dlg::SetShowBeamDeflection(const CString& strCode)
{
	const BOOL bShowBeamDeflect_new = [strCode]() -> BOOL
	{
		if (strCode == CONCODE_ACI318_14) { return TRUE; }
		if (strCode == CONCODE_ACI318M_14) { return TRUE; }
		if (strCode == CONCODE_ACI318_19) { return TRUE; }
		if (strCode == CONCODE_ACI318M_19) { return TRUE; }
		if (strCode == CONCODE_ACI318_25) { return TRUE; }
		if (strCode == CONCODE_ACI318M_25) { return TRUE; }
		if (strCode == CONCODE_NSCP_2015) { return TRUE; }
		if (strCode == CONCODE_NTC_DCEC_2017) { return TRUE; }
		if (strCode == CONCODE_NTC_DCEC_2023) { return TRUE; }
		return FALSE;
	}();

	if (m_bShowBeamDeflect == bShowBeamDeflect_new) return;

	CArray<UINT, UINT> aCtrlID, aCtrlID_move;
	CArray<UINT, UINT> aCtrlID_remove; aCtrlID_remove.Add(IDC_DGN_RCCTRL_BEAM_DEFLECT);
	CDlgUtil::GetCtrlAllIDbyDlg(this, aCtrlID);
	CArrayUtil::SubtractUINT(aCtrlID, aCtrlID_remove, aCtrlID_move);

	int nDistY = m_nBeamDeflectSpan;
	if (!m_bShowBeamDeflect) nDistY *= -1;

	CDlgUtil::CtrlMoveDistY(this, aCtrlID_move, nDistY);

	GetDlgItem(IDC_DGN_RCCTRL_BEAM_DEFLECT)->ShowWindow(bShowBeamDeflect_new ? SW_SHOW : SW_HIDE);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	m_bShowBeamDeflect = bShowBeamDeflect_new;
}
void CDgnConCodeNewChild12Dlg::SetAlpha2Colm(const CString& strCode)
{
	BOOL bShowAlpha2Colm_new = UseAlpha2Colm(strCode);

	if (m_bShowAlpha2Colm == bShowAlpha2Colm_new) return;

	CRect RefRect, TgtRect;

	GetDlgItem(IDC_DGN_EQ_FACTOR20)->GetWindowRect(&RefRect);
	GetDlgItem(IDC_DGN_EQ_FACTOR25)->GetWindowRect(&TgtRect);

	CDlgUtil::CtrlShowHide(this, m_arCtrlAlphaColm, bShowAlpha2Colm_new);

	int nDistY = RefRect.bottom - TgtRect.bottom;
	if (!m_bShowAlpha2Colm) nDistY *= -1;

	CDlgUtil::CtrlMoveDistY(this, m_aSCWBDesign, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMembTypeSeisDgn, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aBeamColmJoint, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aExposureCond, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aBeamCalcMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aInteractionShear, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);


	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	m_bShowAlpha2Colm = bShowAlpha2Colm_new;
}

void CDgnConCodeNewChild12Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();
	m_dMrft = m_pData->dMrft;
	m_dTrft = m_pData->dTrft;
	m_iAlphaType1 = m_pData->iAlphaType;
	m_dAlpha1 = m_pData->dAlpha1;
	m_dAlpha2 = m_pData->dAlpha2;
	m_dAlpha2_Colm = m_pData->dAlpha2_Colm;
	m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
	m_bTorsionDesign = m_pData->bTorsionDesign;
	m_bSpecialWall = m_pData->nSpecialWall;

	m_nBndrElemMethod = m_pData->iBndrElemMethod;
	m_dCd = m_pData->dCd;
	m_dIe = m_pData->dIe;

	m_nExposure = m_pData->nExposure;
	m_bSubBeam = m_pData->bNonSeisSubBeam;
	m_bCantilever = m_pData->bNonSeisCantilever;
	m_bUnderBmColm = m_pData->bNonSeisUnderGround;
	m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
	m_nPMCurveMethod = m_pData->nPMCurveMethod;
	m_nBeamRebarConsiderOpt = m_pData->nBeamRebarConsiderOpt;
	m_bConsiderBeamElemForce = m_pData->bConsiderBeamElemForce;
	m_nCheckPos = m_pData->nCheckPos;
	m_bConsiderInteractionShear = m_pData->bConsiderInteractShear;
	m_nFsMethod = m_pData->nFsMethod4Beam;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
}

void CDgnConCodeNewChild12Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	// 	SetShowBeamDeflection(strCode);
	SetShowExposure(strCode);
	SetAlpha2Colm(strCode);
	SetShowPMCurveMethod(strCode);

	GetInitValue(strCode);
	UpdateData(FALSE);

	SetComboBox();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowByBeamColmJointDesign(strCode);
}

void CDgnConCodeNewChild12Dlg::ResizeChildDlg()
{
	BOOL bShowFrame = m_iFrameType != 2;
	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	BOOL bSpecialEQShowFrame = (!bSeisParamFolded && m_bSpecialEQ && bShowFrame);

	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_SEIS_PARAM_GRB, m_bSpecialEQ, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aShearWallType, bSpecialEQShowFrame);
	CDlgUtil::CtrlShowHide(this, m_aShearforDesign, bSpecialEQShowFrame);
	CDlgUtil::CtrlShowHide(this, m_aMembTypeSeisDgn, bSpecialEQShowFrame);
	CDlgUtil::CtrlShowHide(this, m_aSCWBDesign, bSpecialEQShowFrame);

	UINT nIDRef2 = 0;
	UINT nIDTgt2 = 0;
	CRect RectRef2, RectTgt2;

	GetDlgItem(IDC_DGN_RCCTRL_SPECIAL)->GetWindowRect(&RectTgt2);
	int nSpace = (int)RectTgt2.Height() / 2;

	if (bShowFrame)	nIDRef2 = IDC_DGN_SCWB_METHOD_GRP;
	else			nIDRef2 = IDC_DGN_COLM_WEAK_BEAM_CHK;
	nIDTgt2 = IDC_DGN_EUROCODE204_N17;
	GetDlgItem(nIDRef2)->GetWindowRect(&RectRef2);
	GetDlgItem(nIDTgt2)->GetWindowRect(&RectTgt2);
	
	int nDistY2 = RectRef2.bottom - RectTgt2.top + nSpace;

	CDlgUtil::CtrlMoveDistY(this, m_aBeamColmJoint, nDistY2);

	if (bSpecialEQShowFrame)
		ReAlingShearDesignCtrl();

	CDlgUtil::CtrlShowHide(this, m_arCtrlAlphaColm, m_bShowAlpha2Colm);

	UINT nIDRef = 0;
	UINT nIDTgt = 0;
	CRect RectRef, RectTgt;

	if (!bSeisParamFolded)
	{
		if (bShowFrame)	nIDRef = IDC_DGN_KCI_USD07_N1;
		else			nIDRef = IDC_DGN_EUROCODE204_N19_GRB;
		nIDTgt = IDC_DGN_SEIS_PARAM_GRB;
		GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
		GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
		ScreenToClient(RectRef);
		ScreenToClient(RectTgt);
		RectTgt.bottom = RectRef.bottom + nSpace;
		GetDlgItem(nIDTgt)->MoveWindow(RectTgt, TRUE);
	}

	nIDRef = IDC_DGN_RCCTRL_SPECIAL;
	nIDTgt = IDC_DGN_RCCTRL_TORSION;
	if (m_bSpecialEQ) nIDRef = IDC_DGN_SEIS_PARAM_GRB;

	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	int nDistY = RectRef.bottom - RectTgt.top + nSpace;

	if (nDistY != 0)
	{
		CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aExposureCond, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aBeamCalcMethod, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aInteractionShear, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);
	}

	CDlgUtil::CtrlShowHide(this, m_aTorsionDesign, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aMomentRedistr, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aExposureCond, m_bShowExposure);
	CDlgUtil::CtrlShowHide(this, m_aBeamCalcMethod, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aPMCurveMethod, m_bShowPMCurveMethod);
	CDlgUtil::CtrlShowHide(this, m_aInteractionShear, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aFsMethod, SW_SHOW);

	nIDRef = IDC_DGN_RCCTRL_MRFT_STATIC;
	if (m_bShowPMCurveMethod) nIDRef = IDC_DGN_RCCTRL_INTERSHEAR2;
	else if (m_bShowExposure) nIDRef = IDC_DGN_EXPOSURE_STC;

	nIDTgt = IDC_DGN_FS_METHOD_FRM;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	nDistY = RectRef.bottom - RectTgt.top + nSpace;
	CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);


	nIDRef = IDC_DGN_FS_METHOD_FRM;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	ScreenToClient(RectRef);
	GetClientRect(&RectTgt);
	RectTgt.bottom = RectRef.bottom + nSpace;
	MoveWindow(RectTgt, TRUE);

	Invalidate();

	CDconChildDialog::ResizeChildDlg();
}

void CDgnConCodeNewChild12Dlg::ReAlingShearDesignCtrl()
{
	// Shear for Design 위치를 위해서 사용한 Ctrl 들은 우선 안보이게 함
	GetDlgItem(IDC_DGN_EQ_FACTOR16)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR17)->ShowWindow(SW_HIDE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	int iStatus = GetDataStatus(strCode);

	CWnd* pSDR = GetDlgItem(IDC_DGN_EQ_FACTOR11);
	CWnd* pSDRHide = GetDlgItem(IDC_DGN_EQ_FACTOR16);
	CWnd* pSDRShow = GetDlgItem(IDC_DGN_EQ_FACTOR17);
	CRect SDRRect, SDRHideRect, SDRShowRect;
	pSDR->GetWindowRect(SDRRect);
	pSDRHide->GetWindowRect(SDRHideRect);
	pSDRShow->GetWindowRect(SDRShowRect);

	int nDistY = 0;
	if ((iStatus == 10 && m_iFrameType == 0) || (iStatus == 18 && m_iFrameType == 0) || (iStatus == 20 && m_iFrameType == 0) || iStatus == 15)
	{
		nDistY = SDRShowRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_HIDE);
		if (m_bShowAlpha2Colm)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_EQ_FACTOR25)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_EQ_FACTOR25)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		nDistY = SDRHideRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR25)->ShowWindow(SW_HIDE);
		if (m_bShowAlpha2Colm)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_HIDE);
		}
	}

	if (nDistY != 0)
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlSDR, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aSCWBDesign, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aMembTypeSeisDgn, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aBeamColmJoint, nDistY);
	}
}

void CDgnConCodeNewChild12Dlg::EnableWindowBySpecialWall()
{
	if (m_bSpecialWall)
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

void CDgnConCodeNewChild12Dlg::SetEQCtrl(const CString& strCode)
{
	CString strTxt = _T("");
	int iStatus = GetDataStatus(strCode);

	if (iStatus == 9 || iStatus == 10 || iStatus == 11 || iStatus == 12 || iStatus == 13 ||
		iStatus == 14 || iStatus == 15 || iStatus == 16 || iStatus == 18 || iStatus == 20)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arCtrlAlphaColm, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arCtrlSDR, m_iFrameType != 2);

		if (m_iAlphaType1 == 0 || m_iAlphaType1 == 1)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_arCtrlAlphaColm, TRUE);
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
			CDlgUtil::CtrlEnableDisable(this, m_arCtrlAlphaColm, TRUE);
		}
	}

	CString strQ = _T("");
	if (iStatus == 9)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_AIJ));
	}
	else if (iStatus == 10 || iStatus == 18 || iStatus == 20)
	{
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
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if (iStatus == 12)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_KSCE));
	}
	else if (iStatus == 11 || iStatus == 13 || iStatus == 14)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if (iStatus == 15)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_TWN));
	}
	else if (iStatus == 16)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_IS));
	}

	if (UseAlpha2Colm(strCode) && m_iFrameType != 2) GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI_BEAM));
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
	if ((iStatus == 10 && m_iFrameType == 0) || (iStatus == 18 && m_iFrameType == 0) || (iStatus == 20 && m_iFrameType == 0) || iStatus == 15)
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
}

BOOL CDgnConCodeNewChild12Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft = m_dMrft;
	m_pData->dTrft = m_dTrft;
	m_pData->iAlphaType = m_iAlphaType1;
	m_pData->dAlpha1 = m_dAlpha1;
	m_pData->dAlpha2 = m_dAlpha2;
	m_pData->dAlpha2_Colm = m_dAlpha2_Colm;
	m_pData->dConShearStrengthRatio = m_dConShearStrengthRatio;
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->bCheckBeamDeflect = m_bBeamDeflect;

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	if (UseBeamColmJointDesign(strCode))m_pData->bBCJointDgn = m_bBCJointDgn;

	// Save KCI-USD07
	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);
	m_dCd = _ttof(strNewValue);
	m_CmbIe.GetWindowText(strNewValue);
	m_dIe = _ttof(strNewValue);

	m_pData->nSpecialWall = m_bSpecialWall;

	m_pData->iBndrElemMethod = m_nBndrElemMethod;
	m_pData->dCd = m_dCd;
	m_pData->dIe = m_dIe;

	// Exposure Condition
	m_pData->nExposure = m_nExposure;
	m_pData->bNonSeisSubBeam = m_bSubBeam;
	m_pData->bNonSeisCantilever = m_bCantilever;
	m_pData->bNonSeisUnderGround = m_bUnderBmColm;
	m_pData->bStrongColmWeakBeam = m_bStrongColmWeakBeam;
	m_pData->nPMCurveMethod = m_nPMCurveMethod;
	m_pData->nCheckPos = m_nCheckPos;
	m_pData->bConsiderInteractShear = m_bConsiderInteractionShear;
	m_pData->nBeamRebarConsiderOpt = m_nBeamRebarConsiderOpt;
	m_pData->bConsiderBeamElemForce = m_bConsiderBeamElemForce;
	m_pData->nFsMethod4Beam = m_nFsMethod;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);

	CDconChildDialog::Dlg2Data();
	return TRUE;
}

void CDgnConCodeNewChild12Dlg::OnBCJoint()//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
{
	UpdateData(TRUE);
	BOOL bJoint = m_bBCJointDgn;
	CDlgUtil::CtrlEnableDisable(this, m_aSelectPosGroup, bJoint);
}

LRESULT CDgnConCodeNewChild12Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
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