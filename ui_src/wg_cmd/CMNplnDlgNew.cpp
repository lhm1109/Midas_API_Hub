// CMNplnDlgNew.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNplnDlgNew.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlgNew

IMPLEMENT_DYNCREATE(CCMNplnDlgNew, MChildFormView)

CCMNplnDlgNew::CCMNplnDlgNew()
	: MChildFormView(CCMNplnDlgNew::IDD)
{
	//{{AFX_DATA_INIT(CCMNplnDlgNew)
		// NOTE: the ClassWizard will add member initialization here
	//{{AFX_DATA_MAP(CCMNplnDlg)
	//}}AFX_DATA_INIT
	m_nPlaneType  = 0;

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

CCMNplnDlgNew::~CCMNplnDlgNew()
{
}

void CCMNplnDlgNew::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNplnDlgNew)
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


BEGIN_MESSAGE_MAP(CCMNplnDlgNew, MChildFormView)
	//{{AFX_MSG_MAP(CCMNplnDlgNew)
	ON_BN_CLICKED(IDC_CMD_PT_3P, OnPlanTypeChange)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_NPLN_LIST, OnItemchangedCmdNplnList)
	ON_BN_CLICKED(IDC_CMD_PT_XY, OnPlanTypeChange)
	ON_BN_CLICKED(IDC_CMD_PT_XZ, OnPlanTypeChange)
	ON_BN_CLICKED(IDC_CMD_PT_YZ, OnPlanTypeChange)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_CMD_TOL_TITLE, &CCMNplnDlgNew::OnStnClickedCmdTolTitle)
	ON_EN_CHANGE(IDC_CMD_TOL_EDIT, &CCMNplnDlgNew::OnEnChangeCmdTolEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlgNew diagnostics

#ifdef _DEBUG
void CCMNplnDlgNew::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CCMNplnDlgNew::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMNplnDlgNew message handlers

void CCMNplnDlgNew::OnInitialUpdate() 
{
	MChildFormView::OnInitialUpdate();
	InitCtrls();
	AlignControls();
	MakeListHeader();
	MakeListContents();
	PlaneTypeCtrlMan(this->m_nPlaneType);
	UpdateData(FALSE);
}

void CCMNplnDlgNew::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st point 
	GetDlgItem(IDC_CMD_1ST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_XYZ_TITLE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	
	CDlgUtil::CtrlMoveDistY(this,m_aXYZCtrl,nDistY);

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

void CCMNplnDlgNew::InitCtrls()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
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
	csDefTol.Format(_T("%g"), pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit());
	m_wndTol.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTol.SetWindowText(csDefTol);
	m_wndTolUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndXYZ.SetAttWcsPos();
	m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
	m_wndXYZTol.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndXYZTol.SetWindowText(csDefTol);
	m_wndXYZTolUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndXYZUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CDlgUtil::CtrlShowHide(this,m_a3PtCtrl, m_nPlaneType == 0);
	CDlgUtil::CtrlShowHide(this,m_aXYZCtrl, m_nPlaneType != 0);
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);
}

void CCMNplnDlgNew::MakeListHeader()
{
	CStringArray HTitles;
	HTitles.Add(_LS(IDS_WG_CMD__ADDD__Plane_Name));
	HTitles.Add(_LS(IDS_WG_CMD__ADDD__Plane_Type));
	CArray<int,int> arW;
	arW.Add(114);
	arW.Add(90);
	CDlgUtil::_SetListCtrlHeader(&m_List,HTitles,NULL,&arW);
}

CString CCMNplnDlgNew::DataToStr(int i, T_NPLN_K key, T_NPLN_D &data)
{
	UNUSED(key);
	CString value;
	CString aPlaneTypeName[] = {_LS(IDS_WG_CMD__ADDD__3_Points), _LS(IDS_WG_CMD__ADDD__X_Y_Plane), _LS(IDS_WG_CMD__ADDD__X_Z_Plane), _LS(IDS_WG_CMD__ADDD__Y_Z_Plane)};
	if (i == 1) value = data.PlaneName;
	else if (i == 2) value = aPlaneTypeName[data.nPlaneType-1];
	return value;
}

void CCMNplnDlgNew::GetDataStringArr(T_NPLN_K key,CStringArray &arStr)
{
	T_NPLN_D data;

	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNpln(key,data)) return;

	CString value;
	CString aPlaneTypeName[] = {_LS(IDS_WG_CMD__ADDD__3_Points), _LS(IDS_WG_CMD__ADDD__X_Y_Plane), _LS(IDS_WG_CMD__ADDD__X_Z_Plane), _LS(IDS_WG_CMD__ADDD__Y_Z_Plane)};
	arStr.Add(data.PlaneName);
	arStr.Add(aPlaneTypeName[data.nPlaneType-1]);
}

static int CALLBACK 
__MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	 // lParamSort contains a pointer to the list view control.
	 // The lParam of an item is just its index.
	 CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	 CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
	 CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);
	 return _tcscmp(strItem2, strItem1);
}

void CCMNplnDlgNew::MakeListContents()
{
	m_List.DeleteAllItems();
	CDBDoc * pDoc = CDBDoc::GetDocPoint();

	int nNpln = pDoc->m_pAttrCtrl->GetCountNpln();
	if(nNpln == 0) return;
	
	CArray<T_NPLN_K,T_NPLN_K> NplnKeys;
	pDoc->m_pAttrCtrl->GetNplnKeyList(NplnKeys);
	nNpln = NplnKeys.GetSize();
	//qsort(NplnKeys.GetBuffer(),nNpln,sizeof(T_NPLN_K),CCompFunc::UINTAsc);
	CStringArray Contents;
	CString TStr;
	for(int i= 0; i< nNpln; i++)
	{
		Contents.RemoveAll();
		GetDataStringArr(NplnKeys[i],Contents);
		if(Contents.GetSize())
		{
			CDlgUtil::SetListItem(&m_List,i,Contents,(DWORD)NplnKeys[i]);
		}
	}
	
	//m_List.SortItems(__MyCompareProc, (LPARAM) &m_List);
}

void CCMNplnDlgNew::OnPlanTypeChange() 
{
	UpdateData();
	PlaneTypeCtrlMan(m_nPlaneType);
}

void CCMNplnDlgNew::PlaneTypeCtrlMan(int nPlaneType)
{
	UINT aID[] = {IDC_CMD_PT_3P, IDC_CMD_PT_XY, IDC_CMD_PT_XZ, IDC_CMD_PT_YZ};

	CDlgUtil::CtrlShowHide(this,m_a3PtCtrl, nPlaneType == 0);
	CDlgUtil::CtrlShowHide(this,m_aXYZCtrl, nPlaneType != 0);

	switch(nPlaneType)
	{
	case 0:
		m_wnd1stPt.SetFocus();
		break;
	case 1:
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Z_Position__));
		m_wndXYZ.SetFocus();
		break;
	case 2: //
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Y_Position__));
		m_wndXYZ.SetFocus();
		break;
	case 3: 
		m_wndXYZ.SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);
		m_wndXYZTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__X_Position__));
		m_wndXYZ.SetFocus();
		break;
	default:
		return;
	}
}

void CCMNplnDlgNew::SetNplnData(T_NPLN_K Key)
{
	//UINT aID[] = {IDC_CMD_PT_3P, IDC_CMD_PT_XY, IDC_CMD_PT_XZ, IDC_CMD_PT_YZ};
	/*
	int nPlaneType;
	double dPoint[3][3]; // nPlaneType = 1
	double dCoor;        // nPlaneType = 2, 3, 4;
	*/
	T_NPLN_D Data;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	if(pDoc->m_pAttrCtrl->GetNpln(Key,Data))
	{
		m_wndPlaneName.SetWindowText(Data.PlaneName);
		CString csTolerance, csVal;
		csTolerance.Format(_T("%g"), Data.dTolerance);
		m_wndTol.SetWindowText(csTolerance);
		m_wndXYZTol.SetWindowText(csTolerance);
		m_nPlaneType = Data.nPlaneType -1;

		PlaneTypeCtrlMan(m_nPlaneType);

		switch(m_nPlaneType)
		{
			case 0:  // 3Point 
				csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[0][0], Data.dPoint[0][1], Data.dPoint[0][2]);
				m_wnd1stPt.SetWindowText(csVal);
				csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[1][0], Data.dPoint[1][1], Data.dPoint[1][2]);
				m_wnd2ndPt.SetWindowText(csVal);
				csVal.Format(_LS(IDS_WG_CMD__ADDD___g___g___g), Data.dPoint[2][0], Data.dPoint[2][1], Data.dPoint[2][2]);
				m_wnd3rdPt.SetWindowText(csVal);
				break;
			case 1:  // XY Plane
				csVal.Format(_T("%g"), Data.dCoor);
				m_wndXYZ.SetWindowText(csVal);
				break;
			case 2:  // XZ Plane
				csVal.Format(_T("%g"), Data.dCoor);
				m_wndXYZ.SetWindowText(csVal);
				break;
			case 3:  // YZ Plane
				csVal.Format(_T("%g"), Data.dCoor);
				m_wndXYZ.SetWindowText(csVal);
				break;
			default:
				break;
		}
	}

	// PMS:XXXX-THBAE-20110520 : Named Plan Dynamic View
	pDoc->m_pViewCtrl->UnselectAll(NULL);
	
	CArray<T_ELEM_K,T_ELEM_K> aElemK_Npln;
	CArray<T_NODE_K,T_NODE_K> aNodeK_Npln;
	CArray<T_NPLN_K,T_NPLN_K> aNplnK_Temp;
	aNplnK_Temp.Add(Key);
	pDoc->m_pAttrCtrl->GetKeyListNplnUser(&aNplnK_Temp, &aNodeK_Npln, &aElemK_Npln);

	pDoc->m_pViewCtrl->SelectNode(NULL, aNodeK_Npln, TRUE, TRUE);
	pDoc->m_pViewCtrl->SelectElem(NULL, aElemK_Npln, TRUE, TRUE);

	UpdateData(FALSE);
}

BOOL CCMNplnDlgNew::GetCurNplnData(T_NPLN_D &Data)
{
	UpdateData();
	CString csValue;
	BOOL bSuccess = TRUE;
	m_wndPlaneName.GetWindowText(Data.PlaneName);
	Data.PlaneName.TrimLeft();
	Data.PlaneName.TrimRight();

	if(Data.PlaneName.IsEmpty())
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Missing_plane_name_));
		return FALSE;
	}
	Data.nPlaneType = m_nPlaneType + 1;
	double Tx,Ty,Tz;
	switch(m_nPlaneType)
	{
	case 0:  // 3Point 
		bSuccess = m_wnd1stPt.GetCoordData(&Tx,&Ty,&Tz);
		Data.dPoint[0][0] = Tx;Data.dPoint[0][1] = Ty;Data.dPoint[0][2] = Tz;
		bSuccess &= m_wnd2ndPt.GetCoordData(&Tx,&Ty,&Tz);
		Data.dPoint[1][0] = Tx;Data.dPoint[1][1] = Ty;Data.dPoint[1][2] = Tz;
		bSuccess &= m_wnd3rdPt.GetCoordData(&Tx,&Ty,&Tz);
		Data.dPoint[2][0] = Tx;Data.dPoint[2][1] = Ty;Data.dPoint[2][2] = Tz;
		Data.dTolerance = m_wndTol.GetEditValue();
		break;
	case 1:  // XY Plane 
		bSuccess = m_wndXYZ.GetCoordData(&Tx,&Ty,&Tz);
		Data.dCoor = Tz;
		Data.dTolerance = m_wndXYZTol.GetEditValue();
		break;
	case 2:  // XZ Plane
		bSuccess = m_wndXYZ.GetCoordData(&Tx,&Ty,&Tz);
		Data.dCoor = Ty;
		Data.dTolerance = m_wndXYZTol.GetEditValue();
		break;
	case 3:  // YZ Plane
		bSuccess = m_wndXYZ.GetCoordData(&Tx,&Ty,&Tz);
		Data.dCoor = Tx;
		Data.dTolerance = m_wndXYZTol.GetEditValue();
		break;
	default:
		break;
	}
	return bSuccess;
}

void CCMNplnDlgNew::SelectListItem(CString& csPlaneName)
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

void CCMNplnDlgNew::SelectListItem(int nIndex)
{
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CCMNplnDlgNew::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMNplnDlgNew::OnCmdBtnAdd() 
{
	T_NPLN_D data;
	//if (!Dlg2Item(data)) return;
	if(!GetCurNplnData(data)) return;
	if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddNpln(data))
		SelectListItem(data.PlaneName);
	
}


void CCMNplnDlgNew::OnCmdBtnDelete() 
{
	int nIndex;
	if (!GetSelectedItem(nIndex))	
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_named_plane_));
		return;
	}
	CString csPlaneName = m_List.GetItemText(nIndex, 0);
	if (CDBDoc::GetDocPoint()->m_pDataCtrl->DelNpln(csPlaneName))
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

void CCMNplnDlgNew::OnCmdBtnModify() 
{
	int nIndex;
	if (!GetSelectedItem(nIndex))	
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_named_plane_));
		return;
	}
	T_NPLN_D data;
	CString csPlaneName = m_List.GetItemText(nIndex, 0);
	if (!GetCurNplnData(data)) return;
	if (CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyNpln(csPlaneName, data))
		SelectListItem(data.PlaneName);
}

void CCMNplnDlgNew::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

	/*
	int nIndex;
	if (GetSelectedItem(nIndex))
	{
		CDBDoc *pDoc = CDBDoc::GetDocPoint();
		T_NPLN_K key = m_List.GetItemData(nIndex);
		T_NPLN_D data;
		if (pDoc->m_pAttrCtrl->GetNpln(key, data)) Item2Dlg(data);
		else GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Named_plane_key__d_does_n), key);
	}
	else
	{
		//T_NPLN_D data;
		//data.Initialize();
		//data.nPlaneType = 1;  // default=3points
		//Item2Dlg(data);
	}
	*/
}

void CCMNplnDlgNew::UpdateBuffer()
{

	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

//	T_NPLN_K Key;
	T_NPLN_D Data;
	BOOL bIsNpln = FALSE;

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
				//pViewBuff->GetNpln(nKey, Key, Data);
				//InsertItem(Key, Data);
				bIsNpln = TRUE;
			}
			break;
		case(UR_NPLN_DEL):
			{
				//pViewBuff->GetNpln(nKey, Key, Data);
				//DeleteItem(Key, Data);
				bIsNpln = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while

	if(bIsNpln)
	{
		MakeListContents();
	}
}

void CCMNplnDlgNew::OnItemchangedCmdNplnList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))	return;

	T_NPLN_K NplnK = (T_NPLN_K)this->m_List.GetItemData(nIndex);
	SetNplnData(NplnK);

	
	*pResult = 0;
}

void CCMNplnDlgNew::OnTmClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}


void CCMNplnDlgNew::OnStnClickedCmdTolTitle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCMNplnDlgNew::OnEnChangeCmdTolEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// __super::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

