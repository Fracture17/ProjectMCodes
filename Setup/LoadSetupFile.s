.section .text.loadSetupFile
   	.globl loadSetupFile
   	.type loadSetupFile, @function

   	.set sp, 1;  .set rtoc, 2;
    .set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
    .set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
    .set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
    .set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
    .set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
    .set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
    .set r30,30; .set r31,31;
    .set f0,0;   .set f1,1;   .set f2,2; .set f3,3;   .set f4,4
    .set f5,5;   .set f6,6;   .set f7,7;   .set f8,8;   .set f9,9
    .set f10,10; .set f11,11; .set f12,12; .set f13,13; .set f14,14
    .set f15,15; .set f16,16; .set f17,17; .set f18,18; .set f19,19
    .set f20,20; .set f21,21; .set f22,22; .set f23,23; .set f24,24
    .set f25,25; .set f26,26; .set f27,27; .set f28,28; .set f29,29
    .set f30,30; .set f31,31;

    	.macro SAVE_REGS
    	stw r0, -4(sp)
    	mflr r0
    	bl saveRegs
    .endm
    	.macro RESTORE_REGS
    	bl restoreRegs
    	mtlr r0
    	lwz r0, -4(sp)
    .endm

loadSetupFile:
setupFileName:
    .asciz "setup.bin"
openMode:
    .asciz "r"
    #gecko injection
    #inject to 0x8002d4f8
    .int 0xC202d4f8
    #size is constant 7 lines
    .int (loadSetupFile_size + 4) / 8
    SAVE_REGS
    #open file
    lis r3, setupFileName@ha
    ori r3, r3, setupFileName@l
    lis r4, openMode@ha
    ori r4, r4, openMode@l
    lis r7, 0x803e
    ori r7, r7, 0xbeb8
    mtctr r7
    #call fopen
    btctrl

    #read file
    mr r31, r3
    #These are macros to be defined when assembling
    SET_SETUP_FILE_ADDRESS
    SET_SETUP_FILE_SIZE
    li r5, 1
    mr r6, r31
    lis r7, 0x803e
    ori r7, r7, 0xbee4
    mtctr r7
    #call fread
    btctrl

    #close file
    mr r3, r31
    lis r7, 0x803e
    ori r7, r7, 0xbe8c
    mtctr r7
    #call fopen
    btctrl

    #call setup function from file
    SET_SETUP_FILE_ADDRESS
    mtctr r3
    btctrl

    RESTORE_REGS
    mflr 0
    b END


        .section .text.saveRegs
    	.globl saveRegs
    	.type saveRegs, @function
    saveRegs:
    	stw 0, 4(1)
    	mfctr 0
    	stw 0, -8(1)

    	stfd 0, -0x10(1)
    	stfd 1, -0x18(1)
    	stfd 2, -0x20(1)
    	stfd 3, -0x28(1)
    	stfd 4, -0x30(1)
    	stfd 5, -0x38(1)
    	stfd 6, -0x40(1)
    	stfd 7, -0x48(1)
    	stfd 8, -0x50(1)
    	stfd 9, -0x58(1)
    	stfd 10, -0x60(1)
    	stfd 11, -0x68(1)
    	stfd 12, -0x70(1)
    	stfd 13, -0x78(1)

    	stwu 1, -248(1)
    	stmw 2, 8(1)

    	blr

    	.size saveRegs, .-saveRegs


        .section .text.restoreRegs
    	.globl restoreRegs
    	.type restoreRegs, @function
    restoreRegs:
    	lmw 2, 8(1)
    	addi 1, 1, 248

    	lfd 0, -0x10(1)
    	lfd 1, -0x18(1)
    	lfd 2, -0x20(1)
    	lfd 3, -0x28(1)
    	lfd 4, -0x30(1)
    	lfd 5, -0x38(1)
    	lfd 6, -0x40(1)
    	lfd 7, -0x48(1)
    	lfd 8, -0x50(1)
    	lfd 9, -0x58(1)
    	lfd 10, -0x60(1)
    	lfd 11, -0x68(1)
    	lfd 12, -0x70(1)
    	lfd 13, -0x78(1)

    	lwz 0, -8(1)
    	mtctr 0
    	lwz 0, 4(1)

    	blr

    	.size restoreRegs, .-restoreRegs

END:
    nop
    nop
    nop


    .equ loadSetupFile_size, .-loadSetupFile