// PGCurvGuideImageDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGCurvGuideImageDlg.h"

// CPGCurvGuideImageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCPGCurvGuideImageDlg, CDialogMove)

CCPGCurvGuideImageDlg::CCPGCurvGuideImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCPGCurvGuideImageDlg::IDD, pParent)
{

}

CCPGCurvGuideImageDlg::~CCPGCurvGuideImageDlg()
{
}

void CCPGCurvGuideImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_wndPicture);
}

BOOL CCPGCurvGuideImageDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\Convex_Concave_Guide_Image.svg"));
	m_wndPicture.SetIgnoreLargeView();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCPGCurvGuideImageDlg, CDialogMove)
END_MESSAGE_MAP()


// CPGCurvGuideImageDlg 메시지 처리기입니다.
