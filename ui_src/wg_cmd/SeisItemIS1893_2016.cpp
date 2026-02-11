// SeisItemIS2002.cpp : implementation file
//
// 2003. 12. 29   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemIS1893_2016.h"
#include "SeisItemDlgNew.h"

#include "SeisIS1893_16PeriodCalcDlg.h"

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
// CSeisItemIS1893_2016 dialog


CSeisItemIS1893_2016::CSeisItemIS1893_2016(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemIS1893_2016::IDD, pParent)
{
	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.2"));
	m_aIFactorData.Add(1.2);
	m_aIFactorName.Add(_T("1.5"));
	m_aIFactorData.Add(1.5);

	m_aRFactorName.RemoveAll();
	m_aRFactorData.RemoveAll();
	m_aRFactorName.Add(_T("1.5"));
	m_aRFactorData.Add(1.5);
	m_aRFactorName.Add(_T("2.0"));
	m_aRFactorData.Add(2.0);
	m_aRFactorName.Add(_T("2.5"));
	m_aRFactorData.Add(2.5);
	m_aRFactorName.Add(_T("3.0"));
	m_aRFactorData.Add(3.0);
	m_aRFactorName.Add(_T("4.0"));
	m_aRFactorData.Add(4.0);
	m_aRFactorName.Add(_T("4.5"));
	m_aRFactorData.Add(4.5);
	m_aRFactorName.Add(_T("5.0"));
	m_aRFactorData.Add(5.0);

	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemIS1893_2016::SetData2Dlg(T_SEIS_IS1893_2016& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemIS1893_2016::SetDlg2Data(T_SEIS_IS1893_2016& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemIS1893_2016::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_IS02_ZONE_CMB, m_cobxZone);
	DDX_Control(pDX, IDC_CMD_IS02_SOIL_CMB, m_cobxSoilType);
	DDX_Control(pDX, IDC_CMD_IS02_IMPORTANCE_CMB, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_IS02_DAMPING_EDIT, m_editDamping);
	DDX_Control(pDX, IDC_CMD_IS02_DAMPING_FACTOR_EDIT, m_editDFactor);
	DDX_Control(pDX, IDC_CMD_IS02_TX_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_IS02_TY_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_IS02_RX_CMB, m_cobxRX);
	DDX_Control(pDX, IDC_CMD_IS02_RY_CMB, m_cobxRY);
	DDX_Control(pDX, IDC_CMD_IS02_FRAME, m_grpFold);
}

BEGIN_MESSAGE_MAP(CSeisItemIS1893_2016, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_IS02_T_CALC_BTN, OnCmdIs02TCalcBtn)
	ON_EN_CHANGE(IDC_CMD_IS02_DAMPING_EDIT, OnChangeCmdIs02DampingEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIS1893_2016 message handlers

BOOL CSeisItemIS1893_2016::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editDamping.SetUnitType(D_UNITSYS_NONE);
	m_editDFactor.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodX.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitZoneCombo();
	InitSoilCombo();
	InitIFactorCombo();
	InitRxRyCombo();
	SetFoldGroupCtrls();

	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemIS1893_2016::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemIS1893_2016::IDD, IDC_CMD_IS02_FRAME, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemIS1893_2016::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data();
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();
}

void CSeisItemIS1893_2016::OnCmdIs02TCalcBtn()
{
	// TODO: Add your control notification handler code here	
	CSeisIS1893PeriodCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_dXHedit = m_dHXIS2002;
	dlg.m_dYHedit = m_dHYIS2002;
	dlg.m_dXAcedit = m_dAcX;
	dlg.m_dYAcedit = m_dAcY;
	dlg.m_dXDedit = m_dBXIS2002;
	dlg.m_dYDedit = m_dBYIS2002;
	dlg.m_nXMethod = m_nMtdXIS2002;
	dlg.m_nYMethod = m_nMtdYIS2002;

	// 	dlg.m_dHX			 = m_dHXIS2002;
	// 	dlg.m_dHY			 = m_dHYIS2002;
	// 	dlg.m_dBX			 = m_dBXIS2002;
	// 	dlg.m_dBY			 = m_dBYIS2002;	
	// 	dlg.m_nXMethod = m_nMtdXIS2002;
	// 	dlg.m_nYMethod = m_nMtdYIS2002;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_Y);
		m_editPeriodX.SetEditUnit(_tstof(cx));
		m_editPeriodY.SetEditUnit(_tstof(cy));

		m_dHXIS2002 = dlg.m_dXHedit;
		m_dHYIS2002 = dlg.m_dYHedit;
		m_dBXIS2002 = dlg.m_dXDedit;
		m_dBYIS2002 = dlg.m_dYDedit;
		m_nMtdXIS2002 = dlg.m_nXMethod;
		m_nMtdYIS2002 = dlg.m_nYMethod;
		m_dAcX = dlg.m_dXAcedit;
		m_dAcY = dlg.m_dYAcedit;
	}
}

void CSeisItemIS1893_2016::OnChangeCmdIs02DampingEdit()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	double dDamping = m_editDamping.GetEditValue();
	m_editDFactor.SetEditUnit(GetDampingFactor(dDamping));
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemIS1893_2016::InitDefaultData()
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
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	if (nStorNum > 0)
	{
		for (int i = 0; i < nStorNum; i++)
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

	m_dHXIS2002 = m_dHYIS2002 = dHn;
	m_dBXIS2002 = dBx;
	m_dBYIS2002 = dBy;
	m_nMtdXIS2002 = 0;
	m_nMtdYIS2002 = 0;
	m_dAcX = 0.0;
	m_dAcY = 0.0;
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemIS1893_2016::InitZoneCombo()
{
	m_cobxZone.ResetContent();
	m_cobxZone.AddString(_T("II (0.10)"));
	m_cobxZone.AddString(_T("III (0.16)"));
	m_cobxZone.AddString(_T("IV (0.24)"));
	m_cobxZone.AddString(_T("V (0.36)"));
	m_cobxZone.SetCurSel(0);
}

void CSeisItemIS1893_2016::InitSoilCombo()
{
	m_cobxSoilType.ResetContent();
	m_cobxSoilType.AddString(_T("I (Rock or Hard Soil)"));
	m_cobxSoilType.AddString(_T("II (Medium Soil)"));
	m_cobxSoilType.AddString(_T("III (Soft Soil)"));
	m_cobxSoilType.SetCurSel(0);
}

void CSeisItemIS1893_2016::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemIS1893_2016::InitRxRyCombo()
{
	m_cobxRX.ResetContent();
	m_cobxRY.ResetContent();
	for (int i = 0; i < m_aRFactorName.GetSize(); i++)
	{
		m_cobxRX.AddString(m_aRFactorName[i]);
		m_cobxRY.AddString(m_aRFactorName[i]);
	}
	m_cobxRX.SetCurSel(0);
	m_cobxRY.SetCurSel(0);
}

void CSeisItemIS1893_2016::Data2Dlg()
{
	int i, nCount;
	T_PREFERENCE ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	CString csValue;
	m_cobxZone.SetCurSel(m_Data.nSeismicZone);
	m_cobxSoilType.SetCurSel(m_Data.nSoilType);

	nCount = m_aIFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dImportanceFactor - m_aIFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxIFactor.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxIFactor.SetWindowText(csValue);
	}

	m_editDamping.SetEditUnit(m_Data.dDamping);
	m_editDFactor.SetEditUnit(GetDampingFactor(m_Data.dDamping));

	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);

	nCount = m_aRFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dResponseReductionFactor_X - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRX.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseReductionFactor_X);
		m_cobxRX.SetWindowText(csValue);
	}
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dResponseReductionFactor_Y - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRY.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseReductionFactor_Y);
		m_cobxRY.SetWindowText(csValue);
	}
}

BOOL CSeisItemIS1893_2016::Dlg2Data()
{
	CString str;
	CString strError;
	strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));

	m_Data.nSeismicZone = m_cobxZone.GetCurSel();
	m_Data.nSoilType = m_cobxSoilType.GetCurSel();
	m_cobxIFactor.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);
	m_Data.dDamping = m_editDamping.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();
	m_cobxRX.GetWindowText(str);
	m_Data.dResponseReductionFactor_X = _tstof(str);
	m_cobxRY.GetWindowText(str);
	m_Data.dResponseReductionFactor_Y = _tstof(str);

	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_X < 0)
	{
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	if (m_Data.dResponseReductionFactor_X <= 0 || m_Data.dResponseReductionFactor_Y <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Reduction_Fact));
		return FALSE;
	}

	/*
	CString csValue;
	m_Data.nSoilType = m_cobxSoilType.GetCurSel();
	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);
	m_Data.dDamping = m_editDamping.GetEditValue();

	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();
	m_cobxRX.GetWindowText(csValue);
	m_Data.dResponseReductionFactor_X = _tstof(csValue);
	m_cobxRY.GetWindowText(csValue);
	m_Data.dResponseReductionFactor_Y = _tstof(csValue);
	*/

	return TRUE;
}

double CSeisItemIS1893_2016::GetDampingFactor(double dDamping)
{
	double dDampingFactor = 0.;
	double aDamping[] = {0., 2., 5., 7., 10., 15., 20., 25., 30.};
	double aFactor[] = {3.20, 1.40, 1.00, 0.90, 0.80, 0.70, 0.60, 0.55, 0.50};

	if (dDamping < 0) dDamping = 0.;
	if (dDamping > 30) dDamping = 30.;
	int Index = 8;
	for (Index = 8; Index >= 0; Index--)
	{
		if (dDamping >= aDamping[Index]) break;
	}
	double dDifRatio = 0.;
	if (Index < 8)
	{
		dDifRatio = (dDamping - aDamping[Index]) / (aDamping[Index + 1] - aDamping[Index]);
		dDampingFactor = aFactor[Index] - (aFactor[Index] - aFactor[Index + 1]) * dDifRatio;
	}
	else dDampingFactor = 0.50;

	return dDampingFactor;
}

LRESULT CSeisItemIS1893_2016::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
