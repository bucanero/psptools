DecEboot v0.3 by NoOneee
A small utility to decrypt PSP executable EBOOTs to ELF files.
Based on PPSSPP v1.2.2. Thank you all!


Usage: deceboot input_file [output_file]

input_file is the encrypted EBOOT you want to decrypt.
output_file is the filename for the decrypted EBOOT. (optional, default is EBOOT_DEC.BIN)

Example:
deceboot EBOOT.BIN         		Decrypts EBOOT.BIN to EBOOT_DEC.bin
deceboot EBOOT.BIN BOOT.BIN		Decrypts EBOOT.BIN to BOOT.bin


CHANGELOG

v0.3
-Added decryption support for ELF contained in .PBP files (experimental)
-If input is already decrypted, the executable is simply copied to the output file

v0.2
-Fixed compilation on Linux

v0.1
-Initial Release