// CPGPosiDgnOutDlg.cpp: implementation of the CCPGPosiDgnOutDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGPosiDgnOutDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CCPGPosiDgnOutDlg::CCPGPosiDgnOutDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CCPGPosiDgnOutDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_iPosi = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

void CCPGPosiDgnOutDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		CArray<T_ELEM_K, T_ELEM_K> aSelKey;
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			if (m_nOption == OPTION_ADD_REPLACE)
			{
				Dlg2Data();
				if (m_pDoc->m_pDataCtrl->AddCgre(aSelKey, m_Data)) Initial_SelectItem();
			}
			else
			{
				if (m_pDoc->m_pDataCtrl->DelCgre(aSelKey)) Initial_SelectItem();
			}
		}
		else
		{
			AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
		}			
	}
	else
	{
		CArray<T_ELEM_K, T_ELEM_K> aSelKey;
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			if (m_nOption == OPTION_ADD_REPLACE)
			{
				Dlg2Data();

				T_CGRV_D CgrvD;
				CgrvD.iDgnPosi = m_Data.iDgnPosi;

				if (m_pDoc->m_pDataCtrl->AddCgrv(aSelKey, CgrvD))	Initial_SelectItem();
			}
			else
			{
				if (m_pDoc->m_pDataCtrl->DelCgrv(aSelKey)) Initial_SelectItem();
			}
		}
		else
		{
			AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
		}
	}	
}

void CCPGPosiDgnOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_CPG_OPT_ADD_RDO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_DGN_CPG_POSI_DGN_I_RDO, m_iPosi);
}


BEGIN_MESSAGE_MAP(CCPGPosiDgnOutDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO, OnDgnCPGPosiAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO, OnDgnCPGPosiAddDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGPosiDgnOutDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPGPosiDgnOutDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGPosiDgnOutDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_POSI_DGN_GRP);
	m_aPositionCtrl.Add(IDC_DGN_CPG_POSI_DGN_I_RDO);
	m_aPositionCtrl.Add(IDC_DGN_CPG_POSI_DGN_J_RDO);
	m_aPositionCtrl.Add(IDC_DGN_CPG_POSI_DGN_IJ_RDO);
}

void CCPGPosiDgnOutDlg::AlignControl()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CPG_POSI_DGN_GRP)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode))
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CCPGPosiDgnOutDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CCPGPosiDgnOutDlg::ShowHideControls()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}

BOOL CCPGPosiDgnOutDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.iDgnPosi = m_iPosi;

	return TRUE;
}

void CCPGPosiDgnOutDlg::OnDgnCPGPosiAddDel()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CCPGPosiDgnOutDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}
