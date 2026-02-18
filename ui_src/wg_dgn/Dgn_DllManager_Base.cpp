// Dgn_DllManager_Base.cpp: implementation of the CDgn_DllManager_Base class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_DllManager_Base.h"


#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\FileCtrl.h"
//#include "DgnStruct.h"
#include "DgnDataCtrl.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\dgnengine\idesign\DGN_link\ILanguageLib.h"
#include "..\dgnengine\src\ILanguage\DgnMSGInfoStruct.h"
#include "..\dgnengine\src\dgnbase\DgnBase_CodeStruct.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_DllManager_Base::CDgn_DllManager_Base()
{	
    
}

CDgn_DllManager_Base::~CDgn_DllManager_Base()
{
}

int CDgn_DllManager_Base::Get_NationalIDformCode(int iCodeType, int iCode)
{
	int iNationType = 0;
    switch ( iCodeType )
    {
    case DGNE_CODETYPE_RC:
        iNationType = Get_NationalIDformRCCode(iCode);
        break;
    case DGNE_CODETYPE_STEEL:
        iNationType = Get_NationalIDformSteelCode(iCode);
        break;
    case DGNE_CODETYPE_SRC:
        iNationType = Get_NationalIDformSRCCode(iCode);
        break;
    case DGNE_CODETYPE_PSC:
        iNationType = Get_NationalIDformPSCCode(iCode);
        break;
    case DGNE_CODETYPE_CSG:
        iNationType = Get_NationalIDformCSGCode(iCode);
        break;
    case DGNE_CODETYPE_SOD:
        iNationType = Get_NationalIDformSodCode(iCode);
        break;
    default:
        ASSERT(0);
        break;
    }

    return iNationType;
}


int CDgn_DllManager_Base::Get_NationalIDformRCCode(int iCode)
{// 1:KR, 2:US, 3:CH, 4:JP;
	switch(iCode)
	{	
		//RC (KR) - DgnStruct.h에 등록된 Code ID 입니다.
		case KSCE_RC_USD05:	     //	117	
		case KSCE_RC_RAIL_USD04: // 118
		case KCI_USD07:          // 121
		case KCI_USD07_CVL:
		case KCI_USD12:  
		case KR_LRFD11_RC:
		case KSCE_RC_USD10:
		case KSCE_RC_RAIL_USD11:
		case KSCE_USD10_RC_RATING:
		case KSCE_RAIL_USD11_RC_RATING:
		case KSCE_LSD15_RC_RATING:
		case KDS_24_14_21_2022:
			return DGNENGINE_KR_DLL;
		//RC (US) - DgnStruct.h에 등록된 Code ID 입니다.
		case Eurocode2_2_05:     // 122
		case AASHTO_LC07:        // 157
		case AASHTO_LC12:        // 
		case AASHTO_LC16:        // 
        case AASHTO_LC17:        // 
		case IRC_112_11:				//179
        case IS456_2000:         // 114
        case IRC_112_20:				//182
			return DGNENGINE_US_DLL;
		case SNiP_20503_84RC:
		case SP_35_13330_11RC:
		case SNiP_20503_84RC_MKS:
		case SP_35_13330_11RC_MKS:
			return DGNENGINE_RS_DLL;
		//RC (CH) - ..\wg_db\DB_ST_DN.h에 등록된 Code ID 입니다.
		case RC_JTG_D60_04:      // 1261
		case RC_JTG_D62_04:      // 1262
		case RC_TB_10002_1_05:   // 1263
		case RC_TB_10002_3_05:   // 1264
		case RC_JTG_B02_01_2008: // 1265
		case RC_CJJ_11_2011: // 1266
		case RC_CJJ_166_2011: // 1267
			return DGNENGINE_CH_DLL;
		//RC (JP)
		// ...
	}
	ASSERT(0);
	return 0;
}
int CDgn_DllManager_Base::Get_NationalIDformSteelCode(int iCode)
{// 1:KR, 2:US, 3:CH, 4:JP;
	switch(iCode)
	{	
		//Steel (KR) 
		case KSCE_ASD05:
		case KSCE_RAIL_ASD04:
		case KSCE_ASD10:
		case KSCE_RAIL_ASD11:
		case KR_BRG_LSD15:   
		case STL_KSCE_ASD05:   //PMS:RATING BSC-20140314
		case STL_KSCE_ASD10:   
		case KDS_24_14_31_2018:
		case KDS_24_14_30_2019:
		case STL_KSCE_RAIL_ASD04:   
		case STL_KSCE_RAIL_ASD11:   
		case STL_KSCE_LSD15:
		case JROAD_H14:
		case JROAD_H24:
		case JROAD_H29:
		case JROAD_R07:
			return DGNENGINE_KR_DLL;		
		//Steel (US)
		case AASHTO_LS12:        //
		case STL_AASHTO_LRFD12:
        case STL_AASHTO_LRFD19:
        case STL_CS454_20:
		case STL_NR_GN_CIV_025_06:
			return DGNENGINE_US_DLL;
		//Steel (RS)
		case SP_16_13330_2017:
			return DGNENGINE_RS_DLL;
		//Steel (CH)
		// ...
		//Steel (JP)
		// ...
	}
	ASSERT(0);
	return 0;
}
int CDgn_DllManager_Base::Get_NationalIDformSRCCode(int iCode)
{// 1:KR, 2:US, 3:CH, 4:JP;
	//switch(iCode)
	//{	
		//SRC (KR) 
		// ...
		//SRC (US)
		// ...
		//SRC (CH)
		// ...
		//SRC (JP)
		// ...
	//}
	ASSERT(0);
	return 0;
}

int CDgn_DllManager_Base::Get_NationalIDformPSCCode(int iCode)
{// 1:KR, 2:US, 3:CH, 4:JP;
	switch(iCode)
	{	
		//CSD (KR) 
		case KSCE_ASD10_CSG:
		case KSCE_RAIL_ASD11_CSG:
			return DGNENGINE_KR_DLL;		
		//PSC (US)
		case AASHTO_LRFD08:
		case AASHTO_LRFD12:
		case AASHTO_LRFD14:
		case AASHTO_LRFD16:
        case AASHTO_LRFD17:
        case AASHTO_LRFD20:
		case AASHTO_LRFD24:
		case EUROCODE2_2_05_PSC:
		case IRC_112_2011_PSC:
		case IRC_112_2020_PSC:
		case AS_5100_5_17_PSC:
		case KDS_24_14_21_2022_PSC:    
		case KSCE_LSD15_PSC:    // KSCE_LSD15_PSC는 EC2계산을 따르므로..
		case BS5400_90_PSC:
		case TMH07_3_1989:
        case BD_44_15_PSC:
        case CS_455_PSC:
		case AASHTO_LC07:       
		case CSA_S6_10:
		case CSA_S6_14:
		case CSA_S6_19:
		case KDS_24_14_31_2018_CSG:
		case KSCE_LSD15_CSG:
		case KSSC_2014_CSG:
        case AASHTO_LRFD20_CSG:
        case AASHTO_LRFD17_CSG:
		case AASHTO_LRFD16_CSG:
		case AASHTO_LRFD12_CSG:
		case AASHTO_LRFD07_CSG:
		case IRS_PSC:
		case ABNT_NBR_6118_2023_PSC:
		case AREMA_23:
			return DGNENGINE_US_DLL;		
		//PSC (RUS)
		case SNiP_20503_84_PSC:
		case SP_35_13330_11_PSC:
		case SNiP_20503_84_PSC_MKS:
		case SP_35_13330_11_PSC_MKS:
			return DGNENGINE_RS_DLL;
		//PSC (CH)
		case JTG_D62_04:
		case TB_10002_3_05:
			return DGNENGINE_CH_DLL;		
		// ...
		//PSC (JP)
		// ...
	}
	ASSERT(0);
	return 0;
}

int CDgn_DllManager_Base::Get_NationalIDformCSGCode(int nCode)
{
    switch (nCode)
    {
    case KSCE_ASD10_CSG:
    case KSCE_RAIL_ASD11_CSG:
        return DGNENGINE_KR_DLL;   
    case KDS_24_14_31_2018_CSG:
    case KSCE_LSD15_CSG:
    case AASHTO_LRFD20_CSG:
    case AASHTO_LRFD17_CSG:
    case AASHTO_LRFD16_CSG:
    case AASHTO_LRFD12_CSG:
    case AASHTO_LRFD07_CSG:
    case KSSC_2014_CSG:
    case CSA_S6_10_CSG:
    case CSA_S6_14_CSG:
	case CSA_S6_19_CSG:
    case STL_KSCE_LSD15:
    case CS457_R1_CSG:
	case AS5100_6_17_CSG:
	case NR_GN_CIV_025_06_CSG:
        return DGNENGINE_US_DLL; // 
    case SNiP_20503_84_CSG:
    case SP_35_13330_11_CSG:
        return DGNENGINE_RS_DLL; // 

    default: ASSERT(0); break;
    }
    return 0;
}

int CDgn_DllManager_Base::Get_NationalIDformSodCode(int nCode)
{
	switch (nCode)
	{
	case SNiP_20503_84_SOD:
	case SP_35_13330_11_SOD:
		return DGNENGINE_RS_DLL;   
		
	default: ASSERT(0); break;
	}
	return 0;

}

CString CDgn_DllManager_Base::Get_CalcDllFileName(int iCodeType, int iCode)
{
	int iNationType = Get_NationalIDformCode(iCodeType, iCode);
	return CDb_DllManager_Base::Get_CalcDllFileName(iNationType);
}

CString CDgn_DllManager_Base::Get_ReportDllFileName(int iCodeType, int iCode)
{
	int iNationType = Get_NationalIDformCode(iCodeType, iCode);
	return CDb_DllManager_Base::Get_ReportDllFileName(iNationType);
}

CString CDgn_DllManager_Base::Get_CalcDllFileName(int iNationType)
{
	return CDb_DllManager_Base::Get_CalcDllFileName(iNationType);
}
CString CDgn_DllManager_Base::Get_ReportDllFileName(int iNationType)
{
	return CDb_DllManager_Base::Get_ReportDllFileName(iNationType);
}

HINSTANCE CDgn_DllManager_Base::GetOrLoad_CalcDllHandle(int iCodeType, int iCode)
{
	int iNationType = Get_NationalIDformCode(iCodeType, iCode);
	return CDb_DllManager_Base::GetOrLoad_CalcDllHandle(iNationType);
}

HINSTANCE CDgn_DllManager_Base::GetOrLoad_ReportDllHandle(int iCodeType, int iCode)
{
	int iNationType = Get_NationalIDformCode(iCodeType, iCode);
	return CDb_DllManager_Base::GetOrLoad_ReportDllHandle(iNationType);
}

HINSTANCE CDgn_DllManager_Base::GetOrLoad_CalcDllHandle(int iNationType)
{
	return CDb_DllManager_Base::GetOrLoad_CalcDllHandle(iNationType);
}
HINSTANCE CDgn_DllManager_Base::GetOrLoad_ReportDllHandle(int iNationType)
{
	return CDb_DllManager_Base::GetOrLoad_ReportDllHandle(iNationType);
}
//////////////////////////////////////////////////////////////////////////

int CDgn_DllManager_Base::ConvertToDLL_Code(int nCode)
{
	if     (nCode == RC_JTG_D60_04)      nCode = 161;
	else if(nCode == RC_JTG_D62_04)      nCode = 162;
	else if(nCode == RC_TB_10002_1_05)   nCode = 163;
	else if(nCode == RC_TB_10002_3_05)   nCode = 164;
	else if(nCode == RC_JTG_B02_01_2008||nCode == RC_CJJ_166_2011) nCode = 165;
	else ASSERT(0);

	// DLL에서 사용하는 ID값으로 변환 (위항목을 제외하고 현재09.09.11 wGen과 동일)
	return nCode;
}

void CDgn_DllManager_Base::GetRptOptionData(CString strProgramDir, 
																						CString strProjectFileDir, 
																						CString strSaveFileName,
																						int nDgnCode,
																						D_EXCEL_PRT_OPTION &ROpt)
{
	CStringW strWProgramDir(strProgramDir);
	CStringW strWProjectFileDir(strProjectFileDir);
	CStringW strWSaveFileName(strSaveFileName);

	int  nType  = 0;
	BOOL bRunMS = TRUE;
	BOOL bRowMark = TRUE;
	BOOL bColumnMark = TRUE;
	CProduct::GetTestOutType(nType, bRunMS, bRowMark, bColumnMark);

	if(nType != 0) 
	{
		CString sSaveFileTemp = strSaveFileName;
		CFileCtrl fileCtrl(sSaveFileTemp);     
		CString sSaveFileNa = fileCtrl.GetFilePathAndNameWithoutExtension() + _T("_Detail.txt");
		strWSaveFileName = CT2W(sSaveFileNa);
	}

	ROpt.strPath        = strWProgramDir;
	ROpt.strProject     = strWProjectFileDir;
	ROpt.strSaveName    = strWSaveFileName;
	ROpt.nCode          = nDgnCode;
	//ROpt.bIsView        = ROption.bXLView;
	ROpt.bIsForceSheet  = FALSE;
	ROpt.nRecType       = nType;
	ROpt.bRecExcelRun   = bRunMS;
	ROpt.bRecRowMark    = bRowMark;
	ROpt.bRecColumnMark = bColumnMark;

}

int CDgn_DllManager_Base::GetSteelCodeConvert2Engine(const UINT& nCode)
{
	int nEngineCode = -1;
	switch (nCode)
	{
	case JROAD_H14:
		return DGN_JROAD_H14;
	case JROAD_H24:
		return DGN_JROAD_H24;
	case JROAD_H29:
		return DGN_JROAD_H29;
	case JROAD_R07:
		return DGN_JROAD_R07;
	case KSCE_ASD05:
		return DGN_KSCE_ASD05;
	case KSCE_ASD10:
		return DGN_KSCE_ASD10;
	case KSCE_RAIL_ASD04:
		return DGN_KSCE_RAIL_ASD04;
	case KSCE_RAIL_ASD11:
		return DGN_KSCE_RAIL_ASD11;
	case KR_BRG_LSD15:
		return DGN_KR_BRG_LSD15;
	case KDS_24_14_30_2019:
		return DGN_KDS_24_14_30_2019;
	default:
		ASSERT(0);
		break;
	}
	return nEngineCode;
}

