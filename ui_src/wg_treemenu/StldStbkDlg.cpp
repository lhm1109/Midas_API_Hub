// StldStbkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldStbkDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\SelectCtrl.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#define STBK_DATA_FMT_INT _LS(IDS_WG_TREEMENU__d)
#define STBK_DATA_FMT_DBL _LS(IDS_WG_TREEMENU__g)

// CStldStbkDlg 대화 상자입니다.

CStldStbkDlg::CStldStbkDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldStbkDlg::IDD, pParent)
{
	m_nOption = -1;

	m_aCtrlOption.Add(IDC_TM_STBK_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_STBK_OPT_REPLACE);
	m_aCtrlOption.Add(IDC_TM_STBK_OPT_DELETE);

	m_aCtrlDisable.Add(IDC_TM_STBK_EDIT_DX);
	m_aCtrlDisable.Add(IDC_TM_STBK_EDIT_DY);
	m_aCtrlDisable.Add(IDC_TM_STBK_EDIT_DZ);

	//ctrl ini 추가
}

CStldStbkDlg::~CStldStbkDlg()
{
}

void CStldStbkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_cmbLdgr);
	DDX_Radio(pDX, IDC_TM_STBK_OPT_ADD, m_nOption);

	DDX_Control(pDX, IDC_TM_STBK_EDIT_DX, m_edtDx);
	DDX_Control(pDX, IDC_TM_STBK_EDIT_DY, m_edtDy);
	DDX_Control(pDX, IDC_TM_STBK_EDIT_DZ, m_edtDz);

	DDX_Control(pDX, IDC_TM_STBK_DX_UNIT, m_untDx);
	DDX_Control(pDX, IDC_TM_STBK_DY_UNIT, m_untDy);
	DDX_Control(pDX, IDC_TM_STBK_DZ_UNIT, m_untDz);

	DDX_Control(pDX, IDC_TM_STBK_SDL_LIST, m_listSdlElnk);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC3, m_wndPicture);
}

BEGIN_MESSAGE_MAP(CStldStbkDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, &CStldStbkDlg::OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_STBK_OPT_ADD, &CStldStbkDlg::OnChangeOption)
	ON_BN_CLICKED(IDC_TM_STBK_OPT_REPLACE, &CStldStbkDlg::OnChangeOption)
	ON_BN_CLICKED(IDC_TM_STBK_OPT_DELETE, &CStldStbkDlg::OnChangeOption)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_STBK_SDL_LIST, OnChangedItemSelection)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()


void CStldStbkDlg::InitUnit()
{
	m_edtDx.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDx);
	m_edtDy.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDy);
	m_edtDz.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDz);
	m_untDx.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDx);
	m_untDy.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDy);
	m_untDz.SetUnitType(CUnitCtrl::m_STBK_UNIT.dDz);
}


void CStldStbkDlg::InitCtrl()
{
	GetDlgItem(IDC_TM_STBK_STATIC2)->SetWindowText(_LS(IDS_WG_TREEMENU_STBK_IDC_TM_STBK_STATIC2)); //"Load Group Name"

	int nOpt = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOpt);
	CtrlEnableDisable(m_aCtrlDisable, nOpt==0);

	m_nOption = 0;
	m_edtDx.SetWindowText(_T("0"));
	m_edtDy.SetWindowText(_T("0"));
	m_edtDz.SetWindowText(_T("0"));

	// ListCtrl의 헤더 및 요소 설정
	SetListCtrlHeader();
	SetListCtrlItem();
	ResizeListCtrl();
}

// CStldStbkDlg 메시지 처리기입니다.
BOOL CStldStbkDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitUnit();
	InitCtrl();	

	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_sdsp 1.svg"));
	GetDlgItem(IDC_TM_EXECUTE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TM_CLOSE)->EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;
}

void CStldStbkDlg::SetListCtrlHeader()
{
	CStringArray aTitles;
	CArray<int,int> aWidths;
	CArray<float,float> aWRatio;
	aTitles.Add(_LS(IDS_WG_TREEMENU_ID));
	aTitles.Add(_LS(IDS_WG_TREEMENU_NODE1));
	aTitles.Add(_LS(IDS_WG_TREEMENU_NODE2));

    aWRatio.Add((float)(1.0/7.0));
    aWRatio.Add((float)(2.7/7.0));
    aWRatio.Add((float)(2.7/7.0));
	CDlgUtil::_SetListCtrlHeader(&m_listSdlElnk,aTitles,&aWRatio,NULL);
}

void CStldStbkDlg::SetListCtrlItem()
{
	// Saddle Type Elastic Link만 List 컨트롤의 요소로 추가한다.
	m_listSdlElnk.DeleteAllItems();

	int nCount = m_pDoc->m_pAttrCtrl->GetCountElnk();
	if(nCount == 0) return;


	T_ELNK_D Data;
	CArray<T_ELNK_K, T_ELNK_K> aKeyElnk;
	CArray<T_ELNK_D, T_ELNK_D&> aDataElnk;
	m_pDoc->m_pAttrCtrl->GetElnkList(aKeyElnk, aDataElnk);


	CArray<UINT, UINT> KeyBuffer;
	for(int i=0; i< nCount; i++)
	{
		if(aDataElnk[i].nSubOption == 5) // Saddle Type
		{
			KeyBuffer.Add(aKeyElnk[i]);
		}
	}
	
	int nSize = KeyBuffer.GetSize();
	if(nSize == 0) return;

	CStringArray aContents;
	CString      tmpStr;

	qsort(KeyBuffer.GetData(),nSize,sizeof(T_ELNK_K), CCompFunc::UINTAsc);
	for(int i = 0 ;i < nSize; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetElnk(KeyBuffer[i],Data))
		{
			aContents.RemoveAll();
			tmpStr.Format(STBK_DATA_FMT_INT, KeyBuffer[i]); aContents.Add(tmpStr);
			tmpStr.Format(STBK_DATA_FMT_INT, Data.Node1); aContents.Add(tmpStr);
			tmpStr.Format(STBK_DATA_FMT_INT, Data.Node2); aContents.Add(tmpStr);

			CDlgUtil::SetListItem(&m_listSdlElnk,i,aContents,(DWORD)KeyBuffer[i]);
		}
	}

}

void CStldStbkDlg::ResizeListCtrl()
{
	if(m_listSdlElnk.m_hWnd)
	{
		//m_listSdlElnk.ShowScrollBar(SB_VERT);
		//int iListCount = m_listSdlElnk.GetItemCount();
		//int iListSize = m_listSdlElnk.GetCountPerPage();
		//
		//if (iListCount < iListSize)
		//{
		//	m_listSdlElnk.EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
		//}
		//else
		//{
		//	m_listSdlElnk.EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
		//}
	}
}

BOOL CStldStbkDlg::GetSelectedSdlELink(T_ELNK_K &key)
{
	int iItem = m_listSdlElnk.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	key = (T_ELNK_K)m_listSdlElnk.GetItemData(iItem);
	return TRUE;
}

BOOL CStldStbkDlg::GetSelectedSetBack(T_STBK_K &key)
{
	// List Ctrl에서 선택된 링크와 물려있는 set-back 가져오는 기능
	T_ELNK_K elnkKey;
	T_ELNK_D elnkData;
	if(!GetSelectedSdlELink(elnkKey)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElnk(elnkKey,elnkData)) return FALSE;

	CArray<T_STBK_K, T_STBK_K> aKeyStbk;
	CArray<T_STBK_D, T_STBK_D&> aDataStbk;
	
	if(m_pDoc->m_pAttrCtrl->GetStbkList(aKeyStbk,aDataStbk) == 0) return FALSE;

	for(int i=0; i< aKeyStbk.GetSize(); i++)
	{
		if(aDataStbk[i].node1 == elnkData.Node1 && aDataStbk[i].node2 == elnkData.Node2)
		{
			key = aKeyStbk[i];
			return TRUE;
		}
	}

	key = 0;
	return FALSE;
}

void CStldStbkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		SetListCtrlItem();
		ResizeListCtrl();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		SetListCtrlItem();
		ResizeListCtrl();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

BOOL CStldStbkDlg::CheckNodeList()
{
	// 2개 노드가 선택되었는지 확인
	CArray<int, int> aKeyNode;
	CString csNodeList;

	if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
		return FALSE;
	return TRUE;
}

void CStldStbkDlg::OnTmDefineLdgrBtn()
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);
}


void CStldStbkDlg::Execute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	T_ELNK_K KeyOld;
	if (!GetSelectedSdlELink(KeyOld))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_sdl_elastic_link));
		return;
	}

	T_STBK_D data;
	T_ELNK_D elnkData;
	unsigned int nLoadGrpKey;

	m_cmbLdgr.GetSelectedLdgr(nLoadGrpKey);
	m_pDoc->m_pAttrCtrl->GetElnk(KeyOld,elnkData);
	
	data.LoadGrpKey = nLoadGrpKey;
	data.node1 = elnkData.Node1;
	data.node2 = elnkData.Node2;
	data.dDx = m_edtDx.GetEditValue();
	data.dDy = m_edtDy.GetEditValue();
	data.dDz = m_edtDz.GetEditValue();
	
	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		bSuccess = m_pDoc->m_pDataCtrl->AddStbk(data, (m_nOption==0)?FALSE:TRUE);
	}
	else if (m_nOption == 2)  // delete
	{
		T_STBK_K Key;
		if(GetSelectedSetBack(Key))
			bSuccess = m_pDoc->m_pDataCtrl->DelStbk(Key);	
	}
	if(bSuccess)
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldStbkDlg::OnChangeOption()
{
	UpdateData(TRUE);
	if (m_nOption == 2) // delete
	{
		CtrlEnableDisable(m_aCtrlDisable, FALSE);
	}
	else
	{
		CtrlEnableDisable(m_aCtrlDisable, TRUE);
	}
}

void CStldStbkDlg::OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	CArray<T_NODE_K, T_NODE_K> arKeyNode;
	CString strNode;
	CNumericOptimizer optimizer;
	CSelectConnector connector;


	int iItem = m_listSdlElnk.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if(iItem == -1) return;
	CString sNode1 = m_listSdlElnk.GetItemText(iItem, 1);
	CString sNode2 = m_listSdlElnk.GetItemText(iItem, 2);


	arKeyNode.Add(_ttol(sNode1));
	arKeyNode.Add(_ttol(sNode2));

	strNode =  optimizer.OptimizeUnsortedArray(arKeyNode);
	connector.SelectByStr(strNode);
	
	*pResult = 0;
}

LRESULT CStldStbkDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	// wParam : 핸들 or 정수 등 실제 데이터
	// lParam : 포인터 값
	/*if(m_wndNodeList.GetSafeHwnd()==(HWND)lParam)
	{
	if(CheckNodeList())
	{
	OnTmExecute();
	m_wndNodeList.ClearNodeKeyList();
	}
	}*/
	return 0L;
}