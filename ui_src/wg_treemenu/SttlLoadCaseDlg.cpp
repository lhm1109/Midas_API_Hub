// SttlLoadCaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SttlLoadCaseDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSttlLoadCaseDlg dialog


CSttlLoadCaseDlg::CSttlLoadCaseDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSttlLoadCaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSttlLoadCaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CSttlLoadCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSttlLoadCaseDlg)
	DDX_Control(pDX, IDC_TM_STTL_LCASE_LIST, m_List);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_DESC_EDIT, m_edtDesc);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_SFACTOR_EDIT, m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_SMIN_EDIT, m_edtSMin);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_SMAX_EDIT, m_edtSMax);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_SEL_LIST, m_lstSelGroup);
	DDX_Control(pDX, IDC_TM_STTL_LCASE_SET_LIST, m_lstSetGroup);
	DDX_Control(pDX, IDC_TM_STTL_GROUP_NAME_EDIT, m_edtLoadCaseName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSttlLoadCaseDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSttlLoadCaseDlg)
	ON_BN_CLICKED(IDC_TM_STTL_LCASE_ADD, OnTmSttlLcaseAdd)
	ON_BN_CLICKED(IDC_TM_STTL_LCASE_MODIFY, OnTmSttlLcaseModify)
	ON_BN_CLICKED(IDC_TM_STTL_LCASE_DELETE, OnTmSttlLcaseDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_STTL_LCASE_LIST, OnItemchangedTmSttlLcaseList)
	ON_BN_CLICKED(IDC_TM_STTL_GROUP_CLOSE, OnTmSttlGroupClose)
	ON_BN_CLICKED(IDC_TM_STTL_LCASE_SEL_BTN, OnTmSttlLcaseSelBtn)
	ON_BN_CLICKED(IDC_TM_STTL_LCASE_REM_BTN, OnTmSttlLcaseRemBtn)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_STTL_LCASE_SMIN_SPIN, OnDeltaposTmSttlLcaseSminSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_STTL_LCASE_SMAX_SPIN, OnDeltaposTmSttlLcaseSmaxSpin)
	ON_LBN_DBLCLK(IDC_TM_STTL_LCASE_SEL_LIST, OnDblclkTmSttlLcaseSelList)
	ON_LBN_DBLCLK(IDC_TM_STTL_LCASE_SET_LIST, OnDblclkTmSttlLcaseSetList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSttlLoadCaseDlg message handlers

BOOL CSttlLoadCaseDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_edtSMin.SetRange(1, 1000);
	m_edtSMin.SetValue(1);
	m_edtSMin.SetInteger(TRUE);
	m_edtSMax.SetRange(1, 1000);
	m_edtSMax.SetValue(1);
	m_edtSMax.SetInteger(TRUE);
	m_edtScaleFactor.SetWindowText(_T("1"));

	InitListTitle();
	ResetSmptData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define COLCOUNT 2
void CSttlLoadCaseDlg::InitListTitle()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Load_Case),_LS(IDS_WG_TREEMENU_Group_List)};
	int nColWidth[COLCOUNT]={100,180};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	if(m_pDoc->m_pAttrCtrl->GetCountSmlc())
	{
		InitSmlcData();
		m_List.SetItemState(0,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CSttlLoadCaseDlg::InitSmlcData()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSmlc();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	CString strGroupList;
	int nCount;

	if(nItemCount)
	{
		T_SMLC_K Key;
		T_SMLC_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSmlc();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSmlc(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_SMLC_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSmlc(Key,Data);

			int nIndex = m_List.InsertItem(nCount, Data.LoadCaseName);
		
		  for(int j=0; j<Data.aPointData.GetSize(); j++)
			{
			  m_pDoc->m_pAttrCtrl->GetSmpt(Data.aPointData.GetAt(j), m_SmptData);		
			  (j == (Data.aPointData.GetSize()-1)) ? strGroupList += m_SmptData.GroupName : strGroupList += m_SmptData.GroupName+_T(",");
			}
		  m_List.SetItemText(nIndex,1, strGroupList);
			strGroupList.Empty();
		}
		delete []KeyBuf;
	}
}

void CSttlLoadCaseDlg::OnTmSttlLcaseAdd() 
{
	CString strLoadCaseName;
	CString strGroupList;
	CString strDesc;
	CString strScaleFactor;
	CString strTemp;

	m_edtLoadCaseName.GetWindowText(strLoadCaseName);
	m_edtScaleFactor.GetWindowText(strScaleFactor);
	m_edtDesc.GetWindowText(strDesc);
	
	m_SmlcData.Initialize();

	m_SmlcData.LoadCaseName = strLoadCaseName;
	m_SmlcData.Description = strDesc;
	if(!GetFloatNumber(strScaleFactor, m_SmlcData.dFactor)) return;
	if(!m_edtSMax.GetEditValue(m_SmlcData.nStMax)) return;
	if(!m_edtSMin.GetEditValue(m_SmlcData.nStMin)) return;
	
	for(int i=0; i<m_lstSelGroup.GetCount(); i++)
	{
		 m_lstSelGroup.GetText(i, strTemp);
		 m_SmptKey = m_pDoc->m_pAttrCtrl->GetSmptKey(strTemp);
		 m_SmlcData.aPointData.Add(m_SmptKey);
	}       
	
	m_pDoc->m_pDataCtrl->AddSmlc(m_SmlcData);
	m_List.SetItemState(m_List.GetItemCount()-1,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
}

void CSttlLoadCaseDlg::OnTmSttlLcaseModify() 
{
	if(GetCurrentRow() < 0)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___LoadCase_Name_was_not_selec));
		return;
	}
	
	CString strLoadCaseName;
	CString strOldLoadCaseName;
	CString strGroupList;
	CString strDesc;
	CString strScaleFactor;
	CString strTemp;

	T_SMLC_D SmlcOldData;
	int nSelect = GetCurrentRow();

	m_edtLoadCaseName.GetWindowText(strLoadCaseName);
	m_edtScaleFactor.GetWindowText(strScaleFactor);
	m_edtDesc.GetWindowText(strDesc);
	
	m_SmlcData.Initialize();

	m_SmlcData.LoadCaseName = strLoadCaseName;
	m_SmlcData.Description = strDesc;
	if (!GetFloatNumber(strScaleFactor, m_SmlcData.dFactor)) return;
	if (!m_edtSMax.GetEditValue(m_SmlcData.nStMax)) return;
	if (!m_edtSMin.GetEditValue(m_SmlcData.nStMin)) return;
	
	for(int i=0; i<m_lstSelGroup.GetCount(); i++)
	{
		 m_lstSelGroup.GetText(i, strTemp);
		 m_SmptKey = m_pDoc->m_pAttrCtrl->GetSmptKey(strTemp);
		 m_SmlcData.aPointData.Add(m_SmptKey);
	}       

	m_pDoc->m_pAttrCtrl->GetSmlc(GetCurrentSmlcKey(), SmlcOldData);
	m_SmlcData.LoadCaseId = SmlcOldData.LoadCaseId;
	strOldLoadCaseName = GetCurrentSmlcKeyName();

	if(m_pDoc->m_pDataCtrl->ModifySmlc(strOldLoadCaseName, m_SmlcData))
	  m_List.SetItemState(nSelect,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
}

void CSttlLoadCaseDlg::OnTmSttlLcaseDelete() 
{
	if(GetCurrentRow() < 0)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___LoadCase_Name_was_not_selec));
		return;
	}

	int nSelect = GetCurrentRow();

	m_pDoc->m_pDataCtrl->DelSmlc(GetCurrentSmlcKeyName());

	if(!m_List.SetItemState(nSelect, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
	  if(!m_List.SetItemState(nSelect-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
		{
			m_edtLoadCaseName.SetWindowText(_T(""));
			m_lstSelGroup.ResetContent();
			ResetSmptData();
			m_lstSetGroup.RedrawWindow();
			m_edtSMin.SetValue(1);
			m_edtSMax.SetValue(1);
			m_edtScaleFactor.SetWindowText(_T("1"));
			m_edtDesc.SetWindowText(_T(""));
			m_List.SetItemState(-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_edtLoadCaseName.SetFocus();
		}
}

void CSttlLoadCaseDlg::OnItemchangedTmSttlLcaseList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CString strTemp;

	int nSelect = GetCurrentRow();
	
	m_SmlcKey = m_pDoc->m_pAttrCtrl->GetSmlcKey(m_List.GetItemText(nSelect, 0));
	if(m_pDoc->m_pAttrCtrl->GetSmlc(m_SmlcKey, m_SmlcData))
	{
		m_edtLoadCaseName.SetWindowText(m_SmlcData.LoadCaseName);
		m_edtSMin.SetValue(m_SmlcData.nStMin);
		m_edtSMax.SetValue(m_SmlcData.nStMax);
		strTemp.Format(_LS(IDS_WG_TREEMENU__g), m_SmlcData.dFactor);
		m_edtScaleFactor.SetWindowText(strTemp);
		m_edtDesc.SetWindowText(m_SmlcData.Description);

		m_lstSelGroup.ResetContent();
		for(int i=0; i<m_SmlcData.aPointData.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetSmpt(m_SmlcData.aPointData.GetAt(i), m_SmptData);
			m_lstSelGroup.AddString(m_SmptData.GroupName);
		}

		ResetSmptData();
	}

	*pResult = 0;
}

void CSttlLoadCaseDlg::OnTmSttlGroupClose() 
{
	CloseDlg();
}

void CSttlLoadCaseDlg::InitSmptData()
{
	m_lstSetGroup.ResetContent();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSmpt();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	CString strDist;
	int nCount;

	if(nItemCount)
	{
		T_SMPT_K Key;
		T_SMPT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSmpt();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSmpt(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		qsort(KeyBuf, nItemCount, sizeof(T_SMPT_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSmpt(Key,Data);
			m_lstSetGroup.AddString(Data.GroupName);
		}
		delete []KeyBuf;
	}
}

void CSttlLoadCaseDlg::ResetSmptData()
{
	InitSmptData();

	if(m_lstSelGroup.GetCount())
	{
		CString strSetTemp; 
		CString strSelTemp;

		for(int i=0; i<m_lstSelGroup.GetCount(); i++)
			for(int j=0; j<m_lstSetGroup.GetCount(); j++)
			{
				m_lstSetGroup.GetText(j, strSetTemp);
				m_lstSelGroup.GetText(i, strSelTemp);
				if(strSetTemp == strSelTemp)
				{
					m_lstSetGroup.DeleteString(j);
					break;
				}
			}
	}
}

void CSttlLoadCaseDlg::ResetSmptSelData()
{
	m_lstSelGroup.ResetContent();
	m_List.SetItemState(-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	if(m_List.GetItemCount())
		m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	CtrlMinMaxValue();
}

void CSttlLoadCaseDlg::OnTmSttlLcaseSelBtn() 
{
	CString strSelString;
	CString strTemp;
	BOOL flag = FALSE;
	if(m_lstSetGroup.GetSelCount() == 0)
		return;

	if((m_lstSetGroup.GetSelCount() + m_lstSelGroup.GetCount()) > 50)
		return;

	int* lIndex = new int[m_lstSetGroup.GetSelCount()];	
	m_lstSetGroup.GetSelItems(m_lstSetGroup.GetSelCount(), lIndex);

	for(int i=0; i<m_lstSetGroup.GetSelCount(); i++)
	{
		m_lstSetGroup.GetText(lIndex[i], strSelString);

		for(int j=0; j<m_lstSelGroup.GetCount(); j++)
		{
			m_lstSelGroup.GetText(j, strTemp);
			if(strTemp == strSelString) flag=TRUE;
		}
		if(flag)
		{
			flag = FALSE;
			continue;
		}

		m_lstSelGroup.AddString(strSelString);
	}

	for(int i=m_lstSetGroup.GetSelCount()-1; i>=0; i--)
		m_lstSetGroup.DeleteString(lIndex[i]);

	m_lstSelGroup.SetCurSel(m_lstSelGroup.GetCount()-1);

	delete[] lIndex;
}

void CSttlLoadCaseDlg::OnTmSttlLcaseRemBtn() 
{
	CString strTemp;
	if(m_lstSelGroup.GetSelCount() == 0)
		return;

	if(m_lstSelGroup.GetSelCount() == 1)
	{
		int nSelect = m_lstSelGroup.GetCurSel();
		m_lstSelGroup.DeleteString(m_lstSelGroup.GetCurSel());	

		if(m_lstSelGroup.GetCount() > nSelect)
		{
			m_lstSelGroup.SetCurSel(nSelect);
		}
		else if(m_lstSelGroup.GetCount() == nSelect)
		{
			m_lstSelGroup.SetCurSel(nSelect-1);
		}
		else
			m_lstSelGroup.SetCurSel(-1);
	}
	else
	{
		int* lIndex = new int[m_lstSelGroup.GetSelCount()];	
		m_lstSelGroup.GetSelItems(m_lstSelGroup.GetSelCount(), lIndex);

		for(int i=m_lstSelGroup.GetSelCount()-1; i >= 0; i--)
		{
			m_lstSelGroup.DeleteString(lIndex[i]);
		}

		delete[] lIndex;

		if(m_lstSelGroup.GetCount() > 0)
			m_lstSelGroup.SetCurSel(m_lstSelGroup.GetCount()-1);
	}

	CtrlMinMaxValue();
	ResetSmptData();
}

void CSttlLoadCaseDlg::OnDeltaposTmSttlLcaseSminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString strTemp;	
	CString strMaxTemp;

	m_edtSMin.GetWindowText(strTemp);

	if(pNMUpDown->iDelta < 0)
	{
	strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp)+1);	
	}
	else if(_ttoi(strTemp) > 1)
	{
	strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp)-1);	
	}
 
	m_edtSMin.SetWindowText(strTemp);		
	*pResult = 0;
}

void CSttlLoadCaseDlg::OnDeltaposTmSttlLcaseSmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString strTemp;

	m_edtSMax.GetWindowText(strTemp);
	if(pNMUpDown->iDelta < 0)
	{
	  strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp)+1);	
	}
	else if(_ttoi(strTemp) > 1)
	{
	  strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp)-1);	
	}
 
	m_edtSMax.SetWindowText(strTemp);			
	*pResult = 0;
}

void CSttlLoadCaseDlg::CtrlMinMaxValue()
{
	CString strMin, strMax;

	m_edtSMax.GetWindowText(strMax);
	m_edtSMax.GetWindowText(strMin);

	if(m_lstSelGroup.GetCount())
	{
		if(m_lstSelGroup.GetCount() < _ttoi(strMax))
		{
			strMax.Format(_LS(IDS_WG_TREEMENU__d), m_lstSelGroup.GetCount());
			m_edtSMax.SetWindowText(strMax);
		}

		if(m_lstSelGroup.GetCount() < _ttoi(strMin))
		{
			strMin.Format(_LS(IDS_WG_TREEMENU__d), m_lstSelGroup.GetCount());
			m_edtSMin.SetWindowText(strMin);
		}
	}
	else
	{
		strMax.Format(_LS(IDS_WG_TREEMENU__d), 1);
		strMin = strMax;
		m_edtSMax.SetWindowText(strMax);		
		m_edtSMin.SetWindowText(strMin);		
	}
}

void CSttlLoadCaseDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		InitSmlcData();
 		ResetSmptSelData();
		ResetSmptData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		InitSmlcData();
		ResetSmptSelData();
		ResetSmptData();
		break;
	case D_UPDATE_UNIT:
//    UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

int CSttlLoadCaseDlg::GetCurrentSmlcKey()
{
	int nSelect;
	LV_ITEM lvitem;
	CString strLCaseName;

	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			break;
		}
	}
	strLCaseName = m_List.GetItemText(nSelect, 0);

	return m_pDoc->m_pAttrCtrl->GetSmlcKey(strLCaseName);		
}

CString CSttlLoadCaseDlg::GetCurrentSmlcKeyName()
{
	int nSelect;
	LV_ITEM lvitem;
	CString strLCaseName;

	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			break;
		}
	}
	strLCaseName = m_List.GetItemText(nSelect, 0);

	return strLCaseName;		
}

int CSttlLoadCaseDlg::GetCurrentRow()
{
	int nSelect=-1;
	LV_ITEM lvitem;
	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			break;
		}
	}
	return nSelect;
}

void CSttlLoadCaseDlg::OnDblclkTmSttlLcaseSelList() 
{
	OnTmSttlLcaseRemBtn(); 	
}

void CSttlLoadCaseDlg::OnDblclkTmSttlLcaseSetList() 
{
	OnTmSttlLcaseSelBtn(); 
}

BOOL CSttlLoadCaseDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_SMLC_D data;

	m_pDoc->m_pAttrCtrl->GetSmlc(key, data);
	
	CString strTemp;
	m_edtLoadCaseName.SetWindowText(data.LoadCaseName);
	strTemp.Format(_LS(IDS_WG_TREEMENU__d), data.nStMin);
	m_edtSMin.SetWindowText(strTemp);
	strTemp.Format(_LS(IDS_WG_TREEMENU__d), data.nStMax);
	m_edtSMax.SetWindowText(strTemp);
	strTemp.Format(_LS(IDS_WG_TREEMENU__g), data.dFactor);
	m_edtScaleFactor.SetWindowText(strTemp);
	m_edtDesc.SetWindowText(data.Description);

	m_lstSelGroup.ResetContent();
	for(int i=0; i<data.aPointData.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSmpt(data.aPointData.GetAt(i), m_SmptData);
		m_lstSelGroup.AddString(m_SmptData.GroupName);
	}

	ResetSmptData();

	return TRUE;
}
