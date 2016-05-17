#include "ref-count/sp_counted_base.h"

int main(int argc, char *argv[])
{
	sp_counted_base *b = sp_counted_new(0);
	return 0;
}
