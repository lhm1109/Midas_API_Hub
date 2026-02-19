// DgnCRCDgnParameter_RUS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_RUS.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnCRCDgnParameter_RUS::CDgnCRCDgnParameter_RUS(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
: CMyChildDialog(CDgnCRCDgnParameter_RUS::IDD, pParent)
{
	m_pData = pParamData;
	m_bmb7mb8 = FALSE;
	m_nmb7mb8Type = 0;
	m_bmb9 = FALSE;
	m_nmb9Type = 0;
}

CDgnCRCDgnParameter_RUS::~CDgnCRCDgnParameter_RUS()
{
}

void CDgnCRCDgnParameter_RUS::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_EC2PSC)  
	DDX_Control(pDX, IDC_RUS_INPUT_BRIDGE_TYPE_CMB, m_cmbBridgeType);
	DDX_Check  (pDX, IDC_RUS_INPUT_MB7_MB8_CHK,     m_bmb7mb8);
	DDX_Radio  (pDX, IDC_RUS_INPUT_MB7_MB8_RDO1,    m_nmb7mb8Type);
	DDX_Control(pDX, IDC_RUS_INPUT_MB7_EDT,         m_edtmb7);
	DDX_Control(pDX, IDC_RUS_INPUT_MB8_EDT,         m_edtmb8);
	DDX_Check  (pDX, IDC_RUS_INPUT_MB9_CHK,         m_bmb9);
	DDX_Radio  (pDX, IDC_RUS_INPUT_MB9_RDO1,        m_nmb9Type);
	DDX_Control(pDX, IDC_RUS_INPUT_MB9_EDT,         m_edtmb9);	
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER1_CHK,  m_b1stGrp_1);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER2_CHK,  m_b1stGrp_2);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER3_CHK,  m_b1stGrp_3);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER4_CHK,  m_b1stGrp_4);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER5_CHK,  m_b2ndGrp_1);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER6_CHK,  m_b2ndGrp_2);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER7_CHK,  m_b2ndGrp_3);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER8_CHK,  m_b2ndGrp_4);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER9_CHK,  m_b2ndGrp_5);
	DDX_Check  (pDX, IDC_RUS_PRINT_PARAMETER10_CHK, m_b2ndGrp_6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_RUS, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_EC2PSC)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB7_MB8_CHK,  OnConditionFactormb7mb8Chk)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB7_MB8_RDO1, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB7_MB8_RDO2, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB7_MB8_RDO3, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB7_MB8_RDO4, OnConditionFactormb7mb8TypeRdo)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB9_CHK,      OnConditionFactormb9Chk)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB9_RDO1,     OnConditionFactormb9TypeRdo)
	ON_BN_CLICKED(IDC_RUS_INPUT_MB9_RDO2,     OnConditionFactormb9TypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////

BOOL CDgnCRCDgnParameter_RUS::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	InitUnit();
	InitCombo();
	Data2Dlg();

	EnableDisableControls();
		
	return TRUE;  
}

void CDgnCRCDgnParameter_RUS::OnConditionFactormb7mb8Chk()
{
	EnableDisableControls();
}

void CDgnCRCDgnParameter_RUS::OnConditionFactormb7mb8TypeRdo()
{
	EnableDisableControls();
}

void CDgnCRCDgnParameter_RUS::OnConditionFactormb9Chk()
{
	EnableDisableControls();
}

void CDgnCRCDgnParameter_RUS::OnConditionFactormb9TypeRdo()
{
	EnableDisableControls();
}

void CDgnCRCDgnParameter_RUS::InitUnit()
{
	m_edtmb7.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb8.SetEditUnit(D_UNITSYS_NONE);
	m_edtmb9.SetEditUnit(D_UNITSYS_NONE);
}

void CDgnCRCDgnParameter_RUS::InitCombo()
{
	m_cmbBridgeType.ResetContent();
	m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_ROADWAY));
	m_cmbBridgeType.AddString(_LS(IDS_DGN_BRIDGE_TYPE_RAILROAD));
	m_cmbBridgeType.SetCurSel(0);
}

void CDgnCRCDgnParameter_RUS::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_RUS_INPUT_MB7_MB8_RDO1)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_RUS_INPUT_MB7_MB8_RDO2)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_RUS_INPUT_MB7_MB8_RDO3)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_RUS_INPUT_MB7_MB8_RDO4)->EnableWindow(m_bmb7mb8);
	GetDlgItem(IDC_RUS_INPUT_MB7_STC     )->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type==3);
	GetDlgItem(IDC_RUS_INPUT_MB7_EDT     )->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type==3);
	GetDlgItem(IDC_RUS_INPUT_MB8_STC     )->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type==3);
	GetDlgItem(IDC_RUS_INPUT_MB8_EDT     )->EnableWindow(m_bmb7mb8 && m_nmb7mb8Type==3);

	GetDlgItem(IDC_RUS_INPUT_MB9_RDO1)->EnableWindow(m_bmb9);
	GetDlgItem(IDC_RUS_INPUT_MB9_RDO2)->EnableWindow(m_bmb9);
	GetDlgItem(IDC_RUS_INPUT_MB9_STC )->EnableWindow(m_bmb9 && m_nmb9Type==1);
	GetDlgItem(IDC_RUS_INPUT_MB9_EDT )->EnableWindow(m_bmb9 && m_nmb9Type==1);
}

BOOL CDgnCRCDgnParameter_RUS::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->iBrgType = m_cmbBridgeType.GetCurSel();

	m_pData->bmb7mb8     = m_bmb7mb8;
	m_pData->nmb7mb8Type = m_nmb7mb8Type;
	m_pData->dmb7        = m_edtmb7.GetEditValue();
	m_pData->dmb8        = m_edtmb8.GetEditValue();
	m_pData->bmb9        = m_bmb9;
	m_pData->nmb9Type    = m_nmb9Type;
	m_pData->dmb9        = m_edtmb9.GetEditValue();	

	m_pData->bStrePrint4AASHTO[0] = m_b1stGrp_1;
	m_pData->bStrePrint4AASHTO[1] = m_b1stGrp_2;
	m_pData->bStrePrint4AASHTO[2] = m_b1stGrp_3;
	m_pData->bStrePrint4AASHTO[3] = m_b1stGrp_4;
	m_pData->bServPrint4AASHTO[0] = m_b2ndGrp_1;
	m_pData->bServPrint4AASHTO[1] = m_b2ndGrp_2;
	m_pData->bServPrint4AASHTO[2] = m_b2ndGrp_3;
	m_pData->bServPrint4AASHTO[3] = m_b2ndGrp_4;
	m_pData->bServPrint4AASHTO[4] = m_b2ndGrp_5;
	m_pData->bServPrint4AASHTO[5] = m_b2ndGrp_6;

	return TRUE;
}

void CDgnCRCDgnParameter_RUS::Data2Dlg()
{
	m_cmbBridgeType.SetCurSel(m_pData->iBrgType);

	m_bmb7mb8     = m_pData->bmb7mb8;
	m_nmb7mb8Type = m_pData->nmb7mb8Type;
	m_edtmb7.SetEditUnit(m_pData->dmb7);
	m_edtmb8.SetEditUnit(m_pData->dmb8);
	m_bmb9     = m_pData->bmb9;
	m_nmb9Type = m_pData->nmb9Type;
	m_edtmb9.SetEditUnit(m_pData->dmb9);

	m_b1stGrp_1 = m_pData->bStrePrint4AASHTO[0];
	m_b1stGrp_2 = m_pData->bStrePrint4AASHTO[1];
	m_b1stGrp_3 = m_pData->bStrePrint4AASHTO[2];
	m_b1stGrp_4 = m_pData->bStrePrint4AASHTO[3];
	m_b2ndGrp_1 = m_pData->bServPrint4AASHTO[0];
	m_b2ndGrp_2 = m_pData->bServPrint4AASHTO[1];
	m_b2ndGrp_3 = m_pData->bServPrint4AASHTO[2];
	m_b2ndGrp_4 = m_pData->bServPrint4AASHTO[3];
	m_b2ndGrp_5 = m_pData->bServPrint4AASHTO[4];
	m_b2ndGrp_6 = m_pData->bServPrint4AASHTO[5];

	UpdateData(FALSE);
}