DgnSeisAnchorRebarDlg// DgnSeisAnchorEvalItem2NewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItem2NewDlg.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\InitCtrl.h"

#include "../wg_main/wg_main.h"

#include "..\wg_dgn\DgnSeisAnchor2FailDlg.h"
#include "..\wg_dgn\DgnSeisAnchorFailDlg.h"
#include "..\wg_dgn\DgnSeisAnchorRebarDlg.h"
#include "..\wg_dgn\DgnSeisAnchorSupportDlg.h"
#include "..\wg_dgn\DgnSeisAnchorShearFactor.h"

#include "DgnSeisAnchorEvalItem2NewSubArrangeDlg.h"
#include "DgnSeisAnchorEvalItem2NewSubPropertyDlg.h"
#include "DgnSeisAnchorEvalItem2NewSubSupportDlg.h"
#include "DgnSeisAnchorEvalItem2NewSubTensionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewDlg dialog

CDgnSeisAnchorEvalItem2NewDlg::CDgnSeisAnchorEvalItem2NewDlg(CWnd* pParent /*=NULL*/, BOOL bAdd/* = FALSE*/)
	: CDialogMove(CDgnSeisAnchorEvalItem2NewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItem2NewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bAdd = bAdd;
	m_bRebar = FALSE;
	m_bTension = FALSE;
	m_bShearFactor = FALSE;
	m_bPryout = TRUE;
	m_bPryoutRow = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_Key = 0;
	m_Data.Initialize();
	m_DataPre.Initialize();
	m_bModify = FALSE;
	m_bSetEmf = FALSE;
	m_bDrawOKEval = TRUE;

	m_nType = 0;
	m_nAbutPos = 0;

	m_IsGuideView = FALSE;
}

CDgnSeisAnchorEvalItem2NewDlg::~CDgnSeisAnchorEvalItem2NewDlg()
{
}

void CDgnSeisAnchorEvalItem2NewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItem2NewDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, m_tabEval);

	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_NAME_EDT,              m_edtName);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_DESC_EDT,				 m_edtDesc);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,      m_cmbGroup);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_EDT,        m_edtfck);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_FCK_UNT,        m_untfck);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,       m_edtHcop);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_UNT,       m_untHcop);
	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,				 m_bRebar);
	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_CHK,        m_bShearFactor);
	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_PRYOUT_CHK,              m_bPryout);
	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_PRYOUTROW_CHK,           m_bPryoutRow);
	DDX_Check(pDX, IDC_DGN_SEIS_TENSION_ANCHOR_CHK,                  m_bTension);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_VIEW, m_View);

	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItem2NewDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisAnchorEvalItem2NewDlg::SetParamData(T_ANEV_K Key, T_ANEV_D &Data)
{
	m_Key = Key;
	m_Data = Data;
	m_DataPre = Data;
	m_bModify = TRUE;
}

void CDgnSeisAnchorEvalItem2NewDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

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
			Data2Dlg();
			SetViewData();
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnSeisAnchorEvalItem2NewDlg::MakeEmfAndClose(CString strPath)
{
	m_bSetEmf = TRUE;
	m_strPath = strPath;
}


void CDgnSeisAnchorEvalItem2NewDlg::InitTabControl(BOOL bTension/* = FALSE*/)
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_PLACEHOLDER);
	m_tabEval.SetPlaceHolder(pPlaceHolder);
	m_tabEval.DeleteAllItems();

	// 받침배치
	m_pArrange = new CDgnSeisAnchorEvalItem2NewSubArrangeDlg(this);
	m_pArrange->SetArrangeData(&m_Data);
	m_tabEval.AddTab(m_pArrange, _LS(IDS_DGN_SEIS_ANCHOR_ARRANGE), CDgnSeisAnchorEvalItem2NewSubArrangeDlg::IDD, TRUE); // _T("받침배치"),

	// bearing property
	m_pProperty = new CDgnSeisAnchorEvalItem2NewSubPropertyDlg(this);
	m_pProperty->SetPropertyData(&m_Data);
	m_tabEval.AddTab(m_pProperty, _LS(IDS_DGN_SEIS_ANCHOR_BEARING_PROPERTY), CDgnSeisAnchorEvalItem2NewSubPropertyDlg::IDD, TRUE); // _T("Bearing Property")

	// 저항받침 위치 설정
	m_pSupport = new CDgnSeisAnchorEvalItem2NewSubSupportDlg(this);
	m_pSupport->SetSupportData(&m_Data);
	m_tabEval.AddTab(m_pSupport, _LS(IDS_DGN_SEIS_ANCHOR_SUPPORT), CDgnSeisAnchorEvalItem2NewSubSupportDlg::IDD, TRUE); // _T("저항받침 위치 설정")

	if(bTension)
	{
		// 앵커 인장 검토
		m_pTension = new CDgnSeisAnchorEvalItem2NewSubTensionDlg(this);
		m_pTension->SetTensionData(&m_Data);
		m_tabEval.AddTab(m_pTension, _LS(IDS_DGN_SEIS_ANCHOR_TENSION), CDgnSeisAnchorEvalItem2NewSubTensionDlg::IDD, TRUE); // _T("앵커 인장 검토"),
	}

	m_tabEval.ShowTab(0);
}

void CDgnSeisAnchorEvalItem2NewDlg::InitUnit()
{	
	m_edtfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
	m_untfck.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dfck);
	m_edtHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
	m_untHcop.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dHcop);
}

void CDgnSeisAnchorEvalItem2NewDlg::InitCombo()
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

}

void CDgnSeisAnchorEvalItem2NewDlg::InitView()
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

void CDgnSeisAnchorEvalItem2NewDlg::EnableDisableControls()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN)->EnableWindow(m_bRebar);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_BTN)->EnableWindow(m_bShearFactor);

	if(m_bModify) GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN)->EnableWindow(FALSE);

	GetDlgItem(IDC_DGN_SEIS_SHOW_PREVIEW_BTN)->EnableWindow(!m_IsGuideView);
}

void CDgnSeisAnchorEvalItem2NewDlg::SetViewData()
{
	T_BRPR_D BrprD; BrprD.Initialize();
	std::vector<std::vector<T_BRPR_D>> aArBrprD;

	int nMaxNum = 0;
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		int nNum = m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize();
		nMaxNum = max(nNum, nMaxNum);
	}

	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		int j = 0;
		for (j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(m_Data.BearingProp.aBrprKs[i].aBrprK[j]>0)
			{
				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) { /*ASSERT(0); */ }
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}
		}

		for(int jj = j; jj< nMaxNum; jj++)
		{
			BrprD.Initialize();
			aArBrprD[i].push_back(BrprD);
		}
	}
	//if (aArBrprD.size() == 0) return; // layer가 0인 경우 고려
	m_View.SetDrawType(0, 0, !m_bSetEmf);
	m_View.SetData(aArBrprD, m_Data, FALSE);
	m_View.GetDrawOKInfo(m_bDrawOKEval);
}

void CDgnSeisAnchorEvalItem2NewDlg::Data2Dlg(BOOL bChangEval/* = FALSE*/)
{
	// Arrange
	m_pArrange->SetArrangeData(&m_Data);
	if (m_pArrange->m_hWnd != NULL)
	{
		if (bChangEval) m_pArrange->Data2Dlg(); // 새롭게 변경된 데이터를 세팅.
	}

	if (m_Data.aLayer.GetSize() == 0) return;

	// Bearing Property
	m_pProperty->SetPropertyData(&m_Data);
	if (m_pProperty->m_hWnd != NULL) m_pProperty->Data2Dlg();   // m_pProperty->Dlg2Data();

	// aSx, aSy
	CArray<double, double> aSax; aSax.RemoveAll();
	int nNumSx = 0;
	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		if (nNumSx < m_Data.aLayer[i].aSax.GetSize())
		{
			nNumSx = m_Data.aLayer[i].aSax.GetSize();
			aSax.Copy(m_Data.aLayer[i].aSax);
		}
	}

	CArray<double, double> aSay; aSay.RemoveAll();
	T_ANEV_LAYER_D Layer1, Layer2;
	if (m_Data.aLayer.GetSize() == 1)
	{
		aSay.Add(0.0);
	}
	else
	{
		for (int i = 0; i < m_Data.aLayer.GetSize() - 1; i++)
		{
			Layer1 = m_Data.aLayer[i];
			Layer2 = m_Data.aLayer[i + 1];
			double dSay = fabs(Layer2.dSay - Layer1.dSay);
			aSay.Add(dSay);
		}
	}

	// Bearing Support
	if (bChangEval)
	{
		T_ANEV_TENS_CHK_D OutTens;
		T_ANEV_REG_SUPP_TYPE_D OutSupp; OutSupp.Initialize();
		m_pDoc->m_pAttrCtrl2->SetAnevSupportNew2(m_Data.BrevK, m_Data.aLayer, m_Data.SuppType, OutSupp, OutTens, bChangEval);
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
			m_pDoc->m_pAttrCtrl2->SetAnevTensionNew2(m_Data.aLayer, m_Data.BrevK, m_Data.TensChk, TensChk, bChangEval);
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

	m_bRebar = m_Data.bRebar;
	m_bTension = m_Data.bTensChk;
	m_bShearFactor = m_Data.bShearFactor;
	m_bPryout = m_Data.bPryout;
	m_bPryoutRow = m_Data.bPryoutRow;

	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
	if (BsedD.nDesignCode != KALIS_2023)
	{
		m_bShearFactor = FALSE;
		m_bPryout = TRUE;
		m_bPryoutRow = FALSE;
	}

	UpdateData(FALSE);

	SetViewData();
}


BOOL CDgnSeisAnchorEvalItem2NewDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	if (m_pArrange->m_hWnd != NULL) 
	{
		if (!m_pArrange->Dlg2Data()) 
			return FALSE;
	}
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

 	m_Data.bRebar = m_bRebar;
	m_Data.bTensChk = m_bTension;
	m_Data.bShearFactor = m_bShearFactor;
	m_Data.bPryout = m_bPryout;
	m_Data.bPryoutRow = m_bPryoutRow;
	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
	if (BsedD.nDesignCode != KALIS_2023)
	{
		m_Data.bShearFactor = FALSE;
		m_Data.nShearFactorType = 0;
		m_Data.bPryout = TRUE;
		m_Data.bPryoutRow = FALSE;
	}

	double dZero = m_pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		for(int j=0; j<m_Data.aLayer[i].aSax.GetSize(); j++)
		{
			if (j == 0) continue;

			double dSax = m_Data.aLayer[i].aSax[j];
			if (fabs(dSax) < dZero)
			{
				m_Data.aLayer[i].aSax.RemoveAt(j);
				j--;
			}
		}
	}

	return TRUE;
}

BOOL CDgnSeisAnchorEvalItem2NewDlg::DataCheck()
{
	if(!m_bDrawOKEval)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK));
		return FALSE;
	}

	// check 1
	double dZero = m_pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		BOOL bCheck = TRUE;
		double dCalcB = 0.0;
		double dCalcH = 0.0;

		dCalcB = m_Data.aLayer[i].dD1 + m_Data.aLayer[i].dD1_q;
		for (int j = 0; j < m_Data.aLayer[i].aSax.GetSize(); j++)
		{
			dCalcB += m_Data.aLayer[i].aSax[j];
		}
		if (fabs(m_Data.dB - dCalcB) > dZero) bCheck = FALSE;

		dCalcH = m_Data.aLayer[i].dD2 + m_Data.aLayer[i].dD2_q;
		if (fabs(m_Data.dH - dCalcH) > dZero) bCheck = FALSE;

		if (m_Data.aLayer[i].dD1 <= 0.0)   bCheck = FALSE;
		if (m_Data.aLayer[i].dD1_q <= 0.0) bCheck = FALSE;

		for (int j = 0; j < m_Data.aLayer[i].aSax.GetSize(); j++)
		{
			if (m_Data.aLayer[i].aSax[j] < 0.0) bCheck = FALSE;
		}

		if (m_Data.aLayer[i].dD2 <= 0.0)   bCheck = FALSE;
		if (m_Data.aLayer[i].dD2_q <= 0.0) bCheck = FALSE;

		if(!bCheck)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_COPING_SHAPE_DATA_CHECK));
			return FALSE;
		}
	}


	// check 2
	// 다른 것들은 BOOL CEditData::CheckAnev(T_ANEV_K Key, T_ANEV_D& rData) 에서 체크
	// Bearing Property
	T_BRPR_D BrprD; 
	int nElastic = 0;
	int nSteel = 0;
	for (int i = 0; i < m_Data.BearingProp.aBrprKs.GetSize(); i++)
	{
		for (int j = 0; j < m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			// 0 : 비대칭 고려
			if (m_Data.BearingProp.aBrprKs[i].aBrprK[j] < 0)
			{
				m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK)); 

				return FALSE;  
			}

			BrprD.Initialize();
			if (!m_pDoc->m_pAttrCtrl2->GetBrpr(m_Data.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) { continue;}
			if (BrprD.nBearing == 0) nElastic += 1;
			else if (BrprD.nBearing == 1) nSteel += 1;

		}
	}
	if (nElastic > 0 && nSteel > 0)
	{
		CString strErr = _T("[Error]") + _LS(IDS_DB_DT_BRPR_STEEL_ELASTIC);
		m_pDoc->DisplayHistoryMessage(strErr);
		return FALSE;
	}

	// check 3
	T_BREV_D BrevD; BrevD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(m_Data.BrevK, BrevD)) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK));
		return FALSE;
	}
	int nTotalNum = BrevD.n2wFixed + BrevD.n2wMovable + BrevD.n1wLMovable + BrevD.n1wTMovable;

	int nNum = 0;
	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		if (m_Data.aLayer[i].aSax.GetSize() == 1 && fabs(m_Data.aLayer[i].aSax[0]) < dZero)
		{
			nNum += 1;
		}
		else
		{
			nNum += m_Data.aLayer[i].aSax.GetSize() + 1;
		}
	}
	if (nTotalNum != nNum) 
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK));
		return FALSE;

	}

	// check 4 	인장 검토
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

				if(KeyTens.i1 == 0 && KeyTens.i2==0 ) 
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

	// check 5
	int Num1 = m_Data.aLayer.GetSize();               // Arrange
	int Num2 = m_Data.BearingProp.aBrprKs.GetSize();  // Bearing Property
	int Num3 = m_Data.SuppType.aLayer.GetSize();      // Suport Type
	int Num4 = m_Data.Fail.ConcFailL.aFailSelLayer.GetSize(); // Fail
	int Num5 = m_Data.Fail.ConcFailT.aFailSelLayer.GetSize();
	int Num6 = m_Data.Fail.PryoutL.aFailSelLayer.GetSize();
	int Num7 = m_Data.Fail.PryoutT.aFailSelLayer.GetSize();
	if (Num1 != Num2 || Num2 != Num3 || 
		Num3 != Num4 || Num4 != Num5 || 
		Num5 != Num6 || Num6 != Num7)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_SUPPORT_FAILURE_CHECK));
		return FALSE;
	}
	for (int i = 0; i < Num1; i++)
	{
		int Num11 = m_Data.aLayer[i].aSax.GetSize() + 1;
		if(m_Data.aLayer[i].aSax.GetSize() == 1)
		{
			if(m_Data.aLayer[i].aSax[0] == 0.0)
			{
				Num11 = 1;
			}
		}

		int Num21 = m_Data.BearingProp.aBrprKs[i].aBrprK.GetSize();
		int Num31 = m_Data.SuppType.aLayer[i].aBrevType.GetSize();    
		int Num41 = m_Data.Fail.ConcFailL.aFailSelLayer[i].aFailSel.GetSize();
		int Num51 = m_Data.Fail.ConcFailT.aFailSelLayer[i].aFailSel.GetSize();
		int Num61 = m_Data.Fail.PryoutL.aFailSelLayer[i].aFailSel.GetSize();
		int Num71 = m_Data.Fail.PryoutT.aFailSelLayer[i].aFailSel.GetSize();

		if (Num21 != Num41 || Num21 != Num51 ||
			Num21 != Num61 || Num21 != Num71)   // 저항받침 위치 정보 탭 확인 안한 상태에서  Fail 정보 개수가 맞지 않아서 발생됨
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_SUPPORT_FAILURE_CHECK));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CDgnSeisAnchorEvalItem2NewDlg::ApplyData()
{
	if (!Dlg2Data()) return FALSE;
	if (!DataCheck()) return FALSE;

	// 파괴형상을 먼저 확인할 땐 저장된 데이터가 필요해서 저장하는 동작을 추가함.
	T_ANEV_K AnevK = m_pDoc->m_pAttrCtrl2->GetAnevKey(m_Data.strName);
	if(AnevK > 0)
	{
		m_Key = AnevK;
		m_bModify = TRUE;
	}

	if (m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyAnev(m_Key, m_Data))
		{
			return FALSE;
		}
	}
	else
	{
		if (!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return FALSE;

		m_Key = m_pDoc->m_pAttrCtrl2->GetAnevKey(m_Data.strName);
		m_bModify = TRUE;
	}

	return TRUE;
}

void CDgnSeisAnchorEvalItem2NewDlg::UpdateName()
{
	CString strName;

	CString strGroup;
	CString strProp;

	m_cmbGroup.GetWindowText(strGroup);
	
	strName.Format(_T("%s"), strGroup);

	m_edtName.SetWindowText(strName);

	UpdateData(FALSE);
}

void CDgnSeisAnchorEvalItem2NewDlg::Calcfck()
{
	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;

	double dfck = m_pDoc->m_pAttrCtrl2->GetPierCapMinfck(BrevD.EvgpK);
	m_edtfck.SetEditUnit(dfck);
}

void CDgnSeisAnchorEvalItem2NewDlg::CalcHcop()
{
	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) return;
	
	double dHcop = m_pDoc->m_pAttrCtrl2->GetPierCapMinH(BrevD.EvgpK);
	m_edtHcop.SetEditUnit(dHcop);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItem2NewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalItem2NewDlg)

	ON_CBN_SELCHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_CMB,     OnSeisAnchorEvalGroupCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_BTN,        OnSeisAnchorEvalGroupBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_BEARING_PROP_BTN,        OnSeisAnchorEvalPropBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_SHOW_PREVIEW_BTN,                    OnSeisAnchorEvalGuideBtn)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_COPING_HCOP_EDT,			OnSeisAnchorEvalPropHcopEdt)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_CHK,               OnSeisAnchorEvalRebarChk)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_REBAR_BTN,               OnSeisAnchorEvalRebarBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_CHK,        OnSeisAnchorEvalShearFactorChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_BTN,        OnSeisAnchorEvalShearFactorBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_TENSION_ANCHOR_CHK,					OnSeisAnchorEvalTensionChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_FAILURE_BTN,             OnSeisAnchorEvalFailureBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_OK_BTN,                  OnSeisAnchorEvalOKBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_CANCEL_BTN,              OnSeisAnchorEvalCanCelBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_APPLY_BTN,               OnSeisAnchorEvalApplyBtn)	

	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, OnEvaluationChangTab)
	ON_NOTIFY(TCN_SELCHANGE,   IDC_DGN_SEIS_ANCHOR_EVAL_ITEM_TAB, OnEvaluationChangedTab)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewDlg message handlers

BOOL CDgnSeisAnchorEvalItem2NewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
	BOOL bShow = (BsedD.nDesignCode == KALIS_2023) ? TRUE : FALSE;
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_CHK)->ShowWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_SHEAR_FACTOR_BTN)->ShowWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_PRYOUT_CHK      )->ShowWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_PRYOUTROW_CHK   )->ShowWindow(bShow);

	InitUnit();
	InitCombo();
	InitView();

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
		UpdateData(FALSE);
	}
		
	EnableDisableControls();
			
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

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalGroupCmb()
{
	UpdateName();

	Calcfck();
	CalcHcop();

	Dlg2Data();
	Data2Dlg(TRUE);

	//
	T_BREV_K BrevK;
	BrevK = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());

	T_BREV_D BrevD; BrevD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) BrevD.Initialize();

	T_EVGP_D EvgpD; EvgpD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevK, EvgpD)) EvgpD.Initialize();

	m_nType    = EvgpD.nType;    // 0:Pier,  1:Abutment
	m_nAbutPos = EvgpD.nAbutPos; // 0:Start, 1:End
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalGroupBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_EVAL,0)); 
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalPropCmb()
{
	UpdateName();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalPropBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_BEARING_PROP,0)); 
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalGuideBtn()
{
	m_pGuideDlg = new CDgnSeisAnchorEvalGuide2Dlg(this, 0);
	m_pGuideDlg->SetInitPos(D_INIT_POS_RT);
	m_pGuideDlg->Create(IDD_DGN_SEIS_ANCHOR_EVAL_GUIDE2_DLG, this);
	m_pGuideDlg->ShowWindow(SW_SHOW);

	m_IsGuideView = TRUE;
	EnableDisableControls();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalPropHcopEdt()
{
	Dlg2Data();
	SetViewData();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalRebarChk()
{
	EnableDisableControls();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalShearFactorChk()
{
	EnableDisableControls();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnCloseGuideDlg()
{
	if (m_pGuideDlg != NULL)
	{
		m_pGuideDlg->DestroyWindow();
		m_pGuideDlg = NULL;

		m_IsGuideView = FALSE;
	}
	EnableDisableControls();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalTensionChk()
{
	UpdateData(TRUE);

	InitTabControl(m_bTension);

	Dlg2Data();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalRebarBtn()
{
	CDgnSeisAnchorRebarDlg dlg;
	dlg.m_Data = m_Data.Rebar;
	if(dlg.DoModal() == IDOK)
	{
		m_Data.Rebar = dlg.m_Data;
	}
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalShearFactorBtn()
{
	CDgnSeisAnchorShearFactor dlg;
	dlg.m_nShearFactorType = m_Data.nShearFactorType;

	if (dlg.DoModal() == IDOK)
	{
		m_Data.nShearFactorType = dlg.m_nShearFactorType;
	}
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalFailureBtn()
{
	if(m_pArrange)
	{
		if(!m_pArrange->Dlg2Data(TRUE))
			return;
	}	

	if (!Dlg2Data()) return;
	if (!DataCheck()) return;

	// aSx, aSy
	CArray<double, double> aSax; aSax.RemoveAll();
	int nNumSx = 0;
	for (int i = 0; i < m_Data.aLayer.GetSize(); i++)
	{
		if (nNumSx < m_Data.aLayer[i].aSax.GetSize())
		{
			nNumSx = m_Data.aLayer[i].aSax.GetSize();
			aSax.Copy(m_Data.aLayer[i].aSax);
		}
	}

	CArray<double, double> aSay; aSay.RemoveAll();
	T_ANEV_LAYER_D Layer1, Layer2;
	if (m_Data.aLayer.GetSize() == 1)
	{
		aSay.Add(0.0);
	}
	else
	{
		for (int i = 0; i < m_Data.aLayer.GetSize() - 1; i++)
		{
			Layer1 = m_Data.aLayer[i];
			Layer2 = m_Data.aLayer[i + 1];
			double dSay = fabs(Layer2.dSay - Layer1.dSay);
			aSay.Add(dSay);
		}
	}

	T_ANEV_TENS_CHK_D OutTens;
	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->SetAnevSupportNew2(m_Data.BrevK, m_Data.aLayer, m_Data.SuppType, SuppType, OutTens))
	{
		for (int nN2 = 0; nN2 < OutTens.aElnkNodeKs.GetSize(); nN2++)
		{
			for (int nN1 = 0; nN1 < OutTens.aElnkNodeKs[nN2].aNodeK1.GetSize(); nN1++)
			{
				BOOL bNg = FALSE;
				if (OutTens.aElnkNodeKs[nN2].aNodeK1[nN1] == 0) bNg = TRUE;
				if (OutTens.aElnkNodeKs[nN2].aNodeK2[nN1] == 0) bNg = TRUE;

				if (bNg)
				{
					//AfxMessageBox(_T("Bearing 배치가 Link정보와 맞지 않습니다."));
					//return;
				}
			}
		}
	}
	m_Data.SuppType.Initialize();
	m_Data.SuppType = SuppType;

	T_ANEV_BRPR_D BearingProp; BearingProp.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevPropertyNew2(m_Data.aLayer, m_Data.BearingProp, BearingProp);
	m_Data.BearingProp.Initialize();
	m_Data.BearingProp = BearingProp;

	if(m_Key == 0)
	{
		if(!ApplyData())
		{
			AfxMessageBox(_T("데이터 저장 후 확인 가능합니다."));
			return;
		}
	}

	CDgnSeisAnchor2FailDlg dlg;

	dlg.m_Data = m_Data;
	if (dlg.DoModal() == IDOK)
	{
		m_Data = dlg.m_Data;
	}

}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalOKBtn()
{
	if(!Dlg2Data()) return;
	if(!DataCheck()) return;

	if(m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyAnev(m_Key, m_Data))
		{
			//m_Data = m_DataPre;
			return;
		}
	}
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddAnev(m_Data)) return;
	}

	CDialogMove::OnOK();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalCanCelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisAnchorEvalItem2NewDlg::OnSeisAnchorEvalApplyBtn()
{
	if(!ApplyData()) return;
}

void CDgnSeisAnchorEvalItem2NewDlg::OnEvaluationChangTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	Dlg2Data();
	Data2Dlg();
	*pResult = 0;
}

void CDgnSeisAnchorEvalItem2NewDlg::OnEvaluationChangedTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	Data2Dlg();
	*pResult = 0;
}
