// CMSectItemTap.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemTapOld.h"
#include "..\wg_base\wg_base_MsgDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "CMSecPageStiffDlg.h"

// Default Tapered Section Shape
#define D_SECT_TAPERED_DEF_SHAPE D_SECT_SHAPE_REG_H


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTapOld dialog
#define   CDialog   CCMSectItemBase

CCMSectItemTapOld::CCMSectItemTapOld(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemTapOld::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemTapOld)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlDB.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_DB_RADIO);
}


void CCMSectItemTapOld::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemTapOld)
	DDX_Control(pDX, IDC_CMD_SP_ID_VAR2, m_wndVar2);
	DDX_Control(pDX, IDC_CMD_SP_ID_VAR1, m_wndVar1);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_OFFSET, m_wndOffset);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME2, m_wndSecondName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME1, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	//}}AFX_DATA_MAP
	UINT aFirstID[][6] = {
		{ IDC_CMD_SP_ID_TITLE11, IDC_CMD_SP_ID_TITLE12, IDC_CMD_SP_ID_TITLE13,
			IDC_CMD_SP_ID_TITLE14, IDC_CMD_SP_ID_TITLE15, IDC_CMD_SP_ID_TITLE16 },
		{ IDC_CMD_SP_ID_VALUE11, IDC_CMD_SP_ID_VALUE12, IDC_CMD_SP_ID_VALUE13,
			IDC_CMD_SP_ID_VALUE14, IDC_CMD_SP_ID_VALUE15, IDC_CMD_SP_ID_VALUE16 },
		{ IDC_CMD_SP_ID_UNIT11,  IDC_CMD_SP_ID_UNIT12,  IDC_CMD_SP_ID_UNIT13 ,
			IDC_CMD_SP_ID_UNIT14,  IDC_CMD_SP_ID_UNIT15,  IDC_CMD_SP_ID_UNIT16  },
	};
	UINT aSecondID[][6] = {
		{ IDC_CMD_SP_ID_TITLE21, IDC_CMD_SP_ID_TITLE22, IDC_CMD_SP_ID_TITLE23, 
			IDC_CMD_SP_ID_TITLE24, IDC_CMD_SP_ID_TITLE25, IDC_CMD_SP_ID_TITLE26 },
		{ IDC_CMD_SP_ID_VALUE21, IDC_CMD_SP_ID_VALUE22, IDC_CMD_SP_ID_VALUE23, 
			IDC_CMD_SP_ID_VALUE24, IDC_CMD_SP_ID_VALUE25, IDC_CMD_SP_ID_VALUE26 },
		{ IDC_CMD_SP_ID_UNIT21,  IDC_CMD_SP_ID_UNIT22,  IDC_CMD_SP_ID_UNIT23,  
			IDC_CMD_SP_ID_UNIT24,  IDC_CMD_SP_ID_UNIT25,  IDC_CMD_SP_ID_UNIT26  },
	};
	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
	for (i = 0; i < 6; i++)
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

BOOL CCMSectItemTapOld::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출

BOOL CCMSectItemTapOld::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		CString strID;
		strID.Format("%d", m_Key);
		m_wndID.SetWindowText(strID);
		//m_Data.SName = "";
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
void CCMSectItemTapOld::ShowCalcResult()
{
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}
	CCMStiffDlg dlg(m_pDoc, &m_Data, this);
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemTapOld::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == "")
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}

BOOL CCMSectItemTapOld::Dlg2Data()
{
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_TAPERED;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)atol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	// Variation
	m_Data.SectBefore.nLytype = m_wndVar1.GetCurSel()+1;
	m_Data.SectBefore.nLztype = m_wndVar2.GetCurSel()+1;

	// Offset
	m_Data.SectBefore.nOffsetPoint = m_wndOffset.GetCurSel()+1;

	// Section I, J
	T_SECT_SECTBASE_D* pSectI = &(m_Data.SectBefore.SectI);
	T_SECT_SECTBASE_D* pSectJ = &(m_Data.SectBefore.SectJ);

	// I, J Section Type
	m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR; // currently, only regular is supported

	// Built-Up : Always 1(built-up)
	m_Data.SectBefore.BuiltUpFlag = 1;

	// Section I
	T_SECT_SECTBASE_D* pDataI = pSectI;
	{
		// DB
		pDataI->DBName = GetDB();
		// SName
		if (pDataI->DBName == "") pDataI->SName = "";
		else m_wndFirstName.GetWindowText(pDataI->SName);
		// Size
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
		for (int i = 0; i < nSizeNum; i++)
			pDataI->Size[i] = m_wndFirstSize[i].GetEditValue();
	}

	// Second
	T_SECT_SECTBASE_D* pDataJ = pSectJ;
	{
		// DB
		pDataJ->DBName = GetDB();
		// SName
		if (pDataJ->DBName == "") pDataJ->SName = "";
		else m_wndSecondName.GetWindowText(pDataJ->SName);
		// Size
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
		for (int i = 0; i < nSizeNum; i++)
			pDataJ->Size[i] = m_wndSecondSize[i].GetEditValue();
	}

	return TRUE;
}

BOOL CCMSectItemTapOld::GetPeriAndStiff()
{
	T_SECT_SECTBASE_D *pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D *pSectJ = &m_Data.SectBefore.SectJ;
	T_SECT_SECTBASE_D *pDataI = pSectI;
	T_SECT_SECTBASE_D *pDataJ = pSectJ;

	if (pDataI->DBName == "") // 계산한다.
	{
		T_SECT_D data;

		// Regular Section 단면 계산을 사용한다.
		data.Initialize();
		data.nStype = D_SECT_TYPE_REGULAR;
		data.SectBefore.Shape = m_Data.SectBefore.Shape;
		data.SectBefore.SectI = *pSectI;
		m_pDoc->m_pSectDB->CalcSectData(data);

		// peri and stiffness
		pSectI->PeriIn = data.SectBefore.SectI.PeriIn;
		pSectI->PeriOut = data.SectBefore.SectI.PeriOut;
		pSectI->Stiffness = data.SectBefore.SectI.Stiffness;
		pSectI->Design =data.SectBefore.SectI.Design;
		//double *Tgt[13], *Src[13];
		//CSectUtil::MakeMapStiffVar(&pDataI->Stiffness, Tgt);
		//CSectUtil::MakeMapStiffVar(&data.SectBefore.SectI.Stiffness, Src);
		//for (int k = 0; k < 13; k++) *Tgt[k] = *Src[k];

		data.Initialize();
		data.nStype = D_SECT_TYPE_REGULAR;
		data.SectBefore.Shape = m_Data.SectBefore.Shape;
		data.SectBefore.SectI = *pSectJ;
		m_pDoc->m_pSectDB->CalcSectData(data);

		// peri and stiffness
		pSectJ->PeriIn = data.SectBefore.SectI.PeriIn;
		pSectJ->PeriOut = data.SectBefore.SectI.PeriOut;
		pSectJ->Stiffness = data.SectBefore.SectI.Stiffness;
		pSectJ->Design =data.SectBefore.SectI.Design;
		//CSectUtil::MakeMapStiffVar(&pDataJ->Stiffness, Tgt);
		//CSectUtil::MakeMapStiffVar(&data.SectBefore.SectI.Stiffness, Src);
		//for (k = 0; k < 13; k++) *Tgt[k] = *Src[k];
	}
	else  // DB에 계산된 강성을 가져온다.
	{
		T_SECT_SECTBASE_D SectData;

		// peri and stiffness data for sect I
		m_pDoc->m_pSectDB->GetSectData(pDataI->DBName, pDataI->SName, SectData); 
		pSectI->PeriIn = SectData.PeriIn;
		pSectI->PeriOut = SectData.PeriOut;
		pSectI->Stiffness = SectData.Stiffness;
		pSectI->Design = SectData.Design;
		//double *Tgt[13], *Src[13];
		//CSectUtil::MakeMapStiffVar(&pDataI->Stiffness, Tgt);
		//CSectUtil::MakeMapStiffVar(&SectData.Stiffness, Src);
		//for (int k = 0; k < 13; k++) *Tgt[k] = *Src[k];

		// peri and stiffness data for sect J
		m_pDoc->m_pSectDB->GetSectData(pDataJ->DBName, pDataJ->SName, SectData);
		pSectJ->PeriIn = SectData.PeriIn;
		pSectJ->PeriOut = SectData.PeriOut;
		pSectJ->Stiffness = SectData.Stiffness;
		pSectJ->Design = SectData.Design;
		//CSectUtil::MakeMapStiffVar(&pDataJ->Stiffness, Tgt);
		//CSectUtil::MakeMapStiffVar(&SectData.Stiffness, Src);
		//for (k = 0; k < 13; k++) *Tgt[k] = *Src[k];
	}

	return TRUE;
}

void CCMSectItemTapOld::InitControls()
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

	// Init Offset combo
	m_wndOffset.ResetContent();
	CStringArray aOffsetNameList;
	CSectUtil::GetOffsetNameList(aOffsetNameList);
	for (int i = 0; i < aOffsetNameList.GetSize(); i++)
		m_wndOffset.AddString(aOffsetNameList.GetAt(i));
	m_wndOffset.SetCurSel(4); // default value : center, center

	// Init Variation combo
	CString varName[] = {_LS(IDS_WG_CMD__ADDD__Linear_ITIZI), _LS(IDS_WG_CMD__ADDD__Parabolic), _LS(IDS_WG_CMD__ADDD__Cubic)};
	for (i = 0; i < 3; i++)
	{
		m_wndVar1.AddString(varName[i]);
		m_wndVar2.AddString(varName[i]);
	}
	m_wndVar1.SetCurSel(0); // default value : Linear
	m_wndVar2.SetCurSel(0); 

	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// Unit control
	for (i = 0; i < 6; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (i = 0; i < 6; i++)
	{
		m_wndSecondSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSecondUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
}

void CCMSectItemTapOld::SetSectionShapeCombo()
{
	UINT aBitmapID[] = { 
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10, 
	};
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section), _LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe), 
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle), _LS(IDS_WG_CMD__ADDD__Solid_Round),
	};

	int nCount = sizeof(aBitmapID)/sizeof(UINT);

	// build image list
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
	m_cboType.SetImageList(m_pImageList);

	// Make items
	COMBOBOXEXITEM cbi;
	CString str;

	for (i = 0; i < nCount; i++)
	{
		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_TEXT
							| CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE;
		cbi.iItem = i;
		str = aItemName[i];
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		cbi.iOverlay = i;
		cbi.iIndent = 0;
		m_cboType.InsertItem(&cbi);
	}
}

void CCMSectItemTapOld::SetDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);
	for (int i = 0; i < DBNameList.GetSize(); i++)
		m_wndDB.AddString(DBNameList.GetAt(i));
}

void CCMSectItemTapOld::ShowKey2Dlg()
{
	CString csKey;
	csKey.Format("%ld", m_Key);
	m_wndID.SetWindowText(csKey);
}

void CCMSectItemTapOld::ShowData2Dlg()
{
	// shape
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	m_cboType.SetCurSel(nShapeIndex);

	// variation
	m_wndVar1.SetCurSel(m_Data.SectBefore.nLytype-1);
	m_wndVar2.SetCurSel(m_Data.SectBefore.nLztype-1);

	// db
	SetDB(m_Data.SectBefore.SectI.DBName);

	// name(db보다 나중에 설정: For AutoChangeName)
	m_wndName.SetWindowText(m_Data.SName);

	// sect I, J
	SetNameCombo();
	SetSectData();   // Title, Value 설정, Show/Hide 수행


	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	m_wndSecView.Invalidate();
}

void CCMSectItemTapOld::SetNameCombo()
{
	// Name List를 삭제
	m_wndFirstName.ResetContent();
	m_wndSecondName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;
	csDBName = pSectI->DBName;
	csShape = m_Data.SectBefore.Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == "") 
	{
		m_wndFirstName.EnableWindow(FALSE);
		m_wndSecondName.EnableWindow(FALSE);
		return;
	}
	m_wndFirstName.EnableWindow(TRUE);
	m_wndSecondName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);

	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	for (int i = 0; i < nSize; i++)
	{
		m_wndFirstName.AddString(aSectNameList.GetAt(i));  
		m_wndSecondName.AddString(aSectNameList.GetAt(i));  
	}

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndFirstName.FindStringExact(-1, pSectI->SName);
	if(nSel == CB_ERR) m_wndFirstName.SelectString(-1, pSectI->SName);
	else m_wndFirstName.SetCurSel(nSel);
	nSel = m_wndSecondName.FindStringExact(-1, pSectJ->SName);
	if (nSel == CB_ERR) m_wndSecondName.SelectString(-1, pSectJ->SName);
	else m_wndSecondName.SetCurSel(nSel);
}

void CCMSectItemTapOld::SetSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_tapered_section_s));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;
	BOOL bEnable = (pSectI->DBName == "") ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	for (int i = 0;i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format("%g", pSectI->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		m_wndFirstSize[i].EnableWindow(bEnable);

		m_wndSecondTitle[i].ShowWindow(SW_SHOW);
		m_wndSecondSize[i].ShowWindow(SW_SHOW);
		m_wndSecondUnit[i].ShowWindow(SW_SHOW);
		m_wndSecondTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format("%g", pSectJ->Size[i]);
		m_wndSecondSize[i].SetWindowText(csValue);
		m_wndSecondSize[i].EnableWindow(bEnable);
	}
	for (; i < 6; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);

		m_wndSecondTitle[i].ShowWindow(SW_HIDE);
		m_wndSecondSize[i].ShowWindow(SW_HIDE);
		m_wndSecondUnit[i].ShowWindow(SW_HIDE);
	}
}

void CCMSectItemTapOld::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_tapered_section_s));
		return;
	}

	UINT aBitmapID[] = {
		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
		IDB_CMD_SP_ISGL_BMP10,
	};
	// set appropriate bitmap
	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMSectItemTapOld::DoActionWhenDBChanged()
{
	CString csDB = GetDB();
	if (csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;

	if (csDB != "") 
	{
		pSectI->Initialize();
		pSectJ->Initialize();
		// change new DB name
	  pSectI->DBName = csDB;
		pSectJ->DBName = csDB;
	}
	else // DB가 None으로 변경되면 Size는 유지

	{
		pSectI->DBName = "";
		pSectJ->DBName = "";
	}
 
	// Dialog에 반영한다.
	SetNameCombo();
	SetSectData();

	// Update preview
	m_wndSecView.Invalidate();
}

CString CCMSectItemTapOld::GetDB()
{
	CString csDB;
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) csDB = "";
	else m_wndDB.GetWindowText(csDB);
	return csDB;
}

void CCMSectItemTapOld::SetDB(CString &csDBName)
{
	if (csDBName == "") // DB None -> user
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
		m_wndDB.EnableWindow(FALSE);
	}
	else  // use DB
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		m_wndDB.EnableWindow(TRUE);
			int nSel = m_wndDB.FindStringExact(-1, csDBName);
			if (nSel == CB_ERR) m_wndDB.SelectString(-1, csDBName);
			else m_wndDB.SetCurSel(nSel);
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemTapOld, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemTapOld)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME1, OnChangeFirstName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME2, OnChangeSecondName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE11, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE21, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_USER_RADIO, OnChangeUseDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_VAR1, OnChangeVar1)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_VAR2, OnChangeVar2)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE12, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE13, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE14, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE15, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE16, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE22, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE23, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE24, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE25, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE26, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_DB_RADIO, OnChangeUseDB)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTapOld message handlers
BOOL CCMSectItemTapOld::OnInitDialog() 
{
	CCMSectItemBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();
		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_TAPERED;
		m_Data.SectBefore.Shape = D_SECT_TAPERED_DEF_SHAPE;
		m_Data.SectBefore.nLytype = 1;
		m_Data.SectBefore.nLztype = 1;
		T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
		T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSectI->DBName = pref.Property.SectionDBName;
		pSectJ->DBName = pSectI->DBName;
	}
	m_wndSecView.SetDataSource(&m_Data);

	ShowKey2Dlg();
	ShowData2Dlg();

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE	
}

void CCMSectItemTapOld::OnChangeShape() 
{
	// TODO: Add your control notification handler code here

	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	T_SECT_SECTBASE_D *pSectI, *pSectJ;
	pSectI = &m_Data.SectBefore.SectI;
	pSectJ = &m_Data.SectBefore.SectJ;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameI = pSectI->DBName;
	CString csDBNameJ = pSectJ->DBName;

	// First 초기화
	pSectI->Initialize();
	pSectI->DBName = csDBNameI;

	// Second 초기화
	pSectJ->Initialize();
	pSectJ->DBName = csDBNameJ;

	// Dialog에 반영
	ShowData2Dlg();
}

void CCMSectItemTapOld::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	DoActionWhenDBChanged();
}

void CCMSectItemTapOld::OnChangeUseDB() 
{
	// TODO: Add your control notification handler code here
	int nUserOrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserOrDB);
	if (nUserOrDB == 0) m_wndDB.EnableWindow(FALSE);
	else m_wndDB.EnableWindow(TRUE);

	// DB가 변경된 경우와 같다.
	DoActionWhenDBChanged();	
}

void CCMSectItemTapOld::OnChangeFirstName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndFirstName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
	if (csName == pSect->SName) return;
	pSect->SName = csName;

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != "")
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 6; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	m_wndSecView.Invalidate();
}

void CCMSectItemTapOld::OnChangeSecondName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndSecondName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectJ;
	if (csName == pSect->SName) return;
	pSect->SName = csName;

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != "")
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 6; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	m_wndSecView.Invalidate();
}


void CCMSectItemTapOld::OnChangeFirstSize() 
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
	case IDC_CMD_SP_ID_VALUE16 : nIx = 5; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	m_wndSecView.Invalidate();
}

void CCMSectItemTapOld::OnChangeSecondSize() 
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
	case IDC_CMD_SP_ID_VALUE25 : nIx = 4; break;
	case IDC_CMD_SP_ID_VALUE26 : nIx = 5; break;
	default: ASSERT(0); break;
	}	
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	pSect->Size[nIx] = m_wndSecondSize[nIx].GetEditValue();
	m_wndSecView.Invalidate();
}

void CCMSectItemTapOld::OnChangeVar1() 
{
	// TODO: Add your control notification handler code here
	m_Data.SectBefore.nLytype = m_wndVar1.GetCurSel()+1;
}

void CCMSectItemTapOld::OnChangeVar2() 
{
	// TODO: Add your control notification handler code here
	m_Data.SectBefore.nLztype = m_wndVar2.GetCurSel()+1;
}

void CCMSectItemTapOld::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}

void CCMSectItemTapOld::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
	if (csName == "") m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		if (csDB == "") m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}	
}
