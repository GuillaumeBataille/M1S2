
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

struct Node
{
    std::vector<std::vector<int>> state;
    int blank_x;
    int blank_y;
    int g_value; // valeur g (coût actuel)
    int h_value; // valeur h (heuristique)
    int f_value; // valeur f (coût total = g + h)

    bool operator<(const Node &n) const
    {
        return f_value > n.f_value;
    }
};
void display_game(const std::vector<std::vector<int>> &state)
{
    for (const auto &row : state)
    {
        for (int value : row)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
bool move_blank_up(std::vector<std::vector<int>> &state, int &blank_x, int &blank_y)
{
    if (blank_x == 0)
    {
        return false;
    }
    std::swap(state[blank_x][blank_y], state[blank_x - 1][blank_y]);
    blank_x--;
    return true;
}

bool move_blank_down(std::vector<std::vector<int>> &state, int &blank_x, int &blank_y)
{
    int size = state.size();
    if (blank_x == size - 1)
    {
        return false;
    }
    std::swap(state[blank_x][blank_y], state[blank_x + 1][blank_y]);
    blank_x++;
    return true;
}

bool move_blank_left(std::vector<std::vector<int>> &state, int &blank_x, int &blank_y)
{
    if (blank_y == 0)
    {
        return false;
    }
    std::swap(state[blank_x][blank_y], state[blank_x][blank_y - 1]);
    blank_y--;
    return true;
}

bool move_blank_right(std::vector<std::vector<int>> &state, int &blank_x, int &blank_y)
{
    int size = state.size();
    if (blank_y == size - 1)
    {
        return false;
    }
    std::swap(state[blank_x][blank_y], state[blank_x][blank_y + 1]);
    blank_y++;
    return true;
}
int manhattan_distance(const std::vector<std::vector<int>> &state, int blank_x, int blank_y)
{
    int distance = 0;
    for (int i = 0; i < state.size(); i++)
    {
        for (int j = 0; j < state[i].size(); j++)
        {
            int value = state[i][j];
            if (value != 0)
            {
                int target_x = (value - 1) / state.size();
                int target_y = (value - 1) % state.size();
                distance += abs(i - target_x) + abs(j - target_y);
            }
        }
    }
    return distance;
}

bool astar(std::vector<std::vector<int>> state, int blank_x, int blank_y)
{
    std::priority_queue<Node> q;
    Node start;
    start.state = state;
    start.blank_x = blank_x;
    start.blank_y = blank_y;
    start.g_value = 0;
    start.h_value = manhattan_distance(state, blank_x, blank_y);
    start.f_value = start.g_value + start.h_value;
    q.push(start);

    while (!q.empty())
    {
        Node current = q.top();
        q.pop();
        if (current.h_value == 0)
        {
            std::cout << "Résolu en " << current.g_value << " mouvements !" << std::endl;
            display_game(current.state);
            return true;
        }
        std::vector<std::vector<int>> new_state = current.state;
        int new_blank_x = current.blank_x;
        int new_blank_y = current.blank_y;
        if (move_blank_up(new_state, new_blank_x, new_blank_y))
        {
            Node neighbor;
            neighbor.state = new_state;
            neighbor.blank_x = new_blank_x;
            neighbor.blank_y = new_blank_y;
            neighbor.g_value = current.g_value + 1;
            neighbor.h_value = manhattan_distance(new_state, new_blank_x, new_blank_y);
            neighbor.f_value = neighbor.g_value + neighbor.h_value;
            q.push(neighbor);
        }
        new_state = current.state;
        new_blank_x = current.blank_x;
        new_blank_y = current.blank_y;
        if (move_blank_down(new_state, new_blank_x, new_blank_y))
        {
            Node neighbor;
            neighbor.state = new_state;
            neighbor.blank_x = new_blank_x;
            neighbor.blank_y = new_blank_y;
            neighbor.g_value = current.g_value + 1;
            neighbor.h_value = manhattan_distance(new_state, new_blank_x, new_blank_y);
            neighbor.f_value = neighbor.g_value + neighbor.h_value;
            q.push(neighbor);
        }
        new_state = current.state;
        new_blank_x = current.blank_x;
        new_blank_y = current.blank_y;
        if (move_blank_left(new_state, new_blank_x, new_blank_y))
        {
            Node neighbor;
            neighbor.state = new_state;
            neighbor.blank_x = new_blank_x;
            neighbor.blank_y = new_blank_y;
            neighbor.g_value = current.g_value + 1;
            neighbor.h_value = manhattan_distance(new_state, new_blank_x, new_blank_y);
            neighbor.f_value = neighbor.g_value + neighbor.h_value;
            q.push(neighbor);
        }
        new_state = current.state;
        new_blank_x = current.blank_x;
        new_blank_y = current.blank_y;
        if (move_blank_right(new_state, new_blank_x, new_blank_y))
        {
            Node neighbor;
            neighbor.state = new_state;
            neighbor.blank_x = new_blank_x;
            neighbor.blank_y = new_blank_y;
            neighbor.g_value = current.g_value + 1;
            neighbor.h_value = manhattan_distance(new_state, new_blank_x, new_blank_y);
            neighbor.f_value = neighbor.g_value + neighbor.h_value;
            q.push(neighbor);
        }
    }
    return false;
}

int main()
{
    int size;
    std::cout << "Entrez la taille de la grille (entier >= 2) : ";
    std::cin >> size;
    std::vector<std::vector<int>> state(size, std::vector<int>(size));
    display_game(state);
    int blank_x, blank_y;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            std::cin >> state[i][j];
            if (state[i][j] == 0)
            {
                blank_x = i;
                blank_y = j;
            }
        }
    }
    if (!astar(state, blank_x, blank_y))
    {
        std::cout << "Impossible de résoudre ce taquin." << std::endl;
    }
    return 0;
};
