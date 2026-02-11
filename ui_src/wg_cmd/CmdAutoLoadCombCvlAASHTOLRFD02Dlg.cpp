// CmdAutoLoadCombCvlAASHTOLRFD02Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD02Dlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
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
// CCmdAutoLoadCombCvlAASHTOLRFD02Dlg dialog
CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::CCmdAutoLoadCombCvlAASHTOLRFD02Dlg(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    m_nDcRdo = 0;
    m_nDdRdo = 0;
    m_nDeformationRdo = 1;
    m_nDwRdo = 0;
    m_nEhTypeRdo = 0;
    m_nEh1Rdo = 0;
    m_nEh2Rdo = 0;
    m_nEsRdo = 0;
    m_nEvTypeRdo = 0;
    m_nEv2Rdo = 0;
    m_nEv3Rdo = 0;
    m_nEv4Rdo = 0;
    m_nEv5Rdo = 0;
    m_nEv6Rdo = 0;


}

void CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)  

    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD98_SETTLEMENT_EDIT, m_wndSettlement);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD98_LOAD_MODIFIER_EDIT, m_wndModifier);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD98_CULVERT_CHK, m_wndCulvertChk);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_DC_MAX_RDO, m_nDcRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_DD_MAX_RDO, m_nDdRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_DEFORMATION_RDO1, m_nDeformationRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_DW_MAX_RDO, m_nDwRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EH_RDO1, m_nEhTypeRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EH1_MAX_RDO, m_nEh1Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EH2_MAX_RDO, m_nEh2Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_ES_MAX_RDO, m_nEsRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV_RDO1, m_nEvTypeRdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV2_MAX_RDO, m_nEv2Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV3_MAX_RDO, m_nEv3Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV4_MAX_RDO, m_nEv4Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV5_MAX_RDO, m_nEv5Rdo);
    DDX_Radio(pDX, IDC_CMD_AASHTO_LRFD98_EV6_MAX_RDO, m_nEv6Rdo);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD98_LOAD_FACTOR_FRAME, m_chkLoadFactor);


    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlAASHTOLRFD02Dlg)

    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EH_RDO1, OnCmdAASHTOLRFD02EhRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EH_RDO2, OnCmdAASHTOLRFD02EhRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO1, OnCmdAASHTOLRFD02EvRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO2, OnCmdAASHTOLRFD02EvRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO3, OnCmdAASHTOLRFD02EvRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO4, OnCmdAASHTOLRFD02EvRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO5, OnCmdAASHTOLRFD02EvRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD98_EV_RDO6, OnCmdAASHTOLRFD02EvRdo)

    //}}AFX_MSG_MAP

    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

void CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::GetAASHTOLRFD02LoadFactor()
{
    UpdateData(TRUE);

    switch(m_nDcRdo)  // components and attachments
    {
    case 0: m_nAshLr98DcNum = 1; m_dAshLr98Dc1 = 1.25; m_dAshLr98Dc2 = 1.25; break; 
    case 1: m_nAshLr98DcNum = 1; m_dAshLr98Dc1 = 0.90; m_dAshLr98Dc2 = 0.90; break; 
    case 2: m_nAshLr98DcNum = 2; m_dAshLr98Dc1 = 1.25; m_dAshLr98Dc2 = 0.90; break;
    }
    switch(m_nDdRdo)  // downdrag
    {
    case 0: m_nAshLr98DdNum = 1; m_dAshLr98Dd1 = 1.80; m_dAshLr98Dd2 = 1.80; break; 
    case 1: m_nAshLr98DdNum = 1; m_dAshLr98Dd1 = 0.45; m_dAshLr98Dd2 = 0.45; break; 
    case 2: m_nAshLr98DdNum = 2; m_dAshLr98Dd1 = 1.80; m_dAshLr98Dd2 = 0.45; break;
    }
    switch(m_nDwRdo)  // wearing surfaces and utilities
    {
    case 0: m_nAshLr98DwNum = 1; m_dAshLr98Dw1 = 1.50; m_dAshLr98Dw2 = 1.50; break; 
    case 1: m_nAshLr98DwNum = 1; m_dAshLr98Dw1 = 0.65; m_dAshLr98Dw2 = 0.65; break; 
    case 2: m_nAshLr98DwNum = 2; m_dAshLr98Dw1 = 1.50; m_dAshLr98Dw2 = 0.65; break;
    }  
    switch(m_nEhTypeRdo)  // horizontal earth pressure
    {
    case 0:   // active
        if(m_nEh1Rdo == 0)      {m_nAshLr98EhNum = 1; m_dAshLr98Eh1 = 1.50; m_dAshLr98Eh2 = 1.50;} 
        else if(m_nEh1Rdo == 1) {m_nAshLr98EhNum = 1; m_dAshLr98Eh1 = 0.90; m_dAshLr98Eh2 = 0.90;} 
        else if(m_nEh1Rdo == 2) {m_nAshLr98EhNum = 2; m_dAshLr98Eh1 = 1.50; m_dAshLr98Eh2 = 0.90;}      
        break;
    case 1:  // at-rest 
        if(m_nEh2Rdo == 0)      {m_nAshLr98EhNum = 1; m_dAshLr98Eh1 = 1.35; m_dAshLr98Eh2 = 1.35;} 
        else if(m_nEh2Rdo == 1) {m_nAshLr98EhNum = 1; m_dAshLr98Eh1 = 0.90; m_dAshLr98Eh2 = 0.90;} 
        else if(m_nEh2Rdo == 2) {m_nAshLr98EhNum = 2; m_dAshLr98Eh1 = 1.35; m_dAshLr98Eh2 = 0.90;}            
        break;
    }
    m_nAshLr98ElNum = 1; m_dAshLr98El1 = 1.00; m_dAshLr98El2 = 1.00;  // locked-in erection stresses    
    switch(m_nEvTypeRdo)
    {
    case 0: // Overall Stability
        m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.00; m_dAshLr98Ev2 = 1.00;      
        break;
    case 1: // Retaining Walls and Abutments
        if(m_nEv2Rdo == 0)      {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.30; m_dAshLr98Ev2 = 1.30;}
        else if(m_nEv2Rdo == 1) {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 0.90; m_dAshLr98Ev2 = 0.90;} 
        else if(m_nEv2Rdo == 2) {m_nAshLr98EvNum = 2; m_dAshLr98Ev1 = 1.30; m_dAshLr98Ev2 = 0.90;}      
        break;
    case 2:  // Rigid Buried Structure
        if(m_nEv3Rdo == 0)      {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.35; m_dAshLr98Ev2 = 1.35;} 
        else if(m_nEv3Rdo == 1) {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 0.90; m_dAshLr98Ev2 = 0.90;} 
        else if(m_nEv3Rdo == 2) {m_nAshLr98EvNum = 2; m_dAshLr98Ev1 = 1.35; m_dAshLr98Ev2 = 0.90;}      
        break;
    case 3:  // Rigid Frames
        if(m_nEv4Rdo == 0)      {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.95; m_dAshLr98Ev2 = 1.95;} 
        else if(m_nEv4Rdo == 1) {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 0.90; m_dAshLr98Ev2 = 0.90;} 
        else if(m_nEv4Rdo == 2) {m_nAshLr98EvNum = 2; m_dAshLr98Ev1 = 1.95; m_dAshLr98Ev2 = 0.90;}      
        break;
    case 4:  // Flexible buried structures other than metal box culverts
        if(m_nEv5Rdo == 0)      {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.50; m_dAshLr98Ev2 = 1.50;} 
        else if(m_nEv5Rdo == 1) {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 0.90; m_dAshLr98Ev2 = 0.90;} 
        else if(m_nEv5Rdo == 2) {m_nAshLr98EvNum = 2; m_dAshLr98Ev1 = 1.50; m_dAshLr98Ev2 = 0.90;}      
        break;
    case 5:  // flexible metal box culverts
        if(m_nEv6Rdo == 0)      {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 1.50; m_dAshLr98Ev2 = 1.50;} 
        else if(m_nEv6Rdo == 1) {m_nAshLr98EvNum = 1; m_dAshLr98Ev1 = 0.90; m_dAshLr98Ev2 = 0.90;} 
        else if(m_nEv6Rdo == 2) {m_nAshLr98EvNum = 2; m_dAshLr98Ev1 = 1.50; m_dAshLr98Ev2 = 0.90;}      
        break;
    }
    switch(m_nEsRdo) // earth surcharge
    {
    case 0: m_nAshLr98EsNum = 1; m_dAshLr98Es1 = 1.50; m_dAshLr98Es2 = 1.50; break; 
    case 1: m_nAshLr98EsNum = 1; m_dAshLr98Es1 = 0.75; m_dAshLr98Es2 = 0.75; break; 
    case 2: m_nAshLr98EsNum = 2; m_dAshLr98Es1 = 1.50; m_dAshLr98Es2 = 0.75; break;
    }  
}

void CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::OnCmdAASHTOLRFD02EnableDisable()
{  
	CArray <unsigned int, unsigned int> arMLoad;   // Moving Load
	CArray <unsigned int, unsigned int> arSmLoad;  // Settlement (not static)

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bDcExist = FALSE;
	BOOL bDdExist = FALSE;
	BOOL bDwExist = FALSE;
	BOOL bEhExist = FALSE;
	BOOL bEvExist = FALSE;
	BOOL bEsExist = FALSE;
	BOOL bLExist  = FALSE;
	BOOL bTExist  = FALSE;  
	BOOL bStlExist= FALSE;

	if(iCount!=0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            

			if     (rData.LoadCaseType==_T("D"))	 bDcExist = TRUE;  // General Dead Load
			else if(rData.LoadCaseType==_T("DC"))	 bDcExist = TRUE;  // Component and Attachments
			else if(rData.LoadCaseType==_T("DD"))	 bDdExist = TRUE;  // Downdrag
			else if(rData.LoadCaseType==_T("DW"))	 bDwExist = TRUE;  // Wearing Surfaces and Utilities
			else if(rData.LoadCaseType==_T("EP"))	 bEhExist = TRUE;  // General earth pressure load
			else if(rData.LoadCaseType==_T("EH"))	 bEhExist = TRUE;  // horizontal earth pressure load
			else if(rData.LoadCaseType==_T("EV"))	 bEvExist = TRUE;  // vertical earth pressure load
			else if(rData.LoadCaseType==_T("ES"))	 bEsExist = TRUE;  // Earth Surcharge      
			else if(rData.LoadCaseType==_T("L"))	 bLExist  = TRUE;  // Live Load
			else if(rData.LoadCaseType==_T("T"))	 bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("CR"))	 bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("SH"))	 bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			else if(rData.LoadCaseType==_T("STL"))   bStlExist= TRUE;  // 부등침하 하중      
		}
		m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmLoad);

		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);
			break;
		case D_MOVE_CODE_CHINA:
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad);
			break;
		case D_MOVE_CODE_JAPAN:
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad);
			break;
		case D_MOVE_CODE_INDIA:
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad);
			break;
		case D_MOVE_CODE_BS:
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMLoad);
			break;
		case D_MOVE_CODE_FRANCE:
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMLoad);
			break;
		case D_MOVE_CODE_TRANS:
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMLoad);
			break;
		default:
			ASSERT(0);
			break;
		}
		/*
		int nMovingType = CProduct::GetMovingType();
		switch(nMovingType)
		{
		case D_PRODUCT_MOVING_ORG: m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);   break;
		case D_PRODUCT_MOVING_JP:  m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad); break;
		case D_PRODUCT_MOVING_CH:  m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad); break;
		case D_PRODUCT_MOVING_US:  
		if(CProduct::IsIndia()) {m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad); break;}
		else                    {m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);   break;}
		}
		*/
		if(arMLoad.GetSize() > 0)  bLExist = TRUE;  
		if(arSmLoad.GetSize() > 0) bStlExist = TRUE;
	}
	////////////////////////////////////////////////////////////////// Add by KJH20050104, requester:강희정
	UpdateData(TRUE);
	if(m_nConstLoad!=0)
	{
		CArray<T_SGLD_K,T_SGLD_K> aSgldK; aSgldK.RemoveAll();
		if(m_pDoc->IsPostMode()) m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(aSgldK);
		else                     m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldK);

		T_STCT_D StctD; StctD.Initialize();
		if(m_pDoc->m_pAttrCtrl->ExistStct()) m_pDoc->m_pAttrCtrl->GetStct(StctD);

		BOOL bCEL = FALSE;
		for(int iKey=0; iKey<aSgldK.GetSize(); iKey++)
		{
			T_SGLD_K SgldK = aSgldK.GetAt(iKey);
			if     (SgldK==D_SGLD_DL_CVL)    { bDcExist = TRUE; }
			else if(SgldK >=D_SGLD_ER1_CVL && SgldK <= D_SGLD_ER20_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER1_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER2_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER3_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER4_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER5_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER6_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER7_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER8_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER9_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER10_CVL) { bCEL = TRUE; }
			else if(SgldK==D_SGLD_CP_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_CS_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SP_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SS_CVL)    { bTExist  = TRUE; }
		}

		if(bCEL)
		{
			for(int i=0; i<StctD.aErection.GetSize(); i++)
			{
				if     (StctD.aErection[i].LoadCaseType==_T("D"))	bDcExist = TRUE;  // General Dead Load
				else if(StctD.aErection[i].LoadCaseType==_T("DC"))	bDcExist = TRUE;  // Component and Attachments
				else if(StctD.aErection[i].LoadCaseType==_T("DW"))	bDwExist = TRUE;  // Wearing Surfaces and Utilities
				else if(StctD.aErection[i].LoadCaseType==_T("L"))	bLExist  = TRUE;  // Live Load
				else if(StctD.aErection[i].LoadCaseType==_T("T"))	bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			}			
		}
	}
	//////////////////////////////////////////////////////////////////

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DC_MAX_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DC_MIN_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DC_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DD_MAX_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DD_MIN_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DD_BOTH_RDO)->EnableWindow(bDdExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DW_MAX_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DW_MIN_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DW_BOTH_RDO)->EnableWindow(bDwExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH_RDO1)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH_RDO2)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_MAX_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_MIN_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_BOTH_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_MAX_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_MIN_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_BOTH_RDO)->EnableWindow(bEhExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO1)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO2)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO3)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO4)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO5)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV_RDO6)->EnableWindow(bEvExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD98_ES_MAX_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_ES_MIN_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_ES_BOTH_RDO)->EnableWindow(bEsExist);

	m_wndCulvertChk.EnableWindow(bLExist);
	m_wndSettlement.EnableWindow(bStlExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DEFORMATION_RDO1)->EnableWindow(bTExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD98_DEFORMATION_RDO2)->EnableWindow(bTExist);
}


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD02Dlg message handlers
BOOL CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::IDD, IDC_CMD_AASHTO_LRFD98_LOAD_FACTOR_FRAME,TRUE,TRUE);

    m_wndModifier.SetEditUnit(1.0);
    m_wndSettlement.SetEditUnit(1.0);

    UpdateData(FALSE);

    OnCmdAASHTOLRFD02EhRdo();
    OnCmdAASHTOLRFD02EvRdo();
    OnCmdAASHTOLRFD02EnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

    return TRUE;
}

void CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::OnCmdAASHTOLRFD02EhRdo() 
{
    UpdateData(TRUE);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_MAX_RDO)->EnableWindow(m_nEhTypeRdo==0);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_MIN_RDO)->EnableWindow(m_nEhTypeRdo==0);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH1_BOTH_RDO)->EnableWindow(m_nEhTypeRdo==0);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_MAX_RDO)->EnableWindow(m_nEhTypeRdo==1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_MIN_RDO)->EnableWindow(m_nEhTypeRdo==1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EH2_BOTH_RDO)->EnableWindow(m_nEhTypeRdo==1);	  
}

void CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::OnCmdAASHTOLRFD02EvRdo() 
{
    UpdateData(TRUE);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV2_MAX_RDO)->EnableWindow(m_nEvTypeRdo==1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV2_MIN_RDO)->EnableWindow(m_nEvTypeRdo==1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV2_BOTH_RDO)->EnableWindow(m_nEvTypeRdo==1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV3_MAX_RDO)->EnableWindow(m_nEvTypeRdo==2);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV3_MIN_RDO)->EnableWindow(m_nEvTypeRdo==2);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV3_BOTH_RDO)->EnableWindow(m_nEvTypeRdo==2);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV4_MAX_RDO)->EnableWindow(m_nEvTypeRdo==3);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV4_MIN_RDO)->EnableWindow(m_nEvTypeRdo==3);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV4_BOTH_RDO)->EnableWindow(m_nEvTypeRdo==3);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV5_MAX_RDO)->EnableWindow(m_nEvTypeRdo==4);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV5_MIN_RDO)->EnableWindow(m_nEvTypeRdo==4);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV5_BOTH_RDO)->EnableWindow(m_nEvTypeRdo==4);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV6_MAX_RDO)->EnableWindow(m_nEvTypeRdo==5);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV6_MIN_RDO)->EnableWindow(m_nEvTypeRdo==5);
    GetDlgItem(IDC_CMD_AASHTO_LRFD98_EV6_BOTH_RDO)->EnableWindow(m_nEvTypeRdo==5);		
}

LRESULT CCmdAutoLoadCombCvlAASHTOLRFD02Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}