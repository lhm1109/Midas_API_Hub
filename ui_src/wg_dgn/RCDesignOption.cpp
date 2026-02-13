// RCDesignOption.cpp: implementation of the CRCDesignOption class.
// Add by sshan. MNET:2607.('20061220)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCDesignOption.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "CRCDataCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CRCDesignOption::CRCDesignOption(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CRCDesignOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCDesignOption)
	m_nOption = -1;
	m_nMOption = -1;
	m_nSOption = -1;
	m_nAOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CRCDesignOption::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)
	DDX_Radio(pDX, IDC_DGN_RC_DORC_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_RC_DORC_M1, m_nMOption);
	DDX_Radio(pDX, IDC_DGN_RC_DORC_S1, m_nSOption);
	DDX_Radio(pDX, IDC_DGN_RC_DORC_A1, m_nAOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCDesignOption, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CRCDesignOption)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_RC_DORC_AR, OnDgnCRCDorcAr)
	ON_BN_CLICKED(IDC_DGN_RC_DORC_DEL, OnDgnCRCDorcDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CRCDesignOption::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCDesignOption::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CRCDesignOption::OnDgnExecute() 
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
			//modify by maxiao 2007.01.08
			T_ELEM_D ElemD;
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,ElemD))  ElemD.Initialize();
			T_MATD_D MatdD;
			m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD);
			if(MatdD.Type==_T("C")) //concret 단면만 가능
				rSelKey.Add(key);
		}

		T_DORC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.iMomChk = m_nMOption;
			rData.iShrChk = m_nSOption;
			rData.iAxisChk = m_nAOption;
			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddDorc(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_RC_CON_CHECK),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelDorc(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CRCDesignOption::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CRCDesignOption::Initial_Data()
{
	m_nOption  = 0;	// add/replace

	m_nMOption  = 3;
	m_nSOption  = 3;
	m_nAOption  = 3;

	UpdateData(FALSE);
}

void CRCDesignOption::OnDgnCRCDorcAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CRCDesignOption::OnDgnCRCDorcDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

