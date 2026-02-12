// MPhiHingeChildDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiHingeChildDlg.h"

#include "MPhiHingeDlg.h"


#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_RCPSC_TYPE	0
#define D_STEEL_TYPE	1
#define D_SRC_TYPE		2

/////////////////////////////////////////////////////////////////////////////
// CMPhiHingeChildDlg dialog


CMPhiHingeChildDlg::CMPhiHingeChildDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent /*=NULL*/, int nType /*=0*/)
	: CDlgChild(CMPhiHingeChildDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CMPhiHingeDlg*) pParent;
	m_pMPhiDataAll = pMPhiDataAll;
	m_nType = nType;

	m_nCurSelectedKey = 0;
	m_bModify = FALSE;

	//{{AFX_DATA_INIT(CMPhiHingeChildDlg)
	m_bMzChk = FALSE;
	m_bMyChk = FALSE;
	m_bYbPointCbx = FALSE;
	m_nConsider = 0;
	m_bPmmChk = TRUE;
	m_pGuideDlg = NULL;
	m_nEarthquakeType = 1;
	m_nExamTarget = 0; // 0: Exist, 1: Rein
	m_nSectionConsider = T_MPHG_D::kNAs;
	m_nScrollPos = 0; // Scroll Position Init
	//}}AFX_DATA_INIT

	// set flag childHasBtn Flag....
	SetChildHasBtnFlag(TRUE);
}


void CMPhiHingeChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiHingeChildDlg)
	DDX_Control(pDX, IDC_PMM_MODEL_COBX,		m_PmmModelCobx);	
	DDX_Control(pDX, IDC_CMD_PROPERTY_LIST,		m_cPropertyListCtrl);
	DDX_Control(pDX, IDC_NO_PMM_EDT,			m_NumPmmEdit);
	DDX_Control(pDX, IDC_NO_MZ_EDT,				m_NumMzEdit);
	DDX_Control(pDX, IDC_NO_MY_EDT,				m_NumMyEdit);
	DDX_Control(pDX, IDC_CODE_COBX,				m_cCode);
	DDX_Check(pDX,   IDC_PMM_CHK,				m_bPmmChk);	
	DDX_Check(pDX,	 IDC_MZ_CHK,				m_bMzChk);
	DDX_Check(pDX,   IDC_MY_CHK,				m_bMyChk);
	DDX_Radio(pDX,   IDC_SEISMIC_A_REG_3_RDO,	m_nLimitState);	
	DDX_Radio(pDX,   IDC_KEP_STR_DOWN_RDO,		m_nStrAfterLimit);
	DDX_Radio(pDX,   IDC_CONSIDER_RDO,			m_nConsider);	
	DDX_Radio(pDX, IDC_SECTION_CONSIDER_RDO, m_nSectionConsider);
	DDX_Radio(pDX, IDC_TYPE_I_RDO, m_nEarthquakeType);
	DDX_Check(pDX, IDC_CMD_MPHI_PASS_YB_CBX, m_bYbPointCbx);
	DDX_Radio(pDX, IDC_WG_CMD_EXAM_TARGET_EXIST_RDO, m_nExamTarget);
	DDX_Control(pDX, IDC_WG_CMD_STATIC1, m_FoldGrp);

	//}}AFX_DATA_MAP
	UINT nMod[2] = { IDC_MY_MODEL_COBX, IDC_MZ_MODEL_COBX };

	for(int i=0; i<2; i++)
	{
		DDX_Control(pDX, nMod[i], m_cmbModel[i]);
	}
	DDX_Control(pDX, IDC_FIRST_BROKEN_COBX,		m_cmbFirstBrokenLine);
	DDX_Control(pDX, IDC_SECOND_BROKEN_COBX,	m_cmbSecondBrokenLine);
}


BEGIN_MESSAGE_MAP(CMPhiHingeChildDlg, CDlgChild)
	//{{AFX_MSG_MAP(CMPhiHingeChildDlg)
	ON_BN_CLICKED(IDC_ADD_REPLACE_BTN, OnAddReplaceBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_MY_CHK, OnMyChk)
	ON_BN_CLICKED(IDC_MZ_CHK, OnMzChk)
	ON_BN_CLICKED(IDC_PMM_CHK, OnPmmChk)
	ON_BN_CLICKED(IDC_RCPSC_BTN, OnRcpscBtn)
	ON_BN_CLICKED(IDC_STEEL_BTN, OnSteelBtn)
	ON_BN_CLICKED(IDC_SRC_BTN, OnSRCBtn)
	ON_BN_CLICKED(IDC_WG_CMD_MPHI_KEEP_STR_GUIDE_BTN, OnKeepStrGuideBtn)  
	ON_BN_CLICKED(IDC_KEP_STR_DOWN_RDO, OnAferLimBtn)  
	ON_BN_CLICKED(IDC_STR_ZERO_RDO, OnAferLimBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_PROPERTY_LIST, OnItemchangedMatlList)
	ON_NOTIFY(NM_CLICK, IDC_CMD_PROPERTY_LIST, OnClickMatlList)
	ON_EN_CHANGE(IDC_NO_PMM_EDT, OnChangeNoPmmEdt)
	ON_CBN_SELCHANGE(IDC_PMM_MODEL_COBX, OnSelchangePmmModelCobx)
	ON_CBN_SELCHANGE(IDC_CODE_COBX, OnSelchangeModelCobx)
	ON_BN_CLICKED(IDC_TYPE_I_RDO, OnTypeIRdo)
	ON_BN_CLICKED(IDC_CMD_MPHI_PASS_YB_CBX, OnClickYbPointCbx)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiHingeChildDlg message handlers
BOOL CMPhiHingeChildDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	m_pGuideDlg = new CMPhiSectionImageDlg();

	// 창이 완전히 그려진 후 스크롤 적용
	SetTimer(1001, 10, nullptr);

	InitListCtrl();
	InitData();

	InitCombx();
	AlignCtrl();

	if(!Data2Dlg()) return FALSE;

	EnableCtrl();	
	ShowHideCtrl();

	UpdateData(FALSE);

	m_FoldGrp.InitControl(this, CMPhiHingeChildDlg::IDD, IDC_WG_CMD_STATIC1, FALSE, FALSE);
	// initcontrol 에서 리소스 자체의 기본값 세팅을 하는데, 같은 값이면 변화가 없다고 판단하여
	// 상태 변화 메시지를 보내지 않음. 따라서 강제로 상태 변화를 줌.
	m_FoldGrp.SetFoldState(FALSE); // 접힌 상태로 시작
	m_FoldGrp.SetFoldState(TRUE); // 접힌 상태로 시작

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPhiHingeChildDlg::Init_Scroll()
{
	m_nScrollPos = 0;
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = 100;  // 컨텐츠 크기 설정
	si.nPage = 20; // 한 번에 표시되는 크기
	SetScrollInfo(SB_VERT, &si, TRUE);
	ShowScrollBar(SB_VERT, TRUE);
}

void CMPhiHingeChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1001)
	{
		KillTimer(1001);

		Init_Scroll();
	}

	CDialog::OnTimer(nIDEvent);
}

void CMPhiHingeChildDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurPos = GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_LINEUP:    nCurPos -= 20; break;
	case SB_LINEDOWN:  nCurPos += 20; break;
	case SB_PAGEUP:    nCurPos -= 20; break;
	case SB_PAGEDOWN:  nCurPos += 20; break;
	case SB_THUMBTRACK: nCurPos = nPos; break;
	}

	nCurPos = max(0, min(nCurPos, 100));
	SetScrollPos(SB_VERT, nCurPos);
	ScrollWindow(0, m_nScrollPos - nCurPos);
	m_nScrollPos = nCurPos;
}

BOOL CMPhiHingeChildDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nWheel = GetScrollPos(SB_VERT);
	if (zDelta > 0)
		nWheel -= 20;
	else if (zDelta < 0)
		nWheel += 20;

	nWheel = max(0, min(nWheel, 100));
	SetScrollPos(SB_VERT, nWheel);
	ScrollWindow(0, m_nScrollPos - nWheel);
	m_nScrollPos = nWheel;

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMPhiHingeChildDlg::SetModifyData(T_MATL_K Key)
{
	m_nCurSelectedKey = Key; 
	m_bModify = TRUE;
}

void CMPhiHingeChildDlg::InitListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_cPropertyListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_cPropertyListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;

	
	if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
	{		
		const TCHAR *list[9] = { _T("ID"),
						_LS(IDS_CMD_MPHI_NAME),
						_LS(IDS_CMD_MPHI_C_GRADE),
						_LS(IDS_CMD_MPHI_EC),
						_LS(IDS_CMD_MPHI_SIG_CK),
						_LS(IDS_CMD_MPHI_S_GRADE),
						_LS(IDS_CMD_MPHI_ES),
						_LS(IDS_CMD_MPHI_SIG_SY),
						_LS(IDS_CMD_MPHI_USE) };

		int width[9]	= {34,80,63,65,65,63,65,65,34};		
		for(int i=0; i<9; i++)
		{
			lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt      = LVCFMT_CENTER;
			lvcolumn.pszText  = (LPTSTR)list[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
			m_cPropertyListCtrl.InsertColumn(i,&lvcolumn);
		}
	}
	else
	{
		const TCHAR *list[6] = {_T("ID"),
						_LS(IDS_CMD_MPHI_NAME),
						_LS(IDS_CMD_MPHI_GRADE),
						_LS(IDS_CMD_MPHI_ES),
						_LS(IDS_CMD_MPHI_SIG_SY),
						_LS(IDS_CMD_MPHI_USE) };

		int width[6]	= {40,154,110,95,95,40};
		for(int i=0; i<6; i++)
		{
			lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt      = LVCFMT_CENTER;
			lvcolumn.pszText  = (LPTSTR)list[i];
			lvcolumn.iSubItem = i;
			lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
			m_cPropertyListCtrl.InsertColumn(i,&lvcolumn);
		}
	}
}

BOOL CMPhiHingeChildDlg::ExistMphg(T_MPHG_K MphgK)
{
	for (int i = 0; i < m_pMPhiDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pMPhiDataAll->aMphgK[i] == MphgK)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CMPhiHingeChildDlg::ShowHideCtrl()
{
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nCode); // �� ���ڰ� �ǹ��ϴ� ���� DB�� �ּ� ����

	GetDlgItem(IDC_WG_CMD_EQ_GRP)->ShowWindow(nCode == 1); 
	GetDlgItem(IDC_TYPE_I_RDO)->ShowWindow(nCode == 1); 
	GetDlgItem(IDC_TYPE_II_RDO)->ShowWindow(nCode == 1); 
	
	GetDlgItem(IDC_WG_CMD_LIMIT_STATE_STC)->ShowWindow(nCode != 1); 
	GetDlgItem(IDC_SEISMIC_A_REG_3_RDO)->ShowWindow(nCode != 1); 
	GetDlgItem(IDC_SEISMIC_B_REG_2_RDO)->ShowWindow(nCode != 1);

#if defined(_CIVIL_JP)
	GetDlgItem(IDC_WG_CMD_STATIC7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WG_CMD_STATIC8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RCPSC_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WG_CMD_STATIC9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STEEL_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WG_CMD_STATIC10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SRC_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SECTION_CONSIDER_GRP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SECTION_CONSIDER_RDO)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SECTION_DONT_CONSIDER_RDO)->ShowWindow(SW_SHOW);
	if (m_nType == D_RCPSC_TYPE) GetDlgItem(IDC_CMD_MPHI_PASS_YB_CBX)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_CMD_MPHI_PASS_YB_CBX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SECTION_CONSIDER_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SECTION_CONSIDER_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SECTION_DONT_CONSIDER_RDO)->ShowWindow(SW_HIDE);
#endif
}

void CMPhiHingeChildDlg::AlignCtrl()
{
	CRect rectRef, rectMove;
	int nYDist;
	
	CArray<UINT,UINT> aCtrl;		
	aCtrl.Add(IDC_WG_CMD_EQ_GRP);
	aCtrl.Add(IDC_TYPE_I_RDO);
	aCtrl.Add(IDC_TYPE_II_RDO);
	
	GetDlgItem(IDC_WG_CMD_LIMIT_STATE_STC)->GetWindowRect(rectRef);
	GetDlgItem(IDC_WG_CMD_EQ_GRP)->GetWindowRect(rectMove);
	nYDist = rectRef.top - rectMove.top;

	CDlgUtil::CtrlMoveDistY(this, aCtrl, nYDist);

	// JP 
#if defined(_CIVIL_JP)

	int nCloseMatrginY = globalUtils.ScaleByDPI(4);

	m_aExamTargetCtrl.RemoveAll();
	m_aExamTargetCtrl.Add(IDC_WG_CMD_EXAM_TARGET_GRP);
	m_aExamTargetCtrl.Add(IDC_WG_CMD_EXAM_TARGET_EXIST_RDO);
	m_aExamTargetCtrl.Add(IDC_WG_CMD_EXAM_TARGET_REIN_RDO);

	CArray<UINT, UINT> aOtherAllCtrl, aOtherAllCtrl2;
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC3);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC4);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC5);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC6);
	aOtherAllCtrl.Add(IDC_PMM_CHK);
	aOtherAllCtrl.Add(IDC_NO_PMM_EDT);
	aOtherAllCtrl.Add(IDC_PMM_MODEL_COBX);
	aOtherAllCtrl.Add(IDC_FIRST_BROKEN_STC);
	aOtherAllCtrl.Add(IDC_FIRST_BROKEN_COBX);
	aOtherAllCtrl.Add(IDC_SECOND_BROKEN_STC);
	aOtherAllCtrl.Add(IDC_SECOND_BROKEN_COBX);
	aOtherAllCtrl.Add(IDC_MY_CHK);
	aOtherAllCtrl.Add(IDC_NO_MY_EDT);
	aOtherAllCtrl.Add(IDC_MY_MODEL_COBX);
	aOtherAllCtrl.Add(IDC_MZ_CHK);
	aOtherAllCtrl.Add(IDC_NO_MZ_EDT);
	aOtherAllCtrl.Add(IDC_MZ_MODEL_COBX);
	aOtherAllCtrl.Add(IDC_CMD_MPHI_PASS_YB_CBX);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC1);
	aOtherAllCtrl2.Add(IDC_WG_CMD_LIMIT_STATE_STC);
	aOtherAllCtrl2.Add(IDC_SEISMIC_A_REG_3_RDO);
	aOtherAllCtrl2.Add(IDC_SEISMIC_B_REG_2_RDO);
	aOtherAllCtrl2.Add(IDC_WG_CMD_STR_AFTER_LIMIT);
	aOtherAllCtrl2.Add(IDC_KEP_STR_DOWN_RDO);
	aOtherAllCtrl2.Add(IDC_WG_CMD_MPHI_KEEP_STR_GUIDE_BTN);
	aOtherAllCtrl2.Add(IDC_STR_ZERO_RDO);
	aOtherAllCtrl2.Add(IDC_WG_CMD_STATIC0);
	aOtherAllCtrl2.Add(IDC_SECTION_CONSIDER_GRP);
	aOtherAllCtrl2.Add(IDC_SECTION_CONSIDER_RDO);
	aOtherAllCtrl2.Add(IDC_SECTION_DONT_CONSIDER_RDO);
	aOtherAllCtrl2.Add(IDC_BT_CONSIDER_GRP);
	aOtherAllCtrl2.Add(IDC_CONSIDER_RDO);
	aOtherAllCtrl2.Add(IDC_DONT_CONSIDER_RDO);

	m_aExamTypeCtrl.RemoveAll();
	m_aExamTypeCtrl.Add(IDC_WG_CMD_EQ_GRP);
	m_aExamTypeCtrl.Add(IDC_TYPE_I_RDO);
	m_aExamTypeCtrl.Add(IDC_TYPE_II_RDO);

	m_aBtnCtrl.RemoveAll();
	m_aBtnCtrl.Add(IDC_ADD_REPLACE_BTN);
	m_aBtnCtrl.Add(IDC_DEL_BTN);

	m_aFoldGrpResource.RemoveAll();
	m_aFoldGrpResource.Append(aOtherAllCtrl2);

	if (m_nType != D_RCPSC_TYPE) return;

	CRect rectExamTarget;
	GetDlgItem(IDC_WG_CMD_CODE_STC)->GetWindowRect(rectRef);
	GetDlgItem(IDC_WG_CMD_EXAM_TARGET_GRP)->GetWindowRect(rectExamTarget);
	nYDist = rectRef.bottom - rectExamTarget.top - nCloseMatrginY;
	CDlgUtil::CtrlMoveDistY(this, m_aExamTargetCtrl, nYDist);

	nYDist = rectExamTarget.Height() - nCloseMatrginY * 2;
	CDlgUtil::CtrlMoveDistY(this, aOtherAllCtrl, nYDist);
	CDlgUtil::CtrlMoveDistY(this, aOtherAllCtrl2, nYDist);
	CDlgUtil::CtrlMoveDistY(this, m_aExamTypeCtrl, nYDist);
	nYDist = nCloseMatrginY * 4;
	CDlgUtil::CtrlMoveDistY(this, m_aBtnCtrl, nYDist);
#else
	int nCloseMatrginY = globalUtils.ScaleByDPI(4);
	CArray<UINT, UINT> aBtConsiderCtrl;
	aBtConsiderCtrl.Add(IDC_BT_CONSIDER_GRP);
	aBtConsiderCtrl.Add(IDC_CONSIDER_RDO);
	aBtConsiderCtrl.Add(IDC_DONT_CONSIDER_RDO);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(rectRef);
	GetDlgItem(IDC_SECTION_CONSIDER_GRP)->GetWindowRect(rectMove);
	nYDist = rectRef.top - rectMove.bottom + nCloseMatrginY * 4;

	CDlgUtil::CtrlMoveDistY(this, aBtConsiderCtrl, nYDist);
	CDlgUtil::CtrlResizeBottom(this, IDC_WG_CMD_STATIC0, nYDist);
	CDlgUtil::CtrlResizeBottom(this, IDC_WG_CMD_STATIC1, nYDist);

	CArray<UINT, UINT> aOtherAllCtrl;
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC7);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC8);
	aOtherAllCtrl.Add(IDC_RCPSC_BTN);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC9);
	aOtherAllCtrl.Add(IDC_STEEL_BTN);
	aOtherAllCtrl.Add(IDC_WG_CMD_STATIC10);
	aOtherAllCtrl.Add(IDC_SRC_BTN);
	aOtherAllCtrl.Add(IDC_ADD_REPLACE_BTN);
	aOtherAllCtrl.Add(IDC_DEL_BTN);
	CDlgUtil::CtrlMoveDistY(this, aOtherAllCtrl, nYDist);
#endif
}

void CMPhiHingeChildDlg::InitData()
{
	m_cPropertyListCtrl.DeleteAllItems();
	CArray<T_MATL_K,T_MATL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(KeyList);
	int iCount = KeyList.GetSize();

	int ListLineNo = 0;
	for(int i=0; i<iCount; i++)
	{
		T_MATD_D mData;
		mData.Initialize();
		int nID = KeyList.GetAt(i);
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
		if(bCheck)
		{
			if(m_nType==D_RCPSC_TYPE && mData.Type == _T("C"))
			{
				ListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data1.CodeMatlName;
				data.dElast = mData.Data1.Analysis.Elast;
				data.dSig = mData.Data1.Design.C_fc;
				data.cSGradeName = mData.MainRebar_RebarName;
				data.dSElast = mData.MainRebarData.B_Elast;
				// 20080514 mylee - MPhi �̰� Rebar Code�� None ���ý� Preference �ڵ带 �����Ͽ� �����Ѵ�.
				// ���� ������ �ٸ� ������ �ǰ� ������, ���⼭�� ����Ʈ�� Es ���� �����ֱ� ���ؼ� ó��. (������)
				if(mData.Rebar_CodeName==_T("None"))
					GetEsFromPreference(data.dSElast);
				data.dSigSy = mData.MainRebarData.B_fy;								
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
			else if(m_nType==D_STEEL_TYPE && mData.Type == _T("S"))
			{
				ListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data1.CodeMatlName;
				data.dElast = mData.Data1.Analysis.Elast;
				data.dSig = mData.Data1.Design.S_Fy1;			
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
			else if(m_nType==D_SRC_TYPE && mData.Type == _T("SRC"))
			{
				ListData data; data.initialize();
				data.nId = nID;
				data.cName = mData.Name;
				data.cGradeName = mData.Data2.CodeMatlName;
				data.dElast = mData.Data2.Analysis.Elast;
				data.dSig = mData.Data2.Design.C_fc;
				data.cSGradeName = mData.Data1.CodeMatlName;
				data.dSElast = mData.Data1.Analysis.Elast;
				data.dSigSy = mData.Data1.Design.S_Fy1;								
				data.bUse = ExistMphg(nID);
				SetListCtrl(ListLineNo, data);				        
				ListLineNo++;
			}
		}
	}
}

LRESULT CMPhiHingeChildDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	ShowHildeForFold();

	m_pParent->Invalidate();
	m_pParent->UpdateWindow();
	return 0L;
}

void CMPhiHingeChildDlg::ShowHildeForFold()
{
	if (m_aBtnCtrl.GetSize() == 0) return;
	int nCloseMatrginY = globalUtils.ScaleByDPI(8);
	CRect rectRef, rectBtn;
	GetDlgItem(IDC_WG_CMD_STATIC1)->GetWindowRect(rectRef);
	GetDlgItem(m_aBtnCtrl[0])->GetWindowRect(rectBtn);
	double nYDist = rectRef.bottom - rectBtn.top + nCloseMatrginY;
	CDlgUtil::CtrlMoveDistY(this, m_aBtnCtrl, nYDist);

	if (m_FoldGrp.GetFoldState() == FALSE)
	{
		// 펼치기
		ShowHideCtrl();
	}
	else
	{
		// 접기
		if (m_nType == D_RCPSC_TYPE)
		{
			CDlgUtil::CtrlShowHide(this, m_aExamTargetCtrl, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aExamTypeCtrl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aFoldGrpResource, FALSE);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aExamTargetCtrl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aExamTypeCtrl, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aFoldGrpResource, FALSE);
		}
	}
	UpdateData(FALSE);
}

// Main Rebar Code �� None �϶��� �Ҹ��� �Լ�
void CMPhiHingeChildDlg::GetEsFromPreference(double& dEs)
{
	// 1. Preference�� ������ Default ö�ٱ��ذ� ���������� �޾ƿɴϴ�.
	CString strRebarCode=_T(""), strRebarName=_T("");
	CDBLib::GetDefaultConRbar(strRebarCode, strRebarName);

	// 2. ö�ٱ��ذ� ���������� �̿�(1���� �޾ƿ�)�Ͽ� ö�������� �޾ƿɴϴ�.
	T_MATL_REBAR RbarD;
	if(!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,RbarD))          RbarD.Initialize();
	
	// 3. ������ ���������� ö�������� �̿��Ͽ� ��ö�ٰ����� ��ȯ�մϴ�.
	dEs = RbarD.B_Elast;
}

void CMPhiHingeChildDlg::SetListCtrl(int ListLineNo, ListData data)
{
	LV_ITEM lvitem;
	if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
	{
		LPTSTR szText[9];
		CString tempS;
		tempS.Format(_T("%5d"), data.nId);
		szText[0] = (LPTSTR)(LPCTSTR)tempS;
		szText[1] = (LPTSTR)(LPCTSTR)data.cName;
		szText[2] = (LPTSTR)(LPCTSTR)data.cGradeName;
		CString tempS2;
		tempS2.Format(_T("%.2e"), data.dElast);
		szText[3] = (LPTSTR)(LPCTSTR)tempS2;
		CString tempS3;
		tempS3.Format(_T("%.2e"), data.dSig);
		szText[4] = (LPTSTR)(LPCTSTR)tempS3;
		szText[5] = (LPTSTR)(LPCTSTR)data.cSGradeName;
		CString tempS4;
		tempS4.Format(_T("%.2e"), data.dSElast);
		szText[6] = (LPTSTR)(LPCTSTR)tempS4;
		CString tempS5;
		tempS5.Format(_T("%.2e"), data.dSigSy);
		szText[7] = (LPTSTR)(LPCTSTR)tempS5;
		CString tempS6;
		tempS6 = data.bUse ? _T("O") : _T("X");
		szText[8] = (LPTSTR)(LPCTSTR)tempS6;
		// Input ListCtrl Box.		
		for(int i=0; i<9; i++)
		{				
			lvitem.iItem  	= ListLineNo;
			lvitem.iSubItem = i;
			lvitem.pszText	= szText[i];			
			lvitem.mask		  = LVIF_TEXT;
			if( i == 0 ) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)data.nId;
				m_cPropertyListCtrl.InsertItem(&lvitem);
			}
			else m_cPropertyListCtrl.SetItem(&lvitem);
		}
	}
	else if(m_nType==D_STEEL_TYPE)
	{
		LPTSTR szText[6];
		CString tempS;
		tempS.Format(_T("%5d"), data.nId);
		szText[0] = (LPTSTR)(LPCTSTR)tempS;
		szText[1] = (LPTSTR)(LPCTSTR)data.cName;
		szText[2] = (LPTSTR)(LPCTSTR)data.cGradeName;
		CString tempS2;
		tempS2.Format(_T("%.2e"), data.dElast);
		szText[3] = (LPTSTR)(LPCTSTR)tempS2;
		CString tempS3;
		tempS3.Format(_T("%.2e"), data.dSig);
		szText[4] = (LPTSTR)(LPCTSTR)tempS3;		
		CString tempS4;
		tempS4 = data.bUse ? _T("O") : _T("X");
		szText[5] = (LPTSTR)(LPCTSTR)tempS4;
		// Input ListCtrl Box.		
		for(int i=0; i<6; i++)
		{			
			lvitem.iItem  	= ListLineNo;
			lvitem.iSubItem = i;
			lvitem.pszText	= szText[i];
			lvitem.mask		  = LVIF_TEXT;			
			if( i == 0 ) 
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)data.nId;
				m_cPropertyListCtrl.InsertItem(&lvitem);
			}
			else m_cPropertyListCtrl.SetItem(&lvitem);
		}
	}
}

void CMPhiHingeChildDlg::OnClickMatlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchangedMatlList(pNMHDR,pResult);
	*pResult = 0;
}

void CMPhiHingeChildDlg::OnItemchangedMatlList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nItem = pNMListView->iItem;
	CString strID = m_cPropertyListCtrl.GetItemText(nItem,0);	
	if(strID != _T(""))
		m_nCurSelectedKey = _ttoi(strID);		
		
	if(!Data2Dlg()) return;		
	EnableCtrl();	
	ShowHildeForFold();

	*pResult = 0;
}

void CMPhiHingeChildDlg::InitCombx()
{
	//UpdateData(TRUE);
	int nIndex = 0;
		
	// Inelastic Hinge Property.........
	m_PmmModelCobx.ResetContent();
	nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_KINEMA));
	m_PmmModelCobx.SetItemData(nIndex, D_IEHP_KINE);

	nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_TAKEDA));
	m_PmmModelCobx.SetItemData(nIndex, D_IEHP_TAKE);

	nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_TAKTET));
	m_PmmModelCobx.SetItemData(nIndex, D_IEHP_TTET);

	nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));
	m_PmmModelCobx.SetItemData(nIndex, D_IEHP_MTAK);

	nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_MTATET));
	m_PmmModelCobx.SetItemData(nIndex, D_IEHP_MTTE);

	//m_PmmModelCobx.SetCurSel(0);
	for(int i=0; i<2; i++)
	{
		m_cmbModel[i].ResetContent();
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_KINEMA));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_KINE);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ORIGIN));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_ORIG);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PEAK));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_PICK);

		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_CLOUGH));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_CLOU);

		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DEGRAD));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_DEGR);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKEDA));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_TAKE);    

		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKTET));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_TTET);

		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTAK);
	
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MTATET));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTTE);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NORBIL));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_NBIL);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELABIL));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_EBIL);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATRI));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETRI);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATET));      
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETET);

		// SLIP Type �̷¸��� (6.9.0 �߰�)...
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPB);		
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBT);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBC);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPT);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTT);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP));      
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTC);

		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SRCTET));
		m_cmbModel[i].SetItemData(nIndex, D_IEHP_SRCT);

		// �̷� ���� �߰� �ÿ��� �������� �߰� �Ѵ�....
	}

	m_cmbFirstBrokenLine.ResetContent();
	if(m_nType==D_RCPSC_TYPE)
	{
		m_cmbFirstBrokenLine.AddString(_LS(IDS_CMD_MPHI_CRACK));
		m_cmbFirstBrokenLine.AddString(_LS(IDS_CMD_MPHI_Yield));
		m_cmbFirstBrokenLine.SetCurSel(0);

		m_cmbSecondBrokenLine.ResetContent();
		m_cmbSecondBrokenLine.AddString(_LS(IDS_CMD_MPHI_Yield));
		m_cmbSecondBrokenLine.AddString(_LS(IDS_CMD_MPHI_ULTIMATE));
		m_cmbSecondBrokenLine.SetCurSel(0);
	}
	else
	{
		m_cmbFirstBrokenLine.AddString(_LS(IDS_CMD_MPHI_Compression_Yield));
		m_cmbFirstBrokenLine.AddString(_LS(IDS_CMD_MPHI_Tension_Yield));
		m_cmbFirstBrokenLine.SetCurSel(1);

		m_cmbSecondBrokenLine.ResetContent();
		m_cmbSecondBrokenLine.AddString(_T(""));
		m_cmbSecondBrokenLine.AddString(_T(""));
		m_cmbSecondBrokenLine.SetCurSel(0);
	}

	// Code Cbx
	m_cCode.ResetContent();
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_CONC_CODE_BOOK));
	m_cCode.SetItemData(nIndex, 0);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_V));
	m_cCode.SetItemData(nIndex, 1);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_III_SIG_CK));
	m_cCode.SetItemData(nIndex, 2);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_III_85_SIG_CK));
	m_cCode.SetItemData(nIndex, 3);  
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_V24_DEFAULT)); // ������ ������ �����Ǵ� ���� �ٸ�! ���� �ϱ� �ٶ�..!!
	m_cCode.SetItemData(nIndex, 4);
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_IV_14_24_PIPE));
	m_cCode.SetItemData(nIndex, 5);
#if defined(_CIVIL_JP)
	if (m_nType == D_RCPSC_TYPE)
	{
		nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_RAILWAY_STRUCT_H24));
		m_cCode.SetItemData(nIndex, 6);
		nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_III_85_SIG_CK_H29));
		m_cCode.SetItemData(nIndex, 7);
	}
#endif
	m_cCode.SetCurSel(4);
	AdjustComboListBox(m_cCode);
}


void CMPhiHingeChildDlg::AdjustComboListBox(CComboBox& Cbx)
	/*
	�޺��ڽ��� ���̸� ���� �����ǰ� �ִ� ����Ʈ�� ���̿� 
	���߾� �����Ѵ�.
	*/
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = Cbx.GetDC();
	for (int i=0;i < Cbx.GetCount();i++)
	{
		Cbx.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}

void CMPhiHingeChildDlg::OnAddReplaceBtn() 
{
	T_MATL_D MatlData;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(m_nCurSelectedKey, MatlData))
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	} 

	if(!Apply()) return;
	InitData();

	SetFocus();
	RedrawWindow();
}

void CMPhiHingeChildDlg::OnDelBtn() 
{
	T_MATL_D MatlData;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(m_nCurSelectedKey, MatlData))
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	} 

	ASSERT(m_pMPhiDataAll->aMphgK.GetSize() == m_pMPhiDataAll->aMphgD.GetSize());
	int nFindIdx = -1;
	for (int i = 0; i < m_pMPhiDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pMPhiDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}
	if(nFindIdx == -1)
	{
		ASSERT(0); // ���� ������..
		return ; 
	}
	m_pMPhiDataAll->aMphgK.RemoveAt(nFindIdx);
	//if(!m_pDoc->m_pDataCtrl->DelMphg(m_nCurSelectedKey)) return ;
	InitData();

	SetFocus();
	RedrawWindow();
}

void CMPhiHingeChildDlg::OnMyChk() 
{	
	UpdateData();
	EnableCtrl();	
	ShowHildeForFold();
}

void CMPhiHingeChildDlg::OnMzChk() 
{
	UpdateData();	
	EnableCtrl();	
	ShowHildeForFold();
}

void CMPhiHingeChildDlg::OnPmmChk() 
{
	UpdateData();	  
	SetPmmChk();	
	EnableCtrl();
	ShowHildeForFold();
}

void CMPhiHingeChildDlg::SetPmmChk() 
{ 	
	if(m_bPmmChk)
	{
		m_bMzChk = m_bMyChk = TRUE;
		((CButton*)GetDlgItem(IDC_MY_CHK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_MZ_CHK))->SetCheck(TRUE);

		CString selectedText;
		int nPmm = m_PmmModelCobx.GetCurSel();
		if(nPmm != -1 ) m_PmmModelCobx.GetLBText(nPmm, selectedText);

		int index = m_cmbModel[0].FindStringExact(-1, selectedText);

		m_cmbModel[0].SetCurSel(index);
		m_cmbModel[1].SetCurSel(index);

		OnChangeNoPmmEdt();
	}

	UpdateData(FALSE);	
}

void CMPhiHingeChildDlg::OnRcpscBtn() 
{
	// TODO: Add your control notification handler code here
	 // RC menu�� ȣ��(PSC���� PSC Bridge Option�� �������� ������ Disable��)
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_MATD, 0)); 
}

void CMPhiHingeChildDlg::OnSteelBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_STL_MATD, 0));	
}

void CMPhiHingeChildDlg::OnSRCBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_SRC_MATD, 0));	
}

void CMPhiHingeChildDlg::OnAferLimBtn() 
{
	EnableCtrl();
	ShowHildeForFold();
}

void CMPhiHingeChildDlg::OnKeepStrGuideBtn() 
{
	CRect Rect;
	GetWindowRect(Rect);
	m_pGuideDlg->CreateInit(this, Rect.right, Rect.top);
}

void CMPhiHingeChildDlg::OnOK()
{
	return;
}

void CMPhiHingeChildDlg::OnCancel() 
{
	return;
}

BOOL CMPhiHingeChildDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_cPropertyListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CMPhiHingeChildDlg::OnChangeNoPmmEdt() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDlgChild::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if(m_bPmmChk)
	{
		CString tempS;
		m_NumPmmEdit.GetWindowText(tempS);
		m_NumMzEdit.SetWindowText(tempS);
		m_NumMyEdit.SetWindowText(tempS);
	}
	UpdateData(FALSE);
}

void CMPhiHingeChildDlg::OnSelchangePmmModelCobx() 
{
	UpdateData();
	if(m_bPmmChk)
	{
		CString selectedText;
		m_PmmModelCobx.GetLBText(m_PmmModelCobx.GetCurSel(), selectedText);

		int index = m_cmbModel[0].FindStringExact(-1, selectedText);

		m_cmbModel[0].SetCurSel(index);
		m_cmbModel[1].SetCurSel(index);

		EnableCtrl();
	}
	UpdateData(FALSE);
}

void CMPhiHingeChildDlg::OnSelchangeStressStrainCobx() 
{
	EnableCtrl();
	ShowHildeForFold();
}


void CMPhiHingeChildDlg::EnableCtrl()
{
	UpdateData();
	
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nCode);

	GetDlgItem(IDC_CONSIDER_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 0 || nCode == 1 || nCode == 4));
	GetDlgItem(IDC_DONT_CONSIDER_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 0 || nCode == 1 || nCode == 4));

	GetDlgItem(IDC_RCPSC_BTN)->EnableWindow(m_nType==D_RCPSC_TYPE);
	GetDlgItem(IDC_STEEL_BTN)->EnableWindow(m_nType==D_STEEL_TYPE);
	GetDlgItem(IDC_SRC_BTN)->EnableWindow(m_nType==D_SRC_TYPE);
	
	GetDlgItem(IDC_MY_MODEL_COBX)->EnableWindow(m_bMyChk && !m_bPmmChk);
	GetDlgItem(IDC_NO_MY_EDT)->EnableWindow(m_bMyChk && !m_bPmmChk);

	GetDlgItem(IDC_MZ_MODEL_COBX)->EnableWindow(m_bMzChk && !m_bPmmChk);
	GetDlgItem(IDC_NO_MZ_EDT)->EnableWindow(m_bMzChk && !m_bPmmChk);

	GetDlgItem(IDC_PMM_MODEL_COBX)->EnableWindow(m_bPmmChk);
	GetDlgItem(IDC_NO_PMM_EDT)->EnableWindow(m_bPmmChk);

	GetDlgItem(IDC_MY_CHK)->EnableWindow(!m_bPmmChk);
	GetDlgItem(IDC_MZ_CHK)->EnableWindow(!m_bPmmChk);  

	BOOL bHas3rd = FALSE;
	if (m_bPmmChk)
	{
		int nIndex = m_PmmModelCobx.GetCurSel();
		int nHysModel = m_PmmModelCobx.GetItemData(nIndex);
		if (nHysModel == D_IEHP_MTTE || nHysModel == D_IEHP_TTET)
		{
			bHas3rd = TRUE;
		}
	}

	GetDlgItem(IDC_FIRST_BROKEN_STC)->EnableWindow(m_bPmmChk && !bHas3rd);
	GetDlgItem(IDC_FIRST_BROKEN_COBX)->EnableWindow(m_bPmmChk && !bHas3rd);
	GetDlgItem(IDC_SECOND_BROKEN_STC)->EnableWindow(m_nType==D_RCPSC_TYPE && m_bPmmChk && !bHas3rd);
	GetDlgItem(IDC_SECOND_BROKEN_COBX)->EnableWindow(m_nType==D_RCPSC_TYPE && m_bPmmChk && !bHas3rd);
	
	GetDlgItem(IDC_WG_CMD_LIMIT_STATE_STC)->EnableWindow(m_nType==D_RCPSC_TYPE && nCode == 4); 
	GetDlgItem(IDC_SEISMIC_A_REG_3_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && nCode == 4); 
	GetDlgItem(IDC_SEISMIC_B_REG_2_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && nCode == 4); 

	GetDlgItem(IDC_WG_CMD_STR_AFTER_LIMIT)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 1 || nCode == 4));
	GetDlgItem(IDC_KEP_STR_DOWN_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 1 || nCode == 4));  
	GetDlgItem(IDC_WG_CMD_MPHI_KEEP_STR_GUIDE_BTN)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 1 || nCode == 4) && m_nStrAfterLimit == 0);  
	GetDlgItem(IDC_STR_ZERO_RDO)->EnableWindow(m_nType==D_RCPSC_TYPE && (nCode == 1 || nCode == 4));  

#if defined(_CIVIL_JP)
	if (m_nType == D_RCPSC_TYPE)
	{
		GetDlgItem(IDC_WG_CMD_EXAM_TARGET_EXIST_RDO)->EnableWindow(nCode == 1);
		GetDlgItem(IDC_WG_CMD_EXAM_TARGET_REIN_RDO)->EnableWindow(nCode == 1);

		// ö�� ������ ����ǥ�� 
		GetDlgItem(IDC_CMD_MPHI_PASS_YB_CBX)->EnableWindow(nCode == 6);
		GetDlgItem(IDC_SECTION_CONSIDER_RDO)->EnableWindow(nCode != 5);
		GetDlgItem(IDC_SECTION_DONT_CONSIDER_RDO)->EnableWindow(nCode != 5);
	}
	else
	{
		GetDlgItem(IDC_WG_CMD_EXAM_TARGET_EXIST_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_WG_CMD_EXAM_TARGET_REIN_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_MPHI_PASS_YB_CBX)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECTION_CONSIDER_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECTION_DONT_CONSIDER_RDO)->EnableWindow(FALSE);
	}
#else
	GetDlgItem(IDC_WG_CMD_EXAM_TARGET_EXIST_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_WG_CMD_EXAM_TARGET_REIN_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_MPHI_PASS_YB_CBX)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECTION_CONSIDER_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECTION_DONT_CONSIDER_RDO)->EnableWindow(FALSE);
#endif
}

/////////////////////////////////////////////////////////////////////
// Data Control.....
BOOL CMPhiHingeChildDlg::Apply()
{
	if(!Dlg2Data()) return FALSE;
	if(!m_pDoc->m_pEditData->CheckMphg(m_nCurSelectedKey, m_Data)) return FALSE;

	// �ϴ� CMPhiParameterDlg::m_pMPhiDataAll�� �׾� ����
	ASSERT(m_pMPhiDataAll->aMphgK.GetSize() == m_pMPhiDataAll->aMphgD.GetSize());
	int nFindIdx = -1;
	for (int i = 0; i < m_pMPhiDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pMPhiDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}
	if(nFindIdx == -1)
	{
		m_pMPhiDataAll->aMphgK.Add(m_nCurSelectedKey);
		m_pMPhiDataAll->aMphgD.Add(m_Data);
	}
	else
	{
		ASSERT(m_pMPhiDataAll->aMphgK[nFindIdx] == m_nCurSelectedKey);    
		m_pMPhiDataAll->aMphgD[nFindIdx] = m_Data;
	}

	T_MATD_D MatdD;
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(m_nCurSelectedKey,MatdD);
	ModifyItem(m_nCurSelectedKey, m_nCurSelectedKey, MatdD);
	//
	//if(!m_pDoc->m_pDataCtrl->AddMphg(m_nCurSelectedKey, m_Data)) return FALSE;

	return TRUE;
}


BOOL CMPhiHingeChildDlg::Data2Dlg()
{		
	int nFindIdx = -1;
	for (int i = 0; i < m_pMPhiDataAll->aMphgK.GetSize(); i++)
	{
		if(m_pMPhiDataAll->aMphgK[i] == m_nCurSelectedKey)
		{
			nFindIdx = i;
			break;
		}
	}

	if(nFindIdx == -1)
	{
		m_Data.Initialize();
		if(m_nType!=D_RCPSC_TYPE)
		{
			m_Data.nBrokenLine1 = 1; // Steel, SRC�� ���� �⺻���� �ٸ�
		}
	}
	else
	{
		m_Data = m_pMPhiDataAll->aMphgD[nFindIdx];
	}
	
	m_bPmmChk = (m_Data.cDir[0]=='0')?0:1;
	m_bMyChk  = (m_Data.cDir[1]=='0')?0:1;
	m_bMzChk  = (m_Data.cDir[2]=='0')?0:1;
	// JP
	m_bYbPointCbx = m_Data.bPassYbPoint;
	m_nExamTarget = m_Data.nExamTarget;
	m_nSectionConsider = m_Data.nSectionConsider;

	// change conbo box control..
	SetPmmChk();

	CString tempS;
	tempS.Format(_T("%d"), m_Data.nSectionNum[0]);
	m_NumPmmEdit.SetWindowText(tempS);
	tempS.Format(_T("%d"), m_Data.nSectionNum[1]);
	m_NumMyEdit.SetWindowText(tempS);
	tempS.Format(_T("%d"), m_Data.nSectionNum[2]);
	m_NumMzEdit.SetWindowText(tempS);

	m_PmmModelCobx.SelectString(0, GetHysModelName(m_Data.nHysModel[0]));
	for(int i=0; i<2 ; i++)
	{			
		m_cmbModel[i].SelectString(0, GetHysModelName(m_Data.nHysModel[i+1]));
	}

	m_cmbFirstBrokenLine.SetCurSel(m_Data.nBrokenLine1);
	m_cmbSecondBrokenLine.SetCurSel(m_Data.nBrokenLine2);

	m_nConsider = m_Data.nConsiderSigbt;
	
	CDlgUtil::CobxSetCurSelItemData(m_cCode, m_Data.nCode);

	m_nLimitState = m_Data.nLimitState;	
	m_nStrAfterLimit = m_Data.nStrenthLimit;
	m_nEarthquakeType = m_Data.nEarthquakeType;

	UpdateData(FALSE);
	OnSelchangeStressStrainCobx();
#if defined(_CIVIL_JP)
	if (m_nType == D_RCPSC_TYPE) ResetPmmMyMzComboBox();
#endif

	if(m_bModify) 
	{
		CDlgUtil::SelectListItemData(&m_cPropertyListCtrl, m_nCurSelectedKey);
		m_bModify = FALSE;
	}

	return TRUE;
}

BOOL CMPhiHingeChildDlg::Dlg2Data()
{
	UpdateData();
	m_Data.Initialize();

	m_Data.cDir[0] = (m_bPmmChk==0)?'0':'1';
	m_Data.cDir[1] = (m_bMyChk==0)?'0':'1';
	m_Data.cDir[2] = (m_bMzChk==0)?'0':'1';
	m_Data.bPassYbPoint = m_bYbPointCbx;
	m_Data.nSectionConsider = m_nSectionConsider;

	CFormulaEdit::GetEditValue(&m_NumPmmEdit, m_Data.nSectionNum[0]);
	CFormulaEdit::GetEditValue(&m_NumMyEdit, m_Data.nSectionNum[1]);
	CFormulaEdit::GetEditValue(&m_NumMzEdit, m_Data.nSectionNum[2]);

	int nIndex = m_PmmModelCobx.GetCurSel();
	m_Data.nHysModel[0] = m_PmmModelCobx.GetItemData(nIndex);
	for(int i=0; i<2 ; i++)
	{	
		nIndex = m_cmbModel[i].GetCurSel();
		m_Data.nHysModel[i+1] = m_cmbModel[i].GetItemData(nIndex);
	}

	DWORD nItemData;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nItemData);
	m_Data.nCode = nItemData;

	m_Data.nBrokenLine1 = m_cmbFirstBrokenLine.GetCurSel();
	m_Data.nBrokenLine2 = m_cmbSecondBrokenLine.GetCurSel();

	m_Data.nConsiderSigbt = m_nConsider;
	m_Data.nLimitState = m_nLimitState;	
	m_Data.nStrenthLimit = m_nStrAfterLimit;

	m_Data.nEarthquakeType = m_nEarthquakeType;
	m_Data.nExamTarget =  m_nExamTarget;
	return TRUE;
}

CString CMPhiHingeChildDlg::GetHysModelName(int nHysModel)
{
	CString strModel = _T("");
	switch(nHysModel)
	{
	case D_IEHP_KINE: strModel = _LS(IDS_CMD_IEHP_KINEMA);       break;
	case D_IEHP_ORIG: strModel = _LS(IDS_CMD_IEHP_ORIGIN);       break;
	case D_IEHP_PICK: strModel = _LS(IDS_CMD_IEHP_PEAK);         break;
	case D_IEHP_CLOU: strModel = _LS(IDS_CMD_IEHP_CLOUGH);       break;
	case D_IEHP_DEGR: strModel = _LS(IDS_CMD_IEHP_DEGRAD);       break;
	case D_IEHP_TAKE: strModel = _LS(IDS_CMD_IEHP_TAKEDA);       break;
	case D_IEHP_MTAK: strModel = _LS(IDS_CMD_IEHP_MOD_TAKETA);   break;
	case D_IEHP_NELS: strModel = _LS(IDS_CMD_IEHP_NONL_ELA_SYM); break;
	case D_IEHP_ASYM: strModel = _LS(IDS_CMD_IEHP_NONL_INV_DIR); break;
	case D_IEHP_NELA: strModel = _LS(IDS_CMD_IEHP_NONL_ELA_ASY); break;
	case D_IEHP_NBIL: strModel = _LS(IDS_CMD_IEHP_NORBIL);       break;
	case D_IEHP_EBIL: strModel = _LS(IDS_CMD_IEHP_ELABIL);       break;
	case D_IEHP_ETRI: strModel = _LS(IDS_CMD_IEHP_ELATRI);       break;
	case D_IEHP_ETET: strModel = _LS(IDS_CMD_IEHP_ELATET);       break;
	case D_IEHP_TTET: strModel = _LS(IDS_CMD_IEHP_TAKTET);       break;
	case D_IEHP_MTTE: strModel = _LS(IDS_CMD_IEHP_MTATET);       break;
	case D_IEHP_LRBB: strModel = _LS(IDS_CMD_IEHP_LRBBIL);       break;
	case D_IEHP_LRBT: strModel = _LS(IDS_CMD_IEHP_LRBTRI);       break;
	case D_IEHP_DMPR: strModel = _LS(IDS_CMD_IEHP_DMPRBR);       break;
	case D_IEHP_LRBH16: strModel = _LS(IDS_CMD_IEHP_LRBH16);       break;
	case D_IEHP_LRBH30: strModel = _LS(IDS_CMD_IEHP_LRBH30);       break;
	case D_IEHP_DMPRH16: strModel = _LS(IDS_CMD_IEHP_DMPRH16);       break;
	case D_IEHP_DMPRH30: strModel = _LS(IDS_CMD_IEHP_DMPRH30);       break;
	// SLIP Type �߰�...(6.9.0)
	case D_IEHP_SLPB: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR);				break;
	case D_IEHP_SLBT: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS);   break;
	case D_IEHP_SLBC: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP);   break;
	case D_IEHP_SLPT: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR);       break;
	case D_IEHP_SLTT: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS);  break;
	case D_IEHP_SLTC: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP);  break;

	case D_IEHP_SRCT: strModel = _LS(IDS_CMD_IEHP_SRCTET);       break;
	default: ASSERT(0); break;
	}

	return strModel;
}

/////////////////////////////////////////
// modify list item data.....
BOOL CMPhiHingeChildDlg::ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATD_D &mData)
{
	ListData data; data.initialize();

	if(m_nType==D_RCPSC_TYPE && mData.Type == _T("C"))
	{		
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data1.CodeMatlName;
		data.dElast = mData.Data1.Analysis.Elast;
		data.dSig = mData.Data1.Design.C_fc;
		data.cSGradeName = mData.MainRebar_RebarName;
		data.dSElast = mData.MainRebarData.B_Elast;
		// 20080514 mylee - MPhi �̰� Rebar Code�� None ���ý� Preference �ڵ带 �����Ͽ� �����Ѵ�.
		// ���� ������ �ٸ� ������ �ǰ� ������, ���⼭�� ����Ʈ�� Es ���� �����ֱ� ���ؼ� ó��. (������)
		if(mData.Rebar_CodeName==_T("None"))
			GetEsFromPreference(data.dSElast);
		data.dSigSy = mData.MainRebarData.B_fy;								
		data.bUse = ExistMphg(Key);
	}
	else if(m_nType==D_STEEL_TYPE && mData.Type == _T("S"))
	{
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data1.CodeMatlName;
		data.dElast = mData.Data1.Analysis.Elast;
		data.dSig = mData.Data1.Design.S_Fy1;			
		data.bUse = ExistMphg(Key);
	}
	else if(m_nType==D_SRC_TYPE && mData.Type == _T("SRC"))
	{
		data.nId = (int)Key;
		data.cName = mData.Name;
		data.cGradeName = mData.Data2.CodeMatlName;
		data.dElast = mData.Data2.Analysis.Elast;
		data.dSig = mData.Data2.Design.C_fc;
		data.cSGradeName = mData.Data1.CodeMatlName;
		data.dSElast = mData.Data1.Analysis.Elast;
		data.dSigSy = mData.Data1.Design.S_Fy1;								
		data.bUse = ExistMphg(Key);
	}
	
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_cPropertyListCtrl.FindItem(&FindInfo);

	if (nItem != -1)
	{
		CString str;
		if(m_nType==D_RCPSC_TYPE || m_nType==D_SRC_TYPE)
		{
			for(int i = 0; i < 9; i++)
			{
				str = DataToStr(i, (int)Key, data);
				m_cPropertyListCtrl.SetItemText(nItem, i, str);
			}
		}
		else if(m_nType==D_STEEL_TYPE)
		{
			int index = 0;
			for(int i = 0; i < 9; i++)
			{
				if(i<5 || i>7)
				{
					str = DataToStr(i, (int)Key, data);
					m_cPropertyListCtrl.SetItemText(nItem, index, str);
					index++;
				}
			}
		}
		// key �� ���� 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_cPropertyListCtrl.SetItem(&lvitem);
	}
	return TRUE;
}

CString CMPhiHingeChildDlg::DataToStr(int i, int Key, ListData &Data)
{
	CString str;

	if (i==0) str.Format(_T("%5d"), Data.nId);
	else if (i==1) str = Data.cName;
	else if (i==2) str = Data.cGradeName;
	else if (i==3) str.Format(_T("%.2e"), Data.dElast);
	else if (i==4) str.Format(_T("%.2e"), Data.dSig);	
	else if (i==5) str = Data.cSGradeName;
	else if (i==6) str.Format(_T("%.2e"), Data.dSElast);
	else if (i==7) str.Format(_T("%.2e"), Data.dSigSy);
	else if (i==8) str = Data.bUse ? _T("O") : _T("X");	
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

void CMPhiHingeChildDlg::UpdateList(LPARAM lHint, CObject* pHint)
{
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
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CMPhiHingeChildDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MATL_K Key, KeyBak;
	T_MATD_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATD_ADD):
				// ���Ե� ��ġ(nRow)�� ã�� �ִ´�.
				pViewBuff->GetMatd(nKey, Key, Data);
				//InsertItem(Key, Data);
				break;
		case(UR_MATD_DEL):
				// ������ ��ġ(nRow)�� ã�Ƽ� ������.
				pViewBuff->GetMatd(nKey, Key, Data);
				//DeleteItem(Key, Data);
				break;
		case(UR_MATD_MFD):
				// ���⼭�� ���常 �ϰ� UR_MATD_MFS���� ó���Ѵ�.
				pViewBuff->GetMatd(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_MATD_MFS):
				// MFD�� �������� MFS�� �´�. �׻� �� �������� �� ¦�� �Ǿ� �´�.
				ASSERT(bMFD);
				pViewBuff->GetMatd(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// �÷��� ����
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

void CMPhiHingeChildDlg::OnSelchangeModelCobx() 
{
	EnableCtrl();
	ShowHildeForFold();

#if defined(_CIVIL_JP)
	if(m_nType == D_RCPSC_TYPE) ResetPmmMyMzComboBox();
#endif
	RedrawWindow();
}

void CMPhiHingeChildDlg::ResetPmmMyMzComboBox()
{
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nCode);
	int nIndex = 0;
	m_PmmModelCobx.ResetContent();

	if (nCode == 6)
	{
		nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_TAKTET));
		m_PmmModelCobx.SetItemData(nIndex, D_IEHP_TTET);

		nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_MTATET));
		m_PmmModelCobx.SetItemData(nIndex, D_IEHP_MTTE);

		for (int i = 0; i < 2; i++)
		{
			m_cmbModel[i].ResetContent();
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKTET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_TTET);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MTATET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTTE);
		}
	}
	else
	{
		nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_KINEMA));
		m_PmmModelCobx.SetItemData(nIndex, D_IEHP_KINE);

#if defined(_CIVIL_JP)
		if (nCode != 5)
		{
			nIndex = m_PmmModelCobx.AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));
			m_PmmModelCobx.SetItemData(nIndex, D_IEHP_MTAK);
		}
#endif
		for (int i = 0; i < 2; i++)
		{
			m_cmbModel[i].ResetContent();
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_KINEMA));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_KINE);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ORIGIN));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_ORIG);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PEAK));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_PICK);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_CLOUGH));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_CLOU);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DEGRAD));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_DEGR);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKEDA));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_TAKE);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKTET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_TTET);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTAK);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MTATET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTTE);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NORBIL));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_NBIL);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELABIL));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_EBIL);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATRI));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETRI);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETET);

			// SLIP Type �̷¸��� (6.9.0 �߰�)...
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPB);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBC);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTC);

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SRCTET));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_SRCT);

			// �̷� ���� �߰� �ÿ��� �������� �߰� �Ѵ�....
		}
	}

	int default = m_PmmModelCobx.SelectString(0, GetHysModelName(m_Data.nHysModel[0]));
	if (default == CB_ERR) m_PmmModelCobx.SetCurSel(0);
	for (int i = 0; i < 2; i++)
	{
		default = m_cmbModel[i].SelectString(0, GetHysModelName(m_Data.nHysModel[i + 1]));
		if (default == CB_ERR) m_cmbModel[i].SetCurSel(0);
	}
}

void CMPhiHingeChildDlg::OnTypeIRdo()
{
}

void CMPhiHingeChildDlg::OnDestroy()
{
	m_pGuideDlg->End();
	delete m_pGuideDlg;
	m_pGuideDlg = NULL;

	CDlgChild::OnDestroy();
}

void CMPhiHingeChildDlg::OnClickYbPointCbx()
{
	UpdateData(TRUE);
}