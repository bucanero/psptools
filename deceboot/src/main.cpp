// Copyright (c) 2016- NoOneee.
// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

#include <stdio.h>

#include "cooltypedefs.h"

#include "Core/ELF/PrxDecrypter.h"

long getFileSize(FILE *file)
{
	long curPos, endPos;
	curPos = ftell(file);
	fseek(file, 0, 2);
	endPos = ftell(file);
	fseek(file, curPos, 0);
	return endPos;
}

int main(int argc, char *argv[])	
{
	char *filePathIn = NULL;
	char *filePathOut = NULL;
	printf("DecEboot v0.3 by NoOneee\n");
	printf("A small utility to decrypt PSP executable EBOOTs to ELF files.\n");
	printf("Based on PPSSPP v1.2.2. Thank you all!\n");
	printf("\n");
	switch (argc){
		case 1:
			printf("Usage: deceboot input_file [output_file]\n");
			printf("\n");
			printf("input_file is the encrypted EBOOT you want to decrypt.\n");
			printf("output_file is the filename for the decrypted EBOOT. (optional, default is EBOOT_DEC.BIN)\n");
			printf("\n");
			printf("Example:\n");
			printf("deceboot EBOOT.BIN         \tDecrypts EBOOT.BIN to EBOOT_DEC.bin\n");
			printf("deceboot EBOOT.BIN BOOT.BIN\tDecrypts EBOOT.BIN to BOOT.bin\n");
			return 0;
		case 2:
			filePathIn = argv[1];
			filePathOut = (char*)"EBOOT_DEC.BIN";
			break;
		case 3:
			filePathIn = argv[1];
			filePathOut = argv[2];
			break;
	}
		
	u8 *in;
	FILE *file = NULL;
	if ((file = fopen(filePathIn, "rb")) == NULL){
		printf("Could not open input file\n");
		return 0;
	}
	else
		printf("Input file opened successfully\n");
	
	long fileSizeIn = getFileSize(file);

	in = new u8[fileSizeIn];
	fread(in, fileSizeIn, 1, file);
	fclose(file);
	
	u32_le *magicPtr = (u32_le *) in;
	u8 *encPrt = in;
	if(*magicPtr == 0x50425000){ //if file starts with \x00PBP
		printf("Input file is PBP.\n");
		unsigned int offset = *(u32_le*)(in+0x20);
		encPrt += offset;
		printf("Executable is at 0x%x\n",offset);
		magicPtr = (u32_le *) encPrt;
	}
	if(*magicPtr == 0x464C457F){ //if data starts with \x7FELF
		printf("Data is already decrypted!\n");
		FILE *decryptedEbootFile = fopen(filePathOut, "wb");
		fwrite(encPrt, sizeof(u8), fileSizeIn - (unsigned int)encPrt + 
			(unsigned int)in, decryptedEbootFile);
		fclose(decryptedEbootFile);
		printf("\nSaved to %s\n",filePathOut);
		delete[] in;
		return 0;
	}
	if(*magicPtr != 0x5053507e){ //if data doesn't start with ~PSP
		printf("Input file is not an encrypted executable.\nExiting...\n");
		delete[] in;
		return 0;		
	}
	
	printf("Decrypting executable file...\n");
	
	
	PSP_Header *head = (PSP_Header*)encPrt;
	
	u32 size = head->elf_size;
	if (head->psp_size > size)
	{
		size = head->psp_size;
	}
	u8 *ptr = new u8[head->elf_size + head->psp_size];
	
	int ret = pspDecryptPRX(encPrt, (u8*)ptr, head->psp_size);
	if(ret < 0){
		printf("\nError! Decrypt function returned %d\n",ret);
	} else{
		printf("\nSuccess! Decrypted EBOOT size: %d bytes\n",ret);
		FILE *decryptedEbootFile = fopen(filePathOut, "wb");
		fwrite(ptr, sizeof(u8), ret, decryptedEbootFile);
		fclose(decryptedEbootFile);
		printf("Saved to %s\n",filePathOut);
	}
	delete[] ptr;
	delete[] in;
    return 0;
}
