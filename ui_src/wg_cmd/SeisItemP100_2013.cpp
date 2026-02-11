// SeisItemP100_2013.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemP100_2013.h"
#include "SeisEuroCalcDlg.h"
#include "SeisItemDlgNew.h"
#include "SeisItemEURO1996.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemP100_2013 dialog


CSeisItemP100_2013::CSeisItemP100_2013(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemP100_2013::IDD, pParent)
{
	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemP100_2013::SetData2Dlg(T_SEIS_P100_2013& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemP100_2013::SetDlg2Data(T_SEIS_P100_2013& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemP100_2013::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioSpecType, m_Data.nSpecType);

	m_wndTb.SetEditUnit(m_Data.dTb);
	m_wndTc.SetEditUnit(m_Data.dTc);
	m_wndTd.SetEditUnit(m_Data.dTd);
	m_wndAg.SetEditUnit(m_Data.dAg);
	m_wndQ.SetEditUnit(m_Data.dQ);
	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);

	CString csValue;
	//   csValue.Format(_T("%g"), m_Data.dImportance);
	// 	m_cmbImportance.SetWindowText(csValue); 
	csValue.Format(_T("%g"), m_Data.dImportance);
	if (CB_ERR == m_cmbImportance.FindStringExact(-1, csValue))
	{
		m_cmbImportance.SetWindowText(csValue);
	}
	else
	{
		m_cmbImportance.SelectString(-1, csValue);
	}
}

BOOL CSeisItemP100_2013::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioSpecType, m_Data.nSpecType);

	m_Data.dTb = m_wndTb.GetEditValue();
	m_Data.dTc = m_wndTc.GetEditValue();
	m_Data.dTd = m_wndTd.GetEditValue();
	m_Data.dAg = m_wndAg.GetEditValue();
	m_Data.dQ = m_wndQ.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();

	CString str;
	m_cmbImportance.GetWindowText(str);
	m_Data.dImportance = _tstof(str);

	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_Y < 0)
	{
		CString strError = _LS(IDS_CMD_HSFC_error); // Error
		strError += _T("! "); // Error!
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}
	return TRUE;
}

void CSeisItemP100_2013::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TD_EDIT, m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TC_EDIT, m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TB_EDIT, m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_BEHAVIOR_EDIT, m_wndQ);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_AG_EDIT, m_wndAg);
	DDX_Radio(pDX, IDC_ETC_GDS_P100_2013_PARATYPE_RDO, m_nParaTypeRdo);
	DDX_Control(pDX, IDC_CMD_P100_2013_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_Y_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_EURO_PERIOD_X_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_EURO_FRAME1, m_grpFold);
}


BEGIN_MESSAGE_MAP(CSeisItemP100_2013, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemP100_2013)
	ON_BN_CLICKED(IDC_CMD_EURO_BTN_CALC_PERIOD, OnCmdEuroBtnCalcPeriod)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO, OnP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO2, OnP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO3, OnP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO4, OnP100_2013ParatypeRdo)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemP100_2013 message handlers

BOOL CSeisItemP100_2013::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	m_aCtrlRadioSpecType.RemoveAll();
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_P100_2013_PARATYPE_RDO);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_P100_2013_PARATYPE_RDO2);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_P100_2013_PARATYPE_RDO3);
	m_aCtrlRadioSpecType.Add(IDC_ETC_GDS_P100_2013_PARATYPE_RDO4);

	InitCtrl();
	Data2Dlg();
	CtrlManager();
	SetFoldGroupCtrls();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemP100_2013::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemP100_2013::IDD, IDC_CMD_EURO_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemP100_2013::DestroyWindow()
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();

	// TODO: Add your message handler code here
}

void CSeisItemP100_2013::InitIFactorCombo()
{
	m_cmbImportance.ResetContent();
	CString aItem[4] = {_T("0.8"), _T("1.0"), _T("1.2"), _T("1.4")};
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(1);
}

void CSeisItemP100_2013::InitDefaultData()
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
	pAttrCtrl->GetStorKeyList(aStorK); //Story Level로 정렬된 리스트
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_dHX = dHn;
	m_dHY = dHn;
	m_dAcX = 0.0;
	m_dAcY = 0.0;
	m_nStoryNumX = 0;
	m_nStoryNumY = 0;
	m_nMtdX = 0;
	m_nMtdY = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemP100_2013::OnCmdEuroBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	CSeisEuroCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	
	// 현재 dialog의 변수를 child dialog의 변수에 넘겨줌		
	dlg.m_dXHedit = m_dHX;
	dlg.m_dYHedit = m_dHY;
	dlg.m_dXAcedit = m_dAcX;
	dlg.m_dYAcedit = m_dAcY;
	dlg.m_dXDedit = m_nStoryNumX;
	dlg.m_dYDedit = m_nStoryNumY;
	dlg.m_nXMethod = m_nMtdX;
	dlg.m_nYMethod = m_nMtdY;
	dlg.SetP100Flag();

	double dTc = m_wndTc.GetEditValue();

	if (dlg.DoModal() == IDOK)
	{
		double dP_x, dP_y;
		dP_x = dlg.m_dPeriod_x;
		dP_y = dlg.m_dPeriod_y;

		// 5. T=1.0n일 때는 자연수여야 함
		if (dlg.m_nXMethod == D_P100_PERIOD_METHOD5) dP_x = static_cast<int>(dP_x);
		if (dlg.m_nYMethod == D_P100_PERIOD_METHOD5) dP_y = static_cast<int>(dP_y);

		CString cx, cy;
		cx.Format(_T("%-10.4g"), dP_x);
		cy.Format(_T("%-10.4g"), dP_y);
		m_editPeriodX.SetEditUnit(_tstof(cx));
		m_editPeriodY.SetEditUnit(_tstof(cy));

		// child dialog의 변수를 현재 dialog의 변수가 받음		
		m_dHX = dlg.m_dXHedit;
		m_dHY = dlg.m_dYHedit;
		m_dAcX = dlg.m_dXAcedit;
		m_dAcY = dlg.m_dYAcedit;
		m_nStoryNumX = dlg.m_dXDedit;
		m_nStoryNumY = dlg.m_dYDedit;
		m_nMtdX = dlg.m_nXMethod;
		m_nMtdY = dlg.m_nYMethod;
	}
}

void CSeisItemP100_2013::CtrlManager()
{
	UpdateData(TRUE);

	BOOL bUserDefined = FALSE;
	BOOL bAutomatic = m_nParaTypeRdo != D_SEIS_P100_2013_USERD;

	// Tb, Tc, Td는 Ground Type이 S1, S2 이거나, Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	m_wndTb.SetReadOnly(bAutomatic);
	m_wndTc.SetReadOnly(bAutomatic);
	m_wndTd.SetReadOnly(bAutomatic);

	if (bAutomatic)
	{
		const int nSpecType = 0; // Seismic load
		double dTb, dTc, dTd;

		CLateralLoad load;
		load.Get_P100_2013_Spectrum_Parameter(nSpecType, m_nParaTypeRdo, dTb, dTc, dTd);
		m_wndTb.SetEditUnit(dTb);
		m_wndTc.SetEditUnit(dTc);
		m_wndTd.SetEditUnit(dTd);
	}

	UpdateData(FALSE);
}

void CSeisItemP100_2013::OnP100_2013ParatypeRdo()
{
	CtrlManager();
}

void CSeisItemP100_2013::InitCtrl()
{
	GetDlgItem(IDC_CMD_P100_2013_IMPORTANCE_STC)->SetWindowText(_LS(IDS_GDS_P100_2013_IMPORTANCE_FACTOR_TEXT));
	// 특수문자가 있어서..
	InitIFactorCombo();
}

LRESULT CSeisItemP100_2013::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
