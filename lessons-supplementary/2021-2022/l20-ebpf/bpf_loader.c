#include <inttypes.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/bpf.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>


static const char License[] = "GPL";


int main() {
    int trace_bin_fd = open("bpf_program.bin", O_RDONLY);

    struct bpf_insn instructions[1000] = {};

    size_t trace_bin_size = read(trace_bin_fd, instructions, sizeof(instructions));
    close(trace_bin_fd);
    size_t instructions_count = trace_bin_size / 8;

    char error_log[4096] = {};
    
    union bpf_attr bpf_argument;
    bpf_argument.prog_type = BPF_PROG_TYPE_XDP;
    bpf_argument.insns = (uint64_t) instructions;
    bpf_argument.insn_cnt = instructions_count;
    bpf_argument.license = (uint64_t) License;
    bpf_argument.log_level = 2;
    bpf_argument.log_buf = (uint64_t) error_log;
    bpf_argument.log_size = sizeof(error_log);

    int bpf_fd = syscall(SYS_bpf, BPF_PROG_LOAD, &bpf_argument, sizeof(bpf_argument));

    if (-1 == bpf_fd) {
        perror("bpf failed");
        fprintf(stderr, "Program load failed: %s\n", error_log); _exit(1);
    }
}
