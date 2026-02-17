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
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <seed> <radius_chunks>\n", argv[0]);
        fprintf(stderr, "Example: %s 123456789 64\n", argv[0]);
        return 2;
    }

    uint64_t seed = parse_u64(argv[1]);

    errno = 0;
    char *end = NULL;
    long r = strtol(argv[2], &end, 10);
    if (errno != 0 || end == argv[2] || *end != '\0' || r < 0 || r > 2000000) {
        fprintf(stderr, "Invalid radius_chunks: '%s'\n", argv[2]);
        return 2;
    }
    int R = (int)r;

    Generator g;
    setupGenerator(&g, MC_1_21, 0);
    applySeed(&g, DIM_OVERWORLD, seed);

    Pos spawn = estimateSpawn(&g, NULL);

    int scx = floorDiv16(spawn.x);
    int scz = floorDiv16(spawn.z);

    int min_cx = scx - R;
    int max_cx = scx + R;
    int min_cz = scz - R;
    int max_cz = scz + R;

    // (2R+1)^2 chunks total
    long long side = (long long)2 * (long long)R + 1;
    long long total = side * side;

    printf("Seed: %" PRIu64 "\n", seed);
    printf("Spawn block: (%d, %d)\n", spawn.x, spawn.z);
    printf("Spawn chunk: (%d, %d)\n", scx, scz);
    printf("Radius (chunks): %d\n", R);
    printf("Scan window chunks:\n");
    printf("  cx: [%d, %d]\n", min_cx, max_cx);
    printf("  cz: [%d, %d]\n", min_cz, max_cz);
    printf("Total chunks to scan: %lld\n", total);

    return 0;
}