#include "StdAfx.h"

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_attrctrl.h"

#include "DgnAnalRsltCtrl.h"
#include "DgnAnalRsltConverter.h"
#include "DgnAnalRsltMembConverter.h"

#include "DgnAnalRsltTester.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::def;

CDgnAnalRsltTester::CDgnAnalRsltTester() :
    m_pDBDoc(CDBDoc::GetDocPoint())
{
}

CDgnAnalRsltTester::~CDgnAnalRsltTester()
{
}

bool CDgnAnalRsltTester::Test()
{
    auto* pAttrCtrl = m_pDBDoc->m_pAttrCtrl;

    const BOOL bWithoutDummy = TRUE;
    T_KEY_LIST aKeyList;
    pAttrCtrl->GetElemKeyList(aKeyList, bWithoutDummy);

    auto pConverter = std::shared_ptr<CDgnAnalRsltConverter>(new CDgnAnalRsltConverter(GetDBDoc(), aKeyList));

    CDgnAnalRsltCtrl Ctrl(pConverter.get(), GetDBDoc());

    if ( !Ctrl.IsValid() )
    {
        ASSERT(0); return false;
    }

    if ( !Ctrl.Initialize() )
    {
        ASSERT(0); return false;
    }

    auto* pMembConverter = pConverter->GetProductMembConverter();

    const std::array<enMembKeyType, 2> caKeyType =
    {
        enMembKeyType::enMembKeyElem,
        enMembKeyType::enMembKeyMemb,
    };

    for ( const auto& KeyType : caKeyType )
    {
        std::vector<CDgnAnalRsltMembConverter::MembKey> vKey;
        if ( !pMembConverter->GetMemberKeyList(KeyType, vKey) )
        {
            continue;
        }

        for ( const auto& Key : vKey )
        {            
            T_ELEM_D ElemD;
            if(!m_pDBDoc->m_pAttrCtrl->GetElem(Key.GetKey(), ElemD)) continue;
            if (m_pDBDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) || 
               (m_pDBDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) && KeyType == enMembKeyType::enMembKeyElem))
            {
                const auto* pForce1D = Ctrl.CreateMember1DForceDOF6ByProduct(Key);

                if (!pForce1D)
                {
                    continue;
                }

                std::vector<DKey> vKeyMCas;
                if (!pForce1D->GetDCasList(vKeyMCas))
                {
                    ASSERT(0); return false;
                }

                std::vector<DKey> vKeyCase;
                if (!pForce1D->GetCaseList(vKeyCase))
                {
                    ASSERT(0); return false;
                }

                std::vector<DKey> vKeyLcom;
                if (!pForce1D->GetLcomList(vKeyLcom))
                {
                    ASSERT(0); return false;
                }

                const auto Position = pForce1D->GetPosition1D();
                const CPosition1D PositionArbitrary(37); // 소수가 좋자네

                for (const auto& KeyMCas : vKeyMCas)
                {
                    for (const auto& KeyCase : vKeyCase)
                    {
                        for (const auto Pos : Position)
                        {
                            TForce6Dofs Force;
                            if (!pForce1D->GetCaseData(KeyMCas, KeyCase, Pos, Force))
                            {
                                ASSERT(0); return false;
                            }
                        }

                        for (const auto& Pos : PositionArbitrary)
                        {
                            TForce6Dofs Force;
                            if (!pForce1D->GetCasePosData(KeyMCas, KeyCase, Pos, Force))
                            {
                                ASSERT(0); return false;
                            }
                        }
                    }

                    for (const auto& LcomK : vKeyLcom)
                    {
                        enLcomKeyType enDgnType;
                        DKey KeyLcom;
                        if(!pForce1D->GetLcomTyneKey(LcomK, enDgnType, KeyLcom)) continue;

                        for (const auto Pos : Position)
                        {
                            TForce6Dofs Force;
                            if (!pForce1D->GetLcomData(KeyMCas, enDgnType, KeyLcom, Pos, Force))
                            {
                                ASSERT(0); return false;
                            }
                        }

                        for (const auto& Pos : PositionArbitrary)
                        {
                            TForce6Dofs Force;
                            if (!pForce1D->GetLcomPosData(KeyMCas, enDgnType, KeyLcom, Pos, Force))
                            {
                                ASSERT(0); return false;
                            }
                        }
                    }
                }
                SafeDelete(pForce1D);
            }
            else if (m_pDBDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
            {
                if (m_pDBDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))  continue;
                if (KeyType == enMembKeyType::enMembKeyMemb) continue;

                std::vector<CDgnAnalRsltMembConverter::MembKey> vElemKey;
                vElemKey.push_back(Key);

                // 2D Force
                const auto* pForce2D = Ctrl.CreateMember2DForceByProduct(vElemKey);

                std::vector<DKey> vKeyMCas;
                if (!pForce2D->GetDCasList(vKeyMCas))
                {
                    ASSERT(0); return false;
                }

                std::vector<DKey> vKeyCase;
                if (!pForce2D->GetCaseList(vKeyCase))
                {
                    ASSERT(0); return false;
                }

                std::vector<DKey> vKeyLcom;
                if (!pForce2D->GetLcomList(vKeyLcom))
                {
                    ASSERT(0); return false;
                }

                auto EngineElemKey = pMembConverter->GetEngineKey(enAnalSubType::Force2D, Key);
                for (const auto& KeyMCas : vKeyMCas)
                {
                    for (const auto& KeyCase : vKeyCase)
                    {
                        TForce2D Force;
                        if (!pForce2D->GetCaseData(KeyMCas, KeyCase, EngineElemKey, Force))
                        {
                            ASSERT(0); return false;
                        }
                    }

                    for (const auto& LcomK : vKeyLcom)
                    {
                        enLcomKeyType enDgnType;
                        DKey KeyLcom;
                        if (!pForce2D->GetLcomTyneKey(LcomK, enDgnType, KeyLcom)) continue;

                        TForce2D Force;
                        if (!pForce2D->GetLcomData(KeyMCas, enDgnType, KeyLcom, EngineElemKey, Force))
                        {
                            ASSERT(0); return false;
                        }
                    }
                }


                // 2D Stress
                const auto* pStress2D = Ctrl.CreateMember2DStressByProduct(vElemKey);

                vKeyMCas.clear();
                if (!pStress2D->GetDCasList(vKeyMCas))
                {
                    ASSERT(0); return false;
                }

                vKeyCase.clear();
                if (!pStress2D->GetCaseList(vKeyCase))
                {
                    ASSERT(0); return false;
                }

                vKeyLcom.clear();
                if (!pStress2D->GetLcomList(vKeyLcom))
                {
                    ASSERT(0); return false;
                }

                EngineElemKey = pMembConverter->GetEngineKey(enAnalSubType::Stress2D, Key);
                for (const auto& KeyMCas : vKeyMCas)
                {
                    for (const auto& KeyCase : vKeyCase)
                    {
                        TStress2D Stress;
                        if (!pStress2D->GetCaseData(KeyMCas, KeyCase, EngineElemKey, Stress))
                        {
                            ASSERT(0); return false;
                        }
                    }

                    for (const auto& LcomK : vKeyLcom)
                    {
                        enLcomKeyType enDgnType;
                        DKey KeyLcom;
                        if (!pForce2D->GetLcomTyneKey(LcomK, enDgnType, KeyLcom)) continue;

                        TStress2D Stress;
                        if (!pStress2D->GetLcomData(KeyMCas, enDgnType, KeyLcom, EngineElemKey, Stress))
                        {
                            ASSERT(0); return false;
                        }

                    }
                }
                

            }
        }
    }

    if ( !Ctrl.Finalize() )
    {
        ASSERT(0); return false;
    }

    pConverter->Finalize();

    return true;
}

CDBDoc* CDgnAnalRsltTester::GetDBDoc() const
{
    if ( !m_pDBDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDBDoc;
}