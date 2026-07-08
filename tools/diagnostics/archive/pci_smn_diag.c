/* PCI config dump + SMN scan for SPI-related FCH registers.
 * Compares 256-byte PCI config of LPC bridge (1022:790E, 00:14.3)
 * with Windows dump, and scans nearby SMN addresses for differences.
 *
 * insmod pci_smn_diag.ko && dmesg | grep "psd:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <asm/amd/node.h>  /* amd_smn_read */

#define FCH_BASE 0xFEC10000
static void __iomem *base;

static int __init psd_init(void)
{
	struct pci_dev *lpc;
	u32 val;
	u8 buf[256];
	int i, rc;

	pr_info("psd: ============================================\n");
	pr_info("psd: PCI CONFIG SPACE DUMP — LPC Bridge 1022:790E\n");
	pr_info("psd: ============================================\n");

	/* ===== PCI CONFIG DUMP ===== */
	lpc = pci_get_device(PCI_VENDOR_ID_AMD, 0x790E, NULL);
	if (!lpc) {
		pr_err("psd: LPC bridge not found\n");
		return -ENODEV;
	}
	pr_info("psd: LPC bridge: %04x:%04x at %s\n",
		lpc->vendor, lpc->device, pci_name(lpc));

	/* Dump 256 bytes of config space, 16 bytes per line */
	for (i = 0; i < 256; i += 16) {
		int j;
		for (j = 0; j < 16; j += 4)
			pci_read_config_dword(lpc, i + j, (u32 *)&buf[i + j]);
		pr_info("psd: PCI[%02x-%02x] = %*ph\n", i, i+15, 16, buf + i);
	}

	/* Highlight known registers */
	pr_info("psd: --- KEY REGISTERS ---\n");
	pci_read_config_dword(lpc, 0x50, &val);
	pr_info("psd:  0x50 (RomProtect)  = 0x%08x (Win=0x????0505)\n", val);
	pci_read_config_dword(lpc, 0xB4, &val);
	pr_info("psd:  0xB4 (FIFO_LAYOUT) = 0x%08x (Win=0x7DFFE000)\n", val);
	pci_read_config_dword(lpc, 0xB8, &val);
	pr_info("psd:  0xB8 (FIFO_ACCESS) = 0x%08x (Win=0x33ED0084)\n", val);
	pci_read_config_dword(lpc, 0xBC, &val);
	pr_info("psd:  0xBC               = 0x%08x\n", val);
	pci_read_config_dword(lpc, 0xC0, &val);
	pr_info("psd:  0xC0               = 0x%08x\n", val);
	pci_read_config_dword(lpc, 0xC4, &val);
	pr_info("psd:  0xC4               = 0x%08x\n", val);

	/* Extended cap space (0x100+) */
	pci_read_config_dword(lpc, 0x100, &val);
	pr_info("psd:  0x100              = 0x%08x\n", val);
	pci_read_config_dword(lpc, 0x104, &val);
	pr_info("psd:  0x104              = 0x%08x\n", val);

	pci_dev_put(lpc);

	/* ===== MMIO DUMP ===== */
	pr_info("psd:\n");
	pr_info("psd: ============================================\n");
	pr_info("psd: MMIO REGISTER DUMP — SPI Controller @ 0xFEC10000\n");
	pr_info("psd: ============================================\n");

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	pr_info("psd: CTRL0     (0x00) = 0x%08x\n", ioread32(base + 0x00));
	pr_info("psd: REST_CMD  (0x04) = 0x%08x (Win=0x00000006)\n", ioread32(base + 0x04));
	pr_info("psd: REST_CMD2 (0x08) = 0x%08x (Win=0x00000000)\n", ioread32(base + 0x08));
	pr_info("psd: CTRL1     (0x0C) = 0x%08x (Win=0x020006B5)\n", ioread32(base + 0x0C));
	pr_info("psd: ALT_CS    (0x1D) = 0x%02x   (Win=0xB1)\n", ioread8(base + 0x1D));
	pr_info("psd: ENA       (0x20) = 0x%08x (Win=0x11110713)\n", ioread32(base + 0x20));
	pr_info("psd: SPD_CFG   (0x22) = 0x%04x   (Win=0x1111)\n", ioread16(base + 0x22));
	pr_info("psd: HOST_PREF (0x2C) = 0x%08x (Win=0x8000D4C0)\n", ioread32(base + 0x2C));
	pr_info("psd: SPEED_CFG (0x44) = 0x%04x   (Win=0x0200)\n", ioread16(base + 0x44));
	pr_info("psd: OPCODE    (0x45) = 0x%02x\n", ioread8(base + 0x45));
	pr_info("psd: TRIGGER   (0x47) = 0x%02x\n", ioread8(base + 0x47));
	pr_info("psd: TX_COUNT  (0x48) = 0x%02x\n", ioread8(base + 0x48));
	pr_info("psd: RX_COUNT  (0x4B) = 0x%02x\n", ioread8(base + 0x4B));
	pr_info("psd: STATUS    (0x4C) = 0x%08x (Win=0x000F0E0D)\n", ioread32(base + 0x4C));
	pr_info("psd: SPD7      (0x6C) = 0x%08x (Win=0x00000000)\n", ioread32(base + 0x6C));
	pr_info("psd: MISC_CTRL (0xFC) = 0x%04x   (Win=0x0000)\n", ioread16(base + 0xFC));

	/* ===== SMN SCAN ===== */
	pr_info("psd:\n");
	pr_info("psd: ============================================\n");
	pr_info("psd: SMN SCAN — FCH SPI-related addresses\n");
	pr_info("psd: ============================================\n");

	/* SMN address format: 0xNNFFFFCC where NN=node(0), FFFF=signature, CC=offset
	 * The SSDT shows addresses like 0x03B10528 — this is node 0, instance 3 (FCH),
	 * offset 0xB10528. Let's scan the FCH region. */

	/* Known SMN addresses from SSDT (ACP audio) — verify access works */
	{
		static const u32 test_addrs[] = {
			0x03B10528, 0x03B10564, 0x03B10998, 0x03B1099C,
			0x03B109A0, 0x03B109A4, 0x03B109A8, 0x03B109AC,
		};
		pr_info("psd: --- Known ACP SMN registers (verification) ---\n");
		for (i = 0; i < ARRAY_SIZE(test_addrs); i++) {
			rc = amd_smn_read(0, test_addrs[i], &val);
			if (rc == 0)
				pr_info("psd: SMN[0x%08X] = 0x%08X\n", test_addrs[i], val);
			else
				pr_info("psd: SMN[0x%08X] = ERROR %d\n", test_addrs[i], rc);
		}
	}

	/* Scan FCH PCIe/SPI region: SMN offsets 0xB10xxx - 0xB1xxxx
	 * These should correspond to configuration registers. */
	pr_info("psd:\n");
	pr_info("psd: --- SMN scan: 0x03B10xxx (potential FCH config space) ---\n");
	for (i = 0x00; i <= 0xFC; i += 4) {
		u32 addr = 0x03B10000 + i;
		rc = amd_smn_read(0, addr, &val);
		if (rc == 0 && val != 0 && val != 0xFFFFFFFF)
			pr_info("psd: SMN[0x%08X] = 0x%08X\n", addr, val);
	}

	/* Try lower FCH region: 0x03B00000 */
	pr_info("psd:\n");
	pr_info("psd: --- SMN scan: 0x03B000xx (lower FCH) ---\n");
	for (i = 0x00; i <= 0xFF; i += 4) {
		u32 addr = 0x03B00000 + i;
		rc = amd_smn_read(0, addr, &val);
		if (rc == 0 && val != 0 && val != 0xFFFFFFFF)
			pr_info("psd: SMN[0x%08X] = 0x%08X\n", addr, val);
	}

	/* ===== TRY WRITING CTRL1 VIA SMN? ===== */
	/* The CTRL1 register (MMIO 0x0C) is read-only from MMIO.
	 * Maybe its value comes from an SMN shadow register.
	 * SMN offset 0x03B10xxx might contain the SPI controller registers. */
	pr_info("psd:\n");
	pr_info("psd: --- CTRL1 investigation: MMIO writes to 0x0C ---\n");
	{
		u32 before = ioread32(base + 0x0C);
		iowrite32(0x020006B5, base + 0x0C);
		u32 after = ioread32(base + 0x0C);
		pr_info("psd: CTRL1 before=0x%08x after_write=0x%08x (delta=0x%08x)\n",
			before, after, before ^ after);
		if (before == after)
			pr_info("psd: CTRL1 write IGNORED (read-only)\n");
		else
			pr_info("psd: CTRL1 write ACCEPTED!\n");
	}

	/* Try CTRL1 at various SMN offsets that might shadow it */
	{
		u32 smn_offsets[] = { 0x03B1000C, 0x03B10010, 0x03B10020, 0x03B10040, 0x03B10060, 0 };
		pr_info("psd: CTRL1 via SMN scan:\n");
		for (i = 0; smn_offsets[i]; i++) {
			rc = amd_smn_read(0, smn_offsets[i], &val);
			if (rc == 0)
				pr_info("psd:   SMN[0x%08X] = 0x%08X\n", smn_offsets[i], val);
		}
	}

	iounmap(base);
	pr_info("psd: ============================================\n");
	pr_info("psd: PCI + SMN DIAGNOSTIC COMPLETE\n");
	pr_info("psd: ============================================\n");
	return -ENODEV;
}

module_init(psd_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PCI config + SMN diagnostic for SL4A");
