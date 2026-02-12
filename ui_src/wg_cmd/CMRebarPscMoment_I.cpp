// CMRebarPscMoment_I.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarPscMoment_I.h"

#include "CMRebarPscMomentGrid.h"
#include "CMRebarPscMoment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscMoment_I dialog


CCMRebarPscMoment_I::CCMRebarPscMoment_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarPscMoment_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscMoment_I)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMRebarPscMoment *)pParent;

	m_Data.RemoveAll();
	m_wndGrid = NULL;
}

CCMRebarPscMoment_I::~CCMRebarPscMoment_I()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMRebarPscMoment_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarPscMoment_I)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarPscMoment_I, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarPscMoment_I)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
void CCMRebarPscMoment_I::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CCMRebarPscMoment_I::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
// CCMRebarPscMoment_I message handlers

BOOL CCMRebarPscMoment_I::OnInitDialog() 
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
	m_wndGrid = new CCMRebarPscMomentGrid(m_pParent);

	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize(&m_Data);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMRebarPscMoment_I::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

