// DgnConTRFTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConStrutAngleDlg.h"

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
// CDgnConTRFTDlg dialog


CDgnConStrutAngleDlg::CDgnConStrutAngleDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConStrutAngleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConTRFTDlg)
	m_nOption = -1;
	m_dStrutAngle = 45.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

}


void CDgnConStrutAngleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConTRFTDlg)
	DDX_Radio(pDX, IDC_DGN_CON_STRUT_ANGLE_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_STRUT_ANGLE, m_dStrutAngle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConStrutAngleDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConTRFTDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_STRUT_ANGLE_AR, OnDgnStrutAngleAr)
	ON_BN_CLICKED(IDC_DGN_CON_STRUT_ANGLE_DEL, OnDgnStrutAngleDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConTRFTDlg message handlers
void CDgnConStrutAngleDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConStrutAngleDlg::OnDgnExecute() 
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
			int nMembTyp = m_pDoc->m_pAttrCtrl->GetMemberType(key);
			// Beam
			if(nMembTyp == D_MBTP_BEAM || nMembTyp == D_MBTP_COLUMN) rSelKey.Add(key);
		}

		T_STAN_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			UpdateData(TRUE);
			rData.dAngle = m_dStrutAngle;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddStan(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_BEAM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelTrft(rSelKey))	Initial_SelectItem();
		}
		
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConStrutAngleDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConStrutAngleDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConStrutAngleDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_dStrutAngle = 45.0;

	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnConStrutAngleDlg::OnDgnStrutAngleAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConStrutAngleDlg::OnDgnStrutAngleDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConStrutAngleDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnConStrutAngleDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_STAN, strTitle, bShow, m_pDoc->IsPostMode());
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_STRUT_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}
