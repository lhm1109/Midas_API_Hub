// CMNplnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNplnDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlg
#define CFormView CCMDlgBarBase

IMPLEMENT_DYNCREATE(CCMNplnDlg, CFormView)

CCMNplnDlg::CCMNplnDlg()
	: CFormView(CCMNplnDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMNplnDlg)
	m_nPlaneType = 0;
	//}}AFX_DATA_INIT
	m_aXYZCtrl.Add(IDC_CMD_XYZ_TITLE);
	m_aXYZCtrl.Add(IDC_CMD_XYZ_EDIT);
	m_aXYZCtrl.Add(IDC_CMD_XYZ_UNIT);
	m_aXYZCtrl.Add(IDC_CMD_XYZ_TOL_TITLE);
	m_aXYZCtrl.Add(IDC_CMD_XYZ_TOL_EDIT);
	m_aXYZCtrl.Add(IDC_CMD_XYZ_TOL_UNIT);

	m_a3PtCtrl.Add(IDC_CMD_1ST_TITLE);
	m_a3PtCtrl.Add(IDC_CMD_1ST_EDIT);
	m_a3PtCtrl.Add(IDC_CMD_1ST_UNIT);
	m_a3PtCtrl.Add(IDC_CMD_2ND_TITLE);
	m_a3PtCtrl.Add(IDC_CMD_2ND_EDIT);
	m_a3PtCtrl.Add(IDC_CMD_2ND_UNIT);
	m_a3PtCtrl.Add(IDC_CMD_3RD_TITLE);
	m_a3PtCtrl.Add(IDC_CMD_3RD_EDIT);
	m_a3PtCtrl.Add(IDC_CMD_3RD_UNIT);
	m_a3PtCtrl.Add(IDC_CMD_TOL_TITLE);
	m_a3PtCtrl.Add(IDC_CMD_TOL_EDIT);
	m_a3PtCtrl.Add(IDC_CMD_TOL_UNIT);
}

CCMNplnDlg::~CCMNplnDlg()
{
}

void CCMNplnDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNplnDlg)
	DDX_Control(pDX, IDC_CMD_PLANE_NAME, m_wndPlaneName);
	DDX_Control(pDX, IDC_CMD_NPLN_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_XYZ_TITLE, m_wndXYZTitle);
	DDX_Control(pDX, IDC_CMD_XYZ_TOL_UNIT, m_wndXYZTolUnit);
	DDX_Control(pDX, IDC_CMD_XYZ_TOL_EDIT, m_wndXYZTol);
	DDX_Control(pDX, IDC_CMD_XYZ_UNIT, m_wndXYZUnit);
	DDX_Control(pDX, IDC_CMD_XYZ_EDIT, m_wndXYZ);
	DDX_Control(pDX, IDC_CMD_TOL_UNIT, m_wndTolUnit);
	DDX_Control(pDX, IDC_CMD_TOL_EDIT, m_wndTol);
	DDX_Control(pDX, IDC_CMD_3RD_UNIT, m_wnd3rdUnit);
	DDX_Control(pDX, IDC_CMD_2ND_UNIT, m_wnd2ndUnit);
	DDX_Control(pDX, IDC_CMD_1ST_UNIT, m_wnd1stUnit);
	DDX_Control(pDX, IDC_CMD_3RD_EDIT, m_wnd3rdPt);
	DDX_Control(pDX, IDC_CMD_2ND_EDIT, m_wnd2ndPt);
	DDX_Control(pDX, IDC_CMD_1ST_EDIT, m_wnd1stPt);
	DDX_Radio(pDX, IDC_CMD_PT_3P, m_nPlaneType);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlg implementations

void CCMNplnDlg::CtrlMoveDistY(CArray<UINT, UINT> &aCtrlID, int nDistY)
{
	CWnd* pWnd;
	CRect r;
	CSize p(0, nDistY);
	int nNum = aCtrlID.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		pWnd = GetDlgItem(aCtrlID[i]);
		pWnd->GetWindowRect(r);
		ScreenToClient(r);
		r += p;
		pWnd->MoveWindow(r);
	}
}

void CCMNplnDlg::CtrlEnableDisable(CArray<UINT,UINT>&CtrlArray,BOOL bIsEnable)
{
	CWnd * pWnd;
	int nSize = CtrlArray.GetSize();
	for(int i = 0 ; i < nSize; i++)
	{
		pWnd = GetDlgItem(CtrlArray[i]);
		if(pWnd)
			pWnd->EnableWindow(bIsEnable);
	}
}

void CCMNplnDlg::CtrlShowHide(CArray<UINT,UINT>&CtrlArray,BOOL bIsShow)
{
	CWnd * pWnd;
	int nSize = CtrlArray.GetSize();
	for(int i = 0 ; i < nSize ; i++)
	{
		pWnd = GetDlgItem(CtrlArray[i]);
		if(pWnd)
		{
			if(bIsShow)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}
}


void CCMNplnDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st point 
	GetDlgItem(IDC_CMD_1ST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_XYZ_TITLE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_aXYZCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_CLOSE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;

	GetWindowRect(r);
	r.bottom = rectLast.bottom+5;
	ScreenToClient(r);
	MoveWindow(r);
	CSize size = GetTotalSize();
	size.cy = r.bottom;
	SetScrollSizes(MM_TEXT, size);
}

void CCMNplnDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Plane_Name), _LS(IDS_WG_CMD__ADDD__Plane_Type)};
	CStringArray aTitleTemp;
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Plane_Name));
	aTitleTemp.Add(_LS(IDS_WG_CMD__ADDD__Plane_Type));
	int nColWidth[] = {114, 90};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitleTemp[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(title.GetLength());
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMNplnDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMNplnDlg::SetItem(int nIndex, T_NPLN_K Key, T_NPLN_D &Data)
{
	CString str;
	LVITEM lvitem;
	lvitem.iItem = nIndex;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			lvitem.mask = LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
		  m_List.SetItem(&lvitem);
		}
		else 
		{
			lvitem.iSubItem=i-1;
			str = DataToStr(i, Key, Data);
			lvitem.pszText=str.GetBuffer(0);
			lvitem.mask=LVIF_TEXT;
		  m_List.SetItem(&lvitem);
			str.ReleaseBuffer();
		}
	}
}

void CCMNplnDlg::InsertItem(T_NPLN_K Key, T_NPLN_D &Data)
{
	// 삽입 위치를 찾는다.
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemText(nCount, 0).Compare(Data.PlaneName) > 0) break;
	}
	m_List.InsertItem(nCount, _T(""));
	SetItem(nCount, Key, Data);
}

void CCMNplnDlg::DeleteItem(T_NPLN_K Key, T_NPLN_D &Data)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemText(nCount, 0).Compare(Data.PlaneName) == 0) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error____s_not_found_), Data.PlaneName);
	else m_List.DeleteItem(nCount);
}

void CCMNplnDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountNpln();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_NPLN_K Key;
		T_NPLN_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartNpln();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextNpln(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_NPLN_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetNpln(Key,Data);
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key, Data);
		}
		delete []KeyBuf;
	}
	m_List.SortItems(CompareByPlaneName, 0);
}

CString CCMNplnDlg::DataToStr(int i, T_NPLN_K key, T_NPLN_D &data)
{
	key; // Not used
	CString value;
	CString aPlaneTypeName[] = {_LS(IDS_WG_CMD__ADDD__3_Points), _LS(IDS_WG_CMD__ADDD__X_Y_Plane), _LS(IDS_WG_CMD__ADDD__X_Z_Plane), _LS(IDS_WG_CMD__ADDD__Y_Z_Plane)};
	if (i == 1) value = data.PlaneName;
	else if (i == 2) value = aPlaneTypeName[data.nPlaneType-1];
	return value;
}

BOOL CCMNplnDlg::GetPosition(CString& strPos, double& Px ,double& Py ,double& Pz)
{
	MyParser Parser;
	if(Parser.ParsingPositionArg(strPos))
	{
		Parser.GetPositionData(Px,Py,Pz);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCMNplnDlg::Dlg2Item(T_NPLN_D &Data)
{
	CString csValue;
	BOOL bSuccess;
	int nPos = 0;
	Data.Initialize();
	m_wndPlaneName.GetWindowText(Data.PlaneName);
	if (Data.PlaneName.IsEmpty())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Missing_plane_name_));
		return FALSE;
	}
	Data.nPlaneType = m_nPlaneType + 1;
	switch(m_nPlaneType)
	{
	case 0: // 3 Points
		m_wnd1stPt.GetWindowText(csValue);
		bSuccess = GetPosition(csValue, Data.dPoint[0][0], Data.dPoint[0][1], Data.dPoint[0][2]);
		if (!bSuccess) nPos = 1;
		m_wnd2ndPt.GetWindowText(csValue);
		bSuccess = GetPosition(csValue, Data.dPoint[1][0], Data.dPoint[1][1], Data.dPoint[1][2]);
		if (!bSuccess) nPos = 2;
		m_wnd3rdPt.GetWindowText(csValue);
		bSuccess = GetPosition(csValue, Data.dPoint[2][0], Data.dPoint[2][1], Data.dPoint[2][2]);
		if (!bSuccess) nPos = 3;
		if (nPos != 0)
		{
			CString aPointName[] = {_LS(IDS_WG_CMD__ADDD__1st), _LS(IDS_WG_CMD__ADDD__2nd), _LS(IDS_WG_CMD__ADDD__3rd)};
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid__s_point_), aPointName[nPos-1]);
			return FALSE;
		}
		Data.dTolerance = m_wndTol.GetEditValue();
		break;
	case 1: case 2: case 3: // X-Y, X-Z, Y-Z Plane
		m_wndXYZ.GetWindowText(csValue);
		if(!CStrParser::GetFloatNumber(csValue, Data.dCoor)) return FALSE;
//    Data.dCoor = _tstof(csValue);
		Data.dTolerance = m_wndXYZTol.GetEditValue();
		break;
	}
	return TRUE;
}

BOOL CCMNplnDlg::Item2Dlg(T_NPLN_D &Data)
{
	m_wndPlaneName.SetWindowText(Data.PlaneName);
	UINT nID;
	UINT aID[] = { IDC_CMD_PT_3P, IDC_CMD_PT_XY, IDC_CMD_PT_XZ, IDC_CMD_PT_YZ };
	nID = aID[Data.nPlaneType-1];
	CString csTolerance, csVal;
	csTolerance.Format(_T("%g"), Data.dTolerance);
	m_wndTol.SetWindowText(csTolerance);
	m_wndXYZTol.SetWindowText(csTolerance);
	ChangePlaneType(nID);
	switch(Data.nPlaneType)
	{
	case 1: 
		csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[0][0], Data.dPoint[0][1], Data.dPoint[0][2]);
		m_wnd1stPt.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[1][0], Data.dPoint[1][1], Data.dPoint[1][2]);
		m_wnd2ndPt.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[2][0], Data.dPoint[2][1], Data.dPoint[2][2]);
		m_wnd3rdPt.SetWindowText(csVal);
		break;  
	case 2: case 3: case 4: 
		csVal.Format(_T("%g"), Data.dCoor);
		m_wndXYZ.SetWindowText(csVal);
		break;
	}
	return TRUE;
}

void CCMNplnDlg::SelectListItem(CString& csPlaneName)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemText(nCount, 0).Compare(csPlaneName) == 0) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error____s_not_found_), csPlaneName);
	else SelectListItem(nCount);
}

void CCMNplnDlg::SelectListItem(int nIndex)
{
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNplnDlg::ChangePlaneType(UINT nID)
{
	UINT aID[] = {IDC_CMD_PT_3P, IDC_CMD_PT_XY, IDC_CMD_PT_XZ, IDC_CMD_PT_YZ};
	
	//for (int i = 0; i < 4; i++)
	//  ((CButton*)GetDlgItem(aID[i]))->SetCheck(FALSE);
	//((CButton*)GetDlgItem(nID))->SetCheck(TRUE);
	this->m_nPlaneType = nID;
	UpdateData(FALSE);
	
	CtrlShowHide(m_a3PtCtrl, m_nPlaneType == 0);
	CtrlShowHide(m_aXYZCtrl, m_nPlaneType != 0);

	switch(nID)
	{
	case IDC_CMD_PT_3P: 
		m_nPlaneType = 0; 
		//m_wnd1stPt.SetFocus();
		break;
	case IDC_CMD_PT_XY: 
		m_nPlaneType = 1; 
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Z_Position__));
		//m_wndXYZ.SetFocus();
		break;
	case IDC_CMD_PT_XZ: 
		m_nPlaneType = 2; 
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Y_Position__));
		//m_wndXYZ.SetFocus();
		break;
	case IDC_CMD_PT_YZ: 
		m_nPlaneType = 3; 
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__X_Position__));
		//m_wndXYZ.SetFocus();
		break;
	default:
		return;
	}


}

int CALLBACK CCMNplnDlg::CompareByPlaneName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	T_NPLN_D npln1, npln2;
	pDoc->m_pAttrCtrl->GetNpln(lParam1, npln1);
	pDoc->m_pAttrCtrl->GetNpln(lParam2, npln2);
	return npln1.PlaneName.Compare(npln2.PlaneName);
}

BEGIN_MESSAGE_MAP(CCMNplnDlg, CFormView)
	//{{AFX_MSG_MAP(CCMNplnDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_PT_3P, OnChangePlaneType)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_PT_XY, OnChangePlaneType)
	ON_BN_CLICKED(IDC_CMD_PT_XZ, OnChangePlaneType)
	ON_BN_CLICKED(IDC_CMD_PT_YZ, OnChangePlaneType)
	ON_NOTIFY(NM_CLICK       , IDC_CMD_NPLN_LIST, OnClickCmdNplnList)
	ON_NOTIFY(NM_DBLCLK      , IDC_CMD_NPLN_LIST, OnDblclkCmdNplnList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_NPLN_LIST, OnChangedCurrentItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlg diagnostics

#ifdef _DEBUG
void CCMNplnDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CCMNplnDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlg message handlers

void CCMNplnDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	AlignControls();

	m_wnd1stPt.SetAttWcsPos();
	m_wnd1stPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd2ndPt.SetAttWcsPos();
	m_wnd2ndPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd3rdPt.SetAttWcsPos();
	m_wnd3rdPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd1stUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd2ndUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd3rdUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd1stPt.SetWindowText(_LS(IDS_WG_CMD__ADDD__0_0_0));
	m_wnd2ndPt.SetWindowText(_LS(IDS_WG_CMD__ADDD__0_0_0));
	m_wnd3rdPt.SetWindowText(_LS(IDS_WG_CMD__ADDD__0_0_0));
	m_wnd1stPt.SetNextLink(&m_wnd2ndPt);
	m_wnd2ndPt.SetNextLink(&m_wnd3rdPt);
	m_wnd3rdPt.SetNextLink(NULL);

	CString csDefTol;
	csDefTol.Format(_T("%g"), m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit());
	m_wndTol.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTol.SetWindowText(csDefTol);
	m_wndTolUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndXYZ.SetAttWcsPos();
	m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
	m_wndXYZTol.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndXYZTol.SetWindowText(csDefTol);
	m_wndXYZTolUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CtrlShowHide(m_a3PtCtrl, m_nPlaneType == 0);
	CtrlShowHide(m_aXYZCtrl, m_nPlaneType != 0);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();

	UpdateData(FALSE);
}

void CCMNplnDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMNplnDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}

	int nIndex;
	if (GetSelectedItem(nIndex))
	{
		T_NPLN_K key = m_List.GetItemData(nIndex);
		T_NPLN_D data;
		if (m_pDoc->m_pAttrCtrl->GetNpln(key, data)) Item2Dlg(data);
		else GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Named_plane_key__d_does_n), key);
	}
	else
	{
		/*
		T_NPLN_D data;
		data.Initialize();
		data.nPlaneType = 1;  // default=3points
		Item2Dlg(data);
		*/
	}
}

void CCMNplnDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_NPLN_K Key;
	T_NPLN_D Data;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_NPLN_ADD):
			{
				pViewBuff->GetNpln(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_NPLN_DEL):
			{
				pViewBuff->GetNpln(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		default:
			break;
		}
	} // end of while
}

void CCMNplnDlg::OnChangePlaneType() // Combo !
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	ChangePlaneType(nID);
	switch(nID)
	{
	case IDC_CMD_PT_3P: 
		m_wnd1stPt.SetFocus();
		break;
	case IDC_CMD_PT_XY:
	case IDC_CMD_PT_YZ:
	case IDC_CMD_PT_XZ:
		m_wndXYZ.SetFocus();
		break;
	default:
		break;
	}
}

void CCMNplnDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_NPLN_D data;
	if (!Dlg2Item(data)) return;
	if (m_pDoc->m_pDataCtrl->AddNpln(data))
		SelectListItem(data.PlaneName);
}

void CCMNplnDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))	
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_named_plane_));
		return;
	}
	CString csPlaneName = m_List.GetItemText(nIndex, 0);
	if (m_pDoc->m_pDataCtrl->DelNpln(csPlaneName))
	{
		int nCount = m_List.GetItemCount();
		if (nIndex >= nCount) nIndex--;
		SelectListItem(nIndex);
	}
	else
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Fail_to_delete__s), csPlaneName);
	}
}

void CCMNplnDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))	
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_named_plane_));
		return;
	}
	T_NPLN_D data;
	CString csPlaneName = m_List.GetItemText(nIndex, 0);
	if (!Dlg2Item(data)) return;
	if (m_pDoc->m_pDataCtrl->ModifyNpln(csPlaneName, data))
		SelectListItem(data.PlaneName);
}

void CCMNplnDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return;
	*pResult = 0;
	ShowCurNpnlData();
}

void CCMNplnDlg::ShowCurNpnlData()
{
	int nIndex;
	if (!GetSelectedItem(nIndex))	return;
	
	T_NPLN_K key;
	T_NPLN_D data;
	key = m_List.GetItemData(nIndex);
	if (!m_pDoc->m_pAttrCtrl->GetNpln(key, data))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Named_plane_key__d_does_n), key);
		return;
	}
	Item2Dlg(data);
}

BOOL CCMNplnDlg::ExternalInit(UINT key)
{
/*
	if(key == 0) return FALSE;

	T_NPLN_D data;

	m_pDoc->m_pAttrCtrl->GetNpln(key, data);
	Item2Dlg(data);

	UpdateData(FALSE);
*/
	return TRUE;
}

void CCMNplnDlg::OnClickCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	//ShowCurNpnlData();
}

void CCMNplnDlg::OnDblclkCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	//ShowCurNpnlData();
}
