loadSetupFormat = """
.set SETUP_FILE_ADDRESS, {setupFileAddress}


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

    #gecko injection
    #inject to 0x8002d4f8
    .int 0xC202d4f8
    .int (loadSetupFile_size + 4) / 8
    
loadSetupFile:

    SAVE_REGS
    
    .set setupFileName_offset, setupFileName-. - 4
    .set openMode_offset, openMode-. - 4
    #hack to get current address
    bl _ADDRESS
_ADDRESS:
    mflr r4
    addi r3, r4, setupFileName_offset
    addi r4, r4, openMode_offset

    #open file
    #lis r3, setupFileName@ha
    #ori r3, r3, setupFileName@l
    #lis r4, openMode@ha
    #ori r4, r4, openMode@l
    lis r7, 0x803e
    ori r7, r7, 0xbeb8
    mtctr r7
    #call fopen
    bctrl

    #read file
    mr r31, r3
    lis r3, SETUP_FILE_ADDRESS@ha
    ori r3, r3, SETUP_FILE_ADDRESS@l
    li r4, 1
    lis r5, 2
    mr r6, r31
    lis r7, 0x803e
    ori r7, r7, 0xbee4
    mtctr r7
    #call fread
    bctrl

    #close file
    mr r3, r31
    lis r7, 0x803e
    ori r7, r7, 0xbe8c
    mtctr r7
    #call fopen
    bctrl

    #call setup function from file
    #ptr to it at begining of file
    lis r3, SETUP_FILE_ADDRESS@ha
    lwz r3, SETUP_FILE_ADDRESS@l(r3)
    mtctr r3
    bctrl

    RESTORE_REGS
    mflr 0
    b END


    setupFileName:
    .asciz "setup.bin"
openMode:
    .asciz "r"


    	.globl saveRegs
    	.type saveRegs, @function
    saveRegs:
    	stw 0, 4(1)
    	mfctr 0
    	stw 0, -8(1)

    	stwu 1, -248(1)
    	stmw 2, 8(1)

    	blr

    	.size saveRegs, .-saveRegs


    	.globl restoreRegs
    	.type restoreRegs, @function
    restoreRegs:
    	lmw 2, 8(1)
    	addi 1, 1, 248

    	lwz 0, -8(1)
    	mtctr 0
    	lwz 0, 4(1)

    	blr

    	.size restoreRegs, .-restoreRegs

END:
    nop
    .int 0


    .equ loadSetupFile_size, .-loadSetupFile
"""


def makeLoadSetupFile(filePath, setupFileAddress):
    with open(filePath, "w") as file:
        loadSetupText = loadSetupFormat.format(setupFileAddress=setupFileAddress)
        file.write(loadSetupText)
