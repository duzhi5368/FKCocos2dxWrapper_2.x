//-------------------------------------------------------------------------
#include "../Include/FKCW_RichHtml_RichHtmlCache.h"
#include "../Include/FKCW_RichHtml_RichHtmlElement.h"
//-------------------------------------------------------------------------
FKCW_RichHtml_CacheBase::FKCW_RichHtml_CacheBase()
	: m_rHAlign(e_align_left)
	, m_rVAlign(e_align_bottom)
	, m_rLineHeight(0)
	, m_rSpacing(0)
	, m_rPadding(0)
	, m_rWrapLine(true)
{

}
//-------------------------------------------------------------------------
// LineCache
//-------------------------------------------------------------------------
FKCW_RichHtml_Rect FKCW_RichHtml_LineCache::flush(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_Rect line_rect;

	// 无元素，无需Flush
	element_list_t* line = getCachedElements();
	if ( line->size() == 0 )
		return line_rect;

	std::vector<element_list_t::iterator> line_marks;
	std::vector<short> line_widths;

	FKCW_RichHtml_Rect zone = compositor->getMetricsState()->zone;
	bool wrapline = m_rWrapLine;

	if ( zone.size.w == 0 )
		wrapline = false;

	FKCW_RichHtml_MetricsState* mstate = compositor->getMetricsState();

	FKCW_RichHtml_Pos pen;
	FKCW_RichHtml_Rect temp_linerect;
	short base_line_pos_y = 0;
	element_list_t::iterator inner_start_it = line->begin();
	line_marks.push_back(line->begin());
	for ( element_list_t::iterator it = line->begin(); it != line->end(); it++ )
	{
		FKCW_RichHtml_Metrics* metrics = (*it)->getMetrics();

		// 通知组合器预渲染事件
		(*it)->onCachedCompositBegin(this, pen);

		// 计算基准线偏移量
		short baseline_correct = 0;
		if ( (*it)->needBaselineCorrect() )
		{
			baseline_correct = m_rBaselinePos;
		}

		// 第一个元素
		if ( pen.x == 0 )
		{
			pen.x -= metrics->rect.min_x();
		}

		// 设置位置
		(*it)->setLocalPositionX(pen.x);
		(*it)->setLocalPositionY(pen.y + baseline_correct);

		FKCW_RichHtml_Rect rect = metrics->rect;
		rect.pos.x += pen.x;
		rect.pos.y += baseline_correct;
		temp_linerect.extend(rect);

		// 处理包围线
		element_list_t::iterator next_it = it + 1;
		if ( next_it == line->end() ||			// 最后一个元素
			(*next_it)->isNewlineBefore() ||	// 下一个元素将换行
			(*it)->isNewlineFollow() ||			// 上一个元素是换行元素
			( wrapline && pen.x != 0			// 包围线
			&& pen.x + metrics->advance.x + (*next_it)->getMetrics()->rect.pos.x + (*next_it)->getMetrics()->rect.size.w + getPadding()*2 > zone.size.w 
			&& (*next_it)->canLinewrap() ) )
		{
			short y2correct = -temp_linerect.max_y();

			for ( element_list_t::iterator inner_it = inner_start_it; inner_it != next_it; inner_it++ )
			{
				FKCW_RichHtml_Pos pos = (*inner_it)->getLocalPosition();
				(*inner_it)->setLocalPositionY(pos.y + y2correct);
				(*inner_it)->setLocalPositionX(pos.x /*+ x2correct*/);
			}

			temp_linerect.pos.y = pen.y;
			line_rect.extend(temp_linerect);

			pen.y -= (temp_linerect.size.h + getSpacing());
			pen.x = 0;

			// 填充下一行
			line_marks.push_back(next_it);
			line_widths.push_back(temp_linerect.size.w);

			inner_start_it = next_it;
			temp_linerect = FKCW_RichHtml_Rect();
		}
		else
		{
			pen.x += metrics->advance.x;
		}

		// 通知组合器事件
		(*it)->onCachedCompositEnd(this, pen);
	}

	short align_correct_x = 0;
	size_t line_mark_idx = 0;
	if ( getHAlign() == e_align_left )
		line_rect.size.w += getPadding() * 2;
	else
		line_rect.size.w = MAX(zone.size.w, line_rect.size.w + getPadding() * 2);
	for ( element_list_t::iterator it = line->begin(); it != line->end(); it++ )
	{
		// 通知组合器预渲染事件
		(*it)->onCachedCompositBegin(this, pen);

		if ( it == line_marks[line_mark_idx] )
		{
			short lwidth = line_widths[line_mark_idx];

			// 纠正X坐标
			switch ( getHAlign() )
			{
			case e_align_left:
				align_correct_x = getPadding();
				break;
			case e_align_center:
				align_correct_x = ( line_rect.size.w - lwidth ) / 2;
				break;
			case e_align_right:
				align_correct_x = line_rect.size.w - lwidth - getPadding();
				break;
			}

			line_mark_idx++; // 直到最后一行
		}

		FKCW_RichHtml_Pos pos = (*it)->getLocalPosition();
		(*it)->setLocalPositionX(mstate->pen_x + pos.x + align_correct_x);
		(*it)->setLocalPositionY(mstate->pen_y + pos.y);

		// 通知组合器事件
		(*it)->onCachedCompositEnd(this, pen);
	}

	line_rect.pos.y = mstate->pen_y;

	mstate->pen_y -= (line_rect.size.h + getSpacing());
	mstate->pen_x = 0;

	clear();

	return line_rect;
}
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_LineCache::getCachedElements()
{
	return &m_rCachedLine;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_LineCache::appendElement(FKCW_RichHtml_IRichElement* ele)
{
	m_rCachedLine.push_back(ele);

	m_rBaselinePos = MIN(ele->getBaseline(), m_rBaselinePos);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_LineCache::clear()
{
	m_rCachedLine.clear();
	m_rBaselinePos = 0;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_LineCache::FKCW_RichHtml_LineCache()
	: m_rBaselinePos(0)
{

}
//-------------------------------------------------------------------------
// TableCache
//-------------------------------------------------------------------------
element_list_t* FKCW_RichHtml_TableCache::getCachedElements()
{
	return &m_rCached;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_TableCache::clear()
{
	m_rCached.clear();
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_TableCache::appendElement(FKCW_RichHtml_IRichElement* ele)
{
	m_rCached.push_back(ele);
}
//-------------------------------------------------------------------------
FKCW_RichHtml_Rect FKCW_RichHtml_TableCache::flush(class FKCW_RichHtml_IRichCompositor* compositor)
{
	FKCW_RichHtml_Rect table_rect;

	if ( m_rCached.empty())
	{
		return table_rect;
	}

	// 表单上下文大小
	std::vector<short> row_heights;
	std::vector<short> col_widths;
	std::vector<bool> width_set;
	short max_row_width = 0;
	short max_row_height = 0;
	for ( element_list_t::iterator it = m_rCached.begin(); it != m_rCached.end(); it++ )
	{
		FKCW_RichHtml_EleHTMLRow* row = dynamic_cast<FKCW_RichHtml_EleHTMLRow*>(*it);
		if ( !row )
		{
			CCLog("[CCRich] Table cache can only accept 'FKCW_RichHtml_EleHTMLRow' element!");
			continue;
		}

		short current_row_height = 0;
		std::vector<class FKCW_RichHtml_EleHTMLCell*>& cells = row->getCells();
		for ( size_t i = 0; i < cells.size(); i++ )
		{
			CCAssert(i <= col_widths.size(), "");
			if ( i == col_widths.size() )
			{
				col_widths.push_back(cells[i]->getMetrics()->rect.size.w + getPadding() * 2);
				width_set.push_back(cells[i]->isWidthSet());
			}
			else
			{
				if (width_set[i])
				{
					if (cells[i]->isWidthSet())
					{
						col_widths[i] = MAX(col_widths[i], cells[i]->getMetrics()->rect.size.w + getPadding() * 2);
					}
					else
					{
					}
				}
				else
				{
					if (cells[i]->isWidthSet())
					{
						col_widths[i] = cells[i]->getMetrics()->rect.size.w + getPadding() * 2;
						width_set[i] = true;
					}
					else
					{
					}
				}
			}

			current_row_height = MAX(current_row_height, cells[i]->getMetrics()->rect.size.h);
		}

		current_row_height += getPadding() * 2;
		row_heights.push_back(current_row_height);

		table_rect.size.h += current_row_height;
	}

	// 最大宽度
	for ( size_t i = 0; i < col_widths.size(); i++ )
	{
		table_rect.size.w += col_widths[i];
	}

	// 设置上下文矩阵
	short spacing = getSpacing();
	short pen_x = 0;
	short pen_y = -m_rTable->m_rBorder;
	size_t row_idx = 0;
	for ( element_list_t::iterator it = m_rCached.begin(); it != m_rCached.end(); it++ )
	{
		FKCW_RichHtml_EleHTMLRow* row = dynamic_cast<FKCW_RichHtml_EleHTMLRow*>(*it);
		if ( !row )
		{
			CCLog("[CCRich] Table cache can only accept 'FKCW_RichHtml_EleHTMLRow' element!");
			continue;
		}

		pen_x = m_rTable->m_rBorder;

		// 设置行矩阵
		row->setLocalPositionX(pen_x);
		row->setLocalPositionY(pen_y);
		FKCW_RichHtml_Metrics* row_metrics = row->getMetrics();
		row_metrics->rect.size.h = row_heights[row_idx];
		row_metrics->rect.size.w = table_rect.size.w + spacing * (col_widths.size() - 1);

		// 处理每一行内的格子单元
		short cell_pen_x = 0;
		std::vector<class FKCW_RichHtml_EleHTMLCell*>& cells = row->getCells();
		for ( size_t i = 0; i < cells.size(); i++ )
		{
			cells[i]->setLocalPositionX(cell_pen_x);
			cells[i]->setLocalPositionY(0);
			FKCW_RichHtml_Metrics* cell_metrics = cells[i]->getMetrics();
			cell_metrics->rect.size.w = col_widths[i];
			cell_metrics->rect.size.h = row_heights[row_idx];

			recompositCell(cells[i]);

			cell_pen_x += col_widths[i];
			cell_pen_x += spacing;
		}

		pen_y -= row_heights[row_idx];
		pen_y -= spacing;
		row_idx++;
	}

	table_rect.size.h += m_rTable->m_rBorder * 2 + spacing * (row_heights.size() - 1);
	table_rect.size.w += m_rTable->m_rBorder * 2 + spacing * (col_widths.size() - 1);

	m_rCached.clear();

	return table_rect;
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_TableCache::travesalRecompositChildren(element_list_t* eles, short x_fixed, short y_fixed)
{
	for( element_list_t::iterator it = eles->begin(); it != eles->end(); it++ )
	{
		if ( !(*it)->pushMetricsState() )
		{
			element_list_t* eles = (*it)->getChildren();
			if ( eles && !eles->empty() )
				travesalRecompositChildren(eles, x_fixed, y_fixed);
		}

		FKCW_RichHtml_Pos pos = (*it)->getLocalPosition();
		(*it)->setLocalPositionX(pos.x + x_fixed);
		(*it)->setLocalPositionY(pos.y + y_fixed);
	}
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_TableCache::recompositCell(class FKCW_RichHtml_EleHTMLCell* cell)
{
	FKCW_RichHtml_Size content_size = cell->m_rContentSize.size;
	FKCW_RichHtml_Size zone_size = cell->getMetrics()->rect.size;

	short padding = getPadding();
	short x_fixed = 0;
	short y_fixed = 0;
	ENUM_Alignment halign = cell->m_rHAlignSpecified ? 
		cell->m_rHAlignment : 
	( cell->m_rRow->m_rHAlignSpecified ? cell->m_rRow->m_rHAlignment : m_rHAlign);
	ENUM_Alignment valign = cell->m_rVAlignSpecified ? 
		cell->m_rVAlignment :
	( cell->m_rRow->m_rVAlignSpecified ? cell->m_rRow->m_rVAlignment : m_rVAlign);

	switch ( halign )
	{
	case e_align_left:
		x_fixed = 0 + padding;
		break;
	case e_align_center:
		x_fixed = ( zone_size.w - content_size.w ) / 2;
		break;
	case e_align_right:
		x_fixed = zone_size.w - content_size.w - padding;
		break;
	}

	switch ( valign )
	{
	case e_align_top:
		y_fixed = 0 - padding;
		break;
	case e_align_middle:
		y_fixed = -( zone_size.h - content_size.h ) / 2;
		break;
	case e_align_bottom:
		y_fixed = -(zone_size.h - content_size.h) + padding;
		break;
	}

	//x_fixed = MIN( MAX(x_fixed, 0), (zone_size.w - content_size.w) );
	//y_fixed = MAX( MIN(y_fixed, 0), -(zone_size.h - content_size.h) );

	element_list_t* eles = cell->getChildren();
	if ( eles && !eles->empty() )
		travesalRecompositChildren(eles, x_fixed, y_fixed);
}
//-------------------------------------------------------------------------
void FKCW_RichHtml_TableCache::setTable(class FKCW_RichHtml_EleHTMLTable* table)
{
	m_rTable = table;
}
//-------------------------------------------------------------------------
FKCW_RichHtml_TableCache::FKCW_RichHtml_TableCache()
	: m_rTable(NULL)
{

}
//-------------------------------------------------------------------------