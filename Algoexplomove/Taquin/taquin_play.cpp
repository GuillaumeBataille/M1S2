#include <iostream>
#include <vector>
#include <utility>

int blank_x;
int blank_y;

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

std::pair<int, int> get_blank_position(const std::vector<std::vector<int>> &state)
{
    for (int i = 0; i < state.size(); ++i)
    {
        for (int j = 0; j < state[i].size(); ++j)
        {
            if (state[i][j] == 0)
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

bool is_valid_move(int x, int y, const std::vector<std::vector<int>> &state)
{
    return x >= 0 && x < state.size() && y >= 0 && y < state[0].size();
}

bool move_blank_left(std::vector<std::vector<int>> &state)
{
    auto [blank_x, blank_y] = get_blank_position(state);
    if (is_valid_move(blank_x, blank_y - 1, state))
    {
        state[blank_x][blank_y] = state[blank_x][blank_y - 1];
        state[blank_x][blank_y - 1] = 0;
        return true;
    }
    return false;
}

bool move_blank_right(std::vector<std::vector<int>> &state)
{
    auto [blank_x, blank_y] = get_blank_position(state);
    if (is_valid_move(blank_x, blank_y + 1, state))
    {
        state[blank_x][blank_y] = state[blank_x][blank_y + 1];
        state[blank_x][blank_y + 1] = 0;
        return true;
    }
    return false;
}

bool move_blank_up(std::vector<std::vector<int>> &state)
{
    auto [blank_x, blank_y] = get_blank_position(state);
    if (is_valid_move(blank_x - 1, blank_y, state))
    {
        state[blank_x][blank_y] = state[blank_x - 1][blank_y];
        state[blank_x - 1][blank_y] = 0;
        return true;
    }
    return false;
}

bool move_blank_down(std::vector<std::vector<int>> &state)
{
    auto [blank_x, blank_y] = get_blank_position(state);
    if (is_valid_move(blank_x + 1, blank_y, state))
    {
        state[blank_x][blank_y] = state[blank_x + 1][blank_y];
        state[blank_x + 1][blank_y] = 0;
        return true;
    }
    return false;
}
int main()
{
    srand(time(0)); // initialisation du générateur de nombres aléatoires

    std::vector<std::vector<int>> state = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}};

    blank_x = 2;
    blank_y = 2;

    int num_of_moves = 100; // nombre de mouvements à effectuer pour mélanger la grille

    for (int i = 0; i < num_of_moves; i++)
    {
        int choice = rand() % 4 + 1;
        switch (choice)
        {
        case 1:
            move_blank_up(state);
            break;
        case 2:
            move_blank_down(state);
            break;
        case 3:
            move_blank_left(state);
            break;
        case 4:
            move_blank_right(state);
            break;
        }
    }

    while (true)
    {
        display_game(state);
        std::cout << "Quel mouvement voulez-vous effectuer ?" << std::endl;
        std::cout << "1. Vers le haut" << std::endl;
        std::cout << "2. Vers le bas" << std::endl;
        std::cout << "3. Vers la gauche" << std::endl;
        std::cout << "4. Vers la droite" << std::endl;
        std::cout << "0. Quitter" << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            if (!move_blank_up(state))
            {
                std::cout << "Mouvement impossible." << std::endl;
            }
            break;
        case 2:
            if (!move_blank_down(state))
            {
                std::cout << "Mouvement impossible." << std::endl;
            }
            break;
        case 3:
            if (!move_blank_left(state))
            {
                std::cout << "Mouvement impossible." << std::endl;
            }
            break;
        case 4:
            if (!move_blank_right(state))
            {
                std::cout << "Mouvement impossible." << std::endl;
            }
            break;
        default:
            std::cout << "Choix incorrect." << std::endl;
            break;
        }
    }
    return 0;
}