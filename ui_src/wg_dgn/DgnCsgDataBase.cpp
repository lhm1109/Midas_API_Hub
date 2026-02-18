#include "stdafx.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"

#include "..\..\dgnengine\idesign\DGN_lib\Macro.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeISymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeBoxSymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\Align2D.h"
#include "..\..\dgnengine\idesign\DGN_lib\DgnThreadPool.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeRectangle.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeT.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeTubSymY.h"
#include "..\..\dgnengine\idesign\DGN_lib\AlignedShapeURib.h"
#include "..\..\dgnengine\idesign\DGN_lib\IDgnBaseInternalLib.h"

#include "..\..\dgnengine\idesign\DGN_link\SectionBuilder.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnDocCtrl.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnDoc.h"
#include "..\..\dgnengine\idesign\DGN_link\CalcType.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnQuery.h"
#include "..\..\dgnengine\idesign\DGN_link\IDgnMemoizationLib.h"

#include "..\..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MemberInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\EffectiveLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SteelGeneralSectParamDef.h"
#include "..\..\dgnengine\idesign\DGN_def\BendingCoefficientDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearCoefficientDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SafetyFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SeismicPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\UnbracedLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LcomInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TResult.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemUtil.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemCompoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MembTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnRptCriticalInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionShapeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\PartialFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MembLengthDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ForceUblnDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SteelCompositeSectionDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TSectionShape.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialConcreteDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialSteelHybridDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnItemDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DesignCaseDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnReportInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DArrayDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MaterialSteelDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LoadApplicationTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressCheckDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MomentLoadType.h"
#include "..\..\dgnengine\idesign\DGN_def\TorsionDesignDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnSteelOptionsDef.h"
#include "..\..\dgnengine\idesign\DGN_def\LateralTorsionalBucklingDef.h"
#include "..\..\dgnengine\idesign\DGN_def\IShearConnectorSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearConnectorSingleStud.h"
#include "..\..\dgnengine\idesign\DGN_def\ShearConnectorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MainRebarSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\RebarBaseDef.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionRebarCoordDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TStiffenerSingle.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerRectangle.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerT.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffenerURib.h"
#include "..\..\dgnengine\idesign\DGN_def\LongitudinalStiffenerDef.h"
#include "..\..\dgnengine\idesign\DGN_def\TransverseStiffenerDef.h"
#include "..\..\dgnengine\idesign\DGN_def\Force1DMemberDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DamageEquivalenceFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DamageEquivalenceFactorMembDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressLimitationFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StressSteelCompositeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\MemoizationKeyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ImperfectionFactorDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DesignCodeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\CompositeSectionPropertyDef.h"
#include "..\..\dgnengine\idesign\DGN_def\RptSettingsDef.h"
#include "..\..\dgnengine\idesign\DGN_def\RptDataTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\DgnRptCriticalInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\PanelInfoDef.h"
#include "..\..\dgnengine\idesign\DGN_def\ResidualStressTypeDef.h"
#include "..\..\dgnengine\idesign\DGN_def\StiffnessScaleFactorDef.h"

#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"

#include "..\wg_base\wg_base_UtilFunc.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PlateGirderDesign.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_dbLock\LockMgr.h"

#include "DgnForceCtrl.h"
#include "DgnCsgDataCtrl.h"
#include "CRCForceCtrl.h"
#include "Dgn_CSGManager.h"
#include "Dgn_CSGManager_AASHTO12.h"
#include "Dgn_CSGManager_CSA_S6_14.h"
#include "Dgn_CSGManager_KSCE_LSD15.h"
#include "Dgn_CSGManager_KSSC_2014.h"
#include "Dgn_CSGManager_CS457_R1.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"
#include "DgnTypeConverter.h"
#include "DgnSectUtil.h"

#include "DgnPscCommon.h"
#include "DgnCsgDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

using namespace dgn::lib;
using namespace dgn::link;
using namespace dgn::def;

using namespace dgn::converter;

CDgnCsgDataBase::CDgnCsgDataBase() : CDgnPscCommon(), IDgnDataBase(), IDgnCodeCheck(( IDgnDataBase* ) this)
{
	m_pDgnSectUtil = new CDgnSectUtil();

	m_pMembCtrl = CMembCtrl::GetMembCtrl();

	m_CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);

	m_StctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetStct(m_StctD);

	m_bReport = false;
}

CDgnCsgDataBase::~CDgnCsgDataBase()
{
	if ( m_pDgnSectUtil != nullptr )
	{
		delete m_pDgnSectUtil;
	}
}

enDgnCode CDgnCsgDataBase::GetDgnCode() const
{
	return dgn::converter::CDgnTypeConverter::GetDgnCode(m_iCivilCode, m_nNatinoAnnex);
}

enDgn CDgnCsgDataBase::GetDgnType() const
{
	return dgn::def::enDgn::CSG;
}

enMemb CDgnCsgDataBase::GetDgnMembType() const
{
	return dgn::def::enMemb::Beam;
}

UINT CDgnCsgDataBase::GetLcomSize()
{
	UINT unLcomNum = 1; // 하나씩 돌린다?
	return unLcomNum;
}

std::wstring CDgnCsgDataBase::GetDgnCodeName()
{
	CString strCodeName = CDBLib::GetCsgCodeName(m_iCivilCode);
	return static_cast< std::wstring >( CT2W(strCodeName) );
}

std::wstring CDgnCsgDataBase::GetDgnElemName()
{
	CString sElem; sElem.Format(_T("%d"), GetCheckElem());
	return static_cast< std::wstring >( CT2W(sElem) );
}

std::wstring CDgnCsgDataBase::GetDgnPosiName()
{
	CString strPosiNo = m_pForcCtrl->m_pDgnForceCtrl->Get_PosiNa(GetCheckPosition());
	std::wstring sPosi = static_cast< std::wstring >( CT2W(strPosiNo) );

	return sPosi;
}

std::wstring CDgnCsgDataBase::GetDgnLcomName()
{
	CString strLcomName = _T(""); //???
	return static_cast< std::wstring >( CT2W(strLcomName) );
}

std::wstring CDgnCsgDataBase::GetDgnPosiName(int nPos)
{
	CString strPosName = m_pForcCtrl->m_pDgnForceCtrl->Get_PosiNa(nPos);
	std::wstring sPosi = static_cast< std::wstring >( CT2W(strPosName) );

	return sPosi;
}

std::wstring CDgnCsgDataBase::GetDgnLcomName(CString strLcom)
{
	return static_cast< std::wstring >( CT2W(strLcom) );
}

void CDgnCsgDataBase::GetDgnCurrentUnit(std::wstring& ForCU, std::wstring& LenCU)
{
	T_UNIT_INDEX CodeUnit = GetCodeUnit();

	CString strForU = m_pDoc->m_pUnitCtrl->Get_ForUnit(CodeUnit.nBase_Force);
	CString strLenU = m_pDoc->m_pUnitCtrl->Get_LenUnit(CodeUnit.nBase_Length);

	ForCU = static_cast< std::wstring >( CT2W(strForU) );
	LenCU = static_cast< std::wstring >( CT2W(strLenU) );
}

void CDgnCsgDataBase::GetRptSettings(const CString& strMembTypeName, dgn::def::CRptSettings& rSettings)
{
	rSettings.Init();

	CString strName = m_pDoc->GetPathName();

	enReportType enRptType = enReportType::None;
	enReportLevel enRptLevel = enReportLevel::Detail;
	if (CLockMgr::Get_IsRegressionTest() )
	{
		enRptType = enReportType::PlainText;
		enRptLevel = enReportLevel::Regression;

		CFileCtrl FileCtrl(m_strTestDgnFile);
		strName = FileCtrl.GetFilePathAndNameWithoutExtension();
	}
	else
	{
		enRptType = enReportType::WinWordRTF;

		CString strExt = _T("");
		strExt.Format(_T("%s.doc"), strMembTypeName);

		CFileCtrl FileCtrl(strName);
		strName = FileCtrl.GetFilePathAndNameWithoutExtension() + strExt;
		if ( FileCtrl.FileExists(strName) )
		{
			strName = IDgnBaseInternalLib::Instance()->GetNewStringName(strName);
		}
	}

	const wstring strFileName = CT2W(strName);

	rSettings.SetUserPref(true);
	rSettings.SetDgnCode(GetDgnCode());
	rSettings.SetDgnType(enDgn::CSG);
	rSettings.SetFileOutType(enRptType);
	rSettings.SetFileOutPath(strFileName);
	rSettings.SetReportLevel(enRptLevel);
	rSettings.SetRptUnit(enPredefinedCode);

	rSettings.SetDesignCase(INTEGRATIONDESIGNCASE);
	rSettings.SetMinSigFig(3);
	rSettings.SetReportLanguage(CDgnCodeCtrl::GetCpgLocalePref());
}

bool CDgnCsgDataBase::GetDesignCode(dgn::def::CDesignCode* pData) const
{
	T_UNIT_INDEX CodeUnit = GetCodeUnit();
	pData->STL = GetDgnCode();
	pData->STL_unitL = static_cast< enLengthUnit >( CodeUnit.nBase_Length );
	pData->STL_unitF = static_cast< enForceUnit >( CodeUnit.nBase_Force );

	return true;
}

bool CDgnCsgDataBase::GetMemberInfo(dgn::def::CMemberInfo* pData) const
{
	UINT nElemNo = GetDgnElemNo();
	pData->Key = static_cast< long >( nElemNo );
	pData->DgnType = GetDgnType();
	pData->MembType = GetDgnMembType();
	return true;
}

bool CDgnCsgDataBase::GetMemberInfoArray(dgn::def::CMemberInfoArray* pData) const
{
	dgn::def::CMemberInfo MembInfo;
	GetMemberInfo(&MembInfo);
	pData->vMembInfo.push_back(MembInfo);

	return true;
}

bool CDgnCsgDataBase::GetSection(dgn::def::CSection* pData) const
{
	return true;
}

bool CDgnCsgDataBase::GetMaterial(dgn::def::CMaterial* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_MATL_ELEM MatlElem;
	if ( m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) )
	{
		pData->strCode = MatlElem.MatStf.csCodeName;
		pData->strName = MatlElem.MatStf.csMatlName;
	}

	return true;
}

bool CDgnCsgDataBase::GetSectionProperty(dgn::def::CSectionProperty* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	const auto& itSect4E = m_mCsgElemSect.find(EPairK);
	if (itSect4E != m_mCsgElemSect.end())
	{
		const auto& itSect = m_mCsgSect.find(itSect4E->second);
		if (itSect != m_mCsgSect.end())
		{
			UINT nPos = GetCheckPosition();

			const DGN_SECT_STIFFNESS& GirderStiff = itSect->second.SectPos[nPos].SectInfo.Before.Stiffness;

			ConvertDgnSectStiffToSectionProperty(GirderStiff, pData);
		}
	}

	return true;
}

bool CDgnCsgDataBase::GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_SECT_ELEM SectElem;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	UINT nPos = GetCheckPosition();

	const DGN_SECT_STIFFNESS& RebarSect = SectElem.SectPos[nPos].RbarStiffn;
	const DGN_SECT_STIFFNESS& RebarSectLT = SectElem.SectPos[nPos].RbarStiff3n;
	const DGN_SECT_STIFFNESS& NonSlab   = SectElem.SectPos[nPos].NonSlabStiff;
	const DGN_SECT_STIFFNESS& NonSlabR3 = SectElem.SectPos[nPos].NonSlabStiff;  // NonSlabStiff2 이걸 사용 하면 바꾸자.

	pData->adProp[dgn::def::CSectionProperty::Area] = RebarSect.dArea;
	pData->adProp[dgn::def::CSectionProperty::J]    = RebarSect.dIxx;
	pData->adProp[dgn::def::CSectionProperty::Ix]   = RebarSect.dIyy;
	pData->adProp[dgn::def::CSectionProperty::Iy]   = RebarSect.dIzz;
	//pData->adProp[dgn::def::CSectionProperty::Ixy]  = GirderDgnStiff.
	//pData->adProp[dgn::def::CSectionProperty::Sx] = GirderDgnStiff.Syy;
	//pData->adProp[dgn::def::CSectionProperty::Sy] = GirderDgnStiff.Szz;
	//pData->adProp[dgn::def::CSectionProperty::Zx] = GirderDgnStiff.Zyy;
	//pData->adProp[dgn::def::CSectionProperty::Zy] = GirderDgnStiff.Zzz;
	pData->adProp[dgn::def::CSectionProperty::Cx]  = RebarSect.dCym;
	pData->adProp[dgn::def::CSectionProperty::Cxm] = RebarSect.dCym;
	pData->adProp[dgn::def::CSectionProperty::Cxp] = RebarSect.dCyp;
	pData->adProp[dgn::def::CSectionProperty::Cy]  = RebarSect.dCzm;
	pData->adProp[dgn::def::CSectionProperty::Cym] = RebarSect.dCzm;
	pData->adProp[dgn::def::CSectionProperty::Cyp] = RebarSect.dCzp;
	pData->adProp[dgn::def::CSectionProperty::x1]  = RebarSect.dx1;
	pData->adProp[dgn::def::CSectionProperty::y1]  = RebarSect.dy1;
	pData->adProp[dgn::def::CSectionProperty::x2]  = RebarSect.dx2;
	pData->adProp[dgn::def::CSectionProperty::y2]  = RebarSect.dy2;
	pData->adProp[dgn::def::CSectionProperty::x3]  = RebarSect.dx3;
	pData->adProp[dgn::def::CSectionProperty::y3]  = RebarSect.dy3;
	pData->adProp[dgn::def::CSectionProperty::x4]  = RebarSect.dx4;
	pData->adProp[dgn::def::CSectionProperty::y4]  = RebarSect.dy4;
	//pData->adProp[dgn::def::CSectionProperty::Xbar] = GirderDgnStiff.YBar;
	pData->adProp[dgn::def::CSectionProperty::Ybar] = RebarSect.dCzm;
	//pData->adProp[dgn::def::CSectionProperty::Rox] = EQ0(GirderDgnStiff.Roy) ? sqrt(SafeDiv(RebarSect.Ryy, RebarSect.Area)) : GirderDgnStiff.Roy;
	//pData->adProp[dgn::def::CSectionProperty::Roy] = EQ0(GirderDgnStiff.Roz) ? sqrt(SafeDiv(RebarSect.Rzz, RebarSect.Area)) : GirderDgnStiff.Roz;
	//pData->adProp[dgn::def::CSectionProperty::Rop] = GirderDgnStiff.Rop;
	pData->adProp[dgn::def::CSectionProperty::Asx] = RebarSect.dAsy;
	pData->adProp[dgn::def::CSectionProperty::Asy] = RebarSect.dAsz;
	//pData->adProp[dgn::def::CSectionProperty::Cw] = GirderDgnStiff.Cwp;
	//     pData->adProp[dgn::def::CSectionProperty::Qxb] = m_Qyb[m_iPosiNo];
	//     pData->adProp[dgn::def::CSectionProperty::Qyb] = m_Qzb[m_iPosiNo];

	pData->adPropLT[dgn::def::CSectionProperty::Area] = RebarSectLT.dArea;
	pData->adPropLT[dgn::def::CSectionProperty::J]    = RebarSectLT.dIxx;
	pData->adPropLT[dgn::def::CSectionProperty::Ix]   = RebarSectLT.dIyy;
	pData->adPropLT[dgn::def::CSectionProperty::Iy]   = RebarSectLT.dIzz;
	pData->adPropLT[dgn::def::CSectionProperty::Cx]   = RebarSectLT.dCym;
	pData->adPropLT[dgn::def::CSectionProperty::Cxm]  = RebarSectLT.dCym;
	pData->adPropLT[dgn::def::CSectionProperty::Cxp]  = RebarSectLT.dCyp;
	pData->adPropLT[dgn::def::CSectionProperty::Cy]   = RebarSectLT.dCzm;
	pData->adPropLT[dgn::def::CSectionProperty::Cym]  = RebarSectLT.dCzm;
	pData->adPropLT[dgn::def::CSectionProperty::Cyp]  = RebarSectLT.dCzp;
	pData->adPropLT[dgn::def::CSectionProperty::x1]   = RebarSectLT.dx1;
	pData->adPropLT[dgn::def::CSectionProperty::y1]   = RebarSectLT.dy1;
	pData->adPropLT[dgn::def::CSectionProperty::x2]   = RebarSectLT.dx2;
	pData->adPropLT[dgn::def::CSectionProperty::y2]   = RebarSectLT.dy2;
	pData->adPropLT[dgn::def::CSectionProperty::x3]   = RebarSectLT.dx3;
	pData->adPropLT[dgn::def::CSectionProperty::y3]   = RebarSectLT.dy3;
	pData->adPropLT[dgn::def::CSectionProperty::x4]   = RebarSectLT.dx4;
	pData->adPropLT[dgn::def::CSectionProperty::y4]   = RebarSectLT.dy4;
	pData->adPropLT[dgn::def::CSectionProperty::Ybar] = RebarSectLT.dCzm;
	pData->adPropLT[dgn::def::CSectionProperty::Asx]  = RebarSectLT.dAsy;
	pData->adPropLT[dgn::def::CSectionProperty::Asy]  = RebarSectLT.dAsz;

	pData->adPropNonSlab[dgn::def::CSectionProperty::Area] = NonSlab.dArea;
	pData->adPropNonSlab[dgn::def::CSectionProperty::J]    = NonSlab.dIxx;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Ix]   = NonSlab.dIyy;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Iy]   = NonSlab.dIzz;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cx]   = NonSlab.dCym;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cxm]  = NonSlab.dCym;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cxp]  = NonSlab.dCyp;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cy]   = NonSlab.dCzm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cym]  = NonSlab.dCzm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Cyp]  = NonSlab.dCzp;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x1]   = NonSlab.dx1;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y1]   = NonSlab.dy1;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x2]   = NonSlab.dx2;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y2]   = NonSlab.dy2;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x3]   = NonSlab.dx3;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y3]   = NonSlab.dy3;
	pData->adPropNonSlab[dgn::def::CSectionProperty::x4]   = NonSlab.dx4;
	pData->adPropNonSlab[dgn::def::CSectionProperty::y4]   = NonSlab.dy4;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Ybar] = NonSlab.dCzm;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Asx]  = NonSlab.dAsy;
	pData->adPropNonSlab[dgn::def::CSectionProperty::Asy]  = NonSlab.dAsz;

	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Area] = NonSlabR3.dArea;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::J]    = NonSlabR3.dIxx;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Ix]   = NonSlabR3.dIyy;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Iy]   = NonSlabR3.dIzz;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cx]   = NonSlabR3.dCym;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cxm]  = NonSlabR3.dCym;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cxp]  = NonSlabR3.dCyp;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cy]   = NonSlabR3.dCzm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cym]  = NonSlabR3.dCzm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Cyp]  = NonSlabR3.dCzp;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x1]   = NonSlabR3.dx1;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y1]   = NonSlabR3.dy1;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x2]   = NonSlabR3.dx2;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y2]   = NonSlabR3.dy2;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x3]   = NonSlabR3.dx3;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y3]   = NonSlabR3.dy3;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::x4]   = NonSlabR3.dx4;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::y4]   = NonSlabR3.dy4;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Ybar] = NonSlabR3.dCzm;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Asx]  = NonSlabR3.dAsy;
	pData->adPropNonSlabR3[dgn::def::CSectionProperty::Asy]  = NonSlabR3.dAsz;

	return true;
}

bool CDgnCsgDataBase::GetMaterialSteel(dgn::def::CMaterialSteel* pData) const
{
	UINT nPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	const double dFu = SMat.S_Fu;
	TMaterialSteelSingle Single1(SMat.S_Fy1, dFu);
	TMaterialSteelSingle Single2(SMat.S_Fy2, dFu);
	TMaterialSteelSingle Single3(SMat.S_Fy3, dFu);
	TMaterialSteelSingle Single4(SMat.S_Fy4, dFu);
	TMaterialSteelSingle Single5(SMat.S_Fy5, dFu);
	TMaterialSteelSingle Single6(SMat.S_Fy6, dFu);

	CSG_SECT_ELEM SectElem;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[nPos];

	double dSteelThkWeb = GetTw(SectPos);
	double dSteelThkFlangeTop = GetTfTop(SectPos);
	double dSteelThkFlangeBot = GetTfBot(SectPos);

	double dMinThick = min(min(dSteelThkFlangeTop, dSteelThkFlangeBot), dSteelThkWeb);

	pData->Data.Common.dElastic = SMat.Elast;
	pData->Data.Common.dShear = EQ0(SMat.Shear) ? 80000.0 : SMat.Shear; // 80000 MPa 임시
	//pData->Data.Common.dShear = MatdD.Data1.Analysis.ShearModulus;
	pData->Data.Common.dPoisson = SMat.Poisson;
	pData->Data.Common.dWeightDensity = SMat.Density;
	pData->Data.Steel.nList = 1;
	pData->Data.Steel.bUser = ( MatlElem.MatStf.csCodeName.CompareNoCase(_T("None")) == 0 );

	pData->Data.Steel.aProp[0] = SteelThickProp(dMinThick, Single1);
	pData->Data.Steel.aProp[1] = SteelThickProp(dMinThick, Single2);
	pData->Data.Steel.aProp[2] = SteelThickProp(dMinThick, Single3);
	pData->Data.Steel.aProp[3] = SteelThickProp(dMinThick, Single4);
	pData->Data.Steel.aProp[4] = SteelThickProp(dMinThick, Single5);
	pData->Data.Steel.aProp[5] = SteelThickProp(dMinThick, Single6);

	//pData->ConcD.dfck = MatlElem.dfck;
	//pData->ConcD.dfy  = MatdD.MainRebarData2.B_fy;
	//pData->ConcD.dEc  = MatdD.Data2.Analysis.Elast;

	return true;
}

bool CDgnCsgDataBase::GetEffectiveLength(dgn::def::CEffectiveLength* pData) const
{
	T_KFAC_D KFacD; KFacD.Initialize();

	bool bKfac = false;

	if ( m_pDoc->m_pAttrCtrl->GetKfac(GetCheckElem(), KFacD) )
	{
		bKfac = TRUE;
	}

	if ( bKfac )
	{
		pData->dKy = KFacD.BKy;
		pData->dKz = KFacD.BKz;
	}
	else
	{
		pData->dKy = pData->dKz = 1.0;
	}

	return true;
}

bool CDgnCsgDataBase::GetSteelGeneralSectParam(dgn::def::CSteelGeneralSectParam* pData) const
{
	pData->bGeneral = false; // only for nGen
	//     pData->adProp[dgn::def::CSteelGeneralSectParam::Compression] = m_UFa;
	//     pData->adProp[dgn::def::CSteelGeneralSectParam::Tension] = m_UFt;
	//     pData->adProp[dgn::def::CSteelGeneralSectParam::Bending] = m_UFby;
	//     pData->adProp[dgn::def::CSteelGeneralSectParam::ShearAwx] = m_UFbz;
	//     pData->adProp[dgn::def::CSteelGeneralSectParam::ShearAwy] = m_UFv;

	return true;
}

bool CDgnCsgDataBase::GetBendingCoefficient(dgn::def::CBendingCoefficient* pData) const
{
	//T_CBFT_D CbftD;
	//CbftD.Initialize();
	//m_pDgnDataCtrl->Get_DgnStlCbft(m_iElemNo, CbftD);

	//pData->Factor.v1 = CbftD.bAuto;
	//pData->Factor.v2 = (CbftD.bAuto ? -1.0 : CbftD.Cb);
	return true;
}

bool CDgnCsgDataBase::GetShearCoefficient(dgn::def::CShearCoefficient* pData) const
{
	//     T_CVFT_D CvftD;
	//     CvftD.Initialize();
	//     m_pDgnDataCtrl->Get_DgnStlCvft(m_iElemNo, CvftD);
	//     pData->Factor.v1 = CvftD.bAuto;
	//     pData->Factor.v2 = (CvftD.bAuto ? 0.0 : CvftD.Cv
	return true;
}

bool CDgnCsgDataBase::GetSafetyFactor(dgn::def::CSafetyFactor* pData) const
{
	pData->aFactor[dgn::def::CSafetyFactor::Tension]     = dgn::lib::TFactor(false, m_CpgdD.dPhi_c);
	pData->aFactor[dgn::def::CSafetyFactor::Compression] = dgn::lib::TFactor(false, m_CpgdD.dPhi_c);
	pData->aFactor[dgn::def::CSafetyFactor::Bending]     = dgn::lib::TFactor(false, m_CpgdD.dPhi_f);
	pData->aFactor[dgn::def::CSafetyFactor::Shear]       = dgn::lib::TFactor(false, m_CpgdD.dPhi_v);
	pData->aFactor[dgn::def::CSafetyFactor::ShearConnection] = dgn::lib::TFactor(false, m_CpgdD.dPhi_se);
	pData->aFactor[dgn::def::CSafetyFactor::ShearConnectionSLS] = dgn::lib::TFactor(false, m_CpgdD.dPhi_se);
	//pData->aFactor[dgn::def::CSafetyFactor::Torsion]     = dgn::def::CSafetyFactor::TFactor(true, m_dPhi[1]);

	return true;
}

bool CDgnCsgDataBase::GetSeismicProperty(dgn::def::CSeismicProperty* pData) const
{
	pData->bSeismic = false;// m_bEqSpecial;
	pData->FrameType = dgn::def::enSeismicFrameType::None;

	return true;
}

bool CDgnCsgDataBase::GetUnbracedLength(dgn::def::CUnbracedLength* pData) const
{
	ASSERT(0);

	return true;
}

bool CDgnCsgDataBase::GetUnbracedForce(dgn::def::CForceUbln* pData) const
{
	return true;
}

bool CDgnCsgDataBase::GetForce1DMember(dgn::def::CForce1DMember* pData) const
{
	if ( IsReportMode() )
	{
		GetForce1DMemberRpt(pData);
	}
	else
	{
		GetForce1DMemberDgn(pData);
	}

	return true;
}

bool CDgnCsgDataBase::GetForce1DMemberDgn(dgn::def::CForce1DMember* pData) const
{
	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT unPos = GetCheckPosition();
	int nDgnLcom = GetDgnLcomNo();

	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();
	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	T_SPAN_K CurSpanK = 0;
	ElemPairK nElemI = EPairK, nElemM = EPairK, nElemJ = EPairK;
	int nPosI = 0;
	int nPosM = 0; // 임시, 찾아야함.
	int nPosJ = 1;

	BOOL bChkUblm = FALSE;
	BOOL bCantilever = FALSE;
	BOOL bMakeMembFor = FALSE;
	BOOL b2SideBgg = FALSE;
	DgnBeamPairK nUblmK(EN_EL_BEAM, 0);
	DgnBeamPairK nCurUblmK(EN_EL_BEAM, 0);

	ADGNMEMB_FORCE aDgnMembFor;
	ADGNMEMB_FORCE aDgnMembFor_LTerm;      //Unbraced LongTerm 부재력
	ADGNMEMB_FORCE aDgnMembFor_Pre;        // bIsPLCB 일 경우 거더 부재력

	CSG_MEMB_POSD_KSCE_LSD MembLcomD;

	ArrElemPairKey aUblmElemK;
	m_pDataCtrl->GetUbLengthElemInfo(EPairK, nUblmK, nCurUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
	MembLcomD.bChkUlmf = bChkUblm;

	int nUblElemSize = aUblmElemK.GetSize();

	ADGNFORCE_EX aForceLcom_EX;
	ADGNFORCE_EX aForceTnpr_EX;
	ADGNFORCE_EX aForceTnsc_EX;
	ADGNSHRSADD_EX aStssLcom_EX;
	if ( bMakeMembFor == TRUE )
	{
		aDgnMembFor.RemoveAll();
		if ( nUblElemSize > 0 ) aDgnMembFor.SetSize(nUblElemSize);
		if ( nUblElemSize > 0 ) aDgnMembFor_LTerm.SetSize(nUblElemSize);
		if ( nUblElemSize > 0 ) aDgnMembFor_Pre.SetSize(nUblElemSize);

		for ( int nUbl = 0; nUbl < nUblElemSize; ++nUbl )
		{
			UINT ElemK = aUblmElemK[nUbl].first;

			auto itr = m_mMembForce.find(ElemK);
			auto itr_LT = m_mMembForce_LT.find(ElemK);
			if ( itr == m_mMembForce.end() || itr_LT == m_mMembForce_LT.end() )
			{
				_DGN_FORC_UL_MEMB MembForD;
				_DGN_FORC_UL_MEMB MembForD_LTerm;
				ADGNFORCE_EX aMembForD_NC;
				if ( !m_pForcCtrl->Get_LcomDataForDesign_EX(aUblmElemK[nUbl], 0, TRUE, TRUE, MembForD.ElemFor, nLcomNum) ) return FALSE;
				if ( !m_pForcCtrl->Get_LcomDataForDesign_EX(aUblmElemK[nUbl], TRUE, TRUE, aForceLcom_EX, aForceTnpr_EX, aForceTnsc_EX, MembForD_LTerm.ElemFor, aStssLcom_EX, nLcomNum) ) return FALSE;
				if ( bIsPLCB )  //시공단계가 없어도, Before Force까지 더한 부재력 사용
				{
					_DGN_FORC_UL_MEMB MembFor_Pre;
					if ( !m_pForcCtrl->Get_BeforeForce4PreCompo_EX(aUblmElemK[nUbl], MembFor_Pre.ElemFor, nLcomNum) ) return FALSE;
					aDgnMembFor_Pre.SetAt(nUbl, MembFor_Pre);
				}
				aDgnMembFor.SetAt(nUbl, MembForD);
				aDgnMembFor_LTerm.SetAt(nUbl, MembForD_LTerm);
				( ( Concurrency::concurrent_unordered_map<UINT, _DGN_FORC_UL_MEMB>& )m_mMembForce )[ElemK] = MembForD;
				( ( Concurrency::concurrent_unordered_map<UINT, _DGN_FORC_UL_MEMB>& )m_mMembForce_LT )[ElemK] = MembForD_LTerm;
			}
			else
			{
				aDgnMembFor.SetAt(nUbl, ( _DGN_FORC_UL_MEMB& ) itr->second);
				aDgnMembFor_LTerm.SetAt(nUbl, ( _DGN_FORC_UL_MEMB& ) itr_LT->second);
			}
		}
	}

	T_STRB_D MembForNC; MembForNC.Initialize();

	int nForceBeforeType = 1;
	m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(FALSE, ( T_STCT_D& ) m_StctD, m_pDataCtrl->GetElemPairKey4Elem(ElemK), NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcom, nForceBeforeType, aUblmElemK, &MembForNC);

	CArray<T_STRB_D, T_STRB_D&> aStrbElem;
	CArray<T_STRB_D, T_STRB_D&> aStrbElem_LTerm;
	CArray<T_STRB_D, T_STRB_D&> aStrbElem_Pre;

	if ( nUblElemSize > 0 )
	{
		aStrbElem.SetSize(nUblElemSize);
		aStrbElem_LTerm.SetSize(nUblElemSize);
		aStrbElem_Pre.SetSize(nUblElemSize);
	}

	MembLcomD.aChkFor.SetSize(1);
	MembLcomD.aUlmf.SetSize(5);
	int nDgnLcomIndex = nDgnLcom - 1;
	CDgnPscCommon::MakeConvertCsgMemberForce(bIsPLCB, aUblmElemK, nDgnLcomIndex,
		aDgnMembFor, aDgnMembFor_LTerm, aDgnMembFor_Pre,
		MembForNC, aStrbElem, aStrbElem_LTerm, aStrbElem_Pre, MembLcomD);

	pData->vdForceByPos.resize(5);
	pData->vdForceCatByPos.resize(5);

	const auto nSizeType = CForce1DMember::TypeNumber;
	const auto nSizeCatch = CForce1DMember::CatchNumber;

	typedef std::array<double, nSizeType> TypeList;
	typedef std::array<TypeList, nSizeCatch> CatchList;

	for ( int i = 0; i < 3; ++i )
	{
		const CSG_FORCE_D& ForLC = MembLcomD.aUlmf[0].ForD[i];
		TypeList aForce;
		aForce[CForce1DMember::Fx] = ForLC.Fx();
		aForce[CForce1DMember::Fy] = ForLC.Fy();
		aForce[CForce1DMember::Fz] = ForLC.Fz();
		aForce[CForce1DMember::Mx] = ForLC.Mx();
		aForce[CForce1DMember::My] = ForLC.My();
		aForce[CForce1DMember::Mz] = ForLC.Mz();

		int nPos = i * 2;
		pData->vdForceByPos[nPos] = aForce;

		CatchList aForceCat;
		const CSG_FORCE_D& ForNC = MembLcomD.aUlmf[1].ForD[i]; // Girder
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fx] = ForNC.Fx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fy] = ForNC.Fy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fz] = ForNC.Fz();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mx] = ForNC.Mx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::My] = ForNC.My();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mz] = ForNC.Mz();

		const CSG_FORCE_D& ForLT = MembLcomD.aUlmf[2].ForD[i]; // Long-term
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fx] = ForLT.Fx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fy] = ForLT.Fy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fz] = ForLT.Fz();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mx] = ForLT.Mx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::My] = ForLT.My();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mz] = ForLT.Mz();

		const CSG_FORCE_D& ForST = MembLcomD.aUlmf[3].ForD[i]; // Short-term
		aForceCat[CForce1DMember::Live][CForce1DMember::Fx] = ForST.Fx();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fy] = ForST.Fy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fz] = ForST.Fz();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mx] = ForST.Mx();
		aForceCat[CForce1DMember::Live][CForce1DMember::My] = ForST.My();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mz] = ForST.Mz();

		pData->vdForceCatByPos[nPos] = aForceCat;
		//pData->vdForceCatByPos.push_back(aForceCat);
	}

	for ( int i = 0; i < 2; ++i )
	{
		const CSG_FORCE_D& ForLC = MembLcomD.aUlmf[0].For13D[i];
		TypeList aForce;
		aForce[CForce1DMember::Fx] = ForLC.Fx();
		aForce[CForce1DMember::Fy] = ForLC.Fy();
		aForce[CForce1DMember::Fz] = ForLC.Fz();
		aForce[CForce1DMember::Mx] = ForLC.Mx();
		aForce[CForce1DMember::My] = ForLC.My();
		aForce[CForce1DMember::Mz] = ForLC.Mz();

		int nPos = i*2 + 1;
		pData->vdForceByPos[nPos] = aForce;

		CatchList aForceCat;
		const CSG_FORCE_D& ForNC = MembLcomD.aUlmf[1].For13D[i]; // Girder
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fx] = ForNC.Fx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fy] = ForNC.Fy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fz] = ForNC.Fz();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mx] = ForNC.Mx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::My] = ForNC.My();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mz] = ForNC.Mz();

		const CSG_FORCE_D& ForLT = MembLcomD.aUlmf[2].For13D[i]; // Long-term
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fx] = ForLT.Fx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fy] = ForLT.Fy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fz] = ForLT.Fz();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mx] = ForLT.Mx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::My] = ForLT.My();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mz] = ForLT.Mz();

		const CSG_FORCE_D& ForST = MembLcomD.aUlmf[3].For13D[i]; // Short-term
		aForceCat[CForce1DMember::Live][CForce1DMember::Fx] = ForST.Fx();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fy] = ForST.Fy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fz] = ForST.Fz();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mx] = ForST.Mx();
		aForceCat[CForce1DMember::Live][CForce1DMember::My] = ForST.My();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mz] = ForST.Mz();

		pData->vdForceCatByPos[nPos] = aForceCat;
		//pData->vdForceCatByPos.push_back(aForceCat);
	}

	return true;
}

bool CDgnCsgDataBase::GetForce1DMemberRpt(dgn::def::CForce1DMember* pData) const
{
	pData->vdForceByPos.resize(5);
	pData->vdForceCatByPos.resize(5);

	const auto nSizeType = CForce1DMember::TypeNumber;
	const auto nSizeCatch = CForce1DMember::CatchNumber;

	typedef std::array<double, nSizeType> TypeList;
	typedef std::array<TypeList, nSizeCatch> CatchList;
	for ( int i = 0; i < 3; ++i )
	{
		const T_CSGD_COMF& MembFor = m_DgnForD.MembFor[i];

		TypeList aForce;
		const T_CSGD_FORCE& ForLC = MembFor.ForLC;
		aForce[CForce1DMember::Fx] = ForLC.GetFx();
		aForce[CForce1DMember::Fy] = ForLC.GetFy();
		aForce[CForce1DMember::Fz] = ForLC.GetFz();
		aForce[CForce1DMember::Mx] = ForLC.GetMx();
		aForce[CForce1DMember::My] = ForLC.GetMy();
		aForce[CForce1DMember::Mz] = ForLC.GetMz();

		int nPos = i * 2;
		pData->vdForceByPos[nPos] = aForce;

		CatchList aForceCat;
		const T_CSGD_FORCE& ForNC = MembFor.ForNC;
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fx] = ForNC.GetFx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fy] = ForNC.GetFy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fz] = ForNC.GetFz();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mx] = ForNC.GetMx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::My] = ForNC.GetMy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mz] = ForNC.GetMz();

		const T_CSGD_FORCE& ForLT = MembFor.ForLT;
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fx] = ForLT.GetFx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fy] = ForLT.GetFy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fz] = ForLT.GetFz();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mx] = ForLT.GetMx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::My] = ForLT.GetMy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mz] = ForLT.GetMz();

		const T_CSGD_FORCE& ForST = MembFor.ForST;
		aForceCat[CForce1DMember::Live][CForce1DMember::Fx] = ForST.GetFx();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fy] = ForST.GetFy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fz] = ForST.GetFz();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mx] = ForST.GetMx();
		aForceCat[CForce1DMember::Live][CForce1DMember::My] = ForST.GetMy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mz] = ForST.GetMz();

		pData->vdForceCatByPos[nPos] = aForceCat;
	}

	for ( int i = 0; i < 2; ++i )
	{
		const T_CSGD_COMF& MembFor = m_DgnForD.MembFor13[i];

		TypeList aForce;
		const T_CSGD_FORCE& ForLC = MembFor.ForLC;
		aForce[CForce1DMember::Fx] = ForLC.GetFx();
		aForce[CForce1DMember::Fy] = ForLC.GetFy();
		aForce[CForce1DMember::Fz] = ForLC.GetFz();
		aForce[CForce1DMember::Mx] = ForLC.GetMx();
		aForce[CForce1DMember::My] = ForLC.GetMy();
		aForce[CForce1DMember::Mz] = ForLC.GetMz();

		int nPos = i*2 + 1;
		pData->vdForceByPos[nPos] = aForce;

		CatchList aForceCat;
		const T_CSGD_FORCE& ForNC = MembFor.ForNC;
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fx] = ForNC.GetFx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fy] = ForNC.GetFy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Fz] = ForNC.GetFz();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mx] = ForNC.GetMx();
		aForceCat[CForce1DMember::Girder][CForce1DMember::My] = ForNC.GetMy();
		aForceCat[CForce1DMember::Girder][CForce1DMember::Mz] = ForNC.GetMz();

		const T_CSGD_FORCE& ForLT = MembFor.ForLT;
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fx] = ForLT.GetFx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fy] = ForLT.GetFy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Fz] = ForLT.GetFz();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mx] = ForLT.GetMx();
		aForceCat[CForce1DMember::Dead][CForce1DMember::My] = ForLT.GetMy();
		aForceCat[CForce1DMember::Dead][CForce1DMember::Mz] = ForLT.GetMz();

		const T_CSGD_FORCE& ForST = MembFor.ForST;
		aForceCat[CForce1DMember::Live][CForce1DMember::Fx] = ForST.GetFx();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fy] = ForST.GetFy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Fz] = ForST.GetFz();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mx] = ForST.GetMx();
		aForceCat[CForce1DMember::Live][CForce1DMember::My] = ForST.GetMy();
		aForceCat[CForce1DMember::Live][CForce1DMember::Mz] = ForST.GetMz();

		pData->vdForceCatByPos[nPos] = aForceCat;
	}

	return true;
}

bool CDgnCsgDataBase::GetLcomInfo(dgn::def::CLcomInfo* pData) const
{
	CString strOrgLcomNa = _T("");
	int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;
	int nDgnLcom = GetDgnLcomNo();
	m_pForcCtrl->Get_LcomType(nDgnLcom, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType);

	T_LCOM_D LcomD;
	if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, nOrgLcomNo, LcomD) ) { ASSERT(0); LcomD.Initialize(); }

	BOOL bFatLcom = m_pForcCtrl->Is_LcomForFati(nDgnLcom);
	int nServLcomType = m_pForcCtrl->Get_ServLcomType(nDgnLcom);

	pData->strName = CT2W(LcomD.LoadCombName);
	pData->strDesc = CT2W(LcomD.Description);
	if ( LcomD.nActive == EN_DGN_NONE )
	{
		pData->unDgnType = dgn::def::CLcomInfo::EN_LCOM_NONE;
		pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_NONE;
	}
	else
	{
		if ( bFatLcom )
		{
			pData->unDgnType = dgn::def::CLcomInfo::EN_LCOM_FATI;
		}
		else
		{
			pData->unDgnType = LcomD.nActive == EN_DGN_SERV ? dgn::def::CLcomInfo::EN_LCOM_SERV : dgn::def::CLcomInfo::EN_LCOM_STRN;
			pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_NONE;
			if ( LcomD.nActive == EN_DGN_SERV )
			{
				switch ( nServLcomType )
				{
				case 1: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_QUAS; break;
				case 2: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_FREQ; break;
				case 3: pData->unSubType = dgn::def::CLcomInfo::EN_LCOM_SUB_CHAR; break;
				default: break;
				}
			}
		}
	}

	return true;
}

bool CDgnCsgDataBase::GetForce1D(dgn::def::CForce1D* pData) const
{
	ASSERT(0); //Force1DArray 쓰면되
	return true;
}

bool CDgnCsgDataBase::GetPartialFactor(dgn::def::CPartialFactor* pData) const
{
	pData->aFactor[dgn::def::CPartialFactor::GammaC_Fundamental] = dgn::lib::TFactor(false, m_CpgdD.dGamma_C);
	pData->aFactor[dgn::def::CPartialFactor::GammaS_Fundamental] = dgn::lib::TFactor(false, m_CpgdD.dGamma_S);
	pData->aFactor[dgn::def::CPartialFactor::GammaC_SLS] = dgn::lib::TFactor(false, m_CpgdD.dGamma_C);
	pData->aFactor[dgn::def::CPartialFactor::GammaS_SLS] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M_ser);
	pData->aFactor[dgn::def::CPartialFactor::GammaM0] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M0);
	pData->aFactor[dgn::def::CPartialFactor::GammaM1] = dgn::lib::TFactor(false, m_CpgdD.dGamma_M1);
	//pData->aFactor[dgn::def::CPartialFactor::GammaM2] = dgn::lib::TFactor(false, m_CpgdD.dGamma_);
	pData->aFactor[dgn::def::CPartialFactor::GammaV] = dgn::lib::TFactor(false, m_CpgdD.dGamma_V);
	pData->aFactor[dgn::def::CPartialFactor::GammaFf] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Ff);
	pData->aFactor[dgn::def::CPartialFactor::GammaMf] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Mf);
	pData->aFactor[dgn::def::CPartialFactor::GammaMfs] = dgn::lib::TFactor(false, m_CpgdD.dGamma_Mfs);

	return true;
}

bool CDgnCsgDataBase::GetSectionShape(dgn::def::CSectionShape* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_SECT_ELEM SectElem;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	UINT nPos = GetCheckPosition();

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[nPos];

	CSectionBuilder Builder;

	//Tapered를 지원해야되나 보군.
	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		{
			double dH  = GetH(SectPos);
			double dtw = GetTw(SectPos);
			double dBfTop = GetBfTop(SectPos);
			double dTfTop = GetTfTop(SectPos);
			double dBfBot = GetBfBot(SectPos);
			double dTfBot = GetTfBot(SectPos);

			pData->pSectionShape = Builder.CreateH(dBfTop, dH, dtw, dTfTop, dBfBot, dTfBot, 0.0, 0.0, enMakingType::Builtup);
		}
		break;
	case DGN_SECT_TYPE_COMPO_B:
		{
			double dH  = GetH(SectPos);
			double dtw = GetTw(SectPos);
			double dBftop = GetBfTop(SectPos);
			double dTfTop = GetTfTop(SectPos);
			double dBfbot = GetBfBot(SectPos);
			double dTfBot = GetTfBot(SectPos);
			double dCt = GetBfIntTop(SectPos);
			double dCb = GetBfIntBot(SectPos);

			pData->pSectionShape = Builder.CreateBox(dBftop, dH, dBfbot, dtw, dTfTop, dTfBot, dCt, dCb, 0.0, enMakingType::Builtup);
		}
		break;
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			double dH  = GetH(SectPos);
			double dtw = GetTw(SectPos);
			double dBftop = GetBfTop(SectPos);
			double dTfTop = GetTfTop(SectPos);
			double dBfTopOut = GetBfOutTop(SectPos);
			double dBfBot = GetBfBot(SectPos);
			double dTfBot = GetTfBot(SectPos);
			double dBfBotOut = GetBfOutBot(SectPos);
			double dBfTopVoid= GetBfTopVoid(SectPos);
			double dBfBotCtC = GetBfIntBot(SectPos);
			double dBTop = 2.0*dBftop + dBfTopVoid;

			pData->pSectionShape = Builder.CreateTub(dH, dBTop, dBftop, dBfBot, dBfTopOut, dBfBotOut, dtw, dTfTop, dTfBot, enMakingType::Builtup);
		}
		break;
	default: ASSERT(0); break;
	}

	return true;
}

bool CDgnCsgDataBase::GetMembLength(dgn::def::CMembLength* pData) const
{
	UINT ElemK = GetCheckElem();

	double dElemL = m_pDoc->calcLAVElem(ElemK);

	pData->aLength[Macro::Ordinal(enMembLengthType::Length)] = dElemL;

	return true;
}

bool CDgnCsgDataBase::GetStressCheck(dgn::def::CStressCheck* pData) const
{
	pData->adFactor[CStressCheck::k1] = dgn::lib::TFactor(false, m_CpgdD.dk1);
	pData->adFactor[CStressCheck::k2] = dgn::lib::TFactor(false, m_CpgdD.dk2);
	pData->adFactor[CStressCheck::k3] = dgn::lib::TFactor(false, m_CpgdD.dk3);
	return true;
}

bool CDgnCsgDataBase::GetMomentLoadType(dgn::def::CMomentLoadType* pData) const
{
	//뭐야 이거 어디서 줍줍하는거냐
	bool bLongTermType = false;
	for ( int unCalcLoadType = 0; unCalcLoadType < CMomentLoadType::enType::Number; ++unCalcLoadType )
	{
		pData->aMoment_y[unCalcLoadType].reserve(1);
		pData->aMoment_y[unCalcLoadType].push_back(0.0);
		pData->aTerm[unCalcLoadType] = [ &unCalcLoadType ] (bool bLongTermType)
		{
			switch ( unCalcLoadType )
			{
			case dgn::def::CMomentLoadType::Permanent:
				return CMomentLoadType::enTerm::Long;
			case dgn::def::CMomentLoadType::Variable:
				return bLongTermType ? CMomentLoadType::enTerm::Long : CMomentLoadType::enTerm::Short;
			case dgn::def::CMomentLoadType::EtcLoad:
				return CMomentLoadType::enTerm::Short;
			default: ASSERT(0); return CMomentLoadType::enTerm::Long;
			}
		} ( bLongTermType );
	}

	return true;
}

bool CDgnCsgDataBase::GetTorsionDesign(dgn::def::CTorsionDesign* pData) const
{
	pData->bTorsionDesign = false;

	return true;
}

bool CDgnCsgDataBase::GetDgnSteelOptions(dgn::def::CDgnSteelOptions* pData) const
{
	pData->Init();
	return true;
}

bool CDgnCsgDataBase::GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const
{
	pData->Init();
	return true;
}

bool CDgnCsgDataBase::GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_SECT_ELEM SectElem;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	UINT nPos = GetCheckPosition();

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[nPos];

	double dBc = GetBc(SectPos);
	double dtc = GetTc(SectPos);
	double dHh = GetHh(SectPos);

	const int nIdxRCS = Macro::Ordinal(enType::Concrete);

	CSectionBuilder Builder;
	pData->apSectionShape[nIdxRCS] = Builder.CreateRectangle(dBc, dtc);

	const int nIdxSTL = Macro::Ordinal(enType::Steel);

	double dBh = 0.0;
	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		{
			double dSteelHeight = GetH(SectPos);
			double dSteelWidthFlangeTop = GetBfTop(SectPos);
			double dSteelWidthFlangeBot = GetBfBot(SectPos);
			double dSteelThkWeb = GetTw(SectPos);
			double dSteelThkFlangeTop = GetTfTop(SectPos);
			double dSteelThkFlangeBot = GetTfBot(SectPos);

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeISymY(CAlignedShapeISymY(dSteelHeight,
					dSteelWidthFlangeTop,
					dSteelWidthFlangeBot,
					dSteelThkWeb,
					dSteelThkFlangeTop,
					dSteelThkFlangeBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dSteelWidthFlangeTop;
		}
		break;
	case DGN_SECT_TYPE_COMPO_B:
		{
			double dSteelHeight = GetH(SectPos);
			double dSteelWidthFlangeTop = GetBfTop(SectPos);
			double dSteelWidthFlangeBot = GetBfBot(SectPos);
			double dSteelThkWeb = GetTw(SectPos);
			double dSteelThkFlangeTop = GetTfTop(SectPos);
			double dSteelThkFlangeBot = GetTfBot(SectPos);
			double dCt = GetBfIntTop(SectPos);
			double dCb = GetBfIntBot(SectPos);
			double dCosA = fabs(cos(GetWebSlope(SectPos)));
			double dtwh = SafeDiv(dSteelThkWeb, dCosA);
			double dtwhHalf = 0.5*dtwh;

			double dBfTopOut = GetBfOutTop(SectPos) - dtwhHalf;
			double dBfBotOut = GetBfOutBot(SectPos) - dtwhHalf;

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeBoxSymY(CAlignedShapeBoxSymY(dSteelHeight,
					dSteelWidthFlangeTop,
					dSteelWidthFlangeBot,
					dBfTopOut,
					dBfBotOut,
					dSteelThkWeb,
					dSteelThkFlangeTop,
					dSteelThkFlangeBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dSteelWidthFlangeTop;
		}
		break;
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			double dH = GetH(SectPos);
			double dtw = GetTw(SectPos);
			double dBfTop = GetBfTop(SectPos);
			double dTfTop = GetTfTop(SectPos);
			double dBfTopOut = GetBfOutTop(SectPos);
			double dBfBot = GetBfBot(SectPos);
			double dTfBot = GetTfBot(SectPos);
			double dBfBotOut = GetBfOutBot(SectPos);
			double dBfTopVoid = GetBfTopVoid(SectPos);
			double dBfBotCtC = GetBfIntBot(SectPos);
			double dBTop = 2.0*dBfTop + dBfTopVoid;

			pData->apSectionShape[nIdxSTL] =
				new CSectionShapeTubSymY(CAlignedShapeTubSymY(dH,
					dBTop,
					dBfTop,
					dBfBot,
					dBfTopOut,
					dBfBotOut,
					dtw,
					dTfTop,
					dTfBot,
					CAlign2D(), enMakingType::Builtup));

			dBh = dBfTop;
		}
		break;
	default: ASSERT(0); break;
	}

	const int nIdxHunch = Macro::Ordinal(enType::Hunch);
	pData->apSectionShape[nIdxHunch] = Builder.CreateRectangle(dBh, dHh);

	GetLongitudinalStiffenerData(&pData->Stiffener);

	return true;
}

bool CDgnCsgDataBase::GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const
{
	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	const _UMD_RC_MATL_PROP& MatSlab = MatlElem.MatSlab;

	pData->Data.strName = ( MatSlab.ConcD.strMatlName );
	if ( MatSlab.ConcD.strCodeName.CompareNoCase(_T("None")) == 0 )
	{
		pData->Data.Common.dElastic = MatSlab.RC.dEc;
	}
	else
	{
		pData->Data.Common.dElastic = MatSlab.RC.dEc;
	}

	//pData->Data.Common.dPoisson = MatSlab.RC.Poisson;
	//pData->Data.Common.dWeightDensity = MatdD.Data2.Analysis.Density;
	//
	pData->Data.Concrete.dStrength = MatSlab.RC.dFck;
	//
	pData->Data.Rebar.dYield = MatSlab.RC.dFy;

	return true;
}

bool CDgnCsgDataBase::GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const
{
	UINT nPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	const double dFu = SMat.S_Fu;
	TMaterialSteelSingle Single1(SMat.S_Fy1, dFu);
	TMaterialSteelSingle Single2(SMat.S_Fy2, dFu);
	TMaterialSteelSingle Single3(SMat.S_Fy3, dFu);
	TMaterialSteelSingle Single4(SMat.S_Fy4, dFu);
	TMaterialSteelSingle Single5(SMat.S_Fy5, dFu);
	TMaterialSteelSingle Single6(SMat.S_Fy6, dFu);

	CSG_SECT_ELEM SectElem;

	UINT ElemK = GetCheckElem();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[nPos];

	double dSteelThkWeb = GetTw(SectPos);
	double dSteelThkFlangeTop = GetTfTop(SectPos);
	double dSteelThkFlangeBot = GetTfBot(SectPos);

	TMaterialSteel MaterialSteel;
	MaterialSteel.Common.dElastic = SMat.Elast;
	MaterialSteel.Common.dShear   = EQ0(SMat.Shear) ? 80000.0 : SMat.Shear; // 80000 MPa 임시
	MaterialSteel.Common.dPoisson = SMat.Poisson;
	MaterialSteel.Common.dWeightDensity = SMat.Density;
	MaterialSteel.Steel.nList = 1;
	MaterialSteel.Steel.bUser = ( MatlElem.MatStf.csCodeName.CompareNoCase(_T("None")) == 0 );
	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkFlangeTop, Single1);
	pData->aData[0] = MaterialSteel;

	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkFlangeBot, Single1);
	pData->aData[1] = MaterialSteel;

	MaterialSteel.Steel.aProp[0] = SteelThickProp(dSteelThkWeb, Single1);
	pData->aData[2] = MaterialSteel;

	return true;
}

bool CDgnCsgDataBase::GetForce1DArray(dgn::def::CForce1DArray* pData) const
{
	if ( IsReportMode() )
	{
		GetForce1DArrayRpt(pData);
	}
	else
	{
		GetForce1DArrayDgn(pData);
	}

	return true;
}

bool CDgnCsgDataBase::GetForce1DArrayDgn(dgn::def::CForce1DArray* pData) const
{
	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();

	CForce1D Force1D;
	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT unPos = GetCheckPosition();
	int nDgnLcom = GetDgnLcomNo();

	//Girder Force
	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(EPairK, nGdLastStagK, nLastStepNo);

	int nForceBeforeType = 1;
	_DGN_FORC_CRC ForceNC;
	if ( m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, ( T_STCT_D& ) m_StctD, EPairK, NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcom, nForceBeforeType, ForceNC) )
	{
		Force1D.adForce[dgn::def::CForce1D::Fx] = ForceNC.dFxx[unPos];
		Force1D.adForce[dgn::def::CForce1D::Fy] = ForceNC.dFyy[unPos];
		Force1D.adForce[dgn::def::CForce1D::Fz] = ForceNC.dFzz[unPos];
		Force1D.adForce[dgn::def::CForce1D::Mx] = ForceNC.dMux[unPos];
		Force1D.adForce[dgn::def::CForce1D::My] = ForceNC.dMuy[unPos];
		Force1D.adForce[dgn::def::CForce1D::Mz] = ForceNC.dMuz[unPos];

		pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Girder, Force1D));
	}

	//Composite Long Term Force
	int nFortype = 0;
	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	int nDgnLcomIndex = nDgnLcom - 1;

	ADGNFORCE aForceLcom;
	ADGNFORCE aForceTnpr;
	ADGNFORCE aForceTnsc;
	ADGNFORCE aForceLTerm;
	ADGNFORCE aForceSTerm;
	ADGNSHRSADD aStssLcom;
	if ( m_pForcCtrl->Get_LcomDataForDesign(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum) )
	{
		int nLcomSize = aForceLcom.GetSize();
		if ( nLcomSize == 0 ) ASSERT(0);
		if ( nDgnLcomIndex < 0 || nDgnLcomIndex >= nLcomSize )
		{
			ASSERT(0);
			return true;
		}
	}
	else
		ASSERT(0);

	_DGN_FORC_CRC ForceST = aForceLcom[nDgnLcomIndex];
	_DGN_FORC_CRC ForceLT = aForceLTerm[nDgnLcomIndex];
	_DGN_FORC_CRC ForceSum = aForceLcom[nDgnLcomIndex];
	if ( bIsPLCB == TRUE )
	{
		ForceST -= ForceNC;
		ForceST -= ForceLT;
	}
	else
	{
		ForceST -= ForceLT;
		ForceLT -= ForceNC;
	}

	Force1D.adForce[dgn::def::CForce1D::Fx] = ForceLT.dFxx[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForceLT.dFyy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForceLT.dFzz[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForceLT.dMux[unPos];
	Force1D.adForce[dgn::def::CForce1D::My] = ForceLT.dMuy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForceLT.dMuz[unPos];
	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_LongTerm, Force1D));

	//Composite Short Term Force
	Force1D.adForce[dgn::def::CForce1D::Fx] = ForceST.dFxx[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForceST.dFyy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForceST.dFzz[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForceST.dMux[unPos];
	Force1D.adForce[dgn::def::CForce1D::My] = ForceST.dMuy[unPos];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForceST.dMuz[unPos];
	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_ShortTerm, Force1D));

	return true;
}

bool CDgnCsgDataBase::GetForce1DArrayRpt(dgn::def::CForce1DArray* pData) const
{
	CForce1D Force1D;

	const T_CSGD_COMF& ChkFor = m_DgnForD.ChkFor;

	const T_CSGD_FORCE& ForNC = ChkFor.ForNC;
	Force1D.adForce[dgn::def::CForce1D::Fx] = ForNC.dForce[0];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForNC.dForce[1];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForNC.dForce[2];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForNC.dForce[3];
	Force1D.adForce[dgn::def::CForce1D::My] = ForNC.dForce[4];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForNC.dForce[5];
	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Girder, Force1D));

	const T_CSGD_FORCE& ForLT = ChkFor.ForLT;
	Force1D.adForce[dgn::def::CForce1D::Fx] = ForLT.dForce[0];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForLT.dForce[1];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForLT.dForce[2];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForLT.dForce[3];
	Force1D.adForce[dgn::def::CForce1D::My] = ForLT.dForce[4];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForLT.dForce[5];
	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_LongTerm, Force1D));

	const T_CSGD_FORCE& ForST = ChkFor.ForST;
	Force1D.adForce[dgn::def::CForce1D::Fx] = ForST.dForce[0];
	Force1D.adForce[dgn::def::CForce1D::Fy] = ForST.dForce[1];
	Force1D.adForce[dgn::def::CForce1D::Fz] = ForST.dForce[2];
	Force1D.adForce[dgn::def::CForce1D::Mx] = ForST.dForce[3];
	Force1D.adForce[dgn::def::CForce1D::My] = ForST.dForce[4];
	Force1D.adForce[dgn::def::CForce1D::Mz] = ForST.dForce[5];
	pData->adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_ShortTerm, Force1D));

	return true;
}

bool CDgnCsgDataBase::GetLoadApplicationType(dgn::def::CLoadApplicationType* pData) const
{
	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT unPos = GetCheckPosition();

	CPG_BCDT_ELEM BcdtElem; BcdtElem.Initialize();
	Get_CpgBcdtElem(EPairK, BcdtElem);

	const CPG_BCDT_POSI& BcdtB = BcdtElem.BcdtPosi[unPos];

	pData->adValue[dgn::def::CLoadApplicationType::PanelPitch] = BcdtB.da;
	pData->adValue[dgn::def::CLoadApplicationType::Ss] = BcdtB.dSs;
	pData->adValue[dgn::def::CLoadApplicationType::C] = BcdtB.dC;

	if ( BcdtB.iLoadAppType == 0 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_Web;
	}
	else if ( BcdtB.iLoadAppType == 1 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_Flange;
	}
	else if ( BcdtB.iLoadAppType == 2 )
	{
		pData->enLoadApp = dgn::def::CLoadApplicationType::Flange_unstiffened_end;
	}
	else
		ASSERT(0);

	return true;
}

bool CDgnCsgDataBase::GetLateralTorsionalBucklingParam(dgn::def::CLateralTorsionalBuckling* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();
	int nDgnLcom = GetDgnLcomNo();

	// EC4에만 사용하는 Input

// 	CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();
// 	Get_CpgLtbdElem(GetCheckElem(), LtbdElem);
//
// 	BOOL bReverse = FALSE;
// 	T_ELEM_K showElemK = 0;
// 	CArray<T_ELEM_K, T_ELEM_K> arMembElemList;
// 	int iCountElem = 0;
// 	T_ELEM_K IendElemK = 0;
// 	T_ELEM_K JendElemK = 0;
// 	m_pMembCtrl->GetElemListByIncludeElem(ElemK, bReverse, showElemK, arMembElemList);
//
// 	double dC4 = 0.0;
// 	m_pDataCtrl->Get_C4valueForMomentType(nDgnLcom, showElemK, arMembElemList, dC4);
//
// 	const CPG_LTBD_POSI& LtbdB = LtbdElem.LtbdPosi[unPos];
//
// 	pData->adValue[dgn::def::CLateralTorsionalBuckling::Dspring] = LtbdB.dl;
// 	pData->adValue[dgn::def::CLateralTorsionalBuckling::Cd] = LtbdB.dCd;
// 	pData->adValue[dgn::def::CLateralTorsionalBuckling::a] = LtbdB.da;
// 	pData->adValue[dgn::def::CLateralTorsionalBuckling::Alpha] = static_cast<double>(LtbdB.iAlpha);
// 	pData->adValue[dgn::def::CLateralTorsionalBuckling::C4] = dC4;

	return true;
}

bool CDgnCsgDataBase::GetShearConnector(dgn::def::CShearConnector* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	const auto& itCpsc = m_mCsgCpsc.find(EPairK);
	if (itCpsc != m_mCsgCpsc.end())
	{
		const CSG_CPSC_POSD& SconB = itCpsc->second.TfType[unPos];

		TStudSingle StudSingle;
		StudSingle.dDia = SconB.dDia;
		StudSingle.dHeight = SconB.dHeight;
		StudSingle.dLength = SconB.dSpace;

		enConnectorType SconType = enConnectorType::Stud;

		TMaterialSteelSingle MSingle(SconB.dFu, SconB.dFu);

		CShearConnectorSingleStud Stud(StudSingle, SconType, MSingle);

		double dNumTran = static_cast< double >( SconB.nNum );

		pData->ShearConnector.SetSingle(&Stud);
		pData->ShearConnector.SetLongitudinalInfo(dNumTran, SconB.dPitch);
	}

	return true;
}

bool CDgnCsgDataBase::GetSteelCompositeRebar(dgn::def::CSectionRebarCoord* pData) const
{
	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT nPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	CSG_SECT_ELEM SectElem;
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[nPos];

	double dtc = GetTc(SectPos);
	double dHh = GetHh(SectPos);
	double dHg = GetH(SectPos);

	CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
	Get_CpgRbarElem(EPairK, RbarElem);

	const _PSC_RBAR_POSI& RbarPosB = RbarElem.RbarPosi[nPos];

	const auto dfy = MatlElem.MatSlab.RC.dFy;
	const auto dEs = MatlElem.MatSlab.RC.dEs;

	for ( int i = 0; i < RbarPosB.arRbarPosi.GetSize(); ++i )
	{
		const _DGN_RBAR_CRC& RbarB = RbarPosB.arRbarPosi[i];

		const auto dArea = RbarB.dArea;
		const auto dPerimeter = RbarB.dDia;
		const auto dDia = RbarB.dDia;
		std::wstring strDia = static_cast< std::wstring >( CT2W(RbarB.strRbarNa) );

		const auto dx = RbarB.dy;
		const auto dy = RbarB.dz + ( 0.5*dHg + dHh + 0.5*dtc );

		TDiaGeom Geom(dArea, dPerimeter, dDia);
		TDiaMatl Matl(dfy, dEs);
		TDia Dia(Geom, Matl, strDia.c_str());

		CMainRebarSingle Single(DVector2d(dx, dy), Dia);

		pData->vSingle.push_back(Single);
	}

	return true;
}

bool CDgnCsgDataBase::GetLongitudinalStiffener(dgn::def::CLongitudinalStiffener* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	CSG_MATL_ELEM MatlElem;
	if (!m_mCsgMatl.Lookup(ElemD.elmat, MatlElem)) return false;
		
	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;
	TMaterialCommonProp MComm(SMat.Elast, SMat.Poisson, SMat.Density);
	TMaterialSteelSingle MSingle(SMat.S_Fy1, SMat.S_Fu);
	TStiffenerMaterial MtStfn(MComm, MSingle);

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	ST_SECT_SECTBASE_D SectBaseD = SectPos.SectInfo.Before;
	const int nStype = SectPos.SectInfo.nStype;
	switch (nStype)
	{
	case DGN_SECT_TYPE_COMPO_I:
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{	
			return m_pDataCtrl->m_pCsgDataCtrl->GetLongitudinalStiffenerType1(nStype, SectBaseD, MtStfn, pData);
		}		
	case DGN_SECT_TYPE_COMPO_STLG_I:
		{
			return m_pDataCtrl->m_pCsgDataCtrl->GetLongitudinalStiffenerType2(nStype, SectBaseD, MtStfn, pData);
		}
	case DGN_SECT_TYPE_COMPO_STLG_B:
	case DGN_SECT_TYPE_COMPO_STLG_TUB:		
	default: ASSERT(0); break;
	}

	return true;
}

bool CDgnCsgDataBase::GetLongitudinalStiffenerData(CLongitudinalStiffener* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	const double dFu = SMat.S_Fu;

	const double& dElastic = SMat.Elast;
	const double& dPoisson = SMat.Poisson;
	const double& dWeightDensity = SMat.Density;

	TMaterialCommonProp MComm(dElastic, dPoisson, dWeightDensity);
	TMaterialSteelSingle MSingle(SMat.S_Fy1, dFu);
	TStiffenerMaterial MtStfn(MComm, MSingle);

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	ST_SECT_SECTBASE_D SectBaseD = SectPos.SectInfo.Before;
	bool bStiffLft = true;
	bool bStiffRgt = true;
	int nSectPart  = 4;
	if ( SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_I )
	{
		nSectPart = 2;
		switch ( SectBaseD.Stiffener.nPosition )
		{
		case 0: { bStiffLft = true; bStiffRgt = true; } break;
		case 1: { bStiffLft = true; bStiffRgt = false; } break;
		case 2: { bStiffLft = false; bStiffRgt = true; } break;
		default: ASSERT(0); break;
		}
	}

	std::vector<IStiffenerSingle*> vpSingle;
	for ( int nPart = 0; nPart < nSectPart; ++nPart ) // 0:Left, 1:Right, 2:Bottom, 3:Top
	{
		CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&>* paStiff;

		TStiffenerLocation StiffLoca;
		dgn::def::enPart enSPart;
		switch ( nPart )
		{
		case 0:
			{
				if ( !bStiffLft ) continue;
				paStiff = &SectBaseD.Stiffener.aStiffLeft;
				enSPart = SectPos.SectInfo.nStype == D_SECT_TYPE_COMPO_I ? enPart::Web : enPart::WebL;
				StiffLoca.Location = enStiffenerLocation::Left;
			}
			break;
		case 1:
			{
				if ( !bStiffRgt ) continue;
				paStiff = &SectBaseD.Stiffener.aStiffRight;
				enSPart = SectPos.SectInfo.nStype == D_SECT_TYPE_COMPO_I ? enPart::Web : enPart::WebR;
				StiffLoca.Location = enStiffenerLocation::Right;
			}
			break;
		case 2:
			{
				paStiff = &SectBaseD.Stiffener.aStiffBottom;
				enSPart = enPart::Bot;
				StiffLoca.Location = enStiffenerLocation::Bot;
			}
			break;
		case 3:
			{
				paStiff = &SectBaseD.Stiffener.aStiffTop;
				enSPart = enPart::Top;
				StiffLoca.Location = enStiffenerLocation::Top;
			}
			break;
		default: ASSERT(0); break;
		}

		double dPosY = 0.0;
		double dPosZ = 0.0;
		for ( int i = 0; i < paStiff->GetSize(); ++i )
		{
			const ST_STIFF_PROFILE_D& ProfileB = paStiff->GetAt(i);
			// 보강재의 Stiffness 반영 여부에 관계 없이 보강재 고려
			//if ( ProfileB.bCalc == false )
			//	continue;

			const ST_STIFF_SHAPE_D& Shape = SectBaseD.Stiffener.aStiffShape[ProfileB.nShape];

			if ( enSPart == enPart::Web || enSPart == enPart::WebL || enSPart == enPart::WebR )
			{
				dPosZ = ProfileB.dSpacing;
			}
			else if ( enSPart == enPart::Top || enSPart == enPart::Bot )
			{
				dPosY = ProfileB.dSpacing;
			}

			dgn::lib::enTypicalType enType;
			switch ( Shape.nType ) // 0:I, 1:T, 2:U-Rib, 3:L
			{
			case 0:
				{
					enType = enTypicalType::Rectangle;
					CAlignedShapeRectangle ShapeRect(Shape.dSize[1], Shape.dSize[0], CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeRect.SetAlign(AlignToStfn);

					CStiffenerRectangle LSShape(ShapeRect, MtStfn, enSPart, StiffLoca);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 1:
				{
					enType = enTypicalType::T;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dtw = Shape.dSize[2];
					double dtf = Shape.dSize[3];
					CAlignedShapeT ShapeT(dB, dH, dtw, dtf, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeT.SetAlign(AlignToStfn);

					CStiffenerT LSShape(ShapeT, MtStfn, enSPart, StiffLoca);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			case 2:
				{
					enType = enTypicalType::URib;
					double dB = Shape.dSize[1];
					double dH = Shape.dSize[0];
					double dBb = Shape.dSize[2];
					double dt = Shape.dSize[3];
					double dR = Shape.dSize[4];
					CAlignedShapeURib ShapeU(dB, dH, dBb, dt, dR, CAlign2D());
					auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(dPosY, dPosZ));
					ShapeU.SetAlign(AlignToStfn);

					CStiffenerURib LSShape(ShapeU, MtStfn, enSPart, StiffLoca);
					vpSingle.push_back(LSShape.Clone());
				}
				break;
			default: ASSERT(0); break;
			}
		}
	}

	pData->Set(vpSingle);

	return true;
}

bool CDgnCsgDataBase::GetTransverseStiffener(dgn::def::CTransverseStiffener* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) return false;  // GetElemPost가 아닌 GetElem로..

	CSG_MATL_ELEM MatlElem;
	if ( !m_mCsgMatl.Lookup(ElemD.elmat, MatlElem) ) return false;

	const T_DGN_MATL_STEEL& SMat = MatlElem.MatStf.Steel;

	CSG_TRST_SECT TStfnElem; TStfnElem.Initialize();
	const auto& itSect4E = m_mCsgElemSect.find(EPairK);
	if (itSect4E != m_mCsgElemSect.end())
	{
		const auto& itTrst = m_mCsgTrst.find(itSect4E->second);
		if (itTrst != m_mCsgTrst.end())
		{
			TStfnElem = itTrst->second;
		}
	}

	//CPG_TSES_ELEM TsesElem; TsesElem.Initialize();
	//Get_CpgTsesElem(GetCheckElem(), TsesElem);

	const CSG_TRST_POSD& TStfnPosB = TStfnElem.TrstPos[unPos];
	//const CPG_TSES_POSI& TsesPosB = TsesElem.TsesPosi[unPos];

	if ( TStfnPosB.bUseStifWeb )
	{
		const double dFu = SMat.S_Fu;

		const double& dElastic = SMat.Elast;
		const double& dPoisson = SMat.Poisson;
		const double& dWeightDensity = SMat.Density;

		TMaterialCommonProp MComm(dElastic, dPoisson, dWeightDensity);
		TMaterialSteelSingle MSingle(SMat.S_Fy1, dFu);

		TStiffenerMaterial MtStfn(MComm, MSingle);

		CAlignedShapeRectangle ShapeRect(TStfnPosB.dSizeWeb[0], TStfnPosB.dSizeWeb[1], CAlign2D());
		auto AlignToStfn = CAlign2D::CreateByDegree(0.0, DVector2d(TStfnPosB.dPitchWeb, 0.0));

		dgn::def::enPart enSPart = enPart::Web;

		CStiffenerRectangle TSShape(ShapeRect, MtStfn, enSPart, TStiffenerLocation());

		TTransverseData TransverseD;
		TransverseD.dPitch = TStfnPosB.dPitchWeb;
		// 		TransverseD.dInternalRigidPitch = TStfnPosB.dDistRstif;
		// 		TransverseD.dNonRigidPitch = TStfnPosB.dDistNRstif;
		// 		TransverseD.bInternalRigid = TStfnPosB.bInterRigidStif;
		// 		TransverseD.bNonRigid = TStfnPosB.bInterNonRigidStif;
		TransverseD.enEndType = enEndSupport::NonRigidEndPost;
		// 		if ( TsesPosB.bUseStif )
		// 		{
		// 			switch ( TsesPosB.iEndpost )
		// 			{
		// 			case 0: TransverseD.enEndType = enEndSupport::RigidEndPost; break;
		// 			case 1: TransverseD.enEndType = enEndSupport::NonRigidEndPost; break;
		// 			case 2: TransverseD.enEndType = enEndSupport::NoEndPost; break;
		// 			default: ASSERT(0); break;
		// 			}
		// 		}

		pData->Set(TSShape.Clone(), TransverseD);
	}

	return true;
}

bool CDgnCsgDataBase::GetDamageEquivalenceFactor(dgn::def::CDamageEquivalenceFactorDef* pData) const
{
	pData->adValue[CDamageEquivalenceFactorDef::t_LD] = m_CpgdD.dt_Ld;

	return true;
}

bool CDgnCsgDataBase::GetDamageEquivalenceFactorMember(dgn::def::CDamageEquivalenceFactorMembDef* pData) const
{
	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT unPos = GetCheckPosition();

	CPG_FRDT_ELEM FrdtElem; FrdtElem.Initialize();
	Get_CpgFrdtElem(EPairK, FrdtElem);

	const CPG_FRDT_POSI& FrdtPosB = FrdtElem.FrdtPosi[unPos];

	pData->adValue[CDamageEquivalenceFactorMembDef::LambdaV2] = FrdtPosB.dLamda_v2;
	pData->adValue[CDamageEquivalenceFactorMembDef::LambdaV4] = FrdtPosB.dLamda_v4;

	return true;
}

bool CDgnCsgDataBase::GetStressLimitationFactor(dgn::def::CStressLimitationFactorDef* pData) const
{
	pData->adValue[CStressLimitationFactorDef::ks] = m_CpgdD.dks;

	return true;
}

bool CDgnCsgDataBase::GetStressSteelComposite(dgn::def::CStressSteelCompositeDef* pData) const
{
	if ( IsReportMode() )
	{
		GetStressSteelCompositeRpt(pData);
	}
	else
	{
		GetStressSteelCompositeDgn(pData);
	}

	return true;
}

bool CDgnCsgDataBase::GetStressSteelCompositeDgn(dgn::def::CStressSteelCompositeDef* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();
	UINT nLcomIdx = GetDgnLcomNo() - 1;

	auto itrP1 = m_mStress.find(ElemK);
	if ( itrP1 == m_mStress.end() )
	{
		ASSERT(0); return false;
	}

	auto itrP2 = m_mStressP2.find(ElemK);
	if ( itrP2 == m_mStressP2.end() )
	{
		ASSERT(0); return false;
	}

	pData->adGirder[CStressSteelCompositeDef::LT] = itrP1->second.aStressLcom[nLcomIdx].dStl[unPos];
	pData->adGirder[CStressSteelCompositeDef::RT] = itrP1->second.aStressLcom[nLcomIdx].dStr[unPos];
	pData->adGirder[CStressSteelCompositeDef::RB] = itrP1->second.aStressLcom[nLcomIdx].dSbr[unPos];
	pData->adGirder[CStressSteelCompositeDef::LB] = itrP1->second.aStressLcom[nLcomIdx].dSbl[unPos];
	//
	pData->adSlab[CStressSteelCompositeDef::LT] = itrP2->second.aStressLcom[nLcomIdx].dStl[unPos];
	pData->adSlab[CStressSteelCompositeDef::RT] = itrP2->second.aStressLcom[nLcomIdx].dStr[unPos];
	pData->adSlab[CStressSteelCompositeDef::RB] = itrP2->second.aStressLcom[nLcomIdx].dSbr[unPos];
	pData->adSlab[CStressSteelCompositeDef::LB] = itrP2->second.aStressLcom[nLcomIdx].dSbl[unPos];

	return true;
}

bool CDgnCsgDataBase::GetStressSteelCompositeRpt(dgn::def::CStressSteelCompositeDef* pData) const
{
	UINT unPos = GetCheckPosition();
	pData->adGirder[CStressSteelCompositeDef::LT] = m_GirderStress.dStl[unPos];
	pData->adGirder[CStressSteelCompositeDef::RT] = m_GirderStress.dStr[unPos];
	pData->adGirder[CStressSteelCompositeDef::RB] = m_GirderStress.dSbr[unPos];
	pData->adGirder[CStressSteelCompositeDef::LB] = m_GirderStress.dSbl[unPos];
	//
	pData->adSlab[CStressSteelCompositeDef::LT] = m_SlabStress.dStl[unPos];
	pData->adSlab[CStressSteelCompositeDef::RT] = m_SlabStress.dStr[unPos];
	pData->adSlab[CStressSteelCompositeDef::RB] = m_SlabStress.dSbr[unPos];
	pData->adSlab[CStressSteelCompositeDef::LB] = m_SlabStress.dSbl[unPos];

	return true;
}

bool CDgnCsgDataBase::GetMemoizationKey(dgn::def::CMemoizationKey* pData) const
{
	pData->vMemoKey.push_back(std::make_pair(DataType::Elem, ( UINT ) GetCheckElem()));
	pData->vMemoKey.push_back(std::make_pair(DataType::Node, ( UINT ) GetCheckPosition()));
	return true;
}

bool CDgnCsgDataBase::GetPanelInfoArray(dgn::def::CPanelInfoArray* pData) const
{
	pData->vPanelInfo.resize(2);
	GetPanelInfoSectionProperty(pData);
	// Force
	if ( IsReportMode() )
	{
		GetPanelInfoForceRpt(pData);
	}
	else
	{
		GetPanelInfoForceDgn(pData);
	}

	return true;
}

bool CDgnCsgDataBase::GetPanelInfoForceDgn(dgn::def::CPanelInfoArray* pData) const
{
	_DGN_FORC_CRC ForceNC;
	_DGN_FORC_CRC ForceLT;
	_DGN_FORC_CRC ForceST;
	GetPanelInfoForce(ForceNC, ForceLT, ForceST);

	for ( int nPos = 0; nPos < 2; ++nPos )
	{
		CForce1D Force1D;

		Force1D.adForce[CForce1D::Fx] = ForceNC.dFxx[nPos];
		Force1D.adForce[CForce1D::Fy] = ForceNC.dFyy[nPos];
		Force1D.adForce[CForce1D::Fz] = ForceNC.dFzz[nPos];
		Force1D.adForce[CForce1D::Mx] = ForceNC.dMux[nPos];
		Force1D.adForce[CForce1D::My] = ForceNC.dMuy[nPos];
		Force1D.adForce[CForce1D::Mz] = ForceNC.dMuz[nPos];
		pData->vPanelInfo[nPos].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Girder, Force1D));

		Force1D.adForce[dgn::def::CForce1D::Fx] = ForceLT.dFxx[nPos];
		Force1D.adForce[dgn::def::CForce1D::Fy] = ForceLT.dFyy[nPos];
		Force1D.adForce[dgn::def::CForce1D::Fz] = ForceLT.dFzz[nPos];
		Force1D.adForce[dgn::def::CForce1D::Mx] = ForceLT.dMux[nPos];
		Force1D.adForce[dgn::def::CForce1D::My] = ForceLT.dMuy[nPos];
		Force1D.adForce[dgn::def::CForce1D::Mz] = ForceLT.dMuz[nPos];
		pData->vPanelInfo[nPos].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_LongTerm, Force1D));

		//Composite Short Term Force
		Force1D.adForce[dgn::def::CForce1D::Fx] = ForceST.dFxx[nPos];
		Force1D.adForce[dgn::def::CForce1D::Fy] = ForceST.dFyy[nPos];
		Force1D.adForce[dgn::def::CForce1D::Fz] = ForceST.dFzz[nPos];
		Force1D.adForce[dgn::def::CForce1D::Mx] = ForceST.dMux[nPos];
		Force1D.adForce[dgn::def::CForce1D::My] = ForceST.dMuy[nPos];
		Force1D.adForce[dgn::def::CForce1D::Mz] = ForceST.dMuz[nPos];
		pData->vPanelInfo[nPos].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_ShortTerm, Force1D));
	}

	return true;
}

bool CDgnCsgDataBase::GetPanelInfoForce(_DGN_FORC_CRC& rForNC, _DGN_FORC_CRC& rForLT, _DGN_FORC_CRC& rForST) const
{
	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();

	UINT ElemK = GetCheckElem();
	auto EPairK = GetCheckElemPairK();
	UINT unPos = GetCheckPosition();
	int nDgnLcom = GetDgnLcomNo();

	//Girder Force
	CPG_CSIF_D NCStageD;
	GetGirderStageInfo(EPairK, NCStageD);

	T_STAG_K nGdLastStagK = 0;
	int nLastStepNo = 0;
	m_pForcCtrl->GetGirderLastStageKey(EPairK, nGdLastStagK, nLastStepNo);

	int nForceBeforeType = 1;

	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	int nDgnLcomIndex = nDgnLcom - 1;

	T_WEBP_K WebpK = 0;
	T_WEBP_D WebpD;
	BOOL bWebKey = m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK, WebpK);
	BOOL bWebpD = m_pDoc->m_pAttrCtrl2->GetWebp(WebpK, WebpD);

	if ( WebpD.aElemList.GetSize()>1 )
	{
		double dMy_max = -DBL_MAX;
		double dVz_max = -DBL_MAX;
		T_ELEM_K MyElemK = 0;
		T_ELEM_K VzElemK = 0;
		int nMyPos = 0;
		int nVzPos = 0;
		BOOL bConvertM = FALSE;
		BOOL bConvertV = FALSE;
		for ( INT_PTR nEl=0; nEl<WebpD.aElemList.GetSize(); ++nEl )
		{
			T_ELEM_K nCurElemK = WebpD.aElemList[nEl];
			ElemPairK CurEPairK(nCurElemK, EN_EL_BEAM);

			_DGN_FORC_CRC ForceNC;
			if ( !( nEl == 0 || nEl == WebpD.aElemList.GetSize() - 1 ) ) continue;

			int nPos = nEl == 0 ? 0 : 1;
			m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, ( T_STCT_D& ) m_StctD, CurEPairK, NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcom, nForceBeforeType, ForceNC);

			ADGNFORCE aForceLcom;
			ADGNFORCE aForceTnpr;
			ADGNFORCE aForceTnsc;
			ADGNFORCE aForceLTerm;
			ADGNFORCE aForceSTerm;
			ADGNSHRSADD aStssLcom;
			if ( m_pForcCtrl->Get_LcomDataForDesign(CurEPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum) )
			{
				int nLcomSize = aForceLcom.GetSize();
				if ( nLcomSize == 0 ) ASSERT(0);
				if ( nDgnLcomIndex < 0 || nDgnLcomIndex >= nLcomSize )
				{
					ASSERT(0);
				}
			}
			else
				ASSERT(0);

			_DGN_FORC_CRC ForceST = aForceLcom[nDgnLcomIndex];
			_DGN_FORC_CRC ForceLT = aForceLTerm[nDgnLcomIndex];
			_DGN_FORC_CRC ForceSum = aForceLcom[nDgnLcomIndex];
			if ( bIsPLCB == TRUE )
			{
				ForceST -= ForceNC;
				ForceST -= ForceLT;
			}
			else
			{
				ForceST -= ForceLT;
				ForceLT -= ForceNC;
			}

			rForNC.SetData(nPos, nPos, ForceNC);
			rForLT.SetData(nPos, nPos, ForceLT);
			rForST.SetData(nPos, nPos, ForceST);
		}
	}
	else
	{
		_DGN_FORC_CRC ForceNC;
		m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, ( T_STCT_D& ) m_StctD, EPairK, NCStageD.StagK, NCStageD.nSerialStepK, nDgnLcom, nForceBeforeType, ForceNC);

		ADGNFORCE aForceLcom;
		ADGNFORCE aForceTnpr;
		ADGNFORCE aForceTnsc;
		ADGNFORCE aForceLTerm;
		ADGNFORCE aForceSTerm;
		ADGNSHRSADD aStssLcom;
		if ( m_pForcCtrl->Get_LcomDataForDesign(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum) )
		{
			int nLcomSize = aForceLcom.GetSize();
			if ( nLcomSize == 0 ) ASSERT(0);
			if ( nDgnLcomIndex < 0 || nDgnLcomIndex >= nLcomSize )
			{
				ASSERT(0);
			}
		}
		else
			ASSERT(0);

		_DGN_FORC_CRC ForceST = aForceLcom[nDgnLcomIndex];
		_DGN_FORC_CRC ForceLT = aForceLTerm[nDgnLcomIndex];
		_DGN_FORC_CRC ForceSum = aForceLcom[nDgnLcomIndex];
		if ( bIsPLCB == TRUE )
		{
			ForceST -= ForceNC;
			ForceST -= ForceLT;
		}
		else
		{
			ForceST -= ForceLT;
			ForceLT -= ForceNC;
		}

		rForNC = ForceNC;
		rForLT = ForceLT;
		rForST = ForceST;
	}

	return true;
}

bool CDgnCsgDataBase::GetPanelInfoForceRpt(dgn::def::CPanelInfoArray* pData) const
{
	for ( int i = 0; i < 2; ++i )
	{
		CForce1D Force1D;
		const T_CSGD_FORCE& ForGR = m_Panelij[i].ForNC;
		Force1D.adForce[CForce1D::Fx] = ForGR.GetFx();
		Force1D.adForce[CForce1D::Fy] = ForGR.GetFy();
		Force1D.adForce[CForce1D::Fz] = ForGR.GetFz();
		Force1D.adForce[CForce1D::Mx] = ForGR.GetMx();
		Force1D.adForce[CForce1D::My] = ForGR.GetMy();
		Force1D.adForce[CForce1D::Mz] = ForGR.GetMz();
		pData->vPanelInfo[i].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Girder, Force1D));

		const T_CSGD_FORCE& ForLT = m_Panelij[i].ForLT;
		Force1D.adForce[CForce1D::Fx] = ForLT.GetFx();
		Force1D.adForce[CForce1D::Fy] = ForLT.GetFy();
		Force1D.adForce[CForce1D::Fz] = ForLT.GetFz();
		Force1D.adForce[CForce1D::Mx] = ForLT.GetMx();
		Force1D.adForce[CForce1D::My] = ForLT.GetMy();
		Force1D.adForce[CForce1D::Mz] = ForLT.GetMz();
		pData->vPanelInfo[i].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_LongTerm, Force1D));

		const T_CSGD_FORCE& ForST = m_Panelij[i].ForST;
		Force1D.adForce[CForce1D::Fx] = ForST.GetFx();
		Force1D.adForce[CForce1D::Fy] = ForST.GetFy();
		Force1D.adForce[CForce1D::Fz] = ForST.GetFz();
		Force1D.adForce[CForce1D::Mx] = ForST.GetMx();
		Force1D.adForce[CForce1D::My] = ForST.GetMy();
		Force1D.adForce[CForce1D::Mz] = ForST.GetMz();
		pData->vPanelInfo[i].Force1DArr.adForce.push_back(std::make_pair(dgn::def::CForce1DArray::Composited_ShortTerm, Force1D));
	}

	return true;
}

bool CDgnCsgDataBase::GetPanelInfoSectionProperty(dgn::def::CPanelInfoArray* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT nPos = GetCheckPosition();
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	T_WEBP_K WebpK = 0;
	if ( m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK, WebpK) )
	{
		T_WEBP_D WebpD;
		if ( m_pDoc->m_pAttrCtrl2->GetWebp(WebpK, WebpD) )
		{
			for ( INT_PTR nEl = 0; nEl < WebpD.aElemList.GetSize(); ++nEl )
			{
				T_ELEM_K nCurElemK = WebpD.aElemList[nEl];

				T_ELEM_D ElemD; ElemD.Initialize();
				if ( !m_pDoc->m_pPostCtrl->GetElemPost(nCurElemK, ElemD) ) continue;

				CSG_SECT_ELEM SectElem;
				if (!GetCsgSectElem(EPairK, SectElem)) continue;

				const CSG_SECT_POSD& SectPosD = SectElem.SectPos[nPos];

				const DGN_SECT_STIFFNESS& GirderStiff = SectElem.SectPos[nPos].SectInfo.Before.Stiffness;
				const DGN_SECT_STIFFNESS& RebarSect   = SectElem.SectPos[nPos].RbarStiffn;
				const DGN_SECT_STIFFNESS& RebarSectLT = SectElem.SectPos[nPos].RbarStiff3n;
				const DGN_SECT_STIFFNESS& NonSlabStiff= SectElem.SectPos[nPos].NonSlabStiff;

				if ( nEl == 0 )
				{
					ConvertDgnSectStiffToSectionProperty(GirderStiff, &pData->vPanelInfo[0].PropGr);
					ConvertDgnSectStiffToCompositeSectionProperty(SectPosD, &pData->vPanelInfo[0].Prop);
				}
				if ( nEl == WebpD.aElemList.GetSize() - 1 )
				{
					ConvertDgnSectStiffToSectionProperty(GirderStiff, &pData->vPanelInfo[1].PropGr);
					ConvertDgnSectStiffToCompositeSectionProperty(SectPosD, &pData->vPanelInfo[1].Prop);
				}
			}
		}
	}
	else
	{
		T_ELEM_D ElemD; ElemD.Initialize();
		if ( !m_pDoc->m_pPostCtrl->GetElemPost(GetCheckElem(), ElemD) ) return false;

		CSG_SECT_ELEM SectElem;
		if (!GetCsgSectElem(EPairK, SectElem)) return false;

		const CSG_SECT_POSD& SectPosD = SectElem.SectPos[nPos];

		const DGN_SECT_STIFFNESS& GirderStiff = SectElem.SectPos[nPos].SectInfo.Before.Stiffness;

		for ( int i = 0; i < 2; ++i )
		{
			ConvertDgnSectStiffToSectionProperty(GirderStiff, &pData->vPanelInfo[i].PropGr);
			ConvertDgnSectStiffToCompositeSectionProperty(SectPosD, &pData->vPanelInfo[i].Prop);
		}
	}
	return true;
}

bool CDgnCsgDataBase::GetResidualStressType(dgn::def::CResidualStressType* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT nPos = GetCheckPosition();

	T_CRST_D CrstD; CrstD.Initialize();

	if ( m_pDoc->m_pAttrCtrl2->GetCrst(ElemK, CrstD) )
	{
		int nType = CrstD.bBothIJ ? CrstD.CrstBase[0].nType : CrstD.CrstBase[nPos].nType;
		switch ( nType )
		{
		case 0: //0:SR, 1:HR,  2:LW,  3:HW
			pData->enRStype = CResidualStressType::RS_StressRelieved;
			break;
		case 1:
			pData->enRStype = CResidualStressType::RS_HotRolled;
			break;
		case 2:
			pData->enRStype = CResidualStressType::RS_LightlyWeldedLongitudinally;
			break;
		case 3:
			pData->enRStype = CResidualStressType::RS_HeavilyWeldedLongitudinally;
			break;
		default: ASSERT(0); break;
		}
	}
	else
	{
		pData->enRStype = CResidualStressType::RS_LightlyWeldedLongitudinally;
	}

	return true;
}

bool CDgnCsgDataBase::GetStiffnessScaleFactor(dgn::def::CStiffnessScaleFactorArray* pData) const
{
	UINT ElemK = GetCheckElem();
	UINT nPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	auto _l_SSFactor = [ & ] (UINT nPos, const T_SECF_D& SecfD, CStiffnessScaleFactor& rSSFactor) -> void
	{
		const double* pSFactor = ( SecfD.bSameIJ && nPos == 1 ) ? SecfD.dScaleFactorJ : SecfD.dScaleFactor;
		rSSFactor.adFactor[CStiffnessScaleFactor::fArea]   = pSFactor[0];
		rSSFactor.adFactor[CStiffnessScaleFactor::fAsx]    = pSFactor[1];
		rSSFactor.adFactor[CStiffnessScaleFactor::fAsy]    = pSFactor[2];
		rSSFactor.adFactor[CStiffnessScaleFactor::fJ]      = pSFactor[3];
		rSSFactor.adFactor[CStiffnessScaleFactor::fIx]     = pSFactor[4];
		rSSFactor.adFactor[CStiffnessScaleFactor::fIy]     = pSFactor[5];
		rSSFactor.adFactor[CStiffnessScaleFactor::fWeight] = pSFactor[6];
	};

	T_ACTL_D ActlD; ActlD.Initialize();
	m_pDoc->m_pAttrCtrl->GetActl(ActlD);

	pData->vFactor.clear();
	pData->vFactor.resize(2);

	pData->vFactor[0].Init();
	pData->vFactor[1].Init();

	CArray<T_SECF_K, T_SECF_K> aKey;
	CArray<T_SECF_D, T_SECF_D&> aSecf;

	m_pDoc->m_pAttrCtrl->GetSecfSect(ElemD.elpro, aKey, aSecf);

	if ( ActlD.bConsiderSecf )
	{
		if ( aKey.GetSize() > 0 )
		{
			// 이건 어떻게 할당 해야 하나..

			int nBefore = 0;
			int nAfter = 0;
			for ( int i = 0; i < aSecf.GetSize(); ++i )
			{
				//CStiffnessScaleFactor SCFactor;
				const T_SECF_D& SecfD = aSecf[i];
				if ( nBefore == 0 && SecfD.bBefore )
				{
					_l_SSFactor(nPos, SecfD, pData->vFactor[0]);
					nBefore++;
				}
				//
				if ( nAfter == 0 && SecfD.bAfter )
				{
					_l_SSFactor(nPos, SecfD, pData->vFactor[1]);
					nAfter++;
				}
			}
		}
	}

	return true;
}

bool CDgnCsgDataBase::GetCsgSectElem(ElemPairK EPairK, CSG_SECT_ELEM& SectElem) const
{
	const auto& itSect4E = m_mCsgElemSect.find(EPairK);
	if (itSect4E != m_mCsgElemSect.end())
	{
		const auto& itSect = m_mCsgSect.find(itSect4E->second);
		if (itSect != m_mCsgSect.end())
		{
			SectElem = itSect->second;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CDgnCsgDataBase::UpdateShearData(const int& nDir, std::wstring& strLcb, std::wstring& strPosi, dgn::def::CDgnInput* pInp)
{
	return true;
}

double CDgnCsgDataBase::GetH(const CSG_SECT_POSD& rData) const
{
	double dHw    = GetHw(rData);
	double dTfTop = GetTfTop(rData);
	double dTfBot = GetTfBot(rData);
	return dTfTop + dHw + dTfBot;
}

double CDgnCsgDataBase::GetHw(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	return Before.Size[0];
}

double CDgnCsgDataBase::GetTw(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	return Before.Size[1];
}

double CDgnCsgDataBase::GetBfTop(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return Before.Size[2];
	case DGN_SECT_TYPE_COMPO_B:
		{
			double dBfInt = GetBfIntTop(rData);
			double dBfOut = GetBfOutTop(rData);
			return dBfInt + 2.0*dBfOut;
		}
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return Before.Size[3];
		}
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetTfTop(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return Before.Size[3];
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[4];
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfIntTop(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_B:
		return Before.Size[2] + GetTw(rData);
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			double dBVoid = Before.Size[2];
			double dBf = GetBfTop(rData);
			double dB = 2.0*dBf + dBVoid;
			double dBfOutTop = GetBfOutTop(rData);
			double dTw = GetTw(rData);
			return dB - 2.0*dBfOutTop + dTw;
		}
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfTopVoid(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return Before.Size[2];
		}
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfOutTop(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_B:
		return Before.Size[3] - 0.5*GetTw(rData);
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[8] - 0.5*GetTw(rData);
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfOut2Top(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[3] - Before.Size[8] + 0.5*GetTw(rData);
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfBot(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return Before.Size[4];
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			double dBfInt = GetBfIntBot(rData);
			double dBfOut = GetBfOutBot(rData);
			return dBfInt + 2.0*dBfOut;
		}
	default:
		ASSERT(0);
		break;
	}

	return 0.0;
}

double CDgnCsgDataBase::GetTfBot(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return Before.Size[5];
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[7];
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfIntBot(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[5] + GetTw(rData);
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBfOutBot(const CSG_SECT_POSD& rData) const
{
	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		return Before.Size[6] - 0.5*GetTw(rData);
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

double CDgnCsgDataBase::GetBc(const CSG_SECT_POSD& rData) const
{
	return rData.SectInfo.Slab.Size[0];
}

double CDgnCsgDataBase::GetTc(const CSG_SECT_POSD& rData) const
{
	return rData.SectInfo.Slab.Size[1];
}

double CDgnCsgDataBase::GetHh(const CSG_SECT_POSD& rData) const
{
	return rData.SectInfo.Slab.Size[2];
}

double CDgnCsgDataBase::GetWebSlope(const CSG_SECT_POSD& rData) const
{
	double dhw = GetHw(rData);
	double dAngle = 0.0;

	const ST_SECT_SECTBASE_D& Before = rData.SectInfo.Before;
	switch ( rData.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return acos(1.0);
	case DGN_SECT_TYPE_COMPO_B:
		{
			double dbft = GetBfIntTop(rData);
			double dbfb = GetBfIntBot(rData);
			double dL   = fabs(dbft - dbfb) / 2.0;
			dAngle = EQ0(dhw) ? 0.0 : atan(dL / dhw);

			return dAngle;
		}
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			double dbft_int = GetBfIntTop(rData);
			double dbfb_int = GetBfIntBot(rData);
			double dL       = fabs(dbft_int - dbfb_int) / 2.0;
			dAngle = EQ0(dhw) ? 0.0 : atan(dL / dhw);

			return dAngle;
		}
	default:
		ASSERT(0);
		break;
	}
	return 0.0;
}

bool CDgnCsgDataBase::AreThere3orMoreLStiffenerInWeb()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	ST_SECT_SECTBASE_D SectBaseD = SectPos.SectInfo.Before;

	int nSectPart = SectPos.SectInfo.nStype == DGN_SECT_TYPE_COMPO_I ? 2 : 4;

	int nLeftWebLS  = SectBaseD.Stiffener.aStiffLeft.GetSize();
	int nRightWebLS = SectBaseD.Stiffener.aStiffRight.GetSize();

	int nLSSize = 0;
	switch ( SectPos.SectInfo.nStype )
	{
	case D_SECT_TYPE_COMPO_I:
		{
			nLSSize = nLeftWebLS + nRightWebLS;
			return ( nLSSize > 2 );
		}
		break;
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_TUB:
		{
			// Box, Tub 경우는 보강재 상관없이 검토 하는 것으로
			// 하나라도 있으면 검토
			return ( IsFlangeWithLS() || IsWebWithLS() );
		}
		break;
	default: ASSERT(0);
		break;
	}

	return false;
}

bool CDgnCsgDataBase::IsFlangeWithLS()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return false;
	case DGN_SECT_TYPE_COMPO_B:
		{
			return ( SectB.Stiffener.aStiffTop.GetSize() > 0 || SectB.Stiffener.aStiffBottom.GetSize() > 0 );
		}
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return ( SectB.Stiffener.aStiffBottom.GetSize() > 0 );
		}
	default: ASSERT(0); break;
	}

	return false;
}

bool CDgnCsgDataBase::IsFlangeChecking()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		return false;
	case DGN_SECT_TYPE_COMPO_B:
		{
			// 보강재가 없고 Not compact 인 경우는 6.3.2절을 검토해야 하기 때문에 보강재 유무 판단 하지 않고 수행.
			return true; //(IsFlangeWithLS() || IsWebWithLS());
		}
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return true; // (IsFlangeWithLS() || IsWebWithLS());
		}
	default: ASSERT(0); break;
	}

	return false;
}

bool CDgnCsgDataBase::IsWebWithLS()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			return ( SectB.Stiffener.aStiffLeft.GetSize() > 0 || SectB.Stiffener.aStiffRight.GetSize() > 0 );
		}
	default: ASSERT(0); break;
	}

	return false;
}

bool CDgnCsgDataBase::IsLongitudinalStiffener()
{
	return ( IsFlangeWithLS() || IsWebWithLS() );
}

bool CDgnCsgDataBase::IsBendingChecking(bool bPositiveM)
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		{
			if ( bPositiveM )
			{
				return true;
			}
			else
			{
				if ( !IsLongitudinalStiffener() )
				{
					return true;
				}
				else if ( AreThere3orMoreLStiffenerInWeb() )
				{
				}
				// 보강재 있으면 검토 필요 없음
				return !IsLongitudinalStiffener();
			}
		}
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			if ( bPositiveM )
			{
				return true;
			}
			else
			{
				// 보강재 있으면 검토 필요 없음
				return !IsLongitudinalStiffener();
			}
		}
	default: ASSERT(0); break;
	}

	return false;
}

bool CDgnCsgDataBase::IsCombinedMVChecking()
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	T_ELEM_D ElemD; ElemD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD) ) return false;

	CSG_SECT_ELEM SectElem;
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	if (!GetCsgSectElem(EPairK, SectElem)) return false;

	const CSG_SECT_POSD& SectPos = SectElem.SectPos[unPos];
	const ST_SECT_SECTBASE_D& SectB = SectPos.SectInfo.Before;

	switch ( SectPos.SectInfo.nStype )
	{
	case DGN_SECT_TYPE_COMPO_I:
		{
			if ( AreThere3orMoreLStiffenerInWeb() )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case DGN_SECT_TYPE_COMPO_B:
	case DGN_SECT_TYPE_COMPO_TUB:
		{
			// 보강재 있으면 검토 필요 없음
			return !IsLongitudinalStiffener();
		}
	default: ASSERT(0); break;
	}

	return false;
}

double CDgnCsgDataBase::GetForceMuy() const
{
	UINT ElemK = GetCheckElem();
	UINT unPos = GetCheckPosition();

	int nDgnLcom = GetDgnLcomNo();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	int nForceBeforeType = 1;

	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	int nDgnLcomIndex = nDgnLcom - 1;

	ADGNFORCE aForceLcom;
	ADGNFORCE aForceTnpr;
	ADGNFORCE aForceTnsc;
	ADGNFORCE aForceLTerm;
	ADGNFORCE aForceSTerm;
	ADGNSHRSADD aStssLcom;
	if ( m_pForcCtrl->Get_LcomDataForDesign(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum) )
	{
		int nLcomSize = aForceLcom.GetSize();
		if ( nLcomSize == 0 ) ASSERT(0);
		if ( nDgnLcomIndex < 0 || nDgnLcomIndex >= nLcomSize )
		{
			ASSERT(0);
		}
	}
	else
		ASSERT(0);

	const _DGN_FORC_CRC& ForceSum = aForceLcom[nDgnLcomIndex];

	return ForceSum.dMuy[unPos];
}

void CDgnCsgDataBase::ConvertDgnSectStiffToSectionProperty(const DGN_SECT_STIFFNESS& rDgnStiff, dgn::def::CSectionProperty* pData) const
{
	pData->adProp[dgn::def::CSectionProperty::Area] = rDgnStiff.dArea;
	pData->adProp[dgn::def::CSectionProperty::J]    = rDgnStiff.dIxx;
	pData->adProp[dgn::def::CSectionProperty::Ix]   = rDgnStiff.dIyy;
	pData->adProp[dgn::def::CSectionProperty::Iy]   = rDgnStiff.dIzz;
	//pData->adProp[dgn::def::CSectionProperty::Ixy]  = rDgnStiff.
	pData->adProp[dgn::def::CSectionProperty::Sx]   = rDgnStiff.dSyy;
	pData->adProp[dgn::def::CSectionProperty::Sy]   = rDgnStiff.dSzz;
	pData->adProp[dgn::def::CSectionProperty::Zx]   = rDgnStiff.dZyy;
	pData->adProp[dgn::def::CSectionProperty::Zy]   = rDgnStiff.dZzz;
	pData->adProp[dgn::def::CSectionProperty::Cx]   = rDgnStiff.dCym;
	pData->adProp[dgn::def::CSectionProperty::Cxm]  = rDgnStiff.dCym;
	pData->adProp[dgn::def::CSectionProperty::Cxp]  = rDgnStiff.dCyp;
	pData->adProp[dgn::def::CSectionProperty::Cy]   = rDgnStiff.dCzm;
	pData->adProp[dgn::def::CSectionProperty::Cym]  = rDgnStiff.dCzm;
	pData->adProp[dgn::def::CSectionProperty::Cyp]  = rDgnStiff.dCzp;
	pData->adProp[dgn::def::CSectionProperty::x1]   = rDgnStiff.dx1;
	pData->adProp[dgn::def::CSectionProperty::y1]   = rDgnStiff.dy1;
	pData->adProp[dgn::def::CSectionProperty::x2]   = rDgnStiff.dx2;
	pData->adProp[dgn::def::CSectionProperty::y2]   = rDgnStiff.dy2;
	pData->adProp[dgn::def::CSectionProperty::x3]   = rDgnStiff.dx3;
	pData->adProp[dgn::def::CSectionProperty::y3]   = rDgnStiff.dy3;
	pData->adProp[dgn::def::CSectionProperty::x4]   = rDgnStiff.dx4;
	pData->adProp[dgn::def::CSectionProperty::y4]   = rDgnStiff.dy4;
	pData->adProp[dgn::def::CSectionProperty::Xbar] = rDgnStiff.dYBar;
	pData->adProp[dgn::def::CSectionProperty::Ybar] = rDgnStiff.dZBar;
	pData->adProp[dgn::def::CSectionProperty::Rox]  = EQ0(rDgnStiff.dRoy) ? sqrt(SafeDiv(rDgnStiff.dIyy, rDgnStiff.dArea)) : rDgnStiff.dRoy;
	pData->adProp[dgn::def::CSectionProperty::Roy]  = EQ0(rDgnStiff.dRoz) ? sqrt(SafeDiv(rDgnStiff.dIzz, rDgnStiff.dArea)) : rDgnStiff.dRoz;
	pData->adProp[dgn::def::CSectionProperty::Rop]  = rDgnStiff.dRop;
	pData->adProp[dgn::def::CSectionProperty::Asx]  = rDgnStiff.dAsy;
	pData->adProp[dgn::def::CSectionProperty::Asy]  = rDgnStiff.dAsz;
	pData->adProp[dgn::def::CSectionProperty::Cw]   = rDgnStiff.dCwp;
	//     pData->adProp[dgn::def::CSectionProperty::Qxb] = m_Qyb[m_iPosiNo];
	//     pData->adProp[dgn::def::CSectionProperty::Qyb] = m_Qzb[m_iPosiNo];
}

void CDgnCsgDataBase::ConvertDgnSectStiffToCompositeSectionProperty(const CSG_SECT_POSD& rSectPos, dgn::def::CCompositeSectionProperty* pData) const
{
	ConvertDgnSectStiffToSectionPropertyArr(rSectPos.RbarStiffn, pData->adProp);
	ConvertDgnSectStiffToSectionPropertyArr(rSectPos.RbarStiff3n, pData->adPropLT);
	ConvertDgnSectStiffToSectionPropertyArr(rSectPos.NonSlabStiff, pData->adPropNonSlab);
}

void CDgnCsgDataBase::ConvertDgnSectStiffToSectionPropertyArr(const DGN_SECT_STIFFNESS& rDgnStiff, std::array<double, CSectionProperty::Number>& adProp) const
{
	adProp[dgn::def::CSectionProperty::Area] = rDgnStiff.dArea;
	adProp[dgn::def::CSectionProperty::J]    = rDgnStiff.dIxx;
	adProp[dgn::def::CSectionProperty::Ix]   = rDgnStiff.dIyy;
	adProp[dgn::def::CSectionProperty::Iy]   = rDgnStiff.dIzz;
	//adProp[dgn::def::CSectionProperty::Ixy]  = rDgnStiff.
	adProp[dgn::def::CSectionProperty::Sx]   = rDgnStiff.dSyy;
	adProp[dgn::def::CSectionProperty::Sy]   = rDgnStiff.dSzz;
	adProp[dgn::def::CSectionProperty::Zx]   = rDgnStiff.dZyy;
	adProp[dgn::def::CSectionProperty::Zy]   = rDgnStiff.dZzz;
	adProp[dgn::def::CSectionProperty::Cx]   = rDgnStiff.dCym;
	adProp[dgn::def::CSectionProperty::Cxm]  = rDgnStiff.dCym;
	adProp[dgn::def::CSectionProperty::Cxp]  = rDgnStiff.dCyp;
	adProp[dgn::def::CSectionProperty::Cy]   = rDgnStiff.dCzm;
	adProp[dgn::def::CSectionProperty::Cym]  = rDgnStiff.dCzm;
	adProp[dgn::def::CSectionProperty::Cyp]  = rDgnStiff.dCzp;
	adProp[dgn::def::CSectionProperty::x1]   = rDgnStiff.dx1;
	adProp[dgn::def::CSectionProperty::y1]   = rDgnStiff.dy1;
	adProp[dgn::def::CSectionProperty::x2]   = rDgnStiff.dx2;
	adProp[dgn::def::CSectionProperty::y2]   = rDgnStiff.dy2;
	adProp[dgn::def::CSectionProperty::x3]   = rDgnStiff.dx3;
	adProp[dgn::def::CSectionProperty::y3]   = rDgnStiff.dy3;
	adProp[dgn::def::CSectionProperty::x4]   = rDgnStiff.dx4;
	adProp[dgn::def::CSectionProperty::y4]   = rDgnStiff.dy4;
	adProp[dgn::def::CSectionProperty::Xbar] = rDgnStiff.dYBar;
	adProp[dgn::def::CSectionProperty::Ybar] = rDgnStiff.dZBar;
	adProp[dgn::def::CSectionProperty::Rox]  = EQ0(rDgnStiff.dRoy) ? sqrt(SafeDiv(rDgnStiff.dIyy, rDgnStiff.dArea)) : rDgnStiff.dRoy;
	adProp[dgn::def::CSectionProperty::Roy]  = EQ0(rDgnStiff.dRoz) ? sqrt(SafeDiv(rDgnStiff.dIzz, rDgnStiff.dArea)) : rDgnStiff.dRoz;
	adProp[dgn::def::CSectionProperty::Rop]  = rDgnStiff.dRop;
	adProp[dgn::def::CSectionProperty::Asx]  = rDgnStiff.dAsy;
	adProp[dgn::def::CSectionProperty::Asy]  = rDgnStiff.dAsz;
	adProp[dgn::def::CSectionProperty::Cw]   = rDgnStiff.dCwp;
}

void CDgnCsgDataBase::SetDgnForce(const T_CSGF_MEMB& rData)
{
	m_DgnForD = rData;
}

void CDgnCsgDataBase::SetPanelForce(const T_CSGD_COMF Panelij[2])
{
	for ( int i=0; i<2; ++i ) m_Panelij[i] = Panelij[i];
}

void CDgnCsgDataBase::SetDgnGirderStress(const _DGN_STRS_CRC& rData)
{
	m_GirderStress = rData;
}

void CDgnCsgDataBase::SetDgnSlabStress(const _DGN_STRS_CRC& rData)
{
	m_SlabStress = rData;
}

void CDgnCsgDataBase::SetReportMode(const bool& bReport)
{
	m_bReport = bReport;
}

bool CDgnCsgDataBase::IsReportMode() const
{
	return m_bReport;
}