// DgnCfsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfsDlg.h"

#include "DgnCfServiceLimitDlg.h"
#include "DgnCfCbDlg.h"
#include "DgnCfParaDlg.h"
#include "DgnCfPfmcDlg.h"
#include "DgnCfPfcmDlg.h"
#include "DgnCfPfdlDlg.h"
#include "DgnCfPfmpDlg.h"
#include "DgnCfPfnpDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"    // For GetDocPoint().
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfsDlg

IMPLEMENT_DYNCREATE(CDgnCfsDlg, MChildFormView)

CDgnCfsDlg::CDgnCfsDlg()
	: MChildFormView(CDgnCfsDlg::IDD)
{
	//{{AFX_DATA_INIT(CDgnCfsDlg)
	//}}AFX_DATA_INIT
	CreateChildDlgs();
}

CDgnCfsDlg::~CDgnCfsDlg()
{
	DeleteChildDlgs();
}

void CDgnCfsDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfsDlg)
	DDX_Control(pDX, IDC_DGN_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_DGN_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_DGN_MENU_COMBO, m_ComboMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCfsDlg, MChildFormView)
	//{{AFX_MSG_MAP(CDgnCfsDlg)
	ON_CBN_SELCHANGE(IDC_DGN_MENU_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_DGN_TABLE_BTN, OnDgnTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfsDlg diagnostics

#ifdef _DEBUG
void CDgnCfsDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CDgnCfsDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDgnCfsDlg Implementation Functions


void CDgnCfsDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1; 

	m_ChildDlgs.RemoveAll();
	m_DlgIDs.RemoveAll();
	m_TableIDs.RemoveAll();

#if defined(_MGEN)
	m_ChildDlgs.Add(new CDgnCfServiceLimitDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_SERV__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_SERV);

	m_ChildDlgs.Add(new CDgnCfCbDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_CBFT__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_CBFT);

	#if defined(_CH)
	//Add by tss(2017.8.14)
	m_ChildDlgs.Add(new CDgnCfStabCoefDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_CSMS__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_STABILITY_COEF);

	m_ChildDlgs.Add(new CDgnCfOpenSectCoefDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_CSOS__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_OPENSECT_RESTCOEF);


	m_ChildDlgs.Add(new CDgnCfPhiCoefDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_SPHI__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_SPHI);

	m_ChildDlgs.Add(new CDgnCfPurlinDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PLIN__);
	m_TableIDs.Add(ID_QUERY_DGN_CF_PURLIN);

	//Add by tss(2025.7.25)
	m_ChildDlgs.Add(new CDgnCfPfmcDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PFMC__);
	m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MEMBER_COEFFICIENT);

	m_ChildDlgs.Add(new CDgnCfPfcmDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PFCM__);
	m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MIN_COEFFICEN);

	m_ChildDlgs.Add(new CDgnCfPfdlDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PFDL__);
	m_TableIDs.Add(ID_QUERY_DGN_PERFOR_DUCTILITY_LEVEL_CFS);

	m_ChildDlgs.Add(new CDgnCfPfmpDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PFMP__);
	m_TableIDs.Add(ID_QUERY_DGN_PERFOR_MEMBER_PROPERTIES);

	m_ChildDlgs.Add(new CDgnCfPfnpDlg);
	m_DlgIDs.Add(__TMMODE_DGN_CFS_PFNP__);
	m_TableIDs.Add(ID_QUERY_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT);
	#endif
#endif

}

void CDgnCfsDlg::DeleteChildDlgs()
{
	for(int i=0; i<m_ChildDlgs.GetSize(); i++)	delete m_ChildDlgs[i];
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
// ex) __TMMODE_STRUCT_NODE_CREATE__ ...
BOOL CDgnCfsDlg::ShowChildDlgByID(int nID)
{
	if(nID==__TMMODE_DGN_CFS__)  // 인영근 : 이 파일 Copy시 여기 수정
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
		  m_wndTableBtn.EnableWindow(TRUE);
			else
		  m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnCfsDlg::ShowCurChildDlg(int nDlgIndex)
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

int CDgnCfsDlg::GetHeightFromFont()
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

void CDgnCfsDlg::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Add by ZINU.('01.1.2).
	Update_InitDataByCode(FALSE);
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);	
}

void CDgnCfsDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);

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

void CDgnCfsDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
}

int CDgnCfsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormViewInBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CDgnCfsDlg::OnDgnTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}

// Add by ZINU.('01.1.2).
void CDgnCfsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnCfsDlg::Update_InitDataByCode(BOOL bUpdate)
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
				case(UR_DCFS_ADD):
				case(UR_DCFS_DEL):	{bCODE = TRUE; break;}
				default:	break;
			}
		}
		if(!bCODE) return;
	}

	int iSelID = m_ComboMenu.GetCurSel();
	// Initialize Data.
	m_ComboMenu.ResetContent();
	m_ComboMenuStr.RemoveAll();
#if defined(_MGEN)
	#if defined(_CH)
	m_ComboMenuStr.SetSize(11);
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_SERV);
	m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_CBFT);
	m_ComboMenuStr[2] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_STABILITY_COEF);	//Add by tss(2017.8.14)
	m_ComboMenuStr[3] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_OPENSECT_RESTCOEF);//Add by tss(2017.8.14)
	m_ComboMenuStr[4] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_SPHI);//Add by tss 2022/10/14
	m_ComboMenuStr[5] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_PURLIN);
	m_ComboMenuStr[6] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_PERFOR_MEMBER_COEFFICIENT_CFS);
	m_ComboMenuStr[7] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_PERFOR_MIN_COEFFICEN_CFS);
	m_ComboMenuStr[8] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_PERFOR_DUCTILITY_LEVEL_CFS);
	m_ComboMenuStr[9] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_PERFOR_MEMBER_PROPERTIES_CFS);
	m_ComboMenuStr[10] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT_CFS);
	#else
	m_ComboMenuStr.SetSize(2);
	m_ComboMenuStr[0] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_SERV);
	m_ComboMenuStr[1] = CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_CBFT);
	#endif
#endif
	// Recalculate Height.
	int wHeight = (m_ComboMenuStr.GetSize() + 1) * GetHeightFromFont();
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL,0,0,rectWnd.Width(),wHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	// Add String at ComboBox.
	for(int i=0; i<m_ComboMenuStr.GetSize(); i++)
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	// Recalculate Width.
	CClientDC dc(this);
	int nSize = m_ComboMenuStr.GetSize();
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
void CDgnCfsDlg::OnTmExecute()
{
	if (m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CDgnCfsDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}