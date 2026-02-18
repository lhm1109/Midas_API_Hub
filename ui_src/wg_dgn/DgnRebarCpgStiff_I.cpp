// DgnRebarCpgStiff_I.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgStiff_I.h"

#include "DgnRebarCpgStiffGrid.h"
#include "DgnRebarCpgStiff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff_I dialog


CDgnRebarCpgStiff_I::CDgnRebarCpgStiff_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarCpgStiff_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarCpgStiff_I)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CDgnRebarCpgStiff *)pParent;

	m_Data.RemoveAll();
	m_wndGrid = NULL;
}

CDgnRebarCpgStiff_I::~CDgnRebarCpgStiff_I()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CDgnRebarCpgStiff_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgStiff_I)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_DGN_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgStiff_I, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarCpgStiff_I)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
void CDgnRebarCpgStiff_I::SetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CDgnRebarCpgStiff_I::GetCurData(CArray<T_CLRS_STIF, T_CLRS_STIF&>* pData, BOOL bSaveAlways)
{
	ASSERT(pData);
	HWND hWnd = GetSafeHwnd();
	if (bSaveAlways && hWnd != NULL && ::IsWindow(hWnd))
	{
		if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
			m_wndGrid->CancelEdit();	
	}
	pData->Copy(m_Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiff_I message handlers

BOOL CDgnRebarCpgStiff_I::OnInitDialog() 
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
	m_wndGrid = new CDgnRebarCpgStiffGrid(m_pParent);

	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize(&m_Data);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnRebarCpgStiff_I::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

