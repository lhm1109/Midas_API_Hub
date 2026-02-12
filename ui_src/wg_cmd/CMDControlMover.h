#if !defined(__CMTENDON_DLG_CTRL_MOVER_H__)
#define      __CMTENDON_DLG_CTRL_MOVER_H__

// Old Version : 5.1.0부터 CCMTendonProfileDlg.cpp로 변경

#if _MSC_VER > 1000
#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDControlMover

struct CM_CTRL_MOVER_RATIO
{
	int nVertSize; //100이면 크기가 변한만큼 변한다.
	int nHorzSize;
	int nVertMove;
	int nHorzMove;

	void Init()
	{
		nVertSize = 100;
		nHorzSize = 100;
		nVertMove = 100;
		nHorzMove = 100;
	}
};

typedef CM_CTRL_MOVER_RATIO cmRatio;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMDControlMover
{

public:
	CCMDControlMover(CWnd* pParent);   // standard constructor
	virtual ~CCMDControlMover();

	void AddCtrl2Move(UINT uiCtrlId, int nVertMove = 100, int nHorzMove = 1.0, int nVertSize = 100, int nHorzSize = 100);
	void AddCtrl2Move(UINT uiCtrlId, const CM_CTRL_MOVER_RATIO& Ratio);
	void RemoveAll();
	void OnSizeChanged();

private:
	void MoveCtrl(int nIdx, long lChangeVert, long lChangeHorz);

private:
	CArray<UINT, UINT>        m_caCtrlID;
	CArray<cmRatio, cmRatio>  m_caCtrlRatio;

	CWnd*     m_pParent;
	BOOL      m_bParentOK;
	CRect     m_rectParentPre;
};


#include "HeaderPost.h"

#endif
