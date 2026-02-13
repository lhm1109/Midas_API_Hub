// DgnStlRatingParamAASHTODlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"

#include "DgnStlRatingParamAASHTODlg.h"

#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnStlRatingParamAASHTODlg::CDgnStlRatingParamAASHTODlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlRatingParamAASHTODlg::IDD, pParent)
{
	m_bStre = TRUE;
	m_nGirderType = 1;
	m_bTorsionType = TRUE;
	m_nBridgeType = 0;
	m_bStreOption1 = FALSE;
	m_bStreOption2 = FALSE;
	m_bStreOption3 = FALSE;
	m_bStreOption4 = FALSE;
	
	m_bServ = TRUE;
	m_nCalcType = 0;
	
	m_bFati = TRUE;
	
	m_nMeasurement = 1;

	m_nFatigueLife=0;
	m_nFatServ_G  =0;
	m_nFatServ_R  =0;
	m_nFatServ_I  =0;

	m_RcdsD.Initialize();
	m_Data.Initialize();
}

CDgnStlRatingParamAASHTODlg::~CDgnStlRatingParamAASHTODlg()
{
}

void CDgnStlRatingParamAASHTODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingParamAASHTODlg)
	DDX_Control(pDX, IDC_DGN_SYSTEM_FACT_EDT, m_edtSysTemFact);

	DDX_Check  (pDX, IDC_DGN_STRE_CHK,                     m_bStre);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_Y_EDT,          m_edtStrePhiy);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_U_EDT,          m_edtStrePhiu);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_C_EDT,          m_edtStrePhic);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_F_EDT,          m_edtStrePhif);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_V_EDT,          m_edtStrePhiv);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_SE_EDT,         m_edtStrePhise);
	DDX_Control(pDX, IDC_DGN_STRE_FACT_PHI_B_EDT,          m_edtStrePhib);
	DDX_Radio  (pDX, IDC_DGN_STRE_GIRDER_SINGLE_RDO,       m_nGirderType);
	DDX_Check  (pDX, IDC_DGN_STRE_GIRDER_StVT_STRESS_CHK,  m_bTorsionType);
	DDX_Radio  (pDX, IDC_DGN_STRE_BRIDGE_ST_RDO,           m_nBridgeType);
	DDX_Check  (pDX, IDC_DGN_STRE_OPTION_A6_CHK,           m_bStreOption1);
	DDX_Check  (pDX, IDC_DGN_STRE_OPTION_MNLIMIT_CHK,      m_bStreOption2);
	DDX_Check  (pDX, IDC_DGN_STRE_OPTION_BUCK_CHK,         m_bStreOption3);
	DDX_Check  (pDX, IDC_DGN_STRE_OPTION_LONG_TERM_SECT_PROP_CHK, m_bStreOption4);

	DDX_Check  (pDX, IDC_DGN_SERV_CHK,                  m_bServ);
	DDX_Radio  (pDX, IDC_DGN_SERV_AUTO_RDO,             m_nCalcType);
	DDX_Control(pDX, IDC_DGN_SERV_DGN_LOAD_COMP_EDT,    m_edtDgnComp);
	DDX_Control(pDX, IDC_DGN_SERV_DGN_LOAD_COMP_UNT,    m_untDgnComp);
	DDX_Control(pDX, IDC_DGN_SERV_DGN_LOAD_TENS_EDT,    m_edtDgnTens);
	DDX_Control(pDX, IDC_DGN_SERV_DGN_LOAD_TENS_UNT,    m_untDgnTens);
	DDX_Control(pDX, IDC_DGN_SERV_LEGAL_LOAD_COMP_EDT,  m_edtLegalComp);
	DDX_Control(pDX, IDC_DGN_SERV_LEGAL_LOAD_COMP_UNT,  m_untLegalComp);
	DDX_Control(pDX, IDC_DGN_SERV_LEGAL_LOAD_TENS_EDT,  m_edtLegalTens);
	DDX_Control(pDX, IDC_DGN_SERV_LEGAL_LOAD_TENS_UNT,  m_untLegalTens);

	DDX_Control(pDX, IDC_DGN_FATI_LIFE_PARAM_DETAIL_A_EDT,          m_edtDetailA   );
	DDX_Control(pDX, IDC_DGN_FATI_LIFE_PARAM_ADTT_SL_G_EDT,         m_edtADTTSL_G  );
	//DDX_Control(pDX, IDC_DGN_FATI_LIFE_PARAM_AVG_TRUCKNUM_P_EDT,    m_edtADTTSL_P  ); 지금은 사용안함.
	DDX_Control(pDX, IDC_DGN_FATI_LIFE_PARAM_AVG_TRUCKNUM_0_EDT,    m_edtADTTSL_0  );
	DDX_Control(pDX, IDC_DGN_FATI_LIFE_PARAM_AVG_TRUCKNUM_LIM_EDT,  m_edtADTTSL_Lim);

	DDX_Check  (pDX, IDC_DGN_FATI_CHK,           m_bFati);

	DDX_Radio  (pDX, IDC_DGN_MEASURE_STRAIN_RDO, m_nMeasurement);
	DDX_Radio  (pDX, IDC_DGN_FATI_INFINITE_RDO          , m_nFatigueLife);
	DDX_Radio  (pDX, IDC_DGN_FATI_LIFE_SERVINDEX_G_A_RDO, m_nFatServ_G  );
	DDX_Radio  (pDX, IDC_DGN_FATI_LIFE_SERVINDEX_R_A_RDO, m_nFatServ_R  );
	DDX_Radio  (pDX, IDC_DGN_FATI_LIFE_SERVINDEX_I_A_RDO, m_nFatServ_I  );

	DDX_Control(pDX, IDC_DGN_FATI_LIFE_GRP,         m_chkBmpFat);
	DDX_Control(pDX, IDC_DGN_SERV_LIMIT_STRESS_GRP, m_chkBmpSer);
	// 


	//}}AFX_DATA_MAP
}

void CDgnStlRatingParamAASHTODlg::InitCtrl()
{
	//////////////////////////////////////////////////////////////////////////
	// Enable
	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_FATI_LIFE_GRP,  TRUE);
	CDlgUtil::CtrlShowHide(this, aControls, m_RcdsD.nRatingCode == STL_AASHTO_LRFD19);
	GetDlgItem(IDC_DGN_FATI_GRP )->ShowWindow(m_RcdsD.nRatingCode == STL_AASHTO_LRFD19);
	//////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_DGN_STRE_OPTION_LONG_TERM_SECT_PROP_CHK)->ShowWindow(m_RcdsD.nRatingCode == STL_AASHTO_LRFD19);
	GetDlgItem(IDC_DGN_FATI_LIFE_GRP )->ShowWindow(m_RcdsD.nRatingCode == STL_AASHTO_LRFD19);

	m_aFatCtrls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aFatCtrls, IDC_DGN_FATI_GRP, TRUE);

	m_aTestCtrls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_aTestCtrls,  IDC_DGN_MEASURE_GRP,  TRUE);


	AlignControl();
}

void CDgnStlRatingParamAASHTODlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls;

	GetDlgItem(IDC_DGN_STRE_GRP )->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SERV_GRP )->GetWindowRect(rToMove);
	int nGap = rRef.bottom - rToMove.top;

	// Move Ctrl 
	/// - Strength
	if( m_RcdsD.nRatingCode != STL_AASHTO_LRFD19)
	{
		CRect rectGrp;

		GetDlgItem(IDC_DGN_STRE_OPTION_BUCK_CHK)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_STRE_OPTION_GRP)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.bottom + globalUtils.ScaleByDPI(4);

		GetDlgItem(IDC_DGN_STRE_OPTION_GRP)->GetWindowRect(rectGrp);
		rectGrp.bottom = rectGrp.bottom + nDistY;
		ScreenToClient(rectGrp);

		GetDlgItem(IDC_DGN_STRE_OPTION_GRP)->MoveWindow(rectGrp);

		GetDlgItem(IDC_DGN_STRE_GRP)->GetWindowRect(rectGrp);
		rectGrp.bottom = rectGrp.bottom + nDistY;
		ScreenToClient(rectGrp);

		GetDlgItem(IDC_DGN_STRE_GRP)->MoveWindow(rectGrp);

		CArray<UINT, UINT> aStreCtrls;
		aStreCtrls.Add(IDC_DGN_SERV_CHK);
		CDlgUtil::GetCtrlIDByIncRect(this, aStreCtrls, IDC_DGN_SERV_GRP, TRUE);
		CDlgUtil::CtrlMoveDistY(this, aStreCtrls, nDistY);
	}

	/// - Fatigue
	GetDlgItem(IDC_DGN_SERV_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_FATI_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top - nGap;
	CDlgUtil::CtrlMoveDistY(this, m_aFatCtrls, nDistY);

	/// - Test result
	if( m_RcdsD.nRatingCode == STL_AASHTO_LRFD19)
		GetDlgItem(IDC_DGN_FATI_GRP )->GetWindowRect(rRef);
	else
		GetDlgItem(IDC_DGN_FATI_CHK )->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_MEASURE_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top - nGap;
	CDlgUtil::CtrlMoveDistY(this, m_aTestCtrls, nDistY);

	/// - OK, Cancel
	aControls.RemoveAll();
	aControls.Add(IDOK);
	aControls.Add(IDCANCEL);
	GetDlgItem(IDC_DGN_MEASURE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top - nGap + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	ResizeDialog();
}

void CDgnStlRatingParamAASHTODlg::ResizeDialog()
{
	UINT nLastCtrlID = IDCANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}


void CDgnStlRatingParamAASHTODlg::InitUnit()
{
	m_edtSysTemFact.SetUnitType(D_UNITSYS_NONE);

	m_edtStrePhiy.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhiu.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhic.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhif.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhiv.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhise.SetUnitType(D_UNITSYS_NONE);
	m_edtStrePhib.SetUnitType(D_UNITSYS_NONE);

	m_edtDgnComp.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untDgnComp.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtDgnTens.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untDgnTens.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLegalComp.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untLegalComp.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLegalTens.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untLegalTens.SetUnitType(D_UNITSYS_BASE_STRESS);

    m_edtDetailA   .SetUnitType(D_UNITSYS_NONE);
	m_edtADTTSL_G  .SetUnitType(D_UNITSYS_NONE);
	m_edtADTTSL_P  .SetUnitType(D_UNITSYS_NONE);
	m_edtADTTSL_0  .SetUnitType(D_UNITSYS_NONE);
	m_edtADTTSL_Lim.SetUnitType(D_UNITSYS_NONE);
}

void CDgnStlRatingParamAASHTODlg::ControlsEnableDisable()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_STRE_GRP, m_bStre, FALSE);
	//CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SERV_GRP, m_bServ, FALSE);
	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_SERV_LIMIT_STRESS_GRP, FALSE);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bServ);
	//GetDlgItem(IDC_DGN_SERV_LIMIT_STRESS_GRP)->EnableWindow(m_bServ);

	GetDlgItem(IDC_DGN_STRE_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_SERV_CHK)->EnableWindow(TRUE);

	GetDlgItem(IDC_DGN_STRE_GIRDER_StVT_STRESS_CHK)->EnableWindow(m_bStre && m_nGirderType==1);

	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_GRP     )->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_COMP_STC)->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_COMP_EDT)->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_COMP_UNT)->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_TENS_STC)->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_TENS_EDT)->EnableWindow(m_bServ && m_nCalcType==1);
	GetDlgItem(IDC_DGN_SERV_DGN_LOAD_TENS_UNT)->EnableWindow(m_bServ && m_nCalcType==1);
	
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_GRP     )->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_COMP_STC)->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_COMP_EDT)->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_COMP_UNT)->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_TENS_STC)->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_TENS_EDT)->EnableWindow(m_bServ && m_nCalcType==1);			
	GetDlgItem(IDC_DGN_SERV_LEGAL_LOAD_TENS_UNT)->EnableWindow(m_bServ && m_nCalcType==1);		

	
	if(m_RcdsD.nRatingCode == STL_AASHTO_LRFD19)
	{
		aControls.RemoveAll();
		CDlgUtil::GetCtrlIDByIncRect(this, aControls, IDC_DGN_FATI_LIFE_GRP,  TRUE);
		CDlgUtil::CtrlEnableDisable(this, aControls, m_bFati);
		GetDlgItem(IDC_DGN_FATI_GRP )->ShowWindow(m_bFati);
	}

}

void CDgnStlRatingParamAASHTODlg::Data2Dlg()
{
	m_edtSysTemFact.SetEditUnit(m_Data.dSystemFactor);

	m_bStre = m_Data.bStrength;
	m_edtStrePhiy.SetEditUnit(m_Data.dPhi_y);
	m_edtStrePhiu.SetEditUnit(m_Data.dPhi_u);
	m_edtStrePhic.SetEditUnit(m_Data.dPhi_c);
	m_edtStrePhif.SetEditUnit(m_Data.dPhi_f);
	m_edtStrePhiv.SetEditUnit(m_Data.dPhi_v);
	m_edtStrePhise.SetEditUnit(m_Data.dPhi_se);
	m_edtStrePhib.SetEditUnit(m_Data.dPhi_b);
	m_nGirderType = m_Data.nGirderType;
	m_bTorsionType = m_Data.bConsiderTorsion;
	m_nBridgeType  = m_Data.nBridgeType;
	m_bStreOption1 = m_Data.bStrengthOption[0];
	m_bStreOption2 = m_Data.bStrengthOption[1];
	m_bStreOption3 = m_Data.bStrengthOption[2];
	m_bStreOption4 = m_Data.bStrengthOption[3];

	m_bServ = m_Data.bService;
	m_nCalcType = m_Data.nServCalcType;
	m_edtDgnComp.SetEditUnit(m_Data.dDgnLoadComp);
	m_edtDgnTens.SetEditUnit(m_Data.dDgnLoadTens);
	m_edtLegalComp.SetEditUnit(m_Data.dLegalLoadComp);
	m_edtLegalTens.SetEditUnit(m_Data.dLegalLoadTens);

	m_edtDetailA   .SetEditUnit(m_Data.dCurAge_a);
	m_edtADTTSL_G  .SetEditUnit(m_Data.dAdTTSL_g);
	//m_edtADTTSL_P  .SetEditUnit(m_Data.dAdTTSL_P);
	m_edtADTTSL_0  .SetEditUnit(m_Data.dAdTTSL_0);
	m_edtADTTSL_Lim.SetEditUnit(m_Data.dAdTTSL_lim);

	m_bFati = m_Data.bFatigue;
	m_nMeasurement = m_Data.nMeasurement;

	m_nFatigueLife = m_Data.nFatigueLife;
	m_nFatServ_G   = m_Data.nFatServ_G;
	m_nFatServ_R   = m_Data.nFatServ_R;
	m_nFatServ_I   = m_Data.nFatServ_I;
}

BOOL CDgnStlRatingParamAASHTODlg::Dlg2Data()
{	
	UpdateData(TRUE);


	double dSystemFactor = m_edtSysTemFact.GetEditValue();

		if(dSystemFactor < 0)  // MQC 18. condition factor value는 0이상 1 이하로 제한
		{
			dSystemFactor = 0.0; 
		} 
		else if(dSystemFactor > 1.2)
		{
			dSystemFactor = 1.2;
		}
	  m_edtSysTemFact.SetEditUnit(dSystemFactor);

	m_Data.dSystemFactor = dSystemFactor;
	
	m_Data.bStrength = m_bStre;
	m_Data.dPhi_y  = m_edtStrePhiy.GetEditValue();
	m_Data.dPhi_u  = m_edtStrePhiu.GetEditValue();
	m_Data.dPhi_c  = m_edtStrePhic.GetEditValue();
	m_Data.dPhi_f  = m_edtStrePhif.GetEditValue();
	m_Data.dPhi_v  = m_edtStrePhiv.GetEditValue();
	m_Data.dPhi_se = m_edtStrePhise.GetEditValue();
	m_Data.dPhi_b  = m_edtStrePhib.GetEditValue();
	m_Data.nGirderType = m_nGirderType;
	m_Data.bConsiderTorsion = m_bTorsionType;
	m_Data.nBridgeType = m_nBridgeType;
	m_Data.bStrengthOption[0] = m_bStreOption1;
	m_Data.bStrengthOption[1] = m_bStreOption2;
	m_Data.bStrengthOption[2] = m_bStreOption3;
	m_Data.bStrengthOption[3] = m_bStreOption4;
	
	m_Data.bService = m_bServ;
	m_Data.nServCalcType = m_nCalcType;
	m_Data.dDgnLoadComp  = m_edtDgnComp.GetEditValue();
	m_Data.dDgnLoadTens  = m_edtDgnTens.GetEditValue();
	m_Data.dLegalLoadComp  = m_edtLegalComp.GetEditValue();
	m_Data.dLegalLoadTens  = m_edtLegalTens.GetEditValue();

	m_Data.dCurAge_a   = m_edtDetailA   .GetEditValue();
	m_Data.dAdTTSL_g   = m_edtADTTSL_G  .GetEditValue();
	//m_Data.dAdTTSL_P   = m_edtADTTSL_P  .GetEditValue();
	m_Data.dAdTTSL_0   = m_edtADTTSL_0  .GetEditValue();
	m_Data.dAdTTSL_lim = m_edtADTTSL_Lim.GetEditValue();

	m_Data.bFatigue = m_bFati;
	m_Data.nMeasurement = m_nMeasurement;

	m_Data.nFatigueLife = m_nFatigueLife;
	m_Data.nFatServ_G   = m_nFatServ_G;
	m_Data.nFatServ_R   = m_nFatServ_R;
	m_Data.nFatServ_I   = m_nFatServ_I;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnStlRatingParamAASHTODlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlRatingParamAASHTODlg)
	ON_BN_CLICKED(IDC_DGN_UPDATE_CODE_BTN, OnDgnUpdateCodeBtn)
	ON_BN_CLICKED(IDC_DGN_STRE_CHK,        OnDgnStreChk)
	ON_BN_CLICKED(IDC_DGN_SERV_CHK,        OnDgnServChk)
	ON_BN_CLICKED(IDC_DGN_FATI_CHK,        OnDgnFatiChk)
	ON_BN_CLICKED(IDC_DGN_SERV_AUTO_RDO,   OnDgnCalcTypeRdo)
	ON_BN_CLICKED(IDC_DGN_SERV_USER_RDO,   OnDgnCalcTypeRdo)
	ON_BN_CLICKED(IDC_DGN_STRE_GIRDER_SINGLE_RDO,  OnDgnGirderTypeRdo)
	ON_BN_CLICKED(IDC_DGN_STRE_GIRDER_MULTI_RDO,   OnDgnGirderTypeRdo)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingParamAASHTODlg message handlers

BOOL CDgnStlRatingParamAASHTODlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_RcdsD.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRcds(m_RcdsD);

	//if (m_RcdsD.nRatingCode == STL_AASHTO_LRFD19)
	{
		// Auto Effect 끄자 (Auto Effect 가 왜 안먹을까...)
		m_chkBmpFat.InitControl(this, CDgnStlRatingParamAASHTODlg::IDD, IDC_DGN_FATI_LIFE_GRP, FALSE, TRUE);
		m_chkBmpFat.SetFoldState(FALSE);
		m_chkBmpSer.InitControl(this, CDgnStlRatingParamAASHTODlg::IDD, IDC_DGN_SERV_LIMIT_STRESS_GRP, FALSE, TRUE);
		m_chkBmpSer.SetFoldState(FALSE);
	}

	if(!pDoc->m_pAttrCtrl2->GetRpas(m_Data))
	{
		m_Data.Initialize();
	}

	InitCtrl();
	InitUnit();	
	Data2Dlg();

	UpdateData(FALSE);

	ControlsEnableDisable();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingParamAASHTODlg::OnOK() 
{
	Dlg2Data();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pDataCtrl->AddRpas(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CDgnStlRatingParamAASHTODlg::OnDgnUpdateCodeBtn() 
{
	T_RPAS_D RpasD; RpasD.Initialize();

	m_edtStrePhiy.SetEditUnit(RpasD.dPhi_y);
	m_edtStrePhiu.SetEditUnit(RpasD.dPhi_u);
	m_edtStrePhic.SetEditUnit(RpasD.dPhi_c);
	m_edtStrePhif.SetEditUnit(RpasD.dPhi_f);
	m_edtStrePhiv.SetEditUnit(RpasD.dPhi_v);
	m_edtStrePhise.SetEditUnit(RpasD.dPhi_se);
	m_edtStrePhib.SetEditUnit(RpasD.dPhi_b);

	UpdateData(FALSE);
}

void CDgnStlRatingParamAASHTODlg::OnDgnStreChk() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnStlRatingParamAASHTODlg::OnDgnServChk() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnStlRatingParamAASHTODlg::OnDgnFatiChk() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnStlRatingParamAASHTODlg::OnDgnCalcTypeRdo() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

void CDgnStlRatingParamAASHTODlg::OnDgnGirderTypeRdo() 
{
	UpdateData(TRUE);
	ControlsEnableDisable();
}

LRESULT CDgnStlRatingParamAASHTODlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignControl();
	return 0L;
}
