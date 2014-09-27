#include <stdio.h>

unsigned char data[128*1024]={0};

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
}
