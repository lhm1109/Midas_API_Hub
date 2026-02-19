// DgnConCodeNewChild_11_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_11_Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild11Dlg dialog


CDgnConCodeNewChild11Dlg::CDgnConCodeNewChild11Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild11Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild11Dlg)
	m_bStrongColmWeakBeam = FALSE;
	m_bSpecialWall = FALSE;
	m_bBeamDeflect = FALSE;
	m_nBndrElemMethod = 0;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dAlpha2_Colm = 0.0;
	m_dMrft = 0.0;
	m_dTrft = 0.0;
	m_dCd = 4.5;
	m_dIe = 1.2;
	m_dConShearStrengthRatio = 0.0;
	m_nPMCurveMethod = 0;
	m_bSubBeam = FALSE;
	m_bCantilever = FALSE;
	m_bUnderBmColm = FALSE;
	m_bNotConsiderk1 = FALSE;
	m_dPhic = 0.65;
	m_bConsiderInteractionShear = FALSE;
	m_nCheckPos = EN_SELECT_CHECK_POS_BOT;
#ifdef _ORG
	m_nBeamRebarConsiderOpt = EN_MCMB_EQUI;
#else
	m_nBeamRebarConsiderOpt = EN_MCMB_EACH;
#endif // _ORG

	m_bConsiderBeamElemForce = FALSE;
	m_nFsMethod = EN_FSMETHOD_2_3_FY;

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
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR12);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR13);
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
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR20);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR21);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR22);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR23);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR24);

	m_arCtrlAlphaColm.RemoveAll();
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR22);
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR23);
	m_arCtrlAlphaColm.Add(IDC_DGN_EQ_FACTOR24);

	m_aSCWBDesign.RemoveAll();
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_GRP);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);

	m_aRdoSCWBMethod.RemoveAll();
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);
	m_aRdoSCWBMethod.FreeExtra();

	m_aOptSCWBDesign.RemoveAll();
	m_aOptSCWBDesign.Add(IDC_DGN_SCWB_NOTK1);
	m_aOptSCWBDesign.Add(IDC_DGN_SCWB_PHIC_STATIC);
	m_aOptSCWBDesign.Add(IDC_DGN_SCWB_PHIC);
	m_aOptSCWBDesign.FreeExtra();
	
	m_aSelectPosGroup.RemoveAll();
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_N19_GRB);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_TOP_RDO);
	m_aSelectPosGroup.Add(IDC_DGN_EUROCODE204_BOT_RDO);

	m_aMembTypeSeisDgn.RemoveAll();
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N1);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N2);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N3);
	m_aMembTypeSeisDgn.Add(IDC_DGN_KCI_USD07_N4);
	m_aMembTypeSeisDgn.Add(IDC_DGN_EUROCODE204_N17);

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_TORSION_GROUP);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_STATIC);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_FACTOR);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_aBeamCalcMethod.RemoveAll();
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD_EQUI_RADIO);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_CALC_METHOD_EACH_RADIO);
	m_aBeamCalcMethod.Add(IDC_DGN_BEAM_DESIGN_EACH_ELEM_CHK2);

	m_aPMCurveMethod.RemoveAll();
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_P_RADIO);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_MP_RADIO);

	m_aInteractionShear.RemoveAll();
	m_aInteractionShear.Add(IDC_DGN_RCCTRL_INTERSHEAR);
	
	m_aFsMethod.RemoveAll();
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_FRM);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_2_3_FY_RADIO);
	m_aFsMethod.Add(IDC_DGN_FS_METHOD_PROGRAM_RADIO2);

	m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeNewChild11Dlg::~CDgnConCodeNewChild11Dlg()
{
	_SAFE_DELETE(m_pDgnDataCtrl);
}

void CDgnConCodeNewChild11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild11Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK, m_bStrongColmWeakBeam);
	DDX_Check(pDX, IDC_DGN_WALLTYPE_N1, m_bSpecialWall);
	DDX_Check(pDX, IDC_DGN_RCCTRL_BEAM_DEFLECT2, m_bBeamDeflect);

	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Radio(pDX, IDC_DGN_WALLTYPE_N3, m_nBndrElemMethod);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7, m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR24, m_dAlpha2_Colm);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);

	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);
	DDX_Radio(pDX, IDC_DGN_EUROCODE204_TOP_RDO, m_nCheckPos);

	DDX_Control(pDX, IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);

	DDX_Check(pDX, IDC_DGN_KCI_USD07_N2, m_bSubBeam);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N3, m_bCantilever);
	DDX_Check(pDX, IDC_DGN_KCI_USD07_N4, m_bUnderBmColm);
	DDX_Check(pDX, IDC_DGN_EUROCODE204_N17, m_bBCJointDgn);
	DDX_Check(pDX, IDC_DGN_SCWB_NOTK1, m_bNotConsiderk1);
	DDX_Text(pDX, IDC_DGN_SCWB_PHIC, m_dPhic);
	DDX_Check(pDX, IDC_DGN_RCCTRL_INTERSHEAR, m_bConsiderInteractionShear);

	DDX_Radio(pDX, IDC_DGN_BEAM_CALC_METHOD_EQUI_RADIO, m_nBeamRebarConsiderOpt);
	DDX_Check(pDX, IDC_DGN_BEAM_DESIGN_EACH_ELEM_CHK2, m_bConsiderBeamElemForce);
	DDX_Control(pDX, IDC_DGN_SEIS_PARAM_GRB, m_grpSeisParam);
	DDX_Radio(pDX, IDC_DGN_FS_METHOD_2_3_FY_RADIO, m_nFsMethod);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N5, m_CmbCd);
	DDX_Control(pDX, IDC_DGN_WALLTYPE_N7, m_CmbIe);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild11Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild11Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N1, OnChkSpecialWall)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N3, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_WALLTYPE_N8, OnChangeBndrElemMethodType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
	ON_BN_CLICKED(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO, OnChangeSCWBMethod)
	ON_BN_CLICKED(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO, OnChangeSCWBMethod)
	ON_BN_CLICKED(IDC_DGN_EUROCODE204_N17, OnBCJoint)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild11Dlg message handlers

BOOL CDgnConCodeNewChild11Dlg::OnInitDialog()
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();

	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild11Dlg::InitCtrl()
{
	m_grpSeisParam.InitControl(this, CDgnConCodeNewChild11Dlg::IDD, IDC_DGN_SEIS_PARAM_GRB, TRUE, TRUE);
	m_grpSeisParam.SetFoldState(FALSE); // 무조건 펼친다
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if (!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용
		m_pData->nSCWBDgnMethod = 1; // 1: Norminal Strength
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
	}
	else
	{
		m_dCd = m_pData->dCd >= 1.0 ? m_pData->dCd : 1.0;
		m_dIe = m_pData->dIe >= 1.0 ? m_pData->dIe : 1.0;
		m_bSpecialWall = m_pData->nSpecialWall > 0 ? TRUE : FALSE;
		m_nBndrElemMethod = (m_pData->iBndrElemMethod == 0 || m_pData->iBndrElemMethod == 1) ? m_pData->iBndrElemMethod : 0;

		m_dMrft = m_pData->dMrft;
		m_dTrft = m_pData->dTrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;
		m_dAlpha2_Colm = m_pData->dAlpha2_Colm;
		m_bBCJointDgn = m_pData->bBCJointDgn;
		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_bBeamDeflect = m_pData->bCheckBeamDeflect;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
		m_nPMCurveMethod = m_pData->nPMCurveMethod;
		m_bSubBeam = m_pData->bNonSeisSubBeam;
		m_bCantilever = m_pData->bNonSeisCantilever;
		m_bUnderBmColm = m_pData->bNonSeisUnderGround;
		m_bNotConsiderk1 = m_pData->bNotConsiderk1;
		m_dPhic = m_pData->dSCWB_Phic;
		m_nCheckPos = m_pData->nCheckPos;
		m_bConsiderInteractionShear = m_pData->bConsiderInteractShear;
		m_nBeamRebarConsiderOpt = m_pData->nBeamRebarConsiderOpt;
		m_bConsiderBeamElemForce = m_pData->bConsiderBeamElemForce;
		m_nFsMethod = m_pData->nFsMethod4Beam;
	}

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	SetComboCdAndIe();
	GetInitValue(strCode);
	UpdateData(FALSE);

	OnChangeTorsion();
	OnChangeSCWBMethod();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	EnableWindowBySpecialWall();

	OnBCJoint();
}

void CDgnConCodeNewChild11Dlg::OnDgnRcctrlSpecial()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	GetInitValue(strCode);

	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild11Dlg::GetInitValue(CString strCode)
{
	if (!m_bExistData)
	{
		m_iAlphaType1 = 1;
		m_dAlpha1 = 1.0;
		m_dAlpha2 = 2.0;
		m_dAlpha2_Colm = 2.0;
		m_iFrameType = 1;
	}
	BOOL bDefOK = (m_dAlpha1 * m_dAlpha2 == 0.0 ? TRUE : FALSE);
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	if (bDefOK) m_dAlpha2_Colm = m_dAlpha2;

}

void CDgnConCodeNewChild11Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild11Dlg::OnChkSpecialWall()
{
	UpdateData(TRUE);
	EnableWindowBySpecialWall();
}
void CDgnConCodeNewChild11Dlg::OnChangeBndrElemMethodType()
{
	UpdateData(TRUE);
	EnableWindowByBndrElemMethod();
}
void CDgnConCodeNewChild11Dlg::OnChangeSCWBMethod()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, nCheck);
	GetDlgItem(IDC_DGN_SCWB_PHIC_STATIC)->EnableWindow(nCheck == 1 ? TRUE : FALSE);
	GetDlgItem(IDC_DGN_SCWB_PHIC)->EnableWindow(nCheck == 1 ? TRUE : FALSE);
}


void CDgnConCodeNewChild11Dlg::OnUpdateByCode()
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

void CDgnConCodeNewChild11Dlg::OnChangeTorsion()
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

void CDgnConCodeNewChild11Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft = m_pData->dMrft;
	m_dTrft = m_pData->dTrft;
	m_iAlphaType1 = m_pData->iAlphaType;
	m_dAlpha1 = m_pData->dAlpha1;
	m_dAlpha2 = m_pData->dAlpha2;
	m_bTorsionDesign = m_pData->bTorsionDesign;
	m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;

	m_bSpecialWall = m_pData->nSpecialWall;

	m_nBndrElemMethod = m_pData->iBndrElemMethod;
	m_dCd = m_pData->dCd;
	m_dIe = m_pData->dIe;

	m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
	m_nPMCurveMethod = m_pData->nPMCurveMethod;
	m_bSubBeam = m_pData->bNonSeisSubBeam;
	m_bCantilever = m_pData->bNonSeisCantilever;
	m_bUnderBmColm = m_pData->bNonSeisUnderGround;
	m_bNotConsiderk1 = m_pData->bNotConsiderk1;
	m_dPhic = m_pData->dSCWB_Phic;
	m_bConsiderInteractionShear = m_pData->bConsiderInteractShear;
	m_nBeamRebarConsiderOpt = m_pData->nBeamRebarConsiderOpt;
	m_bConsiderBeamElemForce = m_pData->bConsiderBeamElemForce;
	m_nFsMethod = m_pData->nFsMethod4Beam;
	m_nCheckPos = m_pData->nCheckPos;
	m_bBCJointDgn = m_pData->bBCJointDgn;
	m_bBeamDeflect = m_pData->bCheckBeamDeflect;

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
}

void CDgnConCodeNewChild11Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	UpdateData(FALSE);

	SetComboCdAndIe();
	
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
	GetInitValue(strCode);
	UpdateData(FALSE);

	ResizeChildDlg();
}

void CDgnConCodeNewChild11Dlg::ResizeChildDlg()
{
	const bool bTWN_USD112 = true;
	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	BOOL bSpecialEQShowFrame = (!bSeisParamFolded && m_bSpecialEQ);

	CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_SEIS_PARAM_GRB, m_bSpecialEQ, TRUE);
// 	CDlgUtil::CtrlShowHide(this, m_aStrongColmWeakBeam, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aShearWallType, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aShearforDesign, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aSCWBDesign, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aOptSCWBDesign, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aMembTypeSeisDgn, bSpecialEQShowFrame);
// 	CDlgUtil::CtrlShowHide(this, m_aSelectPosGroup, bSpecialEQShowFrame);

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

	nIDRef = IDC_DGN_RCCTRL_SPECIAL;
	nIDTgt = IDC_DGN_RCCTRL_TORSION;
	if (m_bSpecialEQ) nIDRef = IDC_DGN_SEIS_PARAM_GRB;
	
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	int nDistY = RectRef.bottom - RectTgt.top + RectTgt.Height() / 2;

	if (nDistY != 0)
	{
		CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aBeamCalcMethod, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aInteractionShear, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aFsMethod, nDistY);
	}

	CDlgUtil::CtrlShowHide(this, m_aTorsionDesign, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aMomentRedistr, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aBeamCalcMethod, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aPMCurveMethod, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aInteractionShear, SW_SHOW);
	CDlgUtil::CtrlShowHide(this, m_aFsMethod, SW_SHOW);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	CDconChildDialog::ResizeChildDlg();
}

void CDgnConCodeNewChild11Dlg::SetComboCdAndIe()
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

void CDgnConCodeNewChild11Dlg::EnableWindowBySpecialWall()
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

void CDgnConCodeNewChild11Dlg::EnableWindowByBndrElemMethod()
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

void CDgnConCodeNewChild11Dlg::SetEQCtrl(const CString& strCode)
{
	GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlAlphaColm, FALSE);
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


	CString strQ = _T("");
	strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
	GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
	GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
	GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_TWN));


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

	BOOL bSeisParamFolded = m_grpSeisParam.GetFoldState();
	BOOL bSpecialEQShowFrame = (!bSeisParamFolded && m_bSpecialEQ);
	int nCmdShow = bSpecialEQShowFrame ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(nCmdShow);

}

LRESULT CDgnConCodeNewChild11Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
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

void CDgnConCodeNewChild11Dlg::OnBCJoint()//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121108 #1
{
	UpdateData(TRUE);
	BOOL bJoint = m_bBCJointDgn;
	CDlgUtil::CtrlEnableDisable(this, m_aSelectPosGroup, bJoint);
}

BOOL CDgnConCodeNewChild11Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft = m_dMrft;
	m_pData->dTrft = m_dTrft;
	m_pData->iAlphaType = m_iAlphaType1;
	m_pData->dAlpha1 = m_dAlpha1;
	m_pData->dAlpha2 = m_dAlpha2;
	m_pData->dAlpha2_Colm = m_dAlpha2_Colm;
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->dConShearStrengthRatio = m_dConShearStrengthRatio;
	m_pData->bStrongColmWeakBeam = m_bStrongColmWeakBeam;
	m_pData->bNonSeisSubBeam = m_bSubBeam;
	m_pData->bNonSeisCantilever = m_bCantilever;
	m_pData->bNonSeisUnderGround = m_bUnderBmColm;
	m_pData->bNotConsiderk1 = m_bNotConsiderk1;
	m_pData->dSCWB_Phic = m_dPhic;
	m_pData->bConsiderInteractShear = m_bConsiderInteractionShear;
	m_pData->bBCJointDgn = m_bBCJointDgn;
	m_pData->bCheckBeamDeflect = m_bBeamDeflect;
	m_pData->nBeamRebarConsiderOpt = m_nBeamRebarConsiderOpt;
	m_pData->bConsiderBeamElemForce = m_bConsiderBeamElemForce;
	m_pData->nFsMethod4Beam = m_nFsMethod;
	m_pData->nCheckPos = m_nCheckPos;

	CString strNewValue = _T("");
	m_CmbCd.GetWindowText(strNewValue);
	m_dCd = _ttof(strNewValue);
	m_CmbIe.GetWindowText(strNewValue);
	m_dIe = _ttof(strNewValue);

	m_pData->nSpecialWall = m_bSpecialWall;

	m_pData->iBndrElemMethod = m_nBndrElemMethod;
	m_pData->dCd = m_dCd;
	m_pData->dIe = m_dIe;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
	m_pData->nPMCurveMethod = m_nPMCurveMethod;
	CDconChildDialog::Dlg2Data();
	return TRUE;
}