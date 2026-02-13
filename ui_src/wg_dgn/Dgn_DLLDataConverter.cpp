// Dgn_DLLDataConverter.cpp: implementation of the Dgn_DLLDataConverter namespace.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_DLLDataConverter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace Dgn_DLLDataConverter
{
	ConvertFysg_EngineToCVL(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_BASE& rData)
	{
		rData.bCHK   = InData.bCHK ;
		rData.StagK  = InData.StagK;
		rData.dFT    = InData.dFT  ;
		rData.dFB    = InData.dFB  ;
		rData.dFTL   = InData.dFTL ;
		rData.dFBL   = InData.dFBL ;
		rData.dFTR   = InData.dFTR ;
		rData.dFBR   = InData.dFBR ;
		rData.dFMAX  = InData.dFMAX;
		rData.dALW   = InData.dALW ;
		rData.bOK    = InData.bOK  ;

		return TRUE;
	}

	ConvertFypc_EngineToCVL(const _FYPC_AASHTO& InData, T_FYPC_AASHTO& rData)
	{
		rData.bCHK   = InData.bCHK  ;
		rData.dFDL1  = InData.dFDL1 ;
		rData.dFDL2  = InData.dFDL2 ;
		rData.dFDL3  = InData.dFDL3 ;
		rData.dFLL1  = InData.dFLL1 ;
		rData.dFLL2  = InData.dFLL2 ;
		rData.dAFDL1 = InData.dAFDL1;
		rData.dAFDL2 = InData.dAFDL2;
		rData.dAFDL3 = InData.dAFDL3;
		rData.dAFLL1 = InData.dAFLL1;
		rData.dAFLL2 = InData.dAFLL2;
		rData.bOK    = InData.bOK   ;

		return TRUE;
	}

	ConvertFycm_EngineToCVL(const _FYCM_PSC_LRFD_BASE& InData, T_FYCM_AASHTO_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.dFT   = InData.dFT  ;
		rData.dFB   = InData.dFB  ;
		rData.dFTL  = InData.dFTL ;
		rData.dFBL  = InData.dFBL ;
		rData.dFTR  = InData.dFTR ;
		rData.dFBR  = InData.dFBR ;
		rData.dFMAX = InData.dFMAX;
		rData.dALW  = InData.dALW ;
		rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertFssg_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMT_JTG_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		rData.iMax  = InData.iMax ;    
		rData.dFP1  = InData.dFP1 ;
		rData.dFP2  = InData.dFP2 ;
		rData.dFP3  = InData.dFP3 ;
		rData.dFP4  = InData.dFP4 ;
		rData.dFP5  = InData.dFP5 ;
		rData.dFP6  = InData.dFP6 ;
		rData.dFP7  = InData.dFP7 ;
		rData.dFP8  = InData.dFP8 ;
		rData.dFP9  = InData.dFP9 ;
		rData.dFP10 = InData.dFP10;
		rData.dFMAX = InData.dFMAX;
		rData.dAFP  = InData.dAFP ;
		rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertFpms_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMS_JTG_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		rData.iMax  = InData.iMax ;    
		rData.dFP1  = InData.dFP1 ;
		rData.dFP2  = InData.dFP2 ;
		rData.dFP3  = InData.dFP3 ;
		rData.dFP4  = InData.dFP4 ;
		rData.dFP5  = InData.dFP5 ;
		rData.dFP6  = InData.dFP6 ;
		rData.dFP7  = InData.dFP7 ;
		rData.dFP8  = InData.dFP8 ;
		rData.dFP9  = InData.dFP9 ;
		rData.dFP10 = InData.dFP10;
		rData.dFMAX = InData.dFMAX;
		rData.dAFP  = InData.dAFP ;
		rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertBdcw_EngineToCVL(const _BDCW_PSC_LRFD_BASE& InData, T_BDCW_AASHTO_BASE& rData)
	{
		rData.bCHK   = InData.LInf.bCHK  ;
		rData.LcomK  = InData.LInf.LcomK ;  
		rData.dFT    = InData.ForB.dFT   ;
		rData.dFB    = InData.ForB.dFB   ;
		rData.dFSS   = InData.dFSS  ;
		rData.ds_use = InData.ds_use;
		rData.ds_max = InData.ds_max;
		rData.bOK    = InData.bOK   ;

		return TRUE;
	}

	ConvertTbar_EngineToCVL(const _TBAR_PSC_LRFD_BASE& InData, T_TBAR_AASHTO_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		rData.dREQ  = InData.dREQ ;
		rData.dMIN  = InData.dMIN ;
		rData.dUSE  = InData.dUSE ;
		rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertBdcy_EngineToCVL(const _BDCY_PSC_LRFD_BASE& InData, T_BDCY_AASHTO_BASE& rData)
	{
		rData.bCHK    = InData.bCHK;
		rData.dMuy    = InData.dMuy;
		rData.drMuy   = InData.drMuy;
		rData.dMny    = InData.dMny;
		rData.dMcry   = InData.dMcry;
		//rData.dFacM   = InData.dFacM;
		//rData.dFacMcr = InData.dFacMcr;
		rData.bOK     = InData.bOK;

		return TRUE;
	}

	ConvertCums_EngineToCVL(const _CUMS_AASHTO_BASE& InData, T_CUMS_AASHTO_BASE& rData)
	{
		rData.bCHK      = InData.bCHK     ;
		rData.LcomK     = InData.LcomK    ;
		rData.dVu       = InData.dVu      ;
		rData.dMu       = InData.dMu      ;
		rData.dVn       = InData.dVn      ;
		rData.dPhi      = InData.dPhi     ;
		rData.dPhiVn    = InData.dPhiVn   ;
		rData.dVc       = InData.dVc      ;
		rData.dVs       = InData.dVs      ;
		rData.dVp       = InData.dVp      ;
		rData.dde       = InData.dde      ;
		rData.ddv       = InData.ddv      ;
		rData.dex       = InData.dex      ;
		rData.dTheta    = InData.dTheta   ;
		rData.dBeta     = InData.dBeta    ;
		rData.dAv_s     = InData.dAv_s    ;
		rData.dA_st     = InData.dA_st    ;
		rData.dAl       = InData.dAl      ;
		rData.dbv       = InData.dbv      ;
		rData.dAv_s_min = InData.dAv_s_min;
		rData.dAv_s_req = InData.dAv_s_req;
		rData.dAl_min   = InData.dAl_min  ;
		rData.dbv_min   = InData.dbv_min  ;
		rData.bOK       = InData.bOK      ;
		rData.dVui_PscsD = InData.dVui_PscsD;
		rData.dVni_PscsD = InData.dVni_PscsD;
		rData.dVui_PscsD_S = InData.dVui_PscsD_S;
		rData.dVni_PscsD_S = InData.dVni_PscsD_S;

		rData.dVni_Conc_PscsD = InData.dVni_Conc_PscsD;
		rData.dVni_Fric_PscsD = InData.dVni_Fric_PscsD;
		rData.dVni_Conc_Rat_PscsD = InData.dVni_Conc_Rat_PscsD;
		rData.dVni_Fric_Rat_PscsD = InData.dVni_Fric_Rat_PscsD;

		rData.bOK_PscsD  = InData.bOK_PscsD ;

		return TRUE;
	}

	ConvertCrmt_EngineToCVL(const _CRMS_AASHTO_BASE& InData, T_CRMT_AASHTO_BASE& rData)
	{
		rData.bCHK      = InData.bCHK     ;
		rData.LcomK     = InData.LcomK    ;
		rData.dVu       = InData.dVu      ;
		rData.dMu       = InData.dMu      ;
		rData.dVn       = InData.dVn      ;
		rData.dTn       = InData.dTn      ;
		rData.dTu       = InData.dTu      ;
		rData.dPhi      = InData.dPhi     ;
		rData.dPhiVn    = InData.dPhiVn   ;
		rData.dPhi_t    = InData.dPhi_t   ;
		rData.dPhi_tTn  = InData.dPhi_tTn ;
		rData.dVc       = InData.dVc      ;
		rData.dVs       = InData.dVs      ;
		rData.dVp       = InData.dVp      ;
		rData.dde       = InData.dde      ;
		rData.ddv       = InData.ddv      ;
		rData.dex       = InData.dex      ;
		rData.dTheta    = InData.dTheta   ;
		rData.dBeta     = InData.dBeta    ;
		rData.dAv_s     = InData.dAv_s    ;
		rData.dA_st     = InData.dA_st    ;
		rData.dAl       = InData.dAl      ;
		rData.dbv       = InData.dbv      ;	
		rData.dAv_s_min = InData.dAv_s_min;
		rData.dAv_s_req = InData.dAv_s_req;
		rData.dAl_min   = InData.dAl_min  ;
		rData.dbv_min   = InData.dbv_min  ;	
		rData.bOK       = InData.bOK      ;

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////

	ConvertFysg_CivilToEngineAASHTO(const T_FYSG_BASE& InData, _FYSG_PSC_LRFD_BASE& rData)
	{
		rData.bCHK   = InData.bCHK ;
		rData.StagK  = InData.StagK;
		rData.dFT    = InData.dFT  ;
		rData.dFB    = InData.dFB  ;
		rData.dFTL   = InData.dFTL ;
		rData.dFBL   = InData.dFBL ;
		rData.dFTR   = InData.dFTR ;
		rData.dFBR   = InData.dFBR ;
		rData.dFMAX  = InData.dFMAX;
		// Check, in Engine
		//rData.dALW   = InData.dALW ; 
		//rData.bOK    = InData.bOK  ;

		return TRUE;
	}

	ConvertFypc_CivilToEngineAASHTO(const T_FYPC_AASHTO& InData,      _FYPC_AASHTO& rData)
	{
		rData.bCHK   = InData.bCHK  ;
		rData.dFDL1  = InData.dFDL1 ;
		rData.dFDL2  = InData.dFDL2 ;
		rData.dFDL3  = InData.dFDL3 ;
		rData.dFLL1  = InData.dFLL1 ;
		rData.dFLL2  = InData.dFLL2 ;
		// Check, in Engine
		//rData.dAFDL1 = InData.dAFDL1;
		//rData.dAFDL2 = InData.dAFDL2;
		//rData.dAFDL3 = InData.dAFDL3;
		//rData.dAFLL1 = InData.dAFLL1;
		//rData.dAFLL2 = InData.dAFLL2;
		//rData.bOK    = InData.bOK   ;

		return TRUE;
	}

	ConvertFycm_CivilToEngineAASHTO(const T_FYCM_AASHTO_BASE& InData, _FYCM_PSC_LRFD_BASE& rData)
	{  
		rData.LcomK = InData.LcomK;
		rData.bCHK  = InData.bCHK ;
		rData.dFT   = InData.dFT  ;
		rData.dFB   = InData.dFB  ;
		rData.dFTL  = InData.dFTL ;
		rData.dFBL  = InData.dFBL ;
		rData.dFTR  = InData.dFTR ;
		rData.dFBR  = InData.dFBR ;
		rData.dFMAX = InData.dFMAX;
		// Check, in Engine
		//rData.dALW  = InData.dALW ;
		//rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertFssg_CivilToEngineAASHTO(const T_FPMT_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		rData.iMax  = InData.iMax ;    
		rData.dFP1  = InData.dFP1 ;
		rData.dFP2  = InData.dFP2 ;
		rData.dFP3  = InData.dFP3 ;
		rData.dFP4  = InData.dFP4 ;
		rData.dFP5  = InData.dFP5 ;
		rData.dFP6  = InData.dFP6 ;
		rData.dFP7  = InData.dFP7 ;
		rData.dFP8  = InData.dFP8 ;
		rData.dFP9  = InData.dFP9 ;
		rData.dFP10 = InData.dFP10;
		rData.dFMAX = InData.dFMAX;
		// Check, in Engine
		//rData.dAFP  = InData.dAFP ;
		//rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertFpms_CivilToEngineAASHTO(const T_FPMS_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		rData.iMax  = InData.iMax ;    
		rData.dFP1  = InData.dFP1 ;
		rData.dFP2  = InData.dFP2 ;
		rData.dFP3  = InData.dFP3 ;
		rData.dFP4  = InData.dFP4 ;
		rData.dFP5  = InData.dFP5 ;
		rData.dFP6  = InData.dFP6 ;
		rData.dFP7  = InData.dFP7 ;
		rData.dFP8  = InData.dFP8 ;
		rData.dFP9  = InData.dFP9 ;
		rData.dFP10 = InData.dFP10;
		rData.dFMAX = InData.dFMAX;
		// Check, in Engine
		//rData.dAFP  = InData.dAFP ;
		//rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertBdcw_CivilToEngineAASHTO(const T_BDCW_AASHTO_BASE& InData, _BDCW_PSC_LRFD_BASE& rData)
	{
		rData.LInf.bCHK   = InData.bCHK  ;
		rData.LInf.LcomK  = InData.LcomK ;  
		rData.ForB.dFT    = InData.dFT   ;
		rData.ForB.dFB    = InData.dFB   ;
		// Check, in Engine
		//rData.dFSS   = InData.dFSS  ;
		//rData.ds_use = InData.ds_use;
		//rData.ds_max = InData.ds_max;
		//rData.bOK    = InData.bOK   ;

		return TRUE;
	}

	ConvertTbar_CivilToEngineAASHTO(const T_TBAR_AASHTO_BASE& InData, _TBAR_PSC_LRFD_BASE& rData)
	{
		rData.bCHK  = InData.bCHK ;
		rData.LcomK = InData.LcomK;
		// Check, in Engine
		//rData.dREQ  = InData.dREQ ;
		//rData.dMIN  = InData.dMIN ;
		//rData.dUSE  = InData.dUSE ;
		//rData.bOK   = InData.bOK  ;

		return TRUE;
	}

	ConvertBdcy_CivilToEngineAASHTO(const T_BDCY_AASHTO_BASE& InData, _BDCY_PSC_LRFD_BASE& rData)
	{
		rData.bCHK    = InData.bCHK;
		rData.dMuy    = InData.dMuy;
		rData.drMuy   = InData.drMuy;
		//rData.dMny    = InData.dMny;
		//rData.dMcry   = InData.dMcry;
		//rData.dFacM   = InData.dFacM;
		//rData.dFacMcr = InData.dFacMcr;
		//rData.bOK     = InData.bOK;

		return TRUE;
	}

	ConvertCums_CivilToEngineAASHTO(const T_CUMS_AASHTO_BASE& InData, _CUMS_AASHTO_BASE& rData)
	{
		rData.bCHK      = InData.bCHK     ;
		rData.LcomK     = InData.LcomK    ;
		rData.dVu       = InData.dVu      ;
		rData.dMu       = InData.dMu      ;
		// Check, in Engine
		/*
		rData.dVn       = InData.dVn      ;
		rData.dPhi      = InData.dPhi     ;
		rData.dPhiVn    = InData.dPhiVn   ;
		rData.dVc       = InData.dVc      ;
		rData.dVs       = InData.dVs      ;
		rData.dVp       = InData.dVp      ;
		rData.dde       = InData.dde      ;
		rData.ddv       = InData.ddv      ;
		rData.dex       = InData.dex      ;
		rData.dTheta    = InData.dTheta   ;
		rData.dBeta     = InData.dBeta    ;
		rData.dAv_s     = InData.dAv_s    ;
		rData.dA_st     = InData.dA_st    ;
		rData.dAl       = InData.dAl      ;
		rData.dbv       = InData.dbv      ;
		rData.dAv_s_min = InData.dAv_s_min;
		rData.dAv_s_req = InData.dAv_s_req;
		rData.dAl_min   = InData.dAl_min  ;
		rData.dbv_min   = InData.dbv_min  ;
		rData.bOK       = InData.bOK      ;
		*/

		return TRUE;
	}

	ConvertCrmt_CivilToEngineAASHTO(const T_CRMT_AASHTO_BASE& InData, _CRMS_AASHTO_BASE& rData)
	{
		rData.bCHK      = InData.bCHK     ;
		rData.LcomK     = InData.LcomK    ;
		rData.dVu       = InData.dVu      ;
		rData.dMu       = InData.dMu      ;
		rData.dVn       = InData.dVn      ;
		rData.dTu       = InData.dTu      ;
		// Check, in Engine
		/*
		rData.dTn       = InData.dTn      ;
		rData.dPhi      = InData.dPhi     ;
		rData.dPhiVn    = InData.dPhiVn   ;
		rData.dPhi_t    = InData.dPhi_t   ;
		rData.dPhi_tTn  = InData.dPhi_tTn ;
		rData.dVc       = InData.dVc      ;
		rData.dVs       = InData.dVs      ;
		rData.dVp       = InData.dVp      ;
		rData.dde       = InData.dde      ;
		rData.ddv       = InData.ddv      ;
		rData.dex       = InData.dex      ;
		rData.dTheta    = InData.dTheta   ;
		rData.dBeta     = InData.dBeta    ;
		rData.dAv_s     = InData.dAv_s    ;
		rData.dA_st     = InData.dA_st    ;
		rData.dAl       = InData.dAl      ;
		rData.dbv       = InData.dbv      ;	
		rData.dAv_s_min = InData.dAv_s_min;
		rData.dAv_s_req = InData.dAv_s_req;
		rData.dAl_min   = InData.dAl_min  ;
		rData.dbv_min   = InData.dbv_min  ;	
		rData.bOK       = InData.bOK      ;
		*/

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////

	ConvertFycm_ToEngineAASHTO(const T_FYCM_D& InData, T_FYCM_AASHTO& rData)
	{
		for(int i=0; i<4; i++)
		{
			rData.FycmBase[i].bCHK  = InData.FycmBase[i].bCHK ;
			rData.FycmBase[i].LcomK = InData.FycmBase[i].LcomK;
			rData.FycmBase[i].iMax  = InData.FycmBase[i].iMax ;
			rData.FycmBase[i].iKind = InData.FycmBase[i].iKind;
			rData.FycmBase[i].dFT   = InData.FycmBase[i].dFT  ;
			rData.FycmBase[i].dFB   = InData.FycmBase[i].dFB  ;
			rData.FycmBase[i].dFTL  = InData.FycmBase[i].dFTL ;
			rData.FycmBase[i].dFBL  = InData.FycmBase[i].dFBL ;
			rData.FycmBase[i].dFTR  = InData.FycmBase[i].dFTR ;
			rData.FycmBase[i].dFBR  = InData.FycmBase[i].dFBR ;
			rData.FycmBase[i].dFMAX = InData.FycmBase[i].dFMAX;
			rData.FycmBase[i].dALW  = InData.FycmBase[i].dALW ;
			rData.FycmBase[i].bOK   = InData.FycmBase[i].bOK  ;
		}

		return TRUE;
	}

	ConvertFycm_ToCivilData(const T_FYCM_AASHTO& InData, T_FYCM_D& rData)
	{
		for(int i=0; i<4; i++)
		{
			rData.FycmBase[i].bCHK  = InData.FycmBase[i].bCHK ;
			rData.FycmBase[i].LcomK = InData.FycmBase[i].LcomK;
			rData.FycmBase[i].iMax  = InData.FycmBase[i].iMax ;
			rData.FycmBase[i].iKind = InData.FycmBase[i].iKind;
			rData.FycmBase[i].dFT   = InData.FycmBase[i].dFT  ;
			rData.FycmBase[i].dFB   = InData.FycmBase[i].dFB  ;
			rData.FycmBase[i].dFTL  = InData.FycmBase[i].dFTL ;
			rData.FycmBase[i].dFBL  = InData.FycmBase[i].dFBL ;
			rData.FycmBase[i].dFTR  = InData.FycmBase[i].dFTR ;
			rData.FycmBase[i].dFBR  = InData.FycmBase[i].dFBR ;
			rData.FycmBase[i].dFMAX = InData.FycmBase[i].dFMAX;
			rData.FycmBase[i].dALW  = InData.FycmBase[i].dALW ;
			rData.FycmBase[i].bOK   = InData.FycmBase[i].bOK  ;
		}
		return TRUE;
	}
}