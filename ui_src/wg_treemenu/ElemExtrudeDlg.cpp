// Old Version : ElemExtrudeDlg2 사용
// ElemExtrudeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemExtrudeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemExtrudeDlg dialog


CElemExtrudeDlg::CElemExtrudeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemExtrudeDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CElemExtrudeDlg)
	m_bRemoveSrc = TRUE;
	m_nGenerationType = 0;
	m_nTranOption = 0;
	m_nTranChooseAxis = 0;
	m_nRotAxis = 0;
	m_nDirection = 0;
	m_wndThicknessDir = 0;
	m_chkReverseIJ = FALSE;
	m_bMoveSrc = FALSE;
	//}}AFX_DATA_INIT

	m_aTranslateCtrl.Add(IDC_TM_TRAN_FRAME);
	m_aTranslateCtrl.Add(IDC_TM_EQUAL_DIST_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_EQU_XYZ_STATIC);
	m_aTranslateCtrl.Add(IDC_TM_XYZ_EQUDIST_EDIT);
	m_aTranslateCtrl.Add(IDC_TM_DIST_UNIT1);
	m_aTranslateCtrl.Add(IDC_TM_NTIMES_STATIC2);
	m_aTranslateCtrl.Add(IDC_TM_ITER_NUM2);
	//m_aTranslateCtrl.Add(IDC_TM_ITER_NUM_SPIN2);
	m_aTranslateCtrl.Add(IDC_TM_UNEQUAL_DIST_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_STATIC4);
	m_aTranslateCtrl.Add(IDC_TM_XAXIS_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_YAXIS_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_ZAXIS_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_ARBITRARY_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_STATIC5);
	m_aTranslateCtrl.Add(IDC_TM_UNEQ_DIST_EDIT);
	m_aTranslateCtrl.Add(IDC_TM_DIST_UNIT2);
	m_aTranslateCtrl.Add(IDC_TM_STATIC3);
	m_aTranslateCtrl.Add(IDC_TM_DIR_STATIC);
	m_aTranslateCtrl.Add(IDC_TM_DIR_VECTOR_EDIT);
	m_aTranslateCtrl.Add(IDC_TM_DIST_UNIT3);
// Thickness 살림.
	m_aTranslateCtrl.Add(IDC_TM_THICKNESS_RADIO);
	m_aTranslateCtrl.Add(IDC_TM_THICKNESS_TITLE );
	m_aTranslateCtrl.Add(IDC_TM_THICKNESS_EDIT);
	m_aTranslateCtrl.Add(IDC_TM_THICKNESS_UNIT);
	m_aTranslateCtrl.Add(IDC_TM_DIRECTION_TITLE);
	m_aTranslateCtrl.Add(IDC_TM_DIRECTION_PZ_CBO);
	m_aTranslateCtrl.Add(IDC_TM_DIRECTION_MZ_CBO);  // Thickness 살림.
//	m_aTranslateCtrl.Add(IDC_TM_DIRECTION_BOTH_CBO);  // Both Radio버튼은 구현의 어려움때문에 제거.

	m_aRotateCtrl.Add(IDC_TM_ROT_FRAME);
	m_aRotateCtrl.Add(IDC_TM_ROT_NTIMES_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_NTIMES_EDIT);
	//m_aRotateCtrl.Add(IDC_TM_ROT_NTIMES_SPIN);
	m_aRotateCtrl.Add(IDC_TM_ROT_ANGLE_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_ANGLE_EDIT);
	//m_aRotateCtrl.Add(IDC_TM_ROT_ANGLE_SPIN);
	m_aRotateCtrl.Add(IDC_TM_ROT_ANGLE_UNIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTR_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTR_EDIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTR_UNIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTA_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTA_EDIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_DISTA_UNIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_AXIS_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_AXIS_X);
	m_aRotateCtrl.Add(IDC_TM_ROT_AXIS_Y);
	m_aRotateCtrl.Add(IDC_TM_ROT_AXIS_Z);
	m_aRotateCtrl.Add(IDC_TM_ROT_AXIS_2POINT);
	m_aRotateCtrl.Add(IDC_TM_ROT_1ST_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_1ST_EDIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_1ST_UNIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_2ND_TITLE);
	m_aRotateCtrl.Add(IDC_TM_ROT_2ND_EDIT);
	m_aRotateCtrl.Add(IDC_TM_ROT_2ND_UNIT);

	m_aMatlCtrl.Add(IDC_TM_MATL_TITLE);
	m_aMatlCtrl.Add(IDC_TM_MATL_NO);
	m_aMatlCtrl.Add(IDC_TM_MATL_NAME);
	m_aMatlCtrl.Add(IDC_TM_MATL_BTN);

	m_aSectCtrl.Add(IDC_TM_SECT_TITLE);
	m_aSectCtrl.Add(IDC_TM_SECT_NO);
	m_aSectCtrl.Add(IDC_TM_SECT_NAME);
	m_aSectCtrl.Add(IDC_TM_SECT_BTN);

	m_aThikCtrl.Add(IDC_TM_THIK_TITLE);
	m_aThikCtrl.Add(IDC_TM_THIK_NO);
	m_aThikCtrl.Add(IDC_TM_THIK_NAME);
	m_aThikCtrl.Add(IDC_TM_THIK_BTN);

	// For Translate
	m_aEqualCtrl.Add(IDC_TM_EQU_XYZ_STATIC   );
	m_aEqualCtrl.Add(IDC_TM_XYZ_EQUDIST_EDIT );
	m_aEqualCtrl.Add(IDC_TM_DIST_UNIT1       );
	m_aEqualCtrl.Add(IDC_TM_NTIMES_STATIC2);
	m_aEqualCtrl.Add(IDC_TM_ITER_NUM2);
	//m_aEqualCtrl.Add(IDC_TM_ITER_NUM_SPIN2);
	
	m_aUnEqualCtrl.Add(IDC_TM_STATIC4         );  
	m_aUnEqualCtrl.Add(IDC_TM_XAXIS_RADIO     );
	m_aUnEqualCtrl.Add(IDC_TM_YAXIS_RADIO     );
	m_aUnEqualCtrl.Add(IDC_TM_ZAXIS_RADIO     );
	m_aUnEqualCtrl.Add(IDC_TM_ARBITRARY_RADIO );
	m_aUnEqualCtrl.Add(IDC_TM_STATIC5         );
	m_aUnEqualCtrl.Add(IDC_TM_UNEQ_DIST_EDIT  );
	m_aUnEqualCtrl.Add(IDC_TM_DIST_UNIT2      );
	m_aUnEqualCtrl.Add(IDC_TM_STATIC3         );
	m_aUnEqualCtrl.Add(IDC_TM_DIR_STATIC      );
	m_aUnEqualCtrl.Add(IDC_TM_DIR_VECTOR_EDIT );
	m_aUnEqualCtrl.Add(IDC_TM_DIST_UNIT3      );

	m_aVectorCtrl.Add(IDC_TM_DIR_STATIC);
	m_aVectorCtrl.Add(IDC_TM_DIR_VECTOR_EDIT);
	m_aVectorCtrl.Add(IDC_TM_DIST_UNIT3);

	m_aThicknessCtrl.Add(IDC_TM_THICKNESS_TITLE );
	m_aThicknessCtrl.Add(IDC_TM_THICKNESS_EDIT);
	m_aThicknessCtrl.Add(IDC_TM_THICKNESS_UNIT);
	m_aThicknessCtrl.Add(IDC_TM_DIRECTION_TITLE);
	m_aThicknessCtrl.Add(IDC_TM_DIRECTION_PZ_CBO);
	m_aThicknessCtrl.Add(IDC_TM_DIRECTION_MZ_CBO);
//	m_aThicknessCtrl.Add(IDC_TM_DIRECTION_BOTH_CBO);

	// For Rotate
	m_aRot2ndPtCtrl.Add(IDC_TM_ROT_2ND_TITLE);
	m_aRot2ndPtCtrl.Add(IDC_TM_ROT_2ND_EDIT);
	m_aRot2ndPtCtrl.Add(IDC_TM_ROT_2ND_UNIT);

	// Sub Controls of Element
	m_TTCtrl.Add(IDC_TM_TT_RADIO_TRUSS);
	m_TTCtrl.Add(IDC_TM_TT_RADIO_HOOK);
	m_TTCtrl.Add(IDC_TM_TT_RADIO_CABLE);
	m_TTCtrl.Add(IDC_TM_TT_DIST_TITLE);
	m_TTCtrl.Add(IDC_TM_TT_DIST_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_DIST_UNIT);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_TITLE);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTCtrl.Add(IDC_TM_TT_FORCE_UNIT);

	m_TTDistCtrl.Add(IDC_TM_TT_DIST_TITLE);
	m_TTDistCtrl.Add(IDC_TM_TT_DIST_EDIT);
	m_TTDistCtrl.Add(IDC_TM_TT_DIST_UNIT);
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_TITLE);
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_EDIT);
	m_TTForceCtrl.Add(IDC_TM_TT_FORCE_UNIT);

	m_CTCtrl.Add(IDC_TM_CT_RADIO_TRUSS);
	m_CTCtrl.Add(IDC_TM_CT_RADIO_GAP);
	m_CTCtrl.Add(IDC_TM_CT_DIST_TITLE);
	m_CTCtrl.Add(IDC_TM_CT_DIST_EDIT);
	m_CTCtrl.Add(IDC_TM_CT_DIST_UNIT);

	m_CTDistCtrl.Add(IDC_TM_CT_DIST_TITLE);
	m_CTDistCtrl.Add(IDC_TM_CT_DIST_EDIT);
	m_CTDistCtrl.Add(IDC_TM_CT_DIST_UNIT);

	m_WallCtrl.Add(IDC_TM_W_RADIO_MEMBR);
	m_WallCtrl.Add(IDC_TM_W_RADIO_PLATE);
	m_WallCtrl.Add(IDC_TM_W_ID_TITLE);
	m_WallCtrl.Add(IDC_TM_W_ID_EDIT);

	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_TRUSS);
	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_HOOK);
	m_TTRadioCtrl.Add(IDC_TM_TT_RADIO_CABLE);

	m_CTRadioCtrl.Add(IDC_TM_CT_RADIO_TRUSS);
	m_CTRadioCtrl.Add(IDC_TM_CT_RADIO_GAP);

	m_WallRadioCtrl.Add(IDC_TM_W_RADIO_MEMBR);
	m_WallRadioCtrl.Add(IDC_TM_W_RADIO_PLATE);

	//Added by water tree.
	//For Project
	m_aFrameCtrl.Add(IDC_TM_PRJTYPE_FRAME);
	m_aFrameCtrl.Add(IDC_TM_DEF_FRAME);
	m_aFrameCtrl.Add(IDC_TM_DIRECTION_FRAME);
	m_aFrameCtrl.Add(IDC_TM_PRJTYPE_CBO);
	m_aFrameCtrl.Add(IDC_TM_DIR_NORMAL);
	m_aFrameCtrl.Add(IDC_TM_DIR_2PT_AXIS);
	m_aFrameCtrl.Add(IDC_TM_DIR_PT1_EDIT);
	m_aFrameCtrl.Add(IDC_TM_DIR_PT1_UNIT);

	m_aLineCtrl.Add(IDC_TM_LINE_PT1_TITLE);
	m_aLineCtrl.Add(IDC_TM_LINE_PT1_EDIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT1_UNIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_TITLE);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_EDIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_UNIT);

	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_UNIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_UNIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_UNIT);

	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_UNIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_UNIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_UNIT);

	m_aConeCtrl.Add(IDC_TM_CONE_PT1_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_PT1_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT1_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_UNIT);

	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_TITLE);
	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_EDIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_UNIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_TITLE);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_EDIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_UNIT);

	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_UNIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_UNIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_UNIT);

	m_aElementCtrl.Add(IDC_TM_ELEM_TITLE);
	m_aElementCtrl.Add(IDC_TM_ELEM_EDIT);

	m_aDirPt2Ctrl.Add(IDC_TM_DIR_PT1_EDIT);
	m_aDirPt2Ctrl.Add(IDC_TM_DIR_PT1_UNIT);

	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_TITLE);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THICK);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THIN);
}

void CElemExtrudeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemExtrudeDlg)
	DDX_Control(pDX, IDC_TM_THICKNESS_EDIT, m_wndThicknessEdit);
	DDX_Control(pDX, IDC_TM_THICKNESS_UNIT, m_wndThicknessUnit);
	DDX_Control(pDX, IDC_TM_PRJTYPE_CBO, m_wndProjectType);
	DDX_Control(pDX, IDC_TM_CT_DIST_UNIT, m_wndCTDistUnit);
	DDX_Control(pDX, IDC_TM_TT_FORCE_UNIT, m_wndTTForceUnit);
	DDX_Control(pDX, IDC_TM_TT_DIST_UNIT, m_wndTTDistUnit);
	DDX_Control(pDX, IDC_TM_W_ID_EDIT, m_wndWallID);
	DDX_Control(pDX, IDC_TM_TT_FORCE_EDIT, m_wndTTForceEdit);
	DDX_Control(pDX, IDC_TM_CT_DIST_EDIT, m_wndCTDistEdit);
	DDX_Control(pDX, IDC_TM_TT_DIST_EDIT, m_wndTTDistEdit);
	DDX_Control(pDX, IDC_TM_ET_REMOVE_CHECK, m_wndRemoveCheck);
	DDX_Control(pDX, IDC_TM_ROT_NTIMES_EDIT, m_wndRotIterNum);
	DDX_Control(pDX, IDC_TM_ROT_ANGLE_UNIT, m_wndRotAngleUnit);
	DDX_Control(pDX, IDC_TM_ROT_ANGLE_EDIT, m_wndRotAngle);
	DDX_Control(pDX, IDC_TM_ROT_DISTR_UNIT, m_wndDistRUnit);
	DDX_Control(pDX, IDC_TM_ROT_DISTR_EDIT, m_wndDistR);
	DDX_Control(pDX, IDC_TM_ROT_DISTA_UNIT, m_wndDistAUnit);
	DDX_Control(pDX, IDC_TM_ROT_DISTA_EDIT, m_wndDistA);
	DDX_Control(pDX, IDC_TM_ROT_2ND_UNIT, m_wnd2ndUnit);
	DDX_Control(pDX, IDC_TM_ROT_1ST_UNIT, m_wnd1stUnit);
	DDX_Control(pDX, IDC_TM_ROT_2ND_EDIT, m_wnd2ndPoint);
	DDX_Control(pDX, IDC_TM_ROT_1ST_EDIT, m_wnd1stPoint);
	DDX_Control(pDX, IDC_TM_ITER_NUM2, m_wndIterNum);
	DDX_Control(pDX, IDC_TM_DIST_UNIT3, m_wndDistUnit3);
	DDX_Control(pDX, IDC_TM_DIST_UNIT2, m_wndDistUnit2);
	DDX_Control(pDX, IDC_TM_DIST_UNIT1, m_wndDistUnit1);
	DDX_Control(pDX, IDC_TM_XYZ_EQUDIST_EDIT, m_wndEQDist);
	DDX_Control(pDX, IDC_TM_UNEQ_DIST_EDIT, m_wndUEQDist);
	DDX_Control(pDX, IDC_TM_DIR_VECTOR_EDIT, m_wndDirVector);
	DDX_Control(pDX, IDC_TM_THIK_NAME, m_wndThikName);
	DDX_Control(pDX, IDC_TM_THIK_NO, m_wndThikNo);
	DDX_Control(pDX, IDC_TM_SECT_NAME, m_wndSectName);
	DDX_Control(pDX, IDC_TM_SECT_NO, m_wndSectNo);
	DDX_Control(pDX, IDC_TM_MATL_NAME, m_wndMatlName);
	DDX_Control(pDX, IDC_TM_MATL_NO, m_wndMatlNo);
	DDX_Control(pDX, IDC_TM_EA_ELEM_TYPE, m_wndElemType);
	DDX_Control(pDX, IDC_TM_ET_COMBO, m_wndExtrudeType);
	DDX_Check(pDX, IDC_TM_ET_REMOVE_CHECK, m_bRemoveSrc);
	DDX_Radio(pDX, IDC_TM_GT_TRANSLATE, m_nGenerationType);
	DDX_Radio(pDX, IDC_TM_EQUAL_DIST_RADIO, m_nTranOption);
	DDX_Radio(pDX, IDC_TM_XAXIS_RADIO, m_nTranChooseAxis);
	DDX_Radio(pDX, IDC_TM_ROT_AXIS_X, m_nRotAxis);
	DDX_Control(pDX, IDC_TM_DIR_PT1_UNIT, m_wndDirPt1Unit);
	DDX_Control(pDX, IDC_TM_DIR_PT1_EDIT, m_wndDirPt1);
	DDX_Control(pDX, IDC_TM_ELEM_EDIT, m_wndElemNo);
	DDX_Control(pDX, IDC_TM_ELIP_PT1_UNIT, m_wndElipPt1Unit);
	DDX_Control(pDX, IDC_TM_ELIP_PT1_EDIT, m_wndElipPt1);
	DDX_Control(pDX, IDC_TM_ELIP_RAD_UNIT, m_wndElipRadUnit);
	DDX_Control(pDX, IDC_TM_ELIP_RAD_EDIT, m_wndElipRad);
	DDX_Control(pDX, IDC_TM_ELIP_ORG_UNIT, m_wndElipOrgUnit);
	DDX_Control(pDX, IDC_TM_ELIP_ORG_EDIT, m_wndElipOrg);
	DDX_Control(pDX, IDC_TM_SPHR_RAD_UNIT, m_wndSphrRadUnit);
	DDX_Control(pDX, IDC_TM_SPHR_RAD_EDIT, m_wndSphrRad);
	DDX_Control(pDX, IDC_TM_SPHR_ORG_UNIT, m_wndSphrOrgUnit);
	DDX_Control(pDX, IDC_TM_SPHR_ORG_EDIT, m_wndSphrOrg);
	DDX_Control(pDX, IDC_TM_CONE_RAD2_UNIT, m_wndConeRad2Unit);
	DDX_Control(pDX, IDC_TM_CONE_RAD2_EDIT, m_wndConeRad2);
	DDX_Control(pDX, IDC_TM_CONE_RAD1_UNIT, m_wndConeRad1Unit);
	DDX_Control(pDX, IDC_TM_CONE_RAD1_EDIT, m_wndConeRad1);
	DDX_Control(pDX, IDC_TM_CONE_PT2_UNIT, m_wndConePt2Unit);
	DDX_Control(pDX, IDC_TM_CONE_PT2_EDIT, m_wndConePt2);
	DDX_Control(pDX, IDC_TM_CONE_PT1_UNIT, m_wndConePt1Unit);
	DDX_Control(pDX, IDC_TM_CONE_PT1_EDIT, m_wndConePt1);
	DDX_Control(pDX, IDC_TM_CYL_RAD_UNIT, m_wndCylRadUnit);
	DDX_Control(pDX, IDC_TM_CYL_RAD_EDIT, m_wndCylRad);
	DDX_Control(pDX, IDC_TM_CYL_PT2_UNIT, m_wndCylPt2Unit);
	DDX_Control(pDX, IDC_TM_CYL_PT2_EDIT, m_wndCylPt2);
	DDX_Control(pDX, IDC_TM_CYL_PT1_UNIT, m_wndCylPt1Unit);
	DDX_Control(pDX, IDC_TM_CYL_PT1_EDIT, m_wndCylPt1);
	DDX_Control(pDX, IDC_TM_PLANE_PT3_UNIT, m_wndPlanePt3Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT3_EDIT, m_wndPlanePt3);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_UNIT, m_wndPlanePt2Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_EDIT, m_wndPlanePt2);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT, m_wndPlanePt1Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT, m_wndPlanePt1);
	DDX_Control(pDX, IDC_TM_LINE_PT2_UNIT, m_wndLinePt2Unit);
	DDX_Control(pDX, IDC_TM_LINE_PT2_EDIT, m_wndLinePt2);
	DDX_Control(pDX, IDC_TM_LINE_PT1_UNIT, m_wndLinePt1Unit);
	DDX_Control(pDX, IDC_TM_LINE_PT1_EDIT, m_wndLinePt1);
	DDX_Radio(pDX, IDC_TM_DIR_NORMAL, m_nDirection);
	DDX_Radio(pDX, IDC_TM_DIRECTION_PZ_CBO, m_wndThicknessDir);
	DDX_Check(pDX, IDC_TM_ET_REMOVE_CHECK2, m_chkReverseIJ);
	DDX_Check(pDX, IDC_TM_ET_MOVE_CHECK, m_bMoveSrc);
	//}}AFX_DATA_MAP
}

void CElemExtrudeDlg::AlignControl()
{
	CRect rRef;
	CRect rMove;
	int nDistY;

	// Thickness
	GetDlgItem(IDC_TM_SECT_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_THIK_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aThikCtrl, nDistY);

	// Unequal
	GetDlgItem(IDC_TM_EQU_XYZ_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_STATIC4)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aUnEqualCtrl, nDistY);

	//Thickness in Translate
	GetDlgItem(IDC_TM_EQU_XYZ_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_THICKNESS_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aThicknessCtrl, nDistY);

/*  // Rotate
	GetDlgItem(IDC_TM_TRAN_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ROT_FRAME)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aRotateCtrl, nDistY);  */

	// Project 추가로 수정.  2000.7.3 Water tree.
	GetDlgItem(IDC_TM_PRJTYPE_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ROT_FRAME)->GetWindowRect(rMove);  //Rotate 위치 변경
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aRotateCtrl, nDistY);  
	GetDlgItem(IDC_TM_TRAN_FRAME)->GetWindowRect(rMove);  // Translate 위치변경
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_aTranslateCtrl, nDistY);  

	// Project 관련 서브 항목 위치 설정.
	UINT aProjectSubCtrlID[] = {
		IDC_TM_PLANE_PT1_TITLE, IDC_TM_CYL_PT1_TITLE, IDC_TM_CONE_PT1_TITLE,
		IDC_TM_SPHR_ORG_TITLE, IDC_TM_ELIP_ORG_TITLE, IDC_TM_ELEM_TITLE
	};
	CArray<UINT, UINT> *aCtrlSet[] = { 
		&m_aPlaneCtrl, &m_aCylinderCtrl, &m_aConeCtrl,
		&m_aSphereCtrl, &m_aEllipsoidCtrl, &m_aElementCtrl,
	};

	GetDlgItem(IDC_TM_LINE_PT1_TITLE)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aProjectSubCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aProjectSubCtrlID[i])->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CtrlMoveDistY(*aCtrlSet[i], nDistY);
	}

	// *^^* Elem Type 추가 
	// Tension Force를 Tension Dist와 같은 높이에
	GetDlgItem(IDC_TM_TT_DIST_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_TT_FORCE_TITLE)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CtrlMoveDistY(m_TTForceCtrl, nDistY);

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
	CtrlMoveDistY(m_arPlateTypeCtrl, nDistY+5);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_TOLERANCE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+1;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CElemExtrudeDlg::ChangeStateOfElemTypeSubCtrls()
{
	int nIndex = m_wndElemType.GetCurSel(); ASSERT(nIndex >= 0);
	int nElemType = m_wndElemType.GetItemData(nIndex);

	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bNormalElem = FALSE;
	BOOL bTensTruss = FALSE;
	BOOL bCompTruss = FALSE;
	BOOL bWall = FALSE;
	BOOL bPlate = FALSE;       // Added by Bugboy for Plate Control 2000.9.6
	if (pDoc->m_pAttrCtrl->IsTensTruss(nElemType)) bTensTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsCompTruss(nElemType)) bCompTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsWall(nElemType)) bWall = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlate(nElemType)) bPlate = TRUE;
	else bNormalElem = TRUE;
	
	CtrlShowHide(m_TTCtrl,  bTensTruss);
	if (bTensTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_TTRadioCtrl, nCheck);
		CtrlShowHide(m_TTDistCtrl,  nCheck == 1);
		CtrlShowHide(m_TTForceCtrl,  nCheck == 2);
	}

	CtrlShowHide(m_CTCtrl,  bCompTruss);
	if (bCompTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_CTRadioCtrl, nCheck);
		CtrlShowHide(m_CTDistCtrl,  nCheck == 1);
	}
	CtrlShowHide(m_WallCtrl, bWall);
	CtrlShowHide(m_arPlateTypeCtrl, bPlate);
}

void CElemExtrudeDlg::InitializeElemTypeSubCtrls()
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
	m_wndTTDistEdit.SetWindowText(_T("0"));
	m_wndTTForceEdit.SetWindowText(_T("0"));
	m_wndCTDistEdit.SetWindowText(_T("0"));
	m_wndWallID.SetWindowText(_T("1"));
	
	m_wndTTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTTForceUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CElemExtrudeDlg::ShowHideByGenType(int nGenerationType)
{
	CtrlShowHide(m_aTranslateCtrl, nGenerationType == 0);

	if(nGenerationType == 0) 
	{
		CButton* btn = (CButton*)GetDlgItem(IDC_TM_THICKNESS_RADIO);
		btn->ShowWindow(m_wndExtrudeType.GetCurSel() == 2);           // Thickness 살림.
		OnChangeTranOption(); 
	}

	CtrlShowHide(m_aRotateCtrl, nGenerationType == 1);
	ShowHideProjectCtrls(nGenerationType == 2);

/*  // *^^* TT/CT/Wall Option추가로 Enable/Disable에서 Show/Hide로 변경
	if (nGenerationType == 0)
	{
		CtrlShowHide(m_aEqualCtrl, m_nTranOption == 0);
		CtrlShowHide(m_aUnEqualCtrl, m_nTranOption == 1);
	}  */   // Control Event에서 바로 처리.
}

void CElemExtrudeDlg::ShowHideElemAttr(int nElemType)
{
	BOOL bMatl = m_pDoc->m_pAttrCtrl->IsNeedMatl(nElemType);
	BOOL bSect = m_pDoc->m_pAttrCtrl->IsNeedSect(nElemType);
	BOOL bThik = m_pDoc->m_pAttrCtrl->IsNeedThik(nElemType);

	CtrlEnableDisable(m_aMatlCtrl, bMatl);
	CtrlEnableDisable(m_aSectCtrl, bSect);
	CtrlEnableDisable(m_aThikCtrl, bThik);
	if (!bSect && !bThik) // 둘다 필요 없으면 Thickness는 보이게 한다.(Disable된 채로)
	{
		CtrlShowHide(m_aSectCtrl, FALSE);
		CtrlShowHide(m_aThikCtrl, TRUE);
	}
	else
	{
		CtrlShowHide(m_aSectCtrl, bSect);
		CtrlShowHide(m_aThikCtrl, bThik);
	}
}

void CElemExtrudeDlg::FillElemTypeByExtrudeType(int nExtrudeType)
{
	m_wndElemType.ResetContent();

#ifdef _MVIL
	CString aElemType1[] = {_LS(IDS_WG_TREEMENU_Truss), _LS(IDS_WG_TREEMENU_Beam)};
	UINT    aElemID1[] = {TRUSS_EL, BEAM_EL};
#else
	CString aElemType1[] = {_LS(IDS_WG_TREEMENU_Truss), _LS(IDS_WG_TREEMENU_Beam), _LS(IDS_WG_TREEMENU_Tension), _LS(IDS_WG_TREEMENU_Compression)};
	UINT    aElemID1[] = {TRUSS_EL, BEAM_EL, TRTENS_EL, TRCOMP_EL};
#endif
#ifdef _MGEN
	CString aElemType2[] = {_LS(IDS_WG_TREEMENU_Plane_Stress), _LS(IDS_WG_TREEMENU_Plate), _LS(IDS_WG_TREEMENU_Wall), _LS(IDS_WG_TREEMENU_Plane_Strain), _LS(IDS_WG_TREEMENU_Axisymmetric)};
	UINT    aElemID2[] = {PLSTRS_EL, PLATE_EL, WALL_EL, PLSTRN_EL, AXISYM_EL};
#elif _CIVIL
	CString aElemType2[] = {_LS(IDS_WG_TREEMENU_Plane_Stress), _LS(IDS_WG_TREEMENU_Plate), _LS(IDS_WG_TREEMENU_Plane_Strain), _LS(IDS_WG_TREEMENU_Axisymmetric)};
	UINT    aElemID2[] = {PLSTRS_EL, PLATE_EL, PLSTRN_EL, AXISYM_EL};
#elif _TOWER
	CString aElemType2[] = {_LS(IDS_WG_TREEMENU_Plane_Stress), _LS(IDS_WG_TREEMENU_Plate), _LS(IDS_WG_TREEMENU_Plane_Strain), _LS(IDS_WG_TREEMENU_Axisymmetric)};
	UINT    aElemID2[] = {PLSTRS_EL, PLATE_EL, PLSTRN_EL, AXISYM_EL};
#endif
	CString aElemType3[] = {_LS(IDS_WG_TREEMENU_Solid)};
	UINT    aElemID3[] = {SOLID_EL};
	int i, nIndex;
	CString csDefault;
	switch(nExtrudeType)
	{
	case 0: // Node -> Line
		for(i=0; i < sizeof(aElemType1)/sizeof(CString); i++)
		{
			nIndex = m_wndElemType.AddString(aElemType1[i]);
			m_wndElemType.SetItemData(nIndex, aElemID1[i]);
		}
		csDefault = aElemType1[1];  // Beam
		break;
	case 1: // Line -> Planar
		for(i=0; i < sizeof(aElemType2)/sizeof(CString); i++)
		{
			nIndex = m_wndElemType.AddString(aElemType2[i]);
			m_wndElemType.SetItemData(nIndex, aElemID2[i]);
		}
		csDefault = aElemType2[1]; // Plate
		break;
	case 2: // Planar -> Solid
		for(i=0; i < sizeof(aElemType3)/sizeof(CString); i++)
		{
			nIndex = m_wndElemType.AddString(aElemType3[i]);
			m_wndElemType.SetItemData(nIndex, aElemID3[i]);
		}
		csDefault = aElemType3[0]; // Solid
		break;
	}
	nIndex = m_wndElemType.FindString(0, csDefault);
	if (nIndex < 0) nIndex = 0;
	m_wndElemType.SetCurSel(nIndex);

	// Add by Water Tree
	// For Thickness in Translate Group
	UpdateData(TRUE);
	CButton* btn = (CButton*)GetDlgItem(IDC_TM_THICKNESS_RADIO);
	btn->ShowWindow((nExtrudeType == 2) && (m_nGenerationType == 0));  // Thickness 살림
	if(m_nTranOption == 2 && nExtrudeType != 2 && m_nGenerationType == 0)
	{
		m_nTranOption = 0;
		UpdateData(FALSE);
		OnChangeTranOption(); 
	}
}

BOOL CElemExtrudeDlg::GetParamForTranslate(_ElemExtrude& EE)
{
	CString strDirVector;
	CString strEQDist;
	CString strUEQDist;
	CString strIterNum;
	//double dwx,dwy,dwz;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	EE.m_nOption = m_nTranOption;

	if (m_nTranOption == 0)  // Equal
	{
		m_wndEQDist.GetWindowText(strEQDist);
		if(TRUE == m_wndEQDist.IsKeyPressed())
		{
			if(GetEQDistance(strEQDist,EE.m_EQ_UX, EE.m_EQ_UY, EE.m_EQ_UZ)) 
			{
				pIGM->GetWcsVector(EE.m_EQ_UX, EE.m_EQ_UY, EE.m_EQ_UZ,EE.m_EQ_WX, EE.m_EQ_WY, EE.m_EQ_WZ);
			}
			else return FALSE;
		}
		else
		{
			//m_wndEQDist.GetCurMousePosDist(dwx,dwy,dwz,EE.m_EQ_DX,EE.m_EQ_DY,EE.m_EQ_DZ);
			m_wndEQDist.GetCurMousePosDist(EE.m_EQ_WX, EE.m_EQ_WY, EE.m_EQ_WZ,
																		 EE.m_EQ_UX, EE.m_EQ_UY, EE.m_EQ_UZ);
		}
	}
	else if (m_nTranOption == 1) // Unequal
	{
		EE.m_nUEQ_Axis = this->m_nTranChooseAxis;
		
		m_wndUEQDist.GetWindowText(strUEQDist);
		if(GetUEDistance(strUEQDist,EE.m_UEQ_DistList)) ;
		else return FALSE;

		if(!EE.m_UEQ_DistList.GetSize()) return FALSE;

		EE.m_UEQ_UX  = 0.;
		EE.m_UEQ_UY  = 0.;
		EE.m_UEQ_UZ  = 0.;

		switch(m_nTranChooseAxis)
		{
		case 0: // X Axis...
			EE.m_UEQ_UX = 1.0;
			pIGM->GetWcsVector(EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ,
												 EE.m_UEQ_WX,EE.m_UEQ_WY,EE.m_UEQ_WZ);
			break;
		case 1: // Y Axis...
			EE.m_UEQ_UY = 1.0;
			pIGM->GetWcsVector(EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ,
												 EE.m_UEQ_WX,EE.m_UEQ_WY,EE.m_UEQ_WZ);
			break;
		case 2: // Z Axis...
			EE.m_UEQ_UZ = 1.0;
			pIGM->GetWcsVector(EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ,
												 EE.m_UEQ_WX,EE.m_UEQ_WY,EE.m_UEQ_WZ);
			break;
		case 3: // Arbitrary...
			{
				m_wndDirVector.GetWindowText(strDirVector);
				if(TRUE == m_wndDirVector.IsKeyPressed())
				{
					if(GetDirectionVector(strDirVector,EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ))
					{
						 pIGM->GetWcsVector(EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ,
																EE.m_UEQ_WX,EE.m_UEQ_WY,EE.m_UEQ_WZ);
					}
					else return FALSE;
				}
				else
				{
					//double dwx,dwy,dwz;
					m_wndDirVector.GetCurMousePosDist(EE.m_UEQ_WX,EE.m_UEQ_WY,EE.m_UEQ_WZ,
																						EE.m_UEQ_UX,EE.m_UEQ_UY,EE.m_UEQ_UZ);
				}
				
				double VL = EE.m_UEQ_WX * EE.m_UEQ_WX + EE.m_UEQ_WY * EE.m_UEQ_WY + 
										EE.m_UEQ_WZ * EE.m_UEQ_WZ ; 
				
				
				
				if(pIGM->IsZero(VL))
				{
					// Error Message 표시할것 ..
					return FALSE;
				}
				
				VL = sqrt(VL);

				EE.m_UEQ_WX = EE.m_UEQ_WX / VL;
				EE.m_UEQ_WY = EE.m_UEQ_WY / VL;
				EE.m_UEQ_WZ = EE.m_UEQ_WZ / VL;
			}
			break;
		default:
			return FALSE;
		}
	}
	else if (m_nTranOption == 2)  // Thickness
	{
		//		double dThickness, int nDirection,
		CString strThickness;
		m_wndThicknessEdit.GetWindowText(strThickness);
		if(!GetFloatNumber(strThickness, EE.dThickness)) return FALSE;
		EE.nDirection = m_wndThicknessDir;
	}

	if (m_nTranOption == 0) // when equal distance
	{
		m_wndIterNum.GetWindowText(strIterNum);
		if(!GetNumberOfTimes(strIterNum,EE.m_NTimes)) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_copy_number_));
			return FALSE;
		}
	}
	else if(m_nTranOption == 1)// when unequal distance
	{
		EE.m_NTimes = EE.m_UEQ_DistList.GetSize();
	}

	return TRUE;
}

BOOL CElemExtrudeDlg::GetParamForRotate(_ElemExtrude& EE)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	EE.m_nAxis = m_nRotAxis;  // 0 : X , 1 : Y , 2 : Z , 3 : 2Point

	CString strIterNum;
	m_wndRotIterNum.GetWindowText(strIterNum);

	if(!GetNumberOfTimes(strIterNum,EE.m_nRotNTimes)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_copy_number_));
		return FALSE;
	}
	
	if (!m_wndRotAngle.GetEditValue(EE.m_AngleOfRotDEG)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_rotation_angle_));
		return FALSE;
	}

// 임시  Angle값을 각각 받거나, 하나의 Angle값으로 Array 할 수 있게 한다. 
// Added by BugBoy 2000.11.2
	EE.m_arAngle.RemoveAll();
	for(int i=0; i<EE.m_nRotNTimes; i++)
		EE.m_arAngle.Add(EE.m_AngleOfRotDEG);
	
	if(!GetDistanceDrDz(EE.m_DistR,EE.m_DistZ)) return FALSE; /// *************************
	
	if(Get1st2ndPoint(m_wnd1stPoint, EE.m_FirstP_UX,EE.m_FirstP_UY,EE.m_FirstP_UZ))
	{
		pIGM->GetWcsPos(EE.m_FirstP_UX,EE.m_FirstP_UY,EE.m_FirstP_UZ,
										EE.m_FirstP_WX,EE.m_FirstP_WY,EE.m_FirstP_WZ);
	}
	else
		return FALSE;
	
	EE.m_AxisV_Ux = 0.0;
	EE.m_AxisV_Uy = 0.0;
	EE.m_AxisV_Uz = 0.0;
	switch(m_nRotAxis)
	{
	case 0:
		EE.m_AxisV_Ux = 1.0;
		pIGM->GetWcsVector(EE.m_AxisV_Ux,EE.m_AxisV_Uy,EE.m_AxisV_Uz,
											 EE.m_AxisV_Wx,EE.m_AxisV_Wy,EE.m_AxisV_Wz);
		break;
	case 1:
		EE.m_AxisV_Uy = 1.0;
		pIGM->GetWcsVector(EE.m_AxisV_Ux,EE.m_AxisV_Uy,EE.m_AxisV_Uz,
											 EE.m_AxisV_Wx,EE.m_AxisV_Wy,EE.m_AxisV_Wz);
		break;
	case 2:
		EE.m_AxisV_Uz = 1.0;
		pIGM->GetWcsVector(EE.m_AxisV_Ux,EE.m_AxisV_Uy,EE.m_AxisV_Uz,
											 EE.m_AxisV_Wx,EE.m_AxisV_Wy,EE.m_AxisV_Wz);
		break;
	case 3:
		{
			if(Get1st2ndPoint(m_wnd2ndPoint, EE.m_SecondP_UX,EE.m_SecondP_UY,EE.m_SecondP_UZ))
			{
				pIGM->GetWcsPos(EE.m_SecondP_UX,EE.m_SecondP_UY,EE.m_SecondP_UZ,
												EE.m_SecondP_WX,EE.m_SecondP_WY,EE.m_SecondP_WZ);
				
				EE.m_AxisV_Ux = EE.m_SecondP_UX - EE.m_FirstP_UX;
				EE.m_AxisV_Uy = EE.m_SecondP_UY - EE.m_FirstP_UY;
				EE.m_AxisV_Uz = EE.m_SecondP_UZ - EE.m_FirstP_UZ;

				double VL;
				
				VL = EE.m_AxisV_Ux*EE.m_AxisV_Ux +  EE.m_AxisV_Uy*EE.m_AxisV_Uy + 
						 EE.m_AxisV_Uz*EE.m_AxisV_Uz;

				VL = sqrt(VL);

				if(pIGM->IsZero(VL))
				{
					GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Rotation_axis_vector_is_zer));
					return FALSE;
				}

				EE.m_AxisV_Ux = EE.m_AxisV_Ux / VL;
				EE.m_AxisV_Uy = EE.m_AxisV_Uy / VL;
				EE.m_AxisV_Uz = EE.m_AxisV_Uz / VL;

				pIGM->GetWcsVector(EE.m_AxisV_Ux,EE.m_AxisV_Uy,EE.m_AxisV_Uz,
													 EE.m_AxisV_Wx,EE.m_AxisV_Wy,EE.m_AxisV_Wz);
			}
			else
				return FALSE;
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}

BOOL CElemExtrudeDlg::GetParamForProject(_ElemExtrude& EE)  // Added by Water Tree.
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	EE.nProjType = m_wndProjectType.GetCurSel()+1; // 1,2,3,4,5,6,7 for each type
	EE.nDirType = m_nDirection+1;   // 1 for normal, 2 for direction vector

	double ux,uy,uz;
	CString csValue;

	if(EE.nDirType == 2)
	{
		if(!Get3Point(m_wndDirPt1, ux, uy, uz)) return false;
		pIGM->GetWcsPos(ux, uy, uz, EE.dProjux, EE.dProjuy, EE.dProjuz);
	}

	switch(EE.nProjType)
	{
		case 1: // line
			if(!Get3Point(m_wndLinePt1, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);
			
			if(!Get3Point(m_wndLinePt2, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz, EE.dProjpt2[0], EE.dProjpt2[1], EE.dProjpt2[2]);
			break;

		case 2: // plane
			if(!Get3Point(m_wndPlanePt1, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);

			if(!Get3Point(m_wndPlanePt2, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt2[0], EE.dProjpt2[1], EE.dProjpt2[2]);

			if(!Get3Point(m_wndPlanePt3, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt3[0], EE.dProjpt3[1], EE.dProjpt3[2]);
			break;

	case 3: // cylinder
		if(!Get3Point(m_wndCylPt1, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);

		if(!Get3Point(m_wndCylPt2, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt2[0], EE.dProjpt2[1], EE.dProjpt2[2]);

		m_wndCylRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, EE.dProjRad[0])) return FALSE;
		break;

	case 4: // cone
		if(!Get3Point(m_wndConePt1, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);

		if(!Get3Point(m_wndConePt2, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt2[0], EE.dProjpt2[1], EE.dProjpt2[2]);

		m_wndConeRad1.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, EE.dProjRad[0])) return FALSE;

		m_wndConeRad2.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, EE.dProjRad[1])) return FALSE;
		break;

	case 5: // sphere
		if(!Get3Point(m_wndSphrOrg, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);

		m_wndSphrRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, EE.dProjRad[0])) return FALSE;
		break;

	case 6: // ellipsoid
		if(!Get3Point(m_wndElipOrg, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt1[0], EE.dProjpt1[1], EE.dProjpt1[2]);

		if(!Get3Point(m_wndElipPt1, ux, uy, uz)) return FALSE;
				pIGM->GetWcsPos(ux, uy, uz,	EE.dProjpt2[0], EE.dProjpt2[1], EE.dProjpt2[2]);

		m_wndElipRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, EE.dProjRad[0])) return FALSE;
		break;

	case 7: // element
		{
			CArray<UINT, UINT> aElemKey;
			CString csElemKey;
			m_wndElemNo.GetWindowText(csElemKey);

			// GetNodeList는 Elem List 구할 때도 사용가능하다.
			if (!GetNodeList(csElemKey, aElemKey)) return FALSE;
			if (aElemKey.GetSize() != 1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_1_element_and_try_ag));
				return FALSE;
			}
			EE.KeyElemTarget = aElemKey.GetAt(0);
		}
		break;

	default : ASSERT(0);
	}
	
	return TRUE;
}

BOOL CElemExtrudeDlg::Get3Point(CMouseEdit& pt, double &Px, double &Py, double &Pz)
{
//  if(pt.IsKeyPressed())
//  {
		CString strPos;
		pt.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
/*  }
	else
	{
		double wx, wy, wz;
		pt.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;  */
}

BOOL CElemExtrudeDlg::GetParamElemType(_ElemExtrude &EE)
{
	int nSel = m_wndElemType.GetCurSel();
	if (nSel == LB_ERR) return FALSE;
	int nElemType = m_wndElemType.GetItemData(nSel);

	EE.m_nSubType = 0;
	EE.m_dSubData = 0.0;
	EE.m_nWallID = 0;

	switch(nElemType)
	{
	case TRTENS_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_TTRadioCtrl, nCheck);
			EE.m_nSubType = nCheck+1;
			if (EE.m_nSubType == 2) // hook
			{
				CString csText;
				m_wndTTDistEdit.GetWindowText(csText);
				if(!GetFloatNumber(csText, EE.m_dSubData)) return FALSE;
			}
			else if (EE.m_nSubType == 3)  // cable
				EE.m_dSubData = m_wndTTForceEdit.GetEditValue();
		}
		break;
	case TRCOMP_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_CTRadioCtrl, nCheck);
			EE.m_nSubType = nCheck+1;
			if (EE.m_nSubType == 2) // gap
			{
				CString csText;
				m_wndCTDistEdit.GetWindowText(csText);
				if(!GetFloatNumber(csText, EE.m_dSubData)) return FALSE;
			}
		}
		break;
	case PLATE_EL:
		{
			if(this->IsDlgButtonChecked(IDC_TM_ELEM_TYPE_THIN))
				EE.m_nSubType = 1;
			else
				EE.m_nSubType = 0;
		}
		break;
	case WALL_EL:
		{
			int nCheck;
			CDlgUtil::CtrlRadioGetCheck(this, m_WallRadioCtrl, nCheck);
			EE.m_nSubType = nCheck+1;
			if (!m_wndWallID.GetEditValue(EE.m_nWallID)) 
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Wall_ID_));
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CElemExtrudeDlg::Get1st2ndPoint(CMouseEdit& pt, double &Px, double &Py, double &Pz)
{
	if(pt.IsKeyPressed())
	{
		CString strPos;
		pt.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		pt.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CElemExtrudeDlg::GetDistanceDrDz(double &Dr, double &Dz)
{
/*  CString strRot;
	m_wndDist.GetWindowText(strRot);
	
	return Get2FloatNumber(strRot,Dr,Dz);  */
	CString strRadial, strAxial;
	m_wndDistR.GetWindowText(strRadial);
	m_wndDistA.GetWindowText(strAxial);

	BOOL bGet = GetFloatNumber(strRadial, Dr);
	bGet = bGet && GetFloatNumber(strAxial, Dz);
	
	return bGet;
}


#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemExtrudeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemExtrudeDlg)
	ON_BN_CLICKED(IDC_TM_GT_TRANSLATE, OnChangeGT)
	ON_CBN_SELCHANGE(IDC_TM_ET_COMBO, OnSelchangeTmEtCombo)
	ON_BN_CLICKED(IDC_TM_MATL_BTN, OnClickPropBtn)
	ON_CBN_SELCHANGE(IDC_TM_EA_ELEM_TYPE, OnChangeElemType)
	ON_BN_CLICKED(IDC_TM_EQUAL_DIST_RADIO, OnChangeTranOption)
	ON_BN_CLICKED(IDC_TM_XAXIS_RADIO, OnChangeTranAxis)
	ON_BN_CLICKED(IDC_TM_ROT_AXIS_X, OnChangeRotAxis)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ROT_ANGLE_SPIN, OnDeltaposTmRotAngleSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ITER_NUM_SPIN2, OnDeltaposTmIterNumSpin)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_TRUSS, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_TRUSS, OnChangeCT)
	ON_CBN_SELCHANGE(IDC_TM_PRJTYPE_CBO, OnSelchangeTmPrjtypeCbo)
	ON_BN_CLICKED(IDC_TM_DIR_2PT_AXIS, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_GT_ROTATE, OnChangeGT)
	ON_BN_CLICKED(IDC_TM_SECT_BTN, OnClickPropBtn)
	ON_BN_CLICKED(IDC_TM_THIK_BTN, OnClickPropBtn)
	ON_BN_CLICKED(IDC_TM_UNEQUAL_DIST_RADIO, OnChangeTranOption)
	ON_BN_CLICKED(IDC_TM_YAXIS_RADIO, OnChangeTranAxis)
	ON_BN_CLICKED(IDC_TM_ZAXIS_RADIO, OnChangeTranAxis)
	ON_BN_CLICKED(IDC_TM_ARBITRARY_RADIO, OnChangeTranAxis)
	ON_BN_CLICKED(IDC_TM_ROT_AXIS_Y, OnChangeRotAxis)
	ON_BN_CLICKED(IDC_TM_ROT_AXIS_Z, OnChangeRotAxis)
	ON_BN_CLICKED(IDC_TM_ROT_AXIS_2POINT, OnChangeRotAxis)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ROT_NTIMES_SPIN, OnDeltaposTmIterNumSpin)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_HOOK, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_CABLE, OnChangeTT)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_GAP, OnChangeCT)
	ON_BN_CLICKED(IDC_TM_GT_PROJECT, OnChangeGT)
	ON_BN_CLICKED(IDC_TM_DIR_NORMAL, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_THICKNESS_RADIO, OnChangeTranOption)
	ON_BN_CLICKED(IDC_TM_ET_REMOVE_CHECK, OnTmEtRemoveCheck)
	ON_BN_CLICKED(IDC_TM_ET_MOVE_CHECK, OnTmEtMoveCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemExtrudeDlg message handlers

BOOL CElemExtrudeDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	AlignControl();

	m_wndExtrudeType.AddString(_LS(IDS_WG_TREEMENU_Node____Line_Element));
#ifndef _MVIL
	m_wndExtrudeType.AddString(_LS(IDS_WG_TREEMENU_Line_Elem_____Planar_Elem_));
	m_wndExtrudeType.AddString(_LS(IDS_WG_TREEMENU_Planar_Elem_____Solid_Elem_));
#endif

	m_wndMatlNo.SetComboBoxPoint(&m_wndMatlName);
	m_wndSectNo.SetComboBoxPoint(&m_wndSectName);
	m_wndThikNo.SetComboBoxPoint(&m_wndThikName);
	m_wndMatlName.SetEditBoxPoint(&m_wndMatlNo);
	m_wndSectName.SetEditBoxPoint(&m_wndSectNo);
	m_wndThikName.SetEditBoxPoint(&m_wndThikNo);
	m_wndMatlNo.SetWindowText(_T("1"));
	m_wndSectNo.SetWindowText(_T("1"));
	m_wndThikNo.SetWindowText(_T("1"));

	// For Translate
	m_wndEQDist.SetAttUcsDistance();
	m_wndEQDist.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDirVector.SetAttUcsDistance();
	m_wndDirVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndThicknessEdit.SetAttUcsDistance();
	m_wndThicknessEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndThicknessEdit.SetDistValueMode(TRUE);
		
	m_wndDistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndThicknessUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndIterNum.SetRange(0, SHRT_MAX);
	m_wndIterNum.SetWindowText(_T("1"));
	m_wndIterNum.SetInteger(TRUE);
	m_wndDirVector.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndEQDist.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndThicknessEdit.SetWindowText(_T("0"));

	// For Rotate
	m_wnd1stPoint.SetAttUcsPos();
	m_wnd1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd2ndPoint.SetAttUcsPos();
	m_wnd2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd1stPoint.SetNextLink(&m_wnd2ndPoint);
	m_wnd2ndPoint.SetNextLink(NULL);
	m_wndDistR.SetAttUcsDistance();
	m_wndDistR.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDistR.SetDistValueMode(TRUE);
	m_wndDistA.SetAttUcsDistance();
	m_wndDistA.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDistA.SetDistValueMode(TRUE);

	m_wndRotAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndDistRUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistAUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd1stUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd2ndUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndDistR.SetWindowText(_T("0"));
	m_wndDistA.SetWindowText(_T("0"));
	m_wndRotAngle.SetRange(-360, 360);
	m_wndRotAngle.SetWindowText(_T("0")); 
	m_wnd2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0)); 
	m_wnd1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));  
	m_wndRotIterNum.SetRange(0, SHRT_MAX);
	m_wndRotIterNum.SetWindowText(_T("1"));
	m_wndRotIterNum.SetInteger(TRUE);

	// Default
	m_wndExtrudeType.SetCurSel(0);
	m_bRemoveSrc = TRUE;
	m_nGenerationType = 0;
	m_nTranOption = 0;
	m_nTranChooseAxis = 0;
	m_nRotAxis = 0;         

	UpdateData(FALSE);

	CtrlShowHide(m_aThikCtrl, FALSE);  
	ShowHideByGenType(m_nGenerationType);
	FillElemTypeByExtrudeType(m_wndExtrudeType.GetCurSel());
	ShowHideElemAttr(m_wndElemType.GetItemData(m_wndElemType.GetCurSel()));
	m_wndRemoveCheck.EnableWindow(m_wndExtrudeType.GetCurSel() != 0);
	// reverse i-j added by bugboy
	((CButton*)GetDlgItem(IDC_TM_ET_REMOVE_CHECK2))->EnableWindow(m_wndExtrudeType.GetCurSel()==0);  

	// *^^* TT/CT/Wall Option추가로 Enable/Disable에서 Show/Hide로 변경
	CtrlShowHide(m_aEqualCtrl, m_nTranOption == 0);
	CtrlShowHide(m_aUnEqualCtrl, m_nTranOption == 1);
	CtrlShowHide(m_aThicknessCtrl, m_nTranOption == 2);
	CtrlEnableDisable(m_aVectorCtrl, m_nTranOption == 1 && m_nTranChooseAxis == 3);

	CtrlEnableDisable(m_aRot2ndPtCtrl, m_nRotAxis == 3);

	// *^^* Elemement Sub control
	InitializeElemTypeSubCtrls();
	ChangeStateOfElemTypeSubCtrls();

	//Project Type Sub Control
	InitializeProjectSubCtrls();

	// Plate Control 초기값
	((CButton *)GetDlgItem(IDC_TM_ELEM_TYPE_THICK))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CElemExtrudeDlg::GetParameter(_ElemExtrude& EE)
{
	if (!UpdateData(TRUE)) return FALSE;
	
	// Get Parameter
	EE.m_nExtrudeType = m_wndExtrudeType.GetCurSel();
	EE.m_bMoveSrc = m_bMoveSrc;
	EE.m_bRemoveSrc = m_bRemoveSrc;
	EE.m_nElemType = m_wndElemType.GetItemData(m_wndElemType.GetCurSel());
	m_wndMatlNo.GetKey(EE.m_nMatlNo);
	m_wndSectNo.GetKey(EE.m_nSectNo);
	m_wndThikNo.GetKey(EE.m_nThikNo);
	EE.m_nGenerationType = m_nGenerationType;
	EE.m_bReverseIJ = m_chkReverseIJ;

	if (EE.m_nElemType == TRTENS_EL || EE.m_nElemType == TRCOMP_EL || EE.m_nElemType == PLATE_EL || EE.m_nElemType == WALL_EL)
	{
		if(!GetParamElemType(EE)) return FALSE;
	}


	switch(m_nGenerationType)
	{
		case 0:  // For Translate
			if (!GetParamForTranslate(EE)) return FALSE;
			break;
		case 1:  // For Rotate
			if (!GetParamForRotate(EE)) return FALSE;
			break;
		case 2:  // For Project
			if (!GetParamForProject(EE)) return FALSE;
			break;
	}

	return TRUE;
}

BOOL CElemExtrudeDlg::ExtrudeElem(_ElemExtrude& EE)
{
	/*
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> aKeyNode, aKeyElem;
	
	if (EE.m_nExtrudeType == 0) // node -> line
	{
		if(!pIGM->GetSelectedNodeKeyList(aKeyNode))
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
			return FALSE;
		}
	}
	else  // line -> planar, planar -> solid
	{
		if(!pIGM->GetSelectedElemKeyList(aKeyElem))
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
			return FALSE;
		}
	}

	BOOL bSuccess;

	switch(EE.m_nGenerationType)
	{
		case 0: // Translate
			if (EE.m_nOption == 0)  // Equal Distance
				bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Translate_Equal(EE.m_nExtrudeType+1,
										EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID, EE.m_BetaAngle,
										aKeyNode, aKeyElem, EE.m_NTimes, 
										EE.m_EQ_WX, EE.m_EQ_WY, EE.m_EQ_WZ, EE.m_nMatlNo,
										EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc, EE.m_bMoveSrc, EE.m_bReverseIJ);  
			else if (EE.m_nOption == 1) // Unequal Distance
				bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Translate_Unequal(EE.m_nExtrudeType+1,
										EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID, EE.m_BetaAngle,
										aKeyNode, aKeyElem, EE.m_NTimes,
										EE.m_UEQ_WX, EE.m_UEQ_WY, EE.m_UEQ_WZ, EE.m_UEQ_DistList, 
										EE.m_nMatlNo, EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc, EE.m_bMoveSrc, EE.m_bReverseIJ);
			break;
		case 1:	// Rotate
			T_AXIS_VECTOR AxisV;
		
			AxisV.dblOrgX = EE.m_FirstP_WX;
			AxisV.dblOrgY = EE.m_FirstP_WY;
			AxisV.dblOrgZ = EE.m_FirstP_WZ;
			AxisV.dblVecX = EE.m_AxisV_Wx;
			AxisV.dblVecY = EE.m_AxisV_Wy;
			AxisV.dblVecZ = EE.m_AxisV_Wz;

			bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Rotate(EE.m_nExtrudeType+1,
										EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID, EE.m_BetaAngle,
										aKeyNode, aKeyElem, EE.m_arAngle,
										EE.m_DistR, EE.m_DistZ, AxisV, EE.m_nMatlNo,
										EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc, EE.m_bMoveSrc, EE.m_bReverseIJ);  
			break;
		case 2:  // Project
			bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Project(
				                   EE.m_nExtrudeType+1, EE.m_nElemType, 
													 EE.m_nSubType, EE.m_dSubData, EE.m_nWallID,
													 aKeyNode, aKeyElem, EE.nProjType, EE.nDirType,
													 EE.dProjux, EE.dProjuy, EE.dProjuz,
													 EE.dProjpt1, EE.dProjpt2, EE.dProjpt3, EE.dProjRad, EE.KeyElemTarget, 
													 EE.m_nMatlNo, EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc, EE.m_bMoveSrc, EE.m_bReverseIJ); 
			break;
	}
	*/

/*  if (EE.m_nGenerationType == 0)  // Translate
	{
		if (EE.m_nOption == 0)  // Equal Distance
			bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Translate_Equal(EE.m_nExtrudeType+1,
									EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID, 
									aKeyNode, aKeyElem, EE.m_NTimes, 
									EE.m_EQ_WX, EE.m_EQ_WY, EE.m_EQ_WZ, EE.m_nMatlNo,
									EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc);
		else  // Unequal Distance
			bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Translate_Unequal(EE.m_nExtrudeType+1,
									EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID,
									aKeyNode, aKeyElem, EE.m_NTimes,
									EE.m_UEQ_WX, EE.m_UEQ_WY, EE.m_UEQ_WZ, EE.m_UEQ_DistList, 
									EE.m_nMatlNo, EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc);
	}
	else  // Rotate
	{
		T_AXIS_VECTOR AxisV;
		
		AxisV.dblOrgX = EE.m_FirstP_WX;
		AxisV.dblOrgY = EE.m_FirstP_WY;
		AxisV.dblOrgZ = EE.m_FirstP_WZ;
		AxisV.dblVecX = EE.m_AxisV_Wx;
		AxisV.dblVecY = EE.m_AxisV_Wy;
		AxisV.dblVecZ = EE.m_AxisV_Wz;

		bSuccess = m_pDoc->m_pDataCtrl->ExtrudeElem_Rotate(EE.m_nExtrudeType+1,
									EE.m_nElemType, EE.m_nSubType, EE.m_dSubData, EE.m_nWallID,
									aKeyNode, aKeyElem, EE.m_nRotNTimes, EE.m_AngleOfRotDEG,
									EE.m_DistR, EE.m_DistZ, AxisV, EE.m_nMatlNo,
									EE.m_nSectNo, EE.m_nThikNo, EE.m_bRemoveSrc);
	}  */

 // if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	return TRUE;
}

void CElemExtrudeDlg::Execute()
{
	_ElemExtrude EE;
	if(GetParameter(EE))
	{
		ExtrudeElem(EE);
	}
	else
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
}


void CElemExtrudeDlg::OnChangeGT() 
{
	// TODO: Add your control notification handler code here
/*	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	
	if (nID == IDC_TM_GT_TRANSLATE) m_nGenerationType = 0;
	else if (nID == IDC_TM_GT_ROTATE) m_nGenerationType = 1;
	else m_nGenerationType = 2; */
	UpdateData(TRUE);

	ShowHideByGenType(m_nGenerationType);
}


void CElemExtrudeDlg::OnSelchangeTmEtCombo() 
{
	// TODO: Add your control notification handler code here
	int nExtrudeType = m_wndExtrudeType.GetCurSel();
	FillElemTypeByExtrudeType(nExtrudeType);
	ShowHideElemAttr(m_wndElemType.GetItemData(m_wndElemType.GetCurSel()));
	m_wndRemoveCheck.EnableWindow(nExtrudeType != 0);

	// Reverse I-J  added by bugboy
	((CButton*)GetDlgItem(IDC_TM_ET_REMOVE_CHECK2))->EnableWindow(nExtrudeType==0);

	// *^^* Element Sub control 추가
	ChangeStateOfElemTypeSubCtrls();
}

void CElemExtrudeDlg::OnClickPropBtn() 
{
	UINT nCmdID;
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_MATL_BTN: nCmdID = ID_STRUCT_PROP_MATERIAL; break;
	case IDC_TM_SECT_BTN: nCmdID = ID_STRUCT_PROP_SECTION; break;
	case IDC_TM_THIK_BTN: nCmdID = ID_STRUCT_PROP_THICKNESS; break;
	default: ASSERT(0);
	}
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(nCmdID, 0));
}

void CElemExtrudeDlg::OnChangeElemType() 
{
	// TODO: Add your control notification handler code here
	ShowHideElemAttr(m_wndElemType.GetItemData(m_wndElemType.GetCurSel()));
	
	// *^^* Element Sub control 추가
	ChangeStateOfElemTypeSubCtrls();
}

void CElemExtrudeDlg::OnChangeTranOption() 
{
	// TODO: Add your control notification handler code here
/*	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	if (nID == IDC_TM_EQUAL_DIST_RADIO) m_nTranOption = 0;
	else m_nTranOption = 1;  */
	if(m_nTranOption == 2 && m_wndExtrudeType.GetCurSel() != 2 && m_nGenerationType == 0)
	{
		m_nTranOption = 0;
		UpdateData(FALSE);
	}

	UpdateData(TRUE);

	// *^^* TT/CT/Wall Option추가로 Enable/Disable에서 Show/Hide로 변경
	CtrlShowHide(m_aEqualCtrl, m_nTranOption == 0);
	CtrlShowHide(m_aUnEqualCtrl, m_nTranOption == 1);
	CtrlShowHide(m_aThicknessCtrl, m_nTranOption == 2);
	CtrlEnableDisable(m_aVectorCtrl, m_nTranOption == 1 && m_nTranChooseAxis == 3);
}

void CElemExtrudeDlg::OnChangeTranAxis() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_XAXIS_RADIO: m_nTranChooseAxis = 0; break;
	case IDC_TM_YAXIS_RADIO: m_nTranChooseAxis = 1; break;
	case IDC_TM_ZAXIS_RADIO: m_nTranChooseAxis = 2; break;
	case IDC_TM_ARBITRARY_RADIO: m_nTranChooseAxis = 3; break;
	default : ASSERT(0);
	}
	CtrlEnableDisable(m_aVectorCtrl, m_nTranOption == 1 && m_nTranChooseAxis == 3);
}

void CElemExtrudeDlg::OnChangeRotAxis() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_ROT_AXIS_X: m_nRotAxis = 0; break;
	case IDC_TM_ROT_AXIS_Y: m_nRotAxis = 1; break;
	case IDC_TM_ROT_AXIS_Z: m_nRotAxis = 2; break;
	case IDC_TM_ROT_AXIS_2POINT: m_nRotAxis = 3; break;
	default : ASSERT(0);
	}
	CtrlEnableDisable(m_aRot2ndPtCtrl, m_nRotAxis == 3);
}

void CElemExtrudeDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CElemExtrudeDlg::OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csAngle;
	double dblAngle;
	CFormulaEdit* pwndAngle = (CFormulaEdit*)GetDlgItem(IDC_TM_ROT_ANGLE_EDIT);
	pwndAngle->GetEditValue(dblAngle);

	if(pNMUpDown->iDelta < 0)
		dblAngle -= (double)(pNMUpDown->iDelta);
	else
		dblAngle -= (double)(pNMUpDown->iDelta);
	if (dblAngle > 360.0) dblAngle = 360.0;
	else if (dblAngle < -360.0) dblAngle = -360.0;

	csAngle.Format(_LS(IDS_WG_TREEMENU__g), dblAngle);
	pwndAngle->SetWindowText(csAngle);
	*pResult = 0;
}

void CElemExtrudeDlg::OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;

	CFormulaEditSpin* pIterNum;
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_ITER_NUM_SPIN2: pIterNum = &m_wndIterNum; break;
	case IDC_TM_ROT_NTIMES_SPIN: pIterNum = &m_wndRotIterNum; break;
	default: ASSERT(0);
	}

	pIterNum->GetEditValue(NIter);
	NIter -= pNMUpDown->iDelta;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	pIterNum->SetWindowText(strIter);
	*pResult = 0;
}

void CElemExtrudeDlg::OnChangeTT() 
{
	ChangeStateOfElemTypeSubCtrls();
}

void CElemExtrudeDlg::OnChangeCT() 
{
	ChangeStateOfElemTypeSubCtrls();	
}

void CElemExtrudeDlg::InitializeProjectSubCtrls()
{
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_a_line));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_a_plane));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_a_cylinder));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_a_cone));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_a_sphere));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_an_ellipsoid));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_on_an_element));

	// line
	m_wndLinePt1.SetAttUcsPos();
	m_wndLinePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndLinePt2.SetAttUcsPos();
	m_wndLinePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndLinePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLinePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLinePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndLinePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndLinePt1.SetNextLink(&m_wndLinePt2);
	m_wndLinePt2.SetNextLink(NULL);
	// plane
	m_wndPlanePt1.SetAttUcsPos();
	m_wndPlanePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt2.SetAttUcsPos();
	m_wndPlanePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt3.SetAttUcsPos();
	m_wndPlanePt3.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt3Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndPlanePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndPlanePt3.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndPlanePt1.SetNextLink(&m_wndPlanePt2);
	m_wndPlanePt2.SetNextLink(&m_wndPlanePt3);
	m_wndPlanePt3.SetNextLink(NULL);
	// cylinder
	m_wndCylPt1.SetAttUcsPos();
	m_wndCylPt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCylPt2.SetAttUcsPos();
	m_wndCylPt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCylRad.SetAttUcsDistance();
	m_wndCylRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndCylRad.SetDistValueMode(TRUE);
	m_wndCylPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndCylPt2.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndCylRad.SetWindowText(_T("0"));
	m_wndCylPt1.SetNextLink(&m_wndCylPt2);
	m_wndCylPt2.SetNextLink(&m_wndCylRad);
	m_wndCylRad.SetNextLink(NULL);
	// cone
	m_wndConePt1.SetAttUcsPos();
	m_wndConePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndConePt2.SetAttUcsPos();
	m_wndConePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndConeRad1.SetAttUcsDistance();
	m_wndConeRad1.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndConeRad1.SetDistValueMode(TRUE);
	m_wndConeRad2.SetAttUcsDistance();
	m_wndConeRad2.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndConeRad2.SetDistValueMode(TRUE);
	m_wndConePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConeRad1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConeRad2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndConePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndConeRad1.SetWindowText(_T("0"));
	m_wndConeRad2.SetWindowText(_T("0"));
	m_wndConePt1.SetNextLink(&m_wndConeRad1);
	m_wndConePt2.SetNextLink(&m_wndConeRad2);
	m_wndConeRad1.SetNextLink(&m_wndConePt2);
	m_wndConeRad2.SetNextLink(NULL);
	// sphere
	m_wndSphrOrg.SetAttUcsPos();
	m_wndSphrOrg.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndSphrRad.SetAttUcsDistance();
	m_wndSphrRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndSphrRad.SetDistValueMode(TRUE);
	m_wndSphrOrgUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSphrRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSphrOrg.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndSphrRad.SetWindowText(_T("0"));
	m_wndSphrOrg.SetNextLink(&m_wndSphrRad);
	m_wndSphrRad.SetNextLink(NULL);
	// ellipsoid
	m_wndElipOrg.SetAttUcsPos();
	m_wndElipOrg.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndElipRad.SetAttUcsDistance();
	m_wndElipRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndElipRad.SetDistValueMode(TRUE);
	m_wndElipPt1.SetAttUcsPos();
	m_wndElipPt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndElipOrgUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipOrg.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndElipRad.SetWindowText(_T("0"));
	m_wndElipPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	m_wndElipOrg.SetNextLink(&m_wndElipRad);
	m_wndElipRad.SetNextLink(&m_wndElipPt1);
	m_wndElipPt1.SetNextLink(NULL);
	// element
	m_wndElemNo.SetAttElemList();
	m_wndElemNo.SetLButtonDownNotifyWindow(this);
	m_wndElemNo.SetEnterNotifyWindow(this);
	m_wndElemNo.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndElemNo.SetMaxElemKeyNum(1);
	// Direction
	m_wndDirPt1.SetAttUcsDistance();
	m_wndDirPt1.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndDirPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDirPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0___0___0));
	//Default
	m_wndProjectType.SetCurSel(0);
	OnChangeDirection();
}

void CElemExtrudeDlg::ShowHideProjectCtrls(BOOL flag)
{
	if(flag)
	{
		CtrlShowHide(m_aFrameCtrl, TRUE);		
		OnSelchangeTmPrjtypeCbo();		
	}
	else
	{
		CtrlShowHide(m_aFrameCtrl, FALSE);
		CtrlShowHide(m_aLineCtrl, FALSE);
		CtrlShowHide(m_aPlaneCtrl, FALSE);
		CtrlShowHide(m_aCylinderCtrl, FALSE);
		CtrlShowHide(m_aConeCtrl, FALSE);
		CtrlShowHide(m_aSphereCtrl, FALSE);
		CtrlShowHide(m_aEllipsoidCtrl, FALSE);
		CtrlShowHide(m_aElementCtrl, FALSE);
	}
}

void CElemExtrudeDlg::OnSelchangeTmPrjtypeCbo() 
{
	int nProjType = m_wndProjectType.GetCurSel();

	CtrlShowHide(m_aLineCtrl, nProjType == 0);
	CtrlShowHide(m_aPlaneCtrl, nProjType == 1);
	CtrlShowHide(m_aCylinderCtrl, nProjType == 2);
	CtrlShowHide(m_aConeCtrl, nProjType == 3);
	CtrlShowHide(m_aSphereCtrl, nProjType == 4);
	CtrlShowHide(m_aEllipsoidCtrl, nProjType == 5);
	CtrlShowHide(m_aElementCtrl, nProjType == 6);

//  ChangeBitmap(nProjType+1);

	CString aTitle[] = {
		_LS(IDS_WG_TREEMENU_Base_Line_Definition), _LS(IDS_WG_TREEMENU_Base_Plane_Definition),
		_LS(IDS_WG_TREEMENU_Base_Cylinder_Definition), _LS(IDS_WG_TREEMENU_Base_Cone_Definition),
		_LS(IDS_WG_TREEMENU_Base_Sphere_Definition), _LS(IDS_WG_TREEMENU_Base_Ellipsoid_Definition),
		_LS(IDS_WG_TREEMENU_Base_Element)
	};
	CBCGPStatic* pFrame = (CBCGPStatic*)GetDlgItem(IDC_TM_DEF_FRAME);
	pFrame->SetWindowText(aTitle[nProjType]);	
}

void CElemExtrudeDlg::OnChangeDirection() 
{
/*	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_DIR_NORMAL: m_nDirection = 0; break;
	case IDC_TM_DIR_2PT_AXIS: m_nDirection = 1; break;
	}  */

	UpdateData(TRUE);
	CtrlEnableDisable(m_aDirPt2Ctrl, m_nDirection == 1);
	if (m_nDirection == 1) m_wndDirPt1.SetFocus();	
}

void CElemExtrudeDlg::OnTmEtRemoveCheck() 
{
	UpdateData(TRUE);
	if(m_bMoveSrc)
	{
		m_bMoveSrc=0;
		UpdateData(FALSE);
	}	
}

void CElemExtrudeDlg::OnTmEtMoveCheck() 
{
	UpdateData(TRUE);
	if(m_bRemoveSrc)
	{
		m_bRemoveSrc=0;
		UpdateData(FALSE);
	}
}
