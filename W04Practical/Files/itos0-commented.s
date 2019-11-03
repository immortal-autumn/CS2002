	.text # Defines current section as text (Not data)
	.file	"itos.c" # The code is from "itos.c"
	.globl	itos # identifier in partial program is itos
	.p2align	4, 0x90
	.type	itos,@function # declares type of itos: function
itos: # Name for place in program: "itos"
	.cfi_startproc # Used for debugging
	pushq	%rbp # Saving old rbp and setting base pointer of new stack frame
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp # %rbp = stack frame of the function
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp # %32 bytes reserved on stack for local variables
	movq	%rdi, -8(%rbp) # local argument val = %rdi
	cmpq	$0, -8(%rbp) # substract val with 0
	jne	.LBB0_2 # See whether the comparison is not equal to 0 and if not equal jump to lable ".LBB0_2"
	movl	$2, %eax # %eax = 2
	movl	%eax, %edi # %edi = %eax
	callq	malloc # call function "malloc"
	movq	%rax, -16(%rbp) # local argument *start = %rax
	movq	-16(%rbp), %rax # %rax = *start
	movb	$48, (%rax) # %rax = 48
	movq	-16(%rbp), %rax # %rax = *start
	addq	$1, %rax # %rax += 1
	movq	%rax, -24(%rbp) # local argument *end = %rax
	jmp	.LBB0_3 # jump to label ".LBBO_3"
.LBB0_2:
	xorl	%esi, %esi # %esi = 0
	movq	-8(%rbp), %rdi # %rdi = val
	leaq	-24(%rbp), %rdx # %rdx = *end
	callq	itos_recur # Pushes address of "itos_recur" on stack and transfers control to "itos_recur"
	movq	%rax, -16(%rbp) # *start = %rax
.LBB0_3:
	movq	-24(%rbp), %rax # %rax = *end
	movb	$0, (%rax) # %rax = 0
	movq	-16(%rbp), %rax # %rax = *start
	addq	$32, %rsp # %rsp += 32
	popq	%rbp # restore rbp before return
	.cfi_def_cfa %rsp, 8
	retq # Pops address from stack.
.Lfunc_end0:
	.size	itos, .Lfunc_end0-itos # size of "itos", used by debugger
	.cfi_endproc

	.p2align	4, 0x90
	.type	itos_recur,@function # declares types of "itos_recur"
itos_recur:
	.cfi_startproc
	pushq	%rbp # Saving old rbp and setting base pointer of new stack frame
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp # rbp = rsp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp # 64 bytes reserved on stack for local variables
	movl	$10, %eax # %eax = 10 
	movl	%eax, %ecx # %ecx = %eax
	movq	%rdi, -8(%rbp) # local argument val = %rdi
	movl	%esi, -12(%rbp) # local argument depth = %esi
	movq	%rdx, -24(%rbp) # local argument end = %rdx
	/* In this case, %rbp - 8 to %rbp - 24 are addresses of local arguments and will be used to store arguments*/
	movq	-8(%rbp), %rax # %rax =  val
	cqto # sign extend %rax to val:%rax
	idivq	%rcx # %rcx = %rax / %eax
	movq	%rax, -32(%rbp) # local variable quot = %rax
	movq	-8(%rbp), %rax # %rax = val
	cqto # sign extend %rax to %rbp - 8:%rax
	idivq	%rcx # %rcx = %rax % %eax
	movq	%rdx, %rdi # %rdx = %rdi
	callq	labs # call function "labs"
	movq	%rax, -40(%rbp) # local argument mod = %rax
	cmpq	$0, -32(%rbp) # Subtract 0 from quot
	je	.LBB1_2 # jump to ".LBB1_2" if comparison result is 0
	movq	-32(%rbp), %rdi # %rdi = quot
	movl	-12(%rbp), %eax # %eax = depth
	addl	$1, %eax # %eax += 1
	movq	-24(%rbp), %rdx # %rdx = end
	movl	%eax, %esi # %esi = %eax
	callq	itos_recur # call function "itos_recur"
	movq	%rax, -48(%rbp) # local variable start = rax
	jmp	.LBB1_5 # jump to label ".LBB1_5"
.LBB1_2:
	movl	-12(%rbp), %eax # %eax = depth
	movq	-8(%rbp), %rcx # %rcx = val
	cmpq	$0, %rcx % # substract 0 from %rcx
	movl	$3, %edx # %edx = 3
	movl	$2, %esi # %esi = 2
	cmovll	%edx, %esi # if compare gets 0 then %esi = %edx
	addl	%esi, %eax # %eax += %esi
	movl	%eax, -52(%rbp) # local variable len = %eax
	movslq	-52(%rbp), %rdi # %rdi = len
	callq	malloc # call function "malloc"
	movq	%rax, -48(%rbp) # *start = %rax
	movq	-48(%rbp), %rax # %rax = *start
	movq	-24(%rbp), %rcx # %rcx = *end
	movq	%rax, (%rcx) # %rcx = %rax
	cmpq	$0, -8(%rbp) # substract 0 from val
	jge	.LBB1_4 # jump if greater or equal
	movq	-24(%rbp), %rax # %rax = *end
	movq	(%rax), %rcx # %rcx = %rax
	movq	%rcx, %rdx # %rdx = %rcx
	addq	$1, %rdx # %rdx += 1
	movq	%rdx, (%rax) # %rax = %rdx
	movb	$45, (%rcx) # %rcx = 45
.LBB1_4:
	jmp	.LBB1_5 # jump to label ".LBB1_5"
.LBB1_5:
	movq	-40(%rbp), %rax # %rax = mod
	addq	$48, %rax # %rax += 48
	movb	%al, %cl # %cl = %al
	movq	-24(%rbp), %rax # %rax = *end
	movq	(%rax), %rdx # %rdx = %rax
	movq	%rdx, %rsi # %rsi = %rdx
	addq	$1, %rsi # %rsi += 1
	movq	%rsi, (%rax) # %rax = %rsi
	movb	%cl, (%rdx) # %rdx = %cl
	movq	-48(%rbp), %rax # %rax = *start
	addq	$64, %rsp # %rsp += 64
	popq	%rbp # restore %rbp before return
	.cfi_def_cfa %rsp, 8
	retq # pop value from stack
.Lfunc_end1:
	.size	itos_recur, .Lfunc_end1-itos_recur
	.cfi_endproc


	.ident	"clang version 7.0.1 (Fedora 7.0.1-2.fc29)"
	.section	".note.GNU-stack","",@progbits 
	.addrsig
	.addrsig_sym itos
	.addrsig_sym malloc
	.addrsig_sym itos_recur
	.addrsig_sym labs
