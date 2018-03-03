#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
struct Node{
	struct Node* next=NULL;
	int num_Vertex, leader_num;
	bool explored=false;
};



class linked_list{
public:
	Node *head,*tail; // Tail is required for example in LIFO systems such as Stack, head is required in FIFO systems
	linked_list(){
		head	=	NULL,tail 	=	NULL;
	}

	int leader;
	void add_Node(Node* start);
	void print_list();
	void delete_list();
};


void linked_list::add_Node(Node* start)
{
	if(head==NULL && tail==NULL)
	{
		head = start;
		tail=start;
	}
	else
	{
		tail->next=start;
		tail=tail->next;
	}
}

void linked_list::print_list()
{
	Node* pos;
	cout<<" Link is"<<endl;
	//cout<<head->num_Vertex<<"->";
	if(head!=NULL)
	{
		pos = head;//->next;
		while(pos->next!=NULL)
		{
			cout<<pos->num_Vertex<<"->";
			pos=pos->next;
		}
		cout<<pos->num_Vertex<<endl;
	}
	else 
		cout<<"empty"<<endl;
}

void linked_list::delete_list()
{
	while(head!=NULL)
	{
		Node *n1 = head;
		head=head->next;
		n1=NULL;
	}
}


std::vector<linked_list> reverseGraph(std::vector<linked_list> v)
{
	std::vector<linked_list> rev_graph;
	for(auto i=0;i<v.size();i++)
	{
		linked_list l1;
		Node *start = new Node();
		start->num_Vertex=v[i].head->num_Vertex;
		l1.add_Node(start);
		rev_graph.push_back(l1);
	}
	
	for(auto i=0;i<v.size();i++)
	{
		if(v[i].head!=NULL)
		{
			Node *pos=v[i].head->next;
			while(pos!=NULL)
			{
				int head_index = pos->num_Vertex;
				int tail_index = v[i].head->num_Vertex;
				Node *n1 = new Node();
				n1->num_Vertex=tail_index;
				rev_graph[head_index-1].add_Node(n1);
				pos=pos->next;
			}
		}
	}
	return rev_graph;
}

void DFS_recursive(std::vector<linked_list> &network, int &current_label,const int vertex)
{
	Node *pos = network[vertex].head;
	if(network[vertex].head->explored==false)
	{
		network[vertex].head->explored=true;
		while(pos->next!=NULL)
		{
			pos = pos->next;
			if(network[(pos->num_Vertex)-1].head->explored==false)
				DFS_recursive(network,current_label,(pos->num_Vertex)-1);
		}

		network[vertex].head->leader_num = current_label;
		current_label--;
	}

}

void DFS_main_Loop(std::vector<linked_list> &network)
{
	int current_label = network.size();

	for(auto i=network.size()-1;i>=0;i--)//<network.size();i++)
	{
		if(network[i].head->explored==false)
			DFS_recursive(network,current_label,i);
	}
}

void DFS_second_recursive(std::vector<linked_list> &network, int &current_label, const int vertex)
{
	Node *pos = network[vertex].head;
	if(network[vertex].head->explored==false)
	{
		network[vertex].head->explored=true;
		network[vertex].head->leader_num=current_label;
		while(pos->next!=NULL)
		{
			pos = pos->next;
			if(network[pos->num_Vertex-1].head->explored==false)
				DFS_second_recursive(network,current_label,pos->num_Vertex-1);
		}
	}
		
}
void DFS_second_main_Loop(std::vector<linked_list> &rev_network,std::vector<linked_list> &network)
{
	auto current_label = 1;//network.size();
	for(auto i=0;i<rev_network.size();i++)//<network.size();i++)
	{
		if(network[rev_network[i].head->num_Vertex-1].head->explored==false)
		{
			DFS_second_recursive(network,current_label,rev_network[i].head->num_Vertex-1);
			current_label++;
		}
	}
}

bool sortLeader(const linked_list l1,const linked_list l2)
{
	return l1.head->leader_num < l2.head->leader_num;
}


void SCC_final_loop(std::vector<linked_list> &rev_network, std::vector<linked_list> &network)
{
	std::sort(rev_network.begin(),rev_network.end(),sortLeader);
	DFS_second_main_Loop(rev_network,network);
}


int main ()
{
	vector <linked_list> Graph,Graph1;
	int n;
	string line;
	cout<<"Enter the number of nodes in the network"<<endl;
	cin>>n;

	for (auto i=1;i<=n;i++)
	{
		linked_list l1;
		Node *start = new Node();
		start->num_Vertex =i;
		l1.add_Node(start);
		Graph.push_back(l1);
	}

	ifstream SCC_file;
	string filename;
	cin>>filename;
	string filepath	="./test_cases/";
	string finalPath = filepath		+	filename;
	bool fileFound=false;

	SCC_file.open(finalPath);
	if(SCC_file.is_open())
	{
		fileFound =true;
		while(!SCC_file.eof())
		{
			getline(SCC_file,line);
			istringstream ssi(line);
			string token;
			vector<string> token_array;
			while(std::getline(ssi,token,' '))
			{
				token_array.push_back(token);
			}

			int head_index = stoi(token_array[0]);
			int tail_index = stoi(token_array[1]);
			Node *edge = new Node();
			edge->num_Vertex = tail_index;
			Graph[head_index-1].add_Node(edge);
		}
	}
	else 
	{
		cout<<"File not found please check file path or name"<<endl;
	}

	SCC_file.close();
	if(fileFound==true)
	{
		cout<<"Finished generating the graph"<<endl;
		Graph1 = reverseGraph(Graph);
		cout<<"Finished reversing the graph"<<endl;
		DFS_main_Loop(Graph1);
		cout<<"Finished DFS on the reverse graph"<<endl;
		SCC_final_loop(Graph1,Graph);
		sort(Graph.begin(),Graph.end(),sortLeader);
		cout<<"Finished SCC on the  graph"<<endl;

		std::vector<int> LeaderCount;
		for(auto i=0;i<5;i++)
			LeaderCount.push_back(0);

		auto count =0;
		auto oldLeader=Graph[0].head->leader_num;
		bool reset =false;
		for(auto i=0;i<Graph.size();i++)
		{
			auto newLeader= Graph[i].head->leader_num;
			if( newLeader == oldLeader)
			{
				count++;
				reset = false;
			}
			else 
			{
				LeaderCount.push_back(count);
				count =1;
				oldLeader=newLeader;
				reset =true;
			}
		}

		if(reset ==false)
			LeaderCount.push_back(count);

		sort(LeaderCount.begin(),LeaderCount.end());
		for(auto i = LeaderCount.size()-1; i > LeaderCount.size()-6; i--)
			cout<<"Leader count is: "<<LeaderCount[i]<<endl;
	}

	return 0;
}