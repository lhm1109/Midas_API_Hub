// StlSPCSectEventHandler.cpp: implementation of the CStlSPCSectEventHandler class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "StlSPCSectEventHandler.h"

#include "DgnStlSPCSectDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStlSPCSectEventHandler::CStlSPCSectEventHandler(CWnd* pDlg /*=NULL*/)
{
	m_pIVP = NULL;
	m_pWndTextOut = NULL;
	m_pDlg = (CDgnStlSPCSectDlg*)pDlg;
}

CStlSPCSectEventHandler::~CStlSPCSectEventHandler()
{

}

void CStlSPCSectEventHandler::Initialize()
{

}

void  CStlSPCSectEventHandler::On_RButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV)
{
	if(NULL != m_pIVP)
	{
		DlgView_Object* pObject = m_pIVP->FindObjectByCliPos(point);

		if(NULL != pObject)
		{
			CString* pStr = (CString*)pObject->m_pUserData;
			if(NULL != pStr)
			{
				if(NULL != m_pWndTextOut)
				//	m_pWndTextOut->SetWindowText(*pStr);
				GetWndTextOut()->SetWindowText(*pStr);
			}
		}
	}	
}

void  CStlSPCSectEventHandler::On_LButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV)
{
	if(NULL != m_pIVP && NULL != m_pDlg)
	{
		double x, y, z = 0.0;
		if(((I_GViewWnd*)pVW)->ScreenToWorld(point.x, point.y, x, y, z))
			m_pDlg->SetViewLButtonDown(x, y);
	}	
}

I_DlgViewPanel* CStlSPCSectEventHandler::GetDlgViewPanel()
{
	return m_pIVP;
}

void CStlSPCSectEventHandler::SetDlgViewPanel(I_DlgViewPanel *pIVP)
{
	m_pIVP = pIVP;
}

CWnd*	CStlSPCSectEventHandler::GetWndTextOut()
{
	return m_pWndTextOut;
}

void CStlSPCSectEventHandler::SetWndTexOut(CWnd* pWnd)
{
	m_pWndTextOut = pWnd;
}