#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_v2p(void)
{
	int virtual;
	int *physical;
	
	
  	if(argint(0,&virtual)<0) return -1;
  	if(argptr(1,(char**)&physical, sizeof(int*))<0) return -1;
	
	return v2p(virtual, physical);
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int exit_status;

  if(argint(0, &exit_status) < 0)
    return -1;
  if(argptr(0,(char **) &exit_status, 4)) return -1;
  exit(exit_status);
  
  return 0;
}


int
sys_wait(void)
{
	/*
  int val;
  int *value = &val;
  int *exit_status = (int*) (&value);;
	if(argptr(0,(char**) value,4)<0)
	return -1;
	if(argptr(0, (void*)&exit_status, sizeof(int*))<0)
	return -1;
  return wait(exit_status); // */
  
  int *exit_status;
  if(argptr(0, (char**)&exit_status, sizeof(int)) < 0) return -1;
  return wait(exit_status);
}

int
sys_waitpid(void)
{
  int pid;
  int *exit_status;
  int options;
  
  //if(argint(0, &pid) < 0)
	//return -1;
  //if(argptr(0, (void*)&exit_status, sizeof(int*))<0)
  // return -1;	
  	if(argint(0,&pid)<0) return -1;
	if(argptr(1,(char**)&exit_status, sizeof(int*))<0) return -1;
	if(argint(2, &options)<0) return -1;
	//argptr(0,(char**) &pid, 4); //---------------------################
	//argptr(1,(char**) &exit_status, 4); //---------------------################
	//argptr(2,(char**) &options, 4); //---------------------################
	
  return waitpid(pid,exit_status,options);
}

int
sys_setpriority(void) 
{
	int priority;
	
	//argptr(0, (char**)&priority, 0);
	if(argint(0, &priority)<0) return -1;
    
	return setpriority(priority);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

void 
sys_hello(void)
{
    return hello();
}
// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


