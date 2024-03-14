inline avoid int test(int a);

#pragma pcp generate function @config.generate.function
float b(float b){
	return b + 1;
}


#pragma pcp generate code="int a ${targets};" @config.generate.function
#pragma pcp generate code="wewewe;" @config.generate.function
