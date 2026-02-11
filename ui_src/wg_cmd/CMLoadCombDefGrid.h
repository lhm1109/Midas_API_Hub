#if !defined(AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\TBBrowserWndEx.h"
#include "LoadCombDefineData.h"
#pragma region InitData

#pragma endregion

/////////////////////////////////////////////////////////////////////////////
// CSpfcItemGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadCombDefGrid : public CTBBrowserWndEx
{
// Construction
public:
	CCMLoadCombDefGrid(BOOL bReset=FALSE);
	virtual ~CCMLoadCombDefGrid();
// Attributes
public:
	CLoadCombDefineData *m_pdata;
	CArrayEx<CArrayEx<double, double>, CArrayEx<double, double>&> m_aFactor;
	BOOL m_bInit;
	int m_nRow;
	int m_nCol;

// Operations
public:
	void Initialize(int nIdx, CLoadCombDefineData *pdata, BOOL bInit=TRUE);
	void Data2Grid();
	void Grid2Data();
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit);
	void SetTitle();
protected:
	CDBDoc* m_pDoc;
	BOOL m_bWind;
	void* m_pParent;

protected:
	void SetNewLineNo(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPFCITEMGRID_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
