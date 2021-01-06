OUTPUT_FORMAT("coff-sh")
OUTPUT_ARCH(sh)

SECTIONS
{
	.entry 0x06010000:
	{
		*(.entry)
		_eentry = .;
	}
	.text ALIGN(0x20) :
	{
		* (.text)
		*(.strings)
		_etext = .;
	}
	.tors  ALIGN(0x10) :
	{
		___ctors = . ;
		*(.ctors)
		___ctors_end = . ;
		___dtors = . ;
		*(.dtors)
		___dtors_end = . ;
	}
	.data ALIGN(0x10):
	{
		* (.data)
		_edata = . ;
	}

	.bss ALIGN(0x10) (NOLOAD):
	{
		_bss_start = . ;
		*(.bss)
		* ( COMMON )
		_bss_end = . ;
		_end = .;
 	}
	.stack ALIGN(0x10) (NOLOAD):
	{
	*(.stack)
	 _stack = . ;
	}
}
