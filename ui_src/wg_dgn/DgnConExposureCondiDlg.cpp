// DgnConExposureCondiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConExposureCondiDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConExposureCondiDlg dialog


CDgnConExposureCondiDlg::CDgnConExposureCondiDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConExposureCondiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConExposureCondiDlg)
	//}}AFX_DATA_INIT

	m_nOption = 0;
	m_nExposure = 0;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConExposureCondiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConExposureCondiDlg)
	DDX_Radio(pDX, IDC_DGN_EXPOSURE_AR_RDO,  m_nOption);
	DDX_Radio(pDX, IDC_DGN_EXPOSURE_DRY_RDO, m_nExposure);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConExposureCondiDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConExposureCondiDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConExposureCondiDlg message handlers
void CDgnConExposureCondiDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConExposureCondiDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

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
			// Beam
			if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_BEAM) rSelKey.Add(key);
		}

		T_REXC_D rData; rData.Initialize();

		if(m_nOption==0)	// Add/Replace
		{
			rData.nExposure = m_nExposure;

			if(rSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddRexc(aSelKey, rData))
				{
					if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
				}
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_BEAM_TYPE),MB_OK);
		}
		else	// Delete
		{
			if(m_pDoc->m_pDataCtrl->DelRexc(aSelKey))
			{
				if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		} 
		
		Update_InitDataByCode();
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

BOOL CDgnConExposureCondiDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Update_InitDataByCode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConExposureCondiDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// 
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// 
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// 
			break;
		default:
			ASSERT(TRUE);
	}
}

void CDgnConExposureCondiDlg::Update_InitDataByCode()
{
	CString strDgnCode = CDBLib::GetConCodeName();

	BOOL bShow = FALSE;

    if ( strDgnCode== CONCODE_KCI_USD12 || strDgnCode == CONCODE_KDS_41_30_2018 || strDgnCode == CONCODE_KDS_41_20_2022 ) { bShow = TRUE; }

	GetDlgItem(IDC_DGN_EXPOSURE_OPT_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXPOSURE_AR_RDO )->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXPOSURE_DEL_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_EXPOSURE_GRP    )->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXPOSURE_DRY_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXPOSURE_ETC_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}
