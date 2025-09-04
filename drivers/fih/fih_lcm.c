/*
 * Virtual file interface for FIH LCM, 20151103
 * KuroCHChung@fih-foxconn.com
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define FIH_PROC_PATH_AOD "AOD"
#define FIH_PROC_PATH_PANEL_ID "PanelID"
#define FIH_PROC_DIR_LCM0 "AllHWList/LCM0"
#define FIH_PROC_PATH_AOD_LP "setlp"
#define FIH_PROC_PATH_GLANCE "glance"
#define FIH_PROC_PATH_COLORMANAGER "cm"
#define FIH_PROC_PATH_HIGH_BRIGHTNESS_MODE "hbm"
#define FIH_PROC_FULL_PATH_COLORMANAGER "AllHWList/LCM0/cm"
#define FIH_PROC_FULL_PATH_HIGH_BRIGHTNESS_MODE "AllHWList/LCM0/hbm"
#define FIH_PROC_FULL_PATH_AOD "AllHWList/LCM0/AOD"
#define FIH_PROC_FULL_PATH_PANEL_ID "AllHWList/LCM0/PanelID"
#define FIH_PROC_FULL_PATH_AOD_LP "AllHWList/LCM0/setlp"
#define FIH_PROC_FULL_PATH_GLANCE "AllHWList/LCM0/glance"

#define FIH_PROC_PATH_AWER_CNT "awer_cnt"
#define FIH_PROC_PATH_AWER_STATUS "awer_status"
#define FIH_PROC_FULL_PATH_AWER_CNT "AllHWList/LCM0/awer_cnt"
#define FIH_PROC_FULL_PATH_AWER_STATUS "AllHWList/LCM0/awer_status"

void fih_awer_cnt_set(char *info);
void fih_awer_status_set(char *info);
#ifdef CONFIG_AOD_FEATURE
extern int fih_set_glance(int enable);
extern int fih_get_glance(void);
#endif
extern unsigned int dsi_enable_hbm(u32 type);
extern unsigned int dsi_get_hbm_state(void);

static int fih_lcm_show_glance(struct seq_file *m, void *v)
{
#ifdef CONFIG_AOD_FEATURE
    seq_printf(m, "%d\n", fih_get_glance());
#else
	seq_printf(m, "0\n");
#endif
    return 0;
}

static int fih_lcm_open_glance_settings(struct inode *inode, struct  file *file)
{
    return single_open(file, fih_lcm_show_glance, NULL);
}


static ssize_t fih_lcm_write_glance_settings(struct file *file, const char __user *buffer,
                    size_t count, loff_t *offp)
{
    char *buf;
    unsigned int res=0;

    if (count < 1)
        return -EINVAL;

    buf = kmalloc(count, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    if (copy_from_user(buf, buffer, count))
        return -EFAULT;
	pr_err("fih_lcm_write_glance_settings\n");

#ifdef CONFIG_AOD_FEATURE
    res = fih_set_glance(simple_strtoull(buf, NULL, 0));
#endif
    if (res < 0)
    {
        kfree(buf);
        return res;
    }

    kfree(buf);

    /* claim that we wrote everything */
    return count;
}
static struct file_operations glance_file_ops = {
    .owner   = THIS_MODULE,
    .write   = fih_lcm_write_glance_settings,
    .read    = seq_read,
    .open    = fih_lcm_open_glance_settings,
    .release = single_release
};



char fih_awer_cnt[32] = "unknown";
char fih_awer_status[32] = "unknown";

void fih_awer_cnt_set(char *info)
{
	strcpy(fih_awer_cnt, info);
}

static int fih_awer_cnt_read_proc(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", fih_awer_cnt);
	return 0;
}

static int fih_awer_cnt_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_awer_cnt_read_proc, NULL);
}

static struct file_operations awer_cnt_file_ops = {
  .owner   = THIS_MODULE,
	.open		= fih_awer_cnt_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

void fih_awer_status_set(char *info)
{
	strcpy(fih_awer_status, info);
}

static int fih_awer_status_read_proc(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", fih_awer_status);
	return 0;
}

static int fih_awer_status_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_awer_status_read_proc, NULL);
}

static struct file_operations awer_status_file_ops = {
  .owner   = THIS_MODULE,
	.open		= fih_awer_status_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};


static int fih_lcm_show_hbm(struct seq_file *m, void *v)
{
#ifdef CONFIG_DRM_DSI_LGD_EA9151_HBM
	seq_printf(m, "%d\n",dsi_get_hbm_state());
#else
	seq_printf(m, "0\n");
#endif
    return 0;
}

static int fih_lcm_open_hbm_settings(struct inode *inode, struct  file *file)
{
    return single_open(file, fih_lcm_show_hbm, NULL);
}


static ssize_t fih_lcm_write_hbm_settings(struct file *file, const char __user *buffer,
                    size_t count, loff_t *offp)
{
    char *buf;
    unsigned int res=0;

    if (count < 1)
        return -EINVAL;

    buf = kmalloc(count, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    if (copy_from_user(buf, buffer, count))
        return -EFAULT;
	buf[1]=0;
	pr_err("fih_lcm_write_hbm_settings=%s\n",buf);

#ifdef CONFIG_DRM_DSI_LGD_EA9151_HBM
    res = dsi_enable_hbm(simple_strtoul(buf, NULL, 16));
#endif
    if (res < 0)
    {
        kfree(buf);
        return res;
    }

    kfree(buf);

    /* claim that we wrote everything */
    return count;
}
static struct file_operations hbm_file_ops = {
    .owner   = THIS_MODULE,
    .write   = fih_lcm_write_hbm_settings,
    .read    = seq_read,
    .open    = fih_lcm_open_hbm_settings,
    .release = single_release
};

static int __init fih_proc_init(void)
{
    struct proc_dir_entry *lcm0_dir;
    lcm0_dir = proc_mkdir (FIH_PROC_DIR_LCM0, NULL);

    pr_debug("start to create proc/%s\n", FIH_PROC_PATH_AWER_CNT);
    if (proc_create(FIH_PROC_PATH_AWER_CNT, 0, lcm0_dir, &awer_cnt_file_ops) == NULL)
    {
        pr_err("fail to create /proc/%s\n", FIH_PROC_PATH_AWER_CNT);
    }

    pr_debug("start to create proc/%s\n", FIH_PROC_PATH_AWER_STATUS);
    if (proc_create(FIH_PROC_PATH_AWER_STATUS, 0, lcm0_dir, &awer_status_file_ops) == NULL)
    {
        pr_err("fail to create /proc/%s\n", FIH_PROC_PATH_AWER_STATUS);
    }

	pr_debug("start to create proc/%s\n", FIH_PROC_PATH_GLANCE);
	if (proc_create(FIH_PROC_PATH_GLANCE, 0, lcm0_dir, &glance_file_ops) == NULL)
	{
		pr_err("fail to create /proc/%s\n", FIH_PROC_PATH_GLANCE);
	}

	pr_debug("start to create proc/%s\n", FIH_PROC_FULL_PATH_HIGH_BRIGHTNESS_MODE);
	if (proc_create(FIH_PROC_PATH_HIGH_BRIGHTNESS_MODE, 0, lcm0_dir, &hbm_file_ops) == NULL)
	{
		pr_err("fail to create /proc/%s\n", FIH_PROC_FULL_PATH_HIGH_BRIGHTNESS_MODE);
	}

    return (0);
}

static void __exit fih_proc_exit(void)
{
    remove_proc_entry(FIH_PROC_FULL_PATH_AWER_CNT, NULL);
    remove_proc_entry(FIH_PROC_FULL_PATH_AWER_STATUS, NULL);
    remove_proc_entry(FIH_PROC_FULL_PATH_GLANCE, NULL);
}

module_init(fih_proc_init);
module_exit(fih_proc_exit);
