#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;

module_param(count, uint, S_IRUGO);
MODULE_PARM_DESC(count, "count of repeats hello world");

struct hello_data {
	struct list_head tlist;
	ktime_t time;
};

static struct list_head list;
static struct hello_data *data;

static int __init hello_init(void)
{
unsigned int i;

if (count == 0) {
	pr_info("The parameter value is equal to 0.\n");
} else if (count >= 5 && count <= 10) {
	pr_info("The parameter value is between 5 and 10.\n");
} else if (count > 10) {
	pr_err("Error. The parameter value is greater than 10.\n");
	BUG_ON(count > 10);
	return -EINVAL;
}

INIT_LIST_HEAD(&list);

for (i = 0; i < count; i++) {
	struct hello_data *data = 0;

	if (i != count - 1)
		data = kmalloc(sizeof(*data), GFP_KERNEL);

	if (!data) {
		pr_err("Something went wrong with allocating memory.\n");
		BUG_ON(!data);
}

	data->time = ktime_get();
	INIT_LIST_HEAD(&data->tlist);
	list_add_tail(&data->tlist, &list);

	pr_info("Hello, world!\n");
}
return 0;
}

static void __exit hello_exit(void)
{
unsigned int i;

struct hello_data *tmp;
for (i = 0; i < count; i++) {
	list_for_each_entry_safe(data, tmp, &list, tlist) {

		s64 nanoseconds = ktime_to_ns(data->time);

		pr_info("Time in nanoseconds: %lld\n", nanoseconds);
		list_del(&data->tlist);
		kfree(data);
	}
}
}

module_init(hello_init);
module_exit(hello_exit);
