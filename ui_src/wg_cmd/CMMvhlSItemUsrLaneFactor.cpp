//////////
// CMMvhlSItemUsrLaneFactor.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlSItemUsrLaneFactor.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlSItemUsrLaneFactordialog
CCMMvhlSItemUsrLaneFactor::CCMMvhlSItemUsrLaneFactor(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlSItemUsrLaneFactor::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CCMMvhlSItemUsrLaneFactor::~CCMMvhlSItemUsrLaneFactor()
{

}

void CCMMvhlSItemUsrLaneFactor::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlSItemUsrLaneFactor)	
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT1_EDT, m_edtLaneFact1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT2_EDT, m_edtLaneFact2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT3_EDT, m_edtLaneFact3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT4_EDT, m_edtLaneFact4);
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlSItemUsrLaneFactor, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlSItemUsrLaneFactor)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMMvhlSItemUsrLaneFactor::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetControl();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCMMvhlSItemUsrLaneFactor::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtLaneFact1.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact2.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact3.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact4.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvhlSItemUsrLaneFactor::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CCMMvhlSItemUsrLaneFactor::Dlg2Data() 
{
	m_Data.dLaneFactor[0] = m_edtLaneFact1.GetEditValue();
	m_Data.dLaneFactor[1] = m_edtLaneFact2.GetEditValue();
	m_Data.dLaneFactor[2] = m_edtLaneFact3.GetEditValue();
	m_Data.dLaneFactor[3] = m_edtLaneFact4.GetEditValue();

	return TRUE;
}

void CCMMvhlSItemUsrLaneFactor::Data2Dlg() 
{
	m_edtLaneFact1.SetEditUnit(m_Data.dLaneFactor[0]);
	m_edtLaneFact2.SetEditUnit(m_Data.dLaneFactor[1]);
	m_edtLaneFact3.SetEditUnit(m_Data.dLaneFactor[2]);
	m_edtLaneFact4.SetEditUnit(m_Data.dLaneFactor[3]);
}

void CCMMvhlSItemUsrLaneFactor::SetData( T_MVHL_D& Data )
{
	m_Data.Initialize(); 
	// Equal 연산 정의하는건 좀 아닌 것 같아서 쓰는 값만 세팅 함
	m_Data.dLaneFactor[0] = Data.dLaneFactor[0];
	m_Data.dLaneFactor[1] = Data.dLaneFactor[1];
	m_Data.dLaneFactor[2] = Data.dLaneFactor[2];
	m_Data.dLaneFactor[3] = Data.dLaneFactor[3];
}

T_MVHL_D& CCMMvhlSItemUsrLaneFactor::GetData()
{
	return m_Data;
}

