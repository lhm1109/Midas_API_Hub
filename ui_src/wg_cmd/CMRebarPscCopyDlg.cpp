// CMRebarPscCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarPscCopyDlg.h"

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
// CCMRebarPscCopyDlg dialog


CCMRebarPscCopyDlg::CCMRebarPscCopyDlg(UINT key, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMRebarPscCopyDlg::IDD, pParent)
{
	m_key = key;

	//{{AFX_DATA_INIT(CCMRebarPscCopyDlg)
	m_bLongi   = TRUE;
	m_bLongi_i = TRUE;
	m_bLongi_j = TRUE;
	m_bShear   = TRUE;
	m_bShear_i = TRUE;
	m_bShear_j = TRUE;
	//}}AFX_DATA_INIT
}


void CCMRebarPscCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarPscCopyDlg)
	DDX_Control(pDX, IDC_CMD_SECT_LIST,   m_lstSect);
	DDX_Control(pDX, IDC_CMD_SELECT_LIST, m_lstSelect);
	DDX_Check  (pDX, IDC_CMD_LONGI_CHK,   m_bLongi);
	DDX_Check  (pDX, IDC_CMD_LONGI_I_CHK, m_bLongi_i);
	DDX_Check  (pDX, IDC_CMD_LONGI_J_CHK, m_bLongi_j);
	DDX_Check  (pDX, IDC_CMD_SHEAR_CHK,   m_bShear);
	DDX_Check  (pDX, IDC_CMD_SHEAR_I_CHK, m_bShear_i);
	DDX_Check  (pDX, IDC_CMD_SHEAR_J_CHK, m_bShear_j);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMRebarPscCopyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMRebarPscCopyDlg)
	ON_BN_CLICKED(IDC_CMD_SELECT_BTN,    OnCmdSelectBtn)
	ON_BN_CLICKED(IDC_CMD_UNSELECT_BTN,  OnCmdUnselectBtn)
	ON_BN_CLICKED(IDC_CMD_SELECTALL_BTN, OnCmdSelectAllBtn)
	ON_BN_CLICKED(IDC_CMD_NONE_BTN,      OnCmdNoneBtn)
	ON_BN_CLICKED(IDC_CMD_LONGI_CHK,     OnCmdLongitudinalCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHK,     OnCmdShearCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMRebarPscCopyDlg::SetInitSectionList()
{
	T_SECT_K Key;
	T_SECT_D Data;

	CArray<T_SECT_K, T_SECT_K> aKeyBuf;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSect();
	while(pos)
	{
	  m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
		if(Key==m_key) continue;
		if (Data.nStype != D_SECT_TYPE_PSC &&
				Data.nStype != D_SECT_TYPE_TAPERED) continue;
		
		if (Data.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_3CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_4CELL ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_MID ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_T ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_BOX ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_I ||
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2 || 
				Data.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB) 
		{
			aKeyBuf.Add(Key);
		}
		else if (Data.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
		{
			if (Data.SectBefore.SectI.aOuterPolygon.GetSize() > 0)  // Import한 경우만
				aKeyBuf.Add(Key);
		}
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

BOOL CCMRebarPscCopyDlg::GetSelectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
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

BOOL CCMRebarPscCopyDlg::GetSectData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll)
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
// CCMRebarPscCopyDlg message handlers

BOOL CCMRebarPscCopyDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	CDialogMove::OnInitDialog();
	
	m_lstSect.ResetContent();
	m_lstSelect.ResetContent();

	SetInitSectionList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMRebarPscCopyDlg::OnCmdSelectBtn()
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

void CCMRebarPscCopyDlg::OnCmdUnselectBtn()
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

void CCMRebarPscCopyDlg::OnCmdSelectAllBtn()
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

void CCMRebarPscCopyDlg::OnCmdNoneBtn()
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

void CCMRebarPscCopyDlg::OnCmdLongitudinalCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_LONGI_I_CHK)->EnableWindow(m_bLongi);
	GetDlgItem(IDC_CMD_LONGI_J_CHK)->EnableWindow(m_bLongi);
}

void CCMRebarPscCopyDlg::OnCmdShearCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_SHEAR_I_CHK)->EnableWindow(m_bShear);
	GetDlgItem(IDC_CMD_SHEAR_J_CHK)->EnableWindow(m_bShear);
}

void CCMRebarPscCopyDlg::OnOK() 
{
	int nCount = m_lstSelect.GetCount();
	if(nCount>0)
	{
		CArray<T_RPSC_K, T_RPSC_K> aRpscK; aRpscK.SetSize(nCount);
		for(int i=0; i<nCount; ++i)
		{
			aRpscK[i] = m_lstSelect.GetItemData(i);
		}

		UpdateData(TRUE);
		BOOL bRebar[] = { FALSE, FALSE, FALSE, FALSE };
		if(m_bLongi)
		{
			bRebar[0] = m_bLongi_i;
			bRebar[1] = m_bLongi_j;
		}
		if(m_bShear)
		{
			bRebar[2] = m_bShear_i;
			bRebar[3] = m_bShear_j;
		}

		m_pDoc->m_pDataCtrl->CopyRpsc(m_key, bRebar, aRpscK);
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SECTION));
		return;
	}
	
	CDialogMove::OnOK();
}
