#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLcomServTypeDlg.h"
#include "DgnGenLcomServBSDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServBSDlg dialog

CDgnGenLcomServBSDlg::CDgnGenLcomServBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenLcomServBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLcomServBSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_mUnSel.RemoveAll();
	m_mComb1.RemoveAll();
	m_mComb2.RemoveAll();
}


void CDgnGenLcomServBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLcomServBSDlg)
	DDX_Control(pDX, IDC_DGN_SERV_COMB_LST, m_aUnSelList);
	DDX_Control(pDX, IDC_DGN_SERV_COMB1_LST, m_aComb1List);
	DDX_Control(pDX, IDC_DGN_SERV_COMB2_LST, m_aComb2List);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenLcomServBSDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnGenLcomServBSDlg)
	ON_BN_CLICKED(IDC_DGN_SERV_COMB1_ADD_BTN, OnDgnAddComb1Btn)
	ON_BN_CLICKED(IDC_DGN_SERV_COMB1_DEL_BTN, OnDgnDelComb1Btn)
	ON_BN_CLICKED(IDC_DGN_SERV_COMB2_ADD_BTN, OnDgnAddComb2Btn)
	ON_BN_CLICKED(IDC_DGN_SERV_COMB2_DEL_BTN, OnDgnDelComb2Btn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CDgnGenLcomServBSDlg::InitCtrl()
{
	InitLcomData();

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_aUnSelList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aUnSelList.GetSafeHwnd(), dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[2] = { _T("Load Comb"), _T("Seq") };
	int width[2] = { 120,0 };
	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_aUnSelList.InsertColumn(i, &lvcolumn);
	}
	Write_ListCtrl(m_aUnSelList, m_mUnSel);

	//ServeLimit1
	DWORD ServeLimit1Style;
	ServeLimit1Style = ListView_GetExtendedListViewStyle(m_aComb1List.GetSafeHwnd());
	ServeLimit1Style |= LVS_EX_FULLROWSELECT;
	ServeLimit1Style |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aComb1List.GetSafeHwnd(), ServeLimit1Style);

	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_aComb1List.InsertColumn(i, &lvcolumn);
	}
	Write_ListCtrl(m_aComb1List, m_mComb1);

	//ServeLimit3
	DWORD ServeLimit3Style;
	ServeLimit3Style = ListView_GetExtendedListViewStyle(m_aComb2List.GetSafeHwnd());
	ServeLimit3Style |= LVS_EX_FULLROWSELECT;
	ServeLimit3Style |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aComb2List.GetSafeHwnd(), ServeLimit3Style);

	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_aComb2List.InsertColumn(i, &lvcolumn);
	}
	Write_ListCtrl(m_aComb2List, m_mComb2);
}

void CDgnGenLcomServBSDlg::InitLcomData()
{
	m_mUnSel.RemoveAll();
	m_mComb1.RemoveAll();
	m_mComb2.RemoveAll();

	_DGN_LCOM_D DgnLcomD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iConLcomNum = 0;

	CArray<T_LCOM_K, T_LCOM_K> TempKList, LcomKList;
	TempKList.RemoveAll();
	LcomKList.RemoveAll();
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, TempKList);
	LcomKList.Copy(TempKList);
	iConLcomNum = TempKList.GetSize();
	
	int iSeqNum = 0;
	T_LCOM_D LcomD;
	for (int i = 0; i < LcomKList.GetSize(); i++)
	{
		T_LCOM_K LcomK = LcomKList.GetAt(i);
		LcomD.Initialize();
		if (i < iConLcomNum)
		{
			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD)) continue;
		}
	
		if (LcomD.nActive != 2)  continue; // service load combination

		DgnLcomD.Initialize();
		if (LcomD.nServLcomType == 1)
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_mComb1.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else if (LcomD.nServLcomType == 2)
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_mComb2.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else  // not assigned.
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_mUnSel.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
	}
}



void CDgnGenLcomServBSDlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap)
{
	LoadList.DeleteAllItems();

	LV_ITEM lvitem;
	LPTSTR szText[2];

	_DGN_LCOM_D Load; Load.Initialize();

	int iLoadCount = LoadMap.GetCount();
	int* pSortLoad = new int[iLoadCount];

	int Index = 0;
	int iCount = 0;
	POSITION Pos = LoadMap.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LoadMap.GetNextAssoc(Pos, Index, Load);
		pSortLoad[iCount] = Index;
		iCount++;
	}

	// Sorting by Ascending order.
	CQSort::QSortInt(pSortLoad, iLoadCount);

	for (iCount = 0; iCount < iLoadCount; iCount++)
	{
		Load.Initialize();
		LoadMap.Lookup(pSortLoad[iCount], Load);

		CString strSeq = _T(""); strSeq.Format(_T("%d"), Load.iSeqNum);
		szText[0] = (LPTSTR)(LPCTSTR)Load.strLoadCombName;
		szText[1] = (LPTSTR)(LPCTSTR)strSeq;
		// Input ListBox.
		for (int i = 0; i < 2; i++)
		{
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iCount;
			lvitem.iSubItem = i;
			lvitem.pszText = szText[i];
			if (i == 0)	LoadList.InsertItem(&lvitem);
			else			LoadList.SetItem(&lvitem);
		}
	}

	delete[] pSortLoad;
}




/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServBSDlg message handlers

BOOL CDgnGenLcomServBSDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();

	UpdateData(FALSE);
	return TRUE;
}



void CDgnGenLcomServBSDlg::OnDgnAddComb1Btn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aUnSelList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aUnSelList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aUnSelList.GetItemText(nItem, 1));
			Load.Initialize();
			m_mUnSel.Lookup(iSeqNum, Load);
			m_mUnSel.RemoveKey(iSeqNum);

			Load.nServLcomType = 1;
			m_mComb1.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aUnSelList, m_mUnSel);
		Write_ListCtrl(m_aComb1List, m_mComb1);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServBSDlg::OnDgnDelComb1Btn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aComb1List.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aComb1List.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aComb1List.GetItemText(nItem, 1));
			Load.Initialize();
			m_mComb1.Lookup(iSeqNum, Load);
			m_mComb1.RemoveKey(iSeqNum);

			Load.nServLcomType = 0;
			m_mUnSel.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aUnSelList, m_mUnSel);
		Write_ListCtrl(m_aComb1List, m_mComb1);
	}
	UpdateData(FALSE);

}

void CDgnGenLcomServBSDlg::OnDgnAddComb2Btn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aUnSelList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aUnSelList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aUnSelList.GetItemText(nItem, 1));
			Load.Initialize();
			m_mUnSel.Lookup(iSeqNum, Load);
			m_mUnSel.RemoveKey(iSeqNum);

			Load.nServLcomType = 2;
			m_mComb2.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aUnSelList, m_mUnSel);
		Write_ListCtrl(m_aComb2List, m_mComb2);
	}
	UpdateData(FALSE);

}

void CDgnGenLcomServBSDlg::OnDgnDelComb2Btn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aComb2List.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aComb2List.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aComb2List.GetItemText(nItem, 1));
			Load.Initialize();
			m_mComb2.Lookup(iSeqNum, Load);
			m_mComb2.RemoveKey(iSeqNum);

			Load.nServLcomType = 0;
			m_mUnSel.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aUnSelList, m_mUnSel);
		Write_ListCtrl(m_aComb2List, m_mComb2);
	}
	UpdateData(FALSE);
}


void CDgnGenLcomServBSDlg::OnOK()
{
	// TODO: Add extra validation here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	_DGN_LCOM_D Load;
	T_LCOM_D LcomD;

	int Index = 0;
	POSITION Pos = m_mComb1.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_mComb1.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if (!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		LcomD.nServLcomType = Load.nServLcomType;
		if (!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) ASSERT(0);
	}

	Pos = m_mComb2.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_mComb2.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if (!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	Pos = m_mUnSel.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_mUnSel.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if (!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	CDialogMove::OnOK();
}


void CDgnGenLcomServBSDlg::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialogMove::OnCancel();
}
