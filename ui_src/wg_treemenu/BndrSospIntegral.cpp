// BndrSospIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSospIntegral.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSospIntegral dialog


CBndrSospIntegral::CBndrSospIntegral(SpringIntegral* pData,CWnd* pParent /*=NULL*/)
	: CDlgChild(CBndrSospIntegral::IDD, pParent)
{
	m_pData=pData;
	//{{AFX_DATA_INIT(CBndrSospIntegral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBndrSospIntegral::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSospIntegral)
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_WIDTH_UNIT, m_wndWidthUnit);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_WIDTH_EDIT, m_wndWidth);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE_COMBO, m_CobxElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_TYPE_SOLID_CBO, m_cboElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_VOID_EDIT, m_editVoid);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_UNIT_UNIT, m_unitUnit);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_UNIT_EDIT, m_editUnit);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_ABUT_EDIT, m_edtAbut);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_GROUND_EDIT, m_edtGround);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_DECK_EDIT, m_edtDeck);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_ABUT_UNIT, m_unitAbut);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_GROUND_UNIT, m_unitGround);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_DECK_UNIT, m_unitDeck);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_INCRE_EDIT, m_edtIncre);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_INCRE_UNIT, m_unitIncre);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_DIR_COMBO, m_cboDirection);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_MATERIAL_CBO, m_cboMatl);
	DDX_Control(pDX, IDC_TM_BNDR_SOSP_THERMAL_EDIT, m_edtThermal);  

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrSospIntegral, CDlgChild)
	//{{AFX_MSG_MAP(CBndrSospIntegral)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE_COMBO, OnSelchangeTmElemTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SOSP_TYPE_SOLID_CBO, OnSelchangeTmBndrSospTypeSolidCbo)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SOSP_MATERIAL_CBO, OnSelchangeTmBndrSospMaterialCbo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrSospIntegral message handlers

BOOL CBndrSospIntegral::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	m_CobxElemType.SetCurSel(1);
	CheckMode(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CBndrSospIntegral::InitSubCtrl()
{
	m_wndWidthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_unitAbut.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitGround.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitDeck.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_unitIncre.SetUnitType(D_UNITSYS_BASE_DEGREE);
	

	m_wndWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtAbut.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtGround.SetAttWcsPos();
	m_edtGround.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);  // Default : Global (-Z)
	m_edtGround.SetWindowText(_T("0"));

	m_edtDeck.SetAttUcsDistance();
	m_edtDeck.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtDeck.SetDistValueMode(TRUE);
	m_edtDeck.SetNextLink(NULL);
	m_edtIncre.SetUnitType(D_UNITSYS_BASE_DEGREE);
	// Default Value
	m_wndWidth.SetWindowText(_T("0"));
	m_editUnit.SetWindowText(_T("0"));
	m_edtAbut.SetWindowText(_T("0"));
	m_edtDeck.SetWindowText(_T("0"));
	m_edtIncre.SetWindowText(_T("0"));
	m_editVoid.SetWindowText(_T("0"));

	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_NORMAL_RDO);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_COMP_RDO);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_TENSON_RDO);
	
	
	CDlgUtil::CtrlRadioSetCheck(this,arType,0);

	m_CobxElemType.ResetContent();
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Frame)     );    m_CobxElemType.SetItemData(0,0);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Planar)    );    m_CobxElemType.SetItemData(1,1);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Face_));    m_CobxElemType.SetItemData(2,2);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Node_));    m_CobxElemType.SetItemData(3,3);
	m_CobxElemType.SetCurSel(1);


	// Element Type의 Face ComboBox 초기화.
	CString strElemType[] = {_LS(IDS_WG_TREEMENU_Face__1),_LS(IDS_WG_TREEMENU_Face__2),_LS(IDS_WG_TREEMENU_Face__3),_LS(IDS_WG_TREEMENU_Face__4),_LS(IDS_WG_TREEMENU_Face__5),_LS(IDS_WG_TREEMENU_Face__6)};
	for(int i=0; i<6; i++)
		m_cboElemType.AddString(strElemType[i]);
	m_cboElemType.SetCurSel(0);

		// Direction ComboBox 초기화.
	CString strDirection[] = {_LS(IDS_WG_TREEMENU_Normal___),_LS(IDS_WG_TREEMENU_Normal____MSG1) };
	for(int i = 0; i< 2; i++)
		m_cboDirection.AddString(strDirection[i]);
	m_cboDirection.SetCurSel(0);

	

	MakeMatlCombo();
	m_edtThermal.SetWindowText(_T("0"));
	
}
BOOL CBndrSospIntegral::GetParameter()
{
	CArray<UINT,UINT> arType;
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_NORMAL_RDO);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_COMP_RDO);
	arType.Add(IDC_TM_BNDR_SOSP_TYPE_TENSON_RDO);
	

	CDlgUtil::CtrlRadioGetCheck(this,arType,m_pData->nNonType);

	m_pData->nElType = (int)m_CobxElemType.GetItemData(m_CobxElemType.GetCurSel());
	m_pData->nFace = m_cboElemType.GetCurSel()+1;
	m_pData->dWidth = m_wndWidth.GetEditValue();

	
	CFormulaEdit::GetEditValue(&m_editVoid, m_pData->dVoid);
	m_pData->dWeight = m_editUnit.GetEditValue();
	m_pData->dAbut = m_edtAbut.GetEditValue();

	if (!CFormulaEdit::GetEditValue(&m_edtGround, m_pData->dGround))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
		return FALSE;
	}

	if(!CFormulaEdit::GetEditValue(&m_edtDeck, m_pData->dDeck)) return FALSE;

	m_pData->dIncre = m_edtIncre.GetEditValue();

	m_pData->nDirection = 1;
	switch(m_cboDirection.GetCurSel())
	{
		case 0:
			m_pData->nDirection = 1;
			break;
		case 1:
			m_pData->nDirection = 2;
			break;
		default:
			ASSERT(0);
	}
	
	if(!CFormulaEdit::GetEditValue(&m_edtThermal, m_pData->dThermal)) return FALSE;
	
	return TRUE;
}
BOOL CBndrSospIntegral::GetDirectionVector(CString &strVector, double& Ux, double& Uy, double& Uz)
{
	return GetEQDistance(strVector,Ux,Uy,Uz);
}
BOOL CBndrSospIntegral::GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz)
{
	MyParser Parser;
	
	if(Parser.ParsingPositionArg(EQStr))
	{
		Parser.GetPositionData(Dx,Dy,Dz);
	}
	else
	{
		 Dx = 0. ; Dy = 0. ; Dz = 0.;  
		 return FALSE;
	}
	return TRUE;
}
void CBndrSospIntegral::CheckMode(BOOL bCheck)
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

void CBndrSospIntegral::OnSelchangeTmElemTypeCombo() 
{
	CheckMode(TRUE);
}
void CBndrSospIntegral::OnSelchangeTmBndrSospTypeSolidCbo() 
{
	CheckMode(TRUE);	
}

void CBndrSospIntegral::OnSelchangeTmBndrSospMaterialCbo() 
{
	T_MATL_K MatlK;
	T_MATL_D MatlData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	MatlK = (UINT)m_cboMatl.GetItemData(m_cboMatl.GetCurSel());
	if(MatlK==0)
	{
		m_edtThermal.EnableWindow(TRUE);
	}
	else
	{
		m_edtThermal.EnableWindow(FALSE);
		if(!pDoc->m_pAttrCtrl->GetMatl(MatlK,MatlData)) ASSERT(0);
		CString strTemp;
		strTemp.Format(_T("%g"),MatlData.Data1.Analysis.Thermal);
		m_edtThermal.SetWindowText(strTemp);
	}

	
}
void CBndrSospIntegral::PostNcDestroy() 
{
	CheckMode(FALSE);
	CDlgChild::PostNcDestroy();
}

void CBndrSospIntegral::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bMATL = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_MATL_ADD:
		case UR_MATL_DEL:
		case UR_MATL_MFD:
		case UR_MATL_MFS:
			bMATL = TRUE;
			goto Exit;
			break;
		default:
			break;
		}
	}
Exit:
	if(bMATL)
		MakeMatlCombo();
}
void CBndrSospIntegral::MakeMatlCombo()
{
	CArray<T_MATL_K,T_MATL_K> arMatl;
	T_MATL_D MatlData;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetMatlKeyList(arMatl);
	m_cboMatl.ResetContent();
	m_cboMatl.AddString(_T("None")); m_cboMatl.SetItemData(0,0);
	for(int i=0;i<arMatl.GetSize();i++)
	{
		if(!pDoc->m_pAttrCtrl->GetMatl(arMatl[i],MatlData)) ASSERT(0);
		m_cboMatl.AddString(MatlData.Name); m_cboMatl.SetItemData(i+1,arMatl[i]);
	}
	m_cboMatl.SetCurSel(0);
}