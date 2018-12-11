// AutoLayout.cpp: implementation of the CAutoLayout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "AutoLayout.h"
#include <unordered_map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct OpenNode
{
	char* data;
	OpenNode* parent;
};

class CloseNode
{
public:
	static int data_len;
	CloseNode(const char* data);
	bool operator==(const CloseNode& other) const;
	size_t hash() const;
private:
	const char* data;
};

int CloseNode::data_len = 0;

CloseNode::CloseNode(const char* data)
{
	this->data = data;
}

bool CloseNode::operator == (const CloseNode& other) const
{
	if (memcmp(data, other.data, data_len) == 0)
	{
		return true;
	}
	return false;
}

size_t CloseNode::hash() const
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash_code = 0;
	for (int i = 0; i < data_len; i++)
	{
		hash_code = hash_code * seed + data[i];
	}
	return (hash_code & 0x7FFFFFFF);
}

struct HashFun
{
	size_t operator()(const CloseNode& obj) const
	{
		return obj.hash();
	}
};

CAutoLayout::CAutoLayout(const char *layout, int len)
{
	m_LayoutLen = len;
	m_Level = (int)sqrt(len);
	m_OriginalLayout = (char*)malloc(len);
	memcpy(m_OriginalLayout, layout, len);
	m_TargetLayout = (char*)malloc(len);
	for (int i = 0; i < len; i++)
	{
		m_TargetLayout[i] = i + 1;
	}
}

CAutoLayout::~CAutoLayout()
{
	free(m_OriginalLayout);
	free(m_TargetLayout);
}

vector<vector<char>> CAutoLayout::LayoutBFS()
{
	vector<vector<char>> path_list;
	vector<OpenNode*> open_list;
	unordered_map<CloseNode, char, HashFun> close_list;
	int open_list_pos = 0;

	CloseNode::data_len = m_LayoutLen;
	close_list.rehash(218357);

	OpenNode* first = (OpenNode*)malloc(sizeof(OpenNode));
	first->data = (char*)malloc(m_LayoutLen);
	memcpy(first->data, m_OriginalLayout, m_LayoutLen);
	first->parent = NULL;
	open_list.push_back(first);

	while (open_list_pos < open_list.size())
	{
		OpenNode* open_node = open_list[open_list_pos++];
		close_list[CloseNode(open_node->data)] = 1;

		if (memcmp(open_node->data, m_TargetLayout, m_LayoutLen) == 0)
		{
			printf("Got it\n");
			OpenNode* node = open_node;
			while (node)
			{
				char* p = node->data;
				vector<char> item;
				for (int i = 0; i < m_LayoutLen; i++)
				{
					item.push_back(p[i]);
					printf("%d ", p[i]);
				}
				path_list.push_back(item);
				printf("\n");
				node = node->parent;
			}
			reverse(path_list.begin(), path_list.end());
			break;
		}

		char* nexts[4];
		int next_count = GetNextLayouts(open_node->data, nexts);
		for (int i = 0; i < next_count; i++)
		{
			char* new_data = nexts[i];
			if (close_list.count(CloseNode(new_data)) == 0)
			{
				OpenNode* new_open_node = (OpenNode*)malloc(sizeof(OpenNode));
				new_open_node->data = new_data;
				new_open_node->parent = open_node;
				open_list.push_back(new_open_node);
			}
			else
			{
				free(new_data);
			}
		}
	}

	for (int i = 0; i < open_list.size(); i++)
	{
		OpenNode* node = open_list[i];
		free(node->data);
		free(node);
	}

	return path_list;
}

int CAutoLayout::GetNextLayouts(const char* data, char* new_data_list[4])
{
	int i, j, index;
	for (i = 0; i<m_LayoutLen; i++)
	{
		if (m_LayoutLen == data[i])
		{
			index = i;
			break;
		}
	}

	int near_index[4];
	near_index[0] = index - m_Level;
	near_index[1] = index + m_Level;
	near_index[2] = index - 1;
	near_index[3] = index + 1;
	if (near_index[2] / m_Level != index / m_Level)
		near_index[2] = -1;
	if (near_index[3] / m_Level != index / m_Level)
		near_index[3] = -1;

	int new_data_index = 0;
	for (i = 0; i<4; i++)
	{
		int probe_index = near_index[i];
		if (probe_index >= 0 && probe_index < m_LayoutLen)
		{
			char* new_data = (char*)malloc(m_LayoutLen);
			for (j = 0; j<m_LayoutLen; j++)
			{
				new_data[j] = data[j];
			}
			new_data[probe_index] = data[index];
			new_data[index] = data[probe_index];
			new_data_list[new_data_index] = new_data;
			new_data_index++;
		}
	}
	return new_data_index;
}


void CAutoLayout::PrintLayout(const char* data)
{
	for (int i = 0; i < m_Level; i++)
	{
		printf("    ");
		for (int j = 0; j < m_Level; j++)
		{
			printf("%d ", data[i * m_Level + j]);
		}
		printf("\n");
	}
	printf("\n");
}