// SeisRcMFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisRcMFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\ArrayUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisRcMFactorDlg dialog

enum MFAC_TYPE
{
	TYPE_BEAM=0,
	TYPE_COLM,
	TYPE_COLM_M,
	TYPE_WALL
};

CSeisRcMFactorDlg::CSeisRcMFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisRcMFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisRcMFactorDlg)
	m_nOption = 0;
	m_nMembType = 0;
	//}}AFX_DATA_INIT
}


void CSeisRcMFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisRcMFactorDlg)
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_MEMB_TYPE1_RDO, m_nMembType);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_GRB, m_Title1);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_GRB, m_Title2);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_GRB, m_Title3);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_GRB, m_Title4);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_IO_STC, m_Text2[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_LS_STC, m_Text2[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_CP_STC, m_Text2[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_IO_STC, m_Text3[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_LS_STC, m_Text3[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_CP_STC, m_Text3[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_IO_STC, m_Text4[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_LS_STC, m_Text4[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_CP_STC, m_Text4[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_IO_EDT, m_Value1[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_LS_EDT, m_Value1[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR1_CP_EDT, m_Value1[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_IO_EDT, m_Value2[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_LS_EDT, m_Value2[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR2_CP_EDT, m_Value2[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_IO_EDT, m_Value3[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_LS_EDT, m_Value3[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR3_CP_EDT, m_Value3[2]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_IO_EDT, m_Value4[0]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_LS_EDT, m_Value4[1]);
	DDX_Control(pDX, IDC_TM_M_FACTOR4_CP_EDT, m_Value4[2]);
	//}}AFX_DATA_MAP
}

void CSeisRcMFactorDlg::Execute()
{

}

BEGIN_MESSAGE_MAP(CSeisRcMFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisRcMFactorDlg)
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
// CSeisRcMFactorDlg message handlers

BOOL CSeisRcMFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	m_nMembType = 0;
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	if(m_pDoc->m_pAttrCtrl2->GetSemeMethod()==D_SEME_METHOD_KISTEC2013)
	{
		GetDlgItem(IDC_MEMB_TYPE3_RDO)->SetWindowText(_LS(IDS_TB_MBTP_Wall));
		GetDlgItem(IDC_MEMB_TYPE4_RDO)->ShowWindow(SW_HIDE);

		CRect rRef, rToMove, rResize;
		GetDlgItem(IDC_MEMB_TYPE4_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_MEMB_TYPE_GRB)->GetWindowRect(rResize);
		int nBefore = rResize.bottom;
		rResize.bottom = rRef.top+globalUtils.ScaleByDPI(2);
		int nDistY = rResize.bottom - nBefore;
		ScreenToClient(rResize);
		GetDlgItem(IDC_TM_MEMB_TYPE_GRB)->MoveWindow(&rResize);

		CArray<UINT, UINT> aCtrl;
		CDlgUtil::GetCtrlIDByIncRect(this, aCtrl, IDC_TM_M_FACTOR_GRB, TRUE);
		aCtrl.Add(IDC_TM_EXECUTE);
		aCtrl.Add(IDC_TM_CLOSE);
		CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
	}

	InitCtrl();
	ChangeType(m_nMembType);
	
	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisRcMFactorDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	BOOL bColmM = FALSE;
	int nMembType=0;
	int nType = GetType(m_nMembType);
	switch (nType)
	{
	case TYPE_BEAM:
		nMembType = D_MBTP_BEAM;
		break;
	case TYPE_COLM:
		nMembType = D_MBTP_COLUMN;
		break;
	case TYPE_COLM_M:
		nMembType = D_MBTP_COLUMN;
		bColmM = TRUE;
		break;
	case TYPE_WALL:
		nMembType = D_MBTP_WALL;
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
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
		if(MatlD.Type!=_T("C")) continue;
		aFiltedSelKey.Add(ElemK);
	}

	if(bColmM)
	{
		CArray<T_ELEM_K,T_ELEM_K> aElemK;
		CSeisEvalDataTool Tool;
		Tool.GetEvalInfilledConColmKeyList(aElemK);

		CArray<T_ELEM_K,T_ELEM_K> aColmMElemK;
		CArrayUtil::IntersectUINT(aFiltedSelKey, aElemK, aColmMElemK);
		aFiltedSelKey.RemoveAll();
		aFiltedSelKey.Copy(aColmMElemK);
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
		data.dMFactor4[i] = m_Value4[i].GetEditValue();
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
			IUsageCounter::Use(_T("SPE_MFCK"));
		}
		else if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_MFCM"));
		}
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CSeisRcMFactorDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}


void CSeisRcMFactorDlg::OnChangeOption() 
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

void CSeisRcMFactorDlg::EnableValueEdit(BOOL bEnable)
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].EnableWindow(bEnable);
		m_Value2[i].EnableWindow(bEnable);
		m_Value3[i].EnableWindow(bEnable);
		m_Value4[i].EnableWindow(bEnable);
	}
}

void CSeisRcMFactorDlg::OnChangeType() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	ChangeType(m_nMembType);
}

void CSeisRcMFactorDlg::ChangeType(int nMembType)
{
	int nType = GetType(nMembType);

	m_Title2.ShowWindow(nType==TYPE_COLM || nType==TYPE_COLM_M);
	m_Title3.ShowWindow(nType==TYPE_COLM_M);
	m_Title4.ShowWindow(nType==TYPE_COLM_M);
	for (int i = 0; i < 3; i++)
	{
		m_Text2[i].ShowWindow(nType==TYPE_COLM || nType==TYPE_COLM_M);
		m_Text3[i].ShowWindow(nType==TYPE_COLM_M);
		m_Text4[i].ShowWindow(nType==TYPE_COLM_M);
		m_Value2[i].ShowWindow(nType==TYPE_COLM || nType==TYPE_COLM_M);
		m_Value3[i].ShowWindow(nType==TYPE_COLM_M);
		m_Value4[i].ShowWindow(nType==TYPE_COLM_M);
	}

	CWnd *pWnd = NULL;
	switch (nType)
	{
	case TYPE_BEAM:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR1_GRB);
		m_Title1.SetWindowText(_LS(IDS_TM_Flexure));
		break;
	case TYPE_COLM:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR2_GRB);
		m_Title1.SetWindowText(_LS(IDS_TM_Flexure_y));
		m_Title2.SetWindowText(_LS(IDS_TM_Flexure_z));
		break;
	case TYPE_COLM_M:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR4_GRB);
		m_Title1.SetWindowText(_LS(IDS_TM_Tension));
		m_Title2.SetWindowText(_LS(IDS_TM_Compression));
		m_Title3.SetWindowText(_LS(IDS_TM_Flexure_y));
		m_Title4.SetWindowText(_LS(IDS_TM_Flexure_z));
		break;
	case TYPE_WALL:
		pWnd = GetDlgItem(IDC_TM_M_FACTOR1_GRB);
		m_Title1.SetWindowText(_LS(IDS_TM_Flexure_Shear));
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

void CSeisRcMFactorDlg::InitCtrl()
{
	for (int i = 0; i < 3; i++)
	{
		m_Value1[i].SetUnitType(D_UNITSYS_NONE);
		m_Value2[i].SetUnitType(D_UNITSYS_NONE);
		m_Value3[i].SetUnitType(D_UNITSYS_NONE);
		m_Value4[i].SetUnitType(D_UNITSYS_NONE);

		m_Value1[i].SetEditUnit(2.0);
		m_Value2[i].SetEditUnit(2.0);
		m_Value3[i].SetEditUnit(2.0);
		m_Value4[i].SetEditUnit(2.0);
	}
}

int CSeisRcMFactorDlg::GetType(int nMembType)
{
	int nType = nMembType;
	BOOL bKISTEC2013 = m_pDoc->m_pAttrCtrl2->GetSemeMethod()==D_SEME_METHOD_KISTEC2013;
	if(bKISTEC2013 && nMembType==2)
	{
		nType = MFAC_TYPE::TYPE_WALL;
	}
	return nType;
}