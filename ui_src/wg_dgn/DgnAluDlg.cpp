// DgnStlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnAluDlg.h"

// 인영근 : 추가시 고칠곳 1.
#include "DgnStlServiceLimitDlg.h"
#include "DgnStlCbDlg.h"
#include "DgnStlCvDlg.h"
#include "DgnStlStrDlg.h"
//#include "DgnStlUcfDlg.h"
#include "DgnStlStifDlg.h"
#include "DgnStlSLRSDlg.h"
#include "DgnStlPhibFacDlg.h"
#include "DgnStlSideswayDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "DgnAluElemParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnAluDlg

IMPLEMENT_DYNCREATE(CDgnAluDlg, MChildFormView)

CDgnAluDlg::CDgnAluDlg()
	: MChildFormView(CDgnAluDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnAluDlg)
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnAluDlg::~CDgnAluDlg()
{
	DeleteChildDlgs();
}

void CDgnAluDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnAluDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnAluDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnAluDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnAluDlg diagnostics

#ifdef _DEBUG
void CDgnAluDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnAluDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnAluDlg Implementation Functions


void CDgnAluDlg::CreateChildDlgs()
{
	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();
	m_nCurChildDlg = -1; 
	// Coded by Seungjun MNet:No.2393 ('20061011)
	m_ChildDlgs.Add(new CDgnAluAribDlg);
	m_DlgIDs.Add(__TMMODE_DGN_ALU_ARIB__);
	m_TableIDs.Add(ID_TABLE_ALU_FORCERIB);

	m_ChildDlgs.Add(new CDgnAluAwedDlg);
	m_DlgIDs.Add(__TMMODE_DGN_ALU_AWED__);
	m_TableIDs.Add(ID_TABLE_ALU_WELD_RATIO);

	m_ChildDlgs.Add(new CDgnAluAhtrDlg);
	m_DlgIDs.Add(__TMMODE_DGN_ALU_AHTR__);
	m_TableIDs.Add(ID_TABLE_ALU_HEATRANGE);

	m_ChildDlgs.Add(new CDgnAluAPhiDlg);
	m_DlgIDs.Add(__TMMODE_DGN_ALU_APHI__);
	m_TableIDs.Add(ID_TABLE_ALU_BEND_RATIO);
}

void CDgnAluDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	
		delete m_ChildDlgs[i];
	m_ChildDlgs.RemoveAll();
		m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnAluDlg::ShowChildDlgByID(int nID)
{
	int nIDCount = m_DlgIDs.GetSize();
	for(int i=0; i<nIDCount; i++)
	{
		if(nID==m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if(m_TableIDs[i] > 0)	
				m_wndTableBtn.EnableWindow(TRUE);
			else									
				m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnAluDlg::ShowCurChildDlg(int nDlgIndex)
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
	this->RedrawWindow();
}

int CDgnAluDlg::GetHeightFromFont()
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

void CDgnAluDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Add by ZINU.('01.1.2).
	Update_InitDataByCode(FALSE);
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CDgnAluDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	MChildFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

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
}

void CDgnAluDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CDgnAluDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MChildFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnAluDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Add by ZINU.('01.1.2).
void CDgnAluDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

// Add by ZINU.('01.1.2).
void CDgnAluDlg::Update_InitDataByCode(BOOL bUpdate)
{
	// Add by ZINU.('02.11.29).	Call only if Code Changed.
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
				case(UR_DALU_ADD):
				case(UR_DALU_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_FORCERIB);
	m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_WELD_RATIO);
	m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_HEATRANGE);
	m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_BEND_RATIO);

	// Recalculate Height.
	int wHeight = (sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr)+1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for(int i=0; i<sizeof(m_ComboMenuStr)/sizeof(*m_ComboMenuStr); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = sizeof(m_ComboMenuStr)/sizeof(CString);
	CSize sizeMax, sizeCur;
	sizeMax = dc.GetTextExtent(m_ComboMenuStr[0]);
	for(int k=1; k<nSize; k++)
	{
		sizeCur = dc.GetTextExtent(m_ComboMenuStr[k]);
		if(sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if(sizeMax.cx > m_ComboMenu.GetDroppedWidth())	m_ComboMenu.SetDroppedWidth(sizeMax.cx);

	if(iSelID < 0)	m_ComboMenu.SetCurSel(0);
	else						m_ComboMenu.SetCurSel(iSelID);
}
void CDgnAluDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnAluDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}