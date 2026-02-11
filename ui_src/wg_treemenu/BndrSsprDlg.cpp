// BndrSsprDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSsprDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSsprDlg dialog

CBndrSsprDlg::CBndrSsprDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrSsprDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrSsprDlg)
	m_nConvertSpringType = 0;
	//m_nElementType = 0;

	m_nConvertType = 0;
	m_nElemSelType = 0;
	m_nElemSelSelection = 1;

	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_bUseSsprCalcDlg = FALSE;

#if defined (_MGEN_ORG)
	m_bUseSsprCalcDlg = TRUE;
#endif

	m_arCtrlLink.Add(IDC_TM_TYPE_STC2);
	m_arCtrlLink.Add(IDC_TM_TYPE_CMB2);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_DIR_TITLE);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_DIR_COMBO);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_MODULUS2);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_MOD_EDIT);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_MOD_UNIT);
	
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_STC);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_EDT);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_UNT);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LENGTH_TITLE);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LENGTH_EDIT);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_LENGTH_UNIT);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_TENS_CHK);
	m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_COMP_CHK);

	m_aElementType.Add(IDC_TM_BNDR_SSPR_TYPE_TITLE);
	m_aElementType.Add(IDC_TM_ELEM_TYPE_COMBO);
	m_aElementType.Add(IDC_TM_BNDR_SSPR_TYPE_SOLID_CBO);
	m_aElementType.Add(IDC_TM_BNDR_SSPR_WIDTH_TITLE);
	m_aElementType.Add(IDC_TM_BNDR_SSPR_WIDTH_EDIT);
	m_aElementType.Add(IDC_TM_BNDR_SSPR_WIDTH_UNIT);

	m_aConvertType.Add(IDC_TM_BNDR_SSPR_CONVERT_NODAL_SPRING_RDO);
	m_aConvertType.Add(IDC_TM_BNDR_SSPR_CONVERT_DISTRIBUTED_SPRING_RDO);

	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEMENT_SELECTION);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_PROPERTIES);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_TYPE);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_TYPE_CMB);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_MODUL);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_MODUL_EDT);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_MODUL_UNIT);

	if(m_bUseSsprCalcDlg) m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_SPRING_MODUL_BTN);

	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEM_TYPE);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEM_TYPE_CMB);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEM_SELECTION);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO);
	m_aElemSelSpringProp.Add(IDC_TM_BNDR_SSPR_ELEM_SELECTION_ELEMENT_RDO);
 
	m_aFrameType.Add(IDC_TM_BNDR_SSPR_ELEM_FRAME_LOCAL_AXIS);
	m_aFrameType.Add(IDC_TM_BNDR_SSPR_ELEM_FRAME_LOCAL_AXIS_CMB);
	m_aFrameType.Add(IDC_TM_BNDR_SSPR_ELEM_FRAME_WIDTH);
	m_aFrameType.Add(IDC_TM_BNDR_SSPR_ELEM_FRAME_WIDTH_EDT);
	m_aFrameType.Add(IDC_TM_BNDR_SSPR_ELEM_FRAME_WIDTH_UNIT);

	m_aPlanarFace.Add(IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE);
	m_aPlanarFace.Add(IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE_CMB);

	m_aPlanarEdge.Add(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE);
	m_aPlanarEdge.Add(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE_CMB);

	m_aSolidFace.Add(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE);
	m_aSolidFace.Add(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE_CMB);

	m_aElemSelSelection.Add(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO);
	m_aElemSelSelection.Add(IDC_TM_BNDR_SSPR_ELEM_SELECTION_ELEMENT_RDO);

#if defined(_MGEN) || defined(_CIVIL) // CIVIL & GEN // MNET:2866 20070628 mylee
	m_arCtrlSpring_Type.Add(IDC_TM_TYPE_STC);
	m_arCtrlSpring_Type.Add(IDC_TM_TYPE_CMB);

	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_MODULUS1);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_AXIS_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LX_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LX_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LX_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LY_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LY_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LY_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LZ_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LZ_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_LZ_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_PHU_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_PHU_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_PHU_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_DAMPING_CHK);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CX_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CX_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CX_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CY_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CY_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CY_UNIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CZ_STATIC);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CZ_EDIT);
	m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CZ_UNIT);

	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CX_EDIT);
	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CX_UNIT);
	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CY_EDIT);
	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CY_UNIT);
	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CZ_EDIT);
	m_arCtrlSpring_Damping.Add(IDC_TM_BNDR_SSPR_CZ_UNIT);

	m_arCtrlSpring_Non.Add(IDC_TM_DIR_STC);
	m_arCtrlSpring_Non.Add(IDC_TM_DIR_CMB);
	m_arCtrlSpring_Non.Add(IDC_TM_VAL_STC);
	m_arCtrlSpring_Non.Add(IDC_TM_VAL_EDT);
	m_arCtrlSpring_Non.Add(IDC_TM_VAL_UNT);

	for (int i=0 ; i<m_arCtrlSpring_Linear.GetSize() ; ++i) m_arCtrlSpring.Add(m_arCtrlSpring_Linear[i]);
	for (int i=0 ; i<m_arCtrlSpring_Type.GetSize() ; ++i)   m_arCtrlSpring.Add(m_arCtrlSpring_Type[i]);
	for (int i=0 ; i<m_arCtrlSpring_Non.GetSize() ; ++i)    m_arCtrlSpring.Add(m_arCtrlSpring_Non[i]);
	m_arCtrlSpring.Add(IDC_TM_SPRING_GRP);
#else
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_MODULUS1);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_AXIS_STATIC);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LX_STATIC);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LX_EDIT);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LX_UNIT);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LY_STATIC);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LY_EDIT);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LY_UNIT);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LZ_STATIC);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LZ_EDIT);
	m_arCtrlSpring.Add(IDC_TM_BNDR_SSPR_LZ_UNIT);
#endif

	// [PMS 5075] 지반반력계수 계산 편의기능
	if(m_bUseSsprCalcDlg)
	{
		m_arCtrlLink.Add(IDC_TM_BNDR_SSPR_MOD_BTN);
		m_arCtrlSpring_Linear.Add(IDC_TM_BNDR_SSPR_CZ_BTN);
		m_arCtrlSpring_Non.Add(IDC_TM_VAL_BTN);
	}
	
	m_SosdD.Initialize();
}

CBndrSsprDlg::~CBndrSsprDlg()
{
}

void CBndrSsprDlg::Execute()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (m_nConvertType == 0) // Convert to Nodal Spring
	{
		CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
		CArray<int, int> rFace;
		T_ELEM_D ElemData;

		Sspring Sspr;
		if (!GetParameter(Sspr))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Surface_Spring_Data));
			return;
		}

		if (m_CobxElemType.GetCurSel() != 3)
		{
			pIGM->GetSelectedElemKeyList(arElemKeyList);
			/*
			int nSelElem = arElemKeyList.GetSize();
			for( int i = 0;i < nSelElem; i++)
			{
				rFace.Add(Sspr.nFace);
			}
			*/
		}
		else
		{
			CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
			CArray<T_ELEM_K, T_ELEM_K> aElemList;
			pIGM->GetSelectedNodeKeyList(aSelNodeKey);
			pIGM->GetActiveElemKeys(aElemList);
			CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, aSelNodeKey, arElemKeyList, rFace);
		}

		if (Sspr.nElType == 0) //Frame
		{
			for (int i = arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Frame타입만 선택한다.
			{
				m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i - 1), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
					arElemKeyList.RemoveAt(i - 1);
			}

			if (!arElemKeyList.GetSize())
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
				return;
			}
		}
		else if (Sspr.nElType == 1)  // Planar 일때.
		{
			for (int i = arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Plane타입만 선택한다.
			{
				m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i - 1), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
					arElemKeyList.RemoveAt(i - 1);
			}

			if (!arElemKeyList.GetSize())
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_planar_element_));
				return;
			}
		}
		else if (m_CobxElemType.GetCurSel() == 2) // Solid (Face) 일때.
		{
			for (int i = arElemKeyList.GetSize(); i > 0; i--)  // 선택된 Element 중에서 Solid타입만 선택한다.
			{
				m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList.GetAt(i - 1), ElemData);
				if (!m_pDoc->m_pAttrCtrl->IsSolidType(ElemData.eltyp))
				{
					arElemKeyList.RemoveAt(i - 1);
				}
				else
				{
					//if(m_CobxElemType.GetCurSel() == 2) // Solid Face
					//{
					rFace.Add(Sspr.nFace);
					//}
				}
			}

			if (!arElemKeyList.GetSize())
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
				return;
			}
		}

		////////////// surface spring ////////////////
		// nElType : 0=Frame 1=Planar 2=Solid
		// nFace : Solid인 경우 Face Number
		// nConvertType : 1=Point Support 2=Elastic Link
		// nSpringType : Point Spring인 경우, 1=Linear 2=Comp 3=Tens
		// dSx,dSy,dSz : Point Support일경우, 절점로컬방향별 지반반력계수, Elastic Link일경우, dSx만시용 , Point NonLinear인 경우도 dSx만 사용

		// nLinkType : Elastic Link일경우, 0=General 1=Tens Only 2=Comp Only
		// dLength : Elastic Link일경우, Elastic Link의 길이
		// nDirection,dux,duy,duz : Elastic Link일경우, Point NonLinear경우도 nDirection : 1=Normal(+), 2=Normal(-) 3=dux,duy,duz방향

		//BOOL CDataCtrl::MakeSurfaceSpring(T_BNGR_K KeyBngr, CArray<T_ELEM_K, T_ELEM_K>& rKey, 
		//    int nElType, 
		//    CArray<int, int>& rFace, 
		//    int nConvertType, 
		//    double dSx, 
		//    double dSy, 
		//    double dSz,int nLinkType,
		//    double dLength,
		//    int nDirection, double dux, double duy, double duz)

		T_BNGR_K KeyBngr;
		m_wndGroupCombo.GetSelectedBngr(KeyBngr);

		if (m_pDoc->m_pDataCtrl->MakeSurfaceSpring(KeyBngr, arElemKeyList, Sspr.nElType, rFace, Sspr.nConvertType, Sspr.nSpringType,
			Sspr.dSx, Sspr.dSy, Sspr.dSz, Sspr.dPHU, Sspr.nLinkType, Sspr.dWidth, Sspr.dLength,
			Sspr.nDirection, Sspr.dUx, Sspr.dUy, Sspr.dUz, FALSE, Sspr.bDamping, Sspr.dCx, Sspr.dCy, Sspr.dCz))
		{
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		}
	}

	else // Convert to Distributed Spring
	{
		Dlg2Data();

		m_wndGroupCombo.GetSelectedBngr(m_Data.GroupKey);

		CArray<UINT, UINT> aSelKey;
		CArray<int, int > arFaceOrEdge;
		if (!GetElemByType(aSelKey, arFaceOrEdge)) return;
		BOOL bSuccess;

		if (m_nElemSelSelection == 0) // By Node 
		{
			BOOL bTemp;
			int nElem = aSelKey.GetSize();
			int nFE = arFaceOrEdge.GetSize();
			ASSERT(nElem == nFE);

			for (int i = 0; i < nElem; i++)
			{
				m_Data.nEdgeFace = arFaceOrEdge[i] - 1;
				bTemp = CDBDoc::GetDocPoint()->m_pDataCtrl->AddSsps(aSelKey[i], m_Data, FALSE);
				bSuccess = bTemp;
			}
		}
		else // By Element 
		{
			bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddSsps(aSelKey, m_Data, FALSE);
		}
		if (bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CBndrSsprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSsprDlg)
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_WIDTH_UNIT, m_wndWidthUnit);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_WIDTH_EDIT, m_wndWidth);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE_COMBO, m_CobxElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_TYPE_SOLID_CBO, m_cboElemType);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LX_UNIT, m_unitSpringX);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LY_UNIT, m_unitSpringY);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LZ_UNIT, m_unitSpringZ);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_PHU_UNIT, m_unitPHU);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LX_EDIT, m_edtSpringX);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LY_EDIT, m_edtSpringY);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LZ_EDIT, m_edtSpringZ);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_PHU_EDIT, m_edtPHU);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CX_UNIT, m_unitDampingX);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CY_UNIT, m_unitDampingY);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CZ_UNIT, m_unitDampingZ);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CX_EDIT, m_edtDampingX);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CY_EDIT, m_edtDampingY);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_CZ_EDIT, m_edtDampingZ);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_DAMPING_CHK, m_chkDamping);
// 	DDX_Control(pDX, IDC_TM_BNDR_SSPR_TENS_CHK, m_chkTension);
// 	DDX_Control(pDX, IDC_TM_BNDR_SSPR_COMP_CHK, m_chkCompression);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LENGTH_EDIT, m_edtLinkLength);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LENGTH_UNIT, m_unitLinkLength);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_MOD_UNIT, m_unitLinkModulus);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_MOD_EDIT, m_edtLinkModulus);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_EDT, m_edtLinkLimitStr);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_UNT, m_untLinkLimitStr);
	DDX_Control(pDX, IDC_TM_TYPE_CMB2, m_cmbSpringType2);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_DIR_COMBO, m_cboDirection);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_BNDR_SSPR_CONVERT_POINT, m_nConvertSpringType);
	//DDX_Radio(pDX, IDC_TM_BNDR_SSPR_TYPE_PLANAR, m_nElementType);

	DDX_Control(pDX, IDC_TM_TYPE_CMB, m_cmbSpringType);
	DDX_Control(pDX, IDC_TM_DIR_CMB, m_cmbDirectionNon);
	DDX_Control(pDX, IDC_TM_VAL_UNT, m_untNonLinearModulus);
	DDX_Control(pDX, IDC_TM_VAL_EDT, m_edtNonLinearModulus);

	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_TYPE_CMB,       m_cmbElemSelType);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_FRAME_LOCAL_AXIS_CMB, m_cmbLoaclAxis);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE_CMB, m_cmbPlanarFace);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE_CMB, m_cmbPlanarEdge);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE_CMB, m_cmbSolidFace);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_FRAME_WIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_ELEM_FRAME_WIDTH_UNIT, m_unitWidth);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_SPRING_TYPE_CMB, m_cmbSpringProp);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_SPRING_MODUL_EDT, m_edtSpringPropModulus);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_SPRING_MODUL_UNIT, m_unitSpringPropModulus);
	DDX_Control(pDX, IDC_TM_BNDR_SSPR_BITMAP, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrSsprDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrSsprDlg)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_CONVERT_POINT, OnTmBndrSsprConvertType)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_DAMPING_CHK, OnTmBndrSsprDampingChk)
// 	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_TENS_CHK, OnTmBndrSsprTensChk)
// 	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_COMP_CHK, OnTmBndrSsprCompChk)
	//ON_BN_CLICKED(IDC_TM_BNDR_SSPR_TYPE_PLANAR, OnTmBndrSsprTypePlanar)
	//ON_BN_CLICKED(IDC_TM_BNDR_SSPR_TYPE_SOLID, OnTmBndrSsprTypeSolid)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SSPR_TYPE_SOLID_CBO, OnSelchangeTmBndrSsprTypeSolidCbo)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_CONVERT_ELASTIC, OnTmBndrSsprConvertType)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE_COMBO, OnSelchangeTmElemTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_TYPE_CMB, OnSelchangeSpringTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_TYPE_CMB2, OnSelchangeSpringTypeCombo2)

	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_CONVERT_NODAL_SPRING_RDO,       OnSelectChangeConvertType)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_CONVERT_DISTRIBUTED_SPRING_RDO, OnSelectChangeConvertType)
	ON_CBN_SELCHANGE(IDC_TM_BNDR_SSPR_ELEM_TYPE_CMB, OnSelectChangeElementType)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO,    OnSelectElemSelNodeorElem)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_ELEM_SELECTION_ELEMENT_RDO, OnSelectElemSelNodeorElem)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_MOD_BTN         , OnClickCalcBtn_Mod)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_CZ_BTN          , OnClickCalcBtn_Cz )
	ON_BN_CLICKED(IDC_TM_VAL_BTN                   , OnClickCalcBtn_Val)
	ON_BN_CLICKED(IDC_TM_BNDR_SSPR_SPRING_MODUL_BTN, OnClickCalcBtn_Spr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBndrSsprDlg::AlignControl()
{
	CRect rectRef;
	CRect rectTarget;

	// MNET:2743 20070423 mylee
#if defined(_CIVIL) || defined(_MGEN) // MNET:2866 20070628 mylee
	GetDlgItem(IDC_TM_TYPE_STC2)->GetWindowRect(&rectRef);
	GetDlgItem(IDC_TM_BNDR_SSPR_DIR_TITLE)->GetWindowRect(&rectTarget);
	double dGap = rectRef.top- rectTarget.top;

	GetDlgItem(IDC_TM_BNDR_SSPR_DIR_TITLE)->GetWindowRect(&rectRef);
	GetDlgItem(IDC_TM_TYPE_STC)->GetWindowRect(&rectTarget);

	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Type, dGap+rectRef.top- rectTarget.top);  // Point spring Linear type 일 때...

	GetDlgItem(IDC_TM_BNDR_SSPR_MODULUS2)->GetWindowRect(&rectRef);
	GetDlgItem(IDC_TM_BNDR_SSPR_MODULUS1)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Linear, dGap+rectRef.top- rectTarget.top);  // Point spring Linear type 일 때...


	GetDlgItem(IDC_TM_DIR_STC)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Non, dGap+rectRef.top- rectTarget.top);  // Elastic Link 일 때...


	GetDlgItem(IDC_TM_BNDR_SSPR_TYPE_TITLE)->GetWindowRect(&rectRef);
	GetDlgItem(IDC_TM_BNDR_SSPR_ELEMENT_SELECTION)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_aElemSelSpringProp, rectRef.top- rectTarget.top);   // Distributed spring (Frame) 일 때...
	CDlgUtil::CtrlMoveDistY(this, m_aFrameType, rectRef.top- rectTarget.top);           // Distributed spring (Frame) 일 때...

	
	GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_FRAME_LOCAL_AXIS)->GetWindowRect(&rectRef);
	GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_aPlanarFace, rectRef.top- rectTarget.top);   // Distributed spring 일 때(Planar Face)...

	GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_aPlanarEdge, rectRef.top- rectTarget.top);   // Distributed spring 일 때(Planar Edge)...
	
	GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE)->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_aSolidFace, rectRef.top- rectTarget.top);   // Distributed spring 일 때(Solid Face)...

//   ((CBCGPStatic*)GetDlgItem(IDC_TM_DIR_STC))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_MODULUS1))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Non, rectTarget.top- rectRef.top);
// 
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_TYPE_STC))->GetWindowRect(&rectRef);	
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_DIR_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Type, rectTarget.top - rectRef.top);
// 	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Linear, rectTarget.top - rectRef.top);
//   CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring_Non, rectTarget.top - rectRef.top);
// 
//   // Convert to Distributed Spring 
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_SPRING_MODUL_EDT))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_aElemSelSpringProp, rectTarget.bottom-rectRef.bottom-10);
// 
//   // Frame  
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_FRAME_LOCAL_AXIS))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_WIDTH_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_aFrameType, rectTarget.top-rectRef.top+25);
// 
//   // Planar(Face)
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_WIDTH_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_aPlanarFace, rectTarget.top-rectRef.top+25);
//     
//   // Planar(Edge)
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_WIDTH_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_aPlanarEdge, rectTarget.top-rectRef.top+25);
//   
//   // Solid(Face)
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE))->GetWindowRect(&rectRef);
//   ((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_WIDTH_TITLE))->GetWindowRect(&rectTarget);
//   CDlgUtil::CtrlMoveDistY(this, m_aSolidFace, rectTarget.top-rectRef.top+25);

#else
	((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_MODULUS1))->GetWindowRect(&rectRef);	
	((CBCGPStatic*)GetDlgItem(IDC_TM_BNDR_SSPR_DIR_TITLE))->GetWindowRect(&rectTarget);
	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSpring, rectTarget.top - rectRef.top);
#endif

	((CButton*)GetDlgItem(IDC_TM_EXECUTE))->GetWindowRect(&rectRef);
 	rectRef.bottom += globalUtils.ScaleByDPI(10);

	GetWindowRect(rectTarget);
	rectTarget.bottom = rectRef.bottom;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(rectTarget);
	MoveWindow(rectTarget);  
}

void CBndrSsprDlg::InitUnitType()
{
	m_unitSpringX    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_unitSpringY    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_unitSpringZ    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_unitPHU        .SetUnitType(D_UNITSYS_BASE_STRESS);
	m_unitLinkModulus.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_untLinkLimitStr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_unitLinkLength .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untNonLinearModulus.SetUnitType(D_UNITSYS_BASE_DENSITY); // MNET:2743 20070424

	m_edtSpringX    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtSpringY    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtSpringZ    .SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtPHU        .SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLinkModulus.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtLinkLimitStr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtNonLinearModulus.SetUnitType(D_UNITSYS_BASE_DENSITY); // MNET:2743 20070424

	m_unitDampingX   .SetUnitType(D_UNITSYS_BASE_FSEC_L3);
	m_unitDampingY   .SetUnitType(D_UNITSYS_BASE_FSEC_L3);
	m_unitDampingZ   .SetUnitType(D_UNITSYS_BASE_FSEC_L3);
	m_edtDampingX    .SetUnitType(D_UNITSYS_BASE_FSEC_L);
	m_edtDampingY    .SetUnitType(D_UNITSYS_BASE_FSEC_L);
	m_edtDampingZ    .SetUnitType(D_UNITSYS_BASE_FSEC_L);

	m_edtLinkLength.SetAttUcsDistance();
	m_edtLinkLength.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtLinkLength.SetDistValueMode(TRUE);
	m_edtLinkLength.SetNextLink(NULL);

	m_wndWidth    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndWidthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// Element Selection Width 
	m_unitWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtWidth .SetUnitType(D_UNITSYS_BASE_LENGTH);

	// Spring Properties
	m_unitSpringPropModulus.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_edtSpringPropModulus. SetUnitType(D_UNITSYS_BASE_DENSITY);

	// Default Value
	m_edtSpringX.SetWindowText(_T("0"));
	m_edtSpringY.SetWindowText(_T("0"));
	m_edtSpringZ.SetWindowText(_T("0"));
	m_edtPHU.SetWindowText(_T("0"));
	m_edtLinkModulus.SetWindowText(_T("0"));
	m_edtLinkLimitStr.SetWindowText(_T("0"));

	m_cboDirection   .SetCurSel(0);
	m_cmbDirectionNon.SetCurSel(0); // MNET:2743 20070423 mylee
	m_edtLinkLength  .SetWindowText(_T("0"));
}

void CBndrSsprDlg::InitCmbData()
{
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Frame)     );     m_CobxElemType.SetItemData(0,0);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Planar)    );     m_CobxElemType.SetItemData(1,1);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Face_));    m_CobxElemType.SetItemData(2,2);
	m_CobxElemType.AddString(_LS(IDS_TM_SSPR_Solid_Node_));    m_CobxElemType.SetItemData(3,2);

	m_CobxElemType.SetCurSel(1);
	
	// Element Type의 Face ComboBox 초기화.
	CString strElemType[] = {_LS(IDS_WG_TREEMENU_Face__1),_LS(IDS_WG_TREEMENU_Face__2),_LS(IDS_WG_TREEMENU_Face__3),
													 _LS(IDS_WG_TREEMENU_Face__4),_LS(IDS_WG_TREEMENU_Face__5),_LS(IDS_WG_TREEMENU_Face__6)}; 
	for (int i=0; i<6; i++)
	{
		m_cboElemType.AddString(strElemType[i]);
	}
	m_cboElemType.SetCurSel(0);

	// Type Combo 초기화 - 20070423 mylee
	CString strSpringType[] = {_LS(IDS_TM_WORKTREE_Linear), _LS(IDS_WG_TREEMENU_Comp_Only), _LS(IDS_WG_TREEMENU_Tens_Only), _LS(IDS_WG_TREEMENU_Multi_Linear_bi)};
	for(int i=0; i<4; i++)
	{
		m_cmbSpringType.AddString(strSpringType[i]);
		m_cmbSpringType2.AddString(strSpringType[i]);
	}
	m_cmbSpringType.SetCurSel(0);
	m_cmbSpringType2.SetCurSel(0);
	
	// Direction ComboBox 초기화.
	CString strDirection[] = {_LS(IDS_WG_TREEMENU_Normal___),_LS(IDS_WG_TREEMENU_Normal____MSG1),_LS(IDS_WG_TREEMENU_UCS_x___),_LS(IDS_WG_TREEMENU_UCS_x____MSG2),
		                        _LS(IDS_WG_TREEMENU_UCS_y___) ,_LS(IDS_WG_TREEMENU_UCS_y____MSG3) ,_LS(IDS_WG_TREEMENU_UCS_z___),_LS(IDS_WG_TREEMENU_UCS_z____MSG4) };
	for(int i=0; i< 8; i++)
	{
		m_cboDirection   .AddString(strDirection[i]);
		m_cmbDirectionNon.AddString(strDirection[i]); // MNET:2743 20070423 mylee
	}
	
	//Element Selection Type
	CString strElemSelType[] = {_LS(IDS_TM_SSPR_Frame),_LS(IDS_TM_SSPR_Planar_Face),_LS(IDS_TM_SSPR_Planar_Edge),_LS(IDS_TM_SSPR_Solid_Face_)};
	for(int i=0; i<4; i++)
	{
		m_cmbElemSelType .AddString(strElemSelType[i]);
	}
	m_cmbElemSelType.SetCurSel(0);

	// Local Axis
	CDlgUtil::CobxAddItem(m_cmbLoaclAxis, _LS(IDS_WG_TREEMENU_Local_x), D_SSPS_FRAME_LOCAL_X);
	CDlgUtil::CobxAddItem(m_cmbLoaclAxis, _LS(IDS_WG_TREEMENU_Local_y), D_SSPS_FRAME_LOCAL_Y);
	CDlgUtil::CobxAddItem(m_cmbLoaclAxis, _LS(IDS_WG_TREEMENU_Local_z), D_SSPS_FRAME_LOCAL_Z);
	CDlgUtil::CobxSetCurSelItemData(m_cmbLoaclAxis, D_SSPS_FRAME_LOCAL_Z);

	//Planar(Face)
	m_cmbPlanarFace.AddString(_LS(IDS_WG_TREEMENU_Face__1));
	m_cmbPlanarFace.SetCurSel(0);

	// Planar(Edge)
	CString strPlanarEdge[] = {_LS(IDS_WG_TREEMENU_Edge__1),_LS(IDS_WG_TREEMENU_Edge__2),_LS(IDS_WG_TREEMENU_Edge__3),_LS(IDS_WG_TREEMENU_Edge__4)};
	for(int i=0; i<4; i++)
	{
		m_cmbPlanarEdge.AddString(strPlanarEdge[i]);
	}
	m_cmbPlanarEdge.SetCurSel(0);

	// Solid(Face)
	CString strSolidFace[] = {_LS(IDS_WG_TREEMENU_Face__1),_LS(IDS_WG_TREEMENU_Face__2),_LS(IDS_WG_TREEMENU_Face__3),
														_LS(IDS_WG_TREEMENU_Face__4),_LS(IDS_WG_TREEMENU_Face__5),_LS(IDS_WG_TREEMENU_Face__6)};
	for(int i=0; i<6; i++)
	{
		m_cmbSolidFace.AddString(strSolidFace[i]);
	}
	m_cmbSolidFace.SetCurSel(0);

	// Spring Properties
	CString strSpringProp[] = {_LS(IDS_TM_WORKTREE_Linear), _LS(IDS_WG_TREEMENU_Comp_Only), _LS(IDS_WG_TREEMENU_Tens_Only)};
	for(int i=0; i<3; i++)
	{
		m_cmbSpringProp.AddString(strSpringProp[i]);
	}
	m_cmbSpringProp.SetCurSel(0);
}

void CBndrSsprDlg::ChangeBitmap()
{
	
	CString aBitmapID[] = { 
		_T("SVG\\Illustration\\Dialog\\tm_soil1.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_soil2.svg")
	};
	CString aBitmapID1[] = { 
		_T("SVG\\Illustration\\Dialog\\Frame.svg"),			 
		_T("SVG\\Illustration\\Dialog\\Planar(Face).svg"),
		_T("SVG\\Illustration\\Dialog\\Planar(Edge).svg"),	 
		_T("SVG\\Illustration\\Dialog\\Solid(Face).svg") 
	};
	if (m_nConvertType == 0)
	{
		m_wndPicture.SetImage(aBitmapID[m_nConvertSpringType]);
	}
	else
	{
		m_wndPicture.SetImage(aBitmapID1[m_nElemSelType]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBndrSsprDlg message handlers

BOOL CBndrSsprDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	if(!m_bUseSsprCalcDlg)
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_MOD_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BNDR_SSPR_CZ_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_VAL_BTN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BNDR_SSPR_SPRING_MODUL_BTN)->ShowWindow(SW_HIDE);
	}

	m_nConvertSpringType = 0;
	m_nSpringType  = 0;
	m_nSpringType2 = 0;
	m_nElemSelType = 0;

	CDlgUtil::CtrlRadioSetCheck(this, m_aConvertType, m_nConvertType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aElemSelSelection, m_nElemSelSelection);

	GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO)->EnableWindow(FALSE);

	UpdateData(FALSE);
	// TODO: Add extra initialization here
	AlignControl();
	InitUnitType();
	InitCmbData();

	OnSelectElemSelNodeorElem();
	ControlsShowHide();
	EnableDisableControls();
	OnTmBndrSsprDampingChk();

	m_Data.Initialize();
	
	Data2Dlg();
	
	// Default Value
	m_edtSpringPropModulus.SetWindowText(_T(""));
	m_edtWidth.SetWindowText(_T(""));
	ChangeBitmap();
	
	//CheckMode(m_nElementType);
	CheckMode(TRUE);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrSsprDlg::OnTmBndrSsprConvertType() 
{
	InitUnitType();  
	UpdateData(TRUE);
	ControlsShowHide();
	ChangeBitmap();
}

BOOL CBndrSsprDlg::GetParameter(Sspring& Sspr)
{
	UpdateData(TRUE);

	//Sspr.nElType = m_nElementType + 1;
	//Sspr.nElType = m_CobxElemType.GetCurSel()+1;
	Sspr.nElType = (int)m_CobxElemType.GetItemData(m_CobxElemType.GetCurSel());
	Sspr.nFace   = m_cboElemType.GetCurSel() + 1;

	Sspr.nConvertType = m_nConvertSpringType+1;

	Sspr.dWidth = m_wndWidth.GetEditValue();

	if(Sspr.nConvertType == 1) // Point Support 일경우.
	{
		Sspr.nSpringType = m_nSpringType+1;

		if (Sspr.nSpringType == 1 || Sspr.nSpringType == 4) // Linear
		{
			Sspr.dSx = m_edtSpringX.GetEditValue();
			Sspr.dSy = m_edtSpringY.GetEditValue();
			Sspr.dSz = m_edtSpringZ.GetEditValue();
			Sspr.dPHU = m_edtPHU.GetEditValue();

			Sspr.bDamping = m_chkDamping.GetCheck() ? TRUE : FALSE;
			Sspr.dCx      = m_edtDampingX.GetEditValue();
			Sspr.dCy      = m_edtDampingY.GetEditValue();
			Sspr.dCz      = m_edtDampingZ.GetEditValue();

			Sspr.nLinkType = 0;
			Sspr.dUx = 0.;
			Sspr.dUy = 0.;
			Sspr.dUz = 0.;
			Sspr.nDirection = 0;
		}
		else // NonLinear - Rigid Link 변수 공유
		{
			Sspr.dSx = m_edtNonLinearModulus.GetEditValue();
			Sspr.dSy = 0.;
			Sspr.dSz = 0.;
			Sspr.dLength = 0.;

			Sspr.bDamping = FALSE;
			Sspr.dCx      = 0.;
			Sspr.dCy      = 0.;
			Sspr.dCz      = 0.;

			Sspr.dUx = 0.;
			Sspr.dUy = 0.;
			Sspr.dUz = 0.;
			Sspr.nDirection = 3;
			switch(m_cmbDirectionNon.GetCurSel())
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
		}
	}
	else if(Sspr.nConvertType == 2) // Rigid Link 일경우.
	{
		Sspr.nLinkType = m_nSpringType2;

		Sspr.dSx = m_edtLinkModulus.GetEditValue();
		Sspr.dPHU = m_edtLinkLimitStr.GetEditValue();
		Sspr.dSy = 0.;
		Sspr.dSz = 0.;
		Sspr.dLength = 0.;
		Sspr.bDamping = FALSE;
		Sspr.dCx      = 0.;
		Sspr.dCy      = 0.;
		Sspr.dCz      = 0.;


// 		if(!m_chkTension.GetCheck() && !m_chkCompression.GetCheck())
// 			Sspr.nLinkType = 0;
// 		else
// 		{
// 			if(m_chkTension.GetCheck())
// 				Sspr.nLinkType = 1;
// 			else
// 				Sspr.nLinkType = 2;
// 		}

		CString strTempLength;
		m_edtLinkLength.GetWindowText(strTempLength);
		if(!GetFloatNumber(strTempLength, Sspr.dLength)) return FALSE;	

		Sspr.dUx = 0.;
		Sspr.dUy = 0.;
		Sspr.dUz = 0.;
		Sspr.nDirection = 3;
		switch(m_cboDirection.GetCurSel())
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
	}

	return TRUE;
}

BOOL CBndrSsprDlg::GetElemByType(CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if(m_nElemSelSelection == 0) // Selection(Node)
	{
		CArray<T_ELEM_K,T_ELEM_K> aElemList; // Select된 element list
		CArray<T_NODE_K, T_NODE_K> aSelNodeKey;
		
		pIGM->GetSelectedNodeKeyList(aSelNodeKey);
		pIGM->GetActiveElemKeys(aElemList);
		
		if(m_nElemSelType == 2) // Planar(Edge)
		{
			CDBDoc::GetDocPoint()->SelectEdgeByNode(aElemList, aSelNodeKey, aSelKey, arFaceOrEdge);
		}
		else if(m_nElemSelType == 3) // Solid(Face)
		{
			 CDBDoc::GetDocPoint()->SelectFaceByNode(aElemList, aSelNodeKey, aSelKey, arFaceOrEdge);
		}
	}

	else // Selection(Element)
	{
		pIGM->GetSelectedElemKeyList(aSelKey);
	}

	T_ELEM_D ElemData;

	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	for (int i=aSelKey.GetSize()-1; i>=0; i--)
	{
		pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);

		if (m_nElemSelType == 0) // Frame
		{
			if (!pAttrCtrl->IsTruss(ElemData.eltyp)     && !pAttrCtrl->IsTensTruss(ElemData.eltyp) &&
					!pAttrCtrl->IsCompTruss(ElemData.eltyp) && !pAttrCtrl->IsBeam(ElemData.eltyp))
			{
				aSelKey.RemoveAt(i);
				GSaveHistoryFormatNF(_LS(IDS_TM_SSPS_Frame_Error));
				return FALSE;
			}  
		}
		else if (m_nElemSelType == 1) // Planar(Face)
		{
			if(pAttrCtrl->IsWall(ElemData.eltyp))
			{
				GSaveHistoryFormatNF(_LS(IDS_TM_SSPS_Wall_Only));
				return FALSE;
			}

			if (!pAttrCtrl->IsPlate(ElemData.eltyp)  && !pAttrCtrl->IsPlstrn(ElemData.eltyp) && !pAttrCtrl->IsPlstrs(ElemData.eltyp) &&
					!pAttrCtrl->IsAxisym(ElemData.eltyp) && !pAttrCtrl->IsWall(ElemData.eltyp))
			{
				aSelKey.RemoveAt(i);
				GSaveHistoryFormatNF(_LS(IDS_TM_SSPS_Planar_Face_Error));
				return FALSE;
			}
		}

		else if (m_nElemSelType == 2) // Planar(Edge)
		{
			if (!pAttrCtrl->IsPlate(ElemData.eltyp)  && !pAttrCtrl->IsPlstrn(ElemData.eltyp) && !pAttrCtrl->IsPlstrs(ElemData.eltyp) &&
					!pAttrCtrl->IsAxisym(ElemData.eltyp) && !pAttrCtrl->IsWall(ElemData.eltyp))
			{
				aSelKey.RemoveAt(i);
				if(m_nElemSelSelection == 0) // Node
				{
					arFaceOrEdge.RemoveAt(i);
				}
				GSaveHistoryFormatNF(_LS(IDS_TM_SSPS_Planar_Edge_Error));
				return FALSE;
			}
		}

		else // Solid
		{
			if(!pAttrCtrl->IsSolid(ElemData.eltyp))
			{
				aSelKey.RemoveAt(i);
				if(m_nElemSelSelection == 0) // Node
				{
					arFaceOrEdge.RemoveAt(i);
				}
				GSaveHistoryFormatNF(_LS(IDS_TM_SSPS_Solid_Error));
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CBndrSsprDlg::OnTmBndrSsprDampingChk()
{
	BOOL bEnableDamping=FALSE;
	if(m_chkDamping.GetCheck()) bEnableDamping=TRUE;
	
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlSpring_Damping, bEnableDamping);
}
// void CBndrSsprDlg::OnTmBndrSsprTensChk() 
// {
// 	if(m_chkCompression.GetCheck() && m_chkTension.GetCheck())
// 		m_chkCompression.SetCheck(FALSE);	
// }
// 
// void CBndrSsprDlg::OnTmBndrSsprCompChk() 
// {
// 	if(m_chkTension.GetCheck() && m_chkCompression.GetCheck())
// 		m_chkTension.SetCheck(FALSE);
// }

// void CBndrSsprDlg::OnTmBndrSsprTypePlanar() 
// {
// 	UpdateData(TRUE);
//   m_cboElemType.EnableWindow(FALSE);	
// 	CheckMode(FALSE);
// }
// 
// void CBndrSsprDlg::OnTmBndrSsprTypeSolid() 
// {
//   UpdateData(TRUE);
// 	m_cboElemType.EnableWindow(TRUE);	
// 	CheckMode(TRUE);
// }

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

void CBndrSsprDlg::CheckMode(BOOL bCheck)
{
	if(bCheck)
	{
		UpdateData(TRUE);
		m_pDoc = CDBDoc::GetDocPoint();

		int nSel = m_CobxElemType.GetCurSel();

		if(nSel == 0) 
			m_wndWidth.EnableWindow(TRUE);
		else
			m_wndWidth.EnableWindow(FALSE);
		
		m_cboElemType.EnableWindow(nSel == 2);
		
		if(m_pDoc)
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

				 m_pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
			   m_pDoc->m_pViewCtrl->SetPressureInputParam(PresInputMode, m_cboElemType.GetCurSel());	
			}
			else
			{
				m_pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
				m_pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
			}
		}
	}
	else
	{
		m_pDoc = CDBDoc::GetDocPoint();
		if(m_pDoc)
		{
		  m_pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
			m_pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

void CBndrSsprDlg::OnSelchangeTmBndrSsprTypeSolidCbo() 
{
	CheckMode(TRUE);	
}

void CBndrSsprDlg::OnSelchangeTmElemTypeCombo() 
{
	CheckMode(TRUE);
}

void CBndrSsprDlg::OnSelchangeSpringTypeCombo() 
{
	m_nSpringType = m_cmbSpringType.GetCurSel();
	ControlsShowHide();
	EnableDisableControls();
}

void CBndrSsprDlg::OnSelchangeSpringTypeCombo2() 
{
	m_nSpringType2 = m_cmbSpringType2.GetCurSel();
	ControlsShowHide();
	EnableDisableControls();
}

void CBndrSsprDlg::PostNcDestroy() 
{
	CheckMode(FALSE);
	m_wndPicture.CloseDialog();
	CMenuBarChildDlg::PostNcDestroy();
}

void CBndrSsprDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);	
}

void CBndrSsprDlg::OnSelectChangeConvertType()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlRadioGetCheck(this, m_aConvertType, m_nConvertType);

	if (m_nConvertType == 0)
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_CONVERT_POINT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_BNDR_SSPR_CONVERT_ELASTIC)->EnableWindow(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_CONVERT_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_BNDR_SSPR_CONVERT_ELASTIC)->EnableWindow(FALSE);
	}
 	ControlsShowHide();
	EnableDisableControls();
	ChangeBitmap();
}

void CBndrSsprDlg::ControlsShowHide()
{
	CDlgUtil::CtrlShowHide(this, m_aElementType,        m_nConvertType == 0);  
	CDlgUtil::CtrlShowHide(this, m_aElemSelSpringProp,  m_nConvertType == 1);

	CDlgUtil::CtrlShowHide(this, m_arCtrlSpring,        m_nConvertType == 0 && m_nConvertSpringType == 0);
	CDlgUtil::CtrlShowHide(this, m_arCtrlLink,          m_nConvertType == 0 && m_nConvertSpringType == 1);
	CDlgUtil::CtrlShowHide(this, m_arCtrlSpring_Linear, m_nConvertType == 0 && m_nConvertSpringType == 0 && (m_nSpringType == 0 || m_nSpringType == 3));
	CDlgUtil::CtrlShowHide(this, m_arCtrlSpring_Non,    m_nConvertType == 0 && m_nConvertSpringType == 0 && (m_nSpringType == 1 || m_nSpringType == 2));
	CDlgUtil::CtrlShowHide(this, m_arCtrlSpring_Non,    m_nConvertType == 0 && m_nConvertSpringType == 0 && (m_nSpringType == 1 || m_nSpringType == 2));

	CDlgUtil::CtrlShowHide(this, m_aFrameType,          m_nConvertType == 1 && m_nElemSelType == 0);
	CDlgUtil::CtrlShowHide(this, m_aPlanarFace,         m_nConvertType == 1 && m_nElemSelType == 1);
	CDlgUtil::CtrlShowHide(this, m_aPlanarEdge,         m_nConvertType == 1 && m_nElemSelType == 2);
	CDlgUtil::CtrlShowHide(this, m_aSolidFace,          m_nConvertType == 1 && m_nElemSelType == 3);
}

void CBndrSsprDlg::EnableDisableControls()
{
	int nType = m_cmbSpringType.GetCurSel();

	GetDlgItem(IDC_TM_BNDR_SSPR_PHU_STATIC)->EnableWindow(nType==3);
	GetDlgItem(IDC_TM_BNDR_SSPR_PHU_EDIT)->EnableWindow(nType==3);
	GetDlgItem(IDC_TM_BNDR_SSPR_PHU_UNIT)->EnableWindow(nType==3);

	nType = m_cmbSpringType2.GetCurSel();
	GetDlgItem(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_STC)->EnableWindow(nType==3);
	GetDlgItem(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_EDT)->EnableWindow(nType==3);
	GetDlgItem(IDC_TM_BNDR_SSPR_LIMIT_STRENGTH_UNT)->EnableWindow(nType==3);
}

void CBndrSsprDlg::OnSelectChangeElementType()
{
	m_nElemSelType = m_cmbElemSelType.GetCurSel();

	if(m_nElemSelType == 0)
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO)->EnableWindow(FALSE);

		m_nElemSelSelection = 1;
		CDlgUtil::CtrlRadioSetCheck(this, m_aElemSelSelection, m_nElemSelSelection);
	}
	else if (m_nElemSelType == 1)
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_FACE_CMB)->EnableWindow(FALSE);

		m_nElemSelSelection = 1;
		CDlgUtil::CtrlRadioSetCheck(this, m_aElemSelSelection, m_nElemSelSelection);
	}
	else if (m_nElemSelType == 2)
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE_CMB)->EnableWindow(FALSE);

		m_nElemSelSelection = 0;
		CDlgUtil::CtrlRadioSetCheck(this, m_aElemSelSelection, m_nElemSelSelection);
	}
	else 
	{
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SELECTION_NODE_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE_CMB)->EnableWindow(FALSE);

		m_nElemSelSelection = 0;
		CDlgUtil::CtrlRadioSetCheck(this, m_aElemSelSelection, m_nElemSelSelection);

	}

	ControlsShowHide();
	ChangeBitmap();
}

void CBndrSsprDlg::OnSelectElemSelNodeorElem()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlRadioGetCheck(this, m_aElemSelSelection, m_nElemSelSelection);

	if (m_nElemSelType == 2)
	{
		if(m_nElemSelSelection == 0)
		{
			GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE_CMB)->EnableWindow(FALSE);
		}
		else 
		{
			GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_PLANAR_EDGE_CMB)->EnableWindow(TRUE);
		}
	}
	
	if (m_nElemSelType == 3)
	{
		if(m_nElemSelSelection == 0)
		{
			GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE_CMB)->EnableWindow(FALSE);
		}
		else 
		{
			GetDlgItem(IDC_TM_BNDR_SSPR_ELEM_SOLID_FACE_CMB)->EnableWindow(TRUE);
		}
	}
}

BOOL CBndrSsprDlg::Dlg2Data()
{
	m_Data.nElementType = m_nElemSelType;
	m_Data.nSelection   = m_nElemSelSelection;
	
	if (m_nElemSelType == 0)
	{
		m_Data.nEdgeFace = CDlgUtil::CobxGetCurSelItemData(m_cmbLoaclAxis, m_cmbLoaclAxis.GetCurSel());    
	}
	else if (m_nElemSelType == 1)
	{
		m_Data.nEdgeFace = m_cmbPlanarFace.GetCurSel();
	}
	else if (m_nElemSelType == 2)
	{
		m_Data.nEdgeFace = m_cmbPlanarEdge.GetCurSel();
	}
	else 
	{
		m_Data.nEdgeFace = m_cmbSolidFace.GetCurSel(); 
	}

	m_Data.dWidth = m_edtWidth.GetEditValue();
	m_Data.nSpringType = m_cmbSpringProp.GetCurSel();
	m_Data.dModulus = m_edtSpringPropModulus.GetEditValue();

	return TRUE;
}

BOOL CBndrSsprDlg::Data2Dlg()
{
	m_nElemSelType = m_Data.nElementType;
	m_nElemSelSelection = m_Data.nSelection;

	if (m_nElemSelType == 0)
	{
		//m_cmbLoaclAxis.SetCurSel(m_Data.nEdgeFace);
		//m_cmbLoaclAxis.SetCurSel(D_SSPS_FRAME_LOCAL_Z); //Defalut Local-z
		CDlgUtil::CobxSetCurSelItemData(m_cmbLoaclAxis, D_SSPS_FRAME_LOCAL_Z);
	}
	else if (m_nElemSelType == 1)
	{
		m_cmbPlanarFace.SetCurSel(m_Data.nEdgeFace);
	}
	else if (m_nElemSelType == 2)
	{
		m_cmbPlanarEdge.SetCurSel(m_Data.nEdgeFace);
	}
	else 
	{
		m_cmbSolidFace.SetCurSel(m_Data.nEdgeFace);
	}

	m_edtWidth.SetEditUnit(m_Data.dWidth);
	m_cmbSpringProp.SetCurSel(m_Data.nSpringType);
	m_edtSpringPropModulus.SetEditUnit(m_Data.dModulus);

	return TRUE;
}

void CBndrSsprDlg::OnClickCalcBtn_Mod()
{
	int nType = 0;
	if     (m_cmbSpringType2.GetCurSel() == 0) nType = 0;
	else if(m_cmbSpringType2.GetCurSel() == 1) nType = 2;
	else if(m_cmbSpringType2.GetCurSel() == 2) nType = 1;

// 	if(!m_chkTension.GetCheck() && !m_chkCompression.GetCheck())
// 		nType = 0;
// 	else
// 	{
// 		if(m_chkTension.GetCheck())
// 			nType = 1;
// 		else
// 			nType = 2;
// 	}

	CBndrSsprCalcDlg dlg;
	dlg.SetSosdData(m_SosdD);
	dlg.SetSpringType(nType);
	dlg.SetUseAxisOpt(FALSE);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();
		m_edtLinkModulus.SetEditUnit(dVal);
	}
}

void CBndrSsprDlg::OnClickCalcBtn_Cz ()
{
	int nType = m_cmbSpringType.GetCurSel();
	CBndrSsprCalcDlg dlg;
	dlg.SetSosdData(m_SosdD);
	dlg.SetSpringType(nType);
	dlg.SetUseAxisOpt(TRUE);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();

		if     (m_SosdD.nAxis == 0) m_edtSpringX.SetEditUnit(dVal);
		else if(m_SosdD.nAxis == 1) m_edtSpringY.SetEditUnit(dVal);
		else                        m_edtSpringZ.SetEditUnit(dVal);		
	}
}

void CBndrSsprDlg::OnClickCalcBtn_Val()
{
	int nType = m_cmbSpringType.GetCurSel();
	CBndrSsprCalcDlg dlg;
	dlg.SetSosdData(m_SosdD);
	dlg.SetSpringType(nType);
	dlg.SetUseAxisOpt(FALSE);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();
		m_edtNonLinearModulus.SetEditUnit(dVal);
	}
}

void CBndrSsprDlg::OnClickCalcBtn_Spr()
{
	int nType = m_cmbSpringProp.GetCurSel();
	CBndrSsprCalcDlg dlg;
	dlg.SetSosdData(m_SosdD);
	dlg.SetSpringType(nType);
	dlg.SetUseAxisOpt(FALSE);
	if(dlg.DoModal()==IDOK)
	{
		double dVal = dlg.GetCalcValue();
		m_edtSpringPropModulus.SetEditUnit(dVal);
	}
}
