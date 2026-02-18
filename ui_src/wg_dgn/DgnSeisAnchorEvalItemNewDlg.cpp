// DgnSeisAnchorEvalItemNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemNewDlg.h"

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

#include "DgnSeisAnchorEvalItemNewSubArrangeDlg.h"
#include "DgnSeisAnchorEvalItemNewSubPropertyDlg.h"
#include "DgnSeisAnchorEvalItemNewSubSupportDlg.h"
#include "DgnSeisAnchorEvalItemNewSubTensionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewDlg dialog

CDgnSeisAnchorEvalItemNewDlg::CDgnSeisAnchorEvalItemNewDlg(CWnd* pParent /*=NULL*/, BOOL bAdd/* = FALSE*/)
	: CDialogMove(CDgnSeisAnchorEvalItemNewDlg::IDD, pParent), m_GuideDlg(this)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bAdd = bAdd;
	m_bRebar = FALSE;
	//m_bSupport = FALSE;
	m_bTension = FALSE;
	m_bShowPreview = FALSE;
	m_nViewType = 1;
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_Key = 0;
	m_Data.Initialize();
	m_DataPre.Initialize();
	m_bModify = FALSE;
	m_bSetEmf = FALSE;
	m_bDrawOKEval = TRUE;

	m_GuideDlg.SetDataSource(&m_Data);
}

void CDgnSeisAnchorEvalItemNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemNewDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, m_tabEval);

	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_NAME_EDT,              m_edtName);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_DESC_EDT,				 m_edtDesc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,      m_cmbGroup);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_EDT,        m_edtfck);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_UNT,        m_untfck);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,       m_edtHcop);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_UNT,       m_untHcop);
	//DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_CMB,      m_cmbProp);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_NUM_EDT,               m_edtNum);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_N1_EDT,				 m_edtN1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_N2_EDT,				 m_edtN2);
	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,				 m_bRebar);
	//DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_CHK,			 m_bSupport);
	DDX_Check(pDX, IDC_DGN_SEIS_TENSION_ANCHOR_CHK,                  m_bTension);
	DDX_Check(pDX, IDC_DGN_SEIS_SHOW_PREVIEW_CHK,					 m_bShowPreview);	
	DDX_Radio(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_DRAW_RDO,			 m_nViewType);

	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_VIEW, m_View);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItemNewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
}

void CDgnSeisAnchorEvalItemNewDlg::SetParamData(T_ANEV_K Key, T_ANEV_D &Data)
{
	m_Key = Key;
	m_Data = Data;
	m_GuideDlg.SetDataSource(&m_Data);
	m_DataPre = Data;
	m_bModify = TRUE;
}

void CDgnSeisAnchorEvalItemNewDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	//T_ANEV_K Key,  KeyBack;
	T_ANEV_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_BREV_ADD):
		case(UR_BREV_DEL):
		case(UR_BREV_MFD):
		case(UR_BREV_MFS):
		case(UR_BRPR_ADD):
		case(UR_BRPR_DEL):
		case(UR_BRPR_MFD):
		case(UR_BRPR_MFS):
			InitCombo();
			UpdateName();
			Calcfck();
			CalcHcop();
			CalcNum();
			Data2Dlg();
			SetViewData();
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnSeisAnchorEvalItemNewDlg::MakeEmfAndClose(CString strPath)
{
	m_bSetEmf = TRUE;
	m_strPath = strPath;
}


void CDgnSeisAnchorEvalItemNewDlg::InitTabControl(BOOL bTension/* = FALSE*/)
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_PLACEHOLDER);
	m_tabEval.SetPlaceHolder(pPlaceHolder);
	m_tabEval.DeleteAllItems();

	// 받침배치
	m_pArrange = new CDgnSeisAnchorEvalItemNewSubArrangeDlg(this);
	m_pArrange->SetArrangeData(&m_Data);
	m_tabEval.AddTab(m_pArrange, _T("받침배치"), /*_LS(IDS_CMD2_SECTION_MANAGER_REBAR_LONGI),*/ CDgnSeisAnchorEvalItemNewSubArrangeDlg::IDD, TRUE);

	// bearing property
	m_pProperty = new CDgnSeisAnchorEvalItemNewSubPropertyDlg(this);
	m_pProperty->SetPropertyData(&m_Data);
	m_tabEval.AddTab(m_pProperty, _T("Bearing Property"), /*_LS(IDS_CMD2_SECTION_MANAGER_REBAR_LONGI),*/ CDgnSeisAnchorEvalItemNewSubPropertyDlg::IDD, TRUE);

	// 저항받침 위치 설정
	m_pSupport = new CDgnSeisAnchorEvalItemNewSubSupportDlg(this);
	m_pSupport->SetSupportData(&m_Data);
	m_tabEval.AddTab(m_pSupport, _T("저항받침 위치 설정"), /*_LS(IDS_CMD2_SECTION_MANAGER_REBAR_LONGI),*/ CDgnSeisAnchorEvalItemNewSubSupportDlg::IDD, TRUE);

	if(bTension)
	{
		// 앵커 인장 검토
		m_pTension = new CDgnSeisAnchorEvalItemNewSubTensionDlg(this);
		m_pTension->SetTensionData(&m_Data);
		m_tabEval.AddTab(m_pTension, _T("앵커 인장 검토"), /*_LS(IDS_CMD2_SECTION_MANAGER_REBAR_LONGI),*/ CDgnSeisAnchorEvalItemNewSubTensionDlg::IDD, TRUE);
	}

	m_tabEval.ShowTab(0);
}

void CDgnSeisAnchorEvalItemNewDlg::InitUnit()
{	
	m_edtfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
	m_untfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
	m_edtHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
	m_untHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtN1.SetUnitType(D_UNITSYS_NONE);
	m_edtN2.SetUnitType(D_UNITSYS_NONE);

}

void CDgnSeisAnchorEvalItemNewDlg::InitCombo()
{
	// Bearing Evaluation
	CArray<T_BREV_K, T_BREV_K> aBrevK;
	m_pDoc->m_pAttrCtrl2->GetBrevKeyList(aBrevK);
	
	T_BREV_D BrevD; BrevD.Initialize();

	m_cmbGroup.ResetContent();
	for(int i=0; i<aBrevK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetBrev(aBrevK[i], BrevD)) continue;
		m_cmbGroup.SetItemData(m_cmbGroup.AddString(BrevD.strName), aBrevK[i]);
	}
	m_cmbGroup.SetCurSel(0);

	if(m_bAdd && aBrevK.GetSize() >0) 
	{
		m_Data.BrevK = aBrevK[0];
	}

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
}

void CDgnSeisAnchorEvalItemNewDlg::InitView()
{
	m_View.SetParent(this);
	m_View.Init();
	//m_View.SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	//m_View.SetContext(TRUE, TRUE);// 이거 주석 풀면 컨텍스트 메뉴 열림
	//m_View.SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	int nRatType[4] = { 0 };
	m_View.MakeDrawUnit(nRatType);
}

void CDgnSeisAnchorEvalItemNewDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	CArray<UINT, UINT> aControls;
	aControls.Add(IDC_DGN_SEIS_ANCHOR_EVAL_BMP);

	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_BMP)->GetWindowRect(rToMove);

	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;

	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
}

void CDgnSeisAnchorEvalItemNewDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW)->ShowWindow(m_nViewType == 0);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_BMP)->ShowWindow(m_nViewType == 1);
}

void CDgnSeisAnchorEvalItemNewDlg::EnableDisableControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN)->EnableWindow(m_bRebar);
	//etDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_SUPPORT_BTN)->EnableWindow(m_bSupport);

	if(m_bModify) GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN)->EnableWindow(FALSE);
}

void CDgnSeisAnchorEvalItemNewDlg::SetViewData()
{
	T_BRPR_D BrprD; BrprD.Initialize();
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(!m_pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) { /*ASSERT(0); */}
			aArBrprD[i].push_back(BrprD);
		}
	}
	m_View.SetData(aArBrprD, m_Data, FALSE);
	m_View.SetDrawType(0, 0, FALSE);
	m_View.GetDrawOKInfo(m_bDrawOKEval);

	m_GuideDlg.SetDataToView();
}

void CDgnSeisAnchorEvalItemNewDlg::Data2Dlg(BOOL bChangEval/* = FALSE*/)
{
	// Arrange
	m_pArrange->SetArrangeData(&m_Data);
	if (m_pArrange->m_hWnd != NULL)
	{
		if (bChangEval) m_pArrange->Data2Dlg(); // 새롭게 변경된 데이터를 세팅.
	}

	// Bearing Property
	m_pProperty->SetPropertyData(&m_Data);
	if (m_pProperty->m_hWnd != NULL) m_pProperty->Data2Dlg();   // m_pProperty->Dlg2Data();

	// Bearing Support
	if (bChangEval)
	{
		T_ANEV_REG_SUPP_TYPE_D OutSupp; OutSupp.Initialize();
		m_pDoc->m_pAttrCtrl2->SetAnevSupportNew(m_Data.BrevK, m_Data.aSax, m_Data.aSay, m_Data.SuppType, OutSupp, bChangEval);
		m_Data.SuppType.Initialize();
		m_Data.SuppType = OutSupp;
	}
	m_pSupport->SetSupportData(&m_Data);
	if (m_pSupport->m_hWnd != NULL)
	{
		if (bChangEval) m_pSupport->Data2Dlg(); // 새롭게 변경된 데이터를 세팅.
		m_pSupport->Dlg2Data();
	}

	// Tension Chk
	if (m_bTension)
	{
		if(bChangEval)
		{
			T_ANEV_TENS_CHK_D TensChk; TensChk.Initialize();
			m_pDoc->m_pAttrCtrl2->SetAnevTension(m_Data.aSax, m_Data.aSay, m_Data.BrevK, m_Data.TensChk, TensChk, bChangEval);
			m_Data.TensChk.Initialize();
			m_Data.TensChk = TensChk;
		}

		m_pTension->SetTensionData(&m_Data);
		if (m_pTension->m_hWnd != NULL)
		{
			if (bChangEval) m_pTension->Data2Dlg(); // 새롭게 변경된 데이터를 세팅.
			m_pTension->Dlg2Data();
		}
	}
	else
	{
		m_Data.TensChk.Initialize();
	}

	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDescription);

	CArray<T_BREV_K, T_BREV_K> aBrevK;
	m_pDoc->m_pAttrCtrl2->GetBrevKeyList(aBrevK);

	for(int i=0; i<aBrevK.GetSize(); i++)
	{
		if(aBrevK[i] == m_Data.BrevK) 
		{ 
			m_cmbGroup.SetCurSel(i); 
			break; 
		}
	}

	m_edtfck.SetEditUnit(m_Data.dfck);
	m_edtHcop.SetEditUnit(m_Data.dHcop);

 	//m_edtN2.SetEditUnit(m_Data.nN2);

	m_bRebar = m_Data.bRebar;
	//m_bSupport = m_Data.bSuppType;
	m_bTension = m_Data.bTensChk;

	UpdateData(FALSE);

	SetViewData();
}


BOOL CDgnSeisAnchorEvalItemNewDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	if (m_pArrange->m_hWnd != NULL) m_pArrange->Dlg2Data();
	if (m_pProperty->m_hWnd != NULL) m_pProperty->Dlg2Data();
	if (m_pSupport->m_hWnd != NULL)  m_pSupport->Dlg2Data();

	if (m_bTension)
	{
		if (m_pTension->m_hWnd != NULL) m_pTension->Dlg2Data();
	}

	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDescription);

	m_Data.BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
	m_Data.dfck = m_edtfck.GetEditValue();
	m_Data.dHcop = m_edtHcop.GetEditValue();	
	//m_Data.BrprK = m_cmbProp.GetItemData(m_cmbProp.GetCurSel());

 	m_Data.bRebar = m_bRebar;
 	//m_Data.bSuppType = m_bSupport;
	m_Data.bTensChk = m_bTension;

	return TRUE;
}

BOOL CDgnSeisAnchorEvalItemNewDlg::DataCheck()
{
	if(!m_bDrawOKEval)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK));
		return FALSE;
	}

	// 다른 것들은 BOOL CEditData::CheckAnev(T_ANEV_K Key, T_ANEV_D& rData) 에서 체크
	// Bearing Property
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if (m_Data.BearingProp.aBrprKs[i].aBrprK[j] < 1)
			{
				m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_BRPR_ERROR_NODATA_CHECK));  // [오류] Anchor Evaluation데이터를 수정할 수 없습니다. (항목 : Bearing Property)

				return FALSE;  
			}
		}
	}

	// 인장 검토
	if (m_Data.bTensChk)
	{
		CMap<UINT2, UINT2, int, int> mElnkNodeK;  mElnkNodeK.InitHashTable(11);
		UINT2 KeyTens;
		int nInex = 0;
		int nTemp = 0;
		for (int i = 0; i < m_Data.TensChk.aElnkNodeKs.GetSize(); i++)
		{
			for (int j = 0; j < m_Data.TensChk.aElnkNodeKs[i].aNodeK1.GetSize(); j++)
			{
				KeyTens.i1 = m_Data.TensChk.aElnkNodeKs[i].aNodeK1[j];
				KeyTens.i2 = m_Data.TensChk.aElnkNodeKs[i].aNodeK2[j];

				if (KeyTens.i1 == 0 && KeyTens.i2 == 0)
					continue;

				if (mElnkNodeK.Lookup(KeyTens, nTemp))
				{
					m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_TENSCHK_SAME_CHECK));
					return FALSE;
				}
				mElnkNodeK.SetAt(KeyTens, nInex);
				nInex++;
			}
		}
	}

	return TRUE;
}

void CDgnSeisAnchorEvalItemNewDlg::UpdateName()
{
	CString strName;

	CString strGroup;
	CString strProp;

	m_cmbGroup.GetWindowText(strGroup);
	//m_cmbProp.GetWindowText(strProp);
	
	strName.Format(_T("%s"), strGroup);

	m_edtName.SetWindowText(strName);

	UpdateData(FALSE);
}

void CDgnSeisAnchorEvalItemNewDlg::Calcfck()
{
	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;

	double dfck = m_pDoc->m_pAttrCtrl2->GetPierCapMinfck(BrevD.EvgpK);
	m_edtfck.SetEditUnit(dfck);
}

void CDgnSeisAnchorEvalItemNewDlg::CalcHcop()
{
	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;
	
	double dHcop = m_pDoc->m_pAttrCtrl2->GetPierCapMinH(BrevD.EvgpK);
	m_edtHcop.SetEditUnit(dHcop);
}

void CDgnSeisAnchorEvalItemNewDlg::CalcNum()
{
	UpdateData(TRUE);

	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;

	int nNum = BrevD.n2wFixed + BrevD.n2wMovable + BrevD.n1wLMovable + BrevD.n1wTMovable;

	m_edtNum.SetEditUnit(nNum);

	UpdateData(FALSE);
}

void CDgnSeisAnchorEvalItemNewDlg::CalcN1()
{
	UpdateData(TRUE);

	CString strSax = CStrParser::ConvtDistArr2DistStr(m_Data.aSax);
	CArray<double, double> aSax;
	CStrParser::GetUEDistance(strSax, aSax);

	BOOL bAllZero = TRUE;
	for (int i = 0; i < aSax.GetSize(); i++)
	{
		if (aSax[i] > 0.0)
		{
			bAllZero = FALSE;
		}
	}

	if (bAllZero)
	{
		if (aSax.GetSize() > 0) aSax.RemoveAll();
	}

	int nN1 = aSax.GetSize() + 1;

	m_edtN1.SetEditUnit(nN1);

	UpdateData(FALSE);
}

void CDgnSeisAnchorEvalItemNewDlg::CalcN2()
{
	UpdateData(TRUE);

	CString strSay = CStrParser::ConvtDistArr2DistStr(m_Data.aSay);

	CArray<double, double> aSay;
	CStrParser::GetUEDistance(strSay, aSay);

	BOOL bAllZero = TRUE;
	for (int i = 0; i < aSay.GetSize(); i++)
	{
		if (aSay[i] > 0.0)
		{
			bAllZero = FALSE;
		}
	}

	if (bAllZero)
	{
		if(aSay.GetSize() > 0) aSay.RemoveAll();
	}		

	int nN2 = aSay.GetSize() + 1;

	m_edtN2.SetEditUnit(nN2);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalItemNewDlg)

	ON_CBN_SELCHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,     OnSeisAnchorEvalGroupCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_BTN,        OnSeisAnchorEvalGroupBtn)
	//ON_CBN_SELCHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_CMB,     OnSeisAnchorEvalPropCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_BTN,        OnSeisAnchorEvalPropBtn)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,			OnSeisAnchorEvalPropHcopEdt)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,               OnSeisAnchorEvalRebarChk)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN,               OnSeisAnchorEvalRebarBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_TENSION_ANCHOR_CHK,					OnSeisAnchorEvalTensionChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_SHOW_PREVIEW_CHK,				    OnSeisAnchorEvalShowPreview)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_FAILURE_BTN,             OnSeisAnchorEvalFailureBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_DRAW_RDO,           OnSeisAnchorEvalDrawRdo)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_VIEW_GUIDE_RDO,          OnSeisAnchorEvalGuideRdo)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_OK_BTN,                  OnSeisAnchorEvalOKBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_CANCEL_BTN,              OnSeisAnchorEvalCanCelBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN,               OnSeisAnchorEvalApplyBtn)	

	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, OnEvaluationChangTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, OnEvaluationChangedTab)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewDlg message handlers

void CDgnSeisAnchorEvalItemNewDlg::InitGuideDlg()
{
	m_GuideDlg.SetInitPos(D_INIT_POS_RT);
	m_GuideDlg.Create(IDD_DGN_SEIS_ANCHOR_EVAL_GUIDE_DLG, this);
	m_GuideDlg.ShowWindow(m_bShowPreview);
}

BOOL CDgnSeisAnchorEvalItemNewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	InitView();
	InitGuideDlg();

	m_bTension = m_Data.bTensChk;
	InitTabControl(m_bTension);

	Data2Dlg();

	if(!m_bModify)
	{
		UpdateName();
		Calcfck();
		CalcHcop();
	}

	if(m_bModify)
	{
		m_nViewType = 0;
		UpdateData(FALSE);
	}
		
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
			
	CalcNum();
	CalcN1();
	CalcN2();

	m_GuideDlg.ResetDlgPos();

	if(m_bSetEmf)
	{
		CRect Rect;
		m_View.GetWindowRect(Rect);
		m_View.MakeEmfFile(m_strPath, Rect.Width() * 100, Rect.Height() * 100);
		PostMessage(WM_CLOSE, 0, 0);
	}
					
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalGroupCmb()
{
	UpdateName();

	Calcfck();
	CalcHcop();
	CalcNum();

	Dlg2Data();
	Data2Dlg(TRUE);
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalGroupBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_EVAL,0)); 
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalPropCmb()
{
	UpdateName();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalPropBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_PROP,0)); 
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalPropHcopEdt()
{
	Dlg2Data();
	SetViewData();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalRebarChk()
{
	EnableDisableControls();
}

void CDgnSeisAnchorEvalItemNewDlg::OnCloseGuideDlg()
{
	m_bShowPreview = FALSE;
	UpdateData(FALSE); 
	OnSeisAnchorEvalShowPreview();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalShowPreview()
{
	UpdateData(TRUE);

	if (m_bShowPreview)
	{
		m_GuideDlg.ResetDlgPos();
		m_GuideDlg.ShowWindow(TRUE);
	}
	else
	{
		m_GuideDlg.ShowWindow(FALSE);
	}
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalTensionChk()
{
	UpdateData(TRUE);

	InitTabControl(m_bTension);

	Dlg2Data();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalRebarBtn()
{
	CDgnSeisAnchorRebarDlg dlg;
	dlg.m_Data = m_Data.Rebar;
	if(dlg.DoModal() == IDOK)
	{
		m_Data.Rebar = dlg.m_Data;
	}
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalFailureBtn()
{
	if(m_pArrange)
	{
		m_pArrange->Dlg2Data(TRUE);
	}	

	CString strSax = CStrParser::ConvtDistArr2DistStr(m_Data.aSax);
	CArray<double, double> aSax;
	CStrParser::GetUEDistance(strSax, aSax);
	if(strSax.IsEmpty()) aSax.RemoveAll();

	CString strSay = CStrParser::ConvtDistArr2DistStr(m_Data.aSay);
	CArray<double, double> aSay;
	CStrParser::GetUEDistance(strSay, aSay);
	if(strSay.IsEmpty()) aSay.RemoveAll();
	
	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevSupportNew(m_Data.BrevK, aSax, aSay, m_Data.SuppType, SuppType);
	m_Data.SuppType.Initialize();
	m_Data.SuppType = SuppType;

	T_ANEV_BRPR_D BearingProp; BearingProp.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevProperty(aSax, aSay, m_Data.BearingProp, BearingProp);
	m_Data.BearingProp.Initialize();
	m_Data.BearingProp = BearingProp;

	CDgnSeisAnchorFailDlg dlg;

	dlg.m_Data = m_Data;
	if(dlg.DoModal() == IDOK)
	{
		m_Data = dlg.m_Data;
	}
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalDrawRdo()
{
	ControlsShowHide();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalGuideRdo()
{
	ControlsShowHide();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalOKBtn()
{
	if(!Dlg2Data()) return;

	if(!DataCheck()) return;

	if(m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyAnev(m_Key, m_Data))
		{
			m_Data = m_DataPre;
			return;
		}
	}
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return;
	}

	CDialogMove::OnOK();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalCanCelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisAnchorEvalItemNewDlg::OnSeisAnchorEvalApplyBtn()
{
	if(!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return;

	if(!Dlg2Data()) return;

	if(!DataCheck()) return;
}

void CDgnSeisAnchorEvalItemNewDlg::OnEvaluationChangTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	Dlg2Data();
	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchorEvalItemNewDlg::OnEvaluationChangedTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	Data2Dlg();
	*pResult = 0;
}
