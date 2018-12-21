#include "stdafx.h"
#include <unordered_map>
#include <queue>
#include "AutoLayoutAStar.h"

int OpenNode::level = 0;

OpenNode::OpenNode(char* layout, OpenNode* parent, int len)
{
	data = new char[level * level];
	memcpy(data, layout, level * level);
	this->len = len;
	this->parent = parent;
	CalcWeight();
}

OpenNode::~OpenNode()
{
	delete[] data;
}

void OpenNode::CalcWeight()
{
	int i, j, d;
	d = 0;
	for (i = 0; i < level * level; i++)
	{
		int x, y, x1, y1;
		x = i % level;
		y = i / level;
		for (j = 0; j < level *level; j++)
		{
			if (data[j] == i + 1)
			{
				break;
			}
		}
		x1 = j % level;
		y1 = j / level;
		d += (abs(x1 - x) + abs(y1 - y));
	}
	weight = d * 5 + len;
}

list<vector<char>> OpenNode::GetPath()
{
	list<vector<char>> result;
	OpenNode* p = this;
	int len = level * level;
	while (p)
	{
		char* s = p->data;
		vector<char> item(s, s + len);
		result.push_front(item);
		p = p->parent;
	}
	return result;
}

struct PriorityFun
{
	bool operator()(OpenNode *&a, OpenNode *&b) const
	{
		return a->weight > b->weight;
	}
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

CAutoLayoutAStar::CAutoLayoutAStar(const char *layout, int len)
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

CAutoLayoutAStar::~CAutoLayoutAStar()
{
	free(m_OriginalLayout);
	free(m_TargetLayout);
}

bool CAutoLayoutAStar::CanSolved()
{
	int sum = 0;
	int n = m_LayoutLen;
	char* s = m_OriginalLayout;
	for (int i = 0; i < n; i++)
	{
		if (s[i] == n)
		{
			continue;
		}
		for (int j = i + 1; j < n; j++)
		{
			if (s[j] == n)
			{
				continue;
			}
			if (s[i] > s[j])
			{
				sum++;
			}
		}
	}

	if (m_Level % 2 == 0)
	{
		int i;
		for (i = 0; i < n; i++)
		{
			if (s[i] == n)
			{
				break;
			}
		}
		int line = i / m_Level + 1;
		return ((m_Level - line) % 2) == (sum % 2);
	}
	else
	{
		return (sum % 2 == 0);
	}
}

list<vector<char>> CAutoLayoutAStar::LayoutBFS()
{
	int new_count = 0;
	int delete_count = 0;
	list<vector<char>> path_list;
	
	if (!CanSolved())
	{
		printf("no solution\n");
		return	path_list;
	}

	priority_queue<OpenNode*, vector<OpenNode*>, PriorityFun> open_list;
	unordered_map<CloseNode, OpenNode*, HashFun> close_list;

	OpenNode::level = m_Level;
	CloseNode::data_len = m_LayoutLen;
	close_list.rehash(218357);

	OpenNode* first = new OpenNode(m_OriginalLayout, NULL, 0);
	new_count++;
	open_list.push(first);

	unsigned int begin = GetTickCount();

	while (!open_list.empty())
	{
		OpenNode* open_node = open_list.top();
		open_list.pop();
		if (close_list.count(CloseNode(open_node->data)) != 0)
		{
			delete open_node;
			delete_count++;
			continue;
		}
		close_list[CloseNode(open_node->data)] = open_node;

		if (memcmp(open_node->data, m_TargetLayout, m_LayoutLen) == 0)
		{
			printf("Got it\n");
			path_list = open_node->GetPath();
			break;
		}

		int next_count = GetNextLayouts(open_node->data);
		for (int i = 0; i < next_count; i++)
		{
			char* new_data = m_Probe[i];
			if (close_list.count(CloseNode(new_data)) == 0)
			{
				OpenNode* new_open_node = new OpenNode(new_data, open_node, open_node->len + 1);
				new_count++;
				open_list.push(new_open_node);
			}
		}
	}

	unsigned int end = GetTickCount();

	printf("visited: %d\tall: %d\ttime: %d\n", close_list.size(), open_list.size() + close_list.size(), end - begin);

	while (!open_list.empty())
	{
		OpenNode* node = open_list.top();
		open_list.pop();
		delete node;
		delete_count++;
	}

	for (unordered_map<CloseNode, OpenNode*, HashFun>::iterator it = close_list.begin(); it != close_list.end(); it++)
	{
		OpenNode* node = it->second;
		delete node;
		delete_count++;
	}
	close_list.clear();

	printf("new=%d delete=%d\n", new_count, delete_count);

	return path_list;
}

int CAutoLayoutAStar::GetNextLayouts(const char* data)
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
			char* new_data = m_Probe[new_data_index];
			for (j = 0; j<m_LayoutLen; j++)
			{
				new_data[j] = data[j];
			}
			new_data[probe_index] = data[index];
			new_data[index] = data[probe_index];
			new_data_index++;
		}
	}
	return new_data_index;
}
