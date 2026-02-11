//
// CmdAutoLoadLiveLoadRFDlg.cpp : implementation file
#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadLiveLoadRFDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadLiveLoadRFDlg dialog


CCmdAutoLoadLiveLoadRFDlg::CCmdAutoLoadLiveLoadRFDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadLiveLoadRFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadLiveLoadRFDlg)
	//}}AFX_DATA_INIT
}

void CCmdAutoLoadLiveLoadRFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadLiveLoadRFDlg)
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_CASE_CMB, m_LoadCase);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_RF_LST, m_ListCtrl);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_DEF_RF_EDT, m_edtRLFactor);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CCmdAutoLoadLiveLoadRFDlg, CDialog)
#undef CDialog

	//{{AFX_MSG_MAP(CCmdAutoLoadLiveLoadRFDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LOADCOMB_RF_LST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_RF_LC_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_RF_ADD_BTN, OnCmdRFAddBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_RF_MOD_BTN, OnCmdRFModBtn)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_RF_DEL_BTN, OnCmdRFDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadLiveLoadRFDlg message handlers

void CCmdAutoLoadLiveLoadRFDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_THAT_LC), _LS(IDS_CMD_AUTOLC_factor) };
	int nColWidth[] = { 85, 60 };
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListCtrl.InsertColumn(i, &lvcolumn);
		m_ListCtrl.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCmdAutoLoadLiveLoadRFDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCmdAutoLoadLiveLoadRFDlg::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	int nItemCount = m_Data.aLoadPattern.GetSize();
	if (nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_ListCtrl.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.aLoadPattern.GetAt(nCount));
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

CString CCmdAutoLoadLiveLoadRFDlg::DataToStr(int i, T_LLRF_BASE &data)
{
	CString value;
	if (i == 0)
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D rData;
		m_pDoc->m_pAttrCtrl->GetStld(key, rData);
		value = rData.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dScaleFactor);
	return value;
}

void CCmdAutoLoadLiveLoadRFDlg::SetItem(int nIndex, T_LLRF_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for (int i = 0; i < 2; i++)
	{
		lvitem.iItem = nIndex;
		lvitem.iSubItem = i;
		str = DataToStr(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		m_ListCtrl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CCmdAutoLoadLiveLoadRFDlg::ValidItem(T_LLRF_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dScaleFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdAutoLoadLiveLoadRFDlg::Dlg2Item(T_LLRF_BASE &data)
{
	CString csScaleFactor;
	int Index = m_LoadCase.GetCurSel();
	if (Index != -1)
	{
		CString strName = _T("");
		m_LoadCase.GetLBText(Index, strName);

		CString strLCName = strName;
		int nKey;
		
		nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strLCName);
		T_STLD_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(nKey, rData);
		ASSERT(bCheck);
		data.LoadCaseKey = nKey;
		m_edtRLFactor.GetWindowText(csScaleFactor);
		if (!CStrParser::GetFloatNumber(csScaleFactor, data.dScaleFactor)) return FALSE;
	}

	return TRUE;
}

BOOL CCmdAutoLoadLiveLoadRFDlg::Item2Dlg(int nIndex, T_LLRF_BASE &data)
{
	m_LoadCase.SetCurSel(nIndex);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dScaleFactor);
	m_edtRLFactor.SetWindowText(csScaleFactor);

	return TRUE;
}

void CCmdAutoLoadLiveLoadRFDlg::Initial_LCaseComboBox()
{
	m_LoadCase.ResetContent();

	// Static
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();

	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);

			CString strLoadType = rData.LoadCaseType;
			CString strLoadCase = rData.LoadCaseName;
			if (CDBLib::IsStldLL(strLoadType))
			{
				m_LoadCase.AddString(strLoadCase);
			}
		}
	}

	
	if (m_LoadCase.GetCount() != 0)  m_LoadCase.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCase);
}

BOOL CCmdAutoLoadLiveLoadRFDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// TODO: Add your specialized code here and/or call the base class

	if (!m_pDoc->m_pAttrCtrl2->GetLlrf(m_Data))
	{
		m_Data.Initialize();
	}

	// Set Default Value
	CString csRF;
	csRF.Format(_T("%g"), 0.5);

	m_edtRLFactor.SetWindowText(csRF);

	// LoadCase ComboBox.
	Initial_LCaseComboBox();

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadLiveLoadRFDlg::OnOK()
{
	UpdateData(TRUE);
	if (m_pDoc->m_pDataCtrl->AddLlrf(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CCmdAutoLoadLiveLoadRFDlg::OnCancel()
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCmdAutoLoadLiveLoadRFDlg::OnClickLoadCaseBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CCmdAutoLoadLiveLoadRFDlg::OnCmdRFAddBtn()
{
	T_LLRF_BASE data;
	data.Initialize();

	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
	{
		T_LLRF_BASE LlrfBase = m_Data.aLoadPattern.GetAt(i);
		if (LlrfBase.LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}

	m_Data.aLoadPattern.Add(data);
	int nNumLC = m_Data.aLoadPattern.GetSize();
	m_ListCtrl.InsertItem(nNumLC - 1, _T(""));
	SetItem(nNumLC - 1, data);
	m_ListCtrl.SetItemState(nNumLC - 1, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdAutoLoadLiveLoadRFDlg::OnCmdRFModBtn()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_LLRF_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
	{
		T_LLRF_BASE LlrfBase = m_Data.aLoadPattern.GetAt(i);
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (LlrfBase.LoadCaseKey == data.LoadCaseKey) // Coded by Seungjun MNet:No.2516 ('20061018)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}

	m_Data.aLoadPattern.SetAt(nIndex, data);
	SetItem(nIndex, data);
	m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdAutoLoadLiveLoadRFDlg::OnCmdRFDelBtn()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_ListCtrl.DeleteItem(nIndex);
	m_Data.aLoadPattern.RemoveAt(nIndex);

	if (m_Data.aLoadPattern.GetSize() > 0)
	{
		if (nIndex >= m_Data.aLoadPattern.GetSize()) nIndex--;
		m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCmdAutoLoadLiveLoadRFDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nIndex;
	T_LLRF_BASE data;
	if (!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dScaleFactor = 0.0;
	}
	else data = m_Data.aLoadPattern.GetAt(nIndex);

	CString SeclectName = DataToStr(0, data);

	int Count = m_LoadCase.GetCount();
	for (int i = 0; i < Count; i++)
	{
		CString ComboListName = _T("");
		m_LoadCase.GetLBText(i, ComboListName);
		if (ComboListName == SeclectName)
		{
			// 값을 설정한다.
			Item2Dlg(i, data);
		}
	}

	*pResult = 0;
}

void CCmdAutoLoadLiveLoadRFDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CCmdAutoLoadLiveLoadRFDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bMFD = FALSE;
	BOOL bMFDList = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			bMFD = TRUE;
			break;
		case(UR_STLD_DEL):
			// 현재 loadcase list에서 지워진 것 삭제한다.
			pViewBuff->GetStld(nKey, Key, Data);
			for (i = m_Data.aLoadPattern.GetSize()-1; i >= 0; i--)
			{
				if (m_Data.aLoadPattern[i].LoadCaseKey == Key)
				{
					m_ListCtrl.DeleteItem(i);
					m_Data.aLoadPattern.RemoveAt(i);
				}
			}
			bMFD = bMFDList = TRUE;
			break;
		case(UR_STLD_MFD):
			// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
			pViewBuff->GetStld(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_STLD_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetStld(nKey, Key, Data);
			for (i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
			{
				if (m_Data.aLoadPattern[i].LoadCaseKey == Key)
				{
					m_Data.aLoadPattern[i].LoadCaseKey = KeyBak;
					SetItem(i, m_Data.aLoadPattern[i]);
				}
			}
			// 플래그 수정
			bMFD = TRUE;
			break;
		default:
			break;
		}
	} // end of while

// 	LLRF 의 변화를 반영한다.
// 		BOOL bUpdate = FALSE;
// 		pos = pViewBuff->GetStartBuffer();
// 		while (pos != NULL)
// 		{
// 			buffer_ur = pViewBuff->GetNextBuffer(pos);
// 			int nCmd = buffer_ur.nCmd;
// 			int nKey = buffer_ur.nKey;
// 	
// 			switch (nCmd)
// 			{
// 			case(UR_LLRF_ADD):
// 				pViewBuff->GetLlrf(nKey, m_Data);
// 				bUpdate = TRUE;
// 				break;
// 			}
// 		}
// 		if (bUpdate)
// 		{
// 			CString csVLFac, csSDS;
// 			csVLFac.Format(_T("%g"), m_Data.dDLoadFactor);
// 			csSDS.Format(_T("%g"), m_Data.dSDS);
// 	
// 			m_edtRLFactor.SetWindowText(csVLFac);
// 			m_SDS.SetWindowText(csSDS);
// 	
// 			MakeItemEx();
// 		}
	if (bMFDList) MakeItemEx();
	if (bMFD) Initial_LCaseComboBox();
}

BOOL CCmdAutoLoadLiveLoadRFDlg::CheckLoadCaseName(CString str, int& Index)
{
	BOOL bCheck = TRUE;
	int Count = m_ListCtrl.GetItemCount();
	for (int i = 0; i < Count; i++)
	{
		CString strLcName = m_ListCtrl.GetItemText(i, 0);
		if (str == strLcName)
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}
