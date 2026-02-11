// StructBndrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructBndrDlg.h"

// Dialog�߰��� 3�� ���� : '(����)'�̶�� �ڸ�Ʈ ģ ��
// ���� : ���� ���߾� �� ��

// (����)
#include "BndrSkewDlg.h"
#include "BndrConsDlg.h"
#include "BndrNsprDlg.h"
#include "BndrSprsDlg.h"
#include "BndrElnkDlg.h"
#include "BndrReltDlg.h"
#include "BndrFrlsDlg.h"
#include "BndrOffsDlg.h"
#include "BndrPrlsDlg.h"
#include "BndrRigdDlg.h"
#include "BndrDrlsDlg.h"
#include "BndrIelcDlg.h"
#include "BndrSsprDlg.h"
#include "BndrSospDlg.h"
#include "BndrIntegralDlg.h" //061110 //mylee
#include "BngrChangeDlg.h"
#include "BndrStoryDlg.h"
#include "BndrNLLinkDlg.h"
#include "BndrCglpDlg.h"
#include "BndrMconDlg.h"     // [2007-09-03] Lee, Jong Hyup (Tel: 2145, jhlee@midasit.com)
#include "BndrCldrDlg.h"     // Define Constraint Label Direction, 20081104 KIMJM
#include "LinkRenumberDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\mit_frx\MBarBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructBndrDlg

IMPLEMENT_DYNCREATE(CStructBndrDlg, MChildFormView)

#if defined(_CIVIL)
CStructBndrDlg::CStructBndrDlg()
	: MChildFormView(CStructBndrDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructBndrDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		int nMode;
	};
	// (����) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_BNDR_CONS__ },
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_BNDR_NSPR__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_GSPR__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_SSPR__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_ELNK__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_RIGD__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_NLLINK__ },
		{ D_PROD_GEN+D_PROD_CVL						 , __TMMODE_STRUCT_BNDR_CGLP__ }, // change general link property...
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_FRLS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_OFFS__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_PRLS__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_SOSP__ },
		{            D_PROD_CVL            , __TMMODE_STRUCT_BNDR_RELT__ },
		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_MCON__ },  // [2007-09-03] Lee, Jong Hyup (Tel: 2145, jhlee@midasit.com)
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_BNDR_CLDR__ }, // Define Constraint Label Direction, 20081104 KIMJM
		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_BNGR_CHANGE__ },		
};
#else
CStructBndrDlg::CStructBndrDlg()
	: MChildFormView(CStructBndrDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructBndrDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		int nMode;
	};
	// (����) // redesigned by J.B.Seon 2002.05.22
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , __TMMODE_STRUCT_BNDR_CONS__ },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , __TMMODE_STRUCT_BNDR_NSPR__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_GSPR__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_SSPR__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_SOSP__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_ELNK__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_NLLINK__ },
		{ D_PROD_GEN + D_PROD_CVL						 , __TMMODE_STRUCT_BNDR_CGLP__ }, // change general link property...
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_FRLS__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_OFFS__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_PRLS__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_RIGD__ },
		{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_MCON__ },  // [2007-09-03] Lee, Jong Hyup (Tel: 2145, jhlee@midasit.com)
		{ D_PROD_GEN                       , __TMMODE_STRUCT_BNDR_DRLS__ },
		{ D_PROD_GEN                       , __TMMODE_STRUCT_BNDR_STORY__ },
		//		{ D_PROD_GEN+D_PROD_CVL+D_PROD_TOW , __TMMODE_STRUCT_BNDR_SKEW__ },
				{            D_PROD_CVL            , __TMMODE_STRUCT_BNDR_EWSF__ },
				{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , __TMMODE_STRUCT_BNDR_CLDR__ }, // Define Constraint Label Direction, 20081104 KIMJM
				{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , __TMMODE_STRUCT_BNGR_CHANGE__ },
				// 		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_LINK_COMPACT__},
				#ifdef _MGEN_CH
						{ D_PROD_GEN + D_PROD_CVL            , __TMMODE_STRUCT_BNDR_LINK_RENUMBER__},
				#endif
						//		{ D_PROD_GEN+D_PROD_CVL            , __TMMODE_STRUCT_BNDR_IELC__ },
								{            D_PROD_CVL            , __TMMODE_STRUCT_BNDR_RELT__ },
	};
#endif

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	int i, nCount;
	nCount = sizeof(aItem)/sizeof(T_COMBO_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aItem[i].nMask) == 0) continue;
//     if(aItem[i].nMode==__TMMODE_STRUCT_BNDR_CLDR__) // Define Constraint Label Direction�� KOR�� ��
//     {
//      if(CProduct::GetMovingType()!=D_PRODUCT_MOVING_ORG) continue;
//     }

		AddChildDlgs(aItem[i].nMode);
	}
 
	m_nCurChildDlg = -1; 
}

CStructBndrDlg::~CStructBndrDlg()
{
	DeleteChildDlgs();
}

void CStructBndrDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructBndrDlg)
	DDX_Control(pDX, IDC_TM_BNDR_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_BNDR_MENU_COMBO, m_ComboMenu);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructBndrDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructBndrDlg)
	ON_CBN_SELCHANGE(IDC_BNDR_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TM_BNDR_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructBndrDlg diagnostics

#ifdef _DEBUG
void CStructBndrDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructBndrDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructBndrDlg Implementation Functions
// (����)

void CStructBndrDlg::AddChildDlgs(int nMode)
{
	m_DlgIDs.Add(nMode);
	switch(nMode)
	{
	case __TMMODE_STRUCT_BNDR_CONS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Supports));
		m_ChildDlgs.Add(new CBndrConsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_CONSTRAINT);
		break;
	case __TMMODE_STRUCT_BNDR_NSPR__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Point_Spring_Supports));
		m_ChildDlgs.Add(new CBndrNsprDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_NSPR);
		break;
	case __TMMODE_STRUCT_BNDR_GSPR__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_General_Spring_Supports));
		m_ChildDlgs.Add(new CBndrSprsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_GENSPRING);
		break;
	case __TMMODE_STRUCT_BNDR_SSPR__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Surface_Spring_Supports));
		m_ChildDlgs.Add(new CBndrSsprDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_SURFACESPRING);    
		break;
	case __TMMODE_STRUCT_BNDR_ELNK__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Elastic_Link));
		m_ChildDlgs.Add(new CBndrElnkDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_ELLINK);
		break;
	case __TMMODE_STRUCT_BNDR_NLLINK__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Non_Linear_Link));
		m_ChildDlgs.Add(new CBndrNLLinkDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_NLNK);
		break;
	case __TMMODE_STRUCT_BNDR_RELT__:
		m_ComboMenuStr.Add(_LS(IDS_TM_ELNK_TYPE_RELT));
		m_ChildDlgs.Add(new CBndrReltDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_ELLINK);
		break;
	case __TMMODE_STRUCT_BNDR_CGLP__:
		m_ComboMenuStr.Add(_LS(IDS_TM_DT_CGLK));
		m_ChildDlgs.Add(new CBndrCglpDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_CGLP);
		break;
	case __TMMODE_STRUCT_BNDR_FRLS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Beam_End_Release));
		m_ChildDlgs.Add(new CBndrFrlsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_FRELEASE);
		break;
	case __TMMODE_STRUCT_BNDR_OFFS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Beam_End_Offsets));
		m_ChildDlgs.Add(new CBndrOffsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_OFFSET);
		break;
	case __TMMODE_STRUCT_BNDR_PRLS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Plate_End_Release));
		m_ChildDlgs.Add(new CBndrPrlsDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_PRELEASE);
		break;
	case __TMMODE_STRUCT_BNDR_RIGD__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Rigid_Link));
		m_ChildDlgs.Add(new CBndrRigdDlg);
		m_TableIDs.Add(ID_QUERY_STRUCT_RIGD);
		break;
	
	// [2007-09-03] Lee, Jong Hyup (Tel: 2145, jhlee@midasit.com)
	case __TMMODE_STRUCT_BNDR_MCON__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Linear_Constraints));
		m_ChildDlgs.Add(new CBndrMconDlg);
		m_TableIDs.Add(0);
		break;
	
	case __TMMODE_STRUCT_BNDR_DRLS__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Diaphragm_Disconnect));
		m_ChildDlgs.Add(new CBndrDrlsDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_BNDR_STORY__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Story_Diaphragm_Group));
		m_ChildDlgs.Add(new CBndrStoryDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_BNDR_CLDR__: // Define Constraint Label Direction, 20081104 KIMJM
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Define_Constraint_Label_Direction));
		m_ChildDlgs.Add(new CBndrCldrDlg);
		m_TableIDs.Add(0);
		break;
	case __TMMODE_STRUCT_BNGR_CHANGE__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Change_Boundary_Group));
		m_ChildDlgs.Add(new CBngrChangeDlg);
		m_TableIDs.Add(0);
		break;	
	case __TMMODE_STRUCT_BNDR_SOSP__:
#if defined(_CIVIL)
		m_ComboMenuStr.Add(_LS(IDS_MAIN_RIBBON_BNDR_SOIL_SPRING));
#else
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Pile_Spring_Supports));
#endif
		m_ChildDlgs.Add(new CBndrIntegralDlg); //061110 mylee
		m_TableIDs.Add(0);    
		break;
	/*case __TMMODE_STRUCT_BNDR_LINK_COMPACT__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Compact_Link_Numbers));
		m_ChildDlgs.Add(new CLinkCompactDlg);
		m_TableIDs.Add(0);
		break;*/	
	case __TMMODE_STRUCT_BNDR_LINK_RENUMBER__:
		m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Renumbering));
		m_ChildDlgs.Add(new CLinkRenumberDlg);
		m_TableIDs.Add(0);
		break;

	default: ASSERT(0); break;
	}
}
void CStructBndrDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}
// TreeMenu Mode�� �ش��ϴ� Child Dialog�� Ȱ��ȭ ��Ų��. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CStructBndrDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_BNDR__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
			else m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CStructBndrDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
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
	// Scroll Size�� �����Ѵ�. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

	// ����ó��. Apply�� ���ܾ��ϴ� ���
	// Child Dlg �� �������� ���
	mit::frx::MBarBaseDlg* pParent = (mit::frx::MBarBaseDlg*)GetParent();
	pParent->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

int CStructBndrDlg::_GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

void CStructBndrDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_ComboMenu.GetDC();
	for (int i=0;i < m_ComboMenu.GetCount();i++)
	{
		m_ComboMenu.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);
	
		if (sz.cx > dx) dx = sz.cx;
	}
	m_ComboMenu.ReleaseDC(pDC);
	
	//dx += ::GetSystemMetrics(SM_CXVSCROLL); + 2*::GetSystemMetrics(SM_CXEDGE);
	
	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}
}

void CStructBndrDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize()+1) * _GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<m_ComboMenuStr.GetSize(); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);
	AdjustComboListBox();
}

void CStructBndrDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}  


/////////////////////////////////////////////////////////////////////////////
// CStructBndrDlg message handlers

		

bool CStructBndrDlg::ShowApplyBtn()
{
	if ( m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg )
	{
		ASSERT(0);  return true;
	}

	switch ( m_DlgIDs.GetAt(m_nCurChildDlg) )
	{
	case __TMMODE_STRUCT_BNDR_STORY__:
		return false;
	default:
		return true;
	}
}

void CStructBndrDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitComboMenu();
	InitContent();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStructBndrDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	/*
	if(bActivate)
	{
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
	*/
}

void CStructBndrDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg�� ���� �θ��� CBarTreeMenu�� m_nCurMode�� Update
	// ���� �ʾ� Menu���� Select�ص� �ش� Dialog�� ���� �ʴ� ��찡 �߻�
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CStructBndrDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CStructBndrDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));
}
//////////////////////////////////////////////////////////////////
// By L.C.G
void CStructBndrDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	CMouseEdit::ImNotAssocWindowST();
	CMouseEdit::DisConnectAssocWindowST();

	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE); 
	}
	//if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// ���̺� ���� �׸��� ��� Table ��ư Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;

	//if(m_nCurChildDlg == 3)  // Surface Spring Supports
		if(m_DlgIDs[m_nCurChildDlg] ==__TMMODE_STRUCT_BNDR_SSPR__)
			((CBndrSsprDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);

	 
}
// By L.C.G
// ���� Unselect �� Notify ���� ���� 
void CStructBndrDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{

}

CWnd* CStructBndrDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CStructBndrDlg::SetHolderSize(int nHeight)
{
	CWnd *pWndHolder/*, *pWndExecute, *pWndClose*/; 
	CRect rectHolder;
	pWndHolder = GetDlgItem(IDC_TM_PLACEHOLDER);
	pWndHolder->GetWindowRect(rectHolder);
	rectHolder.bottom = rectHolder.top + nHeight;
	pWndHolder->GetParent()->ScreenToClient(rectHolder);
	pWndHolder->MoveWindow(rectHolder);

	CRect PHRect;
	m_wndPlaceHolder.GetWindowRect(&PHRect);
	ScreenToClient(&PHRect);
	m_RectPlaceHolder = PHRect;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...

//   CArray<UINT,UINT> aNextCtrls;   aNextCtrls.RemoveAll();
//   aNextCtrls.Add(IDC_TM_CLOSE);
//   aNextCtrls.Add(IDC_TM_EXECUTE);
//   aNextCtrls.FreeExtra();
// 
//   CRect rRef, rToMove;
//   pWndHolder->GetWindowRect(rRef);
//   GetDlgItem(aNextCtrls[0])->GetWindowRect(rToMove);
//   int nDistY = (rRef.bottom + 5) - rToMove.top;
//   CDlgUtil::CtrlMoveDistY(this, aNextCtrls, nDistY);
// 
//   /////////////////////////////////////////////////////////////
//   // Scroll Size�� �����Ѵ�. 
//   CRect rectClose;
//   GetDlgItem(IDC_TM_CLOSE)->GetWindowRect(rectClose);
//   ScreenToClient(&rectClose);
// 
//   CSize ScrSize,TSize;
//   ScrSize = GetTotalSize();
//   ScrSize.cy = rectClose.bottom + 5;
// 
//   SetScrollSizes(MM_TEXT,ScrSize);
}

void CStructBndrDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructBndrDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}