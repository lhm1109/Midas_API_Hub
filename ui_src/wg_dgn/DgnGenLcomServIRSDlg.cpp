#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLcomServTypeDlg.h"
#include "DgnGenLcomServIRSDlg.h"

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

enum
{
	UNSELECT = 0,
	SELECT   = 1
};

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServdPIDlg dialog

CDgnGenLcomServIRSDlg::CDgnGenLcomServIRSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenLcomServIRSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLcomServIRSDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arServLcomMAP.RemoveAll();
	m_arServLcomSelectMAP.RemoveAll();
}


void CDgnGenLcomServIRSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLcomServIRSDlg)
	DDX_Control(pDX, IDC_DGN_GEN_SERV_LCOM_LIST,  m_aServLcomList);
	DDX_Control(pDX, IDC_DGN_GEN_SERV_SELECT_LCOM_LIST,  m_aServLcomSelectList);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenLcomServIRSDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnGenLcomServIRSDlg)
	ON_BN_CLICKED(IDC_DGN_GEN_SERV_SELECT,   OnDgnAddSelectBtn )
	ON_BN_CLICKED(IDC_DGN_GEN_SERV_UNSELECT, OnDgnDelSelectBtn )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnGenLcomServIRSDlg::InitCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

#if defined(_CIVIL)
	T_PSCD_D rData;
	
	if(pDoc->m_pAttrCtrl->GetDgnPscd(rData))
	{
		if(rData.iDgnCode == IRS_PSC)
		{
			this->SetWindowText(_LS(IDS_DGN_GEN_LCOM_SERV_IRS_TITLE));
			GetDlgItem(IDC_DGN_GEN_SERV_SELECT_LCOM )->SetWindowText(_LS(IDS_DGN_GEN_LCOM_SERV_IRS_COMB1));
		}
		else if (rData.iDgnCode == AS_5100_5_17_PSC)
		{
			this->SetWindowText(_LS(IDS_DGN_GEN_LCOM_SERV_AS5100_TITLE));
			GetDlgItem(IDC_DGN_GEN_SERV_SELECT_LCOM)->SetWindowText(_LS(IDS_DGN_GEN_LCOM_SERV_AS5100_TRANSFER));
		}
	}	
#endif
	InitLcomData();

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[2] = {_T("Load Comb"), _T("Seq")};
	int width[2]	 = {120,0};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aServLcomList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);

	//Selected Type
	DWORD ServSelectStyle;
	ServSelectStyle = ListView_GetExtendedListViewStyle(m_aServLcomSelectList.GetSafeHwnd());
	ServSelectStyle |= LVS_EX_FULLROWSELECT; 
	ServSelectStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLcomSelectList.GetSafeHwnd(),ServSelectStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aServLcomSelectList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aServLcomSelectList, m_arServLcomSelectMAP);
}


void CDgnGenLcomServIRSDlg::InitLcomData()
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
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, TempKList);
	LcomKList.Copy(TempKList);
	iConLcomNum = TempKList.GetSize();

	int iSeqNum=0;
	T_LCOM_D LcomD;
	for(int i=0; i<LcomKList.GetSize(); i++)
	{
		T_LCOM_K LcomK = LcomKList.GetAt(i);
		LcomD.Initialize();
		if(i < iConLcomNum)
		{
			if(!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD)) continue;
		}

		if(LcomD.nActive != 2)  continue; // service load combination

		DgnLcomD.Initialize();
		if(LcomD.nServLcomType == UNSELECT)  // service load combination
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_arServLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else if(LcomD.nServLcomType == SELECT)  // Select Type
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_arServLcomSelectMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;    
		}
		else // 할당 되었던 Type은 Select Type으로 설정함. (다른 기준에서 할당될 수 있음)
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_arServLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
	}
}



void CDgnGenLcomServIRSDlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap)
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
	while(Pos)
	{
		Load.Initialize();
		LoadMap.GetNextAssoc(Pos, Index, Load);
		pSortLoad[iCount] = Index;
		iCount++;
	}

	// Sorting by Ascending order.
	CQSort::QSortInt(pSortLoad, iLoadCount);

	for(iCount=0; iCount<iLoadCount; iCount++)
	{
		Load.Initialize();
		LoadMap.Lookup(pSortLoad[iCount], Load);

		CString strSeq = _T(""); strSeq.Format(_T("%d"), Load.iSeqNum);
		szText[0] = (LPTSTR)(LPCTSTR)Load.strLoadCombName;
		szText[1] = (LPTSTR)(LPCTSTR)strSeq;
		// Input ListBox.
		for(int i=0; i<2; i++)
		{
			lvitem.mask		  = LVIF_TEXT;
			lvitem.iItem  	= iCount;
			lvitem.iSubItem = i;
			lvitem.pszText	= szText[i];
			if(i==0)	LoadList.InsertItem(&lvitem);
			else			LoadList.SetItem(&lvitem);
		}
	}

	delete[] pSortLoad;
}




/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServIRSDlg message handlers

BOOL CDgnGenLcomServIRSDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();

	UpdateData(FALSE);
	return TRUE;
}



void CDgnGenLcomServIRSDlg::OnDgnAddSelectBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLcomList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomMAP.Lookup(iSeqNum, Load);
			m_arServLcomMAP.RemoveKey(iSeqNum);
			Load.nServLcomType  = SELECT;
			m_arServLcomSelectMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLcomSelectList, m_arServLcomSelectMAP);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServIRSDlg::OnDgnDelSelectBtn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomSelectList.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
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

void CDgnGenLcomServIRSDlg::OnOK()
{
	// TODO: Add extra validation here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	_DGN_LCOM_D Load; 
	T_LCOM_D LcomD; 

	int Index = 0;
	POSITION Pos = m_arServLcomSelectMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomSelectMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL);
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		LcomD.nServLcomType = Load.nServLcomType;
		if(!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) ASSERT(0);
	}

	Pos = m_arServLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	CDialogMove::OnOK();
}


void CDgnGenLcomServIRSDlg::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialogMove::OnCancel();
}
