// CMMlspDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMlspDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMlspDlg
#define CFormView CCMChildBarBase

//IMPLEMENT_DYNCREATE(CCMMlspDlg, CFormView)

CCMMlspDlg::CCMMlspDlg()
	: CFormView(CCMMlspDlg::IDD)
{
//  m_pDoc = 0;
	//{{AFX_DATA_INIT(CCMMlspDlg)
	m_nEType = 0;
	m_nPosition = 0;
	//}}AFX_DATA_INIT
	m_aCtrlOp.Add(IDC_CMD_OPTION_ADD);
	m_aCtrlOp.Add(IDC_CMD_OPTION_DELETE);
}

CCMMlspDlg::~CCMMlspDlg()
{
}

void CCMMlspDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMlspDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Radio(pDX, IDC_CMD_ETYPE_BEAM, m_nEType);
	DDX_Radio(pDX, IDC_CMD_SUP_POS_BOTH, m_nPosition);
	//}}AFX_DATA_MAP
}

//--------------------------------------------------------------------------
// Overridables
void CCMMlspDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
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

void CCMMlspDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MLSP_K Key;
	BOOL bMlspChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MLSP_ADD):
			{
				pViewBuff->GetMlsp(nKey, Key);
				if(Key.key.serial==0)
				{
					InsertItem(Key);
					bMlspChanged = TRUE;
				}
			}
			break;
		case(UR_MLSP_DEL):
			{
				pViewBuff->GetMlsp(nKey, Key);
				if(Key.key.serial==0)
				{
					DeleteItem(Key);
					bMlspChanged = TRUE;
				}
			}
			break;
		default:
			break;
		}
	} // end of while
	if (bMlspChanged)
	{
		CString csNo;
		int nCount = m_List.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			csNo.Format(_T("%d"), i+1);
			m_List.SetItemText(i, 0, csNo);
		}
	}
}

void CCMMlspDlg::CtrlMan()
{
	/*
	IDC_CMD_ETYPE_BEAM
	IDC_CMD_ETYPE_PLATE
	IDC_CMD_SUP_POS_BOTH
	IDC_CMD_SUP_POS_ENDI
	IDC_CMD_OPTION_ENDJ
	*/

	CArray<UINT, UINT> TypeCtrls;
	CArray<UINT, UINT> PosCtrls;

	TypeCtrls.Add(IDC_CMD_ETYPE_BEAM );
	TypeCtrls.Add(IDC_CMD_ETYPE_PLATE);

	PosCtrls.Add(IDC_CMD_SUP_POS_BOTH);
	PosCtrls.Add(IDC_CMD_SUP_POS_ENDI);
	PosCtrls.Add(IDC_CMD_OPTION_ENDJ);

	int nOp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	
	if(nOp == 1) //Delete
	{
		CDlgUtil::CtrlEnableDisable(this,TypeCtrls,FALSE);
		CDlgUtil::CtrlEnableDisable(this,PosCtrls ,FALSE);
	}
	else  //Add
	{
		CDlgUtil::CtrlEnableDisable(this,TypeCtrls,TRUE);
		if(m_nEType == 0) // Beam 
			CDlgUtil::CtrlEnableDisable(this,PosCtrls ,TRUE);
		else              // Plate
			CDlgUtil::CtrlEnableDisable(this,PosCtrls ,FALSE);
	}
}

#define COLCOUNT 4
//--------------------------------------------------------------------------
// Implementations
void CCMMlspDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_MLSP_no), _LS(IDS_CMD_MLSP_element_no), _LS(IDS_CMD_MLSP_type),
											 _LS(IDS_CMD_MLSP_position) };
//18japan	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _T("Element No"), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[] = {40, 45, 45, 60};
	int nAlign[] = {LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMlspDlg::SetItem(int nIndex, T_MLSP_K Key)
{
	CString str;
	for (int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, nIndex+1, Key);
		m_List.SetItemText(nIndex, i, str);
	}
	m_List.SetItemData(nIndex, Key.keymap);
}

void CCMMlspDlg::InsertItem(T_MLSP_K Key)
{
	// 삽입 위치를 찾는다.
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) > Key.keymap) break;
	}
	m_List.InsertItem(nCount, _T(""));
	SetItem(nCount, Key);
}

void CCMMlspDlg::DeleteItem(T_MLSP_K Key)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) == Key.keymap) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Key__d__not_found_), Key);
	else m_List.DeleteItem(nCount);
}

void CCMMlspDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMlsp();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_MLSP_K Key;
		T_MLSP_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMlsp();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMlsp(pos, Key, Data);
			if(Key.key.serial != 0) {--nItemCount; continue;}
		  KeyBuf[nCount++] = Key.keymap;
		}
		if(nItemCount == 0) {delete []KeyBuf; return;}
		qsort(KeyBuf, nItemCount, sizeof(T_MLSP_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key.keymap=KeyBuf[nCount];
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key);
		}
		delete []KeyBuf;
	}
}

CString CCMMlspDlg::DataToStr(int i, int nNo, T_MLSP_K key)
{
	ASSERT(key.key.serial==0);

	CString value;
	if (i == 0) 
	{
		value.Format(_T("%d"), nNo);
	}
	else if (i == 1) 
	{
		value.Format(_T("%d"), key.key.entity);
	}
	else if (i == 2)
	{
		T_ELEM_D elem;
		m_pDoc->m_pAttrCtrl->GetElem(key.key.entity, elem);
		if (m_pDoc->m_pAttrCtrl->IsBeam(elem.eltyp)) 
			value = _LS(IDS_CMD_MLSP_beam);
		else 
			value = _LS(IDS_CMD_MLSP_plate);
	}
	else if (i == 3)
	{
		T_MLSP_D MlspD;
		if(m_pDoc->m_pAttrCtrl->GetMlsp(key,MlspD))
		{
			if(0 == MlspD.nPosition)
			{
				T_ELEM_D elem;
				m_pDoc->m_pAttrCtrl->GetElem(key.key.entity, elem);
				if (m_pDoc->m_pAttrCtrl->IsBeam(elem.eltyp))
					value = _LS(IDS_CMD_MLSP_both);
				else
					value = _T("");
			}
			else if( 1 == MlspD.nPosition)
			{
				//value = _T("End-I");
				value = _LS(IDS_CMD_MLSP_End_I);
				
			}
			else if( 2 == MlspD.nPosition)
			{
				//value = _T("End-J");
				value = _LS(IDS_CMD_MLSP_End_J);
			}
			else
			{
				value = _LS(IDS_CMD_MLSP_error);
			}
		}
		else
		{
			value = _LS(IDS_CMD_MLSP_error);
		}
	}
	else 
	{
		value = _LS(IDS_CMD_MLSP_error);
	}
	return value;
}

void CCMMlspDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;

	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CCMMlspDlg::OnDelKeyPressedInList()
{
	CArray<T_MLSP_K, T_MLSP_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	T_MLSP_K key;
	for (int i = 0; i < nSize; i++)
	{
		key.keymap = m_List.GetItemData(aSelItem[i]);
		aKey.Add(key);
	}
	m_pDoc->m_pDataCtrl->DelMlsp(aKey);
}


BEGIN_MESSAGE_MAP(CCMMlspDlg, CFormView)
	//{{AFX_MSG_MAP(CCMMlspDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_OPTION_ADD, OnCmdOptionAddDel)
	ON_BN_CLICKED(IDC_CMD_OPTION_DELETE, OnCmdOptionAddDel)
	ON_BN_CLICKED(IDC_CMD_ETYPE_BEAM, OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_PLATE, OnCmdEtype)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnClicklist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMlspDlg diagnostics

#ifdef _DEBUG
void CCMMlspDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CCMMlspDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMMlspDlg message handlers
/*
void CCMMlspDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(TRUE);
	CtrlMan();
}
*/
BOOL CCMMlspDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CFormView::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(TRUE);
	CtrlMan();
	return TRUE;
}
void CCMMlspDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nOp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CArray<T_ELEM_K, T_ELEM_K> aKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aKey);

	// Remove element which is not beam type
	T_ELEM_D ElemData;
	
	if(nOp == 0)
	{
		for (int i = aKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aKey.GetAt(i), ElemData);
			if (!((m_pDoc->m_pAttrCtrl->IsBeam (ElemData.eltyp)&& m_nEType == 0)||
						(m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)&& m_nEType == 1))) aKey.RemoveAt(i);
		}
	}

	if (aKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_beam_and_plat));
		return;
	}

	BOOL bSuccess = TRUE;
	T_MLSP_D MlspD;
	MlspD.Initialize();
	MlspD.nPosition = m_nPosition;
	if (nOp == 0)       // Add
	{
		T_MLSP_K MlspK;
		MlspK.key.serial = 0;
		for(int i=0 ; i<aKey.GetSize() ; ++i)
		{
			MlspK.key.entity = aKey[i];
			if(!m_pDoc->m_pDataCtrl->AddMlsp(MlspK,MlspD)) bSuccess=FALSE;
		}
	}
	else if (nOp == 1)  // Delete
	{
		T_MLSP_K MlspK;
		MlspK.key.serial = 0;
		CArray<T_MLSP_K, T_MLSP_K> arrKey;
		for(int i=0 ; i<aKey.GetSize() ; ++i)
		{
			MlspK.key.entity = aKey[i];
			arrKey.Add(MlspK);
			//if(!m_pDoc->m_pDataCtrl->DelMlsp(MlspK)) bSuccess=FALSE;
		}

		if(!m_pDoc->m_pDataCtrl->DelMlsp(arrKey)) bSuccess=FALSE;
	}

	if(bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMMlspDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMMlspDlg::OnClicklist(NMHDR* pNMHDR, LRESULT* pResult)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	CArray<T_ELEM_K, T_ELEM_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		aKey.Add(m_List.GetItemData(aSelItem[i]));
	}
	
	pIGM->UnselectAllElem();
	pIGM->AddSelectedElemKeyList(aKey);
}


BOOL CCMMlspDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			OnDelKeyPressedInList();
			return TRUE;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CCMMlspDlg::OnCmdOptionAddDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CtrlMan();
}

void CCMMlspDlg::OnCmdEtype() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CtrlMan();
}
