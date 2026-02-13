// DgnConCondiFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCondiFactorDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCondiFactorDlg dialog


CDgnConCondiFactorDlg::CDgnConCondiFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConCondiFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCondiFactorDlg)
	//}}AFX_DATA_INIT

	m_nOption   = 0;
	m_bmb6      = FALSE;
	m_nPosmb6   = 0;
	m_nmb6Type  = 0;
	m_bmb10     = FALSE;
	m_nPosmb10  = 0; 
	m_nmb10Type = 0;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConCondiFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCondiFactorDlg)
	DDX_Radio  (pDX, IDC_DGN_CONDI_FACTOR_AR_RDO,         m_nOption);
	DDX_Check  (pDX, IDC_DGN_CONDI_FACTOR_MB6_CHK,        m_bmb6);
	DDX_Radio  (pDX, IDC_DGN_CONDI_FACTOR_MB6_POS_I_RDO,  m_nPosmb6);
	DDX_Radio  (pDX, IDC_DGN_CONDI_FACTOR_MB6_RDO1,       m_nmb6Type);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_SIG_BY_EDT,     m_edtSigby);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_SIG_BY_UNT,     m_untSigby);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_MB6_EDT,        m_edtmb6);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_MB6_S_EDT,      m_edtmb6_s);
	DDX_Check  (pDX, IDC_DGN_CONDI_FACTOR_MB10_CHK,       m_bmb10);
	DDX_Radio  (pDX, IDC_DGN_CONDI_FACTOR_MB10_POS_I_RDO, m_nPosmb10);
	DDX_Radio  (pDX, IDC_DGN_CONDI_FACTOR_MB10_RDO1,      m_nmb10Type);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_THICK_EDT,      m_edtThick);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_THICK_UNT,      m_untThick);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_RBJ_EDT,        m_edtRbj);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_RBJ_UNT,        m_untRbj);
	DDX_Control(pDX, IDC_DGN_CONDI_FACTOR_MB10_EDT,       m_edtmb10);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCondiFactorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConCondiFactorDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,   OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)

	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_AR_RDO,    OnDgnCondiFactorAddDelRdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_DEL_RDO,   OnDgnCondiFactorAddDelRdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB6_CHK,   OnDgnCondiFactormb6Chk)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB6_RDO1,  OnDgnCondiFactormb6Rdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB6_RDO2,  OnDgnCondiFactormb6Rdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB10_CHK,  OnDgnCondiFactormb10Chk)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB10_RDO1, OnDgnCondiFactormb10Rdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB10_RDO2, OnDgnCondiFactormb10Rdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB10_RDO3, OnDgnCondiFactormb10Rdo)
	ON_BN_CLICKED(IDC_DGN_CONDI_FACTOR_MB10_RDO4, OnDgnCondiFactormb10Rdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCondiFactorDlg message handlers
void CDgnConCondiFactorDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConCondiFactorDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{		
		T_OCDF_D rData; rData.Initialize();

		if(m_nOption==0)	// Add/Replace
		{
			rData.bmb6 = m_bmb6;
			rData.nPosmb6 = m_nPosmb6;
			rData.nmb6Type = m_nmb6Type;
			rData.dSigby = m_edtSigby.GetEditValue();
			rData.dmb6 = m_edtmb6.GetEditValue();
			rData.dmb6_s = m_edtmb6_s.GetEditValue();
			rData.bmb10 = m_bmb10;
			rData.nPosmb10 = m_nPosmb10;
			rData.nmb10Type = m_nmb10Type;
			rData.dThick = m_edtThick.GetEditValue();
			rData.dRbj = m_edtRbj.GetEditValue();
			rData.dmb10 = m_edtmb10.GetEditValue();

			if(aSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddOcdf(aSelKey, rData))
				{
					if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
				}
			}
			else	AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
		}
		else	// Delete
		{
			if(m_pDoc->m_pDataCtrl->DelOcdf(aSelKey))
			{
				if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		} 
		
		EnableDisableControl();
	}
	else	AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
}

BOOL CDgnConCondiFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();
	EnableDisableControl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCondiFactorDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// 
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// 
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				EnableDisableControl();
				break;
			}
		case D_UPDATE_UNIT:
			// 
			break;
		default:
			ASSERT(TRUE);
	}
}

void CDgnConCondiFactorDlg::InitUnit()
{
	m_edtSigby.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untSigby.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtmb6  .SetUnitType(D_UNITSYS_NONE);
	m_edtmb6_s.SetUnitType(D_UNITSYS_NONE);
	m_edtThick.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untThick.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRbj  .SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untRbj  .SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtmb10 .SetUnitType(D_UNITSYS_NONE);
}

void CDgnConCondiFactorDlg::EnableDisableControl()
{
	CString strDgnCode = CDBLib::GetConCodeName();

	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	
	BOOL bEnable = FALSE;
	if(strDgnCode == CONCODE_SNIP_2_05_03_84 || strDgnCode == CONCODE_SP_35_13330_2011 ||
		 strDgnCode == CONCODE_SNIP_2_05_03_84_MKS || strDgnCode == CONCODE_SP_35_13330_2011_MKS ||
		 PscdD.iDgnCode==SNiP_20503_84_PSC || PscdD.iDgnCode==SP_35_13330_11_PSC || PscdD.iDgnCode==SNiP_20503_84_PSC_MKS || PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		bEnable = TRUE;
	}
	
	GetDlgItem(IDC_DGN_CONDI_FACTOR_OPT_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_AR_RDO )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_DEL_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_CHK       )->EnableWindow(bEnable && m_nOption==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_POS_STC   )->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_POS_I_RDO )->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_POS_J_RDO )->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_POS_IJ_RDO)->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_RDO1      )->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_RDO2      )->EnableWindow(bEnable && m_nOption==0 && m_bmb6);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_SIG_BY_EDT    )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_SIG_BY_UNT    )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_STC       )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==1);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_EDT       )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==1);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_S_STC     )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==1);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB6_S_EDT     )->EnableWindow(bEnable && m_nOption==0 && m_bmb6 && m_nmb6Type==1);

	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_CHK       )->EnableWindow(bEnable && m_nOption==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_POS_STC   )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_POS_I_RDO )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_POS_J_RDO )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_POS_IJ_RDO)->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_RDO1      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_RDO2      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_RDO3      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_RDO4      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_THICK_STC      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_THICK_EDT      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_THICK_UNT      )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_RBJ_STC        )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_RBJ_EDT        )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_RBJ_UNT        )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==0);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_STC       )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==3);
	GetDlgItem(IDC_DGN_CONDI_FACTOR_MB10_EDT       )->EnableWindow(bEnable && m_nOption==0 && m_bmb10 && m_nmb10Type==3);
}

void CDgnConCondiFactorDlg::OnDgnCondiFactorAddDelRdo()
{
	UpdateData(TRUE);
	EnableDisableControl();
}

void CDgnConCondiFactorDlg::OnDgnCondiFactormb6Chk()
{
	UpdateData(TRUE);
	EnableDisableControl();
}

void CDgnConCondiFactorDlg::OnDgnCondiFactormb6Rdo()
{
	UpdateData(TRUE);
	EnableDisableControl();	
}

void CDgnConCondiFactorDlg::OnDgnCondiFactormb10Chk()
{
	UpdateData(TRUE);
	EnableDisableControl();
}

void CDgnConCondiFactorDlg::OnDgnCondiFactormb10Rdo()
{
	UpdateData(TRUE);
	EnableDisableControl();	
}
