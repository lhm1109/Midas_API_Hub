// DgnConCodeEC8InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallEnvelopMethod.h"

#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallEnvelopMethod dialog


CDgnConWallEnvelopMethod::CDgnConWallEnvelopMethod(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConWallEnvelopMethod::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWallEnvelopMethod)
	m_nMethod = 0;
	//}}AFX_DATA_INIT

}


void CDgnConWallEnvelopMethod::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallEnvelopMethod)
 	DDX_Control(pDX, IDB_DGN_WAL_ENVELOP_METHOD1, m_wndMethod1);
	DDX_Control(pDX, IDB_DGN_WAL_ENVELOP_METHOD2, m_wndMethod2);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConWallEnvelopMethod, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConWallEnvelopMethod)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallEnvelopMethod message handlers

BOOL CDgnConWallEnvelopMethod::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_aMethodRdo.Add(IDC_DGN_WALL_ENVELOP_METHOD1_RDO);
	m_aMethodRdo.Add(IDC_DGN_WALL_ENVELOP_METHOD2_RDO);
	
	m_wndMethod1.SetImage(_T("SVG\\illustration\\Dialog\\wall_design_bending_method1.svg"));
	m_wndMethod2.SetImage(_T("SVG\\illustration\\Dialog\\wall_design_bending_method2.svg"));


	Data2Dlg();
	return TRUE;
}


void CDgnConWallEnvelopMethod::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aMethodRdo, m_nMethod);
}

BOOL CDgnConWallEnvelopMethod::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aMethodRdo, m_nMethod);
	return TRUE;
}

void CDgnConWallEnvelopMethod::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnConWallEnvelopMethod::OnOK() 
{
	if(!Dlg2Data()) return ;
	CDialogMove::OnOK();
}
	