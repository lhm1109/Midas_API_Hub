// CMSectItemBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemBase dialog
#define CDialog CDlgChild

CCMSectItemBase::CCMSectItemBase(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemBase::IDD, pParent)
{
	ASSERT(0);
	//{{AFX_DATA_INIT(CCMSectItemBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMSectItemBase::CCMSectItemBase(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
	InitMember();
}

CCMSectItemBase::~CCMSectItemBase()
{
	if (m_pImageList != 0) delete m_pImageList;
	if (m_pBitmap != 0) delete m_pBitmap;
}

void CCMSectItemBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////
void CCMSectItemBase::SetModifyData(T_SECT_K Key, T_SECT_D &Data)
{
	m_OrgKey = Key;
	m_Key = Key;
	m_Data = Data;
	m_bModify = TRUE;
}

void CCMSectItemBase::SetSectionID(UINT key)
{
	m_bWorkTab = TRUE;
	m_FixID = key;
}


bool CCMSectItemBase::IsEnableSectShape7thDOF_Temp(const CString& strShape, const bool& bHSQType, const bool& bRoundType)
{
	if (strShape == D_SECT_SHAPE_REG_B)
	{
		if (bHSQType) return false;
		if (bRoundType) return false;
	}
	if (strShape == D_SECT_SHAPE_REG_2C) return false;
	if (strShape == D_SECT_SHAPE_REG_2CB) return false;
	if (strShape == D_SECT_SHAPE_REG_CC) return false;
	if (strShape == D_SECT_SHAPE_REG_CCH) return false;
	if (strShape == D_SECT_SHAPE_COMA_FOH) return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
void CCMSectItemBase::InitMember()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_OrgKey = 0;
	m_pImageList = 0;
	m_pBitmap = 0;
	m_bModify = FALSE;
	m_bModifyGrpd = FALSE;
	m_bModifyStfn = FALSE;
	m_bInitDone = FALSE;

	m_GrpdSSMD.Initialize();
	m_StfnSSMD.Initialize();

	m_bDesign = FALSE;
	m_bVirtual = FALSE;
	m_bWorkTab = FALSE;
}

BEGIN_MESSAGE_MAP(CCMSectItemBase, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemBase message handlers

BOOL CCMSectItemBase::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		CWnd* pWnd = GetParentOwner();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()))
			pWnd->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	*/
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
	return CDlgChild::PreTranslateMessage(pMsg);	
}

BOOL CCMSectItemBase::DestroyWindow()
{
	return CDialog::ShowWindow(SW_HIDE);
}