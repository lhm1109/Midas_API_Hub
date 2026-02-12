// CMSectItemTap.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemTap.h"
#include "CMSectItemImportPSC.h"
#include "CMSectOffsetDlg.h"
#include "CMConcrSteelMatlDlg.h"
#include "CMCompoMatDlg.h" // added by mylee 070221
#include "CMSectItemPSC_WarpingCheckTapDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\CMSectImportSPC.h"
#include "..\wg_db\SectDBRussiaUtil.h"

#include "CMSecPageStiffDlg.h"
#include "CCMSectItemPSCDefine.h"
#include "CMSectItemPSCImport.h"
#include "CMSectItemStlgView.h"

// Default Tapered Section Shape
#define D_SECT_TAPERED_DEF_SHAPE D_SECT_SHAPE_REG_H

#define D_SECT_PSC_SHAPE_INDEX 100  // DB/USER Section Shape의 갯수 보다 큰 수이면 된다.
#define D_SECT_COMPO_SHAPE_INDEX 150
#define D_SECT_STLG_SHAPE_INDEX 200

#define D_DEF_CALC_STIFF_OPT 1  // nCalcStiffOpt

#define D_SECT_TP_STLG_MCEL_NO_MIN 0
#define D_SECT_TP_STLG_MCEL_NO_MAX 10

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTap dialog
#define   CDialog   CCMSectItemBase

namespace SectItemTapResource
{
#if defined(_MGEN) || defined(_TOWER)
	UINT aBitmapID[] = {
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10, IDB_CMD_SEC_REG_24,
	};
	CString aImgPath[] = {
		_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon24.svg"),
	};
	DWORD nIndex[] = {
		0,  1,  2,
		3,  4,  5,
		6,  7,  8,
		9,  21,
	};
#elif defined(_CIVIL)
	UINT aBitmapID[] = {
		IDB_CMD_SEC_REG_01, IDB_CMD_SEC_REG_02, IDB_CMD_SEC_REG_03,
		IDB_CMD_SEC_REG_04, IDB_CMD_SEC_REG_05, IDB_CMD_SEC_REG_06,
		IDB_CMD_SEC_REG_07, IDB_CMD_SEC_REG_08, IDB_CMD_SEC_REG_09,
		IDB_CMD_SEC_REG_10,
		IDB_CMD_SEC_REG_13, IDB_CMD_SEC_REG_14, /*IDB_CMD_SEC_REG_21,*/
		IDB_CMD_SEC_REG_15, IDB_CMD_SEC_REG_16, IDB_CMD_SEC_REG_17,
		IDB_CMD_SEC_REG_24,
		IDB_CMD_SEC_PSC_1C, IDB_CMD_SEC_PSC_2C, IDB_CMD_SEC_PSC_3C,
		IDB_CMD_SEC_PSC_9C, IDB_CMD_SEC_PSC_10C, IDB_CMD_SEC_PSC_4C,
		IDB_CMD_SEC_PSC_8C, IDB_CMD_SEC_PSC_5C, IDB_CMD_SEC_PSC_6C,
		IDB_CMD_SEC_PSC_7C, IDB_CMD_SEC_PSC_11C, IDB_CMD_SEC_PSC_12C,
		IDB_CMD_SEC_TCOMP_01, IDB_CMD_SEC_TCOMP_02, IDB_CMD_SEC_TCOMP_05,
		IDB_CMD_SEC_TCOMP_06, IDB_CMD_SEC_TCOMP_07, IDB_CMD_SEC_TCOMP_08,
		IDB_CMD_SEC_TCOMP_03, IDB_CMD_SEC_TCOMP_04, IDB_CMD_SEC_REG_24  ,
		IDB_CMD_SEC_TSTLG_01, IDB_CMD_SEC_TSTLG_02, IDB_CMD_SEC_TSTLG_03
	};
	CString aImgPath[] = {
		_T("SVG\\illustration\\ComboBox\\SecRegIcon01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon03.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon04.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon05.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon06.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon07.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon08.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon09.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon10.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon13.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon14.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon15.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon16.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon17.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon24.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc1c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc2c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc3c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc9c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc10c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc4c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc8c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc5c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc6c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc7c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc11c.svg"),
		_T("SVG\\illustration\\ComboBox\\cmd_sec_psc12c.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon05.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon06.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon07.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon08.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon03.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTCompIcon04.svg"),
		_T("SVG\\illustration\\ComboBox\\SecRegIcon24.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTStlg01.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTStlg02.svg"),
		_T("SVG\\illustration\\ComboBox\\SecTStlg03.svg"),
	};
	DWORD nIndex[] = {
		0,  1,  2,
		3,  4,  5,
		6,  7,  8,
		9,
		12, 13, /*18,*/
		14, 15, 16,
		21,
		D_SECT_PSC_SHAPE_INDEX, D_SECT_PSC_SHAPE_INDEX+1, D_SECT_PSC_SHAPE_INDEX+2,
		D_SECT_PSC_SHAPE_INDEX+8, D_SECT_PSC_SHAPE_INDEX+9, D_SECT_PSC_SHAPE_INDEX+3,
		D_SECT_PSC_SHAPE_INDEX+7, D_SECT_PSC_SHAPE_INDEX+4, D_SECT_PSC_SHAPE_INDEX+5,
		D_SECT_PSC_SHAPE_INDEX+6, D_SECT_PSC_SHAPE_INDEX+10, D_SECT_PSC_SHAPE_INDEX+11,
		D_SECT_COMPO_SHAPE_INDEX, D_SECT_COMPO_SHAPE_INDEX+1, D_SECT_COMPO_SHAPE_INDEX+6,
		D_SECT_COMPO_SHAPE_INDEX+7, D_SECT_COMPO_SHAPE_INDEX+8, D_SECT_COMPO_SHAPE_INDEX+9,
		D_SECT_COMPO_SHAPE_INDEX+2, D_SECT_COMPO_SHAPE_INDEX+3, D_SECT_COMPO_SHAPE_INDEX+5,
		D_SECT_STLG_SHAPE_INDEX, D_SECT_STLG_SHAPE_INDEX+1, D_SECT_STLG_SHAPE_INDEX+2
	};
#else
#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif

}
CCMSectItemTap::CCMSectItemTap(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemTap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemTap)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBitmap = NULL;
	m_pImageList = NULL;
	m_pViewer = NULL;
	m_pStlgViewer= NULL;

	m_aCtrlDB.Add(IDC_CMD_SP_ID_VALUE_RADIO); // *^^* Value Section
	m_aCtrlDB.Add(IDC_CMD_SP_ID_USER_RADIO);
	m_aCtrlDB.Add(IDC_CMD_SP_ID_DB_RADIO);

	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_NAME_I_TITLE);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_CBO_NAME1);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE11);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE11);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT11);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE12);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE12);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT12);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE13);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE13);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT13);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE14);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE14);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT14);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE15);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE15);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT15);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE16);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE16);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT16);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE17);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE17);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT17);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE18);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE18);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT18);

	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_NAME_J_TITLE);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_CBO_NAME2);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE21);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE21);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT21);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE22);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE22);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT22);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE23);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE23);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT23);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE24);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE24);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT24);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE25);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE25);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT25);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE26);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE26);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT26);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE27);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE27);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT27);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_TITLE28);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_VALUE28);
	m_aCtrlUserDB.Add(IDC_CMD_SP_ID_UNIT28);

	m_aCtrlValue.Add(IDC_CMD_SP_ID_GRID_I);
	m_aCtrlValue.Add(IDC_CMD_SP_ID_GRID_J);

	m_aCtrlFrame.Add(IDC_CMD_SP_ID_FRAME_I);
	m_aCtrlFrame.Add(IDC_CMD_SP_ID_FRAME_J);

	m_aCtrlValuePSC.Add(IDC_CMD_SP_ID_FRAME_PSC);
	m_aCtrlValuePSC.Add(IDC_CMD_SP_ID_GRID_PSC);

	m_aCtrlROctagon.Add(IDC_CMD_TEXT_CELL_NUM);
	m_aCtrlROctagon.Add(IDC_CMD_EDIT_CELL_NUM);
	m_aCtrlROctagon.Add(IDC_CMD_SPIN_CELL_NUM);

	m_aCtrlMeshSize.Add(IDC_CMD_MESH_SIZE_CHK2);
	m_aCtrlMeshSize.Add(IDC_CMD_MESH_SIZE_EDIT2);
	m_aCtrlMeshSize.Add(IDC_CMD_MESH_SIZE_UNIT);

	m_aCtrlMaterial.Add(IDC_CMD_MATL_FRM);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_BTN);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MTITLE1);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MVALUE1);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MTITLE2);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MVALUE2);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MTITLE3);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MVALUE3);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MTITLE4);
	m_aCtrlMaterial.Add(IDC_CMD_MATL_MVALUE4);

	m_aCtrlOption.Add(IDC_CMD_FEM_RDO);
	m_aCtrlOption.Add(IDC_CMD_EQU_RDO);

	// tapered composite 단면용 추가
	m_aCtrlComposite.Add(IDC_CMD_SP_ID_FRAME_COMPO);
	m_aCtrlComposite.Add(IDC_CMD_SP_ID_GRID_COMPO);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_GRP);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_BTN);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC1);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT1);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC2);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT2);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_CHK);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC3);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT3);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC4);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT4);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC5);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT5);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC6);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT6);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_STC7);
	m_aCtrlComposite.Add(IDC_CMD_MATL2_EDT7);

	// material-multiple CI, CT 일때 Disable 시키기 위해
	m_aCtrlCompositeMulti.Add(IDC_CMD_MATL2_CHK);
	m_aCtrlCompositeMulti.Add(IDC_CMD_MATL2_STC3);
	m_aCtrlCompositeMulti.Add(IDC_CMD_MATL2_EDT3);
	m_aCtrlCompositeMulti.Add(IDC_CMD_MATL2_STC4);
	m_aCtrlCompositeMulti.Add(IDC_CMD_MATL2_EDT4);

	m_aCtrlComposite2.Add(IDC_CMD_SP_CG_PART_STC);
	m_aCtrlComposite2.Add(IDC_CMD_SP_CG_PART_CMB);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_BTN_I);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_STC_I);
	m_aCtrlComposite2.Add(IDC_CMD_CALC_SECT_PROP_I_BTN);
	m_aCtrlComposite2.Add(IDC_CMD_CALC_SECT_PROP_J_BTN);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_GRID_I2);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_BTN_J);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_STC_J);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_GRID_J2);
	m_aCtrlComposite2.Add(IDC_GENERAL_PSC_DESIGN_CHK);	
	m_aCtrlComposite2.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_STC);
	m_aCtrlComposite2.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_I_EDT);
	m_aCtrlComposite2.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_J_EDT);
	m_aCtrlComposite2.Add(IDC_GENERAL_PSC_DESIGN_SHEAR_UNT);
	m_aCtrlComposite2.Add(IDC_CMD_SP_CG_BEFOREPART_STC);
	m_aCtrlComposite2.Add(IDC_CMD_SP_CG_BEFOREPART_CMB);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_FRAME_I2);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_FRAME_J2);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_PREVIEW_CGI);
	m_aCtrlComposite2.Add(IDC_CMD_SP_ID_PREVIEW_CGJ);

	m_aCtrlView.Add(IDC_CMD_SP_ID_BMP);
	m_aCtrlView.Add(IDC_CMD_SP_ID_PREVIEW);

	m_aCtrlStlgBoxI.Add(IDC_CMD_SP_ID_FRAME_STLG);
	m_aCtrlStlgBoxI.Add(IDC_CMD_SP_ID_GRID_STLG);

	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_SYM_GRP);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_SYM_CHK);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_NO_CELL_STC);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_NO_CELL_EDT);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_NO_CELL_SPN);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_LEFT_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_RIGHT_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_FSHAPE_GRP);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_GEN_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_REBAR1_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_REBAR2_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_CLOSED_RDO);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_J1_CHK);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_J2_CHK);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_JR1_CHK);
	m_aCtrlStlgMCell.Add(IDC_CMD_SECT_MCELL_JR2_CHK);

	// Import Flag 는 모두 FALSE
	memset(&m_bIsCGImport, 0, sizeof(m_bIsCGImport));

	m_bReCalc = FALSE;
	m_nWarpingCheck= 0;
}

CCMSectItemTap::~CCMSectItemTap()
{
	if(m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList = NULL; }
}

void CCMSectItemTap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemTap)
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_UNIT, m_unitMeshSize);
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_EDIT2, m_editMeshSize);
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_CHK2, m_chkMeshSize);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_EDIT_CELL_NUM, m_editCellNum);
	DDX_Control(pDX, IDC_CMD_SP_ID_VAR1, m_wndVar1);
	DDX_Control(pDX, IDC_CMD_SP_ID_VAR2, m_wndVar2);
	DDX_Control(pDX, IDC_CMD_SP_ID_CODE, m_wndDB);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME2, m_wndSecondName);
	DDX_Control(pDX, IDC_CMD_SP_ID_CBO_NAME1, m_wndFirstName);
	DDX_Control(pDX, IDC_CMD_SP_ID_TYPE, m_cboType);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK, m_chkHumbly);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK_BF, m_chkHumblyBefore);
	DDX_Control(pDX, IDC_CMD_HUMBLY_CHK_AF, m_chkHumblyAfter);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_CHK, m_chkPSCDesign);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_SHEAR_I_EDT, m_edtPSCShear_I);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_SHEAR_J_EDT, m_edtPSCShear_J);
	DDX_Control(pDX, IDC_GENERAL_PSC_DESIGN_SHEAR_UNT,   m_untPSCShear);
	DDX_Control(pDX, IDC_CMD_MATL2_EDT1, m_wndMatlValueCompo[0]); // added by mylee 20070221
	DDX_Control(pDX, IDC_CMD_MATL2_EDT2, m_wndMatlValueCompo[1]);
	DDX_Control(pDX, IDC_CMD_MATL2_EDT3, m_edtMat1); 
	DDX_Control(pDX, IDC_CMD_MATL2_EDT4, m_edtMat2);
	DDX_Control(pDX, IDC_CMD_MATL2_EDT5, m_edtMatPs); 
	DDX_Control(pDX, IDC_CMD_MATL2_EDT6, m_edtMatPc);
	DDX_Control(pDX, IDC_CMD_MATL2_EDT7, m_edtMatTstc);
	DDX_Control(pDX, IDC_CMD_MATL2_CHK, m_chkMat);
	DDX_Control(pDX, IDC_CMD_SP_CG_PART_CMB, m_cmbPart); // MNET:3022 20080103 mylee
	DDX_Control(pDX, IDC_CMD_SP_CG_BEFOREPART_CMB, m_cmbBeforePart);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_SYM_CHK     , m_chkMCellSym);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_NO_CELL_SPN , m_spnMCellNoCell);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_NO_CELL_EDT , m_edtMCellNoCell);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_J1_CHK      , m_chkMCellJ1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_J2_CHK      , m_chkMCellJ2);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_JR1_CHK     , m_chkMCellJr1);
	DDX_Control(pDX, IDC_CMD_SECT_MCELL_JR2_CHK     , m_chkMCellJr2);
	DDX_Control(pDX, IDC_CMD_SP_ID_BMP, m_ImgSectItem);


	//}}AFX_DATA_MAP

	UINT aFirstID[][8] = {
		{ IDC_CMD_SP_ID_TITLE11, IDC_CMD_SP_ID_TITLE12, IDC_CMD_SP_ID_TITLE13,
		IDC_CMD_SP_ID_TITLE14, IDC_CMD_SP_ID_TITLE15, IDC_CMD_SP_ID_TITLE16,
		IDC_CMD_SP_ID_TITLE17, IDC_CMD_SP_ID_TITLE18 },
		{ IDC_CMD_SP_ID_VALUE11, IDC_CMD_SP_ID_VALUE12, IDC_CMD_SP_ID_VALUE13,
		IDC_CMD_SP_ID_VALUE14, IDC_CMD_SP_ID_VALUE15, IDC_CMD_SP_ID_VALUE16,
		IDC_CMD_SP_ID_VALUE17, IDC_CMD_SP_ID_VALUE18},
		{ IDC_CMD_SP_ID_UNIT11,  IDC_CMD_SP_ID_UNIT12,  IDC_CMD_SP_ID_UNIT13 ,
		IDC_CMD_SP_ID_UNIT14,  IDC_CMD_SP_ID_UNIT15,  IDC_CMD_SP_ID_UNIT16,
		IDC_CMD_SP_ID_UNIT17,  IDC_CMD_SP_ID_UNIT18 },
	};
	UINT aSecondID[][8] = {
		{ IDC_CMD_SP_ID_TITLE21, IDC_CMD_SP_ID_TITLE22, IDC_CMD_SP_ID_TITLE23, 
		IDC_CMD_SP_ID_TITLE24, IDC_CMD_SP_ID_TITLE25, IDC_CMD_SP_ID_TITLE26,
		IDC_CMD_SP_ID_TITLE27, IDC_CMD_SP_ID_TITLE28 },
		{ IDC_CMD_SP_ID_VALUE21, IDC_CMD_SP_ID_VALUE22, IDC_CMD_SP_ID_VALUE23, 
		IDC_CMD_SP_ID_VALUE24, IDC_CMD_SP_ID_VALUE25, IDC_CMD_SP_ID_VALUE26,
		IDC_CMD_SP_ID_VALUE27, IDC_CMD_SP_ID_VALUE28},
		{ IDC_CMD_SP_ID_UNIT21,  IDC_CMD_SP_ID_UNIT22,  IDC_CMD_SP_ID_UNIT23,  
		IDC_CMD_SP_ID_UNIT24,  IDC_CMD_SP_ID_UNIT25,  IDC_CMD_SP_ID_UNIT26,
		IDC_CMD_SP_ID_UNIT27,  IDC_CMD_SP_ID_UNIT28 },
	};
	for (int i = 0; i < 8; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndFirstTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndFirstSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndFirstUnit[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		DDX_Control(pDX, aSecondID[0][i], m_wndSecondTitle[i]);
		DDX_Control(pDX, aSecondID[1][i], m_wndSecondSize[i]);
		DDX_Control(pDX, aSecondID[2][i], m_wndSecondUnit[i]);
	}

	UINT aMatlID[4] = { IDC_CMD_MATL_MVALUE1, IDC_CMD_MATL_MVALUE2, IDC_CMD_MATL_MVALUE3, IDC_CMD_MATL_MVALUE4 };
	for (int i = 0; i < 4; i++)
	{
		DDX_Control(pDX, aMatlID[i], m_wndMatlValue[i]);
	}
	//DDX_Control(pDX, IDC_CMD_MATL2_EDT1, m_wndMatlValueCompo[0]);
	//DDX_Control(pDX, IDC_CMD_MATL2_EDT2, m_wndMatlValueCompo[1]);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_I, m_wndGridFirst);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_J, m_wndGridSecond);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_I2, m_wndGridComGenI);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_J2, m_wndGridComGenJ);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_PSC, m_wndGrid4Psc);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_COMPO, m_wndGrid4Composite);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID_STLG, m_wndGrid4StlG);
}

/////////////////////////////////////////////////////////////////////////////
// Interfaces
/////////////////////////////////////////////////////////////////////////////

// 부모 Page에서 OK 버튼 눌러졌을 때 호출

BOOL CCMSectItemTap::OnOKPublic()
{
	return SaveData();
}

// 부모 페이지에서 Apply 버튼 눌러졌을 때 호출

BOOL CCMSectItemTap::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);

		ShowKey2Dlg();
		m_wndFirstName.SetFocus();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

// 강성 계산 결과를 보여준다.
void CCMSectItemTap::ShowCalcResult()
{
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> aUserStiff, aOrgStiff;
	aUserStiff.SetSize(2);
	aOrgStiff.SetSize(2);
	BOOL bUserInput = m_Data.SectBefore.bUserInputStiffness;
	if (bUserInput) 
	{
		aUserStiff[0] = m_Data.SectBefore.SectI.Stiffness;
		aUserStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
	}

	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	m_Data.SectBefore.bUserInputStiffness = bUserInput;
	aOrgStiff[0] = m_Data.SectBefore.SectI.Stiffness;
	aOrgStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
	if (bUserInput) 
	{
		m_Data.SectBefore.SectI.Stiffness = aUserStiff[0];
		m_Data.SectBefore.SectJ.Stiffness = aUserStiff[1];
	}

	CCMStiffDlg dlg(m_pDoc, &m_Data, &aOrgStiff, this);
	dlg.DoModal();
}

void CCMSectItemTap::DisplayOffsetPoint()
{
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC)
		OnCmdSectCentroidBtn();
	else
	{
		m_wndSecView.Invalidate();	
		ReDrawStlgViewer();
	}
}

void CCMSectItemTap::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

void CCMSectItemTap::ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData)
{
	// add code here
	T_SECT_SECTION_D* pData = &(m_Data.SectBefore);
	pData->Matl_Elast = SteelData.Elast/ConcrData.Elast;
	pData->Matl_Density = SteelData.Density/ConcrData.Density;
	pData->Matl_Poisson_S = SteelData.Poisson;
	pData->Matl_Poisson_C = ConcrData.Poisson;
	pData->Matl_Thermal = SteelData.Thermal/ConcrData.Thermal;
	SetMatlSectData();
}

// 하위 대화창에서 호출해서 값 바꾸기 위함
void CCMSectItemTap::ChangeMatlDataCICT(T_MATL_CONCRETE &ConcreteDataSlab, T_MATL_CONCRETE &ConcreteDataGirder)
{
	// add code here
	T_SECT_SECTION_D* pData = &(m_Data.SectBefore);
	pData->Matl_Elast = ConcreteDataGirder.Elast/ConcreteDataSlab.Elast;
	pData->Matl_Density = ConcreteDataGirder.Density/ConcreteDataSlab.Density;
	pData->Matl_Poisson_S = ConcreteDataGirder.Poisson;
	pData->Matl_Poisson_C = ConcreteDataSlab.Poisson;
	pData->Matl_Thermal = ConcreteDataGirder.Thermal/ConcreteDataSlab.Thermal;
	SetMatlSectData();
}

// Material Data 를 대화창으로
void CCMSectItemTap::SetMatlSectData()
{
	if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT)
	{
		m_wndMatlValueCompo[0].SetEditUnit(m_Data.SectBefore.Matl_Elast);
		m_wndMatlValueCompo[1].SetEditUnit(m_Data.SectBefore.Matl_Density);
		m_edtMatPs.SetEditUnit(m_Data.SectBefore.Matl_Poisson_S);
		m_edtMatPc.SetEditUnit(m_Data.SectBefore.Matl_Poisson_C);
		m_edtMatTstc.SetEditUnit(m_Data.SectBefore.Matl_Thermal);
	}
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I       || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B  ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		m_wndMatlValueCompo[0].SetEditUnit(m_Data.SectBefore.Matl_Elast);
		m_wndMatlValueCompo[1].SetEditUnit(m_Data.SectBefore.Matl_Density);
		m_edtMatPs.SetEditUnit(m_Data.SectBefore.Matl_Poisson_S);
		m_edtMatPc.SetEditUnit(m_Data.SectBefore.Matl_Poisson_C);
		m_edtMatTstc.SetEditUnit(m_Data.SectBefore.Matl_Thermal);
		m_chkMat.SetCheck(m_Data.SectBefore.bMultipleElastic);

		if (m_Data.SectBefore.bMultipleElastic)
		{
			m_edtMat1.SetEditUnit(m_Data.SectBefore.dLongTermESEC);
			m_edtMat2.SetEditUnit(m_Data.SectBefore.dShrinkageESEC);
		}
	}
	else
	{
		m_wndMatlValue[0].SetEditUnit(m_Data.SectBefore.Matl_Elast);
		m_wndMatlValue[1].SetEditUnit(m_Data.SectBefore.Matl_Density);
		m_wndMatlValue[2].SetEditUnit(m_Data.SectBefore.Matl_Poisson_S);
		m_wndMatlValue[3].SetEditUnit(m_Data.SectBefore.Matl_Poisson_C);  
	}
}

void CCMSectItemTap::GetMatlSectData(T_SECT_D* pData)
{
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I       || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B  ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		pData->SectBefore.Matl_Elast     = m_wndMatlValueCompo[0].GetEditValue();
		pData->SectBefore.Matl_Density   = m_wndMatlValueCompo[1].GetEditValue();
		pData->SectBefore.Matl_Poisson_S = m_edtMatPs.GetEditValue();
		pData->SectBefore.Matl_Poisson_C = m_edtMatPc.GetEditValue();
		pData->SectBefore.Matl_Thermal   = m_edtMatTstc.GetEditValue();
	}
	else
	{
		pData->SectBefore.Matl_Elast     = m_wndMatlValue[0].GetEditValue();
		pData->SectBefore.Matl_Density   = m_wndMatlValue[1].GetEditValue();
		pData->SectBefore.Matl_Poisson_S = m_wndMatlValue[2].GetEditValue();
		pData->SectBefore.Matl_Poisson_C = m_wndMatlValue[3].GetEditValue();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemTap::SaveData()
{  
	int nShapeIndex = GetCurShapeIndex();

	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	T_SECT_STIFFNESS StiffBackup[2];
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC && 
		m_Data.SectBefore.bUserInputStiffness)
	{
		StiffBackup[0] = m_Data.SectBefore.SectI.Stiffness;
		StiffBackup[1] = m_Data.SectBefore.SectJ.Stiffness;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC &&
		m_Data.SectBefore.bUserInputStiffness)
	{
		T_SECT_STIFFNESS* pStiff;
		pStiff = &m_Data.SectBefore.SectI.Stiffness;
		pStiff->Area  = StiffBackup[0].Area; 
		pStiff->WArea = StiffBackup[0].WArea; 
		pStiff->Asy   = StiffBackup[0].Asy; 
		pStiff->Asz   = StiffBackup[0].Asz; 
		pStiff->Rxx   = StiffBackup[0].Rxx; 
		pStiff->Ryy   = StiffBackup[0].Ryy; 
		pStiff->Rzz   = StiffBackup[0].Rzz; 

		pStiff = &m_Data.SectBefore.SectJ.Stiffness;
		pStiff->Area  = StiffBackup[1].Area; 
		pStiff->WArea = StiffBackup[1].WArea; 
		pStiff->Asy   = StiffBackup[1].Asy; 
		pStiff->Asz   = StiffBackup[1].Asz; 
		pStiff->Rxx   = StiffBackup[1].Rxx; 
		pStiff->Ryy   = StiffBackup[1].Ryy; 
		pStiff->Rzz   = StiffBackup[1].Rzz; 
	}

	BOOL bReturn = FALSE;
	BOOL bReturnSect = TRUE;
	BOOL bReturnSecp = TRUE;

#if defined(_CIVIL)
	BOOL bGrpdMod = FALSE;
	BOOL bStfnMod = FALSE;
	if(m_pDoc->m_pAttrCtrl2->ExistGrpdSsm(m_Key)) bGrpdMod = TRUE;
	if(m_pDoc->m_pAttrCtrl2->ExistStfnSsm(m_Key)) bStfnMod = TRUE;

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
	{
		m_bModifyGrpd = TRUE;
		m_bModifyStfn = TRUE;

		//if (!bGrpdMod)
		{
			T_GRPD_SSM_D rData_I, rData_J;
			if (!m_pDoc->m_pAttrCtrl2->GetGrpdSsm(m_wndGridFirst.m_SectKOrg,  rData_I)) ASSERT(0);
			if (!m_pDoc->m_pAttrCtrl2->GetGrpdSsm(m_wndGridSecond.m_SectKOrg, rData_J)) ASSERT(0);

			m_GrpdSSMD.Initialize();
			m_GrpdSSMD.bSameJ=FALSE;  // I==J:TRUE, I!=J:FALSE
			m_GrpdSSMD.aGrpd[0].Copy(rData_I.aGrpd[0]);
			m_GrpdSSMD.aGrpd[1].Copy(rData_J.aGrpd[0]);
			m_GrpdSSMD.aPart[0].Copy(rData_I.aPart[0]);
			m_GrpdSSMD.aPart[1].Copy(rData_J.aPart[0]);
		}

		//if (!bStfnMod)
		{
			T_STFN_SSM_D rData_I, rData_J;
			if (!m_pDoc->m_pAttrCtrl2->GetStfnSsm(m_wndGridFirst.m_SectKOrg,  rData_I)) ASSERT(0);
			if (!m_pDoc->m_pAttrCtrl2->GetStfnSsm(m_wndGridSecond.m_SectKOrg, rData_J)) ASSERT(0);

			m_StfnSSMD.Initialize();
			m_StfnSSMD.bSameJ = FALSE;  // I==J:TRUE, I!=J:FALSE
			m_StfnSSMD.aStiffener[0].Copy(rData_I.aStiffener[0]);
			m_StfnSSMD.aStiffener[1].Copy(rData_J.aStiffener[0]);
		}
	}


	if(m_bModifyGrpd)
	{
		if(bGrpdMod) m_pDoc->m_pDataCtrl->ModifyGrpdSsm(m_Key, m_Key, m_GrpdSSMD);
		else         m_pDoc->m_pDataCtrl->AddGrpdSsm(m_Key, m_GrpdSSMD);
	}

	if(m_bModifyStfn)
	{
		if(bStfnMod) m_pDoc->m_pDataCtrl->ModifyStfnSsm(m_Key, m_Key, m_StfnSSMD);
		else         m_pDoc->m_pDataCtrl->AddStfnSsm(m_Key, m_StfnSSMD);
	}
#endif

	if (m_bModify)
	{
		if (m_bDesign)
		{ bReturnSect = m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual); }
		else
		{ bReturnSect = m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data); }
	}
	else
	{
		bReturnSect = m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
	}

#if defined(_CIVIL)
	if(bReturnSect)
	{
		if(m_pDoc->m_pAttrCtrl2->ExistSecp(m_OrgKey))
		{
			bReturnSecp = m_pDoc->m_pDataCtrl->ModifySecp(m_OrgKey, m_Key, m_PSCDesign);
		}
		else
		{
			bReturnSecp = m_pDoc->m_pDataCtrl->AddSecp(m_Key, m_PSCDesign);
		}
	}
#endif

	if(bReturnSect && bReturnSecp) bReturn = TRUE;
	else                           bReturn = FALSE; 

	return bReturn;
}

/*BOOL CCMSectItemTap::SaveGeneralData()
{
// offset data backup
int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
double dUserDefOffsetY[2], dUserDefOffsetZ[2];
for (int i=0; i<2; i++)
{
dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
}

// save size
CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon;
CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon;
CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
CArray<int, int> aLineLoop;
CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape;
CArray<T_GSEC_PART, T_GSEC_PART&> aGeneralPart;

int nBeforePart = m_Data.SectBefore.SectI.nBeforePart;
aOuterPolygon.Copy(m_Data.SectBefore.SectI.aOuterPolygon);
aInnerPolygon.Copy(m_Data.SectBefore.SectI.aInnerPolygon);
aLine.Copy(m_Data.SectBefore.SectI.aLine);
aLineLoop.Copy(m_Data.SectBefore.SectI.aLineLoop);
aShape.Copy(m_Data.SectBefore.SectI.aShape);
aGeneralPart.Copy(m_Data.SectBefore.SectI.aGeneralPart);

T_SECT_SECTBASE_D SectBaseCopy;
SectBaseCopy.Initialize();
SectBaseCopy.Stiffness = m_Data.SectAfter.SectI.Stiffness;
SectBaseCopy.PeriOut = m_Data.SectAfter.SectI.PeriOut;
SectBaseCopy.PeriIn = m_Data.SectAfter.SectI.PeriIn;
SectBaseCopy.Design.YBar = m_Data.SectAfter.SectI.Design.YBar;
SectBaseCopy.Design.ZBar = m_Data.SectAfter.SectI.Design.ZBar;

T_SECT_SECTBASE_D SectBaseCopyBefore;
SectBaseCopyBefore.Initialize();
SectBaseCopyBefore.Stiffness = m_Data.SectBefore.SectI.Stiffness;
SectBaseCopyBefore.PeriOut = m_Data.SectBefore.SectI.PeriOut;
SectBaseCopyBefore.PeriIn = m_Data.SectBefore.SectI.PeriIn;
SectBaseCopyBefore.Design.YBar = m_Data.SectBefore.SectI.Design.YBar;
SectBaseCopyBefore.Design.ZBar = m_Data.SectBefore.SectI.Design.ZBar;

// save data
m_Data.Initialize();
m_Data.nStype = D_SECT_TYPE_TAPERED;
m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;

// ID
CString csID;
m_wndID.GetWindowText(csID);
m_Key = (T_SECT_K)_ttol(csID);

// Name
m_wndName.GetWindowText(m_Data.SName);
m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

// Offset
m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef; // MQC:1493 20071130 mylee
m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
for (i=0; i<2; i++)
{
m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
}

// consider shear deformation
m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();

// size, peri and stiffness
m_Data.SectBefore.SectI.Stiffness.WArea = m_Data.SectBefore.SectI.Stiffness.Area;

// general section
m_Data.SectAfter.SectI.Stiffness = SectBaseCopy.Stiffness;
m_Data.SectAfter.SectI.PeriOut = SectBaseCopy.PeriOut;
m_Data.SectAfter.SectI.PeriIn = SectBaseCopy.PeriIn;
m_Data.SectAfter.SectI.Design.YBar = SectBaseCopy.Design.YBar;
m_Data.SectAfter.SectI.Design.ZBar = SectBaseCopy.Design.ZBar;

m_Data.SectBefore.SectI.Stiffness = SectBaseCopyBefore.Stiffness;
m_Data.SectBefore.SectI.PeriOut = SectBaseCopyBefore.PeriOut;
m_Data.SectBefore.SectI.PeriIn = SectBaseCopyBefore.PeriIn;
m_Data.SectBefore.SectI.Design.YBar = SectBaseCopyBefore.Design.YBar;
m_Data.SectBefore.SectI.Design.ZBar = SectBaseCopyBefore.Design.ZBar;

m_Data.SectBefore.SectI.nBeforePart = nBeforePart;
m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon);
m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon);
m_Data.SectBefore.SectI.aLine.Copy(aLine);
m_Data.SectBefore.SectI.aLineLoop.Copy(aLineLoop);
m_Data.SectBefore.SectI.aShape.Copy(aShape);
m_Data.SectBefore.SectI.aGeneralPart.Copy(aGeneralPart);
//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);
m_Data.SectBefore.nCalcStiffOpt = 1;

// before part
m_Data.SectBefore.SectI.nBeforePart = m_cmbBeforePart.GetCurSel()+1;

// Before 에 해당하는 강성을 SectBefore 에 복사한다. (EditData 에도 해주는 부분 있음)
int nTmp = m_Data.SectBefore.SectI.nBeforePart - 1;
if(nTmp<aGeneralPart.GetSize())
{
m_Data.SectBefore.SectI.Stiffness = aGeneralPart[nTmp].StiffnessSum;
m_Data.SectBefore.SectI.PeriOut = aGeneralPart[nTmp].PeriOutSum;
m_Data.SectBefore.SectI.PeriIn = aGeneralPart[nTmp].PeriInSum;
m_Data.SectBefore.SectI.Design.YBar = aGeneralPart[nTmp].YBarSum;
m_Data.SectBefore.SectI.Design.ZBar = aGeneralPart[nTmp].ZBarSum;
}

return TRUE;
}*/

BOOL CCMSectItemTap::Dlg2Data()
{
	// offset data backup
	int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
		dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
		dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}

	// stiffness, polygon data backup
	BOOL bUserStiff;
	T_SECT_STIFFNESS aUserStiff[2];
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon[4];
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon[4];
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine[4];
	CArray<int, int> aLineLoop[4];
	CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape[4];
	CArray<T_GSEC_PART,T_GSEC_PART&> aGeneralPart[4];
	T_SECT_STIFFNESS aAfterSectStiffness;
	T_SECT_SECTBASE_D SectBaseCopy[2];
	T_SECT_SECTBASE_D SectBaseCopyBefore[2];
	T_SECT_STIFF_D Stiffener;  
	T_SECT_SOD_STIFF_D SODStiffener;
	T_SECT_RIVET_STIFF_D RivetStiff;
	CString aSName[2]; // 텐던 자동배치에 사용

	double dWarpingCheckPosI[2][6]={0.0};
	double dWarpingCheckPosJ[2][6]={0.0};

	if (IsPscSect() || IsGeneralSect())
	{
		// 임의형상 합성단면의 경우 미리 I,J 단의 강성값을 저장해두고 데이터를 복사한다...
		// 근데 왜 복사하는 걸까??
		if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
		{
			m_wndGridComGenI.SaveData();
			m_wndGridComGenJ.SaveData();
		}

		bUserStiff = m_Data.SectBefore.bUserInputStiffness;
		if (bUserStiff)
		{
			aUserStiff[0] = m_Data.SectBefore.SectI.Stiffness;
			aUserStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
		}
		aOuterPolygon[0].Copy(m_Data.SectBefore.SectI.aOuterPolygon);
		aOuterPolygon[1].Copy(m_Data.SectBefore.SectJ.aOuterPolygon);
		aOuterPolygon[2].Copy(m_Data.SectAfter.SectI.aOuterPolygon);
		aOuterPolygon[3].Copy(m_Data.SectAfter.SectJ.aOuterPolygon);
		aInnerPolygon[0].Copy(m_Data.SectBefore.SectI.aInnerPolygon);
		aInnerPolygon[1].Copy(m_Data.SectBefore.SectJ.aInnerPolygon);
		aInnerPolygon[2].Copy(m_Data.SectAfter.SectI.aInnerPolygon);
		aInnerPolygon[3].Copy(m_Data.SectAfter.SectJ.aInnerPolygon);
		aLine[0].Copy(m_Data.SectBefore.SectI.aLine);
		aLine[1].Copy(m_Data.SectBefore.SectJ.aLine);
		aLine[2].Copy(m_Data.SectAfter.SectI.aLine);
		aLine[3].Copy(m_Data.SectAfter.SectJ.aLine);
		aLineLoop[0].Copy(m_Data.SectBefore.SectI.aLineLoop);
		aLineLoop[1].Copy(m_Data.SectBefore.SectJ.aLineLoop);
		aLineLoop[2].Copy(m_Data.SectAfter.SectI.aLineLoop);
		aLineLoop[3].Copy(m_Data.SectAfter.SectJ.aLineLoop);
		aShape[0].Copy(m_Data.SectBefore.SectI.aShape);
		aShape[1].Copy(m_Data.SectBefore.SectJ.aShape);    
		aShape[2].Copy(m_Data.SectAfter.SectI.aShape);
		aShape[3].Copy(m_Data.SectAfter.SectJ.aShape);    
		aGeneralPart[0].Copy(m_Data.SectBefore.SectI.aGeneralPart);        
		aGeneralPart[1].Copy(m_Data.SectBefore.SectJ.aGeneralPart);    
		aGeneralPart[2].Copy(m_Data.SectAfter.SectI.aGeneralPart);    
		aGeneralPart[3].Copy(m_Data.SectAfter.SectJ.aGeneralPart);
		aSName[0] = m_Data.SectBefore.SectI.SName;
		aSName[1] = m_Data.SectBefore.SectJ.SName;

		aAfterSectStiffness = m_Data.SectAfter.Stiffness;

		SectBaseCopy[0].Initialize();
		SectBaseCopy[1].Initialize();
		SectBaseCopyBefore[0].Initialize();
		SectBaseCopyBefore[1].Initialize();
		if (IsPscSect())
		{
			memcpy(dWarpingCheckPosI, m_Data.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPosI)); 
			memcpy(dWarpingCheckPosJ, m_Data.SectBefore.dWarpingCheckPosJ, sizeof(dWarpingCheckPosJ));
		}

		if (IsGeneralSect())
		{
			SectBaseCopy[0].Stiffness = m_Data.SectAfter.SectI.Stiffness;
			SectBaseCopy[0].PeriOut = m_Data.SectAfter.SectI.PeriOut;
			SectBaseCopy[0].PeriIn = m_Data.SectAfter.SectI.PeriIn;
			SectBaseCopy[0].Design.YBar = m_Data.SectAfter.SectI.Design.YBar;
			SectBaseCopy[0].Design.ZBar = m_Data.SectAfter.SectI.Design.ZBar;

			SectBaseCopyBefore[0].Stiffness = m_Data.SectBefore.SectI.Stiffness;
			SectBaseCopyBefore[0].PeriOut = m_Data.SectBefore.SectI.PeriOut;
			SectBaseCopyBefore[0].PeriIn = m_Data.SectBefore.SectI.PeriIn;
			SectBaseCopyBefore[0].Design.YBar = m_Data.SectBefore.SectI.Design.YBar;
			SectBaseCopyBefore[0].Design.ZBar = m_Data.SectBefore.SectI.Design.ZBar;

			SectBaseCopy[1].Stiffness = m_Data.SectAfter.SectJ.Stiffness;
			SectBaseCopy[1].PeriOut = m_Data.SectAfter.SectJ.PeriOut;
			SectBaseCopy[1].PeriIn = m_Data.SectAfter.SectJ.PeriIn;
			SectBaseCopy[1].Design.YBar = m_Data.SectAfter.SectJ.Design.YBar;
			SectBaseCopy[1].Design.ZBar = m_Data.SectAfter.SectJ.Design.ZBar;

			SectBaseCopyBefore[1].Stiffness = m_Data.SectBefore.SectJ.Stiffness;
			SectBaseCopyBefore[1].PeriOut = m_Data.SectBefore.SectJ.PeriOut;
			SectBaseCopyBefore[1].PeriIn = m_Data.SectBefore.SectJ.PeriIn;
			SectBaseCopyBefore[1].Design.YBar = m_Data.SectBefore.SectJ.Design.YBar;
			SectBaseCopyBefore[1].Design.ZBar = m_Data.SectBefore.SectJ.Design.ZBar;
		}            
	}

	Stiffener = m_Data.SectBefore.SectI.Stiffener;
	SODStiffener = m_Data.SectBefore.SectI.SODStiffener;
	RivetStiff = m_Data.SectBefore.SectI.RivetStiff;

	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_TAPERED;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_ttol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex == LB_ERR) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
		return FALSE;
	}
	if (IsPscSect()) // psc인 경우 Shape은 grid 안에서 저장한다.
		m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexPsc(nShapeIndex-D_SECT_PSC_SHAPE_INDEX);
	else if (IsCompositeSect()) 
	{
		int nTmp = nShapeIndex-D_SECT_COMPO_SHAPE_INDEX;
		if (nTmp==0)      m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_B; 
		else if (nTmp==1) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_I; 
		else if (nTmp==2) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_CI; 
		else if (nTmp==3) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_CT; 
		else if (nTmp==5) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G; 
		else if (nTmp==6) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_TUB;
		else if (nTmp==7) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_B;
		else if (nTmp==8) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_I;
		else if (nTmp==9) m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_TUB;
	}
	else if (IsStlgSect()) 
	{
		m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexStlGirder(nShapeIndex-D_SECT_STLG_SHAPE_INDEX);
	}
	else
		m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	// Cell shape - Cell number
	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT)
	{
		CString csCellNum;
		m_editCellNum.GetWindowText(csCellNum);
		m_Data.SectBefore.nCellShape = _ttoi(csCellNum);
	}

	// Variation
	m_Data.SectBefore.nLytype = m_wndVar1.GetCurSel()+1;
	m_Data.SectBefore.nLztype = m_wndVar2.GetCurSel()+1;

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef;
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	for (int i=0; i<2; i++)
	{
		m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
		m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	if (IsValueSect())  // *^^* Value Section
	{
		// Size, 강성저장
		if (nShapeIndex-D_SECT_COMPO_SHAPE_INDEX == 5) 
			m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
		else 
			m_Data.SectBefore.nStype = D_SECT_TYPE_USER;

		if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER)
		{
			m_wndGridFirst.SaveData();
			m_wndGridSecond.SaveData();

			if (IsGeneralSect())
			{
				if (aOuterPolygon[0].GetSize() > 0)
					m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon[0]);
				if (aOuterPolygon[1].GetSize() > 0)
					m_Data.SectBefore.SectJ.aOuterPolygon.Copy(aOuterPolygon[1]);
				if (aInnerPolygon[0].GetSize() > 0) 
					m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon[0]);
				if (aInnerPolygon[1].GetSize() > 0) 
					m_Data.SectBefore.SectJ.aInnerPolygon.Copy(aInnerPolygon[1]);
				if (aLine[0].GetSize() > 0)
					m_Data.SectBefore.SectI.aLine.Copy(aLine[0]);
				if (aLine[1].GetSize() > 0)
					m_Data.SectBefore.SectJ.aLine.Copy(aLine[1]);
				if (aLineLoop[0].GetSize() > 0)
					m_Data.SectBefore.SectI.aLineLoop.Copy(aLineLoop[0]);
				if (aLineLoop[1].GetSize() > 0)
					m_Data.SectBefore.SectJ.aLineLoop.Copy(aLineLoop[1]);
				if (aShape[0].GetSize() > 0)
					m_Data.SectBefore.SectI.aShape.Copy(aShape[0]);
				if (aShape[1].GetSize() > 0)
					m_Data.SectBefore.SectJ.aShape.Copy(aShape[1]);
			}
		}
		else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
		{
			if (aOuterPolygon[0].GetSize() > 0)
				m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon[0]);
			if (aOuterPolygon[1].GetSize() > 0)
				m_Data.SectBefore.SectJ.aOuterPolygon.Copy(aOuterPolygon[1]);
			if (aOuterPolygon[2].GetSize() > 0)
				m_Data.SectAfter.SectI.aOuterPolygon.Copy(aOuterPolygon[2]);
			if (aOuterPolygon[3].GetSize() > 0)
				m_Data.SectAfter.SectJ.aOuterPolygon.Copy(aOuterPolygon[3]);

			if (aInnerPolygon[0].GetSize() > 0) 
				m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon[0]);
			if (aInnerPolygon[1].GetSize() > 0) 
				m_Data.SectBefore.SectJ.aInnerPolygon.Copy(aInnerPolygon[1]);
			if (aInnerPolygon[2].GetSize() > 0) 
				m_Data.SectAfter.SectI.aInnerPolygon.Copy(aInnerPolygon[2]);
			if (aInnerPolygon[3].GetSize() > 0) 
				m_Data.SectAfter.SectJ.aInnerPolygon.Copy(aInnerPolygon[3]);

			if (aLine[0].GetSize() > 0)
				m_Data.SectBefore.SectI.aLine.Copy(aLine[0]);
			if (aLine[1].GetSize() > 0)
				m_Data.SectBefore.SectJ.aLine.Copy(aLine[1]);
			if (aLine[2].GetSize() > 0)
				m_Data.SectAfter.SectI.aLine.Copy(aLine[2]);
			if (aLine[3].GetSize() > 0)
				m_Data.SectAfter.SectJ.aLine.Copy(aLine[3]);

			if (aLineLoop[0].GetSize() > 0)
				m_Data.SectBefore.SectI.aLineLoop.Copy(aLineLoop[0]);
			if (aLineLoop[1].GetSize() > 0)
				m_Data.SectBefore.SectJ.aLineLoop.Copy(aLineLoop[1]);
			if (aLineLoop[2].GetSize() > 0)
				m_Data.SectAfter.SectI.aLineLoop.Copy(aLineLoop[2]);
			if (aLineLoop[3].GetSize() > 0)
				m_Data.SectAfter.SectJ.aLineLoop.Copy(aLineLoop[3]);

			if (aShape[0].GetSize() > 0)
				m_Data.SectBefore.SectI.aShape.Copy(aShape[0]);
			if (aShape[1].GetSize() > 0)
				m_Data.SectBefore.SectJ.aShape.Copy(aShape[1]);
			if (aShape[2].GetSize() > 0)
				m_Data.SectAfter.SectI.aShape.Copy(aShape[2]);
			if (aShape[3].GetSize() > 0)
				m_Data.SectAfter.SectJ.aShape.Copy(aShape[3]);

			if (aGeneralPart[0].GetSize()>0)
				m_Data.SectBefore.SectI.aGeneralPart.Copy(aGeneralPart[0]);
			if (aGeneralPart[1].GetSize()>0)
				m_Data.SectBefore.SectJ.aGeneralPart.Copy(aGeneralPart[1]);
			if (aGeneralPart[2].GetSize()>0)
				m_Data.SectAfter.SectI.aGeneralPart.Copy(aGeneralPart[2]);
			if (aGeneralPart[3].GetSize()>0)
				m_Data.SectAfter.SectJ.aGeneralPart.Copy(aGeneralPart[3]);

			m_Data.SectAfter.Stiffness = aAfterSectStiffness;
			m_Data.SectBefore.SectI.nBeforePart = m_cmbBeforePart.GetCurSel()+1;

			// general section
			m_Data.SectAfter.SectI.Stiffness    = SectBaseCopy[0].Stiffness;
			m_Data.SectAfter.SectI.PeriOut      = SectBaseCopy[0].PeriOut;
			m_Data.SectAfter.SectI.PeriIn       = SectBaseCopy[0].PeriIn;
			m_Data.SectAfter.SectI.Design.YBar  = SectBaseCopy[0].Design.YBar;
			m_Data.SectAfter.SectI.Design.ZBar  = SectBaseCopy[0].Design.ZBar;

			m_Data.SectBefore.SectI.Stiffness   = SectBaseCopyBefore[0].Stiffness;
			m_Data.SectBefore.SectI.PeriOut     = SectBaseCopyBefore[0].PeriOut;
			m_Data.SectBefore.SectI.PeriIn      = SectBaseCopyBefore[0].PeriIn;
			m_Data.SectBefore.SectI.Design.YBar = SectBaseCopyBefore[0].Design.YBar;
			m_Data.SectBefore.SectI.Design.ZBar = SectBaseCopyBefore[0].Design.ZBar;

			m_Data.SectAfter.SectJ.Stiffness    = SectBaseCopy[1].Stiffness;
			m_Data.SectAfter.SectJ.PeriOut      = SectBaseCopy[1].PeriOut;
			m_Data.SectAfter.SectJ.PeriIn       = SectBaseCopy[1].PeriIn;
			m_Data.SectAfter.SectJ.Design.YBar  = SectBaseCopy[1].Design.YBar;
			m_Data.SectAfter.SectJ.Design.ZBar  = SectBaseCopy[1].Design.ZBar;

			m_Data.SectBefore.SectJ.Stiffness   = SectBaseCopyBefore[1].Stiffness;
			m_Data.SectBefore.SectJ.PeriOut     = SectBaseCopyBefore[1].PeriOut;
			m_Data.SectBefore.SectJ.PeriIn      = SectBaseCopyBefore[1].PeriIn;
			m_Data.SectBefore.SectJ.Design.YBar = SectBaseCopyBefore[1].Design.YBar;
			m_Data.SectBefore.SectJ.Design.ZBar = SectBaseCopyBefore[1].Design.ZBar;

			int nTmp = m_Data.SectBefore.SectI.nBeforePart - 1;
			if(nTmp < aGeneralPart[0].GetSize())
			{
				CArray<T_GSEC_PART,T_GSEC_PART&>& aTemp = aGeneralPart[0];

				m_Data.SectBefore.SectI.Stiffness   = aTemp[nTmp].StiffnessSum;
				m_Data.SectBefore.SectI.PeriOut     = aTemp[nTmp].PeriOutSum;
				m_Data.SectBefore.SectI.PeriIn      = aTemp[nTmp].PeriInSum;
				m_Data.SectBefore.SectI.Design.YBar = aTemp[nTmp].YBarSum;
				m_Data.SectBefore.SectI.Design.ZBar = aTemp[nTmp].ZBarSum;
			}
			if(nTmp < aGeneralPart[1].GetSize())
			{      
				CArray<T_GSEC_PART,T_GSEC_PART&>& aTemp = aGeneralPart[1];

				m_Data.SectBefore.SectJ.Stiffness   = aTemp[nTmp].StiffnessSum;
				m_Data.SectBefore.SectJ.PeriOut     = aTemp[nTmp].PeriOutSum;
				m_Data.SectBefore.SectJ.PeriIn      = aTemp[nTmp].PeriInSum;
				m_Data.SectBefore.SectJ.Design.YBar = aTemp[nTmp].YBarSum;
				m_Data.SectBefore.SectJ.Design.ZBar = aTemp[nTmp].ZBarSum;
			}
		}
	}
	else if (IsPscSect())
	{
		// Flag, Size 저장

		m_Data.SectBefore.nStype = D_SECT_TYPE_PSC;
		//m_wndGridFirst4Psc.SaveData();
		//m_wndGridSecond4Psc.SaveData();
		m_wndGrid4Psc.SaveData();

		// warping stress point
		m_Data.SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
		m_Data.SectBefore.nWarpingCheck[1] =m_nWarpingCheck;
		memcpy(m_Data.SectBefore.dWarpingCheckPosI, dWarpingCheckPosI, sizeof(dWarpingCheckPosI)); 
		memcpy(m_Data.SectBefore.dWarpingCheckPosJ, dWarpingCheckPosJ, sizeof(dWarpingCheckPosJ));

		// material data
		if (m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)   // 파형강판
			GetMatlSectData(&m_Data);

		// mesh size
		m_Data.SectBefore.bUserDefMeshSize = (m_chkMeshSize.GetCheck() == 1) ? TRUE: FALSE;
		if (m_Data.SectBefore.bUserDefMeshSize)
			m_Data.SectBefore.dMeshSize = m_editMeshSize.GetEditValue();

		// stiffness data
		m_Data.SectBefore.bUserInputStiffness = bUserStiff;
		if (bUserStiff)
		{
			m_Data.SectBefore.SectI.Stiffness = aUserStiff[0];
			m_Data.SectBefore.SectJ.Stiffness = aUserStiff[1];
		}

		// polygon data
		if (aOuterPolygon[0].GetSize() > 0)
			m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon[0]);
		if (aOuterPolygon[1].GetSize() > 0)
			m_Data.SectBefore.SectJ.aOuterPolygon.Copy(aOuterPolygon[1]);
		if (aInnerPolygon[0].GetSize() > 0) 
			m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon[0]);
		if (aInnerPolygon[1].GetSize() > 0) 
			m_Data.SectBefore.SectJ.aInnerPolygon.Copy(aInnerPolygon[1]);

		m_Data.SectBefore.SectI.SName = aSName[0];
		m_Data.SectBefore.SectJ.SName = aSName[1];

	}
	else if (IsCompositeSect())
	{
		// Flag 저장
		int nTmp = nShapeIndex-D_SECT_COMPO_SHAPE_INDEX;
		if (nTmp==0)      m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_B; 
		else if (nTmp==1) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_I; 
		else if (nTmp==2) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_CI; 
		else if (nTmp==3) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_CT; 
		else if (nTmp==6) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_TUB;
		else if (nTmp==7) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_B;
		else if (nTmp==8) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_I;
		else if (nTmp==9) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_TUB;
		else ASSERT(0);
		// Composite General 은 위에 Value Sect 에 들어감
		// else if (nTmp==5) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G; 

		// Grid에 있는 Size 정보 저장
		m_wndGrid4Composite.SaveData(); 

		if (nTmp == 2 || nTmp == 3) // CI, CT
		{
			T_SECT_SECTION_D SectBefore;
			m_wndGrid4Composite.GetSectBefore(SectBefore);
			
			if (SectBefore.bShearCheck == TRUE)
			{
				m_Data.SectBefore.bShearCheck = SectBefore.bShearCheck;
				for (int i = 0; i < 2; i++)
				{
					m_Data.SectBefore.bAutoWebThk[i] = SectBefore.bAutoWebThk[i];
					m_Data.SectBefore.dWebThk[i]     = SectBefore.dWebThk[i];

					for (int j = 0; j < 3; j++)
					{
						m_Data.SectBefore.bAutoShearCheckPos[i][j] = SectBefore.bAutoShearCheckPos[i][j];
						m_Data.SectBefore.dShearCheckPos[i][j]     = SectBefore.dShearCheckPos[i][j];

						m_Data.SectBefore.bAutoQy[i][j]          = SectBefore.bAutoQy[i][j];
						m_Data.SectBefore.dQy[i][j]              = SectBefore.dQy[i][j];

						m_Data.SectBefore.bAutoWebThkShear[i][j] = SectBefore.bAutoWebThkShear[i][j];
						m_Data.SectBefore.dWebThkShear[i][j]     = SectBefore.dWebThkShear[i][j];
					}
				}
			}
		}

		// material data
		m_Data.SectBefore.Matl_Elast   = m_wndMatlValueCompo[0].GetEditValue();
		m_Data.SectBefore.Matl_Density = m_wndMatlValueCompo[1].GetEditValue();
		m_Data.SectBefore.Matl_Poisson_S = m_edtMatPs.GetEditValue();
		m_Data.SectBefore.Matl_Poisson_C = m_edtMatPc.GetEditValue();
		m_Data.SectBefore.Matl_Thermal = m_edtMatTstc.GetEditValue();

		if (nTmp == 0 || nTmp == 1 || nTmp == 2 || nTmp == 3 || nTmp == 6 || nTmp == 7 || nTmp == 8 || nTmp == 9)
		{
			m_Data.SectBefore.bMultipleElastic = m_chkMat.GetCheck();
			if (m_Data.SectBefore.bMultipleElastic)
			{
				m_Data.SectBefore.dLongTermESEC = m_edtMat1.GetEditValue();
				m_Data.SectBefore.dShrinkageESEC = m_edtMat2.GetEditValue();
			}
		}

		// stiffness data - 없음
		// polygon data - 없음
	}
	else if (IsStlgSect())
	{
		// Flag 저장
		int nTmp = nShapeIndex-D_SECT_STLG_SHAPE_INDEX;
		if (nTmp==0)      m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_B; 
		else if (nTmp==1) m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_I; 
		else if (nTmp==2) m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_MCELL; 

		else ASSERT(0);
		// Composite General 은 위에 Value Sect 에 들어감
		// else if (nTmp==5) m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G; 

		if(nTmp == 2)
		{
			Dlg2DataMCellEtcCtrl();
		}

		// Grid에 있는 Size 정보 저장
		m_wndGrid4StlG.SaveData(); 

		// stiffness data - 없음
		// polygon data - 없음
	}
	else  
	{
		m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR;

		// Section I, J
		T_SECT_SECTBASE_D* pSectI = &(m_Data.SectBefore.SectI);
		T_SECT_SECTBASE_D* pSectJ = &(m_Data.SectBefore.SectJ);

		// I, J Section Type
		m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR; // currently, only regular is supported

		// Built-Up : Always 1(built-up)
		m_Data.SectBefore.BuiltUpFlag = 1;

		// Section I
		T_SECT_SECTBASE_D* pDataI = pSectI;
		{
			// DB
			pDataI->DBName = GetDB();
			// SName
			if (pDataI->DBName == _T("")) pDataI->SName = _T("");
			else m_wndFirstName.GetWindowText(pDataI->SName);

#if defined(_RUS)
			CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pDataI->DBName, pDataI->SName);
#endif

			// Size
			int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
			for (int i = 0; i < nSizeNum; i++)
				pDataI->Size[i] = m_wndFirstSize[i].GetEditValue();
		}

		// Second
		T_SECT_SECTBASE_D* pDataJ = pSectJ;
		{
			// DB
			pDataJ->DBName = GetDB();
			// SName
			if (pDataJ->DBName == _T("")) pDataJ->SName = _T("");
			else m_wndSecondName.GetWindowText(pDataJ->SName);

#if defined(_RUS)
			CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pDataJ->DBName, pDataJ->SName);
#endif
			// Size
			int nSizeNum = CSectUtil::GetSizeCountByRegShape(m_Data.SectBefore.Shape);
			for (int i = 0; i < nSizeNum; i++)
				pDataJ->Size[i] = m_wndSecondSize[i].GetEditValue();
		}
	}

	// consider shear deformation - 공통
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.SectBefore.bHumblyChk = (m_chkHumbly.GetCheck() || m_chkHumblyBefore.GetCheck());
	m_Data.SectAfter.bHumblyChk = (m_chkHumbly.GetCheck() || m_chkHumblyAfter.GetCheck());

	if (!m_Data.SectBefore.bConsiderWarpingEffect)
	{
		m_Data.SectBefore.nWarpingCheck[0] =0;
		m_Data.SectBefore.nWarpingCheck[1] =0;
		memset(m_Data.SectBefore.dWarpingCheckPosI, 0, sizeof(dWarpingCheckPosI)); 
		memset(m_Data.SectBefore.dWarpingCheckPosJ, 0, sizeof(dWarpingCheckPosJ));
	}
	// PSC Design
	m_PSCDesign.bCompPSC = m_chkPSCDesign.GetCheck();
	m_PSCDesign.dt1[0] = m_edtPSCShear_I.GetEditValue();
	m_PSCDesign.dt1[1] = m_edtPSCShear_J.GetEditValue();

	if (IsGeneralSect() &&
		(m_Data.SectBefore.SectI.aLine.GetSize() > 0 ||
		m_Data.SectBefore.SectJ.aLine.GetSize() > 0 ))
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);
	else m_Data.SectBefore.nCalcStiffOpt = D_DEF_CALC_STIFF_OPT;

	m_Data.SectBefore.SectI.Stiffener = Stiffener;
	m_Data.SectBefore.SectI.SODStiffener = SODStiffener;  
	m_Data.SectBefore.SectI.RivetStiff = RivetStiff;

	ASSERT(m_Data.nStype == D_SECT_TYPE_TAPERED);
	int nStype = m_Data.SectBefore.nStype;
	if(nStype != D_SECT_TYPE_CONSTRUCTION && CDBLib::IsCompositeSect(nStype))
	{
		// hslee : CCMSectItemCom에서 퍼옴..
		// MQC 9509 by Hsshim.
		if (CSectDB::IsEnable7thDOF())
		{
			// MQC Alpha/Beta 4632-59. HSSHIM-20130826. 7자유도 계산시 Slab Width로 계산하지 않음. bc(유효폭)으로 계산하기로 결정.
			// T_SECT_COMPO_CI::Set(const T_SECT_D& rSectD) 함수 참고로 만듬.
			m_Data.CmpTapJ.Size[59] = m_Data.SectAfter.SectI.Size[0] = m_Data.SectAfter.SectJ.Size[0];    // rSect.dWidth = rSect.Slab.Bc
			m_Data.SectAfter.SectI.BuiltUpFlag = 1;                                 // rSect.nNum   = 1
		}
	}

	return TRUE;
}

void CCMSectItemTap::Dlg2DataMCellEtcCtrl()
{
	m_Data.SectBefore.bSymmetric = m_chkMCellSym.GetCheck();

	m_Data.SectBefore.nCellNum = (int)m_edtMCellNoCell.GetEditValue();
	CDlgUtil::CtrlRadioGetCheck(this,  m_aRdoMCellLorR, m_Data.SectBefore.nCellType);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aRdoMCellFShape, m_Data.SectBefore.nFlangeShape);

	m_Data.SectBefore.nJoint = 0;
	if(m_chkMCellJ1.GetCheck())  m_Data.SectBefore.nJoint |= 0x00000001;
	if(m_chkMCellJ2.GetCheck())  m_Data.SectBefore.nJoint |= 0x00000002;
	if(m_chkMCellJr1.GetCheck()) m_Data.SectBefore.nJoint |= 0x00000004;
	if(m_chkMCellJr2.GetCheck()) m_Data.SectBefore.nJoint |= 0x00000008;
}

BOOL CCMSectItemTap::GetPeriAndStiff()
{
	// Tapered User 이면 단면 강성은 입력된 값을 사용한다.
	if (IsValueSect()) 
	{
		int nShapeIndex = GetCurShapeIndex();

		// 강성 저장
		if (nShapeIndex-D_SECT_COMPO_SHAPE_INDEX == 5)
		{
			/*m_wndGridComGenI.SaveStiffness();
			m_wndGridComGenJ.SaveStiffness();

			int nTmp = m_Data.SectBefore.SectI.nBeforePart - 1;
			if(nTmp<aGeneralPart[0].GetSize())
			{
			CArray<T_GSEC_PART,T_GSEC_PART&>& aTemp = aGeneralPart[0];

			m_Data.SectBefore.SectI.Stiffness   = aTemp[nTmp].StiffnessSum;
			m_Data.SectBefore.SectI.PeriOut     = aTemp[nTmp].PeriOutSum;
			m_Data.SectBefore.SectI.PeriIn      = aTemp[nTmp].PeriInSum;
			m_Data.SectBefore.SectI.Design.YBar = aTemp[nTmp].YBarSum;
			m_Data.SectBefore.SectI.Design.ZBar = aTemp[nTmp].ZBarSum;
			}
			if(nTmp<aGeneralPart[1].GetSize())
			{      
			CArray<T_GSEC_PART,T_GSEC_PART&>& aTemp = aGeneralPart[1];

			m_Data.SectBefore.SectJ.Stiffness   = aTemp[nTmp].StiffnessSum;
			m_Data.SectBefore.SectJ.PeriOut     = aTemp[nTmp].PeriOutSum;
			m_Data.SectBefore.SectJ.PeriIn      = aTemp[nTmp].PeriInSum;
			m_Data.SectBefore.SectJ.Design.YBar = aTemp[nTmp].YBarSum;
			m_Data.SectBefore.SectJ.Design.ZBar = aTemp[nTmp].ZBarSum;
			}*/
		}
		else
		{
			m_wndGridFirst.SaveStiffness();
			m_wndGridSecond.SaveStiffness();
		}

		m_Data.SectBefore.SectI.Stiffness.WArea = m_Data.SectBefore.SectI.Stiffness.Area;
		m_Data.SectBefore.SectJ.Stiffness.WArea = m_Data.SectBefore.SectJ.Stiffness.Area;

		return TRUE; // *^^* Value
	}
	// PSC Value 이면 단면 강성은 입력된 값을 사용한다.
	else if (IsPSCValue())
	{
		m_wndGrid4Psc.SaveSize();
		m_Data.SectBefore.SectI.Stiffness.WArea = m_Data.SectBefore.SectI.Stiffness.Area;
		m_Data.SectBefore.SectJ.Stiffness.WArea = m_Data.SectBefore.SectJ.Stiffness.Area;

		if(m_Data.SectBefore.bConsiderWarpingEffect)
		{
			// 추가 point 6개 계산. 
			CSectUtil::CalcSectData4PScTaperedVal(m_Data); 
		}

		return TRUE;
	}

	return CSectUtil::CalcSectData(m_Data); // regular or PSC
}

void CCMSectItemTap::InitControls()
{
	// Init grid for user section
	m_unitMeshSize.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.dMeshSize);
	m_editMeshSize.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.dMeshSize);
	m_wndGridFirst.SetTapered(TRUE, 0); // i
	m_wndGridSecond.SetTapered(TRUE, 1);  // j
	
	m_wndGridFirst.InitGrid(&m_wndSecView);
	m_wndGridSecond.InitGrid(&m_wndSecView);

	// MNET:XXXX-CJJEONG-20090309 (Tel : 2124) 
	//m_wndGridComGenI.SetTapered(TRUE, 0); // i
	//m_wndGridComGenJ.SetTapered(TRUE, 1);  // j
	
	m_wndGridComGenI.InitGrid(&m_wndSecViewCGI);
	m_wndGridComGenJ.InitGrid(&m_wndSecViewCGJ);

	m_wndGrid4Psc.InitGrid(&m_wndSecView);

	// warping stress point
	ShowHidePSCWarpingCheck(FALSE);

	m_wndGrid4Composite.InitGrid(&m_wndSecView);

	m_wndGrid4StlG.InitGrid(&m_wndSecView);

	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);

	// Init DB name combo
	SetDBNameList();
	m_wndDB.SetCurSel(0);

	// Init first/second name combo
	m_wndFirstName.SetActionWhenKillFocus(2, -1);
	m_wndSecondName.SetActionWhenKillFocus(2, -1);  

	// Init Variation combo
	CString varName[] = {_LS(IDS_WG_CMD__ADDD__Linear_ITIZI), _LS(IDS_WG_CMD__ADDD__Parabolic), _LS(IDS_WG_CMD__ADDD__Cubic)};
	m_wndVar1.SetRedraw(FALSE);
	m_wndVar2.SetRedraw(FALSE);
	for (int i = 0; i < 3; i++)
	{
		m_wndVar1.AddString(varName[i]);
		m_wndVar2.AddString(varName[i]);
	}
	m_wndVar1.SetCurSel(0); // default value : Linear
	m_wndVar2.SetCurSel(0); 
	m_wndVar1.SetRedraw(TRUE);
	m_wndVar2.SetRedraw(TRUE);
	// preview control
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	// MNET:XXXX-CJJEONG-20090316 (Tel : 2124) 
	CWnd* pWndI = GetDlgItem(IDC_CMD_SP_ID_PREVIEW_CGI);
	ASSERT(pWndI);
	m_wndSecViewCGI.Init(pWndI);
	CWnd* pWndJ = GetDlgItem(IDC_CMD_SP_ID_PREVIEW_CGJ);
	ASSERT(pWndJ);
	m_wndSecViewCGJ.Init(pWndJ);

	// Unit control
	for (int i = 0; i < 8; i++)
	{
		m_wndFirstSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndFirstUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (int i = 0; i < 8; i++)
	{
		m_wndSecondSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSecondUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for (int i = 0; i < 4; i++)
	{
		m_wndMatlValue[i].SetUnitType(0);
	}
	for (int i = 0; i < 2; i++)
	{
		m_wndMatlValueCompo[i].SetUnitType(0);
	}

	m_edtPSCShear_I.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPSCShear_J.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPSCShear.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtMatTstc.SetUnitType(0);

	m_spnMCellNoCell.SetRange(D_SECT_TP_STLG_MCEL_NO_MIN, D_SECT_TP_STLG_MCEL_NO_MAX);

	m_aRdoMCellLorR.RemoveAll();
	m_aRdoMCellLorR.Add(IDC_CMD_SECT_MCELL_LEFT_RDO);
	m_aRdoMCellLorR.Add(IDC_CMD_SECT_MCELL_RIGHT_RDO);

	m_aRdoMCellFShape.RemoveAll();
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_GEN_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_REBAR1_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_REBAR2_RDO);
	m_aRdoMCellFShape.Add(IDC_CMD_SECT_MCELL_CLOSED_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoMCellFShape, 0);

	// *^^* Value Section
	// Align Control, Resize Dialog
	AlignControls();
}

void CCMSectItemTap::AlignControls()
{
	CRect rRef;
	CRect rMove;
	int nDistX;

	// move grid
	GetDlgItem(IDC_CMD_SP_ID_FRAME_I)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SP_ID_GRID_I)->GetWindowRect(rMove);
	nDistX = (rRef.Width() - rMove.Width()) / 2;
	nDistX = (rRef.left + nDistX) - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlValue, nDistX);

	//GetDlgItem(IDC_CMD_SP_ID_GRID_I_PSC)->GetWindowRect(rMove);
	GetDlgItem(IDC_CMD_SP_ID_GRID_PSC)->GetWindowRect(rMove);
	nDistX = (rRef.Width() - rMove.Width()) / 2;
	nDistX = (rRef.left + nDistX) - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlValuePSC, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_GRID_COMPO)->GetWindowRect(rMove);
	nDistX = (rRef.Width() - rMove.Width()) / 2;
	nDistX = (rRef.left + nDistX) - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlComposite, nDistX);

	// MNET:XXXX-CJJEONG-20090309 (Tel : 2124) 
	GetDlgItem(IDC_CMD_SP_ID_GRID_I2)->GetWindowRect(rMove);
	nDistX = (rRef.Width() - rMove.Width()) / 2;
	nDistX = (rRef.left + nDistX) - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlComposite2, nDistX);

	// Y축 방향 이동 추가
	CRect rRefName;
	GetDlgItem(IDC_CMD_SP_CG_PART_STC)->GetWindowRect(rMove);
	GetDlgItem(IDC_CMD_SP_ID_NAME)->GetWindowRect(rRefName);
	int nDistY = rRefName.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlComposite2, nDistY);

	GetDlgItem(IDC_CMD_MATL_FRM)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlOption[0])->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlOption, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_FRAME_I)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlStlgBoxI[0])->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlStlgBoxI, nDistX);

	GetDlgItem(IDC_CMD_SP_ID_BMP)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlStlgMCell[0])->GetWindowRect(rMove);
	nDistX = rRef.left - rMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlStlgMCell, nDistX);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_SP_ID_VAR2);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right+globalUtils.ScaleByDPI(1);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

// Composite 일때만 보여지고 숨겨지는 것들 (TRUE - Composite 일때, FALSE - 그외 다른것으로 바뀔때)
void CCMSectItemTap::ShowHideComposite(BOOL bComposite) 
{
	// 위아래로 옮겨지는 컨트롤들
	CArray<UINT, UINT> aCtrlTmp;
	aCtrlTmp.Add(IDC_CMD_VAR1_STC);
	aCtrlTmp.Add(IDC_CMD_VAR2_STC);
	aCtrlTmp.Add(IDC_CMD_SP_ID_VAR1);
	aCtrlTmp.Add(IDC_CMD_SP_ID_VAR2);
	aCtrlTmp.Add(IDC_CMD_SHEAR_DEFORM);
	aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_CHK);

	if(bComposite || IsStlgSect())
	{
		GetDlgItem(IDC_CMD_WARPING_EFFECT_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->ShowWindow(FALSE);
	}

	if (bComposite)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite, TRUE);    

		CRect rFrom, rTo;
		GetDlgItem(IDC_CMD_SP_ID_VAR1)->GetWindowRect(rFrom);
		GetDlgItem(IDC_CMD_MATL2_GRP)->GetWindowRect(rTo);

		int nDistY = rTo.bottom - rFrom.top;
		CDlgUtil::CtrlMoveDistY(this, aCtrlTmp, nDistY+globalUtils.ScaleByDPI(4));

		// Warping
		if ( m_Data.SectBefore.nStype != D_SECT_TYPE_COMPO_G )
		{
			CArray<UINT, UINT> aCtrlWarping;
			aCtrlWarping.Add(IDC_CMD_WARPING_EFFECT_CHK);

			GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK)->GetWindowRect(rFrom);
			GetDlgItem(IDC_CMD_SHEAR_DEFORM)->GetWindowRect(rTo);

			nDistY = rTo.bottom - rFrom.top;
			CDlgUtil::CtrlMoveDistY(this, aCtrlWarping, nDistY + globalUtils.ScaleByDPI(4));
		}
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite, FALSE);

		CRect rFrom, rTo;
		GetDlgItem(IDC_CMD_SP_ID_VAR1)->GetWindowRect(rFrom);
		GetDlgItem(IDC_CMD_SP_ID_FRAME_J)->GetWindowRect(rTo);

		int nDistY = rTo.bottom - rFrom.top;
		CDlgUtil::CtrlMoveDistY(this, aCtrlTmp, nDistY+globalUtils.ScaleByDPI(4));

		if ( m_Data.SectBefore.nStype != D_SECT_TYPE_COMPO_G )
		{
			CArray<UINT, UINT> aCtrlWarping;
			aCtrlWarping.Add(IDC_CMD_WARPING_EFFECT_CHK);

			GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK)->GetWindowRect(rFrom);
			GetDlgItem(IDC_CMD_MESH_SIZE_CHK2)->GetWindowRect(rTo);

			nDistY = rTo.bottom - rFrom.top;
			CDlgUtil::CtrlMoveDistY(this, aCtrlWarping, nDistY + globalUtils.ScaleByDPI(4));
		}
	}

	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I) 
	{
		GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(TRUE);

		GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->ShowWindow(FALSE);
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->ShowWindow(TRUE);

		GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_HUMBLY_CHK)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_GRBOX)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_BF)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HUMBLY_CHK_AF)->ShowWindow(FALSE);
	}
}

void CCMSectItemTap::ShowHidePSCWarpingCheck(BOOL bPSC)
{
	CArray<UINT, UINT> m_aCtrlTmp;
	m_aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_STC);
	m_aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_AUTO_RDO);
	m_aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_USER_RDO);
	m_aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_BTN);


	if (bPSC)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlTmp, TRUE);    

		m_nWarpingCheck = m_Data.SectBefore.nWarpingCheck[0];
		if (!CSectDB::IsEnable7thDOF())
		{
			m_chkWarpingEffect.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->ShowWindow(SW_HIDE);
		}
		else
		{
			BOOL bChkWE = m_chkWarpingEffect.GetCheck();
			GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
			GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);

			if(m_nWarpingCheck==0) // Auto
			{
				GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlTmp, FALSE);    
	}

}

void CCMSectItemTap::SetSectionShapeCombo()
{
#if defined(_MGEN) || defined(_TOWER)
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section),
		_LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe),
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle),
		_LS(IDS_WG_CMD__ADDD__Solid_Round),_LS(IDS_WG_CMD__ADDD__General_Section),
	};
#elif defined(_CIVIL)
	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section),
		_LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe),
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle),
		_LS(IDS_WG_CMD__ADDD__Solid_Round),
		_LS(IDS_WG_CMD__ADDD__Octagon), _LS(IDS_WG_CMD__ADDD__Solid_Octagon), /*_LS(IDS_WG_CMD__ADDD__Rect_Octagon),*/
		_LS(IDS_WG_CMD__ADDD__Track), _LS(IDS_WG_CMD__ADDD__Solid_Track), _LS(IDS_WG_CMD__ADDD__Half_Track),
		_LS(IDS_WG_CMD__ADDD__General_Section),
		_LS(IDS_CMD0417__PSC_1CELL), _LS(IDS_CMD0417__PSC_2CELL), _LS(IDS_CMD_PSC_SHAPE_3CELL),
		_LS(IDS_CMD_PSC_SHAPE_4CELL), _LS(IDS_CMD_PSC_SHAPE_nCELL2), _LS(IDS_CMD_PSC_SHAPE_MID),
		_LS(IDS_CMD_PSC_SHAPE_I), _LS(IDS_CMD_PSC_SHAPE_HALF), _LS(IDS_CMD_PSC_SHAPE_TEE),
		_LS(IDS_CMD_PSC_SHAPE_PLAT), _LS(IDS_CMD_PSC_SHAPE_VALUE), _LS(IDS_CMD_PSC_SHAPE_CMP),
		_LS(IDS_CMD_SECT_FULL_Comp_Steel_Box_TYPE1), _LS(IDS_CMD_SECT_FULL_Comp_Steel_I_TYPE1), _LS(IDS_CMD_SECT_FULL_Comp_Steel_TUB_TYPE1),
		_LS(IDS_CMD_SECT_FULL_Comp_Steel_Box_TYPE2), _LS(IDS_CMD_SECT_FULL_Comp_Steel_I_TYPE2), _LS(IDS_CMD_SECT_FULL_Comp_Steel_TUB_TYPE2),
		_LS(IDS_CMD_SECT_FULL_Comp_Composite_CI), _LS(IDS_CMD_SECT_FULL_Comp_Composite_CT), _LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_TAPERED),
		_LS(IDS_CMD_SECT_STEEL_GIRDER_BOX), _LS(IDS_CMD_SECT_STEEL_GIRDER_I), _LS(IDS_CMD_SECT_STEEL_GIRDER_MCELL)
	};
#else
#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif
	int nCount = sizeof(SectItemTapResource::aBitmapID)/sizeof(UINT);
	// MNET:XXXX-HSSHIM-20110321.
	// Gen의 General Section을 다시 살림.
	// 아래 코드로 봐서는 
	// Civil의 Composite General을 막으면서 실수로 막혔었던 것으로 보임.
	//
	//   nCount--; // 20080707 mylee - Composite General 막음
	// 
	// #if defined(_CIVIL)
	//   nCount++;
	// #endif

#if defined(_CIVIL)
#if !defined(_CH)
	if (CTestEnvMgr::IsTestEnvST(_T("MCellTest"), _T("yes")) == false)
	{
		nCount--;
	}
#endif 
#endif

	// build image list
	if(m_pImageList != NULL){ delete m_pImageList; m_pImageList = NULL; }
	m_pImageList = new CImageList;
	m_pImageList->Create(25, 25, ILC_COLOR, nCount, 1);

	// load image and add it to image list
	for (int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(SectItemTapResource::aBitmapID[i]);
		m_pImageList->Add(&bitmap, COLORREF(RGB(0,0,0)));
	}

	// Attach image list to ComboBox
	m_cboType.SetImageList(SectItemTapResource::aImgPath, nCount);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
	m_cboType.SetRedraw(FALSE);

	// Make items
	CString str;
	for (int i = 0; i < nCount; i++)
	{
		str = aItemName[i];
		if (CProduct::GetMovingType() == D_PRODUCT_MOVING_US) 
		{
			if (SectItemTapResource::nIndex[i] != 16 && SectItemTapResource::nIndex[i] != D_SECT_PSC_SHAPE_INDEX+4) 
				str.Replace('H', 'I');
		}

		//m_cboType.SetItemData(m_cboType.InsertItem(&cbi), nIndex[i]);
		m_cboType.SetItemData(m_cboType.AddString(str.GetBuffer()), SectItemTapResource::nIndex[i]);
		str.ReleaseBuffer();
	}
	m_cboType.SetRedraw(TRUE);
}

void CCMSectItemTap::SetDBNameList()
{
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);
	m_wndDB.SetRedraw(FALSE);
	for (int i = 0; i < DBNameList.GetSize(); i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBEngToRus(DBNameList[i]);
#endif
		m_wndDB.AddString(DBNameList.GetAt(i));
	}
	m_wndDB.SetRedraw(TRUE);
}

void CCMSectItemTap::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;

	CString csKey;
	csKey.Format(_T("%ld"), m_Key);
	m_wndID.SetWindowText(csKey);
}

// 대화창 내에서 컨트롤 보여지고 숨겨지는 것 여기에서 실행됨 - AlignControls 보조
void CCMSectItemTap::ShowData2Dlg()
{
	// shape
	int nShapeIndex;
	nShapeIndex = GetShapeIndexFromData(m_Data);
	SetCurShapeByIndex(nShapeIndex);
	// show/hide
	CDlgUtil::CtrlShowHide(this, m_aCtrlROctagon, m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT);
	if (m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_ROCT)
	{
		CString strCellNum;
		strCellNum.Format(_T("%d"), m_Data.SectBefore.nCellShape);
		m_editCellNum.SetWindowText(strCellNum);
	}

	// variation
	m_wndVar1.SetCurSel(m_Data.SectBefore.nLytype-1); 
	m_wndVar2.SetCurSel(m_Data.SectBefore.nLztype-1);

	// db
	SetDB(m_Data.SectBefore.SectI.DBName, m_Data.SectBefore.nStype);

	// name(db보다 나중에 설정: For AutoChangeName)
	m_wndName.SetWindowText(m_Data.SName);

	// MNET:XXXX-CJJEONG-20090309 (Tel : 2124)   
	CDlgUtil::CtrlShowHide(this, m_aCtrlOption, m_Data.SectBefore.nStype!=D_SECT_TYPE_COMPO_G);  
	CDlgUtil::CtrlShowHide(this, m_aCtrlView,   m_Data.SectBefore.nStype!=D_SECT_TYPE_COMPO_G && m_Data.SectBefore.nStype!=D_SECT_TYPE_STLG_MCELL);
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
	{
		ShowHideComposite(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->ShowWindow(FALSE);

		{
			// 위아래로 옮겨지는 컨트롤들
			CArray<UINT, UINT> m_aCtrlTmp;
			m_aCtrlTmp.Add(IDC_CMD_VAR1_STC);
			m_aCtrlTmp.Add(IDC_CMD_VAR2_STC);
			m_aCtrlTmp.Add(IDC_CMD_SP_ID_VAR1);
			m_aCtrlTmp.Add(IDC_CMD_SP_ID_VAR2);
			m_aCtrlTmp.Add(IDC_CMD_SHEAR_DEFORM);
			m_aCtrlTmp.Add(IDC_CMD_WARPING_EFFECT_CHK);

			CDlgUtil::CtrlShowHide(this, m_aCtrlComposite, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, TRUE);

			CRect rFrom, rTo;
			GetDlgItem(IDC_CMD_SP_ID_VAR1)->GetWindowRect(rFrom);
			GetDlgItem(IDC_CMD_SP_CG_BEFOREPART_CMB)->GetWindowRect(rTo);

			int nDistY = rTo.bottom - rFrom.top;
			CDlgUtil::CtrlMoveDistY(this, m_aCtrlTmp, nDistY+globalUtils.ScaleByDPI(4));

		}

		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);    

		m_wndGridComGenI.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		//m_wndGridComGenI.SetPSCDesign(m_PSCDesign.bCompPSC);
		m_wndGridComGenJ.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		//m_wndGridComGenJ.SetPSCDesign(m_PSCDesign.bCompPSC);
		m_wndGridComGenI.ShowData();
		m_wndGridComGenJ.ShowData();
		m_wndSecViewCGI.Invalidate();
		m_wndSecViewCGJ.Invalidate();

		int nSel = m_Data.SectBefore.SectI.nBeforePart-1;
		if(nSel < 0) nSel = 0;
		m_cmbBeforePart.SetCurSel(nSel);    

		Invalidate();
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER) // *^^* Value Section
	{
		ShowHideComposite(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		m_wndGridSecond.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		m_wndGridFirst.ShowData();
		m_wndGridSecond.ShowData();
	}
	else if(m_Data.SectBefore.nStype == D_SECT_TYPE_PSC)
	{
		ShowHideComposite(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, m_Data.SectBefore.Shape==D_SECT_SHAPE_PSC_CMPWEB);
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		m_wndGrid4Psc.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		m_wndGrid4Psc.ShowData();

		// warping stress point
		ShowHidePSCWarpingCheck(TRUE);

		// material data
		if (m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)   // 파형강판
			SetMatlSectData();

		// mesh size
		m_chkMeshSize.SetCheck(m_Data.SectBefore.bUserDefMeshSize);
		if (m_Data.SectBefore.bUserDefMeshSize)
			m_editMeshSize.SetEditUnit(m_Data.SectBefore.dMeshSize);
		OnCmdMeshSizeChk2();
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I       || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B  ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		ShowHideComposite(TRUE);
		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		m_wndGrid4Composite.ShowData();

		// Es/Ec - Egd/Esb
		if (m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_I          ||
			m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_TUB      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
			m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
		{
			GetDlgItem(IDC_CMD_MATL2_STC1)->SetWindowText(_T("Es/Ec"));
			GetDlgItem(IDC_CMD_MATL2_STC2)->SetWindowText(_T("Ds/Dc"));
			GetDlgItem(IDC_CMD_MATL2_STC5)->SetWindowText(_T("Ps"));
			GetDlgItem(IDC_CMD_MATL2_STC6)->SetWindowText(_T("Pc"));
			GetDlgItem(IDC_CMD_MATL2_STC3)->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_CREEP));
			GetDlgItem(IDC_CMD_MATL2_STC4)->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_STL_MULTI_SHRINKAGE));			
		}
		else
		{
			GetDlgItem(IDC_CMD_MATL2_STC1)->SetWindowText(_T("Egd/Esb"));
			GetDlgItem(IDC_CMD_MATL2_STC2)->SetWindowText(_T("Dgd/Dsb"));
			GetDlgItem(IDC_CMD_MATL2_STC5)->SetWindowText(_T("Pgd"));
			GetDlgItem(IDC_CMD_MATL2_STC6)->SetWindowText(_T("Psb"));
			GetDlgItem(IDC_CMD_MATL2_STC3)->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_CREEP));
			GetDlgItem(IDC_CMD_MATL2_STC4)->SetWindowText(_LS(IDS_WG_CMD_SECT_ITEM_CONC_MULTI_SHRINKAGE));
		}

		// material data
		SetMatlSectData();

		// multi-elastic data
		if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I       || 
			m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI      || 
			m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B  ||
			m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlCompositeMulti, TRUE);
			m_chkMat.SetCheck(m_Data.SectBefore.bMultipleElastic);
			OnCmdMultiModulusCheck();
		}
		else
		{
			m_Data.SectBefore.bMultipleElastic = FALSE; // Disable 시키면서 초기화
			m_chkMat.SetCheck(FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlCompositeMulti, FALSE);
		}
	}
	else if(m_Data.SectBefore.nStype==D_SECT_TYPE_STLG_B || m_Data.SectBefore.nStype==D_SECT_TYPE_STLG_I || m_Data.SectBefore.nStype==D_SECT_TYPE_STLG_MCELL)
	{
		ShowHideComposite(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, m_Data.SectBefore.nStype==D_SECT_TYPE_STLG_MCELL);

		m_wndGrid4StlG.ShowData();

		if(m_Data.SectBefore.nStype==D_SECT_TYPE_STLG_MCELL)
		{
			m_chkMCellSym.SetCheck(m_Data.SectBefore.bSymmetric);
			m_edtMCellNoCell.SetEditUnit(m_Data.SectBefore.nCellNum);
			CDlgUtil::CtrlRadioSetCheck(this,  m_aRdoMCellLorR, m_Data.SectBefore.nCellType);
			CDlgUtil::CtrlRadioSetCheck(this,  m_aRdoMCellFShape, m_Data.SectBefore.nFlangeShape);
			Data2DlgMCellJoint();
		}
	}
	else
	{
		ShowHideComposite(FALSE);
		GetDlgItem(IDC_CMD_SP_ID_CODE)->ShowWindow(SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aCtrlDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlFrame, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMeshSize, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMaterial, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlComposite2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		// sect I, J
		SetNameCombo();
		SetSectData();   // Title, Value 설정, Show/Hide 수행
	}

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_Data.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// consider shear deformation
	m_chkShearDeform.SetCheck((int)m_Data.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_Data.SectBefore.bConsiderWarpingEffect);

	// PSC
	m_chkPSCDesign.SetCheck((int)m_PSCDesign.bCompPSC);
	m_edtPSCShear_I.SetEditUnit(m_PSCDesign.dt1[0]);
	m_edtPSCShear_J.SetEditUnit(m_PSCDesign.dt1[1]);

	if(!CSectDB::IsEnableSectShape7thDOF(m_Data.SectBefore.Shape))
	{
		m_Data.SectBefore.bConsiderWarpingEffect = FALSE;
		m_chkWarpingEffect.ShowWindow(SW_HIDE);
	}
	else
	{
		BOOL bHide_7thDOF = FALSE;
		if (!CSectDB::IsEnable7thDOF())
			bHide_7thDOF = TRUE;

		m_chkWarpingEffect.ShowWindow(!bHide_7thDOF);
	}

	if (IsEnableSectShape7thDOF_Temp(m_Data.SectBefore.Shape, m_Data.bHSQType, m_Data.bRoundType))
	{
		m_chkWarpingEffect.EnableWindow(TRUE);
	}
	else
	{
		m_chkWarpingEffect.EnableWindow(FALSE);
	}

	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I)
	{
		m_chkHumbly.ShowWindow(SW_SHOW);
		m_chkHumbly.SetCheck((int)m_Data.SectBefore.bHumblyChk);
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		m_chkHumblyBefore.ShowWindow(SW_SHOW);
		m_chkHumblyBefore.SetCheck((int)m_Data.SectBefore.bHumblyChk);

		m_chkHumblyAfter.ShowWindow(SW_SHOW);
		m_chkHumblyAfter.SetCheck((int)m_Data.SectAfter.bHumblyChk);

	}
	else
	{
		m_chkHumbly.ShowWindow(SW_HIDE);
		m_chkHumblyBefore.ShowWindow(SW_HIDE);
		m_chkHumblyAfter.ShowWindow(SW_HIDE);
	}

	// stiffness calc. method
	ShowHideEnableDisableStiffCalcMethod();
	EnableDisableStlgMCell();

	// guide bitmap / section preview
	ChangeBitmap(); // Change guide bitmap
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();  
}

void CCMSectItemTap::Data2DlgMCellJoint()
{
	m_chkMCellJ1.SetCheck(TRUE);
	m_chkMCellJ2.SetCheck(FALSE);
	m_chkMCellJr1.SetCheck(TRUE);
	m_chkMCellJr2.SetCheck(FALSE);

	//if(m_Data.SectBefore.nJoint & 0x00000001) m_chkMCellJ1.SetCheck(TRUE);
	if(m_Data.SectBefore.nJoint & 0x00000002) m_chkMCellJ2.SetCheck(TRUE);
	//if(m_Data.SectBefore.nJoint & 0x00000004) m_chkMCellJr1.SetCheck(TRUE);
	if(m_Data.SectBefore.nJoint & 0x00000008) m_chkMCellJr2.SetCheck(TRUE);
}

void CCMSectItemTap::SetNameCombo()
{
	// Name List를 삭제
	m_wndFirstName.ResetContent();
	m_wndSecondName.ResetContent();

	CString csDBName, csShape;
	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;
	csDBName = pSectI->DBName;
	csShape = m_Data.SectBefore.Shape;

	// DB가 None이면 Disable하고 Return한다.
	if (csDBName == _T("")) 
	{
		m_wndFirstName.EnableWindow(FALSE);
		m_wndSecondName.EnableWindow(FALSE);
		return;
	}
	m_wndFirstName.EnableWindow(TRUE);
	m_wndSecondName.EnableWindow(TRUE);

	// Section name list를 가져온다.
	CArray<CString, CString&> aSectNameList;
	m_pDoc->m_pSectDB->GetSectNameList(csDBName, csShape, aSectNameList);
	m_wndFirstName.SetRedraw(FALSE);
	m_wndSecondName.SetRedraw(FALSE);
	// Section name list를 설정한다.
	int nSize = aSectNameList.GetSize();;
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, aSectNameList[i]);
#endif
		m_wndFirstName.AddString(aSectNameList.GetAt(i));  
		m_wndSecondName.AddString(aSectNameList.GetAt(i));  
	}

	CString SectNameI = pSectI->SName;
	CString SectNameJ = pSectJ->SName;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectNameI);
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(csDBName, SectNameJ);
#endif

	// 이름을 콤보에서 선택한다.
	int nSel = m_wndFirstName.FindStringExact(-1, SectNameI);
	if(nSel == CB_ERR) m_wndFirstName.SelectString(-1, SectNameI);
	else m_wndFirstName.SetCurSel(nSel);
	nSel = m_wndSecondName.FindStringExact(-1, SectNameJ);
	if (nSel == CB_ERR) m_wndSecondName.SelectString(-1, SectNameJ);
	else m_wndSecondName.SetCurSel(nSel);

	m_wndFirstName.SetRedraw(TRUE);
	m_wndSecondName.SetRedraw(TRUE);
}

void CCMSectItemTap::SetSectData()
{
	int nShapeIndex;
	CStringArray aSizeTitle;
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC) return;
	if (m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_B        || m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_I  ||
		m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_TUB      || m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_CI || 
		m_Data.SectBefore.nStype==D_SECT_TYPE_COMPO_CT       || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB) return;
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER) return; 
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_tapered_section_s));
		return;
	}
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;
	BOOL bEnable = (pSectI->DBName == _T("")) ? TRUE : FALSE;
	CString csValue;
	int nSizeNum = aSizeTitle.GetSize();
	int i = 0;
	for (i = 0;i < nSizeNum; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_SHOW);
		m_wndFirstSize[i].ShowWindow(SW_SHOW);
		m_wndFirstUnit[i].ShowWindow(SW_SHOW);
		m_wndFirstTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSectI->Size[i]);
		m_wndFirstSize[i].SetWindowText(csValue);
		m_wndFirstSize[i].EnableWindow(bEnable);

		m_wndSecondTitle[i].ShowWindow(SW_SHOW);
		m_wndSecondSize[i].ShowWindow(SW_SHOW);
		m_wndSecondUnit[i].ShowWindow(SW_SHOW);
		m_wndSecondTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), pSectJ->Size[i]);
		m_wndSecondSize[i].SetWindowText(csValue);
		m_wndSecondSize[i].EnableWindow(bEnable);
	}
	for (; i < 8; i++)
	{
		m_wndFirstTitle[i].ShowWindow(SW_HIDE);
		m_wndFirstSize[i].ShowWindow(SW_HIDE);
		m_wndFirstUnit[i].ShowWindow(SW_HIDE);

		m_wndSecondTitle[i].ShowWindow(SW_HIDE);
		m_wndSecondSize[i].ShowWindow(SW_HIDE);
		m_wndSecondUnit[i].ShowWindow(SW_HIDE);
	}
	// 임시(KJH20040119) => JIS, JIS2K round(r1, r2)
	// User type && Channel이면 r1, r2는 disable
	int nValUsrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nValUsrDB);
	if (nValUsrDB == 2) m_wndDB.EnableWindow(TRUE);
	else m_wndDB.EnableWindow(FALSE);

	if(nShapeIndex==1 && (nValUsrDB<2))
	{
		for(i=6; i<8; i++)
		{
			pSectI->Size[i] = 0.;
			pSectJ->Size[i] = 0.;
			if(nValUsrDB==1)
			{
				m_wndFirstSize[i].SetEditUnit(0.);
				m_wndFirstSize[i].EnableWindow(FALSE);
				m_wndSecondSize[i].SetEditUnit(0.);
				m_wndSecondSize[i].EnableWindow(FALSE);
			}
		}
	}

	if (nValUsrDB < 2 && nShapeIndex == 18)  // ROCT
	{
		m_wndFirstSize[6].EnableWindow(m_Data.SectBefore.nCellShape != 1);
		m_wndSecondSize[6].EnableWindow(m_Data.SectBefore.nCellShape != 1);    
		if (m_Data.SectBefore.nCellShape == 1)
		{
			pSectI->Size[6] = 0.;
			pSectJ->Size[6] = 0.;
			m_wndFirstSize[6].SetEditUnit(0.);
			m_wndSecondSize[6].SetEditUnit(0.);
		}
	}
}

void CCMSectItemTap::ChangeBitmap()
{
	int nShapeIndex;
	nShapeIndex = GetShapeIndexFromData(m_Data);
	nShapeIndex = GetCobxIndexByShapeIndex(nShapeIndex);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_tapered_section_s));
		return;
	}

#if defined(_MGEN) || defined(_TOWER)
	UINT aBitmapID[] = {
		//IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP19, IDB_CMD_SP_ISGL_BMP20,
		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
		IDB_CMD_SP_ISGL_BMP10, IDB_CMD_SP_ISGL_BMP25,
	};
	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\Sec01.svg"),
		_T("SVG\\illustration\\Dialog\\Sec19.svg"),
		_T("SVG\\illustration\\Dialog\\Sec20.svg"),
		_T("SVG\\illustration\\Dialog\\Sec04.svg"),
		_T("SVG\\illustration\\Dialog\\Sec05.svg"),
		_T("SVG\\illustration\\Dialog\\Sec06.svg"),
		_T("SVG\\illustration\\Dialog\\Sec07.svg"),
		_T("SVG\\illustration\\Dialog\\Sec08.svg"),
		_T("SVG\\illustration\\Dialog\\Sec09.svg"),
		_T("SVG\\illustration\\Dialog\\Sec10.svg"),
		_T("SVG\\illustration\\Dialog\\Sec25.svg"),
	};
#elif defined(_CIVIL)
	UINT aBitmapID[] = {
		//IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP19, IDB_CMD_SP_ISGL_BMP20,
		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
		IDB_CMD_SP_ISGL_BMP10, 
		IDB_CMD_SP_ISGL_BMP13, IDB_CMD_SP_ISGL_BMP14, /*IDB_CMD_SP_ISGL_BMP21,*/
		IDB_CMD_SP_ISGL_BMP15, IDB_CMD_SP_ISGL_BMP16, IDB_CMD_SP_ISGL_BMP17,
		IDB_CMD_SP_ISGL_BMP25,
		IDB_CMD_SP_IPSC_BMP1, IDB_CMD_SP_IPSC_BMP2, IDB_CMD_SP_IPSC_BMP3, 
		IDB_CMD_SP_IPSC_BMP9, IDB_CMD_SP_IPSC_BMP10,
		IDB_CMD_SP_IPSC_BMP4, IDB_CMD_SP_IPSC_BMP8, 
		IDB_CMD_SP_IPSC_BMP5, IDB_CMD_SP_IPSC_BMP6, IDB_CMD_SP_IPSC_BMP7, 
		IDB_CMD_SP_IPSC_BMP11, IDB_CMD_SP_IPSC_BMP12,
		IDB_CMD_SP_COMP_BMP01, IDB_CMD_SP_COMP_BMP02, IDB_CMD_SP_COMP_BMP05, 
		IDB_CMD_SEC_COMP_STL_GIRDER_BOX, IDB_CMD_SEC_COMP_STL_GIRDER_I, IDB_CMD_SEC_COMP_STL_GIRDER_TUB,
		IDB_CMD_SP_COMP_BMP03, IDB_CMD_SP_COMP_BMP04, IDB_CMD_SP_ISGL_BMP25,      
		IDB_CMD_SEC_STL_GIRDER_BOX, IDB_CMD_SEC_STL_GIRDER_I
	};

	CString aImgPath_Sect[] = {
		_T("SVG\\illustration\\Dialog\\Sec01.svg"),
		_T("SVG\\illustration\\Dialog\\Sec19.svg"),
		_T("SVG\\illustration\\Dialog\\Sec20.svg"),
		_T("SVG\\illustration\\Dialog\\Sec04.svg"),
		_T("SVG\\illustration\\Dialog\\Sec05.svg"),
		_T("SVG\\illustration\\Dialog\\Sec06.svg"),
		_T("SVG\\illustration\\Dialog\\Sec07.svg"),
		_T("SVG\\illustration\\Dialog\\Sec08.svg"),
		_T("SVG\\illustration\\Dialog\\Sec09.svg"),
		_T("SVG\\illustration\\Dialog\\Sec10.svg"),
		_T("SVG\\illustration\\Dialog\\Sec13.svg"),
		_T("SVG\\illustration\\Dialog\\Sec14.svg"),
		_T("SVG\\illustration\\Dialog\\Sec15.svg"),
		_T("SVG\\illustration\\Dialog\\Sec16.svg"),
		_T("SVG\\illustration\\Dialog\\Sec17.svg"),
		_T("SVG\\illustration\\Dialog\\Sec25.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc01.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc02.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc03.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc09.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc10.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc04.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc08.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc05.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc06.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc07.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc11.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_psc12.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp01.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp02.svg"),
		_T("SVG\\illustration\\Dialog\\SectComp03.svg"),
		_T("SVG\\illustration\\Dialog\\SectCompStlGirder_Box.svg"),
		_T("SVG\\illustration\\Dialog\\SectCompStlGirder_I.svg"),
		_T("SVG\\illustration\\Dialog\\SectCompStlGirder_Tub.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_ci.svg"),
		_T("SVG\\illustration\\Dialog\\cmd_sp_ct.svg"),
		_T("SVG\\illustration\\Dialog\\Sec25.svg"),
		_T("SVG\\illustration\\Dialog\\SectStlGirder_Box.svg"),
		_T("SVG\\illustration\\Dialog\\SectStlGirder_I.svg"),
		_T("SVG\\illustration\\Dialog\\SectStlGirder_I.svg"), // MCELL 용은 필요없으나 동작을 위해 채워둠
		};

#else
#error _LS(IDS_CMD0417__Unknown_Product_Definition)
#endif

	// set appropriate bitmap
	if(m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
// 	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
// 	m_pBitmap = (CBitmap*)SecImage;
// 	m_pBitmap = new CBitmap;
// 	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT) // ROCT Shape
// 	{
// 		CString csCellNum;
// 		m_editCellNum.GetWindowText(csCellNum);
// 		UINT nBitmap = (_ttoi(csCellNum) == 1) ? IDB_CMD_SP_ISGL_BMP21 : IDB_CMD_SP_ISGL_BMP22;
// 		m_pBitmap->LoadBitmap(nBitmap);
// 	}
// 	else m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
// 	SecImage->SetBitmap(HBITMAP(*m_pBitmap));

	if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT) // ROCT Shape
	{
		CString csCellNum;
		m_editCellNum.GetWindowText(csCellNum);
		CString csImagePath = (_ttoi(csCellNum) == 1) ? _T("SVG\\illustration\\Dialog\\Sec21.svg") : _T("SVG\\illustration\\Dialog\\Sec22.svg");
		m_ImgSectItem.SetImage(csImagePath);
	}
	else
	{
		m_ImgSectItem.SetImage(aImgPath_Sect[nShapeIndex]);
	}
}

void CCMSectItemTap::DoActionWhenDBChanged()
{
	CString csDB = GetDB();
	//if (csDB == m_Data.SectBefore.SectI.DBName) return;

	T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
	T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;

	if (csDB != _T("")) 
	{
		pSectI->Initialize();
		pSectJ->Initialize();
		// change new DB name
		pSectI->DBName = csDB;
		pSectJ->DBName = csDB;
	}
	else // DB가 None으로 변경되면 Size는 유지

	{
		pSectI->DBName = _T("");
		pSectJ->DBName = _T("");
	}

	// Dialog에 반영한다.
	SetNameCombo();
	SetSectData();

	// Update preview
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

int CCMSectItemTap::GetShapeIndexFromData(T_SECT_D& rData)
{
	if (rData.SectBefore.nStype == D_SECT_TYPE_PSC)
	{
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL)
			return D_SECT_PSC_SHAPE_INDEX;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL)
			return D_SECT_PSC_SHAPE_INDEX+1;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_3CELL)
			return D_SECT_PSC_SHAPE_INDEX+2;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_4CELL)
			return D_SECT_PSC_SHAPE_INDEX+8;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2)
			return D_SECT_PSC_SHAPE_INDEX+9;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_MID)
			return D_SECT_PSC_SHAPE_INDEX+3;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF)
			return D_SECT_PSC_SHAPE_INDEX+4;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_T)
			return D_SECT_PSC_SHAPE_INDEX+5;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_BOX)
			return D_SECT_PSC_SHAPE_INDEX+6;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_I)
			return D_SECT_PSC_SHAPE_INDEX+7;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
			return D_SECT_PSC_SHAPE_INDEX+10;
		if (rData.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)
			return D_SECT_PSC_SHAPE_INDEX+11;
		ASSERT(0);
		return 0;
	}
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_B)          return D_SECT_COMPO_SHAPE_INDEX;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_I)          return D_SECT_COMPO_SHAPE_INDEX+1;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_CI)         return D_SECT_COMPO_SHAPE_INDEX+2;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_CT)         return D_SECT_COMPO_SHAPE_INDEX+3;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_G)          return D_SECT_COMPO_SHAPE_INDEX+5;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB)        return D_SECT_COMPO_SHAPE_INDEX+6;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B)     return D_SECT_COMPO_SHAPE_INDEX+7;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I)     return D_SECT_COMPO_SHAPE_INDEX+8;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)   return D_SECT_COMPO_SHAPE_INDEX+9;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_STLG_B)           return D_SECT_STLG_SHAPE_INDEX;
	else if (rData.SectBefore.nStype == D_SECT_TYPE_STLG_I)           return D_SECT_STLG_SHAPE_INDEX+1;  
	else if (rData.SectBefore.nStype == D_SECT_TYPE_STLG_MCELL)       return D_SECT_STLG_SHAPE_INDEX+2;  

	else return CSectUtil::GetShapeIndexFromNameReg(rData.SectBefore.Shape);
}

BOOL CCMSectItemTap::IsValueSect()
{
	if (IsPscSect()) return FALSE;

	int nValUsrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nValUsrDB);
	if (nValUsrDB == 0) return TRUE;
	return FALSE;
}

BOOL CCMSectItemTap::IsPscSect()
{
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex >= D_SECT_PSC_SHAPE_INDEX && nShapeIndex < D_SECT_COMPO_SHAPE_INDEX) return TRUE;
	return FALSE;
}

BOOL CCMSectItemTap::IsCompositeSect()
{
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex >= D_SECT_COMPO_SHAPE_INDEX && nShapeIndex < D_SECT_STLG_SHAPE_INDEX) return TRUE;
	return FALSE;
}

BOOL CCMSectItemTap::IsStlgSect()
{
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex >= D_SECT_STLG_SHAPE_INDEX) return TRUE;
	return FALSE;
}

BOOL CCMSectItemTap::IsPSCValue()
{
	return (m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE);
}

BOOL CCMSectItemTap::IsGeneralSect()
{
	return (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_GEN ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G);
}

CString CCMSectItemTap::GetDB()
{
	CString csDB;
	int nValUsrDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nValUsrDB);
	if (nValUsrDB == 2) m_wndDB.GetWindowText(csDB);
	else csDB = _T("");

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBRusToEng(csDB);
#endif

	return csDB;
}

void CCMSectItemTap::SetDB(CString &csDBName, int nStype)
{
	BOOL bEnableRadio;
	BOOL bEnableDB;
	if (nStype == D_SECT_TYPE_USER) // *^^* Value Section
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
		if(m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN)
		{
			bEnableRadio = FALSE;
		}
		else
		{
			bEnableRadio=TRUE;
		}
		bEnableDB = FALSE;
	}
	else if (nStype == D_SECT_TYPE_PSC) // PSC -> user
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		bEnableRadio = FALSE;
		bEnableDB = FALSE;
	}
	else if (nStype == D_SECT_TYPE_COMPO_B   || nStype == D_SECT_TYPE_COMPO_I  || 
		nStype == D_SECT_TYPE_COMPO_TUB || nStype == D_SECT_TYPE_COMPO_CI || 
		nStype == D_SECT_TYPE_COMPO_CT  || nStype == D_SECT_TYPE_COMPO_STLG_B   ||
		nStype == D_SECT_TYPE_COMPO_STLG_I || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		bEnableRadio = FALSE;
		bEnableDB = FALSE;
	}
	else if (nStype == D_SECT_TYPE_COMPO_G)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 0);
		bEnableRadio = FALSE;
		bEnableDB = FALSE;
	}
	else if (nStype == D_SECT_TYPE_STLG_B || nStype == D_SECT_TYPE_STLG_I || nStype == D_SECT_TYPE_STLG_MCELL)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
		bEnableRadio = FALSE;
		bEnableDB = FALSE;
	}
	else
	{
		bEnableRadio = TRUE;
		if (csDBName == _T("")) // DB None -> user
		{
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 1);
			bEnableDB = FALSE;
		}
		else  // use DB
		{
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDB, 2);
			bEnableDB = TRUE;
			CString csDB = csDBName;
#if defined(_RUS)
			CSectDBRussiaUtil::ConvertSectDBEngToRus(csDB);
#endif
			int nSel = m_wndDB.FindStringExact(-1, csDB);
			if (nSel == CB_ERR) m_wndDB.SelectString(-1, csDB);
			else m_wndDB.SetCurSel(nSel);
		}
	}

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDB, bEnableRadio);
	m_wndDB.EnableWindow(bEnableDB);
}

int CCMSectItemTap::GetCurShapeIndex()
{
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) return LB_ERR;

	nShapeIndex = (int)m_cboType.GetItemData(nShapeIndex);
	return nShapeIndex;
}

void CCMSectItemTap::SetCurShapeByIndex(int nShapeIndex)
{
	int nCount = m_cboType.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		if ((int)m_cboType.GetItemData(i) == nShapeIndex)
			break;
	}
	nShapeIndex = i;

	m_cboType.SetCurSel(nShapeIndex);
	//CDlgUtil::CobxSetCurSelItemData(m_cboType, nShapeIndex);
}

int CCMSectItemTap::GetCobxIndexByShapeIndex(int nShapeIndex)
{
	int nCount = m_cboType.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		if ((int)m_cboType.GetItemData(i) == nShapeIndex)
			break;
	}
	if (i == nCount) return LB_ERR;
	return i;
}

//////////////////////////////////////////////////////////////////////////////////
// PSC Viewer 연결
void CCMSectItemTap::CreatePSCView() // Composite 도 함께 사용
{
	// Window가 이미 있으면 만들지 않는다.
	if(IsViewWindow()) return;

	m_pViewer = new CCMSectItmePSCView();
	m_pViewer->SetInitPos(D_INIT_POS_RT);
	m_pViewer->Create(IDD_CMD_SECT_ITEM_PSC_VIEW, this);
	m_pViewer->ShowWindow(SW_SHOW);

	// View를 숨기고 다이얼로그 크기 조절 
	CWnd* pWnd;
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_PSC_VIEW_STATIC); ASSERT(pWnd);
	pWnd->ShowWindow(SW_HIDE);
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_VIEW_FRM); ASSERT(pWnd);
	pWnd->ShowWindow(SW_HIDE);

	CRect rectLast, r;
	pWnd = m_pViewer->GetDlgItem(IDC_CMD_PICTURE); ASSERT(pWnd);
	pWnd->GetWindowRect(rectLast);
	m_pViewer->GetWindowRect(r);
	r.bottom = rectLast.bottom+10;
	m_pViewer->MoveWindow(r);
}

void CCMSectItemTap::DestroyPSCView()
{
	// Window가 있으면 없앤다.
	if(m_pViewer != NULL)
	{
		m_pViewer->DestroyWindow();
		m_pViewer = NULL;    
	}
}

BOOL CCMSectItemTap::IsViewWindow()
{ 
	if(m_pViewer == NULL) return FALSE;
	HWND hWnd = m_pViewer->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;

	return TRUE;
}

BOOL CCMSectItemTap::ChangeBitmapForViewer(UINT nIndex)
{
	if(!IsViewWindow()) return FALSE;

	return m_pViewer->ChangeBitmap(nIndex); 
}

void CCMSectItemTap::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_Data.SectBefore.dWarpingCheckPosI[i][j] = dWarpingCheckPosI[i][j];
			m_Data.SectBefore.dWarpingCheckPosJ[i][j] = dWarpingCheckPosJ[i][j]; 
		}
	}
}

struct BITMAP_INDEX
{
	TCHAR* pShape;
	UINT nIndex;
};

UINT CCMSectItemTap::GetBitmapIndexFromInfoPSC(int nParam/*=0*/)
{
	if(m_Data.SectBefore.nStype != D_SECT_TYPE_PSC) return -1;

	UINT nIndex = -1;
	CString strShape = m_Data.SectBefore.Shape;
	int nCellType = m_Data.SectBefore.nCellType;
	int nCellShape =  m_Data.SectBefore.nCellShape;

	if(strShape == D_SECT_SHAPE_PSC_MID)
	{
		UINT aIndex[] = {
			__SECT_PSC_MID_NONE_NONE__, __SECT_PSC_MID_NONE_CIRC__, __SECT_PSC_MID_NONE_POLY__,
			__SECT_PSC_MID_CIRC_NONE__, __SECT_PSC_MID_CIRC_CIRC__, __SECT_PSC_MID_CIRC_POLY__,
			__SECT_PSC_MID_POLY_NONE__, __SECT_PSC_MID_POLY_CIRC__, __SECT_PSC_MID_POLY_POLY__,
		};
		int nIx = 3 * nCellType + nCellShape;
		if(nIx < sizeof(aIndex)/sizeof(UINT)) nIndex = aIndex[nIx];
	}
	else if(strShape == D_SECT_SHAPE_PSC_HALF)
	{
		UINT aIndex[] = {
			__SECT_PSC_HLF_LEFT_NONE__, __SECT_PSC_HLF_LEFT_CIRC__, __SECT_PSC_HLF_LEFT_POLY__,
			__SECT_PSC_HLF_RGHT_NONE__, __SECT_PSC_HLF_RGHT_CIRC__, __SECT_PSC_HLF_RGHT_POLY__,
		};
		int nIx = 3 * nCellType + nCellShape;
		if(nIx < sizeof(aIndex)/sizeof(UINT)) nIndex = aIndex[nIx];
	}
	else if(strShape == D_SECT_SHAPE_PSC_BOX)
	{
		UINT aIndex[] = {
			__SECT_PSC_PLAT_HALF_LEFT__, __SECT_PSC_PLAT_HALF_RGHT__, __SECT_PSC_PLAT_1CEL_CIRC__,
			__SECT_PSC_PLAT_1CEL_POLY__, __SECT_PSC_PLAT_2CEL__, 
		};
		int nIx;
		if(nCellType == 0) nIx = nCellShape;
		else if(nCellType == 1) nIx = nCellShape+1;
		else if(nCellType == 2) nIx = 4;
		if(nIx < sizeof(aIndex)/sizeof(UINT)) nIndex = aIndex[nIx];
	}
	else if (strShape == D_SECT_SHAPE_PSC_NCEL2)
	{
		if (nCellType == 0)
		{
			if (nCellShape < 2) nIndex = __SECT_PSC_nCELL_POLY1_LEFT__;
			else nIndex = __SECT_PSC_nCELL_POLY2_LEFT__;
		}
		else nIndex = __SECT_PSC_nCELL_CIRC_LEFT__;
	}
	else 
	{
		BITMAP_INDEX aIndex[] = {
			{ D_SECT_SHAPE_PSC_1CELL  , __SECT_PSC_NML_1_CELL__ },
			{ D_SECT_SHAPE_PSC_2CELL  , __SECT_PSC_NML_2_CELL__ }, 
			{ D_SECT_SHAPE_PSC_3CELL  , __SECT_PSC_NML_3_CELL__ },
			{ D_SECT_SHAPE_PSC_T      , __SECT_PSC_TEE__        },
			{ D_SECT_SHAPE_PSC_I      , __SECT_PSC_I__          },
			{ D_SECT_SHAPE_PSC_4CELL  , __SECT_PSC_NML_4_CELL__ },
			{ D_SECT_SHAPE_PSC_VALUE  , __SECT_PSC_VALUE__      },
			{ D_SECT_SHAPE_PSC_CMPWEB , __SECT_PSC_CMPWEB_TAP__ },
		};
		int nCount = sizeof(aIndex)/sizeof(BITMAP_INDEX);
		int i = 0;
		for( i=0; i<nCount; i++)
			if(aIndex[i].pShape == strShape) break;
		ASSERT(i != nCount);
		nIndex = aIndex[i].nIndex;
	}

	return nIndex;
}

UINT CCMSectItemTap::GetBitmapIndexFromInfoComposite(int nParam/*=0*/) 
{
	int nIndex = 0;
	if      (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_B) ASSERT(0);
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_I) ASSERT(0);
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB) ASSERT(0);
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CI) nIndex = __SECT_PSC_I__;
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CT) nIndex = __SECT_PSC_TEE__;
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B) nIndex = -1;
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I) nIndex = -1;
	else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB) nIndex = -1;

	return nIndex; 
}

//////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CCMSectItemTap, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemTap)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_TYPE, OnChangeShape)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME1, OnChangeFirstName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CBO_NAME2, OnChangeSecondName)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_CODE, OnChangeDB)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE11, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE21, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_DB_RADIO, OnChangeUseDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_VAR1, OnChangeVar1)
	ON_CBN_SELCHANGE(IDC_CMD_SP_ID_VAR2, OnChangeVar2)
	ON_EN_SETFOCUS(IDC_CMD_SP_ID_NAME, OnSetfocusCmdSpIdName)
	ON_EN_UPDATE(IDC_CMD_SP_ID_NAME, OnUpdateCmdSpIdName)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_EDIT_CELL_NUM, OnChangeCmdEditCellNum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SPIN_CELL_NUM, OnDeltaposCmdSpinCellNum)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_MATL_BTN, OnCmdSelMaterialBtn)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE12, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE13, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE14, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE15, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE16, OnChangeFirstSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE22, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE23, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE24, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE25, OnChangeSecondSize)
	ON_EN_CHANGE(IDC_CMD_SP_ID_VALUE26, OnChangeSecondSize)
	ON_BN_CLICKED(IDC_CMD_SP_ID_USER_RADIO, OnChangeUseDB)
	ON_BN_CLICKED(IDC_CMD_SP_ID_VALUE_RADIO, OnChangeUseDB)
	ON_BN_CLICKED(IDC_CMD_MESH_SIZE_CHK2, OnCmdMeshSizeChk2)
	ON_EN_CHANGE(IDC_CMD_MATL_MVALUE1, OnChangeMaterial)
	ON_BN_CLICKED(IDC_CMD_FEM_RDO, OnCmdFemRdo)
	ON_BN_CLICKED(IDC_CMD_EQU_RDO, OnCmdFemRdo)
	ON_BN_CLICKED(IDC_CMD_MATL2_BTN, OnMaterialBtn)
	ON_EN_CHANGE(IDC_CMD_MATL2_EDT1, OnChangeCmdSpIdEsec)
	ON_EN_CHANGE(IDC_CMD_MATL2_EDT2, OnChangeCmdSpIdDsdc)
	ON_EN_CHANGE(IDC_CMD_MATL2_EDT5, OnChangeCmdSpIdPs)
	ON_EN_CHANGE(IDC_CMD_MATL2_EDT6, OnChangeCmdSpIdPc)
	ON_BN_CLICKED(IDC_CMD_MATL2_CHK, OnCmdMultiModulusCheck)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BTN_I, OnComGenImportSPCI)
	ON_BN_CLICKED(IDC_CMD_SP_ID_BTN_J, OnComGenImportSPCJ)
	ON_CBN_SELCHANGE(IDC_CMD_SP_CG_PART_CMB, OnSelchangeCmdPartSel)
	ON_BN_CLICKED(IDC_CMD_CALC_SECT_PROP_I_BTN, OnCmdCalcSectPropIBtn)
	ON_BN_CLICKED(IDC_CMD_CALC_SECT_PROP_J_BTN, OnCmdCalcSectPropJBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK, OnChkWarpingEffect)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK, OnChkhumbly)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK_BF, OnChkhumblyBefore)
	ON_BN_CLICKED(IDC_CMD_HUMBLY_CHK_AF, OnChkhumblyAfter)
	ON_BN_CLICKED(IDC_GENERAL_PSC_DESIGN_CHK, OnChkPSCDesign)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,       OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_AUTO_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_USER_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_SYM_CHK,      OnChangeMCellConnectSize)  
	ON_EN_CHANGE(IDC_CMD_SECT_MCELL_NO_CELL_EDT,   OnChangeMCellConnectSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_SECT_MCELL_NO_CELL_SPN, OnSpin) 
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_LEFT_RDO,     OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_RIGHT_RDO,    OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_GEN_RDO,      OnChangeMCellFShale)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_REBAR1_RDO,   OnChangeMCellFShale)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_REBAR2_RDO,   OnChangeMCellFShale)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_CLOSED_RDO,   OnChangeMCellFShale)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_J1_CHK,       OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_J2_CHK,       OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_JR1_CHK,      OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_MCELL_JR2_CHK,      OnChangeMCellConnectSize)  
	ON_BN_CLICKED(IDC_CMD_SECT_CHANGE_BTN,		   OnCmdSectChangeBtn)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CCMSectItemGrid::WM_GRID_DATA_CHANGED,OnGridDataChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemTap message handlers
BOOL CCMSectItemTap::OnInitDialog() 
{
	CCMSectItemBase::OnInitDialog();

	SetRedraw(FALSE);

	InitControls(); // 1번만 초기화 해주면 되는 것 들을 초기화 한다.
	SetGeneralPartList();

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)
			m_Key = m_FixID;
		else
			m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();

		m_Data.Initialize();
		m_PSCDesign.Initialize();
		m_Data.nStype = D_SECT_TYPE_TAPERED;
		m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR;
		m_Data.SectBefore.Shape = D_SECT_TAPERED_DEF_SHAPE;
		m_Data.SectBefore.nLytype = 1;
		m_Data.SectBefore.nLztype = 1;
		T_SECT_SECTBASE_D* pSectI = &m_Data.SectBefore.SectI;
		T_SECT_SECTBASE_D* pSectJ = &m_Data.SectBefore.SectJ;

		// init db value
		T_PREFERENCE pref;
		m_pDoc->m_pInitCtrl->GetPreference(pref);
		pSectI->DBName = pref.Property.SectionDBName;
		pSectJ->DBName = pSectI->DBName;
	}

	if(m_bModify)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetSecp(m_Key, m_PSCDesign)) m_PSCDesign.Initialize();
	}

	m_wndSecView.SetDataSource(&m_Data);
	m_wndGridFirst.SetDataSource(&m_Data.SectBefore.SectI, 0, D_SECT_TYPE_USER, &m_Data.SectBefore.nCellShape);
	m_wndGridSecond.SetDataSource(&m_Data.SectBefore.SectJ, 0, D_SECT_TYPE_USER, &m_Data.SectBefore.nCellShape);
	m_wndSecViewCGI.SetDataSource(&m_Data);
	m_wndSecViewCGJ.SetDataSource(&m_Data);
	m_wndSecViewCGJ.SetComGenJFlag(TRUE);
	/*m_wndGridComGenI.SetDataSource2(&m_Data.SectBefore.SectI.aGeneralPart[nPart].Stiffness,
	&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriOut,
	&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriIn,
	&m_Data.SectBefore.SectI.aGeneralPart[nPart].YBar,
	&m_Data.SectBefore.SectI.aGeneralPart[nPart].ZBar,
	&m_Data.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
	m_wndGridComGenI.SetDataSource2(&m_Data.SectBefore.SectJ.aGeneralPart[nPart].Stiffness,
	&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriOut,
	&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriIn,
	&m_Data.SectBefore.SectJ.aGeneralPart[nPart].YBar,
	&m_Data.SectBefore.SectJ.aGeneralPart[nPart].ZBar,
	&m_Data.SectBefore.SectJ, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);*/
	m_wndGridComGenI.SetDataSource(&m_Data.SectAfter.SectI, 0, D_SECT_TYPE_COMPO_G, &m_Data.SectAfter.nCellShape);
	m_wndGridComGenJ.SetDataSource(&m_Data.SectAfter.SectJ, 0, D_SECT_TYPE_COMPO_G, &m_Data.SectAfter.nCellShape);
	m_wndGrid4Psc.SetDataSource(&m_Data, this);
	m_wndGrid4Composite.SetDataSource(&m_Data, this);
	m_wndGrid4StlG.SetDataSource(&m_Data, this);
	int nShapeIndex;
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC)
	{
		nShapeIndex = CSectUtil::GetShapeIndexFromNamePsc(m_Data.SectBefore.Shape);
		m_wndGrid4Psc.ChangeShape(nShapeIndex);

		// warping stress point
		m_chkWarpingEffect.SetCheck((int)m_Data.SectBefore.bConsiderWarpingEffect);
		ShowHidePSCWarpingCheck(TRUE);

		// 추가 *^^*
		CreatePSCView();
		int nIndex = GetBitmapIndexFromInfoPSC();   // 처음엔 무조건 Left 기준 
		if(nIndex>=0) ChangeBitmapForViewer(nIndex);
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I       || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B  ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		if      (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_B) nShapeIndex = 0;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_I) nShapeIndex = 1;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CI) nShapeIndex = 2;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CT) nShapeIndex = 3;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB) nShapeIndex = 6;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B) nShapeIndex = 7;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I) nShapeIndex = 8;
		else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB) nShapeIndex = 9;
		m_wndGrid4Composite.ChangeShape(nShapeIndex, TRUE);

		if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT)
		{
			CreatePSCView();
			int nIndex = GetBitmapIndexFromInfoComposite();   
			if(nIndex>=0) ChangeBitmapForViewer(nIndex);
		}
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
	{
		m_wndGridComGenI.ChangeSectType(D_SECT_TYPE_COMPO_G);
		m_wndGridComGenJ.ChangeSectType(D_SECT_TYPE_COMPO_G);
		m_wndGridComGenI.ChangeShape(21);
		m_wndGridComGenJ.ChangeShape(21);
	}
	else if ( m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_B ||
		m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_I ||
		m_Data.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL)
	{
		nShapeIndex = CSectUtil::GetShapeIndexFromNameStlGirder(m_Data.SectBefore.Shape);
		m_wndGrid4StlG.ChangeShape(nShapeIndex);
	}
	else
	{
		m_wndGridFirst.ChangeSectType(D_SECT_TYPE_USER);
		m_wndGridSecond.ChangeSectType(D_SECT_TYPE_USER);
		nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
		m_wndGridFirst.ChangeShape(nShapeIndex);
		m_wndGridSecond.ChangeShape(nShapeIndex);
		if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER)
		{
			m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			m_wndGridSecond.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		}
	}

	ShowKey2Dlg();
	ShowData2Dlg();
	OnChkPSCDesign();
	CreateOrDestoryStlgView();
	
	BOOL bUseCentroidBtn 
		= (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC          || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI     ||  m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT    ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_STLG_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_STLG_MCELL);
	GetDlgItem(IDC_CMD_SECT_CENTROID_BTN)->EnableWindow(bUseCentroidBtn);

	// 초기화 되었음을 표시
	m_bInitDone = TRUE;

	if (m_bDesign) m_wndID.EnableWindow(FALSE);

	if (m_bModify)
	{
		// Modify 이면 Import Flag 는 모두 TRUE
		memset(&m_bIsCGImport, TRUE, sizeof(m_bIsCGImport));
	}

	m_chkMCellJ1.SetCheck(TRUE);
	m_chkMCellJr1.SetCheck(TRUE);
	GetDlgItem(IDC_CMD_SECT_CHANGE_BTN)->ShowWindow(FALSE);
#ifdef _MGEN_CH
	GetDlgItem(IDC_CMD_SECT_CHANGE_BTN)->ShowWindow(TRUE);
#endif

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}

void CCMSectItemTap::OnChangeShape() 
{
	// TODO: Add your control notification handler code here

	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) return;

	int nCurShapeIndex = GetShapeIndexFromData(m_Data);
	if (nShapeIndex == nCurShapeIndex) return;
	ShowHideComposite(IsCompositeSect() && !IsGeneralSect()); 

	int nPrevStype = m_Data.SectBefore.nStype;               
	if( IsPscSect () ) 
	{
		m_Data.SectBefore.nStype = D_SECT_TYPE_PSC;
		m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexPsc(nShapeIndex - D_SECT_PSC_SHAPE_INDEX);
		m_Data.SectBefore.SectI.DBName = _T("");  // db는 none으로
		m_Data.SectBefore.SectJ.DBName = _T("");
		m_Data.SectBefore.nCellType = 0;
		m_Data.SectBefore.nCellShape = 0;
		m_Data.SectBefore.nJoint = 0;
		m_Data.SectBefore.SectI.Initialize();
		m_Data.SectBefore.SectJ.Initialize();
		m_Data.SectBefore.bUserInputStiffness = FALSE;

		// PSC VIEW 처리
		if( !(nPrevStype==D_SECT_TYPE_PSC || nPrevStype==D_SECT_TYPE_COMPO_CI || nPrevStype==D_SECT_TYPE_COMPO_CT) ) 
			CreatePSCView();
		int nIndex = GetBitmapIndexFromInfoPSC();
		if(nIndex>=0) ChangeBitmapForViewer(nIndex);
	}
	else if ( IsCompositeSect() )
	{
		// Type, Shape Setting
		int nTmp = nShapeIndex-D_SECT_COMPO_SHAPE_INDEX;
		if (nTmp==0)      { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_B; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_B; }
		else if (nTmp==1) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_I; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_I; }
		else if (nTmp==2) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_CI; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_CI; }
		else if (nTmp==3) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_CT; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_CT; }
		else if (nTmp==5) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G; }
		else if (nTmp==6) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_TUB; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_TUB; }
		else if (nTmp==7) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_B; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_B; }
		else if (nTmp==8) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_I; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_I; }
		else if (nTmp==9) { m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_STLG_TUB; m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_STLG_TUB; }

		m_Data.SectBefore.SectI.DBName = _T("");  // db는 none으로
		m_Data.SectBefore.SectJ.DBName = _T("");
		m_Data.SectBefore.nCellType = 0;
		m_Data.SectBefore.nCellShape = 0;
		m_Data.SectBefore.nJoint = 0;
		m_Data.SectBefore.bSymmetric = TRUE; // symmetric 은 TRUE 로
		m_Data.SectBefore.SectI.Initialize();
		m_Data.SectBefore.SectJ.Initialize();
		m_Data.SectAfter.SectI.Initialize();
		m_Data.SectAfter.SectJ.Initialize();
		m_Data.CmpTapJ.Initialize();
		m_Data.SectBefore.bUserInputStiffness = FALSE;

		// PSC VIEW 처리
		if (nShapeIndex >= D_SECT_COMPO_SHAPE_INDEX+2 && nShapeIndex <= D_SECT_COMPO_SHAPE_INDEX+3) // CI, CT : VIEW 있는 경우
		{
			if( !(nPrevStype==D_SECT_TYPE_PSC || nPrevStype==D_SECT_TYPE_COMPO_CI || nPrevStype==D_SECT_TYPE_COMPO_CT) ) 
				CreatePSCView();
			int nIndex = GetBitmapIndexFromInfoComposite();
			if(nIndex>=0) ChangeBitmapForViewer(nIndex);
		}
		else // B, I, General : VIEW 없는 경우
		{
			if (nPrevStype == D_SECT_TYPE_PSC || nPrevStype==D_SECT_TYPE_COMPO_CI || nPrevStype==D_SECT_TYPE_COMPO_CT)  
				DestroyPSCView();
		}
	}
	else if ( IsStlgSect() )
	{
		// Type, Shape Setting
		int nTmp = nShapeIndex-D_SECT_STLG_SHAPE_INDEX;
		if (nTmp==0)      { m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_B; m_Data.SectBefore.Shape = D_SECT_SHAPE_STLG_B; }
		else if (nTmp==1) { m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_I; m_Data.SectBefore.Shape = D_SECT_SHAPE_STLG_I; }
		else if (nTmp==2) { m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_MCELL; m_Data.SectBefore.Shape = D_SECT_SHAPE_STLG_MCELL; }

		m_Data.SectBefore.SectI.DBName = _T("");  // db는 none으로
		m_Data.SectBefore.SectJ.DBName = _T("");
		m_Data.SectBefore.nCellType = 0;
		m_Data.SectBefore.nCellShape = 0;
		m_Data.SectBefore.nJoint = 0;
		m_Data.SectBefore.bSymmetric = TRUE; // symmetric 은 TRUE 로
		m_Data.SectBefore.SectI.Initialize();
		m_Data.SectBefore.SectJ.Initialize();
		m_Data.SectAfter.SectI.Initialize();
		m_Data.SectAfter.SectJ.Initialize();
		m_Data.CmpTapJ.Initialize();
		m_Data.SectBefore.bUserInputStiffness = FALSE;
	}
	else
	{
		if (nPrevStype == D_SECT_TYPE_PSC || nPrevStype==D_SECT_TYPE_COMPO_CI || nPrevStype==D_SECT_TYPE_COMPO_CT)  
			DestroyPSCView();

		int nLytype=1, nLztype=1;

		BOOL bPSCOrComposite 
			= (nPrevStype == D_SECT_TYPE_PSC || nPrevStype == D_SECT_TYPE_COMPO_B || nPrevStype == D_SECT_TYPE_COMPO_I ||
			nPrevStype == D_SECT_TYPE_COMPO_TUB || nPrevStype == D_SECT_TYPE_COMPO_CI || nPrevStype == D_SECT_TYPE_COMPO_CT || 
			nPrevStype == D_SECT_TYPE_COMPO_G || nPrevStype == D_SECT_TYPE_COMPO_STLG_B  || nPrevStype == D_SECT_TYPE_COMPO_STLG_I ||
			nPrevStype == D_SECT_TYPE_COMPO_STLG_TUB);

		if (bPSCOrComposite)
		{
			nLytype = m_wndVar1.GetCurSel()+1;
			nLztype = m_wndVar2.GetCurSel()+1;
		}

		m_Data.Initialize();
		m_Data.nStype = D_SECT_TYPE_TAPERED;
		m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR;
		m_Data.SectBefore.SectI.DBName = _T("");  // user로 설정
		m_Data.SectBefore.nLytype = nLytype;
		m_Data.SectBefore.nLztype = nLztype;

		// Save new shape in data
		m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
		if(m_Data.SectBefore.Shape==D_SECT_SHAPE_REG_GEN)
		{
			m_Data.SectBefore.nStype=D_SECT_TYPE_USER;
		}
	}
	m_Data.SectBefore.nCalcStiffOpt = D_DEF_CALC_STIFF_OPT;
	ShowHideEnableDisableStiffCalcMethod();
	EnableDisableStlgMCell();

	// 위치이동 불가
	GetDlgItem(IDC_CMD_SECT_CENTROID_BTN)->EnableWindow(nShapeIndex >= D_SECT_PSC_SHAPE_INDEX);

	// PSC Value 인 경우 Mesh Size 사용하지 않음 
	// Composite 인 경우도 사용하지 않음
	BOOL bPSCValue = IsPSCValue();
	BOOL bComposite = 
		m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_B || m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_I || 
		m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_TUB || m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_CI || 
		m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_CT || m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_G ||    
		m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_STLG_B || m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_STLG_I || 
		m_Data.SectBefore.Shape==D_SECT_SHAPE_COMPO_STLG_TUB;
	if (bPSCValue || bComposite) m_chkMeshSize.SetCheck(0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlMeshSize, !(bPSCValue || bComposite) );

	// GRID change
	if (IsPscSect())
	{
		nShapeIndex = CSectUtil::GetShapeIndexFromNamePsc(m_Data.SectBefore.Shape);
		m_wndGrid4Psc.ChangeShape(nShapeIndex);
	}
	else if ( IsCompositeSect() )
	{
		if ( IsGeneralSect() )
		{
			m_wndGridComGenI.ChangeSectType(D_SECT_TYPE_COMPO_G);
			m_wndGridComGenJ.ChangeSectType(D_SECT_TYPE_COMPO_G);
			m_wndGridComGenI.ChangeShape(21);
			m_wndGridComGenJ.ChangeShape(21);
		}
		else
		{
			if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_B) nShapeIndex = 0;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_I) nShapeIndex = 1;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CI) nShapeIndex = 2;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_CT) nShapeIndex = 3;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_G) nShapeIndex = 5;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB) nShapeIndex = 6;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B) nShapeIndex = 7;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I) nShapeIndex = 8;
			else if (m_Data.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB) nShapeIndex = 9;

			m_wndGrid4Composite.ChangeShape(nShapeIndex, TRUE);
		}    
	}  
	else if( IsStlgSect() )
	{
		nShapeIndex = nShapeIndex - D_SECT_STLG_SHAPE_INDEX;
		m_wndGrid4StlG.ChangeShape(nShapeIndex);
	}
	else
	{
		m_wndGridFirst.ChangeSectType(D_SECT_TYPE_USER);
		m_wndGridSecond.ChangeSectType(D_SECT_TYPE_USER);
		m_wndGridFirst.ChangeShape(nShapeIndex);
		m_wndGridSecond.ChangeShape(nShapeIndex);
	}

	T_SECT_SECTBASE_D *pSectI, *pSectJ;
	pSectI = &m_Data.SectBefore.SectI;
	pSectJ = &m_Data.SectBefore.SectJ;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameI = pSectI->DBName;
	CString csDBNameJ = pSectJ->DBName;

	// First 초기화
	pSectI->Initialize();
	pSectI->DBName = csDBNameI;

	// Second 초기화
	pSectJ->Initialize();
	pSectJ->DBName = csDBNameJ;

	// Dialog에 반영
	ShowData2Dlg();

	CreateOrDestoryStlgView();
}

void CCMSectItemTap::OnChangeDB() 
{
	// TODO: Add your control notification handler code here
	if (m_Data.SectBefore.SectI.DBName == GetDB()) return;
	DoActionWhenDBChanged();
}

void CCMSectItemTap::OnChangeUseDB() 
{
	// TODO: Add your control notification handler code here
	int nValUsrDB;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nValUsrDB);
	if (nValUsrDB == 2) m_wndDB.EnableWindow(TRUE);
	else m_wndDB.EnableWindow(FALSE);
#ifdef _MGEN_CH
	if (nValUsrDB == 2) 
		GetDlgItem(IDC_CMD_SECT_CHANGE_BTN)->EnableWindow(TRUE);
	else 
		GetDlgItem(IDC_CMD_SECT_CHANGE_BTN)->EnableWindow(FALSE);
#endif
	int nPrevValUsrDB;
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER) nPrevValUsrDB = 0;
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC) 
	{
		ASSERT(0);
		return;
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		ASSERT(0);
		return;
	}
	else if (m_Data.SectBefore.SectI.DBName == _T("")) nPrevValUsrDB = 1;
	else nPrevValUsrDB = 2;

	if (nValUsrDB == nPrevValUsrDB) return;

	if (nValUsrDB == 0)   // value type
	{
		m_Data.SectBefore.nStype = D_SECT_TYPE_USER;
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		// db->val or user->val
		m_Data.SectBefore.SectI.DBName = m_Data.SectBefore.SectJ.DBName = _T("");
		m_Data.SectBefore.SectI.SName = m_Data.SectBefore.SectJ.SName = _T("");
		m_wndGridFirst.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		m_wndGridSecond.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		m_wndGridFirst.ShowData();
		m_wndGridSecond.ShowData();
	}
	else 
	{
		// val->user, db->user
		// val->db, user->db
		m_Data.SectBefore.nStype = D_SECT_TYPE_REGULAR;
		CDlgUtil::CtrlShowHide(this, m_aCtrlUserDB, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValue, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlValuePSC, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgBoxI, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlStlgMCell, FALSE);

		DoActionWhenDBChanged();
	}
}

void CCMSectItemTap::OnChangeFirstName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndFirstName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectI;
#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	if (csName == pSect->SName) return;
	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 8; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeSecondName() 
{
	// TODO: Add your control notification handler code here
	CString csName;
	m_wndSecondName.GetWindowText(csName);
	T_SECT_SECTBASE_D *pSect = &m_Data.SectBefore.SectJ;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameRusToEng(pSect->DBName, csName);
#endif

	if (csName == pSect->SName) return;
	pSect->SName = csName;

#if defined(_RUS)
	CSectDBRussiaUtil::ConvertSectDBNameEngToRus(pSect->DBName, csName);
#endif

	// 데이타를 초기화 한다.
	T_SECT_SECTBASE_D SectData;
	SectData.Initialize();
	if (csName != _T(""))
		m_pDoc->m_pSectDB->GetSectData(pSect->DBName, pSect->SName, SectData);
	for (int i = 0; i < 8; i++)
		pSect->Size[i] = SectData.Size[i];

	// Dialog에 반영한다.
	SetSectData();

	// Section의 Name을 바꾼다.
	if (m_bAutoChangeName)
	{
		m_Data.SName = csName;
		m_wndName.SetWindowText(csName);
	}

	// Preview Update
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}


void CCMSectItemTap::OnChangeFirstSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE11 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE12 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE13 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE14 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE15 : nIx = 4; break;
	case IDC_CMD_SP_ID_VALUE16 : nIx = 5; break;
	case IDC_CMD_SP_ID_VALUE17 : nIx = 6; break;
	case IDC_CMD_SP_ID_VALUE18 : nIx = 7; break;
	default: ASSERT(0); break;
	}
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectI;
	pSect->Size[nIx] = m_wndFirstSize[nIx].GetEditValue();
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeSecondSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	int nIx;
	switch(nID)
	{
	case IDC_CMD_SP_ID_VALUE21 : nIx = 0; break;
	case IDC_CMD_SP_ID_VALUE22 : nIx = 1; break;
	case IDC_CMD_SP_ID_VALUE23 : nIx = 2; break;
	case IDC_CMD_SP_ID_VALUE24 : nIx = 3; break;
	case IDC_CMD_SP_ID_VALUE25 : nIx = 4; break;
	case IDC_CMD_SP_ID_VALUE26 : nIx = 5; break;
	case IDC_CMD_SP_ID_VALUE27 : nIx = 6; break;
	case IDC_CMD_SP_ID_VALUE28 : nIx = 7; break;
	default: ASSERT(0); break;
	}	
	T_SECT_SECTBASE_D* pSect = &m_Data.SectBefore.SectJ;
	pSect->Size[nIx] = m_wndSecondSize[nIx].GetEditValue();
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeMaterial()
{
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT|| m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		m_Data.SectBefore.Matl_Elast = m_wndMatlValueCompo[0].GetEditValue();
		m_Data.SectBefore.Matl_Density = m_wndMatlValueCompo[1].GetEditValue();
		m_Data.SectBefore.Matl_Poisson_S = m_edtMatPs.GetEditValue();
		m_Data.SectBefore.Matl_Poisson_C = m_edtMatPc.GetEditValue();
		m_Data.SectBefore.Matl_Thermal = m_edtMatTstc.GetEditValue();
		m_wndSecView.Invalidate();    
	}
	else
	{
		m_Data.SectBefore.Matl_Elast = m_wndMatlValue[0].GetEditValue();
		m_wndSecView.Invalidate();
	}
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeVar1() 
{
	// TODO: Add your control notification handler code here
	m_Data.SectBefore.nLytype = m_wndVar1.GetCurSel()+1;
}

void CCMSectItemTap::OnChangeVar2() 
{
	// TODO: Add your control notification handler code here
	m_Data.SectBefore.nLztype = m_wndVar2.GetCurSel()+1;
}

void CCMSectItemTap::OnSetfocusCmdSpIdName() 
{
	::PostMessage(m_wndName.GetSafeHwnd(), EM_SETSEL, 0, -1);
}

void CCMSectItemTap::OnUpdateCmdSpIdName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO: Add your control notification handler code here
	CString csDB, csName;
	csDB = GetDB();
	m_wndName.GetWindowText(csName);
	if (csName == _T("")) m_bAutoChangeName = TRUE;
	else
	{
		T_SECT_SECTBASE_D SectData;

		if (csDB == _T("")) m_bAutoChangeName = FALSE;  // change by key in
		else if (m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData))
			m_bAutoChangeName = TRUE; // exist in db
		else m_bAutoChangeName = FALSE; // not exist in db(change by key in)
	}	
}

BOOL CCMSectItemTap::DestroyWindow() 
{
	// 현재 단면 타입이 PSC면 -> 뷰를 죽인다.
	if(m_Data.SectBefore.nStype == D_SECT_TYPE_PSC)
		DestroyPSCView();	
	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B      || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB    || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI     || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT     || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G      || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
		DestroyPSCView();	

	DestroyStlgView();

	return CDialog::DestroyWindow();
}

void CCMSectItemTap::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bOK = TRUE;
	if (!Dlg2Data()) bOK = FALSE;
	if (IsValueSect())
	{
		if (!m_wndGridFirst.GetPeriAndStiff()) bOK = FALSE;
		if (!m_wndGridSecond.GetPeriAndStiff()) bOK = FALSE;
	}
	else 
	{
		if (!CSectUtil::CalcSectData(m_Data))  bOK = FALSE;
	}
	if (!bOK)
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	m_Data.SectBefore.dShearCheckPos[0][1] = m_Data.SectBefore.SectI.Design.ZBar;
	m_Data.SectBefore.dShearCheckPos[1][1] = m_Data.SectBefore.SectJ.Design.ZBar;

	m_wndSecView.SetCentroidFlag(TRUE);
	m_wndSecView.Invalidate();		
	ReDrawStlgViewer();
}

void CCMSectItemTap::ChangeCellNumber(int nCellNum)
{
	// change bitmap
// 	if(m_pBitmap != NULL){ delete m_pBitmap; m_pBitmap = NULL; }
// 	CBCGPStatic* SecImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
// 	m_pBitmap = (CBitmap*)SecImage;
// 	m_pBitmap = new CBitmap;
// 	UINT nBitmap = (nCellNum == 1) ? IDB_CMD_SP_ISGL_BMP21 : IDB_CMD_SP_ISGL_BMP22;
// 	m_pBitmap->LoadBitmap(nBitmap);
// 	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
	ChangeBitmap();

	// enable/disable
	int nUserDB;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDB, nUserDB);
	if (nUserDB < 2) 
	{
		m_wndFirstSize[6].EnableWindow(nCellNum != 1);
		m_wndSecondSize[6].EnableWindow(nCellNum != 1);
		if (nCellNum == 1)
		{
			m_Data.SectBefore.SectI.Size[6] = 0.;
			m_Data.SectBefore.SectJ.Size[6] = 0.;
			m_wndFirstSize[6].SetEditUnit(0.);
			m_wndSecondSize[6].SetEditUnit(0.);
		}
	}
}

void CCMSectItemTap::OnChangeCmdEditCellNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	// cell number : 1 ~ 4
	CString csCellNum;
	m_editCellNum.GetWindowText(csCellNum);
	int nCellNum = _ttoi(csCellNum);
	if (nCellNum < 1 || nCellNum > 4)
	{
		if (nCellNum > 4) nCellNum = 4;
		if (nCellNum < 1) nCellNum = 1;
		csCellNum.Format(_T("%d"), nCellNum);
		m_editCellNum.SetWindowText(csCellNum);
	}

	ChangeCellNumber(nCellNum);	

	m_Data.SectBefore.nCellShape = nCellNum;
	m_wndSecView.Invalidate();	
	ReDrawStlgViewer();
}


void CCMSectItemTap::OnDeltaposCmdSpinCellNum(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_EDIT_CELL_NUM,_T("1"),_T("4"),1,pNMHDR,pResult);

	CString csCellNum;
	m_editCellNum.GetWindowText(csCellNum);
	m_Data.SectBefore.nCellShape = _ttoi(csCellNum);
	ChangeCellNumber(m_Data.SectBefore.nCellShape);	
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();

	*pResult = 0;
}

void CCMSectItemTap::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D OldData;
	OldData = m_Data.SectBefore;

	CCMSectOffsetDlg dlg(&m_Data.SectBefore, TRUE, this);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray arOffsetNameList;
		CSectUtil::GetOffsetNameList(arOffsetNameList);
		if (m_Data.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = arOffsetNameList[m_Data.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}
	else m_Data.SectBefore = OldData;

	m_wndSecView.Invalidate();	
	ReDrawStlgViewer();
}

// 기존 Material 버튼
void CCMSectItemTap::OnCmdSelMaterialBtn()
{
	CRect rect;
	GetDlgItem(IDC_CMD_MATL_BTN)->GetWindowRect(rect);
	//rect.top += 20;

	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	CCMConcrSteelMatlDlg dlg(pDoc, rect, IDD_CMD_SECT_ITEM_TAP, this);
	CStringArray aData;
	UINT aID[] = {
		IDC_CMD_MATL_MVALUE1, IDC_CMD_MATL_MVALUE2,
		IDC_CMD_MATL_MVALUE3, IDC_CMD_MATL_MVALUE4,
	};

	// save current data
	CString data;
	for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
	{
		GetDlgItem(aID[i])->GetWindowText(data);
		aData.Add(data);
	}
	if (dlg.DoModal() != IDOK)
	{
		// restore old data
		for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
			GetDlgItem(aID[i])->SetWindowText(aData[i]);
	}		
}

void CCMSectItemTap::OnCmdMeshSizeChk2() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkMeshSize.GetCheck();
	BOOL bEnable = (nCheck == 1);
	m_editMeshSize.EnableWindow(bEnable);
	if (!bEnable) m_editMeshSize.SetWindowText(_T(""));	
}

void CCMSectItemTap::OnCmdFemRdo() 
{
	int nCalc;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlOption,nCalc);
	m_Data.SectBefore.nCalcStiffOpt=nCalc;    
	CalcRegGenLineTypeStiffAndUpdateGrid(0);
	CalcRegGenLineTypeStiffAndUpdateGrid(1);
}

void CCMSectItemTap::EnableDisableStlgMCell()
{
	if (m_Data.SectBefore.nStype!=D_SECT_TYPE_STLG_MCELL) return;

	int nNoOfCell = m_edtMCellNoCell.GetEditValue();    
	BOOL bSym = m_chkMCellSym.GetCheck();
	int nFShape;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellFShape, nFShape);

	int nLorR;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoMCellLorR, nLorR);

	GetDlgItem(IDC_CMD_SECT_MCELL_SYM_CHK)->EnableWindow(nNoOfCell != 0);    
	GetDlgItem(IDC_CMD_SECT_MCELL_LEFT_RDO)->EnableWindow(nNoOfCell == 0);
	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);    

	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);
	GetDlgItem(IDC_CMD_SECT_MCELL_RIGHT_RDO)->EnableWindow(nNoOfCell == 0);

	//GetDlgItem(IDC_CMD_SECT_MCELL_J1_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell != 0));
	GetDlgItem(IDC_CMD_SECT_MCELL_J1_CHK)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_SECT_MCELL_J2_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 0) || nNoOfCell != 0));
	//    GetDlgItem(IDC_CMD_SECT_MCELL_JR1_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell != 0 && !bSym)));    
	GetDlgItem(IDC_CMD_SECT_MCELL_JR1_CHK)->EnableWindow(FALSE);    
	GetDlgItem(IDC_CMD_SECT_MCELL_JR2_CHK)->EnableWindow(nFShape == 3 && ((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell != 0 && !bSym)));

}

void CCMSectItemTap::ShowHideEnableDisableStiffCalcMethod()
{
	if (IsGeneralSect())
	{
		if (m_Data.SectBefore.nStype!=D_SECT_TYPE_COMPO_G)
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlOption, TRUE);
			if (m_Data.SectBefore.SectI.aLine.GetSize() > 0 || 
				m_Data.SectBefore.SectJ.aLine.GetSize() > 0 )
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, TRUE);
				CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);
			}
			else 
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
				CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
			}
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrlOption, FALSE);
		}
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlOption, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
	}
}

LRESULT CCMSectItemTap::OnGridDataChanged(WPARAM wParam, LPARAM lParam)
{
	ShowHideEnableDisableStiffCalcMethod();
	EnableDisableStlgMCell();
	CalcRegGenLineTypeStiffAndUpdateGrid(lParam);  
	return 0L;
}

void CCMSectItemTap::CalcRegGenLineTypeStiffAndUpdateGrid(int iOrj)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (iOrj == 0 && m_Data.SectBefore.SectI.aLine.GetSize() > 0)  // i
	{
		// Option에 따라 강성 계산
		if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
		{
			m_wndGridComGenI.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenI.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenJ.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenJ.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenI.ShowData();
			m_wndGridComGenJ.ShowData();
			m_wndSecViewCGI.Invalidate();
			m_wndSecViewCGJ.Invalidate();
		}
		else
		{
			T_SECT_D SectD;
			CSectUtil::GetSectFromTap(m_Data, SectD, TRUE);
			SectD.SectBefore.SectI.Stiffness.Initialize();
			if (pDoc->m_pSectDB->CalcSectData(SectD, TRUE))
			{
				m_Data.SectBefore.SectI.Stiffness.Rxx = SectD.SectBefore.SectI.Stiffness.Rxx;
				m_Data.SectBefore.SectI.Stiffness.Asy = SectD.SectBefore.SectI.Stiffness.Asy;
				m_Data.SectBefore.SectI.Stiffness.Asz = SectD.SectBefore.SectI.Stiffness.Asz;
				m_wndGridFirst.ShowData();
			}
		}
	}
	else if (iOrj == 1 && m_Data.SectBefore.SectJ.aLine.GetSize() > 0) // j
	{
		// Option에 따라 강성 계산
		if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
		{
			m_wndGridComGenI.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenI.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenJ.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenJ.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenI.ShowData();
			m_wndGridComGenJ.ShowData();
			m_wndSecViewCGI.Invalidate();
			m_wndSecViewCGJ.Invalidate();
		}
		else
		{
			T_SECT_D SectD;
			CSectUtil::GetSectFromTap(m_Data, SectD, FALSE);
			SectD.SectBefore.SectI.Stiffness.Initialize();
			if (pDoc->m_pSectDB->CalcSectData(SectD, TRUE))
			{
				m_Data.SectBefore.SectJ.Stiffness.Rxx = SectD.SectBefore.SectI.Stiffness.Rxx;
				m_Data.SectBefore.SectJ.Stiffness.Asy = SectD.SectBefore.SectI.Stiffness.Asy;
				m_Data.SectBefore.SectJ.Stiffness.Asz = SectD.SectBefore.SectI.Stiffness.Asz;
				m_wndGridSecond.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
				m_wndGridSecond.ShowData();
			}
		}
	}
}

// Composite용 New Material 버튼
void CCMSectItemTap::OnMaterialBtn() 
{
	CRect rect;
	GetDlgItem(IDC_CMD_MATL2_BTN)->GetWindowRect(rect);

	CStringArray aData;
	UINT aID[] = {IDC_CMD_MATL2_EDT1, IDC_CMD_MATL2_EDT2, IDC_CMD_MATL2_EDT5, IDC_CMD_MATL2_EDT6, IDC_CMD_MATL2_EDT7};

	// save current data
	CString data;
	for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
	{
		GetDlgItem(aID[i])->GetWindowText(data);
		aData.Add(data);
	}

	if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_B || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_I || 
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_TUB || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_B   ||
		m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_I || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CCMConcrSteelMatlDlg dlg(m_pDoc, rect, IDD_CMD_SECT_ITEM_TAP, this);
		if (dlg.DoModal() != IDOK) // restore old data
			for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++) GetDlgItem(aID[i])->SetWindowText(aData[i]);
	}
	else if(m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CI || m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_CT)
	{
		CCMCompoMatDlg dlg(m_pDoc, rect, IDD_CMD_SECT_ITEM_TAP, this);
		if (dlg.DoModal() != IDOK) // restore old data
			for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++) GetDlgItem(aID[i])->SetWindowText(aData[i]);
	}
	else ASSERT(FALSE);
}

void CCMSectItemTap::OnCmdMultiModulusCheck() 
{
	int nCheck = m_chkMat.GetCheck();
	if (nCheck == 1) // Button Check
	{
		m_edtMat1.EnableWindow(TRUE);
		m_edtMat2.EnableWindow(TRUE);
	}
	else
	{
		m_edtMat1.SetEditUnit(0);
		m_edtMat2.SetEditUnit(0);
		m_edtMat1.EnableWindow(FALSE);
		m_edtMat2.EnableWindow(FALSE);
	}
}

void CCMSectItemTap::OnChangeCmdSpIdEsec()
{
	m_Data.SectBefore.Matl_Elast = m_wndMatlValueCompo[0].GetEditValue();
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeCmdSpIdDsdc()
{
	m_Data.SectBefore.Matl_Density  = m_wndMatlValueCompo[1].GetEditValue();
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeCmdSpIdPs()
{
	m_Data.SectBefore.Matl_Poisson_S = m_edtMatPs.GetEditValue();
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeCmdSpIdPc()
{
	m_Data.SectBefore.Matl_Poisson_C  = m_edtMatPc.GetEditValue();
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::OnChangeCmdSpIdTstc()
{
	m_Data.SectBefore.Matl_Thermal = m_edtMatTstc.GetEditValue();
	m_wndSecView.Invalidate();
	ReDrawStlgViewer();
}

void CCMSectItemTap::SetGeneralPartList()
{  
	int nPartSize = m_Data.SectBefore.SectI.aGeneralPart.GetSize();
	int nIndex;
	CString str;

	m_cmbPart.ResetContent();
	nIndex = m_cmbPart.AddString(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_TOTAL));
	m_cmbPart.SetItemData(nIndex, nIndex);
	int i = 0;
	m_cmbPart.SetRedraw(FALSE);
	for(i=0 ; i<nPartSize ; ++i)
	{
		str.Format(_ULS('Part %d'), i+1);
		nIndex = m_cmbPart.AddString(str);
		m_cmbPart.SetItemData(nIndex, nIndex);
	}
	for(i=1 ; i<(nPartSize-1) ; ++i)
	{
		str.Format(_LS(IDS_CMD_RPSC_PART1));
		CString strTmp;
		for(int j=0 ; j<i ; ++j)
		{
			strTmp.Format(_T("+%d"), j+2);
			str += strTmp;
		}
		nIndex = m_cmbPart.AddString(str);
		m_cmbPart.SetItemData(nIndex, nIndex);
	}
	m_cmbPart.SetCurSel(0);
	
	m_cmbBeforePart.ResetContent();
	nIndex = m_cmbBeforePart.AddString(_LS(IDS_CMD_RPSC_PART1));
	m_cmbBeforePart.SetItemData(nIndex, nIndex);
	m_cmbBeforePart.SetRedraw(FALSE);
	for(i=2 ; i<nPartSize ; ++i)
	{
		str.Format(_LS(IDS_CMD_RPSC_PART1));
		CString strTmp;
		for(int j=2 ; j<=i ; ++j)
		{
			strTmp.Format(_T("+%d"), j);
			str += strTmp;
		}
		nIndex = m_cmbBeforePart.AddString(str);
		m_cmbBeforePart.SetItemData(nIndex, nIndex);
	}
	m_cmbBeforePart.SetCurSel(0);
	m_cmbBeforePart.SetRedraw(TRUE);
	m_cmbPart.SetRedraw(TRUE);
}

void CCMSectItemTap::OnComGenImportSPCI()
{
	CString strSec;
	if (CProduct::IsRusLocal())
		strSec = _T("Section Files(*.sec)|*.sec|All File(*.*)|*.*||");
	else
		strSec = _T("MIDAS Section Files(*.sec)|*.sec|All File(*.*)|*.*||");

	CString csFileName;
	CFileDialog fd(TRUE, _T(".sec"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
		strSec, NULL);
	if (fd.DoModal() != IDOK) return;

	CWaitCursor wait;
	CCMSectImportSPC Importer;
	int nVerCheck=1510;
	if (!Importer.Import(fd.GetPathName(), nVerCheck))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_CAN_NOT_IMPORT));
		return;
	}

	GetDlgItem(IDC_CMD_SP_ID_STC_I)->SetWindowText(fd.GetFileName());

	UINT  nCurSectID=0;
	CArray<UINT, UINT> aID;
	CArray<T_SECT_D, T_SECT_D&> aData;

	Importer.GetList(aID, aData);

	T_GRPD_SSM_D GrpdSSMD; GrpdSSMD.Initialize();
	T_STFN_SSM_D StfnSSMD; StfnSSMD.Initialize();
	if(Importer.ConvertSectionManager(m_Data, GrpdSSMD, StfnSSMD))
	{
		m_bModifyGrpd = TRUE;
		m_bModifyStfn = TRUE;
		m_GrpdSSMD = GrpdSSMD;
		m_StfnSSMD = StfnSSMD;
	}

	CCMSectItemPSCImport dlg;
	dlg.SetCompositeMode(); // 20080223 mylee - Composite General 만 보이도록
	dlg.SetData(aID, aData, nCurSectID);
	if (dlg.DoModal() == IDOK)
	{
		// 주의 메시지 표시
		GSaveHistoryFormatNF(_LS(IDS_CMD_CON_WARNING_2PART));

		T_SECT_D  TempD;
		dlg.GetData(nCurSectID, TempD);   

		// J 단이 Import 되어 있다면
		if (m_bIsCGImport[1])
		{
			// Check
			// 1.두단면의 파트수
			if (m_Data.SectBefore.SectJ.aGeneralPart.GetSize() != TempD.SectBefore.SectI.aGeneralPart.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_PART));
				return;
			}
			// 2.라인수
			if (m_Data.SectBefore.SectJ.aLine.GetSize() != TempD.SectBefore.SectI.aLine.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_LINE));
				return;
			}
			// 3. OuterPolygon
			if (m_Data.SectBefore.SectJ.aOuterPolygon.GetSize() != TempD.SectBefore.SectI.aOuterPolygon.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_OUT));
				return;
			}
			// 4. InnerPolygon
			if (m_Data.SectBefore.SectJ.aOuterPolygon.GetSize() != TempD.SectBefore.SectI.aOuterPolygon.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_INNER));
				return;
			}
			// 5.전체라인방향
			// 6.파트별포인트수
			for (int i=0; i<m_Data.SectBefore.SectJ.aOuterPolygon.GetSize(); i++)
			{
				if (m_Data.SectBefore.SectJ.aOuterPolygon[i].aVertex.GetSize() 
					!= TempD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize())
				{
					CString szMsg;
					szMsg.Format(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_VERTEX), i+1);
					AfxMessageBox(szMsg);
					return;
				}
			}
			// 7.파트별라인방향체크      
		}

		m_Data.SectBefore.SectI = m_Data.SectAfter.SectI = TempD.SectBefore.SectI;
		//m_Data.SectBefore.SectI = TempD.SectBefore.SectI;
		//m_Data.SectAfter.SectI.Stiffness  = TempD.SectBefore.SectI.Stiffness;

		// Qy, Qz 계산
		/*m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = TRUE; 

		double dQy=0., dQz=0.;
		//     if (CSectUtil::GetPscValueDataQ(&m_Data, dQy, dQz)) // later
		//     {
		//       m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
		//       m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
		//     }
		m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = FALSE; */

		m_Data.nStype = D_SECT_TYPE_TAPERED;
		m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
		m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;

		if(m_Data.SectBefore.SectI.aLine.GetSize()>0)
		{
			// Line Type은 강성 계산 (비틀림 값)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE); // 20080604 mylee
			OnCmdFemRdo();
		}
		else
		{
			//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
			// size and stiffness
			m_wndGridComGenI.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenI.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenI.ShowData();
			m_wndSecViewCGI.Invalidate();	
		}
		SetGeneralPartList();

		m_bIsCGImport[0] = TRUE;
		//OnSelchangeCmdPartSel();

		T_SECT_D SectTmp = m_Data;

		// Mesh Check 는 Tapered 이면서 I, J 단 정보가 모두 존재 할때 Check!!!!!
		BOOL bMeshCheck = FALSE;
		if(SectTmp.nStype == D_SECT_TYPE_TAPERED && 
			(SectTmp.SectBefore.SectI.aOuterPolygon.GetSize() != 0) &&
			(SectTmp.SectBefore.SectJ.aOuterPolygon.GetSize() != 0)) 
		{
			bMeshCheck = TRUE;
		}

		if(bMeshCheck)
		{
			if(!CSectUtil::IsMeshOK(SectTmp))
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning_mesh_NG_ImportFromSPC));
		}
	}
}

void CCMSectItemTap::OnComGenImportSPCJ()
{
	CString strSec;
	if (CProduct::IsRusLocal())
		strSec = _T("Section Files(*.sec)|*.sec|All File(*.*)|*.*||");
	else
		strSec = _T("MIDAS Section Files(*.sec)|*.sec|All File(*.*)|*.*||");

	CString csFileName;
	CFileDialog fd(TRUE, _T(".sec"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
		strSec, NULL);
	if (fd.DoModal() != IDOK) return;

	CWaitCursor wait;
	CCMSectImportSPC Importer;
	int nVerCheck=1510;
	if (!Importer.Import(fd.GetPathName(), nVerCheck))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_CAN_NOT_IMPORT));
		return;
	}

	GetDlgItem(IDC_CMD_SP_ID_STC_J)->SetWindowText(fd.GetFileName());

	UINT  nCurSectID=0;
	CArray<UINT, UINT> aID;
	CArray<T_SECT_D, T_SECT_D&> aData;

	Importer.GetList(aID, aData);

	T_GRPD_SSM_D GrpdSSMD; GrpdSSMD.Initialize();
	T_STFN_SSM_D StfnSSMD; StfnSSMD.Initialize();
	if(Importer.ConvertSectionManager(m_Data, GrpdSSMD, StfnSSMD))
	{
		m_bModifyGrpd = TRUE;
		m_bModifyStfn = TRUE;
		m_GrpdSSMD = GrpdSSMD;
		m_StfnSSMD = StfnSSMD;
	}

	CCMSectItemPSCImport dlg;
	dlg.SetCompositeMode(); // 20080223 mylee - Composite General 만 보이도록
	dlg.SetData(aID, aData, nCurSectID);
	if (dlg.DoModal() == IDOK)
	{
		// 주의 메시지 표시
		GSaveHistoryFormatNF(_LS(IDS_CMD_CON_WARNING_2PART));

		T_SECT_D  TempD;
		dlg.GetData(nCurSectID, TempD);

		// I 단이 Import 되어 있다면
		if (m_bIsCGImport[0])
		{
			// Check
			// 1.두단면의 파트수
			if (m_Data.SectBefore.SectI.aGeneralPart.GetSize() != TempD.SectBefore.SectI.aGeneralPart.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_PART));
				return;
			}
			// 2.라인수
			if (m_Data.SectBefore.SectI.aLine.GetSize() != TempD.SectBefore.SectI.aLine.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_LINE));
				return;
			}
			// 3. OuterPolygon
			if (m_Data.SectBefore.SectI.aOuterPolygon.GetSize() != TempD.SectBefore.SectI.aOuterPolygon.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_OUT));
				return;
			}
			// 4. InnerPolygon
			if (m_Data.SectBefore.SectI.aOuterPolygon.GetSize() != TempD.SectBefore.SectI.aOuterPolygon.GetSize())
			{
				AfxMessageBox(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_INNER));
				return;
			}
			// 5.전체라인방향
			// 6.파트별포인트수
			for (int i=0; i<m_Data.SectBefore.SectI.aOuterPolygon.GetSize(); i++)
			{
				if (m_Data.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize() 
					!= TempD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize())
				{
					CString szMsg;
					szMsg.Format(_LS(IDS_CMD_SECT_COMP_COMPOSITE_GENERAL_ERROR_DIFFER_VERTEX), i+1);          
					AfxMessageBox(szMsg);
					return;
				}
			}
			// 7.파트별라인방향체크      
		}

		m_Data.SectBefore.SectJ = m_Data.SectAfter.SectJ = TempD.SectBefore.SectI;

		// Qy, Qz 계산
		/*m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = TRUE; 

		double dQy=0., dQz=0.;
		//     if (CSectUtil::GetPscValueDataQ(&m_Data, dQy, dQz)) // later
		//     {
		//       m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
		//       m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
		//     }
		m_Data.SectBefore.bAutoWebThkShear[0][0] = 
		m_Data.SectBefore.bAutoWebThkShear[0][1] = 
		m_Data.SectBefore.bAutoWebThkShear[0][2] = 
		m_Data.SectBefore.bAutoWebThkShear[1][0] = 
		m_Data.SectBefore.bAutoWebThkShear[1][1] = 
		m_Data.SectBefore.bAutoWebThkShear[1][2] = FALSE;*/

		m_Data.nStype = D_SECT_TYPE_TAPERED;
		m_Data.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
		m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;

		if(m_Data.SectBefore.SectJ.aLine.GetSize()>0)
		{
			// Line Type은 강성 계산 (비틀림 값)
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE); // 20080604 mylee
			OnCmdFemRdo();
		}
		else
		{
			//CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, D_DEF_CALC_STIFF_OPT);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlOption, FALSE);
			// size and stiffness
			m_wndGridComGenJ.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
			//m_wndGridComGenJ.SetPSCDesign(m_PSCDesign.bCompPSC);
			m_wndGridComGenJ.ShowData();
			m_wndSecViewCGJ.Invalidate();	
		}
		SetGeneralPartList();

		m_bIsCGImport[1] = TRUE;    

		T_SECT_D SectTmp = m_Data;

		// Mesh Check 는 Tapered 이면서 I, J 단 정보가 모두 존재 할때 Check!!!!!
		BOOL bMeshCheck = FALSE;
		if(SectTmp.nStype == D_SECT_TYPE_TAPERED && 
			(SectTmp.SectBefore.SectI.aOuterPolygon.GetSize() != 0) &&
			(SectTmp.SectBefore.SectJ.aOuterPolygon.GetSize() != 0)) 
		{
			bMeshCheck = TRUE;
		}

		if(bMeshCheck)
		{
			if(!CSectUtil::IsMeshOK(SectTmp))
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning_mesh_NG_ImportFromSPC));
		}
	}
}

void CCMSectItemTap::OnCmdCalcSectPropIBtn()
{
	T_SECT_D SectD = m_Data;

	// I단
	BOOL bFirstBase = FALSE;
	for (int nGP = 0; nGP < SectD.SectBefore.SectI.aGeneralPart.GetSize(); nGP++)
	{
		if (!bFirstBase && SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial) bFirstBase = TRUE;
		else if (bFirstBase) SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial = FALSE;
	}
	//

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pSectDB->CalcSectCompo_G_ReCalc(SectD))
	{
		m_Data.SectBefore.SectI = m_Data.SectAfter.SectI = SectD.SectBefore.SectI;
		m_bReCalc = TRUE;

		//General composite에서는 Qyb, Qzb 계산이 안되어서 기존값 저장.
		m_Data.SectAfter.SectI.Stiffness.Qyb = SectD.SectAfter.SectI.Stiffness.Qyb;
		m_Data.SectAfter.SectI.Stiffness.Qzb = SectD.SectAfter.SectI.Stiffness.Qzb;

		OnSelchangeCmdPartSel();
	}
}

void CCMSectItemTap::OnCmdCalcSectPropJBtn()
{
	T_SECT_D SectD = m_Data;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	SectD.SectBefore.SectI = SectD.SectBefore.SectJ;

	// J단을 I단으로 복사후
	BOOL bFirstBase = FALSE;
	for (int nGP = 0; nGP < SectD.SectBefore.SectI.aGeneralPart.GetSize(); nGP++)
	{
		if (!bFirstBase && SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial) bFirstBase = TRUE;
		else if (bFirstBase) SectD.SectBefore.SectI.aGeneralPart[nGP].bBaseMaterial = FALSE;
	}
	//

	if(pDoc->m_pSectDB->CalcSectCompo_G_ReCalc(SectD))
	{
		m_Data.SectBefore.SectJ = m_Data.SectAfter.SectJ = SectD.SectBefore.SectI;
		m_bReCalc = TRUE;

		//General composite에서는 Qyb, Qzb 계산이 안되어서 기존값 저장.
		m_Data.SectAfter.SectJ.Stiffness.Qyb = SectD.SectAfter.SectJ.Stiffness.Qyb;
		m_Data.SectAfter.SectJ.Stiffness.Qzb = SectD.SectAfter.SectJ.Stiffness.Qzb;

		OnSelchangeCmdPartSel();
	}
}

void CCMSectItemTap::OnChkWarpingEffect()
{
	BOOL bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_Data.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;

	if (m_Data.SectBefore.nStype == D_SECT_TYPE_USER)
	{
		m_wndGridFirst.SetWarpingEffect(bConsiderWarpingEffect);
		m_wndGridSecond.SetWarpingEffect(bConsiderWarpingEffect);
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_PSC)
	{
		m_wndGrid4Psc.SetWarpingEffect(bConsiderWarpingEffect);
		SectWapingCheckPSC();
	}
	else if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
	{
		m_wndGridComGenI.SetWarpingEffect(bConsiderWarpingEffect);
		m_wndGridComGenJ.SetWarpingEffect(bConsiderWarpingEffect);
	}
}

void CCMSectItemTap::OnChkhumbly()
{
	BOOL bHumbly = m_chkHumbly.GetCheck();
	m_Data.SectBefore.bHumblyChk = bHumbly;
	m_Data.SectAfter.bHumblyChk = bHumbly;
}

void CCMSectItemTap::OnChkhumblyBefore()
{
	BOOL bHumbly = m_chkHumblyBefore.GetCheck();
	m_Data.SectBefore.bHumblyChk = bHumbly;
}

void CCMSectItemTap::OnChkhumblyAfter()
{
	BOOL bHumbly = m_chkHumblyAfter.GetCheck();
	m_Data.SectAfter.bHumblyChk = bHumbly;
}

void CCMSectItemTap::OnChkPSCDesign()
{
	// 	BOOL bPSCDesign = m_chkPSCDesign.GetCheck();
	// 	m_PSCDesign.bCompPSC = bPSCDesign;
	//   
	// 	if (m_Data.SectBefore.nStype == D_SECT_TYPE_COMPO_G)
	//   {
	//     m_wndGridComGenI.SetPSCDesign(bPSCDesign);
	//     m_wndGridComGenJ.SetPSCDesign(bPSCDesign);
	//   }

	BOOL bPSCDesign = m_chkPSCDesign.GetCheck();

	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_STC)->EnableWindow(bPSCDesign);
	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_I_EDT)->EnableWindow(bPSCDesign);
	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_J_EDT)->EnableWindow(bPSCDesign);
	GetDlgItem(IDC_GENERAL_PSC_DESIGN_SHEAR_UNT)->EnableWindow(bPSCDesign);
}

void CCMSectItemTap::OnCmdSectWapingCheckBtn() 
{
	CCMSectItemPSC_WarpingCheckTapDlg WapingChkDlg(this);
	T_SECT_D* sect=&m_Data;

	T_SECT_D_PSC_I Cell;
	Cell.Set(*sect,TRUE,TRUE);
	WapingChkDlg.SetData(sect,this);
	if(WapingChkDlg.DoModal()!=IDOK)
	{
		//    Cell.Get(*sect,TRUE,TRUE);
	}	
}

void CCMSectItemTap::OnChangeMCellFShale()
{
	m_wndGrid4StlG.SaveData();

	//AlignControlDynamic();
	//ControlsShowHide();
	EnableDisableStlgMCell();
	Dlg2DataMCellEtcCtrl();
	DrawStlgViewDlgSection();
	//ChangeText();  
	m_wndGrid4StlG.ShowData();
}

void CCMSectItemTap::OnChangeMCellConnectSize()
{ 
	m_wndGrid4StlG.SaveData();

	//AlignControlDynamic();
	//ControlsShowHide();
	EnableDisableStlgMCell();
	Dlg2DataMCellEtcCtrl();  
	CSectUtil::StlGSymmetricSectAutoCalc(m_Data, FALSE);
	Data2DlgMCellJoint();  
	DrawStlgViewDlgSection();  
	m_wndGrid4StlG.ShowData();
}

void CCMSectItemTap::OnCmdSectWapingCheckUser()
{
	UpdateData(TRUE);
	m_Data.SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
	m_Data.SectBefore.nWarpingCheck[1] =m_nWarpingCheck;

	if(m_nWarpingCheck==0) // Auto  
	{
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->EnableWindow(TRUE);
	}

}

void CCMSectItemTap::SectWapingCheckPSC()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}

void CCMSectItemTap::OnCmdCalcSectPscValPropI()
{
	if(m_Data.SectBefore.Shape != D_SECT_SHAPE_PSC_VALUE) return;

	T_SECT_D SectD;

	BOOL bError=FALSE;
	if(Dlg2Data())
	{
		SectD = m_Data;
		SectD.nStype = D_SECT_TYPE_PSC;
		SectD.SectBefore.SectI = m_Data.SectBefore.SectI;
		SectD.SectBefore.SectJ.Initialize();

		if(m_Data.SectBefore.SectI.aOuterPolygon.GetSize()==0)
			bError = TRUE;
		else if(!CSectUtil::CalcSectData(SectD, TRUE, FALSE, FALSE, TRUE/*bReCalc*/))
			bError = TRUE; 

		m_Data.SectBefore.SectI = SectD.SectBefore.SectI;

		double dQy, dQz;
		if (CSectUtil::GetPscValueDataQ(&SectD, dQy, dQz))
		{
			m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
			m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
		}
	}
	else
	{
		bError =TRUE;
	}

	if(bError)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	m_wndGrid4Psc.ShowData();
}


void CCMSectItemTap::OnCmdCalcSectPscValPropJ()
{
	if(m_Data.SectBefore.Shape != D_SECT_SHAPE_PSC_VALUE) return;
	
	T_SECT_D SectD;

	BOOL bError=FALSE;
	if(Dlg2Data())
	{
		SectD = m_Data;
		SectD.nStype = D_SECT_TYPE_PSC;
		SectD.SectBefore.SectI = m_Data.SectBefore.SectJ;
		SectD.SectBefore.SectJ.Initialize();

		if(m_Data.SectBefore.SectJ.aOuterPolygon.GetSize()==0)
			bError = TRUE;
		else if(!CSectUtil::CalcSectData(SectD, TRUE, FALSE, FALSE, TRUE/*bReCalc*/))
			bError = TRUE; 

		m_Data.SectBefore.SectJ = SectD.SectBefore.SectI;

		double dQy, dQz;
		if (CSectUtil::GetPscValueDataQ(&SectD, dQy, dQz))
		{
			m_Data.SectBefore.SectJ.Stiffness.Qyb = dQy;
			m_Data.SectBefore.SectJ.Stiffness.Qzb = dQz;
		}
	}
	else
	{
		bError =TRUE;
	}

	if(bError)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	m_wndGrid4Psc.ShowData();
}

BOOL CCMSectItemTap::ConfirmBeforeImportCG()
{
	// offset data backup
	/*int nOffsetPoint = m_Data.SectBefore.nOffsetPoint;
	int nOffsetCenter = m_Data.SectBefore.nOffsetCenter;
	int nUserOffsetRef = m_Data.SectBefore.nUserOffsetRef;
	int nHorzOffsetOpt = m_Data.SectBefore.nHorzOffsetOpt;
	int nVertOffsetOpt = m_Data.SectBefore.nVertOffsetOpt;
	double dUserDefOffsetY[2], dUserDefOffsetZ[2];
	for (int i=0; i<2; i++)
	{
	dUserDefOffsetY[i] = m_Data.SectBefore.dUserDefOffsetY[i];
	dUserDefOffsetZ[i] = m_Data.SectBefore.dUserDefOffsetZ[i];
	}

	// save size
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon;
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	CArray<int, int> aLineLoop;
	CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape;
	CArray<T_GSEC_PART, T_GSEC_PART&> aGeneralPart;

	int nBeforePart = m_Data.SectBefore.SectI.nBeforePart;
	aOuterPolygon.Copy(m_Data.SectBefore.SectI.aOuterPolygon);
	aInnerPolygon.Copy(m_Data.SectBefore.SectI.aInnerPolygon);
	aLine.Copy(m_Data.SectBefore.SectI.aLine);
	aLineLoop.Copy(m_Data.SectBefore.SectI.aLineLoop);
	aShape.Copy(m_Data.SectBefore.SectI.aShape);
	aGeneralPart.Copy(m_Data.SectBefore.SectI.aGeneralPart);

	T_SECT_SECTBASE_D SectBaseCopy;
	SectBaseCopy.Initialize();
	SectBaseCopy.Stiffness = m_Data.SectAfter.SectI.Stiffness;
	SectBaseCopy.PeriOut = m_Data.SectAfter.SectI.PeriOut;
	SectBaseCopy.PeriIn = m_Data.SectAfter.SectI.PeriIn;
	SectBaseCopy.Design.YBar = m_Data.SectAfter.SectI.Design.YBar;
	SectBaseCopy.Design.ZBar = m_Data.SectAfter.SectI.Design.ZBar;

	T_SECT_SECTBASE_D SectBaseCopyBefore;
	SectBaseCopyBefore.Initialize();
	SectBaseCopyBefore.Stiffness = m_Data.SectBefore.SectI.Stiffness;
	SectBaseCopyBefore.PeriOut = m_Data.SectBefore.SectI.PeriOut;
	SectBaseCopyBefore.PeriIn = m_Data.SectBefore.SectI.PeriIn;
	SectBaseCopyBefore.Design.YBar = m_Data.SectBefore.SectI.Design.YBar;
	SectBaseCopyBefore.Design.ZBar = m_Data.SectBefore.SectI.Design.ZBar;

	// save data
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_COMPO_G;
	m_Data.SectBefore.Shape = D_SECT_SHAPE_COMPO_G;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_ttol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// Shape
	//   int nShapeIndex = GetCurShapeIndex();
	//   if (nShapeIndex == LB_ERR) 
	//   {
	//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Shape_is_not_selected_));
	//     return FALSE;
	//   }
	//   m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
	//   if (m_Data.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT)
	//   {
	//     CString csCellNum;
	//     m_editCellNum.GetWindowText(csCellNum);
	//     m_Data.SectBefore.nCellShape = _ttoi(csCellNum);
	//   }

	// Built-Up
	//   if (m_wndBuiltUp.GetCheck() == 1) // later
	//     m_Data.SectBefore.SectI.BuiltUpFlag = 1;  // built-up
	//   else m_Data.SectBefore.SectI.BuiltUpFlag = 2; // rolled

	// Offset
	m_Data.SectBefore.nOffsetPoint = nOffsetPoint;
	m_Data.SectBefore.nOffsetCenter = nOffsetCenter;
	m_Data.SectBefore.nUserOffsetRef = nUserOffsetRef; // MQC:1493 20071130 mylee
	m_Data.SectBefore.nHorzOffsetOpt = nHorzOffsetOpt;
	m_Data.SectBefore.nVertOffsetOpt = nVertOffsetOpt;
	for (i=0; i<2; i++)
	{
	m_Data.SectBefore.dUserDefOffsetY[i] = dUserDefOffsetY[i];
	m_Data.SectBefore.dUserDefOffsetZ[i] = dUserDefOffsetZ[i];
	}

	// consider shear deformation
	m_Data.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();

	// size, peri and stiffness
	m_Data.SectBefore.SectI.Stiffness.WArea = m_Data.SectBefore.SectI.Stiffness.Area;

	// general section
	m_Data.SectAfter.SectI.Stiffness = SectBaseCopy.Stiffness;
	m_Data.SectAfter.SectI.PeriOut = SectBaseCopy.PeriOut;
	m_Data.SectAfter.SectI.PeriIn = SectBaseCopy.PeriIn;
	m_Data.SectAfter.SectI.Design.YBar = SectBaseCopy.Design.YBar;
	m_Data.SectAfter.SectI.Design.ZBar = SectBaseCopy.Design.ZBar;

	m_Data.SectBefore.SectI.Stiffness = SectBaseCopyBefore.Stiffness;
	m_Data.SectBefore.SectI.PeriOut = SectBaseCopyBefore.PeriOut;
	m_Data.SectBefore.SectI.PeriIn = SectBaseCopyBefore.PeriIn;
	m_Data.SectBefore.SectI.Design.YBar = SectBaseCopyBefore.Design.YBar;
	m_Data.SectBefore.SectI.Design.ZBar = SectBaseCopyBefore.Design.ZBar;

	m_Data.SectBefore.SectI.nBeforePart = nBeforePart;
	m_Data.SectBefore.SectI.aOuterPolygon.Copy(aOuterPolygon);
	m_Data.SectBefore.SectI.aInnerPolygon.Copy(aInnerPolygon);
	m_Data.SectBefore.SectI.aLine.Copy(aLine);
	m_Data.SectBefore.SectI.aLineLoop.Copy(aLineLoop);
	m_Data.SectBefore.SectI.aShape.Copy(aShape);
	m_Data.SectBefore.SectI.aGeneralPart.Copy(aGeneralPart);
	//CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, m_Data.SectBefore.nCalcStiffOpt);
	m_Data.SectBefore.nCalcStiffOpt = 1;

	// before part
	m_Data.SectBefore.SectI.nBeforePart = m_cmbBeforePart.GetCurSel()+1;

	// Before 에 해당하는 강성을 SectBefore 에 복사한다. (EditData 에도 해주는 부분 있음)
	int nTmp = m_Data.SectBefore.SectI.nBeforePart - 1;
	if(nTmp<aGeneralPart.GetSize())
	{
	m_Data.SectBefore.SectI.Stiffness = aGeneralPart[nTmp].StiffnessSum;
	m_Data.SectBefore.SectI.PeriOut = aGeneralPart[nTmp].PeriOutSum;
	m_Data.SectBefore.SectI.PeriIn = aGeneralPart[nTmp].PeriInSum;
	m_Data.SectBefore.SectI.Design.YBar = aGeneralPart[nTmp].YBarSum;
	m_Data.SectBefore.SectI.Design.ZBar = aGeneralPart[nTmp].ZBarSum;
	}*/

	return TRUE;
}

void CCMSectItemTap::OnSelchangeCmdPartSel() 
{
	// I 단
	int nPartSize = m_Data.SectBefore.SectI.aGeneralPart.GetSize();  
	int nIndex = m_cmbPart.GetCurSel();

	if (nPartSize>0)
	{
		if(!m_bReCalc) m_wndGridComGenI.SaveData();

		if(nIndex==0) // Total
		{
			//if(nPartSize==0)
			{
				m_wndGridComGenI.SetDataSource(&m_Data.SectAfter.SectI, 0, D_SECT_TYPE_COMPO_G, &m_Data.SectAfter.nCellShape);
			}
			//     else
			//     {
			//       int nPart = nPartSize-1;
			//       m_wndGridGeneral.SetDataSource2(&m_Data.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
			//         &m_Data.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
			//     }
		}
		else if(nIndex < nPartSize+1) // Part 1, Part 2...
		{
			int nPart = nIndex-1;
			m_wndGridComGenI.SetDataSource2(&m_Data.SectBefore.SectI.aGeneralPart[nPart].Stiffness,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriOut,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriIn,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].YBar,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].ZBar,
				&m_Data.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
		}
		else if(nIndex < (2*nPartSize-1) ) // Part 1+2, Part 1+2+3...
		{
			int nPart = nIndex-nPartSize;
			m_wndGridComGenI.SetDataSource2(&m_Data.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
				&m_Data.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
				&m_Data.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
		}
		else
			ASSERT(0);

		m_wndGridComGenI.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		//m_wndGridComGenI.SetPSCDesign(m_PSCDesign.bCompPSC);
		m_wndGridComGenI.ShowData();
	}

	// J 단
	nPartSize = m_Data.SectBefore.SectJ.aGeneralPart.GetSize();  
	if (nPartSize>0)
	{
		if(!m_bReCalc) m_wndGridComGenJ.SaveData();

		if(nIndex==0) // Total
		{
			//if(nPartSize==0)
			{
				m_wndGridComGenJ.SetDataSource(&m_Data.SectAfter.SectJ, 0, D_SECT_TYPE_COMPO_G, &m_Data.SectAfter.nCellShape);
			}
			//     else
			//     {
			//       int nPart = nPartSize-1;
			//       m_wndGridGeneral.SetDataSource2(&m_Data.SectBefore.SectI.aGeneralPart[nPart].StiffnessSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriOutSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].PeriInSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].YBarSum,
			//         &m_Data.SectBefore.SectI.aGeneralPart[nPart].ZBarSum,
			//         &m_Data.SectBefore.SectI, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
			//     }
		}
		else if(nIndex < nPartSize+1) // Part 1, Part 2...
		{
			int nPart = nIndex-1;
			m_wndGridComGenJ.SetDataSource2(&m_Data.SectBefore.SectJ.aGeneralPart[nPart].Stiffness,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriOut,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriIn,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].YBar,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].ZBar,
				&m_Data.SectBefore.SectJ, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
		}
		else if(nIndex < (2*nPartSize-1) ) // Part 1+2, Part 1+2+3...
		{
			int nPart = nIndex-nPartSize;
			m_wndGridComGenJ.SetDataSource2(&m_Data.SectBefore.SectJ.aGeneralPart[nPart].StiffnessSum,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriOutSum,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].PeriInSum,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].YBarSum,
				&m_Data.SectBefore.SectJ.aGeneralPart[nPart].ZBarSum,
				&m_Data.SectBefore.SectJ, D_SECT_TYPE_COMPO_G, &m_Data.SectBefore.nCellShape);
		}
		else
			ASSERT(0);

		m_wndGridComGenJ.SetWarpingEffect(m_Data.SectBefore.bConsiderWarpingEffect);
		//m_wndGridComGenJ.SetPSCDesign(m_PSCDesign.bCompPSC);
		m_wndGridComGenJ.ShowData();
	}

	m_bReCalc = FALSE;
}

void CCMSectItemTap::OnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	pNMUpDown->iDelta = -pNMUpDown->iDelta;
	CDlgUtil::SetSpinValue(this, IDC_CMD_SECT_MCELL_NO_CELL_EDT, D_SECT_TP_STLG_MCEL_NO_MIN, D_SECT_TP_STLG_MCEL_NO_MAX, pNMHDR, pResult);

	*pResult = 0;
}

void CCMSectItemTap::CreateOrDestoryStlgView()
{
	int nShapeIdx = GetCurShapeIndex();
	if(!IsStlgSect())
	{
		DestroyStlgView();
		return;
	}

	if(nShapeIdx-D_SECT_STLG_SHAPE_INDEX == 2) // MCell
	{
		m_Data.SectBefore.nStype = D_SECT_TYPE_STLG_MCELL;

		CreateStlgView();
		Dlg2DataMCellEtcCtrl();
		m_pStlgViewer->ChangeBitmap();
	}
	else
	{
		DestroyStlgView();
	}
}

void CCMSectItemTap::ReDrawStlgViewer() 
{
	if(m_pStlgViewer)
	{
		m_pStlgViewer->RedrawSection();
	}
}


void CCMSectItemTap::DrawStlgViewDlgSection() 
{
	CreateStlgView();  
	m_pStlgViewer->ChangeBitmap();
	m_pStlgViewer->RedrawSection();
}

void CCMSectItemTap::DestroyStlgView() 
{
	// Window가 있으면 없앤다.
	if(m_pStlgViewer != NULL)
	{
		m_pStlgViewer->DestroyWindow();
		//-------------------------------------------
		// m_pViewer가 CCMDlgBase를 상속받았으므로
		// 명시적인 delete을 하면 안된단다.. 이크~~~ 
		// delete m_pViewer;    
		m_pStlgViewer = NULL;    
	}
}

void CCMSectItemTap::CreateStlgView() 
{
	if(m_pStlgViewer != NULL) return ;
	HWND hWnd = m_pStlgViewer->GetSafeHwnd();
	if(hWnd) return ;
	if((::IsWindow(hWnd))) return ;

	m_pStlgViewer = new CCMSectItemStlgView();
	m_pStlgViewer->SetInitPos(D_INIT_POS_RT);
	m_pStlgViewer->Create(IDD_CMD_SECT_ITEM_STLG_VIEW, this);
	m_pStlgViewer->ShowWindow(SW_SHOW);
	m_pStlgViewer->SetDataSource(&m_Data);
}

void CCMSectItemTap::OnCmdSectChangeBtn()
{
	UpdateData(TRUE);
	CString csName1, csName2;
	m_wndFirstName.GetWindowText(csName1);
	m_wndSecondName.GetWindowText(csName2);
	m_wndFirstName.SetWindowText(csName2);
	m_wndSecondName.SetWindowText(csName1);
	OnChangeFirstName();
	OnChangeSecondName();
}