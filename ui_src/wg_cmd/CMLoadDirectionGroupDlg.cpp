// CMLoadDirectionGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "CMLoadDirectionGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLSIZE   21
/////////////////////////////////////////////////////////////////////////////
// CCMLoadDirectionGroupDlg dialog


CCMLoadDirectionGroupDlg::CCMLoadDirectionGroupDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadDirectionGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadDirectionGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMLoadDirectionGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadDirectionGroupDlg)
	DDX_Control(pDX, IDC_CMD_GRUP_LCASE_CMB, m_ComboLoadCase);
	DDX_Control(pDX, IDC_CMD_GRUP_ANGLE_CMB, m_ComboAngle);
	DDX_Control(pDX, IDC_CMD_GRUP_SYSTEM_CMB, m_ComboSystem);
	DDX_Control(pDX, IDC_CMD_GRUP_NAME_EDT, m_EditUserGroup);
	DDX_Control(pDX, IDC_CMD_GRUP_ELEM_EDT, m_EditElemList);
	DDX_Control(pDX, IDC_CMD_GRID_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadDirectionGroupDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLoadDirectionGroupDlg)
	ON_BN_CLICKED(IDC_CMD_GRUP_ADD, OnCmdGrupAdd)
	ON_BN_CLICKED(IDC_CMD_GRUP_CLOSE, OnCmdGrupClose)
	ON_BN_CLICKED(IDC_CMD_GRUP_DELETE, OnCmdGrupDelete)
	ON_BN_CLICKED(IDC_CMD_GRUP_REPLACE, OnCmdGrupReplace)
	ON_CBN_SELCHANGE(IDC_CMD_GRUP_SYSTEM_CMB, OnSelchangeSystemType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadDirectionGroupDlg message handlers

BOOL CCMLoadDirectionGroupDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	m_ComboLoadCase.AddStaticIncludeType(_T("E"), FALSE);
	m_ComboLoadCase.SetLoadType(D_SELECTLOAD_STATIC | D_SELECTLOAD_SPECTRUM, TRUE);
	
	m_ComboAngle.ResetContent();
	m_ComboAngle.AddString(_T("0"));
	m_ComboAngle.AddString(_T("30"));
	m_ComboAngle.AddString(_T("45"));
	m_ComboAngle.AddString(_T("60"));
	m_ComboAngle.AddString(_T("90"));
	m_ComboAngle.AddString(_T("120"));
	m_ComboAngle.SetCurSel(0);

	CArray<CString, CString&> aSystemGroup;
	m_pDoc->m_pAttrCtrl->GetEglcSystemGroupList(aSystemGroup);
	for (int i=0; i<aSystemGroup.GetSize(); i++)
	{
		m_ComboSystem.AddString(aSystemGroup[i]);
	}
	m_ComboSystem.SetCurSel(0);
	GetDlgItem(IDC_CMD_GRUP_NAME_EDT)->EnableWindow(FALSE);

	m_wndGrid.Initialize(m_pDoc, this);
//   m_wndGrid.CheckCurrentRecordChanged();

	m_EditElemList.Connect(SC_ID_ELEM, &m_EditElemList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadDirectionGroupDlg::InitCtrlSetting()
{
//   CString SelectElemString = _T("");
//   m_EditElemList.SelectByStr(SelectElemString);

	m_ComboLoadCase.SetCurSel(0);
	CString strAngle = _T("0");
	m_ComboAngle.SetWindowText(strAngle);
	m_ComboSystem.SetCurSel(0);
	m_EditUserGroup.SetWindowText(_T(""));
	GetDlgItem(IDC_CMD_GRUP_NAME_EDT)->EnableWindow(FALSE);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CCMLoadDirectionGroupDlg::Data2Dlg(T_EGLD_D& EgldD)
{
	m_ComboLoadCase.ChangeSelect(EgldD.nLoadCaseType, EgldD.StldKey);
	CString strAngle;
	strAngle.Format(_T("%g"), EgldD.dAngle);
	m_ComboAngle.SetWindowText(strAngle);
	m_ComboSystem.SetCurSel(EgldD.nSystemGrupType);
	if(EgldD.nSystemGrupType == EN_USER_GRUP) m_EditUserGroup.SetWindowText(EgldD.strUserGroup);
	else m_EditUserGroup.SetWindowText(_T(""));

	CString SelectElemString;
	SelectElemString = _T("");

	int nElemSize = EgldD.arKeyElem.GetSize();
	CString TempString;
	for(int i=0 ; i<nElemSize ; i++)
	{
		if(SelectElemString.GetLength()==0)
			SelectElemString.Format(_T("%d"), EgldD.arKeyElem[i]);
		else
		{
			TempString.Format(_T(" %d"), EgldD.arKeyElem[i]);
			SelectElemString += TempString;
		}

	}
	m_EditElemList.SelectByStr(SelectElemString);
	return TRUE;
}

BOOL CCMLoadDirectionGroupDlg::Dlg2Data(T_EGLD_D& EgldD)
{
	m_ComboLoadCase.GetSelectedLoad(EgldD.nLoadCaseType, EgldD.StldKey);
	EgldD.nSystemGrupType = m_ComboSystem.GetCurSel();
	if(EgldD.nSystemGrupType == EN_USER_GRUP)
	{
		m_EditUserGroup.GetWindowText(EgldD.strUserGroup);

		if(EgldD.strUserGroup.IsEmpty())
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_User_Group_Name__));
			return FALSE;
		}
	}
	else EgldD.strUserGroup.Empty();

	CString strAngle;
	m_ComboAngle.GetWindowText(strAngle);
	EgldD.dAngle = _ttof(strAngle);

	CString strElemNumber;
	m_EditElemList.GetWindowText(strElemNumber);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CArray<long, long> arElemList;

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

	CNumericOptimizer optimizer;
	optimizer.SelectNumByStrBase((long*)arElemKeyList.GetData(), (long)arElemKeyList.GetSize(),
		strElemNumber, arElemList);

	if(arElemList.GetSize() <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_element_));
		return FALSE;
	}

	EgldD.arKeyElem.RemoveAll();
	for(int i=0; i<arElemList.GetSize(); i++)
	{
		EgldD.arKeyElem.Add((UINT)arElemList[i]);
	}
	return TRUE;
}
void CCMLoadDirectionGroupDlg::OnChangeCurrentEGLD(T_EGLD_K nKey)
{
	T_EGLD_K key;
	T_EGLD_D data;
	if (nKey == 0)
		data.Initialize();
	else
	{
		if (!m_wndGrid.GetCurrentEGLD(key))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__No_selected_EGLD));
			return;
			data.Initialize();
		}
		if (!m_pDoc->m_pAttrCtrl->GetEgld(key, data))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Can_t_read_EGLD));
			return;
		}
	}
	
	Data2Dlg(data);
	OnSelchangeSystemType();
}

void CCMLoadDirectionGroupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		//    UnitChanged();
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

void CCMLoadDirectionGroupDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_EGLD_K cKey;
	m_wndGrid.GetCurrentCell(ncRow, ncCol);
	m_wndGrid.GetColumnNoFor(m_wndGrid.GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid.GetValueRowCol(ncRow, nkCol));

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_EGLD_K Key;
	T_EGLD_D Data, DataBak;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_EGLD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetEgld(nKey, Key, Data);
				m_wndGrid.InsertRecordByKey(Key, Data);
				break;
		case(UR_EGLD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetEgld(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
// 				m_wndGrid.GetCurrentCell(ncRow, ncCol);
// 				m_wndGrid.OnChangeCurrentRecord(ncRow);
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
//   if (m_wndGrid.GetRowCount() < ncRow) ncRow = m_wndGrid.GetRowCount();
//   m_wndGrid.SetCurrentCell(ncRow, ncCol);

	// [MQC 22341] Add/Modify/Delete 후에 선택 해제된 상태로 개선 가능
	InitCtrlSetting();

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMLoadDirectionGroupDlg::GetEgldData(T_EGLD_D& data)
{
	data.Initialize();
	return Dlg2Data(data);
}

void CCMLoadDirectionGroupDlg::OnCmdGrupAdd() 
{
	T_EGLD_K key;
	T_EGLD_D data;

	if (!GetEgldData(data)) return;

	key = m_pDoc->m_pAttrCtrl->GetStartNumEgld();
	m_pDoc->m_pDataCtrl->AddEgld(key, data);

	IUsageCounter::Use(_T("SPE_EGLD"));

	//Dlg2Data(data);
}

void CCMLoadDirectionGroupDlg::OnCmdGrupDelete() 
{
	T_EGLD_K key;
	T_EGLD_D data;

	if (!m_wndGrid.GetCurrentEGLD(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_EGLD_is_selected_to_be_d));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetEgld(key, data));
	m_pDoc->m_pDataCtrl->DelEgld(key);
}

void CCMLoadDirectionGroupDlg::OnCmdGrupReplace() 
{
	T_EGLD_K key;
	T_EGLD_D data;
	if (!m_wndGrid.GetCurrentEGLD(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_EGLD_is_selected_to_be_m));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetEgld(key, data));

	if (!GetEgldData(data)) return;
	m_pDoc->m_pDataCtrl->ModifyEgld(key, data);

	IUsageCounter::Use(_T("SPE_EGLD"));
}

void CCMLoadDirectionGroupDlg::OnCmdGrupClose() 
{
	DestroyWindow();
}

void CCMLoadDirectionGroupDlg::OnSelchangeSystemType()
{
	int nCurSel = m_ComboSystem.GetCurSel();
	GetDlgItem(IDC_CMD_GRUP_NAME_EDT)->EnableWindow(nCurSel==EN_USER_GRUP);
}