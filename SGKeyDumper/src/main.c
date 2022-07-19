/*
 *  SGKeyDumper
 *
 *  Copyright (C) 2014 qwikrazor87
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <pspkernel.h>
#include <pspsysmem_kernel.h>
#include <psputilsforkernel.h>
#include <string.h>
#include "lib.h"
#include "sctrl.h"

PSP_MODULE_INFO("SGKeyDumper", PSP_MODULE_KERNEL, 1, 7);
PSP_HEAP_SIZE_KB(0);

static STMOD_HANDLER previous = NULL;

#define PSP_GO 4

u32 _sceUtilitySavedataInitStart, _sub;
char SavePath[128];

void qwikDump(u32 a0, u32 a1)
{
	u32 k1 = pspSdkSetK1(0);

	memset((void *)SavePath, 0, sizeof(SavePath));

	strcpy(SavePath, "ms0:/PSP/GAME/SED/gamekey/");

	strcat(SavePath, (char *)(a1 + 0x3C)); //game ID
	strcat(SavePath, ".bin");

	SceUID fd = sceIoOpen(SavePath, PSP_O_CREAT | PSP_O_WRONLY | PSP_O_TRUNC, 0777);

	if (fd < 0) {
			//change device to ef0:/ for PSP Go
			SavePath[0] = 'e';
			SavePath[1] = 'f';

			fd = sceIoOpen(SavePath, PSP_O_CREAT | PSP_O_WRONLY | PSP_O_TRUNC, 0777);
		}
	
	if (fd > 0) {
		sceIoWrite(fd, (const void *)(a1 + ((*(u32*)a1) - 0x24)), 16); //end of savedata struct - 36

		sceIoClose(fd);
	}

	pspSdkSetK1(k1);

	void (* _sub_)(u32, u32) = (void *)_sub;
	_sub_(a0, a1);
}

u32 nextmod = 0;

int module_start_handler(SceModule2 * module)
{
	int ret = previous ? previous(module) : 0;

	//signal that the main game module has loaded.
	if (nextmod == 1)
		nextmod++;

	//signal that sceKernelLibrary has loaded, the next module should be the main game module.
	if (!strcmp(module->modname, "sceKernelLibrary"))
		nextmod++;

	return ret;
}

int thread_start(SceSize args, void *argp)
{
	previous = sctrlHENSetStartModuleHandler(module_start_handler);

	//wait until the main game module starts
	while (nextmod != 2)
		sceKernelDelayThread(1000);
	
	// Check to see if the model is a PSP GO
	if(kuKernelGetModel() == PSP_GO)
		_sceUtilitySavedataInitStart = sctrlHENFindFunction("sceUtility_Driver", "sceUtility", 0x50C4CD57);	
	else
		_sceUtilitySavedataInitStart = FindExport("sceUtility_Driver", "sceUtility", 0x50C4CD57);
	
	if (_sceUtilitySavedataInitStart) {
		//get the sub address called by sceUtilitySavedataInitStart
		_sub = ((*(u32*)(_sceUtilitySavedataInitStart + 0x18) & 0x03FFFFFF) << 2) | 0x80000000;

		_sw(MAKE_CALL(qwikDump), _sceUtilitySavedataInitStart + 0x18);

		ClearCaches();
	}

	return 0;
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("SGKeyDumper", thread_start, 0x22, 0x2000, 0, NULL);

	if (thid >= 0)
		sceKernelStartThread(thid, args, argp);

	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}
