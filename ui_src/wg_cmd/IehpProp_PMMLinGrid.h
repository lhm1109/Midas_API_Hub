#if !defined(AFX_IehpProp_PMMLinGrid_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_IehpProp_PMMLinGrid_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpfcItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
//#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CIehpProp_PMMLinGrid window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIehpProp_PMMLinGrid :public CTBBrowserWnd // public CTBCommon
{
	// Construction
public:
	CIehpProp_PMMLinGrid();

	// Attributes
public:
	T_IEHP_PMMULT* m_pData;
	T_IEHP_PMMULT_AXIAL* m_pDataAxial;
	static CString m_aCurDefVal[2];
	BOOL m_bPushOver;
	int m_nDof;
	int m_nSymmetry;

	// Operations
public:
	void Initialize(T_IEHP_PMMULT* pData, T_UNIT_SYSTEM &UnitSystem,  int nSymmetry, int nDof, int nAxalCmbIndex);
	void Setdata(T_IEHP_PMMULT* pData, int nSymmetry, int nAxalCmbIndex);

	void Data2Grid();
	void Grid2Data();

	//
	void DBChanged();
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	//
	int m_nHeadRow;
	int m_nReturnDir;

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType);

	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ValidateField(CString& value, int nColID);

	//void SetEnableDisable(ROWCOL nFromRow, ROWCOL nFromCol, ROWCOL nToRow, ROWCOL nToCol, BOOL bEnable);

	virtual const CString& GetValueRowCol(ROWCOL nRow, ROWCOL nCol);

	//(2007.12.26) Add by Unsang :: Copy "EidtGrid" Source
	BOOL SetReturnValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpProp_PMMLinGrid)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CIehpProp_PMMLinGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIehpProp_PMMLinGrid)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IehpProp_PMMLinGrid_H__5B311B43_B86C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
