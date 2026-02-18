// DgnUserMPhiGrid.h: interface for the CDgnUserMPhiGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNUSERMPHIGRID_H__CBA02DB3_B342_4983_BA08_0F834299CE77__INCLUDED_)
#define AFX_DGNUSERMPHIGRID_H__CBA02DB3_B342_4983_BA08_0F834299CE77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_common\TBBrowserWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_db\DB_ST_DN.h"


class CDgnUserMPhiDlg;
class CDgnUserMPhiGrid : public CTBBrowserWnd  
{
public:
	CDgnUserMPhiGrid(CDgnUserMPhiDlg* pParent = NULL);
	virtual ~CDgnUserMPhiGrid();

public:		
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	
	void Initialize(CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D>* parPierUserMPhi);
	void Data2Grid();
	void Grid2Data();
	void SetRowSize(int nSize);


protected:
	void SetHeaderTitle();

	int m_nHeadRow;
	int m_nReturnDir;

	CDgnUserMPhiDlg* m_pParent;
	CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D>* m_parPierUserMPhi;
};

#endif // !defined(AFX_DGNUSERMPHIGRID_H__CBA02DB3_B342_4983_BA08_0F834299CE77__INCLUDED_)
