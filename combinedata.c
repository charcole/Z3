#include <stdio.h>

unsigned char data[128*1024]={0};

#define CALLBACK_BASE 0x1F80B
#define INIT_CALLBACK CALLBACK_BASE+0
#define PRINT_CALLBACK CALLBACK_BASE+5
#define PRINTCHAR_CALLBACK CALLBACK_BASE+10
#define PRINTNUM_CALLBACK CALLBACK_BASE+15
#define READ_CALLBACK CALLBACK_BASE+20

int main()
{
	FILE *f;
	f=fopen("Planetfa.z3", "rb");
	fread(&data[0], 1, 0x20000, f);
	fclose(f);
	f=fopen("zfont.dat", "rb");
	fread(&data[0x1F200], 1, 0x500, f);
	fclose(f);
	f=fopen("encoding.txt", "rb");
	fread(&data[0x1F700], 1, 0x100, f);
	f=fopen("bios.z3", "rb");
	fseek(f, 0x1F800, SEEK_SET);
	fread(&data[0x1F800], 1, 0x800, f);
	fclose(f);
	f=fopen("rom.z3","wb");
	fwrite(data, 1, sizeof(data), f);
	fclose(f);

	printf("Insert this into the Verilog...\n");
	printf("`define INIT_CALLBACK\t\t'h%02x%02x\n", data[INIT_CALLBACK], data[INIT_CALLBACK+1]);
	printf("`define PRINT_CALLBACK\t\t'h%02x%02x\n", data[PRINT_CALLBACK], data[PRINT_CALLBACK+1]);
	printf("`define PRINTCHAR_CALLBACK\t'h%02x%02x\n", data[PRINTCHAR_CALLBACK], data[PRINTCHAR_CALLBACK+1]);
	printf("`define PRINTNUM_CALLBACK\t'h%02x%02x\n", data[PRINTNUM_CALLBACK], data[PRINTNUM_CALLBACK+1]);
	printf("`define READ_CALLBACK\t\t'h%02x%02x\n", data[READ_CALLBACK], data[READ_CALLBACK+1]);
}
