#include <cstdint>
#include <cstdio>
#include <random>

const char *DIRECTION[4] = {"North", "South", "West", "East"};
const char *SUIT[4] = {"♠", "♥", "♦", "♣"};
const char *RANK[13] = {"A", "K", "Q", "J", "10", "9", "8",
                        "7", "6", "5", "4", "3",  "2"};

int main() {
    uint8_t cards[52];

    std::random_device rd;
    std::mt19937 re(rd());
    uint32_t r[4] = {re(), re(), re(), re()};

    // Rank: ♠ ♥ ♦ ♣ A K Q J 10 9 8 7 6 5 4 3 2
    for (int dir = 0; dir < 4; dir++) {
        printf("%s:\n", DIRECTION[dir]);
        for (int suit = 0; suit < 4; suit++) {
            printf("%s ", SUIT[suit]);
            for (int rank = 0; rank < 13; rank++) {
                int dir_rand = (r[suit] >> (rank * 2)) & 3;
                if (dir_rand == dir)
                    printf("%s ", RANK[rank]);
            }
            printf("\n");
        }
        printf("\n");
    }
}