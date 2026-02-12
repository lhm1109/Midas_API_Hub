// CMElsPlsMason2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMason2Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason2Dlg dialog


CCMElsPlsMason2Dlg::CCMElsPlsMason2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMason2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsMason2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMElsPlsMason2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMason2Dlg)
	DDX_Control(pDX, IDC_CMD_MASON1_EDT,  m_edtMason1);
	DDX_Control(pDX, IDC_CMD_MASON2_EDT,  m_edtMason2);
	DDX_Control(pDX, IDC_CMD_MASON3_EDT,  m_edtMason3);
	DDX_Control(pDX, IDC_CMD_MASON4_EDT,  m_edtMason4);
	DDX_Control(pDX, IDC_CMD_MASON5_EDT,  m_edtMason5);
	DDX_Control(pDX, IDC_CMD_MASON6_EDT,  m_edtMason6);

	DDX_Control(pDX, IDC_CMD_MASON1_UNT,  m_untMason1);
	DDX_Control(pDX, IDC_CMD_MASON3_UNT,  m_untMason3);
	DDX_Control(pDX, IDC_CMD_MASON4_UNT,  m_untMason4);
	DDX_Control(pDX, IDC_CMD_MASON6_UNT, m_untMason6);
	DDX_Control(pDX, IDC_WG_CMD_STATIC1, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMElsPlsMason2Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsMason2Dlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMElsPlsMason2Dlg::Data2Dlg()
{
	m_edtMason1.SetEditUnit(m_pData->dBJ_Es); 
	m_edtMason2.SetEditUnit(m_pData->dBJ_Pr); 
	m_edtMason3.SetEditUnit(m_pData->dBJ_Fc); 
	m_edtMason4.SetEditUnit(m_pData->dBJ_Ft); 
	m_edtMason5.SetEditUnit(m_pData->dBJ_H);  
	m_edtMason6.SetEditUnit(m_pData->dBJ_BWS);
}

BOOL CCMElsPlsMason2Dlg::Dlg2Data()
{
	m_pData->dBJ_Es   = m_edtMason1.GetEditValue();
	m_pData->dBJ_Pr   = m_edtMason2.GetEditValue();
	m_pData->dBJ_Fc   = m_edtMason3.GetEditValue();
	m_pData->dBJ_Ft   = m_edtMason4.GetEditValue();
	m_pData->dBJ_H    = m_edtMason5.GetEditValue();
	m_pData->dBJ_BWS  = m_edtMason6.GetEditValue();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMason2Dlg message handlers

BOOL CCMElsPlsMason2Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_edtMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dEs); 
	m_edtMason2.SetUnitType(0);
	m_edtMason3.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFc);
	m_edtMason3.SetReadOnly(TRUE);
	m_edtMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFt);
	m_edtMason5.SetUnitType(0);
	m_edtMason6.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBWS);
	m_edtMason6.SetReadOnly(TRUE);

	m_untMason1.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dEs); 
	m_untMason3.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFc);
	m_untMason4.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dFt);
	m_untMason6.SetUnitType(CUnitCtrl::m_EPMT_UNIT.dBWS);

	Data2Dlg();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_bed.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsMason2Dlg::OnOK() 
{
	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}
