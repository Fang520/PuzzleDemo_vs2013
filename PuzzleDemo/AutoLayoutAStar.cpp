#include "stdafx.h"
#include "AutoLayoutAStar.h"
#include <unordered_map>
#include <queue>

using namespace std;

class OpenNode
{
public:
	static int level;
	OpenNode(char* layout, OpenNode* parent);
	~OpenNode();
	void CalcWeight();
	char* data;
	list<vector<char>> GetPath();
	int weight;
	OpenNode* parent;
};

int OpenNode::level = 0;

OpenNode::OpenNode(char* layout, OpenNode* parent)
{
	int len = level * level;
	data = new char[len];
	memcpy(data, layout, len);
	this->parent = parent;
	CalcWeight();
}

OpenNode::~OpenNode()
{
	delete[] data;
}

void OpenNode::CalcWeight()
{
	OpenNode* p = this;
	int n = 0;
	while (p)
	{
		p = p->parent;
		n++;
	}
	weight = n;
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
		//printf("%d\n", a->weight < b->weight);
		return a->weight < b->weight;
	}
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

list<vector<char>> CAutoLayoutAStar::LayoutBFS()
{
	int new_count = 0;
	int delete_count = 0;
	list<vector<char>> path_list;
	//priority_queue<OpenNode*, vector<OpenNode*>, PriorityFun> open_list;
	list<OpenNode*> open_list;
	unordered_map<CloseNode, OpenNode*, HashFun> close_list;

	OpenNode::level = m_Level;
	CloseNode::data_len = m_LayoutLen;
	close_list.rehash(218357);

	OpenNode* first = new OpenNode(m_OriginalLayout, NULL);
	new_count++;
	//open_list.push(first);
	open_list.push_back(first);

	unsigned int begin = GetTickCount();

	while (!open_list.empty())
	{
		//OpenNode* open_node = open_list.top();
		OpenNode* open_node = open_list.front();
		//open_list.pop();
		open_list.pop_front();
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
				OpenNode* new_open_node = new OpenNode(new_data, open_node);
				new_count++;
				//open_list.push(new_open_node);
				open_list.push_back(new_open_node);
			}
		}
	}

	unsigned int end = GetTickCount();

	printf("visited: %d\tall: %d\ttime: %d\n", close_list.size(), open_list.size() + close_list.size(), end - begin);

	while (!open_list.empty())
	{
		//OpenNode* node = open_list.top();
		OpenNode* node = open_list.front();
		//open_list.pop();
		open_list.pop_front();
		delete node;
		delete_count++;
	}

	for (unordered_map<CloseNode, OpenNode*, HashFun>::iterator it = close_list.begin(); it != close_list.end(); it++)
	{
		OpenNode* node = it->second;
		delete node;
		delete_count++;
	}

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


void CAutoLayoutAStar::PrintLayout(const char* data)
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

