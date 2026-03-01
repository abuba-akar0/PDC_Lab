#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL)
    {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    int number, sum = 0;

    while (fscanf(input, "%d", &number) == 1)
    {
        sum += number;
    }

    fprintf(output, "Sum = %d\n", sum);

    fclose(input);
    fclose(output);

    printf("Sum calculated and written successfully.\n");

    return 0;
}
