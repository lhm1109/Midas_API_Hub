// DgnRCDesignParamBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCDesignParamBaseDlg.h"
#include "DgnRCDesignParamMatDlg.h" 
#include "DgnRCDesignParamParamDlg.h"
#include "DgnDataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_main\wg_main.h"

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamBaseDlg dialog


DgnRCDesignParamBaseDlg::DgnRCDesignParamBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(DgnRCDesignParamBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DgnRCDesignParamBaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMatDlg = 0;
 	m_pParamDlg =0;
}
DgnRCDesignParamBaseDlg::~DgnRCDesignParamBaseDlg()
{
	/*if(m_pMatDlg != 0){ delete m_pMatDlg; m_pMatDlg = 0;}
 	if(m_pParamDlg != 0){ delete m_pParamDlg; m_pParamDlg = 0;}*/
}

void DgnRCDesignParamBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DgnRCDesignParamBaseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TAB_RC_DESIGN, m_ctrlTab);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(DgnRCDesignParamBaseDlg, CDialogMove)
ON_BN_CLICKED(ID_RC_CLOSE, OnClose)  
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RC_DESIGN, OnSelchangeCmdTab)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamBaseDlg message handlers

void DgnRCDesignParamBaseDlg::LayoutChildDlg(){
	CWnd* pPlaceHolder = GetDlgItem(IDC_BASE_PLACE_HOLDER);//get text properties
	ASSERT(pPlaceHolder);
 	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pParamDlg = new DgnRCDesignParamParamDlg(this);
	m_ctrlTab.AddTab(m_pParamDlg,_LS(IDS_DGN_PSC_RPTJTG_Design_Parameter), DgnRCDesignParamParamDlg::IDD, TRUE);
	m_pMatDlg = new DgnRCDesignParamMatDlg(this);
	m_ctrlTab.AddTab(m_pMatDlg, _LS(IDS_DGN_PSC_RPTJTG_Material_Param), DgnRCDesignParamMatDlg::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);
	m_ctrlTab.DelTab(2);
	m_ctrlTab.DelTab(3);
	m_ctrlTab.DelTab(4);

}

// close
void DgnRCDesignParamBaseDlg::OnClose() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();	
//  DestroyWindow();
}

// close
/*BOOL DgnRCDesignParamBaseDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/**/
	/*CCheckDialogOpen::ResetSoftwareRendering();
	//m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS);
//	return CDgnDlgBase::DestroyWindow();
	return TRUE;
}*/
/*void DgnRCDesignParamBaseDlg::PostNcDestroy() 
{
//  if(m_pMatDlg != NULL){ delete m_pMatDlg; m_pMatDlg=NULL; }
//  if(m_pParamDlg != NULL){ delete m_pParamDlg; m_pParamDlg=NULL; }
//  CDgnDlgBase::PostNcDestroy();
}
*/
// add/replace
void DgnRCDesignParamBaseDlg::OnOK() 
{
	int iTabIndex = m_ctrlTab.GetCurSel();
	//DB에 LoadCase 정보가 없다면
	bool bExistDB_LoadCase = false;

	if(iTabIndex == 0)//현재 활성화된 텝이 Beam
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Beam입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pParamDlg->SaveOrUpdate();
		}
	}
	else//현재 활서화된 텝이 Column
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Column입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pMatDlg->SaveOrUpdate();
		}
	}
	//설계 입력창을 띄어야함
}

BOOL DgnRCDesignParamBaseDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	/**/
	CCheckDialogOpen::SetSoftwareRendering();
	
//	// Change by ZINU.('06.02.28). Auto-Setting Code by KSCE-USD05.
//	CString strCode = CDBLib::GetConCodeName();
//	T_DCON_D DconD;
//	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDcon(DconD))	DconD.Initialize();
//	if(!(strCode==_T("KSCE-USD05") && DconD.bSpecialSeismic==FALSE))
//	{
//		DconD.DesignCode = _T("KSCE-USD05");
//		DconD.bSpecialSeismic = FALSE;
//		CDgnDataCtrl DataCtrl;
//		double dPhi[5];
//		if(!DataCtrl.Get_DgnConPhiDef(DconD.DesignCode, dPhi))	ASSERT(0);
//		// Set Data.
//	  for(int i=0; i<5; i++)	DconD.dPhi[i] = dPhi[i];
//		if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddDcon(DconD))	ASSERT(0);
// 	}

	//this->LayoutChildDlg();
	LayoutChildDlg();
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void DgnRCDesignParamBaseDlg::SaveOrUpdate()
{
#if defined (_CIVIL)
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_MODS_DESIGN_CODE, 0), 0);  // Design Code 콤보리스트 설정함.
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_MODS_RATING, 0), 0);  // Rating 콤보리스트 설정함.
#endif

	CDialogMove::OnOK();
}
