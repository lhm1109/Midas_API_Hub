// CPGLateraTorsionDlg.cpp: implementation of the CCPGLateraTorsionDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGLateraTorsionDlg.h"
#include "CPGLateralTorsionTabDlg.h"
#include "CPGLateralTorsionTabJDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CCPGLateraTorsionDlg::CCPGLateraTorsionDlg(CWnd* pParent /*=NULL*/, int nType/* = 0*/)  //nType =0 :PSC Rating.  1: Steel Rating 
	: CMenuBarChildDlg(CCPGLateraTorsionDlg::IDD, pParent)
{
	m_nType = nType;
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_bBothIJ = TRUE;
	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;
}

void CCPGLateraTorsionDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Check(pDX, IDC_DGN_CPG_IJBOTH_CHK,       m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_CPG_TAB,              m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_IJBOTH_CHK,       m_Ctrl_BothIJ);
}

BEGIN_MESSAGE_MAP(CCPGLateraTorsionDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO, OnDgnCPGTorsionAdd)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO, OnDgnCPGTorsionDel)
	ON_BN_CLICKED(IDC_DGN_CPG_IJBOTH_CHK, OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGLateraTorsionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CCPGLateralTorsionTabDlg(this, m_nType);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGLateralTorsionTabDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGLateralTorsionTabJDlg(this, m_nType);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGLateralTorsionTabJDlg::IDD, TRUE);		

	m_Tab.ShowTab(0);

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;
}

void CCPGLateraTorsionDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_CPG_TAB);
	m_aPositionCtrl.Add(IDC_DGN_CPG_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_CPG_CLOSE);
}

void CCPGLateraTorsionDlg::AlignControl()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	CRect rRef;
	CRect rToMove;	
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CPG_IJBOTH_CHK)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	int nDistY;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || (RcdsD.nRatingCode != STL_CS454_20 && !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode)))
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CCPGLateraTorsionDlg::ShowHideControls()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || (RcdsD.nRatingCode != STL_CS454_20 && !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode)))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
	GetDlgItem(IDC_DGN_CPG_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CPG_APPLY)->ShowWindow(SW_HIDE);
}

void CCPGLateraTorsionDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

BOOL CCPGLateraTorsionDlg::Dlg2Data(T_CGLT_DATA* pLTdata)
{
	UpdateData(TRUE);

	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCgltData();
			pLTdata[0] = m_I_Data;
			pLTdata[1] = pLTdata[0];
		}
		else
		{
			m_pSubDlg_I->GetCgltData();
			pLTdata[0] = m_I_Data;
			pLTdata[1] = m_J_Data;
		}
	}
	else
	{
		if (m_bBothIJ)
		{
			pLTdata[0] = m_I_Data;
			pLTdata[1] = pLTdata[0];
		}
		else
		{
			pLTdata[0] = m_I_Data;
			m_pSubDlg_J->GetCgltData();
			pLTdata[1] = m_J_Data;
		}
	}

	return TRUE;
}

void CCPGLateraTorsionDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {
		Dlg2Data(rData.LTdata);
		rData.bBothIJ = m_bBothIJ;		
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_CGLT_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCglt(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_CGLV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddCglv(selKeys, rData)) Initial_SelectItem();
		}
		};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelCglt(selKeys) :
			m_pDoc->m_pDataCtrl->DelCglv(selKeys);
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

void CCPGLateraTorsionDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGLateraTorsionDlg::OnBothIJChk() 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);		
}

void CCPGLateraTorsionDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;
}

void CCPGLateraTorsionDlg::OnDgnCPGTorsionAdd() 
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CCPGLateraTorsionDlg::OnDgnCPGTorsionDel() 
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CCPGLateraTorsionDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}
