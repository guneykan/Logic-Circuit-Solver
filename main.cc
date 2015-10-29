#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include<string>
#include <queue>
using namespace std;

//Element Of the Circuit.
struct Node{

	int type;  //Type of the element in the circuit.
	/*(0)->Input (1)->Negate Gate (2)->OR Gate (3)->AND Gate (4)->XOR Gate */
	int index; //Index number of this element.
	bool data;  //Truth value Of this element.
	vector<Node*> children;//Children Of this node.

};
//Keep the nodes in an array.
vector<Node>nodes;//Nodes.
//Reprent circuit as adjacent matrix.
vector< vector< int > > adjmat;
int main(int argc, char **argv){
	bool checkCycle(vector<vector<int> > &graph,int N,int M, int K);	
	int findchildren( int index,int N,int M,int K);
	void calculate(Node& node);
	void orOperate(Node& node);
	void andOperate(Node& node);
	void negateOperate(Node& node);
	void exclusiveOperate(Node& node);
	//Read the input file.	
	string i=argv[1];
	string o=argv[2];	
	ifstream input;
	input.open(i.c_str() );

	int N;
	int M;
	int K;

	input >> N;//Number Of Inputs.
	input >> M;//Number Of Gates.
	input >> K;//Number Of Outputs.
	vector< vector< int > > adjmat2(N+M+K,vector< int >(N+M+K));
	adjmat=adjmat2;
	ofstream output;
	output.open(o.c_str());

	
	//Create input nodes.

	for (int i = 0; i < N; i++){

		Node node;
		node.index = i;
		node.type = 0;
		int a;
		input >> a;
		if (a == 1)
		node.data = true;
		else if (a == 0)
		node.data = false;
		nodes.push_back(node);
		
	}

	//Create gates.
	for (int i = N; i < N + M; i++){

		Node node;
		node.index = i;
		string a;
		input >> a;

		if (a=="N"){
			node.type = 1;
		}
		if (a=="A"){
			node.type = 2;
		}
		if (a=="R"){
			node.type = 3;
		}
		if (a=="X"){
			node.type = 4;
		}

		nodes.push_back(node); 

	}

	//Create output Nodes.
	for (int i = N + M; i < N + M + K; i++){


		Node node;
		string q;

		input>>q;
		node.index = i;
		node.type = 5;
		nodes.push_back(node);

	}
	

	for (int i = 0; i < N + M + K;i++){
		string sub;
		int numb;
		string s;
		getline(input, s);
		istringstream iss(s);
		string consume;
		iss>>consume;
		while(iss>>sub){	
			
			
			
			
			stringstream ( sub ) >>numb ;

			adjmat[i-1][numb] = 1;

		} 
		adjmat[i][i] = 0;

	}


	
	
	
	
	
	//Check if there is a cycle in the circuit.
	if(checkCycle(adjmat, N, M, K)){


		output<<-1;
		return 1;
	}

	//Create a parental relation starting from outputs.
	for (int i = N + M; i < N + M + K; i++){

		queue< Node*> q;
		
		Node* root = &nodes[i];
		q.push(root);
		
		while ((q.empty())==false){
			Node* node = q.front();

			q.pop();


			int size=findchildren(node->index,N,M,K);
			
			for (int j = 0; j <size; j++){
				if(node->children[j]->type!=0)
				q.push((node->children[j]));

			}
			
			


		}


		calculate(*(nodes[i].children[0]));
		nodes[i].data=(nodes[i].children[0]->data);
		
	}	
	for(int i=0;i<M+N+K;i++){
		if(i!=M+N+K-1){
			if(nodes[i].data==false){

				output<<"0"<<" ";	
			}else if(nodes[i].data=true){

				output<<"1"<<" ";
			}
		}else{

			if(nodes[i].data==false){

				output<<"0";	
			}else if(nodes[i].data=true){

				output<<"1";
			}



		}
		


	}

}
//Topological Sort.
bool checkCycle(vector<vector<int> >& graph,int N,int M,int K){

	int indeg[N+M+K];
	bool visited[N+M+K];
	int count=0;
	int cycle=0;

	for(int i=0;i<N+M+K;i++){

		indeg[i]=0;
		visited[i]=false;
	}



	for(int i=0;i<N+M+K;i++){
		for(int j=0;j<M+N+K;j++){

			indeg[i]+=graph[j][i];
			

		}
	}	



	
	while(count<N+M+K){

		int i=0;
		for(i=0;i<N+M+K;i++){
			
			if(indeg[i]==0 && visited[i]==false){	
				visited[i]=true;
				count++;		
				break;
			}
			
		}
		if(i==N+M+K && count<N+M+K){
			
			return true;
		}



		for(int j=0; j<N+M+K;j++){

			indeg[j]-=graph[i][j];
			
		}
	}
	return false;



}
//Find the children of an element.
int findchildren(int index,int N,int M,int K){


	for (int i = 0; i<N + M + K; i++){

		if (adjmat[i][index] == 1){
			Node* node = &nodes[i];
			if(nodes[index].children.size()!=2)
			nodes[index].children.push_back(node);

		}

	}
	

	return nodes[index].children.size();

}



//Find the truth value of a node.
void calculate(Node& node){
	void findchildren( int& index,int N,int M,int K);
	void calculate(Node& node);
	void orOperate(Node& node);
	void andOperate(Node& node);
	void negateOperate(Node& node);
	void exclusiveOperate(Node& node);

	if(node.type==0)
	return;

	else if (node.type == 1){
		
		negateOperate(node);
	}
	else if (node.type == 3){
		orOperate(node);
	}
	else if (node.type == 2){

		andOperate(node);



	}
	else if (node.type == 4){

		exclusiveOperate(node);
	}








}

//Performs 'and' operation.
void andOperate(Node& node){

	for(int i=0;i<node.children.size();i++){
		calculate(*(node.children[i]));
	}
	node.data = node.children[0]->data && node.children[1]->data;
	
}

//Performs 'or' operation.
void orOperate(Node& node) {

	for(int i=0;i<node.children.size();i++){
		calculate(*(node.children[i]));
	}
	node.data = node.children[0]->data || node.children[1]->data;
}




//Performs the negation.
void negateOperate(Node& node){
	for(int i=0;i<node.children.size();i++){
		calculate(*(node.children[i]));
	}
	node.data = !((node.children[0]->data));

}






//Performs 'exclusive or' operation.
void exclusiveOperate(Node& node){
	
	for(int i=0;i<node.children.size();i++){
		calculate(*(node.children[i]));
	}
	node.data = node.children[0]->data xor node.children[1]->data;




}




