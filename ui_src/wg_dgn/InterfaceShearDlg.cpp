// InterfaceShearDlg.cpp: implementation of the CInterfaceShearDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "InterfaceShearDlg.h"
#include "InterfaceShearTabDlg.h"
#include "InterfaceShearTabDlg_CSA.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CInterfaceShearDlg::CInterfaceShearDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CInterfaceShearDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();

#if defined(_CIVIL)
	T_PSCD_D rData;
	m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
	m_iDgnCode = rData.iDgnCode;
#endif
	
		m_pSubDlg_I = new CInterfaceShearTabDlg(0, this); // Tab_I
		m_pSubDlg_J = new CInterfaceShearTabDlg(1, this); // Tab_J

	m_nCurTab = 0;
	m_nOption = 0;
	m_bBothIJ = TRUE;
}



void CInterfaceShearDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio  (pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_OPTION_ADD_RDO,              m_nOption);                   
	DDX_Check  (pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_BOTH_CHK,                    m_bBothIJ);
	DDX_Control(pDX, IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_TAB,                         m_ctrlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterfaceShearDlg, CMenuBarChildDlg)

	ON_BN_CLICKED(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_OPTION_ADD_RDO, OnDgnConnectorClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_OPTION_DEL_RDO, OnDgnConnectorClassOptionRdo)
	ON_BN_CLICKED(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_BOTH_CHK,       OnDgnConnectorClassBothChk)

	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInterfaceShearDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CInterfaceShearDlg::UpdateBuffer()
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
		case(UR_PSCS_ADD):
		case(UR_PSCS_DEL):
			// Test
			//Initial_Data();
			break;
		}
	}  
}

void CInterfaceShearDlg::InitTab()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_PLACEHOLDER);//get text properties
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pSubDlg_I = new CInterfaceShearTabDlg(0, this);
	m_ctrlTab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_I), CInterfaceShearTabDlg::IDD, TRUE);
	m_pSubDlg_J = new CInterfaceShearTabDlg(1, this);
	m_ctrlTab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_PSC_EXPOSURE_CLASS_J), CInterfaceShearTabDlg::IDD, TRUE);


	m_ctrlTab.ShowTab(0);
}

void CInterfaceShearDlg::SetSubDlg()
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


void CInterfaceShearDlg::Data2Dlg()
{
	m_bBothIJ = m_Data.bBothIJ;
}

void CInterfaceShearDlg::Dlg2Data()
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
		m_Data.nFaceState[1]  = m_Data.nFaceState[0];
		m_Data.dBvi[1]        = m_Data.dBvi[0];
		m_Data.dAvf[1]        = m_Data.dAvf[0];
		m_Data.dfy[1]         = m_Data.dfy[0];
	}

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CInterfaceShearDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitTab();
	Data2Dlg();

	UpdateData(FALSE);

	OnDgnConnectorClassOptionRdo();
	OnDgnConnectorClassBothChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInterfaceShearDlg::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			ElemPairK EPairK(key, EN_EL_BEAM);
			if(CDBLib::IsPscDgnFor1st(EPairK,TRUE))	rSelKey.Add(key);
		}

		if(m_nOption==0)	// add/replace
		{
			Dlg2Data();

			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddPscs(rSelKey,m_Data)) Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelPscs(rSelKey)) Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CInterfaceShearDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CInterfaceShearDlg::OnDgnConnectorClassOptionRdo() 
{
	UpdateData(TRUE);

	if(!m_nOption) // Add/Replace
	{
		GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_BOTH_CHK)->EnableWindow(TRUE);               
	}
	else // Del
	{
		GetDlgItem(IDC_DGN_DESIGN_INTERFACE_SHEAR_CLASS_BOTH_CHK)->EnableWindow(FALSE);
	}

	SetSubDlg();
}

void CInterfaceShearDlg::OnDgnConnectorClassBothChk() 
{
	UpdateData(TRUE);

	m_Data.bBothIJ = m_bBothIJ;
	SetSubDlg();
}

void CInterfaceShearDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetSubDlg();
}