// SeisCommonPmcmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonPmcmDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonPmcmDlg dialog

CSeisCommonPmcmDlg::CSeisCommonPmcmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonPmcmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCommonPmcmDlg)
	m_nAddDel = 0;
	m_nElemType = 0;
	m_nCalcMethod = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonPmcmDlg::~CSeisCommonPmcmDlg()
{

}

void CSeisCommonPmcmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCommonPmcmDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_TM_PMCM_ELEM_COLUMN_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_TM_PMCM_KEEP_RDO1, m_nCalcMethod);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonPmcmDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonPmcmDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonPmcmDlg message handlers

void CSeisCommonPmcmDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
	if(m_nElemType==0)
	{
		T_ELEM_D ElemD;
		T_MATL_D MatlD;
		for(int i=aElemK.GetSize()-1; i>=0; i--)
		{
		  T_ELEM_K ElemK = aElemK[i];
		  if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)==D_MBTP_COLUMN && m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))
		  {
			  if(m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD) && MatlD.Type==_T("C")) continue;
		  }
		  aElemK.RemoveAt(i);
		}
	}
	else
	{
		for(int i=aElemK.GetSize()-1; i>=0; i--)
		{
			T_ELEM_K ElemK = aElemK[i];
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)==D_MBTP_WALL) continue;
			aElemK.RemoveAt(i);
		}
	}

	T_PMCM_D PmcmD;
	PmcmD.Initialize();
	PmcmD.nCalcMethod = m_nCalcMethod;

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddPmcm(aElemK, PmcmD);
	else               m_pDoc->m_pDataCtrl->DelPmcm(aElemK);

	if (m_nAddDel == 0)
	{
		IUsageCounter::Use(_T("SPE_PMCM"));
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	// √ ±‚»≠
	m_nElemType=0;
	m_nCalcMethod=0;
	UpdateData(FALSE);
}

BOOL CSeisCommonPmcmDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCommonPmcmDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}