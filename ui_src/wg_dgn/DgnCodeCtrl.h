#pragma once
#include "DgnStruct.h"
#include "../dgnengine/idesign/DGN_def/LocaleDef.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCodeCtrl
{
public:
	CDgnCodeCtrl(void);
	~CDgnCodeCtrl(void);

public:

#pragma region /// STEEL DESIGN CODE

	static int GetStlCodeID(const CString& strCode);
	static int GetStlMatlCodeID(const CString& strCode);
	static CString GetStlCodeName(int nCode);
	static dgn::def::TLocale GetStlLocalePref();

#pragma endregion

#pragma region /// CONCRETE DESIGN CODE

	static int GetConCodeID(const CString& strCode, BOOL bCivil = FALSE);
	static int GetCodeUnitLeng(const int nCode);
	static int GetCodeUnitForc(const int nCode);

	static bool IsConCodeUSD(const CString& strCode);
	static bool IsConCodeTorsionCheck(int nCode);
	static bool IsConCodeNotLimitCmGT04(int nCode);
	static bool IsConCodeLimit2ndMomentRatio14(int nCode);
	static bool IsConCodeNotColmSwaySlenGT35(int nCode);
	static bool IsConCodeServCheck(int nCode);
	static bool IsConCodeSlabForceWoodArmer(int nCode);
	static bool IsConCodePhiVariation(int nCode);
	static bool IsConCodeRhomax075(int nCode);
	static bool IsConCodeMnIMF4SSWB(int nCode);
	static bool IsConCode08DSectSR4Shear(int nCode);
	static bool IsConCodeSpecialWall(int nCode);
	static bool IsConCodeUseAlpha2ColmSSWB(const CString& strCode);
	static bool IsConCodeUseAlpha2ColmSSWB(int nCode);
	static bool	IsConCodeUseMeshedPM4DgnEngine(const CString& strCode);
	static bool IsConCodeMultiOutput(const int nCode);
	static bool IsConCodeGenSectColm(const int nCode);
	static bool IsConCodeRussia(const int nCode);
	static CString GetConCodeName(int nCode);
	static CString GetConCodeNumber4Log(const CString& strPrefix, const CString& strCode);
	static dgn::def::TLocale GetConLocalePref();

	static dgn::def::TLocale GetPscLocalePref();
	static int GetPscReportUnit();
	static int GetRcReportUnit();
	static int GetStlReportUnit();

#pragma endregion

#pragma region /// SRC DESIGN CODE

	static int GetSrcCodeID(const CString& strCode);
	static CString GetSrcCodeName(int nCode);

#pragma endregion

#pragma region /// COMPOSITE PLATE DESIGN CODE

	static BOOL GetDefaultCpgdFactor(int nCsgCodeNo, int nSubCode, T_CPGD_D& cpgdD);
	static dgn::def::TLocale GetCpgLocalePref();
	static int GetCpgReportUnit();

#pragma endregion

#pragma region /// COLD FORMED STEEL DESIGN CODE

	static int GetCfsCodeID(const CString& strCode);
	static CString GetCfsCodeName(int nCode);

#pragma endregion

#pragma region /// CHECK OPTION
	static bool DoPerformModuleConc(const CString& strCode);
	static bool DoPerformModuleStl(const CString& strCode);
	static bool DoPerformModuleSRC(const CString& strCode);
	static bool IsIDesignModule(int nCode);
	static bool IsDetailReportOnPerformModule(int nCode);
#pragma endregion

};

#include "HeaderPost.h"
