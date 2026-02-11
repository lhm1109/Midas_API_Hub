// WindGustIS875_15Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindGustIS875_15Dlg.h"
#include "SeisIS875_15CalcDlg.h"
#include "CMWindItemINDIA2015ManDlg.h"

#include <math.h>
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindGustIS875_15Dlg dialog
CWindGustIS875_15Dlg::CWindGustIS875_15Dlg(int nCodeType/*=KBC_W_2009*/, CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustIS875_15Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindGustIS875_15Dlg)
	// m_GustFactorDirction = 0;
	
	m_dCFS = 0.01;
	//}}AFX_DATA_INIT

	
	m_dGustFactorX = 0.0;
	m_dGustFactorY = 0.0;
	
	dBeta= 0.0;
	dAlongWind= 0.0;
	dAcrossWind= 0.0;
	iFactk=0;
	dCfs= 0.0;
	dMc= 0.0;
	iStrKind = 0;
	// m_bDirection = TRUE;  // 1 for X, 0 for Y
	m_bInit = FALSE;
	m_Data.Initialize();
	bIsUSerDefGustFact = FALSE;
}


void CWindGustIS875_15Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustIS875_15Dlg)
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_STRCATCOMBO, m_CboStruKind);
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_COMBO_FACTK, m_CboFactK);
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_COMBO_FACTKMC, m_CboFactMc);	
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_EDT_DAMPING_COEFF, m_editbeta);	
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_EDT_STRPARAM_ALONG, m_editalong);
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_EDT_STRPARAM_ACROSS, m_editacross);
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_EDT_STRPARAM_CFS, m_editcfs);
	DDX_Control(pDX, IDC_ETC_WIND_GUST_IS875_CHK_USERDEF_GF, m_chkUsrDefGustFact);
	
	// DDX_Radio(pDX, IDC_CMD_WINDGUST_GUST_RDO1, m_GustFactorDirction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindGustIS875_15Dlg, CDialogMove)
	
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_ETC_WIND_GUST_IS875_STRCATCOMBO, OnCbnSelchangeEtcWindGustIs875Strcatcombo)
	ON_CBN_SELCHANGE(IDC_ETC_WIND_GUST_IS875_COMBO_FACTK, OnCbnSelchangeEtcWindGustIs875ComboFactk)
	ON_CBN_SELCHANGE(IDC_ETC_WIND_GUST_IS875_COMBO_FACTKMC,OnCbnSelchangeEtcWindGustIs875ComboFactkmc)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnBnClickedCmdWindgustOk)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnBnClickedCmdWindgustCancel)
	ON_BN_CLICKED(IDC_ETC_WIND_GUST_IS875_BUTAUTOCALC, OnBnClickedEtcWindGustIs875Butautocalc)
	ON_BN_CLICKED(IDC_ETC_WIND_GUST_IS875_USERDEF, OnBnClickedEtcWindGustIs875Userdef)
	ON_BN_CLICKED(IDC_ETC_WIND_GUST_IS875_CHK_USERDEF_GF, OnBnClickedEtcWindGustIs875ChkUserdefGf)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustIS875_15Dlg message handlers

BOOL CWindGustIS875_15Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	// ((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->EnableWindow(m_GustFactorDirction == 1);
	

	InitDefaultData();



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustIS875_15Dlg::InitDefaultData()
{
	
	InitStrCategoryCombo();
	InitFactKCombo();
	InitFactMCCombo();
	m_editcfs.SetEditUnit(m_dCFS);
	InitBldgPeriod();
	m_chkUsrDefGustFact.SetCheck(bIsUSerDefGustFact);
	OnBnClickedEtcWindGustIs875ChkUserdefGf();
	CString cx,cy;
	cx.Format(_T("%g"),dAlongWind);
	cy.Format(_T("%g"),dAcrossWind);
	m_editalong.SetWindowText(cx);
	m_editacross.SetWindowText(cy);	
	UpdateData(FALSE); 
	
}

void CWindGustIS875_15Dlg::InitBldgPeriod()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);  
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();

	// 폭 구하기 
	double dBx = 0.0;
	double dBy = 0.0;
	if (nStorNum > 0)
	{	
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}

		if(StorD2.dStoryLevel > dBaseLevel) 
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	// 초기화 
	m_nNXCH2019 = nStorNum - 1;		
	m_nNYCH2019 = nStorNum - 1;		
	m_dHXCH2019 = StorD.dStoryLevel - dBaseLevel;	
	m_dHYCH2019 = StorD.dStoryLevel - dBaseLevel;
	m_dBXCH2019	 = dBx;
	m_dBYCH2019  = dBy;
	m_nMtdXCH2019 = 0;
	m_nMtdYCH2019 = 0;
}
void CWindGustIS875_15Dlg::InitStrCategoryCombo() 
{
		m_CboStruKind.ResetContent();
		CString aItem[] = {_T("Welded Steel"), _T("Bolted Steel"), 
			_T("Reinfored Concrete"), _T("Pre-Stressed Concrete"), _T("User Defined")};

		int nIndex;
		int nNum = sizeof(aItem) / sizeof(CString);
		for (int i=0; i<nNum; i++)
		{
			nIndex = m_CboStruKind.AddString(aItem[i]);
			m_CboStruKind.SetItemData(nIndex, i);
		}
		m_CboStruKind.SetCurSel(iStrKind); 	
}

void CWindGustIS875_15Dlg::InitFactKCombo() 
{
	m_CboFactK.ResetContent();
	CString aItem[] = {_T("0.5"), _T("1"), 
		_T("1.5"), _T("2.3")};

	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_CboFactK.AddString(aItem[i]);
		m_CboFactK.SetItemData(nIndex, i);
	}
	m_CboFactK.SetCurSel(iFactk); 
}

void CWindGustIS875_15Dlg::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}

void CWindGustIS875_15Dlg::InitFactMCCombo() 
{
	m_CboFactMc.ResetContent();
	CString aItem[] = {_T("Maximum"), _T("Minimum"), 
		_T("Average")};

	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_CboFactMc.AddString(aItem[i]);
		m_CboFactMc.SetItemData(nIndex, i);
	}
	m_CboFactMc.SetCurSel(dMc); 
	OnCbnSelchangeEtcWindGustIs875Strcatcombo();		
}

void CWindGustIS875_15Dlg::OnCbnSelchangeEtcWindGustIs875Strcatcombo()
{
	// TODO: Add your control notification handler code here
	m_editbeta.EnableWindow(FALSE);
	int nIndex = m_CboStruKind.GetCurSel();
	double dRiskCoef=0.01;
	if (nIndex == 0)	  dRiskCoef = 0.01;
	else if(nIndex == 1)  dRiskCoef = 0.02;
	else if(nIndex == 2)  dRiskCoef = 0.02;
	else if(nIndex == 3)  dRiskCoef = 0.016;
	else m_editbeta.EnableWindow(TRUE);
	
	m_editbeta.SetEditUnit(dRiskCoef);
}


void CWindGustIS875_15Dlg::OnCbnSelchangeEtcWindGustIs875ComboFactk()
{
	// TODO: Add your control notification handler code here
}


void CWindGustIS875_15Dlg::OnCbnSelchangeEtcWindGustIs875ComboFactkmc()
{
	// TODO: Add your control notification handler code here
}


void CWindGustIS875_15Dlg::OnBnClickedCmdWindgustOk()
{
	UpdateData(TRUE);
	//Non User Defined
	
	iStrKind	 = m_CboStruKind.GetCurSel();
	dBeta		 = m_editbeta.GetEditValue();
	dAlongWind	 = m_editalong.GetEditValue();
	dAcrossWind	 = m_editacross.GetEditValue();		
	iFactk = m_CboFactK.GetCurSel();
	m_CboFactK.GetLBText(iFactk, m_GustFactor);
	dCfs		 = m_editcfs.GetEditValue();
	dMc			 = m_CboFactMc.GetCurSel(); 
	bIsUSerDefGustFact = m_chkUsrDefGustFact.GetCheck();
	int iErr = 0;
	if (dBeta <= 0)
	{
		AfxMessageBox(_T("Damping coefficient has to be greater than 0."));
		iErr++;
	}
	if (dAlongWind <= 0)
	{
		AfxMessageBox(_T("Fundamental period along wind has to be greater than 0."));
		iErr++;
	}
	if (dAcrossWind <= 0)
	{
		AfxMessageBox(_T("Fundamental period across wind has to be greater than 0."));
		iErr++;
	}
	if (dCfs <= 0)
	{
		AfxMessageBox(_T("Across-Wind Force Spectrum coefficient must be greater than 0."));
		iErr++;
	}
	if (iErr ==0)
	{
		CDialogMove::OnOK();
	}
	
}


void CWindGustIS875_15Dlg::OnBnClickedCmdWindgustCancel()
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}


void CWindGustIS875_15Dlg::OnBnClickedEtcWindGustIs875Butautocalc()
{
	// TODO: Add your control notification handler code here
	int nUnitChange = 0;   // 주기계산 대화상자에서 wind load시에는 단위변환하고, 
	//                       seismic load시에는 단위변환않음			
	CSeisIS875_15CalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nUnitChange = nUnitChange;
	dlg.m_dHX			 = m_dHXCH2019;
	dlg.m_dHY			 = m_dHYCH2019;
	dlg.m_dBX			 = m_dBXCH2019;
	dlg.m_dBY			 = m_dBYCH2019;	
	dlg.m_nNX			 = m_nNXCH2019;
	dlg.m_nNY			 = m_nNYCH2019;	
	dlg.m_nXMethod = m_nMtdXCH2019;
	dlg.m_nYMethod = m_nMtdYCH2019;

	if(dlg.DoModal()== IDOK)
	{
		CString cx,cy;
		cx.Format(_T("%g"),dlg.m_dPeriod_X);
		cy.Format(_T("%g"),dlg.m_dPeriod_Y);
		m_editalong.SetWindowText(cx);
		m_editacross.SetWindowText(cy);		

		m_dHXCH2019		= dlg.m_dHX;
		m_dHYCH2019		= dlg.m_dHY;
		m_dBXCH2019		= dlg.m_dBX;
		m_dBYCH2019		=	dlg.m_dBY;
		m_nNXCH2019		= dlg.m_nNX;
		m_nNYCH2019		=	dlg.m_nNY;
		m_nMtdXCH2019 = dlg.m_nXMethod;
		m_nMtdYCH2019 = dlg.m_nYMethod;
	}
}


void CWindGustIS875_15Dlg::OnBnClickedEtcWindGustIs875Userdef()
{
	// TODO: Add your control notification handler code here
	CMWindItemINDIA2015ManDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	if(dlg.DoModal()== IDOK)
	{
	}
}


void CWindGustIS875_15Dlg::OnBnClickedEtcWindGustIs875ChkUserdefGf()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkUsrDefGustFact.GetCheck();
	GetDlgItem(IDC_ETC_WIND_GUST_IS875_USERDEF)->EnableWindow(nCheck == 1);
	
}

void CWindGustIS875_15Dlg::SetData2Dlg(T_WIND_IS875_2015& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
	m_CboStruKind.SetCurSel(m_Data.GustFact.nStrKind);
	m_editbeta.SetEditUnit(m_Data.GustFact.dBeta);
	m_editalong.SetEditUnit(m_Data.GustFact.dAlongWindPeriod);
	m_editacross.SetEditUnit(m_Data.GustFact.dAAcrossWindPeriod);
	m_CboFactK.SetCurSel(m_Data.GustFact.m_ifact);
	m_editcfs.SetEditUnit(m_Data.GustFact.dCfs);
	m_CboFactMc.SetCurSel(m_Data.GustFact.nMc);
	m_chkUsrDefGustFact.SetCheck(m_Data.GustFact.nProcedure);
}

BOOL CWindGustIS875_15Dlg::SetDlg2Data(T_WIND_IS875_2015& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

BOOL CWindGustIS875_15Dlg::Dlg2Data()
{
	iStrKind	 = m_CboStruKind.GetCurSel();
	dBeta		 = m_editbeta.GetEditValue();
	dAlongWind	 = m_editalong.GetEditValue();
	dAcrossWind	 = m_editacross.GetEditValue();		
	iFactk		 = (int)m_CboFactK.GetCurSel();
	dCfs		 = m_editcfs.GetEditValue();
	dMc			 = m_CboFactMc.GetCurSel(); 
	bIsUSerDefGustFact = m_chkUsrDefGustFact.GetCheck();
	int iErr = 0;
	if (dBeta <= 0)
	{
		AfxMessageBox(_T("Damping coefficient has to be greater than 0."));
		iErr++;
	}
	if (dAlongWind <= 0)
	{
		AfxMessageBox(_T("Fundamental period along wind has to be greater than 0."));
		iErr++;
	}
	if (dAcrossWind <= 0)
	{
		AfxMessageBox(_T("Fundamental period across wind has to be greater than 0."));
		iErr++;
	}
	if (dCfs <= 0)
	{
		AfxMessageBox(_T("Across-Wind Force Spectrum coefficient must be greater than 0."));
		iErr++;
	}
	
	m_Data.GustFact.dBeta				=  dBeta		;
	m_Data.GustFact.dAlongWindPeriod	=  dAlongWind	;
	m_Data.GustFact.dAAcrossWindPeriod  =  dAcrossWind	; 
	m_Data.GustFact.dModeShapeFact		= _wtof(m_GustFactor);
	m_Data.GustFact.dCfs				=  dCfs			;
	m_Data.GustFact.nMc					=  dMc			;
	m_Data.GustFact.nProcedure			=  bIsUSerDefGustFact;

	BOOL bOK= FALSE;
	if (iErr ==0)
	{
		bOK = TRUE;
	}
	
	return bOK;
}