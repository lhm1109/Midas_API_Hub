// CMSectSODStiffenerDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectSODStiffenerDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSectSODStiffenerDlg dialog

CCMSectSODStiffenerDlg::CCMSectSODStiffenerDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectSODStiffenerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectSODStiffenerDlg)
	//}}AFX_DATA_INIT
	m_Data.Initialize();
	m_Data_Draw.Initialize();

	m_bTapJ = FALSE;
	m_nSelIndex = -1;
	m_nDlgType = 0;

	m_pStiffnerBmp = 0;
}

CCMSectSODStiffenerDlg::~CCMSectSODStiffenerDlg()
{
	if(m_pStiffnerBmp) { delete m_pStiffnerBmp; m_pStiffnerBmp = NULL; }
}

void CCMSectSODStiffenerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectSODStiffenerDlg)
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_NAME_EDT,  m_edtName);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_TYPE_CMB,  m_cmbType);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE1_EDT, m_edtSize1);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE1_UNT, m_untSize1);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE2_EDT, m_edtSize2);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE2_UNT, m_untSize2);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE3_EDT, m_edtSize3);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE3_UNT, m_untSize3);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE4_EDT, m_edtSize4);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE4_UNT, m_untSize4);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE5_EDT, m_edtSize5);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_SIZE5_UNT, m_untSize5);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_LST,       m_lstStiffener);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_PIC,       m_ImgSectItem);

	//}}AFX_DATA_MAP
}

void CCMSectSODStiffenerDlg::InitUnit()
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

void CCMSectSODStiffenerDlg::InitCombo()
{
	m_cmbType.ResetContent();

	if (m_nDlgType == 0)
	{
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT)), 0);
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE)),  1);
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_URIB)), 2);

		if (CTestEnvMgr::IsTestEnvST(_T("LTypeTest"), _T("yes")))
		{
			m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_L)), 3);
		}
	}
	else if (m_nDlgType == 1)
	{
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_L)), 3);
	}
	else if (m_nDlgType == 2)
	{
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT)), 0);
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE)), 1);
		m_cmbType.SetItemData(m_cmbType.AddString(_LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_L)), 3);
	}
	else
	{
		ASSERT(0);
	}
	
	m_cmbType.SetCurSel(0);
}

void CCMSectSODStiffenerDlg::InitView()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_VIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetDataSource(&m_Data_Draw);
}

void CCMSectSODStiffenerDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	BOOL bShow1 = FALSE;
	BOOL bShow2 = FALSE;
	BOOL bShow3 = FALSE;
	BOOL bShow4 = FALSE;
	BOOL bShow5 = FALSE;
	
	int nType = m_cmbType.GetItemData(m_cmbType.GetCurSel());
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
	else if(nType == 3) // L
	{
		bShow1 = TRUE;
		bShow2 = TRUE;
		bShow3 = TRUE;
		bShow4 = TRUE;
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE1_STC)->ShowWindow(bShow1);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE1_EDT)->ShowWindow(bShow1);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE1_UNT)->ShowWindow(bShow1);

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE2_STC)->ShowWindow(bShow2);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE2_EDT)->ShowWindow(bShow2);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE2_UNT)->ShowWindow(bShow2);

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE3_STC)->ShowWindow(bShow3);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE3_EDT)->ShowWindow(bShow3);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE3_UNT)->ShowWindow(bShow3);

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE4_STC)->ShowWindow(bShow4);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE4_EDT)->ShowWindow(bShow4);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE4_UNT)->ShowWindow(bShow4);

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE5_STC)->ShowWindow(bShow5);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE5_EDT)->ShowWindow(bShow5);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE5_UNT)->ShowWindow(bShow5);
}

void CCMSectSODStiffenerDlg::ChangeSizeText()
{
	UpdateData(TRUE);

	CString strSize1, strSize2, strSize3, strSize4, strSize5;

	int nType = m_cmbType.GetItemData(m_cmbType.GetCurSel());
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
	else if(nType == 3) // L
	{
		strSize1 = _T("H");
		strSize2 = _T("B");
		strSize3 = _T("tw");
		strSize4 = _T("tf");
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE1_STC)->SetWindowText(strSize1);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE2_STC)->SetWindowText(strSize2);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE3_STC)->SetWindowText(strSize3);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE4_STC)->SetWindowText(strSize4);
	GetDlgItem(IDC_CMD_SOD_STIFFENER_SIZE5_STC)->SetWindowText(strSize5);
}

void CCMSectSODStiffenerDlg::ChangeBitmap()
{
	UpdateData(TRUE);

	UINT aBitmapID[] = { IDB_CMD_SEC_STIFFENER_SR, IDB_CMD_SEC_STIFFENER_T, IDB_CMD_SEC_STIFFENER_URIB, IDB_CMD_SEC_STIFFENER_L};
	
	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\SecStiffener_SR.svg"),
		_T("SVG\\illustration\\Dialog\\SecStiffener_T.svg"),
		_T("SVG\\illustration\\Dialog\\SecStiffener_URib.svg"),
		_T("SVG\\illustration\\Dialog\\SecStiffener_L.svg"),
	};
	int nSelBitmap = m_cmbType.GetItemData(m_cmbType.GetCurSel());
	UINT bitmapID = aBitmapID[nSelBitmap];

// 	if(m_pStiffnerBmp != 0) delete m_pStiffnerBmp;
// 	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SOD_STIFFENER_PIC);
// 	m_pStiffnerBmp = new CBitmap;
// 	m_pStiffnerBmp->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pStiffnerBmp));

	m_ImgSectItem.SetImage(aImgPath_Sect[nSelBitmap]);
}

void CCMSectSODStiffenerDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_SECT_STIFFENER_SHAPE_NAME), _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE)};
	int nColWidth[2] = {250, 150};

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

void CCMSectSODStiffenerDlg::MakeItemEx()
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

BOOL CCMSectSODStiffenerDlg::InsertItem(int nIndex, T_STIFF_SHAPE_D &Data)
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

BOOL CCMSectSODStiffenerDlg::DeleteItem(int nIndex, T_STIFF_SHAPE_D &Data)
{
	if (nIndex >= 0) m_lstStiffener.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMSectSODStiffenerDlg::ModifyItem(int nIndex, T_STIFF_SHAPE_D &Data)
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

CString CCMSectSODStiffenerDlg::DataToStr(int i, T_STIFF_SHAPE_D &Data)
{
	CString str;

	if     (i==0) str = Data.strName;
	else if(i==1)
	{
		if     (Data.nType==0) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT);
		else if(Data.nType==1) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE);
		else if(Data.nType==2) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_URIB);
		else if(Data.nType==3) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_L);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	
	return str;
}

void CCMSectSODStiffenerDlg::Data2Dlg()
{
	m_edtName.SetWindowText(_T(""));
	m_cmbType.SetCurSel(0);
	m_edtSize1.SetEditUnit(0.0);
	m_edtSize2.SetEditUnit(0.0);
	m_edtSize3.SetEditUnit(0.0);
	m_edtSize4.SetEditUnit(0.0);
	m_edtSize5.SetEditUnit(0.0);

	MakeItemEx();
}

BOOL CCMSectSODStiffenerDlg::Dlg2Data()
{
	UpdateData(TRUE);

	return TRUE;
}

void CCMSectSODStiffenerDlg::Data2Dlg_Shape(T_STIFF_SHAPE_D &rData)
{
	m_edtName.SetWindowText(rData.strName);
	CDlgUtil::CobxSetCurSelItemData(m_cmbType, rData.nType);
	m_edtSize1.SetEditUnit(rData.dSize[0]);
	m_edtSize2.SetEditUnit(rData.dSize[1]);
	m_edtSize3.SetEditUnit(rData.dSize[2]);
	m_edtSize4.SetEditUnit(rData.dSize[3]);
	m_edtSize5.SetEditUnit(rData.dSize[4]);
	
	UpdateData(FALSE);
}

BOOL CCMSectSODStiffenerDlg::Dlg2Data_Shape(T_STIFF_SHAPE_D &rData)
{
	UpdateData(TRUE);

	m_edtName.GetWindowText(rData.strName);
	rData.nType = m_cmbType.GetItemData(m_cmbType.GetCurSel());
	rData.dSize[0] = m_edtSize1.GetEditValue();
	rData.dSize[1] = m_edtSize2.GetEditValue();
	rData.dSize[2] = m_edtSize3.GetEditValue();
	rData.dSize[3] = m_edtSize4.GetEditValue();
	rData.dSize[4] = m_edtSize5.GetEditValue();

	return TRUE;
}

BOOL CCMSectSODStiffenerDlg::ErrorCheck_Shape(T_STIFF_SHAPE_D &ShapeD, CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aStiffShape, BOOL bModify, int nIndex)
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

		if (m_nDlgType == 2)
		{
// 			if (!bModify && (ShapeD.nType == 0 || ShapeD.nType == 1))
// 			{
// 				if (aStiffShape[i].nType == 0 || aStiffShape[i].nType == 1)
// 				{
// 					AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_ONLY_ONE_SET_ERROR));
// 					return FALSE;
// 				}
// 			}
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
	else if(ShapeD.nType == 3) // L
	{
		if(ShapeD.dSize[0] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[1] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[2] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
		if(ShapeD.dSize[3] <= 0.0) { AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SIZE_ERROR));	return FALSE; }
	}
	else 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_TYPE_ERROR));
		return FALSE; 
	}

	return TRUE;
}

void CCMSectSODStiffenerDlg::GetShapeList_by_Sect(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape)
{
	aShape.RemoveAll();
	aShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);
}

void CCMSectSODStiffenerDlg::UpdateSectView()
{
	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	Dlg2Data_Shape(ShapeD);

	if(ShapeD.nType == 0)
	{
		m_Data_Draw.SName = ShapeD.strName;
		m_Data_Draw.nStype = D_SECT_TYPE_REGULAR;
		m_Data_Draw.SectBefore.Shape = D_SECT_SHAPE_REG_SB;
		
		for(int i=0; i<8; ++i)
		{
			m_Data_Draw.SectBefore.SectI.Size[i] = ShapeD.dSize[i];
		}
	}
	else if(ShapeD.nType == 1)
	{
		m_Data_Draw.SName = ShapeD.strName;
		m_Data_Draw.nStype = D_SECT_TYPE_REGULAR;
		m_Data_Draw.SectBefore.Shape = D_SECT_SHAPE_REG_T;
		
		for(int i=0; i<8; ++i)
		{
			m_Data_Draw.SectBefore.SectI.Size[i] = ShapeD.dSize[i];
		}
	}
	else if(ShapeD.nType == 2)
	{
		m_Data_Draw.SName = ShapeD.strName;
		m_Data_Draw.nStype = D_SECT_TYPE_REGULAR;
		m_Data_Draw.SectBefore.Shape = D_SECT_SHAPE_REG_URIB;
		
		for(int i=0; i<8; ++i)
		{
			m_Data_Draw.SectBefore.SectI.Size[i] = ShapeD.dSize[i];
		}
	}
	else if(ShapeD.nType == 3)
	{
		m_Data_Draw.SName = ShapeD.strName;
		m_Data_Draw.nStype = D_SECT_TYPE_REGULAR;
		m_Data_Draw.SectBefore.Shape = D_SECT_SHAPE_REG_L;

		for(int i=0; i<8; ++i)
		{
			m_Data_Draw.SectBefore.SectI.Size[i] = ShapeD.dSize[i];
		}
	}
	else
	{
		ASSERT(0);
	}

	//m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();
}

BEGIN_MESSAGE_MAP(CCMSectSODStiffenerDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectSODStiffenerDlg)
	ON_CBN_SELCHANGE(IDC_CMD_SOD_STIFFENER_TYPE_CMB, OnCMSectSODStiffenerTypeCmb)
	ON_EN_CHANGE (IDC_CMD_SOD_STIFFENER_SIZE1_EDT,   OnCMSectSODStiffenerSizeEdt)
	ON_EN_CHANGE (IDC_CMD_SOD_STIFFENER_SIZE2_EDT,   OnCMSectSODStiffenerSizeEdt)
	ON_EN_CHANGE (IDC_CMD_SOD_STIFFENER_SIZE3_EDT,   OnCMSectSODStiffenerSizeEdt)
	ON_EN_CHANGE (IDC_CMD_SOD_STIFFENER_SIZE4_EDT,   OnCMSectSODStiffenerSizeEdt)
	ON_EN_CHANGE (IDC_CMD_SOD_STIFFENER_SIZE5_EDT,   OnCMSectSODStiffenerSizeEdt)
	ON_BN_CLICKED(IDC_CMD_SOD_STIFFENER_ADD_BTN,     OnCMSectSODStiffenerAddBtn)
	ON_BN_CLICKED(IDC_CMD_SOD_STIFFENER_MOD_BTN,     OnCMSectSODStiffenerModBtn)
	ON_BN_CLICKED(IDC_CMD_SOD_STIFFENER_DEL_BTN,     OnCMSectSODStiffenerDelBtn)
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SOD_STIFFENER_LST, OnCMItemchangedSODStiffenerList)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectSODStiffenerDlg message handlers

BOOL CCMSectSODStiffenerDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	InitView();

	SetHeaderTitle();

	Data2Dlg();
	
	UpdateData(FALSE);

	ChangeSizeText();
	ChangeBitmap();
	ControlsShowHide();

	UpdateSectView();

	if(m_nSelIndex != -1)
	{
		m_lstStiffener.SetItemState(m_nSelIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectSODStiffenerDlg::OnOK() 
{
	if (!Dlg2Data()) return;	
	CDialogMove::OnOK();
}

void CCMSectSODStiffenerDlg::OnCMSectSODStiffenerTypeCmb()
{
	ChangeSizeText();
	ChangeBitmap();
	ControlsShowHide();
	UpdateSectView();
}

void CCMSectSODStiffenerDlg::OnCMSectSODStiffenerSizeEdt()
{
	UpdateData(TRUE);

	UpdateSectView();
}

void CCMSectSODStiffenerDlg::OnCMSectSODStiffenerAddBtn()
{
	UpdateData(TRUE);
	
	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	Dlg2Data_Shape(ShapeD);
	
	//////////////////////////////////////////////////////////////////////////
	// Error Check
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	aStiffShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);
	if(!ErrorCheck_Shape(ShapeD, aStiffShape, FALSE, 0)) return;
	//////////////////////////////////////////////////////////////////////////

	int nCount = m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetSize();
	InsertItem(nCount, ShapeD);
	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Add(ShapeD);
			
	UpdateSectView();
}

void CCMSectSODStiffenerDlg::OnCMSectSODStiffenerModBtn()
{
	UpdateData(TRUE);
	
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}

	T_STIFF_SHAPE_D PreShapeD; PreShapeD.Initialize();
	PreShapeD = m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetAt(iItem);
	
	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	Dlg2Data_Shape(ShapeD);
	
	//////////////////////////////////////////////////////////////////////////
	// Error Check
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	aStiffShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);
	if(!ErrorCheck_Shape(ShapeD, aStiffShape, TRUE, iItem)) return;
	//////////////////////////////////////////////////////////////////////////

	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.SetAt(iItem, ShapeD);
	ModifyItem(iItem, ShapeD);
	
	// 楷包 单捞磐 贸府
	for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); ++i)
	{
		for(int j=0; j<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); ++j)
		{
			if(PreShapeD.strName == m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].strShapeName)
			{
				m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].strShapeName = ShapeD.strName;
			}
		}
	}
	
	UpdateSectView();
}

void CCMSectSODStiffenerDlg::OnCMSectSODStiffenerDelBtn()
{
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}
	
	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAt(iItem);
	m_lstStiffener.DeleteItem(iItem);
	
	int nCount = m_lstStiffener.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstStiffener.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
		
	UpdateSectView();
}

void CCMSectSODStiffenerDlg::OnCMItemchangedSODStiffenerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	T_STIFF_SHAPE_D ShapeD; ShapeD.Initialize();
	ShapeD = m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetAt(iItem);
	
	Data2Dlg_Shape(ShapeD);
	OnCMSectSODStiffenerTypeCmb();
	
	*pResult = 0;
}