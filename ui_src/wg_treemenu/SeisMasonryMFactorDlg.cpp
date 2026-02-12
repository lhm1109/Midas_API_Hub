// SeisMasonryMFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisMasonryMFactorDlg.h"

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
// CSeisMasonryMFactorDlg dialog


CSeisMasonryMFactorDlg::CSeisMasonryMFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisMasonryMFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisMasonryMFactorDlg)
	m_nOption = 0;
	m_nMembType = 0;
	//}}AFX_DATA_INIT
}


void CSeisMasonryMFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisMasonryMFactorDlg)
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_MEMB_TYPE1_RDO, m_nMembType);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_IO_EDT, m_Value1[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_LS_EDT, m_Value1[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_CP_EDT, m_Value1[2]);
	//}}AFX_DATA_MAP
}

void CSeisMasonryMFactorDlg::Execute()
{

}

BEGIN_MESSAGE_MAP(CSeisMasonryMFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisMasonryMFactorDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisMasonryMFactorDlg message handlers

BOOL CSeisMasonryMFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	InitCtrl();
	
	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisMasonryMFactorDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_K ElemK=NULL;
	CArray<T_ELEM_K, T_ELEM_K> aFiltedSelKey;

	int nMembType = D_MBTP_BRACE;
	int nElemType = TRUSS_EL;

	T_ELEM_D ElemD;
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		ElemK = aSelKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl2->ExistMisa(ElemK)) continue;
		if(nMembType!=m_pDoc->m_pAttrCtrl->GetMemberType(ElemK))  { ASSERT(0); continue; }
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if(ElemD.eltyp != nElemType) { ASSERT(0); continue; }

		aFiltedSelKey.Add(ElemK);
	}

	if (aFiltedSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_Selected_Member));
		return;
	}

	T_MFAC_D data;
	data.Initialize();

	for(int i=0; i<3; i++)
	{
		data.dMFactor1[i] = m_Value1[i].GetEditValue();
	}

	BOOL bSuccess=FALSE;
	if (m_nOption == 0) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddMfac(aFiltedSelKey, data);	
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelMfac(aFiltedSelKey);

	if (m_nOption == 0)
	{
		if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_KISTEC_Series())
		{
			IUsageCounter::Use(_T("SPE_MFMK"));
		}
		else if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_MFMM"));

		}
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CSeisMasonryMFactorDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CSeisMasonryMFactorDlg::OnChangeOption() 
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

void CSeisMasonryMFactorDlg::EnableValueEdit(BOOL bEnable)
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].EnableWindow(bEnable);
	}
}

void CSeisMasonryMFactorDlg::InitCtrl()
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].SetUnitType(D_UNITSYS_NONE);
		m_Value1[i].SetEditUnit(2.0);
	}
}