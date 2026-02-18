// DgnMeshBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMeshBarDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DBCodeDef.h"

#include "DgnConBarSizeDlg.h"
#include "DgnConBarInfoDlg.h"
#include "DgnConBarSpaceDlg.h"
#include "DgnConBarSectDlg.h"
#include "DgnSlabRebarPictureDlg.h"
#include "DgnMeshBarLThk4SandwichDlg.h"
#include "DgnDataCtrl.h"
#include "DgnMeshBarMatlDlg.h"

#include "..\MIT_lib\MRegistry.h"

enum { EN_BARSERIES_KS=0, EN_BARSERIES_ASTM, EN_BARSERIES_EN, EN_BARSERIES_GB, EN_BARSERIES_CSA, EN_BARSERIES_UNI, EN_BARSERIES_TW, EN_BARSERIES_SS };

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int CDgnMeshBarDlg::WM_AFTERINITDIALOG = RegisterWindowMessage(_T("WM_AFTERINITDIALOG"));
/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlg dialog


CDgnMeshBarDlg::CDgnMeshBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMeshBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMeshBarDlg)
	m_strBarSlab = _T("");
	m_strSpaceSlab = _T("");
	m_strBarMat  = _T("");
	m_strSpaceMat = _T("");
	m_strVerBarWall = _T("");
	m_strHorBarWall = _T("");
	m_strSpaceWall  = _T("");
	m_strBarStrip   = _T(""); 
	m_strSpaceStrip = _T("");
	m_strBarShell = _T("");
	m_strSpaceShell = _T("");
	//}}AFX_DATA_INIT

	m_Msrb.Initialize();
	m_pPictureDlg = NULL;

	m_bPrevUnitType = 0;
	m_bBasicRebar = FALSE;
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_TOP_X_AREA_CMB );
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_BOT_X_AREA_CMB );
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_TOP_Y_AREA_CMB );
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_BOT_Y_AREA_CMB );
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_TOP_X_SPACE_CMB);
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_BOT_X_SPACE_CMB);
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_TOP_Y_SPACE_CMB);
	m_aBasicCtrls.Add(IDC_DGN_MESH_SLAB_BAR_BOT_Y_SPACE_CMB);

	m_aWallCtrls.Add(IDC_WG_DGN_STATIC29);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC30);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC31);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC32);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC33);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC34);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC35);
	m_aWallCtrls.Add(IDC_WG_DGN_STATIC36);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_VER_BAR_EDIT);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_VER_BAR_BTN);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_HOR_BAR_EDIT);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_HOR_BAR_BTN);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_SPACE_EDIT);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_SPACE_BTN);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_DW1_EDIT);
	m_aWallCtrls.Add(IDC_DGN_MESH_WALL_DW1_UNIT);

	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_GRP);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_RBAR_STC);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_RBAR2_STC);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_RBAR_EDT);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_RBAR_BTN);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_SPAC_STC);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_SPAC2_STC);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_SPAC_EDT);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_SPAC_BTN);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_CONC_GRP);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_MAIN_STC);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_MAIN_STC2);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_MAIN_EDT);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_MAIN_EDT2);
	m_aStripCtrls.Add(IDC_WG_DGN_MESH_BAR_STRIP_MAIN_UNT);
	
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DGN_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_EDIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_BTN);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_EDIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_BTN);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DT_DB_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DIR1_STC);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDT_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDB_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDB_UNIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DIR2_STC);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDT_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDB_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDB_UNIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_THK_SAND_BTN);
	m_aShellCtrls.Add(IDC_WG_DGN_STATIC39);
	m_aShellCtrls.Add(IDC_WG_DGN_STATIC41);

	m_aMatlCtrls.Add(IDC_DGN_CON_MATL_RBAR_DIAM_CHK);
	m_aMatlCtrls.Add(IDC_DGN_CON_REBAR_MATL_BTN);

	m_aOkCancelCtrls.Add(IDOK);
	m_aOkCancelCtrls.Add(IDCANCEL);	
}

void CDgnMeshBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMeshBarDlg)
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_XDB_UNIT, m_unitXDbSlab);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_YDB_UNIT, m_unitYDbSlab);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_XDB_UNIT, m_unitXDbMat);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_YDB_UNIT, m_unitYDbMat);
	DDX_Control(pDX, IDC_DGN_MESH_WALL_DW1_UNIT, m_unitDwWall);
	DDX_Control(pDX, IDC_WG_DGN_MESH_BAR_STRIP_MAIN_UNT, m_unitMainStrip);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDB_UNIT, m_unitXDbShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDB_UNIT, m_unitYDbShell);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_XDT_EDIT, m_editXDtSlab);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_XDB_EDIT, m_editXDbSlab);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_YDT_EDIT, m_editYDtSlab);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_YDB_EDIT, m_editYDbSlab);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_XDT_EDIT, m_editXDtMat);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_XDB_EDIT, m_editXDbMat);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_YDT_EDIT, m_editYDtMat);
	DDX_Control(pDX, IDC_DGN_MESH_MAT_YDB_EDIT, m_editYDbMat);
	DDX_Control(pDX, IDC_DGN_MESH_WALL_DW1_EDIT, m_editDwWall);
	DDX_Control(pDX, IDC_WG_DGN_MESH_BAR_STRIP_MAIN_EDT, m_editDtStrip);
	DDX_Control(pDX, IDC_WG_DGN_MESH_BAR_STRIP_MAIN_EDT2, m_editDbStrip);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDT_EDIT, m_editXDtShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDB_EDIT, m_editXDbShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDT_EDIT, m_editYDtShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDB_EDIT, m_editYDbShell);
	DDX_Text(pDX, IDC_DGN_MESH_SLAB_BAR_EDIT, m_strBarSlab);
	DDX_Text(pDX, IDC_DGN_MESH_SLAB_SPACE_EDIT, m_strSpaceSlab);
	DDX_Text(pDX, IDC_WG_DGN_SHELL_REBAR_EDIT, m_strBarShell);
	DDX_Text(pDX, IDC_WG_DGN_SHELL_SPACE_EDIT, m_strSpaceShell);
	DDX_Text(pDX, IDC_DGN_MESH_MAT_BAR_EDIT, m_strBarMat);
	DDX_Text(pDX, IDC_DGN_MESH_MAT_SPACE_EDIT, m_strSpaceMat);
	DDX_Text(pDX, IDC_DGN_MESH_WALL_VER_BAR_EDIT, m_strVerBarWall);
	DDX_Text(pDX, IDC_DGN_MESH_WALL_HOR_BAR_EDIT, m_strHorBarWall);
	DDX_Text(pDX, IDC_DGN_MESH_WALL_SPACE_EDIT, m_strSpaceWall);  
	DDX_Text(pDX, IDC_WG_DGN_MESH_BAR_STRIP_RBAR_EDT, m_strBarStrip);
	DDX_Text(pDX, IDC_WG_DGN_MESH_BAR_STRIP_SPAC_EDT, m_strSpaceStrip);  
	DDX_Check(pDX, IDC_DGN_MESH_SLAB_BASIC_CHK, m_bBasicRebar);    
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_TOP_X_AREA_CMB , m_cmbTopXName );
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_BOT_X_AREA_CMB , m_cmbBotXName );  
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_TOP_Y_AREA_CMB , m_cmbTopYName );
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_BOT_Y_AREA_CMB , m_cmbBotYName );  
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_TOP_X_SPACE_CMB, m_cmbTopXSpace);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_BOT_X_SPACE_CMB, m_cmbBotXSpace);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_TOP_Y_SPACE_CMB, m_cmbTopYSpace);
	DDX_Control(pDX, IDC_DGN_MESH_SLAB_BAR_BOT_Y_SPACE_CMB, m_cmbBotYSpace);

	DDX_Control(pDX, IDC_DGN_CON_MATL_RBAR_DIAM_CHK, m_chkMatlByRbar);
	//}}AFX_DATA_MAP
}

BOOL CDgnMeshBarDlg::DestroyWindow() 
{
	if(IsAlivePictureDlg())
	{
		DestroyPictureDlg();
	}
	return CDialog::DestroyWindow();
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnMeshBarDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnMeshBarDlg)
	ON_BN_CLICKED(IDC_DGN_MESH_SLAB_BAR_BTN, OnDgnSlabbarSize)
	ON_BN_CLICKED(IDC_DGN_MESH_MAT_BAR_BTN, OnDgnMatbarSize)
	ON_BN_CLICKED(IDC_DGN_MESH_WALL_VER_BAR_BTN, OnDgnWallVerbarSize)
	ON_BN_CLICKED(IDC_DGN_MESH_WALL_HOR_BAR_BTN, OnDgnWallHorbarSize)
	ON_BN_CLICKED(IDC_WG_DGN_MESH_BAR_STRIP_RBAR_BTN, OnDgnStripbarSize)
	ON_BN_CLICKED(IDC_WG_DGN_SHELL_REBAR_BTN, OnDgnShellbarSize)
	ON_BN_CLICKED(IDC_DGN_MESH_SLAB_SPACE_BTN, OnDgnSlabbarSpace)
	ON_BN_CLICKED(IDC_DGN_MESH_MAT_SPACE_BTN, OnDgnMatbarSpace)
	ON_BN_CLICKED(IDC_DGN_MESH_WALL_SPACE_BTN, OnDgnWallbarSpace)
	ON_BN_CLICKED(IDC_WG_DGN_MESH_BAR_STRIP_SPAC_BTN, OnDgnStripbarSpace)
	ON_BN_CLICKED(IDC_WG_DGN_SHELL_SPACE_BTN, OnDgnShellbarSpace)
	ON_BN_CLICKED(IDC_DGN_MESH_SLAB_BASIC_CHK, OnDgnBasicRebarChk)
	ON_BN_CLICKED(IDC_WG_DGN_SHELL_THK_SAND_BTN, OnDgnShellThk4Sand)  
	ON_BN_CLICKED(IDC_DGN_CON_MATL_RBAR_DIAM_CHK, OnMatlRbarDiamCheck)
	ON_BN_CLICKED(IDC_DGN_CON_REBAR_MATL_BTN, OnMatlRbarDiamBtn)

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDgnMeshBarDlg::WM_AFTERINITDIALOG, OnAfterInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlg message handlers


void CDgnMeshBarDlg::OnOK() 
{
	UpdateData(TRUE);
	if(Check_ErrorMsg())
	{
		BOOL    bThickOptimal = m_Msrb.bThickOptimal; 
		double  dTopThik = m_Msrb.dTopThik;        
		double  dBotThik = m_Msrb.dBotThik;        
		int     nIteration = m_Msrb.nIteration;    
		double  dConvTolerance = m_Msrb.dConvTolerance;

        const int N_SIZE = 5;
        const int N_SPAC = 50;
       
        auto L_SetRebarInfo =[this] (const CString& strBarInfo, const INT_PTR nSizeArr, OUT CString aEachRebar[])
        {
            CArray<CString, CString&> astrBar;
            Get_RebarByString(strBarInfo, astrBar);
            if ( astrBar.GetSize() == 0 ) { ASSERT(0); }
            const INT_PTR nSize = min(astrBar.GetSize(), nSizeArr);
            for ( INT_PTR i=0; i<nSize; ++i )
            {
                aEachRebar[i] = astrBar.GetAt(i);
            }
        };

		CString strMatlName = m_Msrb.strMatlCode;
		CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&> aMatlByDia;
		if (m_Msrb.aMatlByDia.GetSize() > 0) aMatlByDia.Copy(m_Msrb.aMatlByDia);

		m_Msrb.Initialize();
		m_Msrb.strMatlCode = strMatlName;
		if (aMatlByDia.GetSize() > 0) m_Msrb.aMatlByDia.Copy(aMatlByDia);
		// Set Slab Rebar Size and Spacing Data.		
        L_SetRebarInfo(m_strBarSlab, N_SIZE, m_Msrb.RebarSlabSize);        
        L_SetRebarInfo(m_strSpaceSlab, N_SPAC, m_Msrb.RebarSlabSpace);
        
		// Set Mat Rebar Size and Spacing Data.		
        L_SetRebarInfo(m_strBarMat, N_SIZE, m_Msrb.RebarMatSize);
		L_SetRebarInfo(m_strSpaceMat, N_SPAC, m_Msrb.RebarMatSpace);
		
		// Set Wall Rebar Size and Spacing Data.		
        L_SetRebarInfo(m_strVerBarWall, N_SIZE, m_Msrb.RebarWallVerSize);
        L_SetRebarInfo(m_strHorBarWall, N_SIZE, m_Msrb.RebarWallHorSize);
        L_SetRebarInfo(m_strSpaceWall, N_SPAC, m_Msrb.RebarWallSpace);
	
		// Set Strip Rebar Size and Spacing Data.
        L_SetRebarInfo(m_strBarStrip, N_SIZE, m_Msrb.RebarStripSize);
        L_SetRebarInfo(m_strSpaceStrip, N_SPAC, m_Msrb.RebarStripSpace);

		// Set Shell Rebar Size and Spacing Data.
        L_SetRebarInfo(m_strBarShell, N_SIZE, m_Msrb.RebarShellSize);
        L_SetRebarInfo(m_strSpaceShell, N_SPAC, m_Msrb.RebarShellSpace);

		// Set Protection Thickness.
		m_Msrb.dB_Slab_X	= m_editXDbSlab.GetEditValue();
		m_Msrb.dT_Slab_X	= m_editXDtSlab.GetEditValue();
		m_Msrb.dB_Slab_Y	= m_editYDbSlab.GetEditValue();
		m_Msrb.dT_Slab_Y	= m_editYDtSlab.GetEditValue();

		m_Msrb.dB_Mat_X	= m_editXDbMat.GetEditValue();
		m_Msrb.dT_Mat_X	= m_editXDtMat.GetEditValue();
		m_Msrb.dB_Mat_Y	= m_editYDbMat.GetEditValue();
		m_Msrb.dT_Mat_Y	= m_editYDtMat.GetEditValue();

		m_Msrb.dB_Shell_X	= m_editXDbShell.GetEditValue();
		m_Msrb.dT_Shell_X	= m_editXDtShell.GetEditValue();
		m_Msrb.dB_Shell_Y	= m_editYDbShell.GetEditValue();
		m_Msrb.dT_Shell_Y	= m_editYDtShell.GetEditValue();

		m_Msrb.dW1	= m_editDwWall.GetEditValue();
		m_Msrb.dT_Strip	= m_editDtStrip.GetEditValue();
		m_Msrb.dB_Strip	= m_editDbStrip.GetEditValue();

		m_Msrb.bBasicRebar = m_bBasicRebar;
		m_cmbTopXName.GetWindowText(m_Msrb.strTopRebarName_X);
		m_cmbBotXName.GetWindowText(m_Msrb.strBotRebarName_X);
		m_cmbTopYName.GetWindowText(m_Msrb.strTopRebarName_Y);
		m_cmbBotYName.GetWindowText(m_Msrb.strBotRebarName_Y);
		
		CString strTemp;
		m_cmbTopXSpace.GetWindowText(strTemp);
		m_Msrb.dTopRebarSpace_X = ConvertSpaceString2Data(strTemp);
		m_cmbBotXSpace.GetWindowText(strTemp);
		m_Msrb.dBotRebarSpace_X = ConvertSpaceString2Data(strTemp);
		m_cmbTopYSpace.GetWindowText(strTemp);
		m_Msrb.dTopRebarSpace_Y = ConvertSpaceString2Data(strTemp);
		m_cmbBotYSpace.GetWindowText(strTemp);
		m_Msrb.dBotRebarSpace_Y = ConvertSpaceString2Data(strTemp);

		m_Msrb.bThickOptimal  = bThickOptimal;  
		m_Msrb.dTopThik       = dTopThik;        
		m_Msrb.dBotThik       = dBotThik;        
		m_Msrb.nIteration     = nIteration;    
		m_Msrb.dConvTolerance = dConvTolerance;

		m_Msrb.bMatlByDia = m_chkMatlByRbar.GetCheck();

		BOOL bChangeBasicRebar = FALSE;
		if(m_bSaveBasicRebar != m_bBasicRebar)
		{
			if(m_pDoc->m_pAttrCtrl->GetCountRbms() > 0)
			{
				if(IDCANCEL==AfxMessageBox(_LS(IDS_DGN_WARN_REMOVE_SLAB_REBAR), MB_OKCANCEL)) return;
			}
			bChangeBasicRebar = TRUE;      
		}
	
		// Save Data at DB.
	  if(m_pDoc->m_pDataCtrl->AddMsrb(m_Msrb, bChangeBasicRebar))	CDialogMove::OnOK();        
	}  
}

BOOL CDgnMeshBarDlg::Check_ErrorMsg()
{
	const int nChkSize = 11;
	BOOL bErrorExist[nChkSize] = {FALSE};
	if(m_editXDtSlab.GetEditValue() < 0.0)	bErrorExist[0] = TRUE;
	if(m_editXDbSlab.GetEditValue() < 0.0)	bErrorExist[1] = TRUE;
	if(m_editYDtSlab.GetEditValue() < 0.0)	bErrorExist[2] = TRUE;
	if(m_editYDbSlab.GetEditValue() < 0.0)	bErrorExist[3] = TRUE;
	if(m_editXDtMat.GetEditValue() < 0.0)	bErrorExist[4] = TRUE;
	if(m_editXDbMat.GetEditValue() < 0.0)	bErrorExist[5] = TRUE;
	if(m_editYDtMat.GetEditValue() < 0.0)	bErrorExist[6] = TRUE;
	if(m_editYDbMat.GetEditValue() < 0.0)	bErrorExist[7] = TRUE;
	if(m_editDwWall.GetEditValue() < 0.0)	bErrorExist[8] = TRUE;
	if(m_editDtStrip.GetEditValue() < 0.0)	bErrorExist[9] = TRUE;
	if(m_editDbStrip.GetEditValue() < 0.0)	bErrorExist[10] = TRUE;
	if(m_editXDtShell.GetEditValue() < 0.0)	bErrorExist[0] = TRUE;
	if(m_editXDbShell.GetEditValue() < 0.0)	bErrorExist[1] = TRUE;
	if(m_editYDtShell.GetEditValue() < 0.0)	bErrorExist[2] = TRUE;
	if(m_editYDbShell.GetEditValue() < 0.0)	bErrorExist[3] = TRUE;

	BOOL bExistMsg = FALSE;
	for(int i=0; i<nChkSize; i++)
	{
		if(bErrorExist[i])	bExistMsg = TRUE;
	}
	if(bExistMsg)
	{
		//AfxMessageBox(_LS(IDS_DGN_DLG_ERR_COVER));
		return FALSE;
	}
	else return TRUE;
}

BOOL CDgnMeshBarDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	SetCaption();

	AlignCtrl();

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_M:   
    case D_UNITSYS_LENGTH_INDEX_MM:
    case D_UNITSYS_LENGTH_INDEX_CM:
		m_bPrevUnitType = 0;
		break;
	case D_UNITSYS_LENGTH_INDEX_IN: 
    case D_UNITSYS_LENGTH_INDEX_FT:
		m_bPrevUnitType = 1;
		break;
	}

	Initial_UnitData();

	InitRebarComboBox();

	// Initialize Data.	
	m_Msrb.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMsrb(m_Msrb))	
		m_pDoc->m_pMatlDB->InitializeMsrb(m_Msrb);
	Initial_ExistData();
	UpdateData(FALSE);	
		
	m_bSaveBasicRebar = m_bBasicRebar;

	OnDgnBasicRebarChk();  
	if (CDBLib::IsCodeForMeshedDesignIS456())
	{
		//GetDlgItem(IDC_WG_DGN_MESH_BAR_STRIP_GRP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_VER_BAR_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_VER_BAR_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_HOR_BAR_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_HOR_BAR_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_SPACE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_SPACE_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MESH_WALL_DW1_EDIT)->EnableWindow(FALSE);
	}
	else if (CDBLib::IsCodeForMeshedDesignAIJ_JP())
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_DGN_STATIC29, FALSE, TRUE);
	}

	if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_WG_DGN_MESH_BAR_STRIP_CONC_GRP, FALSE, TRUE);
	}

	m_chkMatlByRbar.SetCheck(m_Msrb.bMatlByDia);
	GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->EnableWindow(m_Msrb.bMatlByDia);

	OnMatlRbarDiamCheck();

	this->PostMessage(WM_AFTERINITDIALOG);

	return TRUE;
}

void CDgnMeshBarDlg::SetCaption()
{
	if(CDBLib::IsConCodeForShell())
	{
		GetDlgItem(IDC_DGN_MESH_SLAB_BASIC_CHK)->SetWindowText(_LS(IDS_DGN_BASIC_RBAR_FOR_SLAB_MAT_SHELL));
	}
	else
	{
		GetDlgItem(IDC_DGN_MESH_SLAB_BASIC_CHK)->SetWindowText(_LS(IDS_DGN_BASIC_RBAR_FOR_SLAB_MAT));
	}
}

void CDgnMeshBarDlg::AlignCtrl()
{
	// 국문 버전에서는 Wall 대신 Strip 사용(iGen에서도 국문버전과 같게 함 - 2015-06-23 : JWKWON)
	CRect rRef;
	CRect rToMove;
	int dx=0;
	int dy=0;
	BOOL bShowStrip = FALSE;
#if defined(_ORG) || defined(_JP) || defined(_CH)
	GetDlgItem(IDC_WG_DGN_STATIC29)->GetWindowRect(rRef);  
	GetDlgItem(IDC_WG_DGN_MESH_BAR_STRIP_GRP)->GetWindowRect(rToMove);  
	dx = rRef.left - rToMove.left;
	dy = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aStripCtrls, dx, dy, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aWallCtrls, FALSE);
	bShowStrip = TRUE;
#endif

	GetDlgItem(IDC_WG_DGN_SHELL_DGN_STC)->GetWindowRect(rToMove);
	if(bShowStrip)
	{
		GetDlgItem(IDC_WG_DGN_MESH_BAR_STRIP_GRP)->GetWindowRect(rRef); // Strip
	}
	else
	{
		GetDlgItem(IDC_WG_DGN_STATIC29)->GetWindowRect(rRef); // Wall
	} 
	dy = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aShellCtrls, dy, FALSE);
	CDlgUtil::CtrlMoveDistY(this, m_aMatlCtrls, dy, FALSE);


	BOOL bShowShell = CDBLib::IsConCodeForShell();
	CDlgUtil::CtrlShowHide(this, m_aShellCtrls, bShowShell);
	
	if(bShowShell)
	{
		GetDlgItem(IDC_WG_DGN_SHELL_DGN_STC)->GetWindowRect(rRef); // Shell    
	}
	else if(bShowStrip)
	{
		GetDlgItem(IDC_WG_DGN_MESH_BAR_STRIP_GRP)->GetWindowRect(rRef); // Strip
	}
	else
	{
		GetDlgItem(IDC_WG_DGN_STATIC29)->GetWindowRect(rRef); // Wall
	} 

	// Material by Rebar Diameter for Slab/Mat 컨트롤을 위로 올린다.
	BOOL bShowMatl = FALSE;
#if defined(_IGEN) || defined(_DEBUG)
	bShowMatl = TRUE;
#endif
	CDlgUtil::CtrlShowHide(this, m_aMatlCtrls, bShowMatl);
	if (bShowMatl)
	{
		GetDlgItem(IDC_DGN_CON_MATL_RBAR_DIAM_CHK)->GetWindowRect(rToMove);
		dy = rRef.bottom - rToMove.top + 8;
		CDlgUtil::CtrlMoveDistY(this, m_aMatlCtrls, dy, FALSE);

		GetDlgItem(IDC_DGN_CON_MATL_RBAR_DIAM_CHK)->GetWindowRect(rRef);
	}

	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);

	dy = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, m_aOkCancelCtrls, dy, FALSE);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(10);
	MoveWindow(r);
}

void CDgnMeshBarDlg::Initial_ExistData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	CString strRebarCodeNa = rPref.DgnCode.ConcRebarMatlCode;

	int iRebarSeries= -1;
	CString strS = m_Msrb.RebarSlabSize[0].Left(1);
	CString strM = m_Msrb.RebarMatSize[0].Left(1);
	CString strVW = m_Msrb.RebarWallVerSize[0].Left(1);
	CString strHW = m_Msrb.RebarWallHorSize[0].Left(1);
	CString strSt = m_Msrb.RebarStripSize[0].Left(1);
	if(strSt==_T(""))
		strSt = strS;
	
	if(strS==_T("D") && strM==_T("D") && strVW==_T("D") && strHW==_T("D") && strSt==_T("D"))
	{
        if ( strRebarCodeNa== MATLCODE_CON_CNS || strRebarCodeNa== MATLCODE_CON_CNS560 || strRebarCodeNa == MATLCODE_CON_CNS560_18 )
        {
            iRebarSeries = EN_BARSERIES_TW;
        }
        else
        {
            iRebarSeries = EN_BARSERIES_KS;
        }
	}
	else if(strS==_T("#") && strM==_T("#") && strVW==_T("#") && strHW==_T("#") && strSt==_T("#"))	
	{
		iRebarSeries = EN_BARSERIES_ASTM;
	}
	else if(strS==_T("P") && strM==_T("P") && strVW==_T("P") && strHW==_T("P") && strSt==_T("P"))
	{
        if ( strRebarCodeNa==MATLCODE_CON_UNI )
        {
            iRebarSeries = EN_BARSERIES_UNI;
        }
        else
        {
            iRebarSeries = EN_BARSERIES_EN;
        }
	}
	else if(strS==_T("d") && strM==_T("d") && strVW==_T("d") && strHW==_T("d") && strSt==_T("d"))	
	{
		iRebarSeries = EN_BARSERIES_GB;
	}
	else if(strS==_T("H") && strM==_T("H") && strVW==_T("H") && strHW==_T("H") && strSt==_T("H"))	
	{
		iRebarSeries = EN_BARSERIES_SS;
	}
	else
	{
		strS = m_Msrb.RebarSlabSize[0].Right(1);
		strM = m_Msrb.RebarMatSize[0].Right(1);
		strVW = m_Msrb.RebarWallVerSize[0].Right(1);
		strHW = m_Msrb.RebarWallHorSize[0].Right(1);
		strSt = m_Msrb.RebarStripSize[0].Right(1);
		if(strS==_T("M") && strM==_T("M") && strVW==_T("M") && strHW==_T("M") && strSt==_T("M"))	
			iRebarSeries = EN_BARSERIES_CSA;
	  else ASSERT(0);
	}

	switch(iRebarSeries) 
	{
		//Rebar Series
		// 0 : _T("D") series
		// 1 : _T("#") series
		// 2 : _T("P") series
		// 3 : _T("d") series
		// 4 : _T("M") series
		// 5 : _T("P") series Itlay.
		// 6 : _T("D") series Taiwan.
	case EN_BARSERIES_KS:   InitRbar_KS();   break;
	case EN_BARSERIES_ASTM: InitRbar_ASTM(); break;
	case EN_BARSERIES_EN:   InitRbar_EN();   break;
	case EN_BARSERIES_GB:   InitRbar_GB();   break;
	case EN_BARSERIES_CSA:  InitRbar_CSA();  break;
	case EN_BARSERIES_UNI:  InitRbar_UNI();  break;
	case EN_BARSERIES_TW:   InitRbar_TW();   break;
	case EN_BARSERIES_SS:	InitRbar_SS();   break;
	default: ASSERT(FALSE); break;
	}/*END-switch*/

	m_editXDtSlab.SetEditUnit(m_Msrb.dT_Slab_X);
	m_editXDbSlab.SetEditUnit(m_Msrb.dB_Slab_X);
	m_editYDtSlab.SetEditUnit(m_Msrb.dT_Slab_Y);
	m_editYDbSlab.SetEditUnit(m_Msrb.dB_Slab_Y);
	m_editXDtMat.SetEditUnit(m_Msrb.dT_Mat_X);
	m_editXDbMat.SetEditUnit(m_Msrb.dB_Mat_X);
	m_editYDtMat.SetEditUnit(m_Msrb.dT_Mat_Y);
	m_editYDbMat.SetEditUnit(m_Msrb.dB_Mat_Y);
	m_editDwWall.SetEditUnit(m_Msrb.dW1);
	m_editDtStrip.SetEditUnit(m_Msrb.dT_Strip);
	m_editDbStrip.SetEditUnit(m_Msrb.dB_Strip);
	m_editXDtShell.SetEditUnit(m_Msrb.dT_Shell_X);
	m_editXDbShell.SetEditUnit(m_Msrb.dB_Shell_X);
	m_editYDtShell.SetEditUnit(m_Msrb.dT_Shell_Y);
	m_editYDbShell.SetEditUnit(m_Msrb.dB_Shell_Y);
	
	m_bBasicRebar = m_Msrb.bBasicRebar;
	if(m_bBasicRebar)
	{
		m_cmbTopXName.SelectString(0, m_Msrb.strTopRebarName_X);
		m_cmbBotXName.SelectString(0, m_Msrb.strBotRebarName_X);
		m_cmbTopYName.SelectString(0, m_Msrb.strTopRebarName_Y);
		m_cmbBotYName.SelectString(0, m_Msrb.strBotRebarName_Y);
		
		m_cmbTopXSpace.SetWindowText(ConvertSpaceData2String(m_Msrb.dTopRebarSpace_X));
		m_cmbBotXSpace.SetWindowText(ConvertSpaceData2String(m_Msrb.dBotRebarSpace_X));
		m_cmbTopYSpace.SetWindowText(ConvertSpaceData2String(m_Msrb.dTopRebarSpace_Y));
		m_cmbBotYSpace.SetWindowText(ConvertSpaceData2String(m_Msrb.dBotRebarSpace_Y));
	}

	UpdateData(FALSE);
}

void CDgnMeshBarDlg::Initial_UnitData()
{
	m_unitXDbSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitYDbSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitXDbMat.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitYDbMat.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_unitDwWall.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitMainStrip.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitXDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitYDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDtSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDbSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDtSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDbSlab.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDtMat.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDbMat.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDtMat.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDbMat.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDwWall.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDtStrip.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDbStrip.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDtShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDtShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
}

void CDgnMeshBarDlg::Init_DefaultData(const int iRebarSeries)
{
	m_editXDtSlab.SetEditUnit(0.0);
	m_editXDbSlab.SetEditUnit(0.0);
	m_editYDtSlab.SetEditUnit(0.0);
	m_editYDbSlab.SetEditUnit(0.0);
	m_editXDtMat.SetEditUnit(0.0);
	m_editXDbMat.SetEditUnit(0.0);
	m_editYDtMat.SetEditUnit(0.0);
	m_editYDbMat.SetEditUnit(0.0);
	m_editDwWall.SetEditUnit(0.0);
	m_editDtStrip.SetEditUnit(0.0);
	m_editDbStrip.SetEditUnit(0.0);
	m_editXDtShell.SetEditUnit(0.0);
	m_editXDbShell.SetEditUnit(0.0);
	m_editYDtShell.SetEditUnit(0.0);
	m_editYDbShell.SetEditUnit(0.0);
	switch(iRebarSeries)
	{
		case EN_BARSERIES_KS:
			m_strBarSlab	  = _T("D10,D13");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = _T("D22,D25,D29");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("D10,D13");
			m_strHorBarWall	= _T("D10,D13");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("D10,D13");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("D10,D13");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_ASTM:
			m_strBarSlab	  = _T("#4,#5");
			m_strSpaceSlab	= _T("@4,@6,@8,@12,@16");
			m_strBarMat		  = _T("#9,#10,#11");
			m_strSpaceMat		= _T("@4,@8");
			m_strVerBarWall	= _T("#4,#5");
			m_strHorBarWall	= _T("#4,#5");
			m_strSpaceWall	= _T("@4,@8");
			m_strBarStrip	  = _T("#4,#5");
			m_strSpaceStrip	= _T("@4,@6,@8,@12,@16");
			m_strBarShell	  = _T("#4,#5");
			m_strSpaceShell	= _T("@4,@6,@8,@12,@16");
			break;
		case EN_BARSERIES_EN:
			m_strBarSlab	  = _T("P10,P12");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = _T("P20,P25,P32");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("P10,P12");
			m_strHorBarWall	= _T("P10,P12");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("P10,P12");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("P10,P12");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_GB:
			m_strBarSlab	  = _T("d10,d12");
			m_strSpaceSlab	= _T("@100,@150,@200");
			m_strBarMat		  = _T("d22,d25,d28");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("d10,d12");
			m_strHorBarWall	= _T("d10,d12");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("d10,d12");
			m_strSpaceStrip	= _T("@100,@150,@200");
			m_strBarShell	  = _T("d10,d12");
			m_strSpaceShell	= _T("@100,@150,@200");
			break;
		case EN_BARSERIES_CSA:
			m_strBarSlab	  = _T("10M,15M");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = _T("20M,25M,30M");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("10M,15M");
			m_strHorBarWall	= _T("10M,15M");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("10M,15M");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("10M,15M");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_UNI://UNI(S,RC), CNS(S,RC), EN05(S), EN04(RC).
			m_strBarSlab	  = _T("P8,P10");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = _T("P20,P22,P24");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("P8,P10");
			m_strHorBarWall	= _T("P8,P10");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("P8,P10");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("P8,P10");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_TW:
			m_strBarSlab	  = _T("D10,D13");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = _T("D22,D25,D29");
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("D10,D13");
			m_strHorBarWall	= _T("D10,D13");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("D10,D13");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("D10,D13");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_SS:
			m_strBarSlab	  = _T("H10,H12");
			m_strSpaceSlab	= _T("@100,@150,@200,@300,@400");
			m_strBarMat		  = ")H20,H26,H32_T(";
			m_strSpaceMat		= _T("@100,@200");
			m_strVerBarWall	= _T("H10,H12");
			m_strHorBarWall	= _T("H10,H12");
			m_strSpaceWall	= _T("@100,@200");
			m_strBarStrip	  = _T("H10,H12");
			m_strSpaceStrip	= _T("@100,@150,@200,@300,@400");
			m_strBarShell	  = _T("H10,H12");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		default:
			ASSERT(FALSE);
			break;
	}

	m_bBasicRebar = TRUE;
	UpdateData(FALSE);	
}

BOOL CDgnMeshBarDlg::Get_RebarByString(CString strRebar, CArray<CString,CString&>& SelRebarList)
{
	TCHAR chTemp;
	CString strTemp = _T("");
	int iLen = strRebar.GetLength();
	for(int i=0; i<iLen; i++)
	{
		chTemp = strRebar.GetAt(i);
		if(chTemp==',' || i+1 == iLen)
		{
			if(chTemp==',') SelRebarList.Add(strTemp);
			else						SelRebarList.Add(strTemp + chTemp);
			strTemp = _T("");
		}
		else	strTemp = strTemp + chTemp;
	}
	if(SelRebarList.GetSize() > 0)	return TRUE;
	else							return FALSE;
}

CString CDgnMeshBarDlg::Add_RebarList(int iMax, CString* strRebar)
{
	CString strRebarData=_T("");
	for(int i=0; i<iMax; i++)
	{
		CString strTemp = strRebar[i];
		if(strTemp!=_T(""))	strRebarData = strRebarData + strTemp + _T(",");
	}
	int iLen = strRebarData.GetLength();
	if(iLen > 0)	return strRebarData.Left(iLen-1);
	else					return _T("");
}

CString CDgnMeshBarDlg::Add_RebarSizeList(CString* strRebar)
{
	return Add_RebarList(iConstRebarSizeMax, strRebar);
}

CString CDgnMeshBarDlg::Add_RebarSpaceList(CString* strRebar)
{
	return Add_RebarList(iConstRebarSpaceMax, strRebar);
}

void CDgnMeshBarDlg::OnDgnSlabbarSize() 
{
	UpdateData(TRUE);
	
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strBarSlab;
	if (dlg.DoModal() == IDOK)
	{
		m_strBarSlab = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnMatbarSize() 
{
	UpdateData(TRUE);
	
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strBarMat;
	if (dlg.DoModal() == IDOK)
	{
		m_strBarMat = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}	
}

void CDgnMeshBarDlg::OnDgnWallVerbarSize() 
{
	UpdateData(TRUE);
	
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strVerBarWall;
	if (dlg.DoModal() == IDOK)
	{
		m_strVerBarWall = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}	
}

void CDgnMeshBarDlg::OnDgnWallHorbarSize() 
{
	UpdateData(TRUE);
	
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strHorBarWall;
	if (dlg.DoModal() == IDOK)
	{
		m_strHorBarWall = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}	
}

void CDgnMeshBarDlg::OnDgnStripbarSize()
{
	UpdateData(TRUE);

	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strBarStrip;
	if (dlg.DoModal() == IDOK)
	{
		m_strBarStrip = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnShellbarSize() 
{
	UpdateData(TRUE);

	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strBarShell;
	if (dlg.DoModal() == IDOK)
	{
		m_strBarShell = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}	
}

void CDgnMeshBarDlg::OnDgnSlabbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceSlab;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceSlab = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnMatbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceMat;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceMat = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnWallbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceWall;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceWall = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnStripbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceStrip;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceStrip = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlg::OnDgnShellbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceShell;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceShell = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

LRESULT CDgnMeshBarDlg::OnAfterInitDialog(WPARAM wParam, LPARAM lParam)
{
	if(IsAlivePictureDlg()) return 0L;
	
	m_pPictureDlg = new CDgnSlabRebarPictureDlg();
	m_pPictureDlg->SetInitPos(D_INIT_POS_RT);
	m_pPictureDlg->Create(CDgnSlabRebarPictureDlg::IDD, this);
	m_pPictureDlg->ShowWindow(SW_SHOW);

	return 0L;
}

void CDgnMeshBarDlg::DestroyPictureDlg()
{
	// Window가 있으면 없앤다.
	if(m_pPictureDlg != NULL)
	{
		m_pPictureDlg->DestroyWindow();
		delete m_pPictureDlg;
		m_pPictureDlg = NULL;    
	}
}

BOOL CDgnMeshBarDlg::IsAlivePictureDlg()
{ 
	if(m_pPictureDlg == NULL) return FALSE;
	HWND hWnd = m_pPictureDlg->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;
	
	return TRUE;
}

void CDgnMeshBarDlg::OnDgnShellThk4Sand()
{
	CDgnMeshBarLThk4SandwichDlg Dlg;
	
	LTHIK_4_SANDWICH DlgData;
	DlgData.bThickOptimal  = m_Msrb.bThickOptimal ;
	DlgData.dTopThik       = m_Msrb.dTopThik      ;
	DlgData.dBotThik       = m_Msrb.dBotThik      ;
	DlgData.nIteration     = m_Msrb.nIteration    ;
	DlgData.dConvTolerance = m_Msrb.dConvTolerance;
	Dlg.SetData(DlgData);
	if(Dlg.DoModal() == IDOK)
	{    
		Dlg.GetData(DlgData);
		m_Msrb.bThickOptimal  = DlgData.bThickOptimal ;
		m_Msrb.dTopThik       = DlgData.dTopThik      ;
		m_Msrb.dBotThik       = DlgData.dBotThik      ;
		m_Msrb.nIteration     = DlgData.nIteration    ;
		m_Msrb.dConvTolerance = DlgData.dConvTolerance;
	}
}

void CDgnMeshBarDlg::OnDgnBasicRebarChk()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aBasicCtrls, m_bBasicRebar);
}

void CDgnMeshBarDlg::InitRebarComboBox()
{
	CStringArray arRebar;	arRebar.RemoveAll();
	CDBLib::GetRebarNameListByCode(arRebar);
	
	int nRebarSize = arRebar.GetSize();
	for(int i = 0; i < nRebarSize; i++)
	{
		m_cmbTopXName.InsertString(i, arRebar[i]);
		m_cmbBotXName.InsertString(i, arRebar[i]);
		m_cmbTopYName.InsertString(i, arRebar[i]);
		m_cmbBotYName.InsertString(i, arRebar[i]);
	}
	
	if(nRebarSize > 8) // default P12
	{
		m_cmbTopXName.SetCurSel(7);
		m_cmbBotXName.SetCurSel(7);
		m_cmbTopYName.SetCurSel(7);
		m_cmbBotYName.SetCurSel(7);
	}

	//////////////////////////////////////////////////////////////////////////
	
	m_cmbTopXSpace.ResetContent();
	m_cmbBotXSpace.ResetContent();
	m_cmbTopYSpace.ResetContent();
	m_cmbBotYSpace.ResetContent();
	
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
	
	nRebarSize = arItem.GetSize();
	for(int i = 0; i < nRebarSize; i++)
	{
		m_cmbTopXSpace.InsertString(i, arItem[i]);
		m_cmbBotXSpace.InsertString(i, arItem[i]);
		m_cmbTopYSpace.InsertString(i, arItem[i]);
		m_cmbBotYSpace.InsertString(i, arItem[i]);
	}

	m_cmbTopXSpace.SetCurSel(5);
	m_cmbBotXSpace.SetCurSel(5);
	m_cmbTopYSpace.SetCurSel(5);
	m_cmbBotYSpace.SetCurSel(5);
}

CString CDgnMeshBarDlg::ConvertSpaceData2String(double dSpace)
{
	CString strSpace;
	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		int nSpace = (int)m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%d"), nSpace);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dTempSpace = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%f"), dTempSpace);
	}
	else      ASSERT(0);
	
	return strSpace;
}

double CDgnMeshBarDlg::ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange/*=FALSE*/)
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
			dSpace = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0,D_UNITSYS_LENGTH_INDEX_MM,0,D_UNITSYS_LENGTH_INDEX_IN,
			D_UNITSYS_BASE_LENGTH,_tstof(strSpace));
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:   case D_UNITSYS_LENGTH_INDEX_FT:
		strSpace.Remove('\\');
		if(m_bPrevUnitType == 0 && bUnitChange)
			dSpace = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0,D_UNITSYS_LENGTH_INDEX_IN,0,D_UNITSYS_LENGTH_INDEX_MM,
			D_UNITSYS_BASE_LENGTH,_tstof(strSpace));
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, IN, D_UNITSYS_BASE_LENGTH);
		break;
	default:    ASSERT(0);    break;
	}
	
	return dSpace;
}

void CDgnMeshBarDlg::OnMatlRbarDiamCheck()
{
	BOOL bChkMatlByRbar = m_chkMatlByRbar.GetCheck();
	GetDlgItem(IDC_DGN_CON_REBAR_MATL_BTN)->EnableWindow(m_chkMatlByRbar.GetCheck());
}

void CDgnMeshBarDlg::OnMatlRbarDiamBtn()
{
	CDgnMeshBarMatlDlg dlg(&m_Msrb);
	dlg.DoModal();
}

void CDgnMeshBarDlg::InitRbar_KS()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("D10,D13"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400")); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("D22,D25,D29")); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("D10,D13"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("D10,D13"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("D10,D13"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400")); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("D10,D13"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400")); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_ASTM()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("#4,#5"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@4,@6,@8,@12,@16")); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("#9,#10,#11")); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@4,@8"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("#4,#5") );     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("#4,#5") );     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@4,@8"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("#4,#5") );     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@4,@6,@8,@12,@16") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("#4,#5"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@4,@6,@8,@12,@16")); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_EN()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("P10,P12"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400") ); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("P20,P25,P32")); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("P10,P12"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("P10,P12"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("P10,P12"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("P10,P12"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400") ); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_GB()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("d10,d12"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200")); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("d22,d25,d28") ); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("d10,d12"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("d10,d12"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("d10,d12"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200")); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("d10,d12"));        // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200")); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_CSA()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("10M,15M"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400") ); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("20M,25M,30M") ); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("10M,15M"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("10M,15M"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("10M,15M"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("10M,15M"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400") ); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_UNI()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("P8,P10"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400") ); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("P20,P22,P24") ); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("P8,P10"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("P8,P10"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("P8,P10"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("P8,P10"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400") ); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_TW()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("D10,D13") );	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400") ); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("D22,D25,D29") ); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("D22,D25,D29") ); // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("D22,D25,D29") ); // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("D10,D13") );     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("D10,D13") );     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400") ); // Get Shell Rebar Spacing Data.
}

void CDgnMeshBarDlg::InitRbar_SS()
{
	m_strBarSlab    = GetRebarSize(m_Msrb.RebarSlabSize,    _T("H10,H12"));	
	m_strSpaceSlab  = GetRebarSpac(m_Msrb.RebarSlabSpace,   _T("@100,@150,@200,@300,@400") ); // Get Slab Rebar Spacing Data.	
	m_strBarMat     = GetRebarSize(m_Msrb.RebarMatSize,     _T("H20,H25,H32") ); // Get Mat Rebar Size Data.
	m_strSpaceMat   = GetRebarSpac(m_Msrb.RebarMatSpace,    _T("@100,@200"));   // Get Mat Rebar Spacing Data.
	m_strVerBarWall = GetRebarSize(m_Msrb.RebarWallVerSize, _T("H10,H12"));     // Get Wall Ver Rebar Size Data.
	m_strHorBarWall = GetRebarSize(m_Msrb.RebarWallHorSize, _T("H10,H12"));     // Get Wall Hor Rebar Size Data.
	m_strSpaceWall  = GetRebarSpac(m_Msrb.RebarWallSpace,   _T("@100,@200"));   // Get Wall Rebar Spacing Data.
	m_strBarStrip   = GetRebarSize(m_Msrb.RebarStripSize,   _T("H10,H12"));     // Get Strip Rebar Size Data.
	m_strSpaceStrip = GetRebarSpac(m_Msrb.RebarStripSpace,  _T("@100,@150,@200,@300,@400") ); // Get Strip Rebar Spacing Data.
	m_strBarShell   = GetRebarSize(m_Msrb.RebarShellSize,   _T("H10,H12"));     // Get Shell Rebar Size Data.
	m_strSpaceShell = GetRebarSpac(m_Msrb.RebarShellSpace,  _T("@100,@150,@200,@300,@400") ); // Get Shell Rebar Spacing Data.		
}

CString CDgnMeshBarDlg::GetRebarSize(CString astrBarUser[], const CString& strBarDflt)
{
	return astrBarUser[0]==_T("") ? strBarDflt : Add_RebarSizeList(astrBarUser);
}

CString CDgnMeshBarDlg::GetRebarSpac(CString astrBarUser[], const CString& strBarDflt)
{
	return astrBarUser[0]==_T("") ? strBarDflt : Add_RebarSpaceList(astrBarUser);
}