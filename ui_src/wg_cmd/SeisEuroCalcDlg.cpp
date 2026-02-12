// SeisEuroCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisEuroCalcDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_SpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisEuroCalcDlg dialog

CSeisEuroCalcDlg::CSeisEuroCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisEuroCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisEuroCalcDlg)
	m_nXMethod = 0;
	m_nYMethod = 0;
	m_dXAcedit = 0.0;
	m_dYAcedit = 0.0;
	m_dXDedit = 0.0;
	m_dYDedit = 0.0;
	m_dXHedit = 0.0;
	m_dYHedit = 0.0;
	//}}AFX_DATA_INIT
	m_bNTCFlag = FALSE;
	m_bP100Flag = FALSE;
}


void CSeisEuroCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisEuroCalcDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_YH, m_wndYHedit);
	DDX_Control(pDX, IDC_ETC_SEIS_XH, m_wndXHedit);
	DDX_Control(pDX, IDC_ETC_SEIS_YD, m_wndYDedit);
	DDX_Control(pDX, IDC_ETC_SEIS_XD, m_wndXDedit);
	DDX_Control(pDX, IDC_CMD_SEIS_YAc, m_wndYAcedit);
	DDX_Control(pDX, IDC_CMD_SEIS_XAc, m_wndXAcedit);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_nXMethod);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_nYMethod);
	DDX_Text(pDX, IDC_CMD_SEIS_XAc, m_dXAcedit);
	DDX_Text(pDX, IDC_CMD_SEIS_YAc, m_dYAcedit);
	DDX_Text(pDX, IDC_ETC_SEIS_XD, m_dXDedit);
	DDX_Text(pDX, IDC_ETC_SEIS_YD, m_dYDedit);
	DDX_Text(pDX, IDC_ETC_SEIS_XH, m_dXHedit);
	DDX_Text(pDX, IDC_ETC_SEIS_YH, m_dYHedit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisEuroCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisEuroCalcDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD4, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD5, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD4, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD5, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnRdoXmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnRdoYmethodChange)
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnRdoYmethodChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisEuroCalcDlg message handlers

BOOL CSeisEuroCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndXHedit.EnableWindow(m_nXMethod==0 || m_nXMethod==1 || m_nXMethod==2 || m_nXMethod==3);
	m_wndYHedit.EnableWindow(m_nYMethod==0 || m_nYMethod==1 || m_nYMethod==2 || m_nYMethod==3);
	m_wndXAcedit.EnableWindow(m_nXMethod == 3);
	m_wndYAcedit.EnableWindow(m_nYMethod == 3);
	m_wndXDedit.EnableWindow(m_nXMethod == 4);
	m_wndYDedit.EnableWindow(m_nYMethod == 4);

	m_wndXHedit.SetEditUnit(m_dXHedit);
	m_wndYHedit.SetEditUnit(m_dYHedit);		
	m_wndXAcedit.SetEditUnit(m_dXAcedit);		
	m_wndYAcedit.SetEditUnit(m_dYAcedit);		
	m_wndXDedit.SetEditUnit(m_dXDedit);		
	m_wndYDedit.SetEditUnit(m_dYDedit);		
				
	if(m_bNTCFlag)  CustomizeNTC();
	if(m_bP100Flag) CustomizeP100();

	CString strUnit = _T("");
	strUnit.Format(_T("(m%s)"), D_CH_2SQUARE);
	GetDlgItem(IDC_CMD_ACX_UNIT)->SetWindowText(strUnit);
	GetDlgItem(IDC_CMD_ACX_UNIT2)->SetWindowText(strUnit);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisEuroCalcDlg::OnRdoXmethodChange() 
{
	UpdateData(TRUE);
	m_wndXHedit.EnableWindow(m_nXMethod==0 || m_nXMethod==1 || m_nXMethod==2 || m_nXMethod==3);
	m_wndXAcedit.EnableWindow(m_nXMethod==3);	
	m_wndXDedit.EnableWindow(m_nXMethod==4);	
}

void CSeisEuroCalcDlg::OnRdoYmethodChange() 
{
	UpdateData(TRUE);
	m_wndYHedit.EnableWindow(m_nYMethod==0 || m_nYMethod==1 || m_nYMethod==2 || m_nYMethod==3);
	m_wndYAcedit.EnableWindow(m_nYMethod==3);	
	m_wndYDedit.EnableWindow(m_nYMethod==4);	
}

BOOL CSeisEuroCalcDlg::CalcPeriod(double dH,double dAc,double dD,int nMethod,double& dPeriod)
{
	switch(nMethod)
	{
		case 0: dPeriod = 0.085 * pow(dH,(3.0/4.0));	break;
		case 1:	dPeriod = 0.075 * pow(dH,(3.0/4.0));	break;
		case 2:	dPeriod = 0.050 * pow(dH,(3.0/4.0));	break;
		case 3:	
			if(dAc <= 0)
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Ac_is_invalid_));
				return FALSE;
			}
			dPeriod = 0.075 * pow(dH,(3.0/4.0)) / sqrt(dAc);
			break;
		case 4:	
			if(m_bP100Flag)
			{
				if(dD <= 0)
				{
					CString strErr;
					strErr.Format(_T("%s"), _LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__d), 0);
					AfxMessageBox(strErr);
					return FALSE;
				}			
				dPeriod = dD;
			}
			else
			{
			  if(dD <= 0)
			  {
				  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_d_is_invalid_));
				  return FALSE;
			  }			
			  dPeriod = 2 * sqrt(dD);
			}
			break;
	}
	return TRUE;
}

void CSeisEuroCalcDlg::OnOK() 
{
	UpdateData(TRUE);

	if(!CalcPeriod(m_dXHedit,m_dXAcedit,m_dXDedit,m_nXMethod,m_dPeriod_x)) return;
	if(!CalcPeriod(m_dYHedit,m_dYAcedit,m_dYDedit,m_nYMethod,m_dPeriod_y)) return;

	m_dXHedit = m_wndXHedit.GetEditValue();
	m_dYHedit = m_wndYHedit.GetEditValue();
	m_dXAcedit= m_wndXAcedit.GetEditValue();
	m_dYAcedit= m_wndYAcedit.GetEditValue();
	m_dXDedit = m_wndXDedit.GetEditValue();
	m_dYDedit = m_wndYDedit.GetEditValue();
	CDialogMove::OnOK();
}
	
void CSeisEuroCalcDlg::CustomizeNTC()
{
	GetDlgItem(IDC_RDO_XMETHOD5)->EnableWindow(FALSE); 
	GetDlgItem(IDC_RDO_XMETHOD5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_YMETHOD5)->EnableWindow(FALSE); 
	GetDlgItem(IDC_RDO_YMETHOD5)->ShowWindow(SW_HIDE);
}

void CSeisEuroCalcDlg::CustomizeP100()
{
	GetDlgItem(IDC_RDO_XMETHOD5 )->SetWindowText(_LSX(5. T = 1.0n)); 
	GetDlgItem(IDC_RDO_YMETHOD5 )->SetWindowText(_LSX(5. T = 1.0n)); 
	GetDlgItem(IDC_CMD_DX_TITLE )->SetWindowText(_LSX(n : )); 
	GetDlgItem(IDC_CMD_DX_TITLE2)->SetWindowText(_LSX(n : )); 
	CString strStory;
	strStory.Format(_T("(%s)"),_LS(IDS_WG_CMD__ADDD__Story));
	GetDlgItem(IDC_CMD_BX_UNIT  )->SetWindowText(strStory); 
	GetDlgItem(IDC_CMD_BX_UNIT2 )->SetWindowText(strStory); 
	this->SetWindowText(_LS(IDS_SEIS_CALCPERIOD_P100));
}