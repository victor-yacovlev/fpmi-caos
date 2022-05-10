int trace_some_func(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();
  bpf_trace_printk("Running traccee for PID = %d", pid);
  return 0;
}
