//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_CSGRptManager_CS457_H__INCLUDED_)
#define AFX_DGN_CSGRptManager_CS457_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../wg_base/MSExcel.h"

#include "Dgn_DllManager_Base.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "DgnEngineProgressDlg.h"

namespace dgn
{
	namespace lib
	{
		class IExcel;
	}
}

struct CSG_RPT_CS457_D;

class CDgn_CSGRptManager_CS457 : public CDgn_DllManager_Base
{
public:
    CDgn_CSGRptManager_CS457();
    virtual ~CDgn_CSGRptManager_CS457();

protected:
    PDGNENGINE_RPT_PROG* m_pCS457Detail;

public:
    BOOL InitialData(int iNationType);

    BOOL Print_DetailReport(dgn::lib::IExcel* pExcel, D_XL_PROGRESS_INTERFACE* pProgDlg, int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, CSG_RPT_CS457_D &rData);
    
};
#endif // !defined(AFX_DGN_CSGRptManager_CS457_H__INCLUDED_)