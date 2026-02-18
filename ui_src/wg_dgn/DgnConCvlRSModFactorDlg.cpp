// DgnConCvlRSModFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCvlRSModFactorDlg.h"

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
// CDgnConCvlRSModFactorDlg dialog


CDgnConCvlRSModFactorDlg::CDgnConCvlRSModFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConCvlRSModFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCvlRSModFactorDlg)

	m_nOption = -1;

	m_dFx = 0.0;
	m_dFy = 0.0;
	m_dFz = 0.0;
	m_dMx = 0.0;
	m_dMy = 0.0;
	m_dMz = 0.0;
	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConCvlRSModFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCvlRSModFactorDlg)
	DDX_Radio(pDX, IDC_WG_DGN_OPTION_ADD_RDO, m_nOption);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_FX_EDT, m_dFx);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_FY_EDT, m_dFy);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_FZ_EDT, m_dFz);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_MX_EDT, m_dMx);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_MY_EDT, m_dMy);
	DDX_Text(pDX, IDC_WG_DGN_FACTOR_MZ_EDT, m_dMz);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConCvlRSModFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConCvlRSModFactorDlg)
	ON_BN_CLICKED(IDC_WG_DGN_OPTION_ADD_RDO, OnDgnAddReplaceRdo)
	ON_BN_CLICKED(IDC_WG_DGN_OPTION_DEL_RDO, OnDgnDelRdo)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlRSModFactorDlg message handlers
BOOL CDgnConCvlRSModFactorDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	// Default 값 초기화
	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCvlRSModFactorDlg::OnDgnExecute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> frmSelKey;
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
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))
				frmSelKey.Add(key);
		}

		T_RMFR_D rData; rData.Initialize();
		
		if(m_nOption==0)	// add/replace
		{
			rData.dFx = m_dFx;
			rData.dFy = m_dFy;
			rData.dFz = m_dFz;
			rData.dMx = m_dMx;
			rData.dMy = m_dMy;
			rData.dMz = m_dMz;

			if(frmSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddRmfr(frmSelKey,rData)) Initial_SelectItem();
			}
			else
			{
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
			}
		}
		else	// Delete
		{
 	  		if(m_pDoc->m_pDataCtrl->DelRmfr(frmSelKey))	Initial_SelectItem();
			else AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	}
	else AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConCvlRSModFactorDlg::Initial_SelectItem()
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnConCvlRSModFactorDlg::Initial_Data()
{
	m_nOption = 0; // add/replace

	m_dFx = 1.0;
	m_dFy = 1.0;
	m_dFz = 1.0;
	m_dMx = 1.0;
	m_dMy = 1.0;
	m_dMz = 1.0;

	UpdateData(FALSE);
}

void CDgnConCvlRSModFactorDlg::OnDgnClose()
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConCvlRSModFactorDlg::OnDgnAddReplaceRdo()
{
	// TODO: Add your control notification handler code here
	m_nOption = 0;
	UpdateData(FALSE);
}

void CDgnConCvlRSModFactorDlg::OnDgnDelRdo()
{
	// TODO: Add your control notification handler code here
	m_nOption = 1;
	UpdateData(FALSE);
}