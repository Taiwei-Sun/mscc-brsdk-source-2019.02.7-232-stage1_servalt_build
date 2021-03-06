// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2018 Microsemi Corporation
 */
#include <common.h>
#include <bootm.h>
#include <command.h>
#include <image.h>
#include <lmb.h>
#include <mapmem.h>
#include <linux/kernel.h>
#include <linux/sizes.h>
#include <linux/mtd/mtd.h>
#include <jffs2/load_kernel.h>
#include <spi_flash.h>
#include <asm/io.h>
#include <environment.h>
#include "../common/firmware_vimage.h"
#include <malloc.h>


static struct spi_flash *spi_flash;

static void dump_tlv(mscc_firmware_vimage_tlv_t t, u8 *data)
{
	debug("Tlv type %d, length %d, data %p\n", t.type, t.data_len, data);
}

static void update_bootargs(const char *data) {
	char buffer[CONFIG_SYS_CBSIZE];
	char *bootargs = env_get("bootargs");

	memset(buffer, 0x0, CONFIG_SYS_CBSIZE);
	snprintf(buffer, CONFIG_SYS_CBSIZE, "%s %s", bootargs, data);
	env_set("bootargs", buffer);
}

static int bootmfi_parse(ulong ld, bootm_headers_t *image)
{
	int ret;
	u8 *tlvdata;
	const char *err_msg;
	mscc_firmware_vimage_tlv_t tlv;
	mscc_firmware_vimage_t *fw = (mscc_firmware_vimage_t *) ld;

	ret = mscc_vimage_hdrcheck(fw, &err_msg);

	if (ret) {
		printf("Invalid image: %s\n", err_msg);
		return ret;
	}

	tlvdata = mscc_vimage_find_tlv(fw, &tlv, MSCC_STAGE1_TLV_SIGNATURE);
	if (tlvdata) {
		dump_tlv(tlv, tlvdata);
		if (!mscc_vimage_validate(fw, &tlv, tlvdata))
			return 1;
	} else {
		printf("Invalid image: Signature missing\n");
		return 1;
	}

#if DEBUG
	tlvdata = mscc_vimage_find_tlv(fw, &tlv, MSCC_STAGE1_TLV_METADATA);
	dump_tlv(tlv, tlvdata);
	if (tlvdata) {
		char *buf;

		buf = malloc(tlv.data_len);
		snprintf(buf, tlv.data_len, "Image metadata: %s\n",
			  (char *)tlvdata);
		printf("%s", buf);
		free(buf);
	}
#endif
	tlvdata = mscc_vimage_find_tlv(fw, &tlv, MSCC_STAGE1_TLV_KERNEL);
	if (tlvdata) {
		dump_tlv(tlv, tlvdata);
		debug("Found kernel len=%d\n", tlv.data_len);
		image->os.comp = IH_COMP_XZ;
		image->os.type = IH_TYPE_KERNEL;
		image->os.load = 0x80100000;
		image->os.image_start = (ulong)tlvdata;
		image->os.image_len = tlv.data_len;
	}

	tlvdata = mscc_vimage_find_tlv(fw, &tlv, MSCC_STAGE1_TLV_INITRD);
	if (tlvdata)
	{
		dump_tlv(tlv, tlvdata);
		image->rd_start = (ulong) tlvdata;
		image->rd_end = (ulong) tlvdata + tlv.data_len;
		debug("Found initrd len=%d rd_start =%lX, end=%lX\n",
		      tlv.data_len, image->rd_start, image->rd_end);
	}

	tlvdata = mscc_vimage_find_tlv(fw, &tlv, MSCC_STAGE1_TLV_KERNEL_CMD);
		if (tlvdata)
	{
		dump_tlv(tlv, tlvdata);
		update_bootargs((const char*) tlvdata);
		debug("Found command line = %s \n", tlvdata);
	}

	return 0;
}

/*
 * Mfi linux kernels expects the ebase register to be at the start of
 * the RAM. But uboot relocates the ebase to the top of the memory which
 * cause issues in the linux kernel. Therefore copy the interrupt vectors
 * from the current position to the begining of the RAM and then change
 * the ebase register.
 */
static void relocate_ebase(void)
{
	// 0x500 - represents the total size of the interrupt vector,
	// this value is taken from arch/mips/lib/stack.c
	memcpy((void*)CONFIG_SYS_SDRAM_BASE, (void*)gd->irq_sp, 0x500);
	flush_cache(CONFIG_SYS_SDRAM_BASE, 0x500);
	write_c0_ebase(CONFIG_SYS_SDRAM_BASE);
	gd->irq_sp = CONFIG_SYS_SDRAM_BASE;
}

/*
 * Image booting support
 */
static int bootmfi_start(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[], bootm_headers_t *images)
{
	int ret = 0;
	ulong ld;

	ret = do_bootm_states(cmdtp, flag, argc, argv, BOOTM_STATE_START,
			      images, 1);
	if (ret) {
		printf("do_bootm_states failed\n");
		return ret;
	}

	/* Setup Linux kernel Image entry point */
	if (!argc) {
		ld = load_addr;
		debug("*  kernel: default image load address = 0x%08lx\n",
				load_addr);
	} else {
		ld = simple_strtoul(argv[0], NULL, 16);
		debug("*  kernel: cmdline image address = 0x%08lx\n", ld);
	}
	images->os.os = IH_OS_LINUX;
	images->os.arch = IH_ARCH_MIPS;

	if (bootmfi_parse(ld, images))
		return 1;
	images->ep = images->os.load;

	env_set("boot_mfi", "1");

	return ret;
}

static int flash_setup(void)
{
	static struct udevice *new;
	spi_flash_probe_bus_cs(CONFIG_SF_DEFAULT_BUS, CONFIG_SF_DEFAULT_CS,
			       0, 0, &new);
	spi_flash = dev_get_uclass_priv(new);
	if (spi_flash == NULL) {
	    printf("Spi driver not created\n");
	    return 1;
	}
	 return 0;
}

static int partition_offset(const char *part_name, u32 *offset)
{
	struct mtd_device *mtd_dev = NULL;
	u8 pnum = 0;
	struct part_info *part = NULL;

	mtdparts_init();
	if (find_dev_and_part(part_name, &mtd_dev, &pnum, &part)) {
	    printf("Partition not found: %s\n", part_name);
	    return 1;
	}

	*offset = part->offset;
	return 0;
}

static int bootmfi_nor_start(cmd_tbl_t *cmdtp, int flag, int argc,
			     char * const argv[], bootm_headers_t *images)
{
	int ret = 0;
	mscc_firmware_vimage_t fw;
	u32 offset = 0;
	ulong image_size = 0;
	ulong ld = 0;
	const char *err_msg = NULL;
	unsigned char *data = NULL;

	memset(&fw, 0, sizeof(mscc_firmware_vimage_t));

	ret = do_bootm_states(cmdtp, flag, argc, argv, BOOTM_STATE_START,
			      images, 1);
	if (ret) {
		err_msg = "do_bootm_states failed\n";
		goto error;
	}

	if (flash_setup()) {
		err_msg = "Flash initialization failed\n";
		goto error;
	}

	if (partition_offset(argv[0], &offset)) {
		err_msg = "Partition was not found\n";
		goto error;
	}

	spi_flash_read(spi_flash, offset, sizeof(fw),
	              (unsigned char*)&fw);
	if (mscc_vimage_hdrcheck(&fw, &err_msg)) {
		goto error;
	}

	if (argc == 1) {
		ld = load_addr;
	} else {
		ld = simple_strtoul(argv[1], NULL, 16);
	}

	data = (unsigned char*)map_physmem(ld, fw.imglen, MAP_WRBACK);
	if (data == NULL) {
		err_msg = "Memory allocation failed";
		goto error;
	}
	spi_flash_read(spi_flash, offset, fw.imglen, data);

	images->os.os = IH_OS_LINUX;
	images->os.arch = IH_ARCH_MIPS;

	if (bootmfi_parse((unsigned long)data, images))
		return 1;
	images->ep = images->os.load;
	lmb_reserve(&images->lmb, images->ep, le32_to_cpu(image_size));

	env_set("fis_act", argv[0]);
	env_set("boot_mfi", "1");

	spi_flash_free(spi_flash);
	spi_flash = NULL;

	return ret;
error:
	if (spi_flash)
		spi_flash_free(spi_flash);
	spi_flash = NULL;

	printf("Boot %s failed\n", argv[0]);
	printf("Error: %s\n", err_msg);
	return -1;
}

static int bootmem_size(void)
{
	return CONFIG_SYS_SDRAM_SIZE / 0x1000 / 8;
}

static int ramload_start(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[], bootm_headers_t *images)
{
	struct lmb_region *rgn = &(images->lmb.reserved);
	ulong mmap_size = 0;
	phys_size_t uboot_size = 0;
	int ret = 0;
	char buf[16];

	memset(buf, '\0', 16);

	mmap_size = env_get_ulong("mmap_size", 10, 0);
	if (mmap_size == 0) {
		printf("Please set 'mmap_size' in uboot before using this command\n");
		printf("Ex: setenv mmap_size 'X'; saveenv;\n");
		printf("X - number of MB in decimal\n");
		return 1;
	}
	mmap_size = mmap_size * 1024 * 1024;
	sprintf(buf, "0x%lx",
		CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_SDRAM_SIZE - mmap_size);

	char *dhcp_args[] = {"dhcp", buf, argv[0]};
	cmd_tbl_t *dhcp_cmd = find_cmd("dhcp");
	ret = dhcp_cmd->cmd(dhcp_cmd, flag, 3, dhcp_args);
	if (ret)
		return 1;

	// consume also the filename;
	argc--; argv++;
	ret = bootmfi_start(cmdtp, flag, argc, argv, images);
	if (ret)
		return 1;

	uboot_size = rgn->region[0].size;

	env_set("fis_act", "");
	lmb_alloc(&images->lmb, mmap_size - uboot_size, 0x1000);
	// This space is needed because the kernel allocates between the end of
	// initrd and end of emory the boot memory allocator. If the space is
	// not "reserved" in UBoot, the kernel doesn't have space to allocate
	// boot memomory allocator because UBoot puts the initrd at the highest
	// not reserved memory.
	lmb_alloc(&images->lmb, bootmem_size(), 0x1000);
	return ret;
}

int do_bootmfi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret;

	/* Consume 'bootmfi' */
	argc--; argv++;

	if (bootmfi_start(cmdtp, flag, argc, argv, &images))
		return 1;

	env_set("fis_act", "linux");
	/*
	 * We are doing the BOOTM_STATE_LOADOS state ourselves, so must
	 * disable interrupts ourselves
	 */
	bootm_disable_interrupts();
	relocate_ebase();

	ret = do_bootm_states(cmdtp, flag, argc, argv,
			      BOOTM_STATE_LOADOS |
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
			      BOOTM_STATE_RAMDISK |
#endif
			      BOOTM_STATE_OS_PREP | BOOTM_STATE_OS_FAKE_GO |
			      BOOTM_STATE_OS_GO,
			      &images, 1);

	return ret;
}

int do_bootmfi_nor(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret;

	/* Consume 'bootmfi_nor' */
	argc--; argv++;
	if (argc < 1) {
	    printf("Wrong usage: partition missing\n");
	    return 1;
	}

	if ((gd->flags & GD_FLG_ENV_DEFAULT) == GD_FLG_ENV_DEFAULT)
		env_save();

	if (bootmfi_nor_start(cmdtp, flag, argc, argv, &images))
		return 1;

	/*
	 * We are doing the BOOTM_STATE_LOADOS_ state ourselves, so must
	 * disable interrupts ourselves
	 */
	bootm_disable_interrupts();
	relocate_ebase();

	ret = do_bootm_states(cmdtp, flag, argc, argv,
			      BOOTM_STATE_LOADOS |
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
			      BOOTM_STATE_RAMDISK |
#endif
			      BOOTM_STATE_OS_PREP | BOOTM_STATE_OS_FAKE_GO |
			      BOOTM_STATE_OS_GO,
			      &images, 1);
	return ret;
}

int do_ramload(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret;

	/* Consume 'ramload' */
	argc--; argv++;
	if (argc < 1) {
	    printf("Wrong usage: filename missing\n");
	    return 1;
	}

	if (ramload_start(cmdtp, flag, argc, argv, &images))
		return 1;

	/*
	* We are doing the BOOTM_STATE_LOADOS_ state ourselves, so must
	* disable interrupts ourselves
	*/
	bootm_disable_interrupts();
	relocate_ebase();

	ret = do_bootm_states(cmdtp, flag, argc, argv,
			     BOOTM_STATE_LOADOS |
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
			     BOOTM_STATE_RAMDISK |
#endif
			     BOOTM_STATE_OS_PREP | BOOTM_STATE_OS_FAKE_GO |
			     BOOTM_STATE_OS_GO,
			     &images, 1);
	return ret;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootmfi_help_text[] =
	"[addr]\n"
	"    - boot MFI Linux Image stored in memory\n"
	" if a command line is present in the MFI it would be appended to\n"
	" bootargs env";
#endif

U_BOOT_CMD(
	bootmfi,	2,	1,	do_bootmfi,
	"boot MFI image from memory", bootmfi_help_text
);

#ifdef CONFIG_SYS_LONGHELP
static char bootmfi_nor_help_text[] =
	"partition\n"
	"    - boot MFI Linux Image stored in NOR Flash\n"
	" if a command line is present in the MFI it would be appended to\n"
	" bootargs env";
#endif

U_BOOT_CMD(
	bootmfi_nor,	3,	1,	do_bootmfi_nor,
	"boot MFI image from nor", bootmfi_nor_help_text
);

#ifdef CONFIG_SYS_LONGHELP
static char ramload_help_text[] =
	"file\n"
	"    - download MFI Linux image and boot it from memory\n"
	" if a command line is present in the MFI it would be appended to\n"
	" bootargs env";
#endif

U_BOOT_CMD(
	ramload,	2,	1,	do_ramload,
	"download and boot MFI image from memory", ramload_help_text
);
