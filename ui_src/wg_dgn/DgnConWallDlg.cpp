// DgnConWallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "DgnDataCtrl.h"

#include "DgnConWeldMeshDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDlg dialog


CDgnConWallDlg::CDgnConWallDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWallDlg)
	m_bEndCheck = FALSE;
	m_bUseMt = FALSE;
	m_EndRebarNo = 0;
	m_strWallMark = _T("");
	//}}AFX_DATA_INIT
	m_bWallMarkFocus=FALSE;
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallDlg)
	DDX_Control(pDX, IDC_DGN_WALL_VREBARSPACE, m_VRebarSpace);
	DDX_Control(pDX, IDC_STATIC_VREBARUNIT, m_VRebarUnit);
	DDX_Control(pDX, IDC_DGN_WALL_HREBARSPACE, m_HRebarSpace);
	DDX_Control(pDX, IDC_DGN_WALL_HREBARSIZE, m_HRebarSize);
	DDX_Control(pDX, IDC_STATIC_HREBARUNIT, m_HRebarUnit);
	DDX_Control(pDX, IDC_DGN_WALLCHECK_LIST, m_WallCheckList);
	DDX_Control(pDX, IDC_DGN_RC_WALLID, m_IdList);
	DDX_Control(pDX, IDC_DGN_WALL_ENDREBARSIZE, m_EndRebarSize);
	DDX_Control(pDX, IDC_DGN_WALL_ENDSPIN, m_EndSpin);
	DDX_Control(pDX, IDC_DGN_WALL_VREBARSIZE, m_VRebarSize);
	DDX_Control(pDX, IDC_DGN_WALL_ESPACE, m_ESpace);
	DDX_Control(pDX, IDC_DGN_WALL_ESPACEUNIT, m_ESpaceUnit);
	DDX_Control(pDX, IDC_DGN_RC_WTHICK, m_wT);
	DDX_Control(pDX, IDC_DGN_RC_WDW, m_wDw);
	DDX_Control(pDX, IDC_DGN_RC_WDE, m_wDe);
	DDX_Control(pDX, IDC_DGN_RC_WDEUNIT, m_wDeUnit);
	DDX_Control(pDX, IDC_DGN_RC_WDWUNIT, m_wDwUnit);
	DDX_Control(pDX, IDC_DGN_RC_WTUNIT, m_wTUnit);
	DDX_Control(pDX, IDC_DGN_RC_WESTORY, m_wEstory);
	DDX_Control(pDX, IDC_DGN_RC_WSSTORY, m_wSstory);
	DDX_Check(pDX, IDC_DGN_END_CHECK, m_bEndCheck);
	DDX_Check(pDX, IDC_DGN_RC_USEMT, m_bUseMt);
	DDX_Text(pDX, IDC_DGN_WALL_EBDREBARNO, m_EndRebarNo);
	DDX_Text(pDX, IDC_DGN_RC_WALLMARK2, m_strWallMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConWallDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConWallDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_CBN_SELCHANGE(IDC_DGN_RC_WSSTORY, OnSelchangeDgnRcWsstory)
	ON_BN_CLICKED(IDC_DGN_END_CHECK, OnDgnEndCheck)
	ON_BN_CLICKED(IDC_DGN_RC_USEMT, OnDgnRcUsemt)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_WALL_ENDSPIN, OnDeltaposDgnWallEndspin)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_WALLCHECK_LIST, OnItemchangedDgnWallcheckList)
	ON_BN_CLICKED(IDC_DGN_WALL_ADD, OnDgnWallAdd)
	ON_BN_CLICKED(IDC_DGN_WALL_DELETE, OnDgnWallDelete)
	ON_BN_CLICKED(IDC_DGN_WALL_MODIFY, OnDgnWallModify)
	ON_CBN_SETFOCUS(IDC_DGN_RC_WSSTORY, OnSetfocusDgnRcWsstory)
	ON_CBN_SETFOCUS(IDC_DGN_RC_WESTORY, OnSetfocusDgnRcWestory)
	ON_EN_SETFOCUS(IDC_DGN_RC_WALLID, OnSetfocusDgnRcWallid)
	ON_BN_CLICKED(IDC_DGN_CON_FABRIC, OnDgnFabric)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,         OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE,        OnNodeListEscape)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDlg message handlers

void CDgnConWallDlg::OnDgnWallAdd() 
{
	BOOL bCheck;
	UpdateData(TRUE);
	if(AddErrorCheck())
	{
		CArray<T_REBW_K,T_REBW_K> rKey;
		//  Wall Rebar Data를 저장합니다.
		T_REBW_D rData;
		rData.Initialize();

		rData.dDe = m_De;
		rData.dDw = m_Dw;
		rData.bUseModelThik  = m_bUseMt;
		rData.dThickness     = m_Thickness;
		rData.EndRebarName   = m_ESize;
		rData.nNumEndRebar   = m_ENo;
		rData.dEndRebarSpace = m_EndSpace;
		rData.dVerticalRebarSpace = m_VSpace;
		rData.VerticalRebarName   = m_VSize;
		rData.dHorizonRebarSpace  = m_HSpace;
		rData.HorizonRebarName    = m_HSize;

		// Wall ID들을 잘라서 하나의 Wall ID로 만들자 !!!
		CString str;
		m_IdList.GetWindowText(str);     
		CArray<unsigned int,unsigned int> List;
		CStrParser::GetNodeList(str,List);
		int IdCount = List.GetSize();
		for(int i=0; i<IdCount; i++)
		{
			T_REBW_K key;
			key.key.wallid = List.GetAt(i);
			CString strID;
			strID.Format(_T("%d"),key.key.wallid);

			//  선택된 층의 Key를 찾아옵니다.
			CArray<int,int> sKeyList;
			Search_SelStoryKey(m_wSstoryName,m_wEstoryName,sKeyList);

			int KeyCount = sKeyList.GetSize();
			for(int j=0; j<KeyCount; j++)
			{
				key.key.storid = sKeyList.GetAt(j);
				rKey.Add(key);
			}
		}
		bCheck = m_pDoc->m_pDataCtrl->AddRebw(rKey,rData);
		if(bCheck)
		{
			Initial_SelectionData();
			m_IdList.ClearIDList();
		}
	}
}

void CDgnConWallDlg::OnDgnWallDelete() 
{
	///////////////////////////////////////////////////////////////////////
	//  Multi Selection 일때 사용합니다.
	POSITION pos = m_WallCheckList.GetFirstSelectedItemPosition();
	if(pos==NULL)	AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM),MB_OK);
	else
	{
		CArray<T_REBW_K,T_REBW_K> tKey;
		while (pos)
		{
			int nItem = m_WallCheckList.GetNextSelectedItem(pos);

			CString strID    = m_WallCheckList.GetItemText(nItem,0);
			CString strStory = m_WallCheckList.GetItemText(nItem,1);

			T_REBW_K rKey;
			rKey.key.wallid = _ttoi(strID);
			rKey.key.storid = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);

			tKey.Add(rKey);
		}
	  if(m_pDoc->m_pDataCtrl->DelRebw(tKey))
		{
			Initial_SelectionData();
			m_IdList.ClearIDList();
		}
	}
	///////////////////////////////////////////////////////////////////////
}

BOOL CDgnConWallDlg::CheckSelectOption()
{
	BOOL bCheck=TRUE;

	CString str;
	m_IdList.GetWindowText(str);
	if(str == _T(""))
	{
		bCheck = FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_WALL_ID),MB_OK);
		return bCheck;
	}
	int sIndex = m_wSstory.GetCurSel();
	if(sIndex == -1)
	{
		bCheck = FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_START_STORY),MB_OK);
		return bCheck;
	}
	int eIndex = m_wEstory.GetCurSel();
	if(eIndex == -1)
	{
		bCheck = FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_END_STORY),MB_OK);
		return bCheck;
	}

	return bCheck;
}

void CDgnConWallDlg::OnDgnWallModify() 
{
	/////////////////////////////////////////////////////////////////////////
	//  Multi Selection 일때 사용합니다.
	POSITION pos = m_WallCheckList.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		// Select된 Item이 없으므로 Select Option에서 선택된 내용으로 Rebar Data를 Update합니다.
		UpdateData(TRUE);
		if(CheckSelectOption())
		{
			if(AddErrorCheck())
			{
				CArray<T_REBW_K,T_REBW_K> rKey;

				T_REBW_D rData;
				rData.Initialize();

				rData.dDe = m_De;
				rData.dDw = m_Dw;
				rData.bUseModelThik  = m_bUseMt;
				rData.dThickness     = m_Thickness;
				rData.EndRebarName   = m_ESize;
				rData.nNumEndRebar   = m_ENo;
				rData.dEndRebarSpace = m_EndSpace;
				rData.dVerticalRebarSpace = m_VSpace;
				rData.VerticalRebarName   = m_VSize;
				rData.dHorizonRebarSpace  = m_HSpace;
				rData.HorizonRebarName    = m_HSize;

				//  Wall ID들을 잘라서 하나의 Wall ID로 만들자 !!!
				CString str;
				m_IdList.GetWindowText(str);     
		    CArray<unsigned int,unsigned int> List;
		    CStrParser::GetNodeList(str,List);
				int IdCount = List.GetSize();
				for(int i=0; i<IdCount; i++)
				{
					T_REBW_K key;
					key.key.wallid = List.GetAt(i);
					CString strID;
					strID.Format(_T("%d"),key.key.wallid);

					//  선택된 층의 Key를 찾아옵니다.
					CArray<int,int> sKeyList;
					Search_SelStoryKey(m_wSstoryName,m_wEstoryName,sKeyList);

					int KeyCount = sKeyList.GetSize();
					for(int j=0; j<KeyCount; j++)
					{
						key.key.storid = sKeyList.GetAt(j);
						rKey.Add(key);
					}
				}
				if(m_pDoc->m_pDataCtrl->AddRebw(rKey,rData))
				{
					Initial_SelectionData();
					m_IdList.ClearIDList();
				}
			}
		}
	}
	else
	{
		UpdateData(TRUE);
		if(AddErrorCheck())
		{
			T_REBW_D rData;
			rData.Initialize();

			rData.dDe = m_De;
			rData.dDw = m_Dw;
			rData.bUseModelThik  = m_bUseMt;
			rData.dThickness     = m_Thickness;
			rData.EndRebarName   = m_ESize;
			rData.nNumEndRebar   = m_ENo;
			rData.dEndRebarSpace = m_EndSpace;
			rData.dVerticalRebarSpace = m_VSpace;
			rData.VerticalRebarName   = m_VSize;
			rData.dHorizonRebarSpace  = m_HSpace;
			rData.HorizonRebarName    = m_HSize;

			CArray<T_REBW_K,T_REBW_K> tKey;
			while (pos)
			{
				int nItem = m_WallCheckList.GetNextSelectedItem(pos);

				CString strID    = m_WallCheckList.GetItemText(nItem,0);
				CString strStory = m_WallCheckList.GetItemText(nItem,1);

				T_REBW_K rKey;
				rKey.key.wallid = _ttoi(strID);
				rKey.key.storid = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);

				tKey.Add(rKey);
			}
	    if(m_pDoc->m_pDataCtrl->AddRebw(tKey,rData))
			{
				Initial_SelectionData();
				m_IdList.ClearIDList();
			}
		}
	}
	///////////////////////////////////////////////////////////////////////
}

void CDgnConWallDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

BOOL CDgnConWallDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_HRebarSize.GetCount() > 0)		m_HRebarSize.ResetContent();
	if(m_VRebarSize.GetCount() > 0)		m_VRebarSize.ResetContent();
	if(m_EndRebarSize.GetCount() > 0)	m_EndRebarSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_HRebarSize);
	CDBLib::GetRebarNameAtComboBox(&m_VRebarSize);
	CDBLib::GetRebarNameAtComboBox(&m_EndRebarSize);

	// Delete Selected Elements.
	if(m_pDoc==NULL)	return TRUE;

#if defined(_US) || defined(_RUS)
	if(CProduct::IsMalaysia())
		GetDlgItem(IDC_DGN_CON_FABRIC)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DGN_CON_FABRIC)->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_DGN_CON_FABRIC)->EnableWindow(FALSE);
#endif
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Initialize Start Story ComboBox.
	Initial_SstoryCombo();
	// Initialize End Story ComboBox.
	Initial_EstoryCombo();
	// Initialize Unit.
	Initial_Units();
	// Initialize General Data.
	Initial_Data();
	// Initialize Wall IDs' EditBox.
	Initial_MouseEdit();
	// Initialize ListCtrl Box.
	Initial_ListCtrlBox();
	if(m_pDoc->m_pAttrCtrl->GetCountRebw()!=0)	Write_AllListCtrl();	// Auto Sorting.
	
	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	Enable_EndBarEdit();

	return TRUE;
}

void CDgnConWallDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_WallCheckList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_WallCheckList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[2] = {(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_WALL_ID), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_STORY_NAME)};
	int width[2] = {70,110};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_WallCheckList.InsertColumn(i,&lvcolumn);
	}
}

void CDgnConWallDlg::Write_ListCtrlBox(int Index,CString strID,CString strStory)
{
	LV_ITEM lvitem;

	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strID;
	m_WallCheckList.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strStory;
	m_WallCheckList.SetItem(&lvitem);
}

void CDgnConWallDlg::Write_AllListCtrl()
{
	CArray<long,long> arWall;
	if(m_WallCheckList.GetItemCount()!=0)	m_WallCheckList.DeleteAllItems();

	POSITION Pos = NULL;
	for(Pos=m_pDoc->m_pAttrCtrl->GetStartRebw(); Pos!=NULL;)
	{
		T_REBW_K rKey;
		T_REBW_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetNextRebw(Pos, rKey, rData);
		long lWall = (long)rKey.key.wallid*1000 + rKey.key.storid; 
		arWall.Add(lWall);
	}
	// Sort Wall ID and Story.
	int iWallNum = arWall.GetSize();
	long* pValue = new long[iWallNum];
	for(int i=0; i<iWallNum; i++)	pValue[i] = arWall.GetAt(i);
	CQSort::QSortLong(pValue, iWallNum);

	for(int i=0; i<iWallNum; i++)
	{
		int iWallID		= pValue[i]/1000;
		int iWallStor	= pValue[i]%1000;
		CString strID = _T("");
		strID.Format(_T("%d"), iWallID);

		T_STOR_D sData;
		sData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetStor(iWallStor,sData))
		{
			CString strStory = sData.StoryName;
			int ListLineNo	 = m_WallCheckList.GetItemCount();
			Write_ListCtrlBox(ListLineNo, strID, strStory);
		}
	}
	delete []pValue;
}

void CDgnConWallDlg::Initial_MouseEdit()
{
//m_IdList.SetAttElemList();
	m_IdList.SetLButtonDownNotifyWindow(this);
	m_IdList.SetEnterNotifyWindow(this);
	m_IdList.SetEscNotifyWindow(this);
 	m_IdList.SetModeToUse(MOUSEEDIT_USE_SET_WALL_ID);
}

void CDgnConWallDlg::Initial_SelectionData()
{
	m_wSstory.SetCurSel(-1);
	m_wEstory.SetCurSel(-1);
	m_IdList.SetWindowText(_T(""));
	GetDlgItem(IDC_DGN_RC_WALLMARK2)->SetWindowText(_T(""));
}

void CDgnConWallDlg::Initial_SstoryCombo()
{
	if(m_wSstory.GetCount() != 0) m_wSstory.ResetContent();

	//  Wall Start Story Name을 초기화합니다.
	CArray<CString, CString&> strStoryList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int iCount = strStoryList.GetSize();
	for(int i=iCount-1; i>=0; i--)
	{
		CString tempStoryList=_T("");
		tempStoryList = strStoryList.GetAt(i);
		m_wSstory.AddString(tempStoryList);
	}
}

void CDgnConWallDlg::Initial_EstoryCombo()
{
	if(m_wEstory.GetCount() != 0) m_wEstory.ResetContent();

	int Index = m_wSstory.GetCurSel();
	if(Index != -1)
	{
		CString wSstoryName=_T("");
		m_wSstory.GetLBText(Index,wSstoryName);

		//  Wall End Story Name을 초기화합니다.
		//  (Wall Start Story Name은 제외합니다.)
		CArray<CString, CString&> strStoryList;
		m_pDoc->m_pAttrCtrl->GetStorList(strStoryList);
		int iCount = strStoryList.GetSize();
		for(int i=iCount-1; i>=0; i--)
		{
			CString tempStoryList=_T("");
			tempStoryList = strStoryList.GetAt(i);
			if(wSstoryName != tempStoryList)  m_wEstory.AddString(tempStoryList);
			else
			{
				m_wEstory.AddString(tempStoryList);
				break;
			}
		}
	}
}

void CDgnConWallDlg::Initial_Units()
{
	m_wT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wDw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wDe.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ESpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_VRebarSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HRebarSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_ESpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wTUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wDeUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wDwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_VRebarUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HRebarUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConWallDlg::Initial_Data()
{
	// Change by ZINU.('02.10.24). Default (FALSE -> TRUE).
	m_bUseMt  = TRUE;
	m_bOption = FALSE;
	// End Rebar Data를 초기화합니다.
	Check_EndRebarData();

	// Vertical Rebar.
	CString strVRebar=_T(""), strHERebar=_T("");
	CDBLib::GetDefaultMainRebarSize(4, strVRebar);
	int Index = m_VRebarSize.FindStringExact(-1, strVRebar);
	m_VRebarSize.SetCurSel(Index);		// D13.
	// Horizontal Rebar.
	CDBLib::GetDefaultSubRebarSize(4, strHERebar);
	Index = m_HRebarSize.FindStringExact(-1, strHERebar);
	m_HRebarSize.SetCurSel(Index);		// D10.
	// End Rebar.
	m_EndRebarSize.SetCurSel(Index);	// D10.

	m_EndRebarNo = 4;
	//m_EndRebarNo = 2;
	m_EndSpin.SetRange(1,50);

	m_wT.SetEditUnit(0.);
	m_ESpace.SetEditUnit(0.);
	m_VRebarSpace.SetEditUnit(0.);
	m_HRebarSpace.SetEditUnit(0.);
	//  이곳에서 사용하는 변수들을 초기화합니다.
	m_SpinNo = 0;
	m_ENo    = 0;
	m_WallIDs     = _T("");
	m_wSstoryName = _T("");
	m_wEstoryName = _T("");
	m_VSize       = _T("");
	m_HSize       = _T("");
	m_ESize       = _T("");
	m_Thickness = 0.;
	m_VSpace    = 0.;
	m_HSpace    = 0.;
	m_De        = 0.;
	m_Dw        = 0.;
	m_wDw.SetEditUnit(m_Dw);
	m_wDe.SetEditUnit(m_De);

	UpdateData(FALSE);

	Show_UseModelThickness();
	Show_WallTypeBitmap();
	//GetDlgItem(IDC_DGN_RC_WALLMARK2)->EnableWindow(FALSE);
}

void CDgnConWallDlg::OnSelchangeDgnRcWsstory() 
{
	Initial_EstoryCombo();
}

void CDgnConWallDlg::OnDgnEndCheck() 
{
	Show_EndRebarData();
	Show_WallTypeBitmap();
}

void CDgnConWallDlg::Show_EndRebarData()
{
	UpdateData(TRUE);
	if(m_bEndCheck)	// Use End Rebar.
	{
		GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALL_ENDSPIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_ENDREBAR_HIPON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALL_ENDREBARSIZE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALL_ESPACE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALL_ESPACEUNIT)->ShowWindow(SW_SHOW);
	}
	else	// Not Use End Rebar.
	{
		GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALL_ENDSPIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_ENDREBAR_HIPON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALL_ENDREBARSIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALL_ESPACE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALL_ESPACEUNIT)->ShowWindow(SW_HIDE);
	}
}

void CDgnConWallDlg::OnDgnRcUsemt() 
{
	Show_UseModelThickness();
}

void CDgnConWallDlg::Show_UseModelThickness()
{
	UpdateData(TRUE);
	if(m_bUseMt)   //  Model Thickness를 사용하겠다는 의미
	{
		GetDlgItem(IDC_DGN_WALLTHICK_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALLTHICK_COLON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_WTHICK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_WTUNIT)->ShowWindow(SW_HIDE);
	}
	else           //  Model Thickness를 사용안하겠다는 의미
	{
		GetDlgItem(IDC_DGN_WALLTHICK_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALLTHICK_COLON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_WTHICK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_WTUNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnConWallDlg::OnDeltaposDgnWallEndspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_SpinNo = _ttoi(str)+2*Factor;

	if(m_SpinNo < 2)
	{
		if(Factor == 1) m_SpinNo = 2;
		else            m_SpinNo = 50;
	}
	else if(m_SpinNo > 50)  m_SpinNo = 2;

	str.Format(_T("%d"),m_SpinNo);
	GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->SetWindowText(str);

	*pResult = 0;
}

void CDgnConWallDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_UNIT:
			// do something...
			break;
		case D_UPDATE_SEL_ADD:
			// do something...
			break;
		case D_UPDATE_SEL_DEL:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			if(m_HRebarSize.GetCount() > 0)		m_HRebarSize.ResetContent();
			if(m_VRebarSize.GetCount() > 0)		m_VRebarSize.ResetContent();
			if(m_EndRebarSize.GetCount() > 0)	m_EndRebarSize.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_HRebarSize);
			CDBLib::GetRebarNameAtComboBox(&m_VRebarSize);
			CDBLib::GetRebarNameAtComboBox(&m_EndRebarSize);
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CDgnConWallDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0)	return; 
	ASSERT(nCount==1);

	BOOL bUR_DCRB = FALSE;
	BOOL bUR_STOR = FALSE;
	BOOL bUR_REBW = FALSE;
	BOOL bUR_DCON = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
			case(UR_DCRB_ADD):
			case(UR_DCRB_DEL):	{bUR_DCRB = TRUE;	break;}
			case(UR_STOR_ADD):
			case(UR_STOR_DEL):
			case(UR_STOR_MFD):
			case(UR_STOR_MFS):	{bUR_STOR = TRUE;	break;}
			case(UR_REBW_ADD):
			case(UR_REBW_DEL):	{bUR_REBW = TRUE;	break;}
			case(UR_DCON_ADD):
			case(UR_DCON_DEL):	{bUR_DCON = TRUE;	break;}
			default:	break;
		}
	}
	// Change by ZINU.('00.3.30).
	if(bUR_DCRB)	Check_EndRebarData();	// Modify End Rebar Data by Method.
	if(bUR_STOR)
	{
		// Initialize Start and End Story ComboBox.
		Initial_SstoryCombo();
		Initial_EstoryCombo();
	}
	if(bUR_REBW)	Write_AllListCtrl();
	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	if(bUR_DCON)	Enable_EndBarEdit();
}

void CDgnConWallDlg::Check_EndRebarData()
{
	// End Rebar Data are used at Method2, Method3, Method4. Not used at Method1.
	CDgnDataCtrl DataCtrl;
	T_DCRB_D rData;
	rData.Initialize();
	// Change by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Global -> Elem(0).
	DataCtrl.Get_DgnWalDcrb(0, rData);
	if(rData.nEndRebarDesignMethod==1)	m_bEndCheck=FALSE;
	else																m_bEndCheck=TRUE;

	UpdateData(FALSE);
	Show_EndRebarData();
	Show_WallTypeBitmap();
}

BOOL CDgnConWallDlg::AddErrorCheck()
{
	BOOL bCheck=TRUE;

	GetDlgItem(IDC_DGN_RC_WALLID)->GetWindowText(m_WallIDs);
	if(m_WallIDs == _T("")) 
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_WALL_ID),MB_OK);
		return bCheck;
	}

	int sIndex = m_wSstory.GetCurSel();
	if(sIndex != -1)  m_wSstory.GetLBText(sIndex,m_wSstoryName);
	else              m_wSstoryName = _T("");
	if(m_wSstoryName == _T("")) 
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_START_STORY),MB_OK);
		return bCheck;
	}

	int eIndex = m_wEstory.GetCurSel();
	if(eIndex != -1)  m_wEstory.GetLBText(eIndex,m_wEstoryName);
	else              m_wEstoryName = m_wSstoryName;

	m_De = m_wDe.GetEditValue();
	m_Dw = m_wDw.GetEditValue();

	if(!m_bUseMt)	m_Thickness = m_wT.GetEditValue();
	else					m_Thickness = 0.;
	//////////////////////////////////////////////////////
	// 나머지 Data를 얻어가자
	int Index;
	if(m_bEndCheck)
	{
		m_ENo = m_EndRebarNo;

		Index = m_EndRebarSize.GetCurSel();
		if(Index != -1) m_EndRebarSize.GetLBText(Index,m_ESize);

		m_EndSpace = m_ESpace.GetEditValue();
	}
	else
	{
		m_ENo      = 0;
		m_EndSpace = 0.;
		m_ESize    = _T("");
	}

	Index = m_VRebarSize.GetCurSel();
	if(Index != -1) m_VRebarSize.GetLBText(Index,m_VSize);
	m_VSpace = m_VRebarSpace.GetEditValue();

	Index = m_HRebarSize.GetCurSel();
	if(Index != -1) m_HRebarSize.GetLBText(Index,m_HSize);

	double Space = m_HRebarSpace.GetEditValue();
	if(Space > 0.) m_HSpace = Space;

	return bCheck;
}

BOOL CDgnConWallDlg::DelErrorCheck()
{
	BOOL bCheck=TRUE;

	GetDlgItem(IDC_DGN_RC_WALLID)->GetWindowText(m_WallIDs);
	if(m_WallIDs == _T("")) 
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_WALL_ID),MB_OK);
		return bCheck;
	}

	int sIndex = m_wSstory.GetCurSel();
	if(sIndex != -1)  m_wSstory.GetLBText(sIndex,m_wSstoryName);
	else              m_wSstoryName = _T("");
	if(m_wSstoryName == _T("")) 
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_START_STORY),MB_OK);
		return bCheck;
	}

	int eIndex = m_wEstory.GetCurSel();
	if(eIndex != -1)  m_wEstory.GetLBText(eIndex,m_wEstoryName);
	else              m_wEstoryName = m_wSstoryName;

	return bCheck;
}

LRESULT CDgnConWallDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd() == (HWND)lParam)
	{
		CString str;
		//  Wall ID를 받아와서 개개로 쪼갭니다.
		m_IdList.GetWindowText(str);
		CArray<unsigned int,unsigned int> List;
		CStrParser::GetNodeList(str,List);
		int IdCount = List.GetSize();
		m_strWallMark = _T("");
		CString strIDs=_T("");
		for(int i=0; i<IdCount; i++)
		{
			unsigned int WallId = (unsigned int)List.GetAt(i);
			//  Wall Mark의 존재유무를 Check합니다.
			if(m_pDoc->m_pAttrCtrl->ExistUsrw(WallId))
			{
				//  Wall Mark Key를 받아옵니다.
				T_WMAK_K nKey = m_pDoc->m_pAttrCtrl->GetWmakKey(WallId);
				T_WMAK_D rData;
				rData.Initialize();
				//  Wall Mark Data를 받아옵니다.
				if(!m_pDoc->m_pAttrCtrl->GetWmak(nKey,rData))	ASSERT(0);
				int iCount = rData.arUserWallId.GetSize();
				//  해당 Wall Mark의 Wall ID들을 나열합니다.
				//  같은 Wall Mark의 Wall ID들은 선택하지 않습니다.
				for(int i=0; i<iCount; i++)
				{
					int nID = rData.arUserWallId.GetAt(i);
					CString str1=_T("");
					str1.Format(_T("%d"),nID);
					if(CheckEQWallID(str1,strIDs))
					{
						if(strIDs != _T(""))  strIDs = strIDs + _T(",") + str1;
						else              strIDs = strIDs + str1;
					}
				}
				m_IdList.SetWindowText(strIDs);
				//  같은 Wall Mark는 선택하지 않습니다.
				if(CheckEQWallMark(rData.MarkName))
				{
					if(m_strWallMark != _T(""))	m_strWallMark = m_strWallMark + _T(",") + rData.MarkName;
					else										m_strWallMark = m_strWallMark + rData.MarkName;
					GetDlgItem(IDC_DGN_RC_WALLMARK2)->SetWindowText(m_strWallMark);
				}
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_WALL_MARK),MB_OK);
		}
	}
	return 0L;
}

BOOL CDgnConWallDlg::CheckEQWallID(CString strID,CString strIDs)
{
	BOOL bCheck=TRUE;
	CArray<CString,CString> IDs;
	TCHAR ch;
	CString temp=_T("");
	int iLength = strIDs.GetLength();
	for(int i=0; i<iLength; i++)
	{
		ch = strIDs.GetAt(i);
		if(ch==',' || i+1==iLength)
		{
			if(ch==',')	IDs.Add(temp);
			else
			{
				CString str = temp+ch;
				IDs.Add(str);
			}
			temp=_T("");
		}
		else	temp = temp+ch;
	}
	int iCount=IDs.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString strCheck = IDs.GetAt(i);
		if(strCheck==strID)	bCheck = FALSE;
	}
	return bCheck;
}

BOOL CDgnConWallDlg::CheckEQWallMark(CString MarkName)
{
	BOOL bCheck=TRUE;
	CArray<CString,CString> WallMark;
	TCHAR ch;
	CString temp=_T("");
	int iLength = m_strWallMark.GetLength();
	for(int i=0; i<iLength; i++)
	{
		ch = m_strWallMark.GetAt(i);
		if(ch==',' || i+1==iLength)
		{
			if(ch==',')	WallMark.Add(temp);
			else
			{
				CString str = temp+ch;
				WallMark.Add(str);
			}
			temp=_T("");
		}
		else	temp = temp+ch;
	}
	int iCount=WallMark.GetSize();
	for(int i=0; i<iCount; i++)
	{
		CString strCheck = WallMark.GetAt(i);
		if(strCheck==MarkName)	bCheck = FALSE;
	}
	return bCheck;
}

LRESULT CDgnConWallDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd() == (HWND)lParam)
	{
		//CString str;
		//m_MousePos.GetWindowText(str);     
		//CArray<unsigned int, unsigned int> m_iaJointList;
		//if(CStrParser::GetNodeList(str,m_iaJointList))
		//{
		//  GSaveHistoryFormatNF(_T("Node Key iCount: %d"),m_iaJointList.GetSize());
		//}
		//else
		//{
		//  GSaveHistoryNF(_T("Node List Error !!!"));
		//}
	}
	return 0L;
}

LRESULT CDgnConWallDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if(m_IdList.GetSafeHwnd() == (HWND)lParam)
	{
		//GSaveHistoryNF(_T("Escape !!!"));          
		//m_MousePos.SetWindowText(_T(""));
	}
	return 0L;
}

void CDgnConWallDlg::Search_SelStoryKey(CString strSName,CString strEName,CArray<int,int>& KeyList)
{
	if(KeyList.GetSize() != 0)  KeyList.RemoveAll();

	T_STOR_K SKeyNo = m_pDoc->m_pAttrCtrl->GetStorKey(strSName);
	T_STOR_K EKeyNo = m_pDoc->m_pAttrCtrl->GetStorKey(strEName);

	CArray<T_STOR_K,T_STOR_K> List;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(List);
	int StartNo,EndNo;
	int iCount = List.GetSize();
	for(int i=0; i<iCount; i++)
	{
		int KeyNo = List.GetAt(i);
		if(SKeyNo == KeyNo) StartNo=i;
		if(EKeyNo == KeyNo) EndNo=i;
	}

	for(int i=StartNo; i<=EndNo; i++)
	{
		int KeyNo = List.GetAt(i);
		KeyList.Add(KeyNo);
	}
}

void CDgnConWallDlg::OnItemchangedDgnWallcheckList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(!m_bOption)
	{
	  int ItemNo = pNMListView->iItem;
		Select_ListCtrlItem(ItemNo);
	}
	*pResult = 0;
}

void CDgnConWallDlg::Select_ListCtrlItem(int nItem)
{
	if(nItem != -1)
	{
		CString strID    = m_WallCheckList.GetItemText(nItem,0);
		CString strStory = m_WallCheckList.GetItemText(nItem,1);
		SetDlgItemText(IDC_DGN_RC_WALLID, strID);

		// Change by ZINU.('03.02.14). To Show Wall Mark.
		int iUserWallId = _ttoi(strID);
		m_strWallMark = _T("");
		if(iUserWallId > 0)
		{
			CDgnDataCtrl DataCtrl;
			CArray<unsigned int, unsigned int> arInternalWallIdList;
			if(m_pDoc->m_pPostCtrl->GetInternalWallIdList(iUserWallId, arInternalWallIdList))
			{
				unsigned int iInternalWallId = arInternalWallIdList.GetAt(0);
				T_WALL_KEY KeyWall=0;
				m_pDoc->m_pPostCtrl->GetWallKey(iInternalWallId, KeyWall);
				T_WMAK_D WmakData;
				DataCtrl.Get_DgnWalWmak(KeyWall, WmakData);
				m_strWallMark = WmakData.MarkName;
			}
		}
		GetDlgItem(IDC_DGN_RC_WALLMARK2)->SetWindowText(m_strWallMark);

		int Index = m_wSstory.FindStringExact(-1,strStory);
		if(Index != -1) 
		{
			m_wSstory.SetCurSel(Index);
			// Initialize End Story ComboBox.
			Initial_EstoryCombo();
		}

		Index = m_wEstory.FindStringExact(-1,strStory);
		if(Index != -1) m_wEstory.SetCurSel(Index);

		T_REBW_K Key;
		Key.key.wallid = _ttoi(strID);
		Key.key.storid = m_pDoc->m_pAttrCtrl->GetStorKey(strStory);

		T_REBW_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetRebw(Key,rData);

		m_bUseMt = rData.bUseModelThik;
	  m_wT.SetEditUnit(rData.dThickness);
	  m_wDw.SetEditUnit(rData.dDw);
	  m_wDe.SetEditUnit(rData.dDe);
		m_ESpace.SetEditUnit(rData.dEndRebarSpace);
		m_EndRebarNo = rData.nNumEndRebar;
		if(m_EndRebarNo == 0) 
		{
			m_EndRebarNo = 2;
			m_bEndCheck  = FALSE;
		}
		else	m_bEndCheck = TRUE;

		m_VRebarSpace.SetEditUnit(rData.dVerticalRebarSpace);
		m_HRebarSpace.SetEditUnit(rData.dHorizonRebarSpace);

		Index = m_VRebarSize.FindStringExact(-1,rData.VerticalRebarName);
		if(Index != -1) m_VRebarSize.SetCurSel(Index);

		Index = m_HRebarSize.FindStringExact(-1,rData.HorizonRebarName);
		if(Index != -1) m_HRebarSize.SetCurSel(Index);

		Index = m_EndRebarSize.FindStringExact(-1,rData.EndRebarName);
		if(Index != -1) m_EndRebarSize.SetCurSel(Index);

		UpdateData(FALSE);
		Show_UseModelThickness();
		Show_EndRebarData();
		Show_WallTypeBitmap();
	}
}

void CDgnConWallDlg::Show_WallTypeBitmap()
{
	UpdateData(TRUE);
	if(m_bEndCheck)
	{
		GetDlgItem(IDC_DGN_WALL_TYPE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_WALL_TYPE2)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALL_TYPE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_WALL_TYPE2)->ShowWindow(SW_SHOW);
	}
}

void CDgnConWallDlg::OnSetfocusDgnRcWsstory() 
{
	//  Selected ListBox Item을 UnSeclected 시킵니다.
	UnSelected_ListBoxItem();
}

void CDgnConWallDlg::OnSetfocusDgnRcWestory() 
{
	//  Selected ListBox Item을 UnSeclected 시킵니다.
	UnSelected_ListBoxItem();
}

void CDgnConWallDlg::OnSetfocusDgnRcWallid() 
{
	//  Selected ListBox Item을 UnSeclected 시킵니다.
	UnSelected_ListBoxItem();
	GetDlgItem(IDC_DGN_RC_WALLMARK2)->SetWindowText(_T(""));
}

void CDgnConWallDlg::UnSelected_ListBoxItem() 
{
	POSITION pos = m_WallCheckList.GetFirstSelectedItemPosition();
	if(pos != NULL)  
	{
		m_bOption = TRUE;
		while (pos)
		{
			int nItem = m_WallCheckList.GetNextSelectedItem(pos);
			CString strID    = m_WallCheckList.GetItemText(nItem,0);
			CString strStory = m_WallCheckList.GetItemText(nItem,1);

			m_WallCheckList.DeleteItem(nItem);
			Write_ListCtrlBox(nItem,strID,strStory);
		}
		m_bOption = FALSE;
	}
}

// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
void CDgnConWallDlg::Enable_EndBarEdit()
{
	// Change by ZINU.('01.9.14). Although WSD, Let's usd All Methods.
/*/
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode==_T("AIK-WSD2K") || strCode==_T("AIJ-WSD99"))	// WSD.
	{
		GetDlgItem(IDC_DGN_END_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_ENDREBARSIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_ESPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_ENDSPIN)->EnableWindow(FALSE);
	}
	else	// USD, LSD.
	{
		GetDlgItem(IDC_DGN_END_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_EBDREBARNO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_ENDREBARSIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_ESPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_ENDSPIN)->EnableWindow(TRUE);
	}
/*/
}

void CDgnConWallDlg::OnDgnFabric()
{
	CDgnConWeldMeshDlg dlg;

	int Index = m_VRebarSize.GetCurSel();
	if(Index != -1) m_VRebarSize.GetLBText(Index,dlg.m_strVBarName);

	Index = m_HRebarSize.GetCurSel();
	if(Index != -1) m_HRebarSize.GetLBText(Index,dlg.m_strHBarName);

	dlg.m_dVBarSpace  = m_VRebarSpace.GetEditValue();
	dlg.m_dHBarSpace  = m_HRebarSpace.GetEditValue();


	if(dlg.DoModal()==IDOK)
	{
		Index = m_VRebarSize.FindStringExact(-1,dlg.m_strVBarName);
		if(Index < 0) m_VRebarSize.SetCurSel(0);
		else          m_VRebarSize.SetCurSel(Index);

		m_VRebarSpace.SetEditUnit(dlg.m_dVBarSpace);

		Index = m_HRebarSize.FindStringExact(-1,dlg.m_strHBarName);
		if(Index < 0) m_HRebarSize.SetCurSel(0);
		else          m_HRebarSize.SetCurSel(Index);

		m_HRebarSpace.SetEditUnit(dlg.m_dHBarSpace);

		UpdateData(FALSE);
	}
}