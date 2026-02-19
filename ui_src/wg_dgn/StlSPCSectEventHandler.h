// CStlSPCSectEventHandler.h: interface for the FCMTendonEventHandler class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLSPCSECTEVENTHANDLER_H__)
#define AFX_STLSPCSECTEVENTHANDLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"


class CDgnStlSPCSectDlg;
class CStlSPCSectEventHandler  : public I_DlgView_EventHandler
{
public:
	CStlSPCSectEventHandler(CWnd* pDlg = NULL);
	virtual ~CStlSPCSectEventHandler();

	virtual void Initialize();

	virtual void  On_RButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV);
	virtual void  On_LButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV);

	I_DlgViewPanel* GetDlgViewPanel();
	void SetDlgViewPanel(I_DlgViewPanel *pIVP);

	CWnd*	GetWndTextOut();
	void SetWndTexOut(CWnd* pWnd);

protected:
	I_DlgViewPanel* m_pIVP;
	CWnd*				m_pWndTextOut;
	CDgnStlSPCSectDlg*        m_pDlg;
};

#endif // !defined(AFX_STLSPCSECTEVENTHANDLER_H__)
