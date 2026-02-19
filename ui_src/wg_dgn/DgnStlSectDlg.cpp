// DgnStlSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSectDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectDlg dialog


CDgnStlSectDlg::CDgnStlSectDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlSectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlSectDlg)
	m_yzList[0]=_T("a");
	m_yzList[1]=_T("a*");
	m_yzList[2]=_T("b");
	m_yzList[3]=_T("b*");
	m_yzList[4]=_T("c");
	m_yzList[5]=_T("d*");
	//}}AFX_DATA_INIT
}


void CDgnStlSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlSectDlg)
	DDX_Control(pDX, IDD_DGN_STEELSECT_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_STL_SECT_Y, m_SectY);
	DDX_Control(pDX, IDC_COMBO_STL_SECT_Z, m_SectZ);
	//}}AFX_DATA_MAP
} 

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlSectDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlSectDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDD_DGN_STEELSECT_LIST, OnItemchangedDgnSteelSectList)
	ON_NOTIFY(NM_CLICK, IDD_DGN_STEELSECT_LIST, OnClickDgnSteelSectList)
	ON_CBN_SELCHANGE(IDC_COMBO_STL_SECT_Y, OnSelchangeY)
	ON_CBN_SELCHANGE(IDC_COMBO_STL_SECT_Z, OnSelchangeZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSectDlg message handlers

BOOL CDgnStlSectDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Initial_ListCtrl();
	Initial_Data();
	//ControlMan(TRUE);
	
	for(int i=0; i<6; i++)
	{
		m_SectY.AddString(m_yzList[i]);  
		m_SectZ.AddString(m_yzList[i]);  
	}
	m_SectY.SetCurSel(0);
	m_SectZ.SetCurSel(0);
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnStlSectDlg::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[6]	= {_T("截面号"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("类型"), _T("形状"), _T("截面分类(y轴)"), _T("截面分类(z轴)")};
	int width[6]	= {60,140,60,40,100,100};
	for(int i=0; i<6; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnStlSectDlg::Initial_Data()
{
	//Get From DB
	//CArray<CString, CString&> rstrSectList;
	//m_pDoc->m_pAttrCtrl->GetSectListDesign(rstrSectList);  //Key 锅龋肺 沥纺等 府胶飘
	CArray<T_MATL_K,T_MATL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(KeyList);
	int nItemCount = KeyList.GetSize();
	//int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	//int nItemCount = rstrSectList.GetSize();
	if(nItemCount == 0) return;
	
		T_SECT_K Key;
		T_SECT_D Data;
	CString str=_T("");
	//POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSect();
	for(int i=0; i<nItemCount; i++)
	//while(pos !=NULL)
	{
		Key = KeyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetSectDesign(Key,Data))	continue;
			//m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
	  BOOL bOK = FALSE;
	  if(Data.nStype == D_SECT_TYPE_REGULAR)
	  {
				if (Data.SectBefore.SectI.DBName == _T("")) str = _LS(IDS_WG_CMD__ADD2__User);
				else str =  _LS(IDS_WG_CMD__ADD2__DB);
		bOK = TRUE;
	  }
	  else if(Data.nStype == D_SECT_TYPE_USER)
	  {
		  str = _LS(IDS_WG_CMD__ADDD__Value);    
		  bOK = TRUE;
	  }
	  if(bOK)
	  {
		  T_SLSD_D slsd;
		  CString sy=m_yzList[0], sz=m_yzList[0];
		  if(m_pDoc->m_pAttrCtrl->GetSlsd(Key,slsd))
		  {
			  sy=m_yzList[slsd.yType];
			  sz=m_yzList[slsd.zType];
		  }
		  int ListLineNo = m_ListCtrl.GetItemCount();
		  CString strNo; strNo.Format(_T("%6d"),Key);
		  /*
		  LV_ITEM lvitem;
		  lvitem.mask		  = LVIF_TEXT;
		  lvitem.iItem  	= ListLineNo;
		  lvitem.iSubItem = 0;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)strNo;
		  m_ListCtrl.InsertItem(&lvitem);//id
		  lvitem.iSubItem = 1;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)Data.SName;
		  m_ListCtrl.SetItem(&lvitem);//name
		  lvitem.iSubItem = 2;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)str;
		  m_ListCtrl.SetItem(&lvitem);//type
		  lvitem.iSubItem = 3;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)Data.SectBefore.Shape;
		  m_ListCtrl.SetItem(&lvitem);//shape
		  lvitem.iSubItem = 4;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)sy;
		  m_ListCtrl.SetItem(&lvitem);//Y
		  lvitem.iSubItem = 5;
		  lvitem.pszText	= (LPTSTR)(LPCTSTR)sz;
		  m_ListCtrl.SetItem(&lvitem);//z
		  */
		  m_ListCtrl.InsertItem(ListLineNo,strNo);   
		  m_ListCtrl.SetItemText(ListLineNo,1,Data.SName);  
		  m_ListCtrl.SetItemText(ListLineNo,2,str);  
		  m_ListCtrl.SetItemText(ListLineNo,3,Data.SectBefore.Shape);  
		  m_ListCtrl.SetItemText(ListLineNo,4,sy);  
		  m_ListCtrl.SetItemText(ListLineNo,5,sz); 
	  }
	}
	
	UpdateData(FALSE);
}

void CDgnStlSectDlg::OnClickDgnSteelSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnItemchangedDgnSteelSectList(pNMHDR,pResult);
	*pResult = 0;
}
void CDgnStlSectDlg::OnItemchangedDgnSteelSectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);
	
	int key = _ttoi(strID);
	T_SLSD_D mData;
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSlsd(key, mData);
	if(bCheck)
	{
		m_SectY.SetCurSel(mData.yType);
		m_SectZ.SetCurSel(mData.zType);
	}
	else 
	{
		m_SectY.SetCurSel(0);
		m_SectZ.SetCurSel(0);
	}
	
		UpdateData(FALSE);
	*pResult = 0;
}

void CDgnStlSectDlg::OnSelchangeY()
{
	int Index = m_SectY.GetCurSel();
}
void CDgnStlSectDlg::OnSelchangeZ()
{
	int Index = m_SectZ.GetCurSel();
}

void CDgnStlSectDlg::OnOK() 
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_SECT),MB_OK);
		return;
	}
	T_SLSD_D mData;
	mData.Initialize();
	CString strID = m_ListCtrl.GetItemText(nItem, 0);
	// Modify Material ID
	int nID   = _ttoi(strID);
	m_pDoc->m_pAttrCtrl->GetSlsd(nID, mData);

	UpdateData(TRUE);
	mData.yType = m_SectY.GetCurSel();
	mData.zType = m_SectZ.GetCurSel();
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddSlsd(nID, mData);
	if(bCheck)
	{	   
	  m_ListCtrl.SetItemText(nItem,4,m_yzList[mData.yType]);  
	  m_ListCtrl.SetItemText(nItem,5,m_yzList[mData.zType]); 
	  m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
//  if(!m_pDoc->m_pMatlDB->GetSteelData(strCode, strMatName, sData))	ASSERT(0);
//  mData.Data1.CodeName      = strCode;
//  mData.Data1.CodeMatlName  = strMatName;
	//CDialogMove::OnOK();
}

void CDgnStlSectDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

