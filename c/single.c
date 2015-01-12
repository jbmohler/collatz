#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
//#include <pthread.h>

int glide_divides(mpz_t n)
{
    int d = 0;
    int m = 0;
    mpz_t temp;
    mpz_init_set(temp, n);
    while(mpz_cmp(temp, n)>0 || (d+m) < 1)
    {
        if(1 == mpz_tstbit(temp, 0))
        {
            m = m + 1;
            mpz_mul_ui(temp, temp, 3);
            mpz_add_ui(temp, temp, 1);
        }
        else
        {
            d = d + 1;
            mpz_fdiv_q_2exp(temp, temp, 1);
        }
    }
    mpz_clear(temp);
    return d;
}

typedef struct __attribute__((__packed__)) 
{
    char n16[16];
    char space[1];
    char d10[10];
    char newline[1];
} GLIDE_DIV;

int write_struct_line(GLIDE_DIV *f, mpz_t n, unsigned long divides)
{
    // 16 hex digits (right aligned); ' '; 10 decimal digits (divides); '\n'
    char temp[18];
    mpz_get_str(temp, 16, n);
    sprintf((char*)f, "%16s %10lu\n", temp, divides);
}

int read_struct_line(GLIDE_DIV *f, mpz_t n, unsigned long *divides)
{
    char line[28];
    memcpy(line, f, 28);
    line[16] = 0;
    line[27] = 0;
    mpz_set_str(n, line, 16);
    *divides = strtoul(&line[17], NULL, 10);
}

int write_file_line(FILE *f, mpz_t n, unsigned long divides)
{
    // 16 hex digits (right aligned); ' '; 10 decimal digits (divides); '\n'
    char line[28];
    char temp[18];
    mpz_get_str(temp, 16, n);
    sprintf(line, "%16s %10lu\n", temp, divides);
    fwrite(line, 1, 28, f);
}

int read_file_line(FILE *f, mpz_t n, unsigned long *divides)
{
    char line[28];
    fread(line, 1, 28, f);
    line[16] = 0;
    line[27] = 0;
    mpz_set_str(n, line, 16);
    *divides = strtoul(&line[17], NULL, 10);
}

#define MAX_COUNT  (1<<20)

int main(int argc, const char* argv[])
{
    GLIDE_DIV *store = (GLIDE_DIV*)malloc(sizeof(GLIDE_DIV)*MAX_COUNT);
    mpz_t hold;
    mpz_t base;
    mpz_t specimen;
    unsigned long d;

    mpz_init(hold);
    mpz_init(base);
    mpz_init(specimen);

    // prepare file
    int total_index = 0;
    mpz_set_ui(hold, 0);
    write_struct_line(&store[total_index++], hold, 1);
    mpz_set_ui(hold, 1);
    write_struct_line(&store[total_index++], hold, 2);

    int base_max_index = 2;

    int exponent = 2;
    mpz_set_ui(base, 2);
    while(exponent < 38)
    {
        // iterate through struct
	int index = 0;
        while(index < base_max_index)
        {
            read_struct_line(&store[index++], hold, &d);
            if( d < exponent )
                continue;

            mpz_add(specimen, base, hold);
            d = glide_divides(specimen);
            write_struct_line(&store[total_index++], specimen, d);
	    if( total_index == MAX_COUNT )
                break;
        }
        if( total_index == MAX_COUNT )
	    break;

        exponent += 1;
	base_max_index = total_index;
        mpz_mul_ui(base, base, 2);
    }
    mpz_clear(specimen);
    mpz_clear(base);
    mpz_clear(hold);

    FILE *f = fopen("collatz_data.txt", "w");
    //FILE *src = fopen("collatz_data.txt", "r");

    fwrite(store, MAX_COUNT, 28, f);
    free(store);

    // fclose(src);
    fclose(f);
}
