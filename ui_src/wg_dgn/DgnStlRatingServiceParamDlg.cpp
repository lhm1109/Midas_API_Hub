// DgnStlRatingServiceParamDlg.cpp: implementation of the CDgnStlRatingServiceParamDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingServiceParamDlg.h"
#include "DgnStlRatingServiceParamTabIDlg.h"
#include "DgnStlRatingServiceParamTabJDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;

CDgnStlRatingServiceParamDlg::CDgnStlRatingServiceParamDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlRatingServiceParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	m_nElemType = ELEM_TYPE_ELEMENT;
	//}}AFX_DATA_INITz
	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();



	m_bBothIJ = FALSE;

	m_Data.Initialize();

	// Element Type 그룹과 내부 컨트롤들 (숨김/표시 대상)
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);     // Element Type 그룹박스
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);          // Element 라디오 버튼
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);         // Virtual Beam 라디오 버튼

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_STL_RATING_SRSP_TAB);
	m_aPositionCtrl.Add(IDC_DGN_STL_RATING_SRSP_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_STL_RATING_SRSP_CLOSE);


	// Element Type 그룹박스 아래에 있는 모든 컨트롤들 (위치 이동 대상)
	m_aCtrl.RemoveAll();

	// "Both end parts(i && j) have the same type" 체크박스
	m_aCtrl.Add(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK);    // Both end parts 체크박스

	// Tab 컨트롤
	m_aCtrl.Add(IDC_DGN_STL_RATING_SRSP_TAB);           // Tab 컨트롤

	// Place Holder
	m_aCtrl.Add(IDC_DGN_STL_RATING_SRSP_PLACEHOLDER);   // Place Holder

	// 버튼들
	m_aCtrl.Add(IDC_DGN_STL_RATING_SRSP_APPLY);         // "Apply" 버튼
	m_aCtrl.Add(IDC_DGN_STL_RATING_SRSP_CLOSE);         // "Close" 버튼

}

void CDgnStlRatingServiceParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CDgnStlRatingServiceParamDlg)
	DDX_Radio(pDX, IDC_DGN_STL_RATING_OPT_ADD_RDO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO,           m_nElemType);
	DDX_Control(pDX, IDC_DGN_STL_RATING_SRSP_TAB, m_Tab);
	DDX_Control(pDX, IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK, m_Ctrl_BothIJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlRatingServiceParamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlRatingServiceParamDlg)
// 	ON_BN_CLICKED(IDC_DGN_STL_RATING_SRSP_CLOSE, OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_STL_RATING_SRSP_APPLY, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_STL_RATING_OPT_ADD_RDO, OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_STL_RATING_OPT_DEL_RDO, OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK, OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO,  OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)


	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_STL_RATING_SRSP_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlRatingServiceParamDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if (m_pDoc == NULL)	return TRUE;

	// 	T_CPGD_D CpgdD; CpgdD.Initialize();
	// 	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	// 	m_nRatingCode = CpgdD.iDgnCode;

	m_Ctrl_BothIJ.SetCheck(TRUE);

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_STL_RATING_SRSP_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_Tab.SetPlaceHolder(pPlaceHolder, FALSE);

	// I
	m_pSubDlg_I = new CDgnStlRatingServiceParamTabIDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_STL_RATING_I), CDgnStlRatingServiceParamTabIDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CDgnStlRatingServiceParamTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_STL_RATING_J), CDgnStlRatingServiceParamTabJDlg::IDD, TRUE);


	m_Tab.ShowTab(0);

	SetText();

	AlignControl();
	ShowHideControls();
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_APPLY)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// void CDgnStlRatingServiceParamDlg::OnDgnClose()
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnStlRatingServiceParamDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {
		Dlg2Data(rData.TfType);
		rData.bBothIJ = m_bBothIJ;
		};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_SRSP_D rData; 
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrsp(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_SRVP_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrvp(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelSrsp(selKeys) :
			m_pDoc->m_pDataCtrl->DelSrvp(selKeys);
		if (success) Initial_SelectItem();
	};


	// Select된 Element를 얻어옵니다.
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_STL_RATING_BEAM_NON_MEMBER), MB_OK);
			return;
		}
		if (m_nOption == 0)	// add/replace
			ExcuteAddReplace(aSelKey, true);
		else
			ExcuteDelete(aSelKey, true);
	}
	else if (m_nElemType == ELEM_TYPE_VBEAM)
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_STL_RATING_BEAM_NON_MEMBER), MB_OK);
			return;
		}

		if (m_nOption == 0)
			ExcuteAddReplace(aSelKey, false);
		else
			ExcuteDelete(aSelKey, false);
	}
}

void CDgnStlRatingServiceParamDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlRatingServiceParamDlg::OnBothIJChk()
{
	m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if (m_Tab.GetCurSel() == 1)
	{
		m_pSubDlg_J->ControlsEnableDisable(m_bBothIJ);
	}
}

void CDgnStlRatingServiceParamDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

}


BOOL CDgnStlRatingServiceParamDlg::Dlg2Data(T_SRSP_TYPE* pTfType)
{
	UpdateData(TRUE);

	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetSrspData();
			pTfType[0] = m_I_Data;
			pTfType[1] = pTfType[0];
		}
		else
		{
			m_pSubDlg_I->GetSrspData();
			pTfType[0] = m_I_Data;
			pTfType[1] = m_J_Data;
		}
	}
	else
	{
		if (m_bBothIJ)
		{
			pTfType[0] = m_I_Data;
			pTfType[1] = pTfType[0];
		}
		else
		{
			m_pSubDlg_J->GetSrspData();
			pTfType[0] = m_I_Data;
			pTfType[1] = m_J_Data;
		}
	}

	return TRUE;
}

void CDgnStlRatingServiceParamDlg::AlignControl()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	int nDistY;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_CS454_20)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnStlRatingServiceParamDlg::ShowHideControls()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_CS454_20)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}


void CDgnStlRatingServiceParamDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if (m_Tab.GetCurSel() == 0)
	{
		m_pSubDlg_I->ResizeWindow();
	}
	else
	{
		m_pSubDlg_J->ResizeWindow();
	}*/
}

void CDgnStlRatingServiceParamDlg::OnDgnCPGLoadAppAddDel()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK)->EnableWindow(m_nOption == 0);

	if (m_Tab.GetCurSel() == 0)
	{
		m_pSubDlg_I->ControlsEnableDisable();
	}
	else
	{
		m_pSubDlg_J->ControlsEnableDisable(m_bBothIJ);
	}
}

void CDgnStlRatingServiceParamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CDgnStlRatingServiceParamDlg::UpdateBuffer()
{
	m_Tab.DeleteAllItems();
	// I
	m_pSubDlg_I = new CDgnStlRatingServiceParamTabIDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_STL_RATING_I), CDgnStlRatingServiceParamTabIDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CDgnStlRatingServiceParamTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_STL_RATING_J), CDgnStlRatingServiceParamTabJDlg::IDD, TRUE);
	
	m_Tab.ShowTab(0);


	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);
}

void CDgnStlRatingServiceParamDlg::SetDgnCode(int nDgnCode)
{
	m_nRatingCode = nDgnCode;
}

int  CDgnStlRatingServiceParamDlg::GetDgnCode()
{
	return m_nRatingCode;
}

void CDgnStlRatingServiceParamDlg::SetText()
{
	GetDlgItem(IDC_DGN_STL_RATING_OPT_GRP)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_OPT_GRP));
	GetDlgItem(IDC_DGN_STL_RATING_OPT_ADD_RDO)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_OPT_ADD_RDO));
	GetDlgItem(IDC_DGN_STL_RATING_OPT_DEL_RDO)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_OPT_DEL_RDO));
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_IJBOTH_CHK)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_IJBOTH_CHK));
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_PLACEHOLDER)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_PLACEHOLDER));
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_APPLY)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_APPLY));
	GetDlgItem(IDC_DGN_STL_RATING_SRSP_CLOSE)->SetWindowText(_LS(IDS_DGN_STL_RATING_SERVICE_PARAM_CLOSE));
}
