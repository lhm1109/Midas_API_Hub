// CDgnGenSeisFactor.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSeisFactor.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor dialog


CDgnGenSeisFactor::CDgnGenSeisFactor(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenSeisFactor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenSeisFactor)
	m_bHideSpecialGroup = FALSE;
	m_strCode.Empty();
	//}}AFX_DATA_INIT
}

void CDgnGenSeisFactor::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSeisFactor)
	DDX_Control(pDX, IDC_DGN_OSF_CASE, m_LoadCase);

	DDX_Control(pDX, IDC_DGN_SEIS_OSF_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_DGN_VERT_FAC1, m_VLFactor);
	DDX_Control(pDX, IDC_DGN_SPS_SDS,   m_SDS);
	DDX_Control(pDX, IDC_DGN_OVER_FAC,  m_OSFactor);
	DDX_Control(pDX, IDC_DGN_VERT_FAC2, m_VFFactor);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenSeisFactor, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnGenSeisFactor)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SEIS_OSF_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_DGN_SEIS_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_DGN_OSF_ADD, OnDgnGenSeisAdd)
	ON_BN_CLICKED(IDC_DGN_OSF_MODIFY, OnDgnGenSeisModify)
	ON_BN_CLICKED(IDC_DGN_OSF_DELETE, OnDgnGenSeisDelete)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor message handlers

void CDgnGenSeisFactor::SetListCtrlHeader()
{
	CString aTitle[] = { _T("Load Case"), _T("Factor") };
	int nColWidth[] = {85, 60};
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
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
		m_ListCtrl.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CDgnGenSeisFactor::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CDgnGenSeisFactor::MakeItemEx()
{
	m_ListCtrl.DeleteAllItems();

	int nItemCount = m_Data.aLoadPattern.GetSize(); //!/
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_ListCtrl.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.aLoadPattern.GetAt(nCount));
	}
	m_ListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CDgnGenSeisFactor::DataToStr(int i, T_SOSF_BASE &data)
{
	CString value;
	if (i == 0) 
	{
		if(data.nLoadCaseType==0)
		{
			T_STLD_K key = data.LoadCaseKey;
			T_STLD_D rData;
			m_pDoc->m_pAttrCtrl->GetStld(key, rData);
			value = rData.LoadCaseName+_T("(ST)");
		}
		else if(data.nLoadCaseType==1)
		{
			T_SPLC_K key = data.LoadCaseKey;
			T_SPLC_D rData;
			m_pDoc->m_pAttrCtrl->GetSplc(key, rData);
			value = rData.LoadCaseName+_T("(RS)");
		}
	}
	else if (i == 1) value.Format(_T("%g"), data.dScaleFactor);
	return value;
}

void CDgnGenSeisFactor::SetItem(int nIndex, T_SOSF_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ListCtrl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CDgnGenSeisFactor::ValidItem(T_SOSF_BASE& data)
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

BOOL CDgnGenSeisFactor::Dlg2Item(T_SOSF_BASE &data)
{
	CString csScaleFactor;
	int Index = m_LoadCase.GetCurSel();
	if(Index != -1)
	{
		CString strName=_T("");
		m_LoadCase.GetLBText(Index,strName);

		int iLoadCaseType = GetLoadCaseType(Index);
	
		CString strLCName = strName;
		int nKey;
		if(iLoadCaseType==0)
		{
			strLCName.TrimRight(_T(")"));
			strLCName.TrimRight(_T("T"));
			strLCName.TrimRight(_T("S"));
			strLCName.TrimRight(_T("("));
			
			nKey = m_pDoc->m_pAttrCtrl->GetStldKey(strLCName);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(nKey, rData);
			ASSERT(bCheck);            
			data.LoadCaseKey = nKey;
			data.nLoadCaseType = 0;
			m_OSFactor.GetWindowText(csScaleFactor);
			if(!CStrParser::GetFloatNumber(csScaleFactor, data.dScaleFactor)) return FALSE;
		}
		else if(iLoadCaseType==1)
		{
			strLCName.TrimRight(_T(")"));
			strLCName.TrimRight(_T("S"));
			strLCName.TrimRight(_T("R"));
			strLCName.TrimRight(_T("("));
			
			nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strLCName);
			T_SPLC_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(nKey, rData);
			ASSERT(bCheck);            
			data.LoadCaseKey = nKey;
			data.nLoadCaseType = 1;
			m_OSFactor.GetWindowText(csScaleFactor);
			if(!CStrParser::GetFloatNumber(csScaleFactor, data.dScaleFactor)) return FALSE;
		}
		else
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
			return FALSE;
		}

	}

	return TRUE;
}

BOOL CDgnGenSeisFactor::Item2Dlg(int nIndex, T_SOSF_BASE &data)
{
	m_LoadCase.SetCurSel(nIndex);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dScaleFactor);
	m_OSFactor.SetWindowText(csScaleFactor);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor message handlers

void CDgnGenSeisFactor::Initial_LCaseComboBox()
{
	m_LoadCase.ResetContent();
	m_arLCtype.RemoveAll();

	// Static
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
		
	if(iCount>0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            
			
			CString strLoadType = rData.LoadCaseType;
			CString strLoadCase = rData.LoadCaseName;
			if(CDBLib::IsStldEL(strLoadType)) 
			{
				m_LoadCase.AddString(strLoadCase+_T("(ST)"));    
				
				_DGN_SEIS_LCTP LCType;
				LCType.index		 = m_LoadCase.GetCount()-1;
				LCType.iLoadCaseType = 0; // 0: Static,		1: Response Spectrum
				m_arLCtype.Add(LCType);
			}
				
		}    
	}

	// Spectrum.
	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(KeyList);
	iCount = KeyList.GetSize();  
	if(iCount > 0)
	{
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<iCount; i++)
		{
			T_SPLC_D rData;
			rData.Initialize();
			int key = KeyList.GetAt(i);

			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(key,rData);
			ASSERT(bCheck);

			CString str = rData.LoadCaseName;
			m_LoadCase.AddString(str + _T("(RS)"));

			_DGN_SEIS_LCTP LCType;
			LCType.index		 = m_LoadCase.GetCount()-1;
			LCType.iLoadCaseType = 1; // 0: Static,		1: Response Spectrum
			m_arLCtype.Add(LCType);

		}
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_LoadCase);  
}

BOOL CDgnGenSeisFactor::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	//change by cylee 05.04.20
	if (!m_pDoc->m_pAttrCtrl->GetSlcf(m_Data))
	{
		m_Data.Initialize(m_strCode);
	}

	// Set Default Value
	CString csVLFac, csSDS, csOSFac, csVFFac;
	csVLFac.Format(_T("%g"), m_Data.dDLoadFactor);
	csSDS.Format(_T("%g"), m_Data.dSDS);
	csVFFac.Format(_T("%g"), m_Data.dVLoadFactor);

	m_VLFactor.SetWindowText(csVLFac);
	m_SDS.SetWindowText(csSDS);
	m_VFFactor.SetWindowText(csVFFac);

	// LoadCase ComboBox.
	Initial_LCaseComboBox();

	SetListCtrlHeader();
	MakeItemEx();
	EnableCtrl();
	ShowHideCtrl();
	SetTextCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenSeisFactor::OnOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString csVLFac, csSDS, csOSFac, csVFFac;
	m_VLFactor.GetWindowText(csVLFac);
	m_SDS.GetWindowText(csSDS);
	m_OSFactor.GetWindowText(csOSFac);
	m_VFFactor.GetWindowText(csVFFac);

	if(!CStrParser::GetFloatNumber(csVLFac, m_Data.dDLoadFactor)) return;
	if(!CStrParser::GetFloatNumber(csSDS,   m_Data.dSDS)) return;
	if(!CStrParser::GetFloatNumber(csVFFac, m_Data.dVLoadFactor)) return;

	if (m_pDoc->m_pDataCtrl->AddSlcf(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CDgnGenSeisFactor::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CDgnGenSeisFactor::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CDgnGenSeisFactor::OnDgnGenSeisAdd() 
{
	T_SOSF_BASE data;

	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
	{
		T_SOSF_BASE SosfBase = m_Data.aLoadPattern.GetAt(i);
		// Coded by Seungjun MNet:No.2516 ('20061018)
		if (SosfBase.LoadCaseKey == data.LoadCaseKey && SosfBase.nLoadCaseType == data.nLoadCaseType)
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
	m_ListCtrl.InsertItem(nNumLC-1, _T(""));
	SetItem(nNumLC-1, data);
	m_ListCtrl.SetItemState(nNumLC-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDgnGenSeisFactor::OnDgnGenSeisModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_SOSF_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
	{
		T_SOSF_BASE SosfBase = m_Data.aLoadPattern.GetAt(i);
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (SosfBase.LoadCaseKey == data.LoadCaseKey && data.nLoadCaseType==data.nLoadCaseType) // Coded by Seungjun MNet:No.2516 ('20061018)
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

void CDgnGenSeisFactor::OnDgnGenSeisDelete() 
{
	// TODO: Add your control notification handler code here
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
		m_ListCtrl.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CDgnGenSeisFactor::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_SOSF_BASE data;
	if (!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dScaleFactor = 0.0;
	}
	else
	{
		data = m_Data.aLoadPattern.GetAt(nIndex);
	}
	
	CString SeclectName = DataToStr(0, data);
	
	int Count = m_LoadCase.GetCount();
	for(int i=0; i<Count; i++)
	{
		CString ComboListName=_T("");
		m_LoadCase.GetLBText(i,ComboListName);
		if(ComboListName==SeclectName)
		{
			// 값을 설정한다.
			Item2Dlg(i, data);
		}
	}
	
	*pResult = 0;
}

void CDgnGenSeisFactor::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnGenSeisFactor::UpdateBuffer()
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
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// do nothing
				break;
		case(UR_STLD_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = m_Data.aLoadPattern.GetSize(); i >= 0; i--)
				{
					if (m_Data.aLoadPattern[i].LoadCaseKey == Key && m_Data.aLoadPattern[i].nLoadCaseType==0)
					{
						m_ListCtrl.DeleteItem(i);
						m_Data.aLoadPattern.RemoveAt(i);
					}
				}
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
					if (m_Data.aLoadPattern[i].LoadCaseKey == Key && m_Data.aLoadPattern[i].nLoadCaseType==0)
					{
						m_Data.aLoadPattern[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.aLoadPattern[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// 
	T_SPLC_K spKey, spKeyBak;
	T_SPLC_D spData, spDataBak;

	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SPLC_ADD):
				// do nothing
				break;
		case(UR_SPLC_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				for (i = m_Data.aLoadPattern.GetSize(); i >= 0; i--)
				{
					if (m_Data.aLoadPattern[i].LoadCaseKey == spKey && m_Data.aLoadPattern[i].nLoadCaseType==1)
					{
						m_ListCtrl.DeleteItem(i);
						m_Data.aLoadPattern.RemoveAt(i);
					}
				}
				break;
		case(UR_SPLC_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetSplc(nKey, spKey, spData);
				spKeyBak = spKey;
				spDataBak = spData;
				bMFD = TRUE;
				break;
		case(UR_SPLC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetSplc(nKey, spKey, spData);
				for (i = 0; i < m_Data.aLoadPattern.GetSize(); i++)
				{
					if (m_Data.aLoadPattern[i].LoadCaseKey == spKey && m_Data.aLoadPattern[i].nLoadCaseType==1)
					{
						m_Data.aLoadPattern[i].LoadCaseKey = spKeyBak;
						SetItem(i, m_Data.aLoadPattern[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while


	// SLCF 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SLCF_ADD):
			pViewBuff->GetSlcf(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_SLCF_DEL):
			break;  // do nothing
		}
	}
	if (bUpdate)
	{
		CString csVLFac, csSDS, csVFFac;
		csVLFac.Format(_T("%g"), m_Data.dDLoadFactor);
		csSDS.Format(_T("%g"), m_Data.dSDS);
		csVFFac.Format(_T("%g"), m_Data.dVLoadFactor);

		m_VLFactor.SetWindowText(csVLFac);
		m_SDS.SetWindowText(csSDS);
		m_VFFactor.SetWindowText(csVFFac);

		MakeItemEx();
	}
}

BOOL CDgnGenSeisFactor::CheckLoadCaseName(CString str, int& Index)
{
	BOOL bCheck=TRUE;
	int Count = m_ListCtrl.GetItemCount();
	for(int i=0; i<Count; i++)
	{
		CString strLcName = m_ListCtrl.GetItemText(i,0);
		if(str == strLcName)  
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

int CDgnGenSeisFactor::GetLoadCaseType(int iIndex)
{
	int iType;
	for(int i=0; i<m_arLCtype.GetSize(); i++)
	{
		_DGN_SEIS_LCTP LCtype = m_arLCtype.GetAt(i);
		if(iIndex==LCtype.index)
			iType = LCtype.iLoadCaseType;		
	}
	return iType;
}

void CDgnGenSeisFactor::EnableCtrl()
{
	CArray<UINT, UINT> aCtrlDisable;
	if (m_strCode == CONCODE_TWN_USD112)
	{		
		aCtrlDisable.Add(IDC_WG_DGN_STATIC1);
		aCtrlDisable.Add(IDC_DGN_VERT_FAC1);
		aCtrlDisable.Add(IDC_WG_DGN_STATIC7);
		aCtrlDisable.Add(IDC_DGN_SPS_SDS);
		aCtrlDisable.Add(IDC_WG_DGN_STATIC5);
		aCtrlDisable.Add(IDC_WG_DGN_STATIC6);
		aCtrlDisable.Add(IDC_DGN_VERT_FAC2);
	}

	if (aCtrlDisable.GetSize() > 1)
	{
		CDlgUtil::CtrlEnableDisable(this, aCtrlDisable, FALSE);
	}	
}

void CDgnGenSeisFactor::ShowHideCtrl()
{
	if(!m_bHideSpecialGroup) return;

	CArray<UINT,UINT> aCtrl_Hide;
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC0);
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC1);
	aCtrl_Hide.Add(IDC_DGN_VERT_FAC1);
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC7);
	aCtrl_Hide.Add(IDC_DGN_SPS_SDS);
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC2);
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC3);
	aCtrl_Hide.Add(IDC_DGN_OSF_CASE);
	aCtrl_Hide.Add(IDC_DGN_SEIS_LC);
	aCtrl_Hide.Add(IDC_WG_DGN_STATIC4);
	aCtrl_Hide.Add(IDC_DGN_OVER_FAC);
	aCtrl_Hide.Add(IDC_DGN_SEIS_OSF_LIST);
	aCtrl_Hide.Add(IDC_DGN_OSF_ADD);
	aCtrl_Hide.Add(IDC_DGN_OSF_MODIFY);
	aCtrl_Hide.Add(IDC_DGN_OSF_DELETE);
	CDlgUtil::CtrlShowHide(this, aCtrl_Hide, SW_HIDE);  

	CArray<UINT,UINT> aCtrl_MoveUp;
	aCtrl_MoveUp.Add(IDC_WG_DGN_STATIC5);
	aCtrl_MoveUp.Add(IDC_WG_DGN_STATIC6);
	aCtrl_MoveUp.Add(IDC_DGN_VERT_FAC2);
	aCtrl_MoveUp.Add(IDOK);
	aCtrl_MoveUp.Add(IDCANCEL);

	CRect rRef, rToMove;
	GetDlgItem(IDC_WG_DGN_STATIC0)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_STATIC5)->GetWindowRect(rToMove);
	int nDist = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrl_MoveUp, nDist, TRUE);

	// resize self size  
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(rToMove);
}

void CDgnGenSeisFactor::SetTextCtrl()
{
	const bool bNSCP2015 = [this]()
	{
		if (m_strCode == STLCODE_NSCP_2015_LRFD) { return true; }
		if (m_strCode == STLCODE_NSCP_2015_ASD) { return true; }
		if (m_strCode == CONCODE_NSCP_2015) { return true; }

		return false;
	}();

	if (bNSCP2015)
	{
		GetDlgItem(IDC_WG_DGN_STATIC7)->SetWindowText(_T("Ca*I :"));
		GetDlgItem(IDC_WG_DGN_STATIC6)->SetWindowText(_LS(IDS_WG_CMD__ADDD__Factor) + _T(" = R*Ca*I, R = "));
	}
}