/* skylark@mips.for.ever */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include "sha1.h"

#define ET_HEADER 0
#define ET_STRING 1
#define ET_INTEGER 2
#define ET_SUBDIR 3

typedef struct ent {
	int type;
	char *name;
	union {
		struct {
			int idx;
			int paridx;
			int nkids;
			int fail;
			struct ent **kids;
			struct ent *nextheader;
			struct ent *parent;
		} header;
		struct {
			int secret;
			char *str;
		} string;
		struct {
			int val;
		} integer;
		struct {
			struct ent *header;
		} subdir;
	};
} ent;

ent *hdrlist;
void addheader(ent *p)
{
	p->header.nextheader=hdrlist;
	hdrlist=p;
}
ent *findheader(char *n,int p)
{
	ent *e;
	for(e=hdrlist;e;e=e->header.nextheader)
		if(p==e->header.paridx && !strcmp(n,e->name))
			return e;
	return NULL;
}

typedef unsigned char uchar;

void getcheck(uchar *block, int len, uchar *check)
{
	uchar save[4];
	uchar res[20];
	struct sha_ctx ctx;

	memcpy(save, block+14, 4);
	memset(block+14, 0, 4);

	sha_init(&ctx);
	sha_update(&ctx, block, len);
	sha_final(&ctx);
	sha_digest(&ctx, res);

	memcpy(block+14, save, 4);

	check[0] = res[4] ^ res[3] ^ res[2] ^ res[1] ^ res[0];
	check[1] = res[9] ^ res[8] ^ res[7] ^ res[6] ^ res[5];
	check[2] = res[14] ^ res[13] ^ res[12] ^ res[11] ^ res[10];
	check[3] = res[19] ^ res[18] ^ res[17] ^ res[16] ^ res[15];
}

int checkcheck(uchar *block, int len)
{
	uchar res[4];
	getcheck(block, len, res);
	if(!memcmp(res, block+14, 4))
		return 1;
	return 0;
}

unsigned char d[131072];
struct {
	short _0;
	short _1;
	short parent;
	short _2;
	short _3;
	short nent;
	short nblk;
	char name[28];
	short _4;
	short fat[7];
} a[256];

void parse_subdir(int i, short *f, ent *hdr)
{
	ent *e=calloc(sizeof(ent),1);
	e->type=ET_SUBDIR;
	e->name=strdup(d+i+1);
	hdr->header.kids[hdr->header.nkids++]=e;
}

void parse_int(int i, short *f, ent *hdr)
{
	ent *e=calloc(sizeof(ent),1);
	e->type=ET_INTEGER;
	e->name=strdup(d+i+1);
	e->integer.val=*(int *)(d+i+28);
	hdr->header.kids[hdr->header.nkids++]=e;
}

void parse_string(int i, int s, short *f, ent *hdr)
{
	int l=*(short *)(d+i+28);
	ent *e=calloc(sizeof(ent),1);
	char x[l];
	int j,k;
	for(j=0;j<l;j++) {
		k=d[i+31]+(j>>5);
		x[j]=d[(j&31)+32*(k&15)+512*f[k>>4]];
	}
	x[l]=0;
	e->type=ET_STRING;
	e->name=strdup(d+i+1);
	e->string.secret=s;
	e->string.str=strdup(x);
	hdr->header.kids[hdr->header.nkids++]=e;
}

void parse_header(int i, short *f, ent *hdr)
{
	/* nothing for now */
}

void parse(int i, short *f, ent *hdr)
{
	switch(d[i]) {
	case 0x01:
		parse_subdir(i,f,hdr);
		break;
	case 0x02:
		parse_int(i,f,hdr);
		break;
	case 0x03:
		parse_string(i,0,f,hdr);
		break;
	case 0x04:
		parse_string(i,1,f,hdr);
		break;
	case 0x0f:
	case 0x10:
	case 0x1f:
	case 0x20:
	case 0x2f:
	case 0x30:
	case 0x3f:
		parse_header(i,f,hdr);
		break;
	case 0x00:
		/* WTF? */
		break;
	default:
		fprintf(stderr,"UNKNOWN TYPE <%02x> [@%06x]\n", d[i], i);
	}
}

void walk_fatents(void)
{
	int i,j;
	uchar *buf;
	for(i=0;i<256;i++)
		if(a[i].nblk) {
			ent *e=calloc(sizeof(ent),1);
			e->type=ET_HEADER;
			e->name=strdup(a[i].name);
			e->header.idx=i;
			e->header.paridx=a[i].parent;
			e->header.kids=calloc(sizeof(ent *),a[i].nent);

			/* reassemble segments
			   it can be done better in-place
			   bleh i don't care foo */
			buf=malloc(a[i].nblk*512);
			for(j=0;j<a[i].nblk;j++)
				memcpy(buf+512*j,d+512*a[i].fat[j],512);
			free(buf);
			if(checkcheck(buf,a[i].nblk*512))
				fprintf(stderr,"PASS 0x%02X '%s'\n",i,e->name);
			else {
				fprintf(stderr,"FAIL 0x%02X '%s'\n",i,e->name);
				e->header.fail=1;
			}

			/* walk the walk */
			for(j=0;j<=a[i].nent;j++)
				parse(32*(j&15)+512*a[i].fat[j>>4],a[i].fat,e);
			addheader(e);
		}
}

void resolve_refs(void)
{
	ent *i,*f;
	int j;
	for(i=hdrlist;i;i=i->header.nextheader)
		for(j=0;j<i->header.nkids;j++)
			if(i->header.kids[j]->type==ET_SUBDIR) {
				f=findheader(i->header.kids[j]->name,i->header.idx);
				if(!f) {
					fprintf(stderr,"UNRESOLVED REFERENCE '%s'\n",i->header.kids[j]->name);
					exit(1);
				}
				i->header.kids[j]->subdir.header=f;
				f->header.parent=i;
			}
}

void dump_header(ent *h,int d)
{
	int i;
	char s[2*d+1];
	for(i=0;i<2*d;i++)
		s[i]=' ';
	s[2*d]=0;
	printf("%s<dir name=\"%s\" check=\"%s\">\n",s,h->name,h->header.fail?"fail":"pass");
	for(i=0;i<h->header.nkids;i++)
		switch(h->header.kids[i]->type) {
		case ET_STRING:
			printf("%s  <%s name=\"%s\">",s, h->header.kids[i]->string.secret?"secret":"string", h->header.kids[i]->name);
			printf(h->header.kids[i]->string.str);
			printf("</%s>\n", h->header.kids[i]->string.secret?"secret":"string");
			break;
		case ET_INTEGER:
			printf("%s  <integer name=\"%s\">",s, h->header.kids[i]->name);
			printf("%d",h->header.kids[i]->integer.val);
			printf("</integer>\n");
			break;
		case ET_SUBDIR:
			dump_header(h->header.kids[i]->subdir.header,d+1);
			break;
		default:
			fprintf(stderr,"I AM NOT AMUSED!\n");
		}
	printf("%s</dir>\n",s);
}

void dump_xml(void)
{
	ent *i;
	printf("<?xml version=\"1.0\"?>\n");
	printf("<?xml-stylesheet type=\"text/xml\" href=\"pspreghtmlizer.xsl\"?>\n");
	printf("<registry format=\"psp\">\n");
	for(i=hdrlist;i;i=i->header.nextheader)
		if(!i->header.parent)
			dump_header(i,1);
	printf("</registry>\n");
}

int main(void)
{
	FILE *f=fopen("system.dreg","r");
	fread(d,131072,1,f);
	fclose(f);
	f=fopen("system.ireg","r");
	fseek(f,0x5C,SEEK_SET);
	fread(a,256*58,1,f);
	fclose(f);
	walk_fatents();
	resolve_refs();
	dump_xml();
	return 0;
}
