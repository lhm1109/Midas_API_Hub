// CMGridModelAnalDataDlg.cpp: implementation of the CCMWindPressureBaseDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPressureBaseChildDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureBaseDlg dialog
// added by yskim  대화상자는 CCMGridModelAnalDataDlg 참고하였습니다.
// 2014-03-10 공간구조풍하중.

CCMWindPressureBaseChildDlg::CCMWindPressureBaseChildDlg(UINT nID, CWnd* pParent)
	: CChildDialog(nID, pParent)
{
	m_pParent = (CCMWindPressureBaseDlg*)pParent;
}

CCMWindPressureBaseChildDlg::~CCMWindPressureBaseChildDlg()
{
}