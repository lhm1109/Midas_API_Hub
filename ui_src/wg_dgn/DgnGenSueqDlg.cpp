// DgnGenSueqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSueqDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSueqDlg dialog


CDgnGenSueqDlg::CDgnGenSueqDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenSueqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenSueqDlg)
	m_dEqLcaseN = 0.0;
	m_dEqLcaseM = 0.0;
	m_dEqLcaseV = 0.0;
	m_dEqLcomN  = 0.0;
	m_dEqLcomM  = 0.0;
	m_dEqLcomV  = 0.0;
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenSueqDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSueqDlg)
	DDX_Text(pDX, IDC_DGN_LC_AXIAL, m_dEqLcaseN);
	DDX_Text(pDX, IDC_DGN_LC_MOM, m_dEqLcaseM);
	DDX_Text(pDX, IDC_DGN_LC_SHR, m_dEqLcaseV);
	DDX_Text(pDX, IDC_DGN_LCB_AXIAL, m_dEqLcomN);
	DDX_Text(pDX, IDC_DGN_LCB_MOM, m_dEqLcomM);
	DDX_Text(pDX, IDC_DGN_LCB_SHR, m_dEqLcomV);
	DDX_Radio(pDX, IDC_DGN_SUEQ_AR_Radio, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenSueqDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenSueqDlg)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_SUEQ_AR_Radio, OnDGNSUEQARRadio)
	ON_BN_CLICKED(IDC_DGN_SUEQ_DEL_Radio, OnDGNSUEQDELRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSueqDlg message handlers
BOOL CDgnGenSueqDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	// Default 값 초기화
	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenSueqDlg::OnDgnExecute() 
{
	//Hong,jiseon 2002.7.20
	UpdateData(TRUE);
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey; // DB에서 받아오기
	CArray<T_ELEM_K,T_ELEM_K> frmSelKey; //선요소 key만 따로 
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
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))
				frmSelKey.Add(key); // Beam, Truss, Wall.
		}
		T_SUEQ_D rData;
		rData.Initialize();
		BOOL bInitCheck = FALSE;
		if(m_nOption==0)	// add/replace
		{
			rData.dEqLcaseN = m_dEqLcaseN;
			rData.dEqLcaseM = m_dEqLcaseM;
			rData.dEqLcaseV = m_dEqLcaseV;
			rData.dEqLcomN  = m_dEqLcomN;
			rData.dEqLcomM  = m_dEqLcomM;
			rData.dEqLcomV  = m_dEqLcomV;

			if(frmSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddSueq(frmSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
 	  	if(m_pDoc->m_pDataCtrl->DelSueq(frmSelKey))	Initial_SelectItem();
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
	  }
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenSueqDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenSueqDlg::Initial_Data()
{
	m_nOption = 0; // add/replace
	m_dEqLcaseN = 1.0;
	m_dEqLcaseM = 1.0;
	m_dEqLcaseV = 1.0;
	m_dEqLcomN  = 1.0;
	m_dEqLcomM  = 1.0;
	m_dEqLcomV  = 1.0;

	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnGenSueqDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenSueqDlg::OnDGNSUEQARRadio() 
{
	// TODO: Add your control notification handler code here
	m_nOption = 0;
	UpdateData(FALSE);
}

void CDgnGenSueqDlg::OnDGNSUEQDELRadio() 
{
	// TODO: Add your control notification handler code here
	m_nOption = 1;
	UpdateData(FALSE);
}

BOOL CDgnGenSueqDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	if(m_dEqLcaseN < 0.0)				bCheck = FALSE;
	else if(m_dEqLcaseM < 0.0)	bCheck = FALSE;
	else if(m_dEqLcaseV < 0.0)	bCheck = FALSE;
	else if(m_dEqLcomN  < 0.0)	bCheck = FALSE;
	else if(m_dEqLcomM  < 0.0)	bCheck = FALSE;
	else if(m_dEqLcomV  < 0.0)	bCheck = FALSE;
	
//	if(!bCheck) AfxMessageBox(_LS(IDS_DGN_CHK_SUEQ_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenSueqDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
				Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			ASSERT(TRUE);
	}
}

void CDgnGenSueqDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_SUEQ, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_LC_AXIAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LC_MOM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LC_SHR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LCB_AXIAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LCB_MOM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LCB_SHR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SUEQ_AR_Radio)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SUEQ_DEL_Radio)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_LC_AXIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LC_MOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LC_SHR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LCB_AXIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LCB_MOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LCB_SHR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SUEQ_AR_Radio)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SUEQ_DEL_Radio)->EnableWindow(FALSE);
	}
}
