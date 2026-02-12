#if !defined(__CM_EARTH_DLG_MOVER_H__)
#define __CM_EARTH_DLG_MOVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//  CMEarthPressureDlgMover.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMEarthPressureDlgMover
class CCMEarthPressureDlgMover
{
	// Construction
public:
	CCMEarthPressureDlgMover(CWnd* pParent, CWnd* pBaseDlg = NULL);   // standard constructor
	virtual ~CCMEarthPressureDlgMover();

	void SetSubCtrls(const CArray<UINT, UINT>& caCtrls, UINT uiHolderID);
	BOOL Execute(CWnd* pNewSubDlg);

protected:
	CWnd*              m_pDlgParent;
	CWnd*              m_pBaseDlg;
	CArray<UINT, UINT> m_caCtrls;
	UINT               m_IDHolder;

protected:
	BOOL MoveDlgCtrls(int nMoveVal);
	BOOL IsRealWindow(CWnd* pWnd);
};

#endif
