// CmdAutoLoadCombCvlSP35133302011Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlSP35133302011Page.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlSP35133302011Page dialog
CCmdAutoLoadCombCvlSP35133302011Page::CCmdAutoLoadCombCvlSP35133302011Page(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlSP35133302011Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlSP35133302011Page)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

	m_nConstLoad = 0;

	m_nSP3511D = 0;
	m_nSP3511LIP = 0;
	m_nSP3511PL = 0;
	m_nSP3511BL = 0;
	m_nSP3511PS = 0;
	m_nSP3511EP = 0;
	m_nSP3511WP = 0;
	m_nSP3511CRSH = 0;
	m_nSP3511STL = 0;
}

void CCmdAutoLoadCombCvlSP35133302011Page::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlSP35133302011Page)  
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_BRIDGE_TYPE_CMB,   m_cmbBridgeType);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_D_MAX_RDO,         m_nSP3511D);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_D_MAX_EDT,         m_edtSP3511D_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_D_MIN_EDT,         m_edtSP3511D_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_LIP_MAX_RDO,       m_nSP3511LIP);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_LIP_MAX_EDT,       m_edtSP3511LIP_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_LIP_MIN_EDT,       m_edtSP3511LIP_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_PL_MAX_RDO,        m_nSP3511PL);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_PL_MAX_EDT,        m_edtSP3511PL_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_PL_MIN_EDT,        m_edtSP3511PL_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_BL_MAX_RDO,        m_nSP3511BL);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_BL_MAX_EDT,        m_edtSP3511BL_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_BL_MIN_EDT,        m_edtSP3511BL_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_PS_MAX_RDO,        m_nSP3511PS);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_PS_MAX_EDT,        m_edtSP3511PS_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_PS_MIN_EDT,        m_edtSP3511PS_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_EP_MAX_RDO,        m_nSP3511EP);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_EP_MAX_EDT,        m_edtSP3511EP_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_EP_MIN_EDT,        m_edtSP3511EP_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_WP_MAX_RDO,        m_nSP3511WP);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_WP_MAX_EDT,        m_edtSP3511WP_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_WP_MIN_EDT,        m_edtSP3511WP_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_CRSH_MAX_RDO,      m_nSP3511CRSH);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_CRSH_MAX_EDT,      m_edtSP3511CRSH_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_CRSH_MIN_EDT,      m_edtSP3511CRSH_Min);
	DDX_Radio  (pDX, IDC_CMD_SP35_13330_2011_STL_MAX_RDO,       m_nSP3511STL);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_STL_MAX_EDT,       m_edtSP3511STL_Max);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_STL_MIN_EDT,       m_edtSP3511STL_Min);
	DDX_Control(pDX, IDC_CMD_SP35_13330_2011_LOAD_FACTOR_GRP,   m_chkLoadFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlSP35133302011Page, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlSP35133302011Page)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlSP35133302011Page::InitCombo()
{
	m_cmbBridgeType.ResetContent();
	m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_ROADWAY));
	m_cmbBridgeType.AddString(_LS(IDS_CMD_LOAD_COMB_BRIDGE_TYPE_REILROAD));
	m_cmbBridgeType.SetCurSel(0);
}

void CCmdAutoLoadCombCvlSP35133302011Page::SetDefault()
{
	// Dead Weight 
	double dDMax = 1.1, dDMin = 0.9;
	m_edtSP3511D_Max.SetEditUnit(dDMax);
	m_edtSP3511D_Min.SetEditUnit(dDMin);

	// Weight of Leveling, Insulating, Protection
	double dLIPMax = 1.3, dLIPMin = 0.9;
	m_edtSP3511LIP_Max.SetEditUnit(dLIPMax);
	m_edtSP3511LIP_Min.SetEditUnit(dLIPMin);

	// Weight of Pavement
	double dPLMax = 1.5, dPLMin = 0.9;
	m_edtSP3511PL_Max.SetEditUnit(dPLMax);
	m_edtSP3511PL_Min.SetEditUnit(dPLMin);

	// Weight of Ballast
	double dBLMax = 1.3, dBLMin = 0.9;
	m_edtSP3511BL_Max.SetEditUnit(dBLMax);
	m_edtSP3511BL_Min.SetEditUnit(dBLMin);

	// Prestress
	double dPSMax = 1.1, dPSMin = 0.9;
	m_edtSP3511PS_Max.SetEditUnit(dPSMax);
	m_edtSP3511PS_Min.SetEditUnit(dPSMin);

	// Horizontal Soil Pressure
	double dEPMax = 1.4, dEPMin = 0.7;
	m_edtSP3511EP_Max.SetEditUnit(dEPMax);
	m_edtSP3511EP_Min.SetEditUnit(dEPMin);

	// Hydrostatic Pressure
	double dWPMax = 1.1, dWPMin = 0.9;
	m_edtSP3511WP_Max.SetEditUnit(dWPMax);
	m_edtSP3511WP_Min.SetEditUnit(dWPMin);

	// Creep and Shrinkage
	double dCRSHMax = 1.1, dCRSHMin = 0.9;
	m_edtSP3511CRSH_Max.SetEditUnit(dCRSHMax);
	m_edtSP3511CRSH_Min.SetEditUnit(dCRSHMin);

	// Soil Settlement
	double dSTLMax = 1.5, dSTLMin = 0.5;
	m_edtSP3511STL_Max.SetEditUnit(dSTLMax);
	m_edtSP3511STL_Min.SetEditUnit(dSTLMin);
}

void CCmdAutoLoadCombCvlSP35133302011Page::GetSP3511LoadFactor()
{
	UpdateData(TRUE);
	
	m_nBridgeType = m_cmbBridgeType.GetCurSel();

	// Dead Weight
	double dDMax = m_edtSP3511D_Max.GetEditValue();
	double dDMin = m_edtSP3511D_Min.GetEditValue();

	switch(m_nSP3511D)
	{
		case 0: m_nSP3511DNum = 1; m_dSP3511DMax = dDMax; m_dSP3511DMin = dDMax; break; 
		case 1: m_nSP3511DNum = 1; m_dSP3511DMax = dDMin; m_dSP3511DMin = dDMin; break; 
		case 2: m_nSP3511DNum = 2; m_dSP3511DMax = dDMax; m_dSP3511DMin = dDMin; break;
	}

	
	// Weight of Leveling, Insulating, Protection
	double dLIPMax = m_edtSP3511LIP_Max.GetEditValue();
	double dLIPMin = m_edtSP3511LIP_Min.GetEditValue();

	switch(m_nSP3511LIP)
	{
		case 0: m_nSP3511LIPNum = 1; m_dSP3511LIPMax = dLIPMax; m_dSP3511LIPMin = dLIPMax; break; 
		case 1: m_nSP3511LIPNum = 1; m_dSP3511LIPMax = dLIPMin; m_dSP3511LIPMin = dLIPMin; break; 
		case 2: m_nSP3511LIPNum = 2; m_dSP3511LIPMax = dLIPMax; m_dSP3511LIPMin = dLIPMin; break;
	}
	
	// Weight of Pavement
	double dPLMax = m_edtSP3511PL_Max.GetEditValue();
	double dPLMin = m_edtSP3511PL_Min.GetEditValue();
	
	switch(m_nSP3511PL)
	{
		case 0: m_nSP3511PLNum = 1; m_dSP3511PLMax = dPLMax; m_dSP3511PLMin = dPLMax; break; 
		case 1: m_nSP3511PLNum = 1; m_dSP3511PLMax = dPLMin; m_dSP3511PLMin = dPLMin; break; 
		case 2: m_nSP3511PLNum = 2; m_dSP3511PLMax = dPLMax; m_dSP3511PLMin = dPLMin; break;
	}

	// Weight of Ballast	
	double dBLMax = m_edtSP3511BL_Max.GetEditValue();
	double dBLMin = m_edtSP3511BL_Min.GetEditValue();
	
	switch(m_nSP3511BL)
	{
		case 0: m_nSP3511BLNum = 1; m_dSP3511BLMax = dBLMax; m_dSP3511BLMin = dBLMax; break; 
		case 1: m_nSP3511BLNum = 1; m_dSP3511BLMax = dBLMin; m_dSP3511BLMin = dBLMin; break; 
		case 2: m_nSP3511BLNum = 2; m_dSP3511BLMax = dBLMax; m_dSP3511BLMin = dBLMin; break;
	}
	
	// Prestress
	double dPSMax = m_edtSP3511PS_Max.GetEditValue();
	double dPSMin = m_edtSP3511PS_Min.GetEditValue();
	
	switch(m_nSP3511PS)
	{
		case 0: m_nSP3511PSNum = 1; m_dSP3511PSMax = dPSMax; m_dSP3511PSMin = dPSMax; break; 
		case 1: m_nSP3511PSNum = 1; m_dSP3511PSMax = dPSMin; m_dSP3511PSMin = dPSMin; break; 
		case 2: m_nSP3511PSNum = 2; m_dSP3511PSMax = dPSMax; m_dSP3511PSMin = dPSMin; break;
	}
	
	// Horizontal Soil Pressure
	double dEPMax = m_edtSP3511EP_Max.GetEditValue();
	double dEPMin = m_edtSP3511EP_Min.GetEditValue();
	
	switch(m_nSP3511EP)
	{
		case 0: m_nSP3511EPNum = 1; m_dSP3511EPMax = dEPMax; m_dSP3511EPMin = dEPMax; break; 
		case 1: m_nSP3511EPNum = 1; m_dSP3511EPMax = dEPMin; m_dSP3511EPMin = dEPMin; break; 
		case 2: m_nSP3511EPNum = 2; m_dSP3511EPMax = dEPMax; m_dSP3511EPMin = dEPMin; break;
	}
	
	// Hydrostatic Pressure
	double dWPMax = m_edtSP3511WP_Max.GetEditValue();
	double dWPMin = m_edtSP3511WP_Min.GetEditValue();
	
	switch(m_nSP3511WP)
	{
		case 0: m_nSP3511WPNum = 1; m_dSP3511WPMax = dWPMax; m_dSP3511WPMin = dWPMax; break; 
		case 1: m_nSP3511WPNum = 1; m_dSP3511WPMax = dWPMin; m_dSP3511WPMin = dWPMin; break; 
		case 2: m_nSP3511WPNum = 2; m_dSP3511WPMax = dWPMax; m_dSP3511WPMin = dWPMin; break;
	}

	// Creep and Shrinkage
	double dCRSHMax = m_edtSP3511CRSH_Max.GetEditValue();
	double dCRSHMin = m_edtSP3511CRSH_Min.GetEditValue();
	
	switch(m_nSP3511CRSH)
	{
		case 0: m_nSP3511CRSHNum = 1; m_dSP3511CRSHMax = dCRSHMax; m_dSP3511CRSHMin = dCRSHMax; break; 
		case 1: m_nSP3511CRSHNum = 1; m_dSP3511CRSHMax = dCRSHMin; m_dSP3511CRSHMin = dCRSHMin; break; 
		case 2: m_nSP3511CRSHNum = 2; m_dSP3511CRSHMax = dCRSHMax; m_dSP3511CRSHMin = dCRSHMin; break;
	}

	// Soil Settlement
	double dSTLMax = m_edtSP3511STL_Max.GetEditValue();
	double dSTLMin = m_edtSP3511STL_Min.GetEditValue();
	
	switch(m_nSP3511STL)
	{
		case 0: m_nSP3511STLNum = 1; m_dSP3511STLMax = dSTLMax; m_dSP3511STLMin = dSTLMax; break; 
		case 1: m_nSP3511STLNum = 1; m_dSP3511STLMax = dSTLMin; m_dSP3511STLMin = dSTLMin; break; 
		case 2: m_nSP3511STLNum = 2; m_dSP3511STLMax = dSTLMax; m_dSP3511STLMin = dSTLMin; break;
	}
}

void CCmdAutoLoadCombCvlSP35133302011Page::OnCmdSP3511EnableDisable()
{  
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	BOOL bDExist  = FALSE;
	BOOL bLIPExist  = FALSE;
	BOOL bPLExist  = FALSE;
	BOOL bBLExist  = FALSE;
	BOOL bPSExist = FALSE;
	BOOL bEPExist = FALSE;
	BOOL bWPExist = FALSE;
	BOOL bCRSHExist = FALSE;
	BOOL bSTLExist  = FALSE;

	// 항상 활성화 하도록 수정 
	bDExist  = TRUE;
	bLIPExist = TRUE;
	bPLExist = TRUE;
	bBLExist = TRUE;
	bPSExist = TRUE;
	bEPExist = TRUE;
	bWPExist = TRUE;
	bCRSHExist = TRUE;
	bSTLExist  = TRUE;

// 	if(iCount!=0)
// 	{
// 		for(int i=0; i<iCount; i++)
// 		{
// 			int iKey = arKeyList.GetAt(i);
// 			T_STLD_D rData;
// 			rData.Initialize();
// 			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
// 			ASSERT(bCheck);            
// 
// 			if     (rData.LoadCaseType==_T("D"))   bDExist    = TRUE;  // Dead Weight
// 			else if(rData.LoadCaseType==_T("LIP")) bLIPExist  = TRUE;  // Weight of Leveling, Insulating, Protection
// 			else if(rData.LoadCaseType==_T("PL"))  bPLExist   = TRUE;  // Weight of Pavement
// 			else if(rData.LoadCaseType==_T("BL"))  bBLExist   = TRUE;  // Weight of Ballast	
// 			else if(rData.LoadCaseType==_T("PS"))  bPSExist   = TRUE;  // Load due to Prestressing (including adjusting)
// 			else if(rData.LoadCaseType==_T("EP"))	 bEPExist   = TRUE;  // Soil Pressure due to Embankment Weight
// 			else if(rData.LoadCaseType==_T("EH"))	 bEPExist   = TRUE;  // Soil Pressure due to Embankment Weight
// 			else if(rData.LoadCaseType==_T("EV"))	 bDExist    = TRUE;  // Dead Weight 로 연결한다. 2013.09.25 Agree DKLee
// 			else if(rData.LoadCaseType==_T("ES"))	 bEPExist   = TRUE;  // Soil Pressure due to Embankment Weight
// 			else if(rData.LoadCaseType==_T("WP"))  bWPExist   = TRUE;  // Hydrostatic Pressure
// 			else if(rData.LoadCaseType==_T("CR"))  bCRSHExist = TRUE;  // Stress due to Concrete Creep and Shrinkage
// 			else if(rData.LoadCaseType==_T("SH"))  bCRSHExist = TRUE;  // Stress due to Concrete Creep and Shrinkage
// 			else if(rData.LoadCaseType==_T("STL")) bSTLExist  = TRUE;  // Stress due to Soil Settlement
//     }
//   }
// 
// 	//////////////////////////////////////////////////////////////////
// 	UpdateData(TRUE);
// 
// 	if(m_nConstLoad!=0)
// 	{
// 		CArray<T_SGLD_K,T_SGLD_K> aSgldK; aSgldK.RemoveAll();
// 		if(m_pDoc->IsPostMode()) m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(aSgldK);
// 		else                     m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldK);
// 		
// 		BOOL bCEL = FALSE;
// 		for(int iKey=0; iKey<aSgldK.GetSize(); iKey++)
// 		{
// 			T_SGLD_K SgldK = aSgldK.GetAt(iKey);
// 			T_SGLD_D SgldD; 
// 			SgldD.Initialize();
// 			m_pDoc->m_pAttrCtrl->GetSgld(SgldK, SgldD);
// 			if     (SgldD.LoadCaseName==_LS(IDS_DB_SGLD_DL)  && SgldK==1) { bDExist = TRUE; }
// 			else if(SgldD.LoadCaseName==_LS(IDS_DB_SGLD_EL1) && SgldK==2) { bCEL = TRUE; }
// 			else if(SgldD.LoadCaseName==_LS(IDS_DB_SGLD_CP)  && SgldK==5) { bCRSHExist  = TRUE; }
// 			else if(SgldD.LoadCaseName==_LS(IDS_DB_SGLD_CS)  && SgldK==6) { bCRSHExist  = TRUE; }
// 			else if(SgldD.LoadCaseName==_LS(IDS_DB_SGLD_SP)  && SgldK==7) { bCRSHExist  = TRUE; }
// 			else if(SgldD.LoadCaseName==_LS(IDS_DB_SGLD_SS)  && SgldK==8) { bCRSHExist  = TRUE; }
// 		}
// 		if(bCEL)
// 		{
// 			T_STCT_D StctD; StctD.Initialize();
// 			if(m_pDoc->m_pAttrCtrl->ExistStct()) m_pDoc->m_pAttrCtrl->GetStct(StctD);
// 			if     (StctD.LoadCaseType==_T("D"))	bDExist = TRUE;  // General Dead Load
// // 			else if(StctD.LoadCaseType==_T("DC"))	bDcExist = TRUE;  // Component and Attachments
// // 			else if(StctD.LoadCaseType==_T("DW"))	bDwExist = TRUE;  // Wearing Surfaces and Utilities
// //			else if(StctD.LoadCaseType==_T("L"))	bLExist  = TRUE;  // Live Load
// //			else if(StctD.LoadCaseType==_T("T"))	bTExist  = TRUE;	// 온도변화, 크리프, 건조수축 하중
// 		}
// 	}
// 	//////////////////////////////////////////////////////////////////
	
	GetDlgItem(IDC_CMD_SP35_13330_2011_D_MAX_RDO )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_D_MAX_EDT )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_D_MIN_RDO )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_D_MIN_EDT )->EnableWindow(bDExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_D_BOTH_RDO)->EnableWindow(bDExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_LIP_MAX_RDO )->EnableWindow(bLIPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_LIP_MAX_EDT )->EnableWindow(bLIPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_LIP_MIN_RDO )->EnableWindow(bLIPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_LIP_MIN_EDT )->EnableWindow(bLIPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_LIP_BOTH_RDO)->EnableWindow(bLIPExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_PL_MAX_RDO )->EnableWindow(bPLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PL_MAX_EDT )->EnableWindow(bPLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PL_MIN_RDO )->EnableWindow(bPLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PL_MIN_EDT )->EnableWindow(bPLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PL_BOTH_RDO)->EnableWindow(bPLExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_BL_MAX_RDO )->EnableWindow(bBLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_BL_MAX_EDT )->EnableWindow(bBLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_BL_MIN_RDO )->EnableWindow(bBLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_BL_MIN_EDT )->EnableWindow(bBLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_BL_BOTH_RDO)->EnableWindow(bBLExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_PS_MAX_RDO )->EnableWindow(bPSExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PS_MAX_EDT )->EnableWindow(bPSExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PS_MIN_RDO )->EnableWindow(bPSExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PS_MIN_EDT )->EnableWindow(bPSExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_PS_BOTH_RDO)->EnableWindow(bPSExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_EP_MAX_RDO )->EnableWindow(bEPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_EP_MAX_EDT )->EnableWindow(bEPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_EP_MIN_RDO )->EnableWindow(bEPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_EP_MIN_EDT )->EnableWindow(bEPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_EP_BOTH_RDO)->EnableWindow(bEPExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_WP_MAX_RDO )->EnableWindow(bWPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_WP_MAX_EDT )->EnableWindow(bWPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_WP_MIN_RDO )->EnableWindow(bWPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_WP_MIN_EDT )->EnableWindow(bWPExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_WP_BOTH_RDO)->EnableWindow(bWPExist);
		
	GetDlgItem(IDC_CMD_SP35_13330_2011_CRSH_MAX_RDO )->EnableWindow(bCRSHExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_CRSH_MAX_EDT )->EnableWindow(bCRSHExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_CRSH_MIN_RDO )->EnableWindow(bCRSHExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_CRSH_MIN_EDT )->EnableWindow(bCRSHExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_CRSH_BOTH_RDO)->EnableWindow(bCRSHExist);

	GetDlgItem(IDC_CMD_SP35_13330_2011_STL_MAX_RDO )->EnableWindow(bSTLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_STL_MAX_EDT )->EnableWindow(bSTLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_STL_MIN_RDO )->EnableWindow(bSTLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_STL_MIN_EDT )->EnableWindow(bSTLExist);
	GetDlgItem(IDC_CMD_SP35_13330_2011_STL_BOTH_RDO)->EnableWindow(bSTLExist);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlSP35133302011Page message handlers
BOOL CCmdAutoLoadCombCvlSP35133302011Page::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlSP35133302011Page::IDD, IDC_CMD_SP35_13330_2011_LOAD_FACTOR_GRP,TRUE,TRUE);

	InitCombo();
	SetDefault();

	UpdateData(FALSE);
	OnCmdSP3511EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
 
	return TRUE;
}

LRESULT CCmdAutoLoadCombCvlSP35133302011Page::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->AlignControl();

	return 0L;
}
