// DgnConWallNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallNewDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\ListCtrlScrollMaintainer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\MultiKeyIndex.h"

#include "DgnDataCtrl.h"
#include "DgnConWeldMeshDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallNewDlg dialog
CDgnConWallNewDlg::CDgnConWallNewDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnConWallNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWallNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nD0 = 0.0;
	m_Hc = 0.0;
	m_Bc = 0.0;
	m_Hf = 0.0;
	m_Bf = 0.0;

	m_bInit = FALSE;
}

CDgnConWallNewDlg::~CDgnConWallNewDlg()
{

}

void CDgnConWallNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallNewDlg)
	DDX_Control(pDX, IDC_DGN_WALL_ID_LST, m_WallIDList);
	DDX_Control(pDX, IDC_DGN_SUB_WALL_ID_CHK, m_chkSubWallID);
	DDX_Control(pDX, IDC_DGN_WALL_ID_EDT, m_edtWallID);
	DDX_Control(pDX, IDC_DGN_SUB_WALL_ID_EDT, m_edtSubWallID);
	DDX_Control(pDX, IDC_DGN_START_STORY_CMB, m_cmbStartStory);
	DDX_Control(pDX, IDC_DGN_END_STORY_CMB, m_cmbEndStory);
	DDX_Control(pDX, IDC_DGN_DW_EDT, m_edtDw);
	DDX_Control(pDX, IDC_DGN_DE_EDT, m_edtDe);
	DDX_Control(pDX, IDC_DGN_FACE_EDT, m_untDwDe);
	DDX_Control(pDX, IDC_DGN_USE_MODEL_THIK_CHK, m_chkUseModelThik);
	DDX_Control(pDX, IDC_DGN_USE_MODEL_THIK_EDT, m_edtUseModelThik);
	DDX_Control(pDX, IDC_DGN_USE_MODEL_THIK_UNT, m_untUseModelThik);
	DDX_Control(pDX, IDC_DGN_USE_WELD_FAB_CHK, m_chkUseWeldedFabfic);
	DDX_Control(pDX, IDC_DGN_USE_MODEL_BELENG_EDT, m_edtBELength);
	DDX_Control(pDX, IDC_DGN_USE_MODEL_BELENG_UNT, m_untBELength);
	DDX_Control(pDX, IDC_DGN_REBAR_DATA_GRD, m_RebarGrid);
	DDX_Control(pDX, IDC_DGN_WALL_TYPE1, m_wndWallLayer1);
	DDX_Control(pDX, IDC_DGN_WALL_TYPE2, m_wndWallLayer2);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CDgnConWallNewDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConWallNewDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_WALL_ID_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_SUB_WALL_ID_CHK, OnChkSubWallID)
	ON_BN_CLICKED(IDC_DGN_USE_MODEL_THIK_CHK, OnChkUseModelThickness)
	ON_BN_CLICKED(IDC_DGN_USE_WELD_FAB_CHK, OnChkUseWeldedFabric)
	ON_BN_CLICKED(IDC_DGN_USE_WELD_FAB_BTN, OnBtnUseWeldedFabric)
	ON_CBN_SELCHANGE(IDC_DGN_START_STORY_CMB, OnSelChangeStartStoryCmb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_WALL_ID_LST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_ADD_REP_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_DELETE_BTN, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnConWallNewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_UNIT:
		m_RebarGrid.UpdateUnit();    
		break;
	case D_UPDATE_SEL_ADD:    case D_UPDATE_SEL_DEL:

		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
			break;
	default:
		break;
	}
}

void CDgnConWallNewDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bChange = FALSE;
	T_REBW_K RebwK;   RebwK.keymap = 0;
	T_REBW_D RebwD;
	BOOL bChangeStor = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STOR_ADD):
		case(UR_STOR_DEL):
		case(UR_STOR_MFS):
		case(UR_STOR_MFD):
			bChangeStor = TRUE;
			break;
		case(UR_REBW_ADD):
		case(UR_REBW_MFD):  pViewBuff->GetRebw(nKey, RebwK, RebwD);  
		case(UR_REBW_DEL):
		case(UR_REBW_MFS):  bChange = TRUE;   break;
		default:						break;
		}
	}  

	if(bChangeStor)
	{
		InitStartStoryCombo();
		InitEndStoryCombo();
	}
	if(bChange)
	{
		CListCtrlScrollMaintainer lsm(&m_WallIDList);

		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
		if(RebwK.keymap)    ShowSelectedRebw(RebwK.keymap, FALSE);
	}
}

void CDgnConWallNewDlg::ShowSelectedRebw(T_REBW_KEY RebwKey, BOOL bInit)
{
	int nIndex = 0;
	if(!m_mRebwK2Index.Lookup(RebwKey, nIndex))     {  ASSERT(0);		return ;	}

	if(!bInit)   MakeItemEx();
	// Unselect current selection
	m_WallIDList.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	m_WallIDList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
//  m_SectList.SetSelectionMark(nIndex);

	LRESULT Result;
	OnClkListItem(NULL, &Result);
	m_WallIDList.SetFocus();
}

void CDgnConWallNewDlg::ShowBitmap(BOOL bEndCheck)
{
	if(bEndCheck)
	{
		m_wndWallLayer1.ShowWindow(SW_SHOW);
		m_wndWallLayer2.ShowWindow(SW_HIDE);
		m_wndWallLayer2.HideDialog();
	}
	else
	{
		m_wndWallLayer1.ShowWindow(SW_HIDE);
		m_wndWallLayer2.ShowWindow(SW_SHOW);
		m_wndWallLayer1.HideDialog();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallNewDlg message handlers

BOOL CDgnConWallNewDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	InitCtrls();
	AlignCtrls();
	EnableDisableCtrls();

	Data2Dlg();

	m_bInit = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConWallNewDlg::OnChkSubWallID()
{
	BOOL bCheck = m_chkSubWallID.GetCheck();
	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_WALL_ID_EDT);
	aCtrls.Add(IDC_DGN_DASH_STC);
	aCtrls.Add(IDC_DGN_SUB_WALL_ID_EDT);
	aCtrls.Add(IDC_DGN_START_STORY_CMB);
	aCtrls.Add(IDC_DGN_END_STORY_CMB);
	CDlgUtil::CtrlEnableDisable(this, aCtrls, bCheck);

	T_REBW_KEY RebwKey;
	if(!GetSelectedItem(RebwKey))    return;
	
	T_REBW_K RebwK;   RebwK.keymap = RebwKey;
	BOOL bShow = RebwK.key.storid == 0?   TRUE:   FALSE;

	CString strTemp;
	if(bCheck && bShow)
	{
		int nWallID = RebwK.key.wallid;
		strTemp.Format(_T("%d"), nWallID);
		m_edtWallID.SetWindowText(strTemp);

		int Index = m_cmbStartStory.GetCurSel();
		if(Index == -1)   return;
		
		CString strStartStory=_T("");
		m_cmbStartStory.GetLBText(Index,strStartStory);
		m_edtSubWallID.SetWindowText(strStartStory);
	}
	else
	{
		m_edtWallID.SetWindowText(_T(""));
		m_edtSubWallID.SetWindowText(_T(""));
	}

	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nSelected = GetSelectedItems(aRebcKey);
	
	BOOL bEdtEndStor = TRUE;
	if(nSelected > 1 || (bShow  && !bCheck))     bEdtEndStor = FALSE;
	m_cmbEndStory.EnableWindow(bEdtEndStor);
}

void CDgnConWallNewDlg::OnSelChangeStartStoryCmb()
{
	InitEndStoryCombo();
}

void CDgnConWallNewDlg::OnChkUseModelThickness()
{
	BOOL bCheck = m_chkUseModelThik.GetCheck();

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_USE_MODEL_THIK_EDT);
	aCtrls.Add(IDC_DGN_USE_MODEL_THIK_UNT);
	CDlgUtil::CtrlEnableDisable(this, aCtrls, !bCheck);
}

void CDgnConWallNewDlg::OnChkUseWeldedFabric()
{
	BOOL bCheck = m_chkUseWeldedFabfic.GetCheck();

	GetDlgItem(IDC_DGN_USE_WELD_FAB_BTN)->EnableWindow(bCheck);
}

void CDgnConWallNewDlg::OnBtnUseWeldedFabric()
{
	CDgnConWeldMeshDlg dlg;
	
	T_REBW_D RebwD;
	m_RebarGrid.Grid2Data(&RebwD);

	dlg.m_strVBarName = RebwD.VerticalRebarName;
	dlg.m_strHBarName = RebwD.HorizonRebarName;
	
	dlg.m_dVBarSpace  = RebwD.dVerticalRebarSpace;
	dlg.m_dHBarSpace  = RebwD.dHorizonRebarSpace;
	
	
	if(dlg.DoModal()==IDOK)
	{
		RebwD.VerticalRebarName = dlg.m_strVBarName;
		RebwD.HorizonRebarName = dlg.m_strHBarName;
		
		RebwD.dVerticalRebarSpace = dlg.m_dVBarSpace;
		RebwD.dHorizonRebarSpace = dlg.m_dHBarSpace ;

		m_RebarGrid.Data2Grid(&RebwD);
	}
}

void CDgnConWallNewDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)  OnClkListItem(pNMHDR, pResult);
	
	*pResult = 0;
}

void CDgnConWallNewDlg::OnDgnAddReplace()
{
	T_REBW_D RebwD;   RebwD.Initialize();
	if(!Dlg2Data(&RebwD))   return;

	T_REBW_KEY RebwKey;
	if(!GetSelectedItem(RebwKey))    return ;
	T_REBW_K RebwK;   RebwK.keymap = RebwKey;

	CArray<T_REBW_KEY,T_REBW_KEY> aRebwKey;
	int nRebwSize = GetSelectedItems(aRebwKey);

	if(nRebwSize == 1 && (m_chkSubWallID.GetCheck() || RebwK.key.storid))
	{
		int nStorSize = RebwD.aStorK.GetSize();
		if(nStorSize < 1)      {  ASSERT(0);		return ;	}
		if(!RebwK.key.storid)        RebwK.key.storid = RebwD.aStorK[0];

		CArray<T_REBW_K,T_REBW_K> aRebwK;
		int nRebwSize = m_pDoc->m_pAttrCtrl->GetRebwKeyListUsingWallID(RebwK.key.wallid, aRebwK);
		for(int i = 0; i < nRebwSize; i++)
		{
			T_REBW_K tempRebwK = aRebwK[i];
			if(tempRebwK.keymap == RebwK.keymap)    continue;
			if(tempRebwK.key.storid == 0)   continue;
			T_REBW_D tempRebwD;
			if(!m_pDoc->m_pAttrCtrl->GetRebw(tempRebwK, tempRebwD))      {  ASSERT(0);		return ;	}
			int nTempStorSize = tempRebwD.aStorK.GetSize();
			for(int k = 0; k < nTempStorSize; k++)
			{
				for(int m = 0; m < nStorSize; m++)
				{
					if(tempRebwD.aStorK[k] != RebwD.aStorK[m])    continue;

					AfxMessageBox(_LS(IDS_DGN_STORY_IS_DUPLICATED));
					return;
				}
			}
		}
		if(!m_pDoc->m_pDataCtrl->AddRebw(RebwK, RebwD))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(nRebwSize < 1)   return;
		
		CArray<T_REBW_K,T_REBW_K> aRebwK;
		aRebwK.SetSize(nRebwSize);
		for(int i = 0; i < nRebwSize; i++)
		{
			aRebwK[i].keymap = aRebwKey[i];
		}
		
		if(!m_pDoc->m_pDataCtrl->AddRebw(aRebwK, RebwD))     {  ASSERT(0);		return ;	}
	}
}

void CDgnConWallNewDlg::OnDgnDelete()
{
	CArray<T_REBW_KEY,T_REBW_KEY> aRebwKey;
	int nRebwSize = GetSelectedItems(aRebwKey);
	if(nRebwSize < 1)   return;
	
	CArray<T_REBW_K,T_REBW_K> aRebwK;
	aRebwK.SetSize(nRebwSize);
	for(int i = 0; i < nRebwSize; i++)
	{
		aRebwK[i].keymap = aRebwKey[i];
	}

	if(!m_pDoc->m_pDataCtrl->DelRebw(aRebwK))     {  ASSERT(0);		return ;	}
}

void CDgnConWallNewDlg::OnDgnClose()
{
	m_RebarGrid.DestroyWindow();
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CDgnConWallNewDlg::Data2Dlg()
{
	MakeItemEx();
	
	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

BOOL CDgnConWallNewDlg::Dlg2Data(T_REBW_D* pRebwD)
{  
	m_RebarGrid.Grid2Data(pRebwD);

	pRebwD->dDw = m_edtDw.GetEditValue();
	pRebwD->dDe = m_edtDe.GetEditValue();
	
	if(pRebwD->dDw <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_DW_VALUE),MB_OK);
		return FALSE;
	}
	if(pRebwD->dDe <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_DE_VALUE),MB_OK);
		return FALSE;
	}

	pRebwD->bUseModelThik = m_chkUseModelThik.GetCheck();
	pRebwD->dThickness = m_edtUseModelThik.GetEditValue();
	pRebwD->dBELength  = m_edtBELength.GetEditValue();

	T_REBW_KEY RebwKey;
	if(!GetSelectedItem(RebwKey))    return FALSE;
	T_REBW_K RebwK;   RebwK.keymap = RebwKey;
	
	CArray<T_REBW_KEY,T_REBW_KEY> aRebwKey;
	int nRebwSize = GetSelectedItems(aRebwKey);
	
	pRebwD->aStorK.RemoveAll();
	if(nRebwSize == 1 && (m_chkSubWallID.GetCheck() || RebwK.key.storid))
	{    
		CString strStartStory, strEndStory;
		int Index = m_cmbStartStory.GetCurSel();    if(Index == -1)   return FALSE;
		m_cmbStartStory.GetLBText(Index,strStartStory);
		Index = m_cmbEndStory.GetCurSel();    if(Index == -1)   return FALSE;
		m_cmbEndStory.GetLBText(Index,strEndStory);

		T_STOR_K sttStorK = m_pDoc->m_pAttrCtrl->GetStorKey(strStartStory);
		T_STOR_K endStorK = m_pDoc->m_pAttrCtrl->GetStorKey(strEndStory);
		T_STOR_D sttStorD, endStorD;
		if(!m_pDoc->m_pAttrCtrl->GetStor(sttStorK, sttStorD))     {  ASSERT(0);		return FALSE;	}
		if(!m_pDoc->m_pAttrCtrl->GetStor(endStorK, endStorD))     {  ASSERT(0);		return FALSE;	}
		
		CArray<T_STOR_K,T_STOR_K> aStorK;
		m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(aStorK, &sttStorK, &endStorK);
		int nStorSize = aStorK.GetSize();
		for(int i = 0; i < nStorSize; i++)
		{
			T_STOR_K curStorK = aStorK[i];
			T_STOR_D curStorD;
			if(!m_pDoc->m_pAttrCtrl->GetStor(curStorK, curStorD))      {  ASSERT(0);		return FALSE;	}
			if(curStorD.dStoryLevel < sttStorD.dStoryLevel)   continue;
			if(curStorD.dStoryLevel > endStorD.dStoryLevel)   continue;
			
			pRebwD->aStorK.Add(curStorK);
		}
	}

	return TRUE;
}

void CDgnConWallNewDlg::InitCtrls()
{
	SetListCtrlHeader();

	OnChkSubWallID();

	InitStartStoryCombo();
	InitEndStoryCombo();

	CRect rect;
	GetDlgItem(IDC_DGN_REBAR_DATA_GRD)->GetWindowRect(rect);

	m_RebarGrid.SetGridWndSize(rect.Width(), rect.Height());
	m_RebarGrid.Initialize(this);

	m_edtUseModelThik.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untUseModelThik.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBELength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBELength.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndWallLayer1.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_wall_layer1_small.svg"));
	m_wndWallLayer2.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_wall_layer3_small.svg"));

	ShowBitmap(FALSE);

}

void CDgnConWallNewDlg::InitStartStoryCombo()
{
	if(m_cmbStartStory.GetCount() != 0) m_cmbStartStory.ResetContent();
	
	CArray<CString, CString&> strStoryList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int nCount = strStoryList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		CString tempStoryList=_T("");
		tempStoryList = strStoryList.GetAt(i);
		m_cmbStartStory.AddString(tempStoryList);
	}
}

void CDgnConWallNewDlg::InitEndStoryCombo(CString* strEndStory)
{
	if(m_cmbEndStory.GetCount() != 0) m_cmbEndStory.ResetContent();
	
	int Index = m_cmbStartStory.GetCurSel();
	if(Index == -1)   return;
	
	CString strStartStory=_T("");
	m_cmbStartStory.GetLBText(Index,strStartStory);
	
	CArray<CString, CString&> strStoryList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int nCount = strStoryList.GetSize();

	BOOL bAdd = FALSE;
	for(int i = 0; i < nCount; i++)
	{
		CString tempStoryList=_T("");
		tempStoryList = strStoryList.GetAt(i);
		if(strStartStory == tempStoryList)       bAdd = TRUE;
		if(bAdd)    m_cmbEndStory.AddString(tempStoryList);
	}

	// 정확히 일치하는 항목만 선택
	if(strEndStory)
	{
	    int nFindIndex = CB_ERR;
	    int nComboCount = m_cmbEndStory.GetCount();
	    
	    for(int i = 0; i < nComboCount; i++)
	    {
	        CString strComboText;
	        m_cmbEndStory.GetLBText(i, strComboText);
	        if(strComboText == *strEndStory)  // 정확히 일치하는지 비교
	        {
	            nFindIndex = i;
	            break;
	        }
	    }
	    
	    if(nFindIndex != CB_ERR)
	    {
	        m_cmbEndStory.SetCurSel(nFindIndex);
	    }
	}
	
	BOOL bCheck = m_chkSubWallID.GetCheck();
	if(bCheck)  m_edtSubWallID.SetWindowText(strStartStory);
}

void CDgnConWallNewDlg::AlignCtrls()
{
	BOOL bHide = TRUE;
#if defined(_US) || defined(_RUS)
	if(CProduct::IsMalaysia())    bHide = FALSE;
#endif
	
	if(bHide)
	{
		CArray<UINT,UINT> aHideCtrls;
		aHideCtrls.Add(IDC_DGN_USE_WELD_FAB_CHK);
		aHideCtrls.Add(IDC_DGN_USE_WELD_FAB_BTN);
		CDlgUtil::CtrlShowHide(this, aHideCtrls, FALSE);
	}
}

void CDgnConWallNewDlg::EnableDisableCtrls()
{
	BOOL bEnable = m_pDoc->m_pAttrCtrl->IsWallEndHorBarCondition();

	CArray<UINT,UINT> aBECtrls;
	aBECtrls.Add(IDC_WG_DGN_REBW_BE_LENG_STC);
	aBECtrls.Add(IDC_DGN_USE_MODEL_BELENG_EDT);
	aBECtrls.Add(IDC_DGN_USE_MODEL_BELENG_UNT);
	CDlgUtil::CtrlEnableDisable(this, aBECtrls, bEnable);
}

// List Ctrl 함수들
void CDgnConWallNewDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_REBW_D RebwD;
	
	T_REBW_KEY RebwKey;
	int nIndex = -1;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		if(!m_mIndex2RebwK.Lookup(nIndex, RebwKey))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(!GetSelectedItem(RebwKey))    return;
	}

	CArray<T_REBW_KEY,T_REBW_KEY> aRebwKey;
	int nSelected = GetSelectedItems(aRebwKey);

	if(nSelected == 1)
	{
		CString strSttStory = m_WallIDList.GetItemText(nIndex, 2);
		m_cmbStartStory.SelectString(0, strSttStory);
		CString strEndStory = m_WallIDList.GetItemText(nIndex, 3);
		InitEndStoryCombo(&strEndStory);
	}

	T_REBW_K RebwK;   RebwK.keymap = RebwKey;
	BOOL bEnableChk = RebwK.key.storid == 0;
	if(nSelected > 1)   bEnableChk = FALSE;
	m_chkSubWallID.EnableWindow(bEnableChk);
	
	if(!bEnableChk)  m_chkSubWallID.SetCheck(FALSE);
	OnChkSubWallID();

	if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD))
	{
		RebwD.Initialize();
		CDgnDataCtrl DataCtrl;
		T_DCRB_D DcrbD;    DcrbD.Initialize();
		DataCtrl.Get_DgnConDcrb(0, DcrbD);

		RebwD.VerticalRebarName = DcrbD.VerticalRebarWall[0];
		RebwD.HorizonRebarName = DcrbD.HorizontalRebarWall;
		RebwD.EndRebarName = DcrbD.EndRebarWall;
		RebwD.BEHorizonRebarName = DcrbD.BEHorizontalRebarWall;

		RebwD.dDw = M_InitValueCurUnit(50.8,N,MM,D_UNITSYS_BASE_LENGTH);
		RebwD.dDe = M_InitValueCurUnit(50.8,N,MM,D_UNITSYS_BASE_LENGTH);
	}

	CArray<T_REBW_K,T_REBW_K> aRebwK;
	aRebwK.SetSize(nSelected);
	for(int i = 0; i < nSelected; i++)
	{
		aRebwK[i].keymap = aRebwKey[i];
	}

	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	
	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTempK;
	for(int k = 0; k < nSelected; k++)
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyListUseRebw(aRebwK[k], aTempK);
		aElemK.Append(aTempK);
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE);

	m_RebarGrid.Data2Grid(&RebwD);

	m_edtDw.SetEditUnit(RebwD.dDw);
	m_edtDe.SetEditUnit(RebwD.dDe);
	
	m_chkUseModelThik.SetCheck(RebwD.bUseModelThik);
	OnChkUseModelThickness();
	m_edtUseModelThik.SetEditUnit(RebwD.dThickness, 3);

	m_chkUseWeldedFabfic.SetCheck(FALSE);
	OnChkUseWeldedFabric();

	m_edtBELength.SetEditUnit(RebwD.dBELength);
	*pResult = 0;
}

void CDgnConWallNewDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_WALL_ID),_LS(IDS_DGN_WALL_MARK), _LS(IDS_DGN_START_STORY), _LS(IDS_DGN_END_STORY), _LS(IDS_DGN_BAR)};
	int nColWidth[] = { globalUtils.ScaleByDPI(80), globalUtils.ScaleByDPI(80), globalUtils.ScaleByDPI(80), globalUtils.ScaleByDPI(80), globalUtils.ScaleByDPI(34)};
	int nColNum = 5;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	CRect rect;
	GetDlgItem(IDC_DGN_WALL_ID_LST)->GetWindowRect(rect);
	nColWidth[4] = rect.Width() - nColWidth[0] - nColWidth[1] - nColWidth[2] - nColWidth[3] - globalUtils.ScaleByDPI(4);
	
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_WallIDList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_WallIDList.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_WallIDList.InsertColumn(i,&lvcolumn);
		m_WallIDList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CDgnConWallNewDlg::GetSelectedItem(T_REBW_KEY& RebwKey)
{
	int iItem = m_WallIDList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;
	
	int nIndex = iItem;
	if(!m_mIndex2RebwK.Lookup(nIndex, RebwKey))     {  ASSERT(0);		return FALSE;	}

	return TRUE;
}

int CDgnConWallNewDlg::GetSelectedItems(CArray<T_REBW_KEY,T_REBW_KEY>& aRebwKey)
{
	aRebwKey.RemoveAll();

	POSITION pos = m_WallIDList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		while (pos)
		{
			int nItem = m_WallIDList.GetNextSelectedItem(pos);
			
			int nIndex = nItem;
			T_REBW_KEY RebwKey;
			if(!m_mIndex2RebwK.Lookup(nIndex, RebwKey))     {  ASSERT(0);		return FALSE;	}

			aRebwKey.Add(RebwKey);
		}
	}

	return aRebwKey.GetSize();
}

void CDgnConWallNewDlg::MakeItemEx()
{
	m_WallIDList.DeleteAllItems();
	m_mIndex2RebwK.RemoveAll();
	m_mRebwK2Index.RemoveAll();

	CArray<int,int> aWallID;
	m_pDoc->m_pAttrCtrl->GetWallIdList(aWallID);
	int nWallIDSize = aWallID.GetSize();
	if(nWallIDSize < 1)    return;

	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorSize = aStorK.GetSize();
	if(nStorSize < 1)   return;

	T_STOR_K botStorK = aStorK[0];
	T_STOR_K topStorK = aStorK[nStorSize-1];
	T_STOR_D StorD, sttStorD, endStorD;
	if(!m_pDoc->m_pAttrCtrl->GetStor(botStorK, StorD))      {  ASSERT(0);		return;	}
	CString botStorName = StorD.StoryName;
	if(!m_pDoc->m_pAttrCtrl->GetStor(topStorK, StorD))      {  ASSERT(0);		return;	}
	CString topStorName = StorD.StoryName;

	int nIndex = 0;
	for(int i = 0; i < nWallIDSize; i++)
	{
		int nWallID = aWallID[i];
		m_mIndex2RebwK.SetAt(nIndex, nWallID);
		m_mRebwK2Index.SetAt(nWallID, nIndex);
		
		T_WMAK_D WmakD;
		T_WMAK_K WmakK = m_pDoc->m_pAttrCtrl->GetWmakKey(nWallID);
		if(!m_pDoc->m_pAttrCtrl->GetWmak(WmakK, WmakD))      WmakD.Initialize();

		CString strRebar = _LSX(-);
		T_REBW_K RebwK;   RebwK.keymap = 0;   RebwK.key.wallid = nWallID;
		if(m_pDoc->m_pAttrCtrl->ExistRebw(RebwK))  strRebar = _LSX(In);

		CString strWallIDName;
		strWallIDName.Format(_T("%d"), nWallID);
		SetItem(nIndex++, strWallIDName, WmakD.MarkName, botStorName, topStorName, strRebar);
		
		CArray<T_REBW_K, T_REBW_K> aRebwK;
		int nRebwSize = m_pDoc->m_pAttrCtrl->GetRebwKeyListUsingWallID(nWallID, aRebwK);
		for(int k = 0; k < nRebwSize; k++)
		{
			T_REBW_K RebwK = aRebwK[k];
			T_REBW_D RebwD;
			if(!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD))     {  ASSERT(0);		continue;	}
			if(RebwK.key.storid == 0)   continue;
			m_mIndex2RebwK.SetAt(nIndex, RebwK.keymap);
			m_mRebwK2Index.SetAt(RebwK.keymap, nIndex);

			if(!m_pDoc->m_pAttrCtrl->GetStor(RebwK.key.storid, sttStorD))     {  ASSERT(0);		return ;	}
			int nRebwStorSize = RebwD.aStorK.GetSize();

			endStorD.Initialize();
			for(int m = nRebwStorSize-1; m >= 0; m--)
			{
				if(m_pDoc->m_pAttrCtrl->GetStor(RebwD.aStorK[m], endStorD))
					break;
			}
			
			CString strWallIDName;
			strWallIDName.Format(_T("%d-%s"), nWallID, sttStorD.StoryName);
			strRebar = _LSX(In);
			SetItem(nIndex++, strWallIDName, WmakD.MarkName, sttStorD.StoryName, endStorD.StoryName, strRebar);
		}
	}
	
	//  m_WallIDList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CDgnConWallNewDlg::SetItem(int nIndex, CString& strWallIDName, CString& strWallMark,
																CString& strSttStory, CString& strEndStory, CString& strRebar)
{
	m_WallIDList.InsertItem(nIndex, _T(""));

	LVITEM lvitem;
	CString strTemp;
	for(int i = 0; i < 5; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;

		switch(i)
		{
		case 0:   strTemp = strWallIDName;    break;
		case 1:   strTemp = strWallMark;      break;
		case 2:   strTemp = strSttStory;      break;
		case 3:   strTemp = strEndStory;      break;
		case 4:   strTemp = strRebar;         break;
		default:    ASSERT(0);  
		}

		lvitem.pszText=strTemp.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_WallIDList.SetItem(&lvitem);
		
		strTemp.ReleaseBuffer();
	}
}
