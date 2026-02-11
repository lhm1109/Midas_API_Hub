// CWindSelStoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindSelStoryDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlAddDlg dialog

CWindSelStoryDlg::CWindSelStoryDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CWindSelStoryDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	//{{AFX_DATA_INIT(CWindSelStoryDlg)
	//}}AFX_DATA_INIT
}


void CWindSelStoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMatlAddDlg)
	DDX_Control(pDX, IDC_WIND_SEL_STORY_LIST_UNSEL,   m_lstUnSel);
	DDX_Control(pDX, IDC_WIND_SEL_STORY_LIST_SEL, m_lstSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindSelStoryDlg, CDialogMove)
//{{AFX_MSG_MAP(CCMElsPlsMatlAddDlg)
	ON_BN_CLICKED(IDC_WIND_SEL_STORY_BTN_MOVE_RIGHT, OnSelectionBtn)
	ON_BN_CLICKED(IDC_WIND_SEL_STORY_BTN_MOVE_LEFT, OnUnselectionBtn)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindSelStoryDlg message handlers
BOOL CWindSelStoryDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_aStorK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetStorKeyList(m_aStorK);

	UpdateUnSelList();
	if(m_lstSel.GetCount() > 0) m_lstSel.ResetContent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWindSelStoryDlg::OnOK() 
{
	// TODO: Add extra validation here
	GetSelListStorK(m_aSelStorK);
	CDialogMove::OnOK();
}

void CWindSelStoryDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CWindSelStoryDlg::OnSelectionBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetSelListData(aTrgPair, aName, FALSE)) return;
	aSrcPair.Copy(aTrgPair);
	
	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);
	
	m_lstSel.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstSel.SetItemData(m_lstSel.AddString(strName), nItem);
	}
}
void CWindSelStoryDlg::OnUnselectionBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetUnSelListData(aTrgPair, aName, FALSE)) return;
	aSrcPair.Copy(aTrgPair);
	
	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);
	
	m_lstUnSel.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstUnSel.SetItemData(m_lstUnSel.AddString(strName), nItem);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWindSelStoryDlg member method
void CWindSelStoryDlg::GetSelListStorK(CArray<T_STOR_K, T_STOR_K>& aStorK)
{
	aStorK.RemoveAll();
	int nCnt = m_lstSel.GetCount();
	
	aStorK.SetSize(nCnt);
	for (int i = 0; i < nCnt; ++i)
	{
		CString strStor;
		m_lstSel.GetText(i, strStor);
		T_STOR_K StorK = m_pDoc->m_pAttrCtrl->GetStorKey(strStor);
		aStorK.SetAt(i, StorK);
	}
}

BOOL CWindSelStoryDlg::GetSelListData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
{
	int nUnSelCount = m_lstUnSel.GetCount();
	if(nUnSelCount<1) return FALSE;

	aPosIdPair.RemoveAll();
	aName.RemoveAll();

	// 기존 select list에 존재하는 items
	int nSelCount = m_lstSel.GetCount();
	for(int i=0; i<nSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i;
		pr.nVal = m_lstSel.GetItemData(i);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSel.GetText(i, strName);
		aName.Add(strName);
	}

	// unselect list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nUnSelCount = m_lstUnSel.GetSelCount();
		aSelItem.SetSize(nUnSelCount);
		m_lstUnSel.GetSelItems(nUnSelCount, aSelItem.GetData()); 
	}
	for(int i=0; i<nUnSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i+nSelCount;
		int nIndex = bAll ? i : aSelItem[i];
		pr.nVal = (int)m_lstUnSel.GetItemData(nIndex);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstUnSel.GetText(nIndex, strName);
		aName.Add(strName);
	}

	if(bAll) m_lstUnSel.ResetContent();
	else
	{
		for(int i=0; i<nUnSelCount; i++)
			m_lstUnSel.DeleteString(aSelItem[i]-i);
	}

	aPosIdPair.FreeExtra();
	aName.FreeExtra();

	return TRUE;
}

BOOL CWindSelStoryDlg::GetUnSelListData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
{
	int nSelCount = m_lstSel.GetCount();
	if(nSelCount<1) return FALSE;

	aPosIdPair.RemoveAll();
	aName.RemoveAll();

	// 기존 unselect list에 존재하는 items
	int nUnSelCount = m_lstUnSel.GetCount();
	for(int i=0; i<nUnSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i;
		pr.nVal = m_lstUnSel.GetItemData(i);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstUnSel.GetText(i, strName);
		aName.Add(strName);
	}

	// select list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nSelCount = m_lstSel.GetSelCount();
		aSelItem.SetSize(nSelCount);
		m_lstSel.GetSelItems(nSelCount, aSelItem.GetData()); 
	}
	for(int i=0; i<nSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i+nUnSelCount;
		int nIndex = bAll ? i : aSelItem[i];
		pr.nVal = (int)m_lstSel.GetItemData(nIndex);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSel.GetText(nIndex, strName);
		aName.Add(strName);
	}

	if(bAll) m_lstSel.ResetContent();
	else
	{
		for(int i=0; i<nSelCount; i++)
			m_lstSel.DeleteString(aSelItem[i]-i);
	}

	aPosIdPair.FreeExtra();
	aName.FreeExtra();

	return TRUE;
}

void CWindSelStoryDlg::UpdateUnSelList()
{
	m_lstUnSel.ResetContent();
	int nCnt = m_aStorK.GetSize();
	for (int i = nCnt - 1; i >= 0; --i)
	{
		T_STOR_D StorD;
		m_pDoc->m_pAttrCtrl->GetStor(m_aStorK[i], StorD);
		m_lstUnSel.SetItemData(m_lstUnSel.AddString(StorD.StoryName), (nCnt - 1) - i);
	}
}
