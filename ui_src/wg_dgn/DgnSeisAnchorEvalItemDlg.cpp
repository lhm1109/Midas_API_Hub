// DgnSeisAnchorEvalItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\InitCtrl.h"

#include "../wg_main/wg_main.h"

#include "..\wg_dgn\DgnSeisAnchorFailDlg.h"
#include "..\wg_dgn\DgnSeisAnchorRebarDlg.h"
#include "..\wg_dgn\DgnSeisAnchorSupportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemDlg dialog

// 910 부터 사용안함.
CDgnSeisAnchorEvalItemDlg::CDgnSeisAnchorEvalItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorEvalItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

// 	m_strSax = _T("");
// 	m_strSay = _T("");
// 	m_bRebar = FALSE;
// 	m_bSupport = FALSE;
// 	m_nViewType = 1;
// 	
// 	m_pDoc = CDBDoc::GetDocPoint();
// 
// 	m_Key = 0;
// 	m_Data.Initialize();
// 	m_bModify = FALSE;
// 	m_bSetEmf = FALSE;
}

// void CDgnSeisAnchorEvalItemDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialogMove::DoDataExchange(pDX);
// 	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemDlg)
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_NAME_EDT,              m_edtName);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,      m_cmbGroup);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_EDT,        m_edtfck);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_UNT,        m_untfck);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,       m_edtHcop);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_UNT,       m_untHcop);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_CMB,      m_cmbProp);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_NUM_EDT,               m_edtNum);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_EDT,                 m_edtB);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_UNT,                 m_untB);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_EDT,                 m_edtH);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_UNT,                 m_untH);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT,           m_edtTheta1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_UNT,           m_untTheta1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT,           m_edtTheta2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_UNT,           m_untTheta2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_EDT,          m_edtThetaq1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_UNT,          m_untThetaq1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_EDT,          m_edtThetaq2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_UNT,          m_untThetaq2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_EDT,                m_edtD1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_UNT,                m_untD1);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_EDT,              m_edtD1q);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_UNT,              m_untD1q);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_N1_EDT,                m_edtN1);
// 	DDX_Text   (pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAX_EDT,               m_strSax);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAX_UNT,               m_untSax);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_EDT,                m_edtD2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_UNT,                m_untD2);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_EDT,              m_edtD2q);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_UNT,              m_untD2q);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_N2_EDT,                m_edtN2);
// 	DDX_Text   (pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAY_EDT,               m_strSay);
// 	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAY_UNT,               m_untSay);
// 	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,             m_bRebar);
// 	DDX_Check  (pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_CHK,           m_bSupport);
// 	DDX_Radio  (pDX, IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_DRAW_RDO,         m_nViewType);	
// 	//}}AFX_DATA_MAP
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
// {
// 	HWND hWnd;
// 	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
// 
// 	switch(lHint)
// 	{
// 	case D_UPDATE_DBALL:
// 	case D_UPDATE_DEFAULT:
// 		UpdateBuffer();
// 		break;
// 	case D_UPDATE_BUFFER_BEFORE:
// 		break;
// 	case D_UPDATE_BUFFER_AFTER:
// 		UpdateBuffer();
// 		break;
// 	case D_UPDATE_UNIT:
// 		break;
// 	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
// 		break;
// 	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
// 		break;
// 	default:
// 		//ASSERT(FALSE);
// 		break;
// 	}
// }
// 
// void CDgnSeisAnchorEvalItemDlg::UpdateBuffer()
// {
// 	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
// 	int nCount = pViewBuff->GetCount();
// 	if (nCount == 0) return; 
// 	ASSERT(nCount == 1);
// 
// 	T_UDRD_BUFFER buffer_ur;
// 	POSITION pos = pViewBuff->GetStartBuffer();
// 
// 	//T_ANEV_K Key,  KeyBack;
// 	T_ANEV_D Data, DataBack;
// 
// 	BOOL bMFD = FALSE;
// 	while(pos != NULL)
// 	{
// 		buffer_ur = pViewBuff->GetNextBuffer(pos);
// 
// 		int nCmd = buffer_ur.nCmd;
// 		int nKey = buffer_ur.nKey;
// 
// 		switch(nCmd)
// 		{
// 		case(UR_BREV_ADD):
// 		case(UR_BREV_DEL):
// 		case(UR_BREV_MFD):
// 		case(UR_BREV_MFS):
// 		case(UR_BRPR_ADD):
// 		case(UR_BRPR_DEL):
// 		case(UR_BRPR_MFD):
// 		case(UR_BRPR_MFS):
// 			InitCombo();
// 			UpdateName();
// 			Calcfck();
// 			CalcHcop();
// 			CalcNum();
// 			SetViewData();
// 			break;
// 		default:
// 			break;
// 		}
// 	} // end of while
// }
// 
// void CDgnSeisAnchorEvalItemDlg::MakeEmfAndClose(CString strPath)
// {
// 	m_bSetEmf = TRUE;
// 	m_strPath = strPath;
// }
// 
// void CDgnSeisAnchorEvalItemDlg::InitUnit()
// {	
// 	m_edtfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
// 	m_untfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
// 	m_edtHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
// 	m_untHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
// 	m_edtNum.SetUnitType(D_UNITSYS_NONE);
// 	m_edtB.SetUnitType(D_UNITSYS_BASE_LENGTH);
// 	m_untB.SetUnitType(D_UNITSYS_BASE_LENGTH);
// 	m_edtH.SetUnitType(D_UNITSYS_BASE_LENGTH);
// 	m_untH.SetUnitType(D_UNITSYS_BASE_LENGTH);
// 	m_edtTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
// 	m_untTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
// 	m_edtTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
// 	m_untTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
// 	m_edtThetaq1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q1);
// 	m_untThetaq1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q1);
// 	m_edtThetaq2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q2);
// 	m_untThetaq2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q2);
// 	m_edtD1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1);
// 	m_untD1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1);	
// 	m_edtD1q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1_q);
// 	m_untD1q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1_q);
// 	m_edtN1.SetUnitType(D_UNITSYS_NONE);
// 	m_untSax.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dSax);	
// 	m_edtD2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2);
// 	m_untD2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2);	
// 	m_edtD2q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2_q);
// 	m_untD2q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2_q);
// 	m_edtN2.SetUnitType(D_UNITSYS_NONE);
// 	m_untSay.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dSay);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::InitCombo()
// {
// 	// Bearing Evaluation
// 	CArray<T_BREV_K, T_BREV_K> aBrevK;
// 	m_pDoc->m_pAttrCtrl2->GetBrevKeyList(aBrevK);
// 	
// 	T_BREV_D BrevD; BrevD.Initialize();
// 
// 	m_cmbGroup.ResetContent();
// 	for(int i=0; i<aBrevK.GetSize(); i++)
// 	{
// 		if(!m_pDoc->m_pAttrCtrl2->GetBrev(aBrevK[i], BrevD)) continue;
// 		m_cmbGroup.SetItemData(m_cmbGroup.AddString(BrevD.strName), aBrevK[i]);
// 	}
// 	m_cmbGroup.SetCurSel(0);
// 
// 	// Bearing Property
// 	CArray<T_BRPR_K, T_BRPR_K> aBrprK;
// 	m_pDoc->m_pAttrCtrl2->GetBrprKeyList(aBrprK);
// 
// 	T_BRPR_D BrprD; BrprD.Initialize();
// 
// 	m_cmbProp.ResetContent();
// 	for(int i=0; i<aBrprK.GetSize(); i++)
// 	{
// 		if(!m_pDoc->m_pAttrCtrl2->GetBrpr(aBrprK[i], BrprD)) continue;
// 		m_cmbProp.SetItemData(m_cmbProp.AddString(BrprD.strName), aBrprK[i]);
// 	}
// 	m_cmbProp.SetCurSel(0);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::InitView()
// {
// 	m_View.SubclassDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW, this);
// 	m_View.SetParent(this);
// 	m_View.Init();
// 	//m_View.SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
// 	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
// 	//m_View.SetContext(TRUE, TRUE);// 이거 주석 풀면 컨텍스트 메뉴 열림
// 	//m_View.SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
// 	m_View.MakeDrawUnit();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::AlignControl()
// {
// 	CRect rRef;
// 	CRect rToMove;
// 	int nDistX = 0;
// 	int nDistY = 0;	
// 
// 	CArray<UINT, UINT> aControls;
// 	aControls.Add(IDC_DGN_SEIS_ANCHOR_EVAL_BMP);
// 
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_BMP)->GetWindowRect(rToMove);
// 
// 	nDistY = rRef.top - rToMove.top;
// 	nDistX = rRef.left - rToMove.left;
// 
// 	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::ControlsShowHide()
// {
// 	UpdateData(TRUE);
// 
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW)->ShowWindow(m_nViewType == 0);
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_BMP)->ShowWindow(m_nViewType == 1);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::EnableDisableControls()
// {
// 	UpdateData(TRUE);
// 	
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN)->EnableWindow(m_bRebar);
// 	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_BTN)->EnableWindow(m_bSupport);
// 
// 	if(m_bModify) GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN)->EnableWindow(FALSE);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::Data2Dlg()
// {
// 	m_edtName.SetWindowText(m_Data.strName);
// 
// 	CArray<T_BREV_K, T_BREV_K> aBrevK;
// 	m_pDoc->m_pAttrCtrl2->GetBrevKeyList(aBrevK);
// 
// 	for(int i=0; i<aBrevK.GetSize(); i++)
// 	{
// 		if(aBrevK[i] == m_Data.BrevK) 
// 		{ 
// 			m_cmbGroup.SetCurSel(i); 
// 			break; 
// 		}
// 	}
// 
// 	m_edtfck.SetEditUnit(m_Data.dfck);
// 	m_edtHcop.SetEditUnit(m_Data.dHcop);
// 
// 	CArray<T_BRPR_K, T_BRPR_K> aBrprK;
// 	m_pDoc->m_pAttrCtrl2->GetBrprKeyList(aBrprK);
// 
// 	for(int i=0; i<aBrprK.GetSize(); i++)
// 	{
// 		if(aBrprK[i] == m_Data.BrprK) 
// 		{ 
// 			m_cmbProp.SetCurSel(i); 
// 			break; 
// 		}
// 	}
// 
// 	m_edtTheta1.SetEditUnit(m_Data.dTheta1);
// 	m_edtTheta2.SetEditUnit(m_Data.dTheta2);
// 	m_edtThetaq1.SetEditUnit(m_Data.dTheta_q1);
// 	m_edtThetaq2.SetEditUnit(m_Data.dTheta_q2);
// 	m_edtD1.SetEditUnit(m_Data.dD1);
// 	m_edtD1q.SetEditUnit(m_Data.dD1_q);
// 	m_strSax = CStrParser::ConvtDistArr2DistStr(m_Data.aSax); 
// 	m_edtD2.SetEditUnit(m_Data.dD2);
// 	m_edtD2q.SetEditUnit(m_Data.dD2_q);
// 	m_edtN2.SetEditUnit(m_Data.nN2);
// 	m_strSay = CStrParser::ConvtDistArr2DistStr(m_Data.aSay); 
// 	m_bRebar = m_Data.bRebar;
// 	m_bSupport = m_Data.bSuppType;
// 
// 	UpdateData(FALSE);
// 
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::SetViewData()
// {
// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 	T_BRPR_D BrprD;
// 	std::vector<std::vector<T_BRPR_D>> aArBrprD;
// 	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
// 	{
// 		aArBrprD.push_back(std::vector<T_BRPR_D>());
// 		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
// 		{
// 			if(!pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
// 			aArBrprD[i].push_back(BrprD);
// 		}
// 	}
// 
// 	m_View.SetData(aArBrprD, m_Data, FALSE);	
// 	m_View.SetDrawType(0, 0);
// }
// 
// BOOL CDgnSeisAnchorEvalItemDlg::Dlg2Data()
// {
// 	UpdateData(TRUE);
// 	
// 	m_edtName.GetWindowText(m_Data.strName);
// 
// 	m_Data.BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
// 	m_Data.dfck = m_edtfck.GetEditValue();
// 	m_Data.dHcop = m_edtHcop.GetEditValue();	
// 	m_Data.BrprK = m_cmbProp.GetItemData(m_cmbProp.GetCurSel());
// 	m_Data.dTheta1 = m_edtTheta1.GetEditValue();	
// 	m_Data.dTheta2 = m_edtTheta2.GetEditValue();	
// 	m_Data.dTheta_q1 = m_edtThetaq1.GetEditValue();	
// 	m_Data.dTheta_q2 = m_edtThetaq2.GetEditValue();	
// 	m_Data.dD1 = m_edtD1.GetEditValue();	
// 	m_Data.dD1_q = m_edtD1q.GetEditValue();	
// 	m_Data.dD2 = m_edtD2.GetEditValue();	
// 	m_Data.dD2_q = m_edtD2q.GetEditValue();	
// 
// 	BOOL bAllZero = TRUE;
// 
// 	CArray<double, double> aSax;
// 	CStrParser::GetUEDistance(m_strSax, aSax);
// 
// 	for (int i = 0; i < aSax.GetSize(); i++)
// 	{
// 		if (aSax[i] > 0.0)
// 		{
// 			bAllZero = FALSE;
// 		}
// 	}
// 
// 	if (bAllZero)
// 	{
// 		if (aSax.GetSize() > 0) aSax.RemoveAll();
// 	}
// 
// 	m_Data.aSax.RemoveAll();
// 	m_Data.aSax.Copy(aSax);
// 
// 	bAllZero = TRUE;
// 
// 	CArray<double, double> aSay;	
// 	CStrParser::GetUEDistance(m_strSay, aSay);	
// 
// 	for (int i = 0; i < aSay.GetSize(); i++)
// 	{
// 		if (aSay[i] > 0.0)
// 		{
// 			bAllZero = FALSE;
// 		}
// 	}
// 
// 	if (bAllZero)
// 	{
// 		if (aSay.GetSize() > 0) aSay.RemoveAll();
// 	}
// 
// 	m_Data.aSay.RemoveAll();	
// 	m_Data.aSay.Copy(aSay);
// 	
// 	m_Data.bRebar = m_bRebar;
// 	m_Data.bSuppType = m_bSupport;
// 
// 	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
// 	m_pDoc->m_pAttrCtrl2->SetAnevSupport(aSax, aSay, m_Data.SuppType, SuppType);
// 
// 	m_Data.SuppType.Initialize();
// 	m_Data.SuppType = SuppType;
// 	
// 	return TRUE;
// }
// 
// void CDgnSeisAnchorEvalItemDlg::UpdateName()
// {
// 	CString strName;
// 
// 	CString strGroup;
// 	CString strProp;
// 
// 	m_cmbGroup.GetWindowText(strGroup);
// 	m_cmbProp.GetWindowText(strProp);
// 	
// 	strName.Format(_T("%s_%s"), strGroup, strProp);
// 
// 	m_edtName.SetWindowText(strName);
// 
// 	UpdateData(FALSE);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::Calcfck()
// {
// 	T_BREV_K BrevK;
// 	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
// 
// 	T_BREV_D BrevD; BrevD.Initialize();
// 	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;
// 
// 	double dfck = m_pDoc->m_pAttrCtrl2->GetPierCapMinfck(BrevD.EvgpK);
// 	m_edtfck.SetEditUnit(dfck);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcHcop()
// {
// 	T_BREV_K BrevK;
// 	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
// 
// 	T_BREV_D BrevD; BrevD.Initialize();
// 	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;
// 	
// 	double dHcop = m_pDoc->m_pAttrCtrl2->GetPierCapMinH(BrevD.EvgpK);
// 	m_edtHcop.SetEditUnit(dHcop);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcNum()
// {
// 	UpdateData(TRUE);
// 
// 	T_BREV_K BrevK;
// 	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
// 
// 	T_BREV_D BrevD; BrevD.Initialize();
// 	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;
// 
// 	int nNum = BrevD.n2wFixed + BrevD.n2wMovable + BrevD.n1wLMovable + BrevD.n1wTMovable;
// 
// 	m_edtNum.SetEditUnit(nNum);
// 
// 	UpdateData(FALSE);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcB()
// {
// 	UpdateData(TRUE);
// 
// 	double dD1 = m_edtD1.GetEditValue();	
// 	double dD1_q = m_edtD1q.GetEditValue();	
// 
// 	CArray<double, double> aSax;
// 	CStrParser::GetUEDistance(m_strSax, aSax);
// 	if(m_strSax.IsEmpty()) aSax.RemoveAll();
// 
// 	double dB = dD1 + dD1_q;
// 	for(int i=0; i<aSax.GetSize(); i++)
// 	{
// 		dB += aSax[i];
// 	}
// 	
// 	m_edtB.SetEditUnit(dB);
// 
// 	UpdateData(FALSE);	
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcH()
// {
// 	UpdateData(TRUE);
// 
// 	double dD2 = m_edtD2.GetEditValue();	
// 	double dD2_q = m_edtD2q.GetEditValue();	
// 
// 	CArray<double, double> aSay;
// 	CStrParser::GetUEDistance(m_strSay, aSay);
// 	if(m_strSay.IsEmpty()) aSay.RemoveAll();
// 
// 	double dH = dD2 + dD2_q;
// 	for(int i=0; i<aSay.GetSize(); i++)
// 	{
// 		dH += aSay[i];
// 	}
// 
// 	m_edtH.SetEditUnit(dH);
// 
// 	UpdateData(FALSE);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcN1()
// {
// 	UpdateData(TRUE);
// 
// 	CArray<double, double> aSax;
// 	CStrParser::GetUEDistance(m_strSax, aSax);
// 
// 	BOOL bAllZero = TRUE;
// 	for (int i = 0; i < aSax.GetSize(); i++)
// 	{
// 		if (aSax[i] > 0.0)
// 		{
// 			bAllZero = FALSE;
// 		}
// 	}
// 
// 	if (bAllZero)
// 	{
// 		if (aSax.GetSize() > 0) aSax.RemoveAll();
// 	}
// 
// 	int nN1 = aSax.GetSize() + 1;
// 
// 	m_edtN1.SetEditUnit(nN1);
// 
// 	UpdateData(FALSE);
// }
// 
// void CDgnSeisAnchorEvalItemDlg::CalcN2()
// {
// 	UpdateData(TRUE);
// 
// 	CArray<double, double> aSay;
// 	CStrParser::GetUEDistance(m_strSay, aSay);
// 
// 	BOOL bAllZero = TRUE;
// 	for (int i = 0; i < aSay.GetSize(); i++)
// 	{
// 		if (aSay[i] > 0.0)
// 		{
// 			bAllZero = FALSE;
// 		}
// 	}
// 
// 	if (bAllZero)
// 	{
// 		if(aSay.GetSize() > 0) aSay.RemoveAll();
// 	}		
// 
// 	int nN2 = aSay.GetSize() + 1;
// 
// 	m_edtN2.SetEditUnit(nN2);
// 
// 	UpdateData(FALSE);
// }
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnSeisAnchorEvalItemDlg implementation functions
// BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemDlg, CDialogMove)
// 	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalItemDlg)
// 	ON_CBN_SELCHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,     OnSeisAnchorEvalGroupCmb)
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_BTN,        OnSeisAnchorEvalGroupBtn)
// 	ON_CBN_SELCHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_CMB,     OnSeisAnchorEvalPropCmb)
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_BTN,        OnSeisAnchorEvalPropBtn)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,			OnSeisAnchorEvalPropHcopEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT,				OnSeisAnchorEvalPropTheta1Edt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT,				OnSeisAnchorEvalPropTheta2Edt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_EDT,				OnSeisAnchorEvalPropTheta1QEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_EDT,				OnSeisAnchorEvalPropTheta2QEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D1_EDT,					OnSeisAnchorEvalPropD1Edt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_EDT,					OnSeisAnchorEvalPropD1_qEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_SAX_EDT,					OnSeisAnchorEvalPropSaxEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D2_EDT,					OnSeisAnchorEvalPropD2Edt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_EDT,					OnSeisAnchorEvalPropD2_qEdt)
// 	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_SAY_EDT,					OnSeisAnchorEvalPropSayEdt)
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,               OnSeisAnchorEvalRebarChk)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN,               OnSeisAnchorEvalRebarBtn)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_CHK,             OnSeisAnchorEvalSupportChk)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_BTN,             OnSeisAnchorEvalSupportBtn)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_FAILURE_BTN,             OnSeisAnchorEvalFailureBtn)
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_DRAW_RDO,           OnSeisAnchorEvalDrawRdo)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_RDO,          OnSeisAnchorEvalGuideRdo)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_OK_BTN,                  OnSeisAnchorEvalOKBtn)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_CANCEL_BTN,              OnSeisAnchorEvalCanCelBtn)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN,               OnSeisAnchorEvalApplyBtn)	
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnSeisAnchorEvalItemDlg message handlers
// 
// BOOL CDgnSeisAnchorEvalItemDlg::OnInitDialog() 
// {
// 	CDialogMove::OnInitDialog();
// 
// 	InitUnit();
// 	InitCombo();
// 	InitView();
// 
// 	Data2Dlg();
// 
// 	if(!m_bModify)
// 	{
// 		UpdateName();
// 		Calcfck();
// 		CalcHcop();
// 	}
// 
// 	if(m_bModify)
// 	{
// 		m_nViewType = 0;
// 		UpdateData(FALSE);
// 	}
// 		
// 	AlignControl();
// 	ControlsShowHide();
// 	EnableDisableControls();
// 		
// 	CalcNum();
// 	CalcB();
// 	CalcH();
// 	CalcN1();
// 	CalcN2();
// 
// 	if(m_bSetEmf)
// 	{
// 		CRect Rect;
// 		m_View.GetWindowRect(Rect);
// 		m_View.MakeEmfFile(m_strPath, Rect.Width() * 100, Rect.Height() * 100);
// 		PostMessage(WM_CLOSE, 0, 0);
// 	}
// 				
// 	return TRUE;  // return TRUE unless you set the focus to a control
// 	              // EXCEPTION: OCX Property Pages should return FALSE
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalGroupCmb()
// {
// 	UpdateName();
// 
// 	Calcfck();
// 	CalcHcop();
// 	CalcNum();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalGroupBtn()
// {
// 	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_EVAL,0)); 
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropCmb()
// {
// 	UpdateName();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropBtn()
// {
// 	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_PROP,0)); 
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropHcopEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropTheta1Edt()
// {
// 	Dlg2Data();
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropTheta2Edt()
// {
// 	Dlg2Data();
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropTheta1QEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropTheta2QEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropD1Edt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropD1_qEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropSaxEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// 	CalcN1();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropD2Edt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropD2_qEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalPropSayEdt()
// {
// 	Dlg2Data();
// 	SetViewData();
// 
// 	CalcB();
// 	CalcH();
// 	CalcN2();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalRebarChk()
// {
// 	EnableDisableControls();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalRebarBtn()
// {
// 	CDgnSeisAnchorRebarDlg dlg;
// 	dlg.m_Data = m_Data.Rebar;
// 	if(dlg.DoModal() == IDOK)
// 	{
// 		m_Data.Rebar = dlg.m_Data;
// 	}
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalSupportChk()
// {
// 	EnableDisableControls();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalSupportBtn()
// {
// 	CArray<double, double> aSax;
// 	CStrParser::GetUEDistance(m_strSax, aSax);
// 	if(m_strSax.IsEmpty()) aSax.RemoveAll();
// 	
// 	CArray<double, double> aSay;
// 	CStrParser::GetUEDistance(m_strSay, aSay);
// 	if(m_strSay.IsEmpty()) aSay.RemoveAll();
// 	
// 	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
// 	m_pDoc->m_pAttrCtrl2->SetAnevSupport(aSax, aSay, m_Data.SuppType, SuppType);
// 
// 	CDgnSeisAnchorSupportDlg dlg;
// 	dlg.m_Data = SuppType;
// 	if(dlg.DoModal() == IDOK)
// 	{
// 		m_Data.SuppType.Initialize();
// 		m_Data.SuppType = dlg.m_Data;
// 	}
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalFailureBtn()
// {
// 	CArray<double, double> aSax;
// 	CStrParser::GetUEDistance(m_strSax, aSax);
// 	if(m_strSax.IsEmpty()) aSax.RemoveAll();
// 	
// 	CArray<double, double> aSay;
// 	CStrParser::GetUEDistance(m_strSay, aSay);
// 	if(m_strSay.IsEmpty()) aSay.RemoveAll();
// 	
// 	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
// 	m_pDoc->m_pAttrCtrl2->SetAnevSupport(aSax, aSay, m_Data.SuppType, SuppType);
// 
// 	m_Data.SuppType.Initialize();
// 	m_Data.SuppType = SuppType;
// 
// 	CDgnSeisAnchorFailDlg dlg;
// 
// 	dlg.m_Data = m_Data;
// 	if(dlg.DoModal() == IDOK)
// 	{
// 		m_Data = dlg.m_Data;
// 	}
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalDrawRdo()
// {
// 	ControlsShowHide();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalGuideRdo()
// {
// 	ControlsShowHide();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalOKBtn()
// {
// 	if(!Dlg2Data()) return;
// 
// 	if(m_bModify)
// 	{
// 		if(!m_pDoc->m_pDataCtrl->ModifyAnev(m_Key, m_Data)) return;
// 	}
// 	else
// 	{
// 		if(!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return;
// 	}
// 
// 	CDialogMove::OnOK();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalCanCelBtn()
// {
// 	CDialogMove::OnCancel();
// }
// 
// void CDgnSeisAnchorEvalItemDlg::OnSeisAnchorEvalApplyBtn()
// {
// 	if(!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return;
// 
// 	if(!Dlg2Data()) return;
// }