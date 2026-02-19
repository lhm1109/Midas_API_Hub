// DgnGenSeisDgnTypeByMembDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSeisDgnTypeByMembDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DBCodeCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisDgnTypeByMembDlg dialog


CDgnGenSeisDgnTypeByMembDlg::CDgnGenSeisDgnTypeByMembDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenSeisDgnTypeByMembDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenSeisDgnTypeByMembDlg)
	m_nOption = -1;
	m_nOptSeis= -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenSeisDgnTypeByMembDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSeisDgnTypeByMembDlg)
	DDX_Radio(pDX, IDC_DGN_SEIS_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_SEIS_DGN_RDO, m_nOptSeis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenSeisDgnTypeByMembDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenSeisDgnTypeByMembDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_SEIS_AR, OnDgnSeismicAr)
	ON_BN_CLICKED(IDC_DGN_SEIS_DEL, OnDgnSeismicDel)
	ON_BN_CLICKED(IDC_DGN_SEIS_DGN_RDO,     OnDgnSeisDgn)
	ON_BN_CLICKED(IDC_DGN_NON_SEIS_DGN_RDO, OnDgnNonSeisDgn)
	ON_BN_CLICKED(IDC_DGN_NON_SEIS_FORCE_RDO, OnDgnNonSeisForce)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisDgnTypeByMembDlg message handlers

BOOL CDgnGenSeisDgnTypeByMembDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnExecute() 
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
		T_SDGN_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
			rData.nType = m_nOptSeis;
		  if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddSdgn(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelSdgn(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenSeisDgnTypeByMembDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenSeisDgnTypeByMembDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_nOptSeis = 1;

	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnSeismicAr() 
{
	m_nOption  = 0;			//	add/replace
	EnableAssignMemberCtrl(TRUE);
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnSeismicDel() 
{
	m_nOption  = 1;			//	Delete
	EnableAssignMemberCtrl(FALSE);
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::EnableAssignMemberCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_SEIS_DGN_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_NON_SEIS_DGN_RDO)->EnableWindow(bEnable);
	
	BOOL bEnableByConditon = FALSE;
	if(bEnable)
	{
		T_DCON_D DconD;
		if(!m_pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();

		if (CDBCodeCtrl::IsConCodeSeisDgnType(DconD.DesignCode) && DconD.bSpecialSeismic)
		{
			if(DconD.nClass==0) bEnableByConditon = TRUE; // Select Frame Type이 _T("Special Moment Frames")인 경우.
		}
	}  

	GetDlgItem(IDC_DGN_NON_SEIS_FORCE_RDO)->EnableWindow(bEnableByConditon);
	GetDlgItem(IDC_DGN_NON_SEIS_FORCE_STC)->EnableWindow(bEnableByConditon);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnSeisDgn()
{
	m_nOptSeis = 0;     // 0: Seismic Design
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnNonSeisDgn()
{
	m_nOptSeis = 1;     // 1: Non-Seismic Design
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::OnDgnNonSeisForce()
{
	m_nOptSeis = 2;     // 1: Non-Seismic Force Resisting System
	UpdateData(FALSE);
}

void CDgnGenSeisDgnTypeByMembDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
			  // do something...
				Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

// Coded by Seungjun ('20070516) MNet:No.2831.
void CDgnGenSeisDgnTypeByMembDlg::Update_InitDataByCode()
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_SDGN, strTitle, bShow);

	GetDlgItem(IDC_DGN_SEIS_AR)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_DEL)->EnableWindow(bShow);
	EnableAssignMemberCtrl(bShow);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}