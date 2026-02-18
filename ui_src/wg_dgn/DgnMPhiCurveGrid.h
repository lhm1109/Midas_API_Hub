// DgnMPhiCurveGrid.h: interface for the CDgnMPhiCurveGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNMPHICURVEGRID_H__AEAAD999_9CE3_4249_A1A6_40502723830C__INCLUDED_)
#define AFX_DGNMPHICURVEGRID_H__AEAAD999_9CE3_4249_A1A6_40502723830C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_common\TBBrowserWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDgnMPhiCurveDlg;
class CDgnMPhiCurveGrid : public CTBBrowserWnd
{
public:
	CDgnMPhiCurveGrid(CDgnMPhiCurveDlg *pParent);
	virtual ~CDgnMPhiCurveGrid();

	virtual void Initialize();
	void SetHeaderTitle();
	void MakeItemEx();

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);

public:
	BOOL GetSelectedMpccKeyList(CArray<T_MPCC_K, T_MPCC_K> &aMpccKey);
	BOOL SetSelectedMpccKey(T_MPCC_K MpccKey);
	BOOL GetSelectedMpccKey(T_MPCC_K &MpccKey);
	BOOL SelectAllorUnSelectAll(BOOL bSelect);

	BOOL GetMpccData(CArray<T_MPCC_K, T_MPCC_K> &aMpccKey, CArray<CString, CString> &aNameList);
	BOOL SetResultIndex(CArray<T_MPCC_K, T_MPCC_K> &aResultMpccKey);
	BOOL SetSelectedKey(CArray<T_MPCC_K, T_MPCC_K> &aSelectedMpccKey);

	CDBDoc* m_pDoc; 

	BOOL m_bSelCalc;
	CArray<int, int> m_aResultIndex;
	CArray<T_MPCC_K, T_MPCC_K> m_aSelectedMpccKey;

protected:
	CDgnMPhiCurveDlg* m_pParent;

};

#endif // !defined(AFX_DGNMPHICURVEGRID_H__AEAAD999_9CE3_4249_A1A6_40502723830C__INCLUDED_)
