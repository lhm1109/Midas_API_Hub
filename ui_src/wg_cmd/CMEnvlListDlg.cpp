// CMEnvlListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEnvlListDlg.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMEnvlItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlListDlg dialog


CCMEnvlListDlg::CCMEnvlListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMEnvlListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMEnvlListDlg)
	//}}AFX_DATA_INIT
}


void CCMEnvlListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMEnvlListDlg)
	DDX_Control(pDX, IDC_CMD_LIST2, m_List);
	DDX_Control(pDX, IDC_CMD_LIST, m_List1);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMEnvlListDlg implementation functions
void CCMEnvlListDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_ENVL_loadcase), _LS(IDS_CMD_ENVL_type) };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[] = {100, 73};

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
}

void CCMEnvlListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List1.ResetContent();

	CArray<T_ENVL_K, T_ENVL_K> aKey;
	m_pDoc->m_pAttrCtrl->GetEnvlKeyList(aKey);
	int nItemCount = aKey.GetSize();
	
	if(nItemCount == 0) return;

	int nCount, nIndex;

	if(nItemCount)
	{
		T_ENVL_K Key;
		T_ENVL_D Data;

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=aKey[nCount];
			m_pDoc->m_pAttrCtrl->GetEnvl(Key,Data);
			nIndex = m_List1.InsertString(nCount, Data.EnvelopeName);
			m_List1.SetItemData(nIndex, Key);
		}
	}
}

void CCMEnvlListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ENVL_K Key, KeyBak;
	T_ENVL_D Data, DataBak;

	T_ENVL_K curKey = 0;
	int nSel = m_List1.GetCurSel();
	if (nSel != LB_ERR) curKey = m_List1.GetItemData(nSel);
	BOOL bCurDeleted = FALSE;
	BOOL bCurModified = FALSE;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_ENVL_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetEnvl(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_ENVL_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetEnvl(nKey, Key, Data);
				DeleteItem(Key);
				if (Key == curKey) bCurDeleted = TRUE;
			}
			break;
		case(UR_ENVL_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetEnvl(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_ENVL_MFS): 
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetEnvl(nKey, Key, Data);
				DeleteItem(Key);
				InsertItem(KeyBak, DataBak);
				if (Key == curKey) bCurModified = TRUE;
				if (Key != KeyBak) curKey = KeyBak; // 키도 변경된 경우 curKey 바꾼다.
				// 플래그 수정
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while

	if (bCurDeleted)
	{
		m_List.DeleteAllItems();
	}
	else if (bCurModified)
	{
		T_ENVL_D envl;
		m_pDoc->m_pAttrCtrl->GetEnvl(curKey, envl);
		m_List1.SelectString(-1, envl.EnvelopeName);
		OnSelchangeCmdList();
	}
}

int CCMEnvlListDlg::FindInsertionPos(int nID)
{
	T_ENVL_K key;
	T_ENVL_D data;
	int nCount = m_List1.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List1.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetEnvl(key, data);
		if (nID < data.EnvelopeId) break;
	}
	return i;    
}

BOOL CCMEnvlListDlg::InsertItem(T_ENVL_K Key, T_ENVL_D &Data)
{
	int nIndex;
	int nItem = FindInsertionPos(Data.EnvelopeId);
	nIndex = m_List1.InsertString(nItem, Data.EnvelopeName);
	m_List1.SetItemData(nIndex, Key);

	return TRUE;
}

BOOL CCMEnvlListDlg::DeleteItem(T_ENVL_K Key)
{
	int nCount = m_List1.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (m_List1.GetItemData(i) == Key) break;

	if (i == nCount) return FALSE;
	m_List1.DeleteString(i);
	return TRUE;
}

BOOL CCMEnvlListDlg::ModifyItem(T_ENVL_K KeyOld, T_ENVL_K Key, T_ENVL_D &Data)
{
	if (!DeleteItem(KeyOld)) return FALSE;
	if (!InsertItem(Key, Data)) return FALSE;
	return TRUE;
}

void CCMEnvlListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
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

void CCMEnvlListDlg::ShowEnvelope(int nSel)
{
	m_List.DeleteAllItems();
	if (nSel == LB_ERR) return;
	T_ENVL_K Key = (T_ENVL_K)m_List1.GetItemData(nSel);
	T_ENVL_D Data;
	m_pDoc->m_pAttrCtrl->GetEnvl(Key, Data);
	
	int nSize = Data.arEnvelope.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		m_List.InsertItem(i, _T(""));
		switch(Data.arEnvelope[i].AnalType)
		{
		case D_LCOM_STATIC:
			{
				T_STLD_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_STLD_D stld;
				m_pDoc->m_pAttrCtrl->GetStld(Key, stld);
				m_List.SetItemText(i, 0, stld.LoadCaseName);
				m_List.SetItemText(i, 1, _T("ST"));
			}
			break;
		case D_LCOM_SPECTRUM:
			{
				T_SPLC_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_SPLC_D splc;
				m_pDoc->m_pAttrCtrl->GetSplc(Key, splc);
				m_List.SetItemText(i, 0, splc.LoadCaseName);
				m_List.SetItemText(i, 1, _T("RS"));
			}
			break;
		case D_LCOM_HISTORY:
			{
				T_THIS_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_THIS_D thisData;
				m_pDoc->m_pAttrCtrl->GetThis(Key, thisData);
				m_List.SetItemText(i, 0, thisData.LoadCaseName);
				m_List.SetItemText(i, 1, _T("TH"));
			}
			break;
		case D_LCOM_MOVING:
			{
				T_MVLD_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_MVLD_D mvld;
				m_pDoc->m_pAttrCtrl->GetMvld(Key, mvld);
				m_List.SetItemText(i, 0, mvld.LoadCaseName);
				m_List.SetItemText(i, 1, _T("MV"));
			}
			break;
		case D_LCOM_SETTLEMENT:
			{
				T_SMLC_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_SMLC_D smlc;
				m_pDoc->m_pAttrCtrl->GetSmlc(Key, smlc);
				m_List.SetItemText(i, 0, smlc.LoadCaseName);
				m_List.SetItemText(i, 1, _T("SM"));
			}
			break;
		case D_LCOM_STAGE:
			{
				T_SGLD_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_SGLD_D sgld;
				m_pDoc->m_pAttrCtrl->GetSgld(Key, sgld);
				m_List.SetItemText(i, 0, sgld.LoadCaseName);
				m_List.SetItemText(i, 1, _T("CS"));
			}
			break;
		case D_LCOM_CB_GENERAL:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CB"));
			}
			break;
		case D_LCOM_CB_STEEL:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CBS"));
			}
			break;
		case D_LCOM_CB_CONCRETE:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CBC"));
			}
			break;
		case D_LCOM_CB_SRC:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SRC, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CBR"));
			}
			break;
		case D_LCOM_CB_FOOTING:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_FDN, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CBF"));
			}
			break;
		case D_LCOM_CB_ALUMINUM:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_ALUMINUM, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _T("CBA"));
			}
			break;
		case D_LCOM_CB_STLCOMP:
			{
				T_LCOM_K Key = Data.arEnvelope[i].LoadCaseKey;
				T_LCOM_D lcom;
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, Key, lcom);
				m_List.SetItemText(i, 0, lcom.LoadCombName);
				m_List.SetItemText(i, 1, _LS(IDS_WG_CMD__ADDD__CBSC));
			}
			break;
		}
	}
}

BEGIN_MESSAGE_MAP(CCMEnvlListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMEnvlListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_LBN_SELCHANGE(IDC_CMD_LIST, OnSelchangeCmdList)
	ON_LBN_DBLCLK(IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlListDlg message handlers

BOOL CCMEnvlListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMEnvlListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMEnvlItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();	
}

void CCMEnvlListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_List1.GetCurSel();
	if (nSel == LB_ERR)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_envelope_));
		return;
	}
	T_ENVL_K Key;
	T_ENVL_D Data;
	Key = m_List1.GetItemData(nSel);
	m_pDoc->m_pAttrCtrl->GetEnvl(Key, Data);

	CCMEnvlItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CCMEnvlListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_List1.GetCurSel();
	if (nSel == LB_ERR)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_envelope_));
		return;
	}
	CString csName;
	m_List1.GetText(nSel, csName);
	if (m_pDoc->m_pDataCtrl->DelEnvl(csName))
	{
		if (nSel >= m_List1.GetCount()) nSel -= 1;
		if (nSel >= 0) m_List1.SetCurSel(nSel);
	}
	m_List.DeleteAllItems();
}

void CCMEnvlListDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMEnvlListDlg::OnSelchangeCmdList() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_List1.GetCurSel();
	ShowEnvelope(nSel);
}

void CCMEnvlListDlg::OnDblclkCmdList() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_List1.GetCurSel();
	if (nSel == LB_ERR) return;
		
	T_ENVL_K Key;
	T_ENVL_D Data;
	Key = m_List1.GetItemData(nSel);
	m_pDoc->m_pAttrCtrl->GetEnvl(Key, Data);

	CCMEnvlItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}
