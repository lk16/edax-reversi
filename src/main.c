#include <stdio.h>
#include "eval.h"

void dump_array_1d_int(const char *name, const int *array, int size)
{
	printf("\n=== %s ===\n", name);
	for (int i = 0; i < size; i++)
	{
		printf("[%3d] = %d\n", i, array[i]);
	}
}

void dump_array_2d_int(const char *name, const int array[][2], int size)
{
	printf("\n=== %s ===\n", name);
	for (int i = 0; i < size; i++)
	{
		printf("[%3d] = [%d, %d]\n", i, array[i][0], array[i][1]);
	}
}

void dump_f2x(const char *name, const FeatureToCoordinate *array)
{
	printf("\n=== %s ===\n", name);
	int i = 0;
	while (array[i].n_square > 0)
	{
		printf("[%3d] n_square=%d, squares=[", i, array[i].n_square);
		for (int j = 0; j < array[i].n_square; j++)
		{
			printf("%d%s", array[i].x[j], j < array[i].n_square - 1 ? "," : "");
		}
		printf("]\n");
		i++;
	}
}

void dump_x2f(const char *name, const CoordinateToFeature *array)
{
	printf("\n=== %s ===\n", name);
	int i = 0;
	while (array[i].n_feature > 0)
	{
		printf("[%3d] n_feature=%d, features=[", i, array[i].n_feature);
		for (int j = 0; j < array[i].n_feature; j++)
		{
			printf("(i:%d,x:%d)%s", array[i].feature[j].i, array[i].feature[j].x,
				   j < array[i].n_feature - 1 ? "," : "");
		}
		printf("]\n");
		i++;
	}
}

void dump_eval_weights(const char *name)
{
	if (!EVAL_LOADED || !EVAL_WEIGHT)
	{
		printf("\n=== %s: NOT LOADED ===\n", name);
		return;
	}

	printf("\n=== %s ===\n", name);
	// Print first few values from each ply as sample
	for (int ply = 0; ply < EVAL_N_PLY; ply++)
	{
		printf("PLY %2d (first 10 values):\n", ply);
		printf("  Player 0: ");
		for (int i = 0; i < 10; i++)
		{
			printf("%6d ", EVAL_WEIGHT[0][ply][i]);
		}
		printf("...\n");
		printf("  Player 1: ");
		for (int i = 0; i < 10; i++)
		{
			printf("%6d ", EVAL_WEIGHT[1][ply][i]);
		}
		printf("...\n");
	}
}

int main(void)
{
	// First load the evaluation weights
	eval_open("data/eval.dat"); // Adjust path as needed

	// Dump all static arrays
	dump_array_1d_int("EVAL_SIZE", EVAL_SIZE, 13);
	dump_array_1d_int("EVAL_PACKED_SIZE", EVAL_PACKED_SIZE, 13);
	dump_array_1d_int("EVAL_OFFSET", EVAL_OFFSET, 47);
	dump_array_1d_int("EVAL_MAX_VALUE", EVAL_MAX_VALUE, 47);

	dump_f2x("EVAL_F2X", EVAL_F2X);
	dump_x2f("EVAL_X2F", EVAL_X2F);

	// Dump evaluation weights
	dump_eval_weights("EVAL_WEIGHT");

	// Cleanup
	eval_close();

	return 0;
}
