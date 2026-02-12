// SeisStlMFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisStlMFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
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
// CSeisStlMFactorDlg dialog


CSeisStlMFactorDlg::CSeisStlMFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisStlMFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisStlMFactorDlg)
	m_nOption = 0;
	m_nMembType = 0;
	//}}AFX_DATA_INIT
}


void CSeisStlMFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisStlMFactorDlg)
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_MEMB_TYPE1_RDO, m_nMembType);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_GRB, m_Title1);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_GRB, m_Title2);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_GRB, m_Title3);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_IO_STC, m_Text2[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_LS_STC, m_Text2[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_CP_STC, m_Text2[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_IO_STC, m_Text3[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_LS_STC, m_Text3[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_CP_STC, m_Text3[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_IO_EDT, m_Value1[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_LS_EDT, m_Value1[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_CP_EDT, m_Value1[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_IO_EDT, m_Value2[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_LS_EDT, m_Value2[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_CP_EDT, m_Value2[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_IO_EDT, m_Value3[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_LS_EDT, m_Value3[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_CP_EDT, m_Value3[2]);
	//}}AFX_DATA_MAP
}

void CSeisStlMFactorDlg::Execute()
{

}

BEGIN_MESSAGE_MAP(CSeisStlMFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisStlMFactorDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_MEMB_TYPE1_RDO, OnChangeType)
	ON_BN_CLICKED(IDC_MEMB_TYPE2_RDO, OnChangeType)
	ON_BN_CLICKED(IDC_MEMB_TYPE3_RDO, OnChangeType)
	ON_BN_CLICKED(IDC_MEMB_TYPE4_RDO, OnChangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisStlMFactorDlg message handlers

BOOL CSeisStlMFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	m_nMembType = 0;
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	InitCtrl();
	ChangeType(m_nMembType);
	
	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisStlMFactorDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	int nMembType=0;
	int nElemType=0;
	switch (m_nMembType)
	{
	case 0:
		nMembType = D_MBTP_BEAM;
		nElemType = BEAM_EL;
		break;
	case 1:
		nMembType = D_MBTP_COLUMN;
		nElemType = BEAM_EL;
		break;
	case 2:
		nMembType = D_MBTP_BRACE;
		nElemType = BEAM_EL;
		break;
	case 3:
		nMembType = D_MBTP_BRACE;
		nElemType = TRUSS_EL;
		break;
	default:
		break;
	}

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	T_ELEM_K ElemK=NULL;
	T_ELEM_D ElemD;
	T_MATL_D MatlD;
	CArray<T_ELEM_K, T_ELEM_K> aFiltedSelKey;
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		ElemK = aSelKey.GetAt(i);
		if(nMembType!=m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)) continue;
		if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if(ElemD.eltyp != nElemType) continue;
		if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
		if(MatlD.Type != _T("S")) continue;
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
		data.dMFactor2[i] = m_Value2[i].GetEditValue();
		data.dMFactor3[i] = m_Value3[i].GetEditValue();
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
			IUsageCounter::Use(_T("SPE_MFSK"));
		}
		else if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_MFSM"));
		}
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CSeisStlMFactorDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}


void CSeisStlMFactorDlg::OnChangeOption() 
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

void CSeisStlMFactorDlg::EnableValueEdit(BOOL bEnable)
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].EnableWindow(bEnable);
		m_Value2[i].EnableWindow(bEnable);
		m_Value3[i].EnableWindow(bEnable);
	}
}

void CSeisStlMFactorDlg::OnChangeType() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	ChangeType(m_nMembType);
}

void CSeisStlMFactorDlg::ChangeType(int nMembType)
{
	m_Title2.ShowWindow(nMembType!=0);
	m_Title3.ShowWindow(nMembType==1 || nMembType==2);
	for (int i = 0; i < 3; i++)
	{
		m_Text2[i].ShowWindow(nMembType!=0);
		m_Value2[i].ShowWindow(nMembType!=0);
		m_Text3[i].ShowWindow(nMembType==1 || nMembType==2);
		m_Value3[i].ShowWindow(nMembType==1 || nMembType==2);
	}

	switch (nMembType)
	{
	case 0:
		m_Title1.SetWindowText(_LS(IDS_TM_Flexure));
		break;
	case 1:
	case 2:
		m_Title1.SetWindowText(_LS(IDS_TM_Tension));
		m_Title2.SetWindowText(_LS(IDS_TM_Flexure_y));
		m_Title3.SetWindowText(_LS(IDS_TM_Flexure_z));
		break;
	case 3:
		m_Title1.SetWindowText(_LS(IDS_TM_Tension));
		m_Title2.SetWindowText(_LS(IDS_TM_Compression));
		break;
	default:
		break;
	}

	CWnd *pWnd = NULL;

	switch (nMembType)
	{
	case 0:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR1_GRB);
		break;
	case 1:
	case 2:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR3_GRB);
		break;
	case 3:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR2_GRB);
		break;
	default:
		break;
	}
		
	ASSERT(pWnd);

	CRect rRef, rToMove, rResize;
	pWnd->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_M_FACTOR_GRB)->GetWindowRect(rResize);
	rResize.bottom = rRef.bottom+globalUtils.ScaleByDPI(8);
	ScreenToClient(rResize);
	GetDlgItem(IDC_TM_M_FACTOR_GRB)->MoveWindow(&rResize);

	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rToMove);
	int nDist = rRef.bottom - rToMove.top+ globalUtils.ScaleByDPI(16);
	CArray<UINT, UINT> aCtrlMode;
	aCtrlMode.RemoveAll();
	aCtrlMode.Add(IDC_TM_EXECUTE);
	aCtrlMode.Add(IDC_TM_CLOSE);
	CDlgUtil::CtrlMoveDistY(this, aCtrlMode, nDist);
}

void CSeisStlMFactorDlg::InitCtrl()
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].SetUnitType(D_UNITSYS_NONE);
		m_Value2[i].SetUnitType(D_UNITSYS_NONE);
		m_Value3[i].SetUnitType(D_UNITSYS_NONE);

		m_Value1[i].SetEditUnit(2.0);
		m_Value2[i].SetEditUnit(2.0);
		m_Value3[i].SetEditUnit(2.0);
	}
}