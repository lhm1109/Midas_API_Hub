#include "stdafx.h"
#include "SpfcMakeSpectrumUtil.h"

#include "MyChildDialog.h"

/*00*/ // USER
/*01*/#include "SpfcKSUtil.h"
/*02*/#include "SpfcKS2000Util.h"
/*03*/#include "SpfcKSBRGUtil.h"
/*04*/#include "SpfcUBC88Util.h"
/*05*/#include "SpfcUBC97Util.h"
/*06*/#include "SpfcEURO1996Util.h"
/*07*/#include "SpfcCH2002Util.h"
/*08*/#include "SpfcIBC2000Util.h"
/*09*/#include "SpfcJPN2000Util.h"
/*10*/#include "SpfcCHBRG89Util.h"

/*11*/#include "SpfcCHGBJ1187Util.h"
/*12*/#include "SpfcNBC95Util.h"
/*13*/#include "SpfcEuro1996ELAUtil.h"
/*14*/#include "SpfcIS2002Util.h"
/*15*/#include "SpfcTAIWAN99HUtil.h"
/*16*/#include "SpfcTAIWAN99VUtil.h"
/*17*/#include "SpfcTWBR89HUtil.h"
/*18*/#include "SpfcTWBR89VUtil.h"
/*19*/#include "SpfcJPBRG2002Util.h"
/*20*/#include "SpfcKBC2005Util.h"

/*21*/#include "SpfcCHSH2003Util.h"
/*22*/#include "SpfcEURO2004Util.h"
/*23*/#include "SpfcTW2022Util.h"
/*24*/#include "SpfcGB50111_2006Util.h"
/*25*/#include "SpfcCHJTJ2008Util.h"
/*26*/#include "SpfcKBC2009Util.h"
/*27*/ // USER
/*28*/#include "SpfcTAIWAN98Util.h"
/*29*/#include "SpfcCH2010Util.h"
/*30*/#include "SpfcCHCJJ2011Util.h"

/*31*/#include "SpfcIBC2012Util.h"
/*32*/ //#include "SpfcIBC2012Util.h"
/*33*/#include "SpfcAASHTOLRFD12Util.h"
/*34*/#include "SpfcNSR2010Util.h"
/*35*/#include "SpfcKBC2016Util.h"
/*36*/#include "SpfcPO2013Util.h"
/*37*/#include "SpfcIS2016Util.h"
/*38*/ //KISTEC2013 - 미사용
/*39*/#include "SpfcKSCE2019Util.h"
/*40*/#include "SpfcNTC2018Util.h"

/*41*/#include "SpfcPH2013Util.h"
/*42*/#include "SpfcKDS2019Util.h"
/*43*/#include "SpfcAU2017Util.h"
/*44*/#include "SpfcCH2019Util.h"
/*45*/#include "SpfcIRC2018Util.h"
/*46*/#include "SpfcCHJTG2020Util.h"
/*47*/#include "SpfcJPBRG2012Util.h"
/*48*/  //#include "SpfcJPBRG2012Util.h"
/*49*/#include "SpfcDPT2018Util.h"

/*50*/#include "SpfcCHGB2021Util.h"
/*51*/ //#include "SpfcTW2022Util.h
/*52*/#include "SpfcSP2016Util.h"
/*53*/ //#include "SpfcIBC2012Util.h"
/*55*/#include "SpfcSP14_2018Util.h"

bool CSpfcMakeSpectrumUtil::MakeSpectrumData(const T_SPFC_CODE& codeParam, int nSpecCodeType, CArray<double, double>& arPeriod, CArray<double, double>& arAccel) const
{
	std::unique_ptr<CSpfcBaseUtil> pSpfcUtil = std::move( GetSpfcUtilFromCode(nSpecCodeType) );

	if (pSpfcUtil == nullptr)
		return false;

	pSpfcUtil->SetParamFromCode(codeParam);
	pSpfcUtil->SetParamFromCalc();

	if (pSpfcUtil->CheckValid() == false)
		return false;

	pSpfcUtil->MakeSpectrumData();

	CString strDescription{};
	CString strFuncName{};
	SetSpfcResult(*pSpfcUtil.get(), arPeriod, arAccel, strDescription, strFuncName);

	return true;
}

std::unique_ptr<CSpfcBaseUtil> CSpfcMakeSpectrumUtil::GetSpfcUtilFromCode(int nSpecCodeType) const
{
	switch (nSpecCodeType)
	{
	case D_SPFC_CODE_USER:
		ASSERT(false);
		return nullptr;
	case D_SPFC_CODE_KS:
		return std::make_unique<CSpfcKSUtil>();
	case D_SPFC_CODE_KS2000:
		return std::make_unique<CSpfcKS2000Util>();
	case D_SPFC_CODE_KS_BRG:
		return std::make_unique<CSpfcKSBRGUtil>();
	case D_SPFC_CODE_UBC88:
		return std::make_unique<CSpfcUBC88Util>();
	case D_SPFC_CODE_UBC97:
		return std::make_unique<CSpfcUBC97Util>();
	case D_SPFC_CODE_EURO1996:
		return std::make_unique<CSpfcEURO1996Util>();
	case D_SPFC_CODE_CH2002:
		return std::make_unique<CSpfcCH2002Util>();
	case D_SPFC_CODE_IBC2000:
		return std::make_unique<CSpfcIBC2000Util>();
	case D_SPFC_CODE_JPN2000:
		return std::make_unique<CSpfcJPN2000Util>();
	case D_SPFC_CODE_CH_BRG89:
		return std::make_unique<CSpfcCHBRG89Util>();
	case D_SPFC_CODE_CH_GBJ111_87:
		return std::make_unique<CSpfcCHGBJ1187Util>();
	case D_SPFC_CODE_NBC95:
		return std::make_unique<CSpfcNBC95Util>();
	case D_SPFC_CODE_EURO1996_ELA:
		return std::make_unique<CSpfcEURO1996ELAUtil>();
	case D_SPFC_CODE_IS2002:
		return std::make_unique<CSpfcIS2002Util>();
	case D_SPFC_CODE_TAIWAN99H:
		return std::make_unique<CSpfcTAIWAN99HUtil>();
	case D_SPFC_CODE_TAIWAN99V:
		return std::make_unique<CSpfcTAIWAN99VUtil>();
	case D_SPFC_CODE_TAIWAN89H_BRG:
		return std::make_unique<CSpfcTWBR89HUtil>();
	case D_SPFC_CODE_TAIWAN89V_BRG:
		return std::make_unique<CSpfcTWBR89VUtil>();;
	case D_SPFC_CODE_JP_BRG2002:
		return std::make_unique<CSpfcJPBRG2002Util>();
	case D_SPFC_CODE_KBC2005:
		return std::make_unique<CSpfcKBC2005Util>();
	case D_SPFC_CODE_CHSH2003:
		return std::make_unique<CSpfcCHSH2003Util>();
	case D_SPFC_CODE_EURO2004:
		return std::make_unique<CSpfcEURO2004Util>();
	case D_SPFC_CODE_TAIWAN06:
		return std::make_unique<CSpfcTW2006Util>();
	case D_SPFC_CODE_GB50111_2006:
		return std::make_unique<CSpfcGB50111_2006Util>();
	case D_SPFC_CODE_JTJXXX_2008:
		return std::make_unique<CSpfcCHJTJ2008Util>();
	case D_SPFC_CODE_KBC2009:
		return std::make_unique<CSpfcKBC2009Util>();
	case D_SPFC_CODE_USER_TYPE:
		ASSERT(false);
		return nullptr;
	case D_SPFC_CODE_TAIWANBRG98:
		return std::make_unique<CSpfcTAIWAN98Util>();
	case D_SPFC_CODE_CH2010:
		return std::make_unique<CSpfcCH2010Util>();
	case D_SPFC_CODE_CJJ166_2011:
		return std::make_unique<CSpfcCHCJJ2011Util>();
	case D_SPFC_CODE_IBC2009:
		return std::make_unique<CSpfcIBC2012Util>(D_SPFC_CODE_IBC2009);
	case D_SPFC_CODE_IBC2012:
		return std::make_unique<CSpfcIBC2012Util>(D_SPFC_CODE_IBC2012);
	case D_SPFC_CODE_AASHTO_LRFD12:
		return std::make_unique<CSpfcAASHTOLRFD12Util>();
	case D_SPFC_CODE_NSR2010:
		return std::make_unique<CSpfcNSR2010Util>();
	case D_SPFC_CODE_KBC2015:
		return std::make_unique<CSpfcKBC2016Util>();
	case D_SPFC_CODE_P100_2013:
		return std::make_unique<CSpfcPO2013Util>();
	case D_SPFC_CODE_IS1893_2016:
		return std::make_unique<CSpfcIS2016Util>();
	case D_SPFC_CODE_KISTEC2013:
		ASSERT(false);
		break;
	case D_SPFC_CODE_KSCE2019:
		return std::make_unique<CSpfcKSCE2019Util>();
	case D_SPFC_CODE_NTC2018:
		return std::make_unique<CSpfcNTC2018Util>();
	case D_SPFC_CODE_DPWH_LRFD_2013:
		return std::make_unique<CSpfcPH2013Util>();
	case D_SPFC_CODE_KDS2019:
		return std::make_unique<CSpfcKDS2019Util>();
	case D_SPFC_CODE_AS_5100_2_2017:
		return std::make_unique<CSpfcAU2017Util>();
	case D_SPFC_CODE_CH2019:
		return std::make_unique<CSpfcCH2019Util>();
	case D_SPFC_CODE_IRCSP114_2018:
		return std::make_unique<CSpfcIRC2018Util>();
	case D_SPFC_CODE_JTG_T_2231_01_2020:
		return std::make_unique<CSpfcCHJTG2020Util>();
	case D_SPFC_CODE_JP_BRG2012:
		return std::make_unique<CSpfcJPBRG2012Util>(0);
	case D_SPFC_CODE_JP_BRG2017:
		return std::make_unique<CSpfcJPBRG2012Util>(1);
	case D_SPFC_CODE_DPT2018:
		return std::make_unique<CSpfcDPT2018Util>();
	case D_SPFC_CODE_CH_GB_T_51408_2021:
		return std::make_unique<CSpfcCHGB2021Util>();
	case D_SPFC_CODE_TAIWAN22:
		return std::make_unique<CSpfcTW2022Util>();
	case D_SPFC_CODE_RUS_SP2016:
		return std::make_unique<CSpfcSP2016Util>();
	case D_SPFC_CODE_RUS_SP14_2018:
		return std::make_unique<CSpfcSP14_2018Util>();
	case D_SPFC_CODE_NSCP_2024:
		return std::make_unique<CSpfcIBC2012Util>(D_SPFC_CODE_NSCP_2024);

	default:
		ASSERT(false);
		return nullptr;
	}

	ASSERT(false);
	return nullptr;
}

void CSpfcMakeSpectrumUtil::SetSpfcResult(const CSpfcBaseUtil& spfcUtil, CArray<double, double>& arPeriod, CArray<double, double>& arAccel, CString& strDescription, CString& strFuncName) const
{
	const auto& vecPeriod = spfcUtil.GetPeriod();
	const auto& vecAccel = spfcUtil.GetAccel();

	for (const auto& period : vecPeriod)
		arPeriod.Add(period);

	for (const auto& accel : vecAccel)
		arAccel.Add(accel);

	strFuncName = spfcUtil.GetFuncName().data();
	strDescription = spfcUtil.GetDescript().data();
}
