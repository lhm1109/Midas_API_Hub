#include "stdafx.h"

#include "DgnMemoizationTower.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/DgnDataCtrl.h"
#include "../dgnengine/idesign/DGN_link/IDgnMemoizationLib.h"

CDgnMemoizationTower* CDgnMemoizationTower::m_pTower = nullptr;

CDgnMemoizationTower::CDgnMemoizationTower()
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnMemoizationTower::~CDgnMemoizationTower()
{
}

CDgnMemoizationTower* CDgnMemoizationTower::GetInstance()
{
	if ( m_pTower == nullptr )
	{
		m_pTower = new CDgnMemoizationTower();
	}
	return m_pTower;
}

std::vector<IDgnMemoizationLib::EN_MEMO_TYPE> GetMemoType()
{
	std::vector<IDgnMemoizationLib::EN_MEMO_TYPE> vMemoType;
	vMemoType.emplace_back(IDgnMemoizationLib::EN_MEMO_FIRST_MOMENT_OF_AREA);
	vMemoType.emplace_back(IDgnMemoizationLib::EN_MEMO_PSC_CRACKED_SECTION_PROP);
	return vMemoType;
}

bool CDgnMemoizationTower::InitializeCheck()
{
	if ( !IDgnMemoizationLib::Exist() )
		return false;
	std::vector<IDgnMemoizationLib::EN_MEMO_TYPE> vMemoType = GetMemoType();

	for ( auto& MemoType : vMemoType )
	{
		auto* pInstance = IDgnMemoizationLib::Instance();
		if ( pInstance->IsExist(MemoType) )
		{
			pInstance->DeleteMemo(MemoType);
		}

		pInstance->CreateMemo(MemoType);
	}

	return true;
}

bool CDgnMemoizationTower::InitializeDesign()
{   
	return true;
}

bool CDgnMemoizationTower::Finalize()
{
	std::vector<IDgnMemoizationLib::EN_MEMO_TYPE> vMemoType = GetMemoType();
	if ( IDgnMemoizationLib::Exist() )
	{
		for ( auto& MemoType : vMemoType )
		{
			auto* pInstance = IDgnMemoizationLib::Instance();
			if ( pInstance->IsExist(MemoType) )
			{
				pInstance->DeleteMemo(MemoType);
			}
		}
	}
	
	return true;
}

bool CDgnMemoizationTower::End()
{
	Finalize();

	if ( m_pTower )
	{
		delete m_pTower;
	}
	m_pTower = nullptr;

	return true;
}