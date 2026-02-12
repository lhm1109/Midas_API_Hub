#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageAdditionalDefDlg.h"
#include "CMStageAdditionalBuckDlg.h"
#include "CMStageItemBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "../wg_db/wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalDefDlg

CCMStageAdditionalDefDlg::CCMStageAdditionalDefDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageAdditionalDefDlg::IDD, pParent)
{
	m_bInitAdd = TRUE;
	m_CspaKey = 0;
	m_SelectedStageKey = 0;
	m_bBucklingChk = FALSE;
}

CCMStageAdditionalDefDlg::~CCMStageAdditionalDefDlg()
{
}

void CCMStageAdditionalDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_LOADCASE2, m_cmbStage);
	//DDX_Control(pDX, IDC_CMD_LOADCASE3, m_LoadCase);
	DDX_Control(pDX, IDC_CMD_LOADCASE3, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_LIST2, m_List);
	//DDX_Check(pDX, IDC_CMD_BUCK_STIFFOPT_CHK, m_bBucklingChk);
	DDX_Control(pDX, IDC_CMD_BUCK_STIFFOPT_CHK, m_bBucklingBtn);
}

BEGIN_MESSAGE_MAP(CCMStageAdditionalDefDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageAdditionalDefDlg)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCASE2, OnCmdSelectChangeStage)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnLdAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnLdDel)
	ON_BN_CLICKED(IDC_CMD_BUCK_STIFFOPT_CHK, OnCmdBucklingChk)
	ON_BN_CLICKED(IDC_CMD_BTN_LD_ADD, OnCmdBucklingCtrl)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMStageAdditionalDefDlg::SetListCtrlHeader()
{
	// IDS_CMD_BUCK_loadcase 바꿀 것.
	CString aTitle[] = { _LS(IDS_CMD_BUCK_loadcase) };

	CRect rtList;
	m_List.GetWindowRect(rtList);
	double dWidth = (rtList.Width() * 1.0) - globalUtils.GetSystemMetricsForDPI(&m_List, SM_CXVSCROLL) - 1.0;
	int nColWidth[] = { dWidth * 1.0 };
	int nColNum = 1;

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for (i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMStageAdditionalDefDlg::GetSelectedItem(int& nIndex)
{
	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMStageAdditionalDefDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	// 현재는 하나로 가정하고....
	if(m_CspaData.aSTLoad.GetSize() == 0) return;
	int nItemCount = m_CspaData.aSTLoad[0].aStldKeys.GetSize();
	if (nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_CspaData.aSTLoad[0].aStldKeys[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMStageAdditionalDefDlg::SetItem(int nIndex, T_STLD_K& stlKey)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nIndex;
	lvitem.iSubItem = 0;

	T_STLD_D data;
	m_pDoc->m_pAttrCtrl->GetStld(stlKey, data);
	str = data.LoadCaseName;

	lvitem.pszText = str.GetBuffer(0);
	lvitem.mask = LVIF_TEXT;

	m_List.SetItem(&lvitem);
	str.ReleaseBuffer();
}

BOOL CCMStageAdditionalDefDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitializeStageCombo();

	if (m_CspaKey == 0)
	{
		m_CspaData.Initialize();
		if (m_cmbStage.GetCount() > 0)
		{
			m_cmbStage.SetCurSel(0);
			m_SelectedStageKey = (T_STAG_K)m_cmbStage.GetItemData(0);
		}
	}
	else
	{
		if (m_pDoc->m_pAttrCtrl2->GetCspa(m_CspaKey, m_CspaData))
		{
			m_SelectedStageKey = m_CspaKey;
			for (int i = 0; i < m_cmbStage.GetCount(); i++)
			{
				if ((T_STAG_K)m_cmbStage.GetItemData(i) == m_SelectedStageKey)
				{
					m_cmbStage.SetCurSel(i);
					break;

					//// Stage 선택 확인
					//int nSel = m_cmbStage.GetCurSel();
					//if (nSel == CB_ERR) return;

					//T_STAG_K StagKey = (T_STAG_K)m_cmbStage.GetItemData(nSel);
					//if (StagKey == 0) return;
				}
			}
		}
		else
		{
			ASSERT(FALSE);
			m_CspaData.Initialize();
		}
	}

	//InitializeLoadCaseCombo();

	Data2Dlg(m_CspaData);

	CtrlMan();

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageAdditionalDefDlg::CtrlMan()
{

	if (!CCMStageItemBase::IsCurStageBase())
	{
		CArray<UINT, UINT> Ctrls;
		Ctrls.Add(IDC_CMD_APPLY);
		CDlgUtil::CtrlShowHide(this, Ctrls, FALSE);

		Ctrls.RemoveAll();
		Ctrls.Add(IDC_CMD_BUCK_STIFFOPT_CHK);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	}
	else
	{
		bool check = m_bBucklingBtn.GetCheck();
		CDlgUtil::CtrlEnableDisable(this,IDC_CMD_BTN_LD_ADD,check);
	}
}


void CCMStageAdditionalDefDlg::InitializeStageCombo()
{
	m_cmbStage.ResetContent();

	BOOL m_bHasFinal = FALSE;
	T_STAG_K m_iFinalKey = 0;
	T_STCT_D StctD;
	if (!m_pDoc->m_pAttrCtrl->GetStctForAnalysis(StctD)) StctD.Initialize();
	if (StctD.nFinalStage == 1)
	{
		m_bHasFinal = TRUE;
		m_iFinalKey = StctD.FinalStagKey;
	}

	CArray<T_STAG_K, T_STAG_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetStagKeyList(rKeyList);

	// final 을 별도 정의한 경우 final 보다 앞의 stage 만 표시해야 하나?
	// 아니면 정의는 가능하도록 하고	 DB 생성 시점에 제외해야 하나?

	for (int i = 0; i < rKeyList.GetSize(); i++)
	{
		T_STAG_D StagD;
		if (m_pDoc->m_pAttrCtrl->GetStag(rKeyList[i], StagD))
		{
			int nIndex = m_cmbStage.AddString(StagD.StageName);
			m_cmbStage.SetItemData(nIndex, (DWORD_PTR)rKeyList[i]);
		}

		if (m_bHasFinal)
		{
			if (rKeyList[i] == m_iFinalKey) break;
		}
	}
}

void CCMStageAdditionalDefDlg::InitializeLoadCaseCombo()
{
	m_cmbLoadCase.ResetContent();


}

void CCMStageAdditionalDefDlg::OnCmdSelectChangeStage()
{
	UpdateData(TRUE);

	//InitializeLoadCaseCombo();

	int nSel = m_cmbStage.GetCurSel();
	if (nSel == CB_ERR)
	{
		ASSERT(FALSE);
		return;
	}
	m_SelectedStageKey = (T_STAG_K)m_cmbStage.GetItemData(nSel);
}


BOOL CCMStageAdditionalDefDlg::Dlg2Data(T_CSPA_D& data)
{
	m_bBucklingChk = m_bBucklingBtn.GetCheck();

	//if (m_bBucklingChk == TRUE)
	//{
	//	data.bBuckling = TRUE;
	//}

	// 일단 하나만....
	if(m_aSelectedLoadCases.GetSize() > 0)
		data.aSTLoad[0].aStldKeys.Copy(m_aSelectedLoadCases);

	return TRUE;
}

BOOL CCMStageAdditionalDefDlg::Data2Dlg(T_CSPA_D& data)
{
	m_bBucklingChk = data.aBuckData.GetSize();
	m_bBucklingBtn.SetCheck(m_bBucklingChk);

	// 일단 하나만....
	if(data.aSTLoad.GetSize() > 0)
		m_aSelectedLoadCases.Copy(data.aSTLoad[0].aStldKeys);

	//m_LoadCase.SetLoadType(D_LOADCASE_STATIC);

	T_STLD_D DataStld;
	CArray<T_STLD_K, T_STLD_K> KeyList;
	if (m_pDoc->IsPostMode())
	{
		m_pDoc->m_pPostCtrl->GetValidStldKeyList(KeyList);

		for (int i = KeyList.GetSize()-1; i >=0 ; i--)
		{
			m_pDoc->m_pPostCtrl->GetStld(KeyList[i], DataStld);
			if(DataStld.LoadCaseType != _T("TCS"))
			{
				KeyList.RemoveAt(i);
			}
		}

		for (int i = 0; i < KeyList.GetSize(); i++)
		{
			m_pDoc->m_pPostCtrl->GetStld(KeyList[i], DataStld);
			CString strName = DataStld.LoadCaseName;
			int nIndex = m_cmbLoadCase.AddString(strName);
			m_cmbLoadCase.SetItemData(nIndex, m_aLoadCaseKeys.GetSize());
			m_aLoadCaseKeys.Add(KeyList[i]);
		}
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);

		for (int i = KeyList.GetSize() - 1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetStld(KeyList[i], DataStld);
			if (DataStld.LoadCaseType != _T("TCS"))
			{
				KeyList.RemoveAt(i);
			}
		}

		for (int i = 0; i < KeyList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(KeyList[i], DataStld);
			CString strName = DataStld.LoadCaseName;
			int nIndex = m_cmbLoadCase.AddString(strName);
			m_cmbLoadCase.SetItemData(nIndex, m_aLoadCaseKeys.GetSize());
			m_aLoadCaseKeys.Add(KeyList[i]);
		}

	}

	m_cmbLoadCase.SetCurSel(0);

	UpdateData(TRUE);
	return TRUE;
}

void CCMStageAdditionalDefDlg::OnCmdBucklingChk()
{
	m_bBucklingChk = m_bBucklingBtn.GetCheck();
	if (m_bBucklingChk)
	{
		CDlgUtil::CtrlEnableDisable(this,IDC_CMD_BTN_LD_ADD,m_bBucklingChk);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this,IDC_CMD_BTN_LD_ADD,FALSE);
	}
}

void CCMStageAdditionalDefDlg::OnCmdBucklingCtrl()
{
	CCMStageAdditionalBuckDlg dlg;

	if (m_CspaData.aBuckData.GetSize() == 0)
	{
		//dlg.m_bBucklingChk = FALSE;
		dlg.m_bInitAdd = TRUE;
	}
	else
	{
		//dlg.m_bBucklingChk = TRUE;
		dlg.m_bInitAdd = FALSE;

		dlg.m_Data = m_CspaData.aBuckData[0];	// 일단 하나로 가정
	}

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_bBucklingChk)
		{
			m_bBucklingChk = TRUE;
			if (m_CspaData.aBuckData.GetSize() == 0)	// 일단 하나로 가정
				m_CspaData.aBuckData.SetSize(1);

			m_CspaData.aBuckData[0] = dlg.m_Data;  // 일단 하나로 가정
		}
		else  // remove 된 경우
		{
			m_bBucklingChk = FALSE;
			m_CspaData.aBuckData.RemoveAll();
		}

		m_bBucklingBtn.SetCheck(m_bBucklingChk);
	}
}

BOOL CCMStageAdditionalDefDlg::GetSelectedLoadCaseKey(T_STLD_K& outKey)
{
	int nSel = m_cmbLoadCase.GetCurSel();
	if (nSel == CB_ERR)
		return FALSE;

	DWORD_PTR nIndex = m_cmbLoadCase.GetItemData(nSel);
	if (nIndex < 0 || nIndex >= (DWORD_PTR)m_aLoadCaseKeys.GetSize())
		return FALSE;

	outKey = m_aLoadCaseKeys[nIndex];
	return TRUE;
}

void CCMStageAdditionalDefDlg::OnCmdBtnLdAdd()
{
	T_STLD_K StldKey;
	if (!GetSelectedLoadCaseKey(StldKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return;
	}

	// 일단 하나만....
	m_aSelectedLoadCases.Add(StldKey);
	m_CspaData.aSTLoad[0].aStldKeys.Copy(m_aSelectedLoadCases);

	// 새로 추가된 항목 설정
	int nIndex = m_List.GetItemCount();
	m_List.InsertItem(nIndex, _T(""));
	SetItem(nIndex, StldKey);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMStageAdditionalDefDlg::OnCmdBtnLdDel()
{
	int nIndex = -1;
	if (!GetSelectedItem(nIndex))
	{
		//AfxMessageBox(_T("select case to delete"));
		ASSERT(FALSE);
		return;
	}

	m_aSelectedLoadCases.RemoveAt(nIndex);
	if (m_aSelectedLoadCases.GetSize() > 0)
	{
		m_CspaData.aSTLoad[0].aStldKeys.Copy(m_aSelectedLoadCases);
	}
	else
	{
		m_CspaData.aSTLoad[0].aStldKeys.RemoveAll();
	}

	MakeItemEx();

	// 선택 상태 유지
	if (m_List.GetItemCount() > 0)
	{
		int nNewSel = (nIndex < m_List.GetItemCount()) ? nIndex : nIndex - 1;
		if (nNewSel >= 0)
		{
			m_List.SetItemState(nNewSel, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CCMStageAdditionalDefDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMStageAdditionalDefDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bNeedUpdate = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		int nSize = m_CspaData.aSTLoad.GetSize();

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			// do nothing
			break;
		case(UR_STLD_DEL):
			// 현재 loadcase list에서 지워진 것 삭제한다.
			pViewBuff->GetStld(nKey, Key, Data);

			for (int i = m_aSelectedLoadCases.GetSize() - 1; i >= 0; i--)
			{
				if (m_aSelectedLoadCases[i] == Key)
				{
					m_aSelectedLoadCases.RemoveAt(i);
					bNeedUpdate = TRUE;
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
			// KeyBak/DataBak : MFD에서 가져온 값
			for (int i = m_aSelectedLoadCases.GetSize() - 1; i >= 0; i--)
			{
				if (m_aSelectedLoadCases[i] == Key)
				{
					m_aSelectedLoadCases[i] = KeyBak;
					bNeedUpdate = TRUE;
				}
			}

			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}

		if (bNeedUpdate)
		{
			if(m_aSelectedLoadCases.GetSize() == 0)
			{
				m_CspaData.aSTLoad[0].aStldKeys.RemoveAll();
			}
			else
			{
				m_CspaData.aSTLoad[0].aStldKeys.Copy(m_aSelectedLoadCases);
			}
			MakeItemEx();
		}
	} // end of while
}

void CCMStageAdditionalDefDlg::OnCmdApply()
{
	int nSel = m_cmbStage.GetCurSel();
	if (nSel == CB_ERR)
	{
		ASSERT(FALSE);
		return;
	}
	m_SelectedStageKey = (T_STAG_K)m_cmbStage.GetItemData(nSel);

	Dlg2Data(m_CspaData);

	if (m_CspaData.aSTLoad.GetSize() == 0 && m_CspaData.aBuckData.GetSize() == 0)
	{
		AfxMessageBox(_T("Additional Analysis is not defined."));
		ASSERT(FALSE);
		return;
	}

	T_CSPA_K UseKey = (m_CspaKey == 0) ? m_SelectedStageKey : m_CspaKey;

	BOOL bExist = FALSE;
	BOOL bHasSameData = FALSE;
	T_CSPA_D CspaOld;

	if(m_CspaKey == 0)	// add mode
	{
		// add 일 때는 기존 데이터 존재 여부만 확인
		if (m_pDoc->m_pAttrCtrl2->GetCspa(UseKey, CspaOld))
		{
			AfxMessageBox(_T("already exists for the selected stage."));
			ASSERT(FALSE);
			return;
		}
	}

	if (m_pDoc->m_pAttrCtrl2->GetCspa(UseKey, CspaOld))
	{
		bExist = TRUE;
		bHasSameData = IsExistCspa(CspaOld, m_CspaData);
		if (bHasSameData)
		{
			AfxMessageBox(_T("already exists for the selected stage."));
			ASSERT(FALSE);
			return;
		}
	}

	UpdateData(TRUE);

	if (!bExist)
	{
		if (m_pDoc->m_pDataCtrl->AddCspa(UseKey, m_CspaData))
		{
			CDialogMove::OnOK();
		}
	}
	else
	{
		if (!m_pDoc->m_pDataCtrl->DelCspa(UseKey))
		{
			ASSERT(FALSE);
			return;
		}
		if (m_pDoc->m_pDataCtrl->AddCspa(UseKey, m_CspaData))
		{
			CDialogMove::OnOK();
		}

		//if (m_pDoc->m_pDataCtrl->ModifyCspa(UseKey, m_CspaData))
		//{
		//	CDialogMove::OnOK();
		//}
	}
}

void CCMStageAdditionalDefDlg::OnCmdClose()
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

BOOL CCMStageAdditionalDefDlg::IsExistCspa(T_CSPA_D& CspaOld, T_CSPA_D& CspaNew)
{
	// Loadcase 개수 비교
	if (CspaOld.aSTLoad.GetSize() != CspaNew.aSTLoad.GetSize())
	{
		return FALSE;
	}

	for (int i = 0; i < CspaOld.aSTLoad.GetSize(); i++)
	{
		if (CspaOld.aSTLoad[i].aStldKeys.GetSize() != CspaNew.aSTLoad[i].aStldKeys.GetSize())
		{
			return FALSE;
		}

		for (int j = 0; j < CspaOld.aSTLoad[i].aStldKeys.GetSize(); j++)
		{
			for (int k = 0; k < CspaNew.aSTLoad[i].aStldKeys.GetSize(); k++)
			{
				if (CspaOld.aSTLoad[i].aStldKeys[j] != CspaNew.aSTLoad[i].aStldKeys[k])
				{
					return FALSE;
				}
			}
		}
	}

	// Buckling 옵션 비교
	if (CspaOld.aBuckData.GetSize() != CspaNew.aBuckData.GetSize())
	{
		return FALSE;
	}
	for (int i=0; i < CspaOld.aBuckData.GetSize(); i++)
	{
		if (CspaOld.aBuckData[i] != CspaNew.aBuckData[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}