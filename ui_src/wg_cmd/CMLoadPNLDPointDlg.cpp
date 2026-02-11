// CMLoadPNLDPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadPNLDPointDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointDlg dialog


CCMLoadPNLDPointDlg::CCMLoadPNLDPointDlg(T_PNLD_D *pData,CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMLoadPNLDPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadPNLDPointDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pData);
	m_pData = NULL;
	m_pData = pData;
	m_bInit = FALSE; 
}


void CCMLoadPNLDPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadPNLDPointDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_PLLD_PTGRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadPNLDPointDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMLoadPNLDPointDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointDlg message handlers


void	CCMLoadPNLDPointDlg::SetData2Dlg(void)
{
	m_Data.aPointLoad.RemoveAll();	
	m_Data.aPointLoad.Copy(this->m_pData->aPointLoad);
	m_wndGrid.CancelEdit();
	m_wndGrid.MakeItemEx();
}

BOOL	CCMLoadPNLDPointDlg::SetDlg2Data(void)
{
	if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
	this->m_pData->aPointLoad.RemoveAll();
	this->m_pData->aPointLoad.Copy(m_Data.aPointLoad);
	return TRUE;
}

void	CCMLoadPNLDPointDlg::InitUnit(void)
{
}

BOOL CCMLoadPNLDPointDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}

BOOL CCMLoadPNLDPointDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	m_bInit=TRUE; 
	m_Data.Initialize();
	m_wndGrid.Initialize(&m_Data);
	m_wndGrid.SetScrollBarMode(SB_VERT|SB_HORZ, gxnEnabled | gxnEnhanced);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadPNLDPointDlg::UpdateUnit(void)
{
	//if(m_wndGrid.GetCountData())m_wndGrid.UpdateUnit(); 
	//if(m_wndGrid.GetCountData())m_wndGrid.UnitChanged(CDBDoc::GetDocPoint());
	m_wndGrid.UnitChanged(CDBDoc::GetDocPoint());
}
