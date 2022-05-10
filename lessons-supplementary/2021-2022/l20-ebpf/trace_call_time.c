BPF_HASH(start_times, u64);

int start_tracing(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 start = bpf_ktime_get_ns();
    start_times.update(&pid, &start);
    return 0;
}

int end_tracing(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 *start = start_times.lookup(&pid);
    /*if (0 == start) {
      return 0;
    }*/
    u64 now = bpf_ktime_get_ns();
    u64 delta = now - *start;
    bpf_trace_printk("Function call time: %d", delta);
    return 0;
}
