#pragma once

#include "SeisEvalMOE2018.h"

class CSeisEvalMOE2019 : public CSeisEvalMOE2018
{
public:
    CSeisEvalMOE2019(void);
    virtual ~CSeisEvalMOE2019(void);

protected:    
    virtual double GetAlphaConColm(const T_SECT_D& SectD);
    virtual double GetPhiInfill();
    virtual double CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu);
    virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) override;

    virtual BOOL IsConsiderVbjs(T_ELEM_K ElemK);

private:
    BOOL   IsOkOutPlane(const T_MSIS_D& MsisD, const T_SEMM_D& SemmD);
    BOOL   IsNeedCheckOutPlane(int nMasonryJoint, double dhm, double dtm, int nPerformLevel, double dSDS, double dSD1);
    BOOL   IsArchingAction(int nMasonryJoint, double dhmtm);
    double CalcHmTmLimitTable6_7(int nPerformLevel, double dVal, double dMin, double dMax);

    double CalcQtnArchingAction(double dhm, double dtm, double dfmc);
    double CalcQtnCantilever(double dhm, double dtm, double dfmt);
    double CalcFp(double nPerformLevel, double dtm, double dSDS, double dIe);

    double CalcLambda2(double dhm, double dtm);
    double GetXi(int nPerformLevel);

};

