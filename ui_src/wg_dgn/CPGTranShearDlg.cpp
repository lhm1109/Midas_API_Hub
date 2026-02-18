// CPGTranShearDlg.cpp: implementation of the CCPGTranShearDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGTranShearDlg.h"
#include "CPGTranShearTabDlg.h"
#include "CPGTranShearTabJDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;

CCPGTranShearDlg::CCPGTranShearDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCPGTranShearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranShearDlg)
	m_nOption = 0;
	m_bBothIJ = TRUE;
	m_nElemType = ELEM_TYPE_ELEMENT;

	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_I_Data.Initialize();
	m_J_Data.Initialize();

	m_bJTabEnable = FALSE;

	m_Data.Initialize();

	m_Data.Initialize();

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_DGN_CPG_SHEAR_IJBOTH_CHK);
	m_aPositionCtrl.Add(IDC_DGN_CPG_SHEAR_TAB);
	m_aPositionCtrl.Add(IDC_DGN_CPG_SHEAR_APPLY);
	m_aPositionCtrl.Add(IDC_DGN_CPG_SHEAR_CLOSE);
}


void CCPGTranShearDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCPGTranShearDlg)
	DDX_Radio  (pDX, IDC_DGN_CPG_OPT_ADD_RDO,      m_nOption);
	DDX_Check  (pDX, IDC_DGN_CPG_SHEAR_IJBOTH_CHK, m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_TAB,        m_Tab);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_IJBOTH_CHK, m_Ctrl_BothIJ);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGTranShearDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCPGTranShearDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO,      OnDgnCPGShearAdd)
	ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO,      OnDgnCPGShearDel)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_IJBOTH_CHK, OnBothIJChk)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCPGTranShearDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	m_Tab.DeleteAllItems();

	// I
	m_pSubDlg_I = new CCPGTranShearTabDlg(this);
	m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCPGTranShearTabDlg::IDD, TRUE);

	// J
	m_pSubDlg_J = new CCPGTranShearTabJDlg(this);
	m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCPGTranShearTabJDlg::IDD, TRUE);		

	m_Tab.ShowTab(0);

	AlignControl();
	ShowHideControls();

	//GetDlgItem(IDC_DGN_CPG_SHEAR_CLOSE)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_DGN_CPG_SHEAR_APPLY)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CCPGTranShearDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CCPGTranShearDlg::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			if (m_nOption == 0)	// add/replace
			{
				Dlg2Data();
				if (m_pDoc->m_pDataCtrl->AddCgsc(aSelKey, m_Data))	Initial_SelectItem();
			}
			else	// Delete
			{
				if (m_pDoc->m_pDataCtrl->DelCgsc(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			if (m_nOption == 0)	// add/replace
			{
				Dlg2Data();
				if (m_pDoc->m_pDataCtrl->AddCgsv(aSelKey, m_Data))	Initial_SelectItem();
			}
			else	// Delete
			{
				if (m_pDoc->m_pDataCtrl->DelCgsv(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_CPG_BEAM_NON_MEMBER));
	}

}

void CCPGTranShearDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CCPGTranShearDlg::OnBothIJChk() 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

	if(m_Tab.GetCurSel() == 1)
		m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);		
}

BOOL CCPGTranShearDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bBothIJ = m_Ctrl_BothIJ.GetCheck();

	if(m_Tab.GetCurSel() == 0)
	{
		if(m_Data.bBothIJ)
		{
			//I, J 가 같을 경우
			m_pSubDlg_I->GetCgscData();
			m_Data.Sconn[0] = m_I_Data;
			m_Data.Sconn[1] = m_Data.Sconn[0];			
		}
		else
		{
			m_pSubDlg_I->GetCgscData();
			m_Data.Sconn[0] = m_I_Data;
			m_Data.Sconn[1] = m_J_Data;
		}
	}
	else
	{
		if(m_Data.bBothIJ)
		{
			m_Data.Sconn[0] = m_I_Data;
			m_Data.Sconn[1] = m_Data.Sconn[0];
		}
		else
		{
			m_Data.Sconn[0] = m_I_Data;
			m_pSubDlg_J->GetCgscData();
			m_Data.Sconn[1] = m_J_Data;
		}
	}	

	return TRUE;
}

void CCPGTranShearDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;
}

void CCPGTranShearDlg::OnDgnCPGShearAdd() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CCPGTranShearDlg::OnDgnCPGShearDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CCPGTranShearDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CCPGTranShearDlg::AlignControl()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CWnd* pElementTypeGroup = GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP);
	CWnd* pMomentFactorGroup = GetDlgItem(IDC_DGN_CPG_SHEAR_IJBOTH_CHK);

	if (pElementTypeGroup == nullptr || pMomentFactorGroup == nullptr)
		return;

	CRect rElementType, rMomentFactor;
	pElementTypeGroup->GetWindowRect(rElementType);
	pMomentFactorGroup->GetWindowRect(rMomentFactor);

	const int nDistX = rElementType.left - rMomentFactor.left;
	const int nDistY = (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode)) ?
		(rElementType.top - rMomentFactor.top) :
		(rElementType.bottom - rMomentFactor.top + globalUtils.ScaleByDPI(4));

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CCPGTranShearDlg::ShowHideControls()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_CPG_SHEAR_APPLY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CPG_SHEAR_CLOSE)->ShowWindow(SW_HIDE);
}