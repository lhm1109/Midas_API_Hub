// CMBgenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBgenDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#include "..\wg_base\PL_StrParser.h"

#include "CMBgenTableDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMBgenDlg
#define COLCOUNT 7

IMPLEMENT_DYNCREATE(CCMBgenDlg, MChildFormView)

CCMBgenDlg::CCMBgenDlg()
	: MChildFormView(CCMBgenDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMBgenDlg)
	//}}AFX_DATA_INIT

//  m_bChangeUnit = TRUE;
}

CCMBgenDlg::~CCMBgenDlg()
{
}

void CCMBgenDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBgenDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_DIST_EDIT, m_wndDistEdit);
	DDX_Control(pDX, IDC_CMD_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_CMD_START_ELEM_NUM, m_StartElementNumber);
	DDX_Control(pDX, IDC_CMD_START_NODE_NUM, m_StartNodeNumber);
	DDX_Control(pDX, IDC_CMD_COPYNUM_TXT, m_edtCopyNum);
	DDX_Control(pDX, IDC_CMD_MATERIAL_INC_TXT, m_edtMatInc);
	DDX_Control(pDX, IDC_CMD_COL_INC_TXT, m_edtColInc);
	DDX_Control(pDX, IDC_CMD_BEAM_INC_TXT, m_edtBeamInc);
	DDX_Control(pDX, IDC_CMD_BRACE_INC_TXT, m_edtBraceInc);
	DDX_Control(pDX, IDC_CMD_WALL_INC_TXT, m_edtWallInc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMBgenDlg, MChildFormView)
	//{{AFX_MSG_MAP(CCMBgenDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnTmClose)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_COPYNUM_SPIN, OnDeltaposCmdCopynumSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MAT_SPIN, OnDeltaposCmdMatSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_COL_SPIN, OnDeltaposCmdColSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_BEAM_SPIN, OnDeltaposCmdBeamSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_BRACE_SPIN, OnDeltaposCmdBraceSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_WALL_SPIN, OnDeltaposCmdWallSpin)
	ON_BN_CLICKED(IDC_CMD_START_NODE_NUM_BTN, OnCmdStartNodeNumBtn)
	ON_BN_CLICKED(IDC_CMD_START_ELEM_NUM_BTN, OnCmdStartElemNumBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MERGE, OnCmdBtnMerge)
	ON_BN_CLICKED(IDC_CMD_COPYNODEDATA, OnCmdCopynodedata)
	ON_BN_CLICKED(IDC_CMD_COPYELEMDATA, OnCmdCopyelemdata)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnTmExecute)
	ON_BN_CLICKED(IDC_CMD_COPYBUILDINGDATA, OnCmdCopybuildingdata)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_CMD_LIST, OnItemclickCmdList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_CMD_BGENTABLE_BTN, OnCmdBgentableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMBgenDlg diagnostics

#ifdef _DEBUG
void CCMBgenDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CCMBgenDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMBgenDlg message handlers

void CCMBgenDlg::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc = CDBDoc::GetDocPoint();
	initEditValue();

	m_wndDistEdit.SetAttUcsDistance();
	m_wndDistEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDistEdit.SetDistValueMode(TRUE);

	m_wndDistEdit.SetWindowText(_T("0"));

	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	InitTitle();
	InitAttributeControl();

	m_edtCopyNum.SetRange(0, SHRT_MAX);
	m_edtCopyNum.SetInteger(TRUE);
	m_edtMatInc.SetRange(0, SHRT_MAX);
	m_edtMatInc.SetInteger(TRUE);
	m_edtColInc.SetRange(0, SHRT_MAX);
	m_edtColInc.SetInteger(TRUE);
	m_edtBeamInc.SetRange(0, SHRT_MAX);
	m_edtBeamInc.SetInteger(TRUE);
	m_edtBraceInc.SetRange(0, SHRT_MAX);
	m_edtBraceInc.SetInteger(TRUE);
	m_edtWallInc.SetRange(0, SHRT_MAX);
	m_edtWallInc.SetInteger(TRUE);

	ModifyFlag = FALSE;

	GetDlgItem(IDC_CMD_APPLY)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);
}


void CCMBgenDlg::OnTmClose()
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}



void CCMBgenDlg::OnDeltaposCmdCopynumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMUpDown->iDelta < 0)
		ControlSpin(0, TRUE);	
	else
		ControlSpin(0, FALSE);
	
	*pResult = 0;
}

void CCMBgenDlg::OnDeltaposCmdMatSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMUpDown->iDelta < 0)
		ControlSpin(1, TRUE);	
	else
		ControlSpin(1, FALSE);
	
	*pResult = 0;
}

void CCMBgenDlg::OnDeltaposCmdColSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMUpDown->iDelta < 0)
		ControlSpin(2, TRUE);	
	else
		ControlSpin(2, FALSE);
	
	*pResult = 0;
}

void CCMBgenDlg::OnDeltaposCmdBeamSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if(pNMUpDown->iDelta < 0)
		ControlSpin(3, TRUE);	
	else
		ControlSpin(3, FALSE);
	
	*pResult = 0;
}

void CCMBgenDlg::OnDeltaposCmdBraceSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if(pNMUpDown->iDelta < 0)
		ControlSpin(4, TRUE);	
	else
		ControlSpin(4, FALSE);
	
	*pResult = 0;
}

void CCMBgenDlg::OnDeltaposCmdWallSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	if(pNMUpDown->iDelta < 0)
		ControlSpin(5, TRUE);	
	else
		ControlSpin(5, FALSE);
	
	*pResult = 0;
}

//--------------------------------------------------------------------------
// Implementations

void CCMBgenDlg::ControlSpin(int i, BOOL Flag)  // Spin 버튼 작동하게 
{
	CString strTemp;
	UINT nID[] = { IDC_CMD_COPYNUM_TXT, IDC_CMD_MATERIAL_INC_TXT, IDC_CMD_COL_INC_TXT,
	             IDC_CMD_BEAM_INC_TXT, IDC_CMD_BRACE_INC_TXT, IDC_CMD_WALL_INC_TXT };

	CEdit* pWnd;
	pWnd = (CEdit *)this->GetDlgItem(nID[i]);
	pWnd->GetWindowText(strTemp);

	if(Flag)
		strTemp.Format(_T("%d"),_ttoi(strTemp)+1);	
	else
		switch(i)
		{
			case 0:                                   //Copy Number 는 최소한 1 이므로 
				if(_ttoi(strTemp) > 1)
					strTemp.Format(_T("%d"),_ttoi(strTemp)-1);	
				break;
	  	case 1:
			case 2:
			case 3:
			case 4:
			case 5:
		    if(_ttoi(strTemp) > 0)                   //나머지 항목은 0 부터 시작가능
				  strTemp.Format(_T("%d"),_ttoi(strTemp)-1);	
				break;
		}
	
	pWnd->SetWindowText(strTemp);	
}

void CCMBgenDlg::initEditValue()  //Edit 박스 초기값 지정
{
	UINT nID[] = { IDC_CMD_COPYNUM_TXT, IDC_CMD_MATERIAL_INC_TXT, IDC_CMD_COL_INC_TXT,
	             IDC_CMD_BEAM_INC_TXT, IDC_CMD_BRACE_INC_TXT, IDC_CMD_WALL_INC_TXT };
	CEdit* pWnd;
	for( int i=0; i<6; i++)
	{
	pWnd = (CEdit *)this->GetDlgItem(nID[i]);
	if(i == 0) 
		pWnd->SetWindowText(_T("1"));
	else 
		pWnd->SetWindowText(_T("0"));
	}
}

void CCMBgenDlg::OnCmdStartNodeNumBtn() 
{
	// TODO: Add your control notification handler code here
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->NodeNumberDlg();	
}

void CCMBgenDlg::OnCmdStartElemNumBtn() 
{
	// TODO: Add your control notification handler code here
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->ElemNumberDlg();	
}

void CCMBgenDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
//    UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
//    UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
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

void CCMBgenDlg::UpdateUnit() 
{
	ModifyTitle();
	
	int nUnitType = D_UNITSYS_BASE_LENGTH;
	double dblDistance;                                 

	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		dblDistance = _tstof(m_List.GetItemText(i,1));
		dblDistance = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitType, dblDistance);
		CString strTemp;
		strTemp.Format(_T("%.7g"),dblDistance);
		m_List.SetItemText(i,1,strTemp);
	}
}

void CCMBgenDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nRow;
	CString strDist;
	nRow = m_List.GetItemCount();

	CString strCopyNum;
	CString strMatInc;
	CString strColInc;
	CString strBeamInc;
	CString strBraceInc;
	CString strWallInc;

	m_edtCopyNum.GetWindowText(strCopyNum);
	m_edtMatInc.GetWindowText(strMatInc);
	m_edtColInc.GetWindowText(strColInc);
	m_edtBeamInc.GetWindowText(strBeamInc);
	m_edtBraceInc.GetWindowText(strBraceInc);
	m_edtWallInc.GetWindowText(strWallInc);

	int nIndex = m_List.InsertItem(nRow, strCopyNum);
	m_wndDistEdit.GetWindowText(strDist);
	m_List.SetItemText(nIndex,1, strDist);
	m_List.SetItemText(nIndex,2, strMatInc);
	m_List.SetItemText(nIndex,3, strColInc);
	m_List.SetItemText(nIndex,4, strBeamInc);
	m_List.SetItemText(nIndex,5, strBraceInc);
	m_List.SetItemText(nIndex,6, strWallInc);
}

void CCMBgenDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	ModifyFlag = TRUE;

	UpdateData(TRUE);

	CString strDist;
	LV_ITEM lvitem;
	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
			break;
	}

	CString strCopyNum;
	CString strMatInc;
	CString strColInc;
	CString strBeamInc;
	CString strBraceInc;
	CString strWallInc;

	m_edtCopyNum.GetWindowText(strCopyNum);
	m_edtMatInc.GetWindowText(strMatInc);
	m_edtColInc.GetWindowText(strColInc);
	m_edtBeamInc.GetWindowText(strBeamInc);
	m_edtBraceInc.GetWindowText(strBraceInc);
	m_edtWallInc.GetWindowText(strWallInc);

	m_wndDistEdit.GetWindowText(strDist);
	m_List.SetItemText(lvitem.iItem,0, strCopyNum);
	m_List.SetItemText(lvitem.iItem,1, strDist);
	m_List.SetItemText(lvitem.iItem,2, strMatInc);
	m_List.SetItemText(lvitem.iItem,3, strColInc);
	m_List.SetItemText(lvitem.iItem,4, strBeamInc);
	m_List.SetItemText(lvitem.iItem,5, strBraceInc);
	m_List.SetItemText(lvitem.iItem,6, strWallInc);

	ModifyFlag = FALSE;
}	

void CCMBgenDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nSelect=0;
	LV_ITEM lvitem;
	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			m_List.DeleteItem(i);
		}
	}

	if(!m_List.SetItemState(nSelect, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
		if(!m_List.SetItemState(nSelect-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
			m_List.SetItemState(-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}


void CCMBgenDlg::OnCmdBtnMerge() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CCMBgenDlg::OnCmdCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));	
}

void CCMBgenDlg::OnCmdCopyelemdata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_ELEM,0));	
}

void CCMBgenDlg::OnTmExecute()
{
	// TODO: Add your control notification handler code here
	// Grid 에 있는 데이타를 T_MAKEBUILDING 형식에 저장한다.
	arMakeBuilding.RemoveAll();
	T_MAKEBUILDING item;
	
	int iUntil = m_List.GetItemCount();
	for(int i=0; i < iUntil; i++)
	{
		item.nCopy = _ttoi(m_List.GetItemText(i,0));
		// [MQC 9273] Distance에 사칙연산 먹지 않는 오류 수정 : JWKWON - 2014-12-15
		CString strDist = m_List.GetItemText(i,1);
		if(!CStrParser::GetFloatNumber(strDist,item.dDistance))
		{
			ASSERT(0);
			item.dDistance = _tstof(m_List.GetItemText(i,1));
		}

		//item.dDistance = _tstof(m_List.GetItemText(i,1));
		item.nIncMatl = _ttoi(m_List.GetItemText(i,2));
		item.nIncColumn = _ttoi(m_List.GetItemText(i,3));
		item.nIncBeam = _ttoi(m_List.GetItemText(i,4));
		item.nIncBrace = _ttoi(m_List.GetItemText(i,5));
		item.nIncWall = _ttoi(m_List.GetItemText(i,6));

		arMakeBuilding.Add(item);	
	}
	
	// 선택된 NODE와 ELEMENT 키를 가져온다.
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;		

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE);
	pIGM->GetSelectedElemKeyList(aElemKey,TRUE);

	// NODE, ELEMENT, BUILDING 속성을 COPY 할지 안할지 정한다.
	BOOL CopyNodeAttr, CopyElemAttr, CopyBuildingAttr;

	CopyNodeAttr = this->IsDlgButtonChecked(IDC_CMD_COPY_NODEATTR_CHECK)? TRUE : FALSE;
	CopyElemAttr = this->IsDlgButtonChecked(IDC_CMD_COPY_ELEMATTR_CHECK)? TRUE : FALSE;
	CopyBuildingAttr = this->IsDlgButtonChecked(IDC_CMD_COPY_BUILDINGATTR_CHECK)? TRUE : FALSE;

	if(m_pDoc->m_pDataCtrl->MakeBuilding(arMakeBuilding, aNodeKey, aElemKey, CopyNodeAttr, CopyElemAttr, CopyBuildingAttr))
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);	
}

void CCMBgenDlg::OnCmdCopybuildingdata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_BLDG,0));	
}

void CCMBgenDlg::InitTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); 

	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Copy),_LS(IDS_WG_CMD__ADDD__Dist__)+csUnit+_T(")"),_LS(IDS_WG_CMD__ADDD__Mat_),_LS(IDS_WG_CMD__ADDD__Col_),_LS(IDS_WG_CMD__ADDD__Beam),_LS(IDS_WG_CMD__ADDD__Brace),_LS(IDS_WG_CMD__ADDD__Wall)};
	CStringArray aTitleTemp;
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Copy));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Dist__)+csUnit+_T(")"));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Mat_));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Col_));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Beam));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Brace));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Wall));

	int nColWidth[COLCOUNT]={45,70,45,45,50,50,45};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	int totalWidth = 0;	// 전체 컬럼 너비 계산
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitleTemp[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		totalWidth += lvcolumn.cx;
		lvcolumn.pszText = title.GetBuffer(title.GetLength());
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	// ListView에 스크롤 정보 직접 설정
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;

	CRect rect;
	m_List.GetClientRect(&rect);

	si.nMax = totalWidth;
	si.nPage = rect.Width();

	m_List.SetScrollInfo(SB_HORZ, &si, TRUE);
	m_List.ShowScrollBar(SB_HORZ, TRUE);
}

void CCMBgenDlg::ModifyTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); 

	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Dist__)+csUnit+_T(")")};
	int nColWidth[1]={70};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	title = aTitle[0];
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.iSubItem = 1;
	lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[0]);
	lvcolumn.pszText = title.GetBuffer(0);
	m_List.SetColumn(1,&lvcolumn);
	title.ReleaseBuffer();
}


void CCMBgenDlg::OnItemclickCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CCMBgenDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(!ModifyFlag)
	{
		CString strDist;
		LV_ITEM lvitem;
		int count = m_List.GetItemCount();
		lvitem.mask = LVIF_STATE;

		for(int i=count-1 ; i>=0; i--)
		{
			lvitem.iItem = i;
			if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
				break;
		}

		CString strCopyNum;
		CString strMatInc;
		CString strColInc;
		CString strBeamInc;
		CString strBraceInc;
		CString strWallInc;

		strCopyNum = m_List.GetItemText(lvitem.iItem,0);
		strDist = m_List.GetItemText(lvitem.iItem,1);
		strMatInc = m_List.GetItemText(lvitem.iItem,2);
		strColInc = m_List.GetItemText(lvitem.iItem,3);
		strBeamInc = m_List.GetItemText(lvitem.iItem,4);
		strBraceInc = m_List.GetItemText(lvitem.iItem,5);
		strWallInc = m_List.GetItemText(lvitem.iItem,6);

		m_wndDistEdit.SetWindowText(strDist);


		UpdateData(FALSE);
	}
	
	*pResult = 0;
}

void CCMBgenDlg::OnCmdBgentableBtn() 
{
	// TODO: Add your control notification handler code here
	CCMBgenTableDlg dlg;

	T_MAKEBUILDING tempBuilding;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		tempBuilding.nCopy = _ttoi(m_List.GetItemText(i,0));

		// [MQC 9273] Distance에 사칙연산 먹지 않는 오류 수정 : JWKWON - 2014-12-15
		CString strDist = m_List.GetItemText(i,1);
		if(!CStrParser::GetFloatNumber(strDist,tempBuilding.dDistance))
		{
			ASSERT(0);
			tempBuilding.dDistance = _tstof(m_List.GetItemText(i,1));
		}

		tempBuilding.nIncMatl = _ttoi(m_List.GetItemText(i,2));
		tempBuilding.nIncColumn = _ttoi(m_List.GetItemText(i,3));
		tempBuilding.nIncBeam = _ttoi(m_List.GetItemText(i,4));
		tempBuilding.nIncBrace = _ttoi(m_List.GetItemText(i,5));
		tempBuilding.nIncWall = _ttoi(m_List.GetItemText(i,6));

		for(int j=0; j<tempBuilding.nCopy; j++)
		{
			if(j==0)
			  dlg.m_arMakeBuilding.Add(tempBuilding);
			else
			{
				tempBuilding.nIncMatl = 0;
				tempBuilding.nIncColumn = 0;
				tempBuilding.nIncBeam = 0;
				tempBuilding.nIncBrace = 0;
				tempBuilding.nIncWall = 0;
			  dlg.m_arMakeBuilding.Add(tempBuilding);
			}
		}
	}
	//dlg.SetInitPos(D_INIT_POS_LB);

	CString tempstr;
	if(dlg.DoModal()==IDOK)
	{
		m_List.DeleteAllItems();
		int count = dlg.m_arMakeBuilding.GetSize();	

		for(int i=0; i<count; i++)
		{
			tempBuilding = dlg.m_arMakeBuilding.GetAt(i);
			tempstr.Format(_T("%d"),tempBuilding.nCopy);
			m_List.InsertItem(i, tempstr);
			tempstr.Format(_T("%g"),tempBuilding.dDistance);
			m_List.SetItemText(i,1, tempstr);
			tempstr.Format(_T("%d"),tempBuilding.nIncMatl);
			m_List.SetItemText(i,2, tempstr);
			tempstr.Format(_T("%d"),tempBuilding.nIncColumn);
			m_List.SetItemText(i,3, tempstr);
			tempstr.Format(_T("%d"),tempBuilding.nIncBeam);
			m_List.SetItemText(i,4, tempstr);
			tempstr.Format(_T("%d"),tempBuilding.nIncBrace);
			m_List.SetItemText(i,5, tempstr);
			tempstr.Format(_T("%d"),tempBuilding.nIncWall);
			m_List.SetItemText(i,6, tempstr);
		}
	}
}

void CCMBgenDlg::InitAttributeControl()
{
	UINT uAttribControl[] = {IDC_CMD_COPY_NODEATTR_CHECK, 
													 IDC_CMD_COPY_ELEMATTR_CHECK,
													 IDC_CMD_COPY_BUILDINGATTR_CHECK };

	for(int i=0; i<3; i++)
		((CButton *)GetDlgItem(uAttribControl[i]))->SetCheck(TRUE);
}
