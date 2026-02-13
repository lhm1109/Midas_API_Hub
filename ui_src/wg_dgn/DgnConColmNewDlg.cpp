// DgnConColmNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmNewDlg.h"

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmNewDlg dialog
CDgnConColmNewDlg::CDgnConColmNewDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnConColmNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nD0 = 0.0;
	m_Hc = 0.0;
	m_Bc = 0.0;
	m_Hf = 0.0;
	m_Bf = 0.0;

	m_bInit = FALSE;
}

CDgnConColmNewDlg::~CDgnConColmNewDlg()
{
}

void CDgnConColmNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmNewDlg)
	DDX_Control(pDX, IDC_DGN_SECT_LST, m_SectList);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_CHK, m_chkSubSect);
	DDX_Control(pDX, IDC_DGN_SECT_KEY_EDT, m_edtSectK);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_EDT, m_edtSubSectK);
	DDX_Control(pDX, IDC_DGN_MEMB_LIST_EDT, m_edtMemb);
	DDX_Control(pDX, IDC_DGN_CONC_DZERO_EDT, m_edtD0);
	DDX_Control(pDX, IDC_DGN_CONC_FACE_EDT, m_untD0);
	DDX_Control(pDX, IDC_DGN_RC_BCJR_EDT, m_edtBcjr);
	DDX_Control(pDX, IDC_DGN_RC_BCJR_SPN, m_spnBcjr);
	DDX_Control(pDX, IDC_DGN_DETAIL_FIGURE_STC, m_grbDetail);
	DDX_Control(pDX, IDC_DGN_INODE_STC, m_INodeWnd);
	DDX_Control(pDX, IDC_DGN_CENT_STC, m_CenterWnd);
	DDX_Control(pDX, IDC_DGN_CON_COLM_SAME_END_REBAR_CHK, m_chkSameRebarMidEndt);
	DDX_Control(pDX, IDC_DGN_REBAR_DATA_GRD, m_RebarGrid);
	DDX_Control(pDX, IDC_DGN_COLRECT_BIT, m_wndRect);
	DDX_Control(pDX, IDC_DGN_COLCIRCLE_BIT, m_wndCircle);
	DDX_Control(pDX, IDC_DGN_COLPIPE_BIT, m_wndPipe);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CDgnConColmNewDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConColmNewDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_SUB_SECT_CHK, OnChkSubSection)
	ON_EN_CHANGE(IDC_DGN_CONC_DZERO_EDT, OnChangeDZero)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_ADD_REP_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_DELETE_BTN, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_CON_COLM_SAME_END_REBAR_CHK, OnDgnSameEndNCenterRebar)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnConColmNewDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDgnDlgBase::Create(nIDTemplate, pParentWnd);
}

void CDgnConColmNewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_UNIT:
		m_RebarGrid.UpdateUnit();    
		break;
	case D_UPDATE_SEL_ADD:    case D_UPDATE_SEL_DEL:
//    UpdateSelectedMembEdit();
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
			break;
	default:
		break;
	}
}

BOOL CDgnConColmNewDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//   if (pMsg->message == WM_KEYDOWN)
//   {
//     CWnd* pWnd = GetFocus();
//     if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
//       pMsg->wParam == VK_RETURN && pWnd->GetDlgCtrlID() == IDC_DGN_MEMB_LIST_EDT)
//     {
//       UpdateSelectedMembEdit();
//       return TRUE;
//     }
//   }	
	return CDgnDlgBase::PreTranslateMessage(pMsg);
}

void CDgnConColmNewDlg::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bChange = FALSE;
	BOOL bModify = FALSE;
	T_REBC_K RebcK;   RebcK.keymap = 0;
	T_REBC_D RebcD;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SECD_ADD):
		case(UR_SECD_DEL):
		case(UR_SECD_MFS):
		case(UR_SECD_MFD):
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		case(UR_ELEM_MFS):
		case(UR_ELEM_MFD):
		case(UR_REBC_ADD):
		case(UR_REBC_DEL):  bChange = TRUE; break;
		case(UR_REBC_MFS):
		case(UR_REBC_MFD):  bModify = TRUE;   break;
		default:						break;
		}

		if(nCmd == UR_REBC_ADD)
			pViewBuff->GetRebc(nKey, RebcK, RebcD);
	}
	if(bChange)
	{
		CListCtrlScrollMaintainer lsm(&m_SectList);

		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
		if(RebcK.keymap)    ShowSelectedRebc(RebcK.keymap, FALSE);
	}
	if(bModify)
	{
		LRESULT Result;
		OnClkListItem(NULL, &Result);
	}
}

void CDgnConColmNewDlg::ShowSelectedRebc(T_REBC_KEY RebcKey, BOOL bInit)
{
	int nIndex = 0;
	if(!m_mRebcK2Index.Lookup(RebcKey, nIndex))     {  ASSERT(0);		return ;	}

	if(!bInit)   MakeItemEx();
	// Unselect current selection
	m_SectList.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	m_SectList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
//  m_SectList.SetSelectionMark(nIndex);

	LRESULT Result;
	OnClkListItem(NULL, &Result);
	m_SectList.SetFocus();
}

void CDgnConColmNewDlg::ShowBitmap(CString str)
{
	if(str==_T("SB"))
	{
		m_wndRect.ShowWindow(SW_SHOW);
		m_wndCircle.ShowWindow(SW_HIDE);
		m_wndPipe.ShowWindow(SW_HIDE);
	}
	else if(str==_T("SR"))
	{
		m_wndRect.ShowWindow(SW_HIDE);
		m_wndCircle.ShowWindow(SW_SHOW);
		m_wndPipe.ShowWindow(SW_HIDE);
	}
	else if(str==_T("P"))
	{
		m_wndRect.ShowWindow(SW_HIDE);
		m_wndCircle.ShowWindow(SW_HIDE);
		m_wndPipe.ShowWindow(SW_SHOW);
	}
}

void CDgnConColmNewDlg::DrawSectionWnd(T_REBC_D* pRebcD)
{
	int nShape = 0;
	if(m_strShape == _LSX(SB))    nShape = eDGN_RECT;
	else if(m_strShape == _LSX(SR))    nShape = eDGN_CIR;
	else if(m_strShape == _LSX(P))    nShape = eDGN_PIPE;
	else return;

	m_INodeWnd.Column_DrawData(nShape,m_Hc,m_Bc,pRebcD->nQrb,pRebcD->nRow,m_nD0, pRebcD->bUseCornerRebar);
	m_CenterWnd.Column_DrawData(nShape,m_Hc,m_Bc,pRebcD->nQrb,pRebcD->nRow,m_nD0, pRebcD->bUseCornerRebar);

	m_INodeWnd.Invalidate();
	m_CenterWnd.Invalidate();
}

void CDgnConColmNewDlg::Initial_SectionData(T_SECT_D& SectD)
{  
	if(SectD.SectBefore.Shape==_T("SB"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
		m_Bc = SectD.SectBefore.SectI.Size[1];
	}
	else if(SectD.SectBefore.Shape==_T("T"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
		m_Bf = SectD.SectBefore.SectI.Size[1];
		m_Bc = SectD.SectBefore.SectI.Size[2];
		m_Hf = SectD.SectBefore.SectI.Size[3];
	}
	if(SectD.SectBefore.Shape==_T("SB"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
		m_Bc = SectD.SectBefore.SectI.Size[1];
//    Show_RectColumnShape();
	}
	else if(SectD.SectBefore.Shape==_T("SR"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
//    Show_CircColumnShape();
	}
	else if(SectD.SectBefore.Shape==_T("P"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
		m_Bc = SectD.SectBefore.SectI.Size[1];
//    Show_CircColumnShape();
	}
	
	m_nD0 = m_edtD0.GetEditValue();
	if(m_nD0==0)	m_nD0 = Get_SectionDimension(SectD);
}

double CDgnConColmNewDlg::Get_SectionDimension(T_SECT_D& SectD)
{
	double H = 0.0;
		H = SectD.SectBefore.SectI.Size[0];
	
	double maxN = H/10.;
	
	double Factor=Get_ChangeUnitFactor();
	double ComN=6.35*Factor;
	
	maxN = max(maxN,ComN);
	double Max = 7.62*Factor;
	if(maxN >= Max) maxN = Max;
	
	return maxN;
}

double CDgnConColmNewDlg::Get_ChangeUnitFactor()
{
	//  기준 = tonf/cm^2
	T_UNIT_INDEX CurrentUnitIndex;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);
	
	double LenFactor=0.0;
	if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)			LenFactor = 10;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)	LenFactor = 1.;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)	LenFactor = 0.01;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)	LenFactor = 1./2.54;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)	LenFactor = 1./30.48;
	
	return LenFactor;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmNewDlg message handlers

BOOL CDgnConColmNewDlg::OnInitDialog() 
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
void CDgnConColmNewDlg::OnClose()
{
	CDgnConColmNewDlg::OnDgnClose();
}

void CDgnConColmNewDlg::OnChkSubSection()
{
	BOOL bCheck = m_chkSubSect.GetCheck();
	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_SECT_KEY_EDT);
	aCtrls.Add(IDC_DGN_DASH_STC);
	aCtrls.Add(IDC_DGN_SUB_SECT_EDT);
	CDlgUtil::CtrlEnableDisable(this, aCtrls, bCheck);

//  m_edtMemb.SetReadOnly(!bCheck);

	T_REBC_KEY RebcKey;
	if(!GetSelectedItem(RebcKey))    return;
	
	T_REBC_K RebcK;   RebcK.keymap = RebcKey;
	BOOL bShow = RebcK.key.subSectK == 0?   TRUE:   FALSE;

	CString strTemp;
	if(bCheck && bShow)
	{
		int SectK = RebcK.key.sectK;
		strTemp.Format(_T("%d"), SectK);
		m_edtSectK.SetWindowText(strTemp);

		int nSubSectK = GetNextSubSectionKey(SectK);

		strTemp.Format(_T("%d"), nSubSectK);
		m_edtSubSectK.SetWindowText(strTemp);
	}
	else
	{
		m_edtSectK.SetWindowText(_T(""));
		m_edtSubSectK.SetWindowText(_T(""));
	}

	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nSelected = GetSelectedItems(aRebcKey);
	
	BOOL bEdtMemb = TRUE;
	if(nSelected > 1 || (bShow && !bCheck))    bEdtMemb = FALSE;
	m_edtMemb.SetReadOnly(!bEdtMemb);
}

void CDgnConColmNewDlg::OnChangeDZero()
{
	m_nD0 = m_edtD0.GetEditValue();

	T_REBC_D RebcD;
	m_RebarGrid.Grid2Data(&RebcD);
	DrawSectionWnd(&RebcD);
}

void CDgnConColmNewDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)   OnClkListItem(pNMHDR, pResult);
	
	*pResult = 0;
}

void CDgnConColmNewDlg::OnDgnSameEndNCenterRebar()
{
	BOOL bSameEndNCentert = m_chkSameRebarMidEndt.GetCheck();

	m_RebarGrid.SetSameRebarEndNCenter(bSameEndNCentert);
	
	T_REBC_D RebcD;
	m_RebarGrid.Grid2Data(&RebcD);
	DrawSectionWnd(&RebcD);
}

void CDgnConColmNewDlg::OnDgnAddReplace()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_REBC_D RebcD;   RebcD.Initialize();
	if(!Dlg2Data(&RebcD))   return;

	T_REBC_KEY RebcKey;
	if(!GetSelectedItem(RebcKey))    return ;
	T_REBC_K RebcK;   RebcK.keymap = RebcKey; 

	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nRebcSize = GetSelectedItems(aRebcKey);

	if(nRebcSize == 1 && (m_chkSubSect.GetCheck() || RebcK.key.subSectK))
	{
		int nMrebSize = m_aMrebK.GetSize();
//     if(nMrebSize < 1)
//     {
//       AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
//       return;
//     }

		if(!RebcK.key.subSectK)
		{
			CString strTemp;
			m_edtSubSectK.GetWindowText(strTemp);
			int nSubSectK = _ttoi(strTemp);  
			RebcK.key.subSectK = nSubSectK;
		}

		CArray<T_REBC_K,T_REBC_K> aRebcK;   aRebcK.Add(RebcK);
		CArray<T_REBC_D,T_REBC_D&> aRebcD;   aRebcD.Add(RebcD);

		T_MREB_D MrebD;   MrebD.RebcK = RebcK;
		MrebD.nMembType = D_MBTP_COLUMN;
		CArray<T_MREB_D,T_MREB_D&> aMrebD;
		
		aMrebD.SetSize(nMrebSize);
		for(int i = 0; i < nMrebSize; i++)    aMrebD[i] = MrebD;

		if(!pDoc->m_pDataCtrl->AddRebcMreb(aRebcK, aRebcD, m_aMrebK, aMrebD))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(nRebcSize < 1)   return;

		CArray<T_REBC_K,T_REBC_K> aRebcK;
		aRebcK.SetSize(nRebcSize);
		for(int i = 0; i < nRebcSize; i++)
		{
			aRebcK[i].keymap = aRebcKey[i];
		}

		if(!pDoc->m_pDataCtrl->AddRebc(aRebcK, RebcD))     {  ASSERT(0);		return ;	}
	}
}

void CDgnConColmNewDlg::OnDgnDelete()
{
	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nRebcSize = GetSelectedItems(aRebcKey);
	if(nRebcSize < 1)   return;
	
	CArray<T_REBC_K,T_REBC_K> aRebcK;
	aRebcK.SetSize(nRebcSize);
	for(int i = 0; i < nRebcSize; i++)
	{
		aRebcK[i].keymap = aRebcKey[i];
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if(!pDoc->m_pDataCtrl->DelRebc(aRebcK))     {  ASSERT(0);		return ;	}
}

void CDgnConColmNewDlg::OnDgnClose()
{
	m_RebarGrid.DestroyWindow();
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CDgnConColmNewDlg::Data2Dlg()
{
	MakeItemEx();

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

BOOL CDgnConColmNewDlg::Dlg2Data(T_REBC_D* pRebcD)
{
	m_RebarGrid.Grid2Data(pRebcD);
	pRebcD->dConcCenter = m_edtD0.GetEditValue();

	if(pRebcD->dConcCenter <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_PROTECT_DIST1),MB_OK);
		return FALSE;
	}

	pRebcD->bSameRebarEndNCenter = m_chkSameRebarMidEndt.GetCheck();

	CString strTemp;
	m_edtBcjr.GetWindowText(strTemp);
	pRebcD->iBCJointRebarNum = _ttoi(strTemp);

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	int nHoopType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, aCtrls, nHoopType);
	pRebcD->nHoopType = nHoopType + 1;

	T_REBC_KEY RebcKey;
	if(!GetSelectedItem(RebcKey))    return FALSE;
	T_REBC_K RebcK;   RebcK.keymap = RebcKey;

	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nRebcSize = GetSelectedItems(aRebcKey);

	if(nRebcSize == 1 && (m_chkSubSect.GetCheck() || RebcK.key.subSectK))
	{
		CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		T_ELEM_D ElemD;
		T_MEMB_D MembD;

		T_SECT_K SectK = RebcK.key.sectK;

		CArray<T_ELEM_K,T_ELEM_K> aElemK;
//    int nCount = GetSelectedMember(aMembK);
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
		int nCount = aElemK.GetSize();

		m_aMrebK.RemoveAll();
		for(int i = 0; i < nCount; i++)
		{
			if(pDoc->m_pAttrCtrl->GetMemberType(aElemK[i]) != D_MBTP_COLUMN)   continue;
			if(!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))     {  ASSERT(0);		continue ;	}
			if(ElemD.elpro != SectK)    continue;
//       if(pDoc->m_pAttrCtrl->GetMemb(aMembK[i], MembD))    m_aMrebK.Append(MembD.aElemList);
//       else  m_aMrebK.Add(aMembK[i]);
			m_aMrebK.Add(aElemK[i]);
		}
	}

	return TRUE;
}

void CDgnConColmNewDlg::InitCtrls()
{
	SetListCtrlHeader();

	OnChkSubSection();

	m_edtMemb.Connect(SC_ID_ELEM, &m_edtMemb);
	ShowBitmap(_LSX(SB));

	CRect rect;
	GetDlgItem(IDC_DGN_REBAR_DATA_GRD)->GetWindowRect(rect);

	m_RebarGrid.SetGridWndSize(rect.Width(), rect.Height());
	m_RebarGrid.Initialize(this);

	m_grbDetail.InitControl(this, CDgnConColmNewDlg::IDD, IDC_DGN_DETAIL_FIGURE_STC);

	m_edtD0.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD0.SetUnitType(D_UNITSYS_BASE_LENGTH);

//   CWnd* PlaceHolder0 = GetDlgItem(IDC_DGN_INODE_STC);   ASSERT(PlaceHolder0);
//	m_INodeWnd.Initial(PlaceHolder0);
	m_INodeWnd.m_SectionType = eDGN_COLM;

//   PlaceHolder0 = GetDlgItem(IDC_DGN_CENT_STC);   ASSERT(PlaceHolder0);
// 	m_CenterWnd.Initial(PlaceHolder0);
	m_CenterWnd.m_SectionType = eDGN_COLM;

	m_spnBcjr.SetRange(0, 200);

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aCtrls, 0);

	m_chkSameRebarMidEndt.SetCheck(TRUE);
	OnDgnSameEndNCenterRebar();

	m_wndRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_rect.svg"));
	m_wndCircle.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_cir.svg"));
	m_wndPipe.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_pipe.svg"));
}

void CDgnConColmNewDlg::AlignCtrls()
{
	BOOL bEC8CapaciyDesign = FALSE;
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_DCON_D DconD; DconD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetDcon(DconD))    DconD.Initialize();
	
	BOOL bAfterNTC2012 = DconD.nNTCType == 1 || DconD.nNTCType == 2 ? TRUE : FALSE;
	if((DconD.DesignCode==CONCODE_EC2_04 && DconD.bSpecialSeismic) || (DconD.bNTCFlag==TRUE && bAfterNTC2012 && DconD.bBCJointDgn==TRUE))//PMS:4562 (Tel. 4022) MNET:XXXX-BSCHOI-20121120
		bEC8CapaciyDesign = TRUE;
	

	if(!bEC8CapaciyDesign)
	{
		CArray<UINT,UINT> aHideCtrls;
		aHideCtrls.Add(IDC_DGN_RC_BCJR_STC);
		aHideCtrls.Add(IDC_DGN_RC_BCJR_EDT);
		aHideCtrls.Add(IDC_DGN_RC_BCJR_SPN);
		CDlgUtil::CtrlShowHide(this, aHideCtrls, FALSE);

		CArray<UINT,UINT> aMoveCtrls;
		aMoveCtrls.Add(IDC_DGN_DETAIL_FIGURE_STC);
		aMoveCtrls.Add(IDC_DGN_SECT_ENDI_STC);
		aMoveCtrls.Add(IDC_DGN_SECT_CENTER_STC);
		aMoveCtrls.Add(IDC_DGN_INODE_STC);
		aMoveCtrls.Add(IDC_DGN_CENT_STC);
		CRect rTgt, rSrc;
		int nDistX, nDistY;	
		
		GetDlgItem(IDC_DGN_RC_BCJR_STC)->GetWindowRect(rTgt);
		GetDlgItem(IDC_DGN_DETAIL_FIGURE_STC)->GetWindowRect(rSrc);
		nDistY = rTgt.top - rSrc.top;
		nDistX = rTgt.left - rSrc.left;
		CDlgUtil::CtrlMoveDistXY(this, aMoveCtrls, nDistX, nDistY);
	}
}

void CDgnConColmNewDlg::EnableDisableCtrls()
{

}

// List Ctrl 함수들
void CDgnConColmNewDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_REBC_D RebcD;
	
	T_REBC_KEY RebcKey;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		if(!m_mIndex2RebcK.Lookup(nIndex, RebcKey))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(!GetSelectedItem(RebcKey))    return;
	}

	CArray<T_REBC_KEY,T_REBC_KEY> aRebcKey;
	int nSelected = GetSelectedItems(aRebcKey);
	
	CArray<T_REBC_K,T_REBC_K> aRebcK;
	aRebcK.SetSize(nSelected);
	for(int i = 0; i < nSelected; i++)
	{
		aRebcK[i].keymap = aRebcKey[i];
	}

	T_REBC_K RebcK;   RebcK.keymap = RebcKey;
	BOOL bEnableChk = RebcK.key.subSectK == 0;
	if(nSelected > 1)   bEnableChk = FALSE;
	m_chkSubSect.EnableWindow(bEnableChk);

	if(!bEnableChk)  m_chkSubSect.SetCheck(FALSE);
	OnChkSubSection();

	if(pDoc->m_pViewCtrl->SelectedElemCount()) pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTempK;
	for(int k = 0; k < nSelected; k++)
	{
		int nCount = pDoc->m_pAttrCtrl->GetAllElemKeyListUseRebc(aRebcK[k], aTempK);

		for(int i=nCount-1; i>= 0; i--)
		{
			int nMembType = pDoc->m_pAttrCtrl->GetMemberType(aTempK[i]);
			if(nMembType != D_MBTP_COLUMN)        aTempK.RemoveAt(i);
		}
		aElemK.Append(aTempK);
	}
	pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

	T_SECT_K SectK = RebcK.key.sectK;
	T_SECT_D SectD;   SectD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD))	return;
	m_strShape = SectD.SectBefore.Shape;
	if (!pDoc->m_pAttrCtrl->GetRebc(RebcK, RebcD))
	{
		RebcD.Initialize();
		CDgnDataCtrl DataCtrl;
		T_DCRB_D DcrbD;    DcrbD.Initialize();
		DataCtrl.Get_DgnConDcrb(0, DcrbD);

		RebcD.MainRebarName = DcrbD.MainRebarColumn[0];
		RebcD.CornerRebarName = DcrbD.MainRebarColumn[0]; // Add/Modify by Seungjun PMS:4491  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
		RebcD.SubRebarName = DcrbD.StirrupBarColumn; // Modify by GAY. MQC:4491 검토 중 발견. ('12.06.27). DcrbD.MainRebarColumn[0];
		RebcD.SubRebarName2 = DcrbD.StirrupBarColumn; // Modify by GAY. PMS:4491 검토 중 발견. DcrbD.MainRebarColumn[0];
		
		if(m_strShape==_LSX(SB))
		{
			if(RebcD.nQrb < 4)        RebcD.nQrb = 4;
			if(RebcD.nRow < 2)        RebcD.nRow = 2;
		}
		else if(m_strShape==_LSX(SR) || m_strShape==_LSX(P)) 
		{
			/// [GEN-11023] 원형기둥의 주철근 배근 최소값 수정 : 6 -> 4.
			if(RebcD.nQrb < 4)    RebcD.nQrb = 4;
			if(RebcD.nRow < 2)    RebcD.nRow = 2;
		}
		if(RebcD.nHoopType == 0)    RebcD.nHoopType = 1;

		RebcD.dConcCenter = Get_SectionDimension(SectD);
	}

	m_RebarGrid.Data2Grid(&RebcD, &m_strShape);
	m_chkSameRebarMidEndt.SetCheck(RebcD.bSameRebarEndNCenter);
	m_RebarGrid.SetSameRebarEndNCenter(RebcD.bSameRebarEndNCenter);

	m_edtD0.SetEditUnit(RebcD.dConcCenter);

	ShowBitmap(m_strShape);

	Initial_SectionData(SectD);
	DrawSectionWnd(&RebcD);

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aCtrls, RebcD.nHoopType-1);

	CString strTemp;
	strTemp.Format(_T("%d"), RebcD.iBCJointRebarNum);
	m_edtBcjr.SetWindowText(strTemp);

	*pResult = 0;
}

void CDgnConColmNewDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_GRID_TEXT_SECTION), _LS(IDS_DGN_NAME), _LS(IDS_DGN_LIST_BAR)};
	int nColWidth[] = { globalUtils.ScaleByDPI(50), globalUtils.ScaleByDPI(70), globalUtils.ScaleByDPI(34)};
	int nColNum = 3;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	CRect rect;
	GetDlgItem(IDC_DGN_SECT_LST)->GetWindowRect(rect);
	nColWidth[2] = rect.Width() - nColWidth[0] - nColWidth[1] - globalUtils.ScaleByDPI(21);
	
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_SectList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_SectList.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_SectList.InsertColumn(i,&lvcolumn);
		m_SectList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CDgnConColmNewDlg::GetSelectedItem(T_REBC_KEY& RebcKey)
{
	int iItem = m_SectList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;
	
	int nIndex = iItem;
	if(!m_mIndex2RebcK.Lookup(nIndex, RebcKey))     {  ASSERT(0);		return FALSE;	}

	return TRUE;
}

int CDgnConColmNewDlg::GetSelectedItems(CArray<T_REBC_KEY,T_REBC_KEY>& aRebcKey)
{
	aRebcKey.RemoveAll();

	POSITION pos = m_SectList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		while (pos)
		{
			int nItem = m_SectList.GetNextSelectedItem(pos);
			
			int nIndex = nItem;
			T_REBC_KEY RebcKey;
			if(!m_mIndex2RebcK.Lookup(nIndex, RebcKey))     {  ASSERT(0);		return FALSE;	}

			aRebcKey.Add(RebcKey);
		}
	}

	return aRebcKey.GetSize();
}

void CDgnConColmNewDlg::MakeItemEx()
{
	m_SectList.DeleteAllItems();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMapEx<int,int,int,int> mSectK4Colm;

	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();
	
	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);
		
		T_MATD_D MatdData; MatdData.Initialize();
		if(!pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;
		
		if(MatdData.Type == _T("C"))
		{
			// 1=column, 2=colm, 3=brace, 0=기타
			int nMembType = pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);
			
			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);
			
			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_COLUMN && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("SR") ||
															SectData.SectBefore.Shape == _T("P"))) 
					mSectK4Colm.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	CArray<int,int> aSectK;
	int nSect = mSectK4Colm.GetKeyList(aSectK);
	qsort((void*)aSectK.GetData(), nSect, sizeof(int), CCompFunc::UINTAsc);

	m_mIndex2RebcK.RemoveAll();
	m_mRebcK2Index.RemoveAll();
	int SectK, nIndex = 0;

	int nInitIndex = 0;

	for(int i = 0; i < nSect; i++)
	{
		SectK = aSectK[i];
		m_mIndex2RebcK.SetAt(nIndex, SectK);
		m_mRebcK2Index.SetAt(SectK, nIndex);

		T_SECT_D SectD;     SectD.Initialize();
		pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD);

//    m_SectList.InsertItem(nIndex, _T(""));
		if(pDoc->m_pAttrCtrl->ExistRebc(SectK))    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(In)));
		else    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(-)));

		CArray<T_REBC_K, T_REBC_K> aRebcK;
		int nCountRebc = pDoc->m_pAttrCtrl->GetRebcKeyListUsingSectK(SectK, aRebcK);
		for(int k = 0; k < nCountRebc; k++)
		{
			T_REBC_K RebcK = aRebcK[k];
			m_mIndex2RebcK.SetAt(nIndex, RebcK.keymap);
			m_mRebcK2Index.SetAt(RebcK.keymap, nIndex);

			SetItem(nIndex++, RebcK.key.sectK, RebcK.key.subSectK, SectD.SName, CString(_LSX(In)));
		}
	}

//  m_SectList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CDgnConColmNewDlg::SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar)
{
	m_SectList.InsertItem(nIndex, _T(""));

	LVITEM lvitem;
	CString strTemp;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;

		switch(i)
		{
		case 0:
			if(SubSectK == 0)   strTemp.Format(_T("%d"), SectK);
			else                strTemp.Format(_T("%d-%d"), SectK, SubSectK);
			break;
		case 1:   strTemp = strSectName;      break;
		case 2:   strTemp = strRebar;         break;
		default:    ASSERT(0);  
		}

		lvitem.pszText=strTemp.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_SectList.SetItem(&lvitem);
		
		strTemp.ReleaseBuffer();
	}
}

void CDgnConColmNewDlg::UpdateSelectedMembEdit()
{
	CArray<T_MEMB_K,T_MEMB_K> aMembK;
	int nCount = GetSelectedMember(aMembK);

	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.OptimizeUnsortedArray(aMembK);
	m_edtMemb.SetWindowText(sOptimizedString);
}

int CDgnConColmNewDlg::GetSelectedMember(CArray<UINT,UINT>& aMembK)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	CString strMemb;
	m_edtMemb.GetWindowText(strMemb);
	
	T_MEMB_K MembK;
	T_MEMB_D MembD;
	CArray<int,int> aKey;
	
	if(!CStrParser::ParsingListByTo(strMemb,aKey)) return 0;
	
	int nSize = aKey.GetSize();
	for(int k = 0; k < nSize; k++)
	{
		if(pDoc->m_pAttrCtrl->GetMembAssigned(aKey[k], MembK))
		{
			if(!pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))      {  ASSERT(0);		return 0;	}
			if(MembD.aElemList[0] != aKey[k])      continue;
		}
		aMembK.Add(aKey[k]);
	}

	return aMembK.GetSize();
}

UINT CDgnConColmNewDlg::GetNextSubSectionKey(UINT SectK)
{
	UINT nSubSectK = 1;
	T_REBC_K RebcK;   RebcK.key.sectK = SectK;      RebcK.key.subSectK = nSubSectK;
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	while(pDoc->m_pAttrCtrl->ExistRebc(RebcK))
	{  
		nSubSectK++;
		RebcK.key.sectK = SectK;      RebcK.key.subSectK = nSubSectK;
	}

	return nSubSectK;
}
