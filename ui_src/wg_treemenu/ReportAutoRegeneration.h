#if !defined(__REPORTAUTOREGENERATION_H__)
#define __REPORTAUTOREGENERATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportAutoRegeneration.h : header file
//
#include <map>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CReportAutoRegeneration

#define D_REPORT_TEXT_CNT   22 //23 Response Spectrum 제외.

#include "HeaderPre.h"
class CTreeReportCtrl;

enum LiveLoadType
{
		LiveLoadType_ALL,
		LiveLoadType_MAX,
		LiveLoadType_MIN,
};
enum LiveCaseType
{
	LCT_LoadCombType = 0,
	LCT_SettlementType,
	LCT_MovingloadType,
	LCT_StaticLoadType,
};
struct sLoadCombIDType 
{
	BOOL m_bLoadCase;
	int m_iKeyID;
	int m_LoadCaseType;
	LiveLoadType m_LiveLoadType;
	CString m_strLoadCombName;
	sLoadCombIDType(BOOL _bLoadCase= FALSE,int _iKeyID= 0,int _LoadCaseType =0,LiveLoadType _LiveLoadType= LiveLoadType_ALL,const CString& _strLoadCombName=_T(""))
	{
		m_bLoadCase = _bLoadCase;
		m_iKeyID = _iKeyID;
		m_LiveLoadType = _LiveLoadType;
		m_LoadCaseType = _LoadCaseType;
		m_strLoadCombName = _strLoadCombName;
	}

};
class __MY_EXT_CLASS__ CChinaReportSetting
{
public:
	static CChinaReportSetting* GetChinaReportSetting();
	~CChinaReportSetting();

	BOOL GetChecked(const CString& strItem) const;
	void ModifyCheck( const CString& strItem,BOOL bCheck );
	void AddCheck( const CString& strItem,BOOL bCheck );
	BOOL IsExistItem( const CString& strItem ) const;
	BOOL IsWholeCalProcess() const;
	void SetWholeCalProcess( BOOL bCheck );
	void GetStandradSectKey(CArray<int,int>& sectkey) const;
	void SetStandradSectKey(const CArray<int,int>& sectkey );
	void GetStandradElemKey(CArray<T_GRUP_K,T_GRUP_K>& Elemkey) const;
	void SetStandradElemKey(const CArray<T_GRUP_K,T_GRUP_K>& Elemkey );
	void GetStandradLoadCombKey(CArray<sLoadCombIDType,sLoadCombIDType>& _StandradLoadCombKey) const;
	void SetStandradLoadCombKey(const CArray<sLoadCombIDType,sLoadCombIDType>& _StandradLoadCombKey);
	void GetStandradForcetype(CArray<int,int>& sectkey) const;
	void SetStandradForcetype(const CArray<int,int>& sectkey );

	BOOL GetOutPutElemList(CArray<T_ELEM_K,T_ELEM_K>& elemList);
protected:
	CChinaReportSetting();
private:
	std::map<CString,BOOL> m_mapItems;
	BOOL m_bWholeCalProcess;
	CArray<int,int> m_StandradSectKey;
	CArray<T_GRUP_K,T_GRUP_K> m_StandradElemKey;
	CArray<sLoadCombIDType,sLoadCombIDType> m_StandradLoadCombKey;
	CArray<int,int> m_StandradForcetypeID;
};


class __MY_EXT_CLASS__ CReportAutoRegeneration
{
	// Construction
public:
	CReportAutoRegeneration(CTreeReportCtrl* pReportTree = nullptr);
	~CReportAutoRegeneration();
	
	void  AutoRegeneration();
	void  BatchInsertWithoutEditor();  // for test
	void  BatchOut();  // for test
	//void  WordDllCall(); // word call
	//void  FileSave(); // 일괄출력 시 Save 하는 함수
	//void  WordQuit();		// word quit & Dll freeload
	int  IsUsedText(CString strTextItem); // Text Item 존재여부 1:항목있음, 2:항목없음
	void FilterFileName(CString& strFileName);  // 파일 이름에 (\, /, :, *, ?, ", <, >, |)를 사용할 수 없으므로, 이러한 문자가 있을시에 없애줌.
	
	void AutoGenerateChinaReport();
protected:
	CString ReplaceSymbolByTempChar(LPCTSTR lpStr);
	CString GetModelFullPathName();
	void MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath);
	void AutoUpdateChinaReport();
protected:
	// CTreeReportCtrl도 같이 수정할 것!
	CStringArray  m_aReportTextCaption;  // Workstree에 보여질 Text List 이름
	CStringArray  m_aReportTextStrName;  // 실제 Word에 보내질 Text List 추출정보이름
	
	//HINSTANCE m_hMod;
	//HWND  m_bDllResult;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTAUTOREGENERATION_H__)
