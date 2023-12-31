#include "header.hpp"
#include <array>
#include <regex>

string last(const vector<string> &v) { return v[v.size() - 1]; }

vector<int> get_all_integers(const string &str) {
    std::regex number_regex("[-]?\\d+");
    vector<int> numbers;
    for (std::sregex_iterator iterator(str.begin(), str.end(), number_regex), end; iterator != end;
         ++iterator) {
        std::smatch match = *iterator;
        numbers.push_back(std::stoi(match.str()));
    }
    return numbers;
}

struct Character {
    int health;
    int damage;
    int armor;
};

struct Item {
    int cost;
    int damage;
    int armor;
};

bool fight(const Character &hero, const Character &boss) {
    return ceil(boss.health / fmax(hero.damage - boss.armor, 1.0f)) <=
           ceil(hero.health / fmax(boss.damage - hero.armor, 1.0f));
    // opponent.health / max(damage - opponent.armor) is the number of rounds to defeat the opponent
    // if it's not whole, numbers is rounded UP (ceil)
    // if the number of rounds to defeat the opponent is the same for a hero and for a boss,
    // hero wins as he starts first

    // produces the same result, but unnesessary extra copy and loop
    // while (true) {
    //     boss.health -= fmax(hero.damage - boss.armor, 1);
    //     if (boss.health <= 0) return true;
    //     hero.health -= fmax(boss.damage - hero.armor, 1);
    //     if (hero.health <= 0) return false;
    // }
}

int equip(Character &hero, const Item &item) {
    hero.armor += item.armor;
    hero.damage += item.damage;
    return item.cost;
}

void fill_items(std::array<Item, 5> &weapons, std::array<Item, 6> &armors,
                std::array<Item, 7> &rings) {
    auto lines = read_file_by_lines("input/day_21_0.txt");
    for (int i = 0; i < 5; i++) {
        auto numbers  = get_all_integers(lines[i + 1]);
        weapons.at(i) = Item{numbers[0], numbers[1], numbers[2]};
        numbers       = get_all_integers(lines[i + 8]);
        armors.at(i)  = Item{numbers[0], numbers[1], numbers[2]};
    }
    for (int i = 0; i < 6; i++) {
        auto numbers = get_all_integers(lines[i + 15]);
        rings.at(i)  = Item{numbers[1], numbers[2], numbers[3]};
    }
    armors.at(5) = {0, 0, 0};
    rings.at(6)  = {0, 0, 0};
}

std::pair<int, int> calculate_cost_of_rpg_fight() {
    std::array<Item, 5> weapons;
    std::array<Item, 6> armors;
    std::array<Item, 7> rings;
    fill_items(weapons, armors, rings);
    auto boss_numbers = get_all_integers(read_entire_file("input/day_21.txt"));
    Character boss{boss_numbers[0], boss_numbers[1], boss_numbers[2]};
    Character hero;
    int max_cost = 0;
    int min_cost = INT32_MAX;
    for (auto &weapon : weapons) {
        for (auto &armor : armors) {
            for (auto &ring1 : rings) {
                for (auto &ring2 : rings) {
                    hero     = {100, 0, 0};
                    int cost = equip(hero, ring1) + equip(hero, ring2) + equip(hero, weapon) +
                               equip(hero, armor);
                    bool won = fight(hero, boss);
                    if (cost < min_cost && won) min_cost = cost;
                    if (cost > max_cost && !won) max_cost = cost;
                }
            }
        }
    }
    return {min_cost, max_cost};
}

int main() {
    assert(fight({8, 5, 5}, {12, 7, 2}));
    auto [min, max] = calculate_cost_of_rpg_fight();
    print("Part 1 answer is", min);
    print("Part 1 answer is", max);
    return 0;
}