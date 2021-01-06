##############################################################
#
# Makefile Pattern Rules
#
# DO NOT DETABIFY (unless your make doesn't care about tabs)
# ... or BAD things will happen!!
#
##############################################################

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) $(INCLUDES) -o $@ $<

%.sc: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -S -o $*.sc $<

%.lst: %.sc
	$(AS) $(ASFLAGS) $(INCLUDES)  -ahld $< > $*.lst

%.lst: %.s
	$(AS) $(ASFLAGS) $(INCLUDES)  -ahld $< > $*.lst

