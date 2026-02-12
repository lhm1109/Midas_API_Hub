// CMSectItemBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageItemBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
/*
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_ViewBuff.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageItemBase dialog
#define CDialog CChildDialog//CDlgChild

CCMStageItemBase::CCMStageItemBase(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageItemBase::IDD, pParent)
{
	ASSERT(0);
	//{{AFX_DATA_INIT(CCMStageItemBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMStageItemBase::CCMStageItemBase(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
	
}

CCMStageItemBase::~CCMStageItemBase()
{

}

void CCMStageItemBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageItemBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCMStageItemBase, CDialog)
	//{{AFX_MSG_MAP(CCMStageItemBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//CDlgUtil로 옮김... ^^ㅇ
/*
void CCMStageItemBase::_SetListCtrlHeader(CListCtrl* pList, CStringArray& HTitles, CArray<float, float>* pHRatio,
																								CArray<int, int >* pHWidth)
{

	CArray<int,int> HWidths;
	ASSERT(pList);
	if(pHRatio && pHWidth) ASSERT(0);
	if(!pHRatio && !pHWidth) ASSERT(0);
	
	CRect Rect;
	pList->GetWindowRect(&Rect);
	
	if(pHRatio)
	{
		ASSERT(HTitles.GetSize() == pHRatio->GetSize());
		for(int i = 0 ; i < HTitles.GetSize() ; i++)
		{
			if(i == HTitles.GetSize()-1)
				HWidths.Add(Rect.Width() * (*pHRatio)[i]-5+i);
			else
				HWidths.Add(Rect.Width() * (*pHRatio)[i]);
		}
	}
	else
	{
		HWidths.Copy(*pHWidth);  
	}
	
	int nColNum = HWidths.GetSize();
	LV_COLUMN lvcolumn;
	CString title;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(pList->GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(pList->GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = HTitles[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = HWidths[i];
		lvcolumn.pszText = title.GetBuffer(0);
		pList->InsertColumn(i,&lvcolumn);
		pList->SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMStageItemBase::SetListItem(CListCtrl *pList, int nIndex , CStringArray &Contents,DWORD ItemData)
{
	//pList->Getcou
	int nColumn = pList->GetHeaderCtrl()->GetItemCount();

	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	
	ASSERT(Contents.GetSize() <= nColumn);

	int nIter = Contents.GetSize();
	
	for(int i = 0; i < nColumn ; i++)
	{
		lvitem.iSubItem = i;
		str = Contents[i];
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		
		if(i == 0)
		 pList->InsertItem(&lvitem);
		else
		 pList->SetItem(&lvitem);

	  str.ReleaseBuffer();
	}
	pList->SetItemData(nIndex, (DWORD)ItemData);

	return TRUE;
}
*/
void CCMStageItemBase::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	
}

void CCMStageItemBase::UpdateBuffer()
{
	;
}

void CCMStageItemBase::SetCurStageData(T_STAG_D* pStageD)
{
	;
}


void CCMStageItemBase::ResetParam() // 각 Item별 입력 필드를 초기화 한다. 
{
	;
}

BOOL CCMStageItemBase::IsCurStageBase()
{
	T_STAG_K CurStageK = CDBDoc::GetDocPoint()->m_pStagCtrl->GetCurStag();
	return CDBDoc::GetDocPoint()->m_pStagCtrl->IsBaseStagKey(CurStageK);
}
