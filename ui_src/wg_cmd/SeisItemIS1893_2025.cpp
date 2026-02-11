// SeisItemIS2002.cpp : implementation file
//
// 2003. 12. 29   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemDlgNew.h"
#include "SeisItemIS1893_2025.h"
#include "..\wg_base\DlgUtil.h"
#include "SeisIS1893_25PeriodCalcDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIS1893_2025 dialog


CSeisItemIS1893_2025::CSeisItemIS1893_2025(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemIS1893_2025::IDD, pParent)
{
	m_aRFactorName.RemoveAll();
	m_aRFactorData.RemoveAll();
	m_aRFactorName.Add(_T("1.5")); m_aRFactorData.Add(1.5);
	m_aRFactorName.Add(_T("2.5")); m_aRFactorData.Add(2.5);
	m_aRFactorName.Add(_T("3.0")); m_aRFactorData.Add(3.0);
	m_aRFactorName.Add(_T("4.0")); m_aRFactorData.Add(4.0);
	m_aRFactorName.Add(_T("4.5")); m_aRFactorData.Add(4.5);
	m_aRFactorName.Add(_T("5.0")); m_aRFactorData.Add(5.0);
	m_aRFactorName.Add(_T("5.5")); m_aRFactorData.Add(5.5);
	m_aRFactorName.Add(_T("6.0")); m_aRFactorData.Add(6.0);
	//{{AFX_DATA_INIT(CSeisItemIS1893_2025)
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemIS1893_2025::SetData2Dlg(T_SEIS_IS1893_2025& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;  
}

BOOL CSeisItemIS1893_2025::SetDlg2Data(T_SEIS_IS1893_2025& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemIS1893_2025::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemIS1893_2025)
	DDX_Control(pDX, IDC_CMD_IS25_SPEC_TYPE_CMB, m_cmbSpecType);
	DDX_Control(pDX, IDC_CMD_IS25_EQ_ZONE_CMB, m_cmbEqZone);
	DDX_Control(pDX, IDC_CMD_IS25_RET_PERIOD_CMB, m_cmbRetPeriod);
	DDX_Control(pDX, IDC_CMD_IS25_ZONE_FACTOR_EDIT, m_editZoneFactor);
	DDX_Control(pDX, IDC_CMD_IS25_SITE_CLASS_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_IS25_IMPORTANCE_CMB, m_cmbImportanceFact);
	DDX_Control(pDX, IDC_CMD_IS25_DAMPING_EDIT, m_editDamping);
	DDX_Control(pDX, IDC_CMD_IS02_TX_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_IS02_TY_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_IS02_RX_CMB, m_cobxRX);
	DDX_Control(pDX, IDC_CMD_IS02_RY_CMB, m_cobxRY);
	DDX_Control(pDX, IDC_CMD_IS02_FRAME, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemIS1893_2025, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemIS1893_2025)
	ON_BN_CLICKED(IDC_CMD_IS02_T_CALC_BTN, OnCmdIs25TCalcBtn)
	ON_CBN_SELCHANGE(IDC_CMD_IS25_EQ_ZONE_CMB, OnEqZoneChange)
	ON_CBN_SELCHANGE(IDC_CMD_IS25_RET_PERIOD_CMB, OnRetPeriodChange)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIS1893_2025 message handlers

BOOL CSeisItemIS1893_2025::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editDamping.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodX.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodY.SetUnitType(D_UNITSYS_NONE);

	

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	InitSpectrumTypeCombo();
	InitEqZoneCombo();
	InitRetPeriodCombo();
	InitSiteClassCombo();
	m_editZoneFactor.SetEditUnit(0.0375);

	CString strImportance[] = { _T("1.0"),_T("1.15") };
	CString strResponse[] = { _T("1.5"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0"),_T("5.5") ,_T("6.0") };

	for(int i = 0; i < 2; i++)  m_cmbImportanceFact.AddString(strImportance[i]);
	for(int i = 0; i < 8; i++)  m_cobxRX.AddString(strResponse[i]);
	for(int i = 0; i < 8; i++)  m_cobxRY.AddString(strResponse[i]);

	m_cmbImportanceFact.SetWindowTextW(strImportance[0]);		// 1.0
	m_cobxRX.SetCurSel(5);
	m_cobxRY.SetCurSel(5);

	SetFoldGroupCtrls();
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemIS1893_2025::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemIS1893_2025::IDD, IDC_CMD_IS02_FRAME, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemIS1893_2025::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data();
	if(m_pParent != NULL)
	{
		if(!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CSeisItemIS1893_2025::OnCmdIs25TCalcBtn() 
{
	// TODO: Add your control notification handler code here	

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	T_KEY_LIST arStorK; arStorK.RemoveAll();
	pAttrCtrl->GetStorKeyList(arStorK);
	if(arStorK.GetSize() == 0)
	{
		AfxMessageBox(_T("Please define story data!"));
		return;
	}

	CSeisIS1893_25PeriodCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);		

	dlg.m_dXHedit	 = m_dHXIS2025;		
	dlg.m_dYHedit  = m_dHYIS2025;		
	dlg.m_dXAfpedit = m_dAfpX;	
	dlg.m_dYAfpedit = m_dAfpY;	
	dlg.m_dXAcoedit = m_dAcoX;	
	dlg.m_dYAcoedit = m_dAcoY;	
	dlg.m_dXAwaedit = m_dAwaX;	
	dlg.m_dYAwaedit = m_dAwaY;	
	dlg.m_dXAmwedit = m_dAmwX;	
	dlg.m_dYAmwedit = m_dAmwY;
	dlg.m_dXDedit  = m_dBXIS2025;		
	dlg.m_dYDedit	 = m_dBYIS2025;		
	dlg.m_nXMethod = m_nMtdXIS2025;
	dlg.m_nYMethod = m_nMtdYIS2025;
	dlg.m_nBaseStoryK = m_nBaseStoryK;
	
	if (dlg.DoModal()== IDOK)
	{
		CString cx,cy;
		cx.Format(_T("%-10.4f"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4f"), dlg.m_dPeriod_Y);
		m_editPeriodX.SetEditUnit(_tstof(cx), 4);
		m_editPeriodY.SetEditUnit(_tstof(cy), 4);

		m_dHXIS2025	  = dlg.m_dXHedit;
		m_dHYIS2025	  = dlg.m_dYHedit;
		m_dBXIS2025	  = dlg.m_dXDedit;
		m_dBYIS2025	  = dlg.m_dYDedit;
		m_nMtdXIS2025 = dlg.m_nXMethod;
		m_nMtdYIS2025 = dlg.m_nYMethod;
		m_dAfpX	      = dlg.m_dXAfpedit;
		m_dAfpY	      = dlg.m_dYAfpedit;
		m_dAcoX	      = dlg.m_dXAcoedit;
		m_dAcoY	      = dlg.m_dYAcoedit;
		m_dAwaX	      = dlg.m_dXAwaedit;
		m_dAwaY	      = dlg.m_dYAwaedit;
		m_dAmwX	      = dlg.m_dXAmwedit;
		m_dAmwY	      = dlg.m_dYAmwedit;
		m_nBaseStoryK = dlg.m_nBaseStoryK;

	}			
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemIS1893_2025::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dBx = 1.0;
	double dBy = 1.0;
	double dHn = 0.0;
	T_STOR_D StorD, BaseStorD;
	
	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);   

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum>0) pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();
		
	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = max(StorD.dStoryLevel - dBaseLevel, 0.0);
	if (!pAttrCtrl->GetCountStor())	dHn = 0;
	
	if (nStorNum > 0)
	{	
		for (int i=0; i<nStorNum; i++)
		{
			pAttrCtrl->GetStor(aStorK[i], BaseStorD);
			if (BaseStorD.dStoryLevel > dBaseLevel) break;
		}
		if (BaseStorD.dStoryLevel > dBaseLevel) 
		{
			dBx = BaseStorD.WindWidthX;
			dBy = BaseStorD.WindWidthY;
		}
		else { dBx = dBy = 1.0; }
	}
	else { dBx = dBy = 1.0; }

	m_nBaseStoryK = aStorK.GetSize() > 0 ? aStorK[0] : 0;
	m_dHXIS2025  = m_dHYIS2025 = 0.0;
	m_dAwaX	=0.0;
	m_dAwaY	= 0.0;
	double dAreaFloorPlanAtBase = 0.0;
	m_dBXIS2025  = dBx;
	m_dBYIS2025  = dBy;
	m_nMtdXIS2025 = 3;
	m_nMtdYIS2025 = 3;
	m_dAcoX	= 0.0;
	m_dAcoY	= 0.0;
	m_dAfpX	= 0.0;
	m_dAfpY	= 0.0;
	m_dAmwX = 0.0;
	m_dAmwY = 0.0;
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemIS1893_2025::InitSpectrumTypeCombo()
{
	// 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	m_cmbSpecType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbSpecType, _LS(IDS_CMD_IS2025_H_SPECTRUM), 0);
	m_cmbSpecType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSpecType);
}


void CSeisItemIS1893_2025::InitEqZoneCombo()
{
	// 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	m_cmbEqZone.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("II"), EN_IS25_EQ_ZONE_II);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("III"), EN_IS25_EQ_ZONE_III);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("IV"), EN_IS25_EQ_ZONE_IV);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("V"), EN_IS25_EQ_ZONE_V);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("VI"), EN_IS25_EQ_ZONE_VI);
	m_cmbEqZone.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbEqZone);
}

void CSeisItemIS1893_2025::InitRetPeriodCombo()
{
	m_cmbRetPeriod.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("75"), EN_IS25_RET_75);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("175"), EN_IS25_RET_175);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("275"), EN_IS25_RET_275);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("475"), EN_IS25_RET_475);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("975"), EN_IS25_RET_975);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("1275"), EN_IS25_RET_1275);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("2475"), EN_IS25_RET_2475);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("4975"), EN_IS25_RET_4975);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("9975"), EN_IS25_RET_9975);
	m_cmbRetPeriod.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbRetPeriod);
}

void CSeisItemIS1893_2025::InitSiteClassCombo()
{
	m_cmbSiteClass.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("A"), EN_IS25_SITE_A);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("B"), EN_IS25_SITE_B);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("C"), EN_IS25_SITE_C);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("D"), EN_IS25_SITE_D);
	m_cmbSiteClass.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSiteClass);
}

void CSeisItemIS1893_2025::OnEqZoneChange()
{
	Dlg2Data();
	double dZoneFact = m_adZoneFacts[m_Data.nEqZone][m_Data.nReturnPeriod];
	m_editZoneFactor.SetEditUnit(dZoneFact);
	m_Data.dZoneFactor = dZoneFact;
}

void CSeisItemIS1893_2025::OnRetPeriodChange()
{
	Dlg2Data();
	double dZoneFact = m_adZoneFacts[m_Data.nEqZone][m_Data.nReturnPeriod];
	m_editZoneFactor.SetEditUnit(dZoneFact);
	m_Data.dZoneFactor = dZoneFact;
}

void CSeisItemIS1893_2025::Data2Dlg()
{
	int i, nCount;
	T_PREFERENCE  ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	CString csValue;
	m_cmbSpecType.SetCurSel(m_Data.nSpecType);
	m_cmbEqZone.SetCurSel(m_Data.nEqZone);
	m_cmbRetPeriod.SetCurSel(m_Data.nReturnPeriod);
	m_cmbSiteClass.SetCurSel(m_Data.nSiteClass);
	m_editZoneFactor.SetEditUnit(m_Data.dZoneFactor);
	
	CString strImportance[] = { _T("1.0"),_T("1.15") };
	if (fabs(m_Data.dImpFactor - 1.0) <= zero)
		m_cmbImportanceFact.SetCurSel(0);
	else if (fabs(m_Data.dImpFactor - 1.15) <= zero)
		m_cmbImportanceFact.SetCurSel(1);
	else
	{
		csValue.Format(_T("%g"), m_Data.dImpFactor);
		m_cmbImportanceFact.SetWindowText(csValue);
	}

	m_editDamping.SetEditUnit(m_Data.dDamping);

	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);

	nCount = m_aRFactorData.GetSize();
	for (i=0; i<nCount; i++)
		if (fabs(m_Data.dResponseReductionFactor_X - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRX.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseReductionFactor_X);
		m_cobxRX.SetWindowText(csValue);
	}
	for (i=0; i<nCount; i++)
		if (fabs(m_Data.dResponseReductionFactor_Y - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRY.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseReductionFactor_Y);
		m_cobxRY.SetWindowText(csValue);
	}
}

BOOL CSeisItemIS1893_2025::Dlg2Data()
{
	CString str;
	CString strError;
	strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));

	m_Data.nSpecType	= m_cmbSpecType.GetCurSel();
	m_Data.nEqZone      = m_cmbEqZone.GetCurSel();
	m_Data.nReturnPeriod = m_cmbRetPeriod.GetCurSel();
	m_Data.dZoneFactor = m_editZoneFactor.GetEditValue();
	m_Data.nSiteClass   = m_cmbSiteClass.GetCurSel();
	m_cmbImportanceFact.GetWindowText(str);  
	m_Data.dImpFactor = _tstof(str);
	m_Data.dDamping     = m_editDamping.GetEditValue();
	m_Data.dPeriod_X    = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y    = m_editPeriodY.GetEditValue();
	m_cobxRX.GetWindowText(str);
	m_Data.dResponseReductionFactor_X = _tstof(str);
	m_cobxRY.GetWindowText(str);
	m_Data.dResponseReductionFactor_Y = _tstof(str);

	if (dgn::LT0(m_Data.dPeriod_X) || dgn::LT0(m_Data.dPeriod_X))
	{    
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	if (dgn::LE0(m_Data.dResponseReductionFactor_X) || dgn::LE0(m_Data.dResponseReductionFactor_Y))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Reduction_Fact));
		return FALSE;
	}
	if (dgn::LE0(m_Data.dImpFactor))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Importance_Fact));
	}
	if (dgn::LT0(m_Data.dDamping))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Damping));
	}
	if (dgn::LE0(m_Data.dZoneFactor))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Zone_Factor));
	}
	return TRUE;
}

LRESULT CSeisItemIS1893_2025::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	m_grpFold.GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
