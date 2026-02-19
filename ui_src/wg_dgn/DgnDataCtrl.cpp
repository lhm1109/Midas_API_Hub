// DgnDataCtrl.cpp: implementation of the CDgnDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

#include "DgnDynData.h"					// for m_arKfacData, m_arReduData, m_arReduKind.

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_VectorUtil.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_StoryData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DesignResult.h"
#include "..\wg_db\wg_db_TendonInfo.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\CFSDgnTool.h"
#include "..\wg_db\StlDgnTool.h"
#include "..\wg_db\BeamDiagram.h"
#include "..\wg_db\qFactor.h"

#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_Product.h"

#include "..\dgnengine\src\DgnBase\DgnBaseDef.h"
#include "..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"

// For debugging.
#include "..\MIT_Lib\rptostream.h"
#include "DgnForceCtrl.h"
#include "DgnTypeConverter.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// for SLS
#define LOCAL_DISP            0
#define LOCAL_INC_NODAL_DISP  1
#define GLOBAL_DISP           2
#define Div_Num               18
#define Div_Num_Cantilever    199

using namespace std;
using namespace dgn::def;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDataCtrl::CDgnDataCtrl()
{
	// Get DBDoc Pointer.
	m_pDoc = CDBDoc::GetDocPoint();
	m_pMembCtrl = CMembCtrl::GetMembCtrl();
	if (m_pDoc == 0 || m_pMembCtrl == 0)	return;
	m_strTestFile = _T("");

	m_IsTestMode = FALSE;

	m_pMembCtrl->Reset_EndOffSet_ElemList();

	m_iDgnStatus = 0;
}

CDgnDataCtrl::~CDgnDataCtrl()
{
}

BOOL CDgnDataCtrl::Get_DgnStlStfn(int key, BOOL& bFlag, T_STFN_D& rData)
{
	// Key is Elem.
	bFlag = FALSE;
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData)) { return FALSE; }// By Show Key.

	T_SECT_D sData;
	sData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro, sData)) { return FALSE; }
	if (sData.nStype == D_SECT_TYPE_REGULAR && sData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
	{
		if (m_pDoc->m_pAttrCtrl->ExistStfn(eData.elpro))
		{
			bFlag = TRUE;
			if (!m_pDoc->m_pAttrCtrl->GetStfn(eData.elpro, rData)) { return FALSE; }
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlSlrs(int key, T_SLRS_D& rData)
{
	// Key is Elem.
	rData.Initialize();

	T_ELEM_K ShowKey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, ShowKey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistSlrs(ShowKey)) // ���纰 �Էµ����Ͱ� �����ϸ�,
	{
		if (!m_pDoc->m_pAttrCtrl->GetSlrs(ShowKey, rData)) { return FALSE; }
	}
	else // ���纰 �Էµ����Ͱ� �������� ������, ��ü Global Data���� �о�ͼ�? Setting.
	{
		T_DSTL_D DstlD;
		if (!m_pDoc->m_pAttrCtrl->GetDstl(DstlD)) DstlD.Initialize();

		rData.nFrameType = DstlD.nFrameType;
		rData.bCheck = DstlD.bSpecialSeismic;
	}

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlBcif(int key, T_BCIF_D& rData)
{
	// Key is Elem.
	rData.Initialize();

	T_ELEM_K ShowKey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, ShowKey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistBcif(ShowKey)) // ���纰 �Էµ����Ͱ� �����ϸ�,
	{
		if (!m_pDoc->m_pAttrCtrl->GetBcif(ShowKey, rData)) { return FALSE; }
		return TRUE;		
	}
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnGenMatd(int key, T_MATD_D& rData)
{
	// Key is Elem.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }
	//PMS:4092
	if (!m_pDoc->m_pAttrCtrl->GetDgnGenMatd(Showkey, rData)) {/* ASSERT(0);*/ return FALSE; }

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenMatd4Rpt(T_MATL_K MatlK, T_MATD_D& rData)
{
	rData.Initialize();
	// Get Material Data.
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, rData)) { return FALSE; }

	const CString& strMatlType = rData.Type;
	if (strMatlType == _T("S")) { Get_DgnStlMatd(rData); }
	else if (strMatlType == _T("C")) { Get_DgnConMatd(rData); }
	else if (strMatlType == _T("SRC")) { Get_DgnSrcMatd(rData); }
	else { return FALSE; }

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnSrcMatd(T_MATD_D& mData)
{
	//PMS:4092
	return m_pDoc->m_pAttrCtrl->GetDgnSrcMatd(mData);
}

BOOL CDgnDataCtrl::Get_DgnConMatd(T_MATD_D& mData)
{
	//PMS:4092
	return m_pDoc->m_pAttrCtrl->GetDgnConMatd(mData);
}

BOOL CDgnDataCtrl::Get_DgnStlMatd(T_MATD_D& mData)
{
	//PMS:4092
	return m_pDoc->m_pAttrCtrl->GetDgnStlMatd(mData);
}

BOOL CDgnDataCtrl::Get_DgnStlDstl(T_DSTL_D& sData)
{
	sData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDstl())
	{
		m_pDoc->m_pAttrCtrl->GetDstl(sData);
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		sData.DesignCode = rPref.DgnCode.SteelCode;
		sData.nNationalAnnex = rPref.DgnCode.nStlNationalAnnex;
		// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50017-03, GBJ17-88).
		int iSafeLevel = 0, iEqGrade = 0;
		if (Get_DgnStlChinaDef(sData.DesignCode, iSafeLevel, iEqGrade))
		{
			if (sData.bSpecialSeismic)	sData.nClass = iEqGrade;
			else						sData.nClass = iSafeLevel;
		}

		if(sData.DesignCode == STLCODE_EC3_05)
		{
			switch(sData.nNationalAnnex)
			{
				case dgn::def::enNationalAnnex::Austria:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Belgium:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::CzechRepublic:	sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Denmark:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::France:			sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::Luxembourg:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::Netherlands:	sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Poland:			sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Romania:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::Slovakia:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Slovenia:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_B; break;
				case dgn::def::enNationalAnnex::Sweden:			sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				case dgn::def::enNationalAnnex::Sweden19:		sData.nInteractionKijType = EN_EC3_KIJ_ANNEX_A; break;
				default:										sData.nInteractionKijType = EN_EC3_KIJ_BY_CODE; break;
			}
		}
	}
	double pdPhi[7] = { 0.,0.,0.,0.,0.,0.,0. };

	int iDgnStatusOrg = m_iDgnStatus;
	m_iDgnStatus = STEEL;

	if (m_pDoc->EnableJUD() && sData.DesignCode == STLCODE_AIJ_ASD02)
	{
		sData.bAllFrameLaterallyBraced = FALSE; // iGen������ _T("AIJ-ASD02")�����϶� �ش� �ɼ��� ���������� Off�θ� ����ϱ�� �Ͽ���
	}

	m_iDgnStatus = iDgnStatusOrg;

	if (!Get_DgnStlPhiDef(sData.DesignCode, pdPhi, sData.nNationalAnnex))  ASSERT(0);
	for (int i = 0; i < 5; i++) // PCS, v851 �и� �� 5 -> 7 ���� ���?
	{
		if (sData.dPhi[i] == 0.0)
		{
			sData.dPhi[i] = pdPhi[i];
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnCfsDcfs(T_DCFS_D& sData)
{
	sData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcfs())
	{
		m_pDoc->m_pAttrCtrl->GetDcfs(sData);
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		sData.DesignCode = rPref.DgnCode.CFSteelCode;
		sData.nNationalAnnex = rPref.DgnCode.nCfsNationalAnnex;
	}

	double pdPhi[5] = { 0.,0.,0.,0.,0. };

	if (!Get_DgnCfsPhiDef(sData.DesignCode, pdPhi, sData.nNationalAnnex))  ASSERT(0);
	for (int i = 0; i < 5; i++)
	{
		if (sData.dPhi[i] == 0.0)
		{
			sData.dPhi[i] = pdPhi[i];
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenLtsr(int key, double dFy1, T_LTSR_D& rData)
{
	// Key is Elem.
	rData.Initialize();
	////////////////////////////////////////////////
	//  rData.bNotCheck : Not auto-calculation.
	//  rData.CLMT      : Not auto-calculation.
	//  rData.TLMT      : Not auto-calculation.
	////////////////////////////////////////////////

	// Change by ZINU.('03.02.17). dFy is Only used at GBJ17-88, GB50017-03 (by Code Units).
	// Change by ZINU.('06.05.22). dFy -> dFy1. See GB50011-01 8.3.1 p.91.
	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	// Get Matl.
	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!Get_DgnGenMatd(key, MatdD))	return FALSE;

	BOOL bChkLtsr = FALSE;
	if (MatdD.Type == _T("S")) { bChkLtsr = TRUE; }
	else if (MatdD.Type == _T("SRC")) { bChkLtsr = TRUE; }

	auto l_SetNotChkLtsr = [](T_LTSR_D& rLtsrD)
	{
		rLtsrD.bNotCheck = TRUE;
		rLtsrD.CLMT = 0.0;
		rLtsrD.TLMT = 0.0;
	};

	if (!bChkLtsr)
	{
		l_SetNotChkLtsr(rData);
		return TRUE;
	}

	T_DSTL_D DstlD;
	DstlD.Initialize();
	Get_DgnStlDstl(DstlD);
	const CString& strStlCode = DstlD.DesignCode;

	if (m_pDoc->m_pAttrCtrl->ExistLtsr(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetLtsr(Showkey, rData))	return FALSE;
		if (MatdD.Type == _T("S") && (strStlCode == STLCODE_GBJ17_88 || strStlCode == STLCODE_GB50017_03 || strStlCode == STLCODE_GB50017_15 || strStlCode == STLCODE_GB50017_17 || strStlCode == STLCODE_JGJ209_2010))//add by maxiao(2015-9-25)GB50017-15
		{
			double dCF = (dFy1 > 0.0 ? sqrt(235.0 / dFy1) : 1.0);
			rData.CLMT *= dCF;
			rData.TLMT *= dCF;
		}
		if (rData.bNotCheck) { rData.CLMT = 0.0; rData.TLMT = 0.0; }
	}
	else
	{
		if (MatdD.Type == _T("S"))
		{
			if (strStlCode == STLCODE_GBJ17_88 || strStlCode == STLCODE_GB50017_03 || strStlCode == STLCODE_GB50017_15)//add by maxiao(2015-9-25)GB50017-15
			{
				// Changed by ParkBong.('07.06.28) 
				BOOL bHighRised = Is_HighRiseForGBJ();
				BOOL bCode_CH_17_03 = DstlD.DesignCode == STLCODE_GB50017_03;
				BOOL bEqSpecial = DstlD.bSpecialSeismic;
				int iDgnClass = DstlD.nClass + 1;
				T_GREQ_D GreqD;
				GreqD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetGreq(Showkey, GreqD))
				{
					iDgnClass = GreqD.iEqGradeStl + 1;
				}
				int iMembType = Get_DgnGenMbtp(Showkey);
				double dCF = (dFy1 > 0.0 ? sqrt(235.0 / dFy1) : 1.0);
				double dSlenR = 0.0;
				double dSlenR_T = 200.0;
				// See Copies from Kye CJ.('03.02.17).
				if (iMembType == D_MBTP_COLUMN)	// Column.
				{
					if (bCode_CH_17_03 == FALSE)
					{
						if (bEqSpecial)	// Seismic.
						{
							switch (iDgnClass)
							{
							case 1: { dSlenR = (bHighRised ? 60.0 : 100.0); } break; // Intensity 9.
							case 2: { dSlenR = (bHighRised ? 60.0 : 120.0); } break; // Intensity 8.
							case 3: { dSlenR = (bHighRised ? 80.0 : 120.0); } break; // Intensity 7.
							case 4: { dSlenR = (bHighRised ? 120.0 : 120.0); } break; // Intensity 6.
							default: { dSlenR = 150.0; } break;
							}
						}
						else
						{
							dSlenR = 150.0;
						}
					}
					else
					{
						if (bEqSpecial)
						{
							switch (iDgnClass)
							{
							case 1: { dSlenR = (bHighRised ? 60.0 : 60.0); } break; // Intensity 9.
							case 2: { dSlenR = (bHighRised ? 70.0 : 80.0); } break; // Intensity 8.
							case 3: { dSlenR = (bHighRised ? 80.0 : 100.0); } break; // Intensity 7.
							case 4: { dSlenR = (bHighRised ? 100.0 : 120.0); } break; // Intensity 6.
							default: { dSlenR = (bHighRised ? 100.0 : 150.0); } break;
							}
							dSlenR_T = dSlenR;
						}
						else
						{
							dSlenR = (bHighRised ? 100.0 : 150.0);
							dSlenR_T = dSlenR;
						}
					}
				}
				else if (iMembType == D_MBTP_BRACE)	// Brace.
				{
					if (bCode_CH_17_03 == FALSE)
					{
						if (bEqSpecial)	// Seismic.
						{
							switch (iDgnClass)
							{
							case 1: { dSlenR = (bHighRised ? 60.0 : 120.0); } break; // Intensity 9.
							case 2: { dSlenR = (bHighRised ? 90.0 : 120.0); } break; // Intensity 8.
							case 3: { dSlenR = (bHighRised ? 120.0 : 150.0); } break; // Intensity 7.
							case 4: { dSlenR = (bHighRised ? 120.0 : 150.0); } break; // Intensity 6.
							default: { dSlenR = 150.0; } break;
							}
						}
						else
						{
							dSlenR = 150.0;
						}
					}
					else
					{
						if (bEqSpecial && (iDgnClass == 1 || iDgnClass == 2 || iDgnClass == 3))
						{
							dSlenR = 120.0;
							dSlenR_T = 120.0;
						}
						else
						{
							T_MBTP_D MbtpData;
							if (Get_DgnGenMbtp(Showkey, MbtpData) == FALSE)
								MbtpData.Initialize();
							if (bEqSpecial && iDgnClass == 4)
							{
								dSlenR = 120;
								if (MbtpData.nSubType == 0)
								{
									dSlenR_T = 180.0;
								}
								else
								{
									dSlenR_T = 120.0;
								}
							}
							else
							{
								dSlenR = 150.0;
								if (MbtpData.nSubType == 0)
								{
									dSlenR_T = (bHighRised ? 180.0 : 200.0);
								}
							}
						}
					}
				}
				else
				{
					dSlenR = 150.0;	// Beam.
				}

				rData.CLMT = dSlenR * dCF;
				rData.TLMT = dSlenR_T * dCF;	// Assumed by ZINU.
			}
			else if (strStlCode == STLCODE_CSA_S6_14 || strStlCode == STLCODE_CSA_S6_19)
			{
				int iMembType = Get_DgnGenMbtp(Showkey);
				if (iMembType == D_MBTP_BRACE)  // Braced Element
				{
					rData.CLMT = 160.0;
					rData.TLMT = 200.0;
				}
				else   // UnBraced
				{
					rData.CLMT = 120.0;
					rData.TLMT = 200.0;
				}

			}
			else if (strStlCode == STLCODE_KSCE_ASD96) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_KSCE_ASD05) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_JTJ025_86) { rData.CLMT = 100.0;  rData.TLMT = 130.0; }
			else if (strStlCode == STLCODE_AASHTO_LRFD24) { rData.CLMT = 120.0;  rData.TLMT = 200.0; } /// [AASHTO-2024, 6.8.4, 6.9.3] for primary memberrs
			else if (strStlCode == STLCODE_AASHTO_LRFD20) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_LRFD17) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_LRFD16) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_LRFD12) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_LRFD02) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_LFD96) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_AASHTO_ASD96) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_TWN_BRG_LSD90) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_TWN_BRG_ASD90) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_IS800_2007) { rData.CLMT = 180.0;  rData.TLMT = 400.0; }
			else if (strStlCode == STLCODE_AIK_CFSD98) { rData.CLMT = 120.0;  rData.TLMT = 300.0; }  // MQC:3267-Seungjun-20100405 �ð������� �����? ����ġ ����. 200/300 -> 120/300
			else if(strStlCode == STLCODE_IRC_24_2010) { rData.CLMT = 120.0;  rData.TLMT = 300.0; }
			else if (strStlCode == STLCODE_JROAD_H14) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_JROAD_H24) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_JROAD_H29) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else if (strStlCode == STLCODE_JROAD_R07) { rData.CLMT = 120.0;  rData.TLMT = 200.0; }
			else { rData.CLMT = 200.0;  rData.TLMT = 300.0; }
		}
		else
		{
			T_DSRC_D DsrcD;
			DsrcD.Initialize();
			Get_DgnSrcDsrc(DsrcD);

			if (MatdD.Type == _T("SRC") && DsrcD.DesignCode == SRCCODE_AIK_SRC2K)
			{
				l_SetNotChkLtsr(rData);
			}
			else
			{
				rData.CLMT = 200.0;
				rData.TLMT = 300.0;
			}
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlUtcf(int key, T_UTCF_D& rData)
{
	// Key is Elem.
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.Ucf		  	: Not auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	if (m_pDoc->m_pAttrCtrl->ExistUtcf(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetUtcf(Showkey, rData))	return FALSE;
	}
	else	rData.Ucf = 1.0;

	return TRUE;
}

void CDgnDataCtrl::Create_DgnGenKfac(BOOL bCount)
{
	double dPi = 4.0*atan(1.0);
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	BOOL bCountKfac = (pDgnDynData->m_arKfacData.GetCount() == 0);
	if (bCount) bCountKfac = TRUE;
	if (bCountKfac)
	{
		T_DCTL_D DctlD;
		DctlD.Initialize();
		Get_DgnGenDctl(DctlD);
		// For Column.
		CKFactor k;
		k.Calc_KFactor(TRUE, DctlD.bBraceFrame_X, DctlD.bBraceFrame_Y, pDgnDynData->m_arKfacData);
		//////////////////////////////////////////////////////////////////////////
		POSITION pos = k.m_mapK1y.GetStartPosition();
		T_ELEM_K myElemK;
		double   mydK;
		while (pos)
		{
			k.m_mapK1y.GetNextAssoc(pos, myElemK, mydK);
			m_mapK1y.SetAt(myElemK, mydK);
		}
		//
		pos = k.m_mapK2y.GetStartPosition();
		while (pos)
		{
			k.m_mapK2y.GetNextAssoc(pos, myElemK, mydK);
			m_mapK2y.SetAt(myElemK, mydK);
		}
		//
		pos = k.m_mapK1z.GetStartPosition();
		while (pos)
		{
			k.m_mapK1z.GetNextAssoc(pos, myElemK, mydK);
			m_mapK1z.SetAt(myElemK, mydK);
		}
		//
		pos = k.m_mapK2z.GetStartPosition();
		while (pos)
		{
			k.m_mapK2z.GetNextAssoc(pos, myElemK, mydK);
			m_mapK2z.SetAt(myElemK, mydK);
		}
		//////////////////////////////////////////////////////////////////////////
		// For Wall.
		// Add by ZINU.('04.01.17). Apply Auto-calculation Value at Wall.
		T_WALL_KEY WallK;
		CArray<T_ELEM_K, T_ELEM_K> arElemList;
		int iCountWall = m_pDoc->m_pPostCtrl->GetCountInternalWallId();
		for (int i = 0; i < iCountWall; i++)
		{
			m_pDoc->m_pPostCtrl->GetWallKey(i + 1, WallK);
			CArray<T_ELEM_K, T_ELEM_K> arElemK;
			m_pDoc->m_pPostCtrl->GetWallElemList(WallK, arElemK);
			arElemList.Append(arElemK);
		}
		for (int i = 0; i < arElemList.GetSize(); i++)
		{
			T_ELEM_K ElemK = arElemList.GetAt(i);
			T_ELEM_D ElemD; ElemD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	continue;
			// 1st node.
			T_NODE_D NodeD1; NodeD1.Initialize();
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1);
			// 2nd node.
			T_NODE_D NodeD2; NodeD2.Initialize();
			m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2);
			// Check X-Dir or Y-Dir.
			double dX = fabs(NodeD1.x - NodeD2.x);
			double dY = fabs(NodeD1.y - NodeD2.y);
			double dAng = (dX < cDgn_Zero ? dPi / 2.0 : atan(dY / dX));	// Angle from Global X.
			stKFactor Factor;
			if (dAng < dPi / 4.0)	// X-Dir.
			{
				Factor.KFact_StrongAxis = (DctlD.bBraceFrame_X ? 0.8 : 2.0);
				Factor.KFact_WeakAxis = (DctlD.bBraceFrame_Y ? 0.8 : 2.0);
			}
			else	// Y-Dir.
			{
				Factor.KFact_StrongAxis = (DctlD.bBraceFrame_Y ? 0.8 : 2.0);
				Factor.KFact_WeakAxis = (DctlD.bBraceFrame_X ? 0.8 : 2.0);
			}
			// Set K-Factor at Wall.
			pDgnDynData->m_arKfacData.SetAt(ElemK, Factor);
		}
	}

	/*
#if defined(_CIVIL)
	//Add by GAY.('06.02.24). Calculation Q_Factor
	// Change by Jaeoh('06.03.16).
	if(m_pMembCtrl->GetLcomType() == D_LCOMTYPE_CONCRETE)
	{
		if(pDgnDynData->m_arQfacData.GetCount()==0 && CDBLib::GetConCodeName()==_T("KSCE-USD05"))
		{
		  // For Column.
		  CQFactor qFactor;
			qFactor.Calc_QFactor(pDgnDynData->m_arQfacData);
		}
	}
#endif
	*/
}

// Create Q-Factor. ('06.03.20). Add by GAY.
void CDgnDataCtrl::Create_DgnGenQfac(int iDgnNo, CString strCodeIn)
{
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	// Add by GAY.('06.02.24). Calculation Q_Factor  
	// Change by Jaeoh('06.03.16).
	// Change by GAY.('06.03.22). Delete the condition m_arQfacData.GetCount() == 0.
	//if(pDgnDynData->m_arQfacData.GetCount()==0 && CDBLib::GetConCodeName()==_T("KSCE-USD05"))
	CString strConCode = _T("");
	if (strCodeIn == _T(""))
	{
		strConCode = CDBLib::GetConCodeName();
	}
	else
	{
		strConCode = strCodeIn;
	}

	if (strConCode == CONCODE_KSCE_USD05 || strConCode == CONCODE_KSCE_RAIL_USD04 || strConCode == CONCODE_KCI_USD07 || strConCode == CONCODE_KCI_USD12 ||
		strConCode == CONCODE_KSCE_USD10 || strConCode == CONCODE_KSCE_RAIL_USD11)
	{
		// For Column.
		CQFactor qFactor;
		qFactor.SetDgnCode(strConCode);
		qFactor.Calc_QFactor(pDgnDynData->m_arQfacData, iDgnNo);
	}
}

void CDgnDataCtrl::Create_DgnGenQfac_LRFD(int iDgnNo, CString strCodeIn)
{
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();

	CQFactor qFactor;
	qFactor.SetDgnCode(strCodeIn);
	qFactor.Calc_QFactor_LRFD(pDgnDynData->m_arQfacData, iDgnNo);
}

BOOL CDgnDataCtrl::Get_DgnGenKfac(int key, T_KFAC_D& rData, T_KFAC_D& aData)
{
	// Key is Elem.
	rData.Initialize();
	aData.Initialize();
	////////////////////////////////////////////////
	//	rData.BKy	  : Can auto-calculation.
	//	rData.BKz	  : Can auto-calculation.
	//	aData.BKy	  : Value of auto-calculation.
	//	aData.BKz	  : Value of auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K ShowKey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, ShowKey))	return FALSE;

	T_DCTL_D DctlD; DctlD.Initialize();
	Get_DgnGenDctl(DctlD);

	BOOL bGetAutoOK = FALSE;
	stKFactor Factor;
	// Column, Wall.
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	bGetAutoOK = pDgnDynData->m_arKfacData.Lookup(ShowKey, Factor);
	// Change by ZINU.('04.01.12). 1.0 -> 0.0.
	if (bGetAutoOK)
	{
		aData.BKy = Factor.KFact_StrongAxis;
		aData.BKz = Factor.KFact_WeakAxis;
		aData.BKt = 1.0;
	}
	else
	{
		aData.BKy = 0.0;
		aData.BKz = 0.0;
		aData.BKt = 0.0;
	}	

	if (m_pDoc->m_pAttrCtrl->ExistKfac(ShowKey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetKfac(ShowKey, rData))	return FALSE;
	}
	else
	{
		// Change by ZINU.('03.08.27). For Auto-Effective Length by SlabType (Cast-in-place, Precast).
		BOOL bGetOK = Get_ConColmEffLengFactorForGB(ShowKey, rData.BKy, rData.BKz);
		if (!bGetOK)
		{
			if (DctlD.bAutoKFactor && bGetAutoOK)
			{
				rData.BKy = (aData.BKy == 0.0 ? 1.0 : aData.BKy);
				rData.BKz = (aData.BKz == 0.0 ? 1.0 : aData.BKz);
			}
			else
			{
				rData.BKy = 1.0;
				rData.BKz = 1.0;
			}
		}
		rData.BKt = 1.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenGreq(int key, int& iDgnClass)
{
	// key is Elem.
	T_ELEM_K ShowKey;
	T_HCBM_K HcbmKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey))
	{
		if (!m_pMembCtrl->GetShowElemByIncludeHcbm(key, ShowKey))	return FALSE;
	}
	else
	{
		if (!m_pMembCtrl->GetShowElemByIncludeElem(key, ShowKey))	return FALSE;
	}
	// Get Matl.
	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!Get_DgnGenMatd(key, MatdD))	return FALSE;
	// Get Stl.
	T_DSTL_D DstlD;
	DstlD.Initialize();
	Get_DgnStlDstl(DstlD);
	// Get Con.
	T_DCON_D DconD;
	DconD.Initialize();
	Get_DgnConDcon(DconD);
	// Get Src.
	T_DSRC_D DsrcD;
	DsrcD.Initialize();
	Get_DgnSrcDsrc(DsrcD);
	//add by maxiao(2015-9-25)GB50017-15
	BOOL bStlEqCH = (MatdD.Type == _T("S")   && DstlD.bSpecialSeismic && (DstlD.DesignCode == STLCODE_GBJ17_88 || DstlD.DesignCode == STLCODE_GB50017_03 || DstlD.DesignCode == STLCODE_GB50017_15 || DstlD.DesignCode == STLCODE_GB50017_17 || DstlD.DesignCode == STLCODE_JGJ209_2010) ? TRUE : FALSE);
	BOOL bConEqCH = (MatdD.Type == _T("C")   && DconD.bSpecialSeismic && (DconD.DesignCode == CONCODE_GB50010_02 || DconD.DesignCode == CONCODE_GB50010_10) ? TRUE : FALSE);
	BOOL bSrcEqCH = (MatdD.Type == _T("SRC") && DsrcD.bSpecialSeismic && (DsrcD.DesignCode == SRCCODE_JGJ138_01 || DsrcD.DesignCode == SRCCODE_GB50068_2019) ? TRUE : FALSE);
	if (bStlEqCH || bConEqCH || bSrcEqCH)
	{
		T_GREQ_D GreqD;
		GreqD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetGreq(ShowKey, GreqD))
		{
			if (bStlEqCH)
			{
				iDgnClass = GreqD.iEqGradeStl + 1;
				if (iDgnClass == 5)
					iDgnClass = 0;
			}
			if (bConEqCH)	iDgnClass = GreqD.iEqGradeCon + 1;
			if (bSrcEqCH)	iDgnClass = GreqD.iEqGradeSrc + 1;
		}
		else
		{
			// Change by ZINU.('03.11.20). Set by Global Data.
			if (bStlEqCH)	iDgnClass = DstlD.nClass + 1;
			if (bConEqCH)	iDgnClass = DconD.nClass + 1;
			if (bSrcEqCH)	iDgnClass = DsrcD.nClass + 1;
		}
	}
	else	return FALSE;	// Not change iDgnClass.

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenFmag(int key, T_FMAG_D& rData)
{
	// Key is Elem.
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.B1y	: Can auto-calculation.
	//	rData.B1z	: Can auto-calculation.
	//	rData.B2y	: Not auto-calculation.
	//	rData.B2z	: Not auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistFmag(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetFmag(Showkey, rData)) { return FALSE; }
	}
	else
	{
		rData.B1y = 0.0;
		rData.B1z = 0.0;
		rData.B2y = 1.0;
		rData.B2z = 1.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenSueq(int key, T_SUEQ_D& rData)
{
	// Key is Elem.
	rData.Initialize();

	T_ELEM_K Showkey;
	T_HCBM_K HcbmKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey))
	{
		if (!m_pMembCtrl->GetShowElemByIncludeHcbm(key, Showkey)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }
	}

	if (m_pDoc->m_pAttrCtrl->ExistSueq(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetSueq(Showkey, rData)) { return FALSE; }
	}
	else
	{
		rData.dEqLcaseN = 0.0;
		rData.dEqLcaseM = 0.0;
		rData.dEqLcaseV = 0.0;
		rData.dEqLcomN = 0.0;
		rData.dEqLcomM = 0.0;
		rData.dEqLcomV = 0.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenRedu(int key, T_REDU_D& rData)
{
	// Key is Elem.
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.RFactor	: Can auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistRedu(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetRedu(Showkey, rData)) { return FALSE; }
	}
	else
	{
		double dFactor = 0.0;
		CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
		if (pDgnDynData->m_arReduData.Lookup(Showkey, dFactor))
		{
			int iReduKind = 0;
			pDgnDynData->m_arReduKind.Lookup(Showkey, iReduKind);
			rData.bReduAxl = ((iReduKind / 100) % 10 == 1 ? TRUE : FALSE);
			rData.bReduMom = ((iReduKind / 10) % 10 == 1 ? TRUE : FALSE);
			rData.bReduShr = ((iReduKind / 1) % 10 == 1 ? TRUE : FALSE);
			// If auto-calculation value is existed.
			rData.Factor = (dFactor > 0.0 ? dFactor : 1.0);
		}
		else	rData.Factor = 1.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlDalw(int key, T_DALW_D& rData)
{
	// Key is Elem.
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.Fa		  :	Not auto-calculation.
	//	rData.F_Allow	:	Not auto-calculation.
	//	rData.Fby 		:	Not auto-calculation.
	//	rData.Fbz	  	:	Not auto-calculation.
	//	rData.Ft		  :	Not auto-calculation.
	//	rData.Fv		  :	Not auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistDalw(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetDalw(Showkey, rData)) { return FALSE; }
	}
	else
	{
		rData.Fa = 0.0;
		rData.Fby = 0.0;
		rData.Fbz = 0.0;
		rData.Ft = 0.0;
		rData.Fv = 0.0;
		rData.F_Allow = 1.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnCfsCsms(int key, T_CSMS_D& rData)
{
	rData.Initialize();

	BOOL biGen = FALSE;
#if defined(_IGEN)
	biGen = TRUE;
#endif

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistCsms(Showkey))
	{
		m_pDoc->m_pAttrCtrl->GetCsms(Showkey, rData);
		if (rData.m_bAutoPhi)
		{
			rData.m_dPhiby = 0.0;
			rData.m_dPhibz = 0.0;
		}
	}
	else
	{
		bool enableAutoPhi = false;

		/* AIJ ASD 2002 + Bending Coefficient */
		// DSTL bending coefficient
		const CString& strStlCodeName = CDBLib::GetStlCodeName();

		T_DSTL_D dstlD;
		if (!m_pDoc->m_pAttrCtrl->GetDstl(dstlD)) dstlD.Initialize();
		if (dstlD.bBendingCoeff && strStlCodeName == STLCODE_AIJ_ASD02)
			enableAutoPhi = true;


		/* GB Cold-Formed Steel Codes */
		const CString cfsCode = CDBLib::GetCfsCodeName();
		if (cfsCode == _T("GB50018-02") || cfsCode == _T("GB/T50018-25"))
		{
			enableAutoPhi = true;
		}

		const double phi = enableAutoPhi ? 0.0 : 1.0;
		rData.m_bAutoPhi = enableAutoPhi;
		rData.m_dPhiby = phi;
		rData.m_dPhibz = phi;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnCfsCsos(int key, T_CSOS_D& rData)
{
	rData.Initialize();

	BOOL biGen = FALSE;
#if defined(_IGEN)
	biGen = TRUE;
#endif

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistCsos(Showkey))
	{
		m_pDoc->m_pAttrCtrl->GetCsos(Showkey, rData);
		rData.m_dAlpha = 0.0;
		rData.m_dBeta = 0.0;
	}
	else
	{
		bool setZero = false;

		// DSTL bending coefficient
		const CString& strStlCodeName = CDBLib::GetStlCodeName();

		T_DSTL_D dstlD;
		if (!m_pDoc->m_pAttrCtrl->GetDstl(dstlD)) dstlD.Initialize();
		if (dstlD.bBendingCoeff && strStlCodeName == STLCODE_AIJ_ASD02)
			setZero = true;


		// GB codes
		const CString& cfsCode = CDBLib::GetCfsCodeName();
		if (cfsCode == CFSCODE_GB50018_02 || cfsCode == CFSCODE_GB50018_25)
		{
			setZero = true;
		}

		const double dv = setZero ? 0.0 : 1.0;
		rData.m_dAlpha = dv;
		rData.m_dBeta  = dv;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnCfsPlin(int key, T_PLIN_D& rData)
{
	rData.Initialize();

	BOOL biGen = FALSE;
#if defined(_IGEN)
	biGen = TRUE;
#endif

	T_ELEM_K Showkey;
	if(!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if(m_pDoc->m_pAttrCtrl->ExistPlin(Showkey))
	{
		m_pDoc->m_pAttrCtrl->GetPlin(Showkey, rData);
		rData.bPurlin = TRUE;
	}
	else
	{
		bool enablePurlin = false;

		// DSTL bending coefficient
		const CString& strStlCodeName = CDBLib::GetStlCodeName();
		
		T_DSTL_D dstlD;
		if (!m_pDoc->m_pAttrCtrl->GetDstl(dstlD)) dstlD.Initialize();
		if (dstlD.bBendingCoeff && strStlCodeName == STLCODE_AIJ_ASD02)
			enablePurlin = true;
		

		// GB Cold-Formed Steel codes
		const CString& cfsCode = CDBLib::GetCfsCodeName();
		if (cfsCode == CFSCODE_GB50018_02 ||
			cfsCode == CFSCODE_GB50018_25)
		{
			enablePurlin = true;
		}

		rData.bPurlin = enablePurlin ? TRUE : FALSE;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlCbft(int key, T_CBFT_D& rData)
{
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.bAuto	:	Option for auto-calculation.
	//	rData.Cb	  :	Can auto-calculation.
	////////////////////////////////////////////////

	BOOL biGen = FALSE;
#if defined(_IGEN)
	biGen = TRUE;
#endif

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	if (m_pDoc->m_pAttrCtrl->ExistCbft(Showkey))
	{
		m_pDoc->m_pAttrCtrl->GetCbft(Showkey, rData);
		if (rData.bAuto)	rData.Cb = 0.0;
	}
	else
	{
		const CString& strStlCodeName = CDBLib::GetStlCodeName();
		//add by maxiao(2015-9-25)GB50017-15
		// Change by Shin('11.11.01) : iGen�� ���? AIJ-ASD02������ C�� �ڵ� �����? Default�� ó��(MidasIT JP, ����BJ ��û����)
		if (strStlCodeName == STLCODE_GBJ17_88 || strStlCodeName == STLCODE_GB50017_03 || strStlCodeName == STLCODE_GB50017_15 || strStlCodeName == STLCODE_GB50017_17 || strStlCodeName == STLCODE_JGJ209_2010 || strStlCodeName == STLCODE_AIJ_ASD02)
		{
			rData.bAuto = TRUE;
			rData.Cb = 0.0;
		}
		else
		{
			rData.bAuto = FALSE;
			rData.Cb = 1.0;
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenCmft(int key, T_CMFT_D& rData)
{
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.Cmy	  :	Can auto-calculation.
	//	rData.Cmz	  :	Can auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistCmft(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetCmft(Showkey, rData)) { return FALSE; }
		if (rData.bAuto)
		{
			rData.Cmy = 0.0;
			rData.Cmz = 0.0;
		}
	}
	else
	{
		// Change by ZINU.('04.10.20). Set Beam=1.0, Column=0.85.
		// Get Member Type.
		int iMembType = Get_DgnGenMbtp(Showkey);
		BOOL b10 = (iMembType == D_MBTP_BEAM || iMembType == D_MBTP_BRACE ? TRUE : FALSE);
		rData.bAuto = FALSE;
		rData.Cmy = (b10 ? 1.0 : 0.85);
		rData.Cmz = (b10 ? 1.0 : 0.85);
	}
	return TRUE;
}

BOOL CDgnDataCtrl::IsAutoDgnGenCmft(int key)
{
	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	T_CMFT_D CmftD;
	if (m_pDoc->m_pAttrCtrl->GetCmft(Showkey, CmftD))
	{		
		return CmftD.bAuto;		
	}
	else
	{
		return TRUE;
	}	
}

BOOL CDgnDataCtrl::Get_DgnGenMbtp(int key, T_MBTP_D& rData)
{
	rData.Initialize();
	////////////////////////////////////////////////////////////////////////////////////////////
	// nMbType (Member Type).
	//#define D_MBTP_COLUMN  1
	//#define D_MBTP_BEAM    2
	//#define D_MBTP_BRACE   3
	//1=column(����-1/100 �������?), 2=beam(����-1/100 �������?), 3=brace 0=Frame ��Ұ�? �ƴҶ�

	// Change by ZINU.('02.12.30). For SubType at China Code.
	// nSubType (Sub-Type) - Only China Code (GBJ17-88, GB50017-03, GB50010-02, JGJ138-01).
	//////////////////////// If nMbType==D_MBTP_BEAM
	// 0 = Frame Beam
	// 1 = Lintel Beam
	//////////////////////// If nMbType==D_MBTP_COLUMN
	// 0 = Frame Column
	// 1 = Lower Frame Column
	// 2 = Corner Column
	// 3 = Lower Corner Column
	// 4 = Column Supporting Frame
	//////////////////////// If nMbType==D_MBTP_BRACE
	// Not used.
	//////////////////////// If nMbType==D_MBTP_WALL
	// 0 = General Shear Wall (Strengthened part)
	// 1 = General Shear Wall (Ordinary part)
	// 2 = Shear Wall at Complicated Tall Building (Strengthened part)
	// 3 = Shear Wall at Complicated Tall Building (Ordinary part)
	////////////////////////////////////////////////////////////////////////////////////////////

	T_ELEM_K Showkey;
	T_HCBM_K HcbmKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey))
	{
		if (!m_pMembCtrl->GetShowElemByIncludeHcbm(key, Showkey)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }
	}

	if (m_pDoc->m_pAttrCtrl->ExistMbtp(Showkey))	// �����? ���ǹ��?
	{
		m_pDoc->m_pAttrCtrl->GetMbtp(Showkey, rData);
	}
	else	// �Ϲ����� ���?
	{
		if (IsJUD()) rData.nMbType = m_pDoc->m_pAttrCtrl->GetMemberType_JUD(Showkey);
		else        rData.nMbType = m_pDoc->m_pAttrCtrl->GetMemberType(Showkey);// �Ϲ����� ���?
		rData.nSubType = 0;
	}
	return TRUE;
}

int CDgnDataCtrl::Get_DgnGenMbtp(int key)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// member type
	//#define D_MBTP_COLUMN  1
	//#define D_MBTP_BEAM    2
	//#define D_MBTP_BRACE   3
	//1=column(����-1/100 �������?), 2=beam(����-1/100 �������?), 3=brace 0=Frame ��Ұ�? �ƴҶ�
	////////////////////////////////////////////////////////////////////////////////////////////

	int nType = 0;
	if (m_pDoc->m_pAttrCtrl->ExistMbtp(key))
	{
		T_MBTP_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetMbtp(key, rData); // �����? ���ǹ��?
		nType = rData.nMbType;
	}
	else
	{
		if (IsJUD()) nType = m_pDoc->m_pAttrCtrl->GetMemberType_JUD(key);
		else        nType = m_pDoc->m_pAttrCtrl->GetMemberType(key);	// �Ϲ����� ���?
	}

	return nType;
}

int CDgnDataCtrl::Get_DgnGenSpva(int key)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// member type
	//#define D_MBTP_COLUMN  1
	//#define D_MBTP_BEAM    2
	//#define D_MBTP_BRACE   3
	//1=column(����-1/100 �������), 2=beam(����-1/100 �������), 3=brace 0=Frame ��Ұ� �ƴҶ�
	////////////////////////////////////////////////////////////////////////////////////////////

	int nType = 0;
	if (m_pDoc->m_pAttrCtrl->ExistSpvm(key))
	{
		T_SPVM_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetSpvm(key, rData); // ����� ���ǹ��
		nType = rData.nMbType;
}
	else
	{
		nType = -1;
	}

	return nType;
}

int CDgnDataCtrl::Get_DgnGenMbtpTower(int key)
{
#if defined(_TOWER)
	////////////////////////////////////////////////////////////////////////////////////////////
	// Slenderness checking type
	// 1=yy-zz-vv, 2=yy-zz, 3=vv
	////////////////////////////////////////////////////////////////////////////////////////////
	int nType = 0;
	if (m_pDoc->m_pAttrCtrl->ExistMbtp(key))
	{
		T_MBTP_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetMbtp(key, rData);
		nType = rData.nMbType;	// by User.
	}
	else	nType = 1;					// by Default.
	return nType;
#else
	return 0;
#endif
}

// Get Stablity Index. Add by GAY.('06.02.24)
// [param] elemK : ��ҹ��?
// [param] lcomK : �������� ��ȣ
// [param] qFactor : �־��� ���ǿ� �ش��ϴ� Stability Index ���� struct.
BOOL CDgnDataCtrl::Get_DgnGenQfac(int elemK, int lcomK, Q_FACTOR& qFactor)
{
	T_QFAC_K key;
	key.elemK = elemK;
	key.lcomK = lcomK;

	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	if (!pDgnDynData->m_arQfacData.Lookup(key, qFactor)) { return FALSE; }

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlCvft(int key, T_CVFT_D& rData)
{
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.bAuto	:	Option for auto-calculation.
	//	rData.Cv	  :	Can auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	if (m_pDoc->m_pAttrCtrl->ExistCvft(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetCvft(Showkey, rData)) { return FALSE; }
		if (rData.bAuto)	rData.Cv = 0.0;
	}
	else
	{
		rData.bAuto = TRUE;
		rData.Cv = 0.0;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen)
{
	double dNetLy = 0.0, dNetLz = 0.0;
	return Get_DgnGenLeng(key, iPosiNo, rData, dLen, dNetLy, dNetLz);
}

BOOL CDgnDataCtrl::Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen, double& dNetLy, double& dNetLz)
{
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.BLu 			  :	Not auto-calculation.
	//	rData.BLy	  		  :	Not auto-calculation.
	//	rData.BLz		    	:	Not auto-calculation.
	//	rData.bNotUseBLu	:	Selection by User.
	//  rData.bAutoCalcLe : Selection by User.
	////////////////////////////////////////////////

	BOOL bReverseDir = FALSE;
	T_HCBM_K HcbmKey;
	BOOL bHaunched = (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey) ? TRUE : FALSE);

	T_ELEM_K ShowKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	if (bHaunched)
	{
		if (!m_pMembCtrl->GetElemListByIncludeHcbm(key, ShowKey, aElemList, TRUE)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetElemListByIncludeElem(key, bReverseDir, ShowKey, aElemList, TRUE)) { return FALSE; }
	}

	// Get by Jung BJ ('07.03.15).
	//----------------------------------------------------
	//  �����? Offset Information save                  
	//  BLNG       : BEAM LENGTH                        
	//  IOFSTYP    : INDEX FOR OFFSET CONDITION           
	//             0   : GLOBAL OFFSET,SECTION OFFSET       
	//             1   : BUILDING CONTROL & LOCAL OFFSET    
	//  IF(IOFSTYP.EQ.1) THEN       ! LOCAL OFFSET      
	//     OFSDT(1,1) : I-END X OFFSET                  
	//     OFSDT(2,1) : J-END X OFFSET                  
	//     OFSDT(1,2) : I-END Y OFFSET                  
	//     OFSDT(2,2) : J-END Y OFFSET                  
	//     OFSDT(1,3) : I-END Z OFFSET                  
	//     OFSDT(2,3) : J-END Z OFFSET                  
	//     RGDFAC     : FACTOR                          
	//  ELSEIF(IOFSTYP.EQ.0) THEN   ! GLOBAL OFFSET     
	//     OFSDT(1,1) : I-END X OFFSET                  
	//     OFSDT(2,1) : I-END Y OFFSET                  
	//     OFSDT(1,2) : I-END Z OFFSET                  
	//     OFSDT(2,2) : J-END X OFFSET                  
	//     OFSDT(1,3) : J-END Y OFFSET                  
	//     OFSDT(2,3) : J-END Z OFFSET                  
	//     RGDFAC     : 1.0                             
	//  ENDIF                                           
	//----------------------------------------------------

	T_ELEM_K CurrKey = 0;
	double dMemLy = 0.0, dMemLz = 0.0;
	if (aElemList.GetSize() == 1)	// Only 1.
	{
		CurrKey = aElemList[0]; //key;
		dLen = m_pDoc->calcLAVElem(CurrKey);
		dMemLy = dLen;
		dMemLz = dLen;

		// Add by ZINU.('07.03.27). NO:2713, Net Length by Member.
		// Change by ZINU.('07.06.26). dNetLy=dMemLy, dNetLz=dMemLz -> dNetLy=dLen, dNetLz=dLen.
		dNetLy = dLen;
		dNetLz = dLen;

		T_CFMO_D CfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(CurrKey, CfmoD))
		{
			if (CfmoD.nOffsetType == 1)	// Local Offset.
			{
				dNetLy -= (CfmoD.dOffset[2] + CfmoD.dOffset[3]);
				dNetLz -= (CfmoD.dOffset[4] + CfmoD.dOffset[5]);
			}
		}
	}
	else	// Over 1.
	{
		if (bHaunched) dLen = m_pMembCtrl->GetHcbmLengByIncludeElem(ShowKey);	// Total Length.
		else          dLen = m_pMembCtrl->GetMembLengByIncludeElem(ShowKey);	// Total Length.
		int iElemCount = 0;
		double dElemLen = 0.0, dSumLen = 0.0;
		double dPartLen = 0.0, dPartLen1 = 0.0, dPartLen2 = 0.0;
		if (!m_pMembCtrl->GetMembPosiData(iPosiNo, aElemList, dLen,
			CurrKey, iElemCount, dElemLen, dSumLen, dPartLen, dPartLen1, dPartLen2))	ASSERT(0);
		// Search Ly, Lz.
		CArray<double, double> aLenYList, aLenZList;
		if (!m_pMembCtrl->GetUnbracedLength(CurrKey, aLenYList, aLenZList))	ASSERT(0);

		int iElemNum = aElemList.GetSize();
		int iCount = 0;
		for (int i = 0; i < iElemNum; i++)
		{
			if (CurrKey == aElemList.GetAt(i))
			{
				iCount = i;//(bReverseDir ? (iElemNum-1)-i : i);
				break;
			}
		}
		dMemLy = aLenZList.GetAt(iCount);	// zList for Ly.
		dMemLz = aLenYList.GetAt(iCount);	// yList for Lz.

		// Add by ZINU.('07.03.27). NO:2713, Net Length by Member.
		// Change by ZINU.('07.06.26). dNetLy=dMemLy, dNetLz=dMemLz -> dNetLy=dLen, dNetLz=dLen.
		dNetLy = dLen;
		dNetLz = dLen;
		// Start Elem.

		T_ELEM_K StaElemK = (bReverseDir ? aElemList.GetAt(aElemList.GetSize() - 1) : aElemList.GetAt(0));
		T_CFMO_D StaCfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(StaElemK, StaCfmoD))
		{
			if (StaCfmoD.nOffsetType == 1)	// Local Offset.
			{
				dNetLy -= (bReverseDir ? StaCfmoD.dOffset[3] : StaCfmoD.dOffset[2]);	// Member-I.
				dNetLz -= (bReverseDir ? StaCfmoD.dOffset[5] : StaCfmoD.dOffset[4]);	// Member-I.
			}
		}
		// End Elem.
		T_ELEM_K EndElemK = (bReverseDir ? aElemList.GetAt(0) : aElemList.GetAt(aElemList.GetSize() - 1));
		T_CFMO_D EndCfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(EndElemK, EndCfmoD))
		{
			if (EndCfmoD.nOffsetType == 1)	// Local Offset.
			{
				dNetLy -= (bReverseDir ? EndCfmoD.dOffset[2] : EndCfmoD.dOffset[3]);	// Member-J.
				dNetLz -= (bReverseDir ? EndCfmoD.dOffset[4] : EndCfmoD.dOffset[5]);	// Member-J.
			}
		}
	}

	if (m_pDoc->m_pAttrCtrl->ExistLeng(CurrKey))
	{
		m_pDoc->m_pAttrCtrl->GetLeng(CurrKey, rData);
		if (rData.BLy == 0.0)	rData.BLy = dMemLy;
		if (rData.BLz == 0.0)	rData.BLz = dMemLz;

		if (rData.bNotUseBLu)	rData.BLu = 0.0;
		else					rData.BLu = (rData.BLu == 0.0 ? rData.BLz : rData.BLu);

		if (rData.BLt == 0.0)	rData.BLt = max(rData.BLy, rData.BLz);
	}
	else
	{
		// Set Default Length.
		rData.BLy = dMemLy;
		rData.BLz = dMemLz;

		T_DSTL_D cData;
		cData.Initialize();
		Get_DgnStlDstl(cData);
		rData.bNotUseBLu = cData.bAllFrameLaterallyBraced;

		if (rData.bNotUseBLu)
		{
			// Get Element Type.
			int nType = Get_DgnGenMbtp(CurrKey);
			if (nType == D_MBTP_BEAM)	rData.BLu = 0.0;
			else						rData.BLu = rData.BLz;
		}
		else
		{
			rData.BLu = (rData.BLu == 0.0 ? rData.BLz : rData.BLu);
		}

		rData.BLt = max(dMemLy, dMemLz);
	}

	if (IsJUD())
	{
		int nType = Get_DgnGenMbtp(CurrKey);
		if (nType == D_MBTP_BEAM)// Steel Beam�� ���? �����?
		{
			double dLbMax = 0.0;
			CArray<double, double> arSpace;
			int nIMJ = iPosiNo == 0 ? 0 : (iPosiNo == 4 ? 2 : 1);
			if (m_pMembCtrl->Get_Blcf_SpaceList(key, arSpace, dLbMax, nIMJ))
			{
				rData.BLu = dLbMax;
			}
		}
	}

	return TRUE;
}

// Seungjun-20110408 �Ϻ�2������ ������ �������? ����
// nLengType ; 0:���ػ� ���� ��������, 1:node to node(�𵨸� ����), 2:face to face(������)
// nOffSetOutType : 0:�ؼ��� ���������? ���ܿ��� ������ �Ÿ�, 1:���� ���ܿ��������� �Ÿ�
BOOL CDgnDataCtrl::Get_DgnGenLeng(int key, int iPosiNo, T_LENG_D& rData, double& dLen, double& dNetLy, double& dNetLz, double* dOffSetLy, double* dOffSetLz, int nLengTypei/*0*/, int nLengTypej/*0*/, int nOffSetOutType/*0*/)
{
	rData.Initialize();
	dOffSetLy[0] = dOffSetLz[0] = dOffSetLy[1] = dOffSetLz[1] = 0.0;
	////////////////////////////////////////////////
	//	rData.BLu 			  :	Not auto-calculation.
	//	rData.BLy	  		  :	Not auto-calculation.
	//	rData.BLz		    	:	Not auto-calculation.
	//	rData.bNotUseBLu	:	Selection by User.
	//  rData.bAutoCalcLe : Selection by User.
	////////////////////////////////////////////////

	BOOL bReverseDir = FALSE;
	T_HCBM_K HcbmKey;
	BOOL bHaunched = (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey) ? TRUE : FALSE);

	T_ELEM_K ShowKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;

	if (bHaunched)
	{
		if (!m_pMembCtrl->GetElemListByIncludeHcbm(key, ShowKey, aElemList, TRUE)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetElemListByIncludeElem(key, bReverseDir, ShowKey, aElemList, TRUE)) { return FALSE; }
	}

	// Get by Jung BJ ('07.03.15).
	//----------------------------------------------------
	//  �����? Offset Information save                  
	//  BLNG       : BEAM LENGTH                        
	//  IOFSTYP    : INDEX FOR OFFSET CONDITION           
	//             0   : GLOBAL OFFSET,SECTION OFFSET       
	//             1   : BUILDING CONTROL & LOCAL OFFSET    
	//  IF(IOFSTYP.EQ.1) THEN       ! LOCAL OFFSET      
	//     OFSDT(1,1) : I-END X OFFSET                  
	//     OFSDT(2,1) : J-END X OFFSET                  
	//     OFSDT(1,2) : I-END Y OFFSET                  
	//     OFSDT(2,2) : J-END Y OFFSET                  
	//     OFSDT(1,3) : I-END Z OFFSET                  
	//     OFSDT(2,3) : J-END Z OFFSET                  
	//     RGDFAC     : FACTOR                          
	//  ELSEIF(IOFSTYP.EQ.0) THEN   ! GLOBAL OFFSET     
	//     OFSDT(1,1) : I-END X OFFSET                  
	//     OFSDT(2,1) : I-END Y OFFSET                  
	//     OFSDT(1,2) : I-END Z OFFSET                  
	//     OFSDT(2,2) : J-END X OFFSET                  
	//     OFSDT(1,3) : J-END Y OFFSET                  
	//     OFSDT(2,3) : J-END Z OFFSET                  
	//     RGDFAC     : 1.0                             
	//  ENDIF                                           
	//----------------------------------------------------

	T_PZEF_D PzefD; // panel zone effect
	BOOL bExistPzef = FALSE;
	if (m_pDoc->m_pAttrCtrl->GetPzef(PzefD))
	{
		bExistPzef = (PzefD.bOffset && PzefD.nOffsType == 2) ? TRUE : FALSE;
	}

	T_ELEM_K CurrKey = 0;
	double dMemLy = 0.0, dMemLz = 0.0;
	if (aElemList.GetSize() == 1)	// Only 1.
	{
		CurrKey = key;
		dLen = m_pDoc->calcLAVElem(CurrKey);
		dMemLy = dLen;
		dMemLz = dLen;

		// Add by ZINU.('07.03.27). NO:2713, Net Length by Member.
		// Change by ZINU.('07.06.26). dNetLy=dMemLy, dNetLz=dMemLz -> dNetLy=dLen, dNetLz=dLen.
		dNetLy = dLen;
		dNetLz = dLen;

		BOOL bExistOffs = m_pDoc->m_pAttrCtrl->ExistOffsElem(CurrKey);
		T_CFMO_D CfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(CurrKey, CfmoD))
		{
			if (CfmoD.nOffsetType == 1)	// Local Offset.
			{
				dNetLy -= (CfmoD.dOffset[2] + CfmoD.dOffset[3]);
				dNetLz -= (CfmoD.dOffset[4] + CfmoD.dOffset[5]);

				dOffSetLy[0] = CfmoD.dOffset[2];
				dOffSetLz[0] = CfmoD.dOffset[4];
				dOffSetLy[1] = CfmoD.dOffset[3];
				dOffSetLz[1] = CfmoD.dOffset[5];
			}
		}
	}
	else	// Over 1.
	{
		if (bHaunched) dLen = m_pMembCtrl->GetHcbmLengByIncludeElem(ShowKey);	// Total Length.
		else          dLen = m_pMembCtrl->GetMembLengByIncludeElem(ShowKey);	// Total Length.
		int iElemCount = 0;
		double dElemLen = 0.0, dSumLen = 0.0;
		double dPartLen = 0.0, dPartLen1 = 0.0, dPartLen2 = 0.0;
		if (!m_pMembCtrl->GetMembPosiData(iPosiNo, aElemList, dLen,
			CurrKey, iElemCount, dElemLen, dSumLen, dPartLen, dPartLen1, dPartLen2))	ASSERT(0);
		// Search Ly, Lz.
		CArray<double, double> aLenYList, aLenZList;
		if (!m_pMembCtrl->GetUnbracedLength(CurrKey, aLenYList, aLenZList))	ASSERT(0);

		int iElemNum = aElemList.GetSize();
		int iCount = 0;
		for (int i = 0; i < iElemNum; i++)
		{
			if (CurrKey == aElemList.GetAt(i))
			{
				iCount = i;//(bReverseDir ? (iElemNum-1)-i : i);
				break;
			}
		}
		dMemLy = aLenZList.GetAt(iCount);	// zList for Ly.
		dMemLz = aLenYList.GetAt(iCount);	// yList for Lz.

		// Add by ZINU.('07.03.27). NO:2713, Net Length by Member.
		// Change by ZINU.('07.06.26). dNetLy=dMemLy, dNetLz=dMemLz -> dNetLy=dLen, dNetLz=dLen.
		dNetLy = dLen;
		dNetLz = dLen;
		// Start Elem.

		T_ELEM_K StaElemK = (bReverseDir ? aElemList.GetAt(aElemList.GetSize() - 1) : aElemList.GetAt(0));
		T_CFMO_D StaCfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(StaElemK, StaCfmoD))
		{
			if (StaCfmoD.nOffsetType == 1)	// Local Offset.
			{
				dOffSetLy[0] = (bReverseDir ? StaCfmoD.dOffset[3] : StaCfmoD.dOffset[2]);
				dOffSetLz[0] = (bReverseDir ? StaCfmoD.dOffset[5] : StaCfmoD.dOffset[4]);
				dNetLy -= dOffSetLy[0];	// Member-I.
				dNetLz -= dOffSetLz[0];	// Member-I.
			}
		}
		// End Elem.
		T_ELEM_K EndElemK = (bReverseDir ? aElemList.GetAt(0) : aElemList.GetAt(aElemList.GetSize() - 1));
		T_CFMO_D EndCfmoD;
		if (m_pDoc->m_pPostCtrl->GetCfmo(EndElemK, EndCfmoD))
		{
			if (EndCfmoD.nOffsetType == 1)	// Local Offset.
			{
				dOffSetLy[1] = (bReverseDir ? EndCfmoD.dOffset[2] : EndCfmoD.dOffset[3]);
				dOffSetLz[1] = (bReverseDir ? EndCfmoD.dOffset[4] : EndCfmoD.dOffset[5]);
				dNetLy -= dOffSetLy[1];	// Member-J.
				dNetLz -= dOffSetLz[1];	// Member-J.
			}
		}
	}

	double dOffset_y = 0.0, dOffset_z = 0.0;
	m_pMembCtrl->GetEndOffsetLeng(dOffset_y, dOffset_z, key, 0, nLengTypei, FALSE, TRUE);
	double dDgnOffsetI = max(dOffset_y, dOffset_z);
	m_pMembCtrl->GetEndOffsetLeng(dOffset_y, dOffset_z, key, 1, nLengTypej, FALSE, TRUE);
	double dDgnOffsetJ = max(dOffset_y, dOffset_z);

	if (nOffSetOutType == 0)
	{// �ؼ��� �������?(���������� ���ŵ�)�� ���ܿ��� ������ �Ÿ�
		dOffSetLy[0] = dDgnOffsetI - dOffSetLy[0];
		dOffSetLz[0] = dDgnOffsetI - dOffSetLz[0];
		dOffSetLy[1] = dDgnOffsetJ - dOffSetLy[1];
		dOffSetLz[1] = dDgnOffsetJ - dOffSetLz[1];
	}
	else
	{// ���� ����(Node)���������� �Ÿ�
		dOffSetLy[0] = dDgnOffsetI;
		dOffSetLz[0] = dDgnOffsetI;
		dOffSetLy[1] = dDgnOffsetJ;
		dOffSetLz[1] = dDgnOffsetJ;
	}

	if (m_pDoc->m_pAttrCtrl->ExistLeng(CurrKey))
	{
		m_pDoc->m_pAttrCtrl->GetLeng(CurrKey, rData);
		if (rData.BLy == 0.0)	rData.BLy = dMemLy;
		if (rData.BLz == 0.0)	rData.BLz = dMemLz;

		if (rData.bNotUseBLu)	rData.BLu = 0.0;
		else					rData.BLu = (rData.BLu == 0.0 ? rData.BLz : rData.BLu);
	}
	else
	{
		// Set Default Length.
		rData.BLy = dMemLy;
		rData.BLz = dMemLz;

		T_DSTL_D cData;
		cData.Initialize();
		Get_DgnStlDstl(cData);
		rData.bNotUseBLu = cData.bAllFrameLaterallyBraced;

		if (rData.bNotUseBLu)
		{
			// Get Element Type.
			int nType = Get_DgnGenMbtp(CurrKey);
			if (nType == D_MBTP_BEAM)	rData.BLu = 0.0;
			else						rData.BLu = rData.BLz;
		}
		else
		{
			rData.BLu = (rData.BLu == 0.0 ? rData.BLz : rData.BLu);
		}
	}

	if (IsJUD())
	{
		int nType = Get_DgnGenMbtp(CurrKey);
		if (nType == D_MBTP_BEAM)// Steel Beam�� ���? �����?
		{
			double dLbMax = 0.0;
			CArray<double, double> arSpace;
			int nIMJ = iPosiNo == 0 ? 0 : (iPosiNo == 4 ? 2 : 1);
			if (m_pMembCtrl->Get_Blcf_SpaceList(key, arSpace, dLbMax, nIMJ))
			{
				rData.BLu = dLbMax;
			}
		}
	}

	return TRUE;
}

// MQC10875 : ö���� tapered section gruop�� ���ؼ��� ������ ���? ��ü�� �ϳ��� �����ϱ� ������ �ش� �����? section�� �ƴ� ���? ��ü�� section �����;� ��.
// bDgnMemb : TRUE, ���? ������ ��ü�� I, J �ܸ� ��������
//            FALSE, �ش���(key)�� I, J �ܸ� ��������
BOOL CDgnDataCtrl::Get_DgnStlSect(int key, T_SECT_D& rData, BOOL bDgnMemb/*=FALSE*/)
{
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData)) { return FALSE; }
	// Skip if Element Type is Wall.
	if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))
	{
		// Change by ZINU.('01.10.6). For Group of Tapered Section.
		T_TSGR_K TsgrKey = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(Showkey);
		BOOL bCheck = FALSE;
		if (TsgrKey > 0 && !bDgnMemb)	bCheck = m_pDoc->calcElemSect(Showkey, rData, 1, TRUE);
		else						    bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro, rData);
		ASSERT(bCheck);
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnHcbmSect(int PartK, int iPartNo, T_SECT_D& rData)
{
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeHcbm(PartK, Showkey)) { return FALSE; }

	T_HCBM_K HcbmKey;
	if (!m_pDoc->m_pAttrCtrl->GetHcbmAssigned(PartK, HcbmKey)) { return FALSE; }

	T_HCBM_D HcbmD; HcbmD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD)) { return FALSE; }

	int nElem = HcbmD.aElemListPart[iPartNo].GetSize();
	T_ELEM_K ElemIK = HcbmD.aElemListPart[iPartNo].GetAt(0);
	T_ELEM_K ElemJK = HcbmD.aElemListPart[iPartNo].GetAt(nElem - 1);

	T_SECT_D SectI; SectI.Initialize();
	T_ELEM_D ElemI; ElemI.Initialize();
	ElemI.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemIK, ElemI)) { return FALSE; }
	m_pDoc->m_pAttrCtrl->GetSectDesign(ElemI.elpro, SectI);
	rData = SectI;

	T_SECT_D SectJ; SectJ.Initialize();
	T_ELEM_D ElemJ; ElemJ.Initialize();
	ElemJ.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemJK, ElemJ)) { return FALSE; }
	m_pDoc->m_pAttrCtrl->GetSectDesign(ElemJ.elpro, SectJ);

	if (iPartNo == 1) //Regular
	{
		rData.SectBefore.SectJ = SectJ.SectBefore.SectI;
	}
	else //Tapered
	{
		rData.SectBefore.SectJ = SectJ.SectBefore.SectJ;
	}

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnHcbmTaperedSectSize(int PartK, int iPartNo, double& dHc, double& dBc)
{
	T_SECT_D SectData; SectData.Initialize();
	if (!Get_DgnHcbmSect(PartK, iPartNo, SectData)) { return FALSE; }

	double dHI = SectData.SectBefore.SectI.Size[0];
	double dBI = SectData.SectBefore.SectI.Size[1];
	double dHJ = SectData.SectBefore.SectJ.Size[0];
	double dBJ = SectData.SectBefore.SectJ.Size[1];

	double dPartLength = m_pMembCtrl->GetMembLengByIncludeElem(PartK);
	double dOffsetLength = 0.0;
	T_HCBM_K HcbmKey;
	T_HCBM_D HcbmD; HcbmD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetHcbmAssigned(PartK, HcbmKey)) ASSERT(0);
	if (!m_pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))	ASSERT(0);
	if (iPartNo == 0)      dOffsetLength = HcbmD.dL1;
	else if (iPartNo == 2) dOffsetLength = dPartLength - HcbmD.dL2;

	dHc = dHJ + (dHI - dHJ)*(dPartLength - dOffsetLength) / dPartLength;
	dBc = dBJ + (dBI - dBJ)*(dPartLength - dOffsetLength) / dPartLength;

	return TRUE;
}

void CDgnDataCtrl::Create_DgnGenRedu()
{
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	if (pDgnDynData->m_arReduData.GetCount() > 0) { return; }
	if (pDgnDynData->m_arReduKind.GetCount() > 0) { return; }

	if (m_pDoc->m_pAttrCtrl->ExistDctl())
	{
		T_DCTL_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDctl(rData);

		int iCount = rData.arReduction.GetSize();
		if (iCount > 0)
		{
			// ���� �ٲ� Story Data�� ������ ����ϴ�?.(2000.6.15.)
			CStoryData* pStoryData = new CStoryData(m_pDoc);
			// nSelectionType == 0   // �ش����� ���Ϻη� �����? ����, ���?
			//                == 1   // �ش����� ����η�? �����? ����, ���?
			//                == 2   // �ش����� ����ο�? ���Ϻη� �����? ���? ����, ���?
			//                == 3   // �ش����� ���Ϻ��� ���? ����, ���?
			//                == 4   // �ش����� �������? ���? ����, ���?
			//                == 5   // �ش�����
			int nSelectionType = 1;
			pStoryData->MakeStoryData(nSelectionType);//0 �Ǵ� 1�� �ƴϸ� return
			for (int i = 0; i < iCount; i++)
			{
				T_DCTL_BASE bData;
				bData.Initialize();
				bData = rData.arReduction.GetAt(i);
				int nCH = rData.nReductType;
				BOOL bReduAxl = rData.bReduAxl;
				BOOL bReduMom = rData.bReduMom;
				BOOL bReduShr = rData.bReduShr;
				Cal_DgnGenRedu(nCH, bData, pStoryData, bReduAxl, bReduMom, bReduShr);
			}
			if (pStoryData != nullptr) { delete pStoryData; }
		}
	}
}

void CDgnDataCtrl::Cal_DgnGenRedu(int nCH, T_DCTL_BASE Data, CStoryData* pStoryData,
	BOOL bReduAxl, BOOL bReduMom, BOOL bReduShr)
{
	int BaseStoryNo = 0;
	double ReducFactor = 0.0, Factor = 0.0;
	double Rmax = Data.dRmax;
	double Rmin = Data.dRmin;

	// Story Key�� ������, Story Data�� ������ �ɴϴ�.
	T_STOR_D sData;
	sData.Initialize();
	m_pDoc->m_pAttrCtrl->GetStor(Data.KeyStory, sData);

	CArray<CString, CString&> strStorList;
	m_pDoc->m_pAttrCtrl->GetStorList(strStorList);
	int iCount = strStorList.GetSize();
	for (int i = 0; i < iCount; i++)
	{
		CString strStoryName = strStorList.GetAt(i);
		if (strStoryName == sData.StoryName)	BaseStoryNo = i;
	}

	for (int i = BaseStoryNo - 1; i >= 0; i--)
	{
		CString strStoryName = strStorList.GetAt(i);
		if (nCH)
		{
			if ((BaseStoryNo - i) <= 1)        Factor = 1.0;
			else if ((BaseStoryNo - i) <= 3)   Factor = 0.85;
			else if ((BaseStoryNo - i) <= 5)   Factor = 0.70;
			else if ((BaseStoryNo - i) <= 8)   Factor = 0.65;
			else if ((BaseStoryNo - i) <= 20)  Factor = 0.60;
			else                            Factor = 0.55;
		}
		else
		{
			Factor = Rmax - ReducFactor;
			if (Factor > Rmax)       Factor = Rmax;
			else if (Factor < Rmin)  Factor = Rmin;
			// Element���� Reduction Factor�� �����մϴ�.
			ReducFactor = ReducFactor + 0.05;
		}
		Set_DgnGenRedu(Data, pStoryData, strStoryName, Factor, bReduAxl, bReduMom, bReduShr);
	}
}

void CDgnDataCtrl::Set_DgnGenRedu(T_DCTL_BASE rData, CStoryData* pStoryData, CString StoryName,
	double dFactor, BOOL bReduAxl, BOOL bReduMom, BOOL bReduShr)
{
	if (m_pDoc->m_pAttrCtrl->ExistStor(StoryName))
	{
		// Story Name�� Story Key�� ���ɴϴ�.
		int key = m_pDoc->m_pAttrCtrl->GetStorKey(StoryName);
		// �� ���� �ִ� Element�� ���� ������ �´�.
		CArray<T_STOR_K, T_STOR_K> StoryKey;
		StoryKey.Add(key);
		CArray<T_ELEM_K, T_ELEM_K> ElemKey;
		// �ٲ� Story Data�� Element Key�� �޾ƿɴϴ�.(2000.6.15.)
		pStoryData->GetStoryElemKey(key, ElemKey);
		//m_pDoc->m_pAttrCtrl->GetKeyListStorUser(&StoryKey,NULL,&ElemKey);
		// Reduction Factor ���� �غ�
		CArray<T_ELEM_K, T_ELEM_K> aSelKey;
		int iCount = ElemKey.GetSize();
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				T_ELEM_K key = ElemKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(key, eData))	continue;
				// Change by ZINU.('02.10.22).  Include not only column but also wall.
				if (m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))  // Wall.
				{
					T_NODE_D nData1, nData2;
					// 1st node.
					nData1.Initialize();
					m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[0], nData1);
					BOOL bOkX1 = ((rData.dMinX - cDgn_Zero) < nData1.x && (rData.dMaxX + cDgn_Zero) > nData1.x);
					BOOL bOkY1 = ((rData.dMinY - cDgn_Zero) < nData1.y && (rData.dMaxY + cDgn_Zero) > nData1.y);
					// 2nd node.
					nData2.Initialize();
					m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[1], nData2);
					BOOL bOkX2 = ((rData.dMinX - cDgn_Zero) < nData2.x && (rData.dMaxX + cDgn_Zero) > nData2.x);
					BOOL bOkY2 = ((rData.dMinY - cDgn_Zero) < nData2.y && (rData.dMaxY + cDgn_Zero) > nData2.y);
					// Check OK.
					if (bOkX1 && bOkY1 && bOkX2 && bOkY2)	aSelKey.Add(key);
				}
				else
				{
					if (Get_DgnGenMbtp(key) == D_MBTP_COLUMN)
					{
						T_NODE_D nData;
						nData.Initialize();
						m_pDoc->m_pAttrCtrl->GetNode(eData.elnod[0], nData);
						BOOL bOkX = ((rData.dMinX - cDgn_Zero) < nData.x && (rData.dMaxX + cDgn_Zero) > nData.x);
						BOOL bOkY = ((rData.dMinY - cDgn_Zero) < nData.y && (rData.dMaxY + cDgn_Zero) > nData.y);
						if (bOkX && bOkY)	aSelKey.Add(key);
					}
				}
			}

			CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				int key = aSelKey.GetAt(i);
				pDgnDynData->m_arReduData.SetAt(key, dFactor);
				int iReduKind = (bReduAxl ? 1 : 0) * 100 + (bReduMom ? 1 : 0) * 10 + (bReduShr ? 1 : 0) * 1;
				pDgnDynData->m_arReduKind.SetAt(key, iReduKind);
			}
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_STORY_DATA), MB_OK);
	}
}

BOOL CDgnDataCtrl::Get_DgnGenDctl(T_DCTL_D& rData)
{
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDctl())
	{
		m_pDoc->m_pAttrCtrl->GetDctl(rData);
		return TRUE;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConDcon(T_DCON_D& rData)
{
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcon())
	{
		m_pDoc->m_pAttrCtrl->GetDcon(rData);
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		rData.DesignCode = rPref.DgnCode.ConcCode;

		// EC2:04/ EC2-2:05������ DgnCode �� National Annex�� ������.
		const CString& strConCode = rData.DesignCode;
		if (strConCode == CONCODE_EC2_04 || strConCode == CONCODE_EC2_2_05)
		{
			rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
			rData.dA = 0.7;
			rData.dB = 1.1;
			rData.dC = 1.0;
			rData.bAutoC = TRUE;
		}
		else if (strConCode == CONCODE_IRC112_2011 || strConCode == CONCODE_IRC112_2020)
		{
			rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
			rData.dA = 0.7;
			rData.dB = 1.1;
			rData.dC = 1.0;
			rData.bAutoC = TRUE;
			rData.dAlphacc = 0.67;
		}
		else if (strConCode == CONCODE_IS456_2000)
		{
			rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
			rData.dA = 0.7;
			rData.dB = 1.1;
			rData.dC = 1.0;
			rData.bAutoC = TRUE;
			rData.dAlphacc = 0.67;
		}
		else if (strConCode == CONCODE_TWN_USD112)
		{
			rData.dSCWB_Phic = 0.65;
		}
		else if(strConCode == NTC_DCEC_2023)
		{
			rData.nSCWBDgnMethod = 1; // norminal strength
		}

		// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50010-02).
		int iSafeLevel = 0, iEqGrade = 0;
		if (Get_DgnConChinaDef(rData.DesignCode, iSafeLevel, iEqGrade))
		{
			if (rData.bSpecialSeismic)	rData.nClass = iEqGrade;
			else						rData.nClass = iSafeLevel;
		}
	}

	//	if(IsJUD()) // PMS:4215
	//	{ rData.DesignCode = _T("AIJ-WSD99"); }

	double pdPhi[6] = { 0.,0.,0.,0.,0.,0. };
	if (!Get_DgnConPhiDef(rData.DesignCode, pdPhi))  ASSERT(0);
	for (int i = 0; i < 6; i++)
	{
		if (rData.dPhi[i] == 0.0)
		{
			rData.dPhi[i] = pdPhi[i];
		}
	}

	// Alphacc
// #if defined(_CIVIL)
//   if(rData.dAlphacc==0.0) rData.dAlphacc = 0.85;
// #else
//   if(rData.dAlphacc==0.0) rData.dAlphacc = (rData.nNationalAnnex==0)? 0.85 : 1.00; // Modify by GAY. PMS:4020 .('09.11.30). National Annex=Italy�� ���?, 0.85 Setting.
// #endif
	const CString& strConCode = rData.DesignCode;
	// by GAY. ('14.02.04). PMS:4817. add Singapore National Annex.
	if (rData.dAlphacc == 0.0)
	{
		rData.dAlphacc = [&rData](int nNA) -> double
		{
#if defined (_CIVIL)
			int nRCCode = CDgnCodeCtrl::GetConCodeID(rData.DesignCode);
			if (nRCCode != Eurocode2_2_05)
				return 0.0;

			auto enDgnCode = dgn::converter::CDgnTypeConverter::GetDgnCode(nRCCode, rData.nNationalAnnex);
			dgn::lib::CNationalAnnexInterface::Instance()->SetCode(enDgnCode);

			def::enAlphacc Type;
			dgn::lib::CNationalAnnexInterface::Instance()->GetParamType(Type);

			dgn::def::TNAParam Param;
			switch (Type)
			{
			case enAlphacc::None:
				break;
			case enAlphacc::bCompression:
			{
				Param.vb.push_back(true);
			}
			break;
			case enAlphacc::bSimpleBending:
			{
				Param.vb.push_back(true);
			}
			break;
			case enAlphacc::bLightWeight:
			{
				Param.vb.push_back(false);
			}
			break;
			case enAlphacc::RatioOfLongterm:
			{
				Param.vd.push_back(0.9);
			}
			break;
			default:
				ASSERT(0);
				break;
			}

			return dgn::lib::CNationalAnnexInterface::Instance()->GetAlphacc(Type, Param);
#else

			// EN1992-1-1 2004 3.1.6 (1) ��cc for design compressive strength
			switch (nNA)
			{
				case dgn::def::enNationalAnnex::Belgium:
				case dgn::def::enNationalAnnex::Finland:
				case dgn::def::enNationalAnnex::Germany:
				case dgn::def::enNationalAnnex::Greece:
				case dgn::def::enNationalAnnex::Luxembourg:
				case dgn::def::enNationalAnnex::Norway:
				case dgn::def::enNationalAnnex::Italy:
				case dgn::def::enNationalAnnex::Singapore:
					return 0.85;
				default: // Recommended and other National Annexes
					return 1.0;
			}
#endif
		}(rData.nNationalAnnex);
	}

#if defined _CIVIL
	switch ( rData.nNationalAnnex )
	{
	case 2:
		{
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.004 : rData.dRhominVertShell;
			rData.dRhominHorzShell = rData.dRhominHorzShell == 0.0 ? 0.001 : rData.dRhominHorzShell;
		}
		break;
	case 0:
	case 1:
	default:
		{
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.002 : rData.dRhominVertShell;
			rData.dRhominHorzShell = rData.dRhominHorzShell == 0.0 ? 0.001 : rData.dRhominHorzShell;
		}
		break;
	}
#else
	// EN1992-1-1 9.6.2 (1) ��,min for vertical reinforcement of wall
	switch (rData.nNationalAnnex)
	{
		case dgn::def::enNationalAnnex::Netherlands:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.0 : rData.dRhominVertShell;
			break;
		case dgn::def::enNationalAnnex::Germany:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.0015 : rData.dRhominVertShell;
			break;
		case dgn::def::enNationalAnnex::Italy:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.004 : rData.dRhominVertShell;
			break;
		case dgn::def::enNationalAnnex::Slovenia:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.003 : rData.dRhominVertShell;
			break;
		case dgn::def::enNationalAnnex::Spain:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.0012 : rData.dRhominVertShell;
			break;
		case dgn::def::enNationalAnnex::Sweden19:
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.001 : rData.dRhominVertShell;
			break;
		default: // Recommended and other National Annexes
			rData.dRhominVertShell = rData.dRhominVertShell == 0.0 ? 0.002 : rData.dRhominVertShell;
			break;
	}
	// EN1992-1-1 9.6.3 (1) ��,hmin for horizontal reinforcement of wall
	switch (rData.nNationalAnnex)
	{
		case dgn::def::enNationalAnnex::Germany:
		case dgn::def::enNationalAnnex::Netherlands:
		case dgn::def::enNationalAnnex::Norway:
		case dgn::def::enNationalAnnex::Sweden19:
			rData.dRhominHorzShell = rData.dRhominHorzShell == 0.0 ? 0.0 : rData.dRhominHorzShell;
			break;
		case dgn::def::enNationalAnnex::Spain:
			rData.dRhominHorzShell = rData.dRhominHorzShell == 0.0 ? 0.004 : rData.dRhominHorzShell;
			break;
		default: // Recommended and other National Annexes
			rData.dRhominHorzShell = rData.dRhominHorzShell == 0.0 ? 0.001 : rData.dRhominHorzShell;
		break;
	}
#endif

	if (strConCode == CONCODE_IRC112_2011 && rData.dAlphacc != 0.67)
	{
		rData.dAlphacc = 0.67;
	}
	else if (strConCode == CONCODE_IS456_2000 && rData.dAlphacc != 0.67)
	{
		rData.dAlphacc = 0.67;
	}
	else if (strConCode == CONCODE_IRC112_2020 && rData.dAlphacc != 0.67)
	{
		rData.dAlphacc = 0.67;
	}

	if (rData.dRhor == 0.0)
	{
		rData.dRhor = 0.03;
	}

	if (rData.dRhoc == 0.0)
	{
		//���� �Լ� CDgnConBarRatioDlg::Initial_DefaultData() �����? ��ġ�µ� -_-;
		rData.dRhoc = [strConCode]() -> double
		{
			if ( strConCode == CONCODE_EC2_2_05 || strConCode == CONCODE_EC2_04 )
				return 0.04;
			else if ( strConCode == CONCODE_AASHTO_LRFD20 || strConCode == CONCODE_AASHTO_LRFD24 || strConCode == CONCODE_AREMA_2023)
				return 0.08;
			else
				return 0.03;
		}();
	}

	if (rData.dRhow == 0.0)
	{
		rData.dRhow = 0.04;
	}

	// Add by ZINU.('04.02.04). For Scale Up Factor for Shear.
	if (!Get_DgnConSufsDef(strConCode, rData.nClass, rData.iAlphaType, rData.dAlpha1, rData.dAlpha2)) { ASSERT(0); }
	// Add by ZINU.('04.02.16). For Alpha by AlphaType.
	if (rData.iAlphaType == 2)
	{
		rData.dAlpha2 = 1.0;	// Use Only Alpha1.
		rData.dAlpha2_Colm = 1.0;
	}
	else if (rData.iAlphaType == 3)
	{
		rData.dAlpha1 = 1.0;	// Use Only Alpha2.
	}

	//(2009.06.16) Add by Unsang :: Only, EC2-2:05
	if (strConCode == CONCODE_EC2_2_05)
	{
		if (rData.dMinRhoc == 0.0)  rData.dMinRhoc = 0.002; // UNIT..NONE.
		if (rData.dMaxLimitConc == 0.0)  rData.dMaxLimitConc = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 70.0);// UNIT..STRESS
		if (rData.dMinLimitConc == 0.0)  rData.dMinLimitConc = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 30.0);//
		if (rData.dMaxYieldRbar == 0.0)  rData.dMaxYieldRbar = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 500.0);//
		if (rData.dMinMainRbarD == 0.0)  rData.dMinMainRbarD = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 8.0);// UNIT..LENGTH
		if (rData.dMinSubRbarD == 0.0)  rData.dMinSubRbarD = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 6.0);//
		if (rData.dMaxSubRbarSp == 0.0)  rData.dMaxSubRbarSp = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 400.0);//
	}
	else if (strConCode == CONCODE_IRC112_2011 || strConCode == CONCODE_IRC112_2020)
	{
		if (rData.dMinRhoc == 0.0)  rData.dMinRhoc = 0.002; // UNIT..NONE.
		if (rData.dMaxLimitConc == 0.0)  rData.dMaxLimitConc = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 70.0);// UNIT..STRESS
		if (rData.dMinLimitConc == 0.0)  rData.dMinLimitConc = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 30.0);//
		if (rData.dMaxYieldRbar == 0.0)  rData.dMaxYieldRbar = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 500.0);//
		if (rData.dMinMainRbarD == 0.0)  rData.dMinMainRbarD = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 8.0);// UNIT..LENGTH
		if (rData.dMinSubRbarD == 0.0)  rData.dMinSubRbarD = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 6.0);//
		if (rData.dMaxSubRbarSp == 0.0)  rData.dMaxSubRbarSp = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 400.0);//
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConDcrb(int key, T_DCRB_D& rData)
{
	// MQC:2341-Seungjun-20081218 Data�� ���� ���? Default Data ����.
	if (!m_pDoc->m_pAttrCtrl->GetDcrb(rData))
	{
		rData.Initialize();
		Get_DgnConDcrbDef(rData);
	}
	// MNET:4076-SHJUNG-20100310 - BEHorizontalRebarWall ����Ÿ ����
	if (rData.BEHorizontalRebarWall == _T(""))
	{
		CDBLib::GetDefaultSubRebarSize(4, rData.BEHorizontalRebarWall);
	}
	// Add by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Elem.
	if (key > 0)
	{
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD)) { ElemD.Initialize(); }
		if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
		{
			int iMembType = Get_DgnGenMbtp(key);
			switch (iMembType)
			{
			case D_MBTP_BEAM: { Get_DgnConDcbb(key, rData); }break;
			case D_MBTP_COLUMN: { Get_DgnConDcbc(key, rData); } break;
			case D_MBTP_BRACE: { Get_DgnConDcbr(key, rData); } break;
			default: ASSERT(0); break;
			}
		}
		else
		{
			ASSERT(0);
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConDcrb(T_DCRB_D& rData)
{
	return Get_DgnConDcrb(0, rData);
}

// PMS:4020-Seungjun-20090731 EC2:04 Slab/Wall Design.
BOOL CDgnDataCtrl::Get_DgnWalDcrb(int key, T_DCRB_D& rData)
{
	// MQC:2341-Seungjun-20081218 Data�� ���� ���? Default Data ����.
	if (!m_pDoc->m_pAttrCtrl->GetDcrb(rData))
	{
		rData.Initialize();
		Get_DgnConDcrbDef(rData);
	}
	// MNET:4076-SHJUNG-20100310 - BEHorizontalRebarWall ����Ÿ ����
	if (rData.BEHorizontalRebarWall == _T(""))
	{
		CDBLib::GetDefaultSubRebarSize(4, rData.BEHorizontalRebarWall);
	}

	// Add by ZINU.('06.11.28). NO:2484, Get Rebar Data for Design by Elem.
	if (key > 0)
	{
		Get_DgnWalDcbw(key, rData);
	}
	if (key > 0)
	{
		Get_DgnWallEbmw(key, rData);
	}

	return TRUE;
}
int CDgnDataCtrl::Get_DgnBEEBarDesignMethod(int key)
{
	T_DCON_D rData;
	if (!m_pDoc->m_pAttrCtrl->GetDcon(rData))
	{
		rData.Initialize();
	}
	if (rData.DesignCode != CONCODE_IS456_2000)	{ return 0; } 
	if (!rData.bConsiderBEDesign)				{ return 0; }
	int nBEEBarDesignMethod = rData.nBEEBarDesignMethod;
	T_EBMW_D rEbmwD; rEbmwD.Initialize();
	T_EBMW_K Key; Key.keymap = key;
	if (m_pDoc->m_pAttrCtrl->GetEbmw(Key, rEbmwD))
	{
		nBEEBarDesignMethod = rEbmwD.iBEEbarDesignMethod;
	}
	return nBEEBarDesignMethod;
}
int CDgnDataCtrl::Get_DgnWalBndrElemMethod(int iWallID)
{
	// ����ڰ�? �Է��� �����Ͱ� ������, T_DCON_D�� ������ �����Ѵ�.
	T_BEMW_D BemwD;
	if (!m_pDoc->m_pAttrCtrl->GetBemw(iWallID, BemwD)) { BemwD.Initialize(); }

	if (BemwD.bBndrElemMethod)
	{
		return BemwD.nMethodType;
	}

	T_DCON_D DconD;
	if (!Get_DgnConDcon(DconD)) { DconD.Initialize(); }
	return DconD.iBndrElemMethod;
}

BOOL CDgnDataCtrl::Get_DgnWalUccf(int key, T_UCCF_D& rData)
{
	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_UCCF_D rTempData;
			rTempData.Initialize();
			Get_DgnConUccf(iKey, rTempData);

			rData.dGammard = max(rData.dGammard, rTempData.dGammard);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDgnDataCtrl::Get_DgnWalLamb(int key, double& rdLambda, BOOL& rbLightCon) // key : Wall Key
{
	rbLightCon = FALSE;
	rdLambda = 1.0;

	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { ASSERT(0); return FALSE; }

	int nCount = wData.arElemKey.GetSize();
	if (nCount < 1) { ASSERT(0); return FALSE; }

	for (int i = 0; i < nCount; i++)
	{
		int nElemKey = wData.arElemKey.GetAt(i);
		BOOL bLambda = FALSE;
		double dLambda = 0.0;

		if (!Get_DgnConLamb(nElemKey, dLambda, bLambda)) { ASSERT(0); continue; }

		if (bLambda)
		{
			rbLightCon = TRUE;
			rdLambda = min(rdLambda, dLambda);
		}
	}

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnWalCEnv(int key, T_REXC_D& rData) // key : Wall Key
{
	rData.Initialize();
	rData.nExposure = 0;

	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { ASSERT(0); return FALSE; }
	int nCount = wData.arElemKey.GetSize();
	if (nCount < 1) { ASSERT(0); return FALSE; }

	for (int i = 0; i < nCount; i++)
	{
		int nElemKey = wData.arElemKey.GetAt(i);

		T_REXC_D TempData;
		if (!Get_DgnConCEnv(nElemKey, TempData)) { ASSERT(0); continue; }

		if (TempData.nExposure == 1)
		{
			rData.nExposure = TempData.nExposure;
		}
	}

	return TRUE;
}

double CDgnDataCtrl::Get_DgnWalLamb(int key)
{
	double dLambda = 1.0;
	BOOL bLightCon = FALSE;
	if (!Get_DgnWalLamb(key, dLambda, bLightCon)) { ASSERT(0); return dLambda; }
	return dLambda;
}

UINT CDgnDataCtrl::Get_DgnWalCEnv(int key)
{
	T_REXC_D RexcData;
	if (!Get_DgnWalCEnv(key, RexcData)) { ASSERT(0); return 0; }
	return RexcData.nExposure;
}

int CDgnDataCtrl::Get_DgnWalPmdm(T_WALL_KEY WallK)
{
	return m_pDoc->m_pAttrCtrl2->Get_DgnWalPmdm(WallK, TRUE);
}

int CDgnDataCtrl::Get_DgnWalPmdm_SeisEval()
{
	// ���������򰡿����� P-M �ɼ��� �����Ƿ�, ���� �������? ������ �Ӵϴ�.
	return D_DGN_PMDM_E;
}

BOOL CDgnDataCtrl::Get_DgnSrcRebs(int key, BOOL& bFlag, T_REBS_D& rData)
{
	// key is Element.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData))	return FALSE;

	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetRebs(eData.elpro, rData);
	if (!bCheck)
	{
		rData.dConcCenter = 0.0;
		rData.bUseRebarSpace = TRUE;
		rData.dRebarSpace = 0.0;

		T_SECT_D sData;
		sData.Initialize();
		m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro, sData);
		CString strShape = sData.SectBefore.Shape.Left(1);

		if (strShape == _T("R"))	// Rectangle(RHB, RBO, RBC, RPO, RPC).
		{
			CDBLib::GetDefaultSrcRebarSize(rData.MainRebarName);	// Same Column.
			CDBLib::GetDefaultSrcSubbarSize(rData.SubRebarName);
			CDBLib::GetDefaultSrcSubbarSpace(rData.dSubRebarSpace);
			rData.nQrb = 4;
			rData.nRow = 2;
		}
		else if (strShape == _T("C"))	// Circular(CHB, CBO, CBC, CPO, CPC).
		{
			CDBLib::GetDefaultSrcRebarSize(rData.MainRebarName);	// Same Column.
			CDBLib::GetDefaultSrcSubbarSize(rData.SubRebarName);
			CDBLib::GetDefaultSrcSubbarSpace(rData.dSubRebarSpace);
			rData.nQrb = 4;
			rData.nRow = 0;
		}
		else if (strShape == _T("E"))	// Empty(EBC, EPC).
		{
			rData.MainRebarName = _T("");
			rData.nQrb = 0;
			rData.nRow = 0;
		}
	}

	bFlag = rData.MainRebarName == _T("") ? FALSE : TRUE;

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConRebc(int key, BOOL& bFlag, T_REBC_D& rData)
{
	// key is ElemNo.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData)) { return FALSE; }

	// Modify by GAY. PMS:4073. ('09.09.24). Get Checking Rebar Data for Member(Element).
	BOOL bGetRebc = m_pDoc->m_pAttrCtrl->GetRebcByElemK(Showkey, rData);

	bFlag = (rData.MainRebarName == _T("")) ? FALSE : TRUE;

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConRebr(int key, BOOL& bFlag, T_REBR_D& rData)
{
	// key is ElemNo.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData)) { return FALSE; }

	// Modify by GAY. PMS:4073. ('09.09.24). Get Checking Rebar Data for Member(Element).
	BOOL bGetRebr = m_pDoc->m_pAttrCtrl->GetRebrByElemK(Showkey, rData);

	bFlag = (rData.MainRebarName == _T("")) ? FALSE : TRUE;

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConMrft(int key, T_MRFT_D& rData)
{
	rData.Initialize();
	rData.dFactor = 0.0;

	// Change by ZINU.('03.11.19). For all Concrete Code.
	if (Get_DgnGenMbtp(key) != D_MBTP_BEAM) { return FALSE; }
	if (!m_pDoc->m_pAttrCtrl->IsNegativeMomentMember(key)) { return FALSE; }

	T_DCON_D DconData;
	DconData.Initialize();
	Get_DgnConDcon(DconData);
	if (!m_pDoc->m_pAttrCtrl->GetMrft(key, rData))	// by Member.
	{
		// Change by ZINU.('05.02.01). Initialize rData.dFactor by DconData.dMrft.
		rData.dFactor = DconData.dMrft;
		Get_DgnConMrftDef(DconData.DesignCode, rData.dFactor);
	}
	return TRUE;
}

// Add by GAY. MNET:3780. ('09.03.20). Torsion Reduction Factor.
BOOL CDgnDataCtrl::Get_DgnConTrft(int key, T_TRFT_D& rData)
{
	rData.Initialize();
	rData.dFactor = 0.0;

	if (Get_DgnGenMbtp(key) != D_MBTP_BEAM) { return FALSE; }

	T_DCON_D DconData;
	DconData.Initialize();
	Get_DgnConDcon(DconData);
	if (!m_pDoc->m_pAttrCtrl->GetTrft(key, rData)) // by Member.
	{
		rData.dFactor = DconData.dTrft;
		Get_DgnConTrftDef(DconData.DesignCode, rData.dFactor);
	}
	return TRUE;
}

// Add by GAY. PMS:4267. ('11.12.28). �߱��ű���-��Ȯ���� ���װ��?.
BOOL CDgnDataCtrl::Get_DgnConUccf(int key, T_UCCF_D& rData)
{
	rData.Initialize();
	rData.dGammard = 0.0;

	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!Get_DgnGenMatd(key, MatdD)) { return FALSE; }
	if (MatdD.Type != _T("C")) return FALSE;

	// ��Ȯ���� ���װ����? ���� Level���θ� �Է��� �����ϴ�. (Global ����:T_DCON_D���� �߰����� ����)
	if (!m_pDoc->m_pAttrCtrl2->GetUccf(key, rData)) { return FALSE; }

	return TRUE;
}

// SIAM / 2013.01.23 / KCI-USD12 : �淮��ũ��Ʈ���?
BOOL CDgnDataCtrl::Get_DgnConLamb(int key, double& rdLambda, BOOL& rbLightCon)
{
	rdLambda = 1.0;
	rbLightCon = FALSE;

	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!Get_DgnGenMatd(key, MatdD)) { ASSERT(0); return FALSE; }
	if (MatdD.Type != _T("C")) return TRUE;

	rbLightCon = MatdD.Data1.Design.bLambda;
	rdLambda = MatdD.Data1.Design.dLambda;

	return TRUE;
}

// SIAM / 2013.01.23 / KCI-USD12 : �տ�ȯ��
BOOL CDgnDataCtrl::Get_DgnConCEnv(int key, T_REXC_D& rData)
{
	rData.Initialize();
	rData.nExposure = 0;

	T_DCON_D DconData;
	DconData.Initialize();
	if (!Get_DgnConDcon(DconData)) { ASSERT(0); return FALSE; }
	if (!m_pDoc->m_pAttrCtrl2->GetRexc(key, rData)) // by Member.
	{
		rData.nExposure = DconData.nExposure;
	}

	return TRUE;
}

double CDgnDataCtrl::Get_DgnConLamb(int key)
{
	double dLambda = 1.0;
	BOOL bLightCon = FALSE;
	if (!Get_DgnConLamb(key, dLambda, bLightCon)) { ASSERT(0); return dLambda; }
	return dLambda;
}

UINT CDgnDataCtrl::Get_DgnConCEnv(int key)
{
	T_REXC_D RexcData;
	if (!Get_DgnConCEnv(key, RexcData)) { ASSERT(0); return 0; }
	return RexcData.nExposure;
}

double CDgnDataCtrl::Get_DgnConStan(int key)
{
	T_STAN_D StanD;
	if (m_pDoc->m_pAttrCtrl->GetStan(key, StanD))
	{
		return StanD.dAngle;
	}

	T_DCON_D DconD;
	DconD.Initialize();
	if (!Get_DgnConDcon(DconD)) { ASSERT(0); return 0.0; }
	return DconD.dStrutAngle;
}

int CDgnDataCtrl::Get_DgnConPmdm(T_ELEM_K ElemK)
{
	if (!CDBLib::IsConCodeForPMCalcMethod()) { return D_DGN_PMDM_E; }

	T_PMDM_D PmdmD;
	if (m_pDoc->m_pAttrCtrl2->GetPmdm(ElemK, PmdmD))
	{
		return PmdmD.nCalcMethod;
	}

	T_DCON_D DconD;
	if (!Get_DgnConDcon(DconD)) { ASSERT(0); return D_DGN_PMDM_E; }
	return DconD.nPMCurveMethod;
}

int CDgnDataCtrl::Get_DgnConPmdm_SeisEval()
{
	// ���������򰡿����� P-M �ɼ��� �����Ƿ�, ���� �������? ������ �Ӵϴ�.
	return D_DGN_PMDM_E;
}

bool CDgnDataCtrl::IsConSeisColmType(T_ELEM_K ElemK)
{
	return m_pDoc->m_pAttrCtrl2->ExistScol(ElemK) ? true : false;
}

int CDgnDataCtrl::GetConSeisColmType(T_ELEM_K ElemK)
{
	T_SCOL_D ScolD;
	if (m_pDoc->m_pAttrCtrl2->GetScol(ElemK, ScolD))
	{
		return ScolD.nType;
	}
	return -1;
}

BOOL CDgnDataCtrl::Get_DgnConRebb(int key, BOOL& bFlag, T_REBB_D& rData)
{
	// key is ElemNo.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData))	return FALSE;

	// Modify by GAY. PMS:4073. ('09.09.24). Get Checking Rebar Data for Member(Element).
	BOOL bGetRebb = m_pDoc->m_pAttrCtrl->GetRebbByElemK(Showkey, rData);
	if (bGetRebb)
	{
		BOOL bNoneI = ((rData.Top_I.RebarName == _T("") && rData.Bot_I.RebarName == _T("")) ? TRUE : FALSE);
		BOOL bNoneC = ((rData.Top_C.RebarName == _T("") && rData.Bot_C.RebarName == _T("")) ? TRUE : FALSE);
		BOOL bNoneJ = ((rData.Top_J.RebarName == _T("") && rData.Bot_J.RebarName == _T("")) ? TRUE : FALSE);
		if (bNoneI && bNoneC && bNoneJ)	bFlag = FALSE;
		else
		{
			bFlag = TRUE;
			Chk_DgnConRebb(rData);
		}
	}

	if (bFlag)
	{
		const CString& strConCode = CDBLib::GetConCodeName();
		if (strConCode == CONCODE_IS456_2000)
		{
			if ((rData.Top_I.iSubRebarNum % 2) != 0 || (rData.Top_C.iSubRebarNum % 2) != 0 || (rData.Top_J.iSubRebarNum % 2) != 0)
				bFlag = FALSE;
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnSrcRbsb(int key, BOOL& bFlag, T_RBSB_D& rData)
{
	// key is ElemNo.
	rData.Initialize();

	T_ELEM_K Showkey;
	if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey))	return FALSE;

	T_ELEM_D eData;
	eData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(Showkey, eData))	return FALSE;

	if (m_pDoc->m_pAttrCtrl->GetRbsb(eData.elpro, rData))
	{
		BOOL bNoneI = ((rData.Top_I.RebarName == _T("") && rData.Bot_I.RebarName == _T("")) ? TRUE : FALSE);
		BOOL bNoneC = ((rData.Top_C.RebarName == _T("") && rData.Bot_C.RebarName == _T("")) ? TRUE : FALSE);
		BOOL bNoneJ = ((rData.Top_J.RebarName == _T("") && rData.Bot_J.RebarName == _T("")) ? TRUE : FALSE);
		if (bNoneI && bNoneC && bNoneJ)	bFlag = FALSE;
		else
		{
			bFlag = TRUE;
			Chk_DgnSrcRbsb(rData);
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Chk_DgnConRebb(T_REBB_D& rData)
{
	if (rData.Top_I.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TI"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TI"), _T("TM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TI"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TI"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TI"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Top_C.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TM"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TM"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TM"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TM"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TM"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Top_J.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TJ"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TJ"), _T("TM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TJ"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TJ"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("TJ"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_I.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BI"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BI"), _T("BM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BI"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BI"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BI"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_C.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BM"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BM"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BM"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BM"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BM"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_J.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BJ"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BJ"), _T("BM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BJ"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BJ"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnConRebb(_T("BJ"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}

	if (rData.Top_I.RebarName2nd == _T("")) { rData.Top_I.RebarName2nd = rData.Top_I.RebarName; }
	if (rData.Top_C.RebarName2nd == _T("")) { rData.Top_C.RebarName2nd = rData.Top_C.RebarName; }
	if (rData.Top_J.RebarName2nd == _T("")) { rData.Top_J.RebarName2nd = rData.Top_J.RebarName; }
	if (rData.Bot_I.RebarName2nd == _T("")) { rData.Bot_I.RebarName2nd = rData.Bot_I.RebarName; }
	if (rData.Bot_C.RebarName2nd == _T("")) { rData.Bot_C.RebarName2nd = rData.Bot_C.RebarName; }
	if (rData.Bot_J.RebarName2nd == _T("")) { rData.Bot_J.RebarName2nd = rData.Bot_J.RebarName; }
	return TRUE;
}

BOOL CDgnDataCtrl::Chk_DgnSrcRbsb(T_RBSB_D& rData)
{
	if (rData.Top_I.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TI"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TI"), _T("TM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TI"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TI"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TI"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Top_C.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TM"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TM"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TM"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TM"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TM"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Top_J.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TJ"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TJ"), _T("TM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TJ"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TJ"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("TJ"), _T("BM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_I.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BI"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BI"), _T("BM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BI"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BI"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BI"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_C.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BM"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BM"), _T("BJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BM"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BM"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BM"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	if (rData.Bot_J.RebarName == _T(""))
	{
		BOOL bNoneRebar = FALSE;
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BJ"), _T("BI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BJ"), _T("BM"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BJ"), _T("TI"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BJ"), _T("TJ"), rData);
		if (!bNoneRebar)	bNoneRebar = Chk_DgnSrcRbsb(_T("BJ"), _T("TM"), rData);
		if (!bNoneRebar)	ASSERT(0);
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Chk_DgnConRebb(CString strFirst, CString strSecond, T_REBB_D& rData)
{
	// Change by ZINU.('03.08.29). Include iSubRebarNum.
	T_REBB_BASE RebbBase;
	RebbBase.Initialize();

	if (strSecond == _T("TI"))			RebbBase = rData.Top_I;
	else if (strSecond == _T("TM"))	RebbBase = rData.Top_C;
	else if (strSecond == _T("TJ"))	RebbBase = rData.Top_J;
	else if (strSecond == _T("BI"))	RebbBase = rData.Bot_I;
	else if (strSecond == _T("BM"))	RebbBase = rData.Bot_C;
	else if (strSecond == _T("BJ"))	RebbBase = rData.Bot_J;
	else	ASSERT(0);

	if (RebbBase.RebarName != _T(""))
	{
		if (strFirst == _T("TI"))		rData.Top_I = RebbBase;
		else if (strFirst == _T("TM"))	rData.Top_C = RebbBase;
		else if (strFirst == _T("TJ"))	rData.Top_J = RebbBase;
		else if (strFirst == _T("BI"))	rData.Bot_I = RebbBase;
		else if (strFirst == _T("BM"))	rData.Bot_C = RebbBase;
		else if (strFirst == _T("BJ"))	rData.Bot_J = RebbBase;
		else	ASSERT(0);
		return TRUE;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::Chk_DgnSrcRbsb(CString strFirst, CString strSecond, T_RBSB_D& rData)
{
	// Change by ZINU.('03.08.29). Include iSubRebarNum.
	T_RBSB_BASE RbsbBase;
	RbsbBase.Initialize();

	if (strSecond == _T("TI"))			RbsbBase = rData.Top_I;
	else if (strSecond == _T("TM"))	RbsbBase = rData.Top_C;
	else if (strSecond == _T("TJ"))	RbsbBase = rData.Top_J;
	else if (strSecond == _T("BI"))	RbsbBase = rData.Bot_I;
	else if (strSecond == _T("BM"))	RbsbBase = rData.Bot_C;
	else if (strSecond == _T("BJ"))	RbsbBase = rData.Bot_J;
	else	ASSERT(0);

	if (RbsbBase.RebarName != _T(""))
	{
		if (strFirst == _T("TI"))		rData.Top_I = RbsbBase;
		else if (strFirst == _T("TM"))	rData.Top_C = RbsbBase;
		else if (strFirst == _T("TJ"))	rData.Top_J = RbsbBase;
		else if (strFirst == _T("BI"))	rData.Bot_I = RbsbBase;
		else if (strFirst == _T("BM"))	rData.Bot_C = RbsbBase;
		else if (strFirst == _T("BJ"))	rData.Bot_J = RbsbBase;
		else	ASSERT(0);
		return TRUE;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConDcbb(int key, T_DCRB_D& rData)
{
	T_DCBB_D DcbbD; DcbbD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetDcbb(key, DcbbD))
	{
		rData.MainRebarBeam[0] = DcbbD.MainRebarBeam;
		for (int i = 1; i < 5; i++)	rData.MainRebarBeam[i] = _T("");
		// Change by ZINU.('06.12.08). NO:2484, Except Subbar, Sidebar, Covering with Taiwan,�̴��GJ.
		rData.StirrupBarBeam = DcbbD.StirrupBarBeam;
		rData.SideBarBeam = DcbbD.SideBarBeam;
		rData.dT_Beam = DcbbD.dT_Beam;
		rData.dB_Beam = DcbbD.dB_Beam;
		rData.iSubRebarNum_Beam = DcbbD.iSubRebarNum_Beam;
		rData.bSpliceBeamJP = DcbbD.bSpliceBeamJP;
		rData.iSpliceBeam = DcbbD.iSpliceBeam;
		rData.iSpliceBeamSubType = DcbbD.iSpliceBeamSubType;
		// Coded by Seungjun ('20070205) MNet:No.2619 For Taiwan. Design cosidered the compression rebars.
		rData.bDoublyBeam = DcbbD.bDoublyBeam;
		rData.dDoublyBeam = DcbbD.dDoublyBeam;
		// Add by ZINU.('08.03.26). NO:3292, Option to Apply Spacing Limit.
		rData.bCheckRebarSpacing_Beam = DcbbD.bCheckRebarSpacing_Beam;
	}
	if (rData.iSubRebarNum_Beam > 0)	return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConDcbc(int key, T_DCRB_D& rData)
{
	T_DCBC_D DcbcD; DcbcD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetDcbc(key, DcbcD))
	{
		rData.MainRebarColumn[0] = DcbcD.MainRebarColumn;
		for (int i = 1; i < 5; i++)	rData.MainRebarColumn[i] = _T("");
		// Change by ZINU.('06.12.08). NO:2484, Except Subbar, Sidebar, Covering with Taiwan,�̴��GJ.
		rData.StirrupBarColumn = DcbcD.StirrupBarColumn;
		rData.d0_Column = DcbcD.d0_Column;
		rData.iSubRebarNum_Column[0] = DcbcD.iSubRebarNum_Column[0];
		rData.iSubRebarNum_Column[1] = DcbcD.iSubRebarNum_Column[1];
		rData.bSpliceColumnJP = DcbcD.bSpliceColumnJP;
		rData.iSpliceColumn = DcbcD.iSpliceColumn;
		// Add by ZINU.('08.03.26). NO:3292, Option to Apply Spacing Limit.
		rData.bCheckRebarSpacing_Column = DcbcD.bCheckRebarSpacing_Column;
	}
	if (rData.iSubRebarNum_Column[0] > 0) return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConDcbr(int key, T_DCRB_D& rData)
{
	T_DCBR_D DcbrD; DcbrD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetDcbr(key, DcbrD))
	{
		rData.MainRebarBrace[0] = DcbrD.MainRebarBrace;
		for (int i = 1; i < 5; i++)	rData.MainRebarBrace[i] = _T("");
		// Change by ZINU.('06.12.08). NO:2484, Except Subbar, Sidebar, Covering with Taiwan,�̴��GJ.
		rData.StirrupBarBrace = DcbrD.StirrupBarBrace;
		rData.d0_Brace = DcbrD.d0_Brace;
		rData.iSubRebarNum_Brace[0] = DcbrD.iSubRebarNum_Brace[0];
		rData.iSubRebarNum_Brace[1] = DcbrD.iSubRebarNum_Brace[1];
		rData.bSpliceBraceJP = DcbrD.bSpliceBraceJP;
		rData.iSpliceBrace = DcbrD.iSpliceBrace;
		// Add by ZINU.('08.03.26). NO:3292, Option to Apply Spacing Limit.
		rData.bCheckRebarSpacing_Brace = DcbrD.bCheckRebarSpacing_Brace;
	}
	if (rData.iSubRebarNum_Brace[0] > 0)	return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalDcbw(int key, T_DCRB_D& rData)
{
	T_DCBW_D DcbwD; DcbwD.Initialize();
	T_DCBW_K Key;
	Key.keymap = key;
	if (m_pDoc->m_pAttrCtrl->GetDcbw(Key, DcbwD))
	{
		rData.VerticalRebarWall[0] = DcbwD.VerticalRebarWall;
		for (int i = 1; i < 5; i++)	rData.VerticalRebarWall[i] = _T("");
		rData.HorizontalRebarWall = DcbwD.HorizontalRebarWall;
		rData.EndRebarWall = DcbwD.EndRebarWall;
		rData.dDe_Wall = DcbwD.dDe_Wall;
		rData.dDw_Wall = DcbwD.dDw_Wall;
		// MNET:3909-Seungjun-20090210
		rData.bVerEndBar_Wall = FALSE;//DcbwD.bVerEndBar_Wall; // PMS:4752 Seungjun-20131118 : ö���� ���濡 ���� ������ ������ �� �ִ� ���? �߰�, ���� �ɼ� ����.
		rData.strVerEndBar_Wall = DcbwD.strVerEndBar_Wall;
		rData.strVerEndBarFy_Wall = DcbwD.strVerEndBarFy_Wall;
		// Modify by GAY. PMS:4076. ('10.04.22).
		rData.BEHorizontalRebarWall = DcbwD.BEHorizontalRebarWall;
		rData.dBEHorizontalRebarSpace = DcbwD.dBEHorizontalRebarSpace;
		rData.dBEVerticalRebarSpace = DcbwD.dBEVerticalRebarSpace;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnWallEbmw(int key, T_DCRB_D& rData)
{
	T_EBMW_D rEbmwD; rEbmwD.Initialize();
	T_EBMW_K Key; Key.keymap = key;
	if(m_pDoc->m_pAttrCtrl->GetEbmw(Key, rEbmwD))
	{
		rData.nEndRebarDesignMethod= rEbmwD.iEBarDesignMethod;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnWalKfac(int key, T_KFAC_D& rData, T_KFAC_D& aData)
{
	// Change by ZINU.('08.01.22). M1565, Because of 1.0 at Initialize(), Initialize 0.0 at this.
	//rData.Initialize();
	//aData.Initialize();
	rData.BKy = 0.0; rData.BKz = 0.0;
	aData.BKy = 0.0; aData.BKz = 0.0;

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_KFAC_D rTempData;
			T_KFAC_D aTempData;
			rTempData.Initialize();
			aTempData.Initialize();
			Get_DgnGenKfac(iKey, rTempData, aTempData);

			rData.BKy = max(rData.BKy, rTempData.BKy);
			rData.BKz = max(rData.BKz, rTempData.BKz);
			aData.BKy = max(aData.BKy, aTempData.BKy);
			aData.BKz = max(aData.BKz, aTempData.BKz);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalCmft(int key, T_CMFT_D& rData)
{
	rData.Initialize();

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		rData.bAuto = TRUE;
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_CMFT_D rTempData;
			rTempData.Initialize();
			Get_DgnGenCmft(iKey, rTempData);

			if (!rTempData.bAuto)	rData.bAuto = FALSE;
			rData.Cmy = max(rData.Cmy, rTempData.Cmy);
			rData.Cmz = max(rData.Cmz, rTempData.Cmz);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalFmag(int key, T_FMAG_D& rData)
{
	rData.Initialize();

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_FMAG_D rTempData;
			rTempData.Initialize();
			Get_DgnGenFmag(iKey, rTempData);

			rData.B1y = max(rData.B1y, rTempData.B1y);
			rData.B1z = max(rData.B1z, rTempData.B1z);
			rData.B2y = max(rData.B2y, rTempData.B2y);
			rData.B2z = max(rData.B2z, rTempData.B2z);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalSueq(int key, T_SUEQ_D& rData)
{
	rData.Initialize();

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_SUEQ_D rTempData;
			rTempData.Initialize();
			Get_DgnGenSueq(iKey, rTempData);

			rData.dEqLcaseN = max(rData.dEqLcaseN, rTempData.dEqLcaseN);
			rData.dEqLcaseM = max(rData.dEqLcaseM, rTempData.dEqLcaseM);
			rData.dEqLcaseV = max(rData.dEqLcaseV, rTempData.dEqLcaseV);
			rData.dEqLcomN = max(rData.dEqLcomN, rTempData.dEqLcomN);
			rData.dEqLcomM = max(rData.dEqLcomM, rTempData.dEqLcomM);
			rData.dEqLcomV = max(rData.dEqLcomV, rTempData.dEqLcomV);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalMbtp(int key, T_MBTP_D& rData)
{
	rData.Initialize();

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_MBTP_D rTempData;
			rTempData.Initialize();
			Get_DgnGenMbtp(iKey, rTempData);

			// Change by ZINU.('03.08.28) Always Wall.
			rData.nMbType = D_MBTP_WALL;	// Wall.
			rData.nSubType = max(rData.nSubType, rTempData.nSubType);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalRedu(int key, T_REDU_D& rData)
{
	rData.Initialize();

	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			T_REDU_D rTempData;
			rTempData.Initialize();
			Get_DgnGenRedu(iKey, rTempData);

			// Change by ZINU.('03.08.28) To Save Disadvantage Condition.
			rData.bReduAxl = (rData.Factor > rTempData.Factor ? rTempData.bReduAxl : rData.bReduAxl);
			rData.bReduMom = (rData.Factor > rTempData.Factor ? rTempData.bReduMom : rData.bReduMom);
			rData.bReduShr = (rData.Factor > rTempData.Factor ? rTempData.bReduShr : rData.bReduShr);
			rData.Factor = min(rData.Factor, rTempData.Factor);
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalGreq(int key, int& iDgnClass)
{
	// Key is Wall Key, Data is the largest value of plates(=elements).
	T_WALL_D WallD;
	WallD.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, WallD))	return FALSE;

	// Get Con.
	T_DCON_D DconD;
	DconD.Initialize();
	Get_DgnConDcon(DconD);

	const CString& strConCode = DconD.DesignCode;
	BOOL bCNConCode = (strConCode == CONCODE_GB50010_02 || strConCode == CONCODE_GB50010_10) ? TRUE : FALSE;
	if (!(DconD.bSpecialSeismic && bCNConCode))	return FALSE;

	int iCount = WallD.arElemKey.GetSize();
	for (int i = 0; i < iCount; i++)
	{
		int iKey = WallD.arElemKey.GetAt(i);
		T_GREQ_D GreqD;
		GreqD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetGreq(iKey, GreqD))
		{
			iDgnClass = GreqD.iEqGradeCon + 1;
		}
		else
		{
			iDgnClass = DconD.nClass + 1;
		}
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnWalMatd(int key, int& iMatlNo, T_MATD_D& rData)
{
	iMatlNo = 0;
	rData.Initialize();

	T_WALL_D wData;
	wData.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallData(key, wData)) { return FALSE; }

	int iCount = wData.arElemKey.GetSize();
	if (iCount > 0)
	{
		int nIter = 0;
		for (int i = 0; i < iCount; i++)
		{
			int iKey = wData.arElemKey.GetAt(i);
			// Get Element Data.
			T_ELEM_D eData;
			eData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(iKey, eData)) ASSERT(0);
			// Get Material Data.
			T_MATD_D mData;
			mData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(eData.elmat, mData))  continue;

			if (mData.Type == _T("C"))
			{
				T_MATD_D tempData;
				tempData.Initialize();
				if (nIter == 0)
				{
					rData = mData;
					tempData = mData;
					Get_DgnConMatd(rData);
					Get_DgnConMatd(tempData);
				}
				else
				{
					tempData = mData;
					Get_DgnConMatd(tempData);
				}

				if (tempData.Data1.Design.C_fc == rData.Data1.Design.C_fc)
				{
					if (tempData.MainRebarData.B_fy == rData.MainRebarData.B_fy)
					{
						if (tempData.SubRebarData.B_fy < rData.SubRebarData.B_fy)
						{
							iMatlNo = eData.elmat;
							rData.SubRebarData.B_fy = tempData.SubRebarData.B_fy;
						}
						else
						{
							iMatlNo = eData.elmat;
						}
					}
					else if (tempData.MainRebarData.B_fy < rData.MainRebarData.B_fy)
					{
						iMatlNo = eData.elmat;
						rData.MainRebarData.B_fy = tempData.MainRebarData.B_fy;
						rData.SubRebarData.B_fy = tempData.SubRebarData.B_fy;
					}
					else
					{
						iMatlNo = eData.elmat;
					}
				}
				else if (tempData.Data1.Design.C_fc < rData.Data1.Design.C_fc)
				{
					iMatlNo = eData.elmat;
					rData.Data1.Design.C_fc = tempData.Data1.Design.C_fc;
					rData.MainRebarData.B_fy = tempData.MainRebarData.B_fy;
					rData.SubRebarData.B_fy = tempData.SubRebarData.B_fy;
				}
				else
				{
					iMatlNo = eData.elmat;
				}
			}
			// Change by ZINU.('03.12.18). Only Concrete.
			else
			{
				return FALSE;
			}
			// Increse count.
			nIter++;
		}
		return TRUE;
	}
	else	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnWalWmak(int key, T_WMAK_D& rData)
{
	return m_pDoc->m_pPostCtrl->Get_DgnWalWmak(key, rData);
}

BOOL CDgnDataCtrl::Get_DgnWalRebw(int key, BOOL& bFlag, T_REBW_D& rData)
{
	rData.Initialize();

	T_REBW_K Key;
	Key.keymap = key;
	if (m_pDoc->m_pAttrCtrl->ExistRebwData(Key))
	{
		if (!m_pDoc->m_pAttrCtrl->GetRebw(Key, rData)) { ASSERT(0); }
		if (rData.bUseModelThik)
		{
			T_WALL_D wData;
			wData.Initialize();
			if (m_pDoc->m_pPostCtrl->GetWallData(key, wData))	rData.dThickness = wData.dThickness;
		}

		if (rData.nNumEndRebar == 0)
		{
			rData.nNumEndRebar = 0;
			rData.dEndRebarSpace = 0.;
			rData.EndRebarName = _T("");
		}
		bFlag = TRUE;
	}
	else
	{
		T_WALL_D wData;
		wData.Initialize();
		if (m_pDoc->m_pPostCtrl->GetWallData(key, wData))
		{
			rData.dThickness = wData.dThickness;
			bFlag = FALSE;
		}
	}

	if (rData.BEHorizonRebarName == _T(""))
	{
		T_DCRB_D DcrbD;    DcrbD.Initialize();
		Get_DgnConDcrb(0, DcrbD);
		rData.BEHorizonRebarName = DcrbD.BEHorizontalRebarWall;
	}

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnSrcDsrc(T_DSRC_D& rData)
{
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDsrc())	m_pDoc->m_pAttrCtrl->GetDsrc(rData);
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		rData.DesignCode = rPref.DgnCode.SRCCode;
		rData.nDesignMaterial = 0;
		// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (JGJ138-01).
		int iSafeLevel = 0, iEqGrade = 0;
		if (Get_DgnSrcChinaDef(rData.DesignCode, iSafeLevel, iEqGrade))
		{
			if (rData.bSpecialSeismic)	rData.nClass = iEqGrade;
			else						rData.nClass = iSafeLevel;
		}
	}

	if (IsJUD())
	{
		rData.DesignCode = SRCCODE_AIJ_SRC01;
	}

	// Add by ZINU.('04.02.04). For Scale Up Factor for Shear.
	if (!Get_DgnSrcSufsDef(rData.DesignCode, rData.nClass, rData.iAlphaType, rData.dAlpha1, rData.dAlpha2))	ASSERT(0);
	// Add by ZINU.('04.02.16). For Alpha by AlphaType.
	if (rData.iAlphaType == 2)			rData.dAlpha2 = 1.0;	// Use Only Alpha1.
	else if (rData.iAlphaType == 3)	rData.dAlpha1 = 1.0;	// Use Only Alpha2.

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnPscPscd(T_PSCD_D& rData)
{
	return m_pDoc->m_pAttrCtrl->GetDgnPscd(rData);
}

// Tendon.
BOOL CDgnDataCtrl::Get_DgnConTndn(int key, int iStepNo, CArray<_DGN_TNDN, _DGN_TNDN> aTndnList[])
{
	// iStepNo = No of Transfer Stage.
	for (int i = 0; i < 5; i++)	aTndnList[i].RemoveAll();

	// Add by ZINU.('02.12.26).
	// Get Jacking Stress at Pre-stressed Beam.
	T_JSFT_D JsftData;
	JsftData.Initialize();
	Get_DgnConJsft(key, JsftData);
	double dJackForcFactor = (JsftData.bAuto ? -1.0 : JsftData.dJackFactor);
	BOOL bUserJackForce = (dJackForcFactor >= 0.0 ? TRUE : FALSE);

	BOOL bReverseDir = FALSE;
	T_ELEM_K ShowKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	if (!m_pMembCtrl->GetElemListByIncludeElem(key, bReverseDir, ShowKey, aElemList)) { return FALSE; }

	if (aElemList.GetSize() == 1)	// Only 1.
	{
		T_ELEM_K CurrKey = key;
		double dLen = m_pDoc->calcLAVElem(CurrKey);
		for (int i = 0; i < 5; i++)	// iPosiNo.
		{
			double dPartLen = i * dLen / 4.0;
			double dPartLen1 = dPartLen + pow(cDgn_Zero, 2);
			double dPartLen2 = dPartLen - pow(cDgn_Zero, 2);
			T_TENDON_ELEMINFO TndnElemInfo;
			m_pDoc->m_pPostCtrl->GetTendonInfo()->GetTendonElemInfo(CurrKey, TndnElemInfo);
			int iTndnNum = TndnElemInfo.aTendonElemInfoTendon.GetSize();
			for (int j = 0; j < iTndnNum; j++)
			{
				T_TENDON_ELEMINFO_TENDON TndnData = TndnElemInfo.aTendonElemInfoTendon.GetAt(j);
				// Add by ZINU.('05.09.14). Check if Prestressed Tendon.
				if (!TndnData.bPrestressed)	continue;
				int iTndnBaseNum = TndnData.aTendonElemInfoTendonBase.GetSize();
				// Change by ZINU.('02.12.26). User or Program.
				// Get Tendon Force.
				CArray<double, double> aTndnForceTran, aTndnForceServ;
				aTndnForceTran.RemoveAll();
				aTndnForceServ.RemoveAll();

				if (bUserJackForce)	// by User.
				{
					for (int m = 0; m < iTndnBaseNum; m++)
					{
						aTndnForceTran.Add(dJackForcFactor);	// Stress Factor at Transfer Stage.
						aTndnForceServ.Add(dJackForcFactor);	// Stress Factor at Service Load Stage.
					}
				}
				else	// by Program.
				{
					// Construction Stage Analysis at Post Mode.
					if (!(m_pDoc->m_pPostCtrl->StageAnalysis() && iStepNo > 0))	continue;
					int iStepNoTran = iStepNo;
					int iStepNoServ = m_pDoc->m_pPostCtrl->GetMaxStep_SG();
					if (!m_pDoc->m_pPostCtrl->GetTdfc(TndnData.KeyTdna, CurrKey, iStepNoTran, aTndnForceTran, TRUE))	continue;	// Force at Transfer Stage.
					if (!m_pDoc->m_pPostCtrl->GetTdfc(TndnData.KeyTdna, CurrKey, iStepNoServ, aTndnForceServ, TRUE))	continue;	// Force at Service Load Stage.
					if (iTndnBaseNum != aTndnForceTran.GetSize())	continue;
					if (iTndnBaseNum != aTndnForceServ.GetSize())	continue;
				}
				// Change by ZINU.('04.06.14). For Reverse Direction about Element Direction.
				BOOL bReverseDir = FALSE;
				if (iTndnBaseNum >= 2)
				{
					T_TENDON_ELEMINFO_TENDON_BASE TndnBase0 = TndnData.aTendonElemInfoTendonBase.GetAt(0);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBase1 = TndnData.aTendonElemInfoTendonBase.GetAt(1);
					bReverseDir = (TndnBase0.dPositionRatio_x > TndnBase1.dPositionRatio_x ? TRUE : FALSE);
				}
				int k = 0;
				for ((bReverseDir ? k = iTndnBaseNum - 2 : k = 0); (bReverseDir ? k > -1:k < iTndnBaseNum - 1); (bReverseDir ? k-- : k++))
				{
					// Prev.
					int iPrev = (bReverseDir ? k + 1 : k + 0);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBasePrev = TndnData.aTendonElemInfoTendonBase.GetAt(iPrev);
					double dPrevL = dLen * TndnBasePrev.dPositionRatio_x;
					double dPrevY = TndnBasePrev.dDistance_y;
					double dPrevZ = TndnBasePrev.dDistance_z;
					double dPrevFT = aTndnForceTran[iPrev];	// Tendon Force at Transfer Stage.
					double dPrevFS = aTndnForceServ[iPrev];	// Tendon Force at Service Load Stage.
					// Next.
					int iNext = (bReverseDir ? k + 0 : k + 1);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBaseNext = TndnData.aTendonElemInfoTendonBase.GetAt(iNext);
					double dNextL = dLen * TndnBaseNext.dPositionRatio_x;
					double dNextY = TndnBaseNext.dDistance_y;
					double dNextZ = TndnBaseNext.dDistance_z;
					double dNextFT = aTndnForceTran[iNext];	// Tendon Force at Transfer Stage.
					double dNextFS = aTndnForceServ[iNext];	// Tendon Force at Service Load Stage.
					// Compare Location by Length.
					if (dPrevL <= dPartLen1 && dNextL >= dPartLen2)
					{
						_DGN_TNDN DgnTndn;
						double dLenx = (fabs(dNextL - dPrevL) < pow(cDgn_Zero, 2) ? pow(cDgn_Zero, 2) : dNextL - dPrevL);
						double dRatL = (dPartLen - dPrevL) / dLenx;
						// Set Positions, Jacking Force, Rotate.
						DgnTndn.dyz[0] = dPrevY + dRatL * (dNextY - dPrevY);
						DgnTndn.dyz[1] = dPrevZ + dRatL * (dNextZ - dPrevZ);
						DgnTndn.dJackForceTran = dPrevFT + dRatL * (dNextFT - dPrevFT);
						DgnTndn.dJackForceServ = dPrevFS + dRatL * (dNextFS - dPrevFS);
						DgnTndn.dRotRad = (dLenx < cDgn_Zero ? 0.0 : atan((dPrevZ - dNextZ) / dLenx));
						// Set Tendon Type.
						T_TDNA_D TdnaData;
						TdnaData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetTdna(TndnData.KeyTdna, TdnaData))	ASSERT(0);
						double dDiaTndn4 = 0.0, dDiaDuct4 = 0.0;
						T_TDNT_D TdntData;  // Tendon Type.
						TdntData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaData.TendonTypeKey, TndnData.dTendonNum, TdntData, dDiaTndn4, dDiaDuct4))	ASSERT(0);
						DgnTndn.iTdnaKey = TndnData.KeyTdna;
						DgnTndn.iTndnType = TdntData.nTendonType;
						DgnTndn.iBondType = TdntData.nBondType;
						DgnTndn.iLoadType = TdntData.nLoadType;
						DgnTndn.dfpu = TdntData.dUltimateStrength;
						DgnTndn.dfpy = TdntData.dYieldStrength;
						DgnTndn.dAps = TdntData.dTendonArea;
						DgnTndn.dDia = sqrt(4 * DgnTndn.dAps / (4.0*atan(1.0)));
						DgnTndn.dJackForceTran *= (bUserJackForce ? DgnTndn.dfpu*DgnTndn.dAps : 1.0);
						DgnTndn.dJackForceServ *= (bUserJackForce ? DgnTndn.dfpu*DgnTndn.dAps : 1.0);
						// Set Tendon Elastic Coefficient.
						T_MATL_D MatlData;
						MatlData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetMatl(TdntData.MatlKey, MatlData))	ASSERT(0);
						DgnTndn.dEps = MatlData.Data1.Analysis.Elast;
						// Save Tendon Data.
						aTndnList[i].Add(DgnTndn);
						break;
					}
				}	// Tndn Base.
			}	// Tndn Elem.
		}	// iPosiNo.
	}
	else	// Over 1.
	{
		double dLen = m_pMembCtrl->GetMembLengByIncludeElem(ShowKey);	// Total Length.
		for (int i = 0; i < 5; i++)	// iPosiNo.
		{
			T_ELEM_K CurrKey = 0;
			int iElemCount = 0;
			double dElemLen = 0.0, dSumLen = 0.0;
			double dPartLen = 0.0, dPartLen1 = 0.0, dPartLen2 = 0.0;
			if (!m_pMembCtrl->GetMembPosiData(i, aElemList, dLen,
				CurrKey, iElemCount, dElemLen, dSumLen, dPartLen, dPartLen1, dPartLen2))	ASSERT(0);

			T_TENDON_ELEMINFO TndnElemInfo;
			m_pDoc->m_pPostCtrl->GetTendonInfo()->GetTendonElemInfo(CurrKey, TndnElemInfo);
			int iTndnNum = TndnElemInfo.aTendonElemInfoTendon.GetSize();
			for (int j = 0; j < iTndnNum; j++)
			{
				T_TENDON_ELEMINFO_TENDON TndnData = TndnElemInfo.aTendonElemInfoTendon.GetAt(j);
				// Add by ZINU.('05.09.14). Check if Prestressed Tendon.
				if (!TndnData.bPrestressed)	continue;
				int iTndnBaseNum = TndnData.aTendonElemInfoTendonBase.GetSize();
				// Change by ZINU.('02.12.26). User or Program.
				// Get Tendon Force.
				CArray<double, double> aTndnForceTran, aTndnForceServ;
				aTndnForceTran.RemoveAll();
				aTndnForceServ.RemoveAll();

				if (bUserJackForce)	// by User.
				{
					for (int m = 0; m < iTndnBaseNum; m++)
					{
						aTndnForceTran.Add(dJackForcFactor);	// Stress Factor at Transfer Stage.
						aTndnForceServ.Add(dJackForcFactor);	// Stress Factor at Service Load Stage.
					}
				}
				else	// by Program.
				{
					// Construction Stage Analysis at Post Mode.
					if (!(m_pDoc->m_pPostCtrl->StageAnalysis() && iStepNo > 0))	continue;
					int iStepNoTran = iStepNo;
					int iStepNoServ = m_pDoc->m_pPostCtrl->GetMaxStep_SG();
					if (!m_pDoc->m_pPostCtrl->GetTdfc(TndnData.KeyTdna, CurrKey, iStepNoTran, aTndnForceTran, TRUE))	continue;	// Force at Transfer Stage.
					if (!m_pDoc->m_pPostCtrl->GetTdfc(TndnData.KeyTdna, CurrKey, iStepNoServ, aTndnForceServ, TRUE))	continue;	// Force at Service Load Stage.
					if (iTndnBaseNum != aTndnForceTran.GetSize())	continue;
					if (iTndnBaseNum != aTndnForceServ.GetSize())	continue;
				}

				// Change by ZINU.('04.06.14). For Reverse Direction about Element Direction.
				BOOL bReverseDir = FALSE;
				if (iTndnBaseNum >= 2)
				{
					T_TENDON_ELEMINFO_TENDON_BASE TndnBase0 = TndnData.aTendonElemInfoTendonBase.GetAt(0);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBase1 = TndnData.aTendonElemInfoTendonBase.GetAt(1);
					bReverseDir = (TndnBase0.dPositionRatio_x > TndnBase1.dPositionRatio_x ? TRUE : FALSE);
				}
				int k = 0;
				for ((bReverseDir ? k = iTndnBaseNum - 2 : k = 0); (bReverseDir ? k > -1:k < iTndnBaseNum - 1); (bReverseDir ? k-- : k++))
				{
					// Prev.
					int iPrev = (bReverseDir ? k + 1 : k + 0);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBasePrev = TndnData.aTendonElemInfoTendonBase.GetAt(iPrev);
					double dPrevL = dSumLen + dElemLen * TndnBasePrev.dPositionRatio_x;
					double dPrevY = TndnBasePrev.dDistance_y;
					double dPrevZ = TndnBasePrev.dDistance_z;
					double dPrevFT = aTndnForceTran[iPrev];	// Tendon Force at Transfer Stage.
					double dPrevFS = aTndnForceServ[iPrev];	// Tendon Force at Service Load Stage.
					// Next.
					int iNext = (bReverseDir ? k + 0 : k + 1);
					T_TENDON_ELEMINFO_TENDON_BASE TndnBaseNext = TndnData.aTendonElemInfoTendonBase.GetAt(iNext);
					double dNextL = dSumLen + dElemLen * TndnBaseNext.dPositionRatio_x;
					double dNextY = TndnBaseNext.dDistance_y;
					double dNextZ = TndnBaseNext.dDistance_z;
					double dNextFT = aTndnForceTran[iNext];	// Tendon Force at Transfer Stage.
					double dNextFS = aTndnForceServ[iNext];	// Tendon Force at Service Load Stage.
					// Compare Location by Length.
					if (dPrevL <= dPartLen1 && dNextL >= dPartLen2)
					{
						_DGN_TNDN DgnTndn;
						double dLenx = (fabs(dNextL - dPrevL) < pow(cDgn_Zero, 2) ? pow(cDgn_Zero, 2) : dNextL - dPrevL);
						double dRatL = (dPartLen - dPrevL) / dLenx;
						// Set Positions, Jacking Force, Rotate.
						DgnTndn.dyz[0] = dPrevY + dRatL * (dNextY - dPrevY);
						DgnTndn.dyz[1] = dPrevZ + dRatL * (dNextZ - dPrevZ);
						DgnTndn.dJackForceTran = dPrevFT + dRatL * (dNextFT - dPrevFT);
						DgnTndn.dJackForceServ = dPrevFS + dRatL * (dNextFS - dPrevFS);
						DgnTndn.dRotRad = (dLenx < cDgn_Zero ? 0.0 : atan((dPrevZ - dNextZ) / dLenx));
						// Set Tendon Type.
						T_TDNA_D TdnaData;
						TdnaData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetTdna(TndnData.KeyTdna, TdnaData))	ASSERT(0);
						double dDiaTndn4 = 0.0, dDiaDuct4 = 0.0;
						T_TDNT_D TdntData;  // Tendon Type.
						TdntData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaData.TendonTypeKey, TndnData.dTendonNum, TdntData, dDiaTndn4, dDiaDuct4))	ASSERT(0);
						DgnTndn.iTdnaKey = TndnData.KeyTdna;
						DgnTndn.iTndnType = TdntData.nTendonType;
						DgnTndn.iBondType = TdntData.nBondType;
						DgnTndn.iLoadType = TdntData.nLoadType;
						DgnTndn.dfpu = TdntData.dUltimateStrength;
						DgnTndn.dfpy = TdntData.dYieldStrength;
						DgnTndn.dAps = TdntData.dTendonArea;
						DgnTndn.dDia = sqrt(4 * DgnTndn.dAps / (4.0*atan(1.0)));
						DgnTndn.dJackForceTran *= (bUserJackForce ? DgnTndn.dfpu*DgnTndn.dAps : 1.0);
						DgnTndn.dJackForceServ *= (bUserJackForce ? DgnTndn.dfpu*DgnTndn.dAps : 1.0);
						// Set Tendon Elastic Coefficient.
						T_MATL_D MatlData;
						MatlData.Initialize();
						if (!m_pDoc->m_pAttrCtrl->GetMatl(TdntData.MatlKey, MatlData))	ASSERT(0);
						DgnTndn.dEps = MatlData.Data1.Analysis.Elast;
						// Set Tendon Data.
						aTndnList[i].Add(DgnTndn);
						break;
					}
				}	// Tndn Base.
			}	// Tndn Elem.
		}	// iPosiNo.
	}
	BOOL bGetOK = FALSE;
	for (int m = 0; m < 5; m++)
	{
		if (!bGetOK && aTndnList[m].GetSize() > 0)	bGetOK = TRUE;
	}
	return bGetOK;
}

BOOL CDgnDataCtrl::Get_DgnConPsas(int key, T_PSAS_D& rData)
{
	// Key is Elem.
	rData.Initialize();

	T_ELEM_K Showkey;
	T_HCBM_K HcbmKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey))
	{
		if (!m_pMembCtrl->GetShowElemByIncludeHcbm(key, Showkey)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }
	}

	if (m_pDoc->m_pAttrCtrl->ExistPsas(Showkey))
	{
		if (!m_pDoc->m_pAttrCtrl->GetPsas(Showkey, rData)) { return FALSE; }
	}
	else
	{
		// Change by ZINU.('05.05.04). Set by KCI-USD99 -> by Code.
		T_DCON_D DconD; DconD.Initialize();
		Get_DgnConDcon(DconD);
		double dFtsc = 0.0, dFtst_s = 0.0, dFtst_m = 0.0, dFssc_s = 0.0, dFssc_t = 0.0, dFsst = 0.0;
		if (!Get_DgnConPsasDef(DconD.DesignCode, dFtsc, dFtst_s, dFtst_m, dFssc_s, dFssc_t, dFsst)) { return FALSE; }
		rData.dTranComp = dFtsc;
		rData.dTranSupT = dFtst_s;
		rData.dTranMidT = dFtst_m;
		rData.dServSusC = dFssc_s;
		rData.dServTotC = dFssc_t;
		rData.dServTens = dFsst;
	}
	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnConJsft(int key, T_JSFT_D& rData)
{
	rData.Initialize();
	////////////////////////////////////////////////
	//	rData.bAuto				:	Option for auto-calculation.
	//	rData.dJackFactor : Can auto-calculation.
	////////////////////////////////////////////////

	T_ELEM_K Showkey;
	T_HCBM_K HcbmKey;
	if (m_pDoc->m_pAttrCtrl->GetHcbmAssigned(key, HcbmKey))
	{
		if (!m_pMembCtrl->GetShowElemByIncludeHcbm(key, Showkey)) { return FALSE; }
	}
	else
	{
		if (!m_pMembCtrl->GetShowElemByIncludeElem(key, Showkey)) { return FALSE; }
	}

	if (m_pDoc->m_pAttrCtrl->ExistJsft(Showkey))
	{
		m_pDoc->m_pAttrCtrl->GetJsft(Showkey, rData);
		if (rData.bAuto)	rData.dJackFactor = 0.0;
	}
	else
	{
		rData.bAuto = FALSE;
		rData.dJackFactor = 0.7;
	}
	return TRUE;
}

// For Display Option. Auto-calculation Data.
BOOL CDgnDataCtrl::Exist_DgnGenKfacAuto()
{
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	return (pDgnDynData->m_arKfacData.GetCount() > 0) ? TRUE : FALSE;
}

BOOL CDgnDataCtrl::Exist_DgnGenReduAuto()
{
	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	return (pDgnDynData->m_arReduData.GetCount() > 0) ? TRUE : FALSE;
}

BOOL CDgnDataCtrl::Get_DgnGenKfacAuto(CMap<T_ELEM_K, T_ELEM_K, T_KFAC_D, T_KFAC_D>& arKfacDataAuto)
{
	arKfacDataAuto.RemoveAll();

	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	T_KFAC_D KfacD;
	stKFactor Factor;
	T_ELEM_K Key = 0;
	POSITION Pos = pDgnDynData->m_arKfacData.GetStartPosition();
	while (Pos)
	{
		KfacD.Initialize();
		pDgnDynData->m_arKfacData.GetNextAssoc(Pos, Key, Factor);
		if (Factor.KFact_StrongAxis <= 0.0)	continue;
		if (Factor.KFact_WeakAxis <= 0.0)	continue;
		KfacD.BKy = Factor.KFact_StrongAxis;
		KfacD.BKz = Factor.KFact_WeakAxis;
		arKfacDataAuto.SetAt(Key, KfacD);
	}
	return (arKfacDataAuto.GetCount() > 0) ? TRUE : FALSE;
}

BOOL CDgnDataCtrl::Get_DgnGenReduAuto(CMap<T_ELEM_K, T_ELEM_K, T_REDU_D, T_REDU_D>& arReduDataAuto)
{
	arReduDataAuto.RemoveAll();

	CDgnDynData* pDgnDynData = CDgnDynData::GetDgnDynData();
	T_REDU_D ReduD;
	double dFactor = 0.0;
	T_ELEM_K Key = 0;
	POSITION Pos = pDgnDynData->m_arReduData.GetStartPosition();
	while (Pos)
	{
		ReduD.Initialize();
		pDgnDynData->m_arReduData.GetNextAssoc(Pos, Key, dFactor);
		if (dFactor <= 0.0)	continue;
		int iReduKind = 0;
		pDgnDynData->m_arReduKind.Lookup(Key, iReduKind);
		ReduD.bReduAxl = ((iReduKind / 100) % 10 == 1 ? TRUE : FALSE);
		ReduD.bReduMom = ((iReduKind / 10) % 10 == 1 ? TRUE : FALSE);
		ReduD.bReduShr = ((iReduKind / 1) % 10 == 1 ? TRUE : FALSE);
		ReduD.Factor = dFactor;
		// Set Auto-calculation Live Load Reduction Factor.
		arReduDataAuto.SetAt(Key, ReduD);
	}
	return (arReduDataAuto.GetCount() > 0) ? TRUE : FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConDcrbDef(T_DCRB_D& rData)
{
	/////////////////////////////////////////////////////////////////
	// Beam.
	// Main Rebar.
	if (rData.MainRebarBeam[0] == _T("")) { CDBLib::GetDefaultMainRebarSize(1, rData.MainRebarBeam[0]); }
	// Stirrup Rebar.
	if (rData.StirrupBarBeam == _T("")) { CDBLib::GetDefaultSubRebarSize(1, rData.StirrupBarBeam); }
	if (rData.iSubRebarNum_Beam == 0) { CDBLib::GetDefaultSubRebarNum(rData.iSubRebarNum_Beam); }
	// Side Rebar.
	if (rData.SideBarBeam == _T("")) { CDBLib::GetDefaultSideRebarSize(rData.SideBarBeam); }
	/////////////////////////////////////////////////////////////////
	// Column.
	// Main Rebar.
	if (rData.MainRebarColumn[0] == _T("")) { CDBLib::GetDefaultMainRebarSize(2, rData.MainRebarColumn[0]); }
	// Sub Rebar.
	if (rData.iSubRebarNum_Column[0] == 0) { CDBLib::GetDefaultSubRebarNum(rData.iSubRebarNum_Column[0]); }
	if (rData.iSubRebarNum_Column[1] == 0) { CDBLib::GetDefaultSubRebarNum(rData.iSubRebarNum_Column[1]); }
	if (rData.StirrupBarColumn == _T("")) { CDBLib::GetDefaultSubRebarSize(2, rData.StirrupBarColumn); }
	/////////////////////////////////////////////////////////////////
	// Brace.
	// Main Rebar.
	if (rData.MainRebarBrace[0] == _T("")) { CDBLib::GetDefaultMainRebarSize(3, rData.MainRebarBrace[0]); }
	// Sub Rebar.
	if (rData.iSubRebarNum_Brace[0] == 0) { CDBLib::GetDefaultSubRebarNum(rData.iSubRebarNum_Brace[0]); }
	if (rData.iSubRebarNum_Brace[1] == 0) { CDBLib::GetDefaultSubRebarNum(rData.iSubRebarNum_Brace[1]); }
	if (rData.StirrupBarBrace == _T("")) { CDBLib::GetDefaultSubRebarSize(3, rData.StirrupBarBrace); }
	/////////////////////////////////////////////////////////////////
	// Wall.
	// Vertical Rebar.
	if (rData.VerticalRebarWall[0] == _T("")) { CDBLib::GetDefaultMainRebarSize(4, rData.VerticalRebarWall[0]); }
	// Horizontal Rebar.
	if (rData.HorizontalRebarWall == _T("")) { CDBLib::GetDefaultSubRebarSize(4, rData.HorizontalRebarWall); }
	// End Rebar.
	if (rData.EndRebarWall == _T("")) { CDBLib::GetDefaultSubRebarSize(4, rData.EndRebarWall); }
	// BE Horizontal Rebar.
	if (rData.BEHorizontalRebarWall == _T("")) { CDBLib::GetDefaultSubRebarSize(4, rData.BEHorizontalRebarWall); }

	/////////////////////////////////////////////////////////////////
	double dEndDefaultSpace[3] = { 0.0, };
	CDBLib::GetDefaultEndRebarSpace(dEndDefaultSpace);
	if (rData.dEndRebarSpace[0] == 0.0) { rData.dEndRebarSpace[0] = dEndDefaultSpace[0]; }
	if (rData.dEndRebarSpace[1] == 0.0) { rData.dEndRebarSpace[1] = dEndDefaultSpace[1]; }
	if (rData.dEndRebarSpace[2] == 0.0) { rData.dEndRebarSpace[2] = dEndDefaultSpace[2]; }

	if (rData.nEndRebarDesignMethod == 0) { rData.nEndRebarDesignMethod = 1; }
	if (rData.bOutofPlaneBending == FALSE) { rData.bOutofPlaneBending = FALSE; }
	if (rData.dSpacingHorizontalRebar == 0.0) { rData.dSpacingHorizontalRebar = dEndDefaultSpace[2] / 2.0; }
	if (rData.SpacingVerticalRebar[0] == _T(""))
	{
		int iRebarNum = 0;
		CString strRebarSpace[CONST_DGN_iWALL_VSPACE] = { _T(""), };
		CDBLib::GetDefaultWallRebarSpace(iRebarNum, strRebarSpace);
		for (int i = 0; i < CONST_DGN_iWALL_VSPACE; i++)
		{
			rData.SpacingVerticalRebar[i] = strRebarSpace[i];
		}
	}

	// Add by ZINU.('05.08.22). Default Splice Option.
	T_DCON_D DconD;
	DconD.Initialize();
	if (!Get_DgnConDcon(DconD))	ASSERT(0);
	if (DconD.DesignCode == CONCODE_AIJ_WSD99)
	{
		rData.bSpliceBeamJP = TRUE;
		rData.bSpliceColumnJP = TRUE;
		rData.bSpliceBraceJP = TRUE;
		rData.iSpliceBeam = 0;
		rData.iSpliceColumn = 0;
		rData.iSpliceBrace = 0;
	}
	else
	{
		rData.bSpliceBeamJP = FALSE;
		rData.bSpliceColumnJP = FALSE;
		rData.bSpliceBraceJP = FALSE;
		// Coded by SeungJun MNET:3433  '20080528 ���ֹ��������� ���? �������? �� ���� Default�� ó��
#if defined(_US) || defined(_RUS)
		rData.iSpliceBeam = 0;
		rData.iSpliceColumn = 0;
		rData.iSpliceBrace = 0;
#else
		rData.iSpliceBeam = 1;
		rData.iSpliceColumn = 1;
		rData.iSpliceBrace = 1;
#endif
	}

	rData.bVerEndBar_Wall = FALSE;
	if (rData.strVerEndBar_Wall == _T("")) { CDBLib::GetDefaultVerEndWallRebarSize(rData.strVerEndBar_Wall); }
	if (rData.strVerEndBarFy_Wall == _T("")) { rData.strVerEndBarFy_Wall = _T("SD500"); }

	if (rData.dBEHorizontalRebarSpace == 0.0)
	{
		T_UNIT_INDEX CurIndex;
		CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

		double dLenScale = 0.0;
		switch (CurIndex.nBase_Length)
		{
		case D_UNITSYS_LENGTH_INDEX_MM: { dLenScale = 100.0; } break; // 100 mm. 
		case D_UNITSYS_LENGTH_INDEX_CM: { dLenScale = 10.0; }  break; // 10 cm.
		case D_UNITSYS_LENGTH_INDEX_M: { dLenScale = 0.1; } break; // 0.1 m.
		case D_UNITSYS_LENGTH_INDEX_IN: { dLenScale = 4.0; } break; // 4 in.
		case D_UNITSYS_LENGTH_INDEX_FT: { dLenScale = 4.0 / 12.0; } break; // 1/3 ft.
		default: ASSERT(0); break;
		}

		rData.dBEHorizontalRebarSpace = 2.0*dLenScale;
	}

	if(rData.dBEVerticalRebarSpace == 0.0)
	{
		T_UNIT_INDEX CurIndex;
		CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

		double dLenScale = 0.0;
		switch(CurIndex.nBase_Length)
		{
		case D_UNITSYS_LENGTH_INDEX_MM: { dLenScale = 100.0; } break; // 100 mm. 
		case D_UNITSYS_LENGTH_INDEX_CM: { dLenScale = 10.0; }  break; // 10 cm.
		case D_UNITSYS_LENGTH_INDEX_M: { dLenScale = 0.1; } break; // 0.1 m.
		case D_UNITSYS_LENGTH_INDEX_IN: { dLenScale = 4.0; } break; // 4 in.
		case D_UNITSYS_LENGTH_INDEX_FT: { dLenScale = 4.0 / 12.0; } break; // 1/3 ft.
		default: ASSERT(0); break;
		}

		rData.dBEVerticalRebarSpace = dLenScale;
	}

	return TRUE;
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05(int nNationalAnnex, int nExposure, int nLife, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	switch (nNationalAnnex)
	{
	case enNationalAnnex::Recommended:
	case enNationalAnnex::Belgium:
	case enNationalAnnex::UnitedKingdom:
	case enNationalAnnex::Cyprus:
	case enNationalAnnex::Denmark:
	case enNationalAnnex::France:
	case enNationalAnnex::Germany:
	case enNationalAnnex::Greece:
	case enNationalAnnex::Italy:
	case enNationalAnnex::Malaysia:
	case enNationalAnnex::Norway:
	case enNationalAnnex::Poland:
	case enNationalAnnex::Romania:
	case enNationalAnnex::Singapore:
	case enNationalAnnex::Slovenia:
		{
			Get_DgnConCrackDef_EC2_05_NA_REC(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;
	case enNationalAnnex::Austria:
		{
			Get_DgnConCrackDef_EC2_05_NA_AUS(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;	
	case enNationalAnnex::CzechRepublic:
		{
			Get_DgnConCrackDef_EC2_05_NA_CZE(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;
	case enNationalAnnex::Netherlands:
		{
			Get_DgnConCrackDef_EC2_05_NA_NLD(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;
	case enNationalAnnex::Finland:
		{
			Get_DgnConCrackDef_EC2_05_NA_FIN(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;
	case enNationalAnnex::Ireland:
		{
			Get_DgnConCrackDef_EC2_05_NA_IRL(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;
	case enNationalAnnex::Luxembourg:
		{
			Get_DgnConCrackDef_EC2_05_NA_LUX(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;	
	case enNationalAnnex::Slovakia:
		{
			Get_DgnConCrackDef_EC2_05_NA_SVK(nExposure, rdWmaxQ, rdWmaxF);
		}
		break;	
	case enNationalAnnex::Spain:
		{
			Get_DgnConCrackDef_EC2_05_NA_ESP(nExposure, rdWmaxQ, rdWmaxF);			
		}
		break;
	case enNationalAnnex::Sweden:
		{
			Get_DgnConCrackDef_EC2_05_NA_SWE(nExposure, nLife, rdWmaxQ, rdWmaxF);
		}
		break;
	default:
		ASSERT(0);
		break;
	}	
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_REC(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{	
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:		
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:		
	case EN_XD1:
	case EN_XD2:
	case EN_XD3:
	case EN_XS1:
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.3;
		}
		break;
	default:		
		break;
	}	
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_AUS(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
	case EN_XD1:
	case EN_XS1:
	case EN_XD2:
	case EN_XS2:
		{
			rdWmaxQ = 0.3;
		}
		break;	
	case EN_XD3:
	case EN_XS3:
		{
			rdWmaxF = 0.3;
		}
		break;
	default:
		break;
	}
}
	
void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_CZE(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
		{
			rdWmaxQ = 0.4;
		}
		break;
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
	case EN_XD1:
	case EN_XS1:	
		{
			rdWmaxQ = 0.3;
		}
		break;
	case EN_XD2:
	case EN_XD3:
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.2;
		}
		break;
	default:
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_NLD(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
		{
			rdWmaxQ = 0.4;
		}
		break;
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
		{
			rdWmaxQ = 0.3;
		}
		break;
	case EN_XD1:		
	case EN_XD2:
	case EN_XD3:
	case EN_XS1:
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.2;
		}
		break;
	default:
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_FIN(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
		{
			rdWmaxQ = 0.3;
		}
		break;
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
	case EN_XD1:
	case EN_XS1:
		{
			rdWmaxQ = 0.2;
		}
		break;	
	case EN_XD2:
	case EN_XD3:	
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.1;
		}
		break;
	default:
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_IRL(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:		
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
		{
			rdWmaxQ = 0.25;
		}
		break;
	case EN_XD1:		
	case EN_XD2:
	case EN_XD3:
	case EN_XS1:
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.1;
		}
		break;
	default:
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_LUX(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = 0.2; /// assumed other structures.
	rdWmaxF = 0.0;	
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_SVK(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:		
	case EN_XD1:
	case EN_XD2:
	case EN_XD3:
	case EN_XS1:
	case EN_XS2:
	case EN_XS3:
		{
			rdWmaxQ = 0.3;
		}
		break;
	default:
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_ESP(int nExposure, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
		{
			rdWmaxQ = 0.4;
		}
		break;
	case EN_XC1:
	case EN_XC2:
	case EN_XC3:
	case EN_XC4:
	case EN_XF1:
	case EN_XF3:
		{
			rdWmaxQ = 0.3;
		}
		break;
	case EN_XS1:
	case EN_XS2:
	case EN_XD1:
	case EN_XD2:
	case EN_XD3:
	case EN_XF2:
	case EN_XF4:
	case EN_XA1:
		{
			rdWmaxQ = 0.2;
		}
		break;
	case EN_XS3:
	case EN_XA2:
	case EN_XA3:
		{
			rdWmaxQ = 0.1;
		}
		break;
	default:		
		break;
	}
}

void CDgnDataCtrl::Get_DgnConCrackDef_EC2_05_NA_SWE(int nExposure, int nLife, OUT double& rdWmaxQ, OUT double& rdWmaxF)
{	
	rdWmaxQ = rdWmaxF = 0.0;
	switch (nExposure)
	{
	case EN_X0:
	case EN_XC1:
		{			
			rdWmaxQ = 0.45;
		}
		break;
	case EN_XC2:
		{
			double adWmaxQ[3] = { 0.4, 0.45, 0.0 };
			rdWmaxQ = adWmaxQ[nLife];
		}
		break;
	case EN_XC3:
	case EN_XC4:
		{
			double adWmaxQ[3] = { 0.3, 0.4, 0.0 };
			rdWmaxQ = adWmaxQ[nLife];
		}
		break;
	case EN_XS1:
	case EN_XS2:
	case EN_XD1:
	case EN_XD2:
		{
			double adWmaxQ[3] = { 0.2, 0.3, 0.4 };
			rdWmaxQ = adWmaxQ[nLife];
		}
		break;
	case EN_XS3:
	case EN_XD3:
		{
			double adWmaxQ[3] = { 0.15, 0.2, 0.3 };
			rdWmaxQ = adWmaxQ[nLife];
		}
		break;
	default:
		break;
	}
}

BOOL CDgnDataCtrl::Get_DgnCfsPhiDef(CString strCode, double* pdPhi, int nNatioalAnnex)
{
	for (int i = 0; i < 5; i++)
	{
		pdPhi[i] = 1.0;
	}
	// Gen.
	if (strCode == CFSCODE_AIK_CFSD98) { return TRUE; }
	if (strCode == CFSCODE_AISI_CFSD86) { return TRUE; }
	if (strCode == CFSCODE_AISI_CFSD08) { return TRUE; }
	if (strCode == CFSCODE_GB50018_02 || strCode == CFSCODE_GB50018_25) { return TRUE; }
	if (strCode == CFSCODE_EC3_06 && nNatioalAnnex == 0) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.25; return TRUE; }
	if (strCode == CFSCODE_EC3_06 && nNatioalAnnex == 1) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.25; return TRUE; } // MQC:13577, Cold Form Factor.
	if (strCode == CFSCODE_EC3_06 && nNatioalAnnex == 2) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.25; return TRUE; }
	if (strCode == CFSCODE_EC3_06)
	{
		// EN 1993-1-1:2005 6.1 (1) Partial factors ��Mi
		switch (nNatioalAnnex)
		{
			case dgn::def::enNationalAnnex::Denmark:		{ pdPhi[0] = 1.10; pdPhi[1] = 1.20; pdPhi[2] = 1.35; return TRUE; }
			case dgn::def::enNationalAnnex::Germany:		{ pdPhi[0] = 1.10; pdPhi[1] = 1.10; pdPhi[2] = 1.25; return TRUE; }
			case dgn::def::enNationalAnnex::Italy:			
			case dgn::def::enNationalAnnex::Norway:			{ pdPhi[0] = 1.05; pdPhi[1] = 1.05; pdPhi[2] = 1.25; return TRUE; }
			case dgn::def::enNationalAnnex::Romania:
			case dgn::def::enNationalAnnex::Slovakia:		{ pdPhi[0] = 1.00; pdPhi[1] = 1.10; pdPhi[2] = 1.25; return TRUE; }
			case dgn::def::enNationalAnnex::Sweden:			
			case dgn::def::enNationalAnnex::Sweden19:		{ pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.20; return TRUE; }
			default:										{ pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.25; return TRUE; } // Recommended
		}
	}

	ASSERT(0);
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnStlPhiDef(CString strCode, double* pdPhi, int nNatioalAnnex)
{
	for (int i = 0; i < 5; i++)
	{
		pdPhi[i] = 1.0;
	}
	// Gen.
	if (strCode == STLCODE_AISC_LRFD93) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.85; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_AISC_LRFD2K) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.85; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_AIK_LSD97) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.85; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_TWN_LSD90) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.85; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_TWN_LSD96) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.85; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; } // Add by GAY. PMS:4064. ('09.09.17)
	if (strCode == STLCODE_EC3) { pdPhi[0] = 1.10; pdPhi[1] = 1.10; pdPhi[2] = 1.25; return TRUE; }
	if (strCode == STLCODE_EC3_05)
	{
		// EN 1993-1-1:2005 6.1 (1) Partial factors ��Mi
		switch (nNatioalAnnex)
		{
			case dgn::def::enNationalAnnex::Denmark:		{ pdPhi[0] = 1.10; pdPhi[1] = 1.20; pdPhi[2] = 1.35; return TRUE; }
			case dgn::def::enNationalAnnex::Germany:		{ pdPhi[0] = 1.00; pdPhi[1] = 1.10; pdPhi[2] = 1.25; return TRUE; }
			case dgn::def::enNationalAnnex::Italy:			
			case dgn::def::enNationalAnnex::Norway:			
			case dgn::def::enNationalAnnex::Spain:			{ pdPhi[0] = 1.05; pdPhi[1] = 1.05; pdPhi[2] = 1.25; return TRUE; }
			case dgn::def::enNationalAnnex::Malaysia:		{ pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.20; return TRUE; }
			case dgn::def::enNationalAnnex::Poland:			
			case dgn::def::enNationalAnnex::Singapore:		
			case dgn::def::enNationalAnnex::UnitedKingdom:
			case dgn::def::enNationalAnnex::Sweden:			
			case dgn::def::enNationalAnnex::Sweden19:		{ pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.10; return TRUE; }
			default:										{ pdPhi[0] = 1.00; pdPhi[1] = 1.00; pdPhi[2] = 1.25; return TRUE; } // Recommended
		}
	}
	if (strCode == STLCODE_BS5950_2K) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; return TRUE; } // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060623)
	if (strCode == STLCODE_BS5950_90) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; return TRUE; }
	if (strCode == STLCODE_CSA_S16_01) { pdPhi[0] = 0.90; return TRUE; }
	if (strCode == STLCODE_CSA_S6_14) { pdPhi[0] = 0.90; return TRUE; }
	if (strCode == STLCODE_CSA_S6_19) { pdPhi[0] = 0.95; pdPhi[1] = 1.00; pdPhi[2] = 0.90; pdPhi[3] = 0.95; pdPhi[4] = 0.95; return TRUE; }
	if (strCode == STLCODE_AISC_ASD89) { return TRUE; }
	if (strCode == STLCODE_AIK_ASD83) { return TRUE; }
	if (strCode == STLCODE_AIJ_ASD02) { return TRUE; }
	if (strCode == STLCODE_TWN_ASD90) { return TRUE; }
	if (strCode == STLCODE_TWN_ASD96) { return TRUE; } // Add by GAY. PMS:4064. ('09.09.17).
	if (strCode == STLCODE_GBJ17_88) { return TRUE; }
	if (strCode == STLCODE_GB50017_03) { return TRUE; }
	if (strCode == STLCODE_GB50017_15) { return TRUE; }//add by maxiao(2015-9-25)GB50017-15
	if (strCode == STLCODE_GB50017_17) { return TRUE; }//add by xuezc(2018/2/8)GB50017-17
	if (strCode == STLCODE_GB51249_2017) { return TRUE; }//add by xuezc(2018/2/8)GB50017-17
	if (strCode == STLCODE_JGJ209_2010) { return TRUE; }
	if (strCode == STLCODE_IS800_1984) { return TRUE; }
	if (strCode == STLCODE_IS800_2007) { pdPhi[0] = 1.10; pdPhi[1] = 1.25; return TRUE; }// MNET:3774-Seungjun-20081028
	if (strCode == STLCODE_AISI_CFSD86) { return TRUE; }
	if (strCode == STLCODE_AISI_CFSD08) { return TRUE; }
	if (strCode == STLCODE_AIK_CFSD98) { return TRUE; }
	if (strCode == STLCODE_KEPCO97_1111) { return TRUE; }
	if (strCode == STLCODE_KSSC_ASD03) { return TRUE; }
	if (strCode == STLCODE_KSSC_LSD09) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; } // Modify by GAY. PMS:4035. ('09.06.22). Add <KSSC-LSD09>.
	if (strCode == STLCODE_KSSC_LSD16) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_KDS_41_31_2019) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_KDS_41_30_10_2022) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_AISC_LRFD05) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; } // Add by GAY. PMS:4061. ('09.09.02). Add <AISC(13th)-LRFD05>.
	if (strCode == STLCODE_AISC_ASD05) { pdPhi[0] = 1.67; pdPhi[1] = 2.00; pdPhi[2] = 1.67; pdPhi[3] = 1.67; pdPhi[4] = 1.67; return TRUE; } // Add by GAY. PMS:4061. ('09.09.02). Add <AISC(13th)-ASD05>.
	if (strCode == STLCODE_AISC_LRFD10) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; } // Add by GAY. PMS:XXXX. ('13.01.15). Add <AISC(14th)-LRFD10>.
	if (strCode == STLCODE_AISC_ASD10) { pdPhi[0] = 1.67; pdPhi[1] = 2.00; pdPhi[2] = 1.67; pdPhi[3] = 1.67; pdPhi[4] = 1.67; return TRUE; } // Add by GAY. PMS:XXXX. ('13.01.15). Add <AISC(14th)-ASD10>.
	if (strCode == STLCODE_AISC_LRFD16) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_AISC_ASD16) { pdPhi[0] = 1.67; pdPhi[1] = 2.00; pdPhi[2] = 1.67; pdPhi[3] = 1.67; pdPhi[4] = 1.67; return TRUE; }
	if (strCode == STLCODE_AISC_LRFD22) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_AISC_ASD22) { pdPhi[0] = 1.67; pdPhi[1] = 2.00; pdPhi[2] = 1.67; pdPhi[3] = 1.67; pdPhi[4] = 1.67; return TRUE; }
	if (strCode == STLCODE_NSCP_2015_LRFD) { pdPhi[0] = 0.90; pdPhi[1] = 0.75; pdPhi[2] = 0.90; pdPhi[3] = 0.90; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == STLCODE_NSCP_2015_ASD) { pdPhi[0] = 1.67; pdPhi[1] = 2.00; pdPhi[2] = 1.67; pdPhi[3] = 1.67; pdPhi[4] = 1.67; return TRUE; }
	if (strCode == STLCODE_IRC_24_2010) { pdPhi[0] = 1.10; pdPhi[1] = 1.25; return TRUE; }
	if (strCode == STLCODE_IRS_SBC) { return TRUE; }
	if (strCode == STLCODE_SP_16_13330_2017) { pdPhi[0] = 1.30; pdPhi[1] = 1.00; pdPhi[2] = 1.00; pdPhi[3] = 1.00; return TRUE; }
	// Civil.
	//if ( strCode == STLCODE_EC3_2_05 ) { pdPhi[0]=1.00; pdPhi[1]=1.10; pdPhi[2]=1.25; return TRUE; }  // Civil
	if (strCode == STLCODE_EC3_2_05)
	{
		auto* pNA = dgn::lib::CNationalAnnexInterface::Instance();
		if (pNA == nullptr)
		{
			ASSERT(0); return TRUE;
		}
		const auto StlCode = pNA->GetBridgeCode(dgn::def::enDgn::STL, nNatioalAnnex);
		//�ʿ�� �߰����ٰ�
		pNA->SetCode(StlCode);
		pdPhi[0] = pNA->GetGammaM0();
		pdPhi[1] = pNA->GetGammaM1();
		pdPhi[2] = pNA->GetGammaM2();
		return TRUE;
	}
	if (strCode == STLCODE_AASHTO_LRFD24) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.95; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LRFD20) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.95; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LRFD17) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.95; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LRFD16) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.95; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LRFD12) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.90; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LRFD02) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.90; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_AASHTO_LFD96) { return TRUE; }
	if (strCode == STLCODE_KSCE_ASD96) { return TRUE; }
	if (strCode == STLCODE_KSCE_ASD05) { return TRUE; }
	if (strCode == STLCODE_TWN_BRG_LSD90) { return TRUE; }
	if (strCode == STLCODE_JTJ025_86) { return TRUE; }
	if (strCode == STLCODE_AASHTO_ASD96) { return TRUE; }
	if (strCode == STLCODE_TWN_BRG_ASD90) { return TRUE; }
	if (strCode == STLCODE_KSCE_ASD10) { return TRUE; }
	if (strCode == STLCODE_KDS_24_14_30_2019) { return TRUE; }
	if (strCode == STLCODE_KDS_24_14_31_2018) { pdPhi[0] = 0.95; pdPhi[1] = 0.80; pdPhi[2] = 0.9; pdPhi[3] = 1.00; pdPhi[4] = 1.00; return TRUE; }
	if (strCode == STLCODE_KSCE_RAIL_ASD11) { return TRUE; }
	if (strCode == STLCODE_KSCE_LSD15) { pdPhi[0] = 1.0; pdPhi[1] = 1.0; pdPhi[2] = 0.90; pdPhi[3] = 0.8; pdPhi[4] = 0.95; pdPhi[5] = 0.8; pdPhi[6] = 0.8; return TRUE; }
	if (strCode == STLCODE_JROAD_H14) { return TRUE; }
	if (strCode == STLCODE_JROAD_H24) { return TRUE; }
	if (strCode == STLCODE_JROAD_H29) { return TRUE; }
	if (strCode == STLCODE_JROAD_R07) { return TRUE; }
	if (strCode == STLCODE_SP_16_13330_2017) { pdPhi[0] = 1.0; pdPhi[1] = 1.0; pdPhi[2] = 1.0; pdPhi[3] = 1.0; return TRUE; /*TODO:GEN-9484*/ }

	ASSERT(0);
	return FALSE;
}

BOOL CDgnDataCtrl::Get_DgnConPhiDef(CString strCode, double* pdPhi)
{
	for (int i = 0; i < 6; i++)
	{
		pdPhi[i] = 1.0;
	}
	if (strCode == CONCODE_SNIP_2_05_03_84) { return TRUE; }
	if (strCode == CONCODE_SP_35_13330_2011) { return TRUE; }
	if (strCode == CONCODE_SNIP_2_05_03_84_MKS) { return TRUE; }
	if (strCode == CONCODE_SP_35_13330_2011_MKS) { return TRUE; }
	// 0:Bending, 1:Tension, 2:Comp Spiral, 3:Comp Tie, 4:Shear
	// Gen.
	if (strCode == CONCODE_ACI318_89) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_ACI318_95) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_ACI318_99) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_ACI318_02) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318_05) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318_08) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; } // Add by GAY. PMS:XXXX. ('12.03.12). See ACI318-08, 9.3.2.
	if (strCode == CONCODE_ACI318_11) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; } // Add by GAY. PMS:XXXX. ('12.03.12). See ACI318-11, 9.3.2.
	if (strCode == CONCODE_ACI318_14) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318M_14) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318_19) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318M_19) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318_25) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_ACI318M_25) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_NSR_10) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; } // by GAY. PMS:5009. ('15.04.03). see NSR-10, C.9.3.2.
	if (strCode == CONCODE_NSCP_2015) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_NTC_DCEC_2017) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_NTC_DCEC_2023) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_AIK_USD94) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_TWN_USD92) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_TWN_USD100) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_TWN_USD112) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_AIK_WSD2K) { return TRUE; }
	if (strCode == CONCODE_AIJ_WSD99) { return TRUE; }
	if (strCode == CONCODE_BS8110_97) { pdPhi[0] = 1.50; pdPhi[1] = 1.15; pdPhi[2] = 1.25; return TRUE; }
	if (strCode == CONCODE_EC2) { pdPhi[0] = 1.50; pdPhi[1] = 1.30; pdPhi[2] = 1.15; pdPhi[3] = 1.00; return TRUE; }
	if (strCode == CONCODE_EC2_04) { pdPhi[0] = 1.50; pdPhi[1] = 1.20; pdPhi[2] = 1.15; pdPhi[3] = 1.00; return TRUE; } // Coded by Seungjun ('20070613) MNet:No.2313.
	if (strCode == CONCODE_IS456_2000) { pdPhi[0] = 1.50; pdPhi[1] = 1.15; return TRUE; }
	if (strCode == CONCODE_GB50010_02) { pdPhi[0] = 1.40; pdPhi[1] = 1.10; return TRUE; }
	if (strCode == CONCODE_GB50010_10) { pdPhi[0] = 1.40; pdPhi[1] = 1.10; return TRUE; }
	if (strCode == CONCODE_GB50010_19) { pdPhi[0] = 1.40; pdPhi[1] = 1.10; return TRUE; }
	if (strCode == CONCODE_CSA_A23_3_94) { pdPhi[0] = 0.60; pdPhi[1] = 0.85; pdPhi[2] = 0.75; return TRUE; }
	// Civil.
	if (strCode == CONCODE_AASHTO_LRFD24) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD20) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AREMA_2023) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD17) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD16) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD12) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD02) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD07) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.75; pdPhi[4] = 0.90; return TRUE; }
	if (strCode == CONCODE_AASHTO_LFD96) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_KSCE_USD96) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.80; return TRUE; }
	if (strCode == CONCODE_TWN_BRG_LSD90) { pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE; }
	if (strCode == CONCODE_IRC21_2000) { return TRUE; }
	if (strCode == CONCODE_IRC112_2011) { pdPhi[0] = 1.50; pdPhi[1] = 1.20; pdPhi[2] = 1.15; pdPhi[3] = 1.00; pdPhi[4] = 1.00; pdPhi[5] = 1.00; return TRUE; }//PinakinIRC112RC
	if (strCode == CONCODE_IRC112_2020) { pdPhi[0] = 1.50; pdPhi[1] = 1.20; pdPhi[2] = 1.15; pdPhi[3] = 1.00; pdPhi[4] = 1.00; pdPhi[5] = 1.00; return TRUE; }//PinakinIRC112RC
	if (strCode == CONCODE_JTJ023_85) { pdPhi[0] = 1.25; pdPhi[1] = 1.25; return TRUE; }
	if (strCode == CONCODE_CSA_S6_00) { pdPhi[0] = 0.75; pdPhi[1] = 0.90; return TRUE; }
	if (strCode == CONCODE_CSA_S6_14) { pdPhi[0] = 0.75; pdPhi[1] = 0.90; return TRUE; }
	if (strCode == CONCODE_CSA_S6_19) { pdPhi[0] = 0.75; pdPhi[1] = 0.90; return TRUE; }
	if (strCode == CONCODE_TB10002_3_05) { pdPhi[0] = 1.00; pdPhi[1] = 1.00; return TRUE; }
	if (strCode == CONCODE_BS5400_90 ) { pdPhi[0] = 1.5; pdPhi[1] = 1.15; pdPhi[2] = 1.0; pdPhi[3] = 1.15; pdPhi[4] = 0.67; return TRUE; }
	if (strCode == CONCODE_TMH07_89 ) { pdPhi[0] = 1.5; pdPhi[1] = 1.15; pdPhi[2] = 1.0; pdPhi[3] = 1.15; pdPhi[4] = 0.67; return TRUE; }
	if (strCode == CONCODE_EC2_2_05) 
	{ 
		double dDummy = 0.0;
		m_pDoc->m_pDgnDataCtrl->SetDatabyNationalAnnex(Recommended, pdPhi, dDummy, dDummy);
		return TRUE;
	}
	// Common.
	if (strCode == CONCODE_KCI_USD99 || strCode == CONCODE_KCI_USD03 || strCode == CONCODE_KSCE_USD05 || strCode == CONCODE_KSCE_RAIL_USD04)
	{
#if defined(_CIVIL)	// Civil.
		{
			pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.80; return TRUE;
		}
#else								// Gen.
		{
			pdPhi[0] = 0.90; pdPhi[1] = 0.90; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.85; return TRUE;
		}
#endif
	}
	if (strCode == CONCODE_KCI_USD07) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_KCI_USD12) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_KDS_41_30_2018) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_KDS_41_20_2022) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_KSCE_USD10) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.80; return TRUE; }
	if (strCode == CONCODE_KSCE_RAIL_USD11) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.75; pdPhi[3] = 0.70; pdPhi[4] = 0.80; return TRUE; }
	if (strCode == CONCODE_KSCE_LSD15) { pdPhi[0] = 0.65, pdPhi[5] = 0.90; return TRUE; } //��  // ���� pdPhi[0]=0.85, pdPhi[6]=1.0; - �����? �ʿ����?.   
	if (strCode == CONCODE_KDS_24_14_21_2022) { return TRUE; }
	if (strCode == CONCODE_KDS_14_20_00_2022) { pdPhi[0] = 0.85; pdPhi[1] = 0.85; pdPhi[2] = 0.70; pdPhi[3] = 0.65; pdPhi[4] = 0.75; return TRUE; }
	if (strCode == CONCODE_IRS) { pdPhi[0] = 1.5; pdPhi[1] = 1.25; pdPhi[2] = 1.0; pdPhi[3] = 1.15; pdPhi[4] = 0.67; return TRUE; }
	if (strCode == CONCODE_SP_63_13330_2018) { return TRUE; }
	if (strCode == CONCODE_AS5100_5_17) { pdPhi[0] = 0.80; pdPhi[1] = 0.80; pdPhi[2] = 0.60; pdPhi[3] = 0.60; pdPhi[4] = 0.70; return TRUE; }
	ASSERT(0);
	return FALSE;
}

// Add by ZINU.('03.11.19). To Set Default Moment Redistribution Factor.
BOOL CDgnDataCtrl::Get_DgnConMrftDef(CString strCode, double& dMrft)
{
	BOOL bDefOK = (dMrft == 0.0) ? TRUE : FALSE;
	if (bDefOK)
	{
		dMrft = (strCode == CONCODE_GB50010_02 || strCode == CONCODE_GB50010_10) ? 0.85 : 1.0;
	}
	return (dMrft == 0.0) ? FALSE : TRUE;
}

// Add by GAY. MNET:3780. ('09.03.20). To Set Default Torsion Reduction Factor.
BOOL CDgnDataCtrl::Get_DgnConTrftDef(CString strCode, double& dTrft)
{
	BOOL bDefOK = (dTrft == 0.0) ? TRUE : FALSE;
	if (bDefOK)
	{
		dTrft = (strCode == CONCODE_GB50010_10) ? 0.4 : 1.0;
	}
	return (dTrft == 0.0) ? FALSE : TRUE;
}

// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50017-03, GBJ17-88, GB50010-02).
BOOL CDgnDataCtrl::Get_DgnStlChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade)
{
	//add by maxiao(2015-9-25)GB50017-15	add by xuezc(2018/2/7)GB50017-17
	if (!(strCode == STLCODE_GB50017_03 || strCode == STLCODE_GB50017_15 || strCode == STLCODE_GBJ17_88 || strCode == STLCODE_GB50017_17 || strCode == STLCODE_JGJ209_2010)) { return FALSE; }
	iSafeLevel = 1;	// 0=Level1, 1=Level2, 2=Level3.
	iEqGrade = 2;	// 0=9��, 1=8��, 2=7��, 3=6��.
	return TRUE;
}

// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (GB50010-02).
BOOL CDgnDataCtrl::Get_DgnConChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade)
{
	if (strCode != CONCODE_GB50010_02 && strCode != CONCODE_GB50010_10) { return FALSE; }
	iSafeLevel = 1;	// 0=Level1, 1=Level2, 2=Level3.
	iEqGrade = 4;	// 0=Ư1��, 1=9�� �� 1�� �����ӱ���, 2=1��, 3=2��, 4=3��, 5=4��.
	return TRUE;
}

// Add by ZINU.('05.05.25). To Set Default Safety Level, Eq Grade (JGJ138-01).
BOOL CDgnDataCtrl::Get_DgnSrcChinaDef(CString strCode, int& iSafeLevel, int& iEqGrade)
{
	if (strCode == SRCCODE_JGJ138_01 || strCode == SRCCODE_GB50068_2019)
	{
		iSafeLevel = 1;	// 0=Level1, 1=Level2, 2=Level3.
		iEqGrade = 2;	// 0=9��, 1=8��, 2=7��, 3=6��.
		return TRUE;
	}
	return FALSE;
}

// Add by ZINU.('04.02.04). To Set Default Scale Up Factor for Shear.
BOOL CDgnDataCtrl::Get_DgnConSufsDef(CString strCode, int iClass, int& iAlphaType, double& dAlpha1, double& dAlpha2)
{
	BOOL bDefOK = (dAlpha1*dAlpha2 == 0.0) ? TRUE : FALSE;
	if (bDefOK)
	{
		// iAlphaType (0=MAX[Alpha1,Alpha2], 1=MIN[Alpha1,Alpha2], 2=Alpha1, 3=Alpha2).
		if (strCode == CONCODE_AIK_USD94) { iAlphaType = 1; dAlpha1 = 1.0; dAlpha2 = 2.0; }
		else if (strCode == CONCODE_KSCE_USD96) { iAlphaType = 1; dAlpha1 = 1.0; dAlpha2 = 2.0; }
		else if (strCode == CONCODE_KSCE_USD05) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_KSCE_USD10) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_KSCE_RAIL_USD04) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_KSCE_RAIL_USD11) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_KDS_24_14_21_2022) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.    
		else if (strCode == CONCODE_KDS_14_20_00_2022) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_KSCE_LSD15) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.    
		else if (strCode == CONCODE_KCI_USD99) { iAlphaType = 1; dAlpha1 = 1.0; dAlpha2 = 2.0; }
		else if (strCode == CONCODE_KCI_USD03) { iAlphaType = 1; dAlpha1 = 1.0; dAlpha2 = 2.0; }
		else if (strCode == CONCODE_KCI_USD07) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_KCI_USD12) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_KDS_41_30_2018) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_KDS_41_20_2022) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_TWN_USD92) { iAlphaType = 0; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_TWN_USD100) { iAlphaType = 0; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_TWN_USD112) { iAlphaType = 0; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_ACI318_89) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_95) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_99) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_02) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_05) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_08) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); } // Add by GAY. PMS:4450. ('12.03.12)
		else if (strCode == CONCODE_ACI318_11) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); } // Add by GAY. PMS:4450. ('12.03.12)
		else if (strCode == CONCODE_ACI318_14) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318M_14) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_19) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318M_19) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318_25) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_ACI318M_25) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_NSR_10) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); } // by GAY. PMS:5009. ('15.04.03).
		else if (strCode == CONCODE_NSCP_2015) { iAlphaType = (iClass == 0 ? 0 : 1); dAlpha1 = 1.0; dAlpha2 = (iClass == 0 ? 1.0 : 2.0); }
		else if (strCode == CONCODE_NTC_DCEC_2017) { iAlphaType = 0; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_NTC_DCEC_2023) { iAlphaType = 0; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_CSA_A23_3_94) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 2.0; }
		else if (strCode == CONCODE_IS456_2000) { iAlphaType = 2; dAlpha1 = 1.4; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_AIJ_WSD99) { iAlphaType = 1; dAlpha1 = 1.0; dAlpha2 = 1.5; }
		else if (strCode == CONCODE_AIK_WSD2K) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_BS8110_97) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_EC2) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_EC2_04) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_EC2_2_05) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_BS5400_90) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_TMH07_89) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_GB50010_02) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_GB50010_10) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_JTJ023_85) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_IRC21_2000) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_IRC112_2011) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used. PinakinIRC112RC
		else if (strCode == CONCODE_IRC112_2020) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used. PinakinIRC112RC
		else if (strCode == CONCODE_AASHTO_LRFD02) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD07) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD12) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD16) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD17) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD20) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LRFD24) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AREMA_2023) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_CSA_S6_00) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_CSA_S6_14) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_CSA_S6_19) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_AASHTO_LFD96) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = (iClass == 2 || iClass == 3 ? 1.3 : 1.0); }
		else if (strCode == CONCODE_TWN_BRG_LSD90) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = 1.3; }
		else if (strCode == CONCODE_SNIP_2_05_03_84) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_SP_35_13330_2011) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_SNIP_2_05_03_84_MKS) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_SP_35_13330_2011_MKS) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == CONCODE_IRS) { iAlphaType = 2; dAlpha1 = 1.4; dAlpha2 = 1.0; }
		else if (strCode == CONCODE_AS5100_5_17) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else	ASSERT(0);
	}
	return (dAlpha1*dAlpha2 == 0.0) ? FALSE : TRUE;
}

BOOL CDgnDataCtrl::Get_DgnSrcSufsDef(CString strCode, int iClass, int& iAlphaType, double& dAlpha1, double& dAlpha2)
{
	BOOL bDefOK = (dAlpha1*dAlpha2 == 0.0) ? TRUE : FALSE;
	if (bDefOK)
	{
		// Change by ZINU.('05.12.26). If AIJ-SRC01, iAlphaType = 1 -> 3.
		//*/Temp by ZINU. �����? �� �ֵ��� ������ �������? �����Ǹ� �۾��ʿ�.
		if (strCode == SRCCODE_TWN_SRC92) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == SRCCODE_TWN_SRC100) { iAlphaType = 2; dAlpha1 = 1.0; dAlpha2 = 1.0; }
		else if (strCode == SRCCODE_SSRC79) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == SRCCODE_AIK_SRC2K) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == SRCCODE_AIJ_SRC01) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 2.0; }	// 1 -> 3. // AIJ-SRC01, p.17, Eq(49)
		else if (strCode == SRCCODE_JGJ138_01) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else if (strCode == SRCCODE_GB50068_2019) { iAlphaType = 3; dAlpha1 = 1.0; dAlpha2 = 1.0; }	// Not Used.
		else { ASSERT(0); }
	}
	return (dAlpha1*dAlpha2 == 0.0) ? FALSE : TRUE;
}

// Add by ZINU.('02.12.24). To Set Default Lcom for Transfer Stage.
BOOL CDgnDataCtrl::Get_DgnConLcomDef(int iLcomKind, int iLcomKeyTran)
{
	if (iLcomKeyTran == 0)	// If not exist, Set Default.
	{
		CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(iLcomKind, arLcomKey);
		T_LCOM_D LcomData;
		for (int i = 0; i < arLcomKey.GetSize(); i++)
		{
			T_LCOM_K LcomKey = arLcomKey.GetAt(i);
			LcomData.Initialize();
			m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, LcomKey, LcomData);
			if (LcomData.Description.Left(4) == _T("TRNS"))
			{
				iLcomKeyTran = LcomKey;
				break;
			}
		}
	}
	return (iLcomKeyTran > 0) ? TRUE : FALSE;
}

// Add by ZINU.('03.01.03). To Set Default PS-Allowable Stress.
BOOL CDgnDataCtrl::Get_DgnConPsasDef(CString strCode, double& dRtsc, double& dRtsts, double& dRtstm,
	double& dRsscs, double& dRssct, double& dRsst)
{
	BOOL bGetOK = TRUE;
#if defined(_CIVIL)
	if (strCode == CONCODE_KSCE_USD96) { dRtsc = 0.60; dRtsts = 1.50; dRtstm = 0.75; dRsscs = 0.40; dRssct = 0.40; dRsst = 1.50; }
	else if (strCode == CONCODE_KCI_USD99) { dRtsc = 0.60; dRtsts = 1.60; dRtstm = 0.80; dRsscs = 0.40; dRssct = 0.40; dRsst = 1.60; }
	else if (strCode == CONCODE_KCI_USD03) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_KCI_USD07) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_JTJ023_85) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD02) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD07) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD12) {}
	else if (strCode == CONCODE_AASHTO_LRFD16) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD17) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD20) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AASHTO_LRFD24) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_ACI318_02) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_CSA_S6_00) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_CSA_S6_14) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_CSA_S6_19) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_IRC21_2000) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_IRC112_2011) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_IRC112_2020) {}	// Add if PS-Beam is available in this Code.
	else { bGetOK = FALSE; }
#else
	//                                 / Transfer Stage                            / Service Load Stage.
	//                                 / Comp        / Tens                        / Comp                        / Tens.
	//                                               / Support      / Middle       / Sustained    / Total.
	if (strCode == CONCODE_ACI318_89) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.45; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_95) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_99) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_02) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_05) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_08) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; } // Add by GAY. PMS:4450. ('12.03.12)
	else if (strCode == CONCODE_ACI318_11) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; } // Add by GAY. PMS:4450. ('12.03.12)
	else if (strCode == CONCODE_ACI318_14) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318M_14) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_19) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318M_19) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318_25) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_ACI318M_25) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_NSR_10) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; } // by GAY. PMS:5009. ('15.04.06).
	else if (strCode == CONCODE_NSCP_2015) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_NTC_DCEC_2017) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_NTC_DCEC_2023) { dRtsc = 0.60; dRtsts = 6.00; dRtstm = 3.00; dRsscs = 0.45; dRssct = 0.60; dRsst = 6.00; }
	else if (strCode == CONCODE_AIK_USD94) { dRtsc = 0.60; dRtsts = 1.60; dRtstm = 0.80; dRsscs = 0.45; dRssct = 0.45; dRsst = 1.60; }
	else if (strCode == CONCODE_KSCE_USD96) { dRtsc = 0.60; dRtsts = 1.50; dRtstm = 0.75; dRsscs = 0.40; dRssct = 0.40; dRsst = 1.50; }
	else if (strCode == CONCODE_KCI_USD99) { dRtsc = 0.60; dRtsts = 1.60; dRtstm = 0.80; dRsscs = 0.40; dRssct = 0.40; dRsst = 1.60; }
	else if (strCode == CONCODE_KCI_USD03) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_KCI_USD07) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_KCI_USD12) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_KDS_41_30_2018) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_KDS_41_20_2022) { dRtsc = 0.60; dRtsts = 0.50; dRtstm = 0.25; dRsscs = 0.40; dRssct = 0.40; dRsst = 0.50; }
	else if (strCode == CONCODE_AIK_WSD2K) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_AIJ_WSD99) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_BS8110_97) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_EC2) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_EC2_04) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_EC2_2_05) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_IS456_2000) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_GB50010_02) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_GB50010_10) {}	// Add if PS-Beam is available in this Code.
	else if (strCode == CONCODE_CSA_A23_3_94) {}	// Add if PS-Beam is available in this Code.
	else { bGetOK = FALSE; }
#endif
	return bGetOK;
}

// Add by ZINU.('03.02.26). Agree with Kye CJ.
void CDgnDataCtrl::Create_DgnHighRised(const CString& strCode)
{
	m_pDoc->IsHighrisedBLD(strCode, m_bHighRised, m_bOver12F);
}

// Add by ZINU.('03.02.15). For GBJ17-88, GB50017-03 Seismic Design.
BOOL CDgnDataCtrl::Is_HighRiseForGBJ() { return m_bHighRised; }

// Add by ParkBong.('07.06.28).Mnet:2761, Check Over 12F at GB50011-01 8.3.1.
BOOL CDgnDataCtrl::Is_Over12FloorGBJ() { return m_bOver12F; }

bool CDgnDataCtrl::Is_UngerGround(const UINT& iMemb)
{
	// Get MembType (Only Column).
	T_MBTP_D MbtpD;
	MbtpD.Initialize();
	bool bUserColm = (Get_DgnGenMbtp(iMemb, MbtpD) && MbtpD.nMbType == D_MBTP_COLUMN ? true : false);

	bool bMembUnderGround = false;
	// Check Member SubType.
	if (bUserColm)
	{
		// Lower Frame Column(1), Lower Corner Column(3).  See CRCS_DesignPrint::Get_StringMbtp()
		if (MbtpD.nSubType == 1 || MbtpD.nSubType == 3) { bMembUnderGround = true; }
	}
	else
	{
		// Get Ground Level.
		// Check Member is under Ground Level.
		double dGroundLevel = 0.0;
		T_BLDC_D BldcD;
		BldcD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetBldc(BldcD))	dGroundLevel = BldcD.dBaseLevel;

		// Search Nodes (I,J) at Member.
		BOOL bReverseDir = FALSE;
		T_ELEM_K ElemShowKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		if (!m_pMembCtrl->GetElemListByIncludeElem(iMemb, bReverseDir, ElemShowKey, aElemList)) { return false; }
		T_ELEM_K ElemK[2] = { aElemList.GetAt(0), aElemList.GetAt(aElemList.GetSize() - 1) };
		T_ELEM_D ElemD;
		for (int i = 0; i < 2; i++)
		{
			ElemD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK[i], ElemD)) { return false; }
			T_NODE_K NodeK = (bReverseDir ? ElemD.elnod[1] : ElemD.elnod[0]);
			T_NODE_D NodeD; NodeD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD)) { return false; }
			if (NodeD.z < dGroundLevel + cDgn_Zero)	bMembUnderGround = true;
		}
	}

	return bMembUnderGround;
}

// Change by ZINU.('06.08.30). NO:2338, Add Follwing Items.
// 1. Check Conditions (Code, Matl, MembType).
// 2. Check Member SubType (Lower Frame Column, Lower Corner Column).
BOOL CDgnDataCtrl::Get_ConColmEffLengFactorForGB(int key, double& dKy, double& dKz)
{
	dKy = 1.0;
	dKz = 1.0;
	// Set Effective Length Factor.
	// See GB50010-02 Table 7.3.11.
	int i = 0;

	// Check Conditions.
	// Get Con-Code (Only GB50010-02).
	T_DCON_D DconD;
	DconD.Initialize();
	BOOL bCodeOK = (Get_DgnConDcon(DconD) && (DconD.DesignCode == CONCODE_GB50010_02 || DconD.DesignCode == CONCODE_GB50010_10) ? TRUE : FALSE);
	// Get Matl (Only Concrete).
	T_MATD_D MatdD;
	MatdD.Initialize();
	BOOL bMatlOK = (Get_DgnGenMatd(key, MatdD) && MatdD.Type == _T("C") ? TRUE : FALSE);
	// Get MembType (Only Column).
	T_MBTP_D MbtpD;
	MbtpD.Initialize();
	BOOL bMembOK = (Get_DgnGenMbtp(key, MbtpD) && MbtpD.nMbType == D_MBTP_COLUMN ? TRUE : FALSE);
	if (!(bCodeOK && bMatlOK && bMembOK))	return FALSE;

	BOOL bMembUnderGround = FALSE;
	// Check Member SubType.
	if (m_pDoc->m_pAttrCtrl->ExistMbtp(key))
	{
		// Lower Frame Column(1), Lower Corner Column(3).  See CRCS_DesignPrint::Get_StringMbtp()
		if (MbtpD.nSubType == 1 || MbtpD.nSubType == 3) { bMembUnderGround = TRUE; }
	}
	else
	{
		// Get Ground Level.
		// Check Member is under Ground Level.
		double dGroundLevel = 0.0;
		T_BLDC_D BldcD;
		BldcD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetBldc(BldcD))	dGroundLevel = BldcD.dBaseLevel;
		// Search Nodes (I,J) at Member.
		BOOL bReverseDir = FALSE;
		T_ELEM_K ElemShowKey;
		CArray<T_ELEM_K, T_ELEM_K> aElemList;
		if (!m_pMembCtrl->GetElemListByIncludeElem(key, bReverseDir, ElemShowKey, aElemList)) { return FALSE; }
		T_ELEM_K ElemK[2] = { aElemList.GetAt(0), aElemList.GetAt(aElemList.GetSize() - 1) };
		T_ELEM_D ElemD;
		for (i = 0; i < 2; i++)
		{
			ElemD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK[i], ElemD)) { return FALSE; }
			T_NODE_K NodeK = (bReverseDir ? ElemD.elnod[1] : ElemD.elnod[0]);
			T_NODE_D NodeD; NodeD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD)) { return FALSE; }
			if (NodeD.z < dGroundLevel + cDgn_Zero)	bMembUnderGround = TRUE;
		}
	}

	for (int i = 0; i < 2; i++)	// 0=Ky, 1=Kz.
	{
		double dEffLengFactor = 0.0;
		if (bMembUnderGround)	// 1st Floor.
		{
			if (DconD.iSlabType == 0)		dEffLengFactor = 1.0;	// Cast-in-place.
			else if (DconD.iSlabType == 1)	dEffLengFactor = 1.25;	// Precast.
			else	ASSERT(0);
		}
		else	// Other Floor.
		{
			if (DconD.iSlabType == 0)		dEffLengFactor = 1.25;	// Cast-in-place.
			else if (DconD.iSlabType == 1)	dEffLengFactor = 1.5;	// Precast.
			else	ASSERT(0);
		}
		(i == 0 ? dKy : dKz) = dEffLengFactor;
	}

	return TRUE;
}


///////////////////////////
// For Civil.
double CDgnDataCtrl::Get_ClearHDistRbar(double dDistFactor, double dBarDia, CString DgnCode, int iPourigType, int nLayer)
{
	double dClearDist = 0.0;
	if (DgnCode == CONCODE_KSCE_USD96)
	{
		dClearDist = max(2.5*(4.0 / 3.0), dDistFactor*dBarDia);// 2.5 cm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KCI_USD99)
	{
		dClearDist = max(2.5*(4.0 / 3.0), dDistFactor*dBarDia);// 2.5 cm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KCI_USD03 || DgnCode == CONCODE_KCI_USD07)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KSCE_USD05)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KSCE_RAIL_USD04)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_JTJ023_85)
	{
		dClearDist = max(40.0, 1.25*dBarDia);//6.2.12 3���̻�
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD02)
	{
		dClearDist = max(max(1.5*dBarDia, 1.5*1.0), 1.5);//5.10.3.1.1
	}
	else if (DgnCode == CONCODE_AASHTO_LFD96)
	{
		dClearDist = max(max(1.5*dBarDia, 1.5*1.0), 1.5);//5.10.3.1.1
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD07 || DgnCode == CONCODE_AASHTO_LRFD12 ||
			 DgnCode == CONCODE_AASHTO_LRFD16 || DgnCode == CONCODE_AASHTO_LRFD17 ||
			 DgnCode == CONCODE_AASHTO_LRFD20 || DgnCode == CONCODE_AASHTO_LRFD24)
	{
		// 5.10.3.1 Minimum Spcaing of Reinforcing Bars, Length=in
		if (iPourigType == 0)
		{
			dClearDist = max(max(1.5*dBarDia, 1.5*1.0), 1.5); // Cast-in-Place
		}
		else if (iPourigType == 1)
		{
			dClearDist = max(max(dBarDia, 1.33*1.0), 1.0);    // Precast
		}
		else ASSERT(0);
	}
	else if (DgnCode == CONCODE_ACI318_02 || DgnCode == CONCODE_ACI318_05 || DgnCode == CONCODE_ACI318_08 || DgnCode == CONCODE_ACI318_11 ||
		DgnCode == CONCODE_ACI318_14 || DgnCode == CONCODE_ACI318_19 || DgnCode == CONCODE_ACI318_25)
	{
		dClearDist = max(dDistFactor * max(dBarDia, 1.0), 1.0*(4.0 / 3.0));// max(max(BarDia, 1 in), (gravel Dia)*(4/3))
	}
	else if (DgnCode == CONCODE_ACI318M_14 || DgnCode == CONCODE_ACI318M_19 || DgnCode == CONCODE_ACI318M_25 || DgnCode == CONCODE_NSCP_2015 
		|| DgnCode == CONCODE_NTC_DCEC_2017	|| DgnCode == CONCODE_NTC_DCEC_2023)
	{
		dClearDist = max(dDistFactor * max(dBarDia, 25.0), 25.0*(4.0 / 3.0));
	}
	else if (DgnCode == CONCODE_NSR_10)
	{
		dClearDist = max(dDistFactor * max(dBarDia, 25.0), 25.0*(4.0 / 3.0));// max(max(BarDia, 25mm), (gravel Dia)*(4/3))
	}
	else if (DgnCode == CONCODE_CSA_S6_00 || DgnCode == CONCODE_CSA_S6_14 || DgnCode == CONCODE_CSA_S6_19)
	{
		dClearDist = max(max(1.5*dBarDia, 1.5*25.0), 40.0); // 1.5*BarDia, 1.5*Aggregate, 40mm
	}
	else if (DgnCode == CONCODE_IRC21_2000)
	{
		dClearDist = max(dBarDia, 10 + 20); // BarDia, 10mm+Aggregate
	}
	else if (DgnCode == CONCODE_TWN_BRG_LSD90)
	{
		dClearDist = max(max(1.5*dBarDia, 1.5*2.5), 4.0);//1.5*BarDia, 1.5*Aggregate, 4cm
	}
	else if ( DgnCode == CONCODE_BS5400_90 )
	{
		dClearDist = 25.0; // hgg + 5 mm
	}
	else if (DgnCode == CONCODE_TMH07_89)
	{
		dClearDist = 25.0; // hgg + 5 mm
	}
	else if (DgnCode == CONCODE_EC2_2_05)
	{
		// Recommended, British, Italy�� ���� ���? ������ ������ �����ϰ� ����. ���� �ٸ� ���? �߻� �� �ٸ� �Լ��� �����ؾ� ��.
		double dk1 = 1.0; // 1.0mm
		double dk2 = 5.0; // 5.0mm
		dClearDist = max(dk1*dBarDia, max(dk2 + 25.0, 20.0));// k1*BarDai, 5.0+Aggregate, 20mm
	}
	else if (DgnCode == CONCODE_IRC112_2011 || DgnCode == CONCODE_IRC112_2020)
	{
		// Recommended, British, Italy�� ���� ���? ������ ������ �����ϰ� ����. ���� �ٸ� ���? �߻� �� �ٸ� �Լ��� �����ؾ� ��.
		double dk1 = 1.0; // 1.0mm
		double dk2 = 5.0; // 5.0mm
		dClearDist = max(dk1*dBarDia, max(dk2 + 25.0, 20.0));// k1*BarDai, 5.0+Aggregate, 20mm
	}
	else if (DgnCode == CONCODE_KSCE_LSD15 || DgnCode == CONCODE_KDS_24_14_21_2022)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KSCE_USD10 || DgnCode == CONCODE_KSCE_RAIL_USD11)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_KCI_USD12 || DgnCode == CONCODE_KDS_41_30_2018 || DgnCode == CONCODE_KDS_41_20_2022 || DgnCode == CONCODE_KDS_14_20_00_2022)
	{
		dClearDist = max(25.0*(4.0 / 3.0), dDistFactor*dBarDia);// 25 mm.(gravel Dia)*(4/3).
	}
	else if (DgnCode == CONCODE_SNIP_2_05_03_84 || DgnCode == CONCODE_SP_35_13330_2011 || DgnCode == CONCODE_SNIP_2_05_03_84_MKS || DgnCode == CONCODE_SP_35_13330_2011_MKS)
	{
		// SNiP:3.122, SP35: 7.122
		if (nLayer < 2) { dClearDist = 40.0; }// 40 mm
		else if (nLayer == 2) { dClearDist = 50.0; }// 50 mm
		else if (nLayer > 2) { dClearDist = 60.0; }// 60 mm
	}
	else if (DgnCode == CONCODE_IRS)
	{
		dClearDist = 25.0; // IRS 15.9.2.2
	}
	else if (DgnCode == CONCODE_IS456_2000)
	{
		dClearDist = max(dBarDia, 5 + 20); // BarDia, 10mm+Aggregate
	}
	else ASSERT(0);

	return dClearDist;
}

double CDgnDataCtrl::Get_BeamVDistRbar(double dBarDia, CString DgnCode)
{
	double dClearDist = 0.0;

	if (DgnCode == CONCODE_KSCE_USD96)
	{
		dClearDist = 2.5; //cm
	}
	else if (DgnCode == CONCODE_KCI_USD99)
	{
		dClearDist = 2.5; //cm
	}
	else if (DgnCode == CONCODE_KCI_USD03)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KCI_USD07)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KSCE_USD05)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KSCE_RAIL_USD04)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_JTJ023_85)
	{
		dClearDist = max(40.0, 1.25*dBarDia);//mm
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD02)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD07)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD12)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD16)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD17)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if ( DgnCode == CONCODE_AASHTO_LRFD20 )
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LRFD24)
	{
		dClearDist = max(1.0, dBarDia);//in
	}
	else if (DgnCode == CONCODE_AASHTO_LFD96)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318_02)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318_05)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318_08)
	{
		dClearDist = 1.0;//in // Add by GAY. PMS:4450. ('12.03.12)
	}
	else if (DgnCode == CONCODE_ACI318_11)
	{
		dClearDist = 1.0;//in // Add by GAY. PMS:4450. ('12.03.12). See ACI318-11, 7.6.2.
	}
	else if (DgnCode == CONCODE_ACI318_14)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318M_14)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_ACI318_19)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318M_19)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_ACI318_25)
	{
		dClearDist = 1.0;//in
	}
	else if (DgnCode == CONCODE_ACI318M_25)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_NSR_10)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_NSCP_2015)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_NTC_DCEC_2017 || DgnCode == CONCODE_NTC_DCEC_2023)
	{
		dClearDist = 25.0; // 25 mm
	}
	else if (DgnCode == CONCODE_CSA_S6_00)
	{
		dClearDist = max(25.0, dBarDia);//mm
	}
	else if (DgnCode == CONCODE_CSA_S6_14)
	{
		dClearDist = max(25.0, dBarDia);//mm
	}
	else if (DgnCode == CONCODE_CSA_S6_19)
	{
	dClearDist = max(25.0, dBarDia);//mm
	}
	else if (DgnCode == CONCODE_IRC21_2000)
	{
		dClearDist = max(max(12.0, 20.0), dBarDia); // 12mm, Aggregate, BarDia
	}
	else if (DgnCode == CONCODE_TWN_BRG_LSD90)
	{
		dClearDist = 2.5;//cm
	}
	else if (DgnCode == CONCODE_EC2_2_05)
	{
		// Eurocode2:04/Eurocode2-2:05������ ���򰣰� �� ���������� �����ϰ� ������. Ȯ�ο��?. EC2:04, 8.2(2)Ȯ��.
		// Recommended, British, Italy�� ���� ���? ������ ������ �����ϰ� ����. ���� �ٸ� ���? �߻� �� �ٸ� �Լ��� �����ؾ� ��.
		double dk1 = 1.0; // 1.0mm
		double dk2 = 5.0; // 5.0mm
		dClearDist = max(dk1*dBarDia, max(dk2 + 25.0, 20.0));// k1*BarDai, 5.0+Aggregate, 20mm
	}
	else if (DgnCode == CONCODE_IRC112_2011 || DgnCode == CONCODE_IRC112_2020)
	{
		// Eurocode2:04/Eurocode2-2:05������ ���򰣰� �� ���������� �����ϰ� ������. Ȯ�ο��?. EC2:04, 8.2(2)Ȯ��.
		// Recommended, British, Italy�� ���� ���? ������ ������ �����ϰ� ����. ���� �ٸ� ���? �߻� �� �ٸ� �Լ��� �����ؾ� ��.
		double dk1 = 1.0; // 1.0mm
		double dk2 = 5.0; // 5.0mm
		dClearDist = max(dk1*dBarDia, max(dk2 + 25.0, 20.0));// k1*BarDai, 5.0+Aggregate, 20mm
	}
	else if (DgnCode == CONCODE_KSCE_LSD15 || DgnCode == CONCODE_KDS_24_14_21_2022)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KSCE_USD10)
	{
		dClearDist = 25.0;//mm
	}

	else if (DgnCode == CONCODE_KSCE_RAIL_USD11)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KCI_USD12 || DgnCode == CONCODE_KDS_14_20_00_2022)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_KDS_41_30_2018 || DgnCode == CONCODE_KDS_41_20_2022)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_SNIP_2_05_03_84 || DgnCode == CONCODE_SP_35_13330_2011 || DgnCode == CONCODE_SNIP_2_05_03_84_MKS || DgnCode == CONCODE_SP_35_13330_2011_MKS)
	{
		dClearDist = 50.0; // 50 mm	// SNiP:3.122, SP35: 7.122 b)
	}
	else if (DgnCode == CONCODE_IRS)
	{
		dClearDist = 25.0;//mm
	}
	else if (DgnCode == CONCODE_IS456_2000)
	{
		dClearDist = max(dBarDia, 15.0), 25.0*(2.0 / 3.0);// max(max(BarDia, 25mm), (gravel Dia)*(4/3))
	}
	else ASSERT(0);

	return dClearDist + dBarDia;
}

double CDgnDataCtrl::Get_BeamDistRbar(double dBarDia, CString DgnCode, int iPourigType, int nLayer)
{
	double dClearDist = Get_ClearHDistRbar(1.0, dBarDia, DgnCode, iPourigType, nLayer);
	return dBarDia + dClearDist;
}

double CDgnDataCtrl::Get_ColmDistRbar(double dBarDia, CString DgnCode, int nLayer)
{
	double dClearDist1 = 0.0;
	if (DgnCode == CONCODE_KSCE_USD96) { dClearDist1 = 4.0; }	// 4.0 cm.
	else if (DgnCode == CONCODE_KCI_USD99) { dClearDist1 = 4.0; } // 4.0 cm.
	else if (DgnCode == CONCODE_KCI_USD03) { dClearDist1 = 40.0; }// 40. mm.
	else if (DgnCode == CONCODE_KCI_USD07) { dClearDist1 = 40.0; } // 40. mm.
	else if (DgnCode == CONCODE_KCI_USD12) { dClearDist1 = 40.0; } // 40. mm.
	else if (DgnCode == CONCODE_KDS_41_30_2018) { dClearDist1 = 40.0; } // 40. mm.
	else if (DgnCode == CONCODE_KDS_41_20_2022) { dClearDist1 = 40.0; } // 40. mm.
	else if (DgnCode == CONCODE_KSCE_USD05) { dClearDist1 = 40.0; } // 40. mm.
	else if (DgnCode == CONCODE_KSCE_RAIL_USD04) { dClearDist1 = 40.0; }	// 40. mm.
	else if (DgnCode == CONCODE_EC2_2_05) { dClearDist1 = 20.0; }// 20. mm.  // EC2:04, 8.2(2)����.
	else if (DgnCode == CONCODE_BS5400_90) { dClearDist1 = 20.0; }// 20. mm.  
	else if (DgnCode == CONCODE_TMH07_89) { dClearDist1 = 20.0; }// 20. mm.  
	else if (DgnCode == CONCODE_IRC112_2011) { dClearDist1 = 20.0; }// 20. mm.  // EC2:04, 8.2(2)����.
	else if (DgnCode == CONCODE_IRC112_2020) { dClearDist1 = 20.0; }// 20. mm.  // EC2:04, 8.2(2)����.
	else if (DgnCode == CONCODE_IRS) { dClearDist1 = 50.0; }// 20. mm IRS 15.9.2.2
	else if (DgnCode == CONCODE_IS456_2000) { dClearDist1 = 40.0; }	// 40. mm.

	int nPourigType = 0;
	double dClearDist2 = Get_ClearHDistRbar(1.5, dBarDia, DgnCode, nPourigType, nLayer);
	return dBarDia + max(dClearDist1, dClearDist2);
}

BOOL CDgnDataCtrl::Check_BeamUseDist(int iSectShp, int iSite, double dD[], int iBarNum, double dBarDia,
	double dDc, double dDc1, CString DgnCode, int iPourigType)
{
	// Get Column Clear Dist by Code.
	double dBarDist = Get_BeamDistRbar(dBarDia, DgnCode, iPourigType);
	int iDivNum = iBarNum - 1;
	double dUseDist = 0.0;
	if (iSectShp == 3)	// T.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		double dTw = dD[2];
		double dTf = dD[3];
		if (dDc > dHc / 2.0) { return FALSE; }
		if (iSite == 1)		dUseDist = (dTw - 2 * dDc1) / iDivNum;	// Top.
		else if (iSite == 2)	dUseDist = (dTw - 2 * dDc1) / iDivNum;	// Bot.
		else	ASSERT(0);
	}
	else if (iSectShp == 7)	// SB.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		if (dDc > dHc / 2.0) { return FALSE; }
		if (iSite == 1)		dUseDist = (dB1 - 2 * dDc1) / iDivNum;	// Top.
		else if (iSite == 2)	dUseDist = (dB1 - 2 * dDc1) / iDivNum;	// Bot.
		else	ASSERT(0);
	}
	else	ASSERT(0);

	// Check Bar Dist.
	return (dUseDist >= dBarDist) ? TRUE : FALSE;
}


BOOL CDgnDataCtrl::Check_ColmUseDist(int iSectShp, int iSite, double dD[], int iBarNum, double dBarDia,
	double dDc, CString DgnCode, int nLayer)
{
	double dPi = 4.0*atan(1.0);
	// Get Column Clear Dist by Code.
	double dBarDist = Get_ColmDistRbar(dBarDia, DgnCode, nLayer);
	int iDivNum = iBarNum;
	double dUseDist = 0.0;
	if (iSectShp == 2)	// H.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		double dTw = dD[2];
		double dTf1 = dD[3];
		double dB2  = dgn::UQ0(dD[4]) ? dD[4] : dB1;
		double dTf2 = dgn::UQ0(dD[5]) ? dD[5] : dTf1;
		if (dDc > min(dTf1, dTf2)) { return FALSE; }
		if (dDc > min(dHc, min(dB2, dB1) / 2.0)) { return FALSE; }
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)	dUseDist = (dB1 - 2 * dDc) / (iDivNum - 1);
		else if (iSite == 2)	dUseDist = (dB2 - 2 * dDc) / (iDivNum + 1);
		else	ASSERT(0);
	}
	else if (iSectShp == 4)	// B.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		double dTw = dD[2];
		double dTf1 = dD[3];
		double dB2 = dD[4];
		double dTf2 = dD[5];
		if (dDc > min(dTw, min(dTf1, dTf2))) { return FALSE; }
		if (dDc > min(dHc, dB1) / 2.0) { return FALSE; }
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)	dUseDist = (dB1 - 2 * dDc) / (iDivNum - 1);
		else if (iSite == 2)	dUseDist = (dHc - 2 * dDc) / (iDivNum + 1);
		else	ASSERT(0);
	}
	else if (iSectShp == 5)	// P.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		if (dDc > dB1) { return FALSE; }
		if (dDc > dHc / 2.0) { return FALSE; }
		if (iSite == 1)
		{
			double dAng = 2 * dPi / iDivNum;	// Full Circle.
			dUseDist = 2 * (dHc / 2.0 - dDc)*sin(dAng / 2.0);
		}
		else	ASSERT(0);
	}
	else if (iSectShp == 6)	// SR.
	{
		double dHc = dD[0];
		if (dDc > dHc / 2.0) { return FALSE; }
		if (iSite == 1)
		{
			double dAng = 2 * dPi / iDivNum;	// Full Circle.
			dUseDist = 2 * (dHc / 2.0 - dDc)*sin(dAng / 2.0);
		}
		else	ASSERT(0);
	}
	else if (iSectShp == 7)	// SB.
	{
		double dHc = dD[0];
		double dB1 = dD[1];
		if (dDc > min(dHc, dB1) / 2.0) { return FALSE; }
		// Seungjun
		if (iSite == 1)
		{
			dUseDist = (dB1 - 2 * dDc) / (iDivNum - 1);
		}
		else if (iSite == 2)
		{
			dUseDist = (dHc - 2 * dDc) / (iDivNum + 1);
		}
		else	ASSERT(0);
	}
	else if (iSectShp == 12)	// OCT.
	{
		double dH = dD[0];
		double dB = dD[1];
		double da = dD[2];
		double db = dD[3];
		double dt = dD[4];
		if (dDc > dt) { return FALSE; }
		// Calculate Inner Section Dimension.
		T_SECT_SECTBASE_D SectData;
		SectData.Initialize();
		SectData.Size[0] = dH;
		SectData.Size[1] = dB;
		SectData.Size[2] = da;
		SectData.Size[3] = db;
		SectData.Size[4] = dt;
		double dH1 = 0.0, dB1 = 0.0, da1 = 0.0, db1 = 0.0, dDist0 = 0.0, dTheta1 = 0.0;
		if (!CDBDoc::GetDocPoint()->m_pSectDB->CalcSectRegular_InOCT(SectData, dH1, dB1, da1, db1, dDist0, dTheta1)) { ASSERT(0); }
		double dPosY = (dB / 2.0 - da) - ((dB / 2.0 - da) - (dB1 / 2.0 - da1))*(dDc / dt);
		double dPosZ = (dH / 2.0 - db) - ((dH / 2.0 - db) - (dH1 / 2.0 - db1))*(dDc / dt);
		double dDist = 0.0;
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)
		{
			dUseDist = 2 * dPosY / (iDivNum - 1);
		}
		else if (iSite == 2)
		{
			dUseDist = sqrt(pow(dPosY - dB1 / 2, 2) + pow(dPosZ - dH1 / 2, 2)) / (iDivNum + 1);
		}
		else if (iSite == 3)
		{
			dUseDist = 2 * dPosZ / (iDivNum - 1);
		}
		else	ASSERT(0);
		//dUseDist = dDist/iDivNum;
	}
	else if (iSectShp == 13)	// SOCT.
	{
		double dH = dD[0];
		double dB = dD[1];
		double da = dD[2];
		double db = dD[3];
		if (dDc > min(dH, dB) / 2.0) { return FALSE; }
		double dTheta = (dPi - atan(db / da)) / 2.0;	// Radian.
		double dPosY = (dB / 2.0 - da) - dDc / tan(dTheta);
		double dPosZ = (dH / 2.0 - db) - dDc / tan(dTheta);
		double dDist = 0.0;
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)
		{
			dUseDist = 2 * dPosY / (iDivNum - 1);
		}
		else if (iSite == 2)
		{
			dUseDist = sqrt(pow(dPosY - (dB / 2 - dDc), 2) + pow(dPosZ - (dH / 2 - dDc), 2)) / (iDivNum + 1);
		}
		else if (iSite == 3)
		{
			dUseDist = 2 * dPosZ / (iDivNum - 1);
		}
		else	ASSERT(0);
		//dUseDist = dDist/iDivNum;
	}
	else if (iSectShp == 14)	// TRK.
	{
		double dH = dD[0];
		double dB = dD[1];
		double dt = dD[2];
		if (dDc > dt) { return FALSE; }
		if (dDc > min(dH, dB) / 2.0) { return FALSE; }
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)	dUseDist = (dB - dH) / (iDivNum - 1);
		else if (iSite == 2)
		{
			double dAng = dPi / (iDivNum + 1);	// Half Circle.
			dUseDist = 2 * (dH / 2.0 - dDc)*sin(dAng / 2.0);
		}
		else	ASSERT(0);
	}
	else if (iSectShp == 15)	// STRK.
	{
		double dH = dD[0];
		double dB = dD[1];
		if (dDc > min(dH, dB) / 2.0) { return FALSE; }
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)
		{
			dUseDist = (dB - dH) / (iDivNum - 1);
		}
		else if (iSite == 2)
		{
			double dAng = dPi / (iDivNum + 1);	// Half Circle.
			dUseDist = 2 * (dH / 2.0 - dDc)*sin(dAng / 2.0);
		}
		else	ASSERT(0);
	}
	else if (iSectShp == 16)	// HTRK.
	{
		double dH = dD[0];
		double dB = dD[1];
		if (dDc > min(dB - dH / 2.0, dH / 2.0)) { return FALSE; }
		// changing the form of setting rebar. changed the code by SeungJun (`06.02.24)
		if (iSite == 1)
		{
			dUseDist = (dB - dH / 2.0 - dDc) / (iDivNum - 1);
		}
		else if (iSite == 2)
		{
			double dAng = dPi / (iDivNum + 1);	// Half.
			dUseDist = 2 * (dH / 2.0 - dDc)*sin(dAng / 2.0);
		}
		else if (iSite == 3)
		{
			dUseDist = (dH - 2 * dDc) / (iDivNum + 1);
		}
		else	ASSERT(0);
	}
	else	ASSERT(0);

	// Check Bar Dist.
	return (dUseDist >= dBarDist) ? TRUE : FALSE;
}

int CDgnDataCtrl::Check_RbarCivil(int iKind, _DGNC_RBAR& RbarData)
{
	// iKind = 1:Beam, 2:Column/Brace.
	// Return Value (If 0 is Good, else as follows).
	//  1 : No good Code.
	//  2 : Not exist Section.
	//  3 : No good Section.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	T_DCON_D ConcData;
	ConcData.Initialize();
	Get_DgnConDcon(ConcData);

	const CString& strCode = ConcData.DesignCode;
	double dCovering = 0.0;
	// Create Rbar Data by Code Unit.
	if (strCode == CONCODE_KSCE_USD96 || strCode == CONCODE_KCI_USD99 || strCode == CONCODE_TWN_BRG_LSD90)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_CM;	// cm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KG;		// kgf.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 4.0;  // 4 cm.
	}
	else if (strCode == CONCODE_JTJ023_85 || strCode == CONCODE_CSA_S6_00 || 
		strCode == CONCODE_CSA_S6_14 || strCode == CONCODE_CSA_S6_19 ||
		strCode == CONCODE_IRC21_2000 || strCode == CONCODE_TB10002_3_05 ||
		strCode == CONCODE_KCI_USD03 || strCode == CONCODE_KCI_USD07 ||
		strCode == CONCODE_KCI_USD12 || strCode == CONCODE_KDS_41_30_2018 || 
		strCode == CONCODE_KDS_41_20_2022 || strCode == CONCODE_KSCE_RAIL_USD04 || 
		strCode == CONCODE_KSCE_RAIL_USD11 || strCode == CONCODE_KSCE_USD05 || 
		strCode == CONCODE_KSCE_USD10 || strCode == CONCODE_KSCE_LSD15 ||
		strCode == CONCODE_KDS_24_14_21_2022 || strCode == CONCODE_KDS_14_20_00_2022 || 
		strCode == CONCODE_SNIP_2_05_03_84 || strCode == CONCODE_SP_35_13330_2011 ||
		strCode == CONCODE_SNIP_2_05_03_84_MKS || strCode == CONCODE_SP_35_13330_2011_MKS || 
		strCode == CONCODE_AS5100_5_17)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		if (strCode == CONCODE_JTJ023_85 || strCode == CONCODE_TB10002_3_05)
		{
			dCovering = 25.0;  // 25 mm.
		}
		else if (strCode == CONCODE_SNIP_2_05_03_84 || strCode == CONCODE_SP_35_13330_2011 || strCode == CONCODE_SNIP_2_05_03_84_MKS || strCode == CONCODE_SP_35_13330_2011_MKS)
		{
			dCovering = 0.0;  // 0 mm.
		}
		else
		{
			dCovering = 40.0;  // 40 mm.
		}
	}
	else if (strCode == CONCODE_AASHTO_LRFD02 || strCode == CONCODE_AASHTO_LRFD07 ||
			 strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || 
			 strCode == CONCODE_AASHTO_LRFD17 || strCode == CONCODE_AASHTO_LRFD20 || strCode == CONCODE_AASHTO_LRFD24)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;	// in.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KIP;	// Kips.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 1.5;  // 1.5 in.
	}
	else if (strCode == CONCODE_ACI318_02 || strCode == CONCODE_ACI318_05 || strCode == CONCODE_AASHTO_LFD96 ||
		strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 || strCode == CONCODE_ACI318_14 ||
		strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318_25)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;	// in.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_LBF;	// lb.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 1.5;  // 1.5 in.
	}
	else if (strCode == CONCODE_ACI318M_19 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NSR_10 || strCode == CONCODE_NSCP_2015 || 
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0; // 40 mm.
	}
	else if (strCode == CONCODE_EC2_2_05)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if ( strCode == CONCODE_BS5400_90 )
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0;  // 10 mm.
	}
	else if (strCode == CONCODE_TMH07_89)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IS456_2000||strCode==CONCODE_AREMA_2023)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IRC112_2020)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IRS)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else
	{
		return 1;
	}

	// Get Length Factor by Code Unit.
	double dLenCodeFactor = 0.0;
	switch (CngIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM: { dLenCodeFactor = 10.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_CM: { dLenCodeFactor = 1.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_M: { dLenCodeFactor = 0.01; } break;
	case D_UNITSYS_LENGTH_INDEX_IN: { dLenCodeFactor = 1.0 / 2.54; } break;
	case D_UNITSYS_LENGTH_INDEX_FT: { dLenCodeFactor = 1.0 / 30.48; } break;
	default: ASSERT(0); break;
	}

	// Get Length Factor by Cur Unit.
	double dLenCurFactor = 0.0;
	switch (CurIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM: { dLenCurFactor = 10.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_CM: { dLenCurFactor = 1.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_M: { dLenCurFactor = 0.01; } break;
	case D_UNITSYS_LENGTH_INDEX_IN: { dLenCurFactor = 1.0 / 2.54; } break;
	case D_UNITSYS_LENGTH_INDEX_FT: { dLenCurFactor = 1.0 / 30.48; } break;
	default: ASSERT(0); break;
	}

	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] /= (dLenCurFactor / dLenCodeFactor);
	}

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////
	// Get Section by Code Unit.
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(RbarData.iSectNo, sData);
	//////////////////////////////////////////////
	// <Remember> Data�� �ް����� User Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data�� �ް����� User Unit����
	//////////////////////////////////////////////
	if (!bGetOK)	return 2;

	int iSectShp = 0;
	int iSiteNum = 0;
	double dD[8] = { 0.0 };
	if (sData.nStype == D_SECT_TYPE_REGULAR)
	{
		CString strShp = sData.SectBefore.Shape;
		if (strShp == _T("H")) { iSectShp = 2;	iSiteNum = 2; }// Add by sshan. MNET:2607.('20061222)
		else if (strShp == _T("T")) { iSectShp = 3;	iSiteNum = 2; }
		else if (strShp == _T("B")) { iSectShp = 4;	iSiteNum = 2; }
		else if (strShp == _T("P")) { iSectShp = 5;	iSiteNum = 1; }
		else if (strShp == _T("SR")) { iSectShp = 6;	iSiteNum = 1; }
		else if (strShp == _T("SB")) { iSectShp = 7;	iSiteNum = 2; }
		else if (strShp == _T("OCT")) { iSectShp = 12;	iSiteNum = 3; }
		else if (strShp == _T("SOCT")) { iSectShp = 13;	iSiteNum = 3; }
		else if (strShp == _T("TRK")) { iSectShp = 14;	iSiteNum = 2; }
		else if (strShp == _T("STRK")) { iSectShp = 15;	iSiteNum = 2; }
		else if (strShp == _T("HTRK")) { iSectShp = 16;	iSiteNum = 3; }

		if (iKind == 1)	// Beam.
		{
			if (!(iSectShp == 3 || iSectShp == 7))	return 3;
		}
		else if (iKind == 2)	// Column/Brace.
		{
			if (!(iSectShp == 2 || iSectShp == 4 || iSectShp == 5 || iSectShp == 6 ||
				iSectShp == 7 || iSectShp == 12 || iSectShp == 13 ||
				iSectShp == 14 || iSectShp == 15 || iSectShp == 16))	return 3;
		}
		dD[0] = sData.SectBefore.SectI.Size[0];
		dD[1] = sData.SectBefore.SectI.Size[1];
		dD[2] = sData.SectBefore.SectI.Size[2];
		dD[3] = sData.SectBefore.SectI.Size[3];
		dD[4] = sData.SectBefore.SectI.Size[4];
		dD[5] = sData.SectBefore.SectI.Size[5];
		dD[6] = sData.SectBefore.SectI.Size[6];
		dD[7] = sData.SectBefore.SectI.Size[7];
		if (iSectShp == 4)	// B.
		{
			if (dD[4] == 0.0)	dD[4] = dD[1] - dD[2];	// Cw = B1-tw.
			if (dD[5] == 0.0)	dD[5] = dD[2];				// Tf2 = Tf1.
			if (fabs(dD[4] - (dD[1] - dD[2])) > 1.0E-08)	return 3;
		}
	}
	// Initialize Check Results.
	BOOL bCheckRebar[3][5];

	for (int i = 0; i < 5; i++)
	{
		RbarData.bCheckCovc[i] = TRUE;
		RbarData.bCheckCovt[i] = TRUE;
		RbarData.bCheckCovb[i] = TRUE;
		for (int j = 0; j < 3; j++)
		{
			RbarData.bCheckRebar[j][i] = TRUE;
			RbarData.bCheckSpace[j][i] = TRUE;

			bCheckRebar[j][i] = TRUE;
		}
	}
	// Check Rebar Spacing.
	if (iKind == 1)	// Beam.
	{
		CString strSubBarNa = RbarData.strSubBarNa;
		if (strSubBarNa == _T("")) { CDBLib::GetDefaultSubRebarSize(1, strSubBarNa); }// by Beam.

		if (RbarData.iBarLayt == 0 && RbarData.iBarLayb == 0)
		{// Layer�� ���? ���� �� 
			RbarData.bCheckRebar[0][0] = FALSE;
			RbarData.bCheckRebar[1][0] = FALSE;
			return 0;
		}

		double dSubBarDia = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strSubBarNa);
		for (int i = 0; i < 2; i++)	// 0:Top, 1:Bot.
		{
			int iBarLay = (i == 0 ? RbarData.iBarLayt : RbarData.iBarLayb);
			int nBrarLayer = (iBarLay == 0) ? 1 : iBarLay;
			for (int j = 0; j < nBrarLayer; j++)
			{
				int iBarNum = RbarData.iBarNum[i][j];
				double dDc = (i == 0 ? RbarData.dDt[j] : RbarData.dDb[j]);
				double dDc1 = (i == 0 ? RbarData.dDt[0] : RbarData.dDb[0]);
				CString strBarNa1 = RbarData.strBarNa1[i][j];
				CString strBarNa2 = RbarData.strBarNa2[i][j];

				if (iBarNum < 0)
				{
					RbarData.bCheckRebar[i][j] = FALSE;
					continue;
				}
				else if (iBarNum == 0 && dDc == 0.0 && strBarNa1 == _T(""))
				{ // ö�� ������ ������    True
					RbarData.bCheckRebar[i][j] = TRUE;
					bCheckRebar[i][j] = FALSE;

					if (i == 1)
					{  // ���ϴ� ���? ���� ���� �ɷ���.
						if (bCheckRebar[0][j] == FALSE && bCheckRebar[1][j] == FALSE)
						{
							RbarData.bCheckRebar[0][j] = FALSE;
							RbarData.bCheckRebar[1][j] = FALSE;
							continue;
						}
					}
					continue;
				}
				else if (iBarNum > 0 && dDc > 0.0 && strBarNa1 != _T(""))
				{// ���? �Էµ� ���� True
					RbarData.bCheckRebar[i][j] = TRUE;
					continue;
				}
				else
				{// �׿ܴ� False 
					RbarData.bCheckRebar[i][j] = FALSE;
					continue;
				}

				if (strBarNa1 == _T("")) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }

				if (dDc <= 0.0 && iBarNum <= 0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else		RbarData.bCheckCovb[j] = FALSE;	// Bot.
					continue;
				}
				if (dDc < 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else		RbarData.bCheckCovb[j] = FALSE;	// Bot.
					continue;
				}

				double dBarDia1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa1);
				double dBarDia2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa2) : dBarDia1);
				double dBarDia = max(dBarDia1, dBarDia2);
				if (min(dBarDia1, dBarDia2) <= 0.0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }

				double dBarDist = Get_BeamVDistRbar(dBarDia, strCode);
				double dCompSpace = 0.0;

				if (strCode == CONCODE_SNIP_2_05_03_84 || strCode == CONCODE_SP_35_13330_2011 || strCode == CONCODE_SNIP_2_05_03_84_MKS || strCode == CONCODE_SP_35_13330_2011_MKS)
				{ // ���þ� ���ؿ��� ö�ٰ��� ���� ������.
					dCompSpace = 0.0;
				}
				else
				{
					RbarData.bCheckSpace[i][j] = Check_BeamUseDist(iSectShp, i + 1, dD, iBarNum, dBarDia, dDc, dDc1, ConcData.DesignCode);

					if (j == 0)	dCompSpace = dDc - (dCovering + dSubBarDia + dBarDia / 2.0);
					else
					{
						double dDcp = (i == 0 ? RbarData.dDt[j - 1] : RbarData.dDb[j - 1]);
						dCompSpace = (dDc - dDcp) - dBarDist;
					}
				}
				if (dCompSpace < 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
				}
				// Calculate Ast,Asb (As for Beam).
				double dBarAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa1);
				double dBarAs2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa2) : dBarAs1);
				// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
				double dBarNum1 = iBarNum / 2.;
				double dBarNum2 = dBarNum1;
				if (i == 0)	RbarData.dAst += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Top.
				else		RbarData.dAsb += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Bot.
			}
		}

	}
	else if (iKind == 2)	// Column, Brace.
	{
		CString strShp = sData.SectBefore.Shape;
		CString strSubBarNa = RbarData.strSubBarNa;
		if (strSubBarNa == _T(""))	CDBLib::GetDefaultSubRebarSize(2, strSubBarNa);	// by Column.
		double dSubBarDia = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strSubBarNa);
		for (int i = 0; i < iSiteNum; i++)
		{
			for (int j = 0; j < RbarData.iBarLayc; j++)
			{
				int iBarNum = RbarData.iBarNum[i][j];
				double dDc = RbarData.dDc[j];
				if (strShp == _T("H")) dDc = RbarData.dDc[0];//H �ϰ��? ���̾ �����ص� ö�� ��ٽ�? ���� ���� �ʴ´�.
				CString strBarNa1 = RbarData.strBarNa1[i][j];
				CString strBarNa2 = RbarData.strBarNa2[i][j];
				// Change by Jaeoh. (06.05.23)
				if (strShp != _T("HTRK"))
				{
					if (i % 2 == 0)
					{
						if (iBarNum < 2) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
					}
					else
					{
						if (iBarNum < 0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
					}
				}
				else
				{
					if (i == 0)
					{
						if (iBarNum < 2) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
					}
					else
					{
						if (iBarNum < 0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
					}
				}
				if (strBarNa1 == _T("")) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				if (dDc <= 0.0) { RbarData.bCheckCovc[j] = FALSE;	continue; }
				double dBarDia1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa1);
				double dBarDia2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa2) : dBarDia1);
				double dBarDia = max(dBarDia1, dBarDia2);
				if (min(dBarDia1, dBarDia2) <= 0.0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				RbarData.bCheckSpace[i][j] = Check_ColmUseDist(iSectShp, i + 1, dD, iBarNum, dBarDia, dDc, ConcData.DesignCode, RbarData.iBarLayc);
				double dBarDist = Get_ColmDistRbar(dBarDia, ConcData.DesignCode, RbarData.iBarLayc);
				double dCompSpace = 0.0;
				if (j == 0)	dCompSpace = dDc - (dCovering + dSubBarDia + dBarDia / 2.0);
				else
				{
					double dDcp = RbarData.dDc[j - 1];
					dCompSpace = (dDc - dDcp) - dBarDist;
				}
				if (dCompSpace < 0.0)	RbarData.bCheckCovc[j] = FALSE;
				// Calculate Asc (As for Column).
				double dBarAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa1);
				double dBarAs2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa2) : dBarAs1);
				// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
				double dBarNum1 = ceil(iBarNum / 2.);
				double dBarNum2 = iBarNum - dBarNum1;
				RbarData.dAsc += Get_UsedAs(RbarData.iSectNo, i + 1, dBarNum1*dBarAs1 + dBarNum2 * dBarAs2);
			}
		}
	}
	// Change Cur Unit.
	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] *= (dLenCurFactor / dLenCodeFactor);
	}
	// Convert As by Current Unit.
	RbarData.dAsb *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAst *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAsc *= pow(dLenCurFactor / dLenCodeFactor, 2);

	return 0;
}


int CDgnDataCtrl::Check_RbarCivil4PlateRating(int iKind, int iThik, _DGNC_RBAR& RbarData)
{
	// iKind = 1:Beam, 2:Column/Brace.
	// Return Value (If 0 is Good, else as follows).
	//  1 : No good Code.
	//  2 : Not exist Section.
	//  3 : No good Section.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	T_DCON_D ConcData;
	ConcData.Initialize();
	Get_DgnConDcon(ConcData);

	//   UINT nCode = CDBLib::GetRatCode();
	//   CString strDesignCode = CDBLib::GetRcRatCodeName(nCode);
	// 
	//   double dCovering=0.0;
	//   // Create Rbar Data by Code Unit.
	//   if(strDesignCode== _T("KSCE-USD10") || strDesignCode==_T("KSCE-RAIL-USD11")  )
	//   {
	//     CngIndex.nBase_Length	= D_UNITSYS_LENGTH_INDEX_MM;	// mm.
	//     CngIndex.nBase_Force	= D_UNITSYS_FORCE_INDEX_N;		// N.
	//     CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
	// 
	//     dCovering = 40.0;  // 40 mm.
	//   }
	//   else	return 1;

	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
	CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
	CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);

	double dCovering = 40.0;  // 40 mm.

	// Get Length Factor by Code Unit.
	double dLenCodeFactor = 0.0;
	switch (CngIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM: { dLenCodeFactor = 10.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_CM: { dLenCodeFactor = 1.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_M: { dLenCodeFactor = 0.01; } break;
	case D_UNITSYS_LENGTH_INDEX_IN: { dLenCodeFactor = 1.0 / 2.54; } break;
	case D_UNITSYS_LENGTH_INDEX_FT: { dLenCodeFactor = 1.0 / 30.48; } break;
	default: ASSERT(0); break;
	}

	// Get Length Factor by Cur Unit.
	double dLenCurFactor = 0.0;
	switch (CurIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM: { dLenCurFactor = 10.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_CM: { dLenCurFactor = 1.0;  } break;
	case D_UNITSYS_LENGTH_INDEX_M: { dLenCurFactor = 0.01; } break;
	case D_UNITSYS_LENGTH_INDEX_IN: { dLenCurFactor = 1.0 / 2.54; } break;
	case D_UNITSYS_LENGTH_INDEX_FT: { dLenCurFactor = 1.0 / 30.48; } break;
	default: ASSERT(0); break;
	}

	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] /= (dLenCurFactor / dLenCodeFactor);
	}

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////
	// Get Section by Code Unit.
//   T_SECT_D sData;
//   sData.Initialize();
//  BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(RbarData.iSectNo, sData);

	//////////////////////////////////////////////
	// <Remember> Data�� �ް����� User Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data�� �ް����� User Unit����
	//////////////////////////////////////////////
//   if(!bGetOK)	return 2;

	int iSectShp = 7;
	int iSiteNum = 2;
	double dD[8] = { 0.0 };

	double dUnitWidth = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1000.0);	 // 1000 mm//Get_UnitWidthLength();

	double dThik = 0.0;
	T_THIK_D ThikD; ThikD.Initialize();
	m_pDoc->m_pAttrCtrl->GetThik(iThik, ThikD);
	m_pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThik);
	dD[0] = dThik;
	dD[1] = dUnitWidth;

	// Initialize Check Results.
	BOOL bCheckRebar[3][5];

	for (int i = 0; i < 5; i++)
	{
		RbarData.bCheckCovc[i] = TRUE;
		RbarData.bCheckCovt[i] = TRUE;
		RbarData.bCheckCovb[i] = TRUE;
		for (int j = 0; j < 3; j++)
		{
			RbarData.bCheckRebar[j][i] = TRUE;
			RbarData.bCheckSpace[j][i] = TRUE;

			bCheckRebar[j][i] = TRUE;
		}
	}
	// Check Rebar Spacing.
	if (iKind == 1)	// Beam.
	{
		CString strSubBarNa = RbarData.strSubBarNa;
		if (strSubBarNa == _T(""))	CDBLib::GetDefaultSubRebarSize(1, strSubBarNa);	// by Beam.

		if (RbarData.iBarLayt == 0 && RbarData.iBarLayb == 0)
		{// Layer�� ���? ���� �� 
			RbarData.bCheckRebar[0][0] = FALSE;
			RbarData.bCheckRebar[1][0] = FALSE;
			return 0;
		}

		double dSubBarDia = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strSubBarNa);
		for (int i = 0; i < 2; i++)	// 0:Top, 1:Bot.
		{
			int iBarLay = (i == 0 ? RbarData.iBarLayt : RbarData.iBarLayb);
			int nBrarLayer = (iBarLay == 0) ? 1 : iBarLay;
			for (int j = 0; j < nBrarLayer; j++)
			{
				int iBarNum = RbarData.iBarNum[i][j];
				double dDc = (i == 0 ? RbarData.dDt[j] : RbarData.dDb[j]);
				double dDc1 = (i == 0 ? RbarData.dDt[0] : RbarData.dDb[0]);
				CString strBarNa1 = RbarData.strBarNa1[i][j];
				CString strBarNa2 = RbarData.strBarNa2[i][j];

				if (iBarNum < 0)
				{
					RbarData.bCheckRebar[i][j] = FALSE;
					continue;
				}
				else if (iBarNum == 0 && dDc == 0.0 && strBarNa1 == _T(""))
				{ // ö�� ������ ������    True
					RbarData.bCheckRebar[i][j] = TRUE;
					bCheckRebar[i][j] = FALSE;

					if (i == 1)
					{  // ���ϴ� ���? ���� ���� �ɷ���.
						if (bCheckRebar[0][j] == FALSE && bCheckRebar[1][j] == FALSE)
						{
							RbarData.bCheckRebar[0][j] = FALSE;
							RbarData.bCheckRebar[1][j] = FALSE;
							continue;
						}
					}
					continue;
				}
				else if (iBarNum > 0 && dDc > 0.0 && strBarNa1 != _T(""))
				{// ���? �Էµ� ���� True
					RbarData.bCheckRebar[i][j] = TRUE;
					continue;
				}
				else
				{// �׿ܴ� False 
					RbarData.bCheckRebar[i][j] = FALSE;
					continue;
				}

				if (strBarNa1 == _T("")) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }

				if (dDc <= 0.0 && iBarNum <= 0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
					continue;
				}
				if (dDc < 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
					continue;
				}

				double dBarDia1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa1);
				double dBarDia2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa2) : dBarDia1);
				double dBarDia = max(dBarDia1, dBarDia2);
				if (min(dBarDia1, dBarDia2) <= 0.0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }

				double dBarDist = Get_BeamVDistRbar(dBarDia, ConcData.DesignCode);
				double dCompSpace = 0.0;

				//         if(ConcData.DesignCode==_T("SNiP 2.05.03-84*")|| ConcData.DesignCode==_T("SP 35.13330.2011") || ConcData.DesignCode==_T("SNiP 2.05.03-84*(MKS)")|| ConcData.DesignCode==_T("SP 35.13330.2011(MKS)")) 
				//         { // ���þ� ���ؿ��� ö�ٰ��� ���� ������.
				//           dCompSpace=0.0;
				//         }
				//         else
				{
					RbarData.bCheckSpace[i][j] = Check_BeamUseDist(iSectShp, i + 1, dD, iBarNum, dBarDia, dDc, dDc1, ConcData.DesignCode);

					if (j == 0)	dCompSpace = dDc - (dCovering + dSubBarDia + dBarDia / 2.0);
					else
					{
						double dDcp = (i == 0 ? RbarData.dDt[j - 1] : RbarData.dDb[j - 1]);
						dCompSpace = (dDc - dDcp) - dBarDist;
					}
				}
				if (dCompSpace < 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
				}
				// Calculate Ast,Asb (As for Beam).
				double dBarAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa1);
				double dBarAs2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa2) : dBarAs1);
				// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
				double dBarNum1 = iBarNum / 2.;
				double dBarNum2 = dBarNum1;
				if (i == 0)	RbarData.dAst += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Top.
				else			RbarData.dAsb += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Bot.
			}
		}

	}
	else ASSERT(0);

	// Change Cur Unit.
	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] *= (dLenCurFactor / dLenCodeFactor);
	}
	// Convert As by Current Unit.
	RbarData.dAsb *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAst *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAsc *= pow(dLenCurFactor / dLenCodeFactor, 2);

	return 0;
}
int CDgnDataCtrl::Check_RbarCivilRC(int iKind, int iPosi, _DGNC_RBAR& RbarData)
{
	// iKind = 1:Beam, 2:Column/Brace.
	// Return Value (If 0 is Good, else as follows).
	//  1 : No good Code.
	//  2 : Not exist Section.
	//  3 : No good Section.
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	T_DCON_D ConcData;
	ConcData.Initialize();
	Get_DgnConDcon(ConcData);

	const CString& strCode = ConcData.DesignCode;
	double dCovering = 0.0;
	// Create Rbar Data by Code Unit.
	if (strCode == CONCODE_KSCE_USD96 || strCode == CONCODE_KCI_USD99 || strCode == CONCODE_TWN_BRG_LSD90)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_CM;	// cm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KG;		// kgf.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 4.0;  // 4 cm.
	}
	else if (strCode == CONCODE_JTJ023_85 || strCode == CONCODE_CSA_S6_00 || strCode == CONCODE_CSA_S6_14 || strCode == CONCODE_CSA_S6_19
		|| strCode == CONCODE_IRC21_2000
		|| strCode == CONCODE_KCI_USD03 || strCode == CONCODE_KCI_USD07 || strCode == CONCODE_KSCE_USD05
		|| strCode == CONCODE_KSCE_RAIL_USD04)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		if (strCode == CONCODE_JTJ023_85)
		{
			dCovering = 25.0;  // 25 mm.
		}
		else
		{
			dCovering = 40.0;  // 40 mm.
		}
	}
	else if (strCode == CONCODE_AASHTO_LRFD02)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;	// in.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KIP;	// Kips.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 1.5;  // 1.5 in.
	}
	else if (strCode == CONCODE_ACI318_02 || strCode == CONCODE_ACI318_05 || strCode == CONCODE_AASHTO_LFD96 ||
		strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 || strCode == CONCODE_ACI318_14 ||
		strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318_25)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;	// in.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_LBF;	// lb.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 1.5;  // 1.5 in.
	}
	else if (strCode == CONCODE_ACI318M_19 || strCode == CONCODE_ACI318M_14 || strCode == CONCODE_ACI318M_25 ||
		strCode == CONCODE_NSR_10 || strCode == CONCODE_NSCP_2015 || 
		strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0; // 40 mm.
	}
	else if (strCode == CONCODE_EC2_2_05)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if ( strCode == CONCODE_BS5400_90 )
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0;  // 10 mm.
	}
	else if (strCode == CONCODE_TMH07_89)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 40.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IRC112_2020)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IRS)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else if (strCode == CONCODE_IS456_2000)
	{
		CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
		CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;		// N.
		CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CngIndex.nBase_Length, CngIndex.nBase_Force, TRUE);
		dCovering = 10.0;  // 10 mm.
	}
	else
	{
		return 1;
	}

	// Get Length Factor by Code Unit.
	double dLenCodeFactor = 0.0;
	if (CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)		dLenCodeFactor = 10.0;
	else if (CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)	dLenCodeFactor = 1.0;
	else if (CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)	dLenCodeFactor = 0.01;
	else if (CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)	dLenCodeFactor = 1. / 2.54;
	else if (CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)	dLenCodeFactor = 1. / 30.48;
	else	ASSERT(0);
	// Get Length Factor by Cur Unit.
	double dLenCurFactor = 0.0;
	if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)		dLenCurFactor = 10.0;
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)	dLenCurFactor = 1.0;
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)	dLenCurFactor = 0.01;
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)	dLenCurFactor = 1. / 2.54;
	else if (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)	dLenCurFactor = 1. / 30.48;
	else	ASSERT(0);

	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] /= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] /= (dLenCurFactor / dLenCodeFactor);
	}

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////
	// Get Section by Code Unit.
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(RbarData.iSectNo, sData);
	//////////////////////////////////////////////
	// <Remember> Data�� �ް����� User Unit����
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data�� �ް����� User Unit����
	//////////////////////////////////////////////
	if (!bGetOK)	return 2;

	int iSectShp = 0;
	int iSiteNum = 0;
	double dD[3][8];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			dD[i][j] = 0.0;

	if (sData.nStype == D_SECT_TYPE_REGULAR)
	{
		CString strShp = sData.SectBefore.Shape;
		if (strShp == _T("T")) { iSectShp = 3;	iSiteNum = 2; }
		else if (strShp == _T("B")) { iSectShp = 4;	iSiteNum = 2; }
		else if (strShp == _T("P")) { iSectShp = 5;	iSiteNum = 1; }
		else if (strShp == _T("SR")) { iSectShp = 6;	iSiteNum = 1; }
		else if (strShp == _T("SB")) { iSectShp = 7;	iSiteNum = 2; }
		else if (strShp == _T("OCT")) { iSectShp = 12;	iSiteNum = 3; }
		else if (strShp == _T("SOCT")) { iSectShp = 13;	iSiteNum = 3; }
		else if (strShp == _T("TRK")) { iSectShp = 14;	iSiteNum = 2; }
		else if (strShp == _T("STRK")) { iSectShp = 15;	iSiteNum = 2; }
		else if (strShp == _T("HTRK")) { iSectShp = 16;	iSiteNum = 3; }

		if (iKind == 1)	// Beam.
		{
			if (!(iSectShp == 3 || iSectShp == 7))	return 3;
		}
		else if (iKind == 2)	// Column/Brace.
		{
			if (!(iSectShp == 4 || iSectShp == 5 || iSectShp == 6 ||
				iSectShp == 7 || iSectShp == 12 || iSectShp == 13 ||
				iSectShp == 14 || iSectShp == 15 || iSectShp == 16))	return 3;
		}
		for (int i = 0; i < 3; i++)
		{
			dD[i][0] = sData.SectBefore.SectI.Size[0];
			dD[i][1] = sData.SectBefore.SectI.Size[1];
			dD[i][2] = sData.SectBefore.SectI.Size[2];
			dD[i][3] = sData.SectBefore.SectI.Size[3];
			dD[i][4] = sData.SectBefore.SectI.Size[4];
			dD[i][5] = sData.SectBefore.SectI.Size[5];
			dD[i][6] = sData.SectBefore.SectI.Size[6];
			dD[i][7] = sData.SectBefore.SectI.Size[7];
			if (iSectShp == 4)	// B.
			{
				if (dD[i][4] == 0.0)	dD[i][4] = dD[i][1] - dD[i][2];	// Cw = B1-tw.
				if (dD[i][5] == 0.0)	dD[i][5] = dD[i][2];				// Tf2 = Tf1.
				if (fabs(dD[i][4] - (dD[i][1] - dD[i][2])) > 1.0E-08)	return 3;
			}
		}
	}
	else if (sData.nStype == D_SECT_TYPE_TAPERED && sData.SectBefore.nStype != D_SECT_TYPE_USER)
	{
		CString strShp = sData.SectBefore.Shape;
		if (strShp == _T("T")) { iSectShp = 3;	iSiteNum = 2; }
		else if (strShp == _T("B")) { iSectShp = 4;	iSiteNum = 2; }
		else if (strShp == _T("P")) { iSectShp = 5;	iSiteNum = 1; }
		else if (strShp == _T("SR")) { iSectShp = 6;	iSiteNum = 1; }
		else if (strShp == _T("SB")) { iSectShp = 7;	iSiteNum = 2; }
		else if (strShp == _T("OCT")) { iSectShp = 12;	iSiteNum = 3; }
		else if (strShp == _T("SOCT")) { iSectShp = 13;	iSiteNum = 3; }
		else if (strShp == _T("TRK")) { iSectShp = 14;	iSiteNum = 2; }
		else if (strShp == _T("STRK")) { iSectShp = 15;	iSiteNum = 2; }
		else if (strShp == _T("HTRK")) { iSectShp = 16;	iSiteNum = 3; }

		if (iKind == 1)	// Beam.
		{
			if (!(iSectShp == 3 || iSectShp == 7))	return 3;
		}
		else if (iKind == 2)	// Column/Brace.
		{
			if (!(iSectShp == 4 || iSectShp == 5 || iSectShp == 6 ||
				iSectShp == 7 || iSectShp == 12 || iSectShp == 13 ||
				iSectShp == 14 || iSectShp == 15 || iSectShp == 16))	return 3;
		}
		// I.
		dD[0][0] = sData.SectBefore.SectI.Size[0];
		dD[0][1] = sData.SectBefore.SectI.Size[1];
		dD[0][2] = sData.SectBefore.SectI.Size[2];
		dD[0][3] = sData.SectBefore.SectI.Size[3];
		dD[0][4] = sData.SectBefore.SectI.Size[4];
		dD[0][5] = sData.SectBefore.SectI.Size[5];
		dD[0][6] = sData.SectBefore.SectI.Size[6];
		dD[0][7] = sData.SectBefore.SectI.Size[7];
		// J.
		dD[2][0] = sData.SectBefore.SectJ.Size[0];
		dD[2][1] = sData.SectBefore.SectJ.Size[1];
		dD[2][2] = sData.SectBefore.SectJ.Size[2];
		dD[2][3] = sData.SectBefore.SectJ.Size[3];
		dD[2][4] = sData.SectBefore.SectJ.Size[4];
		dD[2][5] = sData.SectBefore.SectJ.Size[5];
		dD[2][6] = sData.SectBefore.SectJ.Size[6];
		dD[2][7] = sData.SectBefore.SectJ.Size[7];
		// Mid.(I,J���� ���������? ����)    
		dD[1][0] = 0.5*(sData.SectBefore.SectI.Size[0] + sData.SectBefore.SectJ.Size[0]);
		dD[1][1] = 0.5*(sData.SectBefore.SectI.Size[1] + sData.SectBefore.SectJ.Size[1]);
		dD[1][2] = 0.5*(sData.SectBefore.SectI.Size[2] + sData.SectBefore.SectJ.Size[2]);
		dD[1][3] = 0.5*(sData.SectBefore.SectI.Size[3] + sData.SectBefore.SectJ.Size[3]);
		dD[1][4] = 0.5*(sData.SectBefore.SectI.Size[4] + sData.SectBefore.SectJ.Size[4]);
		dD[1][5] = 0.5*(sData.SectBefore.SectI.Size[5] + sData.SectBefore.SectJ.Size[5]);
		dD[1][6] = 0.5*(sData.SectBefore.SectI.Size[6] + sData.SectBefore.SectJ.Size[6]);
		dD[1][7] = 0.5*(sData.SectBefore.SectI.Size[7] + sData.SectBefore.SectJ.Size[7]);

		if (iSectShp == 4)	// B.
		{
			for (int i = 0; i < 3; i++)
			{
				if (dD[i][4] == 0.0)	dD[i][4] = dD[i][1] - dD[i][2];	// Cw = B1-tw.
				if (dD[i][5] == 0.0)	dD[i][5] = dD[i][2];					// Tf2 = Tf1.
				if (fabs(dD[i][4] - (dD[i][1] - dD[i][2])) > 1.0E-08)	return 3;
			}
		}
	}
	else ASSERT(0);

	double dDT[8];
	for (int k = 0; k < 8; k++)
		dDT[k] = dD[iPosi][k];

	// Initialize Check Results.
	for (int i = 0; i < 5; i++)
	{
		RbarData.bCheckCovc[i] = TRUE;
		RbarData.bCheckCovt[i] = TRUE;
		RbarData.bCheckCovb[i] = TRUE;
		for (int j = 0; j < 3; j++)
		{
			RbarData.bCheckRebar[j][i] = TRUE;
			RbarData.bCheckSpace[j][i] = TRUE;
		}
	}
	// Check Rebar Spacing.
	if (iKind == 1)	// Beam.
	{
		CString strSubBarNa = RbarData.strSubBarNa;
		if (strSubBarNa == _T(""))	CDBLib::GetDefaultSubRebarSize(1, strSubBarNa);	// by Beam.
		double dSubBarDia = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strSubBarNa);
		for (int i = 0; i < 2; i++)	// 0:Top, 1:Bot.
		{
			int iBarLay = (i == 0 ? RbarData.iBarLayt : RbarData.iBarLayb);
			for (int j = 0; j < iBarLay; j++)
			{
				int iBarNum = RbarData.iBarNum[i][j];
				double dDc = (i == 0 ? RbarData.dDt[j] : RbarData.dDb[j]);
				double dDc1 = (i == 0 ? RbarData.dDt[0] : RbarData.dDb[0]);
				CString strBarNa1 = RbarData.strBarNa1[i][j];
				CString strBarNa2 = RbarData.strBarNa2[i][j];
				if (iBarNum < 2) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				if (strBarNa1 == _T("")) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				if (dDc <= 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
					continue;
				}
				double dBarDia1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa1);
				double dBarDia2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa2) : dBarDia1);
				double dBarDia = max(dBarDia1, dBarDia2);
				if (min(dBarDia1, dBarDia2) <= 0.0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				RbarData.bCheckSpace[i][j] = Check_BeamUseDist(iSectShp, i + 1, dDT, iBarNum, dBarDia, dDc, dDc1, ConcData.DesignCode);
				double dBarDist = Get_BeamVDistRbar(dBarDia, ConcData.DesignCode);
				double dCompSpace = 0.0;
				if (j == 0)	dCompSpace = dDc - (dCovering + dSubBarDia + dBarDia / 2.0);
				else
				{
					double dDcp = (i == 0 ? RbarData.dDt[j - 1] : RbarData.dDb[j - 1]);
					dCompSpace = (dDc - dDcp) - dBarDist;
				}
				if (dCompSpace < 0.0)
				{
					if (i == 0)	RbarData.bCheckCovt[j] = FALSE;	// Top.
					else			RbarData.bCheckCovb[j] = FALSE;	// Bot.
				}
				// Calculate Ast,Asb (As for Beam).
				double dBarAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa1);
				double dBarAs2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa2) : dBarAs1);
				// Change by ZINU.('06.02.27). ex) 5(3EA + 2EA) -> 5(2.5EA + 2.5EA).
				double dBarNum1 = iBarNum / 2.;
				double dBarNum2 = dBarNum1;
				if (i == 0)	RbarData.dAst += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Top.
				else			RbarData.dAsb += dBarNum1 * dBarAs1 + dBarNum2 * dBarAs2;	// Bot.
			}
		}
	}
	else if (iKind == 2)	// Column, Brace.
	{
		CString strShp = sData.SectBefore.Shape;
		CString strSubBarNa = RbarData.strSubBarNa;
		if (strSubBarNa == _T(""))	CDBLib::GetDefaultSubRebarSize(2, strSubBarNa);	// by Column.
		double dSubBarDia = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strSubBarNa);
		for (int i = 0; i < iSiteNum; i++)
		{
			for (int j = 0; j < RbarData.iBarLayc; j++)
			{
				int iBarNum = RbarData.iBarNum[i][j];
				double dDc = RbarData.dDc[j];
				CString strBarNa1 = RbarData.strBarNa1[i][j];
				CString strBarNa2 = RbarData.strBarNa2[i][j];
				// Change by Jaeoh. (06.05.23)
				if (strShp != _T("HTRK"))
				{
					if (i % 2 == 0)
						if (iBarNum < 2) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
						else
							if (iBarNum < 0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				}
				else
				{
					if (i == 0)
						if (iBarNum < 2) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
						else
							if (iBarNum < 0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				}
				if (strBarNa1 == _T("")) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				if (dDc <= 0.0) { RbarData.bCheckCovc[j] = FALSE;	continue; }
				double dBarDia1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa1);
				double dBarDia2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarDia(strBarNa2) : dBarDia1);
				double dBarDia = max(dBarDia1, dBarDia2);
				if (min(dBarDia1, dBarDia2) <= 0.0) { RbarData.bCheckRebar[i][j] = FALSE;	continue; }
				RbarData.bCheckSpace[i][j] = Check_ColmUseDist(iSectShp, i + 1, dDT, iBarNum, dBarDia, dDc, ConcData.DesignCode, RbarData.iBarLayc);
				double dBarDist = Get_ColmDistRbar(dBarDia, ConcData.DesignCode, RbarData.iBarLayc);
				double dCompSpace = 0.0;
				if (j == 0)	dCompSpace = dDc - (dCovering + dSubBarDia + dBarDia / 2.0);
				else
				{
					double dDcp = RbarData.dDc[j - 1];
					dCompSpace = (dDc - dDcp) - dBarDist;
				}
				if (dCompSpace < 0.0)	RbarData.bCheckCovc[j] = FALSE;
				// Calculate Asc (As for Column).
				double dBarAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa1);
				double dBarAs2 = (strBarNa2 != _T("") ? CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(strBarNa2) : dBarAs1);
				// Change by ZINU.('06.02.27). ex) 5(2.5EA + 2.5EA) -> 5(3EA + 2EA).
				double dBarNum1 = ceil(iBarNum / 2.);
				double dBarNum2 = iBarNum - dBarNum1;
				RbarData.dAsc += Get_UsedAs(RbarData.iSectNo, i + 1, dBarNum1*dBarAs1 + dBarNum2 * dBarAs2);
			}
		}
	}
	// Change Cur Unit.
	for (int i = 0; i < 5; i++)
	{
		RbarData.dDt[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDb[i] *= (dLenCurFactor / dLenCodeFactor);
		RbarData.dDc[i] *= (dLenCurFactor / dLenCodeFactor);
	}
	// Convert As by Current Unit.
	RbarData.dAsb *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAst *= pow(dLenCurFactor / dLenCodeFactor, 2);
	RbarData.dAsc *= pow(dLenCurFactor / dLenCodeFactor, 2);

	return 0;
}

double CDgnDataCtrl::Get_UsedAs(int iSect, int iGrup, double dAsBar)
{
	double dAs = 0.0;
	// Get Section Shape.
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(iSect, sData);
	if (sData.nStype == D_SECT_TYPE_REGULAR)
	{
		CString strShp = sData.SectBefore.Shape;
		int iMagNum = 0;
		if (strShp == _T("H"))	iMagNum = 1;
		else if (strShp == _T("B"))	iMagNum = 2;
		else if (strShp == _T("P"))	iMagNum = 1;
		else if (strShp == _T("SR"))	iMagNum = 1;
		else if (strShp == _T("SB"))	iMagNum = 2;
		else if (strShp == _T("OCT"))	iMagNum = (iGrup == 2 ? 4 : 2);
		else if (strShp == _T("SOCT"))	iMagNum = (iGrup == 2 ? 4 : 2);
		else if (strShp == _T("TRK"))	iMagNum = 2;
		else if (strShp == _T("STRK"))	iMagNum = 2;
		else if (strShp == _T("HTRK"))	iMagNum = (iGrup == 1 ? 2 : 1);
		else	ASSERT(0);
		dAs = iMagNum * dAsBar;
	}
	return dAs;
}

BOOL CDgnDataCtrl::Get_SufcCivil(int iElem, T_SUFC_D& rData)
{
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistSufc(iElem))
	{
		m_pDoc->m_pAttrCtrl->GetSufc(iElem, rData);
		return TRUE;
	}
	else	return FALSE;
}

//(2009.06.11) Add by Unsang :: Only Used. EC2-2:05
BOOL CDgnDataCtrl::Get_UlecCivil(int iElem, T_ULEC_D& rData)
{
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistUlec(iElem))
	{
		m_pDoc->m_pAttrCtrl->GetUlec(iElem, rData);
		return TRUE;
	}
	else  return FALSE;
}

BOOL CDgnDataCtrl::Get_CTCfactor4RCcolumnCivil(int iElem, int iCode_Length, double& dfactor)
{
	dfactor = 1.0;
	T_RCCO_D rData;
	rData.Initialize();

	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int nLenID = CurIndex.nBase_Length;
	double dLen1meter = Lengthfor1Meter(nLenID);
	if (m_pDoc->m_pAttrCtrl->ExistRcco(iElem))
	{
		m_pDoc->m_pAttrCtrl->GetRcco(iElem, rData);
		if (rData.bCTC)
			dfactor = dLen1meter == 0.0 ? 1.0 : rData.dCTC / dLen1meter;
		else
			dfactor = 1.0;
	}
	else
	{
		dfactor = 1.0;
	}

	return TRUE;
}

BOOL CDgnDataCtrl::IsCPBEExsit(int nPosi, T_CPBE_D& CpbeD)
{
	BOOL bIsMbar = IsCPBEMbarExist(nPosi, CpbeD);
	BOOL bIsSbar = IsCPBESbarExist(nPosi, CpbeD);
	if (bIsMbar || bIsSbar) return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPBEMbarExist(int nPosi, T_CPBE_D& CpbeD)
{
	int j;
	for (j = 0; j < 5; ++j)
	{
		if (CpbeD.dDt[nPosi][j] != 0.0) return TRUE;
		if (CpbeD.dDb[nPosi][j] != 0.0) return TRUE;
		if (CpbeD.dTopBarNum[nPosi][j] != 0.0) return TRUE;
		if (CpbeD.dBotBarNum[nPosi][j] != 0.0) return TRUE;
		if (CpbeD.dTopBarCTC[nPosi][j] != 0.0) return TRUE;
		if (CpbeD.dBotBarCTC[nPosi][j] != 0.0) return TRUE;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPBESbarExist(int nPosi, T_CPBE_D& CpbeD)
{
	if (CpbeD.dSubBarNum[nPosi] != 0.0) return TRUE;
	if (CpbeD.dSubBarDist[nPosi] != 0.0) return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPCOExsit(int nPosi, T_CPCO_D& CpcoD)
{
	BOOL bIsMbar = IsCPCOMbarExist(nPosi, CpcoD);
	BOOL bIsSbar = IsCPCOSbarExist(nPosi, CpcoD);
	if (bIsMbar || bIsSbar) return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPCOMbarExist(int nPosi, T_CPCO_D& CpcoD)
{
	int i, j;
	for (i = 0; i < 5; ++i)
	{
		if (CpcoD.dDc[nPosi][i] != 0.0) return TRUE; // �ܸ� �ܰ����� �Ÿ�
		for (j = 0; j < 3; ++j)
		{
			if (CpcoD.iBarNum[nPosi][j][i] != 0.0) return TRUE; // ö�� ����       (Check : ���?(0 �̻�) )
		}
	}
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPCOSbarExist(int nPosi, T_CPCO_D& CpcoD)
{
	if (CpcoD.strSubBarNa[nPosi].IsEmpty()) return FALSE;
	if (CpcoD.dSubBarNum_y[nPosi] == 0.0) return FALSE;
	if (CpcoD.dSubBarNum_z[nPosi] == 0.0) return FALSE;
	if (CpcoD.dSubBarDist[nPosi] == 0.0) return FALSE;
	return TRUE;
}



BOOL CDgnDataCtrl::IsRCHKCOExsit(T_RCHK_COLM& RchkD)
{
	BOOL bIsMbar = IsRCHKCOMbarExist(RchkD);
	BOOL bIsSbar = IsRCHKCOSbarExist(RchkD);
	if (bIsMbar || bIsSbar) return TRUE;
	return FALSE;

}
BOOL CDgnDataCtrl::IsRCHKCOMbarExist(T_RCHK_COLM& RchkD)
{
	// Before_Release_v825_Check - Rebar ����ü ����Ǹ�? �����Ǿ��? �ϴ� �κ� 
	// �ϴ� 0 ���� �����صξ���~	

	int i, j;
	for (i = 0; i < 5; ++i)
	{
		if (RchkD.dDc[0][i] != 0.0) return TRUE; // �ܸ� �ܰ����� �Ÿ�
		for (j = 0; j < 3; ++j)
		{
			if (RchkD.iBarNum[0][j][i] != 0.0) return TRUE; // ö�� ����       (Check : ���?(0 �̻�) )
		}
	}
	return FALSE;
}
BOOL CDgnDataCtrl::IsRCHKCOSbarExist(T_RCHK_COLM& RchkD)
{
	if (RchkD.strSubBarNa[0].IsEmpty()) return FALSE;
	if (RchkD.dSubBarNum[0] == 0.0) return FALSE;
	if (RchkD.dSubBarNum_y[0] == 0.0) return FALSE;
	if (RchkD.dSubBarNum_z[0] == 0.0) return FALSE;
	if (RchkD.dSubBarDist[0] == 0.0) return FALSE;
	return TRUE;
}




BOOL CDgnDataCtrl::IsCPWAExsit(int nPosi, T_CPWA_D& CpwaD)
{
	BOOL bIsMbar = IsCPWAMbarExist(nPosi, CpwaD);
	BOOL bIsSbar = IsCPWASbarExist(nPosi, CpwaD);
	if (bIsMbar || bIsSbar) return TRUE;
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPWAMbarExist(int nPosi, T_CPWA_D& CpwaD)
{
	int j;
	for (j = 0; j < 5; ++j)
	{
		if (CpwaD.dDt[nPosi][j] != 0.0) return TRUE;
		if (CpwaD.dDb[nPosi][j] != 0.0) return TRUE;
		if (CpwaD.dTopBarNum[nPosi][j] != 0.0) return TRUE;
		if (CpwaD.dBotBarNum[nPosi][j] != 0.0) return TRUE;
		if (CpwaD.dTopBarCTC[nPosi][j] != 0.0) return TRUE;
		if (CpwaD.dBotBarCTC[nPosi][j] != 0.0) return TRUE;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::IsCPWASbarExist(int nPosi, T_CPWA_D& CpwaD)
{
	if (CpwaD.dSubBarNum[nPosi] != 0.0) return TRUE;
	if (CpwaD.dSubBarDist[nPosi] != 0.0) return TRUE;
	return FALSE;
}



// Coded by Seungjun MNet:No.2393 ('20060816)
BOOL CDgnDataCtrl::Get_DgnGenDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever, int nCode_Length, BOOL bServ/*TRUE*/)
{
	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	BOOL bSetUnit = CurIndex.nBase_Length != nCode_Length;
	if (bSetUnit)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	// Get the member data of element asigned a member.
	BOOL bReverseDir = FALSE;
	T_ELEM_K ElemShowKey = ElemKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	aElemList.RemoveAll();
	m_pMembCtrl->GetElemListByIncludeElem(ElemKey, bReverseDir, ElemShowKey, aElemList);

	// 1=column(����-1/100 �������?), 2=beam(����-1/100 �������?), 3=brace 0=No Frame.
	// D_MBTP_COLUMN  1
	// D_MBTP_BEAM    2
	// D_MBTP_BRACE   3
	T_MBTP_D MbtpData;
	MbtpData.Initialize();
	Get_DgnGenMbtp(ElemShowKey, MbtpData);

	BOOL bOK = TRUE;
	if (bServ) // Coded by Seungjun ('20070614) MNet:No.2313. bServ is for Eurocode 2 calculating Cmi0. 
	{
		if (MbtpData.nMbType == 1)       Get_DgnGenColmDisp(ElemKey, iLcomKind, LcomKey, Disp, iCantilever);
		else if (MbtpData.nMbType == 2)  Get_DgnGenBeamDisp(ElemKey, iLcomKind, LcomKey, Disp, iCantilever);
		else if (MbtpData.nMbType == 3)  Get_DgnGenBeamDisp(ElemKey, iLcomKind, LcomKey, Disp, iCantilever);//{  AfxMessageBox(_T("This element is the Brace!"), MB_OK); return FALSE; }
		//else if(MbtpData.nMbType == 0) {  AfxMessageBox(_T("This is not the Frame!"), MB_OK); return FALSE; }
		else { /*ASSERT(0);*/ bOK = FALSE; }
	}
	else
	{
		Get_DgnGenBeamDisp(ElemKey, iLcomKind, LcomKey, Disp, iCantilever, FALSE);
	}

	//////////////////////////////////////////////
	// <Remember> Data�� �ް����� User Unit����
	if (bSetUnit)
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	// <Remember> Data�� �ް����� User Unit����
	//////////////////////////////////////////////

	if (!bOK)  return FALSE;
	return TRUE;
}

// Coded by Seungjun MNet:No.2393 ('20060818)
BOOL CDgnDataCtrl::Get_DgnGenBeamDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever, BOOL bServ/*TRUE*/)
{	
	// Get the Load combination data.
	T_LCOM_D LcomData;
	m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, LcomKey, LcomData);

	// Get the member data of element asigned a member.
	BOOL bReverseDir = FALSE;
	T_ELEM_K ElemShowKey = ElemKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	aElemList.RemoveAll();
	m_pMembCtrl->GetElemListByIncludeElem(ElemKey, bReverseDir, ElemShowKey, aElemList);

	CArray<double, double> aDGZ;
	aDGZ.RemoveAll();
	CArray<double, double> aorgDLY;
	aorgDLY.RemoveAll();
	CArray<double, double> aorgDLZ;
	aorgDLZ.RemoveAll();
	
	Disp.Initialize();
	Disp.LcomKey = LcomKey;

	// Set reference point.
	T_NODE_K iEdgeNode[2] = { 0,0 };
	m_pMembCtrl->GetEdgeNodeAtMemb(aElemList[0], iEdgeNode);
	Disp.RefPointKey = iEdgeNode[0];

	int nSupport = 0;
	if (iCantilever > 0)
	{
		BOOL bFix[6] = { FALSE };
		m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(iEdgeNode[0], bFix);
		if ( !bFix[0] && !bFix[1] && !bFix[2] && !bFix[3] && !bFix[4] && !bFix[5] )
		{
			nSupport = iEdgeNode[1];
		}
		else
		{
			nSupport = iEdgeNode[0];
		}
	}

	if(!CPostCtrl::IsPossibleBeamDetailAnalysisExactSolution(LcomData))
	{
		return TRUE;
	}

	const bool bSupportEndI = (nSupport != 0 && nSupport != Disp.RefPointKey) ? false : true;

	int nCount = aElemList.GetSize();
	// Reverse Direction Member�� ���? ó������ ��Һ���? ���������� ������ �����Ƿ� ������ �ٲ����? �ȹٲ����? ã�Ƽ� ó��.
	CArray<BOOL, BOOL> arbInverse;
	arbInverse.RemoveAll();
	T_NODE_K NextK = 0;
	for (int m = 0; m < nCount; m++)
	{
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(aElemList[m], ElemD);
		if (m == 0)
		{
			if (ElemD.elnod[0] == Disp.RefPointKey)
			{
				arbInverse.Add(FALSE);
				NextK = ElemD.elnod[1];
			}
			else
			{
				arbInverse.Add(TRUE);
				NextK = ElemD.elnod[0];
			}
		}
		else
		{
			if (ElemD.elnod[0] == NextK)
			{
				arbInverse.Add(FALSE);
				NextK = ElemD.elnod[1];
			}
			else
			{
				arbInverse.Add(TRUE);
				NextK = ElemD.elnod[0];
			}
		}
	}


	int iDiv = 0;
	if (iCantilever > 0) iDiv = Div_Num_Cantilever;
	else                iDiv = Div_Num;

	int iPosi[3] = { 0 };

	CArray<double, double> aDistRatio;
	if (iCantilever > 0)
	{
		Get_DistRatioAlongMemberCantilever(bSupportEndI, aElemList, iDiv, aDistRatio);
	}
	else
	{
		Get_DistRatioAlongMember(aElemList, iDiv, aDistRatio); 
	}

	// Get the CBeamDiagram instance for getting the diplacment of elements.
	CBeamDiagram BeamDiagram;

	// Find Maximum and Minimum displacements of Member.
	for (int i = 0; i < aElemList.GetSize(); i++)
	{
		T_ELEM_K ElemK = aElemList.GetAt(i);

		// Get the displacement of each element.
		if (!BeamDiagram.MakeBeamDispl(ElemK, LcomData, iDiv, GLOBAL_DISP)) return FALSE;

		// Store the displacement.
		if (!arbInverse[i])
		{
			for (int j = 0; j < iDiv; j++)
			{
				aDGZ.Add(BeamDiagram.m_dDG[j][2]);  // Global Z
				aorgDLY.Add(BeamDiagram.m_dDL[2][j]);  // Global Z
				aorgDLZ.Add(BeamDiagram.m_dDL[3][j]);  // Global Z
			}
		}
		else
		{
			for (int j = iDiv; j > 0; j--)
			{
				aDGZ.Add(BeamDiagram.m_dDG[j][2]);  // Global Z
				aorgDLY.Add(BeamDiagram.m_dDL[2][j]);  // Global Z
				aorgDLZ.Add(BeamDiagram.m_dDL[3][j]);  // Global Z
			}
		}
	}
	if (!arbInverse[nCount - 1])
	{
		aDGZ.Add(BeamDiagram.m_dDG[iDiv][2]);  // Global Z
		aorgDLY.Add(BeamDiagram.m_dDL[2][iDiv]);  // Global Z
		aorgDLZ.Add(BeamDiagram.m_dDL[3][iDiv]);  // Global Z
	}
	else
	{
		aDGZ.Add(BeamDiagram.m_dDG[0][2]);  // Global Z
		aorgDLY.Add(BeamDiagram.m_dDL[2][0]);  // Global Z
		aorgDLZ.Add(BeamDiagram.m_dDL[3][0]);  // Global Z
	}

	// cantilever�� ���? �׻� �������� ���� ó�� ���?.
	if ( !bSupportEndI )
	{
		CArray<double, double> aTempDGZ, aTempDLY, aTempDLZ;
		aTempDGZ.RemoveAll();
		aTempDLY.RemoveAll();
		aTempDLZ.RemoveAll();
		int nNum = aDGZ.GetSize();

		for (int n = nNum - 1; n >= 0; n--)
		{
			aTempDGZ.Add(aDGZ[n]);
			aTempDLY.Add(aorgDLY[n]);
			aTempDLZ.Add(aorgDLZ[n]);
		}
		aDGZ.RemoveAll();
		aorgDLY.RemoveAll();
		aorgDLZ.RemoveAll();
		aDGZ.Copy(aTempDGZ);
		aorgDLY.Copy(aTempDLY);
		aorgDLZ.Copy(aTempDLZ);
	}

	// Coded by Seungjun ('20070612) MNet:No.2313.
	if (!bServ)
	{
		double dTempy = 0.0, dTempz = 0.0;
		int iCount = aorgDLY.GetSize();
		for (int k = 0; k < iCount; k++)
		{
			if (fabs(dTempy) <= fabs(aorgDLY.GetAt(k)))
				dTempy = aorgDLY.GetAt(k);
			if (fabs(dTempz) <= fabs(aorgDLZ.GetAt(k)))
				dTempz = aorgDLZ.GetAt(k);
		}
		Disp.MaxDispY = dTempy;
		Disp.MaxDispZ = dTempz;

		return TRUE;
	}

	// Get Local Displacement difference.
	CArray<double, double> aDLZ;
	aDLZ.RemoveAll();
	int iCount = aDGZ.GetSize();

	if (iCantilever > 0)
	{
		// ������ ó���� 0�� ���? boundary�� �����ִ� �ɷ� �Ǵ��Ͽ� ���� �������? ����.
		if (fabs(aDGZ.GetAt(0)) < 1e-10) // 1e-10���� �������? 0.0���� ����. ó������ tolerance.
		{
			for (int k = 0; k < iCount; k++) aDLZ.Add(0.0);
		}
		else
			Get_DgnGenLocalDispCantilever(aDGZ.GetAt(0), aDGZ.GetAt(1), aDistRatio, aDLZ);
	}
	else
	{
		Get_DgnGenLocalDisp(aDGZ.GetAt(0), aDGZ.GetAt(iCount - 1), aDistRatio, aDLZ);
	}

	// For Debugging.
	if (m_IsTestMode)
	{
		rptwofstream fout(m_strTestFile, ios::app);
		fout << _T("Div.No      Rel.Z          Abs.Z        Dif.Z") << endl
			<< _T("===============================================") << endl;
	}

	// Calculate Displacement.
	for (int i = 0; i < aDGZ.GetSize(); i++)
	{
		double dZ = aDGZ[i];

		// Trangfer relative displacement to absolute displacement.
		aDGZ[i] -= aDLZ[i];  // Global Z

		// Find the maximum displcement and position.
		if (fabs(Disp.MaxDispZ) < fabs(aDGZ[i])) { Disp.MaxDispZ = aDGZ[i]; iPosi[2] = i; }  // Global Z

		// For Debugging.
		if (m_IsTestMode)
		{
			CString  strTemp = _T("");
			strTemp.Format(_T(" %5d  -  %10lf, %10lf, %10lf"), i, dZ, aDGZ[i], aDLZ[i]);
			rptwofstream fout(m_strTestFile, ios::app);
			fout << strTemp << endl;
		}
	}

	// Calculate distance from reference point of the maximum displacement position.
	double dLength = m_pMembCtrl->GetMembLengByIncludeElem(ElemKey);
	if (iCantilever > 0)
	{
		Disp.MaxDistZ = aDistRatio[iPosi[2]] / aDistRatio[aDistRatio.GetSize() - 1] * dLength;
	}
	else
	{
		Disp.MaxDistZ = aDistRatio[iPosi[2]] * dLength;
	}

	if ( !bSupportEndI )
	{
		Disp.MaxDistZ = dLength - Disp.MaxDistZ;
	}

	return TRUE;
}

// Coded by Seungjun MNet:No.2393 ('20060818)
BOOL CDgnDataCtrl::Get_DgnGenColmDisp(T_ELEM_K ElemKey, int iLcomKind, T_LCOM_K LcomKey, _DGN_DISP& Disp, int iCantilever)
{
	// Get the member data of element asigned a member.
	BOOL bReverseDir = FALSE;
	T_ELEM_K ElemShowKey = ElemKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	aElemList.RemoveAll();
	m_pMembCtrl->GetElemListByIncludeElem(ElemKey, bReverseDir, ElemShowKey, aElemList);

	CArray<UINT, UINT> aNode;
	aNode.RemoveAll();
	auto nCount = aElemList.GetSize();
	for(int i=0; i<nCount; ++i)
	{
		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aElemList[i], ElemD)) continue;
		aNode.Add(ElemD.elnod[0]);
		aNode.Add(ElemD.elnod[1]);
	}
	CArrayUtil::RemoveDup(aNode);
	nCount = aNode.GetSize();

	double dMax_GX[2] = {-DBL_MAX, DBL_MAX}; // Max - Global X. {max, min}
	double dMin_GX[2] = {-DBL_MAX, DBL_MAX}; // Min - Global X. {max, min}
	double dMax_GY[2] = {-DBL_MAX, DBL_MAX}; // Max - Global Y. {max, min}
	double dMin_GY[2] = {-DBL_MAX, DBL_MAX}; // Min - Global Y. {max, min}

	// Get the Load combination data.
	T_LCOM_D LcomData;
	LcomData.Initialize();
	m_pDoc->m_pAttrCtrl->GetLcom(iLcomKind, LcomKey, LcomData);

	// Set LoadCombination.
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
	for(int i=0; i<nCount; ++i)
	{
		T_DISP_D DispMax, DispMin;
		DispMax.Initialize(); DispMin.Initialize();
		m_pDoc->m_pPostCtrl->GetDispNew(aNode[i], &DispMax, &DispMin, NULL);

		// DispMax - Global X
		dMax_GX[0] = max(dMax_GX[0], DispMax.dblDisp[0]);
		dMax_GX[1] = min(dMax_GX[1], DispMax.dblDisp[0]);
		// DispMin - Global X
		dMin_GX[0] = max(dMin_GX[0], DispMin.dblDisp[0]);
		dMin_GX[1] = min(dMin_GX[1], DispMin.dblDisp[0]);
		// DispMax - Global Y
		dMax_GY[0] = max(dMax_GY[0], DispMax.dblDisp[1]);
		dMax_GY[1] = min(dMax_GY[1], DispMax.dblDisp[1]);
		// DispMin - Global Y
		dMin_GY[0] = max(dMin_GY[0], DispMin.dblDisp[1]);
		dMin_GY[1] = min(dMin_GY[1], DispMin.dblDisp[1]);
	}

	double dMaxX = fabs(dMax_GX[0]-dMax_GX[1]); // Max - Global X.
	double dMinX = fabs(dMin_GX[0]-dMin_GX[1]); // Min - Global X. 
	double dMaxY = fabs(dMax_GY[0]-dMax_GY[1]); // Max - Global Y. 
	double dMinY = fabs(dMin_GY[0]-dMin_GY[1]); // Min - Global Y.

	Disp.Initialize();
	Disp.LcomKey = LcomKey;
	Disp.MaxDispX = max(dMaxX, dMinX);
	Disp.MaxDispY = max(dMaxY, dMinY);

	return TRUE;
}

void CDgnDataCtrl::Get_DgnGenLocalDispCantilever(double dDisp1, double dDisp2, const CArray<double, double>& aDistRatio, OUT CArray<double, double>& raLocalDisp)
{
	raLocalDisp.RemoveAll();
	int nSize = aDistRatio.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		double dLocal = dDisp1 + (dDisp2 - dDisp1)*aDistRatio[i];
		raLocalDisp.Add(dLocal);
	}
}

void CDgnDataCtrl::Get_DgnGenLocalDisp(double dDisp1, double dDisp2, const CArray<double, double>& aDistRatio, OUT CArray<double, double>& raLocalDisp)
{
	raLocalDisp.RemoveAll();
	int nSize = aDistRatio.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		double dLocal = dDisp1 + (dDisp2 - dDisp1)*aDistRatio[i];
		raLocalDisp.Add(dLocal);
	}
}

void CDgnDataCtrl::Get_DistRatioAlongMemberCantilever(bool bSupportEndI, const T_ELEM_K_LIST& aElemK, int nDiv4Elem, OUT CArray<double, double>& raDistRatio)
{
	raDistRatio.RemoveAll();
	raDistRatio.Add(0.0);

	const int nElem = aElemK.GetSize();
	double dL0 = 0.0;
	if ( bSupportEndI )
	{
		/// ù��° �����? segment�� ���̸� 1�� ����, ������ segment ������ �Ÿ��� ����Ѵ�?.
		dL0 = m_pDoc->calcLAVElem(aElemK[0]) / nDiv4Elem;

		double dSumLeng = 0.0;
		for ( int i = 0; i < nElem; ++i )
		{
			double dLeng = m_pDoc->calcLAVElem(aElemK[i]);
			for ( int m = 0; m < nDiv4Elem; ++m )
			{
				raDistRatio.Add(dLeng / nDiv4Elem * (m + 1) + dSumLeng);
			}
			dSumLeng += dLeng;
		}
	}
	else
	{
		dL0 = m_pDoc->calcLAVElem(aElemK[nElem-1]) / nDiv4Elem;

		double dSumLeng = 0.0;
		for ( int i = nElem-1; i >= 0; --i)
		{
			double dLeng = m_pDoc->calcLAVElem(aElemK[i]);
			for ( int m = 0; m < nDiv4Elem; ++m )
			{
				raDistRatio.Add(dLeng / nDiv4Elem * (m + 1) + dSumLeng);
			}
			dSumLeng += dLeng;
		}
	}
	
	int nSize = raDistRatio.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		raDistRatio.SetAt(i, raDistRatio[i] / dL0);
	}
}

void CDgnDataCtrl::Get_DistRatioAlongMember(const T_ELEM_K_LIST& aElemK, int nDiv4Elem, OUT CArray<double, double>& raDistRatio)
{
	raDistRatio.RemoveAll();
	raDistRatio.Add(0.0);

	double dSumLeng = 0.0;
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		double dLeng = m_pDoc->calcLAVElem(aElemK[i]);
		for (int m = 0; m < nDiv4Elem; ++m)
		{
			raDistRatio.Add(dLeng / nDiv4Elem * (m + 1) + dSumLeng);
		}
		dSumLeng += dLeng;
	}

	int nSize = raDistRatio.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		raDistRatio.SetAt(i, raDistRatio[i] / dSumLeng);
	}
}


// Coded by Seungjun MNet:No.2393 ('20061110)
double CDgnDataCtrl::Get_DgnConEc(double dfc, CString strMatlDB/*_T("")*/, CString strMatlNa/*_T("")*/)
{
	const CString& strConCode = CDBLib::GetConCodeName();

	double dUp3 = 1.0 / 3.0;
	double dUp03 = 0.30;

	double dEc = 0.0;
	if (strConCode == CONCODE_ACI318_89 || strConCode == CONCODE_ACI318_95 || strConCode == CONCODE_ACI318_99 ||
		strConCode == CONCODE_ACI318_02 || strConCode == CONCODE_ACI318_05 || strConCode == CONCODE_ACI318_08 ||
		strConCode == CONCODE_ACI318_11 || strConCode == CONCODE_ACI318_14 || strConCode == CONCODE_ACI318_19 ||
		strConCode == CONCODE_ACI318_25)
	{
		dEc = 57000.0*sqrt(dfc);
	}
	else if (strConCode == CONCODE_ACI318M_14 || strConCode == CONCODE_ACI318M_19 || strConCode == CONCODE_ACI318M_25 ||
		strConCode == CONCODE_NSR_10 || strConCode == CONCODE_NSCP_2015)
	{
		dEc = 4700.0*sqrt(dfc);
	}
	else if (strConCode == CONCODE_AIK_USD94 ||
		strConCode == CONCODE_TWN_USD92 || strConCode == CONCODE_TWN_USD100 || strConCode == CONCODE_TWN_USD112)
	{
		dEc = 15000.0*sqrt(dfc);	// See TWN-USD92 1.9.1 p.1-3.
	}
	else if (strConCode == CONCODE_KSCE_USD96 || strConCode == CONCODE_KCI_USD99)
	{
		dEc = dgn::LE(dfc, 300.0) ? 15000.0*sqrt(dfc) : 10500.0*sqrt(dfc) + 70000.0;
	}
	else if (strConCode == CONCODE_KCI_USD03)	// See KCI-USD03 3.4.3 p.66~67.
	{
		dEc = dgn::LE(dfc, 30.0) ? 4700.0*sqrt(dfc) : 3300.0*sqrt(dfc) + 7700.0;
	}
	else if (strConCode == CONCODE_KCI_USD07)
	{
		const double dfcu = dfc + 8.0;
		dEc = 8500.0*pow(dfcu, 1.0 / 3.0);
	}
	else if (strConCode == CONCODE_KCI_USD12 || strConCode == CONCODE_KDS_41_30_2018 || strConCode == CONCODE_KDS_41_20_2022)
	{
		double dDeltaf = 0.0;
		if (dfc < 40.0 + cDgn_Zero)
		{
			dDeltaf = 4.0;
		}
		else if (dfc < 60.0)
		{
			dDeltaf = 4.0 + (dfc - 40.0) / 10.0;
		}
		else
		{
			dDeltaf = 6.0;
		}

		double dfcu = dfc + dDeltaf;  // (MPa).
		dEc = 8500.0 * pow(dfcu, 1.0 / 3.0);
	}
	else if (strConCode == CONCODE_AIK_WSD2K)
	{
		if (dfc < 300.0 + cDgn_Zero)
		{
			dEc = 1.4E+05;
		}
		else
		{
			dEc = 10500.0*sqrt(dfc) + 70000.0;
		}
	}
	else if (strConCode == CONCODE_AIJ_WSD99)
	{
		// Concrete Type.
		int iConcType = 0;	// 0=Normal, 1=1��, 2=2��.
		if (strMatlDB != MATLCODE_CON_JIS)	    iConcType = 0;
		else if (strMatlNa.Find(_T("(LT1)")) >= 0)	iConcType = 1;
		else if (strMatlNa.Find(_T("(LT2)")) >= 0)	iConcType = 2;
		else									iConcType = 0;

		double dWc = 0.0;
		// Change by ZINU.('04.11.12). Add Light Weight Concrete.
		// See AIJ-WSD99 3�� 7��2 p.7
		if (iConcType == 1)	// 1��.
		{
			if (dfc < 27.0 + cDgn_Zero)		dWc = 20.0 - 1.0;
			else if (dfc < 36.0 + cDgn_Zero)	dWc = 22.0 - 1.0;
			else							dWc = 22.0 - 1.0;
		}
		else if (iConcType == 2)	// 2��.
		{
			dWc = 18.0 - 1.0;
		}
		else	// Normal Concrete.
		{
			if (dfc < 36.0 + cDgn_Zero)		dWc = 24.0 - 1.0;
			else if (dfc < 48.0 + cDgn_Zero)	dWc = 24.5 - 1.0;
			else if (dfc < 60.0 + cDgn_Zero)	dWc = 25.0 - 1.0;
			else							dWc = 25.0 - 1.0;
		}
		dEc = 3.35E+04*pow(dWc / 24., 2)*pow(dfc / 60., dUp3);
	}
	else if ( strConCode == CONCODE_BS5400_90 )
	{
		if ( dfc < 20.0 + cDgn_Zero )	    dEc = 25000.0;
		else if ( dfc < 25.0 + cDgn_Zero )	dEc = 26000.0;
		else if ( dfc < 30.0 + cDgn_Zero )	dEc = 28000.0;
		else if ( dfc < 40.0 + cDgn_Zero )	dEc = 31000.0;
		else if ( dfc < 50.0 + cDgn_Zero )	dEc = 34000.0;
		else if ( dfc < 60.0 + cDgn_Zero )	dEc = 36000.0;
		else                                dEc = 36000.0;
	}
	else if (strConCode == CONCODE_TMH07_89)
	{
		if (dfc < 20.0 + cDgn_Zero)	    dEc = 25000.0;
		else if (dfc < 25.0 + cDgn_Zero)	dEc = 26000.0;
		else if (dfc < 30.0 + cDgn_Zero)	dEc = 28000.0;
		else if (dfc < 40.0 + cDgn_Zero)	dEc = 31000.0;
		else if (dfc < 50.0 + cDgn_Zero)	dEc = 34000.0;
		else if (dfc < 60.0 + cDgn_Zero)	dEc = 36000.0;
		else                                dEc = 36000.0;
	}
	else if (strConCode == CONCODE_BS8110_97)
	{
		dEc = 5.5E+03*sqrt(dfc / 1.5);	// See BS8110-97 Figure 2.1 p.8 (cubic).
	}
	else if (strConCode == CONCODE_EC2)
	{
		dEc = 9.5E+03*pow(dfc + 8.0, dUp3);
	}
	else if (strConCode == CONCODE_EC2_04)
	{
		dEc = 9.5E+03*pow(dfc + 8., dUp3); // Coded by Seungjun ('20070613) MNet:No.2313.
	}
	else if (strConCode == CONCODE_EC2_2_05)
	{
		dEc = 2.2E+04*pow((dfc + 8) / 10.0, dUp03);
	}
	else if (strConCode == CONCODE_IRC112_2011 || strConCode == CONCODE_IRC112_2020)
	{
		dEc = 2.2E+04*pow((dfc + 10) / 12.5, dUp03);
	}
	else if (strConCode == CONCODE_IS456_2000)
	{
		dEc = 5.0E+03*sqrt(dfc);	// See IS456:2000 6.2.3.1 p.16 (cubic).
	}
	else if (strConCode == CONCODE_GB50010_02 || strConCode == CONCODE_GB50010_10)
	{
		// See GB50010-02 4.1.3 p.17 (Table). = See [GB/T50010-10, 4.1.5, Table 4.1.5]
		// Add by Park Bong('05.10.24). Primarily, Data by Table.
		// Modify by GAY. ('06.10.27). Consider Unit Factor (10^4 N/mm^2).
		if (dfc > 15.0 - cDgn_Zero &&
			dfc < 20.0 - cDgn_Zero)	dEc = 2.20 + (2.55 - 2.20)*(dfc - 15.0) / 5.0;
		else if (dfc < 25.0 - cDgn_Zero)	dEc = 2.55 + (2.80 - 2.55)*(dfc - 20.0) / 5.0;
		else if (dfc < 30.0 - cDgn_Zero)	dEc = 2.80 + (3.00 - 2.80)*(dfc - 25.0) / 5.0;
		else if (dfc < 35.0 - cDgn_Zero)	dEc = 3.00 + (3.15 - 3.00)*(dfc - 30.0) / 5.0;
		else if (dfc < 40.0 - cDgn_Zero)	dEc = 3.15 + (3.25 - 3.15)*(dfc - 35.0) / 5.0;
		else if (dfc < 45.0 - cDgn_Zero)	dEc = 3.25 + (3.35 - 3.25)*(dfc - 40.0) / 5.0;
		else if (dfc < 50.0 - cDgn_Zero)	dEc = 3.35 + (3.45 - 3.35)*(dfc - 45.0) / 5.0;
		else if (dfc < 55.0 - cDgn_Zero)	dEc = 3.45 + (3.55 - 3.45)*(dfc - 50.0) / 5.0;
		else if (dfc < 60.0 - cDgn_Zero)	dEc = 3.55 + (3.60 - 3.55)*(dfc - 55.0) / 5.0;
		else if (dfc < 65.0 - cDgn_Zero)	dEc = 3.60 + (3.65 - 3.60)*(dfc - 60.0) / 5.0;
		else if (dfc < 70.0 - cDgn_Zero)	dEc = 3.65 + (3.70 - 3.65)*(dfc - 65.0) / 5.0;
		else if (dfc < 75.0 - cDgn_Zero)	dEc = 3.70 + (3.75 - 3.70)*(dfc - 70.0) / 5.0;
		else if (dfc < 80.0 + cDgn_Zero)	dEc = 3.75 + (3.80 - 3.75)*(dfc - 75.0) / 5.0;
		else                          dEc = 10.0 / (2.2 + 34.7 / dfc);	// cubic.

		dEc *= 1.0E+04;
	}
	else if (strConCode == CONCODE_CSA_A23_3_94)
	{
		dEc = 4.5E+03*sqrt(dfc);	// See CSA-A23.3-94 8.6.2.3 p.20. 
	}
	else ASSERT(0);

	return dEc;
}

// Coded by Seungjun MNet:No.2393 ('20061121)
// Crack Width Default Value
BOOL CDgnDataCtrl::Get_DgnConCrackDef(const CString &strCode, int nSubCode, double* pdCrack)
{
	int iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_M;
	BOOL bGetOK = TRUE;
	for (int i = 0; i < 4; i++)
	{
		pdCrack[i] = 0.0;
	}

	// Gen.	
	if (strCode == CONCODE_EC2)
	{
		pdCrack[0] = 0.10;
		pdCrack[1] = 0.20;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.40;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if (strCode == CONCODE_EC2_04)
	{
		if (nSubCode == dgn::def::enNationalAnnex::Singapore) // by GAY. PMS:4817. ('14.02.17). add Singapore annex.
		{
			// Coded by Seungjun ('20070613) MNet:No.2313.
			pdCrack[0] = 0.10;
			pdCrack[1] = 0.20;
			pdCrack[2] = 0.20; // quasi-permanant limit.
			pdCrack[3] = 0.30; // frequent limit.
			iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
		}
		else
		{
			// Coded by Seungjun ('20070613) MNet:No.2313.
			pdCrack[0] = 0.10;
			pdCrack[1] = 0.20;
			pdCrack[2] = 0.30;
			pdCrack[3] = 0.40;
			iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
		}
	}
	else if (strCode == CONCODE_EC2_2_05)
	{
		pdCrack[0] = 0.10;
		pdCrack[1] = 0.20;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.40;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if ( strCode == CONCODE_BS5400_90 )
	{
		pdCrack[0] = 0.10;
		pdCrack[1] = 0.20;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.40;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if (strCode == CONCODE_TMH07_89)
	{
		pdCrack[0] = 0.10;
		pdCrack[1] = 0.20;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.40;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IRC112_2020)
	{
		pdCrack[0] = 0.10;
		pdCrack[1] = 0.20;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.40;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if (strCode == CONCODE_IS456_2000)
	{
		pdCrack[0] = 0.30;
		pdCrack[1] = 0.30;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.30;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else if (strCode == CONCODE_IRS)
	{
		pdCrack[0] = 0.30;
		pdCrack[1] = 0.30;
		pdCrack[2] = 0.30;
		pdCrack[3] = 0.30;
		iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	}
	else { ASSERT(0);	bGetOK = FALSE; }

	double dFlen = Change_Flen(iCodeLenUnit);
	for (int i = 0; i < 4; i++)
	{
		pdCrack[i] /= dFlen;
	}

	return bGetOK;
}

// Coded by Seungjun MNet:No.2393 ('20061121)
double CDgnDataCtrl::Change_Flen(int iLenID)
{
	// Get Current Lenth Unit.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	double dFlen1 = 0.0;
	switch (CurIndex.nBase_Length)
	{
	case(D_UNITSYS_LENGTH_INDEX_M): {dFlen1 = 100.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_CM): {dFlen1 = 1.0;			break; }
	case(D_UNITSYS_LENGTH_INDEX_MM): {dFlen1 = 1.0 / 10.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_IN): {dFlen1 = 2.54;			break; }
	case(D_UNITSYS_LENGTH_INDEX_FT): {dFlen1 = 2.54*12.0;	break; }
	}
	double dFlen2 = 0.0;
	switch (iLenID)
	{
	case(D_UNITSYS_LENGTH_INDEX_M): {dFlen2 = 100.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_CM): {dFlen2 = 1.0;			break; }
	case(D_UNITSYS_LENGTH_INDEX_MM): {dFlen2 = 1.0 / 10.0;		break; }
	case(D_UNITSYS_LENGTH_INDEX_IN): {dFlen2 = 2.54;			break; }
	case(D_UNITSYS_LENGTH_INDEX_FT): {dFlen2 = 2.54*12.0;	break; }
	}

	return dgn::SafeDiv(dFlen1, dFlen2);
}

double CDgnDataCtrl::Change_Fstr(int iLenID, int iForID)
{
	double dFln2 = Change_Fln2(iLenID);
	double dFfor = Change_Ffor(iForID);
	return dgn::SafeDiv(dFfor, dFln2);
}

double CDgnDataCtrl::Lengthfor1Meter(int iLenID)
{
	double dFlen = 0.0;
	switch (iLenID)
	{
	case(D_UNITSYS_LENGTH_INDEX_M): {dFlen = 1.0;			    break; }
	case(D_UNITSYS_LENGTH_INDEX_CM): {dFlen = 100.0;		        break; }
	case(D_UNITSYS_LENGTH_INDEX_MM): {dFlen = 1000.0;	        break; }
	case(D_UNITSYS_LENGTH_INDEX_IN): {dFlen = 100.0 / 2.54;	    break; }
	case(D_UNITSYS_LENGTH_INDEX_FT): {dFlen = 100.0 / (2.54*12.0);	break; }
	}
	return dFlen;
}

double CDgnDataCtrl::Change_Ffor(int iForID)
{
	// Get Current Lenth Unit.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	double dFfor = 0.0;
	// 1 lb = 4.448223 N, 1 kgf = 9.80665 N
	// 1 lb = 0.453592511 (kgf) = 4.448223/9.80665 �̹Ƿ�
	double dFfor1 = 0.0;
	switch (CurIndex.nBase_Force)
	{
	case(D_UNITSYS_FORCE_INDEX_KG): {dFfor1 = 1.0;							break; }
	case(D_UNITSYS_FORCE_INDEX_TON): {dFfor1 = 1000.0;						break; }
	case(D_UNITSYS_FORCE_INDEX_N): {dFfor1 = 1.0 / 9.80665;			break; }
	case(D_UNITSYS_FORCE_INDEX_KN): {dFfor1 = 1000.0 / 9.80665;		break; }
	case(D_UNITSYS_FORCE_INDEX_LBF): {dFfor1 = 4.448223 / 9.80665;	break; }
	case(D_UNITSYS_FORCE_INDEX_KIP): {dFfor1 = 4448.223 / 9.80665;	break; }
	}
	double dFfor2 = 0.0;
	switch (iForID)
	{
	case(D_UNITSYS_FORCE_INDEX_KG): {dFfor2 = 1.0;							break; }
	case(D_UNITSYS_FORCE_INDEX_TON): {dFfor2 = 1000.0;						break; }
	case(D_UNITSYS_FORCE_INDEX_N): {dFfor2 = 1.0 / 9.80665;			break; }
	case(D_UNITSYS_FORCE_INDEX_KN): {dFfor2 = 1000.0 / 9.80665;		break; }
	case(D_UNITSYS_FORCE_INDEX_LBF): {dFfor2 = 4.448223 / 9.80665;	break; }
	case(D_UNITSYS_FORCE_INDEX_KIP): {dFfor2 = 4448.223 / 9.80665;	break; }
	}
	dFfor = dFfor1 / dFfor2;

	return dFfor;
}

double CDgnDataCtrl::Change_Fln2(int iLenID)
{
	return pow(Change_Flen(iLenID), 2);
}

// Coded by Seungjun MNet:No.2591 ('20061211)	For SCWB Design.
BOOL CDgnDataCtrl::IsSCWBDesign() { return (m_pDoc->GetDesignStatus() == 8 ? TRUE : FALSE); }

// Coded by Seungjun MNet:No.2591 ('20061201)	For SCWB Design.
BOOL CDgnDataCtrl::IsRBeamOrColumn(T_ELEM_K ElemK, int& iMBType)
{
	// Check the Element type.
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))                 return FALSE;
	if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp))                   return FALSE;

	// Check Material.
	T_MATD_D MatdD;
	MatdD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD))	    return FALSE;
	if (MatdD.Type != _T("C"))                                           return FALSE;

	// Check if the element is column.
	iMBType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
	if (iMBType == D_MBTP_COLUMN || iMBType == D_MBTP_BEAM)          return TRUE;

	return FALSE;
}

// Coded by Seungjun MNet:No.2393 ('20060831)
double CDgnDataCtrl::GetDeflectLimByCode(int iMembType, int& iDeflectType)
{
	const CString& strStlCode = CDBLib::GetStlCodeName();

	double dDeflectLim = 0.0;
	if (strStlCode == STLCODE_EC3 || strStlCode == STLCODE_EC3_05 || strStlCode == STLCODE_EC3_2_05)
	{
		dDeflectLim = GetDeflectLimByCode_EC3(iMembType, iDeflectType);
	}
	else if (strStlCode == STLCODE_TWN_ASD96 || strStlCode == STLCODE_TWN_LSD96)
	{
		// PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
		dDeflectLim = GetDeflectLimByCode_TWN96(iMembType, iDeflectType);
	}
	else if (strStlCode == STLCODE_KSSC_LSD09 || strStlCode == STLCODE_KSSC_LSD16 ||
		strStlCode == STLCODE_KDS_41_31_2019 )
	{
		dDeflectLim = GetDeflectLimByCode_KSSC09(iMembType, iDeflectType);
	}
	else if (strStlCode == STLCODE_KDS_41_30_10_2022 || strStlCode ==  STLCODE_SP_16_13330_2017 ||
		     strStlCode == STLCODE_NSCP_2015_LRFD    || strStlCode == STLCODE_NSCP_2015_ASD)
	{
		dDeflectLim = GetDeflectLimByCode_KDS2022(iMembType, iDeflectType);
	}
	else if (strStlCode == STLCODE_KDS_24_14_31_2018)
	{
		dDeflectLim = GetDeflectLimByCode_KSSC09(iMembType, iDeflectType);
	}
	else if (
		strStlCode == STLCODE_AISC_LRFD05 || strStlCode == STLCODE_AISC_ASD05 ||
		strStlCode == STLCODE_AISC_LRFD10 || strStlCode == STLCODE_AISC_ASD10 ||
		strStlCode == STLCODE_AISC_LRFD16 || strStlCode == STLCODE_AISC_ASD16 ||
		strStlCode == STLCODE_AISC_LRFD22 || strStlCode == STLCODE_AISC_ASD22 )
	{
		dDeflectLim = GetDeflectLimByCode_AISC05(iMembType, iDeflectType);
	}
	else if (strStlCode == STLCODE_GB50017_17 || strStlCode == STLCODE_JGJ209_2010)
	{
		dDeflectLim = GetDeflectLimByCode_GB17(iMembType, iDeflectType);
	}
	else if ( strStlCode == STLCODE_IS800_2007 )
	{
		dDeflectLim = GetDeflectLimByCode_IS800( iMembType, iDeflectType );
	}
	else if (strStlCode == STLCODE_IRC_24_2010) {
		dDeflectLim = GetDeflectLimByCode_IRC24( iMembType, iDeflectType );
	}
	else
	{
		if ( m_pDoc->EnableJUD() )
		{
			dDeflectLim = 250.0;
			if ( iDeflectType != 0 ) iDeflectType = 1;
		}
	}

	return dDeflectLim;
}

double CDgnDataCtrl::GetDeflectLimByCode4ColdFormed(int iMembType, int& iDeflectType)
{
	const CString& strCfsCode = CDBLib::GetCfsCodeName();

	CArray<double, double> aDefLims;
	double dDeflectLim = 0.0;

	if (strCfsCode == CFSCODE_EC3_06 || ICFSteelDgnTool::IsCFSteelDgnCode_CH(strCfsCode))
	{
		int nSubCode = CDBLib::GetCfsSubCode();
		if (nSubCode == 1) // Singapore.
		{
			switch (iMembType)
			{
			case D_MBTP_BEAM:
			case D_MBTP_BRACE:
			{
				aDefLims.Add(200.0);
				aDefLims.Add(360.0);
				aDefLims.Add(180.0);
			}
			break;
			case D_MBTP_COLUMN:
			{
				aDefLims.Add(300.0);
			}
			break;
			default: ASSERT(0);
				break;
			}
		}
		else
		{
			switch (iMembType)
			{
			case D_MBTP_BEAM:
			case D_MBTP_BRACE:
			{
				aDefLims.Add(400.0);
				aDefLims.Add(250.0);
				aDefLims.Add(200.0);
			}
			break;
			case D_MBTP_COLUMN:
			{
				aDefLims.Add(500.0);
				aDefLims.Add(300.0);
				aDefLims.Add(150.0);
			}
			break;
			default: ASSERT(0);
				break;
			}
		}

		if (iDeflectType >= aDefLims.GetSize())
		{
			iDeflectType = aDefLims.GetSize() - 1;
		}
		dDeflectLim = aDefLims.GetAt(iDeflectType);
	}

	return dDeflectLim;
}


// Coded by SeungJun '20081013
double CDgnDataCtrl::Get_SCWBfactor()
{
	T_DCON_D DconD;
	double dFact = 1.2;
	if (Get_DgnConDcon(DconD))
	{
		if (DconD.DesignCode == CONCODE_EC2_04)
		{
			dFact = DconD.dStorColmWeakBeamFactor < cDgn_Zero ? 1.2 : DconD.dStorColmWeakBeamFactor;
		}
		else if (DconD.DesignCode == _T("IS456:2000"))
		{

			dFact = DconD.dStorColmWeakBeamFactor < cDgn_Zero ? 1.4 : DconD.dStorColmWeakBeamFactor;
		}
	}
	return dFact;
}

BOOL CDgnDataCtrl::IsConsiderTopStorForConSCWB()
{
	T_DCON_D DconD;
	if (Get_DgnConDcon(DconD))
	{
		return DconD.bStrongColmWeakBeam;
	}
	return FALSE;
}

BOOL CDgnDataCtrl::IsConsiderTopStorForStlSCWB()
{
	T_DSTL_D DstlD;
	if (Get_DgnStlDstl(DstlD))
	{
		return DstlD.bStrongColmWeakBeam;
	}
	return FALSE;
}

 // added by rahul gives the element Connected to node number
int CDgnDataCtrl::GetHorizontalElem_On_Node(T_ELEM_K ElemK, CArray<T_ELEM_K, T_ELEM_K>& arTop, CArray<T_ELEM_K, T_ELEM_K>& arBottom,T_NODE_K &TopNode ,T_NODE_K &BottomNode, CArray<double , double>& TAyzJunction, CArray<double, double> & BAyzJunction, CArray<T_MATD_D, T_MATD_D> &MatlArrayTop, CArray<T_MATD_D, T_MATD_D> &MatlArrayBottom, CArray<CString, CString> &SectTypeTop, CArray<CString, CString> &SectTypeBottom) {
	CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
	T_DSTL_D DstlD;
	if (!m_pDoc->m_pAttrCtrl->GetDstl(DstlD)) DstlD.Initialize();
	if (!DstlD.bSpecialSeismic) return FALSE;

	T_NODE_K iEdgeNode[2] = { 0 };
	pMembCtrl->GetEdgeNodeAtMemb(ElemK, iEdgeNode);
	 
	T_NODE_K NodeK1 = iEdgeNode[0];//Bottom Node
	T_NODE_K NodeK2 = iEdgeNode[1];//Top Node

	T_NODE_D NodeDK1;
	NodeDK1.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeDK1);

	T_NODE_D NodeDK2;
	NodeDK2.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeDK2);


	T_NODE_K TempNode;


	if(NodeDK2.z<NodeDK1.z){//j<i
		TempNode = NodeK1;
		NodeK1 = NodeK2;
		NodeK2 = TempNode;
	}

	TopNode = NodeK2;
	BottomNode = NodeK1;

	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(TopNode, arElemList);
	for (int IEL = 0; IEL < arElemList.GetSize(); IEL++) {
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		T_ELEM_D elemD; elemD.Initialize();
		T_MATL_D matlD; matlD.Initialize();
		T_SECT_D rData; rData.Initialize();
		T_MATD_D MatlData; MatlData.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);
		m_pDoc->m_pAttrCtrl->GetSect(elemD.elpro, rData);
		m_pDoc->m_pAttrCtrl->GetMatl(elemD.elmat, matlD);
		Get_DgnGenMatd(KeyElem, MatlData);
		MatlArrayTop.Add(MatlData);
		int iCBM = m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_BEAM) { arTop.Add(KeyElem); 
		T_NODE_K iBeamNode[2] = { 0 };
		pMembCtrl->GetEdgeNodeAtMemb(KeyElem, iBeamNode);
			if (iBeamNode[0] == TopNode) {
				TAyzJunction.Add(rData.SectBefore.SectI.Stiffness.Asz);
			}
			else {
				TAyzJunction.Add(rData.SectBefore.SectJ.Stiffness.Asz);
			} 

			SectTypeTop.Add(rData.SectBefore.Shape);
		}
	}

	m_pDoc->m_pAttrCtrl->GetConnectedElem(BottomNode, arElemList);

	for (int IEL = 0; IEL < arElemList.GetSize(); IEL++) {
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		T_ELEM_D elemD; elemD.Initialize();
		T_MATD_D MatlData; MatlData.Initialize();
		T_SECT_D rData; rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);
		m_pDoc->m_pAttrCtrl->GetSect(elemD.elpro, rData);
		//m_pDoc->m_pAttrCtrl->GetMatl(elemD.elmat, matlD);
		Get_DgnGenMatd(KeyElem, MatlData);
		MatlArrayBottom.Add(MatlData);
		int iCBM = m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_BEAM) { arBottom.Add(KeyElem); 
			T_NODE_K iBeamNode[2] = { 0 };
			pMembCtrl->GetEdgeNodeAtMemb(KeyElem, iBeamNode);
			if (iBeamNode[0] == BottomNode) {
				BAyzJunction.Add(rData.SectBefore.SectI.Stiffness.Asz);
			}
			else {
				BAyzJunction.Add(rData.SectBefore.SectJ.Stiffness.Asz);
			}

			SectTypeBottom.Add(rData.SectBefore.Shape);
		}
	}
	return TRUE;
}


// added by rahul gives the element Connected Section detail
int CDgnDataCtrl::GetSectionAttachedtoElem(T_ELEM_K ElemK, CArray<T_ELEM_K, T_ELEM_K>& arTop, CArray<T_ELEM_K, T_ELEM_K>& arBottom, T_NODE_K &TopNode, T_NODE_K &BottomNode, CArray<CString, CString> &SectTypeTop, CArray<CString, CString> &SectTypeBottom, CArray<T_SECT_SECTION_D, T_SECT_SECTION_D> &SectTop, CArray<T_SECT_SECTION_D, T_SECT_SECTION_D> &SectBottom, CArray<T_MATD_D, T_MATD_D> &MatlArrayTop, CArray<T_MATD_D, T_MATD_D> &MatlArrayBottom) {
	CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
	T_DSTL_D DstlD;
	if (!m_pDoc->m_pAttrCtrl->GetDstl(DstlD)) DstlD.Initialize();
	if (!DstlD.bSpecialSeismic) return FALSE;

	T_NODE_K iEdgeNode[2] = { 0 };
	pMembCtrl->GetEdgeNodeAtMemb(ElemK, iEdgeNode);

	T_NODE_K NodeK1 = iEdgeNode[0];//Bottom Node
	T_NODE_K NodeK2 = iEdgeNode[1];//Top Node

	T_NODE_D NodeDK1;
	NodeDK1.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeDK1);

	T_NODE_D NodeDK2;
	NodeDK2.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeDK2);


	T_NODE_K TempNode;


	if (NodeDK2.z < NodeDK1.z) {//j<i
		TempNode = NodeK1;
		NodeK1 = NodeK2;
		NodeK2 = TempNode;
	}

	TopNode = NodeK2;
	BottomNode = NodeK1;

	CArray<T_ELEM_K, T_ELEM_K> arElemList;
	m_pDoc->m_pAttrCtrl->GetConnectedElem(TopNode, arElemList);
	for (int IEL = 0; IEL < arElemList.GetSize(); IEL++) {
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		T_ELEM_D elemD; elemD.Initialize();
		T_MATL_D matlD; matlD.Initialize();
		T_SECT_D rData; rData.Initialize();
		T_MATD_D MatlData; MatlData.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);
		m_pDoc->m_pAttrCtrl->GetSect(elemD.elpro, rData);
		m_pDoc->m_pAttrCtrl->GetMatl(elemD.elmat, matlD);
		Get_DgnGenMatd(KeyElem, MatlData);
		MatlArrayTop.Add(MatlData);
		int iCBM = m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_BEAM) {
			arTop.Add(KeyElem);
			T_NODE_K iBeamNode[2] = { 0 };
			pMembCtrl->GetEdgeNodeAtMemb(KeyElem, iBeamNode);
			SectTypeTop.Add(rData.SectBefore.Shape);
			SectTop.Add(rData.SectBefore);
		}
	}

	m_pDoc->m_pAttrCtrl->GetConnectedElem(BottomNode, arElemList);

	for (int IEL = 0; IEL < arElemList.GetSize(); IEL++) {
		T_ELEM_K KeyElem = arElemList.GetAt(IEL);
		T_ELEM_D elemD; elemD.Initialize();
		T_MATD_D MatlData; MatlData.Initialize();
		T_SECT_D rData; rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, elemD);
		m_pDoc->m_pAttrCtrl->GetSect(elemD.elpro, rData);
		//m_pDoc->m_pAttrCtrl->GetMatl(elemD.elmat, matlD);
		Get_DgnGenMatd(KeyElem, MatlData);
		MatlArrayBottom.Add(MatlData);
		int iCBM = m_pDoc->m_pAttrCtrl->GetMemberType(KeyElem);
		if (iCBM == D_MBTP_BEAM) {
			arBottom.Add(KeyElem);
			T_NODE_K iBeamNode[2] = { 0 };
			pMembCtrl->GetEdgeNodeAtMemb(KeyElem, iBeamNode);
			SectTypeBottom.Add(rData.SectBefore.Shape);
			SectBottom.Add(rData.SectBefore);
		}
	}
	return TRUE;
}

// PMS:4020-Seungjun-20090713 EC2:04 Slab/Wall Deisng
int CDgnDataCtrl::Get_Node_in_Elem(T_ELEM_D& ElemD)
{
	int nNodeNum = 0;
	for (int i = 0; i < D_ELEM_MAXNOD; i++)
	{
		if (ElemD.elnod[i] == 0) break;
		nNodeNum++;
	}

	return nNodeNum;
}

// PMS:4020-Seungjun-20090818 EC2:04 Slab/Wall Design.
BOOL CDgnDataCtrl::Get_DgnConRbms(T_ELEM_K ElemK, T_RBMS_D& RbmsD, int nCode_Length)
{
	RbmsD.Initialize();

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	CCurUnitSaver unisaver(true);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length != nCode_Length)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	BOOL bExist = TRUE;
	ASSERT(0);
	int nLayer = 0; // 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2
	if (!m_pDoc->m_pAttrCtrl->GetRbmsByElemK(ElemK, nLayer, RbmsD))
	{
		bExist = FALSE;
	}

	return bExist;
}

BOOL CDgnDataCtrl::Get_DgnConRbms(T_ELEM_K ElemK, CArray<T_RBMS_D, T_RBMS_D&>& aRbmsD, int nCode_Length)
{
	T_RBMS_D RbmsD;
	aRbmsD.RemoveAll();
	aRbmsD.SetSize(4);

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	CCurUnitSaver unisaver(true);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length != nCode_Length)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	BOOL bExist = TRUE;
	// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2
	for (int i = 0; i < 4; i++)
	{
		aRbmsD[i].Initialize();
		if (m_pDoc->m_pAttrCtrl->GetRbmsByElemK(ElemK, i, RbmsD))
		{
			aRbmsD[i] = RbmsD;
		}
		else
		{
			bExist = FALSE;
		}
	}

	return bExist;
}

BOOL CDgnDataCtrl::Get_DgnConRbst(T_MSTR_K MstrK, CArray<T_RBST_D, T_RBST_D&>& aRbstD, int nCode_Length)
{
	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	CCurUnitSaver unisaver(true);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length != nCode_Length)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	aRbstD.RemoveAll();
	aRbstD.SetSize(2);

	T_MSTR_D MstrD;
	if (!m_pDoc->m_pAttrCtrl2->GetMstr(MstrK, MstrD)) { return FALSE; }

	T_ELEM_K ElemK_1st;
	T_MEMB_D MembD;
	BOOL bExist = TRUE;
	for (int i = 0; i < 2; ++i) // 0:Top, 1:Bottom
	{
		T_RBST_D &RbstD = aRbstD[i];
		RbstD.Initialize();

		if (!m_pDoc->m_pAttrCtrl->GetMemb(MstrD.MembK, MembD)) { ASSERT(0); continue; }
		ElemK_1st = MembD.aElemList[0];
		if (!m_pDoc->m_pAttrCtrl2->GetRbstByElemK(ElemK_1st, i, RbstD))
		{
			bExist = FALSE;
		}
	}

	return bExist;
}

BOOL CDgnDataCtrl::Get_DgnConRbsl(T_ELEM_K ElemK, OUT CArray<T_RBSL_D, T_RBSL_D&>& raRbslD, int nCode_Length)
{
	raRbslD.RemoveAll();
	raRbslD.SetSize(4);

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	CCurUnitSaver unisaver(true);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length != nCode_Length)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	T_RBSL_D RbslD;
	BOOL bExist = TRUE;
	// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2
	for (int i = 0; i < 4; i++)
	{
		raRbslD[i].Initialize();
		if (m_pDoc->m_pAttrCtrl->GetRbslByElemK(ElemK, i, RbslD))
		{
			raRbslD[i] = RbslD;
		}
		else
		{
			bExist = FALSE;
		}
	}

	return bExist;
}

BOOL CDgnDataCtrl::Get_DgnConRbmw(T_ELEM_K ElemK, T_RBMW_D& RbmwD, int nCode_Length)
{
	RbmwD.Initialize();

	//////////////////////////////////////////////
	// <Remember> Data�� �޾ƿö��� Code Unit����
	CCurUnitSaver unisaver(true);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	if(CurIndex.nBase_Length != nCode_Length)
	{
		CngIndex.nBase_Length = nCode_Length;
		CngIndex.nBase_Force = CurIndex.nBase_Force;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	}
	// <Remember> Data�� �޾ƿö��� Code Unit����
	//////////////////////////////////////////////

	BOOL bExist = TRUE;
	if (!m_pDoc->m_pAttrCtrl->GetRbmwByElemK(ElemK, RbmwD))
	{
		bExist = FALSE;
	}

	return bExist;
}

BOOL CDgnDataCtrl::Get_DgnConServ(T_ELEM_K iElemNo, T_SERV_D& ServD)
{
	ServD.Initialize();

	if (!m_pDoc->m_pAttrCtrl->GetServ(iElemNo, ServD))
	{
		// Set Default.
		T_DCON_D rData;
		Get_DgnConDcon(rData);
		const CString& strCode = rData.DesignCode;
		if (strCode == CONCODE_EC2_04 || strCode == CONCODE_EC2 || strCode == CONCODE_EC2_2_05)
		{
			ServD.iStressEnv = 5;     // Exposure class               (default :0 boundary 0 ~ 17)

			const int iSubCode = CDBLib::GetConSubCode();

#if defined _CIVIL
			switch (iSubCode)
			{
			case enNationalAnnex::Italy:
			{
				// PMS:4145-Seungjun-20100218 national annex ����.
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 0.8, 0.9)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 0.9)
				ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 0.9)
				ServD.dStressk[3] = 0.9;  // Stress parameter             (default : 0.6, 0.45, 0.8, 0.9)
			}
			break;
			case enNationalAnnex::Recommended:
			case enNationalAnnex::France:
			{
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			}
			break;
			case enNationalAnnex::Sweden:
			{
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[2] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
			}
			break;
			case enNationalAnnex::Singapore:
			{
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			}
			break;
			default:
			{
				ASSERT(0);
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			}
			break;
			}

			ServD.dRH = 0.0;            // Relative Humidity            (default : 0 boundary 0 ~ 100
			ServD.iCrackEnv = 0;        // Crack Environment            (default : 0 boundary 0 ~ 4)

			if (iSubCode == enNationalAnnex::Singapore) // Singapore
			{
				// See [SS EN1992-1-1, 7.3.1(5) -> Table NA.4]
				ServD.bCrackFlag[0] = FALSE;//Crack user defined           (default : FALSE)
				ServD.bCrackFlag[1] = FALSE;//Crack user defined           (default : FALSE)
				ServD.bCrackFlag[2] = TRUE; //Crack user defined           (default : FALSE)

				ServD.dCrackLim[0] = 0.0;// Crack user defined           (default : 0 boundary >0)
				ServD.dCrackLim[1] = 0.2;// Crack user defined           (default : 0 boundary >0) - frequent load combination.
				ServD.dCrackLim[2] = 0.3;// Crack user defined           (default : 0 boundary >0) - quasi-permanant load combination.
			}
			else
			{
				ServD.bCrackFlag[0] = FALSE;//Crack user defined           (default : FALSE)
				ServD.bCrackFlag[1] = FALSE;//Crack user defined           (default : FALSE)
				ServD.bCrackFlag[2] = TRUE; //Crack user defined           (default : FALSE)

				ServD.dCrackLim[0] = 0.0;// Crack user defined           (default : 0 boundary >0)
				ServD.dCrackLim[1] = 0.4;// Crack user defined           (default : 0 boundary >0)
				ServD.dCrackLim[2] = 0.3;// Crack user defined           (default : 0 boundary >0)
			}

			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 250.;     // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 250.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)

			ServD.iServLife = 2;         // Crack Service Life          (default : 2 boundary 0 ~ 2)
			ServD.bHighCorrosion = FALSE;     // Crack Corrosion Sencitivity (default : FALSE           )
#else
			// EN 1992-1-1:2004 7.2 (2) k1 for stress limitation
			// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
			// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
			switch (iSubCode)
			{
				case dgn::def::enNationalAnnex::Belgium:
				{
					if(5 <= ServD.iStressEnv && ServD.iStressEnv <= 14) ServD.dStressk[0] = 0.5;
					else                                                ServD.dStressk[0] = 0.6;
				}
				break;
				case dgn::def::enNationalAnnex::Poland:	ServD.dStressk[0] = 1.0; break;
				default:								ServD.dStressk[0] = 0.6; break; // recommended
			}
			
			// EN 1992-1-1:2004 7.2 (3) k2 for stress limitation
			ServD.dStressk[1] = 0.45; 
			
			// EN 1992-1-1:2004 7.2 (5) k3, k4 for stress limitation
			switch (iSubCode)
			{
			case dgn::def::enNationalAnnex::Netherlands:
			{
				ServD.dStressk[2] = 0.0;
				ServD.dStressk[3] = 0.0;
			}
			break;
			case dgn::def::enNationalAnnex::Finland:
			{
				ServD.dStressk[2] = 0.6;
				ServD.dStressk[3] = 0.8;
			}
			break;
			case dgn::def::enNationalAnnex::France:
			case dgn::def::enNationalAnnex::Germany:
			{
				ServD.dStressk[2] = 0.8;
				ServD.dStressk[3] = 1.0;
			}
			break;
			case dgn::def::enNationalAnnex::Italy:
			{
				ServD.dStressk[2] = 0.8;
				ServD.dStressk[3] = 0.9;
			}
			break;
			case dgn::def::enNationalAnnex::Sweden:
			case dgn::def::enNationalAnnex::Sweden19:
			{
				ServD.dStressk[2] = 1.0;
				ServD.dStressk[3] = 1.0;
			}
			break;
			default:
			{
				ServD.dStressk[2] = 0.8;
				ServD.dStressk[3] = 1.0;
			}
			break;
			}

			ServD.dRH = 0.0;            // Relative Humidity            (default : 0 boundary 0 ~ 100
			ServD.iCrackEnv = 5;        // Crack Environment            (default : 0 boundary 0 ~ 4)
			ServD.iServLife = 2;        // Crack Service Life          (default : 2 boundary 0 ~ 2)
			ServD.bHighCorrosion = FALSE;     // Crack Corrosion Sencitivity (default : FALSE           )

			EN_EC2_EXPOSURE_CLASS enExClass  = static_cast<EN_EC2_EXPOSURE_CLASS>(ServD.iCrackEnv);
			EN_EC2_SWE_SERV_LIFE  enServLife = static_cast<EN_EC2_SWE_SERV_LIFE>(ServD.iServLife);
			T_CRACK_EC2 CrackD;
			Get_CrackData_EN04(iSubCode, enExClass, enServLife, ServD.bHighCorrosion, CrackD);
			for(int k=0; k<EM_LCOM_NUM; ++k)
			{
				ServD.bCrackFlag[k] = CrackD.bChkFlag[k];
				ServD.dCrackLim[k]  = CrackD.dCrkLim[k];
			}

			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 250.;     // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 250.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)
#endif
		}
		else if (strCode == CONCODE_ACI318_05 || strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
			strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 ||
			strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
			strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
			strCode == CONCODE_NSR_10 ||
			strCode == CONCODE_KCI_USD12 || strCode == CONCODE_KDS_41_30_2018 || strCode == CONCODE_KDS_41_20_2022 ||
			strCode == CONCODE_NSCP_2015 ||
			strCode == CONCODE_TWN_USD112)
		{
			ServD.iDeflecEnv = 1;
			ServD.dDAF = 1.0;
			ServD.dDefLim = 360.0;

			ServD.iDeflecEnv_long = 0;
			ServD.dDefLim_long = 480.0;
			ServD.iTimeFactor = 0;
			ServD.dXi = 2.0;
			ServD.dReductFactor = 0.5;
		}
		else if (strCode == CONCODE_NTC_DCEC_2017 || strCode == CONCODE_NTC_DCEC_2023)
		{
			ServD.iDeflecEnv = 1;
			ServD.dDAF = 1.0;
			ServD.dDefLim = 240.0;

			ServD.iDeflecEnv_long = 0;
			ServD.dDefLim_long = 500.0;
		}
		else if (strCode == CONCODE_IRC112_2011 || strCode == CONCODE_IS456_2000 || strCode == CONCODE_IRC112_2020)
		{
			ServD.iStressEnv = 1;     // Exposure class               (default :0 boundary 0 ~ 17)

			ServD.dStressk[0] = 0.48;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)

			ServD.dRH = 0.0;            // Relative Humidity            (default : 0 boundary 0 ~ 100
			ServD.iCrackEnv = 0;        // Crack Environment            (default : 0 boundary 0 ~ 4)
			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 250.;     // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 250.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)

			ServD.iServLife = 2;         // Crack Service Life          (default : 2 boundary 0 ~ 2)
			ServD.bHighCorrosion = FALSE;     // Crack Corrosion Sencitivity (default : FALSE           )
#ifdef _MGEN
			ServD.dCrackLim[0] = 0.3;	// 0.3 mm
			ServD.bCrackFlag[0] = TRUE;
#endif
		}
		else if (strCode == CONCODE_AIJ_WSD99)
		{
			ServD.dDefLim = 250.0;
			ServD.dDAF = 16.0;
			ServD.dK = 0.56;
		}
		else if (strCode == CONCODE_AASHTO_LRFD24 || strCode == CONCODE_AASHTO_LRFD20 ||
			strCode == CONCODE_AASHTO_LRFD12 || strCode == CONCODE_AASHTO_LRFD16 || strCode == CONCODE_AASHTO_LRFD17 || strCode == CONCODE_AASHTO_LRFD02 ||
			strCode == CONCODE_AASHTO_LRFD07 || strCode == CONCODE_AASHTO_LRFD14)
		{
		}
		else if (strCode == CONCODE_GB50010_10 || strCode == CONCODE_GB50010_19)
		{
		}
		else if ( strCode == CONCODE_BS5400_90 )
		{
			//TODO BS�ڵ� Ȯ�� �ʿ�.
			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 250.;     // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 250.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)
		}
		else if (strCode == CONCODE_TMH07_89)
		{
			//TODO BS�ڵ� Ȯ�� �ʿ�.
			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 250.;     // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 250.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)
		}
		else if(strCode == CONCODE_CSA_S6_00 || strCode == CONCODE_CSA_S6_14 || strCode == CONCODE_CSA_S6_19)
		{
			// Do nothing.
		}
		else if (strCode == CONCODE_AREMA_2023)// Rahul AREMA
		{
			// Do nothing.
		}
		else if (strCode == CONCODE_AS5100_5_17)
		{
			DO_NOTHING();
		}
		else
		{
			ASSERT(0);
		}
	}

	return TRUE;
}

BOOL CDgnDataCtrl::Get_DgnStlServ(T_ELEM_K iElemNo, T_SERV_D& ServD)
{
	ServD.Initialize();

	if (!m_pDoc->m_pAttrCtrl->GetServ(iElemNo, ServD))
	{
		// Set Default.
		T_DSTL_D rData;
		rData.Initialize();
		Get_DgnStlDstl(rData);

		const CString& strCode = rData.DesignCode;
		if (strCode == STLCODE_EC3_05 || strCode == STLCODE_EC3_2_05 || strCode == STLCODE_EC3)
		{
			const int iSubCode = CDBLib::GetStlSubCode();

			ServD.iStressEnv = 0;     // Exposure class               (default :0 boundary 0 ~ 17)

			switch (iSubCode)
			{
			case dgn::def::enNationalAnnex::Sweden:
			case dgn::def::enNationalAnnex::Sweden19:
			{
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[2] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 1.0, 1.0)
				ServD.dDefLim = 200.0;// Deflection user defined      (default : 0 boundary >0)
			}
			break;
			default:
			{
				ServD.dStressk[0] = 0.6;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[1] = 0.45; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[2] = 0.8;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dStressk[3] = 1.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
				ServD.dDefLim = 250.0;// Deflection user defined      (default : 0 boundary >0)
			}
			break;
			}

			ServD.dRH = 0.0;            // Relative Humidity            (default : 0 boundary 0 ~ 100
			ServD.iCrackEnv = 0;        // Crack Environment            (default : 0 boundary 0 ~ 4)
			ServD.bCrackFlag[0] = FALSE;//Crack user defined           (default : FALSE)
			ServD.bCrackFlag[1] = FALSE;//Crack user defined           (default : FALSE)
			ServD.bCrackFlag[2] = FALSE; //Crack user defined           (default : FALSE)

			ServD.dCrackLim[0] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.dCrackLim[1] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.dCrackLim[2] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.iDeflecEnv = 1;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dRareDefl = 0.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)

			ServD.iServLife = 2;         // Crack Service Life          (default : 2 boundary 0 ~ 2)
			ServD.bHighCorrosion = FALSE;     // Crack Corrosion Sencitivity (default : FALSE           )
		}
		// PMS:4130-Seungjun-20100210 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
		else if ((strCode == STLCODE_TWN_LSD96 || strCode == STLCODE_TWN_ASD96) && (CProduct::IsMovingType(D_PRODUCT_MOVING_US) || CProduct::IsMovingType(D_PRODUCT_MOVING_RUS))) // 785 //
		{
			ServD.iStressEnv = 0;     // Exposure class               (default :0 boundary 0 ~ 17)

			ServD.dStressk[0] = 0.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[1] = 0.0; // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[2] = 0.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)
			ServD.dStressk[3] = 0.0;  // Stress parameter             (default : 0.6, 0.45, 0.8, 1.0)

			ServD.dRH = 0.0;            // Relative Humidity            (default : 0 boundary 0 ~ 100
			ServD.iCrackEnv = 0;        // Crack Environment            (default : 0 boundary 0 ~ 4)
			ServD.bCrackFlag[0] = FALSE;//Crack user defined           (default : FALSE)
			ServD.bCrackFlag[1] = FALSE;//Crack user defined           (default : FALSE)
			ServD.bCrackFlag[2] = FALSE; //Crack user defined           (default : FALSE)

			ServD.dCrackLim[0] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.dCrackLim[1] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.dCrackLim[2] = 0.0;// Crack user defined           (default : 0 boundary >0)
			ServD.iDeflecEnv = 0;     // Deflection environment       (default : 0 boundary 0 ~ 4)
			ServD.dDefLim = 360.;  // Deflection user defined      (default : 0 boundary >0)
			ServD.dRareDefl = 0.;   // Rare deflection user defined (default ; 0 >0)
			ServD.dDAF = 1.0;        // Deflection Amplificaton Factor (default ; 1 > 0)

			ServD.iServLife = 2;         // Crack Service Life          (default : 2 boundary 0 ~ 2)
			ServD.bHighCorrosion = FALSE;     // Crack Corrosion Sencitivity (default : FALSE           )
		}
		else
		{
			if (IsJUD())
			{
				ServD.dDefLim = 250.0;
			}
		}
		// PMS:4130-Seungjun-20100210 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
		return FALSE;
	}

	return TRUE;
}

int  CDgnDataCtrl::Get_DgnConSbdoKList(int iType, std::vector<T_SBDO_K>& aSbdoKeys) // 0:all, 1:Slab, 2:Mat, 3:wall, 4:slab/mat, 5:inclined slab
{
	aSbdoKeys.clear();

	std::vector<T_SBDO_K> aAllSbdoKeys;
	aAllSbdoKeys.clear();
	m_pDoc->m_pAttrCtrl->GetSbdoKeyList(aAllSbdoKeys);
	const INT_PTR nSbdo = aAllSbdoKeys.size();

	if (iType != EN_DGNSBDO_ALL)
	{
		aSbdoKeys.reserve(nSbdo*0.1);
		for (INT_PTR i = 0; i < nSbdo; ++i)
		{
			T_SBDO_K SbdoK = aAllSbdoKeys[i];

			T_SBDO_D SbdoD;
			if (!m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) { continue; }

			switch (iType)
			{
			case EN_DGNSBDO_SLAB:
			{
				if (SbdoD.nType == SBDO_SLAB)  // Slab
				{
					aSbdoKeys.push_back(SbdoK);
				}
			}
			break;
			case EN_DGNSBDO_MAT:
			{
				if (SbdoD.nType == SBDO_MAT)  // Mat
				{
					aSbdoKeys.push_back(SbdoK);
				}
			}
			break;
			case EN_DGNSBDO_WALL:
			{
				if (SbdoD.nType == SBDO_WALL)  // Wall
				{
					aSbdoKeys.push_back(SbdoK);
				}
			}
			break;
			case EN_DGNSBDO_SLABMAT:
			{
				if (SbdoD.nType == SBDO_SLAB || SbdoD.nType == SBDO_MAT)  // Slab/Mat
				{
					aSbdoKeys.push_back(SbdoK);
				}
			}
			break;
			//case EN_DGNSBDO_INSLAB:
			//  if(SbdoD.nType == SBDO_INSL)  // inclined slab
			//    aSbdoKeys.push_back(SbdoK);
			//  break;
			case EN_DGNSBDO_SHELL:
			{
				if (SbdoD.nType == SBDO_SHEL)
				{
					aSbdoKeys.push_back(SbdoK);
				}
			}
			break;
			default:
				ASSERT(0);
				break;
			}
		}
	}
	else
	{
		aSbdoKeys.assign(aAllSbdoKeys.begin(), aAllSbdoKeys.end());
	}

	CVectorUtil::Sort(aSbdoKeys);
	return static_cast<int>(aSbdoKeys.size());
}


// PMS:4020-Seungjun-20091125 EC2:04 Slab/Wall Design.
int CDgnDataCtrl::GetPlateList(CArray<T_ELEM_K, T_ELEM_K>& aElemList, BOOL bWall/*FALSE*/)
{
	aElemList.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> aSelElemList, aOkElemList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aSelElemList);

	int iSelElemNum = aSelElemList.GetSize();
	int iOkElemNum = 0;
	for (int i = 0; i < iSelElemNum; i++)
	{
		T_ELEM_K ElemShowKey = 0;
		T_ELEM_K ElemKey = aSelElemList.GetAt(i);

		T_ELEM_D ElemData;
		ElemData.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemKey, ElemData)) { continue; }

		if (!bWall)
		{
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)) { continue; }
		}
		else
		{
			if (!(m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp) ||
				m_pDoc->m_pAttrCtrl->IsPlstrs(ElemData.eltyp)))
			{
				continue;
			}
		}

		aOkElemList.Add(ElemKey);
		iOkElemNum++;
	}
	// Sorting by Member.
	int* pValue = new int[iOkElemNum];
	for (int i = 0; i < iOkElemNum; i++)
	{
		pValue[i] = aOkElemList.GetAt(i);
	}

	CQSort::QSortInt(pValue, iOkElemNum);
	// Save elements for Design/Checking.
	if (iOkElemNum == 1)
	{
		aElemList.Add(pValue[0]);
	}
	else
	{
		for (int j = 0; j < iOkElemNum - 1; j++)
		{
			T_ELEM_K iKeyPrev = pValue[j + 0];
			T_ELEM_K iKeyNext = pValue[j + 1];
			if (!(iKeyPrev > 0 && iKeyNext > 0))	ASSERT(0);
			// Save only NOT Same member key.
			if (j <= iOkElemNum - 2)
			{
				if (iKeyPrev != iKeyNext)	aElemList.Add(iKeyPrev);
			}
			if (j == iOkElemNum - 2)
			{
				if (iKeyPrev != iKeyNext)	aElemList.Add(iKeyNext);
				else					aElemList.Add(iKeyPrev);
			}
		}
	}
	delete[]pValue;

	return aElemList.GetSize();
}


int CDgnDataCtrl::GetDgnStatus()
{
	return m_iDgnStatus;
}

BOOL CDgnDataCtrl::SetDgnStatus(int iDgnStatus)
{
	m_iDgnStatus = iDgnStatus;
	return TRUE;
}

// �Ϻ�1������ �Ǵ�
BOOL CDgnDataCtrl::IsJUD()
{
	if (!m_pDoc->EnableJUD()) { return FALSE; }

	T_JUDB_INFO JudbD;  JudbD.Initialize();
	m_pDoc->GetJDDBatchInfo(JudbD);
	if (JudbD.nStatus != 0) { return TRUE; }

	if (m_iDgnStatus != 0)
	{
		// Change by SHIN : 2011.11.09 midasIT-JP ���ǻ���(����,������,������,���¿�,�̹���,����,�ź���)
		//                  iGen���� �Ϻ� ���� ���ý� Design�޴����� �������� �ϰ����� ������ ��������?�� �ɼ� ����        
		switch (m_iDgnStatus)
		{
		case RC_BEAM:
		case RC_COLUMN:
		case RC_BRACE:
		case RC_WALL:
		case RC_FOOTING:
		{
			T_DCON_D DconD;
			Get_DgnConDcon(DconD);
			if (DconD.DesignCode == CONCODE_AIJ_WSD99) { return TRUE; }
		}
		break;
		case STEEL:
		{
			T_DSTL_D DstlD;
			Get_DgnStlDstl(DstlD);
			if (DstlD.DesignCode == STLCODE_AIJ_ASD02) { return TRUE; }
		}
		break;
		case CFSTEEL:
		{
			return FALSE; /// cold form ������ �Ϻ� ������ ����.
		}
		break;
		default:
			return FALSE;
		}
	}
	return FALSE;
}

BOOL CDgnDataCtrl::ConsiderJUDWebBending(int nMembType)
{
	if (!IsJUD()) return TRUE;
	T_DSTL_D DstlD; DstlD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDstl(DstlD)) {/*ASSERT(0);*/ return TRUE; }

	BOOL bWeb = TRUE;
	if (nMembType == D_MBTP_COLUMN)     bWeb = DstlD.bWebColm;
	else if (nMembType == D_MBTP_BEAM)  bWeb = DstlD.bWebBeam;
	return bWeb;
}

BOOL CDgnDataCtrl::IsLongTerm(int nLcomType, T_LCOM_K LcomK)
{
	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) { return TRUE; }

	T_LCOM_D_UL	LcomUlData;
	if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData)) { return TRUE; }

	const INT_PTR nSize = LcomUlData.Combination.GetSize();
	// Modify by GAY. PMS:4215-126. ('12.01.04). Short-X, Short-Y�� ����ǰ�? �ִ� ���� ����.
	T_LCTP_D LctpD; LctpD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetLctp(LctpD))
	{
		int nSumLong = 0;
		for (INT_PTR i = 0; i < nSize; ++i)
		{
			int nLcaseKey = LcomUlData.Combination[i].LoadCaseKey;
			int nAnalType = LcomUlData.Combination[i].AnalType;

			const INT_PTR nLongSize = LctpD.arLongKey.GetSize();
			for (INT_PTR j = 0; j < nLongSize; ++j)
			{
				BOOL bTypeOK = nAnalType == LctpD.arLongType[j] ? TRUE : FALSE;
				BOOL bKeyOK = nLcaseKey == LctpD.arLongKey[j] ? TRUE : FALSE;

				if (bTypeOK && bKeyOK) { nSumLong += 1; }
			}
		}
		return (nSumLong == nSize) ? TRUE : FALSE;
	}

	return TRUE;
}

BOOL CDgnDataCtrl::IsSeismicLcom(int nLcomType, T_LCOM_K LcomK)
{
	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) { return TRUE; }

	T_LCOM_D_UL	LcomUlData;
	if (!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD, LcomUlData)) { return TRUE; }

	const INT_PTR nSize = LcomUlData.Combination.GetSize();
	for (INT_PTR i = 0; i < nSize; ++i)
	{
		T_LCOM_BASE LcomBaseD = LcomUlData.Combination[i];
		switch (LcomBaseD.AnalType)
		{
		case D_LCOM_STATIC:
		{
			T_STLD_D StldD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStld(LcomBaseD.LoadCaseKey, StldD))
			{
				if (CDBLib::IsStldEL(StldD.LoadCaseType)) return TRUE;
			}
		}
		break;
		case D_LCOM_SPECTRUM:
		case D_LCOM_ECCENSPEC:
		case D_LCOM_SPECTRUM_ND:
		case D_LCOM_ECCENSPEC_ND:
			return TRUE;
		case D_LCOM_CB_GENERAL:
			if (IsSeismicLcom(D_LCOMTYPE_GENERAL, LcomBaseD.LoadCaseKey)) return TRUE;
			break;
		case D_LCOM_CB_CONCRETE:
			if (IsSeismicLcom(D_LCOMTYPE_CONCRETE, LcomBaseD.LoadCaseKey)) return TRUE;
			break;
		case D_LCOM_CB_STEEL:
			if (IsSeismicLcom(D_LCOMTYPE_STEEL, LcomBaseD.LoadCaseKey)) return TRUE;
			break;
		case D_LCOM_CB_FOOTING:
			if (IsSeismicLcom(D_LCOMTYPE_FDN, LcomBaseD.LoadCaseKey)) return TRUE;
			break;
		case D_LCOM_CB_SRC:
			if (IsSeismicLcom(D_LCOMTYPE_SRC, LcomBaseD.LoadCaseKey)) return TRUE;
			break;
		default:
			break;
		}
	}
	return FALSE;
}

bool CDgnDataCtrl::IsAccidentalLoad(int nLoadCaseK)
{
	auto l_IsAccidentalLoad =[] (const CString& strLoadType)
	{
		// Accidental design situations - fire, explosion, impact, consequences of localised failure.
		if ( strLoadType == _T("IL") ) return TRUE;
		if ( strLoadType == _T("CO") ) return TRUE;
		if ( strLoadType == _T("CT") ) return TRUE;
		if ( strLoadType == _T("CV") ) return TRUE;
		return FALSE;
	};

	T_STLD_D rData;
	rData.Initialize();
	if ( !m_pDoc->m_pAttrCtrl->GetStld(nLoadCaseK, rData) )
	{
		/*ASSERT(0); */return false; 
	}

	return l_IsAccidentalLoad(rData.LoadCaseType);
}

void CDgnDataCtrl::Get_UnitFactorForThick(double& rdLenUnit, double& rdStrUnit, int nCodeUnitLen, int nCodeUnitFor)
{
	auto L_GetFactorFor = [](int nUnitFor) -> double
	{
		switch (nUnitFor)
		{
		case D_UNITSYS_FORCE_INDEX_KG:	return 9.80665;
		case D_UNITSYS_FORCE_INDEX_TON:	return 9806.65;
		case D_UNITSYS_FORCE_INDEX_N:	return 1.0;
		case D_UNITSYS_FORCE_INDEX_KN:	return 1000.0;
		case D_UNITSYS_FORCE_INDEX_LBF:	return 4.448223;
		case D_UNITSYS_FORCE_INDEX_KIP:	return 4448.223;
		}
		ASSERT(0); return 1.0;
	};
	auto L_GetFactorLen = [](int nUnitLen) -> double
	{
		switch (nUnitLen)
		{
		case D_UNITSYS_LENGTH_INDEX_M: return 1000.0;
		case D_UNITSYS_LENGTH_INDEX_CM:	return 10.0;
		case D_UNITSYS_LENGTH_INDEX_MM:	return 1.0;
		case D_UNITSYS_LENGTH_INDEX_IN:	return 25.4;
		case D_UNITSYS_LENGTH_INDEX_FT:	return 25.4*12.0;
		}
		ASSERT(0); return 1.0;
	};

	double dFfor = L_GetFactorFor(nCodeUnitFor);
	double dFlen = L_GetFactorLen(nCodeUnitLen);
	// Get Length, Stress Factor.
	rdLenUnit = dFlen;
	rdStrUnit = dFfor / pow(dFlen, 2.0);
}

double CDgnDataCtrl::Get_FyByThick_Code(const CString& strMatlDB, const CString& strMatlNa, int nShapeClass, BOOL bHollowSect, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM])
{
	const double dFy1 = adFy[EN_FY_THK_1];
	const double dFy2 = adFy[EN_FY_THK_2];
	const double dFy3 = adFy[EN_FY_THK_3];
	const double dFy4 = adFy[EN_FY_THK_4];
	const double dFy5 = adFy[EN_FY_THK_5];
	const double dFy6 = adFy[EN_FY_THK_6];

	if (strMatlDB == MATLCODE_STL_KS_CIVIL) { return Get_FyByThick_KS_Civil(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KS08_CIVIL) { return Get_FyByThick_KS_Civil(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KS10_CIVIL) { return Get_FyByThick_KS_Civil(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KSCE_LSD15) { return Get_FyByThick_KS_Civil(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KS08) { return Get_FyByThick_KS08(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KS09) { return Get_FyByThick_KS09(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_KS16) { return Get_FyByThick_KS16(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_KS18) { return Get_FyByThick_KS18_KDS14(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_KS22) { return Get_FyByThick_KS22(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_KS) { return Get_FyByThick_KS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_ASTM) { return dFy1; }
	if (strMatlDB == MATLCODE_STL_ASTM09) { return dFy1; } // Add by GAY. PMS:4061. ('09.09.08). Add <ASTM09(S)>.
	if (strMatlDB == MATLCODE_STL_JIS) { return Get_FyByThick_JIS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_JIS_CIVIL) { return Get_FyByThick_JIS_Civil(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_DIN) { return Get_FyByThick_DIN(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_BS04) { return Get_FyByThick_BS04(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_BS) { return Get_FyByThick_BS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_EN05) { return Get_FyByThick_EN05(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_EN05_PS) { return Get_FyByThick_EN05P(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_EN05_SW) { return Get_FyByThick_EN05SW(strMatlNa, dThkMax, UnitParam, dFy1); }
	if (strMatlDB == MATLCODE_STL_EN) { return Get_FyByThick_EN(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_UNI) { return Get_FyByThick_UNI(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_GB12) { return Get_FyByThick_GB12(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_GB03) { return Get_FyByThick_GB03(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_GB) { return Get_FyByThick_GB(strMatlNa, nShapeClass, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_GB50018_02) { return Get_FyByThick_GB50018_02(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_GB50018_25) { return Get_FyByThick_GB50018_25(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_JGJ2015) { return Get_FyByThick_JGJ2015(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_GB50017_17) { return Get_FyByThick_GB50017_17(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_TB10091_17) { return Get_FyByThick_TB10091_17(strMatlNa, dThkMax, UnitParam, dFy1, dFy2); }
	if (strMatlDB == MATLCODE_STL_TB10092_17) { return Get_FyByThick_TB10092_17(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_JGJ) { return Get_FyByThick_JGJ(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_JTJ) { return Get_FyByThick_JTJ(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_JTJ023_85) { return Get_FyByThick_JTJ023_85(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_JTG04) { ASSERT(0); return dFy1; }
	if (strMatlDB == MATLCODE_STL_TB05) { ASSERT(0); return dFy1; }
	if (strMatlDB == MATLCODE_STL_CSA) { return Get_FyByThick_CSA(strMatlNa, bHollowSect, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_IS) { return Get_FyByThick_IS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_CNS) { return Get_FyByThick_CNS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_CNS06) { return Get_FyByThick_CNS06(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4); }
	if (strMatlDB == MATLCODE_STL_BC1_12_ASTM) { return Get_FyByThick_BS1_12ASTM(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_BC1_12_BSEN) { return Get_FyByThick_BS1_12BSEN(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_BC1_12_JIS) { return Get_FyByThick_BS1_12JIS(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_BC1_12_GB) { return Get_FyByThick_BS1_12GB(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_BC1_12_CLASS2) { return Get_FyByThick_BS1_12BSEN(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_BC1_12_CLASS3) { return Get_FyByThick_BS1_12BSEN(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_JTG3362_18) { return Get_FyByThick_JTG3362_18(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_EN10326) { return Get_FyByThick_EN10326(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_EN10149_2) { return Get_FyByThick_EN10149_2(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_EN10149_3) { return Get_FyByThick_EN10149_3(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_GB50917_13) { return Get_FyByThick_GB_50917_13(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5); }
	if (strMatlDB == MATLCODE_STL_AS_NZS_3678) { return Get_FyByThick_ASNZS3678_17(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3, dFy4, dFy5, dFy6); }
	if (strMatlDB == MATLCODE_STL_AS_NZS_3679_1) { return Get_FyByThick_ASNZS3679_17(strMatlNa, dThkMax, UnitParam, dFy1, dFy2, dFy3); }
	if (strMatlDB == MATLCODE_STL_AS_NZS_4672_1) { return Get_FyByThick_ASNZS4672_17(strMatlNa, dFy1); }
	if (strMatlDB == MATLCODE_STL_TIS1228_2018) { return Get_FyByThick_TIS1228_2018(strMatlNa, dThkMax, UnitParam, dFy1); }
	if (strMatlDB == MATLCODE_STL_SP16_2017_TB3) { return Get_FyByThick_SP16_2017_tB3(strMatlNa, dThkMax, UnitParam, adFy); }
	if (strMatlDB == MATLCODE_STL_SP16_2017_TB4) { return Get_FyByThick_SP16_2017_tB4(strMatlNa, dThkMax, UnitParam, adFy); }
	if (strMatlDB == MATLCODE_STL_SP16_2017_TB5) { return Get_FyByThick_SP16_2017_tB5(strMatlNa, dThkMax, UnitParam, adFy); }
	if (strMatlDB == MATLCODE_STL_NR_GN_CIV_025) { return Get_FyByThick_NR_GN_CIV_025(strMatlNa, dThkMax, UnitParam, adFy); }

	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_KS16(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// Add by GAY. PMS:5002. ('16.03.28). Add KS16(S).
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("TMC")) >= 0 || strMatlNa.Find(_T("HSA800")) >= 0) // SM490TMC, SM520TMC, SM570TMC, HSA800.
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH400")) >= 0 || strMatlNa.Find(_T("SPSR")) >= 0) // SWH400, SPSR400.
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("STKN")) >= 0 || strMatlNa.Find(_T("SPAP")) >= 0) // STKN400B, STKN490B, SPAP235, SPAP325.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SPAR")) >= 0 || strMatlNa.Find(_T("STK400")) >= 0) // SPAR295, STK400.
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SCW")) >= 0) // SCW410, SWC480.
	{
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN")) >= 0)   // SHN400, SHN490.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SS490")) >= 0)   // SS490.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SS540")) >= 0)   // SS540.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SSC400")) >= 0)   // SSC400.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}

	// SS400, SM400, SN400, SMA400, SM490, SMA490, SN490, SM520, SM570, SMA570.	
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_KS18_KDS41(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SS275")) >= 0 || strMatlNa.Find(_T("SS315")) >= 0)  // SS275, SS315.
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SS410")) >= 0)  // SS410.
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SN275")) >= 0 || strMatlNa.Find(_T("SN355")) >= 0) // SN275, SN355 
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN")) >= 0)  // SHN275, SHN355
	{
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SSC275")) >= 0) // SSC275
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH275")) >= 0) // SWH275
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SNRT")) >= 0 || strMatlNa.Find(_T("SNT")) >= 0) // SNRT275A, SNRT355A, SNRT295E, SNT275, SNT355
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SGT275")) >= 0 || strMatlNa.Find(_T("SRT275")) >= 0) // SGT275, SRT275
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SGT355")) >= 0 || strMatlNa.Find(_T("SF")) >= 0) // SGT355, SF490A, SF540A 
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SRT355")) >= 0) // SRT355
	{
		if (UnitParam.IsLE(dThkMax, 30.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("TMC")) >= 0 || strMatlNa.Find(_T("HSA")) >= 0) // SM420TMC, SM460TMC, SM275TMC, SM355TMC, HSA650 
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}

	// SM275, SMA275, SM355, SMA355, SM420, SM460, SMA460
	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_KS18_KDS14(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	// Add by psg0604. PMS:5410. ('17.12.20). Add KS18(S).
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SS410")) >= 0 || strMatlNa.Find(_T("SS450")) >= 0 || strMatlNa.Find(_T("SS550")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSB380")) >= 0 || strMatlNa.Find(_T("HSB460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSM500")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSB690")) >= 0 || strMatlNa.Find(_T("HSA650")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SM275TMC")) >= 0 || strMatlNa.Find(_T("SM355TMC")) >= 0 || strMatlNa.Find(_T("SM420TMC")) >= 0 || strMatlNa.Find(_T("SM460TMC")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SN275")) >= 0 || strMatlNa.Find(_T("SN355")) >= 0 || strMatlNa.Find(_T("SN460")) >= 0 || strMatlNa.Find(_T("SNT")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SGT")) >= 0 || strMatlNa.Find(_T("SNRT275")) >= 0 || strMatlNa.Find(_T("SNRT355")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SNRT295")) >= 0 || strMatlNa.Find(_T("SNRT360")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHT460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 25.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SRT")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 30.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH")) >= 0 || strMatlNa.Find(_T("SSC275")) >= 0)
	{
		return dFy1;
	}
	if (strMatlNa.Find(_T("SM460")) >= 0 || strMatlNa.Find(_T("SMA460")) >= 0 || strMatlNa.Find(_T("SMA275C")) >= 0 || strMatlNa.Find(_T("SMA355C")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFy5;
}

double CDgnDataCtrl::Get_FyByThick_KS22(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SS410")) >= 0 || strMatlNa.Find(_T("SS450")) >= 0 || strMatlNa.Find(_T("SS550")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSB380")) >= 0 || strMatlNa.Find(_T("HSB460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSM500")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("HSB690")) >= 0 || strMatlNa.Find(_T("HSA650")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SM275TMC")) >= 0 || strMatlNa.Find(_T("SM355TMC")) >= 0 || strMatlNa.Find(_T("SM420TMC")) >= 0 || strMatlNa.Find(_T("SM460TMC")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SN275")) >= 0 || strMatlNa.Find(_T("SN355")) >= 0 || strMatlNa.Find(_T("SN460")) >= 0 || strMatlNa.Find(_T("SNT")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SGT")) >= 0 || strMatlNa.Find(_T("SNRT275A")) >= 0 || strMatlNa.Find(_T("SNRT355A")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SNRT295E")) >= 0 || strMatlNa.Find(_T("SNRT390E")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHT460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 25.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SRT")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 30.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH")) >= 0 || strMatlNa.Find(_T("SSC275")) >= 0)
	{
		return dFy1;
	}
	if (strMatlNa.Find(_T("SM460")) >= 0 || strMatlNa.Find(_T("SMA460")) >= 0 || strMatlNa.Find(_T("SMA275C")) >= 0 || strMatlNa.Find(_T("SMA355C")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFy5;
}

double CDgnDataCtrl::Get_FyByThick_GB50018_02(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("Q235")) >= 0 || strMatlNa.Find(_T("Q345")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}
double CDgnDataCtrl::Get_FyByThick_GB50018_25(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("Q235")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q390")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("S280")) >= 0 || strMatlNa.Find(_T("S350")) >= 0)
	{
		return dFy1;
	}
	if (strMatlNa.Find(_T("S550")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 0.6)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 0.9)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 1.2)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 1.5)) { return dFy4; }
		return dFyZero;
	}
	ASSERT(0);
	return 0.0;
}
double CDgnDataCtrl::Get_FyByThick_JGJ2015(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	//Add by xuezc 2017/9/8
	double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("Q235")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345GJ")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFyZero; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q390")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q420")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	ASSERT(0);

	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_GB50017_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	//Add by xuezc 2018/2/8
	//Modify by tss 2019/07/22
	const double dFyZero = UnitParam.GetCurZeroStress();

	if (strMatlNa.Find(_T("Q235")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q355")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		if (UnitParam.IsGE(dThkMax, 100.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345GJ")) >= 0)
	{
		if (UnitParam.IsGE(dThkMax, 16.0) && UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q390")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q420")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_GB50429_07(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("6063A-T5")) >= 0 || strMatlNa.Find(_T("6063A-T6")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 10.0)) { return dFy1; }
		return dFy2;
	}
	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_TB10092_17(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("Strand")) >= 0 || strMatlNa.Find(_T("Wire")) >= 0 || strMatlNa.Find(_T("PS")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_TB10091_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	const double dFyZero = UnitParam.GetCurZeroStress();

	if (strMatlNa.Find(_T("Q235q")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 35.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("Q345q")) >= 0 || strMatlNa.Find(_T("Q370q")) >= 0 || strMatlNa.Find(_T("Q420q")) >= 0 || strMatlNa.Find(_T("Q500q")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_JTG3362_18(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("Strand")) >= 0 || strMatlNa.Find(_T("Wire")) >= 0 || strMatlNa.Find(_T("Steel")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_EN10326(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("GD")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_EN10149_2(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("MC")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_EN10149_3(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("NC")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_GB_50917_13(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	/// [MQC 23785] �Ϲ� ö�� ������ �β��� ���� ���� �ݿ�.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("Strand")) >= 0 || strMatlNa.Find(_T("Wire")) >= 0 || strMatlNa.Find(_T("Steel")) >= 0)
	{
		return dFy1;
	}
	if (strMatlNa == _T("Q235"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa == _T("Q345") || strMatlNa == _T("Q390") || strMatlNa == _T("Q420"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa == _T("Q235q") || strMatlNa == _T("Q345q") || strMatlNa == _T("Q370q") || strMatlNa == _T("Q420q"))
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_ASNZS3678_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6)
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("200")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("250")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 8.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("300")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 8.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy5; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy6; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("WR350")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("350")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("400")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("450")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 32.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
		return dFyZero;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_ASNZS3679_17(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	if (strMatlNa.Find(_T("HRS300")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLT(dThkMax, 100.0)) { return dFy2; }
		return dFy3; // �β��� 100 �̻��� ���? 
	}
	if (strMatlNa.Find(_T("HRS350")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLT(dThkMax, 100.0)) { return dFy2; }
		return dFy3; // �β��� 100 �̻��� ���?
	}
	if (strMatlNa.Find(_T("300")) >= 0)
	{
		if (UnitParam.IsLT(dThkMax, 11.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 17.0)) { return dFy2; }
		return dFy3; // �β��� 17 �̻��� ���? 
	}
	if (strMatlNa.Find(_T("350")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 11.0)) { return dFy1; }
		if (UnitParam.IsLT(dThkMax, 40.0)) { return dFy2; }
		return dFy3; // �β��� 40 �̻��� ���? 
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_ASNZS4672_17(const CString& strMatlNa, double dFy1)
{
	if (strMatlNa.Find(_T("1030")) >= 0 || strMatlNa.Find(_T("1670")) >= 0 || strMatlNa.Find(_T("1700")) >= 0 || strMatlNa.Find(_T("1700S")) >= 0 || strMatlNa.Find(_T("1790S")) >= 0
		|| strMatlNa.Find(_T("1820S")) >= 0 || strMatlNa.Find(_T("1830S")) >= 0 || strMatlNa.Find(_T("1850S")) >= 0 || strMatlNa.Find(_T("1870S")) >= 0)
	{
		return dFy1;
	}

	ASSERT(0);
	return 0.0;
}

double CDgnDataCtrl::Get_FyByThick_KS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	// Change by ZINU.('07.05.30). NO:2829, Add SHN490.
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("490-CF")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 8.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("490TMC")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_KS08(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// Add by ZINU.('08.05.28). NO:3446, Add KS08(S).
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("TMC")) >= 0)  // SM490TMC, SM520TMC.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SM490Y")) >= 0 || strMatlNa.Find(_T("SM520")) >= 0 || strMatlNa.Find(_T("SM570")) >= 0) // SM490Y, SM520, SM570.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SMA")) >= 0)  // SMA400, SMA490, SMA570.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SSC")) >= 0) // SSC400.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH")) >= 0) // SWH400.
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SPSR")) >= 0) // SPS490, SPSR490.
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SPS")) >= 0) // SPS400, SPSR400.
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SCW")) >= 0) // SCW410, SCW480.
	{
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy1; }
		return dFyZero;
	}
	// SS400, SM400, SM490, SN400, SN490.	
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_KS09(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	// Add by GAY. PMS:4035. ('09.06.19). Add KS09(S).
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("TMC")) >= 0) // SM490TMC, SM520TMC, SM570TMC.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("490-CF")) >= 0) // SCW490-CF.
	{
		if (UnitParam.IsLE(dThkMax, 8.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SSC400")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SWH400")) >= 0 || strMatlNa.Find(_T("SPSR")) >= 0) // SWH400, SPSR400, SPSR490.
	{
		if (UnitParam.IsLE(dThkMax, 12.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("STK400")) >= 0 || strMatlNa.Find(_T("STK490")) >= 0) // STK400, STK490.
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SPAR")) >= 0) // SPAR295.
	{
		if (UnitParam.IsLE(dThkMax, 22.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("STKN")) >= 0 || strMatlNa.Find(_T("SPAP")) >= 0) // STKN400B, STKN490B, SPAP235, SPAP325.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SDP")) >= 0) // SDP1, SDP2, SDP3.
	{
		if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SCW")) >= 0) // SCW410, SWC480.
	{
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN400")) >= 0)   // SHN400, by PSG0604. ('14.06.19). PMS:4889
	{
		if (UnitParam.IsLE(dThkMax, 70.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SHN490")) >= 0)   // SHN490,  by PSG0604. ('14.06.19). PMS:4889
	{
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy1; }
		return dFyZero;
	}
	// SS400, SM400, SN400, SMA400, SM490, SN490, SMAP490, SM520, SM570.
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_KS_Civil(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// Add by Jaeoh. ('08.10.30).
	// See GEN-I.D.S(V3.3) p.146.		
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SM")) >= 0 || strMatlNa.Find(_T("SS")) >= 0 || strMatlNa.Find(_T("STK")) >= 0 || strMatlNa.Find(_T("HSB")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	return dFy3;
}

double CDgnDataCtrl::Get_FyByThick_JIS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa == _T("SS540"))
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa == _T("SM520"))
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_JIS_Civil(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.		
	if (strMatlNa == _T("SS400") ||
		strMatlNa == _T("SM400") || strMatlNa == _T("SM490") || strMatlNa == _T("SM490Y") || strMatlNa == _T("SM520") || strMatlNa == _T("SM570") ||
		strMatlNa == _T("SMA400W") || strMatlNa == _T("SMA490W") || strMatlNa == _T("SMA570W"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		return dFy4;
	}

	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_DIN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// See GEN-I.D.S(V3.3) p.146.	
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("GS")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("C35N")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_BS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.
	// Modify by GAY. MNET:2706. ('07.03.22). Add Parameter dFy2, dFy3, dFy4.
	// Modify by GAY. MQC:3163. ('10.02.16). ������ ��ȯ ���� ����.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	if (strMatlNa.Find(_T("43")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("50")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("55")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 25.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy4; }
		return dFyZero;
	}

	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_BS04(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6)
{
	// See GEN-I.D.S(V3.3) p.146.
	// add by Seungjun (`06.05.29).
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	if (strMatlNa.Find(_T("275")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy6; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("355")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy6; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("460")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy6; }
		return dFyZero;
	}

	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_EN05(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	// See GEN-I.D.S(V3.3) p.146.	
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_EN05P(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6)
{
	// See GEN-I.D.S(V3.3) p.146.	
	const double dFyZero = UnitParam.GetCurZeroStress();
	double dThick6_Lt = 0.0;
	if (strMatlNa.Find(_T("275M")) >= 0 || strMatlNa.Find(_T("355M")) >= 0 || strMatlNa.Find(_T("420M")) >= 0 || strMatlNa.Find(_T("460M")) >= 0)
	{
		dThick6_Lt = 120.0;
	}
	else
	{
		dThick6_Lt = 150.0;
	}

	if (strMatlNa.Find(_T("460Q")) >= 0)
	{
		if (UnitParam.IsGE(dThkMax, 3.0) && UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy3; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
	if (UnitParam.IsLE(dThkMax, dThick6_Lt)) { return dFy6; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_EN05SW(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1)
{
	//   Sweden ���? ������ ���?,�ϴ� �β��� �׺������� �ٸ��� ����ϰ�? ����������, ���� �߰� �ݿ����θ� ���� �Լ�������.// PMS4588:DNKIM-20130313 
	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_EN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	// See GEN-I.D.S(V3.3) p.146.	
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_UNI(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2)
{
	// Add by ZINU.('05.08.02). See E-Mail from Italy('05.07.20).	
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_GB12(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// See GB50017-13 4.4.1. p.19.
	if (strMatlNa == _T("Q235"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa == _T("Q345"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
		return dFyZero;
	}
	if (strMatlNa == _T("Q390") || strMatlNa == _T("Q420") || strMatlNa == _T("Q460"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}	// Series (Q345GJ).

	if (UnitParam.IsGT(dThkMax, 16.0) && UnitParam.IsLE(dThkMax, 35.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_GB03(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// See GB50017-03 3.4.1. p.17.
	if (strMatlNa == _T("Q235"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	// Series (Q345, Q390, Q420).	
	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 35.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_GB(const CString& strMatlNa, int nShapeClass, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	// See GBJ17-88 3.2.1. p.6.			
	if (strMatlNa == _T("Grade3"))
	{
		switch (nShapeClass)
		{
		case 1:
		{
			if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
			if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
			return dFyZero;
		}
		case 2:
		{
			if (UnitParam.IsLE(dThkMax, 15.0)) { return dFy1; }
			if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy2; }
			return dFy3;
		}
		case 3:
		{
			if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy1; }
			if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
			if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
			return dFyZero;
		}
		default:
		{
			ASSERT(0);
			return 0.0;
		}
		}
	}
	if (strMatlNa.Find(_T("16Mn")) >= 0 || strMatlNa.Find(_T("15MnV")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 25.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 36.0)) { return dFy3; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_JGJ(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	// See CHINA SUM. SUMMARY NOTE1.
	if (strMatlNa == _T("Q235"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 35.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_JTJ(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	// See JTJ025-86 p541.
	if (strMatlNa == _T("16Mn"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 25.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 36.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy4; }
		return dFyZero;
	}

	return dFy1;
}

double CDgnDataCtrl::Get_FyByThick_JTJ023_85(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{

	return dFy1;
}


double CDgnDataCtrl::Get_FyByThick_CSA(const CString& strMatlNa, BOOL bHollowSect, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	// See GEN-I.D.S(V3.3) p.146.
	// Set Matl Class.
	// See CSA-S16-01 13.3.1 p.30.	
	const double dFyZero = UnitParam.GetCurZeroStress();
	// See CISC Handbook of Steel Construction(1985),Mechanical properties summary Table6-3. p6-7.	
	if (bHollowSect)
	{
		if (strMatlNa.Find(_T("300WT")) >= 0)
		{
			if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
			if (UnitParam.IsLE(dThkMax, 65.0)) { return dFy2; }
			if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
			return dFyZero;
		}
		else if (strMatlNa.Find(_T("300W")) >= 0 || strMatlNa.Find(_T("350W")) >= 0 || strMatlNa.Find(_T("380W")) >= 0 || strMatlNa.Find(_T("350A")) >= 0)
		{
			if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
			return dFyZero;
		}
	}

	if (strMatlNa.Find(_T("350R")) >= 0)	// Max.THK 14.
	{
		if (UnitParam.IsLE(dThkMax, 14.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("400W")) >= 0 || strMatlNa.Find(_T("480W")) >= 0 || strMatlNa.Find(_T("480A")) >= 0)	// Max.THK 2O.
	{
		if (UnitParam.IsLE(dThkMax, 20.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("350G")) >= 0 || strMatlNa.Find(_T("400G")) >= 0)	// Max.THK 3O.
	{
		if (UnitParam.IsLE(dThkMax, 30.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("400A")) >= 0)	// Max.THK 4O.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("700Q")) >= 0)	// Max.THK 5O.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("350WT")) >= 0)	// Max.THK 6O.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("260W")) >= 0 || strMatlNa.Find(_T("300W")) >= 0 || strMatlNa.Find(_T("350W")) >= 0)	// Max.THK 10O.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 65.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("230G")) >= 0)	// Max.THK 30O.
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 65.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 300.0)) { return dFy3; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 65.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_IS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3)
{
	T_PREFERENCE rPref;
	 rPref.Initialize();
	 m_pDoc->m_pInitCtrl->GetPreference(rPref);
	 CString strDesignCode = CDBLib::GetStlCodeName();
	 BOOL bIsIS800_2007 = (strDesignCode == STLCODE_IS800_2007) ? TRUE : FALSE;
	 double dFy1Thk = bIsIS800_2007 ? 20. : 16.;
	 double dFy2Thk = bIsIS800_2007 ? 40. : 40.;
	 double dFy3Thk = bIsIS800_2007 ? 10000. : 63;
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	// Found(0), Not found(-1).
	if (strMatlNa.Find(_T("Fe290")) >= 0 || strMatlNa.Find(_T("Fe330")) >= 0 || strMatlNa.Find(_T("Fe370")) >= 0)
	{
		// See IS1977:1996, Table3.
		return dFy1;
	}
	if (strMatlNa.Find(_T("Fe410")) >= 0)
	{
		// See IS2062:1999, Table3.
		if (UnitParam.IsLT(dThkMax, 20.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFy3;
	}

	if (strMatlNa.Find(_T("E250")) >= 0 || strMatlNa.Find(_T("E275")) >= 0 || strMatlNa.Find(_T("E300")) >= 0 ||
		strMatlNa.Find(_T("E350")) >= 0 || strMatlNa.Find(_T("E410")) >= 0 || strMatlNa.Find(_T("E450")) >= 0 ||
		strMatlNa.Find(_T("E550")) >= 0 || strMatlNa.Find(_T("E600")) >= 0 || strMatlNa.Find(_T("E650")) >= 0 ||
		strMatlNa.Find(_T("Fe440")) >= 0 || strMatlNa.Find(_T("Fe490")) >= 0 || strMatlNa.Find(_T("Fe540")) >= 0 || strMatlNa.Find(_T("Fe570")) >= 0 || strMatlNa.Find(_T("Fe590")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, dFy1Thk)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, dFy2Thk)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, dFy3Thk)) { return dFy3; }
		return dFy3;
	}

	// See IS8500:1991 p.3.
	if (UnitParam.IsLE(dThkMax, dFy1Thk)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, dFy2Thk)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, dFy3Thk)) { return dFy3; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_CNS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// See GEN-I.D.S(V3.3) p.146.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SS")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return (strMatlNa.Find(_T("540")) >= 0) ? dFyZero : dFy4;
	}
	if (strMatlNa.Find(_T("SMA")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFy3;
	}
	if (strMatlNa.Find(_T("SM")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SN")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("STK")) >= 0)	// STK, STKR.
	{
		return dFy1;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_CNS06(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4)
{
	// Add by GAY. PMS:4064. ('09.09.17).
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("SS")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy3; }
		return (strMatlNa.Find(_T("540")) >= 0) ? dFyZero : dFy4;
	}
	if (strMatlNa.Find(_T("SMA")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		return dFy3;
	}
	if (strMatlNa.Find(_T("SM")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("SN")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFyZero;
	}
	if (strMatlNa.Find(_T("STK")) >= 0)	// STK, STKR.
	{
		return dFy1;
	}
	if (strMatlNa.Find(_T("SHY")) >= 0) // SHY685, SHY685N, SHY685NS.
	{
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
		return dFy3;
	}
	if (strMatlNa.Find(_T("SWH")) >= 0 || strMatlNa.Find(_T("WH")) >= 0 || strMatlNa.Find(_T("SDP")) >= 0 || strMatlNa.Find(_T("SH590")) >= 0 || strMatlNa.Find(_T("SPA")) >= 0)
	{
		return dFy1;
	}

	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy2; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_BS1_12ASTM(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	// See BC1:2012, Table 18. Design parameters of American (ASTM and API) structural steels.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 32.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 65.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_BS1_12BSEN(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6)
{
	// by GAY. PMS:5007. ('15.03.27). BC1:12-Class2(S), BC1:12-Class3(S)
	// by GAY. PMS:4817. ('14.03.11). Add BC1:12-BS EN(S).
	// See BC1:2012, Table 17. Design parameters of British/European (BS EN) structural steels.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 63.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 80.0)) { return dFy4; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy5; }
	if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy6; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_BS1_12JIS(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5, double dFy6)
{
	// by GAY. PMS:4817. ('14.03.11). Add BC1:12-JIS(S).
	// See BC1:2012, Table 19. Design parameters of Japanese (JIS) structural steels
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("490Y")) >= 0 || strMatlNa.Find(_T("520")) >= 0 || strMatlNa.Find(_T("570")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 160.0)) { return dFy5; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 40.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 75.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	if (UnitParam.IsLE(dThkMax, 160.0)) { return dFy5; }
	if (UnitParam.IsLE(dThkMax, 200.0)) { return dFy6; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_BS1_12GB(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1, double dFy2, double dFy3, double dFy4, double dFy5)
{
	// by GAY. PMS:4817. ('14.03.11). Add BC1:12-GB(S).
	// See BC1:2012, Table 21. Design parameters of Chinese (GB) structural steels.
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa.Find(_T("Q235")) >= 0 || strMatlNa.Find(_T("Q275")) >= 0)
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
		if (UnitParam.IsLE(dThkMax, 35.0)) { return dFy2; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
		if (UnitParam.IsLE(dThkMax, 150.0)) { return dFy5; }
		return dFyZero;
	}

	if (UnitParam.IsLE(dThkMax, 16.0)) { return dFy1; }
	if (UnitParam.IsLE(dThkMax, 35.0)) { return dFy2; }
	if (UnitParam.IsLE(dThkMax, 50.0)) { return dFy3; }
	if (UnitParam.IsLE(dThkMax, 100.0)) { return dFy4; }
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_TIS1228_2018(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double dFy1)
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (UnitParam.IsLE(dThkMax, 6.0)) { return dFy1; }	
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_SP16_2017_tB3(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM])
{	
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa == _T("C235"))
	{		
		return UnitParam.IsLE(dThkMax, 4.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C245"))
	{
		return UnitParam.IsLE(dThkMax, 20.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C255"))
	{
		if (UnitParam.IsLE(dThkMax,  4.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 10.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_4]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C345K"))
	{
		return UnitParam.IsLE(dThkMax, 10.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C355"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_4]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_5]; }
		if (UnitParam.IsLE(dThkMax, 160.0)) { return adFy[EN_FY_THK_6]; }
		if (UnitParam.IsLE(dThkMax, 200.0)) { return adFy[EN_FY_THK_7]; }
		if (UnitParam.IsLE(dThkMax, 260.0)) { return adFy[EN_FY_THK_8]; }
		if (UnitParam.IsLE(dThkMax, 300.0)) { return adFy[EN_FY_THK_9]; }
		if (UnitParam.IsLE(dThkMax, 360.0)) { return adFy[EN_FY_THK_10]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C355-1") || strMatlNa == _T("C390"))
	{
		return UnitParam.IsLE(dThkMax, 16.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C355-K"))
	{
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return adFy[EN_FY_THK_2]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C355P"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C3390-1"))
	{		
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_4]; }
		if (UnitParam.IsLE(dThkMax, 160.0)) { return adFy[EN_FY_THK_5]; }		
		return dFyZero;
	}
	if (strMatlNa == _T("C440"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_4]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_5]; }
		if (UnitParam.IsLE(dThkMax, 160.0)) { return adFy[EN_FY_THK_6]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C550") || strMatlNa == _T("C590"))
	{
		return UnitParam.IsLE(dThkMax, 50.0) ? adFy[EN_FY_THK_1]: dFyZero;
	}

	ASSERT(0);
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_SP16_2017_tB4(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM])
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa == _T("C255B"))
	{
		return UnitParam.IsLE(dThkMax, 10.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C255B-1") || strMatlNa == _T("C355B"))
	{
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_4]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_5]; }
		return adFy[EN_FY_THK_6];
	}
	if (strMatlNa == _T("C345B") || strMatlNa == _T("C345B-1"))
	{
		if (UnitParam.IsLE(dThkMax, 10.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_3]; }		
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_4]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C355B-1"))
	{
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }		
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_3]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C390B"))
	{
		if (UnitParam.IsLE(dThkMax, 30.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_4]; }
		return adFy[EN_FY_THK_5];
	}
	if (strMatlNa == _T("C440B"))
	{		
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 30.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 80.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 100.0)) { return adFy[EN_FY_THK_4]; }
		return adFy[EN_FY_THK_5];
	}
	ASSERT(0);
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_SP16_2017_tB5(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM])
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa == _T("C245"))
	{
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C255") || strMatlNa == _T("C345"))
	{
		if (UnitParam.IsLE(dThkMax, 10.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_3]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C345K"))
	{
		return UnitParam.IsLE(dThkMax, 10.0) ? adFy[EN_FY_THK_1] : dFyZero;
	}
	if (strMatlNa == _T("C355") || strMatlNa == _T("C355-1"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_3]; }
		return dFyZero;
	}
	if (strMatlNa == _T("C390") || strMatlNa == _T("C440"))
	{
		if (UnitParam.IsLE(dThkMax, 10.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 20.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 40.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 60.0)) { return adFy[EN_FY_THK_4]; }
		return dFyZero;
	}
	ASSERT(0);
	return dFyZero;
}

double CDgnDataCtrl::Get_FyByThick_NR_GN_CIV_025(const CString& strMatlNa, double dThkMax, T_FY_UNITPARAM& UnitParam, double adFy[EN_FY_THK_NUM])
{
	const double dFyZero = UnitParam.GetCurZeroStress();
	if (strMatlNa == _T("Pre1906") || strMatlNa == _T("BS15:1906") || strMatlNa == _T("Wrought Iron")) /// for all thickness.
	{
		return adFy[EN_FY_THK_1];
	}
	if (strMatlNa == _T("BS15:1948") || strMatlNa == _T("BS2762:1956(A)") || strMatlNa == _T("BS2762:1956(B)"))
	{
		return UnitParam.IsLE(dThkMax, 19.0) ? adFy[EN_FY_THK_1] : adFy[EN_FY_THK_2];
	}
	if (strMatlNa == _T("BS15:1961"))
	{
		if (UnitParam.IsLE(dThkMax, 19.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 38.0)) { return adFy[EN_FY_THK_2]; }
		return adFy[EN_FY_THK_3];
	}
	if (strMatlNa == _T("BS548:1934"))
	{
		if (UnitParam.IsLE(dThkMax, 32.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 45.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 57.0)) { return adFy[EN_FY_THK_3]; }
		if (UnitParam.IsLE(dThkMax, 70.0)) { return adFy[EN_FY_THK_4]; }
		return adFy[EN_FY_THK_5];
	}
	if (strMatlNa == _T("BS968:1941"))
	{
		if (UnitParam.IsLE(dThkMax, 13.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 25.0)) { return adFy[EN_FY_THK_2]; }
		return adFy[EN_FY_THK_3];
	}
	if (strMatlNa == _T("BS968:1962"))
	{
		if (UnitParam.IsLE(dThkMax, 16.0)) { return adFy[EN_FY_THK_1]; }
		if (UnitParam.IsLE(dThkMax, 32.0)) { return adFy[EN_FY_THK_2]; }
		if (UnitParam.IsLE(dThkMax, 50.0)) { return adFy[EN_FY_THK_3]; }
		return dFyZero;
	}
	ASSERT(0);
	return dFyZero;
}

int CDgnDataCtrl::GetChkKindStlMatl(const CString& strStlMatlCode)
{
	if (strStlMatlCode == MATLCODE_STL_KS_CIVIL)		return 3;
	if (strStlMatlCode == MATLCODE_STL_KS08_CIVIL)		return 3;
	if (strStlMatlCode == MATLCODE_STL_KS22)			return 5;
	if (strStlMatlCode == MATLCODE_STL_KS18)			return 5; // Add by psg0604.       ('17.12.20). Add KS18(S). (Available : Fy1, Fy2, Fy3, Fy4). PMS:5410.
	if (strStlMatlCode == MATLCODE_STL_KS16)			return 3; // Add by GAY. PMS:5002. ('16.03.29). Add KS15(S). (Available : Fy1, Fy2, Fy3).
	if (strStlMatlCode == MATLCODE_STL_KS08)			return 3; // Add by ZINU.('08.05.29). NO:3446, Add KS08(S).
	if (strStlMatlCode == MATLCODE_STL_KS09)			return 2; // Add by GAY. PMS:4035. ('09.06.19). Add KS09(S). (Available : Fy1, Fy2).
	if (strStlMatlCode == MATLCODE_STL_KS)				return 2;
	if (strStlMatlCode == MATLCODE_STL_ASTM09)			return 1; // Add by GAY. PMS:4061. ('09.09.08). Add ASTM09(S).
	if (strStlMatlCode == MATLCODE_STL_ASTM)			return 1;
	if (strStlMatlCode == MATLCODE_STL_JIS)				return 2;
	if (strStlMatlCode == MATLCODE_STL_JIS_CIVIL)		return 2;
	if (strStlMatlCode == MATLCODE_STL_BS04)			return 6;
	if (strStlMatlCode == MATLCODE_STL_BS)				return 4; // Modify by GAY. MNET:2706. ('07.03.23). 1->4.
	if (strStlMatlCode == MATLCODE_STL_DIN)				return 2;
	if (strStlMatlCode == MATLCODE_STL_EN05)			return 2;
	if (strStlMatlCode == MATLCODE_STL_EN05_PS)			return 6;
	if (strStlMatlCode == MATLCODE_STL_EN05_SW)			return 1;
	if (strStlMatlCode == MATLCODE_STL_EN)				return 2;
	if (strStlMatlCode == MATLCODE_STL_UNI)				return 2;
	if (strStlMatlCode == MATLCODE_STL_GB12)			return 6;
	if (strStlMatlCode == MATLCODE_STL_GB03)			return 4;
	if (strStlMatlCode == MATLCODE_STL_GB)				return 3;
	if (strStlMatlCode == MATLCODE_STL_GB50018_02)		return 1;
	if (strStlMatlCode == MATLCODE_STL_GB50018_25)		return 4;
	if (strStlMatlCode == MATLCODE_STL_JGJ)				return 4;
	if (strStlMatlCode == MATLCODE_STL_JTJ023_85)		return 4;
	if (strStlMatlCode == MATLCODE_STL_JTJ)				return 4;
	if (strStlMatlCode == MATLCODE_STL_JTG04)			return 1;	// Only Strand, Wire, Steel-Bar.	
	if (strStlMatlCode == MATLCODE_STL_TB05)			return 1;	// Only Strand, Wire, Steel-Bar.
	if (strStlMatlCode == MATLCODE_STL_CNS)				return 2;
	if (strStlMatlCode == MATLCODE_STL_CNS06)			return 2; // Add by GAY. PMS:4064. ('09.09.17). Add CNS06(S).
	if (strStlMatlCode == MATLCODE_STL_GOST_SP)  		return 4;
	if (strStlMatlCode == MATLCODE_STL_GOST_SNIP)		return 4;
	if (strStlMatlCode == MATLCODE_STL_BC1_12_ASTM)		return 5;
	if (strStlMatlCode == MATLCODE_STL_BC1_12_BSEN)		return 6;
	if (strStlMatlCode == MATLCODE_STL_BC1_12_JIS)		return 6;
	if (strStlMatlCode == MATLCODE_STL_BC1_12_GB)		return 5;
	if (strStlMatlCode == MATLCODE_STL_BC1_12_CLASS2)	return 6; // by GAY. PMS:5007. ('15.03.27)
	if (strStlMatlCode == MATLCODE_STL_BC1_12_CLASS3)	return 6; // by GAY. PMS:5007. ('15.03.27)	
	if (strStlMatlCode == MATLCODE_STL_JGJ2015)			return 5; // by xuezc (2017/9/5)
	if (strStlMatlCode == MATLCODE_STL_GB50017_17)		return 5; // by xuezc (2018/2/7)
	if (strStlMatlCode == MATLCODE_STL_TB10092_17)		return 1;
	if (strStlMatlCode == MATLCODE_STL_TB10091_17)		return 2;
	if (strStlMatlCode == MATLCODE_STL_CSA)				return 3;
	if (strStlMatlCode == MATLCODE_STL_IS)				return 3;
	if (strStlMatlCode == MATLCODE_STL_KSCE_LSD15)		return 3;
	if (strStlMatlCode == MATLCODE_STL_KS10_CIVIL)		return 3;
	if (strStlMatlCode == MATLCODE_STL_JTG3362_18)		return 1;
	if (strStlMatlCode == MATLCODE_STL_EN10326)			return 1; //#if defined(_US) �϶� ���?
	if (strStlMatlCode == MATLCODE_STL_EN10149_2)		return 1; //#if defined(_US) �϶� ���?
	if (strStlMatlCode == MATLCODE_STL_EN10149_3)		return 1; //#if defined(_US) �϶� ���?
	if (strStlMatlCode == MATLCODE_STL_JTG_D64_2015)	return 5; //������ GOST-SP �� GOST-SNIP �� ���������� �β� ���� ���� ���� 
	if (strStlMatlCode == MATLCODE_STL_GB50917_13)		return 1;
	if (strStlMatlCode == MATLCODE_STL_AS_NZS_3678)		return 6;
	if (strStlMatlCode == MATLCODE_STL_AS_NZS_3679_1)	return 3;
	if (strStlMatlCode == MATLCODE_STL_AS_NZS_4672_1)	return 1;
	if (strStlMatlCode == MATLCODE_STL_TIS1228_2018)	return 1;
	if (strStlMatlCode == MATLCODE_STL_SP16_2017_TB3)	return 10;
	if (strStlMatlCode == MATLCODE_STL_SP16_2017_TB4)	return 6;
	if (strStlMatlCode == MATLCODE_STL_SP16_2017_TB5)	return 4;
	if (strStlMatlCode == MATLCODE_STL_NR_GN_CIV_025)   return 5;
	return 1;
}

double CDgnDataCtrl::GetDeflectLimByCode_EC3(int nMembType, int& rnDeflType)
{
	CArray<double, double> aDefLims;

	int nSubCode = CDBLib::GetStlSubCode();
	if (nSubCode == dgn::def::enNationalAnnex::Singapore) // Singapore.
	{
		switch (nMembType)
		{
		case D_MBTP_BEAM:
		case D_MBTP_BRACE:
		{
			aDefLims.Add(200.0);
			aDefLims.Add(360.0);
			aDefLims.Add(180.0);
		}
		break;
		case D_MBTP_COLUMN:
		{
			aDefLims.Add(300.0);
		}
		break;
		default:
			break;
		}
	}
	else
	{
		switch (nMembType)
		{
		case D_MBTP_BEAM:
		case D_MBTP_BRACE:
		{
			aDefLims.Add(400.0);
			aDefLims.Add(250.0);
			aDefLims.Add(200.0);
		}
		break;
		case D_MBTP_COLUMN:
		{
			aDefLims.Add(500.0);
			aDefLims.Add(300.0);
			aDefLims.Add(150.0);
		}
		break;
		default:
			break;
		}
	}

	if (rnDeflType >= aDefLims.GetSize())
	{
		rnDeflType = aDefLims.GetSize() - 1;
	}
	return aDefLims.GetAt(rnDeflType);
}

double CDgnDataCtrl::GetDeflectLimByCode_TWN96(int nMembType, int& rnDeflType)
{
	double dDeflLimit = 0.0;
	switch (nMembType)
	{
	case D_MBTP_BEAM:
	case D_MBTP_BRACE:
	{
		dDeflLimit = 360.0;
	}
	break;
	case D_MBTP_COLUMN:
	{
		dDeflLimit = 300.0;
	}
	break;
	default: ASSERT(0);
		break;
	}

	if (rnDeflType != 0) { rnDeflType = 1; }
	return dDeflLimit;
}

double CDgnDataCtrl::GetDeflectLimByCode_KSSC09(int nMembType, int& rnDeflType)
{
	double dDeflLimit = 0.0;
	switch (nMembType)
	{
	case D_MBTP_BEAM:
	case D_MBTP_BRACE:
	{
		dDeflLimit = 300.0;
	}
	break;
	case D_MBTP_COLUMN:
	{
		dDeflLimit = 500.0;
	}
	break;
	default: ASSERT(0);
		break;
	}

	if (rnDeflType != 0) { rnDeflType = 1; }
	return dDeflLimit;
}

double CDgnDataCtrl::GetDeflectLimByCode_AISC05(int nMembType, int& rnDeflType)
{
	double dDeflLimit = 0.0;
	switch (nMembType)
	{
	case D_MBTP_BEAM:
	case D_MBTP_BRACE:
	{
		dDeflLimit = 360.0;
	}
	break;
	case D_MBTP_COLUMN:
	{
		dDeflLimit = 300.0;
	}
	break;
	default: ASSERT(0);
		break;
	}

	if (rnDeflType != 0) { rnDeflType = 1; }
	return dDeflLimit;
}

double CDgnDataCtrl::GetDeflectLimByCode_KDS2022(int nMembType, int& rnDeflType)
{
	CArray<double, double> aDefLims;
	switch (nMembType)
	{
	case D_MBTP_BEAM:
	case D_MBTP_BRACE:
		{
			aDefLims.Add(400.0);
			aDefLims.Add(250.0);
			aDefLims.Add(200.0);
		}
		break;
	case D_MBTP_COLUMN:
		{
			aDefLims.Add(500.0);
			aDefLims.Add(300.0);
			aDefLims.Add(150.0);
		}
		break;
	default: ASSERT(0);
		break;
	}
	if (rnDeflType >= aDefLims.GetSize()) { rnDeflType = aDefLims.GetSize() - 1; }
	return aDefLims.GetAt(rnDeflType);
}

double CDgnDataCtrl::GetDeflectLimByCode_GB17(int nMembType, int& rnDeflType)
{
	CArray<double, double> aDefLims;
	switch (nMembType)
	{
	case D_MBTP_BEAM:
		//case D_MBTP_BRACE:
	{
		aDefLims.Add(400.0);
		aDefLims.Add(250.0);
		aDefLims.Add(200.0);
	}
	break;
	case D_MBTP_COLUMN:
	{
		aDefLims.Add(500.0);
		aDefLims.Add(300.0);
		aDefLims.Add(150.0);
	}
	break;
	default: ASSERT(0);
		break;
	}
	if (rnDeflType >= aDefLims.GetSize()) { rnDeflType = aDefLims.GetSize() - 1; }
	return aDefLims.GetAt(rnDeflType);
}

double CDgnDataCtrl::GetDeflectLimByCode_IS800( int nMembType, int& rnDeflType )
{
	double dDeflLimit = 0.0;
	switch ( nMembType )
	{
	case D_MBTP_BEAM:
	case D_MBTP_BRACE:
	{
		dDeflLimit = 360.0;
	}
	break;
	case D_MBTP_COLUMN:
	{
		dDeflLimit = 500.0;
	}
	break;
	default: ASSERT( 0 );
		break;
	}

	if ( rnDeflType != 0 ) { rnDeflType = 1; }
	return dDeflLimit;
}


double CDgnDataCtrl::GetDeflectLimByCode_IRC24(int nMembType, int& rnDeflType)
{
	CArray<double, double> aDefLims;

	/*int nSubCode = CDBLib::GetStlSubCode();*/
		switch (nMembType)
		{
		case D_MBTP_BEAM:
		{
			aDefLims.Add(600.0);
			aDefLims.Add(800.0);
			aDefLims.Add(300.0);
			aDefLims.Add(400.0);
		}
		break;
		case D_MBTP_COLUMN:
		case D_MBTP_BRACE:
			aDefLims.Add(0);
			aDefLims.Add(0);
			aDefLims.Add(0);
			aDefLims.Add(0);
		default:
			break;
		}

	if (rnDeflType >= aDefLims.GetSize())
	{
		rnDeflType = aDefLims.GetSize() - 1;
	}
	return aDefLims.GetAt(rnDeflType);
}

bool CDgnDataCtrl::IsExistDgnLcomULS(int nLcomType)
{
	auto L_IsULS = [](int nActive)
	{
		switch (nActive)
		{
		case EN_DGN_STRN:
		case EN_DGN_SPEC:
		case EN_DGN_VERT:
		case EN_DGN_UG_STRN:
		case EN_DGN_UG_SPEC:
			return true;
		case EN_DGN_NONE:
		case EN_DGN_SERV:
		case EN_DGN_UG_SERV:
		case EN_DGN_STRE:
			return false;
		default:
			ASSERT(0);
			return false;
		}
	};

	T_LCOM_K_LIST aLcomK;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomK);
	const INT_PTR nLcom = aLcomK.GetSize();
	if (nLcom < 1) return false;

	for (INT_PTR i = 0; i < nLcom; ++i)
	{
		const T_LCOM_K LcomK = aLcomK[i];
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) continue;
		if (L_IsULS(LcomD.nActive))
		{
			return true;
		}
	}
	return false;
}

bool CDgnDataCtrl::IsExistDgnLcomSLS(int nLcomType)
{
	auto L_IsSLS = [](int nActive)
	{
		switch (nActive)
		{
		case EN_DGN_NONE:
		case EN_DGN_STRN:
		case EN_DGN_SPEC:
		case EN_DGN_VERT:
		case EN_DGN_UG_STRN:
		case EN_DGN_UG_SPEC:
		case EN_DGN_STRE:
			return false;
		case EN_DGN_SERV:
		case EN_DGN_UG_SERV:
			return true;
		default:
			ASSERT(0);
			return false;
		}
	};

	T_LCOM_K_LIST aLcomK;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomK);
	const INT_PTR nLcom = aLcomK.GetSize();
	if (nLcom < 1) return false;

	for (INT_PTR i = 0; i < nLcom; ++i)
	{
		const T_LCOM_K LcomK = aLcomK[i];
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) continue;
		if (L_IsSLS(LcomD.nActive))
		{
			return true;
		}
	}
	return false;
}

bool CDgnDataCtrl::IsExistDgnLcomELS(int nLcomType)
{
	auto L_IsELS = [](int nActive)
	{
		switch (nActive)
		{
		case EN_DGN_NONE:
		case EN_DGN_STRN:
		case EN_DGN_SPEC:
		case EN_DGN_VERT:
		case EN_DGN_UG_STRN:
		case EN_DGN_UG_SPEC:
		case EN_DGN_SERV:
		case EN_DGN_UG_SERV:
			return false;
		case EN_DGN_STRE:
			return true;
		default:
			ASSERT(0);
			return false;
		}
	};

	T_LCOM_K_LIST aLcomK;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomK);
	const INT_PTR nLcom = aLcomK.GetSize();
	if (nLcom < 1) return false;

	for (INT_PTR i = 0; i < nLcom; ++i)
	{
		const T_LCOM_K LcomK = aLcomK[i];
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD)) continue;
		if (L_IsELS(LcomD.nActive))
		{
			return true;
		}
	}
	return false;
}

BOOL CDgnDataCtrl::IsMeshedSlab(const int& iDgnCode)
{
	BOOL bMeshed = FALSE;
	switch (iDgnCode)
	{
	case Eurocode2_04:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case GB50010_10:
	case GB50010_19:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case IS456_2000:
	case AIJ_WSD99:
	case TWN_USD112:
		bMeshed = TRUE;
		break;
	default:
		break;
	}

	return bMeshed;
}

BOOL CDgnDataCtrl::IsMeshedWall(const int& iDgnCode)
{
	BOOL bMeshed = FALSE;
	switch (iDgnCode)
	{
	case Eurocode2_04:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318M_14:
	case ACI318_19:
	case ACI318M_19:
	case ACI318_25:
	case ACI318M_25:
	case NSR_10:
	case NSCP_2015_C:
	case TWN_USD112:
		bMeshed = TRUE;
		break;
	default:
		break;
	}

	return bMeshed;
}

int CDgnDataCtrl::GetCodeID4Meshed(const CString& strCode, const BOOL& bPlateForce)
{
	// by GAY. ('13.07.25). MQC(Alpha):4603-1. ACI ���ؿ��� �������պ� ���? ��¾ȵ�?.
	if (strCode == CONCODE_EC2_04 || bPlateForce)
	{
		return Eurocode2_04;
	}
	if (strCode == CONCODE_ACI318_05)
	{
		return ACI318_05;
	}
	if (strCode == CONCODE_ACI318_08)
	{
		return ACI318_08;
	}
	if (strCode == CONCODE_ACI318_11)
	{
		return ACI318_11;
	}
	if (strCode == CONCODE_ACI318_14)
	{
		return ACI318_14;
	}
	if (strCode == CONCODE_ACI318M_14)
	{
		return ACI318M_14;
	}
	if (strCode == CONCODE_ACI318_19)
	{
		return ACI318_19;
	}
	if (strCode == CONCODE_ACI318M_19)
	{
		return ACI318M_19;
	}
	if (strCode == CONCODE_ACI318_25)
	{
		return ACI318_25;
	}
	if (strCode == CONCODE_ACI318M_25)
	{
		return ACI318M_25;
	}
	if (strCode == CONCODE_NSR_10)
	{
		return NSR_10;
	}
	if (strCode == CONCODE_NSCP_2015)
	{
		return NSCP_2015_C;
	}
	if (strCode == CONCODE_KCI_USD12)
	{
		return KCI_USD12;
	}
	if (strCode == CONCODE_KDS_41_30_2018)
	{
		return KDS_41_30_2018;
	}
	if (strCode == CONCODE_KDS_41_20_2022)
	{
		return KDS_41_20_2022;
	}
	if (strCode == CONCODE_IS456_2000)//Pinakin
	{
		return IS456_2000;
	}
	if (strCode == CONCODE_AIJ_WSD99)
	{
		return AIJ_WSD99;
	}
	if(strCode == CONCODE_TWN_USD112)
	{
		return TWN_USD112;
	}
	if (strCode == CONCODE_SP_63_13330_2018)
	{
		return SP_63_1330_2018;
	}

	ASSERT(0);
	return 0;
}

void CDgnDataCtrl::SetDatabyNationalAnnex(int nNationalAnnex, double* dPhi, double& dAlpha_cc, double& dAlphacc4Shear)
{
	dPhi[0] = 1.50; // Fund.Conc
	dPhi[1] = 1.20; // Acci.Conc
	dPhi[2] = 1.15; // Fund.Rbar
	dPhi[3] = 1.00; // Acci.Rbar
	dPhi[4] = 1.00; // SLS. Conc
	dPhi[5] = 1.00; // SLS. Rbar

	switch ( nNationalAnnex )
	{
	case Recommended:
	case Austria:
	case Belgium:
	case Cyprus:
	case Finland:
	case Germany:
	case Greece:
	case Ireland:
	case Italy:
	case Norway:
	case Poland:
	case Romania:
	case Singapore:
	case Slovakia:
	case Slovenia:
		{
			dAlpha_cc = 0.85;
			dAlphacc4Shear = 0.85;
		}
		break;
	case CzechRepublic:
		{
			dAlpha_cc = 0.9;
			dAlphacc4Shear = 0.9;
		}
		break;
	case Denmark:
		{
			dPhi[0] = 1.45; // Fund.Conc			
			dPhi[2] = 1.20; // Fund.Rbar

			dAlpha_cc = 1.0;
			dAlphacc4Shear = 1.0;
		}
		break;
	case Netherlands:
	case France:
	case Spain:
	case Sweden:
		{
			dAlpha_cc = 1.0;
			dAlphacc4Shear = 1.0;
		}
		break;
	case Luxembourg:
		{
			dAlpha_cc = 0.85;
			dAlphacc4Shear = 1.0;
		}
		break;

	case UnitedKingdom:
		{
			dAlpha_cc = 0.85;
			dAlphacc4Shear = 1.0;
		}
		break;
	default: ASSERT(0); break;
	}

	return;
}

BOOL CDgnDataCtrl::IsNodeOnElemPlane(T_NODE_D& NodeD, T_ELEM_D& ElemD, double dTol/*=1e-4*/)
{
	int node_in_elem = Get_Node_in_Elem(ElemD);
	if (node_in_elem < 3) { ASSERT(0); return FALSE; }

	double dP0_ofElemPlane[3] = { 0.0 };
	double dP1_ofElemPlane[3] = { 0.0 };
	double dP2_ofElemPlane[3] = { 0.0 };
	double dP_Node[3] = { 0.0 };
	T_NODE_D NodeD_temp;

	if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD_temp)) { ASSERT(0); return FALSE; }
	dP0_ofElemPlane[0] = NodeD_temp.x;
	dP0_ofElemPlane[1] = NodeD_temp.y;
	dP0_ofElemPlane[2] = NodeD_temp.z;

	if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD_temp)) { ASSERT(0); return FALSE; }
	dP1_ofElemPlane[0] = NodeD_temp.x;
	dP1_ofElemPlane[1] = NodeD_temp.y;
	dP1_ofElemPlane[2] = NodeD_temp.z;

	if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[2], NodeD_temp)) { ASSERT(0); return FALSE; }
	dP2_ofElemPlane[0] = NodeD_temp.x;
	dP2_ofElemPlane[1] = NodeD_temp.y;
	dP2_ofElemPlane[2] = NodeD_temp.z;

	double dA = 0.0;
	double dB = 0.0;
	double dC = 0.0;
	double dH = 0.0;
	CMathFunc::mathPlaneEquation(dP0_ofElemPlane, dP1_ofElemPlane, dP2_ofElemPlane, dA, dB, dC, dH);

	// elem plane�� punching node ���� �Ÿ� 
	double dLength = CMathFunc::mathDistanceToPlane(dA, dB, dC, dH, NodeD.x, NodeD.y, NodeD.z);

	// ���� ���? ���� ������ skip
	// [GEN-7880] code unit�� ���� ���� ���? ���� ���ΰ� �޶����Ƿ�, mm�� ��ȯ�Ͽ� �����ϵ��� ����. (0.001mm�� ���? ������)
	const double dTolerance = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM,
		D_UNITSYS_BASE_LENGTH, 0.001);
	if (dLength > dTolerance) { return FALSE; }

	return TRUE;
}

BOOL CDgnDataCtrl::IsSame3DPoint(T_NODE_D &p1, T_NODE_D &p2)
{
	const double dTolerance = 1e-8;
	if (fabs(p1.x - p2.x) > dTolerance) { return FALSE; }
	if (fabs(p1.y - p2.y) > dTolerance) { return FALSE; }
	if (fabs(p1.z - p2.z) > dTolerance) { return FALSE; }

	return TRUE;
}

BOOL CDgnDataCtrl::IsSameLine(double dOCx[2], double dOCy[2], double dNCx[2], double dNCy[2])
{
	// ���� ��ȯ�ϸ� B0(���� �ܸ� �ѷ�����)�� ���� �޶����� ���� Tolerance 1.0E-20�� 1.0E-10 ���� ���� - MQC:2005-JHYUN-20080718
	const double dTol = 1.0E-10;
	if (fabs(dOCx[0] - dNCx[0]) < dTol && fabs(dOCx[1] - dNCx[1]) < dTol &&
		fabs(dOCy[0] - dNCy[0]) < dTol && fabs(dOCy[1] - dNCy[1]) < dTol)
	{
		return TRUE;
	}


	if (fabs(dOCx[0] - dNCx[1]) < dTol && fabs(dOCx[1] - dNCx[0]) < dTol &&
		fabs(dOCy[0] - dNCy[1]) < dTol && fabs(dOCy[1] - dNCy[0]) < dTol)
	{
		return TRUE;
	}

	return FALSE;
}

// Seungjun-20090506 Ŀ�ö����� 2���� �����? ������ �� �ʿ�.
// PilePoint���� �����? ������ ���� �� ���̰� ���� �� �� ����.
double CDgnDataCtrl::GetMaxDistFormPnt(UINT NodeK, UINT ElemK)
{
	T_NODE_D NodeD;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, NodeD);

	T_ELEM_D elem_d;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, elem_d);
	int node_in_elem = Get_Node_in_Elem(elem_d);

	double dMaxDist = 0.0;
	for (int i = 0; i < node_in_elem; i++)
	{
		UINT ElemNodeK = elem_d.elnod[i];
		T_NODE_D ElemNodeD;
		m_pDoc->m_pAttrCtrl->GetNode(ElemNodeK, ElemNodeD);
		double dDist = sqrt(pow(ElemNodeD.x - NodeD.x, 2.0) + pow(ElemNodeD.y - NodeD.y, 2.0));

		if (dMaxDist < dDist) { dMaxDist = dDist; }
	}
	return dMaxDist;
}

int  CDgnDataCtrl::Get_SamePlaneList(T_KEY NodeK, CArray<UINT, UINT> &arElemK, CArray<UINT, UINT> &arSamePlaneElemK)
{
	arSamePlaneElemK.RemoveAll();

	CArray<CArray<UINT, UINT>*, CArray<UINT, UINT>*> aEachPlaneElem;
	int nPlane = m_pDoc->m_pAttrCtrl2->GetPlaneElemListInSbdo(arElemK, aEachPlaneElem);  // Sub-Domain�ȿ� �����? ����Ʈ
	if (nPlane == 1)
	{
		arSamePlaneElemK.Copy(arElemK);
		return arSamePlaneElemK.GetSize();
	}

	for (int k = 0; k < nPlane; k++)
	{
		CArray<T_ELEM_K, T_ELEM_K> aPlaneElemK;
		aPlaneElemK.RemoveAll();
		int nPlaneElemK = aEachPlaneElem[k]->GetSize();
		for (int i = 0; i < nPlaneElemK; i++)
		{
			aPlaneElemK.Add(aEachPlaneElem[k]->GetAt(i));
		}

		for (int j = 0; j < nPlaneElemK; j++)
		{
			BOOL bBreak = FALSE;
			T_ELEM_K ElemK = aPlaneElemK[j];
			T_ELEM_D ElemD;
			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
			int nNode_in_elem = Get_Node_in_Elem(ElemD);
			for (int n = 0; n < nNode_in_elem; n++)
			{
				if (ElemD.elnod[n] == NodeK)
				{
					arSamePlaneElemK.Copy(aPlaneElemK);
					return arSamePlaneElemK.GetSize();
				}
			}
			if (bBreak)  break;
		}
	}

	return 0;
}

CString CDgnDataCtrl::Get_OptimizedListString(CArray<T_ELEM_K, T_ELEM_K>& aKey)
{
	long nSize = aKey.GetSize();
	if (nSize == 0) { return _T(""); } // Select�� ���� ������ _T("") ����

	long *aNum = new long[nSize];
	for (int i = 0; i < nSize; i++)
	{
		aNum[i] = aKey[i];
	}
	qsort((void*)aNum, nSize, sizeof(long), CNumericOptimizer::comparei);

	// Optimize�Ѵ�.
	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.Optimize(aNum, nSize);

	delete[]aNum;

	return sOptimizedString;
}

UINT CDgnDataCtrl::GetBrdgSeisEvalCode()
{
	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetBsed(BsedD))
	{
		BsedD.nDesignCode = KISTEC_2015;
	}

	return BsedD.nDesignCode;
}

UINT CDgnDataCtrl::GetSRCCodeEnum(const CString& strCode)
{
	UINT enCode = 0;
	if (strCode == SRCCODE_AIK_SRC2K) { enCode = AIK_SRC2K; }
	else if (strCode == SRCCODE_AIJ_SRC01) { enCode = AIJ_SRC01; }
	else if (strCode == SRCCODE_TWN_SRC92) { enCode = TWN_SRC92; }
	else if (strCode == SRCCODE_TWN_SRC100) { enCode = TWN_SRC100; }
	else ASSERT(0);

	return enCode;
}

UINT CDgnDataCtrl::GetStlCodeEnum(const CString& strCode)
{
	if (strCode == STLCODE_AIK_ASD83) { return AIK_ASD83; }
	if (strCode == STLCODE_AISC_ASD89) { return AISC_ASD89; }
	if (strCode == STLCODE_AISC_LRFD93) { return AISC_LRFD93; }
	if (strCode == STLCODE_AISC_LRFD2K) { return AISC_LRFD2K; }
	if (strCode == STLCODE_AIK_LSD97) { return AIK_LSD97; }
	if (strCode == STLCODE_KSCE_ASD96) { return KSCE_ASD96; }
	if (strCode == STLCODE_BS5950_90) { return BS5950_90; }
	if (strCode == STLCODE_BS5950_2K) { return BS5950_2K; }
	if (strCode == STLCODE_EC3) { return Eurocode3; }
	if (strCode == STLCODE_EC3_05) { return Eurocode3_05; }
	if (strCode == STLCODE_EC3_2_05) { return Eurocode3_2_05; }
	if (strCode == STLCODE_IS800_2007) { return IS800_2007; }
	if (strCode == STLCODE_IS800_1984) { return IS800_1984; }
	if (strCode == STLCODE_AISI_CFSD86) { return AISI_CFSD86; }
	if (strCode == STLCODE_AIK_CFSD98) { return AIK_CFSD98; }
	if (strCode == STLCODE_AISI_CFSD08) { return AISI_CFSD08; }
	if (strCode == STLCODE_AIJ_ASD02) { return AIJ_ASD02; }
	if (strCode == STLCODE_GBJ17_88) { return GBJ17_88; }
	if (strCode == STLCODE_GB50017_03) { return GB50017_03; }
	if (strCode == STLCODE_AASHTO_LRFD02) { return AASHTO_LS02; }
	if (strCode == STLCODE_AASHTO_LRFD12) { return AASHTO_LS12; }
	if (strCode == STLCODE_AASHTO_LRFD16) { return AASHTO_LS16; }
	if (strCode == STLCODE_AASHTO_LRFD17) { return AASHTO_LS17; }
	if (strCode == STLCODE_AASHTO_LRFD20) { return AASHTO_LS20; }
	if (strCode == STLCODE_AASHTO_LRFD24) { return AASHTO_LS24; }
	if (strCode == STLCODE_JTJ025_86) { return JTJ025_86; }
	if (strCode == STLCODE_CSA_S16_01) { return CSA_S16_01; }
	if (strCode == STLCODE_CSA_S6_14) { return CSA_S6_14_ST; }
	if (strCode == STLCODE_CSA_S6_19) { return CSA_S6_19_ST; }
	if (strCode == STLCODE_AASHTO_LFD96) { return AASHTO_LS96; }
	if (strCode == STLCODE_TWN_BRG_LSD90) { return TWN_BRG_LS90; }
	if (strCode == STLCODE_TWN_ASD90) { return TWN_ASD90; }
	if (strCode == STLCODE_TWN_LSD90) { return TWN_LSD90; }
	if (strCode == STLCODE_AASHTO_ASD96) { return AASHTO_AS96; }
	if (strCode == STLCODE_TWN_BRG_ASD90) { return TWN_BRG_AS90; }
	if (strCode == STLCODE_KSSC_ASD03) { return KSSC_ASD03; }
	if (strCode == STLCODE_KSSC_LSD09) { return KSSC_LSD09; }
	if (strCode == STLCODE_KSSC_LSD16) { return KSSC_LSD16; }
	if (strCode == STLCODE_KDS_41_31_2019) { return KDS_41_31_2019; }
	if (strCode == STLCODE_KDS_41_30_10_2022) { return KDS_41_30_10_2022; }
	if (strCode == STLCODE_AISC_LRFD05) { return AISC_LRFD05; }
	if (strCode == STLCODE_AISC_ASD05) { return AISC_ASD05; }
	if (strCode == STLCODE_AISC_LRFD10) { return AISC_LRFD10; }
	if (strCode == STLCODE_AISC_ASD10) { return AISC_ASD10; }
	if (strCode == STLCODE_AISC_LRFD16) { return AISC_LRFD16; }
	if (strCode == STLCODE_AISC_ASD16) { return AISC_ASD16; }
	if (strCode == STLCODE_AISC_LRFD22) { return AISC_LRFD22; }
	if (strCode == STLCODE_AISC_ASD22) { return AISC_ASD22; }
	if (strCode == STLCODE_TWN_ASD96) { return TWN_ASD96; }
	if (strCode == STLCODE_TWN_LSD96) { return TWN_LSD96; }
	if (strCode == STLCODE_KSCE_ASD05) { return KSCE_ASD05; }
	if (strCode == STLCODE_KSCE_LSD15) { return KR_BRG_LSD15; }
	if (strCode == STLCODE_KSCE_ASD10) { return KSCE_ASD10; }
	if (strCode == STLCODE_KDS_24_14_30_2019) { return KDS_24_14_30_2019; }
	if (strCode == STLCODE_KDS_24_14_31_2018) { return KDS_24_14_31_2018; }
	if (strCode == STLCODE_IRC_24_2010) { return IRC24_2010; }
	if (strCode == STLCODE_NSCP_2015_LRFD) { return NSCP_2015_S_LRFD; }
	if (strCode == STLCODE_NSCP_2015_ASD) { return NSCP_2015_S_ASD; }
	if (strCode == STLCODE_IRS_SBC) { return IRS_SBC; }
	if (strCode == STLCODE_JROAD_H14) { return JROAD_H14; }
	if (strCode == STLCODE_JROAD_H24) { return JROAD_H24; }
	if (strCode == STLCODE_JROAD_H29) { return JROAD_H29; }
	if (strCode == STLCODE_JROAD_R07) { return JROAD_R07; }
	if (strCode == STLCODE_SP_16_13330_2017) { return SP_16_13330_2017; }

	ASSERT(FALSE);
	return 0;
}

UINT CDgnDataCtrl::GetConCodeEnum(const CString& strCode)
{
	if (strCode == CONCODE_ACI318_89) { return ACI318_89; }
	if (strCode == CONCODE_ACI318_95) { return ACI318_95; }
	if (strCode == CONCODE_ACI318_99) { return ACI318_99; }
	if (strCode == CONCODE_ACI318_02) { return ACI318_02; }
	if (strCode == CONCODE_ACI318_05) { return ACI318_05; }
	if (strCode == CONCODE_ACI318_08) { return ACI318_08; }
	if (strCode == CONCODE_ACI318_11) { return ACI318_11; }
	if (strCode == CONCODE_ACI318_14) { return ACI318_14; }
	if (strCode == CONCODE_ACI318M_14) { return ACI318M_14; }
	if (strCode == CONCODE_ACI318_19) { return ACI318_19; }
	if (strCode == CONCODE_ACI318M_19) { return ACI318M_19; }
	if (strCode == CONCODE_ACI318_25) { return ACI318_25; }
	if (strCode == CONCODE_ACI318M_25) { return ACI318M_25; }
	if (strCode == CONCODE_NSR_10) { return NSR_10; }
	if (strCode == CONCODE_AIK_USD94) { return AIK_USD94; }
	if (strCode == CONCODE_KSCE_USD96) { return KSCE_USD96; }
	if (strCode == CONCODE_KCI_USD99) { return KCI_USD99; }
	if (strCode == CONCODE_KCI_USD03) { return KCI_USD03; }
	if (strCode == CONCODE_KCI_USD07) { return KCI_USD07; }
	if (strCode == CONCODE_KCI_USD12) { return KCI_USD12; }
	if (strCode == CONCODE_KDS_41_30_2018) { return KDS_41_30_2018; }
	if (strCode == CONCODE_KDS_41_20_2022) { return KDS_41_20_2022; }
	if (strCode == CONCODE_TWN_USD92) { return TWN_USD92; }
	if (strCode == CONCODE_TWN_USD100) { return TWN_USD100; }
	if (strCode == CONCODE_TWN_USD112) { return TWN_USD112; }
	if (strCode == CONCODE_AIK_WSD2K) { return AIK_WSD2K; }
	if (strCode == CONCODE_AIJ_WSD99) { return AIJ_WSD99; }
	if (strCode == CONCODE_BS8110_97) { return BS8110_97; }
	if (strCode == CONCODE_EC2) { return Eurocode2; }
	if (strCode == CONCODE_EC2_04) { return Eurocode2_04; }
	if (strCode == CONCODE_EC2_2_05) { return Eurocode2_2_05; }
	if (strCode == CONCODE_BS5400_90) { return BS5400_4_90; }
	if (strCode == CONCODE_TMH07_89) { return TMH07_89; }
	if (strCode == CONCODE_IS456_2000) { return IS456_2000; }
	if (strCode == CONCODE_GB50010_02) { return GB50010_02; }
	if (strCode == CONCODE_GB50010_10) { return GB50010_10; }
	if (strCode == CONCODE_GB50010_19) { return GB50010_19; }
	if (strCode == CONCODE_CSA_A23_3_94) { return CSA_A233_94; }
	if (strCode == CONCODE_NSCP_2015) { return NSCP_2015_C; }
	if (strCode == CONCODE_NTC_DCEC_2017) { return NTC_DCEC_2017; }
	if (strCode == CONCODE_NTC_DCEC_2023) { return NTC_DCEC_2023; }
	if (strCode == CONCODE_AASHTO_LRFD20) { return AASHTO_LC20; }
	if (strCode == CONCODE_AASHTO_LRFD24) { return AASHTO_LC24; }
	if (strCode == CONCODE_AREMA_2023) { return AREMA23; }

	else ASSERT(0);
	return 0;
}

double CDgnDataCtrl::GetHcbmMaxHc(const T_ELEM_K& iElem)
{
	double dMaxHc(0.0);

	T_HCBM_K HcbmK(0);
	if (!m_pDoc->m_pAttrCtrl->GetHcbmAssigned(iElem, HcbmK)) return dMaxHc;
	T_HCBM_D HcbmD;
	if (!m_pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD)) return dMaxHc;

	for (int iPart = 0; iPart < 3; ++iPart)
	{
		int nElem = HcbmD.aElemListPart[iPart].GetSize();
		if (nElem == 0) continue;
		for (int i = 0; i < nElem; ++i)
		{
			T_ELEM_K ElemK = HcbmD.aElemListPart[iPart].GetAt(i);
			T_ELEM_K PartK = m_pMembCtrl->GetPartKeyInHcbm(ElemK, iPart);
			T_SECT_D SectD;
			if (!Get_DgnHcbmSect(PartK, iPart, SectD)) continue;
			dMaxHc = max(dMaxHc, SectD.SectBefore.SectI.Size[0]);
			dMaxHc = max(dMaxHc, SectD.SectBefore.SectJ.Size[0]);
		}
	}
	return dMaxHc;
}

int CDgnDataCtrl::GetBeamSupportType(const UINT& iElemNo, const int& iCantilever)
{
	int nSupport = EN_SUPPORT_PIN_PIN;

	if (iCantilever > 0)
	{
		return EN_SUPPORT_CANTI;
	}

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(iElemNo, ElemD)) { ASSERT(0); return nSupport; }

	T_ELEM_K RepElemK = iElemNo; 
	m_pMembCtrl->GetShowElemByIncludeElem(iElemNo, RepElemK);

	T_NODE_K aEdgeNode[2] = { 0,0 };
	m_pMembCtrl->GetEdgeNodeAtMemb(iElemNo, aEdgeNode);
	T_NODE_K NodeK_I = aEdgeNode[0];
	T_NODE_K NodeK_J = aEdgeNode[1];

	auto _LIsContinuesBeam = [&](T_NODE_K NodeK) -> BOOL
	{
		CArray<T_ELEM_K, T_ELEM_K> arElemK;
		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK, arElemK);
		for (int i = 0; i < arElemK.GetSize(); ++i)
		{
			T_ELEM_K ElemK = arElemK[i];

			if (ElemK == iElemNo) continue;

			T_ELEM_K CurRepElemK = ElemK;
			m_pMembCtrl->GetShowElemByIncludeElem(ElemK, CurRepElemK);
			if (RepElemK == CurRepElemK) continue;

			int iMbtp = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK, FALSE);
			if (iMbtp != D_MBTP_BEAM)  continue;
			if (!m_pDoc->m_pAttrCtrl->IsInterval45DegElemDirection(ElemK, iElemNo))
			{
				return TRUE;
			}
		}
		return FALSE;
	};

	BOOL bConstraint[6] = { FALSE, };
	m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(NodeK_I, bConstraint, FALSE);
	BOOL bFixed_I = (bConstraint[4]) ? TRUE : FALSE;
	m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(NodeK_J, bConstraint, FALSE);
	BOOL bFixed_J = (bConstraint[4]) ? TRUE : FALSE;

	if (bFixed_I == FALSE)  bFixed_I = _LIsContinuesBeam(NodeK_I);
	if (bFixed_J == FALSE)  bFixed_J = _LIsContinuesBeam(NodeK_J);

	if (m_pDoc->m_pPostCtrl->IsPinNode(iElemNo, 0, FALSE))  bFixed_I = FALSE;
	if (m_pDoc->m_pPostCtrl->IsPinNode(iElemNo, 1, FALSE))  bFixed_J = FALSE;

	if (bFixed_I == FALSE && bFixed_J == FALSE)      nSupport = EN_SUPPORT_PIN_PIN;
	else if (bFixed_I == TRUE && bFixed_J == TRUE)   nSupport = EN_SUPPORT_FIX_FIX;
	else                                             nSupport = EN_SUPPORT_FIX_PIN;

	return nSupport;
}


void CDgnDataCtrl::Get_CrackData_EN04(const int& nNationalAnnex, const EN_EC2_EXPOSURE_CLASS& enClass, const EN_EC2_SWE_SERV_LIFE& enServLife, const bool& bCorrosionHigh, T_CRACK_EC2& CrackD)
{
	switch(nNationalAnnex)
	{
		
		case enNationalAnnex::Austria:
		case enNationalAnnex::Belgium:
		case enNationalAnnex::Cyprus:
		case enNationalAnnex::CzechRepublic:
		case enNationalAnnex::Greece:
		case enNationalAnnex::Germany:
		case enNationalAnnex::Italy:
		case enNationalAnnex::Luxembourg:
		case enNationalAnnex::Poland:
		case enNationalAnnex::Romania:
		case enNationalAnnex::Slovakia:
		case enNationalAnnex::Slovenia:
		case enNationalAnnex::Recommended:	Get_CrackData_EN04_REC(enClass, CrackD);	break;
		case enNationalAnnex::Denmark:		Get_CrackData_EN04_DNK(enClass, CrackD);	break;
		case enNationalAnnex::Netherlands:	Get_CrackData_EN04_NLD(enClass, CrackD);	break;
		case enNationalAnnex::Finland:		Get_CrackData_EN04_FIN(enClass, CrackD);	break;
		case enNationalAnnex::France:		Get_CrackData_EN04_FRA(enClass, CrackD);	break;
		case enNationalAnnex::Ireland:
		case enNationalAnnex::Malaysia:
		case enNationalAnnex::Singapore:
		case enNationalAnnex::UnitedKingdom:Get_CrackData_EN04_GBR(enClass, CrackD);	break;
		case enNationalAnnex::Norway:		Get_CrackData_EN04_NOR(enClass, enServLife, CrackD);	break;
		case enNationalAnnex::Spain:		Get_CrackData_EN04_ESP(enClass, CrackD);	break;
		case enNationalAnnex::Sweden:
		case enNationalAnnex::Sweden19:	
		{
			if(bCorrosionHigh)
			{
				switch (enServLife)
				{
					case EN_SWE_L100:	Get_CrackData_EN04_SWE_C_L100(enClass, CrackD);	break;
					case EN_SWE_L50:	Get_CrackData_EN04_SWE_C_L50(enClass, CrackD);	break;
					case EN_SWE_L20:	Get_CrackData_EN04_SWE_C_L20(enClass, CrackD);	break;
					default: ASSERT(0); break;
				}
			}
			else
			{
				switch (enServLife)
				{
					case EN_SWE_L100:	Get_CrackData_EN04_SWE_L_L100(enClass, CrackD);	break;
					case EN_SWE_L50:	Get_CrackData_EN04_SWE_L_L50(enClass, CrackD);	break;
					case EN_SWE_L20:	Get_CrackData_EN04_SWE_L_L20(enClass, CrackD);	break;
					default: ASSERT(0); break;
				}
			}
			break;
		}
		default: ASSERT(0);
		break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_REC(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
		case EN_XD1:
		case EN_XD2:
		case EN_XD3:
		case EN_XS1:
		case EN_XS2:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_DNK(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, false, false);
				CrackD.SetFlag    (false, false, false);
				CrackD.SetLimit   (0.0, 0.0, 0.0);
			}
			break;
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XD1:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XD2:
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_NLD(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XD3:
		case EN_XS1:
		case EN_XS2:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_FIN(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XD2:
		case EN_XD3:
		case EN_XS2:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
		case EN_XD1:
		case EN_XS1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_FRA(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XD3:
		case EN_XS1:
		case EN_XS2:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_GBR(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
		case EN_XC1:
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
		case EN_XD1:
		case EN_XD2:
		case EN_XD3:
		case EN_XS1:
		case EN_XS2:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_NOR(const EN_EC2_EXPOSURE_CLASS& enClass, const EN_EC2_SWE_SERV_LIFE& enServLife, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	const double dDelta_c_dev = 10.0; // 4.4.1.3
	const double dc_min_b = 10.0; // 4.4.1.2 (3) Diameter of bar not less than 10 mm
	const double dDelta_c_dur_gamma = 0.0; // 4.4.1.2 (6) recommended and NA value 0 mm
	const double dDelta_c_dur_st    = 0.0; // 4.4.1.2 (7) recommended and NA value 0 mm
	const double dDelta_c_dur_add   = 0.0; // 4.4.1.2 (8) recommended and NA value 0 mm

	const double dc_min_dur = [&]()
	{
		switch (enClass)
		{
			case EN_X0:		return dc_min_b;
			case EN_XC1:	return enServLife==EN_SWE_L100 ? 25.0 : 15.0;
			case EN_XC2:
			case EN_XC3:
			case EN_XC4:	return enServLife==EN_SWE_L100 ? 35.0 : 25.0;
			case EN_XD1:
			case EN_XS1:	return enServLife==EN_SWE_L100 ? 50.0 : 40.0;
			case EN_XD2:
			case EN_XD3:
			case EN_XS2:	return enServLife==EN_SWE_L100 ? 50.0 : 40.0;
			case EN_XS3:	return enServLife==EN_SWE_L100 ? 60.0 : 50.0;
			case EN_XF1:
			case EN_XF2:
			case EN_XF3:
			case EN_XF4:
			case EN_XA1:
			case EN_XA2:	return 0.0;
			case EN_XA3:	return enServLife==EN_SWE_L100 ? 50.0 : 40.0;
			default: ASSERT(0); break;
		}
	}();

	const double dc_min = max(max(dc_min_b, dc_min_dur + dDelta_c_dur_gamma - dDelta_c_dur_st - dDelta_c_dur_add), 10.0); // 4.4.1.2 EQ(4.2)
	const double dc_nom = dc_min + dDelta_c_dev;
	const double dkc = min(SafeDiv(dc_nom, dc_min_dur), 1.3); // c_nom/c_min_dur ; ��Ī�Ǻ� / �ּҳ����Ǻ�

	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC1:
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
		case EN_XD1:
		case EN_XD2:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3*dkc);
			}
			break;
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, true, false);
				CrackD.SetLimit   (0.0, 0.3*dkc, 0.3*dkc);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_ESP(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		case EN_X0:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC1:
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XD3:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.1);
			}
			break;
		case EN_XF1:
		case EN_XF3:
			{
				CrackD.SetActivate(true, false, true);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.0, 0.3);
			}
			break;
		case EN_XF2:
		case EN_XF4:
		case EN_XA1:
			{
				CrackD.SetActivate(true, false, true);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.0, 0.2);
			}
			break;
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, false, true);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.0, 0.1);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_C_L100(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		// Little corrosion sensitivity
		// L100
		case EN_X0:
			{
				CrackD.SetActivate(false, false, false);
				CrackD.SetFlag    (false, false, false);
				CrackD.SetLimit   (0.0, 0.0, 0.0);
			}
			break;
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.15);
			}
			break;
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.1);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_C_L50(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	Get_CrackData_EN04_SWE_L_L100(enClass, CrackD);
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_C_L20(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	Get_CrackData_EN04_SWE_L_L50(enClass, CrackD);
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_L_L100(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		// Little corrosion sensitivity
		// L100
		case EN_X0:
			{
				CrackD.SetActivate(false, false, false);
				CrackD.SetFlag    (false, false, false);
				CrackD.SetLimit   (0.0, 0.0, 0.0);
			}
			break;
		case EN_XC1:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.45);
			}
			break;
		case EN_XC2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.2);
			}
			break;
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.15);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_L_L50(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		// Little corrosion sensitivity
		// L100
		case EN_X0:
		case EN_XC1:
			{
				CrackD.SetActivate(false, false, false);
				CrackD.SetFlag    (false, false, false);
				CrackD.SetLimit   (0.0, 0.0, 0.0);
			}
			break;
		case EN_XC2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.45);
			}
			break;
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.3);
			}
			break;
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.20);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

void CDgnDataCtrl::Get_CrackData_EN04_SWE_L_L20(const EN_EC2_EXPOSURE_CLASS& enClass, T_CRACK_EC2& CrackD)
{
	// CHAR, FREQ, QUAS
	CrackD.Initialize();
	switch (enClass)
	{
		// Little corrosion sensitivity
		// L100
		case EN_X0:
		case EN_XC1:
		case EN_XC2:
		case EN_XC3:
		case EN_XC4:
			{
				CrackD.SetActivate(false, false, false);
				CrackD.SetFlag    (false, false, false);
				CrackD.SetLimit   (0.0, 0.0, 0.0);
			}
			break;
		case EN_XD1:
		case EN_XD2:
		case EN_XS1:
		case EN_XS2:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.4);
			}
			break;
		case EN_XD3:
		case EN_XS3:
			{
				CrackD.SetActivate(false, true, true);
				CrackD.SetFlag    (false, false, true);
				CrackD.SetLimit   (0.0, 0.4, 0.30);
			}
			break;
		case EN_XF1:
		case EN_XF2:
		case EN_XF3:
		case EN_XF4:
		case EN_XA1:
		case EN_XA2:
		case EN_XA3:
			{
				CrackD.SetActivate(true, true, false);
				CrackD.SetFlag    (true, false, false);
				CrackD.SetLimit   (0.2, 0.1, 0.0);
			}
			break;
		default: ASSERT(0); break;
	}
}

////////////////////////////////////////////////////////////////////////
CDgnDataCtrlStatusSaver::CDgnDataCtrlStatusSaver(CDgnDataCtrl* pDgnDataCtrl, int nType)
{

}

CDgnDataCtrlStatusSaver::~CDgnDataCtrlStatusSaver()
{
}

