// DgnConBeamRebarGrid.h: interface for the CDgnConBeamRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CMatlByRebarDiaGrid_H__)
#define __CMatlByRebarDiaGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUnitCtrl;
class CMatlDB;

struct T_DCRB_D;
class CMatlByRebarDiaGrid : public CGXGridWnd
{
public:
	CMatlByRebarDiaGrid();
//	virtual ~CDgnConBeamRebarGrid();

	enum GRID_MODE/*:unsigned int*/
	{
		VERT_RABAR = 0,
		HORZ_REBAR,
		MSRB_REBAR,
	};
	// Interface
public:
	void Initialize(CWnd* pParent, T_DCRB_D* const pDcrb, const GRID_MODE eMode);
	void Initialize(CWnd* pParent, T_MSRB_D* const pMsrb);

	void EnableGrid(int nRow, int nCol, BOOL bEnable=TRUE);

	BOOL Data2Grid(T_DCRB_D* pData);
	BOOL Data2Grid(T_MSRB_D* pData);

	BOOL Grid2Data(T_DCRB_D* pData);
	BOOL Grid2Data(T_MSRB_D* pData);

	void AddRow(BOOL bCurrent);
	void DelRow(BOOL bCurrent);

	// override 
public:
//   virtual BOOL Copy();
//   virtual BOOL Paste();
//   virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnCanceledEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

	virtual ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

	// Operation
protected:
	void Initialize_Grid();
	BOOL Data2Grid(CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&>* paMatl);
	BOOL Grid2Data(CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&>* paMatl);

	void SetSizeColumn();
	void SetDataColumn();

	void SetHeaderTitle();
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	CString Get_ListRebar();
	CString Get_ListMatl();

	CString Get_StressForm();

protected: 
	CUnitCtrl* m_pUnitCtrl;
	CMatlDB* m_pMatlDB;

	T_DCRB_D* m_pData;
	T_MSRB_D* m_pMarb;

	GRID_MODE m_eMode;
};

#endif // !defined(__CMatlByRebarDiaGrid_H__)