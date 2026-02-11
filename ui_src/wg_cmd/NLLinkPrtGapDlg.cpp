// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtGapDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtGapDlg dialog


CNLLinkPrtGapDlg::CNLLinkPrtGapDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtGapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtGapDlg)
	//}}AFX_DATA_INIT
}
CNLLinkPrtGapDlg::~CNLLinkPrtGapDlg()
{
}

void CNLLinkPrtGapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtGapDlg)
	DDX_Control(pDX, IDC_OPEN_STATIC, m_wndOpenTitle);
	DDX_Control(pDX, IDC_CMD_NLLK_OPEN_UNIT, m_wndOpenUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNIT, m_wndStifUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_OPEN_EDIT, m_wndOpen);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_EDIT, m_wndStif);
	DDX_Control(pDX, IDC_CMD_GAP_BMP01, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_GAP_BMP02, m_wndPicture2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtGapDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtGapDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtGapDlg message handlers
BOOL CNLLinkPrtGapDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_gap01.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_gap02.svg"));

	// Lead Rubber Bearing Isolator(Dx,Rx,Ry,Rz) 
	// Friction Pendulum(Rx,Ry,Rz)
	// Triple Friction Pendulum(Rx,Ry,Rz) 에서 쓰일 경우
	if(m_nCodeType == 4 || 
		((m_nCodeType == 5 || m_nCodeType == 6 || m_nCodeType == 7) && (m_nDofType == 3 || m_nDofType == 4 || m_nDofType == 5)))   
	{
		m_wndOpen.ShowWindow(SW_HIDE);
		m_wndOpenUnit.ShowWindow(SW_HIDE);
		m_wndOpenTitle.ShowWindow(SW_HIDE);
		m_wndOpen.SetWindowText(_T("0"));
		if(m_nCodeType == 4)  
			SetWindowText(_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop8));
		if((m_nCodeType == 5 || m_nCodeType == 6 || m_nCodeType == 7) && (m_nDofType == 3 || m_nDofType == 4 || m_nDofType == 5))
			SetWindowText(_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop9));

		m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_gaplr01.svg"));
		m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_gaplr02.svg"));
	}

	if((m_nCodeType == 5 || m_nCodeType == 6 || m_nCodeType == 7) && m_nDofType == 0)   // Friction Pendulum(Dx), Triple Friction Pendulum(Dx)에서 쓰일 경우
	{
		m_wndOpen.EnableWindow(FALSE);
		m_wndOpen.SetWindowText(_T("0"));
		SetWindowText(_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop7));
	}

	switch(m_nDofType)
	{
		case 0: ShowData(0); break;
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
		case 3: ShowData(3); break;
		case 4: ShowData(4); break;
		case 5: ShowData(5); break;
	}
	switch(m_nDofType)
	{
		case 0: case 1: case 2:
			m_wndStifUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
			m_wndOpenUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
			break;
		case 3: case 4: case 5:
			m_wndStifUnit.SetUnitType(D_UNITSYS_BASE_ROSPRING);
			m_wndOpenUnit.SetUnitType(D_UNITSYS_BASE_RADIAN);
			break;
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtGapDlg::ShowData(int nFlag)
{
	m_wndStif.SetEditUnit(m_Data.NDP_Prop[nFlag].GAP.dStiffness);
	m_wndOpen.SetEditUnit(m_Data.NDP_Prop[nFlag].GAP.dOpen     );
}

void CNLLinkPrtGapDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_wndStif.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
	}
	if(m_nCodeType == 1)
	{
		if(m_wndOpen.GetEditValue() < 0)
		{
	    AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Open)); 	return;
		}
	}
	
	m_Data.NDP_Prop[m_nDofType].GAP.dStiffness        = m_wndStif.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].GAP.dOpen             = m_wndOpen.GetEditValue();

	CDialogMove::OnOK();
}

void CNLLinkPrtGapDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

