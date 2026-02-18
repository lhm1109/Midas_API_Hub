#pragma once

#include <map>

#include "HeaderPre.h"

class CDBDoc;
class __MY_EXT_CLASS__ CMembTypeUtil
{
public:
    CMembTypeUtil();
    virtual ~CMembTypeUtil();

public:
    bool IsSTL(const T_ELEM_K& ElemK);
    bool IsRC(const T_ELEM_K& ElemK);
    bool IsSRC(const T_ELEM_K& ElemK);

public:
    bool IsBeam(const T_ELEM_K& ElemK);
    bool IsColumn(const T_ELEM_K& ElemK);
    bool IsBrace(const T_ELEM_K& ElemK);
	bool IsWall(const T_ELEM_K& ElemK);
    bool IsPlate(const T_ELEM_K& ElemK);
    bool IsSlab(const T_ELEM_K& ElemK);

	bool IsEffectiveBeam(const T_ELEM_K& ElemK);
	bool IsColumnStrip(const T_ELEM_K& ElemK);

public:
    bool IsSTLBeam(const T_ELEM_K& ElemK);
    bool IsSTLColumn(const T_ELEM_K& ElemK);
    bool IsSTLBrace(const T_ELEM_K& ElemK);

public:
    bool IsRCBeam(const T_ELEM_K& ElemK);
    bool IsHaunchedBeam(const T_ELEM_K& ElemK);
    bool IsRCColumn(const T_ELEM_K& ElemK);
    bool IsRCBrace(const T_ELEM_K& ElemK);
    bool IsRCWall(const T_ELEM_K& ElemK);
    bool IsRCPlate(const T_ELEM_K& ElemK);
    bool IsRCPlateBeam(const T_ELEM_K& ElemK);
    bool IsRCPlateColumn(const T_ELEM_K& ElemK);
    bool IsRCShell(const T_ELEM_K& ElemK);

    //아래것들도 다 이동시켜야함.
public:
    int  GetMemberType(const T_ELEM_K& ElemK);
    int  GetHaunckedMembKey(const T_ELEM_K& ElemK, std::vector<T_MEMB_K>& vMemb);

private:
    CDBDoc* m_pDoc;
};

#include "HeaderPost.h"
