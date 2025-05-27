set pagination off
set confirm off
set disassemble-next-line on
set listsize 20
target remote localhost:1234
break kernel_main
layout src
continue
