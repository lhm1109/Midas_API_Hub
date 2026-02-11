// CMLlanJPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLlanJPDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_strParser.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\FlagCtrl.h"  //added by KYM
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_db\ViewCtrl.h"  //added by KYM
#include "..\wg_db\StagCtrl.h"  //added by KYM
#include "..\wg_db\VehlDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_NOM    0
#define D_NUM_ELE    1
#define D_NUM_ECC    2
#define D_NUM_CF_s   3
#define D_NUM_CF_r   4
#define D_NUM_IF     5
#define D_NUM_SPAN   6
#define D_NUM_COL    D_NUM_SPAN

/////////////////////////////////////////////////////////////////////////////
// CCMLlanJPDlg dialog


CCMLlanJPDlg::CCMLlanJPDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLlanJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLlanJPDlg)
	m_nLoadDistType    = 0;
	m_nMovDir          = -1;
	m_bCentForce       = FALSE;
	m_nSelectionMethod = 0;
	//}}AFX_DATA_INIT
	m_bModify   = FALSE;
	m_bListMode = TRUE;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_COPY);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_LANE_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_LANE_NAME_EDIT);
	// Traffic Lane Properties
	m_aCtrlData.Add(IDC_CMD_ST_LOADDIST);  
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC_GRP);
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC);
	m_aCtrlData.Add(IDC_CMD_WIDTH_STC);
	m_aCtrlData.Add(IDC_CMD_WIDTH_EDT);
	m_aCtrlData.Add(IDC_CMD_WIDTH_UNT);
	m_aCtrlData.Add(IDC_CMD_ECCEN_TITLE);
	m_aCtrlData.Add(IDC_CMD_ECCEN_EDIT);
	m_aCtrlData.Add(IDC_CMD_ECCEN_UNIT);
	m_aCtrlData.Add(IDC_CMD_FACTOR_TITLE);
	m_aCtrlData.Add(IDC_CMD_FACTOR_EDIT);
	m_aCtrlData.Add(IDC_CMD_CF_CHK);
	m_aCtrlData.Add(IDC_CMD_CF_GRP);
	m_aCtrlData.Add(IDC_CMD_CF_SPEED_STC);
	m_aCtrlData.Add(IDC_CMD_CF_SPEED_EDT);
	m_aCtrlData.Add(IDC_CMD_CF_SPEED_UNIT);
	m_aCtrlData.Add(IDC_CMD_CF_RADIUS_STC);
	m_aCtrlData.Add(IDC_CMD_CF_RADIUS_EDT);
	m_aCtrlData.Add(IDC_CMD_CF_RADIUS_UNIT);
	m_aCtrlData.Add(IDC_CMD_CF_IF_CHK);

	// Vehicular Load Distribution
	m_aCtrlData.Add(IDC_CMD_FRAME1);   
	m_aCtrlData.Add(IDC_CMD_LANE_ELEM);
	m_aCtrlData.Add(IDC_CMD_CROSSBEAM);
	m_aCtrlData.Add(IDC_CMD_ST_CROSSBEAMGRUP);
	m_aCtrlData.Add(IDC_CMD_CROSSBEAM_GROUP);
	m_aCtrlData.Add(IDC_CMD_SKEW_TITLE_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_START_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_END_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_START_EDIT);
	m_aCtrlData.Add(IDC_CMD_SK_END_EDIT);
	m_aCtrlData.Add(IDC_CMD_SK_DEG_UNIT);
	// Moving Direction
	m_aCtrlData.Add(IDC_CMD_ST_MOVINGDIR);  
	m_aCtrlData.Add(IDC_CMD_MOV_FORWARD);
	m_aCtrlData.Add(IDC_CMD_MOV_BACKWARD);
	m_aCtrlData.Add(IDC_CMD_MOV_BOTH);	 
	// Selection by
	m_aCtrlData.Add(IDC_CMD_FRAME7);        
	m_aCtrlData.Add(IDC_CMD_2POINTS);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT1);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT2);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT1);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT2);
	m_aCtrlData.Add(IDC_CMD_PICKING);
	m_aCtrlData.Add(IDC_CMD_PICKING_EDIT);
	m_aCtrlData.Add(IDC_CMD_ELEMSELECT);
	m_aCtrlData.Add(IDC_CMD_ELEMSELECT_EDIT);
	// Operations
	m_aCtrlData.Add(IDC_CMD_FRAME2);   
	m_aCtrlData.Add(IDC_CMD_OP_ADD);
	m_aCtrlData.Add(IDC_CMD_OP_INSERT);
	m_aCtrlData.Add(IDC_CMD_OP_DELETE);

	m_aCtrlData.Add(IDC_CMD_PLACEHOLDER);  // Grid
	m_aCtrlData.Add(IDC_CMD_BTN_OK);
	m_aCtrlData.Add(IDC_CMD_BTN_CANCEL);
	m_aCtrlData.Add(IDC_CMD_BTN_APPLY);

	////////////////////////////////////
	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
	m_aCtrlSelect.Add(IDC_CMD_ELEMSELECT);

}


void CCMLlanJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLlanJPDlg)
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Control(pDX, IDC_CMD_LIST          , m_List);

	DDX_Control(pDX, IDC_CMD_WIDTH_EDT   , m_edtWidth);
	DDX_Control(pDX, IDC_CMD_WIDTH_UNT   , m_untWidth);
	DDX_Control(pDX, IDC_CMD_ECCEN_EDIT  , m_wndEccenEdit);
	DDX_Control(pDX, IDC_CMD_ECCEN_UNIT  , m_wndEccenUnit);
	DDX_Control(pDX, IDC_CMD_FACTOR_EDIT , m_wndIFEdit);

	DDX_Check(pDX,   IDC_CMD_CF_CHK, m_bCentForce);
	DDX_Check(pDX,   IDC_CMD_CF_IF_CHK, m_bCentIF);
	DDX_Control(pDX, IDC_CMD_CF_SPEED_EDT,   m_wndCF_s);
	DDX_Control(pDX, IDC_CMD_CF_SPEED_UNIT,  m_untCF_s);
	DDX_Control(pDX, IDC_CMD_CF_RADIUS_EDT,  m_wndCF_r);
	DDX_Control(pDX, IDC_CMD_CF_RADIUS_UNIT, m_untCF_r);

	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2   , m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT1   , m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_CROSSBEAM_GROUP, m_cmbCrossBeamElemGroup);
	DDX_Control(pDX, IDC_CMD_ELEMSELECT_EDIT, m_wndElemSelectEdit);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1  , m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2  , m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT   , m_wndPickingEdit);

	DDX_Radio  (pDX, IDC_CMD_LANE_ELEM      , m_nLoadDistType);
	DDX_Radio  (pDX, IDC_CMD_MOV_FORWARD    , m_nMovDir);
	DDX_Radio  (pDX, IDC_CMD_2POINTS        , m_nSelectionMethod);
	DDX_Control(pDX, IDC_CMD_SK_END_EDIT    , m_edtSkewStart);
	DDX_Control(pDX, IDC_CMD_SK_START_EDIT  , m_edtSkewEnd);

	DDX_Control(pDX, IDC_CMD_BMP_STATIC_GRP , m_chkBmp);

	DDX_Control(pDX, IDC_CMD_BMP_STATIC, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLlanJPDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLlanJPDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,    OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE,  OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY,   OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_BTN_OK,     OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY,  OnCmdBtnApply)
	ON_BN_CLICKED(IDC_CMD_OP_ADD,     OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT,  OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE,  OnCmdOpDelete)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST,       OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_LANE_ELEM,         OnCmdLoadDistType)
	ON_BN_CLICKED(IDC_CMD_CROSSBEAM,         OnCmdLoadDistType)
	ON_BN_CLICKED(IDC_CMD_2POINTS,        OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_PICKING,        OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_ELEMSELECT,     OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_CF_CHK,         OnCmdCentForceChk)

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

#define COLCOUNT 1
/////////////////////////////////////////////////////////////////////////////
// CCMLlanJPDlg implementation functions
void CCMLlanJPDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY= 0;
	int nDistX= 0;

	// Traffic Line Lnaes List 위치로 전부 이동
	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_NAME_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);

	// Traffic Lane Properties 안에 그림을 제외한 컨트롤 이동 (Fold Group Box Auto Effect 꺼서 들어간 코드...)
	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_WIDTH_STC);
	aControls.Add(IDC_CMD_WIDTH_EDT);
	aControls.Add(IDC_CMD_WIDTH_UNT);
	aControls.Add(IDC_CMD_ECCEN_TITLE);
	aControls.Add(IDC_CMD_ECCEN_EDIT);
	aControls.Add(IDC_CMD_ECCEN_UNIT);
	aControls.Add(IDC_CMD_FACTOR_TITLE);
	aControls.Add(IDC_CMD_FACTOR_EDIT);
	aControls.Add(IDC_CMD_CF_GRP);
	aControls.Add(IDC_CMD_CF_CHK);
	aControls.Add(IDC_CMD_CF_SPEED_STC);
	aControls.Add(IDC_CMD_CF_SPEED_EDT);
	aControls.Add(IDC_CMD_CF_SPEED_UNIT);
	aControls.Add(IDC_CMD_CF_RADIUS_STC);
	aControls.Add(IDC_CMD_CF_RADIUS_EDT);
	aControls.Add(IDC_CMD_CF_RADIUS_UNIT);    
	aControls.Add(IDC_CMD_CF_IF_CHK);

	GetDlgItem(IDC_CMD_BMP_STATIC_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ECCEN_EDIT)->GetWindowRect(rToMove);
	if(m_chkBmp.GetFoldState()) nDistY = rRef.bottom - rToMove.top;
	else                        nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);


	// Vehicular Load Distribution
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME1);
	aControls.Add(IDC_CMD_LANE_ELEM);
	aControls.Add(IDC_CMD_CROSSBEAM);
	aControls.Add(IDC_CMD_ST_CROSSBEAMGRUP);
	aControls.Add(IDC_CMD_CROSSBEAM_GROUP);
	aControls.Add(IDC_CMD_SKEW_TITLE_STATIC);
	aControls.Add(IDC_CMD_SK_START_STATIC);
	aControls.Add(IDC_CMD_SK_END_STATIC);
	aControls.Add(IDC_CMD_SK_START_EDIT);
	aControls.Add(IDC_CMD_SK_END_EDIT);
	aControls.Add(IDC_CMD_SK_DEG_UNIT);

	GetDlgItem(IDC_CMD_ST_LOADDIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Moving Direction
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ST_MOVINGDIR);
	aControls.Add(IDC_CMD_MOV_FORWARD);
	aControls.Add(IDC_CMD_MOV_BACKWARD);
	aControls.Add(IDC_CMD_MOV_BOTH);

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Selection by
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME7);
	aControls.Add(IDC_CMD_2POINTS);
	aControls.Add(IDC_CMD_PICKING);
	aControls.Add(IDC_CMD_ELEMSELECT);
	aControls.Add(IDC_CMD_2POINTS_EDIT1);
	aControls.Add(IDC_CMD_2POINT_UNIT1);
	aControls.Add(IDC_CMD_2POINTS_EDIT2);
	aControls.Add(IDC_CMD_2POINT_UNIT2);

	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rRef); 
	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// Operations
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME2);
	aControls.Add(IDC_CMD_OP_ADD);
	aControls.Add(IDC_CMD_OP_INSERT);
	aControls.Add(IDC_CMD_OP_DELETE);

	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Grid
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_PLACEHOLDER);

	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// OK, Cancel, Apply
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_BTN_OK);
	aControls.Add(IDC_CMD_BTN_CANCEL);
	aControls.Add(IDC_CMD_BTN_APPLY);
	
	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_BTN_CANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);


#if defined(_CIVIL_JP) //(by jbseon)
	CWnd* pWndCheckBox = GetDlgItem(IDC_CMD_CF_CHK);
	CRect rectCheckBox;
	pWndCheckBox->GetWindowRect(rectCheckBox);
	ScreenToClient(rectCheckBox);
	rectCheckBox.right = rectCheckBox.left + globalUtils.ScaleByDPI(100);
	pWndCheckBox->MoveWindow(rectCheckBox);
#endif	

	ResizeDialog();
}

void CCMLlanJPDlg::ResizeDialog()
{
	UINT nLastCtrlID;
	if (m_bListMode) nLastCtrlID = IDC_CMD_BTN_CLOSE;
	else nLastCtrlID = IDC_CMD_BTN_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);

	MoveWindow(r);

	Invalidate();
}

void CCMLlanJPDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Lane_Name)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 140;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMLlanJPDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	T_LLANjp_D LlanD;
	CArray<T_LLANjp_K, T_LLANjp_K> aLlanK;
	m_pDoc->m_pAttrCtrl->GetLlanjpKeyList(aLlanK);

	for (int i = 0; i < aLlanK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLlanjp(aLlanK[i], LlanD)) continue;

		InsertItem(aLlanK[i], LlanD);
	}
}

void CCMLlanJPDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		MakeItemEx();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLlanJPDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
}

void CCMLlanJPDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_LLANjp_K Key, KeyBak;
	T_LLANjp_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_LLANjp_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetLlanjp(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_LLANjp_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetLlanjp(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_LLANjp_MFD):
			{
				pViewBuff->GetLlanjp(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_LLANjp_MFS):
			{
				pViewBuff->GetLlanjp(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
			}
			bMFD = FALSE;
			break;
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

void CCMLlanJPDlg::UpdateUnit()
{
	SetGridTitle();
	if (m_bListMode) return;  // unit change할 게 없다.
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	int nUnitEccent = CUnitCtrl::m_LLANjp_UNIT.Eccentricity;
	int nUnitCF_s = CUnitCtrl::m_LLANjp_UNIT.dCF_s;
	int nUnitCF_r = CUnitCtrl::m_LLANjp_UNIT.dCF_r;
	double dblEccen, dCF_s, dCF_r;
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		dblEccen = _tstof(m_wndGrid.GetValueRowCol(i+1, D_NUM_ECC));
		dblEccen = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitEccent, dblEccen);
		m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_ECC), dblEccen);

		dCF_s = _tstof(m_wndGrid.GetValueRowCol(i + 1, D_NUM_CF_s));
		dCF_s = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitCF_s, dCF_s);
		m_wndGrid.SetValueRange(CGXRange(i + 1, D_NUM_CF_s), dCF_s);

		dCF_r = _tstof(m_wndGrid.GetValueRowCol(i + 1, D_NUM_CF_r));
		dCF_r = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitCF_r, dCF_r);
		m_wndGrid.SetValueRange(CGXRange(i + 1, D_NUM_CF_r), dCF_r);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

int CCMLlanJPDlg::FindInsertionPos(CString& strName)
{
	T_LLANjp_K key;
	T_LLANjp_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetLlanjp(key, data);

		if (strName.Compare(data.LineLaneName) < 0) break;
	}
	return i;    
}

BOOL CCMLlanJPDlg::InsertItem(T_LLANjp_K Key, T_LLANjp_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LineLaneName);
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMLlanJPDlg::DeleteItem(T_LLANjp_K Key, T_LLANjp_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMLlanJPDlg::ModifyItem(T_LLANjp_K KeyOld, T_LLANjp_K Key, T_LLANjp_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CCMLlanJPDlg::DataToStr(int i, T_LLANjp_K Key, T_LLANjp_D &Data)
{
	CString str;
	if (i == 0) str = Data.LineLaneName;
	else str = _T("");

	return str;
}

void CCMLlanJPDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.LineLaneName);
	InitItemDataVar();

	m_edtWidth  .SetEditUnit(m_Data.dLineWidth);
	m_bCentForce    = m_Data.bCentForce;
	m_bCentIF       = m_Data.bIncludeIF;
	m_nLoadDistType = m_Data.nLoadDistribution -1;
	m_nMovDir       = m_Data.nMovingDir;

	if (m_Data.arLaneData.GetSize() > 0)
	{
		m_wndEccenEdit.SetEditUnit(m_Data.arLaneData[0].Eccentricity);
		m_wndCF_s.SetEditUnit(m_Data.arLaneData[0].dCF_s);
		m_wndCF_r.SetEditUnit(m_Data.arLaneData[0].dCF_r);
	}

	//if(m_Data.nLoadDistribution == 2)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cmbCrossBeamElemGroup, m_Data.ElemGroup);

		CString str;
		str.Format(_T("%g"), m_Data.dSkewStart);
		m_edtSkewStart.SetWindowText(str);

		str.Format(_T("%g"), m_Data.dSkewEnd);
		m_edtSkewEnd.SetWindowText(str);
	}

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.arLaneData.GetSize();
	m_wndGrid.SetRowCount(nSize);
	for (int i = 0; i < nSize; i++)
	{
		m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_ELE), (long)m_Data.arLaneData[i].KeyElem);
		m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_ECC), m_Data.arLaneData[i].Eccentricity);
		//if(IsUseCentrifugalForce())
		{
			m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_CF_s), m_Data.arLaneData[i].dCF_s);
			m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_CF_r), m_Data.arLaneData[i].dCF_r);

			m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_IF), m_Data.arLaneData[i].dImpactFactor);
			CString str = (m_Data.arLaneData[i].bSpanStart) ? _T("1") : _T("0");
			m_wndGrid.SetValueRange(CGXRange(i+1, D_NUM_SPAN), str);
		}
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	UpdateData(FALSE);

	OnCmdLoadDistType();
	SelectElem();			// added by KYM
	OnCmdCentForceChk();
}

BOOL CCMLlanJPDlg::Dlg2Data()
{
	T_LLANjp_BASE item; item.Initialize();
//	double dx,dy,dz,dux,duy,duz;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	UpdateData(TRUE);
	m_wndLaneName.GetWindowText(m_Data.LineLaneName);

	m_Data.dLineWidth = m_edtWidth.GetEditValue();
	m_Data.bCentForce = m_bCentForce;
	m_Data.bIncludeIF = (!m_bCentForce)? FALSE : m_bCentIF;
	m_Data.nLoadDistribution = m_nLoadDistType +1;
	m_Data.nMovingDir = m_nMovDir;

	if(m_nLoadDistType == 1)
	{
		DWORD dwCobxItemData = 0;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbCrossBeamElemGroup, dwCobxItemData);  
		m_Data.ElemGroup = (T_GRUP_K)dwCobxItemData;

		m_edtSkewStart.GetEditValue(m_Data.dSkewStart);
		m_edtSkewEnd.GetEditValue(m_Data.dSkewEnd);
	}

	int nCount = m_wndGrid.GetRowCount();
	m_Data.arLaneData.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		item.KeyElem      = _tstol(m_wndGrid.GetValueRowCol(i+1, D_NUM_ELE));
		item.Eccentricity = _tstof(m_wndGrid.GetValueRowCol(i+1, D_NUM_ECC));
		if (IsUseCentrifugalForce())
		{
			//item.dCF_s = _tstof(m_wndGrid.GetValueRowCol(i+1, D_NUM_CF_s));
			item.dCF_r = _tstof(m_wndGrid.GetValueRowCol(i+1, D_NUM_CF_r));
			CFormulaEdit::GetEditValue(&m_wndCF_s, item.dCF_s);
			//CFormulaEdit::GetEditValue(&m_wndCF_r, item.dCF_r);
		}
		else
		{
			item.dCF_s = 0.0;
			item.dCF_r = 0.0;
		}

		item.dImpactFactor = _tstof(m_wndGrid.GetValueRowCol(i+1, D_NUM_IF));
		item.bSpanStart    = (m_wndGrid.GetValueRowCol(i+1, D_NUM_SPAN) == _T("1")) ? TRUE : FALSE;

		m_Data.arLaneData.SetAt(i, item);
	}

	return TRUE;
}

void CCMLlanJPDlg::InitItemDataVar()
{
	m_wndEccenEdit.SetWindowText(_T("0.0"));
	m_wndCF_s.SetWindowText(_T("0.0"));
	m_wndCF_r.SetWindowText(_T("0.0"));
	m_wndIFEdit.SetWindowText(_T("1.0"));
	
	m_wnd2PointsEdit1 .SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2 .SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit  .SetWindowText(_T(""));
	
	OnCmdLoadDistType();
}

void CCMLlanJPDlg::InitCombo()
{
	m_cmbCrossBeamElemGroup.ResetContent();

	 CArray<T_GRUP_K, T_GRUP_K>GrupKeyList;
	 m_pDoc->m_pAttrCtrl->GetGrupKeyList(GrupKeyList);

	T_GRUP_D rData;
	int nGrup=GrupKeyList.GetSize();
	for(int i=0; i<nGrup; i++)
	{
		T_GRUP_K Key = GrupKeyList[i];
		if(!m_pDoc->m_pAttrCtrl->GetGrup(Key, rData)) continue;

		CDlgUtil::CobxAddItem(m_cmbCrossBeamElemGroup, rData.GroupName, Key );
	}

	CDlgUtil::CobxSetCurSelItemData(m_cmbCrossBeamElemGroup, m_Data.ElemGroup);

}

void CCMLlanJPDlg::InitGrid()
{
	m_wndGrid.SubclassDlgItem(IDC_CMD_PLACEHOLDER, this);
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
			.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetAllowEnter(FALSE));

	// Special grid settings
	pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	//pParam->EnableTrackColWidth(FALSE);          // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();
	m_wndGrid.SetColWidthByDPI(D_NUM_NOM, D_NUM_NOM, 25);
	m_wndGrid.SetColWidthByDPI(D_NUM_ELE, D_NUM_ELE, 40);
	m_wndGrid.SetColWidthByDPI(D_NUM_ECC, D_NUM_ECC, 50);
	m_wndGrid.SetColWidthByDPI(D_NUM_CF_s, D_NUM_CF_s,50);
	m_wndGrid.SetColWidthByDPI(D_NUM_CF_r, D_NUM_CF_r,50);
	m_wndGrid.SetColWidthByDPI(D_NUM_IF  , D_NUM_IF,  40);
	m_wndGrid.SetColWidthByDPI(D_NUM_SPAN, D_NUM_SPAN, 40);

	// Check Box Column의 스타일 변경
	m_wndGrid.SetStyleRange(CGXRange().SetCols(D_NUM_SPAN), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		//.SetValue(_T("0"))
	);
  //m_wndGrid.EnableOleDataSource(GX_DNDDISABLED); // copy & paste 금지
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMLlanJPDlg::SetGridTitle()
{
	CString csUnit, csCF_s, csCF_r;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLANjp_UNIT.Eccentricity, csUnit);
  //m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLANjp_UNIT.dCF_s       , csCF_s);
  //m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLANjp_UNIT.dCF_r       , csCF_r);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, D_NUM_NOM), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, D_NUM_ELE), _LS(IDS_WG_CMD__ADDD__Elem));
	m_wndGrid.SetValueRange(CGXRange(0, D_NUM_ECC), CString(_LS(IDS_WG_CMD__ADDD__Eccen__n_)) + csUnit + _T(")"));
	//if (IsUseCentrifugalForce())
	{
// 		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_CF_s), _T("Speed\n(") + csCF_s + _T(")"));
		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_CF_s), CString(_LS(IDS_WG_CMD__ADDD__Speed)) + csCF_s + _T(")"));
		
//		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_CF_r), _T("Radius\n(") + csCF_r + _T(")"));
		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_CF_r), CString(_LS(IDS_WG_CMD__ADDD__Radius)) + csCF_r + _T("m)"));

		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_IF), _T("ia"));  
		m_wndGrid.SetValueRange(CGXRange(0, D_NUM_SPAN), _LS(IDS_CMD_SLAN_SPAN_START));
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMLlanJPDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMLlanJPDlg::Grid_InsertItem(int nPos, BOOL bFirst, T_LLANjp_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_ELE), (long)item.KeyElem);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_ECC), item.Eccentricity);
	//if (IsUseCentrifugalForce())
	{
		m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_CF_s), item.dCF_s);
		m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_CF_r), item.dCF_r);
		m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_IF),   item.dImpactFactor);
		CString str = _T("0");
		m_wndGrid.SetValueRange(CGXRange(nRow+1, D_NUM_SPAN), str);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMLlanJPDlg::InsertBaseItem(int nPos, int nType)
{
	T_LLANjp_BASE item;
	CFormulaEdit::GetEditValue(&m_wndEccenEdit, item.Eccentricity);
	CFormulaEdit::GetEditValue(&m_wndCF_s     , item.dCF_s);
	CFormulaEdit::GetEditValue(&m_wndCF_r     , item.dCF_r);
	CFormulaEdit::GetEditValue(&m_wndIFEdit   , item.dImpactFactor);

	if(nType==0) //Select by 2 Points
	{
			CArray<T_ELEM_K, T_ELEM_K> aElemKey;
			double ux, uy, uz;
			double WX1, WY1, WZ1;
			double WX2, WY2, WZ2;

			if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
				return;
			}
			m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
			if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
				return;
			}
			m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

			// Get Elem Key List
			double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();  // added by KYM
			m_pDoc->m_pDataCtrl->GetFrameElemOnLine(WX1, WY1, WZ1, WX2, WY2, WZ2, aElemKey, TRUE);
			// Remove Element which is not beam
			T_ELEM_D Data;
			for (int i = aElemKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aElemKey[i], Data);
				if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
					aElemKey.RemoveAt(i);
			}
			if (aElemKey.GetSize() == 0)
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_beam_element_on_line_));
				return;
			}

			int nElemKey;
			int nCount = aElemKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				nElemKey = aElemKey[i];
				if (Grid_IsExistKey(nElemKey))
				{
					CString msg;
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_already), nElemKey);
					AfxMessageBox(msg);
					return;
				}
				item.KeyElem = (unsigned int)nElemKey;
				BOOL bFirst = (i == 0) ? TRUE : FALSE;
				Grid_InsertItem(nPos, bFirst, item);
				if (nPos >= 0) nPos++;
			}
			SelectElem();    //added by KYM
			return;
	}

	else if(nType==1) //Select by Picking
	{
			int nElemKey;
			CFormulaEdit::GetEditValue(&m_wndPickingEdit, nElemKey);
			if (nElemKey <= 0)
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_exi), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			if (Grid_IsExistKey(nElemKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			T_ELEM_D Data;
			m_pDoc->m_pAttrCtrl->GetElem(nElemKey, Data);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			item.KeyElem = (unsigned int)nElemKey;
			Grid_InsertItem(nPos, FALSE, item);
	}

	else if(nType==2)  //Select Elem Number
	{
		CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
		CArray<long, long> arElemList;

		CString strElemNumber;
	  m_wndElemSelectEdit.GetWindowText(strElemNumber);

		m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

		CNumericOptimizer optimizer;

		optimizer.SelectNumByStrBase((long*)arElemKeyList.GetData(), (long)arElemKeyList.GetSize(),
			                            strElemNumber, arElemList);

		if(arElemList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_element_));
			return;
		}

		for(int i=0; i<arElemList.GetSize(); i++)
			if (Grid_IsExistKey((UINT)arElemList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), (UINT)arElemList[i]);
				AfxMessageBox(msg);
				return;
			}  

		int nInsertPos = nPos;
		for(int i=0; i<arElemList.GetSize(); i++)
		{
			T_ELEM_D Data;

			m_pDoc->m_pAttrCtrl->GetElem((UINT)arElemList[i], Data);

			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), (UINT)arElemList[i]);
				AfxMessageBox(msg);
				return;
			}
			item.KeyElem = (UINT)arElemList[i];
			BOOL bFirst = (i==0)? TRUE : FALSE;
			Grid_InsertItem(nPos, bFirst, item);
			if(nInsertPos >= 0) nPos++;
		}
	}

	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;
	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos+1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);
	SelectElem();   // added by KYM
}

void CCMLlanJPDlg::SelectElem()
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_ELEM_K, T_ELEM_K>arSelElem;
	  for (int i = 0; i < nCount; i++) arSelElem.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectElem(NULL, arSelElem, FALSE, TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMLlanJPDlg message handlers

BOOL CCMLlanJPDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	// Auto Effect 끄자 (Auto Effect 가 왜 안먹을까...)
	m_chkBmp.InitControl(this, CCMLlanJPDlg::IDD, IDC_CMD_BMP_STATIC_GRP,FALSE,TRUE);
	m_chkBmp.SetFoldState(FALSE);
	// TODO: Add extra initialization here
	AlignControl();
	SetHeaderTitle();
	MakeItemEx();
	ButtonCtrl();

	m_edtSkewStart.SetRange(SHRT_MIN, SHRT_MAX);
	m_edtSkewEnd.SetRange(SHRT_MIN, SHRT_MAX);

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_tlin 2.svg");

	m_ImgViewer.SetImage(strImgPath);

	m_edtWidth.EnableWindow(FALSE);
	// InitUnit
	m_edtWidth  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWidth  .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndEccenEdit.SetUnitType(CUnitCtrl::m_LLANjp_UNIT.Eccentricity);
	m_wndEccenUnit.SetUnitType(CUnitCtrl::m_LLANjp_UNIT.Eccentricity);

	m_wndCF_s.SetUnitType(CUnitCtrl::m_LLANjp_UNIT.dCF_s);
	m_untCF_s.SetWindowTextW(_T("km/h"));
	m_wndCF_r.SetUnitType(CUnitCtrl::m_LLANjp_UNIT.dCF_r);
	m_untCF_r.SetWindowTextW(_T("m"));

	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);
	m_wnd2PointsEdit2.SetNextLink(&m_wnd2PointsEdit1);

	m_wndPickingEdit.SetAttElemList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndPickingEdit.SetMaxElemKeyNum(1);

	m_csTitle1 = _LS(IDS_CMD_LLAN_TITLE1); // _T("Traffic Line Lanes")
	m_csTitle2 = _LS(IDS_CMD_LLAN_TITLE2); // _T("Define Design Traffic Lane")

	InitCombo();

	InitGrid();

	InitItemDataVar();

	if(m_bModify)
	{
		SetInitModify();    
		SetWindowText(m_csTitle2);
	}
	else
	{
		SetWindowText(m_csTitle1);
		CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
		CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	}
	
	SelectMethodMan();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMLlanJPDlg::IsUseCentrifugalForce()
{
	if(m_bCentForce) return TRUE;

	return FALSE;
}

void CCMLlanJPDlg::SetParamData(T_LLANjp_D& data)
{
	m_bModify = TRUE;
	m_Data = data;
}

void CCMLlanJPDlg::SetInitModify()
{
	m_csOldLineLaneName = m_Data.LineLaneName;

	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
}

void CCMLlanJPDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_bModify = FALSE;
	m_Data.Initialize();

	//m_Data.dWheelSpace = GetInitialWheelSpacing();
	Data2Dlg();

	m_bListMode = FALSE;
	m_chkBmp.SetFoldState(FALSE);
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
}

void CCMLlanJPDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_LLANjp_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
 
	Key = (T_LLANjp_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetLlanjp(Key, m_Data))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_lane_key__d_d), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldLineLaneName = m_Data.LineLaneName;

	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	m_chkBmp.SetFoldState(FALSE);
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
}

void CCMLlanJPDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}

	CString csLineLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelLlanjp(csLineLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLlanJPDlg::OnCmdBtnCopy() 
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
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanjpList(LLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(LLaneNames,csLineLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}

	
	T_LLANjp_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanjp(csLineLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csLineLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LineLaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddLlanjp(LaneD);
}

void CCMLlanJPDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();	
}

void CCMLlanJPDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}


void CCMLlanJPDlg::OnCmdBtnOk() 
{
	// TODO: Add your control notification handler code here
	BOOL CheckLlan=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckLlan = m_pDoc->m_pDataCtrl->ModifyLlanjp(m_csOldLineLaneName, m_Data);
	else CheckLlan = m_pDoc->m_pDataCtrl->AddLlanjp(m_Data);

	if (!CheckLlan) // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return;

	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMLlanJPDlg::OnCmdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMLlanJPDlg::OnCmdBtnApply() 
{
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) m_pDoc->m_pDataCtrl->ModifyLlanjp(m_csOldLineLaneName, m_Data);
	else m_pDoc->m_pDataCtrl->AddLlanjp(m_Data);
}

void CCMLlanJPDlg::OnCmdChangeSelectBy() 
{
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndElemSelectEdit.SetFocus();
}

LRESULT CCMLlanJPDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(m_nSelectionMethod == 0)
	{
		InsertBaseItem(-1, 0);
	}
	else if(m_nSelectionMethod == 1)
	{
		InsertBaseItem(-1, 1);
		//m_wnd2PointsEdit1.SetFocus();
	}
	else
	{
	
	}
	return 0L;
}

void CCMLlanJPDlg::OnCmdOpAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMLlanJPDlg::OnCmdOpInsert() 
{
	// TODO: Add your control notification handler code here
	ROWCOL nRow, nCol;
	if (m_wndGrid.GetCurrentCell(nRow, nCol))
	{
		nRow = nRow-1;
	}
	else nRow = -1;
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(nRow, nCheck);
}

void CCMLlanJPDlg::OnCmdOpDelete() 
{
	// TODO: Add your control notification handler code here
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	SelectElem();  // added by KYM
}

void CCMLlanJPDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid.LockUpdate(FALSE);
	if (bOldLock) m_wndGrid.Redraw();
	m_wndGrid.LockUpdate(bOldLock);
	// Do not call CCMDlgBase::OnPaint() for painting messages
}


BOOL CCMLlanJPDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMLlanJPDlg::OnCmdLoadDistType() 
{
	UpdateData(TRUE);
	
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_CMD_SK_START_EDIT);
	Ctrls.Add(IDC_CMD_SK_END_EDIT  );

	if(m_nLoadDistType == 0)
	{
		m_cmbCrossBeamElemGroup.EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	}
	else
	{
		m_cmbCrossBeamElemGroup.EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	}
}

BOOL CCMLlanJPDlg::GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz)
{
	MyParser Parser;
	
	if(Parser.ParsingPositionArg(EQStr))
	{
		Parser.GetPositionData(Dx,Dy,Dz);
	}
	else
	{
		 Dx = 0. ; Dy = 0. ; Dz = 0.;  
		 return FALSE;
	}
	return TRUE;
}


void CCMLlanJPDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	CArray<UINT,UINT> ElemSelArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT1     );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
	ElemSelArr.Add(IDC_CMD_ELEMSELECT_EDIT);


	if(m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this, PointArr  , TRUE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE);
	}
	else if( m_nSelectionMethod == 1)
	{
		CRect DRect,SRect;
		
		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndPickingEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top  - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr  , FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, TRUE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE); 

		CDlgUtil::CtrlMoveDistXY(this, PickingArr, nDistX, nDistY);
	}
	else
	{
		CRect DRect, SRect;
		m_wnd2PointsEdit1 .GetWindowRect(&DRect);
		m_wndElemSelectEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top  - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr  , FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, TRUE); 

		CDlgUtil::CtrlMoveDistXY(this, ElemSelArr, nDistX, nDistY);
	}
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}

////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMLlanJPDlg::ExternalSetting(T_LLANjp_K LlanKey)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistLlanjp(LlanKey);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetLlanjp(LlanKey, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	if(IsWindow(m_chkBmp.GetSafeHwnd()))
	{
		m_chkBmp.SetFoldState(FALSE);
	}  

	m_csOldLineLaneName = m_Data.LineLaneName;
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();
	SelectMethodMan();

	return TRUE;
}


LRESULT CCMLlanJPDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignControl();
	SelectMethodMan();

	return 0L;
}

void CCMLlanJPDlg::OnCmdCentForceChk()
{
	UpdateData(TRUE);

	BOOL bCentForce = IsUseCentrifugalForce();
	m_wndGrid.HideCols(D_NUM_CF_s, D_NUM_CF_s, TRUE); // 항상 숨김

	GetDlgItem(IDC_CMD_CF_SPEED_EDT)->EnableWindow(bCentForce);
	GetDlgItem(IDC_CMD_CF_RADIUS_EDT)->EnableWindow(bCentForce);
	GetDlgItem(IDC_CMD_CF_IF_CHK)->EnableWindow(bCentForce);
}
