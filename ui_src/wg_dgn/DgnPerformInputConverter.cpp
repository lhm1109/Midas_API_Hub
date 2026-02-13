#include "stdafx.h"
#include "DgnPerformInputConverter.h"

#include "../../dgnengine/idesign/DGN_def/DataTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnInput.h"
#include "../../dgnengine/idesign/DGN_def/SectionDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionPropertyDef.h"
#include "../../dgnengine/idesign/DGN_def/UnbracedLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/EffectiveLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/SteelGeneralSectParamDef.h"
#include "../../dgnengine/idesign/DGN_def/BendingCoefficientDef.h"
#include "../../dgnengine/idesign/DGN_def/ShearCoefficientDef.h"
#include "../../dgnengine/idesign/DGN_def/SafetyFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/SeismicPropertyDef.h"
#include "../../dgnengine/idesign/DGN_def/LimitSlendernessRatioDef.h"
#include "../../dgnengine/idesign/DGN_def/LcomInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/Force1DDef.h"
#include "../../dgnengine/idesign/DGN_def/MembTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnNodeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnElemDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnMembDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnMembGroupDef.h"
#include "../../dgnengine/idesign/DGN_def/BindingDataDef.h"
#include "../../dgnengine/idesign/DGN_def/Dgn1DPositionDef.h"
#include "../../dgnengine/idesign/DGN_def/Dgn2DPositionDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnTypeEnumDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnFrameTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/LcomDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialSteelDef.h"
#include "../../dgnengine/idesign/DGN_def/BoundaryDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaBeamDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaColumnDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaWallDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaAsDef.h"
#include "../../dgnengine/idesign/DGN_def/CheckOptionDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnRevisionDef.h"
#include "../../dgnengine/idesign/DGN_def/BetaAngleDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnLLRFDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSUEQDef.h"
#include "../../dgnengine/idesign/DGN_def/MomentRedistributionFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnTRFTDef.h"
#include "../../dgnengine/idesign/DGN_def/MembLengthDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnOffset1DDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSpcVerLcomDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnUnderLcomTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSameRebarJointDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnHaunchedBeamDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCodeDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCaseDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnLoadCaseDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberRebarBeamDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberRebarColumnDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberRebarWallDef.h"
#include "../../dgnengine/idesign/DGN_def/MaterialConcreteDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnRptCriticalInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnItemDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnRptCaseDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnRptEnvironmentDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnPMCalcOptionDef.h"
#include "../../dgnengine/idesign/DGN_def/MomentMagnifierDef.h"
#include "../../dgnengine/idesign/DGN_def/EquivalentMomentDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnDeflectionParamPerformDef.h"
#include "../../dgnengine/idesign/DGN_def/RebarRatioDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnLimitOptionDef.h"
#include "../../dgnengine/idesign/DGN_def/ProbableMomentStrengthBeamDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnStoryDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnStructureInfoDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnModuleDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignMomentStrengthBeamDef.h"
#include "../../dgnengine/idesign/DGN_def/TorsionDesignDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnCheckDirectionDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnWallIDDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnWallOptionsDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSlabOptionsDef.h"
#include "../../dgnengine/idesign/DGN_def/ForceOption2DDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSlabServParamDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnPointLoadDef.h"
#include "../../dgnengine/idesign/DGN_def/SlabShearReinforcementDef.h"
#include "../../dgnengine/idesign/DGN_def/SlabColumnLocationDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionRebar2DDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionRebar2DAddDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnCuttingLineDef.h"
#include "../../dgnengine/idesign/DGN_def/SlendernessApproximationDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaSlabDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnNodalCrackedDeformDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnConcStressStrainTypeDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnConcRebarStressMethodDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnColumnPilotiDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnColumnCapitalDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnBoundaryElementMethodDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnScaleUpfactorRSDef.h"
#include "../../dgnengine/idesign/DGN_def/ShearAngleDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnUnitDef.h"
#include "../../dgnengine/idesign/DGN_def/PartialFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/StressCheckDef.h"
#include "../../dgnengine/idesign/DGN_def/CrackParameterDef.h"
#include "../../dgnengine/idesign/DGN_def/LimitSlendernessRatioParamEUDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionRebarPlateDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionRebarGeneralDef.h"
#include "../../dgnengine/idesign/DGN_def/MainRebarSingle.h"
#include "../../dgnengine/idesign/DGN_def/DgnSpvmDef.h"
#include "../../dgnengine/idesign/DGN_def/SectionGeometryDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnShellOptionsDef.h"
#include "../../dgnengine/idesign/DGN_def/SandwichModelDef.h"
#include "../../dgnengine/idesign/DGN_def/DesignCriteriaPlateDef.h"
#include "../../dgnengine/idesign/DGN_def/BarExposureConditionDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberRebarGeneralDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnDefStiffRatioDef.h"
#include "../../dgnengine/idesign/DGN_def/StiffenerBaseDef.h"
#include "../../dgnengine/idesign/DGN_def/IStiffenerSingle.h"
#include "../../dgnengine/idesign/DGN_lib/IAlignedShapeTypical.h"
#include "../../dgnengine/idesign/DGN_lib/AlignedShapeRectangle.h"
#include "../../dgnengine/idesign/DGN_def/StiffenerRectangle.h"
#include "../../dgnengine/idesign/DGN_lib/AlignedShapeT.h"
#include "../../dgnengine/idesign/DGN_def/StiffenerT.h"
#include "../../dgnengine/idesign/DGN_lib/AlignedShapeURib.h"
#include "../../dgnengine/idesign/DGN_def/StiffenerURib.h"
#include "../../dgnengine/idesign/DGN_def/LongitudinalStiffenerDef.h"
#include "../../dgnengine/idesign/DGN_def/MembTransverseStiffenerDef.h"
#include "../../dgnengine/idesign/DGN_def/MemberVectorDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnSteelOptionsDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnDisp1DRelativeDef.h"
#include "../../dgnengine/idesign/DGN_def/DeflectionParameterStlDef.h"
#include "../../dgnengine/idesign/DGN_def/ImperfectionFactorDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnLoadPointMcrDef.h"
#include "../../dgnengine/idesign/DGN_def/ModularRatioDef.h"
#include "../../dgnengine/idesign/DGN_def/PrefOptionDef.h"
#include "../../dgnengine/idesign/DGN_def/DgnResponseModificationFactorDef.h"

#include "../../dgnengine/idesign/DGN_lib/DgnSectPropMaker.h"
#include "../../dgnengine/idesign/DGN_lib/IPair.h"
#include "../../dgnengine/idesign/DGN_lib/Macro.h"
#include "../../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../../dgnengine/idesign/DGN_link/CalcType.h"
#include "../../dgnengine/idesign/DGN_link/IDgnQuery.h"
#include "../../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../../dgnengine/idesign/DGN_link/IDgnPerform.h"
#include "../../dgnengine/idesign/DGN_link/ITmpDgnCalcBaseLib.h"

#include "../wg_db/wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_AttrCtrl2.h"
#include "../wg_db/wg_db_PostCtrl.h"
#include "../wg_db/wg_db_MembCtrl.h"
#include "../wg_db/wg_db_DBLib.h"
#include "../wg_db/wg_db_MatlDB.h"
#include "../wg_db/DB_ST_DT_SECT.h"
#include "../wg_db/wg_db_StageInfo.h"
#include "../wg_db/wg_db_DesignResult.h"
#include "../wg_db/wg_db_UnitCtrl.h"
#include "../wg_db/DBCodeDef.h"
#include "../wg_db/SCWBRatio.h"
#include "../wg_db/MathFunc.h"
#include "../wg_db/QSort.h"
#include "../wg_db/SectDB.h"
#include "../wg_db/DBDoc.h"

#include "../wg_base/ArrayUtil.h"
#include "../wg_base/ServiceProvider.h"
#include "../wg_gps/GPS_ServiceDef.h"

#include "MembTypeUtil.h"
#include "DgnTypeConverter.h"
#include "DgnCodeCtrl.h"
#include "IDgnPerformDataBase.h"
#include "DgnDataCtrl.h"
#include "DgnForceCtrl.h"
#include "DgnReportTargetItemFinderFactory.h"
#include "DgnReportTargetItemFinder.h"
#include <utility>
#include "TProductTypeStruct.h"
#include "IDgnPerformDataBaseRC.h"

#include "../../dgnengine/src/DgnCalcBase/DgnCalcBase_SectData_Maker.h" //?? 머여 이건


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::converter;
using namespace dgn::lib;
using namespace dgn::link;
using namespace dgn::def;
using namespace dgn::item;

CDgnPerformInputConverter::CDgnPerformInputConverter(IDgnPerformDataBase* pDataBase)
{
	m_pDataBase = pDataBase;
	const auto enDgnType = [this]()
	{
		if (m_pDataBase->IsRcsBeam()) return DGN_DPOOL_TYPE::BEAM;
		else if (m_pDataBase->IsRcsHcbm()) return DGN_DPOOL_TYPE::BEAM;
		else if (m_pDataBase->IsRcsColm()) return DGN_DPOOL_TYPE::COLUMN;
		else if (m_pDataBase->IsRcsBrce()) return DGN_DPOOL_TYPE::BRACE;
		else if (m_pDataBase->IsRcsWall()) return DGN_DPOOL_TYPE::WALL;
		else if (m_pDataBase->IsRcsSlab()) return DGN_DPOOL_TYPE::SLAB;
		else if (m_pDataBase->IsStl())     return DGN_DPOOL_TYPE::STL;
		else return DGN_DPOOL_TYPE::None;
	}();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->SetDgnDPoolType(enDgnType);
}

CDgnPerformInputConverter::~CDgnPerformInputConverter()
{
}

bool CDgnPerformInputConverter::AddPrefOption()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	std::vector<CBindingData> vBindingData;

	CPrefOption* pPrefOption = new CPrefOption;

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);

	pPrefOption->nDgnThread = (dgn::def::enPrefDgnThread)rPref.DgnCode.nDgnThread;

	CBindingData Data;
	Data.pData = pPrefOption;
	vBindingData.push_back(Data);

	if(!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCase(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	// for slab
	CArray<T_SBDO_K, T_SBDO_K> aSbdoList;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoList);
	int nSbdo = aSbdoList.GetSize();
	std::unordered_map<T_ELEM_K, T_SBDO_K> mSbdoElem;
	for (int i = 0; i < nSbdo; ++i)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoList[i], aElem);
		for (int j = 0; j < nElem; ++j)
		{
			mSbdoElem.insert(std::make_pair(aElem[j], aElem[0]));
		}
	}

	std::vector<CBindingData> vBindingData;

	std::vector<T_MEMB_K> vMemb; vMemb.clear();
	for (const auto& iMemb : vKeyMemb)
	{
		const auto pElem = mSbdoElem.find(iMemb);
		if (pElem == mSbdoElem.end())
		{
			T_ELEM_D eData;
			eData.Initialize();
			if (!pDoc->m_pAttrCtrl->GetElem(iMemb, eData))	continue;
			// Change by ZINU.('02.10.22).  Include not only column but also wall.
			if( pDoc->m_pAttrCtrl->IsWall(eData.eltyp) )  // Wall.
			{
				vMemb.push_back(iMemb);
			}
			else
			{
				T_ELEM_K ShowKey;
				if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(iMemb, ShowKey))
				{
					ASSERT(0); continue;
				}
				vMemb.push_back(ShowKey);
			}
		}
		else
		{
			vMemb.push_back(pElem->second);
		}
	}
	Macro::UniqueSort(vMemb);

	CDesignCase* pDesignCase = new CDesignCase;
	pDesignCase->strName = _T("I'm Nothing");
	const int nMemb = vMemb.size();
	pDesignCase->vChkMember.SetSize(nMemb);
	for (int i = 0; i < nMemb; ++i)
	{
		pDesignCase->vChkMember.PushBack(vMemb[i]);
		pDesignCase->mChkMemberIdx.insert(std::make_pair(vMemb[i], i));
	}
	pDesignCase->vValidElem.SetSize(vKeyMemb.size());
	for (const auto& iMemb : vKeyMemb)
	{
		pDesignCase->vValidElem.PushBack(iMemb);
	}

	Macro::FTArrayUnique(pDesignCase->vChkMember);
	pDesignCase->vDgnMember.Copy(pDesignCase->vChkMember);
	pDesignCase->KeyMcas = 1;
	pDesignCase->bUseLcom = false;
	pDesignCase->vLcom.RemoveAll();

	CBindingData Data;
	Data.pData = pDesignCase;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddStructureInfo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CDgnStructureInfo* pStructureInfo = new CDgnStructureInfo;

	T_BLDC_D BldcD;
	BldcD.Initialize();
	if (pDoc->m_pAttrCtrl->GetBldc(BldcD))	pStructureInfo->dGroundLevel = BldcD.dBaseLevel;
	else                                        pStructureInfo->dGroundLevel = 0.0;
	pStructureInfo->enStoryType = CDgnStructureInfo::enStory_u; // u타입

	CBindingData Data;
	Data.pData = pStructureInfo;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddFrameType()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	T_DCTL_D DctlD;
	DataCtrl.Get_DgnGenDctl(DctlD);

	std::vector<CBindingData> vBindingData;

	CDgnFrameType* pFrameType = new CDgnFrameType;
	pFrameType->DesignPlane = (CDgnFrameType::enDesignPlane)DctlD.nDesignType;
	pFrameType->xDir = DctlD.bBraceFrame_X ? CDgnFrameType::Braced_NonSway : CDgnFrameType::Unbraced_Sway;
	pFrameType->yDir = DctlD.bBraceFrame_Y ? CDgnFrameType::Braced_NonSway : CDgnFrameType::Unbraced_Sway;

	CBindingData Data;
	Data.pData = pFrameType;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddStory()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStory = aStorK.GetSize();

	CMap<T_STOR_K, T_STOR_K, double, double> mStor2Height;
	if (!pDoc->m_pAttrCtrl->GetStorHeight(aStorK, mStor2Height, TRUE))
	{
		ASSERT(0);
	}

	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < nStory; ++i)
	{
		T_STOR_D StorD;
		if (!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) continue;

		T_KEY_LIST aStoryElem;
		pDoc->m_pAttrCtrl->GetKeyListStorUser(aStorK[i], 1, NULL, &aStoryElem);// // 해당층과 층상부로 연결된 요소; u타입
		//if (aStoryElem.GetSize() == 0) continue;;

		CDgnStory* pStory = new CDgnStory;
		pStory->strName = CT2W(StorD.StoryName);
		pStory->dStoryLevel = StorD.dStoryLevel;
		if (!mStor2Height.Lookup(aStorK[i], pStory->dHeight))
		{
			pStory->dHeight = 0.0;
			ASSERT(0);
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, aStorK[i]);
		Data.pData = pStory;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		for (int j = 0; j < aStoryElem.GetSize(); ++j)
		{
			Query.vOnlyOneKey.push_back(aStoryElem[j]);
		}
		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddModule()
{
	//GEN 멀티 타워 지원 안함
	return true;
}

bool CDgnPerformInputConverter::AddNode(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;
	T_KEY_LIST aNodeK;
	pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeK);
	for (int i = 0; i < aNodeK.GetSize(); ++i)
	{
		T_NODE_D NodeD;
		if (!pDoc->m_pAttrCtrl->GetNode(aNodeK[i], NodeD))
		{
			ASSERT(0); continue;
		}

		auto* pDgnNode = new CDgnNode;
		pDgnNode->vPos[0] = NodeD.x;
		pDgnNode->vPos[1] = NodeD.y;
		pDgnNode->vPos[2] = NodeD.z;

		CBindingData Data;
		Data.pData = pDgnNode;
		Data.ForceKey = std::make_pair(true, aNodeK[i]);

		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddElem(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_D ElemD;
		auto ElemK = aElemK[i];
		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0); continue;
		}

		// element
		auto* pDgnElem = new CDgnElem;
		pDgnElem->enType = (dgn::def::enEl_Type)ElemD.eltyp;
		const int nNode = [&pDoc](const int& eltyp)
		{
			if (pDoc->m_pAttrCtrl->IsFrameType(eltyp))
				return 2;
			else if (pDoc->m_pAttrCtrl->IsPlaneType(eltyp))
				return 4;
			else if (pDoc->m_pAttrCtrl->IsSolidType(eltyp))
				return D_ELEM_MAXNOD;
			else
				ASSERT(0);

			return 2;
		}(ElemD.eltyp);
		for (int j = 0; j < nNode; ++j)
		{
			if (ElemD.elnod[j] == 0) break;
			pDgnElem->vNode.push_back(ElemD.elnod[j]);
		}

		CBindingData Data;
		Data.pData = pDgnElem;
		Data.ForceKey = std::make_pair(true, ElemK);

		CRelationalQuery Query;
		Query.DataType = DataType::Node;
		Query.vOnlyOneKey = pDgnElem->vNode;
		Data.vQueryInfo.push_back(Query);

		vBindingData.push_back(Data);

		// element beta-angle
		auto* pBetaAngle = new CBetaAngle;
		pBetaAngle->dDegree = ElemD.angle;

		CBindingData DataAngle;
		DataAngle.pData = pBetaAngle;
		DataAngle.ForceKey = std::make_pair(true, ElemK);

		CRelationalQuery QueryAngle;
		QueryAngle.DataType = DataType::Elem;
		QueryAngle.vOnlyOneKey.push_back(ElemK);
		DataAngle.vQueryInfo.push_back(QueryAngle);

		vBindingData.push_back(DataAngle);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSpvaData(const std::vector<long>& aKeyMemb) {
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aMembK;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_K ShowKey;
		if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
		{
			ASSERT(0); continue;
		}
		aMembK.Add(ShowKey);
	}

	CArrayUtil::RemoveDup(aMembK);

	std::unordered_map<UINT, bool> mMemb;

	bool bExistWall = false;
	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;
	auto nType = -1;
	auto nTypePlateBeam = -1;
	bool bPlateBeamDetected = false;

	//Find Maximum spvm if its Plate Beam
	for (int i = 0; i < aMembK.GetSize(); ++i)
	{
		T_ELEM_D elem_data;
		elem_data.Initialize();
		T_ELEM_K key = aMembK.GetAt(i);
		pDoc->m_pAttrCtrl->GetElem(key, elem_data);
		if (pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_PLATEBEAM)
		{
			nTypePlateBeam =max(nTypePlateBeam, DataCtrl.Get_DgnGenSpva(key));
			bPlateBeamDetected = true;
		}
	}


	for (int i = 0; i < aMembK.GetSize(); ++i)
	{
		T_SPVM_K MembK = aMembK[i];
		//T_SPVM_D MbtpD;
		T_ELEM_D elem_data;
		elem_data.Initialize();
		T_ELEM_K key = aMembK.GetAt(i);
		pDoc->m_pAttrCtrl->GetElem(key, elem_data);
		//MbtpD.Initialize();
		nType = (bPlateBeamDetected && pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_PLATEBEAM) ? nTypePlateBeam: DataCtrl.Get_DgnGenSpva(MembK);
		

		//if (nType != -1) 
		{
			auto* pDgnSpvm = new CDgnSpvm;

			if (nType == 1) {
				pDgnSpvm->MembType = enSpvm::Mono;
			}
			else if (nType == 2) {
				pDgnSpvm->MembType = enSpvm::SS;
			}
			else if (nType == 0) {
				pDgnSpvm->MembType = enSpvm::None;
			}
			else pDgnSpvm->MembType = enSpvm::Other;
			CBindingData Data;
			Data.pData = pDgnSpvm;
			Data.ForceKey = std::make_pair(true, MembK);


			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Elem;
			RQuery.vOnlyOneKey.push_back(MembK);
			Data.vQueryInfo.push_back(RQuery);

			vBindingData.push_back(Data);
		}
	
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddMemb(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aMembK;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_K ShowKey;
		if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
		{
			ASSERT(0); continue;
		}
		aMembK.Add(ShowKey);
	}

	CArrayUtil::RemoveDup(aMembK);

	std::unordered_map<UINT, bool> mMemb;

	bool bExistWall = false;
	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < aMembK.GetSize(); ++i)
	{
		T_MEMB_K MembK = aMembK[i];
		T_MBTP_D MbtpD;
		MbtpD.Initialize();
		DataCtrl.Get_DgnGenMbtp(MembK, MbtpD);

		T_KEY_LIST aElemK;
		BOOL bRev = FALSE;
		T_MEMB_K ShowK = 0;
		if (MbtpD.nMbType != D_MBTP_WALL)
		{
			if (!CMembCtrl::GetMembCtrl()->GetElemListByIncludeElem(MembK, bRev, ShowK, aElemK))
			{
				ASSERT(0);
				continue;
			}
		}
		else
		{
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(MembK, nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_K WallK;
			T_WALL_D WallD;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD))          continue;
			ShowK = WallD.RootElemKey;
			if (!pDoc->m_pPostCtrl->GetWallElemList(WallK.keymap, aElemK))     continue;
			bExistWall = true;
		}

		//이미 넣었던 Key는 제낀다.
		if (mMemb.find(ShowK) != mMemb.end()) continue;

		T_HCBM_K HcbmK;
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(ShowK, HcbmK))
		{
			T_HCBM_D HcbmD;
			if (!pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD)) ASSERT(0);
			for (int iPart = 0; iPart < 3; ++iPart)
			{
				if (ShowK == HcbmD.aElemListPart[iPart].GetAt(0))
				{
					auto* pDgnMemb = new CDgnMemb;
					int nElem = HcbmD.aElemListPart[iPart].GetSize();
					for (int j = 0; j < nElem; ++j)
					{
						pDgnMemb->vElem.push_back(HcbmD.aElemListPart[iPart].GetAt(j));
					}
					CString strName;
					switch (iPart)
					{
					case 0:
						strName.Format(_T("%s Part A"), HcbmD.HaunchName);
						break;
					case 1:
						strName.Format(_T("%s Part B"), HcbmD.HaunchName);
						break;
					case 2:
						strName.Format(_T("%s Part C"), HcbmD.HaunchName);
						break;
					default:
						break;
					}

					pDgnMemb->MembType.DgnType = enDgn::RCS;
					pDgnMemb->MembType.MembType = enMemb::Beam;
					pDgnMemb->bReverse = bRev;
					pDgnMemb->strName = CT2W(strName);

					CBindingData Data;
					Data.pData = pDgnMemb;
					Data.ForceKey = std::make_pair(true, ShowK);

					CRelationalQuery Query;
					Query.DataType = DataType::Elem;
					Query.vOnlyOneKey = pDgnMemb->vElem;
					Data.vQueryInfo.push_back(Query);

					vBindingData.push_back(Data);

					mMemb.insert(std::make_pair(ShowK, true));

					break;
				}
			}
		}
		else
		{
			auto* pDgnMemb = new CDgnMemb;
			int nElem = aElemK.GetSize();
			for (int j = 0; j < nElem; ++j) {
				pDgnMemb->vElem.push_back(aElemK[j]);
			}

			T_MATD_D MatdD;
			DataCtrl.Get_DgnGenMatd(aElemK[0], MatdD);
			
			if(MatdD.Type == _T("C"))			pDgnMemb->MembType.DgnType = enDgn::RCS;
			else if(MatdD.Type == _T("S"))		pDgnMemb->MembType.DgnType = enDgn::STL;
			else if(MatdD.Type == _T("SRC"))	pDgnMemb->MembType.DgnType = enDgn::SRC;
			else                                pDgnMemb->MembType.DgnType = enDgn::None;
			pDgnMemb->MembType.ElemType = enModelElemType::Elem1D;

			CString strName;
			switch (MbtpD.nMbType)
			{
			case D_MBTP_COLUMN:
				pDgnMemb->MembType.MembType = enMemb::Column;
				strName.Format(_T("Column %d"), aElemK[0]);
				break;
			case D_MBTP_BEAM:
				pDgnMemb->MembType.MembType = enMemb::Beam;
				strName.Format(_T("Beam %d"), aElemK[0]);
				break;
			case D_MBTP_BRACE:
				pDgnMemb->MembType.MembType = enMemb::Brace;
				strName.Format(_T("Brace %d"), aElemK[0]);
				break;
			case D_MBTP_WALL:
				pDgnMemb->MembType.MembType = enMemb::Wall;
				strName.Format(_T("Wall %d"), aElemK[0]);
				break;
			case D_MBTP_ETC:
			case D_MBTP_SHELL:
			case D_MBTP_PLATEBEAM:
			case D_MBTP_PLATECOLM:
			{
				_SAFE_DELETE(pDgnMemb);
				continue;
			}
			break;
			default:
				ASSERT(0);
				break;
			}
			pDgnMemb->bReverse = bRev;
			pDgnMemb->strName = CT2W(strName);

			CBindingData Data;
			Data.pData = pDgnMemb;
			Data.ForceKey = std::make_pair(true, ShowK);

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey = pDgnMemb->vElem;
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);

			mMemb.insert(std::make_pair(ShowK, true));
		}
	}

	std::map<T_ELEM_K, int> mSubSubDomain;
	std::map<int, T_VECTOR3BY3D> mSubSubDomainVec;
	// slab, plate beam, plate colummn
	std::vector<T_SBDO_K> aSbdoKeys; // Sub domain key list. (설계시는 sub-domain만 사용)
	int nSbdo = DataCtrl.Get_DgnConSbdoKList(EN_DGNSBDO_ALL, aSbdoKeys);
	for (int i = 0; i < nSbdo; ++i)
	{
		T_SBDO_K SbdoK = aSbdoKeys[i];
		T_ELEM_K_LIST aSbdoElem;
		int nSbdoElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
		if (nSbdoElem == 0) continue;

		auto* pDgnMemb = new CDgnMemb;
		for (int j = 0; j < nSbdoElem; ++j)
		{
			pDgnMemb->vElem.push_back(aSbdoElem[j]);
		}

		int nMbtp = pDoc->m_pAttrCtrl->GetMemberType(aSbdoElem[0]);

		pDgnMemb->MembType.DgnType = enDgn::RCS;
		pDgnMemb->MembType.MembType = CDgnTypeConverter::GetMembType(nMbtp);
		pDgnMemb->MembType.ElemType = enModelElemType::Elem2D;

		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) SbdoD.Initialize();
		T_MADO_D MadoD;
		if (!pDoc->m_pAttrCtrl->GetMado(SbdoD.KeyMado, MadoD)) MadoD.Initialize();
		CString strDomain = _T("");
		strDomain.Format(_T("DOMAIN %s-%s"), MadoD.strName, SbdoD.strName);
		pDgnMemb->strName = CT2W(strDomain);

		CBindingData Data;
		Data.pData = pDgnMemb;
		Data.ForceKey = std::make_pair(true, aSbdoElem[0]); // element key is unique.

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		Query.vOnlyOneKey = pDgnMemb->vElem;
		Data.vQueryInfo.push_back(Query);

		vBindingData.push_back(Data);

		//Subdomain Member Vector
		double v3d[3][3] = { { 0.0, }, };
#if defined (_CIVIL)
		if (SbdoD.nRebarAxisType == 0)
		{
			if (!pDoc->calcPlaneLocalVector(aSbdoElem[0], v3d))
			{
				ASSERT(0); continue;
			}
		}
		else if (SbdoD.nRebarAxisType == 1)
		{
			if (SbdoD.strUCS == _LS(IDS_WG_GPS_Current_UCS))
			{
				v3d[0][0] = 1.0;
				v3d[0][1] = 0.0;
				v3d[0][2] = 0.0;

				v3d[1][0] = 0.0;
				v3d[1][1] = 1.0;
				v3d[1][2] = 0.0;

				v3d[2][0] = 0.0;
				v3d[2][1] = 0.0;
				v3d[2][2] = 1.0;
			}
			else
			{
				T_NUCS_D NucsD;
				if (!pDoc->m_pAttrCtrl->GetNucs(SbdoD.strUCS, NucsD))
				{
					ASSERT(0); // 이상황이 안생길 것 같지만 혹시모르니 기본값으로 세팅하도록 코드 넣어 놓자..
					v3d[0][0] = 1.0;
					v3d[0][1] = 0.0;
					v3d[0][2] = 0.0;

					v3d[1][0] = 0.0;
					v3d[1][1] = 1.0;
					v3d[1][2] = 0.0;

					v3d[2][0] = 0.0;
					v3d[2][1] = 0.0;
					v3d[2][2] = 1.0;
					return TRUE;
				}

				v3d[0][0] = NucsD.dVx[0];
				v3d[0][1] = NucsD.dVx[1];
				v3d[0][2] = NucsD.dVx[2];

				v3d[1][0] = NucsD.dVy[0];
				v3d[1][1] = NucsD.dVy[1];
				v3d[1][2] = NucsD.dVy[2];

				CMathFunc::mathNormal(v3d[0], v3d[1], v3d[2]); // z축은 x와 y의 외적
			}
			// UCS 기준으로 element에 프로젝션 된 방향으로 계산을 해야 하므로.. 여기서 Return 하지 않는다.
		}
		else if (SbdoD.nRebarAxisType == 2)
		{
			double dCoor[3][3];
			memset(dCoor, 0, sizeof(dCoor));
			for (int k = 0; k < 2; k++)
			{
				dCoor[k + 1][0] = SbdoD.dAxisVector[k][0];
				dCoor[k + 1][1] = SbdoD.dAxisVector[k][1];
				dCoor[k + 1][2] = SbdoD.dAxisVector[k][2];
			}
			if (!pDoc->calcPlaneLocalVector(3, dCoor, 0.0, v3d))
			{
				continue;
			}
		}
		else
		{
			ASSERT(0);
			continue;
		}
#else				
		if(!pDoc->m_pPostCtrl->Get_SBDO_LocalVector(SbdoK, v3d, mSubSubDomain, mSubSubDomainVec, TRUE))
		{
			double dVecGlobal[3][3] = { { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };
			memcpy(v3d, dVecGlobal, sizeof(v3d));
		}		
#endif // 				

		if ( m_pDataBase->IsDesigning() )
		{
			if ( nMbtp == D_MBTP_PLATEBEAM )
			{
				T_PBDD_D PbddD;
				if ( CMembCtrl::GetMembCtrl()->GetPbddByElem(aSbdoElem[0], PbddD) )
				{
					if ( PbddD.nDir == 1 )
					{
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[0][0], v3d[0][1], v3d[0][2]);
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[1][0], v3d[1][1], v3d[1][2]);
					}
				}
			}
			else if ( nMbtp == D_MBTP_PLATECOLM )
			{
				T_PCDD_D PcddD;
				if ( CMembCtrl::GetMembCtrl()->GetPcddByElem(aSbdoElem[0], PcddD) )
				{
					if ( PcddD.nDir == 1 )
					{
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[0][0], v3d[0][1], v3d[0][2]);
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[1][0], v3d[1][1], v3d[1][2]);
					}
				}
			}
		}
		else
		{
			if ( nMbtp == D_MBTP_PLATEBEAM )
			{
				T_PBDC_D PbdcD;
				if ( CMembCtrl::GetMembCtrl()->GetPbdcByElem(aSbdoElem[0], PbdcD) )
				{
					if ( PbdcD.nOptDir == 1 )
					{
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[0][0], v3d[0][1], v3d[0][2]);
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[1][0], v3d[1][1], v3d[1][2]);
					}
				}
			}
			else if ( nMbtp == D_MBTP_PLATECOLM )
			{
				T_PCDC_D PcddD;
				if ( CMembCtrl::GetMembCtrl()->GetPcdcByElem(aSbdoElem[0], PcddD) )
				{
					if ( PcddD.nOptDir == 1 )
					{
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[0][0], v3d[0][1], v3d[0][2]);
						CMathFunc::mathRotate(-90.0, 0., 0., 0., v3d[2][0], v3d[2][1], v3d[2][2], v3d[1][0], v3d[1][1], v3d[1][2]);
					}
				}
			}
		}
		
		auto *pMembVector = new CMemberVector;

		pMembVector->bAuto = false;
		memcpy(&pMembVector->Vec, v3d, sizeof(v3d));

		CBindingData MembVecData;
		MembVecData.pData = pMembVector;
		MembVecData.ForceKey = std::make_pair(true, aSbdoElem[0]);

		CRelationalQuery MembQuery;
		MembQuery.DataType = DataType::Memb;
		MembQuery.vOnlyOneKey.push_back(aSbdoElem[0]);
		MembVecData.vQueryInfo.push_back(MembQuery);

		vBindingData.push_back(MembVecData);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}


bool CDgnPerformInputConverter::AddBoundary(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	auto lGetFixStatus = [](const T_FRLS_D& FrlsD, int nPos, int nDof) -> int
	{
		if (FrlsD.nType == 0) // relative.
		{
			return (FrlsD.Release[nPos][nDof] == '0' || (FrlsD.Release[nPos][nDof] == '1' && FrlsD.Fixity[nPos][nDof] == 1.0)) ? 0 : 1;
		}
		else
		{
			return (FrlsD.Release[nPos][nDof] == '0') ? 0 : 1;
		}
	};

	std::vector<CBindingData> vBindingData;

	// beam-end-release
	std::unordered_map<UINT, CBoundary*> mFrls;

	// boundary
	T_KEY_LIST aNodeK;
	pDoc->m_pAttrCtrl->GetConsNodeKeyList(aNodeK);
	for (int i = 0; i < aNodeK.GetSize(); ++i)
	{
		T_CONS_D ConsD;
		if (!pDoc->m_pAttrCtrl->GetConsNode(aNodeK[i], ConsD))
		{
			continue;
		}

		CBoundary* pBoundary = new CBoundary;
		for (int j = 0; j < 7; ++j)
		{
			pBoundary->bFix[j] = ConsD.Constraint[j] == '1' ? true : false;
		}
		mFrls.insert(std::make_pair(aNodeK[i], pBoundary));
	}

	size_t nMemb = vKeyMemb.size();
	for (int i = 0; i < nMemb; ++i)
	{
		T_ELEM_K KeyElem = vKeyMemb[i];
		T_MEMB_K MembK;
		BOOL bMembAssigned = pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, MembK);

		T_FRLS_D FrlsD[2];
		if (!pDoc->m_pAttrCtrl->GetFrlsElem_Fix(KeyElem, FrlsD[0]))
		{
			if(!bMembAssigned)	continue;
			FrlsD[0].Initialize();
		}
		
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(KeyElem, ElemD)) continue;
		T_NODE_K elnod[2] = { (UINT)ElemD.elnod[0], (UINT)ElemD.elnod[1]};

		if(bMembAssigned)
		{
			T_MEMB_D MembD;
			if (!pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))
			{
				ASSERT(0);
				FrlsD[1] = FrlsD[0];
			}
			else
			{
				const int nElem = MembD.aElemList.GetSize();
				const int ElemJ = MembD.aElemList[nElem-1];
				if (!pDoc->m_pAttrCtrl->GetFrlsElem_Fix(ElemJ, FrlsD[1]))
				{
					FrlsD[1].Initialize();
				}
				if (!pDoc->m_pAttrCtrl->GetElem(ElemJ, ElemD)) continue;
				elnod[1] = ElemD.elnod[1];
			}
		}

		for (int j = 0; j < 2; ++j) // i, j
		{
			T_NODE_K NodeK = elnod[j];

			CBoundary* pBoundary = nullptr;
			auto itrFrls = mFrls.find(NodeK);
			if (itrFrls == mFrls.end())
				pBoundary = new CBoundary;
			else
				pBoundary = itrFrls->second;

			for (int k = 0; k < 7; k++)
			{
				pBoundary->bFix[k] = lGetFixStatus(FrlsD[j], j, k) == 0 ? false : true;
			}

			mFrls[NodeK] = pBoundary;
		}
	}

	for (auto itr = mFrls.begin(); itr != mFrls.end(); ++itr)
	{
		CBindingData Data;
		Data.pData = itr->second;
		Data.ForceKey = std::make_pair(true, itr->first);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCode(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	auto* pDesignCode = new CDesignCode;

	const auto DgnType = m_pDataBase->GetDgnType();
	switch (DgnType)
	{
	case enDgn::RCS:
	{
		T_DCON_D DConD;
		if (DataCtrl.Get_DgnConDcon(DConD))
		{
			int nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
#if defined(_CIVIL )
			int nSubCode = DConD.nNationalAnnex;
#else
			int nSubCode = DConD.nNationalAnnex;
#endif
			pDesignCode->RC = dgn::converter::CDgnTypeConverter::GetDgnCode(nRCCode, nSubCode);
			pDesignCode->RC_unitL = static_cast<enLengthUnit>(m_pDataBase->GetCodeUnitLength());
			pDesignCode->RC_unitF = static_cast<enForceUnit>(m_pDataBase->GetCodeUnitForce());
		}
	}
	break;
	case enDgn::STL:
	{
		T_DSTL_D DstlD;
		if (DataCtrl.Get_DgnStlDstl(DstlD))
		{
			int nStlCode = CDgnCodeCtrl::GetStlCodeID(DstlD.DesignCode);
			pDesignCode->STL = dgn::converter::CDgnTypeConverter::GetDgnCode(nStlCode, DstlD.nNationalAnnex);
			pDesignCode->STL_unitL = static_cast<enLengthUnit>(m_pDataBase->GetCodeUnitLength());
			pDesignCode->STL_unitF = static_cast<enForceUnit>(m_pDataBase->GetCodeUnitForce());
		}
	}
	break;
	case enDgn::SRC:
	case enDgn::CSG:
	case enDgn::CFS:
	default:
		ASSERT(0);
		break;
	}

	std::vector<CBindingData> vBindingData;

	CBindingData Data;
	Data.pData = pDesignCode;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSect(const std::vector<long>& aKeyMemb)
{
	// Add Section
	if (!AddSectForDesign(aKeyMemb))
	{
		ASSERT(0); return false;
	}

	// Add Section for design
	if (!AddSectForDesign(aKeyMemb, true))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSectForDesign(const std::vector<long>& aKeyMemb, bool bDesign)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	CArray<UINT, UINT> aProp;
	CArray<UINT, UINT> aWall;

	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD)) {
			ASSERT(0); continue;
		}

		if (pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
		{
			aProp.Add(ElemD.elpro);
		}
		else if (pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
		{
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(aElemK[i], nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_K WallK;
			T_WALL_D WallD;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD))          continue;

			T_ELEM_K nElemK = WallD.RootElemKey;
			aWall.Add(nElemK);
		}
	}
	CArrayUtil::RemoveDup(aProp);
	CArrayUtil::RemoveDup(aWall);
	std::vector<CBindingData> vBindingData;

	for (int i = 0; i < aWall.GetSize(); ++i)
	{
		if (bDesign)
			continue;
		
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(aWall[i], ElemD)) {
			ASSERT(0); continue;
		}

		unsigned int iInterWallID = 0;
		pDoc->m_pPostCtrl->GetInternalWallId(aWall[i], iInterWallID);

		T_WALL_K WallK;
		if (!pDoc->m_pPostCtrl->GetWallKey(iInterWallID, WallK.keymap)) {
			ASSERT(0); continue;
		}

		T_WALL_D WallD;
		if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD)) {
			ASSERT(0); continue;
		}

		T_THIK_K ThikK = [this, pDoc](T_WALL_D& DataWall)
		{
			CQSort::QSortUInt(DataWall.arElemKey.GetData(), DataWall.arElemKey.GetSize());
			T_ELEM_D DataElem;
			for (int j = 0; j < DataWall.arElemKey.GetSize(); j++)
			{
				pDoc->m_pAttrCtrl->GetElem(DataWall.arElemKey[j], DataElem);
				double dWallThik = 0.0;
				if (!pDoc->m_pAttrCtrl->GetThikForWallAssemble(DataElem.elpro, dWallThik))	dWallThik = 0.0;
				if(EQ(DataWall.dThickness, dWallThik))	return DataElem.elpro;
			}
			pDoc->m_pAttrCtrl->GetElem(DataWall.arElemKey[0], DataElem);
			return DataElem.elpro;
		}(WallD);
		
		if(!m_pDataBase->IsDesigning())
		{
			T_REBW_K RebwK;
			RebwK.keymap = WallK.keymap;
			T_REBW_D RebwD;
			if(pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD))
			{
				if(!RebwD.bUseModelThik) WallD.dThickness = RebwD.dThickness;
			}

		}
		T_THIK_D ThikD;
		if (!pDoc->m_pAttrCtrl->GetThik(ThikK, ThikD)) {
			ASSERT(0); continue;
		}

		auto* pSection = new CSection;
		pSection->strShape    = _T("SB");
		pSection->strSection  = CT2W(ThikD.Value.strName);
		pSection->iSectNo     = ElemD.elpro;
		pSection->enShapeType = CSection::EN_SECT_SHAPE_REG;
		pSection->enSectType  = CSection::EN_ROLLED;
		pSection->enSectShape = CSection::EN_SECT_SHAPE_SB;
		pSection->adSize[0]   = WallD.dLength;
		pSection->adSize[1]   = WallD.dThickness;

		CBindingData PropData;
		PropData.pData = pSection;

		CRelationalQuery PropQuery;
		PropQuery.DataType    = DataType::Elem;
		PropQuery.vOnlyOneKey = Macro::CArray2Vec<T_ELEM_K, long>(WallD.arElemKey);
		PropData.vQueryInfo.push_back(PropQuery);
		vBindingData.push_back(PropData);
	}

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	const double dUnitWidth = [&]()
	{
		if (vSbdo.size() == 0) return 1.0;
		int nRCCode = 0;
		T_DCON_D DConD;
		if (CDgnDataCtrl().Get_DgnConDcon(DConD))
		{
			nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
		}
		return Get_UnitWidth(nRCCode);
	}();

	CArray<T_THIK_K, T_THIK_K> aKeyList;
	pDoc->m_pAttrCtrl->GetThikKeyList(aKeyList);

	if (bDesign)
	{
		for (const auto SbdoK : vSbdo)
		{
			T_SBDO_D SbdoD;
			if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) continue;
			if (SbdoD.nType != 1 && SbdoD.nType != 2) continue;

			T_ELEM_K_LIST aSbdoElem;
			int nSbdoElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
			if (nSbdoElem == 0) continue;

			const T_ELEM_K ElemK = aSbdoElem[0];

			T_ELEM_D ElemD;
			if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) {
				ASSERT(0); continue;
			}

			T_THIK_D ThikD;
			pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);

			if (!SbdoD.bUseMt && SbdoD.dThickness > 0.0)
			{
				//강제로 이 두께를 사용한다.
			}
			else
			{
				continue;
			}

			auto* pSection        = new CSection;
			pSection->strShape    = _T("SB");
			pSection->strSection  = CT2W(ThikD.Value.strName);
			pSection->iSectNo     = ElemD.elpro;
			pSection->enShapeType = CSection::EN_SECT_SHAPE_REG;
			pSection->enSectType  = CSection::EN_ROLLED;
			pSection->enSectShape = CSection::EN_SECT_SHAPE_SB;
			pSection->adSize[0]   = SbdoD.dThickness;
			pSection->adSize[1]   = dUnitWidth;

			CBindingData PropData;
			PropData.pData = pSection;

			CRelationalQuery PropQuery;
			PropQuery.DataType = DataType::Elem;
			for (int j = 0; j < aSbdoElem.GetSize(); ++j)
			{
				PropQuery.vOnlyOneKey.push_back(aSbdoElem[j]);
			}
			PropData.vQueryInfo.push_back(PropQuery);
			vBindingData.push_back(PropData);
		}
	}
	else
	{
		for (int i = 0; i < aKeyList.GetSize(); ++i)
		{
			T_THIK_K Thick = aKeyList[i];
			CArray<T_THIK_K, T_THIK_K> DummyThick; DummyThick.Add(Thick);
			CArray<T_NODE_K, T_NODE_K> aNodeList;
			CArray<T_ELEM_K, T_ELEM_K> aElemList;
			pDoc->m_pAttrCtrl->GetKeyListThikUser(&DummyThick, &aNodeList, &aElemList);

			if (aElemList.GetSize() == 0)
				continue;

			T_THIK_D ThikD;
			pDoc->m_pAttrCtrl->GetThik(Thick, ThikD);

			auto* pSection = new CSection;
			pSection->strShape    = _T("SB");
			pSection->strSection  = CT2W(ThikD.Value.strName);
			pSection->iSectNo     = Thick;
			pSection->enShapeType = CSection::EN_SECT_SHAPE_REG;
			pSection->enSectType  = CSection::EN_ROLLED;
			pSection->enSectShape = CSection::EN_SECT_SHAPE_SB;

			pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, pSection->adSize[0]);
			pSection->adSize[1] = dUnitWidth;

			CBindingData PropData;
			PropData.pData = pSection;

			CRelationalQuery PropQuery;
			PropQuery.DataType = DataType::Elem;
			for (int j = 0; j < aElemList.GetSize(); ++j)
			{
				PropQuery.vOnlyOneKey.push_back(aElemList[j]);
			}
			PropData.vQueryInfo.push_back(PropQuery);
			vBindingData.push_back(PropData);
		}
	}
	
    auto _l_ConvertSectToSection = [] (const T_SECT_D& SectD, UINT SectK, CSection* pSection)
    {
        pSection->strShape = CT2W(SectD.SectBefore.Shape);
        pSection->strSection = [&] ()
        {
            if ( SectD.SectBefore.SectI.SName.IsEmpty() )
            {
                return SectD.SName;
            }
            else
            {
                CString strName = SectD.SName + _T(", ") + SectD.SectBefore.SectI.SName;
                return strName;
            }
        }();
        pSection->iSectNo = SectK;
        pSection->enSectType = SectD.SectBefore.SectI.BuiltUpFlag == 1 ? CSection::EN_BUILT_UP : CSection::EN_ROLLED;
        bool bSectJ = false;
        pSection->enSectShape = [&SectD, &bSectJ] ()
        {
            if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_L )
            {
                return CSection::EN_SECT_SHAPE_L;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_C )
            {
                return CSection::EN_SECT_SHAPE_C;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_I )
            {
                return CSection::EN_SECT_SHAPE_STG_I;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_H )
            {
                return CSection::EN_SECT_SHAPE_H;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T )
            {
                return CSection::EN_SECT_SHAPE_T;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B )
            {
                return CSection::EN_SECT_SHAPE_B;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_B )
            {
                return CSection::EN_SECT_SHAPE_STG_B;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P )
            {
                return CSection::EN_SECT_SHAPE_P;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB )
            {
                return CSection::EN_SECT_SHAPE_SB;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR )
            {
                return CSection::EN_SECT_SHAPE_SR;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_2L )
            {
                return CSection::EN_SECT_SHAPE_2L;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_2C )
            {
                return CSection::EN_SECT_SHAPE_2C;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_CC )
            {
                return CSection::EN_SECT_SHAPE_CC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_URIB )
            {
                return CSection::EN_SECT_SHAPE_URIB;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT )
            {
                return CSection::EN_SECT_SHAPE_OCT;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT )
            {
                return CSection::EN_SECT_SHAPE_SOCT;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK )
            {
                return CSection::EN_SECT_SHAPE_TRK;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK )
            {
                return CSection::EN_SECT_SHAPE_STRK;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK )
            {
                return CSection::EN_SECT_SHAPE_HTRK;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_CL )
            {
                return CSection::EN_SECT_SHAPE_CL;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_ROCT )
            {
                return CSection::EN_SECT_SHAPE_ROCT;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_BSTF )
            {
                return CSection::EN_SECT_SHAPE_BSTF;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_PSTF )
            {
                return CSection::EN_SECT_SHAPE_PSTF;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN )
            {
                return CSection::EN_SECT_SHAPE_GEN;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_UDT )
            {
                return CSection::EN_SECT_SHAPE_UDT;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_CB )
            {
                return CSection::EN_SECT_SHAPE_CB;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_UP )
            {
                return CSection::EN_SECT_SHAPE_UP;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_Z )
            {
                return CSection::EN_SECT_SHAPE_Z;
            }
            // SRC
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RBO )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RBO;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RBC )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RBC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RPO )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RPO;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RPC )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RPC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CBO )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_CBO;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CBC )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_CBC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CPO )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_CPO;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CPC )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_CPC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EBC )
            {
                return CSection::EN_SECT_SHAPE_SRC_EBC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EPC )
            {
                return CSection::EN_SECT_SHAPE_SRC_EPC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EPO )
            {
                return CSection::EN_SECT_SHAPE_SRC_EPO;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_EBC )
            {
                return CSection::EN_SECT_SHAPE_SRC_EBC;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RIB )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RIB;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_CIB )
            {
				bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_CIB;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RH2T )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RH2T;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_RHT )
            {
                bSectJ = true;
                return CSection::EN_SECT_SHAPE_SRC_RHT;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_BSTF )
            {
                return CSection::EN_SECT_SHAPE_SRC_BSTF;
            }
            else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_PSTF )
            {
                return CSection::EN_SECT_SHAPE_SRC_PSTF;
            }
            else
            {
                return CSection::EN_SECT_SHAPE_GEN;
            }
        }();

        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_I )
        {
            pSection->adSize[0] = SectD.SectBefore.SectI.Size[4] + SectD.SectBefore.SectI.Size[5] + SectD.SectBefore.SectI.Size[6];
            pSection->adSize[1] = SectD.SectBefore.SectI.Size[0] + SectD.SectBefore.SectI.Size[1];
            pSection->adSize[2] = SectD.SectBefore.SectI.Size[7];
            pSection->adSize[3] = SectD.SectBefore.SectI.Size[5];
            pSection->adSize[4] = SectD.SectBefore.SectI.Size[2] + SectD.SectBefore.SectI.Size[3];
            pSection->adSize[5] = SectD.SectBefore.SectI.Size[6];
        }
        else
        {
            if ( bSectJ )
                memcpy(&pSection->adSize, SectD.SectBefore.SectJ.Size, sizeof(double) * CSection::Number);
            else
                memcpy(&pSection->adSize, SectD.SectBefore.SectI.Size, sizeof(double) * CSection::Number);
        }

        if ( SectD.nStype == D_SECT_TYPE_TAPERED )
        {
            pSection->enShapeType = CSection::EN_SECT_SHAPE_TAP;
            memcpy(&pSection->adTaperedSize, SectD.SectBefore.SectJ.Size, sizeof(double) * CSection::Number);
        }
        else
        {
            pSection->enShapeType = CSection::EN_SECT_SHAPE_REG;
        }

        pSection->bConsiderWarpingEffect = SectD.SectBefore.bConsiderWarpingEffect;
    };

	std::vector<UINT> vTaperedProp;
	for (int i = 0; i < aProp.GetSize(); ++i)
	{
		T_SECT_D SectD;
		if (bDesign)
		{
			if (!pDoc->m_pAttrCtrl->GetSectDesign(aProp[i], SectD)) {
				continue;
			}
		}
		else
		{
			if (!pDoc->m_pAttrCtrl->GetSect(aProp[i], SectD, FALSE)) {
				ASSERT(0);  continue;
			}
		}

        T_KEY_LIST aKeyList, aKeyElem;
        aKeyList.Add(aProp[i]);
        pDoc->m_pAttrCtrl->GetKeyListSectUser(&aKeyList, nullptr, &aKeyElem);
		T_TSGR_K TsgrK = pDoc->m_pAttrCtrl->GetElemTsgrKey(aKeyElem[0]);
        if ( SectD.nStype == D_SECT_TYPE_TAPERED && TsgrK != 0 )
        {	//Tapered Group인 경우
            vTaperedProp.push_back(aProp[i]);
            continue;
        }

		auto* pSection = new CSection;
		_l_ConvertSectToSection(SectD, aProp[i], pSection);
		CBindingData Data;
		Data.pData = pSection;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		for (int j = 0; j < aKeyElem.GetSize(); ++j)
		{
			Query.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);

		//For General Section
		if (pSection->enSectShape == CSection::EN_SECT_SHAPE_GEN)
		{
			auto* pGenSection = new CSectionGeometry;
			int nOuterSize = SectD.SectBefore.SectI.aOuterPolygon.GetSize();
			if (nOuterSize == 0)
			{   //이런단면도 설계를 지원해?
				/*ASSERT(0);*/ _SAFE_DELETE(pGenSection); continue;
			}

			for (int j = 0; j < nOuterSize; ++j)
			{
				auto& Outer = SectD.SectBefore.SectI.aOuterPolygon[j];
				int nVertexSize = Outer.aVertex.GetSize();
				pGenSection->vPolygon.resize(nVertexSize);
				for (int k = 0; k < nVertexSize; ++k)
				{
					pGenSection->vPolygon[k].vec[0] = Outer.aVertex[k].dX;
					pGenSection->vPolygon[k].vec[1] = Outer.aVertex[k].dY;
				}
			}

			int nInnerSize = SectD.SectBefore.SectI.aInnerPolygon.GetSize();
			pGenSection->vInnerPolygon.resize(nInnerSize);
			for (int j = 0; j < nInnerSize; ++j)
			{
				auto& Inner = SectD.SectBefore.SectI.aInnerPolygon[j];
				int nVertexSize = Inner.aVertex.GetSize();
				pGenSection->vInnerPolygon[j].resize(nVertexSize);
				for (int k = 0; k < nVertexSize; ++k)
				{
					pGenSection->vInnerPolygon[j][k].vec[0] = Inner.aVertex[k].dX;
					pGenSection->vInnerPolygon[j][k].vec[1] = Inner.aVertex[k].dY;
				}
			}

			CBindingData Data;
			Data.pData = pGenSection;
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}

		T_MATD_D MatdD;
		if ( !CDgnDataCtrl().Get_DgnGenMatd(aKeyElem[0], MatdD) )
		{
			continue; // User
		}
		const double dFu = MatdD.Data1.Design.S_Fu;

		const double& dElastic = MatdD.Data1.Analysis.Elast;
		const double& dPoisson = MatdD.Data1.Analysis.Poisson;
		const double& dWeightDensity = MatdD.Data1.Analysis.Density;

		TMaterialCommonProp MComm(dElastic, dPoisson, dWeightDensity);
		TMaterialSteelSingle MSingle(MatdD.Data1.Design.S_Fy1, dFu);
		TStiffenerMaterial MtStfn(MComm, MSingle);
		//Longitudinal Stiffener
		const T_SECT_SOD_STIFF_D& SODStiffener = SectD.SectBefore.SectI.SODStiffener;
		const CString& strShape = SectD.SectBefore.Shape;
		const int nStiffSize = SODStiffener.aSODStiffSub.GetSize();
		if ( nStiffSize > 0 )
		{
			auto* pLongitudinalStiff = new CLongitudinalStiffener;
			std::vector<IStiffenerSingle*> vpSingle;
			for ( int j = 0; j <nStiffSize; ++j )
			{
				const T_SECT_SOD_STIFF_SUB_D& CurStiffSub = SODStiffener.aSODStiffSub[j];
				double dPosZ = 0.0;
				int nStiffDataSize = CurStiffSub.aStiffData.GetSize();
				for ( int k = 0; k<nStiffDataSize; ++k )
				{
					for ( int l = 0; l< SODStiffener.aStiffShape.GetSize(); ++l )
					{
						if (CurStiffSub.aStiffData[k].strShapeName == SODStiffener.aStiffShape[l].strName )
						{
							dgn::def::enPart enSPart = [strShape, &CurStiffSub] ()
							{
								if (strShape == D_SECT_SHAPE_STLG_I)
								{
									switch (CurStiffSub.nDeckPos )
									{
									case 0:// Top
										return enPart::Top;
									case 1:// WEB
										return enPart::Web;
									case 2:// BOTTOM
										return enPart::Bot;
									default:
										ASSERT(0);
										break;
									}
									return enPart::Web;
								}
								else if (strShape == D_SECT_SHAPE_STLG_B)
								{
									switch ( CurStiffSub.nDeckPos )
									{
									case 0:// Top
										return enPart::Top;
									case 1:// WEB
										return enPart::WebL;
									case 2:// WEB
										return enPart::WebR;
									case 3:// BOTTOM
										return enPart::Bot;
									default:
										ASSERT(0);
										break;
									}
									return enPart::Web;
								}
								else
								{
									ASSERT(0);
									return enPart::Web;
								}
							}();

							if ( CurStiffSub.aStiffData[k].bCalc == false )
								continue;

							dPosZ = CurStiffSub.aStiffData[k].dSpacing;
							dgn::lib::enTypicalType enType;
							TStiffenerLocation Location;
							Location.Location = [CurStiffSub, k, enSPart] ()
							{
								switch ( enSPart )
								{
								case enPart::Top:
									return enStiffenerLocation::Bot;
								case enPart::Bot:
									return enStiffenerLocation::Top;
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									{
										switch (CurStiffSub.aStiffData[k].nStiffPos )
										{
										case 0:
											return enStiffenerLocation::Left;
										case 1:
											return enStiffenerLocation::Right;
										case 2:
											return enStiffenerLocation::Both;
										default:
											break;
										}
										ASSERT(0);
										return enStiffenerLocation::Left;
									}
									break;
								default:
									break;
								}
								ASSERT(0);
								return enStiffenerLocation::Left;
							}();
							Location.RefPoint = [enSPart, CurStiffSub] ()
							{
								switch ( enSPart )
								{
								case enPart::Top:
									return CurStiffSub.nRefPos == 0 ? enStiffenerRefPoint::Left : enStiffenerRefPoint::Right;
								case enPart::Bot:
									return CurStiffSub.nRefPos == 0 ? enStiffenerRefPoint::Left : enStiffenerRefPoint::Right;
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									return CurStiffSub.nRefPos == 0 ? enStiffenerRefPoint::Top : enStiffenerRefPoint::Bottom;
								default:
									break;
								}
								ASSERT(0);
								return enStiffenerRefPoint::None;
							}();

							Location.RefPart = [enSPart, CurStiffSub] ()
							{
								switch ( enSPart )
								{
								case enPart::Top:
									return (enStiffenerRefPart)(CurStiffSub.nDeckPart + 1);
								case enPart::Bot:
									return (enStiffenerRefPart)(CurStiffSub.nDeckPart + 1);
								case enPart::Web:
								case enPart::WebL:
								case enPart::WebR:
									return enStiffenerRefPart::None;
								default:
									break;
								}
								return enStiffenerRefPart::None;
							}();

							auto Shape = SODStiffener.aStiffShape[l];
							switch ( SODStiffener.aStiffShape[l].nType )
							{
							case 0: //I
								{
									enType = enTypicalType::Rectangle;
									CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
									auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
									ShapeRect.SetAlign(AlignToStfn);

									CStiffenerRectangle LSShape(ShapeRect, MtStfn, enSPart, Location);
									vpSingle.push_back(LSShape.Clone());
								}
								break;
							case 1: //T
								{
									enType = enTypicalType::T;
									double dB = Shape.dSize[1];
									double dH = Shape.dSize[0];
									double dtw = Shape.dSize[2];
									double dtf = Shape.dSize[3];
									CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
									auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
									ShapeT.SetAlign(AlignToStfn);

									CStiffenerT LSShape(ShapeT, MtStfn, enSPart, Location);
									vpSingle.push_back(LSShape.Clone());
								}
								break;
							case 2: //U
								{
									enType = enTypicalType::URib;
									double dB = Shape.dSize[1];
									double dH = Shape.dSize[0];
									double dBb = Shape.dSize[2];
									double dt = Shape.dSize[3];
									double dR = Shape.dSize[4];
									CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
									auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(0.0, dPosZ));
									ShapeU.SetAlign(AlignToStfn);

									CStiffenerURib LSShape(ShapeU, MtStfn, enSPart, Location);
									vpSingle.push_back(LSShape.Clone());
								}
								break;
							default:
								ASSERT(0);
								break;
							}
						}
					}
				}
			}
			pLongitudinalStiff->Set(vpSingle);

			CBindingData Data;
			Data.pData = pLongitudinalStiff;
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}

		//Transverse Stiffener
		T_TVSF_D tvsfD;
		if ( pDoc->m_pAttrCtrl2->GetTvsf(aProp[i], tvsfD) )
		{
			auto *pMembTransverseStiff = new CMembTransverseStiffener;
			for ( int sect = 0; sect<3; ++sect )
			{
				CTransverseStiffener* pStiffener = new CTransverseStiffener;
				TMaterialCommonProp MComm(dElastic, dPoisson, dWeightDensity);
				TMaterialSteelSingle MSingle(MatdD.Data1.Design.S_Fy1, dFu);
				TStiffenerMaterial MtStfn(MComm, MSingle);

				CAlignedShapeRectangle ShapeRect(tvsfD.Tvsf[sect].dht, tvsfD.Tvsf[sect].dt, CAlign2D());
				auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(tvsfD.Tvsf[sect].dPitch, 0.0));

				CStiffenerRectangle* pTSShape = new CStiffenerRectangle(ShapeRect, MtStfn, enPart::Web, TStiffenerLocation());

				auto *pTransData = new TTransverseData;
				pTransData->dPitch = tvsfD.Tvsf[sect].dPitch;
				pTransData->dInternalRigidPitch = tvsfD.Tvsf[sect].dDistRstif;
				pTransData->dNonRigidPitch = tvsfD.Tvsf[sect].dDistNRstif;
				pTransData->bInternalRigid = tvsfD.Tvsf[sect].bInterRigidStif;
				pTransData->bNonRigid = tvsfD.Tvsf[sect].bInterNonRigidStif;
				pTransData->enEndType = enEndSupport::NonRigidEndPost;

				pStiffener->Set(pTSShape, *pTransData);
				enSector Sector = [sect] ()
				{
					switch ( sect )
					{
					case 0:
						return enSector::I;
					case 1:
						return enSector::M;
					case 2:
						return enSector::J;
					default:
						ASSERT(0);
						return enSector::I;
					}
				}();
				pMembTransverseStiff->Set(Sector, *pStiffener);
			}

			CBindingData Data;
			Data.pData = pMembTransverseStiff;
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}

	}

	//Tapered Section처리. sectionproperty랑 겹치는데 성능 느리면 처리 필요
	for ( auto TaperedProp : vTaperedProp )
	{
        T_KEY_LIST aKeyList, aKeyElem;
        aKeyList.Add(TaperedProp);
        pDoc->m_pAttrCtrl->GetKeyListSectUser(&aKeyList, nullptr, &aKeyElem);

        T_SECT_D SectD;
        if ( !pDoc->m_pAttrCtrl->GetSect(TaperedProp, SectD, FALSE) )
        {
            ASSERT(0); continue;
        }

        for ( int i = 0; i<aKeyElem.GetSize(); ++i )
        {
            T_TSGR_K TsgrK = pDoc->m_pAttrCtrl->GetElemTsgrKey(aKeyElem[i]);
            if ( TsgrK == 0 )
            {
                ASSERT(0); continue;
            }

            T_ELEM_D ElemD;
            if ( !pDoc->m_pAttrCtrl->GetElem(aKeyElem[i], ElemD) )
            {
                ASSERT(0); continue;
            }

            double dTsgrTotLength = 0.0, dPositionI = 0.0;
            T_TSGR_D TsgrD;
            if ( !pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD) ) { ASSERT(0); continue; }
            if ( !pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, aKeyElem[i], dTsgrTotLength, dPositionI) )
            {
                ASSERT(0); continue;
            }
            double dPositionJ = dPositionI + pDoc->calcLAVElem(ElemD);
            T_SECT_D TaperedD;
            if ( !pDoc->m_pSectDB->CalcTaperedSection(SectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, TaperedD) ) { ASSERT(0); continue; }

            auto* pSection = new CSection;
            _l_ConvertSectToSection(TaperedD, TaperedProp, pSection);

            CBindingData PropData;
            PropData.pData = pSection;

            CRelationalQuery PropQuery;
            PropQuery.DataType = DataType::Elem;
            PropQuery.vOnlyOneKey.push_back(aKeyElem[i]);
            PropData.vQueryInfo.push_back(PropQuery);
            vBindingData.push_back(PropData);
        }
	}

	std::vector<DWORD_PTR> vIter;
	if (!SetDataPool(vBindingData, vIter))
	{
		ASSERT(0); return false;
	}

	if (bDesign)
	{
		// set Revision data
		// Section for design
		if (AddMembQueryFromElemQuery(vBindingData))
		{
			if (!AddRevisionData(DataType::Section, vBindingData, vIter)) {
				ASSERT(0); return false;
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddSectProperty(const std::vector<long>& aKeyMemb)
{
	// Add Section
	if (!AddSectPropertyForDesign(aKeyMemb)) {
		ASSERT(0); return false;
	}

	// Add Section for design
	if (!AddSectPropertyForDesign(aKeyMemb, true))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSectPropertyForDesign(const std::vector<long>& aKeyMemb, bool bDesign)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	CArray<UINT, UINT> aProp;
	CArray<UINT, UINT> aWall;

	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))
		{
			ASSERT(0); continue;
		}

		if (pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
		{
			aProp.Add(ElemD.elpro);
		}
		else if (pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
		{
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(aElemK[i], nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_K WallK;
			T_WALL_D WallD;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD))          continue;

			T_ELEM_K nElemK = WallD.RootElemKey;
			aWall.Add(nElemK);
		}
	}
	CArrayUtil::RemoveDup(aProp);
	CArrayUtil::RemoveDup(aWall);

	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < aWall.GetSize(); ++i)
	{
		if (bDesign)
		{
			continue;
		}

		double dB = 0.0, dH = 0.0;
		if (!pDoc->m_pAttrCtrl->GetWallSize(aWall[i], dB, dH))
		{
			ASSERT(0); continue;
		}

		unsigned int iInterWallID = 0;
		pDoc->m_pPostCtrl->GetInternalWallId(aWall[i], iInterWallID);

		T_WALL_K WallK;
		if (!pDoc->m_pPostCtrl->GetWallKey(iInterWallID, WallK.keymap)) {
			ASSERT(0); continue;
		}

		T_WALL_D WallD;
		if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD)) {
			ASSERT(0); continue;
		}

		double dSize[8] = { 0.0, };
		dSize[0] = dH;
		dSize[1] = dB;
		auto* pSectionProperty = new CSectionProperty;
		if (!IDgnSectPropMaker::Instance()->Run_RegularSection(CSection::EN_SECT_SHAPE_SB, dSize, *pSectionProperty))
		{
			ASSERT(0); continue;
		}
		CBindingData PropData;
		PropData.pData = pSectionProperty;

		CRelationalQuery PropQuery;
		PropQuery.DataType = DataType::Elem;
		PropQuery.vOnlyOneKey = Macro::CArray2Vec<T_ELEM_K, long>(WallD.arElemKey);
		PropData.vQueryInfo.push_back(PropQuery);
		vBindingData.push_back(PropData);
	}

    auto _l_ConvertSectDToSectionProperty = [] (const T_SECT_D& SectD, OUT CSectionProperty* pSectionProperty)
    {
        const T_SECT_STIFFNESS& SectStiff = SectD.nStype == D_SECT_TYPE_COMBINED ? SectD.SectBefore.Stiffness : SectD.SectBefore.SectI.Stiffness;
        pSectionProperty->adProp[CSectionProperty::Area] = SectStiff.Area;
        pSectionProperty->adProp[CSectionProperty::Asx] = SectStiff.Asy;
        pSectionProperty->adProp[CSectionProperty::Asy] = SectStiff.Asz;
        pSectionProperty->adProp[CSectionProperty::J] = SectStiff.Rxx;
        pSectionProperty->adProp[CSectionProperty::Ix] = SectStiff.Ryy;
        pSectionProperty->adProp[CSectionProperty::Iy] = SectStiff.Rzz;
        pSectionProperty->adProp[CSectionProperty::Cx] = SectStiff.Cym;
        pSectionProperty->adProp[CSectionProperty::Cxp] = SectStiff.Cyp;
        pSectionProperty->adProp[CSectionProperty::Cxm] = SectStiff.Cym;
        pSectionProperty->adProp[CSectionProperty::Cy] = SectStiff.Czm;
        pSectionProperty->adProp[CSectionProperty::Cyp] = SectStiff.Czp;
        pSectionProperty->adProp[CSectionProperty::Cym] = SectStiff.Czm;
        pSectionProperty->adProp[CSectionProperty::Qxb] = SectStiff.Qyb;
        pSectionProperty->adProp[CSectionProperty::Qyb] = SectStiff.Qzb;
        pSectionProperty->adProp[CSectionProperty::x1] = SectStiff.dx1;
        pSectionProperty->adProp[CSectionProperty::y1] = SectStiff.dy1;
        pSectionProperty->adProp[CSectionProperty::x2] = SectStiff.dx2;
        pSectionProperty->adProp[CSectionProperty::y2] = SectStiff.dy2;
        pSectionProperty->adProp[CSectionProperty::x3] = SectStiff.dx3;
        pSectionProperty->adProp[CSectionProperty::y3] = SectStiff.dy3;
        pSectionProperty->adProp[CSectionProperty::x4] = SectStiff.dx4;
        pSectionProperty->adProp[CSectionProperty::y4] = SectStiff.dy4;

        const T_SECT_DESIGN& SectDesign = SectD.nStype == D_SECT_TYPE_COMBINED ? SectD.SectBefore.Design : SectD.SectBefore.SectI.Design;
        pSectionProperty->adProp[CSectionProperty::Cw] = SectDesign.Cwp;
        pSectionProperty->adProp[CSectionProperty::Sx] = SectDesign.Syy;
        pSectionProperty->adProp[CSectionProperty::Sy] = SectDesign.Szz;
        pSectionProperty->adProp[CSectionProperty::Zx] = SectDesign.Zyy;
        pSectionProperty->adProp[CSectionProperty::Zy] = SectDesign.Zzz;
        pSectionProperty->adProp[CSectionProperty::Xbar] = SectDesign.YBar;
        pSectionProperty->adProp[CSectionProperty::Ybar] = SectDesign.ZBar;
        pSectionProperty->adProp[CSectionProperty::Rox] = SectDesign.Roy;
        pSectionProperty->adProp[CSectionProperty::Roy] = SectDesign.Roz;
        pSectionProperty->adProp[CSectionProperty::Rop] = SectDesign.Rop;

        const T_SECT_SECTBASE_D& SectI = SectD.SectBefore.SectI;
        const T_SECT_SECTBASE_D& SectJ = SectD.SectBefore.SectJ;
        double dc = 0.0;
        if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_CL )          dc = SectI.Size[4];
        else if ( SectD.SectBefore.Shape == D_SECT_SHAPE_COMB_4L )    dc = SectJ.Size[0];
        pSectionProperty->adProp[CSectionProperty::Ixy] = [] (CString sType, double H, double B1, double tw, double tf1, double Ybar, double Zbar, double C) -> double
        {
            //단면 상승 모멘트 (Gen Manual I.개요-p.90)
            double dRyz = 0.0;
            if ( sType == D_SECT_SHAPE_REG_L )
            {
                dRyz = (B1*tf1)*(Ybar - B1 / 2.0)*(Zbar - (H - tf1 / 2.0)) + ((H - tf1)*tw)*(Ybar - tw / 2.0)*(Zbar - (H - tf1) / 2.0);
            }
            else if ( sType == D_SECT_SHAPE_REG_CL || sType == D_SECT_SHAPE_COMB_4L )
            {
                double dRyzf = (B1*tf1)*(C / 2.0 + tf1 / 2.0)*(C / 2.0 + B1 / 2.0);
                double dRyzw = ((H - tf1)*tw)*(C / 2.0 + tw / 2.0)*(C / 2.0 + H / 2.0 + tf1 / 2.0);
                dRyz = (sType == D_SECT_SHAPE_COMB_4L ? 4.0 : 2.0) * (dRyzf + dRyzw);
            }
            return dRyz;
        }(SectD.SectBefore.Shape, SectI.Size[0], SectI.Size[1], SectI.Size[2], SectI.Size[3], SectDesign.YBar, SectDesign.ZBar, dc);

        if ( SectD.nStype == D_SECT_TYPE_TAPERED )
        {
            const T_SECT_STIFFNESS& SectJStiff = SectD.SectBefore.SectJ.Stiffness;
            pSectionProperty->adTaperedProp[CSectionProperty::Area] = SectJStiff.Area;
            pSectionProperty->adTaperedProp[CSectionProperty::Asx] = SectJStiff.Asy;
            pSectionProperty->adTaperedProp[CSectionProperty::Asy] = SectJStiff.Asz;
            pSectionProperty->adTaperedProp[CSectionProperty::J] = SectJStiff.Rxx;
            pSectionProperty->adTaperedProp[CSectionProperty::Ix] = SectJStiff.Ryy;
            pSectionProperty->adTaperedProp[CSectionProperty::Iy] = SectJStiff.Rzz;
            pSectionProperty->adTaperedProp[CSectionProperty::Cx] = SectJStiff.Cym;
            pSectionProperty->adTaperedProp[CSectionProperty::Cxp] = SectJStiff.Cyp;
            pSectionProperty->adTaperedProp[CSectionProperty::Cy] = SectJStiff.Czm;
            pSectionProperty->adTaperedProp[CSectionProperty::Cyp] = SectJStiff.Czp;
            pSectionProperty->adTaperedProp[CSectionProperty::Qxb] = SectJStiff.Qyb;
            pSectionProperty->adTaperedProp[CSectionProperty::Qyb] = SectJStiff.Qzb;
            pSectionProperty->adTaperedProp[CSectionProperty::x1] = SectJStiff.dx1;
            pSectionProperty->adTaperedProp[CSectionProperty::y1] = SectJStiff.dy1;
            pSectionProperty->adTaperedProp[CSectionProperty::x2] = SectJStiff.dx2;
            pSectionProperty->adTaperedProp[CSectionProperty::y2] = SectJStiff.dy2;
            pSectionProperty->adTaperedProp[CSectionProperty::x3] = SectJStiff.dx3;
            pSectionProperty->adTaperedProp[CSectionProperty::y3] = SectJStiff.dy3;
            pSectionProperty->adTaperedProp[CSectionProperty::x4] = SectJStiff.dx4;
            pSectionProperty->adTaperedProp[CSectionProperty::y4] = SectJStiff.dy4;

            const T_SECT_DESIGN& SectJDesign = SectD.SectBefore.SectJ.Design;
            pSectionProperty->adTaperedProp[CSectionProperty::Cw] = SectJDesign.Cwp;
            pSectionProperty->adTaperedProp[CSectionProperty::Sx] = SectJDesign.Syy;
            pSectionProperty->adTaperedProp[CSectionProperty::Sy] = SectJDesign.Szz;
            pSectionProperty->adTaperedProp[CSectionProperty::Zx] = SectJDesign.Zyy;
            pSectionProperty->adTaperedProp[CSectionProperty::Zy] = SectJDesign.Zzz;
            pSectionProperty->adTaperedProp[CSectionProperty::Xbar] = SectJDesign.YBar;
            pSectionProperty->adTaperedProp[CSectionProperty::Ybar] = SectJDesign.ZBar;
            pSectionProperty->adTaperedProp[CSectionProperty::Rox] = SectJDesign.Roy;
            pSectionProperty->adTaperedProp[CSectionProperty::Roy] = SectJDesign.Roz;
            pSectionProperty->adTaperedProp[CSectionProperty::Rop] = SectJDesign.Rop;

            const T_SECT_SECTBASE_D& SectJ = SectD.SectBefore.SectJ;
            double dc = 0.0;
            if ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_CL )          dc = SectJ.Size[4];
            pSectionProperty->adTaperedProp[CSectionProperty::Ixy] = [] (CString sType, double H, double B1, double tw, double tf1, double Ybar, double Zbar, double C) -> double
            {
                //단면 상승 모멘트 (Gen Manual I.개요-p.90)
                double dRyz = 0.0;
                if ( sType == D_SECT_SHAPE_REG_L )
                {
                    dRyz = (B1*tf1)*(Ybar - B1 / 2.0)*(Zbar - (H - tf1 / 2.0)) + ((H - tf1)*tw)*(Ybar - tw / 2.0)*(Zbar - (H - tf1) / 2.0);
                }
                return dRyz;
            }(SectD.SectBefore.Shape, SectJ.Size[0], SectJ.Size[1], SectJ.Size[2], SectJ.Size[3], SectJDesign.YBar, SectJDesign.ZBar, dc);
        }
    };

	std::vector<UINT> vTaperedProp;
	for (int i = 0; i < aProp.GetSize(); ++i)
	{
		T_SECT_D SectD;
		if (bDesign)
		{
			if (!pDoc->m_pAttrCtrl->GetSectDesign(aProp[i], SectD))
			{
				ASSERT(0); continue;
			}
		}
		else
		{
			if (!pDoc->m_pAttrCtrl->GetSect(aProp[i], SectD, FALSE))
			{
				ASSERT(0); continue;
			}
		}

        T_KEY_LIST aKeyList, aKeyElem;
        aKeyList.Add(aProp[i]);
        pDoc->m_pAttrCtrl->GetKeyListSectUser(&aKeyList, nullptr, &aKeyElem);
        T_TSGR_K TsgrK = pDoc->m_pAttrCtrl->GetElemTsgrKey(aKeyElem[0]);
        if ( SectD.nStype == D_SECT_TYPE_TAPERED && TsgrK != 0 )
		{
			vTaperedProp.push_back(aProp[i]);
			continue;
		}

		auto* pSectionProperty = new CSectionProperty;
		_l_ConvertSectDToSectionProperty(SectD, pSectionProperty);

		CBindingData PropData;
		PropData.pData = pSectionProperty;

		CRelationalQuery PropQuery;
		PropQuery.DataType = DataType::Elem;
		for (int j = 0; j < aKeyElem.GetSize(); ++j)
		{
			PropQuery.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		PropData.vQueryInfo.push_back(PropQuery);
		vBindingData.push_back(PropData);
	}
	
	//Tapered에 대한 처리. Tapered인 경우 요소의 Tapered Section을 본체에서 넘기고 중간 요소에 1/2, M, 3/4에 대한 Property값은 엔진 내부에서 계산한다.
	for ( auto TaperedProp : vTaperedProp )
	{
        T_KEY_LIST aKeyList, aKeyElem;
        aKeyList.Add(TaperedProp);
		pDoc->m_pAttrCtrl->GetKeyListSectUser(&aKeyList, nullptr, &aKeyElem);

        T_SECT_D SectD;
        if ( !pDoc->m_pAttrCtrl->GetSect(TaperedProp, SectD, FALSE) )
        {
            ASSERT(0); continue;
        }

		for ( int i = 0; i<aKeyElem.GetSize(); ++i )
        {
			T_TSGR_K TsgrK = pDoc->m_pAttrCtrl->GetElemTsgrKey(aKeyElem[i]);
			if ( TsgrK == 0 )
			{
				ASSERT(0); continue;
			}
			
            T_ELEM_D ElemD;
            if ( !pDoc->m_pAttrCtrl->GetElem(aKeyElem[i], ElemD) )
            {
                ASSERT(0); continue;
            }

			double dTsgrTotLength = 0.0, dPositionI = 0.0;
			T_TSGR_D TsgrD;
            if ( !pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD) ) { ASSERT(0); continue; }
			if ( !pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, aKeyElem[i], dTsgrTotLength, dPositionI) )
			{
				ASSERT(0); continue;
			}
			double dPositionJ = dPositionI + pDoc->calcLAVElem(ElemD);
			T_SECT_D TaperedD;
			if ( !pDoc->m_pSectDB->CalcTaperedSection(SectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, TaperedD) ) { ASSERT(0); continue; }

			auto* pSectionProperty = new CSectionProperty;
			_l_ConvertSectDToSectionProperty(TaperedD, pSectionProperty);

            CBindingData PropData;
            PropData.pData = pSectionProperty;

            CRelationalQuery PropQuery;
            PropQuery.DataType = DataType::Elem;
            PropQuery.vOnlyOneKey.push_back(aKeyElem[i]);
            PropData.vQueryInfo.push_back(PropQuery);
            vBindingData.push_back(PropData);
		}
	}

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	const double dB = [&]()
	{
		if (vSbdo.size() == 0) return 1.0;
		int nRCCode = 0;
		T_DCON_D DConD;
		if (CDgnDataCtrl().Get_DgnConDcon(DConD))
		{
			nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
		}
		return Get_UnitWidth(nRCCode);
	}();

	auto L_IsElemWallType = [pDoc](const T_ELEM_K ElemK)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0);
			return false;
		}

		return pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) ? true : false;
	};

	if (bDesign)
	{
		CArray<T_THIK_K, T_THIK_K> aKeyList;
		pDoc->m_pAttrCtrl->GetThikKeyList(aKeyList);
		for (int i = 0; i < aKeyList.GetSize(); ++i)
		{
			T_THIK_K Thick = aKeyList[i];
			CArray<T_THIK_K, T_THIK_K> DummyThick;
			DummyThick.Add(Thick);

			CArray<T_NODE_K, T_NODE_K> aNodeList;
			CArray<T_ELEM_K, T_ELEM_K> aElemList;
			pDoc->m_pAttrCtrl->GetKeyListThikUser(&DummyThick, &aNodeList, &aElemList);

			if (aElemList.GetSize() == 0)
				continue;

			T_THIK_D ThikD;
			pDoc->m_pAttrCtrl->GetThik(Thick, ThikD);

 			/* auto* pSection = new CSection;
 			pSection->strShape = _T("SB");
 			pSection->strSection = CT2W(ThikD.Value.strName);
 			pSection->iSectNo = Thick;
 			pSection->enShapeType = CSection::EN_SECT_SHAPE_REG;
 			pSection->enSectType = CSection::EN_ROLLED;
 			pSection->enSectShape = CSection::EN_SECT_SHAPE_SB; */

			double dH = 0.0;
			pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dH);

			auto* pSectionProperty = new CSectionProperty;
			pSectionProperty->adProp[CSectionProperty::Area] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::Asx] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::Asy] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::J] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Ix] = dB * dH * dH * dH / 12.0;
			pSectionProperty->adProp[CSectionProperty::Iy] = dH * dB * dB * dB / 12.0;
			pSectionProperty->adProp[CSectionProperty::Cx] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cxp] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cy] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cyp] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Qxb] = ((dH / 2.0)*dB*(dH / 2.0) / 2.) / dB;
			pSectionProperty->adProp[CSectionProperty::Qyb] = ((dB / 2.0)*dH*(dB / 2.0) / 2.) / dH;
			pSectionProperty->adProp[CSectionProperty::x1] = -dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y1] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x2] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y2] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x3] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y3] = -dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x4] = -dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y4] = -dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cw] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Sx] = dB * dH * dH * dH / 6.0;
			pSectionProperty->adProp[CSectionProperty::Sy] = dH * dB * dB * dB / 6.0;
			pSectionProperty->adProp[CSectionProperty::Zx] = dB * dH * dH * dH / 6.0;
			pSectionProperty->adProp[CSectionProperty::Zy] = dH * dB * dB * dB / 6.0;
			pSectionProperty->adProp[CSectionProperty::Xbar] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Ybar] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Rox] = sqrt(pSectionProperty->adProp[CSectionProperty::Ix] / pSectionProperty->adProp[CSectionProperty::Area]);
			pSectionProperty->adProp[CSectionProperty::Roy] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Rop] = 0.0;

			CBindingData PropData;
			PropData.pData = pSectionProperty;

			CRelationalQuery PropQuery;
			PropQuery.DataType = DataType::Elem;
			for (int i = 0; i < aElemList.GetSize(); ++i)
			{
				const T_ELEM_K ElemK = aElemList[i];
				if (L_IsElemWallType(ElemK)) { continue; }
				PropQuery.vOnlyOneKey.push_back(ElemK);
			}

			PropData.vQueryInfo.push_back(PropQuery);
			vBindingData.push_back(PropData);
		}
	}
	else
	{
		for (const auto SbdoK : vSbdo)
		{
			T_SBDO_D SbdoD;
			if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) continue;
			if (SbdoD.nType != 1 && SbdoD.nType != 2) continue;

			if (!SbdoD.bUseMt && SbdoD.dThickness > 0.0)
			{
				//이 두께로 강제 집행
			}
			else
			{
				continue;
			}

			T_ELEM_K_LIST aSbdoElem;
			int nSbdoElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
			if (nSbdoElem == 0) continue;

			const T_ELEM_K ElemK = aSbdoElem[0];
			const double dH = [&pDoc, &SbdoD](const T_ELEM_K& ElemK)
			{
				double dThickness = 0.0;
				if (!SbdoD.bUseMt && SbdoD.dThickness > 0.0)
				{
					dThickness = SbdoD.dThickness;
				}
				else
				{
					ASSERT(0);
				}
				return dThickness;
			}(ElemK);

			auto* pSectionProperty = new CSectionProperty;
			pSectionProperty->adProp[CSectionProperty::Area] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::Asx] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::Asy] = dB * dH;
			pSectionProperty->adProp[CSectionProperty::J] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Ix] = dB * dH * dH * dH / 12.0;
			pSectionProperty->adProp[CSectionProperty::Iy] = dH * dB * dB * dB / 12.0;
			pSectionProperty->adProp[CSectionProperty::Cx] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cxp] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cy] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cyp] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Qxb] = ((dH / 2.0)*dB*(dH / 2.0) / 2.) / dB;
			pSectionProperty->adProp[CSectionProperty::Qyb] = ((dB / 2.0)*dH*(dB / 2.0) / 2.) / dH;
			pSectionProperty->adProp[CSectionProperty::x1] = -dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y1] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x2] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y2] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x3] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y3] = -dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::x4] = -dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::y4] = -dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Cw] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Sx] = dB * dH * dH * dH / 6.0;
			pSectionProperty->adProp[CSectionProperty::Sy] = dH * dB * dB * dB / 6.0;
			pSectionProperty->adProp[CSectionProperty::Zx] = dB * dH * dH * dH / 6.0;
			pSectionProperty->adProp[CSectionProperty::Zy] = dH * dB * dB * dB / 6.0;
			pSectionProperty->adProp[CSectionProperty::Xbar] = dB / 2.0;
			pSectionProperty->adProp[CSectionProperty::Ybar] = dH / 2.0;
			pSectionProperty->adProp[CSectionProperty::Rox] = sqrt(pSectionProperty->adProp[CSectionProperty::Ix] / pSectionProperty->adProp[CSectionProperty::Area]);
			pSectionProperty->adProp[CSectionProperty::Roy] = 0.0;
			pSectionProperty->adProp[CSectionProperty::Rop] = 0.0;

			CBindingData PropData;
			PropData.pData = pSectionProperty;

			CRelationalQuery PropQuery;
			PropQuery.DataType = DataType::Elem;
			for (int i = 0; i < aSbdoElem.GetSize(); ++i)
			{
				PropQuery.vOnlyOneKey.push_back(aSbdoElem[i]);
			}

			PropData.vQueryInfo.push_back(PropQuery);
			vBindingData.push_back(PropData);
		}
	}

	std::vector<DWORD_PTR> vIter;
	if (!SetDataPool(vBindingData, vIter))
	{
		ASSERT(0); return false;
	}

	if (bDesign)
	{
		// set Revision data
		// Section for design
		if (AddMembQueryFromElemQuery(vBindingData))
		{
			if (!AddRevisionData(DataType::SectionProperty, vBindingData, vIter))
			{
				ASSERT(0); return false;
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddMatl(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	CMembTypeUtil Util;
	CArray<UINT, UINT> aSTLProp;
	CArray<UINT, UINT> aRCProp;
	CArray<UINT, UINT> aSRCProp;
	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		const auto ElemK = aElemK[i];
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0); continue;
		}

		if (Util.IsSTL(ElemK))
		{
			aSTLProp.Add(ElemD.elmat);
		}
		else if (Util.IsRC(ElemK))
		{
			aRCProp.Add(ElemD.elmat);
		}
		else if (Util.IsSRC(ElemK))
		{
			aSTLProp.Add(ElemD.elmat);
			aRCProp.Add(ElemD.elmat);
		}
		else
		{
			/*ASSERT(0);*/
		}
	}

	CArrayUtil::RemoveDup(aSTLProp);
	CArrayUtil::RemoveDup(aRCProp);

	CArray<CString, CString&> arStlMatlCode;
	CString strMatlType = _T("S");
	pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arStlMatlCode);
	
	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < aSTLProp.GetSize(); ++i)
	{
		T_MATL_D MatlD;
		if (!pDoc->m_pAttrCtrl->GetMatl(aSTLProp[i], MatlD))
		{
			ASSERT(0); continue;
		}

		auto* pMaterial = new CMaterial;

		CBindingData Data;
		Data.pData = pMaterial;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		T_KEY_LIST aKeyList, aKeyElem;
		aKeyList.Add(aSTLProp[i]);
		pDoc->m_pAttrCtrl->GetKeyListMatlUser(&aKeyList, nullptr, &aKeyElem);
		for (int j = 0; j < aKeyElem.GetSize(); ++j)
		{
			Query.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aKeyElem[0], MatdD))
		{
			continue; // User
		}

		pMaterial->strCode = CT2W(MatdD.Data1.CodeName);
		pMaterial->strName = CT2W(MatdD.Data1.CodeMatlName);

		auto* pMaterialSteel = new CMaterialSteel;
		pMaterialSteel->Data.Common.dElastic = MatdD.Data1.Analysis.Elast;
		pMaterialSteel->Data.Common.dShear = MatdD.Data1.Analysis.ShearModulus;
		pMaterialSteel->Data.Common.dPoisson = MatdD.Data1.Analysis.Poisson;
		pMaterialSteel->Data.Common.dWeightDensity = MatdD.Data1.Analysis.Density;
		pMaterialSteel->Data.Steel.bUser = [&arStlMatlCode](const CString& strCode)
		{
			if(strCode.IsEmpty()) return true;
			const int nStlMatlCode = arStlMatlCode.GetSize();
			for(int j=0; j<nStlMatlCode; ++j)
			{
				if(arStlMatlCode[j] == strCode) return false;
			}
			return true;
		}(MatdD.Data1.CodeName);
		if ( pMaterialSteel->Data.Steel.bUser )
		{
			TMaterialSteelSingle MatlVal;
			MatlVal.dYield    = MatdD.Data1.Design.S_Fy1;
			MatlVal.dUltimate = MatdD.Data1.Design.S_Fu;
			SteelThickProp UserProp = std::make_pair(1.0, MatlVal); /// user일 때는 두께가 의미가 없지.

			pMaterialSteel->Data.Steel.nList = 1;
			pMaterialSteel->Data.Steel.aProp[0] = UserProp;
		}

		CBindingData SteelData;
		SteelData.pData = pMaterialSteel;

		CRelationalQuery SteelQuery;
		SteelQuery.DataType = DataType::Elem;
		for (int j = 0; j < aKeyElem.GetSize(); ++j)
		{
			SteelQuery.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		SteelData.vQueryInfo.push_back(SteelQuery);
		vBindingData.push_back(SteelData);
	}


	int nRCCode = 0;
	T_DCON_D DConD;
	if ( CDgnDataCtrl().Get_DgnConDcon(DConD) )
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}
	
	bool bUserModularRatio = [nRCCode] ()
	{
		switch ( nRCCode )
		{
		case Eurocode2_2_05:
		case BS5400_4_90:
		case TMH07_89:
			return true;
		default:
			break;
		}
		return false;
	}();

	for (int i = 0; i < aRCProp.GetSize(); ++i)
	{
		T_MATL_D MatlD;
		if (!pDoc->m_pAttrCtrl->GetMatl(aRCProp[i], MatlD))
		{
			ASSERT(0); continue;
		}

		auto* pMaterial = new CMaterial;
		CBindingData Data;
		Data.pData = pMaterial;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		T_KEY_LIST aKeyList, aKeyElem;
		aKeyList.Add(aRCProp[i]);
		pDoc->m_pAttrCtrl->GetKeyListMatlUser(&aKeyList, nullptr, &aKeyElem);
		for (int j = 0; j < aKeyElem.GetSize(); ++j)
		{
			Query.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aKeyElem[0], MatdD))
		{
			continue; // User
		}

		pMaterial->strCode = CT2W(MatdD.Data1.CodeName);
		pMaterial->strName = CT2W(MatdD.Data1.CodeMatlName);

		auto* pMaterialConcrete = new CMaterialConcrete;
		pMaterialConcrete->Data.strName = pMaterial->strName;
		pMaterialConcrete->Data.Common.dElastic = MatdD.Data1.Analysis.Elast;
		pMaterialConcrete->Data.Common.dShear = MatdD.Data1.Analysis.ShearModulus;
		pMaterialConcrete->Data.Common.dPoisson = MatdD.Data1.Analysis.Poisson;
		pMaterialConcrete->Data.Common.dWeightDensity = MatdD.Data1.Analysis.Density;
		pMaterialConcrete->Data.Concrete.dLambda = MatdD.Data1.Design.dLambda;
		pMaterialConcrete->Data.Concrete.dStrength = MatdD.Data1.Design.C_fc;
		pMaterialConcrete->Data.Concrete.dAggregateSize = DConD.dAggregateSize;

		pMaterialConcrete->Data.Rebar.dYield = MatdD.MainRebarData.B_fy;
		pMaterialConcrete->Data.Rebar.dUltimate = MatdD.MainRebarData.B_fy;

		CBindingData ConcData;
		ConcData.pData = pMaterialConcrete;
		ConcData.vQueryInfo.push_back(Query);
		vBindingData.push_back(ConcData);

		if ( bUserModularRatio )
		{
			auto* pModularRatio = new CModularRatio;
			pModularRatio->aModularRatio[CModularRatio::EsEc_Shorterm].v2 = MatdD.dShortTerm;
			pModularRatio->aModularRatio[CModularRatio::EsEc_Shorterm].v1 = EQ0(MatdD.dShortTerm) ? true : false;
			pModularRatio->aModularRatio[CModularRatio::EsEc_Longterm].v2 = MatdD.dLongTerm;
			pModularRatio->aModularRatio[CModularRatio::EsEc_Longterm].v1 = EQ0(MatdD.dLongTerm) ? true : false;

			CBindingData ModularRatioData;
			ModularRatioData.pData = pModularRatio;
			ModularRatioData.vQueryInfo.push_back(Query);

			vBindingData.push_back(ModularRatioData);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddPosi(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	// 부재마다 check position을 가져오는 db가 다르다
	CMembTypeUtil Util;

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nAll = aElemK.GetSize();

	std::vector<CBindingData> vBindingData;
	std::vector<T_MEMB_K> vHaunch; vHaunch.clear();
	for (int i = 0; i < nAll; ++i)
	{
		T_MEMB_K MembK = aElemK[i];
		T_HCBM_K HcbmK(0);
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(MembK, HcbmK))
		{
			vHaunch.push_back(HcbmK);
			continue;
		}
		T_ELEM_K Showkey = 0;
		T_ELEM_K_LIST aMembElemList;
		BOOL bReverseDir = FALSE;
		if (!CMembCtrl::GetMembCtrl()->GetElemListByIncludeElem(MembK, bReverseDir, Showkey, aMembElemList))
		{
			continue;
		}

		//요건 부재별로 따로 처리해야됨.
		auto* p1DPosition = new CDgn1DPosition;
		p1DPosition->MembK = MembK;
		int nMembType = pDoc->m_pAttrCtrl->GetMemberType(MembK);

		auto bMakeChkPos = [&]()
		{
			T_DFBA_D DfbaD;
			if (pDoc->m_pAttrCtrl2->GetDfba(MembK, DfbaD))
			{
				return DfbaD.nForceType == EN_DFBA_BY_ELEM_FORCE ? true : false;
			}
			return DConD.bConsiderBeamElemForce ? true : false;
		}();
		
		double dst = 0.0;
		double ded = 1.0;
		double dstep = 0.25;
		if(nMembType != D_MBTP_WALL)
		{
			double dMembLen = CMembCtrl::GetMembCtrl()->GetMembLengByIncludeElem(MembK);
			double dOffsetStaLen = 0.0;
			double dOffsetEndLen = 0.0;
			int nIncElem = aMembElemList.GetSize();
			for(int i = 0; i < nIncElem; ++i)
			{
				const double dLength = pDoc->calcLAVElem(aMembElemList[i]);

				T_CFMO_D CfmoD;
				if(pDoc->m_pPostCtrl->GetCfmo(aMembElemList[i], CfmoD))
				{
					if(CfmoD.nOffsetType == 1)	// Local Offset.
					{
						double dOffset = 0.0;
						if(bReverseDir)
							dOffset = max(CfmoD.dOffset[3], CfmoD.dOffset[5]);	// Member-I.
						else
							dOffset = max(CfmoD.dOffset[2], CfmoD.dOffset[4]);	// Member-I.

						if(EQ(dLength, dOffset)) // 부재력은 offset고려되서 계산되지만 구조체에 들어오는 값은 offset이 되건 안되건 동일한 형태로 들어오기 때문에 전체 요소가 offset에 포함되는 경우만 고려해줌.
							dOffsetStaLen += dLength; // 수치오차 때문에 그냥 요소길이를 더함.

						if(bReverseDir)
							dOffset = max(CfmoD.dOffset[2], CfmoD.dOffset[4]);	// Member-J.
						else
							dOffset = max(CfmoD.dOffset[3], CfmoD.dOffset[5]);	// Member-J.

						if(EQ(dLength, dOffset))
							dOffsetEndLen += dLength;
					}
				}
			}

			if(UQ0(dOffsetStaLen + dOffsetEndLen) && dMembLen > dOffsetStaLen + dOffsetEndLen)
			{
				dst = dOffsetStaLen / dMembLen;
				ded = 1.0 - dOffsetEndLen / dMembLen;
				dstep = (ded - dst) / 4.0;
			}
		}

		const auto DgnType = CDgnTypeConverter::GetDgnType(MembK);
		switch ( DgnType )
		{
		case enDgn::RCS:
		case enDgn::None:
			{
				switch ( nMembType )
				{
				case D_MBTP_ETC:
				case D_MBTP_SHELL:
				case D_MBTP_PLATEBEAM:
				case D_MBTP_PLATECOLM:
					{
						_SAFE_DELETE(p1DPosition);
					}
					continue;
				case D_MBTP_BEAM:
					{
						p1DPosition->aSectorRange.SetSize(3);
						SectorRange Info;
						Info.Sector = enSector::I;
						Info.dRangeMinRatio = dst;
						Info.dRangeMaxRatio = dst+dstep;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::M;
						Info.dRangeMinRatio = dst+dstep;
						Info.dRangeMaxRatio = ded-dstep;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::J;
						Info.dRangeMinRatio = ded-dstep;
						Info.dRangeMaxRatio = ded;
						p1DPosition->aSectorRange.PushBack(Info);

						int nIncElem = aMembElemList.GetSize();
						if ( nIncElem == 1 )
						{
							p1DPosition->aChkRatio.SetSize(5);
							p1DPosition->aChkRatio.PushBack(dst);
							p1DPosition->aChkRatio.PushBack(dst+dstep);
							p1DPosition->aChkRatio.PushBack(dst+dstep*2.0);
							p1DPosition->aChkRatio.PushBack(ded-dstep);
							p1DPosition->aChkRatio.PushBack(ded);
						}
						else if ( bMakeChkPos )
						{
							FArrayd aChkRatio;
							aChkRatio.RemoveAll();

							double dTotalLen = 0.0;
							std::vector<double> vLen;
							for ( int i = 0; i < nIncElem; ++i )
							{
								const double dLength = pDoc->calcLAVElem(aMembElemList.GetAt(i));
								vLen.push_back(dLength);
								dTotalLen += dLength;
							}

							double dLenSum = 0.0;
							for ( int i = 0; i < nIncElem; ++i )
							{
								const double dLength = vLen[i];
								const double dStep = dLength * 0.25;
								for ( int j = 0; j < 5; ++j )
								{
									if ( j == 4 )
									{
										if ( i < nIncElem - 1 ) continue;
									}

									const double dCurPos = dLenSum + dStep * static_cast<double>(j);
									const double dPos = SafeDiv(dCurPos, dTotalLen);
									aChkRatio.Add(dPos);
								}
								dLenSum += dLength;
							}

							aChkRatio.Add(dst);
							aChkRatio.Add(dst + dstep);
							aChkRatio.Add(dst + dstep * 2.0);
							aChkRatio.Add(ded - dstep);
							aChkRatio.Add(ded);

							Macro::FArrayUnique(aChkRatio);
							const int nPos = aChkRatio.GetSize();

							FArrayd aChkPos;
							aChkPos.RemoveAll();
							for(int i=0; i<nPos; ++i)
							{
								if(LT(aChkRatio[i], dst)) continue;
								if(MT(aChkRatio[i], ded)) continue;
								aChkPos.Add(aChkRatio[i]);
							}
							p1DPosition->aChkRatio.Copy(aChkPos.GetData(), aChkPos.GetSize());
						}
						else
						{
							FArrayd aChkRatio;
							aChkRatio.RemoveAll();
							aChkRatio.Add(dst);
							aChkRatio.Add(dst + dstep);
							aChkRatio.Add(dst + dstep * 2.0);
							aChkRatio.Add(ded - dstep);
							aChkRatio.Add(ded);

							double dTotalLen = 0.0;
							std::vector<double> vLen;
							for ( int i = 0; i < nIncElem; ++i )
							{
								const double dLength = pDoc->calcLAVElem(aMembElemList.GetAt(i));
								vLen.push_back(dLength);
								dTotalLen += dLength;
							}

							double dLenSum = 0.0;
							for ( int i = 0; i < nIncElem - 1; ++i )
							{
								const double dLength = vLen[i];
								const double dCurPos = dLenSum + dLength;
								const double dPos = SafeDiv(dCurPos, dTotalLen);
								dLenSum += dLength;
								if(LE(dPos, dst)) continue;
								if(ME(dPos, ded)) continue;
								aChkRatio.Add(dPos);								
							}

							Macro::FArrayUnique(aChkRatio);
							int nPos = aChkRatio.GetSize();
							
							FArrayd aChkPos;
							aChkPos.RemoveAll();
							for(int i = 0; i < nPos; ++i)
							{
								if(LT(aChkRatio[i], dst)) continue;
								if(MT(aChkRatio[i], ded)) continue;
								aChkPos.Add(aChkRatio[i]);
							}

							p1DPosition->aChkRatio.Copy(aChkPos.GetData(), aChkPos.GetSize());
						}
					}
					break;
				case D_MBTP_COLUMN:
					{
#if defined (_CIVIL)
						p1DPosition->aChkRatio.SetSize(2);
						p1DPosition->aChkRatio.PushBack(dst);
						p1DPosition->aChkRatio.PushBack(ded);

						p1DPosition->aSectorRange.SetSize(1);
						SectorRange Info;
						Info.Sector = enSector::End;
						Info.dRangeMinRatio = dst;
						Info.dRangeMaxRatio = ded;
						p1DPosition->aSectorRange.PushBack(Info);
#else
						int nIncElem = aMembElemList.GetSize();
						if ( nIncElem > 1 )
						{
							p1DPosition->aChkRatio.SetSize(5);
							p1DPosition->aChkRatio.PushBack(dst);
							p1DPosition->aChkRatio.PushBack(dst + dstep);
							p1DPosition->aChkRatio.PushBack(dst + dstep * 2.0);
							p1DPosition->aChkRatio.PushBack(ded - dstep);
							p1DPosition->aChkRatio.PushBack(ded);
						}
						else
						{
							p1DPosition->aChkRatio.SetSize(3);
							p1DPosition->aChkRatio.PushBack(dst);
							p1DPosition->aChkRatio.PushBack((dst+ded)/2.0);
							p1DPosition->aChkRatio.PushBack(ded);
						}

						p1DPosition->aSectorRange.SetSize(3);
						SectorRange Info;
						Info.Sector = enSector::End;
						Info.dRangeMinRatio = dst;
						Info.dRangeMaxRatio = dst+dstep;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::Mid;
						Info.dRangeMinRatio = dst+dstep;
						Info.dRangeMaxRatio = ded-dstep;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::End;
						Info.dRangeMinRatio = ded-dstep;
						Info.dRangeMaxRatio = ded;
						p1DPosition->aSectorRange.PushBack(Info);
#endif
					}
					break;
				case D_MBTP_BRACE:
					{
						if ( pDoc->m_pAttrCtrl->ExistMembAssigned(MembK) )
						{
							p1DPosition->aChkRatio.SetSize(3);
							p1DPosition->aChkRatio.PushBack(dst);
							p1DPosition->aChkRatio.PushBack((dst + ded) / 2.0);
							p1DPosition->aChkRatio.PushBack(ded);

							p1DPosition->aSectorRange.SetSize(3);
							SectorRange Info;
							Info.Sector = enSector::End;
							Info.dRangeMinRatio = dst;
							Info.dRangeMaxRatio = dst + dstep;
							p1DPosition->aSectorRange.PushBack(Info);
							Info.Sector = enSector::Mid;
							Info.dRangeMinRatio = dst + dstep;
							Info.dRangeMaxRatio = ded - dstep;
							p1DPosition->aSectorRange.PushBack(Info);
							Info.Sector = enSector::End;
							Info.dRangeMinRatio = ded - dstep;
							Info.dRangeMaxRatio = ded;
							p1DPosition->aSectorRange.PushBack(Info);
						}
						else
						{
							p1DPosition->aChkRatio.SetSize(2);
							p1DPosition->aChkRatio.PushBack(dst);
							p1DPosition->aChkRatio.PushBack(ded);

							p1DPosition->aSectorRange.SetSize(2);
							SectorRange Info;
							Info.Sector = enSector::End;
							Info.dRangeMinRatio = dst;
							Info.dRangeMaxRatio = (dst+ded) / 2.0;
							p1DPosition->aSectorRange.PushBack(Info);
							Info.Sector = enSector::End;
							Info.dRangeMinRatio = (dst+ded) / 2.0;
							Info.dRangeMaxRatio = ded;
							p1DPosition->aSectorRange.PushBack(Info);
						}
					}
					break;
				case D_MBTP_WALL:
					{
						p1DPosition->aChkRatio.SetSize(2);
						p1DPosition->aChkRatio.PushBack(0.0);
						p1DPosition->aChkRatio.PushBack(1.0);

						p1DPosition->aSectorRange.SetSize(2);
						SectorRange Info;
						Info.Sector = enSector::I;
						Info.dRangeMinRatio = 0.0;
						Info.dRangeMaxRatio = 0.5;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::J;
						Info.dRangeMinRatio = 0.5;
						Info.dRangeMaxRatio = 1.0;
						p1DPosition->aSectorRange.PushBack(Info);
					}
					break;
				default:
					{
						ASSERT(0);
						p1DPosition->aChkRatio.SetSize(2);
						p1DPosition->aChkRatio.PushBack(0.0);
						p1DPosition->aChkRatio.PushBack(1.0);

						p1DPosition->aSectorRange.SetSize(2);
						SectorRange Info;
						Info.Sector = enSector::I;
						Info.dRangeMinRatio = 0.0;
						Info.dRangeMaxRatio = 0.5;
						p1DPosition->aSectorRange.PushBack(Info);
						Info.Sector = enSector::J;
						Info.dRangeMinRatio = 0.5;
						Info.dRangeMaxRatio = 1.0;
						p1DPosition->aSectorRange.PushBack(Info);
					}
					break;
				}
			}
			break;
		case enDgn::STL:
			{
				p1DPosition->aSectorRange.SetSize(3);
				SectorRange Info;
				Info.Sector = enSector::I;
				Info.dRangeMinRatio = dst;
				Info.dRangeMaxRatio = dst+dstep;
				p1DPosition->aSectorRange.PushBack(Info);
				Info.Sector = enSector::M;
				Info.dRangeMinRatio = dst+dstep;
				Info.dRangeMaxRatio = ded-dstep;
				p1DPosition->aSectorRange.PushBack(Info);
				Info.Sector = enSector::J;
				Info.dRangeMinRatio = ded-dstep;
				Info.dRangeMaxRatio = ded;
				p1DPosition->aSectorRange.PushBack(Info);

				int nIncElem = aMembElemList.GetSize();
				T_DSTL_D DstlD;
				int nStlCode = 0;
				if ( CDgnDataCtrl().Get_DgnStlDstl(DstlD) )
				{
					nStlCode = CDgnCodeCtrl::GetStlCodeID(DstlD.DesignCode);
				}

				auto bAASHTO17 = [&] () -> bool
				{
					switch ( nStlCode )
					{
					case AASHTO_LS17:
					case AASHTO_LS20://상세 계산서 출력때문에 Check_Section을 사용한다.
					case AASHTO_LS24:
						return true;
					default:
						break;
					}
					return false;
				}();

				if ( nIncElem == 1 || bAASHTO17 == true )
				{
					p1DPosition->aChkRatio.SetSize(5);
					p1DPosition->aChkRatio.PushBack(dst);
					p1DPosition->aChkRatio.PushBack(dst+dstep);
					p1DPosition->aChkRatio.PushBack(dst+dstep*2.0);
					p1DPosition->aChkRatio.PushBack(ded-dstep);
					p1DPosition->aChkRatio.PushBack(ded);
				}
				else
				{
					FArrayd aChkRatio;
					aChkRatio.RemoveAll();
					aChkRatio.Add(dst);
					aChkRatio.Add(dst + dstep);
					aChkRatio.Add(dst + dstep * 2.0);
					aChkRatio.Add(ded - dstep);
					aChkRatio.Add(ded);

					double dTotalLen = 0.0;
					std::vector<double> vLen;
					for(int i = 0; i < nIncElem; ++i)
					{
						const double dLength = pDoc->calcLAVElem(aMembElemList.GetAt(i));
						vLen.push_back(dLength);
						dTotalLen += dLength;
					}

					double dLenSum = 0.0;
					for(int i = 0; i < nIncElem - 1; ++i)
					{
						const double dLength = vLen[i];
						const double dCurPos = dLenSum + dLength;
						const double dPos = SafeDiv(dCurPos, dTotalLen);
						dLenSum += dLength;
						if(LT(dPos, dst)) continue;
						if(MT(dPos, ded)) continue;
						aChkRatio.Add(dPos);
					}

					Macro::FArrayUnique(aChkRatio);
					int nPos = aChkRatio.GetSize();
					p1DPosition->aChkRatio.SetSize(nPos);
					for(int i = 0; i < nPos; ++i)
					{
						if(LT(aChkRatio[i], dst)) continue;
						if(MT(aChkRatio[i], ded)) continue;
						p1DPosition->aChkRatio.PushBack(aChkRatio[i]);
					}
				}
			}
			break;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = p1DPosition;

		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(p1DPosition->MembK);
		Data.vQueryInfo.push_back(RQuery);

		vBindingData.push_back(Data);
	}

	Macro::UniqueSort(vHaunch);
	for (const auto HcbmK : vHaunch)
	{
		T_HCBM_D HcbmD;
		if (!pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD)) continue;
		if (HcbmD.aElemListPart[0].GetSize() == 0 ||
			HcbmD.aElemListPart[1].GetSize() == 0 ||
			HcbmD.aElemListPart[2].GetSize() == 0) continue;

		for (int iPart = 0; iPart < 3; ++iPart) // Part A, B, C
		{
			T_MEMB_K PartMembK = HcbmD.aElemListPart[iPart].GetAt(0);
			auto* p1DPosition = new CDgn1DPosition;
			p1DPosition->MembK = PartMembK;

			const int nIncElem = HcbmD.aElemListPart[iPart].GetSize();

			double dLen = 0.0, dOffsetStaLen = 0.0, dOffsetEndLen = 0.0;
			std::vector<double> vLen;
			for (int i = 0; i < nIncElem; ++i)
			{
				const auto ElemK = HcbmD.aElemListPart[iPart].GetAt(i);
				const double dLength = pDoc->calcLAVElem(ElemK);
				vLen.push_back(dLength);
				dLen += dLength;

				T_CFMO_D CfmoD;
				if(pDoc->m_pPostCtrl->GetCfmo(ElemK, CfmoD))
				{
					if(CfmoD.nOffsetType == 1)	// Local Offset.
					{
						double doffset = max(CfmoD.dOffset[2], CfmoD.dOffset[4]);	// Member-I.
						if(EQ(doffset, dLength)) 
							dOffsetStaLen += dLength;

						doffset = max(CfmoD.dOffset[3], CfmoD.dOffset[5]);	// Member-J.
						if(EQ(doffset, dLength))
							dOffsetEndLen += dLength;
					}
				}
			}

			double dst = 0.0;
			double ded = 1.0;
			double dstep = 0.25;
			if(UQ0(dOffsetStaLen + dOffsetEndLen) && dLen > dOffsetStaLen + dOffsetEndLen)
			{
				dst = dOffsetStaLen / dLen;
				ded = 1.0 - dOffsetEndLen / dLen;
				dstep = (ded - dst) / 4.0;
			}

			p1DPosition->aChkRatio.SetSize(4 * nIncElem + 1 + 3);
			double dLenSum = 0.0;
			for (int i = 0; i < nIncElem; ++i)
			{
				const double dLength = vLen[i];
				for (int j = 0; j < 5; ++j)
				{
					if (j == 4)
					{
						if (i < nIncElem - 1) continue;
					}

					const double dCurPos = dLenSum + dLength * static_cast<double>(j) / 4.0;
					const double dPos = SafeDiv(dCurPos, dLen);
					p1DPosition->aChkRatio.PushBack(dPos);
				}
				dLenSum += dLength;
			}

			const double dCen = [&HcbmD, &iPart, &dLen, &dst, &ded]()
			{
				if (HcbmD.nDesignPosition == 1 && iPart != 1)
				{
					const double dLpart = iPart == 0 ? dst+HcbmD.dL1 : ded - HcbmD.dL2;
					const double dCen = fabs(dLpart) / dLen;
					if (EQ0(dCen) || EQ(dCen, ded)) return (dst+ded)/2.0;
					else                            return min(dCen, ded);
				}
				return (dst+ded)/2.0;
			}();

			p1DPosition->aChkRatio.PushBack((dst+dCen) / 2.0);
			p1DPosition->aChkRatio.PushBack(dCen);
			p1DPosition->aChkRatio.PushBack((dCen + ded) / 2.0);

			Macro::FTArrayUnique(p1DPosition->aChkRatio);

			p1DPosition->aSectorRange.SetSize(3);
			SectorRange Info;
			Info.Sector = enSector::I;
			Info.dRangeMinRatio = dst;
			Info.dRangeMaxRatio = (dst+dCen) / 2.0;
			p1DPosition->aSectorRange.PushBack(Info);
			Info.Sector = enSector::M;
			Info.dRangeMinRatio = (dst+dCen) / 2.0;
			Info.dRangeMaxRatio = (dCen+ded) / 2.0;
			p1DPosition->aSectorRange.PushBack(Info);
			Info.Sector = enSector::J;
			Info.dRangeMinRatio = (dCen+ded) / 2.0;
			Info.dRangeMaxRatio = ded;
			p1DPosition->aSectorRange.PushBack(Info);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, PartMembK);
			Data.pData = p1DPosition;

			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(p1DPosition->MembK);
			Data.vQueryInfo.push_back(RQuery);

			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddPosi2D(const std::vector<long>& aKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::unordered_map<UINT, UINT> mElemSbdo;
	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_ELEM_K_LIST aElemK;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElemK);
		for (int i = 0; i < nElem; ++i)
		{
			mElemSbdo[aElemK[i]] = aElemK[0];
		}
	}

	CArray<T_NODE_K, T_NODE_K> aConsNodeList;
	aConsNodeList.RemoveAll();
	pDoc->m_pAttrCtrl->GetConsNodeKeyList(aConsNodeList);
	auto l_ExistCons = [&aConsNodeList](const T_NODE_K& NodeK)
	{
		int nConst = aConsNodeList.GetSize();
		for (int i = 0; i < nConst; i++)
		{
			if (NodeK == aConsNodeList[i]) return true;
		}
		return false;
	};

	// aKeyMemb는 선택된 요소 key가 들어와야함.    
	CMembTypeUtil Util;
	std::unordered_map<UINT, CDgn2DPosition*> mPosi2D;
	for (const auto ElemK : aKeyMemb)
	{
		const auto itrSbdo = mElemSbdo.find(ElemK);
		if (itrSbdo == mElemSbdo.end())
		{
			ASSERT(0);
			continue;
		}
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if (ElemD.elnod[2] == 0) continue;

		auto* p2DPosition = [&mPosi2D](const UINT& SbdoK)
		{
			auto itrPos = mPosi2D.find(SbdoK);
			if (itrPos == mPosi2D.end())
			{
				return new CDgn2DPosition;
			}
			return itrPos->second;
		}(itrSbdo->second);

		p2DPosition->aElemK.Add(ElemK);
		for (int i = 0; i < 4; ++i)
		{
			if (ElemD.elnod[i] == 0) continue;

			const bool bIs2WayNode = [&pDoc, &Util, &l_ExistCons](const UINT& NodeK)
			{
				// 기둥이 연결된 절점
				CArray<T_ELEM_K, T_ELEM_K> aConnectElem;
				pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, aConnectElem);  // 절점에 연결된 element의 key를 가져온다.
				int nConn = aConnectElem.GetSize();
				bool bExistColumn = false;
				for (int j = 0; j < nConn; ++j)
				{
					if ( Util.IsWall(aConnectElem[j]) ) return false;
					if ( Util.IsBeam(aConnectElem[j]) )
					{
						if ( !(Util.IsEffectiveBeam(aConnectElem[j]) || Util.IsColumnStrip(aConnectElem[j])) )
							return false;
					}

					if(!bExistColumn)
					{
						if ( Util.IsColumn(aConnectElem[j]) || Util.IsBrace(aConnectElem[j]) )
						{
							bExistColumn = true;
						}
					}
				}
				if(bExistColumn) return true;

				// 지점
				if (l_ExistCons(NodeK))
				{
					T_CONS_D ConsD;
					if (!pDoc->m_pAttrCtrl->GetConsNode(NodeK, ConsD)) return false;
					if (ConsD.Constraint[2] == '1')                     return true;
					return false;
				}

				// FZ방향 집중하중을 받는 절점
				CArray<T_CNLD_K, T_CNLD_K>  aCnldK;  aCnldK.RemoveAll();
				CArray<T_CNLD_D, T_CNLD_D&> aCnldD;  aCnldD.RemoveAll();
				pDoc->m_pAttrCtrl->GetCnld(NodeK, aCnldK, aCnldD); // 절점하중이 존재하는 경우.
				int nCnld = aCnldD.GetSize();
				for (int j = 0; j < nCnld; ++j)
				{
					if (UQ0(aCnldD[j].Force[2])) // Z방향 재하 하중이 존재하는 절점만 펀칭검토를 하기 위함.
					{
						return true;
					}
				}

				return false;
			}(ElemD.elnod[i]);
			if (bIs2WayNode) p2DPosition->aNodeK.Add(ElemD.elnod[i]);
		}

		mPosi2D[itrSbdo->second] = p2DPosition;
	}

	// cutting line
	CArray<UINT, UINT> aCutlK;
	aCutlK.RemoveAll();
	CServiceHeadOffice::Instance()->ReqService(_ULS(gps), D_GPS_GET_CUTLINE_KEYS, (void*)&aCutlK);
	const int nCutl = aCutlK.GetSize();
	if (nCutl > 0)
	{
		for (const auto iElem : aKeyMemb)
		{
			const auto itrSbdo = mElemSbdo.find(aKeyMemb[0]);
			if (itrSbdo == mElemSbdo.end()) continue;

			auto* p2DPosition = [&mPosi2D](const UINT& SbdoK)
			{
				auto itrPos = mPosi2D.find(SbdoK);
				if (itrPos == mPosi2D.end())
				{
					return new CDgn2DPosition;
				}
				return itrPos->second;
			}(itrSbdo->second);

			p2DPosition->aCutlK.SetSize(nCutl);
			for (int i = 0; i < nCutl; ++i)
				p2DPosition->aCutlK[i] = aCutlK[i];

			break;
		}
	}

	std::vector<CBindingData> vBindingData;
	for (const auto& Posi2D : mPosi2D)
	{
		if (Posi2D.second->aElemK.GetSize() == 0) continue;
		FArrayUniqueSort(Posi2D.second->aElemK);
		FArrayUniqueSort(Posi2D.second->aNodeK);

		CBindingData Data;
		Data.pData = Posi2D.second;

		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Posi2D.first);
		Data.vQueryInfo.push_back(RQuery);

		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLcom(const std::vector<long>& vKeyMemb)
{
	enum EN_DgnType
	{
		Steel = 0,
		RC,
		FND,
		SRC,
		Aluminum,
		StlComp,
		CfsSteel,
		Number,
	};

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_LCTP_D LctpD; LctpD.Initialize();
	bool bLctp = pDoc->m_pAttrCtrl->GetLctp(LctpD);
	auto l_GetLongShortTerm = [&LctpD](const UINT& unType, const UINT& Key)
	{
		int nLong = LctpD.arLongKey.GetSize();
		for (int j = 0; j < nLong; ++j)
		{
			if (LctpD.arLongType[j] == unType && LctpD.arLongKey[j] == Key)
			{
				return CDgnLoadCase::enLongShortTerm::LongTerm;
			}
		}
		return CDgnLoadCase::enLongShortTerm::ShortTerm;
	};

	CMembTypeUtil Util;
	T_ELEM_K_LIST aElemK[7];
	for (int i = 0; i < Number; ++i) aElemK[i].RemoveAll();
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (Util.IsSTL((T_ELEM_K)vKeyMemb[i]))
		{
			aElemK[Steel].Add((T_ELEM_K)vKeyMemb[i]);
		}
		else if (Util.IsRC((T_ELEM_K)vKeyMemb[i]))
		{
			if (Util.IsPlate((T_ELEM_K)vKeyMemb[i]))
				aElemK[RC].Add(CMembCtrl::GetMembCtrl()->GetRepElem((T_ELEM_K)vKeyMemb[i]));
			else
				aElemK[RC].Add((T_ELEM_K)vKeyMemb[i]);
		}
		else
		{
			continue; //아직 지원하고 있지 않아.
		}
	}
	CArrayUtil::RemoveDup(aElemK[RC]);

	int nRCCode = 0, nStlCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}
	T_DSTL_D DstlD;
	if (CDgnDataCtrl().Get_DgnStlDstl(DstlD))
	{
		nStlCode = CDgnCodeCtrl::GetStlCodeID(DstlD.DesignCode);
	}

	std::vector<CBindingData> vBindingData;

	T_KEY_LIST aLoadCaseKey[dgn::def::EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCOM];
	for (int i = Steel; i < Number; ++i)
	{
		int nMemb = aElemK[i].GetSize();
		if (nMemb == 0) continue;

		T_KEY_LIST aKeyList;
		int nLcomType = D_LCOMTYPE_GENERAL;
		dgn::def::enDgn DgnType;
		switch (i)
		{
		case Steel:
			nLcomType = D_LCOMTYPE_STEEL;
			DgnType = enDgn::STL;
			break;
		case RC:
			nLcomType = D_LCOMTYPE_CONCRETE;
			DgnType = enDgn::RCS;
			break;
		case FND:
			nLcomType = D_LCOMTYPE_FDN;
			DgnType = enDgn::RCS;
			break;
		case SRC:
			nLcomType = D_LCOMTYPE_SRC;
			DgnType = enDgn::SRC;
			break;
		case Aluminum:
			nLcomType = D_LCOMTYPE_ALUMINUM;
			DgnType = enDgn::CSG;
			break;
		case StlComp:
			nLcomType = D_LCOMTYPE_STLCOMP;
			DgnType = enDgn::CSG;
			break;
		case CfsSteel:
			nLcomType = D_LCOMTYPE_CFSTEEL;
			DgnType = enDgn::STL;
			break;
		default:
			continue;
		}

		if ( m_pDataBase->GetDgnType() != DgnType )
			continue;

		T_LCOM_K_LIST aSelLcomK;
		aSelLcomK.RemoveAll();
		pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aKeyList);
		for (int j = 0; j < aKeyList.GetSize(); ++j)
		{
			const T_LCOM_K LcomK = aKeyList[j];

			auto* pLcom = new CLcom;
			pLcom->LcomType = DgnType;

			T_LCOM_D LcomD;
			if (!pDoc->m_pAttrCtrl->GetLcom(nLcomType, LcomK, LcomD))
			{
				ASSERT(0); continue;
			}

			// 일단 하중조합내 하중조합 처리 때문에 모든 하중조합을 가져감.
			//if ( LcomD.nActive != 1 && LcomD.nActive != 2 ) continue;

			pLcom->Lcom.LcomInfo.strName = CT2W(LcomD.LoadCombName);
			pLcom->Lcom.LcomInfo.strDesc = CT2W(LcomD.Description);
			pLcom->Lcom.LcomInfo.unDgnType = [&LcomD]()
			{
				//0:inactive, 1:active or strength/stress(CIVIL RC), 2:serviceability(CIVIL RC), 5:strength(Elastic), 6:U.G_Strength, 7:U.G_serviceability, 8:U.G_Special
				switch (LcomD.nActive)
				{
				case EN_DGN_STRN:
				case EN_DGN_SPEC:
				case EN_DGN_VERT:
				case EN_DGN_UG_STRN:
				case EN_DGN_UG_SPEC:
					return CLcomInfo::EN_LCOM_STRN;
				case EN_DGN_SERV:
				case EN_DGN_UG_SERV:
					return CLcomInfo::EN_LCOM_SERV;
				default:
					return CLcomInfo::EN_LCOM_NONE; // 이건 나중에 필요하면 늘리는 걸로~~
				}
			}();
			pLcom->Lcom.LcomInfo.unSubType = [&LcomD]()
			{
				//0:inactive, 1:active or strength/stress(CIVIL RC), 2:serviceability(CIVIL RC), 5:strength(Elastic), 6:U.G_Strength, 7:U.G_serviceability, 8:U.G_Special
				switch (LcomD.nActive)
				{
				case EN_DGN_SERV:
				{
					return [&LcomD]()
					{
						switch (LcomD.nServLcomType)
						{
						case 0:
							return CLcomInfo::EN_LCOM_SUB_NONE;
						case 1:
							return CLcomInfo::EN_LCOM_SUB_QUAS;
						case 2:
							return CLcomInfo::EN_LCOM_SUB_FREQ;
						case 3:
							return CLcomInfo::EN_LCOM_SUB_CHAR;
						default:
							break;
						}
						return CLcomInfo::EN_LCOM_SUB_SERV_GENERAL;
					}();
				}
				break;
				case EN_DGN_STRN:
				case EN_DGN_UG_SERV:
					return CLcomInfo::EN_LCOM_SUB_NONE;
				case EN_DGN_SPEC:
					return CLcomInfo::EN_LCOM_SUB_STRN_SPECIAL;
				case EN_DGN_VERT:
					return CLcomInfo::EN_LCOM_SUB_STRN_VERTICAL;
				case EN_DGN_UG_STRN:
					return CLcomInfo::EN_LCOM_SUB_STRN_UNDERGROUND;
				case EN_DGN_UG_SPEC:
					return CLcomInfo::EN_LCOM_SUB_STRN_UNDERGROUND_SPECIAL;
				default:
					return CLcomInfo::EN_LCOM_SUB_NONE; // 이건 나중에 필요하면 늘리는 걸로~~
				}
			}();
			pLcom->Lcom.LcomInfo.bIncludeSeisLset = CDgnDataCtrl().IsSeismicLcom(nLcomType, LcomK) ? true : false;
			pLcom->Lcom.LcomInfo.bSRSS = LcomD.LoadCombType == 3 ? true : false;
#if defined _CIVIL
			pLcom->Lcom.LcomInfo.bEnv = LcomD.LoadCombType == 1 ? true : false;
			pLcom->Lcom.LcomInfo.bInclueEnv = pDoc->m_pAttrCtrl->IncludeEnvlopeLcom(LcomD);
#else 
			pLcom->Lcom.LcomInfo.bEnv = pDoc->m_pAttrCtrl->IsEnvlopeLcom(LcomD);
			pLcom->Lcom.LcomInfo.bInclueEnv = pDoc->m_pAttrCtrl->IncludeEnvlopeLcom(LcomD);
#endif
			

			int nComb = LcomD.aCombination.GetSize();
			pLcom->Lcom.vInfo.SetSize(nComb);
			bool bAccidentalLset = false;
			for (int k = 0; k < nComb; ++k)
			{
				T_LCOM_BASE LcomBaseD = LcomD.aCombination[k];
				pLcom->Lcom.vInfo[k].unLcasType = [&LcomBaseD]()
				{
					switch (LcomBaseD.AnalType)
					{
					case D_LCOM_STATIC:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC;
					case D_LCOM_SPECTRUM:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESP;
					case D_LCOM_HISTORY:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LHIS;
					case D_LCOM_MOVING:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LMOV;
					case D_LCOM_SETTLEMENT:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSET;
					case D_LCOM_ENVELOPE:
					case D_LCOM_CB_GENERAL:
					case D_LCOM_CB_STEEL:
					case D_LCOM_CB_CONCRETE:
					case D_LCOM_CB_FOOTING:
					case D_LCOM_CB_SRC:
					case D_LCOM_CB_ALUMINUM:
					case D_LCOM_CB_STLCOMP:
					case D_LCOM_CB_CFSTEEL:
					case D_LCOM_CB_SEISMIC:
					case D_LCOM_CB_HISTORY:
					case D_LCOM_CB_LINEAR_EVAL:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCOM;
					case D_LCOM_ECCENSPEC:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESE;
					case D_LCOM_CRANE:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCRN;
					case D_LCOM_STAGE:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTG;
					case D_LCOM_SPECTRUM_ND:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDR;
					case D_LCOM_ECCENSPEC_ND:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDE;
					default:
						return EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC;
					}

				}();
				if (pLcom->Lcom.vInfo[k].unLcasType != EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCOM)
					aLoadCaseKey[pLcom->Lcom.vInfo[k].unLcasType].Add(LcomBaseD.LoadCaseKey);

				pLcom->Lcom.vInfo[k].LoadCaseKey = LcomBaseD.LoadCaseKey; // 제품에서의 key
				pLcom->Lcom.vInfo[k].dFactor = LcomBaseD.Factor;
				pLcom->Lcom.vInfo[k].bLead = false;

				if ( CDgnDataCtrl().IsAccidentalLoad(LcomBaseD.LoadCaseKey) )
				{
					pLcom->Lcom.LcomInfo.bIncludeAccidentalLset = true;
				}
			}

			pLcom->Lcom.LcomInfo.unConcurrentType = CLcomInfo::EN_LCOM_CONCURRENT_NORMAL;
			// Set concurrent type
#if defined(_CIVIL)
			BOOL bConCurrent12 = pDoc->m_pAttrCtrl->IsConCurrentComb12(LcomD);
			if (bConCurrent12)
			{
				pLcom->Lcom.LcomInfo.unConcurrentType = CLcomInfo::EN_LCOM_CONCURRENT_DOFS;
			}
			else if(pLcom->Lcom.LcomInfo.bInclueEnv)
			{
				pLcom->Lcom.LcomInfo.unConcurrentType = CLcomInfo::EN_LCOM_CONCURRENT_DOFS;//EN_LCOM_CONCURRENT_MINMAX;
			}

			if(pLcom->Lcom.LcomInfo.unConcurrentType == CLcomInfo::EN_LCOM_CONCURRENT_DOFS)
			{
				if (m_pDataBase->IsRcsPBeam() || m_pDataBase->IsRcsPWall())
					pLcom->Lcom.LcomInfo.nDofs = 8;
				else if (m_pDataBase->GetDgnType() == enDgn::STL)
				{
					switch (nStlCode)
					{
					case KSCE_ASD05:
					case KSCE_RAIL_ASD04:
					case KSCE_ASD10:
					case KDS_24_14_30_2019:
					case KSCE_RAIL_ASD11:
					case KR_BRG_LSD15:
						pLcom->Lcom.LcomInfo.nDofs = 6;  /// Fx, Fy, Fz, Mx, My, Mz
						break;
					default:
						//ASSERT(0); // 계산에 7자유도 요소를 고려할 때 적용? 지금은 AASHTO 강합성만 Stress에 Bi moment를 고려하고 있음
						pLcom->Lcom.LcomInfo.nDofs = 9;  /// Fx, Fy, Fz, Mx, My, Mz, Mbi, Tst, Twp
						break;
					}
				}
				else if (m_pDataBase->GetDgnType() == enDgn::RCS)
				{
					switch (nRCCode)
					{
					case KSCE_RC_USD05:
					case KSCE_RC_RAIL_USD04:
					case KCI_USD07:
					case KCI_USD07_CVL:
					case KCI_USD12:
					case KDS_41_30_2018:
					case KDS_41_20_2022:
					case KSCE_RC_USD10:
					case KSCE_RC_RAIL_USD11:
					case KR_LRFD11_RC:
					case KDS_24_14_21_2022:
						pLcom->Lcom.LcomInfo.nDofs = 6;  /// Fx, Fy, Fz, Mx, My, Mz
						break;
					default:
						//ASSERT(0);
						pLcom->Lcom.LcomInfo.nDofs = 9;  /// Fx, Fy, Fz, Mx, My, Mz, Mbi, Tst, Twp
						break;
					}
				}
			}
#else
			BOOL bMaxMinLcase = pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
			if (bMaxMinLcase)
				pLcom->Lcom.LcomInfo.unConcurrentType = CLcomInfo::EN_LCOM_CONCURRENT_MINMAX;
			else if (pLcom->Lcom.LcomInfo.bInclueEnv)
			{
				pLcom->Lcom.LcomInfo.unConcurrentType = CLcomInfo::EN_LCOM_CONCURRENT_MINMAX;
			}
#endif

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, LcomK);
			Data.pData = pLcom;

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			for (int k = 0; k < nMemb; ++k)
			{
				if (Util.IsSlab(aElemK[i].GetAt(k)) || Util.IsRCShell(aElemK[i].GetAt(k)))
				{
					const auto iLcom = ((IDgnPerformDataBaseRC*)m_pDataBase)->GetSlabSelectedLcomK();
					if (iLcom == 0)
					{
						int nSelLcom = aSelLcomK.GetSize();
						if (nSelLcom == 0)
						{
							nSelLcom = ((IDgnPerformDataBaseRC*)m_pDataBase)->GetLcomKeyListMeshedMemb(aSelLcomK);
						}
						for (int kk = 0; kk < nSelLcom; ++kk)
						{
							if (aSelLcomK[kk] == LcomK)
							{
								Query.vOnlyOneKey.push_back(aElemK[i].GetAt(k));
							}
						}
					}
					else if (iLcom == LcomK)
					{
						Query.vOnlyOneKey.push_back(aElemK[i].GetAt(k));
					}
				}
				else
				{
					Query.vOnlyOneKey.push_back(aElemK[i].GetAt(k));
				}
			}
			Data.vQueryInfo.push_back(Query);

			//Lcom query
			Query.DataType = DataType::Lcom;
			Query.vOnlyOneKey.clear();
			Query.vOnlyOneKey.push_back(LcomK);
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}
	}

	int nLoadCase = 0;
	//Static Load case
	int nStld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC].GetSize();
	if (nStld > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC], TRUE);
		nStld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC].GetSize();
		for (int j = 0; j < nStld; ++j)
		{
			T_STLD_K StldK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTC][j];
			T_STLD_D StldD;
			if (!pDoc->m_pAttrCtrl->GetStld(StldK, StldD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(StldD.LoadCaseName);
			pLoadCase->iOrgKey = StldK;
			pLoadCase->Type = CDgnTypeConverter::GetLoadCaseType(StldD.LoadCaseType);
			if (bLctp)   pLoadCase->Term = l_GetLongShortTerm(D_LCOM_STATIC, StldK);
			else        pLoadCase->Term = CDBLib::IsStldLTerm(StldD.LoadCaseType) ? CDgnLoadCase::LongTerm : CDgnLoadCase::ShortTerm;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nSplc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESP].GetSize();
	if (nSplc > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESP], TRUE);
		nSplc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESP].GetSize();
		for (int j = 0; j < nSplc; ++j)
		{
			T_SPLC_K SplcK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESP][j];
			T_SPLC_D SplcD;
			if (!pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(SplcD.LoadCaseName);
			pLoadCase->iOrgKey = SplcK;
			pLoadCase->Type = SplcD.nDirection == 1 ? enLoadCase::ESP : enLoadCase::ESPz;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_SPECTRUM, SplcK);
			pLoadCase->bResultSign = SplcD.bResultSign;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nThis = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LHIS].GetSize();
	if (nThis > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LHIS], TRUE);
		nThis = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LHIS].GetSize();
		for (int j = 0; j < nThis; ++j)
		{
			T_THIS_K ThisK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LHIS][j];
			T_THIS_D ThisD;
			if (!pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(ThisD.LoadCaseName);
			pLoadCase->iOrgKey = ThisK;
			pLoadCase->Type = enLoadCase::TH;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_HISTORY, ThisK);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nMvld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LMOV].GetSize();
	if (nMvld > 0)
	{
		T_MVCD_D MvcdD;
		if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LMOV], TRUE);
		nMvld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LMOV].GetSize();
		for (int j = 0; j < nMvld; ++j)
		{
			T_MVLD_K MvldK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LMOV][j];
			auto* pLoadCase = new CDgnLoadCase;
			switch (MvcdD.nCodeType)
			{
			case D_MOVE_CODE_NONE:
				break;
			case D_MOVE_CODE_KOREA:
			case D_MOVE_CODE_AASHTO_STAN:
			case D_MOVE_CODE_AASHTO_LRFD:
			case D_MOVE_CODE_TAIWAN:
			case D_MOVE_CODE_CANADA:
			case D_MOVE_CODE_PENDOT:
			case D_MOVE_CODE_EURO_BS:
			case D_MOVE_CODE_RUSSIA:
			case D_MOVE_CODE_KOREA_LRFD_2011:
			case D_MOVE_CODE_AUSTRALIA:
			case D_MOVE_CODE_POLAND:
			case D_MOVE_CODE_SOUTH_AFRICA:
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
			{
				T_MVLD_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvld(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
				break;
			}
			break;
			case D_MOVE_CODE_JAPAN:
			{
				T_MVLDjp_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldjp(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			case D_MOVE_CODE_CHINA:
			{
				T_MVLDch_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldch(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			case D_MOVE_CODE_INDIA:
			{
				T_MVLDid_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldid(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			case D_MOVE_CODE_BS:
			{
				T_MVLDbs_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldbs(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			case D_MOVE_CODE_FRANCE:
			{
				T_MVLDfr_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldfr(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			case D_MOVE_CODE_TRANS:
			{
				T_MVLDtr_D MvldD;
				if (!pDoc->m_pAttrCtrl->GetMvldtr(MvldK, MvldD))
				{
					ASSERT(0); continue;
				}
				pLoadCase->strName = CT2W(MvldD.LoadCaseName);
			}
			break;
			default:
			{
				ASSERT(0);
			}
			break;
			}

			pLoadCase->iOrgKey = MvldK;
			pLoadCase->Type = enLoadCase::M;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_MOVING, MvldK);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nSmlc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSET].GetSize();
	if (nSmlc > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSET], TRUE);
		nSmlc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSET].GetSize();
		for (int j = 0; j < nSmlc; ++j)
		{
			T_SMLC_K SmlcK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSET][j];
			T_SMLC_D SmlcD;
			if (!pDoc->m_pAttrCtrl->GetSmlc(SmlcK, SmlcD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(SmlcD.LoadCaseName);
			pLoadCase->iOrgKey = SmlcK;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_SETTLEMENT, SmlcK);
			pLoadCase->Term = CDgnLoadCase::ShortTerm;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nEccn = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESE].GetSize();
	if (nEccn > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESE], TRUE);
		nEccn = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESE].GetSize();
		for (int j = 0; j < nEccn; ++j)
		{
			T_SPLC_K SplcK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LESE][j];
			T_SPLC_D SplcD;
			if (!pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(SplcD.LoadCaseName);
			pLoadCase->iOrgKey = SplcK;
			pLoadCase->Type = enLoadCase::ESPE;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_ECCENSPEC, SplcK);
			pLoadCase->bResultSign = SplcD.bResultSign;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nCran = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCRN].GetSize();
	if (nCran > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCRN], TRUE);
		nCran = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCRN].GetSize();
		for (int j = 0; j < nCran; ++j)
		{
			T_CRAN_K CranK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LCRN][j];
			T_CRAN_D CranD;
			if (!pDoc->m_pAttrCtrl->GetCran(CranK, CranD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(CranD.LoadTypeName);
			pLoadCase->iOrgKey = CranK;
			pLoadCase->Type = enLoadCase::CRN;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_CRANE, CranK);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nSgld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTG].GetSize();
	if (nSgld > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTG], TRUE);
		nSgld = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTG].GetSize();
		for (int j = 0; j < nSgld; ++j)
		{
			T_SGLD_K SgldK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LSTG][j];
			T_SGLD_D SgldD;
			if (!pDoc->m_pAttrCtrl->GetSgld(SgldK, SgldD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(SgldD.LoadCaseName);
			pLoadCase->iOrgKey = SgldK;
			if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_DL))	    pLoadCase->Type = enLoadCase::cDL;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_LL))	pLoadCase->Type = enLoadCase::cLL;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_CR))	pLoadCase->Type = enLoadCase::cCR;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_SH))	pLoadCase->Type = enLoadCase::cSH;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_CP))	pLoadCase->Type = enLoadCase::cCR;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_CS))	pLoadCase->Type = enLoadCase::cCR;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_SP))	pLoadCase->Type = enLoadCase::cSH;
			else if (SgldD.LoadCaseName == _LS(IDS_DB_SGLD_SS))	pLoadCase->Type = enLoadCase::cSH;
			else pLoadCase->Type = enLoadCase::CS;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_STAGE, SgldK);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nNspc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDR].GetSize();
	if (nNspc > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDR], TRUE);
		nNspc = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDR].GetSize();
		for (int j = 0; j < nNspc; ++j)
		{
			T_NSPC_K NspcK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDR][j];
			T_NSPC_D NspcD;
			if (!pDoc->m_pAttrCtrl->GetNspc(NspcK, NspcD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(NspcD.LoadCaseName);
			pLoadCase->iOrgKey = NspcK;
			pLoadCase->Type = NspcD.nDirection == 1 ? enLoadCase::NESP : enLoadCase::NESPz;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_SPECTRUM, NspcK);
			pLoadCase->bResultSign = NspcD.bResultSign;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	int nNecn = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDE].GetSize();
	if ( nNecn > 0)
	{
		CArrayUtil::RemoveDup(aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDE], TRUE);
		nNecn = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDE].GetSize();
		for (int j = 0; j < nNecn; ++j)
		{
			T_NSPC_K NspcK = aLoadCaseKey[EN_DGNLCOM_LTYP::EN_DGNLCOM_ATYP_LNDE][j];
			T_NSPC_D NspcD;
			if (!pDoc->m_pAttrCtrl->GetNspc(NspcK, NspcD))
			{
				ASSERT(0); continue;
			}

			auto* pLoadCase = new CDgnLoadCase;
			pLoadCase->strName = CT2W(NspcD.LoadCaseName);
			pLoadCase->iOrgKey = NspcK;
			pLoadCase->Type = enLoadCase::NESPE;
			pLoadCase->Term = l_GetLongShortTerm(D_LCOM_ECCENSPEC, NspcK);
			pLoadCase->bResultSign = NspcD.bResultSign;

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ++nLoadCase);
			Data.pData = pLoadCase;
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddUnbracedLength(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;

	std::vector<CBindingData> vBindingData;
	std::vector<long> RemainSTLKey;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		bool bUser = true;
		T_LENG_D LengD;
		if (!pDoc->m_pAttrCtrl->GetLeng(vKeyMemb[i], LengD))
		{
			if (Util.IsSTL(vKeyMemb[i]))
			{
				RemainSTLKey.push_back(vKeyMemb[i]);
			}
			continue;
		}

		auto* pUnbracedLength = new CUnbracedLength;
		pUnbracedLength->dLb.v1 = !bUser;
		pUnbracedLength->dLy.v1 = !bUser;
		pUnbracedLength->dLz.v1 = !bUser;

		pUnbracedLength->dLb.v2 = LengD.BLu;
		pUnbracedLength->dLy.v2 = LengD.BLy;
		pUnbracedLength->dLz.v2 = LengD.BLz;

		if (LengD.bNotUseBLu)
		{
			pUnbracedLength->dLb.v1 = false;
			pUnbracedLength->dLb.v2 = 0.0;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(bUser, vKeyMemb[i]);
		Data.pData = pUnbracedLength;

		CRelationalQuery Query;
		Query.DataType = DataType::Memb;
		Query.vOnlyOneKey.push_back(vKeyMemb[i]);
		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	if (RemainSTLKey.size() > 0)
	{
		T_DSTL_D StlD;
		if (!CDgnDataCtrl().Get_DgnStlDstl(StlD))
		{
			ASSERT(0); return false;
		}

		if (StlD.bAllFrameLaterallyBraced)
		{
			for (int i = 0; i < RemainSTLKey.size(); ++i)
			{
				auto* pUnbracedLength = new CUnbracedLength;
				pUnbracedLength->dLy.v1 = true;
				pUnbracedLength->dLy.v2 = 0.0;
				pUnbracedLength->dLz.v1 = true;
				pUnbracedLength->dLz.v2 = 0.0;
				pUnbracedLength->dLb.v1 = false;
				pUnbracedLength->dLb.v2 = 0.0;

				CBindingData Data;
				Data.ForceKey = std::make_pair(true, RemainSTLKey[i]);
				Data.pData = pUnbracedLength;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(RemainSTLKey[i]);
				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddEffectiveLength(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	std::vector<long> vUserInputedMemb;

	CDgnDataCtrl DataCtrl;
	T_DCTL_D DctlD;
	DataCtrl.Get_DgnGenDctl(DctlD);

	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		T_ELEM_D eData;
		eData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], eData))
			continue;

		T_KFAC_D KFacD;
		T_KFAC_D AKFacD;
// 		if (!pDoc->m_pAttrCtrl->GetKfac(vKeyMemb[i], KFacD))
// 		{
// 			continue;
// 		}

		if (!DataCtrl.Get_DgnGenKfac(vKeyMemb[i], KFacD, AKFacD))
		{
			continue;
		}

		CEffectiveLength* pKFac = new CEffectiveLength;
		pKFac->dKy = KFacD.BKy;
		pKFac->dKz = KFacD.BKz;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, vKeyMemb[i]);
		Data.pData = pKFac;

		CRelationalQuery Query;
		Query.DataType = DataType::Memb;
		Query.vOnlyOneKey.push_back(vKeyMemb[i]);
		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);

		vUserInputedMemb.push_back(vKeyMemb[i]);
	}

	std::vector<long> vCompleMemb;
	Macro::GetComplementSet(vKeyMemb, vUserInputedMemb, vCompleMemb);

	if (DctlD.bAutoKFactor == FALSE)
	{
		//GEN은 이변수가 OFF면 1.0 강재입력이래 벽빼고
		auto* pKFac = new CEffectiveLength;
		pKFac->dKy = 1.0;
		pKFac->dKz = 1.0;

		CBindingData Data;
		Data.ForceKey = std::make_pair(false, 0);
		Data.pData = pKFac;
		CRelationalQuery Query;
		Query.DataType = DataType::Memb;
		Query.vOnlyOneKey = vCompleMemb;
		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLimitSlendernessRatio(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		T_LTSR_D LimitSlenRatioD;
		if (!pDoc->m_pAttrCtrl->GetLtsr(vKeyMemb[i], LimitSlenRatioD))
		{
			continue;
		}

		auto* pLimit = new CLimitSlendernessRatio;
		pLimit->aFactor[CLimitSlendernessRatio::Compression].v1 = false;
		pLimit->aFactor[CLimitSlendernessRatio::Compression].v2 = LimitSlenRatioD.CLMT;
		pLimit->aFactor[CLimitSlendernessRatio::Tension].v1  = false;
		pLimit->aFactor[CLimitSlendernessRatio::Tension].v2 = LimitSlenRatioD.TLMT;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, vKeyMemb[i]);
		Data.pData = pLimit;
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBeam(const std::vector<long>& vKeyMemb)
{
	// Get All Member
	std::vector<long> vElemK;
	vElemK.clear();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_ELEM_K_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		vElemK.push_back(aElemK[i]);
	}

	std::vector<CBindingData> vBindingData;
#if defined(_CIVIL)
	if (!AddRebarBeam_Civil(vElemK, vBindingData)) return false;
#else
	if (!AddRebarBeam_Gen(vElemK, vBindingData)) return false;
#endif

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBeam_Gen(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CMembTypeUtil Util;
	std::unordered_map<std::pair<T_REBB_KEY, int>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCBeam(vKeyMemb[i]))
			continue;

		auto RebbK = pDoc->m_pAttrCtrl->GetRebbByElemK(vKeyMemb[i]);

		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RebbK.keymap, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		T_REBB_D RebbD;
		RebbD.Initialize();

		auto _l_convert_rebb = [&](enSector Sector, CMemberRebarBeam* pRebeam, T_REBB_BASE& RebbTop, T_REBB_BASE& RebbBot, double dT, double dB) -> bool
		{
			T_MATD_D MatdD;
			if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
			{
				return false; // User
			}

			T_MATL_REBAR RebarData = MatdD.MainRebarData;
			
			double dClearDist = Get_ClearDistArray(nRCCode, pDoc->m_pMatlDB->Get_RebarDia(RebbTop.RebarName), false);
			double dDisty = dT;
			CRebarBeam Pattern;
			Pattern.aTop.resize(RebbTop.GetArrayType());
			for (int i = 0; i < RebbTop.GetArrayType(); ++i)
			{
				if(i == 0)
				{
					Pattern.aTop[i].nNum = RebbTop.nRebar1;
					Pattern.aTop[i].Rebar.Dia = CreateRebarDia(RebbTop.RebarName, RebarData);
				}
				else
				{
					Pattern.aTop[i].nNum = RebbTop.nRebar2;
					if(!RebbTop.RebarName2nd.IsEmpty())
						Pattern.aTop[i].Rebar.Dia = CreateRebarDia(RebbTop.RebarName2nd, RebarData);
					else
						Pattern.aTop[i].Rebar.Dia = CreateRebarDia(RebbTop.RebarName, RebarData);
				}
				

				if (i != 0) dDisty += (Pattern.aTop[i].Rebar.Dia.Geom.dDia / 2.0);
				Pattern.aTop[i].dCenterCover = dDisty;
				dDisty += (Pattern.aTop[i].Rebar.Dia.Geom.dDia / 2.0 + dClearDist);
			}

			dClearDist = Get_ClearDistArray(nRCCode, pDoc->m_pMatlDB->Get_RebarDia(RebbBot.RebarName), false);
			dDisty = dB;
			Pattern.aBot.resize(RebbBot.GetArrayType());
			for (int i = 0; i < RebbBot.GetArrayType(); ++i)
			{
				if(i == 0)
				{
					Pattern.aBot[i].nNum = RebbBot.nRebar1;
					Pattern.aBot[i].Rebar.Dia = CreateRebarDia(RebbBot.RebarName, RebarData);
				}
				else
				{
					Pattern.aBot[i].nNum = RebbBot.nRebar2;
					if(!RebbBot.RebarName2nd.IsEmpty())
						Pattern.aBot[i].Rebar.Dia = CreateRebarDia(RebbBot.RebarName2nd, RebarData);
					else
						Pattern.aBot[i].Rebar.Dia = CreateRebarDia(RebbBot.RebarName, RebarData);
				}

				if (i != 0) dDisty += (Pattern.aBot[i].Rebar.Dia.Geom.dDia / 2.0);
				Pattern.aBot[i].dCenterCover = dDisty;
				dDisty += (Pattern.aBot[i].Rebar.Dia.Geom.dDia / 2.0 + dClearDist);
			}

			T_MATL_REBAR RebarStirrupData = MatdD.SubRebarData;
			
			Pattern.Shear.nLeg = RebbTop.iSubRebarNum;
			Pattern.Shear.Rebar.dSpace = RebbTop.dSubRebarSpace;
			Pattern.Shear.Rebar.Dia = CreateRebarDia(RebbD.SubRebarName, RebarStirrupData);

			if (!RebbD.strSkinName.IsEmpty())
			{
				Pattern.Skin.Type = TSkinRebarBeam::enBatch::Even;
				Pattern.Skin.Data.Rebar.Dia = CreateRebarDia(RebbD.strSkinName, RebarStirrupData);
				Pattern.Skin.Data.nNum = RebbTop.nSkin;
				Pattern.Skin.Data.dCenterCover = max(dB, dT);
			}

			pRebeam->SetRebar(Sector, Pattern);

			return true;
		};

		if (!m_pDataBase->IsDesigning())
		{
			T_REBB_K RebbK;
			RebbK.keymap = itr->first.first;
			if (!pDoc->m_pAttrCtrl->GetRebb(RebbK, RebbD))
			{
				continue;
			}

			auto* pRebarBeam = new CMemberRebarBeam;

			_l_convert_rebb(enSector::I, pRebarBeam, RebbD.Top_I, RebbD.Bot_I, RebbD.dT, RebbD.dB);
			_l_convert_rebb(enSector::M, pRebarBeam, RebbD.Top_C, RebbD.Bot_C, RebbD.dT, RebbD.dB);
			_l_convert_rebb(enSector::J, pRebarBeam, RebbD.Top_J, RebbD.Bot_J, RebbD.dT, RebbD.dB);

			CBindingData Data;
			Data.pData = pRebarBeam;

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = itr->second;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			for (const auto MembK : itr->second)
			{
				((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebbFromResult(MembK, RebbD);

				auto* pRebarBeam = new CMemberRebarBeam;

				_l_convert_rebb(enSector::I, pRebarBeam, RebbD.Top_I, RebbD.Bot_I, RebbD.dT, RebbD.dB);
				_l_convert_rebb(enSector::M, pRebarBeam, RebbD.Top_C, RebbD.Bot_C, RebbD.dT, RebbD.dB);
				_l_convert_rebb(enSector::J, pRebarBeam, RebbD.Top_J, RebbD.Bot_J, RebbD.dT, RebbD.dB);

				CBindingData Data;
				Data.pData = pRebarBeam;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBeam_Civil(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::unordered_map<std::pair<T_RCHK_K, T_MATL_K>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCBeam(vKeyMemb[i]))
			continue;

		auto RchkK = pDoc->m_pAttrCtrl->GetRchkByElemK(vKeyMemb[i]);

		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RchkK, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		if (!m_pDataBase->IsDesigning())
		{
			auto* pRebarBeam = new CMemberRebarBeam;

			T_RCHK_D RchkD;
			if (!pDoc->m_pAttrCtrl->GetRchk(itr->first.first, RchkD))
			{
				continue;
			}
			if (RchkD.nType != 0)
			{
				continue; // 여기는 보만~~~
			}

			T_MATD_D MatdD;
			if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
			{
				continue; // User
			}

			T_MATL_REBAR RebarData = RebarData = MatdD.MainRebarData;
			T_MATL_REBAR SRebarData = MatdD.SubRebarData;

			for (int i = 0; i < 3; ++i)
			{
				CRebarBeam Pattern;

				// Top rebar
				int nLayer = 0;
				for (int j = 0; j < T_RCHK_BEAM_LAYER; ++j)
				{
					if (RchkD.BEAM.strTopBarNa1[i][j].IsEmpty()) break;
					nLayer++;
				}

				Pattern.aTop.resize(nLayer);
				for (int j = 0; j < nLayer; ++j)
				{
					CStringW strDia = CT2W(RchkD.BEAM.strTopBarNa1[i][j]);
					double dDia = pDoc->m_pMatlDB->Get_RebarDia(RchkD.BEAM.strTopBarNa1[i][j]);
					double dArea = pDoc->m_pMatlDB->Get_RebarArea(RchkD.BEAM.strTopBarNa1[i][j]);
					double dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(RchkD.BEAM.strTopBarNa1[i][j]);
					double dWeight = pDoc->m_pMatlDB->Get_RebarWeight(RchkD.BEAM.strTopBarNa1[i][j]);
					if (!RchkD.BEAM.strTopBarNa2[i][j].IsEmpty()) // 교번배근인 경우
					{
						strDia += CT2W(_T("+") + RchkD.BEAM.strTopBarNa2[i][j]);
						dDia += pDoc->m_pMatlDB->Get_RebarDia(RchkD.BEAM.strTopBarNa2[i][j]);
						dArea += pDoc->m_pMatlDB->Get_RebarArea(RchkD.BEAM.strTopBarNa2[i][j]);
						dOutDia += pDoc->m_pMatlDB->Get_RebarOutDia(RchkD.BEAM.strTopBarNa2[i][j]);
						dWeight += pDoc->m_pMatlDB->Get_RebarWeight(RchkD.BEAM.strTopBarNa2[i][j]);
						// average
						dDia *= 0.5;
						dArea *= 0.5;
						dOutDia *= 0.5;
						dWeight *= 0.5;
					}

					Pattern.aTop[j].nNum = RchkD.BEAM.iTopBarNum[i][j];
					Pattern.aTop[j].Rebar.Dia.strName = strDia;
					Pattern.aTop[j].Rebar.Dia.Geom.dDia = dDia;
					Pattern.aTop[j].Rebar.Dia.Geom.dArea = dArea;
					Pattern.aTop[j].Rebar.Dia.Geom.dOutDia = dOutDia;
					Pattern.aTop[j].Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * dDia;
					Pattern.aTop[j].Rebar.Dia.Matl.dfy = RebarData.B_fy;
					Pattern.aTop[j].Rebar.Dia.Matl.dEs = RebarData.B_Elast;
					Pattern.aTop[j].dCenterCover = RchkD.BEAM.dDt[i][j];
				}

				// Bottom rebar
				nLayer = 0;
				for (int j = 0; j < T_RCHK_BEAM_LAYER; ++j)
				{
					if (RchkD.BEAM.strBotBarNa1[i][j].IsEmpty()) break;
					nLayer++;
				}

				Pattern.aBot.resize(nLayer);
				for (int j = 0; j < T_RCHK_BEAM_LAYER; ++j)
				{
					if (RchkD.BEAM.strBotBarNa1[i][j].IsEmpty()) break;
					CStringW strDia = CT2W(RchkD.BEAM.strBotBarNa1[i][j]);
					double dDia = pDoc->m_pMatlDB->Get_RebarDia(RchkD.BEAM.strBotBarNa1[i][j]);
					double dArea = pDoc->m_pMatlDB->Get_RebarArea(RchkD.BEAM.strBotBarNa1[i][j]);
					double dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(RchkD.BEAM.strBotBarNa1[i][j]);
					double dWeight = pDoc->m_pMatlDB->Get_RebarWeight(RchkD.BEAM.strBotBarNa1[i][j]);
					if (!RchkD.BEAM.strBotBarNa2[i][j].IsEmpty()) // 교번배근인 경우
					{
						strDia += CT2W(_T("+") + RchkD.BEAM.strBotBarNa2[i][j]);
						dDia += pDoc->m_pMatlDB->Get_RebarDia(RchkD.BEAM.strBotBarNa2[i][j]);
						dArea += pDoc->m_pMatlDB->Get_RebarArea(RchkD.BEAM.strBotBarNa2[i][j]);
						dOutDia += pDoc->m_pMatlDB->Get_RebarOutDia(RchkD.BEAM.strBotBarNa2[i][j]);
						dWeight += pDoc->m_pMatlDB->Get_RebarWeight(RchkD.BEAM.strBotBarNa2[i][j]);
						// average
						dDia *= 0.5;
						dArea *= 0.5;
						dOutDia *= 0.5;
						dWeight *= 0.5;
					}

					Pattern.aBot[j].nNum = RchkD.BEAM.iBotBarNum[i][j];
					Pattern.aBot[j].Rebar.Dia.strName = strDia;
					Pattern.aBot[j].Rebar.Dia.Geom.dDia = dDia;
					Pattern.aBot[j].Rebar.Dia.Geom.dArea = dArea;
					Pattern.aBot[j].Rebar.Dia.Geom.dOutDia = dOutDia;
					Pattern.aBot[j].Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * dDia;
					Pattern.aBot[j].Rebar.Dia.Matl.dfy = RebarData.B_fy;
					Pattern.aBot[j].Rebar.Dia.Matl.dEs = RebarData.B_Elast;
					Pattern.aBot[j].dCenterCover = RchkD.BEAM.dDb[i][j];
				}

				// Shear
				Pattern.Shear.nLeg = RchkD.BEAM.dSubBarNum[i];
				Pattern.Shear.Rebar.dSpace = RchkD.BEAM.dSubBarDist[i];
				Pattern.Shear.Rebar.Dia = CreateRebarDia(RchkD.BEAM.strSubBarNa[i], SRebarData);

				// Torsion with Bundle, logitudinal

				// Bent-up

				// Group ????
				enSector Sector = [i]()
				{
					switch (i)
					{
					case 0:
						return enSector::I;
					case 1:
						return enSector::M;
					case 2:
						return enSector::J;
					default:
						break;
					}

					return enSector::I;
				}();

				pRebarBeam->SetRebar(Sector, Pattern);
			}
			CBindingData Data;
			Data.pData = pRebarBeam;

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = itr->second;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			//산정돌고나서 report 출력하기 위해
			for (auto MembK : itr->second)
			{
				auto* pRebarBeam = new CMemberRebarBeam;

				if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebarFromResult(enMemb::Beam, MembK, pRebarBeam))
				{
					_SAFE_DELETE(pRebarBeam); continue;
				}

				CBindingData Data;
				Data.pData = pRebarBeam;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarColumn(const std::vector<long>& vKeyMemb)
{
	// Get All Member
	std::vector<long> vElemK = vKeyMemb;
	if (!m_pDataBase->IsDesigning())
	{
		vElemK.clear();

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_ELEM_K_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			vElemK.push_back(aElemK[i]);
		}
	}

	std::vector<CBindingData> vBindingData;
#if defined(_CIVIL)
	if (!AddRebarColumn_Civil(vElemK, vBindingData)) return false;
#else
	if (!AddRebarColumn_Gen(vElemK, vBindingData)) return false;
#endif

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarGeneral(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddRebarColumn_Gen(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CMembTypeUtil Util;

	std::unordered_map<std::pair<T_REBC_KEY, T_MATL_K>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCColumn(vKeyMemb[i]))
			continue;

		T_REBC_K RebcK = pDoc->m_pAttrCtrl->GetRebcByElemK(vKeyMemb[i]);
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RebcK.keymap, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		T_REBC_K RebcK;
		RebcK.keymap = (itr->first).first;
		T_REBC_D RebcD;
		T_SECT_D sData;
		sData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetSectDesign(RebcK.key.sectK, sData))
		{
			ASSERT(0); continue;
		}

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
		{
			continue; // User
		}

		T_MATL_REBAR RMatl = MatdD.MainRebarData;
		T_MATL_REBAR RStirrupMatl = MatdD.SubRebarData;

		auto l_pRebarColumn = [&]()
		{
			auto* pRebarColumn = new CMemberRebarColumn;

			CRebarColumn Pattern;
			Pattern.aMain.resize(1);
			
			if (RebcD.bUseCornerRebar)
			{
				Pattern.aMain[0].Rebar.Dia = CreateRebarDia(RebcD.CornerRebarName, RMatl);
				Pattern.aMain[0].Rebar.Type = TMainDia::enSub::Edge;
				Pattern.aMain[0].Rebar.Sub = CreateRebarDia(RebcD.MainRebarName, RMatl);
			}
			else
			{
				Pattern.aMain[0].Rebar.Dia = CreateRebarDia(RebcD.MainRebarName, RMatl);
			}

			bool bRect = false;
			if (sData.SectBefore.Shape == D_SECT_SHAPE_REG_SR || sData.SectBefore.Shape == D_SECT_SHAPE_REG_P)
			{
				Pattern.aMain[0].nRow = RebcD.nQrb;
			}
			else if (sData.SectBefore.Shape == D_SECT_SHAPE_REG_SB)
			{
				bRect = true;
				Pattern.aMain[0].nRow = RebcD.nRow;
				Pattern.aMain[0].nCol = ((RebcD.nQrb - 2 * RebcD.nRow) / 2) + 2; // Rect
			}
			else ASSERT(0);

			Pattern.aMain[0].dCenterCover = RebcD.dConcCenter;

			Pattern.Shear.Rebar.Dia = CreateRebarDia(RebcD.SubRebarName, RStirrupMatl);
			Pattern.Shear.Rebar.dSpace = RebcD.dSubRebarSpace;
			Pattern.Shear.nHoopType = RebcD.nHoopType == 1 ? enHoopTypeTie : enHoopTypeSpiral;
			Pattern.Shear.nLegHor = bRect ? RebcD.iSubRebarNum[0] : 2;// Rect 이외 다면은 leg 2개 고정. CRCSDataBase::Set_ColmBarDataToMyDB 참조
			Pattern.Shear.nLegVer = bRect ? RebcD.iSubRebarNum[1] : 2;

			pRebarColumn->SetRebar(enSector::End, Pattern);

			Pattern.Shear.Rebar.Dia = CreateRebarDia(RebcD.SubRebarName2, RStirrupMatl);
			Pattern.Shear.Rebar.dSpace = RebcD.dSubRebarSpace2;
			Pattern.Shear.nLegHor = bRect ? RebcD.iSubRebarNum2[0] : 2;
			Pattern.Shear.nLegVer = bRect ? RebcD.iSubRebarNum2[1] : 2;

			pRebarColumn->SetRebar(enSector::Mid, Pattern);

			return pRebarColumn;
		};

		if (!m_pDataBase->IsDesigning())
		{
			if (!pDoc->m_pAttrCtrl->GetRebc(RebcK, RebcD))
			{
				continue;
			}
			CBindingData Data;
			Data.pData = l_pRebarColumn();

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = itr->second;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			for (const auto MembK : itr->second)
			{
				((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebcFromResult(MembK, RebcD);
				CBindingData Data;
				Data.pData = l_pRebarColumn();

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarColumn_Civil(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::unordered_map<std::pair<T_RCHK_K, T_MATL_K>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCColumn(vKeyMemb[i]))
			continue;

		auto RchkK = pDoc->m_pAttrCtrl->GetRchkByElemK(vKeyMemb[i]);
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RchkK, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		if (!m_pDataBase->IsDesigning())
		{
			T_RCHK_D RchkD;
			if (!pDoc->m_pAttrCtrl->GetRchk(itr->first.first, RchkD))
			{
				continue;
			}
			if (RchkD.nType != 1)
			{
				continue; // 여기는 기둥만~~~
			}
			ASSERT(RchkD.COLM.bIMJSame); //이거 UI에서 IMJ모두 다 TRUE로만 받는것 같은데?

			T_SECT_D sData;
			sData.Initialize();
			if (!pDoc->m_pAttrCtrl->GetSectDesign((itr->first).first, sData))
			{
				ASSERT(0); continue;
			}

			T_MATD_D MatdD;
			if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
			{
				continue; // User
			}

			T_MATL_REBAR RebarData = MatdD.MainRebarData;
			T_MATL_REBAR SRebarData = MatdD.SubRebarData;

			if (sData.SectBefore.Shape == D_SECT_SHAPE_REG_SR || sData.SectBefore.Shape == D_SECT_SHAPE_REG_P || sData.SectBefore.Shape == D_SECT_SHAPE_REG_SB || sData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
			{
				auto* pRebarColumn = new CMemberRebarColumn;
				CRebarColumn Pattern;

				// Main rebar
				int nSector = 0;
				int nLayer = 0;
				for (int j = 0; j < T_RCHK_COLM_LAYER; ++j)
				{
					if (RchkD.COLM.strBarNa1[nSector][0][j].IsEmpty()) break;
					nLayer++;
				}
				Pattern.aMain.resize(nLayer);

				for (int j = 0; j < nLayer; ++j)
				{
					// 일단 gen처럼 철근이 배근된다고 가정; pos1 = pos2, pos3 고려 안함.
					Pattern.aMain[j].Rebar.Dia = CreateRebarDia(RchkD.COLM.strBarNa1[nSector][0][j], RebarData);
					if (!RchkD.COLM.strBarNa1[nSector][0][j].IsEmpty())
					{
						Pattern.aMain[j].Sub = CreateRebarDia(RchkD.COLM.strBarNa1[nSector][1][j], RebarData);
						Pattern.aMain[j].Type = TMainRebarColumn::enSub::LeftRight;
					}
					Pattern.aMain[j].dCenterCover = RchkD.COLM.dDc[nSector][j];
					if (!RchkD.COLM.strBarNa2[nSector][0][j].IsEmpty())
					{
						Pattern.aMain[j].Rebar.Type = TMainDia::enSub::Edge;
						Pattern.aMain[j].Rebar.Sub = CreateRebarDia(RchkD.COLM.strBarNa2[nSector][0][j], RebarData);
					}

					if (sData.SectBefore.Shape == D_SECT_SHAPE_REG_SR || sData.SectBefore.Shape == D_SECT_SHAPE_REG_P)
					{
						Pattern.aMain[j].nRow = RchkD.COLM.iBarNum[nSector][0][j];
					}
					else if (sData.SectBefore.Shape == D_SECT_SHAPE_REG_SB || sData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
					{
						Pattern.aMain[j].nCol = RchkD.COLM.iBarNum[nSector][0][j];
						Pattern.aMain[j].nRow = RchkD.COLM.iBarNum[nSector][1][j] + 2;
					}
				}

				// Shear
				Pattern.Shear.nLegVer = RchkD.COLM.dSubBarNum_z[nSector];
				Pattern.Shear.nLegHor = RchkD.COLM.dSubBarNum_y[nSector];
				Pattern.Shear.Rebar.dSpace = RchkD.COLM.dSubBarDist[nSector];
				Pattern.Shear.Rebar.Dia = CreateRebarDia(RchkD.COLM.strSubBarNa[nSector], SRebarData);
				pRebarColumn->SetRebar(enSector::End, Pattern);

				CBindingData Data;
				Data.pData = pRebarColumn;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey = itr->second;

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
			else
			{
				//SolidOctagon:
				//Octagon:
				//Track:
				//SolidTrack:
				//HalfTrack:
				auto* pMemberRebarColumn = new CMemberRebarGeneral;
				_UMD_SECT_MAKER_COL_RBAR Input;
				_UMD_RC_RBAR Output;

				for (int i = 0; i < T_RCHK_COLM_LAYER; ++i)
				{
					Input.dDc[i] = RchkD.COLM.dDc[0][i];
					for (int j = 0; j < 3; ++j)
					{
						Input.iBarNum[j][i] = RchkD.COLM.iBarNum[0][j][i];
						Input.strBarDiaNa1[j][i] = (CStringW)CT2W(RchkD.COLM.strBarNa1[0][j][i]);
						Input.strBarDiaNa2[j][i] = (CStringW)CT2W(RchkD.COLM.strBarNa2[0][j][i]);
						Input.dBarDiaNa1[j][i] = pDoc->m_pMatlDB->Get_RebarDia(RchkD.COLM.strBarNa1[0][j][i]);
						Input.dBarDiaNa2[j][i] = pDoc->m_pMatlDB->Get_RebarDia(RchkD.COLM.strBarNa2[0][j][i]);
						Input.dBarAreaNa1[j][i] = pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strBarNa1[0][j][i]);
						Input.dBarAreaNa2[j][i] = pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strBarNa2[0][j][i]);
					}
				}

				//배근된 좌표 정보가 left bottom 기준이다.
				if (!ITmpDgnCalcBaseLib::GetInstance()->GetArrdAnnoyingSectionColumn(false, (CStringW)CT2W(sData.SectBefore.Shape), sData.SectBefore.SectI.Size, Input, Output))
				{
					ASSERT(0); continue;
				}
				double dOffsetY = sData.SectBefore.SectI.Design.YBar;
				double dOffsetZ = sData.SectBefore.SectI.Design.ZBar;
				int nArrdSize = Output.arRbarUnit.GetCount();
				CRebarGeneral General;
				General.vSingle.reserve(nArrdSize);
                General.dCenterCover = Input.dDc[0];
				POSITION Pos = Output.arRbarUnit.GetStartPosition();
				while (Pos)
				{
					int nKey;
					_UMD_RC_RBAR_UNIT Rebar;
					Output.arRbarUnit.GetNextAssoc(Pos, nKey, Rebar);
					lib::DVector2d p;
					p.vec[0] = Rebar.dyz[0] - dOffsetY;
					p.vec[1] = Rebar.dyz[1] - dOffsetZ;

					TDia Dia = CreateRebarDia(Rebar.strName, RebarData);

					CMainRebarSingle Single;
					Single.Set(p, Dia);
					General.vSingle.push_back(Single);
				}

				// Shear
				General.Shear.nLegVer = RchkD.COLM.dSubBarNum_z[0];
				General.Shear.nLegHor = RchkD.COLM.dSubBarNum_y[0];
				General.Shear.Rebar.dSpace = RchkD.COLM.dSubBarDist[0];
				General.Shear.Rebar.Dia = CreateRebarDia(RchkD.COLM.strSubBarNa[0], SRebarData);
				pMemberRebarColumn->SetRebar(enSector::End, General);

				CBindingData Data;
				Data.pData = pMemberRebarColumn;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey = itr->second;

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
		else
		{
			//산정돌고나서 report 출력하기 위해
			for (auto MembK : itr->second)
			{
				auto* pRebarColumn = new CMemberRebarColumn;

				if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebarFromResult(enMemb::Column, MembK, pRebarColumn))
				{
					_SAFE_DELETE(pRebarColumn); continue;
				}

				CBindingData Data;
				Data.pData = pRebarColumn;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	//general column rebar
	if (!m_pDataBase->IsDesigning())
	{
		T_KEY_LIST aGenRebar;
		pDoc->m_pAttrCtrl->GetRebtKeyList(aGenRebar);
		for (int i = 0; i < aGenRebar.GetSize(); ++i)
		{
			T_REBT_D RebtD;
			if (!pDoc->m_pAttrCtrl->GetRebt(aGenRebar[i], RebtD))
			{
				ASSERT(0); continue;
			}

			T_SECT_K SectK = aGenRebar[i];
			T_KEY_LIST aSectK, aElemK;
			aSectK.Add(SectK);
			pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectK, &T_KEY_LIST(), &aElemK);

			if (aElemK.GetSize() == 0)
			{
				continue;
			}

			T_MATD_D MatdD;
			if (!DataCtrl.Get_DgnGenMatd(aElemK[0], MatdD))
			{
				continue; // User
			}

			T_MATL_REBAR RebarData = MatdD.MainRebarData;
			T_MATL_REBAR SRebarData = MatdD.SubRebarData;

			auto* pRebarColumn = new CMemberRebarGeneral;
			int nArrdSize = RebtD.arMainRebar[0].GetSize();
			CRebarGeneral General;
			General.vSingle.reserve(nArrdSize);
			for (int j = 0; j < nArrdSize; ++j)
			{
				auto Rebar = RebtD.arMainRebar[0][j];
				lib::DVector2d p;
				p.vec[0] = Rebar.dCenterY;
				p.vec[1] = Rebar.dCenterZ;

				TDia Dia = CreateRebarDia(Rebar.strRebarName, RebarData);

				CMainRebarSingle Single;
				Single.Set(p, Dia);
				General.vSingle.push_back(Single);
			}

			// Shear
			General.Shear.nLegHor = RebtD.ShearRebar->dSubRebarNum[0];
			General.Shear.nLegVer = RebtD.ShearRebar->dSubRebarNum[1];
			General.Shear.Rebar.dSpace = RebtD.ShearRebar->dSubRebarSpace;
			General.Shear.Rebar.Dia = CreateRebarDia(RebtD.ShearRebar->strSubRebarName, SRebarData);
			pRebarColumn->SetRebar(enSector::End, General);

			CBindingData Data;
			Data.pData = pRebarColumn;

			CRelationalQuery Query;
			Query.SetSimpleQueryData(DataType::Memb, Macro::CArray2Vec<T_ELEM_K, long>(aElemK));

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBrace(const std::vector<long>& vKeyMemb)
{
	// Get All Member
	std::vector<long> vElemK = vKeyMemb;
	if (!m_pDataBase->IsDesigning())
	{
		vElemK.clear();

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_ELEM_K_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			vElemK.push_back(aElemK[i]);
		}
	}

	std::vector<CBindingData> vBindingData;
#if defined(_CIVIL)
	if (!AddRebarBrace_Civil(vElemK, vBindingData)) return false;
#else
	if (!AddRebarBrace_Gen(vElemK, vBindingData)) return false;
#endif

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarWall(const std::vector<long>& vKeyMemb)
{
	// Get All Member
	std::vector<long> vElemK = vKeyMemb;
	if (!m_pDataBase->IsDesigning())
	{
		vElemK.clear();

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_ELEM_K_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			vElemK.push_back(aElemK[i]);
		}
	}

	std::vector<CBindingData> vBindingData;
	if (!AddRebarWall_Gen(vElemK, vBindingData)) return false;

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarPlate(const std::vector<long>& vKeyMemb)
{
	std::vector<CBindingData> vBindingData;

	if (m_pDataBase->IsRcsPBeam())
	{
		if (!AddRebarPlateBeam(vKeyMemb, vBindingData)) return false;
	}

	if (m_pDataBase->IsRcsPWall())
	{
		if (!AddRebarPlateColumn(vKeyMemb, vBindingData)) return false;
	}

	if (m_pDataBase->IsRcsShell())
	{
		if (!AddRebarShell(vKeyMemb, vBindingData)) return false;
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarSlab()
{
	std::vector<CBindingData> vBindingData;
	if (!AddRebarSlab_Gen(vBindingData)) return false;

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBrace_Gen(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CMembTypeUtil Util;

	std::unordered_map<std::pair<T_REBR_KEY, T_MATL_K>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCBrace(vKeyMemb[i]))
			continue;

		T_REBR_K RebrK = pDoc->m_pAttrCtrl->GetRebrByElemK(vKeyMemb[i]);
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RebrK.keymap, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		T_REBR_K RebrK;
		RebrK.keymap = (itr->first).first;
		T_REBR_D RebrD;

		T_SECT_D sData;
		sData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetSectDesign(RebrK.key.sectK, sData))
		{
			ASSERT(0); continue;
		}

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
		{
			continue; // User
		}

		T_MATL_REBAR RMatl = MatdD.MainRebarData;
		T_MATL_REBAR RStirrupMatl = MatdD.SubRebarData;

		auto l_pRebarBrace = [&]()
		{
			auto* pRebarColumn = new CMemberRebarColumn;

			CRebarColumn Pattern;
			Pattern.aMain.resize(1);
			Pattern.aMain[0].Rebar.Dia = CreateRebarDia(RebrD.MainRebarName, RMatl);

			if (sData.SectBefore.Shape == _T("SR") || sData.SectBefore.Shape == _T("P"))
			{
				Pattern.aMain[0].nRow = RebrD.nQrb;
			}
			else
			{
				Pattern.aMain[0].nRow = RebrD.nRow;
				Pattern.aMain[0].nCol = ((RebrD.nQrb - 2 * RebrD.nRow) / 2) + 2; // Rect
			}

			Pattern.aMain[0].dCenterCover = RebrD.dConcCenter;

			Pattern.Shear.Rebar.Dia = CreateRebarDia(RebrD.SubRebarName, RStirrupMatl);
			Pattern.Shear.Rebar.dSpace = RebrD.dSubRebarSpace;
			Pattern.Shear.nLegHor = RebrD.iSubRebarNum[0];
			Pattern.Shear.nLegVer = RebrD.iSubRebarNum[1];

			pRebarColumn->SetRebar(enSector::End, Pattern);

			Pattern.Shear.Rebar.Dia = CreateRebarDia(RebrD.SubRebarName2, RStirrupMatl);
			Pattern.Shear.Rebar.dSpace = RebrD.dSubRebarSpace2;
			Pattern.Shear.nLegHor = RebrD.iSubRebarNum2[0];
			Pattern.Shear.nLegVer = RebrD.iSubRebarNum2[1];

			pRebarColumn->SetRebar(enSector::Mid, Pattern);

			return pRebarColumn;
		};

		if (!m_pDataBase->IsDesigning())
		{
			if (!pDoc->m_pAttrCtrl->GetRebr(RebrK, RebrD))
			{
				continue;
			}
			CBindingData Data;
			Data.pData = l_pRebarBrace();

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = itr->second;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			for (const auto MembK : itr->second)
			{
				((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebrFromResult(MembK, RebrD);
				CBindingData Data;
				Data.pData = l_pRebarBrace();

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarBrace_Civil(const std::vector<long>& vKeyMemb, std::vector<CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::unordered_map<std::pair<T_RCHK_K, T_MATL_K>, std::vector<long>, pair_hash> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCBrace(vKeyMemb[i]))
			continue;

		auto RchkK = pDoc->m_pAttrCtrl->GetRchkByElemK(vKeyMemb[i]);
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(vKeyMemb[i], ElemD))
		{
			ASSERT(0); continue;
		}

		std::vector<long>& vMembK = mData[std::make_pair(RchkK, ElemD.elmat)];
		vMembK.push_back(vKeyMemb[i]);
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		T_RCHK_D RchkD;
		if (!pDoc->m_pAttrCtrl->GetRchk(itr->first.first, RchkD))
		{
			ASSERT(0); continue;
		}
		if (RchkD.nType != 2)
		{
			continue; // 여기는 브레이스만~~~
		}

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
		{
			continue; // User
		}

		T_MATL_REBAR RebarData = MatdD.MainRebarData;
		T_MATL_REBAR SRebarData = MatdD.SubRebarData;

		auto* pRebarColumn = new CMemberRebarColumn;
		for (int i = 0; i < 3; ++i)
		{
			CRebarColumn Pattern;

			// Main rebar
			int nLayer = 0;
			for (int j = 0; j < T_RCHK_COLM_LAYER; ++j)
			{
				if (RchkD.COLM.strBarNa1[i][0][j].IsEmpty()) break;
				nLayer++;
			}
			Pattern.aMain.resize(nLayer);

			for (int j = 0; j < nLayer; ++j)
			{
				// 일단 gen처럼 철근이 배근된다고 가정; pos1 = pos2, pos3 고려 안함.
				Pattern.aMain[j].Rebar.Dia = CreateRebarDia(RchkD.COLM.strBarNa1[i][0][j], RebarData);
				Pattern.aMain[j].dCenterCover = RchkD.COLM.dDc[i][j];
				if (!RchkD.COLM.strBarNa2[i][0][j].IsEmpty())
				{
					Pattern.aMain[j].Rebar.Type = TMainDia::enSub::Edge;
					Pattern.aMain[j].Rebar.Sub = CreateRebarDia(RchkD.COLM.strBarNa2[i][0][j], RebarData);
				}

				Pattern.aMain[j].nCol = RchkD.COLM.iBarNum[i][0][j];
				Pattern.aMain[j].nRow = RchkD.COLM.iBarNum[i][1][j] + 2;
			}

			// Shear
			Pattern.Shear.nLegVer = RchkD.COLM.dSubBarNum_z[i];
			Pattern.Shear.nLegHor = RchkD.COLM.dSubBarNum_y[i];
			Pattern.Shear.Rebar.dSpace = RchkD.COLM.dSubBarDist[i];
			Pattern.Shear.Rebar.Dia = CreateRebarDia(RchkD.COLM.strSubBarNa[i], SRebarData);

			switch (i)
			{
			case 0:
				pRebarColumn->SetRebar(enSector::I, Pattern);
				break;
			case 1:
				pRebarColumn->SetRebar(enSector::M, Pattern);
				break;
			case 2:
				pRebarColumn->SetRebar(enSector::J, Pattern);
				break;
			default:
				break;
			}
		}

		CBindingData Data;
		Data.pData = pRebarColumn;

		CRelationalQuery Query;
		Query.DataType = DataType::Memb;
		Query.vOnlyOneKey = itr->second;

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarWall_Gen(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CMembTypeUtil Util;

	std::unordered_map<T_WALL_KEY, std::vector<long>> mData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		if (!Util.IsRCWall(vKeyMemb[i]))
			continue;

		unsigned int nInternalWallID;
		if (!pDoc->m_pPostCtrl->GetInternalWallId(vKeyMemb[i], nInternalWallID))
		{
			ASSERT(0); continue;
		}

		T_WALL_KEY WallKey;
		if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallKey))	
		{
			ASSERT(0); continue;
		}
		T_WALL_D WallD;
		if (!pDoc->m_pPostCtrl->GetWallData(WallKey, WallD))
		{
			ASSERT(0); continue;
		}
		T_ELEM_K MembK = WallD.RootElemKey;

		std::vector<long>& vMembK = mData[WallKey];
		vMembK.push_back(MembK);
	}

	T_DCRB_D DcrbD;
	if (!pDoc->m_pAttrCtrl->GetDcrb(DcrbD))
	{
		DcrbD.Initialize();
	}

	CDgnDataCtrl DataCtrl;
	for (auto itr = mData.begin(); itr != mData.end(); ++itr)
	{
		T_WALL_KEY WallK = itr->first;
		T_REBW_D RebwD;
		T_REBW_K RebwK;
		int nBEH1(0), nBEH2(0);
		RebwK.keymap = WallK;

		T_WALL_D wData;
		wData.Initialize();
		if (!pDoc->m_pPostCtrl->GetWallData(WallK, wData))	wData.Initialize();

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(itr->second.front(), MatdD))
		{
			continue; // User
		}

		T_MATL_REBAR RMatl = MatdD.MainRebarData;
		T_MATL_REBAR RStirrupMatl = MatdD.SubRebarData;

		auto l_pRebarWall = [&]()
		{
			auto* pRebarWall = new CMemberRebarWall;

			auto _l_GetFyDia = [&DcrbD, &RMatl, &RStirrupMatl](int nVerHor, CString strDia) -> double
			{

				switch (nVerHor)
				{
				case 0:
				{
					if (DcrbD.bMatlByDia)
					{
						for (int i = 0; i < DcrbD.aVertMatlByDia.GetSize(); ++i)
						{
							if (strDia.CompareNoCase(DcrbD.aVertMatlByDia[i].strDia) == 0)
							{
								return DcrbD.aVertMatlByDia[i].dFy;
							}
						}
					}
					return RMatl.B_fy;
				}
				break;
				case 1:
				{
					if (DcrbD.bMatlByDia)
					{
						for (int i = 0; i < DcrbD.aHorzMatlByDia.GetSize(); ++i)
						{
							if (strDia.CompareNoCase(DcrbD.aHorzMatlByDia[i].strDia) == 0)
							{
								return DcrbD.aHorzMatlByDia[i].dFy;
							}
						}
					}
					return RStirrupMatl.B_fy;
				}
				break;
				default:
					break;
				}
				return RMatl.B_fy;
			};

			CRebarWall Wall;
			Wall.VerBar.Rebar.Dia.strName = CT2W(RebwD.VerticalRebarName);
			Wall.VerBar.Rebar.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(RebwD.VerticalRebarName);
			Wall.VerBar.Rebar.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(RebwD.VerticalRebarName);
			Wall.VerBar.Rebar.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(RebwD.VerticalRebarName);
			Wall.VerBar.Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * Wall.VerBar.Rebar.Dia.Geom.dDia;
			Wall.VerBar.Rebar.Dia.Matl.dfy = _l_GetFyDia(0, RebwD.VerticalRebarName);
			Wall.VerBar.Rebar.Dia.Matl.dEs = Get_Es(nRCCode, Wall.VerBar.Rebar.Dia.Matl.dfy);
			Wall.VerBar.dCenterCover = RebwD.dDe;
			Wall.VerBar.dSideCover   = RebwD.dDw;
			Wall.VerBar.dSpace = RebwD.dVerticalRebarSpace;

			Wall.HorBar.Rebar.Dia.strName = CT2W(RebwD.HorizonRebarName);
			Wall.HorBar.Rebar.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(RebwD.HorizonRebarName);
			Wall.HorBar.Rebar.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(RebwD.HorizonRebarName);
			Wall.HorBar.Rebar.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(RebwD.HorizonRebarName);
			Wall.HorBar.Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * Wall.HorBar.Rebar.Dia.Geom.dDia;
			Wall.HorBar.Rebar.Dia.Matl.dfy = _l_GetFyDia(1, RebwD.HorizonRebarName);
			Wall.HorBar.Rebar.Dia.Matl.dEs = Get_Es(nRCCode, Wall.HorBar.Rebar.Dia.Matl.dfy);
			Wall.HorBar.dCenterCover = RebwD.dDe;
			Wall.HorBar.dSpace = RebwD.dHorizonRebarSpace;

			Wall.EndBar.bSubRebar = RebwD.nNumEndRebar > 0 ? true : false;
			Wall.EndBar.unRowNum = 2;
			Wall.EndBar.unColNum = RebwD.nNumEndRebar * 0.5;
			Wall.EndBar.Rebar.Dia.strName = CT2W(RebwD.EndRebarName);
			Wall.EndBar.Rebar.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(RebwD.EndRebarName);
			Wall.EndBar.Rebar.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(RebwD.EndRebarName);
			Wall.EndBar.Rebar.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(RebwD.EndRebarName);
			Wall.EndBar.Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * Wall.EndBar.Rebar.Dia.Geom.dDia;
			Wall.EndBar.Rebar.Dia.Matl.dfy = _l_GetFyDia(0, RebwD.EndRebarName);
			Wall.EndBar.Rebar.Dia.Matl.dEs = Get_Es(nRCCode, Wall.EndBar.Rebar.Dia.Matl.dfy);
			Wall.EndBar.dSpace = RebwD.dEndRebarSpace;

			const double dhc2 = wData.dThickness - (2.0*RebwD.dDe + Wall.EndBar.Rebar.Dia.Geom.dDia + Wall.HorBar.Rebar.Dia.Geom.dDia);
			Wall.BEHorBar.bShearRein = RebwD.dBEHorizonRebarSpace > 0.0 ? true : false;
			Wall.BEHorBar.unLegX = Wall.BEHorBar.bShearRein ? nBEH1 : 0;
			Wall.BEHorBar.unLegY = Wall.BEHorBar.bShearRein ? nBEH2 : 0;

			CString strBEHorBar = RebwD.BEHorizonRebarName.IsEmpty() ? RebwD.HorizonRebarName : RebwD.BEHorizonRebarName;
			Wall.BEHorBar.Rebar.strDia = CT2W(strBEHorBar);
			Wall.BEHorBar.Rebar.dDia = pDoc->m_pMatlDB->Get_RebarDia(strBEHorBar);
			Wall.BEHorBar.Rebar.dArea = pDoc->m_pMatlDB->Get_RebarArea(strBEHorBar);
			Wall.BEHorBar.Rebar.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strBEHorBar);
			Wall.BEHorBar.Rebar.dFy = _l_GetFyDia(1, strBEHorBar);
			Wall.BEHorBar.Rebar.dEs = RMatl.B_Elast;
			Wall.BEHorBar.dSpace = RebwD.dBEHorizonRebarSpace;
			Wall.BEHorBar.dBELength = RebwD.dBELength;

			pRebarWall->SetRebar(Wall);

			return pRebarWall;
		};

		if (!m_pDataBase->IsDesigning())
		{
			((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebwFromResult(itr->second.front(), RebwD, nBEH1, nBEH2); // for BEH1,2
			if (!pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD))
			{
				continue;
			}
			
			CBindingData Data;
			Data.pData = l_pRebarWall();

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = itr->second;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			for (const auto MembK : itr->second)
			{
				((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebwFromResult(itr->second.front(), RebwD, nBEH1, nBEH2); // for report
				CBindingData Data;
				Data.pData = l_pRebarWall();

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(MembK);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarSlab_Gen(std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	const bool bDesign = m_pDataBase->IsDesigning();
	T_KEY_LIST aRbms;
	aRbms.RemoveAll();
	if (!bDesign)
	{
		pDoc->m_pAttrCtrl->GetRbmsKeyList(aRbms);
	}
	int nRbms = aRbms.GetSize();

	auto l_ValidRebar = [](const CString& strRebar)
	{
		return (strRebar == _T("") || strRebar.CompareNoCase(_T("NONE")) == 0) ? false : true;
	};

	T_MSRB_D MsrbD;
	pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);
	auto l_GetfyByDia = [&MsrbD](const CString& strDia, const double& dfy)
	{
		if (MsrbD.bMatlByDia)
		{
			for (int i = 0; i < MsrbD.aMatlByDia.GetSize(); ++i)
			{
				if (strDia.CompareNoCase(MsrbD.aMatlByDia[i].strDia) == 0)
				{
					return MsrbD.aMatlByDia[i].dFy;
				}
			}
		}
		return dfy;
	};


	std::unordered_map<UINT, std::array<double, 2>> mRebarMatl;
	CDgnDataCtrl DataCtrl;
	if (nRbms != 0)
	{
		std::unordered_map<UINT, CSectionRebar2D*> mBasicRebar;
		std::unordered_map<UINT, CSectionRebar2DAdd*> mAddRebar;

		for (int i = 0; i < nRbms; ++i)
		{
			const T_RBMS_K RbmsK = aRbms.GetAt(i);
			T_RBMS_D RbmsD;
			if (!pDoc->m_pAttrCtrl->GetRbms(RbmsK, RbmsD)) continue;
			const bool bBasic = l_ValidRebar(RbmsD.strBasicRebarName);
			const bool bAdd1 = l_ValidRebar(RbmsD.strAdd1RebarName);
			const bool bAdd2 = bAdd1 ? l_ValidRebar(RbmsD.strAdd2RebarName) : false;

			if (!bBasic && !bAdd1 && !bAdd2)
				continue;

			CArray<T_ELEM_K, T_ELEM_K> aElemK;
			int nElem = pDoc->m_pAttrCtrl->GetElemKeyListUseRbms(RbmsK, aElemK);

			if (nElem == 0)
				continue;

			for (int k = 0; k < nElem; ++k)
			{
				const auto ElemK = aElemK[k];
				std::array<double, 2> aMatd;
				auto itrMatd = mRebarMatl.find(ElemK);
				if (itrMatd == mRebarMatl.end())
				{
					T_MATD_D MatdD;
					if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD)) continue; // User
					aMatd[0] = MatdD.MainRebarData.B_fy;
					aMatd[1] = MatdD.MainRebarData.B_Elast;
					mRebarMatl.insert(std::make_pair(ElemK, aMatd));
				}
				else
				{
					aMatd = itrMatd->second;
				}

				TMainRebar2D BasicD;
				if (bBasic)
				{
					SetRebar2D(RbmsD.strBasicRebarName, RbmsD.dBasicRebarSpace, RbmsD.dCover, BasicD);
					const auto dfy = l_GetfyByDia(RbmsD.strBasicRebarName, aMatd[0]);
					SetRebarMatl(dfy, Get_Es(nRCCode, dfy), BasicD.Rebar.Dia.Matl);
				}

				TMainRebar2D Add1D, Add2D;
				if (bAdd1)
				{
					SetRebar2D(RbmsD.strAdd1RebarName, RbmsD.dAdd1RebarSpace, RbmsD.dCover, Add1D);
					const auto dfy = l_GetfyByDia(RbmsD.strAdd1RebarName, aMatd[0]);
					SetRebarMatl(dfy, Get_Es(nRCCode, dfy), Add1D.Rebar.Dia.Matl);
					if (bAdd2)
					{
						SetRebar2D(RbmsD.strAdd2RebarName, RbmsD.dAdd2RebarSpace, RbmsD.dCover, Add2D);
						const auto dfy = l_GetfyByDia(RbmsD.strAdd2RebarName, aMatd[0]);
						SetRebarMatl(dfy, Get_Es(nRCCode, dfy), Add2D.Rebar.Dia.Matl);
						Add1D.Rebar.Sub = Add2D.Rebar.Dia;
						Add1D.Rebar.Type = TMainDia::enSub::Altr;
					}
				}

				//Basic
				if (bBasic)
				{
					auto itr = mBasicRebar.find(ElemK);
					CSectionRebar2D* pBasic = nullptr;
					if (itr == mBasicRebar.end())
					{
						pBasic = new CSectionRebar2D;
						pBasic->Rebar2D.aTopX.resize(1);
						pBasic->Rebar2D.aBotX.resize(1);
						pBasic->Rebar2D.aTopY.resize(1);
						pBasic->Rebar2D.aBotY.resize(1);
						mBasicRebar.insert(std::make_pair(ElemK, pBasic));
					}
					else
					{
						pBasic = itr->second;
					}
					
					// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2  
					switch (RbmsD.nLayerType)
					{
					case 0:
						{
							pBasic->Rebar2D.aTopX[0] = BasicD;
						}
						break;
					case 1:
						{
							pBasic->Rebar2D.aBotX[0] = BasicD;
						}
						break;
					case 2:
						{
							pBasic->Rebar2D.aTopY[0] = BasicD;
						}
						break;
					case 3:
						{
							pBasic->Rebar2D.aBotY[0] = BasicD;
						}
						break;
					default:ASSERT(0);  break;
					}
				}

				//AddRebar
				if (bAdd1)
				{
					auto itr = mAddRebar.find(ElemK);
					CSectionRebar2DAdd* pAddRebar = nullptr;
					if (itr == mAddRebar.end())
					{
						pAddRebar = new CSectionRebar2DAdd;
						pAddRebar->Rebar2D.aTopX.resize(1);
						pAddRebar->Rebar2D.aBotX.resize(1);
						pAddRebar->Rebar2D.aTopY.resize(1);
						pAddRebar->Rebar2D.aBotY.resize(1);
						mAddRebar.insert(std::make_pair(ElemK, pAddRebar));
					}
					else
					{
						pAddRebar = itr->second;
					}
					
					// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2  
					switch (RbmsD.nLayerType)
					{
					case 0:
						{
							pAddRebar->Rebar2D.aTopX[0] = Add1D;
						}
						break;
					case 1:
						{
							pAddRebar->Rebar2D.aBotX[0] = Add1D;
						}
						break;
					case 2:
						{
							pAddRebar->Rebar2D.aTopY[0] = Add1D;
						}
						break;
					case 3:
						{
							pAddRebar->Rebar2D.aBotY[0] = Add1D;
						}
						break;
					default:ASSERT(0);  break;
					}
				}
			}
		}

		for (const auto& Basic : mBasicRebar)
		{
			CBindingData Data;
			Data.pData = Basic.second;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.push_back(Basic.first);
			Data.vQueryInfo.push_back(Query);

			Query.DataType = DataType::CuttingLine;
			Query.vOnlyOneKey.clear();
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}

		for (const auto& Add : mAddRebar)
		{
			CBindingData Data;
			Data.pData = Add.second;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.push_back(Add.first);
			Data.vQueryInfo.push_back(Query);

			Query.DataType = DataType::CuttingLine;
			Query.vOnlyOneKey.clear();
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}
	}
	else
	{
		std::unordered_map<UINT, CSectionRebar2D*> mBasicRebar;
		std::unordered_map<UINT, CSectionRebar2DAdd*> mAddRebar;
		CArray<T_ELEM_K, T_ELEM_K> aElemK;
		int nElem = pDoc->m_pPostCtrl->GetDesignResult()->Get_2waySlabResultKeyList(0, aElemK);
		for (int i = 0; i < nElem; ++i)
		{
			const T_ELEM_K ElemK = aElemK.GetAt(i);

			T_RBMS_D aRbmsD[4][4];
			if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRbmsFromResult(ElemK, aRbmsD)) continue;

			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					T_RBMS_D RbmsD = aRbmsD[j][k];

					const bool bBasic = l_ValidRebar(RbmsD.strBasicRebarName);
					const bool bAdd1 = l_ValidRebar(RbmsD.strAdd1RebarName);
					const bool bAdd2 = bAdd1 ? l_ValidRebar(RbmsD.strAdd2RebarName) : false;

					TMainRebar2D BasicD;
					if (bBasic)
					{
						SetRebar2D(RbmsD.strBasicRebarName, RbmsD.dBasicRebarSpace, RbmsD.dCover, BasicD);
					}

					TMainRebar2D Add1D, Add2D;
					if (bAdd1)
					{
						SetRebar2D(RbmsD.strAdd1RebarName, RbmsD.dAdd1RebarSpace, RbmsD.dCover, Add1D);

						if (bAdd2)
						{
							SetRebar2D(RbmsD.strAdd2RebarName, RbmsD.dAdd2RebarSpace, RbmsD.dCover, Add2D);
						}
					}

					std::array<double, 2> aMatd;
					auto itrMatd = mRebarMatl.find(ElemK);
					if (itrMatd == mRebarMatl.end())
					{
						T_MATD_D MatdD;
						if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD)) continue; // User
						aMatd[0] = MatdD.MainRebarData.B_fy;
						aMatd[1] = MatdD.MainRebarData.B_Elast;
						mRebarMatl.insert(std::make_pair(ElemK, aMatd));
					}
					else
					{
						aMatd = itrMatd->second;
					}

					// Basic rebar
					if (bBasic)
					{
						const auto dfy = l_GetfyByDia((CString)BasicD.Rebar.Dia.strName, aMatd[0]);
						SetRebarMatl(dfy, Get_Es(nRCCode, dfy), BasicD.Rebar.Dia.Matl);

						auto itr = mBasicRebar.find(ElemK);
						CSectionRebar2D* pBasic = nullptr;
						if (itr == mBasicRebar.end())
						{
							pBasic = new CSectionRebar2D;
							pBasic->Rebar2D.aTopX.resize(4);
							pBasic->Rebar2D.aBotX.resize(4);
							pBasic->Rebar2D.aTopY.resize(4);
							pBasic->Rebar2D.aBotY.resize(4);
							mBasicRebar.insert(std::make_pair(ElemK, pBasic));
						}
						else
						{
							pBasic = itr->second;
						}

						// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2  
						switch (RbmsD.nLayerType)
						{
						case 0: pBasic->Rebar2D.aTopX[k] = BasicD; break;
						case 1: pBasic->Rebar2D.aBotX[k] = BasicD; break;
						case 2: pBasic->Rebar2D.aTopY[k] = BasicD; break;
						case 3: pBasic->Rebar2D.aBotY[k] = BasicD; break;
						default:ASSERT(0);  break;
						}
					}

					// Add rebar
					if (bAdd1)
					{
						const auto dfy = l_GetfyByDia((CString)Add1D.Rebar.Dia.strName, aMatd[0]);
						SetRebarMatl(dfy, Get_Es(nRCCode, dfy), Add1D.Rebar.Dia.Matl);
						if (bAdd2)
						{
							const auto dfy = l_GetfyByDia((CString)Add2D.Rebar.Dia.strName, aMatd[0]);
							SetRebarMatl(dfy, Get_Es(nRCCode, dfy), Add2D.Rebar.Dia.Matl);
							Add1D.Rebar.Sub = Add2D.Rebar.Dia;
							Add1D.Rebar.Type = TMainDia::enSub::Altr;
						}

						auto itr = mAddRebar.find(ElemK);
						CSectionRebar2DAdd* pAdd = nullptr;
						if (itr == mAddRebar.end())
						{
							pAdd = new CSectionRebar2DAdd;
							pAdd->Rebar2D.aTopX.resize(4);
							pAdd->Rebar2D.aBotX.resize(4);
							pAdd->Rebar2D.aTopY.resize(4);
							pAdd->Rebar2D.aBotY.resize(4);
							mAddRebar.insert(std::make_pair(ElemK, pAdd));
						}
						else
						{
							pAdd = itr->second;
						}

						// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2  
						switch (RbmsD.nLayerType)
						{
						case 0: pAdd->Rebar2D.aTopX[k] = Add1D; break;
						case 1: pAdd->Rebar2D.aBotX[k] = Add1D; break;
						case 2: pAdd->Rebar2D.aTopY[k] = Add1D; break;
						case 3: pAdd->Rebar2D.aBotY[k] = Add1D; break;
						default:ASSERT(0);  break;
						}
					}
				}
			}
		}

		for (const auto& Basic : mBasicRebar)
		{
			CBindingData Data;
			Data.pData = Basic.second;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.push_back(Basic.first);
			Data.vQueryInfo.push_back(Query);

			Query.DataType = DataType::CuttingLine;
			Query.vOnlyOneKey.clear();
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}

		for (const auto& Add : mAddRebar)
		{
			CBindingData Data;
			Data.pData = Add.second;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.push_back(Add.first);
			Data.vQueryInfo.push_back(Query);

			Query.DataType = DataType::CuttingLine;
			Query.vOnlyOneKey.clear();
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}
	}

	if (bDesign)
	{
		return AddRebarSlab1way_Gen(vBindingData);
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarSlab1way_Gen(std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!m_pDataBase->IsDesigning()) ASSERT(0);
	if (!((IDgnPerformDataBaseRC*)m_pDataBase)->IsPrint1waySlabDgn()) return true;

	// only Gen
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	if (pServiceHeadOffice == nullptr)
	{
		ASSERT(0);
		return true;
	}
	CArray<UINT, UINT> aCutlK;
	aCutlK.RemoveAll();
	if (!pServiceHeadOffice->ReqService(_ULS(gps), D_GPS_GET_CUTLINE_KEYS, (void*)&aCutlK)) return true;

	CDgnDataCtrl DataCtrl;
	std::unordered_map<UINT, CSectionRebar2DAdd*> mAddRebar;
	std::unordered_map<UINT, std::array<double, 2>> mRebarMatl;

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	int nCutline = aCutlK.GetSize();
	for (int i = 0; i < nCutline; ++i)
	{
		T_CUTL_K CutlK = aCutlK[i];

		for (int j = 0; j < 2; ++j)
		{
			T_RSL1_D SlabRes;
			if (!pDoc->m_pPostCtrl->GetDesignResult()->Get_1waySlabResult(CutlK, SlabRes, j == 0)) continue;
			if (SlabRes.Rebar.strSize.IsEmpty()) continue;

			const auto ElemK = SlabRes.nCriticalElemKey;
			const double dcover = SlabRes.Rebar.dCover;

			TMainRebar2D Add1D, Add2D;
			SetRebar2D(SlabRes.Rebar.strSize, SlabRes.Rebar.dSpace, dcover, Add1D);

			std::array<double, 2> aMatd;
			auto itrMatd = mRebarMatl.find(ElemK);
			if (itrMatd == mRebarMatl.end())
			{
				T_MATD_D MatdD;
				if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD)) continue; // User
				aMatd[0] = MatdD.MainRebarData.B_fy;
				aMatd[1] = Get_Es(nRCCode, aMatd[0]);
				mRebarMatl.insert(std::make_pair(ElemK, aMatd));
			}
			else
			{
				aMatd = itrMatd->second;
			}

			// Add rebar
			SetRebarMatl(aMatd[0], aMatd[1], Add1D.Rebar.Dia.Matl);

			auto itr = mAddRebar.find(CutlK);
			CSectionRebar2DAdd* pAdd = nullptr;
			if (itr == mAddRebar.end())
			{
				pAdd = new CSectionRebar2DAdd;
				pAdd->Rebar2D.aTopX.resize(1);
				pAdd->Rebar2D.aBotX.resize(1);
				pAdd->Rebar2D.aTopY.resize(1);
				pAdd->Rebar2D.aBotY.resize(1);
				mAddRebar.insert(std::make_pair(CutlK, pAdd));
			}
			else
			{
				pAdd = itr->second;
			}

			switch (j)
			{
			case 0: pAdd->Rebar2D.aTopY[0] = Add1D; break;
			case 1: pAdd->Rebar2D.aBotY[0] = Add1D; break;
			default:ASSERT(0);  break;
			}
		}
	}

	for (const auto& Add : mAddRebar)
	{
		CBindingData Data;
		CSectionRebar2D* p2D = new CSectionRebar2D;
		Data.pData = p2D;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		Query.vOnlyOneKey.clear();
		Data.vQueryInfo.push_back(Query);

		Query.DataType = DataType::CuttingLine;
		Query.vOnlyOneKey.push_back(Add.first);
		Data.vQueryInfo.push_back(Query);

		vBindingData.push_back(Data);
	}

	for (const auto& Add : mAddRebar)
	{
		CBindingData Data;
		Data.pData = Add.second;

		CRelationalQuery Query;
		Query.DataType = DataType::Elem;
		Query.vOnlyOneKey.clear();
		Data.vQueryInfo.push_back(Query);

		Query.DataType = DataType::CuttingLine;
		Query.vOnlyOneKey.push_back(Add.first);
		Data.vQueryInfo.push_back(Query);

		vBindingData.push_back(Data);
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarPlateBeam(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!m_pDataBase->IsDesigning())
	{
		T_KEY_LIST aKeyList;
		int nSize = pDoc->m_pAttrCtrl2->GetRipbKeyList(aKeyList);
		for (int i = 0; i < nSize; ++i)
		{
			T_KEY_LIST aElemK;
			int nElemSize = pDoc->m_pAttrCtrl2->GetElemKeyListUseRipb(aKeyList[i], aElemK);
			if (nElemSize == 0)
				continue;

			T_RIPB_D RipbD;
			if (!pDoc->m_pAttrCtrl2->GetRipb(aKeyList[i], RipbD))
			{
				continue;
			}

			auto PMainFyEs = GetRCMainRebarFyEs(aElemK[0]);
			auto PSubFyEs = GetRCSubRebarFyEs(aElemK[0]);

			auto* pRebarPlate = new CSectionRebarPlate;
			CRebarPlate RebarPlate;
			for (int j = 0; j < RipbD.MainRebar.nBotLayer; ++j)
			{
				TMainRebar2D Rebar2DBot;
				SetRebar2D(RipbD.MainRebar.strBotBarNa1[j], RipbD.MainRebar.strBotBarNa2[j], RipbD.MainRebar.dBotBarCTC[j], RipbD.MainRebar.dDb[j], PMainFyEs.first, PMainFyEs.second, Rebar2DBot);
				RebarPlate.Rebar2D.aBotX.push_back(Rebar2DBot);
			}

			for (int j = 0; j < RipbD.SubRebar.nBotLayer; ++j)
			{
				TMainRebar2D Rebar2DBot;
                SetRebar2D(RipbD.SubRebar.strBotBarNa1[j], _T(""), RipbD.SubRebar.dBotBarCTC[j], RipbD.MainRebar.dDb[j], PMainFyEs.first, PMainFyEs.second, Rebar2DBot);
                RebarPlate.Rebar2D.aBotY.push_back(Rebar2DBot);
			}

			for (int j = 0; j < RipbD.MainRebar.nTopLayer; ++j)
			{
				TMainRebar2D Rebar2DTop;
				SetRebar2D(RipbD.MainRebar.strTopBarNa1[j], RipbD.MainRebar.strTopBarNa2[j], RipbD.MainRebar.dTopBarCTC[j], RipbD.MainRebar.dDt[j], PMainFyEs.first, PMainFyEs.second, Rebar2DTop);
				RebarPlate.Rebar2D.aTopX.push_back(Rebar2DTop);
			}

			for (int j = 0; j < RipbD.SubRebar.nTopLayer; ++j)
			{
				TMainRebar2D Rebar2DTop;
                SetRebar2D(RipbD.SubRebar.strTopBarNa1[j], _T(""), RipbD.SubRebar.dTopBarCTC[j], RipbD.MainRebar.dDb[j], PMainFyEs.first, PMainFyEs.second, Rebar2DTop);
                RebarPlate.Rebar2D.aTopY.push_back(Rebar2DTop);
			}

			TShearDia Shear;
			SetRebarMatl(PSubFyEs.first, PSubFyEs.second, Shear.Dia.Matl);
			if (RipbD.bChkSubBar)
			{
				SetShearRebar2D(RipbD.strSubBarNa, RipbD.dSubBarDist, Shear);				
				RebarPlate.ShearX.nLeg = RipbD.nSubBarNum;
				RebarPlate.ShearX.Rebar = Shear;
			}
			else
			{
				RebarPlate.ShearX.nLeg = 0;
				RebarPlate.ShearX.Rebar = Shear;				
			}
			pRebarPlate->SetRebar(RebarPlate);

			CBindingData Data;
			Data.pData = pRebarPlate;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.clear();
			for (int j = 0; j < aElemK.GetSize(); ++j)
			{
				Query.vOnlyOneKey.push_back(aElemK[j]);
			}
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}
	else
	{
		for (auto ElemK : vKeyMemb)
		{
			auto* pRebar = new CSectionRebarPlate;

			if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebarFromResult(enMemb::PlateBeam, ElemK, pRebar))
			{
				_SAFE_DELETE(pRebar); continue;
			}

			CBindingData Data;
			Data.pData = pRebar;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.clear();
			Query.vOnlyOneKey.push_back(ElemK);
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarPlateColumn(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (!m_pDataBase->IsDesigning())
	{
		T_KEY_LIST aKeyList;
		int nSize = pDoc->m_pAttrCtrl2->GetRipcKeyList(aKeyList);
		for (int i = 0; i < nSize; ++i)
		{
			T_KEY_LIST aElemK;
			int nElemSize = pDoc->m_pAttrCtrl2->GetElemKeyListUseRipc(aKeyList[i], aElemK);
			if (nElemSize == 0)
				continue;

			T_RIPC_D RipbD;
			if (!pDoc->m_pAttrCtrl2->GetRipc(aKeyList[i], RipbD))
			{
				continue;
			}

			auto PMainFyEs = GetRCMainRebarFyEs(aElemK[0]);
			auto PSubFyEs = GetRCSubRebarFyEs(aElemK[0]);

			auto* pRebarPlate = new CSectionRebarPlate;
			CRebarPlate RebarPlate;
			for (int j = 0; j < RipbD.MainRebar.nBotLayer; ++j)
			{
				TMainRebar2D Rebar2DBot;
				SetRebar2D(RipbD.MainRebar.strBotBarNa1[j], RipbD.MainRebar.strBotBarNa2[j], RipbD.MainRebar.dBotBarCTC[j], RipbD.MainRebar.dDb[j], PMainFyEs.first, PMainFyEs.second, Rebar2DBot);
				RebarPlate.Rebar2D.aBotX.push_back(Rebar2DBot);
				SetRebar2D(RipbD.SubRebar.strBotBarNa1[j], _T(""), RipbD.SubRebar.dBotBarCTC[j], RipbD.MainRebar.dDb[j], PMainFyEs.first, PMainFyEs.second, Rebar2DBot);
				RebarPlate.Rebar2D.aBotY.push_back(Rebar2DBot);
			}

			for (int j = 0; j < RipbD.MainRebar.nTopLayer; ++j)
			{
				TMainRebar2D Rebar2DTop;
				SetRebar2D(RipbD.MainRebar.strTopBarNa1[j], RipbD.MainRebar.strTopBarNa2[j], RipbD.MainRebar.dTopBarCTC[j], RipbD.MainRebar.dDt[j], PMainFyEs.first, PMainFyEs.second, Rebar2DTop);
				RebarPlate.Rebar2D.aTopX.push_back(Rebar2DTop);
				SetRebar2D(RipbD.SubRebar.strTopBarNa1[j], _T(""), RipbD.SubRebar.dTopBarCTC[j], RipbD.MainRebar.dDt[j], PMainFyEs.first, PMainFyEs.second, Rebar2DTop);
				RebarPlate.Rebar2D.aTopY.push_back(Rebar2DTop);
			}

			if (RipbD.bChkSubBar)
			{
				TShearDia Shear;
				SetShearRebar2D(RipbD.strSubBarNa, RipbD.dSubBarDist, Shear);
				SetRebarMatl(PSubFyEs.first, PSubFyEs.second, Shear.Dia.Matl);
				RebarPlate.ShearX.nLeg = RipbD.nSubBarNum;
				RebarPlate.ShearX.Rebar = Shear;
			}
			else
			{
				RebarPlate.ShearX.nLeg = 0;
			}
			pRebarPlate->SetRebar(RebarPlate);

			CBindingData Data;
			Data.pData = pRebarPlate;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.clear();
			for (int j = 0; j < aElemK.GetSize(); ++j)
			{
				Query.vOnlyOneKey.push_back(aElemK[j]);
			}
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}
	else
	{
		for (auto ElemK : vKeyMemb)
		{
			auto* pRebar = new CSectionRebarPlate;

			if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRebarFromResult(enMemb::PlateColumn, ElemK, pRebar))
			{
				_SAFE_DELETE(pRebar); continue;
			}

			CBindingData Data;
			Data.pData = pRebar;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.clear();
			Query.vOnlyOneKey.push_back(ElemK);

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarShell(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	int iUnitLen = D_UNITSYS_LENGTH_INDEX_M;

	int nSize = vKeyMemb.size();
	if (!m_pDataBase->IsDesigning())
	{
		for (int i = 0; i < nSize; ++i)
		{
			if (!Util.IsRCShell(vKeyMemb[i]))
				continue;

			CArray<T_RBSL_D, T_RBSL_D&> aRsbl;
			if (!DataCtrl.Get_DgnConRbsl(vKeyMemb[i], aRsbl, iUnitLen))
			{
				continue;
			}

			if (aRsbl.GetSize() != 4)
			{
				ASSERT(0); continue;
			}

			auto PMainFyEs = GetRCMainRebarFyEs(vKeyMemb[i]);
			auto PSubFyEs = GetRCSubRebarFyEs(vKeyMemb[i]);

			//교번배근 처리해야함.
			auto* pRebarPlate = new CSectionRebarPlate;
			CRebarPlate RebarPlate;
			TMainRebar2D Rebar2D;
			SetRebar2D(aRsbl[0].strAdd1RebarName, aRsbl[0].strAdd2RebarName, aRsbl[0].dAdd1RebarSpace, aRsbl[0].dCover, PMainFyEs.first, PMainFyEs.second, Rebar2D);
			RebarPlate.Rebar2D.aTopX.push_back(Rebar2D);
			SetRebar2D(aRsbl[1].strAdd1RebarName, aRsbl[1].strAdd2RebarName, aRsbl[1].dAdd1RebarSpace, aRsbl[1].dCover, PMainFyEs.first, PMainFyEs.second, Rebar2D);
			RebarPlate.Rebar2D.aBotX.push_back(Rebar2D);

			SetRebar2D(aRsbl[2].strAdd1RebarName, aRsbl[2].strAdd2RebarName, aRsbl[2].dAdd1RebarSpace, aRsbl[2].dCover, PMainFyEs.first, PMainFyEs.second, Rebar2D);
			RebarPlate.Rebar2D.aTopY.push_back(Rebar2D);
			SetRebar2D(aRsbl[3].strAdd1RebarName, aRsbl[3].strAdd2RebarName, aRsbl[3].dAdd1RebarSpace, aRsbl[3].dCover, PMainFyEs.first, PMainFyEs.second, Rebar2D);
			RebarPlate.Rebar2D.aBotY.push_back(Rebar2D);

			pRebarPlate->SetRebar(RebarPlate);

			CBindingData Data;
			Data.pData = pRebarPlate;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.clear();
			Query.vOnlyOneKey.push_back(vKeyMemb[i]);
			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
	}
	else
	{
		std::unordered_map<UINT, CSectionRebarPlate*> mBasicRebar;
		std::unordered_map<UINT, std::array<double, 2>> mRebarMatl;
		CArray<T_ELEM_K, T_ELEM_K> aElemK;
		int nElem = pDoc->m_pPostCtrl->GetDesignResult()->Get_ShellResultKeyList(aElemK);
		for (int i = 0; i < nElem; ++i)
		{
			const T_ELEM_K ElemK = aElemK.GetAt(i);

			T_RBSL_D aRbmsD[4];
			if (!((IDgnPerformDataBaseRC*)m_pDataBase)->GetRbslFromResult(ElemK, aRbmsD)) continue;

			auto l_ValidRebar = [](const CString& strRebar)
			{
				return (strRebar == _T("") || strRebar.CompareNoCase(_T("NONE")) == 0) ? false : true;
			};

			for (int j = 0; j < 4; ++j)
			{
				T_RBSL_D RbmsD = aRbmsD[j];

				const bool bBasic = l_ValidRebar(RbmsD.strAdd1RebarName);
				const bool bAdd1 = bBasic ? l_ValidRebar(RbmsD.strAdd2RebarName) : false;

				TMainRebar2D PlateRebar;

				if (!bBasic && !bAdd1)
					continue;

				std::array<double, 2> aMatd;
				auto itrMatd = mRebarMatl.find(ElemK);
				if (itrMatd == mRebarMatl.end())
				{
					T_MATD_D MatdD;
					if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD)) continue; // User
					aMatd[0] = MatdD.MainRebarData.B_fy;
					aMatd[1] = MatdD.MainRebarData.B_Elast;
					mRebarMatl.insert(std::make_pair(ElemK, aMatd));
				}
				else
				{
					aMatd = itrMatd->second;
				}

				SetRebar2D(RbmsD.strAdd1RebarName, RbmsD.strAdd2RebarName, RbmsD.dAdd1RebarSpace, RbmsD.dCover, aMatd[0], aMatd[1], PlateRebar);

				auto itr = mBasicRebar.find(ElemK);
				CSectionRebarPlate* pPlateRebar = nullptr;
				if (itr == mBasicRebar.end())
				{
					pPlateRebar = new CSectionRebarPlate;
					pPlateRebar->RebarPlate.Rebar2D.aTopX.resize(1);
					pPlateRebar->RebarPlate.Rebar2D.aBotX.resize(1);
					pPlateRebar->RebarPlate.Rebar2D.aTopY.resize(1);
					pPlateRebar->RebarPlate.Rebar2D.aBotY.resize(1);
					mBasicRebar.insert(std::make_pair(ElemK, pPlateRebar));
				}
				else
				{
					pPlateRebar = itr->second;
				}

				// 0=Top-Dir1, 1=Bot-Dir1, 2=Top-Dir2, 3=Bot-Dir2  
				switch (RbmsD.nLayerType)
				{
				case 0: pPlateRebar->RebarPlate.Rebar2D.aTopX[0] = PlateRebar; break;
				case 1: pPlateRebar->RebarPlate.Rebar2D.aBotX[0] = PlateRebar; break;
				case 2: pPlateRebar->RebarPlate.Rebar2D.aTopY[0] = PlateRebar; break;
				case 3: pPlateRebar->RebarPlate.Rebar2D.aBotY[0] = PlateRebar; break;
				default:ASSERT(0);  break;
				}
			}
		}

		for (const auto& Basic : mBasicRebar)
		{
			CBindingData Data;
			Data.pData = Basic.second;

			CRelationalQuery Query;
			Query.DataType = DataType::Elem;
			Query.vOnlyOneKey.push_back(Basic.first);
			Data.vQueryInfo.push_back(Query);

			Query.DataType = DataType::CuttingLine;
			Query.vOnlyOneKey.clear();
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}
	}

	return true;
}

bool CDgnPerformInputConverter::AddBendingCoefficient(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		T_CBFT_D CbftD;
		if (!DataCtrl.Get_DgnStlCbft(vKeyMemb[i], CbftD))
		{
			continue;
		}

		auto* pBendingC = new CBendingCoefficient;
		pBendingC->Factor.v1 = CbftD.bAuto;
		pBendingC->Factor.v2 = CbftD.Cb;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, vKeyMemb[i]);
		Data.pData = pBendingC;
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddShearCoefficient(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		T_CVFT_D CvftD;
		if (!DataCtrl.Get_DgnStlCvft(vKeyMemb[i], CvftD))
		{
			continue;
		}

		auto* pShearC = new CShearCoefficient;
		pShearC->Factor.v1 = CvftD.bAuto;
		pShearC->Factor.v2 = CvftD.Cv;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, vKeyMemb[i]);
		Data.pData = pShearC;
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSafetyFactor(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();

	CMembTypeUtil Util;
	std::vector<long> RemainRCKey, RemainSTLKey;
	for (int i = 0; i < nElem; ++i)
	{
		if (Util.IsSTL(aElemK[i]))
		{
			RemainSTLKey.push_back(aElemK[i]);
		}
		else if (Util.IsRC(aElemK[i]))
		{
			if (Util.IsWall((T_ELEM_K)aElemK[i]))
				RemainRCKey.push_back(aElemK[i]);
			else if (Util.IsPlate((T_ELEM_K)aElemK[i]))
			{
				const auto ElemK = CMembCtrl::GetMembCtrl()->GetRepElem((T_ELEM_K)aElemK[i]);
				if(ElemK != 0) RemainRCKey.push_back(ElemK);
			}
			else
				RemainRCKey.push_back(aElemK[i]);
		}
		else
		{
			//ASSERT(0);
		}
	}

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	if (RemainSTLKey.size() > 0)
	{
		T_DSTL_D StlD;
		if (!DataCtrl.Get_DgnStlDstl(StlD))
		{
			ASSERT(0); return false;
		}

		auto* pSafetyFactor = new CSafetyFactor;
		pSafetyFactor->aFactor[CSafetyFactor::Compression].v1 = false;
		pSafetyFactor->aFactor[CSafetyFactor::Compression].v2 = StlD.dPhi[2];
		pSafetyFactor->aFactor[CSafetyFactor::Tension].v1 = false;
		pSafetyFactor->aFactor[CSafetyFactor::Tension].v2 = StlD.dPhi[0];
		pSafetyFactor->aFactor[CSafetyFactor::Bending].v1 = false;
		pSafetyFactor->aFactor[CSafetyFactor::Bending].v2 = StlD.dPhi[3];
		pSafetyFactor->aFactor[CSafetyFactor::Shear].v1 = false;
		pSafetyFactor->aFactor[CSafetyFactor::Shear].v2 = StlD.dPhi[4];

		CBindingData Data;
		Data.pData = pSafetyFactor;

		CRelationalQuery Query;
		Query.DataType = DataType::Memb;
		Query.vOnlyOneKey = RemainSTLKey;

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	if (RemainRCKey.size() > 0)
	{
		T_DCON_D ConD;
		if (!CDgnDataCtrl().Get_DgnConDcon(ConD))
		{
			ASSERT(0); return false;
		}
		std::array<dgn::lib::TFactor, CSafetyFactor::Number> aFactor;
		aFactor[CSafetyFactor::Compression].v1 = false;
		aFactor[CSafetyFactor::Compression].v2 = ConD.dPhi[3];
		aFactor[CSafetyFactor::CompressionSpiral].v1 = false;
		aFactor[CSafetyFactor::CompressionSpiral].v2 = ConD.dPhi[2];
		aFactor[CSafetyFactor::Tension].v1 = false;
		aFactor[CSafetyFactor::Tension].v2 = ConD.dPhi[1];
		aFactor[CSafetyFactor::Bending].v1 = false;
		aFactor[CSafetyFactor::Bending].v2 = ConD.dPhi[0];
		aFactor[CSafetyFactor::Shear].v1 = false;
		aFactor[CSafetyFactor::Shear].v2 = ConD.dPhi[4];
		aFactor[CSafetyFactor::Torsion].v1 = false;
		aFactor[CSafetyFactor::Torsion].v2 = ConD.dPhi[4];
		aFactor[CSafetyFactor::Joint].v1 = false;
		aFactor[CSafetyFactor::Joint].v2 = 0.85;

		if (m_pDataBase->IsDesigning())
		{
			auto* pSafetyFactor = new CSafetyFactor;
			std::copy(aFactor.begin(), aFactor.end(), pSafetyFactor->aFactor.begin());

			CBindingData Data;
			Data.pData = pSafetyFactor;

			CRelationalQuery Query;
			Query.DataType = DataType::Memb;
			Query.vOnlyOneKey = RemainRCKey;

			Data.vQueryInfo.push_back(Query);
			vBindingData.push_back(Data);
		}
		else
		{
			for (const auto iMemb : RemainRCKey)
			{
				bool bSpiral = false;
				auto* pSafetyFactor = new CSafetyFactor;
				std::copy(aFactor.begin(), aFactor.end(), pSafetyFactor->aFactor.begin());

				if (Util.IsColumn(iMemb))
				{
					auto RebcK = pDoc->m_pAttrCtrl->GetRebcByElemK(iMemb);
					T_REBC_D RebcD;
					if (!pDoc->m_pAttrCtrl->GetRebc(RebcK, RebcD)) RebcD.Initialize();
					bSpiral = RebcD.nHoopType == 2 ? true : false;
				}
				else if (Util.IsBrace(iMemb))
				{
					auto RebrK = pDoc->m_pAttrCtrl->GetRebrByElemK(iMemb);
					T_REBR_D RebrD;
					if (!pDoc->m_pAttrCtrl->GetRebr(RebrK, RebrD)) RebrD.Initialize();
					bSpiral = RebrD.nHoopType == 2 ? true : false;
				}
				else
				{
					bSpiral = false;
				}

				if (bSpiral)
				{
					aFactor[CSafetyFactor::Compression].v2 = ConD.dPhi[2];
				}

				CBindingData Data;
				Data.pData = pSafetyFactor;

				CRelationalQuery Query;
				Query.DataType = DataType::Memb;
				Query.vOnlyOneKey.push_back(iMemb);

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddCombinedRatioMethod(const std::vector<long>& vKeyMemb)
{
	// 이거 ngen은 필요 없다는데 진짜 필요한건지 다시 한번 확인해볼것.https://midasitdev.atlassian.net/browse/DGNDEV-985
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double CDgnPerformInputConverter::Get_UnitWidth(const int iDgnCode)
{
	// Unit; m
	double dUnitWidth = 1.0; // m
	switch (iDgnCode)
	{
		// in.
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318_19:
	case ACI318_25:
	case AASHTO_LC20:
	case AASHTO_LC24:
		dUnitWidth = 12.0 * (2.54 / 100.0); // 1ft
		break;
	default:
		break;
	}

	return dUnitWidth;
}

double CDgnPerformInputConverter::Get_ClearDistArray(const int iDgnCode, const double& dBarDia, bool bSRC)
{
	// Unit; m
	// Only Beam.
	const double dGravelDia = 0.025;
	double dClearDistArray = dGravelDia;
	if (bSRC)
	{
		switch (iDgnCode)
		{
		case AIK_SRC2K:
		{
			dClearDistArray = max(1.5*dBarDia, 2.5 / 100.0);		// max(1.5BarDia, 2.5 cm).
		}
		break;
		case AIJ_SRC01:
		{
			dClearDistArray = max(1.5*dBarDia, 25.0 / 1000.0);		// max(1.5BarDia, 25 mm).
			dClearDistArray = max(dGravelDia*(1.25), dClearDistArray);	// (gravel Dia)*(4/3).
		}
		break;
		case TWN_SRC92:
		case TWN_SRC100:
		{
			dClearDistArray = max(1.5*dBarDia, 2.5 / 100.0);		// max(1.5BarDia, 2.5 cm).
			dClearDistArray = max(dGravelDia*(1.25), dClearDistArray);	// (gravel Dia)*1.25.
		}
		break;
		default: ASSERT(0);
			break;
		}
	}
	else
	{
		switch (iDgnCode)
		{
		case ACI318_89: // 1 in.
		case ACI318_95: // 1 in.
		case ACI318_99: // 1 in.
		case ACI318_02: // 1 in.
		case ACI318_05: // 1 in.
		case ACI318_08: // 1 in. // Add by GAY. PMS:4450. ('12.03.13)
		case ACI318_11: // 1 in. // Add by GAY. PMS:4450. ('12.03.13). See ACI318-11, 7.6.2.
		case ACI318_14: // 1 in. // see ACI318-14, 25.2.2
		case ACI318_19: // 1 in. // see ACI318-14, 25.2.2
		case ACI318_25:
			dClearDistArray = 2.54 / 100.0;
			break;
		case ACI318M_14: // see ACI318M-14, 25.2.2
		case ACI318M_19: // see ACI318M-14, 25.2.2
		case ACI318M_25:
		case NSR_10:
		case NTC_DCEC_2023:
			dClearDistArray = 25.0 / 1000.0;	 // see C7.6.2.
			break;
		case AIK_USD94:  // 2.5 cm.
		case KSCE_USD96: // 2.5 cm.
		case KCI_USD99:  // 2.5 cm.
			dClearDistArray = 2.5 / 100.0;
			break;
		case KCI_USD03:	// 25 mm.
		case KCI_USD07:	// 25 mm.
		case KCI_USD12:	// 25 mm.
		case KDS_41_30_2018:
		case KDS_41_20_2022:
			dClearDistArray = 25.0 / 1000.0;
			break;
		case TWN_USD92:  // 2.5 cm.
		case TWN_USD100: // 2.5 cm.
		case TWN_USD112: // 2.5 cm.
		case AIK_WSD2K:  // 2.5 cm.
			dClearDistArray = 2.5 / 100.0;
			break;
		case AIJ_WSD99: // AIJ-WSD99, Appendix 10, 5. Table 10.3., p.373.
			dClearDistArray = max(25.0 / 1000.0, max(dBarDia*1.5, 1.25*dGravelDia)); // max(1.5*Dia, 1.25조골재, 25 mm).
			break;
		case BS8110_97:
			dClearDistArray = (2.0 / 3.0)*dGravelDia;	// (gravel Dia)*(2/3) mm.  See BS8110 : Part 1 : 1997 Section 3 3.12.11.1 b) p.69.
			break;
		case Eurocode2:
			dClearDistArray = max(dGravelDia, max(dBarDia, 20.0 / 1000.0));					// (gravel Dia) < 32 mm.  See ENV 1992-1-1 : 1991 5.2.1.1 p.179.
			break;
		case Eurocode2_04:
		case Eurocode2_2_05:
			dClearDistArray = max(dGravelDia, max(dBarDia, 20.0 / 1000.0));					// (gravel Dia) < 32 mm.  See ENV 1992-1-1 : 2000 8.2 p.131
			break;
		case IS456_2000:
			dClearDistArray = max(15.0 / 1000.0, max(dBarDia, (2.0 / 3.0)*dGravelDia));	// See IS456:2000 26.3.2(c) p.46.
			break;
		case GB50010_02: // See GB50010-02 10.2.1 p.129.
		case GB50010_10: // See [GB/T50010-10, 9.2.1-5]
		case GB50010_19:
			dClearDistArray = max(25.0 / 1000.0, dBarDia);
			break;
		case CSA_A233_94:
			dClearDistArray = 1.4 * max(dBarDia, dGravelDia);	// max(BarDia, gravel Dia).
			dClearDistArray = max(30.0 / 1000.0, dClearDistArray);				// 30 mm.
			break;
		case BS5400_4_90:
			dClearDistArray = 2.5 / 1000.0; //dhgg + 5mm
			break;
		case TMH07_89:
			dClearDistArray = 2.5 / 1000.0; //dhgg + 5mm
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	return dClearDistArray;
}

double CDgnPerformInputConverter::Get_VerRebarSpaceByString(const int iDgnCode, CString strVerSpace)
{
	double dSpace = 0.0;
	BOOL bInchUnit = FALSE;
	if (strVerSpace.GetLength() > 0)
	{
		if (strVerSpace.Left(1) == _T("@"))
		{
			int iTextCount = 0;
			if (strVerSpace.Right(1) == _T("\"")) { iTextCount = 2; bInchUnit = TRUE; }	// @12").
			else { iTextCount = 1; bInchUnit = FALSE; }	// @300.
			CString strSpace = strVerSpace.Mid(1, strVerSpace.GetLength() - iTextCount);
			dSpace = _ttof(strSpace);
		}
	}

	int iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
	double dLenFactor = 0.0;
	switch (iDgnCode)
	{
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14:
	case ACI318_19:
	case ACI318_25:
	{
		if (bInchUnit)	dLenFactor = 1.0;
		else			dLenFactor = 1. / 25.4;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_IN;
		break;
	}
	case ACI318M_14:
	case ACI318M_19:
	case ACI318M_25:
	case NSR_10:
	case KCI_USD03:
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case AIJ_WSD99:
	case BS8110_97:
	case Eurocode2:
	case Eurocode2_04:
	case Eurocode2_2_05:
	case BS5400_4_90:
	case TMH07_89:
	case IS456_2000:
	case GB50010_02:
	case GB50010_10:
	case GB50010_19:
	case CSA_A233_94:
	case NTC_DCEC_2023:
	{
		if (bInchUnit)	dLenFactor = 25.4;
		else			dLenFactor = 1.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
		break;
	}
	case AIK_USD94:
	case KSCE_USD96:
	case KCI_USD99:
	case TWN_USD92:
	case TWN_USD100:
	case TWN_USD112:
	case AIK_WSD2K:
	{
		if (bInchUnit)	dLenFactor = 2.54;
		else			dLenFactor = 0.1;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_CM;
		break;
	}
	default:
		ASSERT(0);
		break;
	}

	// neutral unit
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, iUnitLen, D_UNITSYS_BASE_LENGTH, 1.0);

	return	dSpace * dLenFactor * dNeutralLen;
}

double CDgnPerformInputConverter::Get_RebarSpaceIncrease(const int iDgnCode, int nSubCode) const
{
	int iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
	double dSpace = 0.0;
	switch (iDgnCode)
	{
	case ACI318_14:
	case ACI318_19:
	case ACI318_25:
    case AASHTO_LC20:
	case AASHTO_LC24:
	{
		dSpace = 0.5;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_IN;
		break;
	}
	case ACI318M_14:
	case ACI318M_19:
	case ACI318M_25:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NTC_DCEC_2023:
	{
		dSpace = 10.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
		break;
	}
	case TWN_USD112:
	{
		dSpace = 1.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_CM;
		break;
	}
	default:
		ASSERT(0);
		break;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	const auto dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, iUnitLen, D_UNITSYS_BASE_LENGTH, 1.0);

	return dSpace * dNeutralLen;
}

double CDgnPerformInputConverter::Get_RebarSpaceMax(const int iDgnCode, int nSubCode) const
{
	int iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
	double dSpaceMax = 0.0;
	switch (iDgnCode)
	{
	case ACI318_14:
	case ACI318_19:
	case ACI318_25:
	{
		dSpaceMax = 40.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_IN;
		break;
	}
	case ACI318M_14:
	case ACI318M_19:
	case ACI318M_25:
	case KDS_41_30_2018:
	case KDS_41_20_2022:
	case NTC_DCEC_2023:
	{
		dSpaceMax = 1000.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_MM;
		break;
	}
	case TWN_USD112:
	{
		dSpaceMax = 100.0;
		iUnitLen = D_UNITSYS_LENGTH_INDEX_CM;
		break;
	}
	default:
		ASSERT(0);
		break;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	const auto dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, iUnitLen, D_UNITSYS_BASE_LENGTH, 1.0);

	return dSpaceMax * dNeutralLen;
}

bool CDgnPerformInputConverter::SetRebar2D(const CString strRebar, const CString strAddRebar, const double& dSpace, const double& dCover, const double& dfy, const double& dEs, TMainRebar2D& RebarD)
{
	RebarD.Init();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	RebarD.Rebar.Dia.strName = CT2W(strRebar);
	RebarD.Rebar.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(strRebar);
	RebarD.Rebar.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(strRebar);
	RebarD.Rebar.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strRebar);
	RebarD.Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * RebarD.Rebar.Dia.Geom.dDia;

	RebarD.Rebar.Dia.Matl.dfy = dfy;
	RebarD.Rebar.Dia.Matl.dEs = dEs;

	if (!strAddRebar.IsEmpty())
	{
		RebarD.Rebar.Sub.strName = CT2W(strAddRebar);
		RebarD.Rebar.Sub.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(strAddRebar);
		RebarD.Rebar.Sub.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(strAddRebar);
		RebarD.Rebar.Sub.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strAddRebar);
		RebarD.Rebar.Sub.Geom.dPerimeter = CMathFunc::m_pi * RebarD.Rebar.Sub.Geom.dDia;

		RebarD.Rebar.Sub.Matl.dfy = dfy;
		RebarD.Rebar.Sub.Matl.dEs = dEs;

		RebarD.Rebar.Type = TMainDia::enSub::Altr;
	}

	RebarD.dCenterCover = dCover;
	RebarD.dSpace = dSpace;
	return true;
}

bool CDgnPerformInputConverter::SetRebar2D(const CString strRebar, const double& dSpace, const double& dCover, TMainRebar2D& RebarD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	RebarD.Rebar.Dia.strName = CT2W(strRebar);
	RebarD.Rebar.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(strRebar);
	RebarD.Rebar.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(strRebar);
	RebarD.Rebar.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strRebar);
	RebarD.Rebar.Dia.Geom.dPerimeter = CMathFunc::m_pi * RebarD.Rebar.Dia.Geom.dDia;

	RebarD.dCenterCover = dCover;
	RebarD.dSpace = dSpace;
	return true;
}

bool CDgnPerformInputConverter::SetRebarMatl(const double& dfy, const double& dEs, TDiaMatl& RebarMatlD)
{
	RebarMatlD.dfy = dfy;
	RebarMatlD.dEs = dEs;
	return true;
}

bool CDgnPerformInputConverter::SetShearRebar2D(const CString strRebar, const double& dSpace, dgn::def::TShearDia& RebarD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	RebarD.Dia.strName = CT2W(strRebar);
	RebarD.Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(strRebar);
	RebarD.Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(strRebar);
	RebarD.Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strRebar);
	RebarD.Dia.Geom.dPerimeter = CMathFunc::m_pi * RebarD.Dia.Geom.dDia;
	RebarD.dSpace = dSpace;
	return true;
}

bool CDgnPerformInputConverter::SetShearRebarMatl2D(const double& dfy, const double& dEs, dgn::def::TShearDia& RebarD)
{
	RebarD.Dia.Matl.dfy = dfy;
	RebarD.Dia.Matl.dEs = dEs;
	return true;
}

bool CDgnPerformInputConverter::AddMembGroup(const std::vector<long>& aKeyMemb)
{
	return true;

	// Gen/Civil은 그룹개념이 없으므로 넘기지 않음.

	// 철근 업데이트 단위
	// Gen/Civil은 단면 별로 Update되니깐 단면 기준으로 data 넣어주면 됨.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	CArray<UINT, UINT> aProp;

	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))
		{
			ASSERT(0); continue;
		}

		aProp.Add(ElemD.elpro);
	}
	CArrayUtil::RemoveDup(aProp);

	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;

	// Frame type
	for (int i = 0; i < aProp.GetSize(); ++i)
	{
		T_KEY_LIST aKeyList, aKeyElem;
		aKeyList.Add(aProp[i]);
		pDoc->m_pAttrCtrl->GetKeyListSectUser(&aKeyList, nullptr, &aKeyElem);
		int nPMemb = aKeyElem.GetSize();
		if (nPMemb == 0) continue;

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aKeyElem[0], MatdD))
		{
			continue; // User
		}

		auto* pDgnMembGroup = new CDgnMembGroup;

		pDgnMembGroup->MembType.DgnType = MatdD.Type == _T("S") ? dgn::def::enDgn::STL : dgn::def::enDgn::RCS;
		switch (pDoc->m_pAttrCtrl->GetMemberType(aElemK[0]))
		{
		case D_MBTP_COLUMN:
			pDgnMembGroup->MembType.MembType = dgn::def::enMemb::Column;
			break;
		case D_MBTP_ETC:
			pDgnMembGroup->MembType.MembType = dgn::def::enMemb::Beam;
			break;
		case D_MBTP_BRACE:
			pDgnMembGroup->MembType.MembType = dgn::def::enMemb::Brace;
			break;
		case D_MBTP_WALL:
			ASSERT(0);
			pDgnMembGroup->MembType.MembType = dgn::def::enMemb::Wall;
			break;
		default:
			break;
		}
		T_SECT_D SectD;
		SectD.Initialize();
		DataCtrl.Get_DgnStlSect(aProp[i], SectD);
		pDgnMembGroup->strName = CT2W(SectD.SName);

		for (int i = 0; i < nPMemb; ++i)
		{
			pDgnMembGroup->vMemb.push_back(aKeyElem[i]);
		}

		dgn::def::CBindingData Data;
		Data.pData = pDgnMembGroup;

		CRelationalQuery Query;
		Query.DataType = dgn::def::DataType::Memb;
		for (int j = 0; j < nPMemb; ++j)
		{
			Query.vOnlyOneKey.push_back(aKeyElem[j]);
		}

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	// Wall group - wall mark + story
	CArray<T_STOR_K, T_STOR_K> aStory;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStory);
	if (aStory.GetSize() == 0) aStory.Add(0); // default
	int nStory = aStory.GetSize();

	CArray<T_WMAK_K, T_WMAK_K> aWmakList;
	pDoc->m_pAttrCtrl->GetWmakKeyList(aWmakList);
	int nWmak = aWmakList.GetSize();
	for (int i = 0; i < nWmak; ++i)
	{
		T_WMAK_K WmakK = aWmakList.GetAt(i);
		T_WMAK_D WmakD;
		if (!pDoc->m_pAttrCtrl->GetWmak(WmakK, WmakD)) continue;

		T_KEY_LIST aNode, aElem;
		pDoc->m_pAttrCtrl->GetKeyListWallUser(&WmakD.arUserWallId, &aNode, &aElemK);
		int nElem = aElemK.GetSize();
		if (nElem == 0) continue;

		std::unordered_map<long, long> mMembStory;
		for (int j = 0; j < nElem; ++j)
		{
			T_MEMB_K ShowK = 0;
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(aElemK[j], nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_K WallK;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			T_WALL_D WallD;
			if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD))          continue;
			ShowK = WallD.RootElemKey;

			T_STOR_K StorK;
			pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ShowK);
			mMembStory.insert(std::make_pair(ShowK, StorK));
		}

		for (int j = 0; j < nStory; ++j)
		{
			const T_STOR_K StorK = aStory[j];
			std::vector<long> vStroyMemb;
			for (auto& pair : mMembStory)
			{
				if (pair.second == StorK)
				{
					vStroyMemb.push_back(pair.first);
				}
			}

			if (vStroyMemb.size() > 0)
			{
				T_STOR_D StorD;
				if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD)) StorD.Initialize();
				CString strName;
				strName.Format(_T("%s(%s)"), WmakD.MarkName, StorD.StoryName);

				auto* pDgnMembGroup = new CDgnMembGroup;

				pDgnMembGroup->MembType.DgnType = dgn::def::enDgn::RCS;
				pDgnMembGroup->MembType.MembType = dgn::def::enMemb::Wall;
				pDgnMembGroup->strName = CT2W(strName);
				pDgnMembGroup->vMemb = vStroyMemb;

				dgn::def::CBindingData Data;
				Data.pData = pDgnMembGroup;

				CRelationalQuery Query;
				Query.DataType = dgn::def::DataType::Memb;
				Query.vOnlyOneKey = vStroyMemb;

				Data.vQueryInfo.push_back(Query);
				vBindingData.push_back(Data);
			}
		}
	}

	// slab group; domain
	std::vector<T_MADO_K> vMado; // domain list
	pDoc->m_pAttrCtrl->GetMadoKeyList(vMado);
	for (const auto MadoK : vMado)
	{
		std::vector<T_SBDO_K> vSbdo;
		int nSdbo = pDoc->m_pAttrCtrl->GetSbdoKeyList(MadoK, vSbdo);
		if (nSdbo == 0) continue;

		std::vector<long> vMemb;
		for (const auto SbdoK : vSbdo)
		{
			T_SBDO_D SbdoD;
			if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD)) continue;
			if (SbdoD.nType == SBDO_NONE) continue;

			T_ELEM_K_LIST aSbdoElem;
			int nSbdoElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
			if (nSbdoElem == 0) continue;
			vMemb.push_back(aSbdoElem[0]);
		}

		if (vMemb.size() == 0) continue;

		T_MADO_D MadoD;
		if (!pDoc->m_pAttrCtrl->GetMado(MadoK, MadoD)) MadoD.Initialize();

		auto* pDgnMembGroup = new CDgnMembGroup;

		pDgnMembGroup->MembType.DgnType = dgn::def::enDgn::RCS;
		pDgnMembGroup->MembType.MembType = dgn::def::enMemb::MeshedSlab;
		pDgnMembGroup->strName = CT2W(MadoD.strName);
		pDgnMembGroup->vMemb = vMemb;

		CBindingData Data;
		Data.pData = pDgnMembGroup;

		CRelationalQuery Query;
		Query.DataType = dgn::def::DataType::Memb;
		Query.vOnlyOneKey = vMemb;

		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRptCriticalInfo(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

	dgn::link::IDgnQuery* pDgnQuery = pDgnDoc->GetDgnQuery();

	CMembTypeUtil Util;

	std::unordered_map<long, bool> mSetCrtical;

	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		const auto DgnCode = CDgnTypeConverter::GetDgnCodeByElem(Key);
		const auto DgnType = CDgnTypeConverter::GetDgnType(Key);
		const auto MembType = CDgnTypeConverter::GetMembType(pDoc->m_pAttrCtrl->GetMemberType(Key));

		const auto MembK = (MembType == enMemb::MeshedSlab || MembType == enMemb::Shell) ? CMembCtrl::GetMembCtrl()->GetRepElem(Key) : Key;
		const auto itr = mSetCrtical.find(MembK);
		if (itr != mSetCrtical.end()) continue;

		CMembType MType;
		MType.DgnType = DgnType;
		MType.MembType = MembType;

		CCalcType Type(DgnCode, DgnType, MembType);
		auto enResultDatabaseType = dgn::converter::CResultDataBaseTypeUtil::GetResultDataBaseType(DgnType, MembType);
		auto* const pFinder = CDgnReportTargetItemFinderFactory::GetFinder(enResultDatabaseType);
		if (!pFinder)
		{
			ASSERT(0); return false;
		}

		pFinder->Initialize(pDgnDoc, m_pDataBase);

		TEnumList<enDgnResultItem> vItem;
		if (!pFinder->GetTargetItemList(MembK, vItem))
		{
			ASSERT(0); return false;
		}

		auto* pRptCritInfo = new CDgnRptCriticalInfo;
		if (!m_pDataBase->GetCriticalInfo(MembK, vItem.GetList(), pRptCritInfo))
		{
			continue;
		}

		dgn::def::CBindingData Data;
		Data.pData = pRptCritInfo;

		CRelationalQuery Query;
		Query.DataType = dgn::def::DataType::Memb;
		Query.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(Query);
		vBindingData.push_back(Data);

		mSetCrtical[MembK] = true;
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool dgn::converter::CDgnPerformInputConverter::AddRptCase(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	bool bAddRslt = false;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		const auto nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
		if (nRCCode == ACI318_19 || nRCCode == ACI318M_19 || 
			nRCCode == ACI318_25 || nRCCode == ACI318M_25 || 
			nRCCode == TWN_USD112 || nRCCode == NTC_DCEC_2023)
		{
			bAddRslt = DConD.bSpecialSeismic && DConD.nClass == 0 ? true : false;
		}
	}

	CMembTypeUtil Util;

	std::unordered_map<long, long> mSlab;
	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;
		for (int i = 0; i < nElem; ++i)
		{
			mSlab.insert(std::make_pair(aElem[i], aElem[0]));
		}
	}

	std::vector<long> vMemb;
	for (auto MembK : vKeyMemb)
	{
		const auto itr = mSlab.find(MembK);
		if (itr != mSlab.end())
		{
			vMemb.push_back(itr->second);
		}
		else
		{
			vMemb.push_back(MembK);
		}
	}
	Macro::UniqueSort(vMemb);

	std::vector<UINT> vWallID;
	auto* pRptCase = new CDgnRptCase;
	pRptCase->aTMembK.SetSize(vMemb.size());
	for (auto MembK : vMemb)
	{
		dgn::def::CMembType MembType;
		MembType.DgnType = dgn::converter::CDgnTypeConverter::GetDgnType(MembK);
		MembType.MembType = dgn::converter::CDgnTypeConverter::GetMembType(pDoc->m_pAttrCtrl->GetMemberType(MembK));

		pRptCase->aTMembK.PushBack(std::make_pair(MembType, MembK));

		if (bAddRslt)
		{
			if (!Util.IsRCWall(MembK)) continue;

			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(MembK, nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_K WallK;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			vWallID.push_back(WallK.key.wallid);
		}
	}

	if (bAddRslt && vWallID.size() > 0)
	{
		CDesignResult* pDgnRslt = pDoc->m_pPostCtrl->GetDesignResult();
		Macro::UniqueSort(vWallID);
		CArray<int, int> arWallID; arWallID.RemoveAll();
		for (const auto iWallID : vWallID)
		{
			arWallID.Add(iWallID);
		}
		CArray<T_NODE_K, T_NODE_K> arKeyNode;
		CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
		pDoc->m_pAttrCtrl->GetKeyListWallUser(&arWallID, &arKeyNode, &arKeyElem);
		int nWall = arKeyElem.GetSize();
		for (int i = 0; i < nWall; ++i)
		{
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(arKeyElem[i], nInternalWallID))
			{
				ASSERT(0);
				continue;
			}
			T_RWAL_D MaxRwalD;  MaxRwalD.Initialize();
			if (!pDgnRslt->ReadConcreteDesignResultWall(nInternalWallID, 0, TRUE, 0, MaxRwalD) ||
				MaxRwalD.bCheck == 0)
			{
				continue;
			}
			const auto dMaxRat = [&MaxRwalD]()
			{
				if (LE(MaxRwalD.res_dRatPy, 1.0))
				{
					return MaxRwalD.dblCodeRatio[3];
				}
				else
				{
					return max(MaxRwalD.res_dRatPy, MaxRwalD.dblCodeRatio[3]);
				}
			}();
			const auto dPos = MaxRwalD.nMaximumPosition==0 ? 0.0 : 1.0;
			pRptCase->mRsltData[arKeyElem[i]] = std::make_pair(dMaxRat, dPos);
		}
	}

	std::vector<CBindingData> vBindingData;
	dgn::def::CBindingData Data;
	Data.pData = pRptCase;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRptEnvironment(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//query에 물어보면 부재별로 어떤 info를 출력하는지 알려줘, 근데 난 아니깐 그냥 넘겨줄꼐.

	std::vector<CBindingData> vBindingData;

	auto* pRptEnv = new CDgnRptEnvironment;
	pRptEnv->Lang = enLanguage::eng;

	dgn::def::CBindingData Data;
	Data.pData = pRptEnv;
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLiveLoadReduction(const std::vector<long>& vKeyMemb)
{
#if defined(_CIVIL)
	return true;
#endif // _CIVIL

	// Live Load Reduction Factor
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	DataCtrl.Create_DgnGenRedu(); // 사용자 입력 / 생성정보에 따른 계산 2가지 타입이 존재함. 그래서 만들어 줘야됨..ㅜㅜ

	T_DCTL_D DctlData;
	DctlData.Initialize();
	DataCtrl.Get_DgnGenDctl(DctlData);
	FArray<UINT> aLcase; aLcase.RemoveAll();
	for (int i = 0; i < 5; ++i)
	{
		if (DctlData.KeyStld[i] > 0) aLcase.Add(DctlData.KeyStld[i]);
	}

	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		T_ELEM_D eData;
		eData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetElem(Key, eData))	continue;
		// Change by ZINU.('02.10.22).  Include not only column but also wall.
		bool bWall = pDoc->m_pAttrCtrl->IsWall(eData.eltyp);  // Wall.

		T_REDU_D ReduD;
		if (bWall)
		{
			if (!DataCtrl.Get_DgnWalRedu(Key, ReduD)) continue;
		}
		else
		{
			if (!DataCtrl.Get_DgnGenRedu(Key, ReduD)) continue;
		}
		if (EQ0(ReduD.Factor)) continue;

		auto* pDgnLLRF = new CDgnLLRF;

		pDgnLLRF->dFactor = ReduD.Factor;
		pDgnLLRF->bReduAxl = static_cast<bool>(ReduD.bReduAxl);
		pDgnLLRF->bReduMom = static_cast<bool>(ReduD.bReduMom);
		pDgnLLRF->bReduShr = static_cast<bool>(ReduD.bReduShr);
		pDgnLLRF->aLcasK.RemoveAll();
		if (aLcase.GetSize() > 0) {
			pDgnLLRF->aLcasK.Copy(aLcase);
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnLLRF;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);

		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddScaleUpFactorEQ(const std::vector<long>& vKeyMemb)
{
	// Scale up factor for EQ
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		T_ELEM_D eData;
		eData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetElem(Key, eData))	continue;
		// Change by ZINU.('02.10.22).  Include not only column but also wall.
		bool bWall = pDoc->m_pAttrCtrl->IsWall(eData.eltyp);  // Wall.

		T_SUEQ_D SueqD;
		if (bWall)
		{
			if (!DataCtrl.Get_DgnWalSueq(Key, SueqD)) continue;
		}
		else
		{
			if (!DataCtrl.Get_DgnGenSueq(Key, SueqD)) continue;
		}
		if (EQ0(SueqD.dEqLcaseN + SueqD.dEqLcaseM + SueqD.dEqLcaseV +
			SueqD.dEqLcomN + SueqD.dEqLcomM + SueqD.dEqLcomV)) continue;
		if (EQ0(SueqD.dEqLcaseN)) SueqD.dEqLcaseN = 1.0;
		if (EQ0(SueqD.dEqLcaseM)) SueqD.dEqLcaseM = 1.0;
		if (EQ0(SueqD.dEqLcaseV)) SueqD.dEqLcaseV = 1.0;
		if (EQ0(SueqD.dEqLcomN)) SueqD.dEqLcomN = 1.0;
		if (EQ0(SueqD.dEqLcomM)) SueqD.dEqLcomM = 1.0;
		if (EQ0(SueqD.dEqLcomV)) SueqD.dEqLcomV = 1.0;

		auto* pDgnSUEQ = new CDgnSUEQ;

		pDgnSUEQ->dEqLcaseN = SueqD.dEqLcaseN;
		pDgnSUEQ->dEqLcaseM = SueqD.dEqLcaseM;
		pDgnSUEQ->dEqLcaseV = SueqD.dEqLcaseV;
		pDgnSUEQ->dEqLcomN = SueqD.dEqLcomN;
		pDgnSUEQ->dEqLcomM = SueqD.dEqLcomM;
		pDgnSUEQ->dEqLcomV = SueqD.dEqLcomV;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnSUEQ;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddMomentRedistribution(const std::vector<long>& vKeyMemb)
{
	// Moment redistribution factor
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		T_MRFT_D MrftD;
		if (!DataCtrl.Get_DgnConMrft(Key, MrftD)) continue;
		if (MrftD.dFactor <= 0.0) continue;;
		if (!Util.IsRC(Key)) continue;

		auto* pDgnMRFT = new CMomentRedistributionFactor;

		pDgnMRFT->dFactor = MrftD.dFactor;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnMRFT;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddTorsionRedistribution(const std::vector<long>& vKeyMemb)
{
	// Moment redistribution factor
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key)) continue;
		T_TRFT_D TrftD;
		if (!DataCtrl.Get_DgnConTrft(Key, TrftD)) continue;
		if (TrftD.dFactor <= 0.0) continue;;

		auto* pDgnTRFT = new CDgnTRFT;

		pDgnTRFT->dFactor = TrftD.dFactor;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnTRFT;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddOffset1D(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		T_MEMB_K MembK;
		T_ELEM_K_LIST aElem;
		if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(Key, MembK))
		{
			T_MEMB_D MembD;
			if (pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))
			{
				aElem.Copy(MembD.aElemList);
			}
			else
			{
				aElem.Add(Key);
			}
		}
		else
		{
			aElem.Add(Key);
		}

		int nElem = aElem.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			T_ELEM_K ElemK = aElem[i];

			T_CFMO_D CfmoD;
			if (!pDoc->m_pPostCtrl->GetCfmo(ElemK, CfmoD)) continue;

			auto* pDgnOffset1D = new CDgnOffset1D;

			pDgnOffset1D->nOffsetType = CfmoD.nOffsetType;
			for (int i = 0; i < 6; ++i) {
				pDgnOffset1D->aOffset[i] = CfmoD.dOffset[i];
			}

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, ElemK);
			Data.pData = pDgnOffset1D;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Elem;
			RQuery.vOnlyOneKey.push_back(ElemK);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddMembLength(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	if (m_pDataBase->GetDgnType() == enDgn::RCS && ((IDgnPerformDataBaseRC*)m_pDataBase)->IsMeshed2D())
	{
		std::vector<T_SBDO_K> vSbdo;
		pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
		for (const auto SbdoK : vSbdo)
		{
			T_ELEM_K_LIST aElemK;
			int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElemK);
			if (nElem == 0) continue;

			const double dLen = ((IDgnPerformDataBaseRC*)m_pDataBase)->Get2DMemberLength(SbdoK);

			auto* pMembLength = new CMembLength;
			pMembLength->SetLength(enMembLengthType::Length, dLen);
			pMembLength->SetLength(enMembLengthType::NetLengthY, dLen);
			pMembLength->SetLength(enMembLengthType::NetLengthZ, dLen);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, aElemK[0]);
			Data.pData = pMembLength;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(aElemK[0]);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}
	else
	{
		std::vector<UINT> vAllMemb;
		T_KEY_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			T_HCBM_K HcbmKey(0);
			if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
			{
				T_HCBM_D HcbmD;
				if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
				{
					for (int j = 0; j < 3; ++j)
					{
						vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
					}
				}
				else ASSERT(0);
			}
			else
			{
				T_MEMB_K ShowKey(0);
				if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
				{
					ShowKey = aElemK[i];
				}
				vAllMemb.push_back(ShowKey);
			}
		}
		Macro::UniqueSort(vAllMemb);

		double dLeng = 0.0;
		double dNetLy = 0.0;
		double dNetLz = 0.0;
		for (auto Key : vAllMemb/*vKeyMemb*/)
		{
			const int nMembType = pDoc->m_pAttrCtrl->GetMemberType(Key);
			switch (nMembType)
			{
			case D_MBTP_BEAM:
			case D_MBTP_COLUMN:
			case D_MBTP_BRACE:
				if (!CalcMembLength1D(Key, dLeng, dNetLy, dNetLz)) 
				{ 
					ASSERT(0); 
					dLeng = CMembCtrl::GetMembCtrl()->GetMembLengByIncludeElem(Key);
					dNetLy = dNetLz = 0.0;
				}
				break;
			case D_MBTP_WALL:
				if (!CalcMembLengthWall(Key, dLeng)) { ASSERT(0); continue; }
				break;
			case D_MBTP_ETC:
			case D_MBTP_PLATEBEAM:
			case D_MBTP_PLATECOLM:
			case D_MBTP_SHELL:
				continue;
				break;
			default:
				ASSERT(0);
				break;
			}

			auto* pMembLength = new CMembLength;
			pMembLength->SetLength(enMembLengthType::Length, dLeng);
			pMembLength->SetLength(enMembLengthType::NetLengthY, dNetLy);
			pMembLength->SetLength(enMembLengthType::NetLengthZ, dNetLz);

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, Key);
			Data.pData = pMembLength;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(Key);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSeismicProperty(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnForceCtrl ForceCtrl;
	CDgnDataCtrl DgnDataCtrl;
	const auto DgnType = m_pDataBase->GetDgnType();

	std::vector<CBindingData> vBindingData;
	if(DgnType == enDgn::RCS)
	{
		ForceCtrl.SetDataForSeismic();
	
		T_DCON_D DconD;
		DconD.Initialize();
		DgnDataCtrl.Get_DgnConDcon(DconD);

		std::vector<UINT> vAllMemb;
		T_KEY_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			T_HCBM_K HcbmKey(0);
			if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
			{
				T_HCBM_D HcbmD;
				if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
				{
					for (int j = 0; j < 3; ++j)
					{
						vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
					}
				}
				else ASSERT(0);
			}
			else
			{
				T_MEMB_K ShowKey(0);
				if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
				{
					ShowKey = aElemK[i];
				}
				vAllMemb.push_back(ShowKey);
			}
		}
		Macro::UniqueSort(vAllMemb);

		const auto bMehsed2D = m_pDataBase->GetDgnType() == enDgn::RCS && ((IDgnPerformDataBaseRC*)m_pDataBase)->IsMeshed2D();
		
		for (const auto& iMemb : vAllMemb)
		{
			int nMbtp = DgnDataCtrl.Get_DgnGenMbtp(iMemb);

			auto* pSeismicProperty = new CSeismicProperty;

			pSeismicProperty->bUseUGLcom4UGMemb = DconD.bUseUGLcom4UGMemb ? true : false;
			switch (nMbtp)
			{
			case D_MBTP_ETC:
			case D_MBTP_SHELL:
			case D_MBTP_PLATEBEAM:
			case D_MBTP_PLATECOLM:
				pSeismicProperty->bUseUGLcom4UGMemb = false;
				break;
			case D_MBTP_WALL:
			{
				if (bMehsed2D) break;
				pSeismicProperty->bSystemSeismicProv = DconD.bSpecialSeismic;
				pSeismicProperty->bSeismic = DconD.bSpecialSeismic; // Apply Special Provision for Seismic Design
				if (DconD.bSpecialSeismic)
				{
					unsigned int nInternalWallID = 0;
					if (!pDoc->m_pPostCtrl->GetInternalWallId(iMemb, nInternalWallID)) { continue; }

					pSeismicProperty->bSeismic = ForceCtrl.IsEqSpecialElem(nInternalWallID, true);
				}

				if (pSeismicProperty->bSeismic)
				{

					if (DconD.DesignCode == CONCODE_TWN_USD112)
					{
						pSeismicProperty->FrameType = enSeismicFrameType::_SMF;
					}
					else if(DconD.DesignCode == CONCODE_NTC_DCEC_2023)
					{
						switch(DconD.nClass)   // Seismic System Type (EqGrade)
						{
							case 0:
							pSeismicProperty->FrameType = enSeismicFrameType::_SMF;
							break;
							case 1:
							pSeismicProperty->FrameType = enSeismicFrameType::_IMF;
							break;
							case 2:
							pSeismicProperty->FrameType = enSeismicFrameType::_OMF;
							break;
							default:
							break;
						}
					}
					else
					{
						/// [GEN-4802] 벽체의 Special wall 고려 여부가 match되어야 함. 변수를 추가할지는 승준과 추가 협의해 보고. (변수를 추가하면 다른 함수들도 처리 필요)
						pSeismicProperty->FrameType = DconD.nSpecialWall == 1 ? enSeismicFrameType::_SMF : enSeismicFrameType::_OMF;
					}
				}
				else
				{
					pSeismicProperty->FrameType = enSeismicFrameType::None;
				}

				pSeismicProperty->BEMethod = [&DconD]()
				{
					if (DconD.nSpecialWall)
					{
						switch (DconD.iBndrElemMethod)
						{
						case EN_WALL_BE_DBM: return enBoundaryElemMethod::Displacement;
						case EN_WALL_BE_SBM: return enBoundaryElemMethod::Stress;
						default: ASSERT(0); return enBoundaryElemMethod::Number;
						}
					}

					return enBoundaryElemMethod::Number;
				} ();
				pSeismicProperty->dDeflAmplFactor = DconD.dCd;
				pSeismicProperty->dImportantFactor = DconD.dIe;
			}
			break;
			case D_MBTP_BEAM:
			case D_MBTP_COLUMN:
			case D_MBTP_BRACE:
			{
				if (bMehsed2D) break;

				T_MATD_D MatdD;
				if (!DgnDataCtrl.Get_DgnGenMatd(iMemb, MatdD))
				{
					continue; // User
				}
				if (MatdD.Type != _T("C")) continue;

				pSeismicProperty->bSeismic = DconD.bSpecialSeismic; // Apply Special Provision for Seismic Design
				pSeismicProperty->bSystemSeismicProv = DconD.bSpecialSeismic;
				if (DconD.bSpecialSeismic)
				{
					ForceCtrl.SetMemberDataForSeismic(iMemb);

					T_SDGN_D SdgnD;
					if (pDoc->m_pAttrCtrl->GetSdgn(iMemb, SdgnD))
					{
						switch (SdgnD.nType)
						{
						case 0:
							ForceCtrl.m_arSeismicElemList.SetAt(iMemb, TRUE);
							break;
						case 1:
							ForceCtrl.m_arSecondaryElemList.SetAt(iMemb, TRUE);
							break;
						case 2:
							ForceCtrl.m_arNotSFRSElemList.SetAt(iMemb, TRUE);
							break;
						default:
							ASSERT(0);
							break;
						}
					}

					pSeismicProperty->bSeismic = ForceCtrl.IsEqSpecialElem(iMemb);

					if (DconD.DesignCode == CONCODE_TWN_USD112)
					{
						pSeismicProperty->FrameType = enSeismicFrameType::_SMF;
						if (pSeismicProperty->bSeismic && SdgnD.nType == 2) pSeismicProperty->bNonSeisRes = true;
						pSeismicProperty->bSCWBUse_k1 = DconD.bNotConsiderk1 ? false : true;
					}
					else if ( DconD.DesignCode == CONCODE_AASHTO_LRFD20 || DconD.DesignCode == CONCODE_AASHTO_LRFD24)
					{
						pSeismicProperty->nZone = DconD.nClass;
						pSeismicProperty->FrameType = enSeismicFrameType::None; // Civil에 이런게 없는데 우선 이걸로...
					}
					else
					{
						pSeismicProperty->bSCWBUse_k1 = false;
						switch (DconD.nClass)   // Seismic System Type (EqGrade)
						{
						case 0:
							pSeismicProperty->FrameType = enSeismicFrameType::_SMF;
							if (pSeismicProperty->bSeismic && SdgnD.nType == 2) pSeismicProperty->bNonSeisRes = true;
							break;
						case 1:
							pSeismicProperty->FrameType = enSeismicFrameType::_IMF;
							break;
						case 2:
							pSeismicProperty->FrameType = enSeismicFrameType::_OMF;
							break;
						default:
							break;
						}
					}
				}
				else
				{
					pSeismicProperty->FrameType = enSeismicFrameType::None;
				}

				switch (DconD.iAlphaType)// Ve Method Type
				{
				case 0:
					pSeismicProperty->VeCalcType = enSeismicVeMethod::Max;
					break;
				case 1:
					pSeismicProperty->VeCalcType = enSeismicVeMethod::Min;
					break;
				case 2:
					pSeismicProperty->VeCalcType = enSeismicVeMethod::Ve1;
					break;
				case 3:
					pSeismicProperty->VeCalcType = enSeismicVeMethod::Ve2;
					break;
				default:
					break;
				}

				pSeismicProperty->aVeFactor[0] = DconD.dAlpha1;
				pSeismicProperty->aVeFactor[1] = DconD.dAlpha2;
				pSeismicProperty->aVeFactor[2] = CDgnCodeCtrl::IsConCodeUseAlpha2ColmSSWB(DconD.DesignCode) ? DconD.dAlpha2_Colm : DconD.dAlpha2;

				pSeismicProperty->dVcReductionFactor = DconD.dConShearStrengthRatio;

				pSeismicProperty->bSCWBCheckLastFloor = DconD.bStrongColmWeakBeam;
				pSeismicProperty->bSCWBNorminalStrn = DconD.nSCWBDgnMethod == 1 ? true : false;
				if (DconD.DesignCode == CONCODE_TWN_USD112 && pSeismicProperty->bSCWBNorminalStrn)
				{
					// set phic for nominal
					pSeismicProperty->dPhic = DconD.dSCWB_Phic;
				}
				else
				{
					pSeismicProperty->dPhic = 0.0;
				}

				pSeismicProperty->bBCJoint = (pSeismicProperty->bSeismic && DconD.bBCJointDgn) ? true : false;
				pSeismicProperty->nBCJointPos = DconD.nCheckPos;

				if(DconD.DesignCode == CONCODE_NTC_DCEC_2023)
				{
					pSeismicProperty->bOMFProbable = true;
				}
			}
			break;
			default:
				ASSERT(0);
				break;
			}

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, iMemb);
			Data.pData = pSeismicProperty;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(iMemb);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}
	else if(DgnType == enDgn::STL)
	{
		ForceCtrl.SetDataForSeismic4Steel();

		T_DSTL_D DstlD;
		DstlD.Initialize();
		DgnDataCtrl.Get_DgnStlDstl(DstlD);

		std::vector<UINT> vAllMemb;
		T_KEY_LIST aElemK;
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			T_MATD_D MatdD;
			if (!DgnDataCtrl.Get_DgnGenMatd(aElemK[i], MatdD))
			{
				continue; // User
			}
			if (MatdD.Type != _T("S")) continue;

			T_HCBM_K HcbmKey(0);
			if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
			{
				T_HCBM_D HcbmD;
				if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
				{
					for (int j = 0; j < 3; ++j)
					{
						vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
					}
				}
				else ASSERT(0);
			}
			else
			{
				T_MEMB_K ShowKey(0);
				if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
				{
					ShowKey = aElemK[i];
				}
				vAllMemb.push_back(ShowKey);
			}
		}
		Macro::UniqueSort(vAllMemb);

		//DstlD.bSpecialSeismic = TRUE; // Test code
		//DstlD.iFrameType = 1;

		for (const auto& iMemb : vAllMemb)
		{
			int nMbtp = DgnDataCtrl.Get_DgnGenMbtp(iMemb);

			auto* pSeismicProperty = new CSeismicProperty;

			switch (nMbtp)
			{
			case D_MBTP_ETC:
			case D_MBTP_SHELL:
			case D_MBTP_PLATEBEAM:
			case D_MBTP_PLATECOLM:
			case D_MBTP_WALL:
				break;
			case D_MBTP_BEAM:
			case D_MBTP_COLUMN:
			case D_MBTP_BRACE:
			{				
				if (DstlD.bSpecialSeismic)
				{
					ForceCtrl.SetMemberDataForSeismic(iMemb);

					T_SDGN_D SdgnD;
					if (pDoc->m_pAttrCtrl->GetSdgn(iMemb, SdgnD))
					{
						switch (SdgnD.nType)
						{
						case 0:
							ForceCtrl.m_arSeismicElemList.SetAt(iMemb, TRUE);
							break;
						case 1:
							ForceCtrl.m_arSecondaryElemList.SetAt(iMemb, TRUE);
							break;
						case 2:
							ForceCtrl.m_arNotSFRSElemList.SetAt(iMemb, TRUE);
							break;
						default:
							ASSERT(0);
							break;
						}
					}

					//pSeismicProperty->bSeismic = true;//ForceCtrl.IsEqSpecialElem(iMemb);

					if (DstlD.DesignCode == STLCODE_EC3_05 || DstlD.DesignCode == STLCODE_EC3_2_05)
					{
						pSeismicProperty->dq = DstlD.dq;
						pSeismicProperty->dgamma_ov = DstlD.dgamma_ov;
						pSeismicProperty->enFrameTypeStl = [](const int& iFrameType)
						{
							switch (iFrameType)
							{
							case 0: return enSeismicFrameTypeStl::MomentFrame;
							case 1: return enSeismicFrameTypeStl::BracedFrame;
							default:
								break;
							}
							return enSeismicFrameTypeStl::MomentFrame;
						}(DstlD.iFrameType);
					}
				}
			}
			break;
			default:
				ASSERT(0);
				break;
			}

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, iMemb);
			Data.pData = pSeismicProperty;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(iMemb);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddTorsionDesign(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	T_DCON_D DconD;
	DconD.Initialize();
	DataCtrl.Get_DgnConDcon(DconD);

	const int nStlCode = [&]()
	{
		T_DSTL_D DstlD;
		DstlD.Initialize();
		DataCtrl.Get_DgnStlDstl(DstlD);

		return CDgnCodeCtrl::GetStlCodeID(DstlD.DesignCode);
	}();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (const auto& iMemb : vKeyMemb)
	{
		if (Util.IsRC(iMemb))
		{
			auto* pTorsionDesign = new CTorsionDesign;

#if defined _CIVIL
			int nRCCode = CDgnCodeCtrl::GetConCodeID(DconD.DesignCode);
			pTorsionDesign->bTorsionDesign = CDgnCodeCtrl::IsConCodeTorsionCheck(nRCCode) ? true : false;
#else
			pTorsionDesign->bTorsionDesign = DconD.bTorsionDesign ? true : false;
#endif

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, iMemb);
			Data.pData = pTorsionDesign;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(iMemb);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
		else if (Util.IsSTL(iMemb))
		{
			auto* pTorsionDesign = new CTorsionDesign;

			switch (nStlCode)
			{
			case AISC_LRFD2K:
			case AISC_LRFD05:
			case AISC_ASD05:
			case AISC_LRFD10:
			case AISC_ASD10:
			case AISC_LRFD16:
			case AISC_ASD16:
			case AISC_LRFD22:
			case AISC_ASD22:
			case NSCP_2015_S_LRFD:
			case NSCP_2015_S_ASD:
			case KSSC_LSD09:
			case KSSC_LSD16:
			case KDS_41_31_2019:
			case KDS_41_30_10_2022:
			case KDS_24_14_31_2018:
			case Eurocode3:
			case Eurocode3_05:
			case Eurocode3_1_3_06:
			case Eurocode3_2_05:
			{
				pTorsionDesign->bTorsionDesign = true;
				break;
			}
			default:
				pTorsionDesign->bTorsionDesign = false;
				continue;
			}

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, iMemb);
			Data.pData = pTorsionDesign;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Memb;
			RQuery.vOnlyOneKey.push_back(iMemb);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddCheckDirection(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	T_DCRB_D DcrbD;
	DcrbD.Initialize();
	DataCtrl.Get_DgnConDcrb(DcrbD);

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (const auto& iMemb : vKeyMemb)
	{
		if (!Util.IsRCWall(iMemb)) continue;

		auto* pDgnCheckDirection = new CDgnCheckDirection;

		pDgnCheckDirection->nCheckType = DcrbD.bOutofPlaneBending ? CDgnCheckDirection::Both : CDgnCheckDirection::Major;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pDgnCheckDirection;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSpcVerLcom(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	FTArray<FStr> aLoad;
	FTArray<double> aFactor;

	std::vector<CBindingData> vBindingData;

	bool bExistSLCF = false;
	T_SLCF_D SlcfD;
	if (pDoc->m_pAttrCtrl->GetSlcf(SlcfD))
	{
		bExistSLCF = true;
		const auto nLoad = SlcfD.aLoadPattern.GetSize();
		if (nLoad > 0)
		{
			aLoad.SetSize(nLoad);
			aFactor.SetSize(nLoad);
			for (int i = 0; i < nLoad; ++i)
			{
				const T_SOSF_BASE& SosfBase = SlcfD.aLoadPattern.GetAt(i);

				switch (SosfBase.nLoadCaseType)
				{
				case 0: // Static Load Case.
				{
					T_STLD_D StldD;
					if (!pDoc->m_pAttrCtrl->GetStld(SosfBase.LoadCaseKey, StldD)) { ASSERT(0); continue; }
					CString strLcase;
					strLcase.Format(_T("%s(ST)"), StldD.LoadCaseName);
					aLoad[i] = CT2W(strLcase);
					aFactor[i] = SosfBase.dScaleFactor;
				}
				break;
				case 1: // Response Spectrum.
				{
					T_SPLC_D SplcD;
					if (!pDoc->m_pAttrCtrl->GetSplc(SosfBase.LoadCaseKey, SplcD)) { ASSERT(0); continue; }
					CString strLcase;
					strLcase.Format(_T("%s(RS)"), SplcD.LoadCaseName);
					aLoad[i] = CT2W(strLcase);
					aFactor[i] = SosfBase.dScaleFactor;
				}
				break;
				default:
					ASSERT(0);
					break;
				}
			}
		}
	}
	else
	{
		SlcfD.Initialize();
	}

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CDgnForceCtrl ForceCtrl;
	ForceCtrl.SetSlcmTypeMap();

	for (const auto& iMemb : vKeyMemb)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(iMemb, ElemD)) {
			ASSERT(0); continue;
		}

		// Frame and Wall element
		if (!(pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) || pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)))
		{
			continue;
		}

		auto* pSpcVerLcom = new CDgnSpcVerLcom;
		const int iSlcmType = ForceCtrl.GetSlcmType(iMemb);
		if(iSlcmType == 0)      pSpcVerLcom->enLcomType = enSpecialLcom;
		else if(iSlcmType == 1) pSpcVerLcom->enLcomType = enVerticalLcom;
		else                    pSpcVerLcom->enLcomType = enNonSpcLcom;

		if(bExistSLCF)
		{
			pSpcVerLcom->bSeimicLoadFactor = true;
			pSpcVerLcom->dDLoadFactor = SlcfD.dDLoadFactor;
			pSpcVerLcom->dSDS         = SlcfD.dSDS;
			pSpcVerLcom->dVLoadFactor = SlcfD.dVLoadFactor;
			if(aLoad.GetSize() > 0)
			{
				pSpcVerLcom->aLoadPartternSrt    = aLoad;
				pSpcVerLcom->aLoadPartternFactor = aFactor;
			}
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pSpcVerLcom;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if(CDBLib::IsCodeMeshSlabLcomSpecVert())
	{
		/// for MESHED-SLAB.
		CMap<UINT, UINT, int, int> mSlabSlcmType;
		POSITION PosSsla = pDoc->m_pAttrCtrl2->GetStartSsla();
		while (PosSsla)
		{
			T_SSLA_K SslaK = 0;
			T_SSLA_D SslaD;

			SslaD.Initialize();
			pDoc->m_pAttrCtrl2->GetNextSsla(PosSsla, SslaK, SslaD);

			T_SSLC_D SslcD;
			if (!pDoc->m_pAttrCtrl2->GetSslc(SslaD.SslcK, SslcD)) { continue; }

			if (SslcD.nMembType == 0) // column strip에 대한 정보는 strip 설계 시에 설정함.
			{
				mSlabSlcmType.SetAt(SslaK, SslcD.nSeisType);
			}
		}

		if(mSlabSlcmType.GetCount() > 0)
		{
			// sbdo
			std::vector<T_SBDO_K> vSbdo;
			pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
			const auto nSbdo = vSbdo.size();
			for (int i = 0; i < nSbdo; ++i)
			{
				T_SBDO_K SbdoK = vSbdo[i];
				T_ELEM_K_LIST aSbdoElem;
				int nSbdoElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
				if (nSbdoElem == 0) continue;

				int nSlcmType = 0;
				if (!mSlabSlcmType.Lookup(aSbdoElem[0], nSlcmType))
				{
					nSlcmType = -1;
				}
				for(int j=1; j<nSbdoElem; ++j)
				{
					int nElemSlcmType = 0;
					if (!mSlabSlcmType.Lookup(aSbdoElem[j], nElemSlcmType))
					{
						nElemSlcmType = -1;
					}
					if( nSlcmType != nElemSlcmType)
					{
						nSlcmType = -2;
						break;
					}
				}

				auto* pSpcVerLcom = new CDgnSpcVerLcom;
				pSpcVerLcom->enLcomType = [&nSlcmType]()
				{
					switch (nSlcmType)
					{
						case -2: return enNoneLcom;
						case -1: return enNonSpcLcom;
						case  0: return enSpecialLcom;
						case  1: return enVerticalLcom;
					default:
						break;
					}

					return enNonSpcLcom;
				}();

				CBindingData Data;
				Data.ForceKey = std::make_pair(true, aSbdoElem[0]);
				Data.pData = pSpcVerLcom;
				CRelationalQuery RQuery;
				RQuery.DataType = DataType::Memb;
				RQuery.vOnlyOneKey.push_back(aSbdoElem[0]);
				Data.vQueryInfo.push_back(RQuery);
				vBindingData.push_back(Data);
			}
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddUnderLcomType(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;
	for (const auto& iMemb : vKeyMemb)
	{
		T_ULCT_D UlctD;
		if (!pDoc->m_pAttrCtrl->GetUlct(iMemb, UlctD)) continue;

		auto* pUnderLcomType = new CDgnUnderLcomType;

		if (UlctD.nType == 0) pUnderLcomType->enUnderLcomType = enUnderGroundLcom;
		else if (UlctD.nType == 1) pUnderLcomType->enUnderLcomType = enNoneUnderGroundLcom;
		else {
			ASSERT(0);
			pUnderLcomType->enUnderLcomType = enNoneUnderGroundLcom;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pUnderLcomType;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddCheckOption(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	const bool bUseEquivalentRebar_KDS18 = [&]()
	{
		if (nRCCode != KDS_41_30_2018) return false;
		const CString& strEnv = CProduct::GetTestEnvValue(_ULS(UseEqRebarKDS18));
		return (strEnv == _ULS(yes) || strEnv == _T("")) ? true : false;
	}();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		const bool bRC = Util.IsRC(Key);
		const bool bBeam = Util.IsBeam(Key);
		const bool bColumn = Util.IsColumn(Key);
		const bool bPlate = Util.IsPlate(Key);
		if (!bRC || !(bBeam || bColumn || bPlate)) continue;
		const auto MembK = bPlate ? CMembCtrl::GetMembCtrl()->GetRepElem(Key) : Key;

		auto* pCheckOption = new CCheckOption;

		// 복근보 검정 옵션
		pCheckOption->bConsiderCompRebar = [&] ()
		{
			switch ( nRCCode )
			{
			case Eurocode2_2_05:
			case BS5400_4_90:
			case TMH07_89:
				return DConD.nBeamDgn == 1 ? true : false;
			default:
				return bBeam ? true : false;
			}
		}();
		pCheckOption->bUseEquivalentRebar = [&]()
		{
			if (nRCCode == KDS_41_30_2018)
			{
				return bUseEquivalentRebar_KDS18;
			}

			if (bPlate)
			{
				return true; // 복근 고려 안하고 무조건 1단 배근이므로
			}
			else
			{
				T_MCMB_D McmbD;
				if (pDoc->m_pAttrCtrl->GetMcmb(Key, McmbD))
				{
					return McmbD.nCalcMethod == EN_MCMB_EQUI ? true : false;
				}
				else
				{
					return DConD.nBeamRebarConsiderOpt == EN_MCMB_EQUI ? true : false;
				}
			}
		}();

		// interaction shear
		pCheckOption->bIneractionShear = DConD.bConsiderInteractShear;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pCheckOption;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaBeam(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key) || !Util.IsBeam(Key)) continue;

		T_DCRB_D DcrbD;
		if (!DataCtrl.Get_DgnConDcrb(Key, DcrbD)) continue;

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(Key, MatdD))
		{
			continue; // User
		}

		auto* pDgnCriteria = new CDesignCriteriaBeam;

		// 복근보 옵션
		pDgnCriteria->bDoubly = [&Util, &pDoc, &DcrbD](const long Key)
		{
			T_ELEM_D ElemD;
			if (!pDoc->m_pAttrCtrl->GetElem(Key, ElemD))
			{
				ASSERT(0); return (DcrbD.bDoublyBeam == TRUE) ? true : false;
			}

			T_SECT_D SectD;
			if (!pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD, FALSE))
			{
				ASSERT(0); return (DcrbD.bDoublyBeam == TRUE) ? true : false;
			}

			if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T)
			{
				// Gen 에서 T형은 복근보 산정 안함
				return false;
			}

			return (DcrbD.bDoublyBeam == TRUE) ? true : false;
		} (Key);

		pDgnCriteria->dk = pDgnCriteria->bDoubly ? DcrbD.dDoublyBeam : 1.0;

		TDia DiaD;
		for (int i = 0; i < 5; ++i)
		{
			if (DcrbD.MainRebarBeam[i].IsEmpty()) continue;
			DiaD = CreateRebarDia(DcrbD.MainRebarBeam[i], MatdD.MainRebarData);
			pDgnCriteria->vMainRebar.push_back(DiaD);
		}

		DiaD = CreateRebarDia(DcrbD.StirrupBarBeam, MatdD.SubRebarData);
		pDgnCriteria->vStirrups.push_back(DiaD);

		pDgnCriteria->nLegs = DcrbD.iSubRebarNum_Beam;
		pDgnCriteria->nMaxLayer = 2; // gen 고정
		pDgnCriteria->vSideBar.reserve(1);
		pDgnCriteria->vSideBar.push_back(CreateRebarDia(DcrbD.SideBarBeam, MatdD.SubRebarData));

		double dCover = 0.0;
		if (EQ0(DcrbD.dT_Beam) || EQ0(DcrbD.dB_Beam))
		{
			const double dH = [&]()
			{
				const double dMaxHc = DataCtrl.GetHcbmMaxHc(Key);
				if (MT0(dMaxHc)) return dMaxHc;

				T_SECT_D SectD;
				SectD.Initialize();
				DataCtrl.Get_DgnStlSect(Key, SectD);

				return SectD.SectBefore.SectI.Size[0];
			}();

			dCover = min(max(dH / 10.0, 0.0635), 0.0762);
		}

		if (UQ0(DcrbD.dT_Beam))
			pDgnCriteria->dTop = DcrbD.dT_Beam;
		else
			pDgnCriteria->dTop = dCover;

		if (UQ0(DcrbD.dB_Beam))
			pDgnCriteria->dBot = DcrbD.dB_Beam;
		else
			pDgnCriteria->dBot = dCover;

		switch (DcrbD.iSpliceBeam)
		{
		case 0:
		{
			TDgnCriteriaSpliceType Splice;
			Splice.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Beam, dgn::def::enSpliceType::None);
			pDgnCriteria->enSpliceType = Splice;
		}
		break;
		case 1:
		{
			TDgnCriteriaSpliceType Splice;
			Splice.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Beam, dgn::def::enSpliceType::SplicedBar50);
			pDgnCriteria->enSpliceType = Splice;
		}
		break;
		case 2:
		{
			TDgnCriteriaSpliceType Splice;
			Splice.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Beam, dgn::def::enSpliceType::SplicedBar100);
			pDgnCriteria->enSpliceType = Splice;
		}
		break;
		default:
		{
			TDgnCriteriaSpliceType Splice;
			Splice.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Beam, dgn::def::enSpliceType::None);
			pDgnCriteria->enSpliceType = Splice;
		}
		break;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaColumn(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRCColumn(Key)) continue;

		T_DCRB_D DcrbD;
		if (!DataCtrl.Get_DgnConDcrb(Key, DcrbD)) continue;

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(Key, MatdD))
		{
			continue; // User
		}

		auto* pDgnCriteria = new CDesignCriteriaColumn;

		TDia DiaD;
		for (int i = 0; i < 5; ++i)
		{
			if (DcrbD.MainRebarColumn[i].IsEmpty()) continue;

			DiaD = CreateRebarDia(DcrbD.MainRebarColumn[i], MatdD.MainRebarData);
			pDgnCriteria->MainDia.vMainRebar.push_back(dgn::def::RebarBySection(0.0, DiaD));
		}

		DiaD = CreateRebarDia(DcrbD.StirrupBarColumn, MatdD.SubRebarData);
		pDgnCriteria->vStirrups.push_back(DiaD);

		pDgnCriteria->anLegNum[0] = DcrbD.iSubRebarNum_Column[0];
		pDgnCriteria->anLegNum[1] = DcrbD.iSubRebarNum_Column[1];

		double dCover = 0.0;
		if (EQ0(DcrbD.d0_Column))
		{
			T_SECT_D SectD;
			SectD.Initialize();
			DataCtrl.Get_DgnStlSect(Key, SectD);

			const double dB = SectD.SectBefore.SectI.Size[1];
			const double dH = SectD.SectBefore.SectI.Size[0];
			pDgnCriteria->dCover = min(max(max(dH / 10.0, dB / 10.0), 0.0635), 0.0762);
		}
		else
			pDgnCriteria->dCover = DcrbD.d0_Column;

		switch (DcrbD.iSpliceColumn)
		{
		case 0:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Column, enSpliceType::None);
			break;
		case 1:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Column, enSpliceType::SplicedBar50);
			break;
		case 2:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Column, enSpliceType::SplicedBar100);
			break;
		default:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Column, enSpliceType::None);
			break;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	AddDesignCriteriaBrace(vKeyMemb);

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaBrace(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRCBrace(Key)) continue;

		T_DCRB_D DcrbD;
		if (!DataCtrl.Get_DgnConDcrb(Key, DcrbD)) continue;

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(Key, MatdD))
		{
			continue; // User
		}

		auto* pDgnCriteria = new CDesignCriteriaColumn;

		TDia DiaD;
		for (int i = 0; i < 5; ++i)
		{
			if (DcrbD.MainRebarBrace[i].IsEmpty()) continue;

			DiaD = CreateRebarDia(DcrbD.MainRebarBrace[i], MatdD.MainRebarData);
			pDgnCriteria->MainDia.vMainRebar.push_back(dgn::def::RebarBySection(0.0, DiaD));
		}

		DiaD = CreateRebarDia(DcrbD.StirrupBarBrace, MatdD.SubRebarData);
		pDgnCriteria->vStirrups.push_back(DiaD);

		pDgnCriteria->anLegNum[0] = DcrbD.iSubRebarNum_Brace[0];
		pDgnCriteria->anLegNum[1] = DcrbD.iSubRebarNum_Brace[1];

		double dCover = 0.0;
		if (EQ0(DcrbD.d0_Brace))
		{
			T_SECT_D SectD;
			SectD.Initialize();
			DataCtrl.Get_DgnStlSect(Key, SectD);

			const double dB = SectD.SectBefore.SectI.Size[1];
			const double dH = SectD.SectBefore.SectI.Size[0];
			pDgnCriteria->dCover = min(max(max(dH / 10.0, dB / 10.0), 0.0635), 0.0762);
		}
		else
			pDgnCriteria->dCover = DcrbD.d0_Brace;

		switch (DcrbD.iSpliceBrace)
		{
		case 0:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Brace, enSpliceType::None);
			break;
		case 1:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Brace, enSpliceType::SplicedBar50);
			break;
		case 2:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Brace, enSpliceType::SplicedBar100);
			break;
		default:
			pDgnCriteria->enSpliceType.enSpliceType = std::make_pair(DcrbD.bCheckRebarSpacing_Brace, enSpliceType::None);
			break;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaWall(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	if (CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRCWall(Key)) continue;

		T_MEMB_K ShowK = 0;
		unsigned int nInternalWallID = 0;
		if (!pDoc->m_pPostCtrl->GetInternalWallId(Key, nInternalWallID))
		{
			ASSERT(0);
			continue;
		}

		T_WALL_K WallK;
		if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
		T_WALL_D WallD;
		if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD)) continue;
		ShowK = WallD.RootElemKey;
		
		T_DCRB_D DcrbD;
		if (!DataCtrl.Get_DgnWalDcrb(WallK.keymap, DcrbD)) continue;

		auto l_GetfyByDia = [&DcrbD](const CString& strDia, const double& dfy, const bool& bVertical)
		{
			if (!DcrbD.bMatlByDia) return dfy;
			if (bVertical)
			{
				int nVertical = DcrbD.aVertMatlByDia.GetSize();
				for (int i = 0; i < nVertical; ++i)
				{
					if (strDia == DcrbD.aVertMatlByDia[i].strDia)
					{
						return DcrbD.aVertMatlByDia[i].dFy;
					}
				}
			}
			else
			{
				int nHorizontal = DcrbD.aHorzMatlByDia.GetSize();
				for (int i = 0; i < nHorizontal; ++i)
				{
					if (strDia == DcrbD.aHorzMatlByDia[i].strDia)
					{
						return DcrbD.aHorzMatlByDia[i].dFy;
					}
				}
			}

			return dfy;
		};

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(Key, MatdD))
		{
			continue; // User
		}

		auto* pDgnCriteria = new CDesignCriteriaWall;

		TDia DiaD;
		for (int i = 0; i < 5; ++i)
		{
			if (DcrbD.VerticalRebarWall[i].IsEmpty()) continue;

			DiaD.strName = CT2W(DcrbD.VerticalRebarWall[i]);
			DiaD.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(DcrbD.VerticalRebarWall[i]);
			DiaD.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(DcrbD.VerticalRebarWall[i]);
			DiaD.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(DcrbD.VerticalRebarWall[i]);
			DiaD.Geom.dPerimeter = CMathFunc::m_pi * DiaD.Geom.dDia;
			DiaD.Matl.dfy = l_GetfyByDia(DcrbD.VerticalRebarWall[i], MatdD.MainRebarData.B_fy, true);
			DiaD.Matl.dEs = Get_Es(nRCCode, DiaD.Matl.dfy);

			pDgnCriteria->vVerticalRebar.push_back(DiaD);
		}

		for (int i = 0; i < 50; ++i)
		{
			const double dSpace = Get_VerRebarSpaceByString(nRCCode, DcrbD.SpacingVerticalRebar[i]);
			if (EQ0(dSpace)) break;
			pDgnCriteria->vVerticalSpaces.push_back(dSpace);
		}

		DiaD.strName = CT2W(DcrbD.HorizontalRebarWall);
		DiaD.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(DcrbD.HorizontalRebarWall);
		DiaD.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(DcrbD.HorizontalRebarWall);
		DiaD.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(DcrbD.HorizontalRebarWall);
		DiaD.Geom.dPerimeter = CMathFunc::m_pi * DiaD.Geom.dDia;
		DiaD.Matl.dfy = l_GetfyByDia(DcrbD.HorizontalRebarWall, MatdD.SubRebarData.B_fy, false);
		DiaD.Matl.dEs = Get_Es(nRCCode, DiaD.Matl.dfy);
		pDgnCriteria->vHorizontalRebar.push_back(DiaD);

		const double dIncrease = Get_RebarSpaceIncrease(nRCCode, DConD.nNationalAnnex);
		const double dInitSpace = DcrbD.dSpacingHorizontalRebar;
		const double dSpaceMax = Get_RebarSpaceMax(nRCCode, DConD.nNationalAnnex);

		for (int i = 0; i < 60; ++i)
		{
			const auto dSpace = dInitSpace + (dIncrease * i);
			if (MT(dSpace, dSpaceMax))
			{
				break;
			}
			pDgnCriteria->vHorizontalSpaces.push_back(dSpace);
		}

		// End Rebar From : DcrbD.EndRebarWall
		int nEndBarNo = pDoc->m_pMatlDB->Get_BarSizeNoByBarName(DcrbD.EndRebarWall);
		while (!pDoc->m_pMatlDB->Get_BarSizeNoByMax(nEndBarNo))
		{
			CString strDia = pDoc->m_pMatlDB->Get_BarNameByBarSizeNo(nEndBarNo);
			DiaD.strName = CT2W(strDia);
			DiaD.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(nEndBarNo);
			DiaD.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(nEndBarNo);
			DiaD.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(nEndBarNo);
			DiaD.Geom.dPerimeter = CMathFunc::m_pi * DiaD.Geom.dDia;
			DiaD.Matl.dfy = l_GetfyByDia(strDia, MatdD.MainRebarData.B_fy, true);
			DiaD.Matl.dEs = Get_Es(nRCCode, DiaD.Matl.dfy);
			pDgnCriteria->vEndVerticalRebar.push_back(DiaD);

			nEndBarNo++;
		}

		// boundary element rebar
		pDgnCriteria->BoundaryHorizontalRebar = [this, &pDoc, &DcrbD, &MatdD, &l_GetfyByDia, &nRCCode]()
		{
			TDia DiaD;
			DiaD.strName = CT2W(DcrbD.BEHorizontalRebarWall);
			DiaD.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(DcrbD.BEHorizontalRebarWall);
			DiaD.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(DcrbD.BEHorizontalRebarWall);
			DiaD.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(DcrbD.BEHorizontalRebarWall);
			DiaD.Geom.dPerimeter = CMathFunc::m_pi * DiaD.Geom.dDia;
			DiaD.Matl.dfy = l_GetfyByDia(DcrbD.BEHorizontalRebarWall, MatdD.SubRebarData.B_fy, false);
			DiaD.Matl.dEs = Get_Es(nRCCode, DiaD.Matl.dfy);

			return DiaD;
		} ();

		pDgnCriteria->BoundaryHorizontalSpace = DcrbD.dBEHorizontalRebarSpace;

		// see CRCSDataBase::Calc_WallCoverThick(double dCover)
		pDgnCriteria->de = EQ0(DcrbD.dDe_Wall) ? 0.0508 : DcrbD.dDe_Wall;
		pDgnCriteria->dw = EQ0(DcrbD.dDw_Wall) ? 0.0508 : DcrbD.dDw_Wall;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaSlab(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	const auto nRCCode = [&DataCtrl]()
	{
		T_DCON_D DConD;
		DataCtrl.Get_DgnConDcon(DConD);
		return CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	} ();

	T_MSRB_D MsrbD;
	pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);

	auto l_GetMatlByDia = [&MsrbD](const T_MATD_D& MatdD, const CString& strDia)
	{
		auto RebarD = MatdD.MainRebarData;
		if (MsrbD.bMatlByDia)
		{
			for (int i = 0; i < MsrbD.aMatlByDia.GetSize(); ++i)
			{
				if (strDia.CompareNoCase(MsrbD.aMatlByDia[i].strDia) == 0)
				{
					RebarD.B_fy = MsrbD.aMatlByDia[i].dFy;
					break;
				}
			}
		}
		return RebarD;
	};

	T_ELEM_K_LIST aKeys;
	for (const auto key : vKeyMemb) aKeys.Add(key);

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		if (!Util.IsSlab(aElem[0]))
			continue;

		T_ELEM_K_LIST aIntersect;
		CArrayUtil::IntersectUINT(aKeys, aElem, aIntersect);
		int nIntersect = aIntersect.GetSize();
		if (nIntersect == 0) continue;

		T_ELEM_K_LIST aSubtract;
		CArrayUtil::SubtractUINT(aKeys, aIntersect, aSubtract);

		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		for (int j = 0; j < nIntersect; ++j)
		{
			const auto key = aIntersect[j];

			double dThick = 0.0;

			T_ELEM_D ElemD;
			pDoc->m_pAttrCtrl->GetElem(key, ElemD);

			T_THIK_D ThikD;
			pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);
			pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThick);

			T_MATD_D MatdD;
			if (!DataCtrl.Get_DgnGenMatd(key, MatdD))
			{
				continue; // User
			}

			auto* pDgnCriteria = new CDesignCriteriaSlab;

			auto l_GetCover = [](const double& dCover)
			{
				return (EQ0(dCover)) ? 0.026 : dCover; // default; 26mm
			};

			if(SbdoD.nType == 2) // mat
			{
				for(int i = 0; i < 5; ++i)
				{
					if(MsrbD.RebarMatSize[i].IsEmpty()) continue;

					const auto RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.RebarMatSize[i]);
					const auto DiaD = CreateRebarDia(MsrbD.RebarMatSize[i], RebarMatlD);
					pDgnCriteria->vRebar.push_back(DiaD);
				}

				for(int i = 0; i < 50; ++i)
				{
					const auto dSpace = Get_VerRebarSpaceByString(nRCCode, MsrbD.RebarMatSpace[i]);
					if(EQ0(dSpace)) break;

					pDgnCriteria->vSpaces.push_back(dSpace);
				}

				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir1)] = l_GetCover(MsrbD.dT_Mat_X);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir2)] = l_GetCover(MsrbD.dT_Mat_Y);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir1)] = l_GetCover(MsrbD.dB_Mat_X);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir2)] = l_GetCover(MsrbD.dB_Mat_Y);
			}
			else
			{
				for(int i = 0; i < 5; ++i)
				{
					if(MsrbD.RebarSlabSize[i].IsEmpty()) continue;

					const auto RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.RebarSlabSize[i]);
					const auto DiaD = CreateRebarDia(MsrbD.RebarSlabSize[i], RebarMatlD);
					pDgnCriteria->vRebar.push_back(DiaD);
				}

				for(int i = 0; i < 50; ++i)
				{
					const auto dSpace = Get_VerRebarSpaceByString(nRCCode, MsrbD.RebarSlabSpace[i]);
					if(EQ0(dSpace)) break;

					pDgnCriteria->vSpaces.push_back(dSpace);
				}

				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir1)] = l_GetCover(MsrbD.dT_Slab_X);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir2)] = l_GetCover(MsrbD.dT_Slab_Y);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir1)] = l_GetCover(MsrbD.dB_Slab_X);
				pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir2)] = l_GetCover(MsrbD.dB_Slab_Y);
			}

			if (SbdoD.bBasicRebar)
			{
				pDgnCriteria->BasicRebar.bUseBasicRebar = true;

				auto RebarMatlD = l_GetMatlByDia(MatdD, SbdoD.strTopRebarName_X);
				const auto TDiaDTopX = CreateRebarDia(SbdoD.strTopRebarName_X, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Top_Dir1)] = std::make_pair(TDiaDTopX, SbdoD.dTopRebarSpace_X);

				RebarMatlD = l_GetMatlByDia(MatdD, SbdoD.strTopRebarName_Y);
				const auto TDiaDTopY = CreateRebarDia(SbdoD.strTopRebarName_Y, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Top_Dir2)] = std::make_pair(TDiaDTopY, SbdoD.dTopRebarSpace_Y);

				RebarMatlD = l_GetMatlByDia(MatdD, SbdoD.strBotRebarName_X);
				const auto TDiaDBotX = CreateRebarDia(SbdoD.strBotRebarName_X, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Bot_Dir1)] = std::make_pair(TDiaDBotX, SbdoD.dBotRebarSpace_X);

				RebarMatlD = l_GetMatlByDia(MatdD, SbdoD.strBotRebarName_Y);
				const auto TDiaDBotY = CreateRebarDia(SbdoD.strBotRebarName_Y, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Bot_Dir2)] = std::make_pair(TDiaDBotY, SbdoD.dBotRebarSpace_Y);
			}
			else if (MsrbD.bBasicRebar)
			{
				pDgnCriteria->BasicRebar.bUseBasicRebar = true;

				auto RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.strTopRebarName_X);
				const auto TDiaDTopX = CreateRebarDia(MsrbD.strTopRebarName_X, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Top_Dir1)] = std::make_pair(TDiaDTopX, MsrbD.dTopRebarSpace_X);

				RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.strTopRebarName_Y);
				const auto TDiaDTopY = CreateRebarDia(MsrbD.strTopRebarName_Y, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Top_Dir2)] = std::make_pair(TDiaDTopY, MsrbD.dTopRebarSpace_Y);

				RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.strBotRebarName_X);
				const auto TDiaDBotX = CreateRebarDia(MsrbD.strBotRebarName_X, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Bot_Dir1)] = std::make_pair(TDiaDBotX, MsrbD.dBotRebarSpace_X);

				RebarMatlD = l_GetMatlByDia(MatdD, MsrbD.strBotRebarName_Y);
				const auto TDiaDBotY = CreateRebarDia(MsrbD.strBotRebarName_Y, RebarMatlD);
				pDgnCriteria->BasicRebar.aBasicRebar[Macro::Ordinal(en2DRebarDir::Bot_Dir2)] = std::make_pair(TDiaDBotY, MsrbD.dBotRebarSpace_Y);
			}
			else
			{
				pDgnCriteria->BasicRebar.bUseBasicRebar = false;
			}

			CBindingData Data;
			Data.ForceKey = std::make_pair(true, key);
			Data.pData = pDgnCriteria;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Elem;
			RQuery.vOnlyOneKey.push_back(key);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}

		if (aSubtract.GetSize() == 0) break;
		aKeys.Copy(aSubtract);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaAs(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;

	std::vector<UINT> vRepElem;
	for (auto Key : vKeyMemb)
	{
		if (Util.IsPlate(Key))
		{
			auto RepMembK = CMembCtrl::GetMembCtrl()->GetRepElem(Key);
			vRepElem.push_back(RepMembK);
		}
		else
		{
			vRepElem.push_back(Key);
		}
	}
	Macro::UniqueSort(vRepElem);

	for (auto Key : vRepElem)
	{
		if (!Util.IsRC(Key)) continue;

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(Key, MatdD))
		{
			continue; // User
		}

		T_MATL_REBAR RebarData = MatdD.MainRebarData;
		T_MATL_REBAR RebarStirrupData = MatdD.SubRebarData;

		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(Key, ElemD))
			continue;

		//TODO civil data 구조체를 찾아보자
		auto* pDgnCriteria = new CDesignCriteriaAs;

		if (Util.IsBeam(Key))
		{
			T_RDGN_D RdgnD;
			if ( !pDoc->m_pAttrCtrl->GetRdgn(ElemD.elpro, RdgnD) )
			{
				if ( !((IDgnPerformDataBaseRC*)m_pDataBase)->GetRdgnDefaultValue(enMemb::Beam, Key, RdgnD) )
				{
					ASSERT(0); continue;
				}
			}

			pDgnCriteria->vdCover.reserve(2);
			pDgnCriteria->vdCover.push_back(RdgnD.BEAM.dDt[0]);
			pDgnCriteria->vdCover.push_back(RdgnD.BEAM.dDb[0]);

			pDgnCriteria->MainBar = CreateRebarDia(_T(""), RebarData);
			pDgnCriteria->Stirrup = CreateRebarDia(RdgnD.BEAM.strSubBarNa[0], RebarStirrupData);
			pDgnCriteria->dLeg = RdgnD.BEAM.dSubBarNum[0];
		}
		else if (Util.IsColumn(Key))
		{
			T_RDGN_D RdgnD;
			if ( !pDoc->m_pAttrCtrl->GetRdgn(ElemD.elpro, RdgnD) )
			{
				if ( !((IDgnPerformDataBaseRC*)m_pDataBase)->GetRdgnDefaultValue(enMemb::Column, Key, RdgnD) )
				{
					ASSERT(0); continue;
				}
			}
			
			pDgnCriteria->vdCover.reserve(1);
			pDgnCriteria->vdCover.push_back(RdgnD.COLM.dDc[0]);

			pDgnCriteria->MainBar = CreateRebarDia(_T(""), RebarData);
			pDgnCriteria->Stirrup = CreateRebarDia(RdgnD.COLM.strSubBarNa[0], RebarStirrupData);
			pDgnCriteria->dLeg = RdgnD.COLM.dSubBarNum[0];
		}
		else if (Util.IsRCPlateBeam(Key))
		{
			T_PBDD_D PbddD;
			if (!CMembCtrl::GetMembCtrl()->GetPbddByElem(Key, PbddD))
			{
				_SAFE_DELETE(pDgnCriteria); continue;
			}

			pDgnCriteria->vdCover.reserve(4);
			pDgnCriteria->vdCover.push_back(PbddD.dDt);
			pDgnCriteria->vdCover.push_back(PbddD.dDt);
			pDgnCriteria->vdCover.push_back(PbddD.dDb);
			pDgnCriteria->vdCover.push_back(PbddD.dDb);

			pDgnCriteria->MainBar = CreateRebarDia(_T(""), RebarData);
			pDgnCriteria->Stirrup = CreateRebarDia(PbddD.strBarNa, RebarStirrupData);
			pDgnCriteria->dLeg = PbddD.nNum;
		}
		else if (Util.IsRCPlateColumn(Key))
		{
			T_PCDD_D PcddD;
			if (!CMembCtrl::GetMembCtrl()->GetPcddByElem(Key, PcddD))
			{
				_SAFE_DELETE(pDgnCriteria); continue;
			}

			pDgnCriteria->vdCover.reserve(4);
			pDgnCriteria->vdCover.push_back(PcddD.dDt);
			pDgnCriteria->vdCover.push_back(PcddD.dDt);
			pDgnCriteria->vdCover.push_back(PcddD.dDb);
			pDgnCriteria->vdCover.push_back(PcddD.dDb);

			pDgnCriteria->MainBar = CreateRebarDia(_T(""), RebarData);
			pDgnCriteria->Stirrup = CreateRebarDia(PcddD.strBarNa, RebarStirrupData);
			pDgnCriteria->dLeg = PcddD.nNum;
		}
		else
			ASSERT(0);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignCriteriaPlate(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	const auto nRCCode = [&DataCtrl]()
	{
		T_DCON_D DConD;
		DataCtrl.Get_DgnConDcon(DConD);
		return CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	} ();

	T_MSRB_D MsrbD;
	pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbD);

	T_ELEM_K_LIST aKeys;
	for (const auto key : vKeyMemb) aKeys.Add(key);

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		if (!Util.IsRCShell(aElem[0]))
			continue;

		T_ELEM_K_LIST aIntersect;
		CArrayUtil::IntersectUINT(aKeys, aElem, aIntersect);
		int nIntersect = aIntersect.GetSize();
		if (nIntersect == 0) continue;

		T_ELEM_K_LIST aSubtract;
		CArrayUtil::SubtractUINT(aKeys, aIntersect, aSubtract);

		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		auto key = aElem[0];

		double dThick = 0.0;

		T_ELEM_D ElemD;
		pDoc->m_pAttrCtrl->GetElem(key, ElemD);

		T_THIK_D ThikD;
		pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD);
		pDoc->m_pAttrCtrl->GetThikForDesign(ThikD, dThick);

		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(key, MatdD))
		{
			continue; // User
		}

		auto* pDgnCriteria = new CDesignCriteriaPlate;

		for (int i = 0; i < 5; ++i)
		{
			if (MsrbD.RebarShellSize[i].IsEmpty()) continue;

			const auto DiaD = CreateRebarDia(MsrbD.RebarShellSize[i], MatdD.MainRebarData);
			pDgnCriteria->vRebar.push_back(DiaD);
		}

		for (int i = 0; i < 50; ++i)
		{
			const auto dSpace = Get_VerRebarSpaceByString(nRCCode, MsrbD.RebarShellSpace[i]);
			if (EQ0(dSpace)) break;

			pDgnCriteria->vSpaces.push_back(dSpace);
		}

		auto l_GetCover = [](const double& dCover)
		{
			return (EQ0(dCover)) ? 0.026 : dCover; // default; 26mm
		};

		if (pDgnCriteria->vRebar.size() == 0 || pDgnCriteria->vSpaces.size() == 0)
		{
			_SAFE_DELETE(pDgnCriteria);
			continue;
		}

		pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir1)] = l_GetCover(MsrbD.dT_Shell_X);
		pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Top_Dir2)] = l_GetCover(MsrbD.dT_Shell_Y);
		pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir1)] = l_GetCover(MsrbD.dB_Shell_X);
		pDgnCriteria->aCover[static_cast<int>(en2DRebarDir::Bot_Dir2)] = l_GetCover(MsrbD.dB_Shell_Y);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, key);
		Data.pData = pDgnCriteria;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);

		if (aSubtract.GetSize() == 0) break;
		aKeys.Copy(aSubtract);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSameRebarAtJoint(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_BREQ_K, T_BREQ_K> aBreqK;
	aBreqK.RemoveAll();
	pDoc->m_pAttrCtrl2->GetBreqKeyList(aBreqK);
	const INT_PTR nBreq = aBreqK.GetSize();
	if (nBreq == 0) {
		return false;
	}

	std::vector<CBindingData> vBindingData;
	for (INT_PTR i = 0; i < nBreq; ++i)
	{
		T_BREQ_K BrdqK = aBreqK[i];
		T_BREQ_D BrdqD;
		if (!pDoc->m_pAttrCtrl2->GetBreq(BrdqK, BrdqD)) continue;
		if (BrdqD.bDonotConsider) continue;
		INT_PTR nMemb = BrdqD.aElemList.GetSize();

		auto* pSameRebarJoint = new CDgnSameRebarJoint;
		pSameRebarJoint->NodeK = BrdqD.NodeK;
		for (INT_PTR j = 0; j < nMemb; ++j)
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(BrdqD.aElemList[j], ShowKey)) {
				ShowKey = BrdqD.aElemList[j];
			}

			pSameRebarJoint->vMemb.push_back(ShowKey);
		}

		Macro::UniqueSort(pSameRebarJoint->vMemb);
		if (pSameRebarJoint->vMemb.size() < 2)
		{
			_SAFE_DELETE(pSameRebarJoint);
			continue;
		}

		CBindingData Data;
		Data.pData = pSameRebarJoint;
		Data.ForceKey = std::make_pair(true, BrdqK);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddHaunchedBeam(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_HCBM_K, T_HCBM_K> aHcbmK;
	aHcbmK.RemoveAll();
	pDoc->m_pAttrCtrl->GetHcbmKeyList(aHcbmK);
	const INT_PTR nHcbm = aHcbmK.GetSize();
	if (nHcbm == 0) {
		return false;
	}

	std::vector<CBindingData> vBindingData;
	for (INT_PTR i = 0; i < nHcbm; ++i)
	{
		T_HCBM_K HcbmK = aHcbmK[i];
		T_HCBM_D HcbmD;
		if (!pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD)) continue;
		if (HcbmD.aElemListPart[0].GetSize() == 0) continue;;
		if (HcbmD.aElemListPart[1].GetSize() == 0) continue;;
		if (HcbmD.aElemListPart[2].GetSize() == 0) continue;;

		auto* pHaunchedBeam = new CDgnHaunchedBeam;
		pHaunchedBeam->MembK_PartA = HcbmD.aElemListPart[0].GetAt(0);
		pHaunchedBeam->MembK_PartB = HcbmD.aElemListPart[1].GetAt(0);
		pHaunchedBeam->MembK_PartC = HcbmD.aElemListPart[2].GetAt(0);
		pHaunchedBeam->bEachSect = false;
		if (HcbmD.nDesignPosition == 0) {
			pHaunchedBeam->dL1 = 0.5;
			pHaunchedBeam->dL2 = 0.5;
		}
		else
		{
			// Part A
			double dPartLen = 0.0;
			for (int j = 0; j < HcbmD.aElemListPart[0].GetSize(); ++j)
			{
				dPartLen += pDoc->calcLAVElem(HcbmD.aElemListPart[0].GetAt(j));
			}
			pHaunchedBeam->dL1 = fabs(HcbmD.dL1 / dPartLen);
			pHaunchedBeam->dL1 = min(pHaunchedBeam->dL1, 1.0);

			dPartLen = 0.0;
			for (int j = 0; j < HcbmD.aElemListPart[2].GetSize(); ++j)
			{
				dPartLen += pDoc->calcLAVElem(HcbmD.aElemListPart[2].GetAt(j));
			}
			pHaunchedBeam->dL2 = fabs(HcbmD.dL2 / dPartLen);
			pHaunchedBeam->dL2 = min(pHaunchedBeam->dL2, 1.0);
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, HcbmK);
		Data.pData = pHaunchedBeam;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(pHaunchedBeam->MembK_PartA);
		RQuery.vOnlyOneKey.push_back(pHaunchedBeam->MembK_PartB);
		RQuery.vOnlyOneKey.push_back(pHaunchedBeam->MembK_PartC);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddPMCalcOption(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	/// for plate column in CIVIL.
	CArray<T_SBDO_K, T_SBDO_K> aSbdoList;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoList);
	int nSbdo = aSbdoList.GetSize();
	std::unordered_map<T_ELEM_K, T_SBDO_K> mSbdoElem;
	for (int i = 0; i < nSbdo; ++i)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoList[i], aElem);
		for (int j = 0; j < nElem; ++j)
		{
			mSbdoElem.insert(std::make_pair(aElem[j], aElem[0]));
		}
	}

	std::unordered_map<long, bool> mSbdoDone;

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	T_DCON_D DConD;
	if (!DataCtrl.Get_DgnConDcon(DConD)) DConD.Initialize();

	for (const auto& iMemb : vKeyMemb)
	{	
		int nPMcvMethod = DataCtrl.Get_DgnConPmdm(iMemb);

		auto* pPMCalcOption = new CDgnPMCalcOption;
		switch (nPMcvMethod)
		{
		case 0:
			pPMCalcOption->enOption = CDgnPMCalcOption::ByPu;
			break;
		case 1:
			pPMCalcOption->enOption = CDgnPMCalcOption::ByEccen;
			break;
		default:
			pPMCalcOption->enOption = CDgnPMCalcOption::ByEccen;
			break;
		}
		pPMCalcOption->bReduceRebar = false;
		pPMCalcOption->bReduceRebarPo = true;

		int nMembType = pDoc->m_pAttrCtrl->GetMemberType(iMemb);
#if defined _CIVIL				
		pPMCalcOption->enInteraction = [&] ()
		{			
			if ( nMembType == D_MBTP_PLATECOLM )
			{
				if ( DConD.bChkPM )
					return CDgnPMCalcOption::InteractAxialBending;
			}
			return CDgnPMCalcOption::SperateAxialBending;
		}();

		pPMCalcOption->enDir = [&] ()
		{
			if ( nMembType == D_MBTP_COLUMN || nMembType == D_MBTP_BRACE )
			{
				return DConD.nColumnDgn == 0 ? CDgnPMCalcOption::BiAxial : CDgnPMCalcOption::UniAxial;
			}

			return CDgnPMCalcOption::Auto;
		}();
        pPMCalcOption->enMnOption = CDgnPMCalcOption::MnAtCurve;
#else
		pPMCalcOption->enDir = CDgnPMCalcOption::Auto;
        T_ELEM_D eData;
        eData.Initialize();
        if ( !pDoc->m_pAttrCtrl->GetElem(iMemb, eData) )	continue;
        bool bWall = pDoc->m_pAttrCtrl->IsWall(eData.eltyp);  // Wall.
        if ( bWall )
        {
            pPMCalcOption->enMnOption = CDgnPMCalcOption::MnAtCurve;
        }
        else
        {
            pPMCalcOption->enMnOption = CDgnPMCalcOption::MnAtPnmax;
        }
#endif
		long Key = iMemb;
		if (nMembType == D_MBTP_PLATECOLM)
		{
			const auto itr = mSbdoElem.find(Key);
			if (itr == mSbdoElem.end()) continue;
			Key = itr->second;

			const auto itrDone = mSbdoDone.find(Key);
			if (itrDone != mSbdoDone.end()) continue;
			mSbdoDone[Key] = true;
		}		

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pPMCalcOption;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddMomentMagnifier(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	bool bPDelta = pDoc->m_pPostCtrl->GetPointDataCount()->m_nNumPdel > 0 ? true : false;

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	for (const auto& iMemb : vKeyMemb)
	{
		T_FMAG_D FmagD;
		FmagD.Initialize();
		DataCtrl.Get_DgnGenFmag(iMemb, FmagD);

		auto* pMomentMagnifier = new CMomentMagnifier;
		pMomentMagnifier->bMagnifyMoment = true;
		pMomentMagnifier->bPDelta = bPDelta;

		pMomentMagnifier->aB1[CMomentMagnifier::y].v1 = EQ0(FmagD.B1y);
		pMomentMagnifier->aB1[CMomentMagnifier::z].v1 = EQ0(FmagD.B1z);
		pMomentMagnifier->aB2[CMomentMagnifier::y].v1 = false;
		pMomentMagnifier->aB2[CMomentMagnifier::z].v1 = false;

		pMomentMagnifier->aB1[CMomentMagnifier::y].v2 = FmagD.B1y;
		pMomentMagnifier->aB1[CMomentMagnifier::z].v2 = FmagD.B1z;
		pMomentMagnifier->aB2[CMomentMagnifier::y].v2 = FmagD.B2y;
		pMomentMagnifier->aB2[CMomentMagnifier::z].v2 = FmagD.B2z;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pMomentMagnifier;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddEquivalentMoment(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	for (const auto& iMemb : vKeyMemb)
	{
		T_CMFT_D CmftD;
		CmftD.Initialize();
		DataCtrl.Get_DgnGenCmft(iMemb, CmftD);

		auto* pEquivalentMoment = new CEquivalentMoment;
		pEquivalentMoment->aCm[CEquivalentMoment::y].v1 = CmftD.bAuto;
		pEquivalentMoment->aCm[CEquivalentMoment::z].v1 = CmftD.bAuto;
		pEquivalentMoment->aCm[CEquivalentMoment::y].v2 = CmftD.Cmy;
		pEquivalentMoment->aCm[CEquivalentMoment::z].v2 = CmftD.Cmz;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pEquivalentMoment;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDeflectionParameter(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	T_DCON_D DConD;
	if (!DataCtrl.Get_DgnConDcon(DConD)) DConD.Initialize();

	std::vector<T_STLD_K> vDL, vLL;
	T_STLD_K_LIST aStld;
	pDoc->m_pAttrCtrl->GetStldKeyList(aStld);
	int nStld = aStld.GetSize();
	if (nStld == 0) return true;
	for (int i = 0; i < nStld; ++i)
	{
		T_STLD_K StldK = aStld[i];
		T_STLD_D StldD;
		if (!pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;
		if (CDBLib::IsStldDL(StldD.LoadCaseType)) vDL.push_back(StldK);
		else if (CDBLib::IsStldLL(StldD.LoadCaseType)) vLL.push_back(StldK);
	}

	DataCtrl.m_pMembCtrl->Reset_Serial_ElemList();

	CMembTypeUtil Util;
	for (const auto& iMemb : vKeyMemb)
	{
		if (!Util.IsRCBeam(iMemb)) continue;

		T_SERV_D rData;
		rData.Initialize();
		if (!DataCtrl.Get_DgnConServ(iMemb, rData)) continue;

		auto* pDeflectionParam = new CDgnDeflectionParamPerform;
		pDeflectionParam->bCheck = false;

		if (DConD.bCheckBeamDeflect &&
			(/*DConD.DesignCode == CONCODE_KDS_41_30_2018 || CONCODE_KDS_41_30_2021 ||*/
				DConD.DesignCode == CONCODE_ACI318_19 || DConD.DesignCode == CONCODE_ACI318M_19 ||
				DConD.DesignCode == CONCODE_ACI318_25 || DConD.DesignCode == CONCODE_ACI318M_25 ||
				DConD.DesignCode == CONCODE_TWN_USD112 || DConD.DesignCode == CONCODE_NTC_DCEC_2023))
		{
			pDeflectionParam->bCheck = true;
			pDeflectionParam->adValue[static_cast<int>(enDeflectionParameter::ShortLimit)] = rData.dDefLim;
			pDeflectionParam->adValue[static_cast<int>(enDeflectionParameter::LongLimit)] = rData.dDefLim_long;
			pDeflectionParam->adValue[static_cast<int>(enDeflectionParameter::SustainRatio)] = EQ0(rData.dReductFactor) ? 1.0 : rData.dReductFactor;
			pDeflectionParam->Time_DependentData.SetData([&rData]() {
				switch (rData.iTimeFactor)
				{
				case 0: return TDeflectionTimeFactor::enOption::Over_Year_5;
				case 1: return TDeflectionTimeFactor::enOption::Month_12;
				case 2: return TDeflectionTimeFactor::enOption::Month_06;
				case 3: return TDeflectionTimeFactor::enOption::Month_03;
				case 4: return TDeflectionTimeFactor::enOption::User;
				default:
					ASSERT(0);
					break;
				}
				return TDeflectionTimeFactor::enOption::Month_03;
			}(), rData.dXi);

			auto l_getStaticForce = [&](const std::vector<T_STLD_K>& vStld, double& dMax, double& dMin) {
				dMax = 0.0;
				dMin = 0.0;
				if (vStld.size() == 0) return;

				T_STRB_D StrbD;
				StrbD.Initialize();
				for (const auto& StldK : vStld)
				{
					T_STRB_D StrbMax; StrbMax.Initialize();
					T_STRB_D StrbMin; StrbMin.Initialize();
					double dMaxDL(0.0), dMinDL(0.0);
					if (!DataCtrl.m_pMembCtrl->GetStrbDgn(iMemb, D_LOADCASE_STATIC, StldK, &StrbMax, &StrbMin)) // code unit data
					{
						ASSERT(0);
						continue;
					}
					StrbD.Add(StrbMax);
				}
				for (int i = 0; i < 4; ++i)
				{
					dMax = max(dMax, StrbD.dblForce[i][4]);
					dMax = max(dMax, StrbD.dblForce[i][10]);
					dMin = min(dMin, StrbD.dblForce[i][4]);
					dMin = min(dMin, StrbD.dblForce[i][10]);
				}
			};

			pDeflectionParam->enCalcType = enDeflectionCalcType::ByCode;
			l_getStaticForce(vDL, pDeflectionParam->adMaxMoment[static_cast<int>(enDeflMaxMomentByCode::DeadPos)], pDeflectionParam->adMaxMoment[static_cast<int>(enDeflMaxMomentByCode::DeadNeg)]);
			l_getStaticForce(vLL, pDeflectionParam->adMaxMoment[static_cast<int>(enDeflMaxMomentByCode::LivePos)], pDeflectionParam->adMaxMoment[static_cast<int>(enDeflMaxMomentByCode::LiveNeg)]);

			const int iCantilever = m_pDataBase->GetCantileverType(iMemb);
			pDeflectionParam->SupportType = static_cast<enDeflSupportType>(DataCtrl.GetBeamSupportType(iMemb, iCantilever));
		}
		else if ( DConD.DesignCode == CONCODE_EC2_2_05 || DConD.DesignCode == CONCODE_BS5400_90 || DConD.DesignCode == CONCODE_TMH07_89)
		{
			pDeflectionParam->bCheck = true;
			pDeflectionParam->adValue[static_cast<int>(enDeflectionParameter::ShortLimit)] = rData.dRareDefl;
			pDeflectionParam->adValue[static_cast<int>(enDeflectionParameter::LongLimit)]  = rData.dDefLim;

			pDeflectionParam->enCalcType = enDeflectionCalcType::ByAnal;
			const int iCantilever = m_pDataBase->GetCantileverType(iMemb);
			pDeflectionParam->SupportType = static_cast<enDeflSupportType>(DataCtrl.GetBeamSupportType(iMemb, iCantilever));
			pDeflectionParam->dDAF = rData.dDAF;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pDeflectionParam;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddRebarRatio(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		DConD.Initialize();
		DConD.dRhoc = 0.03;
		DConD.dRhor = 0.03;
		DConD.dRhow = 0.04;
	}

	std::vector<CBindingData> vBindingData;

	CMembTypeUtil Util;
	for (const auto& iMemb : vKeyMemb)
	{
		if (!Util.IsRC(iMemb)) continue;

		auto* pRebarRatio = new CRebarRatio;
		if (Util.IsBeam(iMemb))
		{
			pRebarRatio->bConsiderMin = DConD.bMinRebarBeam;
		}
		else if (Util.IsColumn(iMemb))
		{
			pRebarRatio->bConsiderMin = DConD.bMinRebarColumn;
			pRebarRatio->adRho[CRebarRatio::Max] = DConD.dRhoc;
		}
		else if (Util.IsBrace(iMemb))
		{
			pRebarRatio->bConsiderMin = DConD.bMinRebarBrace;
			pRebarRatio->adRho[CRebarRatio::Max] = DConD.dRhor;
		}
		else if (Util.IsRCWall(iMemb))
		{
			pRebarRatio->bConsiderMin = DConD.bMinRebarWall;
			pRebarRatio->adRho[CRebarRatio::Max] = DConD.dRhow;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pRebarRatio;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddProbableMomentStrengthBeam(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pPostCtrl->GetDesignResult()->IsConcreteBeamDesignResultEnable()) return false;

	CMembTypeUtil Util;

	T_KEY_LIST aMembK;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	for (int i = 0; i < aElemK.GetSize(); ++i)
	{
		if (!Util.IsRCBeam(aElemK[i])) continue;;

		T_ELEM_K ShowKey;
		if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
		{
			ASSERT(0); continue;
		}
		aMembK.Add(ShowKey);
	}

	CArrayUtil::RemoveDup(aMembK);

	std::vector<CBindingData> vBindingData;
	int nMemb = aMembK.GetSize();
	for (int i = 0; i < nMemb; ++i)
	{
		T_RBEM_D RbemD;
		if (!pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignResultBeam(aMembK[i], RbemD)) continue;
		if (!RbemD.bCheck) continue;

		auto* pProbableMomentStrengthBeam = new CProbableMomentStrengthBeam;
		TProbableMomentBeam BeamData;
		BeamData.SetMpr(TProbableMomentBeam::MprIN, RbemD.Result[0].res_dProbpNMn);
		BeamData.SetMpr(TProbableMomentBeam::MprIP, RbemD.Result[0].res_dProbpPMn);
		BeamData.SetMpr(TProbableMomentBeam::MprJN, RbemD.Result[2].res_dProbpNMn);
		BeamData.SetMpr(TProbableMomentBeam::MprJP, RbemD.Result[2].res_dProbpPMn);
		pProbableMomentStrengthBeam->SetBeamMpr(BeamData);

		CBindingData Data;
		//Data.ForceKey = std::make_pair(true, aMembK[i]);
		Data.pData = pProbableMomentStrengthBeam;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		std::vector<long> vKey;
		vKey.push_back(aMembK[i]);
		RQuery.vvDcasNKey.push_back(std::make_pair(1, vKey));
		//RQuery.vOnlyOneKey.push_back(aMembK[i]);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDesignMomentStrengthBeam(const std::vector<long>& vKeyMemb)
{
	typedef T_KEY_LIST KeyList;
	typedef T_ELEM_K   EKey;

	auto _L_GetBeamMembKeyList = [](KeyList& rMembKeyList) -> int
	{
		auto* const pDoc = CDBDoc::GetDocPoint();
		auto* const pAttrCtrl = pDoc->m_pAttrCtrl;
		if (!pDoc || !pAttrCtrl)
		{
			ASSERT(0); return 0;
		}

		KeyList aElemKey;
		pAttrCtrl->GetElemKeyList(aElemKey);

		rMembKeyList.RemoveAll();

		CMembTypeUtil MembTypeUtil;
		const auto nElemSize = aElemKey.GetSize();
		for (int nIdx = 0; nIdx < nElemSize; ++nIdx)
		{
			const auto& CurElemKey = aElemKey[nIdx];
			if (!MembTypeUtil.IsRCBeam(CurElemKey))
			{
				continue;
			}

			EKey CurElemKeyShow;
			auto* pMembCtrl = CMembCtrl::GetMembCtrl();
			if (!pMembCtrl->GetShowElemByIncludeElem(CurElemKey, CurElemKeyShow))
			{
				ASSERT(0); continue;
			}

			rMembKeyList.Add(CurElemKeyShow);
		}

		CArrayUtil::RemoveDup(rMembKeyList);

		return rMembKeyList.GetSize();
	};

	SCWBRatio SCWBRat;
	const auto dphib = SCWBRat.Get_Phib();

	auto _L_CreateBindingData = [&dphib](const KeyList& aMembKey,
		std::vector<CBindingData>& rvBindingData)
	{
		enum TempSector { I, M, J };

		auto* const pDoc = CDBDoc::GetDocPoint();
		auto* const pPostCtrl = pDoc->m_pPostCtrl;
		if (!pPostCtrl)
		{
			ASSERT(0); return false;
		}

		rvBindingData.clear();

		const auto nMembSize = aMembKey.GetSize();
		for (int nIdx = 0; nIdx < nMembSize; ++nIdx)
		{
			const auto& CurMembKey = aMembKey[nIdx];

			T_RBEM_D ProductResult;
			if (!pPostCtrl->GetDesignResult()->ReadConcreteDesignResultBeam(CurMembKey, ProductResult))
			{
				continue;
			}

			if (!ProductResult.bCheck)
			{
				continue;
			}

			//TODO SCWB 여기서 처리하면 참 좋을텐데,
			//          지금 엔진에서 결과Item으로 Phi를 들고 있고,
			//          결과는 생성하지만 현재 제품에서 받을만한 변수가 없어서 넘기지는 않는 상태.
			//          추후에 결과 넘어가고 다시 엔진으로 받아올 수 있으면 Phib로 처리하면 될듯함.
			//          Gen은 개별 Phi값을 다시 게산하기 어려우니 걍 퉁쳐서 Phib값으로 쓰고 있었음.
			//          나중에 PhiBending값 넘어가면 요기에 채워줄 것.
			const auto dPhiIN = dphib;
			const auto dPhiIP = dphib;
			const auto dPhiJN = dphib;
			const auto dPhiJP = dphib;

			TDesignMomentStrengthBeam DataMn;

			DataMn.SetPhi(TDesignMomentStrengthBeam::PhiIN, dPhiIN);
			DataMn.SetPhi(TDesignMomentStrengthBeam::PhiIP, dPhiIP);
			DataMn.SetPhi(TDesignMomentStrengthBeam::PhiJN, dPhiJN);
			DataMn.SetPhi(TDesignMomentStrengthBeam::PhiJP, dPhiJP);

			DataMn.SetPhiMn(TDesignMomentStrengthBeam::PhiMnIN, ProductResult.Result[I].res_dpNMn);
			DataMn.SetPhiMn(TDesignMomentStrengthBeam::PhiMnIP, ProductResult.Result[I].res_dpPMn);
			DataMn.SetPhiMn(TDesignMomentStrengthBeam::PhiMnJN, ProductResult.Result[J].res_dpNMn);
			DataMn.SetPhiMn(TDesignMomentStrengthBeam::PhiMnJP, ProductResult.Result[J].res_dpPMn);
			
			DataMn.SetVe(TDesignMomentStrengthBeam::VeI, ProductResult.Result[I].aShrTor[EN_LSIDX_ULS].dVu);
			DataMn.SetVe(TDesignMomentStrengthBeam::VeJ, ProductResult.Result[J].aShrTor[EN_LSIDX_ULS].dVu);

			auto* pInputData = new CDesignMomentStrengthBeam();
			pInputData->SetBeamMn(DataMn);

			CRelationalQuery RelationQuery;
			RelationQuery.DataType = DataType::Memb;
			RelationQuery.vOnlyOneKey.push_back(CurMembKey);

			CBindingData BindingData;
			BindingData.ForceKey = std::make_pair(true, CurMembKey);
			BindingData.pData = pInputData;
			BindingData.vQueryInfo.push_back(RelationQuery);

			rvBindingData.push_back(BindingData);
		}

		return true;
	};

	auto* const pDoc = CDBDoc::GetDocPoint();
	auto* const pPostCtrl = pDoc->m_pPostCtrl;
	if (!pDoc || !pPostCtrl)
	{
		ASSERT(0); return false;
	}

	if (!pPostCtrl->GetDesignResult()->IsConcreteBeamDesignResultEnable())
	{
		return false;
	}

	KeyList aBeamMembKey;
	if (!_L_GetBeamMembKeyList(aBeamMembKey))
	{
		ASSERT(0); return false;
	}

	std::vector<CBindingData> vBindingData;
	if (!_L_CreateBindingData(aBeamMembKey, vBindingData))
	{
		ASSERT(0); return false;
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLimitOption(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD)) DConD.Initialize();

	std::vector<CBindingData> vBindingData;
	CMembTypeUtil Util;
	for (const auto& iMemb : vKeyMemb)
	{
		auto* pSectLimit = new CDgnLimitOption;
		if (Util.IsRC(iMemb))
		{
			if (Util.IsBeam(iMemb))
			{
				pSectLimit->bSectLimit = DConD.bBeam;
			}
			else if (Util.IsColumn(iMemb))
			{
				pSectLimit->bSectLimit = DConD.bColumn;
			}
			else if (Util.IsBrace(iMemb))
			{
				pSectLimit->bSectLimit = DConD.bBrace;
			}
			else if (Util.IsRCWall(iMemb))
			{
				pSectLimit->bSectLimit = DConD.bWall;
			}
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pSectLimit;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddWallID(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CArray<int, int> arWallId;
	pDoc->m_pAttrCtrl->GetWallIdList(arWallId);
	int nWallID = arWallId.GetSize();
	for (int i = 0; i < nWallID; ++i)
	{
		const int nWallID = arWallId[i];

		CArray<int, int> aWallId;
		aWallId.Add(nWallID);
		CArray<T_NODE_K, T_NODE_K> aKeyNode;
		CArray<T_ELEM_K, T_ELEM_K> aKeyElem;
		pDoc->m_pAttrCtrl->GetKeyListWallUser(&aWallId, &aKeyNode, &aKeyElem);
		int nElem = aKeyElem.GetSize();
		if (nElem == 0) continue;

		T_WMAK_D WmakData;
		WmakData.Initialize();
		if (!pDoc->m_pPostCtrl->Get_DgnWalWmakByWallID(nWallID, WmakData)) continue;

		auto* pWallID = new CDgnWallID;
		pWallID->vElem = Macro::CArray2Vec<T_ELEM_K, long>(aKeyElem);
		pWallID->strWallMark = CT2W(WmakData.MarkName);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, nWallID);
		Data.pData = pWallID;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey = pWallID->vElem;
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddBoundaryElementMethod(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CArray<int, int> arWallId;
	pDoc->m_pAttrCtrl->GetWallIdList(arWallId);
	int nWallID = arWallId.GetSize();
	for (int i = 0; i < nWallID; ++i)
	{
		const int nWallID = arWallId[i];

		CArray<int, int> aWallId;
		aWallId.Add(nWallID);
		CArray<T_NODE_K, T_NODE_K> aKeyNode;
		CArray<T_ELEM_K, T_ELEM_K> aKeyElem;
		pDoc->m_pAttrCtrl->GetKeyListWallUser(&aWallId, &aKeyNode, &aKeyElem);
		int nElem = aKeyElem.GetSize();
		if (nElem == 0) continue;
		T_BEMW_D BemwD;
		if (!pDoc->m_pAttrCtrl->GetBemw(nWallID, BemwD)) { BemwD.Initialize(); }

		auto* pBEMW = new CDgnBoundaryElementMethod;
		pBEMW->bBndrElemMethod = BemwD.bBndrElemMethod;
		pBEMW->nMethodType = BemwD.nMethodType;
		pBEMW->bBotStor = BemwD.bBotStor;
		pBEMW->nStorK = BemwD.nStorK;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, nWallID);
		Data.pData = pBEMW;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey = Macro::CArray2Vec<T_ELEM_K, long>(aKeyElem);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddWallOptions(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	std::unordered_map<UINT, bool> mMemb;
	std::vector<CBindingData> vBindingData;

	T_DCON_D DConD;
	if(!CDgnDataCtrl().Get_DgnConDcon(DConD)) DConD.Initialize();

	for (const auto MembK : vKeyMemb)
	{
		if (mMemb.find(MembK) != mMemb.end()) continue;

		T_MBTP_D MbtpD;
		MbtpD.Initialize();
		DataCtrl.Get_DgnGenMbtp(MembK, MbtpD);

		T_WALL_K WallK;
		T_KEY_LIST aElemK;
		BOOL bRev = FALSE;
		T_MEMB_K ShowK = 0;
		if (MbtpD.nMbType != D_MBTP_WALL)
		{
			continue;
		}
		else
		{
			unsigned int nInternalWallID = 0;
			if (!pDoc->m_pPostCtrl->GetInternalWallId(MembK, nInternalWallID))
			{
				ASSERT(0);
				continue;
			}

			T_WALL_D WallD;
			if (!pDoc->m_pPostCtrl->GetWallKey(nInternalWallID, WallK.keymap)) continue;
			if (!pDoc->m_pPostCtrl->GetWallData(WallK.keymap, WallD))          continue;
			ShowK = WallD.RootElemKey;
			//if (!pDoc->m_pPostCtrl->GetWallElemList(WallK.keymap, aElemK))     continue;
			//pDoc->m_pAttrCtrl->GetRootWallElemKey(aElemK[0], ShowK);
		}
		T_DCRB_D Dcrb;
		if (!DataCtrl.Get_DgnWalDcrb(WallK.keymap, Dcrb)) Dcrb.Initialize();

		auto* pWallOptions = new CDgnWallOptions;

		pWallOptions->bOutOfBending = Dcrb.bOutofPlaneBending;
		pWallOptions->enCheckMethod = [&Dcrb]()
		{
			switch (Dcrb.nEndRebarDesignMethod)
			{
			case 1: return CDgnWallOptions::enWallMethod1;
			case 2: return CDgnWallOptions::enWallMethod2;
			case 3: return CDgnWallOptions::enWallMethod3;
			case 4: return CDgnWallOptions::enWallMethod4;
			default:
				ASSERT(0);
				break;
			}
			return CDgnWallOptions::enWallMethod1;
		}();
		pWallOptions->enForceEnvelopMethod = [&DConD]()
		{
			if(DConD.bSpecialSeismic)
			{
				switch(DConD.nWallDgnEnvelop)
				{
					case 1: return CDgnWallOptions::enForceEnvMethod_Max;
					case 2: return CDgnWallOptions::enForceEnvMethod_Pre;
					default:
					{
						if(DConD.DesignCode == CONCODE_NTC_DCEC_2023) return CDgnWallOptions::enForceEnvMethod_Max;
						return CDgnWallOptions::enForceEnvMethod_None;
					}
					break;
				}
			}
			return CDgnWallOptions::enForceEnvMethod_None;
		}(); 
		pWallOptions->aSpacingEndBar[CDgnWallOptions::enSpcEndBar4] = Dcrb.dEndRebarSpace[CDgnWallOptions::enSpcEndBar4];
		pWallOptions->aSpacingEndBar[CDgnWallOptions::enSpcEndBar6] = Dcrb.dEndRebarSpace[CDgnWallOptions::enSpcEndBar6];
		pWallOptions->aSpacingEndBar[CDgnWallOptions::enSpcEndBar8] = Dcrb.dEndRebarSpace[CDgnWallOptions::enSpcEndBar8];
		pWallOptions->dClearDist = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 40.0); // refer to CRCS_WallDesign::Get_WallClearDistBar

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, ShowK);
		Data.pData = pWallOptions;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(ShowK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSlabOptions(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_MSRB_D MsrbData;
	MsrbData.Initialize();
	pDoc->m_pAttrCtrl2->Get_DgnConMsrb(MsrbData);

	std::vector<CBindingData> vBindingData;

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		auto* pSlabOptions = new CDgnSlabOptions;

		pSlabOptions->bFlexural = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlabFlexural();
		pSlabOptions->b1WayFlexural = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlab1WayFlexural();
		pSlabOptions->b2wayShear = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlabPunchingShear();
		pSlabOptions->en2WayType = [](const int& iShearType, const int& iStrType)
		{
			if (iShearType == 0) return CDgnSlabOptions::en2WayShearCode;
			if (iStrType == 1)   return CDgnSlabOptions::en2WayShearFEMSide;
			return CDgnSlabOptions::en2WayShearFEMSegment;
		}(((IDgnPerformDataBaseRC*)m_pDataBase)->GetSlabPunchingType(), ((IDgnPerformDataBaseRC*)m_pDataBase)->GetSlabPunchingStressType());
		pSlabOptions->b1wayShear = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlab1WayShear();
		pSlabOptions->bUserThick = SbdoD.bUseMt ? false : true;
		pSlabOptions->dUserThick = SbdoD.dThickness;

		// cover
		if (SbdoD.nType == SBDO_SLAB)
		{
			pSlabOptions->dT[0] = MsrbData.dT_Slab_X;
			pSlabOptions->dT[1] = MsrbData.dT_Slab_Y;
			pSlabOptions->dB[0] = MsrbData.dB_Slab_X;
			pSlabOptions->dB[1] = MsrbData.dB_Slab_Y;
		}
		else if (SbdoD.nType == SBDO_MAT)
		{
			pSlabOptions->dT[0] = MsrbData.dT_Mat_X;
			pSlabOptions->dT[1] = MsrbData.dT_Mat_Y;
			pSlabOptions->dB[0] = MsrbData.dB_Mat_X;
			pSlabOptions->dB[1] = MsrbData.dB_Mat_Y;
		}
		// see default vaut ; double CRCSDataBase::Calc_CoverThick
		if (EQ0(pSlabOptions->dT[0]))    pSlabOptions->dT[0] = 0.026; // m
		if (EQ0(pSlabOptions->dT[1]))    pSlabOptions->dT[1] = 0.026; // m
		if (EQ0(pSlabOptions->dB[0]))    pSlabOptions->dB[0] = 0.026; // m
		if (EQ0(pSlabOptions->dB[1]))    pSlabOptions->dB[1] = 0.026; // m

		pSlabOptions->nLineDiv = 1;
		pSlabOptions->nRoundDiv = 3;
		pSlabOptions->nCircleDiv = 24;

		pSlabOptions->en1wayCalcType = static_cast<CDgnSlabOptions::en1WayCalc>(((IDgnPerformDataBaseRC*)m_pDataBase)->GetCuttingDiagramLeftRight());

		pSlabOptions->bServiceability = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlabServiceability();
		pSlabOptions->enDefCheckType = static_cast<CDgnSlabOptions::enDefType>(((IDgnPerformDataBaseRC*)m_pDataBase)->GetDeflectionCheckType());
		pSlabOptions->bLongTermDef = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsLongTermDef();
		pSlabOptions->dCreepCoef = ((IDgnPerformDataBaseRC*)m_pDataBase)->GetDeflectionCreepCoeff();

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, aElem[0]);
		Data.pData = pSlabOptions;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		for (int i = 0; i < nElem; ++i)
		{
			RQuery.vOnlyOneKey.push_back(aElem[i]);
		}
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddShellOptions(const std::vector<long>& vKeyMemb)
{
	CMembTypeUtil Util;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		if (!Util.IsRCShell(aElem[0]))
			continue;

		auto* pShellOptions = new CDgnShellOptions;

		pShellOptions->bFlexural = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsShellFlexureCheck();
		pShellOptions->bShear = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsShellShearCheck();

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, aElem[0]);
		Data.pData = pShellOptions;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(aElem[0]);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSandwichModel(const std::vector<long>& vKeyMemb)
{
	CMembTypeUtil Util;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDgnDataCtrl DataCtrl;
	std::vector<CBindingData> vBindingData;
	int iUnitLen = D_UNITSYS_LENGTH_INDEX_M;
	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		if (!Util.IsRCShell(aElem[0]))
			continue;

		auto* pSandwich = new CSandwichModel;

		//여기서 Isdesign으로 처리하는것도 좀 별룬데..
		if (m_pDataBase->IsDesigning())
		{
			T_MSRB_D MsrbD;
			if (!pDoc->m_pAttrCtrl->GetMsrb(MsrbD))
			{
				MsrbD.Initialize();
			}

			pSandwich->bOptimal = MsrbD.bThickOptimal;
			pSandwich->nNumItr = MsrbD.nIteration;
			pSandwich->dTol = MsrbD.dConvTolerance;
			pSandwich->dTopLayerRatio = MsrbD.dTopThik;
			pSandwich->dBotLayerRatio = MsrbD.dBotThik;
		}
		else
		{
			CArray<T_RBSL_D, T_RBSL_D&> aRsbl;
			for (int i = 0; i < aElem.GetSize(); ++i)
			{
				if (!DataCtrl.Get_DgnConRbsl(aElem[i], aRsbl, iUnitLen))
				{
					continue;
				}
				break;
			}

			pSandwich->bOptimal = aRsbl[0].bThickOptimal;
			pSandwich->nNumItr = aRsbl[0].nIteration;
			pSandwich->dTol = aRsbl[0].dConvTolerance;
			pSandwich->dTopLayerRatio = aRsbl[0].dTopThik;
			pSandwich->dBotLayerRatio = aRsbl[0].dBotThik;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, aElem[0]);
		Data.pData = pSandwich;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(aElem[0]);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddForceOption2D(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	std::vector<CBindingData> vBindingData;

	std::vector<T_SBDO_K> vSbdo;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(vSbdo);
	for (const auto SbdoK : vSbdo)
	{
		T_SBDO_D SbdoD;
		if (!pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD))
		{
			ASSERT(0);
			continue;
		}

		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aElem);
		if (nElem == 0) continue;

		auto* p2DForceOptions = new CForceOption2D;
		if (SbdoD.nMembTypeCivil == 1) // plate beam
		{
			if ( m_pDataBase->IsDesigning() )
			{
				T_PBDD_D PbddD;
				if (!CMembCtrl::GetMembCtrl()->GetPbddByElem(aElem[0], PbddD)) PbddD.Initialize();
				p2DForceOptions->bAvgNodal = PbddD.nOpt == 1 ? true : false;
			}
			else
			{
				T_PBDC_D PbdcD;
				if (!CMembCtrl::GetMembCtrl()->GetPbdcByElem(aElem[0], PbdcD)) PbdcD.Initialize();
				p2DForceOptions->bAvgNodal = PbdcD.nOpt==1 ? true : false;
			}
			
			p2DForceOptions->enCheckPosition = CForceOption2D::enBothElemCenterNodal;
			p2DForceOptions->bAllRange = false;
			p2DForceOptions->enSmoothingType = CForceOption2D::enSmoothingElement;
			p2DForceOptions->bWoodArmer = false;
			p2DForceOptions->bWoodArmer_first = false;
		}
		else if (SbdoD.nMembTypeCivil == 2) // plate column
		{
			if ( m_pDataBase->IsDesigning() )
			{
				T_PCDD_D PcddD;
				if (!CMembCtrl::GetMembCtrl()->GetPcddByElem(aElem[0], PcddD)) PcddD.Initialize();
				p2DForceOptions->bAvgNodal = PcddD.nOpt==1 ? true : false;
			}
			else
			{
				T_PCDC_D PcdcD;
				if (!CMembCtrl::GetMembCtrl()->GetPcdcByElem(aElem[0], PcdcD)) PcdcD.Initialize();
				p2DForceOptions->bAvgNodal = PcdcD.nOpt == 1 ? true : false;
			}

			p2DForceOptions->enCheckPosition = CForceOption2D::enBothElemCenterNodal;
			p2DForceOptions->bAllRange = false;
			p2DForceOptions->enSmoothingType = CForceOption2D::enSmoothingElement;
			p2DForceOptions->bWoodArmer = false;
			p2DForceOptions->bWoodArmer_first = false;
		}
		else
		{
			p2DForceOptions->enCheckPosition = CForceOption2D::enNodal;
			p2DForceOptions->bAllRange = true;
			p2DForceOptions->bAvgNodal = ((IDgnPerformDataBaseRC*)m_pDataBase)->IsSlabAverageNodal();
			p2DForceOptions->enSmoothingType = ((IDgnPerformDataBaseRC*)m_pDataBase)->GetSlabCellOption() == 1 ? CForceOption2D::enSmoothingWidth : CForceOption2D::enSmoothingElement;
			p2DForceOptions->dWidth = ((IDgnPerformDataBaseRC*)m_pDataBase)->GetSlabCellSize();
			p2DForceOptions->bWoodArmer = [&pDoc]()
			{
				if (CDBLib::IsCodeMeshSlabDesignControl())
				{
					T_SLDC_D SldcD;
					if (pDoc->m_pAttrCtrl2->GetSldc(SldcD))
					{
						return SldcD.bConsiderMxy ? true : false;
					}
				}
				return true;
			}();
			p2DForceOptions->bWoodArmer_first = false;
			if (p2DForceOptions->bWoodArmer)
			{
				CString strWoodArmerBefore = CProduct::GetTestEnvValue(_ULS(WABeforeSmoothing)); // Yes면 부재력 우드아머 먼저 계산하고 스무딩
				p2DForceOptions->bWoodArmer_first = strWoodArmerBefore.CompareNoCase(_ULS(yes)) == 0 ? true : false;
			}
		}

		p2DForceOptions->dAngle1 = SbdoD.dAngle[0];
		p2DForceOptions->dAngle2 = SbdoD.dAngle[1];

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, aElem[0]);
		Data.pData = p2DForceOptions;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		for (int i = 0; i < nElem; ++i)
		{
			RQuery.vOnlyOneKey.push_back(aElem[i]);
		}
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSlabServParam(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	std::vector<CBindingData> vBindingData;
	for (const auto ElemK : vKeyMemb)
	{
		T_SERV_D ServD;
		if (!DataCtrl.Get_DgnConServ(ElemK, ServD)) continue;

		auto* pSlabServParam = new CDgnSlabServParam;
		pSlabServParam->enAllowDef = static_cast<CDgnSlabServParam::enDefCtrl>(ServD.iDeflecEnv);
		pSlabServParam->dAllowDef = ServD.dDefLim;
		pSlabServParam->dDAF = ServD.dDAF;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, ElemK);
		Data.pData = pSlabServParam;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey.push_back(ElemK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSlabShearReinforcement()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;


	POSITION pos = pDoc->m_pAttrCtrl2->GetStartSsri();
	while (pos)
	{
		T_SSRI_K SsriK;
		T_SSRI_D SsriD;
		pDoc->m_pAttrCtrl2->GetNextSsri(pos, SsriK, SsriD);

		auto* pSSR = new CSlabShearReinforcement;
		pSSR->aData[Macro::Ordinal(CSlabShearReinforcement::enComponent::Avy)] = SsriD.dAvY;
		pSSR->aData[Macro::Ordinal(CSlabShearReinforcement::enComponent::Avz)] = SsriD.dAvZ;
		pSSR->aData[Macro::Ordinal(CSlabShearReinforcement::enComponent::Space)] = SsriD.dSpace;
		pSSR->aData[Macro::Ordinal(CSlabShearReinforcement::enComponent::Yield)] = SsriD.dYield;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, SsriK);
		Data.pData = pSSR;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey.push_back(SsriK);

		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSlabColumnLocation()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;


	POSITION pos = pDoc->m_pAttrCtrl2->GetStartColc();
	while (pos)
	{
		T_COLC_K ColcK;
		T_COLC_D ColcD;
		pDoc->m_pAttrCtrl2->GetNextColc(pos, ColcK, ColcD);

		auto* pSSR = new CSlabColumnLocation;
		pSSR->enColumnLocation = static_cast<CSlabColumnLocation::enPosition>(ColcD.nColumnLocation);
		for (int i = 0; i < 2; ++i)
		{
			pSSR->enCriSectDir[i] = static_cast<CSlabColumnLocation::enDir>(ColcD.nCriSectDir[i]);
			pSSR->dCriSectDis[i] = ColcD.dCriSectDis[i];
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, ColcK);
		Data.pData = pSSR;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey.push_back(ColcK);

		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddSlendernessApproximation(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		T_ELEM_D eData;
		eData.Initialize();
		if (!pDoc->m_pAttrCtrl->GetElem(Key, eData))	continue;
		// Change by ZINU.('02.10.22).  Include not only column but also wall.
		bool bWall = pDoc->m_pAttrCtrl->IsWall(eData.eltyp);  // Wall.

		if (!Util.IsRC(Key) || !(Util.IsColumn(Key) || Util.IsBrace(Key) || bWall)) continue;

		auto* pSlendernessApproximation = new CSlendernessApproximation;

		pSlendernessApproximation->bApproximaion = true; // 현재 Gen은 항상 기준에 제시된 근사치 적용

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pSlendernessApproximation;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddConcStressStrainType(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD)) DConD.Initialize();

	CArray<T_SBDO_K, T_SBDO_K> aSbdoList;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoList);
	int nSbdo = aSbdoList.GetSize();
	std::unordered_map<T_ELEM_K, T_SBDO_K> mSbdoElem;
	for (int i = 0; i < nSbdo; ++i)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoList[i], aElem);
		for (int j = 0; j < nElem; ++j)
		{
			mSbdoElem.insert(std::make_pair(aElem[j], aElem[0]));
		}
	}

	std::unordered_map<long, bool> mSbdoDone;

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key)) continue;
		if (Util.IsPlate(Key))
		{
			const auto itr = mSbdoElem.find(Key);
			if (itr == mSbdoElem.end()) continue;
			Key = itr->second;

			const auto itrDone = mSbdoDone.find(Key);
			if (itrDone != mSbdoDone.end()) continue;
			mSbdoDone[Key] = true;
		}

		auto* pConcStresStrainType = new CDgnConcStressStrainType;

		if (Util.IsBeam(Key) || Util.IsPlate(Key))
		{
			switch (DConD.nStressStrainType)
			{
			case 0:
				pConcStresStrainType->enConcStressStrainType = enConcStressStrainType::EquivalentRectangle;
				break;
			case 1:
				pConcStresStrainType->enConcStressStrainType = enConcStressStrainType::ParabolaRectangleAvg;
				break;
			case 2:
				pConcStresStrainType->enConcStressStrainType = enConcStressStrainType::ParabolaRectangle;
				break;
			default:
				ASSERT(0);
				pConcStresStrainType->enConcStressStrainType = enConcStressStrainType::EquivalentRectangle;
				break;
			}
		}
		else
		{
			pConcStresStrainType->enConcStressStrainType = enConcStressStrainType::EquivalentRectangle;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pConcStresStrainType;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddConcRebarStressMethod(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD)) DConD.Initialize();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key)) continue;
	
		auto* pConcRebarStressMethod = new CDgnConcRebarStressMethod;

		if (Util.IsBeam(Key))
		{
			if(DConD.nFsMethod4Beam == 0)
				pConcRebarStressMethod->enStress = enConcRebarStressMethod::Assume;
			else
				pConcRebarStressMethod->enStress = enConcRebarStressMethod::Detail;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pConcRebarStressMethod;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddShearAngle(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;


	T_DCON_D DConD;
	if (!pDoc->m_pAttrCtrl->GetDcon(DConD))
	{
		ASSERT(0); DConD.Initialize();
	}

	for (const auto ElemK : vKeyMemb)
	{
		auto* pShearAngle = new CShearAngle;
		pShearAngle->dDegree = DConD.dStrutAngle;

		T_RCHK_D RchkD; RchkD.Initialize();
		if (pDoc->m_pAttrCtrl->GetRchk(ElemK, RchkD))
		{
			pShearAngle->dStirrupAngleI = RchkD.BEAM.dSubBarAngle[0];
			pShearAngle->dStirrupAngleM = RchkD.BEAM.dSubBarAngle[1];
			pShearAngle->dStirrupAngleJ = RchkD.BEAM.dSubBarAngle[2];
		}

		CBindingData Data;
		Data.pData = pShearAngle;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey.push_back(ElemK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddPartialFactor(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CMembTypeUtil Util;
	if ( vKeyMemb.size() == 0 )
		return true;

	//나중에 요소별로 다르게 입력하는 경우가 있으면 수정 필요.
	auto* pPartialFactor = new CPartialFactor;

	std::vector<long> vTgtKeyMemb = vKeyMemb;
	if ( Util.IsRC(vKeyMemb[0]) )
	{
		T_DCON_D DConD;
		if ( !pDoc->m_pAttrCtrl->GetDcon(DConD) )
		{
			ASSERT(0); DConD.Initialize();
		}

		if ( EQ0(DConD.dPhi[0]) )
		{
			//default 값이 없다면..
			if (DConD.DesignCode == CONCODE_EC2_2_05)
			{
				pDoc->m_pDgnDataCtrl->SetDatabyNationalAnnex(DConD.nNationalAnnex, DConD.dPhi, DConD.dAlphacc, DConD.dAlphacc4Shear);
			}
			else
			{			
				double dPhi[6] = { 1.0, };
				pDoc->m_pDgnDataCtrl->Get_DgnConPhiDef(DConD.DesignCode, dPhi);
				DConD.dPhi[0] = dPhi[0];
				DConD.dPhi[1] = dPhi[1];
				DConD.dPhi[2] = dPhi[2];
				DConD.dPhi[3] = dPhi[3];
				DConD.dPhi[4] = dPhi[4];
				DConD.dPhi[5] = dPhi[5];
			}			
		}

		if ( DConD.DesignCode == CONCODE_BS5400_90 )
		{
            pPartialFactor->aFactor[CPartialFactor::GammaC_Fundamental].MakePair(false, DConD.dPhi[0]);
            pPartialFactor->aFactor[CPartialFactor::GammaC_Accidental].MakePair(false, DConD.dPhi[0]);
            pPartialFactor->aFactor[CPartialFactor::GammaS_Fundamental].MakePair(false, DConD.dPhi[1]);
            pPartialFactor->aFactor[CPartialFactor::GammaS_Accidental].MakePair(false, DConD.dPhi[1]);
            pPartialFactor->aFactor[CPartialFactor::Alphacc].MakePair(false, DConD.dAlphacc);
		}
		else if (DConD.DesignCode == CONCODE_TMH07_89)
		{
			pPartialFactor->aFactor[CPartialFactor::GammaC_Fundamental].MakePair(false, DConD.dPhi[0]);
			pPartialFactor->aFactor[CPartialFactor::GammaC_Accidental].MakePair(false, DConD.dPhi[0]);
			pPartialFactor->aFactor[CPartialFactor::GammaS_Fundamental].MakePair(false, DConD.dPhi[1]);
			pPartialFactor->aFactor[CPartialFactor::GammaS_Accidental].MakePair(false, DConD.dPhi[1]);
			pPartialFactor->aFactor[CPartialFactor::Alphacc].MakePair(false, DConD.dAlphacc);
		}
		else
        {
            pPartialFactor->aFactor[CPartialFactor::GammaC_Fundamental].MakePair(false, DConD.dPhi[0]);
            pPartialFactor->aFactor[CPartialFactor::GammaC_Accidental].MakePair(false, DConD.dPhi[1]);
            pPartialFactor->aFactor[CPartialFactor::GammaC_SLS].MakePair(false, DConD.dPhi[4]);
            pPartialFactor->aFactor[CPartialFactor::GammaS_Fundamental].MakePair(false, DConD.dPhi[2]);
            pPartialFactor->aFactor[CPartialFactor::GammaS_Accidental].MakePair(false, DConD.dPhi[3]);
            pPartialFactor->aFactor[CPartialFactor::GammaS_SLS].MakePair(false, DConD.dPhi[5]);
            pPartialFactor->aFactor[CPartialFactor::Alphacc].MakePair(false, DConD.dAlphacc);
			if (DConD.nNationalAnnex == Italy)
			{
				// Italy는 입력 안받고 있어 Alpha_cc로 할당
				pPartialFactor->aFactor[CPartialFactor::Alphacc_Shear].MakePair(false, DConD.dAlphacc);
			}
			else
			{
				pPartialFactor->aFactor[CPartialFactor::Alphacc_Shear].MakePair(false, DConD.dAlphacc4Shear);
			}
		}

		if ( Util.IsPlate(vKeyMemb[0]) )
        {			
			for (auto Key : vKeyMemb)
			{
				T_ELEM_K ElemK = CMembCtrl::GetMembCtrl()->GetRepElem((T_ELEM_K)Key);
				if (ElemK != 0) vTgtKeyMemb.push_back(ElemK);
			}
        }
	}
	else if ( Util.IsSTL(vKeyMemb[0]) )
	{
		T_DSTL_D DStlD;
		if (!CDgnDataCtrl().Get_DgnStlDstl(DStlD))
		{
			ASSERT(0); DStlD.Initialize();
		}

		CDgnDataCtrl DataCtrl;
		double dPhi[7];
		if ( !DataCtrl.Get_DgnStlPhiDef(DStlD.DesignCode, dPhi, DStlD.nNationalAnnex) )	ASSERT(0);
		if ( DStlD.dPhi[0] == 0.0 )
		{
			DStlD.dPhi[0] = dPhi[0];
		}

		if ( DStlD.dPhi[1] == 0.0 )
		{
			DStlD.dPhi[1] = dPhi[1];
		}

		if ( DStlD.dPhi[2] == 0.0 )
		{
			DStlD.dPhi[2] = dPhi[2];
		}

		pPartialFactor->aFactor[CPartialFactor::GammaM0].MakePair(false, DStlD.dPhi[0]);
		pPartialFactor->aFactor[CPartialFactor::GammaM1].MakePair(false, DStlD.dPhi[1]);
		pPartialFactor->aFactor[CPartialFactor::GammaM2].MakePair(false, DStlD.dPhi[2]);
	}
	else
	{
		_SAFE_DELETE(pPartialFactor); return true;
	}

	vTgtKeyMemb.erase(std::unique(vTgtKeyMemb.begin(), vTgtKeyMemb.end()), vTgtKeyMemb.end());

	CBindingData Data;
	Data.pData = pPartialFactor;
	CRelationalQuery RQuery;
	RQuery.SetSimpleQueryData(DataType::Memb, vTgtKeyMemb);
	Data.vQueryInfo.push_back(RQuery);
	vBindingData.push_back(Data);

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddStressCheck(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key))
			continue;

		T_SERV_D ServD;
		ServD.Initialize();
        bool bExistData = pDoc->m_pAttrCtrl->GetServ(Key, ServD);
		
        auto* pStressCheck = new CStressCheck;
		if ( bExistData )
		{
			pStressCheck->adFactor[CStressCheck::k1].MakePair(false, ServD.dStressk[0]);
			pStressCheck->adFactor[CStressCheck::k2].MakePair(false, ServD.dStressk[1]);
			pStressCheck->adFactor[CStressCheck::k3].MakePair(false, ServD.dStressk[2]);
			pStressCheck->adFactor[CStressCheck::k4].MakePair(false, ServD.dStressk[3]);
		}
		else
		{   //auto
			pStressCheck->adFactor[CStressCheck::k1].MakePair(true, 0.6);
			pStressCheck->adFactor[CStressCheck::k2].MakePair(true, 0.45);
			pStressCheck->adFactor[CStressCheck::k3].MakePair(true, 0.8);
			pStressCheck->adFactor[CStressCheck::k4].MakePair(true, 1.0);
		}

        //뭐 이런 경우가.. 다른 부재는 없으면 기본값으로 설계해주는데
        //Column인 경우에 ServD 데이터가 없거나 ExposureClass가 범위내에 내용이 아닌 경우 응력검토 안함. CRCSC_DataBase::IsColmServiceChk() 내용 참조.
        if ( Util.IsColumn(Key) )
        {
            if ( !bExistData || ServD.iStressEnv < 5 || ServD.iStressEnv > 14 )
            {
                pStressCheck->bCheck = false;
            }
        }
        else
        {
            pStressCheck->bCheck = true;
        }

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pStressCheck;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddCrackLimit(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		ASSERT(0); return false;
	}

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key))
			continue;

		T_SERV_D ServD;
		ServD.Initialize();
		auto* pCrackLimit = new CCrackParameter;
		if (pDoc->m_pAttrCtrl->GetServ(Key, ServD))
		{
			pCrackLimit->aFactor[CCrackParameter::Limit_Q].MakePair(!ServD.bCrackFlag[2], ServD.dCrackLim[2]);
			pCrackLimit->aFactor[CCrackParameter::Limit_F].MakePair(!ServD.bCrackFlag[1], ServD.dCrackLim[1]);
			pCrackLimit->aFactor[CCrackParameter::Limit_C].MakePair(!ServD.bCrackFlag[0], ServD.dCrackLim[0]);
			switch ( ServD.iStressEnv )
			{
				case 0:
                    pCrackLimit->ExposureClass = enExposureClassEU::X0;
					break;
				case 1:
					pCrackLimit->ExposureClass = enExposureClassEU::XC1;
					break;
				case 2:
                    pCrackLimit->ExposureClass = enExposureClassEU::XC2;
                    break;
				case 3:
                    pCrackLimit->ExposureClass = enExposureClassEU::XC3;
                    break;
				case 4:
                    pCrackLimit->ExposureClass = enExposureClassEU::XC4;
                    break;
				case 5:
                    pCrackLimit->ExposureClass = enExposureClassEU::XD1;
                    break;
				case 6:
                    pCrackLimit->ExposureClass = enExposureClassEU::XD2;
                    break;
				case 7:
                    pCrackLimit->ExposureClass = enExposureClassEU::XD3;
                    break;
				case 8:
                    pCrackLimit->ExposureClass = enExposureClassEU::XS1;
                    break;
				case 9:
                    pCrackLimit->ExposureClass = enExposureClassEU::XS2;
                    break;
				case 10:
                    pCrackLimit->ExposureClass = enExposureClassEU::XS3;
                    break;
				case 11:
                    pCrackLimit->ExposureClass = enExposureClassEU::XF1;
                    break;
				case 12:
                    pCrackLimit->ExposureClass = enExposureClassEU::XF2;
                    break;
				case 13:
                    pCrackLimit->ExposureClass = enExposureClassEU::XF3;
                    break;
				case 14:
                    pCrackLimit->ExposureClass = enExposureClassEU::XF4;
                    break;
				case 15:
                    pCrackLimit->ExposureClass = enExposureClassEU::XA1;
                    break;
				case 16:
                    pCrackLimit->ExposureClass = enExposureClassEU::XA2;
                    break;
				case 17:
                    pCrackLimit->ExposureClass = enExposureClassEU::XA3;
                    break;
				default:
					ASSERT(0);
					break;
			}
		}
		else
		{
			pCrackLimit->aFactor[CCrackParameter::Limit_Q].MakePair(true, 0.0);
			pCrackLimit->aFactor[CCrackParameter::Limit_F].MakePair(true, 0.0);
			pCrackLimit->aFactor[CCrackParameter::Limit_C].MakePair(true, 0.0);
		}

		pCrackLimit->ExposureClass = [&ServD] ()
		{
			//XSA 이게 없는데?
			switch ( ServD.iStressEnv )
			{
			case 0:
				return enExposureClassEU::X0;
			case 1:
				return enExposureClassEU::XC1;
			case 2:
				return enExposureClassEU::XC2;
			case 3:
				return enExposureClassEU::XC3;
			case 4:
				return enExposureClassEU::XC4;
			case 5:
				return enExposureClassEU::XD1;
			case 6:
				return enExposureClassEU::XD2;
			case 7:
				return enExposureClassEU::XD3;
			case 8:
				return enExposureClassEU::XS1;
			case 9:
				return enExposureClassEU::XS2;
			case 10:
				return enExposureClassEU::XS3;
			case 11:
				return enExposureClassEU::XF1;
			case 12:
				return enExposureClassEU::XF2;
			case 13:
				return enExposureClassEU::XF3;
			case 14:
				return enExposureClassEU::XF4;
			case 15:
				return enExposureClassEU::XA1;
			case 16:
				return enExposureClassEU::XA2;
			case 17:
				return enExposureClassEU::XA3;
			default:
				ASSERT(0);
				break;
			}

			return enExposureClassEU::X0;
		}();
		pCrackLimit->aFactor[CCrackParameter::k1].MakePair(true, 0.0);
		pCrackLimit->aFactor[CCrackParameter::k2].MakePair(true, 0.0);
		pCrackLimit->aFactor[CCrackParameter::k3].MakePair(true, 0.0);
		pCrackLimit->aFactor[CCrackParameter::k4].MakePair(true, 0.0);

		pCrackLimit->dkc = DConD.dkc;
		pCrackLimit->ProtectionLevel = DConD.enPL;
		pCrackLimit->ServiceLife = DConD.enSL;
		pCrackLimit->StructType = DConD.enSType;
		pCrackLimit->BridgeType = static_cast<enBridgeTypeEU>(DConD.nBridgeType);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pCrackLimit;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLimitSlendernessRatioParamEU(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_DCON_D DConD;
	if (!CDgnDataCtrl().Get_DgnConDcon(DConD))
	{
		ASSERT(0); return false;
	}

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < vKeyMemb.size(); ++i)
	{
		auto* pLimit = new CLimitSlendernessRatioParamEU;
		pLimit->aFactor[CLimitSlendernessRatioParamEU::A].MakePair(false, DConD.dA);
		pLimit->aFactor[CLimitSlendernessRatioParamEU::B].MakePair(false, DConD.dB);
		pLimit->aFactor[CLimitSlendernessRatioParamEU::C].MakePair(DConD.bAutoC, DConD.dC);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, vKeyMemb[i]);
		Data.pData = pLimit;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(vKeyMemb[i]);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::SetDataPool(const std::vector<dgn::def::CBindingData>& vBindingData, std::vector<DWORD_PTR>& vIter)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

	if (!pDgnDoc->GetDgnPerform()->SetDataPool(vBindingData, vIter))
	{
		ASSERT(0); return false;
	}

	if (pDoc->IsRegTestMode())
	{
		pDoc->SetModifiedFlag(FALSE);
	}

	return true;
}

bool CDgnPerformInputConverter::SetDataPool(const std::vector<dgn::def::CBindingData>& vBindingData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

	if (!pDgnDoc->GetDgnPerform()->SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	if (pDoc->IsRegTestMode())
	{
		pDoc->SetModifiedFlag(FALSE);
	}

	return true;
}

bool CDgnPerformInputConverter::AddMembQueryFromElemQuery(std::vector<dgn::def::CBindingData>& vBindingData)
{
	if (vBindingData.size() == 0) return false;
	if (vBindingData[0].vQueryInfo[0].DataType == DataType::Memb) return true;
	if (vBindingData[0].vQueryInfo[0].DataType != DataType::Elem) return false;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	for (auto& Data : vBindingData)
	{
		std::vector<long> vMembKey;
		for (auto& ElemK : Data.vQueryInfo[0].vOnlyOneKey)
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(ElemK, ShowKey)) {
				ShowKey = ElemK;
			}
			vMembKey.push_back(ShowKey);
		}

		CRelationalQuery QueryMember;
		QueryMember.DataType = DataType::Memb;
		QueryMember.vOnlyOneKey.assign(vMembKey.begin(), vMembKey.end());
		Data.vQueryInfo.push_back(QueryMember);

		CRelationalQuery QueryCuttingLine;
		QueryCuttingLine.DataType = DataType::CuttingLine;
		QueryCuttingLine.vOnlyOneKey.clear();
		Data.vQueryInfo.push_back(QueryCuttingLine);
	}

	return true;
}

TDia CDgnPerformInputConverter::CreateRebarDia(const CString& strRebarName, const T_MATL_REBAR& RebarData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	int nRCCode = 0;
	T_DCON_D DConD;
	CDgnDataCtrl DataCtrl;
	if (DataCtrl.Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	TDia Dia;
	Dia.strName = CT2W(strRebarName);
	Dia.Geom.dDia = pDoc->m_pMatlDB->Get_RebarDia(strRebarName);
	Dia.Geom.dArea = pDoc->m_pMatlDB->Get_RebarArea(strRebarName);
	Dia.Geom.dOutDia = pDoc->m_pMatlDB->Get_RebarOutDia(strRebarName);
	Dia.Geom.dPerimeter = CMathFunc::m_pi * pow(Dia.Geom.dDia, 2.0);
	Dia.Matl.dfy = RebarData.B_fy;
	Dia.Matl.dEs = Get_Es(nRCCode, Dia.Matl.dfy);

	return Dia;
}

bool CDgnPerformInputConverter::AddPointLoad()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_NODE_K, T_NODE_K> aNodeK;
	pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeK);
	int nNode = aNodeK.GetSize();
	if (nNode == 0) return false;

	std::vector<CBindingData> vBindingData;
	for (int i = 0; i < nNode; ++i)
	{
		const auto NodeK = aNodeK.GetAt(i);

		CArray<T_CNLD_K, T_CNLD_K>  aCnldK;
		CArray<T_CNLD_D, T_CNLD_D&> aCnldD;
		pDoc->m_pAttrCtrl->GetCnld(NodeK, aCnldK, aCnldD);
		int nCnldD = aCnldD.GetSize();
		if (nCnldD == 0) continue;

		for (int j = 0; j < nCnldD; ++j)
		{
			const auto CnldD = aCnldD.GetAt(j);

			CDgnPointLoad* pPL = new CDgnPointLoad;
			pPL->iOrgStaticLcasK = CnldD.LoadCaseKey;
			pPL->adForce[CDgnPointLoad::Fx] = CnldD.Force[CDgnPointLoad::Fx];
			pPL->adForce[CDgnPointLoad::Fy] = CnldD.Force[CDgnPointLoad::Fy];
			pPL->adForce[CDgnPointLoad::Fz] = CnldD.Force[CDgnPointLoad::Fz];
			pPL->adForce[CDgnPointLoad::Mx] = CnldD.Force[CDgnPointLoad::Mx];
			pPL->adForce[CDgnPointLoad::My] = CnldD.Force[CDgnPointLoad::My];
			pPL->adForce[CDgnPointLoad::Mz] = CnldD.Force[CDgnPointLoad::Mz];

			CBindingData Data;
			Data.pData = pPL;

			CRelationalQuery Query;
			Query.DataType = DataType::Node;
			Query.vOnlyOneKey.push_back(NodeK);
			Data.vQueryInfo.push_back(Query);

			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddScaleUpfactorRS()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;
	if (pDoc->m_pAttrCtrl2->ExistSurs())
	{
		T_SURS_D SursD;
		if (pDoc->m_pAttrCtrl2->GetSurs(SursD))
		{

			const INT_PTR nBase = SursD.aSursBase.GetSize();
			for (INT_PTR i = 0; i < nBase; ++i)
			{
				CDgnScaleUpfactorRS* pSURS = new CDgnScaleUpfactorRS;
				pSURS->dFactor = SursD.aSursBase[i].dFactor;

				CBindingData Data;
				Data.ForceKey = std::make_pair(true, SursD.aSursBase[i].SplcK);
				Data.pData = pSURS;

				CRelationalQuery Query;
				Query.DataType = DataType::LoadCase;
				Query.vOnlyOneKey.push_back(SursD.aSursBase[i].SplcK);
				Data.vQueryInfo.push_back(Query);

				vBindingData.push_back(Data);
			}
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddGeneralLineInfo(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddCuttingLine()
{
	// only Gen

	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	if (pServiceHeadOffice == nullptr)
	{
		ASSERT(0);
		return true;
	}
	CArray<UINT, UINT> aCutlK;
	aCutlK.RemoveAll();
	if (!pServiceHeadOffice->ReqService(_ULS(gps), D_GPS_GET_CUTLINE_KEYS, (void*)&aCutlK)) return true;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;
	int nCutl = aCutlK.GetSize();
	for (int i = 0; i < nCutl; i++)
	{
		T_CUTL_K CutlK = aCutlK[i];
		T_CUTL_D CutlD;
		if (!pDoc->m_pAttrCtrl->GetCutl(CutlK, CutlD)) continue;

		CDgnCuttingLine* pCL = new CDgnCuttingLine;
		pCL->strName = CT2W(CutlD.CutLineName);
		for (int j = 0; j < 3; ++j)
		{
			pCL->dPoint1.vec[j] = CutlD.Point1[j];
			pCL->dPoint2.vec[j] = CutlD.Point2[j];
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, CutlK);
		Data.pData = pCL;

		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddNodalCrackedDeform(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_MLCB_D MldbD; MldbD.Initialize();
	if (!pDoc->m_pAttrCtrl->Get_DefaultMlcb(MldbD)) return true;
	const int nCrack = MldbD.aSlabCracK.GetSize();
	if (nCrack == 0) return true;

	std::vector<long> vNode;
	for (const auto iElem : vKeyMemb)
	{
		T_ELEM_D ElemD;
		if (!pDoc->m_pAttrCtrl->GetElem(iElem, ElemD))
		{
			ASSERT(0); continue;
		}
		for (int i = 0; i < D_ELEM_MAXNOD; ++i)
		{
			if (ElemD.elnod[i] == 0) break;
			vNode.push_back(ElemD.elnod[i]);
		}
	}
	Macro::UniqueSort(vNode);

	std::vector<CBindingData> vBindingData;
	for (const auto iNode : vNode)
	{
		CDgnNodalCrackedDeform* pCrackedDeform = new CDgnNodalCrackedDeform;

		for (int j = 0; j < nCrack; ++j)
		{
			const auto LcomK = MldbD.aSlabCracK[j];
			T_DISP_D DispD;
			if (!pDoc->m_pPostCtrl->GetDispCrack(LcomK, iNode, DispD)) continue;
			pCrackedDeform->mDeflection.insert(std::make_pair(LcomK, DispD.dblDisp[2])); // Z-dir
		}
		if (pCrackedDeform->mDeflection.size() == 0)
		{
			_SAFE_DELETE(pCrackedDeform);
			continue;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iNode);
		Data.pData = pCrackedDeform;

		CRelationalQuery Query;
		Query.DataType = DataType::Node;
		Query.vOnlyOneKey.push_back(iNode);
		Data.vQueryInfo.push_back(Query);

		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddColumnPiloti(const std::vector<long>& vKeyMemb)
{
	//T_SCOL_D
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMembTypeUtil Util;
	std::vector<CBindingData> vBindingData;
	for (auto Key : vKeyMemb)
	{
		if (!Util.IsRC(Key)) continue;
		if (!Util.IsColumn(Key)) continue;

		auto* pColumnPiloti = new CDgnColumnPiloti;

		T_SCOL_D ScolD;
		if (!pDoc->m_pAttrCtrl2->GetScol(Key, ScolD)) pColumnPiloti->enPilotiType = CDgnColumnPiloti::enPiloti_Not;
		else
		{
			switch (ScolD.nType)
			{
			case EN_SCOL_PILOTI:
				pColumnPiloti->enPilotiType = CDgnColumnPiloti::enPiloti_Piloti;
				break;
			case EN_SCOL_SOFT_STORY:
				pColumnPiloti->enPilotiType = CDgnColumnPiloti::enPiloti_Soft_Story;
				break;
			default:
				ASSERT(0);
				pColumnPiloti->enPilotiType = CDgnColumnPiloti::enPiloti_Not;
				break;
			}
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, Key);
		Data.pData = pColumnPiloti;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(Key);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddColumnCapital(const std::vector<long>& vKeyMemb)
{
	// T_CCPT_D
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CArray<T_CCPA_K, T_CCPA_K> aCcpaKeyList;
	pDoc->m_pAttrCtrl2->GetCcpaKeyList(aCcpaKeyList);
	int nCcpa = aCcpaKeyList.GetSize();
	if (nCcpa == 0) return true;

	for (int i = 0; i < nCcpa; ++i)
	{
		T_CCPA_D CCpaD;
		if (pDoc->m_pAttrCtrl2->GetCcpa(aCcpaKeyList[i], CCpaD)) // Column Capital Size 반영.
		{
			T_CCPT_K CCptK = CCpaD.CcptK;
			T_CCPT_D CCptD;
			if (pDoc->m_pAttrCtrl2->GetCcpt(CCptK, CCptD))
			{
				auto* pColumnCapital = new CDgnColumnCapital;

				pColumnCapital->dB1 = CCptD.dB1;
				pColumnCapital->dB2 = CCptD.dB2;
				pColumnCapital->dH1 = CCptD.dH1;
				pColumnCapital->dH2 = CCptD.dH2;

				CBindingData Data;
				Data.ForceKey = std::make_pair(true, aCcpaKeyList[i]);
				Data.pData = pColumnCapital;
				CRelationalQuery RQuery;
				RQuery.DataType = DataType::Elem;
				RQuery.vOnlyOneKey.push_back(aCcpaKeyList[i]);
				Data.vQueryInfo.push_back(RQuery);
				vBindingData.push_back(Data);
			}
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddCrackControlCheck(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddSkinBarSpaceCheck(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddBarExposureCondition(const std::vector<long>& vKeyMemb)
{
#if defined _CIVIL
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	int nRCCode = 0;
	T_DCON_D DConD;
	if (DataCtrl.Get_DgnConDcon(DConD))
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);

    CMembTypeUtil Util;
    std::vector<CBindingData> vBindingData;
    for ( auto Key : vKeyMemb )
    {
        if ( !Util.IsRC(Key) ) { continue; }

		T_ELEM_D ElemD; ElemD.Initialize();
		if (!pDoc->m_pPostCtrl->GetElemPost(Key, ElemD))
			continue;
		
		auto* pBarExposure = new CBarExposureCondition;

		switch (nRCCode)
		{
		case AASHTO_LC12:
		case AASHTO_LC16:
		case AASHTO_LC17:
		case AASHTO_LC20:
		case AASHTO_LC24:
			{
				T_RCHK_D RchkD;
				if (pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RchkD))
				{
					pBarExposure->enExpoCondition = [&RchkD]()
						{
							switch (RchkD.nEnvironType)
							{
							case 0:
								return CBarExposureCondition::enExposure::Class1;
							default:
								return CBarExposureCondition::enExposure::Class2;
							}
							return CBarExposureCondition::enExposure::Class2;
						}();
				}
				else
				{
					pBarExposure->enExpoCondition = CBarExposureCondition::enExposure::Class1; /// default value.
				}				
			}
			break;
		case AREMA23:
		{
			T_SERV_D ServD;
			ServD.Initialize();
			if (pDoc->m_pAttrCtrl->GetServ(Key, ServD))
			{
				///TODO 아래 조건 분기가 BS 기준 한정이라 일단 이렇게 처리. 기준별로 분기가 필요할 듯?
				pBarExposure->enExpoCondition = [](int nEnvironment)
					{
						switch (nEnvironment)
						{
						case 0: return CBarExposureCondition::enExposure::Moderate;
						case 1: return CBarExposureCondition::enExposure::Severe;

						default: return CBarExposureCondition::enExposure::Severe;
						}
					}(ServD.iStressEnv);
			}
			else
			{
				pBarExposure->enExpoCondition = CBarExposureCondition::enExposure::Severe;
			}
		}
		break;
		default:
			{
				T_SERV_D ServD;
				ServD.Initialize();
				if (pDoc->m_pAttrCtrl->GetServ(Key, ServD))
				{
					///TODO 아래 조건 분기가 BS 기준 한정이라 일단 이렇게 처리. 기준별로 분기가 필요할 듯?
					pBarExposure->enExpoCondition = [](int nEnvironment)
					{
						switch (nEnvironment)
						{
						case 0: return CBarExposureCondition::enExposure::Moderate;
						case 1: return CBarExposureCondition::enExposure::Severe;
						case 2: return CBarExposureCondition::enExposure::VerySevere;
						case 3: return CBarExposureCondition::enExposure::Extreme;
						default: ASSERT(0); return CBarExposureCondition::enExposure::Severe;
						}
					}(ServD.iCrackEnv);
				}
				else
				{
					pBarExposure->enExpoCondition = CBarExposureCondition::enExposure::Severe;
				}
			}
		}

        CBindingData Data;
        Data.ForceKey = std::make_pair(true, Key);
        Data.pData = pBarExposure;
        CRelationalQuery RQuery;
        RQuery.DataType = DataType::Elem;
        RQuery.vOnlyOneKey.push_back(Key);
        Data.vQueryInfo.push_back(RQuery);
        vBindingData.push_back(Data);
    }

    if ( !SetDataPool(vBindingData) )
    {
        ASSERT(0); return false;
    }

    return true;
#else
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	CDgnDataCtrl DataCtrl;
	for (const auto& iMemb : vKeyMemb)
	{
		T_REXC_D RexcD;
		if (!DataCtrl.Get_DgnConCEnv(iMemb, RexcD)) /// 유효 조건 : 콘크리트이고, beam 요소로 부재별 입력 제한됨.
		{
			RexcD.Initialize();
		}

		auto* pBarExposure = new CBarExposureCondition;
		pBarExposure->enExpoCondition = [](const int nExposure)
		{
			switch (nExposure)
			{
			case 0: return CBarExposureCondition::enExposure::Dry;
			case 1: return CBarExposureCondition::enExposure::Wet;  /// Gen에서는 Dry or Etc.로 입력.
			default: ASSERT(0); return CBarExposureCondition::enExposure::Dry;
			}

		}(RexcD.nExposure);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, iMemb);
		Data.pData = pBarExposure;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Elem;
		RQuery.vOnlyOneKey.push_back(iMemb);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
#endif
}

bool CDgnPerformInputConverter::AddDefStiffRatio(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	std::vector<CBindingData> vBindingData;

	T_KEY_LIST aRCLcomList;
	pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, aRCLcomList);
	int nLcom = aRCLcomList.GetSize();

	// for slab
	CArray<T_SBDO_K, T_SBDO_K> aSbdoList;
	pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoList);
	int nSbdo = aSbdoList.GetSize();
	for (int i = 0; i < nSbdo; ++i)
	{
		T_ELEM_K_LIST aElem;
		int nElem = pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoList[i], aElem);
		for (int j = 0; j < nElem; ++j)
		{
			auto* pDefStiffRat = new CDgnDefStiffRatio;
			for (int k = 0; k < nLcom; ++k)
			{
				T_CRLT_D CrltD; CrltD.Initialize();
				if (pDoc->m_pPostCtrl->ExistCrlt(aRCLcomList[k], aElem[j], CrltD))
				{
					if (pDoc->m_pPostCtrl->GetCrlt(aRCLcomList[k], aElem[j], CrltD))
					{
						StiffRatio data;
						data.OrgLcomKey = aRCLcomList[k];
						data.dRatioXY[0] = CrltD.dRatioXY[0];
						data.dRatioXY[1] = CrltD.dRatioXY[1];
						pDefStiffRat->vStiffRatio.push_back(data);
					}
				}
			}
			CBindingData Data;
			Data.ForceKey = std::make_pair(true, aElem[j]);
			Data.pData = pDefStiffRat;
			CRelationalQuery RQuery;
			RQuery.DataType = DataType::Elem;
			RQuery.vOnlyOneKey.push_back(aElem[j]);
			Data.vQueryInfo.push_back(RQuery);
			vBindingData.push_back(Data);
		}
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddContinuousWall(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddSteelGeneralSectParam(const std::vector<long>& vKeyMemb)
{
	// error 여서 추가 
	return true;
}

bool CDgnPerformInputConverter::AddFatigueParameter(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddDeflectionParameterStl(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDgnDataCtrl DataCtrl;

	std::vector<CBindingData> vBindingData;
	for(const auto MembK : vKeyMemb)
	{
		const auto iElemKind = pDoc->m_pAttrCtrl->GetMemberType(MembK);

        switch ( iElemKind )
        {
        case D_MBTP_COLUMN:
        case D_MBTP_BEAM:
        case D_MBTP_BRACE:
            break;
        default:
            continue;
        }

		T_SERV_D ServD;
		if (!DataCtrl.Get_DgnStlServ(MembK, ServD))
		{
			ServD.dDefLim = DataCtrl.GetDeflectLimByCode(iElemKind, ServD.iDeflecEnv);
		}

		auto* pData = new CDeflectionParameterStl;
		pData->bCheck = true;
		pData->dLimit = ServD.dDefLim;
		pData->dDAF   = ServD.dDAF;

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = pData;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLongitudinalStiffener(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddTransverseStiffener(const std::vector<long>& vKeyMemb)
{
	return true;
}

bool CDgnPerformInputConverter::AddSteelOptions(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	T_DSTL_D DstlD;
	if (!DataCtrl.Get_DgnStlDstl(DstlD)) DstlD.Initialize();

	std::vector<UINT> vAllMemb;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		T_MATD_D MatdD;
		DataCtrl.Get_DgnGenMatd(aElemK[i], MatdD);
		if (MatdD.Type != _T("S"))	continue;

		T_HCBM_K HcbmKey(0);
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
		{
			T_HCBM_D HcbmD;
			if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
			{
				for (int j = 0; j < 3; ++j)
				{
					vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
				}
			}
			else ASSERT(0);
		}
		else
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
			{
				ShowKey = aElemK[i];
			}
			vAllMemb.push_back(ShowKey);
		}
	}
	Macro::UniqueSort(vAllMemb);

	std::vector<CBindingData> vBindingData;
	for ( const auto MembK : vAllMemb/*vKeyMemb*/ )
	{
		auto* pSteelOptions = new CDgnSteelOptions;
		pSteelOptions->Init();

		pSteelOptions->enCombinedForceType = [] (int nCombForcType)
		{
			switch ( nCombForcType )
			{				
			case 0:	return CDgnSteelOptions::enCombinedForce::enCombinedForce_Pos;
			case 1:	return CDgnSteelOptions::enCombinedForce::enCombinedForce_Max;
			default: ASSERT(0); return CDgnSteelOptions::enCombinedForce::enCombinedForce_Pos;				
			}
		}(DstlD.iUseMaxForce);
		
		pSteelOptions->enInteractionFactType = [] (int nKijType)
		{
			switch ( nKijType )
			{
			case EN_EC3_KIJ_BY_CODE: return CDgnSteelOptions::enInteractionFact::enInteractionFact_ByCode;
			case EN_EC3_KIJ_ANNEX_A: return CDgnSteelOptions::enInteractionFact::enInteractionFact_AnnexA;
			case EN_EC3_KIJ_ANNEX_B: return CDgnSteelOptions::enInteractionFact::enInteractionFact_AnnexB;				
			default: ASSERT(0); return CDgnSteelOptions::enInteractionFact::enInteractionFact_ByCode;				
			}
		}(DstlD.nInteractionKijType);
		
		pSteelOptions->bInteractionGeneralEQ = DstlD.bCombinedRatioLinearSum ? true : false;		

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = pSteelOptions;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddImperfectionFactor(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;

	std::vector<UINT> vAllMemb;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aElemK[i], MatdD))
		{
			continue; // User
		}
		if (MatdD.Type != _T("S")) continue;

		T_HCBM_K HcbmKey(0);
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
		{
			T_HCBM_D HcbmD;
			if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
			{
				for (int j = 0; j < 3; ++j)
				{
					vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
				}
			}
			else ASSERT(0);
		}
		else
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
			{
				ShowKey = aElemK[i];
			}
			vAllMemb.push_back(ShowKey);
		}
	}
	Macro::UniqueSort(vAllMemb);

	std::vector<CBindingData> vBindingData;
	for (const auto MembK : vAllMemb/*vKeyMemb*/)
	{
		auto* pImperfectionFactor = new CImperfectionFactor;
		pImperfectionFactor->Init();

		T_BCIF_D BcifD;
		if(DataCtrl.Get_DgnStlBcif(MembK, BcifD))
		{
			pImperfectionFactor->bBuckling_Auto = false;
			pImperfectionFactor->aenBuckling[CImperfectionFactor::enIF_Dir_y] = (CImperfectionFactor::enBUCKCURVE)BcifD.nAlphaY;
			pImperfectionFactor->aenBuckling[CImperfectionFactor::enIF_Dir_z] = (CImperfectionFactor::enBUCKCURVE)BcifD.nAlphaZ;
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = pImperfectionFactor;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddLoadPointMcr(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CDgnDataCtrl DataCtrl;
	T_DSTL_D DstlD;
	if (!DataCtrl.Get_DgnStlDstl(DstlD)) DstlD.Initialize();

	CMembTypeUtil Util;

	std::vector<UINT> vAllMemb;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aElemK[i], MatdD))
		{
			continue; // User
		}
		if (MatdD.Type != _T("S")) continue;

		T_HCBM_K HcbmKey(0);
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
		{
			T_HCBM_D HcbmD;
			if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
			{
				for (int j = 0; j < 3; ++j)
				{
					vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
				}
			}
			else ASSERT(0);
		}
		else
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
			{
				ShowKey = aElemK[i];
			}
			vAllMemb.push_back(ShowKey);
		}
	}
	Macro::UniqueSort(vAllMemb);

	std::vector<CBindingData> vBindingData;
	for (const auto MembK : vAllMemb/*vKeyMemb*/)
	{
		auto* pLoadPointMcr = new CDgnLoadPointMcr;
		pLoadPointMcr->Init();
		
		auto nLoadPositionMcr = DstlD.nLoadPositionMcr;
		T_PMCR_D PmcrD;
		if(pDoc->m_pAttrCtrl->GetPmcr(MembK, PmcrD))
		{
			// 사용자 정의가 있으면 넣어줘야함. DB 추가되면~~
			nLoadPositionMcr = PmcrD.nPMCR_TYPE;
			if(PmcrD.nPMCR_TYPE == EN_STL_EC3_PMCR_USER)
			{
				pLoadPointMcr->dzg = PmcrD.dUserZg;
			}
		}
		else
		{
			if(!Util.IsBeam(MembK)) nLoadPositionMcr = 1; // Global is only beam.
		}

		pLoadPointMcr->enLoadPoint = [](const int& nLoadPositionMcr)
		{
			switch (nLoadPositionMcr)
			{
			case 0: return CDgnLoadPointMcr::enLoadPnt_Top;
			case 1: return CDgnLoadPointMcr::enLoadPnt_ShearCenter;
			case 2: return CDgnLoadPointMcr::enLoadPnt_Bottom;
			case 3: return CDgnLoadPointMcr::enLoadPnt_User;
			default:
				break;
			}
			return CDgnLoadPointMcr::enLoadPnt_ShearCenter;
		}(nLoadPositionMcr);

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = pLoadPointMcr;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddDisp1DRelative(const std::vector<long>& vKeyMemb)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CDgnDataCtrl DataCtrl;

	std::vector<UINT> vAllMemb;
	T_KEY_LIST aElemK;
	pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElem = aElemK.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		T_MATD_D MatdD;
		if (!DataCtrl.Get_DgnGenMatd(aElemK[i], MatdD))
		{
			continue; // User
		}

		T_HCBM_K HcbmKey(0);
		if (pDoc->m_pAttrCtrl->GetHcbmAssigned(aElemK[i], HcbmKey))
		{
			T_HCBM_D HcbmD;
			if (pDoc->m_pAttrCtrl->GetHcbm(HcbmKey, HcbmD))
			{
				for (int j = 0; j < 3; ++j)
				{
					vAllMemb.push_back(HcbmD.aElemListPart[j].GetAt(0));
				}
			}
			else ASSERT(0);
		}
		else
		{
			T_MEMB_K ShowKey(0);
			if (!CMembCtrl::GetMembCtrl()->GetShowElemByIncludeElem(aElemK[i], ShowKey))
			{
				ShowKey = aElemK[i];
			}
			vAllMemb.push_back(ShowKey);
		}
	}
	Macro::UniqueSort(vAllMemb);

	const auto* pLcomList = m_pDataBase->GetLcomList();
	std::vector<CBindingData> vBindingData;
	for (const auto MembK : vAllMemb/*vKeyMemb*/)
	{
		auto* pDisp1DRelative = new CDgnDisp1DRelative;
		pDisp1DRelative->Init();

		const bool bDgnMemb = [&MembK, &vKeyMemb]()
		{
			for (const auto iMemb : vKeyMemb) if(MembK == iMemb) return true;
			return false;
		}();

		if(bDgnMemb)
		{
			int nLcomType = [MembK]()
			{
				switch ( CDgnTypeConverter::GetDgnType(MembK) )
				{
				case enDgn::RCS:
					return D_LCOMTYPE_CONCRETE;
				case enDgn::STL:
					return D_LCOMTYPE_STEEL;
				default:
					ASSERT(0);
					break;
				}
				return D_LCOMTYPE_STEEL;
			}();

			const auto iCantilever = m_pDataBase->GetCantileverType(MembK);
			auto pos = pLcomList->GetStartPosition();
			while(pos)
			{
				T_LCOM_K LcomK;
				_DGN_LCOM LcomD;
				pLcomList->GetNextAssoc(pos, LcomK, LcomD);

				_DGN_DISP Disp;
				Disp.Initialize();
				DataCtrl.Get_DgnGenDisp(MembK, nLcomType, LcomD.OriginalLcomKey, Disp, iCantilever, D_UNITSYS_LENGTH_INDEX_M, TRUE); // neutral unit

				tDisp DispD;
				DispD.dDisp[0] = Disp.MaxDispX;
				DispD.dDisp[1] = Disp.MaxDispY;
				DispD.dDisp[2] = Disp.MaxDispZ;
				DispD.dDist[0] = Disp.MaxDistX;
				DispD.dDist[1] = Disp.MaxDistY;
				DispD.dDist[2] = Disp.MaxDistZ;

				const auto key = std::make_tuple(LcomD.OriginalLcomKey, LcomD.bMaxLcom, LcomD.iLcomCompType);
				pDisp1DRelative->mDeflection[key] = DispD;
			}
		}

		CBindingData Data;
		Data.ForceKey = std::make_pair(true, MembK);
		Data.pData = pDisp1DRelative;
		CRelationalQuery RQuery;
		RQuery.DataType = DataType::Memb;
		RQuery.vOnlyOneKey.push_back(MembK);
		Data.vQueryInfo.push_back(RQuery);
		vBindingData.push_back(Data);
	}

	if (!SetDataPool(vBindingData))
	{
		ASSERT(0); return false;
	}

	return true;
}

bool CDgnPerformInputConverter::AddResponseModificationFactor(const std::vector<long>& vKeyMemb)
{
    // Response Modification Factor
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    CDgnDataCtrl DataCtrl;
    std::vector<CBindingData> vBindingData;
    for (auto Key : vKeyMemb)
    {
        T_RMFR_D RmfrD; RmfrD.Initialize();
        if (!pDoc->m_pAttrCtrl->GetRmfr(Key, RmfrD))	continue;

        auto* pDgnRMFactor = new CDgnResponseModificationFactor;

        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RFx] = RmfrD.dFx;
        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RFy] = RmfrD.dFy;
        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RFz] = RmfrD.dFz;
        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RMx] = RmfrD.dMx;
        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RMy] = RmfrD.dMy;
        pDgnRMFactor->adFactor[CDgnResponseModificationFactor::RMz] = RmfrD.dMz;

        CBindingData Data;
        Data.ForceKey = std::make_pair(true, Key);
        Data.pData = pDgnRMFactor;

        CRelationalQuery RQuery;
        RQuery.DataType = DataType::Elem;
        RQuery.vOnlyOneKey.push_back(Key);
        Data.vQueryInfo.push_back(RQuery);
        vBindingData.push_back(Data);
    }

    if (!SetDataPool(vBindingData))
    {
        ASSERT(0); return false;
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double CDgnPerformInputConverter::Get_Es(const int& iDgnCode, const double& dfy) const
{
	double dEs = 0.0;

	// neutral unit
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);

	switch (iDgnCode)
	{
	case ACI318_89:
	case ACI318_95:
	case ACI318_99:
	case ACI318_02:
	case ACI318_05:
	case ACI318_08:
	case ACI318_11:
	case ACI318_14: /// 20.2.2.2.
	case ACI318_19:
	case ACI318_25:
	case AREMA23:// same as AASHTO
    case AASHTO_LC20:
	case AASHTO_LC24:
		dEs = 2.90*(1.0E+07);	// lb/in2.	
		dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_LBF, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_STRESS, 1.0);
		break;
	case ACI318M_14:
	case ACI318M_19:
	case ACI318M_25:
	case NSR_10: /// see NSR-10, C.8.5.2.
	case NSCP_2015_C: /// see [NSCP 2015, 420.2.2.2]
	case NTC_DCEC_2023:
		dEs = 2.00*(1.0E+05);
		break;
	case AIK_USD94:
	case AIK_WSD2K:
		dEs = 2.10*(1.0E+06);	// kgf/cm2.
		dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_STRESS, 1.0);
		break;
	case KSCE_USD96:
		dEs = 2.04*(1.0E+06);	// kgf/cm2.
		dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_STRESS, 1.0);
		break;
	case KCI_USD99:
		dEs = 2.00*(1.0E+06);	// kgf/cm2.
		dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_STRESS, 1.0);
		break;
	case KCI_USD03: // N/mm2, See KCI-USD03 3.4.3(2) p.67.
	case KCI_USD07:
	case KCI_USD12:
	case KDS_41_30_2018: /// see KDS 14 20 10 : 2018, 4.3(2)
	case KDS_41_20_2022:
		dEs = 2.00*(1.0E+05);
		break;
	case TWN_USD92: // kgf/cm2, See TWN-USD92 1.9.2 p.1-3.
	case TWN_USD100:
	case TWN_USD112:
		dEs = 2.04*(1.0E+06);
		dNeutralLen = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_STRESS, 1.0);
		break;
	case AIJ_WSD99:
		dEs = 2.05*(1.0E+05);	// N/mm2.
		break;
	case BS8110_97:
	case Eurocode2:
	case Eurocode2_04:
	case Eurocode2_2_05: // PMS.5537 Civil Shell Design Eurocode2_2_05
	case IS456_2000:  // N/mm2, See IS456:2000 5.6.3 p.15.
	case CSA_A233_94: // N/mm2, See CSA-A23.3-94 8.5.4.1 p.19.
	case BS5400_4_90: 
	case TMH07_89:
		dEs = 2.00*(1.0E+05);	// N/mm2.
		break;
	case GB50010_02: // N/mm2, See GB50010-02 4.2.4 p.21.
		dEs = dgn::EQ(dfy, 235.0 / dNeutralLen) ? 2.1*(1.0E+05) : 2.0*(1.0E+05);  /// HPB235 | ETC.    
		break;
	case GB50010_10: // N/mm2, See [GB/T50010-10, 4.2.4, Table 4.2.4]
	case GB50010_19:
		dEs = dgn::EQ(dfy, 300.0 / dNeutralLen) ? 2.1*(1.0E+05) : 2.0*(1.0E+05); /// HPB300 | ETC.
		break;
	default: ASSERT(0);
		break;
	}

	return dEs * dNeutralLen;
}

std::pair<double, double> CDgnPerformInputConverter::GetRCMainRebarFyEs(T_ELEM_K ElemK)
{
	int nRCCode = 0;
	T_DCON_D DConD;
	CDgnDataCtrl DataCtrl;
	if (DataCtrl.Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	T_MATD_D MatdD;
	if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD))
	{
		return std::make_pair(0.0, 0.0); // User
	}

	return std::make_pair(MatdD.MainRebarData.B_fy, Get_Es(nRCCode, MatdD.MainRebarData.B_fy));
}

std::pair<double, double> CDgnPerformInputConverter::GetRCSubRebarFyEs(T_ELEM_K ElemK)
{
	int nRCCode = 0;
	T_DCON_D DConD;
	CDgnDataCtrl DataCtrl;
	if (DataCtrl.Get_DgnConDcon(DConD))
	{
		nRCCode = CDgnCodeCtrl::GetConCodeID(DConD.DesignCode);
	}

	T_MATD_D MatdD;
	if (!DataCtrl.Get_DgnGenMatd(ElemK, MatdD))
	{
		return std::make_pair(0.0, 0.0); // User
	}

	return std::make_pair(MatdD.SubRebarData.B_fy, Get_Es(nRCCode, MatdD.SubRebarData.B_fy));
}

bool CDgnPerformInputConverter::CalcMembLength1D(const T_ELEM_K ElemK, OUT double& rdLeng, OUT double& rdNetLy, OUT double& rdNetLz)
{
	rdLeng = 0.0;
	rdNetLy = 0.0;
	rdNetLz = 0.0;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();

	T_HCBM_K HcbmK = 0;
	const BOOL bHaunched = pDoc->m_pAttrCtrl->GetHcbmAssigned(ElemK, HcbmK);

	BOOL bReverseDir = FALSE;
	T_ELEM_K ShowKey = 0;
	T_ELEM_K_LIST aElemK;
	if (bHaunched)
	{
		if (!pMembCtrl->GetElemListByIncludeHcbm(ElemK, ShowKey, aElemK, TRUE)) { return false; }
	}
	else
	{
		if (!pMembCtrl->GetElemListByIncludeElem(ElemK, bReverseDir, ShowKey, aElemK, TRUE)) { return false; }
	}

	if (aElemK.GetSize() == 1)
	{
		T_ELEM_K CurElemK = aElemK[0];
		const double dTotalLeng = pDoc->calcLAVElem(CurElemK);

		rdLeng = dTotalLeng;
		rdNetLy = dTotalLeng;
		rdNetLz = dTotalLeng;

		T_CFMO_D CfmoD;
		if (pDoc->m_pPostCtrl->GetCfmo(CurElemK, CfmoD))
		{
			if (CfmoD.nOffsetType == 1)	// Local Offset.
			{
				rdNetLy -= (CfmoD.dOffset[2] + CfmoD.dOffset[3]);
				rdNetLz -= (CfmoD.dOffset[4] + CfmoD.dOffset[5]);
			}
		}
	}
	else /// member assigned.
	{
		double dTotalLeng = 0.0;
		if (bHaunched)
		{
			dTotalLeng = pMembCtrl->GetHcbmLengByIncludeElem(ShowKey);
		}
		else
		{
			dTotalLeng = pMembCtrl->GetMembLengByIncludeElem(ShowKey);
		}

		rdLeng = dTotalLeng;
		rdNetLy = dTotalLeng;
		rdNetLz = dTotalLeng;

		T_ELEM_K stElemK = bReverseDir ? aElemK.GetAt(aElemK.GetSize() - 1) : aElemK.GetAt(0);
		T_CFMO_D stCfmoD;
		if (pDoc->m_pPostCtrl->GetCfmo(stElemK, stCfmoD))
		{
			if (stCfmoD.nOffsetType == 1)	// Local Offset.
			{
				rdNetLy -= (bReverseDir ? stCfmoD.dOffset[3] : stCfmoD.dOffset[2]);	// Member-I.
				rdNetLz -= (bReverseDir ? stCfmoD.dOffset[5] : stCfmoD.dOffset[4]);	// Member-I.
			}
		}
		// End Elem.
		T_ELEM_K edElemK = (bReverseDir ? aElemK.GetAt(0) : aElemK.GetAt(aElemK.GetSize() - 1));
		T_CFMO_D edCfmoD;
		if (pDoc->m_pPostCtrl->GetCfmo(edElemK, edCfmoD))
		{
			if (edCfmoD.nOffsetType == 1)	// Local Offset.
			{
				rdNetLy -= (bReverseDir ? edCfmoD.dOffset[2] : edCfmoD.dOffset[3]);	// Member-J.
				rdNetLz -= (bReverseDir ? edCfmoD.dOffset[4] : edCfmoD.dOffset[5]);	// Member-J.
			}
		}
	}

	return true;
}

bool CDgnPerformInputConverter::CalcMembLengthWall(const T_ELEM_K ElemK, OUT double& rdLeng)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	UINT unInternalWallID = 0;
	if (!pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID))
	{
		ASSERT(0);
		return false;
	}

	T_WALL_KEY WallKey;
	if (!pDoc->m_pPostCtrl->GetWallKey(unInternalWallID, WallKey))
	{
		ASSERT(0);
		return false;
	}

	T_WALL_D WallD;
	if (!pDoc->m_pPostCtrl->GetWallData(WallKey, WallD))
	{
		ASSERT(0);
		return false;
	}

	rdLeng = WallD.dHeight;
	return true;
}