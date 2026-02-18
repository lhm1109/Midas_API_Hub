// DgnRebarPscMoment_I.cpp : implementation file
//
// Add by sshan. MNET:2607.('20061211)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarPscMoment_I.h"

#include "DgnRebarPscMomentGrid.h"
#include "DgnRebarPscMoment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscMoment_I dialog


CDgnRebarPscMoment_I::CDgnRebarPscMoment_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnRebarPscMoment_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarPscMoment_I)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CDgnRebarPscMoment *)pParent;

	m_Data.RemoveAll();
	m_wndGrid = NULL;
}

CDgnRebarPscMoment_I::~CDgnRebarPscMoment_I()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CDgnRebarPscMoment_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarPscMoment_I)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_DGN_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarPscMoment_I, CChildDialog)
	//{{AFX_MSG_MAP(CDgnRebarPscMoment_I)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
void CDgnRebarPscMoment_I::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CDgnRebarPscMoment_I::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
// CDgnRebarPscMoment_I message handlers

BOOL CDgnRebarPscMoment_I::OnInitDialog() 
{
	if (m_wndGrid == nullptr)
		m_wndGrid = new CDgnRebarPscMomentGrid(m_pParent);

	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
	m_wndGrid->Initialize(&m_Data);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnRebarPscMoment_I::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

