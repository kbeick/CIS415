/*
:author: Kevin Beick
Proj1 : CIS415 : S14
*/

#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage int sys_helloworld() {
    printk(KERN_EMERG "hello world!");
    return 1;
}