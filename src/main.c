#include "editor.h"
#include "array.h"

fint32
main(fint32 argc, fchar *argv[])
{
    editor_init(argv);

    if (argc >= 2)
    {
	editor_open(argv[1]);
    }

    while(1)
    {
	editor_update();
    }

    /* DARRAY(int) arr1 = DARRAY_INIT(int, 10); */
    /* printf("size %u\n", DARRAY_SIZE(arr1)); */
    /* printf("cap %u\n", *darray_cap(arr1)); */
    /* printf("es %u\n", *darray_es(arr1)); */

    /* for(int i=0; i<DARRAY_SIZE(arr1); i++) */
    /* { */
    /* 	printf("%d = %d\n", i, arr1[i]); */
    /* } */
    
    /* DARRAY_FREE(arr1); */
    /* return 0; */
}
