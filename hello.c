
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Natalia Maksymuk <natavaksumyk@gmail.com>");
MODULE_DESCRIPTION("Lab 6");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int param = 1;

module_param(param, uint, S_IRUGO);

MODULE_PARM_DESC(param, "Number of times 'Hello, World!' string will be printed");

struct event_list {
    struct list_head list;
    ktime_t e_time;
};

static struct list_head event_list_head;

static LIST_HEAD(event_list_head);

void new_event(void);

static int __init task(void)
{
    if (param == 0 || (param >= 5 && param <= 10)) {
        printk(KERN_WARNING "Parameter between 5 and 10 or 0");  
    }

    if (param > 10) {
        printk(KERN_ERR "Parameter is large");
        return -EINVAL;
    }
  
    int i;
    for (i = 0; i < param; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        new_event();
    }
  return 0;
}

static void __exit task_exit(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->e_time);
    list_del(&md->list);
    kfree(md);
  }
}

void new_event(void) {
    struct event_list *element = NULL;
    element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
    element->e_time = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}


module_init(task);
module_exit(task_exit);
