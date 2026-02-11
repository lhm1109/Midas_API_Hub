// SeisItemChina2001.cpp : implementation file
//
// 2003. 12. 29    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemChina2001.h"
#include "SeisChinaCalcDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "SeisItemDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemChina2001 dialog


CSeisItemChina2001::CSeisItemChina2001(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemChina2001::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisItemChina2001)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlRadio.Add(IDC_CMD_CHINA_ER_RDO1);
	m_aCtrlRadio.Add(IDC_CMD_CHINA_ER_RDO2);

	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;	
}

void CSeisItemChina2001::SetData2Dlg(T_SEIS_CH2002& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemChina2001::SetDlg2Data(T_SEIS_CH2002& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemChina2001::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemChina2001)
	DDX_Control(pDX, IDC_CMD_CHINA_SDC_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_CHINA_SFI_COMBO, m_cobxIntensity);
	DDX_Control(pDX, IDC_CMD_CHINA_SC_COMBO, m_cobxSiteClass);
	DDX_Control(pDX, IDC_CMD_CHINA_DELTAN_COMBO, m_cobxStType);
	DDX_Control(pDX, IDC_CMD_CHINA_XI_EDIT, m_editDampRatio);
	DDX_Control(pDX, IDC_CMD_CHINA_ALPHA1_CHK, m_btnAlpha);
	DDX_Control(pDX, IDC_CMD_CHINA_PERIOD_Y_EDIT, m_editPeriodY);
	DDX_Control(pDX, IDC_CMD_CHINA_PERIOD_X_EDIT, m_editPeriodX);
	DDX_Control(pDX, IDC_CMD_CHINA_FRAME1, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemChina2001, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemChina2001)
	ON_BN_CLICKED(IDC_CMD_CHINA_BTN_CALC_PERIOD, OnCmdChinaBtnCalcPeriod)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemChina2001 message handlers

BOOL CSeisItemChina2001::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_editDampRatio.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodY.SetUnitType(D_UNITSYS_NONE);
	m_editPeriodX.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitCategoryCombo();
	InitIntensityCombo();
	InitSiteClassCombo();
	InitStrtTypeCombo();

	SetFoldGroupCtrls();
	Data2Dlg();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemChina2001::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemChina2001::IDD, IDC_CMD_CHINA_FRAME1, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemChina2001::DestroyWindow()
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

void CSeisItemChina2001::OnCmdChinaBtnCalcPeriod()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nUnitChange = 1; // 주기계산 대화상자에서 wind load시에는 단위변환하고, 
	//                       seismic load시에는 단위변환않음
	CSeisChinaCalcDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT); // child dialog의 위치 지정	

	dlg.m_nUnitChange = nUnitChange;
	dlg.m_dHX = m_dHXCH2001;
	dlg.m_dHY = m_dHYCH2001;
	dlg.m_dBX = m_dBXCH2001;
	dlg.m_dBY = m_dBYCH2001;
	dlg.m_nNX = m_nNXCH2001;
	dlg.m_nNY = m_nNYCH2001;
	dlg.m_nXMethod = m_nMtdXCH2001;
	dlg.m_nYMethod = m_nMtdYCH2001;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriod_X);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriod_Y);
		m_editPeriodX.SetEditUnit(_tstof(cx));
		m_editPeriodY.SetEditUnit(_tstof(cy));

		m_dHXCH2001 = dlg.m_dHX;
		m_dHYCH2001 = dlg.m_dHY;
		m_dBXCH2001 = dlg.m_dBX;
		m_dBYCH2001 = dlg.m_dBY;
		m_nNXCH2001 = dlg.m_nNX;
		m_nNYCH2001 = dlg.m_nNY;
		m_nMtdXCH2001 = dlg.m_nXMethod;
		m_nMtdYCH2001 = dlg.m_nYMethod;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CSeisItemChina2001::InitDefaultData()
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
	pAttrCtrl->GetStorKeyList(aStorK); //Story Level로 정렬된 리스트
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

	m_dBXCH2001 = dBx;
	m_dBYCH2001 = dBy;
	m_dHXCH2001 = m_dHYCH2001 = dHn;
	m_nNXCH2001 = m_nNYCH2001 = nStorNum - 1; // added by KYM
	m_nMtdXCH2001 = 0;
	m_nMtdYCH2001 = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemChina2001::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	m_cobxCategory.AddString(_T("1"));
	m_cobxCategory.AddString(_T("2"));
	m_cobxCategory.AddString(_T("3"));
	m_cobxCategory.SetCurSel(0);
}

void CSeisItemChina2001::InitIntensityCombo()
{
	m_cobxIntensity.ResetContent();
	m_cobxIntensity.AddString(_T("6 (0.05g)"));
	m_cobxIntensity.AddString(_T("7 (0.10g)"));
	m_cobxIntensity.AddString(_T("7 (0.15g)"));
	m_cobxIntensity.AddString(_T("8 (0.20g)"));
	m_cobxIntensity.AddString(_T("8 (0.30g)"));
	m_cobxIntensity.AddString(_T("9 (0.40g)"));
	m_cobxIntensity.SetCurSel(0);
}

void CSeisItemChina2001::InitSiteClassCombo()
{
	m_cobxSiteClass.ResetContent();
	m_cobxSiteClass.AddString(_T("I"));
	m_cobxSiteClass.AddString(_T("II"));
	m_cobxSiteClass.AddString(_T("III"));
	m_cobxSiteClass.AddString(_T("IV"));
	m_cobxSiteClass.SetCurSel(0);
}

void CSeisItemChina2001::InitStrtTypeCombo()
{
	m_cobxStType.ResetContent();
	m_cobxStType.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_1)); // RC or Steel Multistory
	m_cobxStType.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_2)); // Interior Framed Multistory
	m_cobxStType.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_3)); // Tall Steel Building(JGJ99-98)
	m_cobxStType.AddString(_LS(IDS_WG_CMD__ADD2_DELTAN_4)); // Etc
	m_cobxStType.SetCurSel(0);
}

void CSeisItemChina2001::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nSeisDgnCategory);
	m_cobxIntensity.SetCurSel(m_Data.nSeisFortification);
	m_cobxSiteClass.SetCurSel(m_Data.nSiteClass);
	m_cobxStType.SetCurSel(m_Data.nDeltaN);
	m_editDampRatio.SetEditUnit(m_Data.dDamping);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, m_Data.nEarthResponse);
	m_btnAlpha.SetCheck(m_Data.bAlpha1);
	m_editPeriodY.SetEditUnit(m_Data.dPeriod_Y);
	m_editPeriodX.SetEditUnit(m_Data.dPeriod_X);
}

BOOL CSeisItemChina2001::Dlg2Data()
{
	m_Data.nSeisDgnCategory = m_cobxCategory.GetCurSel();
	m_Data.nSeisFortification = m_cobxIntensity.GetCurSel();
	m_Data.nSiteClass = m_cobxSiteClass.GetCurSel();
	m_Data.nDeltaN = m_cobxStType.GetCurSel();
	m_Data.dDamping = m_editDampRatio.GetEditValue();
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nEarthResponse);
	m_Data.bAlpha1 = m_btnAlpha.GetCheck();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();

	if (m_Data.dPeriod_X < 0 || m_Data.dPeriod_Y < 0)
	{
		CString strError = _LS(IDS_CMD_HSFC_error); // Error
		strError += _T("! "); // Error!
		AfxMessageBox(strError + _LS(IDS_WG_CMD__ADD2__Period));
		return FALSE;
	}

	/*
	m_Data.nSeisDgnCategory = m_cobxCategory.GetCurSel();
	m_Data.nSeisFortification = m_cobxIntensity.GetCurSel();
	m_Data.nSiteClass = m_cobxSiteClass.GetCurSel();
	m_Data.nDeltaN = m_cobxStType.GetCurSel();
	m_Data.dDamping = m_editDampRatio.GetEditValue();
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, m_Data.nEarthResponse);
	m_Data.bAlpha1 = m_btnAlpha.GetCheck();
	m_Data.dPeriod_Y = m_editPeriodY.GetEditValue();
	m_Data.dPeriod_X = m_editPeriodX.GetEditValue();
	*/

	return TRUE;
}

LRESULT CSeisItemChina2001::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
