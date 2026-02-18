// DgnConBeamNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamNewDlg.h"

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
// CDgnConBeamNewDlg dialog
CDgnConBeamNewDlg::CDgnConBeamNewDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnConBeamNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aSameValRdo.Add(IDC_DGN_ALL_SECT_RDO);
	m_aSameValRdo.Add(IDC_DGN_BOTH_CENT_RDO);
	m_aSameValRdo.Add(IDC_DGN_EACH_CENT_RDO);

	m_DtNo = 0.0;
	m_DbNo = 0.0;
	m_Hc = 0.0;
	m_Bc = 0.0;
	m_Hf = 0.0;
	m_Bf = 0.0;
	m_Space = 0.0;

	m_bInit = FALSE;
}

CDgnConBeamNewDlg::~CDgnConBeamNewDlg()
{
}

void CDgnConBeamNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamNewDlg)
	DDX_Control(pDX, IDC_DGN_SECT_LST, m_SectList);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_CHK, m_chkSubSect);
	DDX_Control(pDX, IDC_DGN_SECT_KEY_EDT, m_edtSectK);
	DDX_Control(pDX, IDC_DGN_SUB_SECT_EDT, m_edtSubSectK);
	DDX_Control(pDX, IDC_DGN_MEMB_LIST_EDT, m_edtMemb);
	DDX_Control(pDX, IDC_DGN_CONC_DT_EDT, m_edtDT);
	DDX_Control(pDX, IDC_DGN_CONC_DB_EDT, m_edtDB);
	DDX_Control(pDX, IDC_DGN_CONC_FACE_EDT, m_untDTDB);
	DDX_Control(pDX, IDC_DGN_SAME_TOB_BOT_CHK, m_chkSameSizeTopBot);
	DDX_Control(pDX, IDC_DGN_SAME_IMJ_CHK, m_chkSameSizeIMJ);
	DDX_Control(pDX, IDC_DGN_SAME_LAYER_CHK, m_chkSameSizeLayer);
	DDX_Control(pDX, IDC_DGN_DETAIL_FIGURE_STC, m_grbDetail);
	DDX_Control(pDX, IDC_DGN_INODE_STC, m_INodeWnd);
	DDX_Control(pDX, IDC_DGN_CENT_STC, m_CenterWnd);
	DDX_Control(pDX, IDC_DGN_JNODE_STC, m_JNodeWnd);
	DDX_Control(pDX, IDC_DGN_REBAR_DATA_GRD, m_RebarGrid);
	DDX_Control(pDX, IDC_DGN_BEAMRECT_BIT, m_wndRect);
	DDX_Control(pDX, IDC_DGN_BEAMTEE_BIT, m_wndTee);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CDgnConBeamNewDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConBeamNewDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_SECT_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_SUB_SECT_CHK, OnChkSubSection)
	ON_BN_CLICKED(IDC_DGN_ALL_SECT_RDO, OnSectionRdo)
	ON_BN_CLICKED(IDC_DGN_BOTH_CENT_RDO, OnSectionRdo)
	ON_BN_CLICKED(IDC_DGN_EACH_CENT_RDO, OnSectionRdo)
	ON_BN_CLICKED(IDC_DGN_SAME_TOB_BOT_CHK, OnChkSameRebarSize)
	ON_BN_CLICKED(IDC_DGN_SAME_IMJ_CHK, OnChkSameRebarSize)
	ON_BN_CLICKED(IDC_DGN_SAME_LAYER_CHK, OnChkSameRebarSize)
	ON_EN_CHANGE(IDC_DGN_CONC_DT_EDT, OnChangeDtDb)
	ON_EN_CHANGE(IDC_DGN_CONC_DB_EDT, OnChangeDtDb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SECT_LST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_ADD_REP_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_DELETE_BTN, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnConBeamNewDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDgnDlgBase::Create(nIDTemplate, pParentWnd);
}

void CDgnConBeamNewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BOOL CDgnConBeamNewDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDgnConBeamNewDlg::UpdateBuffer()
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
	T_REBB_K RebbK;   RebbK.keymap = 0;
	T_REBB_D RebbD;
	
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
		case(UR_REBB_ADD):
		case(UR_REBB_DEL):  bChange = TRUE;   break;
		case(UR_REBB_MFS):
		case(UR_REBB_MFD):  bModify = TRUE;   break;
		default:						break;
		}

		if(nCmd == UR_REBB_ADD)
			pViewBuff->GetRebb(nKey, RebbK, RebbD);
	}

	if(bChange)
	{
		CListCtrlScrollMaintainer lsm(&m_SectList);

		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
		if(RebbK.keymap)    ShowSelectedRebb(RebbK.keymap, FALSE);
	}
	if(bModify)
	{
		LRESULT Result;
		OnClkListItem(NULL, &Result);
	}
}

void CDgnConBeamNewDlg::ShowSelectedRebb(T_REBB_KEY RebbKey, BOOL bInit)
{
	int nIndex = 0;
	if(!m_mRebbK2Index.Lookup(RebbKey, nIndex))     {  ASSERT(0);		return ;	}

	if(!bInit)   MakeItemEx();
	// Unselect current selection
	m_SectList.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	m_SectList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
//  m_SectList.SetSelectionMark(nIndex);

	LRESULT Result;
	OnClkListItem(NULL, &Result);
	m_SectList.SetFocus();
}

void CDgnConBeamNewDlg::ShowBitmap(CString str)
{
	if(str==_LSX(SB))
	{
		m_wndRect.ShowWindow(SW_SHOW);
		m_wndTee.ShowWindow(SW_HIDE);
	}
	else if(str==_LSX(T))
	{
		m_wndRect.ShowWindow(SW_HIDE);
		m_wndTee.ShowWindow(SW_SHOW);
	}
}

void CDgnConBeamNewDlg::DrawSectionWnd(T_REBB_D* pRebbD)
{
	int nShape = 0;
	if(m_strShape == _LSX(SB))    nShape = eDGN_RECT;
	else if(m_strShape == _LSX(T))    nShape = eDGN_TEE;

	BOOL bSameLayer = m_chkSameSizeLayer.GetCheck();

	BOOL bUseSkin = m_RebarGrid.UseSkin();

	m_INodeWnd.Beam_DrawData(nShape,m_Hc,m_Bc,m_Hf,m_Bf,1,1, pRebbD->Bot_I.nRebar1,pRebbD->Bot_I.nRebar2,
		pRebbD->Top_I.nRebar1,pRebbD->Top_I.nRebar2,m_DtNo,m_DbNo, (bUseSkin ? pRebbD->Top_I.nSkin : 0), !bSameLayer);
	m_CenterWnd.Beam_DrawData(nShape,m_Hc,m_Bc,m_Hf,m_Bf,1,1, pRebbD->Bot_C.nRebar1,pRebbD->Bot_C.nRebar2,
		pRebbD->Top_C.nRebar1,pRebbD->Top_C.nRebar2,m_DtNo,m_DbNo, (bUseSkin ? pRebbD->Top_C.nSkin : 0), !bSameLayer);
	m_JNodeWnd.Beam_DrawData(nShape,m_Hc,m_Bc,m_Hf,m_Bf,1,1, pRebbD->Bot_J.nRebar1,pRebbD->Bot_J.nRebar2,
		pRebbD->Top_J.nRebar1,pRebbD->Top_J.nRebar2,m_DtNo,m_DbNo, (bUseSkin ? pRebbD->Top_J.nSkin : 0), !bSameLayer);

	m_INodeWnd.Invalidate();
	m_CenterWnd.Invalidate();
	m_JNodeWnd.Invalidate();
}

void CDgnConBeamNewDlg::Initial_SectionData(T_SECT_D& SectD)
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
	
	m_DbNo = m_edtDB.GetEditValue();
	m_DtNo = m_edtDT.GetEditValue();
	if(m_DbNo==0)	m_DbNo = Get_SectionDimension(SectD);
	if(m_DtNo==0)	m_DtNo = Get_SectionDimension(SectD);
}

double CDgnConBeamNewDlg::Get_SectionDimension(T_SECT_D& SectD)
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

double CDgnConBeamNewDlg::Get_ChangeUnitFactor()
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
// CDgnConBeamNewDlg message handlers

BOOL CDgnConBeamNewDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	InitCtrls();
	EnableDisableCtrls();

	Data2Dlg();

	m_bInit = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamNewDlg::OnChkSubSection()
{
	BOOL bCheck = m_chkSubSect.GetCheck();
	CArray<UINT,UINT> aCtrls;
	aCtrls.Add(IDC_DGN_SECT_KEY_EDT);
	aCtrls.Add(IDC_DGN_DASH_STC);
	aCtrls.Add(IDC_DGN_SUB_SECT_EDT);
	CDlgUtil::CtrlEnableDisable(this, aCtrls, bCheck);

//  m_edtMemb.SetReadOnly(!bCheck);

	T_REBB_KEY RebbKey;
	if(!GetSelectedItem(RebbKey))    return;
	
	T_REBB_K RebbK;   RebbK.keymap = RebbKey;
	BOOL bShow = RebbK.key.subSectK == 0?   TRUE:   FALSE;

	CString strTemp;
	if(bCheck && bShow)
	{
		int SectK = RebbK.key.sectK;
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

	CArray<T_REBB_KEY,T_REBB_KEY> aRebbKey;
	int nSelected = GetSelectedItems(aRebbKey);

	BOOL bEdtMemb = TRUE;
	if(nSelected > 1 || (bShow && !bCheck))    bEdtMemb = FALSE;
	m_edtMemb.SetReadOnly(!bEdtMemb);
}

void CDgnConBeamNewDlg::OnSectionRdo()
{
	int nArrangeType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aSameValRdo, nArrangeType);
	
	T_REBB_D RebbD;
	m_RebarGrid.Grid2Data(&RebbD);

	m_RebarGrid.Initialize(this, nArrangeType);
	OnChkSameRebarSize(); 

//   LRESULT Result;
//   OnClkListItem(NULL, &Result);

	m_RebarGrid.Data2Grid(&RebbD);
	m_RebarGrid.Grid2Data(&RebbD);
	DrawSectionWnd(&RebbD);
}

void CDgnConBeamNewDlg::OnChkSameRebarSize()
{
	BOOL bSameTopBot = m_chkSameSizeTopBot.GetCheck();
	BOOL bSameIMJ = m_chkSameSizeIMJ.GetCheck();
	// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만)
	BOOL bSameLayer = m_chkSameSizeLayer.GetCheck();
	
//   T_REBB_D RebbD;
//   m_RebarGrid.Grid2Data(&RebbD);

	m_RebarGrid.SetSameSizeSetting(bSameTopBot, bSameIMJ, bSameLayer);
	
//   LRESULT Result;
//   OnClkListItem(NULL, &Result);

	T_REBB_D RebbD;
	m_RebarGrid.Grid2Data(&RebbD);
	DrawSectionWnd(&RebbD);
//  m_RebarGrid.Data2Grid(&RebbD);
}

void CDgnConBeamNewDlg::OnChangeDtDb()
{
	m_DbNo = m_edtDB.GetEditValue();
	m_DtNo = m_edtDT.GetEditValue();

	T_REBB_D RebbD;
	m_RebarGrid.Grid2Data(&RebbD);
	DrawSectionWnd(&RebbD);
}

void CDgnConBeamNewDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)  OnClkListItem(pNMHDR, pResult);
	
	*pResult = 0;
}

void CDgnConBeamNewDlg::OnDgnAddReplace()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_REBB_D RebbD;   RebbD.Initialize();
	if(!Dlg2Data(&RebbD))   return;

	T_REBB_KEY RebbKey;
	if(!GetSelectedItem(RebbKey))    return ;
	T_REBB_K RebbK;   RebbK.keymap = RebbKey;    

	CArray<T_REBB_KEY,T_REBB_KEY> aRebbKey;
	int nRebbSize = GetSelectedItems(aRebbKey);

	if(nRebbSize == 1 && (m_chkSubSect.GetCheck() || RebbK.key.subSectK))
	{
		int nMrebSize = m_aMrebK.GetSize();
//     if(nMrebSize < 1)
//     {
//       AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
//       return;
//     }

		if(!RebbK.key.subSectK)
		{
			CString strTemp;
			m_edtSubSectK.GetWindowText(strTemp);
			int nSubSectK = _ttoi(strTemp);
			RebbK.key.subSectK = nSubSectK;
		}

		CArray<T_REBB_K,T_REBB_K> aRebbK;   aRebbK.Add(RebbK);
		CArray<T_REBB_D,T_REBB_D&> aRebbD;   aRebbD.Add(RebbD);

		T_MREB_D MrebD;   MrebD.RebbK = RebbK;
		MrebD.nMembType = D_MBTP_BEAM;
		CArray<T_MREB_D,T_MREB_D&> aMrebD;
		
		aMrebD.SetSize(nMrebSize);
		for(int i = 0; i < nMrebSize; i++)    aMrebD[i] = MrebD;

		if(!pDoc->m_pDataCtrl->AddRebbMreb(aRebbK, aRebbD, m_aMrebK, aMrebD))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(nRebbSize < 1)   return;

		CArray<T_REBB_K,T_REBB_K> aRebbK;
		aRebbK.SetSize(nRebbSize);
		for(int i = 0; i < nRebbSize; i++)
		{
			aRebbK[i].keymap = aRebbKey[i];
		}

		if(!pDoc->m_pDataCtrl->AddRebb(aRebbK, RebbD))     {  ASSERT(0);		return ;	}
	}
}

void CDgnConBeamNewDlg::OnDgnDelete()
{
	CArray<T_REBB_KEY,T_REBB_KEY> aRebbKey;
	int nRebbSize = GetSelectedItems(aRebbKey);
	if(nRebbSize < 1)   return;
	
	CArray<T_REBB_K,T_REBB_K> aRebbK;
	aRebbK.SetSize(nRebbSize);
	for(int i = 0; i < nRebbSize; i++)
	{
		aRebbK[i].keymap = aRebbKey[i];
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if(!pDoc->m_pDataCtrl->DelRebb(aRebbK))     {  ASSERT(0);		return ;	}
}

void CDgnConBeamNewDlg::OnDgnClose()
{
	m_RebarGrid.DestroyWindow();
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CDgnConBeamNewDlg::Data2Dlg()
{
	MakeItemEx();

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

BOOL CDgnConBeamNewDlg::Dlg2Data(T_REBB_D* pRebbD)
{
	m_RebarGrid.Grid2Data(pRebbD);
	pRebbD->dT = m_edtDT.GetEditValue();
	pRebbD->dB = m_edtDB.GetEditValue();
	
	if(pRebbD->dT <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_DT_VALUE),MB_OK);
		return FALSE;
	}
	if(pRebbD->dB <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_DB_VALUE),MB_OK);
		return FALSE;
	}

	pRebbD->bSameSizeTopBot = m_chkSameSizeTopBot.GetCheck();
	pRebbD->bSameSizeIMJ = m_chkSameSizeIMJ.GetCheck();
	pRebbD->bSameSizeLayer = m_chkSameSizeLayer.GetCheck();

	T_REBB_KEY RebbKey;
	if(!GetSelectedItem(RebbKey))    return FALSE;
	T_REBB_K RebbK;   RebbK.keymap = RebbKey;

	CArray<T_REBB_KEY,T_REBB_KEY> aRebbKey;
	int nRebbSize = GetSelectedItems(aRebbKey);

	if(nRebbSize == 1 && (m_chkSubSect.GetCheck() || RebbK.key.subSectK))
	{
		CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
		T_ELEM_D ElemD;
		T_MEMB_D MembD;

		CArray<T_ELEM_K,T_ELEM_K> aElemK;
//    int nCount = GetSelectedMember(aMembK);
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
		int nCount = aElemK.GetSize();
		T_SECT_K SectK = RebbK.key.sectK;

		m_aMrebK.RemoveAll();
		for(int i = 0; i < nCount; i++)
		{
			if(pDoc->m_pAttrCtrl->GetMemberType(aElemK[i]) != D_MBTP_BEAM)   continue;
			if(!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))     {  ASSERT(0);		continue ;	}
			if(ElemD.elpro != SectK)    continue;
//       if(pDoc->m_pAttrCtrl->GetMemb(aMembK[i], MembD))    m_aMrebK.Append(MembD.aElemList);
//       else  m_aMrebK.Add(aMembK[i]);
			m_aMrebK.Add(aElemK[i]);
		}
	}

	return TRUE;
}

void CDgnConBeamNewDlg::InitCtrls()
{
	SetListCtrlHeader();

	OnChkSubSection();

	m_edtMemb.Connect(SC_ID_ELEM, &m_edtMemb);
	ShowBitmap(_LSX(SB));

	CDlgUtil::CtrlRadioSetCheck(this, m_aSameValRdo, 2);

	CRect rect;
	GetDlgItem(IDC_DGN_REBAR_DATA_GRD)->GetWindowRect(rect);

	m_RebarGrid.SetGridWndSize(rect.Width(), rect.Height());
	m_RebarGrid.Initialize(this);

	m_grbDetail.InitControl(this, CDgnConBeamNewDlg::IDD, IDC_DGN_DETAIL_FIGURE_STC);

	m_edtDT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDTDB.SetUnitType(D_UNITSYS_BASE_LENGTH);

//   CWnd* PlaceHolder0 = GetDlgItem(IDC_DGN_INODE_STC);   ASSERT(PlaceHolder0);
//	m_INodeWnd.Initial(PlaceHolder0);
	m_INodeWnd.m_SectionType = eDGN_BEAM;

//  PlaceHolder0 = GetDlgItem(IDC_DGN_CENT_STC);   ASSERT(PlaceHolder0);
//	m_CenterWnd.Initial(PlaceHolder0);
	m_CenterWnd.m_SectionType = eDGN_BEAM;

//  PlaceHolder0 = GetDlgItem(IDC_DGN_JNODE_STC);   ASSERT(PlaceHolder0);
//	m_JNodeWnd.Initial(PlaceHolder0);
	m_JNodeWnd.m_SectionType = eDGN_BEAM;

	m_chkSameSizeTopBot.SetCheck(TRUE);
	m_chkSameSizeIMJ.SetCheck(TRUE);
	m_chkSameSizeLayer.SetCheck(TRUE);
	OnChkSameRebarSize();

	m_wndRect.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam_rect.svg"));
	m_wndTee.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_beam_tee.svg"));

}

void CDgnConBeamNewDlg::EnableDisableCtrls()
{

}

// List Ctrl 함수들
void CDgnConBeamNewDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_REBB_D RebbD;
	
	T_REBB_KEY RebbKey;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		if(!m_mIndex2RebbK.Lookup(nIndex, RebbKey))     {  ASSERT(0);		return ;	}
	}
	else
	{
		if(!GetSelectedItem(RebbKey))    return;
	}

	CArray<T_REBB_KEY,T_REBB_KEY> aRebbKey;
	int nSelected = GetSelectedItems(aRebbKey);
	
	CArray<T_REBB_K,T_REBB_K> aRebbK;
	aRebbK.SetSize(nSelected);
	for(int i = 0; i < nSelected; i++)
	{
		aRebbK[i].keymap = aRebbKey[i];
	}

	T_REBB_K RebbK;   RebbK.keymap = RebbKey;
	BOOL bEnableChk = RebbK.key.subSectK == 0;
	if(nSelected > 1)   bEnableChk = FALSE;
	m_chkSubSect.EnableWindow(bEnableChk);
	
	if(!bEnableChk)  m_chkSubSect.SetCheck(FALSE);
	OnChkSubSection();

	T_SECT_K SectK = RebbK.key.sectK;
	T_SECT_D SectD;   SectD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD))	return;

	if (!pDoc->m_pAttrCtrl->GetRebb(RebbK, RebbD))
	{
		RebbD.Initialize();
		CDgnDataCtrl DataCtrl;
		T_DCRB_D DcrbD;    DcrbD.Initialize();
		DataCtrl.Get_DgnConDcrb(0, DcrbD);

		RebbD.Top_I.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Top_J.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Top_C.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Bot_I.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Bot_J.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Bot_C.RebarName = DcrbD.MainRebarBeam[0];
		RebbD.Top_I.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.Top_J.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.Top_C.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.Bot_I.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.Bot_J.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.Bot_C.RebarName2nd = DcrbD.MainRebarBeam[0];
		RebbD.SubRebarName = DcrbD.StirrupBarBeam;
//    RebbD.strSkinName = DcrbD.MainRebarBeam[0];

		RebbD.dT = Get_SectionDimension(SectD);
		RebbD.dB = Get_SectionDimension(SectD);
	}

	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTempK;
	for(int k = 0; k < nSelected; k++)
	{
		int nCount = pDoc->m_pAttrCtrl->GetAllElemKeyListUseRebb(aRebbK[k], aTempK);

		for(int i=nCount-1; i>= 0; i--)
		{
			int nMembType = pDoc->m_pAttrCtrl->GetMemberType(aTempK[i]);
			if(nMembType != D_MBTP_BEAM)        aTempK.RemoveAt(i);
		}
		aElemK.Append(aTempK);
	}
	pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

	m_RebarGrid.Data2Grid(&RebbD);
	m_chkSameSizeTopBot.SetCheck(RebbD.bSameSizeTopBot);
	m_chkSameSizeIMJ.SetCheck(RebbD.bSameSizeIMJ);
	m_chkSameSizeLayer.SetCheck(RebbD.bSameSizeLayer);
	OnChkSameRebarSize();

	m_edtDT.SetEditUnit(RebbD.dT);
	m_edtDB.SetEditUnit(RebbD.dB);

	m_strShape = SectD.SectBefore.Shape;
	ShowBitmap(SectD.SectBefore.Shape);

	Initial_SectionData(SectD);
	DrawSectionWnd(&RebbD);

	*pResult = 0;
}

void CDgnConBeamNewDlg::SetListCtrlHeader()
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

BOOL CDgnConBeamNewDlg::GetSelectedItem(T_REBB_KEY& RebbKey)
{
	int iItem = m_SectList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;
	
	int nIndex = iItem;
	if(!m_mIndex2RebbK.Lookup(nIndex, RebbKey))     {  ASSERT(0);		return FALSE;	}

	return TRUE;
}

int CDgnConBeamNewDlg::GetSelectedItems(CArray<T_REBB_KEY,T_REBB_KEY>& aRebbKey)
{
	aRebbKey.RemoveAll();

	POSITION pos = m_SectList.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		while (pos)
		{
			int nItem = m_SectList.GetNextSelectedItem(pos);
			
			int nIndex = nItem;
			T_REBB_KEY RebbKey;
			if(!m_mIndex2RebbK.Lookup(nIndex, RebbKey))     {  ASSERT(0);		return FALSE;	}

			aRebbKey.Add(RebbKey);
		}
	}

	return aRebbKey.GetSize();
}

void CDgnConBeamNewDlg::MakeItemEx()
{
	m_SectList.DeleteAllItems();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMapEx<int,int,int,int> mSectK4Beam;

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
			// 1=column, 2=beam, 3=brace, 0=기타
			int nMembType = pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);
			
			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);
			
			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T"))) 
					mSectK4Beam.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	CArray<int,int> aSectK;
	int nSect = mSectK4Beam.GetKeyList(aSectK);
	qsort((void*)aSectK.GetData(), nSect, sizeof(int), CCompFunc::UINTAsc);

	m_mIndex2RebbK.RemoveAll();
	m_mRebbK2Index.RemoveAll();
	int SectK, nIndex = 0;

	int nInitIndex = 0;

	for(int i = 0; i < nSect; i++)
	{
		SectK = aSectK[i];
		m_mIndex2RebbK.SetAt(nIndex, SectK);
		m_mRebbK2Index.SetAt(SectK, nIndex);

		T_SECT_D SectD;     SectD.Initialize();
		pDoc->m_pAttrCtrl->GetSectDesign(SectK,SectD);

//    m_SectList.InsertItem(nIndex, _T(""));
		if(pDoc->m_pAttrCtrl->ExistRebb(SectK))    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(In)));
		else    SetItem(nIndex++, SectK, 0, SectD.SName, CString(_LSX(-)));

		CArray<T_REBB_K, T_REBB_K> aRebbK;
		int nCountRebb = pDoc->m_pAttrCtrl->GetRebbKeyListUsingSectK(SectK, aRebbK);
		for(int k = 0; k < nCountRebb; k++)
		{
			T_REBB_K RebbK = aRebbK[k];
			m_mIndex2RebbK.SetAt(nIndex, RebbK.keymap);
			m_mRebbK2Index.SetAt(RebbK.keymap, nIndex);

			SetItem(nIndex++, RebbK.key.sectK, RebbK.key.subSectK, SectD.SName, CString(_LSX(In)));
		}
	}

//  m_SectList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CDgnConBeamNewDlg::SetItem(int nIndex, UINT SectK, UINT SubSectK, CString& strSectName, CString& strRebar)
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

void CDgnConBeamNewDlg::UpdateSelectedMembEdit()
{
	CArray<T_MEMB_K,T_MEMB_K> aMembK;
	int nCount = GetSelectedMember(aMembK);

	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.OptimizeUnsortedArray(aMembK);
	m_edtMemb.SetWindowText(sOptimizedString);
}

int CDgnConBeamNewDlg::GetSelectedMember(CArray<UINT,UINT>& aMembK)
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

UINT CDgnConBeamNewDlg::GetNextSubSectionKey(UINT SectK)
{
	UINT nSubSectK = 1;
	T_REBB_K RebbK;   RebbK.key.sectK = SectK;      RebbK.key.subSectK = nSubSectK;
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	while(pDoc->m_pAttrCtrl->ExistRebb(RebbK))
	{  
		nSubSectK++;
		RebbK.key.sectK = SectK;      RebbK.key.subSectK = nSubSectK;
	}

	return nSubSectK;
}
