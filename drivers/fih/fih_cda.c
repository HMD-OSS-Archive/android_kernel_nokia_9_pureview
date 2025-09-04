#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/of_device.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <fih/share/cda.h>

#define FIH_CDA_FILE_PATH    MANUF_FILE_LOCATION
#define FIH_CDA_FILE_OFFSET  0x00001000
#define FIH_CDA_FILE_SIZE    0x00001000

/* used for function */
#define FIH_CDA_KERN_USER  (0)
#define FIH_CDA_KERN_ROOT  (1)

/* used for manufacture */
#define FIH_CDA_STAT_USER  (0)
#define FIH_CDA_STAT_ROOT  (1)

static char skuid[16];
static char status[16];

static unsigned int fih_cda_update(char *str_status)
{
	struct file *fp= NULL;
	mm_segment_t oldfs;
	char tmp[16] = {0};
	int len = sizeof(tmp);
	char *path = FIH_CDA_FILE_PATH;
	unsigned int offset = FIH_CDA_FILE_OFFSET;
	struct manuf_data *buf;
	unsigned int new_status;
	unsigned int old_status;
	loff_t pos = 0;

	strncpy(tmp, str_status, sizeof(tmp));

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fp = filp_open(path, O_RDWR|O_NONBLOCK, 0);
	if (IS_ERR(fp)) {
		pr_err("%s: open fail %s\n", __func__, path);
		set_fs(oldfs);
		return 1;
	}

	if (fp->f_op == NULL) {
		pr_err("%s: fp->f_op is NULL\n", __func__);
		set_fs(oldfs);
		return 2;
	}

	buf = kmalloc(sizeof(struct manuf_data), GFP_KERNEL);

	if (fp->f_op->llseek == NULL) {
		pr_err("%s: fp->f_op->llseek is NULL\n", __func__);
		set_fs(oldfs);
		return 3;
	}
	fp->f_op->llseek(fp, offset, 0);

	pos = fp->f_pos;
	len = vfs_read(fp, (unsigned char __user *)buf, sizeof(struct manuf_data), &pos);
	old_status = buf->rootflag.status;
	pr_info("%s: status = 0x%08x (old)\n", __func__, old_status);

	new_status = simple_strtoull(tmp, NULL, len);
	switch (new_status) {
		case FIH_CDA_KERN_USER:
			buf->rootflag.status = FIH_CDA_STAT_USER;
			break;
		case FIH_CDA_KERN_ROOT:
			buf->rootflag.status = FIH_CDA_STAT_ROOT;
			break;
		default:
			buf->rootflag.status = FIH_CDA_STAT_ROOT;
			break;
	}

	pr_info("%s: status = 0x%08x (new)\n", __func__, buf->rootflag.status);
	if (old_status == buf->rootflag.status){
		pr_info("%s: status : stay the same !!!\n", __func__);
	} else {
		fp->f_op->llseek(fp, offset, 0);
		pos = fp->f_pos;
		len = vfs_write(fp, (unsigned char __user *)buf, sizeof(struct manuf_data), &pos);
	}

	filp_close(fp, NULL);
	set_fs(oldfs);
	kfree(buf);

	return 0;
}

ssize_t fih_cda_proc_write_status(struct file *file, const char __user *buffer,
	size_t count, loff_t *ppos)
{
	unsigned char temp[16];
	unsigned int size;
	unsigned int i;

	size = (count >= sizeof(temp))? (sizeof(temp) - 1) : count;
	memset(temp, 0, sizeof(temp));

	if (copy_from_user(temp, buffer, size)) {
		pr_err("%s: copy_from_user fail\n", __func__);
		return -EFAULT;
	}

	/* filter abnormal character */
	for (i = 0; i < sizeof(temp); i++) {
		//if (temp[i] == 0x0A) continue;
		if ((temp[i] < 0x20)||(0x7E < temp[i])) temp[i] = 0x00;
	}
	temp[size] = 0x00;  /* end at input size */

	memset(status, 0, sizeof(status));
	snprintf(status, sizeof(status), "%s", temp);
	status[sizeof(status)-1] = 0x0;  /* avoid endless */

	/* write into deviceinfo partition */
	fih_cda_update(status);

	return size;
}

static int fih_cda_proc_read_status(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", status);
	return 0;
}

static int fih_cda_proc_open_status(struct inode *inode, struct file *file)
{
	return single_open(file, fih_cda_proc_read_status, NULL);
}

static const struct file_operations fih_cda_fops_status = {
	.open    = fih_cda_proc_open_status,
	.read    = seq_read,
	.write   = fih_cda_proc_write_status,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int fih_cda_proc_read_skuid(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", skuid);
	return 0;
}

static int fih_cda_proc_open_skuid(struct inode *inode, struct file *file)
{
	return single_open(file, fih_cda_proc_read_skuid, NULL);
}

static const struct file_operations fih_cda_fops_skuid = {
	.open    = fih_cda_proc_open_skuid,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int fih_cda_property(struct platform_device *pdev)
{
	int rc = 0;
	static const char *p_chr;

	p_chr = of_get_property(pdev->dev.of_node, "fih-cda,skuid", NULL);
	if (!p_chr) {
		pr_info("%s:%d, skuid not specified\n", __func__, __LINE__);
	} else {
		strlcpy(skuid, p_chr, sizeof(skuid));
		pr_info("%s: skuid = %s\n", __func__, skuid);
	}

	p_chr = of_get_property(pdev->dev.of_node, "fih-cda,status", NULL);
	if (!p_chr) {
		pr_info("%s:%d, status not specified\n", __func__, __LINE__);
	} else {
		strlcpy(status, p_chr, sizeof(status));
		pr_info("%s: status = %s\n", __func__, status);
	}

	return rc;
}

static int fih_cda_probe(struct platform_device *pdev)
{
	int rc = 0;

	if (!pdev || !pdev->dev.of_node) {
		pr_err("%s: Unable to load device node\n", __func__);
		return -ENOTSUPP;
	}

	rc = fih_cda_property(pdev);
	if (rc) {
		pr_err("%s Unable to set property\n", __func__);
		return rc;
	}

	proc_mkdir("cda", NULL);
	proc_create("cda/status", 0, NULL, &fih_cda_fops_status);
	proc_create("cda/skuid", 0, NULL, &fih_cda_fops_skuid);

	return rc;
}

static int fih_cda_remove(struct platform_device *pdev)
{
	remove_proc_entry ("cda/skuid", NULL);
	remove_proc_entry ("cda/status", NULL);

	return 0;
}

static const struct of_device_id fih_cda_dt_match[] = {
	{.compatible = "fih_cda"},
	{}
};
MODULE_DEVICE_TABLE(of, fih_cda_dt_match);

static struct platform_driver fih_cda_driver = {
	.probe = fih_cda_probe,
	.remove = fih_cda_remove,
	.shutdown = NULL,
	.driver = {
		.name = "fih_cda",
		.of_match_table = fih_cda_dt_match,
	},
};

static int __init fih_cda_init(void)
{
	int ret;

	ret = platform_driver_register(&fih_cda_driver);
	if (ret) {
		pr_err("%s: failed!\n", __func__);
		return ret;
	}

	return ret;
}
module_init(fih_cda_init);

static void __exit fih_cda_exit(void)
{
	platform_driver_unregister(&fih_cda_driver);
}
module_exit(fih_cda_exit);
