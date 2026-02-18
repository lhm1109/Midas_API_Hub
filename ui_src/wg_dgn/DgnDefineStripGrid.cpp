// DgnDefineStripGrid.cpp : implementation file
//

#include "stdafx.h"
// #include "wg_dgn.h"
// #include "DgnDefineStripGrid.h"
// #include "DgnDefineStripDlg.h"
// 
// #include "..\wg_db\wg_db_DBDoc.h"
// #include "..\wg_db\wg_db_DBLib.h"
// #include "..\wg_db\wg_db_AttrCtrl.h"
// #include "..\wg_db\wg_db_AttrCtrl2.h"
// #include "..\wg_db\wg_db_PostCtrl.h"
// #include "..\wg_db\wg_db_UnitCtrl.h"
// #include "..\wg_db\wg_db_DataCtrl.h"
// #include "..\wg_base\PL_Parser.h"
// 
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
// 
// #define NAME_COL_ID  2
// #define MSTR_KEY_FMT "%d"
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnDefineStripGrid
// CString CDgnDefineStripGrid::m_aCurDefVal[] = {"", "", "", "", "", "", "", "", ""};
// 
// 
// CDgnDefineStripGrid::CDgnDefineStripGrid()
// {
//   m_pDoc = CDBDoc::GetDocPoint();
// 
//   m_MstrData.InitHashTable(101);
// 
//   m_nCurrentRow = 0;
// 
//   m_nColCount = 9;
//   m_nColHeader = 0;
//   m_nSortCount = 0;
// 
//   T_MSTR_UNIT * pUnit = &(CUnitCtrl::m_MSTR_UNIT);
// 
//   AddColInfo(_LSX(key)                     , t_integer, D_UNITSYS_NONE);
//   AddColInfo(_LS(IDS_DGN_STRIP_GRID_NO  )  , t_integer, D_UNITSYS_NONE);
//   AddColInfo(_LS(IDS_DGN_STRIP_GRID_NAME)  , t_string , D_UNITSYS_NONE);
//   AddColInfo(_LS(IDS_DGN_STRIP_GRID_NODE)  , t_string , D_UNITSYS_NONE);
//   AddColInfo(_LS(IDS_DGN_STRIP_GRID_I_NODE), t_real   , pUnit->adWidth);
//   AddColInfo(_LSX(Temp1)                   , t_real   , D_UNITSYS_NONE);
//   AddColInfo(_LS(IDS_DGN_STRIP_GRID_J_NODE), t_real   , pUnit->adWidth);
//   AddColInfo(_LSX(Temp2)                   , t_real   , D_UNITSYS_NONE);
//   AddColInfo(_LSX(SortNum)                 , t_integer, 0); // 그리드에서 입력된 순서대로 정렬하기 위한 변수
// 
//   m_aPrimaryKey.Add(0);
// }
// 
// CDgnDefineStripGrid::~CDgnDefineStripGrid()
// {
// }
// 
// #define CWnd CTBCommon
// BEGIN_MESSAGE_MAP(CDgnDefineStripGrid, CWnd)
// #undef CWnd
//   //{{AFX_MSG_MAP(CDgnDefineStripGrid)
//   ON_WM_RBUTTONDBLCLK()
//   ON_WM_LBUTTONDOWN()
//   ON_WM_LBUTTONDBLCLK()
//   //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
// 
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnDefineStripGrid message handlers
// void CDgnDefineStripGrid::Initialize(CDgnDefineStripDlg* pParent)
// {
//   CTBBrowserWndEx::Initialize();
// 
//   m_pParent = pParent;
//   GetParam()->EnableUndo(TRUE);
//   EnableHints();
//   GetParam()->EnableTrackRowHeight(FALSE); 
//   T_UNIT_SYSTEM UnitSystem;
// 
//   CTBCommon::m_pDoc = m_pDoc;
//   LockUpdate(TRUE);
// 
//   // 컬럼 갯수 설정
//   SetColCount(GetInitialColCount());
// 
//   // 행2
//   CustomizeHeader(); 
// 
//   SetHeaderTitle(UnitSystem, TRUE);
// 
//   // 표준 스타일 변경
//   ChangeStandardStyle(CGXStyle()
//     .SetFont(CGXFont().SetSize(9)));
// 
//   // 컬럼 스타일 변경
//   // Key column의 스타일 변경
//   SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
//     .SetHorizontalAlignment(DT_RIGHT)
//     .SetValueType(GX_VT_NUMERIC)
//     .SetFormat(GX_FMT_FIXED)
//     .SetPlaces(0)
//     .SetEnabled(FALSE));
// 
//   SetStyleRange(CGXRange().SetCols(3, 4), CGXStyle()
//     .SetHorizontalAlignment(DT_CENTER));
// 
//   SetStyleRange(CGXRange().SetCols(5, 8), CGXStyle()
//     .SetHorizontalAlignment(DT_RIGHT));
// 
//   SetRowHeight(0, 0, 18);
// 
//   // Key, Serial No Column은 숨긴다.
//   // 주1 참조
//   HideCols(1, 1);
//   HideCols(9, 9);
// 
//   // 현재 셀의 border 설정 변경
//   CGXProperties* pProp = GetParam()->GetProperties();
//   pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
//     (CGXStyle) pProp->sInvertNoBorder);
// 
//   // 현재 셀 설정
//   SetRightValidCell(1, 1);
// 
//   // Sorting 정보 초기화 : 이름순으로 정렬
//   m_aSortInfo.SetSize(2);
//   m_aSortInfo[0].nRC = 2; // name
//   m_aSortInfo[0].sortType = CGXSortInfo::alphanumeric;
//   m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
//   m_aSortInfo[1].nRC = 8; // SortNum
//   m_aSortInfo[1].sortType = CGXSortInfo::numeric;
//   m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;
// 
//   // Column 크기 설정
//   SetColWidth(1, 1, 0);
//   SetColWidth(2, 2, 30);
//   SetColWidth(3, 3, 73);
//   SetColWidth(4, 4, 60);
//   SetColWidth(5, 8, 60);
//   SetColWidth(9, 9, 0);
// 
//   MakeItemEx();
//   LockUpdate(FALSE);
//   HideRows(GetRowCount(), GetRowCount());
// }
// 
// void CDgnDefineStripGrid::MakeItemEx()
// {
//   CWaitCursor wait;
// 
//   HideRows(GetRowCount(), GetRowCount());
//   BOOL bOldLock = LockUpdate(TRUE);
// 
//   CArray<T_MSTR_K, T_MSTR_K> aMstrK;
//   m_pDoc->m_pAttrCtrl2->GetMstrKeyList(aMstrK);
//   T_MSTR_D MstrD;
// 
//   int i;
//   for(i=0; i<aMstrK.GetSize(); i++)
//   {
//     m_pDoc->m_pAttrCtrl2->GetMstr(aMstrK[i], MstrD);
//     InsertRecordByKey(aMstrK[i], MstrD);
//   }
//   ResetNoColumn();
// 
//   LockUpdate(bOldLock);
//   if(!bOldLock) Redraw();  
// }
// 
// BOOL CDgnDefineStripGrid::InsertRecordByKey(T_MSTR_K &key, T_MSTR_D &data)
// {
//   // 삽입 위치를 찾는다.
//   ROWCOL nRow;
// 
//   // aKey를 만드는 코드 삽입
//   CStringArray aKey;
//   MakeSearchKey(key, data, aKey);
//   FindRecordByKey(aKey, nRow);
// 
//   VERIFY(InsertRecord(nRow));
//   return SetValue(nRow, key, data);
// }
// 
// BOOL CDgnDefineStripGrid::DeleteRecordByKey(T_MSTR_K &key, T_MSTR_D &data)
// {
//   // 레코드를 찾는다.
//   ROWCOL nRow;
// 
//   // aKey를 만드는 코드 삽입
//   CStringArray aKey;
//   MakeSearchKey(key, data, aKey);
//   if (!FindRecordByKey(aKey, nRow)) return FALSE;
// 
//   // 삭제한다.
//   return DeleteRecord(nRow);
// }
// 
// void CDgnDefineStripGrid::UnitChanged(CDBDoc* pDoc)
// {
//   ReInitialize();
//   return;
// }
// 
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnDefineStripGrid Overridable Functions
// void CDgnDefineStripGrid::OnChangedSelectionPublic(long *keys, int nSize)
// {
//   if (nSize <= 0)
//   {
//     m_aSelectedKeys.RemoveAll();
//   }
//   else
//   {
//     m_aSelectedKeys.RemoveAll();
//     for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_MSTR_K)keys[i]);
//   }
// 
//   //m_pParent->Data2Dlg();
// }
// 
// BOOL CDgnDefineStripGrid::ValidateField(CString value, int nColID)
// {
//   if (nColID >= GetColCount())
//   {
//     SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
//     return FALSE;
//   }
// 
//   return TRUE;
// }
// 
// // 삽입이나 수정전에 Record의 Validation을 검사한다.
// BOOL CDgnDefineStripGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
// {
//   CString msg = "";
//   int nPos;
// 
//   for (int i = 2; i < GetColCount()-1; i++)
//   {
//     nPos = aCols[i]-1;
//     if (value[nPos] == "")
//       msg += ((msg != "") ? "\n" : "") + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
//   }
// 
//   // Node List 검사
//   CArray<UINT, UINT> aNodeList;
//   if (!m_pParent->GetNodeList(value[3], aNodeList))
//   {
//     GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_list_));
//     return FALSE;
//   }
//   if(!m_pParent->CheckStripNode(aNodeList)) return FALSE;
// 
//   if (msg == "") return TRUE;
//   else SetWarningText(msg);
//   return FALSE;
// }
// 
// // 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// // Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
// BOOL CDgnDefineStripGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_MSTR_K &key, T_MSTR_D &data)
// {
//   // 각 필드에 대해 Validation을 검사한다.
//   for (int i = 0; i < value.GetSize(); i++)
//   {
//     if (!ValidateField(value[aCols[i]-1], i)) 
//     {
//       SetWarningText("");
//       return FALSE;
//     }
//   }
//   // 레코드에 대해 Validation을 검사한다.
//   if (!ValidateRecord(value, aCols))
//   {
//     SetWarningText("");
//     return FALSE;
//   }
// 
//   // 레코드를 만들어서 리턴한다.
//   int nPos;
//   for (i = 0; i < GetColCount(); i++)
//   {
//     nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
//     if(i==0) 
//     {
//       if (value[nPos] == "") key = 0;
//       else key = atol(value[nPos]);
//     }
//     else ConvStrToData(i, value[nPos], data);
//   }
//   return TRUE;
// }
// 
// BOOL CDgnDefineStripGrid::AddToDB(ROWCOL nRow)
// {
//   // Record를 만들어서 DB에 입력한다.
//   T_MSTR_K key;
//   T_MSTR_D data;
// 
//   GetValue(nRow, key, data);
// 
//   return true;
// }
// 
// BOOL CDgnDefineStripGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
// {
//   ASSERT(raKey.GetSize() == raData.GetSize());
//   T_MSTR_K tKey;
//   CArray<T_MSTR_K, T_MSTR_K> aKey;
//   CArray<T_MSTR_D, T_MSTR_D&> aData;
//   for (int i = 0; i < raKey.GetSize(); i++)
//   {
//     tKey = *((T_MSTR_K*)raKey.GetAt(i));
//     aKey.Add(tKey);
//     aData.Add(*((T_MSTR_D*)raData.GetAt(i)));
//   }
// 
//   return true;
// }
// 
// 
// BOOL CDgnDefineStripGrid::DeleteFromDB(ROWCOL nRow)
// {
//   T_MSTR_K key;
//   T_MSTR_D data;
// 
//   GetValue(nRow, key, data);
// 
//   return true;
// }
// 
// BOOL CDgnDefineStripGrid::DeleteFromDB(CRowColArray& awRows)
// {
//   if (awRows.GetSize() <= 0) return TRUE;
// 
//   // _LS(IDS_WG_CMD__ADDD__Name) 칼럼을 얻는다.
//   ROWCOL nCol;
//   VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));
// 
//   CString value;
//   CArray<CString, CString&> awKeys;
// 
//   for (int i = 0; i < awRows.GetSize(); i++)
//   {
//     value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
//     awKeys.Add(value);
//   }
// 
//   return TRUE;  
// }
// 
// BOOL CDgnDefineStripGrid::ModifyToDB(ROWCOL nRow)
// {
//   ROWCOL nCol;
//   VERIFY(GetColumnNoFor(2, nCol));  // load case name
// 
//   T_MSTR_K key;
//   T_MSTR_D data;
//   CString pOldLoadCaseName;
// 
//   pOldLoadCaseName = GetValueRowColBeforeEdit(nRow, nCol);
//   GetValue(nRow, key, data);
// 
//   return true;
// }
// 
// BOOL CDgnDefineStripGrid::ModifyToDB(CStringArray& raOldKey, 
//                                      CArray<void*, void*>& raKey, 
//                                      CArray<void*, void*>& raData)
// {
//   ASSERT(raKey.GetSize() == raData.GetSize());
//   ASSERT(raKey.GetSize() == raOldKey.GetSize());
//   CArray<CString, CString&> aOldKey;
//   CArray<T_MSTR_D, T_MSTR_D&> aData;
//   T_MSTR_K Key;
//   T_MSTR_D Data;
//   for (int i = 0; i < raKey.GetSize(); i++)
//   {
//     Key = atol(raOldKey.GetAt(i));
// 
//     aData.Add(*((T_MSTR_D*)raData.GetAt(i)));
//   }
// 
//   return true;
// }
// 
// 
// CString CDgnDefineStripGrid::GetNewKey()
// {
//   CString value;
//   T_MSTR_K key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
// 
//   value.Format(MSTR_KEY_FMT, key);
//   return value;
// }
// 
// CString CDgnDefineStripGrid::GetDefValue(int nIndex)
// {
//   return m_aCurDefVal[nIndex];
// }
// 
// CString* CDgnDefineStripGrid::GetDefValueArray()
// {
//   return m_aCurDefVal;
// }
// 
// BOOL CDgnDefineStripGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
//                                               CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
// {
//   T_MSTR_K* pKey;
//   T_MSTR_D* pData;
//   pKey = new T_MSTR_K;
//   pData = new T_MSTR_D;
//   aKey.Add((void*)pKey);
//   aData.Add((void*)pData);
//   if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
//   return TRUE;
// }
// 
// void CDgnDefineStripGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
// {
//   T_MSTR_K* pKey;
//   T_MSTR_D* pData;
//   ASSERT(aKey.GetSize() == aData.GetSize());
//   for (int i = 0; i < aKey.GetSize(); i++)
//   {
//     pKey = (T_MSTR_K*)aKey.GetAt(i);
//     pData = (T_MSTR_D*)aData.GetAt(i);
//     delete pKey;
//     delete pData;
//   }
// }
// 
// long CDgnDefineStripGrid::GetCountData()
// {
//   CDBDoc* pDoc = m_pDoc;
//   return pDoc->m_pAttrCtrl->GetCountElem();  
// }
// 
// void CDgnDefineStripGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
// {
//   T_MSTR_D Data;
// 
//   for(long nCount=0 ; nCount < nFilteredNum; nCount++)
//   {
//     SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
//   }
// }
// 
// void CDgnDefineStripGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
// {
//   //T_MSTR_K Key;
//   T_MSTR_D Data;
//   CDBDoc* pDoc = m_pDoc;
// 
//   long* KeyBuf = new long[nNumData];
//   long nCount = 0;
// 
//   POSITION pos = pDoc->m_pAttrCtrl->GetStartElem();
//   while(pos != NULL)
//   {
// 
//   }
//   qsort(KeyBuf, nNumData, sizeof(T_MSTR_K), CTBBrowserWnd::CompareKey);
// 
//   for(nCount=0 ; nCount < nNumData; nCount++)
//   {
//     SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
//   } // end of loop nCount
// 
//   delete []KeyBuf;
// }
// 
// void CDgnDefineStripGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
// {
//   T_MSTR_K Key;
//   T_MSTR_D Data;
//   for (int i = 0; i < GetColCount(); i++)
//   {
//     if (i == 0) Key = atol(EditingRecord[i]);
//     else ConvStrToData(i, EditingRecord[i], Data);
//   }  
//   // 현재 값을 설정한다.
//   SetValue(ncRow, Key, Data);
// }
// 
// /////////////////////////////////////////////////////////////////////////////
// // CDgnDefineStripGrid Implementation
// BOOL CDgnDefineStripGrid::GetValue(ROWCOL nRow, T_MSTR_K &key, T_MSTR_D &data)
// {
//   ROWCOL nCol;
//   CString str;
//   for (int i = 0; i < GetColCount(); i++)
//   {
//     VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
//     str = GetValueRowCol(nRow, nCol);
//     // 수식인 경우 값으로 변환한다.
//     // Validation이 OK인 Field이므로 항상 TRUE 리턴
//     VERIFY(ConvertFormulaToValue(str)); 
// 
//     if(i==0) 
//     {
//       key = atol(str);
//     }
//     else ConvStrToData(i, str, data);
//   }
//   return TRUE;
// }
// 
// BOOL CDgnDefineStripGrid::SetValue(ROWCOL nRow, T_MSTR_K key, T_MSTR_D &data)
// {
//   CRowColArray awCols;
//   awCols.SetSize(GetColCount());
//   for (int i = 0;i < GetColCount(); i++)
//     VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
//   return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
// }
// 
// BOOL CDgnDefineStripGrid::SetValue(CGXData* pData, ROWCOL nRow, T_MSTR_K key, T_MSTR_D &data, CRowColArray &awCols)
// {
//   CString str;
//   for (int i = 0; i < GetColCount(); i++)
//   {
//     if(i==0) 
//     {
//       if (key == 0) str.Empty();
//       else str.Format(MSTR_KEY_FMT,key);
//     }
//     else if(i==1)
//     {
//       str.Format("%d", nRow);
//     }
//     else ConvDataToStr(i, key, data, str);
//     pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
//   }
// 
//   return TRUE;
// }
// 
// void CDgnDefineStripGrid::ConvStrToData(int i, CString& value, T_MSTR_D& data)
// {
//   if (i == 1) i;    // module name // do nothing
//   else if (i == 2) data.strName = value;
//   else if (i == 3) 
//   {
//     MyParser Parser;
//     CArray<UINT,UINT> aNodeK;
//     Parser.ParsingNodeList(value,aNodeK);
//     data.anNodeK[0] = aNodeK[0];
//     data.anNodeK[1] = aNodeK[1];
//   }
//   else if (i == 4) data.adWidth[0] = atof(value);
//   else if (i == 5) data.adWidth[1] = atof(value);
//   else if (i == 6) data.adWidth[2] = atof(value);
//   else if (i == 7) data.adWidth[3] = atof(value);
// }
// 
// void CDgnDefineStripGrid::ConvDataToStr(int i, T_MSTR_K key, T_MSTR_D& data, CString& value)
// {
//   switch(i)
//   {
//   case 2:
//     value = data.strName;
//     break;
//   case 3:
//     value.Format("%d, %d", data.anNodeK[0], data.anNodeK[1]);
//     break;
//   case 4:
//     value.Format("%g", data.adWidth[0]);
//     break;
//   case 5:
//     value.Format("%g", data.adWidth[1]);
//     break;
//   case 6:
//     value.Format("%g", data.adWidth[2]);
//     break;
//   case 7:
//     value.Format("%g", data.adWidth[3]);
//     break;
//   case 8:
//     value.Format("%d", m_nSortCount++);
//     break;
//   default:
//     ASSERT(0);
//     break;
//   }	
// }
// 
// void CDgnDefineStripGrid::MakeSearchKey(T_MSTR_K &key, T_MSTR_D &data, CStringArray &aKey)
// {
//   int nColID;
//   int nSortKeyNum = m_aSortInfo.GetSize();
// 
//   aKey.SetSize(nSortKeyNum);
//   for (int i = 0; i < nSortKeyNum; i++)
//   {
//     nColID = m_aSortInfo[i].nRC;
//     if (nColID == 0) aKey[i].Format(MSTR_KEY_FMT, key);
//     else ConvDataToStr(nColID, key, data, aKey[i]);
//   }
// }
// 
// void CDgnDefineStripGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
// {
//   SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
// }
// 
// 
// // Hide 된 Row들이 resize되지 않도록 한다.
// // 주1 참조
// BOOL CDgnDefineStripGrid::OnTrackColWidth(ROWCOL nCol)
// {
//   if (nCol == 1) return FALSE;
//   return CTBCommon::OnTrackColWidth(nCol);
// }
// 
// // TBBrowserWndEx::OnSelDragColsDrop 참조
// // 부모를 먼저 호출하면 안 된다.
// // 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// // 이동되는 것을 금지한다.
// // 주1 참조
// BOOL CDgnDefineStripGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
// {
//   // Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
//   //if (IsFrozenCol(nDestCol)) return FALSE;
//   if (nDestCol == 1 || nFirstCol == 1) return FALSE;
//   if (nDestCol == 2 || nFirstCol == 2) return FALSE;
//   return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
// }
// 
// // 키와 배열을 넘겨주면 그리드를 갈아엎는다.
// void CDgnDefineStripGrid::ResetGridWithData(CArray<T_MSTR_K,T_MSTR_K>& aMstrK, CArray<T_MSTR_D,T_MSTR_D>& aMstrD)
// {
//   ASSERT(aMstrK.GetSize() == aMstrD.GetSize());
// 
//   DeleteAllRecords();
//   HideRows(GetRowCount(), GetRowCount());
//   for(int i=0; i<aMstrK.GetSize(); i++)
//   {
//     InsertRecordByKey(aMstrK[i], aMstrD[i]);
//   }
//   ResetNoColumn();
//   Redraw();
// }
// 
// void CDgnDefineStripGrid::AddFromDlg(T_MSTR_K key, T_MSTR_D& data)
// {
//   InsertRecordByKey(key, data);
//   ResetNoColumn();
//   Redraw();
// }
// 
// void CDgnDefineStripGrid::DelFromDlg()
// {
//   ROWCOL ncRow, ncCol;
//   GetCurrentCell(ncRow, ncCol);
// 
//   T_MSTR_K MstrK;
//   T_MSTR_D MstrD;
//   GetValue(ncRow, MstrK, MstrD);
//   DeleteRecord(ncRow);
//   ResetNoColumn();
//   Redraw();
// }
// 
// // No.열은 번호가 1부터 가지런히 찍혀야 된다고 스펙에 나와있다.
// // Add, Delete 될 때 요걸 불러주자.
// void CDgnDefineStripGrid::ResetNoColumn()
// {
//   CString str;
//   int i;
//   for (i=1; i<GetRowCount(); i++)
//   {
//     str.Format("%d", i);
//     GetParam()->GetData()->StoreValueRowCol(i+1, 2, str, gxOverride, GX_VT_NUMERIC);
//   }
// }
// 
// // 그리드의 정보를 넘겨준다
// void CDgnDefineStripGrid::GetMstrKeyDataFromGrid(CArray<T_MSTR_K, T_MSTR_K>& rKeyList, CArray<T_MSTR_D, T_MSTR_D>& rDataList)
// {
//   rKeyList.RemoveAll();
//   rDataList.RemoveAll();
// 
//   int nSize = GetRowCount()-2;
//   rKeyList.SetSize(nSize);
//   rDataList.SetSize(nSize);
// 
//   for (int i=1; i<=nSize; i++)
//   {
//     GetValue(i+1, rKeyList[i-1], rDataList[i-1]);
//   }
// }
// 
// void CDgnDefineStripGrid::ReInitialize()
// {
//   T_UNIT_SYSTEM UnitSystem;
//   SetHeaderTitle(UnitSystem, FALSE);
// 
//   DeleteAllRecords();
//   MakeItemEx();
//   Redraw();
// }
// 
// // 엔터키가 안먹어서 처리;
// BOOL CDgnDefineStripGrid::PreTranslateMessage(MSG* pMsg) 
// {
//   if(pMsg->message == WM_KEYDOWN)
//   {
//     switch(pMsg->wParam)
//     {
//     case VK_RETURN:
//     case VK_ESCAPE:
//       OnKeyDown(pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
//       break;
//     default:
//       break;
//     }    
//   }
// 
//   return CTBCommon::PreTranslateMessage(pMsg);
// }
// 
// 
// // 헤더를 2줄로 만들기 위한 함수들
// // 다음 라인에 Unit을 붙여주며, aUnitIndex를 입력 받는다.
// void CDgnDefineStripGrid::SetHeaderTitle2(int* aUnitIndex, BOOL bInit, BOOL bSameLine)
// {
//   CString title;
//   int i;
//   ROWCOL nCol;
// 
//   CString csUnit;
//   CString csBegin    = "(";
//   CString csEnd      = ")";
// 
//   // 첫번째 줄
//   int nColCount = GetColCount();
//   for(i = 0; i < nColCount; i++)
//   {
//     title = m_aTitle[i];
//     // 제일 처음 시작일 때면 순서대로
//     // 아니면 Column 위치가 변경되었을 수도 있기 때문에 Query 해서 설정 한다.
//     if (bInit) nCol = i + 1;
//     else VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
// 
//     CUnitCtrl::GetUnitSystem(aUnitIndex[i], csUnit);
//     if (csUnit != "") title += csBegin+csUnit+csEnd;
// 
//     // 컬럼 헤더 이름 및 ID를 설정
//     SetStyleRange(CGXRange(0, nCol), CGXStyle()
//       .SetValue(title)
//       .SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
//   }
// 
//   // 두번째 줄
//   SetStyleRange(CGXRange(1, 5), CGXStyle().SetValue(_LS(IDS_DGN_STRIP_GRID_W_LEFT)));
//   SetStyleRange(CGXRange(1, 6), CGXStyle().SetValue(_LS(IDS_DGN_STRIP_GRID_W_RIGHT)));
//   SetStyleRange(CGXRange(1, 7), CGXStyle().SetValue(_LS(IDS_DGN_STRIP_GRID_W_LEFT)));
//   SetStyleRange(CGXRange(1, 8), CGXStyle().SetValue(_LS(IDS_DGN_STRIP_GRID_W_RIGHT)));
// }
// 
// BOOL CDgnDefineStripGrid::DeleteAllRecords()
// {
//   BOOL bOldLock = LockUpdate(TRUE);
//   RemoveRows(2, GetRowCount());
//   SetRowCount(2);
//   GetBrowseParam()->m_nRecordCount = 0;
//   GetBrowseParam()->m_nCurrentRow = 2;
//   m_nEditMode = noMode;
//   SetRightValidCell(2, 0);
//   LockUpdate(bOldLock);
//   if (!bOldLock) Redraw();
//   return TRUE;
// }
// 
// BOOL CDgnDefineStripGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
// {
//   BOOL bRet = CTBBrowserWndEx::GetStyleRowCol(nRow, nCol, style, mt, nType);
// 
//   // 2번째 행을 헤더 색깔로 변경
//   if(nRow==1)
//   {
//     style.SetInterior(::GetSysColor(COLOR_BTNFACE))
//       .SetTextColor(::GetSysColor(COLOR_BTNTEXT));
//   }
// 
//   return bRet;
// }
// 
// void CDgnDefineStripGrid::CustomizeHeader()
// {
//   VERIFY(InsertRecord(1));
//   SetCoveredCellsRowCol(0,5,0,6);
//   SetCoveredCellsRowCol(0,7,0,8);
//   SetCoveredCellsRowCol(0,0,1,0);
//   SetCoveredCellsRowCol(0,1,1,1);
//   SetCoveredCellsRowCol(0,2,1,2);
//   SetCoveredCellsRowCol(0,3,1,3);
//   SetCoveredCellsRowCol(0,4,1,4);
// 
//   SetStyleRange(CGXRange().SetRows(1), CGXStyle()   
//     .SetFont(CGXFont().SetBold(TRUE))
//     .SetHorizontalAlignment(DT_CENTER)
//     .SetControl(GX_IDS_CTRL_HEADER)
//     .SetEnabled(FALSE));
// 
//   // Width\n(Left) 행은 높이를 늘려준다.
//   SetRowHeight(1, 1, 30);
// }