// Rating_CS454_XLOut.h: interface for the CRating_CS454_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Rating_CS454_XLOut_H__)
#define _Rating_CS454_XLOut_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Rating_BD21_XLOut.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRating_CS454_XLOut : public CRating_BD21_XLOut
{
public:
    CRating_CS454_XLOut(void);
    virtual ~CRating_CS454_XLOut(void);

protected:
    virtual CString GetFlexTableHeadName();
    virtual CString GetShearTableHeadName();
    virtual CString GetAssessFactorContentsName();

};

#include "HeaderPost.h"

#endif // !defined(_Rating_CS454_XLOut_H__)
