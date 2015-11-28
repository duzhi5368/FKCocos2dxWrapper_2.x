//--------------------------------------------------------------------
#include "../Include/FKCW_Base_Memory.h"
#include <memory.h>
//--------------------------------------------------------------------
bool g_bIsFlagAllocationLog = false;
//--------------------------------------------------------------------
#ifdef FKCW_MEMORY_TRACKING
//--------------------------------------------------------------------
// 最大内存块数
#define MEMORY_RECORD_INDEX_SIZE 0x10000
#define MEMORY_RECORD_INDEX_MASK 0xFFFF
//--------------------------------------------------------------------
// 内存记录结构
struct SMemoryRecord
{
	void*			m_pStart;			// 分配内存的起始位
	int				m_nSize;			// 内存大小
	const char*		m_szFile;			// 分配该内存的文件名
	int				m_nLine;			// 分配该内存的行数
	SMemoryRecord*	m_pNext;			// 下一个内存信息块
};
//--------------------------------------------------------------------
static SMemoryRecord* s_MemoryRecord[MEMORY_RECORD_INDEX_SIZE] = { 0 };
static int s_TotalUsedMemory = 0;		// 总使用内存数		
static int s_MaxUsedMemory = 0;			// 最大使用的内存数
static int s_TotalAlloc = 0;			// 总分配的块数
static int s_TotalFree = 0;				// 总释放的块数
//--------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

void AddRecord( SMemoryRecord* r )
{
	// 获取hash位置
	int hash = (uintptr_t)r->m_pStart & MEMORY_RECORD_INDEX_MASK;
	SMemoryRecord* pTemp = s_MemoryRecord[hash];
	SMemoryRecord* pPrev = NULL;

	// 找的块起始大小是否比传入的块起始大小大
	while (pTemp) 
	{
		if (pTemp->m_pStart > r->m_pStart) 
		{
			break;
		}
		pPrev = pTemp;
		pTemp = pTemp->m_pNext;
	}

	// 插入新数据到记录链表
	if (pPrev == NULL) 
	{
		s_MemoryRecord[hash] = r;
		if (pTemp) 
		{
			r->m_pNext = pTemp;
		}
	} 
	else 
	{
		r->m_pNext = pPrev->m_pNext;
		pPrev->m_pNext = r;
	}

	// 更新统计数据
	s_TotalUsedMemory += r->m_nSize;
	s_MaxUsedMemory = (s_MaxUsedMemory > s_TotalUsedMemory ? s_MaxUsedMemory : s_TotalUsedMemory);
	s_TotalAlloc++;
}

SMemoryRecord* FindRecord( void* p ) 
{
	int hash = (uintptr_t)p & MEMORY_RECORD_INDEX_MASK;
	SMemoryRecord* pTemp = s_MemoryRecord[hash];
	SMemoryRecord* r = NULL;
	while (pTemp) 
	{
		if (pTemp->m_pStart == p) 
		{
			r = pTemp;
			break;
		}
		pTemp = pTemp->m_pNext;
	}
	return r;
}

SMemoryRecord* RemoveRecord(SMemoryRecord* r) 
{
	int hash = (uintptr_t)r->m_pStart & MEMORY_RECORD_INDEX_MASK;
	SMemoryRecord* pTemp = s_MemoryRecord[hash];
	SMemoryRecord* pPrev = NULL;
	while(pTemp) {
		if(pTemp == r) {
			break;
		}
		pPrev = pTemp;
		pTemp = pTemp->m_pNext;
	}

	// 没找到
	if(pTemp != r) {
		CCLOG("[MEMRECORD] Unmatched record (%p)(record=%p):Size=%d [%s:Line=%d]", 
			r->m_pStart, r, r->m_nSize, r->m_szFile, r->m_nLine);
		return NULL;
	}

	// 删除记录
	if(pTemp == s_MemoryRecord[hash]) 
	{
		s_MemoryRecord[hash] = pTemp->m_pNext;
	}
	else 
	{
		pPrev->m_pNext = pTemp->m_pNext;
	}

	// 同步信息
	s_TotalUsedMemory -= r->m_nSize;
	s_TotalFree++;

	r->m_pNext = NULL;
	return r;
}

void* _ccMalloc(size_t size, const char* file, int line, const char* logTag) 
{
	if (size == 0)
		return NULL;

	void* p = malloc(size);
	if (p) 
	{
		SMemoryRecord* r = (SMemoryRecord*)malloc(sizeof(SMemoryRecord));
		if(!r) 
		{
			free(p);
			p = NULL;
		} 
		else 
		{
			r->m_pStart = p;
			r->m_nSize = size;
			r->m_szFile = file;
			r->m_nLine = line;
			r->m_pNext = NULL;
			AddRecord(r);
		}

		if( g_bIsFlagAllocationLog )
		{
			CCLOG("[%s](%p)(record=%p):%d [%s:%d]", 
				logTag, r->m_pStart, r, r->m_nSize, r->m_szFile, r->m_nLine );
		}
	}

	return p;
}


void* _ccCalloc(size_t nitems, size_t size, const char* file, int line) 
{
	void* ptr = _ccMalloc(nitems * size, file, line, "CALLOC");
	memset(ptr, 0, nitems * size);
	return ptr;
}

void* _ccRealloc(void* ptr, size_t size, const char* file, int line) 
{
	SMemoryRecord* r = FindRecord(ptr);
	if(r) 
	{
		// 实际分配
		void* newPtr = _ccMalloc(size, file, line, "REALLOC");
		memcpy(newPtr, ptr, r->m_nSize);
		free(ptr);

		// 移除记录
		RemoveRecord(r);
		free(r);

		return newPtr;
	} 
	else 
	{
		return realloc(ptr, size);
	}
}

void _ccFree(void* ptr, const char* file, int line) 
{
	if(!ptr)
		return;

	// 查找记录并删除
	SMemoryRecord* r = FindRecord(ptr);
	if(r) 
	{
		if( g_bIsFlagAllocationLog)
			CCLOG("[FREE](%p)(record=%p):%d [%s:%d]", r->m_pStart, r, r->m_nSize, r->m_szFile, r->m_nLine);
		RemoveRecord(r);
		free(r);
	}

	// 即使找不到记录，我们必须也要释放内存
	free(ptr);
}

#ifdef __cplusplus
}
#endif
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
// FKCW_Base_Memory
//--------------------------------------------------------------------
// 输出内存使用列表
void FKCW_Base_Memory::PrintUasge()
{
#ifdef FKCW_MEMORY_TRACKING
	CCLOG("-----------------------------FKCW_Memory Uasge--------------------------------");
	CCLOG("[MEMREPORT] peak %d bytes, now %d bytes, alloc %d times, free %d times",
		s_MaxUsedMemory, s_TotalUsedMemory, s_TotalAlloc, s_TotalFree);
	CCLOG("------------------------------------------------------------------------------");
#endif
}
//--------------------------------------------------------------------
// 输出内存泄露记录
void FKCW_Base_Memory::PrintDump()
{
#ifdef FKCW_MEMORY_TRACKING
	CCLOG("-----------------------------FKCW_Memory Dump--------------------------------");
	int leak = 0;
	int leakNum = 0;
	for (int i = 0; i < MEMORY_RECORD_INDEX_SIZE; i++) 
	{
		SMemoryRecord* r = s_MemoryRecord[i];
		while(r) 
		{
			CCLOG("%d.[MEMRECORD](%p)(record=%p):Size=%d [%s:Line=%d]", ++leakNum, 
				r->m_pStart, r, r->m_nSize, r->m_szFile, r->m_nLine );
			leak += r->m_nSize;
			r = r->m_pNext;
		}
	}

	if(leakNum > 0) 
	{
		CCLOG("[MEMRECORD] total leak = %d", leak);
	}
	else 
	{
		CCLOG("[MEMRECORD] no memory leak, congratulations!");
	}
	CCLOG("------------------------------------------------------------------------------");
#endif
}
//--------------------------------------------------------------------