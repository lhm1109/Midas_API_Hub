#if !defined(__DGNSEISMPHICURVEGRID_H__)
#define __DGNSEISMPHICURVEGRID_H__

#include "..\wg_dgn\BrdgSeisEval.h"

class CDgnSeisMPhiCurveDlg;
class CDgnSeisMPhiCurveGrid : public CGXGridWnd
{

public:
	CDgnSeisMPhiCurveGrid();
	virtual ~CDgnSeisMPhiCurveGrid();

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
	void SetData2Grid(int nAngleDir, T_BRDGEVAL_MPHI_RES MphiResD[2]);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
};

#endif // !defined(__DGNSEISMPHICURVEGRID_H__)
