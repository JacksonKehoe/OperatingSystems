/*+
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){


    int numChars;
    int vsz;
    int rss;
    int pageSize = PAGE_SIZE >> 10;

    numChars = sprintf(page, "PID\t UID\t VSZ\t RSS\n");

    if (fpos == 0){
        //numChars = sprintf(page,"Hello");
        //numChars += sprintf(page + numChars, " World\n");
	    // write headers
        //numChars = sprintf(page, "PID\t UID\t VSZ\t RSS\n");
	    // find first task
        // write first task
        // advance to next task
         theTask = &init_task;
         
         while(theTask->pid == 0){
            theTask = theTask->next_task;
         }
            firstTask = theTask;

         numChars += sprintf(page + numChars, "%d\t %d\t", theTask->pid, theTask->uid);

         int pageSize = PAGE_SIZE >> 10;

         if(theTask->mm == NULL){
             vsz = 0;
             rss = 0;
         }
         else{
             vsz = (theTask->mm->total_vm) * pageSize;
             rss = (theTask->mm->rss) * pageSize;
         }

         numChars += sprintf(page + numChars, "%d\t %d\n", vsz, rss);

         do {
             theTask = theTask->next_task;
         }
         while (theTask->pid == 0);
        
    } else {
        //if (at back at begining of list){
        if (theTask == firstTask){
            *eof = 0;
            *start = page;
            return 0;
            
        }
        
        //}
	    // write task info for one task
	    // advance to next task
        numChars = sprintf(page, "%d\t %d\t", theTask->pid, theTask->uid);

         if(theTask->mm == NULL){
             vsz = 0;
             rss = 0;
         }
         else{
             vsz = (theTask->mm->total_vm) * pageSize;
             rss = (theTask->mm->rss) * pageSize;
         }

         numChars += sprintf(page + numChars, "%d\t %d\n", vsz, rss);

         do {
             theTask = theTask->next_task;
         }
         while (theTask->pid == 0);
    }
    *eof = 1;
    *start = page;
    return numChars;
}

int init_module(){
   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   if (proc_entry == NULL){
       return -1;
   }
   else{
       proc_entry->read_proc = my_read_proc;
        return 0;
    }
}

void cleanup_module(){
    remove_proc_entry("lab2", NULL);
}
