#pragma once
#include "..\wg_cmd\CMGXGridWnd.h"
#include "..\wg_common\wg_common_TBGrid.h"

class CCMWindPressureFunctionGrid : public CCMGXGridWnd
{
public:
	CCMWindPressureFunctionGrid();
	virtual ~CCMWindPressureFunctionGrid();
protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
};

