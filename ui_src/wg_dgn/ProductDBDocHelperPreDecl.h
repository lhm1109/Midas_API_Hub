#pragma once

#include "../dgnengine/idesign/DGN_def/Rebar2D.h"

class CDBDoc;
class CPostCtrl;
class CAttrCtrl;
class CAttrCtrl2;
class CMembCtrl;
class CUnitCtrl;
class CMatlDB;
class IDgnPerformDataBase;
class CDesignResult;
class CDgnDataCtrl;

namespace idgn
{
    typedef	DWORD_PTR FIterator;
}

namespace dgn
{
    namespace def
    {
        enum class enUnitType;
        enum class enSector;
        enum class enMemb;
        enum class enDgn;
        enum class DataType : unsigned int;

        class  CRebarBeam;
        class  CDgnRevision;
        class  CResultUnitOut;
        struct TResultVItem;
        struct TResultVMemb;
    }

    namespace item
    {
        enum class enDgnResultItem : unsigned int;
    }

    namespace link
    {
        class IDgnDoc;
        class Pepe;
    }

    namespace converter
    {
        enum class enProductDgnMode;

        struct TEngineResult;
    }
}

struct tRebar2D
{
    typedef Macro::TNumber<dgn::def::enRebar2D> Rebar2DNumb;

    std::array<T_RBMS_D, Rebar2DNumb::Number> aRebar;

    void Initialize()
    {
        for (int i = 0; i < Rebar2DNumb::Number; ++i)
        {
            aRebar[i].Initialize();
        }
    }
};