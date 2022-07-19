/*
    Fake sceKernelDevkitVersion() to return 0x06060010
    Davee

    Twitter: @DaveeFTW
    web: https://lolhax.org
*/

#include <pspsdk.h>
#include <pspkernel.h>

PSP_MODULE_INFO("version_spoof", 0x1007, 1, 0);

int module_start(SceSize args, void *argp)
{
    SceModule2 *mod = sceKernelFindModuleByName("sceSystemMemoryManager");

    if (mod)
    {
        _sw(0x34620010, mod->text_addr + 0x00011924); // 6.61
        sceKernelIcacheInvalidateAll();
        sceKernelDcacheWritebackInvalidateAll();
    }

    return 0;
}
