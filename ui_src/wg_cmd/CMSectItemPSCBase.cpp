// MSectItemPSCBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCBase dialog
#define CDialog CChildDialog

CCMSectItemPSCBase::CCMSectItemPSCBase(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemPSCBase::IDD, pParent)
{
	ASSERT(0);
	//{{AFX_DATA_INIT(CCMSectItemPSCBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMSectItemPSCBase::CCMSectItemPSCBase(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
}

CCMSectItemPSCBase::~CCMSectItemPSCBase()
{
}

void CCMSectItemPSCBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCBase, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSCBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCBase message handlers

BOOL CCMSectItemPSCBase::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message == WM_KEYDOWN )
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  return FALSE;
			// DO NOT process further
		}
	}
	return CDialog::PreTranslateMessage(pMsg);	
}
