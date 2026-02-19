#ifndef __WG_DGNALL_H__
#define __WG_DGNALL_H__

#ifdef _AFXEXT
#define  __INTER_DLL__  // DLL 에서 다른 DLL의 Member에 대한 참조가 발생할 때 
#endif

// 현재 DLL 에서 다른 DLL을 참조 할때 아래 예와 같이 참조하는  DLL의  
// Interface Header File을 Include 한다.  "WGen DLL 템플릿 생성"  App Wizard에서 
// 생성된 DLL의 Interface Header File은 suffix "All"을 Header File 이름에 갖는다. 

//#if !defined(__WG_BASEALL_H__)
//	#include "..\wg_base\wg_baseall.h"
//#endif

#if !defined(__WG_DGNVER_H__)
	#include "wg_dgnver.h"
#endif

#if !defined(__WG_DGN_H__)
	#include "wg_dgn.h"
#endif

#if !defined(__DGNEXPORTFUNC_H__)
	#include "DgnExportFunc.h"
#endif

// wg_dgn.dll에 추가되는 class 에 대한 header file을 추가한다. 
//제시된 예는 추가된 Class Header를 Include 하는 모습이다. 
//Export가 필요없는 Class 또는 Function은 Include할 필요 없다. 
//자세한 내용은 AddClass.txt를 참고 할것 
//#if !defined(__ADDEDCLASS_H__)
//#include "AddedClass.h"
//#endif

#if !defined(__XLADVANCE_H__)
#include "XlAdvance.h"
#endif

#if !defined(__XLBASE_H__)
#include "XlBase.h"
#endif

#if !defined(__EXCEL9_H__)
#include "Excel9.h"
#endif

#if !defined(__DGNSTRUCT_H__)
#include "DgnStruct.h"
#endif

#if !defined(__DGNDATACTRL_H__)
#include "DgnDataCtrl.h"
#endif

#if !defined(__DGNFORCECTRL_H__)
#include "DgnForceCtrl.h"
#endif

#if !defined(__DGNMAKEINPUTDATA_H__)
#include "DgnMakeInputData.h"
#endif

#if !defined(__DGNCFCODEDLG_H__)
#include "DgnCfCodeDlg.h"
#endif

#if !defined(__DGNCFPFDGDLG_H__)
#include "DgnCfPfdgDlg.h"
#endif

#if !defined(__DGNSTLCODEDLG_H__)
#include "DgnStlCodeDlg.h"
#endif

#include "DgnStlPfdgDlg.h"

#if !defined(__DGNSTLCODEKRDLG_H__)
#include "DgnStlCodeKRDlg.h"
#endif

#if !defined(__DGNSTLCODEJPDLG_H__)
#include "DgnStlCodeJPDlg.h"
#endif

#if !defined(__DGNCONCODEDLG_H__)
#include "DgnConCodeDlg.h"
#endif

#if !defined(__DGN_CON_CODE_NEW_DLG_H__)
#include "DgnConCodeNewDlg.h"
#endif

#if !defined(__DGNSRCCODEDLG_H__)
#include "DgnSrcCodeDlg.h"
#endif

#if !defined(__DGNCFMATDLG_H__)
#include "DgnCfMatDlg.h"
#endif

#if !defined(__DGNSTLMATDLG_H__)
#include "DgnStlMatDlg.h"
#endif

#if !defined(__DGNSTLSECTDLG_H__)
#include "DgnStlSectDlg.h"
#endif

#if !defined(__DgnSodMatDlg_H__)
#include "DgnSodMatDlg.h"
#endif 

#if !defined(__CONCMATDLG_H__)
#include "DgnConMatDlg.h"
#endif

#if !defined(__DGNSRCMATDLG_H__)
#include "DgnSrcMatDlg.h"
#endif

#if !defined(__DGNTOWERMATDLG_H__)
#include "DgnTowerMatDlg.h"
#endif

#if !defined(__DGNCONBARDLG_H__)
#include "DgnConBarDlg.h"
#endif

#if !defined(__DGNMESHBARDLG_H__)
#include "DgnMeshBarDlg.h"
#endif

#if !defined(__DGNCONLOSSFACTORDLG_H__)
#include "DgnConLossFactorDlg.h"
#endif

#if !defined(__DGNCONPSLCOM_H__)
#include "DgnConPSLCOM.h"
#endif

#if !defined(__CONPSCMATDLG_H__)
#include "DgnConPSCMatDlg.h"
#endif

#if !defined(__CONJSFTDLG_H__)
#include "DgnConPSASDlg.h"
#endif

#if !defined(__CONPSASDLG_H__)
#include "DgnConPSASDlg.h"
#endif

#if !defined(__GREDUCTIONDLG_H__)
#include "DgnGenLrdfDlg.h"
#endif

#if !defined(__DGNGENFRAMEDLG_H__)
#include "DgnGenFrameDlg.h"
#endif

#if !defined(__DGNCONBARRATIODLG_H__)
#include "DgnConBarRatioDlg.h"
#endif

#if !defined(__DGNCFSECTDLG_H__)
#include "DgnCfSectDlg.h"
#endif

#if !defined(__DGNCFPHIDLG_H__)
#include "DgnCfPhiDlg.h"
#endif

#if !defined(__DGNSTLPHIDLG_H__)
#include "DgnStlPhiDlg.h"
#endif

#if !defined(__DGNCONPHIDLG_H__)
#include "DgnConPhiDlg.h"
#endif

#if !defined(__DGNPROGRESSDLG_H__)
#include "DgnProgressDlg.h"
#endif

#if !defined(__DGNCONMRFTDLG_H__)
#include "DgnConMRFTDlg.h"
#endif

#if !defined(__DGNCONMCMBDLG_H__)
#include "DgnConMCMBDlg.h"
#endif

#if !defined(__DGNGENSTERMLOADDLG_H__)
#include "DgnGenSTermLoadDlg.h"
#endif

#if !defined(__DGNCRCDGNPARAMETERDLG_H__)
#include "DgnCRCDgnParameterDlg.h"
#endif

#if !defined(__CRCFILEBASE_H__)
#include "CRCFileBase.h"
#endif

#if !defined(__CRCEXPORTFILE_H__)
#include "CRCExportFile.h"
#endif

#if !defined(__CRCIMPORTFILE_H__)
#include "CRCImportFile.h"
#endif

#if !defined(__CRCDGNOPTION_H__)
#include "CRCDgnOption.h"
#endif

#if !defined(__CRCDGNLCOMDLG_H__)
#include "CRCDgnLcomDlg.h"
#endif

#if !defined(__CRCDATACTRL_H__)
#include "CRCDataCtrl.h"
#endif

#if !defined(__CRCFORCECTRL_H__)
#include "CRCForceCtrl.h"
#endif

#if !defined(__DGNCONBEAMREINFORCEDLGCIVIL_H__)
#include "DgnConBeamReinforceDlgCIVIL.h"
#endif

#if !defined(__DGNCONCOLMREINFORCEDLGCIVIL_H__)
#include "DgnconColmReinforceDlgCIVIL.h"
#endif

#if !defined(__DGNCONBEAMREINDESDLGCIVIL_H__)
#include "DgnConBeamReinDesDlgCIVIL.h"
#endif

#if !defined(_DGNCONCOLMREINDESDLGCIVIL_H__)
#include "DgnConColmReinDesDlgCIVIL.h"
#endif

#if !defined(__DGNGENSEISFACTOR_H__)
#include "DgnGenSeisFactor.h"
#endif

#if !defined(__DGNIMPORTFORCEDLG_H__)
#include "DgnImportForceDlg.h"
#endif

#if !defined(__RCDESIGNPARAMDLG_H__)
#include "RCDesignParamDlg.h"
#endif

#if !defined(__RCDESIGNPLATEPARAMDLG_H__)
#include "RCDesignPlateParamDlg.h"
#endif

#if !defined(__STEELKRDESIGNPARAMDLG_H__)
#include "SteelKRDesignParamDlg.h"
#endif

#if !defined(__ONCSECTIONREBARVIEWDLG_H__)
#include "oncSectionRebarViewDlg.h"
#endif

#if !defined(__CONCSECTIONBEAMREBARVIEWDLG_H__)
#include "ConcSectionBeamRebarViewDlg.h"
#endif

#if !defined(__RATINGCASEDLG_H__)
#include "RatingCaseDlg.h"
#endif

#if !defined(__RATINGPARAMDLG_H__)
#include "RatingParamDlg.h"
#endif

#if !defined(__RATINGGROUPSETTINGDLG_H__)
#include "RatingGroupSettingDlg.h"
#endif

#if !defined(__DGNSTLRATINGGROUPSETTINGDLG_H__)
#include "DgnStlRatingGroupSettingDlg.h"
#endif

#if !defined(__DGNRCDESIGNPARAMBASEDLG_H__)
#include "DgnRCDesignParamBaseDlg.h"
#endif

#if !defined(__DGNGENLCOMSERVTYPEDLG_H__)
#include "DgnGenLcomServTypeDlg.h"
#endif

#if !defined(__DGNCONBARSECTDLG_H__)
#include "DgnConBarSectDlg.h"
#endif

#if !defined(__CPGTRANSTIFENDSUPPORTDLG_H__)
#include "CPGTranStifEndSupportDlg.h"
#endif

#if !defined(__CPGTRANSTIFENDSUPPORT_IRC_DLG_H__)
#include "CPGTranStifEndSupport_IRC_Dlg.h"
#endif

#if !defined(__DGNCONCOLMREINFORCEREBARDLGCIVIL_H__)
#include "DgnConColmReinforceRebarDlgCIVIL.h"
#endif

#if !defined(__DGNREBARDLG_H__)
#include "DgnRebarDlg.h"
#endif

#if !defined(__DGNREBARCPGDLG_H__)
#include "DgnRebarCpgDlg.h"
#endif

#if !defined(__CPGPARAMDLG_H__)
#include "CPGParamDlg.h"
#endif

#if !defined(AFX_DGNSTLKRPARAMTABLEDLG_H__)
#include "DgnStlKRParamTableDlg.h"
#endif

#if !defined(AFX_DGNSTLSPCSECTDLG_H__)
#include "DgnStlSPCSectDlg.h"
#endif

#if !defined(__DGNRATINGCODEDLG_H__)
#include "DgnRatingCodeDlg.h"
#endif

#if !defined(__DGNRATINGSTLCODEDLG_H__)
#include "DgnRatingStlCodeDlg.h"
#endif

#if !defined(__RATINGPARAMKRDLG_H__)
#include "RatingParamKRDlg.h"
#endif

#if !defined(__RATINGCASEKRDLG_H__)
#include "RatingCaseKRDlg.h"
#endif

#if !defined(__DgnSlabSeisLoadCombTypeDlg_H__)
#include "DgnSlabSeisLoadCombTypeDlg.h"
#endif

//(2008.03.31) Add by Unsang :: XL_Print
/*#if !defined(AFX_DGNSAVEREPORTOEXCELDLG_H__)
#include "DgnSaveReportoExcelDlg.h"
#endif*/

//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
/*
//(2009.06.12) Add by Unsang :: Only National Annex by EC2-2:05..
#if !defined(__DGNCONCVLMATLLIMIT_H__)
#include "DgnConCvlMatlLimit.h"
#endif

#if !defined(__DGNCONCVLRBARLIMIT_H__)
#include "DgnConCvlRbarLimit.h"
#endif
*/

#ifdef _AFXEXT
#undef  __INTER_DLL__
#endif

#endif 
