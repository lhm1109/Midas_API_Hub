// CPGCurveBridDlg.cpp: implementation of the CCPGCurveBridDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGCurveBridDlg.h"
#include "CPGCurveBridTabIDlg.h"
#include "CPGCurveBridTabJDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "../wg_base/DlgUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CCPGCurveBridDlg::CCPGCurveBridDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CCPGCurveBridDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_pDoc = CDBDoc::GetDocPoint();	

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bBothIJ = FALSE;
}

void CCPGCurveBridDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Control(pDX, IDC_DGN_CPG_CPCB_TAB,         m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_CPCB_IJBOTH_CHK,  m_Ctrl_BothIJ);
}


BEGIN_MESSAGE_MAP(CCPGCurveBridDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGLoadAppAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_CPCB_IJBOTH_CHK,  OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_CPG_CPCB_TAB, OnSelchangeTab)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGCurveBridDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;	

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;
}

void CCPGCurveBridDlg::InitCtrl()
{
	m_Ctrl_BothIJ.SetCheck(TRUE);

	m_Tab.DeleteAllItems();

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_CPG_CPCB_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_Tab.SetPlaceHolder(pPlaceHolder, FALSE);

	// I
	m_pSubDlg_I = new CCPGCurveBridTabIDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGCurveBridTabIDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGCurveBridTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGCurveBridTabJDlg::IDD, TRUE);

	m_Tab.ShowTab(0);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPCB_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPCB_TAB);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPCB_PLACEHOLDER);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPCB_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CPCB_CLOSE);
}

void CCPGCurveBridDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CPG_CPCB_IJBOTH_CHK)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CCPGCurveBridDlg::ShowHideControls()
{
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(m_nDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
	GetDlgItem(IDC_DGN_CPG_CPCB_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CPG_CPCB_APPLY)->ShowWindow(SW_HIDE);
}

void CCPGCurveBridDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CCPGCurveBridDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {

		T_CPCB_TYPE arType[2];
		Dlg2Data(arType);
		memcpy(&rData.TfType[0], &arType[0], sizeof(T_CPCB_TYPE));
		memcpy(&rData.TfType[1], &arType[1], sizeof(T_CPCB_TYPE));
		rData.bBothIJ = m_bBothIJ;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_CPCB_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCpcb(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_CPCV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCpcv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelCpcb(selKeys) :
			m_pDoc->m_pDataCtrl->DelCpcv(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, true);
		else
			ExcuteDelete(aSelKey, true);
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

BOOL CCPGCurveBridDlg::Dlg2Data(T_CPCB_TYPE* pTfType)
{
	UpdateData(TRUE);

	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCpcbData();
			pTfType[0] = m_I_Data;
			pTfType[1] = pTfType[0];
		}
		else
		{
			m_pSubDlg_I->GetCpcbData();
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
			pTfType[0] = m_I_Data;
			m_pSubDlg_J->GetCpcbData();
			pTfType[1] = m_J_Data;
		}
	}

	return TRUE;
}

void CCPGCurveBridDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGCurveBridDlg::OnBothIJChk() 
{
	m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->ControlsEnableDisable();	
}

void CCPGCurveBridDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bBothIJ = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 0) m_pSubDlg_I->ResizeWindow();
	else m_pSubDlg_J->ResizeWindow();
}

void CCPGCurveBridDlg::OnDgnCPGLoadAppAddDel() 
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_DGN_CPG_CPCB_IJBOTH_CHK)->EnableWindow(m_nOption==OPTION_ADD_REPLACE);

	EnableDisableControls();
	if(m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
	else                       m_pSubDlg_J->ControlsEnableDisable();	
}

void CCPGCurveBridDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}

void CCPGCurveBridDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
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

void CCPGCurveBridDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bMFD = FALSE;
	BOOL bOther=FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_CPGD_ADD):
		case(UR_CPGD_DEL):
			
			T_CPGD_D CpgdD; CpgdD.Initialize();
			m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
			m_nDgnCode = CpgdD.iDgnCode;

			BOOL bAllowCode = [](int nCode)
			{
				switch (nCode)
				{
				case AASHTO_LRFD07_CSG:
				case AASHTO_LRFD12_CSG:
				case AASHTO_LRFD16_CSG:
				case AASHTO_LRFD17_CSG:
				case AASHTO_LRFD20_CSG:
				case KDS_24_14_31_2018_CSG:
				case KSCE_LSD15_CSG:
				case KSSC_2014_CSG:
				case CSA_S6_14_CSG:
				case CSA_S6_19_CSG:
					return TRUE;
				default:
					return FALSE;
				}					
			}(m_nDgnCode);
													    
			GetDlgItem(IDC_DGN_CPG_OPT_GRP)->EnableWindow(bAllowCode);
			GetDlgItem(IDC_DGN_CPG_OPT_ADD_RDO)->EnableWindow(bAllowCode);
			GetDlgItem(IDC_DGN_CPG_OPT_DEL_RDO)->EnableWindow(bAllowCode);

			GetDlgItem(IDC_DGN_CPG_CPCB_IJBOTH_CHK)->EnableWindow(bAllowCode && m_nOption==0);
			GetDlgItem(IDC_DGN_CPG_CPCB_TAB)->EnableWindow(bAllowCode && m_nOption==0);

			if(m_Tab.GetCurSel() == 0) m_pSubDlg_I->ControlsEnableDisable();
			else                       m_pSubDlg_J->ControlsEnableDisable();

			if(m_Tab.GetCurSel() == 0) m_pSubDlg_I->ResizeWindow();
			else                       m_pSubDlg_J->ResizeWindow();			
			break;
		}
	}  
}

void CCPGCurveBridDlg::ResizeWindow()
{
	CRect rPage = 0;
	if(m_Tab.GetCurSel() == 0) rPage = m_pSubDlg_I->GetResizeHeight();
	else                       rPage = m_pSubDlg_J->GetResizeHeight();
	
	int nHeight = rPage.bottom - rPage.top;
	
	CRect rTo;
	CRect rMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;
	
	// Tab Control Resize
	CWnd* pWnd = GetDlgItem(IDC_DGN_CPG_CPCB_TAB);
	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.top + nHeight + globalUtils.ScaleByDPI(28);
	pWnd->SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width(), rMove.Height(), SWP_NOMOVE);
	
	// PlaceHolder Resize
	pWnd = GetDlgItem(IDC_DGN_CPG_CPCB_PLACEHOLDER);
	pWnd->GetWindowRect(rMove);
	rMove.bottom = rMove.top + nHeight;
	pWnd->SetWindowPos(NULL, rMove.left, rMove.top, rMove.Width(), rMove.Height(), SWP_NOMOVE);

	// Move Controls
// 	aControls.RemoveAll();
// 	aControls.Add(IDC_DGN_CPG_CPCB_APPLY);
// 	aControls.Add(IDC_DGN_CPG_CPCB_CLOSE);
// 
// 	GetDlgItem(IDC_DGN_CPG_CPCB_TAB)->GetWindowRect(rTo);
// 	GetDlgItem(IDC_DGN_CPG_CPCB_APPLY)->GetWindowRect(rMove);
// 
// 	nDistY = rTo.bottom - rMove.top + globalUtils.ScaleByDPI(10);
// 
// 	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
}

void CCPGCurveBridDlg::SetDgnCode(int nDgnCode)
{
	m_nDgnCode = nDgnCode;
}

int  CCPGCurveBridDlg::GetDgnCode()
{
	return m_nDgnCode;
}

void CCPGCurveBridDlg::SubDlgCtrlEnableDisable()
{
	OnInitDialog();
	OnDgnCPGLoadAppAddDel();
}