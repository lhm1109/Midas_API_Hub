// DgnConDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConDlg.h"

// �ο��� : �߰��� ��ĥ�� 1.
#include "DgnConBeamDlg.h"
#include "DgnConColmDlg.h"
#include "DgnConBraceDlg.h"
#include "DgnConWallDlg.h"
#include "DgnConBeamEqualDlg.h"

#include "DgnConWallMarkDlg.h"
#include "DgnConCvlCtcDlg.h"
#include "DgnConCvlRSModFactorDlg.h"
#include "DgnConBeamDlgCIVIL.h"
#include "DgnConColmDlgCIVIL.h"
#include "DgnConBeamDgnDlgCIVIL.h"
#include "DgnConColmDgnDlgCIVIL.h"
#include "DgnConPSASDlg.h"
#include "DgnConJSFTDlg.h"
#include "DgnConMRFTDlg.h"
#include "DgnConMCMBDlg.h"
#include "DgnConDFBADlg.h"
#include "DgnConTRFTDlg.h"
#include "DgnConUCCFDlg.h"
#include "DgnConExposureCondiDlg.h"
#include "DgnConPmdmDlg.h"
#include "DgnConScolDlg.h"
#include "DgnConReinfoShearDlg.h"
#include "DgnConServiceLimtDlg.h"
#include "DgnConSPVAREMADlg.h"
#include "DgnConBarSectDlg.h"
#include "DgnConBndrElemWallIDDlg.h"
#include "DgnConMndcDlg.h"
#include "DgnConSafmDlg.h"
#include "DgnConRrtrDlg.h"
#include "DgnConEBarDesignMethodWallIDDlg.h"
#include "ExposureClassDlg_RC.h"

//(2009.07.10) Add by Unsang :: SPEC���濡 ���� �ش� �Է�â�� EC2-2:05, Colm_Dgn�� �������� ����.
//(2009.06.10) Add by Unsang :: Only EC2-2:05
//#include "DgnConCvlUlsParamDlg.h"
#include "DgnConStrutAngleDlg.h"
#include "DgnGenMembPlateDlg.h"
#include "DgnConCondiFactorDlg.h"

#include "DgnDataCtrl.h"

#include "RCDesignOption.h"
#include "RCPrintOption.h"
#include "RCCrackWidth.h"
#include "RCLengKFactorDlg.h"
#include "RCDesignSeismic.h"

#include "DgnRCPlateBeamChkingDlg.h"
#include "DgnRCPlateColChkingDlg.h"
#include "DgnRCPlateBeamDesignDlg.h"
#include "DgnRCPlateColDesignDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\mit_frx\MBarBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnConDlg

IMPLEMENT_DYNCREATE(CDgnConDlg, MChildFormView)


CDgnConDlg::CDgnConDlg()
	: MChildFormView(CDgnConDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnConDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnConDlg::~CDgnConDlg()
{
	DeleteChildDlgs();
}

void CDgnConDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnConDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg diagnostics

#ifdef _DEBUG
void CDgnConDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnConDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg message handlers

bool CDgnConDlg::ShowApplyBtn()
{
    if ( m_nCurChildDlg < 0 || m_MenuItems.GetSize() - 1 < m_nCurChildDlg )
    {
        return true;
    }

    switch ( m_MenuItems[m_nCurChildDlg].nDlgID )
    {
    case __TMMODE_DGN_CON_PBDC__:
    case __TMMODE_DGN_CON_PBDD__:
    case __TMMODE_DGN_CON_PCDC__:
    case __TMMODE_DGN_CON_PCDD__:
        return false;
    default:
        return true;
    }
}

void CDgnConDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	// Clear existing menu items
	ClearMenuItems();

	CDBDoc* pDoc;
	pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	T_DCON_D dconData; dconData.Initialize();
	DataCtrl.Get_DgnConDcon(dconData);

    const CString& strConCode = dconData.DesignCode;

    auto L_IsCodeCH =[strConCode] () -> bool
    {
        if ( strConCode == CONCODE_JTJ023_85 ) { return true; }
        if ( strConCode == CONCODE_GB50010_02 ) { return true; }
        if ( strConCode == CONCODE_JTG_D62_04 ) { return true; }
        if ( strConCode == CONCODE_TB10002_3_05 ) { return true; }
        if ( strConCode == CONCODE_CJJ11_2011 ) { return true; }

        return false;
    };

    bool bCodeCH = L_IsCodeCH();
	
#if defined(_CIVIL)
	// Change by Jaeoh ('05.07.15) if define(_ORG) || define(_CH) ���� 
	//m_ChildDlgs.Add(new CDgnConCvlCtcDlg);
	//m_DlgIDs.Add(__TMMODE_DGN_CON_CVL_SUFC__);
	//m_TableIDs.Add(ID_QUERY_DESIGN_CONC_CVL_CTC_FACTOR);
	//++++++++add by maxiao 2006.12.25

    switch ( CProduct::GetMovingType() )
    {
    case D_PRODUCT_MOVING_CH:
        CreateChildDlgs_CH(strConCode, bCodeCH);
        break;
    default:
        CreateChildDlgs_Else(strConCode, bCodeCH);
        break;
    }
	if (strConCode != CONCODE_AREMA_2023) {
		AddMenuItem(new CDgnConMndcDlg, __TMMODE_DGN_CON_MNDC__, ID_QUERY_DGN_CONC_NONLINEAR_DEFORMATION, CString(_LS(IDS_TREE_TEXT_CON_MNDC)));
		AddMenuItem(new CDgnConSafmDlg, __TMMODE_DGN_CON_SAFM__, ID_QUERY_DGN_CONC_SEISMIC_ACCOUNTINT, CString(_LS(IDS_TREE_TEXT_CON_SAFM)));
	}
	/**
	m_ChildDlgs.Add(new CDgnConBeamDlgCIVIL);
	m_DlgIDs.Add(__TMMODE_DGN_CON_REBB__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnConColmDlgCIVIL);
	m_DlgIDs.Add(__TMMODE_DGN_CON_REBC__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnConBeamDgnDlgCIVIL);
	m_DlgIDs.Add(__TMMODE_DGN_CON_CVL_RDGN_BEAM__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnConColmDgnDlgCIVIL);
	m_DlgIDs.Add(__TMMODE_DGN_CON_CVL_RDGN_COLU__);
	m_TableIDs.Add(0);
	**/

#else//Gen
	// Coded by sshan MNet:No.2484 ('20061109)
	AddMenuItem(new CDgnConBarSectDlg, __TMMODE_DGN_CON_ELEM_BAR__, ID_QUERY_DGN_CON_DCBALL, CString(_LS(IDS_TREE_TEXT_CON_MOD_ELEM_BAR)));
	AddMenuItem(new CDgnConBeamEqualDlg, __TMMODE_DGN_CON_BEAM_BAR__, 0, CString(_LS(IDS_TREE_TEXT_CON_EQUAL_BEAM)));
	AddMenuItem(new CDgnConMRFTDlg, __TMMODE_DGN_CON_MRFT__, ID_QUERY_DGN_CON_MRFT, CString(_LS(IDS_TREE_TEXT_CON_MOD_MRFT)));
	AddMenuItem(new CDgnConTRFTDlg, __TMMODE_DGN_CON_TRFT__, ID_QUERY_DGN_CON_TRFT, CString(_LS(IDS_TREE_TEXT_CON_MOD_TRFT)));
	AddMenuItem(new CDgnConServiceLimtDlg, __TMMODE_DGN_CON_SERV__, ID_QUERY_DGN_CON_SERV, CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
#if defined (_IGEN) || defined (_DEBUG)
	AddMenuItem(new CDgnConReinfoShearDlg, __TMMODE_DGN_CON_REIN_SHEAR__, ID_QUERY_DGN_CON_RESS_SLAB, CString(_LS(IDS_TREE_TEXT_CON_MOD_SHEAR_SLAB)));
#endif
	AddMenuItem(new CDgnConServiceLimtDlg(TRUE), __TMMODE_DGN_CON_SERV_SLAB__, ID_QUERY_DGN_CON_SERV_SLAB, CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV_SLAB)));

#ifndef _IGEN
	AddMenuItem(new CDgnConUCCFDlg, __TMMODE_DGN_CON_UCCF__, ID_QUERY_DGN_CON_UCCF, CString(_LS(IDS_TREE_TEXT_CON_MOD_UCCF)));
#endif

	AddMenuItem(new CDgnConStrutAngleDlg, __TMMODE_DGN_CON_STAN__, ID_QUERY_DGN_CON_STAN, CString(_LS(IDS_TREE_TEXT_CON_MOD_STAN)));
	AddMenuItem(new CDgnConMCMBDlg, __TMMODE_DGN_CON_MCMB__, ID_QUERY_DGN_CON_MCMB, CString(_LS(IDS_TREE_TEXT_CON_MOD_MCMB)));
	AddMenuItem(new CDgnConDFBADlg, __TMMODE_DGN_CON_DFBA__, ID_QUERY_DGN_CON_DFBA, CString(_LS(IDS_TREE_TEXT_CON_MOD_DFBA)));
	AddMenuItem(new CDgnConExposureCondiDlg, __TMMODE_DGN_CON_REXC__, ID_QUERY_DGN_CON_REXC, CString(_LS(IDS_TREE_TEXT_CON_MOD_REXC)));
	AddMenuItem(new CDgnConPmdmDlg, __TMMODE_DGN_CON_PMDM__, ID_QUERY_DGN_CON_PMDM, CString(_LS(IDS_MAIN_RIBBON_MENU_PMCM)));
	AddMenuItem(new CDgnConScolDlg, __TMMODE_DGN_CON_SCOL__, ID_QUERY_DGN_CON_SCOL, CString(_LS(IDS_MAIN_RIBBON_MENU_SCOL)));

#if defined(_ORG) && defined(_MGEN)
	//Add Code By RSH 2002.12.26
	AddMenuItem(new CDgnConJSFTDlg, __TMMODE_DGN_CON_JSFT__, ID_QUERY_DGN_CON_JSFT, CString(_LS(IDS_TREE_TEXT_CON_MOD_JSFT)));
	//Add Code By RSH 2002.12.02
	AddMenuItem(new CDgnConPSASDlg, __TMMODE_DGN_CON_PSAS__, ID_QUERY_DGN_CON_PSAS, CString(_LS(IDS_TREE_TEXT_CON_MOD_PSAS)));
#endif

	AddMenuItem(new CDgnConWallMarkDlg, __TMMODE_DGN_CON_WMAK__, 0, CString(_LS(IDS_TREE_TEXT_CON_MOD_WALL_MARK)));
	AddMenuItem(new CDgnConBndrElemWallIDDlg, __TMMODE_DGN_CON_BEMW__, ID_QUERY_DGN_CON_BEMW, CString(_LS(IDS_TREE_TEXT_CON_BEM_WALL_ID)));

    if(strConCode.CompareNoCase(CONCODE_SP_63_13330_2018) == 0)
    {
        AddMenuItem(new CDgnConMndcDlg, __TMMODE_DGN_CON_MNDC__, ID_QUERY_DGN_CONC_NONLINEAR_DEFORMATION, CString(_LS(IDS_TREE_TEXT_CON_MNDC)));
        AddMenuItem(new CDgnConSafmDlg, __TMMODE_DGN_CON_SAFM__, ID_QUERY_DGN_CONC_SEISMIC_ACCOUNTINT, CString(_LS(IDS_TREE_TEXT_CON_SAFM)));
        AddMenuItem(new CDgnConRrtrDlg, __TMMODE_DGN_CON_RRTR__, ID_QUERY_DGN_CONC_TRANSVERSE_REINFORCEMENT, CString(_LS(IDS_TREE_TEXT_CON_RRTR)));
    }

    AddMenuItem(new CDgnConEBarDesignMethodWallIDDlg, __TMMODE_DGN_CON_EBMW__, ID_QUERY_DGN_CON_EBMW, CString(_LS(IDS_TREE_TEXT_CON_EBM_WALL_ID)));

#endif
}

void CDgnConDlg::CreateChildDlgs_CH(CString strConCode, bool bCodeCH)
{
    CDBDoc* pDoc;
    pDoc = CDBDoc::GetDocPoint();

    T_CHRP_D ChrcD; ChrcD.Initialize();
    pDoc->m_pAttrCtrl->GetChrp(ChrcD);
    if ( ChrcD.iDgnCode == RC_JTG_D62_04 || ChrcD.iDgnCode == RC_CJJ_11_2011 ||ChrcD.iDgnCode == RC_TB_10002_3_05 || ChrcD.iDgnCode == RC_JTG_B02_01_2008|| ChrcD.iDgnCode == RC_CJJ_166_2011 )
    {
        bCodeCH = true;
    }
    //+++++++++++

    if ( bCodeCH )
    {
        if ( ChrcD.iDgnCode == RC_JTG_B02_01_2008||ChrcD.iDgnCode == RC_CJJ_166_2011 )
        {
            AddMenuItem(new CRCDesignSeismic, __TMMODE_DGN_CON_RSDM__, ID_QUERY_RC_DESIGN_SEISMIC, CString(_LS(IDS_DGN_TREE_TEXT_RC_RSDM)));
        }
        else
        {
            AddMenuItem(new CRCDesignOption, __TMMODE_DGN_CON_DORC__, ID_QUERY_RC_DESIGN_OPTION, CString(_LS(IDS_DGN_TREE_TEXT_RC_DORC)));
            AddMenuItem(new CRCPrintOption, __TMMODE_DGN_CON_PORC__, ID_QUERY_RC_PRINT_OPTION, CString(_LS(IDS_DGN_TREE_TEXT_RC_PORC)));
            AddMenuItem(new CRCCrackWidth, __TMMODE_DGN_CON_CWRC__, ID_QUERY_RC_CRACK_WIDTH, CString(_LS(IDS_DGN_TREE_TEXT_RC_CWRC)));
        }
    }
    else
    {
        AddMenuItem(new CDgnConCvlCtcDlg, __TMMODE_DGN_CON_CVL_SUFC__, ID_QUERY_DESIGN_CONC_CVL_CTC_FACTOR, CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));

		if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
			strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
			strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
			strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
		{
			AddMenuItem(new CDgnConCvlRSModFactorDlg, __TMMODE_DGN_CON_CVL_RMFR__, ID_QUERY_DESIGN_CONC_CVL_RS_MOD_FACTOR, CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));
		}
    }
}

void CDgnConDlg::CreateChildDlgs_Else(CString strConCode, bool bCodeCH)
{
    if ( strConCode != CONCODE_KSCE_USD10 && strConCode != CONCODE_KSCE_RAIL_USD11 &&
        strConCode != CONCODE_KDS_24_14_21_2022 && strConCode != CONCODE_KSCE_LSD15 &&
        strConCode != CONCODE_KCI_USD12 && strConCode != CONCODE_KDS_14_20_00_2022 &&
        strConCode != CONCODE_SNIP_2_05_03_84 && strConCode != CONCODE_SP_35_13330_2011 &&
        strConCode != CONCODE_SNIP_2_05_03_84_MKS && strConCode != CONCODE_SP_35_13330_2011_MKS )
    {
        AddMenuItem(new CDgnConCvlCtcDlg, __TMMODE_DGN_CON_CVL_SUFC__, ID_QUERY_DESIGN_CONC_CVL_CTC_FACTOR, CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));
    }


	if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
		strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
		strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
		strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
	{
		AddMenuItem(new CDgnConCvlRSModFactorDlg, __TMMODE_DGN_CON_CVL_RMFR__, ID_QUERY_DESIGN_CONC_CVL_RS_MOD_FACTOR, CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));
	}

    //(2009.07.10) Add by Unsang :: SPEC���濡 ���� �ش� �Է�â�� EC2-2:05, Colm_Dgn�� �������� ����.
    /*
    //(2009.06.10) Add by Unsang :: Only EC2-2:05!
    m_ChildDlgs.Add(new CDgnConCvlUlsParamDlg); // wg_dgn�� �߰�
    m_DlgIDs.Add(__TMMODE_DGN_CON_CVL_ULSP__);
    m_TableIDs.Add(ID_QUERY_DESIGN_CONC_CVL_ULS_FACTOR); // wg_main Menu ID��.
    */

#if defined (_IGEN) || defined (_DEBUG)
	if (strConCode == CONCODE_AIJ_WSD99 )
	{
		AddMenuItem(new CDgnConReinfoShearDlg, __TMMODE_DGN_CON_REIN_SHEAR__, ID_QUERY_DGN_CON_SERV, CString(_LS(IDS_TREE_TEXT_CON_REIN_SHEAR)));
	}
#endif

    if ( strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_IRS ||
        strConCode == CONCODE_IRC112_2020 || strConCode == CONCODE_BS5400_90||strConCode==CONCODE_AREMA_2023 ||
        strConCode == CONCODE_TMH07_89)
    {
        AddMenuItem(new CDgnConServiceLimtDlg, __TMMODE_DGN_CON_SERV__, ID_QUERY_DGN_CON_SERV, CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    }

    if (strConCode == CONCODE_AREMA_2023) {
        AddMenuItem(new CDgnConSPVAREMADlg, __TMMODE_DGN_CON_SPVAREMA__, ID_QUERY_DGN_CON_SPVAREMA, CString(_LS(IDS_TREE_TEXT_CON_MOD_SPVAREMA)));
    }

    if ( strConCode == CONCODE_KSCE_USD10 || strConCode == CONCODE_KSCE_RAIL_USD11 ||
        strConCode == CONCODE_KDS_24_14_21_2022 || strConCode == CONCODE_KSCE_LSD15 ||
        strConCode == CONCODE_KCI_USD12 || strConCode == CONCODE_KDS_14_20_00_2022)
    {
        AddMenuItem(new CDgnGenMembPlateDlg, __TMMODE_DGN_GEN_MEMB_PLATE__, 0, CString(_LS(IDS_TREE_TEXT_GEN_MEMBER_PLATE)));
    }

    if ( strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS )
    {
        AddMenuItem(new CDgnConCondiFactorDlg, __TMMODE_DGN_CON_OCDF__, ID_QUERY_DGN_CON_OCDF, CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
        // for russia plate
        AddMenuItem(new CDgnRCPlateBeamChkingDlg, __TMMODE_DGN_CON_PBDC__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        AddMenuItem(new CDgnRCPlateColChkingDlg, __TMMODE_DGN_CON_PCDC__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }

    if (strConCode == CONCODE_AS5100_5_17)
    {
        AddMenuItem(new CExposureClassDlg_RC, __TMMODE_DGN_CON_EXPOSURE_CLASS__, ID_QUERY_RC_EXPOSURE_CLASS, CString(_LS(IDS_TREE_TEXT_RC_EXPOSURE_CLASS)));
    }

    if ( strConCode == CONCODE_AASHTO_LRFD12 || strConCode == CONCODE_AASHTO_LRFD14 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 || 
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 ||
        strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011        ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_IRC112_2020 || strConCode == CONCODE_BS5400_90 ||
        strConCode == CONCODE_TMH07_89 ||  strConCode==CONCODE_AREMA_2023 ||
        strConCode == CONCODE_AS5100_5_17)
    {
        AddMenuItem(new CDgnRCPlateBeamDesignDlg, __TMMODE_DGN_CON_PBDD__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_DESIGN)));
        AddMenuItem(new CDgnRCPlateBeamChkingDlg, __TMMODE_DGN_CON_PBDC__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        AddMenuItem(new CDgnRCPlateColDesignDlg, __TMMODE_DGN_CON_PCDD__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_DESIGN)));
        AddMenuItem(new CDgnRCPlateColChkingDlg, __TMMODE_DGN_CON_PCDC__, 0, CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
}


void CDgnConDlg::AddMenuItem(CMenuBarChildDlg* pDlg, int nDlgID, UINT nTableID, const CString& strMenuText)
{
	DgnConMenuItem item(pDlg, nDlgID, nTableID, strMenuText);
	m_MenuItems.Add(item);
}

void CDgnConDlg::ClearMenuItems()
{
	for (int i = 0; i < m_MenuItems.GetSize(); i++)
	{
		if (m_MenuItems[i].pChildDlg != NULL)
		{
			delete m_MenuItems[i].pChildDlg;
			m_MenuItems[i].pChildDlg = NULL;
		}
	}
	m_MenuItems.RemoveAll();
}

void CDgnConDlg::DeleteChildDlgs()
{
	ClearMenuItems();
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnConDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_CON__)  // �ο��� : �� ���� Copy�� ���� ����
	{
		ShowCurChildDlg(0,0);
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}
	if(	nID == __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM ||
			nID == __TMMODE_DGN_CON_ELEM_BAR__SUB_COL ||
			nID == __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC ||
			nID == __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL)
	{
		int nIDCount = m_MenuItems.GetSize();
		for(int i=0; i<nIDCount; i++)
		{
			if(__TMMODE_DGN_CON_ELEM_BAR__==m_MenuItems[i].nDlgID)
			{
				ShowCurChildDlg(i,nID);
				m_ComboMenu.SetCurSel(i);

				BOOL bEnable = FALSE;

				if(m_MenuItems[i].nTableID > 0)	bEnable = TRUE;
				else bEnable = FALSE;

				if(m_MenuItems[i].nTableID==ID_QUERY_DGN_CON_BEMW)
				{
					bEnable = IsEnbleForBemw(m_MenuItems[i].nTableID);
				}

				m_wndTableBtn.EnableWindow(bEnable);
				return TRUE;
			}
		}
	}
	else
	{
		int nIDCount = m_MenuItems.GetSize();
		for(int i=0; i<nIDCount; i++)
		{
			if(nID==m_MenuItems[i].nDlgID)
			{
				ShowCurChildDlg(i,0);
				m_ComboMenu.SetCurSel(i);

				BOOL bEnable = FALSE;

				if(m_MenuItems[i].nTableID > 0)	bEnable = TRUE;
				else bEnable = FALSE;

				if(m_MenuItems[i].nTableID==ID_QUERY_DGN_CON_BEMW)
				{
					bEnable = IsEnbleForBemw(m_MenuItems[i].nTableID);
				}

				m_wndTableBtn.EnableWindow(bEnable);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CDgnConDlg::LayoutChildDialog()
{
	if (m_nCurChildDlg==0) return;
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;

	GetClientRect(&ParentRect);
	m_MenuItems[m_nCurChildDlg].pChildDlg->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);
	
	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;
	
	Wx = DlgRect.Width();
	Wy = DlgRect.Height();
	
	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);
	
	ScreenToClient(&PHRect);
	
	/////////////////////////////////////////////////////////////
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
	
	ScrSize += TSize;
	
	m_RectPlaceHolder = PHRect;
	
	SetScrollSizes(MM_TEXT,ScrSize);
}

void CDgnConDlg::ShowCurChildDlg(int nDlgIndex, int nSubIndex)
{
	if(m_nCurChildDlg==nDlgIndex)
	{
		if(	nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM || nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_COL ||
			nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC || nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL)
		{
			CDgnConBarSectDlg* pDlg = (CDgnConBarSectDlg*)m_MenuItems[nDlgIndex].pChildDlg;
			if(pDlg != NULL)	pDlg->ChangeCurrentTab( abs(__TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM-nSubIndex) );
		}
		return;
	}

	if(nDlgIndex > m_MenuItems.GetSize()-1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if(m_nCurChildDlg >= 0 && m_MenuItems.GetSize()-1 >= m_nCurChildDlg)
		m_MenuItems[m_nCurChildDlg].pChildDlg->End();

	m_nCurChildDlg = nDlgIndex;
	m_MenuItems[nDlgIndex].pChildDlg->CreateInit(&m_wndPlaceHolder,0,0);


	if(	nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM || nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_COL ||
			nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC || nSubIndex == __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL)
	{
		CDgnConBarSectDlg* pDlg = (CDgnConBarSectDlg*)m_MenuItems[nDlgIndex].pChildDlg;
		if(pDlg != NULL)	pDlg->ChangeCurrentTab( abs(__TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM-nSubIndex) );
	}

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;

	GetClientRect(&ParentRect);
	m_MenuItems[nDlgIndex].pChildDlg->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	/////////////////////////////////////////////////////////////
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

    // ����ó��. Apply�� ���ܾ��ϴ� ���
	// Child Dlg �� �������� ���
    static_cast<mit::frx::MBarBaseDlg*>(GetParent())->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

int CDgnConDlg::GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

void CDgnConDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnConDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class

	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(bActivate)
	{
		if(m_nCurChildDlg != -1)
		{
			ShowCurChildDlg(m_nCurChildDlg,0);
			CTreeMenuBarBase::SetModeOnlyST(m_MenuItems[m_nCurChildDlg].nDlgID);
		}
		else
		{
			ShowCurChildDlg(0,0);
			CTreeMenuBarBase::SetModeOnlyST(m_MenuItems[0].nDlgID);
		}
	}
}

void CDgnConDlg::OnComboMenuSelect()
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_MenuItems[nCurI].nDlgID);
	//ShowCurChildDlg(nCurI);
}

int CDgnConDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnConDlg::OnDgnTableBtn()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_MenuItems[nCurSel].nTableID <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_MenuItems[nCurSel].nTableID, 0));
}

// Change by ZINU.('01.8.1). To change Dynamic Combo Menu.
void CDgnConDlg::Update_ComboMenu()
{
	// Change by Jaeoh ('05.01.26)
	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();

	// Use the new unified structure - menu texts are already in m_MenuItems
	// No need to rebuild menu strings separately - they're stored in each DgnConMenuItem

	int nMenuCount = m_MenuItems.GetSize();
	if (nMenuCount == 0)
	{
		// No menu items yet, keep old selection
		if (iSelID < 0) m_ComboMenu.SetCurSel(0);
		else m_ComboMenu.SetCurSel(iSelID);
		return;
	}

	// Calculate combo box height
	int wHeight = (nMenuCount + 1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	// Add all menu items from the unified structure
	CClientDC dc(this);
	CSize sizeMax(0, 0), sizeCur;

	for (int i = 0; i < nMenuCount; i++)
	{
		const CString& strMenuText = m_MenuItems[i].strMenuText;
		m_ComboMenu.AddString(strMenuText);

		// Calculate max width
		sizeCur = dc.GetTextExtent(strMenuText);
		if (sizeCur.cx > sizeMax.cx) sizeMax.cx = sizeCur.cx;
	}

	// Set dropdown width
	if (sizeMax.cx > m_ComboMenu.GetDroppedWidth())
		m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	// Restore selection
	if (iSelID < 0 || iSelID >= nMenuCount) m_ComboMenu.SetCurSel(0);
	else m_ComboMenu.SetCurSel(iSelID);
}

void CDgnConDlg::Update_ComboMenu_ORG(CString strConCode, CArray<CString, CString&> & m_ComboMenuStr, bool bCodeCH)
{
    if ( strConCode != CONCODE_KSCE_USD10 && strConCode != CONCODE_KSCE_RAIL_USD11 &&
        strConCode != CONCODE_KDS_24_14_21_2022 && strConCode != CONCODE_KSCE_LSD15 && 
        strConCode != CONCODE_KCI_USD12 && strConCode != CONCODE_KDS_14_20_00_2022 &&
        strConCode != CONCODE_SNIP_2_05_03_84 && strConCode != CONCODE_SP_35_13330_2011 &&
        strConCode != CONCODE_SNIP_2_05_03_84_MKS && strConCode != CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));
    }

    if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
        strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));        
    }

    //m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR)));
    if ( strConCode == CONCODE_EC2_2_05 )                   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84 )       m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_IRC112_2011 )			m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IRC112_2020 )			m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IS456_2000 )			m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IRS )					m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_BS5400_90 )				m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_TMH07_89 )				m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if (strConCode == CONCODE_AREMA_2023) {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SPVAREMA)));
    };
    

    

    if ( strConCode == CONCODE_SNIP_2_05_03_84 )            m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));

    if ( strConCode == CONCODE_KSCE_USD10 || strConCode == CONCODE_KSCE_RAIL_USD11 ||
        strConCode == CONCODE_KDS_24_14_21_2022 || strConCode == CONCODE_KSCE_LSD15 || 
        strConCode == CONCODE_KCI_USD12 || strConCode == CONCODE_KDS_14_20_00_2022)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_GEN_MEMBER_PLATE)));
    }

    if ( strConCode == CONCODE_AASHTO_LRFD12 || strConCode == CONCODE_AASHTO_LRFD14 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 || 
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 ||
        strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011 ||  strConCode == CONCODE_AREMA_2023 ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_BS5400_90 || strConCode == CONCODE_TMH07_89)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
    // for russia plate
    if ( strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
}

void CDgnConDlg::Update_ComboMenu_CH(CString strConCode, CArray<CString, CString&> & m_ComboMenuStr, bool bCodeCH)
{
    CDBDoc* pDoc;
    pDoc = CDBDoc::GetDocPoint();
    //+++++++++++++++add by maxiao 2006.12.25
    T_CHRP_D ChrcD; ChrcD.Initialize();
    pDoc->m_pAttrCtrl->GetChrp(ChrcD);
    if ( ChrcD.iDgnCode == RC_JTG_D62_04 || ChrcD.iDgnCode == RC_CJJ_11_2011 ||ChrcD.iDgnCode == RC_TB_10002_3_05 || ChrcD.iDgnCode == RC_JTG_B02_01_2008||ChrcD.iDgnCode == RC_CJJ_166_2011 ) {
        bCodeCH = true;
    }
    //+++++++++++++++++++++++++++++++++++++++

    if ( !bCodeCH )
    {
        if ( strConCode != CONCODE_SNIP_2_05_03_84 && strConCode != CONCODE_SP_35_13330_2011 &&
            strConCode != CONCODE_SNIP_2_05_03_84_MKS && strConCode != CONCODE_SP_35_13330_2011_MKS )
        {
            m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));
        }

		if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
			strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
			strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
			strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
		{
			m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));
		}

        //m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR)));  
        if ( strConCode == CONCODE_EC2_2_05 )   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
        else if ( strConCode == CONCODE_SNIP_2_05_03_84 ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
        else if ( strConCode == CONCODE_SP_35_13330_2011 ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
        else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
        else if ( strConCode == CONCODE_SP_35_13330_2011_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));

        if ( strConCode == CONCODE_SNIP_2_05_03_84 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
        else if ( strConCode == CONCODE_SP_35_13330_2011 )    m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
        else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
        else if ( strConCode == CONCODE_SP_35_13330_2011_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));

    }
    if ( bCodeCH )
    {
        if ( ChrcD.iDgnCode == RC_JTG_B02_01_2008||ChrcD.iDgnCode == RC_CJJ_166_2011 )
        {
            m_ComboMenuStr.Add(CString(_LS(IDS_DGN_TREE_TEXT_RC_RSDM)));// SHIN ���ؿ� ���� ������ �� �ֵ���
        }
        else
        {
            m_ComboMenuStr.Add(CString(_LS(IDS_DGN_TREE_TEXT_RC_DORC)));
            m_ComboMenuStr.Add(CString(_LS(IDS_DGN_TREE_TEXT_RC_PORC)));
            m_ComboMenuStr.Add(CString(_LS(IDS_DGN_TREE_TEXT_RC_CWRC)));
        }
    }
    if ( strConCode == CONCODE_AASHTO_LRFD12 || strConCode == CONCODE_AASHTO_LRFD14 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 ||
        strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011        ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_IRC112_2020)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
    // for russia plate
    if ( strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
}

void CDgnConDlg::Update_ComboMenu_JP(CString strConCode, CArray<CString, CString&> & m_ComboMenuStr, bool bCodeCH)
{
    if ( strConCode != CONCODE_KSCE_USD10 && strConCode != CONCODE_KSCE_RAIL_USD11 &&
        strConCode != CONCODE_KDS_24_14_21_2022 && strConCode != CONCODE_KSCE_LSD15 && 
        strConCode != CONCODE_KCI_USD12 && strConCode != CONCODE_KDS_14_20_00_2022 &&
        strConCode != CONCODE_SNIP_2_05_03_84 && strConCode != CONCODE_SP_35_13330_2011 &&
        strConCode != CONCODE_SNIP_2_05_03_84_MKS && strConCode != CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));
    }

	if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
		strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
		strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
		strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
	{
		m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));
	}

    //m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR)));
    if ( strConCode == CONCODE_EC2_2_05 )        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )     m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));

    if ( strConCode == CONCODE_SNIP_2_05_03_84 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )     m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS ) m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));

    if ( strConCode == CONCODE_KSCE_USD10 || strConCode == CONCODE_KSCE_RAIL_USD11 ||
        strConCode == CONCODE_KDS_24_14_21_2022 || strConCode == CONCODE_KSCE_LSD15 || 
        strConCode == CONCODE_KCI_USD12 || strConCode == CONCODE_KDS_14_20_00_2022)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_GEN_MEMBER_PLATE)));
    }

    if ( strConCode == CONCODE_AASHTO_LRFD12 || strConCode == CONCODE_AASHTO_LRFD14 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 || 
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 ||
        strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011        ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_IRC112_2020)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
    // for russia plate
    if ( strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
}

void CDgnConDlg::Update_ComboMenu_Else(CString strConCode, CArray<CString, CString&> & m_ComboMenuStr, bool bCodeCH)
{
    if ( strConCode != CONCODE_SNIP_2_05_03_84 && strConCode != CONCODE_SP_35_13330_2011 &&
        strConCode != CONCODE_SNIP_2_05_03_84_MKS && strConCode != CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR)));
    }

	if (strConCode == CONCODE_AASHTO_LFD96 || strConCode == CONCODE_AASHTO_LRFD02 ||
		strConCode == CONCODE_AASHTO_LRFD07 || strConCode == CONCODE_AASHTO_LRFD12 ||
		strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 ||
		strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24)
	{
		m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_RS_MOD_FACTOR)));
	}

    //m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR))); 
    if (strConCode == CONCODE_EC2_2_05)                     m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if (strConCode == CONCODE_AREMA_2023) 
    { 
    m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV))); 
    m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SPVAREMA)));
    }
    else if ( strConCode == CONCODE_SNIP_2_05_03_84 )       m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_2ND_GROUP)));
    else if ( strConCode == CONCODE_IRC112_2011 )           m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IRC112_2020 )           m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IS456_2000 )            m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_IRS )				    m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_BS5400_90 )				m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));
    else if ( strConCode == CONCODE_TMH07_89 )				m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_MOD_SERV)));

    if ( strConCode == CONCODE_SNIP_2_05_03_84 )            m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011 )      m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SNIP_2_05_03_84_MKS )   m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));
    else if ( strConCode == CONCODE_SP_35_13330_2011_MKS )  m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_CONDI_FACTOR)));

    if ( strConCode == CONCODE_AASHTO_LRFD12 || strConCode == CONCODE_AASHTO_LRFD14 ||
        strConCode == CONCODE_AASHTO_LRFD16 || strConCode == CONCODE_AASHTO_LRFD17 || 
        strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 ||
        strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_IRC112_2011 ||
        strConCode == CONCODE_IS456_2000 || strConCode == CONCODE_IRC112_2020 || strConCode == CONCODE_BS5400_90 || 
        strConCode == CONCODE_TMH07_89   || strConCode == CONCODE_AREMA_2023)
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_DESIGN)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
    // for russia plate
    if ( strConCode == CONCODE_SNIP_2_05_03_84 || strConCode == CONCODE_SP_35_13330_2011 ||
        strConCode == CONCODE_SNIP_2_05_03_84_MKS || strConCode == CONCODE_SP_35_13330_2011_MKS )
    {
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_BEAM_DATA_CHK)));
        m_ComboMenuStr.Add(CString(_LS(IDS_TREE_TEXT_CON_PLATE_COL_DATA_CHK)));
    }
}


void CDgnConDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd = GetSafeHwnd();
    if ( !hWnd || !IsWindow(hWnd) ) return;

    switch ( lHint )
    {
    case D_UPDATE_DEFAULT:
        // do something...
        break;
    case D_UPDATE_BUFFER_BEFORE:
        // do something...
        break;
    case D_UPDATE_BUFFER_AFTER:
        {
            Update_InitDataByCode(TRUE);
            break;
        }
    case D_UPDATE_UNIT:
        // do something...
        break;
    default:
        ASSERT(TRUE);
    }
}

void CDgnConDlg::Update_InitDataByCode(BOOL bUpdate)
{
    if ( bUpdate )
    {
        CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
        int nCount = pViewBuff->GetCount();
        if ( nCount==0 ) return;
        ASSERT(nCount==1);

        BOOL bCODE=FALSE;
        T_UDRD_BUFFER buffer_ur;
        POSITION pos = pViewBuff->GetStartBuffer();
        while ( pos )
        {
            buffer_ur = pViewBuff->GetNextBuffer(pos);
            int nCmd = buffer_ur.nCmd;
            int nKey = buffer_ur.nKey;

            switch ( nCmd )
            {
            case(UR_DCON_ADD):
            case(UR_DCON_DEL): {bCODE = TRUE; break; }
            default:	break;
            }
        }
        if ( !bCODE ) return;
    }

    int iSelID = m_ComboMenu.GetCurSel();
    if ( iSelID < 0 )	iSelID = 0;

    if ( m_MenuItems[iSelID].nTableID==ID_QUERY_DGN_CON_BEMW )
    {
        BOOL bEnable = IsEnbleForBemw(m_MenuItems[iSelID].nTableID);
        m_wndTableBtn.EnableWindow(bEnable);
    }
}

BOOL CDgnConDlg::IsEnbleForBemw(int nTableID)
{
    BOOL bEnable = TRUE;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    if ( nTableID==ID_QUERY_DGN_CON_BEMW )
    {
        if ( !pDoc->m_pPostCtrl->IsPostEnable() )  bEnable = FALSE;
        if ( !pDoc->m_pAttrCtrl->IsWallEndHorBarCondition() ) bEnable = FALSE;
    }
    return bEnable;
}

void CDgnConDlg::OnTmExecute()
{
	if ( m_nCurChildDlg < m_MenuItems.GetSize() )
		m_MenuItems[m_nCurChildDlg].pChildDlg->Execute();
}

void CDgnConDlg::OnTmClose()
{
	if ( CDBDoc::GetDocPoint() )
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}