/**
 *  Jiazi Yi
 *
 * LIX, Ecole Polytechnique
 * jiazi.yi@polytechnique.edu
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"url.h"


int main()
{
	char* url= malloc(150*sizeof(char));
	strcpy(url, "www.polytechnique.edu////");
	url_info info;

	if(!url)
		exit_with_error("Please enter URL");

	parse_url(url, &info);

	print_url_info(info);

	return (EXIT_SUCCESS);
}


