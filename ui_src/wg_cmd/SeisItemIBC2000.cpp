// SiesItemIBC2000.cpp : implementation file
//
// 2003. 12. 30   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemIBC2000.h"
#include "SeisItemDlgNew.h"

#include "SeisIBCCalcDlg.h"

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
// CSeisItemIBC2000 dialog


CSeisItemIBC2000::CSeisItemIBC2000(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemIBC2000::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aSsName.RemoveAll();
	m_aSsData.RemoveAll();
	m_aSsName.Add(_T("0.25"));
	m_aSsData.Add(0.25);
	m_aSsName.Add(_T("0.5"));
	m_aSsData.Add(0.5);
	m_aSsName.Add(_T("0.75"));
	m_aSsData.Add(0.75);
	m_aSsName.Add(_T("1.0"));
	m_aSsData.Add(1.0);
	m_aSsName.Add(_T("1.25"));
	m_aSsData.Add(1.25);

	m_aS1Name.RemoveAll();
	m_aS1Data.RemoveAll();
	m_aS1Name.Add(_T("0.1"));
	m_aS1Data.Add(0.1);
	m_aS1Name.Add(_T("0.2"));
	m_aS1Data.Add(0.2);
	m_aS1Name.Add(_T("0.3"));
	m_aS1Data.Add(0.3);
	m_aS1Name.Add(_T("0.4"));
	m_aS1Data.Add(0.4);
	m_aS1Name.Add(_T("0.5"));
	m_aS1Data.Add(0.5);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.25"));
	m_aIFactorData.Add(1.25);
	m_aIFactorName.Add(_T("1.5"));
	m_aIFactorData.Add(1.5);

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CSeisItemIBC2000::SetData2Dlg(T_SEIS_IBC2000& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemIBC2000::SetDlg2Data(T_SEIS_IBC2000& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemIBC2000::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO1, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO3, m_cobxSs);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO2, m_cobxSite);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO4, m_cobxS1);
	DDX_Control(pDX, IDC_CMD_IBC_COMBO5, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT3, m_editAX);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT3, m_editAY);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT1, m_editCX);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT1, m_editCY);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_X_EDIT2, m_editRX);
	DDX_Control(pDX, IDC_CMD_IBC_PERIOD_Y_EDIT2, m_editRY);
	DDX_Control(pDX, IDC_CMD_IBC_FRAME1, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemIBC2000, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemIBC2000)
	ON_BN_CLICKED(IDC_CMD_IBC_BTN_CALC_PERIOD, OnCmdIbcBtnCalcPeriod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIBC2000 message handlers

BOOL CSeisItemIBC2000::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editAX.SetUnitType(D_UNITSYS_NONE);
	m_editAY.SetUnitType(D_UNITSYS_NONE);
	m_editCX.SetUnitType(D_UNITSYS_NONE);
	m_editCY.SetUnitType(D_UNITSYS_NONE);
	m_editRX.SetUnitType(D_UNITSYS_NONE);
	m_editRY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitCategoryCombo();
	InitSiteClassCombo();
	InitShtPeriodCombo();
	InitSecPeriodCombo();
	InitIFactorCombo();
	SetFoldGroupCtrls();

	if (m_bModify) Data2Dlg();
	else
	{
		m_cobxSite.SetCurSel(3);
		m_editAX.SetWindowText(_T("0"));
		m_editAY.SetWindowText(_T("0"));
		m_editCX.SetWindowText(_T("0"));
		m_editCY.SetWindowText(_T("0"));
		m_editRX.SetWindowText(_T("0"));
		m_editRY.SetWindowText(_T("0"));
	}

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CSeisItemIBC2000::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemIBC2000::IDD, IDC_CMD_IBC_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemIBC2000::DestroyWindow()
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

void CSeisItemIBC2000::OnCmdIbcBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	CSeisIBCCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_hnX = m_dHXIBC2000;
	dlg.m_hnY = m_dHYIBC2000;
	dlg.m_xrdo = m_nMtdXIBC2000;
	dlg.m_yrdo = m_nMtdYIBC2000;
	dlg.m_nNX = m_nNXIBC2000;
	dlg.m_nNY = m_nNYIBC2000;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_Y);
		m_editCX.SetEditUnit(_tstof(cx));
		m_editCY.SetEditUnit(_tstof(cy));

		m_dHXIBC2000 = dlg.m_hnX;
		m_dHYIBC2000 = dlg.m_hnY;
		m_nMtdXIBC2000 = dlg.m_xrdo;
		m_nMtdYIBC2000 = dlg.m_yrdo;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemIBC2000::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
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

	m_nNXIBC2000 = m_nNYIBC2000 = nStorNum - 1;
	m_dHXIBC2000 = m_dHYIBC2000 = dHn;
	m_nMtdXIBC2000 = 0;
	m_nMtdYIBC2000 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemIBC2000::InitCategoryCombo()
{
	CString aItem[6] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F")};
	m_cobxCategory.ResetContent();
	for (int i = 0; i < 6; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CSeisItemIBC2000::InitSiteClassCombo()
{
	CString aItem[5] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};
	m_cobxSite.ResetContent();
	for (int i = 0; i < 5; i++)
		m_cobxSite.AddString(aItem[i]);
	m_cobxSite.SetCurSel(3);
}

void CSeisItemIBC2000::InitShtPeriodCombo()
{
	m_cobxSs.ResetContent();
	for (int i = 0; i < m_aSsName.GetSize(); i++)
		m_cobxSs.AddString(m_aSsName[i]);
	m_cobxSs.SetCurSel(0);
}

void CSeisItemIBC2000::InitSecPeriodCombo()
{
	m_cobxS1.ResetContent();
	for (int i = 0; i < m_aS1Name.GetSize(); i++)
		m_cobxS1.AddString(m_aS1Name[i]);
	m_cobxS1.SetCurSel(0);
}

void CSeisItemIBC2000::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemIBC2000::Data2Dlg()
{
	int i, nCount;
	T_PREFERENCE ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	CString csValue;
	m_cobxCategory.SetCurSel(m_Data.nSeismicDesignCategory);
	m_cobxSite.SetCurSel(m_Data.nSiteClass);

	nCount = m_aSsData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dMSRAs - m_aSsData[i]) <= zero) break;
	if (i != nCount) m_cobxSs.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dMSRAs);
		m_cobxSs.SetWindowText(csValue);
	}

	nCount = m_aS1Data.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dMSRA1 - m_aS1Data[i]) <= zero) break;
	if (i != nCount) m_cobxS1.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dMSRA1);
		m_cobxS1.SetWindowText(csValue);
	}

	nCount = m_aIFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dImportanceFactor - m_aIFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxIFactor.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxIFactor.SetWindowText(csValue);
	}

	m_editAX.SetEditUnit(m_Data.dPeriodCalculated_X);
	m_editAY.SetEditUnit(m_Data.dPeriodCalculated_Y);
	m_editCX.SetEditUnit(m_Data.dPeriodCode_X);
	m_editCY.SetEditUnit(m_Data.dPeriodCode_Y);
	m_editRX.SetEditUnit(m_Data.dResponseModificationFactor_X);
	m_editRY.SetEditUnit(m_Data.dResponseModificationFactor_Y);
}

BOOL CSeisItemIBC2000::Dlg2Data()
{
	CString str;
	CString strError;
	strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));

	m_Data.nSeismicDesignCategory = m_cobxCategory.GetCurSel();
	m_Data.nSiteClass = m_cobxSite.GetCurSel();
	m_cobxS1.GetWindowText(str);
	m_Data.dMSRA1 = _tstof(str);
	m_cobxSs.GetWindowText(str);
	m_Data.dMSRAs = _tstof(str);
	m_cobxIFactor.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);
	CFormulaEdit::GetEditValue(&m_editCX, m_Data.dPeriodCode_X);
	CFormulaEdit::GetEditValue(&m_editCY, m_Data.dPeriodCode_Y);
	CFormulaEdit::GetEditValue(&m_editAX, m_Data.dPeriodCalculated_X);
	CFormulaEdit::GetEditValue(&m_editAY, m_Data.dPeriodCalculated_Y);
	CFormulaEdit::GetEditValue(&m_editRX, m_Data.dResponseModificationFactor_X);
	CFormulaEdit::GetEditValue(&m_editRY, m_Data.dResponseModificationFactor_Y);

	if (m_Data.dPeriodCalculated_X < 0 || m_Data.dPeriodCalculated_Y < 0 ||
		m_Data.dPeriodCode_X < 0 || m_Data.dPeriodCode_Y < 0)
	{
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	if (m_Data.dResponseModificationFactor_X <= 0 || m_Data.dResponseModificationFactor_Y <= 0)
	{
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADDD__Response_Modification_Coeff___));
		return FALSE;
	}

	/*
	CString csValue;
	m_Data.nSeismicDesignCategory = m_cobxCategory.GetCurSel();
	m_Data.nSiteClass = m_cobxSite.GetCurSel();
	m_cobxSs.GetWindowText(csValue);
	m_Data.dMSRAs = _tstof(csValue);

	m_cobxS1.GetWindowText(csValue);
	m_Data.dMSRA1 = _tstof(csValue);

	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);
	
	m_Data.dPeriodCalculated_X = m_editAX.GetEditValue();
	m_Data.dPeriodCalculated_Y = m_editAY.GetEditValue();
	m_Data.dPeriodCode_X = m_editCX.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCY.GetEditValue();
	m_Data.dResponseModificationFactor_X = m_editRX.GetEditValue();
	m_Data.dResponseModificationFactor_Y = m_editRY.GetEditValue();
	*/

	return TRUE;
}
LRESULT CSeisItemIBC2000::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
