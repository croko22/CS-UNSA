#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class AFNtoAFD
{
    vector<set<int>> E_closure;
    map<pair<set<int>, int>, set<int>> D_tran;
    vector<set<int>> D_states;
    set<int> states, accept_states, inputs;
    map<int, vector<pair<int, int>>> transitions;

    set<int> E_closure_function(set<int> R)
    {
        stack<int> stack;
        set<int> closure = R;
        for (int r : R)
            stack.push(r);
        while (!stack.empty())
        {
            int r = stack.top();
            stack.pop();
            for (auto &tran : transitions[r])
            {
                if (tran.second == -1 && closure.find(tran.first) == closure.end())
                {
                    closure.insert(tran.first);
                    stack.push(tran.first);
                }
            }
        }
        return closure;
    }

    set<int> move(set<int> R, int x)
    {
        set<int> result;
        for (int r : R)
        {
            for (auto &tran : transitions[r])
            {
                if (tran.second == x)
                    result.insert(tran.first);
            }
        }
        return result;
    }

public:
    void convert()
    {
        set<int> initial = {0};
        set<int> initial_closure = E_closure_function(initial);
        D_states.push_back(initial_closure);
        E_closure.push_back(initial_closure);

        for (size_t i = 0; i < D_states.size(); ++i)
        {
            set<int> R = D_states[i];
            for (int x : inputs)
            {
                set<int> U = E_closure_function(move(R, x));
                if (U.empty())
                    continue;
                if (find(D_states.begin(), D_states.end(), U) == D_states.end())
                {
                    D_states.push_back(U);
                    E_closure.push_back(U);
                }
                D_tran[{R, x}] = U;
            }
        }
    }

    void read_input(const string &filename)
    {
        ifstream infile(filename);
        string line;

        getline(infile, line); // Estados
        istringstream ss(line.substr(8));
        int state;
        while (ss >> state)
            states.insert(state);

        getline(infile, line); // Estados de Aceptación
        ss.clear();
        ss.str(line.substr(21));
        while (ss >> state)
            accept_states.insert(state);

        getline(infile, line); // Entradas
        ss.clear();
        ss.str(line.substr(9));
        int input;
        while (ss >> input)
            inputs.insert(input);

        getline(infile, line); // Transiciones
        while (getline(infile, line))
        {
            if (line.empty())
                break;
            ss.clear();
            ss.str(line);
            int from, to, symbol;
            ss >> from >> to >> symbol;
            transitions[from].push_back({to, symbol});
        }
    }

    void write_output(const string &filename)
    {
        ofstream outfile(filename);

        outfile << "Estados\n";
        for (size_t i = 0; i < D_states.size(); ++i)
        {
            outfile << i << " = { ";
            for (int state : D_states[i])
                outfile << state << " ";
            outfile << "}\n";
        }

        outfile << "Estados de Aceptacion\n";
        for (size_t i = 0; i < D_states.size(); ++i)
        {
            for (int state : D_states[i])
            {
                if (accept_states.find(state) != accept_states.end())
                {
                    outfile << i << " ";
                    break;
                }
            }
        }
        outfile << "\nTransiciones de Estados (x,y,z)\n";
        for (auto &tran : D_tran)
        {
            auto [R, x] = tran.first;
            set<int> U = tran.second;
            int from = find(D_states.begin(), D_states.end(), R) - D_states.begin();
            int to = find(D_states.begin(), D_states.end(), U) - D_states.begin();
            outfile << from << " " << x << " " << to << "\n";
        }
    }

    void generate_graphviz(const string &filename)
    {
        ofstream outfile(filename);
        outfile << "digraph G {" << endl;

        for (size_t i = 0; i < D_states.size(); ++i)
        {
            outfile << "    " << i;
            if (!accept_states.empty() && accept_states.find(*D_states[i].begin()) != accept_states.end())
                outfile << " [shape=doublecircle];" << endl;
            else
                outfile << " [shape=circle];" << endl;
        }

        for (auto &tran : D_tran)
        {
            auto [R, x] = tran.first;
            set<int> U = tran.second;
            int from = find(D_states.begin(), D_states.end(), R) - D_states.begin();
            int to = find(D_states.begin(), D_states.end(), U) - D_states.begin();
            outfile << "    " << from << " -> " << to << " [label=\"" << x << "\"];" << endl;
        }

        outfile << "}" << endl;
        outfile.close();
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Uso: " << argv[0] << " <archivo de entrada> <archivo de salida>\n";
        return 1;
    }

    AFNtoAFD converter;
    converter.read_input(argv[1]);
    converter.convert();
    converter.write_output(argv[2]);

    string graphviz_file = "graphviz.dot";
    converter.generate_graphviz(graphviz_file);

    return 0;
}
