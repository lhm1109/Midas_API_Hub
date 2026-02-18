// DgnGenEwsdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenEwsdDlg.h"

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
// CDgnGenEwsdDlg dialog


CDgnGenEwsdDlg::CDgnGenEwsdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenEwsdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenEwsdDlg)
	m_nOption = -1;
	m_bJCheck = FALSE;
	m_dISbz = 1.0;
	m_dIZTop = 1.0;
	m_dIZBot = 1.0;
	m_dJSbz = 1.0;
	m_dJZTop = 1.0;
	m_dJZBot = 1.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenEwsdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenEwsdDlg)
	DDX_Radio(pDX, IDC_DGN_EWSD_AR, m_nOption);
	DDX_Check(pDX, IDC_DGN_EWSD_J_CHECK,  m_bJCheck);
	DDX_Text(pDX, IDC_DGN_I_SBZ_EDIT, m_dISbz);
	DDX_Text(pDX, IDC_DGN_I_ZTOP_EDIT, m_dIZTop);
	DDX_Text(pDX, IDC_DGN_I_ZBOT_EDIT, m_dIZBot);
	DDX_Text(pDX, IDC_DGN_J_SBZ_EDIT, m_dJSbz);
	DDX_Text(pDX, IDC_DGN_J_ZTOP_EDIT, m_dJZTop);
	DDX_Text(pDX, IDC_DGN_J_ZBOT_EDIT, m_dJZBot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenEwsdDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenEwsdDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)	
	ON_BN_CLICKED(IDC_DGN_EWSD_AR, OnDgnEwsdAr)
	ON_BN_CLICKED(IDC_DGN_EWSD_DEL, OnDgnEwsdDel)
	ON_BN_CLICKED(IDC_DGN_EWSD_J_CHECK, OnDgnJClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenEwsdDlg message handlers

BOOL CDgnGenEwsdDlg::OnInitDialog() 
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

void CDgnGenEwsdDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenEwsdDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	int aSelKeySize = aSelKey.GetSize();
	if(aSelKeySize > 0)
	{
		for(int i=0; i<aSelKeySize; i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rSelKey.Add(key);
		}

		T_EWSF_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.EwsfSubY.dScaleFactor = m_dISbz;
			rData.EwsfSubY.dZtopFactor = m_dIZTop;
			rData.EwsfSubY.dZbotFactor = m_dIZBot;
			rData.EwsfSubY.bJ = m_bJCheck;
			rData.EwsfSubY.dScaleFactor_j = m_dJSbz;
			rData.EwsfSubY.dZtopFactor_j = m_dJZTop;
			rData.EwsfSubY.dZbotFactor_j = m_dJZBot;

			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddEwsd(rSelKey, rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_BEEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelEwsd(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenEwsdDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenEwsdDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	UpdateData(TRUE);

	if(m_dISbz <= 0.)			bCheck = FALSE;
	if(m_dIZTop <= 0.)		bCheck = FALSE;
	if(m_dIZBot <= 0.)		bCheck = FALSE;

	if(m_bJCheck)
	{
		if(m_dJSbz <= 0.)			bCheck = FALSE;
		if(m_dJZTop <= 0.)		bCheck = FALSE;
		if(m_dJZBot <= 0.)		bCheck = FALSE;
	}

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_EFFECT_WIDTH_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenEwsdDlg::Initial_Data()
{
	T_EWSF_D data;
	data.Initialize();

	m_nOption  = 0;	// add/replace

	m_dISbz  = data.EwsfSubY.dScaleFactor;
	m_dIZTop  = data.EwsfSubY.dZtopFactor;
	m_dIZBot  = data.EwsfSubY.dZbotFactor;
	m_bJCheck = data.EwsfSubY.bJ;
	m_dJSbz  = data.EwsfSubY.dScaleFactor_j;
	m_dJZTop  = data.EwsfSubY.dZtopFactor_j;
	m_dJZBot  = data.EwsfSubY.dZbotFactor_j;
	UpdateData(FALSE);

	OnDgnJClick();
}

void CDgnGenEwsdDlg::OnDgnEwsdAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenEwsdDlg::OnDgnEwsdDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnGenEwsdDlg::OnDgnJClick() 
{
	UpdateData(TRUE);

	if(m_bJCheck)
	{
		GetDlgItem(IDC_DGN_J_SBZ_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_ZTOP_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_ZBOT_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_SBZ_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_ZTOP_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_ZBOT_EDIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_J_SBZ_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_ZTOP_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_ZBOT_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_SBZ_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_ZTOP_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_ZBOT_EDIT)->EnableWindow(FALSE);
	}
}



