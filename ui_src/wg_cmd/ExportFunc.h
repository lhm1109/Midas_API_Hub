#if !defined(__EXPORTFUNC_H__)
#define __EXPORTFUNC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum D_CMD_DOMODAL_NUM {
 D_CMD_CCMSECTPAGE4DESIGN     ,
 D_CMD_CCMSECTPAGE4DESIGN_VIR ,
 D_CMD_CMACTIVEIDENTITY       ,
 D_CMD_CONSTRUCTION_STAGE     ,
 D_CMD_STAGE_DEFINE           ,
 D_CMD_TENDON_TYPE            ,
 D_CMD_TENDON_TYPE_ADD        ,
 D_CMD_STAGE_CHANGE           ,
 D_CMD_CONSTRUCTION_STAGE_HYD ,
 D_CMD_TDMT_CREEP_SHRINKAGE   ,
 D_CMD_TDME_ELASTICITY        ,
 D_CMD_TDMF_MODIFY            ,
 D_CMD_SECT_SCALE_FACTOR      ,
 D_CMD_SECT_PSC_REINFORCEMENT ,
 D_CMD_RPOS_LIST_DLG          ,
 D_CMD_ELSPLS_MATL_DLG        ,
 D_CMD_ELSPLS_MATL_ADD_DLG    ,
 D_CMD_ELDPLS_MICRO_PARAM_DLG ,
 D_CMD_MAINGIRDER             ,
 D_CMD_CROSSBEAM              ,
 D_CMD_GRIDLINE_LOAD_CASES    ,
 D_CMD_BRIDGE_TYPE_CASES      ,
 D_CMD_RESULT_LCRS_DLG        ,
 D_CMD_EXTERNAL_TYPE_DLG      ,
 D_CMD_CSCS_ITEM_DLG          ,
 D_CMD_PREB_ITEM_DLG          ,
 D_CMD_PRCS_ITEM_DLG          ,
 D_CMD_DPAN_ITEM_DLG          ,
 D_CMD_CCPT_ITEM_DLG          ,
 D_CMD_PSLT_ITEM_DLG          ,
 D_CMD_SDVI_ITEM_DLG          ,
 D_CMD_SDVE_ITEM_DLG          ,
 D_CMD_SDST_ITEM_DLG          ,
 D_CMD_SDHY_ITEM_DLG          ,
 D_CMD_SDIS_ITEM_DLG          ,
 D_CMD_RSMODFACT_DLG          ,
 D_CMD_GRIDANALCTRL_DLG       ,
 D_CMD_MHTP_DEFINE_DLG        ,
 D_CMD_FLUE_GAS_TEMP_DLG      ,
 D_CMD_STEEL_TIME_TEMP_DLG    ,
 D_CMD_FLUE_GAS_TEMP_ITEM_DLG      ,
 D_CMD_STEEL_TIME_TEMP_ITEM_DLG    ,
 D_CMD_TENDON_CHANGE_PROF_DLG	,
 D_CMD_ASGB_ITEM_DLG,
 D_CMD_DMGB_ITEM_DLG,
 D_CMD_DAMP_CODE_CHECK_INFO_DLG,
 D_CMD_DAMP_CODE_CHECK_PARAM_DLG,

 D_CMD_REINFORCE_ASSESSMENT_INFO_DLG,
 D_CMD_REINFORCE_DEFINE_SECT_DLG,
 D_CMD_REINFORCE_CHECK_DESIGN_INFO_DLG,

 D_CMD_PT_SET_TENDON_PROFILE_DLG,
 D_CMD_PT_SET_TENDON_STRESSING_DLG,
 D_CMD_PT_SET_TENDON_PROFILE_MATCH_DLG,
};

class CDBDoc;
namespace mit{
	namespace frx {
		/*!@#$*/class MBaseTabWnd;
	}
}

extern __declspec(dllexport) void* CreateOrActivateDlg(CDBDoc* pDoc, UINT nIDTemplate, CWnd* pParent = NULL, void* pExtData = NULL);
extern __declspec(dllexport) void* CreateOrActivatePropty(CDBDoc* pDoc,  int nActivePage);
// Tree menu related exported functions
//!@#$extern __declspec(dllexport) void AddTabToTabWnd(SEC3DTabWnd* pTabWnd, CRuntimeClass* pClass, CString sTabName);
extern __declspec(dllexport) void AddTabToTabWnd(mit::frx::MBaseTabWnd* pTabWnd, CRuntimeClass* pClass, CString sTabName);
//!@#$extern __declspec(dllexport) void CMD_CreateBarTreeDlg(CWnd* pParent, SEC3DTabWnd* pTabWnd, int nWhich);
extern __declspec(dllexport) void CMD_CreateBarTreeDlg(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich, CString& curString);

// Modal Dialog
extern __declspec(dllexport) void CMD_DoModal(int nWhich, void* Key = NULL);
extern __declspec(dllexport) void CMD_DoModal1(int nWhich, void* Key = NULL);
extern __declspec(dllexport) void CMD_DoModal_Stage(int nWhich, void* Key = NULL);
extern __declspec(dllexport) void CMD_DoModal_CreepShrinkage(void* Key = NULL);

extern __declspec(dllexport) int CMD_DoModal(LPCTSTR lpszDlgClassName, void* pExtraData=NULL);

#endif // !defined(__EXPORTFUNC_H__)