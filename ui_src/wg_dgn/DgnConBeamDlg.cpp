// DgnConBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamDlg.h"

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
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnConBeamBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlg dialog


CDgnConBeamDlg::CDgnConBeamDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBeamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamDlg)
	m_nCrack = -1;
	m_nOption = -1;
	m_Shape = _T("");
	m_nDeflect = -1;
	//}}AFX_DATA_INIT
	m_iStirrupNumI = 0;
	m_iStirrupNumC = 0;
	m_iStirrupNumJ = 0;

	m_aTobeMovedCtrl.RemoveAll();
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_RebarFrm);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_BGRID);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_INODE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_MIDDLE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_JNODE);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BSTIRRUP);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BSTIRRUPNA);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDT);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDTNA);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDTEQ);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDTUNIT);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDB);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDBNA);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDBEQ);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BDBUNIT);
	m_aTobeMovedCtrl.Add(IDC_DGN_EXECUTE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CLOSE);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_SI);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_SC);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_SJ);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BSIDENA);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_SIDE_NUM);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BSIDENA2);
	m_aTobeMovedCtrl.Add(IDC_DGN_RC_BSIDEBAR);

	m_aDeflectMovedCtrl.RemoveAll();
	m_aDeflectMovedCtrl.Add(IDC_DGN_CONC_DeflectFrm);
	m_aDeflectMovedCtrl.Add(IDC_DGN_RC_BDEFLECT1);
	m_aDeflectMovedCtrl.Add(IDC_DGN_RC_BDEFLECT2);
	m_aDeflectMovedCtrl.Add(IDC_DGN_RC_BDEFLECT3);
	m_aDeflectMovedCtrl.Add(IDC_DGN_RC_BDEFLECT4);

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();

	m_iDeflectEnvNum=0;
	m_iCrackEnvNum=0;
}


void CDgnConBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamDlg)
	DDX_Control(pDX, IDC_DGN_RC_SJ, m_SjCtrl);
	DDX_Control(pDX, IDC_DGN_RC_SC, m_ScCtrl);
	DDX_Control(pDX, IDC_DGN_RC_SI, m_SiCtrl);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP, m_StirrupRebar);
	DDX_Control(pDX, IDC_DGN_RC_BDT, m_Dt);
	DDX_Control(pDX, IDC_DGN_RC_BDB, m_Db);
	DDX_Control(pDX, IDC_DGN_RC_BHF, m_Hf);
	DDX_Control(pDX, IDC_DGN_RC_BHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BBF, m_Bf);
	DDX_Control(pDX, IDC_DGN_RC_BBC, m_Bc);
	DDX_Control(pDX, IDC_DGN_RC_BDBUNIT, m_DbUnit);
	DDX_Control(pDX, IDC_DGN_RC_BDTUNIT, m_DtUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHFUNIT, m_HfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHCUNIT, m_HcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBFUNIT, m_BfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBCUNIT, m_BcUnit);
	DDX_Radio(pDX, IDC_DGN_BEAM_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_RC_BSHAPE, m_Shape);
 	DDX_Radio(pDX, IDC_DGN_RC_BCRACK1, m_nCrack);
	DDX_Radio(pDX, IDC_DGN_RC_BDEFLECT1, m_nDeflect);
	DDX_Control(pDX, IDC_DGN_RC_BSIDEBAR, m_SideRebar);
	DDX_Control(pDX, IDC_DGN_RC_SIDE_NUM, m_SideNum);
	DDX_Control(pDX, IDC_DGN_CONC_BGRID, m_wndGrid);
	DDX_Control(pDX, IDC_DGN_CONC_SELECTGRID, m_SelectGrid.m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBeamDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_BEAM_DEL, OnDgnBeamDel)
	ON_BN_CLICKED(IDC_DGN_BEAM_AR, OnDgnBeamAr)
	ON_BN_CLICKED(IDC_DGN_CONC_INODE, OnDgnConcInode)
	ON_BN_CLICKED(IDC_DGN_CONC_JNODE, OnDgnConcJnode)
	ON_BN_CLICKED(IDC_DGN_CONC_MIDDLE, OnDgnConcMiddle)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_SELECTALL, OnDgnBeamSelectAll)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_UNSELECTALL, OnDgnBeamUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlg message handlers

void CDgnConBeamDlg::Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa)
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

BOOL CDgnConBeamDlg::ErrorCheck()
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

	CString strCode = GetDgnConcCode();
	if (CDBLib::IsConCodeForTorsionCheck(strCode)) // Seungjun '151126 PMS.5165 ACI318-08, 11 보 비틀림 설계 추가
	{
		if(m_SideRebar.GetCurSel()==-1)
		{
			bCheck=FALSE;
			AfxMessageBox(_LS(IDS_DGN_CHK_SIDERAR_DATA),MB_OK);
			return bCheck;
		}	

		int iNum = (int)m_SideNum.GetEditValue();
		if(iNum%2 == 1)
		{
			bCheck=FALSE;
			AfxMessageBox(_LS(IDS_DGN_CHK_SIDERAR_NUM_VALUE),MB_OK);
			return bCheck;
		}
	}

	return bCheck;
}

void CDgnConBeamDlg::OnDgnExecute() 
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
		int nRebbSize = arRchKey.GetSize();
		if(nRebbSize > 0)
		{
			CArray<T_REBB_K,T_REBB_K> aRebbK;
			aRebbK.SetSize(nRebbSize);
			for(int i = 0; i < nRebbSize; i++)
			{
				aRebbK[i].keymap = arRchKey[i];
			}

			if(!m_pDoc->m_pDataCtrl->DelRebb(aRebbK))
				bCheck = FALSE;
		}

		if(bCheck)
			SetData2Dlg();
	}
}

void CDgnConBeamDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConBeamDlg::OnDgnBeamDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConBeamDlg::OnDgnBeamAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

BOOL CDgnConBeamDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	if(m_StirrupRebar.GetCount() > 0)	m_StirrupRebar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_StirrupRebar);
	if(m_SideRebar.GetCount() > 0)	m_SideRebar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SideRebar);
	// Initialize Space Status.
	GetDlgItem(IDC_DGN_RC_SI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_SC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_SJ)->ShowWindow(SW_HIDE);

	InitSectGrid();
	Initial_Unit();
	ShowBitmap(_T("SB"));
	Initial_GridWnd();

	SetData2Dlg();

	ChangeDlgItemInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamDlg::Write_GridTitle()
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
	m_wndGrid.SetColWidth(0,0,globalUtils.ScaleByDPI(35), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_I_NODE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,1),style);
	m_wndGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_MIDDLE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,2),style);
	m_wndGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_J_NODE);
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(0,3),style);
	m_wndGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);
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

void CDgnConBeamDlg::Initial_GridWnd()
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

void CDgnConBeamDlg::Initial_Unit()
{
	m_Hf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Dt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Db.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SiCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ScCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SjCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_SideNum.SetUnitType(D_UNITSYS_NONE);

	m_HfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DbUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DtUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConBeamDlg::ShowBitmap(CString str)
{
	if(str==_T("SB"))
	{
		GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_HIDE);
	}
	else if(str==_T("T"))
	{
		GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnConBeamDlg::Initial_ExistData()
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

	m_iStirrupNumI = m_RchkData.Top_I.iSubRebarNum==0 ? 2 : m_RchkData.Top_I.iSubRebarNum;
	m_iStirrupNumC = m_RchkData.Top_C.iSubRebarNum==0 ? 2 : m_RchkData.Top_C.iSubRebarNum;
	m_iStirrupNumJ = m_RchkData.Top_J.iSubRebarNum==0 ? 2 : m_RchkData.Top_J.iSubRebarNum;

	// Coded by Seungjun MNet:No.2393 ('20060831)
	if(m_RchkData.iCrackEnv <= m_iCrackEnvNum)
		m_nCrack = m_RchkData.iCrackEnv;
	else
		m_nCrack = m_iCrackEnvNum;
	if(m_RchkData.iDelfectEnv <= m_iDeflectEnvNum)
		m_nDeflect = m_RchkData.iDelfectEnv;
	else
		m_nDeflect = m_iDeflectEnvNum;

	m_SideNum.SetEditUnit(m_RchkData.iSideRebarNum);

	int Index = m_StirrupRebar.FindStringExact(-1,m_RchkData.SubRebarName);
	if(Index != -1) m_StirrupRebar.SetCurSel(Index);

	Index = m_SideRebar.FindStringExact(-1,m_RchkData.strSideRebarName);
	if(Index != -1) m_SideRebar.SetCurSel(Index);
}

void CDgnConBeamDlg::Set_SectionShape(CString strShape)
{
	m_Shape = strShape;
	CString strSectShp=_T("");
	if(strShape==_T("SB"))			GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_RECT));
	else if(strShape==_T("T"))	GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_TEE));
	else										GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_RECT));
}

void CDgnConBeamDlg::Set_ShowTSection()
{
	GetDlgItem(IDC_DGN_RC_BBF)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BHF)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DGN_BF_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_HF_STATIC)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DGN_RC_BBFUNIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BHFUNIT)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DGN_BF_COLON)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_HF_COLON)->ShowWindow(SW_SHOW);
}

void CDgnConBeamDlg::Set_ShowSBSection()
{
	GetDlgItem(IDC_DGN_RC_BBF)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BHF)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_DGN_BF_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_HF_STATIC)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_DGN_RC_BBFUNIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BHFUNIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_DGN_BF_COLON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_HF_COLON)->ShowWindow(SW_HIDE);
}

void CDgnConBeamDlg::Write_GridItem(CString Type,CString strNt1,CString strNt2,CString strNb1,CString strNb2)
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

void CDgnConBeamDlg::OnDgnConcInode() 
{

	CDgnConBeamBarDlg dlg;
	
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = m_Shape;
	dlg.m_iMode = 0;
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

void CDgnConBeamDlg::OnDgnConcJnode() 
{
	CDgnConBeamBarDlg dlg;
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = m_Shape;
	dlg.m_iMode = 0;
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

void CDgnConBeamDlg::OnDgnConcMiddle() 
{
	CDgnConBeamBarDlg dlg;
	dlg.m_RchkKey = m_RchkKey;
	dlg.m_BeamShape = m_Shape;
	dlg.m_iMode = 0;
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

void CDgnConBeamDlg::OnDgnDesignSection() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));
}

void CDgnConBeamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			CDBLib::GetRebarNameAtComboBox(&m_StirrupRebar);
			if(m_SideRebar.GetCount() > 0)	m_SideRebar.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_SideRebar);
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConBeamDlg::UpdateBuffer()
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
			case(UR_REBB_ADD):
			case(UR_REBB_DEL):  {bMFD  = TRUE; break;}
			case(UR_DCON_ADD): 
			case(UR_DCON_DEL):  {bDCON = TRUE; break;}
			default:						{bOther=TRUE;	break;}
		}
	}

	if(bMFD) ResetSectGridData(TRUE);
	if(bDCON) ChangeDlgItemInfo();

}

void CDgnConBeamDlg::SelectElemOnView()
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

void CDgnConBeamDlg::ChangeDlgItemInfo()
{
	// Crack.
	GetDlgItem(IDC_DGN_RC_BCRACK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BCRACK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BCRACK3)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_DGN_RC_BCRACK4)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_HIDE);
	/*
	CStringArray aCrackEnv;
	aCrackEnv.RemoveAll();

	int iCrackEnvNum = CDBLib::GetCrackEnvList(aCrackEnv);
	// Coded by Seungjun MNet:No.2393 ('20060831)
	m_iCrackEnvNum = iCrackEnvNum-1;
*/
	//AlignControls(iCrackEnvNum, FALSE);
	AlignControls(0, FALSE);
/*
	if(iCrackEnvNum ==1)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->SetWindowText(aCrackEnv[0]);
	}
	else if(iCrackEnvNum ==2)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->SetWindowText(aCrackEnv[1]);
	}
	else if(iCrackEnvNum == 3)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_RC_BCRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK3)->SetWindowText(aCrackEnv[2]);
	}
	else if(iCrackEnvNum == 4)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_RC_BCRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK3)->SetWindowText(aCrackEnv[2]);
		GetDlgItem(IDC_DGN_RC_BCRACK4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BCRACK4)->SetWindowText(aCrackEnv[3]);
	}
*/
	// Coded by Seungjun MNet:No.2393 ('20060830)
	// Deflection.
	GetDlgItem(IDC_DGN_RC_BDEFLECT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BDEFLECT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BDEFLECT3)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_DGN_RC_BDEFLECT4)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
/*
	CStringArray aDeflectEnv;
	aDeflectEnv.RemoveAll();

	int iDeflectEnvNum = CDBLib::GetConDeflectionEnvList(aDeflectEnv);
	// Coded by Seungjun MNet:No.2393 ('20060831)
	m_iDeflectEnvNum = iDeflectEnvNum-1;
*/
	//AlignControls(iDeflectEnvNum, TRUE);
	AlignControls(0, TRUE);

	SetSideRebarControl();
/*
	if(iDeflectEnvNum ==1)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
	}
	else if(iDeflectEnvNum ==2)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
	}
	else if(iDeflectEnvNum == 3)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT3)->SetWindowText(aDeflectEnv[2]);
	}
	else if(iDeflectEnvNum == 4)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT3)->SetWindowText(aDeflectEnv[2]);
		GetDlgItem(IDC_DGN_RC_BDEFLECT4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BDEFLECT4)->SetWindowText(aDeflectEnv[3]);
	}
*/
	UpdateData(FALSE);
	Invalidate();
}

// Coded by Seungjun MNet:No.2393 ('20060830)
void CDgnConBeamDlg::AlignControls(int iEnvNum, BOOL bWhich)
{
	CRect RectDlg, rRef, rToMove;
	CRect rRect1, rRect2;
	
	if(!bWhich)
	{
		GetWindowRect(&RectDlg);
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->GetWindowRect(rRef);
	  GetDlgItem(IDC_DGN_CONC_DeflectFrm)->GetWindowRect(rToMove);
		GetDlgItem(IDC_DGN_RC_BCRACK1)->GetWindowRect(rRect1);
		GetDlgItem(IDC_DGN_RC_BCRACK2)->GetWindowRect(rRect2);

		CRect rCrack;
		rCrack.left = rRef.left;
		rCrack.top = rRef.top;
		rCrack.right = rRef.right;
		rCrack.bottom = rRef.top + iEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top);
		ScreenToClient(rCrack);
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->SetWindowPos(NULL, rCrack.left, rCrack.top, rCrack.Width(), rCrack.Height(), SWP_DRAWFRAME);

		int nDistY = 0;
		if(iEnvNum == 0)
			nDistY = rRef.top - rToMove.top;
		else
			nDistY = rRef.top - rToMove.top + iEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top) + globalUtils.ScaleByDPI(8);

		CDlgUtil::CtrlMoveDistY(this, m_aDeflectMovedCtrl, nDistY); // Coded by Seungjun MNet:No.2393 ('20060830)
	}
	else
	{
		// Coded by Seungjun MNet:No.2393 ('20060830)
		GetWindowRect(&RectDlg);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_RebarFrm)->GetWindowRect(rToMove);
		GetDlgItem(IDC_DGN_RC_BDEFLECT1)->GetWindowRect(rRect1);
		GetDlgItem(IDC_DGN_RC_BDEFLECT2)->GetWindowRect(rRect2);
		
		CRect rDflect;
		rDflect.left = rRef.left;
		rDflect.top = rRef.top;
		rDflect.right = rRef.right;
		rDflect.bottom = rRef.top + iEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top);
		ScreenToClient(rDflect);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->SetWindowPos(NULL, rDflect.left, rDflect.top, rDflect.Width(), rDflect.Height(), SWP_DRAWFRAME);

		int nDistY = 0;
		if(iEnvNum == 0)
			nDistY = rRef.top - rToMove.top;
		else
			nDistY = rRef.top - rToMove.top + iEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top) + globalUtils.ScaleByDPI(8);

		CDlgUtil::CtrlMoveDistY(this, m_aTobeMovedCtrl, nDistY);  		
	}
}

void CDgnConBeamDlg::SetSideRebarControl()
{
	//081030 sshan:M_net:3780
	CRect RectMainDlg, RectRebarGroup, RectSideBar, RectdB, RectSi, RectSc, RectSj, RectApply, RectClose;
	GetWindowRect(&RectMainDlg);
	GetDlgItem(IDC_DGN_CONC_RebarFrm)->GetWindowRect(RectRebarGroup);
	GetDlgItem(IDC_DGN_RC_BSIDEBAR)->GetWindowRect(RectSideBar);
	GetDlgItem(IDC_DGN_RC_BDB)->GetWindowRect(RectdB);
	GetDlgItem(IDC_DGN_RC_SI)->GetWindowRect(RectSi);
	GetDlgItem(IDC_DGN_RC_SC)->GetWindowRect(RectSc);
	GetDlgItem(IDC_DGN_RC_SJ)->GetWindowRect(RectSj);
	GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(RectApply);
	GetDlgItem(IDC_DGN_CLOSE)->GetWindowRect(RectClose);

	CRect rMainDlg, rGroup, rSi, rSc, rSj, rApply, rClose;
	
	CString strCode = GetDgnConcCode();
	if (CDBLib::IsConCodeForTorsionCheck(strCode)) // Seungjun '151126 PMS.5165 ACI318-08, 11 보 비틀림 설계 추가
	{
		GetDlgItem(IDC_DGN_RC_BSIDENA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_SIDE_NUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BSIDENA2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_BSIDEBAR)->ShowWindow(SW_SHOW);						

		rGroup.left		= RectRebarGroup.left;			
		rGroup.right	= RectRebarGroup.right;
		rGroup.top		= RectRebarGroup.top;
		rGroup.bottom = RectSideBar.bottom + RectSideBar.Height()*0.3;			
	}
	else
	{
		GetDlgItem(IDC_DGN_RC_BSIDENA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_SIDE_NUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_BSIDENA2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_BSIDEBAR)->ShowWindow(SW_HIDE);						

		rGroup.left		= RectRebarGroup.left;			
		rGroup.right	= RectRebarGroup.right;
		rGroup.top		= RectRebarGroup.top;
		rGroup.bottom = RectdB.bottom + RectSideBar.Height()*0.5;			
	}

	rMainDlg.left		= RectMainDlg.left;			
	rMainDlg.right	= RectMainDlg.right;
	rMainDlg.top		= RectMainDlg.top;
	rMainDlg.bottom = rGroup.bottom + RectApply.Height()*2.5;			

	rSi.left		= RectSi.left;			
	rSi.right		= RectSi.right;
	rSi.top			= rGroup.bottom + RectApply.Height()*0.3;//rMainDlg.bottom - RectSideBar.Height()*0.5 - RectSi.Height();
	rSi.bottom	= rSi.top + RectSi.Height();

	rSc.left		= RectSc.left;			
	rSc.right		= RectSc.right;
	rSc.top			= rGroup.bottom + RectApply.Height()*0.3;//rMainDlg.bottom - RectSideBar.Height()*0.5 - RectSc.Height();
	rSc.bottom	= rSc.top + RectSc.Height();

	rSj.left		= RectSj.left;			
	rSj.right		= RectSj.right;
	rSj.top			= rGroup.bottom + RectApply.Height()*0.3;//rMainDlg.bottom - RectSideBar.Height()*0.5 - RectSj.Height();
	rSj.bottom	= rSj.top + RectSj.Height();

	rApply.left		= RectApply.left;			
	rApply.right	= RectApply.right;
	rApply.top		= rGroup.bottom + RectApply.Height()*0.3;//rMainDlg.bottom - RectSideBar.Height()*0.5 - RectApply.Height();
	rApply.bottom	= rApply.top + RectApply.Height();

	rClose.left		= RectClose.left;			
	rClose.right	= RectClose.right;
	rClose.top		= rGroup.bottom + RectApply.Height()*0.3;//rMainDlg.bottom - RectSideBar.Height()*0.5 - RectClose.Height();
	rClose.bottom	= rClose.top + RectClose.Height();

	ScreenToClient(rMainDlg);
	ScreenToClient(rGroup);
	ScreenToClient(rSi);
	ScreenToClient(rSc);
	ScreenToClient(rSj);
	ScreenToClient(rApply);
	ScreenToClient(rClose);

	SetWindowPos(NULL, rMainDlg.left, rMainDlg.top, rMainDlg.Width(), rMainDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDC_DGN_CONC_RebarFrm)->SetWindowPos(NULL, rGroup.left, rGroup.top, rGroup.Width(), rGroup.Height(), NULL);
	GetDlgItem(IDC_DGN_RC_SI)->SetWindowPos(NULL, rSi.left, rSi.top, rSi.Width(), rSi.Height(), SWP_NOSIZE);
	GetDlgItem(IDC_DGN_RC_SC)->SetWindowPos(NULL, rSc.left, rSc.top, rSc.Width(), rSc.Height(), SWP_NOSIZE);
	GetDlgItem(IDC_DGN_RC_SJ)->SetWindowPos(NULL, rSj.left, rSj.top, rSj.Width(), rSj.Height(), SWP_NOSIZE);
	GetDlgItem(IDC_DGN_EXECUTE)->SetWindowPos(NULL, rApply.left, rApply.top, rApply.Width(), rApply.Height(), SWP_NOSIZE);
	GetDlgItem(IDC_DGN_CLOSE)->SetWindowPos(NULL, rClose.left, rClose.top, rClose.Width(), rClose.Height(), SWP_NOSIZE);
}

void CDgnConBeamDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}


void CDgnConBeamDlg::InitSectGrid()
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
				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T"))) 
					 m_arBeamData.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	m_SelectGrid.InitSectGrid(m_arBeamData,3);
}

void CDgnConBeamDlg::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnConBeamDlg::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();
	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData)) return;

	if(SectData.SectBefore.Shape==_T("T"))
	{
		Set_SectionShape(_T("T"));
		ShowBitmap(_T("T"));

		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bf.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[2]);   //  Tw
		m_Hf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);   //  Tf1

		Set_ShowTSection();
	}
	else if(SectData.SectBefore.Shape==_T("SB"))
	{
		Set_SectionShape(_T("SB"));
		ShowBitmap(_T("SB"));

		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B

		Set_ShowSBSection();
	}

	m_RchkData.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetRebb(m_RchkKey, m_RchkData))  Initial_ExistData();
	else        Initial_WithoutData();

	UpdateData(FALSE);
	UpdateData(TRUE);
	if(m_SelectGrid.IsExistEnabledSect())  
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(FALSE);
	}

}

LRESULT CDgnConBeamDlg::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{
	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnConBeamDlg::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnConBeamDlg::OnDgnBeamSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnConBeamDlg::OnDgnBeamUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

void CDgnConBeamDlg::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	ResetSectGridData();

	m_Shape = _T("");
	m_nCrack = 0;
	m_nDeflect = 0; // Coded by Seungjun MNet:No.2393 ('20060831)
	m_nOption = 0;    //  add/replace

	//  대화상자가 바뀌면 변수를 초기화합니다.
	m_Hf.SetEditUnit(0.0);
	m_Hc.SetEditUnit(0.0);
	m_Bf.SetEditUnit(0.0);
	m_Bc.SetEditUnit(0.0);
	m_Dt.SetEditUnit(0.0);
	m_Db.SetEditUnit(0.0);

	m_SideNum.SetEditUnit(0);

	m_Hf.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_Bf.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);

	//  Stirrup Rebar를 초기화시킵니다.
	m_StirrupRebar.SetCurSel(-1);
	m_SideRebar.SetCurSel(-1);

	GetDlgItem(IDC_DGN_RC_BSHAPE)->EnableWindow(FALSE);

	UpdateData(FALSE);

	// PropNo가 선택이 안되었으므로..
	if(m_SelectGrid.m_arKey.GetCount() <= 0)
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(FALSE);
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

void CDgnConBeamDlg::Initial_WithoutData()
{
	Write_GridItem(_T("I"),_T(""),_T(""),_T(""),_T(""));
	Write_GridItem(_T("M"),_T(""),_T(""),_T(""),_T(""));
	Write_GridItem(_T("J"),_T(""),_T(""),_T(""),_T(""));

	m_StirrupRebar.SetCurSel(-1);
	m_Dt.SetEditUnit(0.0);
	m_Db.SetEditUnit(0.0);

	m_SideNum.SetEditUnit(0);
	m_SideRebar.SetCurSel(-1);

	m_StirrupSpaceI = 0.0;
	m_StirrupSpaceC = 0.0;
	m_StirrupSpaceJ = 0.0;
	m_SiCtrl.SetEditUnit(m_StirrupSpaceI);
	m_ScCtrl.SetEditUnit(m_StirrupSpaceC);
	m_SjCtrl.SetEditUnit(m_StirrupSpaceJ);

	m_iStirrupNumI = 2;
	m_iStirrupNumC = 2;
	m_iStirrupNumJ = 2;
}

BOOL CDgnConBeamDlg::SetDlg2Data()
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

	nIndex = m_SideRebar.GetCurSel();
	if(nIndex != -1)  m_SideRebar.GetLBText(nIndex,m_RchkData.strSideRebarName);
	m_RchkData.iSideRebarNum = m_SideNum.GetEditValue();

	m_RchkData.Top_I.dSubRebarSpace = m_SiCtrl.GetEditValue();
	m_RchkData.Top_C.dSubRebarSpace = m_ScCtrl.GetEditValue();
	m_RchkData.Top_J.dSubRebarSpace = m_SjCtrl.GetEditValue();
	m_RchkData.Top_I.iSubRebarNum	 = m_iStirrupNumI;
	m_RchkData.Top_C.iSubRebarNum	 = m_iStirrupNumC;
	m_RchkData.Top_J.iSubRebarNum	 = m_iStirrupNumJ;

	m_RchkData.iCrackEnv = m_nCrack;
	// Coded by Seungjun MNet:No.2393 ('20060831)
	m_RchkData.iDelfectEnv = m_nDeflect;

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

	int nRebbSize = arRchKey.GetSize();
	if(nRebbSize > 0)
	{
		CArray<T_REBB_K,T_REBB_K> aRebbK;
		aRebbK.SetSize(nRebbSize);
		for(int i = 0; i < nRebbSize; i++)
		{
			aRebbK[i].keymap = arRchKey[i];
		}

		if(!m_pDoc->m_pDataCtrl->AddRebb(aRebbK, m_RchkData))
			bCheck = FALSE;
	}

	return bCheck;
}

CString CDgnConBeamDlg::GetDgnConcCode()
{
	CString str = _T("");

	if(m_pDoc->m_pAttrCtrl->ExistDcon())
	{
		T_DCON_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDcon(rData);
		str = rData.DesignCode;
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		str = rPref.DgnCode.ConcCode;		
	}

	return str;
}
