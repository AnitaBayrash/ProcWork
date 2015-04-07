#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

int first = 0, second = 0;
char operation = '+';

ssize_t first_operand_write(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
	int i = 0, value = 0, sign_value = 1;
	
	if (buffer[0] == '-')
		sign_value = -1;
	else if (buffer[0] > '9' || buffer[0] < '0') {
		printk(KERN_INFO "First operator: Invalid parameter.\n");
		return count;
	} else {
		value = value * 10 + (buffer[0]-'0');	
	}

	for (i = 1; i < count - 1; ++i) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			printk(KERN_INFO "First operator: Invalid parameter.\n");
			return count;
		} else {
			value = value * 10 + (buffer[i]-'0');
		}
	}

	first = value * sign_value;
	return count;
}

ssize_t second_operand_write(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
	int i = 0, value = 0, sign_value = 1;
	
	if (buffer[0] == '-')
		sign_value = -1;
	else if (buffer[0] < '0' || buffer[0] > '9') {
		printk(KERN_INFO "Second operator: Invalid parameter.\n");
		return count;
	} else {
		value = value * 10 + (buffer[0]-'0');	
	}

	for (i = 1; i < count - 1; ++i) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			printk(KERN_INFO "Second operator:Invalid parameter.\n");
			return count;
		} else {
			value = value * 10 + (buffer[i]-'0');
		}
	}

	second = value * sign_value;
	return count;
}

ssize_t operation_write(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
	if (count > 2 || (buffer[0] != '+' && buffer[0] != '-' && buffer[0] != '*' && buffer[0] != '/')) {
		printk(KERN_INFO "Operation: Invalid operation.\n");
		return count;
	}

	operation = buffer[0];

	return count;
}

ssize_t result_read(struct file *filp, char *buffer, size_t count, loff_t *offp)
{
	int length, result = 0;
	char number[12];
	switch(operation){
	case '+':
		result = first + second;
		break;
	case '-':
                result = first - second;
                break;
	case '*':
                result = first * second;
                break;
	case '/':
                if (second != 0)
			result = first / second;
		else
			printk(KERN_INFO "Division by zero.\n");
                break;
	default:
		break;
	}
	length = sprintf(number, "%d\n", result);

	if (count < length)
		return -EINVAL;
	if (*offp >= length)
                return 0;
	if (*offp + count > length)
        	count = length - *offp;

	strcpy(buffer, number);
	*offp += length;

	return length;
}

static const struct file_operations first_operand_fops = {
	.owner = THIS_MODULE,
	.write = first_operand_write
};

static const struct file_operations second_operand_fops = {
	.owner = THIS_MODULE,
	.write = second_operand_write
};

static const struct file_operations operation_fops = {
	.owner = THIS_MODULE,
	.write = operation_write
};

static const struct file_operations result_fops = {
	.owner = THIS_MODULE,
	.read = result_read
};


static int __init initialize_proc(void)
{
	printk(KERN_INFO "Module loaded.\n");
	proc_create("first_operand_proc", 0666, NULL, &first_operand_fops);
	proc_create("second_operand_proc", 0666, NULL, &second_operand_fops);
	proc_create("operation_proc", 0666, NULL, &operation_fops);
	proc_create("result_proc", 0, NULL, &result_fops);
	return 0;
}

module_init(initialize_proc);

static void __exit exit_proc(void) 
{
	remove_proc_entry("first_operand_proc", NULL);
	remove_proc_entry("second_operand_proc", NULL);
	remove_proc_entry("operation_proc", NULL);
	remove_proc_entry("result_proc", NULL);
	printk(KERN_INFO "Module removed.\n");
}

module_exit(exit_proc);
