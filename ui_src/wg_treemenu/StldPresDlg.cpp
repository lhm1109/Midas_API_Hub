// StldPresDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPresDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldPresDlg dialog


CStldPresDlg::CStldPresDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldPresDlg::IDD, pParent)
{
	m_pBitmap = 0;
	//{{AFX_DATA_INIT(CStldPresDlg)
	m_nOption = 0;
	m_nPlateOption = 0;
	m_nSolidOption = 0;
	m_nSelectionMethod = 1;
	m_nLoadType = 1;
	//}}AFX_DATA_INIT
	m_aCtrlDisable.Add(IDC_TM_ELEM_TYPE);
	m_aCtrlDisable.Add(IDC_TM_DIR_TITLE);
	m_aCtrlDisable.Add(IDC_TM_DIR_COMBO);
	m_aCtrlDisable.Add(IDC_TM_UNIFORM);
	m_aCtrlDisable.Add(IDC_TM_LINEAR);
	m_aCtrlDisable.Add(IDC_TM_CONCENT_RADIO);
	m_aCtrlDisable.Add(IDC_TM_VALUE1_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VALUE2_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VALUE3_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VALUE4_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VALUE1);
	m_aCtrlDisable.Add(IDC_TM_VALUE2);
	m_aCtrlDisable.Add(IDC_TM_VALUE3);
	m_aCtrlDisable.Add(IDC_TM_VALUE4);
	m_aCtrlDisable.Add(IDC_TM_VALUE1_UNIT);
	m_aCtrlDisable.Add(IDC_TM_VALUE2_UNIT);
	m_aCtrlDisable.Add(IDC_TM_VALUE3_UNIT);
	m_aCtrlDisable.Add(IDC_TM_VALUE4_UNIT);
	
	//m_aCtrlDisable.Add(IDC_TM_LOADTYPE_COMBO);
	m_aCtrlDisable.Add(IDC_TM_STATIC3);
	m_aCtrlDisable.Add(IDC_TM_DX_STR );
	m_aCtrlDisable.Add(IDC_TM_DY_STR );
	m_aCtrlDisable.Add(IDC_TM_DZ_STR );
	m_aCtrlDisable.Add(IDC_TM_FORCP_STR);
	m_aCtrlDisable.Add(IDC_TM_DX_EDIT);
	m_aCtrlDisable.Add(IDC_TM_DY_EDIT);
	m_aCtrlDisable.Add(IDC_TM_DZ_EDIT);
	m_aCtrlDisable.Add(IDC_TM_FORCP_EDIT  );
	m_aCtrlDisable.Add(IDC_DX_UNIT_STATIC);
	m_aCtrlDisable.Add(IDC_DY_UNIT_STATIC);
	m_aCtrlDisable.Add(IDC_DZ_UNIT_STATIC);
	m_aCtrlDisable.Add(IDC_TM_FORCEP_UNIT );
	m_aCtrlDisable.Add(IDC_TM_FACE_RADIO);
	m_aCtrlDisable.Add(IDC_TM_EDGE_RADIO);

	m_aCtrlDisable.Add(IDC_TM_STATIC2       );
	m_aCtrlDisable.Add(IDC_TM_SELECT_BY_NODE);
	m_aCtrlDisable.Add(IDC_TM_SELECT_BY_ELEM);


	m_aCtrlUniform.Add(IDC_TM_CONCENT_RADIO);
	m_aCtrlUniform.Add(IDC_TM_UNIFORM);
	m_aCtrlUniform.Add(IDC_TM_LINEAR);
	

	m_arConLType.Add(IDC_TM_STATIC3);
	m_arConLType.Add(IDC_TM_DX_STR );
	m_arConLType.Add(IDC_TM_DY_STR );
	m_arConLType.Add(IDC_TM_DZ_STR );
	m_arConLType.Add(IDC_TM_FORCP_STR);
	m_arConLType.Add(IDC_TM_DX_EDIT);
	m_arConLType.Add(IDC_TM_DY_EDIT);
	m_arConLType.Add(IDC_TM_DZ_EDIT);
	m_arConLType.Add(IDC_TM_FORCP_EDIT  );
	m_arConLType.Add(IDC_DX_UNIT_STATIC);
	m_arConLType.Add(IDC_DY_UNIT_STATIC);
	m_arConLType.Add(IDC_DZ_UNIT_STATIC);
	m_arConLType.Add(IDC_TM_FORCEP_UNIT );
	

	m_arNoConLType.Add(IDC_TM_VALUE1_TITLE);
	m_arNoConLType.Add(IDC_TM_VALUE1);
	m_arNoConLType.Add(IDC_TM_VALUE1_UNIT);
	m_arNoConLType.Add(IDC_TM_VALUE2_TITLE);
	m_arNoConLType.Add(IDC_TM_VALUE2);
	m_arNoConLType.Add(IDC_TM_VALUE2_UNIT);
	m_arNoConLType.Add(IDC_TM_VALUE3_TITLE);
	m_arNoConLType.Add(IDC_TM_VALUE3);
	m_arNoConLType.Add(IDC_TM_VALUE3_UNIT);
	m_arNoConLType.Add(IDC_TM_VALUE4_TITLE);
	m_arNoConLType.Add(IDC_TM_VALUE4);
	m_arNoConLType.Add(IDC_TM_VALUE4_UNIT);
	

	m_aCtrlValue[0].Add(IDC_TM_VALUE1_TITLE);
	m_aCtrlValue[0].Add(IDC_TM_VALUE1);
	m_aCtrlValue[0].Add(IDC_TM_VALUE1_UNIT);
	m_aCtrlValue[1].Add(IDC_TM_VALUE2_TITLE);
	m_aCtrlValue[1].Add(IDC_TM_VALUE2);
	m_aCtrlValue[1].Add(IDC_TM_VALUE2_UNIT);
	m_aCtrlValue[2].Add(IDC_TM_VALUE3_TITLE);
	m_aCtrlValue[2].Add(IDC_TM_VALUE3);
	m_aCtrlValue[2].Add(IDC_TM_VALUE3_UNIT);
	m_aCtrlValue[3].Add(IDC_TM_VALUE4_TITLE);
	m_aCtrlValue[3].Add(IDC_TM_VALUE4);
	m_aCtrlValue[3].Add(IDC_TM_VALUE4_UNIT);

	m_arCtrlDirection.Add(IDC_TM_DIR_TITLE2);
	m_arCtrlDirection.Add(IDC_TM_DIR_COMBO2);

	m_arCtrlProject.Add(IDC_TM_PROJ_TITLE);
	m_arCtrlProject.Add(IDC_TM_PROJ_YES);
	m_arCtrlProject.Add(IDC_TM_PROJ_NO);

	m_arCtrlPlateOpt.Add(IDC_TM_FACE_RADIO);
	m_arCtrlPlateOpt.Add(IDC_TM_EDGE_RADIO);

	m_arCtrlSolidOpt.Add(IDC_TM_SOLID_ALL_RADIO  );
	m_arCtrlSolidOpt.Add(IDC_TM_SOLID_4NODE_RADIO);
	m_arCtrlSolidOpt.Add(IDC_TM_SOLID_6NODE_RADIO);
	m_arCtrlSolidOpt.Add(IDC_TM_SOLID_8NODE_RADIO);
	m_arCtrlSolidOpt.Add(IDC_TM_STATIC);
}

CStldPresDlg::~CStldPresDlg()
{
	if (m_pBitmap) delete m_pBitmap;
	
	
}

void CStldPresDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPresDlg)
	DDX_Control(pDX, IDC_TM_FORCEP_UNIT, m_wndForcePUnit);
	DDX_Control(pDX, IDC_DZ_UNIT_STATIC, m_wndDZUnit);
	DDX_Control(pDX, IDC_DY_UNIT_STATIC, m_wndDYUnit);
	DDX_Control(pDX, IDC_DX_UNIT_STATIC, m_wndDXUnit);
	DDX_Control(pDX, IDC_TM_LOADTYPE_COMBO, m_wndLoadType);
	DDX_Control(pDX, IDC_TM_DIR_COMBO2, m_cboDirection);
	DDX_Control(pDX, IDC_TM_FRAME, m_wndFrame);
	DDX_Control(pDX, IDC_TM_DIR_COMBO, m_wndDirCobx);
	DDX_Control(pDX, IDC_TM_DIR_TITLE, m_wndDirTitle);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE, m_wndElemType);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_FACE_RADIO, m_nPlateOption);
	DDX_Radio(pDX, IDC_TM_SOLID_ALL_RADIO, m_nSolidOption);
	DDX_Radio(pDX, IDC_TM_SELECT_BY_NODE, m_nSelectionMethod);
	DDX_Radio(pDX, IDC_TM_CONCENT_RADIO, m_nLoadType);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	//}}AFX_DATA_MAP

	UINT nID[][3] = {
		{IDC_TM_VALUE1_TITLE, IDC_TM_VALUE1, IDC_TM_VALUE1_UNIT},
		{IDC_TM_VALUE2_TITLE, IDC_TM_VALUE2, IDC_TM_VALUE2_UNIT},
		{IDC_TM_VALUE3_TITLE, IDC_TM_VALUE3, IDC_TM_VALUE3_UNIT},
		{IDC_TM_VALUE4_TITLE, IDC_TM_VALUE4, IDC_TM_VALUE4_UNIT},
	};

	for (int i = 0; i < 4; i++)
	{
		DDX_Control(pDX, nID[i][0], m_Title[i]);
		DDX_Control(pDX, nID[i][1], m_Value[i]);
		DDX_Control(pDX, nID[i][2], m_Unit[i]);
	}

	DDX_Control(pDX,IDC_TM_DX_EDIT , m_ConcentVal[0]);
	DDX_Control(pDX,IDC_TM_DY_EDIT , m_ConcentVal[1]);
	DDX_Control(pDX,IDC_TM_DZ_EDIT , m_ConcentVal[2]);
	DDX_Control(pDX,IDC_TM_FORCP_EDIT , m_ConcentVal[3]);

}

void CStldPresDlg::SetDirection(int nElemType)
{
	CString csItem;

	switch(nElemType)
	{
	case 0: // Plate
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Direction___));
		m_wndDirCobx.ResetContent();
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_x));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_y));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Local_z));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_X));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
		m_wndDirCobx.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
		break;
	case 7:
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Edge__));
		m_wndDirCobx.ResetContent();
		for(int i=0; i < 4; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Edge___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	case 9: // Wall 
	case 1: // Plane Stress
	case 2: // Plane Strain
	case 3: // Axisymmetric
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Edge__));
		m_wndDirCobx.ResetContent();
		for(int i=0; i < 4; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Edge___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	
	case 5: // 6 Nodes Solid (Face 5)
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Face__));
		m_wndDirCobx.ResetContent();
		//for(int i=0; i < 10-nElemType; i++)
		for(int i=0; i < 5; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Face___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	case 6: // 4 Nodes Solid (Face 4)
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Face__));
		m_wndDirCobx.ResetContent();
		//for(int i=0; i < 10-nElemType; i++)
		for(int i=0; i < 4; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Face___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	case 4: // 8 Nodes Solid (Face 6)
	case 8:
		m_wndDirTitle.SetWindowText(_LS(IDS_WG_TREEMENU_Pressure_Face__));
		m_wndDirCobx.ResetContent();
		//for(int i=0; i < 10-nElemType; i++)
		for(int i=0; i < 6; i++)
		{
			csItem.Format(_LS(IDS_WG_TREEMENU_Face___d), i+1);
			m_wndDirCobx.AddString(csItem);
		}
		break;
	}
}

// nDirection : 요소 타입에 따라방향, 에지, 면으로 쓰임
void CStldPresDlg::SetStateOfCtrl(int nElemType, int nDir, BOOL bUniform)
{
	
	UINT aID[][3] = {
		IDC_TM_VALUE1_TITLE, IDC_TM_VALUE1, IDC_TM_VALUE1_UNIT,
		IDC_TM_VALUE2_TITLE, IDC_TM_VALUE2, IDC_TM_VALUE2_UNIT,
		IDC_TM_VALUE3_TITLE, IDC_TM_VALUE3, IDC_TM_VALUE3_UNIT,
		IDC_TM_VALUE4_TITLE, IDC_TM_VALUE4, IDC_TM_VALUE4_UNIT,
	};

	UINT aEnable[][4] = {
		{TRUE,  TRUE,   TRUE,   TRUE},
		{TRUE,  FALSE,  FALSE,  FALSE},
		{TRUE,  TRUE,   FALSE,  FALSE},
		{TRUE,  TRUE,   TRUE,   FALSE},
	 };
	int nIndex;
	BOOL bEnableUniform = TRUE; // Always true

	CString aFrameName[] = {
		_LS(IDS_WG_TREEMENU_Plate_Face), _LS(IDS_WG_TREEMENU_Plane_Stress), _LS(IDS_WG_TREEMENU_Plane_Strain),
		_LS(IDS_WG_TREEMENU_Axisymmetric), _LS(IDS_WG_TREEMENU_8_Nodes_Solid), _LS(IDS_WG_TREEMENU_6_Nodes_Solid), _LS(IDS_WG_TREEMENU_4_Nodes_Solid) , 
		_LS(IDS_WG_TREEMENU_Plate_Edge),_LS(IDS_WG_TREEMENU_Solid_Faces),_LS(IDS_WG_TREEMENU_Wall),
	};

	switch(nElemType)
	{
	case 0:
		if (bUniform) nIndex = 1;
		else nIndex = 0;
		break;
	case 7:
		if (bUniform) nIndex = 1;
		else nIndex = 2;
		break;
	case 9: // Wall
	case 1:
	case 2:
	case 3:
		if (bUniform) nIndex = 1;
		else nIndex = 2;
		break;
	case 4:
		if (bUniform) nIndex = 1;
		else nIndex = 0;
		break;
	case 8:
		if (bUniform) nIndex = 1;
		else nIndex = 0;
		break;
	case 5:
		if (bUniform) nIndex = 1;
		else 
		{
			if(nDir == 0 || nDir == 1)
				nIndex = 3;
			else
				nIndex = 0;
		}
		break;
	case 6:
		if (bUniform) nIndex = 1;
		else nIndex = 3;
		break;
	default:
		return;
	}
	CString csFrameName;
	csFrameName.Format(_LS(IDS_WG_TREEMENU_Pressure_on__s), aFrameName[nElemType]);
	m_wndFrame.SetWindowText(csFrameName);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlUniform, bEnableUniform);
	for (int i = 0; i < 4; i++)
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlValue[i], aEnable[nIndex][i]);

	CArray<UINT,UINT> arTemp ;
	arTemp.Add(IDC_TM_CONCENT_RADIO);

	if(nElemType == 0)// Plate Point Load 사용불가로....
		//this->CtrlEnableDisable(arTemp,TRUE);
		this->CtrlEnableDisable(arTemp,FALSE);
	else 
		this->CtrlEnableDisable(arTemp,FALSE);
}

void CStldPresDlg::ChangeBitmap(int nElemType)
{
	int nBitmap;
	if (nElemType == 0) nBitmap = 1;
	else if (nElemType >= 1 && nElemType <= 3) nBitmap = 2;
	else if (nElemType == 4) nBitmap = 3;
	else if (nElemType == 5) nBitmap = 4;
	else if (nElemType == 6) nBitmap = 5;
	else if (nElemType == 7) nBitmap = 2; // Plate Edge
	else if (nElemType == 8) nBitmap = 3;
	else if (nElemType == 9) nBitmap = 6;
	else return;

	// Bitmap을 Oneway, Twoway로 바꾼다.
	UINT aBitmapID[] = { 
		IDB_TM_PRES1, IDB_TM_PRES2, IDB_TM_PRES3, IDB_TM_PRES4, IDB_TM_PRES5, 
		IDB_TM_PRES6,
	};

	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nBitmap-1]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CStldPresDlg::EnableValueEdit(BOOL bEnable)
{
	int nLoadType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUniform, nLoadType);
	//nLoadType = this->m_wndLoadType.GetCurSel();

	int nDir = m_wndDirCobx.GetCurSel();
	int nElemType = GetPlateSolidOptionMan();
	//SetStateOfCtrl(nElemType, nDir, nLoadType==0);
	SetStateOfCtrl(nElemType, nDir, nLoadType==1);
}
// Element Type과 Solid Node 갯수에 따른 Filtering을 수행한다. 
void CStldPresDlg::GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	
	 //18 Wall Edge 
	if(nOption != 2) // Add Or Modify 
	{
		if(nElemType <= 8 || nElemType == 18) //By Element 
		{
			pIGM->GetSelectedElemKeyList(aSelKey);
		}
		else
		{
			CArray<T_ELEM_K,T_ELEM_K> aElemList; // Select된 element list
			CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
			
			pIGM->GetSelectedNodeKeyList(aSelNodeKey);
			pIGM->GetActiveElemKeys(aElemList);
			
			if(nElemType == 10 || nElemType == 14 || nElemType == 15 || nElemType == 16)
			{
				 CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, 
																								 aSelNodeKey, 
																								 aSelKey,
																								 arFaceOrEdge);
			}else if( nElemType == 9 || nElemType == 11 || nElemType == 12 || nElemType == 13 
															 || nElemType == 17  )
			{
				CDBDoc::GetDocPoint()->SelectEdgeByNode(aElemList   , 
																								aSelNodeKey , 
																								aSelKey     ,
																								arFaceOrEdge);
			}
		}
	}
	else  // Delete
	{
		pIGM->GetSelectedElemKeyList(aSelKey);
	}

	int i;
	T_ELEM_D ElemData;
	int nNodeNum;
	
	if (nOption == 2) // delete
	{
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (ElemData.eltyp == PLATE_EL || 
					ElemData.eltyp == PLSTRS_EL ||
					ElemData.eltyp == PLSTRN_EL ||
					ElemData.eltyp == AXISYM_EL ||
					ElemData.eltyp == SOLID_EL  ||
					ElemData.eltyp == WALL_EL   ) continue;
			aSelKey.RemoveAt(i);
		}
		return;
	}

	switch(nElemType)
	{
	case 0: // Plate/PlaneStress Face By Element
	case 7: // Plate/PlaneStress Edge By Element 
	case 9: // Plate/PlaneStress Edge By Node
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp) && !m_pDoc->m_pAttrCtrl->IsPlstrs(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 9)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 11: // Plane Stress Edge By Node
	case 1:  // Plane Stress Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrs(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 11)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 12: // Plane Strain Edge By Node
	case 2:  // Plane Strain Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsPlstrn(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 12)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 13: // AxiSymmetric Edge By Node
	case 3:  // AxiSymmetric Edge By Element 
		for (i = aSelKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
			if (!m_pDoc->m_pAttrCtrl->IsAxisym(ElemData.eltyp))
			{
				aSelKey     .RemoveAt(i);
				if(nElemType == 13)
					arFaceOrEdge.RemoveAt(i);
			}
		}
		break;
	case 17: // Wall Edge By Node
	case 18: // Wall Edge By Element
		{
			for (i = aSelKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsWall(ElemData.eltyp))
				{
					aSelKey     .RemoveAt(i);
					if(nElemType == 17)
						arFaceOrEdge.RemoveAt(i);
				}
			}
		}
		break;
	case 4:  // 8 Solid Face By Element 
	case 5:  // 6 Solid Face By Element 
	case 6:  // 4 Solid Face By Element 
	case 8:  // Solid   Face By Element
	case 10: // Solid   Face By Node 
	case 14: // 8 Solid Face By Node
	case 15: // 6 Solid Face By Node
	case 16: // 4 Solid Face By Node
		if      (nElemType == 4 || nElemType == 14) nNodeNum = 8;
		else if (nElemType == 5 || nElemType == 15) nNodeNum = 6;
		else if (nElemType == 6 || nElemType == 16) nNodeNum = 4;
		else    nNodeNum = 100; // All Solid 

		if( nElemType == 4 || nElemType == 5 || nElemType ==6 || nElemType == 8)// by Element
		{
			for (i = aSelKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsSolid(ElemData.eltyp))aSelKey.RemoveAt(i);
				else 
				{
					if( nNodeNum <= 8 && nNodeNum >= 4)
					{
						if (m_pDoc->m_pAttrCtrl->GetNumNode(ElemData.eltyp, ElemData.elnod) != nNodeNum)
								aSelKey     .RemoveAt(i);
					}
				}
			}
		}
		else if( nElemType == 10 || nElemType == 14|| nElemType == 15|| nElemType ==16 ) // by Node
		{
			for (i = aSelKey.GetSize()-1; i >= 0; i--)
			{
				m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
				if( nNodeNum <= 8 && nNodeNum >= 4)
				{
					if (m_pDoc->m_pAttrCtrl->GetNumNode(ElemData.eltyp, ElemData.elnod) != nNodeNum)
					{
						aSelKey     .RemoveAt(i);
						arFaceOrEdge.RemoveAt(i);
					}
				}
			}
		}
		break;
	}
}

int CStldPresDlg::GetForceNum(int nElemType, int nDir)
{
	// 8 Nodes Solid
	if (nElemType == 0 || nElemType == 1 || nElemType == 2 || 
			nElemType == 3 || nElemType == 4) return 4;

	// 4 Nodes Solid
	if (nElemType == 6) return 3; 

	// 6 Nodes Solid
	if (nDir == 0 || nDir == 1) return 3;
	return 4;
}

void CStldPresDlg::SetUnitType(int nElemType)
{

	// 0 Plate Face       
	// 1 Planestress 
	// 2 Planestrain
	// 3 Axisymmetric
	// 4 Solid 8 Face
	// 5 Solid 6 Face
	// 6 Solid 4 Face
	// 7 Plate Edge
	// 8 Solid Face 
	// 9 Plate Edge By Node 
	//10 Solid Face By Node
	//11 stress Edge By Node
	//12 strain Edge By Node
	//13 axisymmetric Edge By Node
	//14 8 Node Solid Select By Node
	//15 6 Node Solid Select By Node
	//16 4 Node Solid Select By Node
	//17 Wall Edge By Node
	//18 Wall Edge 
		
	int nUnitType;

	if(nElemType == 7 || nElemType == 9)
	{
		nUnitType = CUnitCtrl::m_PRES_UNIT.EdgeLoad; // 저장되는 변수도 다름 ...
	}
	else if(nElemType == 1  || nElemType == 2  || nElemType == 3  ||
					nElemType == 11 ||
					nElemType == 12 || nElemType == 13 || nElemType == 17 ||
					nElemType == 18 )
					nUnitType = CUnitCtrl::m_PRES_UNIT.Force;
	else 
		nUnitType = CUnitCtrl::m_PRES_UNIT.Force;
	
	for (int i = 0; i < 4; i++)
	{
		m_Value[i].SetUnitType(nUnitType);
		m_Unit[i].SetUnitType(nUnitType);
	}  
}

BEGIN_MESSAGE_MAP(CStldPresDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldPresDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE, OnChangeElemType)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_UNIFORM, OnChangeUniform)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO2, OnSelchangeTmDirCombo2)
	ON_BN_CLICKED(IDC_TM_SOLID_ALL_RADIO, OnTmSolidOptionRadio)
	ON_BN_CLICKED(IDC_TM_FACE_RADIO, OnTmPlateOptionRadio)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_NODE, OnTmSelectionMethod)
	ON_CBN_SELCHANGE(IDC_TM_LOADTYPE_COMBO, OnSelchangeTmLoadtypeCombo)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_LINEAR, OnChangeUniform)
	ON_BN_CLICKED(IDC_TM_SOLID_8NODE_RADIO, OnTmSolidOptionRadio)
	ON_BN_CLICKED(IDC_TM_SOLID_6NODE_RADIO, OnTmSolidOptionRadio)
	ON_BN_CLICKED(IDC_TM_SOLID_4NODE_RADIO, OnTmSolidOptionRadio)
	ON_BN_CLICKED(IDC_TM_EDGE_RADIO, OnTmPlateOptionRadio)
	ON_BN_CLICKED(IDC_TM_SELECT_BY_ELEM, OnTmSelectionMethod)
	ON_BN_CLICKED(IDC_TM_CONCENT_RADIO, OnChangeUniform)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldPresDlg message handlers
void CStldPresDlg::DistPrjCtrl()
{
	CRect rRef1, rRef2;

	GetDlgItem(IDC_TM_DIR_TITLE2)->GetWindowRect(rRef1);
	GetDlgItem(IDC_TM_PROJ_TITLE)->GetWindowRect(rRef2);

	m_nDistPrjCtrl = rRef2.top - rRef1.top;
}

BOOL CStldPresDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	m_nPlateOption = 0;
	m_nSolidOption = 0;
	m_nSelectionMethod = 1;
	m_nLoadType = 1;

	UpdateData(FALSE);
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	m_wndForcePUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndDZUnit    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDYUnit    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDXUnit    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_wndElemType.ResetContent();
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plate_Plane_Stress));
	//m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plane_Stress));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Plane_Strain));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Axisymmetric));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Solid));
#ifndef _CIVIL
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_Wall));
#endif
	/*
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_8_Nodes_Solid));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_6_Nodes_Solid));
	m_wndElemType.AddString(_LS(IDS_WG_TREEMENU_4_Nodes_Solid));
	*/

	m_cboDirection.ResetContent();
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Normal));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_cboDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
	m_cboDirection.SetCurSel(0);
	((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);  // Project 항목의 기본값.
	CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);

	int nElemType = 0;
	m_wndElemType.SetCurSel(nElemType); // default : plate

	nElemType = this->GetPlateSolidOptionMan();

	//m_wndLoadType.SetCurSel(1);
	m_nLoadType = 1;

	ChangeBitmap(nElemType);
	SetDirection(nElemType);
	m_wndDirCobx.SetCurSel(2);  // default : local z
	m_nOption = 0;    // default : add

	int nLoadType = 0;  // default : uniform
	//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUniform, nLoadType);

	EnableValueEdit(TRUE); // TRUE를 넘겨주더라도 조건에 따라서 FALSE가 된다.
	
	SetUnitType(GetElemTypeExternal()/*nElemType*/);
	
	SelectionMethodMan();

	//UpdateData(FALSE);

	CheckMode(TRUE);
	 // Added by BugBoy   Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.10
	DistPrjCtrl();  // Project Ctrl을 옵션에 따라서 위, 아래로 이동시키기 위한 거리계산.
	m_bUpDown = FALSE;
	ChangePrjCtrlPosition(TRUE);

	LoadTypeCtrlMan();

	DlgSizeMan();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldPresDlg::OnTmExecute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<int, int> arIFaceOrEdge;
	
	
	//int nElemType = m_wndElemType.GetCurSel();
	//int nElemType = this->GetPlateSolidOptionMan();

	int nElemType = this->GetElemTypeExternal();

	// 선택된 Element를 Filtering 한다.  
	// Select By Node Mode가 추가됨에 따라. Node에 의한 Select 대상 Object설정
	// 요구...
	GetElemByType(m_nOption, nElemType, aSelKey,arIFaceOrEdge);  //Face Or Edge 
	// By Node 와 By Element구분에 의한 후보 Element 가 설정 된 상태 .. aSelKey

		
	if (aSelKey.GetSize() == 0) 
	{
		CString csElemTypeName;
		if (m_nOption == 2) // Delete
			csElemTypeName = _LS(IDS_WG_TREEMENU_pressure);
		else
			m_wndElemType.GetWindowText(csElemTypeName);
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), csElemTypeName);
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_PRES_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;
	data.LoadCommandTypeNew = 1;  // Pres(1), Hydro(2)
 
	// 0 Plate Face          By Element
	// 1 Planestress         By Element
	// 2 Planestrain         By Element
	// 3 Axisymmetric        By Element
	// 4 Solid 8 Face        By Element
	// 5 Solid 6 Face        By Element
	// 6 Solid 4 Face        By Element
	// 7 Plate Edge          By Element
	// 8 Solid Face          By Element
	// 9 Plate Edge          By Node 
	//10 Solid Face          By Node
	//11 stress Edge         By Node
	//12 strain Edge         By Node
	//13 axisymmetric Edge   By Node
	//14 8 Node Solid Select By Node
	//15 6 Node Solid Select By Node
	//16 4 Node Solid Select By Node
	//17 Wall Edge By Node         
	//18 Wall Edge By Element    

	
	if (nElemType == 0 || nElemType == 7 || nElemType == 9 ||
			nElemType == 17|| nElemType == 18 ) data.LoadType = 1;
	else if (nElemType == 1 || nElemType == 2 || nElemType == 3 ||
					 nElemType == 11|| nElemType == 12|| nElemType == 13 
					 ) data.LoadType = 2;
	else data.LoadType = 3;

	// Edge Face
	//if(data.LoadType != 1 && nElemType != 0 ) // Plate Face만 제외 
	if(0 != nElemType)
		data.nEdgeFace = m_wndDirCobx.GetCurSel() + 1;  // By Node 는 Pressure 추가시 다시 설정 

	// Direction
	int nDir=0;
	if(data.LoadType == 1 && nElemType == 0)
		nDir = m_wndDirCobx.GetCurSel()+1;
	else if(data.LoadType == 3)
		nDir = m_cboDirection.GetCurSel()+1;

	if(nDir)
		data.LoadDirectionNew = nDir;

	
	
	// Projection  Plate Edge 제외 
	if(data.LoadType != 2 && nElemType != 7)
		data.Projection = ((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->GetCheck(); 
	//

	int nLoadType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUniform, nLoadType);
	//nLoadType = m_wndLoadType.GetCurSel();
	//nLoadType (0) Consent (1) Uniform (2) Linear

	

	if (data.LoadType == 2) // Plane Stress, Plane Strain, Axisymmetric
	{
		//if (nLoadType == 0) //Uniform
		if (nLoadType == 1) //Uniform
		  data.Force[0] = m_Value[0].GetEditValue();
		else if(nLoadType ==2)	// Linear
		{
		  data.Force[1] = m_Value[0].GetEditValue();
			data.Force[2] = m_Value[1].GetEditValue();
		}
		else // Concentrat Value
		{
			ASSERT(FALSE);
		}
	}
	else // Solid , Plate
	{
		if(nElemType == 7 || nElemType == 9 || nElemType == 17 || nElemType == 18)
		{
			data.nSubLoadType = 2;
			if (nLoadType == 1) //Uniform
		    data.EdgeLoad[0] = m_Value[0].GetEditValue();
			else if(nLoadType ==2)	// Linear
			{
			  data.EdgeLoad[1] = m_Value[0].GetEditValue();
				data.EdgeLoad[2] = m_Value[1].GetEditValue();
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			if (nLoadType == 1)
				data.Force[0] = m_Value[0].GetEditValue();
			else if( nLoadType == 2)
			{
				int nNum = GetForceNum(nElemType, nDir);
				for (int i = 0; i < nNum; i++)
					data.Force[i+1] = m_Value[i].GetEditValue();
			}
			else
			{
				if(nElemType == 0)
				{
					/*
					data.nSubLoadType = 1;
					data.dGX       = m_ConcentVal[0].GetEditValue();
					data.dGY       = m_ConcentVal[1].GetEditValue();
					data.dGZ       = m_ConcentVal[2].GetEditValue();
					data.PointLoad = m_ConcentVal[3].GetEditValue();
					*/
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}
	}

	// Plate Face concentrated Load 는 Projection을 불허한다. 
	if( nLoadType == 0 && nElemType == 0) 
	{
		data.Projection = FALSE;
	}

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	if((nElemType >= 0 && nElemType <= 8) || nElemType == 18) // By Element
	{
		BOOL bSuccess=FALSE;
		if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		{
			
			data.GroupKey = LdgrK;
			
			bSuccess = m_pDoc->m_pDataCtrl->AddPres(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
		}
		else if (m_nOption == 2)  // delete
			bSuccess = m_pDoc->m_pDataCtrl->DelPres(aSelKey, nLoadCaseKey,LdgrK);	

		if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else // By Node
	{
		
		BOOL bSuccess = TRUE;
		BOOL bTemp;
		
		if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		{
			int nElem  = aSelKey.GetSize();
			int nFE    = arIFaceOrEdge.GetSize();
			ASSERT( nElem == nFE );
			
			data.GroupKey = LdgrK;
			
			for( int i = 0 ; i < nElem ; i++)
			{
				data.nEdgeFace = arIFaceOrEdge[i];
				bTemp = m_pDoc->m_pDataCtrl->AddPres(aSelKey[i],data,(m_nOption==0)?FALSE:TRUE);
				bSuccess &= bTemp;
			}
		}
		else if (m_nOption == 2)  // delete
			bSuccess = m_pDoc->m_pDataCtrl->DelPres(aSelKey, nLoadCaseKey,LdgrK);	

		if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	
}

void CStldPresDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	if(CDBDoc::GetDocPoint())
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
	CloseDlg();
}

void CStldPresDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
		return;
	}
	else
	{
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, TRUE);
	EnableValueEdit(TRUE);			
}

void CStldPresDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldPresDlg::OnChangeElemType() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	int nElemType = GetPlateSolidOptionMan();

	//Plate Face Select : 0  Edge Select : 7
	//Solid All Select  : 8  
	//Wall Edge         : 9

	ChangeBitmap(nElemType);
	SetDirection(nElemType);

	int nDir;
	if (nElemType == 0) nDir = 2;
	else nDir = 0;

	m_wndDirCobx.SetCurSel(nDir);
	int nLoadType = 1;  // default : uniform
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUniform, nLoadType);
	//m_wndLoadType.SetCurSel(nLoadType);

	EnableValueEdit(TRUE); // TRUE를 넘겨주더라도 조건에 따라서 FALSE가 된다.
	SetUnitType(nElemType);

	CheckMode(TRUE); 
	 // Added by BugBoy   Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.10
	LoadTypeCtrlMan();

	ModifyProjectCtrl(nElemType);
}





// 변경된 ElemType과 변경 되지 않은 ElemType을 모두 참조 해서.. Control 관리 할것..
int CStldPresDlg::GetPlateSolidOptionMan()
{
	if(NULL ==m_wndElemType.GetSafeHwnd()) return 0;
	int nElemType = m_wndElemType.GetCurSel();
	
	if(nElemType >0) nElemType++;
	

	//Plate Face Select : 0  Edge Select : 7
	//Solid All Select  : 8

	if(nElemType == 0 )
	{
		CtrlShowHide(this->m_arCtrlPlateOpt, TRUE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
		//////////////////////////////////////////
		//Solid Option을 비활성화 ,  Plate Option을 활성화 시킨다.
		//Plate Option을 참조 nElemType을 설정..
		if(m_nPlateOption == 1)
			nElemType = 7;
	}
	else if( nElemType == 4) //Solid
	{
		CWnd * pSrc, * pDest;
		pDest = GetDlgItem(IDC_TM_FACE_RADIO);
		pSrc  = GetDlgItem(IDC_TM_SOLID_ALL_RADIO);

		ASSERT(pDest); ASSERT(pSrc);

		CRect DestRect, SrcRect;
		pDest->GetWindowRect(&DestRect);
		pSrc ->GetWindowRect(&SrcRect);

		int YDelta = DestRect.top - SrcRect.top;

		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, TRUE);
		this->CtrlMoveDistY(this->m_arCtrlSolidOpt, YDelta);
		

		//////////////////////////////////////////
		//Plate Option을 비활성화 , Solid Option을 활성화 시킨다. 
		//Solid Option을 참조 nElemType설정..
		switch(m_nSolidOption)
		{
		case 0: nElemType = 8; break;
		case 1: nElemType = 4; break;
		case 2: nElemType = 5; break;
		case 3: nElemType = 6; break;
		default: ASSERT( FALSE); break;
		}
	}
	else if( nElemType == 5) // Wall
	{
		nElemType = 9;
		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
	}
	else
	{
		CtrlShowHide(this->m_arCtrlPlateOpt, FALSE);
		CtrlShowHide(this->m_arCtrlSolidOpt, FALSE);
	}

	
	CArray<UINT,UINT> SelMCtrls;
	//SelMCtrls.Add(IDC_TM_STATIC2);
	SelMCtrls.Add(IDC_TM_SELECT_BY_NODE);
	//SelMCtrls.Add(IDC_TM_SELECT_BY_ELEM);

	CArray<UINT,UINT> TSelMCtrls;
	TSelMCtrls.Add(IDC_TM_SELECT_BY_NODE);
	TSelMCtrls.Add(IDC_TM_SELECT_BY_ELEM);
	//CDlgUtil::CtrlRadioGetCheck(this,TSelMCtrls,m_nSelectionMethod);
	if(nElemType == 0)
	{
		
		CDlgUtil::CtrlRadioSetCheck(this,TSelMCtrls, 1);
		CtrlEnableDisable(SelMCtrls,FALSE);
		m_nSelectionMethod = 1;
	}
	else
	{
		CtrlEnableDisable(SelMCtrls,TRUE);
	}


	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_TM_DIR_TITLE);
	Ctrls.Add(IDC_TM_DIR_COMBO);
	
	if((nElemType == 8          && m_nSelectionMethod != 1) || 
		 (m_nSelectionMethod != 1 && nElemType <= 6 && nElemType >= 0) ||
		 (nElemType == 7 && m_nSelectionMethod != 1)||
		 (nElemType == 9 && m_nSelectionMethod != 1))
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
	else 
	{
		this->CtrlEnableDisable(Ctrls,TRUE);   
	}

	return nElemType;
}


void CStldPresDlg::ModifyProjectCtrl(int nElemType)
{
	switch(nElemType)
	{
		case 0:   // plate			
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
			ChangePrjCtrlPosition(TRUE);
			if(m_wndDirCobx.GetCurSel() > 2)
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, TRUE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			else
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			break;
		case 7:   // plate Edge			
			ChangePrjCtrlPosition(TRUE);
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			break;
		case 9:   // Wall 
		case 1:   // plane stress
		case 2:   // plane strain
		case 3:   // Axisymmetric
			ChangePrjCtrlPosition(TRUE);
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			break;
		case 4:   // solid 8
		case 5:   // solid 6
		case 6:   // solid 4
		case 8:
			CDlgUtil::CtrlShowHide(this, m_arCtrlDirection, TRUE);
			ChangePrjCtrlPosition(FALSE);
			if(m_cboDirection.GetCurSel() > 0)
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, TRUE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			else
			{
				CDlgUtil::CtrlEnableDisable(this, m_arCtrlProject, FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_YES))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_TM_PROJ_NO))->SetCheck(TRUE);
			}
			break;
	}

	int nLType;// = this->m_wndLoadType.GetCurSel();
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUniform, nLType);
	//if(nLType == 0) // Concentrated Load 
	{
		if( nLType == 0 && nElemType == 0)
		{
			this->CtrlEnableDisable(m_arCtrlProject,FALSE);
		}
		else
		{
			int nSelEType = this->m_wndElemType.GetCurSel();
			if(nSelEType >0) nSelEType++;
			
			int nDir      = this->m_wndDirCobx.GetCurSel();
			if((nSelEType == 0 && nDir > 2) || (nSelEType == 4 && nDir > 0))
				this->CtrlEnableDisable(m_arCtrlProject,TRUE);
		}
	}
}

void CStldPresDlg::ChangePrjCtrlPosition(BOOL bUpDown)  //bUpDown=1 위로, bUpDown=0 아래로.
{
	if(bUpDown && !m_bUpDown)
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlProject, -m_nDistPrjCtrl);
		m_bUpDown = TRUE;
	}
	else if(!bUpDown && m_bUpDown)
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlProject, m_nDistPrjCtrl);
		m_bUpDown = FALSE;
	}
}

void CStldPresDlg::OnChangeDirection() 
{
	// TODO: Add your control notification handler code here
	//int nElemType = m_wndElemType.GetCurSel();
	int nElemType = GetPlateSolidOptionMan();

	CheckMode(TRUE); 
	 // Added by BugBoy   Pressure Load 의 Edge나 Face를 나타내기 위해서.
	 // 2000.8.10
	//ModifyProjectCtrl(m_wndElemType.GetCurSel());
	ModifyProjectCtrl(nElemType);

	if (nElemType != 5) return; // if not 6 node solid, do nothing
	EnableValueEdit(TRUE);
}

void CStldPresDlg::OnChangeUniform() 
{
	// TODO: Add your control notification handler code here
	LoadTypeCtrlMan();
	EnableValueEdit(TRUE);
}

//int C
int CStldPresDlg::GetElemTypeExternal()
{
	UpdateData(TRUE);
	
	// Edge Select By Node ( 9 )  Face Select By Node ( 10);
		// 0 Plate Face       
		// 1 Planestress 
		// 2 Planestrain
		// 3 Axisymmetric
		// 4 Solid 8 Face
		// 5 Solid 6 Face
		// 6 Solid 4 Face
		// 7 Plate Edge
		// 8 Solid Face 
		// 9 Plate Edge By Node 
		//10 Solid Face By Node
		//11 stress Edge By Node
		//12 strain Edge By Node
		//13 axisymmetric Edge By Node
		//14 8 Node Solid Select By Node
		//15 6 Node Solid Select By Node
		//16 4 Node Solid Select By Node
		//17 Wall Edge By Node
		//18 Wall Edge 

	int nElemType = GetPlateSolidOptionMan();

	if(m_nSelectionMethod == 0 && nElemType == 9) nElemType = 17;  // Wall Edge By Node
	if(m_nSelectionMethod == 1 && nElemType == 9) nElemType = 18;  // Wall Edge 

	if(m_nSelectionMethod == 0 && nElemType == 7) nElemType = 9;
	if(m_nSelectionMethod == 0 && nElemType == 8) nElemType = 10;
	
	if(m_nSelectionMethod == 0 && nElemType == 1) nElemType = 11;  // stress Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 2) nElemType = 12;  // strain Edge By Node
	if(m_nSelectionMethod == 0 && nElemType == 3) nElemType = 13;  // axisymmetric Edge By Node
	
	if(m_nSelectionMethod == 0 && nElemType == 4) nElemType = 14;// 8 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 5) nElemType = 15;// 6 Node Solid Select By Node
	if(m_nSelectionMethod == 0 && nElemType == 6) nElemType = 16;// 4 Node Solid Select By Node

	return nElemType;
}


void CStldPresDlg::CheckMode(BOOL bCheck)
{
	m_pDoc = CDBDoc::GetDocPoint();
	if(m_pDoc)
	{
		if(bCheck)
		{
			int nElemType = GetElemTypeExternal();
			m_pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
			m_pDoc->m_pViewCtrl->SetPressureInputParam(nElemType, m_wndDirCobx.GetCurSel());	
		}
		else
		{
			m_pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);  
			m_pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

void CStldPresDlg::PostNcDestroy() 
{
 	CheckMode(FALSE);
	CMenuBarChildDlg::PostNcDestroy();
}

void CStldPresDlg::OnSelchangeTmDirCombo2() 
{
	//ModifyProjectCtrl(m_wndElemType.GetCurSel());	
	int nElemType = GetPlateSolidOptionMan();
	ModifyProjectCtrl(nElemType);
}


void CStldPresDlg::OnTmSolidOptionRadio() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	this->OnChangeElemType();
	/*
	int nElemType = GetPlateSolidOptionMan();
	ModifyProjectCtrl(nElemType);
	*/
	//SelectionMethodMan();

}

void CStldPresDlg::OnTmPlateOptionRadio() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	this->OnChangeElemType();
	/*
	int nElemType = GetPlateSolidOptionMan();
	ModifyProjectCtrl(nElemType);
	*/
	LoadTypeCtrlMan();
	EnableValueEdit(TRUE);
}

void CStldPresDlg::OnTmSelectionMethod() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->OnChangeElemType();
}

void CStldPresDlg::SelectionMethodMan()
{
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_TM_DIR_TITLE);
	Ctrls.Add(IDC_TM_DIR_COMBO);
	if(m_nSelectionMethod == 1)
	{
		this->CtrlEnableDisable(Ctrls,TRUE);  
	}
	else
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
}

void CStldPresDlg::OnSelchangeTmLoadtypeCombo() 
{
	// TODO: Add your control notification handler code here
	LoadTypeCtrlMan();
	EnableValueEdit(TRUE);
}

void CStldPresDlg::DlgSizeMan()
{
	CRect RectDlg, RectApply;
	
	GetWindowRect(&RectDlg);
	CWnd * pWnd = GetDlgItem(IDC_TM_EXECUTE);
	ASSERT(pWnd);
	pWnd->GetWindowRect(&RectApply);

	int DeltaY = RectDlg.bottom - RectApply.bottom;

	int Wx = RectDlg.Width();
	int Wy = RectDlg.Height() - DeltaY;

	SetWindowPos(NULL, 0,0,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CStldPresDlg::LoadTypeCtrlMan()
{
	//Dest .. IDC_TM_VALUE1_TITLE
	//Src ..  IDC_TM_STATIC3

	CWnd *pwndDest, *pwndSrc;

	pwndDest = GetDlgItem(IDC_TM_VALUE1_TITLE);
	//pwndSrc  = GetDlgItem(IDC_TM_STATIC3);
	pwndSrc  = GetDlgItem(IDC_TM_DX_STR);

	ASSERT(pwndDest);
	ASSERT(pwndSrc );

	CRect RectDest, RectSrc;

	pwndDest->GetWindowRect(&RectDest);
	pwndSrc ->GetWindowRect(&RectSrc );

	int DeltaY = RectDest.top - RectSrc.top;

	int nElemType = this->GetPlateSolidOptionMan();

	/*
	m_arConLType  ;
	m_arNoConLType;
	*/

	int nLType;// = this->m_wndLoadType.GetCurSel();
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUniform, nLType);
	//if(nLType == 0) // Concentrated Load 
	//{
		if( nLType == 0 && nElemType == 0)
		{
			this->CtrlEnableDisable(m_arCtrlProject,FALSE);
		}
		else
		{
			int nSelEType = this->m_wndElemType.GetCurSel();
			if(nSelEType >0) nSelEType++;

			int nDir      = this->m_wndDirCobx.GetCurSel();
			if((nSelEType == 0 && nDir > 2) || (nSelEType == 4 && nDir > 0))
				this->CtrlEnableDisable(m_arCtrlProject,TRUE);
		}
	//}

	if( nElemType == 0 || nElemType == 4 || nElemType == 5 ||
			nElemType == 6 || nElemType == 8  )
	{
		
		if( nLType == 0 )
		{
			this->CtrlShowHide (m_arNoConLType,FALSE );
			this->CtrlMoveDistY(m_arConLType  ,DeltaY);
			this->CtrlShowHide (m_arConLType  ,TRUE  );
		}
		else
		{
			this->CtrlShowHide(m_arNoConLType ,TRUE );
			this->CtrlShowHide(m_arConLType   ,FALSE);
		}
	}
	else if(nElemType == 9) // Wall
	{
		this->CtrlShowHide(m_arNoConLType,TRUE);
		this->CtrlShowHide(m_arConLType, FALSE);
		if(nLType == 0)
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUniform, 1);
	
	}
	else // Plate Edge
	{
		this->CtrlShowHide(m_arNoConLType,TRUE);
		this->CtrlShowHide(m_arConLType, FALSE);
		if(nLType == 0)
		 //m_wndLoadType.SetCurSel(1);
		 CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUniform, 1);
	}


	//IDC_TM_STATIC3
	//IDC_TM_DX_STR
	//IDC_TM_DY_STR
	//IDC_TM_DZ_STR
	//IDC_TM_FORCP_STR
	//IDC_TM_DX_EDIT
	//IDC_TM_DY_EDIT
	//IDC_TM_DZ_EDIT
	//IDC_TM_FORCP_EDIT
	//IDC_DX_UNIT_STATIC1
	//IDC_DY_UNIT_STATIC1
	//IDC_DZ_UNIT_STATIC1
	//IDC_TM_FORCEP_UNIT
}

void CStldPresDlg::OnTmDefineLdgrBtn() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

