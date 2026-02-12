// WindBSFactor.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindBSFactor.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindBSFactor dialog


CWindBSFactor::CWindBSFactor(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindBSFactor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindBSFactor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWindBSFactor::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindBSFactor)
	DDX_Control(pDX, IDC_CMD_BS_SWSP_SS_EDIT, m_wndBS6399Ss);
	DDX_Control(pDX, IDC_CMD_BS_SWSP_SP_EDIT, m_wndBS6399Sp);
	DDX_Control(pDX, IDC_CMD_BS_SWSP_SD_EDIT, m_wndBS6399Sd);
	DDX_Control(pDX, IDC_CMD_BS_SWSP_SA_EDIT, m_wndBS6399Sa);
	DDX_Control(pDX, IDC_CMD_BS_SWSP_SH_EDIT, m_wndBS6399Sh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindBSFactor, CDialogMove)
	//{{AFX_MSG_MAP(CWindBSFactor)
	ON_BN_CLICKED(IDC_CMD_BS_WSP_OK, OnCmdBsWspOk)
	ON_BN_CLICKED(IDC_CMD_BS_WSP_CANCEL, OnCmdBsWspCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindBSFactor message handlers
BOOL CWindBSFactor::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	if(nProcedure == 0) 	m_wndBS6399Sh.EnableWindow(FALSE);	
	else 									m_wndBS6399Sh.EnableWindow(TRUE);	
	
	// 부모 dialog로 부터 받은 m_Data의 값을 현재 dialog에 셋팅
	// m_Data : m_Data of parent dialog
	m_wndBS6399Sa.SetEditUnit(m_Data.CodeParam.BS6399.dAltitudeFactor		);  
	m_wndBS6399Sd.SetEditUnit(m_Data.CodeParam.BS6399.dDirectionalFactor);
	m_wndBS6399Ss.SetEditUnit(m_Data.CodeParam.BS6399.dSeasonalFactor		);
	m_wndBS6399Sp.SetEditUnit(m_Data.CodeParam.BS6399.dProbabilityFactor);
	m_wndBS6399Sh.SetEditUnit(m_Data.CodeParam.BS6399.dTopographicFactor);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWindBSFactor::OnCmdBsWspOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// 현재 dialog의 수정된 값을 현재 dialog의 local 데이터 객체인 
	// m_Data.BS6399에 저장
	m_Data.CodeParam.BS6399.dAltitudeFactor = m_wndBS6399Sa.GetEditValue();
	m_Data.CodeParam.BS6399.dDirectionalFactor = m_wndBS6399Sd.GetEditValue();
	m_Data.CodeParam.BS6399.dSeasonalFactor = m_wndBS6399Ss.GetEditValue();
	m_Data.CodeParam.BS6399.dProbabilityFactor = m_wndBS6399Sp.GetEditValue();
	m_Data.CodeParam.BS6399.dTopographicFactor = m_wndBS6399Sh.GetEditValue();

	CDialogMove::OnOK();	
}

void CWindBSFactor::OnCmdBsWspCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();		
}

