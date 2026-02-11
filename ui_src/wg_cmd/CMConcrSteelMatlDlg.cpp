// CMConcrSteelMatlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMConcrSteelMatlDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

//#include "CMSecPageSrcPage.h"
#include "CmSectItemSrc.h"
#include "CmSectItemCon.h"
#include "CMSectItemPSC_CMP.h"
#include "CMSEctItemTap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCMConcrSteelMatlDlg dialog


CCMConcrSteelMatlDlg::CCMConcrSteelMatlDlg(
	CDBDoc* pDoc, CRect &rect, UINT ParentID, CWnd* pParent/*=NULL*/, BOOL bDumbbell/* = FALSE*/)
	: CDialog(CCMConcrSteelMatlDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_rect = rect;
	m_bDumbbell = bDumbbell;
	//m_pParent = (CCMSecPageSrcPage*)pParent;
	//m_pParent = NULL;

	m_ParentID = ParentID;
	if(m_ParentID == IDD_CMD_SECT_ITEM_CON)
		m_pParentCon = (CCMSectItemCon*)pParent;
	else if(m_ParentID == IDD_CMD_SECT_ITEM_SRC)
		m_pParentSrc = (CCMSectItemSrc*)pParent;
	else if (m_ParentID == IDD_CMD_SECT_ITEM_PSC_CMP)
		m_pParentPSC = (CCMSectItemPSC_CMP*)pParent;
	else if (m_ParentID == IDD_CMD_SECT_ITEM_TAP)
		m_pParentTap = (CCMSectItemTap*)pParent;

	//{{AFX_DATA_INIT(CCMConcrSteelMatlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMConcrSteelMatlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMConcrSteelMatlDlg)
	DDX_Control(pDX, IDC_CMD_SP_CD_S_NAME, m_cboNameSteel);
	DDX_Control(pDX, IDC_CMD_SP_CD_S_DB,   m_cboDBSteel);
	DDX_Control(pDX, IDC_CMD_SP_CD_C_NAME, m_cboNameConcr);
	DDX_Control(pDX, IDC_CMD_SP_CD_C_DB,   m_cboDBConcr);
	DDX_Control(pDX, IDC_CMD_SP_CD_C_DB2,  m_cboCodeConcr);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMConcrSteelMatlDlg implementations

int CCMConcrSteelMatlDlg::ChangeDBNameList()
{
	// reset contents of ComboBox
	m_cboDBConcr.ResetContent();
	m_cboDBSteel.ResetContent();
	m_nCurDBConcr = -1;
	m_nCurDBSteel = -1;

	// get concrete design code by type
	CArray<CString, CString&> DBNameList;
	CString strType = _T("C");
	int i = 0;
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DBNameList, m_bDumbbell);
	for (i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DBNameList[i]);
#endif
		m_cboDBConcr.AddString(DBNameList[i]);
	}
	if (i == 0) return 0;

	// get steel design code by type
	DBNameList.RemoveAll();
	strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DBNameList, m_bDumbbell);
	for (i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DBNameList[i]);
#endif
		m_cboDBSteel.AddString(DBNameList[i]);
	}

	return i;
}

int CCMConcrSteelMatlDlg::ChangeCodeNameList()
{
	CString strDB = _T("");
	m_cboDBConcr.GetWindowText(strDB);
	
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDB);
#endif

	m_cboCodeConcr.ResetContent();

	CArray <CString, CString&> aNameList;
	int nSize = m_pDoc->m_pMatlDB->GetConcreteSubNameList(strDB, aNameList);
	for(int i=0; i<nSize; ++i)
	{
		m_cboCodeConcr.AddString(aNameList[i]);
	}

	return (nSize>0) ? 1 : 0;
}

int CCMConcrSteelMatlDlg::ChangeMaterialNameList(CString MatlType)
{
	CComboBox* pNameList;
	int *nCurDB;
	int* nCurName;
	CString DBName;

	if (MatlType == _T("C"))
	{
		pNameList = &m_cboNameConcr;
		nCurDB = &m_nCurDBConcr;
		nCurName = &m_nCurNameConcr;
		m_cboDBConcr.GetWindowText(DBName);
	}
	else if (MatlType == _T("S"))
	{
		pNameList = &m_cboNameSteel;
		nCurDB = &m_nCurDBSteel;
		nCurName = &m_nCurNameSteel;
		m_cboDBSteel.GetWindowText(DBName);
	}
	else ASSERT(0);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(DBName);
#endif

	// reset contents of ComboBox
	pNameList->ResetContent();
	*nCurName = -1;

	if (*nCurDB == -1) return 0;

	// get material name list
	CArray <CString, CString&> NameList;

	// next get material name list
	if (MatlType == _T("S"))  // steel
		m_pDoc->m_pMatlDB->GetSteelNameList(DBName, NameList, m_bDumbbell);
	else if (MatlType == _T("C")) // concrete
		m_pDoc->m_pMatlDB->GetConcreteNameList(DBName, NameList, m_bDumbbell);
	else ASSERT(0);
	int i = 0;
	for (i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(DBName, NameList[i]);
#endif
		pNameList->AddString(NameList[i]);
	}
	return i;
}

void CCMConcrSteelMatlDlg::ChangeValueInParentDialog()
{
	CString DBNameConcr, CodeNameConcr, DBNameSteel;
	CString MatlNameConcr, MatlNameSteel;
	m_cboDBConcr.GetWindowText(DBNameConcr);
	m_cboCodeConcr.GetWindowText(CodeNameConcr);
	m_cboDBSteel.GetWindowText(DBNameSteel);
	m_cboNameConcr.GetWindowText(MatlNameConcr);
	m_cboNameSteel.GetWindowText(MatlNameSteel);

	if (MatlNameConcr.IsEmpty()) return;
	if (MatlNameSteel.IsEmpty()) return;


#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(DBNameConcr);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(DBNameSteel);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(DBNameSteel, MatlNameSteel);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(DBNameConcr, MatlNameConcr);
#endif

	T_MATL_CONCRETE ConcreteData;
	T_MATL_STEEL SteelData;
	m_pDoc->m_pMatlDB->GetConcreteDataNew(DBNameConcr, CodeNameConcr, MatlNameConcr, ConcreteData);
	m_pDoc->m_pMatlDB->GetSteelData(DBNameSteel, MatlNameSteel, SteelData);
	//if (m_pParent)
	//  m_pParent->ChangeMatlData(ConcreteData, SteelData);
	//else if (m_pParent2)
	if(m_ParentID == IDD_CMD_SECT_ITEM_CON)
		m_pParentCon->ChangeMatlData(ConcreteData, SteelData);
	else if(m_ParentID == IDD_CMD_SECT_ITEM_SRC)
		m_pParentSrc->ChangeMatlData(ConcreteData, SteelData);
	else if (m_ParentID == IDD_CMD_SECT_ITEM_PSC_CMP)
		m_pParentPSC->ChangeMatlData(ConcreteData, SteelData);
	else if (m_ParentID == IDD_CMD_SECT_ITEM_TAP)
		m_pParentTap->ChangeMatlData(ConcreteData, SteelData);
}

BEGIN_MESSAGE_MAP(CCMConcrSteelMatlDlg, CDialog)
	//{{AFX_MSG_MAP(CCMConcrSteelMatlDlg)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_C_DB, OnChangeDBConcr)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_C_DB2, OnChangeCodeConcr)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_S_DB, OnChangeDBSteel)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_C_NAME, OnChangeNameConcr)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_S_NAME, OnChangeNameSteel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMConcrSteelMatlDlg message handlers

BOOL CCMConcrSteelMatlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// change db name list and set initial db name
	if (ChangeDBNameList() > 0) 
	{
		// None으로 설정 : 이전 버전
		//m_cboDBConcr.SetCurSel(0);
		//m_cboDBSteel.SetCurSel(0);
		//m_nCurDBConcr = 0;
		//m_nCurDBSteel = 0;
		// Preference에 정의된 값으로 설정 : 새 버전
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(pref.Property.ConcreteMaterialDBName);
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(pref.Property.SteelMaterialDBName);
#endif
		/*m_nCurDBConcr = */m_cboDBConcr.SelectString(-1, pref.Property.ConcreteMaterialDBName);
		OnChangeDBConcr();
		if (m_nCurDBConcr == CB_ERR)
		{
			m_cboDBConcr.SetCurSel(0);
			m_nCurDBConcr = 0;
		}
		m_nCurDBSteel = m_cboDBSteel.SelectString(-1, pref.Property.SteelMaterialDBName);
		if (m_nCurDBSteel == CB_ERR)
		{
			m_cboDBSteel.SetCurSel(0);
			m_nCurDBSteel = 0;
		}
		if(ChangeCodeNameList()>0)
		{
			m_cboCodeConcr.SelectString(-1, pref.Property.ConcreteMaterialCodeName);
		}
	}

	// change material name list but do not set initial material name
	ChangeMaterialNameList(_T("C"));
	ChangeMaterialNameList(_T("S"));
	
	// move position
	// m_rect에 지정된 right, top만 참조하도록 변경
	CRect rect;
	GetWindowRect(rect);
	int w, h;
	w = rect.Width();
	//h = (rect.Height() > m_rect.Height()) ? rect.Height() : m_rect.Height();
	h = rect.Height();
	
	//MoveWindow(m_rect.left, m_rect.top, w, h);
	MoveWindow(m_rect.right, m_rect.top, w, h);
	MoveDlgWithinScreen();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMConcrSteelMatlDlg::OnChangeDBConcr() 
{
	// TODO: Add your control notification handler code here
	int nDB = m_cboDBConcr.GetCurSel();
	if (nDB == m_nCurDBConcr)
	{
		GetDlgItem(IDC_CMD_SP_CD_C_DB2)->EnableWindow(FALSE);
		return;
	}

	m_nCurDBConcr = nDB;

	CString strDB = _T("");
	m_cboDBConcr.GetWindowText(strDB);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDB);
#endif
	BOOL bEnable = (strDB==_LSX(KS01(RC)) || strDB==_LSX(KS01-Civil(RC)));
	GetDlgItem(IDC_CMD_SP_CD_C_DB2)->EnableWindow(bEnable);

	if(ChangeCodeNameList()>0)
	{
		m_cboCodeConcr.SetCurSel(0);
	}

	if (ChangeMaterialNameList(_T("C")) > 0)
	{
		m_cboNameConcr.SetCurSel(0);
		m_nCurNameConcr = 0;

		ChangeValueInParentDialog();
	}
}

void CCMConcrSteelMatlDlg::OnChangeCodeConcr()
{
	ChangeValueInParentDialog();
}

void CCMConcrSteelMatlDlg::OnChangeDBSteel() 
{
	// TODO: Add your control notification handler code here
	int nDB = m_cboDBSteel.GetCurSel();
	if (nDB == m_nCurDBSteel) return;

	m_nCurDBSteel = nDB;

	if (ChangeMaterialNameList(_T("S")) > 0)
	{
		m_cboNameSteel.SetCurSel(0);
		m_nCurNameSteel = 0;
		ChangeValueInParentDialog();
	}
}

void CCMConcrSteelMatlDlg::OnChangeNameConcr() 
{
	// TODO: Add your control notification handler code here
	int nName = m_cboNameConcr.GetCurSel();
	if (nName == m_nCurNameConcr) return;
	m_nCurNameConcr = nName;

	ChangeValueInParentDialog();	
}

void CCMConcrSteelMatlDlg::OnChangeNameSteel() 
{
	// TODO: Add your control notification handler code here
	int nName = m_cboNameSteel.GetCurSel();
	if (nName == m_nCurNameSteel) return;
	m_nCurNameSteel = nName;

	ChangeValueInParentDialog();		
}

void CCMConcrSteelMatlDlg::OnOK() 
{
	// TODO: Add extra validation here

	if (m_cboNameConcr.GetCurSel() == -1)
	{
		CString strTemp = _LS(IDS_WG_CMD__ADDD__You_have_to_select_concrete_mater);
		strTemp += _LS(IDS_WG_CMD__ADDD__Please__choose_one_in_the_name_li);
		AfxMessageBox(strTemp);
		return;
	}
	if (m_cboNameSteel.GetCurSel() == -1)
	{
		CString strTemp = _LS(IDS_WG_CMD__ADDD__You_have_to_select_steel_material);
		strTemp += _LS(IDS_WG_CMD__ADDD__Please__choose_one_in_the_name_li);
		AfxMessageBox(strTemp);
		return;
	}
	
	CDialog::OnOK();
}
