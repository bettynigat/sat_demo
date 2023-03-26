
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>
#include <algorithm>
// #include <string>
#include <sstream>
#include <vector>

using namespace std;

int CNF_SAT(int n, int k, vector<vector<int>>edges){
    // cout<<"here\n";
    unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    vector<int> vertexCover; 

    //create an array of literals of size n*k
    Minisat::Lit literals[k][n]; 
   
    cout<<"0\n";
    //create n*k positive literals over n*k new variables
    for (int i=0; i<n; i++){
        for (int j=0; j<k; j++){
            literals[j][i] = Minisat::mkLit(solver->newVar()); 
        }
    }

    cout<<"1\n"; 
    //create temporary vector of clauses - later replace with normal vector
    Minisat::vec<Minisat::Lit> tempClause; 
    
    //1. Create the first set of clauses
    //at least one vertex is the ith vertex in the vertex cover:
    for (int i=0; i<k; i++){
        for (int j=0; j<n; j++){
            tempClause.push(literals[i][j]);
        }
        solver->addClause(tempClause); //also try without using tempCLAUSE
        tempClause.clear(); 
    }

    cout<<"2\n"; 

    //2. Create the second set of clauses 
    for (int m=0; m<n; m++){
        for (int p=0; p<k-1; p++){
            for (int q=p+1; q<k; q++ ){
                solver->addClause(~literals[p][m], ~literals[q][m]); 
            }
        }
    }

    cout<<"3\n";
    //for part three
    for(int m=0; m<k; m++){
        for(int p=0; p<n-1; p++){
            for(int q=p+1; q<n; q++){
                solver->addClause(~literals[m][p], ~literals[m][q]); 
            }
        }
    }

    cout<<"4\n";
    //4. create the fouth set of clauses
    // for (int i=0; i<edges.size(); i++){
    //     for(int j=0; j<edges[i].size(); j++){
    //         if (edges[i][j]>i){
    //             for (int m=0; m<k; m++){
    //                 tempClause.push(literals[m][m-1]);
    //                 tempClause.push(literals[m][edges[i][j]-1]);
    //             }
    //             solver -> addClause(tempClause);
    //             tempClause.clear(); 
    //         }
    //     }
    // }

    // for (int j=0; j<edges.size(); j++){
    //     int left = edges[j][0];
    //     int right = edges[j][1];
    //     for (int i=0; i<k; k++){
    //         tempClause.push(literals[i][left-1]);
    //         tempClause.push(literals[i][right-1]); 
    //     }
    //     solver -> addClause(tempClause);
    //     tempClause.clear(); 
    // }

    // for(int i=0; i<edges.size(); i++){
    //     for (int j=0; j<2; j++){
    //         if (edges[i][j]<i) continue;
    //         for(int w=0; w<k; w++){
    //             tempClause.push(literals[w][i]);
    //             tempClause.push(literals[w][edges[i][j]]);
    //         }
    //         solver -> addClause(tempClause);
    //         tempClause.clear(); 
    //     }
    // }

    int edge_size = edges.size(); 
    for (int a=0; a<edge_size; a++){
        Minisat::vec<Minisat::Lit> E;
        int start = edges[a][0];
        int end = edges[a][1];
        for (int b=0; b<k; b++){
            E.push(literals[b][end]);
            E.push(literals[b][start]);
        }
        solver -> addClause(E); 
    }

    cout<<"5\n";
    bool result = solver->solve();
    if(result==1){
        for (int i=0; i<k ; i++){
            for (int j=0; j<n; j++){
                if (Minisat::toInt(solver->modelValue(literals[i][j]))==0){
                    // vertexCover.push_back(j+1);
                    vertexCover.push_back(j);
                }
            }
        }

        // for (int i=0; i<edges.size(); i++){
        //     for (int j=0; j<k; j++){
        //         if (Minisat::toInt(solver->modelValue(literals[i][j]))==0){
        //             vertexCover.push_back(i);
        //         } 
        //     }
        // }

        cout<<"6\n";
        cout<<"sizeo of vertex cover is "<<vertexCover.size()<<endl; 

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
    else {
        solver.reset (new Minisat::Solver());
        return 0;
    }
}



int main(){

    // vector<vector<int> > array1;
    int vertex_no;
    int a; 
    while (!cin.eof()) {

        vector<vector<int>> array1;
        string line;
        getline(cin, line); 
        
        //line can start with V, with E or with s
        if (line[0] == 'V'){
            istringstream input(line.substr(2,-1));
            input >> vertex_no;
            a=0;
            for (int i=2;i<line.length();i++){
                a=line[i]-48+a*10;
            }
            // array1 = vector<vector<int>>(a+1); 
            // int a=0;
            // for (int i=2;i<line.length();i++){
            //     a=n[i]-48+a*10;
            // }
            // cout<<"the v enetred is "<<a<<endl; 
            //create a vector of vectors with a size of vertext_no+1
            // array1 = vector<vector<int>>(vertex_no+1)
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
            
            // graph.set_edge_no(num_edges); 

            string coordinates[num_edges];

            // graph.set_edges(); 
            

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
            
            int i; 

            for (i=0; i<num_edges; i++){
                istringstream input(coordinates[i]);
   
                vector<int> nums;
                
                
                int num1,  num2; 
                char comma; 
                input >> num1; 
                input >> comma;
                input >> num2; 
                
                nums.push_back(num1);
                nums.push_back(num2); 
                array1.push_back(nums); 
                // array1[num1].push_back(num2);
                // array1[num2].push_back(num1);
            }
            int result; 
            cout<<"vertex no: "<<vertex_no<<endl;
            cout<<"Edge size: "<<array1.size()<<endl;
            for (int i=1; i<vertex_no+1; i++){
                cout<<"cnf starting\n"; 
                int result = CNF_SAT(vertex_no,i, array1); 
                if (result==1){ //means CNF SAT successful 
                    break; 
                }
            }
            //print the values of the array1
            // for (i=0; i<array1.size(); i++){
            //     vector<int> nums = array1[i];
            //     cout << nums[0] <<", "<<nums[1]<<endl; 
            // }
        }
    }
}