// DgnSteelRatingDlg.cpp: implementation of the CDgnSteelRatingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingDlg.h"
#include "DgnRCRatingOptionDlg.h"
#include "DgnRCRatingPrintDlg.h" 
#include "DgnRCRatingPlateDlg.h"
#include "DgnRCRatingPlateOptionDlg.h"
#include "DgnRCRatingPlatePrintDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_base\wg_base_MsgDll.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDgnRCRatingDlg, MChildFormView)

	CDgnRCRatingDlg::CDgnRCRatingDlg()
	: MChildFormView(CDgnRCRatingDlg::IDD)
{
	CreateChildDlgs();
}

CDgnRCRatingDlg::~CDgnRCRatingDlg()
{
	DeleteChildDlgs();
}

void CDgnRCRatingDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSteelRatingDlg)
	DDX_Control(pDX, IDC_DGN_RATING_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_RATING_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_RATING_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRCRatingDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnSteelRatingDlg)
	ON_CBN_SELCHANGE(IDC_DGN_RATING_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_RATING_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingDlg diagnostics

#ifdef _DEBUG
void CDgnRCRatingDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnRCRatingDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelRatingDlg message handlers


void CDgnRCRatingDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

#if defined(_CIVIL)	
	m_ChildDlgs.Add(new CDgnRCRatingOptionDlg);
	m_DlgIDs.Add(__TMMODE_DGN_RC_RATING_OPTION_BEAM__);
	m_TableIDs.Add(ID_QUERY_RC_RATING_OPTION);	

	m_ChildDlgs.Add(new CDgnRCRatingPrintDlg);
	m_DlgIDs.Add(__TMMODE_DGN_RC_RATING_PRINT_BEAM__);
	m_TableIDs.Add(ID_QUERY_RC_RATING_PRINT_OPTION);		

	m_ChildDlgs.Add(new CDgnRCRatingPlateDlg());
	m_DlgIDs.Add(__TMMODE_DGN_RC_RATING_DGN_OPTION_PLATE__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CDgnRCRatingPlateOptionDlg());
	m_DlgIDs.Add(__TMMODE_DGN_RC_RATING_OPTION_PLATE__);
	m_TableIDs.Add( ID_QUERY_RC_RATING_PLATE_OPTION);	

	m_ChildDlgs.Add(new CDgnRCRatingPlatePrintDlg());
	m_DlgIDs.Add(__TMMODE_DGN_RC_RATING_PRINT_PLATE__);
	m_TableIDs.Add( ID_QUERY_RC_RATING_PRINT_PLATE_OPTION);	

#endif
}

void CDgnRCRatingDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnRCRatingDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_RC_RATING__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i=0; i<nIDCount; i++)
	{
		if(nID==m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs[i] > 0)	
				SetTableBtnEnable();
			else									
				m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnRCRatingDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg==nDlgIndex)	return;

	if(nDlgIndex > m_ChildDlgs.GetSize()-1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();

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
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);
}

void CDgnRCRatingDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnRCRatingDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(bActivate)
	{
		SetTableBtnEnable();

		if(m_nCurChildDlg != -1)	
		{
			ShowCurChildDlg(m_nCurChildDlg);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[m_nCurChildDlg]);
		}
		else
		{
			ShowCurChildDlg(0);
			CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[0]);
		}
	}
}

void CDgnRCRatingDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnRCRatingDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnRCRatingDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);
}

int CDgnRCRatingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}

void CDgnRCRatingDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

void CDgnRCRatingDlg::Update_ComboMenu()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
#if defined(_CIVIL) 
	m_ComboMenu.ResetContent();
	m_ComboMenuStr.RemoveAll();

	T_RCDC_D RcdcD; RcdcD.Initialize();	
	pDoc->m_pAttrCtrl2->GetDgnRcdc(RcdcD);

	m_ComboMenuStr.SetSize(5);

	if(RcdcD.nRatingCode==KSCE_USD10_RC_RATING || RcdcD.nRatingCode==KSCE_RAIL_USD11_RC_RATING || RcdcD.nRatingCode==KSCE_LSD15_RC_RATING)
	{
		m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_RC_BEAM_RATING_OPTION);	 // _T("Rating Option(RC Beam)")
		m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_RC_BEAM_PRINT_OPTION);	 // _T("Print Option(RC Beam)")	
		m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_RC_PLATE_REBAR);	       // Rebar Input for Plate
		m_ComboMenuStr[3] = _LS(IDS_DGN_TREE_TEXT_RC_PLATE_RATING_OPTION); // Define Rating Option (Plate)
		m_ComboMenuStr[4] = _LS(IDS_DGN_TREE_TEXT_RC_PLATE_PRINT_OPTION);	 // Define Print Option (Plate)
	}
	else
	{
		//ASSERT(FALSE);
	}

	for (int i = 0; i < m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}

	int iSelID = m_ComboMenu.GetCurSel();
	if (iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else			m_ComboMenu.SetCurSel(iSelID);
#endif
}

void CDgnRCRatingDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			//DeleteChildDlgs();
			//CreateChildDlgs();
			// Add by ZINU.('01.1.2).
			Update_InitDataByCode(TRUE);
			break;
		}
	case D_UPDATE_UNIT:
		// do something...
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CDgnRCRatingDlg::Update_InitDataByCode(BOOL bUpdate)
{	
	if(bUpdate)
	{
		CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
		int nCount = pViewBuff->GetCount();
		if(nCount==0) return; 
		ASSERT(nCount==1);

		BOOL bCODE=FALSE;
		T_UDRD_BUFFER buffer_ur;
		POSITION pos = pViewBuff->GetStartBuffer();
		while(pos)
		{
			buffer_ur = pViewBuff->GetNextBuffer(pos);
			int nCmd = buffer_ur.nCmd;
			int nKey = buffer_ur.nKey;

			switch(nCmd)
			{
			case(UR_RCDC_ADD)://XXXXX
			case(UR_RCDC_DEL):	{bCODE = TRUE; break;}
			default:	break;
			}
		}
		if(!bCODE) return;
	}

	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();

	SetTableBtnEnable();
}

void CDgnRCRatingDlg::SetTableBtnEnable() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_RCDC_D Data;
	Data.Initialize();	
	pDoc->m_pAttrCtrl2->GetDgnRcdc(Data);

	if(Data.nRatingCode == KSCE_USD10_RC_RATING || Data.nRatingCode == KSCE_RAIL_USD11_RC_RATING || Data.nRatingCode == KSCE_LSD15_RC_RATING)
		m_wndTableBtn.EnableWindow(TRUE);
	else
		m_wndTableBtn.EnableWindow(FALSE);
}