#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStorAddDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\FlagCtrl.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\StorSubData.h"


CCMStorAddDlg::CCMStorAddDlg(CWnd* pParent /*= NULL*/)
	: CDialogMove(CCMStorAddDlg::IDD, pParent)
{
	
	m_pDoc = CDBDoc::GetDocPoint();
}

CCMStorAddDlg::~CCMStorAddDlg(void)
{
}


BEGIN_MESSAGE_MAP(CCMStorAddDlg, CDialogMove)
END_MESSAGE_MAP()

void CCMStorAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAED_UI_NEW_STORY_BY_HEIGHT_NUM_ADDED_STORY_EDT, m_edtNumAdded);
	DDX_Control(pDX, IDC_CAED_UI_NEW_STORY_BY_HEIGHT_HEIGHT_EDT, m_edtNumHeight);
	DDX_Control(pDX, IDC_CAED_UI_NEW_STORY_BY_HEIGHT_HEIGHT_UNIT_STC, m_stHeight);
}



BOOL CCMStorAddDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	InitCtrl();

	return TRUE;
}

void CCMStorAddDlg::InitCtrl()
{
	m_edtNumHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_stHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);

}


void CCMStorAddDlg::OnOK()
{
	CWaitCursor wc;

	if (AddStory())
	{
		CStorSubData::GetInstance().AddAllStorKeyForLineGridIfCheckedAll();
		CStorSubData::GetInstance().SetNeedUpdateStor(true);
		CDialogMove::OnOK();
	}
}

BOOL CCMStorAddDlg::AddStory()
{
	// Story DB에 Story 추가하는 함수, Table 업데이트 작업은 Table에서 담당
	int nNumStory = 0;
	double dToler = CMathFunc::Get_NormalZero();

	nNumStory = m_edtNumAdded.GetEditValue();
	double dHeight = m_edtNumHeight.GetEditValue();

	if (nNumStory < 1)
	{
		nsMsgOutError(_LS(IDS_CAED_DB_STORY_UTIL_CHECK_NUM_STORY));
		return FALSE;
	}

	const double dTol = m_pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit();
	if (dHeight < dTol)
	{
		nsMsgOutError(_LS(IDS_CAED_DB_STORY_UTIL_CHECK_STORY_HEIGHT));
		return FALSE;
	}
	//  Roof 층 자동 생성. (층이 없는 story set 에 층을 추가할 때만 여기로 들어온다.)
	double dLevelCurrent = m_pDoc->m_pAttrCtrl->GetBaseLevel();
	CArray<T_STOR_D, T_STOR_D&> aAddStorD;
	for (int i = 0; i <= nNumStory; ++i)
	{
		T_STOR_D Story;
		//Story.Key = pPoolStory->GetNewKey();

		if (i == nNumStory)
			Story.StoryName = _LS(IDS_WG_CMD__ADDD__Roof);
		else
			Story.StoryName.Format(_T("%dF"), i+1);

		Story.dStoryLevel = dLevelCurrent;
		Story.bFloorDiaphragm = FALSE;


		aAddStorD.Add(Story);

		dLevelCurrent += dHeight;
		if (fabs(dLevelCurrent) < dToler) dLevelCurrent = 0.; // 수치오류로 인해 0에 (-) 붙는 경우 방지하기 위함.
	}

	if (aAddStorD.GetSize() > 0)
	{
		if (!m_pDoc->m_pDataCtrl->AddStor(aAddStorD, TRUE))  return FALSE;
	}
	return TRUE;
}
