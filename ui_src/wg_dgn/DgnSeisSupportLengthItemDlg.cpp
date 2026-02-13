// DgnSeisSupportLengthItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisSupportLengthItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthItemDlg dialog

CDgnSeisSupportLengthItemDlg::CDgnSeisSupportLengthItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisSupportLengthItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisSupportLengthItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDoc = CDBDoc::GetDocPoint();

	m_Key = 0;
	m_Data.Initialize();
	m_bModify = FALSE;
}


void CDgnSeisSupportLengthItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisSupportLengthItemDlg)

	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_NAME_EDT,      m_edtName);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_ANEV_CMB,      m_cmbAnev);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_H_EDT,         m_edtH);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_H_UNT,         m_untH);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_THETA_EDT,     m_edtTheta);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_THETA_UNT,     m_untTheta);	
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_NC_EDT,        m_edtNc);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_NC_UNT,        m_untNc);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_L_EDT,         m_edtL);
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_L_UNT,         m_untL);	
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_BMP, m_wndPicture);
	
	//}}AFX_DATA_MAP
}

void CDgnSeisSupportLengthItemDlg::InitUnit()
{
	m_edtH.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dH);
	m_untH.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dH);
	m_edtTheta.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dTheta);
	m_untTheta.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dTheta);
	m_edtNc.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dNc);
	m_untNc.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dNc);
	m_edtL.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dL);
	m_untL.SetUnitType(CUnitCtrl::m_SUPL_UNIT.dL);
}

void CDgnSeisSupportLengthItemDlg::InitCombo()
{
	CArray<T_ANEV_K, T_ANEV_K> aAnevK;
	m_pDoc->m_pAttrCtrl2->GetAnevKeyList(aAnevK);
	
	T_ANEV_D AnevD; AnevD.Initialize();

	m_cmbAnev.ResetContent();
	for(int i=0; i<aAnevK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetAnev(aAnevK[i], AnevD)) continue;

		m_cmbAnev.SetItemData(m_cmbAnev.AddString(AnevD.strName), aAnevK[i]);
	}

	m_cmbAnev.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbAnev);
}

void CDgnSeisSupportLengthItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	
	CArray<T_ANEV_K, T_ANEV_K> aAnevK;
	m_pDoc->m_pAttrCtrl2->GetAnevKeyList(aAnevK);

	for(int i=0; i<aAnevK.GetSize(); i++)
	{
		if(aAnevK[i] == m_Data.AnevK) 
		{ 
			m_cmbAnev.SetCurSel(i); 
			break; 
		}
	}

	m_edtH.SetEditUnit(m_Data.dH);
	m_edtTheta.SetEditUnit(m_Data.dTheta);
	m_edtNc.SetEditUnit(m_Data.dNc);
	m_edtL.SetEditUnit(m_Data.dL);
	
	UpdateData(FALSE);
}

BOOL CDgnSeisSupportLengthItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();

	m_edtName.GetWindowText(m_Data.strName);
	m_Data.AnevK = m_cmbAnev.GetItemData(m_cmbAnev.GetCurSel());
	m_Data.dH = m_edtH.GetEditValue();
	m_Data.dTheta = m_edtTheta.GetEditValue();
	m_Data.dNc = m_edtNc.GetEditValue();
	m_Data.dL = m_edtL.GetEditValue();
	
	return TRUE;
}

void CDgnSeisSupportLengthItemDlg::UpdateName()
{
	CString strName;
	
	m_cmbAnev.GetWindowText(strName);

	m_edtName.SetWindowText(strName);

	UpdateData(FALSE);
}

void CDgnSeisSupportLengthItemDlg::CalcH()
{
	T_ANEV_K AnevK;
	AnevK = m_cmbAnev.GetItemData(m_cmbAnev.GetCurSel());

	double dH = m_pDoc->m_pAttrCtrl2->GetH(AnevK);

	m_edtH.SetEditUnit(dH);

	UpdateData(FALSE);
}

void CDgnSeisSupportLengthItemDlg::CalcTheta()
{
	T_ANEV_K AnevK;
	AnevK = m_cmbAnev.GetItemData(m_cmbAnev.GetCurSel());

	T_ANEV_D AnevD; AnevD.Initialize();

	if(!m_pDoc->m_pAttrCtrl2->GetAnev(AnevK, AnevD)) return;

	double dTheta = AnevD.dTheta_q1;

	m_edtTheta.SetEditUnit(dTheta);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthItemDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisSupportLengthItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisSupportLengthItemDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_SUPPORT_LENGTH_ANEV_CMB,      OnSeisSupportLengthAnevCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_OK_BTN,           OnSeisSupportLengthOKBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_CANCEL_BTN,       OnSeisSupportLengthCancelBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_APPLY_BTN,        OnSeisSupportLengthApplyBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthItemDlg message handlers

BOOL CDgnSeisSupportLengthItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	
	Data2Dlg();

	if(!m_bModify)
	{
		UpdateName();
		CalcH();
		CalcTheta();
	}

	if(m_bModify) GetDlgItem(IDC_DGN_SEIS_SUPPORT_LENGTH_APPLY_BTN)->EnableWindow(FALSE);
	
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\Dgn_Seis_Support_Length.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisSupportLengthItemDlg::OnSeisSupportLengthAnevCmb()
{
	UpdateName();
	CalcH();
	CalcTheta();
}

void CDgnSeisSupportLengthItemDlg::OnSeisSupportLengthOKBtn()
{
	if(!Dlg2Data()) return;

	if(m_bModify)
	{
		if(!m_pDoc->m_pDataCtrl->ModifySupl(m_Key, m_Data)) return;
	}
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddSupl(m_Data)) return;
	}

	CDialogMove::OnOK();	
}

void CDgnSeisSupportLengthItemDlg::OnSeisSupportLengthCancelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisSupportLengthItemDlg::OnSeisSupportLengthApplyBtn()
{
	if(!Dlg2Data()) return;

	if(!m_pDoc->m_pDataCtrl->AddSupl(m_Data)) return;
}