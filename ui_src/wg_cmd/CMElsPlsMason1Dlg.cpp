// CMElsPlsMason1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMason1Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason1Dlg dialog


CCMElsPlsMason1Dlg::CCMElsPlsMason1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMason1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsMason1Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMElsPlsMason1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMason1Dlg)
	DDX_Control(pDX, IDC_CMD_MASON1_EDT,  m_edtMason1);
	DDX_Control(pDX, IDC_CMD_MASON2_EDT,  m_edtMason2);
	// DDX_Control(pDX, IDC_CMD_MASON3_EDT,  m_edtMason3);
	DDX_Control(pDX, IDC_CMD_MASON4_EDT,  m_edtMason4);
	DDX_Control(pDX, IDC_CMD_MASON5_EDT,  m_edtMason5);
	// DDX_Control(pDX, IDC_CMD_MASON6_EDT,  m_edtMason6);
	// DDX_Control(pDX, IDC_CMD_MASON7_EDT,  m_edtMason7);
	// DDX_Control(pDX, IDC_CMD_MASON8_EDT,  m_edtMason8);
	// DDX_Control(pDX, IDC_CMD_MASON9_EDT,  m_edtMason9);
	DDX_Control(pDX, IDC_CMD_MASON10_EDT, m_edtMason10);
	DDX_Control(pDX, IDC_CMD_MASON11_EDT, m_edtMason11);
	// DDX_Control(pDX, IDC_CMD_MASON12_EDT, m_edtMason12);
	// DDX_Control(pDX, IDC_CMD_MASON13_EDT, m_edtMason13);

	DDX_Control(pDX, IDC_CMD_MASON1_UNT,  m_untMason1);
	DDX_Control(pDX, IDC_CMD_MASON4_UNT,  m_untMason4);
	DDX_Control(pDX, IDC_CMD_MASON5_UNT,  m_untMason5);
	DDX_Control(pDX, IDC_CMD_MASON11_UNT, m_untMason11);
	DDX_Control(pDX, IDC_WG_CMD_STATIC1, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMElsPlsMason1Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsMason1Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMElsPlsMason1Dlg::Data2Dlg()
{
	m_edtMason1.SetEditUnit(m_pData->dBR_Es);
	m_edtMason2.SetEditUnit(m_pData->dBR_Pr); 
	// m_edtMason3.SetEditUnit(m_pData->dBR_Val1);
	m_edtMason4.SetEditUnit(m_pData->dBR_Fc);
	m_edtMason5.SetEditUnit(m_pData->dBR_Ft);
	// m_edtMason6.SetEditUnit(m_pData->dBR_Val2);
	// m_edtMason7.SetEditUnit(m_pData->dBR_Val3);
	// m_edtMason8.SetEditUnit(m_pData->dBR_Val4);
	// m_edtMason9.SetEditUnit(m_pData->dBR_Val5);
	m_edtMason10.SetEditUnit(m_pData->dBR_H);
	m_edtMason11.SetEditUnit(m_pData->dBR_FA);
	// m_edtMason12.SetEditUnit(m_pData->dBR_Val6);
	// m_edtMason13.SetEditUnit(m_pData->dBR_Val7);
}

BOOL CCMElsPlsMason1Dlg::Dlg2Data()
{
	m_pData->dBR_Es   = m_edtMason1.GetEditValue();
	m_pData->dBR_Pr   = m_edtMason2.GetEditValue();
	// m_pData->dBR_Val1 = m_edtMason3.GetEditValue();
	m_pData->dBR_Fc   = m_edtMason4.GetEditValue();
	m_pData->dBR_Ft   = m_edtMason5.GetEditValue();
	// m_pData->dBR_Val2 = m_edtMason6.GetEditValue();
	// m_pData->dBR_Val3 = m_edtMason7.GetEditValue();
	// m_pData->dBR_Val4 = m_edtMason8.GetEditValue();
	// m_pData->dBR_Val5 = m_edtMason9.GetEditValue();
	m_pData->dBR_H    = m_edtMason10.GetEditValue();
	m_pData->dBR_FA   = m_edtMason11.GetEditValue();
	// m_pData->dBR_Val6 = m_edtMason12.GetEditValue();
	// m_pData->dBR_Val7 = m_edtMason13.GetEditValue();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason1Dlg message handlers

BOOL CCMElsPlsMason1Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_edtMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dEs); 
	m_edtMason2.SetUnitType(0);
	// m_edtMason3.SetUnitType(0);
	m_edtMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFc);
	m_edtMason4.SetReadOnly(TRUE);
	m_edtMason5.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFt);
	// m_edtMason6.SetUnitType(0);
	// m_edtMason7.SetUnitType(0);
	// m_edtMason8.SetUnitType(0);
	// m_edtMason9.SetUnitType(0);
	m_edtMason10.SetUnitType(0);
	m_edtMason11.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFA);
	m_edtMason11.SetReadOnly(TRUE);
	// m_edtMason12.SetUnitType(0);
	// m_edtMason13.SetUnitType(0);
	m_untMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dEs); 
	m_untMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFc);
	m_untMason5.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFt);
	m_untMason11.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFA);

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_brick.svg"));

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsMason1Dlg::OnOK() 
{
	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}
