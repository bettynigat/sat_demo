#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

using namespace std;

int CNF_SAT(int n, int k, vector<vector<int>>edges){
    unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    vector<int> vertexCover; 

    //create an array of literals of size n*k
    Minisat::Lit literals[k][n]; 

    //create n*k positive literals over n*k new variables
    for (int i=0; i<n; i++){
        for (int j=0; j<k; j++){
            literals[j][i] = Minisat::mkLit(solver->newVar()); 
        }
    }

    //1. Create the first set of clauses
    //at least one vertex is the ith vertex in the vertex cover:
    for (int i=0; i<k; i++){
        Minisat::vec<Minisat::Lit> tempVar;
        for (int j=0; j<n; j++){
            tempVar.push(literals[i][j]);
        }
        solver->addClause(tempVar); //also try without using tempCLAUSE 
    }

    //2. Create the second set of clauses 
    //No one vertex can appear twice in a vertex cover
    for (int m=0; m<n; m++){
        for (int p=0; p<k-1; p++){
            for (int q=p+1; q<k; q++ ){
                solver->addClause(~literals[p][m], ~literals[q][m]); 
            }
        }
    }

     //3. Create the third set of clauses
     //No more than one vertex appears in the mth position of the vertex cover
    for(int m=0; m<k; m++){
        for(int p=0; p<n-1; p++){
            for(int q=p+1; q<n; q++){
                solver->addClause(~literals[m][p], ~literals[m][q]); 
            }
        }
    }

    //4. create the fouth set of clauses
    //Every edge is incident to at least one vertex in the vertex cover
    //the atomic prop is true iff the vertext end/start is the mth vertext in the vertext cover
    for (int l=0; l<edges.size(); l++){
        Minisat::vec<Minisat::Lit> tempVar;
        int start = edges[l][0];
        int end = edges[l][1];
        for (int m=0; m<k; m++){
            tempVar.push(literals[m][end]);
            tempVar.push(literals[m][start]);
        }
        solver -> addClause(tempVar); 
    }

    //check if F is satisfiable. If so, print vertext cover and exit
    bool result = solver->solve();
    if(result==1){
        for (int i=0; i<k ; i++){
            for (int j=0; j<n; j++){
                if (Minisat::toInt(solver->modelValue(literals[i][j]))==0){
                    vertexCover.push_back(j);
                }
            }
        }

        sort(vertexCover.begin(),vertexCover.end());
        for (int i=0; i<vertexCover.size(); i++){
            if (i==vertexCover.size()-1){
                cout<<vertexCover[i]<<endl;
            }
            else{
                cout<<vertexCover[i]<<" ";
            }
        }
        solver.reset (new Minisat::Solver());
        return 1;    
    }

    //continue with the next k element
    else {
        solver.reset (new Minisat::Solver());
        return 0;
    }
}


int main(){
    int vertex_no;
    while (!cin.eof()) {
        vector<vector<int>> edges;
        string line;
        getline(cin, line); 
    
        if (line[0] == 'V'){
            istringstream input(line.substr(2,-1));
            input >> vertex_no;
        }

        else if (line[0] == 'E'){
            stringstream check1(line); 
            stringstream check2(line);
            string intermediate; 
            string intermediate2; 
            int coordinate_num = 0; 
            int num_edges=0; 

            while (getline(check2, intermediate2, '>'))
            {
                num_edges++;
            }
            num_edges--; 
            
            string coordinates[num_edges];

            size_t j = 0; //stores total no of edges

            while (getline(check1, intermediate, '>'))
            {
                string stored; 
                if (coordinate_num == 0) stored = intermediate.substr(4,6);
                else stored = intermediate.substr(2,4);
                coordinates[j] = stored; 
                coordinate_num++; 
                j++; 
                if (coordinate_num == num_edges) break;  
            }
            
            for (int i=0; i<num_edges; i++){
                istringstream input(coordinates[i]);
   
                vector<int> nums;
                
                int num1,  num2; 
                char comma; 
                input >> num1; 
                input >> comma;
                input >> num2; 
                
                nums.push_back(num1);
                nums.push_back(num2); 
                edges.push_back(nums); 
            }

            int result; 
            for (int i=1; i<vertex_no+1; i++){
                int result = CNF_SAT(vertex_no,i, edges); 
                if (result==1){ //means CNF SAT successful 
                    break; 
                }
            }
        }
    }
}