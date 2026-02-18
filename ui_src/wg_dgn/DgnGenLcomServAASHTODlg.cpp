#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLcomServTypeDlg.h"
#include "DgnGenLcomServAASHTODlg.h"

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
// CDgnGenLcomServAASHTODlg dialog

CDgnGenLcomServAASHTODlg::CDgnGenLcomServAASHTODlg(CWnd* pParent /*=NULL*/)
 : CDialogMove(CDgnGenLcomServAASHTODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLcomServAASHTODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arServLcomMAP.RemoveAll();
	m_arServLimit1MAP.RemoveAll();
	m_arServLimit3MAP.RemoveAll();

	
	//   m_arServLimit1MAP.RemoveAll();
//   m_arServLimit3MAP.RemoveAll();
//   m_arRareLcomMAP.RemoveAll();
}


void CDgnGenLcomServAASHTODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLcomServAASHTODlg)
	DDX_Control(pDX, IDC_DGN_SERV_LCOM_LIST,  m_aServLcomList);
	DDX_Control(pDX, IDC_DGN_SERV_LIMIT1_LIST,  m_aServLimit1List);
	DDX_Control(pDX, IDC_DGN_SERV_LIMIT3_LIST,  m_aServLimit3List);

//   DDX_Control(pDX, IDC_DGN_SERV_RARE_LIST,  m_aRareList    ); 1
//   DDX_Control(pDX, IDC_DGN_SERV_FREQ_LIST,  m_aServLimit3List); 2
	//   DDX_Control(pDX, IDC_DGN_SERV_QUASI_LIST, m_aServLimit1List   ); 3

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenLcomServAASHTODlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnGenLcomServAASHTODlg)
	ON_BN_CLICKED(IDC_DGN_SERV_LIMIT1_ADD_BTN, OnDgnAddLimit1Btn )
	ON_BN_CLICKED(IDC_DGN_SERV_LIMIT1_DEL_BTN, OnDgnDelLimit1Btn )
	ON_BN_CLICKED(IDC_DGN_SERV_LIMIT3_ADD_BTN, OnDgnAddLimit3Btn)
	ON_BN_CLICKED(IDC_DGN_SERV_LIMIT3_DEL_BTN, OnDgnDelLimit3Btn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CDgnGenLcomServAASHTODlg::InitCtrl()
{
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
	
	//ServeLimit1
	DWORD ServeLimit1Style;
	ServeLimit1Style = ListView_GetExtendedListViewStyle(m_aServLimit1List.GetSafeHwnd());
	ServeLimit1Style |= LVS_EX_FULLROWSELECT; 
	ServeLimit1Style |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLimit1List.GetSafeHwnd(),ServeLimit1Style);
	
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aServLimit1List.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aServLimit1List, m_arServLimit1MAP);
	
	//ServeLimit3
	DWORD ServeLimit3Style;
	ServeLimit3Style = ListView_GetExtendedListViewStyle(m_aServLimit3List.GetSafeHwnd());
	ServeLimit3Style |= LVS_EX_FULLROWSELECT; 
	ServeLimit3Style |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLimit3List.GetSafeHwnd(),ServeLimit3Style);
	
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aServLimit3List.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aServLimit3List, m_arServLimit3MAP);
	
}


void CDgnGenLcomServAASHTODlg::InitLcomData()
{
	m_arServLcomMAP.RemoveAll();
	m_arServLimit1MAP.RemoveAll();
	m_arServLimit3MAP.RemoveAll();
	
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
	
	//TempKList.RemoveAll();
	//pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, TempKList);
	//LcomKList.Append(TempKList);
	
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
		//else
		//{
		//  //if(!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) continue;
		//}
		
		if(LcomD.nActive != 2)  continue; // service load combination
		
		DgnLcomD.Initialize();
		if(LcomD.nServLcomType == 1)  // ServLimit1
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_arServLimit1MAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else if(LcomD.nServLcomType == 3)  // ServLimit3
		{
			DgnLcomD.Lcomkey = LcomK;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = (i < iConLcomNum);
			m_arServLimit3MAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;    
		}
		else  // not assigned.
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



void CDgnGenLcomServAASHTODlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap)
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
// CDgnGenLcomServAASHTODlg message handlers

BOOL CDgnGenLcomServAASHTODlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrl();
	
	UpdateData(FALSE);
	return TRUE;
}



void CDgnGenLcomServAASHTODlg::OnDgnAddLimit1Btn() 
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
			//Load.bElasticStage = 2;
			Load.nServLcomType  = 1;
			m_arServLimit1MAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLimit1List, m_arServLimit1MAP);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServAASHTODlg::OnDgnDelLimit1Btn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int iSelectedCount = m_aServLimit1List.GetSelectedCount();
	int  nItem = -1;
	
	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLimit1List.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLimit1List.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLimit1MAP.Lookup(iSeqNum, Load);
			m_arServLimit1MAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 0;
			Load.nServLcomType = 0;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLimit1List, m_arServLimit1MAP);
	}
	UpdateData(FALSE);
	
}

void CDgnGenLcomServAASHTODlg::OnDgnAddLimit3Btn() 
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
			
			//Load.bElasticStage = 3;
			Load.nServLcomType = 3;
			m_arServLimit3MAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLimit3List, m_arServLimit3MAP);
	}
	UpdateData(FALSE);
	
}

void CDgnGenLcomServAASHTODlg::OnDgnDelLimit3Btn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int iSelectedCount = m_aServLimit3List.GetSelectedCount();
	int  nItem = -1;
	
	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLimit3List.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLimit3List.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLimit3MAP.Lookup(iSeqNum, Load);
			m_arServLimit3MAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 0;
			Load.nServLcomType = 0;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aServLimit3List, m_arServLimit3MAP);
	}
	UpdateData(FALSE);
}


void CDgnGenLcomServAASHTODlg::OnOK() 
{
	// TODO: Add extra validation here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	_DGN_LCOM_D Load; 
	T_LCOM_D LcomD; 
	
	int Index = 0;
	POSITION Pos = m_arServLimit1MAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLimit1MAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		
		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		if(!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) ASSERT(0);
	}
	
	Pos = m_arServLimit3MAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLimit3MAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		
		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}
	 
	Pos = m_arServLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;
		
		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}
	
	CDialogMove::OnOK();
}


void CDgnGenLcomServAASHTODlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}
