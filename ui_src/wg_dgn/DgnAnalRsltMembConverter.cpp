#include "StdAfx.h"

#include <array>
#include <vector>
#include <map>

#include "../../dgnengine/idesign/DGN_def/DataTypeDef.h"
#include "../../dgnengine/idesign/DGN_lib/DLine.h"
#include "../../dgnengine/idesign/DGN_def/Position1D.h"
#include "../../dgnengine/src/DGN_DB/DPoolPosition1D.h"
#include "../../dgnengine/idesign/DGN_link/IDgnDoc.h"
#include "../../dgnengine/idesign/DGN_link/IDgnDocCtrl.h"
#include "../../dgnengine/idesign/DGN_link/IDgnPerform.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/AttrCtrl2.h"

#include "DgnAnalRsltUtil.h"
#include "IDgnPerformDataBase.h"
#include "DgnAnalRsltMembConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;
using namespace dgn::def;
using namespace dgn::link;

CDgnAnalRsltMembConverter::CDgnAnalRsltMembConverter(CDBDoc* pDBDoc, T_KEY_LIST* paKeyElem) :
    TAnalRsltMembConverter<MembKey>(),
    m_pDBDoc(pDBDoc), m_paKeyElem(paKeyElem)
{
}

CDgnAnalRsltMembConverter::~CDgnAnalRsltMembConverter()
{
}

int CDgnAnalRsltMembConverter::DoGetKeyList(const enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const
{
    vKey.clear();

    const auto ElemType = GetElemTypeByAnalSubType(AnalSubType);
    switch ( ElemType )
    {
    case enElemType::Elem1D: return GetKeyList1D(AnalSubType, vKey);
    case enElemType::Elem2D: return GetKeyList2D(AnalSubType, vKey);
    case enElemType::Node:   return GetKeyListNode(AnalSubType, vKey);
    default: ASSERT(0); return 0;
    }
}

int CDgnAnalRsltMembConverter::DoGetElemPosition1D(const MembKey& KeyMember,
                                                   const enAnalSubType& AnalSubType,
                                                   std::vector<TElemPos1DMemb<MembKey>>& vElemPos) const
{
    vElemPos.clear();

    switch ( KeyMember.GetType() )
    {
    case enMembKeyType::enMembKeyElem:
        vElemPos.push_back(TElemPos1DMemb<MembKey>(KeyMember, 0.0, 1.0));
        return Macro::GetSize(vElemPos);
    case enMembKeyType::enMembKeyMemb:
        {
            auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

            T_KEY_LIST aElem;
            T_HCBM_K HcbmK(0);
            if (pAttrCtrl->GetHcbmAssigned(KeyMember.GetKey(), HcbmK))
            {
                T_HCBM_D HcbmD;
                if (pAttrCtrl->GetHcbm(HcbmK, HcbmD))
                {
                    for (int iPart = 0; iPart < 3; ++iPart)
                    {
                        if (HcbmD.aElemListPart[iPart].GetAt(0) == KeyMember.GetKey())
                        {
                            aElem.Append(HcbmD.aElemListPart[iPart]);
                            break;
                        }
                    }
                }
            }
            else
            {
                T_MEMB_K MembKey;
                if (pAttrCtrl->GetMembAssigned(KeyMember.GetKey(), MembKey))	// Exist Member.
                {
                    pAttrCtrl->GetElemKByMemb(MembKey, aElem);
                }
                else
                {
                    aElem.Add(KeyMember.GetKey());
                }
            }

            if ( aElem.IsEmpty() )
            {
                ASSERT(0); return 0;
            }

            std::vector<double> vLength;
            ConvertC2V_F(aElem, vLength, [&pAttrCtrl] (const T_KEY& Key)
            {
                T_ELEM_D Elem;
                if ( !pAttrCtrl->GetElem(Key, Elem) )
                {
                    ASSERT(0); return 0.0;
                }

                T_NODE_D NodeI;
                if ( !pAttrCtrl->GetNode(Elem.elnod[0], NodeI) )
                {
                    ASSERT(0); return 0.0;
                }

                T_NODE_D NodeJ;
                if ( !pAttrCtrl->GetNode(Elem.elnod[1], NodeJ) )
                {
                    ASSERT(0); return 0.0;
                }

                DLine3d Line(DVector3d(NodeI.x, NodeI.y, NodeI.z),
                    DVector3d(NodeJ.x, NodeJ.y, NodeJ.z));

                return Line.Length();
            });

            // 추후 Matrix가 생기면, Element의 시작점과 끝점이 아닌,
            // 중간에서 Member를 시작하는 경우도 계산 가능함
            // Matrix가 들어오면 그 때 고칩시다.

            const double dLength = Macro::Fold(vLength, 0.0);

            std::vector<double> vRelativeLength;
            Macro::Convert(vLength, vRelativeLength, [&dLength] (const double& d)
            {
                return dgn::SafeDiv(d, dLength);
            });

            double dAccumulatedLength = 0.0;
            for ( int nIdx = 0; nIdx < Macro::GetSize(vRelativeLength); ++nIdx )
            {
                const auto ElemPos = TElemPos1DMemb<MembKey>(MembKey(enMembKeyType::enMembKeyElem, aElem[nIdx]),
                    dAccumulatedLength, dAccumulatedLength + vRelativeLength[nIdx]);
                dAccumulatedLength += ElemPos.GetLength();

                vElemPos.push_back(ElemPos);
            }

            return Macro::GetSize(vElemPos);
        }
    }

    return 0;
}

bool CDgnAnalRsltMembConverter::DoGetPosition1D(const MembKey& KeyMember, CPosition1D& Position1D) const
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    auto* pDocCtrl = dgn::link::IDgnDocCtrl::Instance();
    if ( pDocCtrl == nullptr ) return false;
    dgn::link::IDgnDoc* pDgnDoc = pDocCtrl->GetDgnDoc(pDoc);

    std::vector<DWORD_PTR> vIter;
    int nPos = pDgnDoc->GetDgnPerform()->GetDgnPosition1DItrList(vIter);
    const auto itrPos = [&vIter, &KeyMember]()
    {
        for (const auto itr : vIter)
        {
            const auto key = FITR_TO_KEY(itr);
            if (key == KeyMember.GetKey())
            {
                return itr;
            }
        }
        return (DWORD_PTR)0;
    }();

    if (itrPos == 0)
    {
        Position1D = CPosition1D(5); // Gen / Civil은 5개 Regular 고정
    }
    else
    {
        const auto* pData = DGN_ITR_TO_DATA(CDgn1DPosition, itrPos);
        int nChkRatio = pData->aChkRatio.GetSize();
        if (nChkRatio > 5)
        {
            std::vector<double> vdPoint;
            vdPoint.reserve(nChkRatio);
            for (int i = 0; i < nChkRatio; ++i)
            {
                vdPoint.push_back(pData->aChkRatio[i]);
            }
            Position1D = CPosition1D(vdPoint);
        }
        else
        {
            Position1D = CPosition1D(5);
        }
    }

    return true;
}

static const auto cnNotValidElemType = 0;
enElemType CDgnAnalRsltMembConverter::DoGetElemType(const MembKey& KeyMember) const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

    const auto ElemType = [this, &pAttrCtrl] (const MembKey KeyMember)
    {
        const auto ElemKey = GetElemKeyFront(KeyMember);
        T_ELEM_D Elem;
        if ( !pAttrCtrl->GetElem(KeyMember.GetKey(), Elem) )
        {
            ASSERT(0); return cnNotValidElemType;
        }

        if ( Elem.eltyp == WALL_EL )
        {
            return BEAM_EL;
        }

        return Elem.eltyp;
    } (KeyMember);

    if ( ElemType == cnNotValidElemType )
    {
        ASSERT(0); return enElemType::None;
    }

    if ( pAttrCtrl->IsFrameType(ElemType) )
    {
        return enElemType::Elem1D;
    }
    else if ( pAttrCtrl->IsPlaneType(ElemType) )
    {
        return enElemType::Elem2D;
    }
    else
    {
        ASSERT(0); return enElemType::None;
    }
}

bool CDgnAnalRsltMembConverter::DoIsValidAnalSubType(const MembKey& KeyMember, const enAnalSubType AnalSubType) const
{
    const auto ElemType = (KeyMember.GetType() == enMembKeyNode) ? enElemType::Node : DoGetElemType(KeyMember);

    switch ( ElemType )
    {
    case enElemType::Elem1D:
        {
            auto* pAttrCtrl2 = GetDBDoc()->m_pAttrCtrl2;
            const auto b7Dof = pAttrCtrl2->Is7thDOFBeam(GetElemKeyFront(KeyMember));

            switch ( AnalSubType )
            {
            case enAnalSubType::Disp1D:      return true;
            case enAnalSubType::Force1D6:    return !b7Dof;
            case enAnalSubType::Force1D7:    return b7Dof;
            default: ASSERT(0); return false;
            }
        }
    case enElemType::Elem2D:
    case enElemType::Node:
        return true;
    default:
        ASSERT(0); return false;
    }
}

int CDgnAnalRsltMembConverter::GetProductMemberKeyList(std::vector<MembKey>& vKeyMember) const
{
    vKeyMember.clear();

    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;
    CArray<T_ELEM_K, T_ELEM_K> aKeyElem;
    const BOOL bWithoutDummy = TRUE;
    pAttrCtrl->GetElemKeyList(aKeyElem, bWithoutDummy);

    std::vector<T_ELEM_K> vKeyElem;
    ConvertC2V(aKeyElem, vKeyElem);
    /*
    Macro::Filter(vKeyElem, [&pAttrCtrl] (const T_ELEM_K& Key)
    {
        unsigned int Dummy = 0;
        return !pAttrCtrl->GetMembAssigned(Key, Dummy);
    });
    */
    std::vector<MembKey> vKeyElemMember;
    Macro::Convert(vKeyElem, vKeyElemMember, [] (const T_ELEM_K& Key)
    {
        return MembKey(enMembKeyType::enMembKeyElem, Key);
    });

    T_KEY_LIST aMember;
    pAttrCtrl->GetMembKeyList(aMember);
    int nMember = aMember.GetSize();

    T_KEY_LIST aKeyMemb;
    for (int i = 0; i < nMember; ++i)
    {
        CArray<T_ELEM_K, T_ELEM_K> aElemKey;
        if(!pAttrCtrl->GetElemKByMemb(aMember[i], aElemKey)) continue;
        if (aElemKey.GetSize() == 0) continue;
        aKeyMemb.Add(aElemKey[0]);
    }
    
    T_KEY_LIST aKeyHaunch;
    pAttrCtrl->GetHcbmKeyList(aKeyHaunch);
    int nHcbm = aKeyHaunch.GetSize();
    for (int i = 0; i < nHcbm; ++i)
    {
        T_HCBM_D HcbmD;
        if(!pAttrCtrl->GetHcbm(aKeyHaunch[i], HcbmD)) continue;
        for (int iPart = 0; iPart < 3; ++iPart)
            aKeyMemb.Add(HcbmD.aElemListPart[iPart].GetAt(0));
    }

    std::vector<MembKey> vKeyMembMember;
    ConvertC2V_F(aKeyMemb, vKeyMembMember, [] (const T_KEY& Key)
    {
        return MembKey(enMembKeyType::enMembKeyMemb, Key);
    });

    Macro::Append(vKeyMember, vKeyElemMember);
    Macro::Append(vKeyMember, vKeyMembMember);

    return Macro::GetSize(vKeyMember);
}

int CDgnAnalRsltMembConverter::GetMemberKeyList(const enMembKeyType& KeyType, std::vector<MembKey>& vKey) const
{
    vKey.clear();

    if ( !GetProductMemberKeyList(vKey) )
    {
        ASSERT(0); return 0;
    }

    Macro::Filter(vKey, [&KeyType] (const MembKey& Key)
    {
        return Key.GetType() == KeyType;
    });

    return Macro::GetSize(vKey);
}

int CDgnAnalRsltMembConverter::GetProductNodeKeyList(std::vector<MembKey>& vKeyNode) const
{
    vKeyNode.clear();

    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;
    CArray<T_NODE_K, T_NODE_K> aNodeK;
    pAttrCtrl->GetNodeKeyList(aNodeK);

    std::vector<T_NODE_K> vNodeK;
    ConvertC2V(aNodeK, vNodeK);
    
    Macro::Convert(vNodeK, vKeyNode, [](const T_NODE_K& Key)
    {
        return MembKey(enMembKeyType::enMembKeyNode, Key);
    });

    return Macro::GetSize(vKeyNode);
}

int CDgnAnalRsltMembConverter::GetNodeKeyList(const enMembKeyType& KeyType, std::vector<MembKey>& vKey) const
{
    vKey.clear();

    if (!GetProductNodeKeyList(vKey))
    {
        ASSERT(0); return 0;
    }

    Macro::Filter(vKey, [&KeyType](const MembKey& Key)
    {
        return Key.GetType() == KeyType;
    });

    return Macro::GetSize(vKey);
}

bool CDgnAnalRsltMembConverter::GetPKeyList(const enAnalSubType& AnalSubType,
                                            const std::vector<DKey>& vKey, std::vector<MembKey>& vPKey) const
{
    Macro::Convert(vKey, vPKey, [this, &AnalSubType] (const DKey& Key)
    {
        return GetProductKey(AnalSubType, Key);
    });

    return true;
}

T_KEY CDgnAnalRsltMembConverter::GetElemKeyFront(const MembKey& KeyMember) const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

    switch ( KeyMember.GetType() )
    {
    case enMembKeyElem:
        return KeyMember.GetKey();
    case enMembKeyMemb:
        {
            CArray<T_KEY, T_KEY> aKey;
            T_HCBM_K HcbmK(0);
            if (pAttrCtrl->GetHcbmAssigned(KeyMember.GetKey(), HcbmK))
            {
                T_HCBM_D HcbmD;
                if (pAttrCtrl->GetHcbm(HcbmK, HcbmD))
                {
                    for (int iPart = 0; iPart < 3; ++iPart)
                    {
                        if (HcbmD.aElemListPart[iPart].GetAt(0) == KeyMember.GetKey())
                        {
                            aKey.Append(HcbmD.aElemListPart[iPart]);
                            break;
                        }
                    }
                }
            }
            else
            {
                T_MEMB_K MembKey;
                if (pAttrCtrl->GetMembAssigned(KeyMember.GetKey(), MembKey))	// Exist Member.
                {
                    pAttrCtrl->GetElemKByMemb(MembKey, aKey);
                }
                aKey.Add(KeyMember.GetKey());
            }

            return aKey[0];
        }
    default:
        ASSERT(0); return 0;
    }
}

int CDgnAnalRsltMembConverter::GetKeyList1D(const enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const
{
    T_KEY_LIST* paElem = GetElemKeyList();
    
    ConvertC2V_F(*paElem, vKey, [] (const T_KEY& Key)
    {
        return MembKey(enMembKeyType::enMembKeyElem, Key);
    });

    Macro::Filter(vKey, [this] (const MembKey& Key)
    {
        return DoGetElemType(Key) == enElemType::Elem1D;
    });

    if ( vKey.empty() )
    {
        return 0;
    }

    auto* pAttrCtrl2 = GetDBDoc()->m_pAttrCtrl2;

    switch ( AnalSubType )
    {
    case enAnalSubType::Disp1D:
        return true;
    case enAnalSubType::Force1D6:
        return Macro::Filter(vKey, [&pAttrCtrl2] (const MembKey& Key)
        {
            return !pAttrCtrl2->Is7thDOFBeam(Key.GetKey());
        });
    case enAnalSubType::Force1D7:
        return Macro::Filter(vKey, [&pAttrCtrl2] (const MembKey& Key)
        {
            return pAttrCtrl2->Is7thDOFBeam(Key.GetKey());
        });
    default:
        ASSERT(0); return 0;
    }
}

int CDgnAnalRsltMembConverter::GetKeyList2D(const enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;
    T_KEY_LIST aElem;
    const BOOL bWithoutDummy = TRUE;
    pAttrCtrl->GetElemKeyList(aElem, bWithoutDummy);

    ConvertC2V_F(aElem, vKey, [] (const T_KEY& Key)
    {
        return MembKey(enMembKeyType::enMembKeyElem, Key);
    });

    Macro::Filter(vKey, [this] (const MembKey& Key)
    {
        return DoGetElemType(Key) == enElemType::Elem2D;
    });

    if ( vKey.empty() )
    {
        return 0;
    }

    return Macro::GetSize(vKey);
}

int CDgnAnalRsltMembConverter::GetKeyListNode(const enAnalSubType& AnalSubType, std::vector<MembKey>& vKey) const
{
    auto* pAttrCtrl = GetDBDoc()->m_pAttrCtrl;

    T_KEY_LIST aNode;
    switch (AnalSubType)
    {
    case enAnalSubType::NodalDeform:
        {
            pAttrCtrl->GetNodeKeyList(aNode);
            
        }
        break;
    case enAnalSubType::NodalReact:
        {
            pAttrCtrl->GetNodeKeyList(aNode); // 지점이 아니더라도 key는 모두 넘겨줘야함.
            //pAttrCtrl->GetConsNodeKeyList(aNode);
        }
        break;
    default:
        break;
    }
    
    ConvertC2V_F(aNode, vKey, [](const T_KEY& Key)
    {
        return MembKey(enMembKeyType::enMembKeyNode, Key);
    });

    if (vKey.empty())
    {
        return 0;
    }

    return Macro::GetSize(vKey);
}

T_KEY_LIST* CDgnAnalRsltMembConverter::GetElemKeyList() const
{
    return m_paKeyElem;
}

CDBDoc* CDgnAnalRsltMembConverter::GetDBDoc() const
{
    if ( !m_pDBDoc )
    {
        ASSERT(0); return nullptr;
    }

    return m_pDBDoc;
}