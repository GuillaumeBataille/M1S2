#include <unordered_map>
#include <vector>

using namespace std;

const int N = 3;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
unordered_map<int, int> visited;
vector<vector<int>> state(N, vector<int>(N));

int hash(vector<vector<int>> &state)
{
    int res = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            res = res * 10 + state[i][j];
    return res;
}

int bfs(vector<vector<int>> &start, vector<vector<int>> &goal, int n)
{
    queue<pair<vector<vector<int>>, int>> q;
    q.push({start, 0});
    visited[hash(start)] = 0;
    while (!q.empty())
    {
        auto t = q.front();
        q.pop();
        vector<vector<int>> curr = t.first;
        int depth = t.second;
        if (curr == goal)
            return depth;
        int x, y;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (curr[i][j] == 0)
                {
                    x = i;
                    y = j;
                }
        for (int i = 0; i < 4; i++)
        {
            int a = x + dx[i];
            int b = y + dy[i];
            if (a >= 0 && a < n && b >= 0 && b < n)
            {
                vector<vector<int>> next = curr;
                swap(next[x][y], next[a][b]);
                int h = hash(next);
                if (!visited.count(h))
                {
                    q.push({next, depth + 1});
                    visited[h] = depth + 1;
                }
            }
        }
    }
    return -1;
}

int main()
{
    cout << "Enter the start state of the puzzle: " << endl;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> state[i][j];
    cout << "Enter the goal state of the puzzle: " << endl;

    vector<vector<int>> goal(N, vector<int>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> goal[i][j];
    int depth = bfs(state, goal, N);
    if (depth == -1)
    {
        cout << "No solution found." << endl;
    }
    else
    {
        cout << "Solution found in " << depth << " moves." << endl;
    }
    return 0;
}