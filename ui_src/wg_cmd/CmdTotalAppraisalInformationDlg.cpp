#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdTotalAppraisalInformationDlg.h"
#include "ExportFunc.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CmdTotalAppraisalInformationDlg::CmdTotalAppraisalInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CmdTotalAppraisalInformationDlg::IDD, pParent)
{
	m_nBldType = 0;
}

void CmdTotalAppraisalInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CmdTotalAppraisalInformationDlg)
	DDX_Radio(pDX, IDC_CMD_APPRAISAL_TYPE_RADIO1, m_nBldType);
	DDX_Control(pDX, IDC_CMD_APPRAISAL_CODE_COMBO, m_CodeCombox);
	DDX_Control(pDX, IDC_CMD_APPRAISA_A_MIN_EDIT, m_MainAMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_B_MIN_EDIT, m_MainBMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_C_MIN_EDIT, m_MainCMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_B_MAX_EDIT, m_MainBMaxEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_C_MAX_EDIT, m_MainCMaxEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_D_MAX_EDIT, m_MainDMaxEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_A_MIN_EDIT2, m_SecondaryAMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_B_MIN_EDIT2, m_SecondaryBMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_C_MIN_EDIT2, m_SecondaryCMinEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_B_MAX_EDIT2, m_SecondaryBMaxEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_C_MAX_EDIT2, m_SecondaryCMaxEdit);
	DDX_Control(pDX, IDC_CMD_APPRAISA_D_MAX_EDIT2, m_SecondaryDMaxEdit);
	
	//IDC_CMD_STATIC3
}

BEGIN_MESSAGE_MAP(CmdTotalAppraisalInformationDlg, CDialogMove)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_CMD_APPRAISAL_TYPE_RADIO1, OnChangeBuildingType)
	ON_BN_CLICKED(IDC_CMD_APPRAISAL_TYPE_RADIO2, OnChangeBuildingType)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_A_MIN_EDIT, OnUpdateDate_MainB)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_B_MIN_EDIT, OnUpdateDate_MainC)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_C_MIN_EDIT, OnUpdateDate_MainD)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_A_MIN_EDIT2, OnUpdateDate_SecondaryB)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_B_MIN_EDIT2, OnUpdateDate_SecondaryC)
	ON_EN_KILLFOCUS(IDC_CMD_APPRAISA_C_MIN_EDIT2, OnUpdateDate_SecondaryD)
END_MESSAGE_MAP()

BOOL CmdTotalAppraisalInformationDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_pDoc->m_pAttrCtrl->GetTaif(m_Data);

	GetDlgItem(IDC_CMD_APPRAISA_B_MAX_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPRAISA_C_MAX_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPRAISA_D_MAX_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPRAISA_B_MAX_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPRAISA_C_MAX_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_APPRAISA_D_MAX_EDIT2)->EnableWindow(FALSE);

	if (m_Data.nBldType == 1)
	{
		GetDlgItem(IDC_STATIC6)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC28));
		GetDlgItem(IDC_STATIC7)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC29));
		GetDlgItem(IDC_STATIC8)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC81));
		GetDlgItem(IDC_STATIC9)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC91));
		GetDlgItem(IDC_STATIC10)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC101));
		GetDlgItem(IDC_STATIC11)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC111));
	}
	else
	{
		GetDlgItem(IDC_STATIC6)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC6));
		GetDlgItem(IDC_STATIC7)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC7));
		GetDlgItem(IDC_STATIC8)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC8));
		GetDlgItem(IDC_STATIC9)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC9));
		GetDlgItem(IDC_STATIC10)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC10));
		GetDlgItem(IDC_STATIC11)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC11));
	}

	Data2Dlg();
	return TRUE;
}

void CmdTotalAppraisalInformationDlg::InitCode(int nBldType)
{
	m_CodeCombox.ResetContent();
	CString str;
	if (nBldType == 0)
		str = _T("GB 50144-2019");
	else
		str = _T("GB 50292-2015");
	
	m_CodeCombox.AddString(str);
	m_CodeCombox.SetCurSel(0);
}

void CmdTotalAppraisalInformationDlg::Data2Dlg()
{
	m_nBldType = m_Data.nBldType;
	InitCode(m_nBldType);
	CString str;
	str.Format(_T("%g"), m_Data.dMainGradeA);
	m_MainAMinEdit.SetWindowText(str);
	m_MainBMaxEdit.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dMainGradeB);
	m_MainBMinEdit.SetWindowText(str);
	m_MainCMaxEdit.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dMainGradeC);
	m_MainCMinEdit.SetWindowText(str);
	m_MainDMaxEdit.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dSecondaryGradeA);
	m_SecondaryAMinEdit.SetWindowText(str);
	m_SecondaryBMaxEdit.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dSecondaryGradeB);
	m_SecondaryBMinEdit.SetWindowText(str);
	m_SecondaryCMaxEdit.SetWindowText(str);
	str.Format(_T("%g"), m_Data.dSecondaryGradeC);
	m_SecondaryCMinEdit.SetWindowText(str);
	m_SecondaryDMaxEdit.SetWindowText(str);
	UpdateData(FALSE);
}

BOOL CmdTotalAppraisalInformationDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_CodeCombox.GetWindowText(m_Data.strAppraisalCode);
	m_Data.nBldType = IsDlgButtonChecked(IDC_CMD_APPRAISAL_TYPE_RADIO1) ? 0 : 1;
	m_MainAMinEdit.GetEditValue(m_Data.dMainGradeA);
	m_MainBMinEdit.GetEditValue(m_Data.dMainGradeB);
	m_MainCMinEdit.GetEditValue(m_Data.dMainGradeC);
	m_SecondaryAMinEdit.GetEditValue(m_Data.dSecondaryGradeA);
	m_SecondaryBMinEdit.GetEditValue(m_Data.dSecondaryGradeB);
	m_SecondaryCMinEdit.GetEditValue(m_Data.dSecondaryGradeC);
	return TRUE;
}

void CmdTotalAppraisalInformationDlg::OnOK()
{
	if(!Dlg2Data())	
		return;
	if (!m_pDoc->m_pDataCtrl->AddTaif(m_Data))
		return;
	CDialogMove::OnOK();
}

void CmdTotalAppraisalInformationDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CmdTotalAppraisalInformationDlg::OnChangeBuildingType()
{
	if (IsDlgButtonChecked(IDC_CMD_APPRAISAL_TYPE_RADIO2))
	{
		GetDlgItem(IDC_STATIC6)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC28));
		GetDlgItem(IDC_STATIC7)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC29));
		GetDlgItem(IDC_STATIC8)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC81));
		GetDlgItem(IDC_STATIC9)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC91));
		GetDlgItem(IDC_STATIC10)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC101));
		GetDlgItem(IDC_STATIC11)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC111));
		InitCode(1);
		m_Data.Initialize();
		m_Data.nBldType = 1;
		m_Data.dMainGradeC = 0.9;
		m_Data.dSecondaryGradeB = 0.9;
	}
	else
	{
		GetDlgItem(IDC_STATIC6)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC6));
		GetDlgItem(IDC_STATIC7)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC7));
		GetDlgItem(IDC_STATIC8)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC8));
		GetDlgItem(IDC_STATIC9)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC9));
		GetDlgItem(IDC_STATIC10)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC10));
		GetDlgItem(IDC_STATIC11)->SetWindowText(_LS(IDS_IDD_CMD_APPRAISAL_INFO_DLG_IDC_STATIC11));
		InitCode(0);
		m_Data.Initialize();
	}

	Data2Dlg();
}

void CmdTotalAppraisalInformationDlg::OnUpdateDate_MainB()
{
	double dMainA, dMainB;
	m_MainAMinEdit.GetEditValue(dMainA);
	m_MainBMinEdit.GetEditValue(dMainB);
	if (dMainA < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainA <= dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeA_Error));
		return;
	} 
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainA);
		m_MainBMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}	
}
void CmdTotalAppraisalInformationDlg::OnUpdateDate_MainC()
{
	double dMainA, dMainB, dMainC;
	m_MainAMinEdit.GetEditValue(dMainA);
	m_MainBMinEdit.GetEditValue(dMainB);
	m_MainCMinEdit.GetEditValue(dMainC);
	if (dMainB < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainB <= dMainC)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeB_Error));
		return;
	}
	else if (dMainA < dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeA_Error));
		return;
	}
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainB);
		m_MainCMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}
}
void CmdTotalAppraisalInformationDlg::OnUpdateDate_MainD()
{
	double dMainB, dMainC;
	m_MainBMinEdit.GetEditValue(dMainB);
	m_MainCMinEdit.GetEditValue(dMainC);

	if (dMainC < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainC > dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeB_Error));
		return;
	}
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainC);
		m_MainDMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}
}
void CmdTotalAppraisalInformationDlg::OnUpdateDate_SecondaryB()
{
	double dMainA, dMainB;
	m_SecondaryAMinEdit.GetEditValue(dMainA);
	m_SecondaryBMinEdit.GetEditValue(dMainB);

	if (dMainA < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainA <= dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeA_Error));
		return;
	}
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainA);
		m_SecondaryBMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}
}
void CmdTotalAppraisalInformationDlg::OnUpdateDate_SecondaryC()
{
	double dMainA, dMainB, dMainC;
	m_SecondaryAMinEdit.GetEditValue(dMainA);
	m_SecondaryBMinEdit.GetEditValue(dMainB);
	m_SecondaryCMinEdit.GetEditValue(dMainC);

	if (dMainB < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainB <= dMainC)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeB_Error));
		return;
	}
	else if (dMainA < dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeA_Error));
		return;
	}
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainB);
		m_SecondaryCMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}
}
void CmdTotalAppraisalInformationDlg::OnUpdateDate_SecondaryD()
{
	double dMainB, dMainC;
	m_SecondaryBMinEdit.GetEditValue(dMainB);
	m_SecondaryCMinEdit.GetEditValue(dMainC);
	
	if (dMainC < 0)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_LimitVal_Error));
		return;
	}
	if (dMainC > dMainB)
	{
		MessageBox(_LS(IDS_WG_CMD_Appraisal_GradeB_Error));
		return;
	}
	else
	{
		CString str = _T("");
		str.Format(_T("%.2f"), dMainC);
		m_SecondaryDMaxEdit.SetWindowText(str);
		UpdateData(FALSE);
	}
}

