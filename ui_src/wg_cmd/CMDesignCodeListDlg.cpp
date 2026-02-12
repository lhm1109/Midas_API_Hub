// CMDesignCodeListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDesignCodeListDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_EditData.h"
#include "CMMatPageItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCMDesignCodeListDlg dialog


CCMDesignCodeListDlg::CCMDesignCodeListDlg(
	CDBDoc* pDoc, CString &strType, CRect &rect, CWnd* pParent /*=NULL*/)
	: CDialog(CCMDesignCodeListDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_type = strType;
	m_rect = rect;
	m_pParent = (CCMMatPageItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMDesignCodeListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMDesignCodeListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDesignCodeListDlg)
	DDX_Control(pDX, IDC_CMD_MP_MAT_NAME, m_cboName);
	DDX_Control(pDX, IDC_CMD_MP_DESIGN_CODE, m_cboCode);
	//}}AFX_DATA_MAP
}

int CCMDesignCodeListDlg::ChangeDesignCodeList()
{
	// reset contents of ComboBox
	m_cboCode.ResetContent();
	m_strCurCode = _T("");

	// get design code by type
	if (m_type.IsEmpty()) return 0;
	CArray<CString, CString&> DesignCodeList;
	CString strType = m_type.GetAt(0);
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	int i = 0;
	for (i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_cboCode.AddString(DesignCodeList[i]);
	}
	return i;
}

int CCMDesignCodeListDlg::ChangeMaterialNameList()
{
	// reset contents of ComboBox
	m_cboName.ResetContent();
	m_strCurName = _T("");

	if (m_strCurCode == _T("")) return 0;

	// get material name list by material type
	// first get material type
	CString MatlType;
	m_pDoc->m_pMatlDB->GetMatlType(m_strCurCode, MatlType);

	CArray <CString, CString&> NameList;
	// next get material name list
	if (MatlType == _T("S"))  // steel
		m_pDoc->m_pMatlDB->GetSteelNameList(m_strCurCode, NameList);
	else if (MatlType == _T("C")) // concrete
		m_pDoc->m_pMatlDB->GetConcreteNameList(m_strCurCode, NameList);
	else ASSERT(0);
	int i = 0;
	for (i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(m_strCurCode, NameList[i]);
#endif
		m_cboName.AddString(NameList[i]);
	}
	return i;
}

void CCMDesignCodeListDlg::ChangeValueInParentDialog()
{
	CString MatlType;
	m_pDoc->m_pMatlDB->GetMatlType(m_strCurCode, MatlType);
	if (MatlType == _T("S")) // steel
	{
		T_MATL_STEEL SteelData;
		m_pDoc->m_pMatlDB->GetSteelData(m_strCurCode, m_strCurName, SteelData);
		//m_pParent->ChangeSteelData(SteelData);
	}
	else if (MatlType == _T("C")) // concrete
	{
		T_MATL_CONCRETE ConcreteData;
		m_pDoc->m_pMatlDB->GetConcreteData(m_strCurCode, m_strCurName, ConcreteData);
		//m_pParent->ChangeConcreteData(ConcreteData);
	}
	else ASSERT(0);

	// change name
	CString strName;
	m_cboName.GetWindowText(strName);
	//m_pParent->ChangeName(strName);
}


BEGIN_MESSAGE_MAP(CCMDesignCodeListDlg, CDialog)
	//{{AFX_MSG_MAP(CCMDesignCodeListDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MP_DESIGN_CODE, OnChangeDesignCode)
	ON_CBN_SELCHANGE(IDC_CMD_MP_MAT_NAME, OnChangeMatName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDesignCodeListDlg message handlers

BOOL CCMDesignCodeListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// change design code list and set initial design code
	if (ChangeDesignCodeList() > 0) 
	{
		m_cboCode.SetCurSel(0);
		m_cboCode.GetWindowText(m_strCurCode);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(m_strCurCode);
#endif
	}
	// change material name list but do not set initial material name
	ChangeMaterialNameList();
	
	// move position
	CRect rect;
	GetWindowRect(rect);
	int w, h;
	w = rect.Width();
	h = (rect.Height() > m_rect.Height()) ? rect.Height() : m_rect.Height();
	
	MoveWindow(m_rect.left, m_rect.top, w, h);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDesignCodeListDlg::OnChangeDesignCode() 
{
	// TODO: Add your control notification handler code here
	CString strCode;
	m_cboCode.GetWindowText(strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	if (strCode == m_strCurCode) return;
	m_strCurCode = strCode;

	if (ChangeMaterialNameList() > 0)
	{
		m_cboName.SetCurSel(0);
		m_cboName.GetWindowText(m_strCurName);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCurCode, m_strCurName);
#endif
		ChangeValueInParentDialog();
	}
}

void CCMDesignCodeListDlg::OnChangeMatName() 
{
	// TODO: Add your control notification handler code here
	CString strName;
	m_cboName.GetWindowText(strName);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(m_strCurCode, strName);
#endif
	if (strName == m_strCurName) return;
	m_strCurName = strName;

	ChangeValueInParentDialog();
}

void CCMDesignCodeListDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_strCurName.IsEmpty())
	{
		CString strTemp = _LS(IDS_WG_CMD__ADDD__You_have_to_select_material__n);
		strTemp += _LS(IDS_WG_CMD__ADDD__Please__choose_one_in_the_name_li);

		AfxMessageBox(strTemp);
		return;
	}
	CDialog::OnOK();
}
