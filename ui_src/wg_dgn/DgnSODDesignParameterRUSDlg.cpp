// CDgnSODDesignParameterRUSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSODDesignParameterRUSDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "DgnSODDesignParameterBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSODDesignParameterRUSDlg dialog


CDgnSODDesignParameterRUSDlg::CDgnSODDesignParameterRUSDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSODDesignParameterRUSDlg::IDD, pParent)
{
	ASSERT(pParent);
	m_pParent = (CDgnSODDesignParameterBaseDlg*)pParent;  

	m_pData = NULL;

	m_aComponent.RemoveAll();
	m_aComponent.Add(IDC_DGN_SOD_PARAM_COMPONENT1_RDO);
	m_aComponent.Add(IDC_DGN_SOD_PARAM_COMPONENT2_RDO);
	m_aComponent.Add(IDC_DGN_SOD_PARAM_COMPONENT3_RDO);
	m_aComponent.Add(IDC_DGN_SOD_PARAM_COMPONENT4_RDO);

// 	m_aGamma.RemoveAll();
// 	m_aGamma.Add(IDC_DGN_SOD_PARAM_GAAMMA_AUTO_RDO);
// 	m_aGamma.Add(IDC_DGN_SOD_PARAM_GAAMMA_USER_RDO);
}

CDgnSODDesignParameterRUSDlg::~CDgnSODDesignParameterRUSDlg()
{
	
}

void CDgnSODDesignParameterRUSDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSODDesignParameterRUSDlg)	
	DDX_Text(pDX, IDC_DGN_SOD_PARAM_FACTOR_M_EDIT,	         m_dFactor_m);
	DDX_Text(pDX, IDC_DGN_SOD_PARAM_FACTOR_GAMMA_N_EDIT,	   m_dFactor_gamma);
		
	DDX_Control(pDX, IDC_DGN_SOD_PARAM_STRENGTH_CHK,				 m_bDgnParam_STN);
	DDX_Control(pDX, IDC_DGN_SOD_PARAM_STABILITY_CHK,				 m_bDgnParam_STB);
	DDX_Control(pDX, IDC_DGN_SOD_PARAM_FATIGU_CHK,					 m_bDgnParam_FAT);
	DDX_Control(pDX, IDC_DGN_SOD_PARAM_BRDTYPE_CMB,          m_CmbBRDType);

	//}}AFX_DATA_MAP
}


BOOL CDgnSODDesignParameterRUSDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	CCheckDialogOpen::SetSoftwareRendering();

	InitBridgeType();

	Data2Dlg();	

	//OnClick_Gamma(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSODDesignParameterRUSDlg::InitBridgeType()
{
	int iCountCombo = m_CmbBRDType.GetCount();
	if(iCountCombo > 0) m_CmbBRDType.ResetContent();
	
	CString asrtCmbList[2] = { _LS(IDS_DGN_BRIDGE_ROAD), _LS(IDS_DGN_RAIL_ROAD)};

	for(int i=0; i<2; ++i)
	{
		m_CmbBRDType.AddString(asrtCmbList[i]);
	}
	
	int iIndex = m_pData->nBridgeType;
	m_CmbBRDType.SetCurSel(iIndex);
}

void CDgnSODDesignParameterRUSDlg::Data2Dlg()
{
	int iIndex = m_pData->nBridgeType;
	m_CmbBRDType.SetCurSel(iIndex);

	CDlgUtil::CtrlRadioSetCheck(this, m_aComponent, m_pData->nStressComp); 
	//CDlgUtil::CtrlRadioSetCheck(this, m_aGamma,     m_pData->nSafeFactor); 

	m_dFactor_m 	    =	m_pData->dCondiFactor;
	m_dFactor_gamma 	=	m_pData->dSafeFactor;

	m_bDgnParam_STN.SetCheck(m_pData->bDgnParam[0]);   
	m_bDgnParam_STB.SetCheck(m_pData->bDgnParam[1]);   
	m_bDgnParam_FAT.SetCheck(m_pData->bDgnParam[2]);   

	UpdateData(FALSE);
}

void CDgnSODDesignParameterRUSDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->nBridgeType  = m_CmbBRDType.GetCurSel();
	CDlgUtil::CtrlRadioGetCheck(this, m_aComponent, m_pData->nStressComp); 
	//CDlgUtil::CtrlRadioGetCheck(this, m_aGamma,     m_pData->nSafeFactor); 

	m_pData->dCondiFactor = m_dFactor_m; 	     	
	m_pData->dSafeFactor  = m_dFactor_gamma; 		

	m_pData->bDgnParam[0] =   m_bDgnParam_STN.GetCheck();    
	m_pData->bDgnParam[1] =   m_bDgnParam_STB.GetCheck();    
	m_pData->bDgnParam[2] =   m_bDgnParam_FAT.GetCheck();    
}


BEGIN_MESSAGE_MAP(CDgnSODDesignParameterRUSDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnSODDesignParameterRUSDlg)
	ON_BN_CLICKED(IDOK, OnOk)
// 	ON_BN_CLICKED(IDC_DGN_SOD_PARAM_GAAMMA_AUTO_RDO, OnClick_Gamma)
// 	ON_BN_CLICKED(IDC_DGN_SOD_PARAM_GAAMMA_USER_RDO, OnClick_Gamma)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSODDesignParameterRUSDlg message handlers

void CDgnSODDesignParameterRUSDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();		
	CChildDialog::OnOK();		
}

void CDgnSODDesignParameterRUSDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}

// void CDgnSODDesignParameterRUSDlg::OnClick_Gamma() 
// {
// 	CDlgUtil::CtrlRadioGetCheck(this, m_aGamma, m_pData->nStressComp); 
// 
// 	if(m_pData->nStressComp==0) //Auto
// 	{
// 		GetDlgItem(IDC_DGN_SOD_PARAM_FACTOR_GAMMA_N_EDIT)->EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_DGN_SOD_PARAM_FACTOR_GAMMA_N_EDIT)->EnableWindow(TRUE);
// 	}
// }

