// CMLLaneJPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLLaneJPDlg.h"
#include "CMLLaneJPInfoDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPDlg dialog


CCMLLaneJPDlg::CCMLLaneJPDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLLaneJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLLaneJPDlg)
	//}}AFX_DATA_INIT
	m_pInfoDlg = NULL; 
	m_nOP = 3; // Else
}


void CCMLLaneJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLLaneJPDlg)
	DDX_Control(pDX, IDC_GROUP_LOAD_WIDTH_UNIT, m_wndGLoadWUnit);
	DDX_Control(pDX, IDC_MAIN_LOAD_WIDTH_UNIT, m_wndMLoadWUnit);
	DDX_Control(pDX, IDC_MAIN_LOAD_WIDTH_EDIT, m_wndMLoadW);
	DDX_Control(pDX, IDC_LIVE_LOAD_WIDTH_UNIT, m_wndLLoadWUnit);
	DDX_Control(pDX, IDC_LIVE_LOAD_WIDTH_EDIT, m_wndLLoadW);
	DDX_Control(pDX, IDC_GROUP_LOAD_WIDTH_EDIT, m_wndGLoadW);
	DDX_Control(pDX, IDC_LLANE_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLLaneJPDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLLaneJPDlg)
	ON_BN_CLICKED(IDC_REMOVE_BUTTON, OnRemoveButton)
	ON_BN_CLICKED(IDC_MOD_BUTTON   , OnModButton   )
	ON_BN_CLICKED(IDC_DEL_BUTTON   , OnDelButton   )
	ON_BN_CLICKED(IDC_COPY_BUTTON  , OnCopyButton  )
	ON_BN_CLICKED(IDC_ADD_BUTTON   , OnAddButton   )
	ON_UPDATE_COMMAND_UI(IDC_ADD_BUTTON   , OnUpdateAdd)
	ON_UPDATE_COMMAND_UI(IDC_DEL_BUTTON   , OnUpdateDel)
	ON_UPDATE_COMMAND_UI(IDC_MOD_BUTTON   , OnUpdateMod)
	ON_UPDATE_COMMAND_UI(IDC_COPY_BUTTON  , OnUpdateCopy)
	ON_UPDATE_COMMAND_UI(IDOK             , OnUpdateOk)
	ON_UPDATE_COMMAND_UI(IDC_LLANE_LIST   , OnUpdateLLaneList)
	ON_UPDATE_COMMAND_UI(IDC_REMOVE_BUTTON, OnUpdateRemoveBtn)
	ON_NOTIFY(NM_CLICK, IDC_LLANE_LIST, OnClickLlaneList)
	ON_NOTIFY(NM_DBLCLK, IDC_LLANE_LIST, OnDblclkLlaneList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LLANE_LIST, OnItemchangedLlaneList)
	ON_NOTIFY(NM_RETURN, IDC_LLANE_LIST, OnReturnLlaneList)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE ,OnKickIdle)
	ON_REGISTERED_MESSAGE(CNotifyListCtrl::CNOTIFY_LCTRL_KEYUP,OnListCtrlKeyUp)
END_MESSAGE_MAP()


void CCMLLaneJPDlg::UpdateUnit() 
{
#define CONVUNIT(a,b) b=CDBDoc::GetDocPoint()->m_pUnitCtrl->\
												ConvertUnitDataPrevious((a),(b)) 
	int nInfo = this->m_LLANjpD.arJiganInfo.GetSize();

	for(int i = 0; i < nInfo; i++)
	{
		CONVUNIT(D_UNITSYS_BASE_LENGTH,m_LLANjpD.arJiganInfo[i].dLength);
		CONVUNIT(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadMomentAdj ,m_LLANjpD.arJiganInfo[i].dP1LoadMomentAdj   );
		CONVUNIT(CUnitCtrl::m_LLANjp_UNIT_OLD.dP1LoadShearAdj  ,m_LLANjpD.arJiganInfo[i].dP1LoadShearAdj   ); 
		CONVUNIT(CUnitCtrl::m_LLANjp_UNIT_OLD.dP2LoadAdj   ,m_LLANjpD.arJiganInfo[i].dP2LoadAdj   );
		CONVUNIT(CUnitCtrl::m_LLANjp_UNIT_OLD.dCrowdLoadAdj,m_LLANjpD.arJiganInfo[i].dCrowdLoadAdj);
		CONVUNIT(CUnitCtrl::m_LLANjp_UNIT_OLD.dTLoadAdj    ,m_LLANjpD.arJiganInfo[i].dTLoadAdj    );
	}
#undef CONVUNIT  
	CArray<int, int>arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_List,arRet);
	MakeInfoList();

	if(arRet.GetSize())
	{
		int nSel = arRet[0];
		arRet.RemoveAll();
		arRet.Add(nSel);
		CDlgUtil::SetListItemByMask(&m_List,arRet,LVIS_SELECTED,LVIS_SELECTED);
	}

}

void CCMLLaneJPDlg::OnUpdateRemove(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_REMOVE_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}

void CCMLLaneJPDlg::OnUpdateAdd(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_ADD_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}

void CCMLLaneJPDlg::OnUpdateDel(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_DEL_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}

void CCMLLaneJPDlg::OnUpdateMod(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_MOD_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}	

void CCMLLaneJPDlg::OnUpdateCopy(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_COPY_BUTTON);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}

void CCMLLaneJPDlg::OnUpdateOk(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDOK);
	if(pWnd)
		pWnd->EnableWindow(m_pInfoDlg == NULL);
}

void CCMLLaneJPDlg::OnUpdateLLaneList(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_LLANE_LIST);
	if(pWnd)
		pWnd->EnableWindow(m_nOP != 0);
}

void CCMLLaneJPDlg::OnUpdateRemoveBtn(CCmdUI*pCmdUI)
{
	CWnd* pWnd = GetDlgItem(IDC_REMOVE_BUTTON);  
	if(pWnd)
		pWnd->EnableWindow(CDBDoc::GetDocPoint()->m_pAttrCtrl->ExistLlanjp_Old() && m_pInfoDlg == NULL);
}

LRESULT CCMLLaneJPDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogControls(this, FALSE);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPDlg message handlers
#define COLCOUNT 2
void CCMLLaneJPDlg::SetHeaderTitle()
{
	CStringArray         HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_CMD_TRAFFIC_LINE_LANE__Name));   HRatio.Add(0.5f);
	HTitles.Add(_LS(IDS_CMD_TRAFFIC_LINE_LANE__Length)); HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_List, HTitles,&HRatio,NULL);
 
}


BOOL CCMLLaneJPDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle();

	m_wndGLoadW.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dCrowdLoadWidth);
	m_wndGLoadWUnit.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dCrowdLoadWidth);
	m_wndLLoadW.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dLiveLoadWidth);
	m_wndLLoadWUnit.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dLiveLoadWidth);
	m_wndMLoadW.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dMainLoadWidth);
	m_wndMLoadWUnit.SetUnitType(CUnitCtrl::m_LLANjp_UNIT_OLD.dMainLoadWidth);

	Data2Dlg();
	ButtonCtrl();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLLaneJPDlg::OnOK() 
{
	Dlg2Data();
	if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddLlanjp_Old(m_LLANjpD))
		DestroyWindow();
}

void CCMLLaneJPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//CCMDlgBase::OnCancel();
}

void CCMLLaneJPDlg::OnRemoveButton() 
{
	// TODO: Add your control notification handler code here
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->ExistLlanjp_Old())
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelLlanjp_Old();	
	DestroyWindow();
}

void CCMLLaneJPDlg::OnModButton() 
{
	// TODO: Add your control notification handler code here
	m_nOP = 1;
	if(!m_pInfoDlg)
	{
		//CArray<DWORD, DWORD> arRet;
		//CDlgUtil::GetSelectedListItemData(&m_List,arRet);
		CArray<int,int> arRet;
		CDlgUtil::GetSelectedListItemIndex(&m_List,arRet);

		if(arRet.GetSize() == 0) 
		{
			AfxMessageBox(_LS(IDS_CMD_SELECT_LANE_INFO));
			return ;
		}

		if(arRet.GetSize() > 1)
		{
			AfxMessageBox(_LS(IDS_CMD_LANE_MORE_SELECTED));
			return ;
		}

		m_pInfoDlg = new CCMLLaneJPInfoDlg;
		m_pInfoDlg->m_ppMySelf = &m_pInfoDlg;  //Info Dlg 종료시 InfoDlg에서 정리
		m_pInfoDlg->m_pnOP = & m_nOP;
		m_pInfoDlg->Create(CCMLLaneJPInfoDlg::IDD,this);
		m_pInfoDlg->Data2Dlg(&m_nOP,&(m_LLANjpD.arJiganInfo[arRet[0]]),this);
		m_pInfoDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		//내용 Update --
	}

	
}

void CCMLLaneJPDlg::OnDelButton() 
{
	// TODO: Add your control notification handler code here
	DelOp();	
}

void CCMLLaneJPDlg::OnCopyButton() 
{
	// TODO: Add your control notification handler code here
	CopyOp();
}

void CCMLLaneJPDlg::OnAddButton() 
{
	// TODO: Add your control notification handler code here
	m_nOP = 0;
	if(!m_pInfoDlg)
	{
		m_pInfoDlg = new CCMLLaneJPInfoDlg;
		m_pInfoDlg->m_ppMySelf = &m_pInfoDlg;  //Info Dlg 종료시 InfoDlg에서 정리
		m_pInfoDlg->Create(CCMLLaneJPInfoDlg::IDD,this);
		m_pInfoDlg->Data2Dlg(& m_nOP,NULL,this);
		m_pInfoDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		//내용 Update --
	}
}

BOOL CCMLLaneJPDlg::DestroyWindow()
{
	if(m_pInfoDlg) m_pInfoDlg->DestroyWindow();
	
	return CCMDlgBase::DestroyWindow();
}

// nOP (0) Add (1) Modify
BOOL CCMLLaneJPDlg::SetByInfoDlg(T_LLANjp_BASE_OLD* pLLANDBase)
{
	if     (m_nOP == 0) // Add
	{
		return AddOp(pLLANDBase);
	}
	else if(m_nOP == 1) // Modify 
	{
		return ModOp(pLLANDBase);
	}
	
	return FALSE;
}

void CCMLLaneJPDlg::Dlg2Data()
{
	m_LLANjpD.dCrowdLoadWidth = m_wndGLoadW.GetEditValue();
	m_LLANjpD.dLiveLoadWidth = m_wndLLoadW.GetEditValue();
	m_LLANjpD.dMainLoadWidth  = m_wndMLoadW.GetEditValue();

	////////////////////////////////////////////////////////
	// 지간 정보는 Operation 발생시 지속적으로 관리되므로 
	// 현상태를 그대로 적용한다. 
}

void CCMLLaneJPDlg::Data2Dlg()
{
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanjp_Old(m_LLANjpD))
	{
		m_wndGLoadW.SetEditUnit(m_LLANjpD.dCrowdLoadWidth);
		m_wndLLoadW.SetEditUnit(m_LLANjpD.dLiveLoadWidth);
		m_wndMLoadW.SetEditUnit(m_LLANjpD.dMainLoadWidth);
		MakeInfoList();
	}
}

void CCMLLaneJPDlg::MakeInfoList()
{
	CStringArray Contents;
	CString strTemp;
	
	m_List.DeleteAllItems();
	int nInfo = m_LLANjpD.arJiganInfo.GetSize();

	for(int i = 0; i < nInfo; i++)
	{
		Contents.RemoveAll();
		Contents.Add(m_LLANjpD.arJiganInfo[i].csName);
		strTemp.Format(_T("%g"),m_LLANjpD.arJiganInfo[i].dLength); 
		Contents.Add(strTemp);
		
		CDlgUtil::SetListItem(&m_List,i,Contents,(LONG_PTR)&(m_LLANjpD.arJiganInfo[i]));
	}
}

BOOL CCMLLaneJPDlg::IsExistInfoName(CString& InfoName,CString ToBeExcludeName)
{
	CMapStringToOb StrMap;

	int nInfo = m_LLANjpD.arJiganInfo.GetSize();
	for(int i = 0; i < nInfo; i++)
	{
		if(m_LLANjpD.arJiganInfo[i].csName == ToBeExcludeName)
			continue;
		
		StrMap.SetAt(m_LLANjpD.arJiganInfo[i].csName,NULL);
	}

	CObject* pTemp;

	if(StrMap.Lookup(InfoName,pTemp))
		 return TRUE;

	return FALSE;
}

BOOL CCMLLaneJPDlg::AddOp(T_LLANjp_BASE_OLD* pLlanBaseD)
{
	//이름 중복 검사...
	if(IsExistInfoName(pLlanBaseD->csName,_T("")))
	{
		AfxMessageBox(_LS(IDS_CMD_SAME_NAME_LANE_EXIST));
		return FALSE;
	}
	
	T_LLANjp_BASE_OLD LlanD;

	LlanD = * pLlanBaseD;
	
	m_LLANjpD.arJiganInfo.Add(LlanD);
	CStringArray Contents;
	CString strTemp;

	int nInfo = m_LLANjpD.arJiganInfo.GetSize();

	Contents.Add(pLlanBaseD->csName);
	strTemp.Format(_T("%g"),pLlanBaseD->dLength);
	Contents.Add(strTemp);

	nInfo = nInfo -1;

	CDlgUtil::SetListItem(&m_List,nInfo,Contents,(LONG_PTR)&(m_LLANjpD.arJiganInfo[nInfo]));
	return TRUE;
}

void CCMLLaneJPDlg::DelOp()
{
	T_LLANjp_BASE_OLD* pLane;
	CArray<DWORD, DWORD> Datas;
	CArray<T_LLANjp_BASE_OLD, T_LLANjp_BASE_OLD&> Llans;

	CMapStringToOb MapStrs;
	CDlgUtil::GetSelectedListItemData(&m_List,Datas);
	int nData = Datas.GetSize();
	if(!nData) return;
	
	for(int i = 0; i < nData; i++)
	{
		pLane = (T_LLANjp_BASE_OLD*)(LONG_PTR)Datas[i];
		MapStrs.SetAt(pLane->csName, NULL);
	}

	CObject* pTemp;
	int nItem = m_LLANjpD.arJiganInfo.GetSize();
	for(int i = 0; i< nItem; i++)
	{
		if(!MapStrs.Lookup(m_LLANjpD.arJiganInfo[i].csName,pTemp))
			Llans.Add(m_LLANjpD.arJiganInfo[i]);
	}

	m_LLANjpD.arJiganInfo.Copy(Llans);

	MakeInfoList();
}

BOOL CCMLLaneJPDlg::ModOp(T_LLANjp_BASE_OLD* pLlanBaseD)
{
	T_LLANjp_BASE_OLD* pLane;
	CArray<DWORD, DWORD> Datas    ;
	CArray<int, int    > SelIs    ;  
	CDlgUtil::GetSelectedListItemData (&m_List,Datas);
	CDlgUtil::GetSelectedListItemIndex(&m_List,SelIs);

	if(Datas.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_CMD_SELECT_LANE_INFO));
		return FALSE;
	}

	if(Datas.GetSize() > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_LANE_MORE_SELECTED));
		return FALSE;
	}

	//이름 중복 검사...
	CString ToBeExcludeName = m_List.GetItemText(SelIs[0],0);

	if(IsExistInfoName(pLlanBaseD->csName,ToBeExcludeName))
	{
		AfxMessageBox(_LS(IDS_CMD_SAME_NAME_LANE_EXIST));
		return FALSE;
	}

	pLane = &m_LLANjpD.arJiganInfo[SelIs[0]];

	*pLane = *pLlanBaseD;

	CStringArray Contents;
	CString strTemp;
	
	Contents.Add(pLane->csName);
	strTemp.Format(_T("%g"),pLane->dLength);
	Contents.Add(strTemp);
	
	CDlgUtil::SetListItem(&m_List,SelIs[0],Contents,(LONG_PTR)pLane);
	return TRUE;
}

void CCMLLaneJPDlg::CopyOp()
{
	CArray<int,int> SelIs;
	CDlgUtil::GetSelectedListItemIndex(&m_List,SelIs);

	if(SelIs.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_CMD_SELECT_COPY_LANE_INFO));
		return ;
	}

	if(SelIs.GetSize() > 1)
	{
		AfxMessageBox(_LS(IDS_CMD_LANE_MORE_SELECTED));
	}

	CString csInfoName = m_List.GetItemText(SelIs[0], 0);
	CArray<CString, CString&>InfoNames;

	int nInfo = this->m_LLANjpD.arJiganInfo.GetSize();
	for(int i = 0 ; i < nInfo; i++)
	{
		InfoNames.Add(m_LLANjpD.arJiganInfo[i].csName);
	}

	CString StrName;
	if(!CStrParser::MakeUniqueName(InfoNames,csInfoName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD_CANNOT_LANE_NAME));
		return;
	}

	T_LLANjp_BASE_OLD InfoD;
	InfoD = m_LLANjpD.arJiganInfo[SelIs[0]];
	InfoD.csName = StrName;

	AddOp(&InfoD);
}

/*
void CCMLlanDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
	CString csLineLaneName = m_List.GetItemText(iItem, 0);

	CArray<CString,CString&> LLaneNames;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanList(LLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(LLaneNames,csLineLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}

	
	T_LLAN_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlan(csLineLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csLineLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LineLaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddLlan(LaneD);
}
*/

void CCMLLaneJPDlg::ShowInfoData()
{
	if(m_nOP == 0) return;

	CArray<int,int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_List,arRet);
	if(!arRet.GetSize()) return;
	
	if(!m_pInfoDlg)
	{
		CArray<T_ELEM_K, T_ELEM_K>arSelElem;
		int nElem = m_LLANjpD.arJiganInfo[arRet[0]].arElem.GetSize();
		for(int i = 0; i < nElem; i++)
			arSelElem.Add(m_LLANjpD.arJiganInfo[arRet[0]].arElem[i]);

		CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem
			(NULL,arSelElem, FALSE, TRUE);
		return;
	}
	
	m_pInfoDlg->Data2Dlg(&m_nOP,&(m_LLANjpD.arJiganInfo[arRet[0]]),this);
}

LRESULT CCMLLaneJPDlg::OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam)
{
	ShowInfoData();
	return 0L;
}

void CCMLLaneJPDlg::OnClickLlaneList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowInfoData();
	*pResult = 0;
}

void CCMLLaneJPDlg::OnDblclkLlaneList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//ShowInfoData();
	OnModButton();
	*pResult = 0;
}

void CCMLLaneJPDlg::OnItemchangedLlaneList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	ShowInfoData();
	
	*pResult = 0;
}

void CCMLLaneJPDlg::OnReturnLlaneList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnModButton();	
	*pResult = 0;
}


void CCMLLaneJPDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		MakeInfoList();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLLaneJPDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

void CCMLLaneJPDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMvcdChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

/*
BOOL CCMLLaneJPDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_JAPAN) return TRUE;
	return FALSE;
}

void CCMLLaneJPDlg::ShowHideByCode()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if (!IsRelevantCode(MvcdData.nCodeType)) DestroyWindow();
}
*/