// DgnMeshBarLThk4SandwichDlg.cpp : implementation file
//
#include "StdAfx.h"
#include "wg_dgn.h"
#include "DgnMeshBarLThk4SandwichDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarLThk4SandwichDlg dialog
CDgnMeshBarLThk4SandwichDlg::CDgnMeshBarLThk4SandwichDlg(CWnd* pParent /*=NULL*/)
: CInternationalDlg(CDgnMeshBarLThk4SandwichDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMeshBarLThk4SandwichDlg)
	
	//}}AFX_DATA_INIT  
}

void CDgnMeshBarLThk4SandwichDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMeshBarLThk4SandwichDlg)
	DDX_Control(pDX, IDC_DGN_AUTO_CHK, m_chkAuto);
	DDX_Control(pDX, IDC_DGN_TOP_EDT, m_edtTop);
	DDX_Control(pDX, IDC_DGN_BOT_EDT, m_edtBot);	
	DDX_Control(pDX, IDC_DGN_NUM_OF_ITR_EDT, m_edtItr);
	DDX_Control(pDX, IDC_DGN_TOL_EDT, m_edtTol);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnMeshBarLThk4SandwichDlg, CInternationalDlg)
//{{AFX_MSG_MAP(CDgnMeshBarLThk4SandwichDlg)
	ON_BN_CLICKED(IDC_DGN_AUTO_CHK, OnChkAuto)	
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnMeshBarLThk4SandwichDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog(); 
	
	SetControl();	

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnMeshBarLThk4SandwichDlg::Data2Dlg() 
{
	m_chkAuto.SetCheck(m_Data.bThickOptimal);	
	m_edtTop.SetEditUnit(m_Data.dTopThik);      
	m_edtBot.SetEditUnit(m_Data.dBotThik);    	
	m_edtItr.SetValue(m_Data.nIteration);
	m_edtTol.SetEditUnit(m_Data.dConvTolerance);

	EnableDisableControls();
	ShowHideControls();
	ChangeCaption();
}

void CDgnMeshBarLThk4SandwichDlg::SetControl() 
{		
	m_edtItr.SetRange(1, 200);
	m_edtItr.SetInteger(TRUE);
	m_edtTop.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtBot.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtTol.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

BOOL CDgnMeshBarLThk4SandwichDlg::Dlg2Data()
{	
	m_Data.bThickOptimal = m_chkAuto.GetCheck();
	m_Data.dTopThik = m_edtTop.GetEditValue();		
	m_Data.dBotThik = m_edtBot.GetEditValue();		
	m_Data.nIteration = (int)m_edtItr.GetEditValue();
	m_Data.dConvTolerance = m_edtTol.GetEditValue();		
	
	return TRUE;
}

void CDgnMeshBarLThk4SandwichDlg::OnOK() 
{
	if(!Dlg2Data())
	{
		return;
	}

	CInternationalDlg::OnOK();
}

void CDgnMeshBarLThk4SandwichDlg::SetData(LTHIK_4_SANDWICH& Data)
{
	m_Data = Data;
}

void CDgnMeshBarLThk4SandwichDlg::GetData(LTHIK_4_SANDWICH& Data)
{
	Data = m_Data;
}

void CDgnMeshBarLThk4SandwichDlg::EnableDisableControls()
{
	BOOL bAuto = m_chkAuto.GetCheck();
	
	GetDlgItem(IDC_DGN_NUM_OF_ITR_STC)->EnableWindow(bAuto);	
	GetDlgItem(IDC_DGN_NUM_OF_ITR_EDT)->EnableWindow(bAuto);
	
	GetDlgItem(IDC_DGN_TOL_STC)->EnableWindow(bAuto);	
	GetDlgItem(IDC_DGN_TOL_EDT)->EnableWindow(bAuto);
}

void CDgnMeshBarLThk4SandwichDlg::ChangeCaption()
{
	BOOL bAuto = m_chkAuto.GetCheck();
	if(bAuto)
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_AUTO));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_AUTO));      
	}
	else
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_MANUAL));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_MANUAL));      
	}
}

void CDgnMeshBarLThk4SandwichDlg::ShowHideControls()
{
	//BOOL bAuto = m_chkAuto.GetCheck();
	//
	//GetDlgItem(IDC_DGN_NUM_OF_ITR_STC)->ShowWindow(bAuto);	
	//GetDlgItem(IDC_DGN_NUM_OF_ITR_EDT)->ShowWindow(bAuto);
	//GetDlgItem(IDC_DGN_NUM_OF_ITR_SPN)->ShowWindow(bAuto);
	//GetDlgItem(IDC_DGN_TOL_STC)->ShowWindow(bAuto);	
	//GetDlgItem(IDC_DGN_TOL_EDT)->ShowWindow(bAuto);
}

void CDgnMeshBarLThk4SandwichDlg::OnChkAuto()
{
	EnableDisableControls();
	ShowHideControls();
	ChangeCaption();
}