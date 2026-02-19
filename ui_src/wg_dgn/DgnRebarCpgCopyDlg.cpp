// DgnRebarCpgCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgCopyDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgCopyDlg dialog


CDgnRebarCpgCopyDlg::CDgnRebarCpgCopyDlg(UINT key, CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRebarCpgCopyDlg::IDD, pParent)
{
	m_key = key;

	//{{AFX_DATA_INIT(CDgnRebarCpgCopyDlg)
	m_bLongi   = TRUE;
	m_bLongi_i = TRUE;
	m_bLongi_j = TRUE;
	m_bStiff   = TRUE;
	m_bStiff_i = TRUE;
	m_bStiff_j = TRUE;
	//}}AFX_DATA_INIT
}


void CDgnRebarCpgCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgCopyDlg)
	DDX_Control(pDX, IDC_DGN_SECT_LIST,   m_lstSect);
	DDX_Control(pDX, IDC_DGN_SELECT_LIST, m_lstSelect);
	DDX_Check  (pDX, IDC_DGN_LONGI_CHK,   m_bLongi);
	DDX_Check  (pDX, IDC_DGN_LONGI_I_CHK, m_bLongi_i);
	DDX_Check  (pDX, IDC_DGN_LONGI_J_CHK, m_bLongi_j);
	DDX_Check  (pDX, IDC_DGN_STIFF_CHK,   m_bStiff);
	DDX_Check  (pDX, IDC_DGN_STIFF_I_CHK, m_bStiff_i);
	DDX_Check  (pDX, IDC_DGN_STIFF_J_CHK, m_bStiff_j);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgCopyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRebarCpgCopyDlg)
	ON_BN_CLICKED(IDC_DGN_SELECT_BTN,    OnDgnSelectBtn)
	ON_BN_CLICKED(IDC_DGN_UNSELECT_BTN,  OnDgnUnselectBtn)
	ON_BN_CLICKED(IDC_DGN_SELECTALL_BTN, OnDgnSelectAllBtn)
	ON_BN_CLICKED(IDC_DGN_NONE_BTN,      OnDgnNoneBtn)
	ON_BN_CLICKED(IDC_DGN_LONGI_CHK,     OnDgnLongitudinalCheck)
	ON_BN_CLICKED(IDC_DGN_STIFF_CHK,     OnDgnStiffCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDgnRebarCpgCopyDlg::SetInitSectionList()
{
	T_SECT_K Key;
	T_SECT_D Data;

	CArray<T_SECT_K, T_SECT_K> arSectKeyList;
	CArray<T_SECT_K, T_SECT_K> aKeyBuf;

	m_pDoc->m_pAttrCtrl->GetSectKeyList(arSectKeyList);

	for(int i=0 ; i<arSectKeyList.GetSize() ; i++)
	{
		m_pDoc->m_pAttrCtrl->GetSect(arSectKeyList[i], Data);


		// Modify, Jaeoh. [4/14/2010], Tapered도 고려해야함 
		BOOL bCompoI = FALSE;
		BOOL bCompoTapI = FALSE;

		if(Data.nStype == D_SECT_TYPE_COMPO_I)
			bCompoI = TRUE;
		if(Data.nStype == D_SECT_TYPE_TAPERED && Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)
			bCompoTapI = TRUE;
		
		if(bCompoI || bCompoTapI)
			aKeyBuf.Add(arSectKeyList[i]);
	}
	
	int nItemCount = aKeyBuf.GetSize();
	if(nItemCount==0) return;

	aKeyBuf.FreeExtra();
	qsort(aKeyBuf.GetData(), aKeyBuf.GetSize(), sizeof(T_SECT_K), CCompFunc::UINTAsc);

	CString strName = _T("");
	for(int nCount=0; nCount<nItemCount; ++nCount)
	{
	  Key=aKeyBuf[nCount];
		if(!m_pDoc->m_pAttrCtrl->GetSect(Key, Data)) continue;
		strName.Format(_T("%d: %s"), Key, Data.SName);
		m_lstSect.SetItemData(m_lstSect.AddString(strName), Key);
	}
}

BOOL CDgnRebarCpgCopyDlg::GetSelectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
{
	int nSelCount = m_lstSelect.GetCount();
	if(nSelCount<1) return FALSE;

	aPosIdPair.RemoveAll();
	aName.RemoveAll();

	// 기존 Section list에 존재하는 items
	int nSectCount = m_lstSect.GetCount();
	for(int i=0; i<nSectCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i;
		pr.nVal = m_lstSect.GetItemData(i);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSect.GetText(i, strName);
		aName.Add(strName);
	}

	// Selection list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nSelCount = m_lstSelect.GetSelCount();
		aSelItem.SetSize(nSelCount);
		m_lstSelect.GetSelItems(nSelCount, aSelItem.GetData()); 
	}
	for(int i=0; i<nSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i+nSectCount;
		int nIndex = bAll ? i : aSelItem[i];
		pr.nVal = (int)m_lstSelect.GetItemData(nIndex);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSelect.GetText(nIndex, strName);
		aName.Add(strName);
	}

	if(bAll) m_lstSelect.ResetContent();
	else
	{
		for(int i=0; i<nSelCount; i++)
			m_lstSelect.DeleteString(aSelItem[i]-i);
	}

	aPosIdPair.FreeExtra();
	aName.FreeExtra();

	return TRUE;
}

BOOL CDgnRebarCpgCopyDlg::GetSectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
{
	int nSectCount = m_lstSect.GetCount();
	if(nSectCount<1) return FALSE;

	aPosIdPair.RemoveAll();
	aName.RemoveAll();

	// 기존 Selection list에 존재하는 items
	int nSelCount = m_lstSelect.GetCount();
	for(int i=0; i<nSelCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i;
		pr.nVal = m_lstSelect.GetItemData(i);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSelect.GetText(i, strName);
		aName.Add(strName);
	}

	// Section list에서 선택된 items
	CArray<int, int> aSelItem;
	if(!bAll)
	{
		nSectCount = m_lstSect.GetSelCount();
		aSelItem.SetSize(nSectCount);
		m_lstSect.GetSelItems(nSectCount, aSelItem.GetData()); 
	}
	for(int i=0; i<nSectCount; i++)
	{
		T_POS_INT_PAIR pr;
		pr.nPos = i+nSelCount;
		int nIndex = bAll ? i : aSelItem[i];
		pr.nVal = (int)m_lstSect.GetItemData(nIndex);
		aPosIdPair.Add(pr);

		CString strName(_T(""));
		m_lstSect.GetText(nIndex, strName);
		aName.Add(strName);
	}

	if(bAll) m_lstSect.ResetContent();
	else
	{
		for(int i=0; i<nSectCount; i++)
			m_lstSect.DeleteString(aSelItem[i]-i);
	}

	aPosIdPair.FreeExtra();
	aName.FreeExtra();

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgCopyDlg message handlers

BOOL CDgnRebarCpgCopyDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	CDialogMove::OnInitDialog();
	
	m_lstSect.ResetContent();
	m_lstSelect.ResetContent();

	SetInitSectionList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnRebarCpgCopyDlg::OnDgnSelectBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetSectData(aTrgPair, aName, FALSE)) return;
	aSrcPair.Copy(aTrgPair);

	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);

	m_lstSelect.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = (UINT)aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstSelect.SetItemData(m_lstSelect.AddString(strName), nItem);
	}
}

void CDgnRebarCpgCopyDlg::OnDgnUnselectBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetSelectData(aTrgPair, aName, FALSE)) return;
	aSrcPair.Copy(aTrgPair);

	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);

	m_lstSect.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = (UINT)aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstSect.SetItemData(m_lstSect.AddString(strName), nItem);
	}
}

void CDgnRebarCpgCopyDlg::OnDgnSelectAllBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetSectData(aTrgPair, aName, TRUE)) return;
	aSrcPair.Copy(aTrgPair);

	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);

	m_lstSelect.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = (UINT)aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstSelect.SetItemData(m_lstSelect.AddString(strName), nItem);
	}
}

void CDgnRebarCpgCopyDlg::OnDgnNoneBtn()
{
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aTrgPair, aSrcPair;
	CStringArray aName;
	if(!GetSelectData(aTrgPair, aName, TRUE)) ASSERT(0);
	aSrcPair.Copy(aTrgPair);

	int nSize = aTrgPair.GetSize();
	qsort((void*)aTrgPair.GetData(), nSize, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);

	m_lstSect.ResetContent();
	for(int i=0; i<nSize; i++)
	{
		UINT nItem = (UINT)aSrcPair[aTrgPair[i].nPos].nVal;
		CString strName = aName[aTrgPair[i].nPos];
		m_lstSect.SetItemData(m_lstSect.AddString(strName), nItem);
	}
}

void CDgnRebarCpgCopyDlg::OnDgnLongitudinalCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_LONGI_I_CHK)->EnableWindow(m_bLongi);
	GetDlgItem(IDC_DGN_LONGI_J_CHK)->EnableWindow(m_bLongi);
}

void CDgnRebarCpgCopyDlg::OnDgnStiffCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STIFF_I_CHK)->EnableWindow(m_bStiff);
	GetDlgItem(IDC_DGN_STIFF_J_CHK)->EnableWindow(m_bStiff);
}

void CDgnRebarCpgCopyDlg::OnOK() 
{
	int nCount = m_lstSelect.GetCount();
	if(nCount>0)
	{
		CArray<T_RPSC_K, T_RPSC_K> aRpscK; aRpscK.SetSize(nCount);
		CArray<T_CLRS_K, T_CLRS_K> aClrsK; aClrsK.SetSize(nCount);

		for(int i=0; i<nCount; ++i)
		{
			aRpscK[i] = m_lstSelect.GetItemData(i);
			aClrsK[i] = m_lstSelect.GetItemData(i);
		}

		UpdateData(TRUE);
		BOOL bRebar[] = { FALSE, FALSE, FALSE, FALSE };
		BOOL bStiffRebar[] = { FALSE, FALSE };

		if(m_bLongi)
		{
			bRebar[0] = m_bLongi_i;
			bRebar[1] = m_bLongi_j;			
		}
		if(m_bStiff)
		{
			bStiffRebar[0] = m_bStiff_i;
			bStiffRebar[1] = m_bStiff_j;
		}

		m_pDoc->m_pDataCtrl->CopyRpsd(m_key, bRebar, aRpscK);
		m_pDoc->m_pDataCtrl->CopyClrs(m_key, bStiffRebar, aClrsK);
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_NO_SELECTED_SECTION));
		return;
	}
	
	CDialogMove::OnOK();
}
