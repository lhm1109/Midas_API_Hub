// CMEBeamCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEBeamCalcDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_rcs\RCS_ServiceDef.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMEBeamCalcDlg dialog

#define RES_TYPE_EFF 0
#define RES_TYPE_KFP 1
#define RES_TYPE_KDD 2
#define RES_TYPE_ALP 3

CCMEBeamCalcDlg::CCMEBeamCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMEBeamCalcDlg::IDD, pParent)
{
	//m_pDoc = pDoc;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMEBeamCalcDlg)
	//}}AFX_DATA_INIT
	//GetDefaultData(m_RcsFS);
	GetDefaultData(m_RcsFS_Init);
	m_dCaculatedWidth = 0.0;

	m_aCtrlGrossman.RemoveAll();
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_SLAB_GRP);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_D_STC);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_D_EDT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_D_UNT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_H_STC);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_H_EDT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_H_UNT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_LOCEBEAM_GRP);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_LOCEBEAM_CMB);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_SUPPORT_GRP);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_SUPPORT_CMB);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_KFP_STC);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_KFP_EDT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_LATER_GRP);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_LATERAL_CMB);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_KD_STC);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_KD_EDT);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_A_STC);
	m_aCtrlGrossman.Add(IDC_CMD_EBEAM_CALC_GROSS_A_EDT);

	m_aCtrlChoiSong.RemoveAll();
	m_aCtrlChoiSong.Add(IDC_CMD_EBEAM_CALC_CHOIS_SUPPORT_GRP);
	m_aCtrlChoiSong.Add(IDC_CMD_EBEAM_CALC_CHOIS_SUPPORT_CMB);
	m_aCtrlChoiSong.Add(IDC_CMD_EBEAM_CALC_CHOIS_STIFFNESS_GRP);
	m_aCtrlChoiSong.Add(IDC_CMD_EBEAM_CALC_CHOIS_STIFFNESS_CMB);

	m_aCtrlEtc.RemoveAll();
	m_aCtrlEtc.Add(IDC_CMD_EBEAM_CALC_CALCULATE_BTN);
	m_aCtrlEtc.Add(IDC_CMD_EBEAM_CALC_CALCULATE_EDT);
	m_aCtrlEtc.Add(IDC_CMD_EBEAM_ITEM_APPLY_BTN);
	m_aCtrlEtc.Add(IDCANCEL);
}

void CCMEBeamCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMEBeamCalcDlg)
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_METHOD_CMB    , m_cobxMethod );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_LOCEBEAM_CMB    , m_cobxGrLocEB);  
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_SUPPORT_CMB    , m_cobxGrSupport);
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_LATERAL_CMB    , m_cobxGrLateral);
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_CHOIS_SUPPORT_CMB    , m_cobxCSLocEB  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_CHOIS_STIFFNESS_CMB  , m_cobxCSJoint  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L1_EDT  , m_edtL1 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L2_EDT  , m_edtL2 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L3_EDT  , m_edtL3 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_C1_EDT  , m_edtC1 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_C2_EDT  , m_edtC2 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_D_EDT   , m_edtGrD  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_H_EDT   , m_edtGrH  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_KFP_EDT , m_edtGrKFP);
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_KD_EDT  , m_edtGrKD );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_A_EDT   , m_edtGrA  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L1_UNT  , m_untGrL1 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L2_UNT  , m_untGrL2 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_L3_UNT  , m_untGrL3 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_C1_UNT  , m_untGrC1 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_C2_UNT  , m_untGrC2 );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_D_UNT   , m_untGrD  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_GROSS_H_UNT   , m_untGrH  );
	DDX_Control(pDX, IDC_CMD_EBEAM_CALC_CALCULATE_EDT , m_edtCalcResult );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMEBeamCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMEBeamCalcDlg)
	ON_BN_CLICKED   (IDC_CMD_EBEAM_ITEM_APPLY_BTN , OnApply)
	ON_BN_CLICKED   (IDC_CMD_EBEAM_CALC_CALCULATE_BTN, OnCalculate)
	ON_CBN_SELCHANGE(IDC_CMD_EBEAM_CALC_METHOD_CMB, OnSelchangeMethodCmb)
	ON_CBN_SELCHANGE (IDC_CMD_EBEAM_CALC_GROSS_SUPPORT_CMB, OnChangeSupport)
	//ON_CBN_EDITCHANGE(IDC_CMD_EBEAM_CALC_GROSS_SUPPORT_CMB, OnChangeSupport)
	ON_CBN_SELCHANGE (IDC_CMD_EBEAM_CALC_GROSS_LATERAL_CMB, OnChangeLateral)
	//ON_CBN_EDITCHANGE(IDC_CMD_EBEAM_CALC_GROSS_LATERAL_CMB, OnChangeLateral)

	ON_CBN_SELCHANGE(IDC_CMD_EBEAM_CALC_GROSS_LOCEBEAM_CMB , OnChangeGrLocEBCmb)
	ON_CBN_SELCHANGE(IDC_CMD_EBEAM_CALC_CHOIS_SUPPORT_CMB  , OnChangeCsLocEBCmb)
	ON_CBN_SELCHANGE(IDC_CMD_EBEAM_CALC_CHOIS_STIFFNESS_CMB, OnChangeCsJointCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMEBeamCalcDlg message handlers
BOOL CCMEBeamCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombobox();
	AlignCtrl();
	Data2Dlg();

	ShowHideCtrl();
	OnChangeSupport();
	OnChangeLateral();
	EnableDisableCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMEBeamCalcDlg::OnSelchangeMethodCmb()
{
	ShowHideCtrl();
	EnableDisableCtrls();
}

void CCMEBeamCalcDlg::OnChangeSupport()
{
	double dKFP = GetValue(RES_TYPE_KFP);
	m_edtGrKFP.SetEditUnit(dKFP);
}

void CCMEBeamCalcDlg::OnChangeLateral()
{
	double dAlpha = GetValue(RES_TYPE_ALP);
	m_edtGrA.SetEditUnit(dAlpha);

	double dKd = GetValue(RES_TYPE_KDD);
	m_edtGrKD.SetEditUnit(dKd);
}

void CCMEBeamCalcDlg::InitCombobox()
{
	int index;
	index = m_cobxMethod.AddString(_LS(IDS_CMD_EBEAM_CALC_GROSSMAN));
	m_cobxMethod.SetItemData(index,index);
	index = m_cobxMethod.AddString(_LS(IDS_CMD_EBEAM_CALC_CHOISONG));
	m_cobxMethod.SetItemData(index,index);

	m_cobxGrLocEB.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_Interior));
	m_cobxGrLocEB.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_Edge    ));
	m_cobxGrLocEB.SetItemData(0, STRIP_TYPE_INT);
	m_cobxGrLocEB.SetItemData(1, STRIP_TYPE_EDG);

	m_cobxGrSupport.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Interior));
	m_cobxGrSupport.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Exterior));
	m_cobxGrSupport.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Edge    ));
	m_cobxGrSupport.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Corner  ));
	m_cobxGrLocEB.SetItemData(0, COLPOS_INT);
	m_cobxGrLocEB.SetItemData(1, COLPOS_EXT);
	m_cobxGrLocEB.SetItemData(2, COLPOS_EDG);
	m_cobxGrLocEB.SetItemData(3, COLPOS_COR);

	m_cobxGrLateral.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs100   ));
	m_cobxGrLateral.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs200   ));
	m_cobxGrLateral.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs400   ));
	m_cobxGrLateral.AddString(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs800   ));
	m_cobxGrLocEB.SetItemData(0, DRIFT_0100);
	m_cobxGrLocEB.SetItemData(1, DRIFT_0200);
	m_cobxGrLocEB.SetItemData(2, DRIFT_0400);
	m_cobxGrLocEB.SetItemData(3, DRIFT_0800);
	
	m_cobxCSLocEB.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_Interior));
	m_cobxCSLocEB.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_Edge    ));
	m_cobxCSLocEB.SetItemData(0, STRIP_TYPE_INT);
	m_cobxCSLocEB.SetItemData(1, STRIP_TYPE_EDG);
	
	m_cobxCSJoint.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_RigidBar));
	m_cobxCSJoint.AddString(_LS(IDS_CMD_EBEAM_CALC_CS_SoftBar ));
	m_cobxCSJoint.SetItemData(0, STRIP_BAR_TYPE_RIGD);
	m_cobxCSJoint.SetItemData(1, STRIP_BAR_TYPE_SOFT);
}

void CCMEBeamCalcDlg::AlignCtrl()
{
	CRect rRef, rToMove;
	GetDlgItem(IDC_CMD_EBEAM_CALC_GROSS_SLAB_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_EBEAM_CALC_CHOIS_SUPPORT_GRP)->GetWindowRect(rToMove);
	int dX, dY;
	dX = rRef.left - rToMove.left;
	dY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlChoiSong, dX, dY);
}

void CCMEBeamCalcDlg::ShowHideCtrl()
{
	int nData = m_cobxMethod.GetItemData(m_cobxMethod.GetCurSel());
	CDlgUtil::CtrlShowHide(this, m_aCtrlGrossman, nData==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlChoiSong, nData==1);

	// 컨트롤 높이 조정
	CRect refRect, moveRect;
	if(nData==0) GetDlgItem(IDC_CMD_EBEAM_CALC_GROSS_LATER_GRP)->GetWindowRect(refRect);
	else         GetDlgItem(IDC_CMD_EBEAM_CALC_CHOIS_STIFFNESS_GRP)->GetWindowRect(refRect);
	GetDlgItem(IDC_CMD_EBEAM_CALC_CALCULATE_BTN)->GetWindowRect(moveRect);
	int nDistY = refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlEtc, nDistY);

	// resize self size
	CRect rectLast;
	GetDlgItem(IDC_CMD_EBEAM_ITEM_APPLY_BTN)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMEBeamCalcDlg::InitUnit()
{
	m_edtL1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtL2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtL3 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtC1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtC2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrD  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrH  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrKFP.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrKD .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtGrA  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrL1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrL2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrL3 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrC1 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrC2 .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrD  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untGrH  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtCalcResult.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

double CCMEBeamCalcDlg::GetValue(int nResType)
{
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	if (pServiceHeadOffice == NULL) { ASSERT(0); return 0.0; }

	//RCSFS_EFF_WIDTH m_RcsFS;
	if (!GetInputData(m_RcsFS)) { ASSERT(0); return 0.0; }
	if (!CheckInputData(m_RcsFS)) { ASSERT(0); return 0.0; }

	int nReq = pServiceHeadOffice->ReqService(_ULS(rcs), EN_RCSSA_EFFECTIVE_BEAM_WIDTH_CALC, (void*)&m_RcsFS);
	if (nReq == -1) { ASSERT(0); return 0.0; }

	RCSFS_EFF_WIDTH_RES &CalcRes = m_RcsFS.Res;
	switch (nResType)
	{
		case RES_TYPE_EFF: return CalcRes.dBeff;
		case RES_TYPE_KFP: return CalcRes.dKfp;
		case RES_TYPE_KDD: return CalcRes.dKd;
		case RES_TYPE_ALP: return CalcRes.dAlpha;
	}
	ASSERT(0);
	return 0.0;
}

BOOL CCMEBeamCalcDlg::GetInputData(RCSFS_EFF_WIDTH &rRcsFS)
{
	rRcsFS.nMethod = m_cobxMethod.GetItemData(m_cobxMethod.GetCurSel());

	RCSFS_EFF_WIDTH_INP &Inp = rRcsFS.Inp;

	Inp.dC1 = m_edtC1.GetEditValue();
	Inp.dC2 = m_edtC2.GetEditValue();
	Inp.dL1 = m_edtL1.GetEditValue();
	Inp.dL2 = m_edtL2.GetEditValue();
	Inp.dL3 = m_edtL3.GetEditValue();

	Inp.dts = m_edtGrH.GetEditValue();
	Inp.dCc = m_edtGrD.GetEditValue();

	Inp.nDriftC = GetDriftCriteria();
	Inp.nDgnStr = GetStripType(rRcsFS.nMethod);
	Inp.nColPos = GetColumnPosition();
	if (rRcsFS.nMethod == STRIP_EFF_METHOD_CHOISONG)
	{
		Inp.nBarTyp = GetBarType();
	}

	return TRUE;
}

UINT CCMEBeamCalcDlg::GetStripType(int nMethod)
{
	CString csStripType = _T("");
	if     (nMethod==STRIP_EFF_METHOD_GROSSMAN) m_cobxGrLocEB.GetWindowText(csStripType);
	else if(nMethod==STRIP_EFF_METHOD_CHOISONG) m_cobxCSLocEB.GetWindowText(csStripType);

	if (csStripType.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Interior))==0) return STRIP_TYPE_INT;
	if (csStripType.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Edge    ))==0) return STRIP_TYPE_EDG;

	ASSERT(0);
	return STRIP_TYPE_INT;
}

UINT CCMEBeamCalcDlg::GetDriftCriteria()
{
	CString csDriftC;
	m_cobxGrLateral.GetWindowText(csDriftC);

	if (csDriftC.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs800))==0) return DRIFT_0800;
	if (csDriftC.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs400))==0) return DRIFT_0400;
	if (csDriftC.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs200))==0) return DRIFT_0200;
	if (csDriftC.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Hs100))==0) return DRIFT_0100;

	ASSERT(0);
	return DRIFT_0400;
}

UINT CCMEBeamCalcDlg::GetColumnPosition()
{
	CString csColPos;
	m_cobxGrSupport.GetWindowText(csColPos);

	if (csColPos.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Interior))==0) return COLPOS_INT;
	if (csColPos.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Exterior))==0) return COLPOS_EXT;
	if (csColPos.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Edge    ))==0) return COLPOS_EDG;
	if (csColPos.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_Gr_Corner  ))==0) return COLPOS_COR;
	
	ASSERT(0);
	return COLPOS_INT;
}

UINT CCMEBeamCalcDlg::GetBarType()
{
	CString csBarType;
	m_cobxCSJoint.GetWindowText(csBarType);

	if (csBarType.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_CS_RigidBar))==0) return STRIP_BAR_TYPE_RIGD;
	if (csBarType.CompareNoCase(_LS(IDS_CMD_EBEAM_CALC_CS_SoftBar ))==0) return STRIP_BAR_TYPE_SOFT;

	ASSERT(0);
	return STRIP_BAR_TYPE_RIGD;
}

BOOL CCMEBeamCalcDlg::IsZero(double dValue)
{
	return (fabs(dValue) < 1.0e-7) ? TRUE : FALSE;
}

BOOL CCMEBeamCalcDlg::CheckInputData(const RCSFS_EFF_WIDTH &RcsFS)
{
	switch (RcsFS.nMethod)
	{
	case STRIP_EFF_METHOD_GROSSMAN: return CheckInputDataGrossman(RcsFS);
	case STRIP_EFF_METHOD_CHOISONG: return CheckInputDataChoiSong(RcsFS);
	}
	ASSERT(0);
	return FALSE;
}

BOOL CCMEBeamCalcDlg::CheckDataDesignStrip(UINT nDgnStr)
{
	BOOL bDgnStr = FALSE;
	switch (nDgnStr)
	{
	case STRIP_TYPE_INT: bDgnStr = TRUE; break;
	case STRIP_TYPE_EDG: bDgnStr = TRUE; break;
	}

	return bDgnStr;
}

BOOL CCMEBeamCalcDlg::CheckInputDataGrossman(const RCSFS_EFF_WIDTH &RcsFS)
{
	double dZero = 1.0e-7;
	RCSFS_EFF_WIDTH_INP RcsFsInp = RcsFS.Inp;

	if (RcsFsInp.dL1 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_L1_GREATER_0)); return FALSE; }
	if (RcsFsInp.dL2 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_L2_GREATER_0)); return FALSE; }
	if (RcsFsInp.dC1 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_C1_GREATER_0)); return FALSE; }
	if (RcsFsInp.dC2 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_C2_GREATER_0)); return FALSE; }
	if (RcsFsInp.dts < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_TS_GREATER_0)); return FALSE; }
	if (RcsFsInp.dCc < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_CC_GREATER_0)); return FALSE; }

	// MQC 4895-19 : Column size가 Span 보다 큰 경우 에러처리 
	BOOL bConsiderL3 = FALSE; // MQC 10133 : L3 값은 체크 대상에서 뺀다 JWKWON-2015-04-06
//   if(RcsFsInp.nDgnStr==STRIP_TYPE_INT) bConsiderL3 = FALSE;
	double dMinSpan = bConsiderL3 ? min(min(RcsFsInp.dL1,RcsFsInp.dL2),RcsFsInp.dL3) : min(RcsFsInp.dL1,RcsFsInp.dL2);
	if (dMinSpan <= max(RcsFsInp.dC1,RcsFsInp.dC2)) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_COL_LARGER_SPAN)); return FALSE; }

	if (!CheckDataDesignStrip(RcsFsInp.nDgnStr)) return FALSE;

	BOOL bDriftC = FALSE;
	switch (RcsFsInp.nDriftC)
	{
	case DRIFT_0100: bDriftC = TRUE; break;
	case DRIFT_0200: bDriftC = TRUE; break;
	case DRIFT_0400: bDriftC = TRUE; break;
	case DRIFT_0800: bDriftC = TRUE; break;
	}
	if (!bDriftC) return FALSE;

	BOOL bColPos = FALSE;
	switch (RcsFsInp.nColPos)
	{
	case COLPOS_INT: bColPos = TRUE; break;
	case COLPOS_EXT: bColPos = TRUE; break;
	case COLPOS_EDG: bColPos = TRUE; break;
	case COLPOS_COR: bColPos = TRUE; break;
	}
	if (!bColPos) return FALSE;

	return TRUE;
}

BOOL CCMEBeamCalcDlg::CheckInputDataChoiSong(const RCSFS_EFF_WIDTH &RcsFS)
{
	double dZero = 1.0e-7;
	RCSFS_EFF_WIDTH_INP RcsFsInp = RcsFS.Inp;

	if (RcsFsInp.dL1 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_L1_GREATER_0)); return FALSE; }
	if (RcsFsInp.dL2 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_L2_GREATER_0)); return FALSE; }
	if (RcsFsInp.dC1 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_C1_GREATER_0)); return FALSE; }
	if (RcsFsInp.dC2 < dZero) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_C2_GREATER_0)); return FALSE; }
	
	// MQC 4895-19 : Column size가 Span 보다 큰 경우 에러처리 
	BOOL bConsiderL3 = FALSE; // MQC 10133 : L3 값은 체크 대상에서 뺀다 JWKWON-2015-04-06
	//if(RcsFsInp.nDgnStr==STRIP_TYPE_INT) bConsiderL3 = FALSE;
	double dMinSpan = bConsiderL3 ? min(min(RcsFsInp.dL1,RcsFsInp.dL2),RcsFsInp.dL3) : min(RcsFsInp.dL1,RcsFsInp.dL2);
	if (dMinSpan <= max(RcsFsInp.dC1,RcsFsInp.dC2)) { GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_COL_LARGER_SPAN)); return FALSE; }
	
	if (!CheckDataDesignStrip(RcsFsInp.nDgnStr)) return FALSE;

	BOOL bBarType = FALSE;
	switch (RcsFsInp.nBarTyp)
	{
	case STRIP_BAR_TYPE_RIGD: bBarType = TRUE; break;
	case STRIP_BAR_TYPE_SOFT: bBarType = TRUE; break;
	}
	if (!bBarType) return FALSE;

	return TRUE;
}

void CCMEBeamCalcDlg::GetDefaultData(RCSFS_EFF_WIDTH &rRcsFS)
{
	rRcsFS.nMethod = STRIP_EFF_METHOD_GROSSMAN;

	RCSFS_EFF_WIDTH_INP &RcsFsInp = rRcsFS.Inp;
	RcsFsInp.dC1 = M_InitValueCurUnit( 500.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dC2 = M_InitValueCurUnit( 500.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dL1 = M_InitValueCurUnit(1000.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dL2 = M_InitValueCurUnit(1000.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dL3 = M_InitValueCurUnit(   0.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dts = M_InitValueCurUnit( 200.0, N, MM, D_UNITSYS_BASE_LENGTH);
	RcsFsInp.dCc = M_InitValueCurUnit(  20.0, N, MM, D_UNITSYS_BASE_LENGTH);

	RcsFsInp.nDriftC = DRIFT_0400;
	RcsFsInp.nDgnStr = STRIP_TYPE_INT;
	RcsFsInp.nColPos = COLPOS_INT;
	RcsFsInp.nBarTyp = STRIP_BAR_TYPE_RIGD;
}

void CCMEBeamCalcDlg::Data2Dlg()
{
	m_edtC1.SetEditUnit(m_RcsFS_Init.Inp.dC1);
	m_edtC2.SetEditUnit(m_RcsFS_Init.Inp.dC2);
	m_edtL1.SetEditUnit(m_RcsFS_Init.Inp.dL1);
	m_edtL2.SetEditUnit(m_RcsFS_Init.Inp.dL2);
	m_edtL3.SetEditUnit(m_RcsFS_Init.Inp.dL3);
	m_edtGrD.SetEditUnit(m_RcsFS_Init.Inp.dCc);
	m_edtGrH.SetEditUnit(m_RcsFS_Init.Inp.dts);
	m_edtCalcResult.SetEditUnit(m_dCaculatedWidth);

	m_cobxMethod   .SetCurSel(m_RcsFS_Init.nMethod);
	m_cobxGrLocEB  .SetCurSel(m_RcsFS_Init.Inp.nDgnStr);
	m_cobxGrSupport.SetCurSel(m_RcsFS_Init.Inp.nColPos);
	m_cobxGrLateral.SetCurSel(m_RcsFS_Init.Inp.nDriftC);
	m_cobxCSLocEB  .SetCurSel(m_RcsFS_Init.Inp.nDgnStr);
	m_cobxCSJoint  .SetCurSel(m_RcsFS_Init.Inp.nBarTyp);
}

void CCMEBeamCalcDlg::OnApply()
{
	m_RcsFS_Init = m_RcsFS;
	m_dCaculatedWidth = m_edtCalcResult.GetEditValue();
	CDialogMove::OnOK();
}

void CCMEBeamCalcDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMEBeamCalcDlg::OnCalculate()
{
	m_edtCalcResult.SetEditUnit(GetValue(RES_TYPE_EFF));
}

// 비활성화 조건 : MQC 4895-17
void CCMEBeamCalcDlg::EnableDisableCtrls()
{
	int nMethod = m_cobxMethod.GetItemData(m_cobxMethod.GetCurSel());
	
	BOOL bL2, bL3;
	if(nMethod==STRIP_EFF_METHOD_GROSSMAN)
	{
		int nLocEB = m_cobxGrLocEB.GetItemData(m_cobxGrLocEB.GetCurSel());
		
		bL2 = TRUE;
		bL3 = nLocEB==STRIP_TYPE_EDG ? TRUE:FALSE;
	}
	else // nMethod==STRIP_EFF_METHOD_CHOISONG
	{
		int nLocEB = m_cobxCSLocEB.GetItemData(m_cobxCSLocEB.GetCurSel());
		if(nLocEB==STRIP_TYPE_INT)
		{
			bL2 = bL3 = FALSE;
		}
		else
		{
			int nStripBarType = m_cobxCSJoint.GetItemData(m_cobxCSJoint.GetCurSel());
			bL2 = nStripBarType==STRIP_BAR_TYPE_RIGD ? TRUE:FALSE;
			bL3 = TRUE;
		}
	}

	m_edtL2.EnableWindow(bL2);
	m_edtL3.EnableWindow(bL3);
}

void CCMEBeamCalcDlg::OnChangeGrLocEBCmb() { EnableDisableCtrls(); }
void CCMEBeamCalcDlg::OnChangeCsLocEBCmb() { EnableDisableCtrls(); }
void CCMEBeamCalcDlg::OnChangeCsJointCmb() { EnableDisableCtrls(); }