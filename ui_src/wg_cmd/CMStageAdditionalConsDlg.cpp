#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageAdditionalConsDlg.h"
#include "CMStageAdditionalDefDlg.h"
#include "CMStageItemBase.h"
//#include "CMStageConsDlg.h"
//#include "CMStageDefMainDlg.h"
//#include "CMStageDefineDlg.h"
//#include "CMStageItemBase.h"
//#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

//using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define CDialog CChildDialogMove

CCMStageAdditionalConsDlg::CCMStageAdditionalConsDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageAdditionalConsDlg::IDD, pParent)
{
	EnableLayout(TRUE);
}

void CCMStageAdditionalConsDlg::LayoutCtrl(void)
{
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout)
	{
		pLayout->AddAnchor(IDC_CMD_ADD, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_MODIFY1, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_CMD_DELETE1, CBCGPStaticLayout::XMoveType::e_MoveTypeHorz, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
		pLayout->AddAnchor(IDC_STAGE_LIST, CBCGPStaticLayout::XMoveType::e_MoveTypeNone, CBCGPStaticLayout::XSizeType::e_SizeTypeBoth);
		pLayout->AddAnchor(IDC_CMD_CLOSE, CBCGPStaticLayout::XMoveType::e_MoveTypeBoth, CBCGPStaticLayout::XSizeType::e_SizeTypeNone);
	}
}

void CCMStageAdditionalConsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageConsDlg)
	DDX_Control(pDX, IDC_STAGE_LIST, m_wndStageList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMStageAdditionalConsDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageConsDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_DELETE1, OnCmdDelete1)
	ON_BN_CLICKED(IDC_CMD_MODIFY1, OnCmdModify1)
	ON_NOTIFY(NM_DBLCLK, IDC_STAGE_LIST, OnDblclkStageList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMStageAdditionalConsDlg::CtrlMan()
{
	// show menu 추가시 위치 조정.
	if (!CCMStageItemBase::IsCurStageBase())
	{
		CArray<UINT, UINT> Ctrls;
		Ctrls.Add(IDC_CMD_ADD);
		Ctrls.Add(IDC_CMD_DELETE1);
		CDlgUtil::CtrlEnableDisable(this, Ctrls, FALSE);
	}
}

BOOL CCMStageAdditionalConsDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	LayoutCtrl();

	CStringArray HTitles;
	CArray<float, float> HRatio;

	// _LS(IDS_CMD0417__Name) 형태로 수정 필요.
	HTitles.Add(_T("Stage"));
	HTitles.Add(_T("Analysis"));

	HRatio.Add(0.3f);
	HRatio.Add(0.7f);

	CDlgUtil::_SetListCtrlHeader(&m_wndStageList, HTitles, &HRatio, NULL);

	UpdateAdditionalAnalList();

	CtrlMan();

	return TRUE;
}

BOOL CCMStageAdditionalConsDlg::UpdateAdditionalAnalList()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!pDoc->m_pAttrCtrl->ExistConstStag())
	{
		m_wndStageList.DeleteAllItems();
		return FALSE;
	}

	CArray<T_CSPA_K, T_CSPA_K> rKeyList;
	pDoc->m_pAttrCtrl2->GetCspaKeyList(rKeyList);
	int nKeys = rKeyList.GetSize();
	CStringArray Contents;

	m_wndStageList.DeleteAllItems();

	for (int i = 0; i < nKeys; i++)
	{
		T_CSPA_D CspaD;
		if (pDoc->m_pAttrCtrl2->GetCspa(rKeyList[i], CspaD))
		{
			CStringArray Contents;

			// Stage
			T_STAG_K StagKey = (T_STAG_K)rKeyList[i];
			T_STAG_D StagD;
			if (pDoc->m_pAttrCtrl->GetStag(StagKey, StagD))
			{
				Contents.Add(StagD.StageName);
			}
			else
			{
				ASSERT(FALSE);
				continue;
			}

			int nStatic = CspaD.aSTLoad.GetSize();
			int nBuckling = CspaD.aBuckData.GetSize();

			int iStr = 0;
			CString strLoadCases;
			if (nStatic > 0)
			{
				strLoadCases = _T("ST");
				Contents.Add(strLoadCases);
				iStr++;
			}

			CString strBuckling;
			if (nBuckling > 0)
			{
				strBuckling = iStr > 0 ? _T(", Buck") : _T("Buck");
				Contents.Add(strBuckling);
			}

			// 리스트에 추가 (ItemData에 Key 저장)
			CDlgUtil::SetListItem(&m_wndStageList, i, Contents, (DWORD)rKeyList[i]);
		}
	}

	return TRUE;
}

void CCMStageAdditionalConsDlg::OnCmdAdd()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!pDoc->m_pAttrCtrl->ExistConstStag())
	{
		m_wndStageList.DeleteAllItems();
		return;
	}

	CArray<T_STAG_K, T_STAG_K> rKeyList;
	pDoc->m_pAttrCtrl->GetStagKeyList(rKeyList);

	if (rKeyList.GetSize() == 0)
	{
		ASSERT(FALSE);
		AfxMessageBox(_LS("No construction stage"));
		//AfxMessageBox(_LS(IDS_CMD0417__Not_exist_selected_item)));
		return;
	}

	CCMStageAdditionalDefDlg Dlg;
	//Dlg.m_bInitAdd = TRUE;
	Dlg.m_CspaKey = 0;
	if (Dlg.DoModal() == IDOK)
	{
		//T_CSPA_K CspaKey = (T_CSPA_K)Dlg.m_SelectedStageKey;

		//T_CSPA_D ExistingCspaD;
		//if (pDoc->m_pAttrCtrl2->GetCspa(CspaKey, ExistingCspaD))
		//{
		//	AfxMessageBox(_T("already exists for the selected stage."));
		//	ASSERT(FALSE);
		//	return;
		//}

		//T_CSPA_D CspaD;
		//CspaD.Initialize();
		//CspaD.aSTLoad.Copy(Dlg.m_aSelectedLoadCases);

		//if (Dlg.m_bBucklingChk) // && Dlg.m_CSBuckKey > 0)
		//{
		//	// Buckling Dialog에서 설정한 Key 저장
		//	CspaD.bBuckling = Dlg.m_bBucklingChk;
		//	CspaD.CSBuckKey = Dlg.m_CSBuckKey;
		//}
		//else
		//{
		//	CspaD.CSBuckKey = 0;
		//}

		//pDoc->m_pDataCtrl->AddCspa(CspaKey, CspaD);
	}
}

void CCMStageAdditionalConsDlg::OnCmdModify1()
{
	CArray<int, int> arRet;
	BOOL bCheck = CDlgUtil::GetListItemByMask(&m_wndStageList, arRet, LVIS_SELECTED);
	if (bCheck == FALSE && arRet.GetSize() == 0 && m_wndStageList.GetItemCount() != 0)
	{
		arRet.Add(0); //리스트 박스에 아이템이 존재 하는데 선택 하지 않았을 경우, 첫번째 아이템 자동 선택.
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_CSPA_K OldCspaKey = 0;
	int nSize = arRet.GetSize();
	if (nSize == 0)
	{
		CArray<T_CSPA_K, T_CSPA_K> aCspaK;
		pDoc->m_pAttrCtrl2->GetCspaKeyList(aCspaK);
		if (aCspaK.GetSize() > 0) OldCspaKey = aCspaK[0];
	}
	else
	{
		OldCspaKey = (T_CSPA_K)m_wndStageList.GetItemData(arRet[0]);
	}

	if (OldCspaKey == 0)
	{
		//AfxMessageBox(_LS(IDS_CMD0417__Error___No_Selected_Stage__nChoose_one));
		ASSERT(FALSE);
		return;
	}

	CCMStageAdditionalDefDlg Dlg;
	//Dlg.m_bInitAdd = FALSE;
	Dlg.m_CspaKey = OldCspaKey;
	if (Dlg.DoModal() == IDOK)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();

		T_CSPA_D CspaD; CspaD.Initialize();
		CspaD = Dlg.m_CspaData;

		T_CSPA_K NewCspaKey = (T_CSPA_K)Dlg.m_SelectedStageKey;

		pDoc->m_pDataCtrl->DelCspa(OldCspaKey);
		pDoc->m_pDataCtrl->AddCspa(NewCspaKey,CspaD);

		// UpdateBuffer()에서 자동으로 UpdateAdditionalAnalList() 호출
		// 선택 상태 유지
		m_wndStageList.SetItemState(arRet[0], LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMStageAdditionalConsDlg::OnCmdDelete1()
{
	CArray<DWORD, DWORD> arRet;
	CDlgUtil::GetSelectedListItemData(&m_wndStageList, arRet);

	CArray<T_CSPA_K, T_CSPA_K> aCspaKeys;
	int nRet = arRet.GetSize();
	for (int i = 0; i < nRet; i++)
	{
		aCspaKeys.Add((T_CSPA_K)arRet[i]);
	}

	if (nRet)
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelCspa(aCspaKeys);
}

void CCMStageAdditionalConsDlg::OnCmdClose()
{
	OnOK();
}

void CCMStageAdditionalConsDlg::OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnCmdModify1();
	*pResult = 0;
}

void CCMStageAdditionalConsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	//case D_UPDATE_STAGE_CHANGED:
	//case D_UPDATE_STAGE_DBALL:
		//	UpdateAdditionalAnalList();
		//	break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CCMStageAdditionalConsDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsCspaCommand = FALSE;

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		switch (buffer_ur.nCmd)
		{
		case UR_CSPA_ADD:
		case UR_CSPA_DEL:
		case UR_CSPA_MFD:
		case UR_CSPA_MFS:
		//case UR_CSBK_ADD:
		//case UR_CSBK_DEL:
		//case UR_CSBK_MFD:
		//case UR_CSBK_MFS:
			bIsCspaCommand = TRUE;
			break;
		}
	}

	if (bIsCspaCommand)
	{
		UpdateAdditionalAnalList();
	}
}

//CString CCMStageAdditionalConsDlg::GetLoadCasesString(const CArray<T_STLD_K, T_STLD_K>& aLoadCases)
//{
//	CString strResult;
//	CDBDoc* pDoc = CDBDoc::GetDocPoint();
//
//	for (int i = 0; i < aLoadCases.GetSize(); i++)
//	{
//		if (i > 0) strResult += _T(", ");
//
//		T_STLD_D StldD;
//		if (pDoc->m_pAttrCtrl->GetStld(aLoadCases[i], StldD))
//		{
//			strResult += StldD.LoadCaseName;
//		}
//	}
//
//	return strResult;
//}