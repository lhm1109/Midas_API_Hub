// CMSectItemVal.cpp : implementation file
//
	// FEM, EQU option은 General Section이면 Enable되어 있고
	// Line Type Import시에만 효과 발휘

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemVal.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CMSectOffsetDlg.h"
#include "..\wg_db\CMSectImportSPC.h"
#include "CMSectItemPSCImport.h"

// Default Shape
#define D_SECT_VALUE_DEF_SHAPE D_SECT_SHAPE_REG_H


#define D_DEF_CALC_STIFF_OPT 1  // nCalcStiffOpt

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemVal dialog
#define   CDialog   CCMSectItemBase
namespace SectItemValResource
{
#if defined(_MGEN) || defined(_TOWER)
	UINT aBitmapID[] = {
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10, IDB_CMD_SEC_REG_24,
	};
	CString aImgPath[] = {
		_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon24.svg"),
	};
	DWORD nIndex[] = {
		 0,  1,  2,
		 3,  4,  5,
		 6,  7,  8,
		 9,  21,
	};
#elif defined(_CIVIL)
	UINT aBitmapID[] = {
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10,
		IDB_CMD_SEC_REG_13, IDB_CMD_SEC_REG_14, IDB_CMD_SEC_REG_21,
		IDB_CMD_SEC_REG_15, IDB_CMD_SEC_REG_16, IDB_CMD_SEC_REG_17,IDB_CMD_SEC_REG_24
	};
	CString aImgPath[] = {
		_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon13.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon14.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon21.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon15.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon16.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon17.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon24.svg"),
	};
	DWORD nIndex[] = {
		 0,  1,  2,
		 3,  4,  5,
		 6,  7,  8,
		 9,
		12, 13, 18,
		14, 15, 16,
		21
	};
#else
#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif

	CString aImgPath_Sect[] = {
	_T("SVG\\illustration\\Dialog\\Sec01.svg"),
	_T("SVG\\illustration\\Dialog\\Sec02.svg"),
	_T("SVG\\illustration\\Dialog\\Sec20.svg"),
	_T("SVG\\illustration\\Dialog\\Sec04.svg"),
	_T("SVG\\illustration\\Dialog\\Sec05.svg"),
	_T("SVG\\illustration\\Dialog\\Sec06.svg"),
	_T("SVG\\illustration\\Dialog\\Sec07.svg"),
	_T("SVG\\illustration\\Dialog\\Sec08.svg"),
	_T("SVG\\illustration\\Dialog\\Sec09.svg"),
	_T("SVG\\illustration\\Dialog\\Sec10.svg"),
	_T("SVG\\illustration\\Dialog\\Sec11.svg"),
	_T("SVG\\illustration\\Dialog\\Sec12.svg"),
	_T("SVG\\illustration\\Dialog\\Sec13.svg"),
	_T("SVG\\illustration\\Dialog\\Sec14.svg"),
	_T("SVG\\illustration\\Dialog\\Sec15.svg"),
	_T("SVG\\illustration\\Dialog\\Sec16.svg"),
	_T("SVG\\illustration\\Dialog\\Sec17.svg"),
	_T("SVG\\illustration\\Dialog\\Sec18.svg"),
	_T("SVG\\illustration\\Dialog\\Sec21.svg"),
	_T("SVG\\illustration\\Dialog\\Sec23.svg"),
	_T("SVG\\illustration\\Dialog\\Sec24.svg"),
	_T("SVG\\illustration\\Dialog\\Sec25.svg"),

	};


}

CCMSectItemVal::CCMSectItemVal(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemVal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemVal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlROct.Add(IDC_CMD_TEXT_CELL_NUM);
	m_aCtrlROct.Add(IDC_CMD_EDIT_CELL_NUM);
	m_aCtrlGS.Add(IDC_CMD_IMPORT_BTN);
	m_aCtrlGS.Add(IDC_CMD_SEL_BTN);
	m_aCtrlGS.Add(IDC_CMD_IMPORT_EDIT);
	m_aCtrlGS.Add(IDC_CMD_FEM_RDO); 
	m_aCtrlGS.Add(IDC_CMD_EQU_RDO);
	m_aCtrlOption.Add(IDC_CMD_FEM_RDO);
	m_aCtrlOption.Add(IDC_CMD_EQU_RDO);
	m_aCtrlAType.Add(IDC_WG_CMD_SECT_SYMMETRY);
	m_aCtrlAType.Add(IDC_CMD_SECT_ATYPE_COMBO);

	m_CurSectID = 0;
	m_aImportID.RemoveAll();
	m_aImportSect.RemoveAll();

}

CCMSectItemVal::~CCMSectItemVal()
{
}

void CCMSectItemVal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_EDIT_CELL_NUM, m_editCellNum);
	DDX_Control(pDX, IDC_CMD_SP_ID_BUILTUP, m_wndBuiltUp);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_IMPORT_EDIT, m_editPath);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID, m_wndGridFirst);
	DDX_Control(pDX, IDC_CMD_SP_ID_BMP, m_ImgSectItem);
	DDX_Control(pDX, IDC_CMD_SECT_ATYPE_COMBO, m_cbxAType);
	//}}AFX_DATA_MAP

}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemVal::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemVal::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		CString strID;
		strID.Format(_T("%d"), m_Key);
		m_wndID.SetWindowText(strID);
		//m_Data.SName = _T("");
		//m_wndName.SetWindowText(m_Data.SName);
		m_wndName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemVal::ShowCalcResult()
{
	return;
}

void CCMSectItemVal::DisplayOffsetPoint()
{
	m_wndSecView.Invalidate();	
}

void CCMSectItemVal::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemVal::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

#ifdef _MGEN_CH
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
#endif

	BOOL bGrpdMod = FALSE;
	BOOL bStfnMod = FALSE;
	if(m_pDoc->m_pAttrCtrl2->ExistGrpdSsm(m_Key)) bGrpdMod = TRUE;
	if(m_pDoc->m_pAttrCtrl2->ExistStfnSsm(m_Key)) bStfnMod = TRUE;

	if(m_bModifyGrpd)
	{
		if(bGrpdMod) m_pDoc->m_pDataCtrl->ModifyGrpdSsm(m_Key, m_Key, m_GrpdSSMD);
		else         m_pDoc->m_pDataCtrl->AddGrpdSsm(m_Key, m_GrpdSSMD);
	}

	if(m_bModifyStfn)
	{
		if(bStfnMod) m_pDoc->m_pDataCtrl->ModifyStfnSsm(m_Key, m_Key, m_StfnSSMD);
		else         m_pDoc->m_pDataCtrl->AddStfnSsm(m_Key, m_StfnSSMD);
	}
			
	// MNET:XXXX-HSSHIM-20130725. 
	// Value Type은 Calc. 버튼을 제공하므로 Calc 버튼에서 재계산하도록 아래 코드 막음.
	//m_pDoc->m_pSectDB->CalcSectData(m_Data,FALSE);

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemVal::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}
	
	// save size
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon;
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	CArray<int, int> aLineLoop;
	CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape;
	aOuterPolygon.Copy(m_Data.SectBefore.SectI.aOuterPolygon);
	aInnerPolygon.Copy(m_Data.SectBefore.SectI.aInnerPolygon);
	aLine.Copy(m_Data.SectBefore.SectI.aLine);
	aLineLoop.Copy(m_Data.SectBefore.SectI.aLineLoop);
	aShape.Copy(m_Data.SectBefore.SectI.aShape);
	BOOL bInputSect = m_Data.bInputSec;

	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_USER;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT)
	{
		m_Data.SectBefore.nCellShape = m_editCellNum.GetEditValueInt();
	}

	// Built-Up
	if (m_wndBuiltUp.GetCheck() == 1)
		m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef; // MQC:1493 20071130 mylee
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
#ifdef _MGEN_CH
	m_Data.SectBefore.bConsiderWarpingEffect = TRUE;
#else
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
#endif

	// size, peri and stiffness
	m_wndGridFirst.SaveData();
	m_Data.SectBefore.SectI.Stiffness.WArea = m_Data.SectBefore.SectI.Stiffness.Area;

	// general section
	m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon);
	m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon);
	m_Data.SectBefore.SectI.aLine.Copy(aLine);
	m_Data.SectBefore.SectI.aLineLoop.Copy(aLineLoop);
	m_Data.SectBefore.SectI.aShape.Copy(aShape);
#ifdef _MGEN_CH
	if (m_Data.nStype == D_SECT_TYPE_USER && m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
	{
		m_Data.nStype = D_SECT_TYPE_PSC;
		m_Data.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
		BOOL bConsiderWarpingEffect = m_Data.SectBefore.bConsiderWarpingEffect;
		m_Data.SectBefore.bConsiderWarpingEffect = TRUE;
		double dTempSize[4];
		for (int i = 0; i < 4; i++)
		{
			dTempSize[i] = m_Data.SectBefore.SectI.Size[i];
			m_Data.SectBefore.SectI.Size[i] = 1.0;
		}
		CSectUtil::CalcSectData(m_Data, TRUE, FALSE, TRUE, TRUE);
		m_Data.nStype = D_SECT_TYPE_USER;
		m_Data.SectBefore.Shape = D_SECT_SHAPE_REG_GEN;
		for (int i = 0; i < 4; i++)
		{
			m_Data.SectBefore.SectI.Size[i] = dTempSize[i];
		}
		m_Data.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;
	}
#endif
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);

	//symmetrical type
	m_Data.nSectAType = m_cbxAType.GetCurSel();
	m_Data.bInputSec = bInputSect;
#ifdef _MGEN_CH
	if (!CheckData())
	{
		return FALSE;
	}
#endif
	return TRUE;
}

void CCMSectItemVal::InitControls()
{
	// Init grid for user section
	m_wndGridFirst.InitGrid(&m_wndSecView);

	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	SetSectionATypeCombo();
	m_cbxAType.SetCurSel(0);

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
	m_editPath.SetReadOnly(TRUE);

	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	m_editCellNum.SetRange(1, 4);
	m_editCellNum.SetInteger(TRUE);

}

void CCMSectItemVal::SetSectionATypeCombo()
{
	CString aItemName[] = {
		_LS(IDS_WG_CMD_Section_Symmetry_Type_Double), _LS(IDS_WG_CMD_Section_Symmetry_Type_Y), 
		_LS(IDS_WG_CMD_Section_Symmetry_Type_Z), _LS(IDS_WG_CMD_Section_Symmetry_Type_Non)};

	m_cbxAType.ResetContent();

	for (int i=0; i<4; i++)
	{
		m_cbxAType.AddString(aItemName[i]);
	}
}

void CCMSectItemVal::SetSectionShapeCombo()
{
#if defined(_MGEN) || defined(_TOWER)
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section),
		_LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe),
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle),
		_LS(IDS_WG_CMD__ADDD__Solid_Round), _LS(IDS_WG_CMD__ADDD__General_Section),
	};
#elif defined(_CIVIL)
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section),
		_LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe),
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle),
		_LS(IDS_WG_CMD__ADDD__Solid_Round),
		_LS(IDS_WG_CMD__ADDD__Octagon), _LS(IDS_WG_CMD__ADDD__Solid_Octagon), _LS(IDS_WG_CMD__ADDD__Rect_Octagon),
		_LS(IDS_WG_CMD__ADDD__Track), _LS(IDS_WG_CMD__ADDD__Solid_Track), _LS(IDS_WG_CMD__ADDD__Half_Track),
		_LS(IDS_WG_CMD__ADDD__General_Section)
	};
#else
#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif
	int nCount = sizeof(SectItemValResource::aBitmapID)/sizeof(UINT);

	// Attach image list to ComboBox
	m_cboType.SetImageList(SectItemValResource::aImgPath, nCount);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));

	// Make items
	CString str;
	int nMovingType = CProduct::GetMovingType();
	for (int i = 0; i < nCount; i++)
	{
		str = aItemName[i];
		if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS) 
		{
			if (SectItemValResource::nIndex[i] != 16) str.Replace('H', 'I');
		}
		m_cboType.AddString(str);
		m_cboType.SetItemData(i, SectItemValResource::nIndex[i]);
	}
}

void CCMSectItemVal::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemVal::ShowData2Dlg()
{
	// name
	m_wndName.SetWindowText(m_Data.SName);

	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	SetCurShapeByIndex(nShapeIndex);
	// show/hide
	CDlgUtil::CtrlShowHide(this, m_aCtrlROct, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGS, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN);

	if (m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT)
	{
		m_editCellNum.SetValue(m_Data.SectBefore.nCellShape);
	}

	// BuiltUp
	SetBuiltUp();

	m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);

#ifdef _MGEN_CH
	CDlgUtil::CtrlShowHide(this, m_aCtrlAType, m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN);
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN && m_Data.bInputSec)
	{
		m_wndGridFirst.SetWarpingEffect(TRUE);
	}
#else
	CDlgUtil::CtrlShowHide(this, m_aCtrlAType, FALSE);
#endif
	// size and stiffness
	m_wndGridFirst.ShowData();

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_Data.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_Data.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_Data.SectBefore.bConsiderWarpingEffect);
	
	if(!CSectDB::IsEnableSectShape7thDOF(m_Data.SectBefore.Shape))
	{
		m_Data.SectBefore.bConsiderWarpingEffect = FALSE;
		m_chkWarpingEffect.ShowWindow(SW_HIDE);
	}
	else
	{
		BOOL bHide_7thDOF = FALSE;
		if (!CSectDB::IsEnable7thDOF())
			bHide_7thDOF = TRUE;
		
		m_chkWarpingEffect.ShowWindow(!bHide_7thDOF);
	}

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGS, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN);
	if (m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN && m_Data.SectBefore.SectI.aLine.GetSize() > 0)
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, TRUE);
	else 
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);

	m_cbxAType.SetCurSel(m_Data.nSectAType);

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	m_wndSecView.Invalidate();
}

void CCMSectItemVal::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

// 	UINT aBitmapID[] = {
// 		//IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
// 		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP20,
// 		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
// 		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
// 		IDB_CMD_SP_ISGL_BMP10, IDB_CMD_SP_ISGL_BMP11, IDB_CMD_SP_ISGL_BMP12,
// 		IDB_CMD_SP_ISGL_BMP13, IDB_CMD_SP_ISGL_BMP14, IDB_CMD_SP_ISGL_BMP15,
// 		IDB_CMD_SP_ISGL_BMP16, IDB_CMD_SP_ISGL_BMP17, IDB_CMD_SP_ISGL_BMP18,
// 		IDB_CMD_SP_ISGL_BMP21, IDB_CMD_SP_ISGL_BMP23, IDB_CMD_SP_ISGL_BMP24,
// 		IDB_CMD_SP_ISGL_BMP25
// 	};

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT) // ROCT Shape
	{
		CString csImagePath = (m_editCellNum.GetEditValueInt() == 1) ? _T("SVG\\illustration\\Dialog\\Sec21.svg") : _T("SVG\\illustration\\Dialog\\Sec22.svg");
		m_ImgSectItem.SetImage(csImagePath);
	}
	else m_ImgSectItem.SetImage(SectItemValResource::aImgPath_Sect[nShapeIndex]);
}

void CCMSectItemVal::SetBuiltUp()
{
	// Built-Up Setting
	if (m_Data.SectBefore.SectI.DBName == _T("")) 
		m_wndBuiltUp.EnableWindow(TRUE);
	else m_wndBuiltUp.EnableWindow(FALSE);
	if (m_Data.SectBefore.SectI.BuiltUpFlag == 2) m_wndBuiltUp.SetCheck(0);
	else m_wndBuiltUp.SetCheck(1);
}

int CCMSectItemVal::GetCurShapeIndex()
{
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) return LB_ERR;

	nShapeIndex = (int)m_cboType.GetItemData(nShapeIndex);
	return nShapeIndex;
}

void CCMSectItemVal::SetCurShapeByIndex(int nShapeIndex)
{
	int nCount = m_cboType.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		if ((int)m_cboType.GetItemData(i) == nShapeIndex)
			break;
	}
	nShapeIndex = i;

	m_cboType.SetCurSel(nShapeIndex);
}

BEGIN_MESSAGE_MAP(CCMSectItemVal, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemVal)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_EN_CHANGE(IDC_CMD_SP_ID_ID, OnChangeID)
	ON_EN_CHANGE(IDC_CMD_SP_ID_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BUILTUP, OnChangeBuiltUp)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_EDIT_CELL_NUM, OnChangeCmdEditCellNum)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SPIN_CELL_NUM, OnDeltaposCmdSpinCellNum)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK, OnChkWarpingEffect)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdImportBtn)
	ON_BN_CLICKED(IDC_CMD_SEL_BTN, OnCmdSelectBtn)
	ON_BN_CLICKED(IDC_CMD_FEM_RDO, OnCmdFemRdo)
	ON_BN_CLICKED(IDC_CMD_EQU_RDO, OnCmdFemRdo)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_ATYPE_COMBO, OnChangeSelAType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemVal message handlers

BOOL CCMSectItemVal::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetRedraw(FALSE);

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.
	MoveItems();
	ResizeDialog();
	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_USER;
		m_Data.SectBefore.Shape = D_SECT_VALUE_DEF_SHAPE;
	}
	m_wndSecView.SetDataSource(&m_Data);

	// size and stiffness grid
	m_wndGridFirst.SetDataSource(&m_Data.SectBefore.SectI, 0, D_SECT_TYPE_USER, &m_Data.SectBefore.nCellShape);
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	m_wndGridFirst.ChangeShape(nShapeIndex);
	m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);
	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemVal::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Shape의 변화를 Notify한다.
	m_wndGridFirst.ChangeShape(nShapeIndex);

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	T_SECT_SECTBASE_D *pSectF;
	pSectF = &m_Data.SectBefore.SectI;

	// Section Name은 보존
	m_wndName.GetWindowText(m_Data.SName);

	// Peri 초기화
	m_Data.SectBefore.SectI.PeriIn = 0.0;
	m_Data.SectBefore.SectI.PeriOut = 0.0;

	// First 초기화
	pSectF->Initialize();
	m_Data.SectBefore.nCalcStiffOpt = D_DEF_CALC_STIFF_OPT;

	// Dialog에 반영
	ShowData2Dlg();		
}

void CCMSectItemVal::OnChkWarpingEffect()
{
#ifdef _MGEN_CH
	//m_Data.SectBefore.bConsiderWarpingEffect = TRUE;
	return;
#endif
	Dlg2Data();
	BOOL bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;
	m_wndGridFirst.SetWarpingEffect(bConsiderWarpingEffect);
}

void CCMSectItemVal::OnChangeID() 
{
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = _tstol(csID);
}

void CCMSectItemVal::OnChangeName() 
{
	m_wndName.GetWindowText(m_Data.SName);
}

void CCMSectItemVal::OnChangeBuiltUp() 
{
	// TODO: Add your control notification handler code here
	if (m_wndBuiltUp.GetCheck()) m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;
}

void CCMSectItemVal::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}

void CCMSectItemVal::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	if (!m_wndGridFirst.GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	//m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();			
}

void CCMSectItemVal::OnChangeCmdEditCellNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// cell number : 1 ~ 4
	int nCellNum = m_editCellNum.GetEditValueInt();
	if (nCellNum < 1  || nCellNum > 4)
	{
		if (nCellNum > 4) nCellNum = 4;
		if (nCellNum < 1) nCellNum = 1;
		m_editCellNum.SetValue(nCellNum);
	}

	ChangeBitmap();

	m_Data.SectBefore.nCellShape = nCellNum;
	m_wndSecView.Invalidate();	
}

// void CCMSectItemVal::OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_EDIT_CELL_NUM,_T("1"),_T("4"),1,pNMHDR,pResult);
// 	
// 	CString csCellNum;
// 	m_editCellNum.GetWindowText(csCellNum);
// 	m_Data.SectBefore.nCellShape = _ttoi(csCellNum);
// 	ChangeBitmap();
// 	m_wndSecView.Invalidate();
// 
// 	*pResult = 0;
// }

void CCMSectItemVal::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D OldData;
	OldData = m_Data.SectBefore;

	CCMSectOffsetDlg dlg(&m_Data.SectBefore, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray arOffsetNameList;
		CSectUtil::GetOffsetNameList(arOffsetNameList);
		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}				
	else m_Data.SectBefore = OldData;

	m_wndSecView.Invalidate();	
}
void CCMSectItemVal::ResizeDialog()
{
	UINT nLastCtrlID;
	nLastCtrlID = IDC_CMD_WARPING_EFFECT_CHK;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+4;

	MoveWindow(r);
}

void CCMSectItemVal::MoveItems()
{
	CArray<UINT, UINT> m_aCtrlMove;
	m_aCtrlMove.Add(IDC_CMD_OFFSET_STATIC);
	m_aCtrlMove.Add(IDC_CMD_SP_ID_OFFSET);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Y_STATIC);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Y_EDIT);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Y_UNIT);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Z_STATIC);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Z_EDIT);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_Z_UNIT);
	m_aCtrlMove.Add(IDC_CMD_OFFSET_BTN);
	m_aCtrlMove.Add(IDC_CMD_SECT_CENTROID_BTN);
	CDlgUtil::CtrlMoveDistY(this,m_aCtrlMove,globalUtils.ScaleByDPI(100),false);

	CRect RectTo,RectBefore;
	CWnd* pWndTo=GetDlgItem(IDC_CMD_TEXT_CELL_NUM);
	pWndTo->GetWindowRect(RectTo);
	CWnd* pWndBefore=GetDlgItem(IDC_CMD_IMPORT_BTN);
	pWndBefore->GetWindowRect(RectBefore);
	CDlgUtil::CtrlMoveDistY(this,m_aCtrlGS,RectTo.top-RectBefore.top,false);
}

void CCMSectItemVal::OnCmdImportBtn() 
{
	CString strSec;
	if (CProduct::IsRusLocal())
		strSec = _T("Section Files(*.sec)|*.sec|All File(*.*)|*.*||");
	else
		strSec = _T("MIDAS Section Files(*.sec)|*.sec|All File(*.*)|*.*||");

	// TODO: Add your control notification handler code here
	CString csFileName;
	CFileDialog fd(TRUE, _T(".sec"), NULL,
								 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
								 strSec, NULL);
	if (fd.DoModal() != IDOK) return;
	
	CWaitCursor wait;
	CCMSectImportSPC Importer;
	if (!Importer.Import(fd.GetPathName()))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_CAN_NOT_IMPORT));
		return;
	}

	Importer.GetList(m_aImportID, m_aImportSect);

	m_editPath.SetWindowText(fd.GetPathName());
	m_editPath.SetFocus();
	m_editPath.SetSel(0, -1);

	GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(TRUE);
	OnCmdSelectBtn();

	// m_Data 채운 이후 동작
	T_GRPD_SSM_D GrpdSSMD; GrpdSSMD.Initialize();
	T_STFN_SSM_D StfnSSMD; StfnSSMD.Initialize();
	if(Importer.ConvertSectionManager(m_Data, GrpdSSMD, StfnSSMD))
	{
		m_bModifyGrpd = TRUE;
		m_bModifyStfn = TRUE;
		m_GrpdSSMD = GrpdSSMD;
		m_StfnSSMD = StfnSSMD;
	}
	//

	int nConsiderWarpingEffect = (m_Data.SectBefore.bConsiderWarpingEffect)? 1 : 0;
	m_chkWarpingEffect.SetCheck(nConsiderWarpingEffect);

	//
	m_Data.nSectAType = GetSectionAType();
	m_cbxAType.SetCurSel(m_Data.nSectAType);
	m_wndGridFirst.SetWarpingEffect(TRUE);
	m_Data.bInputSec = TRUE;
}	

void CCMSectItemVal::OnCmdSelectBtn() 
{
	// TODO: Add your control notification handler code here  
	CCMSectItemPSCImport dlg;
	dlg.SetData(m_aImportID, m_aImportSect, m_CurSectID);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_CurSectID, m_Data);
		
		// Qy, Qz 계산
		m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = TRUE; 

		double dQy=0., dQz=0.;
		if (CSectUtil::GetPscValueDataQ(&m_Data, dQy, dQz))
		{
			m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
			m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
		}
		m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = FALSE; 

		m_Data.nStype = D_SECT_TYPE_USER;
		m_Data.SectBefore.Shape = D_SECT_SHAPE_REG_GEN;

		if(m_Data.SectBefore.SectI.aLine.GetSize()>0)
		{
			// Line Type은 강성 계산 (비틀림 값)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, TRUE);
			OnCmdFemRdo();
		}
		else
		{
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);

//      // 7자유도 계산 
//      BOOL bUse7thDOFLib = FALSE;
//      if (CSectDB::IsEnable7thDOF())
//      {
//        bUse7thDOFLib = TRUE;
//      }
//      // Line Type SPC 단면은 제외.
//      if(m_Data.SectBefore.SectI.aLine.GetSize()>0)  { bUse7thDOFLib = FALSE; }   
//      
//      if (bUse7thDOFLib)
//      {
//        CDBDoc* pDoc = CDBDoc::GetDocPoint();
//
//        T_SECT_D& SectD = m_Data;
//        SectD.SectBefore.bConsiderWarpingEffect = TRUE;
//        {
//          CArray<T_SECT_ADDI_STIFF,T_SECT_ADDI_STIFF&> aSectAddiStiff;  aSectAddiStiff.RemoveAll();
//          BOOL bAllStiffness = FALSE;
//          pDoc->m_pSectDB->CalcWarpingFuncion(SectD, aSectAddiStiff, bAllStiffness);
//        }
//        SectD.SectBefore.bConsiderWarpingEffect = FALSE;
//      }

		#ifdef _MGEN_CH
			if (m_Data.nStype == D_SECT_TYPE_USER && m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
			{
				m_Data.nStype = D_SECT_TYPE_PSC;
				m_Data.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
				BOOL bConsiderWarpingEffect = m_Data.SectBefore.bConsiderWarpingEffect;
				m_Data.SectBefore.bConsiderWarpingEffect = TRUE;
				double dTempSize[4];
				for (int i = 0; i < 4; i++)
				{
					dTempSize[i] = m_Data.SectBefore.SectI.Size[i];
					m_Data.SectBefore.SectI.Size[i] = 1.0;
				}
				CSectUtil::CalcSectData(m_Data, TRUE, FALSE, TRUE, TRUE);
				m_Data.nStype = D_SECT_TYPE_USER;
				m_Data.SectBefore.Shape = D_SECT_SHAPE_REG_GEN;
				for (int i = 0; i < 4; i++)
				{
					m_Data.SectBefore.SectI.Size[i] = dTempSize[i];
				}
				m_Data.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;
			}
		#else
			if (CSectDB::IsEnable7thDOF())
			{
				// [경고] SPC에서 import한 단면은 7자유도 단면 속성이 자동 계산되지 않습니다.
				m_pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__Warning_Cannot_auto_calc_7thDOF_because_from_SPC));
			}
		#endif

			// size and stiffness
			m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			m_wndGridFirst.ShowData();
			m_wndSecView.Invalidate();	
		}

		T_SECT_D SectTmp = m_Data;
		if(!CSectUtil::IsMeshOK(SectTmp))
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning_mesh_NG_ImportFromSPC));
	}
}

void CCMSectItemVal::OnCmdFemRdo() 
{
	int nCalc;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlOption,nCalc);
	m_Data.SectBefore.nCalcStiffOpt=nCalc;                        
	if (m_Data.SectBefore.SectI.aLine.GetSize() > 0)
	{
		T_SECT_D SectD = m_Data;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		SectD.SectBefore.SectI.Stiffness.Initialize();
		if (pDoc->m_pSectDB->CalcSectData(SectD, TRUE))
		{
			m_Data.SectBefore.SectI.Stiffness.Rxx = SectD.SectBefore.SectI.Stiffness.Rxx;
			m_Data.SectBefore.SectI.Stiffness.Asy = SectD.SectBefore.SectI.Stiffness.Asy;
			m_Data.SectBefore.SectI.Stiffness.Asz = SectD.SectBefore.SectI.Stiffness.Asz;
			m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			m_wndGridFirst.ShowData();
			m_wndSecView.Invalidate();	
		}
	}
}

void CCMSectItemVal::OnChangeSelAType()
{
	m_wndGridFirst.SetWarpingEffect(TRUE);
}

int  CCMSectItemVal::GetSectionAType()
{
	double dy1 = m_Data.SectBefore.SectI.Stiffness.dx1;
	double dy2 = m_Data.SectBefore.SectI.Stiffness.dx2;
	double dy3 = m_Data.SectBefore.SectI.Stiffness.dx3;
	double dy4 = m_Data.SectBefore.SectI.Stiffness.dx4;
	double dz1 = m_Data.SectBefore.SectI.Stiffness.dy1;
	double dz2 = m_Data.SectBefore.SectI.Stiffness.dy2;
	double dz3 = m_Data.SectBefore.SectI.Stiffness.dy3;
	double dz4 = m_Data.SectBefore.SectI.Stiffness.dy4;
	if (fabs(dy1 + dy2)  < 1.0e-10 && fabs(dy3 + dy4) < 1.0e-10 && fabs(dz2 + dz3) < 1.0e-10 && fabs(dz1 + dz4) < 1.0e-10)
		return 0;
	else if ((fabs(dy1 + dy2) > 1.0e-10 || fabs(dy3 + dy4) > 1.0e-10) && fabs(dz2 + dz3) < 1.0e-10 && fabs(dz1 + dz4) < 1.0e-10)
		return 1;
	else if (fabs(dy1 + dy2) < 1.0e-10 && fabs(dy3 + dy4) < 1.0e-10 && (fabs(dz2 + dz3) > 1.0e-10 || fabs(dz1 + dz4) > 1.0e-10))
		return 2;
	else
		return 0;
}

BOOL CCMSectItemVal::CheckData()
{
	double dIw = m_Data.SectBefore.SectI.Stiffness.dIw;
	double dys_yc = m_Data.SectBefore.SectI.Stiffness.dYsy;
	double dzs_zc = m_Data.SectBefore.SectI.Stiffness.dYsz;
	if (m_Data.nSectAType == 1)
	{
		if (dIw == 0 || dys_yc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error1));
			return FALSE;
		}
	}
	else if (m_Data.nSectAType == 2)
	{
		if (dIw == 0 || dzs_zc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error2));
			return FALSE;
		}
	}
	else if (m_Data.nSectAType == 3)
	{
		if (dIw == 0 || dys_yc == 0 || dzs_zc == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_Section_Symmetry_Type_Error3));
			return FALSE;
		}
	}

	return TRUE;
}
