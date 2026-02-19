// ConcSectionRebarEventHandler.h: interface for the FCMTendonEventHandler class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONCSECTIONREBAREVENTHANDLER_H__F8690BD9_16BE_4DDD_9F7C_62F04F29DC22__INCLUDED_)
#define AFX_CONCSECTIONREBAREVENTHANDLER_H__F8690BD9_16BE_4DDD_9F7C_62F04F29DC22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"

class ConcSectionRebarEventHandler  : public I_DlgView_EventHandler
{
public:
	ConcSectionRebarEventHandler();
	virtual ~ConcSectionRebarEventHandler();

	virtual void Initialize();

	virtual void  On_RButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV);

	I_DlgViewPanel* GetDlgViewPanel();
	void SetDlgViewPanel(I_DlgViewPanel *pIVP);

	CWnd*	GetWndTextOut();
	void SetWndTexOut(CWnd* pWnd);

protected:
	I_DlgViewPanel* m_pIVP;
	CWnd*				m_pWndTextOut;
};

#endif // !defined(AFX_FCMTENDONEVENTHANDLER_H__F8690BD9_16BE_4DDD_9F7C_62F04F29DC22__INCLUDED_)
