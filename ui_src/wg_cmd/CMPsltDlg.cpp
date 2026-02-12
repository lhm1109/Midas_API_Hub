// CMPsltDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPsltDlg.h"
#include "CMPsltHelpDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _MGEN_CH
#include "..\wg_base\ColorTreeCtrl.h"
#endif
//#include "CMPsltPrefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { LTYPE_UNIFORM=0, LTYPE_LINEAR=1 };

/////////////////////////////////////////////////////////////////////////////
// CCMPsltDlg dialog


CCMPsltDlg::CCMPsltDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMPsltDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMPsltDlg)
	m_csName = _T("");
	m_csDesc = _T("");
	//}}AFX_DATA_INIT
	m_StartPsltK = 0;
}


void CCMPsltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPsltDlg)
	DDX_Text(pDX, IDC_CMD_PRES_NAME, m_csName);
	DDX_Text(pDX, IDC_CMD_PRES_DESC, m_csDesc);
	DDX_Control(pDX, IDC_CMD_PRES_ELTYPE_CMB, m_cmbElemType);
	//}}AFX_DATA_MAP

	UINT aID[][PSLT_NUM] = {
		 {IDC_CMD_PRES_LCASE_CMB1, IDC_CMD_PRES_LTYPE_CMB1, IDC_CMD_PRES_LOAD1, IDC_CMD_PRES_LOAD9 , IDC_CMD_PRES_LOAD17, IDC_CMD_PRES_LOAD25, IDC_CMD_PRES_UNIT1},
		 {IDC_CMD_PRES_LCASE_CMB2, IDC_CMD_PRES_LTYPE_CMB2, IDC_CMD_PRES_LOAD2, IDC_CMD_PRES_LOAD10, IDC_CMD_PRES_LOAD18, IDC_CMD_PRES_LOAD26, IDC_CMD_PRES_UNIT2},
		 {IDC_CMD_PRES_LCASE_CMB3, IDC_CMD_PRES_LTYPE_CMB3, IDC_CMD_PRES_LOAD3, IDC_CMD_PRES_LOAD11, IDC_CMD_PRES_LOAD19, IDC_CMD_PRES_LOAD27, IDC_CMD_PRES_UNIT3},
		 {IDC_CMD_PRES_LCASE_CMB4, IDC_CMD_PRES_LTYPE_CMB4, IDC_CMD_PRES_LOAD4, IDC_CMD_PRES_LOAD12, IDC_CMD_PRES_LOAD20, IDC_CMD_PRES_LOAD28, IDC_CMD_PRES_UNIT4},
		 {IDC_CMD_PRES_LCASE_CMB5, IDC_CMD_PRES_LTYPE_CMB5, IDC_CMD_PRES_LOAD5, IDC_CMD_PRES_LOAD13, IDC_CMD_PRES_LOAD21, IDC_CMD_PRES_LOAD29, IDC_CMD_PRES_UNIT5},
		 {IDC_CMD_PRES_LCASE_CMB6, IDC_CMD_PRES_LTYPE_CMB6, IDC_CMD_PRES_LOAD6, IDC_CMD_PRES_LOAD14, IDC_CMD_PRES_LOAD22, IDC_CMD_PRES_LOAD30, IDC_CMD_PRES_UNIT6},
		 {IDC_CMD_PRES_LCASE_CMB7, IDC_CMD_PRES_LTYPE_CMB7, IDC_CMD_PRES_LOAD7, IDC_CMD_PRES_LOAD15, IDC_CMD_PRES_LOAD23, IDC_CMD_PRES_LOAD31, IDC_CMD_PRES_UNIT7},
		 {IDC_CMD_PRES_LCASE_CMB8, IDC_CMD_PRES_LTYPE_CMB8, IDC_CMD_PRES_LOAD8, IDC_CMD_PRES_LOAD16, IDC_CMD_PRES_LOAD24, IDC_CMD_PRES_LOAD32, IDC_CMD_PRES_UNIT8},
	};
	for (int i = 0; i < PSLT_NUM; i++)
	{
		DDX_Control(pDX, aID[i][0], m_cmbLCase [i]);
		DDX_Control(pDX, aID[i][1], m_cmbLType [i]);
		DDX_Control(pDX, aID[i][2], m_wndLoadP1[i]);
		DDX_Control(pDX, aID[i][3], m_wndLoadP2[i]);
		DDX_Control(pDX, aID[i][4], m_wndLoadP3[i]);
		DDX_Control(pDX, aID[i][5], m_wndLoadP4[i]);
		DDX_Control(pDX, aID[i][6], m_wndUnit  [i]);
	}
	DDX_Control(pDX, IDC_CMD_PRES_ELEMCOLOR_CMB, m_WellButtonColor);
	DDX_Control(pDX, IDC_CMD_PRES_GRID, m_wndGrid);
}

/////////////////////////////////////////////////////////////////////////////
// CCMPsltDlg public interface functions
void CCMPsltDlg::OnChangeCurrentPslt(T_PSLT_K nKey)
{
	T_PSLT_K key;
	T_PSLT_D data;
	if (nKey == 0)
		data.Initialize();
	else
	{
		if (!m_wndGrid.GetCurrentPslt(key))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___No_selected_F));
			return;
			data.Initialize();
		}
		if (!m_pDoc->m_pAttrCtrl2->GetPslt(key, data))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___Can_t_read_Fb));
			return;
		}
	}

	m_csName = data.strName;
	m_csDesc = data.strDesc;
	m_cmbElemType.SetCurSel(data.nElemType);
	SetPresUnit();

	CString csLoad[4];
	int nLoadKey;
	for (int i = 0; i < PSLT_NUM; i++)
	{
		if (data.LoadCaseKey[i] != 0)
		{
			nLoadKey = data.LoadCaseKey[i];
			csLoad[0].Format(_T("%g"), data.dLoadP1[i]);
			csLoad[1].Format(_T("%g"), data.dLoadP2[i]);
			csLoad[2].Format(_T("%g"), data.dLoadP3[i]);
			csLoad[3].Format(_T("%g"), data.dLoadP4[i]);
		}
		else
		{
			nLoadKey = D_LOADCASE_NONE;
			csLoad[0] = _T("0");
			csLoad[1] = _T("0");
			csLoad[2] = _T("0");
			csLoad[3] = _T("0");
		}
		m_cmbLCase[i].ChangeSelect(D_LOADCASE_STATIC, nLoadKey);
		m_cmbLType[i].SetCurSel(data.nLoadType[i]);
		m_wndLoadP1[i].SetWindowText(csLoad[0]);
		m_wndLoadP2[i].SetWindowText(csLoad[1]);
		m_wndLoadP3[i].SetWindowText(csLoad[2]);
		m_wndLoadP4[i].SetWindowText(csLoad[3]);
	}
#ifdef _MGEN_CH
	m_WellButtonColor.SetColor(data.color);
#endif
	EnableDisableCtrls();

	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CCMPsltDlg implementation
BOOL CCMPsltDlg::GetPsltData(T_PSLT_D& data,BOOL bModify /* = FALSE */)
{
	if (!UpdateData(TRUE)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_in_input_data_));
		return FALSE;
	}
#if defined (_MGEN_CH)
	COLORREF Oldcolor = data.color;
	int nPreElemType = data.nElemType;
#endif
	data.Initialize();

	data.strName = m_csName;
	data.strDesc = m_csDesc;
	data.nElemType = m_cmbElemType.GetCurSel();

	int ii=0;
	for (int i = 0; i < PSLT_NUM; i++)
	{
		double dLoad[4];
		dLoad[0] = m_wndLoadP1[i].GetEditValue();
		dLoad[1] = m_wndLoadP2[i].GetEditValue();
		dLoad[2] = m_wndLoadP3[i].GetEditValue();
		dLoad[3] = m_wndLoadP4[i].GetEditValue();
		
		if(dLoad[0]==0. && dLoad[1]==0. && dLoad[2]==0. && dLoad[3]==0.) continue;
		
		UINT nLoadCaseType;
		T_STLD_K nLCkey;
		BOOL bSelected = m_cmbLCase[i].GetSelectedLoad(nLoadCaseType, nLCkey);
		if (!bSelected)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
			return FALSE;
		}
		if (nLCkey == D_LOADCASE_NONE) continue;
		
		data.LoadCaseKey[ii] = nLCkey;
		data.nLoadType  [ii] = m_cmbLType[i].GetItemData(m_cmbLType[i].GetCurSel());
		data.dLoadP1    [ii] = dLoad[0];
		data.dLoadP2    [ii] = dLoad[1];
		data.dLoadP3    [ii] = dLoad[2];
		data.dLoadP4    [ii] = dLoad[3];
		ii++;
	}
	if (ii == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_load_value_is_input_));
		return FALSE;
	}
#if defined (_MGEN_CH)
	int nEType = m_cmbElemType.GetCurSel();
	if(nEType == 0)
	{
		COLORREF color = m_WellButtonColor.GetColor();
		T_PSLT_K curPsltk = 0;
		if(bModify && nPreElemType == 0)
		{
			data.color = Oldcolor;
			if(color == data.color)
				return TRUE;
		}
		BOOL bExistSameColor = FALSE;
		POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartPslt();
		T_PSLT_K psltk;
		T_PSLT_D psltd;
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextPslt(pos,psltk,psltd);
			if(psltd.color == color && psltd.nElemType == 0)
			{
				bExistSameColor = TRUE;
				break;
			}
		}
		if(bExistSameColor)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Pslt_color__is_already_ex));
			return FALSE;
		}
		data.color = color;
	}
#endif
	return TRUE;
}

void CCMPsltDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMPsltDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_PSLT_K cKey;
	m_wndGrid.GetCurrentCell(ncRow, ncCol);
	m_wndGrid.GetColumnNoFor(m_wndGrid.GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid.GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_PSLT_K Key, KeyBak;
	T_PSLT_D Data, DataBak;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_PSLT_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetPslt(nKey, Key, Data);
			m_wndGrid.InsertRecordByKey(Key, Data);
			break;
		case(UR_PSLT_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetPslt(nKey, Key, Data);
			m_wndGrid.DeleteRecordByKey(Key, Data);
			break;
		case(UR_PSLT_MFD):
			// 여기서는 저장만 하고 UR_PSLT_MFS에서 처리한다.
			pViewBuff->GetPslt(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_PSLT_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetPslt(nKey, Key, Data);
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

void CCMPsltDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}

BEGIN_MESSAGE_MAP(CCMPsltDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMPsltDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE             , OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_PRES_ADD          , OnCmdPsltAdd)
	ON_BN_CLICKED(IDC_CMD_PRES_DELETE       , OnCmdPsltDelete)
	ON_BN_CLICKED(IDC_CMD_PRES_MODIFY       , OnCmdPsltModify)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN      , OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_PRES_ELTYPE_BTN   , OnCmdElemTypeImageBtn)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB1, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB2, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB3, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB4, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB5, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB6, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB7, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LCASE_CMB8, OnChangeLoadCase)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB1, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB2, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB3, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB4, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB5, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB6, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB7, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_LTYPE_CMB8, OnChangeLoadType)
	ON_CBN_SELCHANGE(IDC_CMD_PRES_ELTYPE_CMB, OnChangeETypeCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPsltDlg message handlers

BOOL CCMPsltDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	for (int i = 0; i < PSLT_NUM; i++)
	{
		m_cmbLCase[i].SetLoadType(D_LOADCASE_STATIC, FALSE);
		m_cmbLCase[i].SetInputDialogMode();
		m_cmbLCase[i].ChangeSelect(D_LOADCASE_STATIC, D_LOADCASE_NONE);
		
		m_cmbLType[i].AddString(_LS(IDS_CMD_ELEMTYPE_UNIFORM));  m_cmbLType[i].SetItemData(0,LTYPE_UNIFORM);
		m_cmbLType[i].AddString(_LS(IDS_CMD_ELEMTYPE_LINEAR ));  m_cmbLType[i].SetItemData(1,LTYPE_LINEAR);
		m_cmbLType[i].SetCurSel(0);

		m_wndLoadP1[i].SetUnitType(CUnitCtrl::m_PRES_UNIT.Force);
		m_wndLoadP2[i].SetUnitType(CUnitCtrl::m_PRES_UNIT.Force);
		m_wndLoadP3[i].SetUnitType(CUnitCtrl::m_PRES_UNIT.Force);
		m_wndLoadP4[i].SetUnitType(CUnitCtrl::m_PRES_UNIT.Force);
		m_wndUnit[i].SetUnitType(CUnitCtrl::m_PRES_UNIT.Force);

		m_cmbLType[i] .EnableWindow(FALSE);
		m_wndLoadP1[i].EnableWindow(FALSE);
		m_wndLoadP2[i].EnableWindow(FALSE);
		m_wndLoadP3[i].EnableWindow(FALSE);
		m_wndLoadP4[i].EnableWindow(FALSE);
		m_wndUnit[i].EnableWindow(FALSE);
	}

	m_cmbElemType.ResetContent();
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(0));
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(1));
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(2));
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(3));
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(4));
#ifndef _CIVIL
	m_cmbElemType.AddString(m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(5));
#endif
	m_cmbElemType.SetCurSel(0);

	m_WellButtonColor.SetColumnsNumber(10);
#ifdef _MGEN_CH

	m_WellButtonColor.EnableOtherButton(_LS(IDS_JUD_DispOptDlg_ColorButtonName_Others));
	
	CPalette palText;
	CColorTreeCtrl::CreatePalette(palText);
	m_WellButtonColor.SetPalette(&palText);
	//m_WellButtonColor.SetColor(m_pContourEngine->m_EdgeColor);
#else
	GetDlgItem(IDC_CMD_PRES_ELEMCOLOR_STATIC)->ShowWindow(FALSE);
	m_WellButtonColor.ShowWindow(FALSE);
#endif

	// Grid를 초기화 시킨다.
	m_wndGrid.Initialize(m_pDoc, this);
	m_wndGrid.CheckCurrentRecordChanged();
	if(m_StartPsltK>0) m_wndGrid.SelectPsltAndUpdateDlg(m_StartPsltK);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMPsltDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();	
}

void CCMPsltDlg::OnCmdPsltAdd() 
{
	// TODO: Add your control notification handler code here
	T_PSLT_K key;
	T_PSLT_D data;
	if (!GetPsltData(data)) return;
	if (m_pDoc->m_pDataCtrl->AddPslt(data))
	{ //새로 추가된 PSLT로 이동(grid에서)
		key = m_pDoc->m_pAttrCtrl2->GetPsltKey(data.strName);
		m_wndGrid.SetCurrentPslt(key, data);
	}
}

void CCMPsltDlg::OnCmdPsltDelete() 
{
	// TODO: Add your control notification handler code here
	T_PSLT_K key;
	T_PSLT_D data;
	if (!m_wndGrid.GetCurrentPslt(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_d));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl2->GetPslt(key, data));
	m_pDoc->m_pDataCtrl->DelPslt(data.strName);
}

void CCMPsltDlg::OnCmdPsltModify()
{
	// TODO: Add your control notification handler code here
	T_PSLT_K key;
	T_PSLT_D data;
	if (!m_wndGrid.GetCurrentPslt(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_m));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl2->GetPslt(key, data));
	CString csOldName = data.strName;
	unsigned int nOldTypeID = data.PsltId;
	if (!GetPsltData(data,TRUE)) return;
	data.PsltId = nOldTypeID;
	m_pDoc->m_pDataCtrl->ModifyPslt(csOldName, data);
}

void CCMPsltDlg::OnChangeLoadCase() 
{
	// TODO: Add your control notification handler code here

	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	UINT nEditID;
	switch(nID)
	{
	case IDC_CMD_PRES_LCASE_CMB1 : nEditID = 0; break;
	case IDC_CMD_PRES_LCASE_CMB2 : nEditID = 1; break;
	case IDC_CMD_PRES_LCASE_CMB3 : nEditID = 2; break;
	case IDC_CMD_PRES_LCASE_CMB4 : nEditID = 3; break;
	case IDC_CMD_PRES_LCASE_CMB5 : nEditID = 4; break;
	case IDC_CMD_PRES_LCASE_CMB6 : nEditID = 5; break;
	case IDC_CMD_PRES_LCASE_CMB7 : nEditID = 6; break;
	case IDC_CMD_PRES_LCASE_CMB8 : nEditID = 7; break;
	default : ASSERT(0); return;
	}
	UINT nLoadCaseType, nLoadCaseKey;
	CSelectLC* pSelectLC = (CSelectLC*)GetDlgItem(nID);
	BOOL bGet = pSelectLC->GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	if (!bGet || nLoadCaseKey == D_LOADCASE_NONE)
	{
		m_wndLoadP1[nEditID].SetWindowText(_T("0"));
		m_wndLoadP2[nEditID].SetWindowText(_T("0"));
		m_wndLoadP3[nEditID].SetWindowText(_T("0"));
		m_wndLoadP4[nEditID].SetWindowText(_T("0"));
	}

	EnableDisableCtrls();
}

void CCMPsltDlg::OnChangeLoadType() 
{
	// TODO: Add your control notification handler code here

	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	UINT nEditID;
	switch(nID)
	{
	case IDC_CMD_PRES_LTYPE_CMB1 : nEditID = 0; break;
	case IDC_CMD_PRES_LTYPE_CMB2 : nEditID = 1; break;
	case IDC_CMD_PRES_LTYPE_CMB3 : nEditID = 2; break;
	case IDC_CMD_PRES_LTYPE_CMB4 : nEditID = 3; break;
	case IDC_CMD_PRES_LTYPE_CMB5 : nEditID = 4; break;
	case IDC_CMD_PRES_LTYPE_CMB6 : nEditID = 5; break;
	case IDC_CMD_PRES_LTYPE_CMB7 : nEditID = 6; break;
	case IDC_CMD_PRES_LTYPE_CMB8 : nEditID = 7; break;
	default : ASSERT(0); return;
	}
	
	CComboBox* pLTypeCmb = (CComboBox*)GetDlgItem(nID);
	int ix = pLTypeCmb->GetCurSel();
	if (ix == CB_ERR) return;
	int nLType = pLTypeCmb->GetItemData(ix);
	if (nLType == LTYPE_UNIFORM)
	{
		m_wndLoadP2[nEditID].SetWindowText(_T("0"));
		m_wndLoadP3[nEditID].SetWindowText(_T("0"));
		m_wndLoadP4[nEditID].SetWindowText(_T("0"));
	}

	EnableDisableCtrls();
}

void CCMPsltDlg::OnCmdLoadcaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CCMPsltDlg::EnableDisableCtrls()
{
	int nEType = m_cmbElemType.GetCurSel();

	for(int i=0; i<PSLT_NUM; i++)
	{
		UINT nLoadCaseType=0, nLoadCaseKey=0;
		BOOL bGet = m_cmbLCase[i].GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
		int ix = m_cmbLType[i].GetCurSel();
		if (ix == CB_ERR) return;
		int nLType = m_cmbLType[i].GetItemData(ix);

		// Key가 None일 때
		if (!bGet || nLoadCaseKey == D_LOADCASE_NONE)
		{
			m_cmbLType [i].EnableWindow(FALSE);
			m_wndLoadP1[i].EnableWindow(FALSE);
			m_wndLoadP2[i].EnableWindow(FALSE);
			m_wndLoadP3[i].EnableWindow(FALSE);
			m_wndLoadP4[i].EnableWindow(FALSE);
			m_wndUnit  [i].EnableWindow(FALSE);
		}
		// Type이 Uniform일 때
		else if(nLType == LTYPE_UNIFORM)
		{
			m_cmbLType [i].EnableWindow(TRUE);
			m_wndLoadP1[i].EnableWindow(TRUE);
			m_wndLoadP2[i].EnableWindow(FALSE);
			m_wndLoadP3[i].EnableWindow(FALSE);
			m_wndLoadP4[i].EnableWindow(FALSE);
			m_wndUnit  [i].EnableWindow(TRUE);
		}    
		else
		{
			// Element Type이 Face일 때
			if(nEType==0 || nEType==2)
			{
				m_cmbLType [i].EnableWindow(TRUE);
				m_wndLoadP1[i].EnableWindow(TRUE);
				m_wndLoadP2[i].EnableWindow(TRUE);
				m_wndLoadP3[i].EnableWindow(TRUE);
				m_wndLoadP4[i].EnableWindow(TRUE);
				m_wndUnit  [i].EnableWindow(TRUE);
			}
			// Element Type이 Edge일 때
			else
			{
				m_cmbLType [i].EnableWindow(TRUE);
				m_wndLoadP1[i].EnableWindow(TRUE);
				m_wndLoadP2[i].EnableWindow(TRUE);
				m_wndLoadP3[i].EnableWindow(FALSE);
				m_wndLoadP4[i].EnableWindow(FALSE);
				m_wndUnit  [i].EnableWindow(TRUE);
			}
		}
	}

#if defined (_MGEN_CH)
	if(nEType == 0)
	{
		GetDlgItem(IDC_CMD_PRES_ELEMCOLOR_STATIC)->EnableWindow(TRUE);
		m_WellButtonColor.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_PRES_ELEMCOLOR_STATIC)->EnableWindow(FALSE);
		m_WellButtonColor.EnableWindow(FALSE);
	}
#endif
}

void CCMPsltDlg::OnCmdElemTypeImageBtn()
{
	CCMPsltHelpDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetElemType(m_cmbElemType.GetCurSel());
	dlg.DoModal();
}

void CCMPsltDlg::SetStartPsltK(T_PSLT_K PsltK)
{
	m_StartPsltK = PsltK;
}

void CCMPsltDlg::OnChangeETypeCmb()
{
	SetPresUnit();
	EnableDisableCtrls();
}

void CCMPsltDlg::SetPresUnit()
{
	int nEType = m_cmbElemType.GetCurSel();
	int nUnitType = 0;
	if(nEType==0 || nEType==2)
		nUnitType = CUnitCtrl::m_PSLT_UNIT.Pressure;
	else
		nUnitType = CUnitCtrl::m_PSLT_UNIT.UnitLoad;

	for(int i=0; i<PSLT_NUM; i++)
	{
		m_wndLoadP1[i].SetUnitType(nUnitType);
		m_wndLoadP2[i].SetUnitType(nUnitType);
		m_wndLoadP3[i].SetUnitType(nUnitType);
		m_wndLoadP4[i].SetUnitType(nUnitType);
		m_wndUnit[i].SetUnitType(nUnitType);
	}
}