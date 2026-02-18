// ExposureClassDlg.cpp: implementation of the CExposureClassDlg_RC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ExposureClassDlg_RC.h"
#include "ExposureClassTabDlg_RC.h"
#include "ExposureClassTabJDlg_RC.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\DlgUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CExposureClassDlg_RC::CExposureClassDlg_RC(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CExposureClassDlg_RC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureClassDlg_RC)

	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_pSubDlg_I = new CExposureClassTabDlg_RC(this);
	m_pSubDlg_J = new CExposureClassTabJDlg_RC(this);

	m_nCurTab = 0;
	m_nOption = 0;
	m_bBothIJ = TRUE;

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_PLACEHOLDER);
}


void CExposureClassDlg_RC::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureClassDlg_RC)
	DDX_Radio(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_ADD_RDO, m_nOption);
	DDX_Check(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK, m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureClassDlg_RC, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CExposureClassDlg_RC)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_ADD_RDO, OnDgnExposureClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_DEL_RDO, OnDgnExposureClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK, OnDgnExposureClassBothChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExposureClassDlg_RC::InitTab()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_PLACEHOLDER); //get text properties
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pSubDlg_I = new CExposureClassTabDlg_RC(this);
	m_ctrlTab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CExposureClassTabDlg_RC::IDD, TRUE);
	m_pSubDlg_J = new CExposureClassTabJDlg_RC(this);
	m_ctrlTab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CExposureClassTabJDlg_RC::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CExposureClassDlg_RC::SetSubDlg()
{
	m_nCurTab = m_ctrlTab.GetCurSel();

	if (m_nCurTab == 0)
	{
		m_pSubDlg_I->SetData(m_bBothIJ, m_nOption);
		m_pSubDlg_I->ControlsEnableDisable();
	}
	else
	{
		m_pSubDlg_J->SetData(m_bBothIJ, m_nOption);
		m_pSubDlg_J->ControlsEnableDisable();
	}
}

void CExposureClassDlg_RC::Data2Dlg()
{
	m_bBothIJ = m_Data.bBothIJ;
}

void CExposureClassDlg_RC::Dlg2Data()
{
	UpdateData(TRUE);

	m_nCurTab = m_ctrlTab.GetCurSel();

	m_Data.bBothIJ = m_bBothIJ;

	if (m_nCurTab == 0)
	{
		m_pSubDlg_I->Dlg2Data();
		m_pSubDlg_I->GetData(m_Data);
	}
	else
	{
		m_pSubDlg_J->Dlg2Data();
		m_pSubDlg_J->GetData(m_Data);
	}

	if (m_Data.bBothIJ)
	{
		m_Data.iExposureClassJ[0] = m_Data.iExposureClassI[0];
		m_Data.iExposureClassJ[1] = m_Data.iExposureClassI[1];
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CExposureClassDlg_RC::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitTab();
	Data2Dlg();
	HideAndMoveControlForRC();

	UpdateData(FALSE);

	OnDgnExposureClassOptionRdo();
	OnDgnExposureClassBothChk();

	//if (m_iDgnCode == SNiP_20503_84_PSC || m_iDgnCode == SP_35_13330_11_PSC || m_iDgnCode == SNiP_20503_84_PSC_MKS)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_EXPOSURE_CLASS));
	}

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExposureClassDlg_RC::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(key, eData)) continue;
			rSelKey.Add(key);
		}

		if (m_nOption == 0) // add/replace
		{
			Dlg2Data();

			if (rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->AddResc(rSelKey, m_Data))
				{
					if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
				}
			}
			else AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only),MB_OK);
		}
		else // Delete
		{
			// Initialize selected Element.
			if (m_pDoc->m_pDataCtrl->DelResc(rSelKey))
			{
				if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}
	}
	else AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CExposureClassDlg_RC::OnDgnExposureClassOptionRdo()
{
	UpdateData(TRUE);

	if (!m_nOption) // Add/Replace
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->EnableWindow(TRUE);
	}
	else // Del
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->EnableWindow(FALSE);
	}

	SetSubDlg();
}

void CExposureClassDlg_RC::OnDgnExposureClassBothChk()
{
	UpdateData(TRUE);

	SetSubDlg();
}

void CExposureClassDlg_RC::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetSubDlg();
}
void CExposureClassDlg_RC::HideAndMoveControlForRC()
{
	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->GetWindowRect(rToMove);

	CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);

	int nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl, nDistY);
}
