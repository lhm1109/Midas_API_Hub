// DgnGenSplnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSplnDlg.h"

#include "CPGTranPosiDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_LENG
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSplnDlg dialog
const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnGenSplnDlg::CDgnGenSplnDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenSplnDlg::IDD, pParent)
{
	m_SpanLength = 0.0;
	m_pDoc = CDBDoc::GetDocPoint();	                
}

void CDgnGenSplnDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_SPLN_L_EDT,     m_LengCtrl);
	DDX_Control(pDX, IDC_DGN_SPLN_L_UNT,     m_LengUnit);
	DDX_Radio(pDX,   IDC_DGN_LENGTH_AR,      m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Text(pDX,    IDC_DGN_SPLN_L_EDT,     m_SpanLength);	
}

BEGIN_MESSAGE_MAP(CDgnGenSplnDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_LENGTH_AR,  OnDgnLengthAr)
	ON_BN_CLICKED(IDC_DGN_LENGTH_DEL, OnDgnLengthDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSplnDlg message handlers

BOOL CDgnGenSplnDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();
	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	
	return TRUE;
}

BOOL CDgnGenSplnDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_SpanLength < 0.)		bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_UNBRACED_LEN),MB_OK);

	return bCheck;
}

void CDgnGenSplnDlg::Initial_Data()
{
	m_LengCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_LengCtrl.SetAttWcsDistance();
	m_LengCtrl.SetDistValueMode(TRUE);	
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;

	m_SpanLength = 0.0;

	UpdateData(FALSE);
}

void CDgnGenSplnDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_SPLN_STC);
	m_aPositionCtrl.Add(IDC_DGN_SPLN_L_STC);
	m_aPositionCtrl.Add(IDC_DGN_SPLN_L_EDT);
	m_aPositionCtrl.Add(IDC_DGN_SPLN_L_UNT);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}

void CDgnGenSplnDlg::AlignControl()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SPLN_STC)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	int nDistY;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_AASHTO_LRFD19)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnGenSplnDlg::ShowHideControls()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_AASHTO_LRFD19)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}

void CDgnGenSplnDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CDgnGenSplnDlg::Execute() 
{
	auto SetCommonData = [this](auto& rData) {
		UpdateData(TRUE);
		rData.dSpanL = m_SpanLength;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_SPLN_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSpln(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_SPLV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSplv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelSpln(selKeys) :
			m_pDoc->m_pDataCtrl->DelSplv(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey, rSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key, eData);
			// Beam, Truss.
			if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(rSelKey, true);
		else
			ExcuteDelete(rSelKey, true);
	}
	else if (m_nElemType == ELEM_TYPE_VBEAM)
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, false);
		else
			ExcuteDelete(aSelKey, false);
	}
}

void CDgnGenSplnDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenSplnDlg::OnDgnLengthAr()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnGenSplnDlg::OnDgnLengthDel() 
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnGenSplnDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}

// Add by ZINU.('01.1.3).
void CDgnGenSplnDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				// Add by ZINU.('01.1.3).
				//Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

