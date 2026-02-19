// RCDesignSeismic.cpp: implementation of the CRCDesignSeismic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCDesignSeismic.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "CRCDataCtrl.h"

#include "DgnMPhiCurveDlg.h"
#include "DgnUserMPhiDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRCDesignSeismic::CRCDesignSeismic(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CRCDesignSeismic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCDesignSeismic)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_bUpdateItemChanged = TRUE;

	T_CHRP_D ChrcD;
	ChrcD.Initialize();
	m_pDoc->m_pAttrCtrl->GetChrp(ChrcD);
	m_iDgnCode = ChrcD.iDgnCode;

	m_strSelectElem = _T("");
	m_strSelectNode = _T("");
	for(int i=0 ; i<2 ; i++)
	{	
		m_arPierUserMPhi[i].RemoveAll();  // Pier - 사용자 입력 M-Phi 곡선 정보
		m_dPierYieldCurvature[i]    = 0.0;// Pier - 사용자 입akr력 항복곡률
		m_dPierUltimateCurvature[i] = 0.0;// Pier - 사용자 입력 극한곡률
	}

	m_nOption = 0;
	m_nMemberType = 0;
	m_nCurMemberType = 0;
	m_nPierHingeType = 0;
	m_bDoubleColumnPier = FALSE;
	m_nElemOutType = 0;
	m_nLinkOutType = 0;
	
	m_aPierID.RemoveAll();
	m_aFoundationID.RemoveAll();	
	m_aCoppingBeamID.RemoveAll();
	m_aRubSupportID.RemoveAll();	
	m_aActSupportID.RemoveAll();
	m_aFixSupportID.RemoveAll();	
	m_aElemPrintID.RemoveAll();
	m_aLinkPrintID.RemoveAll();
	m_aElemListID.RemoveAll();
	m_aELinkListID.RemoveAll();
	m_aGLinkListID.RemoveAll();
	m_aCtrlButtonID.RemoveAll();
	m_aSinglePierListID.RemoveAll();
	m_aPairPierListID.RemoveAll();
	m_aSinglePierID.RemoveAll();

	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_GROUP);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_HN_STC);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_HN_EDT);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_HN_UNIT);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_DCOLUMNPIER_CHK);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_STC);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_EDT);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_UNIT);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_GROUP);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_BOT_RDO);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_TOP_BOT_RDO);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_COMB);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_BTN);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP);
	m_aPierID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_EDT);

	m_aFoundationID.Add(IDC_DGN_RC_RSDM_FDN_GROUP);
	m_aFoundationID.Add(IDC_DGN_RC_RSDM_FDN_AGROUND_STC);
	m_aFoundationID.Add(IDC_DGN_RC_RSDM_FDN_AGROUND_EDT);
	m_aFoundationID.Add(IDC_DGN_RC_RSDM_FDN_AGROUND_UNIT);

	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_GROUP);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_SPAN_STC);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_SPAN_EDT);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_SPAN_UNIT);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_ELEM_STC);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_ELEM_EDT);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_NODE_STC);
	m_aCoppingBeamID.Add(IDC_DGN_RC_RSDM_COPP_NODE_EDT);

	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_GROUP);
	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_THICK_STC);
	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_THICK_EDT);
	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_THICK_UNIT);
	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_FRIC_STC);
	m_aRubSupportID.Add(IDC_DGN_RC_RSDM_SUPPL_FRIC_EDT);

	m_aActSupportID.Add(IDC_DGN_RC_RSDM_SUPAT_GROUP);
	m_aActSupportID.Add(IDC_DGN_RC_RSDM_SUPAT_HDIS_STC);
	m_aActSupportID.Add(IDC_DGN_RC_RSDM_SUPAT_HDIS_EDT);
	m_aActSupportID.Add(IDC_DGN_RC_RSDM_SUPAT_HDIS_UNIT);

	m_aFixSupportID.Add(IDC_DGN_RC_RSDM_SUPFI_GROUP);
	m_aFixSupportID.Add(IDC_DGN_RC_RSDM_SUPFI_HLOAD_STC);
	m_aFixSupportID.Add(IDC_DGN_RC_RSDM_SUPFI_HLOAD_EDT);
	m_aFixSupportID.Add(IDC_DGN_RC_RSDM_SUPFI_HLOAD_UNIT);

	m_aElemPrintID.Add(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP);
	m_aElemPrintID.Add(IDC_DGN_RC_RSDM_ELEM_PRINT_NONE_RDO);
	m_aElemPrintID.Add(IDC_DGN_RC_RSDM_ELEM_PRINT_I_RDO);
	m_aElemPrintID.Add(IDC_DGN_RC_RSDM_ELEM_PRINT_J_RDO);
	m_aElemPrintID.Add(IDC_DGN_RC_RSDM_ELEM_PRINT_IJ_RDO);

	m_aLinkPrintID.Add(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP);
	m_aLinkPrintID.Add(IDC_DGN_RC_RSDM_LINK_PRINT_NONE_RDO);
	m_aLinkPrintID.Add(IDC_DGN_RC_RSDM_LINK_PRINT_OUT_RDO);

	m_aElemListID.Add(IDC_DGN_RC_RSDM_ELEM_LIST_STC);
	m_aElemListID.Add(IDC_DGN_RC_RSDM_ELEM_LIST);

	m_aELinkListID.Add(IDC_DGN_RC_RSDM_ELINK_LIST_STC);
	m_aELinkListID.Add(IDC_DGN_RC_RSDM_ELINK_LIST);

	m_aGLinkListID.Add(IDC_DGN_RC_RSDM_GLINK_LIST_STC);
	m_aGLinkListID.Add(IDC_DGN_RC_RSDM_GLINK_LIST);

	m_aCtrlButtonID.Add(IDC_DGN_RSDM_EXECUTE);
	m_aCtrlButtonID.Add(IDC_DGN_RSDM_CLOSE);

	m_aSinglePierID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_GROUP);
	m_aSinglePierID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_EDT);
	m_aSinglePierID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_STC);
	m_aSinglePierID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_UNIT);

	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_GROUP);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_EDT);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_STC);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_UNIT);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_GROUP);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_BOT_RDO);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_TOP_BOT_RDO);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_COMB);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_BTN);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP);
	m_aSinglePierListID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_EDT);

	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_HN_EDT);
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_HN_UNIT);
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_ALLOWDISP_EDT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_ALLOWDISP_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_ALLOWDISP_UNIT);
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMAX_EDT);
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMAX_UNIT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMIN_EDT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMIN_UNIT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_HN_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMAX_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMIN_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMAX_EDT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMAX_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMAX_UNIT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMIN_EDT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMIN_STC );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMIN_UNIT );
	m_aPairPierListID.Add(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_GROUP );
	
	m_aHingeListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_GROUP);
	m_aHingeListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_BOT_RDO);
	m_aHingeListID.Add(IDC_DGN_RC_RSDM_PIER_HINGE_TOP_BOT_RDO);
	m_aMphiListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP);
	m_aMphiListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_COMB);
	m_aMphiListID.Add(IDC_DGN_RC_RSDM_PIER_MPHI_BTN);
	m_aElemInputListID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP);
	m_aElemInputListID.Add(IDC_DGN_RC_RSDM_PIER_ELEM_EDT);
	

}


void CRCDesignSeismic::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)
	DDX_Radio(pDX, IDC_DGN_RC_RSDM_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_RC_RSDM_PIER_RDO, m_nMemberType);
	DDX_Radio(pDX, IDC_DGN_RC_RSDM_PIER_HINGE_BOT_RDO,            m_nPierHingeType);	  
	DDX_Radio(pDX, IDC_DGN_RC_RSDM_ELEM_PRINT_NONE_RDO,           m_nElemOutType);
	DDX_Radio(pDX, IDC_DGN_RC_RSDM_LINK_PRINT_NONE_RDO,           m_nLinkOutType);
	DDX_Check(pDX, IDC_DGN_RC_RSDM_PIER_VARIABLE_DCOLUMNPIER_CHK, m_bDoubleColumnPier);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_VARIABLE_HN_UNIT,       m_PierHnUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_UNIT,m_PierAllowDispUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_FDN_AGROUND_UNIT,            m_FdnAGroundUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_COPP_SPAN_UNIT,              m_CoppingSpanUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPPL_THICK_UNIT,            m_RubSupThickUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPAT_HDIS_UNIT,             m_ActSupHDisUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPFI_HLOAD_UNIT,            m_FixSupHLoadUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_VARIABLE_HN_EDT,        m_PierHnEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_EDT, m_PierAllowDispEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_FDN_AGROUND_EDT,             m_FdnAGroundEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_COPP_SPAN_EDT,               m_CoppingSpanEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPPL_THICK_EDT,             m_RubSupThickEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPPL_FRIC_EDT,              m_RubSupFricEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPAT_HDIS_EDT,              m_ActSupHDisEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SUPFI_HLOAD_EDT,             m_FixSupHLoadEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_ELEM_LIST,                   m_ElemList);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_ELINK_LIST,                  m_ELinkList);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_GLINK_LIST,                  m_GLinkList);
	
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_MPHI_COMB,              m_cmbMpcc);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PIER_ELEM_EDT,               m_PierElemSelEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_COPP_ELEM_EDT,               m_CoppingElemSelEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_COPP_NODE_EDT,               m_CoppingNodeSelEdit);

	DDX_Control(pDX, IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_EDT,               m_SinglePierHnEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_HN_UNIT,              m_SinglePierHnUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_HN_EDT,               m_PairPierHnEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_HN_UNIT,              m_PairPierHnUnit );
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_ALLOWDISP_UNIT,               m_PairPierAllowDispUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_ALLOWDISP_EDT,               m_PairPierAllowDispEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMAX_UNIT,               m_PairPierBOTMAXUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMAX_EDT,               m_PairPierBOTMAXEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMIN_UNIT,               m_PairPierBOTMINUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_BOTMIN_EDT,               m_PairPierBOTMINEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMAX_UNIT,               m_PairPierTOPMAXUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMAX_EDT,               m_PairPierTOPMAXEdit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMIN_UNIT,               m_PairPierTOPMINUnit);
	DDX_Control(pDX, IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_TOPMIN_EDT,               m_PairPierTOPMINEdit);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCDesignSeismic, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CRCDesignSeismic)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_PIER_MPHI_BTN, OnMPhiBtn)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_PIER_VARIABLE_DCOLUMNPIER_CHK, OnDoubleColumnPierChk)
	ON_BN_CLICKED(IDC_DGN_RSDM_CLOSE,            OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_RSDM_EXECUTE,          OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_AR,            OnDgnCRCRsdmAr)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_DEL,           OnDgnCRCRsdmDel)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_PIER_RDO,      OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_ABUT_RDO,      OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_FDN_RDO,       OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_COPP_RDO,      OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_SUPPL_RDO,     OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_SUPAT_RDO,     OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_SUPFI_RDO,     OnChangeMemberType)
	ON_BN_CLICKED(IDC_DGN_RC_RSDM_ARCH_RDO,      OnChangeMemberType)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RC_RSDM_ELEM_LIST,  OnItemchangedElemList)	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RC_RSDM_ELINK_LIST, OnItemchangedELinkList)		
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RC_RSDM_GLINK_LIST, OnItemchangedGLinkList)	

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CRCDesignSeismic::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CRCDesignSeismic::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();	

	BOOL bELnk = FALSE;
	BOOL bNLnk = FALSE;
	BOOL bElem = FALSE;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		T_RSDM_K RsdmK;
		RsdmK.keymap = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RSDL_ADD):
		case(UR_RSDL_DEL):
				bElem = TRUE;
				break;
		case(UR_RSDM_ADD):
		case(UR_RSDM_DEL):
				bELnk = TRUE;
				bNLnk = TRUE;
				break;
//		case(UR_ELEM_ADD):
//		case(UR_ELEM_DEL):
//        bElem = TRUE;
//        break;
//		case(UR_ELNK_ADD):
//		case(UR_ELNK_DEL):
//        bELnk = TRUE;
//        break;
//		case(UR_NLNK_ADD):
//		case(UR_NLNK_DEL):
//        bNLnk = TRUE;
//        break;
		default:
			break;
		}
	} // end of while

	m_bUpdateItemChanged = FALSE;
	if(bElem) UpdateElemList(m_nCurMemberType);
	if(bELnk) UpdateELinkList();
	if(bNLnk) UpdateGLinkList();
	m_bUpdateItemChanged = TRUE;
}

BOOL CRCDesignSeismic::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;
	
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_RDO)->SetWindowText(_LS(IDS_DGN_PSC_SEISMIC_MEMB_SINGLEPIER));
		GetDlgItem(IDC_DGN_RC_RSDM_FDN_RDO)->SetWindowText(_LS(IDS_DGN_PSC_SEISMIC_MEMB_PAIRPIER));
		InitCtrl_CJJ();
	}
	else
	{
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_RDO)->SetWindowText(_LS(IDS_DGN_PSC_SEISMIC_MEMB_PIER));
		GetDlgItem(IDC_DGN_RC_RSDM_FDN_RDO)->SetWindowText(_LS(IDS_DGN_PSC_SEISMIC_MEMB_FOUNDATION));
		InitCtrl();
	}
	Initial_Data();

	UpdateElemList(m_nCurMemberType);
	UpdateELinkList();
	UpdateGLinkList();

	OnChangeMemberType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCDesignSeismic::OnChangeMemberType()
{
	UpdateData(TRUE);
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		m_nCurMemberType =m_nMemberType + 10;
		ChangeMemberType_CJJ();
	}
	else
	{
		m_nCurMemberType =m_nMemberType;
		ChangeMemberType();
	}
	
}

void CRCDesignSeismic::OnMPhiBtn()
{
	int nSectIndex = m_cmbMpcc.GetCurSel();
	T_MPCC_K MpccK;
	if(nSectIndex == -1) { MpccK = 0; }
	MpccK = m_cmbMpcc.GetItemData(nSectIndex);

	if(MpccK == 0)
	{		
		int i;
		CDgnUserMPhiDlg dlg;
		for(i=0 ; i<2 ; i++)
		{
			dlg.m_arPierUserMPhi[i].Copy(m_arPierUserMPhi[i]);
			dlg.m_dPierYieldCurvature[i]    = m_dPierYieldCurvature[i];
			dlg.m_dPierUltimateCurvature[i] = m_dPierUltimateCurvature[i];
		}
		
		if(dlg.DoModal() == IDOK)
		{
			for(i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].Copy(dlg.m_arPierUserMPhi[i]);
				m_dPierYieldCurvature[i]    = dlg.m_dPierYieldCurvature[i];
				m_dPierUltimateCurvature[i] = dlg.m_dPierUltimateCurvature[i];
			}
		}
	}
	else 
	{
		CDgnMPhiCurveDlg dlg;
		dlg.m_InitMpccK = MpccK;		
		dlg.DoModal();
	}	
}

void CRCDesignSeismic::OnDoubleColumnPierChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_STC)->EnableWindow(m_bDoubleColumnPier);
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_EDT)->EnableWindow(m_bDoubleColumnPier);
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_ALLOWDISP_UNIT)->EnableWindow(m_bDoubleColumnPier);	
}

void CRCDesignSeismic::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CRCDesignSeismic::OnDgnExecute4CJJ() 
{	
	
	//	int nMemberType = m_nCurMemberType-10;
	UpdateData(TRUE); 
	//m_iMemberType	0:Pier  1:Abutment  2:Foundation  3:Copping Beam  4:판식고무지지점  5:활동쟁반형지지점  6:고정쟁반형지지점  7:Main arch	
	if(m_nCurMemberType==10 || m_nCurMemberType==12|| m_nCurMemberType==13)
	{		
		CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelElemKey);
		CArray<T_NODE_K,T_NODE_K> aSelNodeKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedNode(aSelNodeKey);
		
		CArray<T_RSDL_K,T_RSDL_K> aSelRsdlKey;
		int nRsdlSize = GetSelectRsdlKey(m_nCurMemberType, aSelElemKey, aSelRsdlKey);

		T_RSDL_D rData;
		T_RSDL_K rKey;
		rData.Initialize();
		if(aSelElemKey.GetSize() > 0)
		{
			if(m_nOption==0)	// add/replace
			{
				if(nRsdlSize >= 2)
				{
					AfxMessageBox(_LS(IDS_DGN_SEL_MEMB_DOUBLE), MB_OK);
					return;
				}
				else 
				{
					if(nRsdlSize == 0) rKey = m_pDoc->m_pAttrCtrl->GetStartNumRsdl();
					else               rKey = aSelRsdlKey[0];

					rData.nMemberType = m_nCurMemberType;
					rData.nOutputType = m_nElemOutType;

					rData.aElemList.Copy(aSelElemKey);
					if(m_nCurMemberType==10 ||m_nCurMemberType==12)
					{// Pier
						if(m_nCurMemberType == 10)
						{
							rData.dPierHn          = m_SinglePierHnEdit.GetEditValue();
						  rData.nPierPHingeRegionType = m_nPierHingeType;							
						}
						else
						{
							rData.dPierHn          = m_PairPierHnEdit.GetEditValue();
							rData.dPierAllowDisp = m_PairPierAllowDispEdit.GetEditValue();							
							rData.bPierDColumnBent = TRUE;
							rData.dPireMaxMinForce[0] = m_PairPierBOTMAXEdit.GetEditValue();							
							rData.dPireMaxMinForce[1] = m_PairPierBOTMINEdit.GetEditValue();							
							rData.dPireMaxMinForce[2] = m_PairPierTOPMAXEdit.GetEditValue();							
							rData.dPireMaxMinForce[3] = m_PairPierTOPMINEdit.GetEditValue();							

						}
						
						int nSectIndex = m_cmbMpcc.GetCurSel();
						if(nSectIndex == -1) { rData.PierMpccKey = 0; }
						else { rData.PierMpccKey = m_cmbMpcc.GetItemData(nSectIndex); }
						
						if(rData.PierMpccKey == 0)
						{
							for(int i=0 ; i<2 ; i++)
							{
								rData.arPierUserMPhi[i].Copy(m_arPierUserMPhi[i]);
								rData.dPierYieldCurvature[i]    = m_dPierYieldCurvature[i];
								rData.dPierUltimateCurvature[i] = m_dPierUltimateCurvature[i];
							}
						}						
					}
					else if(m_nCurMemberType==13)
					{// Copping Beam
						rData.dCappingBeamSpan = m_CoppingSpanEdit.GetEditValue();
						rData.aNodeList.Copy(aSelNodeKey);
						//rData.dCappingBeamSpan = m_CoppingSpanEdit.GetEditValue();
					}
					else	
					{ AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);  return; }
				}
				m_pDoc->m_pDataCtrl->AddRsdl(rKey, rData);
			}	
			else	// Delete
			{
				if(nRsdlSize == 0) AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MEMB), MB_OK);
				else if(m_pDoc->m_pDataCtrl->DelRsdl(aSelRsdlKey))	Initial_SelectItem();
			}
		}
		UpdateElemList(m_nCurMemberType);
	}	
	else if(m_nCurMemberType==11 || m_nCurMemberType==17)
	{
		// Select된 Element를 얻어옵니다.
		CArray<T_ELEM_K,T_ELEM_K> aSelKey;
		CArray<T_RSDM_K,T_RSDM_K> rSelKey;
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if(aSelKey.GetSize() > 0)
		{
			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				T_ELEM_K Elemkey = aSelKey.GetAt(i);
				T_RSDM_K key; 
				key.key.OType = 1; // Element Type
				//modify by maxiao 2007.01.08
				T_ELEM_D ElemD;
				if(!m_pDoc->m_pAttrCtrl->GetElem(Elemkey,ElemD))  ElemD.Initialize();
				T_MATD_D MatdD;
				m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD);
				if(MatdD.Type==_T("C")) //concret 단면만 가능
				{
					key.key.OKey = Elemkey;
					rSelKey.Add(key);
				}
			}

			T_RSDM_D rData;
			rData.Initialize();
			if(m_nOption==0)	// add/replace
			{
				rData.nMemberType = m_nCurMemberType;
				rData.nOutputType = m_nElemOutType;
				
			
				
				if(rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if(m_pDoc->m_pDataCtrl->AddRsdm(rSelKey,rData))	Initial_SelectItem();
				}
				else	AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
	  		if(m_pDoc->m_pDataCtrl->DelRsdm(rSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
	}
	else if(m_nCurMemberType==14 || m_nCurMemberType==15 || m_nCurMemberType==16)
	{
		CArray<UINT,UINT> aSelKey;
		CArray<T_RSDM_K,T_RSDM_K> rSelKey;
		T_RSDM_K key; 
		if(m_nCurMemberType==14 || m_nCurMemberType==16)
		{// 판식고무지지점, 고정쟁반형지지점
			key.key.OType = 2; // Elastic Link Type
			int nItem = -1;
			int nSelCount = m_ELinkList.GetSelectedCount();
			rSelKey.SetSize(nSelCount);
			for (UINT i=0; i < nSelCount; i++)
			{
				nItem = m_ELinkList.GetNextItem(nItem, LVNI_SELECTED);
				ASSERT(nItem != -1);
				key.key.OKey = m_ELinkList.GetItemData(nItem);
				rSelKey.SetAt(i, key);
			}
		}
		if(m_nCurMemberType==15)
		{// 활동쟁반형지지점
			key.key.OType = 3; // General Link Type
			int nItem = -1;
			int nSelCount = m_GLinkList.GetSelectedCount();
			rSelKey.SetSize(nSelCount);
			for (UINT i=0; i < nSelCount; i++)
			{
				nItem = m_GLinkList.GetNextItem(nItem, LVNI_SELECTED);
				ASSERT(nItem != -1);
				key.key.OKey = m_GLinkList.GetItemData(nItem);
				rSelKey.SetAt(i, key);
			}
		}
		
		if(rSelKey.GetSize() > 0)
		{
			T_RSDM_D rData;
			rData.Initialize();
			if(m_nOption==0)	// add/replace
			{
				rData.nMemberType = m_nCurMemberType;
				rData.nOutputType = m_nLinkOutType;
				if(m_nCurMemberType==14)
				{// 판식고무지지점
					rData.dSupportRubberThick = m_RubSupThickEdit.GetEditValue();
					rData.dSupportFrictionFactor = m_RubSupFricEdit.GetEditValue();
				}
				else if(m_nCurMemberType==15)
				{// 활동쟁반형지지점
					rData.dSupportAllowHorizonDis = m_ActSupHDisEdit.GetEditValue();
				}
				else if(m_nCurMemberType==16)
				{// 고정쟁반형지지점
					rData.dSupportMaxHorizonLoad = m_FixSupHLoadEdit.GetEditValue();
				}				
				if(m_pDoc->m_pDataCtrl->AddRsdm(rSelKey,rData))	Initial_SelectItem();				
			}
			else	// Delete
			{
				// Initialize selected Element.
	  		if(m_pDoc->m_pDataCtrl->DelRsdm(rSelKey))	Initial_SelectItem();
			}

			if     (m_nCurMemberType==14 || m_nCurMemberType==16) UpdateELinkList();
			else if(m_nCurMemberType==15) UpdateGLinkList();
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_LINK),MB_OK);
	}
	UpdateData(FALSE); 
}
void CRCDesignSeismic::OnDgnExecute() 
{	
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		OnDgnExecute4CJJ();
		return;
	}
	int nMemberType = m_nCurMemberType;
	UpdateData(TRUE); 
	//m_iMemberType	0:Pier  1:Abutment  2:Foundation  3:Copping Beam  4:판식고무지지점  5:활동쟁반형지지점  6:고정쟁반형지지점  7:Main arch	
	if(nMemberType==0 || nMemberType==3)
	{		
		CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelElemKey);
		CArray<T_NODE_K,T_NODE_K> aSelNodeKey;
		m_pDoc->m_pViewCtrl->GetAllSelectedNode(aSelNodeKey);
		
		CArray<T_RSDL_K,T_RSDL_K> aSelRsdlKey;
		int nRsdlSize = GetSelectRsdlKey(nMemberType, aSelElemKey, aSelRsdlKey);

		T_RSDL_D rData;
		T_RSDL_K rKey;
		rData.Initialize();
		if(aSelElemKey.GetSize() > 0)
		{
			if(m_nOption==0)	// add/replace
			{
				if(nRsdlSize >= 2)
				{
					AfxMessageBox(_LS(IDS_DGN_SEL_MEMB_DOUBLE), MB_OK);
					return;
				}
				else 
				{
					if(nRsdlSize == 0) rKey = m_pDoc->m_pAttrCtrl->GetStartNumRsdl();
					else               rKey = aSelRsdlKey[0];

					rData.nMemberType = nMemberType;
					rData.nOutputType = m_nElemOutType;

					rData.aElemList.Copy(aSelElemKey);
					if(nMemberType==0)
					{// Pier
						rData.dPierHn          = m_PierHnEdit.GetEditValue();
						rData.bPierDColumnBent = m_bDoubleColumnPier;
						rData.dPierAllowDisp   = m_PierAllowDispEdit.GetEditValue();
						rData.nPierPHingeRegionType = m_nPierHingeType;							
						int nSectIndex = m_cmbMpcc.GetCurSel();
						if(nSectIndex == -1) { rData.PierMpccKey = 0; }
						else { rData.PierMpccKey = m_cmbMpcc.GetItemData(nSectIndex); }
						
						if(rData.PierMpccKey == 0)
						{
							for(int i=0 ; i<2 ; i++)
							{
								rData.arPierUserMPhi[i].Copy(m_arPierUserMPhi[i]);
								rData.dPierYieldCurvature[i]    = m_dPierYieldCurvature[i];
								rData.dPierUltimateCurvature[i] = m_dPierUltimateCurvature[i];
							}
						}						
					}
					else if(nMemberType==3)
					{// Copping Beam
						rData.dCappingBeamSpan = m_CoppingSpanEdit.GetEditValue();
						rData.aNodeList.Copy(aSelNodeKey);
						//rData.dCappingBeamSpan = m_CoppingSpanEdit.GetEditValue();
					}
					else	
					{ AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);  return; }
				}
				m_pDoc->m_pDataCtrl->AddRsdl(rKey, rData);
			}	
			else	// Delete
			{
				if(nRsdlSize == 0) AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MEMB), MB_OK);
				else if(m_pDoc->m_pDataCtrl->DelRsdl(aSelRsdlKey))	Initial_SelectItem();
			}
		}
		UpdateElemList(nMemberType);
	}	
	else if(nMemberType==1 || nMemberType==2 || nMemberType==7)
	{
		// Select된 Element를 얻어옵니다.
		CArray<T_ELEM_K,T_ELEM_K> aSelKey;
		CArray<T_RSDM_K,T_RSDM_K> rSelKey;
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if(aSelKey.GetSize() > 0)
		{
			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				T_ELEM_K Elemkey = aSelKey.GetAt(i);
				T_RSDM_K key; 
				key.key.OType = 1; // Element Type
				//modify by maxiao 2007.01.08
				T_ELEM_D ElemD;
				if(!m_pDoc->m_pAttrCtrl->GetElem(Elemkey,ElemD))  ElemD.Initialize();
				T_MATD_D MatdD;
				m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD);
				if(MatdD.Type==_T("C")) //concret 단면만 가능
				{
					key.key.OKey = Elemkey;
					rSelKey.Add(key);
				}
			}

			T_RSDM_D rData;
			rData.Initialize();
			if(m_nOption==0)	// add/replace
			{
				rData.nMemberType = nMemberType;
				rData.nOutputType = m_nElemOutType;
				
				if(nMemberType==2)
				{// Foundation
					rData.dFdnAllowGround = m_FdnAGroundEdit.GetEditValue();
				}
				
				if(rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if(m_pDoc->m_pDataCtrl->AddRsdm(rSelKey,rData))	Initial_SelectItem();
				}
				else	AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
	  		if(m_pDoc->m_pDataCtrl->DelRsdm(rSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
	}
	else if(nMemberType==4 || nMemberType==5 || nMemberType==6)
	{
		CArray<UINT,UINT> aSelKey;
		CArray<T_RSDM_K,T_RSDM_K> rSelKey;
		T_RSDM_K key; 
		if(nMemberType==4 || nMemberType==6)
		{// 판식고무지지점, 고정쟁반형지지점
			key.key.OType = 2; // Elastic Link Type
			int nItem = -1;
			int nSelCount = m_ELinkList.GetSelectedCount();
			rSelKey.SetSize(nSelCount);
			for (UINT i=0; i < nSelCount; i++)
			{
				nItem = m_ELinkList.GetNextItem(nItem, LVNI_SELECTED);
				ASSERT(nItem != -1);
				key.key.OKey = m_ELinkList.GetItemData(nItem);
				rSelKey.SetAt(i, key);
			}
		}
		if(nMemberType==5)
		{// 활동쟁반형지지점
			key.key.OType = 3; // General Link Type
			int nItem = -1;
			int nSelCount = m_GLinkList.GetSelectedCount();
			rSelKey.SetSize(nSelCount);
			for (UINT i=0; i < nSelCount; i++)
			{
				nItem = m_GLinkList.GetNextItem(nItem, LVNI_SELECTED);
				ASSERT(nItem != -1);
				key.key.OKey = m_GLinkList.GetItemData(nItem);
				rSelKey.SetAt(i, key);
			}
		}
		
		if(rSelKey.GetSize() > 0)
		{
			T_RSDM_D rData;
			rData.Initialize();
			if(m_nOption==0)	// add/replace
			{
				rData.nMemberType = nMemberType;
				rData.nOutputType = m_nLinkOutType;
				if(nMemberType==4)
				{// 판식고무지지점
					rData.dSupportRubberThick = m_RubSupThickEdit.GetEditValue();
					rData.dSupportFrictionFactor = m_RubSupFricEdit.GetEditValue();
				}
				else if(nMemberType==5)
				{// 활동쟁반형지지점
					rData.dSupportAllowHorizonDis = m_ActSupHDisEdit.GetEditValue();
				}
				else if(nMemberType==6)
				{// 고정쟁반형지지점
					rData.dSupportMaxHorizonLoad = m_FixSupHLoadEdit.GetEditValue();
				}				
				if(m_pDoc->m_pDataCtrl->AddRsdm(rSelKey,rData))	Initial_SelectItem();				
			}
			else	// Delete
			{
				// Initialize selected Element.
	  		if(m_pDoc->m_pDataCtrl->DelRsdm(rSelKey))	Initial_SelectItem();
			}

			if     (nMemberType==4 || nMemberType==6) UpdateELinkList();
			else if(nMemberType==5) UpdateGLinkList();
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_LINK),MB_OK);
	}
	UpdateData(FALSE); 
}

void CRCDesignSeismic::InitCtrl()
{		
	SetHeaderTitle();

	Initial_MpccCombo();

	m_PierElemSelEdit.Connect(SC_ID_ELEM, &m_PierElemSelEdit);
	m_CoppingElemSelEdit.Connect(SC_ID_ELEM, &m_CoppingElemSelEdit);	
	m_CoppingNodeSelEdit.Connect(SC_ID_NODE, &m_CoppingNodeSelEdit);	

	int nGap = globalUtils.ScaleByDPI(8);

	m_PierHnUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PierHnEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PierAllowDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PierAllowDispEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_FdnAGroundUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FdnAGroundEdit.SetUnitType(D_UNITSYS_BASE_STRESS);	
	m_CoppingSpanUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_CoppingSpanEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_RubSupThickUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RubSupThickEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RubSupFricEdit.SetUnitType(D_UNITSYS_NONE);
	m_ActSupHDisUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ActSupHDisEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_FixSupHLoadUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_FixSupHLoadEdit.SetUnitType(D_UNITSYS_BASE_FORCE);

	CRect RectOptOrg, RectPrintOrg, RectListOrg, RectListOrg2, RectButOrg, RectCrr, RectCrr2, RectCrr3;
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_GROUP)->GetWindowRect(&RectOptOrg);

	GetDlgItem(IDC_DGN_RC_RSDM_SUPPL_GROUP)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr2);
	RectPrintOrg.top  = RectOptOrg.top + RectCrr.Height() + nGap;
	RectPrintOrg.bottom = RectPrintOrg.top + RectCrr2.Height();
	RectPrintOrg.left = RectCrr2.left;
	RectPrintOrg.right = RectCrr2.right;
	
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST)->GetWindowRect(&RectCrr2);
	RectListOrg.top = RectPrintOrg.top + RectPrintOrg.Height() + nGap;
	RectListOrg.bottom = RectListOrg.top + (RectCrr2.bottom - RectCrr.top);
	RectListOrg.left = RectCrr2.left;
	RectListOrg.right = RectCrr2.right;

	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr);
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST)->GetWindowRect(&RectCrr2);
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectCrr3);
	RectListOrg2.top    = RectCrr3.bottom + nGap + RectPrintOrg.Height() + nGap;	
	RectListOrg2.bottom = RectListOrg2.top + (RectCrr2.bottom - RectCrr.top);
	RectListOrg2.left   = RectCrr2.left;
	RectListOrg2.right  = RectCrr2.right;

	GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RSDM_CLOSE)->GetWindowRect(&RectCrr2);
	RectButOrg.top = RectListOrg2.top + RectListOrg2.Height() + nGap;
	RectButOrg.bottom = RectButOrg.top + RectCrr.Height();
	RectButOrg.left = RectCrr.left;
	RectButOrg.right = RectCrr2.right;
		

	GetDlgItem(IDC_DGN_RC_RSDM_FDN_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aFoundationID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_COPP_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aCoppingBeamID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPPL_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aRubSupportID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPAT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aActSupportID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPFI_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aFixSupportID, (RectOptOrg.top - RectCrr.top), TRUE);

	
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectPrintOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aLinkPrintID, (RectPrintOrg.top - RectCrr.top), TRUE);

	
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectListOrg2.top - RectCrr.top), TRUE);		
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aELinkListID, (RectListOrg.top - RectCrr.top), TRUE);	
	GetDlgItem(IDC_DGN_RC_RSDM_GLINK_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aGLinkListID, (RectListOrg.top - RectCrr.top), TRUE);	
	
	GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectButOrg.top - RectCrr.top), TRUE);

	CRect RectDlg;
	GetWindowRect(&RectDlg);	
	SetWindowPos(NULL, 0, 0, RectDlg.Width(), (RectButOrg.bottom - RectDlg.top + 5), SWP_NOMOVE | SWP_NOZORDER);	
}

void CRCDesignSeismic::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL,FALSE);
	// Initialize Data.
	//Initial_Data();
}

void CRCDesignSeismic::Initial_MpccCombo()
{	
	// Reset Content
	m_cmbMpcc.ResetContent();

	int nSectionIndex = 0;
	int nConcrete1Index = 0;
	int nConcrete2Index = 0;
	int nSteelIndex = 0;

	int nSectionTmp = 0; 
	int nConcrete1Tmp = -1;
	int nConcrete2Tmp = -1;
	int nSteelTmp = -1;

	// Section Combo Box
	T_MPCC_D MpccData;

	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccKey);
	m_cmbMpcc.SetItemData(m_cmbMpcc.AddString(_LS(IDS_DB_MPCC_USER_DEFINE_RSDM)), 0);

	// Rebar input for Column Section 에서 Concrete 만 받기 때문에 Section 에 대한 검사만 한다.
	for(int i=0; i<aMpccKey.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(aMpccKey[i], MpccData)) continue;

		m_cmbMpcc.SetItemData(m_cmbMpcc.AddString(MpccData.strName), aMpccKey[i]);
	}
	m_cmbMpcc.SetCurSel(0);
}

void CRCDesignSeismic::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_nCurMemberType = 0;	
	m_nPierHingeType = 0;
	m_bDoubleColumnPier = FALSE;
	m_nElemOutType = 0;
	m_nLinkOutType = 0;

	UpdateData(FALSE);
}

void CRCDesignSeismic::SetHeaderTitle()
{
	
	CStringArray aTitle;
	CArray<int, int> aWidth;

#define M_ADD_LIST_INFO(TITLE, WIDTH) aTitle.Add(TITLE); aWidth.Add(WIDTH)
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_ID),      34);  //_T("ID")   
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NODE1),   54);  //_T("Node1")
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_NODE2),   54);  //_T("Node2") 
	M_ADD_LIST_INFO(_LS(IDS_DGN_LIST_DESIGN),  54);  //_T("Design") 
#undef M_ADD_LIST_INFO

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ELinkList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ELinkList.GetSafeHwnd(), dwStyle);

	dwStyle = ListView_GetExtendedListViewStyle(m_GLinkList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_GLinkList.GetSafeHwnd(), dwStyle);

	// Set Title
	int nNum = aTitle.GetSize();
	for (i=0; i < nNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ELinkList.InsertColumn(i,&lvcolumn);
		m_GLinkList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	// Element List
	aTitle.RemoveAll();
	aWidth.RemoveAll();
	aTitle.Add(_LS(IDS_DGN_CHK_ELEM_LIST));  aWidth.Add(196);
	
	dwStyle = ListView_GetExtendedListViewStyle(m_ElemList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ElemList.GetSafeHwnd(), dwStyle);

	// Set Title
	nNum = aTitle.GetSize();
	for (i=0; i < nNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ElemList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CRCDesignSeismic::ChangeMemberType()
{
	int nMemberType_Old = m_nCurMemberType;
	UpdateData(TRUE); 
	//m_iMemberType	0:Pier  1:Abutment  2:Foundation  3:Copping Beam  4:판식고무지지점  5:활동쟁반형지지점  6:고정쟁반형지지점  7:Main arch

	CDlgUtil::CtrlShowHide(this, m_aSinglePierListID,       FALSE);
	CDlgUtil::CtrlShowHide(this, m_aPairPierListID, FALSE);

	CDlgUtil::CtrlShowHide(this, m_aPierID,       (m_nCurMemberType==0));
	CDlgUtil::CtrlShowHide(this, m_aFoundationID, (m_nCurMemberType==2));
	CDlgUtil::CtrlShowHide(this, m_aCoppingBeamID,(m_nCurMemberType==3));
	CDlgUtil::CtrlShowHide(this, m_aRubSupportID, (m_nCurMemberType==4));
	CDlgUtil::CtrlShowHide(this, m_aActSupportID, (m_nCurMemberType==5));
	CDlgUtil::CtrlShowHide(this, m_aFixSupportID, (m_nCurMemberType==6));
	CDlgUtil::CtrlShowHide(this, m_aElemPrintID,  (m_nCurMemberType==0 || m_nCurMemberType==1 || m_nCurMemberType==2 || m_nCurMemberType==3 || m_nCurMemberType==7));
	CDlgUtil::CtrlShowHide(this, m_aLinkPrintID,  (m_nCurMemberType==4 || m_nCurMemberType==5 || m_nCurMemberType==6));
	CDlgUtil::CtrlShowHide(this, m_aElemListID,   (m_nCurMemberType==0 || m_nCurMemberType==3));
	CDlgUtil::CtrlShowHide(this, m_aELinkListID,  (m_nCurMemberType==4 || m_nCurMemberType==6));
	CDlgUtil::CtrlShowHide(this, m_aGLinkListID,  (m_nCurMemberType==5));
	

	CRect RectBase, RectCrr, RectCrr2;
	int nGap = globalUtils.ScaleByDPI(8);
	// Print
	if(m_nCurMemberType==0)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
		
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr2);
		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectBase.top - RectCrr2.top), TRUE);
	
	}	
	else if(m_nCurMemberType==1)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==2)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_FDN_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==3)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_COPP_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);

		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr2);
		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectBase.top - RectCrr2.top), TRUE);
	}
	else if(m_nCurMemberType==7)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	}

	// Apply, Close	
	if(m_nCurMemberType==0 || m_nCurMemberType==3)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==1 || m_nCurMemberType==2 || m_nCurMemberType==7)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==4 || m_nCurMemberType==6)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==5)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_GLINK_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else 
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	
	if(m_nCurMemberType==0 || m_nCurMemberType==3) 
	{
		if(nMemberType_Old != m_nCurMemberType) UpdateElemList(m_nCurMemberType);
	}


	if(m_nCurMemberType==0) OnDoubleColumnPierChk();

	RedrawWindow();
}

void CRCDesignSeismic::ChangeMemberType_CJJ()
{
	int nMemberType_Old = m_nCurMemberType;
	UpdateData(TRUE); 
	//m_iMemberType	0:SinglePier  1:Abutment    3:Copping Beam  4:厥스連林  5:삶땡톰駕連林  6:미땍톰駕連林  7:Main arch 8:崗料뚫
	CDlgUtil::CtrlShowHide(this, m_aPierID,       FALSE);
	CDlgUtil::CtrlShowHide(this, m_aFoundationID,       FALSE);

	CDlgUtil::CtrlShowHide(this, m_aSinglePierListID,       (m_nCurMemberType==10));
	CDlgUtil::CtrlShowHide(this, m_aPairPierListID, (m_nCurMemberType==12));
	CDlgUtil::CtrlShowHide(this, m_aCoppingBeamID,(m_nCurMemberType==13));
	CDlgUtil::CtrlShowHide(this, m_aRubSupportID, (m_nCurMemberType==14));
	CDlgUtil::CtrlShowHide(this, m_aActSupportID, (m_nCurMemberType==15));
	CDlgUtil::CtrlShowHide(this, m_aFixSupportID, (m_nCurMemberType==16));
	CDlgUtil::CtrlShowHide(this, m_aElemPrintID,  (m_nCurMemberType==10 || m_nCurMemberType==11 || m_nCurMemberType==12 || m_nCurMemberType==13 || m_nCurMemberType==17));
	CDlgUtil::CtrlShowHide(this, m_aLinkPrintID,  (m_nCurMemberType==14 || m_nCurMemberType==15 || m_nCurMemberType==16));
	CDlgUtil::CtrlShowHide(this, m_aElemListID,   (m_nCurMemberType==10 || m_nCurMemberType==12|| m_nCurMemberType==13));
	CDlgUtil::CtrlShowHide(this, m_aELinkListID,  (m_nCurMemberType==14 || m_nCurMemberType==16));
	CDlgUtil::CtrlShowHide(this, m_aGLinkListID,  (m_nCurMemberType==15));
	
	CDlgUtil::CtrlShowHide(this, m_aMphiListID,  (m_nCurMemberType==10 || m_nCurMemberType==12));
	CDlgUtil::CtrlShowHide(this, m_aElemInputListID,  (m_nCurMemberType==10 || m_nCurMemberType==12));

	CRect RectBase, RectCrr, RectCrr2;
	int nGap = globalUtils.ScaleByDPI(8);
	// Print
	if(m_nCurMemberType==10)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_GROUP)->GetWindowRect(&RectCrr);
		//RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aSinglePierListID, (RectBase.top - RectCrr.top), TRUE);
		
		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_HINGE_GROUP)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aHingeListID, (RectBase.top - RectCrr2.top), TRUE);
		
		RectBase.top = RectBase.top + RectCrr2.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aMphiListID, (RectBase.top - RectCrr2.top), TRUE);
		
		RectBase.top = RectBase.top + RectCrr2.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemInputListID, (RectBase.top - RectCrr2.top), TRUE);
		
		RectBase.top = RectBase.top + RectCrr2.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr2.top), TRUE);


		RectBase.top = RectBase.top + RectCrr2.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectBase.top - RectCrr2.top), TRUE);
	}	
	else if(m_nCurMemberType==11)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==12)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_GROUP)->GetWindowRect(&RectBase);

		GetDlgItem(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_GROUP)->GetWindowRect(&RectCrr);
		//RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aPairPierListID, (RectBase.top - RectCrr.top), TRUE);

		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aMphiListID, (RectBase.top - RectCrr2.top), TRUE);

		RectBase.top = RectBase.top + RectCrr2.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectCrr);
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemInputListID, (RectBase.top - RectCrr.top), TRUE);

		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	
		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectBase.top - RectCrr2.top), TRUE);

	}
	else if(m_nCurMemberType==13)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_COPP_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);

		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr2);
		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectBase.top - RectCrr2.top), TRUE);
	}
	else if(m_nCurMemberType==17)
	{		
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==14 )
	{
		GetDlgItem(IDC_DGN_RC_RSDM_SUPPL_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aLinkPrintID, (RectBase.top - RectCrr.top), TRUE);

		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr2);
		//GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aELinkListID, (RectBase.top - RectCrr2.top), TRUE);

	}
	else if(m_nCurMemberType==15 )
	{
		GetDlgItem(IDC_DGN_RC_RSDM_SUPAT_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aLinkPrintID, (RectBase.top - RectCrr.top), TRUE);

		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_GLINK_LIST_STC)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aGLinkListID, (RectBase.top - RectCrr2.top), TRUE);

	}
	else if(m_nCurMemberType==16 )
	{
		GetDlgItem(IDC_DGN_RC_RSDM_SUPFI_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aLinkPrintID, (RectBase.top - RectCrr.top), TRUE);

		RectBase.top = RectBase.top + RectCrr.Height() + nGap;
		GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr2);
		if(RectBase.top != RectCrr2.top) CDlgUtil::CtrlMoveDistY(this, m_aELinkListID, (RectBase.top - RectCrr2.top), TRUE);

	}


	// Apply, Close	
	if(m_nCurMemberType==10 || m_nCurMemberType==13)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==12)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==11 || m_nCurMemberType==17)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==14 || m_nCurMemberType==16)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else if(m_nCurMemberType==15)
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_GLINK_LIST)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	else 
	{	
		GetDlgItem(IDC_DGN_RC_RSDM_MEMB_GROUP)->GetWindowRect(&RectBase);
		GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
		RectBase.top = RectBase.top + RectBase.Height() + nGap;
		if(RectBase.top != RectCrr.top) CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectBase.top - RectCrr.top), TRUE);
	}
	
	if(m_nCurMemberType==10 || m_nCurMemberType==12|| m_nCurMemberType==13) 
	{
		/*if(nMemberType_Old != m_nCurMemberType) */UpdateElemList(m_nCurMemberType);
	}


	//if(m_nCurMemberType==0) OnDoubleColumnPierChk();

	RedrawWindow();
}
void CRCDesignSeismic::UpdateElemList(int nMemberType)
{
	T_RSDL_K RsdlKey;
	T_RSDL_D RsdlData;
	LVITEM lvitem;
	CString str;

	CArray<UINT, UINT> keyList;
	m_pDoc->m_pAttrCtrl->GetRsdlKeyList(keyList);	
	int nSize = keyList.GetSize();
	int i;
	for(i=nSize-1 ; i>=0 ; i--)
	{
		if(!m_pDoc->m_pAttrCtrl->GetRsdl(keyList[i], RsdlData)) { keyList.RemoveAt(i);  continue; }
		if(RsdlData.nMemberType != nMemberType)                 { keyList.RemoveAt(i);  continue; }
	}

	nSize = keyList.GetSize();
	int nItem = m_ElemList.GetItemCount();
	if(nSize != nItem) 
	{		
		if(nSize < nItem)
		{			
			for(int i=nItem-1 ; i>= nSize ; i--)
			{ m_ElemList.DeleteItem(i); }
		}
		nItem = m_ElemList.GetItemCount();
	}
	
	for(i=0 ; i<nSize ; i++)
	{	
		RsdlKey = keyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetRsdl(RsdlKey, RsdlData)) { ASSERT(0); continue; }

		lvitem.iItem = i;
		str = GetNumbericOptimizer(RsdlData.aElemList);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		for(int j=0; j < 1; j++)
		{
			lvitem.iSubItem = j;
			if     (j==0) str = GetNumbericOptimizer(RsdlData.aElemList);

			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;

			if (j == 0) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)RsdlKey;
				if(i >= nItem) m_ElemList.InsertItem(&lvitem);
				else           m_ElemList.SetItem(&lvitem);					
			}
			else m_ElemList.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		m_ElemList.SetItemData(i, (DWORD)RsdlKey);
	}
}

void CRCDesignSeismic::UpdateELinkList()
{
	T_ELNK_K LinkKey;
	T_ELNK_D LinkData;
	T_RSDM_K RsdmKey;
	BOOL bExistData;
	LVITEM lvitem;
	CString str;

	CArray<UINT, UINT> keyList;
	m_pDoc->m_pAttrCtrl->GetElnkKeyList(keyList);
	int nSize = keyList.GetSize();
	int nItem = m_ELinkList.GetItemCount();
	if(nSize != nItem) 
	{		
		if(nSize < nItem)
		{			
			for(int i=nItem-1 ; i>= nSize ; i--)
			{ m_ELinkList.DeleteItem(i); }
		}
		nItem = m_ELinkList.GetItemCount();
	}
	
	for(int i=0 ; i<nSize ; i++)
	{	
		LinkKey = keyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetElnk(LinkKey, LinkData)) { ASSERT(0); LinkData.Initialize(); }
		RsdmKey.key.OType = 2;
		RsdmKey.key.OKey  = LinkKey;
		bExistData = m_pDoc->m_pAttrCtrl->ExistRsdm(RsdmKey);

		lvitem.iItem = i;
		for(int j=0; j < 4; j++)
		{
			lvitem.iSubItem = j;
			if     (j==0) str.Format(_T("%d"), LinkKey);
			else if(j==1) str.Format(_T("%d"), LinkData.Node1);
			else if(j==2) str.Format(_T("%d"), LinkData.Node2);
			else if(j==3) str = bExistData ? _T("O") : _T("X");

			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;

			if (j == 0) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)LinkKey;
				if(i >= nItem) m_ELinkList.InsertItem(&lvitem);
				else           m_ELinkList.SetItem(&lvitem);
			}
			else m_ELinkList.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		m_ELinkList.SetItemData(i, (DWORD)LinkKey);
	}
}
void CRCDesignSeismic::UpdateGLinkList()
{
	T_NLNK_K LinkKey;
	T_NLNK_D LinkData;
	T_ELNK_K ELinkKey;
	T_ELNK_D ELinkData;

	T_RSDM_K RsdmKey;
	BOOL bExistData;
	LVITEM lvitem;
	CString str;

	CArray<UINT, UINT> keyList;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(keyList);

	CArray<UINT, UINT> keyList2;
	m_pDoc->m_pAttrCtrl->GetElnkKeyList(keyList2);

	int nSize = keyList.GetSize() + keyList2.GetSize();
	int nItem = m_GLinkList.GetItemCount();
	if(nSize != m_GLinkList.GetItemCount()) 
	{			
		if(nSize < nItem)
		{			
			for(int i=nItem ; i> nSize ; i--)
			{ m_GLinkList.DeleteItem(i); }
		}
	}

	
	for(int i=0 ; i<keyList.GetSize() ; i++)
	{	
		LinkKey = keyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetNlnk(LinkKey, LinkData)) { ASSERT(0); LinkData.Initialize(); }
		RsdmKey.key.OType = 3;
		RsdmKey.key.OKey  = LinkKey;
		bExistData = m_pDoc->m_pAttrCtrl->ExistRsdm(RsdmKey);

		lvitem.iItem = i;
		for(int j=0; j < 4; j++)
		{
			lvitem.iSubItem = j;
			if     (j==0) str.Format(_T("%d"), LinkKey);
			else if(j==1) str.Format(_T("%d"), LinkData.Node1);
			else if(j==2) str.Format(_T("%d"), LinkData.Node2);
			else if(j==3) str = bExistData ? _T("O") : _T("X");

			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;

			if (j == 0) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)LinkKey;
				if(i >= nItem) m_GLinkList.InsertItem(&lvitem);
				else           m_GLinkList.SetItem(&lvitem);
			}
			else m_GLinkList.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		m_GLinkList.SetItemData(i, (DWORD)LinkKey);
	}

	for(int i=0 ; i<keyList2.GetSize() ; i++)
	{	
		ELinkKey = keyList2.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetElnk(ELinkKey, ELinkData)) { ASSERT(0); LinkData.Initialize(); }
		RsdmKey.key.OType = 3;
		RsdmKey.key.OKey  = ELinkKey;
		bExistData = m_pDoc->m_pAttrCtrl->ExistRsdm(RsdmKey);
		
		lvitem.iItem = i;
		for(int j=0; j < 4; j++)
		{
			lvitem.iSubItem = j;
			if     (j==0) str.Format(_T("%d"), ELinkKey);
			else if(j==1) str.Format(_T("%d"), ELinkData.Node1);
			else if(j==2) str.Format(_T("%d"), ELinkData.Node2);
			else if(j==3) str = bExistData ? _T("O") : _T("X");
			
			lvitem.pszText = str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;
			
			if (j == 0) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)ELinkKey;
				if(i >= nItem) m_GLinkList.InsertItem(&lvitem);
				else           m_GLinkList.SetItem(&lvitem);
			}
			else m_GLinkList.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
		m_GLinkList.SetItemData(i, (DWORD)ELinkKey);
	}
}

void CRCDesignSeismic::Data2Dlg_Rsdm(T_RSDM_D& rData)
{
	m_nCurMemberType  = rData.nMemberType;
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		m_nMemberType = m_nCurMemberType - 10;
	}
	else
	{
		m_nMemberType = m_nCurMemberType;
	}
	
	m_nLinkOutType = rData.nOutputType;
	
	//m_nCurMemberType	0:Pier  1:Abutment  2:Foundation  3:Copping Beam  4:판식고무지지점  5:활동쟁반형지지점  6:고정쟁반형지지점  7:Main arch	
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		if(m_nCurMemberType==14)
		{// 판식고무지지점
			m_RubSupThickEdit.SetEditUnit(rData.dSupportRubberThick);
			m_RubSupFricEdit.SetEditUnit(rData.dSupportFrictionFactor);
		}
		else if(m_nCurMemberType==15)
		{// 활동쟁반형지지점
			m_ActSupHDisEdit.SetEditUnit(rData.dSupportAllowHorizonDis);
		}
		else if(m_nCurMemberType==16)
		{// 고정쟁반형지지점
			m_FixSupHLoadEdit.SetEditUnit(rData.dSupportMaxHorizonLoad);
	  }
	}
	else
	{
		if(m_nCurMemberType==4)
		{// 판식고무지지점
			m_RubSupThickEdit.SetEditUnit(rData.dSupportRubberThick);
			m_RubSupFricEdit.SetEditUnit(rData.dSupportFrictionFactor);
		}
		else if(m_nCurMemberType==5)
		{// 활동쟁반형지지점
			m_ActSupHDisEdit.SetEditUnit(rData.dSupportAllowHorizonDis);
		}
		else if(m_nCurMemberType==6)
		{// 고정쟁반형지지점
			m_FixSupHLoadEdit.SetEditUnit(rData.dSupportMaxHorizonLoad);
	}
	}
	
	UpdateData(FALSE);
//   if(m_iDgnCode == RC_CJJ_166_2011)
//   {
//     ChangeMemberType_CJJ();
//   }
//   else
//   {
//     ChangeMemberType();
//   }
}
void CRCDesignSeismic::Data2Dlg_Rsdl(T_RSDL_D& rData)
{	
	m_nCurMemberType  = rData.nMemberType;
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		m_nMemberType = m_nCurMemberType -10;
	}
	else
	{
		m_nMemberType = m_nCurMemberType;
	}
	m_nElemOutType = rData.nOutputType;
	
	//m_nCurMemberType	0:Pier  1:Abutment  2:Foundation  3:Copping Beam  4:판식고무지지점  5:활동쟁반형지지점  6:고정쟁반형지지점  7:Main arch	
	if(m_nCurMemberType==0)
	{// Pier
		m_PierHnEdit.SetEditUnit(rData.dPierHn);
		m_bDoubleColumnPier =	rData.bPierDColumnBent;
		m_PierAllowDispEdit.SetEditUnit(rData.dPierAllowDisp);
		m_nPierHingeType = rData.nPierPHingeRegionType;							
		int nSize = m_cmbMpcc.GetCount();
		for(int i=0 ; i<nSize ; i++)
		{
			if(rData.PierMpccKey == m_cmbMpcc.GetItemData(i))
			{ m_cmbMpcc.SetCurSel(i);  break; }
		}
		if(rData.PierMpccKey == 0)
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].Copy(rData.arPierUserMPhi[i]);
				m_dPierYieldCurvature[i]    = rData.dPierYieldCurvature[i];
				m_dPierUltimateCurvature[i] = rData.dPierUltimateCurvature[i];
			}
		}	
		else 
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].RemoveAll();
				m_dPierYieldCurvature[i]    = 0.0;
				m_dPierUltimateCurvature[i] = 0.0;
			}
		}

		m_strSelectElem = GetNumbericOptimizer(rData.aElemList);
		m_PierElemSelEdit.SelectByStr(m_strSelectElem);	
		m_strSelectNode = _T("");
		m_CoppingNodeSelEdit.SelectByStr(m_strSelectNode);
		
		UpdateData(FALSE);
		OnDoubleColumnPierChk();
	}
	else if(m_nCurMemberType==10)
	{// Pier
		m_SinglePierHnEdit.SetEditUnit(rData.dPierHn);
		//m_bDoubleColumnPier =	rData.bPierDColumnBent;
		//m_PierAllowDispEdit.SetEditUnit(rData.dPierAllowDisp);
		m_nPierHingeType = rData.nPierPHingeRegionType;							
		int nSize = m_cmbMpcc.GetCount();
		for(int i=0 ; i<nSize ; i++)
		{
			if(rData.PierMpccKey == m_cmbMpcc.GetItemData(i))
			{ m_cmbMpcc.SetCurSel(i);  break; }
		}
		if(rData.PierMpccKey == 0)
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].Copy(rData.arPierUserMPhi[i]);
				m_dPierYieldCurvature[i]    = rData.dPierYieldCurvature[i];
				m_dPierUltimateCurvature[i] = rData.dPierUltimateCurvature[i];
			}
		}	
		else 
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].RemoveAll();
				m_dPierYieldCurvature[i]    = 0.0;
				m_dPierUltimateCurvature[i] = 0.0;
			}
		}
		
		m_strSelectElem = GetNumbericOptimizer(rData.aElemList);
		m_PierElemSelEdit.SelectByStr(m_strSelectElem);	
		m_strSelectNode = _T("");
		m_CoppingNodeSelEdit.SelectByStr(m_strSelectNode);
		
		UpdateData(FALSE);
		//OnDoubleColumnPierChk();
	}
	else if(m_nCurMemberType==12)
	{// Pier
		m_PairPierHnEdit.SetEditUnit(rData.dPierHn);
		m_bDoubleColumnPier =	rData.bPierDColumnBent;
		//m_PierAllowDispEdit.SetEditUnit(rData.dPierAllowDisp);
		m_PairPierAllowDispEdit.SetEditUnit(rData.dPierAllowDisp);							
		int nSize = m_cmbMpcc.GetCount();
		for(int i=0 ; i<nSize ; i++)
		{
			if(rData.PierMpccKey == m_cmbMpcc.GetItemData(i))
			{ m_cmbMpcc.SetCurSel(i);  break; }
		}
		if(rData.PierMpccKey == 0)
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].Copy(rData.arPierUserMPhi[i]);
				m_dPierYieldCurvature[i]    = rData.dPierYieldCurvature[i];
				m_dPierUltimateCurvature[i] = rData.dPierUltimateCurvature[i];
			}
		}	
		else 
		{
			for(int i=0 ; i<2 ; i++)
			{
				m_arPierUserMPhi[i].RemoveAll();
				m_dPierYieldCurvature[i]    = 0.0;
				m_dPierUltimateCurvature[i] = 0.0;
			}
		}
		
		m_strSelectElem = GetNumbericOptimizer(rData.aElemList);
		m_PierElemSelEdit.SelectByStr(m_strSelectElem);	
		m_strSelectNode = _T("");
		m_CoppingNodeSelEdit.SelectByStr(m_strSelectNode);

		m_PairPierBOTMAXEdit.SetEditUnit(rData.dPireMaxMinForce[0]);
		m_PairPierBOTMINEdit.SetEditUnit(rData.dPireMaxMinForce[1]);
		m_PairPierTOPMAXEdit.SetEditUnit(rData.dPireMaxMinForce[2]);
		m_PairPierTOPMINEdit.SetEditUnit(rData.dPireMaxMinForce[3]);

		
		UpdateData(FALSE);
		//OnDoubleColumnPierChk();
	}
	else if(m_nCurMemberType==3 || m_nCurMemberType==13 )
	{// Copping Beam
		m_CoppingSpanEdit.SetEditUnit(rData.dCappingBeamSpan);
		m_strSelectElem = GetNumbericOptimizer(rData.aElemList);
		m_CoppingElemSelEdit.SelectByStr(m_strSelectElem);
		
		m_strSelectNode = GetNumbericOptimizer(rData.aNodeList);
		m_CoppingNodeSelEdit.SelectByStr(m_strSelectNode);
		
		UpdateData(FALSE);
	}
	/*
	if(m_iDgnCode == RC_CJJ_166_2011)
	{
		ChangeMemberType_CJJ();
	}
	else
	{
		ChangeMemberType();
	}
	*/
}

//////////////////////////////////////////////////////////////////////////

void CRCDesignSeismic::OnDgnCRCRsdmAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCDesignSeismic::OnDgnCRCRsdmDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

int CRCDesignSeismic::GetSelectRsdlKey(int nMemberType, CArray<T_ELEM_K, T_ELEM_K>& arElemKey, CArray<T_RSDL_K, T_RSDL_K>& arRsdlKey)
{
	m_pDoc->m_pAttrCtrl->GetRsdlKeyList(arRsdlKey);
	T_RSDL_D RsdlD;

	int i, j, k, nSize, nElemSize, nSelElemSize;
	nSelElemSize = arElemKey.GetSize();
	nSize = arRsdlKey.GetSize();	
	BOOL bFind;
	for(i=nSize-1 ; i>=0 ; i--)
	{
		if(!m_pDoc->m_pAttrCtrl->GetRsdl(arRsdlKey[i], RsdlD)) { arRsdlKey.RemoveAt(i);  continue; }
		bFind = FALSE;
		 //if(RsdlD.nMemberType == nMemberType)
		 {
		  nElemSize = RsdlD.aElemList.GetSize();
		  
		  for(j=0 ; j<nElemSize ; j++)
		  {
			  for(k=0 ; k<nSelElemSize ; k++)
			  {
				  if(arElemKey[k] == RsdlD.aElemList[j])
				  { bFind = TRUE;  break; }
			  }
			  if(bFind) break;
		  }
		 }
		 
		 
		if(!bFind){ arRsdlKey.RemoveAt(i); }
	}
	
	return (arRsdlKey.GetSize() > 0);
}

CString CRCDesignSeismic::GetNumbericOptimizer(CArray<UINT, UINT>& KeyList)
{
	// Optimize한다.
	CNumericOptimizer optimizer;
	return optimizer.OptimizeUnsortedArray(KeyList);
}

void CRCDesignSeismic::OnItemchangedElemList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	if(!m_bUpdateItemChanged) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_RSDL_K RsdlKey  = pNMListView->lParam; 

	T_RSDL_D RsdlData;
	if(m_pDoc->m_pAttrCtrl->GetRsdl(RsdlKey, RsdlData))
	{ Data2Dlg_Rsdl(RsdlData); }

	return;
}

void CRCDesignSeismic::OnItemchangedELinkList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	if(!m_bUpdateItemChanged) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_RSDM_K RsdmKey; 	
	RsdmKey.key.OType = 2;
	RsdmKey.key.OKey  = pNMListView->lParam; 

	T_RSDM_D RsdmData;
	if(m_pDoc->m_pAttrCtrl->GetRsdm(RsdmKey, RsdmData))
	{ Data2Dlg_Rsdm(RsdmData); }

	return;
}

void CRCDesignSeismic::OnItemchangedGLinkList(NMHDR* pNMHDR, LRESULT* pResult)
{	
	if(!m_bUpdateItemChanged) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_RSDM_K RsdmKey; 
	T_RSDM_D RsdmData;
	RsdmKey.key.OType = 3;
	RsdmKey.key.OKey  = pNMListView->lParam; 

	if(m_pDoc->m_pAttrCtrl->GetRsdm(RsdmKey, RsdmData))
	{ Data2Dlg_Rsdm(RsdmData); }

	return;
}

void CRCDesignSeismic::InitCtrl_CJJ()
{
	SetHeaderTitle();
	
	Initial_MpccCombo();
	
	m_PierElemSelEdit.Connect(SC_ID_ELEM, &m_PierElemSelEdit);
	m_CoppingElemSelEdit.Connect(SC_ID_ELEM, &m_CoppingElemSelEdit);	
	m_CoppingNodeSelEdit.Connect(SC_ID_NODE, &m_CoppingNodeSelEdit);	
	
	int nGap = globalUtils.ScaleByDPI(8);
	
	m_SinglePierHnUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SinglePierHnEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PairPierHnUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PairPierHnEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_PairPierAllowDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_PairPierAllowDispEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_PairPierBOTMAXUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierBOTMAXEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierBOTMINUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierBOTMINEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierTOPMAXUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierTOPMAXEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierTOPMINUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_PairPierTOPMINEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	

	m_FdnAGroundUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FdnAGroundEdit.SetUnitType(D_UNITSYS_BASE_STRESS);	
	m_CoppingSpanUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_CoppingSpanEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_RubSupThickUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RubSupThickEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RubSupFricEdit.SetUnitType(D_UNITSYS_NONE);
	m_ActSupHDisUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ActSupHDisEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_FixSupHLoadUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_FixSupHLoadEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	
	CRect RectOptOrg, RectPrintOrg, RectListOrg, RectListOrg2, RectButOrg, RectCrr, RectCrr2, RectCrr3;
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_VARIABLE_GROUP)->GetWindowRect(&RectOptOrg);

	GetDlgItem(IDC_DGN_RC_RSDM_SINGLEPIER_VARIABLE_GROUP)->GetWindowRect(&RectCrr);	
	CDlgUtil::CtrlMoveDistY(this, m_aSinglePierID, (RectOptOrg.top - RectCrr.top), TRUE);

	
	GetDlgItem(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_GROUP)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_MPHI_GROUP)->GetWindowRect(&RectCrr2);
	GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectCrr3);
	RectPrintOrg.top  = RectOptOrg.top + RectCrr.Height() + nGap;
	RectPrintOrg.bottom = RectPrintOrg.top + RectCrr2.Height()+ nGap+ RectCrr3.Height();
	RectPrintOrg.left = RectCrr3.left;
	RectPrintOrg.right = RectCrr3.right;

	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
	RectPrintOrg.top  = RectPrintOrg.bottom  + nGap;
	RectPrintOrg.bottom = RectPrintOrg.top + RectCrr.Height();
	RectPrintOrg.left = RectCrr.left;
	RectPrintOrg.right = RectCrr.right;
	
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST)->GetWindowRect(&RectCrr2);
	RectListOrg.top = RectPrintOrg.top + RectPrintOrg.Height() + nGap;
	RectListOrg.bottom = RectListOrg.top + (RectCrr2.bottom - RectCrr.top);
	RectListOrg.left = RectCrr2.left;
	RectListOrg.right = RectCrr2.right;
	
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr);
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST)->GetWindowRect(&RectCrr2);
	//GetDlgItem(IDC_DGN_RC_RSDM_PIER_ELEM_GROUP)->GetWindowRect(&RectCrr3);
	RectListOrg2.top    = RectPrintOrg.bottom + nGap ;	
	RectListOrg2.bottom = RectListOrg2.top + RectCrr.Height() +nGap +RectCrr2.Height();
	RectListOrg2.left   = RectCrr2.left;
	RectListOrg2.right  = RectCrr2.right;
	
	GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);	
	GetDlgItem(IDC_DGN_RSDM_CLOSE)->GetWindowRect(&RectCrr2);
	RectButOrg.top = RectListOrg2.bottom + nGap;
	RectButOrg.bottom = RectButOrg.top + RectCrr.Height();
	RectButOrg.left = RectCrr.left;
	RectButOrg.right = RectCrr2.right;
		
	
	GetDlgItem(IDC_DGN_RC_RSDM_PAIRPIER_VARIABLE_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aPairPierListID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_COPP_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aCoppingBeamID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPPL_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aRubSupportID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPAT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aActSupportID, (RectOptOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_SUPFI_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aFixSupportID, (RectOptOrg.top - RectCrr.top), TRUE);
	
	
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_PRINT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aElemPrintID, (RectPrintOrg.top - RectCrr.top), TRUE);
	GetDlgItem(IDC_DGN_RC_RSDM_LINK_PRINT_GROUP)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aLinkPrintID, (RectPrintOrg.top - RectCrr.top), TRUE);
	
	
	GetDlgItem(IDC_DGN_RC_RSDM_ELEM_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aElemListID, (RectListOrg2.top - RectCrr.top), TRUE);		
	GetDlgItem(IDC_DGN_RC_RSDM_ELINK_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aELinkListID, (RectListOrg.top - RectCrr.top), TRUE);	
	GetDlgItem(IDC_DGN_RC_RSDM_GLINK_LIST_STC)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aGLinkListID, (RectListOrg.top - RectCrr.top), TRUE);	
	
	GetDlgItem(IDC_DGN_RSDM_EXECUTE)->GetWindowRect(&RectCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlButtonID, (RectButOrg.top - RectCrr.top), TRUE);
	
	CRect RectDlg;
	GetWindowRect(&RectDlg);	
	SetWindowPos(NULL, 0, 0, RectDlg.Width(), (RectButOrg.bottom - RectDlg.top + 5), SWP_NOMOVE | SWP_NOZORDER);	
}

