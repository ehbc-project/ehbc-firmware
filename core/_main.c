#include "debug.h"
#include "driver.h"
#include "resource.h"
#include "settings.h"

#include "elf.h"
#include "fs.h"
#include "video.h"
#include "keyboard.h"
#include "timer.h"

void main(void)
{
    /* Debugger device is already initialized if the system has one */
    debug_log("Core started");

    /* probe and initialize each drivers and devices */
    struct driver *driver = driver_list();
    int result;

    for (; driver != NULL; driver = driver_list_next(driver)) {
        debug_log("Driver '%s':", driver->name);
        
        result = driver_probe(driver);

        if (result) {
            debug_log("\tProbe error: %d", result);
            continue;
        }
        
        result = driver_init(driver);

        if (result) {
            debug_log("\tInitialization error: %d", result);
            continue;
        }

        struct device *device = driver->devices;
        /* initialize devices */
        for (; device != NULL; device = device->next) {
            result = device_init(device);
            if (result) {
                debug_log("\t\tInitialization error: %d", result);
                continue;
            }

            debug_log("\tDevice '%s':", device->name);
            debug_log("\t\tID: %d", device->id);
            debug_log("\t\tType: %s", device_type_str(device->type));
        }
    }

    /* initialize services */
    video_init();
    keyboard_init();
    timer_init();

    /* find bootable volume */
    struct fs_volume *volume = fs_volume_list(storage);
    for (; volume != NULL; volume = fs_volume_list_next(volume)) {
        struct fs *fs = fs_mount(volume, FS_READONLY);

        struct fs_fileinfo file;
        result = fs_find_file(&file, fs, "boot/m68k.elf");

        if (!result && (file->type == FT_FILE)) {
            debug_log("Bootable: '%s/boot/m68k.elf'", fs->path);
        }
        
        fs_unmount(fs);
    }

    /* continue or configure */
    struct timer *timer = timer_alloc();

    timer_wait(1000);

    timer_free(timer);

    /* load boot file */
    volume = fs_volume_find("ata(0,0)mbr(0)");
    struct fs *fs = fs_mount(volume, FS_READONLY);

    struct elf *elf = elf_open("boot/m68k.elf");

    if (elf->header.e_type != ET_EXEC) {
        debug_log("Invalid ELF object type");
    }

    if (elf->header.e_machine != 0x04) {
        debug_log("Invalid ISA type");
    }

    elf_load_image(elf);
    void (*entrypoint)(void) = elf->header.e_entry;

    elf_close(elf);
    fs_unmount(fs);

    /* execute boot file */
    entrypoint();
}
