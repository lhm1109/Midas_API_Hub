// SeisItemNBC1995.cpp : implementation file
//
// 2003. 12. 30   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemNBC1995.h"

#include "SeisNBC95CalcDlg.h"
#include "SeisItemDlgNew.h"

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
// CSeisItemNBC1995 dialog


CSeisItemNBC1995::CSeisItemNBC1995(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemNBC1995::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aVRatioName.RemoveAll();
	m_aVRatioData.RemoveAll();
	m_aVRatioName.Add(_T("0.00"));
	m_aVRatioData.Add(0.00);
	m_aVRatioName.Add(_T("0.05"));
	m_aVRatioData.Add(0.05);
	m_aVRatioName.Add(_T("0.10"));
	m_aVRatioData.Add(0.10);
	m_aVRatioName.Add(_T("0.15"));
	m_aVRatioData.Add(0.15);
	m_aVRatioName.Add(_T("0.20"));
	m_aVRatioData.Add(0.20);
	m_aVRatioName.Add(_T("0.30"));
	m_aVRatioData.Add(0.30);
	m_aVRatioName.Add(_T("0.40"));
	m_aVRatioData.Add(0.40);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.3"));
	m_aIFactorData.Add(1.3);
	m_aIFactorName.Add(_T("1.5"));
	m_aIFactorData.Add(1.5);

	m_aFFactorName.RemoveAll();
	m_aFFactorData.RemoveAll();
	m_aFFactorName.Add(_T("1.0"));
	m_aFFactorData.Add(1.0);
	m_aFFactorName.Add(_T("1.3"));
	m_aFFactorData.Add(1.3);
	m_aFFactorName.Add(_T("1.5"));
	m_aFFactorData.Add(1.5);
	m_aFFactorName.Add(_T("2.0"));
	m_aFFactorData.Add(2.0);

	m_aRFactorName.RemoveAll();
	m_aRFactorData.RemoveAll();
	m_aRFactorName.Add(_T("1.0"));
	m_aRFactorData.Add(1.0);
	m_aRFactorName.Add(_T("1.5"));
	m_aRFactorData.Add(1.5);
	m_aRFactorName.Add(_T("2.0"));
	m_aRFactorData.Add(2.0);
	m_aRFactorName.Add(_T("3.0"));
	m_aRFactorData.Add(3.0);
	m_aRFactorName.Add(_T("3.5"));
	m_aRFactorData.Add(3.5);
	m_aRFactorName.Add(_T("4.0"));
	m_aRFactorData.Add(4.0);

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CSeisItemNBC1995::SetData2Dlg(T_SEIS_NBC1995& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemNBC1995::SetDlg2Data(T_SEIS_NBC1995& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemNBC1995::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_NBC95_ZONE_RATIO_CMB, m_cobxVRatio);
	DDX_Control(pDX, IDC_CMD_NBC95_ACCE_ZONE_CMB, m_cobxAZone);
	DDX_Control(pDX, IDC_CMD_NBC95_VELO_ZONE_CMB, m_cobxVZone);
	DDX_Control(pDX, IDC_CMD_NBC95_IMPORTANCE_CMB, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_NBC95_FOUNDATION_CMB, m_cobxFFactor);
	DDX_Control(pDX, IDC_CMD_NBC95_TX_ANAL_EDIT, m_editAX);
	DDX_Control(pDX, IDC_CMD_NBC95_TY_ANAL_EDIT, m_editAY);
	DDX_Control(pDX, IDC_CMD_NBC95_TX_CODE_EDIT, m_editCX);
	DDX_Control(pDX, IDC_CMD_NBC95_TY_CODE_EDIT, m_editCY);
	DDX_Control(pDX, IDC_CMD_NBC95_RX_CMB, m_cobxRX);
	DDX_Control(pDX, IDC_CMD_NBC95_RY_CMB, m_cobxRY);
	DDX_Control(pDX, IDC_CMD_NBC95_FRAME1, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemNBC1995, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_NBC95_T_CALC_BTN, OnCmdNbc95TCalcBtn)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNBC1995 message handlers

BOOL CSeisItemNBC1995::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editAX.SetUnitType(D_UNITSYS_NONE);
	m_editAY.SetUnitType(D_UNITSYS_NONE);
	m_editCX.SetUnitType(D_UNITSYS_NONE);
	m_editCY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitVRatioCombo();
	InitAZoneCombo();
	InitVZoneCombo();
	InitIFactorCombo();
	InitFFactorCombo();
	InitRxRyCombo();
	SetFoldGroupCtrls();

	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemNBC1995::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemNBC1995::IDD, IDC_CMD_NBC95_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemNBC1995::DestroyWindow()
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

void CSeisItemNBC1995::OnCmdNbc95TCalcBtn()
{
	// TODO: Add your control notification handler code here	
	CSeisNBC95CalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_dHX = m_dHXNBC1995;
	dlg.m_dHY = m_dHYNBC1995;
	dlg.m_dBX = m_dBXNBC1995;
	dlg.m_dBY = m_dBYNBC1995;
	dlg.m_nNX = m_nNXNBC1995;
	dlg.m_nNY = m_nNYNBC1995;
	dlg.m_nXMethod = m_nMtdXNBC1995;
	dlg.m_nYMethod = m_nMtdYNBC1995;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_Y);
		m_editCX.SetEditUnit(_tstof(cx));
		m_editCY.SetEditUnit(_tstof(cy));

		m_dHXNBC1995 = dlg.m_dHX;
		m_dHYNBC1995 = dlg.m_dHY;
		m_dBXNBC1995 = dlg.m_dBX;
		m_dBYNBC1995 = dlg.m_dBY;
		m_nNXNBC1995 = dlg.m_nNX;
		m_nNYNBC1995 = dlg.m_nNY;
		m_nMtdXNBC1995 = dlg.m_nXMethod;
		m_nMtdYNBC1995 = dlg.m_nYMethod;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemNBC1995::InitDefaultData()
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

	m_dHXNBC1995 = m_dHYNBC1995 = dHn;
	m_dBXNBC1995 = dBx;
	m_dBYNBC1995 = dBy;
	m_nNXNBC1995 = 0;
	m_nNYNBC1995 = 0;
	m_nMtdXNBC1995 = 0;
	m_nMtdYNBC1995 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemNBC1995::InitVRatioCombo()
{
	m_cobxVRatio.ResetContent();
	for (int i = 0; i < m_aVRatioName.GetSize(); i++)
		m_cobxVRatio.AddString(m_aVRatioName[i]);
	m_cobxVRatio.SetCurSel(0);
}

void CSeisItemNBC1995::InitAZoneCombo()
{
	CString csValue;
	m_cobxAZone.ResetContent();
	for (int i = 0; i <= 6; i++)
	{
		csValue.Format(_T("%d"), i);
		m_cobxAZone.AddString(csValue);
	}
	m_cobxAZone.SetCurSel(0);
}

void CSeisItemNBC1995::InitVZoneCombo()
{
	CString csValue;
	m_cobxVZone.ResetContent();
	for (int i = 0; i <= 6; i++)
	{
		csValue.Format(_T("%d"), i);
		m_cobxVZone.AddString(csValue);
	}
	m_cobxVZone.SetCurSel(0);
}

void CSeisItemNBC1995::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemNBC1995::InitFFactorCombo()
{
	m_cobxFFactor.ResetContent();
	for (int i = 0; i < m_aFFactorName.GetSize(); i++)
		m_cobxFFactor.AddString(m_aFFactorName[i]);
	m_cobxFFactor.SetCurSel(0);
}

void CSeisItemNBC1995::InitRxRyCombo()
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

void CSeisItemNBC1995::Data2Dlg()
{
	int i, nCount;
	CString csValue;
	T_PREFERENCE ref;
	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(ref);
	double zero = ref.Data.dblRegardZero;

	nCount = m_aVRatioData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dZonalVelocityRatio - m_aVRatioData[i]) <= zero) break;
	if (i != nCount) m_cobxVRatio.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dZonalVelocityRatio);
		m_cobxVRatio.SetWindowText(csValue);
	}

	m_cobxAZone.SetCurSel(m_Data.nAccelerationZone);
	m_cobxVZone.SetCurSel(m_Data.nVelocityZone);

	nCount = m_aIFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dImportanceFactor - m_aIFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxIFactor.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxIFactor.SetWindowText(csValue);
	}

	nCount = m_aFFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dFoundationFactor - m_aFFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxFFactor.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dFoundationFactor);
		m_cobxFFactor.SetWindowText(csValue);
	}

	m_editAX.SetEditUnit(m_Data.dPeriodAnalysis_X);
	m_editAY.SetEditUnit(m_Data.dPeriodAnalysis_Y);
	m_editCX.SetEditUnit(m_Data.dPeriodCode_X);
	m_editCY.SetEditUnit(m_Data.dPeriodCode_Y);

	nCount = m_aRFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dForceModificationFactor_X - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRX.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dForceModificationFactor_X);
		m_cobxRX.SetWindowText(csValue);
	}

	nCount = m_aRFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dForceModificationFactor_Y - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRY.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dForceModificationFactor_Y);
		m_cobxRY.SetWindowText(csValue);
	}
}

BOOL CSeisItemNBC1995::Dlg2Data()
{
	CString str;

	m_cobxVRatio.GetWindowText(str);
	m_Data.dZonalVelocityRatio = _tstof(str);
	m_cobxIFactor.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);
	m_cobxFFactor.GetWindowText(str);
	m_Data.dFoundationFactor = _tstof(str);
	m_Data.nAccelerationZone = m_cobxAZone.GetCurSel();
	m_Data.nVelocityZone = m_cobxVZone.GetCurSel();
	m_Data.dPeriodAnalysis_X = m_editAX.GetEditValue();
	m_Data.dPeriodAnalysis_Y = m_editAY.GetEditValue();
	m_Data.dPeriodCode_X = m_editCX.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCY.GetEditValue();
	m_cobxRX.GetWindowText(str);
	m_Data.dForceModificationFactor_X = _tstof(str);
	m_cobxRY.GetWindowText(str);
	m_Data.dForceModificationFactor_Y = _tstof(str);

	if (m_Data.dPeriodAnalysis_X < 0 || m_Data.dPeriodAnalysis_Y < 0 ||
		m_Data.dPeriodCode_X < 0 || m_Data.dPeriodCode_Y < 0)
	{
		CString strError;
		strError.Format(_T("%s! "), _LS(IDS_CMD_HSFC_error));
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	if (m_Data.dForceModificationFactor_X <= 0 || m_Data.dForceModificationFactor_Y <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Modification_Fact));
		return FALSE;
	}

	/*
	CString csValue;

	m_Data.dPeriodAnalysis_X = m_editAX.GetEditValue();
	m_Data.dPeriodAnalysis_Y = m_editAY.GetEditValue();
	m_Data.dPeriodCode_X = m_editCX.GetEditValue();
	m_Data.dPeriodCode_Y = m_editCY.GetEditValue();
	m_cobxRX.GetWindowText(csValue);
	m_Data.dForceModificationFactor_X = _tstof(csValue);
	m_cobxRY.GetWindowText(csValue);
	m_Data.dForceModificationFactor_Y = _tstof(csValue);
	*/

	return TRUE;
}

LRESULT CSeisItemNBC1995::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
