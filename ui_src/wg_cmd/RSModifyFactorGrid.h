#if !defined(__RSModifyFactorGrid_H__)
#define __RSModifyFactorGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSModifyFactorGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\TBCommon.h"


/////////////////////////////////////////////////////////////////////////////
// CRSModifyFactorGrid window
class CDBDoc;
class CRSModifyFactorGrid : public CTBCommon
{

	// Construction
public:
	CRSModifyFactorGrid();
	virtual ~CRSModifyFactorGrid();

	// Operations
public:
	void Initialize();
	void SetData(CArray<T_RSMF_K, T_RSMF_K>& aRsmfK, CArray<T_RSMF_D, T_RSMF_D&>& aRsmfD);
	BOOL GetData(CArray<T_RSMF_K, T_RSMF_K>& raRsmfK, CArray<T_RSMF_D, T_RSMF_D&>& raRsmfD);
	void RemoveRecords();

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);


public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);    
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return TRUE; }
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CopyRange(const CGXRangeList& selList);
	virtual BOOL DoLButtonUp(UINT flags, CPoint point);



	virtual BOOL AddToDB(ROWCOL nRow);
	virtual BOOL AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData){return TRUE;}
	virtual BOOL DeleteFromDB(ROWCOL nRow){return TRUE;}
	virtual BOOL DeleteFromDB(CRowColArray& awRows){return TRUE;}
	virtual BOOL ModifyToDB(ROWCOL nRow){return TRUE;}
	virtual BOOL ModifyToDB(CStringArray& raOldKey, 
		CArray<void*, void*>& raKey, CArray<void*, void*>& raData){return TRUE;}

	virtual CString GetNewKey(){return "0";}
	virtual CString GetDefValue(int nIndex){return "0";}
	virtual CString* GetDefValueArray(){return NULL;}
	virtual BOOL MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
		CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
	{
		return TRUE;
	}
	virtual void DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData){return ;}

	virtual BOOL ValidateField(CString value, int nColID){return TRUE;}
	virtual BOOL ValidateRecord(CStringArray &value, CRowColArray &aCols){return TRUE;}
	virtual long GetCountData(){return GetRowCount();}
	virtual void MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
	{
		return ;
	}
	virtual void MakeItemNormal(long nNumData, CRowColArray& awCols)
	{
		return ;
	}
	virtual void MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
	{
		return ;
	}

protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);
	void SetEnableDisableCells(ROWCOL nRow);

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	BOOL IsPasteToAllRange(const CGXRange &range, CString szData); // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함;

	CDBDoc* m_pDoc;

	// Generated message map functions

	//{{AFX_MSG(CRSModifyFactorGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RSModifyFactorGrid_H__)
