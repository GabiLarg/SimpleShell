/*Author: Gabriela Pereira de Assis
Date: 09-18-2014*/
/*Last modified: 09-27-2014*/
#include "getword.h"

int getword(char* s){
	char c;
	int hasChar=0;
	int count =0;
	int i=0;
	while(i<STORAGE){
		*s = '\0';
		i++;
	}	
	
	c='\0';
	while((c=getchar())!=EOF){
		
		if(c=='\\'){
			/*get next character*/
			c=getchar();
			if(c=='>'|| c=='<'||c=='&'||c=='|'||c==' '||c=='\\'){
				/*if it gets a metacharacter concatenate do the string*/
				*s=c;
				s++;
				count++;
			}/*end iff <metacharacter>*/
			else{
				ungetc(c,stdin);
				if(hasChar){/*verify if there are characters caught*/
					*s='\0';
					return count;
				}/*if hasChar*/
			}/*end else <metacharacter>*/
					
		}/*end iff \*/
		
		else{
		
			if(c=='>'||c=='<'||c=='&'||c=='|'){
				if(!hasChar){
					*s=c;
					count++;
					if(c=='>'){
						c=getchar();
						if(c=='>'){ 
							s++;
							*s=c;
							s++;						
							*s='\0';
							count++;
							return count;
						}/*end iff >*/
						else{
							s='\0';
							ungetc(c,stdin);
							return count;
						}/*end else >*/
					}else{
						s++;
						s='\0';
						return count;	
					}
				} /*end iff !hasChar*/
				else{
					*s='\0';
					ungetc(c,stdin);
					return count;
				}/*end else !hasChar*/
			}/*end iff <metacharacter>*/
			else{
				if( c==';'|| c=='\n'){
					if(hasChar!=0){
						*s='\0';
						ungetc(c, stdin);
						return count;
						
					}else{
						return count;
					}
				}else
				if(c==' '){
					if(hasChar!=0){
						*s='\0';
						return count;
					}
					
				}/*end iff space or end of line*/
				
	
				else{
					*s=c;
					s++;
					count++;
					hasChar=1;
				}
				
			}/*end else <metacaracter>*/
		}/*end else \*/
		
	}/*end while*/
	
	return -1;
		
}
