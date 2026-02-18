// CDgnSeisAnchorEvalItemNewSubArrangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemNewSubArrangeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubArrangeDlg dialog

CDgnSeisAnchorEvalItemNewSubArrangeDlg::CDgnSeisAnchorEvalItemNewSubArrangeDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItemNewSubArrangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemNewSubArrangeDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItemNewDlg*)pParent;

	m_strSax = _T("");
	m_strSay = _T("");

	m_bRelAngle = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemNewSubArrangeDlg)

	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_EDT, m_edtB);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_UNT, m_untB);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_EDT, m_edtH);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_UNT, m_untH);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT, m_edtRelAngle); 
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_UNT, m_untRelAngle);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT, m_edtTheta1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_UNT, m_untTheta1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT, m_edtTheta2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_UNT, m_untTheta2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_EDT, m_edtThetaq1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_UNT, m_untThetaq1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_EDT, m_edtThetaq2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_UNT, m_untThetaq2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_EDT, m_edtD1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_UNT, m_untD1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_EDT, m_edtD1q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_UNT, m_untD1q);
	DDX_Text(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAX_EDT, m_strSax);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAX_UNT, m_untSax);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_EDT, m_edtD2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_UNT, m_untD2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_EDT, m_edtD2q);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_UNT, m_untD2q);
	DDX_Text(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAY_EDT, m_strSay);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_SAY_UNT, m_untSay);

	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_CHK, m_bRelAngle);

	//}}AFX_DATA_MAP
}


void CDgnSeisAnchorEvalItemNewSubArrangeDlg::SetArrangeData(T_ANEV_D* pData)
{
	m_pData = pData;
	//CalRelAngle();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::EnableCtrl()
{
	if (m_bRelAngle == TRUE)  // User
	{
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT)->EnableWindow(FALSE);
	}
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::CalRelAngle()
{
	if (!m_bRelAngle) 
	{
		double dAlpha = m_pDoc->m_pAttrCtrl2->GetAlpha(m_pData->BrevK);
		m_edtRelAngle.SetEditUnit(dAlpha);
	}
	
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::InitUnit()
{
	m_edtB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRelAngle.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dRelAngle);
	m_untRelAngle.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dRelAngle);
	m_edtTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
	m_untTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
	m_edtTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
	m_untTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
	m_edtThetaq1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q1);
	m_untThetaq1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q1);
	m_edtThetaq2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q2);
	m_untThetaq2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta_q2);
	m_edtD1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1);
	m_untD1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1);	
	m_edtD1q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1_q);
	m_untD1q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD1_q);
	m_untSax.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dSax);	
	m_edtD2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2);
	m_untD2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2);	
	m_edtD2q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2_q);
	m_untD2q.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dD2_q);
	m_untSay.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dSay);
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::Data2Dlg() 
{ 
	m_bRelAngle = m_pData->bRelAngle;
	m_edtRelAngle.SetEditUnit(m_pData->dRelAngle);
	m_edtTheta1.SetEditUnit(m_pData->dTheta1);
	m_edtTheta2.SetEditUnit(m_pData->dTheta2);
	m_edtThetaq1.SetEditUnit(m_pData->dTheta_q1);
	m_edtThetaq2.SetEditUnit(m_pData->dTheta_q2);
	m_edtD1.SetEditUnit(m_pData->dD1);
	m_edtD1q.SetEditUnit(m_pData->dD1_q);
	m_strSax = CStrParser::ConvtDistArr2DistStr(m_pData->aSax);
	m_edtD2.SetEditUnit(m_pData->dD2);
	m_edtD2q.SetEditUnit(m_pData->dD2_q);
	m_strSay = CStrParser::ConvtDistArr2DistStr(m_pData->aSay);

	UpdateData(FALSE);

	CalRelAngle();
}

BOOL CDgnSeisAnchorEvalItemNewSubArrangeDlg::Dlg2Data(BOOL bIgnoreUpdateData/* = FALSE*/)
{
	if(!bIgnoreUpdateData)
	{
		UpdateData(TRUE);
	}

	m_pData->bRelAngle = m_bRelAngle;
	m_pData->dRelAngle = m_edtRelAngle.GetEditValue();
	m_pData->dTheta1 = m_edtTheta1.GetEditValue();
	m_pData->dTheta2 = m_edtTheta2.GetEditValue();
	m_pData->dTheta_q1 = m_edtThetaq1.GetEditValue();
	m_pData->dTheta_q2 = m_edtThetaq2.GetEditValue();
	m_pData->dD1 = m_edtD1.GetEditValue();
	m_pData->dD1_q = m_edtD1q.GetEditValue();
	m_pData->dD2 = m_edtD2.GetEditValue();
	m_pData->dD2_q = m_edtD2q.GetEditValue();

	BOOL bAllZero = TRUE;

	CArray<double, double> aSax;
	CStrParser::GetUEDistance(m_strSax, aSax);

	for (int i = 0; i < aSax.GetSize(); i++)
	{
		if (aSax[i] > 0.0)
		{
			bAllZero = FALSE;
		}
	}

	if (bAllZero)
	{
		if (aSax.GetSize() > 0) aSax.RemoveAll();
	}

	m_pData->aSax.RemoveAll();
	m_pData->aSax.Copy(aSax);

	bAllZero = TRUE;

	CArray<double, double> aSay;
	CStrParser::GetUEDistance(m_strSay, aSay);

	for (int i = 0; i < aSay.GetSize(); i++)
	{
		if (aSay[i] > 0.0)
		{
			bAllZero = FALSE;
		}
	}

	if (bAllZero)
	{
		if (aSay.GetSize() > 0) aSay.RemoveAll();
	}

	m_pData->aSay.RemoveAll();
	m_pData->aSay.Copy(aSay);

	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevSupportNew(m_pData->BrevK, aSax, aSay, m_pData->SuppType, SuppType);
	m_pData->SuppType.Initialize();
	m_pData->SuppType = SuppType;

	T_ANEV_BRPR_D BearingProp; BearingProp.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevProperty(aSax, aSay, m_pData->BearingProp, BearingProp);
	m_pData->BearingProp.Initialize();
	m_pData->BearingProp = BearingProp;

	if(m_pData->bTensChk)
	{
		T_ANEV_TENS_CHK_D TensChk; TensChk.Initialize();
		m_pDoc->m_pAttrCtrl2->SetAnevTension(aSax, aSay, m_pData->BrevK, m_pData->TensChk, TensChk);
		m_pData->TensChk.Initialize();
		m_pData->TensChk = TensChk;
	}

	return TRUE;
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::CalcB()
{
	UpdateData(TRUE);

	double dD1 = m_edtD1.GetEditValue();
	double dD1_q = m_edtD1q.GetEditValue();

	CArray<double, double> aSax;
	CStrParser::GetUEDistance(m_strSax, aSax);
	if (m_strSax.IsEmpty()) aSax.RemoveAll();

	double dB = dD1 + dD1_q;
	for (int i = 0; i < aSax.GetSize(); i++)
	{
		dB += aSax[i];
	}

	m_edtB.SetEditUnit(dB);

	UpdateData(FALSE);
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::CalcH()
{
	UpdateData(TRUE);

	double dD2 = m_edtD2.GetEditValue();
	double dD2_q = m_edtD2q.GetEditValue();

	CArray<double, double> aSay;
	CStrParser::GetUEDistance(m_strSay, aSay);
	if (m_strSay.IsEmpty()) aSay.RemoveAll();

	double dH = dD2 + dD2_q;
	for (int i = 0; i < aSay.GetSize(); i++)
	{
		dH += aSay[i];
	}

	m_edtH.SetEditUnit(dH);

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemNewSubArrangeDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)

	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT, OnSeisAnchorEvalPropRelAngle)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT, OnSeisAnchorEvalPropTheta1Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT, OnSeisAnchorEvalPropTheta2Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1Q_EDT, OnSeisAnchorEvalPropTheta1QEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2Q_EDT, OnSeisAnchorEvalPropTheta2QEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D1_EDT, OnSeisAnchorEvalPropD1Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D1_Q_EDT, OnSeisAnchorEvalPropD1_qEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_SAX_EDT, OnSeisAnchorEvalPropSaxEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D2_EDT, OnSeisAnchorEvalPropD2Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_D2_Q_EDT, OnSeisAnchorEvalPropD2_qEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_SAY_EDT, OnSeisAnchorEvalPropSayEdt)

	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_CHK, OnSeisAnchorEvalRelAngleChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubArrangeDlg message handlers

BOOL CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitUnit();

	Data2Dlg();
	EnableCtrl();

	CalcB();
 	CalcH();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropRelAngle()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropTheta1Edt()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropTheta2Edt()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropTheta1QEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropTheta2QEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropD1Edt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropD1_qEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropSaxEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
	m_pParent->CalcN1();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropD2Edt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropD2_qEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalPropSayEdt()
{
	Dlg2Data();
	m_pParent->SetViewData();

	CalcB();
	CalcH();
	m_pParent->CalcN2();
}

void CDgnSeisAnchorEvalItemNewSubArrangeDlg::OnSeisAnchorEvalRelAngleChk()
{
	UpdateData(TRUE);
    
	EnableCtrl();
	CalRelAngle();
}