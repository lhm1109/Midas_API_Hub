// CMLoadPNLDAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadPNLDAreaDlg.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDAreaDlg dialog


CCMLoadPNLDAreaDlg::CCMLoadPNLDAreaDlg(T_PNLD_D *pData,CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMLoadPNLDAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadPNLDAreaDlg)
	m_nPt34 = -1;
	//}}AFX_DATA_INIT

	ASSERT(pData);
	m_pData = NULL;
	m_pData = pData;

	for(int i=0; i<4; i++)
	{
		m_sXYEd[i]=_T("");
		//m_sXYLdEd[i]=_T("");	
	}
}


void CCMLoadPNLDAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadPNLDAreaDlg)
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X4Y4LOADED, m_edX4Y4Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X3Y3LOADED, m_edX3Y3Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X2Y2LOADED, m_edX2Y2Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X1Y1LOADED, m_edX1Y1Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X4Y4ED, m_edX4Y4);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X3Y3ED, m_edX3Y3);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X2Y2ED, m_edX2Y2);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X1Y1ED, m_edX1Y1);
	DDX_Check(pDX, IDC_CMD_PLLD_AL_UNICHK, m_bUniChk);
	DDX_Radio(pDX, IDC_CMD_PLLD_AL_PT3, m_nPt34);
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_CMD_PLLD_AL_X1Y1ED, m_sXYEd[0]);
	DDX_Text(pDX, IDC_CMD_PLLD_AL_X2Y2ED, m_sXYEd[1]);
	DDX_Text(pDX, IDC_CMD_PLLD_AL_X3Y3ED, m_sXYEd[2]);
	DDX_Text(pDX, IDC_CMD_PLLD_AL_X4Y4ED, m_sXYEd[3]);
	

	
	
	//DDX_Text(pDX, IDC_CMD_PLLD_AL_X1Y1LOADED, m_sXYLdEd[0]);
	//DDX_Text(pDX, IDC_CMD_PLLD_AL_X2Y2LOADED, m_sXYLdEd[1]);
	//DDX_Text(pDX, IDC_CMD_PLLD_AL_X3Y3LOADED, m_sXYLdEd[2]);
	//DDX_Text(pDX, IDC_CMD_PLLD_AL_X4Y4LOADED, m_sXYLdEd[3]);
	

	DDX_Control(pDX, IDC_CMD_PLLD_AL_X1Y1UNIT, m_stXYUt[0]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X2Y2UNIT, m_stXYUt[1]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X3Y3UNIT, m_stXYUt[2]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X4Y4UNIT, m_stXYUt[3]);
		
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X1Y1LOADUNIT, m_stXYLdUt[0]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X2Y2LOADUNIT, m_stXYLdUt[1]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X3Y3LOADUNIT, m_stXYLdUt[2]);
	DDX_Control(pDX, IDC_CMD_PLLD_AL_X4Y4LOADUNIT, m_stXYLdUt[3]);
}


BEGIN_MESSAGE_MAP(CCMLoadPNLDAreaDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMLoadPNLDAreaDlg)
	ON_BN_CLICKED(IDC_CMD_PLLD_AL_UNICHK, OnCmdPlldAlUnichk)
	ON_BN_CLICKED(IDC_CMD_PLLD_AL_PT3, OnCmdPlldAlPt3)
	ON_BN_CLICKED(IDC_CMD_PLLD_AL_PT4, OnCmdPlldAlPt4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDAreaDlg message handlers


void	CCMLoadPNLDAreaDlg::SetData2Dlg(void)
{
	CString str;
	CWnd *pWnd[4],*pWndL[4];
	
	pWnd[0] = GetDlgItem(IDC_CMD_PLLD_AL_X1Y1ED);
	pWnd[1] = GetDlgItem(IDC_CMD_PLLD_AL_X2Y2ED);
	pWnd[2] = GetDlgItem(IDC_CMD_PLLD_AL_X3Y3ED);
	pWnd[3] = GetDlgItem(IDC_CMD_PLLD_AL_X4Y4ED);
	pWndL[0] = GetDlgItem(IDC_CMD_PLLD_AL_X1Y1LOADED);
	pWndL[1] = GetDlgItem(IDC_CMD_PLLD_AL_X2Y2LOADED);
	pWndL[2] = GetDlgItem(IDC_CMD_PLLD_AL_X3Y3LOADED);
	pWndL[3] = GetDlgItem(IDC_CMD_PLLD_AL_X4Y4LOADED);

	CArray<CString,CString> arStr;
	for(int i=0; i<4; i++)
	{
		// x,y
		m_sXYEd[i].Format(_T("%g,%g"),m_pData->AreaLoad.x[i],m_pData->AreaLoad.y[i]); 
	
		// load
		//arStr[i].Format(_T("%g"),m_pData->AreaLoad.dLoad[i]);
		str.Format(_T("%g"),m_pData->AreaLoad.dLoad[i]);
		arStr.Add(str);
	}
	m_edX1Y1Ld.SetWindowText((LPCTSTR)arStr[0]);
	m_edX2Y2Ld.SetWindowText((LPCTSTR)arStr[1]);
	m_edX3Y3Ld.SetWindowText((LPCTSTR)arStr[2]);
	m_edX4Y4Ld.SetWindowText((LPCTSTR)arStr[3]);
	
	if(m_pData->AreaLoad.b3Points)  this->m_nPt34 = 0;
	else                            this->m_nPt34 = 1;
	this->m_bUniChk  = 	m_pData->AreaLoad.bUniform;
	
	AfterPressUniChkAndPtRd(this->m_bUniChk,this->m_nPt34);
	UpdateData(FALSE);
}

BOOL	CCMLoadPNLDAreaDlg::SetDlg2Data(void)
{
	UpdateData(TRUE);

	double dVals[4];
	CArray<double,double> arrTmp;
	
	dVals[0] = m_edX1Y1Ld.GetEditValue();
	dVals[1] = m_edX2Y2Ld.GetEditValue();
	dVals[2] = m_edX3Y3Ld.GetEditValue();
	dVals[3] = m_edX4Y4Ld.GetEditValue();

	for(int i=0; i<4; i++)
	{
		// x,y
		arrTmp.RemoveAll();
		if(!CStrParser::GetUEDistance(m_sXYEd[i],arrTmp)) continue;
		if(arrTmp.GetSize()!=2)	continue;
		
		m_pData->AreaLoad.x[i] = arrTmp[0];
		m_pData->AreaLoad.y[i] = arrTmp[1];
		
		// load
		//m_pData->AreaLoad.dLoad[i] = _tstof(m_sXYLdEd[i]);
		m_pData->AreaLoad.dLoad[i] = dVals[i];
	}
	m_pData->AreaLoad.bUniform = this->m_bUniChk;
	if(this->m_nPt34==0) m_pData->AreaLoad.b3Points=TRUE;
	else                 m_pData->AreaLoad.b3Points=FALSE;

	return TRUE;
}

BOOL CCMLoadPNLDAreaDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	m_edX1Y1.SetNextLink(&m_edX2Y2);	// 다음  editor로 forcus이동 
	m_edX2Y2.SetNextLink(&m_edX3Y3);	// 다음  editor로 forcus이동 
	m_edX3Y3.SetNextLink(&m_edX4Y4);	// 다음  editor로 forcus이동 
	m_edX4Y4.SetNextLink(NULL);				// 다음  editor로 forcus이동 

	/*
	m_edX1Y1.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_edX2Y2.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_edX3Y3.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_edX4Y4.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	*///MOUSEEDIT_USE_SET_POS
	m_edX1Y1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edX2Y2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edX3Y3.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edX4Y4.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	

	m_edX1Y1.SetAttUcsPos();
	m_edX2Y2.SetAttUcsPos();
	m_edX3Y3.SetAttUcsPos();
	m_edX4Y4.SetAttUcsPos();
	m_edX1Y1.SetForceLevel1(TRUE);
	m_edX2Y2.SetForceLevel1(TRUE);
	m_edX3Y3.SetForceLevel1(TRUE);
	m_edX4Y4.SetForceLevel1(TRUE);
	m_edX1Y1.SetPosComponent(4);
	m_edX2Y2.SetPosComponent(4);
	m_edX3Y3.SetPosComponent(4);
	m_edX4Y4.SetPosComponent(4);


	SetData2Dlg();	
	InitUnit();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadPNLDAreaDlg::AfterPressUniChkAndPtRd(BOOL uni,int pt34)
{
	int i;
	CWnd *pLoadWnd[3],*pDistWnd[3];

	pDistWnd[0] = GetDlgItem(IDC_CMD_PLLD_AL_X2Y2ED);
	pDistWnd[1] = GetDlgItem(IDC_CMD_PLLD_AL_X3Y3ED);
	pDistWnd[2] = GetDlgItem(IDC_CMD_PLLD_AL_X4Y4ED);

	pLoadWnd[0] = GetDlgItem(IDC_CMD_PLLD_AL_X2Y2LOADED);
	pLoadWnd[1] = GetDlgItem(IDC_CMD_PLLD_AL_X3Y3LOADED);
	pLoadWnd[2] = GetDlgItem(IDC_CMD_PLLD_AL_X4Y4LOADED);

	for(i=0; i<3; i++) 
	{ 
		pLoadWnd[i]->EnableWindow(FALSE);	
		pDistWnd[i]->EnableWindow(FALSE);	
	} 
	// uniform + point3
	if(uni && pt34==0)
	{
		for(i=0; i<2; i++) pDistWnd[i]->EnableWindow(TRUE);
	}
	// uniform + point4
	else if(uni && pt34==1)
	{
		for(i=0; i<3; i++) pDistWnd[i]->EnableWindow(TRUE);
	}
	// linear + point3
	else if(!uni && pt34==0)
	{
		for(i=0; i<2; i++) pDistWnd[i]->EnableWindow(TRUE);	  
		for(i=0; i<2; i++) pLoadWnd[i]->EnableWindow(TRUE);
	}
	// linear + point4
	else if(!uni && pt34==1)
	{
		for(i=0; i<3; i++) pDistWnd[i]->EnableWindow(TRUE);	  
		for(i=0; i<3; i++) pLoadWnd[i]->EnableWindow(TRUE);
	}
}

void CCMLoadPNLDAreaDlg::OnCmdPlldAlUnichk() 
{
	UpdateData(TRUE);
	//AfterPressUniChkAndPtRd();
	AfterPressUniChkAndPtRd(this->m_bUniChk,this->m_nPt34);
}


void CCMLoadPNLDAreaDlg::InitUnit(void)
{
	UpdateData(TRUE);
	for(int i=0; i<4; i++)
	{
		m_stXYUt[i].SetUnitType(CUnitCtrl::m_PNLD_UNIT.x);
		m_stXYLdUt[i].SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLoad);
	}
	m_edX4Y4Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLoad);
	m_edX3Y3Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLoad);
	m_edX2Y2Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLoad);
	m_edX1Y1Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLoad);
}

BOOL CCMLoadPNLDAreaDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMLoadPNLDAreaDlg::OnCmdPlldAlPt3() 
{
	UpdateData(TRUE);
	//AfterPressUniChkAndPtRd();
	AfterPressUniChkAndPtRd(this->m_bUniChk,this->m_nPt34);
}

void CCMLoadPNLDAreaDlg::OnCmdPlldAlPt4() 
{
	UpdateData(TRUE);
	//AfterPressUniChkAndPtRd();
	AfterPressUniChkAndPtRd(this->m_bUniChk,this->m_nPt34);
}
