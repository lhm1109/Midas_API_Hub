// DgnGenLrdfCaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLrdfCaseDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfCaseDlg dialog


CDgnGenLrdfCaseDlg::CDgnGenLrdfCaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenLrdfCaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLrdfCaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDgnGenLrdfCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLrdfCaseDlg)
	DDX_Control(pDX, IDC_DGN_REDUCSEL_LIST, m_SelListCtrl);
	DDX_Control(pDX, IDC_DGN_REDUCLIVE_LIST, m_ListCtrl);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenLrdfCaseDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnGenLrdfCaseDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_DGN_REDUCLIVE_LIST, OnDblclkDgnReducliveList)
	ON_BN_CLICKED(IDC_DGN_REDUCLIVE_SEL, OnDgnReducliveSel)
	ON_BN_CLICKED(IDC_DGN_REDUCSEL_DEL, OnDgnReducselDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfCaseDlg message handlers

BOOL CDgnGenLrdfCaseDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	if(m_LiveData.GetSize() != 0)    m_LiveData.RemoveAll();
	if(m_SelLiveData.GetSize() != 0) m_SelLiveData.RemoveAll();

	Initial_ListCtrlBox();
	Initial_SelListCtrlBox();
	Initial_LoadCase();

	if(m_LoadCaseNames != _T("")) Initial_SelLoadCase(m_LoadCaseNames);

	return TRUE;
}

void CDgnGenLrdfCaseDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[4]	= { (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NUMBER),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_TYPE),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_DESCRIPTION) };
	int width[4]	= {50,80,80,170};
	for(int i=0; i<4; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

		if(i == 3)  lvcolumn.fmt = LVCFMT_LEFT;
		else        lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnGenLrdfCaseDlg::Initial_SelListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_SelListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_SelListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[4]	= { (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NUMBER),
					    (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME),
					    (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_TYPE),
						(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_DESCRIPTION)};
	int width[4]	= {50,80,80,170};
	for(int i=0; i<4; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

		if(i == 3)  lvcolumn.fmt = LVCFMT_LEFT;
		else        lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_SelListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnGenLrdfCaseDlg::Initial_LoadCase()
{
	CArray<T_STLD_K,T_STLD_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);

	int ListLineNo = m_ListCtrl.GetItemCount();

	int iCount = KeyList.GetSize();
	for(int i=0; i<iCount; i++)
	{
		int key = KeyList.GetAt(i);
		T_STLD_D rData;
		rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetStld(key,rData))	continue;

		if(rData.LoadCaseType == _T("L") || rData.LoadCaseType == _T("LR"))
		{
			_DGN_REDU Data;
			Data.sDescript = rData.Description;
			Data.iLcasNo	 = rData.LoadCaseId;
			Data.sLcasNa	 = rData.LoadCaseName;
			Data.sLcasType = rData.LoadCaseType;

			m_LiveData.Add(Data);

			CString strNo=_T(""),strName=_T(""),strType=_T(""),strDes=_T("");

			strNo.Format(_T("%7d"),rData.LoadCaseId);
			strName.Format(_T("%s"),rData.LoadCaseName);
			strType.Format(_T("Live Load"));
			strDes.Format(_T("%s"),rData.Description);

			Write_ListCtrlBox(ListLineNo,strNo,strName,strType,strDes);

			ListLineNo++;
		}
	}
}

void CDgnGenLrdfCaseDlg::Initial_SelLoadCase(CString strLoadNo)
{
	CString temp=_T("");
	CArray<CString,CString> LcName;
	int Len = strLoadNo.GetLength();
	for(int i=0; i<Len; i++)
	{
		TCHAR ch = strLoadNo.GetAt(i);
		if(ch == ',')
		{
			LcName.Add(temp);
			temp = _T("");
		}
		else
		{
			temp=temp+ch;
			if(i+1 == Len)
			{
				LcName.Add(temp);
				temp = _T("");
			}
		}
	}

	int iCount = LcName.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString Name = LcName.GetAt(i);

		_DGN_REDU rData;
		rData = Search_LiveLoadData(Name);

		int ListLineNo = m_SelListCtrl.GetItemCount();

		CString strNo=_T(""),strType=_T("");
		strNo.Format(_T("%7d"),rData.iLcasNo);
		strType.Format(_T("Live Load"));

		if(!IsEQLoadCase(strNo))
			 Write_SelListCtrlBox(ListLineNo,strNo,rData.sLcasNa,strType,rData.sDescript);
	}
}

_DGN_REDU CDgnGenLrdfCaseDlg::Search_LiveLoadData(CString name)
{
	_DGN_REDU Data;

	int iCount = m_LiveData.GetSize();
	for(int i=0; i<iCount; i++)
	{
		Data = m_LiveData.GetAt(i);
		if(Data.sLcasNa==name)  return Data;
	}
	return Data;
}

void CDgnGenLrdfCaseDlg::Write_ListCtrlBox(int Index,CString strNo,CString strName,
																					CString strType,CString strDes)
{
	LV_ITEM lvitem;

	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strNo;
	m_ListCtrl.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem	  = Index;
	lvitem.iSubItem = 2;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strType;
	m_ListCtrl.SetItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 3;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strDes;
	m_ListCtrl.SetItem(&lvitem);
}

void CDgnGenLrdfCaseDlg::Write_SelListCtrlBox(int Index,CString strNo,CString strName,
																						 CString strType,CString strDes)
{
	LV_ITEM lvitem;

	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strNo;
	m_SelListCtrl.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
	m_SelListCtrl.SetItem(&lvitem);

	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem	  = Index;
	lvitem.iSubItem = 2;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strType;
	m_SelListCtrl.SetItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 3;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strDes;
	m_SelListCtrl.SetItem(&lvitem);
}

void CDgnGenLrdfCaseDlg::OnDblclkDgnReducliveList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);

	if(nItem != -1)
	{
		CString strNO=_T("");
		strNO = m_ListCtrl.GetItemText(nItem,0);
		if(IsEQLoadCase(strNO))
		{
			CString strMeg=_T("");
			strMeg.Format(_LS(IDS_DGN_CHK_LCASE),strNO);
			AfxMessageBox(strMeg,MB_OK);
		}
		else
		{
			_DGN_REDU rData;
			rData = m_LiveData.GetAt(nItem);

			int ListLineNo = m_SelListCtrl.GetItemCount();

			CString strNo=_T(""),strType=_T("");
			strNo.Format(_T("%7d"),rData.iLcasNo);
			strType.Format(_T("Live Load"));

			Write_SelListCtrlBox(ListLineNo,strNo,rData.sLcasNa,strType,rData.sDescript);
		}
	}
	*pResult = 0;
}

BOOL CDgnGenLrdfCaseDlg::IsEQLoadCase(CString strno)
{
	BOOL bCheck=FALSE;

	int iCount = m_SelListCtrl.GetItemCount();
	for(int i=0; i<iCount; i++)
	{
		CString strNO=_T("");
		strNO = m_SelListCtrl.GetItemText(i,0);
		if(strno == strNO) bCheck=TRUE;
	}
	return bCheck;
}

void CDgnGenLrdfCaseDlg::OnDgnReducliveSel() 
{
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)  AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM),MB_OK);
	else
	{
		while (pos)
		{
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);

			_DGN_REDU rData;
			rData = m_LiveData.GetAt(nItem);

			int ListLineNo = m_SelListCtrl.GetItemCount();

			CString strNo=_T(""),strType=_T("");
			strNo.Format(_T("%7d"),rData.iLcasNo);
			strType.Format(_T("Live Load"));

			if(!IsEQLoadCase(strNo))
				 Write_SelListCtrlBox(ListLineNo,strNo,rData.sLcasNa,strType,rData.sDescript);
		}
	}
}

void CDgnGenLrdfCaseDlg::OnDgnReducselDel() 
{
	POSITION pos = m_SelListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)  AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM),MB_OK);
	else
	{
		CArray<int,int> SelItem;
		while (pos) 
		{
			int nItem = m_SelListCtrl.GetNextSelectedItem(pos);
			SelItem.Add(nItem);
		}
		//  뒤에서부터 지워야 됩니다.
		int iCount = SelItem.GetSize();
		for(int i=iCount-1; i>-1; i--)
		{
			int Index = SelItem.GetAt(i);
			m_SelListCtrl.DeleteItem(Index);
		}
	}
}

void CDgnGenLrdfCaseDlg::OnOK() 
{
	m_LoadCaseNames = _T("");
	CString strTotal=_T("");

	int iCount = m_SelListCtrl.GetItemCount();
	if(iCount < 6)
	{
		for(int i=0; i<iCount; i++)
		{
			CString strName=_T("");
			strName = m_SelListCtrl.GetItemText(i,1);

			if(i==0)  strTotal = strTotal+strName;
			else      strTotal = strTotal+_T(",")+strName;

			_DGN_REDU rData;
			rData.Initialize();

			rData.iLcasNo		= _ttoi(m_SelListCtrl.GetItemText(i,0));
			rData.sLcasNa		= strName;
			rData.sLcasType	= m_SelListCtrl.GetItemText(i,2);
			rData.sDescript	= m_SelListCtrl.GetItemText(i,3);

			m_SelLiveData.Add(rData);
		}
		m_LoadCaseNames = strTotal;

	  CDialogMove::OnOK();
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_SEL_OVER_ITEM),MB_OK);
}

void CDgnGenLrdfCaseDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}
