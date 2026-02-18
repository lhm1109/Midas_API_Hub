// DgnStlRatingShearConnDlg.cpp: implementation of the CDgnStlRatingShearConnDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingShearConnDlg.h"
#include "DgnStlRatingShearConnTabDlg.h"
#include "DgnStlRatingShearConnTabJDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnStlRatingShearConnDlg::CDgnStlRatingShearConnDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CDgnStlRatingShearConnDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_bBothIJ = TRUE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;
}

void CDgnStlRatingShearConnDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_OPT_ADD_RDO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Check(pDX, IDC_DGN_SHEAR_IJBOTH_CHK, m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_SHEAR_TAB, m_Tab);
	DDX_Control(pDX, IDC_DGN_SHEAR_IJBOTH_CHK, m_Ctrl_BothIJ);
}

BEGIN_MESSAGE_MAP(CDgnStlRatingShearConnDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_OPT_ADD_RDO, OnDgnCPGShearAdd)
	ON_BN_CLICKED(IDC_DGN_OPT_DEL_RDO, OnDgnCPGShearDel)
	ON_BN_CLICKED(IDC_DGN_SHEAR_IJBOTH_CHK, OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlRatingShearConnDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	SetText();

	if (m_pDoc == NULL)	return TRUE;

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingShearConnDlg::InitCtrl()
{
	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CDgnStlRatingShearConnTabDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CDgnStlRatingShearConnTabDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CDgnStlRatingShearConnTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CDgnStlRatingShearConnTabJDlg::IDD, TRUE);

	m_Tab.ShowTab(0);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_SHEAR_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_SHEAR_TAB);
	m_aPositionCtrl.Add(IDC_DGN_SHEAR_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_SHEAR_CLOSE);
}

void CDgnStlRatingShearConnDlg::AlignControl()
{
	T_RCDS_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SHEAR_IJBOTH_CHK)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	int nDistY;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || Data.nRatingCode != STL_CS454_20)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);	
	}
	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnStlRatingShearConnDlg::ShowHideControls()
{
	T_RCDS_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || Data.nRatingCode != STL_CS454_20)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
	GetDlgItem(IDC_DGN_SHEAR_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SHEAR_APPLY)->ShowWindow(SW_HIDE);
}

void CDgnStlRatingShearConnDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CDgnStlRatingShearConnDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {
		Dlg2Data(rData.Sconn);
		rData.bBothIJ = m_bBothIJ;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_SRSC_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrsc(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_SRSV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrsv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelSrsc(selKeys) :
			m_pDoc->m_pDataCtrl->DelSrsv(selKeys);
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

void CDgnStlRatingShearConnDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlRatingShearConnDlg::SetText()
{
	GetDlgItem(IDC_DGN_OPT_GRP)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_OPTION));
	GetDlgItem(IDC_DGN_OPT_ADD_RDO)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_ADD_REPLACE));
	GetDlgItem(IDC_DGN_OPT_DEL_RDO)->SetWindowText(_LS(IDS_DGN_STL_SHEAR_CONN_ADD_DEL));
	GetDlgItem(IDC_DGN_SHEAR_IJBOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_SHEAR_CONNECT));
}

void CDgnStlRatingShearConnDlg::OnBothIJChk()
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if (m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);
}

BOOL CDgnStlRatingShearConnDlg::Dlg2Data(T_SRSC_STUD* pData)
{
	UpdateData(TRUE);

	m_bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if (m_Tab.GetCurSel() == 0)
	{
		if (m_bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetSrscData();
			pData[0] = m_I_Data;
			pData[1] = m_I_Data;
		}
		else
		{
			m_pSubDlg_I->GetSrscData();
			pData[0] = m_I_Data;
			pData[1] = m_J_Data;
		}
	}
	else
	{
		if (m_bBothIJ)
		{
			pData[0] = m_I_Data;
			pData[1] = m_I_Data;
		}
		else
		{
			pData[0] = m_I_Data;
			m_pSubDlg_J->GetSrscData();
			pData[1] = m_J_Data;
		}
	}

	return TRUE;
}

void CDgnStlRatingShearConnDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;
}

void CDgnStlRatingShearConnDlg::OnDgnCPGShearAdd()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnStlRatingShearConnDlg::OnDgnCPGShearDel()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnStlRatingShearConnDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}