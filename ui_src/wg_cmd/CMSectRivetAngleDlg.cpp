// CMSectRivetAngleDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectRivetAngleDlg.h"
#include "CMSectSODStiffenerDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_RIVET_ANGLE_SIZE_I 8

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleDlg dialog

CCMSectRivetAngleDlg::CCMSectRivetAngleDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectRivetAngleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectRivetAngleDlg)
	
	m_bSymmetric = TRUE;

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		m_bAngle[i] = TRUE;
	}
	
	m_bRivet = TRUE;

	//}}AFX_DATA_INIT

	m_Data.Initialize();

	//m_pGrid = new CCMSectRivetAngleGrid();
	m_pGrid = new CCMSectRivetAngleGridNew();
}

CCMSectRivetAngleDlg::~CCMSectRivetAngleDlg()
{
	if(m_pGrid) { delete m_pGrid; m_pGrid = NULL; }
}

void CCMSectRivetAngleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectRivetAngleDlg)
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_DEF_STIFF_LST,  m_lstDefStiff);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_SYM_CHK,        m_bSymmetric);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_1_CHK,    m_bAngle[0]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_2_CHK,    m_bAngle[1]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_3_CHK,    m_bAngle[2]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_4_CHK,    m_bAngle[3]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_5_CHK,    m_bAngle[4]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_6_CHK,    m_bAngle[5]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_7_CHK,    m_bAngle[6]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_ANGLE_8_CHK,    m_bAngle[7]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_1_CMB,    m_cmbAngle[0]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_2_CMB,    m_cmbAngle[1]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_3_CMB,    m_cmbAngle[2]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_4_CMB,    m_cmbAngle[3]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_5_CMB,    m_cmbAngle[4]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_6_CMB,    m_cmbAngle[5]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_7_CMB,    m_cmbAngle[6]);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_ANGLE_8_CMB,    m_cmbAngle[7]);
	DDX_Check  (pDX, IDC_CMD_SECT_RIVET_USE_CHK,        m_bRivet);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_GRID,           *m_pGrid);
	DDX_Control(pDX, IDC_CMD_SECT_RIVET_PIC,            m_ImgRivet);
	//}}AFX_DATA_MAP
}

void CCMSectRivetAngleDlg::InitCombo()
{
	int nStiff = m_Data.SectBefore.SectI.RivetStiff.aStiffShape.GetSize();

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		m_cmbAngle[i].ResetContent();

		for (int j = 0; j < nStiff; j++)
		{
			CDlgUtil::CobxAddItem(m_cmbAngle[i], m_Data.SectBefore.SectI.RivetStiff.aStiffShape[j].strName, j);
		}
	}
}

void CCMSectRivetAngleDlg::InitGrid()
{
	m_pGrid->Initialize();
}

void CCMSectRivetAngleDlg::InitView()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_RIVET_VIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetDataSource(&m_Data);
}


void CCMSectRivetAngleDlg::ChangeBitmap()
{
	UpdateData(TRUE);

//	UINT aBitmapID[] = { IDB_CMD_SEC_STIFFENER_SR };

	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\Riveted_Angle.svg"),
	};
	int nSelBitmap = 0;

	m_ImgRivet.SetImage(aImgPath_Sect[nSelBitmap]);
}

void CCMSectRivetAngleDlg::SetHeaderTitle_DefStiff()
{
	CString aTitle[] = {_LS(IDS_CMD_SECT_STIFFENER_SHAPE_NAME), _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE)};
	int nColWidth[2] = {120, 75};
	
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

void CCMSectRivetAngleDlg::MakeItemEx_DefStiff()
{
	m_lstDefStiff.DeleteAllItems();
	
	int nSize = m_Data.SectBefore.SectI.RivetStiff.aStiffShape.GetSize();
	if(nSize == 0) return;
	
	for(int i = 0; i < nSize; i++)
	{
		InsertItem_DefStiff(i, m_Data.SectBefore.SectI.RivetStiff.aStiffShape[i]);
	}
}

BOOL CCMSectRivetAngleDlg::InsertItem_DefStiff(int nIndex, T_STIFF_SHAPE_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;
	
	lvitem.iItem = nIndex;
	for(int i = 0; i < 2; i++)
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

CString CCMSectRivetAngleDlg::DataToStr_DefStiff(int i, T_STIFF_SHAPE_D &Data)
{
	CString str;
	
	if     (i == 0) str = Data.strName;
	else if(i == 1)
	{
		if     (Data.nType == 0) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_FLAT);
		else if(Data.nType == 1) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_TEE);
		else if(Data.nType == 2) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_URIB);
		else if(Data.nType == 3) str = _LS(IDS_CMD_SECT_STIFFENER_SHAPE_TYPE_L);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	
	return str;
}

void CCMSectRivetAngleDlg::EnableDisableCtrl()
{
	GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_1_CMB)->EnableWindow(m_bAngle[0]);
	GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_2_CMB)->EnableWindow(m_bAngle[1]);
	GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_3_CMB)->EnableWindow(m_bAngle[2]);
	GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_4_CMB)->EnableWindow(m_bAngle[3]);

	if (m_bSymmetric)
	{
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_5_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_6_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_7_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_8_CHK)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_5_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_6_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_7_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_8_CMB)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_5_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_6_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_7_CHK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_8_CHK)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_5_CMB)->EnableWindow(m_bAngle[4]);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_6_CMB)->EnableWindow(m_bAngle[5]);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_7_CMB)->EnableWindow(m_bAngle[6]);
		GetDlgItem(IDC_CMD_SECT_RIVET_ANGLE_8_CMB)->EnableWindow(m_bAngle[7]);
	}
}

void CCMSectRivetAngleDlg::Data2Dlg()
{	
	MakeItemEx_DefStiff();

	m_bSymmetric = m_Data.SectBefore.SectI.RivetStiff.bSymmetric;

	if (m_Data.SectBefore.SectI.RivetStiff.aStiffPosD.GetSize() != D_RIVET_ANGLE_SIZE_I)
	{
		m_Data.SectBefore.SectI.RivetStiff.aStiffPosD.RemoveAll();
		m_Data.SectBefore.SectI.RivetStiff.aStiffPosD.SetSize(D_RIVET_ANGLE_SIZE_I);

		for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
		{
			m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].Initialize();
		}
	}

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		m_bAngle[i] = m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].bUseStiff;

		if (m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].strShapeName.IsEmpty())
		{
			m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].Initialize();
			m_bAngle[i] = FALSE;

			if (m_Data.SectBefore.SectI.RivetStiff.aStiffShape.GetSize() > 0)
			{
				m_cmbAngle[i].SetCurSel(0);
			}

			continue;
		}

		BOOL bFind = FALSE;

		for (int j = 0; j < m_Data.SectBefore.SectI.RivetStiff.aStiffShape.GetSize(); j++)
		{
			if (m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].strShapeName == m_Data.SectBefore.SectI.RivetStiff.aStiffShape[j].strName)
			{
				CString strName = m_Data.SectBefore.SectI.RivetStiff.aStiffShape[j].strName;
				//m_cmbAngle[i].SetCurSel(j);
				CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[i], (DWORD&) j);
				bFind = TRUE;
				break;
			}
		}

		if (bFind) continue;

		m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].Initialize();
		m_bAngle[i] = FALSE;

		if (m_Data.SectBefore.SectI.RivetStiff.aStiffShape.GetSize() > 0)
		{
			m_cmbAngle[i].SetCurSel(0);
		}
	}

	m_bRivet = m_Data.SectBefore.SectI.RivetStiff.bRivet;

	UpdateData(FALSE);

	CArray<int, int> aAngleIndex; aAngleIndex.RemoveAll();

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		if (m_bAngle[i]) aAngleIndex.Add(i);
	}
	
	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD, m_bSymmetric, aAngleIndex);
}

BOOL CCMSectRivetAngleDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.SectBefore.SectI.RivetStiff.bSymmetric = m_bSymmetric;

	m_Data.SectBefore.SectI.RivetStiff.aStiffPosD.RemoveAll();
	m_Data.SectBefore.SectI.RivetStiff.aStiffPosD.SetSize(D_RIVET_ANGLE_SIZE_I);

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].bUseStiff = m_bAngle[i];
		m_cmbAngle[i].GetWindowText(m_Data.SectBefore.SectI.RivetStiff.aStiffPosD[i].strShapeName);
	}

	m_Data.SectBefore.SectI.RivetStiff.bRivet = m_bRivet;

	m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD);

	return TRUE;
}

void CCMSectRivetAngleDlg::GetDefStiffener()
{
	m_Data.SectBefore.SectI.RivetStiff.aStiffShape.RemoveAll();
	m_Data.SectBefore.SectI.RivetStiff.aStiffShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);

	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAll();
	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Copy(m_aSODStiffShape);

	InitCombo();

	Data2Dlg();
	m_wndSecView.Invalidate();
}

void CCMSectRivetAngleDlg::UpdateSectView()
{
	m_wndSecView.Invalidate();
}

BEGIN_MESSAGE_MAP(CCMSectRivetAngleDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectRivetAngleDlg)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_DEF_STIFF_BTN, OnCMSectRivetStiffenerBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_SECT_RIVET_DEF_STIFF_LST, OnDblclkDefStiffList)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_SYM_CHK, OnCMSectSymmetricChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_1_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_2_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_3_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_4_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_5_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_6_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_7_CHK, OnCMSectAngleChk)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_ANGLE_8_CHK, OnCMSectAngleChk)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_1_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_2_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_3_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_4_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_5_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_6_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_7_CMB, OnCMSectAngleCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_RIVET_ANGLE_8_CMB, OnCMSectAngleCmb)
	ON_BN_CLICKED(IDC_CMD_SECT_RIVET_USE_CHK, OnCMSectRivetChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectRivetAngleDlg message handlers

BOOL CCMSectRivetAngleDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCombo();
	InitGrid();
	InitView();
	ChangeBitmap();

	SetHeaderTitle_DefStiff();

	Data2Dlg();

	OnCMSectSymmetricChk();
	OnCMSectRivetChk();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectRivetAngleDlg::OnOK()
{
	if (!Dlg2Data()) return;

	///
	if (!m_Data.SectBefore.SectI.RivetStiff.bRivet)
		m_Data.SectBefore.SectI.RivetStiff.aRivetPosD.RemoveAll();
	///

	CDialogMove::OnOK();
}

void CCMSectRivetAngleDlg::OnCMSectRivetStiffenerBtn()
{
	CCMSectSODStiffenerDlg dlg;
	
	// 기존 대화상자 활용을 위해서 Data Convert 를 해준다.
	m_aSODStiffShape.RemoveAll();
	m_aSODStiffShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);

	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAll();
	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Copy(m_Data.SectBefore.SectI.RivetStiff.aStiffShape);

	dlg.m_Data = m_Data;
	dlg.m_nSelIndex = -1;
	dlg.m_nDlgType = 1;

	if (dlg.DoModal() == IDOK)
	{
		m_Data = dlg.m_Data;
		m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD);

		GetDefStiffener();
	}
	else
	{
		m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAll();
		m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Copy(m_aSODStiffShape);
	}

	OnCMSectRivetChk();
}

void CCMSectRivetAngleDlg::OnDblclkDefStiffList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstDefStiff.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	CCMSectSODStiffenerDlg dlg;

	// 기존 대화상자 활용을 위해서 Data Convert 를 해준다.
	m_aSODStiffShape.RemoveAll();
	m_aSODStiffShape.Copy(m_Data.SectBefore.SectI.SODStiffener.aStiffShape);

	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAll();
	m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Copy(m_Data.SectBefore.SectI.RivetStiff.aStiffShape);

	dlg.m_Data = m_Data;
	dlg.m_nSelIndex = -1;
	dlg.m_nDlgType = 1;

 	if (dlg.DoModal() == IDOK)
 	{
		m_Data = dlg.m_Data;
		m_pGrid->GetGrid2Data(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD);

		GetDefStiffener();
 	}
	else
	{
		m_Data.SectBefore.SectI.SODStiffener.aStiffShape.RemoveAll();
		m_Data.SectBefore.SectI.SODStiffener.aStiffShape.Copy(m_aSODStiffShape);
	}

	*pResult = 0;
}

void CCMSectRivetAngleDlg::OnCMSectSymmetricChk()
{
	UpdateData(TRUE);

	if (m_bSymmetric)
	{
		m_bAngle[4] = m_bAngle[0];
		m_bAngle[5] = m_bAngle[1];
		m_bAngle[6] = m_bAngle[2];
		m_bAngle[7] = m_bAngle[3];

		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[4], m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[5], m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[6], m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[7], m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));

		//m_cmbAngle[4].SetCurSel(m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		//m_cmbAngle[5].SetCurSel(m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		//m_cmbAngle[6].SetCurSel(m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		//m_cmbAngle[7].SetCurSel(m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));
	}

	UpdateData(FALSE);

	EnableDisableCtrl();

	Dlg2Data();

	CArray<int, int> aAngleIndex; aAngleIndex.RemoveAll();

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		if (m_bAngle[i]) aAngleIndex.Add(i);
	}

	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD, m_bSymmetric, aAngleIndex);

	m_wndSecView.Invalidate();
}

void CCMSectRivetAngleDlg::OnCMSectAngleChk()
{
	UpdateData(TRUE);

	if (m_bSymmetric)
	{
		m_bAngle[4] = m_bAngle[0];
		m_bAngle[5] = m_bAngle[1];
		m_bAngle[6] = m_bAngle[2];
		m_bAngle[7] = m_bAngle[3];


		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[4], m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[5], m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[6], m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[7], m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));

		//m_cmbAngle[4].SetCurSel(m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		//m_cmbAngle[5].SetCurSel(m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		//m_cmbAngle[6].SetCurSel(m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		//m_cmbAngle[7].SetCurSel(m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));
	}

	UpdateData(FALSE);

	EnableDisableCtrl();

	Dlg2Data();

	CArray<int, int> aAngleIndex; aAngleIndex.RemoveAll();

	for (int i = 0; i < D_RIVET_ANGLE_SIZE_I; i++)
	{
		if (m_bAngle[i]) aAngleIndex.Add(i);
	}

	if (!m_bRivet)
		aAngleIndex.RemoveAll();

	m_pGrid->SetData2Grid(m_Data.SectBefore.SectI.RivetStiff.aRivetPosD, m_bSymmetric, aAngleIndex);

	m_wndSecView.Invalidate();
}

void CCMSectRivetAngleDlg::OnCMSectAngleCmb()
{
	UpdateData(TRUE);

	if (m_bSymmetric)
	{
		m_bAngle[4] = m_bAngle[0];
		m_bAngle[5] = m_bAngle[1];
		m_bAngle[6] = m_bAngle[2];
		m_bAngle[7] = m_bAngle[3];

		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[4], m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[5], m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[6], m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		CDlgUtil::CobxSetCurSelItemData(m_cmbAngle[7], m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));

		//m_cmbAngle[4].SetCurSel(m_cmbAngle[0].GetItemData(m_cmbAngle[0].GetCurSel()));
		//m_cmbAngle[5].SetCurSel(m_cmbAngle[1].GetItemData(m_cmbAngle[1].GetCurSel()));
		//m_cmbAngle[6].SetCurSel(m_cmbAngle[2].GetItemData(m_cmbAngle[2].GetCurSel()));
		//m_cmbAngle[7].SetCurSel(m_cmbAngle[3].GetItemData(m_cmbAngle[3].GetCurSel()));
	}

	UpdateData(FALSE);

	Dlg2Data();

	m_wndSecView.Invalidate();
}

void CCMSectRivetAngleDlg::OnCMSectRivetChk()
{
	UpdateData(TRUE);

	OnCMSectAngleChk();
}