#pragma once

#include "../../dgnengine/idesign/DGN_def/AnalRsltEnumDef.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltRawData1DElem.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltRawData2DElem.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltRawDataNode.h"
#include "../../dgnengine/idesign/DGN_def/TAnalRsltKey.h"

#include "TDgnAnalRsltKey.h"

#include "HeaderPre.h"

class CDBDoc;

class __MY_EXT_CLASS__ CDgnAnalRsltUtil final
{
public:
    CDgnAnalRsltUtil(CDBDoc* pDBDoc);
    virtual ~CDgnAnalRsltUtil();

public:
    static int ConvertLoadCaseEnum(const enCaseKeyType Type);
    static int ConvertLcomKindEnum(const enLcomKeyType Type);

    static enCaseKeyType ConvertLcomCaseType(const int Type);
    static enLcomKeyType ConvertLcomKindType(const int Type);

    static bool IsLcomCaseType(const int Type);

    static int GetCaseKeyList(std::vector<enCaseKeyType>& vCase);
    static int GetLcomKeyList(std::vector<enLcomKeyType>& vLcom);

    static void Convert(const bool& bMax, const T_STRB_D& Strb, dgn::def::TRawForce1D6Elem& Data);
    static void Convert(const bool& bMax, const T_STRB_D& Strb, dgn::def::TRawForce1D7Elem& Data);

    static void Convert(const bool& bMax, const T_SBCF_D& Sbcf, dgn::def::TRawForce1D6Elem& Data);
    static void Convert(const bool& bMax, const T_SBCF_D& Sbcf, dgn::def::TRawForce1D7Elem& Data);

    static void Convert(const bool& bMax, const T_STRT_D& Strt, dgn::def::TRawForce1D6Elem& Data);
    static void Convert(const bool& bMax, const T_STRT_D& Strt, dgn::def::TRawForce1D7Elem& Data);

    static void Convert(const bool& bMax, const T_STRW_D& StrW, dgn::def::TRawForce1D6Elem& Data);
    static void Convert(const bool& bMax, const T_STRW_D& StrW, dgn::def::TRawForce1D7Elem& Data);

    static void Convert(const T_DISP_D& DispMaxD, const T_DISP_D& DispMinD, dgn::def::TRawDeformNode& Data);

    static void Convert(const bool& bMax, const T_STRP_DL& Strp, dgn::def::TRawForce2D& Data);
    static void Convert(const bool& bMax, const T_STRP_DL& Strp, dgn::def::TRawStress2D& Data);

    static void Convert(const bool& bMax, const T_SPCF_DL& Spcf, dgn::def::TRawForce2D& Data);

    static void Convert(const T_REAC_D& ReacD, dgn::def::TRawDeformReact& Data);

private:
    CDBDoc* GetDBDoc() const;

    CDBDoc* m_pDBDoc;
};

#include "HeaderPost.h"

template <typename T>
void ConvertC2V(const CArray<T, T>& A, std::vector<T>& V)
{
    auto nSize = A.GetSize();
    V.reserve(nSize);

    for ( decltype(nSize) nIdx = 0; nIdx < nSize; ++nIdx )
    {
        V.push_back(A[nIdx]);
    }
}

template <typename T1, typename T2, typename Func>
void ConvertC2V_F(const CArray<T1, T1>& A, std::vector<T2>& V, Func _Func)
{
    auto nSize = A.GetSize();
    V.reserve(nSize);

    for ( decltype(nSize) nIdx = 0; nIdx < nSize; ++nIdx )
    {
        V.push_back(_Func(A[nIdx]));
    }
}

template <typename T>
void ConvertC2V(const CArray<T>& A, std::vector<T>& V)
{
    auto nSize = A.GetSize();
    V.reserve(nSize);

    for ( decltype(nSize) nIdx = 0; nIdx < nSize; ++nIdx )
    {
        V.push_back(A[nIdx]);
    }
}

template <typename T1, typename T2, typename Func>
void ConvertC2V_F(const CArray<T1, T1&>& A, std::vector<T2>& V, Func _Func)
{
    auto nSize = A.GetSize();
    V.reserve(nSize);

    for ( decltype(nSize) nIdx = 0; nIdx < nSize; ++nIdx )
    {
        V.push_back(_Func(A[nIdx]));
    }
}