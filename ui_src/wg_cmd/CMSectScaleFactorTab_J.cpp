// CMSectScaleFactorTab_J.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectScaleFactorTab_J.h"

#include "CMSectScaleFactorGrid.h"
#include "CMSectScaleFactorDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorTab_J dialog


CCMSectScaleFactorTab_J::CCMSectScaleFactorTab_J(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectScaleFactorTab_J::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectScaleFactorTab_J)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMSectScaleFactorDlgNew *)pParent;

	//m_Data.RemoveAll();
	m_wndGrid = 0;
}

CCMSectScaleFactorTab_J::~CCMSectScaleFactorTab_J()
{
	if (m_wndGrid != 0)
	{
		delete m_wndGrid;
		m_wndGrid = 0;
	}
}

void CCMSectScaleFactorTab_J::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectScaleFactorTab_J)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectScaleFactorTab_J, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectScaleFactorTab_J)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
/*
void CCMSectScaleFactorTab_J::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CCMSectScaleFactorTab_J::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
*/

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorTab_J message handlers

BOOL CCMSectScaleFactorTab_J::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
	m_wndGrid = new CCMSectScaleFactorGrid();
	m_pParent->SetGridJ(m_wndGrid);
	m_wndGrid->SubclassDlgItem(IDC_CMD_GRID, this);
	m_wndGrid->Initialize(m_pParent);
	m_wndGrid->SetModeJ(); // J scalefactor
	m_wndGrid->MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMSectScaleFactorTab_J::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

