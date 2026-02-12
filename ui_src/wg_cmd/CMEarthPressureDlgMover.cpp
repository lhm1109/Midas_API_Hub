// CMEarthPressureDlgMover.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEarthPressureDlgMover.h"
#include "CMDlgBarBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMEarthPressureDlgMover


CCMEarthPressureDlgMover::CCMEarthPressureDlgMover(CWnd* pParent, CWnd* pBaseDlg)
{
	m_pDlgParent = NULL;
	m_pDlgParent = pParent;

	m_pBaseDlg = NULL;
	m_pBaseDlg = pBaseDlg;
}

CCMEarthPressureDlgMover::~CCMEarthPressureDlgMover()
{
	m_caCtrls.RemoveAll();
}

void CCMEarthPressureDlgMover::SetSubCtrls(const CArray<UINT, UINT>& caCtrls, UINT uiHolderID)
{
	m_caCtrls.RemoveAll();
	m_caCtrls.Copy(caCtrls);
	m_IDHolder = uiHolderID;
}

BOOL CCMEarthPressureDlgMover::IsRealWindow(CWnd* pWnd)
{
	if (pWnd == NULL)                     return FALSE;
	if (pWnd->GetSafeHwnd() == NULL)      return FALSE;
	if (!::IsWindow(pWnd->GetSafeHwnd())) return FALSE;
	return TRUE;
}

BOOL CCMEarthPressureDlgMover::MoveDlgCtrls(int nMoveVal)
{
	if (nMoveVal == 0)
		return TRUE;

	if (!IsRealWindow(m_pDlgParent))
		return FALSE;

	CWnd* pHolder = m_pDlgParent->GetDlgItem(m_IDHolder);
	if (!IsRealWindow(pHolder))
		return FALSE;

	int i, nSize;
	nSize = m_caCtrls.GetSize();
	CRect rectTemp;

	//부모 대화상자의 크기를 수정.
	m_pDlgParent->GetWindowRect(rectTemp);
	if (IsRealWindow(m_pBaseDlg))
	{ //BaseDlg가 있다면 이렇게
		int nHeight = rectTemp.Height() + nMoveVal;
		int nWidth = rectTemp.Width();
		rectTemp.top = 0;
		rectTemp.bottom = nHeight;
		rectTemp.left = 0;
		rectTemp.right = nWidth;
		m_pDlgParent->MoveWindow(rectTemp);
		((CCMDlgBarBase*)m_pBaseDlg)->LayoutAdjust();
	}
	else
	{ //없으면 기본 루틴.
		rectTemp.bottom += nMoveVal;
		m_pDlgParent->MoveWindow(rectTemp);

	}

	//부모대화상자의 PlaceHolder의 크기를 수정.
	pHolder->GetWindowRect(rectTemp);
	m_pDlgParent->ScreenToClient(rectTemp);
	rectTemp.bottom += nMoveVal;
	pHolder->MoveWindow(rectTemp);

	//부모대화상자의 Place Holder아래의 Ctrl의 위치를 수정.
	for (i = 0; i < nSize; i++)
	{ //여기는 이동만 한다.
		CWnd* pCtrl = m_pDlgParent->GetDlgItem(m_caCtrls[i]);
		if (!IsRealWindow(pCtrl)) continue;

		pCtrl->GetWindowRect(rectTemp);
		m_pDlgParent->ScreenToClient(rectTemp);
		rectTemp.top += nMoveVal;
		rectTemp.bottom += nMoveVal;
		pCtrl->MoveWindow(rectTemp);
		pCtrl->RedrawWindow();
	}

	pHolder->RedrawWindow();
	m_pDlgParent->RedrawWindow();

	return TRUE;
}

BOOL CCMEarthPressureDlgMover::Execute(CWnd* pNewSubDlg)
{
	if (!IsRealWindow(pNewSubDlg))
		return FALSE;

	CWnd* pHolder = m_pDlgParent->GetDlgItem(m_IDHolder);

	if (!IsRealWindow(pHolder))
		return FALSE;

	CRect rectHolder, rectNewDlg;
	pHolder->GetWindowRect(rectHolder);
	pNewSubDlg->GetWindowRect(rectNewDlg);

	int nDiff = rectNewDlg.Height() - rectHolder.Height();

	if (!MoveDlgCtrls(nDiff))
		return FALSE;

	return TRUE;
}
