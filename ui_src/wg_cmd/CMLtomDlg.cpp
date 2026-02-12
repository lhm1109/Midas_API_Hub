// CMLtomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLtomDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLtomDlg dialog


CCMLtomDlg::CCMLtomDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLtomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLtomDlg)
	//}}AFX_DATA_INIT
	m_bWorkTab = FALSE;
	m_WTLoadCaseNum = -1;

	m_aCtrlDir.Add(IDC_CMD_DIR_X);
	m_aCtrlDir.Add(IDC_CMD_DIR_Y);
	m_aCtrlDir.Add(IDC_CMD_DIR_Z);
	m_aCtrlDir.Add(IDC_CMD_DIR_XY);
	m_aCtrlDir.Add(IDC_CMD_DIR_YZ);
	m_aCtrlDir.Add(IDC_CMD_DIR_XZ);
	m_aCtrlDir.Add(IDC_CMD_DIR_XYZ);
}


void CCMLtomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLtomDlg)
	DDX_Control(pDX, IDC_CMD_GRAVITY_UNIT, m_wndGravityUnit);
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_LoadCase);
	DDX_Control(pDX, IDC_CMD_REMOVE_DATA, m_wndRemoveBtn);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_GRAVITY, m_wndGravity);
	DDX_Control(pDX, IDC_CMD_CHK_PRES, m_wndPres);
	DDX_Control(pDX, IDC_CMD_CHK_FBLD, m_wndFbld);
	DDX_Control(pDX, IDC_CMD_CHK_BMLD, m_wndBmld);
	DDX_Control(pDX, IDC_CMD_CHK_CNLD, m_wndCnld);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMLtomDlg Implementation

void CCMLtomDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__LoadCase), _LS(IDS_WG_CMD__ADDD__ScaleFactor)};
	int nColWidth[] = {72, 44};
	int nColNum = 2;
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
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMLtomDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMLtomDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.nNumLtomCase;
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.LtomCase[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMLtomDlg::DataToStr(int i, T_LTOM_BASE &data)
{
	CString value;
	if (i == 0) 
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CCMLtomDlg::SetItem(int nIndex, T_LTOM_BASE &data)
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

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCMLtomDlg::ValidItem(T_LTOM_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CCMLtomDlg::Dlg2Item(T_LTOM_BASE &data)
{
	UINT nLoadCaseType;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;
//  data.dblFactor = _tstof(csScaleFactor);

	return TRUE;
}

BOOL CCMLtomDlg::Item2Dlg(T_LTOM_BASE &data)
{
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);

	return TRUE;
}

void CCMLtomDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDir, m_Data.nDirection-1); // XY
	m_wndCnld.SetCheck(m_Data.bNodalLoad);
	m_wndBmld.SetCheck(m_Data.bBeamLoad);
	m_wndFbld.SetCheck(m_Data.bFloorLoad);
	m_wndPres.SetCheck(m_Data.bPressureLoad);
	m_wndGravity.SetEditUnit(m_Data.dGravity);
	m_wndScaleFactor.SetWindowText(_T("1"));

	MakeItemEx();
}

BEGIN_MESSAGE_MAP(CCMLtomDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLtomDlg)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLtomDlg message handlers

BOOL CCMLtomDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_pDoc->m_pAttrCtrl->GetLtom(m_Data))
	{
		GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		m_Data.Initialize();
	 #ifdef _MGEN
		m_Data.nDirection = 4;
	 #endif
	 #ifdef _CIVIL
		m_Data.nDirection = 7;
	 #endif
		m_Data.bNodalLoad = 1;
		m_Data.bBeamLoad = 1;
		m_Data.bFloorLoad = 1;
		m_Data.bPressureLoad = 1;
		m_Data.dGravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	}

	m_wndScaleFactor.SetUnitType(0);
	m_wndGravity.SetUnitType(CUnitCtrl::m_LTOM_UNIT.dGravity);
	m_wndGravityUnit.SetUnitType(CUnitCtrl::m_LTOM_UNIT.dGravity);

	m_LoadCase.SetLoadType(D_LOADCASE_STATIC);
	SetListCtrlHeader();

	// Set Value
	Data2Dlg();

	if(m_bWorkTab) SetCurrentLC();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLtomDlg::SetParamData(int iItem)
{
	m_bWorkTab = TRUE;
	m_WTLoadCaseNum = iItem;
}

void CCMLtomDlg::OnCmdLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMLtomDlg::OnCmdRemoveData() 
{
	// TODO: Add your control notification handler code here
	if (m_pDoc->m_pDataCtrl->DelLtom())
	{
		CDialogMove::OnOK();
	}
}

void CCMLtomDlg::OnCmdBtnAdd() 
{
	T_LTOM_BASE data;
	
	if (m_Data.nNumLtomCase >= D_LTOM_NUMLTOM) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumLtomCase; i++)
	{
		if (m_Data.LtomCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.LtomCase[m_Data.nNumLtomCase] = data;
	m_List.InsertItem(m_Data.nNumLtomCase, _T(""));
	SetItem(m_Data.nNumLtomCase, data);
	m_List.SetItemState(m_Data.nNumLtomCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLtomDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_LTOM_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumLtomCase; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.LtomCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.LtomCase[nIndex] = data;
	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLtomDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);
	for (int i = nIndex; i < m_Data.nNumLtomCase-1; i++)
		m_Data.LtomCase[i] = m_Data.LtomCase[i+1];
	m_Data.nNumLtomCase--;
	if (m_Data.nNumLtomCase > 0)
	{
		if (nIndex >= m_Data.nNumLtomCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CCMLtomDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDir, m_Data.nDirection);
	m_Data.nDirection++;
	m_Data.bNodalLoad    = m_wndCnld.GetCheck() ? 1 : 0;
	m_Data.bBeamLoad     = m_wndBmld.GetCheck() ? 1 : 0;
	m_Data.bFloorLoad = FALSE;
	m_Data.bFloorLoad    = m_wndFbld.GetCheck() ? 1 : 0;
	m_Data.bPressureLoad = m_wndPres.GetCheck() ? 1 : 0;
	m_Data.dGravity = m_wndGravity.GetEditValue();

	// 입력되지 않는 PdelCase는 초기화한다.
	for (int i = m_Data.nNumLtomCase; i < D_LTOM_NUMLTOM; i++)
		m_Data.LtomCase[i].Initialize();
	if (m_pDoc->m_pDataCtrl->AddLtom(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CCMLtomDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_LTOM_BASE data;
	if (!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dblFactor = 1.0;
	}
	else data = m_Data.LtomCase[nIndex];

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CCMLtomDlg::SetCurrentLC()
{
	m_bWorkTab = FALSE;
	T_LTOM_BASE data;

	data = m_Data.LtomCase[m_WTLoadCaseNum];
	m_List.SetItemState(m_WTLoadCaseNum, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	Item2Dlg(data);
}

void CCMLtomDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLtomDlg::UpdateBuffer()
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
				for (i = m_Data.nNumLtomCase-1; i >= 0; i--)
				{
					if (m_Data.LtomCase[i].LoadCaseKey == Key)
					{
						m_List.DeleteItem(i);
						for (int j = i; j < m_Data.nNumLtomCase-1; j++)
							m_Data.LtomCase[j] = m_Data.LtomCase[j+1];
						m_Data.nNumLtomCase--;
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
				for (i = 0; i < m_Data.nNumLtomCase; i++)
				{
					if (m_Data.LtomCase[i].LoadCaseKey == Key)
					{
						m_Data.LtomCase[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.LtomCase[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// LTOM 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_LTOM_ADD):
			pViewBuff->GetLtom(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_LTOM_DEL):
			break;  // do nothing
		}
	}
	if (bUpdate)
	{
		Data2Dlg();
	}
}
