#include "StdAfx.h"

#include <array>
#include <vector>
#include <map>

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_PostCtrl.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_UnitCtrl.h"

#include "DgnAnalRsltMembConverter.h"
#include "DgnAnalRsltLoadConverter.h"
#include "DgnAnalRsltUtil.h"

#include "DgnAnalRsltConverter.h"
#include "IDgnPerformDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;

std::mutex CDgnAnalRsltConverter::m_mutex;

CDgnAnalRsltConverter::CDgnAnalRsltConverter(CDBDoc* pDBDoc, T_KEY_LIST& aKeyElem) :
    IAnalRsltConverter(),
    m_pDBDoc(pDBDoc)
{
    m_aElemList.RemoveAll();
    m_aElemList.Append(aKeyElem);

    ClearData();
    SetDirectProduct(true);
}

CDgnAnalRsltConverter::CDgnAnalRsltConverter(CDBDoc* pDBDoc, T_KEY_LIST& aKeyElem, bool bDirectProduct) :
    IAnalRsltConverter(),
    m_pDBDoc(pDBDoc)
{
    m_aElemList.RemoveAll();
    m_aElemList.Append(aKeyElem);
    
    ClearData();
    SetDirectProduct(bDirectProduct);
}

CDgnAnalRsltConverter::~CDgnAnalRsltConverter()
{
}

void CDgnAnalRsltConverter::ClearData()
{
	//m_StrbData.InitData();
	//m_SbcfData.InitData();
	//m_StrtData.InitData();
	//m_StrwData.InitData();
	//m_ElemDispData.InitData();
	//m_NodalDispData.InitData();
	//m_ReacData.InitData();
	//m_StrpData.InitData();
	//m_SpcfData.InitData();
}

void CDgnAnalRsltConverter::InitData()
{
/*
    ClearData();

    std::vector<enAnalSubType> vAnalSubType;
    const int nAnalSub = GetSupportAnalSubType(vAnalSubType);
    if (nAnalSub == 0) return;

    std::vector<DKey> vKeyCase;
    const int nCase = GetLoadConverter()->GetCaseKeyList(vKeyCase);
    if (nCase == 0) return;

	std::unordered_map<T_KEY, int>  meltyp;

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

	int nStrb=0;
	int nSbcf=0;
	int nStrt=0;
	int nStrw=0;	
	int nElemDisp=0;
	int nNodalDisp=0;
	int nReac=0;
	int nStrp = 0;
	int nSpcf = 0;
	for (const auto KeyCase : vKeyCase)
	{
		for (const auto AnalSubType : vAnalSubType)
		{
			std::vector<DKey> vDataKey;
			const int nData = GetMembConverter()->GetKeyList(AnalSubType, vDataKey);
			if (nData == 0) continue;

			if (AnalSubType == enAnalSubType::Force1D6 || AnalSubType == enAnalSubType::Force1D7)
			{
				for (const auto key : vDataKey)
				{
					const auto PKey = GetProductMembConverter()->GetProductKey(AnalSubType, key);
					T_ELEM_D ElemD;
					ElemD.Initialize();
					if (!pAttrCtrl->GetElem(PKey.GetKey(), ElemD))
					{
						ASSERT(0);
						continue;
					}
					meltyp[PKey.GetKey()] = ElemD.eltyp;

					if (pAttrCtrl->IsBeam(ElemD.eltyp))
					{
#if defined _CIVIL
						nSbcf++;
#else 
						nStrb++;
#endif
					}
					else if (pAttrCtrl->IsTruss(ElemD.eltyp))
					{
						nStrt++;
					}
					else if (pAttrCtrl->IsWall(ElemD.eltyp))
					{
						nStrw++;
					}
					else ASSERT(0);
				}
			}
			else if (AnalSubType == enAnalSubType::Force2D || AnalSubType == enAnalSubType::Stress2D)
			{
				nStrp += vDataKey.size();
			}
			else if (AnalSubType == enAnalSubType::Disp1D)
			{
				continue;
			}
			else if (AnalSubType == enAnalSubType::NodalDeform)
			{
				nNodalDisp += vDataKey.size();
			}
			else if (AnalSubType == enAnalSubType::NodalReact)
			{
				nReac += vDataKey.size();
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	if(nStrb>0)      m_StrbData.Reserve(nStrb);
	if(nSbcf>0)      m_SbcfData.Reserve(nSbcf);
	if(nStrt>0)      m_StrtData.Reserve(nStrt);
	if(nStrw>0)      m_StrwData.Reserve(nStrw);
	if(nElemDisp>0)  m_ElemDispData.Reserve(nElemDisp);
	if(nNodalDisp>0) m_NodalDispData.Reserve(nNodalDisp);
	if(nReac>0)      m_ReacData.Reserve(nReac);
	if(nStrp>0)      m_StrpData.Reserve(nStrp);
	if(nSpcf>0)      m_SpcfData.Reserve(nSpcf);

    for (const auto KeyCase : vKeyCase)
    {
        // set loadcase
        if (!SetPostCtrlLoadCase(ConvertCaseKey(KeyCase)))
        {
            ASSERT(0);
            continue;
        }

        for (const auto AnalSubType : vAnalSubType)
        {
            std::vector<DKey> vDataKey;
            const int nData = GetMembConverter()->GetKeyList(AnalSubType, vDataKey);
            if (nData == 0) continue;

            for (const auto key : vDataKey)
            {
                const auto mapKey = std::make_pair(KeyCase, key);
                const auto PKey = GetProductMembConverter()->GetProductKey(AnalSubType, key);

                if(AnalSubType == enAnalSubType::Force1D6 || AnalSubType == enAnalSubType::Force1D7)
                {
					auto itreltyp = meltyp.find(PKey.GetKey());
					if(itreltyp == meltyp.end()) continue;
                    if (pAttrCtrl->IsBeam(itreltyp->second))
                    {
                        auto _l_MakeStrb = [&]() -> void
                        {
                            if ( m_StrbData.Exist(mapKey) ) return;
                            T_STRB_D StrbMax, StrbMin;
                            if ( !pPostCtrl->GetStrbNew(PKey.GetKey(), &StrbMax, &StrbMin, nullptr, TRUE) )
                            {
                                return;
                            }
							m_StrbData.SetData(mapKey, StrbMax, StrbMin);
                        };

                        auto _l_MakeSbcf = [&] () -> void
                        {
							if ( m_SbcfData.Exist(mapKey) ) return;
                            T_SBCF_D SbcfMax, SbcfMin;
                            if ( !pPostCtrl->GetSbcfNew(PKey.GetKey(), &SbcfMax, &SbcfMin, nullptr, TRUE) )
                            {
                                return;
                            }
							m_SbcfData.SetData(mapKey, SbcfMax, SbcfMin);
                        };

#if defined _CIVIL
                        _l_MakeSbcf();
#else 
                        _l_MakeStrb();
#endif
                    }
                    else if (pAttrCtrl->IsTruss(itreltyp->second))
                    {
						if ( m_StrtData.Exist(mapKey) ) continue;
                        T_STRT_D StrtMax, StrtMin;
                        if (!pPostCtrl->GetStrtNew(PKey.GetKey(), &StrtMax, &StrtMin, nullptr))
                        {
                            continue;
                        }
						m_StrtData.SetData(mapKey, StrtMax, StrtMin);
                    }
                    else if (pAttrCtrl->IsWall(itreltyp->second))
                    {
                        if ( m_StrwData.Exist(mapKey) ) continue;
                        UINT unInternalWallID = PKey.GetKey();
                        if (!pPostCtrl->GetInternalWallId(unInternalWallID, unInternalWallID))
                        {
                            ASSERT(FALSE);
                            continue;
                        }

                        T_STRW_D StrwMax, StrwMin;
                        if (!pPostCtrl->GetStrwNew(unInternalWallID, &StrwMax, &StrwMin, nullptr))
                        {
                            ASSERT(0);
                            continue;
                        }
						m_StrwData.SetData(mapKey, StrwMax, StrwMin);
                    }
                    else ASSERT(0);
                }
                else if(AnalSubType == enAnalSubType::Force2D || AnalSubType == enAnalSubType::Stress2D)
                {
					auto _l_MakeStrp = [&] () -> void
					{
						if ( m_StrpData.Exist(mapKey) ) return;
						T_STRP_DL StrpMax, StrpMin;
						if ( !pPostCtrl->GetStrpNew_L(PKey.GetKey(), &StrpMax, &StrpMin, nullptr) )
						{
							return;
						}
						m_StrpData.SetData(mapKey, StrpMax, StrpMin);
					};

					auto _l_MakeSpcf = [&] () -> void
					{
						if ( m_SpcfData.Exist(mapKey) ) return;
						T_SPCF_DL SpcfMax, SpcfMin;
						if ( !pPostCtrl->GetSpcfNew_L(PKey.GetKey(), &SpcfMax, &SpcfMin, nullptr) )
						{
							return;
						}
						m_SpcfData.SetData(mapKey, SpcfMax, SpcfMin);
					};

					_l_MakeStrp();
                }
                else if (AnalSubType == enAnalSubType::Disp1D)
                {
                    continue;
                }
                else if (AnalSubType == enAnalSubType::NodalDeform)
                {
					if ( m_NodalDispData.Exist(mapKey) ) continue;
                    T_DISP_D DispMaxD, DispMinD;
                    if (!pPostCtrl->GetDispNew(PKey.GetKey(), &DispMaxD, &DispMinD, nullptr))
                    {
                        continue;
                    }
					m_NodalDispData.SetData(mapKey, DispMaxD, DispMinD);
                }
                else if (AnalSubType == enAnalSubType::NodalReact)
                {
                    if ( m_ReacData.Exist(mapKey) ) continue;
                    T_REAC_D ReactMaxD, ReactMinD;
                    if (!pPostCtrl->GetReacNew(PKey.GetKey(), &ReactMaxD, &ReactMinD, nullptr))
                    {
                        continue;
                    }
					m_ReacData.SetData(mapKey, ReactMaxD, ReactMinD);
                }
                else
                {
                    ASSERT(0);
                }
            }
        }
    }
*/
}

bool CDgnAnalRsltConverter::SetNeutralUnit() const
{
    T_UNIT_INDEX ChangeIndex;
    GetDBDoc()->m_pUnitCtrl->GetUnitIndexCurrent(ChangeIndex);
    if (ChangeIndex.nBase_Length != D_UNITSYS_LENGTH_INDEX_M ||
        ChangeIndex.nBase_Force != D_UNITSYS_FORCE_INDEX_N) 
    {
        ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
        ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
        GetDBDoc()->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);

        return true;
    }
    
    return false;
}

int CDgnAnalRsltConverter::GetSupportAnalSubType(std::vector<enAnalSubType>& vAnalSubType) const
{
    vAnalSubType.clear();

    vAnalSubType.push_back(enAnalSubType::Force1D6);
    vAnalSubType.push_back(enAnalSubType::Disp1D);
    vAnalSubType.push_back(enAnalSubType::Force1D7);
    vAnalSubType.push_back(enAnalSubType::Force2D);
    vAnalSubType.push_back(enAnalSubType::Stress2D);
    vAnalSubType.push_back(enAnalSubType::NodalDeform);
    vAnalSubType.push_back(enAnalSubType::NodalReact);

    return Macro::GetSize(vAnalSubType);
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
    const std::vector<DKey>& vKeyNode, std::vector<CRawDataNode>& vData) const
{
    switch (AnalType)
    {
    case enAnalType::Deform:
    {
		/*
        if (IsDirectProduct())
        {
            for (const auto Key : vKeyNode)
            {
                CRawDataNode Raw;
                Raw.Init();

				const auto mapKey = std::make_pair(KeyCase, Key);
				T_DISP_D DispMax, DispMin;
                if (m_NodalDispData.GetData(mapKey, DispMax, DispMin))
                {
					CDgnAnalRsltUtil::Convert(DispMax, DispMin, Raw);
                }
                vData.emplace_back(Raw);
            }
            return true;
        }
        else
		*/
        {
            std::vector<PMembKey> vKey;
            if (!ConvertKey(enAnalSubType::NodalDeform, vKeyNode, vKey))
            {
                ASSERT(0); return false;
            }

            return GetAnalRawDataNode(ConvertCaseKey(KeyCase), vKey, vData);
        }
    }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
    const std::vector<DKey>& vKeyNode, std::vector<CRawDataReact>& vData) const
{
    switch (AnalType)
    {
    case enAnalType::React:
    {
		/*
        if(IsDirectProduct())
        {
            for(const auto Key : vKeyNode)
            {
                CRawDataReact Raw;
                Raw.Init();

				const auto mapKey = std::make_pair(KeyCase, Key);
				T_REAC_D ReacMax, ReacMin;
                if(m_ReacData.GetData(mapKey, ReacMax, ReacMin))
                {
					CDgnAnalRsltUtil::Convert(ReacMax, Raw);
                }
                vData.emplace_back(Raw);
            }
            return true;
        }
        else
		*/
        {
            std::vector<PMembKey> vKey;
            if (!ConvertKey(enAnalSubType::NodalReact, vKeyNode, vKey))
            {
                ASSERT(0); return false;
            }

            return GetAnalRawDataReact(ConvertCaseKey(KeyCase), vKey, vData);
        }
    }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
                                        const std::vector<DKey>& vKey1D, std::vector<CRawData1D3>& vData) const
{
    switch ( AnalType )
    {
    case enAnalType::Deform:
        {
		/*
            if (IsDirectProduct())
            {
                for (const auto Key : vKey1D)
                {
                    CRawData1D3 Raw;
                    Raw.Init();
                    vData.emplace_back(Raw);
                }
                return true;
            }
            else
		*/
            {
                std::vector<PMembKey> vKey;
                if (!ConvertKey(enAnalSubType::Disp1D, vKey1D, vKey))
                {
                    ASSERT(0); return false;
                }

                return GetAnalRawDataDisp1D(ConvertCaseKey(KeyCase), vKey, vData);
            }
        }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
                                        const std::vector<DKey>& vKey1D, std::vector<CRawData1D6>& vData) const
{
    switch ( AnalType )
    {
    case enAnalType::Force:
        {
		/*
            if (IsDirectProduct())
            {
                for (const auto Key : vKey1D)
                {
                    const auto mapKey = std::make_pair(KeyCase, Key);
                    
                    CRawData1D6 Raw;
                    Raw.Init();

#if defined (_CIVIL)
					T_SBCF_D SbcfMax, SbcfMin;
					if (m_SbcfData.GetData(mapKey, SbcfMax, SbcfMin))
                    {
						CDgnAnalRsltUtil::Convert(true,  SbcfMax, Raw);
						CDgnAnalRsltUtil::Convert(false, SbcfMin, Raw);
                        vData.emplace_back(Raw);
                        continue;
                    }
#else
					T_STRB_D StrbMax, StrbMin;
					if (m_StrbData.GetData(mapKey, StrbMax, StrbMin))
                    {
						CDgnAnalRsltUtil::Convert(true,  StrbMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrbMin, Raw);
                        vData.emplace_back(Raw);
                        continue;
                    }
#endif
					T_STRT_D StrtMax, StrtMin;
					if (m_StrtData.GetData(mapKey, StrtMax, StrtMin))
					{
						CDgnAnalRsltUtil::Convert(true,  StrtMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrtMin, Raw);
						vData.emplace_back(Raw);
						continue;
					}

					T_STRW_D StrwMax, StrwMin;
					if (m_StrwData.GetData(mapKey, StrwMax, StrwMin))
					{
						CDgnAnalRsltUtil::Convert(true,  StrwMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrwMin, Raw);
						vData.emplace_back(Raw);
						continue;
					}
                    
					vData.emplace_back(Raw);
                }
                return true;
            }
            else
		*/
            {
                std::vector<PMembKey> vKey;
                if (!ConvertKey(enAnalSubType::Force1D6, vKey1D, vKey))
                {
                    ASSERT(0); return false;
                }

                return GetAnalRawData1D6F(ConvertCaseKey(KeyCase), vKey, vData);
            }
        }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
                                        const std::vector<DKey>& vKey1D, std::vector<CRawData1D9>& vData) const
{
    switch ( AnalType )
    {
    case enAnalType::Force:
        {
		/*
            if (IsDirectProduct())
            {
                for (const auto Key : vKey1D)
                {
                    const auto mapKey = std::make_pair(KeyCase, Key);

                    CRawData1D9 Raw;
                    Raw.Init();

#if defined (_CIVIL)
					T_SBCF_D SbcfMax, SbcfMin;
					if (m_SbcfData.GetData(mapKey, SbcfMax, SbcfMin))
                    {
						CDgnAnalRsltUtil::Convert(true,  SbcfMax, Raw);
						CDgnAnalRsltUtil::Convert(false, SbcfMin, Raw);
                        vData.emplace_back(Raw);
                        continue;
                    }
#else
					T_STRB_D StrbMax, StrbMin;
					if (m_StrbData.GetData(mapKey, StrbMax, StrbMin))
					{
						CDgnAnalRsltUtil::Convert(true,  StrbMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrbMin, Raw);
						vData.emplace_back(Raw);
						continue;
					}
#endif
					T_STRT_D StrtMax, StrtMin;
					if (m_StrtData.GetData(mapKey, StrtMax, StrtMin))
					{
						CDgnAnalRsltUtil::Convert(true,  StrtMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrtMin, Raw);
						vData.emplace_back(Raw);
						continue;
					}

					T_STRW_D StrwMax, StrwMin;
					if (m_StrwData.GetData(mapKey, StrwMax, StrwMin))
					{
						CDgnAnalRsltUtil::Convert(true,  StrwMax, Raw);
						CDgnAnalRsltUtil::Convert(false, StrwMin, Raw);
						vData.emplace_back(Raw);
						continue;
					}

                    vData.emplace_back(Raw);
                }
                return true;
            }
            else
		*/
            {
                std::vector<PMembKey> vKey;
                if (!ConvertKey(enAnalSubType::Force1D7, vKey1D, vKey))
                {
                    ASSERT(0); return false;
                }

                return GetAnalRawData1D9F(ConvertCaseKey(KeyCase), vKey, vData);
            }
        }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
    const std::vector<DKey>& vKey2D, std::vector<CRawData2DF>& vData) const
{
    switch (AnalType)
    {
    case enAnalType::Force:
    {
		/*
        if (IsDirectProduct())
        {
            for (const auto Key : vKey2D)
            {
                CRawData2DF Raw;
                Raw.Init();

				const auto mapKey = std::make_pair(KeyCase, Key);
				T_STRP_DL StrpMax, StrpMin;
                if (m_StrpData.GetData(mapKey, StrpMax, StrpMin))
                {
					CDgnAnalRsltUtil::Convert(true,  StrpMax, Raw);
					CDgnAnalRsltUtil::Convert(false, StrpMin, Raw);
                }
                vData.emplace_back(Raw);
            }
            return true;
        }
        else
		*/
        {
            std::vector<PMembKey> vKey;
            if (!ConvertKey(enAnalSubType::Force2D, vKey2D, vKey))
            {
                ASSERT(0); return false;
            }

            return GetAnalRawData2DF(ConvertCaseKey(KeyCase), vKey, vData);
        }
    }
    default:
        ASSERT(0); return false;
    }
}

bool CDgnAnalRsltConverter::GetAnalData(const dgn::def::DKey& KeyMCas, const enAnalType AnalType, const DKey& KeyCase,
    const std::vector<DKey>& vKey2D, std::vector<CRawData2DS>& vData) const
{
    switch (AnalType)
    {
    case enAnalType::Stress:
    {
		/*
        if (IsDirectProduct())
        {
            for (const auto Key : vKey2D)
            {
                CRawData2DS Raw;
                Raw.Init();

				const auto mapKey = std::make_pair(KeyCase, Key);
				T_STRP_DL StrpMax, StrpMin;
                if (m_StrpData.GetData(mapKey, StrpMax, StrpMin))
                {
					CDgnAnalRsltUtil::Convert(true,  StrpMax, Raw);
					CDgnAnalRsltUtil::Convert(false, StrpMin, Raw);
                }
                vData.emplace_back(Raw);
            }
            return true;
        }
        else
		*/
        {
            std::vector<PMembKey> vKey;
            if (!ConvertKey(enAnalSubType::Stress2D, vKey2D, vKey))
            {
                ASSERT(0); return false;
            }

            return GetAnalRawData2DS(ConvertCaseKey(KeyCase), vKey, vData);
        }
    }
    default:
        ASSERT(0); return false;
    }
}

const CDgnAnalRsltMembConverter* CDgnAnalRsltConverter::GetProductMembConverter() const
{
    return dynamic_cast<const CDgnAnalRsltMembConverter*>(__super::GetMembConverter());
}

const CDgnAnalRsltLoadConverter* CDgnAnalRsltConverter::GetProductLoadConverter() const
{
    return dynamic_cast<const CDgnAnalRsltLoadConverter*>(__super::GetLoadConverter());
}

IAnalRsltMembConverter* CDgnAnalRsltConverter::CreateMembConverter() const
{
    return new CDgnAnalRsltMembConverter(GetDBDoc(), (T_KEY_LIST*)&m_aElemList);
}

IAnalRsltLoadConverter* CDgnAnalRsltConverter::CreateLoadConverter() const
{
    return new CDgnAnalRsltLoadConverter(GetDBDoc());
}

CDgnAnalRsltConverter::PCaseKey CDgnAnalRsltConverter::ConvertCaseKey(const DKey& KeyCase) const
{
    return GetProductLoadConverter()->GetPCaseKey(KeyCase);
}

bool CDgnAnalRsltConverter::ConvertKey(const enAnalSubType& AnalSubType,
                                         const std::vector<DKey>& vKey1D, std::vector<PMembKey>& vKey) const
{
    vKey.clear();

    if ( !GetProductMembConverter()->GetPKeyList(AnalSubType, vKey1D, vKey) )
    {
        ASSERT(0); return false;
    }

    return true;
}

bool CDgnAnalRsltConverter::SetPostCtrlLoadCase(const PCaseKey KeyCase) const
{
    auto* pPostCtrl = GetPostCtrl();

    const auto CaseType = CDgnAnalRsltUtil::ConvertLoadCaseEnum(KeyCase.GetType());

    T_LCOM_D Case;
    if ( !pPostCtrl->MakeLoadComb(CaseType, KeyCase.GetKey(), Case) )
    {
		/*ASSERT(0); return false;*/ return false;
    }

    pPostCtrl->SelectLoadCombNew(Case);

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawDataNode(const PCaseKey KeyCase,
                                               const std::vector<PMembKey>& vKey, std::vector<CRawDataNode>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if (!SetPostCtrlLoadCase(KeyCase))
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawDataNode();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase](const PMembKey& Key)
    {
        CRawDataNode Raw;
        
        T_DISP_D DispMaxD, DispMinD;
        BOOL bExist = pPostCtrl->GetDispNew(Key.GetKey(), &DispMaxD, &DispMinD, nullptr);

        CDgnAnalRsltUtil::Convert(DispMaxD, DispMinD, Raw);

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawDataReact(const PCaseKey KeyCase,
                                                const std::vector<PMembKey>& vKey, std::vector<CRawDataReact>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if (!SetPostCtrlLoadCase(KeyCase))
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawDataReact();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase](const PMembKey& Key)
    {
        CRawDataReact Raw;

        T_REAC_D ReactData;
        ReactData.Initialize();
        BOOL bExist = pPostCtrl->GetReacNew(Key.GetKey(), &ReactData, nullptr, nullptr);

        CDgnAnalRsltUtil::Convert(ReactData, Raw);

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawDataDisp1D(const PCaseKey KeyCase,
    const std::vector<PMembKey>& vKey, std::vector<CRawData1D3>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if ( !SetPostCtrlLoadCase(KeyCase) )
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawData1D3();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase] (const PMembKey& Key)
    {
        CRawData1D3 Raw;

        T_DISP_D DispDataMax, DispDataMin;
        DispDataMax.Initialize();
        DispDataMin.Initialize();
        //TODO BS
        //BOOL bExist = pPostCtrl->GetLocalDispNew(Key.GetKey(), &DispDataMax, &DispDataMin, nullptr);

        //CDgnAnalRsltUtil::Convert(DispDataMax, DispDataMin, Raw);

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawData1D6F(const PCaseKey KeyCase,
                                               const std::vector<PMembKey>& vKey, std::vector<CRawData1D6>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if ( !SetPostCtrlLoadCase(KeyCase) )
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawData1D6();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase] (const PMembKey& Key)
    {
        CRawData1D6 Raw;
        
        T_ELEM_D ElemD;
        ElemD.Initialize();
        if (!pAttrCtrl->GetElem(Key.GetKey(), ElemD)) 
        { 
            ASSERT(0); 
            return Raw;
        }

        if (pAttrCtrl->IsBeam(ElemD.eltyp))
        {
#if defined (_CIVIL)
			T_SBCF_D SbcfMax, SbcfMin, SbcfAbs;
			if (!pPostCtrl->GetSbcfNew(Key.GetKey(), &SbcfMax, &SbcfMin, &SbcfAbs, TRUE))
			{
				return Raw;
			}

			CDgnAnalRsltUtil::Convert(true, SbcfMax, Raw);
			CDgnAnalRsltUtil::Convert(false, SbcfMin, Raw);
#else
            T_STRB_D StrbMax, StrbMin, StrbAbs;
            if (!pPostCtrl->GetStrbNew(Key.GetKey(), &StrbMax, &StrbMin, &StrbAbs, TRUE))
            {  
                return Raw;
            }

            CDgnAnalRsltUtil::Convert(true, StrbMax, Raw);
            CDgnAnalRsltUtil::Convert(false, StrbMin, Raw);
#endif
        }
        else if (pAttrCtrl->IsTruss(ElemD.eltyp))
        {
            T_STRT_D StrtMax, StrtMin, StrtAbs;
            if (!pPostCtrl->GetStrtNew(Key.GetKey(), &StrtMax, &StrtMin, &StrtAbs))
            {
                return Raw;
            }

            CDgnAnalRsltUtil::Convert(true, StrtMax, Raw);
            CDgnAnalRsltUtil::Convert(false, StrtMin, Raw);
        }
        else if (pAttrCtrl->IsWall(ElemD.eltyp))
        {
            UINT unInternalWallID = Key.GetKey();
            if (!pPostCtrl->GetInternalWallId(unInternalWallID, unInternalWallID))
            { 
                ASSERT(FALSE); return Raw;
            }

            T_STRW_D StrwMax, StrwMin, StrwAbs;
            if (!pPostCtrl->GetStrwNew(unInternalWallID, &StrwMax, &StrwMin, &StrwAbs))
            {
                ASSERT(0); return Raw;
            }

            CDgnAnalRsltUtil::Convert(true, StrwMax, Raw);
            CDgnAnalRsltUtil::Convert(false, StrwMin, Raw);
        }
        else ASSERT(0);

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawData1D9F(const PCaseKey KeyCase,
                                               const std::vector<PMembKey>& vKey, std::vector<CRawData1D9>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if ( !SetPostCtrlLoadCase(KeyCase) )
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawData1D9();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase](const PMembKey& Key)
    {
        CRawData1D9 Raw;

        T_ELEM_D ElemD;
        ElemD.Initialize();
        if (!pAttrCtrl->GetElem(Key.GetKey(), ElemD))
        {
            ASSERT(0);
            return Raw;
        }

        if (pAttrCtrl->IsBeam(ElemD.eltyp))
        {
#if defined (_CIVIL)
			T_SBCF_D SbcfMax, SbcfMin, SbcfAbs;
			if (!pPostCtrl->GetSbcfNew(Key.GetKey(), &SbcfMax, &SbcfMin, &SbcfAbs, TRUE))
			{
				return Raw;
			}

			CDgnAnalRsltUtil::Convert(true, SbcfMax, Raw);
			CDgnAnalRsltUtil::Convert(false, SbcfMin, Raw);
#else
            T_STRB_D StrbMax, StrbMin, StrbAbs;
            if (!pPostCtrl->GetStrbNew(Key.GetKey(), &StrbMax, &StrbMin, &StrbAbs, TRUE))
            {
                ASSERT(0); return Raw;
            }

            CDgnAnalRsltUtil::Convert(true, StrbMax, Raw);
            CDgnAnalRsltUtil::Convert(false, StrbMin, Raw);
#endif
        }
        else if (pAttrCtrl->IsWall(ElemD.eltyp))
        {
            UINT unInternalWallID = Key.GetKey();
            if (!pPostCtrl->GetInternalWallId(unInternalWallID, unInternalWallID))
            {
                ASSERT(FALSE); return Raw;
            }

            T_STRW_D StrwMax, StrwMin, StrwAbs;
            if (!pPostCtrl->GetStrwNew(unInternalWallID, &StrwMax, &StrwMin, &StrwAbs))
            {
                ASSERT(0); return Raw;
            }

            CDgnAnalRsltUtil::Convert(true, StrwMax, Raw);
            CDgnAnalRsltUtil::Convert(false, StrwMin, Raw);
        }
        else ASSERT(0);

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawData2DF(const PCaseKey KeyCase,
                                              const std::vector<PMembKey>& vKey, std::vector<CRawData2DF>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if (!SetPostCtrlLoadCase(KeyCase))
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawData2DF();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase](const PMembKey& Key)
    {
        CRawData2DF Raw;
#if defined (_CIVIL)
        T_SPCF_DL SpcfMax, SpcfMin;
        if ( !pPostCtrl->GetSpcfNew_L(Key.GetKey(), &SpcfMax, &SpcfMin, nullptr) )
        {
            ASSERT(0); return Raw;
        }

        CDgnAnalRsltUtil::Convert(true, SpcfMax, Raw);
        CDgnAnalRsltUtil::Convert(false, SpcfMin, Raw);
#else
        T_STRP_DL StrpMax, StrpMin, StrpAbs;
        if (!pPostCtrl->GetStrpNew_L(Key.GetKey(), &StrpMax, &StrpMin, &StrpAbs))
        {
            ASSERT(0); return Raw;
        }

        CDgnAnalRsltUtil::Convert(true, StrpMax, Raw);
        CDgnAnalRsltUtil::Convert(false, StrpMin, Raw);
#endif

        return Raw;
    });

    return true;
}

bool CDgnAnalRsltConverter::GetAnalRawData2DS(const PCaseKey KeyCase,
                                              const std::vector<PMembKey>& vKey, std::vector<CRawData2DS>& vData) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	//중립단위계
	CCurUnitSaver Save(TRUE);
	if(!SetNeutralUnit())
	{
		Save.Reset();
	}

    if (!SetPostCtrlLoadCase(KeyCase))
    {
        Macro::Convert(vKey, vData, [] (const PMembKey& Key)
        {
            return CRawData2DS();
        });
        return true;
    }

    auto* pPostCtrl = GetPostCtrl();
    auto* pAttrCtrl = GetAttrCtrl();

    Macro::Convert(vKey, vData, [pPostCtrl, pAttrCtrl, KeyCase](const PMembKey& Key)
    {
        CRawData2DS Raw;

        T_STRP_DL StrpMax, StrpMin, StrpAbs;
        if (!pPostCtrl->GetStrpNew_L(Key.GetKey(), &StrpMax, &StrpMin, &StrpAbs))
        {
            ASSERT(0); return Raw;
        }

        CDgnAnalRsltUtil::Convert(true, StrpMax, Raw);
        CDgnAnalRsltUtil::Convert(false, StrpMin, Raw);

        return Raw;
    });

    return true;
}

CDBDoc* CDgnAnalRsltConverter::GetDBDoc() const
{
    if ( !m_pDBDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDBDoc;
}

CPostCtrl* CDgnAnalRsltConverter::GetPostCtrl() const
{
    return GetDBDoc()->m_pPostCtrl;
}

CAttrCtrl* CDgnAnalRsltConverter::GetAttrCtrl() const
{
    return GetDBDoc()->m_pAttrCtrl;
}