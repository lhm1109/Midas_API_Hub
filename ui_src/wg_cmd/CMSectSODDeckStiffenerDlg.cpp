// CMSectSODDeckStiffenerDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectSODDeckStiffenerDlg.h"
#include "CMSectSODStiffenerDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "CMSectSODDeckStiffenerDlgUtil.h"
#include "CMSectSODDeckStiffenerCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerDlg dialog

CCMSectSODDeckStiffenerDlg::CCMSectSODDeckStiffenerDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectSODDeckStiffenerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectSODDeckStiffenerDlg)
	//}}AFX_DATA_INIT
	m_nRefPos = 0;

	m_Data.Initialize();
	m_bTapJ = FALSE;
	m_nDeckPos = 0;

	m_pSectionBmp = 0;

	m_pGrid = new CCMSectSODDeckStiffenerGrid();
}

CCMSectSODDeckStiffenerDlg::~CCMSectSODDeckStiffenerDlg()
{
	if(m_pGrid)        { delete m_pGrid;        m_pGrid = NULL; }
	if(m_pSectionBmp)  { delete m_pSectionBmp;  m_pSectionBmp  = NULL; }
}

void CCMSectSODDeckStiffenerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectSODDeckStiffenerDlg)
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_DEF_STIFF_LST,  m_lstDefStiff);	
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_DECK_POS_CMB,   m_cmbDeckPos);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_DECK_PART_CMB,  m_cmbDeckPart);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_DECK_NAME_EDT,  m_edtDeckName);
	DDX_Radio  (pDX, IDC_CMD_SECT_SOD_STIFFENER_REF_POS_RDO1,   m_nRefPos);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_NUM_EDT,        m_edtNumber);	
	//DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_NUM_SPN,        m_spnNumber);	
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_LST, m_lstStiffener);
	DDX_Control(pDX, IDC_CMD_SOD_STIFFENER_NAME_GRID, *m_pGrid);
	DDX_Control(pDX, IDC_CMD_SECT_SOD_STIFFENER_PIC, m_ImgStiffenerItem);
	//}}AFX_DATA_MAP
}

void CCMSectSODDeckStiffenerDlg::InitUnit()
{
	m_edtDeckName.SetUnitType(D_UNITSYS_NONE);
}

void CCMSectSODDeckStiffenerDlg::InitCombo()
{
	if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
		 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		m_cmbDeckPos.ResetContent();
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_TOP_FLANGE),     0);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_LEFT_WEB)  ,     1);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_RIGHT_WEB) ,     2);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_BOT_FLANGE),     3);
		m_cmbDeckPos.SetCurSel(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		     (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
				 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		m_cmbDeckPos.ResetContent();
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_TOP_FLANGE),     0);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_WEB)  ,          1);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_BOT_FLANGE) ,    2);
		m_cmbDeckPos.SetCurSel(0);	
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		m_cmbDeckPos.ResetContent();
		//CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_TOP_FLANGE),     0);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_LEFT_WEB)  ,     1);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_RIGHT_WEB) ,     2);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_BOT_FLANGE),     3);
		m_cmbDeckPos.SetCurSel(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		m_cmbDeckPos.ResetContent();
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_TOP_FLANGE),     0);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_WEB)  ,          1);
		CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_BOT_FLANGE) ,    2);
		if(m_Data.SectBefore.nFlangeShape == 2)
		{
			CDlgUtil::CobxAddItem(m_cmbDeckPos, _LS(IDS_CMD_SECT_SOD_SIDE_FLANGE) ,    3);
		}
		m_cmbDeckPos.SetCurSel(0);	
	}
	else
	{
		ASSERT(0);
	}

	ChangeCombo();
}

void CCMSectSODDeckStiffenerDlg::InitGrid()
{
	int nStype;
	if(m_Data.nStype == D_SECT_TYPE_TAPERED)
	{
		nStype = m_Data.SectBefore.nStype;
	}
	else
	{
		nStype = m_Data.nStype;
	}

	m_pGrid->Initialize(m_Data, this, nStype);
}

void CCMSectSODDeckStiffenerDlg::InitView()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_SOD_DECK_STIFFENER_VIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetDataSource(&m_Data);
}

void CCMSectSODDeckStiffenerDlg::InitSpin()
{
	m_edtNumber.SetRange(0, 100);
	m_edtNumber.SetInteger(TRUE);
}

void CCMSectSODDeckStiffenerDlg::ChangeBitmap()
{
	UpdateData(TRUE);
	 
	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	int nDeckPart = m_cmbDeckPart.GetCurSel();

	CString stritmapID;
	stritmapID = CCMSectSODDeckStiffenerDlgUtil::GetBitmapID(m_Data, nDeckPos, nDeckPart, m_cmbDeckPart.GetCount(), m_nRefPos);
	
// 	if(m_pSectionBmp != 0) delete m_pSectionBmp;
// 	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_PIC);
// 	m_pSectionBmp = new CBitmap;
// 	m_pSectionBmp->LoadBitmap(bitmapID);
// 	pImage->SetBitmap(HBITMAP(*m_pSectionBmp));
	m_ImgStiffenerItem.SetImage(stritmapID);
}

void CCMSectSODDeckStiffenerDlg::ChangeCombo()
{
	UpdateData(TRUE);
	
	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);

	std::vector<CString> aStrCbxDeckPartItem;
	CCMSectSODDeckStiffenerDlgUtil::GetCbxDeckPartItem(m_Data, nDeckPos, &aStrCbxDeckPartItem);

	m_cmbDeckPart.ResetContent();
	for (int i = 0; i < aStrCbxDeckPartItem.size(); i++)
	{
		m_cmbDeckPart.AddString(aStrCbxDeckPartItem[i]);
	}
	m_cmbDeckPart.SetCurSel(0);
}

void CCMSectSODDeckStiffenerDlg::ChangeDeckName()
{
	UpdateData(TRUE);

	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	int nDeckPart = m_cmbDeckPart.GetCurSel();
	
	CString strDeckName;
	CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, nDeckPos, nDeckPart, m_cmbDeckPart.GetCount(), &strDeckName);
	m_edtDeckName.SetWindowText(strDeckName);
}

void CCMSectSODDeckStiffenerDlg::ChangeText()
{
	UpdateData(TRUE);
	
	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);

	CString strLeft;
	CString strRight;

	if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
		 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{	
		if(nDeckPos == 0 || nDeckPos == 3)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_LEFT); 
			strRight = _LS(IDS_CMD_SECT_SOD_RIGHT);
		}
		else if(nDeckPos == 1 || nDeckPos == 2)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_TOP); 
			strRight = _LS(IDS_CMD_SECT_SOD_BOTTOM);			
		}
		else ASSERT(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		     (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
				 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		if(nDeckPos == 0 || nDeckPos == 2)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_LEFT); 
			strRight = _LS(IDS_CMD_SECT_SOD_RIGHT);
		}
		else if(nDeckPos == 1)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_TOP); 
			strRight = _LS(IDS_CMD_SECT_SOD_BOTTOM);
		}
		else ASSERT(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		if(nDeckPos == 0 || nDeckPos == 3)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_LEFT); 
			strRight = _LS(IDS_CMD_SECT_SOD_RIGHT);
		}
		else if(nDeckPos == 1 || nDeckPos == 2)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_TOP); 
			strRight = _LS(IDS_CMD_SECT_SOD_BOTTOM);			
		}
		else ASSERT(0);
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		if(nDeckPos == 0 || nDeckPos == 2 || nDeckPos == 3)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_LEFT); 
			strRight = _LS(IDS_CMD_SECT_SOD_RIGHT);
		}
		else if(nDeckPos == 1)
		{
			strLeft  = _LS(IDS_CMD_SECT_SOD_TOP); 
			strRight = _LS(IDS_CMD_SECT_SOD_BOTTOM);            
		}
		else ASSERT(0);
	}
	else
	{
		ASSERT(0);
	}

	GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_REF_POS_RDO1)->SetWindowText(strLeft);
	GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_REF_POS_RDO2)->SetWindowText(strRight);	
}

void CCMSectSODDeckStiffenerDlg::SetHeaderTitle_DefStiff()
{
	CString aTitle[] = {_LS(IDS_CMD_SECT_STIFFENER_SHAPE_NAME), _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE)};
	int nColWidth[2] = {200, 130};
	
	CString title;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstDefStiff.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstDefStiff.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(int i=0; i<2; ++i)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstDefStiff.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectSODDeckStiffenerDlg::MakeItemEx_DefStiff()
{
	m_lstDefStiff.DeleteAllItems();
	
	int nSize = m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetSize();
	if(nSize == 0) return;
	
	for(int i=0; i<nSize; ++i)
	{
		InsertItem_DefStiff(i, m_Data.SectBefore.SectI.SODStiffener.aStiffShape[i]);
	}
}

BOOL CCMSectSODDeckStiffenerDlg::InsertItem_DefStiff(int nIndex, T_STIFF_SHAPE_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;
	
	lvitem.iItem = nIndex;
	for(int i=0; i<2; ++i)
	{
		lvitem.iSubItem=i;
		str = DataToStr_DefStiff(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		if(i==0) nItem = m_lstDefStiff.InsertItem(&lvitem);
		else     m_lstDefStiff.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

CString CCMSectSODDeckStiffenerDlg::DataToStr_DefStiff(int i, T_STIFF_SHAPE_D &Data)
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

void CCMSectSODDeckStiffenerDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_SECT_SOD_DECK_POS), _LS(IDS_CMD_SECT_SOD_DECK_PART), _LS(IDS_CMD_SECT_SOD_DECK_NAME), _LS(IDS_CMD_SECT_SOD_STIFF_NAME)};
	int nColWidth[4] =  {110, 110, 150, 180};

	CString title;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstStiffener.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstStiffener.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(int i=0; i<4; ++i)
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

void CCMSectSODDeckStiffenerDlg::MakeItemEx()
{
	m_lstStiffener.DeleteAllItems();
	
	int nSize = m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	if(nSize == 0) return;
	
	for(int i=0; i<nSize; ++i)
	{
		InsertItem(i, m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i]);
	}
}

BOOL CCMSectSODDeckStiffenerDlg::InsertItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;
	
	lvitem.iItem = nIndex;
	for(int i=0; i<4; ++i)
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

BOOL CCMSectSODDeckStiffenerDlg::DeleteItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data)
{
	if (nIndex >= 0) m_lstStiffener.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMSectSODDeckStiffenerDlg::ModifyItem(int nIndex, T_SECT_SOD_STIFF_SUB_D &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i=0; i<4; ++i)
		{
			str = DataToStr(i, Data);
			m_lstStiffener.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMSectSODDeckStiffenerDlg::DataToStr(int i, T_SECT_SOD_STIFF_SUB_D &Data)
{
	CString str;

	if(i==0)
	{
		if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
			 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
		{
			if     (Data.nDeckPos == 0) str = _LS(IDS_CMD_SECT_SOD_TOP_FLANGE);
			else if(Data.nDeckPos == 1) str = _LS(IDS_CMD_SECT_SOD_LEFT_WEB);
			else if(Data.nDeckPos == 2) str = _LS(IDS_CMD_SECT_SOD_RIGHT_WEB);
			else                        str = _LS(IDS_CMD_SECT_SOD_BOT_FLANGE);
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
			     (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
					 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
		{
			if     (Data.nDeckPos == 0) str = _LS(IDS_CMD_SECT_SOD_TOP_FLANGE);
			else if(Data.nDeckPos == 1) str = _LS(IDS_CMD_SECT_SOD_WEB);
			else                        str = _LS(IDS_CMD_SECT_SOD_BOT_FLANGE);
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
		{
			if     (Data.nDeckPos == 0) str = _LS(IDS_CMD_SECT_SOD_TOP_FLANGE);
			else if(Data.nDeckPos == 1) str = _LS(IDS_CMD_SECT_SOD_LEFT_WEB);
			else if(Data.nDeckPos == 2) str = _LS(IDS_CMD_SECT_SOD_RIGHT_WEB);
			else                        str = _LS(IDS_CMD_SECT_SOD_BOT_FLANGE);
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
		{
			if     (Data.nDeckPos == 0) str = _LS(IDS_CMD_SECT_SOD_TOP_FLANGE);
			else if(Data.nDeckPos == 1) str = _LS(IDS_CMD_SECT_SOD_WEB);      
			else if(Data.nDeckPos == 2) str = _LS(IDS_CMD_SECT_SOD_BOT_FLANGE);
			else if(Data.nDeckPos == 3) str = _LS(IDS_CMD_SECT_SOD_SIDE_FLANGE);
			else ASSERT(0);
		}
		else
		{
			ASSERT(0);
		}
	}
	else if(i==1)
	{
		std::vector<CString> aStrCbxDeckPartItem;
		CCMSectSODDeckStiffenerDlgUtil::GetCbxDeckPartItem(m_Data, Data.nDeckPos, &aStrCbxDeckPartItem);
		if(aStrCbxDeckPartItem.size() <= Data.nDeckPart) {ASSERT(0); return _T("");}
		str = aStrCbxDeckPartItem[Data.nDeckPart];
	}
	else if(i==2)
	{
		str = Data.strDeckName;
	}
	else if(i==3)
	{
		for(int i=0; i<Data.nNum; ++i) 
		{
			if(i==0) str =  Data.aStiffData[i].strStiffName;
			else     
			{
				str += _T(", ");
				str += Data.aStiffData[i].strStiffName;
			}				
		}
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	
	return str;
}

void CCMSectSODDeckStiffenerDlg::Data2Dlg()
{	
	m_cmbDeckPos.SetCurSel(0);
	m_cmbDeckPart.SetCurSel(0);
	m_edtDeckName.SetWindowText(_LS(IDS_CMD_SECT_SOD_TOP_LEFT));

	m_nRefPos = 0;
	m_edtNumber.SetValue(0);

	MakeItemEx();
	MakeItemEx_DefStiff();

	m_pGrid->InitStiffenerCol(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);

	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	int nDeckPart = m_cmbDeckPart.GetCurSel();
	m_pGrid->InitStiffenerPosCol(m_Data, (int)nDeckPos, nDeckPart, m_cmbDeckPart.GetCount());

	m_pGrid->InitStiffenerDir(m_Data, (int)nDeckPos);
}

BOOL CCMSectSODDeckStiffenerDlg::Dlg2Data()
{
	UpdateData(TRUE);

	return TRUE;
}

void CCMSectSODDeckStiffenerDlg::Data2Dlg_StiffSub(T_SECT_SOD_STIFF_SUB_D &Data)
{
	CDlgUtil::CobxSetCurSelItemData(m_cmbDeckPos, (DWORD)Data.nDeckPos);
	m_cmbDeckPart.SetCurSel(Data.nDeckPart);
	m_edtDeckName.SetWindowText(Data.strDeckName);
	m_nRefPos = Data.nRefPos;
	m_edtNumber.SetValue(Data.nNum);

	UpdateData(FALSE);
}

BOOL CCMSectSODDeckStiffenerDlg::Dlg2Data_StiffSub(T_SECT_SOD_STIFF_SUB_D &Data)
{
	UpdateData(TRUE);

	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	Data.nDeckPos = (int)nDeckPos;
	Data.nDeckPart = m_cmbDeckPart.GetCurSel();
	m_edtDeckName.GetWindowText(Data.strDeckName);
	Data.nRefPos = m_nRefPos;
	Data.nNum = m_edtNumber.GetEditValueInt();		

	return TRUE;
}

BOOL CCMSectSODDeckStiffenerDlg::ErrorCheck_Stiffener(T_SECT_SOD_STIFF_SUB_D &Data, BOOL bAdd)
{
	CDBDoc* pDoc;
	pDoc = CDBDoc::GetDocPoint();

	CString strDeckName = Data.strDeckName;
	strDeckName.TrimRight();
	if(strDeckName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_SOD_DECK_NAME_EMPTY_ERROR));
		return FALSE;
	}

	if(Data.nNum <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_NUM_ERROR));
		return FALSE;
	}

	for(int i=0; i<Data.aStiffData.GetSize(); ++i)
	{
		if(Data.aStiffData[i].dSpacing < 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_SPACING_ERROR));
			return FALSE;
		}

		CString strShapeName = Data.aStiffData[i].strShapeName;
		strShapeName.TrimRight();
		if(strShapeName.IsEmpty())
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_NO_SEL_SHAPE_NAME_ERROR));
			return FALSE;
		}
		
		CString strStiffName = Data.aStiffData[i].strStiffName;
		strStiffName.TrimRight();
		if(strStiffName.IsEmpty())
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_NAME_EMPTY_ERROR));
			return FALSE;
		}

		if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
			 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
		{
			if(Data.nDeckPos == 0) // Top
			{
				if(Data.aStiffData[i].nStiffPos != 1) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
			else if(Data.nDeckPos == 3) // Bottom
			{
				if(Data.aStiffData[i].nStiffPos != 0) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
						return FALSE;
				}
			}
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
			     (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
					 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
		{
			if(Data.nDeckPos == 0) // Top
			{
				if(Data.aStiffData[i].nStiffPos != 1) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
						return FALSE;
				}
			}
			else if(Data.nDeckPos == 2) // Bottom
			{
				if(Data.aStiffData[i].nStiffPos != 0) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
						return FALSE;
				}
			}
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
		{
			if(Data.nDeckPos == 0) // Top
			{
				if(Data.aStiffData[i].nStiffPos != 1) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
			else if(Data.nDeckPos == 3) // Bottom
			{
				if(Data.aStiffData[i].nStiffPos != 0) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
		}
		else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
		{
			if(Data.nDeckPos == 0) // Top
			{
				if(Data.aStiffData[i].nStiffPos != 1) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
			else if(Data.nDeckPos == 2) // Bottom
			{
				if(Data.aStiffData[i].nStiffPos != 0) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
			else if(Data.nDeckPos == 3) // Side Flange
			{
				if(Data.aStiffData[i].nStiffPos != 0) 
				{
					AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_POS_ERROR));
					return FALSE;
				}
			}
		}
		else
		{
		 ASSERT(0); 
		}
	}

	CString strText;
	for(int i=0; i<Data.aStiffData.GetSize(); ++i)
	{
		for(int j=0; j<Data.aStiffData.GetSize(); ++j)
		{
			if(i==j) continue;

			if(Data.aStiffData[i].strStiffName == Data.aStiffData[j].strStiffName)
			{
				strText.Format(_LS(IDS_CMD_SECT_SOD_SAME_STIFF_NAME_ERROR), Data.aStiffData[j].strStiffName);
				AfxMessageBox(strText);
				return FALSE;
			}
		}
	}

	for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); ++i)
	{
		for(int j=0; j<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); ++j)
		{
			for(int k=0; k<Data.aStiffData.GetSize(); ++k)
			{
				if(!bAdd) if(j==k) continue;				

				if(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].strStiffName == Data.aStiffData[k].strStiffName)
				{
					strText.Format(_LS(IDS_CMD_SECT_SOD_SAME_STIFF_NAME_ERROR), Data.aStiffData[k].strStiffName);
					AfxMessageBox(strText);
					return FALSE;
				}
			}
		}
	}

	int nFlangeShape = m_Data.SectBefore.nFlangeShape;
	int nCellNum = m_Data.SectBefore.nCellNum;
	int nCellType = m_Data.SectBefore.nCellType;

	int nWebNum = m_Data.SectBefore.nCellNum - 1; // 반복되는 Web 수

	// Deck 길이를 넘어갈때 Error Msg (MQC Alpha/Beta : 4870-24)
	double dSpacing = 0.0;
	for(int i=0; i<Data.aStiffData.GetSize(); ++i)
	{
		dSpacing += Data.aStiffData[i].dSpacing;
	}
	double dDeckLength = 0.0;
	if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
		 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		double B1  = m_Data.SectBefore.SectI.Size[0];
		double B2  = m_Data.SectBefore.SectI.Size[1];
		double B3  = m_Data.SectBefore.SectI.Size[2];
		double B4  = m_Data.SectBefore.SectI.Size[3];
		double B5  = m_Data.SectBefore.SectI.Size[4];
		double B6  = m_Data.SectBefore.SectI.Size[5];
		double H   = m_Data.SectBefore.SectI.Size[6];
		
		if(Data.nDeckPos == 0) // Top
		{
			if     (Data.nDeckPart == 0) dDeckLength = B1; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B2; // Deck2
			else if(Data.nDeckPart == 2) dDeckLength = B3; // Deck3
		}
		else if(Data.nDeckPos == 1 || Data.nDeckPos == 2)
		{
			dDeckLength = H;
		}
		else
		{
			if     (Data.nDeckPart == 0) dDeckLength = B4; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B5; // Deck2
			else if(Data.nDeckPart == 2) dDeckLength = B6; // Deck3
		}

		if(dSpacing > dDeckLength)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_LENGTH_ERROR));
			return FALSE;
		}
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_I || m_Data.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		     (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
				 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		double B1 = m_Data.SectBefore.SectI.Size[0];
		double B2 = m_Data.SectBefore.SectI.Size[1];
		double B3 = m_Data.SectBefore.SectI.Size[2];
		double B4 = m_Data.SectBefore.SectI.Size[3];
		double H  = m_Data.SectBefore.SectI.Size[4];

		if(Data.nDeckPos == 0) // Top
		{
			if     (Data.nDeckPart == 0) dDeckLength = B1; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B2; // Deck2
		}
		else if(Data.nDeckPos == 1)
		{
			dDeckLength = H;
		}
		else
		{
			if     (Data.nDeckPart == 0) dDeckLength = B3; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B4; // Deck2
		}
		
		if(dSpacing > dDeckLength)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_LENGTH_ERROR));
			return FALSE;
		}
	}
	else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		double B1  = m_Data.SectBefore.SectI.Size[0];
		double B2  = m_Data.SectBefore.SectI.Size[1];
		double B3  = m_Data.SectBefore.SectI.Size[2];
		double B4  = m_Data.SectBefore.SectI.Size[3];
		double B5  = m_Data.SectBefore.SectI.Size[4];
		double B6  = m_Data.SectBefore.SectI.Size[5];
		double H   = m_Data.SectBefore.SectI.Size[6];

		if(Data.nDeckPos == 0) // Top
		{
			if     (Data.nDeckPart == 0) dDeckLength = B1; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B2; // Deck2
			else if(Data.nDeckPart == 2) dDeckLength = B3; // Deck3
		}
		else if(Data.nDeckPos == 1 || Data.nDeckPos == 2)
		{
			dDeckLength = H;
		}
		else
		{
			if     (Data.nDeckPart == 0) dDeckLength = B4; // Deck1
			else if(Data.nDeckPart == 1) dDeckLength = B5; // Deck2
			else if(Data.nDeckPart == 2) dDeckLength = B6; // Deck3
		}

		if(dSpacing > dDeckLength)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_LENGTH_ERROR));
			return FALSE;
		}
	}
	else if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL   ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		int nDeckPartNum = m_cmbDeckPart.GetCount();

		if(m_Data.SectBefore.nFlangeShape == 0 || m_Data.SectBefore.nFlangeShape == 1 || m_Data.SectBefore.nFlangeShape == 2)
		{
			double B1  = m_Data.SectBefore.SectI.Size[0] ;
			double B2  = m_Data.SectBefore.SectI.Size[1] ;
			double B3  = m_Data.SectBefore.SectI.Size[2] ;
			double B4  = m_Data.SectBefore.SectI.Size[3] ;
			double B5  = m_Data.SectBefore.SectI.Size[4] ;
			double B6  = m_Data.SectBefore.SectI.Size[5] ;
			double Br1 = m_Data.SectBefore.SectI.Size[6] ;
			double Br2 = m_Data.SectBefore.SectI.Size[7] ;
			double Br3 = m_Data.SectBefore.SectI.Size[8] ;
			double Br4 = m_Data.SectBefore.SectI.Size[9] ;
			double Br5 = m_Data.SectBefore.SectI.Size[10];
			double H   = m_Data.SectBefore.SectI.Size[11];
			double tf1 = m_Data.SectBefore.SectI.Size[12];
			double tw1 = m_Data.SectBefore.SectI.Size[13];
			double tf2 = m_Data.SectBefore.SectI.Size[14];
			double tw2 = m_Data.SectBefore.SectI.Size[15];


			if(Data.nDeckPos == 0) // Top
			{
				if(m_Data.SectBefore.nCellNum == 0 && m_Data.SectBefore.nCellType == 0)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B1;
					else if(Data.nDeckPart == 1)                dDeckLength = B2;
					else {ASSERT(0); dDeckLength = B1;}
				}
				else if(m_Data.SectBefore.nCellNum == 0 && m_Data.SectBefore.nCellType == 1)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = Br2;
					else if(Data.nDeckPart == 1)                dDeckLength = Br1;
					else {ASSERT(0); dDeckLength = Br1;}
				}
				else if(m_Data.SectBefore.nCellNum == 1)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B1;
					else if(Data.nDeckPart == 1)                dDeckLength = B2+Br2;
					else if(Data.nDeckPart == 2)                dDeckLength = Br1;
					else
					{
						ASSERT(0);
						dDeckLength = B1;
					}
				}
				else
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B1;
					else if(Data.nDeckPart == nDeckPartNum - 1) dDeckLength = Br1;
					else if(Data.nDeckPart == 1)                dDeckLength = B2;
					else if(Data.nDeckPart == nDeckPartNum - 2) dDeckLength = Br2;
					else dDeckLength = B6;
				}
			}
			else if(Data.nDeckPos == 1) // Web
			{
				dDeckLength = H + tw1 + tw2;
			}
			else if(Data.nDeckPos == 2) // Bot
			{
				if(m_Data.SectBefore.nCellNum == 0 && m_Data.SectBefore.nCellType == 0)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B4;
					else if(Data.nDeckPart == 1)                dDeckLength = B5;
					else 
					{
						dDeckLength = B4;
						ASSERT(0);
					}
				}
				else if(m_Data.SectBefore.nCellNum == 0 && m_Data.SectBefore.nCellType == 1)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = Br5;
					else if(Data.nDeckPart == 1)                dDeckLength = Br4;
					else 
					{
						dDeckLength = Br5;
						ASSERT(0);
					}
				}
				else if(m_Data.SectBefore.nCellNum == 1)
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B4;
					else if(Data.nDeckPart == 1)                dDeckLength = B5+Br5;
					else if(Data.nDeckPart == 2)                dDeckLength = Br4;
					else
					{
						ASSERT(0);
						dDeckLength = B1;
					}
				}
				else
				{
					if(Data.nDeckPart == 0)                     dDeckLength = B4;
					else if(Data.nDeckPart == nDeckPartNum - 1) dDeckLength = Br4;
					else if(Data.nDeckPart == 1)                dDeckLength = B5;
					else if(Data.nDeckPart == nDeckPartNum - 2) dDeckLength = Br5;
					else dDeckLength = B6;
				}        
			}
			else if(Data.nDeckPos == 3) // Side Flange
			{
				ASSERT(m_Data.SectBefore.nFlangeShape == 2);

				double rh1  = m_Data.SectBefore.SectI.Size[16]; 
				double rh2  = m_Data.SectBefore.SectI.Size[17]; 
				double rh3  = m_Data.SectBefore.SectI.Size[18]; 
				double rb   = m_Data.SectBefore.SectI.Size[19]; 
				double rt1  = m_Data.SectBefore.SectI.Size[20]; 
				double rt2  = m_Data.SectBefore.SectI.Size[21]; 
				double rhr1 = m_Data.SectBefore.SectI.Size[22]; 
				double rhr2 = m_Data.SectBefore.SectI.Size[23]; 
				double rhr3 = m_Data.SectBefore.SectI.Size[24]; 
				double rbr  = m_Data.SectBefore.SectI.Size[25]; 
				double rtr1 = m_Data.SectBefore.SectI.Size[26]; 
				double rtr2 = m_Data.SectBefore.SectI.Size[27]; 

				double Top = 0.0;
				double sideB = 0.0;
				double sideH = 0.0;
				double sideBr = 0.0;
				double sideHr = 0.0;

				if(nCellNum == 0)
				{
					if(nCellType == 0) // Left
					{
						if(H > 0.0)
						{
							sideB = B1-rb-rt1+((B3*rh3)/H);
							sideH = rh3-rh1;
						}
						else
						{
							return FALSE;
						}

						dDeckLength = sideB;
					}
					else // Right
					{
						if(H > 0.0)
						{
							sideBr = Top+Br2+Br1-rbr-rtr1+((Br3*rhr3)/H);
							sideHr = rhr3-rhr1;
						}

						dDeckLength = sideBr;
					}
				}
				else
				{
					if(H > 0.0)
					{
						sideB = B1-rb-rt1+((B3*rh3)/H);
						sideH = rh3-rh1;
						
						sideBr = Br1-rbr-rtr1+((Br3*rhr3)/H);
						sideHr = rhr3-rhr1;
					}
					else
					{
						return FALSE;
					}

					if     (Data.nDeckPart == 0) { dDeckLength = sideB;   } // Deck 1
					else if(Data.nDeckPart == 1) { dDeckLength = sideBr;  } // Deck 2
					else                             { ASSERT(0); return FALSE;  }
				}
			}
		}
		else if(m_Data.SectBefore.nFlangeShape == 3)
		{
			double B1=0.0, B2=0.0, B3=0.0, B4=0.0, B5=0.0, B6=0.0;
			double Br1=0.0, Br2=0.0, Br3=0.0, Br4=0.0, Br5=0.0;
			double H=0.0, tf1=0.0, tf2=0.0, tw1=0.0, tw2=0.0;
			double rh=0.0, rb=0.0, rt=0.0, rhr=0.0, rbr=0.0, rtr=0.0;
			double rh1=0.0, rh2=0.0, rh3=0.0, rt1=0.0, rt2=0.0, rhr1=0.0, rhr2=0.0, rhr3=0.0, rtr1=0.0, rtr2=0.0;
			double jh1=0.0, jh2=0.0, jb1=0.0, jb2=0.0, jhr1=0.0, jhr2=0.0, jbr1=0.0, jbr2=0.0;
			BOOL   J1=FALSE, J2=FALSE, Jr1=FALSE, Jr2=FALSE;

			double sideB  = 0.0;
			double sideH  = 0.0;
			double sideBr = 0.0;
			double sideHr = 0.0;

			double rt2p, rtr2p;


			if(nFlangeShape == 0 ||  // General 
				nFlangeShape == 1 ||  // Reinforced-1
				nFlangeShape == 2)    // Reinforced-2
			{
				B1  = m_Data.SectBefore.SectI.Size[0] ;
				B2  = m_Data.SectBefore.SectI.Size[1] ;
				B3  = m_Data.SectBefore.SectI.Size[2] ;
				B4  = m_Data.SectBefore.SectI.Size[3] ;
				B5  = m_Data.SectBefore.SectI.Size[4] ;
				B6  = m_Data.SectBefore.SectI.Size[5] ;
				Br1 = m_Data.SectBefore.SectI.Size[6] ;
				Br2 = m_Data.SectBefore.SectI.Size[7] ;
				Br3 = m_Data.SectBefore.SectI.Size[8] ;
				Br4 = m_Data.SectBefore.SectI.Size[9] ;
				Br5 = m_Data.SectBefore.SectI.Size[10];
				H   = m_Data.SectBefore.SectI.Size[11];
				tf1 = m_Data.SectBefore.SectI.Size[12];
				tw1 = m_Data.SectBefore.SectI.Size[13];
				tf2 = m_Data.SectBefore.SectI.Size[14];
				tw2 = m_Data.SectBefore.SectI.Size[15];

				if(nFlangeShape == 1)
				{
					rh  = m_Data.SectBefore.SectI.Size[16];
					rt  = m_Data.SectBefore.SectI.Size[17];
					rb  = m_Data.SectBefore.SectI.Size[18];
					rhr = m_Data.SectBefore.SectI.Size[19];
					rtr = m_Data.SectBefore.SectI.Size[20];
					rbr = m_Data.SectBefore.SectI.Size[21];
				}
				else if(nFlangeShape == 2)
				{
					rh1  = m_Data.SectBefore.SectI.Size[16];
					rh2  = m_Data.SectBefore.SectI.Size[17];
					rh3  = m_Data.SectBefore.SectI.Size[18];
					rb   = m_Data.SectBefore.SectI.Size[19];
					rt1  = m_Data.SectBefore.SectI.Size[20];
					rt2  = m_Data.SectBefore.SectI.Size[21];
					rhr1 = m_Data.SectBefore.SectI.Size[22];
					rhr2 = m_Data.SectBefore.SectI.Size[23];
					rhr3 = m_Data.SectBefore.SectI.Size[24];
					rbr  = m_Data.SectBefore.SectI.Size[25];
					rtr1 = m_Data.SectBefore.SectI.Size[26];
					rtr2 = m_Data.SectBefore.SectI.Size[27];
				}
			}
			else if(nFlangeShape == 3) // Closed
			{
				B1   = m_Data.SectBefore.SectI.Size[0] ;
				B2   = m_Data.SectBefore.SectI.Size[1] ;
				B3   = m_Data.SectBefore.SectI.Size[2] ;
				B4   = m_Data.SectBefore.SectI.Size[3] ;
				Br1  = m_Data.SectBefore.SectI.Size[4] ;
				Br2  = m_Data.SectBefore.SectI.Size[5] ;
				Br3  = m_Data.SectBefore.SectI.Size[6] ;
				H    = m_Data.SectBefore.SectI.Size[7] ;
				tf1  = m_Data.SectBefore.SectI.Size[8] ;
				tw1  = m_Data.SectBefore.SectI.Size[9] ;
				tf2  = m_Data.SectBefore.SectI.Size[10];
				tw2  = m_Data.SectBefore.SectI.Size[11];
				jh1  = m_Data.SectBefore.SectI.Size[12];
				jb1  = m_Data.SectBefore.SectI.Size[13];
				jh2  = m_Data.SectBefore.SectI.Size[14];
				jb2  = m_Data.SectBefore.SectI.Size[15];
				jhr1 = m_Data.SectBefore.SectI.Size[16];
				jbr1 = m_Data.SectBefore.SectI.Size[17];
				jhr2 = m_Data.SectBefore.SectI.Size[18];
				jbr2 = m_Data.SectBefore.SectI.Size[19];

				J1  = m_Data.SectBefore.nJoint & 0x00000001;                                
				J2  = m_Data.SectBefore.nJoint & 0x00000002;
				Jr1 = m_Data.SectBefore.nJoint & 0x00000004;
				Jr2 = m_Data.SectBefore.nJoint & 0x00000008;
			}
			else
			{
				ASSERT(0);
				return FALSE;
			}

			if(nFlangeShape == 0 || nFlangeShape == 1 || nFlangeShape == 2)
			{
				if(nCellNum == 0)
				{
					if(nCellType == 0) // Left
					{
						if(nFlangeShape == 2)
						{           
							if(H > 0.0)
							{
								sideB = B1-rb-rt1+((B3*rh3)/H);
								sideH = rh3-rh1;
							}
						}				
					}
					else // Right
					{
						double Top = Br5+Br3-Br2;
						double Bot = Br2-Br5-Br3;
						if(nFlangeShape == 2)
						{
							if(H > 0.0)
							{
								sideBr = Top+Br2+Br1-rbr-rtr1+((Br3*rhr3)/H);
								sideHr = rhr3-rhr1;
							}
						}
					}
				}
				else
				{
					double MaxTop = B1+B2+((nCellNum-2)*B6)+Br2+Br1;
					double MaxBot = B4+B5+((nCellNum-2)*B6)+Br5+Br4;

					if(nFlangeShape == 2)
					{          
						if(H > 0.0)
						{
							sideB = B1-rb-rt1+((B3*rh3)/H);
							sideH = rh3-rh1;

							if(sideB > 0.0)
							{
								rt2p = rt2/sideB * sqrt(sideB*sideB + sideH*sideH);
							}			

							sideBr = Br1-rbr-rtr1+((Br3*rhr3)/H);
							sideHr = rhr3-rhr1;

							if(sideBr > 0.0)
							{
								rtr2p = rtr2/sideBr * sqrt(sideBr*sideBr + sideHr*sideHr);
							}				
						}
					}
				}
			}

			if(nFlangeShape == 0 || nFlangeShape == 1 || nFlangeShape == 2)
			{
				if(nCellNum == 0) // 0 Cell 
				{
					if(nCellType == 0) // Left
					{
						if(Data.nDeckPos == 0) // Top Flange
						{
							if     (Data.nDeckPart == 0) { dDeckLength = B1;      } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = B2;      } // Deck 2
							else                             { ASSERT(0);  }
						}
						else if(Data.nDeckPos == 1) // Web
						{
							dDeckLength = H;
						}
						else if(Data.nDeckPos == 2) // Bot Flange
						{
							if     (Data.nDeckPart == 0) { dDeckLength = B4;      } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = B5;      } // Deck 2
							else                             { ASSERT(0);  }
						}
						else if(Data.nDeckPos == 3) // Side Flange
						{
							dDeckLength = sideB;
						}
						else
						{
							ASSERT(0);
						}
					}
					else // Right
					{
						if(Data.nDeckPos == 0) // Top Flange
						{
							if     (Data.nDeckPart == 0) { dDeckLength = Br2;     } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = Br1;     } // Deck 2
							else                             { ASSERT(0); }
						}
						else if(Data.nDeckPos == 1) // Web
						{
							dDeckLength = H;
						}
						else if(Data.nDeckPos == 2) // Bot Flange
						{
							if     (Data.nDeckPart == 0) { dDeckLength = Br5;     } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = Br4;     } // Deck 2
							else                             { ASSERT(0); }
						}
						else if(Data.nDeckPos == 3) // Side Flange
						{
							dDeckLength = sideBr;
						}
						else
						{
							ASSERT(0);
						}
					}
				}
				else
				{
					if(Data.nDeckPos == 0) // Top Flange
					{
						if(nCellNum == 1)
						{
							if     (Data.nDeckPart == 0) { dDeckLength = B1;      } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = B2+Br2;  } // Deck 2
							else if(Data.nDeckPart == 2) { dDeckLength = Br1;     } // Deck 3
							else                             { ASSERT(0);  }
						}
						else
						{
							if     (Data.nDeckPart == 0)            { dDeckLength = B1;  } // Deck 1
							else if(Data.nDeckPart == 1)            { dDeckLength = B2;  } // Deck 2
							else if(Data.nDeckPart == (nCellNum))   { dDeckLength = Br2; } // Deck n-1
							else if(Data.nDeckPart == (nCellNum+1)) { dDeckLength = Br1; } // Deck n
							else                                        { dDeckLength = B6;  } // 반복되는 Deck
						}					
					}
					else if(Data.nDeckPos == 1) // Web
					{
						dDeckLength = H;
					}
					else if(Data.nDeckPos == 2) // Bot Flange
					{
						if(nCellNum == 1)
						{
							if     (Data.nDeckPart == 0) { dDeckLength = B4;      } // Deck 1
							else if(Data.nDeckPart == 1) { dDeckLength = B5+Br5;  } // Deck 2
							else if(Data.nDeckPart == 2) { dDeckLength = Br4;     } // Deck 3
							else                             { ASSERT(0);  }
						}
						else
						{
							if     (Data.nDeckPart == 0)            { dDeckLength = B4;  } // Deck 1
							else if(Data.nDeckPart == 1)            { dDeckLength = B5;  } // Deck 2
							else if(Data.nDeckPart == (nCellNum))   { dDeckLength = Br5; } // Deck n-1
							else if(Data.nDeckPart == (nCellNum+1)) { dDeckLength = Br4; } // Deck n
							else                                        { dDeckLength = B6;  } // 반복되는 Deck
						}	
					}
					else if(Data.nDeckPos == 3) // Side Flange
					{
						if     (Data.nDeckPart == 0) { dDeckLength = sideB;   } // Deck 1
						else if(Data.nDeckPart == 1) { dDeckLength = sideBr;  } // Deck 2
						else                             { ASSERT(0);  }
					}
					else
					{
						ASSERT(0);
					}
				}
			}
			else if(nFlangeShape == 3)
			{
				if(nCellNum == 0) // 0 Cell 
				{
					if(nCellType == 0) // Left
					{
						if(Data.nDeckPos == 0) // Top Flange
						{
							dDeckLength = B1;
						}
						else if(Data.nDeckPos == 1) // Web
						{
							if(J2)
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jb1-B2)*(jb1-B2) + (tf1+jh1)*(tf1+jh1));   }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( (jb1-jb2)*(jb1-jb2) + (jh2-jh1)*(jh2-jh1)); }
								else if(Data.nDeckPart == 2) { dDeckLength = sqrt( jb2*jb2 + (H+tf2-jh2)*(H+tf2-jh2));         }
								else                             { ASSERT(0);                                           }
							}
							else
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jb1-B2)*(jb1-B2) + (tf1+jh1)*(tf1+jh1)); }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( jb1*jb1 + (H+tf2-jh1)*(H+tf2-jh1));       }
								else                             { ASSERT(0);                                        }
							}
						}
						else if(Data.nDeckPos == 2) // Bot Flange
						{
							dDeckLength = B3;
						}
						else
						{
							ASSERT(0);
						}
					}
					else // Right
					{
						if(Data.nDeckPos == 0) // Top Flange
						{
							dDeckLength = Br1;
						}
						else if(Data.nDeckPos == 1) // Web
						{
							if(Jr2)
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jbr1-Br2)*(jbr1-Br2) + (tf1+jh1)*(tf1+jh1));       }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( (jbr1-jbr2)*(jbr1-jbr2) + (jhr2-jhr1)*(jhr2-jhr1)); }
								else if(Data.nDeckPart == 2) { dDeckLength = sqrt( jbr2*jbr2 + (H+tf2-jhr2)*(H+tf2-jhr2));             }
								else                             { ASSERT(0);                                                   }
							}
							else
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jbr1-Br2)*(jbr1-Br2) + (tf1+jh1)*(tf1+jh1)); }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( jbr1*jbr1 + (H+tf2-jhr1)*(H+tf2-jhr1));       }
								else                             { ASSERT(0);                                             }
							}
						}
						else if(Data.nDeckPos == 2) // Bot Flange
						{
							dDeckLength = Br3;
						}
						else
						{
							ASSERT(0);
						}
					}
				}
				else
				{
					int nLeftWeb  = J2?  3 : 2;
					int nRightWeb = Jr2? 3 : 2;

					if(Data.nDeckPos == 0) // Top Flange
					{
						if(nCellNum == 1)
						{
							dDeckLength = B1+Br1;
						}
						else
						{
							if     (Data.nDeckPart == 0)            { dDeckLength = B1;  } // Deck 1
							else if(Data.nDeckPart == (nCellNum-1)) { dDeckLength = Br1; } // Deck n
							else                                        { dDeckLength = B4;  } // 반복되는 Deck
						}					
					}
					else if(Data.nDeckPos == 1) // Web
					{
						if(Data.nDeckPart < nLeftWeb)
						{
							if(J2)
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jb1-B2)*(jb1-B2) + (tf1+jh1)*(tf1+jh1));   }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( (jb1-jb2)*(jb1-jb2) + (jh2-jh1)*(jh2-jh1)); }
								else if(Data.nDeckPart == 2) { dDeckLength = sqrt( jb2*jb2 + (H+tf2-jh2)*(H+tf2-jh2));         }
								else                             { ASSERT(0);                                           }
							}
							else
							{
								if     (Data.nDeckPart == 0) { dDeckLength = sqrt( (jb1-B2)*(jb1-B2) + (tf1+jh1)*(tf1+jh1)); }
								else if(Data.nDeckPart == 1) { dDeckLength = sqrt( jb1*jb1 + (H+tf2-jh1)*(H+tf2-jh1));       }
								else                             { ASSERT(0);                                         }
							}
						}
						else if(Data.nDeckPart >= (nLeftWeb+nCellNum-1))
						{
							int nDeckPart = Data.nDeckPart - nLeftWeb - (nCellNum-1);

							if(Jr2)
							{
								if     (nDeckPart == 0) { dDeckLength = sqrt( (jbr1-Br2)*(jbr1-Br2) + (tf1+jh1)*(tf1+jh1));       }
								else if(nDeckPart == 1) { dDeckLength = sqrt( (jbr1-jbr2)*(jbr1-jbr2) + (jhr2-jhr1)*(jhr2-jhr1)); }
								else if(nDeckPart == 2) { dDeckLength = sqrt( jbr2*jbr2 + (H+tf2-jhr2)*(H+tf2-jhr2));             }
								else                    { ASSERT(0);  }
							}
							else
							{
								if     (nDeckPart == 0) { dDeckLength = sqrt( (jbr1-Br2)*(jbr1-Br2) + (tf1+jh1)*(tf1+jh1)); }
								else if(nDeckPart == 1) { dDeckLength = sqrt( jbr1*jbr1 + (H+tf2-jhr1)*(H+tf2-jhr1));       }
								else                    { ASSERT(0);                                            }
							}														
						}
						else
						{
							dDeckLength = H;
						}
					}
					else if(Data.nDeckPos == 2) // Bot Flange
					{
						if(nCellNum == 1)
						{
							dDeckLength = B3+Br3;
						}
						else
						{
							if     (Data.nDeckPart == 0)            { dDeckLength = B3;  } // Deck 1
							else if(Data.nDeckPart == (nCellNum-1)) { dDeckLength = Br3; } // Deck n
							else                                        { dDeckLength = B4;  } // 반복되는 Deck
						}	
					}
					else
					{
						ASSERT(0);
					}
				}
			}
		}

		if(dSpacing > dDeckLength)
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_SOD_STIFF_LENGTH_ERROR));
			return FALSE;
		}
	}
	else
	{
		ASSERT(0);
	}

	return TRUE;
}

void CCMSectSODDeckStiffenerDlg::UpdateSectView()
{
	m_wndSecView.Invalidate();
}

BEGIN_MESSAGE_MAP(CCMSectSODDeckStiffenerDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectSODDeckStiffenerDlg)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_DEF_STIFF_BTN,    OnCMSectSODDefineStiffenerBtn)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_SOD_STIFFENER_DECK_POS_CMB,  OnCMSectSODDeckPosCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_SOD_STIFFENER_DECK_PART_CMB, OnCMSectSODDeckPartCmb)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_REF_POS_RDO1,     OnCMSectSODRefPosRdo)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_REF_POS_RDO2,     OnCMSectSODRefPosRdo)
	ON_EN_CHANGE (IDC_CMD_SECT_SOD_STIFFENER_NUM_EDT,          OnCMSectSODNumberEdt)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_ADD_BTN,          OnCMSectSODAddBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_MOD_BTN,          OnCMSectSODModBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_DEL_BTN,          OnCMSectSODDelBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_SOD_STIFFENER_COPY_BTN,         OnCMSectSODCopyBtn)

	ON_NOTIFY(NM_DBLCLK, IDC_CMD_SECT_SOD_STIFFENER_DEF_STIFF_LST, OnDblclkDefStiffList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SOD_STIFFENER_LST,      OnCMItemchangedSODList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerDlg message handlers

BOOL CCMSectSODDeckStiffenerDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if (CTestEnvMgr::IsTestEnvST(_T("LTypeTest"), _T("yes")))
	{
		GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_COPY_BTN)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SECT_SOD_STIFFENER_COPY_BTN)->ShowWindow(FALSE);
	}

	InitUnit();
	InitCombo();
	InitGrid();
	InitView();
	InitSpin();

	SetHeaderTitle();
	SetHeaderTitle_DefStiff();

	Data2Dlg();
	ChangeDeckName();

	UpdateData(FALSE);

	ChangeBitmap();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectSODDeckStiffenerDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODDeckPosCmb()
{
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);

	UpdateData(TRUE);

	ChangeCombo();
	ChangeDeckName();
	ChangeBitmap();
	ChangeText();

	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);

	m_pGrid->InitStiffenerCol(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);

	int nDeckPart = m_cmbDeckPart.GetCurSel();
	m_pGrid->InitStiffenerPosCol(m_Data, (int)nDeckPos, nDeckPart, m_cmbDeckPart.GetCount());

	m_pGrid->InitStiffenerDir(m_Data, (int)nDeckPos);

	////////////////////////////////////////////////////////////////////////////
	// Default Setting
	BOOL bChange = FALSE;

	if(nDeckPos != m_nDeckPos) bChange = TRUE;
	m_nDeckPos = nDeckPos;	  

	if(bChange)
	{
		SetDefaultStiffPos(nDeckPos, nDeckPart, aStiffData);
	}
	////////////////////////////////////////////////////////////////////////////	

	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData, nDeckPos, aStiffData.GetSize());
}

void CCMSectSODDeckStiffenerDlg::SetDefaultStiffPos(DWORD nDeckPos, int nDeckPart, CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D &> &aStiffData)
{
	int nDefStiffPos;
	CCMSectSODDeckStiffenerDlgUtil::GetDefStiffPos(m_Data, nDeckPos, nDeckPart, m_cmbDeckPart.GetCount(), &nDefStiffPos);
	
	for (int i = 0; i < aStiffData.GetSize(); i++)
	{
		aStiffData[i].nStiffPos = nDefStiffPos;
	}
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODDeckPartCmb()
{
	ChangeDeckName();
	ChangeBitmap();
	
	int nDeckPart = m_cmbDeckPart.GetCurSel();
	
	BOOL bChange = FALSE;
	if(nDeckPart != m_nDeckPart) bChange = TRUE;
	m_nDeckPart = nDeckPart;	  

	if(bChange)
	{
		DWORD nDeckPos;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);

		CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
		m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);

		m_pGrid->InitStiffenerPosCol(m_Data, (int)nDeckPos, nDeckPart, m_cmbDeckPart.GetCount());
		SetDefaultStiffPos(nDeckPos, nDeckPart, aStiffData);
		m_pGrid->InitStiffenerDir(m_Data, (int)nDeckPos);

		m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData, nDeckPos, aStiffData.GetSize());
	}
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODDefineStiffenerBtn()
{
	CCMSectSODStiffenerDlg dlg;
	dlg.m_Data = m_Data;
	dlg.m_nSelIndex = -1;
	dlg.m_nDlgType = 0;

	if(dlg.DoModal() == IDOK)
	{
		m_Data = dlg.m_Data;

		CArray<CString, CString> aShapeName; aShapeName.RemoveAll();
		for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++i)
		{
			aShapeName.Add(m_Data.SectBefore.SectI.SODStiffener.aStiffShape[i].strName);
		}

		CArray<int, int> aDelIndex; aDelIndex.RemoveAll();
		for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); ++i)
		{
			BOOL bDiff = TRUE;
			for(int j=0; j<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); ++j)
			{
				for(int k=0; k<aShapeName.GetSize(); ++k)
				{
					if(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].strShapeName == aShapeName[k])
					{
						bDiff = FALSE;
					}
				}
			}

			if(bDiff) aDelIndex.Add(i);
		}

		int nSize = aDelIndex.GetSize();
		if(nSize > 0) qsort(aDelIndex.GetData(), nSize, sizeof(int), CCompFunc::intDesc);
		
		for(int i=0; i<nSize; ++i)
		{
			m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.RemoveAt(aDelIndex[i]);
		}

		Data2Dlg();
		m_wndSecView.Invalidate();
	}
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODRefPosRdo()
{
	ChangeBitmap();
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODNumberEdt()
{
	if (m_edtNumber.GetSafeHwnd() == NULL || !IsWindow(m_edtNumber.GetSafeHwnd())) return;	

	T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
	
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData_T;
	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);
	StiffSubD.aStiffData.Copy(aStiffData);
	
	UpdateData(TRUE);

	int i=0;
	int nNum = m_edtNumber.GetEditValueInt();
	int nSize = StiffSubD.aStiffData.GetSize();

	aStiffData_T.SetSize(nNum);
	
	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	int nDeckPart = m_cmbDeckPart.GetCurSel();

	////////////////////////////////////////////////////////////////////////////
	// Default Setting
	CString strStiffNameAtType;
	CCMSectSODDeckStiffenerDlgUtil::GetDefStiffName(m_Data, nDeckPos, nDeckPart, m_cmbDeckPart.GetCount(), &strStiffNameAtType);
	int nStiffPos;
	CCMSectSODDeckStiffenerDlgUtil::GetDefStiffPos(m_Data, nDeckPos, nDeckPart, m_cmbDeckPart.GetCount(), &nStiffPos);

	for(i=0; i<nNum; ++i) 
	{
		aStiffData_T[i].Initialize();
		aStiffData_T[i].nStiffPos = nStiffPos;

		// Stiffener 이름 자동 부여 (MQC Alpha/Beta : 4870-18)
		CString strStiffName;
		strStiffName.Format(_T("%s%d"), strStiffNameAtType, i + 1); 
		aStiffData_T[i].strStiffName = strStiffName;
	}
	////////////////////////////////////////////////////////////////////////////

	if(nSize > nNum)
	{
		for(i=0; i<nNum; ++i) aStiffData_T.SetAt(i, aStiffData[i]);
	}
	else if(nSize < nNum)
	{
		for(i=0; i<nSize; ++i) aStiffData_T.SetAt(i, aStiffData[i]);	 
	}
	else
	{
		// Nothing
	}

	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData_T, nDeckPos, nNum);

	m_wndSecView.Invalidate();
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODAddBtn()
{
	UpdateData(TRUE);

	T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
	Dlg2Data_StiffSub(StiffSubD);
	
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);
	StiffSubD.aStiffData.Copy(aStiffData);

	if(!ErrorCheck_Stiffener(StiffSubD, TRUE)) return;

	int nCount = m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize();
	
	InsertItem(nCount, StiffSubD);

	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Add(StiffSubD);

	UpdateSectView();
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODModBtn()
{
	UpdateData(TRUE);
	
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}
	
	T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
	Dlg2Data_StiffSub(StiffSubD);
	
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);
	StiffSubD.aStiffData.Copy(aStiffData);

	if(!ErrorCheck_Stiffener(StiffSubD, FALSE)) return;

	ModifyItem(iItem, StiffSubD);

	Data2Dlg_StiffSub(StiffSubD);
	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.SetAt(iItem, StiffSubD);
	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, StiffSubD.aStiffData, StiffSubD.nDeckPos, StiffSubD.nNum);

	ChangeBitmap();
	ChangeText();

	UpdateSectView();	
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODDelBtn()
{
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}

	m_lstStiffener.DeleteItem(iItem);
	
	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.RemoveAt(iItem);
		
	int nCount = m_lstStiffener.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstStiffener.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
	
	UpdateSectView();	
}

void CCMSectSODDeckStiffenerDlg::OnCMSectSODCopyBtn()
{
	UpdateData(TRUE);

	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_STIFFENER_SELECTED_ERROR));
		return;
	}

	// 선택한 보강재
	T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
	Dlg2Data_StiffSub(StiffSubD);

	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, aStiffData);
	StiffSubD.aStiffData.Copy(aStiffData);

	if(!ErrorCheck_Stiffener(StiffSubD, FALSE)) return;

	// Copy 대화창에 데이터 넘기기
	CArray<int, int> arDeckPart;
	std::vector<CString> aStrCbxDeckPartItem;
	CCMSectSODDeckStiffenerDlgUtil::GetCbxDeckPartItem(m_Data, StiffSubD.nDeckPos, &aStrCbxDeckPartItem);
	for (int i = 0; i < aStrCbxDeckPartItem.size(); i++)
	{
		arDeckPart.Add(i);
	}

	CCMSectSODDeckStiffenerCopyDlg dlg;
	dlg.SetData(m_Data, StiffSubD.nDeckPos, StiffSubD.nDeckPart, arDeckPart);

	if(dlg.DoModal() == IDOK) 
	{
		CArray<int, int> arDeckSel;
		dlg.GetData(arDeckSel);

		BOOL bCopyMirror = dlg.m_bCopyMirror;

		BOOL bWeb2 = FALSE;
		if(m_Data.nStype == D_SECT_TYPE_STLG_B || 
			 m_Data.nStype == D_SECT_TYPE_COMPO_STLG_B ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
			(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
		{
			if(StiffSubD.nDeckPos==1 || StiffSubD.nDeckPos==2) bWeb2 = TRUE;
		}
		else if(m_Data.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
					 (m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
		{
			if(StiffSubD.nDeckPos==1 || StiffSubD.nDeckPos==2) bWeb2 = TRUE;
		}

		if(bWeb2) CopytoPos(bCopyMirror, arDeckSel, StiffSubD);
		else      CopytoPart(bCopyMirror, arDeckSel, StiffSubD);      

		MakeItemEx();
	}

	UpdateSectView();	
}


void CCMSectSODDeckStiffenerDlg::CopytoPart(BOOL bCopyMirror, CArray<int, int>& arDeckPartSel, T_SECT_SOD_STIFF_SUB_D& StiffSubDOrg)
{
	CArray<T_SECT_SOD_STIFF_SUB_D, T_SECT_SOD_STIFF_SUB_D&> aSODStiffSubMod, aSODStiffSubOrg; 
	aSODStiffSubMod.Copy(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub);
	aSODStiffSubOrg.Copy(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub);

	for (int i = 0; i < aSODStiffSubMod.GetSize(); i++)
	{
		T_SECT_SOD_STIFF_SUB_D StiffSubData = aSODStiffSubMod[i]; 
		if(StiffSubData.nDeckPos==StiffSubDOrg.nDeckPos)
		{
			for (int j = 0; j < arDeckPartSel.GetSize(); j++)
			{
				if(StiffSubData.nDeckPart==arDeckPartSel[j])
				{
					aSODStiffSubMod.RemoveAt(i); 
					i -= 1;
				}
			}
		}
	}

	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.RemoveAll();
	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Copy(aSODStiffSubMod);

	T_SECT_SOD_STIFF_SUB_D StiffSubD = StiffSubDOrg;

	// 대칭 반영
	int nNum = StiffSubDOrg.aStiffData.GetSize();
	if(bCopyMirror)
	{
		StiffSubD.nRefPos = (StiffSubD.nRefPos==0)? 1 : 0; 

		for(int j=0; j<nNum; ++j) 
		{
			int nStiffDir = StiffSubD.aStiffData[j].nStiffDir;
			StiffSubD.aStiffData[j].nStiffDir = (nStiffDir==0)? 1 : 0;
		}
	}

	BOOL bMCell = FALSE;
	if(m_Data.nStype == D_SECT_TYPE_STLG_MCELL ||
		(m_Data.nStype == D_SECT_TYPE_TAPERED && m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		bMCell = TRUE;
	}

	// 이름 변경
	for (int i = 0; i < arDeckPartSel.GetSize(); i++)
	{
		StiffSubD.nDeckPart = arDeckPartSel[i];
		
		CString strDeckName;
		CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, StiffSubD.nDeckPos, StiffSubD.nDeckPart, m_cmbDeckPart.GetCount(), &strDeckName);
		StiffSubD.strDeckName = strDeckName;

		CString strStiffNameAtType;
		CCMSectSODDeckStiffenerDlgUtil::GetDefStiffName(m_Data, StiffSubD.nDeckPos, StiffSubD.nDeckPart, m_cmbDeckPart.GetCount(), &strStiffNameAtType);
		for(int j=0; j<nNum; ++j) 
		{
			CString strStiffName;
			strStiffName.Format(_T("%s%d"), strStiffNameAtType, j + 1); 
			StiffSubD.aStiffData[j].strStiffName = strStiffName;     
			if(bMCell && StiffSubD.nDeckPos==1) //web
			{
				int nCellNum     = m_Data.SectBefore.nCellNum;
				int nFlangeShape = m_Data.SectBefore.nFlangeShape;
				int nCellType    = m_Data.SectBefore.nCellType;
				
				BOOL J2  = m_Data.SectBefore.nJoint & 0x00000002;
				BOOL Jr2 = m_Data.SectBefore.nJoint & 0x00000008;

				if(nCellNum == 0) // 0 Cell
				{
					if(nCellType == 0) // Left
					{
						StiffSubD.aStiffData[j].nStiffPos = 1; // Right
					}
					else // Right
					{
						StiffSubD.aStiffData[j].nStiffPos = 0; // Left
					}
				}
				else // N Cell
				{
					if(nFlangeShape==0 || nFlangeShape==1 || nFlangeShape==2)
					{
						if     (StiffSubD.nDeckPart==0)        StiffSubD.aStiffData[j].nStiffPos = 1; 
						else if(StiffSubD.nDeckPart==nCellNum) StiffSubD.aStiffData[j].nStiffPos = 0; 
						else                                   StiffSubD.aStiffData[j].nStiffPos = 2; 
					}
					else if(nFlangeShape==3)
					{
						int nLeftWeb  = J2?  3 : 2;
						int nRightWeb = Jr2? 3 : 2;
						if(StiffSubD.nDeckPart < nLeftWeb) // Left
						{
							StiffSubD.aStiffData[j].nStiffPos = 1; 
						}
						else if(StiffSubD.nDeckPart >= (nLeftWeb+nCellNum-1)) // Right
						{
							StiffSubD.aStiffData[j].nStiffPos = 0;
						}
						else
						{
							StiffSubD.aStiffData[j].nStiffPos = 2;
						}
					}
				}
			}
		}

		if(!ErrorCheck_Stiffener(StiffSubD, TRUE)) 
		{
			m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Copy(aSODStiffSubOrg);
			return;
		}
		m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Add(StiffSubD);
	}
}

void CCMSectSODDeckStiffenerDlg::CopytoPos(BOOL bCopyMirror, CArray<int, int>& arDeckPosSel, T_SECT_SOD_STIFF_SUB_D& StiffSubDOrg)
{
	CArray<T_SECT_SOD_STIFF_SUB_D, T_SECT_SOD_STIFF_SUB_D&> aSODStiffSubMod, aSODStiffSubOrg; 
	aSODStiffSubMod.Copy(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub);
	aSODStiffSubOrg.Copy(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub);

	for (int i = 0; i < aSODStiffSubMod.GetSize(); i++)
	{
		T_SECT_SOD_STIFF_SUB_D StiffSubData = aSODStiffSubMod[i]; 
		for (int j = 0; j < arDeckPosSel.GetSize(); j++)
		{
			if(StiffSubData.nDeckPos==arDeckPosSel[j])
			{
				aSODStiffSubMod.RemoveAt(i); 
				i -= 1;
			}
		}
	}

	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.RemoveAll();
	m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Copy(aSODStiffSubMod);

	T_SECT_SOD_STIFF_SUB_D StiffSubD = StiffSubDOrg;

	// 이름 변경
	int nNum = StiffSubD.aStiffData.GetSize();
	for (int i = 0; i < arDeckPosSel.GetSize(); i++)
	{
		StiffSubD.nDeckPos = arDeckPosSel[i];

		CString strDeckName;
		CCMSectSODDeckStiffenerDlgUtil::GetDeckName(m_Data, StiffSubD.nDeckPos, StiffSubD.nDeckPart, m_cmbDeckPart.GetCount(), &strDeckName);
		StiffSubD.strDeckName = strDeckName;

		CString strStiffNameAtType;
		CCMSectSODDeckStiffenerDlgUtil::GetDefStiffName(m_Data, StiffSubD.nDeckPos, StiffSubD.nDeckPart, m_cmbDeckPart.GetCount(), &strStiffNameAtType);
		for(int j=0; j<nNum; ++j) 
		{
			CString strStiffName;
			strStiffName.Format(_T("%s%d"), strStiffNameAtType, j + 1); 
			StiffSubD.aStiffData[j].strStiffName = strStiffName;     

			int nStiffPosOrg = StiffSubDOrg.aStiffData[j].nStiffPos;

			if     (nStiffPosOrg==0) StiffSubD.aStiffData[j].nStiffPos = 1;
			else if(nStiffPosOrg==1) StiffSubD.aStiffData[j].nStiffPos = 0;
			else if(nStiffPosOrg==2) StiffSubD.aStiffData[j].nStiffPos = 2;
			else ASSERT(0);
		}

		if(!ErrorCheck_Stiffener(StiffSubD, TRUE)) 
		{
			m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Copy(aSODStiffSubOrg);
			return;
		}
		m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.Add(StiffSubD);
	}
}

void CCMSectSODDeckStiffenerDlg::OnDblclkDefStiffList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstDefStiff.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	CCMSectSODStiffenerDlg dlg;
	dlg.m_Data = m_Data;
	dlg.m_nSelIndex = iItem;
	dlg.m_nDlgType = 0;
	
	if(dlg.DoModal() == IDOK)
	{
		m_Data = dlg.m_Data;
		
		CArray<CString, CString> aShapeName; aShapeName.RemoveAll();
		for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aStiffShape.GetSize(); ++i)
		{
			aShapeName.Add(m_Data.SectBefore.SectI.SODStiffener.aStiffShape[i].strName);
		}
		
		CArray<int, int> aDelIndex; aDelIndex.RemoveAll();
		for(int i=0; i<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetSize(); ++i)
		{
			BOOL bDiff = TRUE;
			for(int j=0; j<m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData.GetSize(); ++j)
			{
				for(int k=0; k<aShapeName.GetSize(); ++k)
				{
					if(m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub[i].aStiffData[j].strShapeName == aShapeName[k])
					{
						bDiff = FALSE;
					}
				}
			}
			
			if(bDiff) aDelIndex.Add(i);
		}
		
		int nSize = aDelIndex.GetSize();
		if(nSize > 0) qsort(aDelIndex.GetData(), nSize, sizeof(int), CCompFunc::intDesc);
		
		for(int i=0; i<nSize; ++i)
		{
			m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.RemoveAt(aDelIndex[i]);
		}

		Data2Dlg();
		m_wndSecView.Invalidate();
	}
	
	*pResult = 0;
}

void CCMSectSODDeckStiffenerDlg::OnCMItemchangedSODList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstStiffener.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	T_SECT_SOD_STIFF_SUB_D StiffSubD; StiffSubD.Initialize();
	StiffSubD = m_Data.SectBefore.SectI.SODStiffener.aSODStiffSub.GetAt(iItem);

	CDlgUtil::CobxSetCurSelItemData(m_cmbDeckPos, (DWORD)StiffSubD.nDeckPos);
	m_nDeckPos = StiffSubD.nDeckPos;

	m_nDeckPart = m_cmbDeckPart.GetCurSel();

	ChangeCombo();
	ChangeDeckName();
	Data2Dlg_StiffSub(StiffSubD);
	
	ChangeBitmap();
	ChangeText();

	m_pGrid->InitStiffenerCol(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);
	DWORD nDeckPos;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbDeckPos, nDeckPos);
	int nDeckPart = m_cmbDeckPart.GetCurSel();
	m_pGrid->InitStiffenerPosCol(m_Data, (int)nDeckPos, nDeckPart, m_cmbDeckPart.GetCount());
	m_pGrid->InitStiffenerDir(m_Data, (int)nDeckPos);
	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.SODStiffener.aStiffShape, StiffSubD.aStiffData, StiffSubD.nDeckPos, StiffSubD.nNum);
	
	*pResult = 0;
}
