// BndrSospDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ResponseDispLoadDlg.h"
#include "..\MIT_frx\MBarBaseDlg.h"
#include "..\wg_db\DB_ST_DT.h"
//#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_common\TBUtilFunc.h"
#include "ResponseDispLoadMode1Dlg.h"
#include "ResponseDispLoadMode2Dlg.h"
#include "ResponseDispLoadMode3Dlg.h"
#include "ResponseDispLoadMode4Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResponseDispLoadDlg dialog


CResponseDispLoadDlg::CResponseDispLoadDlg(CWnd* pParent /*=NULL*/)
		: CMenuBarChildDlg(CResponseDispLoadDlg::IDD, pParent)
{
		//{{AFX_DATA_INIT(CResponseDispLoadDlg)
		//}}AFX_DATA_INIT
		m_pDoc = CDBDoc::GetDocPoint();  
}

CResponseDispLoadDlg::~CResponseDispLoadDlg()
{
}

void CResponseDispLoadDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CResponseDispLoadDlg)  
		//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResponseDispLoadDlg, CMenuBarChildDlg)
		//{{AFX_MSG_MAP(CResponseDispLoadDlg)
		ON_BN_CLICKED(IDC_RADIO_RESPONS_LOAD_MODE1, OnChangeMode)
		ON_BN_CLICKED(IDC_RADIO_RESPONS_LOAD_MODE2, OnChangeMode)
		ON_BN_CLICKED(IDC_RADIO_RESPONS_LOAD_MODE3, OnChangeMode)
		ON_BN_CLICKED(IDC_RADIO_RESPONS_LOAD_MODE4, OnChangeMode)
		//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_STATIC_RESPONS_LOAD_MODE, &CResponseDispLoadDlg::OnBnClickedStaticResponsLoadMode)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CResponseDispLoadDlg message handlers

BOOL CResponseDispLoadDlg::OnInitDialog() 
{
		CMenuBarChildDlg::OnInitDialog();
		m_ChildDlgMgr.SetPlaceHolder(this, IDC_TREEMENU_PLACEHOLDER);
		CreateChildDlgs();	
		CDlgUtilEx::CtrlCheck(this, IDC_RADIO_RESPONS_LOAD_MODE1, TRUE);
		//ShowChildDlgs(0);
		m_ChildDlgMgr.ShowTab(0);
		return TRUE;  
}


BOOL	CResponseDispLoadDlg::CreateChildDlgs(void)
{    
		m_ChildDlgMgr.AddTab(new ResponseDispLoadMode1Dlg(this), _T("mode1"), ResponseDispLoadMode1Dlg::IDD, TRUE);
		m_ChildDlgMgr.AddTab(new ResponseDispLoadMode2Dlg(this), _T("mode2"), ResponseDispLoadMode2Dlg::IDD, TRUE);
		m_ChildDlgMgr.AddTab(new ResponseDispLoadMode3Dlg(this), _T("mode3"), ResponseDispLoadMode3Dlg::IDD, TRUE);
		m_ChildDlgMgr.AddTab(new ResponseDispLoadMode4Dlg(this), _T("mode4"), ResponseDispLoadMode4Dlg::IDD, TRUE);
		return TRUE;
}

BOOL	CResponseDispLoadDlg::ShowChildDlgs(int which)
{
		if(which == m_ChildDlgMgr.GetCurTab())
				return TRUE;
		m_ChildDlgMgr.ShowTab(which);
		return TRUE;
}

int  CResponseDispLoadDlg::GetCurMode()
{
		if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_RESPONS_LOAD_MODE1))
				return 0;
		else if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_RESPONS_LOAD_MODE2))
				return 1;
		else if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_RESPONS_LOAD_MODE3))
				return 2;
		else if(CDlgUtilEx::CtrlIsCheck(this, IDC_RADIO_RESPONS_LOAD_MODE4))
				return 3;
		return 0;
}

void CResponseDispLoadDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		HWND hWnd;
		if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

		switch(lHint)
		{
		case D_UPDATE_DBALL:
		case D_UPDATE_DEFAULT:
				break;
		case D_UPDATE_BUFFER_BEFORE:
				break;
		case D_UPDATE_BUFFER_AFTER:
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

void CResponseDispLoadDlg::OnChangeMode()
{
	int nMode = GetCurMode();
	ShowChildDlgs(nMode);
	CWnd *pParent = GetParent();
	while (pParent)
	{
		mit::frx::MBarBaseDlg *pDlg = dynamic_cast<mit::frx::MBarBaseDlg*>(pParent);
		if (pDlg)
		{
			pDlg->SetApplyBtnShowHide(nMode == 3 ? SW_HIDE : SW_SHOW);
			break;
		}
		pParent = pParent->GetParent();
	}
}

void CResponseDispLoadDlg::Execute() 
{
	CMenuBarChildDlg *pChild = dynamic_cast<CMenuBarChildDlg*>(m_ChildDlgMgr.GetTab(m_ChildDlgMgr.GetCurTab()));
	pChild->Execute();
}

void CResponseDispLoadDlg::OnBnClickedStaticResponsLoadMode()
{
		// TODO: 在此添加控件通知处理程序代码
}
