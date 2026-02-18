// CRCForceCtrl.h: interface for the CCRCForceCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CRCFORCECTRL_H__)
#define __CRCFORCECTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\BnFile.h"
#include "..\wg_db\wg_db_SectInfo.h"
//#include "..\wg_db\DB_ST_DT_ELEM.h"

#include "DgnStruct.h"
#include "DgnForceStressST.h"

struct T_STRSPOST_BASE;
struct T_STRSPROP_BASE;
struct T_PSCD_FORCE;

class CCRCDataCtrl;
class CDesignForceResult;
class CDgnForceCtrl;

class CDBDoc;
class CAttrCtrl;
class CAttrCtrl2;
class CUnitCtrl;
class CPostCtrl;
class CSectInfo;
class CStageInfo;
class CResultDataFilePosition;

struct T_STRSPOST_D;

#include "HeaderPre.h"

//const int cFORC_CON = 12;	// Concurrent Force (Fxmax, Fxmin,...,Mzmax,Mzmin).
enum enService
{
    None = 0,
    Service1 = 1,
    Service3 = 3,
    End,
};

struct T_LCOM_IN
{
	ElemPairK ElemK;
	BOOL bBinary;
	int nForType;
	BOOL bIncludeTP;
	int iDgnLcomKind;
	T_SECT_D SectD;
	T_RPSC_D RpscMbar;
	T_RPSC_D RpscSbar;	
	T_STRSPROP_D StrsPropD;
	BOOL bIsCalcStss;
	
	int nCompareType;
	_DGN_LCOM LcomDesign;	

	double dFactor;
	unsigned int iLcaseK;
	int iMaxMinType;
	BOOL bReciLcase;
};

struct T_LCOM_FORC
{
	_DGN_FORC_CRC ForceRemn;
	_DGN_STRS_CRC StressRemn;
	_DGN_SHRS_ADD StressShr;
	_DGN_FORC_CRC ForceRemnP1;
	_DGN_FORC_CRC ForceRemnP2;
	_DGN_STRS_CRC StressRemnP1;
	_DGN_STRS_CRC StressRemnP2;
	_DGN_STRS_ADD StressAdd;

	_DGN_SCFP_PART_LCASE StressScfpTe;
	_DGN_SCFP_PART_LCASE StressScfpRe;
	_DGN_FORC_CRC ForceScfpSum;
	_DGN_FORC_CRC ForceAddDL;
	_DGN_STRS_CRC StressNorDL;
	_DGN_STRS_CRC StressAddDL;
	_DGN_FORC_CRC ForceDrec;
	_DGN_FORC_CRC ForceDnon;
	_DGN_FORC_CRC ForceLTerm;
	_DGN_STRS_CRC StressLTerm, StressLTermP2;
	_DGN_FORC_CRC ForceLrec;
	_DGN_FORC_CRC ForceLnon;
	_DGN_STRS_CRC StressNorLL;
	_DGN_STRS_CRC StressSTermP2;
	_DGN_FORC_CRC ForceLL;
	_DGN_STRS_CRC StressJtgLL;
	_DGN_FORC_CRC ForceJtgTndn;
	_DGN_FORC_CRC ForceDL_TB05;
	_DGN_STRS_CRC StressDL_TB05;
	_DGN_FORC_CRC ForceLL_TB05;
	_DGN_STRS_CRC StressLL_TB05;
	_DGN_FORC_CRC ForceEQE2_JTG;
	_DGN_FORC_CRC ForceWeight_JTG;
	_DGN_FORC_CRC ForceSgldRe;
	_DGN_STRS_CRC StressCS_TP_TB05;
	_DGN_STRS_CRC StressSgldRe;
	_DGN_FORC_CRC ForceCS_TP_TB05;
	_DGN_STRS_CRC StressTnpr;
	_DGN_STRS_CRC StressTnsc;
	_DGN_FORC_CRC ForceStag;
	_DGN_SHRS_ADD ShrsStage;
	_DGN_FORC_CRC ForceTnpr, ForceTnsc;

	_DGN_SCFP_PART_LCASE StressScfpCr; // Creep
	_DGN_SCFP_PART_LCASE StressScfpSh; // Shrinkage
	_DGN_STRS_CRC StressTnpr2;
	_DGN_STRS_CRC StressTnsc2; // Slab(Part 2)
	_DGN_FORC_CRC ForceScfpCrp;
	_DGN_FORC_CRC ForceScfpShr;

	void Init()
	{
		ForceRemn.Initialize();
		StressRemn.Initialize();
		StressShr.Initialize();
		ForceRemnP1.Initialize();
		ForceRemnP2.Initialize();
		StressRemnP1.Initialize();
		StressRemnP2.Initialize();
		StressAdd.Initialize();
		StressScfpTe.Initialize();
		StressScfpRe.Initialize();
		ForceScfpSum.Initialize();
		ForceAddDL.Initialize();
		StressNorDL.Initialize();
		StressAddDL.Initialize();
		ForceDrec.Initialize();
		ForceDnon.Initialize();
		ForceLTerm.Initialize();
		StressLTerm.Initialize();
		StressLTermP2.Initialize();
		ForceLrec.Initialize();
		ForceLnon.Initialize();
		StressNorLL.Initialize();
		StressSTermP2.Initialize();
		ForceLL.Initialize();
		StressJtgLL.Initialize();
		ForceJtgTndn.Initialize();
		ForceDL_TB05.Initialize();
		StressDL_TB05.Initialize();
		ForceLL_TB05.Initialize();
		StressLL_TB05.Initialize();
		ForceEQE2_JTG.Initialize();
		ForceWeight_JTG.Initialize();
		ForceSgldRe.Initialize();
		StressCS_TP_TB05.Initialize();
		StressSgldRe.Initialize();
		ForceCS_TP_TB05.Initialize();
		StressTnpr.Initialize();
		StressTnsc.Initialize();
		ForceStag.Initialize();
		ShrsStage.Initialize();
		ForceTnpr.Initialize();
		ForceTnsc.Initialize();
		StressScfpCr.Initialize();
		StressScfpSh.Initialize();
		StressTnpr2.Initialize();
		StressTnsc2.Initialize();
		ForceScfpCrp.Initialize();
		ForceScfpShr.Initialize();
	}
};
struct T_LCOM_OUT
{
	_DGN_FORC_CRC ForceLcom;
	_DGN_FORC_CRC ForceLcomExTp;
	_DGN_FORC_CRC ForceStag;
	_DGN_SHRS_ADD ShrsStage;
	_DGN_FORC_CRC ForceTnpr;
	_DGN_FORC_CRC ForceTnsc;
	_DGN_STRS_ADD StressAdd;
	_DGN_FORC_CRC ForceLTerm;
	_DGN_FORC_CRC ForceLL;
	_DGN_FORC_CRC ForceJtgTndn;

	_DGN_STRS_CRC StressLcom;
	_DGN_STRS_CRC StressStag;
	_DGN_STRS_CRC StressTnpr;
	_DGN_STRS_CRC StressTnsc; 
	_DGN_STRS_CRC StressLTerm;
	_DGN_SHRS_ADD StssLcom;
	_DGN_STRS_CRC StressLL;

	_DGN_SCFP_PART_LCASE StrScfpCr;
	_DGN_SCFP_PART_LCASE StrScfpSh;
	_DGN_SCFP_PART_LCASE StrScfpTe;
	_DGN_SCFP_PART_LCASE StrScfpRe;
	_DGN_FORC_CRC        ForceScfpSum;
	_DGN_FORC_CRC        ForceScfpCrp;
	_DGN_FORC_CRC        ForceScfpShr;

	_DGN_FORC_CRC ForceLcomPart1;
	_DGN_STRS_CRC StressLcomPart1;
	_DGN_FORC_CRC ForceLcomPart2;
	_DGN_STRS_CRC StressLcomPart2;
	_DGN_STRS_CRC StressLTermPart2;
	_DGN_STRS_CRC StressSTermPart2;
	_DGN_STRS_CRC StrTnprPa2;
	_DGN_STRS_CRC StrTnscPa2;

	_DGN_STRS_CRC StressDL_TB05;
	_DGN_STRS_CRC StressLL_TB05;
	_DGN_STRS_CRC StressCSTP_TB05;

	_DGN_FORC_CRC ForceDL_TB05;
	_DGN_FORC_CRC ForceLL_TB05;
	_DGN_FORC_CRC ForceCSTP_TB05;
	_DGN_FORC_CRC ForceEQE2_JTG;
	_DGN_FORC_CRC ForceWeight_JTG;

	void Init()
	{
		ForceLcom.Initialize();
		ForceLcomExTp.Initialize();
		ForceStag.Initialize();
		ShrsStage.Initialize();
		ForceTnpr.Initialize();
		ForceTnsc.Initialize();
		StressAdd.Initialize();
		ForceLTerm.Initialize();
		ForceLL.Initialize();
		ForceJtgTndn.Initialize();
		StressLcom.Initialize();
		StressStag.Initialize();
		StressTnpr.Initialize();
		StressTnsc.Initialize();
		StressLTerm.Initialize();
		StssLcom.Initialize();
		StressLL.Initialize();
		StrScfpCr.Initialize();
		StrScfpSh.Initialize();
		StrScfpTe.Initialize();
		StrScfpRe.Initialize();		
		ForceScfpSum.Initialize();
		ForceScfpCrp.Initialize();
		ForceScfpShr.Initialize();
		ForceLcomPart1.Initialize();
		StressLcomPart1.Initialize();
		ForceLcomPart2.Initialize();
		StressLcomPart2.Initialize();
		StressLTermPart2.Initialize();
		StressSTermPart2.Initialize();
		StrTnprPa2.Initialize();
		StrTnscPa2.Initialize();
		StressDL_TB05.Initialize();
		StressLL_TB05.Initialize();
		StressCSTP_TB05.Initialize();
		ForceDL_TB05.Initialize();
		ForceLL_TB05.Initialize();
		ForceCSTP_TB05.Initialize();
		ForceEQE2_JTG.Initialize();
		ForceWeight_JTG.Initialize();
	}
};
struct T_LCOM_EX_FORC
{
	_DGN_FORC_CRC_EX ForceDrec_EX;
	_DGN_FORC_CRC_EX ForceLrec_EX;	// For Reciprocal Stress.
	_DGN_FORC_CRC_EX ForceDnon_EX;
	_DGN_FORC_CRC_EX ForceLnon_EX;
	_DGN_FORC_CRC_EX ForceRemn_EX;
	_DGN_FORC_CRC_EX ForceStag_EX;
	_DGN_SHRS_ADD_EX ShrsStage_EX;							// Principal Stress at Stage.
	_DGN_FORC_CRC_EX ForceTnpr_EX;
	_DGN_FORC_CRC_EX ForceTnsc_EX;	// Tendon (Primary, Secondary).
	_DGN_STRS_ADD_EX StressAdd_EX;	
	_DGN_FORC_CRC_EX ForceSgldRe_EX; // For Sgld Remn
	_DGN_FORC_CRC_EX ForceAddDL_EX;  // For Add DL , JTG D6204, 
	_DGN_FORC_CRC_EX ForceLL_EX;     // For 균열검토, JTG D62-04, LL
	_DGN_FORC_CRC_EX ForceLTerm_EX;  // For 균열검토, JTG D62-04, 하중장기효응조합.
	_DGN_FORC_CRC_EX ForceJtgTndn_EX; // For Tendon Stress, JTG D62-04
	_DGN_FORC_CRC_EX ForceDL_TB05_EX;
	_DGN_FORC_CRC_EX ForceLL_TB05_EX;
	_DGN_FORC_CRC_EX ForceCS_TP_TB05_EX;
	_DGN_FORC_CRC_EX ForceEQE2_JTG_EX;
	_DGN_FORC_CRC_EX ForceWeight_JTG_EX;
	_DGN_STRS_CRC_EX StressNorDL_EX;
	_DGN_STRS_CRC_EX StressNorLL_EX;
	_DGN_STRS_CRC_EX StressRemn_EX;
	_DGN_STRS_CRC_EX StressTnpr_EX;
	_DGN_STRS_CRC_EX StressTnsc_EX;
	_DGN_STRS_CRC_EX StressTnpr2_EX;
	_DGN_STRS_CRC_EX StressTnsc2_EX; // Slab(Part 2)
	_DGN_STRS_CRC_EX StressSgldRe_EX;
	_DGN_STRS_CRC_EX StressAddDL_EX;
	_DGN_STRS_CRC_EX StressJtgLL_EX;
	_DGN_STRS_CRC_EX StressLTerm_EX;
	_DGN_STRS_CRC_EX StressDL_TB05_EX;
	_DGN_STRS_CRC_EX StressLL_TB05_EX;
	_DGN_STRS_CRC_EX StressCS_TP_TB05_EX;
	_DGN_SHRS_ADD_EX StressShr_EX;
	_DGN_FORC_CRC_EX ForceRemnP1_EX;
	_DGN_STRS_CRC_EX StressRemnP1_EX;
	_DGN_FORC_CRC_EX ForceRemnP2_EX;
	_DGN_STRS_CRC_EX StressRemnP2_EX;
	_DGN_SCFP_PART_LCASE_EX StressScfpCr_EX; // Creep
	_DGN_SCFP_PART_LCASE_EX StressScfpSh_EX; // Shrinkage
	_DGN_SCFP_PART_LCASE_EX StressScfpTe_EX; // Temperature
	_DGN_SCFP_PART_LCASE_EX StressScfpRe_EX; // Remain
	_DGN_FORC_CRC_EX       ForceScfpSum_EX; // 내부 구속 응력에 해당하는 Force 합
	_DGN_FORC_CRC_EX       ForceScfpCrp_EX; // 내부 구속 응력에 해당하는 Creep Force 
	_DGN_FORC_CRC_EX       ForceScfpShr_EX; // 내부 구속 응력에 해당하는 Shrinkage Force 

	void Init()
	{
		ForceDrec_EX.Initialize();
		ForceLrec_EX.Initialize();
		ForceDnon_EX.Initialize();
		ForceLnon_EX.Initialize();
		ForceRemn_EX.Initialize();
		ForceStag_EX.Initialize();
		ShrsStage_EX.Initialize();
		ForceTnpr_EX.Initialize();
		ForceTnsc_EX.Initialize();
		StressAdd_EX.Initialize();
		ForceSgldRe_EX.Initialize();
		ForceAddDL_EX.Initialize();
		ForceLL_EX.Initialize();
		ForceLTerm_EX.Initialize();
		ForceJtgTndn_EX.Initialize();
		ForceDL_TB05_EX.Initialize();
		ForceLL_TB05_EX.Initialize();
		ForceCS_TP_TB05_EX.Initialize();
		ForceEQE2_JTG_EX.Initialize();
		ForceWeight_JTG_EX.Initialize();
		StressNorDL_EX.Initialize();
		StressNorLL_EX.Initialize();
		StressRemn_EX.Initialize();
		StressTnpr_EX.Initialize();
		StressTnsc_EX.Initialize();
		StressTnpr2_EX.Initialize();
		StressTnsc2_EX.Initialize();
		StressSgldRe_EX.Initialize();
		StressAddDL_EX.Initialize();
		StressJtgLL_EX.Initialize();
		StressLTerm_EX.Initialize();
		StressDL_TB05_EX.Initialize();
		StressLL_TB05_EX.Initialize();
		StressCS_TP_TB05_EX.Initialize();
		StressShr_EX.Initialize();
		ForceRemnP1_EX.Initialize();
		StressRemnP1_EX.Initialize();
		ForceRemnP2_EX.Initialize();
		StressRemnP2_EX.Initialize();
		StressScfpCr_EX.Initialize();
		StressScfpSh_EX.Initialize();
		StressScfpTe_EX.Initialize();
		StressScfpRe_EX.Initialize();
		ForceScfpSum_EX.Initialize();
		ForceScfpCrp_EX.Initialize();
		ForceScfpShr_EX.Initialize();
	}
};
struct T_LCOM_EX_OUT
{
	_DGN_FORC_CRC_EX ForceLcom;
	_DGN_FORC_CRC_EX ForceLcomExTp;
	_DGN_FORC_CRC_EX ForceStag;
	_DGN_SHRS_ADD_EX ShrsStage;
	_DGN_FORC_CRC_EX ForceTnpr;
	_DGN_FORC_CRC_EX ForceTnsc;
	_DGN_STRS_ADD_EX StressAdd;
	_DGN_FORC_CRC_EX ForceLTerm;
	_DGN_FORC_CRC_EX ForceJtgTndn;
	//
	_DGN_STRS_CRC_EX StressLcom;
	_DGN_STRS_CRC_EX StressStag;
	_DGN_STRS_CRC_EX StressTnpr;
	_DGN_STRS_CRC_EX StressTnsc;
	_DGN_STRS_CRC_EX StressLTerm;
	_DGN_SHRS_ADD_EX StssLcom;
	//
	_DGN_STRS_CRC_EX StressDL_TB05;
	_DGN_STRS_CRC_EX StressLL_TB05;
	_DGN_STRS_CRC_EX StressCSTP_TB05;

	_DGN_SCFP_PART_LCASE_EX StrScfpCr;
	_DGN_SCFP_PART_LCASE_EX StrScfpSh;
	_DGN_SCFP_PART_LCASE_EX StrScfpTe;
	_DGN_SCFP_PART_LCASE_EX StrScfpRe;
	_DGN_FORC_CRC_EX ForceScfpSum_EX;
	_DGN_FORC_CRC_EX ForceScfpCrp_EX;
	_DGN_FORC_CRC_EX ForceScfpShr_EX;

	_DGN_FORC_CRC_EX ForceLcomPart1;
	_DGN_STRS_CRC_EX StressLcomPart1;
	_DGN_FORC_CRC_EX ForceLcomPart2;
	_DGN_STRS_CRC_EX StressLcomPart2;
	_DGN_STRS_CRC_EX StrTnprPa2;
	_DGN_STRS_CRC_EX StrTnscPa2;;
	
	_DGN_FORC_CRC_EX ForceDL_TB05;
	_DGN_FORC_CRC_EX ForceLL_TB05;
	_DGN_FORC_CRC_EX ForceCSTP_TB05;
	_DGN_FORC_CRC_EX ForceEQE2_JTG;
	_DGN_FORC_CRC_EX ForceWeight_JTG;

	void Init()
	{
		ForceLcom.Initialize();
		ForceLcomExTp.Initialize();
		ForceStag.Initialize();
		ShrsStage.Initialize();
		ForceTnpr.Initialize();
		ForceTnsc.Initialize();
		StressAdd.Initialize();
		ForceLTerm.Initialize();
		ForceJtgTndn.Initialize();
		StressLcom.Initialize();
		StressStag.Initialize();
		StressTnpr.Initialize();
		StressTnsc.Initialize();
		StressLTerm.Initialize();
		StssLcom.Initialize();
		StrScfpCr.Initialize();
		StrScfpSh.Initialize();
		StrScfpTe.Initialize();
		StrScfpRe.Initialize();
		ForceLcomPart1.Initialize();
		StressLcomPart1.Initialize();
		ForceLcomPart2.Initialize();
		StressLcomPart2.Initialize();
		StrTnprPa2.Initialize();
		StrTnscPa2.Initialize();
		StressDL_TB05.Initialize();
		StressLL_TB05.Initialize();
		StressCSTP_TB05.Initialize();
		ForceDL_TB05.Initialize();
		ForceLL_TB05.Initialize();
		ForceCSTP_TB05.Initialize();
		ForceEQE2_JTG.Initialize();
		ForceWeight_JTG.Initialize();
	}
};

class __MY_EXT_CLASS__ CCRCForceCtrl
{  
// Member Variables.
protected:
	int m_iDgnCode;		// 1=JSCE02, 2=KSCE-USD03, 3=JTG D62-04.
	int m_iDimType;		// 0=2D, 1=2D+Torsion, 2=3D
	int m_iBrgType;		// 0=Fully, 1=Partially(A), 2=Partially(B).
	int m_iCastType;	// 0=Precast, 1=Cast-in-Place.
	int m_iLcomNoStrn, m_iLcomNoStrs;				// For Strength, Stress.
	int m_iLcomNoSLS;
    int m_nLcomNoTran;
	_DGN_FORC_CRC m_ForceDrec, m_ForceLrec;	// For Reciprocal Stress.
	_DGN_FORC_CRC m_ForceDnon, m_ForceLnon;
	_DGN_FORC_CRC m_ForceRemn;
	_DGN_FORC_CRC m_ForceStag;
	_DGN_SHRS_ADD m_ShrsStage;							// Principal Stress at Stage.
	_DGN_FORC_CRC m_ForceTnpr, m_ForceTnsc;	// Tendon (Primary, Secondary).
	_DGN_STRS_ADD m_StressAdd;							// For Beam Section Temperature (Btss, Btms).
	// Add by Jaeoh. ('05.08.10).
	_DGN_FORC_CRC m_ForceExLrec, m_ForceExLnon;
	_DGN_FORC_CRC m_ForceServ;

	// Add by Jaeoh. ('05.08.16)
	_DGN_FORC_CRC m_ForceSgldRe; // For Sgld Remn
	_DGN_FORC_CRC m_ForceAddDL;  // For Add DL , JTG D6204, 
	_DGN_FORC_CRC m_ForceLL;     // For 균열검토, JTG D62-04, LL
	_DGN_FORC_CRC m_ForceLTerm;  // For 균열검토, JTG D62-04, 하중장기효응조합.
	// Add by Jaeoh. ('06.09.18)
	_DGN_FORC_CRC m_ForceJtgTndn; // For Tendon Stress, JTG D62-04
	// Add, Jaeoh. ('07.04.23) // For TB 10002.3-2005
	_DGN_FORC_CRC m_ForceDL_TB05;
	_DGN_FORC_CRC m_ForceLL_TB05;
	// Add, Jaeoh. [12/23/2008] Maxiao 요구 
	_DGN_FORC_CRC m_ForceCS_TP_TB05;
	// Add, bjShin. [07/06/2009] Maxiao 요구 
	_DGN_FORC_CRC m_ForceEQE2_JTG;
	_DGN_FORC_CRC m_ForceWeight_JTG;
	
	// Add by Jaeoh. ('05.08.17)
	_DGN_STRS_CRC m_StressNorDL;
	_DGN_STRS_CRC m_StressNorLL;
	_DGN_STRS_CRC m_StressRemn;
	_DGN_STRS_CRC m_StressTnpr, m_StressTnsc;
	_DGN_STRS_CRC m_StressTnpr2, m_StressTnsc2; // Slab(Part 2)

	_DGN_STRS_CRC m_StressSgldRe;
	_DGN_STRS_CRC m_StressAddDL;
	_DGN_STRS_CRC m_StressJtgLL;
	_DGN_STRS_CRC m_StressLTerm;
	// Add, Jaeoh. ('07.04.23) // For TB 10002.3-2005
	_DGN_STRS_CRC m_StressDL_TB05;
	_DGN_STRS_CRC m_StressLL_TB05;
	// Add, Jaeoh. [12/23/2008] Maxiao 요구 
	_DGN_STRS_CRC m_StressCS_TP_TB05;
	
	// Principle.
	//_DGN_SHRS_CRC m_StssStage;
	_DGN_SHRS_ADD m_StressShr;

	// Composite Part Force
	_DGN_FORC_CRC m_ForceDrecP1, m_ForceLrecP1;	// For Reciprocal Stress.
	_DGN_FORC_CRC m_ForceDnonP1, m_ForceLnonP1;
	_DGN_FORC_CRC m_ForceRemnP1;
	_DGN_FORC_CRC m_ForceStagP1;
	_DGN_STRS_CRC m_StressRemnP1;
	//
	_DGN_FORC_CRC m_ForceDrecP2, m_ForceLrecP2;	// For Reciprocal Stress.
	_DGN_FORC_CRC m_ForceDnonP2, m_ForceLnonP2;
	_DGN_FORC_CRC m_ForceRemnP2;
	_DGN_FORC_CRC m_ForceStagP2;
	_DGN_STRS_CRC m_StressRemnP2;
    _DGN_STRS_CRC m_StressLTermP2;
    _DGN_STRS_CRC m_StressSTermP2;
	// 
	_DGN_SCFP_PART_LCASE m_StressScfpCr; // Creep
	_DGN_SCFP_PART_LCASE m_StressScfpSh; // Shrinkage
	_DGN_SCFP_PART_LCASE m_StressScfpTe; // Temperature
	_DGN_SCFP_PART_LCASE m_StressScfpRe; // Remain
	_DGN_FORC_CRC        m_ForceScfpSum; // 내부 구속 응력에 해당하는 Force 합
	_DGN_FORC_CRC        m_ForceScfpCrp; // 내부 구속 응력에 해당하는 Creep Force 
	_DGN_FORC_CRC        m_ForceScfpShr; // 내부 구속 응력에 해당하는 Shrinkage Force 

	// 5 Position Expansion
	_DGN_FORC_CRC_EX* m_pForceDrec_EX;
	_DGN_FORC_CRC_EX* m_pForceLrec_EX;	// For Reciprocal Stress.
	_DGN_FORC_CRC_EX* m_pForceDnon_EX;
	_DGN_FORC_CRC_EX* m_pForceLnon_EX;
	_DGN_FORC_CRC_EX* m_pForceRemn_EX;
	_DGN_FORC_CRC_EX* m_pForceStag_EX;
	_DGN_SHRS_ADD_EX* m_pShrsStage_EX;							// Principal Stress at Stage.
	_DGN_FORC_CRC_EX* m_pForceTnpr_EX;
	_DGN_FORC_CRC_EX* m_pForceTnsc_EX;	// Tendon (Primary, Secondary).
	_DGN_STRS_ADD_EX* m_pStressAdd_EX;							// For Beam Section Temperature (Btss, Btms).
	_DGN_FORC_CRC_EX* m_pForceExLrec_EX;
	_DGN_FORC_CRC_EX* m_pForceExLnon_EX;
	_DGN_FORC_CRC_EX* m_pForceServ_EX;
	_DGN_FORC_CRC_EX* m_pForceSgldRe_EX; // For Sgld Remn
	_DGN_FORC_CRC_EX* m_pForceAddDL_EX;  // For Add DL , JTG D6204, 
	_DGN_FORC_CRC_EX* m_pForceLL_EX;     // For 균열검토, JTG D62-04, LL
	_DGN_FORC_CRC_EX* m_pForceLTerm_EX;  // For 균열검토, JTG D62-04, 하중장기효응조합.
	_DGN_FORC_CRC_EX* m_pForceJtgTndn_EX; // For Tendon Stress, JTG D62-04
	_DGN_FORC_CRC_EX* m_pForceDL_TB05_EX;
	_DGN_FORC_CRC_EX* m_pForceLL_TB05_EX;
	_DGN_FORC_CRC_EX* m_pForceCS_TP_TB05_EX;
	_DGN_FORC_CRC_EX* m_pForceEQE2_JTG_EX;
	_DGN_FORC_CRC_EX* m_pForceWeight_JTG_EX;
	_DGN_STRS_CRC_EX* m_pStressNorDL_EX;
	_DGN_STRS_CRC_EX* m_pStressNorLL_EX;
	_DGN_STRS_CRC_EX* m_pStressRemn_EX;
	_DGN_STRS_CRC_EX* m_pStressTnpr_EX;
	_DGN_STRS_CRC_EX* m_pStressTnsc_EX;
	_DGN_STRS_CRC_EX* m_pStressTnpr2_EX;
	_DGN_STRS_CRC_EX* m_pStressTnsc2_EX; // Slab(Part 2)
	_DGN_STRS_CRC_EX* m_pStressSgldRe_EX;
	_DGN_STRS_CRC_EX* m_pStressAddDL_EX;
	_DGN_STRS_CRC_EX* m_pStressJtgLL_EX;
	_DGN_STRS_CRC_EX* m_pStressLTerm_EX;
	_DGN_STRS_CRC_EX* m_pStressDL_TB05_EX;
	_DGN_STRS_CRC_EX* m_pStressLL_TB05_EX;
	_DGN_STRS_CRC_EX* m_pStressCS_TP_TB05_EX;
	_DGN_SHRS_ADD_EX* m_pStressShr_EX;
	_DGN_FORC_CRC_EX* m_pForceDrecP1_EX;
	_DGN_FORC_CRC_EX* m_pForceLrecP1_EX;	// For Reciprocal Stress.
	_DGN_FORC_CRC_EX* m_pForceDnonP1_EX;
	_DGN_FORC_CRC_EX* m_pForceLnonP1_EX;
	_DGN_FORC_CRC_EX* m_pForceRemnP1_EX;
	_DGN_FORC_CRC_EX* m_pForceStagP1_EX;
	_DGN_STRS_CRC_EX* m_pStressRemnP1_EX;
	_DGN_FORC_CRC_EX* m_pForceDrecP2_EX;
	_DGN_FORC_CRC_EX* m_pForceLrecP2_EX;	// For Reciprocal Stress.
	_DGN_FORC_CRC_EX* m_pForceDnonP2_EX;
	_DGN_FORC_CRC_EX* m_pForceLnonP2_EX;
	_DGN_FORC_CRC_EX* m_pForceRemnP2_EX;
	_DGN_FORC_CRC_EX* m_pForceStagP2_EX;
	_DGN_STRS_CRC_EX* m_pStressRemnP2_EX;
	_DGN_SCFP_PART_LCASE_EX* m_pStressScfpCr_EX; // Creep
	_DGN_SCFP_PART_LCASE_EX* m_pStressScfpSh_EX; // Shrinkage
	_DGN_SCFP_PART_LCASE_EX* m_pStressScfpTe_EX; // Temperature
	_DGN_SCFP_PART_LCASE_EX* m_pStressScfpRe_EX; // Remain
	_DGN_FORC_CRC_EX*        m_pForceScfpSum_EX; // 내부 구속 응력에 해당하는 Force 합
	_DGN_FORC_CRC_EX*        m_pForceScfpCrp_EX; // 내부 구속 응력에 해당하는 Creep Force 
	_DGN_FORC_CRC_EX*        m_pForceScfpShr_EX; // 내부 구속 응력에 해당하는 Shrinkage Force 
 
	BOOL m_bImportForce;

	double m_dRatNoEffectZone[2];	// 0=I,1=J.
	CMap<T_LCOM_K,T_LCOM_K,_DGN_LCOM,_DGN_LCOM&> m_arLcomData;  
	CMap<T_BLRC_K,T_BLRC_K,BLRD_RCASE,BLRD_RCASE&> m_arServBlrcData;
	CMap<T_BLRC_K,T_BLRC_K,BLRD_RCASE,BLRD_RCASE&> m_arStreBlrcData;
	double m_dZero;
	double m_dNull;
	double m_dPi;
// #if defined (_CIVIL)
// 	CString m_strStctEL[D_STCT_EREC_MAX_CVL];	// 시공단계 EL의 하중조건타입 (D,DC,DW, L, T,TPG).
// #else 
// 	CString m_strStctEL[D_STCT_EREC_MAX_GEN];	// 시공단계 EL의 하중조건타입 (D,DC,DW, L, T,TPG).
// #endif
	// For JTG-D62-04.
	BOOL m_bOverLL04;					// Over 0.4*L(장기), Under 0.4*L(단기).
	BOOL m_bFR, m_bSF, m_bIP;	// 지지점마찰력(FR), 유수압(SF, Stream Flow Pressure), 빙압력(IP, Ice Pressure)
	// For DEBUG.
	CString m_strDebugFile;
	int m_iDebugType;
	int m_iDebugElem;
	int m_iDebugPart;
	int m_iDebugLcom;
	CMap<T_ELEM_K,T_ELEM_K,BOOL,BOOL> m_arDebugElem;

	// Coded by Seungjun for Test. ('20060719)
	BOOL m_bTestMode;
	BOOL m_bDgnMultiThread;

	bool m_bWrite5PositionDgnForce; // 요소당 내부적으로 존재하는 5 Position의Design Force를 저장 여부

	int m_iCivilCode;

	// 
	CMap<T_LCOM_K,T_LCOM_K,UINT,UINT> m_arSerialLcomK;

	BOOL m_bIsPLCB; // Pre-Combined Composite Bridge
	CMap<T_STLD_K,T_STLD_K,T_STLD_K,T_STLD_K> m_mapPlcbStldK;

public:
		//nForceType 1-12
	UINT Get_SerialLcomKey(T_LCOM_K LcomK, int nForceType);	
	UINT Get_SerialLcomKey(T_LCOM_K LcomK);	
	T_LCOM_K Get_OriginalLcomKey(UINT SerialLcomK);
	BOOL Get_DgnLcomData(UINT SerialLcomK, _DGN_LCOM& LcomD);	

public:
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcElemInTP;
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcElemExTP;
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcTnprData;
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcTnscData;
	std::map<ElemPairK, _DGN_CRCK_JTG_ELEM> m_arCrckForcData;
	std::map<ElemPairK, _DGN_STRS_ADD_ELEM> m_arStrsAddData;
	std::map<ElemPairK, _DGN_SHRS_ADD_ELEM> m_arShrsStagData;
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcElemLTerm;
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcElemLTermExTP;
	// Add by Jaeoh. ('06.09.18). JTG D62-04, 텐던응력 계산.
	std::map<ElemPairK, _DGN_FORC_CRC_ELEM> m_arForcForJtgTndnStress;
	std::map<ElemPairK, double>	m_arftkData;	// ftk.
	// Stress.
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressElemInTP;
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressElemExTP;
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressTnprData;
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressTnscData;
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressElemLTerm;
	std::map<ElemPairK, _DGN_STRS_CRC_ELEM> m_arStressElemLTermExTP;
	// Principle.
	std::map<ElemPairK, _DGN_SHRS_ADD_ELEM> m_arStssElemShrs;

	//////////////////////////////////////////////////////////////////////////
	// Expansion Position : 2 Positions -> 5 Positions
	//////////////////////////////////////////////////////////////////////////
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcElemInTP_EX;
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcElemExTP_EX;
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcTnprData_EX;
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcTnscData_EX;
	std::map<ElemPairK, _DGN_CRCK_JTG_EX_ELEM> m_arCrckForcData_EX;
	std::map<ElemPairK, _DGN_STRS_ADD_EX_ELEM> m_arStrsAddData_EX;
	std::map<ElemPairK, _DGN_SHRS_ADD_EX_ELEM> m_arShrsStagData_EX;
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcElemLTerm_EX;
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcElemLTermExTP_EX;
	//  JTG D62-04, 텐던응력 계산.
	std::map<ElemPairK, _DGN_FORC_CRC_EX_ELEM> m_arForcForJtgTndnStress_EX;
	// Stress.
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressElemInTP_EX;
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressElemExTP_EX;
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressTnprData_EX;
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressTnscData_EX;
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressElemLTerm_EX;
	std::map<ElemPairK, _DGN_STRS_CRC_EX_ELEM> m_arStressElemLTermExTP_EX;
	// Principle.
	std::map<ElemPairK, _DGN_SHRS_ADD_EX_ELEM> m_arStssElemShrs_EX;


	// 피로검토 사용.
	CMapEx<T_LCOM_K,T_LCOM_K,T_DGN_LCOM_ID,T_DGN_LCOM_ID&> m_aFatiLcomId;

	// (SOD) 강도검토시 kapa검토 사용 : m_aFatiLcomId 참고
	CMapEx<T_LCOM_K,T_LCOM_K,T_DGN_LCOM_ID,T_DGN_LCOM_ID&> m_aStrnLcomId;
    CArray<int, int&> m_aTranDgnLcomK; // Transfer. Lcom

	CDgnForceCtrl *m_pDgnForceCtrl;


public:
	CCRCForceCtrl();
	virtual ~CCRCForceCtrl();
	
	void Set_DataCtrl(CCRCDataCtrl* pDataCtrl);
	void Set_Write5PositionDgnForce(bool bWrite5PositionDgnForce);
	bool Get_Write5PositionDgnForce();
	bool Creat_MemberVarialbes();
	bool Destruct_MemberVarialbes();

    CDesignForceResult* GetDgnForce() { return m_pDgnForceResult; }
	
protected:
	//
	CCRCDataCtrl       *m_pDataCtrl;
	CDesignForceResult *m_pDgnForceResult;

	CDBDoc *m_pDoc;
	CAttrCtrl  *m_pAttrCtrl;
	CAttrCtrl2 *m_pAttrCtrl2;
	CUnitCtrl  *m_pUnitCtrl;
	CPostCtrl  *m_pPostCtrl;
	CSectInfo  *m_pSectInfo; 
	CStageInfo *m_pStageInfo;

	// Write Binary
	char ID_M[4];
	LONGLONG ID_S,IST_R,LNG_R,NUM_R;

	CFile m_FileOut;
	CFile m_FileOutPart;
	CResultDataFilePosition* m_pRdfp;
	CBnFileResult m_BnFile;
	//
	std::map<ElemPairK,UINT> m_mapChkElemK;
	int m_iLcomNum;

	T_PSCD_D m_Pscd;
	T_CHRP_D m_Chrp;

	T_STRSPOST_D m_StrsPost;
		
public:
	// Write Binary
	 // Load combination
	ArrElemPairKey m_arChkElemK;
	void Set_CheckElem(ArrElemPairKey& arElemK);
	BOOL CreatDesignForceStressFile();
	BOOL OpenDesignForceStressFile();
	BOOL WriteEmptyData(CBnFileResult* pFileWrite, CFile* pFileOut);
	void CloseDgnForceFile();

	BOOL WriteForceLcom(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceLcom_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceLcom_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);

	BOOL WriteForceLcomExTp(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceStage(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteaShrsStage(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL WriteForceTnpr(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceExTnpr(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceTnsc(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteStressAdd(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD& StrsAdd);
	BOOL WriteForceLTerm(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceSTerm(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceJtgTndn(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteStressLcom(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressLcom_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressLcom_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
    BOOL WriteStressLTermPart2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
    BOOL WriteStressSTermPart2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressScfpCrPartLcom(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL WriteStressScfpShPartLcom(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL WriteStressScfpTePartLcom(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL WriteStressScfpRePartLcom(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL WriteForceScfpSum(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceScfpCrp(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceScfpShr(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);

	BOOL WriteStressTnpr(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressTnsc(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressLTerm(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressSTerm(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStssLcom(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL WriteStssLcom_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL WriteStssLcom_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL WriteStressTnpr_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressTnsc_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);

	BOOL WriteForceDL_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceLL_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteStressDL_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteStressLL_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL WriteForceCSTP_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);   // Add, Jaeoh. [12/23/2008] Maxiao 요구 
	BOOL WriteStressCSTP_TB05(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	//
	BOOL WriteForceEQE2_JTG(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL WriteForceWeight_JTG(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);


// Expansion Data  2 Position -> 5 Position
	BOOL WriteForceLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceLcom_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceLcom_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);

	BOOL WriteForceLcomExTp_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceStage_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteaShrsStage_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL WriteForceTnpr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceExTnpr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceTnsc_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteStressAdd_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD_EX& StrsAdd);
	BOOL WriteForceLTerm_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceJtgTndn_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteStressLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressLcom_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressLcom_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressScfpCrPartLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL WriteStressScfpShPartLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL WriteStressScfpTePartLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL WriteStressScfpRePartLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL WriteForceScfpSum_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceScfpCrp_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceScfpShr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);

	BOOL WriteStressTnpr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressTnsc_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressLTerm_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStssLcom_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL WriteStssLcom_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL WriteStssLcom_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL WriteStressTnpr_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressTnsc_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);

	BOOL WriteForceDL_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceLL_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteStressDL_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteStressLL_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL WriteForceCSTP_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);   // Add, Jaeoh. [12/23/2008] Maxiao 요구 
	BOOL WriteStressCSTP_TB05_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);

	BOOL WriteForceEQE2_JTG_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL WriteForceWeight_JTG_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);

	
	// Load Case
	BOOL CreatForceStressByLCaseFile();
	BOOL OpenForceStressByLCaseFile();
	BOOL WriteLCaseEmptyData(CBnFileResult* pFileWrite, CFile* pFileOut);
	//void CloseDgnLCaseForceFile();

	BOOL CreatForceStressByLoadCaseFile();
	BOOL OpenForceStressByLoadCaseFile();
	BOOL WriteLoadCaseEmptyData(CBnFileResult* pFileWrite, CFile* pFileOut);

	BOOL CreatPartForceStressByLoadCaseFile();
	BOOL OpenPartForceStressByLoadCaseFile();
	BOOL WritePartLoadCaseEmptyData(CBnFileResult* pFileWrite, CFile* pFileOut);

	BOOL WriteLcaseForceStld(ElemPairK ElemK, T_STLD_K StldK, T_STLD_LCASE& StldLcase);
	BOOL WriteLcaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceStep(ElemPairK ElemK, T_STEP_K StepK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, T_SBCF_LCASE& SbcfLcase);
	BOOL WriteLcaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, T_SBCF_LCASE& SbcfLcase);
	BOOL WriteLcaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, T_SBCF_LCASE& SbcfLcase);
	BOOL WriteLcaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, T_STRB_LCASE& StrbLcase);
	BOOL WriteLcaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, T_SBCF_LCASE& SbcfLcase);

	//
	BOOL WriteLoadCaseStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE &StldLcase);
	BOOL WriteLoadCaseSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase);
    BOOL WriteLoadCaseMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL WriteLoadCaseSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	BOOL WriteLoadCaseGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	BOOL WriteLoadCaseLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL WriteLoadCaseLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	//
	BOOL WritePartLoadCaseStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
    BOOL WritePartLoadCaseMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL WritePartLoadCaseSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL WritePartLoadCaseGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL WritePartLoadCaseLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL WritePartLoadCaseLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL WritePartLoadCaseStld_SCFP(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE &rData);
	BOOL WritePartLoadCaseSgld_SCFP(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE &rData);

	// 5 Positions Force
	BOOL WriteLoadCaseStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE_EX &StldLcase);
	BOOL WriteLoadCaseSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
    BOOL WriteLoadCaseMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL WriteLoadCaseSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
	BOOL WriteLoadCaseGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
	BOOL WriteLoadCaseLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL WriteLoadCaseLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);

	// 5 Positions Part Force
	BOOL WritePartLoadCaseStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
    BOOL WritePartLoadCaseMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL WritePartLoadCaseSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL WritePartLoadCaseGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL WritePartLoadCaseLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL WritePartLoadCaseLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL WritePartLoadCaseStld_SCFP_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE_EX &rData);
	BOOL WritePartLoadCaseSgld_SCFP_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE_EX &rData);

	UINT GetSerialElemKey(ElemPairK ElemK);
// Member Functions.
protected:
	void Write_GDisplayAnalString(CString strTxt, BOOL bClearPre=TRUE, BOOL bSpacePost=TRUE);
	void Set_ForceIni();
	BOOL Is_Reciprocal(unsigned int iAnalType, unsigned int iLcaseKey);
	BOOL Get_Sbcf(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, ADGNFORCE &aForceLcase, ADGNSTRESS &aStressLcase, ADGNSHRSADD &aStssLcase,
				  ADGNFORCE &aForceP1, ADGNSTRESS &aStressP1, ADGNFORCE &aForceP2, ADGNSTRESS &aStressP2);	// Concurrent Forces.
	BOOL Get_Sbcf(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, ADGNFORCE &aForceLcase, ADGNSTRESS &aStressLcase, ADGNSHRSADD &aStssLcase,
				  ADGNFORCE &aForceP1, ADGNSTRESS &aStressP1, ADGNFORCE &aForceP2, ADGNSTRESS &aStressP2, BOOL bElemAll);
	BOOL Get_Sbcf(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				  _DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int iMaxMinType, BOOL bSpc=FALSE); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL Get_Sbcf(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, ADGNFORCE &aForceLcase, BOOL bElemAll);

	BOOL Get_Sepc(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& aStssLcase,
	   			  _DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);	// Spectrum, Eccentric Response Spectrum.

	BOOL Get_Sepc_EX(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType, _DGN_FORC_CRC_EX& ForceLcase,
					_DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& aStssLcase,_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, 
					_DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);	// Spectrum, Eccentric Response Spectrum.
	BOOL Get_Sbcf_EX(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType,
					ADGNFORCE_EX &aForceLcase,ADGNSTRESS_EX &aStressLcase, ADGNSHRSADD_EX &aStssLcase,ADGNFORCE_EX &aForceP1, 
					ADGNSTRESS_EX &aStressP1, ADGNFORCE_EX &aForceP2, ADGNSTRESS_EX &aStressP2);	// Concurrent Forces.
	BOOL Get_Sbcf_EX(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType,
					ADGNFORCE_EX &aForceLcase, ADGNSTRESS_EX &aStressLcase, ADGNSHRSADD_EX &aStssLcase, 
					ADGNFORCE_EX &aForceP1, ADGNSTRESS_EX &aStressP1, ADGNFORCE_EX &aForceP2, ADGNSTRESS_EX &aStressP2, BOOL bElemAll);
	BOOL Get_Sbcf_EX(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType,
					_DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase, 
					_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, 
					_DGN_STRS_CRC_EX& StressPart2, int iMaxMinType, BOOL bSpc=FALSE);
	BOOL Get_Sbcf_EX(ElemPairK ElemK, unsigned int iLcaseKey, int nForType, BOOL bCalcStss, int iLcaseType,
					ADGNFORCE_EX &aForceLcase, BOOL bElemAll);


	// For Lcom.
	BOOL Get_Stld(ElemPairK ElemK, T_STLD_K StldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase);
	BOOL Get_Splc(ElemPairK ElemK, T_SPLC_K SplcK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_Espc(ElemPairK ElemK, T_ESPC_K EspcK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_This(ElemPairK ElemK, T_THIS_K ThisK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_Sgld(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, BOOL bIncludeTP, int iStepNo);
    BOOL Get_Mvld(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, int iMaxMinType, BOOL bSpc = FALSE); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL Get_Smlc(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, double dFactor, int iMaxMinType);
	BOOL Get_Gilc(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, int iMaxMinType);
 
	BOOL Get_Stld_EX(ElemPairK ElemK, T_STLD_K StldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase);
	BOOL Get_Splc_EX(ElemPairK ElemK, T_SPLC_K SplcK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_Espc_EX(ElemPairK ElemK, T_ESPC_K EspcK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_This_EX(ElemPairK ElemK, T_THIS_K ThisK, int nForType, BOOL bCalcStss, double dFactor);
	BOOL Get_Sgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, BOOL bIncludeTP, int iStepNo);
	BOOL Get_Mvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, int iMaxMinType, BOOL bSpc = FALSE); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL Get_Smlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, double dFactor, int iMaxMinType);
	BOOL Get_Gilc_EX(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, double dFactor, BOOL bReciLcase, int iMaxMinType);
	
	BOOL Get_Lcom(ElemPairK ElemK, T_LCOM_K LcomK, int nForType, BOOL bCalcStss, double dFactor, int iMaxMinType);  // Add by GAY. MNET:2903. ('07.07.31).  
    BOOL Get_Lcom_EX(ElemPairK ElemK, T_LCOM_K LcomK, int nForType, BOOL bCalcStss, double dFactor, int iMaxMinType);
	BOOL Get_EnvelopLcom(ElemPairK ElemK,int nForType, BOOL bIsCalcStss,BOOL bReciInput,BOOL bIncludeTP, _DGN_LCOM& LcomDesign,int nCompareType);//add by gongxing at 2016-05-10 관쭹莉북珂코제瓊혤렘랬
	// Get Summation Forces.
	BOOL Get_Force(_DGN_FORC_CRC& ForceLcom, _DGN_FORC_CRC& ForceStag, _DGN_SHRS_ADD& ShrsStage, _DGN_FORC_CRC& ForceTnpr, 
				_DGN_FORC_CRC& ForceTnsc, _DGN_STRS_ADD& StressAdd, _DGN_FORC_CRC& ForceLTerm, _DGN_FORC_CRC& ForceLL, _DGN_FORC_CRC& ForceJtgTndn, 
				_DGN_FORC_CRC& ForceDL_TB05, _DGN_FORC_CRC& ForceLL_TB05, _DGN_FORC_CRC& ForceCSTP_TB05, 
				_DGN_FORC_CRC& ForceEQE2_JTG, _DGN_FORC_CRC& ForceWeight_JTG);
	BOOL Get_Force_EX(_DGN_FORC_CRC_EX& ForceLcom, _DGN_FORC_CRC_EX& ForceStag, _DGN_SHRS_ADD_EX& ShrsStage, _DGN_FORC_CRC_EX& ForceTnpr, 
					_DGN_FORC_CRC_EX& ForceTnsc, _DGN_STRS_ADD_EX& StressAdd, _DGN_FORC_CRC_EX& ForceLTerm, _DGN_FORC_CRC_EX& ForceJtgTndn,
					_DGN_FORC_CRC_EX& ForceDL_TB05, _DGN_FORC_CRC_EX& ForceLL_TB05, _DGN_FORC_CRC_EX& ForceCSTP_TB05, 
					_DGN_FORC_CRC_EX& ForceEQE2_JTG, _DGN_FORC_CRC_EX& ForceWeight_JTG);
	BOOL Get_Stress(_DGN_STRS_CRC& StressLcom, _DGN_STRS_CRC& StressTnpr, _DGN_STRS_CRC& StressTnsc, _DGN_STRS_CRC& StressLTerm, _DGN_STRS_CRC& StressLL,
				   _DGN_SHRS_ADD& StssLcom, _DGN_STRS_CRC& StressDL_TB05, _DGN_STRS_CRC& StressLL_TB05, _DGN_STRS_CRC& StressCSTP_TB05);
	BOOL Get_Stress_EX(_DGN_STRS_CRC_EX& StressLcom, _DGN_STRS_CRC_EX& StressTnpr, _DGN_STRS_CRC_EX& StressTnsc, 
					_DGN_STRS_CRC_EX& StressLTerm, _DGN_SHRS_ADD_EX& StssLcom, _DGN_STRS_CRC_EX& StressDL_TB05,
					_DGN_STRS_CRC_EX& StressLL_TB05, _DGN_STRS_CRC_EX& StressCSTP_TB05);

	BOOL Get_ForcePart1(_DGN_FORC_CRC &ForceLcom);
	BOOL Get_ForcePart2(_DGN_FORC_CRC &ForceLcom);
	BOOL Get_StressPart1(_DGN_STRS_CRC &StressLcom);
	BOOL Get_StressPart2(_DGN_STRS_CRC &StressLcom);
	BOOL Get_StressScfpPart(_DGN_SCFP_PART_LCASE &StrCr, _DGN_SCFP_PART_LCASE &StrSh, _DGN_SCFP_PART_LCASE &StrTe, _DGN_SCFP_PART_LCASE &StrRe);
	BOOL Get_StressTendonPart2(_DGN_STRS_CRC &Tnpr, _DGN_STRS_CRC &Tnse);
    BOOL Get_StressLTermPart2(_DGN_STRS_CRC &StressLcom);
    BOOL Get_StressSTermPart2(_DGN_STRS_CRC &StressLcom);

	BOOL Get_ForcePart1_EX(_DGN_FORC_CRC_EX &ForceLcom);
	BOOL Get_ForcePart2_EX(_DGN_FORC_CRC_EX &ForceLcom);
	BOOL Get_StressPart1_EX(_DGN_STRS_CRC_EX &StressLcom);
	BOOL Get_StressPart2_EX(_DGN_STRS_CRC_EX &StressLcom);
	BOOL Get_StressScfpPart_EX(_DGN_SCFP_PART_LCASE_EX &StrCr, _DGN_SCFP_PART_LCASE_EX &StrSh, 
							_DGN_SCFP_PART_LCASE_EX &StrTe, _DGN_SCFP_PART_LCASE_EX &StrRe);
	BOOL Get_StressTendonPart2_EX(_DGN_STRS_CRC_EX &Tnpr, _DGN_STRS_CRC_EX &Tnse);

	BOOL Get_LcomDataForDesign(ElemPairK ElemK,  int nForType, int iDgnLcomKind, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceStag, ADGNSHRSADD &aShrsStage, ADGNFORCE &aForceTnpr, ADGNFORCE &aForceTnsc, ADGNSTRSADD &aStressAdd, ADGNFORCE &aForceLTerm, ADGNFORCE &aForceJtgTndn,
														 ADGNSTRESS& aStressLcom, ADGNSTRESS& aStressTnpr, ADGNSTRESS& aStressTnsc, ADGNSTRESS& aStressLTerm, ADGNSHRSADD &aStssLcom,BOOL bBinary=FALSE);

	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK,  int nForType, int iDgnLcomKind, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceStag, ADGNSHRSADD_EX &aShrsStage, ADGNFORCE_EX &aForceTnpr, ADGNFORCE_EX &aForceTnsc, ADGNSTRSADD_EX &aStressAdd, ADGNFORCE_EX &aForceLTerm, ADGNFORCE_EX &aForceJtgTndn,
														 ADGNSTRESS_EX& aStressLcom, ADGNSTRESS_EX& aStressTnpr, ADGNSTRESS_EX& aStressTnsc, ADGNSTRESS_EX& aStressLTerm, ADGNSHRSADD_EX &aStssLcom,BOOL bBinary=FALSE);

	void Set_RatNoEffectZone(ElemPairK ElemK);
	double Get_TauShrTor(double dTsh, double dTst);
	// Set 2D, 2D+Tor, 3D Option.
	void Set_DimTypeForc(_DGN_FORC_CRC& ForcCrc);
	// Calculate Stress for Design.
	BOOL Get_LoadCaseStress(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC& StressLcase);
	BOOL Get_LoadCaseStress(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSTRESS &aStressLcase);

	BOOL Get_LoadCaseStressGilc(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC& StressLcase);
	BOOL Get_LoadCaseStressGilc(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSTRESS &aStressLcase);
	BOOL Get_LoadCaseStressSlab(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC& StressLcase);
	BOOL Get_LoadCaseStressSlab(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, _DGN_STRS_CRC aStrsLcase[12]);

	BOOL Get_LoadCaseStss(T_STSS_D& StrData, _DGN_SHRS_ADD& StressLcase);
	BOOL Get_LoadCaseStss(ElemPairK ElemK, BOOL bTndnPs, T_STRB_D& StrData, _DGN_SHRS_ADD& StssLcase);
	BOOL Get_LoadCaseStss(ElemPairK ElemK, BOOL bTndnPs, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSHRSADD &aStssLcase);
	BOOL Calc_Tat(BOOL bIs7Dof, double dCyi, double dCzi, double dIp, double dJ, double dMux, double dTst, double dTwp, double dXYsh, double dXZsh,
		double &dTat_xy, double &dTat_xz, double &dSwt, double &dSwt_xy, double &dTat);

	// Set 2D, 2D+Tor, 3D Option.
	void Set_DimTypeForc_EX(_DGN_FORC_CRC_EX& ForcCrc);
	// Calculate Stress for Design. For 5 Position for each element

	BOOL Get_LoadCaseStress_EX(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC_EX& StressLcase);
	BOOL Get_LoadCaseStress_EX(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSTRESS_EX &aStressLcase);

	BOOL Get_LoadCaseStressGilc_EX(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC_EX& StressLcase);
	BOOL Get_LoadCaseStressGilc_EX(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSTRESS_EX &aStressLcase);
	BOOL Get_LoadCaseStressSlab_EX(ElemPairK ElemK, T_STRB_D& StrData, _DGN_STRS_CRC_EX& StressLcase);
	BOOL Get_LoadCaseStressSlab_EX(ElemPairK ElemK, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, _DGN_STRS_CRC_EX aStrsLcase[12]);

	BOOL Get_LoadCaseStss_EX(T_STSS_D& StrData, _DGN_SHRS_ADD_EX& StssLcase);
	BOOL Get_LoadCaseStss_EX(ElemPairK ElemK, BOOL bTndnPs, T_STRB_D& StrData, _DGN_SHRS_ADD_EX& StssLcase);
	BOOL Get_LoadCaseStss_EX(ElemPairK ElemK, BOOL bTndnPs, T_SBCF_D& StrMaxData, T_SBCF_D& StrMinData, ADGNSHRSADD_EX &aStssLcase);

	// For DEBUG.
	CString Debug_String(int iCol, CString strTxt);
	CString Debug_Int(int iValue);
	CString Debug_Double(double dValue);
	void Debug_PrintData(double dVal1, double dVal2, double dVal3, double dVal4, double dVal5, double dVal6, double dVal7, double dVal8, double dVal9, double dVal10, double dVal11, double dVal12, double dVal13, double dVal14);
	// For Unit.
	double Cng_LengthUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dLength, int iUp);
	double Cng_ForceUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dForce);
	double Cng_StressUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dStress);
	// For JTG D62-04 6.4.3.
	void Set_CrackJtg(ElemPairK ElemK, int iDgnLcomNo, _DGN_FORC_CRC& ForceLcom);

	// Add by Jaeoh.('05.07.29)
	// Force/Stress by Load Case.
	std::map<ElemPairK, UINT> m_SerialElemKey;
	//
	CArray<STLD_STRB_STSS, STLD_STRB_STSS> m_arStldElem;  
	CArray<SPLC_STRB_STSS, SPLC_STRB_STSS> m_arSplcElem;
	CArray<ESPC_STRB_STSS, ESPC_STRB_STSS> m_arEspcElem;
	CArray<THIS_STRB_STSS, THIS_STRB_STSS> m_arThisElem;
	CArray<SGLD_STRB_STSS, SGLD_STRB_STSS> m_arSgldElem;
	CArray<STEP_STRB_STSS, STEP_STRB_STSS> m_arStepElem;
	CArray<MVLD_SBCF     , MVLD_SBCF>      m_arMvldElem;
	CArray<SMLC_SBCF     , SMLC_SBCF>      m_arSmlcElem;
	CArray<GILC_STRB_STSS, GILC_STRB_STSS> m_arGilcStrbElem;
	CArray<GILC_SBCF     , GILC_SBCF>      m_arGilcSbcfElem;
	CArray<LCOM_STRB_STSS, LCOM_STRB_STSS> m_arLcomStrbElem;    // Add by GAY. MNET:2903. ('07.07.31). Consider SRSS, Envelope.
	CArray<LCOM_SBCF     , LCOM_SBCF>      m_arLcomSbcfElem;    // Add by GAY. MNET:2903. ('07.08.24). Consider SRSS, Envelope that include Concurrent Forces.

	CMap<T_STLD_K,T_STLD_K&,T_STLD_K,T_STLD_K&> m_arStldK;	// Static.
	CMap<T_STLD_K,T_STLD_K&,T_STLD_K,T_STLD_K&> m_arBtssK;	// Btss.
	CMap<T_SPLC_K,T_SPLC_K&,T_SPLC_K,T_SPLC_K&> m_arSplcK;	// Spectrum.
	CMap<T_ESPC_K,T_ESPC_K&,T_ESPC_K,T_ESPC_K&> m_arEspcK;	// Eccentric Response Spectrum.
	CMap<T_MVLD_K,T_MVLD_K&,T_MVLD_K,T_MVLD_K&> m_arMvldK;	// Moving.
	CMap<T_THIS_K,T_THIS_K&,T_THIS_K,T_THIS_K&> m_arThisK;	// Time History.
	CMap<T_SMLC_K,T_SMLC_K&,T_SMLC_K,T_SMLC_K&> m_arSmlcK;	// Settlement.
	CMap<T_SGLD_K,T_SGLD_K&,T_SGLD_K,T_SGLD_K&>	m_arSgldK;	// Stage.
	CMap<T_GILC_K,T_GILC_K&,T_GILC_K,T_GILC_K&>	m_arGilcK;	// Grid Model.
	CMap<T_STEP_K,T_STEP_K&,T_STAG_K,T_STAG_K&> m_arStepK;	// Step in Stage.
	CMap<T_LCOM_K,T_LCOM_K&,T_LCOM_K,T_LCOM_K&> m_arLcomK;  // Add by GAY. MNET:2903. ('07.07.31). Consider SRSS, Envelope.
	CMap<T_STLD_K,T_STLD_K&,T_STLD_K,T_STLD_K&> m_arStldScfpK;	// Static. - 
	CMap<T_SGLD_K,T_SGLD_K&,T_SGLD_K,T_SGLD_K&>	m_arSgldScfpK;	// Stage. - 

	void Set_StrbForDgn(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_new(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Stld(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Splc(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Espc(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_This(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Mvld(ElemPairK ElemK, int iSerial);
    void Set_StrbForDgn_MvldBS(ElemPairK ElemK, int iSerial); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	void Set_StrbForDgn_Smlc(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Sgld(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_GilcStrb(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_GilcSbcf(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_LcomStrb(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_LcomSbcf(ElemPairK ElemK, int iSerial);

	void Set_StrbForDgn_Stld_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Splc_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Espc_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_This_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Mvld_EX(ElemPairK ElemK, int iSerial);
    void Set_StrbForDgn_MvldBS_EX(ElemPairK ElemK, int iSerial); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	void Set_StrbForDgn_Smlc_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_Sgld_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_GilcStrb_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_GilcSbcf_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_LcomStrb_EX(ElemPairK ElemK, int iSerial);
	void Set_StrbForDgn_LcomSbcf_EX(ElemPairK ElemK, int iSerial);

	BOOL GetStldForElem(ElemPairK ElemK, int nForType, int iLcaseType, unsigned LcaseK, T_STRB_D& StrbMax, T_STSS_D& StssMax, T_BTSS_D& BtssMax, T_STRB_D &StrbP1, T_STRB_D &StrbP2);
	BOOL GetStrbForElem(ElemPairK ElemK, int nForType, int iLcaseType, unsigned LcaseK, T_STRB_D& StrbMax, T_STRB_D& StrbMin, T_STSS_D& StssMax, T_STSS_D& StssMin, T_STRB_D &StrbP1, T_STRB_D &StrbP2);
	BOOL GetLcomForElem(ElemPairK ElemK, int nForType, int iLcaseType, unsigned LcaseK, T_STRB_D& StrbMax, T_STRB_D& StrbMin, T_STSS_D& StssMax, T_STSS_D& StssMin, T_STRB_D &StrbP1, T_STRB_D &StrbP2);  // Add by GAY. MNET:2903. ('07.07.31).
	//BOOL GetStepForElem(T_ELEM_K ElemK, int iLcaseType, unsigned StepK, T_STRB_D* pStrbMax, T_STRB_D* pStrbMin, T_STSS_D* pStssMax, T_STSS_D* pStssMin);
	BOOL GetSbcfForElem(ElemPairK ElemK, int nForType, int iLcaseType, unsigned LcaseK, T_SBCF_D& SbcfMax, T_SBCF_D& SbcfMin, T_SBCF_D& SbcfMaxP1, T_SBCF_D& SbcfMinP1, T_SBCF_D& SbcfMaxP2, T_SBCF_D& SbcfMinP2);
	 //
	BOOL GetStld4Elem(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE &Strb, _DGN_STRBFORC_PART_LCASE &StrbPart);
	BOOL GetStrb4Elem(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE &Strb, _DGN_STRBFORC_PART_LCASE &StrbPart);
	BOOL GetLcom4Elem(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, int iMaxMinType, _DGN_STRBFORC_LCASE &Strb, _DGN_STRBFORC_PART_LCASE &StrbPart);
	BOOL GetLcom4Elem(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE &Strb, _DGN_STRBFORC_PART_LCASE &StrbPart);
	BOOL GetSbcf4Elem(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_SBCFFORC_LCASE &Sbcf, _DGN_SBCFFORC_PART_LCASE &SbcfPart, BOOL bSpc=FALSE);   // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)

	// 5 Position Expansion for each element
	BOOL GetStld4Elem_EX(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE_EX &Strb, _DGN_STRBFORC_PART_LCASE_EX &StrbPart);
	BOOL GetStrb4Elem_EX(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE_EX &Strb, _DGN_STRBFORC_PART_LCASE_EX &StrbPart);
	BOOL GetLcom4Elem_EX(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, int iMaxMinType, _DGN_STRBFORC_LCASE_EX &Strb, _DGN_STRBFORC_PART_LCASE_EX &StrbPart);
	BOOL GetLcom4Elem_EX(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_STRBFORC_LCASE_EX &Strb, _DGN_STRBFORC_PART_LCASE_EX &StrbPart);
	BOOL GetSbcf4Elem_EX(ElemPairK ElemK, int nForType, int nLcaseType, unsigned LcaseK, _DGN_SBCFFORC_LCASE_EX &Sbcf, _DGN_SBCFFORC_PART_LCASE_EX &SbcfPart, BOOL bSpc=FALSE);   // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)


	BOOL Get_ChangeStrbStress(T_STRB_D& GetStrb, T_STRB_D& SetStrb);
	BOOL Get_ChangeSbcfStress(T_SBCF_D& GetSbcf, T_SBCF_D& SetSbcf);
	BOOL Get_ChangeStrbStressandForce(T_STRB_D& GetStrb, T_STRB_D& SetStrb);
	BOOL Get_ChangeSbcfStressandForce(T_SBCF_D& GetSbcf, T_SBCF_D& SetSbcf);
	BOOL Get_ConvertStrb(int nType, T_SBCF_D &rSbcf, T_STRB_D &rData);

	BOOL Get_StrbToForce(T_STRB_D &Strb, _DGN_FORC_CRC &rData);
	BOOL Get_SbcfToForce(BOOL bMax, int nIdx, T_SBCF_D &SbcfMax, T_SBCF_D &SbcfMin, _DGN_FORC_CRC &rData);

	// For 5 Position
	BOOL Get_StrbToForce_EX(T_STRB_D &Strb, _DGN_FORC_CRC_EX &rData);
	BOOL Get_SbcfToForce_EX(BOOL bMax, int nIdx, T_SBCF_D &SbcfMax, T_SBCF_D &SbcfMin, _DGN_FORC_CRC_EX &rData);  
	BOOL Get_ScfpToDgnScfp_EX(T_SCFP_D &Scfp, _DGN_SCFP_EX_D &rData); 

public:
	BOOL Get_ScfpToDgnScfp(const T_SCFP_D &Scfp, _DGN_SCFP_D &rData);
	BOOL ConvertDgnScfpToScfp(const _DGN_SCFP_D& Scfp, T_SCFP_D& rData);
	BOOL ConvertDgnForceToPSCForce(int nIdx, const _DGN_FORC_CRC &DgnForD, T_PSCD_FORCE& ForD);
	BOOL ConvertDgnPSCForceToDgnForce(int nIdx, const T_PSCD_FORCE& ForD, _DGN_FORC_CRC& rData);

public:
	// Serial.
	CMap<T_STLD_K,T_STLD_K&,UINT,UINT&> m_arMapStldK;
	CMap<T_SPLC_K,T_SPLC_K&,UINT,UINT&> m_arMapSplcK;
	CMap<T_ESPC_K,T_ESPC_K&,UINT,UINT&> m_arMapEspcK;
	CMap<T_THIS_K,T_THIS_K&,UINT,UINT&> m_arMapThisK;
	CMap<T_SGLD_K,T_SGLD_K&,UINT,UINT&>	m_arMapSgldK;
	CMap<T_STEP_K,T_STEP_K&,UINT,UINT&>	m_arMapStepK;
	CMap<T_MVLD_K,T_MVLD_K&,UINT,UINT&> m_arMapMvldK;	
	CMap<T_SMLC_K,T_SMLC_K&,UINT,UINT&> m_arMapSmlcK;
	CMap<T_GILC_K,T_GILC_K&,UINT,UINT&>	m_arMapGilcStrbK; // Strb
	CMap<T_GILC_K,T_GILC_K&,UINT,UINT&>	m_arMapGilcSbcfK; // Sbcf
	CMap<T_LCOM_K,T_LCOM_K&,UINT,UINT&> m_arMapLcomStrbK; // Add by GAY. MNET:2903. ('07.07.31). Consider SRSS, Envelope.
	CMap<T_LCOM_K,T_LCOM_K&,UINT,UINT&> m_arMapLcomSbcfK; // Add by GAY. MNET:2903. ('07.08.24). Consider SRSS, Envelope that include Concurrent Forces.
	CMap<T_STLD_K,T_STLD_K&,UINT,UINT&> m_arMapStldScfpK;
	CMap<T_SGLD_K,T_SGLD_K&,UINT,UINT&>	m_arMapSgldScfpK;

public:
	int Get_Code();
	void Set_LcomForElem(ArrElemPairKey& aElemK);
	
// [18/12/2006 : maxiao] For CIVIL(CH) 2006
	void Set_LcomForColumnElem(ArrElemPairKey& aElemK);


	// For Lcase.
	BOOL Get_Stld(ElemPairK ElemK, T_STLD_K StldK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_ADD& StressAdd, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);	// Static Load.
	BOOL Get_Splc(ElemPairK ElemK, T_SPLC_K SplcK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);	// Spectrum.
	BOOL Get_Espc(ElemPairK ElemK, T_ESPC_K EspcK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);	// Eccentric Response Spectrum.
	BOOL Get_This(ElemPairK ElemK, T_THIS_K ThisK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);	// Time History.
	BOOL Get_Sgld(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, int iStepNo, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
	BOOL Get_Sgld(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, T_SGLD_D& SgldD, int iStepNo, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int Lcase=0);  
	BOOL Get_Sgld_Expo(ElemPairK ElemK, T_SGLD_K SgldK, T_SGLD_D& SgldD, int iStepNo, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase);
	BOOL Get_Mvld(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE &aForceLcase, ADGNSTRESS &aStressLcase, ADGNSHRSADD &aStssLcase,
				ADGNFORCE &aForceP1, ADGNSTRESS &aStressP1, ADGNFORCE &aForceP2, ADGNSTRESS &aStressP2, BOOL bElemAll=FALSE);	// Moving Load.
	BOOL Get_Mvld(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				  _DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int iMaxMinType, BOOL bSpc=FALSE); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL Get_Mvld(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE &aForceLcase, BOOL bElemAll);
	BOOL Get_Smlc(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE &aForceLcase, ADGNSTRESS &aStressLcase, ADGNSHRSADD &aStssLcase,
				ADGNFORCE &aForceP1, ADGNSTRESS &aStressP1, ADGNFORCE &aForceP2, ADGNSTRESS &aStressP2);	// Settlement.
	BOOL Get_Smlc(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, CString& strLcaseNa, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int iMaxMinType);
	BOOL Get_Gilc(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int iMaxMinType);
	BOOL Get_Gilc(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, ADGNFORCE &aForceLcase);
	BOOL Get_Gilc(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, ADGNFORCE &aForceLcase, ADGNSTRESS &aStressLcase, ADGNSHRSADD &aStssLcase,
				ADGNFORCE &aForceP1, ADGNSTRESS &aStressP1, ADGNFORCE &aForceP2, ADGNSTRESS &aStressP2);	// Grid Influence Load.
	BOOL Get_Lcom(ElemPairK ElemK, T_LCOM_K LcomK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_ADD& StressAdd, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
				_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int iMaxMinType);  // Add by GAY. MNET:2903. ('07.07.31).
	BOOL Get_StldScfp(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE &rData);
	BOOL Get_SgldScfp(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE &rData);

	//////////////////////////////////////////////////////////////////////////
	BOOL Get_Stld_EX(ElemPairK ElemK, T_STLD_K StldK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_ADD_EX& StressAdd, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
																																							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);	// Static Load.

	BOOL Get_Splc_EX(ElemPairK ElemK, T_SPLC_K SplcK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);	// Spectrum.
	BOOL Get_Espc_EX(ElemPairK ElemK, T_ESPC_K EspcK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);	// Eccentric Response Spectrum.
	BOOL Get_This_EX(ElemPairK ElemK, T_THIS_K ThisK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);	// Time History.
	BOOL Get_Sgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, int iStepNo, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2);
	BOOL Get_Sgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, int nForType, BOOL bCalcStss, T_SGLD_D& SgldD, int iStepNo, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2, int Lcase=0);  
	BOOL Get_Sgld_Expo_EX(ElemPairK ElemK, T_SGLD_K SgldK, T_SGLD_D& SgldD, int iStepNo, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase);
	BOOL Get_Mvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE_EX &aForceLcase, ADGNSTRESS_EX &aStressLcase, ADGNSHRSADD_EX &aStssLcase,
							ADGNFORCE_EX &aForceP1, ADGNSTRESS_EX &aStressP1, ADGNFORCE_EX &aForceP2, ADGNSTRESS_EX &aStressP2, BOOL bElemAll=FALSE);	// Moving Load.
	BOOL Get_Mvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2, int iMaxMinType, BOOL bSpc=FALSE);// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL Get_Mvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE_EX &aForceLcase, BOOL bElemAll);
	BOOL Get_Smlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, CString& strLcaseNa, ADGNFORCE_EX &aForceLcase, ADGNSTRESS_EX &aStressLcase, ADGNSHRSADD_EX &aStssLcase,
							ADGNFORCE_EX &aForceP1, ADGNSTRESS_EX &aStressP1, ADGNFORCE_EX &aForceP2, ADGNSTRESS_EX &aStressP2);	// Settlement.
	BOOL Get_Smlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, int nForType, BOOL bCalcStss, CString& strLcaseNa, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2, int iMaxMinType);
	BOOL Get_Gilc_EX(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2, int iMaxMinType);
	BOOL Get_Gilc_EX(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, ADGNFORCE_EX &aForceLcase);
	BOOL Get_Gilc_EX(ElemPairK ElemK, T_GILC_K GilcK, int nForType, BOOL bCalcStss, int iLcaseType, CString& strLcaseNa, ADGNFORCE_EX &aForceLcase, ADGNSTRESS_EX &aStressLcase, ADGNSHRSADD_EX &aStssLcase,
							ADGNFORCE_EX &aForceP1, ADGNSTRESS_EX &aStressP1, ADGNFORCE_EX &aForceP2, ADGNSTRESS_EX &aStressP2);	// Grid Influence Load.
	BOOL Get_Lcom_EX(ElemPairK ElemK, T_LCOM_K LcomK, int nForType, BOOL bCalcStss, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_ADD_EX& StressAdd, _DGN_STRS_CRC_EX& StressLcase, _DGN_SHRS_ADD_EX& StssLcase,
							_DGN_FORC_CRC_EX& ForcePart1, _DGN_STRS_CRC_EX& StressPart1, _DGN_FORC_CRC_EX& ForcePart2, _DGN_STRS_CRC_EX& StressPart2, int iMaxMinType);  // Add by GAY. MNET:2903. ('07.07.31).
	BOOL Get_StldScfp_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE_EX &rData);
	BOOL Get_SgldScfp_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE_EX &rData);
	//////////////////////////////////////////////////////////////////////////

	void MakePLCBStldKey(int nDgnCode);
	
	// Set & Get Lcom Data.
	void Set_LcomDataForDesign();
	BOOL Get_LcomDataForDesignBinary(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D);
	
	BOOL Get_LcomDataForDesign(ElemPairK ElemK, int nForType, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, int iLcomNum=0);
	BOOL Get_LcomDataForDesign(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceTnpr, int iLcomNum=0);
	BOOL Get_LcomDataForDesign(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceTnpr, ADGNFORCE &aForceTnsc, int iLcomNum=0);
	BOOL Get_LcomDataForDesign(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceTnpr, ADGNFORCE &aForceTnsc, ADGNFORCE &aForceLTerm, ADGNFORCE &aForceSTerm, ADGNSHRSADD &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForDesign(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceTnsc, CArray<_DGN_CRCK_JTG,_DGN_CRCK_JTG>& aCrackForc, int iLcomNum=0);
	
	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK, int nForType, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceLcom, int iLcomNum=0);
	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceTnpr, int iLcomNum=0);
	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceTnpr, ADGNFORCE_EX &aForceTnsc, int iLcomNum=0);
	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceTnpr, ADGNFORCE_EX &aForceTnsc, ADGNFORCE_EX &aForceLTerm, ADGNSHRSADD_EX &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForDesign_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE_EX &aForceTnsc, CArray<_DGN_CRCK_JTG_EX,_DGN_CRCK_JTG_EX>& aCrackForc, int iLcomNum=0);
	
	BOOL Get_BeforeForce4PreCompo(ElemPairK ElemK, ADGNFORCE &aForceLcom, int nLcomNum);
	BOOL Get_BeforeForce4PreCompo_EX(ElemPairK ElemK, ADGNFORCE_EX &aForceLcom_EX, int nLcomNum);
	BOOL Get_LcomForce4ScfpSum(ElemPairK ElemK, int nLcomNum, ADGNFORCE &aForceLcom);
	BOOL Get_LcomForce4ScfpCrp(ElemPairK ElemK, int nLcomNum, ADGNFORCE &aForceLcom);
	BOOL Get_LcomForce4ScfpShr(ElemPairK ElemK, int nLcomNum, ADGNFORCE &aForceLcom);
	
	double AdjustCJJLoadCaseFactor( unsigned int iAnalType, _DGN_LCOM &LcomDesign, double dFactor, T_CHRP_D &chrpd, T_PSCD_D &pscd );
	
	BOOL Get_LcomDataForJtgTndn(ElemPairK ElemK, BOOL bAlways3D, ADGNFORCE &aForceJtgTndn, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNSTRSADD &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceTnpr, ADGNSTRSADD &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNFORCE &aForceTnpr, ADGNFORCE &aForceTnsc, ADGNSTRSADD &aStressAdd, ADGNFORCE &aForceLTerm, ADGNFORCE &aForceJtgTndn,
							ADGNSTRESS& aStressLcom, ADGNSTRESS& aStressTnpr, ADGNSTRESS& aStressTnsc, ADGNSTRESS& aStressLTerm, ADGNSHRSADD &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNSTRSADD &aStressAdd, ADGNFORCE &aForceLTerm, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNSHRSADD &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bAlways3D, ADGNFORCE &aForceLcom, ADGNSTRSADD &aStressAdd, ADGNSHRSADD &aStssLcom, int iLcomNum=0);
	// Stress.
	BOOL Get_LcomDataForStress(ElemPairK ElemK, int nForType, BOOL bIncludeTP, BOOL bAlways3D, ADGNSTRESS &aStressLcom, ADGNSTRESS &aStressTnpr, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNSTRESS &aStressLcom, ADGNSTRESS &aStressTnpr, ADGNSTRESS &aStressTnsc, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNSTRESS &aStressLcom, ADGNSTRSADD &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D, ADGNSTRESS &aStressLcom, ADGNSTRSADD &aStressAdd, ADGNSTRESS &aStressLTerm, ADGNSTRESS &aStressSTerm, int iLcomNum=0);
	//
	BOOL Get_LcomStressScfp(ElemPairK ElemK, int nLcomNum, ASCFPSTRESS &aCrD, ASCFPSTRESS &aShD, ASCFPSTRESS &aTeD, ASCFPSTRESS &aReD);
	// For One Load combination
	BOOL Get_LcomDataForDesign_LcomK(ElemPairK ElemK, int iDgnLcomK, BOOL bIncludeTP, BOOL bAlways3D, _DGN_FORC_CRC& ForceLcom, BOOL bCodeUnit=FALSE) const;


	//////////////////////////////////////////////////////////////////////////
	BOOL Get_LcomDataForJtgTndn_EX(ElemPairK ElemK, BOOL bAlways3D, ADGNFORCE_EX &aForceJtgTndn, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNSTRSADD_EX &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceTnpr, ADGNSTRSADD_EX &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceTnpr, ADGNFORCE_EX &aForceTnsc, 
								ADGNSTRSADD_EX &aStressAdd, ADGNFORCE_EX &aForceLTerm, ADGNFORCE_EX &aForceJtgTndn, 
								ADGNSTRESS_EX& aStressLcom, ADGNSTRESS_EX& aStressTnpr, ADGNSTRESS_EX& aStressTnsc, 
								ADGNSTRESS_EX& aStressLTerm, ADGNSHRSADD_EX &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNSTRSADD_EX &aStressAdd, ADGNFORCE_EX &aForceLTerm, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNSHRSADD_EX &aStssLcom, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bAlways3D,
								ADGNFORCE_EX &aForceLcom, ADGNSTRSADD_EX &aStressAdd, ADGNSHRSADD_EX &aStssLcom, int iLcomNum=0);
	// Stress.
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, int nForType, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNSTRESS_EX &aStressLcom, ADGNSTRESS_EX &aStressTnpr, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNSTRESS_EX &aStressLcom, ADGNSTRESS_EX &aStressTnpr, ADGNSTRESS_EX &aStressTnsc, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNSTRESS_EX &aStressLcom, ADGNSTRSADD_EX &aStressAdd, int iLcomNum=0);
	BOOL Get_LcomDataForStress_EX(ElemPairK ElemK, BOOL bIncludeTP, BOOL bAlways3D,
								ADGNSTRESS_EX &aStressLcom, ADGNSTRSADD_EX &aStressAdd, ADGNSTRESS_EX &aStressLTerm, int iLcomNum=0);
	//
	BOOL Get_LcomStressScfp_EX(ElemPairK ElemK, int nLcomNum, ASCFPSTRESS_EX &aCrD, ASCFPSTRESS_EX &aShD,
							ASCFPSTRESS_EX &aTeD, ASCFPSTRESS_EX &aReD);
	// For One Load combination
	BOOL Get_LcomDataForDesign_LcomK_EX(ElemPairK ElemK, int iDgnLcomK, BOOL bIncludeTP, BOOL bAlways3D, _DGN_FORC_CRC_EX& ForceLcom, BOOL bCodeUnit=FALSE);


	void Set_ElemDataForCheck(ElemPairK ElemK);
	BOOL GetDgnMemberForce(ElemPairK ElemK, T_LCOM_K nLcomK, _DGN_FORC_MEMB &rData);


	int Get_LcomDataCount();
	BOOL Get_LcomDataMap(CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K>& aLcomK);
	BOOL Get_LcomListDL(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 사하중시.
	BOOL Get_LcomListLL(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 설계시.
	BOOL Get_LcomListTL(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 온도시.
	BOOL Get_LcomListEQ(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 지진시.
	BOOL Get_LcomListUA(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 종국시A.
	BOOL Get_LcomListUB(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 종국시B.
	BOOL Get_LcomListUC(CArray<_DGN_LCOM,_DGN_LCOM>& aLcomDesign);	// 종국시C.
	int Get_LcomNumForStrn();
	int Get_LcomNumForStrs();
	int Get_LcomNumForSLS();
	BOOL Is_LcomForStrn(int iDgnLcomNo);
	BOOL Is_LcomForStrs(int iDgnLcomNo);
	BOOL Is_LcomForElst(int iDgnLcomNo);
	BOOL Is_LcomForServ(int iDgnLcomNo);
	BOOL Is_LcomForFati(int iDgnLcomNo);
	BOOL Is_LcomForFati2(int iDgnLcomNo);
    BOOL Is_LcomForTran(int nDgnLcomNo);
	BOOL Is_FatigueLcom(int iDgnLcomNo);
	BOOL Is_LongTerm(int iDgnLcomNo);
	BOOL Is_JtgStldLL(CString strLcaseType);
	BOOL Is_LcomForSLS(int iServLcomType);
	void Set_LcomKindIni();
	void Set_LcomKind(int iActiveKind, int iSubType, BOOL bElasticStage, T_LCOM_BASE& LcomBase, int& iDgnLcomKind);
	void Set_LcomKind_EuroCode(int iActiveKind, BOOL bElasticStage, T_LCOM_BASE& LcomBase, int& iDgnLcomKind);
	void Set_LcomKind_BSCode(int iActiveKind, T_LCOM_BASE& LcomBase, int& iDgnLcomKind);
	void Set_LcomKind_SNiP(int iActiveKind, BOOL bElasticStage, T_LCOM_BASE& LcomBase, int& iDgnLcomKind);
	void Set_LcomKind_AS_5100_5(int iActiveKind, BOOL bElasticStage, T_LCOM_BASE& LcomBase, int& iDgnLcomKind);
	int Get_FatigueLcomType();
	int Get_LcomKind(int iDgnLcomNo);
	int Get_ServLcomType(int iDgnLcomNo);
	int Get_FatigueType(int iDgnLcomNo);
	BOOL Get_LcomType(int iDgnLcomNo, CString& strOrgLcomNa, int& iMaxMinType);
	BOOL Get_LcomType(int iDgnLcomNo, CString& strOrgLcomNa, int& iOrgLcomNo, int& iSerLcomCount, int& iMaxMinType);
	int  GetOrgLcomKey(int iDgnLcomNo);
	// Add, Jaeoh. ('07.04.23)
	BOOL Is_TB05StldDL(CString strLcaseType);
	BOOL Is_TB05StldLL(CString strLcaseType);
	// Add, Jaeoh. [11/7/2008], 시공단계 Load Case를 위해.(TB05)
	BOOL Is_TB05SgldDL(CString strLcaseType);
	BOOL Is_LcomForMain(int iDgnLcomNo);
	BOOL Is_LcomForMainAdd(int iDgnLcomNo);
	BOOL Is_LcomForMainSpecial(int iDgnLcomNo);	

	BOOL IsEqLcom_JTG_B02(int iDgnLcomNo); // Only JTG/B02-01-2008	
	BOOL IsEqLoad_JTG_B02(int nAnalType, UINT iLcaseKey);
	BOOL IsSelfWeightLoad_JTG_B02(int nAnalType, UINT iLcaseKey);
	BOOL IsPermanentLoad_JTG_B02(int nAnalType, UINT iLcaseKey);
	BOOL Is_StldLL(CString strLcaseType, UINT nDgnCode);
	// Add by maxiao JTG/B02-01-2008
	BOOL IsPermanentLoad_JTG_B02(CString strLcaseType);
	BOOL IsSelfWeightLoad_JTG_B02(CString strLcaseType);

	// Add by GAY. MNET:2903. ('07.07.31). Distinguish Envelope from SRSS.
	int Get_CombType(unsigned int iLcaseKey, int iAnalType);	// 0=Add, 1=Envelope, 2=ABS, 3=SRSS.

	// Modify by GAY. MNET:2903. ('07.08.24). Get Load Case Combinatio Type. (D_LOADCASE_COMB_XXXX).
	int Get_LcaseCombType();

	// KR, Rating, KSCE-USD05
	int Get_RatingCase(int iDgnRcaseNo);
	BOOL Get_RcaseType(int iDgnRcaseNo, int& iOrgRcaseNo, int& iRatingCase, int& iRcaseTypeDL, int& iRcaseTypeLL);

	// Get Extra Ratio (할증계수).
	double Get_ExtraRatio(T_LCOM_D_UL& LcomD);
    double Get_ExtraRatio_KSCE(T_LCOM_D_UL& LcomD);
    double Get_ExtraRatio_JSCE(T_LCOM_D_UL& LcomD);
	double Get_ExtraRatio(int iDgnLcomNo);
	// Change by ZINU.('05.07.02). Consider Type for EL at Stage.
    BOOL GetErectionLoadType(CString strErName, T_STCT_D &rStct, CString &strLCType);
    BOOL GetErectionCSMaxFactor(T_STCT_D &rStct, T_CSLF_CPT &rCslf);
    BOOL IsBeforeLoadSgld(CString strLoadCaseName, T_STCT_D &rStct);
    BOOL IsTranferLoad(CString strLoadCaseName, T_STCT_D &rStct);
	BOOL IsSgldScfpKey(T_SGLD_K SgldK, T_STCT_D &StctD);
	BOOL IsErectionScfp(int nType, T_STCT_D &StctD);
	//int GetLcomType_Stld_AASHTO_StrnLcase(double dFactor, CString strLcase);
	//int GetLcomType_DC_Strn_AASHTO(double dFactor);
	//int GetLcomType_MV_Strn_AASHTO(double dFactor);
	//int Is_Stld_AASHTO_StrnLcase2(double dFactor, CString strLcase);
	//int GetLcomType_Stld_AASHTO_ServLcase(double dFactor, CString strLcase);
	//int GetLcomType_DC_Serv_AASHTO(double dFactor);
	//int GetLcomType_MV_Serv_AASHTO(double dFactor);
	//int Is_Stld_AASHTO_ServLcase2(double dFactor, CString strLcase);
	// Add by ZINU.('05.04.08). Get Remove Force, Stress (See JTG D62-04 6.3.1 p.58).
	// 장기=0, 단기=발생응력-허용응력(허용응력항을 0기준으로 맞추기 위해서).
	// 정단면 균열검토인 경우는 발생응력=발생응력-계수*TP응력.
	BOOL Get_RemoveForc(ElemPairK ElemK, int iDgnLcomNo, _DGN_FORC_CRC& RemoveForc);
	BOOL Get_RemoveStress(ElemPairK ElemK, int iDgnLcomNo, _DGN_STRS_CRC& RemoveStress);
	BOOL Get_RemoveStrs(ElemPairK ElemK, int iDgnLcomNo, double& dRemoveStrs);
	BOOL Get_RemoveShrs(ElemPairK ElemK, int iDgnLcomNo, double& dRemoveShrs);

	// Get Stresses by Forces.
	BOOL Get_StrsByForc(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_FORC_CRC& ForcCrc, _DGN_STRS_ADD& StrsAdd,
						_DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin);
	// Get Stresses by Stresses.
	BOOL Get_StrsByStrs(ElemPairK ElemK, T_LCOM_K LcomK,
						_DGN_STRS_CRC& StressCrc, _DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin);
	
	// 1=Short, 2=Long, 3=Short + Long
	BOOL Get_StrsByLcom(ElemPairK ElemK, int nForType, _DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin, int iSLTerm);
	BOOL Get_StrsByMaxi(ElemPairK ElemK, _DGN_STRS_CRC& StrsTop, _DGN_STRS_CRC& StrsBot);
	BOOL Get_StrsBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin);
	BOOL Get_ShrsByForc(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_FORC_CRC& ForcCrc, _DGN_STRS_ADD& StrsAdd, _DGN_SHRS_ADD& StssLcom,
					  _DGN_SHRS_CRC& ShrsMax, _DGN_SHRS_CRC& ShrsMin, int iShrTor, BOOL bPseudo);
	BOOL Get_ShrsByLcom(ElemPairK ElemK, _DGN_SHRS_CRC& ShrsMax, _DGN_SHRS_CRC& ShrsMin, int iShrTor, BOOL bPseudo, enService ServiceFilterLcom = enService::None);
	BOOL Get_ShrsBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_SHRS_CRC& ShrsMax, _DGN_SHRS_CRC& ShrsMin, int iShrTor);
	BOOL Get_PrincipalStressByLcom(ElemPairK ElemK, T_SECT_D &SectD, T_RPSC_D &RpscMbar, T_RPSC_D &RpscSbar, T_STRSPROP_D &StrsPropD,
																 int nDgnLcomK, BOOL bPseudo,_DGN_FORC_CRC &ForceLcom, _DGN_SHRS_ADD &StssLcom);
	BOOL Get_ForceByTnPr(ElemPairK ElemK, T_STRB_D& StrbMax, T_STRB_D& StrbMin);
	BOOL Get_ForceByTnPr(ElemPairK ElemK, _DGN_FORC_CRC& rData);
	BOOL Get_ForceByTnPr(ElemPairK ElemK, double *pdForceTpI, double *pdForceTpJ);
	void Set_SectHeight4PscGen(double *dHeight);

	BOOL Get_StrsPartBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, int nPartId,
								_DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin);
	BOOL Get_ShrsPartBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, int nPartId,
							_DGN_SHRS_CRC& ShrsMax, _DGN_SHRS_CRC& ShrsMin, int iShrTor);
	BOOL Get_CompTensStress(int nPos, _DGN_STRS_CRC &StrsD, double &dStr_c, double &dStr_t);
	BOOL Get_CompTensPrincipalStress(int nPos, _DGN_SHRS_ADD &StrsD, double &dStrp_c, double &dStrp_t);
    BOOL GetStageLoadDgnType(CString strLoadCaseName, T_STCT_D &rStct, CString &strDgnType, CString &strErLCType);

    BOOL Get_StrsByLcomBS(ElemPairK ElemK, int nForType, _DGN_STRS_CRC& StrsMax, _DGN_STRS_CRC& StrsMin, int iClass[2] );
    BOOL Get_Strs4Tranfer(T_STCT_D &StctD, ElemPairK ElemK, BOOL bComposite, int nDgnLcomNo, T_STAG_K StagK, int nStepNo,
        _DGN_STRS_CRC& Strs, _DGN_STRS_CRC& StrsLT, _DGN_STRS_CRC& StrsST, 
        _DGN_STRS_CRC& StrsP2, _DGN_STRS_CRC& StrsLTP2, _DGN_STRS_CRC& StrsSTP2);
    BOOL Get_Force4Tranfer(T_STCT_D &StctD, ElemPairK ElemK, int nDgnLcomNo, T_STAG_K StagK, int nStepNo, _DGN_FORC_CRC& ForLC, _DGN_FORC_CRC& ForLT, _DGN_FORC_CRC& ForST, _DGN_FORC_CRC& ForTnpr);
    BOOL GetMyForceLcom(ElemPairK ElemK, _DGN_FORC_CRC& rForMax, _DGN_FORC_CRC& rForMin);

	// For DEBUG.
	void Debug_PrintOpt(int iDebugType);
	void Debug_PrintOpt(int iDebugElem, int iDebugPart, int iDebugLcom);  

	////////////////////////////////////////
	// Bridge Load Rating
	////////////////////////////////////////
protected:  
	double m_dHeight[2];
	std::map<ElemPairK, PRESTRESS_STLD_KEY> m_mapPrstStldKey;
	std::map<ElemPairK, PRESTRESS_STLD_KEY> m_mapTdplStldKey;
	BOOL Get_Stld_TndnSecondary(ElemPairK ElemK, T_STLD_K StldK, double dFactor, BOOL bReciLcase);
	BOOL Get_Stld_TndnSecondary(ElemPairK ElemK, T_STLD_K StldK, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase);
	BOOL Get_Stld_TndnSecondary_EX(ElemPairK ElemK, T_STLD_K StldK, double dFactor, BOOL bReciLcase);
	BOOL Get_Stld_TndnSecondary_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_FORC_CRC_EX& ForceLcase, _DGN_STRS_CRC_EX& StressLcase);

	BOOL Get_Force4RatingDL(_DGN_FORC_CRC& ForceLcom, _DGN_FORC_CRC& ForceTnpr, _DGN_FORC_CRC& ForceTnsc);
	BOOL Get_Force4RatingLL(_DGN_FORC_CRC& ForceMove);
	BOOL Get_Stress4RatingDL(_DGN_STRS_CRC& StressLcom);
	BOOL Get_Stress4RatingLL(_DGN_STRS_CRC& StressMove);
	BOOL Get_PreForceStress4RatingDL(_DGN_FORC_CRC& ForceLcom, _DGN_STRS_CRC& StressLcom);

	BOOL Get_Force4RatingDL_EX(_DGN_FORC_CRC_EX& ForceLcom, _DGN_FORC_CRC_EX& ForceTnpr, _DGN_FORC_CRC_EX& ForceTnsc);
	BOOL Get_Force4RatingLL_EX(_DGN_FORC_CRC_EX& ForceMove);
	BOOL Get_Stress4RatingDL_EX(_DGN_STRS_CRC_EX& StressLcom);
	BOOL Get_Stress4RatingLL_EX(_DGN_STRS_CRC_EX& StressMove);
	BOOL Get_PreForceStress4RatingDL_EX(_DGN_FORC_CRC_EX& ForceLcom, _DGN_STRS_CRC_EX& StressLcom);

    BOOL Get_RCaseLcom(T_RLCS_D &Rlcs, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, T_LCOM_D &rLcom);

public:
	std::map<ElemPairK, FORCE_BLRD_ELEM> m_arBlrdForceElem;
	std::map<ElemPairK, STRESS_BLRD_ELEM> m_arBlrdStressElem;
	std::map<ElemPairK, STRESS_BLRD_ELEM> m_arBlrdSlabStressElem;
	std::map<ElemPairK, FORCE_BLRD_ELEM> m_arBlrdPreCompoForecElem;
	// For Rating-KR
	std::map<ElemPairK, FORCE_LRKR_ELEM> m_arBlrdForceElemKR; //  Just Rating  - Get Check Item
	std::map<ElemPairK, FORCE_LRKR_ELEM> m_arBlrdForceElemKR_Str; // Rating Strength Check -  Get all
	std::map<ElemPairK, STRESS_LRKR_ELEM> m_arBlrdStressElemKR;
	std::map<ElemPairK, STRESS_LRKR_ELEM> m_arBlrdStressElemKR_Str;
	 // Pre-Composite에 해당하는 Force/Stress
	std::map<ElemPairK, FORCE_RCASE_PRECOMP_ELEM> m_aPreCompForceElem;
	//CMap<T_ELEM_K,T_ELEM_K,FORCE_LRKR_COMPO_ELEM,FORCE_LRKR_COMPO_ELEM&>   m_aBlrdComForceElem;
	//CMap<T_ELEM_K,T_ELEM_K,STRESS_LRKR_COMPO_ELEM,STRESS_LRKR_COMPO_ELEM&> m_aBlrdComStressElem;


	void Set_RatingCaseDataForBLR();
	void Set_StlRatingCaseDataForBLR();
	void PrestressStldKey(ArrElemPairKey& arElemK);
    int  GetStlRCDgnLcomKey(const int nLCSize, const int nConcSize, int nRCase, int nLC, int nConc);

    BOOL Get_ForceStressPSCDataForAASHTO(ElemPairK ElemK, CArray<FORCE_BLRD_RCASE, FORCE_BLRD_RCASE>& arForceRCase, CArray<STRESS_BLRD_RCASE, STRESS_BLRD_RCASE>& arStressRCase, CArray<STRESS_BLRD_RCASE, STRESS_BLRD_RCASE>& arSlabStressRCase, CArray<FORCE_BLRD_RCASE, FORCE_BLRD_RCASE>& arPreCompoRCase);
    BOOL Get_ForceStressSTLDataForAASHTO(ElemPairK ElemK, CArray<FORCE_BLRD_RCASE, FORCE_BLRD_RCASE>& arForceRCase, CArray<STRESS_BLRD_RCASE, STRESS_BLRD_RCASE>& arStressRCase, CArray<STRESS_BLRD_RCASE, STRESS_BLRD_RCASE>& arSlabStressRCase, CArray<FORCE_BLRD_RCASE, FORCE_BLRD_RCASE>& arPreCompoRCase);

    BOOL Get_BlrdForceStress(_DGN_FORC_CRC& ForceRCase, _DGN_STRS_CRC& StressRCase);
	BOOL Get_BlrdForceStress_EX(_DGN_FORC_CRC_EX& ForceRCase, _DGN_STRS_CRC_EX& StressRCase);

	// BLR, KSCE-USD05
	void Set_RatingCaseDataForKR05_BLR(const int &nDgnCode);
    void Set_RatingCaseDataForKR05_BLR_PSC(const int &nDgnCode);
    void Set_RatingCaseDataForKR05_BLR_RC(const int &nDgnCode);
    void Set_RatingCaseDataForKR05_BLR_STL(const int &nDgnCode);
	BOOL Get_ForceStressDataForRatingKSCE05(BOOL bIsPLCB, ElemPairK ElemK,
											CArray<FORCE_RCASE_KR,FORCE_RCASE_KR>& arForceRCase, 
											CArray<STRESS_RCASE_KR,STRESS_RCASE_KR>& arStressRCase, 
											FORCE_RCASE_PRECOMP_ELEM &ForcePreComElem,
											const int &nDgnCode, BOOL bAll);
	void Set_RatingCaseForceData(std::map<ElemPairK, FORCE_BLRD_ELEM>& mapBlrdForceElem);

    // PMS.5862 BS BD21 Assessment
	std::map<ElemPairK, FORCE_ASBD_ELEM> m_arBlrdForceElemBD; //  Just Rating  - Get Check Item
	std::map<ElemPairK, STRESS_ASBD_ELEM> m_arBlrdStressElemBD; //  Just Rating  - Get Check Item
	std::map<ElemPairK, STRESS_ASBD_ELEM> m_arBlrdSlabStressElemBD; //  Just Rating  - Get Check Item

    void Set_RatingCaseDataForBD21();
    BOOL Get_ForceStressDataForRatingBD21(ElemPairK ElemK,
                                          CArray<FORCE_RCASE_BD,FORCE_RCASE_BD>& arForceRCase, 
                                          CArray<STRESS_RCASE_BD,STRESS_RCASE_BD>& arStressRCase, 
                                          CArray<STRESS_RCASE_BD,STRESS_RCASE_BD>& arSlabStressRCase, 
                                          FORCE_RCASE_PRECOMP_ELEM &ForcePreComElem);

	////////////////////////////////////////
	// Composite Plate Girder Design.
	////////////////////////////////////////
protected:
	

public:
	BOOL Get_CompoForceBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_FORC_CRC& ForcMax, _DGN_FORC_CRC& ForcMin, BOOL bCompo=TRUE);
	BOOL Get_ForceBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_FORC_CRC& ForcMax, _DGN_FORC_CRC& ForcMin);
	BOOL Get_CompositeForceBySgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, double dFactor, _DGN_FORC_CRC& ForcMax, _DGN_FORC_CRC& ForcMin);
    BOOL Get_GirderForce4CSLoadOfLCOM(BOOL bIsRating, T_STCT_D &StctD, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, int nDgnLcomKey, int nType, _DGN_FORC_CRC& ForceCS, double dFactorDL=1.0, BOOL bUnfactored=FALSE);
    BOOL Get_GirderForce4CStage(T_STCT_D &StctD, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, T_CSLF_CPT &CslFac, _DGN_FORC_CRC& ForceCS);
	BOOL Get_CSGirderForce4CompoBefore(ElemPairK ElemK, _DGN_FORC_CRC& ForD);
	BOOL Get_SfcpExist(int nLcomType, int nOrgLcomK, bool& bExistCrp,bool& bExistShr, bool& bExistBST);
	BOOL GetCSGirderMembForce(T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, ArrElemPairKey &aElemList, T_STRB_D* pDataMax);
    BOOL Get_GirderMembForce4CSLoadOfLCOM(BOOL bIsRating, T_STCT_D &StctD, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, int nDgnLcomKey, int nType, ArrElemPairKey& aElemList, T_STRB_D* pMembF, double dFactorDL=1.0);
    BOOL Get_GirderMembForce4CStage(T_STCT_D &StctD, T_STAG_K GirderLastStagK, int nStepNo, T_CSLF_CPT &CslFac, ArrElemPairKey& aElemList, T_STRB_D* pDataMax);
    BOOL GetGirderLastStageKey(ElemPairK ElemK, T_STAG_K &rLastStagK, int &nLastStepNo);
    BOOL Get_ForceData4Stage(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int nStepNo, _DGN_FORC_CRC& ForceD, _DGN_STRS_CRC& StressD);
    BOOL Get_CompoStress4Stage(ElemPairK ElemK, int nPart, T_SGLD_K SgldK, T_STAG_K StagK, int nStepNo, _DGN_STRS_CRC& StrsD);

	BOOL Get_CompoForceBySgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_FORC_CRC_EX& ForcMax, _DGN_FORC_CRC_EX& ForcMin, BOOL bCompo=TRUE);
	BOOL Get_ForceBySgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, _DGN_FORC_CRC_EX& ForcMax, _DGN_FORC_CRC_EX& ForcMin);
	BOOL Get_CompositeForceBySgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, T_STAG_K StagK, int iStepNo, double dFactor, _DGN_FORC_CRC_EX& ForcMax, _DGN_FORC_CRC_EX& ForcMin);
	BOOL Get_GirderForce4CSLoadOfLCOM_EX(ElemPairK ElemK, int nDgnLcomKey, _DGN_FORC_CRC_EX& ForceCS);
    BOOL Get_GirderForce4CSLoadOfLCOM_EX(BOOL bIsRating, T_STCT_D &StctD, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, int nDgnLcomKey, int nType, _DGN_FORC_CRC_EX& ForceCS, double dFactorDL=1.0);

	BOOL   GetMembForce(ArrElemPairKey& aElemList, CArray<T_STRB_D,T_STRB_D&> &aDataMax, T_STRB_D* pDataMax, DGN_MY_MAX_B& MyMaxD);
	double GetMaxMyByMembForce(T_STRB_D* pMembForD);
	void ConvertDgnForceToStrb(_DGN_FORC_CRC_EX &For, T_STRB_D &rData);
    BOOL IsElem7DOF(ElemPairK ElemK, int nDgnCode);

protected:
	BOOL IsMVLcaseConcurrentForce(T_MVLD_K MvldK);
	CString GetConCurrForceType(int nId);
	void GetStressPointData4STSS(T_STRSPOST_BASE &StrsBase, T_STRSPROP_BASE &PropB, double *pdY, double *pdZ, double *pdW, double *pdXYsh, double *pdXZsh);
	BOOL IsElem7DOF(ElemPairK ElemK);
    BOOL IsCode7DOF(int nDgnCode);
    BOOL IsLcom7DOF(int nDgnCode);
	int  GetConCurrForceNum(BOOL bIs7Dof);
	int  GetForceTypeNum(BOOL bIs7Dof);
    int  GetLcomSubTypeIndex(const BOOL& bMaxMin, const BOOL& bConcurrent, const int& nIndex);

protected:
	// Factory functions Protect stack overflow
	_DGN_FORC_CRC_EX* Factory_Creat_DGN_FORC_CRC_EX();
	_DGN_STRS_ADD_EX* Factory_Creat_DGN_STRS_ADD_EX();
	_DGN_SHRS_ADD_EX* Factory_Creat_DGN_SHRS_ADD_EX();
	_DGN_STRS_CRC_EX* Factory_Creat_DGN_STRS_CRC_EX();
	_DGN_SCFP_PART_LCASE_EX* Factory_Creat_DGN_SCFP_PART_LCASE_EX();
	
	//////////////////////////////////////////////////////
	BOOL Get_LcomDataForDesign_Org(ElemPairK ElemK, int nForType, int iDgnLcomKind, BOOL bIncludeTP, BOOL bAlways3D,
		ADGNFORCE& aForceLcom,
		ADGNFORCE& aForceStag,
		ADGNSHRSADD& aShrsStage,
		ADGNFORCE& aForceTnpr,
		ADGNFORCE& aForceTnsc,
		ADGNSTRSADD& aStressAdd,
		ADGNFORCE& aForceLTerm,
		ADGNFORCE& aForceJtgTndn,
		// Stres
		ADGNSTRESS& aStressLcom,
		ADGNSTRESS& aStressTnpr,
		ADGNSTRESS& aStressTnsc,
		ADGNSTRESS& aStressLTerm,
		// Principle
		ADGNSHRSADD& aStssLcom, BOOL bBinary);

	BOOL Get_LcomData(int iLcom, T_LCOM_IN& paramD, T_LCOM_OUT& outD);
	BOOL Get_LcomDataEX(int iLcom, T_LCOM_IN& paramD, T_LCOM_EX_OUT& outD);

	BOOL Get_EnvelopLcom_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD, BOOL bReciInput);
	BOOL Get_Stld_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_Splc_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_Espc_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_Sepc_mt(T_LCOM_IN& paramD, unsigned int iLcaseKey, int iLcaseType, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
	BOOL Get_This_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_This_mt(T_LCOM_IN& paramD, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
	BOOL Get_Sgld_mt(T_LCOM_IN& paramD, T_SGLD_K SgldK, T_SGLD_D& SgldD, int iStepNo, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, int Lcase=0);
	BOOL Get_Stld_mt(T_LCOM_IN& paramD, T_STLD_K StldK, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_ADD& StressAdd, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1,
		_DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
	BOOL Get_Lcom_mt(T_LCOM_IN paramD, T_LCOM_K LcomK, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_ADD& StressAdd, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
	BOOL Get_Force_mt(T_LCOM_FORC& forcD, T_LCOM_OUT& outD);
	BOOL Get_Stress_mt(T_LCOM_FORC& forcD, T_LCOM_OUT& outD);
	BOOL Get_Lcom_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD, T_LCOM_K LcomK);
	BOOL Get_Sgld_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD, int iStepNo);
	BOOL Get_Mvld_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD, BOOL bSpc = FALSE);
	BOOL Get_Sbcf_mt(T_LCOM_IN& paramD, unsigned int iLcaseKey, int iLcaseType, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2, BOOL bSpc=FALSE);	
	BOOL Get_Smlc_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_Gilc_mt(T_LCOM_IN& paramD, T_LCOM_FORC& forcD);
	BOOL Get_Gilc_mt(T_LCOM_IN& paramD, T_GILC_K GilcK, int iLcaseType, CString& strLcaseNa, _DGN_FORC_CRC& ForceLcase, _DGN_STRS_CRC& StressLcase, _DGN_SHRS_ADD& StssLcase,
		_DGN_FORC_CRC& ForcePart1, _DGN_STRS_CRC& StressPart1, _DGN_FORC_CRC& ForcePart2, _DGN_STRS_CRC& StressPart2);
		
	BOOL Get_LcomDataForDesign_EX_org(ElemPairK ElemK, int nForType, int iDgnLcomKind, BOOL bIncludeTP, BOOL bAlways3D,
		ADGNFORCE_EX &aForceLcom, ADGNFORCE_EX &aForceStag, ADGNSHRSADD_EX &aShrsStage, ADGNFORCE_EX &aForceTnpr, ADGNFORCE_EX &aForceTnsc, ADGNSTRSADD_EX &aStressAdd, ADGNFORCE_EX &aForceLTerm, ADGNFORCE_EX &aForceJtgTndn,
		ADGNSTRESS_EX& aStressLcom, ADGNSTRESS_EX& aStressTnpr, ADGNSTRESS_EX& aStressTnsc, ADGNSTRESS_EX& aStressLTerm, ADGNSHRSADD_EX &aStssLcom, BOOL bBinary = FALSE);
	BOOL Get_Lcom_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Force_EX_mt(T_LCOM_EX_OUT& outD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Stress_EX_mt(T_LCOM_EX_OUT& outD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Stld_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Espc_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_This_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Sgld_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD, int iStepNo);
	BOOL Get_Splc_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Smlc_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Gilc_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD);
	BOOL Get_Mvld_EX_mt(T_LCOM_IN& paramD, T_LCOM_EX_FORC& forcD, BOOL bSpc = FALSE);
	void ChkMultiThreadStatus();
}; 

#include "HeaderPost.h"

#endif // !defined(__CRCFORCECTRL_H__)



