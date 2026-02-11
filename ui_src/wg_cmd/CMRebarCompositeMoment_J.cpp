// CMRebarCompositeMoment_J.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarCompositeMoment_J.h"

#include "CMRebarCompositeMomentGrid.h"
#include "CMRebarCompositeMoment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment_J dialog


CCMRebarCompositeMoment_J::CCMRebarCompositeMoment_J(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarCompositeMoment_J::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarCompositeMoment_J)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMRebarCompositeMoment *)pParent;

	m_wndGrid = NULL;

	m_Data.RemoveAll();
	m_bSymmetry = FALSE;
}

CCMRebarCompositeMoment_J::~CCMRebarCompositeMoment_J()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}	
}

void CCMRebarCompositeMoment_J::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarCompositeMoment_J)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarCompositeMoment_J, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarCompositeMoment_J)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface
void CCMRebarCompositeMoment_J::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSymmetry)
{
	ASSERT(pData);
	m_Data.Copy(*pData);
	m_bSymmetry = bSymmetry;

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->ChangeSymmetry(m_bSymmetry);
		m_wndGrid->MakeItemEx();
	}
}

void CCMRebarCompositeMoment_J::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
// CCMRebarCompositeMoment_J message handlers

BOOL CCMRebarCompositeMoment_J::OnInitDialog() 
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}

	m_wndGrid = new CCMRebarCompositeMomentGrid(m_pParent);

	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize(&m_Data, m_bSymmetry);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMRebarCompositeMoment_J::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}
