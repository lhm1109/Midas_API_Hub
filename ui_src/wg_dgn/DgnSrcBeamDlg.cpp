// DgnConBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSrcBeamDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnConBeamBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcBeamDlg dialog


CDgnSrcBeamDlg::CDgnSrcBeamDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnSrcBeamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSrcBeamDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_iStirrupNumI = 0;
	m_iStirrupNumC = 0;
	m_iStirrupNumJ = 0;

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSrcBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSrcBeamDlg)
	DDX_Control(pDX, IDC_DGN_SRC_SJ, m_SjCtrl);
	DDX_Control(pDX, IDC_DGN_SRC_SC, m_ScCtrl);
	DDX_Control(pDX, IDC_DGN_SRC_SI, m_SiCtrl);
	DDX_Control(pDX, IDC_DGN_SRC_BSTIRRUP, m_StirrupRebar);

	DDX_Control(pDX, IDC_DGN_SRC_BDT, m_Dt);
	DDX_Control(pDX, IDC_DGN_SRC_BDB, m_Db);
	DDX_Control(pDX, IDC_DGN_SRC_TW, m_Tw);
	DDX_Control(pDX, IDC_DGN_SRC_TF, m_Tf);
	DDX_Control(pDX, IDC_DGN_SRC_TF2, m_Tf2);
	DDX_Control(pDX, IDC_DGN_SRC_HC, m_Hc);
	DDX_Control(pDX, IDC_DGN_SRC_CW, m_Cw);
	DDX_Control(pDX, IDC_DGN_SRC_BC, m_Bc);
	DDX_Control(pDX, IDC_DGN_SRC_B, m_B);
	DDX_Control(pDX, IDC_DGN_SRC_H, m_H);
	DDX_Control(pDX, IDC_DGN_SRC_BDBUNIT, m_DbUnit);
	DDX_Control(pDX, IDC_DGN_SRC_BDTUNIT, m_DtUnit);
	DDX_Control(pDX, IDC_DGN_SRC_TWUNIT, m_TwUnit);
	DDX_Control(pDX, IDC_DGN_SRC_TFUNIT, m_TfUnit);
	DDX_Control(pDX, IDC_DGN_SRC_TFUNIT2, m_Tf2Unit);
	DDX_Control(pDX, IDC_DGN_SRC_HUNIT, m_HUnit);
	DDX_Control(pDX, IDC_DGN_SRC_HCUNIT, m_HcUnit);
	DDX_Control(pDX, IDC_DGN_SRC_CWUNIT, m_CwUnit);
	DDX_Control(pDX, IDC_DGN_SRC_BUNIT, m_BUnit);
	DDX_Control(pDX, IDC_DGN_SRC_BCUNIT, m_BcUnit);

	DDX_Radio(pDX, IDC_DGN_BEAM_AR, m_nOption);

	DDX_Control(pDX, IDC_DGN_SRC_BGRID, m_wndGrid);
	DDX_Control(pDX, IDC_DGN_SRC_SELECTGRID, m_SelectGrid.m_Grid);

	DDX_Control(pDX, IDC_DGN_SRCBIT_BEAM, m_wndSrcBeamRHB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSrcBeamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnSrcBeamDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_BEAM_DEL, OnDgnBeamDel)
	ON_BN_CLICKED(IDC_DGN_BEAM_AR, OnDgnBeamAr)
	ON_BN_CLICKED(IDC_DGN_SRC_INODE, OnDgnConcInode)
	ON_BN_CLICKED(IDC_DGN_SRC_JNODE, OnDgnConcJnode)
	ON_BN_CLICKED(IDC_DGN_SRC_MIDDLE, OnDgnConcMiddle)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_SRC_BEAM_SELECTALL, OnDgnBeamSelectAll)
	ON_BN_CLICKED(IDC_DGN_SRC_BEAM_UNSELECTALL, OnDgnBeamUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcBeamDlg message handlers

void CDgnSrcBeamDlg::Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa)
{
	int iCutNo=0;
	int iCount = strTemp.GetLength();
	if(iCount > 0)
	{
		for(int i=0; i<iCount; i++)
		{
			TCHAR ch = strTemp.GetAt(i);
			if(ch=='-')	iCutNo = i;
		}
		iNum		= _ttoi(strTemp.Left(iCutNo));
		RebarNa	= strTemp.Mid(iCutNo+1);
	}
	else
	{
		iNum		= 0;
		RebarNa	= _T("");
	}
}

BOOL CDgnSrcBeamDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	int nCheck=0;
	for(int i=1; i<=4; i++)
	{
		for(int j=1; j<=3; j++)
		{
			if(m_wndGrid.GetValueRowCol(i,j)==_T(""))	nCheck++; 
		}
	}
	if(nCheck==12)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_DATA),MB_OK);
		return bCheck;  
	}

	if(m_StirrupRebar.GetCurSel()==-1)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_STIRRUP_DATA),MB_OK);
		return bCheck;
	}

	if(m_Db.GetEditValue() <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_DB_VALUE),MB_OK);
		return bCheck;
	}
	if(m_Dt.GetEditValue() <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_DT_VALUE),MB_OK);
		return bCheck;
	}
	return bCheck;
}

void CDgnSrcBeamDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	if(!m_SelectGrid.IsExistEnabledSect())
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_PROP));
		return;
	}

	if(m_nOption == 0)   // Add/Replace
	{
		if(ErrorCheck() == FALSE) return;   
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
			if(!m_pDoc->m_pDataCtrl->DelRbsb(arRchKey))
				bCheck = FALSE;
		}

		if(bCheck)
			SetData2Dlg();
	}
}

void CDgnSrcBeamDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnSrcBeamDlg::OnDgnBeamDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnSrcBeamDlg::OnDgnBeamAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

BOOL CDgnSrcBeamDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	if(m_StirrupRebar.GetCount() > 0)	m_StirrupRebar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_StirrupRebar, FALSE);
	// Initialize Space Status.
	GetDlgItem(IDC_DGN_SRC_SI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_SC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_SJ)->ShowWindow(SW_HIDE);

	InitSectGrid();
	Initial_Unit();
	Initial_GridWnd();

	SetData2Dlg();

	m_wndSrcBeamRHB.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_sect_src_beam_RHB.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSrcBeamDlg::Write_GridTitle()
{
	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	CString title=_T("");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,0),style);
	m_wndGrid.SetColWidth(0,0,globalUtils.ScaleByDPI(31), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_I_NODE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,1),style);
	m_wndGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(49), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_MIDDLE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,2),style);
	m_wndGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(49), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_J_NODE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,3),style);
	m_wndGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(49), nullptr,GX_UPDATENOW);
	//////////////////////////////////////////////////////////////
	//  세로 Title를 찍어줍니다.
	title=_LS(IDS_DGN_GRID_TOP);
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(1,0,2,0);
	m_wndGrid.SetStyleRange(CGXRange(1,0),style);

	title=_LS(IDS_DGN_GRID_BOT);
	style.SetValue(title);
	m_wndGrid.SetCoveredCellsRowCol(3,0,4,0);
	m_wndGrid.SetStyleRange(CGXRange(3,0),style);
	//////////////////////////////////////////////////////////////
	title=_T("");
	style.SetValue(title);
	style.SetControl(GX_IDS_CTRL_STATIC);
	for(int i=1; i<5; i++)
		for(int j=1; j<4; j++)
			m_wndGrid.SetStyleRange(CGXRange(i,j),style);
}

void CDgnSrcBeamDlg::Initial_GridWnd()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	m_wndGrid.GetParam()->EnableUndo(FALSE);

	m_wndGrid.SetRowCount(4);
	m_wndGrid.SetColCount(3);

	m_wndGrid.SetRowHeight(0,0, globalUtils.ScaleByDPI(20),NULL,GX_UPDATENOW);
	m_wndGrid.SetRowHeight(1,4, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);

	Write_GridTitle();

	//  Select가 안되도록 하자 !!!
	m_wndGrid.GetParam()->EnableSelection(FALSE);
	//  줄간격 및 칸간격을 조정하지 못하게 하자 !!!
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	m_wndGrid.GetParam()->EnableUndo(TRUE);
}

void CDgnSrcBeamDlg::Initial_Unit()
{
	m_Dt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Db.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Cw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_H.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_SiCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ScCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SjCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_DbUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DtUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_TwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_TfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_CwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnSrcBeamDlg::Initial_ExistData()
{
	CString strNt1=_T(""),strNt2=_T(""),strNb1=_T(""),strNb2=_T("");
	////////////////////////////////////////////////////////////////
	//  I_Node Top m_RchkData...
	if(m_RchkData.Top_I.nArrayType == 1)  // One Array
	{
		if(m_RchkData.Top_I.nRebar1==0)	strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_I.nRebar1,m_RchkData.Top_I.RebarName);
		strNt2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Top_I.nRebar1==0) strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_I.nRebar1,m_RchkData.Top_I.RebarName);

		if(m_RchkData.Top_I.nRebar2==0)	strNt2=_T("");
		else	strNt2.Format(_T("%d-%s"),m_RchkData.Top_I.nRebar2,m_RchkData.Top_I.RebarName);
	}
	////////////////////////////////////////////////////////////////
	//  I_Node Bottom m_RchkData...
	if(m_RchkData.Bot_I.nArrayType == 1)  // One Array
	{
		if(m_RchkData.Bot_I.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_I.nRebar1,m_RchkData.Bot_I.RebarName);
		strNb2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Bot_I.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_I.nRebar1,m_RchkData.Bot_I.RebarName);

		if(m_RchkData.Bot_I.nRebar2==0)	strNb2=_T("");
		else	strNb2.Format(_T("%d-%s"),m_RchkData.Bot_I.nRebar2,m_RchkData.Bot_I.RebarName);
	}
	Write_GridItem(_T("I"),strNt1,strNt2,strNb1,strNb2);
	////////////////////////////////////////////////////////////////
	//  Center_Node Top m_RchkData...
	if(m_RchkData.Top_C.nArrayType == 1) // One Array
	{
		if(m_RchkData.Top_C.nRebar1==0)	strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_C.nRebar1,m_RchkData.Top_C.RebarName);
		strNt2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Top_C.nRebar1==0) strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_C.nRebar1,m_RchkData.Top_C.RebarName);

		if(m_RchkData.Top_C.nRebar2==0)	strNt2=_T("");
		else	strNt2.Format(_T("%d-%s"),m_RchkData.Top_C.nRebar2,m_RchkData.Top_C.RebarName);
	}
	////////////////////////////////////////////////////////////////
	//  Center_Node Bottom m_RchkData...
	if(m_RchkData.Bot_C.nArrayType == 1)	// One Array
	{
		if(m_RchkData.Bot_C.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_C.nRebar1,m_RchkData.Bot_C.RebarName);
		strNb2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Bot_C.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_C.nRebar1,m_RchkData.Bot_C.RebarName);

		if(m_RchkData.Bot_C.nRebar2==0)	strNb2=_T("");
		else	strNb2.Format(_T("%d-%s"),m_RchkData.Bot_C.nRebar2,m_RchkData.Bot_C.RebarName);
	}
	Write_GridItem(_T("M"),strNt1,strNt2,strNb1,strNb2);
	////////////////////////////////////////////////////////////////
	//  J_Node Top m_RchkData...
	if(m_RchkData.Top_J.nArrayType == 1)	// One Array
	{
		if(m_RchkData.Top_J.nRebar1==0)	strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_J.nRebar1,m_RchkData.Top_J.RebarName);
		strNt2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Top_J.nRebar1==0)	strNt1=_T("");
		else	strNt1.Format(_T("%d-%s"),m_RchkData.Top_J.nRebar1,m_RchkData.Top_J.RebarName);

		if(m_RchkData.Top_J.nRebar2==0)	strNt2=_T("");
		else	strNt2.Format(_T("%d-%s"),m_RchkData.Top_J.nRebar2,m_RchkData.Top_J.RebarName);
	}
	////////////////////////////////////////////////////////////////
	//  J_Node Bottom m_RchkData...
	if(m_RchkData.Bot_J.nArrayType == 1)	// One Array
	{
		if(m_RchkData.Bot_J.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_J.nRebar1,m_RchkData.Bot_J.RebarName);
		strNb2 = _T("");
	}
	else	// Two Array
	{
		if(m_RchkData.Bot_J.nRebar1==0)	strNb1=_T("");
		else	strNb1.Format(_T("%d-%s"),m_RchkData.Bot_J.nRebar1,m_RchkData.Bot_J.RebarName);

		if(m_RchkData.Bot_J.nRebar2==0)	strNb2=_T("");
		else	strNb2.Format(_T("%d-%s"),m_RchkData.Bot_J.nRebar2,m_RchkData.Bot_J.RebarName);
	}
	Write_GridItem(_T("J"),strNt1,strNt2,strNb1,strNb2);
	////////////////////////////////////////////////////////////////
	m_Dt.SetEditUnit(m_RchkData.dT);
	m_Db.SetEditUnit(m_RchkData.dB);

	m_StirrupSpaceI = m_RchkData.Top_I.dSubRebarSpace;
	m_StirrupSpaceC = m_RchkData.Top_C.dSubRebarSpace;
	m_StirrupSpaceJ = m_RchkData.Top_J.dSubRebarSpace;
	m_SiCtrl.SetEditUnit(m_StirrupSpaceI);
	m_ScCtrl.SetEditUnit(m_StirrupSpaceC);
	m_SjCtrl.SetEditUnit(m_StirrupSpaceJ);

	m_iStirrupNumI = m_RchkData.Top_I.iSubRebarNum;
	m_iStirrupNumC = m_RchkData.Top_C.iSubRebarNum;
	m_iStirrupNumJ = m_RchkData.Top_J.iSubRebarNum;

	int Index = m_StirrupRebar.FindString(-1,m_RchkData.SubRebarName);
	if(Index != -1) m_StirrupRebar.SetCurSel(Index);
}

void CDgnSrcBeamDlg::Write_GridItem(CString Type,CString strNt1,CString strNt2,CString strNb1,CString strNb2)
{
	int N=0;
	if(Type == _T("I"))      N=1;
	else if(Type == _T("M")) N=2;
	else if(Type == _T("J")) N=3;

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	CString title;
	title = strNt1;
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(1,N),style);

	title = strNt2;
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,N),style);

	title = strNb2;
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(3,N),style);

	title = strNb1;
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(4,N),style);

	m_wndGrid.GetParam()->EnableUndo(TRUE);
}

void CDgnSrcBeamDlg::OnDgnConcInode() 
{

	CDgnConBeamBarDlg dlg;
	
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = _T("RHB");
	dlg.m_iMode = 1;
	//  Grid의 Data를 얻어옵니다.
	dlg.m_sNt  = m_wndGrid.GetValueRowCol(1,1);
	dlg.m_sNt2 = m_wndGrid.GetValueRowCol(2,1);
	dlg.m_sNb2 = m_wndGrid.GetValueRowCol(3,1);
	dlg.m_sNb  = m_wndGrid.GetValueRowCol(4,1);
	dlg.m_DbNo = m_Db.GetEditValue();
	dlg.m_DtNo = m_Dt.GetEditValue();
	dlg.m_Space				= m_SiCtrl.GetEditValue();
	dlg.m_iStirrupNum = m_iStirrupNumI;

	if(dlg.DoModal()==IDOK)
	{
		CString strNt1=_T(""),strNt2=_T(""),strNb1=_T(""),strNb2=_T("");

		strNt1 = dlg.m_sNt;
		strNt2 = dlg.m_sNt2;
		strNb1 = dlg.m_sNb;
		strNb2 = dlg.m_sNb2;

		m_StirrupSpaceI = dlg.m_Space;
		m_SiCtrl.SetEditUnit(m_StirrupSpaceI);
		m_iStirrupNumI = dlg.m_iStirrupNum;

		m_Db.SetEditUnit(dlg.m_DbNo);
		m_Dt.SetEditUnit(dlg.m_DtNo);

		Write_GridItem(_T("I"),strNt1,strNt2,strNb1,strNb2);
	}
}

void CDgnSrcBeamDlg::OnDgnConcJnode() 
{
	CDgnConBeamBarDlg dlg;
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = _T("RHB");
	dlg.m_iMode = 1;
	//  Grid의 Data를 얻어옵니다.
	dlg.m_sNt  = m_wndGrid.GetValueRowCol(1,3);
	dlg.m_sNt2 = m_wndGrid.GetValueRowCol(2,3);
	dlg.m_sNb2 = m_wndGrid.GetValueRowCol(3,3);
	dlg.m_sNb  = m_wndGrid.GetValueRowCol(4,3);
	dlg.m_DbNo = m_Db.GetEditValue();
	dlg.m_DtNo = m_Dt.GetEditValue();
	dlg.m_Space				= m_SjCtrl.GetEditValue();
	dlg.m_iStirrupNum	= m_iStirrupNumJ;

	if(dlg.DoModal()==IDOK)
	{
		CString strNt1=_T(""),strNt2=_T(""),strNb1=_T(""),strNb2=_T("");

		strNt1 = dlg.m_sNt;
		strNt2 = dlg.m_sNt2;
		strNb1 = dlg.m_sNb;
		strNb2 = dlg.m_sNb2;

		m_StirrupSpaceJ = dlg.m_Space;
		m_SjCtrl.SetEditUnit(m_StirrupSpaceJ);
		m_iStirrupNumJ = dlg.m_iStirrupNum;

		m_Db.SetEditUnit(dlg.m_DbNo);
		m_Dt.SetEditUnit(dlg.m_DtNo);

		Write_GridItem(_T("J"),strNt1,strNt2,strNb1,strNb2);
	}
}

void CDgnSrcBeamDlg::OnDgnConcMiddle() 
{
	CDgnConBeamBarDlg dlg;
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = _T("RHB");
	dlg.m_iMode = 1;
	//  Grid의 Data를 얻어옵니다.
	dlg.m_sNt  = m_wndGrid.GetValueRowCol(1,2);
	dlg.m_sNt2 = m_wndGrid.GetValueRowCol(2,2);
	dlg.m_sNb2 = m_wndGrid.GetValueRowCol(3,2);
	dlg.m_sNb  = m_wndGrid.GetValueRowCol(4,2);
	dlg.m_DbNo = m_Db.GetEditValue();
	dlg.m_DtNo = m_Dt.GetEditValue();
	dlg.m_Space				= m_ScCtrl.GetEditValue();
	dlg.m_iStirrupNum = m_iStirrupNumC;

	if(dlg.DoModal()==IDOK)
	{
		CString strNt1=_T(""),strNt2=_T(""),strNb1=_T(""),strNb2=_T("");

		strNt1 = dlg.m_sNt;
		strNt2 = dlg.m_sNt2;
		strNb1 = dlg.m_sNb;
		strNb2 = dlg.m_sNb2;

		m_StirrupSpaceC = dlg.m_Space;
		m_ScCtrl.SetEditUnit(m_StirrupSpaceC);
		m_iStirrupNumC = dlg.m_iStirrupNum;

		m_Db.SetEditUnit(dlg.m_DbNo);
		m_Dt.SetEditUnit(dlg.m_DtNo);

		Write_GridItem(_T("M"),strNt1,strNt2,strNb1,strNb2);
	}
}

void CDgnSrcBeamDlg::OnDgnDesignSection() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));
}

void CDgnSrcBeamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		// do something...
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			// do something...
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			// Add by ZINU.('02.10.16). Update changed rebar kind.
			if(m_StirrupRebar.GetCount() > 0)	m_StirrupRebar.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_StirrupRebar, FALSE);
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnSrcBeamDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD   = FALSE;
	BOOL bDCON  = FALSE;
	BOOL bOther = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_SECD_ADD):
			case(UR_SECD_DEL):
			case(UR_SECD_MFS):
			case(UR_SECD_MFD):	
			case(UR_RBSB_ADD):
			case(UR_RBSB_DEL):  {bMFD  = TRUE; break;}
			case(UR_DCON_ADD): 
			case(UR_DCON_DEL):  {bDCON = TRUE; break;}
			default:						{bOther=TRUE;	break;}
		}
	}

	if(bMFD) ResetSectGridData(TRUE);
}

void CDgnSrcBeamDlg::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

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
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BEAM)  arElemKeyList.Add(ElemKey);
		}
	}
	
	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

void CDgnSrcBeamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CDgnSrcBeamDlg::InitSectGrid()
{
	// Data Sort & Store
	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();

	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);

		int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

		T_SECT_D SectData; SectData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

		if(bCheck == TRUE) 
		{
			if(nMembType == D_MBTP_BEAM && SectData.nStype==D_SECT_TYPE_SRC && SectData.SectBefore.Shape == _T("RHB")) 
				 m_arBeamData.SetAt(ElemData.elpro,ElemData.elpro);
		}
	}

	m_SelectGrid.InitSectGrid(m_arBeamData,6);
}

void CDgnSrcBeamDlg::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnSrcBeamDlg::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();
	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData)) return;

	m_H.SetEditUnit(SectData.SectBefore.SectI.Size[0]);
	m_B.SetEditUnit(SectData.SectBefore.SectI.Size[1]);
	m_Tw.SetEditUnit(SectData.SectBefore.SectI.Size[2]);
	m_Tf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
	if(SectData.SectBefore.SectI.Size[4]==0.)  
		SectData.SectBefore.SectI.Size[4] = SectData.SectBefore.SectI.Size[1]-SectData.SectBefore.SectI.Size[2];
	m_Cw.SetEditUnit(SectData.SectBefore.SectI.Size[4]);
	if(SectData.SectBefore.SectI.Size[5]==0.)	m_Tf2.SetEditUnit(SectData.SectBefore.SectI.Size[3]);
	else																		  m_Tf2.SetEditUnit(SectData.SectBefore.SectI.Size[5]);

	m_Hc.SetEditUnit(SectData.SectBefore.SectJ.Size[0]);
	m_Bc.SetEditUnit(SectData.SectBefore.SectJ.Size[1]);

	m_RchkData.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetRbsb(m_RchkKey, m_RchkData))   Initial_ExistData();
	else                                                      Initial_WithoutData();

	UpdateData(FALSE);
	UpdateData(TRUE);
	if(m_SelectGrid.IsExistEnabledSect())  
	{
		GetDlgItem(IDC_DGN_SRC_INODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_MIDDLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_JNODE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SRC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_JNODE)->EnableWindow(FALSE);
	}

}

LRESULT CDgnSrcBeamDlg::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{
	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnSrcBeamDlg::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnSrcBeamDlg::OnDgnBeamSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnSrcBeamDlg::OnDgnBeamUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

void CDgnSrcBeamDlg::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	ResetSectGridData();

	m_nOption = 0;    //  add/replace

	//  대화상자가 바뀌면 변수를 초기화합니다.
	m_Tw.SetEditUnit(0.0);
	m_Tf.SetEditUnit(0.0);
	m_Tf2.SetEditUnit(0.0);
	m_Hc.SetEditUnit(0.0);
	m_Cw.SetEditUnit(0.0);
	m_Bc.SetEditUnit(0.0);
	m_B.SetEditUnit(0.0);
	m_H.SetEditUnit(0.0);

	m_Dt.SetEditUnit(0.0);
	m_Db.SetEditUnit(0.0);

	m_Tw.EnableWindow(FALSE);
	m_Tf.EnableWindow(FALSE);
	m_Tf2.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_Cw.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);
	m_B.EnableWindow(FALSE);
	m_H.EnableWindow(FALSE);

	//  Stirrup Rebar를 초기화시킵니다.
	m_StirrupRebar.SetCurSel(-1);

	UpdateData(FALSE);

	// PropNo가 선택이 안되었으므로..
	if(m_SelectGrid.m_arKey.GetCount() <= 0)
	{
		GetDlgItem(IDC_DGN_SRC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_JNODE)->EnableWindow(FALSE);
	}

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	//////////////////////////////////////////////////////////////
	//  철근 데이터 그리드의 값들을 초기화합니다.
	CString title=_T("");
	style.SetValue(title);
	style.SetControl(GX_IDS_CTRL_STATIC);
	for(int i=1; i<5; i++)
		for(int j=1; j<4; j++)
			m_wndGrid.SetStyleRange(CGXRange(i,j),style);

	Initial_WithoutData();
}

void CDgnSrcBeamDlg::Initial_WithoutData()
{
	Write_GridItem(_T("I"),_T(""),_T(""),_T(""),_T(""));
	Write_GridItem(_T("M"),_T(""),_T(""),_T(""),_T(""));
	Write_GridItem(_T("J"),_T(""),_T(""),_T(""),_T(""));

	m_StirrupRebar.SetCurSel(-1);
	m_Dt.SetEditUnit(0.0);
	m_Db.SetEditUnit(0.0);

	m_StirrupSpaceI = 0.0;
	m_StirrupSpaceC = 0.0;
	m_StirrupSpaceJ = 0.0;
	m_SiCtrl.SetEditUnit(m_StirrupSpaceI);
	m_ScCtrl.SetEditUnit(m_StirrupSpaceC);
	m_SjCtrl.SetEditUnit(m_StirrupSpaceJ);

	m_iStirrupNumI = 0;
	m_iStirrupNumC = 0;
	m_iStirrupNumJ = 0;
}

BOOL CDgnSrcBeamDlg::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	int N;
	CString strTemp1=_T(""),strTemp2=_T(""),strSize=_T("");
	/////////////////////////////////////////////////////////////////////////////////////
	//  I_Node의 Top에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(1,1);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Top_I.nRebar1   = N;
	m_RchkData.Top_I.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(2,1);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Top_I.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Top_I.nRebar1 != 0 && m_RchkData.Top_I.nRebar2 != 0)  
		m_RchkData.Top_I.nArrayType = 2;
	else                                                                
		m_RchkData.Top_I.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	//  I_Node의 Bottom에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(4,1);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Bot_I.nRebar1   = N;
	m_RchkData.Bot_I.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(3,1);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Bot_I.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Bot_I.nRebar1 != 0 && m_RchkData.Bot_I.nRebar2 != 0)  
		m_RchkData.Bot_I.nArrayType = 2;
	else                                                      
		m_RchkData.Bot_I.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	//  Center_Node의 Top에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(1,2);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Top_C.nRebar1   = N;
	m_RchkData.Top_C.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(2,2);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Top_C.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Top_C.nRebar1 != 0 && m_RchkData.Top_C.nRebar2 != 0)  
		m_RchkData.Top_C.nArrayType = 2;
	else                                                      
		m_RchkData.Top_C.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	//  Center_Node의 Bottom에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(4,2);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Bot_C.nRebar1   = N;
	m_RchkData.Bot_C.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(3,2);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Bot_C.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Bot_C.nRebar1 != 0 && m_RchkData.Bot_C.nRebar2 != 0)  
		m_RchkData.Bot_C.nArrayType = 2;
	else                                                      
		m_RchkData.Bot_C.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	//  J_Node의 Top에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(1,3);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Top_J.nRebar1   = N;
	m_RchkData.Top_J.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(2,3);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Top_J.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Top_J.nRebar1 != 0 && m_RchkData.Top_J.nRebar2 != 0)  
		m_RchkData.Top_J.nArrayType = 2;
	else                                                      
		m_RchkData.Top_J.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	//  J_Node의 Bottom에 대한 Data를 얻어옵니다.
	strTemp1 = m_wndGrid.GetValueRowCol(4,3);
	Get_RebarSizeNumByString(strTemp1,N,strSize);
	m_RchkData.Bot_J.nRebar1   = N;
	m_RchkData.Bot_J.RebarName = strSize;
	/////////////////////////////////////////////////////////////////////////////////////
	strTemp2 = m_wndGrid.GetValueRowCol(3,3);
	Get_RebarSizeNumByString(strTemp2,N,strSize);
	m_RchkData.Bot_J.nRebar2 = N;
	/////////////////////////////////////////////////////////////////////////////////////
	if(m_RchkData.Bot_J.nRebar1 != 0 && m_RchkData.Bot_J.nRebar2 != 0)  
		m_RchkData.Bot_J.nArrayType = 2;
	else                                                      
		m_RchkData.Bot_J.nArrayType = 1;
	/////////////////////////////////////////////////////////////////////////////////////
	m_RchkData.dB = m_Db.GetEditValue();
	m_RchkData.dT = m_Dt.GetEditValue();

	int nIndex = m_StirrupRebar.GetCurSel();
	if(nIndex != -1)  m_StirrupRebar.GetLBText(nIndex,m_RchkData.SubRebarName);

	m_RchkData.Top_I.dSubRebarSpace = m_SiCtrl.GetEditValue();
	m_RchkData.Top_C.dSubRebarSpace = m_ScCtrl.GetEditValue();
	m_RchkData.Top_J.dSubRebarSpace = m_SjCtrl.GetEditValue();
	m_RchkData.Top_I.iSubRebarNum	 = m_iStirrupNumI;
	m_RchkData.Top_C.iSubRebarNum	 = m_iStirrupNumC;
	m_RchkData.Top_J.iSubRebarNum	 = m_iStirrupNumJ;

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
		if(!m_pDoc->m_pDataCtrl->AddRbsb(arRchKey, m_RchkData))
			bCheck = FALSE;
	}

	return bCheck;
}
