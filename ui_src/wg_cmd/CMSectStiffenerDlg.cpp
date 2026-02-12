// CMSectStiffenerDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectStiffenerDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSectStiffenerDlg dialog

CCMSectStiffenerDlg::CCMSectStiffenerDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectStiffenerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectStiffenerDlg)
	//}}AFX_DATA_INIT

	m_nPosition = 0;
	m_nRefD = 0;

	m_Data.Initialize();
	m_bTapJ = FALSE;

	m_pSectionBmp = 0;
	m_pStiffnerBmp = 0;

	m_pGridN1 = new CCMSectStiffenerGrid();
	m_pGridN2 = new CCMSectStiffenerGrid();
	m_pGridN3 = new CCMSectStiffenerGrid();
	m_pGridN4 = new CCMSectStiffenerGrid();
}

CCMSectStiffenerDlg::~CCMSectStiffenerDlg()
{
	if(m_pGridN1) { delete m_pGridN1; m_pGridN1 = NULL; }
	if(m_pGridN2) { delete m_pGridN2; m_pGridN2 = NULL; }
	if(m_pGridN3) { delete m_pGridN3; m_pGridN3 = NULL; }
	if(m_pGridN4) { delete m_pGridN4; m_pGridN4 = NULL; }

	if(m_pSectionBmp)  { delete m_pSectionBmp;  m_pSectionBmp  = NULL; }
	if(m_pStiffnerBmp) { delete m_pStiffnerBmp; m_pStiffnerBmp = NULL; }
}

void CCMSectStiffenerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectStiffenerDlg)
	DDX_Control(pDX, IDC_CMD_STIFFENER_NAME_EDT,  m_edtName);
	DDX_Control(pDX, IDC_CMD_STIFFENER_TYPE_CMB,  m_cmbType);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE1_EDT, m_edtSize1);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE1_UNT, m_untSize1);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE2_EDT, m_edtSize2);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE2_UNT, m_untSize2);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE3_EDT, m_edtSize3);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE3_UNT, m_untSize3);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE4_EDT, m_edtSize4);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE4_UNT, m_untSize4);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE5_EDT, m_edtSize5);
	DDX_Control(pDX, IDC_CMD_STIFFENER_SIZE5_UNT, m_untSize5);
	DDX_Control(pDX, IDC_CMD_STIFFENER_LST,       m_lstStiffener);
	DDX_Radio  (pDX, IDC_CMD_STIFFENER_BOTH_RDO,  m_nPosition); 
	DDX_Radio  (pDX, IDC_CMD_STIFFENER_REF_D_TOP_RDO, m_nRefD);
	DDX_Control(pDX, IDC_CMD_STIFFENER_N1_EDT,    m_edtN1Spin);
	DDX_Control(pDX, IDC_CMD_STIFFENER_N2_EDT,    m_edtN2Spin);
	DDX_Control(pDX, IDC_CMD_STIFFENER_N3_EDT,    m_edtN3Spin);
	DDX_Control(pDX, IDC_CMD_STIFFENER_N4_EDT,    m_edtN4Spin);

	DDX_Control(pDX, IDC_CMD_STIFFENER_GRD1, *m_pGridN1);
	DDX_Control(pDX, IDC_CMD_STIFFENER_GRD2, *m_pGridN2);
	DDX_Control(pDX, IDC_CMD_STIFFENER_GRD3, *m_pGridN3);
	DDX_Control(pDX, IDC_CMD_STIFFENER_GRD4, *m_pGridN4);

	DDX_Control(pDX, IDC_CMD_SECT_STIFFENER_PIC, m_ImgSectItem);
	DDX_Control(pDX, IDC_CMD_STIFFENER_PIC, m_ImgStiffenerItem);
	//}}AFX_DATA_MAP
}

void CCMSectStiffenerDlg::InitUnit()
{
	m_edtName.SetUnitType(D_UNITSYS_NONE);

	m_edtSize1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSize1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSize2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSize2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSize3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSize3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSize4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSize4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSize5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSize5.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMSectStiffenerDlg::InitCombo()
{
	m_cmbType.ResetContent();
	m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT));
	m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE));
	m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_URIB));
	m_cmbType.SetCurSel(0);
}

void CCMSectStiffenerDlg::InitGrid()
{
	m_pGridN1->Initialize(this, 0);
	m_pGridN2->Initialize(this, 1);
	m_pGridN3->Initialize(this, 2);
	m_pGridN4->Initialize(this, 3);
}

void CCMSectStiffenerDlg::InitView()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_STIFFENER_VIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetDataSource(&m_Data);
}

void CCMSectStiffenerDlg::InitSpin()
{
	m_edtN1Spin.SetRange(0, 100);
	m_edtN1Spin.SetInteger(TRUE);
	m_edtN2Spin.SetRange(0, 100);
	m_edtN2Spin.SetInteger(TRUE);
	m_edtN3Spin.SetRange(0, 100);
	m_edtN3Spin.SetInteger(TRUE);
	m_edtN4Spin.SetRange(0, 100);
	m_edtN4Spin.SetInteger(TRUE);
}

void CCMSectStiffenerDlg::InitSectBitmap()
{
	UINT aBitmapID[] = { IDB_CMD_SEC_COMP01_STIFFENER    , IDB_CMD_SEC_COMP02_STIFFENER    , IDB_CMD_SEC_COMP03_STIFFENER    , IDB_CMD_SEC_COMP04_STIFFENER    , IDB_CMD_SEC_COMP05_STIFFENER,
						 IDB_CMD_SEC_COMP01_STIFFENER_BOT, IDB_CMD_SEC_COMP02_STIFFENER_BOT, IDB_CMD_SEC_COMP03_STIFFENER_BOT, IDB_CMD_SEC_COMP04_STIFFENER_BOT, IDB_CMD_SEC_COMP05_STIFFENER_BOT };
	
	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\SectComp01_Stiffener.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp02_Stiffener.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp03_Stiffener.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp04_Stiffener.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp05_Stiffener.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp01_Stiffener_Bottom.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp02_Stiffener_Bottom.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp03_Stiffener_Bottom.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp04_Stiffener_Bottom.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp05_Stiffener_Bottom.svg"),
	};

	int nSectType = 0;

	if     (m_Data.nStype == D_SECT_TYPE_COMPO_B)   nSectType = 0;
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)   
	{
		if     (m_nPosition == 0) nSectType = 1;  // Both
		else if(m_nPosition == 1) nSectType = 3;  // Left
		else if(m_nPosition == 2) nSectType = 4;  // Right
		else ASSERT(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB) nSectType = 2;
	else if(m_Data.nStype == D_SECT_TYPE_TAPERED)
	{
		if     (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B)   nSectType = 0;
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)   
		{
			if     (m_nPosition == 0) nSectType = 1;  // Both
			else if(m_nPosition == 1) nSectType = 3;  // Left
			else if(m_nPosition == 2) nSectType = 4;  // Right
			else ASSERT(0);
		}
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB) nSectType = 2;
		else ASSERT(0);
	}
	else ASSERT(0);

	int nRef = (m_nRefD == 0) ? 0 : 5; // m_nRefD=0: Top 1:Bottom.
	nSectType += nRef;

// 	UINT bitmapID = aBitmapID[nSectType];
// 	if(m_pSectionBmp != 0) delete m_pSectionBmp;
// 	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SECT_STIFFENER_PIC);
// 	m_pSectionBmp = new CBitmap;
// 	m_pSectionBmp->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pSectionBmp));

	m_ImgSectItem.SetImage(aImgPath_Sect[nSectType]);
}

void CCMSectStiffenerDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	// Steel I   : 좌우 -> Both 일 경우 왼쪽 활성화 
	// Steel Box : 좌우하상
	// Steel Tub : 좌우하

	BOOL bSteelBox = FALSE;
	BOOL bSteelI_L = FALSE;
	BOOL bSteelI_R = FALSE;
	BOOL bSteelTub = FALSE;

	if     (m_Data.nStype == D_SECT_TYPE_COMPO_B)   bSteelBox = TRUE;
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)  
	{	
		if     (m_nPosition == 0) bSteelI_L = TRUE;
		else if(m_nPosition == 1) bSteelI_L = TRUE;
		else if(m_nPosition == 2) bSteelI_R = TRUE;
		else ASSERT(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB) bSteelTub = TRUE;
	else if(m_Data.nStype == D_SECT_TYPE_TAPERED)
	{
		if     (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B)   bSteelBox = TRUE;
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)   
		{
			if     (m_nPosition == 0) bSteelI_L = TRUE;
			else if(m_nPosition == 1) bSteelI_L = TRUE;
			else if(m_nPosition == 2) bSteelI_R = TRUE;
			else ASSERT(0);
		}
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB) bSteelTub = TRUE;
		else ASSERT(0);
	}
	else ASSERT(0);

	GetDlgItem(IDC_CMD_STIFFENER_N1_STC)->EnableWindow(bSteelBox || bSteelI_L || bSteelTub);
	GetDlgItem(IDC_CMD_STIFFENER_N1_EDT)->EnableWindow(bSteelBox || bSteelI_L || bSteelTub);

	GetDlgItem(IDC_CMD_STIFFENER_N2_STC)->EnableWindow(bSteelBox || bSteelI_R || bSteelTub);
	GetDlgItem(IDC_CMD_STIFFENER_N2_EDT)->EnableWindow(bSteelBox || bSteelI_R || bSteelTub);

	GetDlgItem(IDC_CMD_STIFFENER_N3_STC)->EnableWindow(bSteelBox || bSteelTub);
	GetDlgItem(IDC_CMD_STIFFENER_N3_EDT)->EnableWindow(bSteelBox || bSteelTub);

	GetDlgItem(IDC_CMD_STIFFENER_N4_STC)->EnableWindow(bSteelBox);
	GetDlgItem(IDC_CMD_STIFFENER_N4_EDT)->EnableWindow(bSteelBox);

	m_pGridN1->EnableDiableGrid(bSteelBox || bSteelI_L || bSteelTub);
	m_pGridN2->EnableDiableGrid(bSteelBox || bSteelI_R || bSteelTub);
	m_pGridN3->EnableDiableGrid(bSteelBox || bSteelTub);
	m_pGridN4->EnableDiableGrid(bSteelBox);

	GetDlgItem(IDC_CMD_STIFFENER_POS_STC  )->EnableWindow(bSteelI_L || bSteelI_R);
	GetDlgItem(IDC_CMD_STIFFENER_BOTH_RDO )->EnableWindow(bSteelI_L || bSteelI_R);
	GetDlgItem(IDC_CMD_STIFFENER_LEFT_RDO )->EnableWindow(bSteelI_L || bSteelI_R);
	GetDlgItem(IDC_CMD_STIFFENER_RIGHT_RDO)->EnableWindow(bSteelI_L || bSteelI_R);

// 	GetDlgItem(IDC_CMD_STIFFENER_REF_D_STC)->EnableWindow(bSteelI_L || bSteelI_R);
// 	GetDlgItem(IDC_CMD_STIFFENER_REF_D_TOP_RDO)->EnableWindow(bSteelI_L || bSteelI_R);
// 	GetDlgItem(IDC_CMD_STIFFENER_REF_D_BOT_RDO)->EnableWindow(bSteelI_L || bSteelI_R);

}

void CCMSectStiffenerDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	BOOL bShow1 = FALSE;
	BOOL bShow2 = FALSE;
	BOOL bShow3 = FALSE;
	BOOL bShow4 = FALSE;
	BOOL bShow5 = FALSE;
	
	int nType = m_cmbType.GetCurSel();
	if(nType == 0) // Flat
	{
		bShow1 = TRUE;
		bShow2 = TRUE;
	}
	else if(nType == 1) // Tee
	{
		bShow1 = TRUE;
		bShow2 = TRUE;
		bShow3 = TRUE;
		bShow4 = TRUE;
	}
	else if(nType == 2) // U-Rib
	{
		bShow1 = TRUE;
		bShow2 = TRUE;
		bShow3 = TRUE;
		bShow4 = TRUE;
		bShow5 = TRUE;
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_STIFFENER_SIZE1_STC)->ShowWindow(bShow1);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE1_EDT)->ShowWindow(bShow1);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE1_UNT)->ShowWindow(bShow1);

	GetDlgItem(IDC_CMD_STIFFENER_SIZE2_STC)->ShowWindow(bShow2);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE2_EDT)->ShowWindow(bShow2);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE2_UNT)->ShowWindow(bShow2);

	GetDlgItem(IDC_CMD_STIFFENER_SIZE3_STC)->ShowWindow(bShow3);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE3_EDT)->ShowWindow(bShow3);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE3_UNT)->ShowWindow(bShow3);

	GetDlgItem(IDC_CMD_STIFFENER_SIZE4_STC)->ShowWindow(bShow4);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE4_EDT)->ShowWindow(bShow4);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE4_UNT)->ShowWindow(bShow4);

	GetDlgItem(IDC_CMD_STIFFENER_SIZE5_STC)->ShowWindow(bShow5);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE5_EDT)->ShowWindow(bShow5);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE5_UNT)->ShowWindow(bShow5);
}

void CCMSectStiffenerDlg::ChangeSizeText()
{
	UpdateData(TRUE);

	CString strSize1, strSize2, strSize3, strSize4, strSize5;

	int nType = m_cmbType.GetCurSel();
	if(nType == 0) // Flat
	{
		strSize1 = _T("H");
		strSize2 = _T("B");
	}
	else if(nType == 1) // Tee
	{
		strSize1 = _T("H");
		strSize2 = _T("B");
		strSize3 = _T("tw");
		strSize4 = _T("tf");
	}
	else if(nType == 2) // U-Rib
	{
		strSize1 = _T("H");
		strSize2 = _T("B1");
		strSize3 = _T("B2");
		strSize4 = _T("t");
		strSize5 = _T("R");
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_STIFFENER_SIZE1_STC)->SetWindowText(strSize1);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE2_STC)->SetWindowText(strSize2);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE3_STC)->SetWindowText(strSize3);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE4_STC)->SetWindowText(strSize4);
	GetDlgItem(IDC_CMD_STIFFENER_SIZE5_STC)->SetWindowText(strSize5);
}

void CCMSectStiffenerDlg::ChangeBitmap()
{
	UpdateData(TRUE);

	UINT aBitmapID[] = { IDB_CMD_SEC_STIFFENER_SR, IDB_CMD_SEC_STIFFENER_T, IDB_CMD_SEC_STIFFENER_URIB };

	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\SecStiffener_SR.svg"),
		_T("SVG\\illustration\\Dialog\\SecStiffener_T.svg"),
		_T("SVG\\illustration\\Dialog\\SecStiffener_URib.svg"),
	};

	int nSelBitmap = m_cmbType.GetCurSel();

// 	UINT bitmapID = aBitmapID[nSelBitmap];
// 	if(m_pStiffnerBmp != 0) delete m_pStiffnerBmp;
// 	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_STIFFENER_PIC);
// 	m_pStiffnerBmp = new CBitmap;
// 	m_pStiffnerBmp->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pStiffnerBmp));

	m_ImgStiffenerItem.SetImage(aImgPath_Sect[nSelBitmap]);
}

void CCMSectStiffenerDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_SECT_STIFFENER_SHAPE_NAME), _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE)};
	int nColWidth[2] = {250, 156};

	CString title;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstStiffener.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstStiffener.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(int i=0; i<2; ++i)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstStiffener.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectStiffenerDlg::MakeItemEx()
{
	m_lstStiffener.DeleteAllItems();

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	GetShapeList_by_Sect(aShape);

	int nSize = aShape.GetSize();
	if(nSize == 0) return;
	
	for(int i=0; i<nSize; ++i)
	{
		InsertItem(i, aShape[i]);
	}
}

BOOL CCMSectStiffenerDlg::InsertItem(int nIndex, T_STIFF_SHAPE_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;
	
	lvitem.iItem = nIndex;
	for(int i=0; i<2; ++i)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		if(i==0) nItem = m_lstStiffener.InsertItem(&lvitem);
		else     m_lstStiffener.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMSectStiffenerDlg::DeleteItem(int nIndex, T_STIFF_SHAPE_D &Data)
{
	if (nIndex >= 0) m_lstStiffener.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMSectStiffenerDlg::ModifyItem(int nIndex, T_STIFF_SHAPE_D &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i=0; i<2; ++i)
		{
			str = DataToStr(i, Data);
			m_lstStiffener.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMSectStiffenerDlg::DataToStr(int i, T_STIFF_SHAPE_D &Data)
{
	CString str;

	if     (i==0) str = Data.strName;
	else if(i==1)
	{
		if     (Data.nType==0) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT);
		else if(Data.nType==1) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE);
		else if(Data.nType==2) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_URIB);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	
	return str;
}

void CCMSectStiffenerDlg::Data2Dlg()
{
	m_edtName.SetWindowText(_T(""));
	m_cmbType.SetCurSel(0);
	m_edtSize1.SetEditUnit(0.0);
	m_edtSize2.SetEditUnit(0.0);
	m_edtSize3.SetEditUnit(0.0);
	m_edtSize4.SetEditUnit(0.0);
	m_edtSize5.SetEditUnit(0.0);

	MakeItemEx();

	m_nPosition = m_Data.SectBefore.SectI.Stiffener.nPosition;
	m_nRefD = m_Data.SectBefore.SectI.Stiffener.nRefPos2;

	int nN1 = 0;
	int nN2 = 0;
	int nN3 = 0;
	int nN4 = 0;

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener1;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener2;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener3;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener4;
	GetShapeList_by_Sect(aShape);

	if(m_bTapJ)
	{
		nN1 = m_Data.CmpTapJ.Stiffener.nStiffNum[0];
		nN2 = m_Data.CmpTapJ.Stiffener.nStiffNum[1];
		nN3 = m_Data.CmpTapJ.Stiffener.nStiffNum[2];
		nN4 = m_Data.CmpTapJ.Stiffener.nStiffNum[3];

		aStiffener1.Copy(m_Data.CmpTapJ.Stiffener.aStiffLeft);
		aStiffener2.Copy(m_Data.CmpTapJ.Stiffener.aStiffRight);
		aStiffener3.Copy(m_Data.CmpTapJ.Stiffener.aStiffBottom);
		aStiffener4.Copy(m_Data.CmpTapJ.Stiffener.aStiffTop);
	}
	else
	{
		nN1 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[0];
		nN2 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[1];
		nN3 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[2];
		nN4 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[3];

		aStiffener1.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffLeft);
		aStiffener2.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffRight);
		aStiffener3.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffBottom);
		aStiffener4.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffTop);
	}
	
	m_edtN1Spin.SetValue(nN1);
	m_edtN2Spin.SetValue(nN2);
	m_edtN3Spin.SetValue(nN3);
	m_edtN4Spin.SetValue(nN4);

	m_pGridN1->InitStiffenerCol(aShape);
	m_pGridN2->InitStiffenerCol(aShape);
	m_pGridN3->InitStiffenerCol(aShape);
	m_pGridN4->InitStiffenerCol(aShape);

	m_pGridN1->SetData2Grid(aShape, aStiffener1, nN1);
	m_pGridN2->SetData2Grid(aShape, aStiffener2, nN2);
	m_pGridN3->SetData2Grid(aShape, aStiffener3, nN3);
	m_pGridN4->SetData2Grid(aShape, aStiffener4, nN4);
}

BOOL CCMSectStiffenerDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.SectBefore.SectI.Stiffener.nPosition = m_nPosition;
	m_Data.SectBefore.SectI.Stiffener.nRefPos2  = m_nRefD;
		
	int nN1 = 0;
	int nN2 = 0;
	int nN3 = 0;
	int nN4 = 0;
	
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener1;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener2;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener3;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener4;

	GetShapeList_by_Sect(aShape);
	
	m_pGridN1->GetGrid2Data(aShape, aStiffener1);
	m_pGridN2->GetGrid2Data(aShape, aStiffener2);
	m_pGridN3->GetGrid2Data(aShape, aStiffener3);
	m_pGridN4->GetGrid2Data(aShape, aStiffener4);

	nN1 = aStiffener1.GetSize();
	nN2 = aStiffener2.GetSize();
	nN3 = aStiffener3.GetSize();
	nN4 = aStiffener4.GetSize();

	if(!ErrorCheck_Stiffener(aStiffener1, 0)) return FALSE;
	if(!ErrorCheck_Stiffener(aStiffener2, 1)) return FALSE;
	if(!ErrorCheck_Stiffener(aStiffener3, 2)) return FALSE;
	if(!ErrorCheck_Stiffener(aStiffener4, 3)) return FALSE;
	
	if(m_bTapJ)
	{
		m_Data.CmpTapJ.Stiffener.nStiffNum[0] = nN1;
		m_Data.CmpTapJ.Stiffener.nStiffNum[1] = nN2;
		m_Data.CmpTapJ.Stiffener.nStiffNum[2] = nN3;
		m_Data.CmpTapJ.Stiffener.nStiffNum[3] = nN4;

		m_Data.CmpTapJ.Stiffener.aStiffLeft  .Copy(aStiffener1);
		m_Data.CmpTapJ.Stiffener.aStiffRight .Copy(aStiffener2);
		m_Data.CmpTapJ.Stiffener.aStiffBottom.Copy(aStiffener3);
		m_Data.CmpTapJ.Stiffener.aStiffTop   .Copy(aStiffener4);
	}
	else
	{
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[0] = nN1;
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[1] = nN2;
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[2] = nN3;
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[3] = nN4;
		
		m_Data.SectBefore.SectI.Stiffener.aStiffLeft  .Copy(aStiffener1);
		m_Data.SectBefore.SectI.Stiffener.aStiffRight .Copy(aStiffener2);
		m_Data.SectBefore.SectI.Stiffener.aStiffBottom.Copy(aStiffener3);
		m_Data.SectBefore.SectI.Stiffener.aStiffTop   .Copy(aStiffener4);
	}

	return TRUE;
}

void CCMSectStiffenerDlg::Data2Dlg_Shape(T_STIFF_SHAPE_D &rData)
{
	m_edtName.SetWindowText(rData.strName);
	m_cmbType.SetCurSel(rData.nType);
	m_edtSize1.SetEditUnit(rData.dSize[0]);
	m_edtSize2.SetEditUnit(rData.dSize[1]);
	m_edtSize3.SetEditUnit(rData.dSize[2]);
	m_edtSize4.SetEditUnit(rData.dSize[3]);
	m_edtSize5.SetEditUnit(rData.dSize[4]);
	
	UpdateData(FALSE);
}

BOOL CCMSectStiffenerDlg::Dlg2Data_Shape(T_STIFF_SHAPE_D &rData)
{
	UpdateData(TRUE);

	m_edtName.GetWindowText(rData.strName);
	rData.nType = m_cmbType.GetCurSel();
	rData.dSize[0] = m_edtSize1.GetEditValue();
	rData.dSize[1] = m_edtSize2.GetEditValue();
	rData.dSize[2] = m_edtSize3.GetEditValue();
	rData.dSize[3] = m_edtSize4.GetEditValue();
	rData.dSize[4] = m_edtSize5.GetEditValue();

	return TRUE;
}

BOOL CCMSectStiffenerDlg::ErrorCheck_Shape(T_STIFF_SHAPE_D &ShapeD, CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aStiffShape, BOOL bModify, int nIndex)
{
	ShapeD.strName.TrimRight();
	if(ShapeD.strName.IsEmpty()) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_NAME_EMPTY_ERROR));
		return FALSE;
	}
	
	for(int i=0; i<aStiffShape.GetSize(); ++i)
	{
		if(bModify)
		{
			if(i == nIndex)  continue;
		}

		if(aStiffShape[i].strName == ShapeD.strName)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SAME_NAME_ERROR));
			return FALSE;
		}
	}
	
	if(ShapeD.nType == 0) // I
	{
		if(ShapeD.dSize[0] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[1] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
	}
	else if(ShapeD.nType == 1) // T
	{
		if(ShapeD.dSize[0] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[1] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[2] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[3] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
	}
	else if(ShapeD.nType == 2) // U
	{
		if(ShapeD.dSize[0] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[1] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[2] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[3] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[4] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
	}
	else 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_TYPE_ERROR));
		return FALSE; 
	}
	return TRUE;
}

BOOL CCMSectStiffenerDlg::ErrorCheck_Stiffener(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nPos)
{
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	GetShapeList_by_Sect(aShape);

	BOOL bSteelBox = FALSE;
	BOOL bSteelI   = FALSE;
	BOOL bSteelTub = FALSE;
	
	if     (m_Data.nStype == D_SECT_TYPE_COMPO_B)   bSteelBox = TRUE;
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_I)   bSteelI   = TRUE;
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_TUB) bSteelTub = TRUE;
	else if(m_Data.nStype == D_SECT_TYPE_TAPERED)
	{
		if     (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B)   bSteelBox = TRUE;
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I)   bSteelI   = TRUE; 
		else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB) bSteelTub = TRUE;
		else ASSERT(0);
	}
	else ASSERT(0);

	int i=0;
	for(i=0; i<aStiffener.GetSize(); ++i)
	{		
		if(bSteelI && m_Data.SectBefore.SectI.Stiffener.nPosition == 1)
		{
			if(nPos == 1) continue; // Right Check 하지 않는다.
		}
		
		if(bSteelI && m_Data.SectBefore.SectI.Stiffener.nPosition == 2)
		{
			if(nPos == 0) continue; // Left Check 하지 않는다.
		}

		if(aStiffener[i].nShape == -1) 
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_NAME_EMPTY_ERROR));
			return FALSE;
		}
		
		if(aStiffener[i].nShape > aShape.GetSize()-1)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_NAME_UNKNOWN_ERROR));
			return FALSE;
		}
		
		if(aStiffener[i].dSpacing <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SPACING_ERROR));
			return FALSE;
		}
	}

	double dleng[3];
	double dHw  = 0.0;
	double dBc  = 0.0;
	double dB1  = 0.0;
	double dB2  = 0.0;
	double dBf1 = 0.0;
	double dBf2 = 0.0;
	double dtw  = 0.0;
	double dtwp = 0.0;
	double theta = 0.0;

	double dTotalSpacing = 0.0;
	for(i=0; i<aStiffener.GetSize(); ++i)
	{
		dTotalSpacing += aStiffener[i].dSpacing;
	}

	if(nPos == 0 || nPos == 1) // Left, Right
	{
		if(m_bTapJ) dHw = m_Data.CmpTapJ.Size[0];
		else        dHw = m_Data.SectBefore.SectI.Size[0];
	
		if(dTotalSpacing > dHw)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_TOTAL_SPACING_ERROR));
			return FALSE;
		}
	}
	else if(nPos == 2 && (bSteelBox || bSteelTub)) // Bottom
	{
		if(m_bTapJ)
		{
			dHw  = m_Data.CmpTapJ.Size[0];
			dtw  = m_Data.CmpTapJ.Size[1];
			dB1  = m_Data.CmpTapJ.Size[2];
			dBf1 = m_Data.CmpTapJ.Size[3];
			dB2  = m_Data.CmpTapJ.Size[5];
			dBf2 = m_Data.CmpTapJ.Size[6];
		}
	  else
		{
		  dHw  = m_Data.SectBefore.SectI.Size[0];
		  dtw  = m_Data.SectBefore.SectI.Size[1];
		  dB1  = m_Data.SectBefore.SectI.Size[2];
		  dBf1 = m_Data.SectBefore.SectI.Size[3];
		  dB2  = m_Data.SectBefore.SectI.Size[5];
		  dBf2 = m_Data.SectBefore.SectI.Size[6];
		}

		dleng[0] = dBc;
		dleng[1] = dBf1 + dB1 + dBf1;
		dleng[2] = dBf2 + dB2 + dBf2;

		double tmp = dBf2+(dleng[1]*0.5-dleng[2]*0.5) - dBf1;
		if(fabs(tmp) > 0.0)
		{
			theta = atan(dHw/tmp);

			// 각이 (-) 일때 뒤집어 준다.
			if(tmp < 0.0)
			{
				theta = (180.0 * CMathFunc::m_trrad) + theta;
			}

			if(sin(theta) == 0.0) dtwp = dtw;
			else                  dtwp = dtw/sin(theta);
		}
		else
		{
			dtwp = dtw;
		}
		
		if(dTotalSpacing > (dB2-dtwp))
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_TOTAL_SPACING_ERROR));
			return FALSE;
		}
	}
	else if(nPos == 3 && bSteelBox) // Top
	{
		if(m_bTapJ)
		{
			dHw  = m_Data.CmpTapJ.Size[0];
			dtw  = m_Data.CmpTapJ.Size[1];
			dB1  = m_Data.CmpTapJ.Size[2];
			dBf1 = m_Data.CmpTapJ.Size[3];
			dB2  = m_Data.CmpTapJ.Size[5];
			dBf2 = m_Data.CmpTapJ.Size[6];
		}
		else
		{
			dHw  = m_Data.SectBefore.SectI.Size[0];
			dtw  = m_Data.SectBefore.SectI.Size[1];
			dB1  = m_Data.SectBefore.SectI.Size[2];
			dBf1 = m_Data.SectBefore.SectI.Size[3];
			dB2  = m_Data.SectBefore.SectI.Size[5];
			dBf2 = m_Data.SectBefore.SectI.Size[6];
		}
		
		dleng[0] = dBc;
		dleng[1] = dBf1 + dB1 + dBf1;
		dleng[2] = dBf2 + dB2 + dBf2;
		
		double tmp = dBf2+(dleng[1]*0.5-dleng[2]*0.5) - dBf1;
		if(fabs(tmp) > 0.0)
		{
			theta = atan(dHw/tmp);
			
			// 각이 (-) 일때 뒤집어 준다.
			if(tmp < 0.0)
			{
				theta = (180.0 * CMathFunc::m_trrad) + theta;
			}
			
			if(sin(theta) == 0.0) dtwp = dtw;
			else                  dtwp = dtw/sin(theta);
		}
		else
		{
			dtwp = dtw;
		}
		
		if(dTotalSpacing > (dB1-dtwp))
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_TOTAL_SPACING_ERROR));
			return FALSE;
		}
	}
	else 
	{
	  // Nothing
	}
	
	return TRUE;
}

void CCMSectStiffenerDlg::GetShapeList_by_Sect(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape)
{
	aShape.RemoveAll();
	if(m_bTapJ) aShape.Copy(m_Data.CmpTapJ.Stiffener.aStiffShape);
	else        aShape.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffShape);
}

void CCMSectStiffenerDlg::GetStiffenerList_by_Sect(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nPos)
{
	aStiffener.RemoveAll();

	if(m_bTapJ)
	{
		if     (nPos==0) aStiffener.Copy(m_Data.CmpTapJ.Stiffener.aStiffLeft);
		else if(nPos==1) aStiffener.Copy(m_Data.CmpTapJ.Stiffener.aStiffRight);
		else if(nPos==2) aStiffener.Copy(m_Data.CmpTapJ.Stiffener.aStiffBottom);
		else if(nPos==3) aStiffener.Copy(m_Data.CmpTapJ.Stiffener.aStiffTop);
		else ASSERT(0);
	}
	else
	{
		if     (nPos==0) aStiffener.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffLeft);
		else if(nPos==1) aStiffener.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffRight);
		else if(nPos==2) aStiffener.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffBottom);
		else if(nPos==3) aStiffener.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffTop);
		else ASSERT(0);
	}
}

void CCMSectStiffenerDlg::UpdateSectView(int nStiffPos)
{
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
	GetShapeList_by_Sect(aShape);

	m_Data.SectBefore.SectI.Stiffener.nPosition = m_nPosition;
	m_Data.SectBefore.SectI.Stiffener.nRefPos2 = m_nRefD;

	if(nStiffPos == 0)
	{
		m_pGridN1->GetGrid2Data(aShape, aStiffener);
		m_Data.SectBefore.SectI.Stiffener.aStiffLeft.RemoveAll();
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[0] = aStiffener.GetSize();
		m_Data.SectBefore.SectI.Stiffener.aStiffLeft.Copy(aStiffener);

		if(m_Data.nStype == D_SECT_TYPE_COMPO_I || (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I) )
		{
			if(m_nPosition == 0) // Both
			{
				m_Data.SectBefore.SectI.Stiffener.aStiffRight.RemoveAll();
				m_Data.SectBefore.SectI.Stiffener.nStiffNum[1] = aStiffener.GetSize();
				m_Data.SectBefore.SectI.Stiffener.aStiffRight.Copy(aStiffener);
			}
		}
	}
	else if(nStiffPos == 1)
	{
		m_pGridN2->GetGrid2Data(aShape, aStiffener);
		m_Data.SectBefore.SectI.Stiffener.aStiffRight.RemoveAll();
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[1] = aStiffener.GetSize();
		m_Data.SectBefore.SectI.Stiffener.aStiffRight.Copy(aStiffener);
	}
	else if(nStiffPos == 2)
	{
		m_pGridN3->GetGrid2Data(aShape, aStiffener);
		m_Data.SectBefore.SectI.Stiffener.aStiffBottom.RemoveAll();
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[2] = aStiffener.GetSize();
		m_Data.SectBefore.SectI.Stiffener.aStiffBottom.Copy(aStiffener);
	}
	else if(nStiffPos == 3)
	{
		m_pGridN4->GetGrid2Data(aShape, aStiffener);
		m_Data.SectBefore.SectI.Stiffener.aStiffTop.RemoveAll();
		m_Data.SectBefore.SectI.Stiffener.nStiffNum[3] = aStiffener.GetSize();
		m_Data.SectBefore.SectI.Stiffener.aStiffTop.Copy(aStiffener);
	}
	else ASSERT(0);
	
	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}

void CCMSectStiffenerDlg::UpdateFromLeftToRight()
{
	if(m_Data.nStype == D_SECT_TYPE_COMPO_I || (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I) )
	{
		if(m_nPosition == 0) // Both
		{
			CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
			CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
			GetShapeList_by_Sect(aShape);

			int nNum = m_edtN1Spin.GetEditValueInt();
			m_edtN2Spin.SetValue(nNum);

			m_pGridN1->GetGrid2Data(aShape, aStiffener);
			m_pGridN2->SetData2Grid(aShape, aStiffener, nNum);
		}
	}
}

BEGIN_MESSAGE_MAP(CCMSectStiffenerDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectStiffenerDlg)
	ON_CBN_SELCHANGE(IDC_CMD_STIFFENER_TYPE_CMB, OnCMSectStiffenerTypeCmb)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_ADD_BTN,     OnCMSectStiffenerAddBtn)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_MOD_BTN,     OnCMSectStiffenerModBtn)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_DEL_BTN,     OnCMSectStiffenerDelBtn)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_BOTH_RDO,    OnCMSectStiffenerPosRdo)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_LEFT_RDO,    OnCMSectStiffenerPosRdo)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_RIGHT_RDO,   OnCMSectStiffenerPosRdo)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_REF_D_TOP_RDO,OnCMSectStiffenerPosRdo)
	ON_BN_CLICKED(IDC_CMD_STIFFENER_REF_D_BOT_RDO,OnCMSectStiffenerPosRdo)
	ON_EN_CHANGE (IDC_CMD_STIFFENER_N1_EDT,      OnCMSectStiffenerN1Edt)
	ON_EN_CHANGE (IDC_CMD_STIFFENER_N2_EDT,      OnCMSectStiffenerN2Edt)
	ON_EN_CHANGE (IDC_CMD_STIFFENER_N3_EDT,      OnCMSectStiffenerN3Edt)
	ON_EN_CHANGE (IDC_CMD_STIFFENER_N4_EDT,      OnCMSectStiffenerN4Edt)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_STIFFENER_LST, OnCMItemchangedStiffenerList)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectStiffenerDlg message handlers

BOOL CCMSectStiffenerDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	InitGrid();
	InitView();
	InitSpin();

	SetHeaderTitle();

	Data2Dlg();
	
	UpdateData(FALSE);

	InitSectBitmap();
	ChangeSizeText();
	ChangeBitmap();
	ControlsShowHide();
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectStiffenerDlg::OnOK() 
{
	if (!Dlg2Data()) return;	
	CDialogMove::OnOK();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerTypeCmb()
{
	ChangeSizeText();
	ChangeBitmap();
	ControlsShowHide();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerAddBtn()
{
	UpdateData(TRUE);

	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	Dlg2Data_Shape(ShapeD);

	//////////////////////////////////////////////////////////////////////////
	// Error Check
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	if(m_bTapJ) aStiffShape.Copy(m_Data.CmpTapJ.Stiffener.aStiffShape);
	else        aStiffShape.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffShape);

	if(!ErrorCheck_Shape(ShapeD, aStiffShape, FALSE, 0)) return;
	//////////////////////////////////////////////////////////////////////////

	int nCount = 0;
	if(m_bTapJ) nCount = m_Data.CmpTapJ.Stiffener.aStiffShape.GetSize();
	else        nCount = m_Data.SectBefore.SectI.Stiffener.aStiffShape.GetSize();
	
	InsertItem(nCount, ShapeD);

	if(m_bTapJ) m_Data.CmpTapJ.Stiffener.aStiffShape.Add(ShapeD);
	else        m_Data.SectBefore.SectI.Stiffener.aStiffShape.Add(ShapeD);

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	GetShapeList_by_Sect(aShape);
	m_pGridN1->InitStiffenerCol(aShape);
	m_pGridN2->InitStiffenerCol(aShape);
	m_pGridN3->InitStiffenerCol(aShape);
	m_pGridN4->InitStiffenerCol(aShape);

	UpdateSectView(0);
	UpdateSectView(1);
	UpdateSectView(2);
	UpdateSectView(3);
}

void CCMSectStiffenerDlg::OnCMSectStiffenerModBtn()
{
	UpdateData(TRUE);

	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}

	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	Dlg2Data_Shape(ShapeD);

	//////////////////////////////////////////////////////////////////////////
	// Error Check
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	if(m_bTapJ) aStiffShape.Copy(m_Data.CmpTapJ.Stiffener.aStiffShape);
	else        aStiffShape.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffShape);

	if(!ErrorCheck_Shape(ShapeD, aStiffShape, TRUE, iItem)) return;
	//////////////////////////////////////////////////////////////////////////

	if(m_bTapJ) m_Data.CmpTapJ.Stiffener.aStiffShape.SetAt(iItem, ShapeD);
	else        m_Data.SectBefore.SectI.Stiffener.aStiffShape.SetAt(iItem, ShapeD);
	
	ModifyItem(iItem, ShapeD);

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	GetShapeList_by_Sect(aShape);
	m_pGridN1->InitStiffenerCol(aShape);
	m_pGridN2->InitStiffenerCol(aShape);
	m_pGridN3->InitStiffenerCol(aShape);
	m_pGridN4->InitStiffenerCol(aShape);

	int nN1 = 0;
	int nN2 = 0;
	int nN3 = 0;
	int nN4 = 0;

	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener1;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener2;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener3;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener4;
	
	if(m_bTapJ)
	{
		nN1 = m_Data.CmpTapJ.Stiffener.nStiffNum[0];
		nN2 = m_Data.CmpTapJ.Stiffener.nStiffNum[1];
		nN3 = m_Data.CmpTapJ.Stiffener.nStiffNum[2];
		nN4 = m_Data.CmpTapJ.Stiffener.nStiffNum[3];
		
		aStiffener1.Copy(m_Data.CmpTapJ.Stiffener.aStiffLeft);
		aStiffener2.Copy(m_Data.CmpTapJ.Stiffener.aStiffRight);
		aStiffener3.Copy(m_Data.CmpTapJ.Stiffener.aStiffBottom);
		aStiffener4.Copy(m_Data.CmpTapJ.Stiffener.aStiffTop);
	}
	else
	{
		nN1 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[0];
		nN2 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[1];
		nN3 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[2];
		nN4 = m_Data.SectBefore.SectI.Stiffener.nStiffNum[3];
		
		aStiffener1.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffLeft);
		aStiffener2.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffRight);
		aStiffener3.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffBottom);
		aStiffener4.Copy(m_Data.SectBefore.SectI.Stiffener.aStiffTop);
	}

	m_pGridN1->SetData2Grid(aShape, aStiffener1, nN1);
	m_pGridN2->SetData2Grid(aShape, aStiffener2, nN2);
	m_pGridN3->SetData2Grid(aShape, aStiffener3, nN3);
	m_pGridN4->SetData2Grid(aShape, aStiffener4, nN4);

	UpdateSectView(0);
	UpdateSectView(1);
	UpdateSectView(2);
	UpdateSectView(3);
}

void CCMSectStiffenerDlg::OnCMSectStiffenerDelBtn()
{
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}

	if(m_bTapJ) m_Data.CmpTapJ.Stiffener.aStiffShape.RemoveAt(iItem);
	else        m_Data.SectBefore.SectI.Stiffener.aStiffShape.RemoveAt(iItem);

	m_lstStiffener.DeleteItem(iItem);
	
	int nCount = m_lstStiffener.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstStiffener.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	GetShapeList_by_Sect(aShape);
	m_pGridN1->InitStiffenerCol(aShape);
	m_pGridN2->InitStiffenerCol(aShape);
	m_pGridN3->InitStiffenerCol(aShape);
	m_pGridN4->InitStiffenerCol(aShape);

	UpdateSectView(0);
	UpdateSectView(1);
	UpdateSectView(2);
	UpdateSectView(3);
}

void CCMSectStiffenerDlg::OnCMItemchangedStiffenerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	if(m_bTapJ) ShapeD = m_Data.CmpTapJ.Stiffener.aStiffShape.GetAt(iItem);
	else        ShapeD = m_Data.SectBefore.SectI.Stiffener.aStiffShape.GetAt(iItem);

	Data2Dlg_Shape(ShapeD);
	OnCMSectStiffenerTypeCmb();
	
	*pResult = 0;
}

void CCMSectStiffenerDlg::OnCMSectStiffenerPosRdo()
{
	UpdateData(TRUE);

	InitSectBitmap();

	UpdateFromLeftToRight();

	UpdateSectView(0);
	UpdateSectView(1);
	UpdateSectView(2);
	UpdateSectView(3);

	EnableDisableControls();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerN1Edt()
{
	if (m_edtN1Spin.GetSafeHwnd() == NULL || !IsWindow(m_edtN1Spin.GetSafeHwnd())) return;

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener_T; // Temp
	T_STIFF_PROFILE_D Stiffener_T; Stiffener_T.Initialize();
	
	GetShapeList_by_Sect(aShape);
	m_pGridN1->GetGrid2Data(aShape, aStiffener_T);

	UpdateData(TRUE);
	int nNum = m_edtN1Spin.GetEditValueInt();
	aStiffener_T.SetSize(nNum);
	
	GetStiffenerList_by_Sect(aStiffener, 0);
	int nSize = aStiffener.GetSize();

	if(nSize > nNum)
	{
		for(int i=0; i<nNum; ++i) aStiffener_T.SetAt(i, aStiffener[i]);
	}
	else if(nSize < nNum)
	{
		for(int i=0; i<nSize; ++i) aStiffener_T.SetAt(i, aStiffener[i]);	 
	}
	else
	{
		// Nothing
	}

	m_Data.SectBefore.SectI.Stiffener.aStiffLeft.RemoveAll();
	m_Data.SectBefore.SectI.Stiffener.nStiffNum[0] = nNum;
	m_Data.SectBefore.SectI.Stiffener.aStiffLeft.Copy(aStiffener_T);

	m_pGridN1->InitStiffenerCol(aShape);
	m_pGridN1->SetData2Grid(aShape, aStiffener_T, nNum);

	UpdateFromLeftToRight();
	EnableDisableControls();

	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerN2Edt()
{
	if (m_edtN2Spin.GetSafeHwnd() == NULL || !IsWindow(m_edtN2Spin.GetSafeHwnd())) return;
	
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener_T; // Temp
	T_STIFF_PROFILE_D Stiffener_T; Stiffener_T.Initialize();
	
	GetShapeList_by_Sect(aShape);
	m_pGridN2->GetGrid2Data(aShape, aStiffener_T);
	
	UpdateData(TRUE);
	int nNum = m_edtN2Spin.GetEditValueInt();
	aStiffener_T.SetSize(nNum);
	
	GetStiffenerList_by_Sect(aStiffener, 1);
	int nSize = aStiffener.GetSize();
			
	if(nSize > nNum)
	{
		for(int i=0; i<nNum; ++i) aStiffener_T.SetAt(i, aStiffener[i]);
	}
	else if(nSize < nNum)
	{
		for(int i=0; i<nSize; ++i) aStiffener_T.SetAt(i, aStiffener[i]);	 
	}
	else
	{
		// Nothing
	}

	m_Data.SectBefore.SectI.Stiffener.aStiffRight.RemoveAll();
	m_Data.SectBefore.SectI.Stiffener.nStiffNum[1] = nNum;
	m_Data.SectBefore.SectI.Stiffener.aStiffRight.Copy(aStiffener_T);
	
	m_pGridN2->InitStiffenerCol(aShape);
	m_pGridN2->SetData2Grid(aShape, aStiffener_T, nNum);

	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerN3Edt()
{
	if (m_edtN3Spin.GetSafeHwnd() == NULL || !IsWindow(m_edtN3Spin.GetSafeHwnd())) return;
	
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener_T; // Temp
	T_STIFF_PROFILE_D Stiffener_T; Stiffener_T.Initialize();
	
	GetShapeList_by_Sect(aShape);
	m_pGridN3->GetGrid2Data(aShape, aStiffener_T);
		
	UpdateData(TRUE);
	int nNum = m_edtN3Spin.GetEditValueInt();
	aStiffener_T.SetSize(nNum);
	
	GetStiffenerList_by_Sect(aStiffener, 2);
	int nSize = aStiffener.GetSize();
	
	if(nSize > nNum)
	{
		for(int i=0; i<nNum; ++i) aStiffener_T.SetAt(i, aStiffener[i]);
	}
	else if(nSize < nNum)
	{
		for(int i=0; i<nSize; ++i) aStiffener_T.SetAt(i, aStiffener[i]);	 
	}
	else
	{
		// Nothing
	}

	m_Data.SectBefore.SectI.Stiffener.aStiffBottom.RemoveAll();
	m_Data.SectBefore.SectI.Stiffener.nStiffNum[2] = nNum;
	m_Data.SectBefore.SectI.Stiffener.aStiffBottom.Copy(aStiffener_T);
	
	m_pGridN3->InitStiffenerCol(aShape);
	m_pGridN3->SetData2Grid(aShape, aStiffener_T, nNum);

	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}

void CCMSectStiffenerDlg::OnCMSectStiffenerN4Edt()
{
	if (m_edtN4Spin.GetSafeHwnd() == NULL || !IsWindow(m_edtN4Spin.GetSafeHwnd())) return;
	
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffener_T; // Temp
	T_STIFF_PROFILE_D Stiffener_T; Stiffener_T.Initialize();
	
	GetShapeList_by_Sect(aShape);
	m_pGridN4->GetGrid2Data(aShape, aStiffener_T);
	
	UpdateData(TRUE);
	int nNum = m_edtN4Spin.GetEditValueInt();
	aStiffener_T.SetSize(nNum);
	
	GetStiffenerList_by_Sect(aStiffener, 3);
	int nSize = aStiffener.GetSize();
	
	if(nSize > nNum)
	{
		for(int i=0; i<nNum; ++i) aStiffener_T.SetAt(i, aStiffener[i]);
	}
	else if(nSize < nNum)
	{
		for(int i=0; i<nSize; ++i) aStiffener_T.SetAt(i, aStiffener[i]);	 
	}
	else
	{
		// Nothing
	}
	
	m_Data.SectBefore.SectI.Stiffener.aStiffTop.RemoveAll();
	m_Data.SectBefore.SectI.Stiffener.nStiffNum[3] = nNum;
	m_Data.SectBefore.SectI.Stiffener.aStiffTop.Copy(aStiffener_T);
	
	m_pGridN4->InitStiffenerCol(aShape);
	m_pGridN4->SetData2Grid(aShape, aStiffener_T, nNum);

	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}