// DgnConBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarDlg.h"

#include "DgnConBarSizeDlg.h"
#include "DgnConBarInfoDlg.h"
#include "DgnConBarSpaceDlg.h"
#include "DgnConWallDataDlg.h"

#include "DgnConBarSectDlg.h"
#include "MatlByRebarDiaDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DBCodeDef.h"

#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

#include "..\MIT_lib\MRegistry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarDlg dialog


CDgnConBarDlg::CDgnConBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarDlg)
	m_BMainBar = _T("");
	m_CMainBar = _T("");
	m_BrMainBar = _T("");
	m_WVBar = _T("");
	m_Db = 0.0;
	m_Dt = 0.0;
	m_cDo = 0.0;
	m_brDo = 0.0;
	m_BEHSpace = 0.0;
	m_BEVSpace = 0.0;
	m_De = 0.0;
	m_Dw = 0.0;
	m_bAIJBeamRecommend = FALSE;
	m_iSpliceSubType = 0;
	m_bsplice_radio = 0;
	m_bAIJColRecommend = FALSE;
	m_csplice_radio = 0;
	m_bAIJBraRecommend = FALSE;
	m_rsplice_radio = 0;
	m_bDoublyRain = FALSE;
	m_dDoublyRain = 0.0;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBeam = TRUE;
	m_bConsiderSpacingLimitColumn = TRUE;
	m_bConsiderSpacingLimitBrace = TRUE;
	//m_bVerEndWall = FALSE;
	//}}AFX_DATA_INIT

	m_Dcrb.Initialize();
	m_strCode = _T("");
}


void CDgnConBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBarDlg)
	DDX_Control(pDX, IDC_STATIC_WALL_BEHSPACE_UNIT, m_BEHSpaceUnit);
	DDX_Control(pDX, IDC_STATIC_WALLDWUNIT, m_DwUnit);
	DDX_Control(pDX, IDC_STATIC_WALLDEUNIT, m_DeUnit);
	DDX_Control(pDX, IDC_STATIC_COLDOUNIT, m_cDoUnit);
	DDX_Control(pDX, IDC_STATIC_BRACEDOUNIT, m_brDoUnit);
	DDX_Control(pDX, IDC_STATIC_BEAMDTUNIT, m_DtUnit);
	DDX_Control(pDX, IDC_STATIC_BEAMDBUNIT, m_DbUnit);
	DDX_Control(pDX, IDC_DGN_RC_WENDREBAR, m_WEndBar);
	DDX_Control(pDX, IDC_DGN_RC_WHBAR, m_WHBar);
	DDX_Control(pDX, IDC_DGN_RC_BEWHBAR, m_BEWHBar);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP, m_BStirrup);
	DDX_Control(pDX, IDC_DGN_RC_BSIDE, m_BSide);
	DDX_Control(pDX, IDC_DGN_RC_CSTIRRUP, m_CStirrup);
	DDX_Control(pDX, IDC_DGN_RC_BRSTIRRUP, m_BrStirrup);
	DDX_Control(pDX, IDC_DGN_RC_CSTIRRUP_YNUM, m_CStirrupYNum);
	DDX_Control(pDX, IDC_DGN_RC_CSTIRRUP_ZNUM, m_CStirrupZNum);
	DDX_Control(pDX, IDC_DGN_RC_BRSTIRRUP_YNUM, m_BrStirrupYNum);
	DDX_Control(pDX, IDC_DGN_RC_BRSTIRRUP_ZNUM, m_BrStirrupZNum);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_NUM, m_BStirrupNum);
	DDX_Control(pDX, IDC_DGN_RC_BEAM_DOUBLY_REIN_EDIT, m_DoublyRainEdit);
	DDX_Check(pDX, IDC_DGN_CON_AIJ_DOUBLY_REIN_CHECK, m_bDoublyRain);
	DDX_Text(pDX, IDC_DGN_RC_BEAM_DOUBLY_REIN_EDIT, m_dDoublyRain);
	DDX_Text(pDX, IDC_DGN_RC_BMAINBAR, m_BMainBar);
	DDX_Text(pDX, IDC_DGN_RC_CMAINBAR, m_CMainBar);
	DDX_Text(pDX, IDC_DGN_RC_BRMAINBAR, m_BrMainBar);
	DDX_Text(pDX, IDC_DGN_RC_WVBAR, m_WVBar);
	DDX_Text(pDX, IDC_DGN_RC_BEAMDB, m_Db);
	DDX_Text(pDX, IDC_DGN_RC_BEAMDT, m_Dt);
	DDX_Text(pDX, IDC_DGN_RC_BRACEDO, m_brDo);
	DDX_Text(pDX, IDC_DGN_RC_COLDO, m_cDo);
	DDX_Text(pDX, IDC_DGN_RC_WALL_BEHSPACE_EDT, m_BEHSpace);
	DDX_Control(pDX, IDC_STATIC_WALL_BEVSPACE_UNIT, m_BEVSpaceUnit);
	DDX_Text(pDX, IDC_DGN_RC_WALL_BEVSPACE_EDT, m_BEVSpace);
	DDX_Text(pDX, IDC_DGN_RC_WALLDE, m_De);
	DDX_Text(pDX, IDC_DGN_RC_WALLDW, m_Dw);	
	//add by cylee 05.06.24
	DDX_Check(pDX, IDC_DGN_CON_AIJ_BWIDTH_CHECK, m_bAIJBeamRecommend);
	DDX_Radio(pDX, IDC_DGN_CON_AIJ_METHOD1_RADIO,m_iSpliceSubType);
	DDX_Radio(pDX, IDC_DGN_CON_BSPLICE_NO_RADIO, m_bsplice_radio);
	DDX_Check(pDX, IDC_DGN_CON_AIJ_CWIDTH_CHECK, m_bAIJColRecommend);
	DDX_Radio(pDX, IDC_DGN_CON_CSPLICE_NO_RADIO, m_csplice_radio);
	DDX_Check(pDX, IDC_DGN_CON_AIJ_RWIDTH_CHECK, m_bAIJBraRecommend);
	DDX_Radio(pDX, IDC_DGN_CON_RSPLICE_NO_RADIO, m_rsplice_radio);
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	DDX_Check(pDX, IDC_DGN_CON_BSPACE_LIMIT, m_bConsiderSpacingLimitBeam);
	DDX_Check(pDX, IDC_DGN_CON_CSPACE_LIMIT, m_bConsiderSpacingLimitColumn);
	DDX_Check(pDX, IDC_DGN_CON_RSPACE_LIMIT, m_bConsiderSpacingLimitBrace);
	// Add by sshan (090222) NO:3909
	//DDX_Check(pDX, IDC_DGN_CON_VER_END_CHECK, m_bVerEndWall);
	//DDX_Control(pDX, IDC_DGN_RC_VER_END_WALL, m_ComboVerEndWall);
	//DDX_Control(pDX, IDC_DGN_RC_VER_END_FY_WALL, m_ComboVerEndFyWall);

	DDX_Control(pDX, IDC_DGN_CON_MATL_RBAR_DIAM_CHK, m_chkMatlByRbar);
	DDX_Control(pDX, IDC_WG_DGN_STATIC0, m_wndBeamDgnFrame);
	DDX_Control(pDX, IDC_WG_DGN_STATIC13, m_wndColmDgnFrame);
	DDX_Control(pDX, IDC_WG_DGN_STATIC24, m_wndBrtrDgnFrame);
	DDX_Control(pDX, IDC_DGN_WALL_GROUP_STATIC, m_wndWallDgnFrame);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBarDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConBarDlg)
	ON_BN_CLICKED(IDC_DGN_RC_SELBARSIZE1, OnDgnRcSelbarsize1)
	ON_BN_CLICKED(IDC_DGN_RC_SELBARSIZE2, OnDgnRcSelbarsize2)
	ON_BN_CLICKED(IDC_DGN_RC_SELBARSIZE3, OnDgnRcSelbarsize3)
	ON_BN_CLICKED(IDC_DGN_RC_SELBARSIZE4, OnDgnRcSelbarsize4)
	ON_BN_CLICKED(IDC_DGN_ADD_WALLDATA,   OnDgnAddWalldata)
	ON_BN_CLICKED(IDC_DGN_CON_AIJ_BWIDTH_CHECK,OnDgnAIJWidCheck)
	ON_BN_CLICKED(IDC_DGN_CON_AIJ_CWIDTH_CHECK,OnDgnAIJWidCheck)
	ON_BN_CLICKED(IDC_DGN_CON_AIJ_RWIDTH_CHECK,OnDgnAIJWidCheck)
	ON_BN_CLICKED(IDC_DGN_RC_BEAM_BTN, OnDgnRcBeam)
	ON_BN_CLICKED(IDC_DGN_RC_COLUMN_BTN, OnDgnRcColumn)
	ON_BN_CLICKED(IDC_DGN_RC_BRACE_BTN, OnDgnRcBrace)
	ON_BN_CLICKED(IDC_DGN_RC_WALL_BTN, OnDgnRcWall)
	ON_BN_CLICKED(IDC_DGN_CON_AIJ_DOUBLY_REIN_CHECK,OnDgnBeamDoublyReinCheck)
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	ON_BN_CLICKED(IDC_DGN_CON_BSPACE_LIMIT,OnDgnConsiderSpacingLimitCheck)
	ON_BN_CLICKED(IDC_DGN_CON_CSPACE_LIMIT,OnDgnConsiderSpacingLimitCheck)
	ON_BN_CLICKED(IDC_DGN_CON_RSPACE_LIMIT,OnDgnConsiderSpacingLimitCheck)
	// Add by sshan (090222) NO:3909
	//ON_BN_CLICKED(IDC_DGN_CON_VER_END_CHECK, OnDgnRcVerEndWall)
	ON_BN_CLICKED(IDC_DGN_CON_MATL_RBAR_DIAM_CHK, OnMatlRbarDiamCheck)
	ON_BN_CLICKED(IDC_DGN_CON_REBAR_MATL_BTN, OnMatlRbarDiamBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarDlg message handlers

void CDgnConBarDlg::OnDgnRcWall() 
{
	CDgnConBarSectDlg dlg;
	dlg.SetTabOrder(3);
	if(dlg.DoModal() == IDOK)
	{    
	}
}

void CDgnConBarDlg::OnDgnRcBrace() 
{
	CDgnConBarSectDlg dlg;
	dlg.SetTabOrder(2);
	if(dlg.DoModal() == IDOK)
	{    
	}
}

void CDgnConBarDlg::OnDgnRcBeam() 
{
	CDgnConBarSectDlg dlg;
	dlg.SetTabOrder(0);
	if(dlg.DoModal() == IDOK)
	{    
	}
}

void CDgnConBarDlg::OnDgnRcColumn() 
{
	CDgnConBarSectDlg dlg;
	dlg.SetTabOrder(1);
	if(dlg.DoModal() == IDOK)
	{    
	}
}

void CDgnConBarDlg::OnDgnRcSelbarsize1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_BMainBar;
	if (dlg.DoModal() == IDOK)
	{
		m_BMainBar = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnConBarDlg::OnDgnRcSelbarsize2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_CMainBar;
	if (dlg.DoModal() == IDOK)
	{
		m_CMainBar = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnConBarDlg::OnDgnRcSelbarsize3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_BrMainBar;
	if (dlg.DoModal() == IDOK)
	{
		m_BrMainBar = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnConBarDlg::OnDgnRcSelbarsize4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_WVBar;
	if (dlg.DoModal() == IDOK)
	{
		m_WVBar = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnConBarDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CArray<CString,CString> arRebarSize;
	int Index=0;
	CString strBStirrup=_T(""),strBSide=_T(""),strCStirrup=_T(""),strBrStirrup=_T(""),strWHBar=_T(""),strWEndBar=_T(""),strBEWHBar=_T("");
	CString strBStirrupNum=_T(""), strCStirrupNum=_T(""), strBrStirrupNum=_T("");
	///////////////////////////////////////////////////////
	// Beam (Main Rebar, Stirrup, Side Rebar, Covering).
	Cutting_ReBarSize(m_BMainBar, arRebarSize);
	int iCount = arRebarSize.GetSize();
	for(int i=0; i<iCount; i++)	m_Dcrb.MainRebarBeam[i] = arRebarSize.GetAt(i);
	for(int i=iCount; i<5; i++)			m_Dcrb.MainRebarBeam[i] = _T("");
	// Beam Stirrup Rebar Data.
	Index = m_BStirrup.GetCurSel();
	m_BStirrup.GetLBText(Index,strBStirrup);
	m_Dcrb.StirrupBarBeam = strBStirrup;
	// Beam Stirrup Rebar Arrangement.
	Index = m_BStirrupNum.GetCurSel();
	m_BStirrupNum.GetLBText(Index,strBStirrupNum);
	m_Dcrb.iSubRebarNum_Beam = _ttoi(strBStirrupNum);
	// Beam Side Rebar Data.
	Index = m_BSide.GetCurSel();
	m_BSide.GetLBText(Index,strBSide);
	m_Dcrb.SideBarBeam = strBSide;
	// Beam Covering.
	m_Dcrb.dT_Beam = m_Dt;
	m_Dcrb.dB_Beam = m_Db;
	///////////////////////////////////////////////////////
	// Column.
	Cutting_ReBarSize(m_CMainBar,arRebarSize);
	iCount = arRebarSize.GetSize();
	for(int i=0; i<iCount; i++)	m_Dcrb.MainRebarColumn[i] = arRebarSize.GetAt(i);
	for(int i=iCount; i<5; i++)	m_Dcrb.MainRebarColumn[i] = _T("");
	// Column Stirrup Rebar Data.
	Index = m_CStirrup.GetCurSel();
	m_CStirrup.GetLBText(Index,strCStirrup);
	m_Dcrb.StirrupBarColumn = strCStirrup;
	// Column Stirrup Rebar Arrangement.
	// Y.
	Index = m_CStirrupYNum.GetCurSel();
	m_CStirrupYNum.GetLBText(Index,strCStirrupNum);
	m_Dcrb.iSubRebarNum_Column[0] = _ttoi(strCStirrupNum);
	// Z.
	Index = m_CStirrupZNum.GetCurSel();
	m_CStirrupZNum.GetLBText(Index,strCStirrupNum);
	m_Dcrb.iSubRebarNum_Column[1] = _ttoi(strCStirrupNum);
	
	// Column Covering.
	m_Dcrb.d0_Column = m_cDo;
	///////////////////////////////////////////////////////
	// Brace.
	Cutting_ReBarSize(m_BrMainBar,arRebarSize);
	iCount = arRebarSize.GetSize();
	for(int i=0; i<iCount; i++)	m_Dcrb.MainRebarBrace[i] = arRebarSize.GetAt(i);
	for(int i=iCount; i<5; i++)	m_Dcrb.MainRebarBrace[i] = _T("");
	// Brace Stirrup Rebar Data.
	Index = m_BrStirrup.GetCurSel();
	m_BrStirrup.GetLBText(Index,strBrStirrup);
	m_Dcrb.StirrupBarBrace = strBrStirrup;
	// Brace Stirrup Rebar Arrangement.
	// Y.
	Index = m_BrStirrupYNum.GetCurSel();
	m_BrStirrupYNum.GetLBText(Index,strBrStirrupNum);
	m_Dcrb.iSubRebarNum_Brace[0] = _ttoi(strBrStirrupNum);
	// Z.
	Index = m_BrStirrupZNum.GetCurSel();
	m_BrStirrupZNum.GetLBText(Index,strBrStirrupNum);
	m_Dcrb.iSubRebarNum_Brace[1] = _ttoi(strBrStirrupNum);
	
	// Brace Covering.
	m_Dcrb.d0_Brace = m_brDo;
	///////////////////////////////////////////////////////
	// Wall.
	Cutting_ReBarSize(m_WVBar,arRebarSize);
	iCount = arRebarSize.GetSize();
	for(int i=0; i<iCount; i++)	m_Dcrb.VerticalRebarWall[i] = arRebarSize.GetAt(i);
	for(int i=iCount; i<5; i++)	m_Dcrb.VerticalRebarWall[i] = _T("");
	// Wall Horizontal Rebar Data.
	Index = m_WHBar.GetCurSel();
	m_WHBar.GetLBText(Index,strWHBar);
	m_Dcrb.HorizontalRebarWall = strWHBar;
	// Wall End Rebar Data.
	Index = m_WEndBar.GetCurSel();
	m_WEndBar.GetLBText(Index,strWEndBar);
	m_Dcrb.EndRebarWall = strWEndBar;
	// Wall Boundary Element Horizontal Rebar Data.
	Index = m_BEWHBar.GetCurSel();
	m_BEWHBar.GetLBText(Index,strBEWHBar);
	m_Dcrb.BEHorizontalRebarWall = strBEWHBar;
	m_Dcrb.dBEHorizontalRebarSpace = m_BEHSpace;
	m_Dcrb.dBEVerticalRebarSpace = m_BEVSpace;
	// Wall Covering.
	m_Dcrb.dDe_Wall = m_De;
	m_Dcrb.dDw_Wall = m_Dw;
	// AIJ Recommendation Check, add by cylee 05.0624
	m_Dcrb.bSpliceBeamJP = m_bAIJBeamRecommend;			
	m_Dcrb.bSpliceColumnJP = m_bAIJColRecommend;		
	m_Dcrb.bSpliceBraceJP = m_bAIJBraRecommend;		
	m_Dcrb.iSpliceBeamSubType = m_iSpliceSubType;
	// Beam, Column, Brace Splice, add by cylee 05.0624
	m_Dcrb.iSpliceBeam = m_bsplice_radio;				
	m_Dcrb.iSpliceColumn = m_csplice_radio;			
	m_Dcrb.iSpliceBrace = m_rsplice_radio;			
	// Add by sshan. MNET:2619.('20070129)
	m_Dcrb.dDoublyBeam = m_dDoublyRain;
	m_Dcrb.bDoublyBeam = m_bDoublyRain;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_Dcrb.bCheckRebarSpacing_Beam   = m_bConsiderSpacingLimitBeam;
	m_Dcrb.bCheckRebarSpacing_Column = m_bConsiderSpacingLimitColumn;
	m_Dcrb.bCheckRebarSpacing_Brace  = m_bConsiderSpacingLimitBrace;

	//m_Dcrb.bVerEndBar_Wall   = m_bVerEndWall;
// 	CString strVerEndWall;
// 	Index = m_ComboVerEndWall.GetCurSel();
// 	m_ComboVerEndWall.GetLBText(Index,strVerEndWall);
// 	m_Dcrb.strVerEndBar_Wall = strVerEndWall;
// 	CString strVerEndFyWall;
// 	Index = m_ComboVerEndFyWall.GetCurSel();
// 	m_ComboVerEndFyWall.GetLBText(Index,strVerEndFyWall);
// 	m_Dcrb.strVerEndBarFy_Wall = strVerEndFyWall;

	m_Dcrb.bMatlByDia = m_chkMatlByRbar.GetCheck();

	// Save Data at DB.
	if(m_pDoc->m_pDataCtrl->AddDcrb(m_Dcrb))	CDialogMove::OnOK();
}

void CDgnConBarDlg::SetBEControlByRCSpecialWall()
{
	BOOL bEnable = m_pDoc->m_pAttrCtrl->IsWallEndHorBarCondition();
	
	// Boundary Element Horz. Rebar
	GetDlgItem(IDC_DGN_RC_BEWHBAR1_STC)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_DGN_RC_BEWHBAR2_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_RC_BEWHBAR)->EnableWindow(bEnable);

	// Boundary Element Horz. Rebar Space
	GetDlgItem(IDC_DGN_RC_BEWHBAR3_STC)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_DGN_RC_BEWHBAR4_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_RC_WALL_BEHSPACE_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_WALL_BEHSPACE_UNIT)->EnableWindow(bEnable);

	// Boundary Element Vert. Rebar Space
	bEnable = m_pDoc->m_pAttrCtrl->IsWallEndVerBarCondition();
	GetDlgItem(IDC_DGN_RC_BEWVBAR3_STC)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_DGN_RC_BEWVBAR4_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_RC_WALL_BEVSPACE_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_WALL_BEVSPACE_UNIT)->EnableWindow(bEnable);
}

BOOL CDgnConBarDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();



	SetBEControlByRCSpecialWall();

	// Initialize Data.
	Initial_EditStaus(m_pDoc);
	if(m_pDoc->m_pAttrCtrl->ExistDcrb())	Initial_ExistData();
	else									Initial_DefaultData();
	UpdateData(FALSE);
	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	Enable_EndBarEdit();

	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	m_strCode = rData.DesignCode;
	AlignCtrl(m_strCode);

	m_wndBeamDgnFrame.InitControl(this, CDgnConBarDlg::IDD, IDC_WG_DGN_STATIC0, TRUE);
	m_wndColmDgnFrame.InitControl(this, CDgnConBarDlg::IDD, IDC_WG_DGN_STATIC13, TRUE);
	m_wndBrtrDgnFrame.InitControl(this, CDgnConBarDlg::IDD, IDC_WG_DGN_STATIC24, TRUE);
	m_wndWallDgnFrame.InitControl(this, CDgnConBarDlg::IDD, IDC_DGN_WALL_GROUP_STATIC, TRUE);
	m_wndBeamDgnFrame.SetFoldCurHeight(TRUE);
	m_wndColmDgnFrame.SetFoldCurHeight(TRUE);
	m_wndBrtrDgnFrame.SetFoldCurHeight(TRUE);
	m_wndWallDgnFrame.SetFoldCurHeight(TRUE);

	return TRUE;
}

void CDgnConBarDlg::AlignCtrl(const CString& strCode)
{
	if (strCode != _T("AIJ-WSD99"))
	{
		GetDlgItem(IDC_DGN_CON_AIJ_BWIDTH_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD1_RADIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD2_RADIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_AIJ_CWIDTH_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_AIJ_RWIDTH_CHECK)->ShowWindow(SW_HIDE);
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		OnDgnConsiderSpacingLimitCheck();
		GetDlgItem(IDC_DGN_CON_BSPACE_LIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_CSPACE_LIMIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_RSPACE_LIMIT)->EnableWindow(TRUE);
	}
	else
	{
		OnDgnAIJWidCheck();
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		GetDlgItem(IDC_DGN_CON_BSPACE_LIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_CSPACE_LIMIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_RSPACE_LIMIT)->EnableWindow(FALSE);
	}

	// Add by sshan. MNET:2619.('20070126)
	// TWN 일경우 Doubly Reinforced Beam Design 보이게 함
	//if(strCode != _T("TWN-USD92"))
	if (!CDgnCodeCtrl::IsConCodeUSD(strCode))
	{
		GetDlgItem(IDC_DGN_CON_AIJ_DOUBLY_REIN_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_Rhob)->ShowWindow(SW_HIDE);
	}
	else
	{
		OnDgnBeamDoublyReinCheck();
	}

	// Change by ZINU.('08.03.21). KCI-USD07 -> KCI-USD07, ACI318-02, ACI318-05.
	if (CDBLib::IsConCodeRhomax075(strCode))
	{
		SetDlgItemText(IDC_DGN_RC_BEAM_DOUBLY_REIN_Rhob, _T("k*0.75*Rhob"));
	}
	else
	{
		SetDlgItemText(IDC_DGN_RC_BEAM_DOUBLY_REIN_Rhob, _T("k*Rhomax"));
	}

	if (strCode == _T("AIJ-WSD99") || CDgnCodeCtrl::IsConCodeUSD(strCode))
		GetDlgItem(IDC_DGN_CON_AIJBEAM_SPLICETYPE_FRAME)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_DGN_CON_AIJBEAM_SPLICETYPE_FRAME)->ShowWindow(SW_HIDE);

	// Temporary Coded by GAY. ('06.10.17). Not show [...] Button.
	GetDlgItem(IDC_DGN_RC_BEAM_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_COLUMN_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BRACE_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_WALL_BTN)->ShowWindow(SW_HIDE);

	if (Is_KCIUSD07_KS_KS01RC())
	{
		GetDlgItem(IDC_DGN_CON_MATL_RBAR_DIAM_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_MATL_RBAR_DIAM_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->ShowWindow(SW_HIDE);
	}

	m_chkMatlByRbar.SetCheck(m_Dcrb.bMatlByDia);
	GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->EnableWindow(m_Dcrb.bMatlByDia);
	OnMatlRbarDiamCheck();

	ControlMan();
}
void CDgnConBarDlg::Initial_ExistData()
{
	m_pDoc->m_pAttrCtrl->GetDcrb(m_Dcrb);

	int Index=0;
	CString strStirrupNum=_T(""); 
	/////////////////////////////////////////////////////////////////////////////////////
	// Beam.
	// Main Rebar.
	if(m_Dcrb.MainRebarBeam[0]==_T(""))	CDBLib::GetDefaultMainRebarSize(1, m_BMainBar);
	else														m_BMainBar = Adding_ReBarSize(m_Dcrb.MainRebarBeam);
	// Stirrup.
	if(m_Dcrb.StirrupBarBeam==_T(""))	CDBLib::GetDefaultSubRebarSize(1, m_Dcrb.StirrupBarBeam);
	Index = m_BStirrup.FindStringExact(-1, m_Dcrb.StirrupBarBeam);
	m_BStirrup.SetCurSel(Index);
	// Stirrup Arrangement.
	if(m_Dcrb.iSubRebarNum_Beam==0)	CDBLib::GetDefaultSubRebarNum(m_Dcrb.iSubRebarNum_Beam);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Beam);
	Index = m_BStirrupNum.FindStringExact(-1, strStirrupNum);
	m_BStirrupNum.SetCurSel(Index<0 ? 0 : Index);
	// Side Rebar.
	if(m_Dcrb.SideBarBeam==_T(""))	CDBLib::GetDefaultSideRebarSize(m_Dcrb.SideBarBeam);
	Index = m_BSide.FindStringExact(-1, m_Dcrb.SideBarBeam);
	m_BSide.SetCurSel(Index);
	// Covering.
	m_Db  = m_Dcrb.dB_Beam;
	m_Dt  = m_Dcrb.dT_Beam;
	/////////////////////////////////////////////////////////////////////////////////////
	// Column.
	// Main Rebar.
	if(m_Dcrb.MainRebarColumn[0]==_T(""))	CDBLib::GetDefaultMainRebarSize(2, m_CMainBar);
	else															m_CMainBar = Adding_ReBarSize(m_Dcrb.MainRebarColumn);
	// Sub Rebar.
	if(m_Dcrb.StirrupBarColumn==_T(""))  CDBLib::GetDefaultSubRebarSize(2, m_Dcrb.StirrupBarColumn);
	Index = m_CStirrup.FindStringExact(-1, m_Dcrb.StirrupBarColumn);
	m_CStirrup.SetCurSel(Index);
	// Sub Rebar Arrangement.
	// Y.
	if(m_Dcrb.iSubRebarNum_Column[0]==0)	CDBLib::GetDefaultSubRebarNum(m_Dcrb.iSubRebarNum_Column[0]);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Column[0]);
	Index = m_CStirrupYNum.FindStringExact(-1, strStirrupNum);
	m_CStirrupYNum.SetCurSel(Index);
	// Z.
	if(m_Dcrb.iSubRebarNum_Column[1]==0)	CDBLib::GetDefaultSubRebarNum(m_Dcrb.iSubRebarNum_Column[1]);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Column[1]);
	Index = m_CStirrupZNum.FindStringExact(-1, strStirrupNum);
	m_CStirrupZNum.SetCurSel(Index);
	// Covering.
	m_cDo = m_Dcrb.d0_Column;
	/////////////////////////////////////////////////////////////////////////////////////
	// Brace.
	// Main Rebar.
	if(m_Dcrb.MainRebarBrace[0]==_T(""))		CDBLib::GetDefaultMainRebarSize(3, m_BrMainBar);
	else															  m_BrMainBar = Adding_ReBarSize(m_Dcrb.MainRebarBrace);
	// Sub Rebar.
	if(m_Dcrb.StirrupBarBrace==_T(""))	CDBLib::GetDefaultSubRebarSize(3, m_Dcrb.StirrupBarBrace);
	Index = m_BrStirrup.FindStringExact(-1, m_Dcrb.StirrupBarBrace);
	m_BrStirrup.SetCurSel(Index);
	// Sub Rebar Arrangement.
	// Y.
	if(m_Dcrb.iSubRebarNum_Brace[0]==0)	CDBLib::GetDefaultSubRebarNum(m_Dcrb.iSubRebarNum_Brace[0]);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Brace[0]);
	Index = m_BrStirrupYNum.FindStringExact(-1, strStirrupNum);
	m_BrStirrupYNum.SetCurSel(Index);
	// Z.
	if(m_Dcrb.iSubRebarNum_Brace[1]==0)	CDBLib::GetDefaultSubRebarNum(m_Dcrb.iSubRebarNum_Brace[1]);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Brace[1]);
	Index = m_BrStirrupZNum.FindStringExact(-1, strStirrupNum);
	m_BrStirrupZNum.SetCurSel(Index);
	// Covering.
	m_brDo = m_Dcrb.d0_Brace;
	/////////////////////////////////////////////////////////////////////////////////////

	CString strWallMainRebarDefault = _T("");
	CDBLib::GetDefaultMainRebarSize(4, strWallMainRebarDefault);

	CString strWallRebarDefault = _T("");
	CDBLib::GetDefaultSubRebarSize(4, strWallRebarDefault);

	// Wall.
	// Vertical Rebar.
	if(m_Dcrb.VerticalRebarWall[0]==_T(""))	m_WVBar = strWallMainRebarDefault;
	else																m_WVBar = Adding_ReBarSize(m_Dcrb.VerticalRebarWall);
	// Horizontal Rebar.
	if(m_Dcrb.HorizontalRebarWall==_T(""))	m_Dcrb.HorizontalRebarWall = strWallRebarDefault;
	Index = m_WHBar.FindStringExact(-1,m_Dcrb.HorizontalRebarWall);
	m_WHBar.SetCurSel(Index);
	// End Rebar.
	if(m_Dcrb.EndRebarWall==_T(""))	m_Dcrb.EndRebarWall = strWallRebarDefault;
	Index = m_WEndBar.FindStringExact(-1, m_Dcrb.EndRebarWall);
	m_WEndBar.SetCurSel(Index);
	// Boundary Element Horizontal Rebar.
	if(m_Dcrb.BEHorizontalRebarWall==_T(""))	m_Dcrb.BEHorizontalRebarWall = strWallRebarDefault;
	Index = m_BEWHBar.FindStringExact(-1,m_Dcrb.BEHorizontalRebarWall);
	m_BEWHBar.SetCurSel(Index);
	m_BEHSpace = m_Dcrb.dBEHorizontalRebarSpace;
	m_BEVSpace = m_Dcrb.dBEVerticalRebarSpace;
	// Covering.
	m_De = m_Dcrb.dDe_Wall;
	m_Dw = m_Dcrb.dDw_Wall;
	// AIJ Recommendation Check, add by cylee 05.06.24
	m_bAIJBeamRecommend = m_Dcrb.bSpliceBeamJP;			
	m_bAIJColRecommend = m_Dcrb.bSpliceColumnJP;		
	m_bAIJBraRecommend = m_Dcrb.bSpliceBraceJP;		
	m_iSpliceSubType = m_Dcrb.iSpliceBeamSubType;
	// Beam, Column, Brace Splice, add by cylee 05.06.24
	m_bsplice_radio = m_Dcrb.iSpliceBeam;				
	m_csplice_radio = m_Dcrb.iSpliceColumn;			
	m_rsplice_radio = m_Dcrb.iSpliceBrace;
	// Add by sshan. MNET:2619.('20070129)
	m_dDoublyRain = (m_Dcrb.dDoublyBeam < 0.5 || m_Dcrb.dDoublyBeam > 1.0 ? 1.0 : m_Dcrb.dDoublyBeam);
	// Add by ZINU.('08.04.24). Set m_bDoublyRain by m_Dcrb.
	m_bDoublyRain = m_Dcrb.bDoublyBeam;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBeam   = m_Dcrb.bCheckRebarSpacing_Beam;
	m_bConsiderSpacingLimitColumn = m_Dcrb.bCheckRebarSpacing_Column;
	m_bConsiderSpacingLimitBrace  = m_Dcrb.bCheckRebarSpacing_Brace;

	//m_bVerEndWall = m_Dcrb.bVerEndBar_Wall;
	if(m_Dcrb.strVerEndBar_Wall==_T(""))	m_Dcrb.strVerEndBar_Wall = strWallMainRebarDefault;
	//Index = m_ComboVerEndWall.FindStringExact(-1, m_Dcrb.strVerEndBar_Wall); m_ComboVerEndWall.SetCurSel(Index);
	CString strMatlDB = MATLCODE_CON_KS;
	if(m_Dcrb.strVerEndBarFy_Wall==_T(""))	m_Dcrb.strVerEndBarFy_Wall = CDBLib::GetDefaultRbarMatlDB(strMatlDB);
	//Index = m_ComboVerEndFyWall.FindStringExact(-1, m_Dcrb.strVerEndBarFy_Wall); m_ComboVerEndFyWall.SetCurSel(Index);

	UpdateData(FALSE);
}

void CDgnConBarDlg::Initial_EditStaus(CDBDoc* pDoc)
{
	if(m_BStirrup.GetCount() > 0)			    m_BStirrup.ResetContent();
	if(m_BSide.GetCount() > 0)				    m_BSide.ResetContent();
	if(m_CStirrup.GetCount() > 0)			    m_CStirrup.ResetContent();
	if(m_BrStirrup.GetCount() > 0)		    m_BrStirrup.ResetContent();
	if(m_WHBar.GetCount() > 0)				    m_WHBar.ResetContent();
	if(m_WEndBar.GetCount() > 0)			    m_WEndBar.ResetContent();
	if(m_BEWHBar.GetCount() > 0)		      m_BEWHBar.ResetContent();
	//if(m_ComboVerEndWall.GetCount() > 0)  m_ComboVerEndWall.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_BStirrup);
	CDBLib::GetRebarNameAtComboBox(&m_BSide);
	CDBLib::GetRebarNameAtComboBox(&m_CStirrup);
	CDBLib::GetRebarNameAtComboBox(&m_BrStirrup);
	CDBLib::GetRebarNameAtComboBox(&m_WHBar);
	CDBLib::GetRebarNameAtComboBox(&m_WEndBar);
	CDBLib::GetRebarNameAtComboBox(&m_BEWHBar);
	//CDBLib::GetRebarNameAtComboBox(&m_ComboVerEndWall);
	
	if(m_BStirrupNum.GetCount() > 0)	m_BStirrupNum.ResetContent();
	if(m_CStirrupYNum.GetCount() > 0)	m_CStirrupYNum.ResetContent();
	if(m_CStirrupZNum.GetCount() > 0)	m_CStirrupZNum.ResetContent();
	if(m_BrStirrupYNum.GetCount() > 0)	m_BrStirrupYNum.ResetContent();
	if(m_BrStirrupZNum.GetCount() > 0)	m_BrStirrupZNum.ResetContent();
	// Coded by SeungJun MNET:3466  '20080529
	CStringArray arStirrupNum;
	CDBLib::GetSubRebarNumList(arStirrupNum);
	int iCount = arStirrupNum.GetSize();
	for(int i=0; i<iCount; i++)
	{
		m_CStirrupYNum.AddString(arStirrupNum[i]);
		m_CStirrupZNum.AddString(arStirrupNum[i]);
		m_BrStirrupYNum.AddString(arStirrupNum[i]);
		m_BrStirrupZNum.AddString(arStirrupNum[i]);
	}
	CStringArray arBeamStirrupNum;
	CDBLib::GetSubRebarNumList(arBeamStirrupNum, TRUE);
	iCount = arBeamStirrupNum.GetSize();
	for(int i=0; i<iCount; i++)
	{
		m_BStirrupNum.AddString(arBeamStirrupNum[i]);
	}

// 	CArray<CString, CString&> arRebar;
// 	CString strRebarMatl = _T("KS19(RC)");
//   m_pDoc->m_pMatlDB->GetRebarNameList(strRebarMatl, arRebar);
//   for(i=0; i<arRebar.GetSize(); i++)  
//   {
//     m_ComboVerEndFyWall.AddString(arRebar[i]);    
//   }
}

void CDgnConBarDlg::Initial_UnitData()
{
	m_BEHSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BEVSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DeUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_cDoUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_brDoUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DtUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DbUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConBarDlg::Initial_DefaultData()
{
	// Change by ZINU.('05.08.22). m_pDoc->m_pAttrCtrl->GetDcrb() -> CDataCtrl::Get_DgnConDcrb().
	CDgnDataCtrl DataCtrl;
	m_Dcrb.Initialize();
	// Change by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Global -> Elem(0).
	DataCtrl.Get_DgnConDcrb(0, m_Dcrb);
	// Main Rbar Size.
	m_BMainBar	= m_Dcrb.MainRebarBeam[0];
	m_CMainBar	= m_Dcrb.MainRebarColumn[0];
	m_BrMainBar	= m_Dcrb.MainRebarBrace[0];
	m_WVBar			= m_Dcrb.VerticalRebarWall[0];

	CString strRebarDefault = _T("");
	int nBeamRebarDefaultIndex=0, nColmRebarDefaultIndex=0, nBraceRebarDefaultIndex=0, nWallRebarDefaultIndex=0;
	// MNET:4076-SHJUNG-20100226
	// 각 부재별로 Default 값을 얻어 오고, FindStringExact 할 때는 부재타입만 맞으면 되니까,
	// WallRebar 콤보를 예로 들자면, m_WHBar, m_WEndBar, m_BEWHBar 이렇게 3개가 있지만 m_WHBar를 대표콤보로 사용하였다
	CDBLib::GetDefaultSubRebarSize(1, strRebarDefault); nBeamRebarDefaultIndex   = m_BStirrup.FindStringExact(-1, strRebarDefault);
	CDBLib::GetDefaultSubRebarSize(2, strRebarDefault); nColmRebarDefaultIndex   = m_CStirrup.FindStringExact(-1, strRebarDefault);
	CDBLib::GetDefaultSubRebarSize(3, strRebarDefault); nBraceRebarDefaultIndex  = m_BrStirrup.FindStringExact(-1, strRebarDefault);
	CDBLib::GetDefaultSubRebarSize(4, strRebarDefault); nWallRebarDefaultIndex   = m_WHBar.FindStringExact(-1, strRebarDefault);

	// Sub Rbar Size.
	int Index=0;
	Index = m_BStirrup.FindStringExact(-1, m_Dcrb.StirrupBarBeam);		    Index<0? m_BStirrup.SetCurSel(nBeamRebarDefaultIndex) : m_BStirrup.SetCurSel(Index);
	Index = m_BSide.FindStringExact(-1, m_Dcrb.SideBarBeam);					    Index<0? m_BSide.SetCurSel(nBeamRebarDefaultIndex) : m_BSide.SetCurSel(Index);
	Index = m_CStirrup.FindStringExact(-1, m_Dcrb.StirrupBarColumn);	    Index<0? m_CStirrup.SetCurSel(nColmRebarDefaultIndex) : m_CStirrup.SetCurSel(Index);
	Index = m_BrStirrup.FindStringExact(-1, m_Dcrb.StirrupBarBrace);	    Index<0? m_BrStirrup.SetCurSel(nBraceRebarDefaultIndex) : m_BrStirrup.SetCurSel(Index);
	Index = m_WHBar.FindStringExact(-1, m_Dcrb.HorizontalRebarWall);	    Index<0? m_WHBar.SetCurSel(nWallRebarDefaultIndex) : m_WHBar.SetCurSel(Index);
	Index = m_WEndBar.FindStringExact(-1, m_Dcrb.EndRebarWall);			      Index<0? m_WEndBar.SetCurSel(nWallRebarDefaultIndex) : m_WEndBar.SetCurSel(Index);
	Index = m_BEWHBar.FindStringExact(-1, m_Dcrb.BEHorizontalRebarWall);	Index<0? m_BEWHBar.SetCurSel(nWallRebarDefaultIndex) : m_BEWHBar.SetCurSel(Index);
	// Sub Rbar Number.
	CString strStirrupNum=_T("");
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Beam);				Index = m_BStirrupNum.FindStringExact(-1, strStirrupNum);		m_BStirrupNum.SetCurSel(Index);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Column[0]);	Index = m_CStirrupYNum.FindStringExact(-1, strStirrupNum);		m_CStirrupYNum.SetCurSel(Index);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Column[1]);	Index = m_CStirrupZNum.FindStringExact(-1, strStirrupNum);		m_CStirrupZNum.SetCurSel(Index);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Brace[0]);		Index = m_BrStirrupYNum.FindStringExact(-1, strStirrupNum);	m_BrStirrupYNum.SetCurSel(Index);
	strStirrupNum.Format(_T("%d"), m_Dcrb.iSubRebarNum_Brace[1]);		Index = m_BrStirrupZNum.FindStringExact(-1, strStirrupNum);	m_BrStirrupZNum.SetCurSel(Index);

	//Boundary Element Horizontal Rebar Space
	double dSpace = 0.2;
	T_UNIT_INDEX UnitIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
	}
	else ASSERT(0);

	m_BEHSpace = dSpace;

	dSpace = 0.1;
	if (UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
	}
	else if (UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
	}
	else ASSERT(0);

	m_BEVSpace = dSpace;

	// Covering.
	m_Db  = 0.0;
	m_Dt  = 0.0;
	m_brDo= 0.0;
	m_cDo = 0.0;
	m_De  = 0.0;
	m_Dw  = 0.0;
	// AIJ Recommendation Check, add by cylee 05.06.24
	m_bAIJBeamRecommend	= m_Dcrb.bSpliceBeamJP;
	m_bAIJColRecommend	= m_Dcrb.bSpliceColumnJP;
	m_bAIJBraRecommend	= m_Dcrb.bSpliceBraceJP;
	m_iSpliceSubType		= m_Dcrb.iSpliceBeamSubType;
	// Beam, Column, Brace Splice, add by cylee 05.06.24
	m_bsplice_radio = m_Dcrb.iSpliceBeam;
	m_csplice_radio = m_Dcrb.iSpliceColumn;
	m_rsplice_radio = m_Dcrb.iSpliceBrace;

	// Coded by Seungjun ('20071113) MNet:No.3087. Default 1.0으로.
	m_dDoublyRain = 1.0;
	m_bDoublyRain = TRUE;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBeam = TRUE;
	m_bConsiderSpacingLimitColumn = TRUE;
	m_bConsiderSpacingLimitBrace = TRUE;

	CString strWallMainRebarDefault = _T("");
	CDBLib::GetDefaultMainRebarSize(4, strWallMainRebarDefault);

	//m_bVerEndWall = m_Dcrb.bVerEndBar_Wall;
	if(m_Dcrb.strVerEndBar_Wall==_T(""))	m_Dcrb.strVerEndBar_Wall = strWallMainRebarDefault;
	//Index = m_ComboVerEndWall.FindStringExact(-1, m_Dcrb.strVerEndBar_Wall); m_ComboVerEndWall.SetCurSel(Index);
	CString strMatlDB = MATLCODE_CON_KS19;
	if(m_Dcrb.strVerEndBarFy_Wall==_T(""))	m_Dcrb.strVerEndBarFy_Wall = CDBLib::GetDefaultRbarMatlDB(strMatlDB);
	//Index = m_ComboVerEndFyWall.FindStringExact(-1, m_Dcrb.strVerEndBarFy_Wall); m_ComboVerEndFyWall.SetCurSel(Index);

	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	m_Dcrb.bMatlByDia = FALSE;
	m_Dcrb.strMatlCode = rPref.DgnCode.ConcRebarMatlCode;
	m_Dcrb.aVertMatlByDia.RemoveAll();
	m_Dcrb.aHorzMatlByDia.RemoveAll();

	if(m_Dcrb.strMatlCode == MATLCODE_CON_KS19)
	{
		T_MATLRBDIA_D bufData;
		T_MATL_REBAR bufRbar;
		BOOL bResult = FALSE;
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D10");
		bufData.strMatl = _T("SD400");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D13");
		bufData.strMatl = _T("SD400");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D16");
		bufData.strMatl = _T("SD500");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D19");
		bufData.strMatl = _T("SD500");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D22");
		bufData.strMatl = _T("SD600");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D25");
		bufData.strMatl = _T("SD600");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D10");
		bufData.strMatl = _T("SD400");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aHorzMatlByDia.Add(bufData);

		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D13");
		bufData.strMatl = _T("SD500");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aHorzMatlByDia.Add(bufData);
	}
    else if ( m_Dcrb.strMatlCode == MATLCODE_CON_KS )
	{
		T_MATLRBDIA_D bufData;
		T_MATL_REBAR bufRbar;
		BOOL bResult = FALSE;
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D10");
		bufData.strMatl = _T("SD40");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D13");
		bufData.strMatl = _T("SD40");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D16");
		bufData.strMatl = _T("SD50");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D19");
		bufData.strMatl = _T("SD50");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D22");
		bufData.strMatl = _T("SD50");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D25");
		bufData.strMatl = _T("SD50");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aVertMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D10");
		bufData.strMatl = _T("SD40");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aHorzMatlByDia.Add(bufData);
		
		bufData.Initialize();
		bufRbar.Initialize();
		bufData.strDia = _T("D13");
		bufData.strMatl = _T("SD50");
		bResult = m_pDoc->m_pMatlDB->GetRebarData(m_Dcrb.strMatlCode, bufData.strMatl, bufRbar);
		ASSERT(bResult);
		bufData.dFy = bufRbar.B_fy;
		if(bResult) m_Dcrb.aHorzMatlByDia.Add(bufData);
	}

	UpdateData(FALSE);	
}

CString CDgnConBarDlg::Adding_ReBarSize(CString* strSize)
{
	CString strTotal=_T(""),str=_T("");
	for(int i=0; i<5; i++)
	{
		CString temp = strSize[i];
		if(temp != _T(""))  str = str+temp+_T(",");
	}
	int nLen = str.GetLength();
	if(nLen > 0)	strTotal = str.Left(nLen-1);

	return strTotal;
}

void CDgnConBarDlg::Cutting_ReBarSize(CString strSize, CArray<CString,CString>& RebarSize)
{
	if(RebarSize.GetSize() != 0)  RebarSize.RemoveAll();

	CString temp=_T("");
	if(strSize != _T(""))
	{
		int nLen = strSize.GetLength();
		for(int i=0; i<nLen; i++)
		{
		  TCHAR ch = strSize.GetAt(i);
		  if(ch==',' || i+1 == nLen)
		  {
			  if(ch==',') RebarSize.Add(temp);
			  else			  
				{
					temp = temp+ch;
					RebarSize.Add(temp);
				}
			  temp=_T("");
		  }
		  else	temp = temp+ch;
		}
	}
}

void CDgnConBarDlg::OnDgnAddWalldata() 
{
	CDgnConWallDataDlg dlg;
	dlg.SetDcrb(m_Dcrb);
	if(dlg.DoModal())
	{
		m_Dcrb = dlg.GetDcrb();
	}
}

// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
void CDgnConBarDlg::Enable_EndBarEdit()
{
	// Change by ZINU.('01.9.14). Although WSD, Let's usd All Methods.
/*/
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode==_T("AIK-WSD2K") || strCode==_T("AIJ-WSD99"))	// WSD.
	{
		GetDlgItem(IDC_DGN_RC_WENDREBAR)->EnableWindow(FALSE);
	}
	else	// USD, LSD.
	{
		GetDlgItem(IDC_DGN_RC_WENDREBAR)->EnableWindow(TRUE);
	}
/*/
}

void CDgnConBarDlg::OnDgnAIJWidCheck()
{
	UpdateData(TRUE);
	if(m_bAIJBeamRecommend == FALSE) 
	{
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD1_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD2_RADIO)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CON_BSPLICE_NO_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_SEMI_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_FULL_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_TITLE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD1_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_AIJ_METHOD2_RADIO)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_DGN_CON_BSPLICE_NO_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_SEMI_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_FULL_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BSPLICE_TITLE)->EnableWindow(FALSE);
		m_bsplice_radio = 0;
	}

	if(m_bAIJColRecommend == FALSE) 
	{
		GetDlgItem(IDC_DGN_CON_CSPLICE_NO_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_SEMI_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_FULL_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_TITLE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_CSPLICE_NO_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_SEMI_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_FULL_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_CSPLICE_TITLE)->EnableWindow(FALSE);
		m_csplice_radio = 0;
	}
		
	if(m_bAIJBraRecommend == FALSE) 
	{
		GetDlgItem(IDC_DGN_CON_RSPLICE_NO_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_SEMI_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_FULL_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_TITLE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_RSPLICE_NO_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_SEMI_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_FULL_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_RSPLICE_TITLE)->EnableWindow(FALSE);
		m_rsplice_radio = 0;
	}
}

void CDgnConBarDlg::OnDgnBeamDoublyReinCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_EDIT)->EnableWindow(m_bDoublyRain);
	GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_STATIC)->EnableWindow(m_bDoublyRain);
	GetDlgItem(IDC_DGN_RC_BEAM_DOUBLY_REIN_Rhob)->EnableWindow(m_bDoublyRain);
}

void CDgnConBarDlg::OnDgnConsiderSpacingLimitCheck()
{
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	UpdateData(TRUE);
	// Beam.
	GetDlgItem(IDC_DGN_CON_BSPLICE_NO_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BSPLICE_SEMI_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BSPLICE_FULL_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BSPLICE_TITLE)->EnableWindow(m_bConsiderSpacingLimitBeam);
	// Column.
	GetDlgItem(IDC_DGN_CON_CSPLICE_NO_RADIO)->EnableWindow(m_bConsiderSpacingLimitColumn);
	GetDlgItem(IDC_DGN_CON_CSPLICE_SEMI_RADIO)->EnableWindow(m_bConsiderSpacingLimitColumn);
	GetDlgItem(IDC_DGN_CON_CSPLICE_FULL_RADIO)->EnableWindow(m_bConsiderSpacingLimitColumn);
	GetDlgItem(IDC_DGN_CON_CSPLICE_TITLE)->EnableWindow(m_bConsiderSpacingLimitColumn);
	// Brace.
	GetDlgItem(IDC_DGN_CON_RSPLICE_NO_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_RSPLICE_SEMI_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_RSPLICE_FULL_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_RSPLICE_TITLE)->EnableWindow(m_bConsiderSpacingLimitBrace);
}

// void CDgnConBarDlg::OnDgnRcVerEndWall()
// {
//   UpdateData(TRUE);
//   //GetDlgItem(IDC_DGN_CON_MATL_RBAR_DIAM_CHK)->EnableWindow(!m_bVerEndWall);
// 
//   if(m_bVerEndWall)
//   {
//     m_chkMatlByRbar.SetCheck(FALSE);
//     GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->EnableWindow(m_chkMatlByRbar.GetCheck());
//   }  
// 
//   //GetDlgItem(IDC_DGN_CON_VER_END_STA1)->EnableWindow(m_bVerEndWall);
//   GetDlgItem(IDC_DGN_RC_VER_END_WALL)->EnableWindow(m_bVerEndWall);
//   GetDlgItem(IDC_DGN_CON_VER_END_STA2)->EnableWindow(m_bVerEndWall);
//   GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->EnableWindow(m_bVerEndWall);  
//   
// }

void CDgnConBarDlg::OnMatlRbarDiamCheck()
{
	BOOL bChkMatlByRbar = m_chkMatlByRbar.GetCheck();
	GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->EnableWindow(m_chkMatlByRbar.GetCheck());
}

void CDgnConBarDlg::OnMatlRbarDiamBtn()
{
	CMatlByRebarDiaDlg dlg(&m_Dcrb);
	dlg.DoModal();
}

BOOL CDgnConBarDlg::Is_KCIUSD07_KS_KS01RC(BOOL bOnlyDgnCode/*FALSE*/) 
{
    bool bKCI_USD07 = false;	
	BOOL bKS01_RC = FALSE;

	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

    auto L_IsCodeKCI07Series =[] (const CString& strCode) -> bool
    {
        if ( strCode == CONCODE_KCI_USD07 ) return true;
        if ( strCode == CONCODE_KCI_USD12 ) return true;
        if ( strCode == CONCODE_KDS_41_30_2018 ) return true;
        if ( strCode == CONCODE_KDS_41_20_2022 ) return true;
        
        return false;
    };

	if(m_pDoc->m_pAttrCtrl->ExistDcon())
	{
		T_DCON_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDcon(rData);
        
        bKCI_USD07 = L_IsCodeKCI07Series(rData.DesignCode);
	}
	else
	{		
		rPref.DgnCode.ConcCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.ConcCode);

        bKCI_USD07 = L_IsCodeKCI07Series(rPref.DgnCode.ConcCode);        
	}

	if(bOnlyDgnCode)
	{
		if(bKCI_USD07)  return TRUE;
		else            return FALSE;
	}

    const bool bKS = [] (const CString& strMatlCode) -> bool
    {
        if ( strMatlCode == MATLCODE_CON_KS_CIVIL )  return true;
        if ( strMatlCode == MATLCODE_CON_KS01_CIVIL ) return true;
        if ( strMatlCode == MATLCODE_CON_KS19 ) return true;
        if ( strMatlCode == MATLCODE_CON_KS01 ) return true;
        if ( strMatlCode == MATLCODE_CON_KS ) return true;
        return false;
    }(rPref.DgnCode.ConcRebarMatlCode);

	if(bKCI_USD07 && bKS)
		return TRUE;
	else
		return FALSE;	
}

void CDgnConBarDlg::ControlMan()
{
	if(!Is_KCIUSD07_KS_KS01RC())
	{
		CRect RectDwWall, RectDlg, RectGroup, RectWallDataBtn, RectOK, RectClose;
		GetWindowRect(&RectDlg);
		GetDlgItem(IDC_DGN_RC_WALLDW)->GetWindowRect(&RectDwWall);
		

		GetDlgItem(IDC_DGN_WALL_GROUP_STATIC)->GetWindowRect(&RectGroup);
		GetDlgItem(IDC_DGN_ADD_WALLDATA)->GetWindowRect(&RectWallDataBtn);
		GetDlgItem(IDOK)->GetWindowRect(&RectOK);
		GetDlgItem(IDCANCEL)->GetWindowRect(&RectClose);
		
		CRect rWallDataBtn;
		rWallDataBtn.left			= RectWallDataBtn.left;
		rWallDataBtn.right		= RectWallDataBtn.right;
		rWallDataBtn.top			= RectDwWall.bottom + globalUtils.ScaleByDPI(5);
		rWallDataBtn.bottom		= rWallDataBtn.top + RectWallDataBtn.Height();
		
		CRect rGroup;
		rGroup.left			= RectGroup.left;
		rGroup.right		= RectGroup.right;
		rGroup.top			= RectGroup.top;
		rGroup.bottom		= rWallDataBtn.bottom + globalUtils.ScaleByDPI(5);
		
		CRect rOK;
		rOK.left			= RectOK.left;
		rOK.right			= RectOK.right;
		rOK.top				= rGroup.bottom + globalUtils.ScaleByDPI(15);
		rOK.bottom		= rOK.top + RectOK.Height();
		
		CRect rClose;
		rClose.left		= RectClose.left;
		rClose.right	= RectClose.right;
		rClose.top		= rOK.top;
		rClose.bottom	= rOK.bottom;
		
		CRect rDlg;
		rDlg.left		= RectDlg.left;
		rDlg.top		= RectDlg.top;
		rDlg.right	=	RectDlg.right;
		rDlg.bottom	= rClose.bottom + globalUtils.ScaleByDPI(10);
		
		ScreenToClient(rWallDataBtn);
		ScreenToClient(rGroup);
		ScreenToClient(rOK);
		ScreenToClient(rClose);
		
		MoveWindow(rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height());
		GetDlgItem(IDC_DGN_ADD_WALLDATA)->MoveWindow(rWallDataBtn.left, rWallDataBtn.top, rWallDataBtn.Width(), rWallDataBtn.Height());
		GetDlgItem(IDC_DGN_WALL_GROUP_STATIC)->MoveWindow(rGroup.left, rGroup.top, rGroup.Width(), rGroup.Height());
		GetDlgItem(IDOK)->MoveWindow(rOK.left, rOK.top, rOK.Width(), rOK.Height());
		GetDlgItem(IDCANCEL)->MoveWindow(rClose.left, rClose.top, rClose.Width(), rClose.Height());
		//ShowWindow(SW_SHOW);  
	}	
}