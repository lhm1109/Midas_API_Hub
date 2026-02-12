#if !defined(__SEISCVLFORCEDIRGRID_H__)
#define __SEISCVLFORCEDIRGRID_H__


class CSeisCvlForceDirDlg;
class CSeisCvlForceDirGrid : public CGXGridWnd
{

public:
	CSeisCvlForceDirGrid();
	virtual ~CSeisCvlForceDirGrid();

public:
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CanPaste() { return FALSE; }
	virtual BOOL CanCut() { return FALSE; }

public:
	void Initialize(CWnd *pParent);
	void SetData2Grid(T_ELEM_K TopElemK, T_ELEM_K BotElemK, double dTopMy, double dBotMy);
	
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
};

#endif // !defined(__SEISCVLFORCEDIRGRID_H__)
