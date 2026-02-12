// NLLinkPrtHookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtHookDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtHookDlg dialog


CNLLinkPrtHookDlg::CNLLinkPrtHookDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtHookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtHookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNLLinkPrtHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtHookDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_OPEN_UNIT, m_wndOpenUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNIT, m_wndStifUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_OPEN_EDIT, m_wndOpen);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_EDIT, m_wndStif);
	DDX_Control(pDX, IDC_WG_CMD_STATIC6, m_wndPicture);
	DDX_Control(pDX, IDC_WG_CMD_STATIC5, m_wndPicture2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtHookDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtHookDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtHookDlg message handlers
BOOL CNLLinkPrtHookDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hook01.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp_hook02.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtHookDlg::ShowData(int nFlag)
{
	m_wndStif.SetEditUnit(m_Data.NDP_Prop[nFlag].HOOK.dStiffness);
	m_wndOpen.SetEditUnit(m_Data.NDP_Prop[nFlag].HOOK.dOpen     );
}

void CNLLinkPrtHookDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_wndStif.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
	}
	if(m_wndOpen.GetEditValue() < 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Open)); 	return;
	}
	
	m_Data.NDP_Prop[m_nDofType].HOOK.dStiffness        = m_wndStif.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].HOOK.dOpen             = m_wndOpen.GetEditValue();

	CDialogMove::OnOK();
}
void CNLLinkPrtHookDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

