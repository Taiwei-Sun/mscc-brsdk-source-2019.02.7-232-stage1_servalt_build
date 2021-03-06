/* Copyright (c) 2015 Microsemi Corporation

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

/*
 *
 * VCore-III platform ressources
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/serial_8250.h>
#include <linux/uio_driver.h>
#include <linux/i2c.h>
#include <linux/i2c/vcoreiii.h>

#include <asm/mach-jaguar2/hardware.h>
#include <asm/vcoreiii-gpio.h>

/* Network */
static struct platform_device vc3fdma_device = {
	.name			= "vc3fdma",
	.id			= -1,
};

/* 8250 */
static struct plat_serial8250_port uart8250_vcoreiii_data[] = {
	{
		.mapbase	= 0x70100000,
		.irq		= UART_IRQ,
		.uartclk	= VCOREIII_AHB_CLOCK,
		.iotype		= UPIO_MEM32,
		.flags		= (UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_IOREMAP),
		.regshift	= 2,
	},
#if defined(CONFIG_SERIAL_8250_RUNTIME_UARTS) && (CONFIG_SERIAL_8250_RUNTIME_UARTS > 1)
	{
		.mapbase	= 0x70100800,
		.irq		= UART2_IRQ,
		.uartclk	= VCOREIII_AHB_CLOCK,
		.iotype		= UPIO_MEM32,
		.flags		= (UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_IOREMAP),
		.regshift	= 2,
	},
#endif
	{ },
};

static struct platform_device uart8250_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = uart8250_vcoreiii_data,
	},
};

/* Switch UIO */
static struct uio_info switch_platform_data = {
    .name = "vcoreiii_switch",
    .version = "0",
    .irq = INT1_IRQ,    // Alternate IRQ. Application must remap IRQ destination
};

static struct resource switch_resources[] = {
    [0] = {
        .name   = "origin1_2",
        .start  = VTSS_IO_ORIGIN1_OFFSET,
        .end    = VTSS_IO_ORIGIN2_OFFSET + VTSS_IO_ORIGIN2_SIZE - 1,
        .flags  = IORESOURCE_MEM,
    },
};

static struct platform_device switch_device = {
    .name           = "uio_pdrv_genirq",
    .id             = -1,
    .dev = {
        .platform_data  = &switch_platform_data,
    },
    .resource       = switch_resources,
    .num_resources  = ARRAY_SIZE(switch_resources),
};

/* Firmware memmap UIO */
static struct uio_info firmware_platform_data = {
    .name = "vcoreiii_firmware",
    .version = "0",
};

static struct resource firmware_resources[] = {
    [0] = {
        .name   = "vcoreiii_firmware",
        .flags  = IORESOURCE_MEM,
    },
};

static struct platform_device firmware_device = {
    .name           = "vcfw_uio",
    .id             = -1,
    .dev = {
        .platform_data  = &firmware_platform_data,
    },
    .resource       = firmware_resources,
    .num_resources  = ARRAY_SIZE(firmware_resources),
};

/* I2C */
static struct vcoreiii_i2c_platform_data i2c_data = {
    .fast_mode = 0,             /* 100 kbit/s */
};

static struct platform_device i2c_device = {
    .name			= "i2c_vcoreiii",
    .id			= -1,
    .dev = {
        .platform_data = &i2c_data,
    },
};

static struct platform_device *target_devices[] __initdata = {
    &uart8250_device,
    &switch_device,
    &vc3fdma_device,
    &i2c_device,
};

static struct i2c_board_info i2c_devs[] __initdata = {
    { I2C_BOARD_INFO("sfp", 0x50),  }, /* I2C SFP */
};

static int switch_core_reset(void)
{
	int timeout;
	u32 val;

	pr_notice("Resetting Switch Core...\n");

	/* Make sure the core is PROTECTED from reset */
	val = readl(VTSS_ICPU_CFG_CPU_SYSTEM_CTRL_RESET);
	val |= VTSS_M_ICPU_CFG_CPU_SYSTEM_CTRL_RESET_CORE_RST_PROTECT;
	writel(val, VTSS_ICPU_CFG_CPU_SYSTEM_CTRL_RESET);

	/* To verify reset */
	writel(0xdeafbeef, VTSS_DEVCPU_GCB_CHIP_REGS_GPR);

	/* Now reset */
	writel(VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST,
	       VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST);
	for(timeout = 0; timeout < 100; timeout++) {
		if ((readl(VTSS_DEVCPU_GCB_CHIP_REGS_SOFT_RST) &
		     VTSS_M_DEVCPU_GCB_CHIP_REGS_SOFT_RST_SOFT_SWC_RST) == 0) {
			if (readl(VTSS_DEVCPU_GCB_CHIP_REGS_GPR) != 0) {
				pr_err("Switch Core Reset failure detected!\n");
			} else {
				pr_notice("Switch Core Reset complete and verified.\n");
			}
			return 0;
		}
		udelay(1);
	}

	pr_warning("Switch Core Reset timeout!\n");
	return -ETIMEDOUT;
}


static int __init target_device_init(void)
{
    int res;
#if defined(CONFIG_SERIAL_8250_RUNTIME_UARTS) && (CONFIG_SERIAL_8250_RUNTIME_UARTS > 1)
    // Bootloader *may* not have set this up, be sure...
#if defined(CONFIG_VTSS_VCOREIII_JAGUAR2_ARCH)
    vcoreiii_gpio_set_alternate(24, 1);
    vcoreiii_gpio_set_alternate(25, 1);
#elif defined(CONFIG_VTSS_VCOREIII_SERVALT)
    vcoreiii_gpio_set_alternate(20, 1);
    vcoreiii_gpio_set_alternate(21, 1);
#else
#error Unknown platform
#endif
#endif

    /* Do Switch Reset */
    switch_core_reset();

    i2c_register_board_info(0, i2c_devs, ARRAY_SIZE(i2c_devs));

    res = platform_add_devices(target_devices, ARRAY_SIZE(target_devices));
    if (res == 0 && vcoreiii_memmap_start && vcoreiii_memmap_size) {
        firmware_resources[0].start = vcoreiii_memmap_start;
        firmware_resources[0].end = vcoreiii_memmap_start + vcoreiii_memmap_size - 1;
        res = platform_device_register(&firmware_device);
    }
    return res;
}
module_init(target_device_init);

MODULE_AUTHOR("Lars Povlsen <lpovlsen@vitesse.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Platform drivers for Jaguar2 platform");
