// IehpPropROHDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpPropROHDDlg.h"

#include "..\wg_base\wg_base_SpecialChar.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpPropROHDDlg dialog


CIehpPropROHDDlg::CIehpPropROHDDlg(T_IEHP_ROHD *pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpPropROHDDlg::IDD, pParent)
{
	m_pData = NULL;
	m_pData = pData;
	ASSERT(m_pData);
	//{{AFX_DATA_INIT(CIehpPropROHDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nType = -1;
	m_nDof  = -1;
}

bool CIehpPropROHDDlg::SyncDialogData()
{
	m_pData->dCriticalDispl = m_edtCriticalDispl.GetEditValue();
	m_pData->dMaxDamping = m_edtMaxDamp.GetEditValue();

	return true;
}


void CIehpPropROHDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpPropROHDDlg)
	DDX_Control(pDX, IDC_CMD_DISP_EDT, m_edtCriticalDispl);
	DDX_Control(pDX, IDC_CMD_DISP_UNT, m_untCriticalDispl);
	DDX_Control(pDX, IDC_CMD_DAMP_EDT, m_edtMaxDamp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIehpPropROHDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpPropROHDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpPropROHDDlg message handlers

BOOL CIehpPropROHDDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(m_nType==D_IEHP_RO) // RO
	{
		this->SetWindowText(_LS(IDS_CMD_IEHP_PROP_RO));
	}
	else if(m_nType==D_IEHP_HD) // HD
	{
		this->SetWindowText(_LS(IDS_CMD_IEHP_PROP_HD));
		GetDlgItem(IDC_CMD_DAMP_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_DAMP_EDT)->ShowWindow(FALSE);
	}
	else ASSERT(0);

	CString strMsg = _T("");
	if(m_nDof==0) // Dx, Dy, Dz
	{
		strMsg = _LS(IDS_CMD_CRITICAL_DISPLACEMENT);
		m_edtCriticalDispl.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_untCriticalDispl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	else if(m_nDof==1) // Rx, Ry, Rz
	{
		strMsg = _LS(IDS_CMD_CRITICAL_ROTATION);
		m_edtCriticalDispl.SetUnitType(D_UNITSYS_BASE_RADIAN);
		m_untCriticalDispl.SetUnitType(D_UNITSYS_BASE_RADIAN);
	}
	else ASSERT(0);

	strMsg += _T("(") + D_CH_DELTA + _T("r)");
	GetDlgItem(IDC_CMD_DISP_STC)->SetWindowText(strMsg);

	m_edtMaxDamp.SetUnitType(0);

	if(fabs(m_pData->dCriticalDispl)>0.0) 
		m_edtCriticalDispl.SetEditUnit(m_pData->dCriticalDispl);
	else
		m_edtCriticalDispl.SetWindowText(_T(""));

	if(fabs(m_pData->dMaxDamping)>0.0)
		m_edtMaxDamp.SetEditUnit(m_pData->dMaxDamping);
	else
		m_edtMaxDamp.SetWindowText(_T(""));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpPropROHDDlg::OnOK() 
{
	m_pData->dCriticalDispl = m_edtCriticalDispl.GetEditValue();
	m_pData->dMaxDamping    = m_edtMaxDamp.GetEditValue();
	
	CDialogMove::OnOK();
}
