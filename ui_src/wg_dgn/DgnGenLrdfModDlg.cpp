// DgnGenLrdfModDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLrdfModDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
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
// CDgnGenLrdfModDlg dialog


CDgnGenLrdfModDlg::CDgnGenLrdfModDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenLrdfModDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLrdfModDlg)
	m_nOption = -1;
	m_RFactor = 0.0;
	m_bReduAll = FALSE;
	m_bReduAxl = FALSE;
	m_bReduMom = FALSE;
	m_bReduShr = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenLrdfModDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLrdfModDlg)
	DDX_Radio(pDX, IDC_DGN_REDUCTION_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_REDUCTION_FACTOR, m_RFactor);
	DDX_Check(pDX, IDC_DGN_REDU_ALL_CHECK, m_bReduAll);
	DDX_Check(pDX, IDC_DGN_REDU_AXL_CHECK, m_bReduAxl);
	DDX_Check(pDX, IDC_DGN_REDU_MOM_CHECK, m_bReduMom);
	DDX_Check(pDX, IDC_DGN_REDU_SHR_CHECK, m_bReduShr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenLrdfModDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenLrdfModDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_REDUCTION_AR, OnDgnReductionAr)
	ON_BN_CLICKED(IDC_DGN_REDUCTION_DEL, OnDgnReductionDel)
	ON_BN_CLICKED(IDC_DGN_REDU_ALL_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_AXL_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_MOM_CHECK, OnDgnCtrlReduKind)
	ON_BN_CLICKED(IDC_DGN_REDU_SHR_CHECK, OnDgnCtrlReduKind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfModDlg message handlers

void CDgnGenLrdfModDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenLrdfModDlg::OnDgnExecute()
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
			// Beam, Truss, Wall.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))	rSelKey.Add(key);
		}
		T_REDU_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.Factor	 = m_RFactor;
			rData.bReduAxl = (m_bReduAll ? TRUE : m_bReduAxl);
			rData.bReduMom = (m_bReduAll ? TRUE : m_bReduMom);
			rData.bReduShr = (m_bReduAll ? TRUE : m_bReduShr);
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddRedu(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelRedu(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}


void CDgnGenLrdfModDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CDgnGenLrdfModDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/ if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenLrdfModDlg::Initial_Data()
{
	m_nOption = 0;		// add/replace
	m_RFactor = 1.0;	// Reduction Factor의 초기화
	if(CDBLib::Is_UseMbtpSubType())	// China.
	{
		m_bReduAll = TRUE;
		m_bReduAxl = FALSE;
		m_bReduMom = FALSE;
		m_bReduShr = FALSE;
	}
	else
	{
		m_bReduAll = FALSE;
		m_bReduAxl = TRUE;
		m_bReduMom = FALSE;
		m_bReduShr = FALSE;
	}
	UpdateReduKindCtrl(m_bReduAll);
	UpdateData(FALSE);
}

BOOL CDgnGenLrdfModDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_RFactor < 0.3 || m_RFactor > 1.0)	bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_LIVE_REDUCT_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenLrdfModDlg::OnDgnReductionAr()
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenLrdfModDlg::OnDgnReductionDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnGenLrdfModDlg::OnDgnCtrlReduKind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateReduKindCtrl(m_bReduAll);
	UpdateData(FALSE);
}

BOOL CDgnGenLrdfModDlg::UpdateReduKindCtrl(BOOL bReduAll)
{
	if(bReduAll)
	{
		m_bReduAxl = TRUE;
		m_bReduMom = TRUE;
		m_bReduShr = TRUE;
		GetDlgItem(IDC_DGN_REDU_AXL_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_REDU_MOM_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_REDU_SHR_CHECK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_REDU_AXL_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_REDU_MOM_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_REDU_SHR_CHECK)->EnableWindow(TRUE);
	}
	return TRUE;
}