#pragma once

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MillustViewer.h"

// CPGCurvGuideImageDlg 대화 상자입니다.

class CCPGCurvGuideImageDlg : public CDialogMove
{
	DECLARE_DYNAMIC(CCPGCurvGuideImageDlg)

public:
	CCPGCurvGuideImageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCPGCurvGuideImageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_CPG_CPCB_CURVE_GUIDE_IMAGE_SHOW };

private:
	mit::frx::MillustViewer m_wndPicture;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
