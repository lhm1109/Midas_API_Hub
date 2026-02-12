// CMStorGrid_IGen.cpp : implementation file
//
// 2005.5.12 층 순서를 고층부터 Display 하도록 변경(jbseon)
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStorGrid_IGen.h"
#include "CMStorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#define STOR_KEY_FMT    _T("%d")
#define STOR_DATA_FMT   _T("%.15g")

#define D_STOR_DIAPH_YES  _LS(IDS_WG_CMD__ADDD__Consider)
#define D_STOR_DIAPH_NO   _LS(IDS_WG_CMD__ADDD__Do_not_consider)

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid_IGen
CString CCMStorGrid_IGen::m_aCurDefVal[] = {_T(""), _T("0"), _T("0"), _T(""), _T("0"), _T("0"), _T("0")};

CCMStorGrid_IGen::CCMStorGrid_IGen()
{
	m_bBeforeSaved = FALSE;

	T_STOR_UNIT * pUnit = &(CUnitCtrl::m_STOR_UNIT);
																																												 
	AddColInfo(_T("ID")                                     , t_integer, 0                     );
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name              ), t_string , pUnit->StoryName      );
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Level             ), t_real   , pUnit->dStoryLevel    );
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Height            ), t_real   , D_UNITSYS_BASE_LENGTH );
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_nDiaphragm  ), t_string , pUnit->bFloorDiaphragm);
	AddColInfo(_LS(IDS_CMD_STORY_GROUP_REPORT          ), t_string , D_UNITSYS_NONE        );
	AddColInfo(_LS(IDS_CMD_STORY_GROUP_ECCEN           ), t_string , D_UNITSYS_NONE        );
	AddColInfo(_LS(IDS_CMD_STORY_GROUP_LEVEL           ), t_real   , D_UNITSYS_BASE_LENGTH );  
	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = {_T(""), _T("0"), _T("0"), D_STOR_DIAPH_YES, _T("0"), _T("0"), _T("0")};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];
}

CCMStorGrid_IGen::~CCMStorGrid_IGen()
{
}


BEGIN_MESSAGE_MAP(CCMStorGrid_IGen, CTBCommon)
	//{{AFX_MSG_MAP(CCMStorGrid_IGen)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid_IGen message handlers

void CCMStorGrid_IGen::Initialize(CDBDoc* pDoc)
{
	T_UNIT_SYSTEM UnitSystem;
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetAllowEnter(TRUE));
	SetRowHeightByDPI(0, 0, 36);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, GetInitialColCount()), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));

	// Key column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetPlaces(0)
			.SetEnabled(FALSE));
	SetFrozenCols(2, 0);
	HideColsByIntend(1, 1, TRUE);

	// Name Column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT));
	// Height Column 편집 금지 -> Append Row는 가능하게 수정
	//SetStyleRange(CGXRange().SetCols(4), CGXStyle()
	//    .SetEnabled(FALSE));

	// Floor Diaphragm의 스타일 변경
	CString szChoiceList;
	szChoiceList.Format(_T("%s\n%s\n"), D_STOR_DIAPH_YES, D_STOR_DIAPH_NO);
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T("")));

	CString strGrupName;
	CArray<CString, CString&> strStoryGrupList;
	CArray<CString, CString&> strGrupList;
	m_pDoc->m_pAttrCtrl->GetGrupList(strGrupList, TRUE);
	for (int i=0; i<strGrupList.GetSize(); i++)
	{
		strGrupName = strGrupList[i];
		T_GRUP_D GrupD;
		if(!m_pDoc->m_pAttrCtrl->GetGrup(strGrupName, GrupD)) {ASSERT(0); continue;}
		if(GrupD.nPlaneType != 3) continue;
		strStoryGrupList.Add(strGrupName);
	}

	// 계산서 출력용의 스타일 변경  
	szChoiceList.Format(_T("%s\n%s\n"), _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY), _LS(IDS_CMD_STORY_GROUP_NO_OUTPUT));
	for (int i=0; i<strStoryGrupList.GetSize(); i++)
	{
		strGrupName = strStoryGrupList[i] + _T("\n");
		szChoiceList += strGrupName;
	}
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(szChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T("")));

	// 강성/편심율용의 스타일 변경  
	szChoiceList.Format(_T("%s\n%s\n"), _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY), _LS(IDS_CMD_STORY_GROUP_EQUAL_REPORT));
	for (int i=0; i<strStoryGrupList.GetSize(); i++)
	{
		strGrupName = strStoryGrupList[i] + _T("\n");
		szChoiceList += strGrupName;
	}
	SetStyleRange(CGXRange().SetCols(7), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(szChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T("")));

	// real type column의 스타일 변경
	int anCol[] = {3, 4, 8};
	for (int i = 0; i < sizeof(anCol)/sizeof(int); i++)
	{
		SetStyleRange(CGXRange().SetCols(anCol[i]), CGXStyle()
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));
	}
	 
	// Dialog Grid는 Preference에 없으므로 자체 Column Size 조절
	SetColumnAttr();

#ifdef _MVIL
	SetModeDisplayOnly(TRUE);
#endif

	SetRightValidCell(1, 1); 

	// Sorting : Level로 Sorting한다. : 이 순서는 안 바뀌는 것으로 가정한다.
	// 왜냐하면 Height는 계산되어 설정되는 값이고 성능상의 이유로 Level별로
	// Sorting되어 있다고 가정하고 프로그래밍 되어 있다.
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 2;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::descending;
	m_aSortInfo[1].nRC = 0;
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	MakeItemEx();

	this->EnableInsertRowByPressingKey(FALSE);  // Insert Key에 의한 Record 삽입 금지
	LockUpdate(FALSE);
}

// 수정 없음
BOOL CCMStorGrid_IGen::InsertRecordByKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, IrstD, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data, IrstD);
}

// 수정 없음
BOOL CCMStorGrid_IGen::DeleteRecordByKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD)
{
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, IrstD, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}

// 수정 없음
void CCMStorGrid_IGen::UnitChanged(CDBDoc* pDoc)
{
	BOOL bOldLock = LockUpdate(TRUE);

	T_UNIT_SYSTEM UnitSystem;
 	//pDoc->m_pUnitCtrl->GetUnitSystemStor(UnitSystem);

	// 타이틀을 수정
	SetHeaderTitle(UnitSystem);
	// 각 항목의 값을 수정
	MakeItemEx(0, -2, TRUE);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

// IsExist???? 수정
void CCMStorGrid_IGen::UpdateBuffer(CDBDoc* pDoc)
{
	int nModified = 0;

	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bOldLock = LockUpdate(TRUE);

	T_STOR_K key, keyBak; 
	T_STOR_D data, dataBak;
	T_IRST_D IrstD, IrstBak;
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STOR_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetStor(nKey, key, data);
				if(m_pDoc->m_pAttrCtrl2->ExistIrst(nKey)) 
					m_pDoc->m_pAttrCtrl2->GetIrst(key, IrstD);
				else 
					IrstD.Initialize();
				InsertRecordByKey(key, data, IrstD);
				nModified |= 0x01;
				break;
		case(UR_STOR_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetStor(nKey, key, data);
				if(m_pDoc->m_pAttrCtrl2->ExistIrst(nKey)) 
					m_pDoc->m_pAttrCtrl2->GetIrst(key, IrstD);
				else                
					IrstD.Initialize();
				DeleteRecordByKey(key, data, IrstD);
				nModified |= 0x02;
				break;
		case(UR_STOR_MFD):
				// 여기서는 저장만 하고 UR_STOR_MFS에서 처리한다.
				pViewBuff->GetStor(nKey, keyBak, dataBak);
				if(!m_pDoc->m_pAttrCtrl2->GetIrst(keyBak, IrstBak))
					IrstBak.Initialize();        
				nModified |= 0x01;
				bMFD = TRUE;
				break;
		case(UR_STOR_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStor(nKey, key, data);
				if(!m_pDoc->m_pAttrCtrl2->GetIrst(key, IrstD))
					IrstD.Initialize();
				nModified |= 0x02;
				DeleteRecordByKey(key, data, IrstD);
				InsertRecordByKey(keyBak, dataBak, IrstBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
/*
		case(UR_IRST_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetIrst(nKey, key, IrstD);        
			if(m_pDoc->m_pAttrCtrl->GetStor(key, data))
			{
				InsertRecordByKey(key, data, IrstD);
			}          
			nModified |= 0x01;      
			break;          
		case(UR_IRST_DEL):      
			break;
		case(UR_IRST_MFD):
			// 여기서는 저장만 하고 UR_STOR_MFS에서 처리한다.
			pViewBuff->GetIrst(nKey, keyBak, IrstBak); 
			nModified |= 0x01;
			bMFD = TRUE;
			break;
		case(UR_IRST_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetIrst(nKey, key, IrstD);
			nModified |= 0x02;
			DeleteRecordByKey(key, data, IrstD);
			InsertRecordByKey(keyBak, dataBak, IrstBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
 */  
		default:  
			break;
		}
	} // end of while
	if (nModified) ShowSelected();
	AdjustAppendRowNo();
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid_IGen Overridable Functions
// 수정 없음
void CCMStorGrid_IGen::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STOR_K)keys[i]);
	}
}

// 수정 없음
long CCMStorGrid_IGen::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<T_STOR_K, T_STOR_K> aKey;

	GetAllSelectedStor(aKey);
	nKeyCount = aKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aKey[i];
	
	return nKeyCount;
}

// 데이타 타입에 맞게 수정
BOOL CCMStorGrid_IGen::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMStorGrid__ValidateField__Erro));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0)  // ID : error check 없음
	{
		return TRUE;  
	}
	else if (nColID == 1 || nColID == 4 || nColID == 5 || nColID == 6)  // Name, Height, Diaphragm, 계산서, 강성/편심율
	{
		nErrCode; // Do Nothing...
	}
	else
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
		if (nColID == 3 && _tstof(value) < 0.0)  // Height의 경우 음수는 안됨
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Height_can_t_be_negative_));
			return FALSE;
		}
	}
	return TRUE;
}

// 수정 없음
BOOL CCMStorGrid_IGen::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// ID 필드는 제외
	CArray<int, int> aCheck;    
	for (int k=1; k<8; k++) aCheck.Add(k);

	int nSize = aCheck.GetSize();
	int i;
	for (int c = 0; c < nSize; c++) 
	{
		i = aCheck[c];
		nPos = aCols[i]-1;
		
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 2Part Key 아니면 수정 없음
BOOL CCMStorGrid_IGen::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STOR_K &key, T_STOR_D &data, T_IRST_D &IrstD)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _tstol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data, IrstD);
	}
	return TRUE;
}

// Add???? 수정
BOOL CCMStorGrid_IGen::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_STOR_K key;
	T_STOR_D data;
	T_IRST_D IrstD;
	GetValue(nRow, key, data, IrstD);
	CArray<T_STOR_D, T_STOR_D&> aData; aData.Add(data);
	CArray<T_IRST_D, T_IRST_D&> aIrst; aIrst.Add(IrstD);
	return m_pDoc->m_pDataCtrl->AddStorWithIrst(aData, aIrst);  
}

// Multiple data paste 지원시 Add???? 수정
BOOL CCMStorGrid_IGen::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	// [Check before Release...!!] KIM GEUN-YOUNG20111007 : 지원 안함.
	return FALSE;

	ASSERT(raKey.GetSize() == raData.GetSize());
	T_STOR_K tKey;
	CArray<T_STOR_K, T_STOR_K> aKey;
	CArray<T_STOR_D, T_STOR_D&> aData;
	CArray<T_IRST_D, T_IRST_D&> aIrst;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_STOR_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_STOR_D*)raData.GetAt(i)));
		//aIrst.Add(*((T_IRST_D*)raIrst.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddStorWithIrst(aData, aIrst);
}

// 수정 없음
BOOL CCMStorGrid_IGen::DeleteFromDB(ROWCOL nRow)
{
	T_STOR_K key;
	T_STOR_D data;
	T_IRST_D IrstD;
 
	GetValue(nRow, key, data, IrstD);
	return m_pDoc->m_pDataCtrl->DelStor(data.StoryName);
}

// 2Part Key 아니면 수정 없음
BOOL CCMStorGrid_IGen::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// Name 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(1, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(value);
	}

	return m_pDoc->m_pDataCtrl->DelStor(awKeys);
}

// 2 Part Key 아니면 수정 없음
BOOL CCMStorGrid_IGen::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Name 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(1, nCol));

	CString csOldName;
	T_STOR_K key;
	T_STOR_D data;
	T_IRST_D IrstD;

	csOldName = GetValueRowColBeforeEdit(nRow, nCol);

	GetValue(nRow, key, data, IrstD);
	CArray<CString, CString&> strStoryName; strStoryName.Add(csOldName);
	CArray<T_STOR_D, T_STOR_D&> aStorD;     aStorD.Add(data);
	CArray<T_IRST_D, T_IRST_D&> aIrstD;     aIrstD.Add(IrstD);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyStorWithIrst(strStoryName, aStorD, aIrstD);
}


BOOL CCMStorGrid_IGen::ModifyToDB(CStringArray& raOldKey, 
																	CArray<void*, void*>& raKey, 
																	CArray<void*, void*>& raData)
{
	// [Check before Release...!!] KIM GEUN-YOUNG20111007 : 지원 안함.
	return FALSE;

	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldName;  
	T_STOR_K key;
	T_STOR_D data;
	T_IRST_D IrstD;
	CArray<T_STOR_D, T_STOR_D&> aData;
	CArray<T_IRST_D, T_IRST_D&> aIrst;   
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		key = _tstol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl->GetStor(key, data);
		aOldName.Add(data.StoryName);
		aData.Add(*((T_STOR_D*)raData.GetAt(i)));
		//aIrst.Add(*((T_IRST_D*)raIrst.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifyStorWithIrst(aOldName, aData, aIrst);
}


// 수정 없음
CString CCMStorGrid_IGen::GetNewKey()
{
	CString value;
	//T_STOR_K key = m_pDoc->m_pAttrCtrl->GetStartNumStor();
	//value.Format(STOR_KEY_FMT, key);
	value = _T("");
	return value;
}

// 수정 없음
CString CCMStorGrid_IGen::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

// 수정 없음
CString* CCMStorGrid_IGen::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMStorGrid_IGen::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																					 CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STOR_K* pKey;
	T_STOR_D* pData;
	T_IRST_D* pIrst;
	pKey = new T_STOR_K;
	pData = new T_STOR_D;
	pIrst = new T_IRST_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	
	CArray<void*, void*> aIrst;
	aIrst.Add((void*)pIrst);

	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData, *pIrst)) return FALSE;
	return TRUE;
}

// 수정 없음
void CCMStorGrid_IGen::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STOR_K* pKey;
	T_STOR_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STOR_K*)aKey.GetAt(i);
		pData = (T_STOR_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

// 수정 없음
long CCMStorGrid_IGen::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountStor();
}

// 수정 없음
void CCMStorGrid_IGen::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_D Data;
	T_IRST_D IrstD;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetStor(aFilteredKeys[nCount], Data);
		if(!pDoc->m_pAttrCtrl2->GetIrst(aFilteredKeys[nCount], IrstD))
			IrstD.Initialize();
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, IrstD, awCols);
	}
}

// 수정 없음
void CCMStorGrid_IGen::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_STOR_K Key;
	T_STOR_D Data;
	T_IRST_D IrstD;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartStor();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextStor(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_STOR_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetStor(KeyBuf[nCount], Data);
		if(!pDoc->m_pAttrCtrl2->GetIrst(KeyBuf[nCount], IrstD))
			IrstD.Initialize();
		
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, IrstD, awCols);    
	} // end of loop nCount

	delete []KeyBuf;
}

// 2 Part Key 외 수정 없음
void CCMStorGrid_IGen::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_K Key;
	T_STOR_D Data;
	T_IRST_D IrstD;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data, IrstD);
	}
	pDoc->m_pUnitCtrl->ConvertUnitStorPrevious(Data);
	pDoc->m_pUnitCtrl->ConvertUnitIrstPrevious(IrstD);  
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data, IrstD);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid_IGen Implementation
// 2 Part Key 외 수정 없음
BOOL CCMStorGrid_IGen::GetValue(ROWCOL nRow, T_STOR_K &key, T_STOR_D &data, T_IRST_D& IrstD)
{
	if (nRow < 1) return FALSE;
	if (IsAppendRow(nRow) || nRow > GetRowCount()) return FALSE;
	ROWCOL nCol;
	CString str;
	data.Initialize();
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		if(i==0) 
		{
			key = _tstol(str);
		}
		else ConvStrToData(i, str, data, IrstD);
	}
	return TRUE;
}

// 수정 없음
BOOL CCMStorGrid_IGen::SetValue(ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D& IrstD)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, IrstD, awCols);
}

// 2Part Key, Column Enable/Disable이 있으면 수정
BOOL CCMStorGrid_IGen::SetValue(CGXData* pData, ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD, CRowColArray &awCols)
{
	//
	CString str;
	int nValueType;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(STOR_KEY_FMT,key);
		}
		else ConvDataToStr(i, data, IrstD, str);
		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);

		BOOL bEnable = TRUE;
		if(i==6) 
		{      
			if(IrstD.nReportType == 1)
				bEnable = FALSE;
			SetStyleRange(CGXRange(nRow, i+1), CGXStyle().SetEnabled(bEnable));      
		}
		if(i==7)
		{
			if(IrstD.nEccenType == 0) 
				bEnable = FALSE;
			else if(IrstD.nEccenType == 1 && (IrstD.nReportType == 0 || IrstD.nReportType == 1))
				bEnable = FALSE;
			SetStyleRange(CGXRange(nRow, i+1), CGXStyle().SetEnabled(bEnable));      
		}    
	}

	return TRUE;
}

// 데이타 특성에 따라 수정
void CCMStorGrid_IGen::ConvStrToData(int i, CString& value, T_STOR_D& data, T_IRST_D& IrstD)
{
	if      (i == 1) data.StoryName = value;
	else if (i == 2) data.dStoryLevel = _tstof(value);
	else if (i == 3) i;  // do nothing
	else if (i == 4) 
	{
		if (value == D_STOR_DIAPH_YES) data.bFloorDiaphragm = TRUE;
		else data.bFloorDiaphragm = FALSE;
	}
	else if (i == 5)
	{
		if(value == _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY))
		{
			IrstD.nReportType  = 0;
			IrstD.nReportGrupK = 0;
		}
		else if(value == _LS(IDS_CMD_STORY_GROUP_NO_OUTPUT))
		{
			IrstD.nReportType  = 1;
			IrstD.nReportGrupK = 0;
		}
		else
		{
			IrstD.nReportType  = 2;
			IrstD.nReportGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(value);
		}
	}
	else if (i == 6)
	{
		if(value == _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY))
		{
			IrstD.nEccenType  = 0;
			IrstD.nEccenGrupK = 0;
		}
		else if(value == _LS(IDS_CMD_STORY_GROUP_EQUAL_REPORT)) 
		{
			IrstD.nEccenType  = 1;
			IrstD.nEccenGrupK = 0;
		}
		else
		{
			IrstD.nEccenType  = 2;
			IrstD.nEccenGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(value);
		}
	}
	else if (i == 7) IrstD.dLevel = _tstof(value);
}

// 데이타 특성에 따라 수정
void CCMStorGrid_IGen::ConvDataToStr(int i, const T_STOR_D& data, const T_IRST_D& IrstD, CString& value)
{  
	T_GRUP_D GrupD; GrupD.Initialize();
	if      (i == 1) value = data.StoryName;
	else if (i == 2) value.Format(STOR_DATA_FMT, data.dStoryLevel);
	else if (i == 3) value = _T("0");
	else if (i == 4) 
	{
		if (data.bFloorDiaphragm) value = D_STOR_DIAPH_YES;
		else value = D_STOR_DIAPH_NO;
	}
	else if (i == 5) 
	{
		if     (IrstD.nReportType == 0) value = _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY);
		else if(IrstD.nReportType == 1) value = _LS(IDS_CMD_STORY_GROUP_NO_OUTPUT  );
		else
		{
			if(!m_pDoc->m_pAttrCtrl->GetGrup(IrstD.nReportGrupK, GrupD)) { value = _LS(IDS_WG_CMD__ADDD__Error); return; }
			value = GrupD.GroupName; 
		}
	}
	else if (i == 6)
	{
		if     (IrstD.nEccenType == 0) value = _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY );
		else if(IrstD.nEccenType == 1) value = _LS(IDS_CMD_STORY_GROUP_EQUAL_REPORT);
		else
		{
			if(!m_pDoc->m_pAttrCtrl->GetGrup(IrstD.nEccenGrupK, GrupD)) { value = _LS(IDS_WG_CMD__ADDD__Error); return; }
			value = GrupD.GroupName; 
		}
	}
	else if (i == 7) 
	{
		double dLevel;
		if(IrstD.nEccenType == 0 || (IrstD.nEccenType == 1 && IrstD.nReportType == 0)) dLevel = data.dStoryLevel;
		else dLevel = IrstD.dLevel;
		value.Format(STOR_DATA_FMT, dLevel);
	}
	else value = _LS(IDS_WG_CMD__ADDD__Error);
}

// 2Part Key 외 수정 없음
void CCMStorGrid_IGen::MakeSearchKey(const T_STOR_K &key, const T_STOR_D &data, const T_IRST_D &IrstD, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(STOR_KEY_FMT, key);
		else ConvDataToStr(nColID, data, IrstD, aKey[i]);
	}
}

// 수정 없음
void CCMStorGrid_IGen::GetAllSelectedStor(CArray<T_STOR_K, T_STOR_K>& aKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aKey.Add(m_aSelectedKeys[i]);
}

// 데이타 특성에 따라 수정
void CCMStorGrid_IGen::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

// Column 폭을 지정한다.
// Hide 시켜야 할 칼럼을 지정한다.
void CCMStorGrid_IGen::SetColumnAttr()
{
	int anColWidth[] = { 0, 85, 85, 85, 130, 105, 105, 105};
	
	for (int i = 0; i < sizeof(anColWidth)/sizeof(int); i++)
		SetColWidthByDPI(i+1, i+1, anColWidth[i]);  
}

void CCMStorGrid_IGen::SetStoryHeight(ROWCOL nHeightCol)
{
	T_STOR_K key;
	T_STOR_D data;
	T_IRST_D IrstD;
	double dblHeight, dblPrevLevel;
	ROWCOL nRowCount = GetRowCount();
	if (nRowCount <= 1) return; // if no story exist then return

	ROWCOL ncRow, ncCol;
	BOOL bGet = GetCurrentCell(ncRow, ncCol);
	
	GetValue(nRowCount - 1, key, data, IrstD); // 첫번째 층(맨 아래층)의 데이터를 얻는다.
	dblPrevLevel = data.dStoryLevel;
	for (int i = nRowCount - 2; i >= 1; i--) // Append Row 제외
	{
		if(m_nEditMode == addnew && bGet && ncRow == i) continue;  // 편집 상태인 row는 제외
		GetValue(i, key, data, IrstD);
		dblHeight = data.dStoryLevel - dblPrevLevel;
		if (dblHeight < 0.0) dblHeight = 0.0; // Add될 때 Update된 것과 입력시 남아 있는 것 처리
		SetValueRange(CGXRange(i+1, nHeightCol), dblHeight);
		BOOL bEnable = FALSE;
		if (IsEdit() && i+1 == ncRow) bEnable = TRUE;
		SetStyleRange(CGXRange(i+1, nHeightCol), CGXStyle().SetEnabled(bEnable));  //2
		dblPrevLevel = data.dStoryLevel;
	}

	if (nRowCount > 1)
	{
		SetValueRange(CGXRange(1, nHeightCol), double(0.0));   // 최상층 층고 0으로 강제 설정 
		SetStyleRange(CGXRange(1, nHeightCol), CGXStyle().SetEnabled(FALSE));  // 최상층 층고는 편집불가
	}
}

void CCMStorGrid_IGen::AdjustAppendRowNo(ROWCOL nCol)
{
	CTBCommon::AdjustAppendRowNo(nCol);

	ROWCOL nHeightCol;
	GetColumnNoFor(3, nHeightCol);

	// Story를 설정해 준다.
	SetStoryHeight(nHeightCol);

	BOOL bEnable = FALSE;
	ROWCOL nRowCount = GetRowCount();
	if (nRowCount > 1) bEnable = TRUE;
	// 첫번째 Row의 Story는 Disable한다.  => 층 개념 변경으로 첫번째 Row의 Disable을 취소한다.
//  SetStyleRange(CGXRange(1, nHeightCol), CGXStyle().SetEnabled(FALSE));  
	// Append Row의 Story는 Record가 있으면 Enable한다.
	SetStyleRange(CGXRange(nRowCount, nHeightCol), CGXStyle().SetEnabled(bEnable));
	// 현재 셀이 Disable된 Cell에 있으면 첫번째 Valid한 Cell로 이동
	if (!bEnable)
	{
		ROWCOL ncCol, ncRow;
		GetCurrentCell(ncRow, ncCol);
		if (ncCol == nHeightCol) SetRightValidCell(ncRow, 0);
	}
}

////////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// Level 변경시 Height 변경, Height 변경시 Level 변경
void CCMStorGrid_IGen::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnModifyCell(nRow, nCol);
	SaveCurrentRecordValue(nRow, nCol);
//  SaveCurrentRecordValue(nRow + 1, nCol);
	ChangeColumnStatus(nRow, nCol);
}

void CCMStorGrid_IGen::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	RestorePrevRecordValue(nRow, nCol);
}

BOOL CCMStorGrid_IGen::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnEndEditing을 호출한다.
	if (!CTBCommon::OnEndEditing(nRow, nCol)) return FALSE;
	if (!m_bBeforeSaved) return TRUE;

	// 값이 저장되었다는 플래그를 reset한다.
	m_bBeforeSaved = FALSE;
	return TRUE;
}

BOOL CCMStorGrid_IGen::OnCanceledModify()  // Unused function
{
	CTBCommon::OnCanceledModify();
	// Height를 재설정한다.
	ROWCOL nRow, nCol;
	GetCurrentCell(nRow, nCol);
	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(3, nHeightCol);

	double dblHeight;
	if (nRow <= 1) dblHeight = 0.0;
	else 
	{
		// 순서 변경
		//dblHeight = _tstof(GetValueRowCol(nRow, nLevelCol))
		//           - _tstof(GetValueRowCol(nRow-1, nLevelCol));
		dblHeight = _tstof(GetValueRowCol(nRow-1, nLevelCol))
							- _tstof(GetValueRowCol(nRow, nLevelCol));
	}
	SetValueRange(CGXRange(nRow, nHeightCol), dblHeight);

	return TRUE;
}

void CCMStorGrid_IGen::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nLevelCol, nHeightCol, nReportCol, nEccenCol, nEccenLevelCol;
	GetColumnNoFor(2, nLevelCol     );
	GetColumnNoFor(3, nHeightCol    );
	GetColumnNoFor(5, nReportCol    );
	GetColumnNoFor(6, nEccenCol     );
	GetColumnNoFor(7, nEccenLevelCol);

	if (nCol == nHeightCol)
	{
		CString csLevel, csHeight;
		double dblLevel, dblHeight;
		
		CGXControl *pControl;
		pControl = GetControl(nRow, nCol);  // 현재층의 control을 얻는다.  
		if (pControl->IsActive() && nCol == nHeightCol) pControl->GetValue(csHeight);
		else csHeight = GetValueRowCol(nRow, nHeightCol);
		csLevel = GetValueRowCol(nRow, nLevelCol);
		
		T_STOR_K key;
		T_STOR_D data;
		T_IRST_D IrstD;
		data.Initialize();
		dblHeight = _tstof(csHeight);
		dblLevel  = _tstof(csLevel);
		GetValue(nRow, key, data, IrstD);  // 현재층의 데이터  
		
		if (!pControl->IsActive() && dblLevel != 0.0) return;
		// Height칼럼이 active ---> Height를 직접 수정한 경우
		// Height칼럼이 inactive ---> Paste됨
		// 또는 Level이 0.0인 경우만 Level수정한다.
		// Paste시 Level이 0.0인 경우 Level 재 수정
		// 층 순서 변경 05.05.12
		//dblLevel = data.dStoryLevel + dblHeight;  // 윗층 Level = 현재층 Level + 현재층 층고
		//if(nRow < nNumStor) SetValueRange(CGXRange(nRow + 1, nLevelCol), dblLevel);  
		if (nRow > 1)
		{
			GetValue(nRow-1, key, data, IrstD);  // 위층의 데이터
			dblLevel = data.dStoryLevel - dblHeight;
			SetValueRange(CGXRange(nRow, nLevelCol), dblLevel);
		}
	}
	else if(nCol == nReportCol)
	{
		BOOL bEnable = TRUE;
		BOOL bEnalbe_Eccen = TRUE;
		CString csType;
		csType = GetValueRowCol(nRow, nCol);
		if(csType == _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY))       
			bEnable = FALSE;
		else if(csType == _LS(IDS_CMD_STORY_GROUP_NO_OUTPUT))
		{
			bEnable = FALSE;
			csType = GetValueRowCol(nRow, nEccenCol);
			if(csType == _LS(IDS_CMD_STORY_GROUP_EQUAL_REPORT))
				bEnalbe_Eccen = FALSE;      
		}
		SetStyleRange(CGXRange(nRow, nEccenLevelCol), CGXStyle().SetEnabled(bEnable      ));
		SetStyleRange(CGXRange(nRow, nEccenCol     ), CGXStyle().SetEnabled(bEnalbe_Eccen));   
	}
	else if(nCol == nEccenCol) 
	{
		BOOL bEnable = TRUE;
		CString csType;
		csType = GetValueRowCol(nRow, nCol);
		if(csType == _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY))
			bEnable = FALSE;    
		else if(csType == _LS(IDS_CMD_STORY_GROUP_EQUAL_REPORT))
		{
			csType = GetValueRowCol(nRow, nReportCol);    
			if(csType == _LS(IDS_CMD_STORY_GROUP_EQUAL_STORY))
				bEnable = FALSE;
		}
		SetStyleRange(CGXRange(nRow, nEccenLevelCol), CGXStyle().SetEnabled(bEnable));
	}
	else
		return;
}
/*
void CCMStorGrid_IGen::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(3, nHeightCol);

	if (nCol != nHeightCol) return;

	CString csLevel, csHeight;
	double dblLevel, dblHeight;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);
	if (pControl->IsActive() && nCol == nHeightCol) pControl->GetValue(csHeight);
	else csHeight = GetValueRowCol(nRow, nHeightCol);
	csLevel = GetValueRowCol(nRow, nLevelCol);

	T_STOR_K key;
	T_STOR_D data;
	data.Initialize();
	dblHeight = _tstof(csHeight);
	dblLevel = _tstof(csLevel);
	if (nRow <= 1) dblHeight = 0.0;
	else GetValue(nRow-1, key, data);  // 아래층의 데이터

	if (!pControl->IsActive() && dblLevel != 0.0) return;
	// Height칼럼이 active ---> Height를 직접 수정한 경우
	// Height칼럼이 inactive ---> Paste됨
	// 또는 Level이 0.0인 경우만 Level수정한다.
	// Paste시 Level이 0.0인 경우 Level 재 수정
	dblLevel = data.dStoryLevel + dblHeight;  // 현재 층의 Level은 아래층의 Level + 현재층의 층고
	SetValueRange(CGXRange(nRow, nLevelCol), dblLevel);
}
*/

// 변경된 후의 값이 저장된다.
void CCMStorGrid_IGen::SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (m_bBeforeSaved) return; // 이전에 저장되었으면 리턴한다.

/*T_STOR_D data;
	CArray<T_STOR_K, T_STOR_K> aStorKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);
	int nNumStor = aStorKey.GetSize();
*/
	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(3, nHeightCol);
	if(nCol != nHeightCol) return;
//  if(nRow < nNumStor) m_dblOldLevel = _tstof(GetValueRowCol(nRow + 1, nLevelCol));
//2_dblOldLevel = _tstof(GetValueRowCol(nRow + 1, nLevelCol));
	m_dblOldLevel = _tstof(GetValueRowCol(nRow, nLevelCol));
	m_bBeforeSaved = TRUE;
}

// 변경된 후의 값이 저장되므로 변경 이전의 값을 구하려면
// GetValueRowCol을 이용해야 한다. 단 이함수가 OnCanceledEditing()에서
// 호출되는 경우에 한해서이다.
void CCMStorGrid_IGen::RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bBeforeSaved) return;

	// Column ID를 구한다.
	int nColID;
	ROWCOL nLevelCol;
	GetColumnIDFor(nCol, nColID);
	GetColumnNoFor(2, nLevelCol);

	if (nColID == 3) // Height
	{
//2    SetValueRange(CGXRange(nRow + 1, nLevelCol), m_dblOldLevel);
		SetValueRange(CGXRange(nRow, nLevelCol), m_dblOldLevel);
	}

	// 플래그를 Reset시킨다.
	m_bBeforeSaved = FALSE;
}

// Story 이름을 바꾸어 주기 위해 Override한다.
BOOL CCMStorGrid_IGen::OnInsertEmptyRecord(ROWCOL nRow)
{
	if (!CTBCommon::OnInsertEmptyRecord(nRow)) return FALSE;

	ROWCOL nNameCol, ncRow, ncCol;
	BOOL bCurSel = GetCurrentCell(ncRow, ncCol);
	ASSERT(bCurSel);
	if (!bCurSel) return TRUE;
	GetColumnNoFor(1, nNameCol);
	if (ncCol == nNameCol) return TRUE; // Name 필드를 수정하고 있으면 그냥 리턴

	int no=1;
	CString csStoryName;
	csStoryName.Format(_LS(IDS_WG_CMD__ADD2__New_d), no++);
	while (m_pDoc->m_pAttrCtrl->GetStorKey(csStoryName))
	{
		csStoryName.Format(_LS(IDS_WG_CMD__ADD2__New_d), no++);
	}
	SetValueRange(CGXRange(nRow, nNameCol), csStoryName);

	return TRUE;
}

