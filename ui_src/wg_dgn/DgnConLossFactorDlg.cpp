// DgnConLossFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConLossFactorDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConLossFactorDlg dialog


CDgnConLossFactorDlg::CDgnConLossFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConLossFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConLossFactorDlg)
		m_dTranLoss = 0.0;
		m_dServLoss = 0.0;
	//}}AFX_DATA_INIT
}

void CDgnConLossFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConLossFactorDlg)
	DDX_Text(pDX, IDC_DGN_LOSS_FACTOR_TRANSFER, m_dTranLoss);
	DDX_Text(pDX, IDC_DGN_LOSS_FACTOR_SERVICE, m_dServLoss);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConLossFactorDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CDgnConLossFactorDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConLossFactorDlg message handlers
BOOL CDgnConLossFactorDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitData();

	UpdateData(FALSE);

	return TRUE;
}

void CDgnConLossFactorDlg::OnCancel() 
{
	// TODO: PSc extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConLossFactorDlg::OnOK() 
{
	UpdateData(TRUE);

	BOOL bCheck = TRUE;

	if(ErrorCheck())
	{
		CString strDgnCode=_T("");
	  T_DCON_D rData;
	  rData.Initialize();
	  // Exist Concrete Control Data at DB.
	  if(!m_pDoc->m_pAttrCtrl->GetDcon(rData))
		{
			CDgnDataCtrl DataCtrl;
			DataCtrl.Get_DgnConDcon(rData);
		}

        GetDataFromDlg(rData);

		bCheck = m_pDoc->m_pDataCtrl->AddDcon(rData);
	}

	if(bCheck)
		CDialogMove::OnOK();
}

void CDgnConLossFactorDlg::InitData()
{
	//Tmp. Setting..
	CString strDgnCode=_T("");
	T_DCON_D rData;
	rData.Initialize();
	// Exist Concrete Control Data at DB.
	if(!m_pDoc->m_pAttrCtrl->GetDcon(rData))
	{
		CDgnDataCtrl DataCtrl;
		DataCtrl.Get_DgnConDcon(rData);
	}

	m_dTranLoss = rData.dTranLossFactor;
	m_dServLoss = rData.dServLossFactor;
}

BOOL CDgnConLossFactorDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if(m_dTranLoss <= 0.0 || m_dTranLoss > 1.0 || m_dServLoss <= 0.0 || m_dServLoss > 1.0)	
	{
		bCheck = FALSE;	
		strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_LOSS_FACTOR);
	}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CDgnConLossFactorDlg::GetDataFromDlg(T_DCON_D& rData)
{
    rData.dTranLossFactor = m_dTranLoss;
    rData.dServLossFactor = m_dServLoss;

    return;
}