// DgnConBarSpaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeSeisDefDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\AfxTemplEx.h"
#include "DgnForceCtrl.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg dialog

CDgnConCodeSeisDefDlg::CDgnConCodeSeisDefDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConCodeSeisDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarSpaceDlg)
	m_strCode = _T("");
	//}}AFX_DATA_INIT
}

CDgnConCodeSeisDefDlg::CDgnConCodeSeisDefDlg(CString strCode, CWnd* pParent /*=NULL*/)
: CDialogMove(CDgnConCodeSeisDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarSpaceDlg)
	m_strCode = strCode;
	
	m_aAllStructGroupK.RemoveAll();
	m_aExcludeGroupK.RemoveAll();
	m_aNotSFSRGroupK.RemoveAll();

	m_aAllStructGroupName.RemoveAll();
	m_aStructGroupName.RemoveAll();
	m_aExcludeGroupName.RemoveAll();
	m_aNotSFSRGroupName.RemoveAll();

	m_LastGrupK = NULL;
	
	//}}AFX_DATA_INIT
}


void CDgnConCodeSeisDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeSeisDefDlg)
	DDX_Control(pDX, IDC_DGN_CON_SEIS_STRUCT_GROUP_LST,   m_StrucGroupList);
	DDX_Control(pDX, IDC_DGN_CON_SEIS_EXCLUDE_GROUP_LST,  m_ExcludeList);
	DDX_Control(pDX, IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_LST, m_NotSFSRList);
	
	DDX_Control(pDX, IDC_DGN_DEFINE_MAIN_ASC_BTN, m_MainSortAscBtn);
	DDX_Control(pDX, IDC_DGN_DEFINE_MAIN_DSC_BTN, m_MainSortDSCBtn);
	DDX_Control(pDX, IDC_DGN_DEFINE_SUB1_ASC_BTN, m_Sub1SortAscBtn);
	DDX_Control(pDX, IDC_DGN_DEFINE_SUB1_DSC_BTN, m_Sub1SortDSCBtn);
	DDX_Control(pDX, IDC_DGN_DEFINE_SUB2_ASC_BTN, m_Sub2SortAscBtn);
	DDX_Control(pDX, IDC_DGN_DEFINE_SUB2_DSC_BTN, m_Sub2SortDSCBtn);
	//}}AFX_DATA_MAP  
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConCodeSeisDefDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConCodeSeisDefDlg)

	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_AUTO_BTN,          OnDgnAutoGroup)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_OK_BTN,            OnOK)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_CANCEL_BTN,        OnCancel)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_EXCLUDE_ADD_BTN,   OnDgnAddExcludeBtn)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_EXCLUDE_DEL_BTN,   OnDgnDelExcludeBtn)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_NOT_SFSR_ADD_BTN,  OnDgnAddNotSFSRBtn)
	ON_BN_CLICKED(IDC_DGN_CON_DEFINE_NOT_SFSR_DEL_BTN,  OnDgnDelNotSFSRBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_MAIN_ASC_BTN,          OnDgnMainASCButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_MAIN_DSC_BTN,          OnDgnMainDSCButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SUB1_ASC_BTN,          OnDgnSub1ASCButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SUB1_DSC_BTN,          OnDgnSub1DSCButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SUB2_ASC_BTN,          OnDgnSub2ASCButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SUB2_DSC_BTN,          OnDgnSub2DSCButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg message handlers


void CDgnConCodeSeisDefDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnConCodeSeisDefDlg::OnOK() 
{
	// Save Concrete Control Data at DB.
	CDBDoc* pDBDoc = CDBDoc::GetDocPoint(); ASSERT(pDBDoc);
	
	m_aAllStructGroupK.RemoveAll();
	m_aExcludeGroupK.RemoveAll();
	m_aNotSFSRGroupK.RemoveAll();

	int nSize = m_aExcludeGroupName.GetSize();
	m_aExcludeGroupK.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		T_GRUP_K key = GetGrupKey(m_aExcludeGroupName[i]);
		m_aExcludeGroupK.SetAt(i, key);
	}

	nSize = m_aNotSFSRGroupName.GetSize();
	m_aNotSFSRGroupK.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		T_GRUP_K key = GetGrupKey(m_aNotSFSRGroupName[i]);
		m_aNotSFSRGroupK.SetAt(i, key);
	}

	nSize = m_aAllStructGroupName.GetSize();
	m_aAllStructGroupK.SetSize(nSize);
	for(int i=0; i<nSize; i++)
	{
		T_GRUP_K key = GetGrupKey(m_aAllStructGroupName[i]);
		T_GRUP_D data;
		if(!m_arAllGroup.Lookup(key, data)) {ASSERT(0); continue;}
		data.nGroupId = i+1;
		m_aAllStructGroupK.SetAt(i, key);
		m_arAllGroup.SetAt(key, data);
	}

	CDialogMove::OnOK();
}

void CDgnConCodeSeisDefDlg::InitControlByCode()
{
	if(m_strCode==_T("Eurocode2:04"))
	{
		m_aNotSFSRGroupK.RemoveAll();

		this->SetWindowText(_LS(IDS_DGN_CON_CODE_EURO2_04_TITLE));
		GetDlgItem(IDC_DGN_CON_SEIS_EXCLUDE_GROUP_STC)->SetWindowText(_LS(IDS_DGN_CON_CODE_EURO2_04_SUB_TITLE));
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_STC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DEFINE_SUB2_ASC_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DEFINE_SUB2_DSC_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_DEFINE_NOT_SFSR_ADD_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_DEFINE_NOT_SFSR_DEL_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_LST)->EnableWindow(FALSE);
	}
	else if(m_strCode==_T("KCI-USD07"))
	{
		this->SetWindowText(_LS(IDS_DGN_CON_CODE_KCI_USD07_TITLE));
		GetDlgItem(IDC_DGN_CON_SEIS_EXCLUDE_GROUP_STC)->SetWindowText(_LS(IDS_DGN_CON_CODE_KCI_USD07_SUB_TITLE));
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_STC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DEFINE_SUB2_ASC_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DEFINE_SUB2_DSC_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_DEFINE_NOT_SFSR_ADD_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_DEFINE_NOT_SFSR_DEL_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_SEIS_NOT_SFSR_GROUP_LST)->EnableWindow(TRUE);
	}
	else ASSERT(0);
}

BOOL CDgnConCodeSeisDefDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitControlByCode();

	m_MainSortAscBtn.SetIcon(IDI_DGN_SORT_ASC);
	m_MainSortAscBtn.SetFlat(TRUE);
	m_MainSortAscBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	//m_MainSortAscBtn.SetTooltipText(_LS(IDS_CMD_GRUP_SORT_ASC));

	m_MainSortDSCBtn.SetIcon(IDI_DGN_SORT_DSC);
	m_MainSortDSCBtn.SetFlat(TRUE);
	m_MainSortDSCBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	//m_MainSortDSCBtn.SetTooltipText(_LS(IDS_CMD_GRUP_SORT_ASC));

	m_Sub1SortAscBtn.SetIcon(IDI_DGN_SORT_ASC);
	m_Sub1SortAscBtn.SetFlat(TRUE);
	m_Sub1SortAscBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	//m_Sub1SortAscBtn.SetTooltipText(_LS(IDS_CMD_GRUP_SORT_ASC));

	m_Sub1SortDSCBtn.SetIcon(IDI_DGN_SORT_DSC);
	m_Sub1SortDSCBtn.SetFlat(TRUE);
	m_Sub1SortDSCBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	//m_Sub1SortDSCBtn.SetTooltipText(_LS(IDS_CMD_GRUP_SORT_ASC));

	m_Sub2SortAscBtn.SetIcon(IDI_DGN_SORT_ASC);
	m_Sub2SortAscBtn.SetFlat(TRUE);
	m_Sub2SortAscBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);
	//m_Sub2SortAscBtn.SetTooltipText(_LS(IDS_CMD_GRUP_SORT_ASC));

	m_Sub2SortDSCBtn.SetIcon(IDI_DGN_SORT_DSC);
	m_Sub2SortDSCBtn.SetFlat(TRUE);
	m_Sub2SortDSCBtn.SetAlign(CButtonSTNew::ST_ALIGN_VERT);

	m_StrucGroupList.InsertColumn(0, _T("Col0"), LVCFMT_LEFT);  
	m_StrucGroupList.SetExtendedStyle(m_StrucGroupList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_StrucGroupList.ModifyStyle(NULL, LVS_SHOWSELALWAYS);
	m_StrucGroupList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	m_ExcludeList.InsertColumn(0, _T("Col0"), LVCFMT_LEFT);  
	m_ExcludeList.SetExtendedStyle(m_ExcludeList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_ExcludeList.ModifyStyle(NULL, LVS_SHOWSELALWAYS);
	m_ExcludeList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	
	m_NotSFSRList.InsertColumn(0, _T("Col0"), LVCFMT_LEFT);  
	m_NotSFSRList.SetExtendedStyle(m_NotSFSRList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_NotSFSRList.ModifyStyle(NULL, LVS_SHOWSELALWAYS);
	m_NotSFSRList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	BOOL bEnable = CDBDoc::GetDocPoint()->m_pPostCtrl->IsPostEnable();
	GetDlgItem(IDC_DGN_CON_DEFINE_AUTO_BTN)->EnableWindow(bEnable);

	UpdateList();
		
	return TRUE;
}

// m_aAllStructGroupK를 바탕으로 Main 과 Sub 리스트의 Name Array와 Key Map을 새로 구성한다.
void CDgnConCodeSeisDefDlg::MakeListData()
{
	CDBDoc* pDBDoc = CDBDoc::GetDocPoint(); ASSERT(pDBDoc);
	
	int i=0;
	int nKeyCount = 0;
	
	m_aStructGroupName.RemoveAll();
	m_aExcludeGroupName.RemoveAll();
	m_aNotSFSRGroupName.RemoveAll();

	CMap<T_GRUP_K, T_GRUP_K, int, int> arStructGroupK;
	CMap<T_GRUP_K, T_GRUP_K, int, int> arExcludeGroupK;
	CMap<T_GRUP_K, T_GRUP_K, int, int> arNotSFSRGroupK;
	
	arStructGroupK.RemoveAll();
	arStructGroupK.InitHashTable(11);
	
	arNotSFSRGroupK.RemoveAll();
	arNotSFSRGroupK.InitHashTable(11);
	
	arExcludeGroupK.RemoveAll();
	arExcludeGroupK.InitHashTable(11);
	
	// ExcludeGroup - Group Key로 Map 구성.
	for(i=0; i<m_aExcludeGroupK.GetSize(); i++)
	{
		arExcludeGroupK.SetAt(m_aExcludeGroupK[i], 0);
	}
	
	// NotSFSRGroup - Group Key로 Map 구성.
	for(i=0; i<m_aNotSFSRGroupK.GetSize(); i++)
	{
		arNotSFSRGroupK.SetAt(m_aNotSFSRGroupK[i], 0);
	}
	
	// StructGroup - Group Key로 Map 구성.
	// StructGroup Key Array 순서대로 Name Array 구성.
	nKeyCount = m_aAllStructGroupK.GetSize();
	for(i=0; i<nKeyCount; i++)
	{
		T_GRUP_K GroupK = m_aAllStructGroupK[i];
		int nDummy=0;
		if(arExcludeGroupK.Lookup(GroupK, nDummy)) continue;
		if(arNotSFSRGroupK.Lookup(GroupK, nDummy)) continue;
		CString strGroupName = GetGrupName(GroupK);
		arStructGroupK.SetAt(GroupK, 0);
		m_aStructGroupName.Add(strGroupName);
	}
	
	// T_DCON_D의 Exclude GroupK Array 순서대로 GroupName Array 구성.
	nKeyCount = m_aExcludeGroupK.GetSize();
	for(i=0; i<nKeyCount; i++)
	{
		T_GRUP_K GroupK = m_aExcludeGroupK[i];
		CString strGroupName = GetGrupName(GroupK);
		m_aExcludeGroupName.Add(strGroupName);
	}
	
	// T_DCON_D의 NotSFSR GroupK Array 순서대로 GroupName Array 구성.
	nKeyCount = m_aNotSFSRGroupK.GetSize();
	for(i=0; i<nKeyCount; i++)
	{
		T_GRUP_K GroupK = m_aNotSFSRGroupK[i];
		CString strGroupName = GetGrupName(GroupK);
		m_aNotSFSRGroupName.Add(strGroupName);
	}
}

void CDgnConCodeSeisDefDlg::UpdateList()
{
	MakeListData();

	Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
	Write_ListCtrl(m_ExcludeList,    m_aExcludeGroupName);
	Write_ListCtrl(m_NotSFSRList,    m_aNotSFSRGroupName);
}

void CDgnConCodeSeisDefDlg::Write_ListCtrl(CListCtrl& GroupList, CArray<CString, CString&>& m_aGroupName)
{
	GroupList.DeleteAllItems();
	int iGroupCount = m_aGroupName.GetSize();

	CString strGroupName = _T("");
	LV_ITEM lvitem;

	for(int i=0; i<iGroupCount; i++)
	{
		strGroupName = m_aGroupName[i];
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= iGroupCount;
		lvitem.iSubItem = 0;
		lvitem.pszText	= strGroupName.GetBuffer(strGroupName.GetLength());
		GroupList.InsertItem(&lvitem);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnAddExcludeBtn()
{
	UpdateData(TRUE);
	
	int iSelectedCount = m_StrucGroupList.GetSelectedCount();
	int  nItem = -1;

	int nDummy = 0;
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_StrucGroupList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strGroupName = m_StrucGroupList.GetItemText(nItem, 0);
			T_GRUP_K GroupK = GetGrupKey(strGroupName);
			m_aExcludeGroupK.Add(GroupK);
		}
		MakeListData();
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
		Write_ListCtrl(m_ExcludeList,    m_aExcludeGroupName);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnDelExcludeBtn()
{
	UpdateData(TRUE);
	
	int iSelectedCount = m_ExcludeList.GetSelectedCount();
	int  nItem = -1;
	
	int nDummy = 0;
	if(iSelectedCount > 0)
	{
		CArray<int, int> aDeleteIndex; aDeleteIndex.SetSize(iSelectedCount);
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_ExcludeList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strGroupName = m_ExcludeList.GetItemText(nItem, 0);
			T_GRUP_K GroupK = GetGrupKey(strGroupName);
			// Array 작업.
			aDeleteIndex.SetAt(i, nItem);
			ASSERT(m_aAllStructGroupK.GetSize() == m_aAllStructGroupName.GetSize());
			for(int j=0; j<m_aAllStructGroupK.GetSize(); j++)
			{
				if(GroupK==m_aAllStructGroupK[j])
				{
					// list 순서를 위해
					m_aAllStructGroupK.RemoveAt(j);
					m_aAllStructGroupName.RemoveAt(j);
					m_aAllStructGroupK.Add(GroupK);
					m_aAllStructGroupName.Add(strGroupName);
					break;
				}
			}
		}
		for(int i=aDeleteIndex.GetSize()-1; i>=0; i--)
		{
			m_aExcludeGroupK.RemoveAt(aDeleteIndex[i]);
		}
		MakeListData();
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
		Write_ListCtrl(m_ExcludeList,    m_aExcludeGroupName);
	}  
}

void CDgnConCodeSeisDefDlg::OnDgnAddNotSFSRBtn()
{
	UpdateData(TRUE);
	
	int iSelectedCount = m_StrucGroupList.GetSelectedCount();
	int  nItem = -1;
	
	int nDummy = 0;
	if(iSelectedCount > 0)
	{
		CArray<int, int> aDeleteIndex; aDeleteIndex.SetSize(iSelectedCount);
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_StrucGroupList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strGroupName = m_StrucGroupList.GetItemText(nItem, 0);
			T_GRUP_K GroupK = GetGrupKey(strGroupName);
			m_aNotSFSRGroupK.Add(GroupK);
		}
		MakeListData();
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
		Write_ListCtrl(m_NotSFSRList,    m_aNotSFSRGroupName);
	}  
}

void CDgnConCodeSeisDefDlg::OnDgnDelNotSFSRBtn()
{
	UpdateData(TRUE);
	
	int iSelectedCount = m_NotSFSRList.GetSelectedCount();
	int  nItem = -1;
	
	int nDummy = 0;
	if(iSelectedCount > 0)
	{
		CArray<int, int> aDeleteIndex; aDeleteIndex.SetSize(iSelectedCount);
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_NotSFSRList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			CString strGroupName = m_NotSFSRList.GetItemText(nItem, 0);
			T_GRUP_K GroupK = GetGrupKey(strGroupName);
			aDeleteIndex.SetAt(i, nItem);
			for(int j=0; j<m_aAllStructGroupK.GetSize(); j++)
			{
				if(GroupK==m_aAllStructGroupK[j])
				{
					// list 순서를 위해
					m_aAllStructGroupK.RemoveAt(j);
					m_aAllStructGroupName.RemoveAt(j);
					m_aAllStructGroupK.Add(GroupK);
					m_aAllStructGroupName.Add(strGroupName);
					break;
				}
			}
		}
		for(int i=aDeleteIndex.GetSize()-1; i>=0; i--)
		{
			m_aNotSFSRGroupK.RemoveAt(aDeleteIndex[i]);
		}
		MakeListData();
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
		Write_ListCtrl(m_NotSFSRList,    m_aNotSFSRGroupName);
	}    
}

void CDgnConCodeSeisDefDlg::SortGroupKListByName(CArray<CString, CString&>& arGroupName, CArray<UINT, UINT>& arGroupKList)
{
	arGroupKList.RemoveAll();
	T_GRUP_K GrupK = NULL;
	for(int i=0; i<arGroupName.GetSize(); i++)
	{
		GrupK = GetGrupKey(arGroupName[i]);
		arGroupKList.Add(GrupK);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnMainASCButton()
{  
	int nCount = m_aStructGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aStructGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
	}

	nCount = m_aAllStructGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aAllStructGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);
		SortGroupKListByName(m_aAllStructGroupName, m_aAllStructGroupK);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnMainDSCButton()
{  
	int nCount = m_aStructGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aStructGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		Write_ListCtrl(m_StrucGroupList, m_aStructGroupName);
	}

	nCount = m_aAllStructGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aAllStructGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		SortGroupKListByName(m_aAllStructGroupName, m_aAllStructGroupK);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnSub1ASCButton()
{
	int nCount = m_aExcludeGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aExcludeGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);
		Write_ListCtrl(m_ExcludeList, m_aExcludeGroupName);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnSub1DSCButton()
{
	int nCount = m_aExcludeGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aExcludeGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		Write_ListCtrl(m_ExcludeList, m_aExcludeGroupName);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnSub2ASCButton()
{
	int nCount = m_aNotSFSRGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aNotSFSRGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);
		Write_ListCtrl(m_NotSFSRList, m_aNotSFSRGroupName);
	}
}

void CDgnConCodeSeisDefDlg::OnDgnSub2DSCButton()
{
	int nCount = m_aNotSFSRGroupName.GetSize();  
	if (nCount>0)
	{
		qsort(m_aNotSFSRGroupName.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		Write_ListCtrl(m_NotSFSRList, m_aNotSFSRGroupName);
	}
}

void CDgnConCodeSeisDefDlg::SetGroupData(CArray<UINT, UINT>& aAllStructGroupK, CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>& mAllStructGroup)
{
	m_aAllStructGroupK.RemoveAll();
	m_aAllStructGroupK.Copy(aAllStructGroupK);
	m_aAllStructGroupName.RemoveAll();

	int nDataSize = aAllStructGroupK.GetSize();

	m_arAllGroup.RemoveAll();
	m_arAllGroup.InitHashTable(nDataSize+1);

	T_GRUP_K MaxGrupK = 0;
	T_GRUP_D data;
	for(int i=0; i<nDataSize; i++)
	{
		T_GRUP_K key = m_aAllStructGroupK[i];
		if(!mAllStructGroup.Lookup(key, data)) {ASSERT(0); continue;}

		m_arAllGroup.SetAt(key, data);
		m_aAllStructGroupName.Add(data.GroupName);
		if(key>MaxGrupK) MaxGrupK = key;
	}
	
	m_LastGrupK = MaxGrupK;
}

void CDgnConCodeSeisDefDlg::SetGroupData(CArray<UINT, UINT>& aExcludeGroup, CArray<UINT, UINT>& aNotSFSRGroup)
{
	m_aExcludeGroupK.RemoveAll();
	m_aNotSFSRGroupK.RemoveAll();
	m_aExcludeGroupK.Copy(aExcludeGroup);
	m_aNotSFSRGroupK.Copy(aNotSFSRGroup);
}

CString CDgnConCodeSeisDefDlg::GetGrupName(T_GRUP_K Key)
{
	CString strGrupName = _T("");

	ASSERT(m_arAllGroup.GetCount()>0);
	T_GRUP_D GrupD;
	if(m_arAllGroup.Lookup(Key, GrupD)) strGrupName = GrupD.GroupName;
	else {ASSERT(0); strGrupName = _T("");}

	return strGrupName;
}

// 중복되는 이름이 있으면 이름 규칙에 의해 이름을 다시 만들어 준다.
CString CDgnConCodeSeisDefDlg::GetGrupName(CString& strGrupName)
{
	int nDummy = 0;
	CString strNewName = strGrupName;
	CString strTail = _T("");
	int nCount = 1;

	for(int i=0; i<m_aAllStructGroupName.GetSize(); i++)
	{
		if(m_aAllStructGroupName[i] == strNewName)
		{
			strTail.Format(_T("_%d"), nCount);
			strNewName = strGrupName+strTail;
			nCount++;
			i=-1; // 처음부터 다시 검사
		}
	}

	return strNewName;
}

BOOL CDgnConCodeSeisDefDlg::GetGrup(T_GRUP_K GrupK, T_GRUP_D& GrupD)
{
	GrupD.Initialize();
	ASSERT(m_arAllGroup.GetCount()>0);
	if(!m_arAllGroup.Lookup(GrupK, GrupD)) return FALSE;
	return TRUE;
}

T_GRUP_K CDgnConCodeSeisDefDlg::GetGrupKey(CString& strGroupName)
{
	T_GRUP_K key;
	T_GRUP_D data;

	T_GRUP_K GrupK = NULL;
	POSITION Pos = m_arAllGroup.GetStartPosition();
	while(Pos)
	{
		m_arAllGroup.GetNextAssoc(Pos, key, data);
		if(data.GroupName == strGroupName)
		{
			GrupK = key;
		}
	}

	ASSERT(GrupK);
	return GrupK;
}

void CDgnConCodeSeisDefDlg::GetGroupData(CArray<UINT, UINT>& aAllStructGroupK, CArray<UINT, UINT>& aExcludeGroupK, CArray<UINT, UINT>& aNotSFSRGroupK)
{  
	aAllStructGroupK.RemoveAll();
	aExcludeGroupK.RemoveAll();
	aNotSFSRGroupK.RemoveAll();
	aAllStructGroupK.Copy(m_aAllStructGroupK);
	aExcludeGroupK.Copy(m_aExcludeGroupK);
	aNotSFSRGroupK.Copy(m_aNotSFSRGroupK);
}

void CDgnConCodeSeisDefDlg::GetGroupData(CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>& mAllStructGroup)
{
	T_GRUP_K key;
	T_GRUP_D data;
	POSITION Pos = m_arAllGroup.GetStartPosition();
	mAllStructGroup.RemoveAll();
	while(Pos)
	{
		m_arAllGroup.GetNextAssoc(Pos, key, data);
		mAllStructGroup.SetAt(key, data);
	}
}

int CDgnConCodeSeisDefDlg::Generate_StructGroupToExclude(CArray<T_GRUP_D, T_GRUP_D&>& aGrupData)
{  
	aGrupData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> aDgnElemList;
	CDgnForceCtrl DgnForcCtrl;
	DgnForcCtrl.m_pDgnDataCtrl->m_pMembCtrl->GetElemListForDgn(FALSE, aDgnElemList, FALSE, 0, TRUE);

	CArray<T_ELEM_K, T_ELEM_K> aCantilever;
	CArray<T_ELEM_K, T_ELEM_K> aSubBeam;
	CArray<T_ELEM_K, T_ELEM_K> aUnderBeam;
	CArray<T_ELEM_K, T_ELEM_K> aUnderColm;

	// Node는 중복될 수 있으므로 Map 사용.
	CMapEx<T_NODE_K, T_NODE_K, int, int> mapNodeCantilever;  mapNodeCantilever.InitHashTable(HASHSIZENODE); mapNodeCantilever.RemoveAll();
	CMapEx<T_NODE_K, T_NODE_K, int, int> mapNodeSubBeam;     mapNodeSubBeam.InitHashTable(HASHSIZENODE);    mapNodeSubBeam.RemoveAll();
	CMapEx<T_NODE_K, T_NODE_K, int, int> mapNodeUnderBeam;   mapNodeUnderBeam.InitHashTable(HASHSIZENODE);  mapNodeUnderBeam.RemoveAll();
	CMapEx<T_NODE_K, T_NODE_K, int, int> mapNodeUnderColm;   mapNodeUnderColm.InitHashTable(HASHSIZENODE);  mapNodeUnderColm.RemoveAll();
	
	T_MBTP_D MbtpD;
	T_ELEM_D ElemD;
	int iNumElem = aDgnElemList.GetSize();
	for(int i=0; i<iNumElem; i++)
	{
		T_ELEM_K ElemK = aDgnElemList[i];

		if(!DgnForcCtrl.m_pDgnDataCtrl->Get_DgnGenMbtp(ElemK, MbtpD)) continue;
		
		if(MbtpD.nMbType==D_MBTP_BEAM)
		{
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
			T_NODE_K sNodeK = ElemD.elnod[0];
			T_NODE_K eNodeK = ElemD.elnod[1];

			int iCantilever = DgnForcCtrl.IsCantilever(ElemK);
			if(iCantilever>0) 
			{
				aCantilever.Add(ElemK);
				mapNodeCantilever.SetAt(sNodeK, sNodeK);
				mapNodeCantilever.SetAt(eNodeK, eNodeK);
			}
						
			BOOL bSubBeam = DgnForcCtrl.IsSubBeamForNoMpr(ElemK, iCantilever);
			if(bSubBeam && iCantilever<1)  // Cantilever가 아닌 Sub-Beam만 고려. 
			{
				aSubBeam.Add(ElemK);
				mapNodeSubBeam.SetAt(sNodeK, sNodeK);
				mapNodeSubBeam.SetAt(eNodeK, eNodeK);
			}

			DgnForcCtrl.m_iElemKind = MbtpD.nMbType;
			BOOL bUnder = m_strCode==_T("Eurocode2:04") ? FALSE : DgnForcCtrl.IsUnderGroundMemb(ElemK); // EC2:04일 경우는 지하층 그룹 만들지 않음.
			if(bUnder)
			{
				aUnderBeam.Add(ElemK);
				mapNodeUnderBeam.SetAt(sNodeK, sNodeK);
				mapNodeUnderBeam.SetAt(eNodeK, eNodeK);
			}
		}
		else if(MbtpD.nMbType==D_MBTP_COLUMN)
		{
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
			T_NODE_K sNodeK = ElemD.elnod[0];
			T_NODE_K eNodeK = ElemD.elnod[1];

			DgnForcCtrl.m_iElemKind = MbtpD.nMbType;
			BOOL bUnder = m_strCode==_T("Eurocode2:04") ? FALSE : DgnForcCtrl.IsUnderGroundMemb(ElemK); // EC2:04일 경우는 지하층 그룹 만들지 않음.
			if(bUnder)
			{
				aUnderColm.Add(ElemK);
				mapNodeUnderColm.SetAt(sNodeK, sNodeK);
				mapNodeUnderColm.SetAt(eNodeK, eNodeK);
			}      
		}
		else continue;
	}

	T_GRUP_D GrupCantileverD;
	if(Generate_GrupData(_T("CantileverBeam"), aCantilever, mapNodeCantilever, GrupCantileverD))
	{
		aGrupData.Add(GrupCantileverD);
	}

	T_GRUP_D GrupSubBeamD;
	if(Generate_GrupData(_T("SubBeam"), aSubBeam, mapNodeSubBeam, GrupSubBeamD))
	{
		aGrupData.Add(GrupSubBeamD);
	}

	T_GRUP_D GrupUnderBeamD;
	if(Generate_GrupData(_T("UnderGroundBeam"), aUnderBeam, mapNodeUnderBeam, GrupUnderBeamD))
	{
		aGrupData.Add(GrupUnderBeamD);
	}
	
	T_GRUP_D GrupUnderColmD;
	if(Generate_GrupData(_T("UnderGroundColm"), aUnderColm, mapNodeUnderColm, GrupUnderColmD))
	{
		aGrupData.Add(GrupUnderColmD);
	}    

	return aGrupData.GetSize();
}

BOOL CDgnConCodeSeisDefDlg::Generate_GrupData(const CString& strName, const CArray<T_ELEM_K, T_ELEM_K>& aElemData, const CMapEx<T_NODE_K, T_NODE_K, int, int>& mapNodeData, T_GRUP_D& GrupD)
{
	if(aElemData.GetSize()*mapNodeData.GetCount()<1) return FALSE;

	GrupD.Initialize();

	GrupD.GroupName = strName;
	GrupD.arKeyElem.Copy(aElemData);
	mapNodeData.GetKeyList(GrupD.arKeyNode);

	return TRUE;
}

void CDgnConCodeSeisDefDlg::OnDgnAutoGroup()
{
	CArray<T_GRUP_D, T_GRUP_D&> aGrupData;
	Generate_StructGroupToExclude(aGrupData);

	CDBDoc* pDBDoc = CDBDoc::GetDocPoint(); ASSERT(pDBDoc);
	
	int nCount = aGrupData.GetSize();
	for(int i=0; i<nCount; i++)
	{
		T_GRUP_D GroupD = aGrupData[i];
		CString strNewName = GetGrupName(GroupD.GroupName);
		m_aAllStructGroupName.Add(strNewName);
		//m_aStructGroupName.Add(strNewName);
		/////////////////////////////////////////
		m_LastGrupK++;
		GroupD.GroupName = strNewName;
		m_aAllStructGroupK.Add(m_LastGrupK);
		m_arAllGroup.SetAt(m_LastGrupK, GroupD);
		if(pDBDoc->m_pAttrCtrl->ExistGrup(m_LastGrupK)) ASSERT(0);
		/////////////////////////////////////////
	}

	UpdateList();
}