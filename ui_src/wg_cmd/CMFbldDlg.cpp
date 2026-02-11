// CMFbldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMFbldDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#include "CMFbldPrefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMFbldDlg dialog


CCMFbldDlg::CCMFbldDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMFbldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMFbldDlg)
	m_csName = _T("");
	m_csDesc = _T("");
	//}}AFX_DATA_INIT
}


void CCMFbldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMFbldDlg)
	DDX_Text(pDX, IDC_CMD_FBLD_NAME, m_csName);
	DDX_Text(pDX, IDC_CMD_FBLD_DESC, m_csDesc);
	//}}AFX_DATA_MAP
	UINT aID[][FBLD_NUM] = {
		{IDC_CMD_FBLD_LC1, IDC_CMD_FBLD_LOAD1, IDC_CMD_FBLD_UNIT1, IDC_CMD_FBLD_SBW1},
		{IDC_CMD_FBLD_LC2, IDC_CMD_FBLD_LOAD2, IDC_CMD_FBLD_UNIT2, IDC_CMD_FBLD_SBW2},
		{IDC_CMD_FBLD_LC3, IDC_CMD_FBLD_LOAD3, IDC_CMD_FBLD_UNIT3, IDC_CMD_FBLD_SBW3},
		{IDC_CMD_FBLD_LC4, IDC_CMD_FBLD_LOAD4, IDC_CMD_FBLD_UNIT4, IDC_CMD_FBLD_SBW4},
		{IDC_CMD_FBLD_LC5, IDC_CMD_FBLD_LOAD5, IDC_CMD_FBLD_UNIT5, IDC_CMD_FBLD_SBW5},
		{IDC_CMD_FBLD_LC6, IDC_CMD_FBLD_LOAD6, IDC_CMD_FBLD_UNIT6, IDC_CMD_FBLD_SBW6},
		{IDC_CMD_FBLD_LC7, IDC_CMD_FBLD_LOAD7, IDC_CMD_FBLD_UNIT7, IDC_CMD_FBLD_SBW7},
		{IDC_CMD_FBLD_LC8, IDC_CMD_FBLD_LOAD8, IDC_CMD_FBLD_UNIT8, IDC_CMD_FBLD_SBW8},
	};
	for (int i = 0; i < FBLD_NUM; i++)
	{
		DDX_Control(pDX, aID[i][0], m_wndLC[i]);
		DDX_Control(pDX, aID[i][1], m_wndLoad[i]);
	  DDX_Control(pDX, aID[i][2], m_wndUnit[i]);
		DDX_Control(pDX, aID[i][3], m_wndSbw[i]);
	}
	DDX_Control(pDX, IDC_CMD_FBLD_GRID, m_wndGrid);
}

/////////////////////////////////////////////////////////////////////////////
// CCMFbldDlg public interface functions
void CCMFbldDlg::OnChangeCurrentFbld(T_FBLD_K nKey)
{
	T_FBLD_K key;
	T_FBLD_D data;
	if (nKey == 0)
		data.Initialize();
	else
	{
		if (!m_wndGrid.GetCurrentFbld(key))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___No_selected_F));
			return;
			data.Initialize();
		}
		if (!m_pDoc->m_pAttrCtrl->GetFbld(key, data))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___Can_t_read_Fb));
			return;
		}
	}
	
	m_csName = data.FloorLoadTypeName;
	m_csDesc = data.Description;

	CString csLoad;
	int nLoadKey;
	for (int i = 0; i < FBLD_NUM; i++)
	{
		if (data.LoadCaseKey[i] != 0)
		{
			nLoadKey = data.LoadCaseKey[i];
			csLoad.Format(_T("%g"), data.FloorLoad[i]);
			m_wndLoad[i].EnableWindow(TRUE);
			m_wndSbw[i].EnableWindow(TRUE);
			m_WndArr[i]->EnableWindow(TRUE);
		}
		else
		{
			nLoadKey = D_LOADCASE_NONE;
			csLoad = _T("0");
			m_wndLoad[i].EnableWindow(FALSE);
			m_wndSbw[i].EnableWindow(FALSE);
			m_WndArr[i]->EnableWindow(FALSE);
		}
		m_wndLC[i].ChangeSelect(D_LOADCASE_STATIC, nLoadKey);
		m_wndLoad[i].SetWindowText(csLoad);
		m_wndSbw[i].SetCheck(data.bSubBeamWeight[i]);
	}
	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CCMFbldDlg implementation
BOOL CCMFbldDlg::GetFbldData(T_FBLD_D& data)
{
	if (!UpdateData(TRUE)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_in_input_data_));
		return FALSE;
	}
	data.Initialize();

	data.FloorLoadTypeName = m_csName;
	data.Description = m_csDesc;
	int ii=0;
	
	UINT nLoadCaseType;
	T_STLD_K nLCkey;
	double dblFloorLoad;
	BOOL bSelected;
	for (int i = 0; i < FBLD_NUM; i++)
	{
		dblFloorLoad = m_wndLoad[i].GetEditValue();
		if (dblFloorLoad == 0.0) continue;
		bSelected = m_wndLC[i].GetSelectedLoad(nLoadCaseType, nLCkey);
		if (!bSelected)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
			return FALSE;
		}
		if (nLCkey == D_LOADCASE_NONE) continue;
		data.LoadCaseKey[ii] = nLCkey;
		data.FloorLoad[ii] = dblFloorLoad;
		data.bSubBeamWeight[ii] = m_wndSbw[i].GetCheck();
		ii++;
	}
	if (ii == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_load_value_is_input_));
		return FALSE;
	}
	return TRUE;
}

void CCMFbldDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		UnitChanged();
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

void CCMFbldDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_FBLD_K cKey;
	m_wndGrid.GetCurrentCell(ncRow, ncCol);
	m_wndGrid.GetColumnNoFor(m_wndGrid.GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid.GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_FBLD_K Key, KeyBak;
	T_FBLD_D Data, DataBak;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_FBLD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetFbld(nKey, Key, Data);
				m_wndGrid.InsertRecordByKey(Key, Data);
				break;
		case(UR_FBLD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetFbld(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
				break;
		case(UR_FBLD_MFD):
				// 여기서는 저장만 하고 UR_FBLD_MFS에서 처리한다.
				pViewBuff->GetFbld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_FBLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetFbld(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
				m_wndGrid.InsertRecordByKey(KeyBak, DataBak);
				if (Key == cKey) bCurrentRecordChanged = TRUE;
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	if (m_wndGrid.GetRowCount() < ncRow) ncRow = m_wndGrid.GetRowCount();
	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	if (!m_wndGrid.CheckCurrentRecordChanged())
	{
		// 현재 레코드 위치 변경없이 내용이 변경된 경우
		if (bCurrentRecordChanged) m_wndGrid.OnChangeCurrentRecord(ncRow);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMFbldDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}


BEGIN_MESSAGE_MAP(CCMFbldDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMFbldDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_FBLD_ADD, OnCmdFbldAdd)
	ON_BN_CLICKED(IDC_CMD_FBLD_DELETE, OnCmdFbldDelete)
	ON_BN_CLICKED(IDC_CMD_FBLD_MODIFY, OnCmdFbldModify)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC1, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC2, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC3, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC4, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC5, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC6, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC7, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_FBLD_LC8, OnChangeLoadCase)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON1, OnCmdLtypeButton1)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON2, OnCmdLtypeButton2)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON3, OnCmdLtypeButton3)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON4, OnCmdLtypeButton4)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON5, OnCmdLtypeButton5)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON6, OnCmdLtypeButton6)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON7, OnCmdLtypeButton7)
	ON_BN_CLICKED(IDC_CMD_LTYPE_BUTTON8, OnCmdLtypeButton8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMFbldDlg message handlers

BOOL CCMFbldDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON1));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON2));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON3));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON4));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON5));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON6));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON7));
	m_WndArr.Add(GetDlgItem(IDC_CMD_LTYPE_BUTTON8));

	for (int i = 0; i < FBLD_NUM; i++)
	{
		m_wndLC[i].SetLoadType(D_LOADCASE_STATIC, FALSE);
		m_wndLC[i].SetInputDialogMode();
		m_wndLC[i].ChangeSelect(D_LOADCASE_STATIC, D_LOADCASE_NONE);
		m_wndLoad[i].SetUnitType(CUnitCtrl::m_FBLD_UNIT.FloorLoad);
		m_wndUnit[i].SetUnitType(CUnitCtrl::m_FBLD_UNIT.FloorLoad);

		m_wndLoad[i].EnableWindow(FALSE);
		m_wndSbw[i].EnableWindow(FALSE);
		m_WndArr[i]->EnableWindow(FALSE);
	}

	if(m_pDoc->m_pAttrCtrl->GetCountFbld())
	{
		CArray<UINT, UINT> arKeyList;
		m_pDoc->m_pAttrCtrl->GetFbldKeyList(arKeyList);
		T_FBLD_D Data;
		m_pDoc->m_pAttrCtrl->GetFbld(arKeyList[0], Data);

		int nCount = 0;
		for(int i=0; i<FBLD_NUM; i++)
		{
			if(Data.LoadCaseKey[i] == D_LOADCASE_NONE || Data.LoadCaseKey[i] == 0)
			{
				nCount = i;
				break;
			}
		}
		for(int i=0; i<nCount; i++) m_WndArr[i]->EnableWindow(TRUE);
	}

	// Grid를 초기화 시킨다.
	m_wndGrid.Initialize(m_pDoc, this);
	m_wndGrid.CheckCurrentRecordChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMFbldDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();	
}

void CCMFbldDlg::OnCmdFbldAdd() 
{
	// TODO: Add your control notification handler code here
	T_FBLD_K key;
	T_FBLD_D data;
	if (!GetFbldData(data)) return;
	if (m_pDoc->m_pDataCtrl->AddFbld(data))
	{ //새로 추가된 FBLD로 이동(grid에서)
		key = m_pDoc->m_pAttrCtrl->GetFbldKey(data.FloorLoadTypeName);
		m_wndGrid.SetCurrentFbld(key, data);
	}
}

void CCMFbldDlg::OnCmdFbldDelete() 
{
	// TODO: Add your control notification handler code here
	T_FBLD_K key;
	T_FBLD_D data;
	if (!m_wndGrid.GetCurrentFbld(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_d));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetFbld(key, data));
	m_pDoc->m_pDataCtrl->DelFbld(data.FloorLoadTypeName);
}

void CCMFbldDlg::OnCmdFbldModify()
{
	// TODO: Add your control notification handler code here
	T_FBLD_K key;
	T_FBLD_D data;
	if (!m_wndGrid.GetCurrentFbld(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_m));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetFbld(key, data));
	CString csOldName = data.FloorLoadTypeName;
	unsigned int nOldTypeID = data.FloorLoadTypeId;
	if (!GetFbldData(data)) return;
	data.FloorLoadTypeId = nOldTypeID;
	m_pDoc->m_pDataCtrl->ModifyFbld(csOldName, data);
}

void CCMFbldDlg::OnChangeLoadCase() 
{
	// TODO: Add your control notification handler code here

	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	UINT nEditID;
	switch(nID)
	{
	case IDC_CMD_FBLD_LC1 : nEditID = 0; break;
	case IDC_CMD_FBLD_LC2 : nEditID = 1; break;
	case IDC_CMD_FBLD_LC3 : nEditID = 2; break;
	case IDC_CMD_FBLD_LC4 : nEditID = 3; break;
	case IDC_CMD_FBLD_LC5 : nEditID = 4; break;
	case IDC_CMD_FBLD_LC6 : nEditID = 5; break;
	case IDC_CMD_FBLD_LC7 : nEditID = 6; break;
	case IDC_CMD_FBLD_LC8 : nEditID = 7; break;
	default : ASSERT(0); break;
	}
	UINT nLoadCaseType, nLoadCaseKey;
	CSelectLC* pSelectLC = (CSelectLC*)GetDlgItem(nID);
	BOOL bGet = pSelectLC->GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	if (!bGet || nLoadCaseKey == D_LOADCASE_NONE)
	{
		m_wndLoad[nEditID].SetWindowText(_T("0"));
		m_wndSbw[nEditID].SetCheck(0);
		m_wndLoad[nEditID].EnableWindow(FALSE);
		m_wndSbw[nEditID].EnableWindow(FALSE);
		m_WndArr[nEditID]->EnableWindow(FALSE);
		
	}
	else
	{
		m_wndLoad[nEditID].EnableWindow(TRUE);
		m_wndSbw[nEditID].EnableWindow(TRUE);
		m_WndArr[nEditID]->EnableWindow(TRUE);
	}
}

void CCMFbldDlg::OnCmdLoadcaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMFbldDlg::OnCmdLtypeButton1() 
{
	// TODO: Add your control notification handler code here
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON1);
	CRect BRect;
	pWnd->GetWindowRect(BRect);

	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[0].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton2() 
{
	// TODO: Add your control notification handler code here
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON2);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[1].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton3() 
{
	// TODO: Add your control notification handler code here
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON3);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[2].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton4() 
{
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON4);
	CRect BRect;
	pWnd->GetWindowRect(BRect);

	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[3].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton5() 
{
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON5);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[4].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton6() 
{
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON6);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[5].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton7() 
{
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON7);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[6].SetEditUnit(Dlg.GetCurLoadValue());
	}
}

void CCMFbldDlg::OnCmdLtypeButton8() 
{
	CWnd*pWnd = GetDlgItem(IDC_CMD_LTYPE_BUTTON8);
	CRect BRect;
	pWnd->GetWindowRect(BRect);
	
	CCMFbldPrefDlg Dlg(NULL);
	if(IDOK == Dlg.MyDoModal(BRect.left,BRect.bottom))
	{
		m_wndLoad[7].SetEditUnit(Dlg.GetCurLoadValue());
	}
}