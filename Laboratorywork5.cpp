#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "ru-RU.UTF8");
    srand(time(NULL));

    int n, is_directed, prob;
    printf("Введите порядок графа: ");
    scanf("%d", &n);
    printf("Тип графа (1 - ориентированный, 0 - неориентированный): ");
    scanf("%d", &is_directed);
    printf("Введите вероятность появления ребра (0-100%%): ");
    scanf("%d", &prob);

    int** SMEZH = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        SMEZH[i] = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        SMEZH[i][i] = ((rand() % 100) < prob) ? 1 : 0;
        for (int j = i + 1; j < n; j++)
        {
            if (is_directed)
            {
                SMEZH[i][j] = ((rand() % 100) < prob) ? 1 : 0;
                SMEZH[j][i] = ((rand() % 100) < prob) ? 1 : 0;
            }
            else
            {
                SMEZH[i][j] = SMEZH[j][i] = ((rand() % 100) < prob) ? 1 : 0;
            }
        }
    }

    printf("\nМатрица смежности:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) printf("%3d", SMEZH[i][j]);
        printf("\n");
    }

    int m_edges = 0;
    int m_loops = 0;
    for (int i = 0; i < n; i++)
    {
        if (SMEZH[i][i] == 1) m_loops++;
        for (int j = (is_directed ? 0 : i + 1); j < n; j++)
        {
            if (i != j && SMEZH[i][j] == 1) m_edges++;
        }
    }
    int stolb = m_edges + m_loops;
    int size = m_edges + m_loops;

    printf("\nРазмер графа: %d (рёбер: %d, петель: %d)\n", size, m_edges, m_loops);

    int* out_deg = (int*)calloc(n, sizeof(int));
    int* in_deg = (int*)calloc(n, sizeof(int));

    if (is_directed)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (SMEZH[i][j] == 1)
                {
                    out_deg[i]++;
                    in_deg[j]++;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            out_deg[i] = 2 * SMEZH[i][i];
            for (int j = 0; j < n; j++)
                if (j != i) out_deg[i] += SMEZH[i][j];
        }
    }

    if (!is_directed)
    {
        printf("Изолированные: ");
        int f = 0;
        for (int i = 0; i < n; i++) if (out_deg[i] == 0) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Концевые: ");
        f = 0;
        for (int i = 0; i < n; i++) if (out_deg[i] == 1) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Доминирующие: ");
        f = 0;
        for (int i = 0; i < n; i++)
        {
            int all = 1;
            for (int j = 0; j < n; j++)
                if (j != i && SMEZH[i][j] != 1) { all = 0; break; }
            if (all) { printf("%d ", i + 1); f = 1; }
        }
        if (!f) printf("отказано(нет таких)");
        printf("\n");
    }
    else
    {
        printf("Изолированные: ");
        int f = 0;
        for (int i = 0; i < n; i++) if (in_deg[i] == 0 && out_deg[i] == 0) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Концевые: ");
        f = 0;
        for (int i = 0; i < n; i++) if (in_deg[i] + out_deg[i] == 1) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Доминирующие: ");
        f = 0;
        for (int i = 0; i < n; i++)
        {
            int all = 1;
            for (int j = 0; j < n; j++)
                if (j != i && SMEZH[i][j] != 1 && SMEZH[j][i] != 1) { all = 0; break; }
            if (all) { printf("%d ", i + 1); f = 1; }
        }
        if (!f) printf("отказано(нет таких)");
        printf("\n");
    }

    int** INZ = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        INZ[i] = (int*)calloc(stolb > 0 ? stolb : 1, sizeof(int));

    int k = 0;

    for (int i = 0; i < n; i++)
    {
        if (SMEZH[i][i] == 1)
        {
            INZ[i][k] = 2;
            k++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = (is_directed ? 0 : i + 1); j < n; j++)
        {
            if (i != j && SMEZH[i][j] == 1)
            {
                if (is_directed)
                {
                    INZ[i][k] = 1;
                    INZ[j][k] = -1;
                }
                else
                {
                    INZ[i][k] = 1;
                    INZ[j][k] = 1;
                }
                k++;
            }
        }
    }

    printf("\nМатрица инцидентности:\n");
    for (int i = 0; i < n; i++)
    {
        for (int p = 0; p < stolb; p++) printf("%3d", INZ[i][p]);
        printf("\n");
    }

    printf("\nРазмер графа: %d\n", size);

    for (int i = 0; i < n; i++)
    {
        in_deg[i] = 0;
        out_deg[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        for (int p = 0; p < stolb; p++)
        {
            if (is_directed)
            {
                if (INZ[i][p] == 1 || INZ[i][p] == 2) out_deg[i]++;
                if (INZ[i][p] == -1 || INZ[i][p] == 2) in_deg[i]++;
            }
            else
            {
                out_deg[i] += INZ[i][p];
            }
        }
    }

    if (!is_directed)
    {
        printf("Изолированные: ");
        int f = 0;
        for (int i = 0; i < n; i++) if (out_deg[i] == 0) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Концевые: ");
        f = 0;
        for (int i = 0; i < n; i++) if (out_deg[i] == 1) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Доминирующие: ");
        f = 0;
        for (int i = 0; i < n; i++)
        {
            int all = 1;
            for (int j = 0; j < n; j++)
            {
                if (i == j) continue;
                int found = 0;
                for (int p = 0; p < stolb; p++)
                {
                    if (INZ[i][p] == 1 && INZ[j][p] == 1) { found = 1; break; }
                }
                if (!found) { all = 0; break; }
            }
            if (all) { printf("%d ", i + 1); f = 1; }
        }
        if (!f) printf("отказано(нет таких)");
        printf("\n");
    }
    else
    {
        printf("Изолированные: ");
        int f = 0;
        for (int i = 0; i < n; i++) if (in_deg[i] == 0 && out_deg[i] == 0) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Концевые : ");
        f = 0;
        for (int i = 0; i < n; i++) if (in_deg[i] + out_deg[i] == 1) { printf("%d ", i + 1); f = 1; }
        if (!f) printf("отказано(нет таких)");
        printf("\n");

        printf("Доминирующие: ");
        f = 0;
        for (int i = 0; i < n; i++)
        {
            int all = 1;
            for (int j = 0; j < n; j++)
            {
                if (i == j) continue;
                int found = 0;
                for (int p = 0; p < stolb; p++)
                {
                    if ((INZ[i][p] == 1 || INZ[i][p] == 2) && (INZ[j][p] == -1 || INZ[j][p] == 2)) { found = 1; break; }
                    if ((INZ[i][p] == -1 || INZ[i][p] == 2) && (INZ[j][p] == 1 || INZ[j][p] == 2)) { found = 1; break; }
                }
                if (!found) { all = 0; break; }
            }
            if (all) { printf("%d ", i + 1); f = 1; }
        }
        if (!f) printf("отказано(нет таких)");
        printf("\n");
    }

    for (int i = 0; i < n; i++) { free(SMEZH[i]); free(INZ[i]); }
    free(SMEZH);
    free(INZ);
    free(in_deg);
    free(out_deg);

    return 0;
}