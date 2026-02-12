// CmdAutoLoadCombCvlCSAPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlCSAPage.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlCSAEPDlg.h"
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
// CCmdAutoLoadCombCvlCSAPage dialog
CCmdAutoLoadCombCvlCSAPage::CCmdAutoLoadCombCvlCSAPage(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlCSAPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlCSAPage)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;
    m_nConstLoad = 0.0;
	m_nDesignType = 0;

	m_nCSAD = 0;
	m_nCSADW = 0;
	m_nCSAEP1 = 0;
	m_nCSAEP2 = 0;
	m_nCSAEP3 = 0;
	m_nCSAEP4 = 0;
	m_nCSAWP = 0;
	m_nCSAPS = 0;
}

void CCmdAutoLoadCombCvlCSAPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlCSAPage)  
	DDX_Radio  (pDX, IDC_CMD_CSA_D_MAX_RDO,      m_nCSAD);
	DDX_Radio  (pDX, IDC_CMD_CSA_DW_MAX_RDO,     m_nCSADW);
	DDX_Radio  (pDX, IDC_CMD_CSA_EP1_MAX_RDO,    m_nCSAEP1);
	DDX_Radio  (pDX, IDC_CMD_CSA_EP2_MAX_RDO,    m_nCSAEP2);
	DDX_Radio  (pDX, IDC_CMD_CSA_EP3_MAX_RDO,    m_nCSAEP3);
	DDX_Radio  (pDX, IDC_CMD_CSA_EP4_MAX_RDO,    m_nCSAEP4);
	DDX_Radio  (pDX, IDC_CMD_CSA_WP_MAX_RDO,     m_nCSAWP);
	DDX_Radio  (pDX, IDC_CMD_CSA_PS_MAX_RDO,     m_nCSAPS);
	
	DDX_Control(pDX, IDC_CMD_CSA_LOAD_FACTOR_GRP,           m_chkLoadFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlCSAPage, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlCSAPage)
	ON_BN_CLICKED(IDC_CMD_CSA_EP_BTN,  OnCmdCSAEPBtn)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlCSAPage::GetCSALoadFactor()
{
	UpdateData(TRUE);

	switch(m_nCSAD)  // Dead Load
	{
	case 0: 
		if(m_nDesignType == 0) { m_nCSADNum = 1; m_dCSADMax = 1.10; m_dCSADMin = 1.10; }
		else                   { m_nCSADNum = 1; m_dCSADMax = 1.20; m_dCSADMin = 1.20; }
		break; 
	case 1: 
		if(m_nDesignType == 0) { m_nCSADNum = 1; m_dCSADMax = 0.95; m_dCSADMin = 0.95; }
		else                   { m_nCSADNum = 1; m_dCSADMax = 0.90; m_dCSADMin = 0.90; }
		break; 
	case 2: 
		if(m_nDesignType == 0) { m_nCSADNum = 2; m_dCSADMax = 1.10; m_dCSADMin = 0.95; }
		else                   { m_nCSADNum = 2; m_dCSADMax = 1.20; m_dCSADMin = 0.90; }
		break; 
	}
	switch(m_nCSADW)  // Wearing Surfaces
	{
	case 0: m_nCSADWNum = 1; m_dCSADWMax = 1.50; m_dCSADWMin = 1.50; break; 
	case 1: m_nCSADWNum = 1; m_dCSADWMax = 0.65; m_dCSADWMin = 0.65; break; 
	case 2: m_nCSADWNum = 2; m_dCSADWMax = 1.50; m_dCSADWMin = 0.65; break;
	}
	switch(m_nCSAEP1)  // Passive Earth Pressure
	{
	case 0: m_nCSAEP1Num = 1; m_dCSAEP1Max = 1.25; m_dCSAEP1Min = 1.25; break; 
	case 1: m_nCSAEP1Num = 1; m_dCSAEP1Max = 0.50; m_dCSAEP1Min = 0.50; break; 
	case 2: m_nCSAEP1Num = 2; m_dCSAEP1Max = 1.25; m_dCSAEP1Min = 0.50; break;
	}
	switch(m_nCSAEP2)  // At-rest Earth Pressure
	{
	case 0: m_nCSAEP2Num = 1; m_dCSAEP2Max = 1.25; m_dCSAEP2Min = 1.25; break; 
	case 1: m_nCSAEP2Num = 1; m_dCSAEP2Max = 0.80; m_dCSAEP2Min = 0.80; break; 
	case 2: m_nCSAEP2Num = 2; m_dCSAEP2Max = 1.25; m_dCSAEP2Min = 0.80; break;
	}
	switch(m_nCSAEP3)  // Active Earth Pressure
	{
	case 0: m_nCSAEP3Num = 1; m_dCSAEP3Max = 1.25; m_dCSAEP3Min = 1.25; break; 
	case 1: m_nCSAEP3Num = 1; m_dCSAEP3Max = 0.80; m_dCSAEP3Min = 0.80; break; 
	case 2: m_nCSAEP3Num = 2; m_dCSAEP3Max = 1.25; m_dCSAEP3Min = 0.80; break;
	}
	switch(m_nCSAEP4)  // Backfill Pressure
	{
	case 0: m_nCSAEP4Num = 1; m_dCSAEP4Max = 1.25; m_dCSAEP4Min = 1.25; break; 
	case 1: m_nCSAEP4Num = 1; m_dCSAEP4Max = 0.80; m_dCSAEP4Min = 0.80; break; 
	case 2: m_nCSAEP4Num = 2; m_dCSAEP4Max = 1.25; m_dCSAEP4Min = 0.80; break;
	}
	switch(m_nCSAWP)  // Hydrostatic Pressure
	{
	case 0: m_nCSAWPNum = 1; m_dCSAWPMax = 1.10; m_dCSAWPMin = 1.10; break; 
	case 1: m_nCSAWPNum = 1; m_dCSAWPMax = 0.90; m_dCSAWPMin = 0.90; break; 
	case 2: m_nCSAWPNum = 2; m_dCSAWPMax = 1.10; m_dCSAWPMin = 0.90; break;
	}
	switch(m_nCSAPS)  // Secondary Prestress Effects
	{
	case 0: m_nCSAPSNum = 1; m_dCSAPSMax = 1.05; m_dCSAPSMin = 1.05; break; 
	case 1: m_nCSAPSNum = 1; m_dCSAPSMax = 0.95; m_dCSAPSMin = 0.95; break; 
	case 2: m_nCSAPSNum = 2; m_dCSAPSMax = 1.05; m_dCSAPSMin = 0.95; break;
	}
}

void CCmdAutoLoadCombCvlCSAPage::OnCmdCSAEnableDisable()
{  
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bDcExist = FALSE;
	BOOL bDdExist = FALSE;
	BOOL bDwExist = FALSE;
	BOOL bEpExist = FALSE;
	BOOL bWpExist = FALSE;
	BOOL bPsExist = FALSE;

	if(iCount!=0)
	{
		for(int i=0; i<iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            
			if     (rData.LoadCaseType==_T("D"))	 bDcExist = TRUE;  // General Dead Load
			else if(rData.LoadCaseType==_T("DC"))	 bDcExist = TRUE;  // Component and Attachments
			else if(rData.LoadCaseType==_T("DW"))	 bDwExist = TRUE;  // Wearing Surfaces and Utilities
			else if(rData.LoadCaseType==_T("EP"))	 bEpExist = TRUE;  // General earth pressure load
			else if(rData.LoadCaseType==_T("WP"))  bWpExist = TRUE;  // Ground Water pressure
			else if(rData.LoadCaseType==_T("PS"))  bPsExist = TRUE;  // Prestress
		}
	}

//////////////////////////////////////////////////////////////////
	UpdateData(TRUE);
    if ( m_nConstLoad!=0 )
    {
        CArray<T_SGLD_K, T_SGLD_K> aSgldK; aSgldK.RemoveAll();
        if ( m_pDoc->IsPostMode() )
        {
            m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(aSgldK);
        }
        else
        {
            m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldK);
        }
            
        T_STCT_D StctD; StctD.Initialize();
        if ( m_pDoc->m_pAttrCtrl->ExistStct() ) m_pDoc->m_pAttrCtrl->GetStct(StctD);

        BOOL bCEL = FALSE;
        for ( int iKey=0; iKey<aSgldK.GetSize(); iKey++ )
        {
            T_SGLD_K SgldK = aSgldK.GetAt(iKey);
            if ( SgldK==D_SGLD_DL_CVL ) { bDcExist = TRUE; }
			else if (SgldK >= D_SGLD_ER1_CVL && SgldK <= D_SGLD_ER20_CVL) { bCEL = TRUE; }
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

        }

        if ( bCEL )
        {
            for ( int i=0; i<StctD.aErection.GetSize(); i++ )
            {
                if ( StctD.aErection[i].LoadCaseType==_T("D") )	bDcExist = TRUE;  // General Dead Load
                else if ( StctD.aErection[i].LoadCaseType==_T("DC") )	bDcExist = TRUE;  // Component and Attachments
                else if ( StctD.aErection[i].LoadCaseType==_T("DW") )	bDwExist = TRUE;  // Wearing Surfaces and Utilities
            }
        }
    }

	GetDlgItem(IDC_CMD_CSA_D_MAX_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_CSA_D_MIN_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_CSA_D_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_CMD_CSA_DW_MAX_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_CSA_DW_MIN_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_CSA_DW_BOTH_RDO)->EnableWindow(bDwExist);

	GetDlgItem(IDC_CMD_CSA_EP_BTN)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP1_MAX_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP1_MIN_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP1_BOTH_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP2_MAX_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP2_MIN_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP2_BOTH_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP3_MAX_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP3_MIN_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP3_BOTH_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP4_MAX_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP4_MIN_RDO)->EnableWindow(bEpExist);
	GetDlgItem(IDC_CMD_CSA_EP4_BOTH_RDO)->EnableWindow(bEpExist);

	GetDlgItem(IDC_CMD_CSA_WP_MAX_RDO)->EnableWindow(bWpExist);
	GetDlgItem(IDC_CMD_CSA_WP_MIN_RDO)->EnableWindow(bWpExist);
	GetDlgItem(IDC_CMD_CSA_WP_BOTH_RDO)->EnableWindow(bWpExist);

	GetDlgItem(IDC_CMD_CSA_PS_MAX_RDO)->EnableWindow(bPsExist);
	GetDlgItem(IDC_CMD_CSA_PS_MIN_RDO)->EnableWindow(bPsExist);
	GetDlgItem(IDC_CMD_CSA_PS_BOTH_RDO)->EnableWindow(bPsExist);
}

void CCmdAutoLoadCombCvlCSAPage::ChangeText(int nDesignType)
{
	m_nDesignType = nDesignType;

	if(nDesignType == 0) // Steel
	{
		GetDlgItem(IDC_CMD_CSA_D_MAX_RDO)->SetWindowText(_T("1.10"));
		GetDlgItem(IDC_CMD_CSA_D_MIN_RDO)->SetWindowText(_T("0.95"));
	}
	else if(nDesignType == 1) // Concrete
	{
		GetDlgItem(IDC_CMD_CSA_D_MAX_RDO)->SetWindowText(_T("1.20"));
		GetDlgItem(IDC_CMD_CSA_D_MIN_RDO)->SetWindowText(_T("0.90"));
	}
	else
	{
		// Nothing
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlCSAPage message handlers
BOOL CCmdAutoLoadCombCvlCSAPage::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlCSAPage::IDD, IDC_CMD_CSA_LOAD_FACTOR_GRP,TRUE,TRUE);

	UpdateData(FALSE);
	OnCmdCSAEnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화
 
	return TRUE;
}

void CCmdAutoLoadCombCvlCSAPage::OnCmdCSAEPBtn()
{
	CCmdAutoLoadCombCvlCSAEPDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		m_aStldEp1.Copy(dlg.m_aStldEp1);
		m_aStldEp2.Copy(dlg.m_aStldEp2);
		m_aStldEp3.Copy(dlg.m_aStldEp3);
		m_aStldEp4.Copy(dlg.m_aStldEp4);
	}
}

LRESULT CCmdAutoLoadCombCvlCSAPage::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->AlignControl();

	return 0L;
}
