#if !defined(__STRUTTIECORNERGRID_H__)
#define __STRUTTIECORNERGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "StrutTieFormView.h"   // 입력 구조체 포함
//#include "..\MIDAS_util\EditGrid.h"
#include "..\wg_common\EditGrid.h"
#include "..\wg_common\ClassPaste.h"


class CDBDoc;
class CDgnStlModifyEffSectPropDlg;

//////////////////////////////////////////////////////////////////////////
class CDgnStlModifyEffSectPropULSGrid : public CEditGrid     
{
public:
	CDgnStlModifyEffSectPropULSGrid(CDgnStlModifyEffSectPropDlg* pParent = NULL);
	virtual ~CDgnStlModifyEffSectPropULSGrid();

public:
	void InitGrid();
	void SetData(T_SECT_STIFFNESS_CF_BASE *pData);   //m_Data의 자료를 grid로 대입
	BOOL GetData(T_SECT_STIFFNESS_CF_BASE *pData);

protected:
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

protected:
	CDgnStlModifyEffSectPropDlg *m_pParent;

};
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
class CDgnStlModifyEffSectPropSLSGrid : public CEditGrid     
{
public:
	CDgnStlModifyEffSectPropSLSGrid(CDgnStlModifyEffSectPropDlg* pParent = NULL);
	virtual ~CDgnStlModifyEffSectPropSLSGrid();

public:
	void InitGrid();
	void SetData(T_SECT_STIFFNESS_CF_BASE *pData);   //m_Data의 자료를 grid로 대입
	BOOL GetData(T_SECT_STIFFNESS_CF_BASE *pData);

protected:
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

protected:
	CDgnStlModifyEffSectPropDlg *m_pParent;

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__STRUTTIECORNERGRID_H__)
