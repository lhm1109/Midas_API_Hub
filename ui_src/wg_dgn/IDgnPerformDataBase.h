#pragma once

#include "../dgnengine/idesign/DGN_def/DgnModeDef.h"

#include "HeaderPre.h"

namespace idgn
{
    typedef	DWORD_PTR FIterator;
}

namespace dgn 
{ 
    namespace def 
    { 
        struct TCriticalInfo; 
        class CMembType;
        class CDgnRptCase;
        class CDgnRptCriticalInfo;
        enum class enDgnCode : unsigned int;
        enum class enDgn;
        enum class enMemb;
    }

    namespace item
    {
        enum class enDgnResultItem : unsigned int;
    }

    namespace converter
    {
        class IProductSaveObjectBuilder;
    }
}

typedef UINT T_KEY;
typedef CArray<T_KEY, T_KEY> T_KEY_LIST;
struct _DGN_LCOM;
class CDgnAnalRsltCtrl;
class __MY_EXT_CLASS__ IDgnPerformDataBase
{
public:
    enum class enResultType
    {
        None,
        DGN,
        CHK,
    };

    typedef dgn::converter::IProductSaveObjectBuilder IProductSaveObjectBuilder;

public:
    IDgnPerformDataBase();
    virtual ~IDgnPerformDataBase();

public:
	virtual BOOL IsStl() { return FALSE; }
	virtual BOOL IsRcsBeam() { return FALSE; }
	virtual BOOL IsRcsColm() { return FALSE; }
	virtual BOOL IsRcsBrce() { return FALSE; }
	virtual BOOL IsRcsWall() { return FALSE; }
	virtual BOOL IsRcsPsbm() { return FALSE; }
	virtual BOOL IsRcsHcbm() { return FALSE; }
	virtual BOOL IsSrcBeam() { return FALSE; }
	virtual BOOL IsRcsSlab() { return FALSE; }
	virtual BOOL IsRcsMeshWall() { return FALSE; }
	virtual BOOL IsRcsPBeam() { return FALSE; }
	virtual BOOL IsRcsPWall() { return FALSE; }
	virtual BOOL IsRcsShell() { return FALSE; }

public:
    virtual dgn::def::enDgn GetDgnType() = 0;
    //아놔 토목쪽 C_DATABASE들 때문에 순수가상하기가 그러네.
    virtual bool IsRunMember(T_KEY MembK) { ASSERT(0); return false; }
    virtual dgn::def::enDgnMode GetDgnMode() { ASSERT(0); return dgn::def::enDgnMode::Checking; }
    virtual enResultType GetResultType(T_KEY MembK) { ASSERT(0); return enResultType::None; }

    virtual int GetCodeUnitForce() { ASSERT(0); return 2; }
    virtual int GetCodeUnitLength() { ASSERT(0); return 0; }
    virtual int GetLcomType() { ASSERT(0); return 0; }
    virtual int GetCantileverType(T_ELEM_K ElemKey) { ASSERT(0); return 0; }

public:
    virtual int GetForceElemList(const T_KEY_LIST& aRunElemList, T_KEY_LIST &raElemList) { ASSERT(0); return 0; }
	virtual CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&>* GetLcomList() = 0;

public:
    //IDgnDatabase를 상속받는놈은 true를 performdatabase만 상속받는 놈은 false를 준다.
    virtual bool IsIdgnViewReport() = 0;
    virtual bool IsSupportedSectionDataBase() = 0;
    virtual bool IsDesigning() = 0;
    virtual void CheckDgnFlag(const T_KEY_LIST& aElem) { }

public:
    //여기서 부터는 Perform 전용이라는데?
    virtual bool GetCriticalInfo(const long& MembKey, const std::vector<dgn::item::enDgnResultItem>& aItem, dgn::def::CDgnRptCriticalInfo* pCritInfo) = 0;

public:
    virtual void WriteElemPerformResult(T_KEY MembK) { ASSERT(0);  }
    virtual void WriteSectResult(bool bRunSuccess) { ASSERT(0); }
    virtual void Count_UseElemPropNum(int TotalElemNum) { ASSERT(0); }

	virtual CDgnAnalRsltCtrl* GetAnalRsltCtrl() const { ASSERT(0); return nullptr; }
};

#include "HeaderPost.h"
