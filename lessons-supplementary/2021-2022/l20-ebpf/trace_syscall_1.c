#include <uapi/linux/ptrace.h>

int trace_execv(struct pt_regs *ctx) {
  char cmd[16];
  bpf_get_current_comm(&cmd, sizeof(cmd));
  // filter for command
  if (! (cmd[0]=='b' && cmd[1]=='a' && cmd[2]=='s' && cmd[3]=='h' && cmd[4]=='\0')) {
    return 0;
  }
  
  bpf_trace_printk("%s tried to exec", cmd);

  bpf_override_return(ctx, 1);
  
  return 0;
}
