#include "StdAfx.h"

#include "../dgnengine/idesign/DGN_link/IDgnPerform.h"
#include "../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../dgnengine/idesign/DGN_def/DgnLcomDef.h"
#include "../dgnengine/idesign/DGN_lib/Macro.h"

#include "../dgnengine/idesign/DGN_def/DgnRevisionDef.h"

#include "../wg_db/DBDoc.h"

#include "ProductDBDocHelper.h"
#include "IProductDataBaseHelper.h"
#include "IProductResultSaver.h"
#include "TDBDataFunctor.h"
#include "DgnStruct.h"
#include <mutex>
#include "TEngineResult.h"
#include "IDgnPerformDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::link;
using namespace dgn::def;
using namespace dgn::converter;

IProductResultSaver::IProductResultSaver(const CProductDBDocHelper* pDBDocHelper,
										 const TProductDgnType& TDgnType,
										 const TProductMembType& TMembType) :
	m_pDBDocHelper(pDBDocHelper),
	m_TDgnType(TDgnType),
	m_TMembType(TMembType),
	m_MembKey(T_KEY()),
	m_DgnMode(enProductDgnMode())
{
}

IProductResultSaver::~IProductResultSaver()
{
}

TProductDgnType IProductResultSaver::GetTDgnType() const
{
	return m_TDgnType;
}

TProductMembType IProductResultSaver::GetTMembType() const
{
	return m_TMembType;
}

bool IProductResultSaver::Initialize(T_KEY MembKey)
{
	return DoInitialize(MembKey);
}

bool IProductResultSaver::GetEngineResultRepInfo(dgn::link::TEngineResultInfo& rInfo) const
{
	return DoGetEngineResultRepInfo(rInfo);
}

bool IProductResultSaver::SaveDgnResult(const TEngineResult& crEResultAll, 
										IDgnPerformDataBase* pIDataBase) const
{
	return DoSaveDgnResult(crEResultAll, pIDataBase);
}

bool IProductResultSaver::SaveDgnForce(const TEngineResult& crEResultAll, 
									   IDgnPerformDataBase* pIDataBase) const
{
	return DoSaveDgnForce(crEResultAll, pIDataBase);
}

bool IProductResultSaver::SetResultFlag() const
{
	return DoSetResultFlag();
}

bool IProductResultSaver::WriteFileDgnResult(IDgnPerformDataBase* pIDataBase) const
{
	return DoWriteFileDgnResult(pIDataBase);
}

bool IProductResultSaver::WriteFileDgnForce(IDgnPerformDataBase* pIDataBase) const
{
	return DoWriteFileDgnForce(pIDataBase);
}

bool dgn::converter::IProductResultSaver::MakePDgnLcomKeyMap()
{
	mPDgnLcomKeyMap.clear();

	std::vector<DWORD_PTR> vDgnLcomItr;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
	if ( pDocCtrl == nullptr ) return false;
	dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

	dgn::def::enDgn nDgnType = GetTDgnType().GetEDgnType();
	int nDgnLcom = pDgnDoc->GetDgnPerform()->GetDgnLcomItrList((int)nDgnType, vDgnLcomItr);

	//여기 좀 별론데 매칭을 잘시키는 방법이 없나? 꼭 이래야됨?
	for ( int i = 0; i<nDgnLcom; ++i )
	{
		const auto& aLcomTotal = GetLcomList();

		const auto* pEDgnLcom = TDBDataFunctor<CDgnLcom>()(vDgnLcomItr[i]);
		if ( !pEDgnLcom )
		{
			ASSERT(0); continue;
		}

		const auto& OriginalLcomKey = FITR_TO_KEY(pEDgnLcom->itrLcom);
		const auto& bMaxLcom = pEDgnLcom->GetConcurrentItem() == CLcomInfo::EN_LCOM_CONCURRENT_ITEM_MIN ? false : true;
		const auto nConcurrentIdx = pEDgnLcom->GetConcurrentDofIdx();
		auto pos = aLcomTotal->GetStartPosition();
		while ( pos )
		{
			auto PDgnLcomKey = 0U;
			_DGN_LCOM PDgnLcom;
			aLcomTotal->GetNextAssoc(pos, PDgnLcomKey, PDgnLcom);

			if ( PDgnLcom.OriginalLcomKey == OriginalLcomKey && PDgnLcom.bMaxLcom == static_cast<BOOL>(bMaxLcom) && PDgnLcom.iLcomCompType == nConcurrentIdx )
			{
				mPDgnLcomKeyMap[vDgnLcomItr[i]] = PDgnLcomKey;
				break;
			}
		}
	}

	return true;
}

bool dgn::converter::IProductResultSaver::SetLcomResult(const UINT& iMemb, const dgn::def::CDgnLesultBundle* pLesultBundle, IDgnPerformDataBase* pIDataBase)
{
	InitializeDefault(iMemb);

	TEngineResult EngineResult;
	EngineResult.pELesultAll = (dgn::def::CDgnLesultBundle*)pLesultBundle;

	if ( !DoSaveDgnResult(EngineResult, pIDataBase) )
	{
		ASSERT(0); return false;
	}

	if ( !DoSaveDgnForce(EngineResult, pIDataBase) )
	{
		ASSERT(0); return false;
	}

	return true;
}

CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&>* dgn::converter::IProductResultSaver::GetLcomList() const
{
	return ((IProductDataBaseHelper*)GetDataBaseHelper())->GetPerformDataBase()->GetLcomList();
}

bool IProductResultSaver::InitializeDefault(T_KEY MembKey)
{
	if ( !SetCurMembKey(MembKey) )
	{
		ASSERT(0); return false;
	}

	if ( !SetDgnMode(MembKey) )
	{
		ASSERT(0); return false;
	}

	GetDataBaseHelper()->SetDgnLcomList(MembKey);
	MakePDgnLcomKeyMap();

	return true;
}

bool IProductResultSaver::IsPLType(FIterator EItrRefer,
								   DataType Type) const
{
	const auto EReferDataType = FITR_TO_DATATYPE(EItrRefer);
	const auto DPoolDataType  = CDataTypeUtil::GetDPoolType(Type);

	return (EReferDataType == DPoolDataType);
}

bool IProductResultSaver::IsPLPointType(FIterator EItrRefer) const
{
	return IsPLType(EItrRefer, DataType::PLevelPoint);
}

bool IProductResultSaver::IsPLPositionType(FIterator EItrRefer) const
{
	return IsPLType(EItrRefer, DataType::PLevelPosition);
}

bool IProductResultSaver::IsPL2DForceType(FIterator EItrRefer) const
{
	return IsPLType(EItrRefer, DataType::PLevel2DForce);
}

bool IProductResultSaver::IsPLCutline(FIterator EItrRefer) const
{
	return IsPLType(EItrRefer, DataType::PLevelCutLine);
}

bool IProductResultSaver::SetCurMembKey(T_KEY MembKey)
{
	if ( MembKey == T_KEY() )
	{
		ASSERT(0); return false;
	}

	m_MembKey = MembKey;

	return true;
}

T_KEY IProductResultSaver::GetCurMembKey() const
{
	return m_MembKey;
}

const CProductDBDocHelper* IProductResultSaver::GetDBDocHelper() const
{
	return m_pDBDocHelper;
}

enProductDgnMode IProductResultSaver::GetCurDgnMode() const
{
	return m_DgnMode;
}

enProductDgnMode IProductResultSaver::GetCurDgnMode(T_KEY MembKey) const
{
	return [this, &MembKey]()
	{
		try
		{
			if (IsEnableDesignMember())
			{
				if (IsDesignMember(MembKey))
				{
					return enProductDgnMode::Design;
				}
			}

			return enProductDgnMode::Checking;
		}
		catch (...)
		{
			ASSERT(0);
			return enProductDgnMode();
		}
	} ();
}

bool IProductResultSaver::SetDgnMode(T_KEY MembKey)
{
	m_DgnMode = [this, &MembKey] ()
	{
		try
		{
			if ( IsEnableDesignMember() )
			{
				if ( IsDesignMember(MembKey) )
				{
					return enProductDgnMode::Design;
				}
			}

			return enProductDgnMode::Checking;
		}
		catch ( ... )
		{
			ASSERT(0);
			return enProductDgnMode();
		}
	} ();

	return true;
}

UINT dgn::converter::IProductResultSaver::GetPDgnLcomKey(idgn::FIterator itrPDgnLcom) const
{
	auto itr = mPDgnLcomKeyMap.find(itrPDgnLcom);
	if ( itr == mPDgnLcomKeyMap.end() )
	{
		return 0U;
	}
	return itr->second;
}
