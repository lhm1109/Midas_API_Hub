// StructStldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructStldDlg.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��

// (����)
#include "StldCnldDlg.h"
#include "StldCnldTypeDlg.h"
#include "StldSdspDlg.h"
#include "StldBodfDlg.h"
#include "StldNbofDlg.h"
#include "StldBmldDlg.h"
#include "StldLbldDlg.h"
#include "StldFbldDlg.h"
#include "StldTbldDlg.h"
#include "StldPrstDlg.h"
#include "StldPrtsDlg.h"
#include "StldPresDlg2.h"
#include "StldPresQueryDlg.h"
#include "StldPresAreaDlg.h"
#include "StldHprsDlg.h"
#include "StldStmpDlg.h"
#include "StldSseiDlg.h"
#include "StldNtmpDlg.h"
#include "StldEtmpDlg.h"
#include "StldFssfDlg.h"
#include "StldFipaDlg.h"
#include "StldFireDlg.h"
#include "StldGtmpDlg.h"
#include "StldLaplDlg.h"
#include "StldIfgsDlg.h"
#include "StldTendonPrst.h"
#include "StldTimeLoadsForCS.h"
#include "StldCamberForCS.h"
#include "StldCrpc.h"
#include "LdgrChangeDlg.h"
#include "StldPnldDlg.h"
#include "StldBtmpDlg.h"
#include "StldPtmpDlg.h"
#include "StldFmldDlg.h"
#include "StldStbkDlg.h"
#include "StldUlcbDlg.h"
#include "ResponseDispLoadDlg.h"
#include "StldEarthPressureSeisDlg.h"
#include "StldEarthPressureStaticDlg.h"
#include "BndrIelcDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#include "..\mit_frx\MBarBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg

IMPLEMENT_DYNCREATE(CStructStldDlg, MChildFormView)

CStructStldDlg::CStructStldDlg()
	: MChildFormView(CStructStldDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructStldDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	struct T_COMBO_ITEM
	{
		int nMask;
		int nMode;
	};
	// (����) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_BODF__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_NBOF__ },
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_STLD_CNLD__ },
#ifdef _MGEN_CH
		{ D_PROD_GEN			           , __TMMODE_STRUCT_STLD_CNLD_TYPE__},
#endif
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_STLD_SDSP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_BMLD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_LBLD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_TBLD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_FBLD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_FMLD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_PRES__ },
#ifdef _MGEN_CH
		{ D_PROD_GEN			           , __TMMODE_STRUCT_STLD_PRES_QUERY__},
#endif
		{ D_PROD_GEN			           , __TMMODE_STRUCT_STLD_PRES_AREA__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_HPRS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_PNLD__ },
		{ D_PROD_GEN                       , __TMMODE_STRUCT_STLD_LAPL__ }, // Loading Area Plane �ϴ� GEN����̴�.
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_STMP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_NTMP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_ETMP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_GTMP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_BTMP__ },
		{ D_PROD_CVL                       , __TMMODE_STRUCT_STLD_PTMP__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_PRST__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_PRTS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_TENDON_PRST__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_TLCS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_CRPC__ },
		{            D_PROD_CVL            , __TMMODE_STRUCT_STLD_CMCS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_IFGS__ },
		{            D_PROD_CVL            , __TMMODE_STRUCT_STLD_SSEI__ }, //
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_LDGR_CHANGE__ },
		{ D_PROD_GEN                       , __TMMODE_STRUCT_RESPOSNS_DISP__}, 
		{ D_PROD_GEN                       , __TMMODE_STRUCT_STLD_FIPA__}, 
		{ D_PROD_GEN                       , __TMMODE_STRUCT_STLD_FIRE__}, 
		{ D_PROD_GEN                       , __TMMODE_STRUCT_STLD_FSSF__}, 
		{ D_PROD_CVL					   , __TMMODE_STRUCT_STLD_STBK__ },
		{ D_PROD_CVL					   , __TMMODE_STRUCT_STLD_ULCB__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_STLD_EPSE__},
		{ D_PROD_GEN                       , __TMMODE_STRUCT_STLD_EPST__},
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_IELC__ },
	};

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	int i, nCount;
	nCount = sizeof(aItem)/sizeof(T_COMBO_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aItem[i].nMask) == 0) continue;
	 
		BOOL bFlag = TRUE;
		switch (aItem[i].nMode)
		{
		case __TMMODE_STRUCT_STLD_SSEI__:
			{
#if defined(_ORG) || defined(_CH) || defined(_US) || defined(_RUS)
				bFlag = FALSE;
#endif
			}
			break;
		case __TMMODE_STRUCT_RESPOSNS_DISP__:
			{
				bFlag = FALSE;
#if defined(_CH) 
				bFlag = TRUE;
#endif
			}
			break;
		case __TMMODE_STRUCT_STLD_LAPL__:
			{
				bFlag = CDBDoc::IsEnableWindPressure();
#if defined(_JP)
				bFlag = TRUE;
#endif
			}
			break;
		case __TMMODE_STRUCT_STLD_EPSE__:
			{
				bFlag = CDBDoc::IsEnableEarthPressureSeis();
			}
			break;
		case __TMMODE_STRUCT_STLD_FIRE__:
		case __TMMODE_STRUCT_STLD_FIPA__:
		case __TMMODE_STRUCT_STLD_FSSF__:
#if !defined(_CH) 
			bFlag = FALSE;
#endif
			break;

		default:
			break;
		}

		if(bFlag) AddChildDlgs(aItem[i].nMode);
	}

	m_nCurChildDlg = -1; 
}

CStructStldDlg::~CStructStldDlg()
{
	DeleteChildDlgs();
}

void CStructStldDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructStldDlg)
	DDX_Control(pDX, IDC_TM_STLD_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_STLD_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructStldDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructStldDlg)
	ON_CBN_SELCHANGE(IDC_STLD_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_STLD_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg diagnostics

#ifdef _DEBUG
void CStructStldDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructStldDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg Implementation Functions
// (����)
void CStructStldDlg::AddChildDlgs(int nMode)
{
	m_DlgIDs.Add(nMode);
	switch(nMode)
	{
	case __TMMODE_STRUCT_STLD_BODF__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Self_Weight));
		m_ChildDlgs.Add(new CStldBodfDlg);
		m_TableIDs.Add(0);  // ���̺� ������ 0���� 
	  break;			
	case __TMMODE_STRUCT_RESPOSNS_DISP__:
			m_ComboMenuStr.Add(_LS(IDS_COMMON_RESPONSE_DISPLACEMENT_LOAD));
			m_ChildDlgs.Add(new CResponseDispLoadDlg);
			m_TableIDs.Add(0);  // ���̺� ������ 0���� 
			break;	
	case __TMMODE_STRUCT_STLD_NBOF__:
		m_ComboMenuStr.Add(_LS(IDS_TM_DT_NBOF));
		m_ChildDlgs.Add(new CStldNbofDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_NBOF);  // ���̺� ������ 0���� 
	  break;																			  
	case __TMMODE_STRUCT_STLD_CNLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Nodal_Loads));
		m_ChildDlgs.Add(new CStldCnldDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_CNLD);
		break;
	case __TMMODE_STRUCT_STLD_CNLD_TYPE__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Nodal_Loads_Type));
		m_ChildDlgs.Add(new CStldCnldTypeDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_CNLD);
		break;
	case __TMMODE_STRUCT_STLD_SDSP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Specified_Displacements_of_Supports));
		m_ChildDlgs.Add(new CStldSdspDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_SDSP);
		break;
	case __TMMODE_STRUCT_STLD_BMLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Element_Beam_Loads));
		m_ChildDlgs.Add(new CStldBmldDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_BMLD);
		break;
	case __TMMODE_STRUCT_STLD_LBLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Line_Beam_Loads));
		m_ChildDlgs.Add(new CStldLbldDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_BMLD);
		break;
	case __TMMODE_STRUCT_STLD_TBLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Typical_Beam_Loads));
		m_ChildDlgs.Add(new CStldTbldDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_BMLD);
		break;
	case __TMMODE_STRUCT_STLD_FBLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Assign_Floor_Loads));
		m_ChildDlgs.Add(new CStldFbldDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_FBLA);    // Define Floor Load
		break;  																			     // DialogBar Menu���� �ٲ�. 2000.7.25
	case __TMMODE_STRUCT_STLD_FMLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Finishing_Loads));
		m_ChildDlgs.Add(new CStldFmldDlg);					//2 by KYM 2002.5.9  Finishing Material Load(��������)
		m_TableIDs.Add(ID_QUERY_STRUCT_FMLD);				
		break;
	case __TMMODE_STRUCT_STLD_PRES__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Pressure_Loads));
		m_ChildDlgs.Add(new CStldPresDlg2);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRES);
		break;
	case __TMMODE_STRUCT_STLD_PRES_QUERY__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Pressure_Loads_Query));
		m_ChildDlgs.Add(new CStldPresQueryDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRES_QUERY);
		break;
	case __TMMODE_STRUCT_STLD_PRES_AREA__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Pressure_Loads_AREA));
		m_ChildDlgs.Add(new CStldPresAreaDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_ARPR);
		break;
	case __TMMODE_STRUCT_STLD_HPRS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Hydrostatic_Pressure_Loads));
		m_ChildDlgs.Add(new CStldHprsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRES);       // JJB 040110
		break;
	case __TMMODE_STRUCT_STLD_PNLD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Assign_Plane_Loads));
		m_ChildDlgs.Add(new CStldPnldDlg);					// ��ȫ 020123
		m_TableIDs.Add(ID_QUERY_STRUCT_PNLA);				// ��ȫ 020123
		break;
	case __TMMODE_STRUCT_STLD_STMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_System_Temperature));
		m_ChildDlgs.Add(new CStldStmpDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_STLD_NTMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Nodal_Temperatures));
		m_ChildDlgs.Add(new CStldNtmpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_NTMP);
		break;
	case __TMMODE_STRUCT_STLD_ETMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Element_Temperatures));
		m_ChildDlgs.Add(new CStldEtmpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_ETMP);
		break;
	case __TMMODE_STRUCT_STLD_FIRE__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Fire_Temperature));
		m_ChildDlgs.Add(new CStldFireDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_FIRE);
		break;
	case __TMMODE_STRUCT_STLD_FSSF__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Fire_Sect_Shape_Coe));
		m_ChildDlgs.Add(new CStldFssfDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_FSSF);
		break;
	case __TMMODE_STRUCT_STLD_FIPA__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Fire_Parameter));
		m_ChildDlgs.Add(new CStldFipaDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_FIPA);
		break;
	case __TMMODE_STRUCT_STLD_GTMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Temperature_Gradient));
		m_ChildDlgs.Add(new CStldGtmpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_GTMP);
		break;
	case __TMMODE_STRUCT_STLD_BTMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Beam_Section_Temperature));
		m_ChildDlgs.Add(new CStldBtmpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_BTMP);
		break;
	case __TMMODE_STRUCT_STLD_PTMP__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Plate_Thickness_Temperature));
		m_ChildDlgs.Add(new CStldPtmpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PTMP);
		break;
	case __TMMODE_STRUCT_STLD_PRST__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Prestress_Beam_Loads));
		m_ChildDlgs.Add(new CStldPrstDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRESTR);
		break;
	case __TMMODE_STRUCT_STLD_PRTS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Pretension_Loads));
		m_ChildDlgs.Add(new CStldPrtsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRETENS);
		break;
	case __TMMODE_STRUCT_STLD_TENDON_PRST__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Tendon_Prestress_Loads));
		m_ChildDlgs.Add(new CStldTendonPrst);
		m_TableIDs.Add(ID_QUERY_STRUCT_TDPL);
		break;
	case __TMMODE_STRUCT_STLD_TLCS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Time_Loads_for_Const_Stage));
		m_ChildDlgs.Add(new CStldTimeLoadsForCS);
		m_TableIDs.Add(ID_QUERY_STRUCT_TMLD);
		break;
	case __TMMODE_STRUCT_STLD_CRPC__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Creep_Coefficient_for_Const_Stage));
		m_ChildDlgs.Add(new CStldCrpc);
		m_TableIDs.Add(ID_QUERY_STRUCT_CRPC);
		break;
	case __TMMODE_STRUCT_STLD_CMCS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Camber_for_Const_Stage));
		m_ChildDlgs.Add(new CStldCamberForCS);
		m_TableIDs.Add(ID_QUERY_STRUCT_CMCS);
		break;
	case __TMMODE_STRUCT_STLD_STBK__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Set_Back_for_Const_Stage));
		m_ChildDlgs.Add(new CStldStbkDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_STBK);
		break;
	case __TMMODE_STRUCT_STLD_ULCB__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Unstrained_Length_of_Cable_for_Construction_Stage));
		m_ChildDlgs.Add(new CStldUlcbDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_ULCB);
		break;
	case __TMMODE_STRUCT_STLD_IFGS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Initial_Forces_for_Geometric_Stiffn));
		m_ChildDlgs.Add(new CStldIfgsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_IFGS);  
		break;
	case __TMMODE_STRUCT_STLD_SSEI__:             // �ͳ� 040308
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Superstructure_Seismic_Loads));
		m_ChildDlgs.Add(new CStldSseiDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_LDGR_CHANGE__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Change_Load_Group));
		m_ChildDlgs.Add(new CLdgrChangeDlg);
		m_TableIDs.Add(0); //  Table Id �߰� �Ұ�...
		break;
	case __TMMODE_STRUCT_STLD_LAPL__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Loading_Area_Plane));
		m_ChildDlgs.Add(new CStldLaplDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_STLD_EPSE__:	// Seismic Earth Pressure
		m_ComboMenuStr.Add(_LS(IDS_CMD_EARTHP_SEIS));  
		m_ChildDlgs.Add(new CStldEarthPressureSeisDlg(this));
		m_TableIDs.Add(ID_QUERY_STRUCT_EARTHP_SEIS); //Table Id
		break;
	case __TMMODE_STRUCT_STLD_EPST__:	// Static Earth Pressure
		m_ComboMenuStr.Add(_LS(IDS_CMD_EARTHP_STATIC));  
		m_ChildDlgs.Add(new CStldEarthPressureStaticDlg(this)); 
		m_TableIDs.Add(ID_QUERY_STRUCT_EARTHP_STATIC); 
		break;
	case __TMMODE_STRUCT_BNDR_IELC__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Ignore_Element));
		m_ChildDlgs.Add(new CBndrIelcDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_IELC);
		break;
	}
}

void CStructStldDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CStructStldDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_STLD__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
			else m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CStructStldDlg::ShowCurChildDlg(int nDlgIndex)
{
	MInitCombo initCombo;

#ifdef _MGEN_CH
	if(m_nCurChildDlg == nDlgIndex)
	{
		if(m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_STLD_CNLD_TYPE__)
			m_ChildDlgs[nDlgIndex]->UpdateChildWindow();
		return;
	}
#else
	if(m_nCurChildDlg == nDlgIndex)
		return;
#endif

		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	mit::frx::MBarBaseDlg* pParent = (mit::frx::MBarBaseDlg*)GetParent();

	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();
		
	pParent->SetApplyBtnEanble(TRUE);

	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
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
	pParent->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

void CStructStldDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[m_nCurChildDlg]->GetWindowRect(&DlgRect);
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

int CStructStldDlg::_GetHeightFromFont()
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

void CStructStldDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_ComboMenu.GetDC();
	for (int i=0;i < m_ComboMenu.GetCount();i++)
	{
		m_ComboMenu.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_ComboMenu.ReleaseDC(pDC);

//  dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CStructStldDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize()+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i< m_ComboMenuStr.GetSize(); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();
}

void CStructStldDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg message handlers

void CStructStldDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	MInitCombo initCombo;
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructStldDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CStructStldDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CStructStldDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


void CStructStldDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	if(m_TableIDs[nCurSel] == ID_QUERY_STRUCT_PRES_QUERY)
	{
		CMD_DoModal(D_CMD_PSLT_ITEM_DLG);
		return;
	}
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));			
}

/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructStldDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
//  if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;

	if(m_DlgIDs[m_nCurChildDlg] == __TMMODE_STRUCT_STLD_PRES__)  // Pressure Load
		((CStldPresDlg2 *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);

	if(m_DlgIDs[m_nCurChildDlg] == __TMMODE_STRUCT_STLD_HPRS__)   // Hydro static Load
		((CStldHprsDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);

}

/////////////////////////////////////////////////////////////////////
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CStructStldDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}


CWnd* CStructStldDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

bool CStructStldDlg::ShowApplyBtn()
{
	if (m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg)
	{
		ASSERT(0);  return true;
	}

	switch ( m_DlgIDs.GetAt(m_nCurChildDlg) )
	{
	case __TMMODE_STRUCT_STLD_BODF__:
	case __TMMODE_STRUCT_STLD_NBOF__:
	case __TMMODE_STRUCT_STLD_TENDON_PRST__:
	//case __TMMODE_STRUCT_STLD_TBLD__:
	case __TMMODE_STRUCT_STLD_STMP__:
		return false;
	default:
		return true;
	}
}

void CStructStldDlg::OnTmExecute()
{
	// ���� ���� Child Dialog�� Execute ��ƾ (Apply ��ƾ) ȣ��ǵ��� �� ��� ��. 
	if(m_ChildDlgs[m_nCurChildDlg])
	m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructStldDlg::OnTmClose()
{
	// ���� ���� Child Dialog�� Close ��ƾ�� ȣ�� �ǵ��� �� ��� �� 
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}
