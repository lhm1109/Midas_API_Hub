// DgnTowerTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnTowerTypeDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnTowerTypeDlg dialog


CDgnTowerTypeDlg::CDgnTowerTypeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnTowerTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnTowerTypeDlg)
	m_nOption = -1;
	m_nElemType = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnTowerTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnTowerTypeDlg)
	DDX_Radio(pDX, IDC_DGN_TYPE_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_TYPE_YYZZVV, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnTowerTypeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnTowerTypeDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_TYPE_AR, OnDgnTypeAr)
	ON_BN_CLICKED(IDC_DGN_TYPE_DEL, OnDgnTypeDel)
	ON_BN_CLICKED(IDC_DGN_TYPE_YYZZVV, OnDgnTypeYYZZVV)
	ON_BN_CLICKED(IDC_DGN_TYPE_YYZZ, OnDgnTypeYYZZ)
	ON_BN_CLICKED(IDC_DGN_TYPE_VV, OnDgnTypeVV)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnTowerTypeDlg message handlers

void CDgnTowerTypeDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnTowerTypeDlg::OnDgnExecute() 
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
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Truss.
			if(m_pDoc->m_pAttrCtrl->IsTruss(eData.eltyp))	rSelKey.Add(key);
		}
		T_MBTP_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
			if(m_nElemType==0)       rData.nMbType = 1;	// yy-zz-vv.
			else if(m_nElemType==1)  rData.nMbType = 2;	// yy-zz.
			else if(m_nElemType==2)  rData.nMbType = 3;	// vv.

			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddMbtp(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelMbtp(rSelKey))	Initial_SelectItem();
	  }
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnTowerTypeDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnTowerTypeDlg::OnDgnTypeAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	Add/Replace
	UpdateData(FALSE);
}

void CDgnTowerTypeDlg::OnDgnTypeDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnTowerTypeDlg::OnDgnTypeYYZZVV() 
{
	// TODO: Add your control notification handler code here
	m_nElemType = 0;	// yy-zz-vv.
	UpdateData(FALSE);
}

void CDgnTowerTypeDlg::OnDgnTypeYYZZ() 
{
	// TODO: Add your control notification handler code here
	m_nElemType = 1;	// yy-zz.
	UpdateData(FALSE);
}

void CDgnTowerTypeDlg::OnDgnTypeVV() 
{
	// TODO: Add your control notification handler code here
	m_nElemType = 2;	// vv.
	UpdateData(FALSE);
}

BOOL CDgnTowerTypeDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_nElemType < 0)	bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);

	return bCheck;
}

BOOL CDgnTowerTypeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// 선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();

	return TRUE;
}

void CDgnTowerTypeDlg::Initial_Data()
{
	m_nOption   = 0;	// Add/Replace
	m_nElemType = 1;	// Initialize yy-zz.
	UpdateData(FALSE);
}
