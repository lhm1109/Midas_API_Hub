// SeisModifyXYFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisModifyXYFactorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisModifyXYFactorDlg dialog

CSeisModifyXYFactorDlg::CSeisModifyXYFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisModifyXYFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisModifyXYFactorDlg)
	m_nOption = 0;
	m_bDoNotExceed = FALSE;
	m_SemeD.Initialize();
	//}}AFX_DATA_INIT
}

void CSeisModifyXYFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisModifyXYFactorDlg)
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_XY_FACT_EDT1, m_Value1);
	DDX_Control(pDX, IDC_TM_XY_FACT_EDT2, m_Value2);
	DDX_Control(pDX, IDC_TM_XY_FACT_EDT3, m_Value3);
	DDX_Check(pDX, IDC_TM_XY_FACT_CHK, m_bDoNotExceed);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisModifyXYFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisModifyXYFactorDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_XY_FACT_CHK, OnDoNotExceedChk)
	ON_EN_CHANGE(IDC_TM_XY_FACT_EDT1, OnChangeEdit)
	ON_EN_CHANGE(IDC_TM_XY_FACT_EDT2, OnChangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisModifyXYFactorDlg message handlers

BOOL CSeisModifyXYFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	m_bDoNotExceed = FALSE;
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	if(!m_pDoc->m_pAttrCtrl2->GetSeme(m_SemeD)) m_SemeD.Initialize();

	InitCtrl();
	
	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisModifyXYFactorDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_K ElemK=NULL;
	T_ELEM_D ElemD;
	T_MATL_D MatlD;
	CArray<T_ELEM_K, T_ELEM_K> aFiltedSelKey;
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		ElemK = aSelKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }
		aFiltedSelKey.Add(ElemK);
	}

	if (aFiltedSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_Selected_Member));
		return;
	}

	T_XFAC_D data;
	data.Initialize();

	data.dChi = m_Value1.GetEditValue();
	data.dGamma = m_Value2.GetEditValue();
	data.bDoNotExeed = m_bDoNotExceed;

	BOOL bSuccess=FALSE;
	if (m_nOption == 0) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddXfac(aFiltedSelKey, data);	
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelXfac(aFiltedSelKey);

	if (m_nOption == 0)
	{
		IUsageCounter::Use(_T("SPE_XFAC"));
	}

	if (bSuccess)
	{
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		InitCtrl();
	}
}

void CSeisModifyXYFactorDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CSeisModifyXYFactorDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 1) // delete
	{
		EnableValueEdit(FALSE); // disable all value edit
	}
	else
	{
		EnableValueEdit(TRUE);
	}
}

void CSeisModifyXYFactorDlg::OnDoNotExceedChk()
{
	SetChiGamma();
}

void CSeisModifyXYFactorDlg::OnChangeEdit()
{
	SetChiGamma();
}

void CSeisModifyXYFactorDlg::SetChiGamma()
{
	UpdateData(TRUE);

	double dChi = m_Value1.GetEditValue();
	double dGamma = m_Value2.GetEditValue();
	double dXY = dChi*dGamma;
	double dBase = m_pDoc->m_pAttrCtrl->GetXfacBase();

	if(m_bDoNotExceed && dXY>dBase)
	{
		m_Value3.SetEditUnit(dBase);
	}
	else
	{
		m_Value3.SetEditUnit(dXY);
	}
}

void CSeisModifyXYFactorDlg::EnableValueEdit(BOOL bEnable)
{
	m_Value1.EnableWindow(bEnable);
	m_Value2.EnableWindow(bEnable);
	m_Value3.EnableWindow(bEnable);
	GetDlgItem(IDC_TM_XY_FACT_CHK)->EnableWindow(bEnable);
}

void CSeisModifyXYFactorDlg::InitCtrl()
{
	m_Value1.SetUnitType(D_UNITSYS_NONE);
	m_Value2.SetUnitType(D_UNITSYS_NONE);
	m_Value3.SetUnitType(D_UNITSYS_NONE);
	m_Value1.SetEditUnit(m_SemeD.data.eval.nPerformLevel==D_SEME_PERFORM_CP ? 1.0 : 1.3);
	m_Value2.SetEditUnit(1.0);
	m_Value3.SetEditUnit(m_SemeD.data.eval.nPerformLevel==D_SEME_PERFORM_CP ? 1.0 : 1.3);
}