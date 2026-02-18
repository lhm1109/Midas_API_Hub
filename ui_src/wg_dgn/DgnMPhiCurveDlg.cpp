// DgnMPhiCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMPhiCurveDlg.h"
#include "DgnMPhiCurveGrid.h"
#include "DgnMPhiCurveConditionDlg.h"
#include "DgnSeisMPhiIdealGuideDlg.h"
#include "DgnMsgDgn.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\dgnengine\idesign\DGN_link\ILanguageLib.h"
#include "..\dgnengine\src\ILanguage\DgnMSGInfoStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveDlg dialog


CDgnMPhiCurveDlg::CDgnMPhiCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMPhiCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMPhiCurveDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	// 국내내진설계 위한 변수 CDgnSeisMPhiItemDlg
	m_Data.bEvgp = FALSE;
	m_Data.EvgpK = 0;    
	m_Data.MatlConcK2 = 0;
	m_Data.MatlConcK2Trans = 0;

	m_ResultD.Initialize();

	m_bIdealized = FALSE;
	m_bUserCurve = FALSE;
	m_nIdealOpt = 1;

	m_bConcTypeMander = FALSE;

	m_pGrid = NULL;
	m_pGrid = new CDgnMPhiCurveGrid(this);
	ASSERT(m_pGrid);
	
	m_InitMpccK = 0;
}

CDgnMPhiCurveDlg::~CDgnMPhiCurveDlg()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

void CDgnMPhiCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMPhiCurveDlg)
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_NAME_EDT,                         m_edtName);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_SECT_CMB,                         m_cmbSection);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_POSITION_CMB,                     m_cmbPosition);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CONC_CMB1,                        m_cmbConcrete1);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CONC_CMB2,                        m_cmbConcrete2);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_STEEL_CMB,                        m_cmbSteel);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_AXIAL_FORCE_EDT,                  m_edtAxialForce);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_AXIAL_FORCE_UNT,                  m_untAxialForce);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_AXIS_ANGLE_EDT,                   m_edtAxisAngle);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_AXIS_ANGLE_UNT,                   m_untAxisAngle);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_NUM_POINT_EDT,                    m_edtNumPoint);
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_IDEALIZED_CHK,                    m_bIdealized);
	DDX_Check  (pDX, IDC_DGN_MPHI_CURVE_USER_CURVE_CHK,                   m_bUserCurve);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_USER_CURVE_EDT,                   m_edtUserCurve);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_USER_CURVE_UNT,                   m_untUserCurve);
	DDX_Radio  (pDX, IDC_DGN_MPHI_IDEAL_OPT1_RDO,                         m_nIdealOpt);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_TYPE_CURVE_CMB,                   m_cmbTypeCurve);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_MOMENT_UNT,                       m_untMoment);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CURVATURE_UNT,                    m_untCurvature);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CRACK_MOMENT_PLUS_EDT,            m_edtCrackMomentPlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CRACK_MOMENT_MINUS_EDT,           m_edtCrackMomentMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CRACK_CURVATURE_PLUS_EDT,         m_edtCrackCurvaturePlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CRACK_CURVATURE_MINUS_EDT,        m_EdtCrackCurvatureMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_INIT_MOMENT_PLUS_EDT,       m_edtYieldInitMomentPlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_INIT_MOMENT_MINUS_EDT,      m_edtYieldInitMomentMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_INIT_CURVATURE_PLUS_EDT,    m_edtYieldInitCurvaturePlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_INIT_CURVATURE_MINUS_EDT,   m_edtYieldInitCurvatureMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_MOMENT_PLUS_EDT,            m_edtYieldMomentPlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_MOMENT_MINUS_EDT,           m_edtYieldMomentMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_CURVATURE_PLUS_EDT,         m_edtYieldCurvaturePlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_CURVATURE_MINUS_EDT,        m_edtYieldCurvatureMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_PLUS_EDT,      m_edtYieldIdealMomentPlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_MINUS_EDT,     m_edtYieldIdealMomentMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_PLUS_EDT,   m_edtYieldIdealCurvaturePlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_MINUS_EDT,  m_edtYieldIdealCurvatureMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_ULTIMATE_MOMENT_PLUS_EDT,         m_edtUltimateMomentPlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_ULTIMATE_MOMENT_MINUS_EDT,        m_edtUltimateMomentMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_ULTIMATE_CURVATURE_PLUS_EDT,      m_edtUltimateCurvaturePlus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_ULTIMATE_CURVATURE_MINUS_EDT,     m_edtUltimateCurvatureMinus);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_SECT_VIEW						,	m_SectWnd);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_CURVE_VIEW, m_ChartWnd);
	DDX_Control(pDX, IDC_DGN_MPHI_CURVE_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnMPhiCurveDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnMPhiCurveDlg)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_SEL_ALL_BTN,          OnDgnMPhiCurveSelAllBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_UNSEL_ALL_BTN,        OnDgnMPhiCurveUnSelAllBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_DEL_BTN,              OnDgnMPhiCurveDelBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CALC_SEL_BTN,         OnDgnMPhiCurveCalcSelBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_OUTPUT_BTN,           OnDgnMPhiCurveOutputBtn)
	ON_CBN_SELCHANGE(IDC_DGN_MPHI_CURVE_SECT_CMB,             OnDgnMPhiCurveSectionCmb)
	ON_CBN_SELCHANGE(IDC_DGN_MPHI_CURVE_POSITION_CMB,         OnDgnMPhiCurvePositionCmb)
	ON_CBN_SELCHANGE(IDC_DGN_MPHI_CURVE_CONC_CMB1,            OnDgnMPhiCurveConcrete1Cmb)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_IDEALIZED_CHK,        OnDgnMPhiCurveIdealizedChk)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_USER_CURVE_CHK,       OnDgnMPhiCurveUserCurveChk)
	ON_BN_CLICKED   (IDC_DGN_MPHI_IDEAL_OPT_GUIDE_BTN,        OnDgnMPhiCurveOptGuideBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_ADD_BTN,              OnDgnMPhiCurveAddBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_MODIFY_BTN,           OnDgnMPhiCurveModifyBtn)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_CALC_BTN,             OnDgnMPhiCurveCalcBtn)
	ON_CBN_SELCHANGE(IDC_DGN_MPHI_CURVE_TYPE_CURVE_CMB,       OnDgnMPhiCurveTypeCurveCmb)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_ECUC_BTN,             OnDgnMPhiCurveECUCBtn)
	ON_EN_CHANGE    (IDC_DGN_MPHI_CURVE_AXIS_ANGLE_EDT,       OnChangeAxisAngle)
	ON_BN_CLICKED   (IDC_DGN_MPHI_CURVE_SECT_BTN,             OnDgnMPhiCurveSectionBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnMPhiCurveDlg::InitCombo() 
{
	// Reset Content
	m_cmbSection.ResetContent();
	m_cmbConcrete1.ResetContent();
	m_cmbConcrete2.ResetContent();
	m_cmbSteel.ResetContent();
	m_cmbPosition.ResetContent();
	m_cmbTypeCurve.ResetContent();

	int nSectionIndex = 0;
	int nConcrete1Index = 0;
	int nConcrete2Index = 0;
	int nSteelIndex = 0;

	int nSectionTmp = 0; 
	int nConcrete1Tmp = -1;
	int nConcrete2Tmp = -1;
	int nSteelTmp = -1;

	// Section Combo Box
	T_SECT_D SectData;
	CArray<T_SECT_K, T_SECT_K> aSectKey;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectKey);

	for(int i=0; i<aSectKey.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSect(aSectKey[i], SectData))
		{
			GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
			return;
		}
		m_cmbSection.SetItemData(m_cmbSection.AddString(SectData.SName), aSectKey[i]);

		if(aSectKey[i] == m_Data.SectK) nSectionIndex = i;
	}

	m_cmbSection.SetCurSel(nSectionIndex);

//   T_SECT_D SectData;
// 
//   CArray<T_RCHK_K, T_RCHK_K> aRchkKey;
//   m_pDoc->m_pAttrCtrl->GetRchkKeyList(aRchkKey);
// 
//   // Rebar input for Column Section 에서 Concrete 만 받기 때문에 Section 에 대한 검사만 한다.
//   for(int i=0; i<aRchkKey.GetSize(); i++)
//   {
//     if(!m_pDoc->m_pAttrCtrl->GetSect(aRchkKey[i], SectData))
//     {
//       GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
//       return;
//     }
//     m_cmbSection.SetItemData(m_cmbSection.AddString(SectData.SName), aRchkKey[i]);
// 
//     if(aRchkKey[i] == m_Data.SectK) nSectionIndex = i;
// 		nSectionTmp++;
//   }
// 
// // 국내의 경우에는 General Section 도 허용한다.
// #if defined(_ORG)
//   CArray<T_REBT_K, T_REBT_K> aRebtKey;
//   m_pDoc->m_pAttrCtrl->GetRebtKeyList(aRebtKey);
//   
//   // Rebar Input for General Section 에서 Concrete 만 받기 때문에 Section 에 대한 검사만 한다.
//   for(i=0; i<aRebtKey.GetSize(); i++)
//   {
//     if(!m_pDoc->m_pAttrCtrl->GetSect(aRebtKey[i], SectData))
//     {
//       GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
//       return;
//     }
//     m_cmbSection.SetItemData(m_cmbSection.AddString(SectData.SName), aRebtKey[i]);
// 
//     if(aRebtKey[i] == m_Data.SectK) nSectionIndex = i + nSectionTmp;
//   }
// #endif
//   
//   m_cmbSection.SetCurSel(nSectionIndex);

	// Concrete Combo Box 1, Concrete Combo Box 2, Steel Combo Box 
	T_FIMP_D FimpData;

	CArray<T_FIMP_K, T_FIMP_K> aFimpKey;
	m_pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpKey);

	for(int i=0; i<aFimpKey.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetFimp(aFimpKey[i], FimpData))
		{
			GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_INELASTIC_MAT_PRO_INFO));
			return;
		}

		if(FimpData.nMaterialType == 0) // Concrete
		{
			if(FimpData.nHysModelType == 6 && FimpData.PROP.CON_MANDER.nConcType == 1) // Mander && Mander - confined
			{
				m_cmbConcrete2.SetItemData(m_cmbConcrete2.AddString(FimpData.strName), aFimpKey[i]);
				nConcrete2Tmp++;
				if(aFimpKey[i] == m_Data.MatlConcK2) nConcrete2Index = nConcrete2Tmp;
			}
			else
			{
				m_cmbConcrete1.SetItemData(m_cmbConcrete1.AddString(FimpData.strName), aFimpKey[i]);
				nConcrete1Tmp++;
				if(aFimpKey[i] == m_Data.MatlConcK1) nConcrete1Index = nConcrete1Tmp;
			}
		}
		else // Steel
		{
			m_cmbSteel.SetItemData(m_cmbSteel.AddString(FimpData.strName), aFimpKey[i]);
			nSteelTmp++;
			if(aFimpKey[i] == m_Data.MatlSteelK) nSteelIndex = nSteelTmp;
		}
	}

	m_cmbConcrete1.SetCurSel(nConcrete1Index);
	m_cmbConcrete2.SetCurSel(nConcrete2Index);
	m_cmbSteel.SetCurSel(nSteelIndex);

	// Position
	m_cmbPosition.AddString(_LS(IDS_DGN_MPHI_CURVE_I));
	m_cmbPosition.AddString(_LS(IDS_DGN_MPHI_CURVE_M));
	m_cmbPosition.AddString(_LS(IDS_DGN_MPHI_CURVE_J));

	m_cmbPosition.SetCurSel(0);

	// Type of Curve Combo Box
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_MOMENT_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_NEUTRAL_AXIS_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_CONC_STRAIN_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_STEEL_STRAIN_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_CONC_COMP_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_STEEL_COMP_CURVATURE));
	m_cmbTypeCurve.AddString(_LS(IDS_DGN_MPHI_CURVE_STEEL_TENS_CURVATURE));

	m_cmbTypeCurve.SetCurSel(0);


	T_FIMP_K FimpK;
	T_FIMP_D FimpD;
	FimpK = m_cmbConcrete1.GetItemData(m_cmbConcrete1.GetCurSel());
	if(m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD))
	{
		if(FimpD.nHysModelType == 6) // Mander Model
		{
			m_bConcTypeMander = TRUE;
		}
		else
		{
			m_bConcTypeMander = FALSE;
		}
	}
}

void CDgnMPhiCurveDlg::InitUnit() 
{
	m_edtAxialForce              .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untAxialForce              .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtAxisAngle               .SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untAxisAngle               .SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtUserCurve               .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_untUserCurve               .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_untMoment                  .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_untCurvature               .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtCrackMomentPlus         .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtCrackMomentMinus        .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtCrackCurvaturePlus      .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_EdtCrackCurvatureMinus     .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldInitMomentPlus     .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldInitMomentMinus    .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldInitCurvaturePlus  .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldInitCurvatureMinus .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldMomentPlus         .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldMomentMinus        .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldCurvaturePlus      .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldCurvatureMinus     .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldIdealMomentPlus    .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldIdealMomentMinus   .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtYieldIdealCurvaturePlus .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtYieldIdealCurvatureMinus.SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtUltimateMomentPlus      .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtUltimateMomentMinus     .SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtUltimateCurvaturePlus   .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_edtUltimateCurvatureMinus  .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
}

void CDgnMPhiCurveDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aShowHideControl;
	aShowHideControl.Add(IDC_DGN_MPHI_CURVE_CONC_CMB2);

	// Concrete Type 이 Mander Model 이면 Combo Box 를 보여준다.
	if(m_bConcTypeMander)
	{
		CDlgUtil::CtrlShowHide(this, aShowHideControl, TRUE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, aShowHideControl, FALSE);
	}
}

void CDgnMPhiCurveDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);

	if(m_bIdealized)
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_STC)->EnableWindow(TRUE);

		if(m_bUserCurve)
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_EDT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_UNT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_EDT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_UNT)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_PLUS_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_MINUS_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_PLUS_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_MINUS_EDT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_USER_CURVE_UNT)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_PLUS_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_MOMENT_MINUS_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_PLUS_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MPHI_CURVE_YIELD_IDEAL_CURVATURE_MINUS_EDT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_DGN_MPHI_IDEAL_OPT_GRP)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_MPHI_IDEAL_OPT1_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_MPHI_IDEAL_OPT2_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_MPHI_IDEAL_OPT3_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_MPHI_IDEAL_OPT_GUIDE_BTN)->EnableWindow(m_bIdealized);
}

void CDgnMPhiCurveDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);

	// Section, Concrete1, Concrete2, Steel 에 관한 처리는 InitComb 에서 한다.

	m_cmbPosition.SetCurSel(m_Data.nPosition);

	m_edtAxialForce.SetEditUnit(m_Data.dAxialForce);
	m_edtAxisAngle.SetEditUnit(m_Data.dAxisAngle);
	m_edtNumPoint.SetEditUnit(m_Data.nNumPoint);

	m_bIdealized = m_Data.bIdealize;
	m_bUserCurve = m_Data.bCurvature;

	m_edtUserCurve.SetEditUnit(m_Data.dCurvature);

	m_nIdealOpt = m_Data.nIdealOpt;

	UpdateData(FALSE);
}

BOOL CDgnMPhiCurveDlg::Dlg2Data()
{ 
	UpdateData(TRUE);

	m_edtName.GetWindowText(m_Data.strName);

	// Section Key
	int nSectIndex = m_cmbSection.GetCurSel();
	if(nSectIndex == -1) { m_Data.SectK = 0; }
	else { m_Data.SectK = m_cmbSection.GetItemData(nSectIndex); }

	// Position
	m_Data.nPosition = m_cmbPosition.GetCurSel();

	// Concrete Key
	int nConcIndex1 = m_cmbConcrete1.GetCurSel();
	if(nConcIndex1 == -1) { m_Data.MatlConcK1 = 0; }
	else { m_Data.MatlConcK1 = m_cmbConcrete1.GetItemData(nConcIndex1); }

	// Concrete Key - Mander Model
	int nConcIndex2 = m_cmbConcrete2.GetCurSel();
	if(nConcIndex2 == -1) { m_Data.MatlConcK2 = 0; }
	else { m_Data.MatlConcK2 = m_cmbConcrete2.GetItemData(nConcIndex2); }

	// Steel Key
	int nSteelIndex = m_cmbSteel.GetCurSel();
	if(nSteelIndex == -1) { m_Data.MatlSteelK = 0; }
	else { m_Data.MatlSteelK = m_cmbSteel.GetItemData(nSteelIndex); }

	m_Data.dAxialForce = m_edtAxialForce.GetEditValue();
	m_Data.dAxisAngle = m_edtAxisAngle.GetEditValue();
	m_Data.nNumPoint = m_edtNumPoint.GetEditValue();

	m_Data.bIdealize = m_bIdealized;
	m_Data.bCurvature = m_bUserCurve;

	m_Data.dCurvature = m_edtUserCurve.GetEditValue();

	m_Data.nIdealOpt = m_nIdealOpt;

	return TRUE;
}

BOOL CDgnMPhiCurveDlg::CheckDialogData()
{
	// Name
	if(m_Data.strName.IsEmpty()) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_NAME_ERROR)); return FALSE; }

	// Section
	if(m_Data.SectK == 0) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_SECTION_ERROR)); return FALSE; }

	// Position
	if(m_Data.nPosition < 0 || m_Data.nPosition > 2) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_POSITION_ERROR)); return FALSE; }

	// Concrete 1
	if(m_Data.MatlConcK1 == 0) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_CONCRETE_ERROR)); return FALSE; }
	else
	{
		T_FIMP_D FimpData;
		if(!m_pDoc->m_pAttrCtrl->GetFimp(m_Data.MatlConcK1, FimpData))
		{
			AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_INELASTIC_MAT_PRO_INFO));
			return FALSE;
		}

		if(FimpData.nMaterialType == 0) // Concrete
		{
			if(FimpData.nHysModelType == 6 && FimpData.PROP.CON_MANDER.nConcType == 1) // Mander && Mander - confined
			{
				// Concrete 2
				if(m_Data.MatlConcK2 == 0) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_CONCRETE_ERROR)); return FALSE; }
			}
		}
	}

	// Steel
	if(m_Data.MatlSteelK == 0) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_STEEL_ERROR)); return FALSE; }

	// Axial Force
	// Nothing

	// National Axis Angle
	if(m_Data.dAxisAngle < -360 || m_Data.dAxisAngle > 360) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_NATIONAL_AXIS_ANGLE_ERROR)); return FALSE; }

	// No. of Points
	if(m_Data.nNumPoint < 10 || m_Data.nNumPoint > 10000) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_NO_OF_POINT_ERROR)); return FALSE; }

	if(m_Data.bIdealize || m_Data.bCurvature) 
	{
		// User Defined Curvature (for Ideal model)
		if(m_Data.dCurvature < 0) { AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_USER_DEFINE_CURVATURE_ERROR)); return FALSE; }
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL bSection = FALSE;
	BOOL bGen     = FALSE;
	BOOL bRebar   = FALSE;
	T_SECT_D SectD;  SectD.Initialize();
	T_RCHK_D RchkD;  RchkD.Initialize();
	T_REBT_D RebtD;  RebtD.Initialize();	
	if(m_pDoc->m_pAttrCtrl->GetSectDesign(m_Data.SectK, SectD))
	{
		if(SectD.nStype == D_SECT_TYPE_REGULAR || SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B   || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P    || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR   || 
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ||
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
			{ bSection = TRUE; bGen = FALSE; }
			else if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
			{ bSection = TRUE; bGen = TRUE; }
		}
		else if(SectD.nStype == D_SECT_TYPE_USER && SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
		{ bSection = TRUE; bGen = TRUE; }
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
		return FALSE;
	}

	if(bSection)
	{
		if(!bGen)
		{ bRebar = m_pDoc->m_pAttrCtrl->GetRchk(m_Data.SectK, RchkD); }
		else
		{ bRebar = m_pDoc->m_pAttrCtrl->GetRebt(m_Data.SectK, RebtD); }
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_SECTION_ERROR2));
		return FALSE;  
	}

	if(!bRebar)
	{
		AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_REBAR_ERROR));
		return FALSE;  
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

void CDgnMPhiCurveDlg::ShowResultData()
{
	m_edtCrackMomentPlus.SetEditUnit(m_ResultD.MPhi_c[0].dMoment, 4);
	m_edtCrackMomentMinus.SetEditUnit(m_ResultD.MPhi_c[1].dMoment, 4);
	m_edtCrackCurvaturePlus.SetEditUnit(m_ResultD.MPhi_c[0].dCurvature);
	m_EdtCrackCurvatureMinus.SetEditUnit(m_ResultD.MPhi_c[1].dCurvature);

	m_edtYieldInitMomentPlus.SetEditUnit(m_ResultD.MPhi_y0[0].dMoment, 4);
	m_edtYieldInitMomentMinus.SetEditUnit(m_ResultD.MPhi_y0[1].dMoment, 4);
	m_edtYieldInitCurvaturePlus.SetEditUnit(m_ResultD.MPhi_y0[0].dCurvature);
	m_edtYieldInitCurvatureMinus.SetEditUnit(m_ResultD.MPhi_y0[1].dCurvature);

	m_edtYieldMomentPlus.SetEditUnit(m_ResultD.MPhi_y[0].dMoment, 4);
	m_edtYieldMomentMinus.SetEditUnit(m_ResultD.MPhi_y[1].dMoment, 4);
	m_edtYieldCurvaturePlus.SetEditUnit(m_ResultD.MPhi_y[0].dCurvature);
	m_edtYieldCurvatureMinus.SetEditUnit(m_ResultD.MPhi_y[1].dCurvature);

	m_edtYieldIdealMomentPlus.SetEditUnit(m_ResultD.MPhi_yI[0].dMoment, 4);
	m_edtYieldIdealMomentMinus.SetEditUnit(m_ResultD.MPhi_yI[1].dMoment, 4);
	m_edtYieldIdealCurvaturePlus.SetEditUnit(m_ResultD.MPhi_yI[0].dCurvature);
	m_edtYieldIdealCurvatureMinus.SetEditUnit(m_ResultD.MPhi_yI[1].dCurvature);

	m_edtUltimateMomentPlus.SetEditUnit(m_ResultD.MPhi_u[0].dMoment, 4);
	m_edtUltimateMomentMinus.SetEditUnit(m_ResultD.MPhi_u[1].dMoment, 4);
	m_edtUltimateCurvaturePlus.SetEditUnit(m_ResultD.MPhi_u[0].dCurvature);
	m_edtUltimateCurvatureMinus.SetEditUnit(m_ResultD.MPhi_u[1].dCurvature);
}

void CDgnMPhiCurveDlg::GetGridData(T_MPCC_K MpccKey)
{
	T_MPCC_D MpccData;

	if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccKey, MpccData)) { ASSERT(0); return; }

	m_Data = MpccData;

	InitCombo();
	Data2Dlg();

	UpdateData(TRUE);

	T_FIMP_K FimpK;
	T_FIMP_D FimpD;
	FimpK = MpccData.MatlConcK1;

	if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD))
	{
		GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
		m_bConcTypeMander = FALSE;
		return;
	}
	else
	{
		if(FimpD.nHysModelType == 6) // Mander Model
		{
			m_bConcTypeMander = TRUE;
		}
		else
		{
			m_bConcTypeMander = FALSE;
		}
	}

	ControlsEnableDisable();
	ControlsShowHide();

	// 받은 Data 를 Data2Dlg 에 뿌려준다..  
	DGN_CALC_MPHI_RESULT ResultD;

	// 해당 Key의 Moment-Curvature 결과를 넘겨줍니다.
	//   Key     : 결과를 넘겨받을 정보의 Key
	// < ResultD : 결과가 있을때 M-Φ결과를 넘겨 받을 변수

	// 결과가 없으면 해당 Dlg 에 모든 칸 비우기...
	if(!m_DgnMPhiDesign.Get_MPhiResult(MpccKey, ResultD)) ResultD.Initialize();

	m_ResultD = ResultD;

	ShowResultData();	
	DrawSection();
	DrawGraph();
}

void CDgnMPhiCurveDlg::DrawSection()
{	
	m_SectWnd.Set_SectPointer(&m_Data);
	m_SectWnd.Invalidate();
}

void CDgnMPhiCurveDlg::DrawGraph()
{
	int nCurveType = m_cmbTypeCurve.GetCurSel();
	m_ChartWnd.Set_ResultPointer(&m_ResultD);
	m_ChartWnd.Set_CurveType(nCurveType);
	m_ChartWnd.Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveDlg message handlers

BOOL CDgnMPhiCurveDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// 기존에 저장되어 있던 Data 를 Grid 에 뿌려준다.
	CArray<T_MPCC_K, T_MPCC_K> aMpccKeyList;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccKeyList);

// 	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_MPHI_CURVE_SECT_VIEW);
//   ASSERT(pPlaceHolder);
// 	m_SectWnd.Initial(pPlaceHolder);	
// 	pPlaceHolder = GetDlgItem(IDC_DGN_MPHI_CURVE_CURVE_VIEW);
//   ASSERT(pPlaceHolder);
// 	m_ChartWnd.Initial(pPlaceHolder);	
	
	m_pGrid->Initialize();

	InitCombo();
	InitUnit();

	Data2Dlg();
	Dlg2Data();
	

	UpdateData(FALSE);

	ControlsEnableDisable();
	ControlsShowHide();

	int nSize = aMpccKeyList.GetSize();
	if(nSize > 0)
	{
		BOOL bFindInitMpccK = FALSE;
		for(int i=0 ; i<nSize ; i++)
		{
			if(aMpccKeyList[i] == m_InitMpccK)
			{ bFindInitMpccK = TRUE;  break; }
		}
		if(bFindInitMpccK)
		{
			m_pGrid->SetSelectedMpccKey(m_InitMpccK);
			GetGridData(m_InitMpccK);
		}
		else             
		{ GetGridData(aMpccKeyList[0]); }
	}
	
	dgn::link::ILanguageLib::Instance()->LoadMessage(DGN_LANGUAGE_ID_US, PROJECT_TYPE_CVL);

	DrawSection();
	DrawGraph();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveSelAllBtn() 
{
	// Select All 
	BOOL bSelct = TRUE;
	m_pGrid->SelectAllorUnSelectAll(bSelct);
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveUnSelAllBtn() 
{
	// UnSelect All
	BOOL bSelct = FALSE;
	m_pGrid->SelectAllorUnSelectAll(bSelct);
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveDelBtn() 
{
	// Delete
	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	if(!m_pGrid->GetSelectedMpccKeyList(aMpccKey)) return;

	if(!m_pDoc->m_pDataCtrl->DelMpcc(aMpccKey)) { ASSERT(0); return; }

	// Grid 에 표현...
	m_pGrid->MakeItemEx();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveCalcSelBtn() 
{
	// Calculation for Selected  

	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	if(!m_pGrid->GetSelectedMpccKeyList(aMpccKey)) return;
	if(!m_pGrid->SetSelectedKey(aMpccKey)) return;

	// 해당 KeyList Moment-Curvature를 계산합니다.
	//   KeyList : 계산할 KeyList
	m_DgnMPhiDesign.CalcMomentCurvature(aMpccKey);

	// M-Φ결과정보를 가지고 있는 KeyList를 넘겨줌
	CArray<T_MPCC_K, T_MPCC_K> aResultMpccKey;
	m_DgnMPhiDesign.Get_MPhiResultKeyList(aResultMpccKey);

	m_pGrid->SetResultIndex(aResultMpccKey);

	// Grid 에 표현...
	m_pGrid->MakeItemEx();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveOutputBtn() 
{
	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	if(!m_pGrid->GetSelectedMpccKeyList(aMpccKey)) return;

	m_DgnMPhiDesign.Print_MPhiResult(aMpccKey);
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveSectionCmb() 
{
	// Section 이 변경 되면 Sect View 를 Update 해준다.
	Dlg2Data();
	DrawSection();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurvePositionCmb() 
{
	// Position 이 변경 되면 Sect View 를 Update 해준다.
	Dlg2Data();
	DrawSection();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveConcrete1Cmb() 
{
	UpdateData(TRUE);

	T_FIMP_K FimpK;
	T_FIMP_D FimpD;
	FimpK = m_cmbConcrete1.GetItemData(m_cmbConcrete1.GetCurSel());

	if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD))
	{
		GSaveHistoryFormatNF(_LS(IDS_DGN_MPHI_CURVE_DONT_GET_SECTION_INFO));
		m_bConcTypeMander = FALSE;
		return;
	}
	else
	{
		if(FimpD.nHysModelType == 6) // Mander Model
		{
			m_bConcTypeMander = TRUE;
		}
		else
		{
			m_bConcTypeMander = FALSE;
		}
	}

	ControlsShowHide();

	// Concrete 가 변경 되면 Sect View 를 Update 해준다.
	Dlg2Data();
	DrawSection();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveIdealizedChk() 
{
	ControlsEnableDisable();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveUserCurveChk() 
{
	ControlsEnableDisable();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveOptGuideBtn()
{
	UpdateData(TRUE);

	CDgnSeisMPhiIdealGuideDlg dlg;
	dlg.SetParamData(m_nIdealOpt);
	dlg.DoModal();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveAddBtn() 
{
	// Add Button
	Dlg2Data();

	if(!CheckDialogData()) return;

	T_MPCC_K Key;
	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	CArray<CString, CString> aNameList;
	if(m_pGrid->GetMpccData(aMpccKey, aNameList))
	{
		// Name
		for(int i=0; i<aNameList.GetSize(); i++)
		{
			if(aNameList[i] == m_Data.strName)
			{
				AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_SAME_NAME_ERROR));
				return;
			}
		}

		// Key
		Key = m_pDoc->m_pAttrCtrl2->GetLastNumMpcc() + 1;

		if(m_pDoc->m_pAttrCtrl2->ExistMpcc(Key))
		{
			AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_SAME_DATA_ERROR));
		}
	}
	else
	{
		Key = m_pDoc->m_pAttrCtrl2->GetStartNumMpcc();
	}

	CArray<T_MPCC_K, T_MPCC_K> arKey;
	CArray<T_MPCC_D, T_MPCC_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();

	arKey.Add(Key);
	arData.Add(m_Data);	

	if(!m_pDoc->m_pDataCtrl->AddMpcc(arKey,arData)) { ASSERT(0); return; }

	// Grid 에 표현...
	m_pGrid->MakeItemEx();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveModifyBtn() 
{
	// Modify Button
	Dlg2Data();

	if(!CheckDialogData()) return;

	T_MPCC_K MpccKey;
	T_MPCC_D MpccData;
	if(!m_pGrid->GetSelectedMpccKey(MpccKey)) return;

	// Grid 에 있는 정보를 모두 가져와서 동일한 이름이 있나 파악한다.
	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	CArray<CString, CString> aNameList;
	if(m_pGrid->GetMpccData(aMpccKey, aNameList))
	{
		for(int i=0; i<aMpccKey.GetSize(); i++)
		{
			if(aMpccKey[i] != MpccKey)
			{
				if(aNameList[i] == m_Data.strName)
				{
					AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_SAME_NAME_ERROR));
					return;
				}
			}
		}
	}

	CArray<T_MPCC_K, T_MPCC_K> aResultMpccKey;
	aResultMpccKey.RemoveAll();

	if(m_DgnMPhiDesign.Get_MPhiResult(MpccKey, m_ResultD))
	{
		aResultMpccKey.Add(MpccKey);
	}

	CArray<T_MPCC_K, T_MPCC_K> arKey;
	CArray<T_MPCC_K, T_MPCC_K> arKeyOld;
	CArray<T_MPCC_D, T_MPCC_D&> arData;
	arKeyOld.RemoveAll();
	arKey.RemoveAll();
	arData.RemoveAll();

	arKeyOld.Add(MpccKey);
	arKey.Add(MpccKey);
	arData.Add(m_Data);	

	if(!m_pDoc->m_pDataCtrl->ModifyMpcc(arKeyOld, arKey, arData)) { ASSERT(0); return; }
	
	m_DgnMPhiDesign.Get_MPhiResultKeyList(aResultMpccKey);
	m_pGrid->SetResultIndex(aResultMpccKey);

	CArray<T_MPCC_K, T_MPCC_K> aSelectedMpccKey;
	aSelectedMpccKey.RemoveAll();
	if(!m_pGrid->GetSelectedMpccKeyList(aSelectedMpccKey)) return;
	if(!m_pGrid->SetSelectedKey(aSelectedMpccKey)) return;
	
	// Grid 에 표현...
	m_pGrid->MakeItemEx();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveCalcBtn() 
{
	Dlg2Data();

	if(!CheckDialogData()) return;

	// 입력한 값으로 Moment-Curvature를 계산합니다.(현재 입력창의 값으로 계산을 수행할때)  
	int nIndex = m_DgnMPhiDesign.CalcMomentCurvature(m_Data, m_ResultD);

	if     (nIndex == 1)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_1));   return; }
	else if(nIndex == 2)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_2));   return; }
	else if(nIndex == 3)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_3));   return; }
	else if(nIndex == 4)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_4));   return; }
	else if(nIndex == 5)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_5));   return; }
	else if(nIndex == 6)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_6));   return; }
	else if(nIndex == 7)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_7));   return; }
	else if(nIndex == 8)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_8));   return; }
	else if(nIndex == 9)   { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_9));   return; }
	else if(nIndex == 10)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_10));  return; }
	else if(nIndex == 11)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_11));  return; }
	else if(nIndex == 12)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_12));  return; }
	else if(nIndex == 13)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_13));  return; }
	else if(nIndex == 31)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_31));  return; }
	else if(nIndex == 32)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_32));  return; }
	else if(nIndex == 33)  { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_33));  return; }
	else if(nIndex == 100) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_100)); return; }
	else if(nIndex == 200) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_200)); return; }
	else if(nIndex == 50)  
	{ 
		CString stForceUnit;  m_pDoc->m_pUnitCtrl->GetUnitSystemForce(stForceUnit);
		CString strValue;  strValue.Format(_T("%d %s"), (int)m_ResultD.dLimitAxialForce,  stForceUnit);
		CString strMsg;  strMsg.Format(_LS(IDS_DGN_MPHI_CURVE_OVER_AXIALFORCE), strValue);  AfxMessageBox(strMsg);  return;
	}

	ShowResultData();

	DrawGraph();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveTypeCurveCmb() 
{
	DrawGraph();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveECUCBtn()
{
	CDgnMPhiCurveConditionDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		// Nothing
	}
}
void CDgnMPhiCurveDlg::OnChangeAxisAngle()
{
	m_Data.dAxisAngle = m_edtAxisAngle.GetEditValue();
	DrawSection();
}

void CDgnMPhiCurveDlg::OnDgnMPhiCurveSectionBtn()
{
	// Combo Box 에서 선택된 Section 이 DB 단면이면 Rebar Input Data for Column Section 호출
	// Combo Box 에서 선택된 Section 이 General 단면이면 Rebar Input Data for General Section 호출
	UpdateData(TRUE);

	T_SECT_K SectK;
	T_SECT_D SectD;  SectD.Initialize();

	int nSectIndex = m_cmbSection.GetCurSel();
	if(nSectIndex == -1) { m_Data.SectK = 0; }
	else { SectK = m_cmbSection.GetItemData(nSectIndex); }

	if(m_pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
	{
		if(SectD.nStype == D_SECT_TYPE_REGULAR || SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B   || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P    || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR   || 
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ||
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
			{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_REBC,0));
			}
			else if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
			{ 
				AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SECTION_REBAR_INPUT,0));
			}
		}
		else if(SectD.nStype == D_SECT_TYPE_USER && SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
		{ 
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SECTION_REBAR_INPUT,0)); 
		}
	}
}

