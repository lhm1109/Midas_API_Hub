// BndrSospIntegral.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ResponseDispLoadMode1Dlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\src\wg_db\UndoCtrl.h"
#include "..\src\wg_db\EditData.h"
#include "BndrSsprDlg.h"
#include "..\wg_base\TreeMenuMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode1Dlg dialog


ResponseDispLoadMode1Dlg::ResponseDispLoadMode1Dlg(CWnd* pParent /*=NULL*/)
		: CMenuBarChildDlg(ResponseDispLoadMode1Dlg::IDD, pParent)
{

}


void ResponseDispLoadMode1Dlg::DoDataExchange(CDataExchange* pDX)
{
		CMenuBarChildDlg::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(ResponseDispLoadMode1Dlg)
		DDX_Control(pDX, IDC_EDIT_RSMM1_JC_RAT, m_EditRat);
		DDX_Control(pDX, IDC_EDIT_RSMM1_TX_LEN, m_EditLen);
		DDX_Control(pDX, IDC_EDIT_RSMM1_JC_KX , m_EditKx );
		DDX_Control(pDX, IDC_EDIT_RSMM1_JC_KY , m_EditKy );
		DDX_Control(pDX, IDC_EDIT_RSMM1_JC_KZ , m_EditKz );
		DDX_Control(pDX, IDC_EDIT_WIDTH       , m_EditWidth );
		//
		DDX_Control(pDX, IDC_STATIC_RSMM1_JC_RAT_UNIT, m_UnitRat);
		DDX_Control(pDX, IDC_STATIC_RSMM1_TX_LEN_UNIT, m_UnitLen);
		DDX_Control(pDX, IDC_STATIC_RSMM1_JC_KX_UNIT , m_UnitKx );
		DDX_Control(pDX, IDC_STATIC_RSMM1_JC_KY_UNIT , m_UnitKy );
		DDX_Control(pDX, IDC_STATIC_RSMM1_JC_KZ_UNIT , m_UnitKz );
		DDX_Control(pDX, IDC_UNIT_WIDTH , m_UnitWidth );
		//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResponseDispLoadMode1Dlg, CMenuBarChildDlg)
		//{{AFX_MSG_MAP(ResponseDispLoadMode1Dlg)
		//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDC_CHECK_RSMM1_CS, &ResponseDispLoadMode1Dlg::OnBnClickedCheckRsmm1Cs)
		ON_BN_CLICKED(IDC_CHECK_RSMM1_SS, &ResponseDispLoadMode1Dlg::OnBnClickedCheckRsmm1Ss)
		ON_CBN_SELCHANGE(IDC_COMBO_RSMM1_ELEMTYPE, &ResponseDispLoadMode1Dlg::OnCbnSelchangeComboRsmm1Elemtype)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode1Dlg message handlers

BOOL ResponseDispLoadMode1Dlg::OnInitDialog() 
{
		CMenuBarChildDlg::OnInitDialog();
		InitialData();
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
}

T_KEY ResponseDispLoadMode1Dlg::MakeGroup(CDBDoc *pDoc,LPCTSTR strName)
{
		T_BNGR_D data;
		data.Initialize();
		data.GroupName = strName;
		T_BNGR_K key = pDoc->m_pEditData->AddBngr(data, FALSE);
		return key;
}

void ResponseDispLoadMode1Dlg::InitialData()
{
		m_UnitRat.SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_UnitLen.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_UnitKx .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_UnitKy .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_UnitKz .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_UnitWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditRat.SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_EditLen.SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_EditKx .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_EditKy .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_EditKz .SetUnitType(D_UNITSYS_BASE_DENSITY);
		m_EditWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
		CComboBox *pcbxDir = (CComboBox *)GetDlgItem(IDC_COMBO_RSMM1_DIR);
		pcbxDir->ResetContent();

		CString strDirection[] = {_LS(IDS_WG_TREEMENU_Normal___),_LS(IDS_WG_TREEMENU_Normal____MSG1),_LS(IDS_WG_TREEMENU_UCS_x___),_LS(IDS_WG_TREEMENU_UCS_x____MSG2),
				_LS(IDS_WG_TREEMENU_UCS_y___) ,_LS(IDS_WG_TREEMENU_UCS_y____MSG3) ,_LS(IDS_WG_TREEMENU_UCS_z___),_LS(IDS_WG_TREEMENU_UCS_z____MSG4) };
		for (int i=0; i< 8; i++)
		{
				pcbxDir->AddString(strDirection[i]);
		}
		pcbxDir->SetCurSel(0);
		//
		CDlgUtilEx::CtrlCheck(this, IDC_CHECK_RSMM1_CS, TRUE);
		CDlgUtilEx::CtrlCheck(this, IDC_CHECK_RSMM1_SS, TRUE);
		//
		CComboBox *pcbxElemType = (CComboBox *)GetDlgItem(IDC_COMBO_RSMM1_ELEMTYPE);
		pcbxElemType->ResetContent();
		pcbxElemType->AddString(_LS(IDS_IDD_CMD_ML_MVCT_INDIA_IDC_WG_CMD_STATIC3));
		pcbxElemType->AddString(_LS(IDS_IDD_GR_DISP_COLOR_IDC_WG_GR_STATIC2));
		pcbxElemType->SetCurSel(1);
		OnCbnSelchangeComboRsmm1Elemtype();
}

int ResponseDispLoadMode1Dlg::GetDirection()
{
		CComboBox *pComb = (CComboBox *)GetDlgItem(IDC_COMBO_RSMM1_DIR);
		return pComb->GetCurSel();
}

int  ResponseDispLoadMode1Dlg::GetElemType()
{
		CComboBox *pComb = (CComboBox *)GetDlgItem(IDC_COMBO_RSMM1_ELEMTYPE);
		return pComb->GetCurSel();
}

BOOL ResponseDispLoadMode1Dlg::GetParameter_Shear(Sspring& Sspr)
{
		memset(&Sspr, 0, sizeof(Sspring));
		Sspr.nElType = GetElemType();
		Sspr.nFace   = 1;
		Sspr.nConvertType = 1;
		Sspr.dWidth = m_EditWidth.GetEditValue();
		Sspr.nSpringType = 1;
		if(Sspr.nElType == 0 && Sspr.dWidth < 1e-6)
				return FALSE;
		Sspr.dSx = m_EditKx.GetEditValue();
		Sspr.dSy = m_EditKy.GetEditValue();
		Sspr.dSz = m_EditKz.GetEditValue();
		if(fabs(Sspr.dSx) <1e-6 && fabs(Sspr.dSy)<1e-6&& fabs(Sspr.dSz)<1e-6)
				return FALSE;

		return TRUE;
}
BOOL ResponseDispLoadMode1Dlg::GetParameter_Compress(Sspring& Sspr)
{
		memset(&Sspr, 0, sizeof(Sspring));
		Sspr.nElType = GetElemType();
		Sspr.nFace   = 1;
		Sspr.nConvertType = 2;
		Sspr.dWidth = m_EditWidth.GetEditValue();
		
		Sspr.dSx = m_EditRat.GetEditValue();
		if(Sspr.nElType == 0 && Sspr.dWidth < 1e-6)
				return FALSE;
		Sspr.dSy = 0.;
		Sspr.dSz = 0.;
		Sspr.dLength = 0.;
		Sspr.bDamping = FALSE;
		Sspr.dCx      = 0.;
		Sspr.dCy      = 0.;
		Sspr.dCz      = 0.;

		if(CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_RSMM1_ONLY_COMP))
				Sspr.nLinkType = 1;
		Sspr.dLength = m_EditLen.GetEditValue();
		if(fabs(Sspr.dSx) < 1e-6)
				return FALSE;

		Sspr.dUx = 0.;
		Sspr.dUy = 0.;
		Sspr.dUz = 0.;
		Sspr.nDirection = 3;

		switch(GetDirection())
		{
		case 0:
				Sspr.nDirection = 1;
				break;
		case 1:
				Sspr.nDirection = 2;
				break;
		case 2:
				Sspr.dUx = 1.;
				break;
		case 3:
				Sspr.dUx = -1.;
				break;
		case 4:
				Sspr.dUy = 1.;
				break;
		case 5:
				Sspr.dUy = -1.;
				break;
		case 6:
				Sspr.dUz = 1;
				break;
		case 7:
				Sspr.dUz = -1.;
				break;
		}
		if(Sspr.nDirection == 3)
		{
				I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
				pIGM->GetWcsVector(Sspr.dUx, Sspr.dUy, Sspr.dUz, Sspr.dUx, Sspr.dUy, Sspr.dUz);
		}
		return TRUE;
}
void ResponseDispLoadMode1Dlg::OnBnClickedTmExecute()
{
		BOOL bAddCompress = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_RSMM1_CS);
		BOOL bAddShear    = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_RSMM1_SS);
		
		if(!bAddCompress && !bAddShear)
		{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Surface_Spring_Data));
				return;
		}
		CDBDoc *pDoc = CDBDoc::GetDocPoint();
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
		CArray<int,int> rFace;
		T_BNGR_K KeyBngr = 0;
		Sspring Sspr_Comp, Sspr_Shear;
		if((bAddCompress && GetParameter_Compress(Sspr_Comp) == FALSE) || (bAddShear && GetParameter_Shear(Sspr_Shear) == FALSE) )
		{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Surface_Spring_Data));
				return;
		}
		pIGM->GetSelectedElemKeyList(arElemKeyList); 
		
		int nElemType = GetElemType();
		T_ELEM_D ElemData;
		for(int i=arElemKeyList.GetSize(); i > 0; i--)  // 急琶等 Element 吝俊辑 Plane鸥涝父 急琶茄促.
		{
				pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i-1), ElemData);	
				if((nElemType==1&&!pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp)) || (nElemType==0&&!pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp)))
						arElemKeyList.RemoveAt(i-1);
		}
		if(arElemKeyList.GetSize() < 1)
		{
				GSaveHistoryFormatNF(_LS(IDS_CMD2_SOD_SEL_ELEM_NO_SEL_ELEM));
				return;
		}       
		//
		if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Surface_Spring)), CMDTYPE_REMOVE_ANALYSIS))
				return;
		if(bAddCompress)
				KeyBngr = MakeGroup(pDoc, _LS(IDS_IDD_TM_RESPONS_DISP_LOAD_MODE1_IDC_CHECK_RSMM1_CS));    
		if(bAddCompress && !pDoc->m_pDataCtrl->MakeSurfaceSpring(KeyBngr, arElemKeyList, Sspr_Comp.nElType, rFace,Sspr_Comp.nConvertType, Sspr_Comp.nSpringType,
				Sspr_Comp.dSx, Sspr_Comp.dSy, Sspr_Comp.dSz, 0.0, Sspr_Comp.nLinkType,Sspr_Comp.dWidth, Sspr_Comp.dLength, 
				Sspr_Comp.nDirection, Sspr_Comp.dUx, Sspr_Comp.dUy, Sspr_Comp.dUz, TRUE, Sspr_Comp.bDamping, Sspr_Comp.dCx, Sspr_Comp.dCy, Sspr_Comp.dCz))
		{
				pDoc->m_pUndoCtrl->CancelEditDB();
				return;
		}
		if(bAddShear)
				KeyBngr = MakeGroup(pDoc, _LS(IDS_IDD_TM_RESPONS_DISP_LOAD_MODE1_IDC_CHECK_RSMM1_SS));  
		if(bAddShear && !pDoc->m_pDataCtrl->MakeSurfaceSpring(KeyBngr, arElemKeyList, Sspr_Shear.nElType, rFace,Sspr_Shear.nConvertType, Sspr_Shear.nSpringType,
				Sspr_Shear.dSx, Sspr_Shear.dSy, Sspr_Shear.dSz, 0.0, Sspr_Shear.nLinkType,Sspr_Shear.dWidth, Sspr_Shear.dLength, 
				Sspr_Shear.nDirection, Sspr_Shear.dUx, Sspr_Shear.dUy, Sspr_Shear.dUz, TRUE, Sspr_Shear.bDamping, Sspr_Shear.dCx, Sspr_Shear.dCy, Sspr_Shear.dCz))
		{
				pDoc->m_pUndoCtrl->CancelEditDB();
				return;
		}
		pDoc->m_pViewCtrl->UnselectAll(NULL);
		unsigned int nCmd = bAddShear ?  LT_NSPR_CMD: LT_ELNK_CMD;
		pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, nCmd);
}


void ResponseDispLoadMode1Dlg::OnBnClickedTmClose()
{
		// TODO: 在此添加控件通知处理程序代码
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}


void ResponseDispLoadMode1Dlg::OnBnClickedCheckRsmm1Cs()
{
		// TODO: 在此添加控件通知处理程序代码
		BOOL bEnable = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_RSMM1_CS);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_STATIC_RSMM1_CS, bEnable, FALSE);
		GetDlgItem(IDC_CHECK_RSMM1_CS)->EnableWindow(TRUE);
}


void ResponseDispLoadMode1Dlg::OnBnClickedCheckRsmm1Ss()
{
		// TODO: 在此添加控件通知处理程序代码
		BOOL bEnable = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_RSMM1_SS);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_STATIC_RSMM1_SS, bEnable, TRUE);
		GetDlgItem(IDC_CHECK_RSMM1_SS)->EnableWindow(TRUE);
}


void ResponseDispLoadMode1Dlg::OnCbnSelchangeComboRsmm1Elemtype()
{
		// TODO: 在此添加控件通知处理程序代码
		int nType = GetElemType();
		GetDlgItem(IDC_UNIT_WIDTH)->EnableWindow(nType == 0);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(nType == 0);
}
