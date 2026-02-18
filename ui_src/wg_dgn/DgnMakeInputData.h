// DgnMakeInputData.h: interface for the CDgnMakeInputData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNMAKEINPUTDATA_H__)
#define __DGNMAKEINPUTDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

struct _LCASE_DATA
{
	unsigned int nAnalType;
	unsigned int nLcaseKey;

	void Initialize()
	{
		nAnalType = 0;
		nLcaseKey = 0;
	}
	_LCASE_DATA() { Initialize();}
	_LCASE_DATA(_LCASE_DATA& sorData) {*this = sorData;}
	_LCASE_DATA& operator = (_LCASE_DATA& sorData) {
		nAnalType = sorData.nAnalType;
		nLcaseKey = sorData.nLcaseKey;
		return *this;
	}
};

class CDgnForceCtrl;
class __MY_EXT_CLASS__ CDgnMakeInputData
{
public:
	CDgnMakeInputData();
	CDgnMakeInputData(int iLcomType);
	virtual ~CDgnMakeInputData();

public:
	CString  m_strPath;
	int m_iLcomType;
	int m_iMembType; //iMembType=0:All, 1:Column, 2:Brace, 3:Wall, 4:Beam
	BOOL m_bSpecialEQ;//내진특별규정 적용여부.

protected:
	CDBDoc*  m_pDoc;
	CDgnForceCtrl* m_pForceCtrl;
	CArray<T_ELEM_K, T_ELEM_K> m_arBeamList;
	CArray<T_ELEM_K, T_ELEM_K> m_arColumnList;
	CArray<T_ELEM_K, T_ELEM_K> m_arBraceList;
	CArray<T_ELEM_K, T_ELEM_K> m_arWallList;
	CArray<_LCASE_DATA, _LCASE_DATA> m_arLcaseList;
	CMap <T_STOR_K, T_STOR_K, T_ELEM_K, T_ELEM_K> m_arStoryID;//ID, StoryID

	CArray<T_NODE_K, T_NODE_K> m_arNodeList;

protected:
	CStdioFile m_cfile;

public:
	void SetLcomType(int iLcomType);

	BOOL WriteInputData4Dgn(int iMembType=0, HWND pWndSet=NULL);
	//iMembType=0:All, 1:Column, 2:Brace, 3:Wall, 4:Beam
	BOOL SetCommandCtrl(int iCommand=0, int iOption=0);

protected:
	void Write_Data();
	void Write_Node_Data(int iMembType);
	void WriteLoadCase();
	void WriteLoadComb();
	void WriteLoadComb_Description();
	void WriteStoryData();
	void WriteMaterialData();
	void WriteSectData();
	void WriteThickData();
	void WriteForceData();
	void Write_ReactionForce(int iMembType);

	BOOL Write_String(CString strValue);

};

__MY_EXT_API__ void DoTempForHanSangSun(CView *pView);

#include "HeaderPost.h"

#endif // !defined(__DGNMAKEINPUTDATA_H__)
