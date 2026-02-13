#if !defined(AFX_DGNSTEELSPCSECTJGRID_H__)
#define AFX_DGNSTEELSPCSECTJGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSteelSPCSectJGrid.h : header file
//

#include "..\wg_common\wg_common_TBBrowserWndEx.h"
#include "DgnStlSPCSectJTabDlg.h"
//#include "..\wg_db\DB_ST_DT.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnSteelSPCSectJGrid window

class CDBDoc;
class CDgnStlSPCSectJTabDlg;

class CDgnSteelSPCSectJGrid : public CTBBrowserWnd
{
// Construction
public:
	CDgnSteelSPCSectJGrid(int nElemCount);
	virtual ~CDgnSteelSPCSectJGrid();

	BOOL PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
													CGXGridWnd &x, int &nPasteMode, int &nPastedCols);

//----------------------------------------------------------
// Overriadable
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	
/*  
	virtual BOOL OnValidateRecord(ROWCOL nRow);
	virtual BOOL OnAddNewRecord(ROWCOL nRow);
	virtual BOOL OnModifyRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);
*/

	virtual BOOL ValidateField(CString& value, int nColID);
	//virtual BOOL ValidateRecord(CStringArray &aValue, CRowColArray &aCols);

	//virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

//----------------------------------------------------------

//----------------------------------------------------------
// Interface
public:
	void Initialize(CDgnStlSPCSectJTabDlg* pParent);
	void MakeItemEx();
	//void UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_RPSC_MBAR, T_RPSC_MBAR&> &aData);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }

//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();

protected:
	//void SetValue(ROWCOL nRow, int index, T_RPSC_MBAR &data);
	//void SetHeaderTitle();
	//void ConvDataToStrArray(T_RPSC_MBAR &data, CStringArray &aValue);

	//BOOL InsertRecordByKey(int index, T_RPSC_MBAR &data);
	//BOOL DeleteRecordByKey(int index, T_RPSC_MBAR &data);  

	//BOOL GetValue(ROWCOL nRow, T_RPSC_MBAR &data);
	//BOOL MakeDataArray(CStringArray& value, CRowColArray& awCols, CArray<T_RPSC_MBAR, T_RPSC_MBAR&>& aData);
	//BOOL ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RPSC_MBAR &data);
	//BOOL ConvStrArrayToData(CStringArray &aValue, T_RPSC_MBAR& data);

	//BOOL AddToDB(ROWCOL nRow);
	//BOOL AddData(int nIndex, T_RPSC_MBAR &data);
	//BOOL ModifyToDB(ROWCOL nRow);
	//BOOL ModifyData(int nOldIndex, int nIndex, T_RPSC_MBAR &data);
	//BOOL DeleteFromDB(CRowColArray &awRow);

	CString GetDefValue(int ix);

	CDBDoc* m_pDoc;	

	//CString GetArrangeStr(int data);
	//int GetArrangeData(CString &value);
	//CString GetRefPosStr(int data);
	//int GetRefPosData(CString &value);
	
	//Exist.
public:
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);

protected:
	int m_nOldCol;
		
	int m_nColHeader;
	int m_nColCount;
	CDgnStlSPCSectJTabDlg* m_pParent;

	void SetHeaderTitle();  

	//{{AFX_MSG(CDgnSteelSPCSectJGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTEELSPCSECTJGRID_H__)
