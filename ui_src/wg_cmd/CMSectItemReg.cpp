#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemReg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_db\SectDBRussiaUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CMSectOffsetDlg.h"

// Default Shape
#if defined(_MGEN) || defined(_CIVIL)
	#define D_SECT_REGULAR_DEF_SHAPE D_SECT_SHAPE_REG_H
#elif defined(_TOWER)
	#define D_SECT_REGULAR_DEF_SHAPE D_SECT_SHAPE_REG_L
#else
	#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif

#define D_SECT_DB_KS _T("KS")

#define D_SECT_SHAPE_REG_CC_IS _T("IS")
#define D_SECT_SHAPE_REG_CC_IW _T("IW")
#define D_SECT_SHAPE_REG_CC_BS _T("BS")
#define D_SECT_SHAPE_REG_CC_BW _T("BW")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemReg dialog
#define   CDialog   CCMSectItemBase


CCMSectItemReg::CCMSectItemReg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBitmap = NULL;
	m_pImageList = NULL;

	m_aCtrlDblType.Add(IDC_CMD_SP_ID_DBL_TEXT);
	m_aCtrlDblType.Add(IDC_CMD_SP_ID_DBL_DB_TITLE);
	m_aCtrlDblType.Add(IDC_CMD_SP_ID_DBL_DB_COMBO);
	m_aCtrlDblType.Add(IDC_CMD_SP_ID_DBL_SECT_TITLE);
	m_aCtrlDblType.Add(IDC_CMD_SP_ID_DBL_SECT_COMBO);

	m_aCtrlDB.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_DB_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_CM_RADIO);

	m_aCtrlColdForm.Add(IDC_CMD_SP_ID_CC_TEXT);
	m_aCtrlColdForm.Add(IDC_CMD_SP_ID_CC_NUM_TEXT);
	m_aCtrlColdForm.Add(IDC_CMD_SP_ID_CC_NUM_COBX);
	m_aCtrlColdForm.Add(IDC_CMD_SP_ID_CC_SHAPE_TEXT);
	m_aCtrlColdForm.Add(IDC_CMD_SP_ID_CC_SHAPE_COBX);

	m_aCtrlROct.Add(IDC_CMD_TEXT_CELL_NUM);
	m_aCtrlROct.Add(IDC_CMD_EDIT_CELL_NUM);
// 	m_aCtrlROct.Add(IDC_CMD_SPIN_CELL_NUM);

	m_aCtrlStiff1.Add(IDC_CMD_SP_ID_TITLE_N1);
	m_aCtrlStiff1.Add(IDC_CMD_SP_ID_VALUE_N1);
	//m_aCtrlStiff1.Add(IDC_CMD_SP_ID_SPIN_N1);
	m_aCtrlStiff2.Add(IDC_CMD_SP_ID_TITLE_N2);
	m_aCtrlStiff2.Add(IDC_CMD_SP_ID_VALUE_N2);
// 	m_aCtrlStiff2.Add(IDC_CMD_SP_ID_SPIN_N2);

	m_aCtrlHSQ.Add(IDC_CMD_HSQ_TYPE_CHK);

	m_aCtrlRoundType.Add(IDC_CMD_ROUND_TYPE_CHK);

	m_aCtrlPrincipalAxis.Add(IDC_CMD_PRINCIPAL_AXIS_CHK);

	m_aCtrlCommDB.Add(IDC_CMD_SP_ID_CM_RADIO);
	m_aCtrlCommDB.Add(IDC_CMD_SP_ID_CODE2);
	m_bShowCommercialDB = TRUE;
	m_nDistCommercial = 0;

}

CCMSectItemReg::~CCMSectItemReg()
{
// 	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	if (m_pImageList != NULL) { delete m_pImageList;  m_pImageList = NULL;  }
}

void CCMSectItemReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemReg)
	DDX_Control(pDX, IDC_CMD_SP_ID_VALUE_N2, m_editN2);
	DDX_Control(pDX, IDC_CMD_SP_ID_VALUE_N1, m_editN1);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_EDIT_CELL_NUM, m_editCellNum);
	DDX_Control(pDX, IDC_CMD_SP_ID_DBL_TEXT, m_wndDblTitle);
	DDX_Control(pDX, IDC_CMD_SP_ID_DBL_SECT_COMBO, m_wndDblSect);
	DDX_Control(pDX, IDC_CMD_SP_ID_DBL_DB_COMBO, m_wndDblDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_BUILTUP, m_wndBuiltUp);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE2, m_wndCommecialDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SP_ID_CC_NUM_COBX, m_wndCCNum);
	DDX_Control(pDX, IDC_CMD_SP_ID_CC_SHAPE_COBX, m_wndCCShape);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_HSQ_TYPE_CHK, m_wndHSQType);
	DDX_Control(pDX, IDC_CMD_SP_ID_BMP, m_ImgSectItem);
	DDX_Control(pDX, IDC_CMD_ROUND_TYPE_CHK, m_wndRoundType);
	DDX_Control(pDX, IDC_CMD_PRINCIPAL_AXIS_CHK, m_wndPrincipalAxis);
	//}}AFX_DATA_MAP

	UINT aFirstID[][11] = {
		{ IDC_CMD_SP_ID_TITLE1, IDC_CMD_SP_ID_TITLE2, IDC_CMD_SP_ID_TITLE3,
			IDC_CMD_SP_ID_TITLE4, IDC_CMD_SP_ID_TITLE5, IDC_CMD_SP_ID_TITLE6,
			IDC_CMD_SP_ID_TITLE7, IDC_CMD_SP_ID_TITLE8, IDC_CMD_SP_ID_TITLE9, 
			IDC_CMD_SP_ID_TITLE10, IDC_CMD_SP_ID_TITLE11 },
		{ IDC_CMD_SP_ID_VALUE1, IDC_CMD_SP_ID_VALUE2, IDC_CMD_SP_ID_VALUE3,
			IDC_CMD_SP_ID_VALUE4, IDC_CMD_SP_ID_VALUE5, IDC_CMD_SP_ID_VALUE6,
			IDC_CMD_SP_ID_VALUE7, IDC_CMD_SP_ID_VALUE8, IDC_CMD_SP_ID_VALUE9,
			IDC_CMD_SP_ID_VALUE10, IDC_CMD_SP_ID_VALUE11 },
		{ IDC_CMD_SP_ID_UNIT1,  IDC_CMD_SP_ID_UNIT2,  IDC_CMD_SP_ID_UNIT3,
			IDC_CMD_SP_ID_UNIT4,  IDC_CMD_SP_ID_UNIT5,  IDC_CMD_SP_ID_UNIT6,
			IDC_CMD_SP_ID_UNIT7,  IDC_CMD_SP_ID_UNIT8,  IDC_CMD_SP_ID_UNIT9,
			IDC_CMD_SP_ID_UNIT10, IDC_CMD_SP_ID_UNIT11 },
	};
	for (int i = 0; i < 11; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출
BOOL CCMSectItemReg::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출
BOOL CCMSectItemReg::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);

		ShowKey2Dlg();
		m_wndFirstName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemReg::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	CCMStiffDlg dlg(m_pDoc, &m_Data, NULL, this);
	dlg.DoModal();
}

void CCMSectItemReg::DisplayOffsetPoint()
{
	m_wndSecView.Invalidate();
}

void CCMSectItemReg::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemReg::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_Data.SectBefore.SectI.DBName == _T("POSCO"))
	{
		if(m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_H)
			IUsageCounter::Use(_T("POSCO_H"));
		else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_B)
			IUsageCounter::Use(_T("POSCO_B"));
		
	}
	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemReg::Dlg2Data()
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

	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_REGULAR;

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
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF)
	{
		m_Data.SectBefore.nCellShape = m_editN1.GetEditValueInt();
		m_Data.SectBefore.nCellType = m_editN2.GetEditValueInt();
	}
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF)
	{
		m_Data.SectBefore.nCellShape = m_editN1.GetEditValueInt();
	}

	//----------------------------------------
	// HSQ type
	BOOL bUser = FALSE;
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB == EN_USER) bUser = TRUE;

	BOOL bHSQ = FALSE;
	if (bUser && m_wndHSQType.GetCheck() == 1)
	{
		bHSQ = CDBLib::ActivateHSQ();
	}

	m_Data.bHSQType = bHSQ;

	BOOL bRoundType = FALSE;
	if ( bUser && m_wndRoundType.GetCheck() == 1 )
	{
		bRoundType = TRUE;
	}

	m_Data.bRoundType = bRoundType;
	//----------------------------------------

	BOOL brincipalAxis = FALSE;
	if (bUser && m_wndPrincipalAxis.GetCheck() == 1) brincipalAxis = TRUE;
	const int nMovingType = CProduct::GetMovingType();
	if( nMovingType != D_PRODUCT_MOVING_US ) brincipalAxis = FALSE;

	m_Data.bPricipalAxis = brincipalAxis;
	//----------------------------------------
		

	// Built-Up
	if (m_wndBuiltUp.GetCheck() == 1)
		m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
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

		// Shape : 없음
		pSectF->Shape = _T("");
		// SName : 
		if (pSectF->DBName == _T("")) pSectF->SName = _T("");
		else m_wndFirstName.GetWindowText(pSectF->SName);

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSectF->DBName, pSectF->SName);
#endif

		// Size
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
		if (bHSQ) nSizeNum = 9;   // MNET:XXXX-HSSHIM-20130428. HSQ 타입인 경우는 파라미터를 3개 더 받음 (bHSQ, B1, B2). 다나가 갖다 쓰는 함수 활인해서 여기도 고쳐야 될 듯...
		if (bRoundType) nSizeNum = 11;
		if (brincipalAxis) nSizeNum = 6;
		for (int i = 0; i < nSizeNum; i++)
			pSectF->Size[i] = m_wndFirstSize[i].GetEditValue();

		if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT)
		{
			m_Data.SectBefore.nCellShape = m_editCellNum.GetEditValueInt();
		}
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// cc 관련
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC)
		m_Data.SectBefore.SectI.Shape = GetCurCCShape();

	return TRUE;
}

BOOL CCMSectItemReg::GetPeriAndStiff()
{
	int nUserDB=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	return CSectUtil::CalcSectData(m_Data, TRUE, nUserDB==EN_USER);
}

void CCMSectItemReg::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init DB name and section name combo
	SetDBNameList();
	SetCommercialDBNameList();
	m_wndDB.SetCurSel(0);
	m_wndDblDB.SetCurSel(0);
	m_wndCommecialDB.SetCurSel(0);
	m_wndFirstName.SetActionWhenKillFocus(2, -1);
	m_wndDblSect.SetActionWhenKillFocus(2, -1);

	m_editCellNum.SetRange(1, 4);
	m_editCellNum.SetInteger(TRUE);

	m_editN2.SetRange(1, 100);
	m_editN2.SetInteger(TRUE);
	m_editN1.SetRange(1, 100);
	m_editN1.SetInteger(TRUE);

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	for (int i = 0; i < 10; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	m_wndBuiltUp.EnableWindow(FALSE);
	
	// cc 관련 컨트롤의 위치 이동 및 Hide
	int nDistY;
	CRect rTgt, rSrc;
	CWnd* pTgtWnd = GetDlgItem(m_aCtrlDblType[0]);
	pTgtWnd->GetWindowRect(rTgt);
	CWnd* pSrcWnd = GetDlgItem(m_aCtrlColdForm[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlColdForm, nDistY, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlColdForm, FALSE);
	m_wndCCNum.AddString(_T("1"));
	m_wndCCNum.AddString(_T("2"));
	m_wndCCNum.AddString(_T("3"));
	m_wndCCNum.AddString(_T("4"));
	
	// stiffener 개수 관련 컨트롤 위치 이동
	pTgtWnd = GetDlgItem(IDC_CMD_EDIT_CELL_NUM);
	pTgtWnd->GetWindowRect(rTgt);
	pSrcWnd = GetDlgItem(m_aCtrlStiff1[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlStiff1, nDistY, TRUE);
	pSrcWnd = GetDlgItem(m_aCtrlStiff2[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlStiff2, nDistY, TRUE);

	// HSQ Type 관련 컨트롤 위치 이동 
	pTgtWnd = GetDlgItem(IDC_CMD_SP_ID_TITLE7);
	pTgtWnd->GetWindowRect(rTgt);
	pSrcWnd = GetDlgItem(m_aCtrlHSQ[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlHSQ, nDistY, TRUE);

	// Round Type 관련 컨트롤 위치 이동
	pTgtWnd = GetDlgItem(IDC_CMD_SP_ID_TITLE10);
	pTgtWnd->GetWindowRect(rTgt);
	pSrcWnd = GetDlgItem(m_aCtrlRoundType[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlRoundType, nDistY, TRUE);

	// Principal Axis 관련 컨트롤 위치 이동 
	pTgtWnd = GetDlgItem(IDC_CMD_SP_ID_TITLE5);
	pTgtWnd->GetWindowRect(rTgt);
	pSrcWnd = GetDlgItem(m_aCtrlPrincipalAxis[0]);
	pSrcWnd->GetWindowRect(rSrc);
	nDistY = rTgt.top - rSrc.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlPrincipalAxis, nDistY, TRUE);

	

	CArray<UINT, UINT> NameCtrl, DBNameCtrl, SizeCtrl;
	CDlgUtil::GetCtrlIDByIncRect(this, NameCtrl, IDC_WG_CMD_STATIC2, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, DBNameCtrl, IDC_WG_CMD_STATIC4, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, SizeCtrl, IDC_WG_CMD_STATIC5, TRUE);
	m_aCtrlMoveByCommDB.Append(NameCtrl);
	m_aCtrlMoveByCommDB.Append(DBNameCtrl);
	m_aCtrlMoveByCommDB.Append(SizeCtrl);
	m_aCtrlMoveByCommDB.Add(IDC_CMD_SHEAR_DEFORM);
	m_aCtrlMoveByCommDB.Add(IDC_CMD_WARPING_EFFECT_CHK);
	m_aCtrlMoveByCommDB.Add(IDC_WG_CMD_STATIC9);
	m_aCtrlMoveByCommDB.Add(IDC_CMD_OFFSET_TEXT);
	m_aCtrlMoveByCommDB.Add(IDC_CMD_CHANGE_OFFSET_BTN);

	CWnd* pTgtWnd1 = GetDlgItem(IDC_CMD_SP_ID_DB_RADIO);
	pTgtWnd1->GetWindowRect(rTgt);
	CWnd* pSrcWnd2 = GetDlgItem(IDC_CMD_SP_ID_CM_RADIO);
	pSrcWnd2->GetWindowRect(rSrc);

	m_nDistCommercial = rTgt.top - rSrc.top;
}

void CCMSectItemReg::SetSectionShapeCombo()
{
	const auto bEnableJUD = CDBDoc::EnableJUD();
	const int nMovingType = CProduct::GetMovingType();
	const int nCount = bEnableJUD ? m_SectItemRes.aBitmapID_JUD.size() : m_SectItemRes.aBitmapID.size();

	// build image list
	if (m_pImageList != NULL) { delete m_pImageList;  m_pImageList = NULL;  }
	m_pImageList = new CImageList;
	m_pImageList->Create(25, 25, ILC_COLOR, nCount, 1);

	// load image and add it to image list
	for (int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		if ( bEnableJUD ) bitmap.LoadBitmap(m_SectItemRes.aBitmapID_JUD[i]);
		else              bitmap.LoadBitmap(m_SectItemRes.aBitmapID[i]);
		m_pImageList->Add(&bitmap, COLORREF(RGB(0,0,0)));
	}

	// Attach image list to ComboBox
	m_cboType.SetImageList(m_SectItemRes.aImgPath.data(), nCount);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
	// Make items
	CString str;
	for (int i = 0; i < nCount; i++)
	{
		if ( bEnableJUD )  str = m_SectItemRes.aItemName_JUD[i];
		else               str = m_SectItemRes.aItemName[i];

		if (nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS) 
		{
			if ( bEnableJUD )
			{
				if (m_SectItemRes.nIndex_JUD[i] != 16 && m_SectItemRes.nIndex_JUD[i] != 26 ) str.Replace('H', 'I');
			}
			else
			{
				if (m_SectItemRes.nIndex[i] != 16 && m_SectItemRes.nIndex[i] != 26 ) str.Replace('H', 'I');
			}
		}

		if ( bEnableJUD ) m_cboType.SetItemData(m_cboType.AddString(str.GetBuffer()), m_SectItemRes.nIndex_JUD[i]);
		else              m_cboType.SetItemData(m_cboType.AddString(str.GetBuffer()), m_SectItemRes.nIndex[i]);
		str.ReleaseBuffer();
	}
}

void CCMSectItemReg::SetDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);

	m_wndDB.InitStorage(DBNameList.GetSize(), m_SectItemRes.MaxByte);
	m_wndDblDB.InitStorage(DBNameList.GetSize(), m_SectItemRes.MaxByte);
	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
		auto name = DBNameList.GetAt(i);
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(name);
#endif
		m_wndDB.AddString(name);
		m_wndDblDB.AddString(std::move(name));
	}
}

void CCMSectItemReg::SetCommercialDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetCommercialDBNameList(DBNameList);
	m_wndCommecialDB.InitStorage(DBNameList.GetSize(), m_SectItemRes.MaxByte);
	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
		m_wndCommecialDB.AddString(DBNameList.GetAt(i));
	}
}
void CCMSectItemReg::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemReg::ChangeCellNumber(int nCellNum)
{
	// change bitmap
	ChangeBitmap();

	// enable/disable
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB == EN_USER) // user type 일때만
	{
		m_wndFirstSize[6].EnableWindow(nCellNum != 1);
		if (nCellNum == 1)
		{
			m_Data.SectBefore.SectI.Size[6] = 0.;
			m_wndFirstSize[6].SetEditUnit(0.);
		}
	}
}

// Shape 변화에 따른 추가 입력 사항 : ROCT, BSTF, PSTF
void CCMSectItemReg::ShowHideControlsByShape(CString &Shape)
{
	BOOL bShowCtrl;

	// cell number
	bShowCtrl = (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT);
	CDlgUtil::CtrlShowHide(this, m_aCtrlROct, bShowCtrl);
	if (bShowCtrl)
	{
		CString strCellNum(_T(""));
		strCellNum.Format(_T("%d"), m_Data.SectBefore.nCellShape);
		m_editCellNum.SetWindowText(strCellNum);
	}

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

	// stiffener number 1
	bShowCtrl = (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF || 
							 m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStiff1, bShowCtrl);
	if (bShowCtrl)
	{
		CString strStiffener(_T(""));
		strStiffener.Format(_T("%d"), m_Data.SectBefore.nCellShape);
		m_editN1.SetWindowText(strStiffener);
		if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF) 
			GetDlgItem(IDC_CMD_SP_ID_TITLE_N1)->SetWindowText(_T("N"));
		else
			GetDlgItem(IDC_CMD_SP_ID_TITLE_N1)->SetWindowText(_T("N1"));
	}

	// stiffener number 2
	bShowCtrl = (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF);
	CDlgUtil::CtrlShowHide(this, m_aCtrlStiff2, bShowCtrl);
	if (bShowCtrl)
	{
		CString strStiffener(_T(""));
		strStiffener.Format(_T("%d"), m_Data.SectBefore.nCellType);
		m_editN2.SetWindowText(strStiffener);
	}
}

void CCMSectItemReg::ShowData2Dlg()
{
	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	SetCurShapeByIndex(nShapeIndex);
	
	// show/hide  
	ShowHideControlsByShape(m_Data.SectBefore.Shape);

	// DB
	BOOL bStiffShape = (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF ||
											m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF);
	if (bStiffShape) m_Data.SectBefore.SectI.DBName = _T("");
	GetDlgItem(IDC_CMD_SP_ID_DB_RADIO)->EnableWindow(!bStiffShape);

	SetDB(m_Data.SectBefore.SectI.DBName);

	// name(DB보다 늦게 설정되어야 한다.)
	m_wndName.SetWindowText(m_Data.SName);

	// first, second
	SetFirstNameCombo();
	SetBuiltUp();
	SetFirstSectData();   // Title, Value 설정, Show/Hide 수행
	SetDoubleTypeSectCtrl();  // control for double angle / double channel

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

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	m_wndSecView.Invalidate();

	// cc 관련 cobx의 내용 변경 
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC)
	{
		CString csShape = m_Data.SectBefore.SectI.Shape;
		if (csShape.IsEmpty()) 
		{ 
			m_wndCCNum.SetCurSel(0);
			SetCCShapeCobx(1);
		}
		else 
		{
			int nCCNum;
			nCCNum = _ttoi(csShape);
			ASSERT(nCCNum >= 2 && nCCNum <=4 );

			m_wndCCNum.SetCurSel(nCCNum-1);
			SetCCShapeCobx(nCCNum);

			csShape = csShape.Mid(1);
			m_wndCCShape.SelectString(-1, csShape);
			//m_wndCCShape.SetWindowText(csShape);
		}
	}

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CCH)
	{
		GetDlgItem(IDC_CMD_SP_ID_DB_RADIO)->EnableWindow(FALSE);
		CButton* pButton = (CButton*)GetDlgItem(IDC_CMD_SP_ID_USER_RADIO);
		if ( pButton != NULL )
		{
			pButton->SetCheck(BST_CHECKED);
			OnChangeUseDB();
		}
	}

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_2CB)
	{
		GetDlgItem(IDC_CMD_SP_ID_DB_RADIO)->EnableWindow(FALSE);
		CButton* pButton = (CButton*)GetDlgItem(IDC_CMD_SP_ID_USER_RADIO);
		if ( pButton != NULL )
		{
			pButton->SetCheck(BST_CHECKED);
			OnChangeUseDB();
		}
	}
}

void CCMSectItemReg::SetFirstNameCombo()
{
	// Name List를 삭제
	m_wndFirstName.ResetContent();

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	const auto& csDBName = pSect->DBName;
	const auto& csShape = m_Data.SectBefore.Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndFirstName.EnableWindow(FALSE);
		return;
	}

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);
	
	m_wndFirstName.EnableWindow(TRUE);
	m_wndFirstName.InitStorage(aSectNameList.GetSize(), m_SectItemRes.MaxByte);
	CString SectName;
	// Section name list를 설정한다.
	for (int i = 0; i < aSectNameList.GetSize(); i++)
	{
		SectName = aSectNameList.GetAt(i);
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName,SectName);
#endif
		m_wndFirstName.AddString(SectName);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_wndFirstName);

	SectName = pSect->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectName);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndFirstName.FindStringExact(-1, SectName);
	if (nSel == CB_ERR) m_wndFirstName.SelectString(-1, SectName);
	else m_wndFirstName.SetCurSel(nSel);
}

void CCMSectItemReg::SetFirstSectData()
{
	const int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	const BOOL bUser = (nUserOrDB == EN_USER) ? TRUE : FALSE;

	CStringArray aSizeTitle;
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle, bUser);

	const auto pSect = &m_Data.SectBefore.SectI;
	const BOOL bEnable = bUser ? TRUE : FALSE;

	CString csValue;
	const int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i=0; i<nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSect->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		m_wndFirstSize[i].EnableWindow(bEnable);
	}
	for (; i<SectItemReg::firstSectNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);
	}
	// 임시(KJH20040119) => JIS, JIS2K round(r1, r2)
	// User type && Channel이면 r1, r2는 disable
	if(nUserOrDB==0 && nShapeIndex==1)
	{
		pSect->Size[6] = 0.;
		pSect->Size[7] = 0.;
		m_wndFirstSize[6].SetEditUnit(0.);
		m_wndFirstSize[7].SetEditUnit(0.);
		m_wndFirstSize[6].EnableWindow(FALSE);
		m_wndFirstSize[7].EnableWindow(FALSE);
	}

	if (nUserOrDB == 0 && nShapeIndex == 18)  // ROCT
	{
		m_wndFirstSize[6].EnableWindow(m_Data.SectBefore.nCellShape != 1);
		if (m_Data.SectBefore.nCellShape == 1)
		{
			pSect->Size[6] = 0.;
			m_wndFirstSize[6].SetEditUnit(0.);
		}
	}

	// SWD-2067
	//const bool bGen_US = CProduct::GetMovingType() == D_PRODUCT_MOVING_US ? true : false;
	bool bGen_US = FALSE;

#ifdef _MGEN_US
	bGen_US = TRUE;
#endif

	BOOL bUseHSQ = FALSE;
	bool bUseRoundType = false;
	if (bUser && nShapeIndex == 4)
	{
		bUseHSQ = CDBLib::ActivateHSQ();  // BOX & User
		bUseRoundType = bGen_US;
	}

	int nCheckHSQ = (m_Data.bHSQType)? 1 : 0;
	if (bUseHSQ) m_wndHSQType.SetCheck(nCheckHSQ);

	m_wndHSQType.ShowWindow(bUseHSQ);
	if (bUseHSQ)
	{
		pSect->Size[6] = 0.;
		m_wndFirstTitle[6].ShowWindow(!bUseHSQ);
		m_wndFirstSize[6].SetEditUnit(0.);
		m_wndFirstSize[6].ShowWindow(!bUseHSQ);
		m_wndFirstUnit[6].ShowWindow(!bUseHSQ);
	}

	int nCheckRoundType = (m_Data.bRoundType) ? 1 : 0;
	if ( bUseRoundType ) m_wndRoundType.SetCheck(nCheckRoundType);

	m_wndRoundType.ShowWindow(bUseRoundType);
	if ( bUseRoundType )
	{
		pSect->Size[9] = 0.0;
		m_wndFirstTitle[9].ShowWindow(!bUseRoundType);
		m_wndFirstSize[9].SetEditUnit(0.0);
		m_wndFirstSize[9].ShowWindow(!bUseRoundType);
		m_wndFirstUnit[9].ShowWindow(!bUseRoundType);
	}

	//------------------------------------------------------------------

	// SWD-2067
	bGen_US = CProduct::GetMovingType() == D_PRODUCT_MOVING_US ? true : false;

	BOOL bUsePrincipalAxis = FALSE;
	if ( bUser && nShapeIndex == 0 && bGen_US )
	{
		bUsePrincipalAxis = TRUE;  // L형강
	}

	int nCheckPricipalAxis = (m_Data.bPricipalAxis) ? 1 : 0;
	if (bUsePrincipalAxis) m_wndPrincipalAxis.SetCheck(nCheckPricipalAxis);

	m_wndPrincipalAxis.ShowWindow(bUsePrincipalAxis);
	if (bUsePrincipalAxis)
	{
		pSect->Size[4] = 0.;
		m_wndFirstTitle[4].ShowWindow(!bUsePrincipalAxis);
		m_wndFirstSize[4].SetEditUnit(0.);
		m_wndFirstSize[4].ShowWindow(!bUsePrincipalAxis);
		m_wndFirstUnit[4].ShowWindow(!bUsePrincipalAxis);

		m_wndFirstSize[5].EnableWindow(nCheckPricipalAxis);
	}

	if(m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_Z || bUsePrincipalAxis)
	{
		m_wndFirstSize[5].SetUnitType(D_UNITSYS_NONE);
		m_wndFirstUnit[5].SetUnitType(D_UNITSYS_BASE_DEGREE);
	}
	else
	{
		m_wndFirstSize[5].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[5].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	OnChangeHSQType();
	OnChangeRoundType();
}

void CCMSectItemReg::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	// set appropriate bitmap
// 	if (m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
// 	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
// 	m_pBitmap = (CBitmap*)SecImage;
// 	m_pBitmap = new CBitmap;
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT) // ROCT Shape
	{
		CString csCellNum;
		m_editCellNum.GetWindowText(csCellNum);
		CString csImagePath = (_ttoi(csCellNum) == 1) ? _T("SVG\\illustration\\Dialog\\Sec21.svg") : _T("SVG\\illustration\\Dialog\\Sec22.svg");
		m_ImgSectItem.SetImage(csImagePath);
	}

	if ( m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_B )
	{
		int nUserDB;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
		const bool bUser = nUserDB == EN_USER ? true : false;
		CString csImagePath = (bUser && m_wndRoundType.GetCheck()) ? _T("SVG\\illustration\\Dialog\\Sec05R.svg") : _T("SVG\\illustration\\Dialog\\Sec05.svg");
		m_ImgSectItem.SetImage(csImagePath);
	}

	else m_ImgSectItem.SetImage(m_SectItemRes.aImgPath_Sect[nShapeIndex]);
// 	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

// control for double angle / double channel
void CCMSectItemReg::SetDoubleTypeSectCtrl()
{
	CString csTitle;
	CString csShape;
	CString csDB;
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_2L || 
			m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CL  )
	{
		csTitle = _LS(IDS_WG_CMD__ADDD__Get_Data_from_Single_Angle);
		// Set title name
		m_wndDblTitle.SetWindowText(csTitle);
		csShape = D_SECT_SHAPE_REG_L;    
	}
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_2C)
	{
		csTitle = _LS(IDS_WG_CMD__ADDD__Get_Data_from_Single_Channel);
		// Set title name
		m_wndDblTitle.SetWindowText(csTitle);
		csShape = D_SECT_SHAPE_REG_C;
	}
	else if ( m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_2CB )
	{
		csTitle = _LS(IDS_WG_CMD__ADDD__Get_Data_from_Single_Channel);
		// Set title name
		m_wndDblTitle.SetWindowText(csTitle);
		csShape = D_SECT_SHAPE_REG_C;
	}
	else
	{
		// Disable controls
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDblType, FALSE);

		// cc 관련 control 조정
		if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC || m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CCH)
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlDblType, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlColdForm, TRUE);

			if ( m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC )
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrlColdForm, TRUE);
				m_wndCCNum.SetCurSel(0);
				SetCCShapeCobx(1);
			}
			else if ( m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CCH )
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrlColdForm, FALSE);
			}
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlDblType, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlColdForm, FALSE);
		}

		return;
	}
	CDlgUtil::CtrlShowHide(this, m_aCtrlDblType, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlColdForm, FALSE);
	if (m_Data.SectBefore.SectI.DBName != _T("")) // DB none이 아니면 return
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDblType, FALSE);
		return;
	}

	// Enable controls
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDblType, TRUE);

	// Get DB Name
	m_wndDblDB.GetWindowText(csDB);

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDB, csShape, aSectNameList);

	// Section name list를 설정한다.
	BOOL bRemoveUC = (csDB==_T("GB-YB05") && csShape==_T("C"));
	m_wndDblSect.ResetContent();
	m_wndDblSect.InitStorage(aSectNameList.GetSize(), m_SectItemRes.MaxByte);

	int nSize = aSectNameList.GetSize();;
	for (int i = 0; i < nSize; i++)
	{
		CString strSectName = aSectNameList.GetAt(i);
		if(bRemoveUC)
		{
			T_SECT_SECTBASE_D SectData;
			m_pDoc->m_pSectDB->GetSectData(csDB, strSectName, SectData);
			if(SectData.Size[1]!=SectData.Size[4]) continue;
		}

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDB, strSectName);
#endif
		m_wndDblSect.AddString(strSectName);  
	}
}

void CCMSectItemReg::SetBuiltUp()
{
	// Built-Up Setting
	/*  // Always Disabled
	if (m_Data.SectBefore.SectI.DBName == _T("")) 
		m_wndBuiltUp.EnableWindow(TRUE);
	else m_wndBuiltUp.EnableWindow(FALSE);
	*/
	if (m_Data.SectBefore.SectI.BuiltUpFlag == 2) m_wndBuiltUp.SetCheck(0);
	else m_wndBuiltUp.SetCheck(1);
}

void CCMSectItemReg::DoActionWhenDBChanged()
{
	CString csDB;

	// *^^* DB와 None이 라디오로 분리.
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);

	if (nUserOrDB == EN_USER) csDB = _T("");
	else if(nUserOrDB==EN_DB) m_wndDB.GetWindowText(csDB);
	else if(nUserOrDB==EN_COMMERCIAL) m_wndCommecialDB.GetWindowText(csDB);
	else { ASSERT(0);  m_wndDB.GetWindowText(csDB); }

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif

	if (!csDB.IsEmpty() && !m_Data.SectBefore.SectI.DBName.IsEmpty() &&
		csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;

	// Built-Up 초기화
	if (csDB == _T("")) m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // Built-Up
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // Rolled

	// fisrt 초기화
	if (csDB != _T("")) 
	{
		pSectF->Initialize();
		// change new DB name
	  pSectF->DBName = csDB;
	}
	else // DB가 None으로 변경되면 Size는 유지
	{
		pSectF->DBName = _T("");
		pSectF->SName = _T("");
	}
 
	// Dialog에 반영한다.
	SetFirstNameCombo();
	SetBuiltUp();
	SetFirstSectData();
	SetDoubleTypeSectCtrl();

	// Update preview
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

CString CCMSectItemReg::GetDB()
{
	CString csDB;
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == EN_USER) csDB = _T("");
	else if (nUserOrDB==EN_DB) m_wndDB.GetWindowText(csDB);
	else if (nUserOrDB==EN_COMMERCIAL) m_wndCommecialDB.GetWindowText(csDB);
	else { ASSERT(0);  m_wndDB.GetWindowText(csDB); }

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif
	return csDB;
}

void CCMSectItemReg::SetDB(CString &csDBName)
{

	CString DBName = csDBName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBEngToRus(DBName);
#endif

	BOOL bEnableDB=FALSE, bEnableComm=FALSE;
	EN_DB_CASE nDBCase = EN_USER;

	int nSelDB = m_wndDB.FindStringExact(-1, DBName);
	int nSelCO = m_wndCommecialDB.FindStringExact(-1, DBName);

	if (csDBName == _T("")) nDBCase = EN_USER;
	else if (nSelDB != CB_ERR) { nDBCase = EN_DB; bEnableDB = TRUE; m_wndDB.SetCurSel(nSelDB); m_wndCommecialDB.SelectString(-1, DBName);	}
	else if (nSelCO != CB_ERR) { nDBCase = EN_COMMERCIAL; bEnableComm = TRUE; m_wndCommecialDB.SetCurSel(nSelCO); m_wndDB.SelectString(-1, DBName); }
	else { ASSERT(0); }

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, static_cast<int>(nDBCase));

	m_wndDB.EnableWindow(bEnableDB);
	m_wndCommecialDB.EnableWindow(bEnableComm);

// 	if (csDBName == _T("")) // DB None -> user
// 	{
// 		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
// 		m_wndDB.EnableWindow(FALSE);
// 	}
// 	else  // use DB
// 	{
// 		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
// 		m_wndDB.EnableWindow(TRUE);
// 		int nSel = m_wndDB.FindStringExact(-1, csDBName);
// 		if (nSel == CB_ERR)
// 		{
// 			 m_wndDB.SelectString(-1, csDBName);
// 		}
// 		else m_wndDB.SetCurSel(nSel);
// 	}
}

int CCMSectItemReg::GetCurShapeIndex()
{
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) return LB_ERR;

	nShapeIndex = (int)m_cboType.GetItemData(nShapeIndex);
	return nShapeIndex;
}

void CCMSectItemReg::SetCurShapeByIndex(int nShapeIndex)
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

BEGIN_MESSAGE_MAP(CCMSectItemReg, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemReg)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME, OnChangeFirstName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_DBL_DB_COMBO, OnChangeDblDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_DBL_SECT_COMBO, OnChangeDblSectName)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE1, OnChangeFirstSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BUILTUP, OnChangeBuiltUp)
	ON_BN_CLICKED(IDC_CMD_SP_ID_USER_RADIO, OnChangeUseDB)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CC_NUM_COBX, OnSelchangeCmdSpIdCcNumCobx)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CC_SHAPE_COBX, OnSelchangeCmdSpIdCcShapeCobx)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_EDIT_CELL_NUM, OnChangeCmdEditCellNum)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SPIN_CELL_NUM, OnDeltaposCmdSpinCellNum)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE2, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE3, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE4, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE5, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE6, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE7, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE8, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE9, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE10, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE11, OnChangeFirstSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_DB_RADIO, OnChangeUseDB)
	ON_BN_CLICKED(IDC_CMD_SP_ID_CM_RADIO, OnChangeUseDB)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE_N1, OnChangeCmdSpIdValueN1)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE_N2, OnChangeCmdSpIdValueN2)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SP_ID_SPIN_N1, OnDeltaposCmdSpIdSpinN1)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SP_ID_SPIN_N2, OnDeltaposCmdSpIdSpinN2)
	ON_BN_CLICKED(IDC_CMD_HSQ_TYPE_CHK, OnChangeHSQType)
	ON_BN_CLICKED(IDC_CMD_ROUND_TYPE_CHK, OnChangeRoundType)
	ON_BN_CLICKED(IDC_CMD_PRINCIPAL_AXIS_CHK, OnChangePrincipalAxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemReg message handlers

BOOL CCMSectItemReg::OnInitDialog() 
{
	SetRedraw(FALSE);
	CDialog::OnInitDialog();

	m_bShowCommercialDB = TRUE;
	m_nDistCommercial = 0;
	m_aCtrlMoveByCommDB.RemoveAll();
	// TODO: Add extra initialization here

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.
	
	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		// WorkTab에서 call하면 undefined 된 id...
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_REGULAR;
		m_Data.SectBefore.Shape = D_SECT_REGULAR_DEF_SHAPE;
		T_SECT_SECTBASE_D* pSectF = &m_Data.SectBefore.SectI;

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSectF->DBName = pref.Property.SectionDBName;
	 
	}
	m_wndSecView.SetDataSource(&m_Data);
	
	ShowKey2Dlg();
	ShowData2Dlg();
	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);
	
	AlignCommercialDB();
	// 현재 DB, Name 저장
	//m_csCurName = m_Data.SName;
	//m_csCurDB = m_Data.SectBefore.SectI.DBName;

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemReg::OnChangeShape() 
{
	SetRedraw(FALSE);

	// TODO: Add your control notification handler code here
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex)
	{
		SetRedraw(TRUE);
		return;
	}

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	
	AlignCommercialDB();

	T_SECT_SECTBASE_D *pSectF;
	pSectF = &m_Data.SectBefore.SectI;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameF = pSectF->DBName;

	// First 초기화
	pSectF->Initialize();
	pSectF->DBName = csDBNameF;

	// MNET:4258-JWKWON-20110108 : 냉간성형강 DB는 ColdForm 형식의 2BS 형식을 사용
	if(m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CB) m_Data.SectBefore.SectI.Shape = _T("2BS");

	// Dialog에 반영
	ShowData2Dlg();

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCMSectItemReg::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	DoActionWhenDBChanged();

	// Parameter 추출
	/*
	#if defined(_DEBUG)
	{
		CString csDBName, csShape, csSectName;
		T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
		csDBName = pSect->DBName;
		int nNumSection = 12; // Gen만
		FILE* fp = _tfopen(_T("C:\\Section.txt"), _T("at"));
		if (fp)
		{
			fprintf(fp, _T("%s\n"), csDBName);
			for (int i = 0; i < nNumSection; i++)
			{
				CString csShape = CSectUtil::GetShapeNameFromIndexReg(i);
				fprintf(fp, _T(" , %s, "), csShape);
				fprintf(fp, _T(" , Area, Center-y, Center-z, Asy, Asz, Ixx, Iyy, Izz, Cyp, Cym, Czp, Czm, Qyb, Qzb, Peri:O, Peri:I"));  // 단면적, 도심 위치 출력
				CStringArray aSizeTitle;
				CSectUtil::GetSizeTitleFromShapeIndexReg(i, aSizeTitle);
				for (int n = 0; n < aSizeTitle.GetSize(); n++)
					fprintf(fp, _T(", %s"), aSizeTitle[n]);
				fprintf(fp, _T("\n"));
				
				// Section name list를 가져온다.
				CArray<CString, CString&> aSectNameList;
				m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);
				for (int j = 0; j < aSectNameList.GetSize(); j++)
				{
					// print name
					fprintf(fp, _T(" , , %s"), aSectNameList[j]);
					// 치수를 가져온다.
					
					csSectName = aSectNameList[j];

					// Size Data
					T_SECT_SECTBASE_D SectData;
					SectData.Initialize();
					if (!m_pDoc->m_pSectDB->GetSectData(csDBName, csSectName, SectData))
					{
						for (int ix = 0; ix < 10; ix++)
							SectData.Size[ix] = 0.0;
					}
					// 단면적, 도심 출력한다.
					fprintf(fp, _T(", %g, %g, %g,  %g, %g,  %g, %g, %g,  %g, %g,  %g, %g,  %g, %g,  %g, %g"), 
						SectData.Stiffness.Area, SectData.Design.YBar, SectData.Design.ZBar,
						SectData.Stiffness.Asy, SectData.Stiffness.Asz,
						SectData.Stiffness.Rxx, SectData.Stiffness.Ryy, SectData.Stiffness.Rzz,
						SectData.Stiffness.Cyp, SectData.Stiffness.Cym, 
						SectData.Stiffness.Czp, SectData.Stiffness.Czm,
						SectData.Stiffness.Qyb, SectData.Stiffness.Qzb, 
						SectData.PeriIn, SectData.PeriOut);

					for (int ix = 0; ix < aSizeTitle.GetSize(); ix++)
						fprintf(fp, _T(", %g"), SectData.Size[ix]);
					fprintf(fp, _T("\n"));
				}
				fprintf(fp, _T("\n"));
			}
			fprintf(fp, _T("\n"));
			fclose(fp);
		}
	}
	#endif
	*/

	// Parameter 추출(2nd format)
	/*
	#if defined(_DEBUG)
	{
		CString csDBName, csShape, csSectName;
		T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
		csDBName = pSect->DBName;
		int nNumSection = 12; // Gen만
		FILE* fp = _tfopen(_T("C:\\Section.txt"), _T("at"));
		if (fp)
		{
			fprintf(fp, _T("[DB: %s]\n"), csDBName);
			for (int i = 0; i < nNumSection; i++)
			{
				CString csShape = CSectUtil::GetShapeNameFromIndexReg(i);
				fprintf(fp, _T("  [Shape: %s]\n"), csShape);
				
				// Section name list를 가져온다.
				CArray<CString, CString&> aSectNameList;
				m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);
				for (int j = 0; j < aSectNameList.GetSize(); j++)
				{
					// print name
					fprintf(fp, _T("      %s\n"), aSectNameList[j]);
				}
				fprintf(fp, _T("\n"));
			}
			fprintf(fp, _T("\n"));
			fclose(fp);
		}
	}
	#endif
	*/
}

void CCMSectItemReg::OnChangeUseDB() 
{
	// TODO: Add your control notification handler code here
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == EN_USER) 
	{
		m_wndDB.EnableWindow(FALSE);
		m_wndCommecialDB.EnableWindow(FALSE);
		m_wndBuiltUp.SetCheck(1); // User -> Always builtup
	}
	else if (nUserOrDB == EN_DB)
	{
		m_wndDB.EnableWindow(TRUE);
		m_wndCommecialDB.EnableWindow(FALSE);
	}
	else 
	{
		m_wndDB.EnableWindow(FALSE);
		m_wndCommecialDB.EnableWindow(TRUE);
	}

	int nShapeIndex = GetCurShapeIndex();
	CString csShape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	if ( csShape == D_SECT_SHAPE_REG_B )
	{
		// change bitmap
		ChangeBitmap();
	}

	// DB가 변경된 경우와 같다.
	DoActionWhenDBChanged();
}

void CCMSectItemReg::OnChangeFirstName() 
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

	// Size Data
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 10; i++)
		pSect->Size[i] = SectData.Size[i];
	// Built Up
	m_Data.SectBefore.SectI.BuiltUpFlag = SectData.BuiltUpFlag;

	// Dialog에 반영한다.
	SetBuiltUp();
	SetFirstSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// cc 관련 
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CC)
		m_Data.SectBefore.SectI.Shape = GetCurCCShape();

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemReg::OnChangeDblDB() 
{
	// TODO: Add your control notification handler code here
	// Get DB Name
	CString csDB;
	CString csShape;
	m_wndDblDB.GetWindowText(csDB);

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_2L ||
			m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_CL  ) 
			csShape = D_SECT_SHAPE_REG_L;
	else csShape = D_SECT_SHAPE_REG_C;

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDB, csShape, aSectNameList);

	// Section name list를 설정한다.
	BOOL bRemoveUC = (csDB==_T("GB-YB05") && csShape==_T("C"));
	m_wndDblSect.ResetContent();
	m_wndDblSect.InitStorage(aSectNameList.GetSize(), m_SectItemRes.MaxByte);

	int nSize = aSectNameList.GetSize();;
	for (int i = 0; i < nSize; i++)
	{
		CString strSectName = aSectNameList.GetAt(i);
		if(bRemoveUC)
		{
			T_SECT_SECTBASE_D SectData;
			m_pDoc->m_pSectDB->GetSectData(csDB, strSectName, SectData);
			if(SectData.Size[1]!=SectData.Size[4]) continue;
		}

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDB, strSectName);
#endif
		m_wndDblSect.AddString(strSectName);  
	}
}

void CCMSectItemReg::OnChangeDblSectName() 
{
	// TODO: Add your control notification handler code here
	CString csDB;
	CString csName;
	m_wndDblDB.GetWindowText(csDB); 
	m_wndDblSect.GetWindowText(csName);
	if (csDB == _T("")) return;
	if (csName == _T("")) return;


#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(csDB, csName);
#endif

	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData);
	for (int i = 0; i < 4; i++) // 4개만 카피한다.
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetFirstSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;

#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDB, csName);
#endif

		int nShapeIndex = GetCurShapeIndex();
		CString csShape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

		if ( csShape == D_SECT_SHAPE_REG_CL )
		{
			m_wndName.SetWindowText(_T("C") + csName);
		}
		else if ( csShape == D_SECT_SHAPE_REG_2CB )
		{
			if ( !csName.IsEmpty() )
			{
				csName.Delete(0, 1);
			}
			m_wndName.SetWindowText(_T("2CB") + csName);
		}
		else m_wndName.SetWindowText(_T("2")+csName);
	}

	// Preview Update
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemReg::OnChangeFirstSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE1 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE2 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE3 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE4 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE5 : nIx = 4; break;
	case IDC_CMD_SP_ID_VALUE6 : nIx = 5; break;
	case IDC_CMD_SP_ID_VALUE7 : nIx = 6; break;
	case IDC_CMD_SP_ID_VALUE8 : nIx = 7; break;
	case IDC_CMD_SP_ID_VALUE9 : nIx = 8; break;
	case IDC_CMD_SP_ID_VALUE10: nIx = 9; break;
	case IDC_CMD_SP_ID_VALUE11: nIx = 10; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();	
}

void CCMSectItemReg::OnChangeBuiltUp() 
{
	// TODO: Add your control notification handler code here
	if (m_wndBuiltUp.GetCheck() == 1)
	  m_Data.SectBefore.SectI.BuiltUpFlag = 1;
	else m_Data.SectBefore.SectI.BuiltUpFlag = 2;
}

void CCMSectItemReg::OnChangePrincipalAxis()
{
	int nShapeIndex = GetCurShapeIndex();
	CString csShape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	if (csShape != D_SECT_SHAPE_REG_L)  return;

	const int nMovingType = CProduct::GetMovingType();
	//if ( nMovingType != D_PRODUCT_MOVING_US ) return;

	BOOL bUser = FALSE;
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB == EN_USER) bUser = TRUE;
	if (!bUser)  return;

	BOOL bPrincipalAxis = FALSE;
	if (bUser && m_wndPrincipalAxis.GetCheck() == 1) bPrincipalAxis = TRUE;

	if( nMovingType != D_PRODUCT_MOVING_US ) bPrincipalAxis = FALSE;

	// B1, B2
	m_wndFirstSize[5].EnableWindow(bPrincipalAxis);

	// C
	m_wndFirstSize[4].EnableWindow(!bPrincipalAxis);
	if (bPrincipalAxis)
	{
		m_wndFirstSize[4].SetEditUnit(0.);
		m_Data.SectBefore.SectI.Size[4] = 0.;
	}
	else
	{
		m_wndFirstSize[5].SetEditUnit(0.);
		m_Data.SectBefore.SectI.Size[5] = 0.;
	}

	const double dTheta = m_wndFirstSize[5].GetEditValue();
	if ( bPrincipalAxis && dgn::EQ0(dTheta) )
	{
		const double dDefAngle = m_pDoc->m_pSectDB->CalcDefaultPrincipalAxisAngle(m_Data.SectBefore.SectI);
		m_Data.SectBefore.SectI.Size[5] = dDefAngle;
		m_wndFirstSize[5].SetEditUnit(dDefAngle);
	}

	m_Data.bPricipalAxis = bPrincipalAxis;
	m_wndSecView.Invalidate();
}

void CCMSectItemReg::OnChangeHSQType() 
{
	int nShapeIndex = GetCurShapeIndex();
	CString csShape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	if (csShape != D_SECT_SHAPE_REG_B)  return;

	BOOL bUser = FALSE;
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB == EN_USER) bUser = TRUE;
	if (!bUser)  return;

	BOOL bHSQ = FALSE;
	if (bUser && m_wndHSQType.GetCheck() == 1)
	{
		bHSQ = CDBLib::ActivateHSQ();
	}

	// B1, B2
	m_wndFirstSize[7].EnableWindow(bHSQ);
	m_wndFirstSize[8].EnableWindow(bHSQ);

	// C
	m_wndFirstSize[4].EnableWindow(!bHSQ);    
	if (bHSQ)
	{
		m_wndFirstSize[4].SetEditUnit(0.);
		m_Data.SectBefore.SectI.Size[4] = 0.;
	}

	m_Data.bHSQType = bHSQ;

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	m_wndSecView.Invalidate();
}

void CCMSectItemReg::OnChangeRoundType()
{
	int nShapeIndex = GetCurShapeIndex();
	CString csShape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	if ( csShape != D_SECT_SHAPE_REG_B ) return;

	BOOL bUser = FALSE;
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if ( nUserDB == EN_USER ) bUser = TRUE;
	if ( !bUser ) return;

	auto& pSect = m_Data.SectBefore.SectI;
	BOOL bRoundType = FALSE;
	if ( bUser && m_wndRoundType.GetCheck() == 1 )
	{
		bRoundType = TRUE;
		pSect.Size[10] = m_wndFirstSize[10].GetEditValue();
	}
	else
	{
		pSect.Size[10] = 0.;
	}

	// change bitmap
	ChangeBitmap();

	BOOL bHSQ = m_Data.bHSQType;
	GetDlgItem(IDC_CMD_HSQ_TYPE_CHK)->EnableWindow(!bRoundType);
	m_wndFirstSize[4].EnableWindow(!(bRoundType || bHSQ));
	if ( bRoundType || bHSQ )
	{
		m_wndFirstSize[4].SetEditUnit(0.0);
		m_Data.SectBefore.SectI.Size[4] = 0.0;
	}

	m_wndFirstSize[3].EnableWindow(!bRoundType);
	m_wndFirstSize[5].EnableWindow(!bRoundType);

	if ( bHSQ )
	{
		m_wndFirstSize[7].EnableWindow(!bRoundType);
		m_wndFirstSize[8].EnableWindow(!bRoundType);
	}

	m_wndFirstSize[10].EnableWindow(bRoundType);

	m_Data.bRoundType = bRoundType;

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	m_wndSecView.Invalidate();

	return;
}

void CCMSectItemReg::OnSetfocusCmdSpIdName() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}

void CCMSectItemReg::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(csDB, csName);
#endif
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		int nUserOrDB = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
		if (nUserOrDB == EN_USER && (csName.Left(2) == _T("2L") || csName.Left(2) == _T("2C") || csName.Left(2) == _T("CL")))
		{
			m_wndDblDB.GetWindowText(csDB);
#if defined(_RUS)
			CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif
			csName = csName.Right(csName.GetLength()-1);
		}
		else if (csName.Left(2) == _T("PH") || csName.Left(3) == _T("PII"))
		{
			m_wndCommecialDB.GetWindowText(csDB);
		}
		if (csDB == _T("")) m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}
}

// cc 관련 
void CCMSectItemReg::SetCCShapeCobx(int nCCNum)
{
	m_wndCCShape.ResetContent();
	if (nCCNum == 1)
	{
		m_wndCCShape.EnableWindow(FALSE);
		return;
	}
	m_wndCCShape.EnableWindow(TRUE);

	// 형태 옵션 정의 (문자열과 enum 값을 쌍으로 관리)
	struct ShapeOption {
		CString displayText;
		EN_SECT_SHAPE_REG_CC enumValue;
	};
	// nCCNum에 따른 옵션 선택
	std::vector<ShapeOption> options;
	if (nCCNum == 2)
	{
		options = {
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_IS), EN_SECT_SHAPE_REG_CC::IS},
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_IW), EN_SECT_SHAPE_REG_CC::IW},
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BS), EN_SECT_SHAPE_REG_CC::BS},
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BW), EN_SECT_SHAPE_REG_CC::BW}
		};
	}
	else
	{
		options = {
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BS), EN_SECT_SHAPE_REG_CC::BS},
			{_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BW), EN_SECT_SHAPE_REG_CC::BW}
		};
	}

	// 옵션 추가
	for (const auto& option : options) {
		int index = m_wndCCShape.AddString(option.displayText);
		m_wndCCShape.SetItemData(index, static_cast<DWORD_PTR>(option.enumValue));
	}

	
// 	int index = m_wndCCShape.AddString(_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_IS));
// 	m_wndCCShape.SetItemData(index, static_cast<DWORD_PTR>(EN_SECT_SHAPE_REG_CC::IS));
// 
// 	index = m_wndCCShape.AddString(_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_IW));
// 	m_wndCCShape.SetItemData(index, static_cast<DWORD_PTR>(EN_SECT_SHAPE_REG_CC::IW));
// 
// 	index = m_wndCCShape.AddString(_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BS));
// 	m_wndCCShape.SetItemData(index, static_cast<DWORD_PTR>(EN_SECT_SHAPE_REG_CC::BS));
// 
// 	index = m_wndCCShape.AddString(_LS(IDS_CMD_D_SECT_SHAPE_REG_CC_BW));
// 	m_wndCCShape.SetItemData(index, static_cast<DWORD_PTR>(EN_SECT_SHAPE_REG_CC::BW));
	
	m_wndCCShape.SetCurSel(0);

	// Adjust Drop Down Width
	CDC* pDC = m_wndCCShape.GetDC();
	CFont* pFont = m_wndCCShape.GetFont();
	CFont* pOldFont = pDC->SelectObject(pFont);

	int maxWidth = 0;
	for (int i = 0; i < m_wndCCShape.GetCount(); i++)
	{
		CString str;
		m_wndCCShape.GetLBText(i, str);

		CSize sz = pDC->GetTextExtent(str);
		maxWidth = max(maxWidth, sz.cx);
	}

	pDC->SelectObject(pOldFont);
	m_wndCCShape.ReleaseDC(pDC);
	m_wndCCShape.SetDroppedWidth(max(maxWidth + 20, m_wndCCShape.GetDroppedWidth()));
}

CString CCMSectItemReg::GetCurCCShape()
{
	int nCCNum = m_wndCCNum.GetCurSel() + 1;
	if (nCCNum <= 1) { return _T(""); }

	int selectedIndex = m_wndCCShape.GetCurSel();
	if (selectedIndex == CB_ERR)
	{
		ASSERT(false);
		return _T("");
	}

	// enum 값에 따른 형태 문자열 매핑
	static const std::map<EN_SECT_SHAPE_REG_CC, CString> shapeMap = {
		{EN_SECT_SHAPE_REG_CC::IS, D_SECT_SHAPE_REG_CC_IS},
		{EN_SECT_SHAPE_REG_CC::IW, D_SECT_SHAPE_REG_CC_IW},
		{EN_SECT_SHAPE_REG_CC::BS, D_SECT_SHAPE_REG_CC_BS},
		{EN_SECT_SHAPE_REG_CC::BW, D_SECT_SHAPE_REG_CC_BW}
	};

	EN_SECT_SHAPE_REG_CC selectedType = static_cast<EN_SECT_SHAPE_REG_CC>(m_wndCCShape.GetItemData(selectedIndex));

	auto it = shapeMap.find(selectedType);
	if (it == shapeMap.end()) 
	{
		ASSERT(false);
		return _T("");
	}

	CString csCCShape;
	csCCShape.Format(_T("%d%s"), nCCNum, it->second);
	
	return csCCShape;
}

void CCMSectItemReg::OnSelchangeCmdSpIdCcNumCobx() 
{
	// TODO: Add your control notification handler code here
	int nCCNum = m_wndCCNum.GetCurSel() + 1;
	SetCCShapeCobx(nCCNum);
	m_Data.SectBefore.SectI.Shape = GetCurCCShape();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	//m_wndSecView.Invalidate();
}

void CCMSectItemReg::OnSelchangeCmdSpIdCcShapeCobx() 
{
	// TODO: Add your control notification handler code here
	m_Data.SectBefore.SectI.Shape = GetCurCCShape();
	//m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

BOOL CCMSectItemReg::GetSectData(T_SECT_D& sect_data) //add hjs 03/03/12
{
	if(Dlg2Data()) 
	{
		sect_data = m_Data;
		return TRUE;
	}
	else
		sect_data.Initialize();
	return FALSE;
}

void CCMSectItemReg::OnCmdSectCentroidBtn() 
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

void CCMSectItemReg::OnChangeCmdEditCellNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// cell number : 1 ~ 4
	CString csCellNum;
	m_editCellNum.GetWindowText(csCellNum);
	int nCellNum = _ttoi(csCellNum);
	if (nCellNum < 1 || nCellNum > 4)
	{
		if (nCellNum > 4) nCellNum = 4;
		if (nCellNum < 1) nCellNum = 1;

		csCellNum.Format(_T("%d"), nCellNum);
		m_editCellNum.SetWindowText(csCellNum);
	}

	ChangeCellNumber(nCellNum);	

	m_Data.SectBefore.nCellShape = nCellNum;
	m_wndSecView.Invalidate();
}

// void CCMSectItemReg::OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_EDIT_CELL_NUM,_T("1"),_T("4"),1,pNMHDR,pResult);
// 	
// 	CString csCellNum;
// 	m_editCellNum.GetWindowText(csCellNum);
// 	m_Data.SectBefore.nCellShape = _ttoi(csCellNum);
// 	ChangeCellNumber(m_Data.SectBefore.nCellShape);	
// 	m_wndSecView.Invalidate();
// 
// 	*pResult = 0;
// }

void CCMSectItemReg::OnChangeCmdSpIdValueN1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int nN1 = m_editN1.GetEditValueInt();
	if (nN1 < 1) 
	{
		nN1 = 1;
		m_editN1.SetValue(nN1);
	}
	m_Data.SectBefore.nCellShape = nN1;
	m_wndSecView.Invalidate();	
}

void CCMSectItemReg::OnChangeCmdSpIdValueN2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int nN2 = m_editN2.GetEditValueInt();
	if (nN2 < 1) 
	{
		m_editN2.SetValue(nN2);
	}
	m_Data.SectBefore.nCellType = nN2;
	m_wndSecView.Invalidate();		
}

// void CCMSectItemReg::OnDeltaposCmdSpIdSpinN1(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_SP_ID_VALUE_N1,_T("1"),_T("100"),1,pNMHDR,pResult);
// 	
// 	CString csN1;
// 	m_editN1.GetWindowText(csN1);
// 	m_Data.SectBefore.nCellShape = _ttoi(csN1);
// 	m_wndSecView.Invalidate();
// 
// 	*pResult = 0;
// }
// 
// void CCMSectItemReg::OnDeltaposCmdSpIdSpinN2(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_SP_ID_VALUE_N2,_T("1"),_T("100"),1,pNMHDR,pResult);
// 	
// 	CString csN2;
// 	m_editN2.GetWindowText(csN2);
// 	m_Data.SectBefore.nCellType = _ttoi(csN2);
// 	m_wndSecView.Invalidate();
// 
// 	*pResult = 0;
// }

void CCMSectItemReg::OnCmdChangeOffsetBtn() 
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

BOOL CCMSectItemReg::IsEnableCommercialDB()
{
#ifdef _MGEN
	return FALSE;
// 	int nMovingType = CProduct::GetMovingType();
// 	if (nMovingType != D_PRODUCT_MOVING_ORG) return FALSE;
// 	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_H || m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_B) return TRUE;
#else
	return FALSE;
#endif
	return FALSE;
}

void CCMSectItemReg::AlignCommercialDB()
{
	BOOL bShowComDB = IsEnableCommercialDB();
	if (m_bShowCommercialDB == bShowComDB) return;

	int nMoveY = bShowComDB ? - m_nDistCommercial : m_nDistCommercial;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMoveByCommDB, nMoveY, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCommDB, bShowComDB);

	m_bShowCommercialDB = bShowComDB;

	// commercial DB 라디오가 체크 되어 있는데 commercial DB 형상이 아닌 경우에는
	// commercial DB 라디오를 일반 DB 라디오로 변경시켜 준다.
	
	int nUserDB = EN_USER;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB == EN_COMMERCIAL && !bShowComDB)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, EN_DB);
		m_wndDB.EnableWindow(TRUE);
		m_wndCommecialDB.EnableWindow(FALSE);
		m_wndDB.GetWindowText(m_Data.SectBefore.SectI.DBName);
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBRusToEng(m_Data.SectBefore.SectI.DBName);
#endif
	}

	Invalidate();
}