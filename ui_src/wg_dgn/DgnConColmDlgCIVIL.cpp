// DgnConColmDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmDlgCIVIL.h"
#include "DgnDataCtrl.h"
#include "DgnConColmBarDlgCIVIL.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmDlgCIVIL dialog


CDgnConColmDlgCIVIL::CDgnConColmDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConColmDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmDlgCIVIL)
	m_AddorDel = 0;
	//}}AFX_DATA_INIT

	if(m_arColmData.GetCount()>0) m_arColmData.RemoveAll();

	m_RchkKey = -1;
	m_RchkData.Initialize();

	m_BGroup.Add(IDC_DGN_COLB_STATIC);
	m_BGroup.Add(IDC_DGN_COLB);
	m_BGroup.Add(IDC_DGN_COLB_UNIT);

	m_HGroup.Add(IDC_DGN_COLH_STATIC);
	m_HGroup.Add(IDC_DGN_COLH);
	m_HGroup.Add(IDC_DGN_COLH_UNIT);

	m_tGroup.Add(IDC_DGN_COLt_STATIC);
	m_tGroup.Add(IDC_DGN_COLt);
	m_tGroup.Add(IDC_DGN_COLt_UNIT);

	m_abGroup.Add(IDC_DGN_COLa_STATIC);
	m_abGroup.Add(IDC_DGN_COLa);
	m_abGroup.Add(IDC_DGN_COLa_UNIT);
	m_abGroup.Add(IDC_DGN_COLb2_STATIC);
	m_abGroup.Add(IDC_DGN_COLb2);
	m_abGroup.Add(IDC_DGN_COLb2_UNIT);

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConColmDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_COLt_UNIT, m_tunit);
	DDX_Control(pDX, IDC_DGN_COLt, m_t);
	DDX_Control(pDX, IDC_DGN_COLH_UNIT, m_Hunit);
	DDX_Control(pDX, IDC_DGN_COLH, m_H);
	DDX_Control(pDX, IDC_DGN_COLb2_UNIT, m_bunit);
	DDX_Control(pDX, IDC_DGN_COLb2, m_b);
	DDX_Control(pDX, IDC_DGN_COLB_UNIT, m_Bunit);
	DDX_Control(pDX, IDC_DGN_COLB, m_B);
	DDX_Control(pDX, IDC_DGN_COLa_UNIT, m_aunit);
	DDX_Control(pDX, IDC_DGN_COLa, m_a);
	DDX_Radio(pDX, IDC_DGN_COLUMN_AR, m_AddorDel);

	DDX_Control(pDX, IDC_DGN_CONC_CGRID, m_InfoGrid);
	DDX_Control(pDX, IDC_DGN_CONC_SELECTGRID, m_SelectGrid.m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConColmDlgCIVIL, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConColmDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_RC_CINPUTBAR, OnDgnRcCinputbar)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AR, OnDgnColumnAr)
	ON_BN_CLICKED(IDC_DGN_COLUMN_DEL, OnDgnColumnDel)
	ON_BN_CLICKED(IDC_DGN_CON_COLM_SELECTALL, OnDgnColmSelectAll)
	ON_BN_CLICKED(IDC_DGN_CON_COLM_UNSELECTALL, OnDgnColmUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmDlgCIVIL message handlers


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  Initialize 관련 
//

void CDgnConColmDlgCIVIL::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnConColmDlgCIVIL::WriteInfoGridTitle()
{
	CGXStyle style;
	CString  title;

	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	title=_LS(IDS_DGN_CON_BEAM_Layer);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,0),style);
	m_InfoGrid.SetColWidth(0,0,globalUtils.ScaleByDPI(45), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_pos1);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,1),style);
	if(Is2TypeRebar(0)) m_InfoGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else                m_InfoGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(45), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_pos2);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,2),style);
	if(Is2TypeRebar(1)) m_InfoGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else                m_InfoGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(45), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_pos3);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,3),style);
	if(Is2TypeRebar(2)) m_InfoGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else                m_InfoGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(45), nullptr,GX_UPDATENOW);
	//////////////////////////////////////////////////////////////
	//  세로 Title를 찍어줍니다.
	for(int i=1; i<=5; i++)
	{
		title.Format(_T("%d"), i);    
	  style.SetValue(title);
		m_InfoGrid.SetStyleRange(CGXRange(i,0),style);
	}

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}

void CDgnConColmDlgCIVIL::SetInitGrid()
{
	m_InfoGrid.Initialize();

	m_InfoGrid.GetParam()->EnableUndo(FALSE);

	m_InfoGrid.SetRowCount(5);
	m_InfoGrid.SetColCount(3);

	m_InfoGrid.SetRowHeight(0,0, globalUtils.ScaleByDPI(20),NULL,GX_UPDATENOW);
	m_InfoGrid.SetRowHeight(1,5, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);

	WriteInfoGridTitle();

	m_InfoGrid.GetParam()->EnableSelection(FALSE);
	m_InfoGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_InfoGrid.GetParam()->EnableTrackColWidth(FALSE);
	m_InfoGrid.GetParam()->EnableMoveCols(FALSE);
	m_InfoGrid.GetParam()->EnableMoveRows(FALSE);

	m_InfoGrid.GetParam()->EnableUndo(TRUE);
	m_InfoGrid.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
}

//------------------------------------------------------------------
// Init Unit
//------------------------------------------------------------------
void CDgnConColmDlgCIVIL::SetInitUnit()
{
	m_t.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_H.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_b.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_a.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_tunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_aunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

//------------------------------------------------------------------
// Init Data
//------------------------------------------------------------------
void CDgnConColmDlgCIVIL::SetData2Dlg()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	ResetSectGridData();
	m_AddorDel = 0;
	m_Shape = _LS(IDS_DGN_COLM_TEXT_SECT_SREC);

	m_H.SetEditUnit(0);
	m_B.SetEditUnit(0);
	m_a.SetEditUnit(0);
	m_b.SetEditUnit(0);
	m_t.SetEditUnit(0);

	m_H.EnableWindow(FALSE);
	m_B.EnableWindow(FALSE);
	m_a.EnableWindow(FALSE);
	m_b.EnableWindow(FALSE);
	m_t.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_SHAPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RC_CINPUTBAR)->EnableWindow(FALSE);

	// Bitmap
	CString str;
	str.Format(_T("%s"), D_SECT_SHAPE_REG_SB);
	ShowBitmap(str);
	ShowHideControl(str);

	UpdateData(FALSE);

	CString title=_T("");
	CGXStyle style;
	style.SetValue(title);
	style.SetInterior(GetSysColor(COLOR_3DFACE));
	style.SetControl(GX_IDS_CTRL_STATIC);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	for(int i=1; i<6; i++)for(int j=1; j<4; j++) m_InfoGrid.SetStyleRange(CGXRange(i,j),style);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}

//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnConColmDlgCIVIL::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitSectGrid();
	SetInitGrid();
	SetInitUnit();
	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  OnOK 관련 
//

//------------------------------------------------------------------
// OnOK
//------------------------------------------------------------------

BOOL CDgnConColmDlgCIVIL::CheckInputData()
{
	// PropNo Check
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnConColmDlgCIVIL::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.nType = 1;

	CArray<T_RCHK_K, T_RCHK_K> arRchKey;
	arRchKey.RemoveAll();

	int Index = 0;
	T_RCHK_K RchKey;
	POSITION Pos = m_SelectGrid.m_arKey.GetStartPosition();
	while(Pos)
	{
		RchKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(Pos, Index, RchKey);
		arRchKey.Add(RchKey);
	}

	if(arRchKey.GetSize() > 0)
	{
		if(!m_pDoc->m_pDataCtrl->AddRchk(arRchKey, m_RchkData))
			bCheck = FALSE;
	}

	return bCheck;
}

void CDgnConColmDlgCIVIL::OnDgnExecute() 
{
	UpdateData(TRUE);

	if(!m_SelectGrid.IsExistEnabledSect())
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_SECT));
		return;
	}

	if(m_AddorDel == 0)   // Add/Replace
	{
		if(CheckInputData() == FALSE) return;
		if(SetDlg2Data()) SetData2Dlg();
	}
	else                  // Del
	{
		BOOL bCheck = TRUE;

		CArray<T_RCHK_K, T_RCHK_K> arRchKey;
		arRchKey.RemoveAll();

		T_RCHK_K RchKey;
		int Index = 0;
	  POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	  while(PosSect)
	  {
		  RchKey=0;
		  m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RchKey);
			arRchKey.Add(RchKey);
		}
		if(arRchKey.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->DelRchk(arRchKey)) bCheck = FALSE;
		}

		if(bCheck)  SetData2Dlg();
	}
}

void CDgnConColmDlgCIVIL::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
	
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  SelChange PropNo CB 관련 
//

//------------------------------------------------------------------
// ShowBitmap
//------------------------------------------------------------------

void CDgnConColmDlgCIVIL::ShowBitmap(CString& colType)
{
	CWnd* pWnd[9];
	pWnd[0] = GetDlgItem(IDC_DGN_RECT_FRAME);   ASSERT(pWnd[0]);
	pWnd[1] = GetDlgItem(IDC_DGN_SRECT_FRAME);  ASSERT(pWnd[1]);
	pWnd[2] = GetDlgItem(IDC_DGN_CIRC_FRAME);   ASSERT(pWnd[2]);
	pWnd[3] = GetDlgItem(IDC_DGN_SCIRC_FRAME);  ASSERT(pWnd[3]);
	pWnd[4] = GetDlgItem(IDC_DGN_OCTA_FRAME);   ASSERT(pWnd[4]);
	pWnd[5] = GetDlgItem(IDC_DGN_SOCTA_FRAME);  ASSERT(pWnd[5]);
	pWnd[6] = GetDlgItem(IDC_DGN_HALF_FRAME);   ASSERT(pWnd[6]);
	pWnd[7] = GetDlgItem(IDC_DGN_TRACK_FRAME);  ASSERT(pWnd[7]);
	pWnd[8] = GetDlgItem(IDC_DGN_STRACK_FRAME); ASSERT(pWnd[8]);

	for(int i=0; i<9; i++) pWnd[i]->ShowWindow(SW_HIDE);

	if(colType == D_SECT_SHAPE_REG_B)         pWnd[0]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SB)   pWnd[1]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_P)    pWnd[2]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SR)   pWnd[3]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_OCT)  pWnd[4]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_SOCT) pWnd[5]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_HTRK) pWnd[6]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_TRK)  pWnd[7]->ShowWindow(SW_SHOW);
	else if(colType == D_SECT_SHAPE_REG_STRK) pWnd[8]->ShowWindow(SW_SHOW);
}

void CDgnConColmDlgCIVIL::SetControlText(BOOL bBox)
{
	if(bBox == TRUE)
	{
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("tf1"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("tf2"));;
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("tw"));;
	}
	else
	{
		GetDlgItem(IDC_DGN_COLa_STATIC)->SetWindowText(_T("a"));
		GetDlgItem(IDC_DGN_COLb2_STATIC)->SetWindowText(_T("b"));
		GetDlgItem(IDC_DGN_COLt_STATIC)->SetWindowText(_T("t"));
	}
}

//------------------------------------------------------------------
// ShowHideControl
//------------------------------------------------------------------
void CDgnConColmDlgCIVIL::ShowHideControl(CString& colType)
{
	if(colType == D_SECT_SHAPE_REG_SR)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);
		
		m_H.SetWindowText(_T("0"));
		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_P)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);

		m_H.SetWindowText(_T("0"));
		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_SB || colType == D_SECT_SHAPE_REG_HTRK || colType == D_SECT_SHAPE_REG_STRK)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);

		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_TRK)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);

		m_a.SetWindowText(_T("0"));
		m_b.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_SOCT)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, FALSE);

		m_t.SetWindowText(_T("0"));
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_OCT)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);
		SetControlText(FALSE);
	}
	else if(colType == D_SECT_SHAPE_REG_B)
	{
		CDlgUtil::CtrlEnableDisable(this, m_BGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_HGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_abGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_tGroup, TRUE);
		SetControlText(TRUE);
	}
}

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConColmDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;
	CDgnDataCtrl DataCtrl;

	T_RCHK_K RchKey;
	int Index = 0;
	POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	while(PosSect)
	{
		RchKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RchKey);

		POSITION PosElem = m_pDoc->m_pAttrCtrl->GetStartElem();
		while(PosElem)
		{
			ElemData.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextElem(PosElem, ElemKey, ElemData);
		
			if(ElemData.elpro == RchKey) 
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_COLUMN)  arElemKeyList.Add(ElemKey);
		}
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

//---------------------------------------------------------------------------
// ChangeSectionInfo 
//---------------------------------------------------------------------------
void CDgnConColmDlgCIVIL::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData)) return;

	CString str = _T("");
	if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SR)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SRND));//(_T("Solid Round")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SR);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_P)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_PIPE));//(_T("Pipe")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_P);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[0]); 
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SB)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SREC));//(_T("Solid Rectangle")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SB);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_BOX));//(_T("Box")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_B);
		double tf2;
		if(SectData.SectBefore.SectI.Size[5] == 0)  tf2 = SectData.SectBefore.SectI.Size[3]; 
		else                                        tf2 = SectData.SectBefore.SectI.Size[5];
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]); 
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
		m_b.SetEditUnit(tf2);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_OCT)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_OCTA));//(_T("Octagon")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_OCT);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
		m_b.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[4]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_SOCT));//(_T("Solid Octagon")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_SOCT);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_a.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
		m_b.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_TRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_TRCK));//(_T("Track")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_TRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
		m_t.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_STRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_STRK));//(_T("Solid Track"));
		str.Format(_T("%s"), D_SECT_SHAPE_REG_STRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
	{
		GetDlgItem(IDC_DGN_SHAPE)->SetWindowText(_LS(IDS_DGN_COLM_TEXT_SECT_HTRK));//(_T("Half Track")); 
		str.Format(_T("%s"), D_SECT_SHAPE_REG_HTRK);
		m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
		m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	}
		
	ShowBitmap(str);
	ShowHideControl(str);

	WriteInfoGridTitle();

	m_RchkData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRchk(m_RchkKey, m_RchkData);

	WriteRebarInfoOnGrid();
		
	UpdateData(TRUE);
	if(m_SelectGrid.IsExistEnabledSect()) 
		GetDlgItem(IDC_DGN_RC_CINPUTBAR)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DGN_RC_CINPUTBAR)->EnableWindow(FALSE);
}

void CDgnConColmDlgCIVIL::OnDgnColumnAr() 
{
	if(m_SelectGrid.m_arKey.GetCount() <= 0) return;
	GetDlgItem(IDC_DGN_RC_CINPUTBAR)->EnableWindow(TRUE);
}

void CDgnConColmDlgCIVIL::OnDgnColumnDel() 
{
	if(m_SelectGrid.m_arKey.GetCount() <= 0) return;
	GetDlgItem(IDC_DGN_RC_CINPUTBAR)->EnableWindow(FALSE);	
}

//------------------------------------------------------------------
// Change Prop CB
//------------------------------------------------------------------

void CDgnConColmDlgCIVIL::InitSectGrid()
{

	// Data Sort & Store
	if(m_arColmData.GetCount()>0) m_arColmData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();

	CMap<int,int,int,int> BeamSecMap; BeamSecMap.RemoveAll();
	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);

		T_MATD_D MatdData; MatdData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

		if(MatdData.Type == _T("C"))
		{
			// 1=column, 2=beam, 3=brace, 0=기타
	    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T")) &&
					 SectData.nStype==D_SECT_TYPE_REGULAR) 
					 BeamSecMap.SetAt(ElemData.elpro,ElemData.elpro);

				if(nMembType == D_MBTP_COLUMN && 
					(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_B    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB   || 
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_P    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SR   ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_OCT  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SOCT ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_TRK  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_STRK ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK )) m_arColmData.SetAt(ElemData.elpro, ElemData.elpro);
			}
		}
	}

	POSITION Pos = BeamSecMap.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		BeamSecMap.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arColmData.Lookup(SectK, rSectK))
		{
			m_arColmData.RemoveKey(SectK);
		}
	}  

	m_SelectGrid.InitSectGrid(m_arColmData);
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
//  Button Click 관련 
//

void CDgnConColmDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
}

void CDgnConColmDlgCIVIL::OnDgnRcCinputbar() 
{
	CDgnConColmBarDlgCIVIL dlg;
	dlg.SetRchkData(m_RchkKey, m_SelectGrid.m_arKey, &m_RchkData.COLM); 
	if(dlg.DoModal() == IDOK)
	{
		WriteInfoGridTitle();
		WriteRebarInfoOnGrid();
		m_InfoGrid.Redraw();
	}
}

void CDgnConColmDlgCIVIL::WriteRebarInfoOnGrid()
{
	CString strRebar;
	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	for(int pos=0; pos<3; pos++) for(int i=0; i<5; i++)
	{  
		strRebar = _T("");
		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		// 일단 0 으로 고정해두었다~	

		if(m_RchkData.COLM.iBarNum[0][pos][i]>0)
		{
			if(m_RchkData.COLM.strBarNa2[0][pos][i]!=_T(""))
			{
				if(m_RchkData.COLM.strBarNa2[0][pos][i].GetLength()>=1)
				{
				int nLength = m_RchkData.COLM.strBarNa2[0][pos][i].GetLength()-1;
				strRebar.Format(_T("%d-%s,%s"), m_RchkData.COLM.iBarNum[0][pos][i], m_RchkData.COLM.strBarNa1[0][pos][i],m_RchkData.COLM.strBarNa2[0][pos][i].Mid(1, nLength));
				}
			}
			else strRebar.Format(_T("%d-%s"), m_RchkData.COLM.iBarNum[0][pos][i], m_RchkData.COLM.strBarNa1[0][pos][i]);
		}

		style.SetValue(strRebar);
	  m_InfoGrid.SetStyleRange(CGXRange(i+1, pos+1), style);

		m_InfoGrid.GetParam()->EnableUndo(TRUE);
	}
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// DB 관련 
//

void CDgnConColmDlgCIVIL::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
		// do something...
			break;
		case D_UPDATE_UNIT:
			UpdateUnit();
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConColmDlgCIVIL::UpdateUnit()
{
	m_pDoc->m_pUnitCtrl->ConvertUnitRchkPrevious(m_RchkData);    
}

void CDgnConColmDlgCIVIL::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bSECD = FALSE;
	BOOL bDEFT = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			// 다른 case도 추가해주어야 한다.
			case(UR_SECD_ADD): 
			case(UR_SECD_DEL): 
			case(UR_SECD_MFS): 
			case(UR_SECD_MFD):
			case(UR_RCHK_ADD): 
			case(UR_RCHK_DEL):  { bSECD = TRUE; break; }
			default:						{ bDEFT = TRUE;	break; }
		}
	}
	if(bSECD) ResetSectGridData(TRUE);
}

BOOL CDgnConColmDlgCIVIL::Is2TypeRebar(int nPos)
{
	for(int i=0; i<5; i++) if(m_RchkData.COLM.strBarNa2[0][nPos][i]!=_T("")) return TRUE;
	
	return FALSE;
}

LRESULT CDgnConColmDlgCIVIL::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{

	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnConColmDlgCIVIL::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{

	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnConColmDlgCIVIL::OnDgnColmSelectAll()
{

	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnConColmDlgCIVIL::OnDgnColmUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}



