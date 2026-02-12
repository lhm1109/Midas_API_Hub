// ElemParamChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemParamChangeDlg.h"
#include "ElemParamChangeRefVecDlg.h"
#include "ElemParamSectOffsetDlg.h"  //Add by ZHOUMIN
#include "StructElementDlg.h"

#include "..\wg_base\wg_base_ModelessDlgCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_gr\GRenderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_EP_LX  _LS(IDS_WG_TREEMENU_Loc_x)
#define D_EP_LY  _LS(IDS_WG_TREEMENU_Loc_y)
#define D_EP_LZ  _LS(IDS_WG_TREEMENU_Loc_z)

#define STR_WALL_FIXED  _LS(IDS_TM_WALL_ID_FIXED)
#define STR_WALL_INCR1  _LS(IDS_TM_WALL_ID_AUTO) 
#define STR_WALL_LAST1  _LS(IDS_TM_WALL_ID_LAST1)

#define TM_TENS_CABLE_PRETENS    _LS(IDS_TM_ELEM_CREATE_Pretension)
#define TM_TENS_CABLE_HORIZ      _LS(IDS_TM_ELEM_CREATE_Horizontal)
#define TM_TENS_CABLE_LU         _LS(IDS_TM_ELEM_CREATE_Lu)
#define TM_FRAME_EL    0
/////////////////////////////////////////////////////////////////////////////
// CElemParamChangeDlg dialog


CElemParamChangeDlg::CElemParamChangeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemParamChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemParamChangeDlg)
	m_nType = 0;
	m_nMode = 0;
	m_csNoInc = _T("1");
	m_csBetaAngle = _T("0");
	m_csBetaInc = _T("0");
	m_nBetaAngleAssignType = 0;
	m_nDefOrCopy = 0;
	m_nCopyType =  0;
	m_nMode2 = 0;
	m_nAxis = 0;
	m_nRefAngle = 0;
	//}}AFX_DATA_INIT
	m_MatlCtrl.Add(IDC_TM_MATL_NO);
	m_MatlCtrl.Add(IDC_TM_MATL_NAME);
	m_MatlCtrl.Add(IDC_TM_MATL_BTN);

	m_SectCtrl.Add(IDC_TM_SECT_NO);
	m_SectCtrl.Add(IDC_TM_SECT_NAME);
	m_SectCtrl.Add(IDC_TM_SECT_BTN);

	m_ThikCtrl.Add(IDC_TM_THIK_NO);
	m_ThikCtrl.Add(IDC_TM_THIK_NAME);
	m_ThikCtrl.Add(IDC_TM_THIK_BTN);

	m_BetaCtrl.Add(IDC_TM_BETA_RADIO_BETAANGLE);
	m_BetaCtrl.Add(IDC_TM_BETA_RADIO_REFPOINT);
	m_BetaCtrl.Add(IDC_TM_BETA_RADIO_VECTOR);  
	m_BetaCtrl.Add(IDC_TM_BETA_EDT_REFPOINT);
	m_BetaCtrl.Add(IDC_TM_BETA_EDT_VECTOR);
	m_BetaCtrl.Add(IDC_TM_BETA_TITLE1);
	m_BetaCtrl.Add(IDC_TM_BETA_ANGLE);
	m_BetaCtrl.Add(IDC_TM_BETA_UNIT1);
	m_BetaCtrl.Add(IDC_TM_BETA_REFPNT_UNIT);
	m_BetaCtrl.Add(IDC_TM_BETA_VECTOR_UNIT);
	//m_BetaCtrl.Add(IDC_TM_BETA_REF_VECTOR_BTN);
	m_BetaCtrl.Add(IDC_TM_BETA_TITLE2);
	m_BetaCtrl.Add(IDC_TM_BETA_INC);
	m_BetaCtrl.Add(IDC_TM_BETA_UNIT2);
	m_BetaCtrl.Add(IDC_TM_BETA_RADIO_COORD);
	m_BetaCtrl.Add(IDC_TM_BETA_AXIS_GRP);
	m_BetaCtrl.Add(IDC_TM_BETA_X_AXIS_RDO);
	m_BetaCtrl.Add(IDC_TM_BETA_Y_AXIS_RDO);
	m_BetaCtrl.Add(IDC_TM_BETA_DIR_GRP);
	m_BetaCtrl.Add(IDC_TM_BETA_DIR_STC);
	m_BetaCtrl.Add(IDC_TM_BETA_DIR_CMB);
	m_BetaCtrl.Add(IDC_TM_BETA_ORG_POINT_STC);
	m_BetaCtrl.Add(IDC_TM_BETA_ORG_POINT_EDT);
	m_BetaCtrl.Add(IDC_TM_BETA_ORG_POINT_UNT);
	m_BetaCtrl.Add(IDC_TM_MODE_ASSIGN2);
	m_BetaCtrl.Add(IDC_TM_MODE_CHANGE2);
	m_BetaCtrl.Add(IDC_TM_LAST_CTRL2);
	m_BetaCtrl.Add(IDC_TM_BETA_ELEM_TYPE_STC);
	m_BetaCtrl.Add(IDC_TM_BETA_ELEM_TYPE_CMB);
	m_BetaCtrl.Add(IDC_TM_GCS_RDO);
	m_BetaCtrl.Add(IDC_TM_REF_ANGLE_RDO);
	m_BetaCtrl.Add(IDC_TM_REF_ANGLE_EDT);
	m_BetaCtrl.Add(IDC_TM_REF_ANGLE_UNT);
	m_BetaCtrl.Add(IDC_TM_REF_AXIS_CMB);

	m_ModeCtrl.Add(IDC_TM_NO_STATIC);
	m_ModeCtrl.Add(IDC_TM_NAME_STATIC);
	m_ModeCtrl.Add(IDC_TM_INC_STATIC);
	m_ModeCtrl.Add(IDC_TM_INC_EDIT);
	m_ModeCtrl.Add(IDC_TM_DEFINE_RADIO);
	m_ModeCtrl.Add(IDC_TM_COPY_RADIO);
	m_ModeCtrl.Add(IDC_TM_COPYPER_STATIC);
	m_ModeCtrl.Add(IDC_TM_COPYPER_ELEM_RADIO);
	m_ModeCtrl.Add(IDC_TM_COPYPER_PROP_RADIO);

	m_CopyCtrl.Add(IDC_TM_COPYPER_STATIC);
	m_CopyCtrl.Add(IDC_TM_COPYPER_ELEM_RADIO);
	m_CopyCtrl.Add(IDC_TM_COPYPER_PROP_RADIO);

	m_RefAngleAxis.Add(IDC_TM_GCS_RDO);
	m_RefAngleAxis.Add(IDC_TM_REF_ANGLE_RDO);
	m_RefAngleAxis.Add(IDC_TM_REF_ANGLE_EDT);
	m_RefAngleAxis.Add(IDC_TM_REF_ANGLE_UNT);
	m_RefAngleAxis.Add(IDC_TM_REF_AXIS_CMB);

	m_aRefAngle.Add(IDC_TM_REF_ANGLE_EDT);
	m_aRefAngle.Add(IDC_TM_REF_ANGLE_UNT);

	m_aRefAxis.Add(IDC_TM_REF_AXIS_CMB);

	m_AssignCtrl.Add(IDC_TM_NO_STATIC);
	m_AssignCtrl.Add(IDC_TM_NAME_STATIC);
	m_AssignCtrl.Add(IDC_TM_MATL_NO);
	m_AssignCtrl.Add(IDC_TM_MATL_NAME);
	m_AssignCtrl.Add(IDC_TM_MATL_BTN);
	m_AssignCtrl.Add(IDC_TM_SECT_NO);
	m_AssignCtrl.Add(IDC_TM_SECT_NAME);
	m_AssignCtrl.Add(IDC_TM_SECT_BTN);
	m_AssignCtrl.Add(IDC_TM_THIK_NO);
	m_AssignCtrl.Add(IDC_TM_THIK_NAME);
	m_AssignCtrl.Add(IDC_TM_THIK_BTN);
	m_AssignCtrl.Add(IDC_TM_WALL_ASSIGN_TITLE);
	m_AssignCtrl.Add(IDC_TM_WALL_ASSIGN_EDIT);   // For Wall ID added by bugboy 2000.8.27
	//m_AssignCtrl.Add(IDC_TM_BETA_REF_VECTOR_BTN);
	m_AssignCtrl.Add(IDC_TM_DEFINE_RADIO);
	m_AssignCtrl.Add(IDC_TM_COPY_RADIO);
	m_AssignCtrl.Add(IDC_TM_COPYPER_STATIC);
	m_AssignCtrl.Add(IDC_TM_COPYPER_ELEM_RADIO);
	m_AssignCtrl.Add(IDC_TM_COPYPER_PROP_RADIO);	

	m_ChangeCtrl.Add(IDC_TM_INC_STATIC);
	m_ChangeCtrl.Add(IDC_TM_INC_EDIT);
	m_ChangeCtrl.Add(IDC_TM_WALL_CHANGE_TITLE);
	m_ChangeCtrl.Add(IDC_TM_WALL_CHANGE_EDIT);   // For Wall ID added by bugboy 2000.8.27
	//m_ChangeCtrl.Add(IDC_TM_W_ID_AUTOINC_CHECK);

	m_AssignCtrl2.Add(IDC_TM_BETA_RADIO_BETAANGLE);
	m_AssignCtrl2.Add(IDC_TM_BETA_RADIO_REFPOINT);
	m_AssignCtrl2.Add(IDC_TM_BETA_RADIO_VECTOR);  
	m_AssignCtrl2.Add(IDC_TM_BETA_EDT_REFPOINT);
	m_AssignCtrl2.Add(IDC_TM_BETA_EDT_VECTOR);
	m_AssignCtrl2.Add(IDC_TM_BETA_TITLE1);
	m_AssignCtrl2.Add(IDC_TM_BETA_ANGLE);
	m_AssignCtrl2.Add(IDC_TM_BETA_UNIT1);
	m_AssignCtrl2.Add(IDC_TM_BETA_REFPNT_UNIT);
	m_AssignCtrl2.Add(IDC_TM_BETA_VECTOR_UNIT);
	m_AssignCtrl2.Add(IDC_TM_BETA_RADIO_COORD);
	m_AssignCtrl2.Add(IDC_TM_BETA_AXIS_GRP);
	m_AssignCtrl2.Add(IDC_TM_BETA_X_AXIS_RDO);
	m_AssignCtrl2.Add(IDC_TM_BETA_Y_AXIS_RDO);
	m_AssignCtrl2.Add(IDC_TM_BETA_DIR_GRP);
	m_AssignCtrl2.Add(IDC_TM_BETA_DIR_STC);
	m_AssignCtrl2.Add(IDC_TM_BETA_DIR_CMB);
	m_AssignCtrl2.Add(IDC_TM_BETA_ORG_POINT_STC);
	m_AssignCtrl2.Add(IDC_TM_BETA_ORG_POINT_EDT);
	m_AssignCtrl2.Add(IDC_TM_BETA_ORG_POINT_UNT);

	m_ChangeCtrl2.Add(IDC_TM_BETA_TITLE2);
	m_ChangeCtrl2.Add(IDC_TM_BETA_INC);
	m_ChangeCtrl2.Add(IDC_TM_BETA_UNIT2);	

	m_CoordDirCtrl.Add(IDC_TM_BETA_RADIO_COORD);
	m_CoordDirCtrl.Add(IDC_TM_BETA_AXIS_GRP);
	m_CoordDirCtrl.Add(IDC_TM_BETA_X_AXIS_RDO);
	m_CoordDirCtrl.Add(IDC_TM_BETA_Y_AXIS_RDO);
	m_CoordDirCtrl.Add(IDC_TM_BETA_DIR_GRP);
	m_CoordDirCtrl.Add(IDC_TM_BETA_DIR_STC);
	m_CoordDirCtrl.Add(IDC_TM_BETA_DIR_CMB);
	m_CoordDirCtrl.Add(IDC_TM_BETA_ORG_POINT_STC);
	m_CoordDirCtrl.Add(IDC_TM_BETA_ORG_POINT_EDT);
	m_CoordDirCtrl.Add(IDC_TM_BETA_ORG_POINT_UNT);

	m_ElemTypeCtrl.Add(IDC_TM_FROM_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_FROM_COMBO);
	m_ElemTypeCtrl.Add(IDC_TM_TO_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_TO_COMBO);
	m_ElemTypeCtrl.Add(IDC_TM_TT_RADIO_TRUSS);
	m_ElemTypeCtrl.Add(IDC_TM_TT_RADIO_HOOK);
	m_ElemTypeCtrl.Add(IDC_TM_TT_RADIO_CABLE);
	m_ElemTypeCtrl.Add(IDC_TM_TT_DIST_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_TT_DIST_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_DIST_UNIT);
	/*
	m_ElemTypeCtrl.Add(IDC_TM_TT_FORCE_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_FORCE_UNIT);
	*/
	m_ElemTypeCtrl.Add(IDC_TM_TT_ALCOMP_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_TT_ALCOMP_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_ALCOMP_UNIT);
	m_ElemTypeCtrl.Add(IDC_TM_TENS_LMT_CHK); // MNET:2774 20070509 mylee
	m_ElemTypeCtrl.Add(IDC_TM_TT_LIMIT_EDT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_LIMIT_UNT);
	//*^^*
	m_ElemTypeCtrl.Add(IDC_TM_TT_LNL_COMBO);
	m_ElemTypeCtrl.Add(IDC_TM_TT_LENGTH_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_LENGTH_UNIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_TT_FORCE_UNIT);
	
	m_ElemTypeCtrl.Add(IDC_TM_CT_RADIO_TRUSS);
	m_ElemTypeCtrl.Add(IDC_TM_CT_RADIO_GAP);
	m_ElemTypeCtrl.Add(IDC_TM_CT_DIST_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_CT_DIST_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_CT_DIST_UNIT);
	m_ElemTypeCtrl.Add(IDC_TM_CT_ALTENS_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_CT_ALTENS_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_CT_ALTENS_UNIT);
	m_ElemTypeCtrl.Add(IDC_TM_COMP_LMT_CHK); // MNET:2774 20070509 mylee
	m_ElemTypeCtrl.Add(IDC_TM_CC_LIMIT_EDT);
	m_ElemTypeCtrl.Add(IDC_TM_CC_LIMIT_UNT);
	m_ElemTypeCtrl.Add(IDC_TM_W_RADIO_MEMBR);
	m_ElemTypeCtrl.Add(IDC_TM_W_RADIO_PLATE);
	m_ElemTypeCtrl.Add(IDC_TM_W_ID_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_W_ID_EDIT);
	m_ElemTypeCtrl.Add(IDC_TM_ELEM_TYPE_TITLE);
	m_ElemTypeCtrl.Add(IDC_TM_ELEM_TYPE_THICK);
	m_ElemTypeCtrl.Add(IDC_TM_ELEM_TYPE_THIN);
	m_ElemTypeCtrl.Add(IDC_TM_ELEM_WITH_DRILL_CHK);

	m_FromToCtrl.Add(IDC_TM_FROM_TITLE);
	m_FromToCtrl.Add(IDC_TM_FROM_COMBO);
	m_FromToCtrl.Add(IDC_TM_TO_TITLE);
	m_FromToCtrl.Add(IDC_TM_TO_COMBO);

	m_TTCtrl.Add(IDC_TM_TT_RADIO_TRUSS);
	m_TTCtrl.Add(IDC_TM_TT_RADIO_HOOK);
	m_TTCtrl.Add(IDC_TM_TT_RADIO_CABLE);
	m_TTCtrl.Add(IDC_TM_TT_DIST_TITLE);
	m_TTCtrl.Add(IDC_TM_TT_DIST_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_DIST_UNIT);
	/*
	m_TTCtrl.Add(IDC_TM_TT_FORCE_TITLE);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_UNIT);
	*/
	m_TTCtrl.Add(IDC_TM_TT_ALCOMP_TITLE);
	m_TTCtrl.Add(IDC_TM_TT_ALCOMP_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_ALCOMP_UNIT);
	m_TTCtrl.Add(IDC_TM_TENS_LMT_CHK); // MNET:XXXX 20070531 mylee
	m_TTCtrl.Add(IDC_TM_TT_LIMIT_EDT);
	m_TTCtrl.Add(IDC_TM_TT_LIMIT_UNT);
	// *^^*
	m_TTCtrl.Add(IDC_TM_TT_LNL_COMBO);
	m_TTCtrl.Add(IDC_TM_TT_LENGTH_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_LENGTH_UNIT);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_UNIT);

	m_TTAlCompCtrl.Add(IDC_TM_TT_ALCOMP_TITLE);
	m_TTAlCompCtrl.Add(IDC_TM_TT_ALCOMP_EDIT);
	m_TTAlCompCtrl.Add(IDC_TM_TT_ALCOMP_UNIT);
	m_TTAlCompCtrl.Add(IDC_TM_TENS_LMT_CHK); // MNET:XXXX 20070531 mylee
	m_TTAlCompCtrl.Add(IDC_TM_TT_LIMIT_EDT);
	m_TTAlCompCtrl.Add(IDC_TM_TT_LIMIT_UNT);
	m_TTDistCtrl.Add(IDC_TM_TT_DIST_TITLE);
	m_TTDistCtrl.Add(IDC_TM_TT_DIST_EDIT);
	m_TTDistCtrl.Add(IDC_TM_TT_DIST_UNIT);
	/*
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_TITLE);
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_UNIT);
	*/
	//*^^*
	m_TTCableCtrl1.Add(IDC_TM_TT_LNL_COMBO);
	m_TTCableCtrl1.Add(IDC_TM_TT_LENGTH_EDIT);
	m_TTCableCtrl1.Add(IDC_TM_TT_LENGTH_UNIT);
	m_TTCableCtrl2.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTCableCtrl2.Add(IDC_TM_TT_FORCE_UNIT);

	m_CTCtrl.Add(IDC_TM_CT_RADIO_TRUSS);
	m_CTCtrl.Add(IDC_TM_CT_RADIO_GAP);
	m_CTCtrl.Add(IDC_TM_CT_DIST_TITLE);
	m_CTCtrl.Add(IDC_TM_CT_DIST_EDIT);
	m_CTCtrl.Add(IDC_TM_CT_DIST_UNIT);
	m_CTCtrl.Add(IDC_TM_CT_ALTENS_TITLE);
	m_CTCtrl.Add(IDC_TM_CT_ALTENS_EDIT);
	m_CTCtrl.Add(IDC_TM_CT_ALTENS_UNIT);
	m_CTCtrl.Add(IDC_TM_COMP_LMT_CHK); // MNET:2774 20070509 mylee
	m_CTCtrl.Add(IDC_TM_CC_LIMIT_UNT);
	m_CTCtrl.Add(IDC_TM_CC_LIMIT_EDT);
	m_CTAlTensCtrl.Add(IDC_TM_CT_ALTENS_TITLE);
	m_CTAlTensCtrl.Add(IDC_TM_CT_ALTENS_EDIT);
	m_CTAlTensCtrl.Add(IDC_TM_CT_ALTENS_UNIT);
	m_CTAlTensCtrl.Add(IDC_TM_COMP_LMT_CHK); // MNET:2774 20070509 mylee
	m_CTAlTensCtrl.Add(IDC_TM_CC_LIMIT_UNT);
	m_CTAlTensCtrl.Add(IDC_TM_CC_LIMIT_EDT);
	m_CTDistCtrl.Add(IDC_TM_CT_DIST_TITLE);
	m_CTDistCtrl.Add(IDC_TM_CT_DIST_EDIT);
	m_CTDistCtrl.Add(IDC_TM_CT_DIST_UNIT);

	m_WallCtrl.Add(IDC_TM_W_RADIO_MEMBR     );
	m_WallCtrl.Add(IDC_TM_W_RADIO_PLATE     );
	m_WallCtrl.Add(IDC_TM_W_ID_TITLE        );
	m_WallCtrl.Add(IDC_TM_W_ID_EDIT         );
	
#ifndef _TOWER  
	m_RevCtrl.Add(IDC_TM_REV_TITLE);
	m_RevCtrl.Add(IDC_TM_REV_FRAME);
	m_RevCtrl.Add(IDC_TM_REV_PLANAR);
	m_RevCtrl.Add(IDC_TM_REV_SOLID);
#endif
	m_RevCtrl.Add(IDC_TM_REV_CHECK);

	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_TRUSS);
	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_HOOK);
	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_CABLE);

	m_CTRadioCtrl.Add(IDC_TM_CT_RADIO_TRUSS);
	m_CTRadioCtrl.Add(IDC_TM_CT_RADIO_GAP);

	m_WallRadioCtrl.Add(IDC_TM_W_RADIO_MEMBR);
	m_WallRadioCtrl.Add(IDC_TM_W_RADIO_PLATE);

	m_RevRadioCtrl.Add(IDC_TM_REV_FRAME);
	m_RevRadioCtrl.Add(IDC_TM_REV_PLANAR);
	m_RevRadioCtrl.Add(IDC_TM_REV_SOLID);

	m_ModeRadioCtrl.Add(IDC_TM_MODE_ASSIGN);
	m_ModeRadioCtrl.Add(IDC_TM_MODE_CHANGE);
	
	m_ThikRadioCtrl.Add(IDC_TM_PT_THIK);
	
	m_PlaneSolidCtrl.Add(IDC_TM_REV_PLANAR);
	m_PlaneSolidCtrl.Add(IDC_TM_REV_SOLID);

	m_arWallCtrl.Add(IDC_TM_WALL_ASSIGN_TITLE);
	m_arWallCtrl.Add(IDC_TM_WALL_ASSIGN_EDIT);
	m_arWallCtrl.Add(IDC_TM_WALL_CHANGE_TITLE);
	m_arWallCtrl.Add(IDC_TM_WALL_CHANGE_EDIT);
	m_arWallCtrl.Add(IDC_TM_WALL_ASSIGN_COMBO);
	//m_arWallCtrl.Add(IDC_TM_W_ID_AUTOINC_CHECK);

	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ELEM_TITLE);
	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ELEM_EDIT);
	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ORDER_TITLE);
	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ORDER_1ST);
	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ORDER_2ND);
	m_arAlignElemCtrl.Add(IDC_TM_ALIGN_ORDER_3RD);
	m_arAlignElemCtrl.Add(IDC_TM_ORDER_1ST);
	m_arAlignElemCtrl.Add(IDC_TM_ORDER_2ND);
	m_arAlignElemCtrl.Add(IDC_TM_ORDER_3RD);

#if defined(_CIVIL) && defined(_CH)
	m_arEccentCtrl.Add(IDC_TM_ECCENT_BTN);   //Add by ZHOUMIN


	m_arLastCtrl.Add(IDC_TM_LAST_CTRL);
	m_arLastCtrl.Add(IDC_TM_MODE_ASSIGN);
	m_arLastCtrl.Add(IDC_TM_DEFINE_RADIO);
	m_arLastCtrl.Add(IDC_TM_COPY_RADIO);
	m_arLastCtrl.Add(IDC_TM_NO_STATIC);
	m_arLastCtrl.Add(IDC_TM_NAME_STATIC);
	m_arLastCtrl.Add(IDC_TM_MATL_NO);
	m_arLastCtrl.Add(IDC_TM_MATL_NAME);
	m_arLastCtrl.Add(IDC_TM_MATL_BTN);
	m_arLastCtrl.Add(IDC_TM_MODE_CHANGE);
	m_arLastCtrl.Add(IDC_TM_INC_STATIC);
	m_arLastCtrl.Add(IDC_TM_INC_EDIT);
#endif

	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_TITLE);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THICK);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THIN);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_WITH_DRILL_CHK);
	
	m_arPlstrsTypeCtrl.Add(IDC_TM_ELEM_WITH_DRILL_CHK);

	sOrder=_LS(IDS_WG_TREEMENU_XYZ);
	m_pRefVecDlg = NULL;
	 /////////Add by ZHOUMIN
	m_bEccentChangeFlag = FALSE;       
	m_SectData.Initialize();
	m_nElemType = 0;
	m_nCoordDir = 2;
	 ////////////
}

CElemParamChangeDlg::~CElemParamChangeDlg()
{
	if(m_pRefVecDlg)
	{
		m_pRefVecDlg->DestroyWindow();
	}
	m_pRefVecDlg = NULL;
}

void CElemParamChangeDlg::DoDataExchange(CDataExchange* pDX)
{
#ifdef _CIVIL
	if (!pDX->m_bSaveAndValidate)
	{
		if (m_nType >= 4) m_nType--;
	}
#endif
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemParamChangeDlg)
	DDX_Control(pDX, IDC_TM_TT_LENGTH_EDIT, m_wndTTLengthEdit);
	DDX_Control(pDX, IDC_TM_TT_LENGTH_UNIT, m_wndTTLengthUnit);
	DDX_Control(pDX, IDC_TM_TT_LNL_COMBO, m_wndTTLnLCobx);
	DDX_Control(pDX, IDC_TM_WALL_ASSIGN_COMBO, m_cboWall);
	DDX_Control(pDX, IDC_TM_WALL_ASSIGN_EDIT, m_editWall);
	DDX_Control(pDX, IDC_TM_BETA_EDT_REFPOINT, m_wndRefPoint);
	DDX_Control(pDX, IDC_TM_BETA_REFPNT_UNIT, m_wndRefPointDistUnit);
	DDX_Control(pDX, IDC_TM_BETA_EDT_VECTOR, m_wndDirVectorEdit);
	DDX_Control(pDX, IDC_TM_BETA_VECTOR_UNIT, m_wndVectorUnit);
	DDX_Control(pDX, IDC_TM_WALL_CHANGE_EDIT, m_edtWallInc);
	DDX_Control(pDX, IDC_TM_ORDER_3RD, m_cboOrder3);
	DDX_Control(pDX, IDC_TM_ORDER_2ND, m_cboOrder2);
	DDX_Control(pDX, IDC_TM_ORDER_1ST, m_cboOrder1);   // For Align Order
	DDX_Control(pDX, IDC_TM_ALIGN_ELEM_EDIT, m_edtAlignLocal);
	DDX_Control(pDX, IDC_TM_W_ID_EDIT, m_wndWallID);
	DDX_Control(pDX, IDC_TM_TT_FORCE_EDIT, m_wndTTForceEdit);
	DDX_Control(pDX, IDC_TM_CT_DIST_EDIT, m_wndCTDistEdit);
	DDX_Control(pDX, IDC_TM_TT_DIST_EDIT, m_wndTTDistEdit);
	DDX_Control(pDX, IDC_TM_CT_DIST_UNIT, m_wndCTDistUnit);
	DDX_Control(pDX, IDC_TM_TT_FORCE_UNIT, m_wndTTForceUnit);
	DDX_Control(pDX, IDC_TM_TT_DIST_UNIT, m_wndTTDistUnit);
	DDX_Control(pDX, IDC_TM_TT_ALCOMP_EDIT, m_wndTTAlCompEdit);
	DDX_Control(pDX, IDC_TM_CT_ALTENS_EDIT, m_wndCTAlTensEdit);
	DDX_Control(pDX, IDC_TM_TT_LIMIT_EDT, m_edtTensLimit); // MNET:2774 20070509 mylee
	DDX_Control(pDX, IDC_TM_CC_LIMIT_EDT, m_edtCompLimit);
	DDX_Control(pDX, IDC_TM_CT_ALTENS_UNIT, m_wndCTAlTensUnit);
	DDX_Control(pDX, IDC_TM_TT_ALCOMP_UNIT, m_wndTTAlCompUnit);
	DDX_Control(pDX, IDC_TM_CC_LIMIT_UNT, m_untCompLimit);  // MNET:2774 20070509 mylee
	DDX_Control(pDX, IDC_TM_TT_LIMIT_UNT, m_untTensLimit);
	DDX_Control(pDX, IDC_TM_TO_COMBO, m_wndElTypeTo);
	DDX_Control(pDX, IDC_TM_FROM_COMBO, m_wndElTypeFrom);
	DDX_Control(pDX, IDC_TM_REV_CHECK, m_wndRevCheck);  
	DDX_Control(pDX, IDC_TM_BETA_ANGLE, m_wndBetaAngle);  
	DDX_Control(pDX, IDC_TM_THIK_BTN, m_wndThikBtn);
	DDX_Control(pDX, IDC_TM_SECT_BTN, m_wndSectBtn);
	DDX_Control(pDX, IDC_TM_MATL_BTN, m_wndMatlBtn);
	DDX_Control(pDX, IDC_TM_THIK_NAME, m_wndThikName);
	DDX_Control(pDX, IDC_TM_THIK_NO, m_wndThikNo);
	DDX_Control(pDX, IDC_TM_SECT_NAME, m_wndSectName);
	DDX_Control(pDX, IDC_TM_SECT_NO, m_wndSectNo);
	DDX_Control(pDX, IDC_TM_MATL_NAME, m_wndMatlName);
	DDX_Control(pDX, IDC_TM_MATL_NO, m_wndMatlNo);
	DDX_Radio  (pDX, IDC_TM_PT_MATL, m_nType);
	DDX_Radio  (pDX, IDC_TM_MODE_ASSIGN, m_nMode);
	DDX_Text   (pDX, IDC_TM_INC_EDIT, m_csNoInc);
	DDX_Text   (pDX, IDC_TM_BETA_ANGLE, m_csBetaAngle);
	DDX_Text   (pDX, IDC_TM_BETA_INC, m_csBetaInc);
	DDX_Radio  (pDX, IDC_TM_BETA_RADIO_BETAANGLE, m_nBetaAngleAssignType);
	DDX_Radio  (pDX, IDC_TM_DEFINE_RADIO, m_nDefOrCopy);
	DDX_Radio  (pDX, IDC_TM_COPYPER_PROP_RADIO, m_nCopyType);
	DDX_Control(pDX, IDC_TM_BETA_ELEM_TYPE_CMB, m_cmbElemType);
	DDX_Radio  (pDX, IDC_TM_MODE_ASSIGN2, m_nMode2);
	DDX_Radio  (pDX, IDC_TM_BETA_X_AXIS_RDO, m_nAxis);
	DDX_Control(pDX, IDC_TM_BETA_DIR_CMB, m_cmbCoordDir);
	DDX_Control(pDX, IDC_TM_BETA_ORG_POINT_EDT, m_edtPoint);
	DDX_Control(pDX, IDC_TM_BETA_ORG_POINT_UNT, m_untPoint);
	DDX_Radio(pDX, IDC_TM_GCS_RDO, m_nRefAngle);
	DDX_Control(pDX, IDC_TM_REF_ANGLE_EDT, m_edtRefAngle);
	DDX_Control(pDX, IDC_TM_REF_ANGLE_UNT, m_untRefAngle);
	DDX_Control(pDX, IDC_TM_REF_AXIS_CMB, m_cmbRefAxis);
	//}}AFX_DATA_MAP
#ifdef _CIVIL
	if (pDX->m_bSaveAndValidate)
	{
		if (m_nType >= 3) m_nType++;
	}
#endif
#ifdef _TOWER
	if (pDX->m_bSaveAndValidate)
	{
		if (m_nType >= 2) m_nType += 2;
	}
#endif

}

void CElemParamChangeDlg::ChangeStateOfElemTypeSubCtrls(int nType)
{
	if (nType != 5) return;
	int nIndex = m_wndElTypeTo.GetCurSel(); ASSERT(nIndex >= 0);
	int nElemType = m_wndElTypeTo.GetItemData(nIndex);

	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bNormalElem = FALSE;
	BOOL bTensTruss = FALSE;
	BOOL bCompTruss = FALSE;
	BOOL bWall = FALSE;
	BOOL bPlate = FALSE;
	BOOL bPlstrs = FALSE;
	if (pDoc->m_pAttrCtrl->IsTensTruss(nElemType)) bTensTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsCompTruss(nElemType)) bCompTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsWall(nElemType)) bWall = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlate(nElemType)) bPlate = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlstrs(nElemType)) bPlstrs = TRUE;
	else bNormalElem = TRUE;
	
	CtrlShowHide(m_TTCtrl,  bTensTruss);
	if (bTensTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_TTRadioCtrl, nCheck);
		CtrlShowHide(m_TTAlCompCtrl,  nCheck == 0);
		CtrlShowHide(m_TTDistCtrl,  nCheck == 1);
		//CtrlShowHide(m_TTForceCtrl,  nCheck == 2);
		//CtrlShowHide(m_TTForceCtrl,  FALSE);
		CtrlShowHide(m_TTCableCtrl1,  nCheck == 2);
		CtrlShowHide(m_TTCableCtrl2,  nCheck == 2);
		if (nCheck == 2) ChangeTensCableCombo();
	}

	CtrlShowHide(m_CTCtrl,  bCompTruss);
	if (bCompTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_CTRadioCtrl, nCheck);
		CtrlShowHide(m_CTAlTensCtrl,  nCheck == 0);
		CtrlShowHide(m_CTDistCtrl,  nCheck == 1);
	}
	CtrlShowHide(m_WallCtrl, bWall);
	if (bWall)
	{
		int nSel = m_wndElTypeFrom.GetCurSel();
		int nFromType = m_wndElTypeFrom.GetItemData(nSel);
		nSel = m_wndElTypeTo.GetCurSel();
		int nToType = m_wndElTypeTo.GetItemData(nSel);

		if (pDoc->m_pAttrCtrl->IsWall(nFromType) &&
				pDoc->m_pAttrCtrl->IsWall(nToType)    )
				m_wndWallID.EnableWindow(FALSE);
		else m_wndWallID.EnableWindow(TRUE);
	}
	CtrlShowHide(m_arPlateTypeCtrl, bPlate);
	CtrlShowHide(m_arPlstrsTypeCtrl, bPlstrs || bPlate);

}

void CElemParamChangeDlg::InitializeElemTypeSubCtrls()
{
	// Set Default for Elem Type sub controls
	CDlgUtil::CtrlRadioSetCheck(this, m_TTRadioCtrl, 0);    // default : truss
	CDlgUtil::CtrlRadioSetCheck(this, m_CTRadioCtrl, 0);    // default : truss
	CDlgUtil::CtrlRadioSetCheck(this, m_WallRadioCtrl, 0);  // default : membrane

	m_wndTTDistEdit.SetAttUcsDistance();
	m_wndTTDistEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndTTDistEdit.SetDistValueMode(TRUE);
	m_wndCTDistEdit.SetAttUcsDistance();
	m_wndCTDistEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndCTDistEdit.SetDistValueMode(TRUE);
	m_wndTTAlCompEdit.SetWindowText(_T("0"));
	m_wndCTAlTensEdit.SetWindowText(_T("0"));
	m_edtTensLimit.SetWindowText(_T("0"));
	m_edtCompLimit.SetWindowText(_T("0"));
	m_wndTTDistEdit.SetWindowText(_T("0"));
	m_wndTTForceEdit.SetWindowText(_T("0"));
	m_wndCTDistEdit.SetWindowText(_T("0"));
	m_wndWallID.SetWindowText(_T("1"));
	m_wndTTLengthEdit.SetWindowText(_T("0"));
	
	m_wndTTAlCompUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTAlTensUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndTTAlCompEdit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTAlTensEdit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_untTensLimit.SetUnitType(D_UNITSYS_BASE_FORCE); // MNET:2774 20070509 mylee
	m_untCompLimit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtTensLimit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtCompLimit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_wndTTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTTForceUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndRefPointDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndVectorUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTTLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPoint.SetAttUcsPos();
	m_edtPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edtPoint.SetEnterNotifyWindow(this);
	m_edtPoint.SetLButtonDownNotifyWindow(this);
	m_untPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_untRefAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CElemParamChangeDlg::SetFromComboBox()
{
	m_wndElTypeFrom.ResetContent();
#ifdef _MGEN
	int aElTypeID[] = {
		TRUSS_EL, TRTENS_EL, TRCOMP_EL, BEAM_EL, PLATE_EL, PLSTRS_EL, WALL_EL,
	};
#elif _CIVIL
	int aElTypeID[] = {
		TRUSS_EL, TRTENS_EL, TRCOMP_EL, BEAM_EL, PLATE_EL, PLSTRS_EL,
	};
#elif _TOWER
	int aElTypeID[] = {
		TRUSS_EL,// TRTENS_EL, TRCOMP_EL, BEAM_EL, PLATE_EL, PLSTRS_EL,
	};
#endif
	int nIndex;
	int nSize = sizeof(aElTypeID)/sizeof(int);
	for (int i = 0; i < nSize; i++)
	{
		nIndex = m_wndElTypeFrom.AddString(CDBLib::GetEltypName(aElTypeID[i]));
		m_wndElTypeFrom.SetItemData(nIndex, aElTypeID[i]);
	}

	CString str;
	CSize   sz;
	int     dx=0;
	CDC* pDC = m_wndElTypeFrom.GetDC();
	for(int i=0; i<m_wndElTypeFrom.GetCount(); ++i)
	{
		m_wndElTypeFrom.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}  
	m_wndElTypeFrom.SetDroppedWidth(dx);
}

void CElemParamChangeDlg::SetToComboBox(int nFromElTypeID)
{
	m_wndElTypeTo.ResetContent();
#ifdef _MGEN
	int aElTypeID[][4] = {
		{ TRTENS_EL,  TRCOMP_EL,  BEAM_EL,    -1      },    //  Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  BEAM_EL },    //  Tension Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  BEAM_EL },    //  Compression Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  -1      },    //  Beam
		{ PLATE_EL,   PLSTRS_EL,  WALL_EL,    -1      },    //  Plate
		{ PLATE_EL,   PLSTRS_EL,  WALL_EL,    -1      },    //  Plane Stress
		{ PLATE_EL,   PLSTRS_EL,  WALL_EL,    -1      },    //  Wall
	};
#elif _CIVIL
	int aElTypeID[][4] = {
		{ TRTENS_EL,  TRCOMP_EL,  BEAM_EL,    -1      },    //  Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  BEAM_EL },    //  Tension Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  BEAM_EL },    //  Compression Truss
		{ TRUSS_EL,   TRTENS_EL,  TRCOMP_EL,  -1      },    //  Beam
		{ PLATE_EL,   PLSTRS_EL,       -1,    -1      },    //  Plate
		{ PLATE_EL,   PLSTRS_EL,       -1,    -1      },    //  Plane Stress
	};
#elif _TOWER
	int aElTypeID[][4] = {
		{ TRUSS_EL,  -1,  -1,    -1      },    //  Truss
		{ TRUSS_EL,  -1,  -1,  -1  },    //  Tension Truss
		{ TRUSS_EL,  -1,  -1,  -1  },    //  Compression Truss
		{ TRUSS_EL,  -1,  -1,  -1  },    //  Beam
		{ TRUSS_EL,  -1,   -1,    -1      },    //  Plate
		{ TRUSS_EL,        -1,    -1,         -1      },    //  Plane Stress
	};
#endif

	int nIx;
#ifdef _MVIL
	switch(nFromElTypeID)
	{
	case TRUSS_EL : nIx = 0; break;
	case BEAM_EL  : nIx = 1; break;
	default: ASSERT(0); break;
	}
#elif _TOWER
	switch(nFromElTypeID)
	{
	case TRUSS_EL : nIx = 0; break;
	default: ASSERT(0); break;
	}
#else
	switch(nFromElTypeID)
	{
	case TRUSS_EL : nIx = 0; break;
	case TRTENS_EL: nIx = 1; break;
	case TRCOMP_EL: nIx = 2; break;
	case BEAM_EL  : nIx = 3; break;
	case PLATE_EL : nIx = 4; break;
	case PLSTRS_EL: nIx = 5; break;
	case WALL_EL  : nIx = 6; break;
	default: ASSERT(0); break;
	}
#endif
	int nIndex;
	for (int i = 0; i < 4; i++)
	{
		if (aElTypeID[nIx][i] < 0) continue;

		nIndex = m_wndElTypeTo.AddString(CDBLib::GetEltypName(aElTypeID[nIx][i]));
		m_wndElTypeTo.SetItemData(nIndex, aElTypeID[nIx][i]);
	}

	CString str;
	CSize   sz;
	int     dx=0;
	CDC* pDC = m_wndElTypeTo.GetDC();
	for(int i=0; i<m_wndElTypeTo.GetCount(); ++i)
	{
		m_wndElTypeTo.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}  
	m_wndElTypeTo.SetDroppedWidth(dx);
}

//void CElemParamChangeDlg::AlignControlLocalAxis(int nElemType)
//{
//#ifndef _CIVIL || _MEC 
//	return;
//#endif
//
//	CRect rRef;
//	CRect rToMove;
//	int nDistY;
//	CRect rRefBottom;
//
//	CArray<UINT, UINT> BetaChange;
//	BetaChange.Add(IDC_TM_MODE_CHANGE2);
//	BetaChange.Add(IDC_TM_BETA_TITLE2);
//	BetaChange.Add(IDC_TM_BETA_INC);
//	BetaChange.Add(IDC_TM_BETA_UNIT2);
//
//	if (nElemType == TM_FRAME_EL ||
//		nElemType == PLATE_EL || nElemType == PLSTRS_EL)
//	{
//		// Orientation Coordinate 
//		GetDlgItem(IDC_TM_BETA_RADIO_COORD)->GetWindowRect(rRef);
//		GetDlgItem(IDC_TM_MODE_CHANGE2)->GetWindowRect(rToMove);
//		nDistY = rRef.top - rToMove.top;
//		CtrlMoveDistY(BetaChange, nDistY);
//
//		GetDlgItem(IDC_TM_BETA_INC)->GetWindowRect(rRefBottom);
//	}
//	else if (nElemType == PLSTRN_EL)
//	{
//		GetDlgItem(IDC_TM_REF_ANGLE_EDT)->GetWindowRect(rRefBottom);
//	}
//	else if (nElemType == SOLID_EL)
//	{
//		GetDlgItem(IDC_TM_REF_AXIS_CMB)->GetWindowRect(rRefBottom);
//	}
//
//	// Grp box Resize
//	CRect rectResize;
//	GetDlgItem(IDC_TM_LAST_CTRL2)->GetWindowRect(rectResize);
//	rectResize.bottom = rRefBottom.bottom + globalUtils.ScaleByDPI(8);
//	ScreenToClient(rectResize);
//	GetDlgItem(IDC_TM_LAST_CTRL2)->MoveWindow(rectResize);
//
//}
#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemParamChangeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemParamChangeDlg)
	ON_BN_CLICKED(IDC_TM_PT_MATL, OnTmPtMatl)
	ON_BN_CLICKED(IDC_TM_PT_SECT, OnTmPtSect)
	ON_BN_CLICKED(IDC_TM_PT_THIK, OnTmPtThik)
	ON_BN_CLICKED(IDC_TM_MODE_ASSIGN, OnTmModeAssign)
	ON_BN_CLICKED(IDC_TM_MODE_CHANGE, OnTmModeChange)
	ON_BN_CLICKED(IDC_TM_MATL_BTN, OnTmMatlBtn)
	ON_BN_CLICKED(IDC_TM_SECT_BTN, OnTmSectBtn)
	ON_BN_CLICKED(IDC_TM_THIK_BTN, OnTmThikBtn)
	ON_BN_CLICKED(IDC_TM_PT_BETA, OnTmPtBeta)
	ON_BN_CLICKED(IDC_TM_PT_ELEM_TYPE, OnTmPtElemType)
	ON_BN_CLICKED(IDC_TM_PT_REVERSE, OnTmPtReverse)
	ON_CBN_SELCHANGE(IDC_TM_FROM_COMBO, OnChangeFromType)
	ON_CBN_SELCHANGE(IDC_TM_TO_COMBO, OnChangeToType)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_TRUSS, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_TRUSS, OnChangeCT)
	ON_BN_CLICKED(IDC_TM_REV_FRAME, OnTmRevFrame)
	ON_BN_CLICKED(IDC_TM_REV_PLANAR, OnTmRevNoFrame)
	ON_BN_CLICKED(IDC_TM_PT_WALL, OnTmPtWall)
	ON_BN_CLICKED(IDC_TM_PT_ALIGNELEM, OnTmPtAlignelem)
	ON_CBN_SELCHANGE(IDC_TM_ORDER_1ST, OnSelchangeTmOrder1st)
	ON_CBN_SELCHANGE(IDC_TM_ORDER_2ND, OnSelchangeTmOrder2nd)
	ON_BN_CLICKED(IDC_TM_BETA_RADIO_BETAANGLE, OnTmBetaTypeRadio_BetaAngle)
	ON_BN_CLICKED(IDC_TM_W_ID_AUTOINC_CHECK, OnTmWIdAutoincCheck)
	ON_BN_CLICKED(IDC_TM_DEFINE_RADIO, OnTmDefCopyRadio)
	ON_CBN_SELCHANGE(IDC_TM_WALL_ASSIGN_COMBO, OnSelchangeTmWallAssignCombo)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_HOOK, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_CABLE, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_GAP, OnChangeCT)
	ON_BN_CLICKED(IDC_TM_REV_SOLID, OnTmRevNoFrame)
	ON_BN_CLICKED(IDC_TM_BETA_RADIO_REFPOINT, OnTmBetaTypeRadio_RefPoint)
	ON_BN_CLICKED(IDC_TM_BETA_RADIO_VECTOR, OnTmBetaTypeRadio_Vector)
	ON_BN_CLICKED(IDC_TM_COPYPER_STATIC, OnTmDefCopyRadio)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmDefCopyRadio)
	ON_CBN_SELCHANGE(IDC_TM_TT_LNL_COMBO, OnSelchangeTmTtLnlCombo)
	ON_BN_CLICKED(IDC_TM_TENS_LMT_CHK, OnTmClickTensLimit) // MNET:XXXX 20070531 mylee
	ON_BN_CLICKED(IDC_TM_COMP_LMT_CHK, OnTmClickCompLimit)
	ON_BN_CLICKED(IDC_TM_PT_ECCENT, OnChangeEccent)  // ZHOUMIN 20151016
	ON_BN_CLICKED(IDC_TM_ECCENT_BTN, OnTmEccentBtn)
	ON_CBN_SELCHANGE(IDC_TM_BETA_ELEM_TYPE_CMB, OnTmElemTypeCombo)
	ON_BN_CLICKED(IDC_TM_MODE_ASSIGN2, OnTmModeAssign2)
	ON_BN_CLICKED(IDC_TM_MODE_CHANGE2, OnTmModeChange2)
	ON_BN_CLICKED(IDC_TM_BETA_RADIO_COORD, OnTmBetaTypeRadio_Coord)
	ON_BN_CLICKED(IDC_TM_BETA_X_AXIS_RDO, OnTmLocalAxisRdo)
	ON_BN_CLICKED(IDC_TM_BETA_Y_AXIS_RDO, OnTmLocalAxisRdo)
ON_BN_CLICKED(IDC_TM_GCS_RDO, OnTmRefAngleAxisRdo)
	ON_BN_CLICKED(IDC_TM_REF_ANGLE_RDO, OnTmRefAngleAxisRdo)
	ON_CBN_SELCHANGE(IDC_TM_BETA_DIR_CMB, OnTmRefDirCmb)
	ON_BN_CLICKED(IDC_TM_REF_BUTTON, OnTmRefButton)
	//ON_BN_CLICKED(IDC_TM_BETA_REF_VECTOR_BTN, OnTmClickRefVectorBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemParamChangeDlg message handlers

int CElemParamChangeDlg::GetWallMode()
{
	CString strSel = _T("");
	int nIndex = m_cboWall.GetCurSel();
	if(nIndex < 0) return -1;

	m_cboWall.GetLBText(nIndex, strSel);
	if(strSel == STR_WALL_FIXED) return 0;
	else if(strSel == STR_WALL_INCR1) return 1;
	else if(strSel == STR_WALL_LAST1) return 2;

	return -1;
}

BOOL CElemParamChangeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_nType = 0;
	AlignControl();

	InitTensCableCombo();

	CString aItem[] = {STR_WALL_FIXED, STR_WALL_INCR1, STR_WALL_LAST1};

	m_cboWall.ResetContent();
	int nCount = sizeof(aItem) / sizeof(CString);
	for(int i=0; i<nCount; i++)
		m_cboWall.AddString(aItem[i]);

	m_cboWall.SelectString(-1, STR_WALL_INCR1);
	int nMode = GetWallMode();
	m_editWall.SetMode(nMode);
	m_editWall.SetWindowText(_T("1"));

	m_wndBetaAngle.AddString(_T("0"));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_180));

	m_wndRefPoint.SetAttUcsPos();
	m_wndRefPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndRefPoint.SetEnterNotifyWindow(this);
	m_wndRefPoint.SetLButtonDownNotifyWindow(this);

	m_wndDirVectorEdit.SetAttUcsDistance();
	m_wndDirVectorEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDirVectorEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	// matl/sect/this combo-edit link
	m_wndMatlNo.SetComboBoxPoint(&m_wndMatlName);
	m_wndSectNo.SetComboBoxPoint(&m_wndSectName);
	m_wndThikNo.SetComboBoxPoint(&m_wndThikName);
	m_wndMatlName.SetEditBoxPoint(&m_wndMatlNo);
	m_wndSectName.SetEditBoxPoint(&m_wndSectNo);
	m_wndThikName.SetEditBoxPoint(&m_wndThikNo);
	m_wndMatlNo.SetWindowText(_T("1"));
	m_wndSectNo.SetWindowText(_T("1"));
	m_wndThikNo.SetWindowText(_T("1"));

#ifndef _MEC
	m_cmbElemType.ResetContent();
	m_cmbElemType.AddString(_LS(IDS_TM_Frame));
	m_cmbElemType.AddString(_LS(IDS_TM_Planar));
	m_cmbElemType.SetCurSel(m_nElemType);
#else
	CArray<UINT, UINT> aElemType; aElemType.RemoveAll();
	aElemType.Add(PLATE_EL);
	aElemType.Add(PLSTRS_EL);
	aElemType.Add(PLSTRN_EL);
	aElemType.Add(SOLID_EL);
	for (int i = 0; i < aElemType.GetSize(); i++)
	{
		m_cmbElemType.SetItemData(m_cmbElemType.AddString(CDBLib::GetEltypName(aElemType[i])), aElemType[i]);
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbElemType);
	m_cmbElemType.SetCurSel(0);
#endif

	CString aPlaneDir[] = {_LSX(+X), _LSX(+Y), _LSX(+Z), _LSX(+R), _LSX(+TH),_LSX(-X), _LSX(-Y), _LSX(-Z), _LSX(-R), _LSX(-TH)};

	m_cmbCoordDir.ResetContent();
	int nSize=10;
	int nIndex = 0;
	for(int i=0; i < nSize; i++)
	{
		nIndex = m_cmbCoordDir.AddString(aPlaneDir[i]);
		m_cmbCoordDir.SetItemData(nIndex, i);
	}
	m_cmbCoordDir.SetCurSel(m_nCoordDir);

#ifdef _MEC
	m_edtRefAngle.AddString(_T("0"));
	m_edtRefAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_edtRefAngle.AddString(_LS(IDS_WG_TREEMENU_180));
	InitComboNucs();
#endif

	// control show/hide according to change parameter type
	CtrlShowHide(m_ModeRadioCtrl, m_nType != 4 && m_nType != 5 && m_nType != 6 && m_nType != 7);
	CtrlShowHide(m_MatlCtrl, m_nType == 0);
	CtrlShowHide(m_SectCtrl, m_nType == 1);
	CtrlShowHide(m_ThikCtrl, m_nType == 2);
	CtrlShowHide(m_arWallCtrl, m_nType == 3);	  // Wall ID 3	
	CtrlShowHide(m_BetaCtrl, m_nType == 4);  GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(m_nType != 4);
//  CtrlShowHide(m_ModeCtrl, m_nType != 4);  // Origin
	CtrlShowHide(m_ModeCtrl, m_nType == 0 || m_nType == 1 || m_nType == 2);
	CtrlShowHide(m_ElemTypeCtrl, m_nType == 5);
	CtrlShowHide(m_RevCtrl, m_nType == 6);
	CtrlShowHide(m_arAlignElemCtrl, m_nType == 7);		// Align Element Local 7
	CtrlShowHide(m_arEccentCtrl, m_nType == 8);           //Add by ZHOUMIN
	m_wndRevCheck.SetCheck(0);
//  m_wndRevCheck.EnableWindow(FALSE);

	CtrlEnableDisable(m_AssignCtrl, m_nMode == 0);
	CtrlEnableDisable(m_ChangeCtrl, m_nMode == 1);
	CtrlEnableDisable(m_AssignCtrl2, m_nMode2 == 0);
	CtrlEnableDisable(m_ChangeCtrl2, m_nMode2 == 1);
	CtrlEnableDisable(m_CoordDirCtrl, FALSE);	

	// Set Default for Elem Type sub controls
	InitializeElemTypeSubCtrls();
	// Set "From", "To" combo box
	SetFromComboBox();
	m_wndElTypeFrom.SetCurSel(0);
	int nElTypeID = m_wndElTypeFrom.GetItemData(0);
	SetToComboBox(nElTypeID);
	m_wndElTypeTo.SetCurSel(0);
	ChangeStateOfElemTypeSubCtrls(m_nType);

	// Set Default for Reverse ...
	CDlgUtil::CtrlRadioSetCheck(this, m_RevRadioCtrl, 0);  // default : frame

#ifdef _MVIL
	CtrlEnableDisable(m_ThikRadioCtrl, FALSE);
	CtrlEnableDisable(m_PlaneSolidCtrl, FALSE);
#endif

	// Align Element Local
	SetOrder();
	m_edtAlignLocal.SetAttElemList();
	m_edtAlignLocal.SetLButtonDownNotifyWindow(this);
	m_edtAlignLocal.SetEnterNotifyWindow(this);
	m_edtAlignLocal.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_edtAlignLocal.SetMaxElemKeyNum(1);

	// Plate Control 초기값
	((CButton *)GetDlgItem(IDC_TM_ELEM_TYPE_THICK))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_TM_ELEM_WITH_DRILL_CHK))->SetCheck(FALSE);
	
	m_nDefOrCopy = 0;
	m_nCopyType =  1;

	OnTmClickTensLimit(); // MNET:XXXX 20070531 mylee
	OnTmClickCompLimit();

	if(m_nType == 4)
	{
		if(m_nMode2 == 0) OnTmModeAssign2();
		else              OnTmModeChange2();
	}
	ChangeText();

	// PMS5010 : 해당기능 사용 안 함
	GetDlgItem(IDC_TM_BETA_REF_VECTOR_BTN)->ShowWindow(SW_HIDE);
	
	UpdateData(FALSE);
	AssignCtrlMan();

#if defined(_CIVIL) 
	#if defined(_CH)
		GetDlgItem(IDC_TM_PT_ECCENT)->ShowWindow(SW_SHOW);
	#else 
		GetDlgItem(IDC_TM_PT_ECCENT)->ShowWindow(SW_HIDE);
	#endif
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemParamChangeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
#ifndef _MEC
	return;
#endif
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CElemParamChangeDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
		{
			InitComboNucs();
		}
		break;
		default:
			break;
		}
	} // end of while
}

void CElemParamChangeDlg::InitComboNucs()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	CArray<T_NUCS_K, T_NUCS_K> aNucsK;
	pDoc->m_pAttrCtrl->GetNucsKeyList(aNucsK);

	T_NUCS_D NucsD; NucsD.Initialize();

	m_cmbRefAxis.ResetContent();
	for (int i = 0; i < aNucsK.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetNucs(aNucsK[i], NucsD)) continue;

		m_cmbRefAxis.SetItemData(m_cmbRefAxis.AddString(NucsD.UcsName), aNucsK[i]);
	}
	m_cmbRefAxis.SetCurSel(0);
}

void CElemParamChangeDlg::AssignCtrlMan()
{
	CArray<UINT, UINT> * pCurArr;
	CArray<UINT, UINT> arNoName;

	arNoName.Add(IDC_TM_NO_STATIC  );
	arNoName.Add(IDC_TM_NAME_STATIC);
	
	switch(m_nType)
	{
	case 0:  // Material 
		pCurArr = &m_MatlCtrl;
		break;
	case 1:  // Section 
		pCurArr = &m_SectCtrl;
		break;
	case 2:  // Thickness 
		pCurArr = &m_ThikCtrl;
		break;
	default:
		break;
	}

	if(m_nType ==  0 || m_nType == 1 || m_nType == 2)
	{
		CtrlShowHide(*pCurArr  , m_nDefOrCopy == 0);
		CtrlShowHide(m_CopyCtrl, m_nDefOrCopy == 1);
		CtrlShowHide(arNoName  , m_nDefOrCopy == 0);
	}
}

void CElemParamChangeDlg::OnTmDefCopyRadio() 
{
	UpdateData();
	AssignCtrlMan();
}

void CElemParamChangeDlg::OnTmPtMatl() 
{
	m_nType = 0;
	AlignControl();

	CtrlShowHide(m_ModeRadioCtrl, TRUE);
	CtrlShowHide(m_ModeCtrl, TRUE);

	CtrlShowHide(m_MatlCtrl, m_nDefOrCopy == 0);
	CtrlShowHide(m_CopyCtrl, m_nDefOrCopy == 1);
	
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	((CBCGPStatic*)GetDlgItem(IDC_TM_NAME_STATIC))->SetWindowText(_LS(IDS_WG_TREEMENU_Name));

	AssignCtrlMan();
}

void CElemParamChangeDlg::ShowHideCtrl_MEC(BOOL bShowFirstModeGrpCtrl)
{
#ifndef _MEC
	return;
#endif
	GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(bShowFirstModeGrpCtrl ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_TM_LAST_CTRL2)->ShowWindow(bShowFirstModeGrpCtrl ? SW_HIDE : SW_SHOW);
}

void CElemParamChangeDlg::OnTmPtSect() 
{
	m_nType = 1;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, TRUE);
	CtrlShowHide(m_ModeCtrl, TRUE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	
	CtrlShowHide(m_SectCtrl, m_nDefOrCopy == 0);
	CtrlShowHide(m_CopyCtrl, m_nDefOrCopy == 1);
	
	CtrlShowHide(m_ThikCtrl, FALSE);	
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	((CBCGPStatic*)GetDlgItem(IDC_TM_NAME_STATIC))->SetWindowText(_LS(IDS_WG_TREEMENU_Name));
	AssignCtrlMan();
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmPtThik() 
{
	m_nType = 2;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, TRUE);
	CtrlShowHide(m_ModeCtrl, TRUE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	
	CtrlShowHide(m_ThikCtrl, m_nDefOrCopy == 0);
	CtrlShowHide(m_CopyCtrl, m_nDefOrCopy == 1);
	
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	((CBCGPStatic*)GetDlgItem(IDC_TM_NAME_STATIC))->SetWindowText(_LS(IDS_WG_TREEMENU_Thickness));
	AssignCtrlMan();
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmPtWall() 
{
	m_nType = 3;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, TRUE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  TRUE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmPtBeta() 
{
	m_nType = 4;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, FALSE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, TRUE);	 GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(FALSE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
#ifndef _MEC
	OnTmBetaTypeRadio();
#else
	OnTmElemTypeCombo();
	ShowHideCtrl_MEC(FALSE);
#endif
}

void CElemParamChangeDlg::OnTmPtElemType() 
{
	m_nType = 5;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, FALSE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, TRUE);
	CtrlShowHide(m_RevCtrl,  FALSE);	
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	ChangeStateOfElemTypeSubCtrls(m_nType);
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmPtReverse() 
{
	m_nType = 6;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, FALSE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  TRUE);
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmPtAlignelem() 
{
	m_nType = 7;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, FALSE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  TRUE);	 // Align Element Local	
	CtrlShowHide(m_arEccentCtrl, FALSE);       //Add by ZHOUMIN
	ShowHideCtrl_MEC(TRUE);
}

void CElemParamChangeDlg::OnTmModeAssign() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_AssignCtrl, TRUE);
	CtrlEnableDisable(m_ChangeCtrl, FALSE);	
}

void CElemParamChangeDlg::OnTmModeChange() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_AssignCtrl, FALSE);
	CtrlEnableDisable(m_ChangeCtrl, TRUE);	
}
void CElemParamChangeDlg::ChangeElemTypeCombo_MEC()
{
#ifndef _MEC
	return;
#endif
	int nElemType = m_cmbElemType.GetItemData(m_cmbElemType.GetCurSel());

	if (nElemType == TRUSS_EL || nElemType == TRTENS_EL || nElemType == TRCOMP_EL || nElemType == BEAM_EL)
	{
		if (m_nBetaAngleAssignType == 3)
		{
			m_nBetaAngleAssignType = 0;
			UpdateData(FALSE);
		}
	}

	if (nElemType == PLATE_EL || nElemType == PLSTRS_EL)
	{
		if (m_nBetaAngleAssignType == 1)
		{
			m_nBetaAngleAssignType = 0;
			UpdateData(FALSE);
		}
	}

	if (nElemType == TRUSS_EL || nElemType == TRTENS_EL || nElemType == TRCOMP_EL || nElemType == BEAM_EL ||
		nElemType == PLATE_EL || nElemType == PLSTRS_EL)
	{
		GetDlgItem(IDC_TM_MODE_ASSIGN2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_MODE_CHANGE2)->ShowWindow(SW_SHOW);
		CtrlShowHide(m_AssignCtrl2, TRUE);
		CtrlShowHide(m_ChangeCtrl2, TRUE);
		CtrlShowHide(m_RefAngleAxis, FALSE);

		CtrlEnableDisable(m_CoordDirCtrl, (nElemType == PLATE_EL || nElemType == PLSTRS_EL) && m_nMode2 == 0);
		GetDlgItem(IDC_TM_BETA_RADIO_REFPOINT)->EnableWindow((nElemType == TRUSS_EL || nElemType == TRTENS_EL || nElemType == TRCOMP_EL || nElemType == BEAM_EL) && m_nMode2 == 0);
		OnTmBetaTypeRadio();
	}
	else if (nElemType == PLSTRN_EL || nElemType == AXISYM_EL)
	{
		GetDlgItem(IDC_TM_MODE_ASSIGN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_MODE_CHANGE2)->ShowWindow(SW_HIDE);
		CtrlShowHide(m_AssignCtrl2, FALSE);
		CtrlShowHide(m_ChangeCtrl2, FALSE);

		CtrlShowHide(m_RefAngleAxis, TRUE);
		CtrlShowHide(m_aRefAngle, TRUE);
		CtrlShowHide(m_aRefAxis, FALSE);

		CtrlEnableDisable(m_aRefAngle, m_nRefAngle == 1);
	}
	else
	{
		GetDlgItem(IDC_TM_MODE_ASSIGN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_MODE_CHANGE2)->ShowWindow(SW_HIDE);
		CtrlShowHide(m_AssignCtrl2, FALSE);
		CtrlShowHide(m_ChangeCtrl2, FALSE);

		CtrlShowHide(m_RefAngleAxis, TRUE);
		CtrlShowHide(m_aRefAngle, FALSE);
		CtrlShowHide(m_aRefAxis, TRUE);

		CtrlEnableDisable(m_aRefAxis, m_nRefAngle == 1);
	}

	ChangeText();
}

void CElemParamChangeDlg::OnTmRefButton()
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);

	pRenderView->CreateAndActivateUcsDlg(7);
}

void CElemParamChangeDlg::OnTmElemTypeCombo() 
{	
#ifdef _MEC
	return ChangeElemTypeCombo_MEC();
#endif
	m_nElemType = m_cmbElemType.GetCurSel();

	if(m_nElemType == 0 && m_nBetaAngleAssignType == 3)
	{
		m_nBetaAngleAssignType = 0;
		UpdateData(FALSE);
	}

	if(m_nElemType == 1 && m_nBetaAngleAssignType == 1)
	{
		m_nBetaAngleAssignType = 0;
		UpdateData(FALSE);
	}

	CtrlEnableDisable(m_CoordDirCtrl, m_nElemType == 1 && m_nMode2 == 0);	
	GetDlgItem(IDC_TM_BETA_RADIO_REFPOINT)->EnableWindow(m_nElemType == 0 && m_nMode2 == 0);
	OnTmBetaTypeRadio();	
}

void CElemParamChangeDlg::OnTmModeAssign2() 
{
	UpdateData(TRUE);
	CtrlEnableDisable(m_AssignCtrl2, TRUE);
	CtrlEnableDisable(m_ChangeCtrl2, FALSE);	
	OnTmElemTypeCombo();
}

void CElemParamChangeDlg::OnTmModeChange2() 
{
	UpdateData(TRUE);
	CtrlEnableDisable(m_AssignCtrl2, FALSE);
	CtrlEnableDisable(m_ChangeCtrl2, TRUE);	
	OnTmElemTypeCombo();
}

void CElemParamChangeDlg::OnTmRefAngleAxisRdo()
{
	UpdateData(TRUE);
	OnTmElemTypeCombo();
}

void CElemParamChangeDlg::OnTmRefDirCmb()
{
	if(m_nMode2 == 1) return;

	// 대화상자 왔다갔다 했을때 정보 유지 해줘야 하나.....
	int nSel = m_cmbCoordDir.GetCurSel();
	if(nSel > 0) m_nCoordDir = nSel;

	GetDlgItem(IDC_TM_BETA_ORG_POINT_STC)->EnableWindow(m_nBetaAngleAssignType == 3 && (m_nCoordDir==3||m_nCoordDir==4||m_nCoordDir==8||m_nCoordDir==9));
	GetDlgItem(IDC_TM_BETA_ORG_POINT_EDT)->EnableWindow(m_nBetaAngleAssignType == 3 && (m_nCoordDir==3||m_nCoordDir==4||m_nCoordDir==8||m_nCoordDir==9));
	GetDlgItem(IDC_TM_BETA_ORG_POINT_UNT)->EnableWindow(m_nBetaAngleAssignType == 3 && (m_nCoordDir==3||m_nCoordDir==4||m_nCoordDir==8||m_nCoordDir==9));
}

void CElemParamChangeDlg::OnTmMatlBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_MATERIAL, 0));
}

void CElemParamChangeDlg::OnTmSectBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_SECTION, 0));
}

void CElemParamChangeDlg::OnTmThikBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_THICKNESS, 0));	
}

void CElemParamChangeDlg::CtrlShowHide(CArray<UINT, UINT> &aCtrl, BOOL bShow)
{
	int nCmd;
	if (bShow) nCmd = SW_SHOW;
	else nCmd = SW_HIDE;

	CWnd* pWnd;
	for (int i = 0; i < aCtrl.GetSize(); i++)
	{
		pWnd = GetDlgItem(aCtrl.GetAt(i));
		if(pWnd == NULL) {ASSERT(0); continue;}

		pWnd->ShowWindow(nCmd);
	}
}

void CElemParamChangeDlg::AlignControl()
{
	CRect rNoEdit, rNoStatic, rNoInc;   // reference
	CRect rToMove;
	CRect rRef, rMove;
	int nDistY;

#if defined(_CIVIL) && defined(_CH)
	int nDistX;
	GetDlgItem(IDC_TM_PT_ALIGNELEM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_PT_ECCENT)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;

	CArray<UINT, UINT> m_arEccentRdo;
  m_arEccentRdo.Add(IDC_TM_PT_ECCENT);
	CDlgUtil::CtrlMoveDistX(this, m_arEccentRdo, nDistX);

	GetDlgItem(IDC_WG_TREEMENU_STATIC0)->GetWindowRect(rRef);
	nDistY = rToMove.bottom - rRef.bottom + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlResizeBottom(this, IDC_WG_TREEMENU_STATIC0, nDistY);

	CtrlMoveDistY(m_arLastCtrl, nDistY);
#endif


	m_wndMatlNo.GetWindowRect(rNoEdit);
	//GetDlgItem(IDC_TM_NO_STATIC)->GetWindowRect(rNoStatic);
	GetDlgItem(IDC_TM_DEFINE_RADIO)->GetWindowRect(rNoStatic);
	GetDlgItem(IDC_TM_INC_STATIC)->GetWindowRect(rNoInc);

	m_wndSectNo.GetWindowRect(rToMove);
	nDistY = rToMove.top - rNoEdit.top;
	CtrlMoveDistY(m_SectCtrl, -nDistY);
	m_wndThikNo.GetWindowRect(rToMove);
	nDistY = rToMove.top - rNoEdit.top;
	CtrlMoveDistY(m_ThikCtrl, -nDistY);
	
// FOR WALL ID Added by bugboy. 2000.8.26
	((CButton*)GetDlgItem(IDC_TM_WALL_ASSIGN_TITLE))->GetWindowRect(rToMove);
	nDistY = rToMove.top - rNoStatic.top;
	CtrlMoveDistY(m_arWallCtrl, -nDistY);
// FOR Align Element  Added by bugboy. 2000.8.26
	((CButton*)GetDlgItem(IDC_TM_ALIGN_ELEM_TITLE))->GetWindowRect(rToMove);
	nDistY = rToMove.top - rNoStatic.top;
	CtrlMoveDistY(m_arAlignElemCtrl, -nDistY);
	
	CRect TRect;
	GetDlgItem(IDC_TM_NO_STATIC)->GetWindowRect(TRect);
	GetDlgItem(IDC_TM_COPYPER_STATIC)->GetWindowRect(rToMove);
	nDistY = TRect.top - rToMove.top;
	CtrlMoveDistY(m_CopyCtrl,nDistY);

	///////////////////////////////////////////////
	//       IDC_TM_WALL_CHANGE_TITLE
	//       IDC_TM_WALL_CHANGE_EDIT
	///////////////////////////////////////////////
	
	rNoInc.top;
	CWnd* pIncEdit   = GetDlgItem(IDC_TM_INC_EDIT         );
	CWnd* pWallEdit  = GetDlgItem(IDC_TM_WALL_CHANGE_EDIT );
	CWnd* pWallTitle = GetDlgItem(IDC_TM_WALL_CHANGE_TITLE);

	if(pIncEdit && pWallEdit && pWallTitle)
	{
		CArray<UINT, UINT> TID;
		CRect rIE,rWE,rWT;
		pIncEdit->GetWindowRect  (rIE);
		pWallEdit->GetWindowRect (rWE);
		pWallTitle->GetWindowRect(rWT);
		int nDY;
		nDY = rNoInc.top - rWT.top;
		TID.Add(IDC_TM_WALL_CHANGE_TITLE);
		CtrlMoveDistY(TID,nDY);
		TID.RemoveAll();
		TID.Add(IDC_TM_WALL_CHANGE_EDIT);
		nDY = rIE.top - rWE.top;
		CtrlMoveDistY(TID,nDY);
	}

//   CArray<UINT, UINT> BetaAssign, BetaChange;
//   BetaAssign.Add(IDC_TM_BETA_RADIO_BETAANGLE);
//   BetaAssign.Add(IDC_TM_BETA_RADIO_REFPOINT);
//   BetaAssign.Add(IDC_TM_BETA_RADIO_VECTOR);  
// 	BetaAssign.Add(IDC_TM_BETA_EDT_REFPOINT);
//   BetaAssign.Add(IDC_TM_BETA_EDT_VECTOR);
//   BetaAssign.Add(IDC_TM_BETA_TITLE1);
//   BetaAssign.Add(IDC_TM_BETA_ANGLE);
//   BetaAssign.Add(IDC_TM_BETA_UNIT1);
//   BetaAssign.Add(IDC_TM_BETA_REFPNT_UNIT);
//   BetaAssign.Add(IDC_TM_BETA_VECTOR_UNIT);
//   //BetaAssign.Add(IDC_TM_BETA_REF_VECTOR_BTN);
//  
// 	BetaChange.Add(IDC_TM_BETA_TITLE2);
//   BetaChange.Add(IDC_TM_BETA_INC);
//   BetaChange.Add(IDC_TM_BETA_UNIT2);
// 
//   GetDlgItem(IDC_TM_BETA_RADIO_BETAANGLE)->GetWindowRect(rToMove);
//   nDistY = rToMove.top - rNoStatic.top;
//   CtrlMoveDistY(BetaAssign, -nDistY);

//   GetDlgItem(IDC_TM_BETA_TITLE2)->GetWindowRect(rToMove);
//   nDistY = rToMove.top - rNoInc.top;
//   CtrlMoveDistY(BetaChange, -nDistY);

	GetDlgItem(IDC_TM_LAST_CTRL)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_LAST_CTRL2)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_BetaCtrl, nDistY);

#ifdef _MEC
	GetDlgItem(IDC_TM_MODE_ASSIGN2)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_GCS_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_RefAngleAxis, nDistY);

	// Ref Axis
	GetDlgItem(IDC_TM_REF_ANGLE_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_REF_AXIS_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_aRefAxis, nDistY);
#endif

	OnTmBetaTypeRadio();

	// Elem Type, Reverse 추가 
	// Tension Force를 Tension Dist와 같은 높이에
	/*
	GetDlgItem(IDC_TM_TT_DIST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_TT_FORCE_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_TTForceCtrl, nDistY);
	*/

	// AlComp를 Tension Dist와 같은 높이에
	GetDlgItem(IDC_TM_TT_DIST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_TT_ALCOMP_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_TTAlCompCtrl, nDistY);

	// Ln/L을 Tension Dist와 같은 높이에
	GetDlgItem(IDC_TM_TT_DIST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_TT_LENGTH_EDIT)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_TTCableCtrl1, nDistY);
	GetDlgItem(IDC_TM_TT_DIST_TITLE)->GetWindowRect(rMove);
	GetDlgItem(IDC_TM_TT_FORCE_EDIT)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_TTCableCtrl2, nDistY);

	// AlTens를 Comp Dist와 같은 높이에
	GetDlgItem(IDC_TM_CT_DIST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_CT_ALTENS_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_CTAlTensCtrl, nDistY);

	// Comp-Truss를 Tens-Truss와 같은 높이에
	GetDlgItem(IDC_TM_TT_RADIO_TRUSS)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_CT_RADIO_TRUSS)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_CTCtrl, nDistY);

	// Wall을 Tens-Truss와 같은 높이에
	GetDlgItem(IDC_TM_TT_RADIO_TRUSS)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_W_RADIO_MEMBR)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_WallCtrl, nDistY);

	// Plate Type 관련 Control 을 같은 높이에
	GetDlgItem(IDC_TM_TT_RADIO_TRUSS)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ELEM_TYPE_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_arPlateTypeCtrl, nDistY);

	// Plane Stress 관련 Control 을 같은 높이에
	GetDlgItem(IDC_TM_TT_RADIO_TRUSS)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ELEM_TYPE_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_arPlstrsTypeCtrl, nDistY);

	// Element Type 전체를 Assign과 같은 높이에
	GetDlgItem(IDC_TM_MODE_ASSIGN)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_FROM_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_ElemTypeCtrl, nDistY);

	// Reverse를 Assign과 같은 높이에
	if(m_nType == 4) GetDlgItem(IDC_TM_MODE_ASSIGN2)->GetWindowRect(rRef);
	else             GetDlgItem(IDC_TM_MODE_ASSIGN )->GetWindowRect(rRef); 
	
#ifdef _TOWER  
	GetDlgItem(IDC_TM_REV_CHECK)->GetWindowRect(rMove);
#else
	GetDlgItem(IDC_TM_REV_TITLE)->GetWindowRect(rMove);
#endif

	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_RevCtrl, nDistY);

	////////////////////////////////////////////////Add by ZHOUMIN
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_ECCENT_BTN);
	if(pBtn != NULL)  pBtn->GetWindowRect(rToMove);
	
	nDistY = rToMove.top - rRef.top;
	CtrlMoveDistY(m_arEccentCtrl, -nDistY);
	 ////////////////////////////////////////////////

	// resize self size
	CWnd* pWndLast;
	if(m_nType == 4) pWndLast = GetDlgItem(IDC_TM_LAST_CTRL2);
	else             pWndLast = GetDlgItem(IDC_TM_LAST_CTRL);
	
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+5;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);

	CRect rectParent;
	GetWindowRect(rectParent);
	((CStructElementDlg*)(GetParent()->GetParent()))->SetHolderSize(rectParent.Height());

	RedrawWindow();
}

BOOL CElemParamChangeDlg::GetParamElemType(_ElemParam &EP)
{
	int nSel = m_wndElTypeFrom.GetCurSel();
	if (nSel == LB_ERR) return FALSE;
	EP.m_nFromType = m_wndElTypeFrom.GetItemData(nSel);

	nSel = m_wndElTypeTo.GetCurSel();
	if (nSel == LB_ERR) return FALSE;
	EP.m_nToType = m_wndElTypeTo.GetItemData(nSel);

	EP.m_nSubType = 0;
	EP.m_dSubData = 0.0;
	EP.m_dSubData2 = 0.0;
	EP.m_nWallID = 0;
	EP.m_nCableOpt = 0;

	switch(EP.m_nToType)
	{
	case TRTENS_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_TTRadioCtrl, nCheck);
			EP.m_nSubType = nCheck+1;
			if (EP.m_nSubType == 1)  // Truss
			{
				EP.m_dSubData = m_wndTTAlCompEdit.GetEditValue();
				int nCheck = ((CButton*)GetDlgItem(IDC_TM_TENS_LMT_CHK))->GetCheck();
				EP.m_bUseLimit = (nCheck==1);
				if(EP.m_bUseLimit)
					EP.m_dSubData2 = m_edtTensLimit.GetEditValue();
			}
			else if (EP.m_nSubType == 2) // hook
			{
				CString csText;
				m_wndTTDistEdit.GetWindowText(csText);
				if(!GetFloatNumber(csText, EP.m_dSubData)) return FALSE;
			}
			else if (EP.m_nSubType == 3)  // cable
			{
				EP.m_nCableOpt = GetTensCableKind();
				if (EP.m_nCableOpt == 3) EP.m_dSubData = m_wndTTLengthEdit.GetEditValue();
				else if (EP.m_nCableOpt == 1||EP.m_nCableOpt == 2) EP.m_dSubData = m_wndTTForceEdit.GetEditValue();
				else ASSERT(0);
			}
		}
		break;
	case TRCOMP_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_CTRadioCtrl, nCheck);
			EP.m_nSubType = nCheck+1;
			if (EP.m_nSubType == 1)  // Truss
			{
				EP.m_dSubData = m_wndCTAlTensEdit.GetEditValue();
				int nCheck = ((CButton*)GetDlgItem(IDC_TM_COMP_LMT_CHK))->GetCheck();
				EP.m_bUseLimit = (nCheck==1);
				if(EP.m_bUseLimit)
					EP.m_dSubData2 = m_edtCompLimit.GetEditValue();
			}
			else if (EP.m_nSubType == 2) // gap
			{
				CString csText;
				m_wndCTDistEdit.GetWindowText(csText);
				if(!GetFloatNumber(csText, EP.m_dSubData)) return FALSE;
			}
		}
		break;    
	case PLATE_EL:
		{
			if(this->IsDlgButtonChecked(IDC_TM_ELEM_WITH_DRILL_CHK))
			{
				if(this->IsDlgButtonChecked(IDC_TM_ELEM_TYPE_THIN))
			    EP.m_nSubType = 3;
		    else 
			    EP.m_nSubType = 2;
			}
		  else
			{
		    if(this->IsDlgButtonChecked(IDC_TM_ELEM_TYPE_THIN))
			    EP.m_nSubType = 1;
		    else 
			    EP.m_nSubType = 0;
			}
		}
		break;
	case PLSTRS_EL:
		{
		  if(this->IsDlgButtonChecked(IDC_TM_ELEM_WITH_DRILL_CHK))
				EP.m_nSubType = 1;		  
		  else
		    EP.m_nSubType = 0;
		}
		break;
	case WALL_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_WallRadioCtrl, nCheck);
			EP.m_nSubType = nCheck+1;
			
			if (!m_wndWallID.GetEditValue(EP.m_nWallID)) 
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Wall_ID_));
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CElemParamChangeDlg::GetParamReverseLocal(_ElemParam &EP)
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_RevRadioCtrl, nCheck);
	EP.m_nElType = nCheck+1;
	if (m_wndRevCheck.GetCheck() == 1) EP.m_bReverse = TRUE;
	else EP.m_bReverse = FALSE;

	return TRUE;
}

BOOL CElemParamChangeDlg::GetParamAlignLocal(_ElemParam &EP)
{
	CString strTemp;
	m_edtAlignLocal.GetWindowText(strTemp);
	EP.m_nKeyStandardElem = _ttoi(strTemp);

	// SortOrder 를 정한다.
	CString strTemp1,strTemp2;
	int OrderTemp;
	m_cboOrder1.GetLBText(m_cboOrder1.GetCurSel(), strTemp1);
	m_cboOrder2.GetLBText(m_cboOrder2.GetCurSel(), strTemp2);
	if((strTemp1 == D_EP_LX) && (strTemp2 == D_EP_LY)) OrderTemp = 1;
	else if((strTemp1 == D_EP_LX) && (strTemp2 == D_EP_LZ)) OrderTemp = 2;
	else if((strTemp1 == D_EP_LY) && (strTemp2 == D_EP_LX)) OrderTemp = 3;
	else if((strTemp1 == D_EP_LY) && (strTemp2 == D_EP_LZ)) OrderTemp = 4;
	else if((strTemp1 == D_EP_LZ) && (strTemp2 == D_EP_LX)) OrderTemp = 5;
	else OrderTemp = 6;

	EP.m_nSortOrder = OrderTemp;
	return TRUE;
}

BOOL CElemParamChangeDlg::GetParameter_MEC(_ElemParam& EP)
{
	if (EP.m_nElemType == TM_FRAME_EL ||
		EP.m_nElemType == PLATE_EL || EP.m_nElemType == PLSTRS_EL)
	{
		if (m_nMode2 == 0)
		{
			return GetKNodeDatas(EP.m_nElemType, EP.m_nBAngle, EP.m_nRefAngle, EP.m_dblBetaAngle, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz,
			                     EP.m_nAxis, EP.m_nCoordDir, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz, EP.m_NuckK);
		}
		else
		{
			return GetFloatNumber(m_csBetaInc, EP.m_dblBetaInc);
		}
	}
	else
	{
		return GetKNodeDatas(EP.m_nElemType, EP.m_nBAngle, EP.m_nRefAngle, EP.m_dblBetaAngle, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz,
		                     EP.m_nAxis, EP.m_nCoordDir, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz, EP.m_NuckK);
	}
}

BOOL CElemParamChangeDlg::GetParameter(_ElemParam& EP)
{
	if (!UpdateData(TRUE)) return FALSE;
	EP.m_nType = m_nType;
	EP.m_nMode = m_nMode;
	EP.m_nMode2 = m_nMode2;
	EP.m_nElemType = m_cmbElemType.GetItemData(m_cmbElemType.GetCurSel());;

	if (EP.m_nType == 4) // beta
	{
#ifndef _MEC
		if(m_nMode2 == 0)
		{
			return GetKNodeDatas(EP.m_nElemType, EP.m_nBAngle, EP.m_nRefAngle, EP.m_dblBetaAngle, EP.m_dBetaWx,EP.m_dBetaWy,EP.m_dBetaWz,
				EP.m_nAxis, EP.m_nCoordDir, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz, EP.m_NuckK);
		}
		else
		{
			return GetFloatNumber(m_csBetaInc, EP.m_dblBetaInc);
		}
#else
		GetParameter_MEC(EP);
#endif
	}
	// Element Type
	if (EP.m_nType == 5) return GetParamElemType(EP);
	// Reverse Local
	if (EP.m_nType == 6) return GetParamReverseLocal(EP);
	// Align Element Local
	if (EP.m_nType == 7) return GetParamAlignLocal(EP);

	if (EP.m_nType == 8) return GetParamEccent(EP);    //Add by ZHOUMIN
	
	if (m_nMode == 0) // Assign
	{
		if (m_nType == 0) // matl
		{
			if (!m_wndMatlNo.GetKey(EP.m_nKey)) return FALSE;
		}
		else if (m_nType == 1) // sect
		{
			if (!m_wndSectNo.GetKey(EP.m_nKey)) return FALSE;
		}
		else if (m_nType == 2) // thik
		{
			if (!m_wndThikNo.GetKey(EP.m_nKey)) return FALSE;
		}
		else if (m_nType == 3) // Wall ID
		{
			CString strTemp = _T("");
			m_editWall.GetWindowText(strTemp);
			EP.m_nKey = _ttoi(strTemp);
		}
		else ASSERT(0);
	}
	else if (m_nMode == 1) // change
	{
		if(m_nType == 3) // Wall ID
		{
			CString strTemp;
			m_edtWallInc.GetWindowText(strTemp);
			if(!GetINumber(strTemp, EP.m_nIncNum)) return FALSE;
		}
		else
		{
			if(!GetINumber(m_csNoInc, EP.m_nIncNum)) return FALSE;
		}
	}
	else ASSERT(0);

	return TRUE;
}
BOOL CElemParamChangeDlg::ChangeParam_BetaAngle_MEC(CArray<UINT, UINT>& aKeyList, _ElemParam& EP)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess = FALSE;

	if (EP.m_nElemType == TM_FRAME_EL ||
		EP.m_nElemType == PLATE_EL || EP.m_nElemType == PLSTRS_EL)
	{
		if (EP.m_nMode2 == 0)  // Assign
		{
			if (EP.m_nBAngle == 0)
				bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, FALSE, EP.m_dblBetaAngle);
			else if (EP.m_nBAngle == 1)
				bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz, TRUE);
			else if (EP.m_nBAngle == 2)
				bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz, FALSE);
			else
				bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nAxis, EP.m_nCoordDir, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz);
		}
		else if (EP.m_nMode2 == 1) // Change
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, TRUE, EP.m_dblBetaInc);
		else ASSERT(0);
	}
	else if (EP.m_nElemType == PLSTRN_EL || EP.m_nElemType == AXISYM_EL)
	{
		if (EP.m_nRefAngle == 0)
		{
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, 0, 0, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz);
		}
		else
		{
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, FALSE, EP.m_dblBetaAngle);
		}

	}
	else if (EP.m_nElemType == SOLID_EL)
	{
		if (EP.m_nRefAngle == 0)
		{
			CString strGCS;
			strGCS = _T("GCS");

			T_NUCS_K NucsK;
			NucsK = pDoc->m_pAttrCtrl->GetNucsKey(strGCS);

			if (NucsK == 0)
			{
				T_NUCS_D NucsD; NucsD.Initialize();
				NucsD.UcsName = strGCS;
				NucsD.dOrg[0] = 0.0;
				NucsD.dOrg[1] = 0.0;
				NucsD.dOrg[2] = 0.0;
				NucsD.dVx[0] = 1.0;
				NucsD.dVx[1] = 0.0;
				NucsD.dVx[2] = 0.0;
				NucsD.dVy[0] = 0.0;
				NucsD.dVy[1] = 1.0;
				NucsD.dVy[2] = 0.0;

				if (!pDoc->m_pDataCtrl->AddNucs(NucsD)) return FALSE;

				NucsK = pDoc->m_pAttrCtrl->GetNucsKey(strGCS);
			}

			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_RefAxis(aKeyList, NucsK);
		}
		else
		{
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_RefAxis(aKeyList, EP.m_NuckK);
		}
	}
	else
	{
		ASSERT(0);
	}

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}
BOOL CElemParamChangeDlg::ChangeParam_BetaAngle(CArray<UINT,UINT> &aKeyList, _ElemParam &EP)
{
#ifdef _MEC
	return ChangeParam_BetaAngle_MEC(aKeyList, EP);
#endif
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess = FALSE;
	if (EP.m_nMode2 == 0)  // Assign
	{
		if(EP.m_nBAngle == 0)
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, FALSE, EP.m_dblBetaAngle);
		else if(EP.m_nBAngle == 1)
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz, TRUE);
		else if(EP.m_nBAngle == 2)
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, EP.m_dBetaWx, EP.m_dBetaWy, EP.m_dBetaWz, FALSE);    
		else
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, EP.m_nAxis, EP.m_nCoordDir, EP.m_dPointx, EP.m_dPointy, EP.m_dPointz);
	}
	else if (EP.m_nMode2 == 1) // Change
		bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_BetaAngle(aKeyList, EP.m_nElemType, TRUE, EP.m_dblBetaInc);
	else ASSERT(0);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

BOOL CElemParamChangeDlg::ChangeParam_ElemType(CArray<UINT,UINT> &aKeyList, _ElemParam &EP)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess;

	bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_ElementType(aKeyList, 
																EP.m_nFromType, EP.m_nToType, 
																EP.m_nSubType, EP.m_dSubData, EP.m_nWallID, EP.m_nCableOpt, 
																EP.m_dSubData2, EP.m_bUseLimit);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

BOOL CElemParamChangeDlg::ChangeParam_ElemRev(CArray<UINT,UINT> &aKeyList, _ElemParam &EP)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess;

	bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_ElementLocal(aKeyList, EP.m_nElType, EP.m_bReverse);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

BOOL CElemParamChangeDlg::ChangeParam_ElemAlignLocal(CArray<UINT,UINT> &aKeyList, _ElemParam &EP)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess;

	bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_AlignLocal(aKeyList, EP.m_nKeyStandardElem, EP.m_nSortOrder);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

BOOL CElemParamChangeDlg::ChangeParam(_ElemParam& EP)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	ASSERT(EP.m_nType >= 0 && EP.m_nType <= 8);            //Add by ZHOUMIN

	CArray<T_ELEM_K, T_ELEM_K> aKeyList;
	pIGM->GetSelectedElemKeyList(aKeyList);
	if (aKeyList.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_selected_element___));
		return FALSE;
	}
	if (EP.m_nType == 4) 
	{
		T_ELEM_D ElemD;
		CArray<T_ELEM_K, T_ELEM_K> aSelKeyList;
		for(int i=0; i<aKeyList.GetSize(); i++)
		{
			if(!pDoc->m_pAttrCtrl->GetElem(aKeyList[i], ElemD)) continue;

			if(EP.m_nElemType == 0)
			{
				if(!pDoc->m_pAttrCtrl->IsNeedSect(ElemD.eltyp)) continue;
				aSelKeyList.Add(aKeyList[i]);
			}
			else
			{
#ifndef _MEC
				if(!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp) && !pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) continue;
#else
				if (ElemD.eltyp != EP.m_nElemType) continue;
#endif

				aSelKeyList.Add(aKeyList[i]);
			}
		}

		if (aSelKeyList.GetSize() == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_No_selected_element___));
			return FALSE;
		}

		return ChangeParam_BetaAngle(aSelKeyList, EP);
	}
		
	if (EP.m_nType == 5) return ChangeParam_ElemType(aKeyList, EP);
	if (EP.m_nType == 6) return ChangeParam_ElemRev(aKeyList, EP);
	if (EP.m_nType == 7) return ChangeParam_ElemAlignLocal(aKeyList, EP);
	if (EP.m_nType == 8) return ChangeParam_Eccent(aKeyList, EP);             // Add by ZHOUMIN 

	BOOL bSuccess = FALSE;
	if (EP.m_nMode == 0)  // Assign
	{
		if(m_nDefOrCopy == 0 ) //Define 
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_Assign(aKeyList, EP.m_nType+1, EP.m_nKey);
		else // Copy                                   aKeyList, EP.m_nType+1, EP.m_nKey nCopyType : 0=Property 1=Element
			bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_Assign_Copy(aKeyList,EP.m_nType+1,m_nCopyType);

		if(bSuccess && EP.m_nType == 3)
		{
			int nMode = GetWallMode();
			if(nMode == 1)
			{
				CString strTemp = _T("");
				m_editWall.GetWindowText(strTemp);
				strTemp.Format(_T("%d"), _ttoi(strTemp)+1);
				m_editWall.SetWindowText(strTemp);
			}
			else if(nMode == 2)
				m_editWall.UpdateEditBoxText();
		}
	}
	else if (EP.m_nMode == 1) // Change
		bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_Change(aKeyList, EP.m_nType+1, EP.m_nIncNum);
	else ASSERT(0);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);

	return TRUE;
}
void CElemParamChangeDlg::ChangeText()
{
	int nElemType = m_cmbElemType.GetItemData(m_cmbElemType.GetCurSel());
	if (nElemType == PLSTRN_EL/* || nElemType == AXISYM_EL*/)
	{
		GetDlgItem(IDC_TM_REF_ANGLE_RDO)->SetWindowText(_LS(IDS_TM_REF_ANGLE));
	}
	else
	{
		GetDlgItem(IDC_TM_REF_ANGLE_RDO)->SetWindowText(_LS(IDS_TM_REF_AXIS));
	}
}

void CElemParamChangeDlg::Execute()
{
	_ElemParam EP;
	if (GetParameter(EP)) 
	{
		ChangeParam(EP);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

void CElemParamChangeDlg::OnChangeFromType() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndElTypeFrom.GetCurSel();
	if (nCurSel == LB_ERR) return;
	int nID = m_wndElTypeFrom.GetItemData(nCurSel);
	SetToComboBox(nID);
	m_wndElTypeTo.SetCurSel(0);
	ChangeStateOfElemTypeSubCtrls(m_nType);
}

void CElemParamChangeDlg::OnChangeToType() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_wndElTypeTo.GetCurSel();
	if (nCurSel == LB_ERR) return;
	ChangeStateOfElemTypeSubCtrls(m_nType);
}

void CElemParamChangeDlg::OnChangeTT() 
{
	// TODO: Add your control notification handler code here
	ChangeStateOfElemTypeSubCtrls(m_nType);
}

void CElemParamChangeDlg::OnChangeCT() 
{
	// TODO: Add your control notification handler code here
	ChangeStateOfElemTypeSubCtrls(m_nType);	
}

void CElemParamChangeDlg::OnTmRevFrame() 
{
	m_wndRevCheck.EnableWindow(TRUE);	
}

void CElemParamChangeDlg::OnTmRevNoFrame() 
{
	m_wndRevCheck.SetCheck(0);
	m_wndRevCheck.EnableWindow(TRUE);	
}

void CElemParamChangeDlg::SetOrder()
{
	m_cboOrder1.AddString(D_EP_LX);
	m_cboOrder1.AddString(D_EP_LY);
	m_cboOrder1.AddString(D_EP_LZ);
	m_cboOrder1.SetCurSel(2);

	m_cboOrder2.AddString(D_EP_LX);
	m_cboOrder2.AddString(D_EP_LY);
	m_cboOrder2.SetCurSel(1);

	m_cboOrder3.AddString(D_EP_LX);
	m_cboOrder3.SetCurSel(0);
}

void CElemParamChangeDlg::OnSelchangeTmOrder1st() 
{
	int flag;

	m_cboOrder2.ResetContent();
	for (int i=0; i<3 ; i++)
	{
	  if (m_cboOrder1.GetCurSel() != i)
	  {
		if(i == 0)m_cboOrder2.AddString(D_EP_LX);
		if(i == 1)m_cboOrder2.AddString(D_EP_LY);
		if(i == 2)m_cboOrder2.AddString(D_EP_LZ);
	  }
	  else
		  flag = i;
	}
	m_cboOrder2.SetCurSel(0);
	
	OnSelchangeTmOrder2nd();	
}

void CElemParamChangeDlg::OnSelchangeTmOrder2nd() 
{
	int i = m_cboOrder2.GetCurSel();
	CString strTemp;

	m_cboOrder3.ResetContent();
	
	if(i)
	{
		m_cboOrder2.GetLBText(0, strTemp);
		m_cboOrder3.AddString(strTemp);
	}
	else
	{
		m_cboOrder2.GetLBText(1, strTemp);
		m_cboOrder3.AddString(strTemp);
	}
	m_cboOrder3.SetCurSel(0);	
}

void CElemParamChangeDlg::OnTmBetaTypeRadio_BetaAngle()
{
#ifndef _MEC
		CArray<UINT, UINT> arUnit;
		arUnit.Add(IDC_TM_BETA_RADIO_BETAANGLE);
		arUnit.Add(IDC_TM_BETA_RADIO_REFPOINT);
		arUnit.Add(IDC_TM_BETA_RADIO_VECTOR);
		arUnit.Add(IDC_TM_BETA_RADIO_COORD);
		CDlgUtil::CtrlRadioSetCheck(this, arUnit, 0);
#endif
		OnTmBetaTypeRadio();
}
void CElemParamChangeDlg::OnTmBetaTypeRadio_RefPoint()
{
#ifndef _MEC
		CArray<UINT, UINT> arUnit;
		arUnit.Add(IDC_TM_BETA_RADIO_BETAANGLE);
		arUnit.Add(IDC_TM_BETA_RADIO_REFPOINT);
		arUnit.Add(IDC_TM_BETA_RADIO_VECTOR);
		arUnit.Add(IDC_TM_BETA_RADIO_COORD);
		CDlgUtil::CtrlRadioSetCheck(this, arUnit, 1);
#endif
		OnTmBetaTypeRadio();
}
void CElemParamChangeDlg::OnTmBetaTypeRadio_Vector()
{
#ifndef _MEC
		CArray<UINT, UINT> arUnit;
		arUnit.Add(IDC_TM_BETA_RADIO_BETAANGLE);
		arUnit.Add(IDC_TM_BETA_RADIO_REFPOINT);
		arUnit.Add(IDC_TM_BETA_RADIO_VECTOR);
		arUnit.Add(IDC_TM_BETA_RADIO_COORD);
		CDlgUtil::CtrlRadioSetCheck(this, arUnit, 2);
#endif
		OnTmBetaTypeRadio();
}
void CElemParamChangeDlg::OnTmBetaTypeRadio_Coord()
{
#ifndef _MEC
		CArray<UINT, UINT> arUnit;
		arUnit.Add(IDC_TM_BETA_RADIO_BETAANGLE);
		arUnit.Add(IDC_TM_BETA_RADIO_REFPOINT);
		arUnit.Add(IDC_TM_BETA_RADIO_VECTOR);
		arUnit.Add(IDC_TM_BETA_RADIO_COORD);
		CDlgUtil::CtrlRadioSetCheck(this, arUnit, 3);
#endif
		OnTmBetaTypeRadio();
}

void CElemParamChangeDlg::OnTmBetaTypeRadio() 
{
	UpdateData(TRUE);	
	
	if(m_nBetaAngleAssignType == 0)
	{
		GetDlgItem(IDC_TM_BETA_TITLE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_ANGLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_EDT_REFPOINT)->ShowWindow(SW_HIDE);    
		GetDlgItem(IDC_TM_BETA_UNIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_REFPNT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_EDT_VECTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_VECTOR_UNIT)->ShowWindow(SW_HIDE);		
	}
	else if(m_nBetaAngleAssignType == 1)
	{
		GetDlgItem(IDC_TM_BETA_TITLE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_EDT_REFPOINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_UNIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_REFPNT_UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_EDT_VECTOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_VECTOR_UNIT)->ShowWindow(SW_HIDE);		
	}
	else if(m_nBetaAngleAssignType == 2)
	{
		GetDlgItem(IDC_TM_BETA_TITLE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_EDT_REFPOINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_UNIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_REFPNT_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_BETA_EDT_VECTOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_BETA_VECTOR_UNIT)->ShowWindow(SW_SHOW);	
	}
	else 
	{
		// Nothing
	}

	if (m_nMode2 == 1) return;

	GetDlgItem(IDC_TM_BETA_TITLE1)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_ANGLE)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_EDT_REFPOINT)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_UNIT1)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_REFPNT_UNIT)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_EDT_VECTOR)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_VECTOR_UNIT)->EnableWindow(m_nBetaAngleAssignType != 3);
	GetDlgItem(IDC_TM_BETA_AXIS_GRP)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_X_AXIS_RDO)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_Y_AXIS_RDO)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_DIR_GRP)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_DIR_STC)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_DIR_CMB)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_ORG_POINT_STC)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_ORG_POINT_EDT)->EnableWindow(m_nBetaAngleAssignType == 3);
	GetDlgItem(IDC_TM_BETA_ORG_POINT_UNT)->EnableWindow(m_nBetaAngleAssignType == 3);
	OnTmRefDirCmb();
}

void CElemParamChangeDlg::OnTmLocalAxisRdo() 
{
	UpdateData(TRUE);
	// Nothing
}

BOOL CElemParamChangeDlg::GetKNodeDatas_MEC(int& nElemType, int& nBAngle, int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz,
	int& nAxis, int& nCoordDir, double& dPointx, double& dPointy, double& dPointz, T_NUCS_K& NucsK)
{
	CString BAStr, RefPStr, VecStr, RefAStr;
	double  RPUx, RPUy, RPUz;

	m_wndRefPoint.GetWindowText(RefPStr);
	BAStr = m_csBetaAngle;
	m_wndDirVectorEdit.GetWindowText(VecStr);
	m_edtRefAngle.GetWindowText(RefAStr);

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

	nElemType = m_cmbElemType.GetItemData(m_cmbElemType.GetCurSel());

	if (nElemType == TM_FRAME_EL ||
		nElemType == PLATE_EL || nElemType == PLSTRS_EL)
	{
		nBAngle = m_nBetaAngleAssignType;
		if (nBAngle == 0) // BetaAngle Need
		{
			GetFloatNumber(BAStr, BAngle);
			RPWx = RPWy = RPWz = 0.0;
		}
		else if (nBAngle == 1)// Reference Point Need
		{
			BAngle = 0.0;
			GetPosition(RefPStr, RPWx, RPWy, RPWz);
			pIGM->GetWcsPos(RPWx, RPWy, RPWz, RPWx, RPWy, RPWz); // UCS Data를 WCS Data로 변환 한다.
		}
		else if (nBAngle == 2)
		{
			m_wndDirVectorEdit.GetWindowText(VecStr);
			if (TRUE == m_wndDirVectorEdit.IsKeyPressed())
			{
				if (GetDirectionVector(VecStr, RPUx, RPUy, RPUz))
				{
					pIGM->GetWcsVector(RPUx, RPUy, RPUz, RPWx, RPWy, RPWz);
				}
				else return FALSE;
			}
			else
			{
				m_wndDirVectorEdit.GetCurMousePosDist(RPWx, RPWy, RPWz, RPUx, RPUy, RPUz);
			}
			BAngle = 0.0;
			pIGM->GetWcsPos(RPWx, RPWy, RPWz, RPWx, RPWy, RPWz); // UCS Data를 WCS Data로 변환 한다.     
		}
		else
		{
			nAxis = m_nAxis;
			nCoordDir = m_cmbCoordDir.GetCurSel();

			CString strText;
			m_edtPoint.GetWindowText(strText);
			GetPosition(strText, dPointx, dPointy, dPointz);
		}
	}
	else if (nElemType == PLSTRN_EL || nElemType == AXISYM_EL)
	{
		nRefAngle = m_nRefAngle;

		if (nRefAngle == 1)
		{
			GetFloatNumber(RefAStr, BAngle);
		}
	}
	else if (nElemType == SOLID_EL)
	{
		NucsK = 0;

		nRefAngle = m_nRefAngle;

		if (nRefAngle == 1)
		{
			if (m_cmbRefAxis.GetCount() > 0)
			{
				NucsK = m_cmbRefAxis.GetItemData(m_cmbRefAxis.GetCurSel());
			}
		}
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CElemParamChangeDlg::GetKNodeDatas(int& nElemType, int& nBAngle, int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz,
	int& nAxis, int& nCoordDir, double& dPointx, double& dPointy, double& dPointz, T_NUCS_K& NucsK)
{
#ifdef _MEC
	return GetKNodeDatas_MEC(nElemType, nBAngle, nRefAngle, BAngle, RPWx, RPWy, RPWz, nAxis, nCoordDir, dPointx, dPointy, dPointz, NucsK);
#endif

	CString BAStr, RefPStr, VecStr;
	double  RPUx, RPUy, RPUz;

	m_wndRefPoint.GetWindowText(RefPStr);
	BAStr = m_csBetaAngle;
	m_wndDirVectorEdit.GetWindowText(VecStr);

	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	 
	nElemType = m_cmbElemType.GetCurSel();
	nBAngle = m_nBetaAngleAssignType;
	if(nBAngle == 0) // BetaAngle Need
	{
		GetFloatNumber(BAStr,BAngle);
		RPWx = RPWy = RPWz = 0.0;    
	}
	else if(nBAngle == 1)// Reference Point Need
	{
		BAngle = 0.0;      
		GetPosition(RefPStr,RPWx,RPWy,RPWz);
		pIGM->GetWcsPos(RPWx,RPWy,RPWz,RPWx,RPWy,RPWz); // UCS Data를 WCS Data로 변환 한다.
	}
	else if(nBAngle == 2)
	{
		m_wndDirVectorEdit.GetWindowText(VecStr);
		if(TRUE == m_wndDirVectorEdit.IsKeyPressed())
		{
			if(GetDirectionVector(VecStr,RPUx,RPUy,RPUz))
			{
				 pIGM->GetWcsVector(RPUx,RPUy,RPUz,RPWx,RPWy,RPWz);
			}
			else return FALSE;
		}
		else
		{        
			m_wndDirVectorEdit.GetCurMousePosDist(RPWx,RPWy,RPWz,RPUx,RPUy,RPUz);
		}    
		BAngle = 0.0;
		pIGM->GetWcsPos(RPWx,RPWy,RPWz,RPWx,RPWy,RPWz); // UCS Data를 WCS Data로 변환 한다.     
	}
	else
	{
		nAxis = m_nAxis;
		nCoordDir = m_cmbCoordDir.GetCurSel();

		CString strText;
		m_edtPoint.GetWindowText(strText);
		GetPosition(strText, dPointx, dPointy, dPointz);
	}
 
	return TRUE;
}

void CElemParamChangeDlg::OnTmWIdAutoincCheck() 
{
	// TODO: Add your control notification handler code here	
}

/*

BOOL ChangeElemParam_Assign_Copy(CArray<T_ELEM_K, T_ELEM_K>& rKey, int nParatype, int nCopyType);
*/

// Wall ID Assign ComboBox가 바뀔때 불리는 메세지
void CElemParamChangeDlg::OnSelchangeTmWallAssignCombo() 
{
	int nMode = GetWallMode();
	if(nMode >= 0) m_editWall.SetMode(nMode);
	if(nMode == 2) m_editWall.UpdateEditBoxText();
}

//////////////////////////// 요기서 부터 새로 추가

void CElemParamChangeDlg::ChangeTensCableCombo()
{
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_TM_TT_LENGTH_EDIT);
	if (pWnd == NULL) { ASSERT(0); return; }

	int nKind = GetTensCableKind();
	ASSERT(nKind == 3 || nKind == 1|| nKind == 2);

	pWnd->ShowWindow(nKind == 3);

	pWnd = GetDlgItem(IDC_TM_TT_LENGTH_UNIT);
	if (pWnd == NULL) { ASSERT(0); return; }
	pWnd->ShowWindow(nKind == 3);

	CDlgUtil::CtrlShowHide(this, m_TTCableCtrl2, nKind == 1||nKind==2);
}

void CElemParamChangeDlg::InitTensCableCombo() 
{
	m_wndTTLnLCobx.ResetContent();
	m_wndTTLnLCobx.AddString(TM_TENS_CABLE_LU);        // Lu
	m_wndTTLnLCobx.AddString(TM_TENS_CABLE_PRETENS);    // Pretension
	m_wndTTLnLCobx.AddString(TM_TENS_CABLE_HORIZ); // Horizontal
	m_wndTTLnLCobx.SetCurSel(0);
}

int CElemParamChangeDlg::GetTensCableKind() 
{
	CString csKind;
	int nIndex = m_wndTTLnLCobx.GetCurSel();
	if (nIndex == CB_ERR) { ASSERT(0); return -1; }
	m_wndTTLnLCobx.GetLBText(nIndex, csKind);
	
	if (csKind == TM_TENS_CABLE_LU) return 3;
	if (csKind == TM_TENS_CABLE_PRETENS) return 1;
	if (csKind == TM_TENS_CABLE_HORIZ) return 2;
	return -1;
}

void CElemParamChangeDlg::OnSelchangeTmTtLnlCombo() 
{
	// TODO: Add your control notification handler code here
	ChangeTensCableCombo();
}

void CElemParamChangeDlg::OnTmClickTensLimit() 
{
	int nCheck=0;
	nCheck = ((CButton*)GetDlgItem(IDC_TM_TENS_LMT_CHK))->GetCheck();
	if (nCheck==0) m_edtTensLimit.SetEditUnit(0);
	GetDlgItem(IDC_TM_TT_LIMIT_EDT)->EnableWindow(nCheck==1);
}

void CElemParamChangeDlg::OnTmClickCompLimit() 
{
	int nCheck=0;
	nCheck = ((CButton*)GetDlgItem(IDC_TM_COMP_LMT_CHK))->GetCheck();
	if (nCheck==0) m_edtCompLimit.SetEditUnit(0);
	GetDlgItem(IDC_TM_CC_LIMIT_EDT)->EnableWindow(nCheck==1);
}

void CElemParamChangeDlg::OnTmClickRefVectorBtn()
{
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	CElemParamChangeRefVecDlg* pDlg = (CElemParamChangeRefVecDlg*)(pDoc->m_pDlgCtrl->GetDlg(CElemParamChangeRefVecDlg::IDD));
	if(pDlg==NULL)
	{
		pDlg = new CElemParamChangeRefVecDlg(this);
		pDlg->m_pDoc = pDoc;
		pDlg->SetInitPos(D_INIT_POS_LT,5,180);
		pDlg->Create(CElemParamChangeRefVecDlg::IDD, this);
	}
	pDlg->ShowWindow(SW_SHOW);
}

void CElemParamChangeDlg::SetBetaVectorEdit(CString str)
{
	m_wndDirVectorEdit.SetWindowText(str);
}

//////////////////////////////////////////////////////////////////////////////////
// Add By ZHOUMIN 2015.10.16  
void CElemParamChangeDlg::OnChangeEccent()
{
	m_nType = 8;
	AlignControl();
	
	CtrlShowHide(m_ModeRadioCtrl, FALSE);
	CtrlShowHide(m_ModeCtrl, FALSE);
	CtrlShowHide(m_MatlCtrl, FALSE);
	CtrlShowHide(m_SectCtrl, FALSE);
	CtrlShowHide(m_ThikCtrl, FALSE);		
	CtrlShowHide(m_BetaCtrl, FALSE); GetDlgItem(IDC_TM_LAST_CTRL)->ShowWindow(TRUE);
	CtrlShowHide(m_ElemTypeCtrl, FALSE);
	CtrlShowHide(m_RevCtrl,  FALSE);
	CtrlShowHide(m_arWallCtrl,  FALSE);	     // Wall ID
	CtrlShowHide(m_arAlignElemCtrl,  FALSE);	 // Align Element Local	
	CtrlShowHide(m_arEccentCtrl,  TRUE);
}
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Add By ZHOUMIN 2015.10.16  뎐놔錦맣쌔충튤懃뚤뺐움
void CElemParamChangeDlg::OnTmEccentBtn()
{
	ElemParamSectOffsetDlg dlg(&m_SectData, FALSE, this);
	if (dlg.DoModal() == IDOK)
	{
		m_bEccentChangeFlag = TRUE;
	} 
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Add By ZHOUMIN 2015.10.16  角뤠錦맣쌔충튤懃
BOOL CElemParamChangeDlg::GetParamEccent( _ElemParam &EP )
{
	if (m_bEccentChangeFlag)
	{
		EP.m_pSectData = &m_SectData;
	}
	else
	{
		EP.m_pSectData = NULL;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Add By ZHOUMIN 2015.10.19  錦맣쌔충튤懃
BOOL CElemParamChangeDlg::ChangeParam_Eccent( CArray<UINT,UINT> &aKeyList, _ElemParam &EP )
{
	if (EP.m_pSectData)
	{
		CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		BOOL bSuccess;

		bSuccess = pDoc->m_pDataCtrl->ChangeElemParam_Eccent(aKeyList, EP.m_pSectData);

		if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
		return TRUE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////