// DgnSODDesignParameterBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSODDesignParameterBaseDlg.h"
#include "DgnSODDesignParameterRUSDlg.h"
 
#include "DgnDataCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\AttrCtrl2.h"

///////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnSODDesignParameterBaseDlg dialog


CDgnSODDesignParameterBaseDlg::CDgnSODDesignParameterBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSODDesignParameterBaseDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pRUSDlg = 0;
	m_nCurrPage = 0;
}
CDgnSODDesignParameterBaseDlg::~CDgnSODDesignParameterBaseDlg()
{
}

void CDgnSODDesignParameterBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
}

// BEGIN_MESSAGE_MAP(CDgnSODDesignParameterBaseDlg, CDialogMove)
// END_MESSAGE_MAP()


void CDgnSODDesignParameterBaseDlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	
	if(!m_pDoc->m_pDataCtrl->AddSdpa(m_MainData))
		return;
	
	CDialogMove::OnOK();		
}

BOOL CDgnSODDesignParameterBaseDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CCheckDialogOpen::SetSoftwareRendering();

	m_MainData.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetSdpa(m_MainData)) m_MainData.Initialize();

	int Index=0;
	m_nCurrPage = Index;

	m_pRUSDlg   = new CDgnSODDesignParameterRUSDlg(this);
	m_pRUSDlg->SetData(&m_MainData);

	m_DlgPageMgr.SetPlaceHolder(this, IDC_DGN_SOD_BASE_PLACE_HOLDER);
	m_DlgPageMgr.AddTab(m_pRUSDlg  , _T(""), CDgnSODDesignParameterRUSDlg  ::IDD, TRUE); 
	m_DlgPageMgr.ShowTab(m_nCurrPage);  

	//Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSODDesignParameterBaseDlg::Data2Dlg()
{
	m_nCurrPage = 0; // m_DesignCode.GetCurSel();
	m_pRUSDlg->Data2Dlg();
}

BOOL CDgnSODDesignParameterBaseDlg::Dlg2Data()
{
	m_nCurrPage = 0; //= m_DesignCode.GetCurSel();

	m_pRUSDlg->Dlg2Data();

	return TRUE;
}
