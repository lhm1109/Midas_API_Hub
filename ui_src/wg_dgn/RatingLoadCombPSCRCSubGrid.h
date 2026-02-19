#if !defined(AFX_RATINGLOADCOMBPSCGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_RATINGLOADCOMBPSCGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombSubGrid.h : header file
//

#include "RatingLoadCombCommonSubGrid.h"

class CRatingLoadCombPSCRCSubGrid : public CRatingLoadCombCommonSubGrid
{
// Construction
public:
	CRatingLoadCombPSCRCSubGrid(int nType);
	virtual ~CRatingLoadCombPSCRCSubGrid();

// Attributes
public:

// Operations
public:  
	virtual void SetData(CString sKey);
	virtual CString GetDefFactorVal(int nGridType);

	void GetData(T_RKLC_D &data);

	// Generated message map functions
protected:
	//{{AFX_MSG(CRatingLoadCombPSCRCSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RATINGLOADCOMBPSCGRID_H__5B5AF003_5156_11D3_92DE_0000C0B0E6B3__INCLUDED_)
