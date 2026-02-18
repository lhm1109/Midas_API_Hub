// ShearConnectorDlg.cpp: implementation of the CShearConnectorDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CShearConnectorDlg::CShearConnectorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CShearConnectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorDlg)

	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

	T_PSCD_D rData;
	m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
	m_iDgnCode = rData.iDgnCode;

	if (m_iDgnCode == BS5400_90_PSC|| m_iDgnCode == TMH07_3_1989)
	{ 
		m_Data.nFaceState[0] = 1;
	}

	if (m_iDgnCode == IRS_PSC)
	{
		m_pSubDlg_IRS_I = new CShearConnectorTabI_IRS_Dlg(this);
		m_pSubDlg_IRS_J = new CShearConnectorTabJ_IRS_Dlg(this);
	}
	else if (m_iDgnCode == BS5400_90_PSC )
	{
	
		m_pSubDlg_BS_I = new CShearConnectorTabI_BS_Dlg(this);
		m_pSubDlg_BS_J = new CShearConnectorTabJ_BS_Dlg(this);
	}
	else if ( m_iDgnCode == TMH07_3_1989)
	{

		m_pSubDlg_TMH_I = new CShearConnectorTabI_TMH_Dlg(this);
		m_pSubDlg_TMH_J = new CShearConnectorTabJ_TMH_Dlg(this);
	}
	else
	{
		m_pSubDlg_I = new CShearConnectorTabIDlg(this);
		m_pSubDlg_J = new CShearConnectorTabJDlg(this);
	}	

	m_nCurTab = 0;
	m_nOption = 0;
	m_bBothIJ = TRUE;

	m_nElemType = ELEM_TYPE_ELEMENT;

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_CONNECTOR_CLASS_TAB);
	m_aMoveCtrl.Add(IDC_DGN_DESIGN_CONNECTOR_CLASS_PLACEHOLDER);
}

void CShearConnectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorDlg)
									 
	DDX_Radio  (pDX, IDC_DGN_DESIGN_CONNECTOR_CLASS_OPTION_ADD_RDO,              m_nOption);                   
	DDX_Check  (pDX, IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK,                    m_bBothIJ);
	DDX_Radio  (pDX, IDC_DGN_PSC_ELEM_RDO,                                       m_nElemType);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_CLASS_TAB,                         m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearConnectorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CShearConnectorDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,                                 OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,                               OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_DESIGN_CONNECTOR_CLASS_OPTION_ADD_RDO, OnDgnConnectorClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_CONNECTOR_CLASS_OPTION_DEL_RDO, OnDgnConnectorClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK,       OnDgnConnectorClassBothChk)
	ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_DESIGN_CONNECTOR_CLASS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShearConnectorDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
		// do something...
			break;
		case D_UPDATE_UNIT:
		// do something...
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
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

void CShearConnectorDlg::UpdateBuffer()
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
			case(UR_PSCN_ADD):
			case(UR_PSCN_DEL):
				// Test
				//Initial_Data();
			break;
		}
	}  
}

void CShearConnectorDlg::InitTab()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_PLACEHOLDER);//get text properties
	ASSERT(pPlaceHolder);
 	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	if (m_iDgnCode == IRS_PSC)
	{
		m_pSubDlg_IRS_I = new CShearConnectorTabI_IRS_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_IRS_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CShearConnectorTabI_IRS_Dlg::IDD, TRUE);
		m_pSubDlg_IRS_J = new CShearConnectorTabJ_IRS_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_IRS_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CShearConnectorTabJ_IRS_Dlg::IDD, TRUE);
	}
	else if (m_iDgnCode == BS5400_90_PSC )
	{
		m_pSubDlg_BS_I = new CShearConnectorTabI_BS_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_BS_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CShearConnectorTabI_BS_Dlg::IDD, TRUE);
		m_pSubDlg_BS_J = new CShearConnectorTabJ_BS_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_BS_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CShearConnectorTabJ_BS_Dlg::IDD, TRUE);
	}
	else if (m_iDgnCode == TMH07_3_1989)
	{
		m_pSubDlg_TMH_I = new CShearConnectorTabI_TMH_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_TMH_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CShearConnectorTabI_TMH_Dlg::IDD, TRUE);
		m_pSubDlg_TMH_J = new CShearConnectorTabJ_TMH_Dlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_TMH_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CShearConnectorTabJ_TMH_Dlg::IDD, TRUE);
	}
	else
	{
		m_pSubDlg_I = new CShearConnectorTabIDlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CShearConnectorTabIDlg::IDD, TRUE);
		m_pSubDlg_J = new CShearConnectorTabJDlg(this);
		m_ctrlTab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CShearConnectorTabJDlg::IDD, TRUE);
	}
	
	m_ctrlTab.ShowTab(0);
}

void CShearConnectorDlg::SetSubDlg()
{
	m_nCurTab = m_ctrlTab.GetCurSel();

	if(m_nCurTab == 0)
	{
		if (m_iDgnCode == IRS_PSC)
		{
			m_pSubDlg_IRS_I->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_IRS_I->ControlsEnableDisable();
		}
		else if (m_iDgnCode == BS5400_90_PSC )
		{
			m_pSubDlg_BS_I->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_BS_I->ControlsEnableDisable();
		}
		else if ( m_iDgnCode == TMH07_3_1989)
		{
			m_pSubDlg_TMH_I->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_TMH_I->ControlsEnableDisable();
		}
		else
		{
			m_pSubDlg_I->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_I->ControlsEnableDisable();
		}	
	}
	else
	{
		if (m_iDgnCode == IRS_PSC)
		{
			m_pSubDlg_IRS_J->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_IRS_J->ControlsEnableDisable();
		}
		else if (m_iDgnCode == BS5400_90_PSC )
		{
			m_pSubDlg_BS_J->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_BS_J->ControlsEnableDisable();
		}
		else if ( m_iDgnCode == TMH07_3_1989)
		{
			m_pSubDlg_TMH_J->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_TMH_J->ControlsEnableDisable();
		}
		else
		{
			m_pSubDlg_J->SetData(m_bBothIJ, m_nOption);
			m_pSubDlg_J->ControlsEnableDisable();
		}		
	}
}

void CShearConnectorDlg::Data2Dlg()
{
	 m_bBothIJ = m_Data.bBothIJ;
}

void CShearConnectorDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_nCurTab = m_ctrlTab.GetCurSel();

	m_Data.bBothIJ = m_bBothIJ;

	if(m_nCurTab == 0)
	{
		if (m_iDgnCode == IRS_PSC)
		{
			m_pSubDlg_IRS_I->Dlg2Data();
			m_pSubDlg_IRS_I->GetData(m_Data);
		}
		else if (m_iDgnCode == BS5400_90_PSC )
		{ 
			m_pSubDlg_BS_I->Dlg2Data();
			m_pSubDlg_BS_I->GetData(m_Data);
		}
		else if (m_iDgnCode == TMH07_3_1989)
		{
			m_pSubDlg_TMH_I->Dlg2Data();
			m_pSubDlg_TMH_I->GetData(m_Data);
		}
		else
		{
			m_pSubDlg_I->Dlg2Data();
			m_pSubDlg_I->GetData(m_Data);
		}
	
	}
	else
	{
		if (m_iDgnCode == IRS_PSC)
		{
			m_pSubDlg_IRS_J->Dlg2Data();
			m_pSubDlg_IRS_J->GetData(m_Data);
		}
		else if (m_iDgnCode == BS5400_90_PSC )
		{
			m_pSubDlg_BS_J->Dlg2Data();
			m_pSubDlg_BS_J->GetData(m_Data);
		}
		else if ( m_iDgnCode == TMH07_3_1989)
		{
			m_pSubDlg_TMH_J->Dlg2Data();
			m_pSubDlg_TMH_J->GetData(m_Data);
		}
		else
		{
			m_pSubDlg_J->Dlg2Data();
			m_pSubDlg_J->GetData(m_Data);
		}		
	}

	if(m_Data.bBothIJ)
	{
		if (m_iDgnCode == IRS_PSC)
		{
			m_Data.nFaceState[1] = m_Data.nFaceState[0];
			m_Data.IRS_SConn.dSL[1] 		 = m_Data.IRS_SConn.dSL[0] ;
			m_Data.IRS_SConn.dASL[1]		 = m_Data.IRS_SConn.dASL[0];
			m_Data.IRS_SConn.dSt[1] 		 = m_Data.IRS_SConn.dSt[0] ;
			m_Data.IRS_SConn.dAe[1] 		 = m_Data.IRS_SConn.dAe[0] ;
			m_Data.IRS_SConn.dFck[1]		 = m_Data.IRS_SConn.dFck[0];
			m_Data.IRS_SConn.dFy[1]  		 = m_Data.IRS_SConn.dFy[0] ;
		}
		else if (m_iDgnCode == BS5400_90_PSC || m_iDgnCode == TMH07_3_1989)
		{
			m_Data.nFaceState[1] = m_Data.nFaceState[0];
			m_Data.dLs[1] = m_Data.dLs[0];
			m_Data.dAe[1] = m_Data.dAe[0];
			m_Data.dfy[1] = m_Data.dfy[0];
		}
		else
		{
			m_Data.nFaceState[1] = m_Data.nFaceState[0];
			m_Data.dAngle[1]     = m_Data.dAngle[0];
			m_Data.dAw[1]        = m_Data.dAw[0];
			m_Data.dfy[1]        = m_Data.dfy[0];
		}		
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CShearConnectorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitTab();
	Data2Dlg();

	HideAndMoveControlVBemByDB();

	UpdateData(FALSE);

	OnDgnConnectorClassOptionRdo();
	OnDgnConnectorClassBothChk();

	if (m_iDgnCode == BS5400_90_PSC || m_iDgnCode == TMH07_3_1989)
	{
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_INTERFACE_SHEAR));
	}
	else
	{
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK)->SetWindowText(_LS(IDS_DGN_BOTH_SHEAR_CONNECT));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShearConnectorDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}


void CShearConnectorDlg::OnDgnPSCElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}


void CShearConnectorDlg::OnDgnExecute() 
{
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
					if (!m_pDoc->m_pDataCtrl->AddPscn(rSelKey, m_Data)) return;
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (!m_pDoc->m_pDataCtrl->DelPscn(rSelKey)) return;
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
					if (!m_pDoc->m_pDataCtrl->AddPscv(rSelKey, m_Data)) return;
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (!m_pDoc->m_pDataCtrl->DelPscv(rSelKey)) return;
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CShearConnectorDlg::HideAndMoveControlVBemByDB()
{
	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK)->GetWindowRect(rToMove);

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

void CShearConnectorDlg::OnDgnConnectorClassOptionRdo() 
{
	UpdateData(TRUE);

	if(!m_nOption) // Add/Replace
	{
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK)->EnableWindow(TRUE);               
	}
	else // Del
	{
		GetDlgItem(IDC_DGN_DESIGN_CONNECTOR_CLASS_BOTH_CHK)->EnableWindow(FALSE);
	}

	SetSubDlg();
}

void CShearConnectorDlg::OnDgnConnectorClassBothChk() 
{
	UpdateData(TRUE);
	 m_Data.bBothIJ = m_bBothIJ;
	SetSubDlg();
}

void CShearConnectorDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetSubDlg();
}