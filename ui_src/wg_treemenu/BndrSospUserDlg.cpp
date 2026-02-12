// BndrSospUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSospUserDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_StrParser.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSospUserDlg dialog


CBndrSospUserDlg::CBndrSospUserDlg(SpringUser* pData,CWnd* pParent /*=NULL*/)
	: CDlgChild(CBndrSospUserDlg::IDD, pParent)
{
	m_pData=pData;

	//{{AFX_DATA_INIT(CBndrSospUserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBndrSospUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSospUserDlg)
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_WIDTH_UNIT, m_wndWidthUnit);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_WIDTH_EDIT, m_wndWidth);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE_COMBO, m_CobxElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_TYPE_SOLID_CBO, m_cboElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_STIFF_UNIT, m_unitStiff);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_STIFF, m_edtStiff);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_DIRECT_CMB, m_cboDirect);
	
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_DIST_CHK, m_chkDist);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_GRA_CMB, m_cboGra);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_REF_UNIT, m_unitReference);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_REF_EDIT, m_edtReference);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_GRADIENT_EDIT, m_edtGradientStiff);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_GRADIENT_UNIT, m_unitGradientStiff);

	DDX_Control(pDX, IDC_TM_BNDR_SOSP_VECTOR, m_edtVector);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrSospUserDlg, CDlgChild)
	//{{AFX_MSG_MAP(CBndrSospUserDlg)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE_COMBO, OnSelchangeTmElemTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SOSP_TYPE_SOLID_CBO, OnSelchangeTmBndrSospTypeSolidCbo)
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_DIST_CHK, OnTmBndrSospDistChk)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SOSP_GRA_CMB, OnChangeGradDir)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SOSP_DIRECT_CMB, OnSelchangeTmDirectCombo)
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_TYPE_NORMAL, OnChangeType)
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_TYPE_TENSION, OnChangeType)
	ON_BN_CLICKED(IDC_TM_BNDR_SOSP_TYPE_COMP, OnChangeType)
	

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrSospUserDlg message handlers

BOOL CBndrSospUserDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	

	m_CobxElemType.SetCurSel(1);
	CheckMode(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CBndrSospUserDlg::InitSubCtrl()
{
	m_wndWidthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitStiff.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_unitReference.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGradientStiff.SetUnitType(D_UNITSYS_BASE_STIFFGRADIENT);

	m_edtReference.SetAttWcsPos();
	m_edtReference.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);  // Default : Global (-Z)
	m_edtReference.SetWindowText(_T("0"));
	m_edtGradientStiff.SetUnitType(D_UNITSYS_BASE_STIFFGRADIENT);
	m_wndWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStiff.SetUnitType(D_UNITSYS_BASE_DENSITY);

	
	m_CobxElemType.ResetContent();
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Frame)     );    m_CobxElemType.SetItemData(0,0);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Planar)    );    m_CobxElemType.SetItemData(1,1);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Face_));    m_CobxElemType.SetItemData(2,2);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Node_));    m_CobxElemType.SetItemData(3,2);
	m_CobxElemType.SetCurSel(1);

	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_NORMAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_COMP);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_TENSION);
	
	CDlgUtil::CtrlRadioSetCheck(this,arType,0);

	InitDirectionCombo();
	
	int nCur =0;
	m_cboDirect.SetCurSel(nCur);
	m_edtVector.EnableWindow(FALSE);

	m_edtVector.SetAttUcsDistance();
	m_edtVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);


	// Element Type의 Face ComboBox 초기화.
	CString strElemType[] = {_LS(IDS_WG_TREEMENU_Face__1),_LS(IDS_WG_TREEMENU_Face__2),_LS(IDS_WG_TREEMENU_Face__3),_LS(IDS_WG_TREEMENU_Face__4),_LS(IDS_WG_TREEMENU_Face__5),_LS(IDS_WG_TREEMENU_Face__6)};
	for(int i=0; i<6; i++)
		m_cboElemType.AddString(strElemType[i]);
	m_cboElemType.SetCurSel(0);  
	
	// Default Value

	m_wndWidth.SetWindowText(_T("0"));

	m_chkDist.SetCheck(TRUE);

	m_cboGra.AddString(_LS(IDS_WG_TREEMENU_Global___X_));
	m_cboGra.AddString(_LS(IDS_WG_TREEMENU_Global___Y_));
	m_cboGra.AddString(_LS(IDS_WG_TREEMENU_Global___Z_));
	m_cboGra.SetCurSel(2);  // Default : Global (-Z)

}

BOOL CBndrSospUserDlg::GetParameter()
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_NORMAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_COMP);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_TENSION);
	

	CDlgUtil::CtrlRadioGetCheck(this,arType,m_pData->nNonType);
	m_pData->ElType = (int)m_CobxElemType.GetItemData(m_CobxElemType.GetCurSel());
	m_pData->nFace = m_cboElemType.GetCurSel()+1;
	
	if(m_pData->ElType == 0)
		m_pData->dWidth = m_wndWidth.GetEditValue();
 
	m_pData->nSpringDirect = m_cboDirect.GetItemData(m_cboDirect.GetCurSel());
	m_pData->dStiff = m_edtStiff.GetEditValue();
	if(m_pData->nSpringDirect == 8)
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		CString csVector;
		double ux, uy, uz;
		m_edtVector.GetWindowText(csVector);
		if (!GetPosition(csVector, ux, uy, uz))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
			return FALSE;
		}
		pIGM->GetWcsVector(ux, uy, uz, m_pData->dVx, m_pData->dVy, m_pData->dVz);
		if(m_pData->dVx == 0.0 && m_pData->dVy == 0.0 && m_pData->dVy == 0.0)
			return FALSE;
	}
	else
	{
		m_pData->dVx = m_pData->dVy = m_pData->dVz = 0.0;
	}
	
	m_pData->bDist = m_chkDist.GetCheck();
	if(m_pData->bDist)
	{
		if (!CFormulaEdit::GetEditValue(&m_edtReference, m_pData->dReference))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
			return FALSE;
		}
		m_pData->nGradientDirect = m_cboGra.GetCurSel();
		m_pData->dGradientStiff = m_edtGradientStiff.GetEditValue();
	}
	return TRUE;
}
void CBndrSospUserDlg::CheckMode(BOOL bCheck)
{
	CDBDoc* pDoc;
	if(bCheck)
	{
		UpdateData(TRUE);
		pDoc = CDBDoc::GetDocPoint();

		int nSel = m_CobxElemType.GetCurSel();

		if(nSel == 0) 
			m_wndWidth.EnableWindow(TRUE);
		else
			m_wndWidth.EnableWindow(FALSE);
		
		m_cboElemType.EnableWindow(nSel == 2);
		
		if(pDoc)
		{
			if(nSel >= 2)
			{
				int PresInputMode;
				if(nSel == 2) 
				{
					PresInputMode = 8; // Solid Face 
				}
				else
				{
					PresInputMode = 10; // Solid Face By Node
				}

				 pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
			   pDoc->m_pViewCtrl->SetPressureInputParam(PresInputMode, m_cboElemType.GetCurSel());	
			}
			else
			{
				pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
				pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
			}
		}
	}
	else
	{
		pDoc = CDBDoc::GetDocPoint();
		if(pDoc)
		{
		  pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
			pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

void CBndrSospUserDlg::OnSelchangeTmElemTypeCombo() 
{
	CheckMode(TRUE);
	InitDirectionCombo();
}
void CBndrSospUserDlg::OnSelchangeTmBndrSospTypeSolidCbo() 
{
	CheckMode(TRUE);	
}
void CBndrSospUserDlg::OnTmBndrSospDistChk() 
{
	CArray<UINT,UINT> arCtrls;
	arCtrls.Add(IDC_TM_BNDR_SOSP_GRA_TITLE);
	arCtrls.Add(IDC_TM_BNDR_SOSP_GRA_CMB);
	arCtrls.Add(IDC_TM_BNDR_SOSP_REF_TITLE);
	arCtrls.Add(IDC_TM_BNDR_SOSP_REF_EDIT);
	arCtrls.Add(IDC_TM_BNDR_SOSP_REF_UNIT);
	arCtrls.Add(IDC_TM_BNDR_SOSP_STIFF_GRA_TITLE);
	arCtrls.Add(IDC_TM_BNDR_SOSP_GRADIENT_EDIT);
	arCtrls.Add(IDC_TM_BNDR_SOSP_GRADIENT_UNIT);
	CDlgUtil::CtrlEnableDisable(this,arCtrls,m_chkDist.GetCheck());
	
}

void CBndrSospUserDlg::PostNcDestroy() 
{
	CheckMode(FALSE);
	CDlgChild::PostNcDestroy();
}
BOOL CBndrSospUserDlg::GetPosition(CString& strPos    ,double& Px ,double& Py ,double& Pz)
{
	MyParser Parser;
	if(Parser.ParsingPositionArg(strPos))
	{
		Parser.GetPositionData(Px,Py,Pz);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CBndrSospUserDlg::OnChangeGradDir() 
{
	// TODO: Add your control notification handler code here
	int nDir = m_cboGra.GetCurSel();
	if (nDir == 0) m_edtReference.SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);
	else if (nDir == 1) m_edtReference.SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
	else m_edtReference.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
}
void CBndrSospUserDlg::OnSelchangeTmDirectCombo() 
{
	int nSelected = m_cboDirect.GetCurSel();
	int nLast = m_cboDirect.GetCount() - 1;
	m_edtVector.EnableWindow(nSelected == nLast);
}

void CBndrSospUserDlg::InitDirectionCombo()
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_NORMAL);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_COMP);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_TENSION);
	
	int nElemType,nType;
	nElemType = m_CobxElemType.GetItemData(m_CobxElemType.GetCurSel());
	CDlgUtil::CtrlRadioGetCheck(this,arType,nType);

	if(nElemType == 0)
	{
		if(nType == 0)
		{
			m_cboDirect.ResetContent();
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_x)); m_cboDirect.SetItemData(0,0);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_y)); m_cboDirect.SetItemData(1,2);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_z)); m_cboDirect.SetItemData(2,4);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Vector)); m_cboDirect.SetItemData(3,8);
		}
		else
		{
			m_cboDirect.ResetContent();
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Plus_x)); m_cboDirect.SetItemData(0,0);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Minus_x)); m_cboDirect.SetItemData(1,1);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Plus_y)); m_cboDirect.SetItemData(2,2);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Minus_y)); m_cboDirect.SetItemData(3,3);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Plus_z)); m_cboDirect.SetItemData(4,4);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Local_Minus_z)); m_cboDirect.SetItemData(5,5);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Vector)); m_cboDirect.SetItemData(6,8);
		}
	}
	else
	{
		if(nType == 0)
		{
			m_cboDirect.ResetContent();
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Normal)); m_cboDirect.SetItemData(0,6);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Vector)); m_cboDirect.SetItemData(1,8);
		}
		else
		{
			m_cboDirect.ResetContent();
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Normal_Plus)); m_cboDirect.SetItemData(0,6);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Normal_Minus)); m_cboDirect.SetItemData(1,7);
			m_cboDirect.AddString(_LS(IDS_TM_NSPR_Vector)); m_cboDirect.SetItemData(2,8);
		}
	}
	m_cboDirect.SetCurSel(0);
}
void CBndrSospUserDlg::OnChangeType() 
{
	InitDirectionCombo();
}