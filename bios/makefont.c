#include <ft2build.h>
#include "zfont2.h"
#include FT_FREETYPE_H

unsigned char data[128][10];

void calculateKeyboard(char *output, char *line1, int s)
{
		int x=0;
		int g=0;
		int h=s/2;
		int sections=1;
		char *sep=strchr(line1,'|');
		while (sep)
		{
			sections+=2;
			sep=strchr(sep+1,'|');
		}
		g=120-sections*h+h;
		while (*line1)
		{
			int w=0;
			sep=line1;
			if (*sep=='|')
				w=data['|'][0];
			else
			{
				while (*sep!='|' && *sep!='\0')
				{
					w+=data[*sep][0];
					sep++;
				}
			}
			while (x+w/2<g)
			{
				x+=data[32][0];
				*(output++)=' ';
			}
			if (*line1=='|')
			{
				x+=data[*line1][0];
				*(output++)=*(line1++);
			}
			else
			{
				while (*line1!='|' && *line1!='\0')
				{
					x+=data[*line1][0];
					*(output++)=*(line1++);
				}
			}
			g+=s;
		}
		*(output++)=0;

}

void fontLoad(const char *ttfFont)
{
	int x,y,i;
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType(&library);
	FT_New_Face(library, ttfFont, 0, &face);
	FT_Set_Pixel_Sizes(face, 10, 10);
	for (i=0; i<128; i++)
	{
		int glyph_index = FT_Get_Char_Index(face, i);
		FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
		printf("%d(%c): %dx%d %d %d,%d\n", i, i, face->glyph->bitmap.width, face->glyph->bitmap.rows, (int)face->glyph->advance.x, face->glyph->bitmap_left, face->glyph->bitmap_top);
		for (y=0; y<face->glyph->bitmap.rows; y++)
		{
			int row=y+7-face->glyph->bitmap_top;
			if (row>=0 && row<=9)
			{
				data[i][row+1]=face->glyph->bitmap.buffer[y*face->glyph->bitmap.pitch];
				if (face->glyph->bitmap_left>0)
				{
					data[i][row+1]>>=face->glyph->bitmap_left;
				}
			}
		}
		data[i][0]=face->glyph->advance.x/64;
		if (face->glyph->bitmap_left<0)
		{
			int shift=-face->glyph->bitmap_left;
			data[i][0]+=shift;
			while (shift)
			{
				for (y=0; y<9; y++)
				{
					if (data[i][y+1]&0x80)
						break;
				}
				if (y==9)
				{
					data[i][0]--;
					for (y=0; y<9; y++)
					{
						data[i][y+1]<<=1;
					}
					shift--;
				}
				else
				{
					break;
				}
			}
		}
		data[i][0]++;
		if (i=='\n')
		{
			for (y=0; y<10; y++)
				data[i][y]=0;
		}
		printf("Adv: %d\n", data[i][0]);
		for (y=0; y<9; y++)
		{
			printf("%d%d%d%d%d%d%d%d\n",
					(data[i][y+1]&128)==0?0:1,
					(data[i][y+1]&64)==0?0:1,
					(data[i][y+1]&32)==0?0:1,
					(data[i][y+1]&16)==0?0:1,
					(data[i][y+1]&8)==0?0:1,
					(data[i][y+1]&4)==0?0:1,
					(data[i][y+1]&2)==0?0:1,
					(data[i][y+1]&1)==0?0:1);
		}
	}
	
	{
		int waiting=0;
		int i=32;
		int x;
		memset(data, 0, sizeof(data));
		for (x=0; x<width; x++)
		{
			if (header_data[10*width+x])
			{
				int k;
				for (k=1; k<10; k++)
				{
					data[i][k]=data[i][k]<<1;
					data[i][k]|=header_data[k*width+x];
				}
				data[i][0]++;
				waiting=0;
			}
			else if (!waiting)
			{
				int y=0;
				for (y=1; y<10; y++)
				{
					data[i][y]<<=8-data[i][0];
				}
				data[i][0]++;
				printf("%d(%c):\n", i, i);
				printf("Adv: %d\n", data[i][0]);
				for (y=0; y<9; y++)
				{
					printf("%d%d%d%d%d%d%d%d\n",
							(data[i][y+1]&128)==0?0:1,
							(data[i][y+1]&64)==0?0:1,
							(data[i][y+1]&32)==0?0:1,
							(data[i][y+1]&16)==0?0:1,
							(data[i][y+1]&8)==0?0:1,
							(data[i][y+1]&4)==0?0:1,
							(data[i][y+1]&2)==0?0:1,
							(data[i][y+1]&1)==0?0:1);
				}
				i++;
				waiting=1;	
			}
		}
	}

	{
		char outputBuffer[1024];
		int s=240/20;
		printf("Strings for BIOS for making the keyboard:\n");
		calculateKeyboard(outputBuffer, "q|w|e|r|t|y|u|i|o|p", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		calculateKeyboard(outputBuffer, "a|s|d|f|g|h|j|k|l", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		calculateKeyboard(outputBuffer, "z|x|c|v|b|n|m|,", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		calculateKeyboard(outputBuffer, "Q|W|E|R|T|Y|U|I|O|P", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		calculateKeyboard(outputBuffer, "A|S|D|F|G|H|J|K|L", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		calculateKeyboard(outputBuffer, "Z|X|C|V|B|N|M|,", s);
		printf("@print \"%s\n\";\n", outputBuffer);
		s=40;
		calculateKeyboard(outputBuffer, "Delete|Space|Enter", s);
		printf("@print \"%s\n\";\n", outputBuffer);
	}
}

int main()
{
	fontLoad("zfont.ttf");
	FILE *f=fopen("zfont.dat", "w");
	fwrite(data, sizeof(data), 1, f);
	fclose(f);
	return 0;
}
