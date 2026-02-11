// CMStageCompSectItemDlg.cpp : implementation file
// 2002. 9. 14    by TAE
////////////////////////////////////////////////////////////////////////////
// 1. 추가하려는 시공합성단면의 Bitmap 그리기
// 2. m_aShapeInfo에 해당 단면의 정보 등록
// 3. "// *^^* 여기 추가" 따라가면서 코딩

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageCompSectItemDlg.h"
#include "CMStageCompSectGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectItemDlg dialog
#define STRING_ACTIVE_STAGE   _LS(IDS_CMD_CSCS__Active_Stage)
#define COMPOSITE_TYPE_1      _ULS('1+2, 3')
#define COMPOSITE_TYPE_2      _ULS('1, 2+3')
#define COMPOSITE_TYPE_U      _LS(IDS_CMD_CSCS_User)
#define COMPOSITE_TYPE_N      _LS(IDS_CMD_CSCS_Normal)
#define COMPOSITE_TYPE_G      _LS(IDS_CMD_CSCS_General)

// *^^* 여기 추가
// 인자 1: 단면 Shape (단면 DB에 정의되어 있는 Shape)
// 인자 2: 해당 단면 Shape 선택시 Dialog에 표시될 Bitmap
// 인자 3: Composite Type 필요여부 - TRUE인 경우 '1+2, 3', '1, 2+3' 타입으로 구분되므로
//                                   m_aHalfInfo를 추가해 주어야 한다.
SHAPE_BIT_PART CCMStageCompSectItemDlg::m_aShapeInfo[] = {
		{ D_SECT_SHAPE_SRC_RBO       , _T("Stag_Sect_Src1.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_RBC       , _T("Stag_Sect_Src2.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_RPO       , _T("Stag_Sect_Src3.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_RPC       , _T("Stag_Sect_Src4.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_CBO       , _T("Stag_Sect_Src5.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_CBC       , _T("Stag_Sect_Src6.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_CPO       , _T("Stag_Sect_Src7.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_CPC       , _T("Stag_Sect_Src8.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_EBC       , _T("Stag_Sect_Src9.svg") , FALSE },
		{ D_SECT_SHAPE_SRC_EPC       , _T("Stag_Sect_Src10.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_RIB       , _T("Stag_Sect_Src11.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_CIB       , _T("Stag_Sect_Src12.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_RH2T      , _T("Stag_Sect_Src13.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_RHT       , _T("Stag_Sect_Src14.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_BSTF      , _T("Stag_Sect_Src15.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_PSTF      , _T("Stag_Sect_Src16.svg"), FALSE },
		{ D_SECT_SHAPE_SRC_STUB      , _T("Stag_Sect_Src18.svg"), FALSE }, // SRC-Single-Tube
		{ D_SECT_SHAPE_SRC_DUMB      , _T("Stag_Sect_Src19.svg"), FALSE }, // 덤벨 단면
		{ D_SECT_SHAPE_COMPO_B       , _T("Stag_Sect_Comp1.svg"), FALSE },    
		{ D_SECT_SHAPE_COMPO_I       , _T("Stag_Sect_Comp2.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_TUB     , _T("Stag_Sect_Comp5.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_STLG_B  , _T("Stag_Sect_Comp1.svg"), FALSE },    
		{ D_SECT_SHAPE_COMPO_STLG_I  , _T("Stag_Sect_Comp2.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_STLG_TUB, _T("Stag_Sect_Comp5.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_CI      , _T("Stag_Sect_Comp3.svg"), FALSE },    
		{ D_SECT_SHAPE_COMPO_CT      , _T("Stag_Sect_Comp4.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_PC      , _T("Stag_Sect_Comp3.svg"), FALSE },
		{ D_SECT_SHAPE_COMPO_G       , _T("Stag_Sect_Comp3.svg"), FALSE },
		{ D_SECT_SHAPE_PSC_1CELL     , _T("Stag_Sect_25.svg"), TRUE  },  
		{ D_SECT_SHAPE_PSC_2CELL     , _T("Stag_Sect_26.svg"), TRUE  }, 
		{ D_SECT_SHAPE_PSC_MID       , _T("Stag_Sect_04.svg"), TRUE  },
		{ D_SECT_SHAPE_PSC_HALF      , _T("Stag_Sect_Nothing.svg"), TRUE  },    
};

SHAPE_BIT_PART CCMStageCompSectItemDlg::m_aHalfInfo[] = {
	{_T(""), _T("Stag_Sect_Psc3.svg"), FALSE }, {_T(""), _T("Stag_Sect_Psc23.svg"), FALSE },
	{_T(""), _T("Stag_Sect_Psc1.svg"), TRUE  }, {_T(""), _T("Stag_Sect_Psc21.svg"), TRUE  },
};

CCMStageCompSectItemDlg::CCMStageCompSectItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageCompSectItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageCompSectItemDlg)
	//}}AFX_DATA_INIT
	m_aPartGroup.Add(IDC_CMD_PART_NUM_STATIC);
	m_aPartGroup.Add(IDC_CMD_PART_NUM_EDIT);
	//m_aPartGroup.Add(IDC_CMD_PART_NUM_SPIN);

	m_Data.Initialize();

	m_bModify = FALSE;
	m_bPropInit = FALSE;
	m_arTargetElemList.RemoveAll();

	// 수정 - 모든 단면 표시 
	// 시공단계 합성단면이 가능한 Section만 표시 
	//m_cboSection.SetCompositeSectionForCS();

	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_wndGrid = new CCMStageCompSectGrid(this);

	m_bCompNormalPar2 = FALSE;

	m_bVirtual = FALSE;
}

CCMStageCompSectItemDlg::~CCMStageCompSectItemDlg()
{
	if(m_wndGrid) delete m_wndGrid;
}

void CCMStageCompSectItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageCompSectItemDlg)
	DDX_Control(pDX, IDC_CMD_COMPOSITE_TYPE_COMBO, m_cboCompositeType);
	DDX_Control(pDX, IDC_CMD_ACTIVE_STAGE_COMBO, m_cboActiveStage);
	DDX_Control(pDX, IDC_CMD_SECT_ID_EDIT, m_editSection);
	DDX_Control(pDX, IDC_CMD_SECT_ID_COMBO, m_cboSection);
	DDX_Control(pDX, IDC_CMD_SECT_TYPE_STATIC, m_ctrlSectType);
	DDX_Control(pDX, IDC_CMD_SECT_SHAPE_STATIC, m_ctrlSectShape);
	DDX_Control(pDX, IDC_CMD_ELEM_LIST_EDIT, m_editElementList);
	DDX_Control(pDX, IDC_CMD_PART_NUM_EDIT, m_edtPartNum);
	DDX_Control(pDX, IDC_CMD_CSCS_GRID, *m_wndGrid);
	DDX_Control(pDX, IDC_CMD_SECT_FRM, m_wndPicture);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// 
// User Defined Function
//

// Modify의 경우, 외부에서 미리 보여줄 Data를 Setting하는 함수
void CCMStageCompSectItemDlg::SetParam(T_CSCS_K key, T_CSCS_D& rData)
{
	m_Key = key;
	m_Data = rData;

	m_bModify = TRUE;
}

void CCMStageCompSectItemDlg::SetVirtualType(BOOL bVirtual/* = FALSE*/)
{
	m_bVirtual = bVirtual;
}

BOOL CCMStageCompSectItemDlg::IsModifyMode()
{
	return m_bModify;
}

BOOL CCMStageCompSectItemDlg::CalAndGetStiff(int nIndex, T_SECT_STIFFNESS& rStiff_I, T_SECT_STIFFNESS& rStiff_J, T_SECT_STIFFNESS& rStiffAuto_I, T_SECT_STIFFNESS& rStiffAuto_J)
{
	// 여기 계산 루틴이 변경되면 CSectUtil::CalcPartStiff4CompSect 함수도 변경되야 함 
	T_CSCS_D CscsData; CscsData.Initialize();
	T_SECT_D SectData; SectData.Initialize();

	m_bCompNormalPar2 = FALSE;

	if(!SetDlg2Data(CscsData)) return FALSE;

	if (!m_pDoc->m_pAttrCtrl->GetSectOrSecv(CscsData.SectKey, SectData, m_bVirtual, TRUE, TRUE))
		return FALSE;

	int nCount = CscsData.aPart.GetSize();
	CArray<T_STAG_K, T_STAG_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetStagKeyList(aKeyList);

	if(CscsData.nCompType == 0 || CscsData.nCompType == 1) // 1+2 & 3, 1 & 2+3
	{
		if(nCount != 3) { ASSERT(0); return FALSE; }
		if(aKeyList.GetSize() < 2) return FALSE;

		if(nIndex == 0)
		{
			CscsData.aPart[0].CompStagKey = aKeyList[0];      
			CscsData.aPart[2].CompStagKey = aKeyList[1];
			if(CscsData.nCompType == 0)
				CscsData.aPart[1].CompStagKey = aKeyList[0];
			else if(CscsData.nCompType == 1)
				CscsData.aPart[1].CompStagKey = aKeyList[1];
			else ASSERT(0);
		}
		else if(nIndex == 1)
		{
			CscsData.aPart[0].CompStagKey = aKeyList[1];
			CscsData.aPart[2].CompStagKey = aKeyList[0];
			if(CscsData.nCompType == 0)
				CscsData.aPart[1].CompStagKey = aKeyList[1];
			else if(CscsData.nCompType == 1)
				CscsData.aPart[1].CompStagKey = aKeyList[0];
			else ASSERT(0);
		}
	}
	else if(CscsData.nCompType == -2 || CscsData.nCompType == -1 || CscsData.nCompType == 2) // Normal , User // CP_G
	{  
		if (m_pDoc->m_pSectDB->IsSectDumbAndStub(SectData))
		{
			if (aKeyList.GetSize() < 1) return FALSE;
		}
		else
		{
			if (aKeyList.GetSize() < CscsData.aPart.GetSize()) return FALSE;

			unsigned int k = 1;
			for (int i = 0; i < nCount; i++)
			{
				if (nIndex == i)
					CscsData.aPart[i].CompStagKey = aKeyList[0];
				else
					CscsData.aPart[i].CompStagKey = aKeyList[k++];
			}
		}
	}

	CArray<int, int&> aIndex;
	CArray<T_SECT_D, T_SECT_D&> aResult;
	CArray<T_SECT_D, T_SECT_D&> aCsResult; //2004.05.19 Add Ahn,Jaeoh

	if(CscsData.nCompType != 2)
	{	
		BOOL bCompositeNormal = (CscsData.nCompType == -2 /*Nomal*/ && nIndex == 1/*part2*/ &&
								m_pDoc->m_pSectDB->IsSectComposite(SectData) || m_pDoc->m_pSectDB->IsSectPSComposite(SectData)) ? TRUE : FALSE;

		if(bCompositeNormal)
		{
			m_bCompNormalPar2 = TRUE;

			CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> *paStiffAuto = new CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>[2];
			paStiffAuto[0].RemoveAll(); // part1 -> I, J (2개)
			paStiffAuto[1].RemoveAll(); // part2 -> I, J (2개)

			T_SECT_STIFFNESS SaleUser; SaleUser.Initialize(1.0);
			paStiffAuto[0].Add(SaleUser); paStiffAuto[0].Add(SaleUser);
			paStiffAuto[1].Add(SaleUser); paStiffAuto[1].Add(SaleUser);

			if (!m_pDoc->m_pSectDB->CalcSectCscs(SectData, CscsData, aResult, aIndex, aCsResult, NULL, FALSE, paStiffAuto))
			{
				delete[]paStiffAuto;
				return FALSE;
			}

			rStiff_I = aResult[0].SectBefore.SectI.Stiffness;
			rStiff_J = aResult[0].SectBefore.SectJ.Stiffness;

			rStiffAuto_I.Rxx = paStiffAuto[nIndex][0].Rxx;
			rStiffAuto_J.Rxx = paStiffAuto[nIndex][1].Rxx;

			delete[]paStiffAuto;
		}
		else
		{
			rStiffAuto_I.Initialize(0.0);
			rStiffAuto_J.Initialize(0.0);

			if(!m_pDoc->m_pSectDB->CalcSectCscs(SectData, CscsData, aResult, aIndex, aCsResult)) 
				return FALSE;

			rStiff_I = aResult[0].SectBefore.SectI.Stiffness;
			rStiff_J = aResult[0].SectBefore.SectJ.Stiffness;

			rStiffAuto_I.Rxx = rStiff_I.Rxx;
			rStiffAuto_J.Rxx = rStiff_J.Rxx;
		}
	}
	else // Composite General
	{
		// MNET:XXXX-CJJEONG-20090324 (Tel : 2124) 
		// Tapered 에 Composite General 단면을 위해 추가..I단,J단이 다르다.
		if (SectData.nStype == D_SECT_TYPE_TAPERED)
		{
			if(nIndex > SectData.SectBefore.SectI.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}
			if(nIndex > SectData.SectBefore.SectJ.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}

			rStiff_I = SectData.SectBefore.SectI.aGeneralPart[nIndex].Stiffness;
			rStiff_J = SectData.SectBefore.SectJ.aGeneralPart[nIndex].Stiffness;
		}
		else
		{
			if(nIndex > SectData.SectBefore.SectI.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}
			rStiff_I = rStiff_J = SectData.SectBefore.SectI.aGeneralPart[nIndex].Stiffness;
		}    
	}

	return TRUE;
}

// 합성강성을 받아오는 함수 
BOOL CCMStageCompSectItemDlg::CalAndGetCompStiff(int nIndex, T_SECT_STIFFNESS& rStiff_I, T_SECT_STIFFNESS& rStiff_J, CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> *paMultiStiff, BOOL &bHasMulti, UINT &ErrorMsg)
{
	T_CSCS_D CscsData; CscsData.Initialize();
	T_SECT_D SectData; SectData.Initialize();

	if (!SetDlg2Data(CscsData)) return FALSE;

	if (!m_pDoc->m_pAttrCtrl->GetSectOrSecv(CscsData.SectKey, SectData, m_bVirtual, TRUE, TRUE))
		return FALSE;

	bHasMulti = SectData.SectBefore.bMultipleElastic; // 결과로 넘겨줄 BOOL 값

	T_ELEM_K ElemK;
	T_ELEM_D ElemD;
	T_MATL_K ElemMatlK = 0;
	BOOL bCanCalcStiff = TRUE;

	int nNumElem = m_arTargetElemList.GetSize();
	if (nNumElem <= 0) 
		{ bCanCalcStiff = FALSE; ErrorMsg = IDC_CMD_CSCS__Err___No_element_assigned;}
	for (int i=0; i<nNumElem; i++)
	{
		ElemK = m_arTargetElemList.GetAt(i);
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) 
			{ ASSERT(0); bCanCalcStiff = FALSE; continue; }
		if (!m_pDoc->m_pAttrCtrl->ExistMatl(ElemD.elmat)) 
			{ bCanCalcStiff = FALSE; ErrorMsg = IDC_CMD_CSCS__Err___Material_not_exist; break; }
		
		if (i == 0) ElemMatlK = ElemD.elmat;
		else 
		{
			if (ElemMatlK != ElemD.elmat) 
				{ bCanCalcStiff = FALSE; ErrorMsg = IDC_CMD_CSCS__Err___Material_not_same; break; }
		}
	}

	int nNumPart = CscsData.aPart.GetSize();
	for (int i=0; i< nNumPart; i++)
	{
		if (CscsData.aPart[i].MatlKey == 0)   // if material type is 'element'
		{
			if (!bCanCalcStiff) return FALSE;
			CscsData.aPart[i].MatlKey = ElemMatlK;
		}
	}

	CArray<int, int&> aIndex;
	CArray<T_SECT_D, T_SECT_D&> aResult;
	CArray<T_SECT_D, T_SECT_D&> aCsResult;
// MQC-9204. 2015-01-27. by hsshim.
// 아래 분기를 2008-03-20에 mylee가 General Section인 경우 분기를 시켰는데,
//             2008-05-27에 mylee가 CSectDB::CalcSectCscs(...) 함수에서 General Section을 받을 수 있도록 수정했음.
// 결과적으로,
// CSectDB::CalcSectCscs(...) 함수에서 처리 가능한 루틴이고, 
// 해석시에는 CSectDB::CalcSectCscs(...) 함수로 계산된 Stiffness가 시공단계에서 사용되기 때문에
// 대화창에서도 해석시에 사용되는 강성이 보일 수 있도록 수정함. => Stiffness scale factor가 적용되도록 함.
//   if(CscsData.nCompType != 2)
//   {
//     if (!m_pDoc->m_pSectDB->CalcSectCscs(SectData, CscsData, aResult, aIndex, aCsResult, paMultiStiff)) 
//       return FALSE;
// 
// 	  //ASSERT(aCsResult.GetSize() == nNumPart);
//     rStiff_I = aCsResult[nIndex].SectBefore.SectI.Stiffness;
//     rStiff_J = aCsResult[nIndex].SectBefore.SectJ.Stiffness;
//   }
//   else // Composite General
//   {
//     if (SectData.nStype == D_SECT_TYPE_TAPERED)
//     {
//       if(nIndex > SectData.SectBefore.SectI.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}
//       if(nIndex > SectData.SectBefore.SectJ.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}
// 
//       rStiff_I = SectData.SectBefore.SectI.aGeneralPart[nIndex].StiffnessSum;
//       rStiff_J = SectData.SectBefore.SectJ.aGeneralPart[nIndex].StiffnessSum;
//     }
//     else
//     {
//       if(nIndex > SectData.SectBefore.SectI.aGeneralPart.GetSize()-1) {nIndex=0; ASSERT(0);}
//       rStiff_I = rStiff_J = SectData.SectBefore.SectI.aGeneralPart[nIndex].StiffnessSum;
//     }
//   }
	if (!m_pDoc->m_pSectDB->CalcSectCscs(SectData, CscsData, aResult, aIndex, aCsResult, paMultiStiff)) 
	{
		if(CscsData.nCompType != 2) 
		{
			return FALSE;
		}
		else // Composite General
		{
			ASSERT(0);  // MQC-9204 수정하면서, 여기 걸리면 안될 듯...
			return FALSE;
		}
	}

	//ASSERT(aCsResult.GetSize() == nNumPart);
	rStiff_I = aCsResult[nIndex].SectBefore.SectI.Stiffness;
	rStiff_J = aCsResult[nIndex].SectBefore.SectJ.Stiffness;

	return TRUE;
}

int CCMStageCompSectItemDlg::GetPartNumber()
{
	if (m_edtPartNum.GetSafeHwnd() == NULL)
		return 0;

	return m_edtPartNum.GetEditValueInt();
}

// 만약 선택된 Section이 유효하지 않으면 디폴트 값 Setting
void CCMStageCompSectItemDlg::SetDefaultValue()
{
	SetNullBitmap();
	m_wndGrid->SetRowNumber(0);
	m_cboCompositeType.ResetContent();
	m_ctrlSectType.SetWindowText(_T(""));
	m_ctrlSectShape.SetWindowText(_T(""));
	m_editElementList.SetWindowText(_T(""));

	CDlgUtil::CtrlShowHide(this, m_aPartGroup, FALSE);
}

// Active Stage ComboBox 초기화 
void CCMStageCompSectItemDlg::InitActiveStageCombo()
{
	m_cboActiveStage.ResetContent();

	CArray<CString, CString&> arStageNameList;
	m_pDoc->m_pAttrCtrl->GetStagList(arStageNameList);
	int nCount = arStageNameList.GetSize();
	if(nCount <=0) return;
 
	for(int i=0; i<nCount; i++) 
	{
		m_cboActiveStage.AddString(arStageNameList[i]);
	}
	m_cboActiveStage.SetCurSel(0);

	CString strStage = _T("");
	m_cboActiveStage.GetLBText(0, strStage);
}

// *^^* 여기 추가
// Section Type을 String으로 변화해주는 함수
CString CCMStageCompSectItemDlg::ConvStypeToString(int nStype)
{
	CString str = _T("");

	switch(nStype)  
	{
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
	case D_SECT_TYPE_COMPO_PC:
		str = _LS(IDS_CMD_CSCS__Composite); 
		break;
	case D_SECT_TYPE_COMPO_G:
		str = _LS(IDS_CMD_CSCS_General); 
		break;
	case D_SECT_TYPE_SRC:       str = _LS(IDS_CMD_CSCS__SRC);       break;
	case D_SECT_TYPE_PSC:       str = _LS(IDS_CMD_CSCS__PSC);       break;
	case D_SECT_TYPE_TAPERED:   str = _LS(IDS_CMD_CSCS__Tapered);   break;
	case D_SECT_TYPE_REGULAR:   str = _LS(IDS_CMD_CSCS__Regular);   break;
	case D_SECT_TYPE_USER:      str = _LS(IDS_CMD_CSCS__User);      break;
	case D_SECT_TYPE_COMBINED:  str = _LS(IDS_CMD_CSCS__Combined);  break;
	case D_SECT_TYPE_CONSTRUCTION: str = _LS(IDS_CMD_CSCS__Construction); break;
	case D_SECT_TYPE_COLDFORMED: str = _LS(IDS_CMD_CSCS__ColdFormed); break;
	case D_SECT_TYPE_HIBEAM:     str = _LS(IDS_CMD_CSCS__Hi_Beam);  break;
	default: break;
	}
	return str;
}

// Active Stage에서 생성되는 Element 중, 
// 선택된 Section과 같은 Element들을 표시 
void CCMStageCompSectItemDlg::ChangeElememtList(T_STAG_K nStagKey, T_SECT_K nSectKey)
{
	if (m_bVirtual)
		return;

	m_editElementList.SetWindowText(_T(""));
	m_arTargetElemList.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arNewActiveElem;
	CNewlyActivatedElemInStag ElemInStage;
	ElemInStage.Initialize(m_pDoc->m_pAttrCtrl);
 if(!ElemInStage.GetNewlyActivatedElemList(BEAM_EL, nStagKey, arNewActiveElem))
	 return;

	// 그 중에서 Section Type이 일치하는 Element 찾음
	int nCount = arNewActiveElem.GetSize();
	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData;
		if(m_pDoc->m_pAttrCtrl->GetElem(arNewActiveElem[i], ElemData))
		{
			// 단면이 필요한 요소종류 중에서
			if(m_pDoc->m_pAttrCtrl->IsNeedSect(ElemData.eltyp))
			{
				// 단면 키가 같은 요소들만 
				if(ElemData.elpro == nSectKey)
					m_arTargetElemList.Add(arNewActiveElem[i]);
			}
		}
	}

	// Edit에 표시
	CString strElemList = _T(""); 
	CNumericOptimizer optimizer;
	strElemList = optimizer.Optimize((long*)m_arTargetElemList.GetData(), m_arTargetElemList.GetSize());
	m_editElementList.SetWindowText(strElemList);

	// View Select
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, m_arTargetElemList, TRUE, TRUE);
}

// *^^* 여기 추가
// 선택된 Section에 따라 Bitmap을 다르게 보여줌 
BOOL CCMStageCompSectItemDlg::ChangeBitmap(T_SECT_D& data)
{
	CString strShape = data.SectBefore.Shape;
	if(data.nStype == D_SECT_TYPE_COMPO_B) strShape = D_SECT_SHAPE_COMPO_B;
	else if(data.nStype == D_SECT_TYPE_COMPO_I)   strShape = D_SECT_SHAPE_COMPO_I;
	else if(data.nStype == D_SECT_TYPE_COMPO_TUB) strShape = D_SECT_SHAPE_COMPO_TUB;
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_B) strShape = D_SECT_SHAPE_COMPO_STLG_B;
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_I) strShape = D_SECT_SHAPE_COMPO_STLG_I;
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_TUB) strShape = D_SECT_SHAPE_COMPO_STLG_TUB;
	else if(data.nStype == D_SECT_TYPE_COMPO_CI)  strShape = D_SECT_SHAPE_COMPO_CI;
	else if(data.nStype == D_SECT_TYPE_COMPO_CT)  strShape = D_SECT_SHAPE_COMPO_CT;
	else if(data.nStype == D_SECT_TYPE_COMPO_PC)  strShape = D_SECT_SHAPE_COMPO_PC;
	else if(data.nStype == D_SECT_TYPE_COMPO_G)   strShape = D_SECT_SHAPE_COMPO_G;
	// MNET:XXXX-CJJEONG-20090317 (Tel : 2124) Taperd Composite General 추가
	else if(data.nStype == D_SECT_TYPE_TAPERED && data.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
		strShape = D_SECT_SHAPE_COMPO_G;

	int nBitmapIndex;
	int nCount = sizeof(m_aShapeInfo)/sizeof(m_aShapeInfo[0]);
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (strShape == m_aShapeInfo[i].strShape){ nBitmapIndex = i; break; }
	if(i == nCount) { SetNullBitmap(1); return TRUE; }

	// 수정 *^^*
	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	if(strShape == D_SECT_SHAPE_PSC_HALF)
	{
		CString aHalfBitmap[] = {
			_T("Stag_Sect_Psc3.svg"), _T("Stag_Sect_Psc24.svg"), _T("Stag_Sect_Psc1.svg"),
			_T("Stag_Sect_Psc23.svg"), _T("Stag_Sect_Psc22.svg"), _T("Stag_Sect_Psc21.svg"),
		};
		UINT nIndex = data.SectBefore.nCellType*3 +data.SectBefore.nCellShape;
		if(nIndex >=0 && nIndex<(sizeof(aHalfBitmap)/sizeof(UINT)))
			m_wndPicture.SetImage(strSVGPath + aHalfBitmap[nIndex]);
		else 
			ASSERT(0);
	}
	else if(strShape == D_SECT_SHAPE_PSC_MID)
	{
		CString aMidBitmap[] = {
			_T("Stag_Sect_Psc5.svg"), _T("Stag_Sect_Psc10.svg"), _T("Stag_Sect_Psc6.svg"),
			_T("Stag_Sect_Psc11.svg"), _T("Stag_Sect_Psc9.svg"), _T("Stag_Sect_Psc13.svg"),
			_T("Stag_Sect_Psc7.svg"), _T("Stag_Sect_Psc12.svg"), _T("Stag_Sect_Psc4.svg"),
		};

		int nIndex = data.SectBefore.nCellType*3 + data.SectBefore.nCellShape;
		if (nIndex >= 0 && nIndex < (sizeof(aMidBitmap) / sizeof(UINT)))
			m_wndPicture.SetImage(strSVGPath + aMidBitmap[nIndex]);
		else
			ASSERT(0);
	}
	else 
	{
		if(m_aShapeInfo[i].uBitmapID == 0) return FALSE;
		m_wndPicture.SetImage(strSVGPath + m_aShapeInfo[nBitmapIndex].uBitmapID);
	} 
 
	return TRUE;
}

// 단면 선택이 이루어지지 않았을 때, 텅빈 비트맵 설정 
void CCMStageCompSectItemDlg::SetNullBitmap(int nOption/*=0*/)
{
	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	if     (nOption == 1) m_wndPicture.SetImage(strSVGPath + _T("stag_cscy_User.svg"));    // IDB_STAG_SECT_CXCY_GUIDE
	else if(nOption == 2) m_wndPicture.SetImage(strSVGPath + _T("stag_cscy_Gen.svg"));
	else                  m_wndPicture.SetImage(strSVGPath + _T("Stag_Sect_Nothing.svg"));
}


// *^^* 여기 추가
// 그리드 로우 개수 초기화 - User Type/General Section 이 아닐때 
void CCMStageCompSectItemDlg::SetGridRowNumber(T_SECT_K nKey)
{
	T_SECT_D data;
	m_pDoc->m_pAttrCtrl->GetSectOrSecv(nKey, data, m_bVirtual, TRUE, TRUE);

	// Composite Section
	if(data.nStype == D_SECT_TYPE_COMPO_I      || data.nStype == D_SECT_TYPE_COMPO_B        || 
		 data.nStype == D_SECT_TYPE_COMPO_TUB    || data.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
		 data.nStype == D_SECT_TYPE_COMPO_STLG_I || data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		 data.nStype == D_SECT_TYPE_COMPO_CI     || data.nStype == D_SECT_TYPE_COMPO_CT       || 
		 data.nStype == D_SECT_TYPE_COMPO_PC)
	{
		m_wndGrid->SetRowNumber(2);
	}
	else if(data.nStype==D_SECT_TYPE_TAPERED && (
					data.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || data.SectBefore.nStype == D_SECT_TYPE_COMPO_B        || 
					data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
					data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
					data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI     || data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT))
	{
		m_wndGrid->SetRowNumber(2, -1, 0, TRUE/*Tapered*/);
	}
	// SRC Seciton
	else if(data.nStype == D_SECT_TYPE_SRC) 
	{
		int nPart = CSectUtil::GetPartNumberFromShapeSRC(data.SectBefore.Shape);

		int nSectType = (m_pDoc->m_pSectDB->IsSectDumbAndStub(data))? 2 : 1;
		m_wndGrid->SetRowNumber(nPart, -1, nSectType);
	}
	// PSC Section
	else if(data.nStype == D_SECT_TYPE_PSC)
	{
		int nPart = CSectUtil::GetPartNumberFromInfoPSC(data.SectBefore.Shape, data.SectBefore.nCellType, data.SectBefore.nCellShape);
		ASSERT(nPart > 1);

		int nSubType = -1;
		if(IsNeedCompositeType(data)) nSubType = GetCompositeType();
		m_wndGrid->SetRowNumber(nPart, nSubType);
	}
	else if(data.nStype == D_SECT_TYPE_TAPERED)
	{
		if(data.SectBefore.nStype != D_SECT_TYPE_PSC) return;

		int nPart = CSectUtil::GetPartNumberFromInfoPSC(data.SectBefore.Shape, data.SectBefore.nCellType, data.SectBefore.nCellShape);
		ASSERT(nPart > 1);

		int nSubType = -1;
		if(IsNeedCompositeType(data)) nSubType = GetCompositeType();
		//m_wndGrid->SetRowNumber(nPart, nSubType); // MQC:1485 20071128 mylee
		m_wndGrid->SetRowNumber(nPart, nSubType, 0/*SRC*/, TRUE/*Tapered*/);
	}
}

// Section과 Composite Type이 바뀌었을 때, 그리드를 새롭게 초기화
void CCMStageCompSectItemDlg::SetInitGrid(T_SECT_K nKey)
{
	CString csCompoType = _T("");
	int nIndex = m_cboCompositeType.GetCurSel();
	m_cboCompositeType.GetLBText(nIndex, csCompoType);

	T_SECT_D data;
	m_pDoc->m_pAttrCtrl->GetSectOrSecv(nKey, data, m_bVirtual, TRUE, TRUE);
	
	int nSectType = 0;
	if (data.nStype == D_SECT_TYPE_SRC)
	{
		nSectType = 1;
		if(m_pDoc->m_pSectDB->IsSectDumbAndStub(data)) nSectType = 2;
	}

	// Composite Type이 User일 때 - 갯수를 받는다.
	if(csCompoType == COMPOSITE_TYPE_U)
	{
		UpdateData(TRUE);
		BOOL bTapered = (data.nStype == D_SECT_TYPE_TAPERED);
		m_wndGrid->SetRowNumber(m_edtPartNum.GetEditValueInt(), 2/*User Type*/, nSectType, bTapered);
		SetNullBitmap(1);
	}
	else if(csCompoType == COMPOSITE_TYPE_G)
	{
		UpdateData(TRUE);
		m_edtPartNum.SetValue((int)data.SectBefore.SectI.aGeneralPart.GetSize());
		UpdateData(FALSE);

		BOOL bTapered = (data.nStype == D_SECT_TYPE_TAPERED);
		m_wndGrid->SetRowNumber(m_edtPartNum.GetEditValueInt(), -1, nSectType, bTapered);
		SetNullBitmap(2);
		m_wndGrid->SetInitCentroidForGeneral(data);
		m_wndGrid->SetInitScaleFactorForGeneral(data);
		m_wndGrid->SetInitStiffnessForGeneral(data);
	}
	else
	{
		SetGridRowNumber(nKey);
		ChangeBitmap(data);
	}
	
	if(csCompoType != COMPOSITE_TYPE_U)
		m_wndGrid->SetInitH(); // MNET:2840 20070703 mylee - H 의 초기값을 계산하여 세팅
}

BOOL CCMStageCompSectItemDlg::GetAutoCalcH(int nSize, CArray<double, double>& aH) // nIndex=0, 1 ...
{
	int nIndex = 0; // 1 번 Row 에 대해서 계산하는 것처럼
	if(nSize <= 0) return FALSE;
	if(nSize==1) nSize=2;
	aH.RemoveAll();
	aH.SetSize(nSize);

	T_CSCS_D CscsData; CscsData.Initialize();
	T_SECT_D SectData; SectData.Initialize();

	if(!SetDlg2Data(CscsData)) return FALSE;

	if(!Get_H_FromCscs(nSize, CscsData, aH)) return FALSE;

	return TRUE;
}

BOOL CCMStageCompSectItemDlg::Get_H_FromCscs(int nSize, T_CSCS_D CscsData, CArray<double, double>& aH)
{
	return m_pDoc->m_pAttrCtrl->Get_H_FromCscs(nSize, CscsData, aH);

// 	int nIndex = 0; // 1 번 Row 에 대해서 계산하는 것처럼
// 	if(nSize <= 0) return FALSE;
// 	if(nSize==1) nSize=2;
// 	aH.RemoveAll();
// 	aH.SetSize(nSize);
// 
// 	T_SECT_D SectData; SectData.Initialize();
// 
// 	int nCount = CscsData.aPart.GetSize();
// 	CArray<T_STAG_K, T_STAG_K> aKeyList;
// 	m_pDoc->m_pAttrCtrl->GetStagKeyList(aKeyList);
// 
// 	if(CscsData.nCompType == -1) return FALSE; // User 인 경우 return FALSE 하여 무조건 0 세팅
// 
// 	if(CscsData.nCompType == 0 || CscsData.nCompType == 1) // 1+2 & 3, 1 & 2+3
// 	{
// 		if(nCount != 3) { ASSERT(0); return FALSE; }
// 		if(aKeyList.GetSize() < 2) return FALSE;
// 
// 		if(nIndex == 0)
// 		{
// 			CscsData.aPart[0].CompStagKey = aKeyList[0];      
// 			CscsData.aPart[2].CompStagKey = aKeyList[1];
// 			if(CscsData.nCompType == 0)
// 				CscsData.aPart[1].CompStagKey = aKeyList[0];
// 			else if(CscsData.nCompType == 1)
// 				CscsData.aPart[1].CompStagKey = aKeyList[1];
// 			else ASSERT(0);
// 		}
// 		else if(nIndex == 1)
// 		{
// 			CscsData.aPart[0].CompStagKey = aKeyList[1];
// 			CscsData.aPart[2].CompStagKey = aKeyList[0];
// 			if(CscsData.nCompType == 0)
// 				CscsData.aPart[1].CompStagKey = aKeyList[1];
// 			else if(CscsData.nCompType == 1)
// 				CscsData.aPart[1].CompStagKey = aKeyList[0];
// 			else ASSERT(0);
// 		}
// 	}
// 	else if(CscsData.nCompType == -2/* || CscsData.nCompType == -1*/) // Normal, not user
// 	{  
// 		if(aKeyList.GetSize() < CscsData.aPart.GetSize()) return FALSE;
// 
// 		unsigned int k = 1;
// 		for(int i=0; i<nCount; i++)
// 		{
// 			if(nIndex == i) 
// 				CscsData.aPart[i].CompStagKey = aKeyList[0];
// 			else 
// 				CscsData.aPart[i].CompStagKey = aKeyList[k++];
// 		}
// 	}
// 
// 	if(!m_pDoc->m_pAttrCtrl->GetSect(CscsData.SectKey, SectData)) 
// 		return FALSE;
// 
// 	CArray<int, int&> aIndex;
// 	CArray<T_SECT_D, T_SECT_D&> aResult;
// 	CArray<T_SECT_D, T_SECT_D&> aCsResult; //2004.05.19 Add Ahn,Jaeoh
// 	if(CscsData.aPart.GetSize() <= 0) return FALSE;
// 
// 	// MNET:3964-CJJEONG-20090320 (Tel : 2124) 
// 	// Tapered 에 Composite General 을 추가하기 위해 처리
// 	if(SectData.nStype == D_SECT_TYPE_COMPO_G || (SectData.nStype==D_SECT_TYPE_TAPERED && SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_G))
// 	{ // 계산 안함
// 	}
// 	else
// 	{
// 	if(!m_pDoc->m_pSectDB->CalcSectCscs(SectData, CscsData, aResult, aIndex, aCsResult)) 
// 		return FALSE;
// 	}
// 
// 	// SectData의 종류에 따라 정확한 위치에서 PeriIn, PeriOut 가져와서 계산
// 	// 1) User
// 	if(SectData.nStype==D_SECT_TYPE_USER) { } // do nothing
// 	// 2) Psc
// 	else if(SectData.nStype==D_SECT_TYPE_PSC)
// 	{
// 		if(aResult.GetSize() > 1)
// 		{
// 			double dUOut = aResult[0].SectBefore.SectI.PeriOut;
// 			double dAc = aResult[0].SectBefore.SectI.Stiffness.Area;
// 			if(dUOut !=0) aH[0] = 2 * dAc / dUOut;
// 
// 			dUOut = aResult[1].SectBefore.SectI.PeriOut;
// 			dAc = aResult[1].SectBefore.SectI.Stiffness.Area;
// 			if(dUOut !=0) aH[1] = 2 * dAc / dUOut;
// 		}
// 	}
// 	// 3) Composite
// 	else if(SectData.nStype==D_SECT_TYPE_COMPO_B       || SectData.nStype==D_SECT_TYPE_COMPO_I         ||
// 		      SectData.nStype==D_SECT_TYPE_COMPO_TUB     || SectData.nStype==D_SECT_TYPE_COMPO_STLG_B    ||
// 					SectData.nStype==D_SECT_TYPE_COMPO_STLG_I  ||	SectData.nStype==D_SECT_TYPE_COMPO_STLG_TUB  ||
// 					SectData.nStype==D_SECT_TYPE_COMPO_CI      || SectData.nStype==D_SECT_TYPE_COMPO_CT        || 
// 					SectData.nStype==D_SECT_TYPE_COMPO_PC)
// 	{
// 		if(aResult.GetSize() > 1)
// 		{
// 			// Part 1
// 			double dUOut = aResult[0].SectBefore.SectI.PeriOut;
// 			double dAc = aResult[0].SectBefore.SectI.Stiffness.Area;
// 			if(dUOut !=0) aH[0] = 2 * dAc / dUOut;
// 
// 			// Part 2
// 			double dB = SectData.SectAfter.SectI.Size[0]; // Slab Width
// 			double dTc = SectData.SectAfter.SectJ.Size[1];
// 			dAc = dB * dTc;
// 			dUOut = 2*dB + 2*dTc;
// 			if(dUOut !=0) aH[1] = 2 * dAc / dUOut;
// 		}
// 	}
// 	// 3-1) Composite General Section
// 	else if(SectData.nStype==D_SECT_TYPE_COMPO_G)
// 	{
// 		int nPartNum = SectData.SectBefore.SectI.aGeneralPart.GetSize();
// 		for(int i=0 ; i<nSize ; ++i)
// 		{
// 			if(i>nPartNum-1) break;
// 
// 			double dUOut = SectData.SectBefore.SectI.aGeneralPart[i].PeriOut;
// 			double dAc = SectData.SectBefore.SectI.aGeneralPart[i].Stiffness.Area;
// 			if(dUOut !=0) aH[i] = 2 * dAc / dUOut;
// 		}
// 	}
// 	// 4) Tapered
// 	else if(SectData.nStype==D_SECT_TYPE_TAPERED)
// 	{
// 		if(SectData.SectBefore.nStype==D_SECT_TYPE_PSC)
// 		{
// 			if(aResult.GetSize() > 1)
// 			{
// 				double dUOut = aResult[0].SectBefore.SectI.PeriOut;
// 				double dAc = aResult[0].SectBefore.SectI.Stiffness.Area;
// 				double dUOutJ = aResult[0].SectBefore.SectJ.PeriOut;
// 				double dAcJ = aResult[0].SectBefore.SectJ.Stiffness.Area;
// 				if(dUOut !=0 && dUOutJ !=0) aH[0] = ((2 * dAc / dUOut)+(2 * dAcJ / dUOutJ)) / 2;
// 				
// 				dUOut = aResult[1].SectBefore.SectI.PeriOut;
// 				dAc = aResult[1].SectBefore.SectI.Stiffness.Area;
// 				dUOutJ = aResult[1].SectBefore.SectJ.PeriOut;
// 				dAcJ = aResult[1].SectBefore.SectJ.Stiffness.Area;
// 				if(dUOut !=0 && dUOutJ !=0) aH[1] = ((2 * dAc / dUOut)+(2 * dAcJ / dUOutJ)) / 2;
// 			}
// 		}
// 		else if(SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_B      || SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_I        ||
// 			      SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_TUB    || SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B   ||
// 						SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I || SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB ||
// 						SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_CI     || SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_CT       || 
// 						SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_PC)
// 		{
// 			if(aResult.GetSize() > 1)
// 			{
// 				// Part 1
// 				double dUOut = aResult[0].SectBefore.SectI.PeriOut;
// 				double dAc = aResult[0].SectBefore.SectI.Stiffness.Area;
// 				double dUOutJ = aResult[0].SectBefore.SectJ.PeriOut;
// 				double dAcJ = aResult[0].SectBefore.SectJ.Stiffness.Area;
// 				if(dUOut !=0 && dUOutJ !=0) aH[0] = ((2 * dAc / dUOut)+(2 * dAcJ / dUOutJ)) / 2;
// 				
// 				// Part 2
// 				double dB = SectData.SectAfter.SectI.Size[0]; // Slab Width
// 				double dTc = SectData.SectAfter.SectJ.Size[1];
// 				double dBJ = SectData.CmpTapJ.Size[59]; // Slab Width
// 				double dTcJ = SectData.SectAfter.SectJ.Size[1];
// 				dAc = dB * dTc;
// 				dUOut = 2*dB + 2*dTc;
// 				dAcJ = dBJ * dTcJ;
// 				dUOutJ = 2*dBJ + 2*dTcJ;
// 				if(dUOut !=0 && dUOutJ !=0) aH[1] = ((2 * dAc / dUOut)+(2 * dAcJ / dUOutJ)) / 2;
// 			}
// 		}
// 		// MNET:3964-CJJEONG-20090320 (Tel : 2124) 
// 		// Tapered 에 Composite General 을 추가하기 위해 처리
// 		else if (SectData.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
// 		{
// 			int nPartNum = min(SectData.SectBefore.SectI.aGeneralPart.GetSize(), SectData.SectBefore.SectJ.aGeneralPart.GetSize());
// 			for(int i=0 ; i<nSize ; ++i)
// 			{
// 				if(i>nPartNum-1) break;
// 				
// 				// 계산식은 아래와 같이 이루어 지나 자동계산을 안 하기로 해서 0으로 처리함..
// 				//double dUOut = SectData.SectBefore.SectI.aGeneralPart[i].PeriOut;
// 				//double dAc = SectData.SectBefore.SectI.aGeneralPart[i].Stiffness.Area;
// 				//double dUOutJ = SectData.SectBefore.SectJ.aGeneralPart[i].PeriOut;
// 				//double dAcJ = SectData.SectBefore.SectJ.aGeneralPart[i].Stiffness.Area;
// 				//if(dUOut !=0 && dUOutJ !=0) aH[i] = ((2 * dAc / dUOut)+(2 * dAcJ / dUOutJ)) / 2;
// 				aH[i] = 0.0;
// 			}
// 			// 대화상자의 성공 카운트를 올리지 않기 위해 return FALSE 함
// 			return FALSE;
// 		}
// 	}
// 	else
// 	{
// 		ASSERT(0);
// 	}
// 
// 	// MNET:1603 20080213 mylee - Japan Code 일 때, 2 로 나눠주는 코드
// 	ASSERT(aH.GetSize() >= 2);
// 	if(CscsData.nCompType == -1) // User
// 	{
// 		ASSERT(0); // 위에서 걸러짐
// 		return FALSE;
// 	}
// 	else if(CscsData.nCompType == 0 || CscsData.nCompType == 1) // 1+2 & 3, 1 & 2+3
// 	{
// 		if( IsJapanCode(CscsData.aPart[0], CscsData) ) aH[0] /= 2.0;
// 		if( IsJapanCode(CscsData.aPart[2], CscsData) ) aH[1] /= 2.0;
// 	}
// 	else if(CscsData.nCompType == -2) // Normal
// 	{
// 		if( IsJapanCode(CscsData.aPart[0], CscsData) ) aH[0] /= 2.0;
// 		if( IsJapanCode(CscsData.aPart[1], CscsData) ) aH[1] /= 2.0;
// 	}
// 
// 	return TRUE;

}

// MNET:1603 20080213 mylee - 해당 Part 가 Japan 기준을 적용해야 하는지 판별하는 함수
// CscsD 에서는 SectKey, ActStagKey 참조
BOOL CCMStageCompSectItemDlg::IsJapanCode(T_CSCS_BASE &Part, const T_CSCS_D& CscsD)
{
	return m_pDoc->m_pAttrCtrl->IsJapanCode4Cscs(Part, CscsD);

// 	if(Part.MatlKey == 0) // Element
// 	{
// 		CArray<T_ELEM_K, T_ELEM_K> arNewActiveElem;
// 		CNewlyActivatedElemInStag ElemInStage;
// 		ElemInStage.Initialize(m_pDoc->m_pAttrCtrl);
// 		if(!ElemInStage.GetNewlyActivatedElemList(BEAM_EL, CscsD.ActStagKey, arNewActiveElem))
// 			return FALSE;
// 		
// 		T_MATL_K MatlK;
// 		CMap<T_MATL_K, T_MATL_K, int, int> mapMatlK;
// 		// 그 중에서 Section Type이 일치하는 Element 찾음
// 		int nCount = arNewActiveElem.GetSize();
// 		for(int i=0; i<nCount; i++)
// 		{
// 			T_ELEM_D ElemData;
// 			if(m_pDoc->m_pAttrCtrl->GetElem(arNewActiveElem[i], ElemData))
// 			{
// 				// 단면이 필요한 요소종류 중에서 단면 키가 같은 요소들만 
// 				if(m_pDoc->m_pAttrCtrl->IsNeedSect(ElemData.eltyp) && ElemData.elpro == CscsD.SectKey)
// 				{
// 					mapMatlK.SetAt(ElemData.elmat, 1);
// 				}
// 			}
// 		}
// 		// 재질들이 모두 Japan 기준이어야 함
// 		int nTmp;
// 		BOOL bTmatExist = FALSE;
// 		BOOL bTdmtExist = FALSE;
// 		BOOL bJapanCode = FALSE;
// 		if(mapMatlK.GetCount() == 0) return FALSE;
// 		POSITION pos = mapMatlK.GetStartPosition();
// 		while(pos)
// 		{
// 			mapMatlK.GetNextAssoc(pos, MatlK, nTmp);
// 			T_MATL_D MatlData;
// 			T_TMAT_D TmatData;
// 			T_TDMT_D TdmtData;
// 			if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlData)) return FALSE;
// 			
// 			bTmatExist = m_pDoc->m_pAttrCtrl->GetTmat(MatlK, TmatData);
// 			if(bTmatExist) bTdmtExist = m_pDoc->m_pAttrCtrl->GetTdmt(TmatData.TdMatlTypeKey, TdmtData);
// 			if(bTdmtExist) bJapanCode = (TdmtData.COMMON.nCode == 8); // Japan Standard인 경우
// 			if(!bTmatExist || !bTdmtExist || !bJapanCode) return FALSE;
// 		}
// 		return TRUE; // 모두 무사통과
// 	}
// 	else
// 	{
// 		T_MATL_K MatlK = Part.MatlKey;
// 		T_MATL_D MatlData;
// 		T_TMAT_D TmatData;
// 		T_TDMT_D TdmtData;
// 		BOOL bTmatExist = FALSE;
// 		BOOL bTdmtExist = FALSE;
// 		BOOL bJapanCode = FALSE;
// 		if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlData)) return FALSE;
// 		
// 		bTmatExist = m_pDoc->m_pAttrCtrl->GetTmat(MatlK, TmatData);
// 		if(bTmatExist) bTdmtExist = m_pDoc->m_pAttrCtrl->GetTdmt(TmatData.TdMatlTypeKey, TdmtData);
// 		if(bTdmtExist) bJapanCode = (TdmtData.COMMON.nCode == 8); // Japan Standard인 경우
// 		if(!bTmatExist || !bTdmtExist || !bJapanCode) return FALSE;
// 		
// 		return TRUE;
// 	}
}

// 
int CCMStageCompSectItemDlg::GetCompositeType()
{
	CString strCompType = _T("");
	int nIndex = m_cboCompositeType.GetCurSel();
	m_cboCompositeType.GetLBText(nIndex, strCompType);
	if(strCompType == COMPOSITE_TYPE_N) return -2;
	else if(strCompType == COMPOSITE_TYPE_1) return 0;
	else if(strCompType == COMPOSITE_TYPE_U) return -1;
	else if(strCompType == COMPOSITE_TYPE_2) return  1;
	else if(strCompType == COMPOSITE_TYPE_G) return  2;

	return -3;
}

// 외부에서 Data가 주어지면 이를 보여줌(Modify의 경우)
void CCMStageCompSectItemDlg::SetData2Dlg()
{
	if(!m_bModify) return;

	T_STAG_D StagData;
	m_pDoc->m_pAttrCtrl->GetStag(m_Data.ActStagKey, StagData);
	m_cboActiveStage.SelectString(-1, StagData.StageName);

	if(m_cboSection.ChangeSelect(m_Data.SectKey))
		m_editSection.SetEditBoxText(m_Data.SectKey);

	// 수정 
	ChangeSection();

	// 수정
	CString csSelCompoType = _T("");

	if(m_Data.nCompType == 0) csSelCompoType = COMPOSITE_TYPE_1;
	else if(m_Data.nCompType == 1) csSelCompoType = COMPOSITE_TYPE_2;
	else if(m_Data.nCompType == -2) csSelCompoType = COMPOSITE_TYPE_N;
	else if(m_Data.nCompType ==  2)
	{
		m_edtPartNum.SetValue((int)m_Data.aPart.GetSize());
		UpdateData(FALSE);
		csSelCompoType = COMPOSITE_TYPE_G;
	}
	else if(m_Data.nCompType == -1)
	{
		m_edtPartNum.SetValue((int)m_Data.aPart.GetSize());
		UpdateData(FALSE);
		csSelCompoType = COMPOSITE_TYPE_U;
	}
	m_cboCompositeType.SelectString(-1, csSelCompoType);

	OnSelchangeCmdCompositeTypeCombo();

	int nCount = m_Data.aPart.GetSize();
	for(int i=0; i<nCount; i++)
		m_wndGrid->SetData(m_Data.aPart[i]);
}

// 입력된 Data를 DB에 저장 
BOOL CCMStageCompSectItemDlg::SetDlg2Data(T_CSCS_D& rData, BOOL bErrorChk/* = FALSE*/)
{
	rData.Initialize();

	CString strStage = _T("");
	int nIndex = m_cboActiveStage.GetCurSel();
	if(nIndex < 0) return FALSE; 
	m_cboActiveStage.GetLBText(nIndex, strStage);
	rData.ActStagKey = m_pDoc->m_pAttrCtrl->GetStagKey(strStage);

	if(!m_editSection.GetKey(rData.SectKey)) return FALSE;

	rData.nCompType = GetCompositeType();
	if(rData.nCompType == -3) return FALSE;

	if(!m_wndGrid->GetAllData(rData.aPart)) return FALSE;

	if (bErrorChk)
	{
		BOOL bChk = TRUE;
		T_SECT_D SectData; SectData.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetSectOrSecv(rData.SectKey, SectData, m_bVirtual, TRUE, TRUE))
		{
			if (SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
			{
				if (rData.aPart[0].CompStagKey <= rData.aPart[1].CompStagKey && rData.aPart[1].CompStagKey <= rData.aPart[2].CompStagKey) {/*OK*/ }
				else bChk = FALSE;
			}	
			else if (SectData.SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
			{
				if (rData.aPart[0].CompStagKey <= rData.aPart[1].CompStagKey) {/*OK*/ }
				else bChk = FALSE;
			}

			if(!bChk)
			{
				GSaveHistoryFormatNF(_T("[Error]The assigned part stage information is incorrect."));
				return FALSE;
			}			
		}
	}

	return TRUE;
}

// Composite Type Combo를 초기화하는 함수
// nKind : (-1) User  (0) Normal, User  (1) 1&2 + 3, 1 + 2&3, User    
void CCMStageCompSectItemDlg::InitCompositeTypeCombo(T_SECT_D& data)
{
	m_cboCompositeType.ResetContent();
	
	int nKind = GetKindOfCompositeType(data);
	if(nKind == 0)
	{
		m_cboCompositeType.AddString(COMPOSITE_TYPE_N);
	}
	else if(nKind == 1)
	{
		m_cboCompositeType.AddString(COMPOSITE_TYPE_1);
		m_cboCompositeType.AddString(COMPOSITE_TYPE_2);
	}
	else if(nKind == 2)
	{
		m_cboCompositeType.AddString(COMPOSITE_TYPE_G);
	}

	m_cboCompositeType.AddString(COMPOSITE_TYPE_U);  
	m_cboCompositeType.SetCurSel(0);

	OnSelchangeCmdCompositeTypeCombo();
}

// Composite Type이 필요한지 아닌지..
BOOL CCMStageCompSectItemDlg::IsNeedCompositeType(T_SECT_D& rData)
{
	// PSC 단면과 Tapered PSC 단면에 대해서만..
	if(rData.nStype != D_SECT_TYPE_PSC && rData.nStype != D_SECT_TYPE_TAPERED) return FALSE;
	if(rData.nStype == D_SECT_TYPE_TAPERED) if(rData.SectBefore.nStype != D_SECT_TYPE_PSC) return FALSE;

	int nCount = sizeof(m_aShapeInfo)/sizeof(SHAPE_BIT_PART);
	int i = 0;
	for(i=0; i<nCount; i++) 
	{
		if(m_aShapeInfo[i].strShape == rData.SectBefore.Shape) break;
	}
	ASSERT(i != nCount);

	if(m_aShapeInfo[i].uBitmapID == 0)
	{
		UINT nIndex = rData.SectBefore.nCellType+rData.SectBefore.nCellShape;
		return m_aHalfInfo[nIndex].bNeedCompType;
	}
	return m_aShapeInfo[i].bNeedCompType;
}

// *^^* 여기 추가
// Composite Type의 종류를 얻어오는 함수 
// (-1) User  (0) Normal, User  (1) 1&2 + 3, 1 + 2&3, User (2) General Section    
int CCMStageCompSectItemDlg::GetKindOfCompositeType(T_SECT_D& rData)
{
	// Composite Type의 경우 Shape이 저장 안될수도 있으므로 Type으로 미리 처리
	if(rData.nStype == D_SECT_TYPE_COMPO_B  ||
		 rData.nStype == D_SECT_TYPE_COMPO_I  ||
		 rData.nStype == D_SECT_TYPE_COMPO_TUB||
		 rData.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		 rData.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		 rData.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		 rData.nStype == D_SECT_TYPE_COMPO_CI ||
		 rData.nStype == D_SECT_TYPE_COMPO_CT ||
		 rData.nStype == D_SECT_TYPE_COMPO_PC) return 0;

	if(rData.nStype == D_SECT_TYPE_COMPO_G) return 2;
	// MNET:XXXX-CJJEONG-20090317 (Tel : 2124) 
	// Taperd 의 Composite General 추가
	if(rData.nStype == D_SECT_TYPE_TAPERED && rData.SectBefore.nStype == D_SECT_TYPE_COMPO_G) return 2;

	// 수정 *^^*
	// PSC_HALF는 특별처리 
	if(rData.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF)
	{
		if(rData.SectBefore.nCellShape == 0) return 0;
		else if(rData.SectBefore.nCellShape == 1) return -1;
		else if(rData.SectBefore.nCellShape == 2) return 1;
		else ASSERT(0);
	}

	// PSC_Mid 역시 특별 처리 
	if(rData.SectBefore.Shape == D_SECT_SHAPE_PSC_MID)
	{
		if(rData.SectBefore.nCellType==2 && rData.SectBefore.nCellShape==2) return 1;
		else return -1;
	}

	// 나머지 Type 단면들.. 
	int nCount = sizeof(m_aShapeInfo) / sizeof(SHAPE_BIT_PART);
	int i = 0;
	for(i=0; i<nCount; i++)
		if(m_aShapeInfo[i].strShape == rData.SectBefore.Shape) break;
	if(i == nCount) return -1; // 그 외 단면인 경우 

	BOOL bNeed = IsNeedCompositeType(rData); 
	
	if(bNeed) return 1;
	else return 0;
}

BEGIN_MESSAGE_MAP(CCMStageCompSectItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageCompSectItemDlg)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_ID_COMBO, OnSelchangeCmdSectIdCombo)
	ON_CBN_SELCHANGE(IDC_CMD_ACTIVE_STAGE_COMBO, OnSelchangeCmdActiveStageCombo)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_EN_CHANGE(IDC_CMD_SECT_ID_EDIT, OnChangeCmdSectIdEdit)
	ON_CBN_SELCHANGE(IDC_CMD_COMPOSITE_TYPE_COMBO, OnSelchangeCmdCompositeTypeCombo)
	ON_EN_CHANGE(IDC_CMD_PART_NUM_EDIT, OnChangeCmdPartNumEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectItemDlg message handlers

//-----------------------------------------------------------------------------
// OnInitDialog   초기화 
//-----------------------------------------------------------------------------
BOOL CCMStageCompSectItemDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	CDialogMove::OnInitDialog();

	m_wndGrid->Initialize(m_pDoc);

	m_editElementList.EnableWindow(FALSE);

	m_edtPartNum.SetRange(0, 1000);
	m_edtPartNum.SetValue(0);

	InitActiveStageCombo();
	// 단면이 하나라도 있을 때만 
	//CArray<UINT, UINT> arSectList;
	//m_pDoc->m_pAttrCtrl->GetSecfKeyList(arSectList);
	//InitCompositeTypeCombo();

	m_cboSection.SetEditBoxPoint(&m_editSection);
	if (m_bVirtual)
	{
		m_cboSection.SetVirtualSect();

		GetDlgItem(IDOK         )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	}
	m_editSection.SetComboBoxPoint(&m_cboSection);
	
	if(m_bModify)
	{
		SetData2Dlg();
	}
	else
	{
		if(m_cboSection.GetCount() != CB_ERR)
		{
			m_cboSection.SetCurSel(0);
			T_SECT_K key;
			if(m_cboSection.GetSelectedSect(key))
				m_editSection.SetEditBoxText(key);
			else m_cboCompositeType.ResetContent();
		}
		ChangeSection();
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// OnOK   
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::OnOK() 
{
	if(!m_wndGrid->ReCalcStiff(1)) return; // MQC 16082

	BOOL bResult;
	if(!SetDlg2Data(m_Data, TRUE))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_CSCS__Err_Input_Data));
		return;
	}

	if(m_bModify) bResult = m_pDoc->m_pDataCtrl->ModifyCscs(m_Key, m_Data);
	else          bResult = m_pDoc->m_pDataCtrl->AddCscs(m_Data);
	if(!bResult) return;

	CDialogMove::OnOK();
}

//-----------------------------------------------------------------------------
// OnCancel   
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::OnCancel() 
{
	// Nothing
	CDialogMove::OnCancel();
}

//-----------------------------------------------------------------------------
// OnCmdApply   
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::OnCmdApply() 
{
	if(!m_wndGrid->ReCalcStiff(1)) return; // MQC 16082

	if(!SetDlg2Data(m_Data, TRUE))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_CSCS__Err_Input_Data));
		return;
	}

	if(m_bModify) m_pDoc->m_pDataCtrl->ModifyCscs(m_Key, m_Data);
	else          m_pDoc->m_pDataCtrl->AddCscs(m_Data);

}

//-----------------------------------------------------------------------------
// OnSelchangeCmdSectIdCombo    단면선택이 바뀌었을 때
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::ChangeSection()
{
	T_SECT_K key;
	if(!m_cboSection.GetSelectedSect(key)){ SetDefaultValue(); return; }

	T_SECT_D data;
	if(!m_pDoc->m_pAttrCtrl->GetSectOrSecv(key, data, m_bVirtual, TRUE, TRUE)){ SetDefaultValue(); return; }

	// Section Type & Shape 
	m_ctrlSectType.SetWindowText(ConvStypeToString(data.nStype));
	if(data.nStype == D_SECT_TYPE_COMPO_B) m_ctrlSectShape.SetWindowText(_ULS(CP_B));
	else if(data.nStype == D_SECT_TYPE_COMPO_I)   m_ctrlSectShape.SetWindowText(_ULS(CP_I));
	else if(data.nStype == D_SECT_TYPE_COMPO_TUB) m_ctrlSectShape.SetWindowText(_ULS(CP_T));
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_B) m_ctrlSectShape.SetWindowText(_ULS(CSGB));
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_I) m_ctrlSectShape.SetWindowText(_ULS(CSGI));
	else if(data.nStype == D_SECT_TYPE_COMPO_STLG_TUB) m_ctrlSectShape.SetWindowText(_ULS(CSGT));
	else if(data.nStype == D_SECT_TYPE_COMPO_CI)  m_ctrlSectShape.SetWindowText(_ULS(CPCI));
	else if(data.nStype == D_SECT_TYPE_COMPO_CT)  m_ctrlSectShape.SetWindowText(_ULS(CPCT));
	else if(data.nStype == D_SECT_TYPE_COMPO_PC)  m_ctrlSectShape.SetWindowText(_ULS(CPPC));
	else if(data.nStype == D_SECT_TYPE_COMPO_G)   m_ctrlSectShape.SetWindowText(_ULS(GEN));
	// MNET:XXXX-CJJEONG-20090317 (Tel : 2124) Taperd Composite General 추가
	else if(data.nStype == D_SECT_TYPE_TAPERED && data.SectBefore.nStype==D_SECT_TYPE_COMPO_G)
		m_ctrlSectShape.SetWindowText(_ULS(CP_G));
	else m_ctrlSectShape.SetWindowText(data.SectBefore.Shape);

	// Bitmap
	ChangeBitmap(data);

	// Composite Type - Enable/Disable
	//m_cboCompositeType.EnableWindow(IsNeedCompositeType(data));
	// 수정 
	InitCompositeTypeCombo(data);

	//if(data.nStype == D_SECT_TYPE_COMPO_G)
	//{
	//  m_nPartNum = data.SectBefore.SectI.aGeneralPart.GetSize();
	//}

	// Element List  ???
	int nIndex = m_cboActiveStage.GetCurSel();
	if(nIndex>=0)
	{
		CString strStage = _T("");
		m_cboActiveStage.GetLBText(nIndex, strStage);
		T_STAG_K nStagKey = m_pDoc->m_pAttrCtrl->GetStagKey(strStage);
		if(m_pDoc->m_pAttrCtrl->ExistStag(nStagKey)) 
			ChangeElememtList(nStagKey, key);
		m_wndGrid->SetCompoStageColumn(nStagKey);
	}
}

void CCMStageCompSectItemDlg::OnSelchangeCmdSectIdCombo() 
{
	// 여기서도 처리를 해주면 OnChangeCmdSectIdEdit과 겹치게 되어 
	// 두번 처리해 주는 결과를 낳음 
	// 항상 Edit -> Combo의 순으로 Change Message가 날아옴 (역은 성립안함) 
	//ChangeSection();
}

void CCMStageCompSectItemDlg::OnChangeCmdSectIdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	ChangeSection();
}

//-----------------------------------------------------------------------------
// OnSelchangeCmdCompositeTypeCombo   Composite Type이 바뀌었을 때 
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::OnSelchangeCmdCompositeTypeCombo() 
{
	const int nCompositType = GetCompositeType();
	m_wndGrid->SetCompositType(nCompositType);
	// 수정 
	CDlgUtil::CtrlShowHide(this, m_aPartGroup, (nCompositType == -1 || nCompositType == 2));
	m_edtPartNum.EnableWindow(nCompositType != 2);

	T_SECT_K key;
	if(!m_cboSection.GetSelectedSect(key)){ SetDefaultValue(); return; }
	SetInitGrid(key);
}

//-----------------------------------------------------------------------------
// OnSelchangeCmdActiveStageCombo   Active Stage 선택이 바뀌었을 때
//-----------------------------------------------------------------------------
void CCMStageCompSectItemDlg::OnSelchangeCmdActiveStageCombo() 
{
	// 만약 Section이 선택되지 않은 상태라면 Data가공만,
	// 선택된 상태라면 데이터 가공 후 ElemList까지 만들어 준다.
	CString strStage = _T("");
	int nIndex = m_cboActiveStage.GetCurSel();
	if(nIndex>=0) m_cboActiveStage.GetLBText(nIndex, strStage);
	T_STAG_K nStagKey = m_pDoc->m_pAttrCtrl->GetStagKey(strStage);
	if(!m_pDoc->m_pAttrCtrl->ExistStag(nStagKey)) return;

	T_SECT_K nSectKey;
	if(m_cboSection.GetSelectedSect(nSectKey)) 
		ChangeElememtList(nStagKey, nSectKey);	
	m_wndGrid->SetCompoStageColumn(nStagKey);
}


void CCMStageCompSectItemDlg::OnChangeCmdPartNumEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here  
	UpdateData(TRUE);
	m_wndGrid->SetRowNumberOnly(m_edtPartNum.GetEditValueInt());

}


BOOL CCMStageCompSectItemDlg::DestroyWindow() 
{
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	return CDialogMove::DestroyWindow();
}
