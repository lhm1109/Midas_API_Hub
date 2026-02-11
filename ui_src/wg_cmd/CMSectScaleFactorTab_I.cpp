// CMSectScaleFactorTab_I.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectScaleFactorTab_I.h"

#include "CMSectScaleFactorGrid.h"
#include "CMSectScaleFactorDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorTab_I dialog


CCMSectScaleFactorTab_I::CCMSectScaleFactorTab_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectScaleFactorTab_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectScaleFactorTab_I)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMSectScaleFactorDlgNew *)pParent;

	//m_Data.RemoveAll();
	m_wndGrid = 0;
}

CCMSectScaleFactorTab_I::~CCMSectScaleFactorTab_I()
{
	if (m_wndGrid != 0)
	{
		delete m_wndGrid;
		m_wndGrid = 0;
	}
}

void CCMSectScaleFactorTab_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectScaleFactorTab_I)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectScaleFactorTab_I, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectScaleFactorTab_I)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Interface
/*
void CCMSectScaleFactorTab_I::SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData)
{
	ASSERT(pData);
	m_Data.Copy(*pData);

	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		m_wndGrid->MakeItemEx();
	}
}

void CCMSectScaleFactorTab_I::GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways)
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
// CCMSectScaleFactorTab_I message handlers

BOOL CCMSectScaleFactorTab_I::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
	m_wndGrid = new CCMSectScaleFactorGrid();
	m_pParent->SetGridI(m_wndGrid);
	m_wndGrid->SubclassDlgItem(IDC_CMD_GRID, this);
	m_wndGrid->Initialize(m_pParent);
	m_wndGrid->MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMSectScaleFactorTab_I::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS))
		m_wndGrid->CancelEdit();	
	return CChildDialog::DestroyWindow();
}

