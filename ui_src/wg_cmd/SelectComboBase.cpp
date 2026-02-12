// SelectComboBase.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SelectComboBase.h"

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectComboNode
CString CSelectComboNode::m_sText = _T("");
CArray<CSelectComboNode*, CSelectComboNode*> CSelectComboNode::m_aInstances;

CSelectComboNode::CSelectComboNode()
{
	// 생성될 때 Instance 배열에 추가된다.
	m_aInstances.Add(this);
}

CSelectComboNode::~CSelectComboNode()
{
	// 소멸될 때 Instance 배열에서 삭제된다.
	for (int i = 0; i < m_aInstances.GetSize(); i++)
	{
		if (m_aInstances.GetAt(i) == this)
		{
			m_aInstances.RemoveAt(i);
			break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSelectComboNode Static members
void CSelectComboNode::Initialize()        // Document가 생성될 경우 호출
{
	m_sText = _T("");
}

void CSelectComboNode::UpdateSelection()   // Selection이 변경될 경우 호출
{
	// Optimized Text를 초기화 한다.
	m_sText = GetOptimizedString();

	// 실제 combo instance들이 update하도록 함수 호출
	CSelectComboNode* pSelectCombo;
	for (int i = 0; i < m_aInstances.GetSize(); i++)
	{
		pSelectCombo = m_aInstances.GetAt(i);
		pSelectCombo->UpdateSelectionText();
	}
}

// m_pDoc을 static member로 저장하고 이걸 리턴하도록 만들 수 도 있다.
CDBDoc* CSelectComboNode::GetDocument()
{
	CAppBase* pApp = (CAppBase*)AfxGetApp();
	if (!pApp) return 0;

	CDBDoc* pDoc = (CDBDoc*)pApp->GetDocument();
	return pDoc;
}

CString CSelectComboNode::GetOptimizedString()
{
	CDBDoc* pDoc = GetDocument();
	if (!pDoc) return _T("");

	// Select된 Key를 가져온다.
	CArray<T_NODE_K, T_NODE_K> aKey;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aKey);
	long nSize = aKey.GetSize();
	if (nSize == 0) return _T("");  // Select된 것이 없으면 _T("") 리턴
	long *aNum = new long[nSize];
	for (int i = 0; i < nSize; i++)
		aNum[i] = aKey[i];
	qsort((void*)aNum, nSize, sizeof(long), CCompFunc::UINTAsc);

	// Optimize한다.
	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.Optimize(aNum, nSize);

	delete[] aNum;
	return sOptimizedString;
}

void CSelectComboNode::UpdateSelectionText()
{
	if (!GetSafeHwnd()) return;
	if (!IsWindow(GetSafeHwnd())) return;
	SetWindowText(m_sText);
}

BEGIN_MESSAGE_MAP(CSelectComboNode, CComboBox)
	//{{AFX_MSG_MAP(CSelectComboNode)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectComboNode message handlers

BOOL CSelectComboNode::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes
					
	  if(pMsg->wParam == VK_RETURN)
		{
			OnEnterKeyPressed();
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);	
}

void CSelectComboNode::OnEnterKeyPressed()
{
	CString sSelectionText;
	GetWindowText(sSelectionText);
	CNumericOptimizer optimizer;

	long *src, *tgt;
	long srcSize, tgtSize;

	CDBDoc* pDoc = GetDocument();
	if (!pDoc) return;

	srcSize = pDoc->m_pAttrCtrl->GetCountNode();
	if (srcSize == 0) // data가 없는 경우
		return;

	// data 배열을 만든다.
	src = new long[srcSize];
	tgt = new long[srcSize];

	long nCount = 0;
	T_NODE_K key;
	T_NODE_D data;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartNode();
	while (pos != NULL)
	{
		pDoc->m_pAttrCtrl->GetNextNode(pos, key, data);
		src[nCount++] = key;
	}
	qsort((void*)src, srcSize, sizeof(long), CCompFunc::UINTAsc);

	optimizer.SelectNumByStr(src, srcSize, sSelectionText.GetBuffer(0), tgt, tgtSize);
	sSelectionText.ReleaseBuffer();

	CArray<T_NODE_K, T_NODE_K>aNode;
	for(int i = 0; i < tgtSize; i++)
		aNode.Add(tgt[i]);

	pDoc->m_pViewCtrl->SelectNode(NULL, aNode, TRUE);
	//pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	//pDoc->m_pViewCtrl->SelectNode(NULL, aNode);

	// 메모리 해제
	delete[] tgt; tgt=NULL;
	delete[] src; src=NULL;

	// for test
	UpdateSelection();
}
