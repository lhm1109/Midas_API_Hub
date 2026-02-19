// DgnStlCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\wg_main\wg_mainRes2.h"
#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeDlg dialog


CDgnStlCodeDlg::CDgnStlCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlCodeDlg)
	m_bNotUseBLu = FALSE;
	m_bCheckDeflect = TRUE;
	m_bSpecialEQ = FALSE;
	m_bStrongColmWeakBeam = FALSE;
	m_iBDEnvelope = -1;
	m_iSafeLevel = -1;
	m_iEqGrade = -1;
	//}}AFX_DATA_INIT
	m_nFrameType = 0;
	m_iRatioCalMethod = 0;
	
	m_dNetGrossRatio = 0.85;
	for (int i=0; i<2; i++)	m_dBendModulus[i] = 1.0;
	m_nSeismicGrade = 0;
	m_bWeldBuch = FALSE;
	m_bStifRib = TRUE;
	m_dStifRibSpace = 300;
	m_bUseUGLcom4UGMemb = TRUE;
	m_bCombinedRatioLinearSum = TRUE;
	m_GroupKey = 0;

	m_bBendingCoeff = TRUE;
	m_bWebBeam = TRUE;
	m_bWebColm = TRUE;


	m_aSeisDgnParaCtrl.RemoveAll();
	m_aSeisDgnParaCtrl.Add(IDC_DGN_SEISMIC_DGN_PARAM_STC);
	m_aSeisDgnParaCtrl.Add(IDC_DGN_STRUCTURE_TYPE_STC   );
	m_aSeisDgnParaCtrl.Add(IDC_DGN_STRUCTURE_TYPE_CMB   );
	m_aSeisDgnParaCtrl.Add(IDC_DGN_STLCTRL_STRONG_COLM_WEAK_BEAM_CHK);

	m_aWidthThickCtrl.RemoveAll();
	m_aWidthThickCtrl.Add(IDC_DGN_DGNCODE_WTR_FRAME);
	m_aWidthThickCtrl.Add(IDC_DGN_WTR_MTYPE_RADIO);
	m_aWidthThickCtrl.Add(IDC_DGN_WTR_MFORCE_RADIO);
	m_aWidthThickCtrl.Add(IDC_DGN_WTR_NOCON_RADIO);
	
	m_aBuckResistCtrl.RemoveAll();
	m_aBuckResistCtrl.Add(IDC_DGN_BUCK_RESIST_GRP );
	m_aBuckResistCtrl.Add(IDC_DGN_BUCK_BIAXIAL_RDO);
	m_aBuckResistCtrl.Add(IDC_DGN_BUCK_MAXIMUM_RDO);
	m_aBuckResistCtrl.FreeExtra();

	m_aNationalAnnexCtrl.RemoveAll();
	m_aNationalAnnexCtrl.Add(IDC_DGN_NATIOANL_ANNEX_STC);
	m_aNationalAnnexCtrl.Add(IDC_DGN_CTRLDATA_SUBCODE_CMB);

	m_aSafetyCtrl.RemoveAll();
	m_aSafetyCtrl.Add(IDC_DGN_SAFETY_LEVEL0);
	m_aSafetyCtrl.Add(IDC_DGN_SAFETY_LEVEL1);
	m_aSafetyCtrl.Add(IDC_DGN_SAFETY_LEVEL2);
	m_aSafetyCtrl.Add(IDC_DGN_SAFETY_LEVEL3);
	
	m_aStrucSectCtrl.RemoveAll();
	m_aStrucSectCtrl.Add(IDC_DGN_STRUCT_IMPORT_COEF);
	m_aStrucSectCtrl.Add(IDC_DGN_STRUCT_IMPORT_COEF_CMB);
	m_aStrucSectCtrl.Add(IDC_DGN_SECT_NETGROSS_RATIO);
	m_aStrucSectCtrl.Add(IDC_DGN_SECT_NETGROSS_RATIO_EDT);

	m_aSeismiCtrl.RemoveAll();
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE_GROUP);
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE1);
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE2);
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE3);
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE4);
	m_aSeismiCtrl.Add(IDC_DGN_SEISMIC_GRADE5);
	
	m_aBeamBuckCrl.RemoveAll();
	m_aBeamBuckCrl.Add(IDC_DGN_BUCK_STRENGTH_GROUP);
	m_aBeamBuckCrl.Add(IDC_DGN_WELD_BUCK_CHECK);
	m_aBeamBuckCrl.Add(IDC_DGN_STIFRIB_CHECK);
	m_aBeamBuckCrl.Add(IDC_DGN_LATE_STIFRIB_SPACE);
	m_aBeamBuckCrl.Add(IDC_DGN_LATE_STIFRIB_SPACE_EDT);

	m_aDefaultCtrl.RemoveAll();
	m_aDefaultCtrl.Add(IDC_DGN_CTRLDATA_GB);
	m_aDefaultCtrl.Add(IDC_DGN_STLCTRL_DEFLECT);
	m_aDefaultCtrl.Add(IDC_DGN_STLCTRL_SPECIAL);

	m_aBendingCtrl.RemoveAll();
	m_aBendingCtrl.Add(IDC_DGN_STEEL_CHECK_PRO);
	m_aBendingCtrl.Add(IDC_DGN_STEEL_CHECK_BENDING);
	m_aBendingCtrl.Add(IDC_DGN_STEEL_CHECK_ENVELOP);

	m_aSmspCtrl.RemoveAll();
	m_aSmspCtrl.Add(IDC_DGN_SMSP_GROUP);
	m_aSmspCtrl.Add(IDC_DGN_SRSS_RDO);
	m_aSmspCtrl.Add(IDC_DGN_LINEAR_SUM_RDO);

	m_aBendModulusCtrl.RemoveAll();
	m_aBendModulusCtrl.Add(IDC_DGN_SECT_BENDMODULUS_RATIO);
	m_aBendModulusCtrl.Add(IDC_DGN_SECT_BENDMODULUS_RATIO_YDIR);
	m_aBendModulusCtrl.Add(IDC_DGN_SECT_BENDMODULUS_RATIO_Y_EDT);
	m_aBendModulusCtrl.Add(IDC_DGN_SECT_BENDMODULUS_RATIO_ZDIR);
	m_aBendModulusCtrl.Add(IDC_DGN_SECT_BENDMODULUS_RATIO_Z_EDT);
	m_aUGMemberCtrl.RemoveAll();
	m_aUGMemberCtrl.Add(IDC_DGN_USE_UGLCOM4UGMEMB_CHK);

	m_aEC3AddCtrl.RemoveAll();
	m_aEC3AddCtrl.Add(IDC_DGN_COMB_RATIO_CHK);
	m_aEC3AddCtrl.Add(IDC_DGN_INTER_KIJ_GROUP);
	m_aEC3AddCtrl.Add(IDC_DGN_INTER_KIJ_CODE_RDO);
	m_aEC3AddCtrl.Add(IDC_DGN_INTER_KIJ_AN_A_RDO);
	m_aEC3AddCtrl.Add(IDC_DGN_INTER_KIJ_AN_B_RDO);
	m_aEC3AddCtrl.Add(IDC_DGN_POINT_MCR_GROUP);
	m_aEC3AddCtrl.Add(IDC_DGN_POINT_MCR_TOP_RDO);
	m_aEC3AddCtrl.Add(IDC_DGN_POINT_MCR_CEN_RDO);
	m_aEC3AddCtrl.Add(IDC_DGN_POINT_MCR_BOT_RDO);

	m_aEC3SeisCtrl.RemoveAll();
	m_aEC3SeisCtrl.Add(IDC_DGN_BEHAVIOUR_FACTOR_GROUP);
	m_aEC3SeisCtrl.Add(IDC_DGN_BEHAVIOUR_FACTOR_Q_STC);
	m_aEC3SeisCtrl.Add(IDC_DGN_BEHAVIOUR_FACTOR_Q_EDT);
	m_aEC3SeisCtrl.Add(IDC_DGN_BEHAVIOUR_FACTOR_OV_STC);
	m_aEC3SeisCtrl.Add(IDC_DGN_BEHAVIOUR_FACTOR_OV_EDT);
	m_aEC3SeisCtrl.Add(IDC_DGN_STEEL_FRAME_TYPE_STC);
	m_aEC3SeisCtrl.Add(IDC_DGN_STEEL_FRAME_TYPE_CMB);
	m_aEC3SeisCtrl.Add(IDC_DGN_NON_SEIS_MEMB_STC);
	m_aEC3SeisCtrl.Add(IDC_DGN_NON_SEIS_MEMB_CMB);
	m_aEC3SeisCtrl.Add(IDC_DGN_NON_SEIS_MEMB_BTN);


	m_aIRC24AddCtrl.RemoveAll();
	m_aIRC24AddCtrl.Add(IDC_DGN_POINT_MCR_GROUP);
	m_aIRC24AddCtrl.Add(IDC_DGN_POINT_MCR_TOP_RDO);
	m_aIRC24AddCtrl.Add(IDC_DGN_POINT_MCR_CEN_RDO);
	m_aIRC24AddCtrl.Add(IDC_DGN_POINT_MCR_BOT_RDO);

	m_aAnnexAASHTOCtrls.RemoveAll();
	m_aAnnexAASHTOCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_STC);
	m_aAnnexAASHTOCtrls.Add(IDC_CRC_DGN_ANNEX_AASHTO_CMB);
	m_aAnnexAASHTOCtrls.FreeExtra();

	m_aAIJASD02Ctrl.RemoveAll();
	m_aAIJASD02Ctrl.Add(IDC_WG_JUD_STATIC16);
	m_aAIJASD02Ctrl.Add(IDC_JUD_CALC_SECT_S_BENDING_COEFF_CHK);
	m_aAIJASD02Ctrl.Add(IDC_WG_JUD_STATIC17);
	m_aAIJASD02Ctrl.Add(IDC_JUD_CALC_SECT_S_BENDING_BEAM_WEB_CHK);
	m_aAIJASD02Ctrl.Add(IDC_JUD_CALC_SECT_S_BENDING_COL_WEB_CHK);

	m_aCodeEnable.RemoveAll();
	m_nOldCodeIndex = 0;

    m_pDgnDataCtrl = new CDgnDataCtrl;

	m_pDstlData = NULL;
}

CDgnStlCodeDlg::~CDgnStlCodeDlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

#define IDSTL2(n)\
	arID2.Add(IDC_DGN_SAFETY_LEVEL##n);
#define IDSTL3(n)\
	arID3.Add(IDC_DGN_EQ_GRADE##n);

void CDgnStlCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlCodeDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_SUBCODE_CMB, m_DesignSubCode);
	DDX_Control(pDX, IDC_CRC_DGN_ANNEX_AASHTO_CMB, m_DesignAnnexAASHTO);
	DDX_Check(pDX, IDC_DGN_CTRLDATA_GB, m_bNotUseBLu);
	DDX_Check(pDX, IDC_DGN_STLCTRL_DEFLECT, m_bCheckDeflect);
	DDX_Check(pDX, IDC_DGN_STLCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_STLCTRL_STRONG_COLM_WEAK_BEAM_CHK, m_bStrongColmWeakBeam);
	DDX_Radio(pDX, IDC_DGN_SAFETY_LEVEL1, m_iSafeLevel);
	DDX_Radio(pDX, IDC_DGN_EQ_GRADE1, m_iEqGrade);
	DDX_Radio(pDX, IDC_DGN_STEEL_CHECK_BENDING, m_iBDEnvelope);
	//add by cylee 05.06.25
	DDX_Radio(pDX, IDC_DGN_WTR_MTYPE_RADIO, m_iWtrRatio);
	DDX_Radio(pDX, IDC_DGN_BUCK_BIAXIAL_RDO, m_iBuckResist);
	DDX_Control(pDX, IDC_DGN_STRUCTURE_TYPE_CMB, m_cmbStructureType);
	//add by maxiao(2015-10-26)GB50017-15
	DDX_Control(pDX, IDC_DGN_STRUCT_IMPORT_COEF_CMB, m_cmbStructImportCoef);
	DDX_Text(pDX, IDC_DGN_SECT_NETGROSS_RATIO_EDT, m_dNetGrossRatio);
	DDX_Text(pDX, IDC_DGN_SECT_BENDMODULUS_RATIO_Y_EDT, m_dBendModulus[0]);
	DDX_Text(pDX, IDC_DGN_SECT_BENDMODULUS_RATIO_Z_EDT, m_dBendModulus[1]);
	DDX_Radio(pDX, IDC_DGN_SEISMIC_GRADE1, m_nSeismicGrade);
	DDX_Check(pDX, IDC_DGN_WELD_BUCK_CHECK, m_bWeldBuch);
	DDX_Check(pDX, IDC_DGN_STIFRIB_CHECK, m_bStifRib);
	DDX_Text(pDX, IDC_DGN_LATE_STIFRIB_SPACE_EDT, m_dStifRibSpace);
	DDX_Radio(pDX, IDC_DGN_SRSS_RDO, m_iRatioCalMethod);
	DDX_Check(pDX, IDC_DGN_USE_UGLCOM4UGMEMB_CHK, m_bUseUGLcom4UGMemb);

	DDX_Radio(pDX, IDC_DGN_INTER_KIJ_CODE_RDO, m_nInteractionKijType);
	DDX_Radio(pDX, IDC_DGN_POINT_MCR_TOP_RDO, m_nLoadPositionMcr);
	DDX_Check(pDX, IDC_DGN_COMB_RATIO_CHK, m_bCombinedRatioLinearSum);
	DDX_Control(pDX, IDC_DGN_STEEL_FRAME_TYPE_CMB, m_cmbFrameType);
	DDX_Control(pDX, IDC_DGN_NON_SEIS_MEMB_CMB, m_cmbNonSeisMemb);
	DDX_Control(pDX, IDC_DGN_BEHAVIOUR_FACTOR_Q_EDT, m_edtBehaviourQ);
	DDX_Control(pDX, IDC_DGN_BEHAVIOUR_FACTOR_OV_EDT, m_edtBehaviourOV);

	DDX_Check(pDX, IDC_JUD_CALC_SECT_S_BENDING_COEFF_CHK, m_bBendingCoeff);
	DDX_Check(pDX, IDC_JUD_CALC_SECT_S_BENDING_BEAM_WEB_CHK, m_bWebBeam);
	DDX_Check(pDX, IDC_JUD_CALC_SECT_S_BENDING_COL_WEB_CHK, m_bWebColm);

	//}}AFX_DATA_MAP
	CArray<UINT,UINT> arID2;  IDSTL2(0)IDSTL2(1)IDSTL2(2)IDSTL2(3);
	CArray<UINT,UINT> arID3;  IDSTL3(0)IDSTL3(1)IDSTL3(2)IDSTL3(3)IDSTL3(4);
	int i=0;
	for(i=0; i<iDgn_StlID2; i++)	m_iID2[i] = arID2[i];
	for(i=0; i<iDgn_StlID3; i++)	m_iID3[i] = arID3[i];
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlCodeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlCodeDlg)
	ON_BN_CLICKED(IDC_DGN_CTRLDATA_ADV_BTN, OnDgnCtrlDataAdvBtn)
	ON_BN_CLICKED(IDC_DGN_STLCTRL_SPECIAL, OnDgnStlctrlSpecial)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_DESIGNCODE, OnSelchangeDgnCtrldataDesigncode)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_SUBCODE_CMB, OnSelchangeDgnCtrldataNationalAnnex)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_ANNEX_AASHTO_CMB, OnSelchangeDgnCtrldataDesignAnnexAashto)
	ON_BN_CLICKED(IDC_DGN_STIFRIB_CHECK, OnDgnStlctrlStifRib)
	ON_BN_CLICKED(IDC_DGN_NON_SEIS_MEMB_BTN, OnBtnGrup)
	ON_CBN_SETFOCUS(IDC_DGN_NON_SEIS_MEMB_CMB, OnSetGrupCombo)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlCodeDlg message handlers

BOOL CDgnStlCodeDlg::OnInitDialog() 
{
	MInitCombo initCombo;

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
#if defined(_TOWER)
	SetWindowText(_LS(IDS_DGN_TOWER_CODE_TITLE));
#endif
	m_DesignCode.ResetContent();
	CStringArray StlCodeName;
	CDBLib::GetStlCodeNameList(StlCodeName, &m_aCodeEnable);

	//(2009.01.15) Add by Unsang :: MNET.3908����, US Units ����
	CString strStlCodeName = _T("");
	for(int i=0; i<StlCodeName.GetSize(); i++)
	{		
		strStlCodeName = CDBLib::ViewConvertCodeName(StlCodeName[i]);
                
        if ( strStlCodeName != STLCODE_KSCE_ASD05 && strStlCodeName != STLCODE_KSCE_LSD15 &&
            strStlCodeName != STLCODE_KSCE_ASD10 && strStlCodeName != STLCODE_KSCE_RAIL_ASD11 &&
            strStlCodeName != STLCODE_KDS_24_14_30_2019 && strStlCodeName != STANDARDCODE_GB50068_2018 )
        { 
#if defined(_RUS)
					CInitCtrl::ConvertEngRusDesignCode(strStlCodeName);
#endif
            m_DesignCode.AddString(strStlCodeName);
        }			
	}
	
#ifdef _MGEN_CH
	if(m_pDstlData)
	{
		m_DesignCode.ResetContent();
		m_DesignCode.AddString(STLCODE_GB50017_17);
	}
#endif

	auto vString = CNationalAnnexTool::GetStlNationalAnnex();
	auto nNA = vString.size();
	for ( int i = 0; i<nNA; ++i )
	{
		CDlgUtil::CobxAddItem(m_DesignSubCode, vString[i], CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_DesignCode);

	m_DesignAnnexAASHTO.ResetContent();
	vString = CNationalAnnexTool::GetRcAASHTOAmendment();
	nNA = vString.size();
	for (int i = 0; i < nNA; ++i)
	{
		CDlgUtil::CobxAddItem(m_DesignAnnexAASHTO, vString[i], CNationalAnnexTool::ConvertAASHTOAmendment(vString[i]));
	}
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, 0/*Amend_Recommended*/);

	m_iSafeLevel	= 0;
	m_iEqGrade		= 0;
	m_iBDEnvelope = 0;
	//add by maxiao(2015-10-26)GB50017-15
	m_cmbStructImportCoef.AddString(_T("1.1"));
	m_cmbStructImportCoef.AddString(_T("1.0"));
	m_cmbStructImportCoef.AddString(_T("0.9"));
	m_cmbStructImportCoef.SetCurSel(1);
	
	CDlgUtil::CobxAddItem(m_cmbFrameType, _LS(IDS_DGN_FRAME_TYPE_MOMENT), EN_STL_EC3_MOMENT_FRAMES);
	CDlgUtil::CobxAddItem(m_cmbFrameType, _LS(IDS_DGN_FRAME_TYPE_BRACED), EN_STL_EC3_BRACE_FRAMES);
	SetGrupComboBox(m_cmbNonSeisMemb);
    if ( m_pDoc->m_pAttrCtrl->ExistDstl() )
    {
        Initial_ExistData();
    }
    else
    {
        Initial_Data();
    }
	OnSetGrupCombo();

#if defined(_TOWER)
	GetDlgItem(IDC_DGN_CTRLDATA_GB)->ShowWindow(SW_HIDE);
#endif

	GetDlgItem(IDC_DGN_CTRLDATA_ADV_BTN)->ShowWindow(SW_HIDE);

	OnSelchangeDgnCtrldataNationalAnnex();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlCodeDlg::Initial_Data()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	//(2009.01.15) Add by Unsang :: MNET.3908����, US Units ����
	rPref.DgnCode.SteelCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.SteelCode);
 
	T_DSTL_D rData;
	rData.Initialize();
	if(m_pDstlData)
	{
		rData = *m_pDstlData;
	}
	else
		m_pDoc->m_pAttrCtrl->GetDstl(rData);

	//(2009.01.15) Add by Unsang :: MNET.3908����, US Units ����
	rData.DesignCode = CDBLib::ViewConvertCodeName(rData.DesignCode);

	CString CodeName;
	CodeName = rPref.DgnCode.SteelCode;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif

	int Index = m_DesignCode.FindStringExact(-1, CodeName);
	if(Index!=-1)	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;

	CDlgUtil::CobxSetCurSelItemData(m_DesignSubCode, rPref.DgnCode.nStlNationalAnnex);
	//m_DesignSubCode.SetCurSel(rPref.DgnCode.nStlNationalAnnex);  
	m_iRatioCalMethod = 0;
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, 0/*Amend_Recommended*/);

#if defined(_IGEN)
	m_bNotUseBLu = TRUE;
#else 
	m_bNotUseBLu = FALSE;
#endif
	m_bCheckDeflect = TRUE;
	m_bSpecialEQ = FALSE;
	m_bStrongColmWeakBeam = FALSE;
	m_bUseUGLcom4UGMemb = TRUE;
	// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50017-03, GBJ17-88).
	m_pDgnDataCtrl->Get_DgnStlChinaDef(rPref.DgnCode.SteelCode, m_iSafeLevel, m_iEqGrade);
	// Get WTR Limit Option, add by cylee 05.06.25
	m_iWtrRatio   = rData.iOptionWTR;
	m_nFrameType  = rData.nFrameType;
	m_iBuckResist = rData.iUseMaxForce;

	m_nInteractionKijType = rData.nInteractionKijType;
	m_nLoadPositionMcr = rData.nLoadPositionMcr;
	m_bCombinedRatioLinearSum = rData.bCombinedRatioLinearSum;

	m_bBendingCoeff = rData.bBendingCoeff;
	m_bWebBeam = rData.bWebBeam;
	m_bWebColm = rData.bWebColm;


	const CString& strCode = rData.DesignCode;
	if (strCode != STLCODE_IS800_2007) {
		InitStructureTypeCombo();
	}
	else {
		InitStructureTypeComboIS();
	}

	m_edtBehaviourQ.SetEditUnit(rData.dq);
	m_edtBehaviourOV.SetEditUnit(rData.dgamma_ov);
	CDlgUtil::CobxSetCurSelItemData(m_cmbFrameType, rData.iFrameType);
	if (m_cmbNonSeisMemb.GetCount() > 0)	m_cmbNonSeisMemb.SetCurSel(0);

	UpdateData(FALSE);
	// Change by ZINU.('02.8.19). For GBJ17-88, GB50017-03. 
	// CHange by ParkBong.('06.05.18)
	SelchangeDgnCtrldataDesigncode(TRUE);
	CString strSelCode;
	m_DesignCode.GetLBText(m_DesignCode.GetCurSel(), strSelCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strSelCode);
#endif
	if (strSelCode == STLCODE_IS800_2007)
	{
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN_IS));
	}
}

void CDgnStlCodeDlg::Initial_ExistData()
{
	T_DSTL_D rData;
	rData.Initialize();
	if(m_pDstlData)
	{
		rData = *m_pDstlData;
	}
	else
		m_pDoc->m_pAttrCtrl->GetDstl(rData);

	//(2009.01.15) Add by Unsasng :: MNET.3908����, US Units ����
	rData.DesignCode = CDBLib::ViewConvertCodeName(rData.DesignCode);

	// Add, Jaeoh. [12/29/2008]
#if defined(_CIVIL)
	if(CDBLib::IsStlCode4CivilKR(rData.DesignCode))
	{
		m_pDoc->m_pDataCtrl->DelDstl();
		rData.Initialize();
	  T_PREFERENCE rPref;
	  rPref.Initialize();
	  m_pDoc->m_pInitCtrl->GetPreference(rPref);

		//(2009.01.15) Add by Unsang :: MNET.3908����, US Units ����
		rPref.DgnCode.SteelCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.SteelCode);

		rData.DesignCode = rPref.DgnCode.SteelCode;    
		if(CDBLib::IsStlCode4CivilKR(rData.DesignCode)) rData.Initialize();
	}
#endif
	m_bNotUseBLu	= rData.bAllFrameLaterallyBraced;
	m_bCheckDeflect = rData.bCheckDeflect;
	m_bSpecialEQ	= rData.bSpecialSeismic;
	m_bStrongColmWeakBeam = rData.bStrongColmWeakBeam;
	m_bUseUGLcom4UGMemb = rData.bUseUGLcom4UGMemb;
	// Get WTR Limit Option, add by cylee 05.06.25
	m_iWtrRatio   = rData.iOptionWTR;
	
	// add by tss(2017-7-27)
    m_dNetGrossRatio =  rData.dNetGrossAreaRatio;
	// add by tss(2021-01-12)
	for (int i=0; i<2; i++)		m_dBendModulus[i] = rData.dBendModulus[i];

    const CString& strCode = rData.DesignCode;
	m_nFrameType  = IsAISCCodeSeries(strCode) || (strCode == STLCODE_IS800_2007)  ? rData.nFrameType : 0;
	m_iBuckResist = (strCode == STLCODE_EC3_05 || strCode == STLCODE_EC3_2_05)? rData.iUseMaxForce : 0;
	m_nInteractionKijType = rData.nInteractionKijType;
	m_nLoadPositionMcr = rData.nLoadPositionMcr;
	m_bCombinedRatioLinearSum = rData.bCombinedRatioLinearSum;

	m_bBendingCoeff = rData.bBendingCoeff;
	m_bWebBeam = rData.bWebBeam;
	m_bWebColm = rData.bWebColm;

	if (strCode == STLCODE_IS800_2007) {
		//m_nFrameType = rData.nFrameType;
		InitStructureTypeComboIS();
		
	}
	else {
		InitStructureTypeCombo();
	}
	//add by maxiao(2015-10-26)GB50017-15
    if ( strCode == STLCODE_GB50017_15 )
	{
	  m_cmbStructImportCoef.SetCurSel(rData.nClass);
	  m_dNetGrossRatio = rData.dNetGrossAreaRatio;
	  m_nSeismicGrade = rData.SeisDgnGroupKey;
	  m_bWeldBuch = rData.bWeldBeambuckling;
	  m_bStifRib = rData.bSupportStiffRib;
	  m_dStifRibSpace = rData.dLatStiffRibSpace;
	}
	
	if(m_pDoc->m_pAttrCtrl->ExistDstl2() || m_pDstlData)
	{
		if( strCode == STLCODE_GB50017_17 || strCode == STLCODE_GB51249_2017 || strCode == STLCODE_JGJ209_2010)
		{
			m_iSafeLevel = rData.nSafeLevel;
			m_iEqGrade = rData.nEqGrade;
			m_iBDEnvelope = rData.nBDEnvelope;
		}
		else if( strCode == STLCODE_GBJ17_88 || strCode == STLCODE_GB50017_03 )
		{
			// Change by ZINU.('02.8.19). For GBJ17-88, GB50017-03.
			m_iSafeLevel	= (!m_bSpecialEQ ? rData.nClass : m_iSafeLevel);
			m_iEqGrade		= (m_bSpecialEQ ? rData.nClass : m_iEqGrade);
		}
	}
	int Index=0;
    if ( strCode != _T("") )
    {
			CString CodeName;
			CodeName = strCode;
#if defined(_RUS)
			CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif
        Index = m_DesignCode.FindStringExact(-1, strCode);
    }
    else
    {
        Index = 0;
    }
	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;

	int IndexNA = (rData.nNationalAnnex < 0) ? 0 : rData.nNationalAnnex;
	CDlgUtil::CobxSetCurSelItemData(m_DesignSubCode, IndexNA);

	CString strCodeTemp = CDBLib::GetConvertCodeName(strCode);
	if (strCodeTemp == STLCODE_AASHTO_LRFD17)
	{
		CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, IndexNA);
	}
	else
	{
		CDlgUtil::CobxSetCurSelItemData(m_DesignAnnexAASHTO, 0/*Amend_Recommended*/);
	}

	m_edtBehaviourQ.SetEditUnit(rData.dq);
	m_edtBehaviourOV.SetEditUnit(rData.dgamma_ov);
	CDlgUtil::CobxSetCurSelItemData(m_cmbFrameType, rData.iFrameType);
	m_GroupKey = rData.NonSeismicGroupKey;
	CDlgUtil::CobxSetCurSelItemData(m_cmbNonSeisMemb, rData.NonSeismicGroupKey);
	
	//CDlgUtil::CobxSetCurSelItemData(m_cmbNonSeisMemb, rData.iFrameType);
	// Change by ZINU.('02.8.19). For GBJ17-88, GB50017-03.
	// CHange by ParkBong.('06.05.18)
	UpdateData(FALSE);
	SelchangeDgnCtrldataDesigncode(FALSE);
	m_iRatioCalMethod = rData.nRatioCalMethod;
	UpdateData(FALSE);

	CString strSelCode;
	m_DesignCode.GetLBText(m_DesignCode.GetCurSel(), strSelCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strSelCode);
#endif
	if (strSelCode == STLCODE_IS800_2007)
	{
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN_IS));
	}
}

void CDgnStlCodeDlg::OnOK() 
{
	UpdateData(TRUE);
	T_DSTL_D rData;
	rData.Initialize();
	// Exist Steel Control Data at DB.
	if(m_pDstlData)
	{
		rData = *m_pDstlData;
	}
    if ( m_pDoc->m_pAttrCtrl->ExistDstl() )
    {
        m_pDoc->m_pAttrCtrl->GetDstl(rData);
    }
	rData.nRatioCalMethod = m_iRatioCalMethod;

	// Get Design Code.
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index,rData.DesignCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(rData.DesignCode);
#endif
	//(2009.01.15) Add by Unsang :: MNET.3908����, US Units ����
	rData.DesignCode = CDBLib::GetConvertCodeName(rData.DesignCode);
	
	if (rData.DesignCode == STLCODE_AASHTO_LRFD17)
	{
		rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnexAASHTO, m_DesignAnnexAASHTO.GetCurSel());
	}
	else
	{
		rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignSubCode, m_DesignSubCode.GetCurSel());
	}

	// Get Special Seismic and Uni Axis.
	rData.bAllFrameLaterallyBraced = m_bNotUseBLu;
	rData.bCheckDeflect            = m_bCheckDeflect;
	rData.bSpecialSeismic		   = m_bSpecialEQ;
	rData.bStrongColmWeakBeam      = m_bStrongColmWeakBeam;
	rData.bUseUGLcom4UGMemb		   = m_bUseUGLcom4UGMemb;
	// Get WTR Limit Option, add by cylee 05.06.25
	rData.iOptionWTR    		   = m_iWtrRatio;
	rData.bExistData = TRUE;

    const CString& strCode = rData.DesignCode;
	if ( strCode == STLCODE_EC3_05 || strCode == STLCODE_EC3_2_05 )
	{
		rData.iUseMaxForce = m_iBuckResist;

		rData.nInteractionKijType = m_nInteractionKijType;
		rData.nLoadPositionMcr = m_nLoadPositionMcr;
		rData.bCombinedRatioLinearSum = m_bCombinedRatioLinearSum;

		rData.bSpecialSeismic = m_bSpecialEQ;
		rData.dq = m_edtBehaviourQ.GetEditValue();
		rData.dgamma_ov = m_edtBehaviourOV.GetEditValue();

		int ix = m_cmbNonSeisMemb.GetCurSel();
		if (ix == CB_ERR) rData.NonSeismicGroupKey = 0;
		else              rData.NonSeismicGroupKey = m_cmbNonSeisMemb.GetItemData(ix);

		rData.iFrameType = CDlgUtil::CobxGetCurSelItemData(m_cmbFrameType, m_cmbFrameType.GetCurSel());
	}
    else
    {
        rData.iUseMaxForce = 0;
    }

	// Change by ZINU.('02.8.19). For GBJ17-88, GB50017-03.	Add by xuezc(2018/2/8)GB50017-17
	if ( strCode == STLCODE_GBJ17_88 || strCode == STLCODE_GB50017_03 || strCode == STLCODE_GB50017_17 || strCode == STLCODE_GB51249_2017 || strCode == STLCODE_JGJ209_2010)
	{
        if ( m_bSpecialEQ )
        {
            rData.nClass = m_iEqGrade; 
        }
        else
        {
            rData.nClass = m_iSafeLevel; 
        }

        rData.nSafeLevel = m_iSafeLevel;
        rData.nEqGrade = m_iEqGrade;
        rData.nBDEnvelope	= m_iBDEnvelope;
        //add by tss(2017.7.27).For GBJ17-88, GB50017-03.
        rData.dNetGrossAreaRatio = m_dNetGrossRatio;
		//add by tss(2021.1.12)
		for (int i=0; i<2; i++)	rData.dBendModulus[i] = m_dBendModulus[i];
        if ( rData.dNetGrossAreaRatio < 1e-6 || rData.dNetGrossAreaRatio > 1.0 )
        {
            MessageBox(_LS(IDS_DGN_TEXT_SECT_NETGROSS_RATIO_ERROR));
            return;
        }
	}
    else if ( strCode == STLCODE_GB50017_15 )
	{
		rData.nClass = m_cmbStructImportCoef.GetCurSel();
		rData.dNetGrossAreaRatio = m_dNetGrossRatio;
		rData.SeisDgnGroupKey = m_nSeismicGrade;
		rData.bWeldBeambuckling = m_bWeldBuch;
		rData.bSupportStiffRib = m_bStifRib;
		rData.dLatStiffRibSpace = m_dStifRibSpace;
	}
    else
    {
        rData.nClass = 0;
    }

	if (strCode == STLCODE_SP_16_13330_2017)
	{
		rData.nLoadPositionMcr = m_nLoadPositionMcr;		
		rData.bSpecialSeismic = m_bSpecialEQ;		
	}
	if (strCode == STLCODE_IRC_24_2010)
	{
		rData.nLoadPositionMcr = m_nLoadPositionMcr;
	}

	if (strCode == STLCODE_AIJ_ASD02)
	{
		rData.bBendingCoeff = m_bBendingCoeff;
		rData.bWebBeam = m_bWebBeam;
		rData.bWebColm = m_bWebColm;
	}

	// Add by ZINU.('03.11.19). For Strength Reduction Factor by Code.
    for ( int i=0; i<5; i++ )
    {
        rData.dPhi[i]=0.0;
    }
	// Save Steel Control Data at DB.

	if (IsAISCCodeSeries(rData.DesignCode))
	{    
		int ix = m_cmbStructureType.GetCurSel();
		if (ix == CB_ERR) rData.nFrameType = 0;
		else              rData.nFrameType = m_cmbStructureType.GetItemData(ix);
	}

	if (strCode == STLCODE_IS800_2007)
	{
		int ix = m_cmbStructureType.GetCurSel();
		if (ix == CB_ERR) rData.nFrameType = 0;
		else              rData.nFrameType = m_cmbStructureType.GetItemData(ix);;
	}
	if(m_pDstlData)
	{
		*m_pDstlData = rData;
		CDialogMove::OnOK();
	}
	else
	{
		BOOL bCheck = m_pDoc->m_pDataCtrl->AddDstl(rData);
		if(bCheck)
		{
			if(!CDBLib::IsCodeForStlSmsp())
			{
				T_UPDATE_HINT Hint;
				Hint.csNotifyCmd = _LSX(STLCODE_UPDATE);
				m_pDoc->UpdateAllViews(NULL, D_UPDATE_NOTIFY, &Hint);
			}


			m_pDoc->m_pDataCtrl->AddDstlLog(rData);
			CDialogMove::OnOK();
		}
	}
}

void CDgnStlCodeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnStlCodeDlg::OnDgnCtrlDataAdvBtn() 
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_IMPL_JUD_CALC_SECT_S, 0), 0);
}

void CDgnStlCodeDlg::OnDgnStlctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeDgnCtrldataDesigncode();
	OnSelchangeDgnCtrldataNationalAnnex();
}
void CDgnStlCodeDlg::OnCodeChange4CH(const CString &strCodeName)
{
    if ( strCodeName == STLCODE_GB50017_03 || strCodeName == STLCODE_GB50017_17 || strCodeName == STLCODE_GB51249_2017 || strCodeName == STLCODE_JGJ209_2010)
	{
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE1, _LS(IDS_DGN_TEXT_GREQ_STL1_GB15));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE2, _LS(IDS_DGN_TEXT_GREQ_STL2_GB15));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE3, _LS(IDS_DGN_TEXT_GREQ_STL3_GB15));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE4, _LS(IDS_DGN_TEXT_GREQ_STL4_GB15));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE0, _LS(IDS_IDD_DGN_STL_CODE_DLG_IDC_DGN_EQ_GRADE0_03));
	}
	else
	{
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE1, _LS(IDS_DGN_TEXT_GREQ_STL1_GB));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE2, _LS(IDS_DGN_TEXT_GREQ_STL2_GB));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE3, _LS(IDS_DGN_TEXT_GREQ_STL3_GB));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE4, _LS(IDS_DGN_TEXT_GREQ_STL4_GB));
		CDlgUtilEx::SetWindowText(this, IDC_DGN_EQ_GRADE0, _LS(IDS_IDD_DGN_STL_CODE_DLG_IDC_DGN_EQ_GRADE0));
	}
}

void CDgnStlCodeDlg::SetGrupComboBox(MComboBox& cobx)
{
	cobx.ResetContent();
	cobx.SetItemData(cobx.AddString(_T("None")), 0);
	cobx.SetCurSel(0);

	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(aKey);
	int nCount = aKey.GetSize();
	if (nCount < 1) return;

	T_GRUP_D Data;
	for (int i = 0; i < nCount; ++i)
	{
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(aKey[i], Data)) continue;
		int ix = cobx.AddString(Data.GroupName);
		cobx.SetItemData(ix, aKey[i]);
	}
}
void CDgnStlCodeDlg::SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef) 
{
	//SetRedraw(FALSE);
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.8.19). For Dynamic Size.
	UpdateData(TRUE);
	m_iRatioCalMethod = 0;

	// For GBJ17-88, GB50017-03.
    for ( int i=0; i<iDgn_StlID2; i++ )
    {
        GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
    }
    for ( int i=0; i<iDgn_StlID3; i++ )
    {
        GetDlgItem(m_iID3[i])->ShowWindow(SW_HIDE);
    }

	CString strDgnCode=_T("");
	m_DesignCode.GetWindowText(strDgnCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDgnCode);
#endif
	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
#if defined(_CIVIL)// Civil�ϰ�� Braced�� ������ ��� �ɼǻ����� �����.(071105 sshan)
	GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->ShowWindow(SW_HIDE);  
	CDlgUtil::CtrlShowHide(this, m_aWidthThickCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aSeisDgnParaCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBuckResistCtrl , FALSE);
	CDlgUtil::CtrlShowHide(this, m_aSmspCtrl ,       FALSE  );
	CDlgUtil::CtrlShowHide(this, m_aEC3AddCtrl,  strDgnCode == STLCODE_EC3_2_05);
	/*CDlgUtil::CtrlShowHide(this, m_aIRC24AddCtrl, strDgnCode == STLCODE_IRC_24_2010);*/
	CDlgUtil::CtrlShowHide(this, m_aAnnexAASHTOCtrls, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aAIJASD02Ctrl, FALSE);
	
	if(strDgnCode == STLCODE_EC3_2_05)
	{
        CDlgUtil::CtrlShowHide(this, m_aNationalAnnexCtrl, TRUE);
        GetDlgItem(IDC_DGN_NATIOANL_ANNEX_STC)->GetWindowRect(&RectOrg);

        CRect rTarget, rSource;
        GetDlgItem(IDC_DGN_NATIOANL_ANNEX_STC)->GetWindowRect(rSource);
        GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(rTarget);
        int nDdistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(8);
        CDlgUtil::CtrlMoveDistY(this, m_aDefaultCtrl, nDdistY);
        GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(&RectOrg);

		CDlgUtil::CtrlShowHide(this, m_aBuckResistCtrl , TRUE );
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(rSource);
		GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(rTarget);
		int nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aBuckResistCtrl, nDistY);	
		
		CRect moveR, moveR2;
		GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
		GetDlgItem(IDC_DGN_COMB_RATIO_CHK)->GetWindowRect(moveR2);
		nDistY = moveR.bottom - moveR2.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEC3AddCtrl, nDistY);
		GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(&RectOrg);
		m_bSpecialEQ = 0;
	}
	else if (strDgnCode == STLCODE_AISC_ASD89 || strDgnCode == STLCODE_KSSC_ASD03)
	{
        GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(&RectOrg);
        CDlgUtil::CtrlShowHide(this, m_aNationalAnnexCtrl, FALSE);

        CRect rTarget, rSource;
        GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rSource);
        GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(rTarget);
        int nDdistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(8);
        CDlgUtil::CtrlMoveDistY(this, m_aDefaultCtrl, nDdistY);
        GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(&RectOrg);

		CDlgUtil::CtrlShowHide(this, m_aSmspCtrl, TRUE);
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(rSource);
		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(rTarget);
		int nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aSmspCtrl, nDistY);
		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(&RectOrg);
	}
    else
    {
        GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(&RectOrg);
        CDlgUtil::CtrlShowHide(this, m_aNationalAnnexCtrl, FALSE);

		BOOL bShowAmendent = FALSE;

		CString strCodeTemp = CDBLib::GetConvertCodeName(strDgnCode);
		if (strCodeTemp == STLCODE_AASHTO_LRFD17) bShowAmendent = TRUE;

		CRect rTarget, rSource;
		if (bShowAmendent)
		{
			CDlgUtil::CtrlShowHide(this, m_aAnnexAASHTOCtrls, bShowAmendent);

			int nDistX, nDistY;
			GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rSource);
			GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(rTarget);
			nDistX = rSource.left - rTarget.left;
			nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAnnexAASHTOCtrls, nDistX, nDistY);

			GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->GetWindowRect(rSource);
			GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(rTarget);
			int nDdistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, m_aDefaultCtrl, nDdistY);
			GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(&RectOrg);

		}
		else
		{
			GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(rSource);
			GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(rTarget);
			int nDdistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, m_aDefaultCtrl, nDdistY);
			GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(&RectOrg);
			if (strDgnCode == STLCODE_IRC_24_2010) {
				CDlgUtil::CtrlShowHide(this, m_aIRC24AddCtrl, TRUE);
				GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(rSource);
				GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(rTarget);
				int nDistY = rSource.bottom - rTarget.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aIRC24AddCtrl, nDistY);
				GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(&RectOrg);
			}
		}
    }

#else // Gen�ϰ��	
	if (strDgnCode != STLCODE_IS800_2007) {
		InitStructureTypeCombo();
	}
	else {
		InitStructureTypeComboIS();
	}
	
	CArray<UINT,UINT> aMoveCtrls;
	aMoveCtrls.Append(m_aWidthThickCtrl);
    for ( int k = 0; k < iDgn_StlID2; k++ )
    {
        aMoveCtrls.Add(m_iID2[k]);
    }
    for ( int k = 0; k < iDgn_StlID3; k++ )
    {
        aMoveCtrls.Add(m_iID3[k]);
    }
	aMoveCtrls.FreeExtra();

	CRect rTgt, rSrc;
    if ( strDgnCode == STLCODE_AIJ_ASD02 || strDgnCode == STLCODE_AIK_ASD83 )
    {
        GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(rSrc);
    }
    else
    {
        GetDlgItem(IDC_DGN_SAFETY_LEVEL0)->GetWindowRect(rSrc);
    }
	
	int nCodeType = 0;
    if ( IsAISCCodeSeries(strDgnCode) && m_bSpecialEQ )
    {
        nCodeType = 1;
    }
    else if ( strDgnCode == STLCODE_GBJ17_88  || strDgnCode == STLCODE_GB50017_03 || strDgnCode == STLCODE_GB50017_17 || strDgnCode==STLCODE_GB51249_2017 || strDgnCode == STLCODE_JGJ209_2010)
    {
        nCodeType = 2;
    }
    else if ( strDgnCode == STLCODE_AIJ_ASD02 || strDgnCode == STLCODE_AIK_ASD83 )
    {
        nCodeType = 3;
    }
    else if ( strDgnCode == STLCODE_EC3_05 )
    {
        nCodeType = 4;
    }
    else if ( strDgnCode == STLCODE_EC3_2_05 )
    {
        nCodeType = 5;
    }
    else if ( strDgnCode == STLCODE_GB50017_15 )
    {
        nCodeType = 6;//add by maxiao(2015-10-26)GB50017-15
    }
	else if (strDgnCode == STLCODE_IS800_2007 && m_bSpecialEQ)
	{
		nCodeType = 7;//add by maxiao(2015-10-26)GB50017-15
	}
	else if (strDgnCode == STLCODE_SP_16_13330_2017 && m_bSpecialEQ) {
		nCodeType = 8;
	}
    else
    {
        nCodeType = 9;
    }

	BOOL bSeis    = FALSE;
	BOOL bSafety  = FALSE;
	BOOL bWidth   = FALSE;
	BOOL bBuck    = FALSE;
	BOOL bEQGrade = FALSE;
	BOOL bNA      = FALSE;
	BOOL bStruct  = FALSE;////add by maxiao(2015-10-26)GB50017-15
	BOOL bSeismic = FALSE;////add by maxiao(2015-10-26)GB50017-15  
	BOOL bBeamBk  = FALSE;////add by maxiao(2015-10-26)GB50017-15
	BOOL bRatioCalMethod    = CDBLib::IsCodeForStlSmsp(strDgnCode);
	BOOL bBendModulus = (strDgnCode == STLCODE_GB50017_17 || strDgnCode == STLCODE_JGJ209_2010);
    BOOL bUseLcomUG = (strDgnCode == STLCODE_KDS_41_31_2019 || strDgnCode == STLCODE_KDS_41_30_10_2022) ? TRUE : FALSE;
	BOOL bEC3Add = (strDgnCode == STLCODE_EC3_05 || strDgnCode == STLCODE_EC3_2_05) ? TRUE : FALSE;
	BOOL bAIJASD02 = strDgnCode == STLCODE_AIJ_ASD02;
	switch(nCodeType)
	{
    case 1: { bSeis   = TRUE;  }            break;
    case 2: { bSafety = FALSE; }           break;
    case 3: { bWidth  = TRUE;  }           break;
    case 4: { /*bSeis   = TRUE;*/  bBuck    = TRUE; bNA = TRUE; } break;
    case 5: { /*bSeis   = TRUE;*/  bBuck    = TRUE;             } break;
    case 6: { bStruct = TRUE;  bSeismic = TRUE; bBeamBk = TRUE; }  break;
	case 7: { /*bStruct = TRUE;*/  bSeis = TRUE;/* bBeamBk = TRUE;*/ }  break;
	case 8: { bSeis = TRUE; } break;
	}

	CDlgUtil::CtrlShowHide(this, m_aNationalAnnexCtrl,  bNA    );
	CDlgUtil::CtrlShowHide(this, m_aDefaultCtrl,        TRUE   );
	CDlgUtil::CtrlShowHide(this, m_aSeisDgnParaCtrl,    bSeis  );
	CDlgUtil::CtrlShowHide(this, m_aWidthThickCtrl,     bWidth );
	CDlgUtil::CtrlShowHide(this, m_aSafetyCtrl,         bSafety);
	CDlgUtil::CtrlShowHide(this, m_aBuckResistCtrl ,    bBuck  );
	CDlgUtil::CtrlShowHide(this, m_aStrucSectCtrl ,     bStruct  );
	CDlgUtil::CtrlShowHide(this, m_aSeismiCtrl ,        bSeismic );
	CDlgUtil::CtrlShowHide(this, m_aBeamBuckCrl ,       bBeamBk  );
	CDlgUtil::CtrlShowHide(this, m_aBendingCtrl ,       FALSE  );
	CDlgUtil::CtrlShowHide(this, m_aSmspCtrl ,       bRatioCalMethod  );
	CDlgUtil::CtrlShowHide(this, m_aBendModulusCtrl, bBendModulus);
	CDlgUtil::CtrlShowHide(this, m_aUGMemberCtrl   , bUseLcomUG);
	CDlgUtil::CtrlShowHide(this, m_aEC3AddCtrl, bEC3Add);
	CDlgUtil::CtrlShowHide(this, m_aEC3SeisCtrl, bEC3Add && m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aAIJASD02Ctrl, bAIJASD02);

	CRect refRect, moveRect;
	GetDlgItem(IDC_DGN_CTRLDATA_SUBCODE_CMB)->GetWindowRect(refRect);
	GetDlgItem(IDC_DGN_CTRLDATA_GB        )->GetWindowRect(moveRect);
	int nNADistY = (nCodeType==4) ? refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8) : refRect.top - moveRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aDefaultCtrl, nNADistY);
	CArray<UINT,UINT> aCtrlID;

	aCtrlID.Add(IDC_DGN_STLCTRL_DEFLECT);
	aCtrlID.Add(IDC_DGN_STLCTRL_SPECIAL);
	int bBDHeight = 0;
    if ( strDgnCode == STLCODE_GB50017_17 || strDgnCode == STLCODE_JGJ209_2010)
	{
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(refRect);
		GetDlgItem(IDC_DGN_STEEL_CHECK_PRO)->GetWindowRect(moveRect);
		int nBDDistY = refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(6);
		bBDHeight = moveRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlShowHide(this, m_aBendingCtrl,TRUE);
		CDlgUtil::CtrlMoveDistY(this,m_aBendingCtrl,nBDDistY);
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->ShowWindow(SW_HIDE);
	}
    if ( strDgnCode == STLCODE_GB51249_2017 )
	{
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(refRect);
		GetDlgItem(IDC_DGN_STEEL_CHECK_PRO)->GetWindowRect(moveRect);
		int nBDDistY = refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(6);
		bBDHeight = moveRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlShowHide(this, m_aBendingCtrl,TRUE);
		CDlgUtil::CtrlMoveDistY(this,m_aBendingCtrl,nBDDistY);
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_CHECK_PRO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_CHECK_BENDING)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_CHECK_ENVELOP)->EnableWindow(FALSE);
	}
	if (strDgnCode == STLCODE_IS800_2007)
	{
		//GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(refRect);
		//GetDlgItem(IDC_DGN_STEEL_CHECK_PRO)->GetWindowRect(moveRect);
		//int nBDDistY = refRect.bottom - moveRect.top + 6;
		//bBDHeight = moveRect.bottom - moveRect.top + 8;
		//CDlgUtil::CtrlShowHide(this, m_aBendingCtrl, TRUE);
		//CDlgUtil::CtrlMoveDistY(this, m_aBendingCtrl, nBDDistY);
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_DGN_CTRLDATA_GB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->EnableWindow(TRUE);
		/*GetDlgItem(IDC_DGN_STEEL_CHECK_BENDING)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_CHECK_ENVELOP)->EnableWindow(FALSE);*/
	}
	
	if (bBDHeight != 0)
	{
		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->GetWindowRect(moveRect);
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(refRect);

		nNADistY = refRect.top - moveRect.top + refRect.Height() + bBDHeight;
		CDlgUtil::CtrlMoveDistY(this, aCtrlID, nNADistY);
	}
	
	CRect refR, moveR;
	GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(refR);  
	GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(&RectOrg);

	if(bRatioCalMethod)
	{
		CRect smspR;
		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(smspR);
		int nDistY = refR.bottom - smspR.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aSmspCtrl, nDistY);

		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(&RectOrg);

		refR = RectOrg;
	}

	if (IsAISCCodeSeries(strDgnCode) && m_bSpecialEQ)
	{
		GetDlgItem(IDC_DGN_SEISMIC_DGN_PARAM_STC)->GetWindowRect(moveR);
		int nDistY = (nCodeType==1) ? refR.bottom - moveR.top + globalUtils.ScaleByDPI(8) : refR.top - moveR.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSeisDgnParaCtrl, nDistY);

		GetDlgItem(IDC_DGN_STLCTRL_STRONG_COLM_WEAK_BEAM_CHK)->GetWindowRect(&RectOrg);
	}
	else if (strDgnCode == STLCODE_IS800_2007 && m_bSpecialEQ)
	{
		GetDlgItem(IDC_DGN_SEISMIC_DGN_PARAM_STC)->GetWindowRect(moveR);
		int nDistY = (nCodeType == 7) ? refR.bottom - moveR.top + 8 : refR.top - moveR.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSeisDgnParaCtrl, nDistY);

		GetDlgItem(IDC_DGN_STLCTRL_STRONG_COLM_WEAK_BEAM_CHK)->GetWindowRect(&RectOrg);
	}
    else if ( strDgnCode == STLCODE_GBJ17_88 || strDgnCode == STLCODE_GB50017_03 || strDgnCode == STLCODE_GB50017_17|| strDgnCode == STLCODE_GB51249_2017 || strDgnCode == STLCODE_JGJ209_2010)
	{
		CRect refRECT;
		GetDlgItem(IDC_DGN_SAFETY_LEVEL0  )->GetWindowRect(moveR);
		int nDistY = (nCodeType==2) ? refR.bottom - moveR.top + globalUtils.ScaleByDPI(8) : refR.top - moveR.top;
		CDlgUtil::CtrlMoveDistY(this, m_aSafetyCtrl, nDistY);

        if ( m_bSpecialEQ )
        {
            RectOrg = MoveCtrl(3, m_iID3);
        }
        else
        {
            RectOrg = MoveCtrl(2, m_iID2);
        }

		//by tss (2017.7.27)
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO)->GetWindowRect(refRECT);
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO_EDT)->GetWindowRect(refRECT);
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO_EDT)->ShowWindow(TRUE);
		int nDistY2 = RectOrg.bottom - refRECT.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aStrucSectCtrl, nDistY2);
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO_EDT)->GetWindowRect(&RectOrg);

		//by tss(2021.01.12)
		if ( strDgnCode == STLCODE_GB50017_17 || strDgnCode == STLCODE_JGJ209_2010)
		{
			CRect refRect;
			GetDlgItem(IDC_DGN_SECT_BENDMODULUS_RATIO)->GetWindowRect(refRect);
			int nDistY3 = refRECT.bottom - refRect.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, m_aBendModulusCtrl, nDistY2+nDistY3);
			GetDlgItem(IDC_DGN_SECT_BENDMODULUS_RATIO)->GetWindowRect(&RectOrg);
		}
		
	}
	else if( strDgnCode == STLCODE_GB50017_15 )//add by maxiao(2015-9-25)GB50017-15
	{
        CRect refRECT;
        GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(refRECT);
        GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->ShowWindow(FALSE);
        GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO)->GetWindowRect(moveR);
        int nDistY = refRECT.bottom - moveR.top + globalUtils.ScaleByDPI(8);
        CDlgUtil::CtrlMoveDistY(this, m_aStrucSectCtrl, nDistY);

        CRect moveR2, moveR3;
        GetDlgItem(IDC_DGN_SEISMIC_GRADE_GROUP)->GetWindowRect(moveR2);
        int nDistY2 = moveR.bottom - moveR2.top + globalUtils.ScaleByDPI(8);
        CDlgUtil::CtrlMoveDistY(this, m_aSeismiCtrl, nDistY2 + nDistY);

        GetDlgItem(IDC_DGN_BUCK_STRENGTH_GROUP)->GetWindowRect(moveR3);
        int nDistY3 = moveR2.bottom - moveR3.top + globalUtils.ScaleByDPI(8);
        CDlgUtil::CtrlMoveDistY(this, m_aBeamBuckCrl, nDistY3 + nDistY2 + nDistY);
        GetDlgItem(IDC_DGN_BUCK_STRENGTH_GROUP)->GetWindowRect(&RectOrg);
        OnDgnStlctrlStifRib();
	}
    else if ( strDgnCode == STLCODE_AIJ_ASD02 || strDgnCode == STLCODE_AIK_ASD83 )
	{
		GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(moveR);
		int nDistY = (nCodeType==3) ? refR.bottom - moveR.top + globalUtils.ScaleByDPI(8) : refR.top - moveR.top;
		CDlgUtil::CtrlMoveDistY(this, m_aWidthThickCtrl, nDistY);
		
		GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(&RectOrg);
	}
    else if ( strDgnCode == STLCODE_EC3_05 )
	{
		CRect refRECT;
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(refRECT);
		if (m_bSpecialEQ)
		{
			GetDlgItem(IDC_DGN_BEHAVIOUR_FACTOR_GROUP)->GetWindowRect(moveR);
			int nDistY = refRECT.bottom - moveR.top + 8;
			CDlgUtil::CtrlMoveDistY(this, m_aEC3SeisCtrl, nDistY);

			GetDlgItem(IDC_DGN_NON_SEIS_MEMB_STC)->GetWindowRect(refRECT);
		}
		GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
		int nDistY = (nCodeType==4) ? refRECT.bottom - moveR.top + globalUtils.ScaleByDPI(8) : refRECT.top - moveR.top;
		CDlgUtil::CtrlMoveDistY(this, m_aBuckResistCtrl, nDistY);
		
		CRect moveR2;
		GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
		GetDlgItem(IDC_DGN_COMB_RATIO_CHK)->GetWindowRect(moveR2);
		nDistY = moveR.bottom - moveR2.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEC3AddCtrl, nDistY);

		GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(&RectOrg);
		//m_bSpecialEQ = 0;
	}
    else if ( strDgnCode == STLCODE_EC3_2_05 )
    {
        GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
        int nDistY = (nCodeType==5) ? refR.bottom - moveR.top + globalUtils.ScaleByDPI(8) : refR.top - moveR.top;
        CDlgUtil::CtrlMoveDistY(this, m_aBuckResistCtrl, nDistY);

		CRect moveR2;
		GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
		GetDlgItem(IDC_DGN_COMB_RATIO_CHK)->GetWindowRect(moveR2);
		nDistY = moveR.bottom - moveR2.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEC3AddCtrl, nDistY);

        GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(&RectOrg);
		//m_bSpecialEQ = 0;
	}
	else if (strDgnCode == STLCODE_IS800_2007)
	{
	//GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
	//int nDistY = (nCodeType == 5) ? refR.bottom - moveR.top + 8 : refR.top - moveR.top;
	//CDlgUtil::CtrlMoveDistY(this, m_aBuckResistCtrl, nDistY);

	//CRect moveR2;
	//GetDlgItem(IDC_DGN_BUCK_RESIST_GRP)->GetWindowRect(moveR);
	//GetDlgItem(IDC_DGN_COMB_RATIO_CHK)->GetWindowRect(moveR2);
	//nDistY = moveR.bottom - moveR2.top + 8;
	//CDlgUtil::CtrlMoveDistY(this, m_aEC3AddCtrl, nDistY);

	//GetDlgItem(IDC_DGN_POINT_MCR_GROUP)->GetWindowRect(&RectOrg);
	//m_bSpecialEQ = 0;
	} 
	else if (strDgnCode == STLCODE_SP_16_13330_2017 && m_bSpecialEQ) {
	}
	else
	{    
		m_bSpecialEQ = 0;
	}

    if ( IsSpecialEQCode(strDgnCode) )
    {
        GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->EnableWindow(FALSE);
    }

// MNET:XXXX-HSSHIM-20111129
// iGen������ �Ϻ� Control�� �����.
// ������� ��ȭ���ڿ��� ���� ������ �� �ֱ� ������ ����ȭ ��Ű�� ����.
// #if defined(_IGEN)
//     if ( strDgnCode == STLCODE_AIJ_ASD02 )
// 	{
// 		GetDlgItem(IDC_DGN_CTRLDATA_GB)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->ShowWindow(SW_HIDE);     
// 		GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->ShowWindow(SW_HIDE); // AIJ-ASD02�� �� �̰� �� ��. ĭ �����ϴ��� �׳� ������
// 
// 		// _T("Combined Ratio Method for Circular Section") �׷�ڽ��� _T("All Beams/Girders are ...") üũ �ڽ� ��ġ���� �̵� 
// 		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(rSrc);
// 		GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(rTgt);
// 
// 		int nDistY = rTgt.top - rSrc.top;
// 		CDlgUtil::CtrlMoveDistY(this, m_aSmspCtrl, nDistY);
// 
// 		// _T("Width/Thickness Limit") �׷�ڽ��� _T("Combined Ratio Method for Circular Section") �׷� �ڽ� ��ġ���� �̵� 
// 		GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(rSrc);
// 		GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(rTgt);
// 
// 		nDistY = rTgt.bottom - rSrc.top + globalUtils.ScaleByDPI(5);
// 		CDlgUtil::CtrlMoveDistY(this, aMoveCtrls, nDistY);
// 		
// 		GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(&RectOrg);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_DGN_CTRLDATA_GB)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->ShowWindow(SW_SHOW);    
// 	}
// #endif

	if (strDgnCode== STLCODE_KDS_41_31_2019 || strDgnCode==STLCODE_KDS_41_30_10_2022)
	{
		if(m_bSpecialEQ) GetDlgItem(IDC_DGN_STLCTRL_STRONG_COLM_WEAK_BEAM_CHK)->GetWindowRect(refR);
		else GetDlgItem(IDC_DGN_SMSP_GROUP)->GetWindowRect(refR);
		GetDlgItem(IDC_DGN_USE_UGLCOM4UGMEMB_CHK)->GetWindowRect(moveR);

		int nDistY = refR.bottom - moveR.top/* + 8*/;
		CDlgUtil::CtrlMoveDistY(this, m_aUGMemberCtrl, nDistY);

		GetDlgItem(IDC_DGN_USE_UGLCOM4UGMEMB_CHK)->GetWindowRect(&RectOrg);
	}
	
	if (strDgnCode == STLCODE_AIJ_ASD02)
	{
		GetDlgItem(IDC_DGN_DGNCODE_WTR_FRAME)->GetWindowRect(refR);
		GetDlgItem(IDC_WG_JUD_STATIC16)->GetWindowRect(moveR);

		int nDistY = refR.bottom - moveR.top + globalUtils.ScaleByDPI(5);
		CDlgUtil::CtrlMoveDistY(this, m_aAIJASD02Ctrl, nDistY);

		GetDlgItem(IDC_WG_JUD_STATIC17)->GetWindowRect(&RectOrg);
	}

	// Add by ParkBong.('06.05.18). Add bUpdateDef Option. 	
	if(bUpdateDef)  
	{
	  // Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50017-03, GBJ17-88).
	  m_pDgnDataCtrl->Get_DgnStlChinaDef(strDgnCode, m_iSafeLevel, m_iEqGrade);
	}
#endif

	BOOL bEnableDeflection = GetEnableDeflection(strDgnCode);
	GetDlgItem(IDC_DGN_STLCTRL_DEFLECT)->EnableWindow(bEnableDeflection);
	

	if(!bEnableDeflection) m_bCheckDeflect = FALSE;

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

	//
	OnCodeChange4CH(strDgnCode);
	UpdateData(FALSE);

	//SetRedraw(TRUE);
	// RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	RedrawWindow();
}

BOOL CDgnStlCodeDlg::GetEnableDeflection(const CString& strDgnCode)
{
    if ( strDgnCode == STLCODE_KDS_41_30_10_2022 || strDgnCode == STLCODE_KDS_41_31_2019 || 
        strDgnCode == STLCODE_KSSC_LSD16 || strDgnCode == STLCODE_KSSC_LSD09 || 
        strDgnCode == STLCODE_AISC_LRFD22 || strDgnCode == STLCODE_AISC_ASD22 ||
        strDgnCode == STLCODE_AISC_LRFD16 || strDgnCode == STLCODE_AISC_ASD16 ||
        strDgnCode == STLCODE_AISC_LRFD10 || strDgnCode == STLCODE_AISC_ASD10 ||
        strDgnCode == STLCODE_AISC_LRFD05 || strDgnCode == STLCODE_AISC_ASD05 ||    
        strDgnCode == STLCODE_NSCP_2015_LRFD || strDgnCode == STLCODE_NSCP_2015_ASD ||
        strDgnCode == STLCODE_EC3_05 || strDgnCode == STLCODE_EC3 || strDgnCode == STLCODE_EC3_2_05 || 
		strDgnCode == STLCODE_SP_16_13330_2017 || strDgnCode == STLCODE_IS800_2007|| strDgnCode == STLCODE_IRC_24_2010)
    {
        return TRUE;
    }
	else if(strDgnCode == STLCODE_TWN_ASD96 || strDgnCode == STLCODE_TWN_LSD96 )
	{
#if defined _US
		return TRUE;
#else 
		return FALSE;
#endif
	}
    else
    {
        return FALSE;
    }
}

void CDgnStlCodeDlg::SetUserData(T_DSTL_D* pDstlData)
{
	m_pDstlData = pDstlData;
}

void CDgnStlCodeDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	int Index = m_DesignCode.GetCurSel();
	if(m_aCodeEnable[Index]==FALSE)
	{
		CString strCodeName = _T("");
		m_DesignCode.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_DesignCode.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;
	SelchangeDgnCtrldataDesigncode(FALSE);
	CString strSelCode;
	m_DesignCode.GetLBText(m_DesignCode.GetCurSel(), strSelCode);
#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strSelCode);
#endif
	if (strSelCode == STLCODE_IS800_2007)
	{
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN_IS));
	}
	else
	{
	    GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->SetWindowText(_LS(IDS_DGN_TEXT_APPLY_SEISMIC_DGN));
	}
}

void CDgnStlCodeDlg::OnSelchangeDgnCtrldataNationalAnnex() 
{
	CString strCodeName = _T("");
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index, strCodeName);
	if(strCodeName != STLCODE_EC3_05) return;

	m_nInteractionKijType = [&]()
	{
		const auto nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignSubCode, m_DesignSubCode.GetCurSel());
		switch(nNationalAnnex)
			{
				case dgn::def::enNationalAnnex::Austria:		return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Belgium:		return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::CzechRepublic:	return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Denmark:		return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::France:			return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::Luxembourg:		return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::Netherlands:	return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Poland:			return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Romania:		return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::Slovakia:		return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Slovenia:		return EN_EC3_KIJ_ANNEX_B;
				case dgn::def::enNationalAnnex::Sweden:			return EN_EC3_KIJ_ANNEX_A;
				case dgn::def::enNationalAnnex::Sweden19:		return EN_EC3_KIJ_ANNEX_A;
				default:										return EN_EC3_KIJ_BY_CODE;
			}
	}();
	switch(m_nInteractionKijType)
	{
		case EN_EC3_KIJ_BY_CODE:
		{
			GetDlgItem(IDC_DGN_INTER_KIJ_CODE_RDO)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_A_RDO)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_B_RDO)->EnableWindow(TRUE);
			break;
		}
		case EN_EC3_KIJ_ANNEX_A:
		{
			GetDlgItem(IDC_DGN_INTER_KIJ_CODE_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_A_RDO)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_B_RDO)->EnableWindow(FALSE);
			break;
		}
		case EN_EC3_KIJ_ANNEX_B:
		{
			GetDlgItem(IDC_DGN_INTER_KIJ_CODE_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_A_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_INTER_KIJ_AN_B_RDO)->EnableWindow(TRUE);
			break;
		}
		default: ASSERT(0);
		break;
	}

	UpdateData(FALSE);
}

void CDgnStlCodeDlg::OnSelchangeDgnCtrldataDesignAnnexAashto()
{
	// 특별한 동작 없음.
}

CRect CDgnStlCodeDlg::MoveCtrl(int iKind, int* pID)
{
	CRect RectOrg;
	int iArSize=0;
    if ( iKind==2 )
    {
        iArSize = iDgn_StlID2;
    }
    else if ( iKind==3 )
    {
        iArSize = iDgn_StlID3;
    }
    else
    {
        ASSERT(0);
    }

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect0, Rect1;
	GetDlgItem(m_iID2[0])->GetWindowRect(&Rect0);
	for(int i=0; i<iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if(i==0)	Rect1 = pRectCng[i];
		pRectCng[i].top		 -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
	}
	RectOrg = pRectCng[0];
	for(int i=0; i<iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;

	return RectOrg;
}

void CDgnStlCodeDlg::InitStructureTypeCombo()
{
	int nIndex = 0;
	m_cmbStructureType.ResetContent();
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_MOMENT_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SMF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_INTERMEDIATE_MOMENT_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_IMF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_MOMENT_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OMF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_CONCENTRICALLY_BRACED_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SCBF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_CONCENTRICALLY_BRACED_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OCBF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ECCENTRICALLY_BRACED_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_EBF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_BUCKLING_RESTRAINED_BRACED_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_BRBF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_PLATE_SHEAR_WALLS));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SPSW);

	m_cmbStructureType.SetCurSel(m_nFrameType);

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStructureType);
}

void CDgnStlCodeDlg::InitStructureTypeComboIS()
{
	int nIndex = 0;
	m_cmbStructureType.ResetContent();
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_MOMENT_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OMF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_MOMENT_FRAMES_IS));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SMF);
	//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_INTERMEDIATE_MOMENT_FRAMES));
	//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_IMF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_CONCENTRICALLY_BRACED_FRAMES));
	m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SCBF);
	//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_CONCENTRICALLY_BRACED_FRAMES));
	//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OCBF);
	//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ECCENTRICALLY_BRACED_FRAMES));
	//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_EBF);
	//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_BUCKLING_RESTRAINED_BRACED_FRAMES));
	//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_BRBF);
	//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_PLATE_SHEAR_WALLS));
	//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SPSW);
	if(m_nFrameType==2){
	m_cmbStructureType.SetCurSel(0);
	}
	else if (m_nFrameType == 0) {
		m_cmbStructureType.SetCurSel(1);
	}
	else if (m_nFrameType == 3) {
		m_cmbStructureType.SetCurSel(2);
	}
	else {
		m_cmbStructureType.SetCurSel(0);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStructureType);
}

BOOL CDgnStlCodeDlg::IsAISCCodeSeries(const CString &strCodeName)
{
    if ( strCodeName == STLCODE_KSSC_LSD09 )     { return TRUE; }
    if ( strCodeName == STLCODE_KSSC_LSD16 )     { return TRUE; }
    if ( strCodeName == STLCODE_KDS_41_31_2019 ) { return TRUE; }
    if ( strCodeName == STLCODE_KDS_41_30_10_2022 ) { return TRUE; }
    if ( strCodeName == STLCODE_KDS_24_14_31_2018 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD05 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD05 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD10 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD10 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD16 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD16 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD22 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD22 )  { return TRUE; }
    if ( strCodeName == STLCODE_TWN_LSD96 )   { return TRUE; }
    if ( strCodeName == STLCODE_TWN_ASD96 )   { return TRUE; }
    if ( strCodeName == STLCODE_NSCP_2015_LRFD ) { return TRUE; }
    if ( strCodeName == STLCODE_NSCP_2015_ASD ) { return TRUE; }

	return FALSE;
}

BOOL CDgnStlCodeDlg::IsSpecialEQCode(const CString &strCodeName)
{
    if ( strCodeName == STLCODE_KSSC_LSD09 ) { return TRUE; }
    if ( strCodeName == STLCODE_KSSC_LSD16 ) { return TRUE; }
    if ( strCodeName == STLCODE_KDS_41_31_2019 ) { return TRUE; }
    if ( strCodeName == STLCODE_KDS_41_30_10_2022 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD05 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD05 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD10 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD10 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD16 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD16 )  { return TRUE; }
    if ( strCodeName == STLCODE_AISC_LRFD22 ) { return TRUE; }
    if ( strCodeName == STLCODE_AISC_ASD22 )  { return TRUE; }
    if ( strCodeName == STLCODE_TWN_LSD96 ) { return TRUE; }
    if ( strCodeName == STLCODE_TWN_ASD96 ) { return TRUE; }
    if ( strCodeName == STLCODE_GBJ17_88 )   { return TRUE; }
    if ( strCodeName == STLCODE_GB50017_03 ) { return TRUE; }
    if ( strCodeName == STLCODE_GB50017_15 ) { return TRUE; }//add by maxiao(2015-9-25)GB50017-15
    if ( strCodeName == STLCODE_GB50017_17 ) { return TRUE; }//add by xuezc(2018/2/8)
    if ( strCodeName == STLCODE_GB51249_2017 ) { return TRUE; }//add by tss(2019/12/8)
	if (strCodeName == STLCODE_JGJ209_2010) { return TRUE;  } // add by tss(2021/07/15)
    if ( strCodeName == STLCODE_NSCP_2015_LRFD ) { return TRUE; }
    if ( strCodeName == STLCODE_NSCP_2015_ASD ) { return TRUE; }
	if ( strCodeName == STLCODE_EC3_05 ) { return TRUE; }
	if ( strCodeName == STLCODE_EC3_2_05 ) { return TRUE; }
	if (strCodeName == STLCODE_IS800_2007) { return TRUE; }
	if (strCodeName == STLCODE_SP_16_13330_2017) { return TRUE; } // add by minenko(2024/10/16)
	
	return FALSE;
}

void CDgnStlCodeDlg::OnDgnStlctrlStifRib() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if(m_bStifRib)
	{
		GetDlgItem(IDC_DGN_LATE_STIFRIB_SPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LATE_STIFRIB_SPACE_EDT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_LATE_STIFRIB_SPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LATE_STIFRIB_SPACE_EDT)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnStlCodeDlg::OnBtnGrup()
{
	// TODO: Add your control notification handler code here

	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP, 0));
}

void CDgnStlCodeDlg::OnSetGrupCombo()
{
	SetGrupComboBox(m_cmbNonSeisMemb);

	T_GRUP_D Data;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(m_GroupKey, Data))
	{
		m_cmbNonSeisMemb.SelectString(0, Data.GroupName);
	}
}