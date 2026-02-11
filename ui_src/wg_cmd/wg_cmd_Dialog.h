#ifndef __WG_CMD_DIALOG_H__
#define __WG_CMD_DIALOG_H__

#ifdef _AFXEXT
#define  __INTER_DLL__  // DLL 에서 다른 DLL의 Member에 대한 참조가 발생할 때 
#endif

// 현재 DLL 에서 다른 DLL을 참조 할때 아래 예와 같이 참조하는  DLL의  
// Interface Header File을 Include 한다.  "WGen DLL 템플릿 생성"  App Wizard에서 
// 생성된 DLL의 Interface Header File은 suffix "All"을 Header File 이름에 갖는다. 

//#if !defined(__WG_BASEALL_H__)
//	#include "..\wg_base\wg_baseall.h"
//#endif

#if !defined(__WG_CMDVER_H__)
	#include "wg_cmdver.h"
#endif

#if !defined(__WG_CMD_H__)
	#include "wg_cmd.h"
#endif

// wg_cmd.dll에 추가되는 class 에 대한 header file을 추가한다. 
//제시된 예는 추가된 Class Header를 Include 하는 모습이다. 
//Export가 필요없는 Class 또는 Function은 Include할 필요 없다. 
//자세한 내용은 AddClass.txt를 참고 할것 
//#if !defined(__ADDEDCLASS_H__)
//#include "AddedClass.h"
//#endif

#if !defined(__MPHIPARAMETERDLG_H__)
#include "MPhiParameterDlg.h"
#endif

#if !defined(__CMD_MPHIPARAMETERDLG_JP_H__)
#include "MPhiParameterDlg_JP.h"
#endif

#if !defined(__ETCPRJCONF_H__)
#include "EtcPrjConf.h"
#endif

#if !defined(__ETCSTYPDLG_H__)
#include "EtcStypDlg.h"
#endif

#if !defined(__ETCSTYPDLG_MEC_H__)
#include "EtcStypDlg_MEC.h"
#endif

#if !defined(__ETCACTLDLG_H__)
#include "EtcActlDlg.h"
#endif

#if !defined(__ETCACTLDLG_MEC_H__)
#include "EtcActlDlg_MEC.h"
#endif

#if !defined(__CMPDELDLG_H__)
#include "CMPdelDlg.h"
#endif

#if !defined(__CMBUCKDLG_H__)
#include "CMBuckDlg.h"
#endif

#if !defined(__BLDCDLG_H__)
#include "BldcDlg.h"
#endif

#if !defined(__AUTOWIDDLG_H__)
#include "AutoWidDlg.h"
#endif

#if !defined(__CMSTORDLG_H__)
#include "CMStorDlg.h"
#endif

#if !defined(__EIGVDLG_H__)
#include "EigvDlg.h"
#endif

#if !defined(__EIGVDLG_MEC_H__)
#include "EigvDlg_MEC.h"
#endif

#if !defined(__SPFCLISTDLG_H__)
#include "SpfcListDlg.h"
#endif

#if !defined(__THFCLISTDLG_H__)
#include "ThfcListDlg.h"
#endif

#if !defined(__THISDLG_H__)
#include "ThisDlg.h"
#endif

#if !defined(__THISLISTDLG_H__)
#include "ThisListDlg.h"
#endif

#if !defined(__CMThgcDlg_H__)
#include "CMThgcDlg.h"
#endif

#if !defined(__CMThgcDlg_MEC_H__)
#include "CMThgcDlg_MEC.h"
#endif

#if !defined(__IETHLISTDLG_H__)
#include "IethListDlg.h"
#endif

#if !defined(__IEHPLISTDLG_H__)
#include "IehpListDlg.h"
#endif

#if !defined(__CMFBLDDLG_H__)
#include "CMFbldDlg.h"
#endif

#if !defined(__ETCSPCTDLG_H__)
#include "EtcSpctDlg.h"
#endif

#if !defined(__CMETCPJST_H__)
#include "CMEtcPjst.h"
#endif

#if !defined(__CMLTOMDLG_H__)
#include "CMLtomDlg.h"
#endif

#if !defined(__ETCPZEFDLG_H__)
#include "EtcPzefDlg.h"
#endif

#if !defined(__WINDLISTDLG_H__)
#include "WindListDlg.h"
#endif

#if !defined(__SEISLISTDLG_H__)
#include "SeisListDlg.h"
#endif

/* 여기 추가 하면 빌드 미친듯이 하게 됨.. 여기다가 추가 하지 마세요... 나중에 이 헤더 파일 지울 예정
#if !defined(__CMMVCTDLG_H__)
#include "CMMvctDlg.h"
#endif

#if !defined(__CMMVCTCHDLG_H__)
#include "CMMvctCHDlg.h"
#endif

#if !defined(__CMMVCTJPDLG_H__)
#include "CMMvctJPDlg.h"
#endif

#if !defined(__CMMVCTIDDLG_H__)
#include "CMMvctIDDlg.h"
#endif

#if !defined(__CMMVCTBSDLG_H__)
#include "CMMvctBSDlg.h"
#endif

#if !defined (__CMLLANDLG_H__)
#include "CMLlanDlg.h"
#endif

#if !defined (__CMMVHLLISTDLG_H__)
#include "CMMvhlListDlg.h"
#endif

#if !defined (__CMMVHCLISTDLG_H__)
#include "CMMvhcListDlg.h"
#endif

#if !defined (__CMMVLDLISTDLG_H__)
#include "CMMvldListDlg.h"
#endif

#if !defined (__CMMVLDLISTCHDLG_H__)
#include "CMMvldListCHDlg.h"
#endif

#if !defined(__CMMVLDLISTJPDLG_H__)
#include "CMMvldListJPDlg.h"
#endif

#if !defined(__MVLDITEMJPDLG_H__)
#include "CMMvldItemJPDlg.h"
#endif
*/
#if !defined (__CMENVLLISTDLG_H__)
#include "CMEnvlListDlg.h"
#endif

#if !defined (__CMSLANDLG_H__)
#include "CMSlanDlg.h"
#endif

#if !defined (__CMSLANCHDLG_H__)
#include "CMSlanCHDlg.h"
#endif

#if !defined (__CMSLANOPDLG_H__)
#include "CMSlanOpDlg.h"
#endif

#if !defined (__CMSLANFRDLG_H__)
#include "CMSlanFRDlg.h"
#endif

#if !defined(__CMANALYSISOPTIONDLG_H__)
#include "CMAnalysisOptionDlg.h"
#endif

#if !defined(__CMANALYSISOPTIONDLG_MEC_H__)
#include "CMAnalysisOptionDlg_MEC.h"
#endif

#if !defined(__CMLOADPLCBDLG_H__)
#include "CMLoadPLCBDlg.h"
#endif

#if !defined(__CMBNDRSPRTDLG_H__)
#include "CMBndrSprtDlg.h"
#endif

#if !defined(__CMLOADESEQDLG_H__)
#include "CMLoadESEQDlg.h"
#endif

#if !defined(__CMLOADMLLCDLG_H__)
#include "CMLoadMLLCDlg.h"
#endif

#if !defined(__CMDCSG2SLOAD_H__)
#include "CMDcsG2SLoad.h"
#endif

#if !defined(__CMDCSG2SMODEL_H__)
#include "CMDcsG2SModel.h"
#endif

#if !defined(__CMDCSS2GREACT_H__)
#include "CMDcsS2GReact.h"
#endif

#if !defined(__MINITFORCECTRLDLG_H__)
#include "MInitForceCtrlDlg.h"
#endif

#if !defined(__CMDCSG2SWIN_H__)
#include "CMDcsG2SWin.h"
#endif

#if !defined(__CMHYDRATIONANALYDLG_H__)
#include "CMHydrationAnalyDlg.h"
#endif

#if !defined(__CMHYDRATIONANALYDLG_MEC_H__)
#include "CMHydrationAnalyDlg_MEC.h"
#endif

#if !defined(__CMMATPAGEITEMDLG_H__)
#include "CMMatPageItemDlg.h"
#endif

#if !defined(__CMTHKPAGEITEMDLG_H__)
#include "CMThkPageItemDlg.h"
#endif

#if !defined(__CMSECTITEMDLG_H__)
#include "CMSectItemDlg.h"
#endif

#if !defined(__CMHYDRCOOLELEMITEM_H__)
#include "CMHydrCoolElemItem.h"
#endif

#if !defined(__CMDCSS2GLOAD_H__)
#include "CMDcsS2GLoad.h"
#endif

#if !defined(__CHMTPLISTDLG_H__)
#include "HmtpListDlg.h"
#endif

#if !defined(__CETFCLISTDLG_H__)
#include "EtfcListDlg.h"
#endif

#if !defined(__CHSFCLISTDLG_H__)
#include "HsfcListDlg.h"
#endif

#if !defined(__CCCFCLISTDLG_H__)
#include "CcfcListDlg.h"
#endif

#if !defined(__CMHCRPDLG_H__) //  Creep/Shrinkage 
#include "CMHcrpDlg.h"
#endif

#if !defined(__CMLOADCASEDLG_H__)
#include "CMLoadCaseDlg.h"
#endif
/*
#if !defined(__WINDITEMDLG_H__)
#include "WindItemDlg.h"
#endif
*/
#if !defined(__CMTENDONDEFDLG_H__)
#include "CMTendonDefDlg.h"
#endif

/* Old
#if !defined(__SEISITEMDLG2_H__)
#include "SeisItemDlg2.h"
#endif
*/
#if !defined(__SEISITEMDLGNEW_H__)
#include "SeisItemDlgNew.h"
#endif

#if !defined(__SPFCITEMDLG_H__)
#include "SpfcItemDlg.h"
#endif

#if !defined(__THFCITEMDLG_H__)
#include "ThfcItemDlg.h"
#endif

#if !defined(__THISITEMDLG_H__)
#include "ThisItemDlg.h"
#endif

#if !defined(__THISITEMDLG_MEC_H__)
#include "ThisItemDlg_MEC.h"
#endif

#if !defined(__CMLLANDLG_H__)
#include "CMLlanDlg.h"
#endif

#if !defined(__CMLLANOPDLG_H__)
#include "CMLlanOpDlg.h"
#endif
/* 여기 추가 하면 빌드 미친듯이 하게 됨.. 여기다가 추가 하지 마세요... 나중에 이 헤더 파일 지울 예정
#if !defined(__CMMVHLITEMSTDDLG_H__)
#include "CMMvhlItemStdDlg.h"
#endif

#if !defined(__CMMVHLITEMUSRDLG_H__)
#include "CMMvhlItemUsrDlg.h"
#endif

#if !defined(__CMMVHCITEMDLG_H__)
#include "CMMvhcItemDlg.h"
#endif

#if !defined(__CMMVLDITEMDLG_H__)
#include "CMMvldItemDlg.h"
#endif

#if !defined(__CMMVLDITEMCHDLG_H__)
#include "CMMvldItemCHDlg.h"
#endif
*/
#if !defined(__CMSECTSCALEFACTORDLG_H__)
#include "CMSectScaleFactorDlg.h"
#endif

#if !defined(__CCFCITEMDLG_H__)
#include "CcfcItemDlg.h"
#endif

#if !defined(__ETFCITEMDLG_H__)
#include "EtfcItemDlg.h"
#endif

#if !defined(__HMTPITEMDLG_H__)
#include "HmtpItemDlg.h"
#endif

#if !defined(__HSFCITEMDLG_H__)
#include "HsfcItemDlg.h"
#endif

#if !defined(__CMNLACDLG_H__)
#include "CMNlacDlg.h"
#endif

#if !defined(__CMNLACDLG_MEC_H__)
#include "CMNlacDlg_MEC.h"
#endif

#if !defined(__CMNLASEQDLG_H__)
#include "CMNlaSeqDlg.h"
#endif

/*
#if !defined(__CMLOADSTDGWIZARDDLG_H__)
#include "CMLoadSTDGWizardDlg.h"
#endif
*/

#if !defined(__CMLOADSTDGWIZARDDLGNEW_H__)
#include "CMLoadSTDGWizardDlgNew.h"
#endif


#if !defined(__NLLINKPRTLISTDLG_H__)
#include "NLLinkPrtListDlg.h"
#endif

#if !defined(__CMSTAGECOMPSECTLISTDLG_H__)
#include "CMStageCompSectListDlg.h"
#endif

#if !defined(__CMELSPLSMATLADDDLG_H__)
#include "CMElsPlsMatlAddDlg.h"
#endif

#if !defined(__CMTENDONPROFILEDLG_H__)
#include "CMTendonProfileDlg.h"
#endif

#if !defined(__IEHPITEMDLG_H__)
#include "IehpItemDlg.h"
#endif

#if !defined(__IEHPITEMDLG_MEC_H__)
#include "IehpItemDlg_MEC.h"
#endif

#if !defined(__CMLLANIDDLG_H__ )
#include "CMLlanIDDlg.h"
#endif
/* 여기 추가 하면 빌드 미친듯이 하게 됨.. 여기다가 추가 하지 마세요... 나중에 이 헤더 파일 지울 예정
#if !defined(__CMMVLDITEMIDDLG_H__ )
#include "CMMvldItemIDDlg.h"
#endif

#if !defined(__CMMVLDLISTIDDLG_H__ )
#include "CMMvldListIDDlg.h"
#endif

#if !defined(__CMMVLDITEMBSDLG_H__ )
#include "CMMvldItemBSDlg.h"
#endif

#if !defined(__CMMVLDLISTBSDLG_H__ )
#include "CMMvldListBSDlg.h"
#endif

#if !defined(__CMMVCDDLG_H__ )
#include "CMMvcdDlg.h"
#endif
*/
#if !defined(__CMWINDITEMDLG_H__ )
#include "CMWindItemDlg.h"
#endif

#if !defined(__CMGROUPDAMPINGDLG_H__ )
#include "CMGroupDampingDlg.h"
#endif

#if !defined(__CMGROUPDAMPINGELEMENTDLG_H__ )
#include "CMGroupDampingElementDlg.h"
#endif

#if !defined(__CMGROUPDAMPINGSTRAINDLG_H__ )
#include "CMGroupDampingStrainDlg.h"
#endif

#if !defined(__CMGRIDMAINGIRDERLISTDLG_H__)
#include "CMGridMainGirderListDlg.h"
#endif

//#if !defined(__CMCROSSBEAMDLG_H__ )
//#include "CMCrossBeamDlg.h"
//#endif
#if !defined(__CMGRIDCROSSBEAMLISTDLG_H__)
#include "CMGridCrossBeamListDlg.h"
#endif

#if !defined(__CMLOADLINELISTDLG_H__ )
#include "CMLoadLineListDlg.h"
#endif

#if !defined(__CMGRIDCROSSBEAMITEMDLG_H__ )
#include "CMGridCrossBeamItemDlg.h"
#endif

#if !defined(__CMGridAnalCtrlDlg_H__ )
#include "CMGridAnalCtrlDlg.h"
#endif

#if !defined(__CMGRIDMAINGIRDERITEMDLG_H__ )
#include "CMGridMainGirderItemDlg.h"
#endif

#if !defined(__CMLOADLINEITEMDLG_H__ )
#include "CMLoadLineItemDlg.h"
#endif

#if !defined(__CMRESULTLCTBDLG_H__ )
#include "CMResultLctbDlg.h"
#endif

#if !defined(__CMREBARPSCDLG_H__)
#include "CMRebarPscDlg.h"
#endif

#if !defined(__CMREBARDLG_H__) // added by mylee
#include "CMRebarDlg.h"
#endif

#if !defined(__CMBRIDGETYPEDLG_H__)
#include "CMBridgeTypeDlg.h"
#endif

#if !defined(__CMGILDCLIVEDLG_H__)
#include "CMGildCLiveDlg.h"
#endif

#if !defined(__CMGILDDEADDLG_H__)
#include "CMGildDeadDlg.h"
#endif

#if !defined(__CMGILDOLIVEDLG_H__)
#include "CMGildOLiveDlg.h"
#endif

#if !defined(__CMGILDVLIVEDLG_H__)
#include "CMGildVLiveDlg.h"
#endif

#if !defined(__CMGRIDP1LOADFACTORTYPEDLG_H__)
#include "CMGridP1LoadFactorTypeDlg.h"
#endif

#if !defined(__CMTHATDLG_H__)
#include "CMThatDlg.h"
#endif

#if !defined(__CMEXLDDLG_H__)
#include "ExtTypeLoadDlg.h"
#endif 

#if !defined(__CMEXTYPEFORPRETENDLG_H__)
#include "CMExTypeForPretenDlg.h"
#endif

#if !defined(__CMSTAGECOMPSECTITEMDLG_H__)
#include "CMStageCompSectItemDlg.h"
#endif

#if !defined(AFX_NLLINKPRTITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_)
#include "NLLinkPrtItemDlg.h"
#endif AFX_NLLINKPRTITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_

#include "CMTimeDefMatTypeDlg.h"
#include "CMTimeDepMatFuncDlg.h"
#include "TdmeListDlg.h"
#include "CMElsPlsMatlDlg.h"
#include "CMLlaneJpDlg.h"
#include "CMLlanCHDlg.h"

#if !defined(__CMDgnAnalOptionDlg_H__)
#include "CMDgnAnalOptionDlg.h"
#endif

#if !defined(__CMSlabDgnControlDlg_H__)
#include "CMSlabDgnControlDlg.h"
#endif

#if !defined(__CMSECSODSTIFFENERDLG_H__)
#include "CMSectSODStiffenerDlg.h"
#endif

#ifdef _AFXEXT
#undef  __INTER_DLL__
#endif

#endif 

