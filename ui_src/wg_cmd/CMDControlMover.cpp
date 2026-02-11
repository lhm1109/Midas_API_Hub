#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDControlMover.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDControlMover

CCMDControlMover::CCMDControlMover(CWnd* pParent /*=NULL*/)
{
	m_pParent = NULL;//부모 윈도우 초기화.

	if(pParent == NULL)
	{
		ASSERT(0);
	}

	HWND hwndParent = pParent->GetSafeHwnd();

	if( hwndParent && ::IsWindow(hwndParent) )
	{
		m_bParentOK = TRUE; //부모 윈도우가 정상적일때만...
		m_pParent = pParent;
		m_pParent->GetWindowRect(m_rectParentPre);
		RemoveAll();
	}
}

CCMDControlMover::~CCMDControlMover()
{

}

void CCMDControlMover::AddCtrl2Move( UINT uiCtrlId, int nVertMove, int nHorzMove, int nVertSize, int nHorzSize)
{
	CM_CTRL_MOVER_RATIO moverRatio;
	moverRatio.nVertMove = nVertMove;
	moverRatio.nHorzMove = nHorzMove;
	moverRatio.nVertSize = nVertSize;
	moverRatio.nHorzSize = nHorzSize;

	AddCtrl2Move(uiCtrlId, moverRatio);  
}

void CCMDControlMover::AddCtrl2Move( UINT uiCtrlId, const CM_CTRL_MOVER_RATIO& Ratio)
{
	BOOL bNotExist = TRUE;
	int i;
	for(i = 0 ; i < m_caCtrlID.GetSize(); i++)
	{
		if(m_caCtrlID[i] == uiCtrlId)
		{
			bNotExist = FALSE;
		}
	}

	if(bNotExist)
	{
		m_caCtrlID.Add(uiCtrlId);
		m_caCtrlRatio.Add(Ratio);
	}
}

void CCMDControlMover::RemoveAll()
{
	m_caCtrlID.RemoveAll();
	m_caCtrlRatio.RemoveAll();
}

void CCMDControlMover::OnSizeChanged()
{
	if(!m_bParentOK)
		return;
	if(m_caCtrlID.GetSize() == 0)
		return;
	if(m_caCtrlID.GetSize() != m_caCtrlRatio.GetSize())
		return;

	long lSize = m_caCtrlRatio.GetSize();

	int i;
	CRect rectParent;
	m_pParent->GetWindowRect(rectParent);

	long lChangeVert = rectParent.Height() - m_rectParentPre.Height();
	long lChangeHorz = rectParent.Width() - m_rectParentPre.Width();

	for(i = 0 ; i < lSize; i++)
	{
		MoveCtrl(i, lChangeVert, lChangeHorz);
	}

	m_rectParentPre = rectParent;
}

void CCMDControlMover::MoveCtrl( int nIdx, long lChangeVert, long lChangeHorz )
{
	if(nIdx < 0)
		return;
	if(m_caCtrlRatio.GetSize() != m_caCtrlID.GetSize())
		return;
	if(nIdx >= m_caCtrlID.GetSize())
		return;

	CWnd* pControl = m_pParent->GetDlgItem(m_caCtrlID[nIdx]);  
	const cmRatio& myRatio = m_caCtrlRatio[nIdx];

	CRect rectControl;
	pControl->GetWindowRect(rectControl);
	m_pParent->ScreenToClient(rectControl);
	
	int nHeight = rectControl.Height();
	int nWidth = rectControl.Width();
	rectControl.left = rectControl.left + (lChangeHorz * myRatio.nHorzMove / 100);
	rectControl.top = rectControl.top + (lChangeVert * myRatio.nVertMove / 100);
	rectControl.right = rectControl.left + nWidth + (lChangeHorz * myRatio.nHorzSize / 100);
	rectControl.bottom = rectControl.top + nHeight + (lChangeVert * myRatio.nVertSize / 100);  

	pControl->MoveWindow(rectControl);
}
