#include <stdio.h>
#include <string.h>


struct temp {
	double celsius;
	double fahrenheit;
	double kelvin;
};

enum temptype {
	CELSIUS,
	FAHRENHEIT,
	KELVIN
};


int make_temp(struct temp*, double, enum temptype);
void parse_args(int, char**, enum temptype*, double*, unsigned char*);


int main(int argc, char** argv) {
	if (argc <= 1) {
		puts("Usage: temp FLAG TEMP");
		return 1;
	}

	unsigned char unset = 0x03;
	enum temptype WHICH;
	double init = 0.;
	parse_args(argc, argv, &WHICH, &init, &unset);
	if (unset) {
		puts("Usage: temp FLAG TEMP");
		return 1;
	}

	struct temp t;
	make_temp(&t, init, WHICH);

	printf("Celsius:\t%-+#10.2lfC\n", t.celsius);
	printf("Fahrenheit:\t%-+#10.2lfF\n", t.fahrenheit);
	printf("Kelvin:\t\t%-+#10.2lfK\n", t.kelvin);

	return 0;
}


void parse_args(int argc, char** argv, enum temptype* t, double* init, unsigned char* unset) {
	for (int i = 1; i < argc; i++) {
		int len = strlen(argv[i]);
		if (len == 0) continue;
		int j = 0;

		if (argv[i][0] == '-' && len > 1) {
			switch (argv[i][1]) {
				case 'c': case 'C':
					*t = CELSIUS;
					*unset &= ~0x01;
					break;
				case 'f': case 'F':
					*t = FAHRENHEIT;
					*unset &= ~0x01;
					break;
				case 'k': case 'K':
					*t = KELVIN;
					*unset &= ~0x01;
					break;
				default:
					*unset |= 0x01;
					return; // erroneous flag arg; fail loudly
			}

			j = 2;
		}

		if (!(*unset & 0x02)) return;

		int side_of_decimal = -1;
		*init = 0.;
		for (; j < len; j++) { // implicitly limited by len
			if (argv[i][j] >= '0' && argv[i][j] <= '9') {
				if (side_of_decimal < 0) {
					*init *= 10.;
					*init += (double)(argv[i][j] - '0');
				} else {
					double val = (double)(argv[i][j] - '0');
					for (int k = 0; k < side_of_decimal; k++) val /= 10;
					side_of_decimal++;

					*init += val;
				}
			} else if (argv[i][j] == '.') {
				if (side_of_decimal < 0)
					side_of_decimal = -side_of_decimal;
				else { // malformatted temperature value
					*unset |= 0x02;
					return;
				}
			} else if (argv[i][j] < '0' || argv[i][j] > '9') {
				*unset |= 0x03;
				return;
			}
		}
		*unset &= ~0x02;
	}
}

int make_temp(struct temp* t, double init, enum temptype WHICH) {
	switch (WHICH) {
		case CELSIUS:
			t->celsius = init;
			t->fahrenheit = init * 1.8 + 32.;
			t->kelvin = init + 273.15;
			break;
		case FAHRENHEIT:
			t->fahrenheit = init;
			t->celsius = (init - 32.) / 1.8;
			t->kelvin = t->celsius + 273.15;
			break;
		case KELVIN:
			t->kelvin = init;
			t->celsius = init - 273.15;
			t->fahrenheit = t->celsius * 1.8 + 32.;
			break;
		default:
			return -1;
	}

	return 0;
}
