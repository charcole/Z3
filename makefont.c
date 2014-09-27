#include <ft2build.h>
#include FT_FREETYPE_H

unsigned char data[128][10];

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
}

int main()
{
	fontLoad("zfont.ttf");
	FILE *f=fopen("zfont.dat", "w");
	fwrite(data, sizeof(data), 1, f);
	fclose(f);
	return 0;
}
