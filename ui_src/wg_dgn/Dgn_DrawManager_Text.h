// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_DRAWMANAGER_TEXT_H__)
#define __DGN_DRAWMANAGER_TEXT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\DgnDrawBase\DgnDrawBase_EtcDraw_Struct.h"
#include "..\dgnengine\src\DgnDrawBase\DgnDrawBase_DrawCtrl_Struct.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgn_DrawManager_Text
{
public:
	CDgn_DrawManager_Text(void);
	~CDgn_DrawManager_Text(void);

public:
	void Draw_DgnText(CDC* pDC, CArray<T_DGN_CHART_FORMAT,T_DGN_CHART_FORMAT>* pDrawFormat, CArray<T_DGN_CHART_GROUP,T_DGN_CHART_GROUP>* dgnChartGroup);
	void SetIsEMF(BOOL bEMF) { m_bEMF=bEMF; }

protected:
	void Draw_Text(CDC* pDC, T_DGN_TEXT_FORMAT &textFormat, T_DGN_TEXT &dgnText);  
	void Draw_Text(CDC* pDC, T_DGN_TEXT_FORMAT &textFormat, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arText);
	void Draw_Text(CDC* pDC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, CString strFont, DGN_POSITION_TYPE enPosiType, T_DGN_TEXT &dgnText, int nEscapement = 0, BOOL bHalfWidth = FALSE);
	void Draw_Text(CDC* pDC, COLORREF fontColor, COLORREF fillColor, BOOL bFill, int fontSize, CString strFont, DGN_POSITION_TYPE enPosiType, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, int nEscapement = 0, BOOL bHalfWidth = FALSE);
	void ConvertNextLineText(int fontSize, DGN_POSITION_TYPE enPosiType, int nEscapement, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng);
	void ConvertMathText(CDC* pDC, int fontSize, int fontSize_sub, DGN_POSITION_TYPE enPosiType, int nEscapement, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextOrg, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng, CArray<T_DGN_TEXT,T_DGN_TEXT&>& arTextCng_sub);

	BOOL m_bEMF;
};


#include "HeaderPost.h"

#endif // !defined(__DGN_DRAWMANAGER_TEXT_H__)
