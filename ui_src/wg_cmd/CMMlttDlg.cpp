// CMMlttDlg.cpp : implementation file 
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMlttDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMlttDlg
#define CFormView CCMChildBarBase

//IMPLEMENT_DYNCREATE(CCMMlttDlg, CFormView)

CCMMlttDlg::CCMMlttDlg()
	: CFormView(CCMMlttDlg::IDD)
{
//  m_pDoc = 0;
	//{{AFX_DATA_INIT(CCMMlttDlg)
	m_nEType = 0;
	m_nRFType = 1;
	//}}AFX_DATA_INIT
	m_aCtrlOp.Add(IDC_CMD_OPTION_ADD);
	m_aCtrlOp.Add(IDC_CMD_OPTION_DELETE);

	m_nMVCode = 0;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD;
	if (pDoc->m_pAttrCtrl->GetMvcd(MvcdD))
	{
		m_nMVCode = MvcdD.nCodeType;
	}

	if (m_nMVCode != D_MOVE_CODE_PENDOT)
		m_COLCOUNT = 5;
	else 
		m_COLCOUNT = 4;

}

CCMMlttDlg::~CCMMlttDlg()
{
}

void CCMMlttDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMlttDlg)
	DDX_Control(pDX, IDC_CMD_LIST,          m_List);
	DDX_Control(pDX, IDC_CMD_MLTT_NODE_EDT, m_edtLink);
	DDX_Radio(pDX,   IDC_CMD_ETYPE_TRUSS,   m_nEType);
	DDX_Radio(pDX,    IDC_CMD_MLTT_RF_F1_RDO, m_nRFType);
	//}}AFX_DATA_MAP
}

//--------------------------------------------------------------------------
// Overridables
void CCMMlttDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMMlttDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MLTT_K Key;
	BOOL bMlttChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MLTT_ADD):
			{
				pViewBuff->GetMltt(nKey, Key);
				//if(Key.key.serial==0)
				{
					InsertItem(Key);
					bMlttChanged = TRUE;
				}
			}
			break;
		case(UR_MLTT_DEL):
			{
				pViewBuff->GetMltt(nKey, Key);
				//if(Key.key.serial==0)
				{
					DeleteItem(Key);
					bMlttChanged = TRUE;
				}
			}
			break;
		default:
			break;
		}
	} // end of while
	if (bMlttChanged)
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

void CCMMlttDlg::CtrlMan()
{
	CArray<UINT, UINT> TypeCtrls;
	TypeCtrls.Add(IDC_CMD_ETYPE_TRUSS);
	TypeCtrls.Add(IDC_CMD_ETYPE_BEAM );
	TypeCtrls.Add(IDC_CMD_ETYPE_PLATE);
	TypeCtrls.Add(IDC_CMD_ETYPE_ELNK);
	TypeCtrls.Add(IDC_CMD_ETYPE_GLNK);

	CArray<UINT, UINT> aNodeCtrls;
	aNodeCtrls.Add(IDC_CMD_MLTT_NODE_STC);
	aNodeCtrls.Add(IDC_CMD_MLTT_NODE_EDT);

	int nOp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	if(nOp == 1) //Delete
	{
		CDlgUtil::CtrlEnableDisable(this,TypeCtrls,FALSE);
		CDlgUtil::CtrlEnableDisable(this,aNodeCtrls,FALSE);
	}
	else  //Add
	{
		CDlgUtil::CtrlEnableDisable(this,TypeCtrls,TRUE);
		if(m_nEType == D_Mltt_TRUS || m_nEType == D_Mltt_BEAM || m_nEType == D_Mltt_PLAT)
		{
			RemoveNodeSelect();
			//UpdateData(TRUE);
			CDlgUtil::CtrlEnableDisable(this, aNodeCtrls,FALSE);
		}
		else              // Plate
		{
			CDlgUtil::CtrlEnableDisable(this, aNodeCtrls,TRUE);
			m_edtLink.Connect(SC_ID_NODE, &m_edtLink);
		}
	}
}

//--------------------------------------------------------------------------
// Implementations

void CCMMlttDlg::AlignControl()
{

	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls1;
	CArray<UINT, UINT> aControls2;

	if (m_nMVCode != D_MOVE_CODE_PENDOT)
	{
		aControls1.RemoveAll();
		aControls1.Add(IDC_CMD_MLTT_RF_GRP);
		aControls1.Add(IDC_CMD_MLTT_RF_STC);
		aControls1.Add(IDC_CMD_MLTT_RF_F1_RDO);
		aControls1.Add(IDC_CMD_MLTT_RF_F2_RDO);
		CDlgUtil::CtrlShowHide(this, aControls1, FALSE);

		aControls2.RemoveAll();
		aControls2.Add(IDC_CMD_LIST);
		aControls2.Add(IDC_CMD_APPLY);
		aControls2.Add(IDC_CMD_CLOSE);

		GetDlgItem(IDC_CMD_MLTT_RF_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls2, nDistY);
	}
}


void CCMMlttDlg::RemoveNodeSelect()
{
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);

	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);

	m_edtLink.SetWindowText(strBlank);
	m_edtLink.Disconnect();
}


void CCMMlttDlg::SetListCtrlHeader()
{
	CArray<CString, CString> aTitle; aTitle.RemoveAll();
	CArray<int, int> nColWidth;      nColWidth.RemoveAll();
	CArray<int, int> nAlign;         nAlign.RemoveAll();
	if (m_nMVCode != D_MOVE_CODE_PENDOT)
	{
		aTitle.Add(_LS(IDS_CMD_MLTT_no));
		aTitle.Add(_LS(IDS_CMD_MLTT_element_no));
		aTitle.Add(_LS(IDS_CMD_MLTT_type));

		nColWidth.Add(40);
		nColWidth.Add(45);
		nColWidth.Add(105);

		nAlign.Add(LVCFMT_RIGHT);
		nAlign.Add(LVCFMT_RIGHT);
		nAlign.Add(LVCFMT_CENTER);
		nAlign.Add(LVCFMT_CENTER);
	}
	else
	{
		aTitle.Add(_LS(IDS_CMD_MLTT_no));
		aTitle.Add(_LS(IDS_CMD_MLTT_element_no));
		aTitle.Add(_LS(IDS_CMD_MLTT_type));
		aTitle.Add(_T("R.F"));

		nColWidth.Add(40);
		nColWidth.Add(45);
		nColWidth.Add(60);
		nColWidth.Add(45);

		nAlign.Add(LVCFMT_RIGHT);
		nAlign.Add(LVCFMT_RIGHT);
		nAlign.Add(LVCFMT_CENTER);
		nAlign.Add(LVCFMT_CENTER);
		nAlign.Add(LVCFMT_CENTER);
	}

// 	CString aTitle[] = { _LS(IDS_CMD_MLTT_no), _LS(IDS_CMD_MLTT_element_no), _LS(IDS_CMD_MLTT_type) };
// 	int nColWidth[]  = {40, 45, 105};
// 	int nAlign[] = {LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER};
//	int nColNum = sizeof(nColWidth)/sizeof(int);

	int nColNum = nColWidth.GetSize();
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

void CCMMlttDlg::SetItem(int nIndex, T_MLTT_K Key)
{
	CString str;
	for (int i = 0; i < m_COLCOUNT; i++)
	{
		str = DataToStr(i, nIndex+1, Key);
		m_List.SetItemText(nIndex, i, str);
	}
	m_List.SetItemData(nIndex, Key.keymap);
}

void CCMMlttDlg::InsertItem(T_MLTT_K Key)
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

void CCMMlttDlg::DeleteItem(T_MLTT_K Key)
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

void CCMMlttDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMltt();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_MLTT_K Key;
		T_MLTT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMltt();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMltt(pos, Key, Data);
			//if(Key.key.serial != 0) {--nItemCount; continue;}
		  KeyBuf[nCount++] = Key.keymap;
		}
		if(nItemCount == 0) {delete []KeyBuf; return;}
		qsort(KeyBuf, nItemCount, sizeof(T_MLTT_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key.keymap=KeyBuf[nCount];
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key);
		}
		delete []KeyBuf;
	}
}

CString CCMMlttDlg::DataToStr(int i, int nNo, T_MLTT_K key)
{
	//ASSERT(key.key.serial==0);

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
		if (key.key.serial == 0 || key.key.serial == 1 || key.key.serial == 2)
		{
			T_ELEM_D elem;
			m_pDoc->m_pAttrCtrl->GetElem(key.key.entity, elem);

			if (key.key.serial == 0)
			{
				if (m_pDoc->m_pAttrCtrl->IsTruss(elem.eltyp)) value = _LS(IDS_CMD_MLTT_Truss);
				else { ASSERT(0); value = _T("-"); }
			}
			else if (key.key.serial == 1)
			{
				if (m_pDoc->m_pAttrCtrl->IsBeam(elem.eltyp)) value = _LS(IDS_CMD_MLTT_beam);
				else { ASSERT(0); value = _T("-"); }
			}
			else if (key.key.serial == 2)
			{
				if (m_pDoc->m_pAttrCtrl->IsPlate(elem.eltyp)) value = _LS(IDS_CMD_MLTT_plate);
				else { ASSERT(0); value = _T("-"); }
			}
			else
			{
				ASSERT(0);
				value = _T("-");
			}		
		}
		else if (key.key.serial == 3)
		{
			if (m_pDoc->m_pAttrCtrl->ExistElnk(key.key.entity)) value = _LS(IDS_CMD_MLTT_Elnk);
			else { ASSERT(0); value = _T("-"); }
		}
		else if (key.key.serial == 4)
		{
			if (m_pDoc->m_pAttrCtrl->ExistNlnk(key.key.entity)) value = _LS(IDS_CMD_MLTT_Glnk);
			else { ASSERT(0); value = _T("-"); }
		}
		else
		{
			ASSERT(0);
			value = _T("-");
		}
	}
	else if (i == 3)
	{
		T_MLTT_D Data;
		m_pDoc->m_pAttrCtrl->GetMltt(key, Data);
		if(Data.nRF == 0) value = _T("90%");
		else              value = _T("100%");
	}
	else 
	{
		value = _LS(IDS_CMD_MLTT_error);
	}
	return value;
}

void CCMMlttDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
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

void CCMMlttDlg::OnDelKeyPressedInList()
{
	CArray<T_MLTT_K, T_MLTT_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	T_MLTT_K key;
	for (int i = 0; i < nSize; i++)
	{
		key.keymap = m_List.GetItemData(aSelItem[i]);
		aKey.Add(key);
	}
	m_pDoc->m_pDataCtrl->DelMltt(aKey);
}


BEGIN_MESSAGE_MAP(CCMMlttDlg, CFormView)
	//{{AFX_MSG_MAP(CCMMlttDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,         OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE,         OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_OPTION_ADD,    OnCmdOptionAddDel)
	ON_BN_CLICKED(IDC_CMD_OPTION_DELETE, OnCmdOptionAddDel)
	ON_BN_CLICKED(IDC_CMD_ETYPE_TRUSS,   OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_BEAM,    OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_PLATE,   OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_ELNK,    OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_GLNK,    OnCmdEtype)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnClicklist)
	ON_EN_SETFOCUS(IDC_CMD_MLTT_NODE_EDT, OnTmSetfocusLinkEdt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMlttDlg diagnostics

#ifdef _DEBUG
void CCMMlttDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CCMMlttDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMMlttDlg message handlers
/*
void CCMMlttDlg::OnInitialUpdate() 
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
BOOL CCMMlttDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CFormView::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	RemoveNodeSelect();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, 0);

	AlignControl();
	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(TRUE);
	CtrlMan();
	return TRUE;
}
void CCMMlttDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nOp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CString str;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELNK_K, T_ELNK_K> aELinkK;
	CArray<T_NLNK_K, T_NLNK_K> aNLinkK;

	BOOL bSuccess = TRUE;
	T_ELEM_D ElemData;
	T_MLTT_K MlttK;
	CArray<T_MLTT_K, T_MLTT_K> aKey;
	aKey.RemoveAll();

	if (nOp == 0)
	{
		if (m_nEType == D_Mltt_TRUS || m_nEType == D_Mltt_BEAM || m_nEType == D_Mltt_PLAT)
		{
			I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
			pIGM->GetSelectedElemKeyList(aElemKey);
		}
		else if (m_nEType == D_Mltt_ELNK || m_nEType == D_Mltt_NLNK)
		{
			m_edtLink.GetWindowText(str);
			GetNodeList(str, aNodeKey);
			m_pDoc->m_pAttrCtrl2->GetLinkList(aNodeKey, aELinkK, aNLinkK);
		}

		// Remove element which is not beam type
		if (m_nEType == D_Mltt_TRUS || m_nEType == D_Mltt_BEAM || m_nEType == D_Mltt_PLAT)
		{
			if (m_nEType == D_Mltt_TRUS)
			{
				for (int i = aElemKey.GetSize() - 1; i >= 0; i--)
				{
					m_pDoc->m_pAttrCtrl->GetElem(aElemKey.GetAt(i), ElemData);
					if (!m_pDoc->m_pAttrCtrl->IsTruss(ElemData.eltyp)) aElemKey.RemoveAt(i);
				}
			}
			else if (m_nEType == D_Mltt_BEAM)
			{
				for (int i = aElemKey.GetSize() - 1; i >= 0; i--)
				{
					m_pDoc->m_pAttrCtrl->GetElem(aElemKey.GetAt(i), ElemData);
					if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aElemKey.RemoveAt(i);
				}
			}
			else if (m_nEType == D_Mltt_PLAT)
			{
				for (int i = aElemKey.GetSize() - 1; i >= 0; i--)
				{
					m_pDoc->m_pAttrCtrl->GetElem(aElemKey.GetAt(i), ElemData);
					if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)) aElemKey.RemoveAt(i);
				}
			}
			for (int i = 0; i < aElemKey.GetSize(); i++)
			{
				MlttK.key.entity = aElemKey[i];
				MlttK.key.serial = m_nEType;
				aKey.Add(MlttK);
			}
		}
		else if (m_nEType == D_Mltt_ELNK)
		{
			for (int i = aELinkK.GetSize() - 1; i >= 0; i--)
			{
				if(!m_pDoc->m_pAttrCtrl->ExistElnk(aELinkK.GetAt(i))) aELinkK.RemoveAt(i);
			}

			for (int i = 0; i < aELinkK.GetSize(); i++)
			{
				MlttK.key.entity = aELinkK[i];
				MlttK.key.serial = m_nEType;
				aKey.Add(MlttK);
			}
		}
		else if (m_nEType == D_Mltt_NLNK)
		{
			for (int i = aNLinkK.GetSize() - 1; i >= 0; i--)
			{
				if(!m_pDoc->m_pAttrCtrl->ExistNlnk(aNLinkK.GetAt(i))) aNLinkK.RemoveAt(i);
			}

			for (int i = 0; i < aNLinkK.GetSize(); i++)
			{
				MlttK.key.entity = aNLinkK[i];
				MlttK.key.serial = m_nEType;
				aKey.Add(MlttK);
			}
		}
		else
			ASSERT(0);

		if (aKey.GetSize() == 0)
		{
			if (m_nEType == D_Mltt_TRUS)
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_truss));
			else if (m_nEType == D_Mltt_BEAM)
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_beam));
			else if (m_nEType == D_Mltt_PLAT)
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_plate));
			else if (m_nEType == D_Mltt_ELNK)
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Elastic_link_data));
			else if (m_nEType == D_Mltt_NLNK)
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_General_link_data));
			else
				ASSERT(0);
			return;
		}

		T_MLTT_D MlttD;
		MlttD.Initialize();

		for (int i = 0; i < aKey.GetSize(); ++i)
		{
			MlttD.nType = aKey[i].key.serial;
			MlttD.nRF = m_nRFType;
			if (!m_pDoc->m_pDataCtrl->AddMltt(aKey[i], MlttD)) bSuccess = FALSE;
		}

	} 
	else // nOp = 1
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->GetSelectedNodeKeyList(aNodeKey);
		pIGM->GetSelectedElemKeyList(aElemKey);
		m_pDoc->m_pAttrCtrl2->GetLinkList(aNodeKey, aELinkK, aNLinkK);

		for (int i = aElemKey.GetSize() - 1; i >= 0; i--)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(aElemKey.GetAt(i), ElemData)) continue;

			if      (m_pDoc->m_pAttrCtrl->IsTruss(ElemData.eltyp)) MlttK.key.serial = 0;
			else if (m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp))  MlttK.key.serial = 1;
			else if (m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)) MlttK.key.serial = 2;
			else 
				continue;
				 
			MlttK.key.entity = aElemKey[i];
			aKey.Add(MlttK);
		}

		for (int i = 0; i < aELinkK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->ExistElnk(aELinkK.GetAt(i))) continue;

			MlttK.key.entity = aELinkK[i];
			MlttK.key.serial = 3;
			aKey.Add(MlttK);
		}

		for (int i = 0; i < aNLinkK.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->ExistElnk(aNLinkK.GetAt(i))) continue;

			MlttK.key.entity = aNLinkK[i];
			MlttK.key.serial = 4;
			aKey.Add(MlttK);
		}

		for (int i = 0; i < aKey.GetSize(); ++i)
		{
			if (!m_pDoc->m_pDataCtrl->DelMltt(aKey[i])) bSuccess = FALSE;
		}
	}
	
	if(bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMMlttDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMMlttDlg::OnClicklist(NMHDR* pNMHDR, LRESULT* pResult)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	T_MLTT_K MlttKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		MlttKey.keymap = m_List.GetItemData(aSelItem[i]);
		int nType = MlttKey.key.serial;
		if (nType == 0 || nType == 1 || nType == 2)
		{
			aElemKey.Add(MlttKey.key.entity);
		}
		else if (nType == 3)
		{
			T_ELNK_D rData;
			if (m_pDoc->m_pAttrCtrl->GetElnk(MlttKey.key.entity, rData))
			{
				aNodeKey.Add(rData.Node1);
				aNodeKey.Add(rData.Node2);
			}
		}
		else if (nType == 4)
		{
			T_NLNK_D rData;
			if (m_pDoc->m_pAttrCtrl->GetNlnk(MlttKey.key.entity, rData))
			{
				aNodeKey.Add(rData.Node1);
				aNodeKey.Add(rData.Node2);
			}
		}
		else 
			continue;
	}

	pIGM->UnselectAllElem();
	pIGM->UnselectAllNode();
	if (aElemKey.GetSize() > 0)
	{
		pIGM->AddSelectedElemKeyList(aElemKey);
	}

	if (aNodeKey.GetSize() > 0)
	{
		pIGM->AddSelectedNodeKeyList(aNodeKey);
	}

	if (nSize == 1)
	{
		MlttKey.keymap = m_List.GetItemData(aSelItem[0]);
		int nType = MlttKey.key.serial;

		m_nEType = nType;

		UpdateData(FALSE);
		CtrlMan();
	}

}


BOOL CCMMlttDlg::PreTranslateMessage(MSG* pMsg) 
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

void CCMMlttDlg::OnCmdOptionAddDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CtrlMan();
}

void CCMMlttDlg::OnCmdEtype() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CtrlMan();
}

void CCMMlttDlg::OnTmSetfocusLinkEdt()
{
	// text preserve
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);
	m_edtLink.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);
	m_edtLink.SelectByStr(strPreviousText);

}
