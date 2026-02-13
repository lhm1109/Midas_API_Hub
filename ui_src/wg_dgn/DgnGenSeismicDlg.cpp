// DgnGenSeismicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSeismicDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeismicDlg dialog


CDgnGenSeismicDlg::CDgnGenSeismicDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenSeismicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenSeismicDlg)
	m_nOption = -1;
	m_nOptSeis= -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenSeismicDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSeismicDlg)
	DDX_Radio(pDX, IDC_DGN_SEIS_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_SEIS_LOADS, m_nOptSeis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenSeismicDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenSeismicDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_SEIS_AR, OnDgnSeismicAr)
	ON_BN_CLICKED(IDC_DGN_SEIS_DEL, OnDgnSeismicDel)
	ON_BN_CLICKED(IDC_DGN_SEIS_LOADS, OnDgnSeismicLoads)
	ON_BN_CLICKED(IDC_DGN_SEIS_FORCES, OnDgnSeismicForces)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeismicDlg message handlers

BOOL CDgnGenSeismicDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Default 값 초기화
	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenSeismicDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenSeismicDlg::OnDgnExecute() 
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
		T_SLCM_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
			rData.nType = m_nOptSeis;
		  if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddSlcm(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelSlcm(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenSeismicDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenSeismicDlg::Initial_Data()
{
	m_nOption  = 0;	// add/replace
	m_nOptSeis = 0;

	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnGenSeismicDlg::OnDgnSeismicAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenSeismicDlg::OnDgnSeismicDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnGenSeismicDlg::OnDgnSeismicLoads()
{
	m_nOptSeis = 0;     // 0:Special Seismic Load Effect.		
	UpdateData(FALSE);
}

void CDgnGenSeismicDlg::OnDgnSeismicForces()
{
	m_nOptSeis = 1;     // 1:Vertical Seismic Forces.
	UpdateData(FALSE);
}

void CDgnGenSeismicDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnGenSeismicDlg::Update_InitDataByCode()
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_SEIS, strTitle, bShow);

	GetDlgItem(IDC_DGN_SEIS_AR)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_DEL)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_LOADS)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SEIS_FORCES)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}