// ConcSectionRebarEventHandler.cpp: implementation of the ConcSectionRebarEventHandler class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionRebarEventHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConcSectionRebarEventHandler::ConcSectionRebarEventHandler()
{
	m_pIVP = NULL;
	m_pWndTextOut = NULL;
}

ConcSectionRebarEventHandler::~ConcSectionRebarEventHandler()
{

}

void ConcSectionRebarEventHandler::Initialize()
{

}

void  ConcSectionRebarEventHandler::On_RButtonDown  (CWnd* pVW, UINT nFlags, CPoint point,BOOL & bProcessDefEV)
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

I_DlgViewPanel* ConcSectionRebarEventHandler::GetDlgViewPanel()
{
	return m_pIVP;
}

void ConcSectionRebarEventHandler::SetDlgViewPanel(I_DlgViewPanel *pIVP)
{
	m_pIVP = pIVP;
}

CWnd*	ConcSectionRebarEventHandler::GetWndTextOut()
{
	return m_pWndTextOut;
}

void ConcSectionRebarEventHandler::SetWndTexOut(CWnd* pWnd)
{
	m_pWndTextOut = pWnd;
}