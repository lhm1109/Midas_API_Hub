// MBaseDockingView.cpp : implementation file
//

#include "stdafx.h"
#include "BaseDockingView.h"

#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MBaseDockingDlg
IMPLEMENT_DYNCREATE(MBaseDockingView, MChildFormView)

MBaseDockingView::MBaseDockingView(UINT nIDTemplate)
	: MChildFormView(nIDTemplate)
{
	m_pDocument = NULL;
	m_pCreateContext = NULL;
	m_hWndFocus = NULL;     // focus window is unknown
	m_nIDHelp = nIDTemplate;

	m_nCurChildDlg = 0;
	m_aMenyStr.RemoveAll();
	m_aDlgID.RemoveAll();
	m_aTableID.RemoveAll();
	m_aChildDlg.RemoveAll();
}

MBaseDockingView::MBaseDockingView() : MChildFormView(-1)
{
}

MBaseDockingView::~MBaseDockingView()
{
	DelChildDlgs();
}

void MBaseDockingView::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MBaseDockingView)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(MBaseDockingView, MChildFormView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetDlg diagnostics
// 

void MBaseDockingView::DelChildDlgs()
{
	int nCount = m_aChildDlg.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		delete m_aChildDlg[i];
	}
}

void MBaseDockingView::InitComboMenu()
{
	int wHeight;
	wHeight = (m_aMenyStr.GetSize() + 1) * GetHeightFromFont();

	CRect rectWnd;
	m_cmbMenu.GetWindowRect(rectWnd);
	m_cmbMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight,
							SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	for (int i = 0; i < m_aMenyStr.GetSize(); ++i)
	{
		m_cmbMenu.AddString(m_aMenyStr[i]);
	}
	m_cmbMenu.SetCurSel(0);
}

int MBaseDockingView::GetHeightFromFont()
{
	CFont* pNewFont = GetFont();
	CFont* pSysFont, * pOldFont, TempFont;
	CDC* pCdc;
	TEXTMETRIC  tmNew, tmSys;
	int     nTemp, nCorrectVal;

	//get the DC for the edit control
	pCdc = GetDC();

	//get the metrics for the system font
	pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	pOldFont = pCdc->SelectObject(pSysFont);
	pCdc->GetTextMetrics(&tmSys);

	//get the metrics for the new font
	pCdc->SelectObject(pNewFont);
	pCdc->GetTextMetrics(&tmNew);

	//select the original font back into the DC and release the DC
	pCdc->SelectObject(pOldFont);
	ReleaseDC(pCdc);

	nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight) / 2);
	//calculate the new height for the edit control
	nTemp = tmNew.tmHeight + nCorrectVal;
	//nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	return nTemp;
}

void MBaseDockingView::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC* pDC = m_cmbMenu.GetDC();

	for (int i = 0; i < m_cmbMenu.GetCount(); i++)
	{
		m_cmbMenu.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_cmbMenu.ReleaseDC(pDC);

	if (m_cmbMenu.GetDroppedWidth() < dx)
	{
		m_cmbMenu.SetDroppedWidth(dx);
		ASSERT(m_cmbMenu.GetDroppedWidth() == dx);
	}
}

// TreeMenu Mode에 해당하는 Child Dialog를 활성화 시킨다. 
BOOL MBaseDockingView::ShowChildDlgByID(int nID)
{
	ShowCurChildDlg(0);
	//m_cmbMenu.SetCurSel(0);
	return TRUE;
}

void MBaseDockingView::ShowCurChildDlg(int nDlgIndex)
{
	if (m_aChildDlg.GetSize() < nDlgIndex + 1) return;
	if (m_nCurChildDlg == nDlgIndex)
		return;

	if (nDlgIndex > m_aChildDlg.GetSize() - 1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if (m_nCurChildDlg >= 0 && m_aChildDlg.GetSize() - 1 >= m_nCurChildDlg)
		m_aChildDlg[m_nCurChildDlg]->End();

	m_aChildDlg[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_aChildDlg[nDlgIndex]->GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);

	int Sx, Sy, Wx, Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();

	m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize, TSize;
	ScrSize = GetTotalSize();

	TSize.cx = PHRect.Width() - m_RectPlaceHolder.Width();
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

	ScrSize += TSize;

	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT, ScrSize);
}

/////////////////////////////////////////////////////////////////////////////
// message map functions
// 

void MBaseDockingView::OnInitialUpdate()
{
	MChildFormView::OnInitialUpdate();
	InitComboMenu();
}

void MBaseDockingView::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void MBaseDockingView::OnTmExecute()
{
	if (m_nCurChildDlg < m_aChildDlg.GetSize())
		m_aChildDlg[m_nCurChildDlg]->Execute();
}

void MBaseDockingView::OnSelChangeMenuCmb()
{
	int nCurI = m_cmbMenu.GetCurSel();
	//I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	//CTreeMenuBarBase::SetModeST(m_aDlgID.GetAt(nCurI));
	ShowCurChildDlg(nCurI);
}

void MBaseDockingView::OnTmTableBtn()
{
	int nCurSel = m_cmbMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_cmbMenu.GetCount()) return;
	if (m_aTableID[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_aTableID[nCurSel], 0));
}