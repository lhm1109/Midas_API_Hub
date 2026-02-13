#include "stdafx.h"
#include "wg_dgn.h"
//#include "DgnGenLcomServTypeDlg.h"
#include "DgnStlLcomJPDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDgnStlLcomJPDlg dialog

CDgnStlLcomJPDlg::CDgnStlLcomJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlLcomJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlLcomJPDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arServLcomMAP.RemoveAll();
	m_arServLcomSelectMAP.RemoveAll();
}


void CDgnStlLcomJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlLcomJPDlg)
	DDX_Control(pDX, IDC_DGN_GEN_SERV_SELECT_FACTOR_EDT, m_edtFactor);
	DDX_Control(pDX, IDC_DGN_GEN_SERV_LCOM_LIST, m_aServLcomList);
	DDX_Control(pDX, IDC_DGN_GEN_SERV_SELECT_LCOM_LIST, m_aServLcomSelectList);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlLcomJPDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlLcomJPDlg)
	ON_BN_CLICKED(IDC_DGN_GEN_SERV_SELECT, OnDgnAddSelectBtn)
	ON_BN_CLICKED(IDC_DGN_GEN_SERV_UNSELECT, OnDgnDelSelectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnStlLcomJPDlg::InitCtrl()
{
	m_edtFactor.SetUnitType(D_UNITSYS_NONE);

	InitLcomData();

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd(), dwStyle);

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
		m_aServLcomList.InsertColumn(i, &lvcolumn);
	}
	Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);

	//Selected Type
	DWORD ServSelectStyle;
	ServSelectStyle = ListView_GetExtendedListViewStyle(m_aServLcomSelectList.GetSafeHwnd());
	ServSelectStyle |= LVS_EX_FULLROWSELECT;
	ServSelectStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLcomSelectList.GetSafeHwnd(), ServSelectStyle);

	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_aServLcomSelectList.InsertColumn(i, &lvcolumn);
	}
	Write_ListCtrl(m_aServLcomSelectList, m_arServLcomSelectMAP);
}


void CDgnStlLcomJPDlg::InitLcomData()
{
	m_arServLcomMAP.RemoveAll();
	m_arServLcomSelectMAP.RemoveAll();

	_DGN_LCOM_D DgnLcomD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iConLcomNum = 0;

	CArray<T_LCOM_K, T_LCOM_K> TempKList, LcomKList;
	TempKList.RemoveAll();
	LcomKList.RemoveAll();
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, TempKList);
	LcomKList.Copy(TempKList);
	iConLcomNum = TempKList.GetSize();

	int iSeqNum = 0;
	int iSelectNum = 0;
	T_LCOM_D LcomD;
	T_LCOM_D LcomSelectD; LcomSelectD.Initialize();
	for (int i = 0; i < LcomKList.GetSize(); i++)
	{
		T_LCOM_K LcomK = LcomKList.GetAt(i);
		LcomD.Initialize();
		if (i < iConLcomNum)
		{
			if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) continue;
		}

		if (LcomD.nActive == 0)  continue; // 0:inactive

		DgnLcomD.Initialize();
		if (LcomD.nServLcomType == UNSELECT)  // service load combination
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = FALSE;
			m_arServLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else if (LcomD.nServLcomType == SELECT)  // Select Type
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = FALSE;
			m_arServLcomSelectMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
			iSelectNum++;

			LcomSelectD = LcomD;
		}
		else // 할당 되었던 Type은 Select Type으로 설정함. (다른 기준에서 할당될 수 있음)
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = FALSE;
			m_arServLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
	}

	if(iSelectNum > 0)
		m_edtFactor.SetEditUnit(LcomSelectD.dLcomFactor);  // 현재는 선택된 Lcom의 모든 Factor는 동일함.
	else 
		m_edtFactor.SetEditUnit(1.0);
}



void CDgnStlLcomJPDlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap)
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
			else		LoadList.SetItem(&lvitem);
		}
	}

	delete[] pSortLoad;
}




/////////////////////////////////////////////////////////////////////////////
// CDgnStlLcomJPDlg message handlers

BOOL CDgnStlLcomJPDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();

	UpdateData(FALSE);
	return TRUE;
}



void CDgnStlLcomJPDlg::OnDgnAddSelectBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aServLcomList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomMAP.Lookup(iSeqNum, Load);
			m_arServLcomMAP.RemoveKey(iSeqNum);
			Load.nServLcomType = SELECT;
			m_arServLcomSelectMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLcomSelectList, m_arServLcomSelectMAP);
	}
	UpdateData(FALSE);
}

void CDgnStlLcomJPDlg::OnDgnDelSelectBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomSelectList.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load;
	// Update all of the selected items.
	if (iSelectedCount > 0)
	{
		for (int i = 0; i < iSelectedCount; i++)
		{
			nItem = m_aServLcomSelectList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomSelectList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomSelectMAP.Lookup(iSeqNum, Load);
			m_arServLcomSelectMAP.RemoveKey(iSeqNum);
			Load.nServLcomType = UNSELECT;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLcomSelectList, m_arServLcomSelectMAP);
	}
	UpdateData(FALSE);

}

void CDgnStlLcomJPDlg::OnOK()
{
	// TODO: Add extra validation here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

#if defined(_CIVIL_JP)
	if (!pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	_DGN_LCOM_D Load;
	T_LCOM_D LcomD;

 	double dFactor = m_edtFactor.GetEditValue();
// 	if (dFactor <= 0.0)
// 	{
// 		if (dFactor <= 0) return CheckError(_LS(IDS_DB_FACTOR));
// 	}

	int Index = 0;
	POSITION Pos = m_arServLcomSelectMAP.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomSelectMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL);
		if (!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		LcomD.nServLcomType = Load.nServLcomType;
		LcomD.dLcomFactor = dFactor;
		if (!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) return;
	}

	Pos = m_arServLcomMAP.GetStartPosition();
	while (Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if (!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		LcomD.nServLcomType = Load.nServLcomType;
		if (!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) return;
	}

	CDialogMove::OnOK();
}


void CDgnStlLcomJPDlg::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialogMove::OnCancel();
}