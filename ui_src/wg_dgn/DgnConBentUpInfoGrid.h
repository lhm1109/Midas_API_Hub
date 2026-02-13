#pragma once

#include "../wg_common/TBBrowserWnd.h"

#define D_COL_NUM					1
#define D_COL_SIZE				2
#define D_COL_DB					3
#define D_COL_XI					4
#define D_COL_ANGLE  			5
#define D_COL_HI					6
#define D_COL_GRP_APP			7
#define D_COL_GRP_NUM			8
#define D_COL_GRP_GAP			9

class  CDBDoc;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpInfoGrid window
class CDgnConBentUpInfoGrid : public CTBBrowserWnd
{
public:
	CDgnConBentUpInfoGrid(CWnd* pParent = NULL);
	virtual ~CDgnConBentUpInfoGrid();

	void Initialize();
	void SetHeaderTitle();
	void UpdateUnit();
 	virtual void SetData(IN CArray<T_BURB_SUB_D, T_BURB_SUB_D&>& pData);	
 	virtual void GetData(OUT CArray<T_BURB_SUB_D, T_BURB_SUB_D&>& aData);	

	void GetCurSelRows(CArray<ROWCOL, ROWCOL>& aSelRows);	
	
protected:
	CDBDoc*  m_pDoc;
	CWnd*	   m_pParent;
	
	virtual CString GetDefValue(int nIndex);	
	BOOL OnInsertEmptyRecord(ROWCOL nRow);	
	void SetEnableByMod(ROWCOL nRow, ROWCOL nCol);
	void SetNoRowVal();

 	virtual void GetValue(ROWCOL nRow, T_BURB_SUB_D& BurbD);
 	virtual void GetValue(ROWCOL nRow, ROWCOL nCol, T_BURB_SUB_D& BurbD);	
 	virtual void SetValue(ROWCOL nRow, ROWCOL nCol, T_BURB_SUB_D& BurbD);
	virtual void SetValue(ROWCOL nRow, T_BURB_SUB_D& Geom);	

	virtual BOOL OnDeleteRecord(ROWCOL nRow);
	virtual BOOL OnDeleteRecords(CRowColArray &awRows);

protected:
	//{{AFX_MSG(CLGTHThermalDgnConBentUpInfoGrid)	
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnLButtonHitRowCol(ROWCOL nHitRow, ROWCOL nHitCol, ROWCOL nDragRow, ROWCOL nDragCol, CPoint point, UINT flags, WORD nHitState);	
	virtual BOOL OnValidateField(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};