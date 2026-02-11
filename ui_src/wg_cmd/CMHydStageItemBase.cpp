// CMSectItemBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMHydStageItemBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageItemBase dialog
#define CDialog CChildDialog//CDlgChild

CCMHydStageItemBase::CCMHydStageItemBase(CWnd* pParent /*=NULL*/)
	: CDialog(CCMHydStageItemBase::IDD, pParent)
{
	ASSERT(0);
	//{{AFX_DATA_INIT(CCMHydStageItemBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMHydStageItemBase::CCMHydStageItemBase(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
	
}

CCMHydStageItemBase::~CCMHydStageItemBase()
{

}

void CCMHydStageItemBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageItemBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCMHydStageItemBase, CDialog)
	//{{AFX_MSG_MAP(CCMHydStageItemBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMHydStageItemBase::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	
}

void CCMHydStageItemBase::UpdateBuffer()
{
	;
}

void CCMHydStageItemBase::SetCurStageData(T_HSTG_D* pStageD)
{
	;
}


void CCMHydStageItemBase::ResetParam() // 각 Item별 입력 필드를 초기화 한다. 
{
	;
}
