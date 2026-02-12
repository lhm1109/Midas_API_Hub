// CMRebarCompositeMoment_I.cpp : implementation file
//
// 2004. 03. 09   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarCompositeMoment_I.h"

#include "CMRebarCompositeMomentGrid.h"
#include "CMRebarCompositeMoment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeMoment_I dialog


CCMRebarCompositeMoment_I::CCMRebarCompositeMoment_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMRebarCompositeMoment_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarCompositeMoment_I)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMRebarCompositeMoment *)pParent;

	m_Data.RemoveAll();
	m_wndGrid = NULL;
}

CCMRebarCompositeMoment_I::~CCMRebarCompositeMoment_I()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMRebarCompositeMoment_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarCompositeMoment_I)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarCompositeMoment_I, CChildDialog)
	//{{AFX_MSG_MAP(CCMRebarCompositeMoment_I)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
void CCMRebarCompositeMoment_I::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CCMRebarCompositeMoment_I::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
// CCMRebarCompositeMoment_I message handlers

BOOL CCMRebarCompositeMoment_I::OnInitDialog() 
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}

	m_wndGrid = new CCMRebarCompositeMomentGrid(m_pParent);

	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize(&m_Data);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMRebarCompositeMoment_I::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

