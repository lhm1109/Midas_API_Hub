// DgnConBraceNewDlg.cpp : implementation file
//
// DgnConColmNewDlg와 동일. 수정시 함께 반영 필요
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBraceNewDlg.h"

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
// CDgnConBraceNewDlg dialog
CDgnConBraceNewDlg::CDgnConBraceNewDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnConBraceNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBraceNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nD0 = 0.0;
	m_Hc = 0.0;
	m_Bc = 0.0;
	m_Hf = 0.0;
	m_Bf = 0.0;

	m_bInit = FALSE;
}

CDgnConBraceNewDlg::~CDgnConBraceNewDlg()
{
}

void CDgnConBraceNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBraceNewDlg)
	DDX_Control(pDX, IDC_DGN_SECT_LST, m_SectList);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_CHK, m_chkSubSect);
	DDX_Control(pDX, IDC_DGN_SECT_KEY_EDT, m_edtSectK);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_EDT, m_edtSubSectK);
	DDX_Control(pDX, IDC_DGN_MEMB_LIST_EDT, m_edtMemb);
	DDX_Control(pDX, IDC_DGN_CONC_DZERO_EDT, m_edtD0);
	DDX_Control(pDX, IDC_DGN_CONC_FACE_EDT, m_untD0);
	DDX_Control(pDX, IDC_DGN_DETAIL_FIGURE_STC, m_grbDetail);
	DDX_Control(pDX, IDC_DGN_INODE_STC, m_INodeWnd);
	DDX_Control(pDX, IDC_DGN_CENT_STC, m_CenterWnd);
	DDX_Control(pDX, IDC_DGN_REBAR_DATA_GRD, m_RebarGrid);
	DDX_Control(pDX, IDC_DGN_COLRECT_BIT, m_wndRect);
	DDX_Control(pDX, IDC_DGN_COLCIRCLE_BIT, m_wndCircle);
	DDX_Control(pDX, IDC_DGN_COLPIPE_BIT, m_wndPipe);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CDgnConBraceNewDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConBraceNewDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_SUB_SECT_CHK, OnChkSubSection)
	ON_EN_CHANGE(IDC_DGN_CONC_DZERO_EDT, OnChangeDZero)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_ADD_REP_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_DELETE_BTN, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnConBraceNewDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDgnDlgBase::Create(nIDTemplate, pParentWnd);
}

void CDgnConBraceNewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BOOL CDgnConBraceNewDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDgnConBraceNewDlg::UpdateBuffer()
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
	T_REBR_K RebrK;   RebrK.keymap = 0;
	T_REBR_D RebrD;
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
		case(UR_REBR_ADD):
		case(UR_REBR_DEL):  bChange = TRUE; break;
		case(UR_REBR_MFS):
		case(UR_REBR_MFD):  bModify = TRUE;   break;
		default:						break;
		}

		if(nCmd == UR_REBR_ADD)
			pViewBuff->GetRebr(nKey, RebrK, RebrD);
	}
	if(bChange)
	{
		CListCtrlScrollMaintainer lsm(&m_SectList);

		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
		if(RebrK.keymap)    ShowSelectedRebr(RebrK.keymap, FALSE);
	}
	if(bModify)
	{
		LRESULT Result;
		OnClkListItem(NULL, &Result);
	}
}

void CDgnConBraceNewDlg::ShowSelectedRebr(T_REBR_KEY RebrKey, BOOL bInit)
{
	int nIndex = 0;
	if(!m_mRebrK2Index.Lookup(RebrKey, nIndex))     {  ASSERT(0);		return ;	}

	if(!bInit)   MakeItemEx();
	// Unselect current selection
	m_SectList.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	m_SectList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
//  m_SectList.SetSelectionMark(nIndex);

	LRESULT Result;
	OnClkListItem(NULL, &Result);
	m_SectList.SetFocus();
}

void CDgnConBraceNewDlg::ShowBitmap(CString str)
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

void CDgnConBraceNewDlg::DrawSectionWnd(T_REBR_D* pRebrD)
{
	int nShape = 0;
	if(m_strShape == _LSX(SB))    nShape = eDGN_RECT;
	else if(m_strShape == _LSX(SR))    nShape = eDGN_CIR;
	else if(m_strShape == _LSX(P))    nShape = eDGN_PIPE;
	else return;

	m_INodeWnd.Column_DrawData(nShape,m_Hc,m_Bc,pRebrD->nQrb,pRebrD->nRow,m_nD0);
	m_CenterWnd.Column_DrawData(nShape,m_Hc,m_Bc,pRebrD->nQrb,pRebrD->nRow,m_nD0);

	m_INodeWnd.Invalidate();
	m_CenterWnd.Invalidate();
}

void CDgnConBraceNewDlg::Initial_SectionData(T_SECT_D& SectD)
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
//    Show_RectBraceShape();
	}
	else if(SectD.SectBefore.Shape==_T("SR"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
//    Show_CircBraceShape();
	}
	else if(SectD.SectBefore.Shape==_T("P"))
	{
		m_Hc = SectD.SectBefore.SectI.Size[0];
		m_Bc = SectD.SectBefore.SectI.Size[1];
//    Show_CircBraceShape();
	}
	
	m_nD0 = m_edtD0.GetEditValue();
	if(m_nD0==0)	m_nD0 = Get_SectionDimension(SectD);
}

double CDgnConBraceNewDlg::Get_SectionDimension(T_SECT_D& SectD)
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

double CDgnConBraceNewDlg::Get_ChangeUnitFactor()
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
// CDgnConBraceNewDlg message handlers

BOOL CDgnConBraceNewDlg::OnInitDialog() 
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

void CDgnConBraceNewDlg::OnChkSubSection()
{
	BOOL bCheck = m_chkSubSect.GetCheck();
	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_SECT_KEY_EDT);
	aCtrls.Add(IDC_DGN_DASH_STC);
	aCtrls.Add(IDC_DGN_SUB_SECT_EDT);
	CDlgUtil::CtrlEnableDisable(this, aCtrls, bCheck);

//  m_edtMemb.SetReadOnly(!bCheck);

	T_REBR_KEY RebrKey;
	if(!GetSelectedItem(RebrKey))    return;
	
	T_REBR_K RebrK;   RebrK.keymap = RebrKey;
	BOOL bShow = RebrK.key.subSectK == 0?   TRUE:   FALSE;

	CString strTemp;
	if(bCheck && bShow)
	{
		int SectK = RebrK.key.sectK;
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

	CArray<T_REBR_KEY,T_REBR_KEY> aRebrKey;
	int nSelected = GetSelectedItems(aRebrKey);
	
	BOOL bEdtMemb = TRUE;
	if(nSelected > 1 || (bShow && !bCheck))    bEdtMemb = FALSE;
	m_edtMemb.SetReadOnly(!bEdtMemb);
}

void CDgnConBraceNewDlg::OnChangeDZero()
{
	m_nD0 = m_edtD0.GetEditValue();

	T_REBR_D RebrD;
	m_RebarGrid.Grid2Data(&RebrD);
	DrawSectionWnd(&RebrD);
}

void CDgnConBraceNewDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)   OnClkListItem(pNMHDR, pResult);
	
	*pResult = 0;
}

void CDgnConBraceNewDlg::OnDgnAddReplace()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_REBR_D RebrD;   RebrD.Initialize();
	if(!Dlg2Data(&RebrD))   return;

	T_REBR_KEY RebrKey;
	if(!GetSelectedItem(RebrKey))    return ;
	T_REBR_K RebrK;   RebrK.keymap = RebrKey; 

	CArray<T_REBR_KEY,T_REBR_KEY> aRebrKey;
	int nRebrSize = GetSelectedItems(aRebrKey);

	if(nRebrSize == 1 && (m_chkSubSect.GetCheck() || RebrK.key.subSectK))
	{
		int nMrebSize = m_aMrebK.GetSize();
//     if(nMrebSize < 1)
//     {
//       AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
//       return;
//     }

		if(!RebrK.key.subSectK)
		{
			CString strTemp;
			m_edtSubSectK.GetWindowText(strTemp);
			int nSubSectK = _ttoi(strTemp);  
			RebrK.key.subSectK = nSubSectK;
		}

		CArray<T_REBR_K,T_REBR_K> aRebrK;   aRebrK.Add(RebrK);
		CArray<T_REBR_D,T_REBR_D&> aRebrD;   aRebrD.Add(RebrD);

		T_MREB_D MrebD;   MrebD.RebrK = RebrK;
		MrebD.nMembType = D_MBTP_BRACE;
		CArray<T_MREB_D,T_MREB_D&> aMrebD;
		
		aMrebD.SetSize(nMrebSize);
		for(int i = 0; i < nMrebSize; i++)    aMrebD[i] = MrebD;

		if(!pDoc->m_pDataCtrl->AddRebrMreb(aRebrK, aRebrD, m_aMrebK, aMrebD))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(nRebrSize < 1)   return;

		CArray<T_REBR_K,T_REBR_K> aRebrK;
		aRebrK.SetSize(nRebrSize);
		for(int i = 0; i < nRebrSize; i++)
		{
			aRebrK[i].keymap = aRebrKey[i];
		}

		if(!pDoc->m_pDataCtrl->AddRebr(aRebrK, RebrD))     {  ASSERT(0);		return ;	}
	}
}

void CDgnConBraceNewDlg::OnDgnDelete()
{
	CArray<T_REBR_KEY,T_REBR_KEY> aRebrKey;
	int nRebrSize = GetSelectedItems(aRebrKey);
	if(nRebrSize < 1)   return;
	
	CArray<T_REBR_K,T_REBR_K> aRebrK;
	aRebrK.SetSize(nRebrSize);
	for(int i = 0; i < nRebrSize; i++)
	{
		aRebrK[i].keymap = aRebrKey[i];
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if(!pDoc->m_pDataCtrl->DelRebr(aRebrK))     {  ASSERT(0);		return ;	}
}

void CDgnConBraceNewDlg::OnDgnClose()
{
	m_RebarGrid.DestroyWindow();
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CDgnConBraceNewDlg::Data2Dlg()
{
	MakeItemEx();

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

BOOL CDgnConBraceNewDlg::Dlg2Data(T_REBR_D* pRebrD)
{
	m_RebarGrid.Grid2Data(pRebrD);
	pRebrD->dConcCenter = m_edtD0.GetEditValue();
	if(pRebrD->dConcCenter <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_PROTECT_DIST1),MB_OK);
		return FALSE;
	}

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	int nHoopType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, aCtrls, nHoopType);
	pRebrD->nHoopType = nHoopType + 1;

	T_REBR_KEY RebrKey;
	if(!GetSelectedItem(RebrKey))    return FALSE;
	T_REBR_K RebrK;   RebrK.keymap = RebrKey;

	CArray<T_REBR_KEY,T_REBR_KEY> aRebrKey;
	int nRebrSize = GetSelectedItems(aRebrKey);

	if(nRebrSize == 1 && (m_chkSubSect.GetCheck() || RebrK.key.subSectK))
	{
		CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		T_ELEM_D ElemD;
		T_MEMB_D MembD;

		T_SECT_K SectK = RebrK.key.sectK;

		CArray<T_ELEM_K,T_ELEM_K> aElemK;
//    int nCount = GetSelectedMember(aMembK);
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
		int nCount = aElemK.GetSize();

		m_aMrebK.RemoveAll();
		for(int i = 0; i < nCount; i++)
		{
			if(pDoc->m_pAttrCtrl->GetMemberType(aElemK[i]) != D_MBTP_BRACE)   continue;
			if(!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))     {  ASSERT(0);		continue ;	}
			if(ElemD.elpro != SectK)    continue;
//       if(pDoc->m_pAttrCtrl->GetMemb(aElemK[i], MembD))    m_aMrebK.Append(MembD.aElemList);
//       else  m_aMrebK.Add(aMembK[i]);
			m_aMrebK.Add(aElemK[i]);
		}
	}

	return TRUE;
}

void CDgnConBraceNewDlg::InitCtrls()
{
	SetListCtrlHeader();

	OnChkSubSection();

	m_edtMemb.Connect(SC_ID_ELEM, &m_edtMemb);
	ShowBitmap(_LSX(SB));

	CRect rect;
	GetDlgItem(IDC_DGN_REBAR_DATA_GRD)->GetWindowRect(rect);

	m_RebarGrid.SetGridWndSize(rect.Width(), rect.Height());
	m_RebarGrid.Initialize(this);

	m_grbDetail.InitControl(this, CDgnConBraceNewDlg::IDD, IDC_DGN_DETAIL_FIGURE_STC);

	m_edtD0.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD0.SetUnitType(D_UNITSYS_BASE_LENGTH);

//   CWnd* PlaceHolder0 = GetDlgItem(IDC_DGN_INODE_STC);   ASSERT(PlaceHolder0);
// 	m_INodeWnd.Initial(PlaceHolder0);
	m_INodeWnd.m_SectionType = eDGN_COLM;

//   PlaceHolder0 = GetDlgItem(IDC_DGN_CENT_STC);   ASSERT(PlaceHolder0);
// 	m_CenterWnd.Initial(PlaceHolder0);
	m_CenterWnd.m_SectionType = eDGN_COLM;

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aCtrls, 0);

	m_wndRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_rect.svg"));
	m_wndCircle.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_cir.svg"));
	m_wndPipe.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_colm_pipe.svg"));
}

void CDgnConBraceNewDlg::AlignCtrls()
{

}

void CDgnConBraceNewDlg::EnableDisableCtrls()
{

}

// List Ctrl 함수들
void CDgnConBraceNewDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_REBR_D RebrD;
	
	T_REBR_KEY RebrKey;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		if(!m_mIndex2RebrK.Lookup(nIndex, RebrKey))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(!GetSelectedItem(RebrKey))    return;
	}

	CArray<T_REBR_KEY,T_REBR_KEY> aRebrKey;
	int nSelected = GetSelectedItems(aRebrKey);
	
	CArray<T_REBR_K,T_REBR_K> aRebrK;
	aRebrK.SetSize(nSelected);
	for(int i = 0; i < nSelected; i++)
	{
		aRebrK[i].keymap = aRebrKey[i];
	}

	T_REBR_K RebrK;   RebrK.keymap = RebrKey;
	BOOL bEnableChk = RebrK.key.subSectK == 0;
	if(nSelected > 1)   bEnableChk = FALSE;
	m_chkSubSect.EnableWindow(bEnableChk);

	if(!bEnableChk)  m_chkSubSect.SetCheck(FALSE);
	OnChkSubSection();

	if(pDoc->m_pViewCtrl->SelectedElemCount()) pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTempK;
	for(int k = 0; k < nSelected; k++)
	{
		int nCount = pDoc->m_pAttrCtrl->GetElemKeyListUseRebr(aRebrK[k], aTempK);

		for(int i=nCount-1; i>= 0; i--)
		{
			int nMembType = pDoc->m_pAttrCtrl->GetMemberType(aTempK[i]);
			if(nMembType != D_MBTP_BRACE)        aTempK.RemoveAt(i);
		}
		aElemK.Append(aTempK);
	}
	pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

	T_SECT_K SectK = RebrK.key.sectK;
	T_SECT_D SectD;   SectD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD))	return;
	m_strShape = SectD.SectBefore.Shape;
	if (!pDoc->m_pAttrCtrl->GetRebr(RebrK, RebrD))
	{
		RebrD.Initialize();
		CDgnDataCtrl DataCtrl;
		T_DCRB_D DcrbD;    DcrbD.Initialize();
		DataCtrl.Get_DgnConDcrb(0, DcrbD);
		
		RebrD.MainRebarName = DcrbD.MainRebarBrace[0];
		RebrD.SubRebarName = DcrbD.MainRebarBrace[0];
		RebrD.SubRebarName2 = DcrbD.MainRebarBrace[0];
		
		if(m_strShape==_LSX(SB))
		{
			if(RebrD.nQrb < 4)        RebrD.nQrb = 4;
			if(RebrD.nRow < 2)        RebrD.nRow = 2;
		}
		else if(m_strShape==_LSX(SR) || m_strShape==_LSX(P)) 
		{
			if(RebrD.nQrb < 6)    RebrD.nQrb = 6;
			if(RebrD.nRow < 3)    RebrD.nRow = 3;
		}
		if(RebrD.nHoopType == 0)    RebrD.nHoopType = 1;

		RebrD.dConcCenter = Get_SectionDimension(SectD);
	}

	m_RebarGrid.Data2Grid(&RebrD, &m_strShape);

	m_edtD0.SetEditUnit(RebrD.dConcCenter);

	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_TIES_RDO);
	aCtrls.Add(IDC_DGN_SPIRALS_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aCtrls, RebrD.nHoopType - 1);

	ShowBitmap(m_strShape);

	Initial_SectionData(SectD);
	DrawSectionWnd(&RebrD);

	*pResult = 0;
}

void CDgnConBraceNewDlg::SetListCtrlHeader()
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

BOOL CDgnConBraceNewDlg::GetSelectedItem(T_REBR_KEY& RebrKey)
{
	int iItem = m_SectList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;
	
	int nIndex = iItem;
	if(!m_mIndex2RebrK.Lookup(nIndex, RebrKey))     {  ASSERT(0);		return FALSE;	}

	return TRUE;
}

int CDgnConBraceNewDlg::GetSelectedItems(CArray<T_REBR_KEY,T_REBR_KEY>& aRebrKey)
{
	aRebrKey.RemoveAll();

	POSITION pos = m_SectList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		while (pos)
		{
			int nItem = m_SectList.GetNextSelectedItem(pos);
			
			int nIndex = nItem;
			T_REBR_KEY RebrKey;
			if(!m_mIndex2RebrK.Lookup(nIndex, RebrKey))     {  ASSERT(0);		return FALSE;	}

			aRebrKey.Add(RebrKey);
		}
	}

	return aRebrKey.GetSize();
}

void CDgnConBraceNewDlg::MakeItemEx()
{
	m_SectList.DeleteAllItems();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMapEx<int,int,int,int> mSectK4Brace;

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
				if(nMembType == D_MBTP_BRACE && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("SR") ||
															SectData.SectBefore.Shape == _T("P"))) 
					mSectK4Brace.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	CArray<int,int> aSectK;
	int nSect = mSectK4Brace.GetKeyList(aSectK);
	qsort((void*)aSectK.GetData(), nSect, sizeof(int), CCompFunc::UINTAsc);

	m_mIndex2RebrK.RemoveAll();
	m_mRebrK2Index.RemoveAll();
	int SectK, nIndex = 0;

	int nInitIndex = 0;

	for(int i = 0; i < nSect; i++)
	{
		SectK = aSectK[i];
		m_mIndex2RebrK.SetAt(nIndex, SectK);
		m_mRebrK2Index.SetAt(SectK, nIndex);

		T_SECT_D SectD;     SectD.Initialize();
		pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD);

//    m_SectList.InsertItem(nIndex, _T(""));
		if(pDoc->m_pAttrCtrl->ExistRebr(SectK))    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(In)));
		else    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(-)));

		CArray<T_REBR_K, T_REBR_K> aRebrK;
		int nCountRebr = pDoc->m_pAttrCtrl->GetRebrKeyListUsingSectK(SectK, aRebrK);
		for(int k = 0; k < nCountRebr; k++)
		{
			T_REBR_K RebrK = aRebrK[k];
			m_mIndex2RebrK.SetAt(nIndex, RebrK.keymap);
			m_mRebrK2Index.SetAt(RebrK.keymap, nIndex);

			SetItem(nIndex++, RebrK.key.sectK, RebrK.key.subSectK, SectD.SName, CString(_LSX(In)));
		}
	}

//  m_SectList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CDgnConBraceNewDlg::SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar)
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

void CDgnConBraceNewDlg::UpdateSelectedMembEdit()
{
	CArray<T_MEMB_K,T_MEMB_K> aMembK;
	int nCount = GetSelectedMember(aMembK);

	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.OptimizeUnsortedArray(aMembK);
	m_edtMemb.SetWindowText(sOptimizedString);
}

int CDgnConBraceNewDlg::GetSelectedMember(CArray<UINT,UINT>& aMembK)
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

UINT CDgnConBraceNewDlg::GetNextSubSectionKey(UINT SectK)
{
	UINT nSubSectK = 1;
	T_REBR_K RebrK;   RebrK.key.sectK = SectK;      RebrK.key.subSectK = nSubSectK;
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	while(pDoc->m_pAttrCtrl->ExistRebr(RebrK))
	{  
		nSubSectK++;
		RebrK.key.sectK = SectK;      RebrK.key.subSectK = nSubSectK;
	}

	return nSubSectK;
}
