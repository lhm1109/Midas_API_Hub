// CMElsPlsMason4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMason4Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason4Dlg dialog


CCMElsPlsMason4Dlg::CCMElsPlsMason4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMason4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsMason4Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMElsPlsMason4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMason4Dlg)
	DDX_Control(pDX, IDC_CMD_MASON1_EDT,  m_edtMason1);
	DDX_Control(pDX, IDC_CMD_MASON2_EDT,  m_edtMason2);
	DDX_Control(pDX, IDC_CMD_MASON3_EDT,  m_edtMason3);
	DDX_Control(pDX, IDC_CMD_MASON4_EDT,  m_edtMason4);

	DDX_Control(pDX, IDC_CMD_MASON1_UNT,  m_untMason1);
	DDX_Control(pDX, IDC_CMD_MASON2_UNT,  m_untMason2);
	DDX_Control(pDX, IDC_CMD_MASON3_UNT,  m_untMason3);
	DDX_Control(pDX, IDC_CMD_MASON4_UNT, m_untMason4);
	DDX_Control(pDX, IDC_WG_CMD_STATIC1, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMElsPlsMason4Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsMason4Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMElsPlsMason4Dlg::Data2Dlg()
{
	m_edtMason1.SetEditUnit(m_pData->dGP_BL);
	m_edtMason2.SetEditUnit(m_pData->dGP_BH);
	m_edtMason3.SetEditUnit(m_pData->dGP_Tb);
	m_edtMason4.SetEditUnit(m_pData->dGP_Th);
}

BOOL CCMElsPlsMason4Dlg::Dlg2Data()
{
	m_pData->dGP_BL = m_edtMason1.GetEditValue();
	m_pData->dGP_BH = m_edtMason2.GetEditValue();
	m_pData->dGP_Tb = m_edtMason3.GetEditValue();
	m_pData->dGP_Th = m_edtMason4.GetEditValue();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason4Dlg message handlers

BOOL CCMElsPlsMason4Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_edtMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBL); 
	m_edtMason2.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBH);
	m_edtMason3.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dTb);
	m_edtMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dTh);

	m_untMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBL); 
	m_untMason2.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBH);
	m_untMason3.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dTb);
	m_untMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dTh);

	Data2Dlg();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_geo.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsMason4Dlg::OnOK() 
{
	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}
