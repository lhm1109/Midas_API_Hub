// ExposureClassDlg.cpp: implementation of the CExposureClassDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ExposureClassDlg.h"
#include "ExposureClassTabDlg.h"
#include "ExposureClassTabJDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CExposureClassDlg::CExposureClassDlg(CWnd* pParent /*=NULL*/, int nDgnCategory/* = 0*/)
	: CMenuBarChildDlg(CExposureClassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExposureClassDlg)

	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	m_nDgnCategory = nDgnCategory;

#if defined(_CIVIL)

	if(m_nDgnCategory == 0)
	{
		T_PSCD_D PscdD;
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		m_iDgnCode = PscdD.iDgnCode;
	}
	else
	{
		T_RACD_D RacdD; RacdD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
		m_iDgnCode = RacdD.nRatingCode;
	}
	
#endif

	m_pSubDlg_I = new CExposureClassTabDlg(this);
	m_pSubDlg_J = new CExposureClassTabJDlg(this);

	m_nCurTab = 0;
	m_nOption = 0;
	m_bBothIJ = TRUE;

	m_nElemType = ELEM_TYPE_ELEMENT;

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_EXPOSURE_CLASS_PLACEHOLDER);
}


void CExposureClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExposureClassDlg)
	DDX_Radio  (pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_ADD_RDO,              m_nOption);
	DDX_Radio(pDX, IDC_DGN_PSC_ELEM_RDO,                                      m_nElemType);
	DDX_Check  (pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK,                    m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB,                         m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExposureClassDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CExposureClassDlg)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_ADD_RDO, OnDgnExposureClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_OPTION_DEL_RDO, OnDgnExposureClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK,       OnDgnExposureClassBothChk)
	ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_DESIGN_EXPOSURE_CLASS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExposureClassDlg::InitTab()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_PLACEHOLDER);//get text properties
	ASSERT(pPlaceHolder);
 	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pSubDlg_I = new CExposureClassTabDlg(this);
	m_ctrlTab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CExposureClassTabDlg::IDD, TRUE);
	m_pSubDlg_J = new CExposureClassTabJDlg(this);
	m_ctrlTab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CExposureClassTabJDlg::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CExposureClassDlg::SetSubDlg()
{
	m_nCurTab = m_ctrlTab.GetCurSel();

	if(m_nCurTab == 0)
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

void CExposureClassDlg::Data2Dlg()
{
	m_bBothIJ = m_Data.bBothIJ;
}

void CExposureClassDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_nCurTab = m_ctrlTab.GetCurSel();

	m_Data.bBothIJ = m_bBothIJ;

	if(m_nCurTab == 0)
	{
		m_pSubDlg_I->Dlg2Data();
		m_pSubDlg_I->GetData(m_Data);
	}
	else
	{
		m_pSubDlg_J->Dlg2Data();
		m_pSubDlg_J->GetData(m_Data);
	}

	if(m_Data.bBothIJ)
	{
		m_Data.iExposureClassJ[0] = m_Data.iExposureClassI[0];
		m_Data.iExposureClassJ[1] = m_Data.iExposureClassI[1];
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CExposureClassDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitTab();
	Data2Dlg();
	HideAndMoveControlVBemByDB();

	UpdateData(FALSE);

	OnDgnExposureClassOptionRdo();
	OnDgnExposureClassBothChk();
	
	if (m_iDgnCode == SNiP_20503_84_PSC || m_iDgnCode == SP_35_13330_11_PSC || m_iDgnCode == SNiP_20503_84_PSC_MKS || m_iDgnCode == SP_35_13330_11_PSC_MKS)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_PSC_CRACK_WIDTH_TITLE_NAME));
	}
	else if (m_iDgnCode == BS5400_90_PSC|| m_iDgnCode == TMH07_3_1989 || m_iDgnCode == CS_454_20_PSC_RATING )
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_ENVIRONMENT_TYPE));
	}
	else if (m_iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_ENVIRONMENTAL_AGGRESSIVENESS_CLASS));
	}
	else
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_EXPOSURE_CLASS));
	}

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExposureClassDlg::OnDgnPSCElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CExposureClassDlg::Execute() 
{
	////////////////////////////////////////////////////////////////////////
	// PSC 와 Rating PSC 에서 동일한 DB 를 사용하도록 요청 2020.04.13
	//  - 개인적인 생각 : 코드가 깔끔하려면 DB 가 분리되야 할것 같은데...
	////////////////////////////////////////////////////////////////////////

	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				ElemPairK EPairK(key, EN_EL_BEAM);
				T_ELEM_D eData;
				eData.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(key, eData)) continue;
				if (CDBLib::IsPscDgnFor1st(EPairK, TRUE))	rSelKey.Add(key);
			}

			if (m_nOption == 0)	// add/replace
			{
				Dlg2Data();

				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddPesc(rSelKey, m_Data, m_nDgnCategory))
					{
						if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
					}
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelPesc(rSelKey, m_nDgnCategory))
				{
					if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
				}
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			ASSERT(0);
			// The function below must be applied according to the virtual beam.
			// Refer to the ELEM_TYPE_ELEMENT case as a guideline.
			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				ElemPairK EPairK(key, EN_EL_VBEAM);
				
				if (CDBLib::IsPscDgnFor1st(EPairK, TRUE))	rSelKey.Add(key);
			}

			if (m_nOption == 0)	// add/replace
			{
				Dlg2Data();

				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddPesv(rSelKey, m_Data, m_nDgnCategory))
					{
						if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
					}
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelPesv(rSelKey, m_nDgnCategory))
				{
					if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
				}
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}


void CExposureClassDlg::HideAndMoveControlVBemByDB()
{
	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->GetWindowRect(rToMove);

	BOOL bPSCDgnVbem = CDBLib::ActivatePscVBEM();
	int nDistY;

	if (m_iDgnCode == EUROCODE2_2_05_PSC)
	{
		if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !bPSCDgnVbem)
		{
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
			m_nElemType = ELEM_TYPE_ELEMENT;
		}
		else
		{
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, TRUE);
		}
	}
	else
	{
		nDistY = rRef.top - rToMove.top;
		m_nElemType = ELEM_TYPE_ELEMENT;
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
	}

	CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl, nDistY);
}

void CExposureClassDlg::OnDgnExposureClassOptionRdo() 
{
	UpdateData(TRUE);

	if(!m_nOption) // Add/Replace
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->EnableWindow(TRUE);
	}
	else // Del
	{
		GetDlgItem(IDC_DGN_DESIGN_EXPOSURE_CLASS_BOTH_CHK)->EnableWindow(FALSE);
	}

	SetSubDlg();
}

void CExposureClassDlg::OnDgnExposureClassBothChk() 
{
	UpdateData(TRUE);

	SetSubDlg();
}

void CExposureClassDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetSubDlg();
}