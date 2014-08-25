#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>

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

int main(int argc, const char* argv[])
{
    FILE *f = fopen("collatz_data.txt", "w");
    FILE *src = fopen("collatz_data.txt", "r");
    mpz_t hold;
    mpz_t base;
    mpz_t specimen;
    unsigned long d;

    mpz_init(hold);
    mpz_init(base);
    mpz_init(specimen);

    // prepare file
    mpz_set_ui(hold, 0);
    write_file_line(f, hold, 1);
    mpz_set_ui(hold, 1);
    write_file_line(f, hold, 2);
    fflush(f);

    int exponent = 2;
    mpz_set_ui(base, 2);
    while(exponent < 38)
    {
        // iterate through file
        fseek(src, 0, SEEK_SET);
        while(1)
        {
            read_file_line(src, hold, &d);
            if( mpz_cmp(hold, base) > 0 )
                break;
            if( d < exponent )
                continue;

            mpz_add(specimen, base, hold);
            d = glide_divides(specimen);
            write_file_line(f, specimen, d);
            fflush(f);
        }

        exponent += 1;
        mpz_mul_ui(base, base, 2);
    }
    mpz_clear(specimen);
    mpz_clear(base);
    mpz_clear(hold);

    fclose(src);
    fclose(f);
}
