#include <iostream>
#include <fstream>

//Containers
#include <vector>
#include <list>
#include <queue>

#include <algorithm>
#include <iomanip>

class Graph {
private:
	struct link {
		Graph* parentGraph;
		const int from, to;

		int cost, capacity;
		int traffic;

		link(Graph* parent, int f, int t, int c, int cap, int tr) 
			: parentGraph(parent), from(f), to(t), cost(c), capacity(cap), traffic(tr) {}

		void printNode() {
			std::cout << std::setw(10) << "link (" << from << ", " << to << "):";
			std::cout << std::setw(10) << "cost: " << cost;
			std::cout << std::setw(15) << "traffic: " << traffic << "/" << capacity * parentGraph->tau / 100.0;

			double congestion_limit = capacity * parentGraph->tau / 100.0;
			if (traffic > congestion_limit)
				std::cout << "        (CONGESTION!)";
			std::cout << '\n';
		}
	};

	//I think I to complex construction did we need just to vectors and stash in all what we need
	void Dijkstra(int ind1, int nums_of_path)
	{
		std::vector<std::pair<int, std::list<link>>> paths(100, {0, {}});//<cost, path>

		std::queue<std::list<link>> q;
		q.push(G[ind1]);

		while (!q.empty())
		{
			std::list<link> tmp = q.front();
			q.pop();
			int i = 0;
			for (const link& t : tmp)
			{
				q.push(G[t.to]);
				paths[i].first += t.cost;
				paths[i].second.push_back(t);
				++i;
			}
		}
		//Sorting paths by it's cost
		std::sort(paths.begin(), paths.end(), [] (const auto& f, const auto& s) 
			{
				return f.first > s.first;
			});

		for (int i = 0; i < paths.size(); ++i)
		{
			if (paths[i].second.size() > 10)
			{
				paths.erase(paths.begin() + i);//No one can escape from the fate prepared for him, after that, all that remains is the result - your death.
				//KIIING CRIMSON! - ORA-ORA-ORA-ORA-ORA
			}
		}
		int add = paths.size() <= nums_of_path ? paths.size() : nums_of_path;

		std::copy(paths.begin(), paths.begin() + add, std::back_inserter(OptPaths));
	}
	
public:
	// Graph implementation via adjecency list
	std::vector<std::list<link>> G; 
	std::vector<std::pair<int, std::list<link>>> OptPaths;

	int tau, K;
	int n, m;

	Graph() : tau(0), K(0), n(0), m(0){}

	Graph(Graph&) = delete;
	Graph& operator = (const Graph&) = delete;

	void CreateGraph(std::istream& input) {
		input >> n >> m >> tau >> K;
		std::cout << "n = " << n << " m = " << m << " t = " << tau << " K = " << K << '\n';

		G.resize(n);

		for (int i = 0; i < m; i++) {
			int u, v, cost, capacity, traffic_uv, traffic_vu;

			input >> u >> v >> cost >> capacity >> traffic_uv >> traffic_vu;
			u--; v--; // Switch to 0-indexation (need to switch back when printing actual result)

			G[u].push_back(link(this, u, v, cost, capacity, traffic_uv));
			G[v].push_back(link(this, v, u, cost, capacity, traffic_vu));

			if (traffic_uv >= capacity * tau/100.0) {
				std::cout << "Congestion at link (" << u << ", " << v << "): " 
					<< traffic_uv << "/" << capacity * tau/100.0 << '\n';
			}
			if (traffic_vu >= capacity * tau/100.0) {
				std::cout << "Congestion at link (" << v << ", " << u << "): " 
					<< traffic_vu << "/" << capacity * tau/100.0 << '\n';
			}
		}
	}
	void printGraph() {
		for (int i = 0; i < n; i++) {
			std::cout << std::endl << "Node " << i << ":\n";
			for (Graph::link& edge : G[i]) {
				edge.printNode();
			}
			if (!G[i].size()) std::cout << "NO LINKS\n";
		}
	}

	void retrafficConjestions()
	{
		for (int i = 0; i < m; ++i)
		{
			for (const auto& g : G[i])
			{
				if (g.traffic >= g.capacity * tau / 100)
				{
					Dijkstra(i, 8);//Just take some percent of the conjestion 
				}
			}
		}
	}
};

int main() {
	std::fstream f("file.txt");
	Graph graph;
	graph.CreateGraph(f);
	graph.printGraph();

	return 0;
}

/*
6 7 80 1
1 2 10 100 100 0
1 4 100 100 0 0
2 3 10 100 50 0
2 5 10 100 30 0
3 6 10 100 30 0
4 5 10 100 0 20
5 6 100 100 0 0
*/