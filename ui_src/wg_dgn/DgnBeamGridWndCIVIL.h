#if !defined(AFX_DGNBEAMGRIDWNDCIVIL_H__DBAB41C3_7E03_4493_882F_C1BFD0A20EDD__INCLUDED_)
#define AFX_DGNBEAMGRIDWNDCIVIL_H__DBAB41C3_7E03_4493_882F_C1BFD0A20EDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnBeamGridWndCIVIL.h : header file
//

#include "..\wg_common\wg_common_TBBrowserWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnBeamGridWndCIVIL window

#define D_REBAR_GRID_COL_LAYER 0
#define D_REBAR_GRID_COL_NUM_CTC   1
#define D_REBAR_GRID_COL_SIZE1 2
#define D_REBAR_GRID_COL_SIZE2 3
#define D_REBAR_GRID_COL_DT_DB 4
#define D_REBAR_GRID_COL_SIZE  D_REBAR_GRID_COL_DT_DB

#define WM_USER_BEFORE_END_EDITING 0x0401

struct T_RCHK_BEAM;
class CDBDoc;
class DgnConBeamBarDlgCIVIL;

class CDgnBeamGridWndCIVIL : public CTBBrowserWnd
{
// Construction
public:
	CDgnBeamGridWndCIVIL(T_RCHK_BEAM* pBeamData, CWnd* pParent = NULL);
	virtual ~CDgnBeamGridWndCIVIL();

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
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL ValidateRecord(CStringArray &aValue, CRowColArray &aCols);

	//virtual BOOL OnInsertEmptyRecord(ROWCOL nRow);

//----------------------------------------------------------

//----------------------------------------------------------
// Interface
public:
	void Initialize();
	void MakeItemEx();
	//void UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_RPSC_MBAR, T_RPSC_MBAR&> &aData);

	BOOL  GetColumnNoFor(int nID, ROWCOL &nCol) { nCol = nID+1; return TRUE; }
	BOOL  GetColumnIDFor(ROWCOL nCol, int &nID) { nID = nCol-1; return TRUE; }

//----------------------------------------------------------
// Implementation
public:
	void SetColInfo();
	
	inline void SetTopBottom(int nTopBtm) { m_nTopBtm = nTopBtm; }
	 void SetNumCTC(int nNumCTC);

protected:
	//void SetValue(ROWCOL nRow, int index, T_RPSC_MBAR &data);
	//void SetHeaderTitle();
	//void ConvDataToStrArray(T_RPSC_MBAR &data, CStringArray &aValue);

	//BOOL InsertRecordByKey(int index, T_RPSC_MBAR &data);
	//BOOL DeleteRecordByKey(int index, T_RPSC_MBAR &data);

	void  SelectGridRow(long* aRow, long nSize);

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

	//CString GetArrangeStr(int data);
	//int GetArrangeData(CString &value);
	//CString GetRefPosStr(int data);
	//int GetRefPosData(CString &value);
	
	//Exist.
public:
	//void Initialize();
	void SetLayerNum(UINT nLayer);

	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual void OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

protected:
	int m_nOldCol;
	DgnConBeamBarDlgCIVIL*  m_pParent;  // Veiw Pointer
	T_RCHK_BEAM*            m_pBeamData;

	int m_nColHeader;
	int m_nColCount;
	int m_nTopBtm;  // 0: Top, 1: Bottom
	int m_nNumCTC;  // 0: Num, 1: CTC

	void SetHeaderTitle();
	void InitSizeComBoBox();

	//{{AFX_MSG(CDgnBeamGridWndCIVIL)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNBEAMGRIDWNDCIVIL_H__DBAB41C3_7E03_4493_882F_C1BFD0A20EDD__INCLUDED_)
