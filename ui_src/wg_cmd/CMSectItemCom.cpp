// CMSectItemCom.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemCom.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\SectDBRussiaUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CMSectOffsetDlg.h"

// Default Combined Section Shape
#define D_SECT_COMBINED_DEF_SHAPE D_SECT_SHAPE_COMA_2H

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCom dialog
#define   CDialog   CCMSectItemBase

CCMSectItemCom::CCMSectItemCom(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemCom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemCom)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBitmap = NULL;
	m_pImageList = NULL;

	m_aCtrlDB.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_DB_RADIO);
}

CCMSectItemCom::~CCMSectItemCom()
{
	if(m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList= NULL;}
}

void CCMSectItemCom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemCom)
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME2, m_wndSecondName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME1, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_SP_ID_BMP, m_ImgSectItem);
	//}}AFX_DATA_MAP

	UINT aFirstID[][5] = {
		{ IDC_CMD_SP_ID_TITLE11, IDC_CMD_SP_ID_TITLE12,
			IDC_CMD_SP_ID_TITLE13, IDC_CMD_SP_ID_TITLE14, IDC_CMD_SP_ID_TITLE15 },
		{ IDC_CMD_SP_ID_VALUE11, IDC_CMD_SP_ID_VALUE12,
			IDC_CMD_SP_ID_VALUE13, IDC_CMD_SP_ID_VALUE14, IDC_CMD_SP_ID_VALUE15 },
		{ IDC_CMD_SP_ID_UNIT11,  IDC_CMD_SP_ID_UNIT12,
			IDC_CMD_SP_ID_UNIT13,  IDC_CMD_SP_ID_UNIT14,  IDC_CMD_SP_ID_UNIT15 },
	};
	UINT aSecondID[][4] = {
		{ IDC_CMD_SP_ID_TITLE21, IDC_CMD_SP_ID_TITLE22,
			IDC_CMD_SP_ID_TITLE23, IDC_CMD_SP_ID_TITLE24 },
		{ IDC_CMD_SP_ID_VALUE21, IDC_CMD_SP_ID_VALUE22,
			IDC_CMD_SP_ID_VALUE23, IDC_CMD_SP_ID_VALUE24 },
		{ IDC_CMD_SP_ID_UNIT21,  IDC_CMD_SP_ID_UNIT22,
			IDC_CMD_SP_ID_UNIT23,  IDC_CMD_SP_ID_UNIT24 },
	};
	for (int i = 0; i < 5; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		DDX_Control(pDX, aSecondID[0][i], m_wndSecondTitle[i]);
		DDX_Control(pDX, aSecondID[1][i], m_wndSecondSize[i]);
		DDX_Control(pDX, aSecondID[2][i], m_wndSecondUnit[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemCom::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemCom::OnApplyPublic()
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
		//m_wndName.SetFocus();
		m_wndFirstName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemCom::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}
	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

void CCMSectItemCom::DisplayOffsetPoint()
{
	m_wndSecView.Invalidate();
}

void CCMSectItemCom::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemCom::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemCom::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}
	
	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_COMBINED;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = m_cboType.GetItemData(m_cboType.GetCurSel());
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexCom(nShapeIndex);
	
	// Built-Up : Always 1(built-up)
	m_Data.SectBefore.SectI.BuiltUpFlag = 1;

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// First
	T_SECT_SECTBASE_D* pSectF = &(m_Data.SectBefore.SectI);
	{
		// DB
		pSectF->DBName = GetDB();
		// Shape
		pSectF->Shape = CSectUtil::GetFirstShapeFromComShape(m_Data.SectBefore.Shape);
		// SName
		if (pSectF->DBName == _T("")) pSectF->SName = _T("");
		else m_wndFirstName.GetWindowText(pSectF->SName);

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSectF->DBName, pSectF->SName);
#endif

		// Size
		int nSizeNum = CSectUtil::GetFirstSizeNumFromIndexCom(nShapeIndex);
		for (int i = 0; i < nSizeNum; i++)
			pSectF->Size[i] = m_wndFirstSize[i].GetEditValue();
	}

	// Second
	T_SECT_SECTBASE_D* pSectS = &(m_Data.SectBefore.SectJ);
	{
		// DB
		if (CSectUtil::IsCombinedAType(m_Data)) pSectS->DBName = pSectF->DBName;
		else pSectS->DBName = _T("");
		// Shape
		pSectS->Shape = CSectUtil::GetSecondShapeFromComShape(m_Data.SectBefore.Shape);
		// SName
		if (pSectS->DBName == _T("")) pSectS->SName = _T("");
		else m_wndSecondName.GetWindowText(pSectS->SName);

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSectS->DBName, pSectS->SName);
#endif

		// Size
		int nSizeNum = CSectUtil::GetSecondSizeNumFromIndexCom(nShapeIndex);
		for (int i = 0; i < nSizeNum; i++)
			pSectS->Size[i] = m_wndSecondSize[i].GetEditValue();
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	return TRUE;
}

BOOL CCMSectItemCom::GetPeriAndStiff()
{
	return CSectUtil::CalcSectData(m_Data);
}


void CCMSectItemCom::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init DB name combo
	SetDBNameList();
	m_wndDB.SetCurSel(0);

	// Init first/second name combo
	m_wndFirstName.SetActionWhenKillFocus(2, -1);
	m_wndSecondName.SetActionWhenKillFocus(2, -1);  

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	for (int i = 0; i < 5; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (int i = 0; i < 4; i++)
	{
		m_wndSecondSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSecondUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
}

void CCMSectItemCom::SetSectionShapeCombo()
{
	enum EN_COMB_SECT_SHAPE
	{
		EN_SECT_COMA_2H = 0,
		EN_SECT_COMA_HC1,
		EN_SECT_COMA_HC2,
		EN_SECT_COMA_HT,
		EN_SECT_COMA_2T1,
		EN_SECT_COMA_2T2,
		EN_SECT_COMA_H2T,
		EN_SECT_COMA_WOH,
		EN_SECT_COMB_2CW1,
		EN_SECT_COMB_2CW2,
		EN_SECT_COMB_1CW1,
		EN_SECT_COMB_1CW2,
		EN_SECT_COMB_4L,
		EN_SECT_COMB_HP,
		EN_SECT_COMA_FOH,
	};
	//UINT aBitmapID[] = { 
	//	IDB_CMD_SEC_COMA_01, IDB_CMD_SEC_COMA_02, IDB_CMD_SEC_COMA_03,
	//	IDB_CMD_SEC_COMA_04, IDB_CMD_SEC_COMA_05, IDB_CMD_SEC_COMA_06,
	//	IDB_CMD_SEC_COMA_07, IDB_CMD_SEC_COMA_08, IDB_CMD_SEC_COMA_08,
	//	IDB_CMD_SEC_COMB_01, IDB_CMD_SEC_COMB_02, IDB_CMD_SEC_COMB_03,
	//	IDB_CMD_SEC_COMB_04, IDB_CMD_SEC_COMB_05, IDB_CMD_SEC_COMB_06,
	//};
	//CString aItemName[] = {
	//	_LS(IDS_WG_CMD__ADDD__2H_Combined_Shape), _LS(IDS_WG_CMD__ADDD__H_C_Combined_Shape), _LS(IDS_WG_CMD__ADDD__H_C_Combined_Shape2),
	//	_LS(IDS_WG_CMD__ADDD__H_T_Combined_Shape), _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_H_), _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_H_2),
	//	_LS(IDS_WG_CMD__ADDD__H_Shape_with_2T_or_Web), _LS(IDS_WG_CMD__ADDD__Web_Opened_H_with_Stiffener), _LS(IDS_WG_CMD__ADDD__Flange_Opened_H_with_Stiffener),
	//	_LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate1), _LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate2), _LS(IDS_WG_CMD__ADDD__C_with_Web_Plate1),
	//	_LS(IDS_WG_CMD__ADDD__C_with_Web_Plate2), _LS(IDS_WG_CMD__ADDD__4_Angle), _LS(IDS_WG_CMD__ADDD__H_shape_with_Plate),
	//};
	const int nMovingType = CProduct::GetMovingType();

// 	CString aImgPath[] = {
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon01.svg"),  //IDB_CMD_SEC_COMA_01
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon02.svg"),  //IDB_CMD_SEC_COMA_02
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon03.svg"),  //IDB_CMD_SEC_COMA_03
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon04.svg"),  //IDB_CMD_SEC_COMA_04
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon05.svg"),  //IDB_CMD_SEC_COMA_05
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon06.svg"),  //IDB_CMD_SEC_COMA_06
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon07.svg"),  //IDB_CMD_SEC_COMA_07
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon08.svg"),  //IDB_CMD_SEC_COMA_08
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon08F.svg"),  //IDB_CMD_SEC_COMA_08
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon09.svg"),  //IDB_CMD_SEC_COMB_01
// 		_T("SVG\\illustration\\Dialog\\SecComAIcon10.svg"),  //IDB_CMD_SEC_COMB_02
// 		_T("SVG\\illustration\\Dialog\\SecComBIcon03.svg"),  //IDB_CMD_SEC_COMB_03
// 		_T("SVG\\illustration\\Dialog\\SecComBIcon04.svg"),  //IDB_CMD_SEC_COMB_04
// 		_T("SVG\\illustration\\Dialog\\SecComBIcon05.svg"),  //IDB_CMD_SEC_COMB_05
// 		_T("SVG\\illustration\\Dialog\\SecComBIcon06.svg"),  //IDB_CMD_SEC_COMB_06
// 	};

	int nSectSizeA = CSectUtil::GetSizeCountCombinedAShape();
	int nSectSizeB = CSectUtil::GetSizeCountCombinedBShape();
	int nSectSize = nSectSizeA + nSectSizeB;

	CString* aImgPath   = new CString[nSectSize];
	UINT* aBitmapID		= new UINT[nSectSize];
	CString* aItemName	= new CString[nSectSize];
	DWORD* nIndex		= new DWORD[nSectSize];

	int nIdx = 0;
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon01.svg");  nIdx++;//IDB_CMD_SEC_COMA_01
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon02.svg");  nIdx++;//IDB_CMD_SEC_COMA_02
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon03.svg");  nIdx++;//IDB_CMD_SEC_COMA_03
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon04.svg");  nIdx++;//IDB_CMD_SEC_COMA_04
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon05.svg");  nIdx++;//IDB_CMD_SEC_COMA_05
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon06.svg");  nIdx++;//IDB_CMD_SEC_COMA_06
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon07.svg");  nIdx++;//IDB_CMD_SEC_COMA_07
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon08.svg");  nIdx++;//IDB_CMD_SEC_COMA_08
	if (nMovingType == D_PRODUCT_MOVING_US)
	{
		aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon08F.svg"); nIdx++; //IDB_CMD_SEC_COMA_08
	}
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon09.svg");  nIdx++;//IDB_CMD_SEC_COMB_01
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComAIcon10.svg");  nIdx++;//IDB_CMD_SEC_COMB_02
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComBIcon03.svg");  nIdx++;//IDB_CMD_SEC_COMB_03
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComBIcon04.svg");  nIdx++;//IDB_CMD_SEC_COMB_04
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComBIcon05.svg");  nIdx++;//IDB_CMD_SEC_COMB_05
	aImgPath[nIdx] = _T("SVG\\illustration\\Dialog\\SecComBIcon06.svg");  nIdx++;//IDB_CMD_SEC_COMB_06


	nIdx = 0;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_01;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__2H_Combined_Shape);					nIndex[nIdx++] = EN_SECT_COMA_2H;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_02;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__H_C_Combined_Shape);				nIndex[nIdx++] = EN_SECT_COMA_HC1;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_03;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__H_C_Combined_Shape2);				nIndex[nIdx++] = EN_SECT_COMA_HC2;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_04;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__H_T_Combined_Shape);				nIndex[nIdx++] = EN_SECT_COMA_HT;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_05;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_H_);					nIndex[nIdx++] = EN_SECT_COMA_2T1;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_06;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_H_2);					nIndex[nIdx++] = EN_SECT_COMA_2T2;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_07;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__H_Shape_with_2T_or_Web);			nIndex[nIdx++] = EN_SECT_COMA_H2T;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMA_08;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__Web_Opened_H_with_Stiffener);		nIndex[nIdx++] = EN_SECT_COMA_WOH;
	if ( nMovingType == D_PRODUCT_MOVING_US )
	{
		aBitmapID[nIdx] = IDB_CMD_SEC_COMA_09;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__Flange_Opened_H_with_Stiffener);nIndex[nIdx++] = EN_SECT_COMA_FOH;
	}
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_01;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate1);				nIndex[nIdx++] = EN_SECT_COMB_2CW1;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_02;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate2);				nIndex[nIdx++] = EN_SECT_COMB_2CW2;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_03;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__C_with_Web_Plate1);					nIndex[nIdx++] = EN_SECT_COMB_1CW1;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_04;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__C_with_Web_Plate2);					nIndex[nIdx++] = EN_SECT_COMB_1CW2;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_05;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__4_Angle);							nIndex[nIdx++] = EN_SECT_COMB_4L;
	aBitmapID[nIdx] = IDB_CMD_SEC_COMB_06;	aItemName[nIdx] = _LS(IDS_WG_CMD__ADDD__H_shape_with_Plate);				nIndex[nIdx++] = EN_SECT_COMB_HP;

	/*
	//if (CLocaleLib::IsEnglish())  // 영어권만(일본은 H로 씀)
	#if defined(_US)
	{
		aItemName[0] = _LS(IDS_WG_CMD__ADDD__2I_Combined_Shape);
		aItemName[1] = _LS(IDS_WG_CMD__ADDD__I_C_Combined_Shape);
		aItemName[2] = _LS(IDS_WG_CMD__ADDD__I_C_Combined_Shape2);
		aItemName[3] = _LS(IDS_WG_CMD__ADDD__I_T_Combined_Shape);
		aItemName[4] = _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_I_);
		aItemName[5] = _LS(IDS_WG_CMD__ADDD__2T_Web_Opened_I_2);
		aItemName[6] = _LS(IDS_WG_CMD__ADDD__I_Shape_with_2T_or_Web);
		aItemName[7] = _LS(IDS_WG_CMD__ADDD__Web_Opened_I_with_Stiffener);
		aItemName[8] = _LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate1);
		aItemName[9] = _LS(IDS_WG_CMD__ADDD__2C_with_Web_Plate2);
		aItemName[10] = _LS(IDS_WG_CMD__ADDD__C_with_Web_Plate1);
		aItemName[11] = _LS(IDS_WG_CMD__ADDD__C_with_Web_Plate2);
		aItemName[12] = _LS(IDS_WG_CMD__ADDD__4_Angle);
		aItemName[13] = _LS(IDS_WG_CMD__ADDD__I_shape_with_Plate);
	}
	#endif
	*/

	//int nCount = sizeof(aBitmapID)/sizeof(UINT);
	int nCount = nIdx;

	// build image list
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList= NULL; }
	m_pImageList = new CImageList;
	m_pImageList->Create(25, 25, ILC_COLOR, nCount, 1);

	// load image and add it to image list
	for (int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(aBitmapID[i]);
		m_pImageList->Add(&bitmap, COLORREF(RGB(0,0,0)));
	}

	// Attach image list to ComboBox
	//m_cboType.SetImageList(m_pImageList);
	m_cboType.SetImageList(aImgPath, nCount);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
	m_cboType.SetRedraw(FALSE);
	// Make items
	COMBOBOXEXITEM cbi;
	CString str;

	for (int i = 0; i < nCount; i++)
	{
		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_TEXT
							| CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE;
		cbi.iItem = i;
		str = aItemName[i];
		if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS) 
		{
			str.Replace('H', 'I');
		}
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		cbi.iOverlay = i;
		cbi.iIndent = 0;
		//m_cboType.InsertItem(&cbi);
		m_cboType.SetItemData(m_cboType.AddString(str.GetBuffer()), nIndex[i]);
		str.ReleaseBuffer();
	}

	delete[] aImgPath;
	delete[] aBitmapID;
	delete[] aItemName;
	delete[] nIndex;
	m_cboType.SetRedraw(TRUE);
}

void CCMSectItemCom::SetDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);
	m_wndDB.SetRedraw(FALSE);
	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(DBNameList[i]);
#endif
		m_wndDB.AddString(DBNameList.GetAt(i));
	}
	m_wndDB.SetRedraw(TRUE);
}

void CCMSectItemCom::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemCom::ShowData2Dlg()
{
	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameCom(m_Data.SectBefore.Shape);
	CDlgUtil::CobxSetCurSelItemData(m_cboType, nShapeIndex);
	//int nShapeIndex = CSectUtil::GetShapeIndex4SelFromNameCom(m_Data.SectBefore.Shape);
	//m_cboType.SetCurSel(nShapeIndex);
	// db
	SetDB(m_Data.SectBefore.SectI.DBName);
	// name(db보다 나중에 설정:For AutoChangeName)
	m_wndName.SetWindowText(m_Data.SName);
	// first, second
	SetFirstNameCombo();
	SetSecondNameCombo();
	SetFirstSectData();   // Title, Value 설정, Show/Hide 수행
	SetSecondSectData();  // Title, Value 설정, Show/Hide 수행

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
	if (!CSectDB::IsEnable7thDOF())
		m_chkWarpingEffect.ShowWindow(SW_HIDE);

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemCom::SetFirstNameCombo()
{
	// Name List를 삭제
	m_wndFirstName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	csDBName = pSect->DBName;
	csShape = pSect->Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndFirstName.EnableWindow(FALSE);
		return;
	}
	m_wndFirstName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	m_wndFirstName.SetRedraw(FALSE);
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndFirstName.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndFirstName.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndFirstName.SelectString(-1, SectName);
	else m_wndFirstName.SetCurSel(nSel);
	m_wndFirstName.SetRedraw(TRUE);
}

void CCMSectItemCom::SetSecondNameCombo()
{
	// Name List를 삭제
	m_wndSecondName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	csDBName = pSect->DBName;
	csShape = pSect->Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndSecondName.EnableWindow(FALSE);
		return;
	}
	m_wndSecondName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	m_wndSecondName.SetRedraw(FALSE);
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndSecondName.AddString(aSectNameList.GetAt(i));
	}

	CString SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndSecondName.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndSecondName.SelectString(-1, SectName);
	else m_wndSecondName.SetCurSel(nSel);
	m_wndSecondName.SetRedraw(TRUE);
}

void CCMSectItemCom::SetFirstSectData()
{
	CStringArray aSizeTitle;
	//nShapeIndex = CSectUtil::GetShapeIndex4SelFromNameCom(m_Data.SectBefore.Shape);
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameCom(m_Data.SectBefore.Shape);

	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_combined_section_));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexComFirst(nShapeIndex, aSizeTitle, FALSE);

	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	const BOOL bUser = (nUserOrDB == 0) ? TRUE : FALSE;

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	BOOL bEnable = bUser ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0;i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		if (i < 4) m_wndFirstSize[i].EnableWindow(bEnable);
		else m_wndFirstSize[i].EnableWindow(TRUE);  // enable always C
	}
	for (; i < 5; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMSectItemCom::SetSecondSectData()
{
	CStringArray aSizeTitle;
	//nShapeIndex = CSectUtil::GetShapeIndex4SelFromNameCom(m_Data.SectBefore.Shape);
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameCom(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalide_combined_section));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexComSecond(nShapeIndex, aSizeTitle, FALSE);

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	BOOL bEnable = (pSect->DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0;i < nSizeNum; i++)
	{
		m_wndSecondTitle[i].ShowWindow(SW_SHOW);
		m_wndSecondSize[i].ShowWindow(SW_SHOW);
		m_wndSecondUnit[i].ShowWindow(SW_SHOW);
		m_wndSecondTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndSecondSize[i].SetWindowText(csValue);
		m_wndSecondSize[i].EnableWindow(bEnable);
	}
	for (; i < 4; i++)
	{
		m_wndSecondTitle[i].ShowWindow(SW_HIDE);
		m_wndSecondSize[i].ShowWindow(SW_HIDE);
		m_wndSecondUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMSectItemCom::ChangeBitmap()
{
	//nShapeIndex = CSectUtil::GetShapeIndex4SelFromNameCom(m_Data.SectBefore.Shape);
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameCom(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_combined_section_));
		return;
	}

//	UINT aBitmapID[] = {
//		IDB_CMD_SP_ICOMA_BMP01, IDB_CMD_SP_ICOMA_BMP02, IDB_CMD_SP_ICOMA_BMP03,
//		IDB_CMD_SP_ICOMA_BMP04, IDB_CMD_SP_ICOMA_BMP05, IDB_CMD_SP_ICOMA_BMP06,
//		IDB_CMD_SP_ICOMA_BMP07, IDB_CMD_SP_ICOMA_BMP08, 
//		IDB_CMD_SP_ICOMB_BMP01, IDB_CMD_SP_ICOMB_BMP02, IDB_CMD_SP_ICOMB_BMP03,
//		IDB_CMD_SP_ICOMB_BMP04, IDB_CMD_SP_ICOMB_BMP05, IDB_CMD_SP_ICOMB_BMP06,
// 	    IDB_CMD_SP_ICOMA_BMP09,
//	};

	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\SecComA01.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA02.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA03.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA04.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA05.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA06.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA07.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA08.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB01.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB02.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB03.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB04.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB05.svg"),
		_T("SVG\\illustration\\Dialog\\SecComB06.svg"),
		_T("SVG\\illustration\\Dialog\\SecComA08F.svg"),
	};

	// set appropriate bitmap
	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
// 	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
// 	m_pBitmap = (CBitmap*)SecImage;
// 	m_pBitmap = new CBitmap;
// 	m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
// 	SecImage->SetBitmap(HBITMAP(*m_pBitmap));

	m_ImgSectItem.SetImage(aImgPath_Sect[nShapeIndex]);
}

void CCMSectItemCom::DoActionWhenDBChanged()
{
	CString csDB = GetDB();

	if (!csDB.IsEmpty() && !m_Data.SectBefore.SectI.DBName.IsEmpty() &&
		 csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectS = &m_Data.SectBefore.SectJ;

	// fisrt 초기화
	if (csDB != _T("")) 
	{
		pSectF->Initialize();
		// change new DB name
	  pSectF->DBName = csDB;
		pSectF->Shape = CSectUtil::GetFirstShapeFromComShape(m_Data.SectBefore.Shape);
	}
	else // DB가 None으로 변경되면 Size는 유지
	{
		pSectF->DBName = _T("");
		pSectF->SName = _T("");
	}
 
	// second 초기화
	if (csDB != _T("") && CSectUtil::IsCombinedAType(m_Data)) 
	{
		pSectS->Initialize();
		pSectS->DBName = csDB;
		pSectS->Shape = CSectUtil::GetSecondShapeFromComShape(m_Data.SectBefore.Shape);
	}
	else  // DB가 None으로 변경되면 Size는 유지
	{
		pSectS->DBName = _T("");
		pSectS->SName = _T("");
	}

	// Dialog에 반영한다.
	SetFirstNameCombo();
	SetSecondNameCombo();
	SetFirstSectData();
	SetSecondSectData();

	// Update preview
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

CString CCMSectItemCom::GetDB()
{
	CString csDB;
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) csDB = _T("");
	else m_wndDB.GetWindowText(csDB);

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif
	return csDB;
}

void CCMSectItemCom::SetDB(CString &csDBName)
{
	if (csDBName == _T("")) // DB None -> user
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
		m_wndDB.EnableWindow(FALSE);
	}
	else  // use DB
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		m_wndDB.EnableWindow(TRUE);

		CString csDB = csDBName;
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(csDB);
#endif

		int nSel = m_wndDB.FindStringExact(-1, csDB);
		if (nSel == CB_ERR) m_wndDB.SelectString(-1, csDB);
		else m_wndDB.SetCurSel(nSel);
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemCom, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemCom)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME1, OnChangeFirstName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME2, OnChangeSecondName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE11, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE21, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_USER_RADIO, OnChangeUseDB)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE12, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE13, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE14, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE15, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE22, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE23, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE24, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_DB_RADIO, OnChangeUseDB)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemCom message handlers
BOOL CCMSectItemCom::OnInitDialog() 
{
	CCMSectItemBase::OnInitDialog();
	
	SetRedraw(FALSE);

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_COMBINED;
		m_Data.SectBefore.Shape = D_SECT_COMBINED_DEF_SHAPE;
		T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;
		T_SECT_SECTBASE_D* pSectS = &m_Data.SectBefore.SectJ;

		pSectF->Shape = CSectUtil::GetFirstShapeFromComShape(m_Data.SectBefore.Shape);
		pSectS->Shape = CSectUtil::GetSecondShapeFromComShape(m_Data.SectBefore.Shape);

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSectF->DBName = pref.Property.SectionDBName;
		if (CSectUtil::IsCombinedAType(m_Data)) pSectS->DBName = pSectF->DBName;
		else pSectS->DBName = _T("");
	}
	m_wndSecView.SetDataSource(&m_Data);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);
	
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE	
}

void CCMSectItemCom::OnChangeShape() 
{
	// TODO: Add your control notification handler code here
	//int nShapeIndex1 = m_cboType.GetCurSel();
	int nShapeIndexInDlg = m_cboType.GetCurSel();
	int nShapeIndex = m_cboType.GetItemData(m_cboType.GetCurSel());
	if (nShapeIndex < 0) return;
	//int nCurShapeIndex = CSectUtil::GetShapeIndex4SelFromNameCom(m_Data.SectBefore.Shape);
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameCom(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexCom(nShapeIndex);

	T_SECT_SECTBASE_D *pSectF, *pSectS;
	pSectF = &m_Data.SectBefore.SectI;
	pSectS = &m_Data.SectBefore.SectJ;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameF = pSectF->DBName;
	CString csDBNameS;
	if (CSectUtil::IsCombinedAType(m_Data)) csDBNameS = csDBNameF;
	else csDBNameS = _T("");

	// First 초기화
	pSectF->Initialize();
	pSectF->DBName = csDBNameF;
	pSectF->Shape = CSectUtil::GetFirstShapeFromComShape(m_Data.SectBefore.Shape);

	// Second 초기화
	pSectS->Initialize();
	pSectS->DBName = csDBNameS;
	pSectS->Shape = CSectUtil::GetSecondShapeFromComShape(m_Data.SectBefore.Shape);

	// Dialog에 반영
	ShowData2Dlg();
}

void CCMSectItemCom::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	DoActionWhenDBChanged();
}

void CCMSectItemCom::OnChangeUseDB() 
{
	// TODO: Add your control notification handler code here
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) m_wndDB.EnableWindow(FALSE);
	else m_wndDB.EnableWindow(TRUE);

	// DB가 변경된 경우와 같다.
	DoActionWhenDBChanged();	
}

void CCMSectItemCom::OnChangeFirstName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndFirstName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	if (csName == pSect->SName) return;
	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 6; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetFirstSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemCom::OnChangeSecondName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndSecondName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectJ;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	if (csName == pSect->SName) return;

	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 6; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSecondSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}


void CCMSectItemCom::OnChangeFirstSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE11 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE12 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE13 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE14 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE15 : nIx = 4; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemCom::OnChangeSecondSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE21 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE22 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE23 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE24 : nIx = 3; break;
	default: ASSERT(0); break;
	}	
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	pSect->Size[nIx] = m_wndSecondSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemCom::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);	
}

void CCMSectItemCom::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		if (csDB == _T("")) m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}	
}

void CCMSectItemCom::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	//m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();		
}


void CCMSectItemCom::OnCmdChangeOffsetBtn() 
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
