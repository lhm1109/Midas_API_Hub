// LoadDB.cpp: implementation of the CLoadDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_ST_DT_PREF.h"

#include "DB_ST_DT_UNIT.h"
#include "..\wg_dbLock\LockMgr.h"
#include "DBCodeDef.h"
#include "SectDBRussiaUtil.h"
#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\Registry.h"
#include "..\MIT_lib\MRegistry.h"

void T_PREFERENCE::Initialize()
{
	int nMovingType = CProduct::GetMovingType();
	General.UserName.Empty();
	General.strTempFilePath.Empty();
	General.strLogoBmpPath.Empty();
	General.nRecentlyProject=15;
	General.bAutoSave=TRUE;
	General.nAutoSaveSecond=10*60;
	General.bMakeBackup=TRUE;
	General.nOptDefFileOpen = 0;
	General.strFileOpenPath.Empty();
	int nDefaultLanguage = 0;
	CString strReg = _T("HKEY_CURRENT_USER\\") + MRegistry::GetRegistryKey() + _T("LANGUAGE");
	MRegistry::GetProfileInt(strReg, _T("LANGUAGE"), nDefaultLanguage);
	General.nLanguageID = nDefaultLanguage;
	Results.bEnvelopeAllSign = TRUE; // MNET:2624 20070108 JHKIM
	if(nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS)
		Results.nCountryCode = 3;
	else if(nMovingType == D_PRODUCT_MOVING_JP)
		Results.nCountryCode = 2;
	else if(nMovingType == D_PRODUCT_MOVING_CH)
		Results.nCountryCode = 1;
	else
		Results.nCountryCode = 0;

#if defined(_MGEN_RUS)
	Results.nCountryCode = 4; // enum 쓰고싶다.
#endif

	Results.bSimpleLegend = FALSE;

	FLoadDB.Initialize();

	DgnCode.nDgnThread = enPrefDgnThreadMax;

	if(nMovingType == D_PRODUCT_MOVING_US || nMovingType == D_PRODUCT_MOVING_RUS)
	{        
		View.dblModelSize =9.144;  // 9.144m = 30ft (1m = 3.2808ft)
		View.dblZoomSpeed = 5.0;
		View.dblGridSpaceX=0.6096; // 0.6096m = 2ft
		View.dblGridSpaceY=0.6096; // 0.6096m = 2ft
		View.bGridDisplay=FALSE;    // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.nViewPoint=0;  // 0:ISO 1:X-Y
		View.bSnapPointGrid=FALSE;  // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapLineGrid=FALSE;   // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapNode=TRUE;
		View.bSnapElement=TRUE;
		View.bSnapTracing = TRUE;
		View.nVerticalPos = 1;
		View.bViewCube = TRUE;

		Data.dblRegardZero=1.e-10;
#if defined(_MGEN)
		Data.dblDuplicateTolerance = 0.001;   // 단위:m
		Data.dblIntersectTolerance = 0.001;   // 단위:m
		Data.dblCoordinateTolerance = 0.001;   // 단위:m
#else
		Data.dblDuplicateTolerance = 0.0009144;   // 0.0009144m = 0.003ft
		Data.dblIntersectTolerance = 0.0009144;   // 0.0009144m = 0.003ft
		Data.dblCoordinateTolerance = 0.0009144;   // 0.0009144m = 0.003ft
#endif
		if(CProduct::IsIndia())
		{
			Property.SteelMaterialDBName    = MATLCODE_STL_IS;
			Property.ConcreteMaterialDBName = MATLCODE_CON_IS;
			Property.ConcreteMaterialCodeName = _T("");
			Property.SectionDBName          = _T("IS808");
		}
		else if(CProduct::IsTaiwan())
		{
			// Modify by GAY. PMS:4064. ('09.09.22). "CNS(S)" -> "CNS06(S)";
			// PMS:4385-Seungjun-20111006 대만 신기준 추가 TWN-USD100 "CNS(RC)" -> "CNS560(RC)"
			Property.SteelMaterialDBName    = MATLCODE_STL_CNS06;
			Property.ConcreteMaterialDBName = MATLCODE_CON_CNS560;
			Property.ConcreteMaterialCodeName = _T("");
			Property.SectionDBName          = _T("CNS91");
		}
		else
		{
			// Change by ZINU.('08.01.23). NO:3184, US -> Italy.
			// Steel    = "ASTM(S)" -> "EN05(S)"
			// Concrete = "ASTM(RC)" -> "EN04(RC)", "Grade C4000" -> "C25/30", "Grade 60" -> "Class A"
			// Sect     = "AISC" -> "UNI"
			if (nMovingType == D_PRODUCT_MOVING_RUS)
			{
				Property.SteelMaterialDBName = MATLCODE_STL_SP16_2017_TB3;
				Property.ConcreteMaterialDBName = MATLCODE_CON_SP63_2018;
				Property.ConcreteMaterialCodeName = _T("");
				Property.SectionDBName = _T("GOST_54929-2012");
				CSectDBRussiaUtil::ConvertSectDBEngToRus(Property.SectionDBName);
			}
			else
			{
				Property.SteelMaterialDBName = MATLCODE_STL_EN05;
				Property.ConcreteMaterialDBName = MATLCODE_CON_EN04;
				Property.ConcreteMaterialCodeName = _T("");
				Property.SectionDBName = _T("UNI"); //"AISC";
			}
		}

		Unit.bAskNewProject=FALSE;
		//Unit.nLengthUnit=D_UNITSYS_LENGTH_INDEX_FT;
		//Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_KIP;
		Unit.nLengthUnit=D_UNITSYS_LENGTH_INDEX_M;
		Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_KN;
		Unit.nHeatUnit=D_UNITSYS_HEAT_INDEX_BTU;
		Unit.nTemperUnit=D_UNITSYS_TEMPER_INDEX_F;

		// kip, ft 기준
		Format.bAutoChange=TRUE;
		Format.bExponential=FALSE;
		Format.nDimension=1;    // 아래 모두 변경 MNET1727-JBSEON-051010
		Format.nDisplacement=3;
		Format.nDimensionless=1;
		Format.nRotation=1;
		Format.nMass=1;
		Format.nWeight=1;
		Format.nForce=1;
		Format.nUnitForce=1;
		Format.nMoment=1;
		Format.nUnitMoment=1;
		Format.nStress=1;
		Format.nAcceleration=1;
#if defined(_MGEN)
		if(CProduct::IsIndia())
		{
			DgnCode.SteelCode= STLCODE_IS800_2007;
			DgnCode.CFSteelCode= CFSCODE_EC3_06;
			DgnCode.ConcCode= CONCODE_IS456_2000;
			DgnCode.ConcRebarMatlCode=_T("IS(RC)");
			DgnCode.ConcRebarMatlDB=_T("Fe415");
			DgnCode.SRCCode=_T("SSRC79");
			DgnCode.SRCRebarMatlCode=_T("IS(RC)");
			DgnCode.SRCRebarMatlDB=_T("Fe415");
			DgnCode.RCCode=_T("AASHTO-LRFD05");
			DgnCode.RatingSTL=_T("KSCE-ASD10");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.RatingPSC=_T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가 
			DgnCode.PSCCode=_T("KSCE-USD03");
			DgnCode.PGCode=_T("EN 1994-2");
			DgnCode.StlOrthoDeck= SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			LoadCode.WLCode = _T("IS875(2015)");  //OLD "IS875(1987)";
			LoadCode.SLCode = _T("IS1893(2002)"); // MQC:8446-JWKWON-20140811
			LoadCode.RSCode = _T("IS1893(2002)");
			LoadCode.MLCode = _LS(IDS_CMD_MVCD_India);
			LoadCode.nWLNationalAnnex = 0;
			LoadCode.nSLNationalAnnex = 0;
			LoadCode.nRSNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;	
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		}
		else if(CProduct::IsTaiwan())
		{
			DgnCode.SteelCode= STLCODE_TWN_ASD96;
			DgnCode.CFSteelCode= CFSCODE_EC3_06;
			DgnCode.ConcCode= CONCODE_TWN_USD112;  //"TWN-USD92";
			DgnCode.ConcRebarMatlCode=_T("CNS560(RC)");
			DgnCode.ConcRebarMatlDB=_T("SD420");
			DgnCode.SRCCode=_T("TWN-SRC100"); //"TWN-SRC92";
			DgnCode.SRCRebarMatlCode=_T("CNS560(RC)");
			DgnCode.SRCRebarMatlDB=_T("SD420");
			DgnCode.RCCode=_T("AASHTO-LRFD05");
			DgnCode.RatingSTL=_T("KSCE-ASD10");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.RatingPSC=_T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가
			DgnCode.PSCCode=_T("KSCE-USD03");
			DgnCode.PGCode=_T("EN 1994-2");
			DgnCode.StlOrthoDeck= SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
			LoadCode.WLCode = _T("Taiwan(2002)");
			LoadCode.SLCode = _T("Taiwan(2011)");
			LoadCode.RSCode = _T("Taiwan(2006)");
			LoadCode.MLCode = _LS(IDS_CMD_MVCD_Taiwan);
			LoadCode.nWLNationalAnnex = 0;
			LoadCode.nSLNationalAnnex = 0;
			LoadCode.nRSNationalAnnex = 0;
		}
		else
		{
			// Change by ZINU.('08.01.23). NO:3184, US -> Italy.
			// "AISC-LRFD2K" -> "Eurocode3:05", "ACI318-02" -> "Eurocode2:04"
			// Concrete = "ASTM(RC)" -> "EN04(RC)", "Grade C4000" -> "C25/30", "Grade 60" -> "Class A"
			if (nMovingType == D_PRODUCT_MOVING_RUS)
			{
				DgnCode.SteelCode = STLCODE_SP_16_13330_2017; //"AISC-LRFD2K";
				DgnCode.CFSteelCode = CFSCODE_EC3_06;
				DgnCode.ConcCode = CONCODE_SP_63_13330_2018;  //"ACI318-02";
				DgnCode.ConcRebarMatlCode = MATLCODE_CON_SP63_2018; //"ASTM(RC)";
				DgnCode.ConcRebarMatlDB = _T("A400");  //"Grade 60";
				DgnCode.SRCCode = _T("SSRC79");
				DgnCode.SRCRebarMatlCode = MATLCODE_CON_SP63_2018;
				DgnCode.SRCRebarMatlDB = _T("A400");
			}
			else
			{
				DgnCode.SteelCode = STLCODE_EC3_05; //"AISC-LRFD2K";
				DgnCode.CFSteelCode = CFSCODE_EC3_06;
				DgnCode.ConcCode = CONCODE_EC2_04;  //"ACI318-02";
				DgnCode.ConcRebarMatlCode = _T("EN04(RC)"); //"ASTM(RC)";
				DgnCode.ConcRebarMatlDB = _T("Class A");  //"Grade 60";
				DgnCode.SRCCode = _T("SSRC79");
				DgnCode.SRCRebarMatlCode = _T("ASTM(RC)");
				DgnCode.SRCRebarMatlDB = _T("Grade 60");
			}
			DgnCode.RCCode = _T("KSCE-USD10");
			DgnCode.RatingSTL = _T("KSCE-ASD10");
			DgnCode.RatingRC = _T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.RatingPSC = _T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가
			DgnCode.PSCCode = _T("KSCE-USD03");
			DgnCode.PGCode = _T("EN 1994-2");
			DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval = _T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;

			if(CProduct::IsCzech())
			{
				LoadCode.WLCode = _T("Eurocode-1(2005)");
				LoadCode.SLCode = _T("Eurocode-8(2004)");
				LoadCode.RSCode = _T("Eurocode-8(2004)");
				LoadCode.MLCode = _LS(IDS_CMD_MVCD_EURO_BS);
				LoadCode.nWLNationalAnnex = 0;
				LoadCode.nSLNationalAnnex = 0;
				LoadCode.nRSNationalAnnex = 0;
			}
			else
			{
				if (nMovingType == D_PRODUCT_MOVING_RUS)
				{
					LoadCode.WLCode = FULL_RUS_W_2016_USER;
					LoadCode.SLCode = _T("User Type");
					LoadCode.RSCode = SPFC_RUS_SP_14_13330;
				}
				else
				{
					LoadCode.WLCode = _T("Eurocode-1(2005)");
					LoadCode.SLCode = _T("Eurocode-8(2004)");
					LoadCode.RSCode = _T("Eurocode-8(2004)");
				}
				LoadCode.MLCode = _LS(IDS_CMD_MVCD_EURO_BS);
				LoadCode.nWLNationalAnnex = 0;
				LoadCode.nSLNationalAnnex = 0;
				LoadCode.nRSNationalAnnex = 0;
			}

		}

#elif defined(_CIVIL)
		if(CProduct::IsIndia())
		{
			//DgnCode.SteelCode=_T("IS:800-1984");
			DgnCode.SteelCode=_T("IRC:24-2010");
			DgnCode.CFSteelCode=_T("");
			DgnCode.ConcCode=_T("IRC:112-2011");
			DgnCode.ConcRebarMatlCode=_T("IS(RC)");
			DgnCode.ConcRebarMatlDB=_T("Fe415");
			DgnCode.SRCCode=_T("SSRC79");
			DgnCode.SRCRebarMatlCode=_T("IS(RC)");
			DgnCode.SRCRebarMatlDB=_T("Fe415");
			DgnCode.RCCode=_T("AASHTO-LRFD05");
			DgnCode.RatingPSC=_T("AASHTO-LRFD05"); 
			DgnCode.RatingSTL=_T("KSCE-ASD10");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.PSCCode=_T("AASHTO-LRFD07");
			DgnCode.PGCode=_T("EN 1994-2");
			DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		}
		else if(CProduct::IsTaiwan())
		{
			DgnCode.SteelCode=_T("TWN-BRG-LSD90");
			DgnCode.CFSteelCode=_T("");
			DgnCode.ConcCode=_T("TWN-BRG-LSD90");
			DgnCode.ConcRebarMatlCode=_T("CNS(RC)");
			DgnCode.ConcRebarMatlDB=_T("SD420");
			DgnCode.SRCCode=_T("TWN-SRC92");
			DgnCode.SRCRebarMatlCode=_T("CNS(RC)");
			DgnCode.SRCRebarMatlDB=_T("SD420");
			DgnCode.RCCode=_T("AASHTO-LRFD05");
			DgnCode.RatingPSC=_T("AASHTO-LRFD05");
			DgnCode.RatingSTL=_T("KSCE-ASD10");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.PSCCode=_T("AASHTO-LRFD07");
			DgnCode.PGCode=_T("EN 1994-2");
			DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		}
		else
		{
#if defined(_RUS)
			DgnCode.SteelCode=_T("AASHTO-LRFD20");
			DgnCode.CFSteelCode=_T("");
			DgnCode.ConcCode= CONCODE_SNIP_2_05_03_84;
			DgnCode.ConcRebarMatlCode= MATLCODE_CON_GOST_SP;
			DgnCode.ConcRebarMatlDB=_T("A400");
			DgnCode.SRCCode=_T("SSRC79");
			DgnCode.SRCRebarMatlCode=_T("ASTM(RC)");
			DgnCode.SRCRebarMatlDB=_T("Grade 60");
			DgnCode.RCCode=_T("AASHTO-LRFD05");
			DgnCode.RatingSTL=_T("KSCE-ASD10");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.PSCCode      = CONCODE_SNIP_2_05_03_84;
			DgnCode.PGCode       = CSGCODE_SNIP_2_05_03_84;
			DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#else
			DgnCode.SteelCode=_T("AASHTO-LRFD20");
			DgnCode.CFSteelCode=_T("");
			DgnCode.ConcCode=_T("AASHTO-LRFD20");
			DgnCode.ConcRebarMatlCode=_T("ASTM19(RC)");
			DgnCode.ConcRebarMatlDB=_T("Grade 60");
			DgnCode.SRCCode=_T("SSRC79");
			DgnCode.SRCRebarMatlCode=_T("ASTM19(RC)");
			DgnCode.SRCRebarMatlDB=_T("Grade 60");
			DgnCode.RCCode=_T("AASHTO-LRFD20");
			DgnCode.RatingPSC=_T("AASHTO-LRFR19");
			DgnCode.RatingSTL=_T("AASHTO-LRFR19");
			DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
			DgnCode.PSCCode=_T("AASHTO-LRFD20");
			DgnCode.PGCode=_T("AASHTO-LRFD20");
			DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
			DgnCode.BrdgSeisEval=_T("KALIS2023");
			DgnCode.OutputLanguage = 0;
			DgnCode.nConcNationalAnnex = 0;
			DgnCode.nPscNationalAnnex = 0;
			DgnCode.nStlNationalAnnex = 0;
			DgnCode.nCfsNationalAnnex = 0;
			DgnCode.nPgNationalAnnex = 0;
			DgnCode.nStlReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportLanguage = EN_REPORT_English;
			DgnCode.nPscReportLanguage = EN_REPORT_English;
			DgnCode.nPgReportLanguage = EN_REPORT_English;
			DgnCode.nConcReportUnit = EN_REPORT_UNIT_US;
			DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
			DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#endif
		}
#endif
	}    
	else if(nMovingType == D_PRODUCT_MOVING_JP)
	{
		View.dblModelSize=10.;   // 단위:m
		View.dblZoomSpeed = 5.0;
		View.dblGridSpaceX=0.5;
		View.dblGridSpaceY=0.5;
		View.bGridDisplay=FALSE;    // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.nViewPoint=0;  // 0:ISO 1:X-Y
		View.bSnapPointGrid=FALSE;  // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapLineGrid=FALSE;   // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapNode=TRUE;
		View.bSnapElement=TRUE;
		View.bSnapTracing = TRUE;
		View.nVerticalPos = 1;
		View.bViewCube = TRUE;

		Data.dblRegardZero=1.e-10;
		Data.dblDuplicateTolerance=0.001;   // 단위:m
		Data.dblIntersectTolerance=0.001;   // 단위:m
		Data.dblCoordinateTolerance=0.001;   // 단위:m

#if defined(_CIVIL)
		Property.SteelMaterialDBName    = MATLCODE_STL_JIS_CIVIL;
		Property.ConcreteMaterialDBName = MATLCODE_CON_JIS_CIVIL;
#else			
		Property.SteelMaterialDBName    = MATLCODE_STL_JIS;
		Property.ConcreteMaterialDBName = MATLCODE_CON_JIS;
#endif
		Property.ConcreteMaterialCodeName = _T("");
		Property.SectionDBName          = _T("JIS");

		Unit.bAskNewProject=FALSE;
		Unit.nLengthUnit=D_UNITSYS_LENGTH_INDEX_M;
		Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_KN;
		Unit.nHeatUnit=D_UNITSYS_HEAT_INDEX_KJOULE; // JBSEON-060127 스펙에 kJ로 표시되어 있어 변경
		Unit.nTemperUnit=D_UNITSYS_TEMPER_INDEX_C;

		// ton, m 기준
		Format.bAutoChange=TRUE;
		Format.bExponential=FALSE;
		Format.nDimension=1;    // 아래 모두 변경 MNET1727-JBSEON-051010
		Format.nDisplacement=3;
		Format.nDimensionless=1;
		Format.nRotation=1;
		Format.nMass=1;
		Format.nWeight=1;
		Format.nForce=1;
		Format.nUnitForce=1;
		Format.nMoment=1;
		Format.nUnitMoment=1;
		Format.nStress=1;
		Format.nAcceleration=1;

#if defined(_MGEN)
		DgnCode.SteelCode=_T("AIJ-ASD02");
		DgnCode.CFSteelCode=_T("AISI-CFSD86");
		DgnCode.ConcCode=_T("AIJ-WSD99");
		DgnCode.ConcRebarMatlCode=_T("JIS(RC)");
		DgnCode.ConcRebarMatlDB=_T("SD390");
		DgnCode.SRCCode=_T("AIJ-SRC01");
		DgnCode.SRCRebarMatlCode=_T("JIS(RC)");
		DgnCode.SRCRebarMatlDB=_T("SD390");
		DgnCode.OutputLanguage = 0;
		DgnCode.RCCode=_T("AASHTO-LRFD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가 
		DgnCode.PSCCode=_T("AASHTO-LRFD12");
		DgnCode.PGCode=_T("EN 1994-2");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		LoadCode.WLCode = _T("Japan(2004)");
		LoadCode.SLCode = _T("Japan(Arch.2000)");
		LoadCode.RSCode = _T("Japan(Arch.2000)");
		LoadCode.MLCode = _LS(IDS_CMD_MVCD_None);
		LoadCode.nWLNationalAnnex = 0;
		LoadCode.nSLNationalAnnex = 0;
		LoadCode.nRSNationalAnnex = 0;

#elif defined(_CIVIL)
		DgnCode.SteelCode=_T("AASHTO-LRFD02");
		DgnCode.CFSteelCode=_T("");
		DgnCode.ConcCode=_T("AASHTO-LRFD02");
		DgnCode.ConcRebarMatlCode=_T("JIS-Civil(RC)");
		DgnCode.ConcRebarMatlDB=_T("SD345");
		DgnCode.SRCCode=_T("AIJ-SRC01");
		DgnCode.SRCRebarMatlCode=_T("JIS-Civil(RC)");
		DgnCode.SRCRebarMatlDB=_T("SD345");
		DgnCode.RCCode=_T("AASHTO-LRFD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("AASHTO-LRFD05"); 
		DgnCode.PSCCode=_T("AASHTO-LRFD12");
		DgnCode.PGCode=_T("EN 1994-2");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval = BRDGSE_JROAD5_H14;
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#endif

	}    
	else if(nMovingType == D_PRODUCT_MOVING_CH)
	{
		View.dblModelSize=10.;   // 단위:m
		View.dblZoomSpeed = 5.0;
		View.dblGridSpaceX=0.5;
		View.dblGridSpaceY=0.5;
		View.bGridDisplay=FALSE;    // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.nViewPoint=0;  // 0:ISO 1:X-Y
		View.bSnapPointGrid=FALSE;  // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapLineGrid=FALSE;   // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapNode=TRUE;
		View.bSnapElement=TRUE;
		View.bSnapTracing = TRUE;
		View.nVerticalPos = 1;
		View.bViewCube = TRUE;

		Data.dblRegardZero=1.e-10;
		Data.dblDuplicateTolerance=0.001;   // 단위:m
		Data.dblIntersectTolerance=0.001;   // 단위:m
		Data.dblCoordinateTolerance=0.001;  // 단위:m

#ifndef _CIVIL
		{
			Property.SteelMaterialDBName    = MATLCODE_STL_GB12;
			Property.ConcreteMaterialDBName = MATLCODE_CON_GB10; // Modify by GAY. PMS:4267. ('12.03.08). GB(RC)->GB10(RC).
#ifdef _MGEN_CH
			Property.SteelMaterialDBName = MATLCODE_STL_GB50017_17;
#endif
		}
#else
		{
			Property.SteelMaterialDBName    = MATLCODE_STL_JTG3362_18;
			Property.ConcreteMaterialDBName = MATLCODE_CON_JTG3362_18;
		}
#endif
		Property.ConcreteMaterialCodeName = _T("");
		Property.SectionDBName          = _T("GB-YB05");

		Unit.bAskNewProject=FALSE;
		Unit.nLengthUnit=D_UNITSYS_LENGTH_INDEX_M;
		Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_TON;
		Unit.nHeatUnit=D_UNITSYS_HEAT_INDEX_KJOULE; // JBSEON-060127 스펙에 kJ로 표시되어 있어 변경
		Unit.nTemperUnit=D_UNITSYS_TEMPER_INDEX_C;

		// ton, m 기준
		Format.bAutoChange=TRUE;
		Format.bExponential=FALSE;
		Format.nDimension=1;    // 아래 모두 변경 MNET1727-JBSEON-051010
		Format.nDisplacement=3;
		Format.nDimensionless=1;
		Format.nRotation=1;
		Format.nMass=1;
		Format.nWeight=1;
		Format.nForce=1;
		Format.nUnitForce=1;
		Format.nMoment=1;
		Format.nUnitMoment=1;
		Format.nStress=1;
		Format.nAcceleration=1;

#if defined(_MGEN)         	  
		//add by maxiao 2015-10-19
		DgnCode.SteelCode=_T("GB50017-17");
		//DgnCode.SteelCode="GB50017-15";//add by maxiao(2015-9-24)GB50017-03 to GB50017-15
		CString strStlCode = CRegistry::AppGetProfileString( _T("Settings"), _T("StlCode"), _T("") );
		if(strStlCode==_T("GB50017-15")) DgnCode.SteelCode=_T("GB50017-15");
		////add by xuezc(2018/2/7)GB50017-17
		//CString strStlCode2 = CRegistry::AppGetProfileString( "Settings", "StlCode", "" );
		//if(strStlCode2=="GB50017-17") DgnCode.SteelCode="GB50017-17";

		DgnCode.CFSteelCode=_T("AISI-CFSD86");
		DgnCode.ConcCode=_T("GB/T50010-10");//"GB50010-02";
		DgnCode.AluliumCode = _T("GB50429-2007");
		DgnCode.ConcRebarMatlCode=_T("GB/T10(RC)");//"GB(RC)";
		DgnCode.ConcRebarMatlDB=_T("HRB400");
		DgnCode.SRCCode=_T("JGJ138-01");
		DgnCode.SRCRebarMatlCode=_T("GB/T10(RC)");//"GB(RC)";
		DgnCode.SRCRebarMatlDB=_T("HRB400");
		DgnCode.RCCode=_T("AASHTO-LRFD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가 
		DgnCode.PSCCode=_T("JTG D62-04");
		DgnCode.PGCode=_T("EN 1994-2");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		LoadCode.WLCode = _T("China(GB55001-2021)");
		LoadCode.SLCode = _T("China(GB/T50011-2010)");
		LoadCode.RSCode = _T("China(GB50011-2019)");
		LoadCode.MLCode = _LS(IDS_CMD_MVCD_None);
		LoadCode.nWLNationalAnnex = 0;
		LoadCode.nSLNationalAnnex = 0;
		LoadCode.nRSNationalAnnex = 0;

#elif defined(_CIVIL)
		DgnCode.SteelCode=_T("JGJ025-86");
		DgnCode.CFSteelCode=_T("");
		DgnCode.ConcCode=_T("JTJ023-85");
		DgnCode.ConcRebarMatlCode=_T("JTG04(RC)");
		DgnCode.ConcRebarMatlDB=_T("HRB335");
		DgnCode.SRCCode=_T("JGJ138-01");
		DgnCode.SRCRebarMatlCode=_T("JTG04(RC)");
		DgnCode.SRCRebarMatlDB=_T("HRB335");
		DgnCode.RCCode=_T("AASHTO-LRFD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("AASHTO-LRFD05"); 
		DgnCode.PSCCode=_T("JTG D62-04");
		DgnCode.PGCode=_T("AASHTO-LRFD12");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#endif
	}    
	else
	{
		View.dblModelSize=10.;   // 단위:m
		View.dblZoomSpeed = 5.0;
		View.dblGridSpaceX=0.5;
		View.dblGridSpaceY=0.5;
		View.bGridDisplay=FALSE;    // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.nViewPoint=0;  // 0:ISO 1:X-Y
		View.bSnapPointGrid=FALSE;  // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapLineGrid=FALSE;   // MNET1727-JBSEON-051010 (TRUE->FALSE)
		View.bSnapNode=TRUE;
		View.bSnapElement=TRUE;
		View.bSnapTracing = TRUE;
		View.nVerticalPos = 1;
		View.bViewCube = TRUE;

		Data.dblRegardZero=1.e-10;
		Data.dblDuplicateTolerance=0.001;   // 단위:m
		Data.dblIntersectTolerance=0.001;   // 단위:m
		Data.dblCoordinateTolerance=0.001;  // 단위:m

#ifndef _CIVIL
		// Change by ZINU.('08.05.28). NO:3446, KS(S) -> KS08(S).
		// Modify by GAY. PMS:4035. ('09.06.22). KS08(S) -> KS09(S).
		Property.SteelMaterialDBName    = MATLCODE_STL_KS22;
		Property.ConcreteMaterialDBName = MATLCODE_CON_KS19;
#else
		Property.SteelMaterialDBName    = MATLCODE_STL_KSCE_LSD15;
		Property.ConcreteMaterialDBName = MATLCODE_CON_KSCE_LSD15;
#endif

		Property.ConcreteMaterialCodeName = _T("KCI-2012");	
		Property.SectionDBName						= _T("KS21");

		Unit.bAskNewProject=FALSE;
		Unit.nLengthUnit=D_UNITSYS_LENGTH_INDEX_M;
		Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_TON;

		Unit.nHeatUnit=D_UNITSYS_HEAT_INDEX_KCAL;
		Unit.nTemperUnit=D_UNITSYS_TEMPER_INDEX_C;

#if defined(_MGEN) && defined(_ORG)
		Unit.nForceUnit=D_UNITSYS_FORCE_INDEX_KN;
#endif


		// ton, m 기준
		Format.bAutoChange=TRUE;
		Format.bExponential=FALSE;
		Format.nDimension=1;    // 아래 모두 변경 MNET1727-JBSEON-051010
		Format.nDisplacement=3;
		Format.nDimensionless=1;
		Format.nRotation=1;
		Format.nMass=1;
		Format.nWeight=1;
		Format.nForce=1;
		Format.nUnitForce=1;
		Format.nMoment=1;
		Format.nUnitMoment=1;
		Format.nStress=1;
		Format.nAcceleration=1;

#if defined(_MGEN)
		DgnCode.SteelCode=STLCODE_KDS_41_30_10_2022;
		DgnCode.CFSteelCode=_T("AIK-CFSD98");
        DgnCode.ConcCode= CONCODE_KDS_41_20_2022;//CONCODE_KDS_41_30_2021; ///[GEN-4372] ACITVE KDS2021
		DgnCode.ConcRebarMatlCode=_T("KS19(RC)");
		DgnCode.ConcRebarMatlDB=_T("SD400");
		DgnCode.SRCCode=_T("AIK-SRC2K"); // PMS:XXXX-THBAE-20130515 : 추후에 KBC-SRC09 기준 추가되면 여기 수정해 줄 것.
		DgnCode.SRCRebarMatlCode=_T("KS19(RC)");
		DgnCode.SRCRebarMatlDB=_T("SD400");
		DgnCode.RCCode=_T("KSCE-USD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("AASHTO-LRFR11"); // 나중에 개발 되면 코드 추가 
		DgnCode.PSCCode=_T("KSCE-USD10");
		DgnCode.PGCode=_T("EN 1994-2");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
		LoadCode.WLCode = FULL_KDS_W_2022;
		LoadCode.SLCode = FULL_KDS_E_2019;
		LoadCode.RSCode = _T("KDS(41-17-00:2019)");
		LoadCode.MLCode = _LS(IDS_CMD_MVCD_Korea);
		LoadCode.nWLNationalAnnex = 0;
		LoadCode.nSLNationalAnnex = 0;
		LoadCode.nRSNationalAnnex = 0;
#elif defined(_CIVIL)
		DgnCode.SteelCode=_T("KSCE-LSD15"); 
		DgnCode.CFSteelCode=_T("");
		DgnCode.ConcCode=_T("KSCE-LSD15");
		DgnCode.ConcRebarMatlCode=_T("KSCE-LSD15(RC)");
		DgnCode.RCCode=_T("KSCE-USD10");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("KSCE-USD10"); 
		DgnCode.ConcRebarMatlDB=_T("SD300");
		DgnCode.SRCCode=_T("AIK-SRC2K"); // PMS:XXXX-THBAE-20130515 : 추후에 KBC-SRC09 기준 추가되면 여기 수정해 줄 것.
		DgnCode.SRCRebarMatlCode=_T("KSCE-LSD15(RC)");
		DgnCode.SRCRebarMatlDB=_T("SD300");
		DgnCode.PSCCode=_T("KSCE-LSD15"); 
		DgnCode.PGCode = CSGCODE_KDS_24_14_31_2018;
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#elif defined(_TOWER)
		DgnCode.SteelCode=_T("KEPCO97-1111");
		DgnCode.CFSteelCode=_T("");
		DgnCode.ConcCode=_T("KCI-USD12");  // Coded by SeungJun '20080620 KCI-USD03 -> KCI-USD07 -> KCI-USD12 //??// Seungjun 요청에 의해 막음.(111020)
		DgnCode.ConcRebarMatlCode=_T("KS19(RC)");
		DgnCode.ConcRebarMatlDB=_T("SD400");
		DgnCode.SRCCode=_T("AIK-SRC2K");
		DgnCode.SRCRebarMatlCode=_T("KS19(RC)");
		DgnCode.SRCRebarMatlDB=_T("SD400");
		DgnCode.RCCode=_T("KSCE-USD05");
		DgnCode.RatingSTL=_T("KSCE-ASD10");
		DgnCode.RatingRC=_T("KSCE-USD10"); // 나중에 개발 되면 코드 추가 
		DgnCode.RatingPSC=_T("KSCE-USD05");
		DgnCode.PSCCode=_T("KSCE-USD05");
		DgnCode.PGCode=_T("EN 1994-2");
		DgnCode.StlOrthoDeck = SODCODE_SNIP_2_05_03_84;
		DgnCode.BrdgSeisEval=_T("KALIS2023");
		DgnCode.OutputLanguage = 0;
		DgnCode.nConcNationalAnnex = 0;
		DgnCode.nPscNationalAnnex = 0;
		DgnCode.nStlNationalAnnex = 0;
		DgnCode.nCfsNationalAnnex = 0;
		DgnCode.nPgNationalAnnex = 0;
		DgnCode.nStlReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportLanguage = EN_REPORT_English;
		DgnCode.nPscReportLanguage = EN_REPORT_English;
		DgnCode.nPgReportLanguage = EN_REPORT_English;
		DgnCode.nConcReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPscReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nPgReportUnit = EN_REPORT_UNIT_SI;
		DgnCode.nStlReportUnit = EN_REPORT_UNIT_SI;
#endif
	}

	Notice .bShowNotice          = TRUE;
	Notice.bUseLocalHelp         = FALSE;
	Notice.nStartPageContents    = 0;
	Notice.strHomePageURL        = _T("http://");
	LoadVal.m_DefLoadValDecPoint = 1   ;
	LoadVal.m_nFormatType = 1;
}

T_PREFER_GENERAL_UNIT::T_PREFER_GENERAL_UNIT()
{
	nCount=9;
	int nUnit[9];
	nUnit[0]=D_UNITSYS_NONE;
	nUnit[1]=D_UNITSYS_NONE;
	nUnit[2]=D_UNITSYS_NONE;
	nUnit[3]=D_UNITSYS_NONE;
	nUnit[4]=D_UNITSYS_NONE;
	nUnit[5]=D_UNITSYS_NONE;
	nUnit[6]=D_UNITSYS_NONE;
	nUnit[7]=D_UNITSYS_NONE;
	nUnit[8]=D_UNITSYS_NONE;

	UserName=nUnit[0];
	strTempFilePath=nUnit[1];
	strLogoBmpPath=nUnit[2];
	nRecentlyProject=nUnit[3];
	bAutoSave=nUnit[4];
	nAutoSaveSecond=nUnit[5];
	bMakeBackup=nUnit[6];
	nOptDefFileOpen = nUnit[7];
	strFileOpenPath = nUnit[8];
}

T_PREFER_VIEW_UNIT::T_PREFER_VIEW_UNIT()
{
	nCount=11;
	nUnit[0]=D_UNITSYS_BASE_LENGTH;
	nUnit[1]=D_UNITSYS_BASE_LENGTH;
	nUnit[2]=D_UNITSYS_BASE_LENGTH;
	nUnit[3]=D_UNITSYS_NONE;
	nUnit[4]=D_UNITSYS_NONE;
	nUnit[5]=D_UNITSYS_NONE;
	nUnit[6]=D_UNITSYS_NONE;
	nUnit[7]=D_UNITSYS_NONE;
	nUnit[8]=D_UNITSYS_NONE;
	nUnit[9] = D_UNITSYS_NONE;
	nUnit[10] = D_UNITSYS_NONE;
	dblModelSize=nUnit[0];
	dblGridSpaceX=nUnit[1];
	dblGridSpaceY=nUnit[2];
	bGridDisplay=nUnit[3];
	nViewPoint=nUnit[4];  // 0:ISO 1:X-Y
	bSnapPointGrid=nUnit[5];
	bSnapLineGrid=nUnit[6];
	bSnapNode=nUnit[7];
	bSnapElement=nUnit[8];
	dblZoomSpeed = nUnit[9];
	bSnapTracing = nUnit[10];
}

T_PREFER_DATA_UNIT::T_PREFER_DATA_UNIT()
{
	nCount=4;
	nUnit[0]=D_UNITSYS_NONE;
	nUnit[1]=D_UNITSYS_BASE_LENGTH;
	nUnit[2]=D_UNITSYS_BASE_LENGTH;
	nUnit[3]=D_UNITSYS_BASE_LENGTH;
	dblRegardZero=nUnit[0];
	dblDuplicateTolerance=nUnit[1];
	dblIntersectTolerance=nUnit[2];
	dblCoordinateTolerance=nUnit[3];
}

T_PREFER_RESULTS_UNIT::T_PREFER_RESULTS_UNIT()
{
	nCount=1;
	nUnit[0]=D_UNITSYS_NONE;
	bEnvelopeAllSign=nUnit[0];
	bSimpleLegend=nUnit[0];
}

T_PREFER_PROPERTY_UNIT::T_PREFER_PROPERTY_UNIT()
{
	nCount=4;
	nUnit[0]=D_UNITSYS_NONE;
	nUnit[1]=D_UNITSYS_NONE;
	nUnit[2]=D_UNITSYS_NONE;
	nUnit[3]=D_UNITSYS_NONE;
	SteelMaterialDBName=nUnit[0];
	ConcreteMaterialDBName=nUnit[1];
	ConcreteMaterialCodeName=nUnit[2];
	SectionDBName=nUnit[3];
}

T_PREFER_UNITTYPE_UNIT::T_PREFER_UNITTYPE_UNIT()
{
	nCount=3;
	nUnit[0]=D_UNITSYS_NONE;
	nUnit[1]=D_UNITSYS_NONE;
	nUnit[2]=D_UNITSYS_NONE;
	bAskNewProject=nUnit[0];
	nLengthUnit=nUnit[1];
	nForceUnit=nUnit[2];
}

T_PREFER_FORMAT_UNIT::T_PREFER_FORMAT_UNIT()
{
	nCount=14;
	nUnit[0]=D_UNITSYS_NONE;
	nUnit[1]=D_UNITSYS_NONE;
	nUnit[2]=D_UNITSYS_NONE;
	nUnit[3]=D_UNITSYS_NONE;
	nUnit[4]=D_UNITSYS_NONE;
	nUnit[5]=D_UNITSYS_NONE;
	nUnit[6]=D_UNITSYS_NONE;
	nUnit[7]=D_UNITSYS_NONE;
	nUnit[8]=D_UNITSYS_NONE;
	nUnit[9]=D_UNITSYS_NONE;
	nUnit[10]=D_UNITSYS_NONE;
	nUnit[11]=D_UNITSYS_NONE;
	nUnit[12]=D_UNITSYS_NONE;
	nUnit[13]=D_UNITSYS_NONE;
	bAutoChange=nUnit[0];
	bExponential=nUnit[1];
	nDimension=nUnit[2];
	nDisplacement=nUnit[3];
	nDimensionless=nUnit[4];
	nRotation=nUnit[5];
	nMass=nUnit[6];
	nWeight=nUnit[7];
	nForce=nUnit[8];
	nDistForce=nUnit[9];
	nMoment=nUnit[10];
	nDistMoment=nUnit[11];
	nStress=nUnit[12];
	nAcceleration=nUnit[13];
}

T_PREFER_DGNCODE_UNIT::T_PREFER_DGNCODE_UNIT()
{
	SteelCode = D_UNITSYS_NONE;
	ConcCode = D_UNITSYS_NONE;
	ConcRebarMatlCode = D_UNITSYS_NONE;
	ConcRebarMatlDB = D_UNITSYS_NONE;
	SRCCode = D_UNITSYS_NONE;
	SRCRebarMatlCode = D_UNITSYS_NONE;
	SRCRebarMatlDB = D_UNITSYS_NONE;
	RCCode = D_UNITSYS_NONE;
	RatingSTL = D_UNITSYS_NONE;
	RatingRC = D_UNITSYS_NONE;
	PSCCode = D_UNITSYS_NONE;
	PGCode = D_UNITSYS_NONE;
	StlOrthoDeck = D_UNITSYS_NONE;
	BrdgSeisEval = D_UNITSYS_NONE;
	OutputLanguage = D_UNITSYS_NONE;

	//(2009.06.17) Add by Unsang :: National Annex for EC2:04 / EC2-2:05
	nConcNationalAnnex = D_UNITSYS_NONE;    
	nPscNationalAnnex = D_UNITSYS_NONE;
	nStlNationalAnnex = D_UNITSYS_NONE;

	nPgNationalAnnex = D_UNITSYS_NONE;
	nStlReportLang = D_UNITSYS_NONE;
	nConcReportLang = D_UNITSYS_NONE;
	nPscReportLang = D_UNITSYS_NONE;
	nPgReportLang = D_UNITSYS_NONE;
}