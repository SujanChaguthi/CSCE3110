#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <set>
#include <queue>


using namespace  std;

const int N = 1e3 + 5, NN = 1e6 + 5;

int n, m, comp;
map<string, int> vis;
set<string> all_nodes;

map<string, vector<pair<string, int>>> adjList;
string startNode;
void dfs_list(const string u) {

	vis[u] = 1;
	for (const auto& v : adjList[u])
	{
		if (vis.find(v.first) == vis.end())
		{
			dfs_list(v.first);
		}
	}
}


bool is_connected()
{
	dfs_list(startNode);
	//check for vis
	for (auto x : all_nodes)
	{
		if (vis.find(x) == vis.end())
		{
			cout << x << "\n";
		}
	}
	return vis.size() == adjList.size();
}

void insert_node(const string& name, string node)
{
	string nodeName, nodeNameL;
	// remove the first and the last element
	node.pop_back();
	if (node[0] == ' ')
	{

		node.erase(0, 2);
	}
	else
	{
		node.erase(0, 1);
	}
	stringstream s(node);
	char c;
	int distance;
	getline(s, nodeName, '-');
	nodeName.pop_back();
	s >> distance;
	adjList[name].push_back({ nodeName ,distance });
	startNode = name;
	all_nodes.insert(name);
	all_nodes.insert(nodeName);
}

void store_adj_list(string& name, string& adj_list_s)
{
	adj_list_s += ';';
	string node;
	stringstream s(adj_list_s);

	while (getline(s, node, ';'))
	{
		insert_node(name, node);
	}

}

void read_csv(fstream& in)
{
	string line;
	string name;
	string adjListS;
	int distance;

	// used for breaking words
	while (getline(in, line))
	{
		stringstream s(line);
		getline(s, name, ',');
		getline(s, adjListS, ',');
		store_adj_list(name, adjListS);
	}
}


map<string, vector<pair<string, int>>> mst_adj_list;

vector< pair<string, pair<string, int>>> all_edges;
bool cmp(pair<string, pair<string, int>> a, pair<string, pair<string, int>> b)
{
	return a.second.second < b.second.second;
}

bool cmpAlpha(pair<string, pair<string, int>> a, pair<string, pair<string, int>> b)
{
	return a.first < b.second.first;
}

map<string, string> Parent;

string Find(string i) {

	// If i is the parent of itself
	if (Parent.find(i) == Parent.end() || Parent[i] == i) {

		Parent[i] = i;
		// Then i is the representative of his set
		return i;
	}
	else { // Else if i is not the parent of itself

		// Then i is not the representative of his set,
		// so we recursively call Find on its parent
		return Find(Parent[i]);
	}
}
void Union(string i, string j) {

	// Find the representatives (or the root nodes) for the set that includes i
	string irep = Find(i),
		// And do the same for the set that includes j
		jrep = Find(j);

	// Make the parent of i's representative be j's representative
	// (effectively moving all of i's set into j's set)
	Parent[irep] = jrep;
}
bool same_set(string a, string b)
{
	return  Find(a) == Find(b);
}

void kruskal_mst()
{
	// load all the edges
	for (auto& x : adjList)
	{
		for (auto& y : x.second)
		{
			all_edges.push_back({ x.first, y });
		}
	}
	// sort by weight
	sort(all_edges.begin(), all_edges.end(), cmp);
	// load the edges one by one
	set<string> edges_in_tree;

	for (auto& edge : all_edges)
	{
		if (same_set(edge.first,edge.second.first))
		{
			continue;
		}
		Union(edge.first, edge.second.first);
		mst_adj_list[edge.first].push_back(edge.second);
		mst_adj_list[edge.second.first].push_back({ edge.first, edge.second.second });
		edges_in_tree.insert(edge.first);
		edges_in_tree.insert(edge.second.first);
	}

}

void prim_mst()
{
	int V = adjList.size();
	priority_queue< pair<int, string>, vector< pair<int, string> >, greater<pair<int, string>>> pq;

	string src = "Anderson County"; // Taking vertex 0 as source
	//string src = "a"; // Taking vertex 0 as source

	map<string, int> key;


	map<string, string> parent;


	map<string, bool> inMST;

	pq.push(make_pair(0, src));
	key[src] = 0;

	while (!pq.empty())
	{

		string u = pq.top().second;
		pq.pop();

		if (inMST[u] == true) {
			continue;
		}

		inMST[u] = true; // Include vertex in MST


		for (auto x : adjList[u])
		{

			string v = x.first;
			int weight = x.second;


			if (inMST[v] == false && key.find(v) == key.end())
			{
				key[v] = weight;
				pq.push(make_pair(key[v], v));
				parent[v] = u;
				mst_adj_list[u].push_back({ v,weight });
				mst_adj_list[u].push_back({ v,weight });
			}
		}
	}
}

void print_mst(fstream& out)
{
	for (auto& node : mst_adj_list)
	{
		out << node.first << ",";
		int indx = 0;
		for (auto& edges : node.second)
		{
			out << "(" << edges.first << " - " << edges.second << ")";
			if (indx != node.second.size() - 1)
			{
				out << ";";
			}
			indx++;
		}
		out << "\n";
	}
}

void mst(int algorithm)
{
	if (algorithm == 0)
	{
		kruskal_mst();
	}
	else
	{
		prim_mst();
	}
}

void save_to_file(string& header, string& algorithm_s)
{
	fstream out(algorithm_s + ".csv", ios::out);
	out << header << endl;
	print_mst(out);
}

int main(int argc, char* argv[])
{
	fstream in, out;
	string filename;
	int algorithm = 0;
	string algorithmS;
	string filename_out[] = { "Prim.csv" , "Kruskal.csv" };
	cout << "Enter  filename: ";
	cin >> filename;
	// ask for required algorithm to use
	cout << "Enter the algorithm u need to use: {Prim, Kruskal}: ";
	cin >> algorithmS;
	if (algorithmS == "Prim")
	{
		algorithm = 1;
	}
	else if (algorithmS == "Kruskal")
	{
		algorithm = 0;
	}
	else
	{
		algorithm = -1;
	}

	if (algorithm == -1)
	{
		cout << "Invalid algorithm.. exiting ";
		return  -1;
	}
	in.open(filename);
	if (!in.is_open())
	{
		cout << "file not found.. exiting ";
		return  -1;
	}
	string header;
	getline(in, header);
	//read the file
	// create the graph
	read_csv(in);
	// check if connected
	if (!is_connected())
	{
		cout << "graph is not connected.. exiting ";
		return  -1;
	}
	cout << "\ngraph is connected\n\nCaltulating MST ...";

	// calculate mst
	mst(algorithm);
	// save to file
	save_to_file(header, algorithmS);
}
