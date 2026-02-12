// SeisItemTaiwan1999.cpp : implementation file
//
// 2003. 12. 30    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemTaiwan1999.h"
#include "SeisItemDlgNew.h"
#include "SeisTaiwan86PeriodDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemTaiwan1999 dialog


CSeisItemTaiwan1999::CSeisItemTaiwan1999(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemTaiwan1999::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;

	m_aCtrlRadio.Add(IDC_CMD_PERIOD_RDO);
	m_aCtrlRadio.Add(IDC_CMD_PERIOD_RDO2);

	m_aIFactorName.RemoveAll();
	m_aIFactorData.RemoveAll();
	m_aIFactorName.Add(_T("1.0"));
	m_aIFactorData.Add(1.0);
	m_aIFactorName.Add(_T("1.25"));
	m_aIFactorData.Add(1.25);
	m_aIFactorName.Add(_T("1.5"));
	m_aIFactorData.Add(1.5);

	m_aAFactorName.RemoveAll();
	m_aAFactorData.RemoveAll();
	m_aAFactorName.Add(_T("1.2"));
	m_aAFactorData.Add(1.2);
	m_aAFactorName.Add(_T("1.5"));
	m_aAFactorData.Add(1.5);

	m_aRFactorName.RemoveAll();
	m_aRFactorData.RemoveAll();
	m_aRFactorName.Add(_T("1.6"));
	m_aRFactorData.Add(1.6);
	m_aRFactorName.Add(_T("2.4"));
	m_aRFactorData.Add(2.4);
	m_aRFactorName.Add(_T("2.8"));
	m_aRFactorData.Add(2.8);
	m_aRFactorName.Add(_T("3.2"));
	m_aRFactorData.Add(3.2);
	m_aRFactorName.Add(_T("3.6"));
	m_aRFactorData.Add(3.6);
	m_aRFactorName.Add(_T("4.0"));
	m_aRFactorData.Add(4.0);
	m_aRFactorName.Add(_T("4.4"));
	m_aRFactorData.Add(4.4);
	m_aRFactorName.Add(_T("4.8"));
	m_aRFactorData.Add(4.8);

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CSeisItemTaiwan1999::SetData2Dlg(T_SEIS_TAIWAN86& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemTaiwan1999::SetDlg2Data(T_SEIS_TAIWAN86& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemTaiwan1999::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_ZONE_CMB, m_cobxZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_SOIL_CMB, m_cobxSoilType);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_IMPORTANCE_CMB, m_cobxIFactor);
	DDX_Control(pDX, IDC_CMD_TAIWAN86_ALPHA_CMB, m_cobxAlpha);
	DDX_Control(pDX, IDC_CMD_TX_ANAL_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_TY_ANAL_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT, m_editApprX);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT, m_editApprY);
	DDX_Control(pDX, IDC_CMD_TX_FINAL_EDIT, m_editFinalX);
	DDX_Control(pDX, IDC_CMD_TY_FINAL_EDIT, m_editFinalY);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cobxRX);
	DDX_Control(pDX, IDC_CMD_RY_CMB, m_cobxRY);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemTaiwan1999, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemTaiwan1999)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnCmdTaiwan86PeriodBtn)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO, OnCmdTaiwan86PeriodRdo)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO2, OnCmdTaiwan86PeriodRdo)
	ON_EN_CHANGE(IDC_CMD_TX_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TX_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemTaiwan1999 message handlers

BOOL CSeisItemTaiwan1999::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editAnalX.SetUnitType(D_UNITSYS_NONE);
	m_editAnalY.SetUnitType(D_UNITSYS_NONE);
	m_editApprX.SetUnitType(D_UNITSYS_NONE);
	m_editApprY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetUnitType(D_UNITSYS_NONE);
	m_editFinalY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetReadOnly(TRUE);
	m_editFinalY.SetReadOnly(TRUE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitZoneCombo();
	InitSoilCombo();
	InitIFactorCombo();
	InitAlphaCombo();
	InitRxRyCombo();
	SetFoldGroupCtrls();

	if (m_bModify) Data2Dlg();
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, 0);
		m_editAnalX.SetWindowText(_T("0"));
		m_editAnalY.SetWindowText(_T("0"));
		m_editApprX.SetWindowText(_T("0"));
		m_editApprY.SetWindowText(_T("0"));
		m_editFinalX.SetWindowText(_T("0"));
		m_editFinalY.SetWindowText(_T("0"));
	}

	OnCmdTaiwan86PeriodRdo();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemTaiwan1999::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemTaiwan1999::IDD, IDC_WG_CMD_STATIC0, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemTaiwan1999::DestroyWindow()
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

void CSeisItemTaiwan1999::OnCmdTaiwan86PeriodBtn()
{
	// TODO: Add your control notification handler code here
	CSeisTaiwan86PeriodDlg dlg(EN_DLG_MODE_TAIWAN_1999, false);
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nMethodX = m_nMtdXTaiwan86;
	dlg.m_nMethodY = m_nMtdYTaiwan86;
	dlg.m_dHnX = m_dHnXTaiwan86;
	dlg.m_dHnY = m_dHnYTaiwan86;
	dlg.m_dAcX = m_dAcXTaiwan86;
	dlg.m_dAcY = m_dAcYTaiwan86;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriodX);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriodY);
		m_editApprX.SetEditUnit(_tstof(cx));
		m_editApprY.SetEditUnit(_tstof(cy));

		m_nMtdXTaiwan86 = dlg.m_nMethodX;
		m_nMtdYTaiwan86 = dlg.m_nMethodY;
		m_dHnXTaiwan86 = dlg.m_dHnX;
		m_dHnYTaiwan86 = dlg.m_dHnY;
		m_dAcXTaiwan86 = dlg.m_dAcX;
		m_dAcYTaiwan86 = dlg.m_dAcY;
	}
}

void CSeisItemTaiwan1999::OnCmdTaiwan86PeriodRdo()
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	m_editAnalX.EnableWindow(nCheck == 0);
	m_editAnalY.EnableWindow(nCheck == 0);
	OnChangeCmdPeriodEdit();
}

void CSeisItemTaiwan1999::OnChangeCmdPeriodEdit()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	double dTXAnal = m_editAnalX.GetEditValue();
	double dTXAppr = m_editApprX.GetEditValue();
	double dTYAnal = m_editAnalY.GetEditValue();
	double dTYAppr = m_editApprY.GetEditValue();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	int nCodeType = 12;
	double dTXFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, nCheck, dTXAnal, dTXAppr);
	double dTYFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, nCheck, dTYAnal, dTYAppr);

	CString str;
	str.Format(_T("%-10.4g"), dTXFinal);
	m_editFinalX.SetWindowText(str);
	str.Format(_T("%-10.4g"), dTYFinal);
	m_editFinalY.SetWindowText(str);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemTaiwan1999::InitDefaultData()
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

	m_dHnXTaiwan86 = m_dHnYTaiwan86 = dHn;
	m_dAcXTaiwan86 = m_dAcYTaiwan86 = 0.0;
	m_nMtdXTaiwan86 = m_nMtdYTaiwan86 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemTaiwan1999::InitZoneCombo()
{
	CString aItem[2] = {_T("I (0.33g)"), _T("II (0.23g)")};

	m_cobxZone.ResetContent();
	for (int i = 0; i < 2; i++)
		m_cobxZone.AddString(aItem[i]);
	m_cobxZone.SetCurSel(0);
}

void CSeisItemTaiwan1999::InitSoilCombo()
{
	CString aItem[4] = {_T("I (Stiff Soil)"),_T("II (Medium Soil)"),_T("III (Soft Soil)"),_T("Taipei Basin")};

	m_cobxSoilType.ResetContent();
	for (int i = 0; i < 4; i++)
		m_cobxSoilType.AddString(aItem[i]);
	m_cobxSoilType.SetCurSel(0);
}

void CSeisItemTaiwan1999::InitIFactorCombo()
{
	m_cobxIFactor.ResetContent();
	for (int i = 0; i < m_aIFactorName.GetSize(); i++)
		m_cobxIFactor.AddString(m_aIFactorName[i]);
	m_cobxIFactor.SetCurSel(0);
}

void CSeisItemTaiwan1999::InitAlphaCombo()
{
	m_cobxAlpha.ResetContent();
	for (int i = 0; i < m_aAFactorName.GetSize(); i++)
		m_cobxAlpha.AddString(m_aAFactorName[i]);
	m_cobxAlpha.SetCurSel(0);
}

void CSeisItemTaiwan1999::InitRxRyCombo()
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

void CSeisItemTaiwan1999::Data2Dlg()
{
	int nCount, i;
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

	nCount = m_aAFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dSeisMagnifyFactor - m_aAFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxAlpha.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dSeisMagnifyFactor);
		m_cobxAlpha.SetWindowText(csValue);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, m_Data.nPeriodMethod);

	m_editAnalX.SetEditUnit(m_Data.dPeriodAnalX);
	m_editAnalY.SetEditUnit(m_Data.dPeriodAnalY);
	m_editApprX.SetEditUnit(m_Data.dPeriodApprX);
	m_editApprY.SetEditUnit(m_Data.dPeriodApprY);
	m_editFinalX.SetEditUnit(m_Data.dPeriodFinalX);
	m_editFinalY.SetEditUnit(m_Data.dPeriodFinalY);

	nCount = m_aRFactorData.GetSize();
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dResponseModifyFactorX - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRX.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseModifyFactorX);
		m_cobxRX.SetWindowText(csValue);
	}
	for (i = 0; i < nCount; i++)
		if (fabs(m_Data.dResponseModifyFactorY - m_aRFactorData[i]) <= zero) break;
	if (i != nCount) m_cobxRY.SetCurSel(i);
	else
	{
		csValue.Format(_T("%g"), m_Data.dResponseModifyFactorY);
		m_cobxRY.SetWindowText(csValue);
	}
}

BOOL CSeisItemTaiwan1999::Dlg2Data()
{
	CString str;

	m_Data.nSeismicZone = m_cobxZone.GetCurSel();
	m_Data.nSoilType = m_cobxSoilType.GetCurSel();
	m_cobxIFactor.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);
	m_cobxAlpha.GetWindowText(str);
	m_Data.dSeisMagnifyFactor = _tstof(str);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nPeriodMethod);

	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	//  m_Data.dPeriodFinalX = m_editFinalX.GetEditValue();
	//  m_Data.dPeriodFinalY = m_editFinalY.GetEditValue();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCodeType = 12;
	m_Data.dPeriodFinalX = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, m_Data.nPeriodMethod,
	                                                                m_Data.dPeriodAnalX, m_Data.dPeriodApprX);
	m_Data.dPeriodFinalY = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, m_Data.nPeriodMethod,
	                                                                m_Data.dPeriodAnalY, m_Data.dPeriodApprY);
	m_cobxRX.GetWindowText(str);
	m_Data.dResponseModifyFactorX = _tstof(str);
	m_cobxRY.GetWindowText(str);
	m_Data.dResponseModifyFactorY = _tstof(str);

	/*
	CString csValue;
	m_cobxIFactor.GetWindowText(csValue);
	m_Data.dImportanceFactor = _tstof(csValue);
	m_cobxAlpha.GetWindowText(csValue);
	m_Data.dSeisMagnifyFactor = _tstof(csValue);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nPeriodMethod);

	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	m_Data.dPeriodFinalX = m_editFinalX.GetEditValue();
	m_Data.dPeriodFinalY = m_editFinalY.GetEditValue();
	m_cobxRX.GetWindowText(csValue);
	m_Data.dResponseModifyFactorX = _tstof(csValue);
	m_cobxRY.GetWindowText(csValue);
	m_Data.dResponseModifyFactorY = _tstof(csValue);
	*/

	return TRUE;
}

LRESULT CSeisItemTaiwan1999::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
