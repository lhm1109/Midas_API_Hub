// CMWindItemJapan2004Param.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan2004Param.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004Param dialog


CCMWindItemJapan2004Param::CCMWindItemJapan2004Param(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMWindItemJapan2004Param::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemJapan2004Param)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	T_WIND_JP2004 data;
	data.Initialize();
	m_dBreadthX     = data.dBreadthX;
	m_dBreadthY     = data.dBreadthY;
	m_dDampRatioX   = data.dDampingX;
	m_dDampRatioY   = data.dDampingY;
	m_dDampRatioR   = data.dDampingR;
	m_dFrequencyX   = data.dFrequencyX;
	m_dFrequencyY   = data.dFrequencyY;
	m_dFrequencyR   = data.dFrequencyR;
	m_dModeFactorX  = data.dModeFactorX;
	m_dModeFactorY  = data.dModeFactorY;
	m_dModeFactorR  = data.dModeFactorR;
}


void CCMWindItemJapan2004Param::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan2004Param)
	DDX_Control(pDX, IDC_CMD_VIBMODE_Z_EDIT, m_editModeZ);
	DDX_Control(pDX, IDC_CMD_VIBMODE_Y_EDIT, m_editModeY);
	DDX_Control(pDX, IDC_CMD_VIBMODE_X_EDIT, m_editModeX);
	DDX_Control(pDX, IDC_CMD_FREQENCY_Z_UNIT, m_unitFreqZ);
	DDX_Control(pDX, IDC_CMD_FREQENCY_Z_EDIT, m_editFreqZ);
	DDX_Control(pDX, IDC_CMD_FREQENCY_Y_UNIT, m_unitFreqY);
	DDX_Control(pDX, IDC_CMD_FREQENCY_Y_EDIT, m_editFreqY);
	DDX_Control(pDX, IDC_CMD_FREQENCY_X_UNIT, m_unitFreqX);
	DDX_Control(pDX, IDC_CMD_FREQENCY_X_EDIT, m_editFreqX);
	DDX_Control(pDX, IDC_CMD_DAMPRATIO_Z_EDIT, m_editDampZ);
	DDX_Control(pDX, IDC_CMD_DAMPRATIO_Y_EDIT, m_editDampY);
	DDX_Control(pDX, IDC_CMD_DAMPRATIO_X_EDIT, m_editDampX);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_UNIT, m_unitBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_Y_EDIT, m_editBreadthY);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_UNIT, m_unitBreadthX);
	DDX_Control(pDX, IDC_CMD_BREADTH_X_EDIT, m_editBreadthX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan2004Param, CDialogMove)
	//{{AFX_MSG_MAP(CCMWindItemJapan2004Param)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004Param message handlers

BOOL CCMWindItemJapan2004Param::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemJapan2004Param::OnOK() 
{
	// TODO: Add extra validation here

	if (!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMWindItemJapan2004Param::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}


//---------------------------------------------------------------------------
void CCMWindItemJapan2004Param::InitControls()
{
	m_editModeZ.SetUnitType(D_UNITSYS_NONE);
	m_editModeY.SetUnitType(D_UNITSYS_NONE);
	m_editModeX.SetUnitType(D_UNITSYS_NONE);
	m_editFreqZ.SetUnitType(D_UNITSYS_NONE);
	m_editFreqY.SetUnitType(D_UNITSYS_NONE);
	m_editFreqX.SetUnitType(D_UNITSYS_NONE);
	m_editDampZ.SetUnitType(D_UNITSYS_NONE);
	m_editDampY.SetUnitType(D_UNITSYS_NONE);
	m_editDampX.SetUnitType(D_UNITSYS_NONE);
	m_unitBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBreadthX.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMWindItemJapan2004Param::Data2Dlg()
{
	m_editBreadthX.SetEditUnit(m_dBreadthX);
	m_editBreadthY.SetEditUnit(m_dBreadthY);
	m_editDampX.SetEditUnit(m_dDampRatioX);
	m_editDampY.SetEditUnit(m_dDampRatioY);
	m_editDampZ.SetEditUnit(m_dDampRatioR);
	m_editFreqX.SetEditUnit(m_dFrequencyX);
	m_editFreqY.SetEditUnit(m_dFrequencyY);
	m_editFreqZ.SetEditUnit(m_dFrequencyR);
	m_editModeX.SetEditUnit(m_dModeFactorX);
	m_editModeY.SetEditUnit(m_dModeFactorY);
	m_editModeZ.SetEditUnit(m_dModeFactorR);
}

BOOL CCMWindItemJapan2004Param::Dlg2Data()
{
	m_dBreadthX    = m_editBreadthX.GetEditValue();
	m_dBreadthY    = m_editBreadthY.GetEditValue();
	m_dDampRatioX  = m_editDampX.GetEditValue();
	m_dDampRatioY  = m_editDampY.GetEditValue();
	m_dDampRatioR  = m_editDampZ.GetEditValue();
	m_dFrequencyX  = m_editFreqX.GetEditValue();
	m_dFrequencyY  = m_editFreqY.GetEditValue();
	m_dFrequencyR  = m_editFreqZ.GetEditValue();
	m_dModeFactorX = m_editModeX.GetEditValue();
	m_dModeFactorY = m_editModeY.GetEditValue();
	m_dModeFactorR = m_editModeZ.GetEditValue();
/*
	if(m_dBreadthX   <= 0.) {AfxMessageBox(_T("Error! Bx")); return FALSE;}
	if(m_dBreadthY   <= 0.) {AfxMessageBox(_T("Error! By")); return FALSE;}
	if(m_dDampRatioX <= 0.) {AfxMessageBox(_T("Error! Zetax")); return FALSE;}
	if(m_dDampRatioY <= 0.) {AfxMessageBox(_T("Error! Zetay")); return FALSE;}
	if(m_dDampRatioR <= 0.) {AfxMessageBox(_T("Error! Zetat")); return FALSE;}
	if(m_dFrequencyX <= 0.) {AfxMessageBox(_T("Error! fx")); return FALSE;}
	if(m_dFrequencyY <= 0.) {AfxMessageBox(_T("Error! fy")); return FALSE;}
	if(m_dFrequencyR <= 0.) {AfxMessageBox(_T("Error! ft")); return FALSE;}
*/  
	return TRUE;
} 
