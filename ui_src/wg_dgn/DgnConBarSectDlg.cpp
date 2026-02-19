// DgnConBarSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarSectDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "DgnConBeamBarSectDlg.h"
#include "DgnConColumnBarSectDlg.h"
#include "DgnConBraceBarSectDlg.h"
#include "DgnConWallBarSectDlg.h"

#include "..\wg_main\wg_mainres2.h"	


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSectDlg dialog


CDgnConBarSectDlg::CDgnConBarSectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBarSectDlg::IDD, pParent)
{
	//////////////////////////////////////////////////////////////////
	//전역변수 초기화	
	m_nCurTab = 0;
	//////////////////////////////////////////////////////////////////
	

	//{{AFX_DATA_INIT(CRatingCaseDlg)

	//}}AFX_DATA_INIT

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConBarSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	

	//{{AFX_DATA_MAP(CDgnConBarSectDlg)
	DDX_Control(pDX, IDC_DGN_CON_BAR_SECT_TAB, m_Tab);
	//}}AFX_DATA_MAP
}

BOOL CDgnConBarSectDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitTabControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CDgnConBarSectDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBarSectDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CON_BAR_SECT_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_DGN_APPLY_BTN, OnDgnApply)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDgnConBarSectDlg::InitTabControl() 
{
	m_Tab.DeleteAllItems();
	TCHAR* aTitle[]  = {(TCHAR*)(LPCTSTR)_LS(IDS_DGN_BEAM), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_COLUMN), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_BRACE), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_WALL)};
	
	m_pSubDlg_Beam = new CDgnConBeamBarSectDlg;
	m_Tab.AddTab(m_pSubDlg_Beam, aTitle[BEAM_TAB], CDgnConBeamBarSectDlg::IDD, TRUE);

	m_pSubDlg_Column = new CDgnConColumnBarSectDlg;
	m_Tab.AddTab(m_pSubDlg_Column, aTitle[COLUMN_TAB], CDgnConColumnBarSectDlg::IDD, TRUE);

	m_pSubDlg_Brace = new CDgnConBraceBarSectDlg;
	m_Tab.AddTab(m_pSubDlg_Brace, aTitle[BRACE_TAB], CDgnConBraceBarSectDlg::IDD, TRUE);

	m_pSubDlg_Wall = new CDgnConWallBarSectDlg;
	m_Tab.AddTab(m_pSubDlg_Wall, aTitle[WALL_TAB], CDgnConWallBarSectDlg::IDD, TRUE);
	
	m_Tab.SetCurSel(m_nCurTab);
	m_Tab.ShowTab(m_nCurTab);
}


void CDgnConBarSectDlg::OnDgnApply() 
{
	if(m_Tab.GetCurSel() == 0)//Beam
		m_pSubDlg_Beam->ApplyData();
	else if(m_Tab.GetCurSel() == 1)//Column
		m_pSubDlg_Column->ApplyData();
	else if(m_Tab.GetCurSel() == 2)//Brace
		m_pSubDlg_Brace->ApplyData();
	else if(m_Tab.GetCurSel() == 3)//Wall
		m_pSubDlg_Wall->ApplyData();
	
	else ASSERT(0);	
}

void CDgnConBarSectDlg::InitChildAIJCtrl() 
{
	if(m_Tab.GetCurSel() == 0)//Beam
		m_pSubDlg_Beam->InitDlgData();
	else if(m_Tab.GetCurSel() == 1)//Column
		m_pSubDlg_Column->InitDlgData();
	else if(m_Tab.GetCurSel() == 2)//Brace
		m_pSubDlg_Brace->InitDlgData();
	else if(m_Tab.GetCurSel() == 3)//Wall
		m_pSubDlg_Wall->InitDlgData();
	
	else ASSERT(0);	
}

void CDgnConBarSectDlg::SetRebarCombo() 
{
	if(m_Tab.GetCurSel() == 0)//Beam
		m_pSubDlg_Beam->SetRebarCombo();
	else if(m_Tab.GetCurSel() == 1)//Column
		m_pSubDlg_Column->SetRebarCombo();
	else if(m_Tab.GetCurSel() == 2)//Brace
		m_pSubDlg_Brace->SetRebarCombo();
	else if(m_Tab.GetCurSel() == 3)//Wall
		m_pSubDlg_Wall->SetRebarCombo();
	
	else ASSERT(0);	
}

void CDgnConBarSectDlg::SetInitData() 
{
	if(m_Tab.GetCurSel() == 0)//Beam
		m_pSubDlg_Beam->InitDlgData();
	else if(m_Tab.GetCurSel() == 1)//Column
		m_pSubDlg_Column->InitDlgData();
	else if(m_Tab.GetCurSel() == 2)//Brace
		m_pSubDlg_Brace->InitDlgData();
	else if(m_Tab.GetCurSel() == 3)//Wall
		m_pSubDlg_Wall->InitDlgData();
	
	else ASSERT(0);	
}


void CDgnConBarSectDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConBarSectDlg::SetTabOrder(int iTab) 
{
	m_nCurTab = iTab;	
}

void CDgnConBarSectDlg::ChangeCurrentTab(int nTabIndex)
{
	m_nCurTab = nTabIndex;
	m_Tab.SetCurSel(m_nCurTab);
	m_Tab.ShowTab(m_nCurTab);
}

void CDgnConBarSectDlg::Initial_Data()
{
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnConBarSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				Update_InitDataByCode();
				UpdateBuffer();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		case D_UPDATE_PREF_CHANGED:
			SetRebarCombo();
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnConBarSectDlg::Update_InitDataByCode()
{	
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	//메인에 있는 메뉴 아이디로 판단하는듯
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_REBAR_MEMBER, strTitle, bShow, m_pDoc->IsPostMode());
	/*
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_MRFT_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_MRFT_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_MRFT_FACTOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_MRFT_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_MRFT_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_MRFT_FACTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}*/
}


void CDgnConBarSectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_DCON_ADD)://코드 변경시
		case(UR_DCON_DEL): 
			{
				//DestroyWindow();
				InitChildAIJCtrl();
				return;
			}
		case(UR_DCRB_ADD)://전체 철근데이터 변경시
		case(UR_DCRB_DEL): 
			{				
				SetInitData();
				return;
			}
		default:
		  break;
		}
	}

	if(m_Tab.GetCurSel() == WALL_TAB)
	{
		if(!m_pDoc->m_pPostCtrl->IsPostEnable())
			GetDlgItem(IDC_DGN_APPLY_BTN)->EnableWindow(FALSE);	
	}
}

void CDgnConBarSectDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nTab = m_Tab.GetCurSel();

	GetDlgItem(IDC_DGN_APPLY_BTN)->EnableWindow(TRUE);
	
	if(nTab == WALL_TAB)
	{
		if(!m_pDoc->m_pPostCtrl->IsPostEnable())
			GetDlgItem(IDC_DGN_APPLY_BTN)->EnableWindow(FALSE);	
	}
}