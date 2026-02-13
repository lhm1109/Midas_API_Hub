// DgnConCvlCtcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCvlCtcDlg.h"

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
// CDgnConCvlCtcDlg dialog


CDgnConCvlCtcDlg::CDgnConCvlCtcDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConCvlCtcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCvlCtcDlg)
	m_nOption = -1;
	m_dCtcUpFactor = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConCvlCtcDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCvlCtcDlg)
	DDX_Radio(pDX, IDC_DGN_CTC_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CTC_UP_FACTOR, m_dCtcUpFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConCvlCtcDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConCvlCtcDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CTC_AR, OnDgnCtcAr)
	ON_BN_CLICKED(IDC_DGN_CTC_DEL, OnDgnCtcDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlCtcDlg message handlers

void CDgnConCvlCtcDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConCvlCtcDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
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
			// Beam.
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rSelKey.Add(key);
		}
		T_SUFC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
            UpdateData(TRUE);
			
            //(2009.06.15) Add by Unsang :: Data Checking..
			if(ErrorCheck() == FALSE) return;   

		    rData.dCtcUpFactor = m_dCtcUpFactor;
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddSufc(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelSufc(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}


void CDgnConCvlCtcDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CDgnConCvlCtcDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCvlCtcDlg::Initial_Data()
{
	m_nOption = 0;				//	add/replace
	m_dCtcUpFactor = 1.0;	//	Scale up Factor의 초기화

	UpdateData(FALSE);
}

BOOL CDgnConCvlCtcDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_dCtcUpFactor < 1.0)	bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_CTC_FACTOR),MB_OK);

	return bCheck;
}

void CDgnConCvlCtcDlg::OnDgnCtcAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConCvlCtcDlg::OnDgnCtcDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}
