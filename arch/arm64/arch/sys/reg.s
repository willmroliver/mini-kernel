.macro SYS_REG_GET_SET SR

.global sys_reg_read_\SR
sys_reg_read_\SR:
	mrs x0, \SR
	ret

.global sys_reg_write_\SR
sys_reg_write_\SR:
	msr \SR, x0
	ret

.global sys_reg_isb_write_\SR
sys_reg_isb_write_\SR:
	msr \SR, x0
	isb
	ret

.global sys_reg_orr_\SR
sys_reg_orr_\SR:
	mrs x1, \SR
	orr x1, x1, x0
	msr \SR, x1
	ret

.global sys_reg_isb_orr_\SR
sys_reg_isb_orr_\SR:
	mrs x1, \SR
	orr x1, x1, x0
	msr \SR, x1
	isb
	ret

.global sys_reg_and_\SR
sys_reg_and_\SR:
	mrs x1, \SR
	and x1, x1, x0
	msr \SR, x1
	ret

.global sys_reg_isb_and_\SR
sys_reg_isb_and_\SR:
	mrs x1, \SR
	and x1, x1, x0
	msr \SR, x1
	isb
	ret

.endm

.text

SYS_REG_GET_SET CPACR_EL1
