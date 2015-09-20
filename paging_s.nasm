global vm_page_inval
vm_page_inval:
	mov eax, cr3
	mov cr3, eax
	ret