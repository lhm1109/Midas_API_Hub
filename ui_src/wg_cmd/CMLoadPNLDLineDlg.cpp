// CMLoadPNLDLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadPNLDLineDlg.h"

/*
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\treemenubarbase.h"
#include "..\wg_base\I_GENModelBase.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\I_GENStateDefine.h"
*/
#include "..\wg_base\treemenubarbase.h"
#include "..\wg_base\I_GENModelBase.h"
#include "..\wg_db\flagctrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_StrParser.h"

using namespace mit::lib;

//#include "..\wg_cmd\wg_cmdAll.h"
//#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

//unsigned int CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW = 
//                RegisterWindowMessage(_T("WM_LBUTTONDOWN_FROM_MVIEW"));

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDLineDlg dialog


CCMLoadPNLDLineDlg::CCMLoadPNLDLineDlg(T_PNLD_D *pData,CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMLoadPNLDLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadPNLDLineDlg)
	//m_sX2Y2LdEd = _T("");
	//m_sX1Y1LdEd = _T("");
	m_sX2Y2Ed		= _T("");
	m_sX1Y1Ed		= _T("");
	m_bUniChk = FALSE;
	m_nTypeRD = -1;
	//m_sX1Y1MomEd = _T("");
	//m_sX2Y2MomEd = _T("");
	//}}AFX_DATA_INIT

	ASSERT(pData);
	m_pData = NULL;
	m_pData = pData;

	
}


void CCMLoadPNLDLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadPNLDLineDlg)
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2LOADUNIT_MOM, m_stX2Y2MomUt);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1LOADUNIT_MOM, m_stX1Y1MomUt);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2LOADED_MOM, m_edX2Y2Mom);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1LOADED_MOM, m_edX1Y1Mom);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2LOADED, m_edX2Y2Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1LOADED, m_edX1Y1Ld);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1ED, m_edX1Y1);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2ED, m_edX2Y2);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1UNIT, m_stX1Y1Ut);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2UNIT, m_stX2Y2Ut);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X2Y2LOADUNIT, m_stX2Y2LdUt);
	DDX_Control(pDX, IDC_CMD_PLLD_LL_X1Y1LOADUNIT, m_stX1Y1LdUt);
	DDX_Text(pDX, IDC_CMD_PLLD_LL_X2Y2ED, m_sX2Y2Ed);
	DDX_Text(pDX, IDC_CMD_PLLD_LL_X1Y1ED, m_sX1Y1Ed);
	DDX_Check(pDX, IDC_CMD_PLLD_LL_UNICHK, m_bUniChk);
	DDX_Radio(pDX, IDC_CMD_PLLD_LL_FRCRD, m_nTypeRD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadPNLDLineDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMLoadPNLDLineDlg)
	ON_BN_CLICKED(IDC_CMD_PLLD_LL_UNICHK, OnCmdPlldLlUnichk)
	ON_BN_CLICKED(IDC_CMD_PLLD_LL_FRCRD, OnCmdPlldLlFrcrd)
	ON_BN_CLICKED(IDC_CMD_PLLD_LL_MMTRD, OnCmdPlldLlMmtrd)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDLineDlg message handlers


void	CCMLoadPNLDLineDlg::SetData2Dlg(void)
{
	CString str;
	this->m_bUniChk = m_pData->LineLoad.bUniform;
	this->m_nTypeRD = m_pData->LineLoad.nType;
	this->m_sX1Y1Ed.Format(_T("%g,%g"),m_pData->LineLoad.x[0],m_pData->LineLoad.y[0]);
	this->m_sX2Y2Ed.Format(_T("%g,%g"),m_pData->LineLoad.x[1],m_pData->LineLoad.y[1]);
	//this->m_sX1Y1LdEd.Format(_T("%g"),m_pData->LineLoad.dForce[0]);
	//this->m_sX2Y2LdEd.Format(_T("%g"),m_pData->LineLoad.dForce[1]);
	//this->m_sX1Y1MomEd.Format(_T("%g"),m_pData->LineLoad.dMoment[0]);
	//this->m_sX2Y2MomEd.Format(_T("%g"),m_pData->LineLoad.dMoment[1]);
	str.Format(_T("%g"),m_pData->LineLoad.dForce[0]);
	m_edX1Y1Ld.SetWindowText(str);
	str.Format(_T("%g"),m_pData->LineLoad.dForce[1]);
	m_edX2Y2Ld.SetWindowText(str);
	str.Format(_T("%g"),m_pData->LineLoad.dMoment[0]);
	m_edX1Y1Mom.SetWindowText(str);
	str.Format(_T("%g"),m_pData->LineLoad.dMoment[1]);
	m_edX2Y2Mom.SetWindowText(str);  
	  
	UpdateData(FALSE);
	AfterPressUniChk(m_bUniChk);
	ShowEdit(m_pData->LineLoad.nType);
}

BOOL	CCMLoadPNLDLineDlg::SetDlg2Data(void)
{
	CArray<double,double> val;
	UpdateData(TRUE);
	m_pData->LineLoad.bUniform = this->m_bUniChk;
	m_pData->LineLoad.nType = this->m_nTypeRD;

	if(!CStrParser::GetUEDistance(m_sX1Y1Ed,val)) return FALSE;
	if(val.GetSize()!=2) return FALSE;
	m_pData->LineLoad.x[0] = val[0];	
	m_pData->LineLoad.y[0] = val[1];	
	val.RemoveAll();

	if(!CStrParser::GetUEDistance(m_sX2Y2Ed,val)) return FALSE;
	if(val.GetSize()!=2) return FALSE;
	m_pData->LineLoad.x[1] = val[0];	
	m_pData->LineLoad.y[1] = val[1];	
	val.RemoveAll();
	
	//m_pData->LineLoad.dForce[0] = _tstof(m_sX1Y1LdEd);
	//m_pData->LineLoad.dForce[1] = _tstof(m_sX2Y2LdEd);
	//m_pData->LineLoad.dMoment[0] = _tstof(m_sX1Y1MomEd);
	//m_pData->LineLoad.dMoment[1] = _tstof(m_sX2Y2MomEd);
	m_pData->LineLoad.dForce[0] = m_edX1Y1Ld.GetEditValue();
	m_pData->LineLoad.dForce[1] = m_edX2Y2Ld.GetEditValue();
	m_pData->LineLoad.dMoment[0] = m_edX1Y1Mom.GetEditValue();
	m_pData->LineLoad.dMoment[1] = m_edX2Y2Mom.GetEditValue();
	
	return TRUE;
}

BOOL CCMLoadPNLDLineDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();

	m_edX1Y1.SetNextLink(&m_edX2Y2);	// 다음  editor로 forcus이동 
	m_edX2Y2.SetNextLink(NULL);	      // 다음  editor로 forcus이동 
	m_edX1Y1.SetLButtonDownNotifyWindow(this);
	m_edX2Y2.SetLButtonDownNotifyWindow(this);
	//m_edX1Y1.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	//m_edX2Y2.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_edX1Y1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edX2Y2.SetModeToUse(MOUSEEDIT_USE_SET_POS);

	m_edX1Y1.SetAttUcsPos();
	m_edX2Y2.SetAttUcsPos();
	m_edX1Y1.SetForceLevel1(TRUE);
	m_edX2Y2.SetForceLevel1(TRUE);
	m_edX1Y1.SetPosComponent(4);
	m_edX2Y2.SetPosComponent(4);

	InitUnit();		// SetData2Dlg뒤에 있어야함.. force, moment인지 구분.. 
	SetData2Dlg();	
	ShowEdit(m_pData->LineLoad.nType);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMLoadPNLDLineDlg::AfterPressUniChk(BOOL uni)
{
	CWnd *pLoadWnd[2];
	
//	pDistWnd= GetDlgItem(IDC_CMD_PLLD_LL_X2Y2ED);
	pLoadWnd[0]= GetDlgItem(IDC_CMD_PLLD_LL_X2Y2LOADED);
	pLoadWnd[1]= GetDlgItem(IDC_CMD_PLLD_LL_X2Y2LOADED_MOM);
	ASSERT(m_nTypeRD>=0);

	// uniform
	if(uni)
	{
//		pDistWnd->EnableWindow(FALSE);
		
		 //if(m_nTypeRD)  pLoadWnd->EnableWindow(FALSE);
		 //else           
		 pLoadWnd[m_nTypeRD]->EnableWindow(FALSE); 
	}
	// linear
	else 
	{
//		pDistWnd->EnableWindow(TRUE);
		// pLoadWnd->EnableWindow(TRUE);
		pLoadWnd[m_nTypeRD]->EnableWindow(TRUE); 
	}
}

void CCMLoadPNLDLineDlg::OnCmdPlldLlUnichk() 
{
	UpdateData(TRUE);
	AfterPressUniChk(this->m_bUniChk);	
}


void CCMLoadPNLDLineDlg::InitUnit(void)
{
	//UpdateData(FALSE);

	this->m_stX1Y1Ut.SetUnitType(CUnitCtrl::m_PNLD_UNIT.x);
	this->m_stX2Y2Ut.SetUnitType(CUnitCtrl::m_PNLD_UNIT.x);
	/*
	if(!this->m_nTypeRD)
	{
		this->m_stX1Y1LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
		this->m_stX2Y2LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
	}
	else
	{
		this->m_stX1Y1LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
		this->m_stX2Y2LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	}
	*/
	m_stX1Y1LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
	m_stX2Y2LdUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
	m_stX2Y2MomUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	m_stX1Y1MomUt.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	m_edX1Y1Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
	m_edX2Y2Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineForce);
	m_edX1Y1Mom.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	m_edX2Y2Mom.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	
	//m_edX2Y2Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
	//m_edX1Y1Ld.SetUnitType(CUnitCtrl::m_PNLD_UNIT.dLineMoment);
}

void CCMLoadPNLDLineDlg::ShowEdit(BOOL Fg)
{
	//UpdateData(TRUE);
	if(!Fg)
	{
		m_edX1Y1Ld.ShowWindow(SW_SHOW);
		m_edX2Y2Ld.ShowWindow(SW_SHOW);
		m_edX1Y1Mom.ShowWindow(SW_HIDE);
		m_edX2Y2Mom.ShowWindow(SW_HIDE);

		m_stX2Y2LdUt.ShowWindow(SW_SHOW);
	  m_stX1Y1LdUt.ShowWindow(SW_SHOW);
		m_stX2Y2MomUt.ShowWindow(SW_HIDE);
	  m_stX1Y1MomUt.ShowWindow(SW_HIDE);
	}
	else
	{
		m_edX1Y1Ld.ShowWindow(SW_HIDE);
		m_edX2Y2Ld.ShowWindow(SW_HIDE);
		m_edX1Y1Mom.ShowWindow(SW_SHOW);
		m_edX2Y2Mom.ShowWindow(SW_SHOW);

		m_stX2Y2LdUt.ShowWindow(SW_HIDE);
	  m_stX1Y1LdUt.ShowWindow(SW_HIDE);
		m_stX2Y2MomUt.ShowWindow(SW_SHOW);
	  m_stX1Y1MomUt.ShowWindow(SW_SHOW);
	}
}

void CCMLoadPNLDLineDlg::OnCmdPlldLlFrcrd() 
{
	UpdateData(TRUE);
	//InitUnit();	
	ShowEdit(m_nTypeRD);
	AfterPressUniChk(m_bUniChk);
}

void CCMLoadPNLDLineDlg::OnCmdPlldLlMmtrd() 
{
	UpdateData(TRUE);
	//InitUnit();	
	ShowEdit(m_nTypeRD);
	AfterPressUniChk(m_bUniChk);
}

BOOL CCMLoadPNLDLineDlg::PreTranslateMessage(MSG* pMsg) 
{
		// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;
	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}

LRESULT CCMLoadPNLDLineDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	if(pIGM)
	{
		//this->SetFocus();
		pIGM->EndTempFLoad();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
	}
	return 0L;
}
