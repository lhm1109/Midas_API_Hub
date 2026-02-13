// CDgnSeisAnchorShearFactor.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorShearFactor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorShearFactor dialog

CDgnSeisAnchorShearFactor::CDgnSeisAnchorShearFactor(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorShearFactor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorShearFactor)	
	//}}AFX_DATA_INIT

}

void CDgnSeisAnchorShearFactor::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_SEIS_ANCHOR_SHEAR_FACTOR1_RDO, m_nShearFactorType);
	//{{AFX_DATA_MAP(CDgnSeisAnchorShearFactor)
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorShearFactor::Data2Dlg() 
{ 


	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorShearFactor::Dlg2Data() 
{
	UpdateData(TRUE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorShearFactor, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_SHEAR_FACTOR_OK_BTN,     OnOK)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_SHEAR_FACTOR_CANCEL_BTN, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorShearFactor message handlers

BOOL CDgnSeisAnchorShearFactor::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	//Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorShearFactor::OnShearFactorOK()
{
	//if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CDgnSeisAnchorShearFactor::OnShearFactorCancel()
{
	
	CDialogMove::OnCancel();
}