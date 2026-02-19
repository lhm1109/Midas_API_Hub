// DgnConMeshedSlabWallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConMeshedSlabWallDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_gr\GR_ServiceDef.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "DgnSlabRebarPictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int CDgnConMeshedSlabWallDlg::WM_AFTERINITDIALOG = RegisterWindowMessage(_T("WM_AFTERINITDIALOG"));
/////////////////////////////////////////////////////////////////////////////
// CDgnConMeshedSlabWallDlg dialog
CDgnConMeshedSlabWallDlg::CDgnConMeshedSlabWallDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnConMeshedSlabWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMeshedSlabWallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aMembTypeRdo.Add(IDC_DGN_SLAB_RDO);
	m_aMembTypeRdo.Add(IDC_DGN_WALL_RDO);
	m_aMembTypeRdo.Add(IDC_DGN_STRIP_RDO);
	m_aMembTypeRdo.Add(IDC_DGN_SHELL_RDO);

	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_GRP);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_REBR_RDO);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_REBR_CMB);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_REBR_STC2);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_REBR_CMB2);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_STUD_RDO);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_STUD_CMB);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_STUD_STC2);
	m_aSlabPunchCtrls.Add(IDC_DGN_SM_PUNCH_STUD_CMB2);

	m_aSlabCtrls.Add(IDC_DGN_SLAB_STC);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_LAYER_STC);  
	m_aSlabCtrls.Add(IDC_DGN_SLAB_LAYER_T1_RDO);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_LAYER_B1_RDO);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_LAYER_T2_RDO);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_LAYER_B2_RDO);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_BASIC_STC);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD1_STC);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD2_STC);
	//m_aSlabCtrls.Add(IDC_DGN_SLAB_BASIC_NAME_CMB);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD1_NAME_CMB);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD2_NAME_CMB);
	//m_aSlabCtrls.Add(IDC_DGN_SLAB_BASIC_SPACE_CMB);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD1_SPACE_CMB);
	//m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD2_SPACE_CMB);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_BASIC_AT_STC);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD1_AT_STC);
	//m_aSlabCtrls.Add(IDC_DGN_SLAB_ADD2_AT_STC);
	m_aSlabCtrls.Add(IDC_DGN_SLAB_COVER_STC);  
	m_aSlabCtrls.Add(IDC_DGN_SLAB_COVER_EDT);    
	m_aSlabCtrls.Add(IDC_DGN_COVER_UNT);
	m_aSlabCtrls.Append(m_aSlabPunchCtrls);

	m_aWallCtrls.Add(IDC_DGN_WALL_STC);
	m_aWallCtrls.Add(IDC_DGN_VERTICAL_STC);
	m_aWallCtrls.Add(IDC_DGN_VER_NAME_CMB);
	m_aWallCtrls.Add(IDC_DGN_VER_AT_STC);
	m_aWallCtrls.Add(IDC_DGN_VER_SPACE_CMB);
	m_aWallCtrls.Add(IDC_DGN_VERTICAL_STC2);
	m_aWallCtrls.Add(IDC_DGN_HORIZONTAL_STC);
	m_aWallCtrls.Add(IDC_DGN_HOR_NAME_CMB);
	m_aWallCtrls.Add(IDC_DGN_HOR_AT_STC);
	m_aWallCtrls.Add(IDC_DGN_HOR_SPACE_CMB);
	
	m_aStripCtrls.Add(IDC_DGN_STRIP_GRP);
	m_aStripCtrls.Add(IDC_DGN_STRIP_LAYER_GRP);
	m_aStripCtrls.Add(IDC_DGN_STRIP_LAYER_TOP_RDO);
	m_aStripCtrls.Add(IDC_DGN_STRIP_LAYER_BOT_RDO);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_I_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_I_CMB);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_I_AT_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_I_CMB2);
	m_aStripCtrls.Add(IDC_DGN_STRIP_CENTER_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_CENTER_CMB);
	m_aStripCtrls.Add(IDC_DGN_STRIP_CENTER_AT_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_CENTER_CMB2);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_J_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_J_CMB);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_J_AT_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_END_J_CMB2);
	m_aStripCtrls.Add(IDC_DGN_STRIP_COVER_STC);
	m_aStripCtrls.Add(IDC_DGN_STRIP_COVER_UNT);
	m_aStripCtrls.Add(IDC_DGN_STRIP_COVER_EDT);
	m_aStripCtrls.Add(IDC_DGN_STRIP_SAME_REBAR_CHK);

	m_aShellCtrls.Add(IDC_DGN_SHELL_STC	 );
	m_aShellCtrls.Add(IDC_DGN_SHELL_LAYER_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_LAYER_T1_RDO);
	m_aShellCtrls.Add(IDC_DGN_SHELL_LAYER_B1_RDO);
	m_aShellCtrls.Add(IDC_DGN_SHELL_LAYER_T2_RDO);
	m_aShellCtrls.Add(IDC_DGN_SHELL_LAYER_B2_RDO);
	m_aShellCtrls.Add(IDC_DGN_SHELL_BASIC_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_BASIC_NAME_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_BASIC_AT_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_BASIC_SPACE_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD1_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD1_NAME_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD1_AT_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD1_SPACE_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD2_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD2_NAME_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD2_AT_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_ADD2_SPACE_CMB);
	m_aShellCtrls.Add(IDC_DGN_SHELL_COVER_STC);
	m_aShellCtrls.Add(IDC_DGN_SHELL_COVER_EDT);
	m_aShellCtrls.Add(IDC_DGN_SHELL_COVER_UNT);
	m_aShellCtrls.Add(IDC_WG_DGN_LTHK_SAND_STC);
	m_aShellCtrls.Add(IDC_DGN_AUTO_CHK	 );
	m_aShellCtrls.Add(IDC_DGN_TOP_STC);
	m_aShellCtrls.Add(IDC_DGN_TOP_EDT);
	m_aShellCtrls.Add(IDC_DGN_TOP_STC3);  
	m_aShellCtrls.Add(IDC_DGN_BOT_STC);
	m_aShellCtrls.Add(IDC_DGN_BOT_EDT);
	m_aShellCtrls.Add(IDC_DGN_BOT_STC3);  
	m_aShellCtrls.Add(IDC_DGN_NUM_OF_ITR_STC);
	m_aShellCtrls.Add(IDC_DGN_NUM_OF_ITR_EDT);	
	m_aShellCtrls.Add(IDC_DGN_TOL_STC);
	m_aShellCtrls.Add(IDC_DGN_TOL_EDT	);

	m_aSlabLayerRdo.Add(IDC_DGN_SLAB_LAYER_T1_RDO);
	m_aSlabLayerRdo.Add(IDC_DGN_SLAB_LAYER_B1_RDO);
	m_aSlabLayerRdo.Add(IDC_DGN_SLAB_LAYER_T2_RDO);
	m_aSlabLayerRdo.Add(IDC_DGN_SLAB_LAYER_B2_RDO);    

	m_aStripLayerRdo.Add(IDC_DGN_STRIP_LAYER_TOP_RDO);
	m_aStripLayerRdo.Add(IDC_DGN_STRIP_LAYER_BOT_RDO);

	m_aShellLayerRdo.Add(IDC_DGN_SHELL_LAYER_T1_RDO);
	m_aShellLayerRdo.Add(IDC_DGN_SHELL_LAYER_B1_RDO);
	m_aShellLayerRdo.Add(IDC_DGN_SHELL_LAYER_T2_RDO);
	m_aShellLayerRdo.Add(IDC_DGN_SHELL_LAYER_B2_RDO);

	m_aPunchRdo.Add(IDC_DGN_SM_PUNCH_REBR_RDO);
	m_aPunchRdo.Add(IDC_DGN_SM_PUNCH_STUD_RDO);
	m_aPunchRebrCtrls.Add(IDC_DGN_SM_PUNCH_REBR_CMB);
	m_aPunchRebrCtrls.Add(IDC_DGN_SM_PUNCH_REBR_STC2);
	m_aPunchRebrCtrls.Add(IDC_DGN_SM_PUNCH_REBR_CMB2);
	m_aPunchStudCtrls.Add(IDC_DGN_SM_PUNCH_STUD_CMB);
	m_aPunchStudCtrls.Add(IDC_DGN_SM_PUNCH_STUD_STC2);
	m_aPunchStudCtrls.Add(IDC_DGN_SM_PUNCH_STUD_CMB2);

	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_CENTER_CMB);
	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_CENTER_AT_STC);
	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_CENTER_CMB2);
	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_END_J_CMB);
	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_END_J_AT_STC);
	m_aStripRebrCJCtrls.Add(IDC_DGN_STRIP_END_J_CMB2);

	m_bInit = FALSE;
	m_pDoc = NULL;

	m_pPictureDlg = NULL;
	m_bPrevUnitType = 0;
	m_bStripSameRebarSize = FALSE;

	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance(); ASSERT(pServiceHeadOffice);
	m_bOriginMstrDispOpt = (BOOL)pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_CUR_MSTR_OPT, NULL);

	m_bShowStrip = FALSE;
	
	m_RbslD.Initialize();
}

CDgnConMeshedSlabWallDlg::~CDgnConMeshedSlabWallDlg()
{
}

void CDgnConMeshedSlabWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMeshedSlabWallDlg)
	DDX_Control(pDX, IDC_DGN_MESHED_SLAB_WALL_LST, m_List);
	DDX_Control(pDX, IDC_DGN_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_DGN_ELEM_LIST_EDT, m_edtElem);

	DDX_Control(pDX, IDC_DGN_SLAB_BASIC_NAME_CMB , m_cmbBasicName );
	DDX_Control(pDX, IDC_DGN_SLAB_BASIC_SPACE_CMB, m_cmbBasicSpace);
	DDX_Control(pDX, IDC_DGN_SLAB_ADD1_NAME_CMB  , m_cmbAdd1Name  );
	DDX_Control(pDX, IDC_DGN_SLAB_ADD1_SPACE_CMB , m_cmbAdd1Space );
	DDX_Control(pDX, IDC_DGN_SLAB_ADD2_NAME_CMB  , m_cmbAdd2Name  );
	DDX_Control(pDX, IDC_DGN_SLAB_ADD2_SPACE_CMB , m_cmbAdd2Space );
	DDX_Control(pDX, IDC_DGN_SLAB_COVER_EDT      , m_edtSlabCover );
	DDX_Control(pDX, IDC_DGN_COVER_UNT           , m_untSlabCover );
	
	DDX_Control(pDX, IDC_DGN_VER_NAME_CMB, m_cmbVerName);
	DDX_Control(pDX, IDC_DGN_VER_SPACE_CMB, m_cmbVerSpace);
	DDX_Control(pDX, IDC_DGN_HOR_NAME_CMB, m_cmbHorName);
	DDX_Control(pDX, IDC_DGN_HOR_SPACE_CMB, m_cmbHorSpace);
	DDX_Control(pDX, IDC_DGN_DW_EDT, m_edtDw);
	DDX_Control(pDX, IDC_DGN_DW_UNT, m_untDw);

	DDX_Control(pDX, IDC_DGN_SM_PUNCH_REBR_CMB , m_cmbPunchRbarName );
	DDX_Control(pDX, IDC_DGN_SM_PUNCH_REBR_CMB2, m_cmbPunchRbarSpace);
	DDX_Control(pDX, IDC_DGN_SM_PUNCH_STUD_CMB , m_cmbPunchStudName );
	DDX_Control(pDX, IDC_DGN_SM_PUNCH_STUD_CMB2, m_cmbPunchStudSpace);

	DDX_Control(pDX, IDC_DGN_STRIP_END_I_CMB , m_cmbStripName_I );
	DDX_Control(pDX, IDC_DGN_STRIP_CENTER_CMB, m_cmbStripName_C );
	DDX_Control(pDX, IDC_DGN_STRIP_END_J_CMB , m_cmbStripName_J );
	DDX_Control(pDX, IDC_DGN_STRIP_END_I_CMB2 , m_cmbStripSpace_I);
	DDX_Control(pDX, IDC_DGN_STRIP_CENTER_CMB2, m_cmbStripSpace_C);
	DDX_Control(pDX, IDC_DGN_STRIP_END_J_CMB2 , m_cmbStripSpace_J);
	DDX_Control(pDX, IDC_DGN_STRIP_COVER_EDT , m_edtStripCover);
	DDX_Control(pDX, IDC_DGN_STRIP_COVER_UNT , m_untStripCover);
	DDX_Check(pDX, IDC_DGN_STRIP_SAME_REBAR_CHK, m_bStripSameRebarSize);

	DDX_Control(pDX, IDC_DGN_SHELL_BASIC_NAME_CMB , m_cmbShellBasicName );
	DDX_Control(pDX, IDC_DGN_SHELL_BASIC_SPACE_CMB, m_cmbShellBasicSpace);
	DDX_Control(pDX, IDC_DGN_SHELL_ADD1_NAME_CMB  , m_cmbShellAdd1Name  );
	DDX_Control(pDX, IDC_DGN_SHELL_ADD1_SPACE_CMB , m_cmbShellAdd1Space );
	DDX_Control(pDX, IDC_DGN_SHELL_ADD2_NAME_CMB  , m_cmbShellAdd2Name  );
	DDX_Control(pDX, IDC_DGN_SHELL_ADD2_SPACE_CMB , m_cmbShellAdd2Space );
	DDX_Control(pDX, IDC_DGN_SHELL_COVER_EDT      , m_edtShellCover     );
	DDX_Control(pDX, IDC_DGN_SHELL_COVER_UNT      , m_untShellCover     );

	DDX_Control(pDX, IDC_DGN_AUTO_CHK, m_chkAuto);
	DDX_Control(pDX, IDC_DGN_TOP_EDT, m_edtTop);
	DDX_Control(pDX, IDC_DGN_BOT_EDT, m_edtBot);	
	DDX_Control(pDX, IDC_DGN_NUM_OF_ITR_EDT, m_edtItr);	
	DDX_Control(pDX, IDC_DGN_TOL_EDT, m_edtTol);

	//}}AFX_DATA_MAP
}

BOOL CDgnConMeshedSlabWallDlg::DestroyWindow() 
{
	if(IsAlivePictureDlg())
	{
		DestroyPictureDlg();
	}
	RestoreOriginMstrDispOpt();
	return CDialog::DestroyWindow();
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CDgnConMeshedSlabWallDlg, CDgnDlgBase)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConMeshedSlabWallDlg)
//  ON_NOTIFY(NM_CLICK, IDC_DGN_MESHED_SLAB_WALL_LST, OnClkListItem)
	ON_BN_CLICKED(IDC_DGN_SLAB_RDO, OnMembTypeRdo)
	ON_BN_CLICKED(IDC_DGN_WALL_RDO, OnMembTypeRdo)
	ON_BN_CLICKED(IDC_DGN_STRIP_RDO, OnMembTypeRdo)
	ON_BN_CLICKED(IDC_DGN_SHELL_RDO, OnMembTypeRdo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_MESHED_SLAB_WALL_LST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_ADD_REP_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_REPLACE_BTN, OnDgnAddReplace)
	ON_BN_CLICKED(IDC_DGN_DELETE_BTN, OnDgnDelete)
	ON_BN_CLICKED(IDC_DGN_CLOSE_BTN, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_SLAB_LAYER_T1_RDO, OnSlabLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SLAB_LAYER_B1_RDO, OnSlabLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SLAB_LAYER_T2_RDO, OnSlabLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SLAB_LAYER_B2_RDO, OnSlabLayerRdo)
	ON_BN_CLICKED(IDC_DGN_STRIP_LAYER_TOP_RDO, OnStripLayerRdo)
	ON_BN_CLICKED(IDC_DGN_STRIP_LAYER_BOT_RDO, OnStripLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SM_PUNCH_REBR_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_DGN_SM_PUNCH_STUD_RDO, EnableDisableCtrls)
	ON_BN_CLICKED(IDC_DGN_STRIP_SAME_REBAR_CHK, OnClickStripSameSizeChk)
	ON_BN_CLICKED(IDC_DGN_SHELL_LAYER_T1_RDO, OnShellLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SHELL_LAYER_B1_RDO, OnShellLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SHELL_LAYER_T2_RDO, OnShellLayerRdo)
	ON_BN_CLICKED(IDC_DGN_SHELL_LAYER_B2_RDO, OnShellLayerRdo)
	ON_BN_CLICKED(IDC_DGN_AUTO_CHK, OnChkAuto)	
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDgnConMeshedSlabWallDlg::WM_AFTERINITDIALOG, OnAfterInitDialog)
END_MESSAGE_MAP()

BOOL CDgnConMeshedSlabWallDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDgnDlgBase::Create(nIDTemplate, pParentWnd);
}

void CDgnConMeshedSlabWallDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_UNIT:
		{
		CString strTemp;
		T_RBMS_D RbmsD;
		m_cmbBasicSpace.GetWindowText(strTemp);
		RbmsD.dBasicRebarSpace = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbAdd1Space.GetWindowText(strTemp);
		RbmsD.dAdd1RebarSpace = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbPunchRbarSpace.GetWindowText(strTemp);
		RbmsD.dPunchRebarSpace = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbPunchStudSpace.GetWindowText(strTemp);
		RbmsD.dStudRebarSpace= ConvertSpaceString2Data(strTemp, TRUE);
		//m_cmbAdd2Space.GetWindowText(strTemp);
		//RbmsD.dAdd2RebarSpace = ConvertSpaceString2Data(strTemp, TRUE);;    

		T_RBSL_D RbslD;
		m_cmbShellBasicSpace.GetWindowText(strTemp);
		RbslD.dBasicRebarSpace = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbShellAdd1Space.GetWindowText(strTemp);
		RbslD.dAdd1RebarSpace = ConvertSpaceString2Data(strTemp, TRUE);    

		InitRebarSpaceComboBox();

		m_cmbBasicSpace.SetWindowText(ConvertSpaceData2String(RbmsD.dBasicRebarSpace));
		m_cmbAdd1Space.SetWindowText(ConvertSpaceData2String(RbmsD.dAdd1RebarSpace));
		m_cmbPunchRbarSpace.SetWindowText(ConvertSpaceData2String(RbmsD.dPunchRebarSpace));
		m_cmbPunchStudSpace.SetWindowText(ConvertSpaceData2String(RbmsD.dStudRebarSpace));
		//m_cmbAdd2Space.SetWindowText(ConvertSpaceData2String(RbmsD.dAdd2RebarSpace));

		T_RBMW_D RbmwD;
		m_cmbVerSpace.GetWindowText(strTemp);
		RbmwD.dVerRebarSpace = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbHorSpace.GetWindowText(strTemp);
		RbmwD.dHorRebarSpace = ConvertSpaceString2Data(strTemp, TRUE);

		m_cmbVerSpace.SetWindowText(ConvertSpaceData2String(RbmwD.dVerRebarSpace));
		m_cmbHorSpace.SetWindowText(ConvertSpaceData2String(RbmwD.dHorRebarSpace));

		T_RBST_D RbstD;
		m_cmbStripSpace_I.GetWindowText(strTemp);
		RbstD.dRebarSpace_I = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbStripSpace_I.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_I));
		m_cmbStripSpace_C.GetWindowText(strTemp);
		RbstD.dRebarSpace_C = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbStripSpace_C.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_C));
		m_cmbStripSpace_J.GetWindowText(strTemp);
		RbstD.dRebarSpace_J = ConvertSpaceString2Data(strTemp, TRUE);
		m_cmbStripSpace_J.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_J));

		// Shell (RBSL)
		m_cmbShellBasicSpace.SetWindowText(ConvertSpaceData2String(RbslD.dBasicRebarSpace));
		m_cmbShellAdd1Space.SetWindowText(ConvertSpaceData2String(RbslD.dAdd1RebarSpace));    

		T_UNIT_INDEX UnitIndex;
		m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
		
		if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
			UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
		{
			m_bPrevUnitType = 0;
		}
		else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
		{
			m_bPrevUnitType = 1;
		}
		else   ASSERT(0);
		break;
		}
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
			break;
	default:
		break;
	}
}

BOOL CDgnConMeshedSlabWallDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDgnDlgBase::PreTranslateMessage(pMsg);
}

void CDgnConMeshedSlabWallDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bChange = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		case(UR_ELEM_MFS):
		case(UR_ELEM_MFD):
		case(UR_RBMW_ADD):
		case(UR_RBMW_DEL):
		case(UR_RBMW_MFS):
		case(UR_RBMW_MFD):
		case(UR_RBMS_ADD):
		case(UR_RBMS_DEL):
		case(UR_RBMS_MFS):
		case(UR_RBMS_MFD):
		case(UR_RBST_ADD):
		case(UR_RBST_DEL):
		case(UR_RBST_MFS):
		case(UR_RBST_MFD):
		case(UR_RBSL_ADD):
		case(UR_RBSL_DEL):
		case(UR_RBSL_MFS):
		case(UR_RBSL_MFD):  
			bChange = TRUE; break;
		default:            break;
		}
	}
	if(bChange)
	{
		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
	}
}

void CDgnConMeshedSlabWallDlg::ShowSelectedRbmw(T_RBMW_K RbmwK, BOOL bInit)
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	if(nMembType != 1 || bInit)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 1);
		OnMembTypeRdo();
	}

	T_RBMW_D RbmwD;
	if(!m_pDoc->m_pAttrCtrl->GetRbmw(RbmwK, RbmwD))     {  ASSERT(0);		return ;	}
	
	// Unselect current selection
	m_List.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);

	int nItemCount = m_List.GetItemCount();
	for(int i = 0; i < nItemCount; i++)
	{
		if(m_List.GetItemText(i, 0) == RbmwD.strName)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}   
}

void CDgnConMeshedSlabWallDlg::ShowSelectedRbms(T_RBMS_K RbmsK, BOOL bInit)
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	T_RBMS_D RbmsD;
	if(!m_pDoc->m_pAttrCtrl->GetRbms(RbmsK, RbmsD))     {  ASSERT(0);		return ;	}

	if(nMembType != 0 || bInit)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aSlabLayerRdo, RbmsD.nLayerType);
		CDlgUtil::CtrlRadioSetCheck(this, m_aShellLayerRdo, RbmsD.nLayerType);
		CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 0);    
		OnMembTypeRdo();
	}
	
	// Unselect current selection
	m_List.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);

	int nItemCount = m_List.GetItemCount();
	for(int i = 0; i < nItemCount; i++)
	{
		if(m_List.GetItemText(i, 0) == RbmsD.strName)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}  
}

void CDgnConMeshedSlabWallDlg::ShowSelectedRbsl(T_RBSL_K RbslK, BOOL bInit)
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	T_RBSL_D RbslD;
	if(!m_pDoc->m_pAttrCtrl->GetRbsl(RbslK, RbslD))     {  ASSERT(0);		return ;	}

	if(nMembType != 3 || bInit)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aSlabLayerRdo, RbslD.nLayerType);
		CDlgUtil::CtrlRadioSetCheck(this, m_aShellLayerRdo, RbslD.nLayerType);
		CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 3);    
		OnMembTypeRdo();
	}

	// Unselect current selection
	m_List.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);

	int nItemCount = m_List.GetItemCount();
	for(int i = 0; i < nItemCount; i++)
	{
		if(m_List.GetItemText(i, 0) == RbslD.strName)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}  
}

void CDgnConMeshedSlabWallDlg::ShowSelectedRbst(T_RBST_K RbstK, BOOL bInit)
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	T_RBST_D RbstD;
	if(!m_pDoc->m_pAttrCtrl2->GetRbst(RbstK, RbstD))     {  ASSERT(0);		return ;	}

	if(nMembType != 2 || bInit)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aStripLayerRdo, RbstD.nLayerType);
		CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 2);
		OnMembTypeRdo();
	}

	// Unselect current selection
	m_List.SetItemState(-1, 0,  LVIS_SELECTED | LVIS_FOCUSED);

	int nItemCount = m_List.GetItemCount();
	for(int i = 0; i < nItemCount; i++)
	{
		if(m_List.GetItemText(i, 0) == RbstD.strName)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}  
}

void CDgnConMeshedSlabWallDlg::SetWallTypeDlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 1);
	OnMembTypeRdo();
}
void CDgnConMeshedSlabWallDlg::SetStripTypeDlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 2);
	OnMembTypeRdo();
}

void CDgnConMeshedSlabWallDlg::SetShellTypeDlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 3);
	OnMembTypeRdo();
}

/////////////////////////////////////////////////////////////////////////////
// CDgnConMeshedSlabWallDlg message handlers

BOOL CDgnConMeshedSlabWallDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_M:   case D_UNITSYS_LENGTH_INDEX_MM:   case D_UNITSYS_LENGTH_INDEX_CM:
		m_bPrevUnitType = 0;
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:   case D_UNITSYS_LENGTH_INDEX_FT:
		m_bPrevUnitType = 1;
		break;
	}

	InitCtrls();
#ifdef _CIVIL
	SetShellTypeDlg();
#endif // _CIVIL
	EnableDisableCtrls();

	Data2Dlg();

	m_bInit = TRUE;
	
	this->PostMessage(WM_AFTERINITDIALOG);

	SetTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConMeshedSlabWallDlg::SetTitle()
{
#ifdef _CIVIL
	this->SetWindowText(_LS(IDS_DGN_SHELL_REBAR_FOR_CHECKING));
#else
	if(CDBLib::IsConCodeForShell())
	{
#if defined(_ORG) || defined(_JP) || defined(_CH)
		this->SetWindowText(_LS(IDS_DGN_SLAB_SHELL_REBAR_FOR_CHECKING));
#else
		this->SetWindowText(_LS(IDS_DGN_SLAB_WALL_SHELL_REBAR_FOR_CHECKING));
#endif
	}
	else
	{
#if defined(_ORG) || defined(_JP) || defined(_CH)
		this->SetWindowText(_LS(IDS_DGN_SLAB_REBAR_FOR_CHECKING));
#else
		this->SetWindowText(_LS(IDS_DGN_SLAB_WALL_REBAR_FOR_CHECKING));
#endif
	}
#endif // _CIVIL
}

void CDgnConMeshedSlabWallDlg::OnMembTypeRdo()
{
	EnableDisableCtrls();
	ShowControl();
	RestoreOriginMstrDispOpt();
	
	int nMembType = 0;   // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	if(nMembType == 0)
	{
		CreatePictureDlg();
		OnSlabLayerRdo();
		return;
	}
	else if(nMembType == 1)
	{
		MakeItemEx();
		DestroyPictureDlg();  

		CString strName;  
		strName.Format(_T("W%d"), m_pDoc->m_pAttrCtrl->GetStartNumRbmw());
		m_edtName.SetWindowText(strName);  
	}
	else if(nMembType == 2)
	{
		MakeItemEx();
		DestroyPictureDlg();  
		OnClickStripSameSizeChk();

		CString strName;  
		strName.Format(_T("ST%d"), m_pDoc->m_pAttrCtrl2->GetStartNumRbst());
		m_edtName.SetWindowText(strName); 
	}
	else if(nMembType == 3)
	{
		MakeItemEx();
		DestroyPictureDlg();  

		CString strName;  
		strName.Format(_T("SL%d"), m_pDoc->m_pAttrCtrl->GetStartNumRbsl());
		m_edtName.SetWindowText(strName);  
	}
	else ASSERT(0);
}

void CDgnConMeshedSlabWallDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)   OnClkListItem(pNMHDR, pResult);
	
	*pResult = 0;
}

void CDgnConMeshedSlabWallDlg::OnDgnAddReplace()
{
	if(!Dlg2Data())   return;

	CString strSelectName;
	GetSelectedItem(strSelectName);

	CStringArray aStrSelectName;
	int nSize = GetSelectedItems(aStrSelectName);

	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	
	//////////////////////////////////////////////////////////////////////////
	if(nMembType == 0)      // slab
	{
		if(nSize > 1)    // Replace
		{
			// [2012-05-23] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
			// Multy Replace를 지원안하도록 수정
			ASSERT(0); return;

			CArray<T_RBMS_K,T_RBMS_K> aRbmsK;      aRbmsK.SetSize(nSize);
			CArray<T_RBMS_D,T_RBMS_D&> aRbmsD;      aRbmsD.SetSize(nSize);

			for(int i = 0; i < nSize; i++)
			{
				aRbmsK[i] = m_pDoc->m_pAttrCtrl->GetRbmsKey(aStrSelectName[i]);
				aRbmsD[i] = m_RbmsD;
				T_RBMS_D RbmsD;
				if(!m_pDoc->m_pAttrCtrl->GetRbms(aRbmsK[i], RbmsD))      {  ASSERT(0);		return ;	}
				aRbmsD[i].strName = RbmsD.strName;
			}
			
			if(!m_pDoc->m_pDataCtrl->AddRbms(aRbmsK, aRbmsD))     { return ;	}
		}
		else      // Add or Replace
		{
			T_RBMS_K RbmsK = m_pDoc->m_pAttrCtrl->GetRbmsKey(strSelectName.IsEmpty() ? m_RbmsD.strName : strSelectName);      
			if(RbmsK != 0)
			{
				T_RBMS_D RbmsD;
				if(!m_pDoc->m_pAttrCtrl->GetRbms(RbmsK, RbmsD)) {ASSERT(0); return;}        
				if(RbmsD.nLayerType != m_RbmsD.nLayerType)
				{
					AfxMessageBox(_LS(IDS_DGN_SAME_NAME_EXIST));
					return;
				}
			}
			if(RbmsK == 0) RbmsK = m_pDoc->m_pAttrCtrl->GetStartNumRbms();      
			
			CArray<T_ELEM_K,T_ELEM_K> aElemK;
			m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
			int nElemSize = aElemK.GetSize();

			CArray<T_MREB_K,T_MREB_K> aMrebK;
			T_SBDO_D SbdoD;
			T_DOEL_D DoelD;
			for(int i = 0; i < nElemSize; i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetDoel(aElemK[i], DoelD))     continue;
				if(DoelD.nDomainType != 1)    continue;
				if(!m_pDoc->m_pAttrCtrl->GetSbdo(DoelD.KeyDomain, SbdoD))     {  ASSERT(0);		return ;	}
				if(SbdoD.nType != 1 && SbdoD.nType != 2)    continue;
				aMrebK.Add(aElemK[i]);
			}
			
			if(!m_pDoc->m_pDataCtrl->AddRbmsMreb(RbmsK, m_RbmsD, aMrebK))     {  return ;	}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if(nMembType==1)   // wall
	{
		if(nSize > 1)    // Replace
		{
			CArray<T_RBMW_K,T_RBMW_K> aRbmwK;      aRbmwK.SetSize(nSize);
			CArray<T_RBMW_D,T_RBMW_D&> aRbmwD;      aRbmwD.SetSize(nSize);
			
			for(int i = 0; i < nSize; i++)
			{
				aRbmwK[i] = m_pDoc->m_pAttrCtrl->GetRbmwKey(aStrSelectName[i]);
				aRbmwD[i] = m_RbmwD;
				T_RBMW_D RbmwD;
				if(!m_pDoc->m_pAttrCtrl->GetRbmw(aRbmwK[i], RbmwD))      {  ASSERT(0);		return ;	}
				aRbmwD[i].strName = RbmwD.strName;
			}
			
			if(!m_pDoc->m_pDataCtrl->AddRbmw(aRbmwK, aRbmwD))     {  return ;	}
		}
		else      // Add or Replace
		{
			T_RBMW_K RbmwK = m_pDoc->m_pAttrCtrl->GetRbmwKey(strSelectName.IsEmpty() ? m_RbmwD.strName : strSelectName);
			if(RbmwK == 0)    RbmwK = m_pDoc->m_pAttrCtrl->GetStartNumRbmw();
			
			CArray<T_ELEM_K,T_ELEM_K> aElemK;
			m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
			int nElemSize = aElemK.GetSize();

			CArray<T_MREB_K,T_MREB_K> aMrebK;
			T_SBDO_D SbdoD;
			T_DOEL_D DoelD;
			for(int i = 0; i < nElemSize; i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetDoel(aElemK[i], DoelD))     continue;
				if(DoelD.nDomainType != 1)    continue;
				if(!m_pDoc->m_pAttrCtrl->GetSbdo(DoelD.KeyDomain, SbdoD))     {  ASSERT(0);		return ;	}
				if(SbdoD.nType != 3)    continue;
				aMrebK.Add(aElemK[i]);
			}
//       int nMrebSize = aMrebK.GetSize();
//       if(nMrebSize < 1)
//       {
//         AfxMessageBox(_LS(IDS_DGN_NOT_SELECTED_ELEMENT_FOR_INPUT));
//         return;
//       }

			T_MREB_D MrebD;   MrebD.RbmwK = RbmwK;
			MrebD.nMembType = 5;
			
			if(!m_pDoc->m_pDataCtrl->AddRbmwMreb(RbmwK, m_RbmwD, aMrebK, MrebD))     {  return ;	}
		}
	}
	else if(nMembType==2)  // strip
	{
		if(nSize > 1)    // Replace
		{
			CArray<T_RBST_K,T_RBST_K> aRbstK;  aRbstK.SetSize(nSize);
			CArray<T_RBST_D,T_RBST_D&> aRbstD; aRbstD.SetSize(nSize);

			for(int i = 0; i < nSize; i++)
			{
				aRbstK[i] = m_pDoc->m_pAttrCtrl2->GetRbstKey(aStrSelectName[i]);
				aRbstD[i] = m_RbstD;
				T_RBST_D RbstD;
				if(!m_pDoc->m_pAttrCtrl2->GetRbst(aRbstK[i], RbstD))      {  ASSERT(0);		return ;	}
				aRbstD[i].strName = RbstD.strName;
			}

			if(!m_pDoc->m_pDataCtrl->AddRbst(aRbstK, aRbstD))     {  return ;	}
		}
		else      // Add or Replace
		{
			T_RBST_K RbstK = m_pDoc->m_pAttrCtrl2->GetRbstKey(strSelectName.IsEmpty() ? m_RbstD.strName : strSelectName);
			if(RbstK == 0)    RbstK = m_pDoc->m_pAttrCtrl2->GetStartNumRbst();

			CArray<T_ELEM_K,T_ELEM_K> aElemK;
			m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
			int nElemSize = aElemK.GetSize();

			CArray<T_MREB_K,T_MREB_K> aMrebK;
			T_ELEM_D ElemD;
			for(int i = 0; i < nElemSize; i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD)) { ASSERT(0); return; }
				if(ElemD.eltyp!=BEAM_EL || ElemD.nSubType!=1) continue; // 유효보인가?
				if(aElemK[i]!=m_pDoc->m_pAttrCtrl->GetFirstElemKInMemb(aElemK[i])) continue; // 멤버의 대표요소 또는 멤버가 아닌 요소만 할당

				aMrebK.Add(aElemK[i]);
			}

			if(!m_pDoc->m_pDataCtrl->AddRbstMreb(RbstK, m_RbstD, aMrebK)) { return; }
		}
	}
	else if(nMembType == 3)      // Shell
	{
		if(nSize > 1)    // Replace
		{
			// [2012-05-23] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
			// Multy Replace를 지원안하도록 수정
			ASSERT(0); return;

			CArray<T_RBSL_K,T_RBSL_K> aRbslK;      aRbslK.SetSize(nSize);
			CArray<T_RBSL_D,T_RBSL_D&> aRbslD;      aRbslD.SetSize(nSize);

			for(int i = 0; i < nSize; i++)
			{
				aRbslK[i] = m_pDoc->m_pAttrCtrl->GetRbslKey(aStrSelectName[i]);
				aRbslD[i] = m_RbslD;
				T_RBSL_D RbslD;
				if(!m_pDoc->m_pAttrCtrl->GetRbsl(aRbslK[i], RbslD))      {  ASSERT(0);		return ;	}
				aRbslD[i].strName = RbslD.strName;
			}

			if(!m_pDoc->m_pDataCtrl->AddRbsl(aRbslK, aRbslD))     {  return ;	}
		}
		else      // Add or Replace
		{
			T_RBSL_K RbslK = m_pDoc->m_pAttrCtrl->GetRbslKey(strSelectName.IsEmpty() ? m_RbslD.strName : strSelectName);      
			if(RbslK != 0)
			{
				T_RBSL_D RbslD;
				if(!m_pDoc->m_pAttrCtrl->GetRbsl(RbslK, RbslD)) {ASSERT(0); return;}        
				if(RbslD.nLayerType != m_RbslD.nLayerType)
				{
					AfxMessageBox(_LS(IDS_DGN_SAME_NAME_EXIST));
					return;
				}
			}
			if(RbslK == 0) RbslK = m_pDoc->m_pAttrCtrl->GetStartNumRbsl();      

			CArray<T_ELEM_K,T_ELEM_K> aElemK;
			m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
			int nElemSize = aElemK.GetSize();

			CArray<T_MREB_K,T_MREB_K> aMrebK;
			T_SBDO_D SbdoD;
			T_DOEL_D DoelD;
			for(int i = 0; i < nElemSize; i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetDoel(aElemK[i], DoelD))     continue;
				if(DoelD.nDomainType != 1)    continue;
				if(!m_pDoc->m_pAttrCtrl->GetSbdo(DoelD.KeyDomain, SbdoD))     {  ASSERT(0);		return ;	}
				if(SbdoD.nType != 4)    continue;
				aMrebK.Add(aElemK[i]);
			}

			if(!m_pDoc->m_pDataCtrl->AddRbslMreb(RbslK, m_RbslD, aMrebK))     {  		return ;	}
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CDgnConMeshedSlabWallDlg::OnDgnDelete()
{
	CStringArray aStrName;
	int nSize = GetSelectedItems(aStrName);
	if(nSize < 1)   return;
	
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	
	//////////////////////////////////////////////////////////////////////////
	if(nMembType == 0)      // slab
	{
		CArray<T_RBMS_K,T_RBMS_K> aRbmsK;
		aRbmsK.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			aRbmsK[i] = m_pDoc->m_pAttrCtrl->GetRbmsKey(aStrName[i]);
		}
		
		if(!m_pDoc->m_pDataCtrl->DelRbms(aRbmsK))     {  ASSERT(0);		return ;	}
	}
	else if(nMembType == 1)     // wall
	{
		CArray<T_RBMW_K,T_RBMW_K> aRbmwK;
		aRbmwK.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			aRbmwK[i] = m_pDoc->m_pAttrCtrl->GetRbmwKey(aStrName[i]);
		}
		
		if(!m_pDoc->m_pDataCtrl->DelRbmw(aRbmwK))     {  ASSERT(0);		return ;	}
	}
	else if(nMembType == 2)      // strip
	{
		CArray<T_RBST_K,T_RBST_K> aRbstK;
		aRbstK.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			aRbstK[i] = m_pDoc->m_pAttrCtrl2->GetRbstKey(aStrName[i]);
		}

		if(!m_pDoc->m_pDataCtrl->DelRbst(aRbstK))     {  ASSERT(0);		return ;	}
	}
	else if(nMembType == 3)      // shell
	{
		CArray<T_RBSL_K,T_RBSL_K> aRbslK;
		aRbslK.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			aRbslK[i] = m_pDoc->m_pAttrCtrl->GetRbslKey(aStrName[i]);
		}

		if(!m_pDoc->m_pDataCtrl->DelRbsl(aRbslK))     {  ASSERT(0);		return ;	}
	}
	else ASSERT(0);
}

void CDgnConMeshedSlabWallDlg::OnDgnClose()
{
	ShowWindow(SW_HIDE);
	DestroyWindow();
}

void CDgnConMeshedSlabWallDlg::Data2Dlg()
{
	MakeItemEx();

	LRESULT Result;
	OnClkListItem(NULL, &Result);

	ShowControl();
	AlignControl();  
}

BOOL CDgnConMeshedSlabWallDlg::Dlg2Data()
{
	UpdateData(TRUE);

	CString strTemp;

	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	
	//////////////////////////////////////////////////////////////////////////
	if(nMembType == 0)      // slab
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aSlabLayerRdo, m_RbmsD.nLayerType);
		CDlgUtil::CtrlRadioGetCheck(this, m_aPunchRdo, m_RbmsD.nPunchingType);

		m_edtName.GetWindowText(m_RbmsD.strName);
		
		CString strRebarName=_T("");      
		m_cmbBasicName.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");
		m_RbmsD.strBasicRebarName = strRebarName;

		m_cmbAdd1Name.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");
		m_RbmsD.strAdd1RebarName = strRebarName;
		
		m_cmbAdd2Name.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");    
		m_RbmsD.strAdd2RebarName = strRebarName;

		m_cmbPunchRbarName.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");
		m_RbmsD.strPunchRebarName = strRebarName;

		m_cmbPunchStudName.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");    
		m_RbmsD.strStudRebarName = strRebarName;
		
		m_cmbBasicSpace.GetWindowText(strTemp);
		m_RbmsD.dBasicRebarSpace = ConvertSpaceString2Data(strTemp);
		m_cmbAdd1Space.GetWindowText(strTemp);
		m_RbmsD.dAdd1RebarSpace = ConvertSpaceString2Data(strTemp);
		m_cmbPunchRbarSpace.GetWindowText(strTemp);
		m_RbmsD.dPunchRebarSpace= ConvertSpaceString2Data(strTemp);
		m_cmbPunchStudSpace.GetWindowText(strTemp);
		m_RbmsD.dStudRebarSpace = ConvertSpaceString2Data(strTemp);

		m_cmbPunchRbarSpace.GetWindowText(strTemp);
		m_RbmsD.dPunchRebarSpace = ConvertSpaceString2Data(strTemp);
		m_cmbPunchStudSpace.GetWindowText(strTemp);
		m_RbmsD.dStudRebarSpace  = ConvertSpaceString2Data(strTemp);
		//m_cmbAdd2Space.GetWindowText(strTemp);
		//m_RbmsD.dAdd2RebarSpace = ConvertSpaceString2Data(strTemp);
		
		m_RbmsD.dCover = m_edtSlabCover.GetEditValue();
		
		if(m_RbmsD.dCover <= 0.)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_DT_VALUE),MB_OK);
			return FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if(nMembType == 1) // wall
	{
		m_edtName.GetWindowText(m_RbmwD.strName);
		
		m_cmbVerName.GetWindowText(m_RbmwD.strVerRebarName);
		m_cmbHorName.GetWindowText(m_RbmwD.strHorRebarName);
		
		m_cmbVerSpace.GetWindowText(strTemp);
		m_RbmwD.dVerRebarSpace = ConvertSpaceString2Data(strTemp);
		m_cmbHorSpace.GetWindowText(strTemp);
		m_RbmwD.dHorRebarSpace = ConvertSpaceString2Data(strTemp);

		m_RbmwD.dCover = m_edtDw.GetEditValue();

		if(m_RbmwD.dCover <= 0.)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_DW_VALUE),MB_OK);
			return FALSE;
		}
	}
	else if(nMembType == 2) // strip
	{
		m_edtName.GetWindowText(m_RbstD.strName);
		CDlgUtil::CtrlRadioGetCheck(this, m_aStripLayerRdo, m_RbstD.nLayerType);

		CString strRebarName_I,strRebarNameI_C,strRebarNameI_J;      
		m_cmbStripName_I.GetWindowText(strRebarName_I);
		if(strRebarName_I == _LS(IDS_DGN__NONE)) strRebarName_I = _T("");
		m_RbstD.strRebarName_I = strRebarName_I;
		if(!m_bStripSameRebarSize) m_cmbStripName_C.GetWindowText(strRebarNameI_C);
		if(strRebarNameI_C == _LS(IDS_DGN__NONE)) strRebarNameI_C = _T("");
		m_RbstD.strRebarName_C = strRebarNameI_C;
		if(!m_bStripSameRebarSize) m_cmbStripName_J.GetWindowText(strRebarNameI_J);
		if(strRebarNameI_J == _LS(IDS_DGN__NONE)) strRebarNameI_J = _T("");
		m_RbstD.strRebarName_J = strRebarNameI_J;

		m_cmbStripSpace_I.GetWindowText(strTemp);
		m_RbstD.dRebarSpace_I = ConvertSpaceString2Data(strTemp);
		if(!m_bStripSameRebarSize) m_cmbStripSpace_C.GetWindowText(strTemp);
		m_RbstD.dRebarSpace_C = ConvertSpaceString2Data(strTemp);
		if(!m_bStripSameRebarSize) m_cmbStripSpace_J.GetWindowText(strTemp);
		m_RbstD.dRebarSpace_J = ConvertSpaceString2Data(strTemp);
		m_RbstD.dCover = m_edtStripCover.GetEditValue();
		if(m_RbstD.dCover <= 0.)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_DT_VALUE),MB_OK);
			return FALSE;
		}
	}
	else if(nMembType == 3)      // Shell
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aShellLayerRdo, m_RbslD.nLayerType);    

		m_edtName.GetWindowText(m_RbslD.strName);

		CString strRebarName=_T("");      
		m_cmbShellBasicName.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");
		m_RbslD.strBasicRebarName = strRebarName;

		m_cmbShellAdd1Name.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");
		m_RbslD.strAdd1RebarName = strRebarName;

		m_cmbShellAdd2Name.GetWindowText(strRebarName);
		if(strRebarName == _LS(IDS_DGN__NONE)) strRebarName = _T("");    
		m_RbslD.strAdd2RebarName = strRebarName;

		m_cmbShellBasicSpace.GetWindowText(strTemp);
		m_RbslD.dBasicRebarSpace = ConvertSpaceString2Data(strTemp);
		m_cmbShellAdd1Space.GetWindowText(strTemp);
		m_RbslD.dAdd1RebarSpace = ConvertSpaceString2Data(strTemp);
				
		m_RbslD.dCover = m_edtShellCover.GetEditValue();

		if(m_RbslD.dCover <= 0.)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_DT_VALUE),MB_OK);
			return FALSE;
		}

		m_RbslD.bThickOptimal = m_chkAuto.GetCheck();
		m_RbslD.dTopThik = m_edtTop.GetEditValue();		
		m_RbslD.dBotThik = m_edtBot.GetEditValue();		
		m_RbslD.nIteration = m_edtItr.GetEditValueInt();
		m_RbslD.dConvTolerance = m_edtTol.GetEditValue();		
	}
	else ASSERT(0);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDgnConMeshedSlabWallDlg::InitCtrls()
{
	SetListCtrlHeader();

	m_edtElem.Connect(SC_ID_ELEM, &m_edtElem);

	CDlgUtil::CtrlRadioSetCheck(this, m_aMembTypeRdo, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSlabLayerRdo, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aStripLayerRdo, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aPunchRdo, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aShellLayerRdo, 0);  

	InitRebarNameComboBox();
	InitRebarSpaceComboBox();

	// Basic Rebar는 Global에서 컨트롤 한다.
	m_cmbBasicName.EnableWindow(FALSE);
	m_cmbBasicSpace.EnableWindow(FALSE);

	m_edtSlabCover.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untSlabCover.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtStripCover.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStripCover.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_cmbShellBasicName.EnableWindow(FALSE);
	m_cmbShellBasicSpace.EnableWindow(FALSE);
	m_edtShellCover.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untShellCover.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtItr.SetRange(1, 200);
	m_edtItr.SetInteger(TRUE);
	m_edtTop.SetUnitType(D_UNITSYS_NONE);
	m_edtBot.SetUnitType(D_UNITSYS_NONE);	
	m_edtTol.SetUnitType(D_UNITSYS_NONE);

	/// [GEN-7375] by GAY. meshed wall의 dw 값은 사용하지 않아서 HIDE 처리.
	GetDlgItem(IDC_DGN_DW_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DW_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DW_UNT)->ShowWindow(SW_HIDE);

// 국문 버전에서는 Wall 대신 Strip 사용
#if defined(_ORG)  || defined(_JP) || defined(_CH)
	ShowStripHideWall();
	m_bShowStrip = TRUE;
#endif  
}

void CDgnConMeshedSlabWallDlg::ChangeCaption()
{
	BOOL bAuto = m_chkAuto.GetCheck();
	if(bAuto)
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_AUTO));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_AUTO));      
	}
	else
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_MANUAL));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_MANUAL));      
	}
}

void CDgnConMeshedSlabWallDlg::EnableDisableCtrls()
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	int nShellLayer;
	CDlgUtil::CtrlRadioGetCheck(this, m_aShellLayerRdo, nShellLayer);

	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	CDlgUtil::CtrlEnableDisable(this, m_aSlabCtrls, nMembType == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aWallCtrls, nMembType == 1);
	CDlgUtil::CtrlEnableDisable(this, m_aStripCtrls, nMembType == 2);

	GetDlgItem(IDC_DGN_ADD_REP_BTN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_REPLACE_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_DELETE_BTN)->EnableWindow(TRUE);

	int nPunchRdo=0; // 0:Add Rebar, 1:Add Stud
	CDlgUtil::CtrlRadioGetCheck(this, m_aPunchRdo, nPunchRdo);
	CDlgUtil::CtrlEnableDisable(this, m_aPunchRebrCtrls, nPunchRdo == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aPunchStudCtrls, nPunchRdo == 1);
		
	BOOL bAuto = m_chkAuto.GetCheck();

	GetDlgItem(IDC_WG_DGN_LTHK_SAND_STC)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_AUTO_CHK)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_TOP_STC)->EnableWindow(nShellLayer == 0);	
	GetDlgItem(IDC_DGN_TOP_EDT)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_TOP_STC3)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_BOT_STC)->EnableWindow(nShellLayer == 0);	
	GetDlgItem(IDC_DGN_BOT_EDT)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_BOT_STC3)->EnableWindow(nShellLayer == 0);
	GetDlgItem(IDC_DGN_NUM_OF_ITR_STC)->EnableWindow(bAuto && nShellLayer == 0);	
	GetDlgItem(IDC_DGN_NUM_OF_ITR_EDT)->EnableWindow(bAuto && nShellLayer == 0);	
	GetDlgItem(IDC_DGN_TOL_STC)->EnableWindow(bAuto && nShellLayer == 0);	
	GetDlgItem(IDC_DGN_TOL_EDT)->EnableWindow(bAuto && nShellLayer == 0);

	if(CDBLib::IsCodeForMeshedDesignAIJ_JP())
	{
		GetDlgItem(IDC_DGN_STRIP_RDO)->EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_STRIP_GRP, FALSE, TRUE);
	}
}

void CDgnConMeshedSlabWallDlg::InitRebarNameComboBox()
{
	CStringArray arRebar;	arRebar.RemoveAll();
	CDBLib::GetRebarNameListByCode(arRebar);
		
	auto InitComboBox = [&arRebar](CComboBox& combo, bool addNone = true)
	{
		combo.SetRedraw(FALSE);
		
		if (addNone == true)
		{
			combo.AddString(_LS(IDS_DGN__NONE));
		}

		int nRebarSize = arRebar.GetSize();
		for (int i = 0; i < nRebarSize; ++i)
		{
			combo.AddString(arRebar[i]);
		}
		combo.SetCurSel(0);

		combo.SetRedraw(TRUE);
	};

	InitComboBox(m_cmbBasicName);
	InitComboBox(m_cmbAdd1Name);
	InitComboBox(m_cmbAdd2Name);
	InitComboBox(m_cmbVerName, false);
	InitComboBox(m_cmbHorName, false);
	InitComboBox(m_cmbPunchRbarName);
	InitComboBox(m_cmbPunchStudName);
	InitComboBox(m_cmbStripName_I);
	InitComboBox(m_cmbStripName_C);
	InitComboBox(m_cmbStripName_J);
	InitComboBox(m_cmbShellBasicName);
	InitComboBox(m_cmbShellAdd1Name);
	InitComboBox(m_cmbShellAdd2Name);
}

CString CDgnConMeshedSlabWallDlg::ConvertSpaceData2String(double dSpace)
{
	CString strSpace;
	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dTempSpace = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%f"), dTempSpace);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dTempSpace = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%f"), dTempSpace);
	}
	else      ASSERT(0);

	return strSpace;
}

double CDgnConMeshedSlabWallDlg::ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange/*=FALSE*/)
{
	double dSpace = 0.0;
	double dTempSpace = 0.0;

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_M:
	case D_UNITSYS_LENGTH_INDEX_MM:
	case D_UNITSYS_LENGTH_INDEX_CM:
		if(m_bPrevUnitType == 1 && bUnitChange)
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, IN, D_UNITSYS_BASE_LENGTH);
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:   case D_UNITSYS_LENGTH_INDEX_FT:
		strSpace.Remove('\\');
		if(m_bPrevUnitType == 0 && bUnitChange)
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, IN, D_UNITSYS_BASE_LENGTH);
		break;
	default:    ASSERT(0);    break;
	}

	return dSpace;
}

void CDgnConMeshedSlabWallDlg::InitRebarSpaceComboBox()
{
	CStringArray arItem; arItem.RemoveAll();

	T_UNIT_INDEX UnitIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		arItem.Add(_LSX(100));
		arItem.Add(_LSX(125));
		arItem.Add(_LSX(150));
		arItem.Add(_LSX(200));
		arItem.Add(_LSX(250));
		arItem.Add(_LSX(300));
		arItem.Add(_LSX(350));
		arItem.Add(_LSX(400));
		arItem.Add(_LSX(450));
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		arItem.Add(_ULS('4'));
		arItem.Add(_ULS('5'));
		arItem.Add(_ULS('6'));
		arItem.Add(_ULS('8'));
		arItem.Add(_ULS('10'));
		arItem.Add(_ULS('12'));
		arItem.Add(_ULS('14'));
		arItem.Add(_ULS('16'));
		arItem.Add(_ULS('18'));
	}
	else   {  ASSERT(0);		return ;	}

	auto InitComboBox = [&arItem](MComboBox& comboBox)
	{
		comboBox.SetRedraw(FALSE);

		comboBox.ResetContent();

		int nRebarSize = arItem.GetSize();
		for (int i = 0; i < nRebarSize; ++i)
		{
			comboBox.AddString(arItem[i]);
		}

		comboBox.SetRedraw(TRUE);
	};

	InitComboBox(m_cmbBasicSpace);
	InitComboBox(m_cmbAdd1Space);
	//InitComboBox(m_cmbAdd2Space);
	InitComboBox(m_cmbVerSpace);
	InitComboBox(m_cmbHorSpace);
	InitComboBox(m_cmbPunchRbarSpace);
	InitComboBox(m_cmbPunchStudSpace);
	InitComboBox(m_cmbStripSpace_I);
	InitComboBox(m_cmbStripSpace_C);
	InitComboBox(m_cmbStripSpace_J);
	InitComboBox(m_cmbShellBasicSpace);
	InitComboBox(m_cmbShellAdd1Space);
}

// List Ctrl 함수들
void CDgnConMeshedSlabWallDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strName;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		strName = m_List.GetItemText(nIndex, 0);
	}
	else
	{
		GetSelectedItem(strName);
	}

	CStringArray aStrName;
	int nSelected = GetSelectedItems(aStrName);  
	if(nSelected == 0) strName.Empty();

	BOOL bReplace = TRUE;
	if(nSelected > 1) bReplace = FALSE;
	GetDlgItem(IDC_DGN_ADD_REP_BTN)->EnableWindow(bReplace);
	
	CArray<T_ELEM_K, T_ELEM_K> aElemK, aTempK;
	CArray<T_MSTR_K, T_MSTR_K> aSelectedMstrK;

	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	//////////////////////////////////////////////////////////////////////////
	if(nMembType == 0)      // slab
	{
		int nLayer = 0;    
		CDlgUtil::CtrlRadioGetCheck(this, m_aSlabLayerRdo, nLayer);

		CArray<T_RBMS_K,T_RBMS_K> aRbmsK;  aRbmsK.SetSize(nSelected);
		for(int i = 0; i < nSelected; i++) aRbmsK[i] = m_pDoc->m_pAttrCtrl->GetRbmsKey(aStrName[i]);
		
		T_MSRB_D MsrbD;    MsrbD.Initialize();
		m_pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);      

		double dGlobalCover = 0.;
		if     (nLayer == 0) dGlobalCover = MsrbD.dT_Slab_X;
		else if(nLayer == 1) dGlobalCover = MsrbD.dB_Slab_X;
		else if(nLayer == 2) dGlobalCover = MsrbD.dT_Slab_Y;
		else if(nLayer == 3) dGlobalCover = MsrbD.dB_Slab_Y;
		else ASSERT(0);
		
		CString strSpace = MsrbD.RebarSlabSpace[0];
		strSpace.Remove('@');
		double dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
		
		double dDefualtCover = M_InitValueCurUnit(26.0,N,MM,D_UNITSYS_BASE_LENGTH);

		BOOL bExistRbms = FALSE; 
		CString strBasicName=_T(""); 
		double dBasicSpace=0.;

		T_RBMS_K RbmsK = m_pDoc->m_pAttrCtrl->GetRbmsKey(strName);
		T_RBMS_D RbmsD; RbmsD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetRbms(RbmsK, RbmsD))
		{
			RbmsD.strAdd1RebarName  = MsrbD.RebarSlabSize[0];      
			RbmsD.dAdd1RebarSpace   = dSpace;
			RbmsD.strPunchRebarName = MsrbD.RebarSlabSize[0];      
			RbmsD.dPunchRebarSpace  = dSpace;
			RbmsD.dStudRebarSpace   = dSpace;
			RbmsD.dCover = dGlobalCover == 0.0 ? dDefualtCover : dGlobalCover; 
		}     
		else 
		{
			strBasicName = RbmsD.strBasicRebarName;
			dBasicSpace  = RbmsD.dBasicRebarSpace;
			bExistRbms = TRUE;
		}
		m_edtName.SetWindowText(RbmsD.strName);
	 
		CString strNameList;
		for(int k = 0; k < nSelected; k++)
		{
			int nCount = m_pDoc->m_pAttrCtrl->GetElemKeyListUseRbms(aRbmsK[k], aTempK);
			aElemK.Append(aTempK);

			if(!m_pDoc->m_pAttrCtrl->GetRbms(aRbmsK[k], RbmsD))     {  ASSERT(0);		return ;	}
			if(k > 0)   strNameList += _T(", ");
			strNameList += RbmsD.strName;
		}
		if(strNameList != _T(""))   m_edtName.SetWindowText(strNameList);

		//////////////////////////////////////////////////////////////////////////
		// [Check before Release...!!] KIM GEUN-YOUNG20111216 : 
		if(!bExistRbms)
		{
			for (int k=0; k<aElemK.GetSize(); k++ )
			{    
				T_DOEL_D DoelD;
				if(!m_pDoc->m_pAttrCtrl->GetDoel(aElemK[k], DoelD)) continue;
				if(DoelD.nDomainType != 1) continue;
				T_SBDO_D SbdoD;
				if(!m_pDoc->m_pAttrCtrl->GetSbdo(DoelD.KeyDomain, SbdoD)) {ASSERT(0); return;}
				if(SbdoD.bBasicRebar)
				{
					if(nLayer == 0)
					{
						strBasicName = SbdoD.strTopRebarName_X;
						dBasicSpace  = SbdoD.dTopRebarSpace_X;
					}
					else if(nLayer == 1)
					{
						strBasicName = SbdoD.strBotRebarName_X;
						dBasicSpace  = SbdoD.dBotRebarSpace_X;
					}
					else if(nLayer == 2)
					{
						strBasicName = SbdoD.strTopRebarName_Y;
						dBasicSpace  = SbdoD.dTopRebarSpace_Y;
					}
					else if(nLayer == 3)
					{
						strBasicName = SbdoD.strBotRebarName_Y;
						dBasicSpace  = SbdoD.dBotRebarSpace_Y;
					}
					else { ASSERT(0); return; }
					
					bExistRbms = TRUE;        
					break;
				}
			}      
			
			if(!bExistRbms)
			{
				T_MSRB_D MsrbD;
				if(m_pDoc->m_pAttrCtrl->GetMsrb(MsrbD))
				{
					if(MsrbD.bBasicRebar)
					{
						if(nLayer == 0)
						{
							strBasicName = MsrbD.strTopRebarName_X;
							dBasicSpace  = MsrbD.dTopRebarSpace_X;
						}
						else if(nLayer == 1)
						{
							strBasicName = MsrbD.strBotRebarName_X;
							dBasicSpace  = MsrbD.dBotRebarSpace_X;
						}
						else if(nLayer == 2)
						{
							strBasicName = MsrbD.strTopRebarName_Y;
							dBasicSpace  = MsrbD.dTopRebarSpace_Y;
						}
						else if(nLayer == 3)
						{
							strBasicName = MsrbD.strBotRebarName_Y;
							dBasicSpace  = MsrbD.dBotRebarSpace_Y;
						}
						else { ASSERT(0); return; }
					}     
				}      
			}    
		}    
		//////////////////////////////////////////////////////////////////////////    

		if(strBasicName == _T(""))
			m_cmbBasicName.SetCurSel(0);
		else
		{
			int nFind = m_cmbBasicName.FindString(0, strBasicName);
			if(nFind > 0) m_cmbBasicName.SelectString(0, strBasicName);  
			else          m_cmbBasicName.SetCurSel(0);
		}

		if(RbmsD.strAdd1RebarName == _T(""))
			m_cmbAdd1Name.SetCurSel(0);
		else
		{
			int nFind = m_cmbAdd1Name.FindString(0, RbmsD.strAdd1RebarName);
			if(nFind > 0) m_cmbAdd1Name.SelectString(0, RbmsD.strAdd1RebarName); 
			else          m_cmbAdd1Name.SetCurSel(0);
		}
					
		if(RbmsD.strAdd2RebarName == _T(""))    
			m_cmbAdd2Name.SetCurSel(0);
		else
		{
			int nFind = m_cmbAdd2Name.FindString(0, RbmsD.strAdd2RebarName);
			if(nFind > 0) m_cmbAdd2Name.SelectString(0, RbmsD.strAdd2RebarName);
			else          m_cmbAdd2Name.SetCurSel(0);    
		}

		if(RbmsD.strPunchRebarName == _T(""))
			m_cmbPunchRbarName.SetCurSel(0);
		else
		{
			int nFind = m_cmbPunchRbarName.FindString(0, RbmsD.strPunchRebarName);
			if(nFind > 0) m_cmbPunchRbarName.SelectString(0, RbmsD.strPunchRebarName); 
			else          m_cmbPunchRbarName.SetCurSel(0);
		}

		if(RbmsD.strStudRebarName == _T(""))    
			m_cmbPunchStudName.SetCurSel(0);
		else
		{
			int nFind = m_cmbPunchStudName.FindString(0, RbmsD.strStudRebarName);
			if(nFind > 0) m_cmbPunchStudName.SelectString(0, RbmsD.strStudRebarName);
			else          m_cmbPunchStudName.SetCurSel(0);    
		}

		CDlgUtil::CtrlRadioSetCheck(this, m_aPunchRdo, RbmsD.nPunchingType);
		m_cmbBasicSpace.SetWindowText(ConvertSpaceData2String(dBasicSpace));
		m_cmbAdd1Space.SetWindowText(ConvertSpaceData2String(RbmsD.dAdd1RebarSpace));
		m_cmbPunchRbarSpace.SetWindowText(ConvertSpaceData2String(RbmsD.dPunchRebarSpace));
		m_cmbPunchStudSpace.SetWindowText(ConvertSpaceData2String(RbmsD.dStudRebarSpace));
		
		m_edtSlabCover.SetEditUnit(RbmsD.dCover);

	}
	//////////////////////////////////////////////////////////////////////////
	else if(nMembType == 1)  // wall
	{
		CArray<T_RBMW_K,T_RBMW_K> aRbmwK;    aRbmwK.SetSize(nSelected);
		for(int i = 0; i < nSelected; i++)
		{
			aRbmwK[i] = m_pDoc->m_pAttrCtrl->GetRbmwKey(aStrName[i]);
		}
		
		T_RBMW_K RbmwK = m_pDoc->m_pAttrCtrl->GetRbmwKey(strName);
		T_RBMW_D RbmwD;
		if (!m_pDoc->m_pAttrCtrl->GetRbmw(RbmwK, RbmwD))
		{
			RbmwD.Initialize();
			T_MSRB_D MsrbD;    MsrbD.Initialize();
			m_pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);    
			RbmwD.strVerRebarName = MsrbD.RebarWallVerSize[0];
			RbmwD.strHorRebarName = MsrbD.RebarWallHorSize[0];

			CString strSpace = MsrbD.RebarWallSpace[0];
			strSpace.Remove('@');
			double dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
			RbmwD.dVerRebarSpace = dSpace;
			RbmwD.dHorRebarSpace = dSpace;

			double dDefualtCover = M_InitValueCurUnit(50.8,N,MM,D_UNITSYS_BASE_LENGTH);
			RbmwD.dCover = MsrbD.dW1 == 0.0?    dDefualtCover:    MsrbD.dW1;
		}
		m_edtName.SetWindowText(RbmwD.strName);

		m_cmbVerName.SelectString(0, RbmwD.strVerRebarName);
		m_cmbHorName.SelectString(0, RbmwD.strHorRebarName);
		
		m_cmbVerSpace.SetWindowText(ConvertSpaceData2String(RbmwD.dVerRebarSpace));
		m_cmbHorSpace.SetWindowText(ConvertSpaceData2String(RbmwD.dHorRebarSpace));

		m_edtDw.SetEditUnit(RbmwD.dCover);

		CString strNameList;
		for(int k = 0; k < nSelected; k++)
		{
			int nCount = m_pDoc->m_pAttrCtrl->GetElemKeyListUseRbmw(aRbmwK[k], aTempK);
			aElemK.Append(aTempK);

			if(!m_pDoc->m_pAttrCtrl->GetRbmw(aRbmwK[k], RbmwD))     {  ASSERT(0);		return ;	}
			if(k > 0)   strNameList += _T(", ");
			strNameList += RbmwD.strName;
		}
		if(strNameList != _T(""))    m_edtName.SetWindowText(strNameList);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nMembType == 2)  // strip
	{
		CArray<T_RBST_K,T_RBST_K> aRbstK;    aRbstK.SetSize(nSelected);
		for(int i = 0; i < nSelected; i++)
		{
			aRbstK[i] = m_pDoc->m_pAttrCtrl2->GetRbstKey(aStrName[i]);
		}

		T_RBST_K RbstK = m_pDoc->m_pAttrCtrl2->GetRbstKey(strName);
		T_RBST_D RbstD;
		if (!m_pDoc->m_pAttrCtrl2->GetRbst(RbstK, RbstD))
		{
			RbstD.Initialize();
			T_MSRB_D MsrbD;    MsrbD.Initialize();
			m_pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);    

			// Strip Rebar는 Default를 None으로 갖는다.
			RbstD.strRebarName_I = _T("");
			RbstD.strRebarName_C = _T("");
			RbstD.strRebarName_J = _T("");

			CString strSpace = MsrbD.RebarStripSpace[0];
			strSpace.Remove('@');
			double dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
			RbstD.dRebarSpace_I = dSpace;
			RbstD.dRebarSpace_C = dSpace;
			RbstD.dRebarSpace_J = dSpace;

			double dGlobalCover = 0.;
			int nLayer = 0;    
			CDlgUtil::CtrlRadioGetCheck(this, m_aStripLayerRdo, nLayer);
			if     (nLayer == 0) dGlobalCover = MsrbD.dT_Strip;
			else if(nLayer == 1) dGlobalCover = MsrbD.dB_Strip;
			else ASSERT(0);

			double dDefualtCover = M_InitValueCurUnit(30.0,N,MM,D_UNITSYS_BASE_LENGTH);
			RbstD.dCover = dGlobalCover == 0.0? dDefualtCover:dGlobalCover;
		}
		m_edtName.SetWindowText(RbstD.strName);

		if(RbstD.strRebarName_I == _T(""))  m_cmbStripName_I.SetCurSel(0);
		else                            m_cmbStripName_I.SelectString(0, RbstD.strRebarName_I);
		if(RbstD.strRebarName_C == _T(""))  m_cmbStripName_C.SetCurSel(0);
		else                            m_cmbStripName_C.SelectString(0, RbstD.strRebarName_C);
		if(RbstD.strRebarName_J == _T(""))  m_cmbStripName_J.SetCurSel(0);
		else                            m_cmbStripName_J.SelectString(0, RbstD.strRebarName_J);

		m_cmbStripSpace_I.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_I));
		m_cmbStripSpace_C.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_C));
		m_cmbStripSpace_J.SetWindowText(ConvertSpaceData2String(RbstD.dRebarSpace_J));
		
		m_edtStripCover.SetEditUnit(RbstD.dCover);

		CString strNameList;
		for(int k = 0; k < nSelected; k++)
		{
			int nCount = m_pDoc->m_pAttrCtrl2->GetElemKeyListUseRbst(aRbstK[k], aTempK);
			aElemK.Append(aTempK);

			if(!m_pDoc->m_pAttrCtrl2->GetRbst(aRbstK[k], RbstD))     {  ASSERT(0);		return ;	}
			if(k > 0)   strNameList += _T(", ");
			strNameList += RbstD.strName;
		}
		if(strNameList != _T(""))    m_edtName.SetWindowText(strNameList);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if(nMembType == 3)      // Shell
	{
		int nLayer = 0;    
		CDlgUtil::CtrlRadioGetCheck(this, m_aShellLayerRdo, nLayer);

		CArray<T_RBSL_K,T_RBSL_K> aRbslK;  aRbslK.SetSize(nSelected);
		for(int i = 0; i < nSelected; i++) aRbslK[i] = m_pDoc->m_pAttrCtrl->GetRbslKey(aStrName[i]);

		T_MSRB_D MsrbD;    MsrbD.Initialize();
		m_pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);      

		double dGlobalCover = 0.;
		if     (nLayer == 0) dGlobalCover = MsrbD.dT_Shell_X;
		else if(nLayer == 1) dGlobalCover = MsrbD.dB_Shell_X;
		else if(nLayer == 2) dGlobalCover = MsrbD.dT_Shell_Y;
		else if(nLayer == 3) dGlobalCover = MsrbD.dB_Shell_Y;
		else ASSERT(0);

		CString strSpace = MsrbD.RebarShellSpace[0];
		strSpace.Remove('@');
		double dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);

		double dDefualtCover = M_InitValueCurUnit(26.0,N,MM,D_UNITSYS_BASE_LENGTH);

		BOOL bExistRbsl = FALSE; 
		CString strBasicName=_T(""); 
		double dBasicSpace=0.;

		T_RBSL_K RbslK = m_pDoc->m_pAttrCtrl->GetRbslKey(strName);
		T_RBSL_D RbslD; RbslD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetRbsl(RbslK, RbslD))
		{
			RbslD.strAdd1RebarName  = MsrbD.RebarShellSize[0];      
			RbslD.dAdd1RebarSpace   = dSpace;
			RbslD.dCover = dGlobalCover == 0.0 ? dDefualtCover : dGlobalCover; 
		}     
		else 
		{
			strBasicName = RbslD.strBasicRebarName;
			dBasicSpace  = RbslD.dBasicRebarSpace;
			bExistRbsl = TRUE;
		}
		m_edtName.SetWindowText(RbslD.strName);

		CString strNameList;
		for(int k = 0; k < nSelected; k++)
		{
			int nCount = m_pDoc->m_pAttrCtrl->GetElemKeyListUseRbsl(aRbslK[k], aTempK);
			aElemK.Append(aTempK);

			if(!m_pDoc->m_pAttrCtrl->GetRbsl(aRbslK[k], RbslD))     {  ASSERT(0);		return ;	}
			if(k > 0)   strNameList += _T(", ");
			strNameList += RbslD.strName;
		}
		if(strNameList != _T(""))   m_edtName.SetWindowText(strNameList);

		//////////////////////////////////////////////////////////////////////////
		// [Check before Release...!!] KIM GEUN-YOUNG20111216 : 
		if(!bExistRbsl)
		{
			for (int k=0; k<aElemK.GetSize(); k++ )
			{    
				T_DOEL_D DoelD;
				if(!m_pDoc->m_pAttrCtrl->GetDoel(aElemK[k], DoelD)) continue;
				if(DoelD.nDomainType != 1) continue;
				T_SBDO_D SbdoD;
				if(!m_pDoc->m_pAttrCtrl->GetSbdo(DoelD.KeyDomain, SbdoD)) {ASSERT(0); return;}
				if(SbdoD.bBasicRebar)
				{
					if(nLayer == 0)
					{
						strBasicName = SbdoD.strTopRebarName_X;
						dBasicSpace  = SbdoD.dTopRebarSpace_X;
					}
					else if(nLayer == 1)
					{
						strBasicName = SbdoD.strBotRebarName_X;
						dBasicSpace  = SbdoD.dBotRebarSpace_X;
					}
					else if(nLayer == 2)
					{
						strBasicName = SbdoD.strTopRebarName_Y;
						dBasicSpace  = SbdoD.dTopRebarSpace_Y;
					}
					else if(nLayer == 3)
					{
						strBasicName = SbdoD.strBotRebarName_Y;
						dBasicSpace  = SbdoD.dBotRebarSpace_Y;
					}
					else { ASSERT(0); return; }

					bExistRbsl = TRUE;        
					break;
				}
			}      

			if(!bExistRbsl)
			{
				T_MSRB_D MsrbD;
				if(m_pDoc->m_pAttrCtrl->GetMsrb(MsrbD))
				{
					if(MsrbD.bBasicRebar)
					{
						if(nLayer == 0)
						{
							strBasicName = MsrbD.strTopRebarName_X;
							dBasicSpace  = MsrbD.dTopRebarSpace_X;
						}
						else if(nLayer == 1)
						{
							strBasicName = MsrbD.strBotRebarName_X;
							dBasicSpace  = MsrbD.dBotRebarSpace_X;
						}
						else if(nLayer == 2)
						{
							strBasicName = MsrbD.strTopRebarName_Y;
							dBasicSpace  = MsrbD.dTopRebarSpace_Y;
						}
						else if(nLayer == 3)
						{
							strBasicName = MsrbD.strBotRebarName_Y;
							dBasicSpace  = MsrbD.dBotRebarSpace_Y;
						}
						else { ASSERT(0); return; }
					}     
				}      
			}    
		}    
		//////////////////////////////////////////////////////////////////////////    

		if(strBasicName == _T(""))
			m_cmbShellBasicName.SetCurSel(0);
		else
		{
			int nFind = m_cmbShellBasicName.FindString(0, strBasicName);
			if(nFind > 0) m_cmbShellBasicName.SelectString(0, strBasicName);  
			else          m_cmbShellBasicName.SetCurSel(0);
		}

		if(RbslD.strAdd1RebarName == _T(""))
			m_cmbShellAdd1Name.SetCurSel(0);
		else
		{
			int nFind = m_cmbShellAdd1Name.FindString(0, RbslD.strAdd1RebarName);
			if(nFind > 0) m_cmbShellAdd1Name.SelectString(0, RbslD.strAdd1RebarName); 
			else          m_cmbShellAdd1Name.SetCurSel(0);
		}

		if(RbslD.strAdd2RebarName == _T(""))    
			m_cmbShellAdd2Name.SetCurSel(0);
		else
		{
			int nFind = m_cmbShellAdd2Name.FindString(0, RbslD.strAdd2RebarName);
			if(nFind > 0) m_cmbShellAdd2Name.SelectString(0, RbslD.strAdd2RebarName);
			else          m_cmbShellAdd2Name.SetCurSel(0);    
		}
		
		m_cmbShellBasicSpace.SetWindowText(ConvertSpaceData2String(dBasicSpace));
		m_cmbShellAdd1Space.SetWindowText(ConvertSpaceData2String(RbslD.dAdd1RebarSpace));
		
		m_edtShellCover.SetEditUnit(RbslD.dCover);

		m_chkAuto.SetCheck(RbslD.bThickOptimal);
		//	m_edtElvBot.SetEditUnit(RbslD.dElvBot);
		m_edtTop.SetEditUnit(RbslD.dTopThik);      
		m_edtBot.SetEditUnit(RbslD.dBotThik);    		
		m_edtItr.SetValue(RbslD.nIteration);
		m_edtTol.SetEditUnit(RbslD.dConvTolerance);
	}
	//////////////////////////////////////////////////////////////////////////

	BOOL bEnable = TRUE;
	if(nSelected > 1)   bEnable = FALSE;
	m_edtName.EnableWindow(bEnable);
	m_edtElem.EnableWindow(bEnable);

	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);
	
	EnableDisableCtrls();

	//m_List.SetFocus();

	ChangeCaption();

	*pResult = 0;
}

void CDgnConMeshedSlabWallDlg::SetListCtrlHeader()
{
	LV_COLUMN lvcolumn;
	
	CRect rect;
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rect);
	int nColWidth = rect.Width();
	
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Set Title
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = nColWidth;
	CString strTitle = _LS(IDS_DGN_NAME);
	lvcolumn.pszText = strTitle.GetBuffer(0);
	m_List.InsertColumn(0,&lvcolumn);
}

BOOL CDgnConMeshedSlabWallDlg::GetSelectedItem(CString& strName)
{
	int nItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) return FALSE;
	
	strName = m_List.GetItemText(nItem, 0);

	return TRUE;
}

int CDgnConMeshedSlabWallDlg::GetSelectedItems(CStringArray& aStrName)
{
	aStrName.RemoveAll();
	
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	
	while (pos)
	{
		int nItem = m_List.GetNextSelectedItem(pos);      
		
		aStrName.Add(m_List.GetItemText(nItem, 0));
	}
	
	return aStrName.GetSize();
}

void CDgnConMeshedSlabWallDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	CStringArray aStrName;

	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);

	if(nMembType == 0)
	{
		int nLayer = 0;    
		CDlgUtil::CtrlRadioGetCheck(this, m_aSlabLayerRdo, nLayer);

		CArray<T_RBMS_K,T_RBMS_K> aRbmsK;
		int nCount = m_pDoc->m_pAttrCtrl->GetRbmsKeyList(aRbmsK, nLayer);

		T_RBMS_D RbmsD;
		for(int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetRbms(aRbmsK[i], RbmsD))     {  ASSERT(0);		return ;	}
			aStrName.Add(RbmsD.strName);
		}
	}
	else if(nMembType == 1)
	{
		CArray<T_RBMW_K,T_RBMW_K> aRbmwK;
		int nCount = m_pDoc->m_pAttrCtrl->GetRbmwKeyList(aRbmwK);

		T_RBMW_D RbmwD;
		for(int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetRbmw(aRbmwK[i], RbmwD))     {  ASSERT(0);		return ;	}
			aStrName.Add(RbmwD.strName);
		}
	}
	else if(nMembType == 2)
	{
		int nLayer = 0;    
		CDlgUtil::CtrlRadioGetCheck(this, m_aStripLayerRdo, nLayer);

		CArray<T_RBST_K,T_RBST_K> aRbstK;
		int nCount = m_pDoc->m_pAttrCtrl2->GetRbstKeyListSorted(aRbstK, nLayer);

		T_RBST_D RbstD;
		for(int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl2->GetRbst(aRbstK[i], RbstD))     {  ASSERT(0);		return ;	}
			aStrName.Add(RbstD.strName);
		}
	}
	else if(nMembType == 3)
	{
		int nLayer = 0;    
		CDlgUtil::CtrlRadioGetCheck(this, m_aShellLayerRdo, nLayer);

		CArray<T_RBSL_K,T_RBSL_K> aRbslK;
		int nCount = m_pDoc->m_pAttrCtrl->GetRbslKeyList(aRbslK, nLayer);

		T_RBSL_D RbslD;
		for(int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetRbsl(aRbslK[i], RbslD))     {  ASSERT(0);		return ;	}
			aStrName.Add(RbslD.strName);
		}
	}
	else      {  ASSERT(0);		return ;	}

	int nDataSize = aStrName.GetSize();
	for(int nIndex = 0; nIndex < nDataSize; nIndex++)
	{
		m_List.InsertItem(nIndex, _T(""));
		
		LVITEM lvitem;
		
		lvitem.iItem=nIndex;
		lvitem.iSubItem=0;
		
		lvitem.pszText=aStrName[nIndex].GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_List.SetItem(&lvitem);
	}

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

LRESULT CDgnConMeshedSlabWallDlg::OnAfterInitDialog(WPARAM wParam, LPARAM lParam)
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	if(nMembType == 0)    CreatePictureDlg();
	else                  DestroyPictureDlg();

	return 0L;
}

void CDgnConMeshedSlabWallDlg::CreatePictureDlg()
{
	if(IsAlivePictureDlg()) return ;
	
	m_pPictureDlg = new CDgnSlabRebarPictureDlg();
	m_pPictureDlg->SetInitPos(D_INIT_POS_RT);
	m_pPictureDlg->Create(CDgnSlabRebarPictureDlg::IDD, this);
	m_pPictureDlg->ShowWindow(SW_SHOW);
	
	return ;
}

void CDgnConMeshedSlabWallDlg::DestroyPictureDlg()
{
	// Window가 있으면 없앤다.
	if(m_pPictureDlg != NULL)
	{
		m_pPictureDlg->DestroyWindow();
		delete m_pPictureDlg;
		m_pPictureDlg = NULL;    
	}
}

BOOL CDgnConMeshedSlabWallDlg::IsAlivePictureDlg()
{ 
	if(m_pPictureDlg == NULL) return FALSE;
	HWND hWnd = m_pPictureDlg->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;
	
	return TRUE;
}

void CDgnConMeshedSlabWallDlg::OnShellLayerRdo()
{  
	MakeShellRebarItem();
}

void CDgnConMeshedSlabWallDlg::MakeShellRebarItem()
{
	MakeItemEx(); 
	m_edtName.SetWindowText(_T(""));
}

void CDgnConMeshedSlabWallDlg::OnSlabLayerRdo()
{  
	MakeSlabRebarItem();
}

void CDgnConMeshedSlabWallDlg::MakeSlabRebarItem()
{
	MakeItemEx(); 
	m_edtName.SetWindowText(_T(""));
}

void CDgnConMeshedSlabWallDlg::AlignControl()
{
	CRect rRef, rToMove;
	int nDistX,nDistY;	
	nDistX = nDistY = 0;
	CArray<UINT, UINT> aCtrl;    
	CRect Rect;
	Rect = CRect(0, 0, 0, 0);

#ifdef _CIVIL
	// Member Type Group hide 에 따른 Ctrl 이동(시작)
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_STATIC1);
	aCtrl.Add(IDC_DGN_NAME_EDT);
	
	GetDlgItem(IDC_DGN_SLAB_RDO)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);    
	CDlgUtil::CtrlMoveDistY(this, aCtrl, globalUtils.ScaleByDPI(-8));    

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_STATIC2);  
	aCtrl.Add(IDC_DGN_ELEM_LIST_EDT);
	aCtrl.Add(IDC_DGN_MESHED_SLAB_WALL_LST);
	aCtrl.Add(IDC_DGN_SLAB_STC);
		
	GetDlgItem(IDC_DGN_NAME_EDT)->GetWindowRect(rRef);    
	GetDlgItem(IDC_DGN_ELEM_LIST_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_STATIC1);  
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_DGN_NAME_EDT);
	GetDlgItem(IDC_DGN_ELEM_LIST_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_NAME_EDT)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	// Member Type Group hide 에 따른 Ctrl 이동 (끝)
#endif // _CIVIL

	GetDlgItem(IDC_DGN_SLAB_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SHELL_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aShellCtrls, nDistY);
 
	if(m_bShowStrip) // Strip이 보이고 Wall이 안보일 경우 MemberType Radio에 Shell이 가려지기 때문에 위치를 조절해 주어야 함
	{
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_DGN_SHELL_RDO);

		GetDlgItem(IDC_DGN_STRIP_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_SHELL_RDO)->GetWindowRect(rToMove);
		nDistX = rRef.right - rToMove.left;
		CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);
#ifdef _MGEN_CH
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_DGN_STRIP_RDO);
		GetDlgItem(IDC_DGN_STRIP_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_SHELL_RDO)->GetWindowRect(rToMove);
		nDistY = rToMove.top - rRef.top;
		CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
#endif
	}

#ifdef _CIVIL
	// Baseic Rebar hide 에 따른 Ctrl 이동(시작)
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_DGN_SHELL_ADD1_STC);
	aCtrl.Add(IDC_DGN_SHELL_ADD1_NAME_CMB);
	aCtrl.Add(IDC_DGN_SHELL_ADD1_AT_STC);
	aCtrl.Add(IDC_DGN_SHELL_ADD1_SPACE_CMB);
	aCtrl.Add(IDC_DGN_SHELL_ADD2_STC);
	aCtrl.Add(IDC_DGN_SHELL_ADD2_NAME_CMB);
	aCtrl.Add(IDC_DGN_SHELL_ADD2_AT_STC);
	aCtrl.Add(IDC_DGN_SHELL_ADD2_SPACE_CMB);
	aCtrl.Add(IDC_DGN_SHELL_COVER_STC);
	aCtrl.Add(IDC_DGN_SHELL_COVER_EDT);
	aCtrl.Add(IDC_DGN_SHELL_COVER_UNT);

	GetDlgItem(IDC_DGN_SHELL_BASIC_NAME_CMB)->GetWindowRect(rRef);    
	GetDlgItem(IDC_DGN_SHELL_ADD1_NAME_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
	
	GetDlgItem(IDC_DGN_SHELL_COVER_EDT)->GetWindowRect(rRef);    
	GetDlgItem(IDC_DGN_SHELL_STC)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.bottom + globalUtils.ScaleByDPI(8);  
	GetDlgItem(IDC_DGN_SHELL_STC)->GetWindowRect(Rect);
	GetDlgItem(IDC_DGN_SHELL_STC)->SetWindowPos(NULL,Rect.left,Rect.top,Rect.Width(),Rect.Height()+nDistY,SWP_NOMOVE | SWP_NOZORDER);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_DGN_LTHK_SAND_STC);
	aCtrl.Add(IDC_DGN_AUTO_CHK	 );
	aCtrl.Add(IDC_DGN_TOP_STC);
	aCtrl.Add(IDC_DGN_TOP_EDT);
	aCtrl.Add(IDC_DGN_TOP_STC3);  
	aCtrl.Add(IDC_DGN_BOT_STC);
	aCtrl.Add(IDC_DGN_BOT_EDT);
	aCtrl.Add(IDC_DGN_BOT_STC3);  
	aCtrl.Add(IDC_DGN_NUM_OF_ITR_STC);
	aCtrl.Add(IDC_DGN_NUM_OF_ITR_EDT);	
	aCtrl.Add(IDC_DGN_TOL_STC);
	aCtrl.Add(IDC_DGN_TOL_EDT	);
	GetDlgItem(IDC_DGN_SHELL_STC)->GetWindowRect(rRef);    
	GetDlgItem(IDC_WG_DGN_LTHK_SAND_STC)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(12);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
	// Baseic Rebar hide 에 따른 Ctrl 이동(끝)

	// 왼쪽에 있는 List Size 맞추기(시작)
	GetDlgItem(IDC_WG_DGN_LTHK_SAND_STC)->GetWindowRect(rRef);    
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.bottom;  
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(Rect);
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->SetWindowPos(NULL,Rect.left,Rect.top,Rect.Width(),Rect.Height()+nDistY,SWP_NOMOVE | SWP_NOZORDER);
	// 왼쪽에 있는 List Size 맞추기(끝)
#endif // _CIVIL

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_DGN_ADD_REP_BTN);
	aCtrl.Add(IDC_DGN_DELETE_BTN);
	aCtrl.Add(IDC_DGN_CLOSE_BTN);

	BOOL bConCodeForShell = CDBLib::IsConCodeForShell();
	if(m_bShowStrip)
	{
		GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rRef);    
	}
	else if(bConCodeForShell)
	{
		GetDlgItem(IDC_WG_DGN_LTHK_SAND_STC)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALL_STC)->GetWindowRect(rRef);        
	}
	GetDlgItem(IDC_DGN_CLOSE_BTN)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	// rRef는 위와 동일한걸 씀..
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom;
	ScreenToClient(rToMove);
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->MoveWindow(rToMove);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_DGN_CLOSE_BTN);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);
	MoveWindow(r);
}

void CDgnConMeshedSlabWallDlg::ShowControl()
{
	int nMembType = 0;    // 0 : slab,    1 : wall,    2 : strip,   3: Shell
	CDlgUtil::CtrlRadioGetCheck(this, m_aMembTypeRdo, nMembType);
	BOOL bConCodeForShell = CDBLib::IsConCodeForShell();
	
	CDlgUtil::CtrlShowHide(this, m_aSlabCtrls,      !bConCodeForShell || nMembType != 3);  
	CDlgUtil::CtrlShowHide(this, m_aWallCtrls,      (!bConCodeForShell || nMembType != 3) && !m_bShowStrip);  
	CDlgUtil::CtrlShowHide(this, m_aSlabPunchCtrls, !bConCodeForShell || nMembType != 3);  
	CDlgUtil::CtrlShowHide(this, m_aStripCtrls,     !bConCodeForShell || nMembType != 3);  
	CDlgUtil::CtrlShowHide(this, m_aShellCtrls, bConCodeForShell && nMembType == 3);  

	GetDlgItem(IDC_DGN_SHELL_RDO)->ShowWindow(bConCodeForShell);
	GetDlgItem(IDC_DGN_SLAB_BASIC_NAME_CMB)->ShowWindow(!bConCodeForShell || nMembType != 3);
	GetDlgItem(IDC_DGN_SLAB_BASIC_SPACE_CMB)->ShowWindow(!bConCodeForShell || nMembType != 3);

	// Add1과 같이 쓰임.
	GetDlgItem(IDC_DGN_SLAB_ADD2_AT_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SLAB_ADD2_SPACE_CMB)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_ADD2_AT_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_ADD2_SPACE_CMB)->ShowWindow(FALSE);  

#ifdef _CIVIL
	GetDlgItem(IDC_WG_DGN_STATIC0)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SLAB_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_WALL_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_RDO)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_STRIP_RDO)->ShowWindow(FALSE);  
	
	GetDlgItem(IDC_DGN_SHELL_BASIC_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_BASIC_NAME_CMB)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_BASIC_AT_STC)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_SHELL_BASIC_SPACE_CMB)->ShowWindow(FALSE);
#endif // _CIVIL  
}

void CDgnConMeshedSlabWallDlg::ShowStripHideWall()
{
	CArray<UINT, UINT> aShowCtrls;
	//aShowCtrls.Append(m_aSlabPunchCtrls); // 전단보강근에 대한 부분은 Hide처리
	aShowCtrls.Append(m_aStripCtrls);

	// 벽 <-> 스트립
	CRect rWall, rStrip;
	int dx=0;
	int dy=0;
	GetDlgItem(IDC_DGN_WALL_STC)->GetWindowRect(rWall);
	GetDlgItem(IDC_DGN_STRIP_GRP)->GetWindowRect(rStrip);
	dx = rWall.left - rStrip.left;
	dy = rWall.top - rStrip.top;
	CDlgUtil::CtrlMoveDistXY(this, aShowCtrls, dx, dy, FALSE);  

	CRect rRef, rToMove;
	CArray<UINT, UINT> aCtrl; 
	int nDistX = 0;
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_DGN_STRIP_RDO);  
	GetDlgItem(IDC_DGN_WALL_RDO)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_STRIP_RDO)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);
	GetDlgItem(IDC_DGN_WALL_RDO )->ShowWindow(SW_HIDE);

	// 기타 컨트롤 및 대화상자 간격 조정
	int dGapY = (rStrip.bottom - rStrip.top) - (rWall.bottom - rWall.top);

	CArray<UINT, UINT> aBtnCtrls;
	aBtnCtrls.Add(IDC_DGN_ADD_REP_BTN);
	aBtnCtrls.Add(IDC_DGN_DELETE_BTN );
	aBtnCtrls.Add(IDC_DGN_CLOSE_BTN  );
	CDlgUtil::CtrlMoveDistXY(this, aBtnCtrls, 0, dGapY, FALSE);

	CRect rList;
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->GetWindowRect(rList);
	GetDlgItem(IDC_DGN_MESHED_SLAB_WALL_LST)->SetWindowPos(NULL,rList.left,rList.top,rList.Width(),rList.Height()+dGapY,SWP_NOMOVE | SWP_NOZORDER);

	CRect rDlg;
	this->GetWindowRect(rDlg);
	this->SetWindowPos(NULL,rDlg.left,rDlg.top,rDlg.Width(),rDlg.Height()+dGapY,SWP_NOMOVE | SWP_NOZORDER);
}

void CDgnConMeshedSlabWallDlg::OnStripLayerRdo()
{
	MakeItemEx(); 
}

void CDgnConMeshedSlabWallDlg::SelectStrip(CArray<T_MSTR_K, T_MSTR_K>& aMstrK)
{
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance(); ASSERT(pServiceHeadOffice);
	T_GR_STRIP_DRAW_OPT StripOpt;
	StripOpt.bShowMstr = TRUE;
	StripOpt.aSelectedMstrK.Copy(aMstrK);
	pServiceHeadOffice->ReqService(_ULS(gr), D_GR_SET_SELECT_MSTR, (void*)&StripOpt);
}

// 대화상자에서 임의로 변경했던 디스플레이 옵션을 원래대로 복원
void CDgnConMeshedSlabWallDlg::RestoreOriginMstrDispOpt()
{
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance(); ASSERT(pServiceHeadOffice);
	T_GR_STRIP_DRAW_OPT StripOpt;
	StripOpt.bShowMstr = m_bOriginMstrDispOpt;
	StripOpt.aSelectedMstrK.RemoveAll();
	pServiceHeadOffice->ReqService(_ULS(gr), D_GR_SET_SELECT_MSTR, (void*)&StripOpt);  
}

void CDgnConMeshedSlabWallDlg::OnClickStripSameSizeChk()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aStripRebrCJCtrls, !m_bStripSameRebarSize);
}

void CDgnConMeshedSlabWallDlg::OnChkAuto()
{
	EnableDisableCtrls();
	ShowControl();
	ChangeCaption();
}