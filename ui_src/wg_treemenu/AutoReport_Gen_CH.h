
#pragma once
#include "..\wg_gr\GDisplaySetting.h"
struct GenSegmentElements;
struct CH_GEN_IFTAG_STR;
class CTreeReportCtrl;
struct T_TABLE_D;
class CDBDoc;
struct Report_Key;

#include <map>
using namespace std;
#include "HeaderPre.h"

class __MY_EXT_CLASS__  AutoReport_Gen_Base
{
public:
	virtual int  TempletID() = 0;
	virtual BOOL PreMakeData() = 0;
	BOOL AutoGeneration(BOOL bUpDate = FALSE);
protected: 
	virtual void MakeContent() = 0;
	virtual void InsertCoverPage();
	virtual void InsertCatalogPage();
protected:
	CTreeReportCtrl* TreeReportCtrl(); 
	void InsertSegmentElements  (GenSegmentElements &Elems,  BOOL bDeleteTempFile = TRUE);
	void InsertReportItemSSTable  (GenSegmentElements &Elems);
	void DeleteTempFiles        (const GenSegmentElements& segs);
protected:
	void Initial();
	virtual void MakeTitle(GenSegmentElements &genElements, LPCTSTR lpTitle, int iChapterID, int iSegmentID, LPCTSTR lpBookMark = _T(""),int iSubId = 0,int iSubSubId = 0);
	virtual void MakeText (GenSegmentElements &genElements, LPCTSTR lpText, LPCTSTR lpLog = _T(""));

	void MakeTable(GenSegmentElements &genElements, T_TABLE_D &TableD,  LPCTSTR lpCaption, LPCTSTR lpBookMark = nullptr);
	void MakeTableCenterText(GenSegmentElements &genElements, T_TABLE_D &TableD, LPCTSTR lpCaption, LPCTSTR lpBookMark = nullptr);
	void MakePicture(GenSegmentElements &genElements, LPCTSTR lpPicName, LPCTSTR lpBookMark);
	CString ChangeLine();
	CString GetImagePath(LPCTSTR lpPicName);
	BOOL MakeImage_Current(LPCTSTR lpPicName, BOOL bBW = FALSE);
	void AddToText(CString &strText, LPCTSTR strFormate, ...);   
	//D_LOADCASE_
	CString LoadCombActiveDes(UINT LCombType, int nActive);
	CString LoadCombTypeDes(UINT nLcomType, int nType);
	BOOL MakeLoadComb(const Report_Key &LoadK, T_LCOM_D &LComD);
	void  PushDspOpt();
	void  PopDspOpt();
private:
	GDisplaySetting m_dspOpt;
protected:
	//nFillFlag 0 left, 1 middle 2 right
	CString FillBlank(LPCTSTR str, int nTolLength=20, int nFillFlag=1);
	CString Length (double dVal, BOOL AddUnit = FALSE, int nPos = -1); //L
	CString Stress (double dVal, BOOL AddUnit = FALSE, int nPos = -1); //F / L2
	CString Density(double dVal, BOOL AddUnit = FALSE, int nPos = -1); //F / L3
	CString Area   (double dVal, BOOL AddUnit = FALSE, int nPos = -1); //L2
	CString Force  (double dVal, BOOL AddUnit = FALSE, int nPos = -1); //F
	CString Moment (double dVal, BOOL AddUnit = FALSE, int nPos = -1); //F*L
	CString UnitDes(int nUnitType, BOOL bAddBrace = FALSE);
	CString ToString(int nVal);
	CString ToString(double dVal, int nPos);
	CString BookMark(LPCTSTR strName, int idx);
	CString CellRltDes(BOOL bOK);
	//
	void Replace_StrPath(CString& str);
	// 0 AbsMax 1 Max 2 Min 3 AbsMin
	double  CompVal(double dVal1, double dVal2, int nCompType=0);
	int     ForceDot(LPCTSTR strUnit);
	int     LengthDot(LPCTSTR strUnit);
	int     Dot(int nUnitType);
	void    MergeCellCol(T_TABLE_D &TableD, int nCol, int nRow, int nNum);
	void    MergeCellRow(T_TABLE_D &TableD, int nCol, int nRow, int nNum);
	void    MergeHeadCol(T_TABLE_D &TableD, int nCol, int nRow, int nNum);
	void    MergeHeadRow(T_TABLE_D &TableD, int nCol, int nRow, int nNum);
	void	MakeAlignText(CString& strContent, CString&& str1, CString&& str2, int n  = 40);
protected:
	//0 ISO 1-6 Mx, My, Mz, Px, Py, Pz
	void SetViewPoint(int nFlag);
	void SetViewPoint(double xyzNormal[3]);
	void SetShape(BOOL bShape);
	void SetPreOrPostMode(BOOL bPost);
	void ActiveElemAll(BOOL bActive = TRUE);
	void DrawNodeAll(BOOL bDraw = TRUE);
	void ActiveObj(const T_KEY_LIST &Elems, BOOL bActive = TRUE, const T_KEY_LIST*lsNodes = nullptr);
	int  FilteElemByType(T_KEY_LIST &Elems, int nType /*BEAM_EL*/);
	int  FilteElemByMembType(T_KEY_LIST &Elems, int nType  /*D_MBTP_BEAM*/);
	//
	BOOL CalcSlabVector(double[3] ,T_KEY_LIST &lstElems);//False为elems不在同一平面,取3d视图;TRUE 为elems的法向量
	void AdjustSlabView(T_KEY_LIST& lstElems);
	BOOL IsVaildLoadCom(T_KEY key, int nType);//nType 0 是板的承载力; 1 是使用性验算
protected:
	CString GetBold(const CString& str);
	CString GetBold(int nValue);
	CString GetBold(double dValue,int n = 2);
	CString Double2String_g(double dValue, int n = 2);
	CString Double2String_f(double dValue, int n = 2);
	void MakeCompleteText(CString& Elems, const CString& str1, int n = 4);
protected:
	//[STOP] [RESUME] [STYLE]
	void styleSetUp(LPCTSTR sStype);
protected:
	CDBDoc	*m_pDoc;
	T_UNIT_NAMEFACT m_UnitName;
	T_UNIT_INDEX    m_UnitIndex;
	UINT			m_iAppendFlag;
};
#include "HeaderPost.h"