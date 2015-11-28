//-------------------------------------------------------------------------
#include "../Include/FKCW_UI_Table.h"
#include "../../FKCW_Base/Include/FKCW_Base_Macro.h"
#include "../../FKCW_Base/Include/FKCW_Base_Geometry.h"
//-------------------------------------------------------------------------
FKCW_UI_Table* FKCW_UI_Table::create(const CCRect& tWindowRect, int nWindowColumnCount, int nWindowRowCount, bool bFillDirectionHorizontal, bool bLayerDirectionHorizontal)
{
	FKCW_UI_Table* pRet = new FKCW_UI_Table();
	return pRet->init(tWindowRect, nWindowColumnCount, nWindowRowCount, bFillDirectionHorizontal, bLayerDirectionHorizontal) ? (FKCW_UI_Table*)pRet->autorelease() : (delete pRet, pRet = NULL);
}
//-------------------------------------------------------------------------
FKCW_UI_Table::FKCW_UI_Table() :
	m_nWindowColumnCount(1),
	m_nWindowRowCount(1),
	m_bFillDirectionHorizontal(true),
	m_bLayerDirectionHorizontal(false)
{
}
//-------------------------------------------------------------------------
bool FKCW_UI_Table::init(const CCRect& tWindowRect, int nWindowColumnCount, int nWindowRowCount, bool bFillDirectionHorizontal, bool bLayerDirectionHorizontal)
{
	m_tWindowRect = tWindowRect;
	m_nWindowColumnCount = nWindowColumnCount;
	m_nWindowRowCount = nWindowRowCount;
	m_bFillDirectionHorizontal = bFillDirectionHorizontal;
	m_bLayerDirectionHorizontal = bLayerDirectionHorizontal;
	m_tCellSize = cczNormalizeTo(m_tWindowRect.size, ccz(m_nWindowColumnCount, m_nWindowRowCount));
	 
	return true;
}
//-------------------------------------------------------------------------
void FKCW_UI_Table::registerNode(CCNode* pNode)
{
	m_tNodes.addObject(pNode);
}
//-------------------------------------------------------------------------
void FKCW_UI_Table::clearNodes()
{
	m_tNodes.removeAllObjects();
}
//-------------------------------------------------------------------------
CCNode* FKCW_UI_Table::createCellNode()
{
	CCNode* pRet = CCNode::create();

	pRet->setContentSize(m_tCellSize);
	registerNode(pRet);

	return pRet;
}
//-------------------------------------------------------------------------
void FKCW_UI_Table::arrangeItems()
{
	unsigned int nItemCount = m_tNodes.count();
	int nColumnCount = (m_bLayerDirectionHorizontal ? nItemCount / m_nWindowRowCount + (int)(nItemCount % m_nWindowRowCount > 0) : m_nWindowColumnCount);
	int nRowCount = (m_bLayerDirectionHorizontal ? m_nWindowRowCount : nItemCount / m_nWindowColumnCount + (int)(nItemCount % m_nWindowColumnCount > 0));
	m_tLayerSize = cczDenormalizeTo(ccz((float)nColumnCount, (float)nRowCount), m_tCellSize);

	int nItemNum = 0;
	if (CCArray* __pArray = (&m_tNodes))
	{
		for (unsigned int __idx = 0; __idx < __pArray->data->num; ++__idx) 
		{
			if ( CCNode* pNode = ( CCNode* )*(__pArray->data->arr + __idx))
			{
				pNode->setPosition(ccpShift(m_tWindowRect.origin,
					cczDenormalizeTo(
					ccz(
					m_bFillDirectionHorizontal ? nItemNum % nColumnCount : nItemNum / nRowCount,
					nRowCount - 1 - (m_bFillDirectionHorizontal ? nItemNum / nColumnCount : nItemNum % nRowCount)),
					m_tCellSize)));
				++nItemNum;
			}
		}
	}
}
//-------------------------------------------------------------------------