#if !defined(__DgnSeisSetFactorH29AllGrid_H__)
#define __DgnSeisSetFactorH29AllGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisSetFactorH29Dlg.h : header file IDD_DGN_SEIS_SET_FACTOR_H29_DLG

#include "..\wg_common\wg_common_TBCommon.h"
#include "HeaderPre.h"

#define D_COMMON_COL_INDENT  0
#define D_COMMON_COL_1  1
#define D_COMMON_COL_2  2
#define D_COMMON_COL_3  3
#define D_COMMON_COL_4  4
#define D_COMMON_COL_5  5
#define D_COMMON_COL_6  6

#define D_COMMON_ROW_INDENT  0
#define D_COMMON_ROW_1  1
#define D_COMMON_ROW_2  2
#define D_COMMON_ROW_3  3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_RcPierGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_RcPierGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_RcPierGrid);

public:
	CDgnSeisSetFactorH29_RcPierGrid();
	~CDgnSeisSetFactorH29_RcPierGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);
	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_StlPierGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_StlPierGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_StlPierGrid);

public:
	CDgnSeisSetFactorH29_StlPierGrid();
	~CDgnSeisSetFactorH29_StlPierGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_CurvatureGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_CurvatureGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_CurvatureGrid);

public:
	CDgnSeisSetFactorH29_CurvatureGrid();
	~CDgnSeisSetFactorH29_CurvatureGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_MomentMydGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_MomentMydGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_MomentMydGrid);

public:
	CDgnSeisSetFactorH29_MomentMydGrid();
	~CDgnSeisSetFactorH29_MomentMydGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_MomentMudGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_MomentMudGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_MomentMudGrid);

public:
	CDgnSeisSetFactorH29_MomentMudGrid();
	~CDgnSeisSetFactorH29_MomentMudGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_ShearSusdGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_ShearSusdGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_ShearSusdGrid);

public:
	CDgnSeisSetFactorH29_ShearSusdGrid();
	~CDgnSeisSetFactorH29_ShearSusdGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_ShearSucdGrid

class __MY_EXT_CLASS__ CDgnSeisSetFactorH29_ShearSucdGrid : public CTBBrowserWnd
{
	DECLARE_DYNCREATE(CDgnSeisSetFactorH29_ShearSucdGrid);

public:
	CDgnSeisSetFactorH29_ShearSucdGrid();
	~CDgnSeisSetFactorH29_ShearSucdGrid();

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

public:
	void Initialize(const T_SIFA_H29_D& rData);
	void GetData(T_SIFA_H29_D& rData);

	// Generated message map functions
	//{{AFX_MSG(CSmSectRebarShearSubPcSteelGrid)
	//}}AFX_MSG

};

#include "HeaderPost.h"
#endif // !defined(__DgnSeisSetFactorH29Dlg_H__)