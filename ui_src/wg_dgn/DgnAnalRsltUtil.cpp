#include "StdAfx.h"

#include <array>
#include <vector>

#include "../../dgnengine/idesign/DGN_lib/Macro.h"

#include "DgnAnalRsltUtil.h"

using namespace dgn::def;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CDgnAnalRsltUtil::CDgnAnalRsltUtil(CDBDoc* pDBDoc) :
    m_pDBDoc(pDBDoc)
{
}

CDgnAnalRsltUtil::~CDgnAnalRsltUtil()
{
}

int CDgnAnalRsltUtil::ConvertLoadCaseEnum(const enCaseKeyType Type)
{
    switch ( Type )
    {
    case enCaseKeyStatic:      return D_LOADCASE_STATIC;
    case enCaseKeySpectrum:    return D_LOADCASE_SPECTRUM;
    case enCaseKeySpectrumND:  return D_LOADCASE_SPECTRUM_ND;
    case enCaseKeyHistory:     return D_LOADCASE_HISTORY;
    case enCaseKeyMoving:      return D_LOADCASE_MOVING;
    case enCaseKeySettle:      return D_LOADCASE_SETTLE;
    case enCasePushOver:       return D_LOADCASE_PUSHOVER;
    case enCaseStage:          return D_LOADCASE_STAGE;
    case enCaseEccenspec:      return D_LOADCASE_ECCENSPEC;
    case enCaseEccenspecNd:    return D_LOADCASE_ECCENSPEC_ND;
    case enCaseGridModel:      return D_LOADCASE_GRIDMODEL;
    case enCaseCrane:          return D_LOADCASE_CRANE;
    default: ASSERT(0); return D_LOADCASE_NONE;
    }
}

const int D_LCOMTYPE_NONE = 0;
int CDgnAnalRsltUtil::ConvertLcomKindEnum(const enLcomKeyType Type)
{
    switch ( Type )
    {
    case enLcomKeyGeneral:    return D_LCOMTYPE_GENERAL;
    case enLcomKeySteel:      return D_LCOMTYPE_STEEL;
    case enLcomKeyConcrete:   return D_LCOMTYPE_CONCRETE;
    case enLcomKeyFdn:        return D_LCOMTYPE_FDN;
    case enLcomKeySrc:        return D_LCOMTYPE_SRC;
    case enLcomKeyAluminum:   return D_LCOMTYPE_ALUMINUM;
    case enLcomKeyStlComp:    return D_LCOMTYPE_STLCOMP;
    case enLcomKeyCfSteel:    return D_LCOMTYPE_CFSTEEL;
    case enLcomKeySeismic:    return D_LCOMTYPE_SEISMIC;
    case enLcomKeyLinearEval: return D_LCOMTYPE_LINEAR_EVAL;
    default: ASSERT(0); return D_LCOMTYPE_NONE;
    }
}

enCaseKeyType CDgnAnalRsltUtil::ConvertLcomCaseType(const int Type)
{
    switch ( Type )
    {
    case D_LCOM_STATIC:       return enCaseKeyStatic;
    case D_LCOM_SPECTRUM:     return enCaseKeySpectrum;
    case D_LCOM_SPECTRUM_ND:  return enCaseKeySpectrumND;
    case D_LCOM_HISTORY:      return enCaseKeyHistory;
    case D_LCOM_MOVING:       return enCaseKeyMoving;
    case D_LCOM_SETTLEMENT:   return enCaseKeySettle;
    case D_LCOM_PUSHOVER:     return enCasePushOver;
    case D_LCOM_STAGE:        return enCaseStage;
    case D_LCOM_ECCENSPEC:    return enCaseEccenspec;
    case D_LCOM_ECCENSPEC_ND: return enCaseEccenspecNd;
    case D_LCOM_GRIDMODEL:    return enCaseGridModel;
    case D_LCOM_CRANE:        return enCaseCrane;
    default: ASSERT(0); return enCaseKeyNone;
    }
}

enLcomKeyType CDgnAnalRsltUtil::ConvertLcomKindType(const int Type)
{
    switch ( Type )
    {
    case D_LCOMTYPE_GENERAL:     return enLcomKeyGeneral;
    case D_LCOMTYPE_STEEL:       return enLcomKeySteel;
    case D_LCOMTYPE_CONCRETE:    return enLcomKeyConcrete;
    case D_LCOMTYPE_FDN:         return enLcomKeyFdn;
    case D_LCOMTYPE_SRC:         return enLcomKeySrc;
    case D_LCOMTYPE_ALUMINUM:    return enLcomKeyAluminum;
    case D_LCOMTYPE_STLCOMP:     return enLcomKeyStlComp;
    case D_LCOMTYPE_CFSTEEL:     return enLcomKeyCfSteel;
    case D_LCOMTYPE_SEISMIC:     return enLcomKeySeismic;
    case D_LCOMTYPE_LINEAR_EVAL: return enLcomKeyLinearEval;
    default: ASSERT(0); return enLcomKeyNone;
    }
}

bool CDgnAnalRsltUtil::IsLcomCaseType(const int Type)
{
    switch ( Type )
    {
    case D_LCOM_STATIC:
    case D_LCOM_SPECTRUM:
    case D_LCOM_SPECTRUM_ND:
    case D_LCOM_HISTORY:
    case D_LCOM_MOVING:
    case D_LCOM_SETTLEMENT:
    case D_LCOM_PUSHOVER:
    case D_LCOM_STAGE:
    case D_LCOM_ECCENSPEC:
    case D_LCOM_ECCENSPEC_ND:
    case D_LCOM_GRIDMODEL:
    case D_LCOM_CRANE:
        return false;
    default:
        return true;
    }
}

const std::array<enCaseKeyType, enCaseKeyNumber - 1> caCaseKeyType =
{
    enCaseKeyStatic,
    enCaseKeySpectrum,
    enCaseKeySpectrumND,
    enCaseKeyHistory,
    enCaseKeyMoving,
    enCaseKeySettle,
    enCasePushOver,
    enCaseStage,
    enCaseEccenspec,
    enCaseEccenspecNd,
    enCaseGridModel,
    enCaseCrane,
};

int CDgnAnalRsltUtil::GetCaseKeyList(std::vector<enCaseKeyType>& vCase)
{
    vCase.clear();

    std::copy(caCaseKeyType.begin(), caCaseKeyType.end(), std::back_inserter(vCase));

    return Macro::GetSize(vCase);
}

const std::array<enLcomKeyType, enLcomKeyNumber - 1> caLcomKeyType =
{
    enLcomKeyGeneral,
    enLcomKeySteel,
    enLcomKeyConcrete,
    enLcomKeyFdn,
    enLcomKeySrc,
    enLcomKeyAluminum,
    enLcomKeyStlComp,
    enLcomKeyCfSteel,
    enLcomKeySeismic,
    enLcomKeyLinearEval,
};

int CDgnAnalRsltUtil::GetLcomKeyList(std::vector<enLcomKeyType>& vLcom)
{
    vLcom.clear();

    std::copy(caLcomKeyType.begin(), caLcomKeyType.end(), std::back_inserter(vLcom));

    return Macro::GetSize(vLcom);
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRB_D& Strb, TRawForce1D6Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    for ( int nIdx = 0; nIdx < 4; ++nIdx )
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 6; ++j)
        {
            int iCom = iMxMn + 6 * j;
            Data[ni][iCom + 0] = Strb.dblForce[nIdx][0];
            Data[ni][iCom + 1] = Strb.dblForce[nIdx][1];
            Data[ni][iCom + 2] = Strb.dblForce[nIdx][2];
            Data[ni][iCom + 3] = Strb.dblForce[nIdx][3];
            Data[ni][iCom + 4] = Strb.dblForce[nIdx][4];
            Data[ni][iCom + 5] = Strb.dblForce[nIdx][5];

            Data[nj][iCom + 0] = Strb.dblForce[nIdx][6];
            Data[nj][iCom + 1] = Strb.dblForce[nIdx][7];
            Data[nj][iCom + 2] = Strb.dblForce[nIdx][8];
            Data[nj][iCom + 3] = Strb.dblForce[nIdx][9];
            Data[nj][iCom + 4] = Strb.dblForce[nIdx][10];
            Data[nj][iCom + 5] = Strb.dblForce[nIdx][11];
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRB_D& Strb, TRawForce1D7Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    for ( int nIdx = 0; nIdx < 4; ++nIdx )
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 9; ++j)
        {
            int iCom = iMxMn + 9 * j;
            Data[ni][iCom + 0] = Strb.dblForce[nIdx][0];
            Data[ni][iCom + 1] = Strb.dblForce[nIdx][1];
            Data[ni][iCom + 2] = Strb.dblForce[nIdx][2];
            Data[ni][iCom + 3] = Strb.dblForce[nIdx][3];
            Data[ni][iCom + 4] = Strb.dblForce[nIdx][4];
            Data[ni][iCom + 5] = Strb.dblForce[nIdx][5];
            Data[ni][iCom + 6] = Strb.dblForce[nIdx][12];
            Data[ni][iCom + 7] = Strb.dblForce[nIdx][13];
            Data[ni][iCom + 8] = Strb.dblForce[nIdx][14];

            Data[nj][iCom + 0] = Strb.dblForce[nIdx][6];
            Data[nj][iCom + 1] = Strb.dblForce[nIdx][7];
            Data[nj][iCom + 2] = Strb.dblForce[nIdx][8];
            Data[nj][iCom + 3] = Strb.dblForce[nIdx][9];
            Data[nj][iCom + 4] = Strb.dblForce[nIdx][10];
            Data[nj][iCom + 5] = Strb.dblForce[nIdx][11];
            Data[nj][iCom + 6] = Strb.dblForce[nIdx][15];
            Data[nj][iCom + 7] = Strb.dblForce[nIdx][16];
            Data[nj][iCom + 8] = Strb.dblForce[nIdx][17];
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_SBCF_D& Sbcf, dgn::def::TRawForce1D6Elem& Data)
{
    //TODO
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    for ( int nIdx = 0; nIdx < 4; ++nIdx )
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for ( int j = 0; j < 6; ++j )
        {
            int iCom = iMxMn + 6 * j;
            Data[ni][iCom + 0] = Sbcf.dblForce[nIdx][0][j][0];
            Data[ni][iCom + 1] = Sbcf.dblForce[nIdx][0][j][1];
            Data[ni][iCom + 2] = Sbcf.dblForce[nIdx][0][j][2];
            Data[ni][iCom + 3] = Sbcf.dblForce[nIdx][0][j][3];
            Data[ni][iCom + 4] = Sbcf.dblForce[nIdx][0][j][4];
            Data[ni][iCom + 5] = Sbcf.dblForce[nIdx][0][j][5];
                                                        
            Data[nj][iCom + 0] = Sbcf.dblForce[nIdx][1][j][0];
            Data[nj][iCom + 1] = Sbcf.dblForce[nIdx][1][j][1];
            Data[nj][iCom + 2] = Sbcf.dblForce[nIdx][1][j][2];
            Data[nj][iCom + 3] = Sbcf.dblForce[nIdx][1][j][3];
            Data[nj][iCom + 4] = Sbcf.dblForce[nIdx][1][j][4];
            Data[nj][iCom + 5] = Sbcf.dblForce[nIdx][1][j][5];
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_SBCF_D& Sbcf, dgn::def::TRawForce1D7Elem& Data)
{
    //TODO
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    for ( int nIdx = 0; nIdx < 4; ++nIdx )
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for ( int j = 0; j < 9; ++j )
        {
            int iCom = iMxMn + 9 * j;
            Data[ni][iCom + 0] = Sbcf.dblForce[nIdx][0][j][0];
            Data[ni][iCom + 1] = Sbcf.dblForce[nIdx][0][j][1];
            Data[ni][iCom + 2] = Sbcf.dblForce[nIdx][0][j][2];
            Data[ni][iCom + 3] = Sbcf.dblForce[nIdx][0][j][3];
            Data[ni][iCom + 4] = Sbcf.dblForce[nIdx][0][j][4];
            Data[ni][iCom + 5] = Sbcf.dblForce[nIdx][0][j][5];
            Data[ni][iCom + 6] = Sbcf.dblForce[nIdx][0][j][6];
            Data[ni][iCom + 7] = Sbcf.dblForce[nIdx][0][j][7];
            Data[ni][iCom + 8] = Sbcf.dblForce[nIdx][0][j][8];
                                                        
            Data[nj][iCom + 0] = Sbcf.dblForce[nIdx][1][j][0];
            Data[nj][iCom + 1] = Sbcf.dblForce[nIdx][1][j][1];
            Data[nj][iCom + 2] = Sbcf.dblForce[nIdx][1][j][2];
            Data[nj][iCom + 3] = Sbcf.dblForce[nIdx][1][j][3];
            Data[nj][iCom + 4] = Sbcf.dblForce[nIdx][1][j][4];
            Data[nj][iCom + 5] = Sbcf.dblForce[nIdx][1][j][5];
            Data[nj][iCom + 6] = Sbcf.dblForce[nIdx][1][j][6];
            Data[nj][iCom + 7] = Sbcf.dblForce[nIdx][1][j][7];
            Data[nj][iCom + 8] = Sbcf.dblForce[nIdx][1][j][8];
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRT_D& Strt, TRawForce1D6Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    // Strt.dblForce // I, J
    double dStep = (Strt.dblForce[1] - Strt.dblForce[0]) / 4.0;

    for (int nIdx = 0; nIdx < 4; ++nIdx)
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 6; ++j)
        {
            int iCom = iMxMn + 6 * j;
            Data[ni][iCom] = Strt.dblForce[0] + dStep * nIdx;
            Data[nj][iCom] = Strt.dblForce[0] + dStep * (nIdx + 1);
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRT_D& Strt, TRawForce1D7Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    double dStep = (Strt.dblForce[1] - Strt.dblForce[0]) / 4.0;

    for (int nIdx = 0; nIdx < 4; ++nIdx)
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 9; ++j)
        {
            int iCom = iMxMn + 9 * j;
            Data[ni][iCom] = Strt.dblForce[0] + dStep * nIdx;
            Data[nj][iCom] = Strt.dblForce[0] + dStep * (nIdx + 1);
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRW_D& Strw, TRawForce1D6Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    // Strw.dblForce // Top. Bottom
    double dStep[6];
    for (int i = 0; i < 6; ++i)
    {
        dStep[i] = (Strw.dblForce[0][i] - Strw.dblForce[1][i]) / 4.0;
    }

    for (int nIdx = 0; nIdx < 4; ++nIdx)
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 6; ++j)
        {
            int iCom = iMxMn + 6 * j;
            for (int k = 0; k < 6; ++k)
            {
                Data[ni][iCom + k] = Strw.dblForce[1][k] + dStep[k] * nIdx;
                Data[nj][iCom + k] = Strw.dblForce[1][k] + dStep[k] * (nIdx+1);
            }
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRW_D& Strw, TRawForce1D7Elem& Data)
{
    int iMxMn = bMax ? Data[0].GetMaxStIndex() : Data[0].GetMinStIndex();
    double dStep[6];
    for (int i = 0; i < 6; ++i)
    {
        dStep[i] = (Strw.dblForce[1][i] - Strw.dblForce[0][i]) / 4.0;
    }

    for (int nIdx = 0; nIdx < 4; ++nIdx)
    {
        int ni = nIdx * 2;
        int nj = ni + 1;

        for (int j = 0; j < 9; ++j)
        {
            int iCom = iMxMn + 9 * j;
            for (int k = 0; k < 9; ++k)
            {
                Data[ni][iCom + k] = Strw.dblForce[0][k] + dStep[k] * nIdx;
                Data[nj][iCom + k] = Strw.dblForce[0][k] + dStep[k] * (nIdx + 1);
            }
        }
    }
}

void CDgnAnalRsltUtil::Convert(const T_DISP_D& DispMaxD, const T_DISP_D& DispMinD, TRawDeformNode& Data)
{
    for (int i = 0; i < 6; ++i)
    {
        int iIdx = i * 6;
        Data[iIdx+0] = DispMaxD.dblDisp[0]; // Dx
        Data[iIdx+1] = DispMaxD.dblDisp[1]; // Dy
        Data[iIdx+2] = DispMaxD.dblDisp[2]; // Dz
        Data[iIdx+3] = DispMaxD.dblDisp[3]; // Rx
        Data[iIdx+4] = DispMaxD.dblDisp[4]; // Ry
        Data[iIdx+5] = DispMaxD.dblDisp[5]; // Rz

        iIdx += 36;
        Data[iIdx + 0] = DispMinD.dblDisp[0]; // Dx
        Data[iIdx + 1] = DispMinD.dblDisp[1]; // Dy
        Data[iIdx + 2] = DispMinD.dblDisp[2]; // Dz
        Data[iIdx + 3] = DispMinD.dblDisp[3]; // Rx
        Data[iIdx + 4] = DispMinD.dblDisp[4]; // Ry
        Data[iIdx + 5] = DispMinD.dblDisp[5]; // Rz
    }
}

void CDgnAnalRsltUtil::Convert(const T_REAC_D& ReacD, TRawDeformReact& Data)
{
    for (int i = 0; i < REAC_SIZE; ++i)
    {
        int iIdx = i * REAC_SIZE;
        Data[iIdx + 0] = ReacD.dblReac[0];
        Data[iIdx + 1] = ReacD.dblReac[1];
        Data[iIdx + 2] = ReacD.dblReac[2];
        Data[iIdx + 3] = ReacD.dblReac[3];
        Data[iIdx + 4] = ReacD.dblReac[4];
        Data[iIdx + 5] = ReacD.dblReac[5];
        Data[iIdx + 6] = ReacD.dblReac[6];

        iIdx += (REAC_SIZE*REAC_SIZE);
        Data[iIdx + 0] = ReacD.dblReac[0];
        Data[iIdx + 1] = ReacD.dblReac[1];
        Data[iIdx + 2] = ReacD.dblReac[2];
        Data[iIdx + 3] = ReacD.dblReac[3];
        Data[iIdx + 4] = ReacD.dblReac[4];
        Data[iIdx + 5] = ReacD.dblReac[5];
        Data[iIdx + 6] = ReacD.dblReac[6];
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRP_DL& Strp, TRawForce2D& Data)
{
    for ( int i = 0; i < 5; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            int iIdx = bMax ? (j * 8) : 8*8 + (j * 8);
            Data[i][iIdx + 0] = Strp.dblStress[i][14]; // Fxx
            Data[i][iIdx + 1] = Strp.dblStress[i][15]; // Fyy
            Data[i][iIdx + 2] = Strp.dblStress[i][16]; // Fxy
            Data[i][iIdx + 3] = Strp.dblStress[i][20]; // Mxx
            Data[i][iIdx + 4] = Strp.dblStress[i][21]; // Myy
            Data[i][iIdx + 5] = Strp.dblStress[i][22]; // Mxy
            Data[i][iIdx + 6] = Strp.dblStress[i][26]; // Vxx
            Data[i][iIdx + 7] = Strp.dblStress[i][27]; // Vyy
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_STRP_DL& Strp, TRawStress2D& Data)
{
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int iIdx = bMax ? (j * 4) : 4*4 + (j * 4);
            Data[i][iIdx + 0] = Strp.dblStress[i][3];  
            Data[i][iIdx + 1] = Strp.dblStress[i][4];  
            Data[i][iIdx + 2] = Strp.dblStress[i][10]; 
            Data[i][iIdx + 3] = Strp.dblStress[i][11]; 
        }
    }
}

void CDgnAnalRsltUtil::Convert(const bool& bMax, const T_SPCF_DL& Spcf, dgn::def::TRawForce2D& Data)
{
    for ( int i = 0; i < 5; ++i )
    {
        for ( int j = 0; j < 8; ++j )
        {
            int iIdx = bMax ? (j * 8) : 8*8 + (j * 8);
            Data[i][iIdx + 0] = Spcf.dForce[i][j][0]; // Fxx
            Data[i][iIdx + 1] = Spcf.dForce[i][j][1]; // Fyy
            Data[i][iIdx + 2] = Spcf.dForce[i][j][2]; // Fxy
            Data[i][iIdx + 3] = Spcf.dForce[i][j][3]; // Mxx
            Data[i][iIdx + 4] = Spcf.dForce[i][j][4]; // Myy
            Data[i][iIdx + 5] = Spcf.dForce[i][j][5]; // Mxy
            Data[i][iIdx + 6] = Spcf.dForce[i][j][6]; // Vxx
            Data[i][iIdx + 7] = Spcf.dForce[i][j][7]; // Vyy
        }
    }
}

CDBDoc* CDgnAnalRsltUtil::GetDBDoc() const
{
    return m_pDBDoc;
}