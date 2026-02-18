#if !defined(__SEISEVAL_PRINT_H__)
#define __SEISEVAL_PRINT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\SeisEvalStruct.h"

class CSeisEval;
class CSeisEvalPrint
{
public:
    CSeisEvalPrint(void);
    virtual ~CSeisEvalPrint(void);

public:    
    BOOL PrintLinRsltInfWall();

    BOOL Print1stRsltConBeam();
    BOOL Print1stRsltConColm();
    BOOL Print1stRsltConWall();
    BOOL Print1stRsltStlBeam();
    BOOL Print1stRsltStlColm();
    BOOL Print1stRsltStlPanz();
    BOOL Print1stRsltStlBrac();
    BOOL Print1stRsltSrcBeam();
    BOOL Print1stRsltSrcColm();

    BOOL Print1stRsltSrcBeam4Memb();
    BOOL Print1stRsltSrcColm4Memb();

private:
    BOOL Print1stRsltConColm_KISTEC2013();
    BOOL Print1stRsltConColm_MOE2018();

    CString GetValueForm(const int nTotalDigit, const int nUnderDigit, const double dValue);
    CString GetPerformLevel(const EN_LEVEL enLevel);
    CString GetEvalCtrlType(const EN_EVALCTRL enCtrl);
    CString GetSeisConfinedType(BOOL bSeisConform);
    CString GetConColmGroup_MOE2018(UINT unGroup);

    BOOL CheckExistLcom(const int nLcomType);
    CString GetCurrentUnitString();

    CString GetOutFileName(const CString& strOutType);    
};

#endif // !defined(__SEISEVAL_PRINT_H__)