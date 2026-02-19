// DgnRatingDlg.cpp: implementation of the CDgnRatingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingDlg.h"

#include "DgnRatingOptionDlg.h"
#include "DgnRatingOptionAASHTODlg.h"
#include "DgnRatingPrintDlg.h"
#include "ExposureClassDlg.h"
#include "DgnPSCAssessmentOptionDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CDgnRatingDlg, MChildFormView)

CDgnRatingDlg::CDgnRatingDlg()
 : MChildFormView(CDgnRatingDlg::IDD)
{
	CreateChildDlgs();
}

CDgnRatingDlg::~CDgnRatingDlg()
{
	DeleteChildDlgs();
}

void CDgnRatingDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRatingDlg)
	DDX_Control(pDX, IDC_DGN_RATING_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_RATING_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_RATING_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRatingDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnRatingDlg)
	ON_CBN_SELCHANGE(IDC_DGN_RATING_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_RATING_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingDlg diagnostics

#ifdef _DEBUG
void CDgnRatingDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnRatingDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingDlg message handlers


void CDgnRatingDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

#if defined(_CIVIL)	

#if defined(_US)// US경우 한국어로 된 대화창을 안보이게 하기 위함 _US
	
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RACD_D Data;
	Data.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRacd(Data);
	int nRatingDesignCode = Data.nRatingCode;
	switch (nRatingDesignCode)
	{
	case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
		{
            CreateChildDlgsAASHTO();
			break;
		}
    case CS_454_20_PSC_RATING:
		{
            CreateChildDlgsBS();
            break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
	m_ChildDlgs.Add(new CDgnRationPrintDlg);
	m_DlgIDs.Add(__TMMODE_DGN_RATING_PRINT__);
	m_TableIDs.Add(ID_QUERY_RATING_PRINT_OPTION);		
#elif defined(_ORG)|| defined(_JP)

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RACD_D Data;
	Data.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRacd(Data);
	int nRatingDesignCode = Data.nRatingCode;
	switch (nRatingDesignCode)
	{
	case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
		{
            CreateChildDlgsAASHTO();
			break;
		}
	case KSCE_USD05_RATING:
	case KSCE_USD10_RATING:
	case KSCE_RAIL_USD11_RATING:
	case KSCE_LSD15_RATING:
		{
            CreateChildDlgsKSCE();
			break;
		}
    case CS_454_20_PSC_RATING:
		{
            CreateChildDlgsBS();
			break;	
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}

	m_ChildDlgs.Add(new CDgnRationPrintDlg);
	m_DlgIDs.Add(__TMMODE_DGN_RATING_PRINT__);
	m_TableIDs.Add(ID_QUERY_RATING_PRINT_OPTION);	
#endif

#endif
}

void CDgnRatingDlg::CreateChildDlgsKSCE()
{
    m_ChildDlgs.Add(new CDgnRationOptionDlg);
    m_DlgIDs.Add(__TMMODE_DGN_RATING_OPTION__);
    m_TableIDs.Add(ID_QUERY_RATING_DESIGN_OPTION);
}

void CDgnRatingDlg::CreateChildDlgsAASHTO()
{
    m_ChildDlgs.Add(new CDgnRatingOptionAASHTODlg);
    m_DlgIDs.Add(__TMMODE_DGN_RATING_OPTION__);
    m_TableIDs.Add(ID_QUERY_RATING_DESIGN_OPTION);
}

void CDgnRatingDlg::CreateChildDlgsBS()
{
    m_ChildDlgs.Add(new CExposureClassDlg(this, 1));
    m_DlgIDs.Add(__TMMODE_DGN_RATING_PSC_ENVIRONMENT_CLASS__);
    m_TableIDs.Add(ID_QUERY_RATING_PSC_ENVIRONMENT_CLASS);

    m_ChildDlgs.Add(new CDgnPSCAssessmentOptionDlg);
    m_DlgIDs.Add(__TMMODE_DGN_PSC_ASSESSMENT_OPTION__);
    m_TableIDs.Add(ID_QUERY_ASSESSMENT_DESIGN_OPTION);
}


void CDgnRatingDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnRatingDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_RATING__)
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

void CDgnRatingDlg::ShowCurChildDlg(int nDlgIndex)
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

void CDgnRatingDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	DeleteChildDlgs();
	CreateChildDlgs();

	Update_ComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CDgnRatingDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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

void CDgnRatingDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnRatingDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);

	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnRatingDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);
}

int CDgnRatingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnRatingDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Change by ZINU.('01.8.1). To change Dynamic Combo Menu.
void CDgnRatingDlg::Update_ComboMenu()
{
#if defined(_CIVIL) 
	m_ComboMenu.ResetContent();
	m_ComboMenuStr.RemoveAll();

	T_RACD_D Data;  Data.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

    switch ( Data.nRatingCode )
    {
    case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
        Update_ComboMenuAASHTO();
        break;
    case KSCE_USD05_RATING:
    case KSCE_USD10_RATING:
    case KSCE_RAIL_USD11_RATING:
    case KSCE_LSD15_RATING:
        Update_ComboMenuKSCE();
        break;
    case CS_454_20_PSC_RATING:
        Update_ComboMenuBS();
        break;
    default:
        ASSERT(0);
        break;
    }

	for (int i = 0; i < m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}

	int iSelID = m_ComboMenu.GetCurSel();
	if (iSelID < 0)
	{
		ShowCurChildDlg(0);
		m_ComboMenu.SetCurSel(0);
	}
	else
	{
		ShowCurChildDlg(iSelID);
		m_ComboMenu.SetCurSel(iSelID);
	}
#endif
}

void CDgnRatingDlg::Update_ComboMenuAASHTO()
{
    m_ComboMenuStr.SetSize(2);
    m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_DIAGNOSTIC_TEST_RESULTS);	//_T("Diagnostic Test Results")
    m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_POSITION_RATING_OUTPUT);	//_T("Position for Rating Output")
}

void CDgnRatingDlg::Update_ComboMenuKSCE()
{
    m_ComboMenuStr.SetSize(2);
    m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_RATING_OPTION);	//_T("Rating Option(PSC)")
    m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_PRINT_OPTION);	//_T("Print Option(PSC)")
}

void CDgnRatingDlg::Update_ComboMenuBS()
{
    m_ComboMenuStr.SetSize(3);
    m_ComboMenuStr[0] = _LS(IDS_DGN_TREE_TEXT_RATING_PSC_ENVIRONMENT_TYPE);
    m_ComboMenuStr[1] = _LS(IDS_DGN_TREE_TEXT_ASSESSMENT_OPTION);
    m_ComboMenuStr[2] = _LS(IDS_DGN_TREE_TEXT_ASSESSMENT_PRINT_OPTION);
}

void CDgnRatingDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnRatingDlg::Update_InitDataByCode(BOOL bUpdate)
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
				case(UR_RACD_ADD)://XXXXX
				case(UR_RACD_DEL):	{bCODE = TRUE; break;}
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

void CDgnRatingDlg::SetTableBtnEnable() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_RACD_D Data;
	Data.Initialize();
	pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

    switch ( Data.nRatingCode )
    {
    case KSCE_USD05_RATING:
    case KSCE_USD10_RATING:
    case KSCE_RAIL_USD11_RATING:
    case KSCE_LSD15_RATING:
    case AASHTO_LRFD12_PSC_RATING:
    case CS_454_20_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
        m_wndTableBtn.EnableWindow(TRUE);
        break;
    default:
        m_wndTableBtn.EnableWindow(FALSE);
        break;
    }
}