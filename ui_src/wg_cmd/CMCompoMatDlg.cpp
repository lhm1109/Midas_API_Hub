// CMCompoMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMCompoMatDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

//#include "CmSectItemSrc.h"
#include "CmSectItemCon.h"
//#include "CMSectItemPSC_CMP.h"
#include "CMSEctItemTap.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMCompoMatDlg dialog


CCMCompoMatDlg::CCMCompoMatDlg(
	CDBDoc* pDoc, CRect &rect, UINT ParentID, CWnd* pParent/*=NULL*/)
	: CDialogMove(CCMCompoMatDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_rect = rect;

	m_ParentID = ParentID;
	if(m_ParentID == IDD_CMD_SECT_ITEM_CON)
		m_pParentCon = (CCMSectItemCon*)pParent;
	/*
	else if(m_ParentID == IDD_CMD_SECT_ITEM_SRC)
		m_pParentSrc = (CCMSectItemSrc*)pParent;
	else if (m_ParentID == IDD_CMD_SECT_ITEM_PSC_CMP)
		m_pParentPSC = (CCMSectItemPSC_CMP*)pParent;
	*/
	else if (m_ParentID == IDD_CMD_SECT_ITEM_TAP)
		m_pParentTap = (CCMSectItemTap*)pParent;
	else // default
		m_pParentCon = (CCMSectItemCon*)pParent;

	//{{AFX_DATA_INIT(CCMCompoMatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMCompoMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMCompoMatDlg)
	DDX_Control(pDX, IDC_CMD_SP_CD_GIRDER_NAME, m_cboNameGirder);
	DDX_Control(pDX, IDC_CMD_SP_CD_GIRDER_DB,   m_cboDBGirder);
	DDX_Control(pDX, IDC_CMD_SP_CD_GIRDER_DB2,  m_cboCodeGirder);
	DDX_Control(pDX, IDC_CMD_SP_CD_SLAB_NAME,   m_cboNameSlab);
	DDX_Control(pDX, IDC_CMD_SP_CD_SLAB_DB,     m_cboDBSlab);
	DDX_Control(pDX, IDC_CMD_SP_CD_SLAB_DB2,    m_cboCodeSlab);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMConcrSteelMatlDlg implementations

int CCMCompoMatDlg::ChangeDBNameList()
{
	// reset contents of ComboBox
	m_cboDBSlab.ResetContent();
	m_cboDBGirder.ResetContent();
	m_nCurDBSlab = -1;
	m_nCurDBGirder = -1;

	// get concrete design code for Slab and Girder by type
	CArray<CString, CString&> DBNameList;
	CString strType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DBNameList);
	int i = 0;
	for (i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DBNameList[i]);
#endif
		m_cboDBSlab.AddString(DBNameList[i]);
		m_cboDBGirder.AddString(DBNameList[i]);
	}
	if (i == 0) return 0;  
	return i;
}

int CCMCompoMatDlg::ChangeCodeNameList()
{
	CString strDBSlab = _T(""), strDBGirder = _T("");
	m_cboDBSlab.GetWindowText(strDBSlab);
	m_cboDBGirder.GetWindowText(strDBGirder);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDBSlab);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDBGirder);
#endif

	m_cboCodeSlab.ResetContent();
	m_cboCodeGirder.ResetContent();

	int nReturn = 0;
	CArray <CString, CString&> aNameList;
	int nSize = m_pDoc->m_pMatlDB->GetConcreteSubNameList(strDBSlab, aNameList);
	for(int i=0; i<nSize; ++i)
	{
		m_cboCodeSlab.AddString(aNameList[i]);
	}
	if(nSize>0) nReturn = 1;
	nSize = m_pDoc->m_pMatlDB->GetConcreteSubNameList(strDBGirder, aNameList);
	for(int i=0; i<nSize; ++i)
	{
		m_cboCodeGirder.AddString(aNameList[i]);
	}
	if(nSize>0) nReturn = (nReturn==0) ? 2 : 3;
	
	return nReturn;
}

int CCMCompoMatDlg::ChangeMaterialNameList(CString StructuralType)
{
	CComboBox* pNameList;
	int *nCurDB;
	int* nCurName;
	CString DBName;

	if (StructuralType == _T("SLAB"))
	{
		pNameList = &m_cboNameSlab;
		nCurDB = &m_nCurDBSlab;
		nCurName = &m_nCurNameSlab;
		m_cboDBSlab.GetWindowText(DBName);
	}
	else if (StructuralType == _T("GIRDER"))
	{
		pNameList = &m_cboNameGirder;
		nCurDB = &m_nCurDBGirder;
		nCurName = &m_nCurNameGirder;
		m_cboDBGirder.GetWindowText(DBName);
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
	m_pDoc->m_pMatlDB->GetConcreteNameList(DBName, NameList);
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

void CCMCompoMatDlg::ChangeValueInParentDialog()
{
	CString DBNameSlab, CodeNameSlab, DBNameGirder, CodeNameGirder;
	CString MatlNameSlab, MatlNameGirder;
	m_cboDBSlab.GetWindowText(DBNameSlab);
	m_cboCodeSlab.GetWindowText(CodeNameSlab);
	m_cboDBGirder.GetWindowText(DBNameGirder);
	m_cboCodeGirder.GetWindowText(CodeNameGirder);
	m_cboNameSlab.GetWindowText(MatlNameSlab);
	m_cboNameGirder.GetWindowText(MatlNameGirder);

	if (MatlNameSlab.IsEmpty()) return;
	if (MatlNameGirder.IsEmpty()) return;

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(DBNameSlab);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(DBNameGirder);

	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(DBNameSlab, MatlNameSlab);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(DBNameGirder, MatlNameGirder);
#endif

	T_MATL_CONCRETE ConcreteDataSlab, ConcreteDataGirder;  
	m_pDoc->m_pMatlDB->GetConcreteDataNew(DBNameSlab, CodeNameSlab, MatlNameSlab, ConcreteDataSlab);
	m_pDoc->m_pMatlDB->GetConcreteDataNew(DBNameGirder, CodeNameGirder, MatlNameGirder, ConcreteDataGirder);
	
	if(m_ParentID == IDD_CMD_SECT_ITEM_CON)
		m_pParentCon->ChangeMatlDataCICTPC(ConcreteDataSlab, ConcreteDataGirder);
	/*
	else if(m_ParentID == IDD_CMD_SECT_ITEM_SRC)
		m_pParentSrc->ChangeMatlDataCICT(ConcreteDataSlab, ConcreteDataGirder);
	else if (m_ParentID == IDD_CMD_SECT_ITEM_PSC_CMP)
		m_pParentPSC->ChangeMatlDataCICT(ConcreteDataSlab, ConcreteDataGirder);
	*/
	else if (m_ParentID == IDD_CMD_SECT_ITEM_TAP)
		m_pParentTap->ChangeMatlDataCICT(ConcreteDataSlab, ConcreteDataGirder);
	else // default
		m_pParentCon->ChangeMatlDataCICTPC(ConcreteDataSlab, ConcreteDataGirder);

}

BEGIN_MESSAGE_MAP(CCMCompoMatDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMCompoMatDlg)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_SLAB_DB, OnChangeDBSlab)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_SLAB_DB2, OnChangeCodeSlab)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_GIRDER_DB, OnChangeDBGirder)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_GIRDER_DB2, OnChangeCodeGirder)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_SLAB_NAME, OnChangeNameSlab)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CD_GIRDER_NAME, OnChangeNameGirder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMCompoMatDlg message handlers
BOOL CCMCompoMatDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	// change db name list and set initial db name
	if (ChangeDBNameList() > 0) 
	{
		// Preference에 정의된 값으로 설정 : 새 버전
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(pref.Property.ConcreteMaterialDBName);
#endif

		m_nCurDBSlab = m_cboDBSlab.SelectString(-1, pref.Property.ConcreteMaterialDBName);
		if (m_nCurDBSlab == CB_ERR)
		{
			m_cboDBSlab.SetCurSel(0);
			m_nCurDBSlab = 0;
		}
		m_nCurDBGirder = m_cboDBGirder.SelectString(-1, pref.Property.ConcreteMaterialDBName);
		if (m_nCurDBGirder == CB_ERR)
		{
			m_cboDBGirder.SetCurSel(0);
			m_nCurDBGirder = 0;
		}
		int nCode = ChangeCodeNameList();
		if(nCode==1 || nCode==3) m_cboCodeSlab.SelectString(-1, pref.Property.ConcreteMaterialCodeName);
		if(nCode==2 || nCode==3) m_cboCodeGirder.SelectString(-1, pref.Property.ConcreteMaterialCodeName);
	}
	// change material name list but do not set initial material name
	ChangeMaterialNameList(_T("SLAB"));
	ChangeMaterialNameList(_T("GIRDER"));
	
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

void CCMCompoMatDlg::OnChangeDBSlab() 
{
	// TODO: Add your control notification handler code here
	int nDB = m_cboDBSlab.GetCurSel();
	if (nDB == m_nCurDBSlab) return;

	m_nCurDBSlab = nDB;

	int nCodeGirder = m_cboCodeGirder.GetCurSel();

	CString strDBSlab = _T("");
	m_cboDBSlab.GetWindowText(strDBSlab);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDBSlab);
#endif

	BOOL bEnable = (strDBSlab==_LSX(KS01(RC)) || strDBSlab==_LSX(KS01-Civil(RC)));
	GetDlgItem(IDC_CMD_SP_CD_SLAB_DB2)->EnableWindow(bEnable);

	int nCode = ChangeCodeNameList();
	if(nCode==1 || nCode==3)
	{
		m_cboCodeSlab.SetCurSel(0);
		if(nCodeGirder == 0) m_cboCodeGirder.SetCurSel(0);
		else                 m_cboCodeGirder.SetCurSel(nCodeGirder);
	}

	if (ChangeMaterialNameList(_T("SLAB")) > 0)
	{
		m_cboNameSlab.SetCurSel(0);
		m_nCurNameSlab = 0;
		ChangeValueInParentDialog();
	}
}

void CCMCompoMatDlg::OnChangeCodeSlab() 
{
	ChangeValueInParentDialog();
}

void CCMCompoMatDlg::OnChangeDBGirder() 
{
	// TODO: Add your control notification handler code here
	int nDB = m_cboDBGirder.GetCurSel();
	if (nDB == m_nCurDBGirder) return;

	m_nCurDBGirder = nDB;

	int nCodeSlab = m_cboCodeSlab.GetCurSel();

	CString strDBGirder = _T("");
	m_cboDBGirder.GetWindowText(strDBGirder);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strDBGirder);
#endif

	BOOL bEnable = (strDBGirder==_LSX(KS01(RC)) || strDBGirder==_LSX(KS01-Civil(RC)));
	GetDlgItem(IDC_CMD_SP_CD_GIRDER_DB2)->EnableWindow(bEnable);

	int nCode = ChangeCodeNameList();
	if(nCode==2 || nCode==3)
	{
		m_cboCodeGirder.SetCurSel(0);
		if(nCodeSlab == 0) m_cboCodeSlab.SetCurSel(0);
		else               m_cboCodeSlab.SetCurSel(nCodeSlab);
	}
	
	if (ChangeMaterialNameList(_T("GIRDER")) > 0)
	{
		m_cboNameGirder.SetCurSel(0);
		m_nCurNameGirder = 0;
		ChangeValueInParentDialog();
	}
}

void CCMCompoMatDlg::OnChangeCodeGirder() 
{
	ChangeValueInParentDialog();
}

void CCMCompoMatDlg::OnChangeNameSlab() 
{
	// TODO: Add your control notification handler code here
	int nName = m_cboNameSlab.GetCurSel();
	if (nName == m_nCurNameSlab) return;
	m_nCurNameSlab = nName;

	ChangeValueInParentDialog();	
}

void CCMCompoMatDlg::OnChangeNameGirder() 
{
	// TODO: Add your control notification handler code here
	int nName = m_cboNameGirder.GetCurSel();
	if (nName == m_nCurNameGirder) return;
	m_nCurNameGirder = nName;

	ChangeValueInParentDialog();		
}

void CCMCompoMatDlg::OnOK() 
{
	// TODO: Add extra validation here

	if (m_cboNameSlab.GetCurSel() == -1)
	{
		CString strTemp = _LS(IDS_WG_CMD__ADDD__You_have_to_select_concrete_mater);
		strTemp += _LS(IDS_WG_CMD__ADDD__Please__choose_one_in_the_name_li);
		AfxMessageBox(strTemp);
		return;
	}
	if (m_cboNameGirder.GetCurSel() == -1)
	{
		CString strTemp = _LS(IDS_WG_CMD__ADDD__You_have_to_select_concrete_mater);
		strTemp += _LS(IDS_WG_CMD__ADDD__Please__choose_one_in_the_name_li);
		AfxMessageBox(strTemp);
		return;
	}
	
	CDialogMove::OnOK();
}
