// TreeReportCtrl.cpp : implementation file
// 
// 추가하려면...
// 0. TreeWorkDefine.h 파일에서 순서에 맞게 ItemID를 Define 해준다. (순서중요)
// 1. SetXX XXX 함수를 추가하여 Item을 추가해준다. (Bitmap 추가)
// 2. UpdateBuffer() 함수에서 데이터 Update에 따른 Item 갱신이 이루어질 수 있도록 코딩.
// 3. OnContextMenu()에서 Context Menu목록을 내용에 맞게 추가해준다.
// 4. Context Menu 목록별로 하나씩 각 Message처리를 해준다.
// 
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TreeReportCtrl.h"
#include "TreeMenuCtrl.h"
#include "BarTreeMenu.h"
#include "ReportSetupDlg.h"
#include "ReportUnitCustomizeDlg.h"
#include "ReportRefDBDlg.h"
#include "ReportTBTemplateDlg.h"
#include "ReportTableDlg.h"
#include "LoadWordLib.h"
#include "ReportImageFileDlg.h"
#include "ReportHeaderFooterDlg.h"
#include "ReportCommonFunc.h"

#include "..\wg_base\wg_base_ViewBase.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\TypeEx.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\XMultiLang.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DispOptMgr.h"
#include "..\wg_db\DispOptColorObj.h"
#include "..\wg_db\DBLib.h"

#include "..\wg_main\wg_mainres2.h"


#include "..\wg_gps\GPSAMRUtil.h"  // for Dynamic Report Figure
#include "..\wg_gps\GPSAMRUDFDlg.h"
#include "..\wg_gps\GPSAMRUDCData.h"  // for Dynamic Report Chart(Graph)

#include "..\wg_tb\TBAMRUDTData.h"
#include "..\wg_tb\TBResultView.h"

#include "..\wg_dgn\DgnAmrSectionTb.h"
#include "..\wg_dgn\DgnDrawSection.h"

using namespace mit::frx;

IMPLEMENT_DYNAMIC(CTreeReportCtrl, MBaseTreeCtrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//========================================================================
// CTreeWorksCtrl Start!
//========================================================================

/////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
CTreeReportCtrl::CTreeReportCtrl() : m_AutoRegen(this)
{
	m_bDrag = FALSE;
	m_bDragPoss = FALSE;

	m_pMemDC = NULL;
	m_rcMemDC.SetRect(-1,-1,-1,-1);
	m_rectEraseOld.SetRect(-1,-1,-1,-1);

	m_BkColor = RGB( 192, 220, 192);
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();
	
	m_ahDragItems.RemoveAll();
	m_nDataType = 0;

	m_aReportTextCaption.RemoveAll();
	m_aReportTextStrName.RemoveAll();

	m_mapCHReportTextCaptions.clear();
	m_mapCHReportTextStrNames.clear();
	
	m_aUchtRes.RemoveAll();
	
	m_iChapterID = 0;
	m_iSegmentID = 0;
	m_bInsert = TRUE;

}

CTreeReportCtrl::~CTreeReportCtrl()
{
	m_strBkColorName.Empty();
	m_strBkTitleColorName.Empty();

	FreeBoundaryInfo();

	if (m_pMemDC)  { delete m_pMemDC;  m_pMemDC = NULL; }
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTreeReportCtrl message handlers
//
//BEGIN_MESSAGE_MAP(CTreeReportCtrl, CTreeCtrl)
#define CTreeCtrl MBaseTreeCtrl
BEGIN_MESSAGE_MAP(CTreeReportCtrl, CTreeCtrl)
#undef CTreeCtrl
	//{{AFX_MSG_MAP(CTreeReportCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_WORKTREE_ACTIVE, OnWorktreeActive)
	ON_COMMAND(ID_WORKTREE_DELETE, OnWorktreeDelete)
	ON_COMMAND(ID_WORKTREE_INACTIVE, OnWorktreeInactive)
	ON_COMMAND(ID_WORKTREE_SELECT, OnWorktreeSelect)
	ON_COMMAND(ID_WORKTREE_UNSELECT, OnWorktreeUnselect)
	ON_COMMAND(ID_WORKTREE_UNSELECTALL, OnWorktreeUnselectall)
	ON_COMMAND(ID_WORKTREE_ACTIVEALL, OnWorktreeActiveall)
	ON_COMMAND(ID_WORKTREE_PROPERTIES, OnWorktreeProperties)
	ON_COMMAND(ID_WORKTREE_REPORT_UNIT, OnWorktreeReportUnit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_WORKTREE_SELECTPLUS, OnWorktreeSelectplus)
	ON_COMMAND(ID_WORKTREE_DISPLAY, OnWorktreeDisplay)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_WORKTREE_DISAPPEAR, OnWorktreeDisappear)
	ON_COMMAND(ID_WORKTREE_ACTIVE_PLUS, OnWorktreeActivePlus)
	ON_COMMAND(ID_WORKTREE_EXPANDALL, OnWorktreeExpandAll)
	ON_COMMAND(ID_WORKTREE_EXPAND_1LEVEL, OnWorktreeExpand1LEVEL)
	ON_COMMAND(ID_WORKTREE_EXPAND_2LEVEL, OnWorktreeExpand2LEVEL)
	ON_COMMAND(ID_WORKTREE_INSERT_REPORT, OnWorktreeInsert)
	ON_COMMAND(ID_WORKTREE_ADD_IMAGEFILE, OnWorktreeAddImageFile)
	ON_COMMAND(ID_WORKTREE_SAVE_IMAGE, OnWorktreeSaveImageFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// DB Update
void CTreeReportCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	CBCGPGridCtrl::m_pHighlightedItem = nullptr;
	UnselectAllItems();

	switch(lHint)
	{
		case D_UPDATE_DBALL:
		case D_UPDATE_DEFAULT:
			SetItemTree(TRUE);
			//UpdateBuffer();
		  break;
		case D_UPDATE_BUFFER_BEFORE:
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		case D_UPDATE_SEL_PRE:
		case D_UPDATE_SEL_POST:
		case D_UPDATE_EXE_END:
			SetUchtRes();
			UpdateBuffer();
			Invalidate();
			break;
		case D_UPDATE_DISPLAYOPTION:
			{
				COLORREF color = GetBkColor();
				SetBkColor( color );
				Invalidate();
			}
		  break;

		case D_UPDATE_UNIT:
			SetDynReportImageItemTree(pDoc,FALSE);
			break;

		case D_UPDATE_POST_STAGE_CHANGED:
		case D_UPDATE_STAGE_CHANGED:
		case D_UPDATE_STAGE_DBALL:
			// Stage가 변할 때 Work Tree를 다시 그려 줌 
			SetDynReportTableItemTree(pDoc,FALSE);
			//SetItemTree();
			break;
		case D_UPDATE_POST_STEP_CHANGED:
			break;
		case D_UPDATE_PREF_CHANGED:
			/*-----------------------------------
				Preference가 변경될 때 Property가 변경되는 경우
				현재 Reinforcement for PSC Section경우만 업데이트 시킴
				2005.11.08 jkpark
			----------------------------------*/
			pDoc = CDBDoc::GetDocPoint();
			if (!pDoc) { TRACE(_LS(IDS_TMWK__MSG1));	return; }
			if(pDoc->m_pAttrCtrl->GetCountRpsc()!=m_nRPSCNum)
				SetDynReportTableItemTree(pDoc, FALSE);
			break;
		case D_UPDATE_NOTIFY:
		  break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
			break;
	}
}

void CTreeReportCtrl::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	LockWindowUpdate();
	SetInUpdateBuffer(TRUE);

	BOOL bDynReportFigureItem = FALSE;
	BOOL bDynReportTableItem = FALSE;
	BOOL bDynReportChartItem = FALSE;
	BOOL bDynReportTextItem = FALSE;

	int nModified = 0;

	// 바뀐 Directory 찾기 
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
//   if (nCount == 0) { SetBoundaryItemTree(pDoc,FALSE); return; } // Panel Zone 때문에... DBALL로 넘어오는 데... 그때는 nCount==0... 으~~
	//ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_UFIG_ADD): case(UR_UFIG_DEL): case(UR_UFIG_MFS): case(UR_UFIG_MFD):
		case(UR_UIMG_ADD): case(UR_UIMG_DEL): case(UR_UIMG_MFS): case(UR_UIMG_MFD):  
			bDynReportFigureItem = TRUE;
			break;
		case(UR_UTBL_ADD): case(UR_UTBL_DEL): case(UR_UTBL_MFS): case(UR_UTBL_MFD):
		case(UR_UTPL_ADD): case(UR_UTPL_DEL): case(UR_UTPL_MFS): case(UR_UTPL_MFD):
		case(UR_SECT_ADD): case(UR_SECT_DEL): case(UR_SECT_MFS): case(UR_SECT_MFD):  
		case(UR_ELEM_ADD): case(UR_ELEM_DEL): case(UR_ELEM_MFS): case(UR_ELEM_MFD):  
		case(UR_CSCS_ADD): case(UR_CSCS_DEL): case(UR_CSCS_MFS): case(UR_CSCS_MFD):
		case(UR_RPSC_ADD): case(UR_RPSC_DEL): 
		case(UR_RPSC_SSM_ADD): case(UR_RPSC_SSM_DEL): 
			bDynReportTableItem = TRUE;
			break;
		case(UR_UCHT_ADD): case(UR_UCHT_DEL): case(UR_UCHT_MFS): case(UR_UCHT_MFD):
		case(UR_SPFC_ADD): case(UR_SPFC_DEL): case(UR_SPFC_MFS): case(UR_SPFC_MFD):
		case(UR_THFC_ADD): case(UR_THFC_DEL): case(UR_THFC_MFS): case(UR_THFC_MFD):
		case(UR_POFC_ADD): case(UR_POFC_DEL): case(UR_POFC_MFS): case(UR_POFC_MFD):
		case(UR_TDMF_ADD): case(UR_TDMF_DEL): case(UR_TDMF_MFS): case(UR_TDMF_MFD):
		case(UR_SBCT_ADD): case(UR_SBCT_DEL): case(UR_SBCT_MFS): case(UR_SBCT_MFD):      
			bDynReportChartItem = TRUE;
		case(UR_USUM_ADD): case(UR_USUM_DEL): case(UR_USUM_MFS): case(UR_USUM_MFD):
			bDynReportTextItem = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	// Tree의 현재 위치 기억 !!
	HTREEITEM   hTIKeep = GetFirstVisibleItem();
	int indx = ItemIndex(hTIKeep);

	// 바뀐 Directory만 다시그려주기 - 속도개선 
	if (bDynReportFigureItem) SetDynReportImageItemTree(pDoc,FALSE);
	if (bDynReportTableItem ) SetDynReportTableItemTree(pDoc,FALSE);
	if (bDynReportChartItem ) SetDynReportChartItemTree(pDoc,FALSE);
	if (bDynReportTextItem  ) SetDynReportTextItemTree(pDoc,FALSE);
 
	UnlockWindowUpdate();
	SetInUpdateBuffer(FALSE);
	// Tree의 이전 위치로 복귀 
	hTIKeep = GetItemAt(indx);
	if(hTIKeep!=NULL) SelectSetFirstVisible(hTIKeep);
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Create / Destroy
void CTreeReportCtrl::ToggleStyle(DWORD lStyleMask )
{
	long		lStyleOld;

	lStyleOld = GetStyle();
	BOOL bSetBits  = !(lStyleMask&lStyleOld);
	if (bSetBits)
		lStyleOld  |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	
	DWORD dwStyle, dwExStyle;
	GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		ModifyTreeCtrlStyles( lStyleMask, 0, 0, 0);
	else
		ModifyTreeCtrlStyles( 0, lStyleMask, 0, 0);
}

void CTreeReportCtrl::CreateTree()
{
	CDC* pDC = GetDC();
	int numColors = pDC->GetDeviceCaps( NUMCOLORS );
	ReleaseDC(pDC);
	COLORREF clrBack =  (numColors == -1 ) ? m_BkColor : RGB( 255, 255, 255);
	//COLORREF clrBack =  (numColors == -1 ) ? RGB( 192, 220, 192) : RGB( 255, 255, 255);
	// change the background color
	SetBkColor( clrBack );

	// change the normal text color
	SetTextColor(RGB(0,0,0));

	// change the selected text color
	SetSelTextColor(RGB(255,255,255));

	return;
}

int CTreeReportCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateTree();
	SetItemTree(TRUE);
	EnableMultiSelect();
	EndInsertTreeItem();
	
	return 0;
}

void CTreeReportCtrl::DestroyTree()
{
// 	for (int i=0; i<m_ImageList.GetImageCount(); i++)
// 		m_ImageList.Remove(0);
// 	m_ImageList.DeleteImageList();
}

void CTreeReportCtrl::OnDestroy() 
{
	MBaseTreeCtrl::OnDestroy();
	DestroyTree();
}

void CTreeReportCtrl::FreeBoundaryInfo()
{
}

void CTreeReportCtrl::FreeWorkTreeBuffer()
{
	FreeBoundaryInfo();
}

COLORREF CTreeReportCtrl::GetBkColor()
{
// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 	COLORREF Color;
// 	if (pDoc != 0 && pDoc->GetDispOpt() != 0)
// 	{
// 		if (m_strBkColorName.IsEmpty())
// 		{
// 			COptColorObjNames CC;  
// 			m_strBkColorName = CC.m_TreeMenu_Background;
// 		}
// 		CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(m_strBkColorName);
// 		Color = pColor->GetCurVal(0);
// 	}
// 	else
// 	{
// 		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
// 		CString csSection, csEntry, csValue;
// 		csSection = _T("DispOpt-Color");
// 		csEntry = _T("Treemenu	Background:Value");
// 		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
// 		Color = RGB(128,128,128);
// 		if (!csValue.IsEmpty()) Color = (COLORREF)atoi(csValue);
// 	}
// 	return Color;
	return RGB(233, 235, 239);
}

COLORREF CTreeReportCtrl::GetBkColorTitle()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	COLORREF clrColor2;
	if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	{
		if (m_strBkTitleColorName.IsEmpty())
		{
			COptColorObjNames CC;  
			m_strBkTitleColorName = CC.m_MV_WorktreeBkTitle;
		}
		CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(m_strBkTitleColorName);
		clrColor2 = pColor->GetCurVal(0);
	}
	else
	{
		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
		CString csSection, csEntry, csValue;
		csSection = _T("DispOpt-Color");
		csEntry = _T("Tree Menu	Background Title:Value");
		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
		clrColor2 = RGB(128,128,128);
		if (!csValue.IsEmpty()) clrColor2 = (COLORREF)_ttoi(csValue);
	}
	return clrColor2;
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Set Tree Item
void CTreeReportCtrl::SetItemTree(BOOL bInit)
{
	HTREEITEM   hTI;
	// 기존의 Tree Item 지우는 부분 필요

	if(GetCount()) DeleteAllItems();
	// 기존 Work tree의 메모리 지움

	FreeWorkTreeBuffer();

	CString strRoot=_T("");
#if defined(_ORG)
	strRoot = CString(_T("Smart Report"));
#else
	strRoot = _LS(IDS_TM_WORK_Dynamic_Report);
#endif

	m_hRootTi = GetRootItem();
	m_hRootTi = InsertTreeItem(m_hRootTi, WORK_ROOT, strRoot, _T("ico24_mods_smartreport_generator"));
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	// Set item list
	hTI = SetDynReportSetupItemTree(pDoc,bInit);
	hTI = SetDynReportUnitItemTree(pDoc,bInit);
	hTI = SetDynReportRefDBItemTree(pDoc,bInit);
	hTI = SetDynReportImageItemTree(pDoc,bInit);
	hTI = SetDynReportTableItemTree(pDoc,bInit);
	hTI = SetDynReportChartItemTree(pDoc,bInit);
#if defined(_CH)
	hTI = SetDynReportTextItemTree_CH(pDoc,bInit);
#else
	hTI = SetDynReportTextItemTree(pDoc,bInit);
#endif
	hTI = SetDynReportFooterItemTree(pDoc,bInit);

	SelectItem(m_hRootTi);
//	if(bInit) ExpandCompletely(m_hRootTi, TRUE); // 최초에 보여줄 때는 모두 펼쳐서...
	if(bInit) Expand(m_hRootTi, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
}

// Dynamic Report Setting, 20090617 KIMJM
HTREEITEM CTreeReportCtrl::SetDynReportSetupItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	int nCount = 1;
	hTI = SetSub1TreeItem(WORK_DYST_FLD,bInit,nCount,1);
	if(hTI==NULL) return NULL;
	
	if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	return hTI;
}

// Dynamic Report Unit
HTREEITEM CTreeReportCtrl::SetDynReportUnitItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	int nCount = 1;
	hTI = SetSub1TreeItem(WORK_DYUN_FLD,bInit,nCount,1);
	if(hTI==NULL) return NULL;

	if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	return hTI;
}


HTREEITEM CTreeReportCtrl::SetDynReportFooterItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	int nCount = 1;
#if defined(_IGEN)
	hTI = SetSub1TreeItem(WORK_UFTR_FLD,bInit,nCount,14);
#else
	hTI = SetSub1TreeItem(WORK_UFTR_FLD,bInit,nCount,1);
#endif
	if(hTI==NULL) return NULL;
	
	if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	return hTI;
}

// Reference DB, 20090624 KIMJM
HTREEITEM CTreeReportCtrl::SetDynReportRefDBItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	int nCount = 1;
	hTI = SetSub1TreeItem(WORK_DYDB_FLD,bInit,nCount,2);
	if(hTI==NULL) return NULL;
	
	if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	return hTI;
}

HTREEITEM CTreeReportCtrl::SetDynReportImageItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL, hT2=NULL;
	CString   tStr=_T("");
	CArray<T_UFIG_K, T_UFIG_K> aUfigK;
	T_UFIG_D UfigD;
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	int nUfig = aUfigK.GetSize();

	CArray<T_UIMG_K, T_UIMG_K> aUimgK;
	T_UIMG_D UimgD;
	pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
	int nUimg = aUimgK.GetSize();

	int nCount=0;
	nCount = nUfig+nUimg;
	if(nCount==0)
		hTI = SetSub1TreeItem(WORK_DYIM_FLD,bInit,3,3);
	else
		hTI = SetSub1TreeItem(WORK_DYIM_FLD,bInit,nCount,3);

	if(hTI==NULL) return NULL;

	T_UNIT_INDEX tUnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent( tUnitIndex );

	T_UNIT_NAMEFACT tUnitName;
	pDoc->m_pUnitCtrl->GetUnitNameDefault( tUnitName );
	
	// User Defined Figures
	if(nUfig>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_UFIG_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Images), _T("ico24_treemenu_report_userdefinedimages"));
		for(int i=0; i<nUfig; i++)
		{
			pDoc->m_pAttrCtrl->GetUfig(aUfigK[i], UfigD);
			tStr = UfigD.strName;

			if( (UfigD.PrePrudD.nUnitLength >= 0 && UfigD.PrePrudD.nUnitLength < 10 ) && tUnitIndex.nBase_Length != UfigD.PrePrudD.nUnitLength )
			{
				CString strLength = tUnitName.strLength[UfigD.PrePrudD.nUnitLength];
				tStr.Format( _T("%s (%s)"), tStr, strLength );
			}

			InsertTreeItem(hT2,WORK_UFIG_LST,tStr, _T("ico24_treemenu_report_userdefinedimages"));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_UFIG_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Images), _T("ico24_treemenu_report_userdefinedimages"));
	}

	// Image Files
	if(nUimg>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_UIMG_FLD,_LS(IDS_TM_DYNA_REPORT_Image_Files), _T("ico24_treemenu_report_imagefiles"));
		for(int i=0; i<nUimg; i++)
		{
			pDoc->m_pAttrCtrl->GetUimg(aUimgK[i], UimgD);
			tStr = UimgD.strFilePath;
			InsertTreeItem(hT2,WORK_UIMG_LST,tStr, _T("ico24_treemenu_report_userdefinedimages"));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_UIMG_FLD,_LS(IDS_TM_DYNA_REPORT_Image_Files), _T("ico24_treemenu_report_imagefiles"));
	}

	//if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	if(hTI!=NULL)
	{
		if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
		else ExpandCompletely(hTI, TRUE);
	}
	return hTI;

//   HTREEITEM hTI=NULL;
//   CString   tStr;
//   CArray<T_UFIG_K, T_UFIG_K> aKeyList;
//   T_UFIG_D data;
//   pDoc->m_pAttrCtrl->GetUfigKeyList(aKeyList);
//   int nCount = aKeyList.GetSize();
//   if(nCount==0)
//   {
//     int nTmp=1;
//     hTI = SetSub1TreeItem(WORK_UFIG_FLD,bInit,nTmp,1);
//   }
//   else
//   {
//     hTI = SetSub1TreeItem(WORK_UFIG_FLD,bInit,nCount,1);
//   }
//   
//   if(hTI==NULL) return NULL;
//   
//   for (int i = 0; i < nCount; i++)
//   {
//     pDoc->m_pAttrCtrl->GetUfig(aKeyList[i], data);
//     tStr = data.strName;
//     InsertTreeItem(hTI,WORK_UFIG_LST,tStr,_T("ico24_temp"));
//   }
//   //if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
//   if(hTI!=NULL)
//   {
//     if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
//     else ExpandCompletely(hTI, TRUE);
//   }
//   return hTI;
}

// Dynamic Report Table, 20090604 KIMJM
HTREEITEM CTreeReportCtrl::SetDynReportTableItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL, hT2=NULL, hT3=NULL;
	CString   tStr;
	m_mapCHReportStrName_New.clear();
	CArray<T_UTBL_K, T_UTBL_K> aUtblK;
	T_UTBL_D UtblD;
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	int nUtbl = aUtblK.GetSize();

	CArray<T_SECT_K, T_SECT_K> aUsedSectKeyList;
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aUsedSectKeyList);
	int nUsedSectKeyListCnt = aUsedSectKeyList.GetSize();
	
	int nSize=0;
	if(nUsedSectKeyListCnt<=0)
		nSize = 1;
	else
		nSize = nUsedSectKeyListCnt;

	CMapEx<UINT,UINT,UINT,UINT> mUsedSectKeyList;
	mUsedSectKeyList.InitHashTable(nSize);
	UINT nKey=0;
	for(int m=0; m<nUsedSectKeyListCnt; m++)
	{
		nKey = aUsedSectKeyList[m];
		mUsedSectKeyList.SetAt(nKey, 1);
	}

	CArray<T_RPSC_K, T_RPSC_K> aRpscKeyList;
	CArray<T_RPSC_K, T_RPSC_K> aRpscKeyListAll;
	pDoc->m_pAttrCtrl->GetRpscKeyList(aRpscKeyListAll); // Reinforcement for PSC Section
	CArrayUtil::IntersectUINT(aUsedSectKeyList, aRpscKeyListAll, aRpscKeyList);

	CArray<T_CSCS_K, T_CSCS_K> aCscsKeyList;
	CArray<T_CSCS_K, T_CSCS_K> aCscsKeyListAll;
	pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsKeyListAll); // Composite Section for C.S.
	SortCscsKey(pDoc, aCscsKeyListAll);
	int nCscsCntAll = aCscsKeyListAll.GetSize();

	T_SECT_K SectK=0;
	T_CSCS_K CscsK=0;
	T_CSCS_D CscsD;
	UINT nVal=0;
	aCscsKeyList.RemoveAll();
	for(int i=0; i<nCscsCntAll; i++)
	{
		CscsK = aCscsKeyListAll[i];
		CscsD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
			continue;
		
		SectK = CscsD.SectKey;
		if(!mUsedSectKeyList.Lookup(SectK, nVal)) 
			continue;

		aCscsKeyList.Add(CscsK);
	}
	
	int nRpscCnt = aRpscKeyList.GetSize();
	int nCscsCnt = aCscsKeyList.GetSize();

	CArray<T_UTPL_K, T_UTPL_K> aUtplK;
	pDoc->m_pAttrCtrl->GetUtplKeyList(aUtplK);
	int nUtpl = aUtplK.GetSize();

	int nCount=0;
	nCount = nUtbl+nUsedSectKeyListCnt+nRpscCnt+nCscsCnt+nUtpl;
	if(nCount==0)
		hTI = SetSub1TreeItem(WORK_DYTB_FLD,bInit,6,6);
	else
		hTI = SetSub1TreeItem(WORK_DYTB_FLD,bInit,nCount,6);
	if(hTI==NULL) return NULL;

	// User Defined Tables
	if(nUtbl>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_UTBL_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Tables), _T("ico24_treemenu_report_userdefinedtables"));
	
		for(int i=0; i<nUtbl; i++)
		{
			pDoc->m_pAttrCtrl->GetUtbl(aUtblK[i], UtblD);
			tStr = UtblD.strName;
			InsertTreeItem(hT2,WORK_UTBL_LST,tStr, _T("ico24_treemenu_report_userdefinedtables"));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_UTBL_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Tables), _T("ico24_treemenu_report_userdefinedtables"));
	}

	// Special Tables
	if(nUsedSectKeyListCnt+nRpscCnt+nCscsCnt>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_USTB_FLD,_LS(IDS_TM_DYNA_REPORT_Special_Tables), _T("ico24_treemenu_report_specialtables"));

		T_SECT_K  SectK=0;
		T_SECT_D  SectD; 

		// Special Tables - Section Summary
		if(nUsedSectKeyListCnt>0)
		{
			hT3 = InsertTreeItem(hT2,WORK_USSS_FLD,_LS(IDS_TM_DYNA_REPORT_Section_Summary), _T("ico24_treemenu_report_sectionsummary"));
			
			for(int i=0; i<nUsedSectKeyListCnt; i++)
			{
				SectK = aUsedSectKeyList[i];
				SectD.Initialize();
				
				if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
					continue;
				tStr.Format(_LS(IDS_TMWK__MSG61), SectK, SectD.SName);
				InsertTreeItem(hT3,WORK_USSS_LST,tStr, _T("ico24_treemenu_report_sectionsummary"));
			}
		}
		else
		{
			hT3 = InsertTreeItem(hT2,WORK_USSS_FLD,_LS(IDS_TM_DYNA_REPORT_Section_Summary), _T("ico24_treemenu_report_sectionsummary"));
		}
		m_mapCHReportStrName_New[_LS(IDS_TM_DYNA_REPORT_Section_Summary)] = hT3;

#if defined(_CIVIL)
		// Special Tables - Reinforcement of Sections
		if(nRpscCnt>0)
		{
			hT3 = InsertTreeItem(hT2,WORK_USRS_FLD,_LS(IDS_TM_DYNA_REPORT_Reinforcement_of_Sections), _T("ico24_treemenu_report_reinforcementofsections"));
			
			for(int i=0; i<nRpscCnt; i++)
			{
				SectK = aRpscKeyList[i];
				SectD.Initialize();
				
				if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
					continue;
				tStr.Format(_LS(IDS_TMWK__MSG61), SectK, SectD.SName);
				InsertTreeItem(hT3,WORK_USRS_LST,tStr, _T("ico24_treemenu_report_reinforcementofsections"));
			}
		}
		else
		{
			hT3 = InsertTreeItem(hT2,WORK_USRS_FLD,_LS(IDS_TM_DYNA_REPORT_Reinforcement_of_Sections), _T("ico24_treemenu_report_reinforcementofsections"));
		}
#endif

		// Special Tables - Composite Section for C.S.
		if(nCscsCnt>0)
		{
			hT3 = InsertTreeItem(hT2,WORK_USCS_FLD,_LS(IDS_TM_DYNA_REPORT_Composite_Section_for_CS), _T("ico24_treemenu_report_compositesectionforcs"));
			
			T_CSCS_K CscsK=0;
			T_CSCS_D CscsD;
			T_STAG_D StagD;
			for(int i=0; i<nCscsCnt; i++)
			{
				CscsK = aCscsKeyList[i];
				CscsD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
					continue;
				
				SectK = CscsD.SectKey;
				SectD.Initialize();
				StagD.Initialize();
				
				if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
					continue;
				if(!pDoc->m_pAttrCtrl->GetStag(CscsD.ActStagKey, StagD))
					continue;
				tStr.Format(_LS(IDS_TM_DYNA_REPORT__d__d__s_Stage__s_Type_s), 
					i+1, CscsD.SectKey, SectD.SName, StagD.StageName, 
					CSectUtil::GetSectionTypeNameFromIndex(SectD.nStype));
				//tStr.Format(_T("%d : %s (%d)"), SectK, SectD.SName, CscsK);
				InsertTreeItem(hT3,WORK_USCS_LST,tStr, _T("ico24_treemenu_report_compositesectionforcs"));
			}
		}
		else
		{
			hT3 = InsertTreeItem(hT2,WORK_USCS_FLD,_LS(IDS_TM_DYNA_REPORT_Composite_Section_for_CS), _T("ico24_treemenu_report_compositesectionforcs"));
		}

	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_USTB_FLD,_LS(IDS_TM_DYNA_REPORT_Special_Tables), _T("ico24_treemenu_report_specialtables"));
	}
	
	// Table Templates
	T_UTPL_D UtplD;
	if(nUtpl>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_UTPL_FLD,_LS(IDS_TM_DYNA_REPORT_Table_Templates), _T("ico24_treemenu_report_tabletemplates"));
		
		for(int i=0; i<nUtpl; i++)
		{
			pDoc->m_pAttrCtrl->GetUtpl(aUtplK[i], UtplD);
			tStr = UtplD.strName;
			InsertTreeItem(hT2,WORK_UTPL_LST,tStr, _T("ico24_treemenu_report_tabletemplates"));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_UTPL_FLD,_LS(IDS_TM_DYNA_REPORT_Table_Templates), _T("ico24_treemenu_report_tabletemplates"));
	}

	//if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	if(hTI!=NULL)
	{
		if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
		else ExpandCompletely(hTI, TRUE);
	}
	return hTI;
}

// Dynamic Report Chart, 20090604 KIMJM
HTREEITEM CTreeReportCtrl::SetDynReportChartItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	CArray<T_UCHT_K, T_UCHT_K> aKeyList;
	T_UCHT_D data;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aKeyList);
	int nCount = aKeyList.GetSize();
	if(nCount==0)
	{
		int nTmp=1;
		hTI = SetSub1TreeItem(WORK_UCHT_FLD,bInit,nTmp,10);
	}
	else
	{
		hTI = SetSub1TreeItem(WORK_UCHT_FLD,bInit,nCount,10);
	}
	
	if(hTI==NULL) return NULL;
	
	for(int i=0; i<nCount; i++)
	{
		pDoc->m_pAttrCtrl->GetUcht(aKeyList[i], data);
		tStr = data.strName;
		InsertTreeItem(hTI,WORK_UCHT_LST,tStr, _T("ico24_treemenu_report_charts"));
	}

	// workstree 색표시 
	SetUchtRes();

	//if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	if(hTI!=NULL)
	{
		if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
		else ExpandCompletely(hTI, TRUE);
	}
	return hTI;
}


// Summary List만 출력함, 20090604 KIMJM
HTREEITEM CTreeReportCtrl::SetDynReportTextItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
{
	HTREEITEM hTI=NULL, hT2=NULL;
	CString   tStr;
	int nCount = 0;

	CArray<T_USUM_K, T_USUM_K> aUsumK;
	T_USUM_D UsumD;
	pDoc->m_pAttrCtrl->GetUsumKeyList(aUsumK);
	int nUsum = aUsumK.GetSize();

#if defined(_MGEN)
	nCount = D_REPORT_TEXT_CNT+nUsum;
#elif(_CIVIL)
	nCount = D_REPORT_TEXT_CNT-6+nUsum;
#else
	ASSERT(0);
#endif

	if(nCount==0)
		hTI = SetSub1TreeItem(WORK_DYTT_FLD,bInit,11,11);
	else
		hTI = SetSub1TreeItem(WORK_DYTT_FLD,bInit,nCount,11);
	if(hTI==NULL) return NULL;

	CString strText=_T("");
	CString strTemp=_T("");
	int nTextCnt=0;

	// CReportAutoRegeneration도 같이 수정할 것!
	// Workstree에 보여줄 용도로..
#if defined(_MGEN)
	m_aReportTextCaption.RemoveAll();
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Control_Data));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Story));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Node));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Boundary));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Floor_Diaphragm_Rigid_Link));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Truss));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Wall));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
	m_aReportTextCaption.Add(_LS(IDS_AMR_Wind_Load));
	m_aReportTextCaption.Add(_LS(IDS_AMR_Seismic_Load));
	m_aReportTextCaption.Add(_LS(IDS_AMR_Load_Combination));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Self_Weight));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Finishing_Material_Load));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_System_Temperature));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	//m_aReportTextCaption.Add(_LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA)); // 원래 Gen에 data가 없어서 삭제. 20100221 KIMJM
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
	m_aReportTextCaption.FreeExtra();

	nTextCnt=D_REPORT_TEXT_CNT;
	ASSERT(nTextCnt==m_aReportTextCaption.GetSize());
	hT2 = InsertTreeItem(hTI,WORK_UTXT_FLD,_LS(IDS_TM_DYNA_REPORT_Defined_Texts), _T("ico24_treemenu_report_definedtext"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = m_aReportTextCaption[i];
		InsertTreeItem(hT2,WORK_UTXT_LST,strText, _T("ico24_treemenu_report_definedtext"));
	}

	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	m_aReportTextStrName.RemoveAll();
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Control_Data));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Story));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Node));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Boundary));
	m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Floor_Diaphragm_Rigid_Link)); // 기호 포함
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Truss));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Wall));
	m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area)); // 기호 포함
	m_aReportTextStrName.Add(_LS(IDS_AMR_Wind_Load));
	m_aReportTextStrName.Add(_LS(IDS_AMR_Seismic_Load));
	m_aReportTextStrName.Add(_LS(IDS_AMR_Load_Combination));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Self_Weight));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Finishing_Material_Load));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_System_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	//m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load)); // 문자열 길이가 28자 이상이므로 줄임.
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
	m_aReportTextStrName.FreeExtra();

#elif(_CIVIL)
	CStringArray aReportTextSVG;
	m_aReportTextCaption.RemoveAll();
	aReportTextSVG.RemoveAll();
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Control_Data));
	aReportTextSVG.Add(_T("ico24_tools_setting_preference"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
	aReportTextSVG.Add(_T("ico24_load_createloadcases_staticloadcases"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Node));
	aReportTextSVG.Add(_T("ico24_nodeelement_nodes_nodestable"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Boundary));
	aReportTextSVG.Add(_T("ico24_boundary_tables_boundarytables"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam));
	aReportTextSVG.Add(_T("ico24_structure_wizard_beam"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Truss));
	aReportTextSVG.Add(_T("ico24_structure_wizard_truss"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
	aReportTextSVG.Add(_T("ico24_query_detailtable_elementweighttable"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_Load_Combination));
	aReportTextSVG.Add(_T("ico24_results_combination_loadcombination"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Self_Weight));
	aReportTextSVG.Add(_T("ico24_load_structureloadsmasses_selfweight"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
	aReportTextSVG.Add(_T("ico24_load_structureloadsmasses_nodalloads"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_System_Temperature));
	aReportTextSVG.Add(_T("ico24_load_temperatureloads_systemtemp"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
	aReportTextSVG.Add(_T("ico24_load_temperatureloads_nodaltemp"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
	aReportTextSVG.Add(_T("ico24_load_temperatureloads_elementtemp"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	aReportTextSVG.Add(_T("ico24_load_temperatureloads_beamsectiontemp"));
	//m_aReportTextCaption.Add(_LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
	aReportTextSVG.Add(_T("ico24_pushover_control_globalcontrol"));
	m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
	aReportTextSVG.Add(_T("ico24_pushover_loadcase_loadcase"));
	m_aReportTextCaption.FreeExtra();
	aReportTextSVG.FreeExtra();

	nTextCnt=D_REPORT_TEXT_CNT-6;
	ASSERT(nTextCnt == m_aReportTextCaption.GetSize());
	ASSERT(aReportTextSVG.GetSize() == m_aReportTextCaption.GetSize());
	hT2 = InsertTreeItem(hTI,WORK_UTXT_FLD,_LS(IDS_TM_DYNA_REPORT_Defined_Texts), _T("ico24_treemenu_report_definedtext"));
	for(int i=0; i<nTextCnt; i++)
	{
		InsertTreeItem(hT2,WORK_UTXT_LST, m_aReportTextCaption[i], aReportTextSVG[i]);
	}

	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	m_aReportTextStrName.RemoveAll();
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Control_Data));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Node));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Boundary));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Truss));
	m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area)); // 기호 포함
	m_aReportTextStrName.Add(_LS(IDS_AMR_Load_Combination));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Self_Weight));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_System_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
	//m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load)); // 문자열 길이가 28자 이상이므로 줄임.
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
	m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
	m_aReportTextStrName.FreeExtra();
#else
	ASSERT(0);
#endif

	// User Defined Texts
	if(nUsum>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_USUM_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Texts), _T("ico24_treemenu_report_userdefinedtext"));
		
		for(int i=0; i<nUsum; i++)
		{
			pDoc->m_pAttrCtrl->GetUsum(aUsumK[i], UsumD);
			tStr = UsumD.strName;
			InsertTreeItem(hT2,WORK_USUM_LST,tStr, _T("ico24_treemenu_report_userdefinedtext"));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_USUM_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Texts), _T("ico24_treemenu_report_userdefinedtext"));
	}

	//if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	if(hTI!=NULL)
	{
		if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
		else ExpandCompletely(hTI, TRUE);
	}
	return hTI;
}
HTREEITEM CTreeReportCtrl::SetDynReportTextItemTree_CH( CDBDoc* pDoc,BOOL bInit/*=TRUE*/ )
{
	
	HTREEITEM hTI=NULL, hT2=NULL;
	CString   tStr;
	int nCount = 0;
	
	CArray<T_USUM_K, T_USUM_K> aUsumK;
	T_USUM_D UsumD;
	pDoc->m_pAttrCtrl->GetUsumKeyList(aUsumK);
	int nUsum = aUsumK.GetSize();
	
#if defined(_MGEN)
	nCount = D_REPORT_TEXT_CNT+nUsum;
#elif(_CIVIL)
	nCount = D_REPORT_TEXT_CNT-16+nUsum;
#else
	ASSERT(0);
#endif
	
	if(nCount==0)
		hTI = SetSub1TreeItem(WORK_DYTT_FLD,bInit,11,11);
	else
		hTI = SetSub1TreeItem(WORK_DYTT_FLD,bInit,nCount,11);
	if(hTI==NULL) return NULL;
	
	CString strText=_T("");
	CString strTemp=_T("");
	int nTextCnt=0;

	m_aReportTextCaption.RemoveAll();
	m_aReportTextStrName.RemoveAll();
	m_mapCHReportTextCaptions.clear();
	m_mapCHReportTextStrNames.clear();
	vector<CString> itemNames;
	
	itemNames.clear();
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_General     ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_Standard    ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_Code        ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_Structure   ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_Material    ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_BasicInfo_Calculation ));
	
	nTextCnt=D_REPORT_TEXT_CNT-16;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_UBSI_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_BASICINFO ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_UBSI_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}
	
	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	
			
	itemNames.clear();
	itemNames.push_back(_LS(IDS_AMR_CHRP_Model_Calculation     ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_Model_Steel    ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_Model_Section        ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_Model_Load   ));
	
	
	nTextCnt=D_REPORT_TEXT_CNT-18;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_UMLD_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_MODEL ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_UMLD_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}

	itemNames.clear();
	itemNames.push_back(_LS(IDS_AMR_CHRP_Force_MemberForce     ));
	
	
	nTextCnt=D_REPORT_TEXT_CNT-21;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_FCMB_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_FORCE ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_FCMB_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}
	
	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	
	itemNames.clear();  
	itemNames.push_back(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth     ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_LoadCapacity_Bending    ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing        ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion   ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_LoadCapacity_Reforce   ));

	m_aReportTextCaption.FreeExtra();
	nTextCnt=D_REPORT_TEXT_CNT-17;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_UBCL_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_LOADCAPACITY ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_UBCL_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}

 
	itemNames.clear();  
	itemNames.push_back(_LS(IDS_AMR_CHRP_ServiceAbility_Normal     ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique    ));
	nTextCnt=D_REPORT_TEXT_CNT-20;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_UNUL_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SERVICEABILITY ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_UNUL_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}

	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	
	itemNames.clear();  
	itemNames.push_back(_LS(IDS_AMR_CHRP_Stress_NormalComp     ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_Stress_Tension    ));
	itemNames.push_back(_LS(IDS_AMR_CHRP_Stress_PrincipalComp    ));
	nTextCnt=D_REPORT_TEXT_CNT-19;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_ULSC_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_STRESS ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_ULSC_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}

	

	itemNames.clear();  
	itemNames.push_back(_LS(IDS_AMR_CHRP_Stress_ShortTerm     ));
	nTextCnt=D_REPORT_TEXT_CNT-21;
	ASSERT(nTextCnt==itemNames.size());
	hT2 = InsertTreeItem(hTI,WORK_CHRP_USSC_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SHORTTERMSTRESS ), _T("ico24_treemenu_report_userdefinedimages"));
	for(int i=0; i<nTextCnt; i++)
	{
		strText = itemNames[i];
		InsertTreeItem(hT2,WORK_CHRP_USSC_LST,strText, _T("ico24_treemenu_report_userdefinedimages"));
	}
	if(hT2 != NULL)
	{
		m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
		m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
	}

	// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
	
	if(nUsum>0)
	{
		hT2 = InsertTreeItem(hTI,WORK_USUM_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Texts), _T("ico24_treemenu_report_userdefinedimages"));
		itemNames.clear();  
		for(int i=0; i<nUsum; i++)
		{
			pDoc->m_pAttrCtrl->GetUsum(aUsumK[i], UsumD);
			tStr = UsumD.strName;
			InsertTreeItem(hT2,WORK_USUM_LST,tStr, _T("ico24_treemenu_report_userdefinedimages"));
			itemNames.push_back(tStr);
		}
		if(hT2 != NULL)
		{
			m_mapCHReportTextCaptions.push_back(make_pair(hT2,itemNames));
			m_mapCHReportTextStrNames.push_back(make_pair(hT2,itemNames));
		}
	}
	else
	{
		hT2 = InsertTreeItem(hTI,WORK_USUM_FLD,_LS(IDS_TM_DYNA_REPORT_User_Defined_Texts), _T("ico24_treemenu_report_userdefinedimages"));
	}
	
	//if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
	if(hTI!=NULL)
	{
		if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
		else ExpandCompletely(hTI, TRUE);
	}
	return hTI;
}
// Dynamic Report Image File, 20090617 KIMJM
// HTREEITEM CTreeReportCtrl::SetDynReportImageFileItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/)
// {
//   HTREEITEM hTI=NULL;
//   CString   tStr;
//   CArray<T_UIMG_K, T_UIMG_K> aKeyList;
//   T_UIMG_D data;
//   pDoc->m_pAttrCtrl->GetUimgKeyList(aKeyList);
//   int nCount = aKeyList.GetSize();
//   if(nCount==0)
//   {
//     int nTmp=1;
//     hTI = SetSub1TreeItem(WORK_UIMG_FLD,bInit,nTmp,1);
//   }
//   else
//   {
//     hTI = SetSub1TreeItem(WORK_UIMG_FLD,bInit,nCount,1);
//   }
//   
//   if(hTI==NULL) return NULL;
//   
//   for (int i = 0; i < nCount; i++)
//   {
//     pDoc->m_pAttrCtrl->GetUimg(aKeyList[i], data);
//     tStr = data.strName;
//     InsertTreeItem(hTI,WORK_UIMG_LST,tStr,_T("ico24_temp"));
//   }
//   //if(hTI!=NULL) ExpandCompletely(hTI, TRUE);
//   if(hTI!=NULL)
//   {
//     if(bInit) Expand(hTI, TVE_EXPAND);// 최초에 보여줄 때는 한단계만...
//     else ExpandCompletely(hTI, TRUE);
//   }
//   return hTI;
// }

////////////////////////////////////////////////////////////////////////////
void CTreeReportCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	BOOL  bSelectPlus;
	short nKeyState = GetKeyState(VK_LCONTROL);
	if (nKeyState & 0x8000) bSelectPlus = FALSE;
	else bSelectPlus = TRUE;

	if (FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		if(Data == WORK_UFIG_LST) // Dynamic Report Figure
		{
			SetDynReportFigureToView(aItem[0]);
		}
		else if(Data == WORK_UTBL_LST) // Dynamic Report Table
		{
			ViewDynReportTable(aItem[0]);
		}
		else
		{
			//if (SelectEntity(Data,bSelectPlus)==FALSE) // Select 명령 없이 Property 명령만 있는 경우
			{
				if (aItem.GetSize() > 1) 
				{
					AfxMessageBox(_LS(IDS_TMWK__MSG170)); 
					return;
				}
				EditProperty(aItem[0]);
			}
		}
	}
	*pResult = 0;
}

BOOL CTreeReportCtrl::GetSelectedItemKeyList(UINT data,CArray<T_NODE_K, T_NODE_K> &rNode,CArray<T_ELEM_K, T_ELEM_K> &rElem)
{
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	rNode.RemoveAll();
	rElem.RemoveAll();

	HTREEITEM ht,prvHt;
	int pos = 0;
	
	// 선택한 아이템의 위치 찾기

	ht = GetSelectedItem();
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	//=== 무엇을 선택했을 까??? ===
// 	if(data == WORK_NODE_FLD) // Node로 selection
//   {
//    	pDoc->m_pAttrCtrl->GetNodeKeyList(rNode);
//     return TRUE;
//   }
// 	if(data == WORK_SPAN_LST) // Span selection
//   {
// 	  CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
//     GetSelectedSpanKeyListWithMultiTreeItemSelect(aSpanKeyList,rElem);
//     return TRUE;
//   }
//   else return FALSE;

	return TRUE;
}

BOOL CTreeReportCtrl::SelectEntity(UINT data,BOOL bReplace)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	BOOL bResult = GetSelectedItemKeyList(data,aNodeList,aElemList);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	pDoc->m_pViewCtrl->Select(NULL, aNodeList, aElemList, FALSE, bReplace);
	return bResult;
}

void CTreeReportCtrl::UnselectEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data,aNodeList,aElemList);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}
	pDoc->m_pViewCtrl->Unselect(NULL, aNodeList, aElemList, FALSE);	
}

void CTreeReportCtrl::ActiveEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data,aNodeList,aElemList);
	
	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->ActiveThisObjects(aNodeList, aElemList);
}

void CTreeReportCtrl::ActiveEntityPlus(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data,aNodeList,aElemList);
	
	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
//  if (pIGM) pIGM->ManagementObject_ActOrInAct(aNodeList, aElemList, TRUE, 1);
	if (pIGM) pIGM->ManagementObject_ActOrInAct(aNodeList, aElemList, FALSE, 1);
}

void CTreeReportCtrl::InactiveEntity(UINT data)
{
	CArray<T_NODE_K, T_NODE_K> aNodeList;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	GetSelectedItemKeyList(data,aNodeList,aElemList);
	
	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->InActiveThisObjects(aNodeList, aElemList);
}

void CTreeReportCtrl::DeleteItemEntity(CArray<HTREEITEM, HTREEITEM> &aItem)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	HTREEITEM parentItem = GetParentItem(aItem[0]);
	UINT parentData = GetItemData(parentItem);

	UINT data = GetItemData(aItem[0]);
	BOOL bSuccess=FALSE;

	if (data == WORK_DYIM_FLD)  // Dynamic Report Image
	{	
		bSuccess = DeleteItemImage(data);
	}	
	else if (data == WORK_UFIG_FLD)  // Dynamic Report Figure
	{	
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUfigList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUfig(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_UFIG_LST)  // Dynamic Report Figure
	{	
		CArray<CString, CString&> aName;
		GetSelectedUfigNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUfig(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_UIMG_FLD)  // Dynamic Report Image File
	{
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUimgList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUimg(aName);
		aName.RemoveAll();    
	}
	else if (data == WORK_UIMG_LST)  // Dynamic Report Image File
	{
		CArray<CString, CString&> aName;
		GetSelectedUimgNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUimg(aName);
		aName.RemoveAll();    
	}
	else if (data == WORK_DYTB_FLD)  // Dynamic Report Table
	{	
		bSuccess = DeleteItemTable(data);
	}	
	else if (data == WORK_UTBL_FLD)  // Dynamic Report Table - User Defined Table
	{	
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUtblList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUtbl(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_UTBL_LST)  // Dynamic Report Table - User Defined Table
	{	
		CArray<CString, CString&> aName;
		GetSelectedUtblNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUtbl(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_USTB_FLD)  // Dynamic Report Table - Special Table
	{
//     CArray<CString, CString&> aName;
//     pDoc->m_pAttrCtrl->GetUstbList(aName);
//     bSuccess = pDoc->m_pDataCtrl->DelUstb(aName);
//     aName.RemoveAll();    
	}
//   else if (data == WORK_USTB_LST)  // Dynamic Report Table - Special Table
//   {
//     CArray<CString, CString&> aName;
//     GetSelectedUstbNameListWithMultiTreeItemSelect(aName);
//     bSuccess = pDoc->m_pDataCtrl->DelUstb(aName);
//     aName.RemoveAll();    
//   }
	else if (data == WORK_UTPL_FLD)  // Dynamic Report Table - Table Template
	{
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUtplList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUtpl(aName);
		aName.RemoveAll();    
	}
	else if (data == WORK_UTPL_LST)  // Dynamic Report Table - Table Template
	{
		CArray<CString, CString&> aName;
		GetSelectedUtplNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUtpl(aName);
		aName.RemoveAll();    
	}
	else if (data == WORK_UCHT_FLD)  // Dynamic Report Chart
	{	
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUchtList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUcht(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_UCHT_LST)  // Dynamic Report Chart
	{	
		CArray<CString, CString&> aName;
		GetSelectedUchtNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUcht(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_USUM_FLD || data == WORK_DYTT_FLD)  // Dynamic Report Text (Design Summary)
	{	
		CArray<CString, CString&> aName;
		pDoc->m_pAttrCtrl->GetUsumList(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUsum(aName);
		aName.RemoveAll();    
	}	
	else if (data == WORK_USUM_LST)  // Dynamic Report Text (Design Summary)
	{	
		CArray<CString, CString&> aName;
		GetSelectedUsumNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelUsum(aName);
		aName.RemoveAll();    
	}	


	// Delete이 수행되지 못했을 경우 Caret 상태 해제 
	if (!bSuccess)
		UnselectAllItems();

	else SelectItem(NULL);
}

BOOL CTreeReportCtrl::DeleteItemImage(UINT data)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	if(data !=WORK_DYIM_FLD) return FALSE;

	CArray<CString, CString&> aUfigName;
	CArray<CString, CString&> aUimgName;
	pDoc->m_pAttrCtrl->GetUfigList(aUfigName);
	pDoc->m_pAttrCtrl->GetUimgList(aUimgName);

	int nUfig = aUfigName.GetSize();
	int nUimg = aUimgName.GetSize();
	if(nUfig+nUimg == 0) return FALSE;
	
	if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_TM_DYNA_REPORT_Delete_Dynamic_Report_Image)), CMDTYPE_FREE)) return FALSE;
	
	for(int i=0; i<nUfig; i++)
	{
		aUfigName[i].TrimRight();
		if(!pDoc->m_pAttrCtrl->ExistUfig(aUfigName[i]))
		{
			return EndEdit(FALSE);
		}
		if(!pDoc->m_pEditData->DelUfig(aUfigName[i])) return EndEdit(FALSE);
	}
	for(int i=0; i<nUimg; i++)
	{
		aUimgName[i].TrimRight();
		if(!pDoc->m_pAttrCtrl->ExistUimg(aUimgName[i]))
		{
			return EndEdit(FALSE);
		}
		if(!pDoc->m_pEditData->DelUimg(aUimgName[i])) return EndEdit(FALSE);
	}
 
	return EndEdit(TRUE);
}

BOOL CTreeReportCtrl::DeleteItemTable(UINT data)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	if(data !=WORK_DYTB_FLD) return FALSE;
	
	CArray<CString, CString&> aUtblName;
	CArray<CString, CString&> aUtplName;
	pDoc->m_pAttrCtrl->GetUtblList(aUtblName);
	pDoc->m_pAttrCtrl->GetUtplList(aUtplName);
	
	int nUtbl = aUtblName.GetSize();
	int nUtpl = aUtplName.GetSize();
	if(nUtbl+nUtpl == 0) return FALSE;
	
	if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_TM_DYNA_REPORT_Delete_Dynamic_Report_Table)), CMDTYPE_FREE)) return FALSE;
	
	for(int i=0; i<nUtbl; i++)
	{
		aUtblName[i].TrimRight();
		if(!pDoc->m_pAttrCtrl->ExistUtbl(aUtblName[i]))
		{
			return EndEdit(FALSE);
		}
		if(!pDoc->m_pEditData->DelUtbl(aUtblName[i])) return EndEdit(FALSE);
	}
	for(int i=0; i<nUtpl; i++)
	{
		aUtplName[i].TrimRight();
		if(!pDoc->m_pAttrCtrl->ExistUtpl(aUtplName[i]))
		{
			return EndEdit(FALSE);
		}
		if(!pDoc->m_pEditData->DelUtpl(aUtplName[i])) return EndEdit(FALSE);
	}
	
	return EndEdit(TRUE);
}

BOOL CTreeReportCtrl::EndEdit(BOOL bEnd)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	if(bEnd && pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault=TRUE;  // Modify by pig
		pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, FALSE, 0);
		return TRUE;
	}
	else
	{
		pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	return FALSE;
}

void CTreeReportCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu     menu;
	CMenu    *pPopup;
	HMENU			hMenu;
	INIResourceManager::LoadMenuToINITranslate(hMenu,IDR_REPORT_CONTEXT, _T("wg_treemenu"), &menu);
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	UINT Data = GetItemData(aItem[0]);

	auto TrackPopupMenu = [this, &pPopup, point]()
	{
		if (g_pContextMenuManager != nullptr)
		{
			int nMenuID = g_pContextMenuManager->TrackPopupMenu(pPopup->GetSafeHmenu(), point.x, point.y, this);
			PostMessage(WM_COMMAND, MAKEWPARAM(nMenuID, 0), 0);
		}
		else
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	};

	switch(Data)
	{
	case WORK_DYST_FLD:
		pPopup = menu.GetSubMenu(0);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_UFTR_FLD:
		pPopup = menu.GetSubMenu(0);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_DYDB_FLD:
		pPopup = menu.GetSubMenu(7);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_DYIM_FLD:
	case WORK_UFIG_FLD:
	case WORK_DYTB_FLD:  
	case WORK_UTBL_FLD:  
	case WORK_UTPL_FLD:  
	case WORK_UCHT_FLD:   
	case WORK_USUM_FLD:  
		pPopup = menu.GetSubMenu(1);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_UFIG_LST:
		pPopup = menu.GetSubMenu(2);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	
	case WORK_UTPL_LST:    
		pPopup = menu.GetSubMenu(3);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_USUM_LST:
	case WORK_UIMG_LST: 
		pPopup = menu.GetSubMenu(4);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_DYTT_FLD:  
	case WORK_UTXT_FLD:  
	case WORK_UTXT_LST:  
	case WORK_USTB_FLD:  
	case WORK_USSS_FLD:
	case WORK_USRS_FLD:
	case WORK_USCS_FLD:
		pPopup = menu.GetSubMenu(5);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_CHRP_UBSI_LST://add by JUHONGHUA(MITC) 2012.2.14
	case WORK_CHRP_UMLD_LST:
	case WORK_CHRP_UBCL_LST:
	case WORK_CHRP_UNUL_LST:
	case WORK_CHRP_ULSC_LST:
	case WORK_CHRP_USSC_LST:
	case WORK_CHRP_UBSI_FLD:
	case WORK_CHRP_UMLD_FLD:
	case WORK_CHRP_UBCL_FLD:
	case WORK_CHRP_UNUL_FLD:
	case WORK_CHRP_ULSC_FLD:
	case WORK_CHRP_USSC_FLD:
		pPopup = menu.GetSubMenu(5);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_UIMG_FLD:  
		pPopup = menu.GetSubMenu(6);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_DYUN_FLD:
		pPopup = menu.GetSubMenu(8);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_USSS_LST:
	case WORK_USRS_LST:
	case WORK_USCS_LST:
		pPopup = menu.GetSubMenu(9);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_UCHT_LST:  
		pPopup = menu.GetSubMenu(10);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	case WORK_UTBL_LST:
		pPopup = menu.GetSubMenu(11);
		VERIFY(pPopup != NULL);
		TrackPopupMenu();
		break;
	}
}


void CTreeReportCtrl::OnWorktreeActive() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		ActiveEntity(Data);
	}
}


void CTreeReportCtrl::OnWorktreeActivePlus() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		ActiveEntityPlus(Data);
	}	
}

void CTreeReportCtrl::OnWorktreeDelete() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);

	if(selItemNum) DeleteItemEntity(aItem);
}

void CTreeReportCtrl::OnWorktreeInactive() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		InactiveEntity(Data);
	}
}

void CTreeReportCtrl::OnWorktreeSelect() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		SelectEntity(Data);
	}
}

void CTreeReportCtrl::OnWorktreeUnselect() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		UnselectEntity(Data);
	}
}

void CTreeReportCtrl::OnWorktreeUnselectall() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}
	pDoc->m_pViewCtrl->UnselectAll(NULL);	
}

void CTreeReportCtrl::OnWorktreeActiveall() 
{
	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	if (pIGM) pIGM->ActiveAll();
}

void CTreeReportCtrl::OnWorktreeProperties() 
{
	WPARAM wParam = 0;
	LPARAM lParam = MAKELPARAM(1, 1);
	SendMessage(WM_MOUSEMOVE, wParam, lParam);

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum>1) 
	{
		AfxMessageBox(_LS(IDS_TMWK__MSG170)); 
		return;
	}
	
	EditProperty(aItem[0]);
}

void CTreeReportCtrl::EditProperty(HTREEITEM ht)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	UINT data = GetItemData(ht);
	// 선택한 아이템의 위치 찾기

	HTREEITEM prvHt,curHt=ht;
	int pos = 0;
	
	for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);

	if(data == WORK_DYST_FLD) // Dynamic Report Setup
	{
		CReportSetupDlg dlg;
		dlg.DoModal();
	}
	else if(data == WORK_UFTR_FLD) // Dynamic Report Header/Footer
	{
		CReportHeaderFooterDlg dlg;
		dlg.DoModal();
	}
	else if(data == WORK_DYDB_FLD) // Dynamic Report Reference DB
	{
		CFileDialog fd(TRUE, D_MODEL_FILE_EXT, NULL, 
									 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
									 D_MODEL_FILE_FILTER, NULL);
		if(fd.DoModal() == IDOK)
		{
			CString strFileName = fd.GetPathName();
			
			CReportRefDBDlg dlg;
			dlg.SetModelPathName(strFileName);
			dlg.DoModal();
		}

//     CReportRefDBDlg dlg;
//     dlg.DoModal();
	}
	else if(data == WORK_UFIG_LST) // Dynamic Report Figure
	{
		CArray<T_UFIG_K, T_UFIG_K> aUfigK;
		pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
		int nUfigNum = aUfigK.GetSize();
		ASSERT(nUfigNum>0);

		T_UFIG_K UfigK=0;
		UfigK = aUfigK[pos-1];

		CGPSAMRUDFDlg dlg;
		dlg.SetData(UfigK);
		dlg.DoModal();
	}
	else if(data == WORK_UCHT_LST) // Dynamic Report Chart
	{
//     CArray<T_UCHT_K, T_UCHT_K> aUchtK;
//     pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
//     int nUchtNum = aUchtK.GetSize();
//     ASSERT(nUchtNum>0);
//     
//     T_UCHT_K UchtK=0;
//     UchtK = aUchtK[pos-1];
//     VERIFY(pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD));
// 
//     T_CHART_ITEM_D ChartItemD;
//     CGPSAMRUDCData GenChart;
//     CString strFilePath=_T("");
//     GenChart.GetData(UchtK, ChartItemD, strFilePath);
// 
//     pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD); 
	}
	else if(data == WORK_UTPL_LST) // Dynamic Report Table Template
	{
		CArray<T_UTPL_K, T_UTPL_K> aUtplK;
		pDoc->m_pAttrCtrl->GetUtplKeyList(aUtplK);
		int nUtplNum = aUtplK.GetSize();
		ASSERT(nUtplNum>0);
		
		T_UTPL_K UtplK=0;
		UtplK = aUtplK[pos-1];

		CReportTBTemplateDlg dlg;
		dlg.SetData(UtplK);
		dlg.DoModal();
	}
	else if(data == WORK_UTBL_LST) // Dynamic Report Table
	{
		CArray<T_UTBL_K, T_UTBL_K> aUtblK;
		pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
		int nUtblNum = aUtblK.GetSize();
		ASSERT(nUtblNum>0);
		
		T_UTBL_K UtblK=0;
		UtblK = aUtblK[pos-1];
		
		CReportTableDlg dlg;
		dlg.SetData(UtblK);
		dlg.DoModal();
	}
	else if(data == WORK_DYUN_FLD)
	{
		CReportUnitCustomizeDlg dlg;
		dlg.DoModal();
	}
} 

void CTreeReportCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem; aItem.RemoveAll();
	int selItemNum = GetSelectedItemList(aItem);
	UINT data = GetItemData(aItem[0]);
	//if(selItemNum>1) { Data=0; } // 두 개 이상의 아이템이 선택되어 있으면 Assign 불가!!

	int nDataType=0;  // 1: 1st FLD, 2: 2nd FLD, 3: LST
	switch(data)
	{
		case WORK_UFTR_FLD:
		case WORK_UCHT_FLD:
		case WORK_UFIG_FLD:
		case WORK_UTBL_FLD:
		case WORK_DYTT_FLD:
		case WORK_UTXT_FLD:
		case WORK_USUM_FLD:
		case WORK_USTB_FLD:  
		case WORK_USSS_FLD:  
		case WORK_USRS_FLD:  
		case WORK_USCS_FLD:  
		case WORK_UIMG_FLD:  
		case WORK_UTPL_FLD:
			nDataType = D_DATATYPE_1ST_FLD;
			break;
		case WORK_UCHT_LST:
		case WORK_UFIG_LST:
		case WORK_UTBL_LST:  
		case WORK_UTXT_LST:
		case WORK_USUM_LST:
		case WORK_USSS_LST:
		case WORK_USRS_LST:
		case WORK_USCS_LST:
		case WORK_UIMG_LST:
		case WORK_UTPL_LST:
			nDataType = D_DATATYPE_LST;
			break;
		case WORK_CHRP_UBSI_LST:  //add by JUHONGHUA(MITC) 2012.2.14
		case WORK_CHRP_UMLD_LST:  
		case WORK_CHRP_UBCL_LST:
		case WORK_CHRP_UNUL_LST:
		case WORK_CHRP_ULSC_LST:  
		case WORK_CHRP_USSC_LST:  
			nDataType = D_DATATYPE_LST;
			break;
		case WORK_CHRP_UBSI_FLD:  //add by JUHONGHUA(MITC) 2012.2.14
		case WORK_CHRP_UMLD_FLD:  
		case WORK_CHRP_UBCL_FLD:
		case WORK_CHRP_UNUL_FLD:
		case WORK_CHRP_ULSC_FLD:  
		case WORK_CHRP_USSC_FLD:  
			nDataType = D_DATATYPE_1ST_FLD;
			break;
		default:
			nDataType = 0;
			break;
	}

	switch(data)
	{
		case WORK_UFTR_FLD:
		case WORK_UCHT_FLD:
		case WORK_UCHT_LST:  
		case WORK_UFIG_FLD:
		case WORK_UFIG_LST:
		case WORK_UTBL_FLD:
		case WORK_UTBL_LST:  
		case WORK_DYTT_FLD:  
		case WORK_UTXT_FLD:  
		case WORK_UTXT_LST:
		case WORK_USUM_FLD:  
		case WORK_USUM_LST:
		case WORK_USTB_FLD:
		case WORK_USSS_FLD:  
		case WORK_USSS_LST:  
		case WORK_USRS_FLD:  
		case WORK_USRS_LST:
		case WORK_USCS_FLD:
		case WORK_USCS_LST:
		case WORK_UIMG_FLD:
		case WORK_UIMG_LST:
		case WORK_UTPL_FLD:
		case WORK_UTPL_LST:
		case WORK_CHRP_UBSI_LST:  //add by JUHONGHUA(MITC) 2012.2.14
		case WORK_CHRP_UMLD_LST:  
		case WORK_CHRP_UBCL_LST:
		case WORK_CHRP_UNUL_LST:
		case WORK_CHRP_ULSC_LST:  
		case WORK_CHRP_USSC_LST:  
		case WORK_CHRP_UBSI_FLD:  //add by JUHONGHUA(MITC) 2012.2.14
		case WORK_CHRP_UMLD_FLD:  
		case WORK_CHRP_UBCL_FLD:
		case WORK_CHRP_UNUL_FLD:
		case WORK_CHRP_ULSC_FLD:  
		case WORK_CHRP_USSC_FLD:  

			{
				BOOL bAMRWordView = pDoc->IsActiveAMRWordView();
				if(bAMRWordView)
				{
					// 마우스 커서를 드래깅 모드로
	          ::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
	          ::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ASSIGN));
						m_bDragPoss = TRUE;
					break;
				}
			}
 		default:
				// 마우스 커서를 드래깅 불가능 모드로
	      ::SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));	
	      ::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_NOASSIGN));
				m_bDragPoss = FALSE;
			break;
	}
	m_bDrag = TRUE;
	//m_hDragItem = GetSelectedItem();
	m_ahDragItems.RemoveAll();
	if      (nDataType==D_DATATYPE_1ST_FLD)  m_ahDragItems.Add(aItem[0]);
	else if (nDataType==D_DATATYPE_LST    )  m_ahDragItems.Copy(aItem);
	else  { ASSERT(0); }
	m_ahDragItems.FreeExtra();
	m_nDataType = nDataType;


	CWnd* pWnd = GetCapture();
	if(pWnd == NULL)
	{
		BOOL bCheck = ReleaseCapture();
		if(bCheck == FALSE)
		{
			AfxMessageBox(_T("Error"));
		}
		else
		{
			SetCapture();
		}
	}
	else
		SetCapture();
	*pResult = 0;
}

void CTreeReportCtrl::OnCaptureChanged(CWnd* pWnd)
{
	if (m_bDrag && pWnd == 0)
		::SetCapture(this->GetSafeHwnd());
}

LRESULT CTreeReportCtrl::OnNcHitTest(CPoint point)
{
	BOOL bRet = MBaseTreeCtrl::OnNcHitTest(point);
	return bRet;
}

void CTreeReportCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDrag) // 드래깅 모드이면 
	{
		SetClassLongPtr(this->m_hWnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));		
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_bDrag = FALSE;
		ReleaseCapture();
		if(m_bDragPoss==TRUE) EndDrag(point);
		m_bDragPoss = FALSE;
	}
	MBaseTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTreeReportCtrl::EndDrag(CPoint point)
{
	ClientToScreen(&point);
	CWnd* pWnd = CWnd::WindowFromPoint(point);
	if(pWnd == NULL) return;
	
	InsertReportItem();

	m_nDataType = 0;
	m_ahDragItems.RemoveAll();
}

void CTreeReportCtrl::InsertReportItem() 
{
	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		if(data == WORK_UCHT_FLD) // Dynamic Report Chart
		{
			InsertReportItemChart();
		}
		if(data == WORK_UFTR_FLD) // Dynamic Report Header & Footer
		{
			InsertReportHeaderFooter();
		}
		else if(data == WORK_DYIM_FLD) // Dynamic Report Image
		{
			InsertReportItemImageAll();
		}
		else if(data == WORK_UFIG_FLD) // Dynamic Report Figure
		{
			InsertReportItemImage();
		}
		else if(data == WORK_DYTB_FLD) // Dynamic Report Table All
		{
			InsertReportItemTableAll();
		}
		else if(data == WORK_UTBL_FLD) // Dynamic Report Table
		{
			InsertReportItemTable();
		}
		else if(data == WORK_DYTT_FLD) // Dynamic Report Text All
		{
			InsertReportItemTextAll();
		}
		else if(data == WORK_UTXT_FLD) // Dynamic Report Text
		{
			InsertReportItemText();
		}
		else if(data == WORK_USUM_FLD) // Dynamic Report Text (Summary)
		{
			InsertReportItemSummary();
		}
		else if(data == WORK_USTB_FLD) // Dynamic Report Special Table
		{
			InsertReportItemSpecialTable();
		}
		else if(data == WORK_USSS_FLD)  // Dynamic Report Special Table(Section Summary)
		{
			InsertReportItemSSTable();
		}
		else if(data == WORK_USRS_FLD)  // Dynamic Report Special Table(Reinforcement of Sections)
		{
			InsertReportItemRSTable();
		}
		else if(data == WORK_USCS_FLD)  // Dynamic Report Special Table(Composite Section for C.S.)
		{
			InsertReportItemCSTable();
		}
		else if(data == WORK_UTPL_FLD) // Dynamic Report Table Template
		{
			InsertReportItemTableTemplate();
		}
		else if(data == WORK_UIMG_FLD) // Dynamic Report Image File
		{
			InsertReportItemImageFile();
		}
		else if(data == WORK_CHRP_UBSI_FLD)
		{
			InsertReportItemText_BasicInfo();
		}
		else if(data == WORK_CHRP_UMLD_FLD)
		{
			InsertReportItemText_Model();
		}
		else if(data == WORK_CHRP_UBCL_FLD)
		{
			InsertReportItemText_LoadCapacity();
		}
		else if(data == WORK_CHRP_UNUL_FLD)
		{
			InsertReportItemText_ServiceAbility();
		}
		else if(data == WORK_CHRP_ULSC_FLD)
		{
			InsertReportItemText_Stress();
		}
		else if(data == WORK_CHRP_USSC_FLD)
		{
			InsertReportItemText_Stress_ShortTerm();
		}
		else if(data == WORK_CHRP_FCMB_FLD)
		{
			InsertReportItemText_ForceMemb();
		}
	}
	else if(m_nDataType==D_DATATYPE_LST)
	{
		if(data == WORK_UCHT_LST) // Dynamic Report Chart
		{
			InsertReportItemChart();
		}
		else if(data == WORK_UFIG_LST) // Dynamic Report Figure
		{
			InsertReportItemImage();
		}
		else if(data == WORK_UTBL_LST) // Dynamic Report Table
		{
			InsertReportItemTable();
		}
		else if(data == WORK_UTXT_LST) // Dynamic Report Text
		{
			InsertReportItemText();
		}
		else if(data == WORK_USUM_LST) // Dynamic Report Text (Summary)
		{
			InsertReportItemSummary();
		}
		else if(data == WORK_USSS_LST)  // Dynamic Report Special Table(Section Summary)
		{
			InsertReportItemSSTable();
		}
		else if(data == WORK_USRS_LST)  // Dynamic Report Special Table(Reinforcement of Sections)
		{
			InsertReportItemRSTable();
		}
		else if(data == WORK_USCS_LST)  // Dynamic Report Special Table(Composite Section for C.S.)
		{
			InsertReportItemCSTable();
		}
		else if(data == WORK_UTPL_LST) // Dynamic Report Table Template
		{
			InsertReportItemTableTemplate();
		}
		else if(data == WORK_UIMG_LST) // Dynamic Report Image File
		{
			InsertReportItemImageFile();
		}
		else if(data == WORK_CHRP_UBSI_LST)
		{
			InsertReportItemText_BasicInfo();
		}
		else if(data == WORK_CHRP_UMLD_LST)
		{
			InsertReportItemText_Model();
		}
		else if(data == WORK_CHRP_UBCL_LST)
		{
			InsertReportItemText_LoadCapacity();
		}
		else if(data == WORK_CHRP_UNUL_LST)
		{
			InsertReportItemText_ServiceAbility();
		}
		else if(data == WORK_CHRP_ULSC_LST)
		{
			InsertReportItemText_Stress();
		}
		else if(data == WORK_CHRP_USSC_LST)
		{
			InsertReportItemText_Stress_ShortTerm();
		}
		else if(data == WORK_CHRP_FCMB_LST)
		{
			InsertReportItemText_ForceMemb();
		}
	}
}

// Inset Chart Items
void CTreeReportCtrl::InsertReportItemChart() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_UCHT_LST && data != WORK_UCHT_FLD) return;

	CArray<T_UCHT_K, T_UCHT_K> aUchtK;  aUchtK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	int nUchtNum = aUchtK.GetSize();
	if(nUchtNum<=0)
	{
		ASSERT(0); return;
	}

	// (Unit Customizing) 현재단위 백업
	T_UNIT_INDEX UnitIndex_cur;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);

	T_UCHT_K  UchtK=0;
	T_UCHT_D  UchtD;  UchtD.Initialize(0);

	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;
	CString strFilePath=_T("");

	CStringArray  aCaption ; // Caption
	CStringArray  aName    ; // T_UCHT_D::strName (추출정보이름)
	CStringArray  aFilePath; // *.bmp Full Path
	aCaption.RemoveAll();
	aName.RemoveAll();
	aFilePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UchtK = 0;
			UchtD.Initialize(0);
			ChartItemD.Initialize();
			strFilePath=_T("");

			UchtK = aUchtK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
				continue;

			// Pushover 결과 있는지 검사.
			if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
				( /*UchtD.nType==2 || */UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
				continue;

			// (Unit Customizing) Defined 단위계로 변경
			if(UchtD.bDefinedUnit == TRUE)
			{
				T_UNIT_INDEX UnitIndex;
				UnitIndex.nBase_Length = UchtD.nUnitLength;
				UnitIndex.nBase_Force  = UchtD.nUnitForce ;
				UnitIndex.nBase_Heat   = UchtD.nUnitHeat 	;
				UnitIndex.nBase_Temper = UchtD.nUnitTemper;
				pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
			}

			ChartItemD.Initialize();
			if(!GenChart.GetData(UchtK, ChartItemD, strFilePath))
				continue;
		
			pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD); 

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UchtD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aCaption.Add(UchtD.strName);
			// (Unit Customizing) 백업해뒀던 현재 단위계 복원
			if(UchtD.bDefinedUnit == TRUE) pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUchtNum; nItem++)
		{
			UchtK = 0;
			UchtD.Initialize(0);
			ChartItemD.Initialize();
			strFilePath=_T("");
			
			UchtK = aUchtK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
				continue;
			
			// Pushover 결과 있는지 검사.
			if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
				( /*UchtD.nType==2 || */UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
				continue;

			// (Unit Customizing) Defined 단위계로 변경
			if(UchtD.bDefinedUnit == TRUE)
			{
				T_UNIT_INDEX UnitIndex;
				UnitIndex.nBase_Length = UchtD.nUnitLength;
				UnitIndex.nBase_Force  = UchtD.nUnitForce ;
				UnitIndex.nBase_Heat   = UchtD.nUnitHeat 	;
				UnitIndex.nBase_Temper = UchtD.nUnitTemper;
				pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
			}

			ChartItemD.Initialize();
			if(!GenChart.GetData(UchtK, ChartItemD, strFilePath))
				continue;
			
			pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD); 
			
			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UchtD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aCaption.Add(UchtD.strName);

			// (Unit Customizing) 백업해뒀던 현재 단위계 복원
			if(UchtD.bDefinedUnit == TRUE) pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();
	aCaption.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(CHT);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;

	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
}

// Inset Image Items (User Defined Image + Image File)
void CTreeReportCtrl::InsertReportItemImageAll() 
{
	InsertReportItemImage();
	InsertReportItemImageFile();
}

// Inset Image Items
void CTreeReportCtrl::InsertReportItemImage() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYIM_FLD && data != WORK_UFIG_LST && data != WORK_UFIG_FLD) return;

	CArray<T_UFIG_K, T_UFIG_K> aUfigK;  aUfigK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	int nUfigNum = aUfigK.GetSize();
	if(nUfigNum<=0)
	{
		ASSERT(0); return;
	}

	T_UFIG_K  UfigK=0;
	T_UFIG_D  UfigD;  UfigD.Initialize();

	CString strFilePath=_T("");

	CStringArray  aName    ; // T_UFIG_D::strName (Caption, 추출정보이름)
	CStringArray  aFilePath; // *.bmp Full Path
	aName.RemoveAll();
	aFilePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UfigK = 0;
			UfigD.Initialize();
			strFilePath=_T("");

			UfigK = aUfigK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
				continue;

			if(!pDoc->m_pPostCtrl->IsAnalysisResultEnable() && !UfigD.bPre && // 해석결과가 없고, 저장된 이미지가 후처리 이미지일때 
				 UfigD.PostFrsiD.nCurrentMode>0)                                // 후처리 결과 이미지는 비활성
			{
				continue;
			}

			// bmp image file name
			CString strFilePathName=_T("");
			CString strFullPath=_T("");
			//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
			::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
			strFullPath.ReleaseBuffer();
			CFileCtrl FileCtrl(strFullPath);
			strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
			strFilePath.Format(_T("%s%s.bmp"), strFilePathName, UfigD.strName);

			if(!CGPSAMRUtil::GetInstance()->GetMakeFigureAndPath(UfigK, strFilePath))
				continue;

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UfigD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUfigNum; nItem++)
		{
			UfigK = 0;
			UfigD.Initialize();
			strFilePath=_T("");
			
			UfigK = aUfigK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
				continue;
			
			if(!pDoc->m_pPostCtrl->IsAnalysisResultEnable() && !UfigD.bPre && // 해석결과가 없고, 저장된 이미지가 후처리 이미지일때 
				 UfigD.PostFrsiD.nCurrentMode>0)                                // 후처리 결과 이미지는 비활성
			{
				continue;
			}

			// bmp image file name
			CString strFilePathName=_T("");
			CString strFullPath=_T("");
			//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
			::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
			strFullPath.ReleaseBuffer();
			CFileCtrl FileCtrl(strFullPath);
			strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
			strFilePath.Format(_T("%s%s.bmp"), strFilePathName, UfigD.strName);
			
			if(!CGPSAMRUtil::GetInstance()->GetMakeFigureAndPath(UfigK, strFilePath))
				continue;
			
			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UfigD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aName[nItem];
		sType         = _LSX(IMG);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = /*_T("UD ")+*/aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
// 
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
// 
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
// 
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
// 
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
// 
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
// 
//     pIftag[nItem].iAppendFlag = iAppendFlag;
// 
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;

#if 1 // 잠시 막는다. Auto Report Test를 위해 
	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
#endif
}

// Inset Image File Items
void CTreeReportCtrl::InsertReportItemImageFile() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYIM_FLD && data != WORK_UIMG_FLD && data != WORK_UIMG_LST) return;

	CArray<T_UIMG_K, T_UIMG_K> aUimgK;  aUimgK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
	int nUimgNum = aUimgK.GetSize();
	if(nUimgNum<=0)
	{
		ASSERT(0); return;
	}

	T_UIMG_K  UimgK=0;
	T_UIMG_D  UimgD;  UimgD.Initialize();

	CString strFilePath=_T("");

	CStringArray  aName    ; // T_UIMG_D::strName (Caption, 추출정보이름)
	CStringArray  aFilePath; // *.bmp Full Path
	CStringArray  aCaption ;  // Caption (image 파일명)
	aName.RemoveAll();
	aFilePath.RemoveAll();
	aCaption.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();
	
	CFileCtrl FileCtrl2(strModelPath);
	CString strFolder=_T("");
	strFolder = FileCtrl2.GetFilePath();

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UimgK = 0;
			UimgD.Initialize();
			strFilePath=_T("");

			UimgK = aUimgK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUimg(UimgK, UimgD))
				continue;

			// bmp image file name
			//strFilePath = UimgD.strFilePath;
			strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
			ASSERT(strFilePath!=_T(""));
			CFileCtrl FileCtrl(strFilePath);
			if(!FileCtrl.FileExists(strFilePath))
			{
				ASSERT(0); continue;
			}

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UimgD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aCaption.Add(UimgD.strFilePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUimgNum; nItem++)
		{
			UimgK = 0;
			UimgD.Initialize();
			strFilePath=_T("");
			
			UimgK = aUimgK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUimg(UimgK, UimgD))
				continue;
			
			// bmp image file name
			//strFilePath = UimgD.strFilePath;
			strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
			ASSERT(strFilePath!=_T(""));
			CFileCtrl FileCtrl(strFilePath);
			if(!FileCtrl.FileExists(strFilePath))
			{
				ASSERT(0); continue;
			}

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UimgD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aCaption.Add(UimgD.strFilePath);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();
	aCaption.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(IFL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = /*_T("IF ")+*/aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T("IF");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;
}

// Inset Table Items (User Defined Table + Special Table + Table Template)
void CTreeReportCtrl::InsertReportItemTableAll() 
{
	InsertReportItemTable();
	InsertReportItemSpecialTable();
	InsertReportItemTableTemplate();
}

// Inset Table Items
void CTreeReportCtrl::InsertReportItemTable() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTB_FLD && data != WORK_UTBL_LST && data != WORK_UTBL_FLD) return;

	CArray<T_UTBL_K, T_UTBL_K> aUtblK;  aUtblK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	int nUtblNum = aUtblK.GetSize();
	if(nUtblNum<=0)
	{
		ASSERT(0); return;
	}

	// (Unit Customizing) 현재단위 백업
	T_UNIT_INDEX UnitIndex_cur;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);

	T_UTBL_K  UtblK=0;
	T_UTBL_D  UtblD;  UtblD.Initialize();

	CString strFilePath=_T("");

	CStringArray  aName    ; // T_UTBL_D::strName (Caption, 추출정보이름)
	CStringArray  aFilePath; // *.bmp Full Path
	aName.RemoveAll();
	aFilePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;
	CArray<BOOL, BOOL> aRepeat; aRepeat.RemoveAll();
	BOOL bRepeat = TRUE;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UtblK = 0;
			UtblD.Initialize();
			strFilePath=_T("");

			UtblK = aUtblK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
				continue;

			// 후처리 테이블은 후처리 모드에서만 그리기
			if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
				continue;

			if(UtblD.nTBMode==D_UTBL_TYPE_BCG_POST && !pDoc->IsPostMode())
				continue;

			if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
				continue;

			if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
			{ 				
				CStringArray aPscCodeName;
				CDBLib::GetPscCodeNameList(aPscCodeName);
				if(aPscCodeName.GetSize()<=0)      
					continue;
			}

			// (Unit Customizing) Defined 단위계로 변경
			if(UtblD.bDefinedUnit == TRUE)
			{
				T_UNIT_INDEX UnitIndex;
				UnitIndex.nBase_Length = UtblD.nUnitLength;
				UnitIndex.nBase_Force  = UtblD.nUnitForce ;
				UnitIndex.nBase_Heat   = UtblD.nUnitHeat 	;
				UnitIndex.nBase_Temper = UtblD.nUnitTemper;
				pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
			}

			// xml file name
			CString strFilePathName=_T("");
			CString strFullPath=_T("");
			//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
			::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
			strFullPath.ReleaseBuffer();
			CFileCtrl FileCtrl(strFullPath);
			strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
			strFilePath.Format(_T("%s%s.xml"), strFilePathName, UtblD.strName);

			bRepeat = TRUE;
			if(!pDoc->MakeReportTableXml(UtblK, strFilePath, bRepeat))
				continue;

			aRepeat.Add(bRepeat);

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UtblD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			// (Unit Customizing) 백업해뒀던 현재 단위계 복원
			if(UtblD.bDefinedUnit == TRUE) pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUtblNum; nItem++)
		{
			UtblK = 0;
			UtblD.Initialize();
			strFilePath=_T("");
			
			UtblK = aUtblK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
				continue;
			
			// 후처리 테이블은 후처리 모드에서만 그리기
			if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
				continue;
			
			if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
				continue;
			
			if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
			{ 				
//         if(!pDoc->IsPostMode())
//           continue;
				CStringArray aPscCodeName;
				CDBLib::GetPscCodeNameList(aPscCodeName);
				if(aPscCodeName.GetSize()<=0)      
					continue;
			}

			// (Unit Customizing) Defined 단위계로 변경
			if(UtblD.bDefinedUnit == TRUE)
			{
				T_UNIT_INDEX UnitIndex;
				UnitIndex.nBase_Length = UtblD.nUnitLength;
				UnitIndex.nBase_Force  = UtblD.nUnitForce ;
				UnitIndex.nBase_Heat   = UtblD.nUnitHeat 	;
				UnitIndex.nBase_Temper = UtblD.nUnitTemper;
				pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
			}

			// xml file name
			CString strFilePathName=_T("");
			CString strFullPath=_T("");
			//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
			::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
			strFullPath.ReleaseBuffer();
			CFileCtrl FileCtrl(strFullPath);
			strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
			strFilePath.Format(_T("%s%s.xml"), strFilePathName, UtblD.strName);

			bRepeat = TRUE;
			if(!pDoc->MakeReportTableXml(UtblK, strFilePath, bRepeat))
				continue;

			aRepeat.Add(bRepeat);

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UtblD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);

			// (Unit Customizing) 백업해뒀던 현재 단위계 복원
			if(UtblD.bDefinedUnit == TRUE) pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aName[nItem];
		sType         = _LSX(TBL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = /*_T("UD ")+*/aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = aRepeat[nItem]? _T(" ") : _T("0");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;

	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
}

// Inset Table Template Items
void CTreeReportCtrl::InsertReportItemTableTemplate() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTB_FLD && data != WORK_UTPL_LST && data != WORK_UTPL_FLD) return;

	CArray<T_UTPL_K, T_UTPL_K> aUtplK;  aUtplK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUtplKeyList(aUtplK);
	int nUtplNum = aUtplK.GetSize();
	if(nUtplNum<=0)
	{
		ASSERT(0); return;
	}

	T_UTPL_K  UtplK=0;
	T_UTPL_D  UtplD;  UtplD.Initialize();

	CString strFilePath=_T("");

	CStringArray  aName    ; // T_UTPL_D::strName (Caption, 추출정보이름)
	CStringArray  aFilePath; // *.xml Full Path
	aName.RemoveAll();
	aFilePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();
	
	CFileCtrl FileCtrl2(strModelPath);
	CString strFolder=_T("");
	strFolder = FileCtrl2.GetFilePath();

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UtplK = 0;
			UtplD.Initialize();
			strFilePath=_T("");

			UtplK = aUtplK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUtpl(UtplK, UtplD))
				continue;

			// xml file name
			//strFilePath = UtplD.strFilePath;
			strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Table_Template_Files__s), strFolder, UtplD.strFilePath);
			ASSERT(strFilePath!=_T(""));
			CFileCtrl FileCtrl(strFilePath);
			if(!FileCtrl.FileExists(strFilePath))
			{
				ASSERT(0); continue;
			}

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UtplD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUtplNum; nItem++)
		{
			UtplK = 0;
			UtplD.Initialize();
			strFilePath=_T("");
			
			UtplK = aUtplK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUtpl(UtplK, UtplD))
				continue;
			
			// xml file name
			//strFilePath = UtplD.strFilePath;
			strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Table_Template_Files__s), strFolder, UtplD.strFilePath);
			ASSERT(strFilePath!=_T(""));
			CFileCtrl FileCtrl(strFilePath);
			if(!FileCtrl.FileExists(strFilePath))
			{
				ASSERT(0); continue;
			}

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UtplD.strName);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

// #if defined(_CIVIL)
//   sProductInfo  = _LSX(CIVIL);
// #elif(_MGEN)
//   sProductInfo  = _LSX(GEN);
// #else
//   ASSERT(0);
//   sProductInfo  = _T(" ");
// #endif

	sProductInfo  = _LSX(WORD); // Table Template은 제품이나 모델에 구애받지 않으므로, Word로

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aName[nItem];
		sType         = _LSX(TPL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = _LSX(TP )+aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;
}

// Inset Table Items
void CTreeReportCtrl::InsertReportItemSpecialTable() 
{
	InsertReportItemSSTable();
	InsertReportItemRSTable();
	InsertReportItemCSTable();
}

// Inset Table Items (Special Tables - Section Summary)
void CTreeReportCtrl::InsertReportItemSSTable() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTB_FLD && data != WORK_USTB_FLD && data != WORK_USSS_FLD && data != WORK_USSS_LST) return;

	CArray<T_SECT_K, T_SECT_K> aSectK;
	aSectK.RemoveAll();
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	int nSectNum = aSectK.GetSize();
	if(nSectNum<=0) 
	{ 
		ASSERT(0); return; 
	}

	// (Unit Customizing) 현재단위 백업, 사용자 정의 단위계로 변경
	T_UNIT_INDEX UnitIndex_cur;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);
	T_UNSP_D UnspD;
	if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
	{
		if(UnspD.SectSumm.nUnitSys==1)
		{
			pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.SectSumm.UnitIdx);
		}
	}

	T_SECT_K  SectK=0;
	T_SECT_D  SectD;  SectD.Initialize();

	CString strFilePath=_T("");
	CString strImagePath=_T("");

	CStringArray  aName    ; // strName (추출정보이름)
	CStringArray  aCaption ; // Caption
	CStringArray  aFilePath; // *.xml Full Path
	CStringArray  aImagePath; // *.bmp Image Path
	aName.RemoveAll();
	aCaption.RemoveAll();
	aFilePath.RemoveAll();
	aImagePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;

	int nType=0;  // section type
	CStringArray arImagePath;

	CDgnAmrSectionTb SectionTb;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);

			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			StiffInfo.Initialize();
			AfterStiffInfo.Initialize();
			nType = 0;
			arImagePath.RemoveAll();

			SectK = aSectK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;

			if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
				continue;

			nType = StiffInfo.nType;
			if(nType<1 || nType>7)
			{
				ASSERT(0); continue;
			}

			// set file path name
			CString strFilePathName=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

			// make section image
			MakeSectionFigure(strImagePath, SectK, nType, arImagePath);

			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, SectK, 0))
				continue;

			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
			strTemp.Format(_T("%d"), SectK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nSectNum; nItem++)
		{
			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			StiffInfo.Initialize();
			AfterStiffInfo.Initialize();
			nType = 0;
			arImagePath.RemoveAll();
			
			SectK = aSectK[nItem];
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;

			if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
				continue;
			
			nType = StiffInfo.nType;
			if(nType<1 || nType>7)
			{
				ASSERT(0); continue;
			}
			
			// set file path name
			CString strFilePathName=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);
			
			// make section image
			MakeSectionFigure(strImagePath, SectK, nType, arImagePath);
			
			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, SectK, 0))
				continue;
			
			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
			strTemp.Format(_T("%d"), SectK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}

	aName.FreeExtra();
	aCaption.FreeExtra();
	aFilePath.FreeExtra();
	aImagePath.FreeExtra();

	// (Unit Customizing) 백업해뒀던 현재 단위계 복원
	pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TBL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = _ULS('SS ')+aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

	delete[] pIftag;

	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);

		sFullPath = aImagePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
}

// Inset Table Items (Special Tables - Reinforcement of Sections)
void CTreeReportCtrl::InsertReportItemRSTable() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTB_FLD && data != WORK_USTB_FLD && data != WORK_USRS_FLD && data != WORK_USRS_LST) return;

	CArray<T_SECT_K, T_SECT_K> aUsedSectKeyList;
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aUsedSectKeyList);

	CArray<T_RPSC_K, T_RPSC_K> aRpscKeyList;
	CArray<T_RPSC_K, T_RPSC_K> aRpscKeyListAll;
	pDoc->m_pAttrCtrl->GetRpscKeyList(aRpscKeyListAll); // Reinforcement for PSC Section
	CArrayUtil::IntersectUINT(aUsedSectKeyList, aRpscKeyListAll, aRpscKeyList);
	int nRpscCnt = aRpscKeyList.GetSize();
	if(nRpscCnt<=0)
	{
		ASSERT(0); return;
	}

	// (Unit Customizing) 현재단위 백업, 사용자 정의 단위계로 변경
	T_UNIT_INDEX UnitIndex_cur;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);
	T_UNSP_D UnspD;
	if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
	{
		if(UnspD.ReinForc.nUnitSys==1)
		{
			pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.ReinForc.UnitIdx);
		}
	}

	T_SECT_K  SectK=0;
	T_SECT_D  SectD;  SectD.Initialize();

	CString strFilePath=_T("");
	CString strImagePath=_T("");

	CStringArray  aName    ; // strName (추출정보이름)
	CStringArray  aCaption ; // Caption
	CStringArray  aFilePath; // *.xml Full Path
	CStringArray  aImagePath; // *.bmp Image Path
	aName.RemoveAll();
	aCaption.RemoveAll();
	aFilePath.RemoveAll();
	aImagePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	BOOL bConsiderJ = FALSE;
	T_RSCT_RBAR_D  RpscInfoI, RpscInfoJ;

	CStringArray arImagePath;

	CDgnDrawSection DgnDrawSection;
	_DGN_DRAW_SECT_OPTION OptionD;
	OptionD.Initialize();
	OptionD.Canvas.SetRect(0,0,7500,7500);
	OptionD.DimSize = 500;

	CDgnAmrSectionTb SectionTb;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);

			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			bConsiderJ = FALSE;
			RpscInfoI.Initialize();
			RpscInfoJ.Initialize();
			arImagePath.RemoveAll();

			SectK = aRpscKeyList[pos-1];
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;

			if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
				continue;

			// set file path name
			CString strFilePathName=_T(""), strImagePath2=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strImagePath2.Format(_T("%s%s_%d_2.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

			// make section image
			if(bConsiderJ==FALSE)
			{
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath);
			}
			else if(bConsiderJ==TRUE)
			{
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath);
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath2, SectK, 2, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath2);
			}
			else
			{
				ASSERT(0); continue;
			}

			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_RS, SectK, 0))
				continue;


			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
			strTemp.Format(_T("%d"), SectK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nRpscCnt; nItem++)
		{
			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			bConsiderJ = FALSE;
			RpscInfoI.Initialize();
			RpscInfoJ.Initialize();
			arImagePath.RemoveAll();
			
			SectK = aRpscKeyList[nItem];
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;

			if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
				continue;
			
			// set file path name
			CString strFilePathName=_T(""), strImagePath2=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strImagePath2.Format(_T("%s%s_%d_2.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

			// make section image
			if(bConsiderJ==FALSE)
			{
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath);
			}
			else if(bConsiderJ==TRUE)
			{
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath);
				DgnDrawSection.Make_SectionFig_For_SectK(strImagePath2, SectK, 2, OptionD, FALSE, 1, FALSE);
				arImagePath.Add(strImagePath2);
			}
			else
			{
				ASSERT(0); continue;
			}
			
			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_RS, SectK, 0))
				continue;
			
			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
			strTemp.Format(_T("%d"), SectK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();

	// (Unit Customizing) 백업해뒀던 현재 단위계 복원
	pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TBL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = _ULS('RS ')+aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

	delete[] pIftag;

	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);

		sFullPath = aImagePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
}

// Inset Table Items (Special Tables - Composite Section for C.S.)
void CTreeReportCtrl::InsertReportItemCSTable() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTB_FLD && data != WORK_USTB_FLD && data != WORK_USCS_FLD && data != WORK_USCS_LST) return;

	CArray<T_SECT_K, T_SECT_K> aUsedSectKeyList;
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aUsedSectKeyList);
	int nUsedSectKeyListCnt = aUsedSectKeyList.GetSize();
	int nSize=0;
	if(nUsedSectKeyListCnt<=0)
		nSize = 1;
	else
		nSize = nUsedSectKeyListCnt;
	
	CMapEx<UINT,UINT,UINT,UINT> mUsedSectKeyList;
	mUsedSectKeyList.InitHashTable(nSize);
	UINT nKey=0;
	for(int m=0; m<nUsedSectKeyListCnt; m++)
	{
		nKey = aUsedSectKeyList[m];
		mUsedSectKeyList.SetAt(nKey, 1);
	}

	CArray<T_CSCS_K, T_CSCS_K> aCscsKeyList;
	CArray<T_CSCS_K, T_CSCS_K> aCscsKeyListAll;
	pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsKeyListAll); // Composite Section for C.S.
	SortCscsKey(pDoc, aCscsKeyListAll);
	int nCscsCntAll = aCscsKeyListAll.GetSize();
	
	T_SECT_K SectK=0;
	T_CSCS_K CscsK=0;
	T_CSCS_D CscsD;
	UINT nVal=0;
	aCscsKeyList.RemoveAll();
	for(int i=0; i<nCscsCntAll; i++)
	{
		CscsK = aCscsKeyListAll[i];
		CscsD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
			continue;
		
		SectK = CscsD.SectKey;
		if(!mUsedSectKeyList.Lookup(SectK, nVal)) 
			continue;
		
		aCscsKeyList.Add(CscsK);
	}
	int nCscsCnt = aCscsKeyList.GetSize();
	if(nCscsCnt<=0)
	{
		ASSERT(0); return;
	}

	// (Unit Customizing) 현재단위 백업, 사용자 정의 단위계로 변경
	T_UNIT_INDEX UnitIndex_cur;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);
	T_UNSP_D UnspD;
	if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
	{
		if(UnspD.CompSect.nUnitSys==1)
		{
			pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.CompSect.UnitIdx);
		}
	}
	T_SECT_D  SectD;  SectD.Initialize();

	CString strFilePath=_T("");
	CString strImagePath=_T("");

	CStringArray  aName    ; // strName (추출정보이름)
	CStringArray  aCaption ; // Caption
	CStringArray  aFilePath; // *.xml Full Path
	CStringArray  aImagePath; // *.bmp Image Path
	aName.RemoveAll();
	aCaption.RemoveAll();
	aFilePath.RemoveAll();
	aImagePath.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	T_STAG_K StagK=0;
	T_STAG_D StagD; StagD.Initialize();
	CscsK=0;
	CscsD.Initialize();

	CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> aCscsInfo;
	aCscsInfo.RemoveAll();

	int nType=0;  // section type
	CStringArray arImagePath; arImagePath.RemoveAll();

	CDgnDrawSection DgnDrawSection;
	_DGN_DRAW_SECT_OPTION OptionD;
	OptionD.Initialize();
	OptionD.Canvas.SetRect(0,0,7500,7500);
	OptionD.DimSize = 500;

	CDgnAmrSectionTb SectionTb;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);

			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			aCscsInfo.RemoveAll();
			nType = 0;
			arImagePath.RemoveAll();

			CscsK = aCscsKeyList[pos-1];
			CscsD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
				continue;

			SectK = CscsD.SectKey;
			StagK = CscsD.ActStagKey;

			SectD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;

			StagD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
					continue;

			nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
			if(nType!=1 && nType!=2) continue;

			int nCscsNum = aCscsInfo.GetSize();

			// set file path name
			CString strFilePathName=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

			if(nType==1)
			{
				for(int i=0; i<nCscsNum; i++)
				{
					// [2012-04-06] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
					// 4429 : 합성단면의 경우 Part1과 Part2를 나눠서 그려야 하나           
					// 아직 이부분에 대한 DgnDraw의 함수가 없는 관계로 일단 합성단면 전체를 그리게끔 임시로 변경.
					// 기존에는 Part1만 그리고 있었음.
					strImagePath.Format(_T("%s_%s_%d.emf"), strFilePathName, strSName, i);
					DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, TRUE);
					arImagePath.Add(strImagePath);
				}
			}
			else if(nType==2)
			{
				arImagePath.RemoveAll();
			}
			else
			{
				ASSERT(0);
				continue;
			}

			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_CS, SectK, StagK))
				continue;

			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			strTemp.Format(_T("%d"), CscsK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nCscsCnt; nItem++)
		{
			SectK = 0;
			SectD.Initialize();
			strImagePath=_T("");
			strFilePath=_T("");
			aCscsInfo.RemoveAll();
			nType = 0;
			arImagePath.RemoveAll();
			
			CscsK = aCscsKeyList[nItem];
			CscsD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
				continue;
			
			SectK = CscsD.SectKey;
			StagK = CscsD.ActStagKey;
			
			SectD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
				continue;
			
			StagD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
				continue;

			nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
			int nCscsNum = aCscsInfo.GetSize();
			
			// set file path name
			CString strFilePathName=_T("");
			strFilePathName = GetModelFullPathName();
			CString strSName=_T("");
			strSName = SectD.SName;
			FilterFileName(strSName);
			strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
			strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

			if(nType==1)
			{
				for(int i=0; i<nCscsNum; i++)
				{
					strImagePath.Format(_T("%s_%s_%d.emf"), strFilePathName, strSName, i);
					DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
					arImagePath.Add(strImagePath);
				}
			}
			else if(nType==2)
			{
				arImagePath.RemoveAll();
			}
			else
			{
				ASSERT(0);
				continue;
			}
			
			if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_CS, SectK, StagK))
				continue;
			
			CString strTemp=_T("");
			strTemp.Format(_T("%d : %s"), SectK, SectD.SName);
			aCaption.Add(strTemp);
			strTemp.Format(_T("%d"), CscsK);
			aName.Add(strTemp);
			aFilePath.Add(strFilePath);
			aImagePath.Add(strImagePath);
		}
	}

	aName.FreeExtra();
	aFilePath.FreeExtra();

	// (Unit Customizing) 백업해뒀던 현재 단위계 복원
	pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	if(nName<=0) return;

	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TBL);
		sFullPath     = aFilePath[nItem];
		sExtInfo      = _ULS('CS ')+aName[nItem];
		sContents     = _T(" ");
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

	delete[] pIftag;

	// 임시파일 삭제
	CFileFind file;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sFullPath=_T("");
		sFullPath = aFilePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);

		sFullPath = aImagePath[nItem];
		if(file.FindFile(sFullPath))
			::DeleteFile(sFullPath);
	}
	file.Close();
}

// Section Summary의 이미지 파일을 만든다.
void CTreeReportCtrl::MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath)
{
	// MQC 8651 : 이미지 경로에 2글자 이상의 공백이나 '&'가 있는 경우 그림 생성 안 되는 문제
	strImagePath.Replace(_T(" "),_T(""));
	strImagePath.Replace(_T("&"),_T(""));

	arImagePath.RemoveAll();

	CDgnDrawSection DgnDrawSection;
	_DGN_DRAW_SECT_OPTION OptionD;
	OptionD.Initialize();
	OptionD.Canvas.SetRect(0,0,7500,7500);
	OptionD.DimSize = 500;

	if(nType==1 || nType==3 || nType==4)
	{
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
	}
	else if(nType==2)
	{
		strImagePath=_T("");
		arImagePath.RemoveAll();
	}
	else if(nType==5)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==6)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==7)
	{
		CString strTmp=_T(""), strTmp2=_T(""), strTmp3=_T(""), strTmp4=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);
		strTmp3.Format(_T("%s_3.emf"), strTmp);
		strTmp4.Format(_T("%s_4.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp3, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp3);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp4, SectK, 2, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp4);
	}
	else
	{
		ASSERT(0);
	}
}

CString CTreeReportCtrl::GetModelFullPathName()
{
	CString strFilePathName=_T("");
	CString strFullPath=_T("");

	//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
	::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
	strFullPath.ReleaseBuffer();
	CFileCtrl FileCtrl(strFullPath);
	strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
	strFilePathName.Replace(_T("\\"), _T("\\\\"));

	return strFilePathName;
}

// Inset Text Items (Defined Text + User Defined Text)
void CTreeReportCtrl::InsertReportItemTextAll() 
{
	InsertReportItemText();
	InsertReportItemSummary();
}

// Inset Text Items
void CTreeReportCtrl::InsertReportItemText() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTT_FLD && data != WORK_UTXT_LST && data != WORK_UTXT_FLD) return;

	CStringArray  aCaption ; // Caption
	CStringArray  aName    ; // 추출정보이름
	CStringArray  aContents; // 내용
	aCaption .RemoveAll();
	aName    .RemoveAll();
	aContents.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	CString strCaption=_T("");
	CString strText=_T("");
	CString strContents=_T("");
	CReportTreeText ReportTreeText;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			strText = m_aReportTextStrName[pos-1];
			if(!ReportTreeText.WriteReportText(strText, strContents))
				continue;

			strCaption = m_aReportTextCaption[pos-1];

			aCaption.Add(strCaption);
			aName.Add(strText);
			aContents.Add(strContents);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		int nCount=0;
#if defined(_MGEN)
		nCount = D_REPORT_TEXT_CNT;
#elif(_CIVIL)
		nCount = D_REPORT_TEXT_CNT-6;
#else
		ASSERT(0);
#endif

		for(int nItem=0; nItem<nCount; nItem++)
		{
			strText = m_aReportTextStrName[nItem];
			if(!ReportTreeText.WriteReportText(strText, strContents))
				continue;
			
			strCaption = m_aReportTextCaption[nItem];
			
			aCaption.Add(strCaption);
			aName.Add(strText);
			aContents.Add(strContents);
		}
	}

	aName.FreeExtra();
	aContents.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nContents = aContents.GetSize();
	ASSERT(nName==nContents);
	if(nName<=0) return;
	
	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TXT);
		sFullPath     = _T(" ");
		sExtInfo      = aName[nItem];
		sContents     = aContents[nItem];
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;
}

// Inset Text Items (Summary)
void CTreeReportCtrl::InsertReportItemSummary() 
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != WORK_DYTT_FLD && data != WORK_USUM_LST && data != WORK_USUM_FLD) return;

	CArray<T_USUM_K, T_USUM_K> aUsumK;  aUsumK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUsumKeyList(aUsumK);
	int nUsumNum = aUsumK.GetSize();
	if(nUsumNum<=0)
	{
		ASSERT(0); return;
	}
	
	T_USUM_K  UsumK=0;
	T_USUM_D  UsumD;  UsumD.Initialize();

	CStringArray  aCaption ; // Caption
	CStringArray  aName    ; // 추출정보이름
	CStringArray  aContents; // 내용
	aCaption .RemoveAll();
	aName    .RemoveAll();
	aContents.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	CString strCaption=_T("");
	CString strText=_T("");
	CString strContents=_T("");
	CReportTreeText ReportTreeText;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	if(m_nDataType==D_DATATYPE_LST) 
	{
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];

			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
		
			UsumK = 0;
			UsumD.Initialize();

			UsumK = aUsumK[pos-1];
			if(!pDoc->m_pAttrCtrl->GetUsum(UsumK, UsumD))
				continue;

			// 후처리 모드에서만
			if(!pDoc->IsPostMode())
				continue;

			if(!pDoc->MakeReportText(UsumK, strContents))
				continue;

			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UsumD.strName);
			aCaption.Add(strTemp);
			aName.Add(strTemp);
			aContents.Add(strContents);
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		for(int nItem=0; nItem<nUsumNum; nItem++)
		{
			UsumK = 0;
			UsumD.Initialize();
			
			UsumK = aUsumK[nItem];
			if(!pDoc->m_pAttrCtrl->GetUsum(UsumK, UsumD))
				continue;
			
			// 후처리 모드에서만
			if(!pDoc->IsPostMode())
				continue;
			
			if(!pDoc->MakeReportText(UsumK, strContents))
				continue;
			
			CString strTemp=_T("");
			strTemp = ReplaceSymbolByTempChar(UsumD.strName);
			aCaption.Add(strTemp);
			aName.Add(strTemp);
			aContents.Add(strContents);
		}
	}

	aName.FreeExtra();
	aContents.FreeExtra();

	// Make structure to export
	int nName = aName.GetSize();
	int nContents = aContents.GetSize();
	ASSERT(nName==nContents);
	if(nName<=0) return;
	
	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = _LSX(TXT);
		sFullPath     = _T(" ");
		sExtInfo      = aName[nItem];
		sContents     = aContents[nItem];
		iAppendFlag   = D_WORD_APPEND_INSERT; 
		sExt          = _T(" ");
		
		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;

//     pIftag[nItem].sProductInfo = (char*)malloc(sProductInfo.GetLength() + 1);
//     memset(pIftag[nItem].sProductInfo, 0x00, sProductInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sProductInfo, sProductInfo.GetBuffer(1));
//     
//     pIftag[nItem].sCaption = (char*)malloc(sCaption.GetLength() + 1);
//     memset(pIftag[nItem].sCaption, 0x00, sCaption.GetLength() + 1);
//     strcpy(pIftag[nItem].sCaption, sCaption.GetBuffer(1));
//     
//     pIftag[nItem].sType = (char*)malloc(sType.GetLength() + 1);
//     memset(pIftag[nItem].sType, 0x00, sType.GetLength() + 1);
//     strcpy(pIftag[nItem].sType, sType.GetBuffer(1));
//     
//     pIftag[nItem].sFullPath = (char*)malloc(sFullPath.GetLength() + 1);
//     memset(pIftag[nItem].sFullPath, 0x00, sFullPath.GetLength() + 1);
//     strcpy(pIftag[nItem].sFullPath, sFullPath.GetBuffer(1));
//     
//     pIftag[nItem].sExtInfo = (char*)malloc(sExtInfo.GetLength() + 1);
//     memset(pIftag[nItem].sExtInfo, 0x00, sExtInfo.GetLength() + 1);
//     strcpy(pIftag[nItem].sExtInfo, sExtInfo.GetBuffer(1));
//     
//     pIftag[nItem].sContents = (char*)malloc(sContents.GetLength() + 1);
//     memset(pIftag[nItem].sContents, 0x00, sContents.GetLength() + 1);
//     strcpy(pIftag[nItem].sContents, sContents.GetBuffer(1));
//     
//     pIftag[nItem].iAppendFlag = iAppendFlag;
//     
//     pIftag[nItem].sExt = (char*)malloc(sExt.GetLength() + 1);
//     memset(pIftag[nItem].sExt, 0x00, sExt.GetLength() + 1);
//     strcpy(pIftag[nItem].sExt, sExt.GetBuffer(1));
	}

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod != NULL) 
	{
		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pIftag);
		pExport=NULL;
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}
	hMod = NULL;

//   for(int nItem=0; nItem<nItemCnt; nItem++)
//   {
//     free((void*)pIftag[nItem].sProductInfo);
//     free((void*)pIftag[nItem].sCaption);
//     free((void*)pIftag[nItem].sType);
//     free((void*)pIftag[nItem].sFullPath);
//     free((void*)pIftag[nItem].sExtInfo);
//     free((void*)pIftag[nItem].sContents);
//     free((void*)pIftag[nItem].sExt);
//   }
	delete[] pIftag;
}

void CTreeReportCtrl::OnWorktreeSelectplus() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	if(FALSE == ItemHasChildren(aItem[0]))
	{
		UINT Data = GetItemData(aItem[0]);
		SelectEntity(Data,FALSE);
	}
}

void CTreeReportCtrl::OnWorktreeDisplay() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;

	UINT Data = GetItemData(aItem[0]);
	if(Data == WORK_UFIG_LST) // Dynamic Report Figure
	{
		SetDynReportFigureToView(aItem[0]);  
	}
}

void CTreeReportCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;
}

void CTreeReportCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	BOOL  bMulti=FALSE;

	short nKeyState = GetKeyState(VK_LCONTROL);
	if (nKeyState & 0x8000) bMulti = TRUE;
	if(bMulti==FALSE) { nKeyState = GetKeyState(VK_LSHIFT); if (nKeyState & 0x8000) bMulti = TRUE; }

	if(selItemNum && bMulti)
	{
		HTREEITEM preParItem = GetParentItem(aItem[0]);
		UINT preParData = GetItemData(preParItem);
		HTREEITEM newParItem = GetParentItem(pNMTreeView->itemNew.hItem);
		UINT newParData = GetItemData(newParItem);
		if(preParData != newParData) { *pResult = TRUE; return; }
	}
// need to debug
	
	*pResult = 0;
}

int CTreeReportCtrl::GetSelectedUfigNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_UFIG_D UfigD;
	CArray<T_UFIG_K, T_UFIG_K> aUfigK;
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UfigD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUfig(aUfigK[pos], UfigD))
				{ ASSERT(0); continue; }
				aName.Add(UfigD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}

int CTreeReportCtrl::GetSelectedUtblNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_UTBL_D UtblD;
	CArray<T_UTBL_K, T_UTBL_K> aUtblK;
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UtblD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUtbl(aUtblK[pos], UtblD))
				{ ASSERT(0); continue; }
				aName.Add(UtblD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}

int CTreeReportCtrl::GetSelectedUchtNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_UCHT_D UchtD;
	CArray<T_UCHT_K, T_UCHT_K> aUchtK;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UchtD.Initialize(0);
				if(!pDoc->m_pAttrCtrl->GetUcht(aUchtK[pos], UchtD))
				{ ASSERT(0); continue; }
				aName.Add(UchtD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}

int CTreeReportCtrl::GetSelectedUimgNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_UIMG_D UimgD;
	CArray<T_UIMG_K, T_UIMG_K> aUimgK;
	pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UimgD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUimg(aUimgK[pos], UimgD))
				{ ASSERT(0); continue; }
				aName.Add(UimgD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}

int CTreeReportCtrl::GetSelectedUtplNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_UTPL_D UtplD;
	CArray<T_UTPL_K, T_UTPL_K> aUtplK;
	pDoc->m_pAttrCtrl->GetUtplKeyList(aUtplK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UtplD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUtpl(aUtplK[pos], UtplD))
				{ ASSERT(0); continue; }
				aName.Add(UtplD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}

int CTreeReportCtrl::GetSelectedUsumNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;
	
	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];
	
	T_USUM_D UsumD;
	CArray<T_USUM_K, T_USUM_K> aUsumK;
	pDoc->m_pAttrCtrl->GetUsumKeyList(aUsumK);
	
	for(prvHt = ht;;) { prvHt = GetPrevSiblingItemWithoutFilter(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				UsumD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUsum(aUsumK[pos], UsumD))
				{ ASSERT(0); continue; }
				aName.Add(UsumD.strName); 
				nSt+=1; 
			}
		}
		nxtHt = GetNextSiblingItemWithoutFilter(ht);
	}
	return aName.GetSize();
}
//////////////////////////////////////////////////////////////////////

BOOL CTreeReportCtrl::GetCustomColor(HTREEITEM hItem)
{
	MBaseTreeCtrl::GetCustomColor(hItem);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == 0) return FALSE;
	if (pDoc->m_pMultiDB->IsTempDBMode()) return FALSE;

	UINT Data = GetItemData(hItem);
	int  bUsed=1;

	if(WORK_UTXT_LST == Data) bUsed = IsUsedText(hItem);
	if(WORK_UIMG_LST == Data) bUsed = IsExistFile(hItem);
	if(WORK_UTPL_LST == Data) bUsed = IsExistFile(hItem);
	if(WORK_UTBL_LST == Data) bUsed = IsPostTable(hItem);
	if(WORK_UCHT_LST == Data) bUsed = IsEnableChart(hItem);
	if(WORK_UFIG_LST == Data) bUsed = IsEnableImage(hItem);
	if(WORK_USUM_LST == Data)
	{
		if(pDoc->IsPostMode())  bUsed = 1;
		else bUsed = 2;
	}

	if (bUsed == 1)  m_ColorData.m_clrText = RGB(0,0,0);
	else if (bUsed == 0)  m_ColorData.m_clrText = (RGB(0,0,255));
	else if (bUsed ==-1)  m_ColorData.m_clrText = (RGB(255,0,0));
	else if (bUsed == 2)  m_ColorData.m_clrText = (RGB(128,128,128));
	else ASSERT(0);

	// HTREEITEM --> !^^! 
	return TRUE;
}

void CTreeReportCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_DELETE)
	{
		OnWorktreeDelete();
	}
	
	MBaseTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTreeReportCtrl::OnWorktreeDisappear() 
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	for(int i=0; i<selItemNum; i++)
	{
		HTREEITEM parentItem = GetParentItem(aItem[i]);
		UINT parentData = GetItemData(parentItem);

//     if (parentData == WORK_IEHG_FLD)
//       DisappearBoundaries(aItem[i]);
	}
}

HTREEITEM CTreeReportCtrl::GetPreviousItemToInsertAfter(UINT insItem)
{
	if(ItemHasChildren(m_hRootTi)==FALSE) return NULL; // Work Tree에 기존의 트리가 존재하지 않으면 

	HTREEITEM   hNextItem,hChildItem,hKeepItem;
	int  max=0;

	hChildItem = GetChildItem(m_hRootTi);
	while (hChildItem != NULL)
	{
		hNextItem = GetNextSiblingItemWithoutFilter(hChildItem);
		UINT Data = GetItemData(hChildItem);
		if(Data<insItem) 
		{
			if(Data>max) { max = Data; hKeepItem = hChildItem; }
		}
		hChildItem = hNextItem;
	}

	if(max>0) return hKeepItem;
	else return NULL;
}

HTREEITEM CTreeReportCtrl::SetSub1TreeItem(UINT insItem,BOOL bInit,int nCount,UINT imageID)
{
	HTREEITEM hTI=NULL;
	CString   tStr;
	CString   strSVGPath;
	HTREEITEM   hNextItem,hChildItem;

	switch(insItem)
	{
		case WORK_DYST_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Setup)); strSVGPath = _T("ico24_treemenu_report_setup"); break;
		case WORK_DYUN_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_UNIT)); strSVGPath = _T("ico24_treemenu_report_unit"); break;
		case WORK_DYDB_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Reference_DB)); strSVGPath = _T("ico24_treemenu_report_referencedb"); break;
		case WORK_DYIM_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Images)); strSVGPath = _T("ico24_treemenu_report_imagefiles"); break;
		case WORK_DYTB_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Tables)); strSVGPath = _T("ico24_treemenu_report_tables"); break;
		case WORK_UCHT_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Charts)); strSVGPath = _T("ico24_treemenu_report_charts"); break;
		case WORK_DYTT_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_Texts)); strSVGPath = _T("ico24_treemenu_report_text"); break;
		case WORK_UFTR_FLD: tStr.Format(_LS(IDS_TM_DYNA_REPORT_HEADER_FOOTER)); strSVGPath = _T("ico24_treemenu_report_headerfooter"); break;
	}

	if(bInit==FALSE) // 처음 생성하는게 아니면
	{
		BOOL  bExist=FALSE;
		if(ItemHasChildren(m_hRootTi)==TRUE) // Work Tree에 기존의 트리가 존재하면
		{
			hChildItem = GetChildItem(m_hRootTi);
			while (hChildItem != NULL)
			{
				hNextItem = GetNextSiblingItemWithoutFilter(hChildItem);
				UINT Data = GetItemData(hChildItem);
				if(Data==insItem) { bExist=TRUE; break; }
				hChildItem = hNextItem;
			}
		}
		if(bExist) // ITEM이 존재하면 
		{
			if(nCount) // 써야할 그룹 이름이 존재하면 
			{
				// 그 그룹의 Text를 Refresh : 갯수가 바뀔 수 있으므로 
				TCHAR aStr[50]; _stprintf(aStr,_LS(IDS_TMWK__MSG58),tStr);
				SetItemText(hChildItem, (LPTSTR)aStr);

				hTI = hChildItem;
				hChildItem = GetChildItem(hTI);
				while (hChildItem != NULL)
				{
					hNextItem = GetNextSiblingItemWithoutFilter(hChildItem);
					DeleteItem(hChildItem);        
					hChildItem = hNextItem;
				}
			}
			else { DeleteItem(hChildItem); return NULL; }
		}
		else // 처음은 아닌데 ITEM이 존재하지 않으면 
		{
			if(nCount)
			{
				BOOL  bExpand = FALSE;
				if(ItemHasChildren(m_hRootTi)==FALSE) bExpand=TRUE; // 처음 삽입되는 아이템이면 펼쳐라


			  TV_INSERTSTRUCT  TreeCtrlItem;
			  TCHAR aStr[50];

				HTREEITEM insAfter = GetPreviousItemToInsertAfter(insItem);
				if (insAfter == NULL) TreeCtrlItem.hInsertAfter = TVI_FIRST;
				else 	             TreeCtrlItem.hInsertAfter = insAfter;
				TreeCtrlItem.hParent = m_hRootTi;
				TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	      TreeCtrlItem.item.lParam = insItem;
	      TreeCtrlItem.item.iImage = imageID;
	      TreeCtrlItem.item.iSelectedImage = imageID;
				_stprintf(aStr,_LS(IDS_TMWK__MSG58),tStr);
	      TreeCtrlItem.item.pszText = (LPTSTR)aStr;

	      hTI = InsertItem(&TreeCtrlItem);
				if(bExpand) ExpandCompletely(m_hRootTi, TRUE);
			}
			else { return NULL; }
		}
	}
	else // 처음생성하는 것이라면 
	{
		if(nCount) 
		{ 
			hTI = InsertTreeItem(m_hRootTi,insItem,tStr, strSVGPath);
		}
		else { return NULL; }
	}
	return hTI;
}

void CTreeReportCtrl::OnWorktreeExpandAll()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	
	ExpandCompletely(aItem[0], TRUE);
}

void CTreeReportCtrl::OnWorktreeExpand1LEVEL()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;
	ExpandCompletely(aItem[0], FALSE);
	Expand(aItem[0], TVE_EXPAND);
}

void CTreeReportCtrl::OnWorktreeExpand2LEVEL()
{
	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return;

	ExpandCompletely(aItem[0], FALSE);
	Expand(aItem[0], TVE_EXPAND);
	if(ItemHasChildren(aItem[0]))
	{
		 HTREEITEM hNextItem;
		 HTREEITEM hChildItem = GetChildItem(aItem[0]);
		 while (hChildItem != NULL)
		 {
				hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
				Expand(hChildItem, TVE_EXPAND);
				hChildItem = hNextItem;
		 }
	}
}

void CTreeReportCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if(GetSafeHwnd())
	{
		//COLORREF clrBack =  (numColors == -1 ) ? RGB( 192, 220, 192) : RGB( 255, 255, 255);
		//COLORREF clrBack =  (numColors == -1 ) ? GetNearestColor(GetDC()->m_hDC,RGB(192, 220, 192)) : GetNearestColor(GetDC()->m_hDC,RGB(192, 220, 192));
	  // change the background color
	  SetBkColor( m_BkColor );
		Invalidate();
	}
}

void CTreeReportCtrl::SetDynReportFigureToView(HTREEITEM ht)
{
	UINT data = GetItemData(ht);
	ASSERT(data==WORK_UFIG_LST);

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}
	
	// 선택한 아이템의 위치 찾기
	HTREEITEM prvHt,curHt=ht;
	int pos = 0;
	BOOL bSuccess=FALSE;
	
	for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
	
	CArray<T_UFIG_K, T_UFIG_K> aUfigK;
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	int nUfigNum = aUfigK.GetSize();
	ASSERT(nUfigNum>0);
	
	T_UFIG_K UfigK=0;
	T_UFIG_D UfigD;
	
	UfigK = aUfigK[pos-1];
	if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
	{ ASSERT(0); return; } 
	
	CWaitCursor wt;

	// model view로 이동
	I_GENModelBase* pIGM = I_GENModelBase::GetCurMySelfST();
	ASSERT(pIGM);
	
	CWnd* pModelViewWnd = pIGM->GetOwnerWnd();
	ASSERT(pModelViewWnd);
	
	CWnd* pModelViewFrm = pModelViewWnd->GetParentFrame();
	ASSERT(pModelViewFrm);
	
	CMDIChildWnd *pWnd = ((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	if (pWnd != pModelViewFrm)
		((CMDIFrameWnd*)AfxGetMainWnd())->MDIActivate(pModelViewFrm);

	// 그리기
	CGPSAMRUtil::GetInstance()->GetMakeFigure(UfigK);
}

void CTreeReportCtrl::OnWorktreeInsert() 
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod==NULL)
	{
		ASSERT(0);
		CString strMsg=_T("");
#if defined(_ORG)
		strMsg = _T("Smart Report Generator를 실행하십시오.");
#else
		strMsg = _LS(IDS_TM_DYNA_REPORT_REPORT_ERROR);
#endif
		AfxMessageBox(strMsg);
		return;
	}

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

#if defined(_CIVIL)
	if(!pDoc->IsActiveAMRWordView())
		return;
#endif

	CArray<HTREEITEM, HTREEITEM> aItem;
	GetSelectedItemList(aItem);
	
	InsertBySelectItems(aItem);
	
	DeleteTempFiles(m_vctAllGenSegmentElements);
	DeleteTempFiles(m_vctTableGenSegmentElements);
}

CString CTreeReportCtrl::ReplaceSymbolByTempChar(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	strTemp.Replace(_T("_"), _T(" ")); 
	//strTemp.Replace("\n", " "); 
	//   sParam.Replace("\\","_");// 
	strTemp.Replace(_T("~"),_T(" "));
	strTemp.Replace(_T("!"),_T(" "));
	strTemp.Replace(_T("@"),_T(" "));
	strTemp.Replace(_T("#"),_T(" "));
	strTemp.Replace(_T("$"),_T(" "));
	strTemp.Replace(_T("%"),_T(" "));
	strTemp.Replace(_T("^"),_T(" "));
	strTemp.Replace(_T("&"),_T(" "));
	strTemp.Replace(_T("*"),_T(" "));
	strTemp.Replace(_T("("),_T(" "));
	strTemp.Replace(_T(")"),_T(" "));
	strTemp.Replace(_T("-"),_T(" "));
	strTemp.Replace(_T("+"),_T(" "));
	strTemp.Replace(_T("="),_T(" "));
	strTemp.Replace(_T("\\"),_T(" "));
	strTemp.Replace(_T("|"),_T(" "));
	strTemp.Replace(_T("["),_T(" "));
	strTemp.Replace(_T("{"),_T(" "));
	strTemp.Replace(_T("]"),_T(" "));
	strTemp.Replace(_T("}"),_T(" "));
	strTemp.Replace(_T(":"),_T(" "));
	strTemp.Replace(_T(";"),_T(" "));
	strTemp.Replace(_T("\'"),_T(" "));
	strTemp.Replace(_T("\""),_T(" "));
	strTemp.Replace(_T("<"),_T(" "));
	strTemp.Replace(_T(","),_T(" "));
	strTemp.Replace(_T("."),_T(" "));
	strTemp.Replace(_T(">"),_T(" "));
	strTemp.Replace(_T("/"),_T(" "));
	strTemp.Replace(_T("?"),_T(" "));

	return strTemp; 
}

// 1:항목있음, 2:항목없음
int CTreeReportCtrl::IsUsedText(HTREEITEM hItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = hItem;
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);
	
	int nRet=2;
	int nSize=0;

	CString strTextItem=_T("");
	strTextItem = m_aReportTextStrName[pos-1];

	//CReportAutoRegeneration AutoRegen;
	nRet = m_AutoRegen.IsUsedText(strTextItem);

	/*
	if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Control_Data))==0)
	{
		nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Static_Loadcase))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Story))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStor();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Node))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountNode();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Boundary))==0)
	{
		int nCons = pDoc->m_pAttrCtrl->GetCountCons();  // support
		int nNspr = pDoc->m_pAttrCtrl->GetCountNspr();  // point spring support
		nSize = nCons + nNspr;
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Floor_Diaphragm_Rigid_Link))==0)
	{
		// floor diaphragm
		CArray<T_STOR_K, T_STOR_K> aStorK, aDStorK;
		T_STOR_D StorD; StorD.Initialize();
		pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
		for (int i=aStorK.GetSize()-1; i>=0; i--)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
			if (StorD.bFloorDiaphragm) aDStorK.Add(aStorK[i]);
		}
		int nDiap = aDStorK.GetSize();
		
		// rigid link
		int nRigd = pDoc->m_pAttrCtrl->GetCountRigd();

		nSize = nDiap + nRigd;
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aBeamK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp))
				aBeamK.Add(aElemK[i]);
		}
		nSize = aBeamK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Truss))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aTrusK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp))
				aTrusK.Add(aElemK[i]);
		}
		nSize = aTrusK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Wall))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aWallK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
				aWallK.Add(aElemK[i]);
		}
		nSize = aWallK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSect();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Wind_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountWind();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Seismic_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSeis();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Load_Combination))==0)
	{
		CArray<T_LCOM_K, T_LCOM_K> aGenLcomK, aStlLComK, aConLcomK, aSRCLcomK, aFdnLcomK;
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,  aGenLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL,    aStlLComK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, aConLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SRC,      aSRCLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_FDN,      aFdnLcomK);
		int nGenSize = aGenLcomK.GetSize();
		int nStlSize = aStlLComK.GetSize();
		int nConSize = aConLcomK.GetSize();
		int nSRCSize = aSRCLcomK.GetSize();
		int nFdnSize = aFdnLcomK.GetSize();
		nSize = nGenSize+nStlSize+nConSize+nSRCSize+nFdnSize; 
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Self_Weight))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountBodf();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountCnld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Finishing_Material_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountFmld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_System_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountNtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Element_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountEtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam_Section_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountBtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSplc();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Global_Control))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountPolc();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Load_Case))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountPolc();
		if(nSize>0) nRet = 1;
	}
	else
	{
		ASSERT(0); nRet = 2;
	}
	*/
	return nRet;
}

// 1:파일존재, 2:파일존재하지 않음.
int CTreeReportCtrl::IsExistFile(HTREEITEM hItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	UINT data=0; 
	data = GetItemData(hItem);
	if(data != WORK_UIMG_LST && data != WORK_UTPL_LST) return 2;

	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = hItem;
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();
	
	CFileCtrl FileCtrl2(strModelPath);
	CString strFolder=_T("");
	strFolder = FileCtrl2.GetFilePath();

	CString strFilePath=_T("");
	if(data == WORK_UIMG_LST)
	{
		CArray<T_UIMG_K, T_UIMG_K> aUimgK;
		pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
		int nUimgNum = aUimgK.GetSize();
		if(nUimgNum<=0)
		{
			ASSERT(0); return 2;
		}
		
		T_UIMG_K  UimgK=0;
		T_UIMG_D  UimgD;
		strFilePath=_T("");
		
		UimgK = aUimgK[pos-1];
		if(!pDoc->m_pAttrCtrl->GetUimg(UimgK, UimgD))
		{
			ASSERT(0); return 2;
		}
		//strFilePath = UimgD.strFilePath;
		strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
	}
	else if(data == WORK_UTPL_LST)
	{
		CArray<T_UTPL_K, T_UTPL_K> aUtplK;
		pDoc->m_pAttrCtrl->GetUtplKeyList(aUtplK);
		int nUtplNum = aUtplK.GetSize();
		if(nUtplNum<=0)
		{
			ASSERT(0); return 2;
		}
		
		T_UTPL_K  UtplK=0;
		T_UTPL_D  UtplD;
		strFilePath=_T("");
		
		UtplK = aUtplK[pos-1];
		if(!pDoc->m_pAttrCtrl->GetUtpl(UtplK, UtplD))
		{
			ASSERT(0); return 2;
		}
		//strFilePath = UtplD.strFilePath;
		strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Table_Template_Files__s), strFolder, UtplD.strFilePath);
	}

	ASSERT(strFilePath!=_T(""));
	CFileCtrl FileCtrl(strFilePath);
	if(!FileCtrl.FileExists(strFilePath))
	{
		return 2;
	}

	return 1;
}

// 1:전처리 테이블, 2:후처리 테이블
int CTreeReportCtrl::IsPostTable(HTREEITEM hItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	UINT data=0; 
	data = GetItemData(hItem);
	if(data != WORK_UTBL_LST) return 2;
	
	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = hItem;
	for (prvHt = ht; prvHt != NULL; ht = prvHt)
	{
		prvHt = GetPrevSiblingItemWithoutFilter(ht);
		pos++;
	}
	
	CString strFilePath=_T("");
	if(data == WORK_UTBL_LST)
	{
		CArray<T_UTBL_K, T_UTBL_K> aUtblK;
		pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
		int nUtblNum = aUtblK.GetSize();
		if(nUtblNum<=0)
		{
			ASSERT(0); return 2;
		}
		
		T_UTBL_K  UtblK=0;
		T_UTBL_D  UtblD;
		strFilePath=_T("");
		
		if(pos-1 >= nUtblNum) pos = nUtblNum-1;
		UtblK = aUtblK[pos-1];
		if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
		{
			ASSERT(0); return 2;
		}
		if(UtblD.nTBMode==D_UTBL_TYPE_DDGN && !pDoc->IsPostMode())
			return 2; 
		if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
			return 2;
		if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
			return 2;
		if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
		{ 				
//       if(!pDoc->IsPostMode())
//         return 2;
			CStringArray aPscCodeName;
			CDBLib::GetPscCodeNameList(aPscCodeName);
			if(aPscCodeName.GetSize()<=0)      
				return 2;
		}
	}
	
	return 1;
}

// 1: 가능, 2: 가능하지 않음.
int CTreeReportCtrl::IsEnableChart(HTREEITEM hItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	UINT data=0; 
	data = GetItemData(hItem);
	if(data != WORK_UCHT_LST) return 2;
	
	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = hItem;
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);

	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;
	CString strFilePath=_T("");

	if(data == WORK_UCHT_LST)
	{
		CArray<T_UCHT_K, T_UCHT_K> aUchtK;
		pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
		int nUchtNum = aUchtK.GetSize();
		if(nUchtNum<=0)
		{
			ASSERT(0); return 2;
		}
		
		int nUcht = m_aUchtRes.GetSize();
		ASSERT(pos-1 < nUcht);

		int nRet = m_aUchtRes[pos-1];
		return nRet;

//     T_UCHT_K  UchtK=0;
//     T_UCHT_D  UchtD;
//     strFilePath=_T("");
//     
//     UchtK = aUchtK[pos-1];
//     if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
//     {
//       ASSERT(0); return 2;
//     }
// 
//     if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
//        ( UchtD.nType==2 || UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
//       return 2;
// 
//     if(!GenChart.GetData(UchtK, ChartItemD, strFilePath))
//       return 2;
	}
	
	return 1;
}

int CTreeReportCtrl::IsEnableImage(HTREEITEM hItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}
	
	UINT data=0; 
	data = GetItemData(hItem);
	if(data != WORK_UFIG_LST) return 2;
	
	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = hItem;
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(ht);
	
	CString strFilePath=_T("");
	if(data == WORK_UFIG_LST)
	{
		CArray<T_UFIG_K, T_UFIG_K> aUfigK;
		pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
		int nUfigNum = aUfigK.GetSize();
		if(nUfigNum<=0)
		{
			ASSERT(0); return 2;
		}
		
		T_UFIG_K  UfigK=0;
		T_UFIG_D  UfigD;
		strFilePath=_T("");
		
		UfigK = aUfigK[pos-1];
		if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
		{
			ASSERT(0); return 2;
		}
		if(!pDoc->m_pPostCtrl->IsAnalysisResultEnable() && !UfigD.bPre && // 해석결과가 없고, 저장된 이미지가 후처리 이미지일때 
			 UfigD.PostFrsiD.nCurrentMode>0)                                // 후처리 결과 이미지는 비활성
		{
			 return 2;
		}
	}
	
	return 1;
}

void CTreeReportCtrl::SetUchtRes()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	CArray<T_UCHT_K, T_UCHT_K> aUchtK;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	int nUchtNum = aUchtK.GetSize();
	if(nUchtNum<=0) return;

	m_aUchtRes.RemoveAll();
	m_aUchtRes.SetSize(nUchtNum);
	for(int i=0; i<nUchtNum; i++)
		m_aUchtRes[i]=0;

	T_UCHT_K  UchtK=0;
	T_UCHT_D  UchtD;

	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;
	CString strFilePath=_T("");

	for(int i=0; i<nUchtNum; i++)
	{
		UchtK = aUchtK[i];
		if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
		{
			ASSERT(0); continue;
		}

		if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
			(  UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
		{
			m_aUchtRes[i] = 2;
			continue;
		}  
//     else if(!pDoc->m_pPostCtrl->IsPostEnable() && UchtD.nType==2 )
//     {
//       m_aUchtRes[i] = 2;
//       continue;
//     }
		if(!GenChart.GetData(UchtK, ChartItemD, strFilePath, 1))
		{
			m_aUchtRes[i] = 2;
			continue;
		}  
		m_aUchtRes[i] = 1;
	}
}

void CTreeReportCtrl::SortCscsKey(CDBDoc* pDoc, CArray<T_CSCS_K, T_CSCS_K>& raCscsK)
{
	int nSize = raCscsK.GetSize();
	if (nSize <= 1) return;  // Sorting할 필요 없음
	CArray<T_POS_KEY2_PAIR, T_POS_KEY2_PAIR&> aPosKey2;
	aPosKey2.SetSize(nSize);
	T_CSCS_D CscsD;
	T_STAG_D StagD;
	for (int i = 0; i < nSize; i++)
	{
		pDoc->m_pAttrCtrl->GetCscs(raCscsK[i], CscsD);
		pDoc->m_pAttrCtrl->GetStag(CscsD.ActStagKey, StagD);
		aPosKey2[i].Set(i, StagD.nStageId, CscsD.SectKey);
	}
	qsort(aPosKey2.GetData(), aPosKey2.GetSize(), sizeof(T_POS_KEY2_PAIR), CCompFunc::ComparePosKey2PairA);
	CArray<T_CSCS_K, T_CSCS_K> aTmpCscsK;
	aTmpCscsK.Copy(raCscsK);
	for(int i=0; i < nSize; i++)
		raCscsK[i] = aTmpCscsK[aPosKey2[i].nPos];
}

void CTreeReportCtrl::OnWorktreeAddImageFile() 
{
	CReportImageFileDlg dlg;
	dlg.DoModal();
}

void CTreeReportCtrl::OnWorktreeSaveImageFile()
{
	CWaitCursor wt;
	OnWorktreeDisplay();

	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_FILE_EXPORT_GRAPHIC, 0), 0);
}

void CTreeReportCtrl::ViewDynReportTable(HTREEITEM ht)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}
	
	UINT data = GetItemData(ht);
	// 선택한 아이템의 위치 찾기
	if(data != WORK_UTBL_LST) return;
	
	HTREEITEM prvHt,curHt=ht;
	int pos = 0;
	for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
	
	CArray<T_UTBL_K, T_UTBL_K> aUtblK;
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	int nUtblNum = aUtblK.GetSize();
	ASSERT(nUtblNum>0);
	
	T_UTBL_K UtblK=0;
	T_UTBL_D UtblD;
	UtblK = aUtblK[pos-1];
	if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
	{
		ASSERT(0); return;
	}
	if(UtblD.nTBMode==D_UTBL_TYPE_PRE || UtblD.nTBMode==D_UTBL_TYPE_POST || UtblD.nTBMode==D_UTBL_TYPE_PO 
		 || UtblD.nTBMode==D_UTBL_TYPE_DGN || UtblD.nTBMode==D_UTBL_TYPE_PSC || UtblD.nTBMode==D_UTBL_TYPE_CMD
		 || UtblD.nTBMode == D_UTBL_TYPE_BCG_PRE || UtblD.nTBMode == D_UTBL_TYPE_BCG_POST )
	{
		if (UtblD.nTBMode != D_UTBL_TYPE_PRE && UtblD.nTBMode != D_UTBL_TYPE_BCG_PRE)
		{
			if (!pDoc->IsPostMode())
				return;
		}

		if(FALSE == ItemHasChildren(ht))
		{
			if (UtblD.nTBMode == D_UTBL_TYPE_PRE)
			{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(UtblD.nMenuID, 0), 0L);
			}
			else if(UtblD.nMenuID > 0)
			{
				//WPARAM wParam = MAKEWPARAM(ID_SHOW_TABLE_BY_UTBL,0);
				//AfxGetMainWnd()->SendMessage(WM_COMMAND,wParam,0L);
				// For Post Table
				pDoc->ShowTableByUtbl(UtblK);
			}
		}    
	}

//   if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
//     return;
//   if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
//     return;
//   if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
//   { 				
//     if(!pDoc->IsPostMode())
//       return 2;
//     CStringArray aPscCodeName;
//     CDBLib::GetPscCodeNameList(aPscCodeName);
//     if(aPscCodeName.GetSize()<=0)      
//       return 2;
//   }

}

// 파일 이름에 (\, /, :, *, ?, ", <, >, |)를 사용할 수 없으므로, 이러한 문자가 있을시에 없애줌.
void CTreeReportCtrl::FilterFileName(CString& strFileName)
{
	strFileName.Replace(_T("\\"), _T(""));
	strFileName.Replace(_T("/"),_T(""));
	strFileName.Replace(_T(":"),_T(""));
	strFileName.Replace(_T("*"),_T(""));
	strFileName.Replace(_T("?"),_T(""));
	strFileName.Replace(_T("\""),_T(""));
	strFileName.Replace(_T("<"),_T(""));
	strFileName.Replace(_T(">"),_T(""));
	strFileName.Replace(_T("|"),_T(""));
}

void CTreeReportCtrl::InsertReportHeaderFooter() 
{
	CReportCommonFunc::InsertReportHeaderFooter();
}

void CTreeReportCtrl::InsertReportItemText_Model()
{
	InsertReportItemText_Method(WORK_CHRP_UMLD_LST,WORK_CHRP_UMLD_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_MODEL));
}

void CTreeReportCtrl::InsertReportItemText_LoadCapacity()
{
	InsertReportItemText_Method(WORK_CHRP_UBCL_LST,WORK_CHRP_UBCL_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_LOADCAPACITY));
}
void CTreeReportCtrl::InsertReportItemText_Method(UINT data1,UINT data2,const CString& strItem)
{
	if(m_nDataType!=D_DATATYPE_LST && m_nDataType!=D_DATATYPE_1ST_FLD) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	int selItemNum = m_ahDragItems.GetSize();
	if(selItemNum==0) return;

	UINT data=0; 
	data = GetItemData(m_ahDragItems[0]);
	if(data != data1 && data != data2 ) return;

	//CStringArray  aCaption ; // Caption
	//CStringArray  aName    ; // 추출정보이름
	//CStringArray  aContents; // 내용
	//aCaption .RemoveAll();
	//aName    .RemoveAll();
	//aContents.RemoveAll();

	HTREEITEM hItem=0;
	HTREEITEM prvHt=0,curHt=0;
	int pos = 0;

	CString	sProductInfo=_T(" ");
	CString strCaption=_T("");
	CString strText=_T("");
	CString strContents=_T("");
	CReportTreeText ReportTreeText;
	vector<GenSegmentElements> AllSegmentElements;
	GenSegmentElements SegmentElements;
	GenSegmentElements DTSegmentElements;

	// Make bmp file
	// 1. List 항목을 Drag & Drop 할 경우
	
	if(m_nDataType==D_DATATYPE_LST) 
	{
		m_iChapterID = 0;
		m_iSegmentID = 0;
		for(int nItem=0; nItem<selItemNum; nItem++)
		{
			hItem = 0;
			hItem = m_ahDragItems[nItem];
			
			curHt=hItem;
			pos = 0;
			for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);
			HTREEITEM grouphItem = GetParentItem(hItem);
			//      strText = m_aReportTextStrName[pos-1];
			
			int iIndex = FindIndexOfReportTexts(grouphItem,m_mapCHReportTextStrNames);
			int iIndex2 = FindIndexOfReportTexts(grouphItem,m_mapCHReportTextCaptions);
			if(iIndex != -1 && iIndex2 != -1)
			{
				m_iSegmentID++;
				SegmentElements.Clear();
				strText = m_mapCHReportTextStrNames[iIndex].second.at(pos-1);
				if(!ReportTreeText.WriteReportText_CH(strText, SegmentElements))
					continue;
				strCaption = m_mapCHReportTextCaptions[iIndex2].second.at(pos-1);
				for (std::vector<CH_GEN_IFTAG_STR>::iterator pit = SegmentElements.m_SegmentElements.begin();
				pit != SegmentElements.m_SegmentElements.end();++pit)
				{
					CH_GEN_IFTAG_STR& iFTAG_STR = *pit;
					if(iFTAG_STR.iLevel == 1)
					ReportTreeText.ReplaceTitileNumber(0,0,iFTAG_STR.sContents);
					if(iFTAG_STR.iLevel == 2)
					ReportTreeText.ReplaceTitileNumber(0,m_iSegmentID,iFTAG_STR.sContents);
					else
						ReportTreeText.ReplaceTitileNumber(m_iChapterID,m_iSegmentID,iFTAG_STR.sContents);

					}

				AllSegmentElements.push_back(SegmentElements);
			}
			else
			{
				ASSERT(0);
			}
			
		}
	}
	// 2. Type 자체 항목을 Drag & Drop 할 경우
	else if(m_nDataType==D_DATATYPE_1ST_FLD)
	{
		hItem = m_ahDragItems[0];
		HTREEITEM grouphItem = hItem;
		int iIndex = FindIndexOfReportTexts(grouphItem,m_mapCHReportTextStrNames);
		int iIndex2 = FindIndexOfReportTexts(grouphItem,m_mapCHReportTextCaptions);
		
		if(iIndex != -1 && iIndex2 != -1)
		{
			int nCount=0;
			nCount = (int)m_mapCHReportTextStrNames[iIndex].second.size();
			
			for(int nItem=-1; nItem<nCount; nItem++)
			{
				if(nItem == -1)
				{
					strContents = strItem;
					strContents = ReportTreeText.BuilderChapterTitle(m_iChapterID,m_iSegmentID,strContents);
					ReportTreeText.WriteTitle(2,strContents);
//           aCaption.Add(strItem);
//           aName.Add(strItem);
//           aContents.Add(strContents);

					SegmentElements.Clear();
					SegmentElements.m_strSegmentName = strItem;
					CH_GEN_IFTAG_STR genFtagStr;
					genFtagStr.sCaption = strItem;
					genFtagStr.sType         = _LSX(TXT);
					genFtagStr.sFullPath     = _T(" ");
					genFtagStr.sExtInfo      = strItem;
					genFtagStr.sContents     = strContents;
					genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
					genFtagStr.sExt          = _T(" ");
					genFtagStr.iLevel        = 1;
					SegmentElements.m_SegmentElements.push_back(genFtagStr);
					AllSegmentElements.push_back(SegmentElements);

				}
				else
				{
					strText = m_mapCHReportTextStrNames[iIndex].second.at(nItem);
					if(!CChinaReportSetting::GetChinaReportSetting()->GetChecked(strText))
						continue;
					m_iSegmentID++;
					SegmentElements.Clear();
					if(!ReportTreeText.WriteReportText_CH(strText, SegmentElements))
						continue;
					for (std::vector<CH_GEN_IFTAG_STR>::iterator pit = SegmentElements.m_SegmentElements.begin();
					pit != SegmentElements.m_SegmentElements.end();++pit)
					{
						CH_GEN_IFTAG_STR& iFTAG_STR = *pit;
						ReportTreeText.ReplaceTitileNumber(m_iChapterID,m_iSegmentID,iFTAG_STR.sContents);
					}
//          ReportTreeText.BuilderChapterTitle(m_iChapterID,m_iSegmentID,strContents);
					//strCaption = m_mapCHReportTextCaptions[iIndex2].second.at(nItem);
					
					//aCaption.Add(strCaption);
					//aName.Add(strText);
					//aContents.Add(strContents);
					AllSegmentElements.push_back(SegmentElements);

				}
				
			}
		}
		
		
	}
	if(!AllSegmentElements.empty())
		m_vctAllGenSegmentElements.insert(m_vctAllGenSegmentElements.end(),AllSegmentElements.begin(),AllSegmentElements.end());


#if defined(_CIVIL)
		sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
		sProductInfo  = _LSX(GEN);
#else
		ASSERT(0);
		sProductInfo  = _T(" ");
#endif
		
		for (vector<GenSegmentElements>::iterator pit = AllSegmentElements.begin();pit!=AllSegmentElements.end();++pit)
		{
			GenSegmentElements& segs = *pit;
			for (std::vector<CH_GEN_IFTAG_STR>::iterator pitSeg = segs.m_SegmentElements.begin();pitSeg!=segs.m_SegmentElements.end();++pitSeg)
			{
				CH_GEN_IFTAG_STR& Iftag_str = *pitSeg;
				Iftag_str.sProductInfo = sProductInfo;
				if(m_bInsert)
				{
					GEN_IFTAG Iftag;
					CReportCommonFunc::ChangeStructStr2Char(&Iftag_str,&Iftag);
					if(Iftag_str.iLevel == 1)
					{
						CReportCommonFunc::ExportData2Word_ByStyle(&Iftag,1);
					}
					else if(Iftag_str.iLevel == 2)
					{
						CReportCommonFunc::ExportData2Word_ByStyle(&Iftag,2);
					}
					else
						CReportCommonFunc::ExportData2Word(&Iftag);
				}
				
			}
		}

	 ReportTreeText.WriteDetailTables(DTSegmentElements);
	 if(!DTSegmentElements.m_SegmentElements.empty())
			m_vctTableGenSegmentElements.push_back(DTSegmentElements);

//   aName.FreeExtra();
//     //aContents.FreeExtra();
//   
//     // Make structure to export
//     //int nName = aName.GetSize();
//     int nContents = aContents.GetSize();
//     ASSERT(nName==nContents);
//     if(nName<=0) return;
//     
//     int nItemCnt=0;
//     nItemCnt = nName;
//   
//     CString	sProductInfo=_T(" "); // 제품정보
//     CString	sCaption    =_T(" "); // Caption
//     CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
//     CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
//     CString	sExtInfo    =_T(" "); // 추출정보
//     CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
//     int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
//     CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL
//   
//     GEN_IFTAG* pIftag=NULL;
//     pIftag = new GEN_IFTAG[nItemCnt];
//   
//     sProductInfo  = _LSX(CIVIL);
//   
//     for(int nItem=0; nItem<nItemCnt; nItem++)
//     {
//       sCaption      = aCaption[nItem];
//       sType         = _LSX(TXT);
//       sFullPath     = _T(" ");
//       sExtInfo      = aName[nItem];
//       sContents     = aContents[nItem];
//       iAppendFlag   = D_WORD_APPEND_INSERT; 
//       sExt          = _T(" ");
//       
//       pIftag[nItem].sProductInfo = sProductInfo;
//       pIftag[nItem].sCaption     = sCaption    ;
//       pIftag[nItem].sType        = sType       ;
//       pIftag[nItem].sFullPath    = sFullPath   ;
//       pIftag[nItem].sExtInfo     = sExtInfo    ;
//       pIftag[nItem].sContents    = sContents   ;
//       pIftag[nItem].sExt         = sExt        ;
//       pIftag[nItem].iAppendFlag  = iAppendFlag;
//   
//     }
//   
//     // Export to Word
//     HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
//     if(hMod != NULL) 
//     {
//       EditorExport pExport=NULL;
//       pExport = (EditorExport) GetProcAddress(hMod, _T("EditorExport"));
//       int nRet = pExport(nItemCnt, &pIftag);
//       pExport=NULL;
//     }
//     else
//     {
//       ASSERT(0);
//       AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
//     }
//     hMod = NULL;
//   
//   	delete[] pIftag;
}

void CTreeReportCtrl::InsertReportItemText_ServiceAbility()
{
	InsertReportItemText_Method(WORK_CHRP_UNUL_FLD,WORK_CHRP_UNUL_LST,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SERVICEABILITY));
}

void CTreeReportCtrl::InsertReportItemText_Stress()
{
	InsertReportItemText_Method(WORK_CHRP_ULSC_FLD,WORK_CHRP_ULSC_LST,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_STRESS));
}

void CTreeReportCtrl::InsertReportItemText_Stress_ShortTerm()
{
	InsertReportItemText_Method(WORK_CHRP_USSC_FLD,WORK_CHRP_USSC_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SHORTTERMSTRESS));
}
void CTreeReportCtrl::InsertReportItemText_BasicInfo()
{
	InsertReportItemText_Method(WORK_CHRP_UBSI_LST,WORK_CHRP_UBSI_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_BASICINFO));
}
void CTreeReportCtrl::InsertReportItemText_ForceMemb()
{
	InsertReportItemText_Method(WORK_CHRP_FCMB_LST,WORK_CHRP_FCMB_FLD,_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_FORCE));
}

void CTreeReportCtrl::InsertBySelectItems( CArray<HTREEITEM, HTREEITEM>& aItem )
{
	int selItemNum = aItem.GetSize();
	if(selItemNum==0) return;
	
	UINT data = GetItemData(aItem[0]);
	
	int nDataType=0;  // 1: 1st FLD, 2: 2nd FLD, 3: LST
	switch(data)
	{
	case WORK_UCHT_FLD:
	case WORK_UFIG_FLD:
	case WORK_DYIM_FLD:
	case WORK_DYTB_FLD:
	case WORK_UTBL_FLD:
	case WORK_DYTT_FLD:  
	case WORK_UTXT_FLD:
	case WORK_USUM_FLD:
	case WORK_USTB_FLD:  
	case WORK_USSS_FLD:  
	case WORK_USRS_FLD:  
	case WORK_USCS_FLD:  
	case WORK_UTPL_FLD:
	case WORK_UIMG_FLD:  
		nDataType = D_DATATYPE_1ST_FLD;
		break;
	case WORK_UCHT_LST:
	case WORK_UFIG_LST:
	case WORK_UTBL_LST:  
	case WORK_UTXT_LST:    
	case WORK_USUM_LST:
	case WORK_USSS_LST:  
	case WORK_USRS_LST:  
	case WORK_USCS_LST:
	case WORK_UTPL_LST:
	case WORK_UIMG_LST:  
		nDataType = D_DATATYPE_LST;
		break;
	case WORK_CHRP_UBSI_LST:  //add by JUHONGHUA(MITC) 2012.2.14
	case WORK_CHRP_UMLD_LST:  
	case WORK_CHRP_UBCL_LST:
	case WORK_CHRP_UNUL_LST:
	case WORK_CHRP_ULSC_LST:  
	case WORK_CHRP_USSC_LST:  
	case WORK_CHRP_FCMB_LST:  

		nDataType = D_DATATYPE_LST;
		break;
	case WORK_CHRP_UBSI_FLD:  //add by JUHONGHUA(MITC) 2012.2.14
	case WORK_CHRP_UMLD_FLD:  
	case WORK_CHRP_UBCL_FLD:
	case WORK_CHRP_UNUL_FLD:
	case WORK_CHRP_ULSC_FLD:  
	case WORK_CHRP_USSC_FLD:  
	case WORK_CHRP_FCMB_FLD:  

		nDataType = D_DATATYPE_1ST_FLD;
		break;
		
	default:
		nDataType = 0;
		break;
	}
	m_nDataType = nDataType;
	
	m_ahDragItems.RemoveAll();
	if      (nDataType==D_DATATYPE_1ST_FLD)  m_ahDragItems.Add(aItem[0]);
	else if (nDataType==D_DATATYPE_LST    )  m_ahDragItems.Copy(aItem);
	else  { ASSERT(0); }
	m_ahDragItems.FreeExtra();
	
	InsertReportItem();
	
	m_nDataType = 0;
	m_ahDragItems.RemoveAll();
}

void CTreeReportCtrl::GetReportTextCaptions( CStringArray& aReportTextCaption )
{
#if defined(_CH)
	{
		for (std::vector<pair<HTREEITEM,vector<CString> > >::const_iterator pit = m_mapCHReportTextCaptions.begin();
			pit != m_mapCHReportTextCaptions.end();++pit)
		{
			const vector<CString>& vctStrs = pit->second;
			for (int i=0;i<vctStrs.size();i++)
			{
				aReportTextCaption.Add(vctStrs.at(i));
			}
		}
	}
#else
	aReportTextCaption.Copy(m_aReportTextCaption);
#endif
	
}

void CTreeReportCtrl::GetReportTextStrNames( CStringArray& aReportTextStrName )
{
#if defined(_CH)
	{
		for (std::vector<pair<HTREEITEM,vector<CString> > >::const_iterator pit = m_mapCHReportTextStrNames.begin();
			pit != m_mapCHReportTextStrNames.end();++pit)
		{
			const vector<CString>& vctStrs = pit->second;
			for (int i=0;i<vctStrs.size();i++)
			{
				aReportTextStrName.Add(vctStrs.at(i));
			}
		}
	}
#else
	aReportTextStrName.Copy(m_aReportTextStrName);
#endif

}



void CTreeReportCtrl::InsertChinaReportItemAll()
{
	 m_iChapterID = 0;
	 m_iSegmentID = 0;

	 m_vctAllGenSegmentElements.clear();
	 m_vctTableGenSegmentElements.clear();


	InsertCoverPage();
	InsertCatalogPage();

	CArray<HTREEITEM,HTREEITEM> listSelectItems;
	HTREEITEM HTreeItem = NULL;
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_BASICINFO)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
		HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_BasicInfo_General));
		listSelectItems.Add(HTreeItem);
		InsertBySelectItems(listSelectItems);

	}
	
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_MODEL)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
	 HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_Model_Calculation));
	listSelectItems.Add(HTreeItem);
	InsertBySelectItems(listSelectItems);

	}
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_FORCE)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
		HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_Force_MemberForce));
		listSelectItems.Add(HTreeItem);
		InsertBySelectItems(listSelectItems);
		
	}
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_LOADCAPACITY)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
	 HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth));
	listSelectItems.Add(HTreeItem);
	InsertBySelectItems(listSelectItems);

	}
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SERVICEABILITY)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
	 HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_ServiceAbility_Normal));
	listSelectItems.Add(HTreeItem);
	InsertBySelectItems(listSelectItems);

	}
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_STRESS)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
	 HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_Stress_NormalComp));
	listSelectItems.Add(HTreeItem);
	InsertBySelectItems(listSelectItems);

	}
	if(CChinaReportSetting::GetChinaReportSetting()->GetChecked(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SHORTTERMSTRESS)))
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		listSelectItems.RemoveAll();
		HTreeItem = FindHTreeItemBySubName(_LS(IDS_AMR_CHRP_Stress_ShortTerm));
		listSelectItems.Add(HTreeItem);
		InsertBySelectItems(listSelectItems);

	}
	//if(CChinaReportSetting::GetChinaReportSetting()->IsWholeCalProcess())
	{
		m_iChapterID++;
		m_iSegmentID = 0;
		InsertReportItemText_DetailTable();
	}
	
	if(m_bInsert)
	{
		CReportCommonFunc::ExportInsertPageNumber(1,1,true);
		CReportCommonFunc::ExportUpdateCatalog();

		DeleteTempFiles(m_vctAllGenSegmentElements);
	}
}

HTREEITEM CTreeReportCtrl::FindHTreeItemByName( const CString& strName )
{
	auto it = m_mapCHReportStrName_New.find(strName);
	if(it != m_mapCHReportStrName_New.end())
		return it->second;
	ASSERT(0);
	return NULL;
}


HTREEITEM CTreeReportCtrl::FindHTreeItemBySubName( const CString& strSubName )
{
	for (std::vector<pair<HTREEITEM,vector<CString> > >::const_iterator pit = m_mapCHReportTextStrNames.begin();
	pit!=m_mapCHReportTextStrNames.end();++pit)
	{
		 const vector<CString>& subNames = pit->second;
		 for (vector<CString>::const_iterator pitSub = subNames.begin();pitSub!=subNames.end();++pitSub)
		 {
			 if(*pitSub == strSubName)
				 return pit->first;
		 }
	}
	ASSERT(0);
	return NULL;
}

int CTreeReportCtrl::FindIndexOfReportTexts( HTREEITEM htree,const std::vector<pair<HTREEITEM,vector<CString> > >& strs )
{
	int iIndex = 0 ;
	for (std::vector<pair<HTREEITEM,vector<CString> > >::const_iterator pit = strs.begin();
	pit!=strs.end();++pit,++iIndex)
	{
		if(htree == pit->first)
			return iIndex;
	}
	return -1;
}

void CTreeReportCtrl::InsertCoverPage()
{
	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	
	CString sProductInfo;
#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif


	if(!m_bInsert)
		return;
	CString strText = _T("");
	strText = _T("");
	CString strTemp=_T("");
	CString csFileName=_T("");
	T_PJCF_D data;
	if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPjcf(data)) 
		data.Initialize();
	CString strProjectName = data.strProject;
	CString csPath = CDBDoc::GetDocPoint()->GetPathName();
 
		CFileCtrl file(csPath);
		CFileStatus status;
		file.GetFileStatus(status);
		csFileName = file.GetFileNameWithoutExtension();

	if(data.strProject.IsEmpty() || data.strProject == _T(""))
	{
		strProjectName = csFileName;
	}

	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center[/DRG_FONT]\n\n\n%s\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"),strProjectName, _LS(IDS_AMR_CHRP__CoverPage));
	strText += strTemp;

	CString strDate = _T("");
	CTime time;
	time=CTime::GetCurrentTime();
	int nYear = time.GetYear ();
	int nMonth = time.GetMonth ();
	int nDate = time.GetDay ();
	strDate.Format(_T("%d-%d-%d"),nYear,nMonth,nDate);
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=center[/DRG_FONT]\n\n\n\n\n%s\n%s\n%s[DRG_FONT]|FI|[/DRG_FONT]"),_LS(IDS_AMR_CHRP__CoverPage_Designer),_LS(IDS_AMR_CHRP__CoverPage_Proofreader), _LS(IDS_AMR_CHRP__CoverPage_Verifyer));
	strText += strTemp;

	strTemp.Format(_T("[DRG_FONT]|FC| size=12 bold=false align=center[/DRG_FONT]\n\n\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"),strDate);
	strText += strTemp;

	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _LS(IDS_AMR_CHRP__CoverPage);
	pSrc.sCaption += _LS(IDS_AMR_CHRP__CoverPage);

	pSrc.sContents = strText;
	pSrc.sExt = _T("");
	pSrc.sExtInfo = _LS(IDS_AMR_CHRP__CoverPage);
	pSrc.sFullPath = _T("");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc,&pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);



	CReportCommonFunc::ExportInsertPageBreak();

}

void CTreeReportCtrl::InsertCatalogPage()
{
	if(!m_bInsert)
		return;
	CString strText = _T("");
	strText = _T("");
	CString strTemp=_T("");
	
	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=true align=center[/DRG_FONT] %s \n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_AMR_CHRP__Contents));
	strText += strTemp;
	
	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	
	CString sProductInfo;
#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif
	
	
	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _LS(IDS_AMR_CHRP__Contents);
	pSrc.sContents = strText;
	pSrc.sExt = _T(" ");
	pSrc.sExtInfo = _LS(IDS_AMR_CHRP__Contents);
	pSrc.sFullPath = _T(" ");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc,&pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertCatalog(2);
	CReportCommonFunc::ExportInsertPageBreak();
}

void CTreeReportCtrl::SetInsertState( BOOL bInsert )
{
	m_bInsert = bInsert;
}

void CTreeReportCtrl::GetAllGenSegments( std::vector<GenSegmentElements>& segs )
{
	segs = m_vctAllGenSegmentElements;
}

void CTreeReportCtrl::InsertReportItemText_DetailTable()
{

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if(!pDoc)
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}



	CString	sProductInfo=_T(" ");
	CString strCaption=_T("");
	CString strText=_T("");
	CString strContents=_T("");
	CReportTreeText ReportTreeText;
	vector<GenSegmentElements> AllSegmentElements;
	GenSegmentElements SegmentElements;

			CString    strItem = _LS(IDS_AMR_CHRP_Detail_Table);
					strContents = strItem;
					strContents = ReportTreeText.BuilderChapterTitle(m_iChapterID,m_iSegmentID,strContents);
					ReportTreeText.WriteTitle(2,strContents);
					SegmentElements.Clear();
					SegmentElements.m_strSegmentName = strItem;
					CH_GEN_IFTAG_STR genFtagStr;
					genFtagStr.sCaption = strItem;
					genFtagStr.sType         = _LSX(TXT);
					genFtagStr.sFullPath     = _T(" ");
					genFtagStr.sExtInfo      = strItem;
					genFtagStr.sContents     = strContents;
					genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
					genFtagStr.sExt          = _T(" ");
					genFtagStr.iLevel        = 1;
					SegmentElements.m_SegmentElements.push_back(genFtagStr);

					strText = _T("");
					strText.Format(_LS(IDS_AMR_CHRP__TableUnit));
					
					genFtagStr.sCaption = _LS(IDS_AMR_CHRP_Detail_Table);
					genFtagStr.sCaption += _LS(IDS_AMR_CHRP__Explanation);
					genFtagStr.sType         = _LSX(TXT);
					genFtagStr.sFullPath     = _T("");
					genFtagStr.sExtInfo      = genFtagStr.sCaption;
					genFtagStr.sContents     = strText;
					genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
					genFtagStr.sExt          = _T("");
					genFtagStr.iLevel        = 0;
					
					SegmentElements.m_SegmentElements.push_back(genFtagStr);

					AllSegmentElements.push_back(SegmentElements);

			 
	if(!m_vctTableGenSegmentElements.empty())
		AllSegmentElements.insert(AllSegmentElements.end(),m_vctTableGenSegmentElements.begin(),m_vctTableGenSegmentElements.end());


#if defined(_CIVIL)
		sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
		sProductInfo  = _LSX(GEN);
#else
		ASSERT(0);
		sProductInfo  = _T(" ");
#endif
		
		if(CChinaReportSetting::GetChinaReportSetting()->IsWholeCalProcess())
		{
			//渴놔圈玖셕炬깊목
			for (vector<GenSegmentElements>::iterator pit = AllSegmentElements.begin();pit!=AllSegmentElements.end();++pit)
			{
				GenSegmentElements& segs = *pit;
				for (std::vector<CH_GEN_IFTAG_STR>::iterator pitSeg = segs.m_SegmentElements.begin();pitSeg!=segs.m_SegmentElements.end();++pitSeg)
				{
					CH_GEN_IFTAG_STR& Iftag_str = *pitSeg;
					Iftag_str.sProductInfo = sProductInfo;
					if(m_bInsert)
					{
						GEN_IFTAG Iftag;
						CReportCommonFunc::ChangeStructStr2Char(&Iftag_str,&Iftag);
						if(Iftag_str.iLevel == 1)
						{
							CReportCommonFunc::ExportData2Word_ByStyle(&Iftag,1);
						}
						else if(Iftag_str.iLevel == 2)
						{
							CReportCommonFunc::ExportData2Word_ByStyle(&Iftag,2);
						}
						else
							CReportCommonFunc::ExportData2Word(&Iftag);
					}
					
				}
				
			}

			if(!AllSegmentElements.empty())
			{
				m_vctAllGenSegmentElements.insert(m_vctAllGenSegmentElements.end(),AllSegmentElements.begin(),AllSegmentElements.end());
			}
		}
		else
		{
			 DeleteTempFiles(m_vctTableGenSegmentElements);
		}


	
}

void CTreeReportCtrl::DeleteTempFiles(const std::vector<GenSegmentElements>& allsegs )
{
	CFileFind file;
	for (std::vector<GenSegmentElements>::const_iterator pit = allsegs.begin();
	pit !=allsegs.end();++pit)
	{
		const GenSegmentElements& segs = *pit;
		for (std::vector<CH_GEN_IFTAG_STR>::const_iterator pit1 = segs.m_SegmentElements.begin();
		pit1 != segs.m_SegmentElements.end();++pit1)
		{
			if(file.FindFile(pit1->sFullPath))
				::DeleteFile(pit1->sFullPath);
		}
	}
	
	file.Close();
}

CTreeReportCtrl* CTreeReportCtrlInstance::GetTreeReportCtrlInstance()
{
	static CTreeReportCtrl m_Static ;
	return &m_Static;
}

CTreeReportCtrlInstance::~CTreeReportCtrlInstance()
{
	OutputDebugString(_T("~CTreeReportCtrlInstance"));
}

void CTreeReportCtrl::OnWorktreeReportUnit() 
{
	WPARAM wParam = 0;
	LPARAM lParam = MAKELPARAM(1, 1);
	SendMessage(WM_MOUSEMOVE, wParam, lParam);

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum>1) 
	{
		AfxMessageBox(_LS(IDS_TMWK__MSG170)); 
		return;
	}

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	UINT data = GetItemData(aItem[0]);
	// 선택한 아이템의 위치 찾기

	HTREEITEM prvHt,curHt=aItem[0];
	int pos = 0;

	for(prvHt = curHt;prvHt!=NULL;curHt = prvHt,pos++) prvHt = GetPrevSiblingItemWithoutFilter(curHt);

	CArray<unsigned int, unsigned int> aKey;
	int nKey=0;
	switch(data)
	{
	case WORK_UTBL_LST:
		pDoc->m_pAttrCtrl->GetUtblKeyList(aKey);
		nKey = aKey[pos-1];
		break;
	case WORK_UCHT_LST:
		pDoc->m_pAttrCtrl->GetUchtKeyList(aKey);
		nKey = aKey[pos-1];
		break;
	case WORK_DYUN_FLD:
	case WORK_USSS_LST:
	case WORK_USRS_LST:
	case WORK_USCS_LST:
		break;
	default:
		ASSERT(0);
		break;
	}

	CReportUnitCustomizeDlg dlg;
	dlg.m_nMode = data;
	dlg.m_Key   = nKey;
	dlg.DoModal();
} 

