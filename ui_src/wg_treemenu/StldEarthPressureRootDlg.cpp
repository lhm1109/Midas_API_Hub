// CMEarthPressureRootDlg.cpp: implementation of the CStldEarthPressureRootDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldEarthPressureRootDlg.h"

#include "StldWindPressureAreaDlg.h"
#include "StldEarthPressureSeisDlg.h"
#include "StldEarthPressureStaticDlg.h"

#include "..\wg_cmd\CMChildBarBase.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_gr\GDisplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureRootDlg dialog

#define CFormView CCMDlgBarBase
IMPLEMENT_DYNCREATE(CStldEarthPressureRootDlg, CFormView)

CStldEarthPressureRootDlg::CStldEarthPressureRootDlg(CWnd* pParent /*=NULL*/)
	: CFormView(CStldEarthPressureRootDlg::IDD)
{
	CreateChildDlgs();
}

CStldEarthPressureRootDlg::~CStldEarthPressureRootDlg()
{
	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	DeleteChildDlgs();
}

void CStldEarthPressureRootDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_TABLE_BUTTON, m_wndTableButton);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_TM_FUNC_COMBO, m_ComboMenu);
}


BEGIN_MESSAGE_MAP(CStldEarthPressureRootDlg, CFormView)
	ON_BN_CLICKED(IDC_TM_TABLE_BUTTON, OnCmdTableButton)
	ON_CBN_SELCHANGE(IDC_TM_FUNC_COMBO, OnComboMenuSelect)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CStldEarthPressureRootDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	// Seis Earth Pressure
	m_ChildDlgs.Add(new CStldEarthPressureSeisDlg(this)); //Dlg
	//m_ChildDlgs.Add(new CStldEarthPressureSeisDlg(this)); //Dlg
	m_DlgIDs.Add(__TMMODE_STRUCT_STLD_EPSE__); //Tree Id
	m_TableIDs.Add(ID_QUERY_STRUCT_EARTHP_SEIS); //Table Id
	m_ComboStrs.Add(_LS(IDS_CMD_EARTHP_SEIS));  // Combo string

	// Static Earth Pressure
	m_ChildDlgs.Add(new CStldEarthPressureStaticDlg(this)); //Dlg
	m_DlgIDs.Add(__TMMODE_STRUCT_STLD_EPST__); //Tree Id
	m_TableIDs.Add(ID_QUERY_STRUCT_EARTHP_STATIC); //Table Id
	m_ComboStrs.Add(_LS(IDS_CMD_EARTHP_STATIC));  // Combo string
}

void CStldEarthPressureRootDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		delete m_ChildDlgs[i];
	}
}

BOOL CStldEarthPressureRootDlg::ShowChildDlgByID(int nID)
{
	int nIDCount = m_DlgIDs.GetSize();
	for (int i = 0; i < nIDCount; i++)
	{
		if (nID == m_DlgIDs[i])
		{
			m_ComboMenu.SetCurSel(i);
			ShowCurChildDlg(i);

			if (m_TableIDs[i] > 0) m_wndTableButton.EnableWindow(TRUE);
			else                   m_wndTableButton.EnableWindow(FALSE);

			return TRUE;
		}
	}
	return FALSE;
}

void CStldEarthPressureRootDlg::ShowCurChildDlg(int nDlgIndex)
{
	if (m_nCurChildDlg == nDlgIndex)
		return;

	if (nDlgIndex > m_ChildDlgs.GetSize() - 1)
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	//   if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
	//     m_ChildDlgs[m_nCurChildDlg]->End();

	//원래 대화상자 종료.
	if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize() > m_nCurChildDlg)
	{
		if (m_ChildDlgs[m_nCurChildDlg] && ::IsWindow(m_ChildDlgs[m_nCurChildDlg]->GetSafeHwnd()))
			m_ChildDlgs[m_nCurChildDlg]->End();
	}

	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
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

void CStldEarthPressureRootDlg::LayoutChildDialog()
{
	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...

	if (m_nCurChildDlg < 0 || m_nCurChildDlg >= m_ChildDlgs.GetSize())
		return;

	if (!(m_ChildDlgs[m_nCurChildDlg] && ::IsWindow(m_ChildDlgs[m_nCurChildDlg]->GetSafeHwnd())))
		return;

	CRect DlgRect, ParentRect, PHRect;

	GetClientRect(&ParentRect);
	m_ChildDlgs[m_nCurChildDlg]->GetWindowRect(&DlgRect);
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

int CStldEarthPressureRootDlg::_GetHeightFromFont()
{
	CFont * pNewFont = GetFont();
	CFont *pSysFont, *pOldFont, TempFont;
	CDC   *pCdc;
	TEXTMETRIC  tmNew, tmSys;
	int     nTemp, nCorrectVal;

	//get the DC for the edit control
	pCdc = GetDC();

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
	nTemp = tmNew.tmHeight + nCorrectVal;
	return nTemp;
}

void CStldEarthPressureRootDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboStrs.GetSize() + 1) * _GetHeightFromFont();

	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for (int i = 0; i < m_ComboStrs.GetSize(); i++)
		m_ComboMenu.AddString(m_ComboStrs[i]);

	m_ComboMenu.SetCurSel(0);
}

int CStldEarthPressureRootDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormViewInBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CStldEarthPressureRootDlg::OnInitialUpdate()
{
	CFormViewInBar::OnInitialUpdate();

	//여기서는 테이블을 볼 수가 없다.
	//m_wndTableButton.ShowWindow(SW_HIDE);

	InitComboMenu();

	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CStldEarthPressureRootDlg::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormViewInBar::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CStldEarthPressureRootDlg::OnComboMenuSelect()
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt);

	int nCurI = m_ComboMenu.GetCurSel();
	ShowCurChildDlg(nCurI);
}

void CStldEarthPressureRootDlg::OnCmdTableButton()
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));
}

CWnd* CStldEarthPressureRootDlg::GetDlgByID(int nID)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (nID == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

