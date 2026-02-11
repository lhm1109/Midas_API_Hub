// CMDTendonTemplateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateDlg.h"
#include "CMDTendonTemplateExportDlg.h"
#include "CMDTendonTemplateCopyDlg.h"
#include "CMDTendonTemplateAddModDlg.h"
#include "CMDTendonTemplateSetPropDlg.h"
#include "CMDTendonTemplateAutoGenDlg.h"
#include "CMDTendonTemplateAssignElem.h"
#include "CMDControlMover.h"
#include "DgnTendonAutoGenCtrl.h"
#include "DrawEditBtn_TndnTemplate.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/ViewCtrl.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/DBLib.h"
#include "../wg_db/DataCtrl.h"
#include "../wg_db/UndoCtrl.h"
#include "../wg_db/EditData.h"

#include "../wg_base/I_GENModelBase.h"
#include "../wg_base/wg_base_StrParser.h"
#include "../wg_base/DlgUtil.h"
#include "../wg_base/wg_base_MsgDll.h"



#include <windowsx.h>

#define DWORD_TREE_YZ 0
#define DWORD_TREE_XZ 999999999
#define DWORD_TREE_XY 999999998
#define IS_TREE_ELEM(input)	(input < DWORD_TREE_XY && input > DWORD_TREE_YZ) ? TRUE:FALSE
#define IS_TREE_XZ(input)	(input == DWORD_TREE_XZ) ? TRUE:FALSE
#define IS_TREE_XY(input)	(input == DWORD_TREE_XY) ? TRUE:FALSE
#define STR_PREFIXNAME _T("strand")

#define IS_DUPLICATED_VEC(vecName, dupVal)  {\
	int loopCnt; BOOL bDupl = FALSE; int nSizeVec = vecName.size(); \
	for ( loopCnt = 0 ; loopCnt< nSizeVec; loopCnt++) { if(vecName[k] == dupVal) bDupl = TRUE; } return bDupl; }

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateDlg::CCMDTendonTemplateDlg(CWnd* pParent /*=NULL*/)
: CCMDlgBase(CCMDTendonTemplateDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pAttrCtrl = m_pDoc->m_pAttrCtrl;
	m_bMinimized = FALSE;

	m_pTemplateCtrl	= new CDgnTendonTemplateCtrl(m_pDoc);
	m_pDraw = new CDrawEditBtn_TndnTemplate(m_pTemplateCtrl);
	m_bIgnoreMsg			  = FALSE;
	m_bOnChangeElemList	= FALSE;
	m_bIsShowWnd        = TRUE;
	m_bAfter            = TRUE;
	m_bUseAfxMsgBox     = TRUE;
	m_pCopyDlg			    = NULL;
	m_pAutoGenDlg       = NULL;
	m_pControlMover     = NULL;
	m_hTreeItemSection  = NULL;
	m_nType				= 0;

	m_arKeyStr.RemoveAll();
}

CCMDTendonTemplateDlg::~CCMDTendonTemplateDlg()
{
	if(m_pTemplateCtrl)
	{
		delete m_pTemplateCtrl;
		m_pTemplateCtrl = NULL;
	}
 
	if(m_pDraw)
	{
		delete m_pDraw;
		m_pDraw = NULL;
	}

	if(m_pControlMover)
	{
		delete m_pControlMover;
		m_pControlMover = NULL;
	}

	DeleteChildDlg(EN_DLG_TYPE_ALL);
}

BEGIN_MESSAGE_MAP(CCMDTendonTemplateDlg, CCMDlgBase)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADD,				   OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_MOD,				   OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_COPY,				   OnCmdCopy)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_DEL,				   OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_IMPORT,			   OnCmdImport)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_EXPORT,			   OnCmdExport)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_TUNNING,			 OnCmdAutoGen)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SET_PROP,			 OnCmdSetProperty)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_RESETNAME,     OnCmdResetName)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATEDLG_USE_PREFIX, OnCmdUsePrefix)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_APPLY,         OnCmdApply)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN,		 OnCmdAssignElemList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TENDON_TEMPLATE_TREE,		    OnDbClickTreeCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TENDON_TEMPLATE_LIST,		    OnCmdModifyDbClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_TENDON_TEMPLATE_LIST,  OnItemchangedList)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_RDO_END_I,			    OnRdoClicked)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_RDO_END_J,			    OnRdoClicked)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL,	    OnCmdAssignElemDetail)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ADD_ASSIGN,   	    OnCmdAssignElemAdd)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN_CBX, OnAssignElemCbxChanged)  
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CCMDTendonTemplateDlg::DoDataExchange( CDataExchange* pDX )
{
	CCMDlgBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN,       m_wndAssignElem);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_LIST,              m_wndTendonList);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATEDLG_USE_PREFIX,     m_btnUsePrefix);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_PREFIXNAME,        m_edtPrefixName);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_TREE,              m_ctlTree);	
 	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_RDO_END_I,         m_rdoEndI);	
 	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_RDO_END_J,         m_rdoEndJ);		
 	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN_CBX,   m_cbxAssignElem);		
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_DRAW_BTN, *m_pDraw);
}

BOOL CCMDTendonTemplateDlg::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();
	SetIgnoreBottomBtnArea();

	MakeListHeader();
	UpdateTdnaList();	
	
	SetControl();
	SetMover(); //Control Mover 세팅.

	return FALSE;
	
}

void CCMDTendonTemplateDlg::UpdateBuffer()
{
	std::vector<T_ELEM_K> vecElemKey;
	std::vector<T_ELEM_K>* pVecElemKEy = m_pTemplateCtrl->GetAssignElemList();

	vecElemKey.clear();
	vecElemKey.insert(vecElemKey.begin(), pVecElemKEy->begin(), pVecElemKEy->end());
		
	m_pTemplateCtrl->SetAssignElemList(vecElemKey);	
}

void CCMDTendonTemplateDlg::OnUpdate( CWnd* pSender, LPARAM lHint, CObject* pHint )
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
		// 방식 변경:후처리 모드에서 Modify하는 경우 전처리로 변경되면서 에러 야기
		DestroyWindow();
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}	
}

void CCMDTendonTemplateDlg::PostNcDestroy()
{
	SetShowWnd(FALSE);
	CCMDlgBase::PostNcDestroy();
}

void CCMDTendonTemplateDlg::OnOK()
{
	if (!DoApply()) return;
	UnselectAll();
	DestroyWindow();
}

void CCMDTendonTemplateDlg::OnCancel()
{
	UnselectAll();
	DestroyWindow();
}

BOOL CCMDTendonTemplateDlg::DoApply()
{
	if(!Dlg2Data())
		return FALSE;

	int i, j, nSizeFor;
	int nSize;

	
	vecTndnKey keyList;
	nSize = m_pTemplateCtrl->GetKeyList(keyList);
	if(nSize <= 0)
	{
		return FALSE;
	}

	nSize = m_arKeyStr.GetSize();
	if(nSize <= 0)
	{
		//요소 선택 콤보박스에 추가된 것이 없다면, 현재 선택된 요소들을 추가한다.
		//현재 선택한 요소를 추가하려 시도한다....

		CString strTemp;
		m_wndAssignElem.GetWindowText(strTemp);
		if(strTemp.IsEmpty())
		{
			ASSERT(0);
			return FALSE;
		}

		m_bUseAfxMsgBox = FALSE;
		OnCmdAssignElemAdd();
		m_bUseAfxMsgBox = TRUE;
		nSize = m_arKeyStr.GetSize();
	}
	
	CArray<int, int> aElem;

	if(!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_TDNA)), CMDTYPE_REMOVE_ANALYSIS))
			return FALSE;

	BOOL bFailExist = FALSE;
	BOOL bCurveFail = FALSE;

	for(i = 0 ; i < nSize; i++)
	{
		if(i != 0)
			OnResetName(FALSE);

		if(!CStrParser::ParsingListByTo(m_arKeyStr[i], aElem))
			continue;

		nSizeFor = aElem.GetSize();

		std::vector<T_ELEM_K> vElemKey;
		vElemKey.resize(nSizeFor);
		for(j = 0 ; j < nSizeFor; j++)
			vElemKey[j] = aElem[j];

		m_pTemplateCtrl->SetAssignElemList(vElemKey);
		m_pTemplateCtrl->OnchangeTotalLength();

		vecTndnKey keyList;
		m_pTemplateCtrl->GetKeyList(keyList);
		
		
		itrKey itrK;
		for(itrK = keyList.begin(); itrK != keyList.end(); itrK++)
		{	
			int nkey = *itrK;
			TndnTemplate tempInfo;
			
			if(!m_pTemplateCtrl->GetTemplateNU(nkey, tempInfo))
			{
				bFailExist = TRUE;
				continue;
			}
			
			T_TDNA_D tTdna;
			if(tempInfo.bCurveTypeOut && m_pTemplateCtrl->IsCurveElem())
			{
				if(!m_pTemplateCtrl->GetTdnaCurveType(tempInfo, tTdna, TRUE))
				{
					//실패하는 조건을 출력하고. 사용자에게 수정하도록 유도한다.
					bFailExist = TRUE;
					bCurveFail = TRUE;
					continue;
				}
			}
			else
			{
				if(!m_pTemplateCtrl->GetTdnaElemType(tempInfo, tTdna, TRUE))
				{
					//실패하는 조건을 출력하고. 사용자에게 수정하도록 유도한다.
					bFailExist = TRUE;
					continue;
				}
			}
			
			if(m_pAttrCtrl->ExistTdna(tTdna.TendonName))
			{ //존재하면 실패.
				GSaveHistoryFormatNF(_LS(IDS_DB_DATA_EXIST_NAME), _LS(IDS_DB_DT_TDNA), tTdna.TendonName);
				return EndEdit(FALSE);
			}
			
			if(!m_pDoc->m_pEditData->AddTdna(tTdna))
				return EndEdit(FALSE); //실패..
		}
	}

	if(bFailExist)
	{
		if(bCurveFail)
			AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_CURVE_TEMPLATE_FAIL));

		if(m_arKeyStr.GetSize() == 1)
		{
			m_arKeyStr.RemoveAll();
			RefreshAssignElemCombo();
		}

		return EndEdit(FALSE); //실패..
	}
	
	
	//선택된 요소를 해제한다.
	m_arKeyStr.RemoveAll();
	RefreshAssignElemCombo();
	return EndEdit(TRUE, TRUE, LT_TDNA_CMD);
}

BOOL CCMDTendonTemplateDlg::Dlg2Data()
{
	// 텐던 템플릿 중복성 검사.
	vecTndnKey tndnKeyList;

	BOOL bExistSameTemplate = FALSE;
	int i, j;
	int nSize = m_pTemplateCtrl->GetKeyList(tndnKeyList);

	TndnTemplate TempInfo;
	TndnTemplate TempInfoCmp;  
	BOOL bDupl;
	int nDuplCounter = 1;

	CString strTemp;
	CString strTemp2;
	CString strErrMsg;
	map<int, int> mapTemplateDupl;
	mapTemplateDupl.clear();

	for(i = 0 ; i < nSize - 1; i++)
	{
		if(!m_pTemplateCtrl->GetTemplateNU(tndnKeyList[i], TempInfo))
			return FALSE;
		
		bDupl = FALSE;
		for(j = i + 1 ; j < nSize; j++)
		{
			if(mapTemplateDupl.find(j) != mapTemplateDupl.end()) //뭔가가 있으면 이미 체크한것.
				continue;

			if(!m_pTemplateCtrl->GetTemplateNU(tndnKeyList[j], TempInfoCmp))
				return FALSE;

			if( TempInfo == TempInfoCmp )
			{
				mapTemplateDupl[j] = nDuplCounter;
				bDupl = TRUE;
			}
		}
		
		if(bDupl) mapTemplateDupl[i] = nDuplCounter++;
	}
	
	if(nDuplCounter != 1)
	{
		strErrMsg.Format(_T("%s\n"), _LS(IDS_CMD_TNDN_TEMPLATE_MSG_SAME_TEMPLATE_EXIST));
		map<int, int>::iterator itr;
		for(i = 1 ; i < nDuplCounter; i++)
		{
			strTemp.Empty();
			for(itr = mapTemplateDupl.begin(); itr != mapTemplateDupl.end(); itr++)
			{
				if(itr->second == i)
				{
					if(m_pTemplateCtrl->GetTemplateNU(tndnKeyList[itr->first], TempInfo))
					{
						if(strTemp.IsEmpty())             //strTemp2.Format(_T("%d. %s"), itr->first+1, TempInfo.strName);
							strTemp2.Format(_T("[%s"), TempInfo.strName);
						else             //strTemp2.Format(_T(", %d. %s"), itr->first+1, TempInfo.strName);
							strTemp2.Format(_T(", %s"), TempInfo.strName);
						
						AppendStr(strTemp, strTemp2);
					}
					else
						ASSERT(0);
				}
			}

			if(!strTemp.IsEmpty())
			{
				strTemp2.Format(_T("]\n"));
				AppendStr(strTemp, strTemp2);
				AppendStr(strErrMsg, strTemp);
			}
		}
		AfxMessageBox(strErrMsg);
		return FALSE;
	}
	// 텐던 템플릿 중복성 검사 종료..

	if(!m_pTemplateCtrl->IsValidElem(FALSE))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		return FALSE;
	}

	return TRUE;
}

BOOL CCMDTendonTemplateDlg::Data2Dlg()
{
	return TRUE;
}

void CCMDTendonTemplateDlg::UnselectAll()
{
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
	if (!I_GENModelBase::GetCurMySelfST()) return;
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	I_GENModelBase::GetCurMySelfST()->ViewInvalidate(FALSE);
}

void CCMDTendonTemplateDlg::MakeListHeader()
{
	CArray<float, float> fHRatio;
	fHRatio.Add(0.1f);
	fHRatio.Add(0.5f);
	fHRatio.Add(0.4f);

	if(m_wndTendonList.GetHeaderCtrl()->GetItemCount() == 0)
	{
		CStringArray         HTitles;	
		HTitles.Add(_LS(IDS_CMD_MVHL_NO));
		HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Name));
		HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Property));
		
		CDlgUtil::_SetListCtrlHeader(&m_wndTendonList, HTitles, &fHRatio, NULL);
	}

	CArray<int,int> HWidths;
	CRect Rect;
	int i, nSize, nCx, nSumSize;

	const int nSizeNo = 40;
	nSumSize = 0;

	m_wndTendonList.GetWindowRect(&Rect);
	
	nCx = Rect.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - 5;
	nSize = fHRatio.GetSize();

	HWidths.SetSize(nSize);
	for(i = 0 ; i < nSize; i++)
	{
		if(i == 0)              HWidths[i] = nSizeNo;
		else if(i == nSize - 1) HWidths[i] = nCx - nSumSize;
		else                    HWidths[i] = nCx * fHRatio[i];

		nSumSize += HWidths[i];
	}

	for(i = 0 ; i < fHRatio.GetSize() ; i++)
	{
		m_wndTendonList.SetColumnWidth(i, HWidths[i]);
	}
}

void CCMDTendonTemplateDlg::UpdateTdnaList()
{
	I_GENModelBase::UTIL_DeleteAllDisplayObjST();
	m_wndTendonList.DeleteAllItems();
	
	CStringArray Contents;
	CString StrTemp;
	
	std::vector<int> keyList;
	
	int i;
	int Index= 0;
	int nCount = m_pTemplateCtrl->GetKeyList(keyList);

	for(i = 0; i < nCount; i++)
	{
		TndnTemplate tndnTemp;
		int nKey = keyList[i];
		if(m_pTemplateCtrl->GetTemplateNU(nKey, tndnTemp))
		{
			Contents.RemoveAll();
			StrTemp.Format(_T("%d"), i + 1);
			Contents.Add(StrTemp);
			Contents.Add(tndnTemp.strName);
			
			T_TDNT_D TdntD;
			if(m_pAttrCtrl->GetTdnt(tndnTemp.keyProperty, TdntD))
				Contents.Add(TdntD.TendonTypeName);
			else
				Contents.Add(_T(""));
			
			CDlgUtil::SetListItem(&m_wndTendonList, Index, Contents, keyList[i]);
			Index++;
		}
	}
}

void CCMDTendonTemplateDlg::OnCmdAssignElemList()
{
	if(m_bOnChangeElemList)
		return;

	m_bOnChangeElemList = TRUE;
	CString StrText;
	m_wndAssignElem.GetWindowText(StrText);

	CArray<int, int> aElem;

	if(CStrParser::ParsingListByTo(StrText, aElem))
	{
		std::vector<T_ELEM_K> vecElemKey;
		int nElem = aElem.GetSize();
		vecElemKey.resize(nElem);
		for(int i = 0; i < nElem; i++)
		{
			vecElemKey[i] = aElem[i];
		}

		m_pTemplateCtrl->SetAssignElemList( vecElemKey );

		if(m_pTemplateCtrl->IsValidElem(FALSE))
		{
			m_pTemplateCtrl->OnchangeTotalLength();
			if(nElem > 0)
			{
				m_pDraw->SetTdnaData();
				
				SetTreeCtrl();
				
				NMHDR pNMHDR;
				LRESULT pResult;
				OnDbClickTreeCtrl(&pNMHDR, &pResult);

			}
			if(m_pTemplateCtrl->IsAllSameSect())
			{
				GetDlgItem(IDC_CMD_TENDON_TEMPLATE_TUNNING)->EnableWindow(TRUE);        
				GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADD_ASSIGN)->EnableWindow(TRUE); //elem add
			}
			else
			{
				GetDlgItem(IDC_CMD_TENDON_TEMPLATE_TUNNING)->EnableWindow(FALSE);
				GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ADD_ASSIGN)->EnableWindow(FALSE); //elem add
			}
		}
	}

	m_bOnChangeElemList = FALSE;
}

void CCMDTendonTemplateDlg::OnOkSubDlgADDMOD(TndnTemplate& tndnData, int nKey/* = -1*/)
{
	if(nKey >= 0) //Modify
	{
		m_pTemplateCtrl->ModTemplate(nKey, tndnData);
		UpdateListDraw(FALSE);
	}
	else //Add
	{
		m_pTemplateCtrl->AddTemplate(tndnData);
		UpdateListDraw();
	}  
}

void CCMDTendonTemplateDlg::OnCmdAdd()
{
	if(!m_pTemplateCtrl->IsValidElem(FALSE))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		return;
	}
	
	DeleteChildDlg();
	
	CCMDTendonTemplateAddModDlg* pDlg = new CCMDTendonTemplateAddModDlg(m_pTemplateCtrl, this);
	pDlg->m_nKey = -1;
	
	CString wndString;
	m_edtPrefixName.GetWindowText(wndString);
	TndnTemplate* tempInfo = pDlg->GetTemplateDate();
	m_pTemplateCtrl->GetNewName(wndString, tempInfo->strName);
	m_pTemplateCtrl->GetDefaultSpanValue((*tempInfo), TRUE, 0, TRUE, TRUE);
	m_pTemplateCtrl->GetDefaultSpanValue((*tempInfo), FALSE, 0, TRUE, TRUE);

	BOOL bUseSpanInfo = TRUE;
	if(m_pTemplateCtrl->IsExistSpanInfo() && bUseSpanInfo)
	{
		tempInfo->bSameSpanInterval = FALSE;
		tempInfo->tndnSubXY.bSameSpanInfo = FALSE;
		tempInfo->tndnSubXZ.bSameSpanInfo = FALSE;
		m_pTemplateCtrl->GetInitSpanInfo(*tempInfo);
	}
	
	SetShowWnd(FALSE);
	ShowWindow(SW_HIDE);  
	pDlg->Create(CCMDTendonTemplateAddModDlg::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
}

void CCMDTendonTemplateDlg::OnCmdModify()
{
	if(!m_pTemplateCtrl->IsValidElem(FALSE))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		return;
	}

	CArray<DWORD, DWORD> arRet;
	CDlgUtil::GetSelectedListItemData(&m_wndTendonList, arRet);

	if(arRet.GetSize() == 1)
	{
		DeleteChildDlg();
		int nKey = (int)arRet[0];
		TndnTemplate tndnTemp;
		if(!m_pTemplateCtrl->GetTemplate(nKey, tndnTemp))
		{
			ASSERT(0);
		}

		CCMDTendonTemplateAddModDlg* pDlg = new CCMDTendonTemplateAddModDlg(m_pTemplateCtrl, this);
		pDlg->m_nKey = nKey;
		pDlg->SetTemplateData(&tndnTemp);
		SetShowWnd(FALSE);
		ShowWindow(SW_HIDE);  
		pDlg->Create(CCMDTendonTemplateAddModDlg::IDD, this);
		pDlg->ShowWindow(SW_SHOW);
	}	
}

void CCMDTendonTemplateDlg::OnCmdCopy()
{
	if(!m_pTemplateCtrl->IsValidElem(FALSE))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		return;
	}

	ShowHideChildDlg(EN_DLG_TYPE_COPY);
	return;
}

void CCMDTendonTemplateDlg::OnCmdDelete()
{	
	vecTndnKey keyList;
	GetSelectedKeyList(keyList);

	int nRet = (int)keyList.size();
	if(nRet == 0) return;
	
	itrKey itr;

	for(itr = keyList.begin(); itr != keyList.end(); itr++)
	{
		m_pTemplateCtrl->DelTemplate(*itr);
	}

	UpdateListDraw(FALSE);
}

void CCMDTendonTemplateDlg::OnCmdImport()
{	
	CString csFileName = _T("");
	LPCTSTR szFileName;
	szFileName = csFileName;
	
	CFileDialog fd(TRUE, _T(".tdt"), szFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
		_T("Tendon Template TDT File(*.tdt)|*.tdt|All File(*.*)|*.*||"), NULL);
	
	BOOL bReplace = FALSE;

	if(fd.DoModal() == IDOK)
	{
		CString filePath = fd.GetPathName();
		m_pTemplateCtrl->Import(filePath.GetBuffer(filePath.GetLength()), bReplace);
		UpdateListDraw();
	}
}

void CCMDTendonTemplateDlg::OnCmdExport()
{
	vecTndnKey keyList;
	GetSelectedKeyList(keyList);

	CCMDTendonTemplateExportDlg dlg(GetTemplateCtrl(), this);
	dlg.SetSelectedKeyList(keyList, TRUE);
	dlg.DoModal();
}

void CCMDTendonTemplateDlg::OnCmdAutoGen()
{
	if(!m_pTemplateCtrl->IsValidElem(FALSE))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		return;
	}

	HTREEITEM hItem = m_ctlTree.GetSelectedItem();
	BOOL      bShowSection = TRUE;
	if(hItem)
	{
		DWORD dWrd = m_ctlTree.GetItemData(hItem);
		if(IS_TREE_ELEM(dWrd)) //이미 단면이 선택되어 있다면 아무것도 안함.
		{
			bShowSection = FALSE;
		}
	}
	if(bShowSection)
	{
		if(m_hTreeItemSection)
		{
			m_ctlTree.Select(m_hTreeItemSection, TVGN_CARET);
			NMHDR pNMHDR;
			LRESULT pResult;
			OnDbClickTreeCtrl(&pNMHDR, &pResult);
		}
		else ASSERT(0);
	}

	ShowHideChildDlg(EN_DLG_TYPE_AUTOGEN);
	return;

}

void CCMDTendonTemplateDlg::OnCmdSetProperty()
{
	vecTndnKey keyList;
	GetSelectedKeyList(keyList);
	
	CCMDTendonTemplateSetPropDlg dlg(GetTemplateCtrl(), this);
	dlg.SetSelectedKeyList(keyList, TRUE);
	if(dlg.DoModal() == IDOK)
	{
		UpdateListDraw();
	}
}

void CCMDTendonTemplateDlg::OnCmdResetName()
{
	OnResetName(TRUE);
}

void CCMDTendonTemplateDlg::OnCmdApply()
{
	if (!DoApply()) return;
	OnResetName(TRUE);
}

int CCMDTendonTemplateDlg::GetSelectedKeyList( vecTndnKey& keyList )
{
	keyList.clear();
	CArray<int, int> aSelectedItem;

	CDlgUtil::GetListItemByMask(&m_wndTendonList, aSelectedItem, LVIS_SELECTED);
	
	int nNum = aSelectedItem.GetSize();
	keyList.resize(nNum);
	
	for (int i = 0; i < nNum; i++)
	{
		int nKey = (int)m_wndTendonList.GetItemData(aSelectedItem[i]);
		keyList[i] = nKey;
	}
	return nNum;
}

void CCMDTendonTemplateDlg::SetControl()
{
	m_btnUsePrefix.SetCheck(TRUE);
	m_bUsePrefix = TRUE;

	SetTreeCtrl();
	
	CString strTemp = STR_PREFIXNAME;
	m_edtPrefixName.SetWindowText(strTemp);
	
	m_pDraw->SetParent(this);
	m_pDraw->SetHasList(TRUE);
	m_pDraw->Init();
	//m_pDraw->SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
	m_pDraw->SetMode(EN_DRAW_WINMODE_SELECT);
	m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	m_pDraw->MakeDrawUnit();

	EnableDisableRdoBtn(FALSE);

	m_wndAssignElem.Connect(SC_ID_ELEM, &m_wndAssignElem);

	ModifyStyle(0, WS_MINIMIZEBOX);
}

void CCMDTendonTemplateDlg::OnCmdUsePrefix()
{
	if( m_btnUsePrefix.GetCheck() == 0 )
	{
		m_bUsePrefix = FALSE;
		m_edtPrefixName.EnableWindow(FALSE);
	}
	else
	{
		m_bUsePrefix = TRUE;
		m_edtPrefixName.EnableWindow(TRUE);
	}
}

void CCMDTendonTemplateDlg::SetTreeCtrl()
{
	BOOL bSelXZ = FALSE;
	BOOL bSelXY = FALSE;
	BOOL bSelYZ = FALSE;

	HTREEITEM hItem = m_ctlTree.GetSelectedItem();
	if(hItem)
	{    
		DWORD dWrd = m_ctlTree.GetItemData(hItem);
		m_nType = dWrd;
		
		if(IS_TREE_XZ(dWrd))
			bSelXZ = TRUE;
		else if(IS_TREE_XY(dWrd))
			bSelXY = TRUE;
		else if(IS_TREE_ELEM(dWrd))
			bSelYZ = TRUE;
	}

	m_ctlTree.DeleteAllItems();
	HTREEITEM rootXY = m_ctlTree.InsertItem( _LS(IDS_CMD_TNDN_TEMPLATE_PLANE_VIEW), TVI_ROOT, TVI_LAST);
	m_ctlTree.SetItemData(rootXY, DWORD_TREE_XY);
	HTREEITEM rootXZ = m_ctlTree.InsertItem( _LS(IDS_CMD_TNDN_TEMPLATE_ELEVATION_VIEW), TVI_ROOT, TVI_LAST);
	m_ctlTree.SetItemData(rootXZ, DWORD_TREE_XZ);
	HTREEITEM rootYZ = m_ctlTree.InsertItem( _LS(IDS_CMD_TNDN_TEMPLATE_SECTION), TVI_ROOT, TVI_LAST);
	m_ctlTree.SetItemData(rootYZ, DWORD_TREE_YZ);
	
	HTREEITEM hFirstItem = NULL;
	if(m_pTemplateCtrl->IsValidElem(FALSE))
	{
		std::vector<T_ELEM_K>* pList = m_pTemplateCtrl->GetAssignElemList();
		for(int i = 0 ; i < pList->size(); i++)
		{
			T_ELEM_K elemKey = (*pList)[i];
			CString strTitle;
			HTREEITEM hItem;

			strTitle.Format(_T("%d"), (int)elemKey);
			hItem = m_ctlTree.InsertItem(strTitle, rootYZ, TVI_LAST);
			if(i == 0)
			{
				hFirstItem = hItem;
			}
			m_ctlTree.SetItemData(hItem, (DWORD)elemKey);
		}
	}

	m_ctlTree.Expand(rootYZ, TVE_EXPAND);
	m_ctlTree.SetScrollPos(SB_VERT, 0);

	if(bSelXY)
		m_ctlTree.SelectItem(rootXY);
	else if(bSelXZ)
		m_ctlTree.SelectItem(rootXZ);
	else if(bSelYZ && hFirstItem)
		m_ctlTree.SelectItem(hFirstItem);

	m_hTreeItemSection = hFirstItem;
}

void CCMDTendonTemplateDlg::OnCmdModifyDbClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnCmdModify();
	*pResult = 0;
}
void CCMDTendonTemplateDlg::OnDbClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_pTemplateCtrl->IsValidElem(TRUE))
	{    
		HTREEITEM hItem = m_ctlTree.GetSelectedItem();
		if(hItem)
		{
			vecTndnKey keyList;
			GetSelectedKeyList(keyList);
			
			DWORD dWrd = m_ctlTree.GetItemData(hItem);
			m_nType = dWrd;
			
			if(IS_TREE_XZ(dWrd))
			{
				m_sectK = 0;
				EnableDisableRdoBtn(FALSE);
				m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_XZ, TRUE);
				m_pDraw->RedrawWindow();
			}
			else if(IS_TREE_XY(dWrd))
			{
				m_sectK = 0;
				EnableDisableRdoBtn(FALSE);
				m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_XY, TRUE);
				m_pDraw->RedrawWindow();
			}
			else if(IS_TREE_ELEM(dWrd))
			{
				m_elemK = (T_ELEM_K)(dWrd);
				T_ELEM_D eData;
				
				BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(m_elemK, eData);
				if(bExist)
				{
					EnableDisableRdoBtn(TRUE);
					changeRdoAndSection(FALSE);
					
					m_sectK = eData.elpro;
					T_SECT_D sectD;
					m_pAttrCtrl->GetSect(m_sectK, sectD);
					m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_YZ);
					m_pDraw->SetSectData(sectD, m_elemK, m_bEndI?0.0:1.0, m_bAfter, TRUE);
				}
				else ASSERT(0);
			}
			
			m_pDraw->SetSelectedLinkKeyListByTndnKey(keyList);
		}
	}
	*pResult = 0;
}

void CCMDTendonTemplateDlg::OnUnitChange()
{
	if(m_sectK > 0)
	{
		T_SECT_D sectD;
		if(m_pAttrCtrl->GetSect(m_sectK, sectD))
		{
			m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_YZ);
			m_pDraw->SetSectData(sectD, m_elemK, m_bEndI ?0.0 : 1.0, m_bAfter, TRUE, FALSE);
			m_pDraw->Redraw(TRUE);
		}
	}

	if(m_pTemplateCtrl)
	{
		CDgnTendonAutoGenCtrl* pAutoCtrl = m_pTemplateCtrl->GetAutoGenCtrl();
		if(pAutoCtrl)
		{
			pAutoCtrl->SetEpsilonUnit();
		}
	}
}

void CCMDTendonTemplateDlg::OnItemchangedList( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(!m_bIgnoreMsg)
	{
		vecTndnKey keyList;	
		GetSelectedKeyList(keyList);
		
		if(m_pCopyDlg)
		{
			m_pCopyDlg->SetPreviewDataDraw();
		}
		else
		{
			m_pDraw->SetSelectedLinkKeyListByTndnKey(keyList);
		}
	}	
	*pResult = 0;
}

void CCMDTendonTemplateDlg::SetSelectedKeyList( const vecTndnKey& keyList )
{
	CArray<int, int> arIndex;
	int i, j;
	int nSize = m_wndTendonList.GetItemCount();
	int nSizeKey = (int)keyList.size();

	arIndex.SetSize(nSizeKey);

	for(j = 0 ; j < nSizeKey; j++)
	{
		for(i = 0 ; i < nSize; i++)
		{
			if(keyList[j] == (int)m_wndTendonList.GetItemData(i))
			{
				arIndex[j] = i;
				break;
			}
		}
	}
	
	m_bIgnoreMsg = TRUE;

	if(nSizeKey > 0)
	{
		m_wndTendonList.SetFocus();
		UnSelectAllKey();
		CDlgUtil::SelectListItems(&m_wndTendonList, arIndex);
	}
	
	if(m_pCopyDlg)
		m_pCopyDlg->SetPreviewDataDraw();

	m_bIgnoreMsg = FALSE;
}

void CCMDTendonTemplateDlg::UpdateListDraw(BOOL bFit)
{
	UpdateTdnaList();
	m_pDraw->SetTdnaData(NULL, bFit);	
}

BOOL CCMDTendonTemplateDlg::EndEdit(BOOL bEnd, BOOL bDirectDrawLabel/* = FALSE*/, unsigned int nCommand/* = 0*/)
{
	if (!m_pDoc) 
	{
		ASSERT(0);
		return FALSE;
	}
	
	if(bEnd && m_pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault = FALSE;  // Modify by pig
		m_pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, bDirectDrawLabel, nCommand);
		return TRUE;
	}

	m_pDoc->m_pUndoCtrl->CancelEditDB();
	return FALSE;	
}

void CCMDTendonTemplateDlg::UnSelectAllKey()
{
	BOOL bPrev = m_bIgnoreMsg;
	m_bIgnoreMsg = TRUE;

	int n = 0;
	int nLast = m_wndTendonList.GetItemCount();
	
	for (;n < nLast;n++)
	{
		//현재 선택되어 있는 항목만, 선택해제
		if(m_wndTendonList.GetItemState(n, LVIS_SELECTED) == LVIS_SELECTED){
			m_wndTendonList.SetItemState(n, 0, LVIS_SELECTED);
			m_wndTendonList.SetItemState(n, 0, LVIS_FOCUSED); //포커스 해제
		} 
	}
	
	m_bIgnoreMsg = bPrev;
}

void CCMDTendonTemplateDlg::OnRdoClicked()
{
	changeRdoAndSection(TRUE);
}

void CCMDTendonTemplateDlg::EnableDisableRdoBtn( BOOL bEnable )
{
	if(bEnable)
	{
		m_rdoEndI.EnableWindow(TRUE);
		m_rdoEndJ.EnableWindow(TRUE);
	}
	else
	{
		m_rdoEndI.EnableWindow(FALSE);
		m_rdoEndJ.EnableWindow(FALSE);
		m_rdoEndI.SetCheck(FALSE);
		m_rdoEndJ.SetCheck(FALSE);
	}
}

void CCMDTendonTemplateDlg::changeRdoAndSection( BOOL bRedraw )
{	
	BOOL bEndI = m_rdoEndI.GetCheck();
	BOOL bEndJ = m_rdoEndJ.GetCheck();
	if(bEndI != bEndJ)
	{
		m_bEndI = bEndI;
	}
	else
	{
		m_rdoEndI.SetCheck(TRUE);
		m_rdoEndJ.SetCheck(FALSE);
		m_bEndI = TRUE;
	}

	if(bRedraw)
	{
		if( IS_TREE_ELEM(m_nType) )
		{
			T_SECT_D sectD;
			m_pAttrCtrl->GetSect(m_sectK, sectD);
			m_pDraw->SetDrawType(DRAW_EDIT_BTN_TYPE_YZ);
			m_pDraw->SetSectData(sectD, m_elemK, m_bEndI?0.0:1.0, m_bAfter, TRUE);
		}
	}
}

void CCMDTendonTemplateDlg::SetMover()
{
	m_pControlMover = new CCMDControlMover(this);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_PREFIXNAME,   0, 0, 0, 100);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN,  0, 0, 0, 100);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_TOP_GROUP,    0, 0, 0, 100);

	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_ADD_ASSIGN,      0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_ELEM_ASSIGN_CBX, 0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL,   0, 100, 0, 0);

	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_ADD,      0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_MOD,      0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_SET_PROP, 0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_COPY,     0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_DEL,      0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_IMPORT,   0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_EXPORT,   0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_TUNNING,  0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_RESETNAME,0, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDOK,     20, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDCANCEL, 20, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_APPLY, 20, 100, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_LIST, 0, 0, 20, 100);

	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_TREE,         20, 0, 80, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_DRAW_BTN,     20, 0, 80, 100);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_BOTTOM_GROUP, 20, 0, 80, 100);

	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_BOT_POS_LAB,  100, 0, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_RDO_END_I,    100, 0, 0, 0);
	m_pControlMover->AddCtrl2Move(IDC_CMD_TENDON_TEMPLATE_RDO_END_J,    100, 0, 0, 0);
}

void CCMDTendonTemplateDlg::OnSize( UINT nFlag, int nMoveX, int nMoveY )
{
	CDialog::OnSize(nFlag, nMoveX, nMoveY);

	if(m_pControlMover)
	{
		if(nFlag == SIZE_MINIMIZED)
		{
			m_bMinimized = TRUE;
			ModifyStyle(WS_MAXIMIZEBOX, 0);

			CWnd* pWnd = AfxGetMainWnd();
			CRect mainRect, newPos, curRect;
			pWnd->GetWindowRect(mainRect);
			GetWindowRect(curRect);

			newPos.top = mainRect.top + mainRect.Height() - curRect.Height();
			newPos.bottom = newPos.top + curRect.Height();
			if(newPos.top > curRect.top)
			{
				newPos.top = curRect.top;
				newPos.bottom = curRect.bottom;
			}
			newPos.left = mainRect.left;
			newPos.right = curRect.Width();
			if(newPos.left < curRect.left)
			{
				newPos.left = curRect.left;
				newPos.right = curRect.right;
			}

			MoveWindow(newPos, TRUE);
		}
		else if(nFlag == SIZE_RESTORED)
		{
			m_bMinimized = FALSE;
			ModifyStyle(0 , WS_MAXIMIZEBOX);
		}

		if(!m_bMinimized)
		{
			m_pControlMover->OnSizeChanged();
			MakeListHeader();
			
			m_pDraw->Init();
			m_pDraw->MakeDrawUnit();
		}
	}  
}

void CCMDTendonTemplateDlg::OnCmdAssignElemDetail()
{
	//대화상자를 띄운다. 현재 대화상자를 숨기고.....
	//domodal...

	CRect rectChild, rectParent;

	GetWindowRect(rectParent);
	SetShowWnd(FALSE);
	ShowWindow(SW_HIDE);
	
	DeleteChildDlg();
	CCMDTendonTemplateAssignElem* pDlg = new CCMDTendonTemplateAssignElem(this);
	pDlg->Create(CCMDTendonTemplateAssignElem::IDD, this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->GetWindowRect(rectChild);
	int nHeight = rectChild.Height();
	int nWidth = rectChild.Width();
	rectChild.left = rectParent.left;
	rectChild.top = rectParent.top;
	rectChild.right = rectChild.left + nWidth;
	rectChild.bottom = rectChild.top + nHeight;
	pDlg->MoveWindow(rectChild);
}

void CCMDTendonTemplateDlg::OnCmdAssignElemAdd()
{  
	CString StrText;  
	CArray<int, int> aElem;
	int i, nSize;
	
	m_wndAssignElem.GetWindowText(StrText);
	
	if(StrText.IsEmpty())
		return;

	if(!CStrParser::ParsingListByTo(StrText, aElem))
	{
		if(m_bUseAfxMsgBox)
			AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_EXIST_ELEM));
		return;
	}
	
	UnselectAll();

	nSize = m_arKeyStr.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		if(m_arKeyStr[i] == StrText)
		{
			//이미 등록.
			if(m_bUseAfxMsgBox)
				AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_ALREADY_ADDED_ELEM));
			return;
		}
	}

	T_SECT_K keySectCur = m_pTemplateCtrl->GetSectKeyByElemsStr(StrText);
	if(keySectCur > 0)
	{
		nSize = m_arKeyStr.GetSize();
		T_SECT_K keySectTemp;
		for(i = 0 ; i < nSize; i++)
		{
			keySectTemp = m_pTemplateCtrl->GetSectKeyByElemsStr(m_arKeyStr[i]);

			if(keySectTemp != keySectCur)
			{
				// 다른 단면 존재
				SetElemsCbxInit();
				if(m_bUseAfxMsgBox)
					AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_DIFF_SECT));
				return;
			}
		}
	}
	else if(m_arKeyStr.GetSize() > 0) //등록된 요소들이 이미 존재할때.
	{
		SetElemsCbxInit();
		if(m_bUseAfxMsgBox)
			AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_DIFF_SECT));
		return;
	}

	m_arKeyStr.Add(StrText);
	
	RefreshAssignElemCombo();
	SetElemsCbxInit( TRUE );
}

void CCMDTendonTemplateDlg::RefreshAssignElemCombo()
{
	int i;
	int idx;
	int nSize = m_arKeyStr.GetSize();

	m_cbxAssignElem.ResetContent();
	for(i = 0 ; i < nSize; i++)
	{
		LPCTSTR strInput = m_arKeyStr[i];
		idx = m_cbxAssignElem.AddString(strInput);
	}

	nSize = m_cbxAssignElem.GetCount();
	if(nSize > 0)
	{
		m_cbxAssignElem.SetCurSel(nSize - 1);
	}
	ResetDropWidth(&m_cbxAssignElem);
}

void CCMDTendonTemplateDlg::ResetDropWidth( CComboBox* pCbx )
{
	int nNumEntries = pCbx->GetCount();

	int nWidth = 0;
	CString str;
	
	CClientDC dc(this);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());
	
	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	for(int i = 0; i < nNumEntries; i++)
	{
		pCbx->GetLBText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = __max(nWidth, nLength);
	}
	nWidth += dc.GetTextExtent(_T("0")).cx;
	
	dc.RestoreDC(nSave);
	pCbx->SetDroppedWidth(nWidth);  
	pCbx->SetItemHeight(-1, 15);
}

void CCMDTendonTemplateDlg::OnAssignElemCbxChanged()
{
	if(m_bIgnoreMsg)
		return;

	int nSel = m_cbxAssignElem.GetCurSel();
	CString strTemp;
	m_cbxAssignElem.GetLBText(nSel, strTemp);
	m_wndAssignElem.SetWindowText(strTemp);
	m_wndAssignElem.SelectByStr(strTemp);
}

void CCMDTendonTemplateDlg::OnResetName( BOOL bUpdateList /*= TRUE*/ )
{
	vecTndnKey keyList;
	int i;
	int nSize = m_pTemplateCtrl->GetKeyList(keyList);
	TndnTemplate tempInfo;
	
	if(m_bUsePrefix)
	{
		m_pTemplateCtrl->ClearNameCounter();
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pTemplateCtrl->GetTemplateNU(keyList[i], tempInfo))
			{
				ASSERT(0);
				continue;
			}
			tempInfo.strName = _T("");
			m_pTemplateCtrl->ModTemplateNU(keyList[i], tempInfo);
		} 
		
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pTemplateCtrl->GetTemplateNU(keyList[i], tempInfo))
			{
				ASSERT(0);
				continue;
			}
			CString strPrefix;
			m_edtPrefixName.GetWindowText(strPrefix);
			m_pTemplateCtrl->GetNewName(strPrefix, tempInfo.strName);
			m_pTemplateCtrl->ModTemplateNU(keyList[i], tempInfo);
		}
		
		if(bUpdateList)
			UpdateTdnaList();
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_NOPREFIX));
	} 
}

BOOL CCMDTendonTemplateDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_DELETE)
			OnCmdDelete(); //삭제.
		else if(pMsg->wParam == 'a' || pMsg->wParam == 'A')
		{
			if(GetKeyState(VK_CONTROL) < 0)
			{ //전체선택.
				vecTndnKey keyList;
				m_pTemplateCtrl->GetKeyList(keyList);
				SetSelectedKeyList(keyList);
				m_pDraw->SetSelectedLinkKeyListByTndnKey(keyList);
			}
		}
	}
	
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMDTendonTemplateDlg::SetElemsCbxInit(BOOL bLast)
{
	int nCount = m_cbxAssignElem.GetCount();
	if( nCount > 0 )
	{
		int nSel;

		if(bLast)
			nSel = nCount - 1;
		else
			nSel = 0;
		
		m_bIgnoreMsg = TRUE;
		m_cbxAssignElem.SetCurSel(nSel);
		CString strTemp;
		m_cbxAssignElem.GetLBText(nSel, strTemp);
		m_wndAssignElem.SetWindowText(strTemp);
		m_bIgnoreMsg = FALSE;
	}
}

void CCMDTendonTemplateDlg::AppendStr( CString& strOrg, const CString& strAppend )
{
	CString strTemp;
	strTemp.Format(_T("%s"), strOrg);
	strOrg.Format(_T("%s%s"), strTemp, strAppend);
}

void CCMDTendonTemplateDlg::GetPrefixName( CString& strName )
{
	m_edtPrefixName.GetWindowText(strName);
}

void CCMDTendonTemplateDlg::ShowHideChildDlg( EN_DLG_TYPE enType )
{
	CWnd* pWnd = NULL;
	if(enType == EN_DLG_TYPE_COPY)
	{  
		if(m_pCopyDlg)
		{
			DeleteChildDlg(enType);
			return;
		}
		else
		{
			DeleteChildDlg();
			m_pCopyDlg = new CCMDTendonTemplateCopyDlg(m_pTemplateCtrl, m_pDraw, this);
			m_pCopyDlg->Create(CCMDTendonTemplateCopyDlg::IDD, this);
			pWnd = m_pCopyDlg;
		}
	}
	else if(enType == EN_DLG_TYPE_AUTOGEN)
	{
		if(m_pAutoGenDlg)
		{
			DeleteChildDlg(enType);
			return;
		}
		else
		{
			DeleteChildDlg();
			m_pAutoGenDlg = new CCMDTendonTemplateAutoGenDlg(GetTemplateCtrl(), this);
			if(m_btnUsePrefix.GetCheck())
			{
				m_edtPrefixName.GetWindowText(m_pAutoGenDlg->m_strPrefixName);
			}
			m_pAutoGenDlg->Create(CCMDTendonTemplateAutoGenDlg::IDD, this);
			pWnd = m_pAutoGenDlg;
		}
	}
	else
		return;

	//   vecTndnKey keyList;
	//   GetSelectedKeyList(keyList);

	CRect rectParent, rectChild, rectNewPos;
	GetWindowRect(&rectParent);
	pWnd->GetWindowRect(&rectChild);
	pWnd->ShowWindow(SW_SHOW);

	rectNewPos.top = rectParent.top;
	rectNewPos.bottom = rectNewPos.top + rectChild.Height();

	BOOL  bMaximized = FALSE;
	BOOL  bOutside = FALSE;
	DWORD style = this->GetStyle();
	int   nSystemWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int   nRightPos = rectParent.right + rectChild.Width();

	//모니터를 벗어나거나, 최대화상태일때.
	if(rectParent.right > nSystemWidth) nRightPos -= nSystemWidth;
	if(nRightPos > nSystemWidth)        bOutside = TRUE;
	if (style & WS_MAXIMIZE)            bMaximized = TRUE;

	if(bMaximized || bOutside)
	{
		rectNewPos.left = rectParent.right - rectChild.Width();
		rectNewPos.right = rectNewPos.left + rectChild.Width();
	}
	else
	{
		rectNewPos.left = rectParent.right;
		rectNewPos.right = rectNewPos.left + rectChild.Width();
	}

	pWnd->MoveWindow(rectNewPos);  
}

void CCMDTendonTemplateDlg::DeleteChildDlg(EN_DLG_TYPE enType, EN_DLG_TYPE enTypeExcept)
{
	CArray<EN_DLG_TYPE, EN_DLG_TYPE> caType;
	if(enType == EN_DLG_TYPE_ALL)
	{
		caType.Add(EN_DLG_TYPE_COPY);
		caType.Add(EN_DLG_TYPE_AUTOGEN);
	}
	else
	{
		caType.Add(enType);
	}

	int i, nSize;
	nSize = caType.GetSize();

	CCMDlgBase* pWnd;
	for(i = 0 ; i < nSize; i++)
	{
		if(caType[i] == enTypeExcept)             continue;
		else if(caType[i] == EN_DLG_TYPE_COPY)    pWnd = m_pCopyDlg;
		else if(caType[i] == EN_DLG_TYPE_AUTOGEN) pWnd = m_pAutoGenDlg;
		else                                      continue;

		if(pWnd)
		{
			if(pWnd->GetSafeHwnd() && ::IsWindow(pWnd->GetSafeHwnd()))
			{
				pWnd->DestroyWindow();
			}
		}
		SetChidDlgNULL(caType[i]);
	}
}

void CCMDTendonTemplateDlg::SetChidDlgNULL( EN_DLG_TYPE enType )
{
	switch (enType)
	{
	case EN_DLG_TYPE_AUTOGEN: m_pAutoGenDlg = NULL; break;
	case EN_DLG_TYPE_COPY:    m_pCopyDlg = NULL;    break;
	}  
}

void CCMDTendonTemplateDlg::SetUseOriginDraw( BOOL bUse )
{
	if(m_pDraw)
	{
		m_pDraw->SetUseOrigin(bUse);
		m_pDraw->MakeDrawUnit();
	}
}

BOOL CCMDTendonTemplateDlg::GetUseOriginDraw()
{
	if(m_pDraw)
		return m_pDraw->GetUseOrigin();
	return FALSE;
}

BOOL CCMDTendonTemplateDlg::GetOriginPos_Draw( double* pPos )
{
	if(m_pDraw)
		return m_pDraw->GetDrawOrigin(pPos);
	return FALSE;
}

void CCMDTendonTemplateDlg::SetOriginPos_Draw( double* pPos )
{
	if(m_pDraw)
		m_pDraw->SetDrawOrigin(pPos, TRUE, FALSE);
}

void CCMDTendonTemplateDlg::SetOriginPos_AutoDlg( double* dPos )
{
	if(m_pAutoGenDlg)
		m_pAutoGenDlg->SetXYPos(dPos);
}

BOOL CCMDTendonTemplateDlg::GetOriginPos_AutoDlg( double* dPos )
{
	if(m_pAutoGenDlg)
		return m_pAutoGenDlg->GetXYPos(dPos);
	return FALSE;
}

