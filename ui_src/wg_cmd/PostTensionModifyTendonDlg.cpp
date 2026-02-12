// PostTensionModifyTendonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PostTensionModifyTendonDlg.h"
#include "PostTensionModifyTendonVerDlg.h"
#include "PostTensionModifyTendonHorDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonDlg dialog

CPostTensionModifyTendonDlg::CPostTensionModifyTendonDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CPostTensionModifyTendonDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nActivePage = 0;
	m_pVerPage = NULL;
	m_pHorPage = NULL;

	m_SeclectedTendonKey = 0;
}

CPostTensionModifyTendonDlg::~CPostTensionModifyTendonDlg()
{
}

void CPostTensionModifyTendonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_PROFILE_TAB, m_wndTab);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_LIST, m_wndTendonList);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_NAME_EDT, m_edtTendonName);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_PROPERTY_CMB, m_cmbTendonProperty);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_STRESSING_CMB2, m_cmbTendonStressing);
}

BEGIN_MESSAGE_MAP(CPostTensionModifyTendonDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_CMD_APPLY_BTN, OnApplyBtn)
	ON_BN_CLICKED(IDC_CMD_DEL_BTN, OnDeleteBtn)
	ON_NOTIFY(NM_CLICK, IDC_CMD_PT_MODIFY_TENDON_LIST, OnItemClickTendonList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonDlg message handlers

BOOL CPostTensionModifyTendonDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	// Initialize Tendon List
	InitTendonList();
	UpdateTendonList();

	// Initialize Combo Boxes
	UpdateTendonPropertyList();
	UpdateTendonStressingList();

	LayoutChildDlg();

	if(m_SeclectedTendonKey > 0)
	{
		UpdateControlsByPttnData(m_SeclectedTendonKey);
	}
	else
	{
		// 첫 번째 아이템이 선택된 경우 해당 데이터로 컨트롤 업데이트
		int nSelectedKey = GetSelectedTendonKey();
		if (nSelectedKey > 0)
		{
			UpdateControlsByPttnData(static_cast<T_PTTN_K>(nSelectedKey));
		}
	}

	return TRUE;
}

void CPostTensionModifyTendonDlg::SetPttnK(T_PTTN_K PttnKey)
{
	m_SeclectedTendonKey = PttnKey;
}

void CPostTensionModifyTendonDlg::LayoutChildDlg()
{
	// 1. Placeholder 설정
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PT_MODIFY_TENDON_PLACEHOLDER);
	ASSERT(pPlaceHolder);

	// Placeholder를 Tab 컨트롤 뒤로 이동 (Z-order)
	pPlaceHolder->SetWindowPos(&m_wndTab, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_wndTab.SetPlaceHolder(pPlaceHolder);

	// 2. Tab Page 인스턴스 생성
	m_pVerPage = new CPostTensionModifyTendonVerDlg(m_pDoc, this);
	m_pHorPage = new CPostTensionModifyTendonHorDlg(m_pDoc, this);

	// 3. Tab에 Page 추가
	m_wndTab.AddTab(m_pVerPage, _T("Vertical"), CPostTensionModifyTendonVerDlg::IDD, TRUE);
	m_wndTab.AddTab(m_pHorPage, _T("Horizontal"), CPostTensionModifyTendonHorDlg::IDD, TRUE);

	// 4. 초기 Tab 표시
	m_wndTab.ShowTab(m_nActivePage);
}

void CPostTensionModifyTendonDlg::SetActivePage(int nPage)
{
	m_wndTab.ShowTab(nPage);
	m_nActivePage = nPage;
}

void CPostTensionModifyTendonDlg::OnClose()
{
	CDialogMove::OnCancel();
}

void CPostTensionModifyTendonDlg::InitTendonList()
{
	if (m_wndTendonList.GetSafeHwnd())
	{
		CStringArray HTitles;
		CArray<float, float> HRatio;

		HTitles.Add(_T("ID"));		HRatio.Add(0.3f);
		HTitles.Add(_T("Name"));	HRatio.Add(0.7f);

		CDlgUtil::_SetListCtrlHeader(&m_wndTendonList, HTitles, &HRatio, NULL);
		m_wndTendonList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	}
}

void CPostTensionModifyTendonDlg::UpdateTendonList()
{
	if (!m_wndTendonList.GetSafeHwnd()) return;

	// 기존 리스트 클리어
	m_wndTendonList.DeleteAllItems();

	// PTTN 키 리스트 가져오기
	CArray<T_PTTN_K, T_PTTN_K> aPttnK;
	m_pDoc->m_pAttrCtrl2->GetPttnKeyList(aPttnK);

	T_PTTN_D PttnD;
	for (int i = 0; i < aPttnK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetPttn(aPttnK[i], PttnD))
		{
			ASSERT(0); continue;
		}

		T_PTTN_K nKey = aPttnK[i];
		CString strName = PttnD.strName;

		// ID 컬럼 (첫 번째 컬럼)
		CString strID;
		strID.Format(_T("%d"), nKey);
		int nItem = m_wndTendonList.InsertItem(i, strID);

		// Name 컬럼 (두 번째 컬럼)
		m_wndTendonList.SetItemText(nItem, 1, strName);

		// 키 값을 ItemData에 저장 (나중에 선택 시 사용)
		m_wndTendonList.SetItemData(nItem, static_cast<DWORD_PTR>(nKey));
	}

	// 첫 번째 아이템 선택
	if (m_wndTendonList.GetItemCount() > 0)
	{
		m_wndTendonList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CPostTensionModifyTendonDlg::UpdateTendonPropertyList()
{
	m_cmbTendonProperty.ResetContent();

	CArray<T_TDNT_K, T_TDNT_K> aTdntK;
	m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
	T_TDNT_D TdntD;
	for (auto i = 0; i < aTdntK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetTdnt(aTdntK[i], TdntD))
		{
			ASSERT(0); return;
		}
		m_cmbTendonProperty.AddString(TdntD.TendonTypeName);
	}

	m_cmbTendonProperty.SetCurSel(0);
}

void CPostTensionModifyTendonDlg::UpdateTendonStressingList()
{
	m_cmbTendonStressing.ResetContent();

	CArray<T_TDST_K, T_TDST_K> aTdstK;
	m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
	T_TDST_D TdstD;
	for (auto i = 0; i < aTdstK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
		{
			ASSERT(0); return;
		}
		m_cmbTendonStressing.AddString(TdstD.strName);
	}

	m_cmbTendonStressing.SetCurSel(0);
}

void CPostTensionModifyTendonDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

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
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CPostTensionModifyTendonDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch (nCmd)
		{
		case(UR_TDNT_ADD):
		case(UR_TDNT_DEL):
		case(UR_TDNT_MFD):
		case(UR_TDNT_MFS):
			UpdateTendonPropertyList();
			break;
		case(UR_TDPF_ADD):
		case(UR_TDPF_DEL):
		case(UR_TDPF_MFD):
		case(UR_TDPF_MFS):
			// Tendon Profile - VerDlg에서 처리
			if (m_pVerPage) m_pVerPage->UpdateTendonProfileList();
			break;
		case(UR_TDST_ADD):
		case(UR_TDST_DEL):
		case(UR_TDST_MFD):
		case(UR_TDST_MFS):
			UpdateTendonStressingList();
			break;
		case(UR_PTTN_ADD):
		case(UR_PTTN_DEL):
		case(UR_PTTN_MFD):
		case(UR_PTTN_MFS):
			UpdateTendonList();
			break;
		default:
			break;
		}
	}
}

void CPostTensionModifyTendonDlg::OnItemClickTendonList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem >= 0)
	{
		T_PTTN_K nKey = static_cast<T_PTTN_K>(m_wndTendonList.GetItemData(pNMLV->iItem));
		UpdateControlsByPttnData(nKey);
	}

	*pResult = 0;
}

int CPostTensionModifyTendonDlg::GetSelectedTendonKey()
{
	int nSelectedIndex = m_wndTendonList.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
		return -1;

	return static_cast<int>(m_wndTendonList.GetItemData(nSelectedIndex));
}

void CPostTensionModifyTendonDlg::UpdateControlsByPttnData(T_PTTN_K nKey)
{
	T_PTTN_D PttnD;
	if (!m_pDoc->m_pAttrCtrl2->GetPttn(nKey, PttnD))
	{
		ASSERT(0); return;
	}

	m_OriginalPttnData = PttnD;

	// 1. Tendon Name 업데이트
	if (m_edtTendonName.GetSafeHwnd())
	{
		m_edtTendonName.SetWindowText(PttnD.strName);
	}

	// 2. Tendon Property ComboBox 업데이트
	if (m_cmbTendonProperty.GetSafeHwnd())
	{
		CArray<T_TDNT_K, T_TDNT_K> aTdntK;
		m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
		for (int i = 0; i < aTdntK.GetSize(); i++)
		{
			if (aTdntK[i] == PttnD.TdntK)
			{
				m_cmbTendonProperty.SetCurSel(i);
				break;
			}
		}
	}

	// 3. Tendon Stressing ComboBox 업데이트
	if (m_cmbTendonStressing.GetSafeHwnd())
	{
		CArray<T_TDST_K, T_TDST_K> aTdstK;
		m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
		for (int i = 0; i < aTdstK.GetSize(); i++)
		{
			if (aTdstK[i] == PttnD.TdstK)
			{
				m_cmbTendonStressing.SetCurSel(i);
				break;
			}
		}
	}

	// 4. Vertical Tab의 Profile ComboBox와 User Defined Checkbox 업데이트
	if (m_pVerPage && m_pVerPage->GetSafeHwnd())
	{
		m_pVerPage->SetPttnKey(nKey);
	}

	// 5. Horizontal Tab의 Profile 데이터 업데이트
	if (m_pHorPage && m_pHorPage->GetSafeHwnd())
	{
		m_pHorPage->SetPttnKey(nKey);
	}
}

void CPostTensionModifyTendonDlg::Dlg2Data(T_PTTN_D& Data)
{
	Data.Initialize();

	// 1. Tendon Name
	if (m_edtTendonName.GetSafeHwnd())
	{
		m_edtTendonName.GetWindowText(Data.strName);
	}

	// 2. Tendon Property (TdntK)
	if (m_cmbTendonProperty.GetSafeHwnd())
	{
		int nSel = m_cmbTendonProperty.GetCurSel();
		if (nSel >= 0)
		{
			CArray<T_TDNT_K, T_TDNT_K> aTdntK;
			m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
			if (nSel < aTdntK.GetSize())
			{
				Data.TdntK = aTdntK[nSel];
			}
		}
	}

	// 3. Tendon Stressing (TdstK)
	if (m_cmbTendonStressing.GetSafeHwnd())
	{
		int nSel = m_cmbTendonStressing.GetCurSel();
		if (nSel >= 0)
		{
			CArray<T_TDST_K, T_TDST_K> aTdstK;
			m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
			if (nSel < aTdstK.GetSize())
			{
				Data.TdstK = aTdstK[nSel];
			}
		}
	}

	// 4. Vertical Tab 데이터 (TdpfK, bUser, aInfo)
	if (m_pVerPage)
	{
		m_pVerPage->GetPttnData(Data);
	}
}

void CPostTensionModifyTendonDlg::OnApplyBtn()
{
	T_PTTN_K nSelectedKey = GetSelectedTendonKey();
	if (nSelectedKey < 0)
	{
		ASSERT(0); return;
	}

	T_PTTN_D PttnD;
	Dlg2Data(PttnD);
	if (!IsDataChanged(PttnD)) return;

	if (!m_pDoc->m_pDataCtrl->ModifyPttn(nSelectedKey, nSelectedKey, PttnD))
	{
		ASSERT(0); return;
	}

	// 리스트 갱신
	UpdateTendonList();

	// 수정된 항목 다시 선택
	for (int i = 0; i < m_wndTendonList.GetItemCount(); i++)
	{
		if (static_cast<T_PTTN_K>(m_wndTendonList.GetItemData(i)) == nSelectedKey)
		{
			m_wndTendonList.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			UpdateControlsByPttnData(nSelectedKey);
			break;
		}
	}
}

BOOL CPostTensionModifyTendonDlg::IsDataChanged(const T_PTTN_D& NewData)
{
	if (NewData.TdntK != m_OriginalPttnData.TdntK)
		return TRUE;

	if (NewData.TdstK != m_OriginalPttnData.TdstK)
		return TRUE;

	if (m_pVerPage && m_pVerPage->IsGridDataChanged())
		return TRUE;

	return FALSE;
}

void CPostTensionModifyTendonDlg::OnDeleteBtn()
{
	T_PTTN_K nSelectedKey = GetSelectedTendonKey();
	if (nSelectedKey < 0)
	{
		ASSERT(0); return;
	}

	if (!m_pDoc->m_pDataCtrl->DelPttn(nSelectedKey))
	{
		ASSERT(0); return;
	}

	// 리스트 갱신
	UpdateTendonList();

	// 첫 번째 아이템이 있으면 선택하고 컨트롤 업데이트
	int nNewSelectedKey = GetSelectedTendonKey();
	if (nNewSelectedKey > 0)
	{
		UpdateControlsByPttnData(static_cast<T_PTTN_K>(nNewSelectedKey));
	}
}