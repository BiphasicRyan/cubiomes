#include "generator.h"
#include "finders.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static inline int floorDiv16(int v)
{
    // Portable floor division by 16 (Minecraft chunk coord rule).
    if (v >= 0) return v / 16;
    return -(((-v) + 15) / 16);
}

static uint64_t parse_u64(const char *s)
{
    errno = 0;
    char *end = NULL;
    unsigned long long v = strtoull(s, &end, 0);
    if (errno != 0 || end == s || *end != '\0') {
        fprintf(stderr, "Invalid seed: '%s'\n", s);
        exit(2);
    }
    return (uint64_t)v;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
        fprintf(stderr, "Example: %s 123456789\n", argv[0]);
        return 2;
    }

    uint64_t seed = parse_u64(argv[1]);

    Generator g;
    setupGenerator(&g, MC_1_21, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    Pos spawn = estimateSpawn(&g, NULL);

    int scx = floorDiv16(spawn.x);
    int scz = floorDiv16(spawn.z);

    printf("Seed: %" PRIu64 "\n", seed);
    printf("Spawn block: (%d, %d)\n", spawn.x, spawn.z);
    printf("Spawn chunk: (%d, %d)\n", scx, scz);

    return 0;
}